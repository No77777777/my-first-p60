/*===========================================================================
               U P L I N K   R A D I O   L I N K   C O N T R O L

DESCRIPTION
  This file provides the functions to process RLC Uplink logical channels.
  
EXTERNALIZED FUNCTIONS
  List any functions that are externalized in this file.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  List any initialization and sequencing that is required for this module.

Copyright (c) 2001-03 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

Copyright (c) 2004-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/rlc/vcs/rlcul.c_v   1.44   12 Jun 2002 18:44:10   tliou  $
$Header: //components/rel/wcdma.mpss/8.4.0/rlc/src/rlcul.c#2 $ $DateTime: 2020/01/24 03:56:36 $ $Author: pwbldsvc $


when      who     what, where, why
01/16/18   sg      Made changes for to log interlayer commands for ML
--------  ---     ----------------------------------------------------------
12/26/16  nk      Fixed KW Errors 
12/15/16  nk      Fixed KW errors
05/07/16  kv      Fix to skip force rlc reset optimization for loopback call
06/03/16  kv      Optimization to trigger RLC UNREC ERROR if PDUs transmitted in UL with at least one PDU having POLL BIT set to TRUE, 
                  if there is no successful DL Scheduling for sufficiently large time X
05/11/16  as      RLC to clear pending partials correctly during RLC RESET.
05/06/16  kv      Merge error - add back wm_reg_success indication to DS
04/22/16  as      Remove excessive F3s
04/22/16  scv     Migrate RLC code to new requirements for DSM Priority Field
04/22/16  kc      Separting out SI timers for grant and no grant cases
04/13/16  as      Fix for EUL ISR delay crash due to MAC reconfig on other sub
03/28/16  kc      Adding changes for WM register/deregister confirmation
02/26/16  as      0x430F logging issue due to conncurrency issue b/w 0x430F logging for secondary 
                  carrier in L2 UL task and PDU building for primary carrier in EUL ISR context.
02/19/16  as      L2 F3 reduction.
12/28/15  scv     L2 QSH Fixes
12/23/15  kc      Disable QSH RLC RESET event by default
12/22/15  kc      Clear CTRL PDU built flag every time R99 PDU building is done
12/10/15  kc      Avoid taking lock on EUL ISR during ULC enable/disable
12/03/15  scv     Optimizations to avoid expensive operations to the Dequeued Chain as part of Multiple EnQueue/DeQueue
10/28/15  aa      Adding events and handler support to QSH
10/16/15  as      Added code changes to verify PDU size selected by MAC only if at least 1 complete PDU built.
08/31/15  as      Replace WL2 usage of dsm_pkt_len  with dsm_length_packet API
08/27/15  kc      Purge old/partial CCCH data only as part of PDU building.
08/20/15  as      RLC to register/de-register non-empty call back function for Watermark queue through DSM API
08/18/15  kv      To break out of DACC - ISR and release DACC mutex, if datapath flag is TRUE
08/18/15  as      RLC optimization to use 1 Large DSM items instead of 26 during R99 UL PDU building.
07/29/15  kv      Race condition during RAB config and check pending SRB data
07/20/15  scv     Changes for DSM Multiple DeQ/EnQ
07/16/15  aa      L2 to pass/process sub-id correctly when DUAL_WCDMA feature is disabled.
06/26/15  scv     Move global to .c & F3 from MSG_HIGH to MSG_LOW
06/26/15  aa      Removing extern declarations from L2 code
06/09/15  kc      Pass as_id for data indications in FACH & PCH
06/03/15  aa      Fixed KW errors
06/01/15  scv     QSH: Changes to support SYNC_OPTIONAL for multiple metric config
06/01/15  scv     Code changes to support QSH
06/01/15  as      In FACH state MAC should send CMAC_STATUS_IND only for TM CCCH UL data.
06/01/15  kc      Remember Aborted Status PDU on HS-RACH
05/26/15  kv      To enable BFI if data is present in WM after Reset Ack is received
05/06/15  as      Added code change at RLC to report buffer occupancy for re-transmission independently for each flexible size 
                  configured RB instead of reporting maximum of 63 PDU's to be re-transmitted for all RB's.
05/26/15  aa      RLC optimization to trigger Status PDUs on all AM channels after LTA
05/22/15  kv      PDU log packet enahncements in UL and DL (CR - 746106)
05/22/15  scv     WL2 Compiler Warnings Fix
05/20/15  aa      W+W and new interface changes for CFCM
05/01/15  scv     W+W Changes to have log packets support SubID
04/29/15  scv     FR 25842: RLC RTT & Queuing Delay Computation
04/28/15  kc      Fix to prevent concurrency issue between ULC ISR & WM Purge request
04/25/15  scv     W+W Changes to have messages support SubID
04/10/15  kv      Moved Qxdm Support for MAC debug F3s from L1 UL to MAC as part of WpW cleanup
04/08/15  as      L2-RRC interdependent changes for W+W
03/19/15  aa      RRC-L2 interface changes for passing AS ID as part of L2 commands
03/10/15  aa      Duplicated L2 task related globals and removed AS_ID hardcodings
03/04/15  aa      Replaced RLC UM/AM/TM config log packets with new packets
02/28/15  rsr     W + W Phase 2 Checkin.
02/23/15  kv      Enable 0x4168/0x4169 logging when turned during a call and print ciphering params if wrong LI
02/19/15  scv     Update HFN to account for roll-over before updating count_c
02/19/15  kc      Fix to prevent concurrency issue between ULC ISR & WM De-registration
02/09/15  aa      SUB_ID changes for L2-RRC interface
02/04/15  kv      Fix KW errors from W+W
01/29/15  aa      L2 global reorg for W+W
01/15/15  vr      Wrapping up some UL globals used by L2 under MACROs
12/05/14  aa      Fix to set poll_next to TRUE whenever partials with poll bit set are cleared
11/25/14  as      Fix to RESET the PDU logging flag when num_pdu = 0
11/06/14  aa      Fix to prevent deadlock between FC mutex and R99 datapath mutex
10/27/14  kv      Fix Bolt Compiler warnings
10/16/14  as      Added code changes to move back to configured window size whenever there is change 
                  in RLC window size and FC is active by posting CFCM_OFF command internally.
10/10/14  kv      Fix to prevent race condition between l2_dl task and rrc task
09/12/14  aa      Fix to avoid deadlock between EUL ISR datapath mutex and RLC FC mutex
09/04/14  scv     L2 changes for FEATURE_WCDMA_DIME_SW Cleanup
07/26/14  rkmk    Reduce image size by using right macro depending upon number of parameters
07/01/14  aa      CFCM changes for RLC
06/19/14  kc      Mainline FEATURE_WCDMA_HS_RACH_DELAY_CU
05/06/14  kv      Replacing dsm_queue_cnt API with DSM_GET_WM_CURRENT_CNT MACRO
05/06/14  geg     Use safer versions of memcpy() and memmove()
04/04/14  scv     Code changes for UL Compression Feature
03/26/14  kv      F3's to indicate lo,hi,dne WM counts for UL and DL channels during LC registration
                  and current_count and highest_count values
01/28/14  scv     Code changes for UL Compression Feature
01/09/14  scv     Code changes for UL Compression Feature
01/06/14  kc      Data Indication to RRC during FACH state
12/09/13  as      Added code change not to disable RACH if there is any pending data in DL 
                  that needs to be received which in turn would not trigger EDRX full sleep.
11/27/13  scv     Code changes for UL Compression feature
11/20/13  kv      Moved mask for MSG_4to9 from MSG_SSID_DIAG to MSG_SSID_WCDMA_RLC
11/15/13  as      Added code change to provide SRB data presence & corresponding TTI information to
                  L1 which in turn would increase the Tx priority for W during SRB data transmission.
10/04/13  kc      Fix to allow status PDU transmission irrespective of WM 
                  registration for AM LCs in EUL Call.
10/04/13  scv     Fix to check if channel is released or not configured before processing RESET command 
05/31/13  scv     Fix to ensure atomic operation of UL and DL release procedure
05/22/13  kc      Added DC-HSUPA Changes
05/17/13  as      Enhanced interface for PURGE_REQ between L2 & DS.
05/09/13  kc      Fix to clear status at RLC while moving to PCH & RACH abort is received
05/08/13  scv     Fix L2 compilation errors
05/03/13  as      Fix to block data RB transimission during ongoing PS call 
                  release procedure at upper layers
04/26/13  scv     Updated code to avoid overflow condition during pointer addition
04/16/13  kc      1. Added Null pointer for sdu_cnf_cb
                  2. Set app_field to zero for SDUs on Data RBs
04/02/13  kc      Added support to indicate CTRL info availability in rlc_check_pending_sdu api
04/02/13  ksr     Fixed bug in check_pending_sdu API
04/02/13  Ksr     Enhance the Interface API to include all UL data pending on SRB(New Data, Retransmit Data, CTRL Data) 
02/19/13  kc      Fix to pend status only if trashed pdu is RLC AM CTRL pdu
01/29/13  ksr     Disabled the registration with CPU based FC mechanism
01/15/13  as      MAC_I - Fix to avoid crash due to EUL ISR accessing a global which is being memset 
                  as part of R99 UL ISR context.
01/04/13  kc      Modularization of few functions
11/7/12   ts      Fixed KW errors
10/15/12  mrg     Before accesing the num_src_dscr in UM logging checkfor the data_bits size
09/21/12  as      Cleaned up code with proper identation and added missing function headers. 
10/19/12  as      Fix to check boundary condition while copying chain of RLC PDU 
                  into one DSM item.
09/11/12  grk     Fix to avoid accessing data part of iovec if there are no
                  source decriptors when logging UM PDU(eg. UM PDU with only LIs).
09/11/12  mrg     NULL pointer check before accessing WM ptr
10/01/12  pm      New enum RLC_SUSPEND/RLC_RESUME handling added in CMAC_UL_CONFIG
08/23/12  mrg     MAC-iis: Special HE functionality with LI 7 configured and 
                  RLC PDU size greater than 126 octets
07/25/12  as      New WM reg api() added for trigger base rach
07/18/12  as      Fix to trigger RACH procedure whenever there is pending data in L2
                  watermark queue when L1 is in FACH state
07/13/12  grk     MBMS feature cleanup at L2(MBMS_DEBUG_1, FEATURE_MODEM_MBMS)
08/13/12   mrg     HS-RACH & MAC-I Feature updated code check-in.
07/24/12  geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
03/28/12  zr      Adding feature definitions for Dime
06/22/12  mrg     L2 crash fix
06/18/12  mrg     L2 - MAC-I and HS-RACH code changes
03/05/12  grk     Fixed compiler warning
04/26/12  grk     Mainlined code under FEATURE_INACTIVITY_HANDLING.
04/12/12  grk     Fix to have separate PDU header and log buffers for EUL and R99.
04/24/12  grk     Fixed compiler warnings
03/27/12  grk     replacing err_fatal with msg_error if PDU build delay is detected
04/09/12  grk     Fix to handle DL only reconfiguration correctly.
04/05/12  grk     Increasing PDU build delay threshold from 30bpg to 90bpg.
03/27/12  grk     1. Removed wcdma_intlock() from R99 pdu building.
                  2. added additional profiling for pdu building
02/24/12  grk     Fix to update ciphering log status during configuration instead 
                  of updating during PDU building.
01/10/12  grk     Profiling support for RLC PDU building and MAC TFCI selection.
03/13/12  ksr     Changes to ERR_FATAL if we try to free
                  1. Any preallocated RLC DSM items.
                  2. An already freed RCL PDU
03/07/12  grk     Fixed compilation error.
02/09/12  Ksr     Order of Mutexs acquiring changed to avoid dedlock between L2-Ul and L2-DL task
02/02/12  Ksr     Mutex lock and unlock sequences are updated in RLC to ensure it will not block ISR for a long time.
02/16/12  Ksr     Added support for Trigger Based Rach Procedure
01/19/12  as      Mainlining FEATURE_DATA_PDCP
01/19/12  as      Fix to properly handle success confirmation from RLC DL for RLC_MODIFY action
11/28/11  as      Fixed the concurrency issue acquring due to DL AM one 
                  sided re-establishment and UL ISR sending ACK's for the
                  DL AM PDU's.
02/01/12  grk     Feature cleanup
01/23/12   as     Support for Free Floating Task changes
01/09/12  grk     Fix to allow status PDU transmission irrespective of WM 
                  registration for AM LCs.
12/02/11  grk     Added TB size check to avoid accessing data_ptr while trashing
                  RLC PDUs when TB size is 0.
11/28/11  grk     Fix for KW errors
10/11/11  as      Fix to update RLC window size irrespective of RRC state 
                  but transmit window update indication only in DCH state.
11/23/11  ksr     Added support for trashed ul pdus
11/07/11  grk     Fix to process UL only re-establishment correctly
10/18/11  grk     Moved log_status check for 0x4168 log packet from header decode
                  to config.
08/24/11  as      Fix to avoid purging of SDU which is partly transmitted.
10/10/11  grk     Fix to process UL only re-establishment correctly
10/06/11  grk     Fixed KW errors
09/22/11  grk     Support for thermal mitigation through NPA inteface
07/18/11  grk     Fixed mutex contention between UL and DL task when configuring
                  FC parameters during RESET procedure.
08/26/11  grk     Fix for KW errors
08/08/11  grk     Removed redundant memset
7/30/11   ksr     MC FC Mutex allocation, free mechanism
06/08/11  ssg     Fix to consider modulo part while calculating no. of pdus.
05/25/11  grk     Fix to update UL ciphering state after updating other ciphering parameters.
05/12/11  grk     Fix to properly update LC state during suspend operation.
03/24/11  grk     Fix to avoid malloc for internal FC commands.
03/09/11  ssg     MAC-ehs support on Apollo.
03/08/11  pj      Added debug code for detecting ul_ctl_blk_id_tbl corruption
03/02/11  pj      Added function definition to check if UL entity is in RESET_PENDING state
02/18/11  pj      Added LW mutex protection while assigning Uplink control block index
01/31/11  grk     Fixed compilation error.
01/25/11  kvk     Fixed featurization issue
01/21/11  vp/kvk  Added support for AMR WB packet simulation
01/17/11  vp      CMI Phase-4 clean-up of header files
12/07/10  vp      Added support for amr packet simulation in uplink
12/07/10  pj      1. Added changes to populate rlc_sn along with hfn for rrc
                  2. Added debug print to log nchan during cipher config
10/18/10  ssg     Added changes for new WCDMA-L2 interface.
09/20/10  pj      Added support to log erroneous conditions
09/28/10  grk     Added API to provide WM and ACK status for AM RBs to RRC.
09/28/10  pj      Added fix to skip filling IOVEC deatails, if length = 0
09/15/10  pj      1. Added fix to count active UL_AM channels, after 
                     current released channel is set to NULL_state
                  2. Added F3s for dumping debug prints of base timer
07/22/10  grk     Changes to avoid multiple PCH timer registration for same LC 
                  when there is already one running.
07/09/10  grk     1. Changes to indicate UL ISR to send RESET_ACK only after 
                     finishing UL RESET procedure.
                  2. Introduced new command to send RESET_ACK for duplicate RESETs.
07/07/10  pj      Added changes to use a global base timer for status processing
06/29/10  pj      replaced WCDMA_INTLOCK with LW mutex
05/18/10  pj      Added ZI reduction changes to reduce memory used in NAK pool
06/15/10  pj      Fixed lint-errors:
                  using proper API while writing to hdr & data sections of ERAM
06/11/10  pj      Fixed lint-errors:
                  Ignoring the return value of LW_LOCK() api, to fix LINT warning
05/25/10  pj      Added debug code to print nchan info during WM registration
05/17/10  av      Registration for temperature FC using correct API 
05/12/10  grk     Reorganized part of code under FEATURE_INACTIVITY_HANDLING.
05/12/10  kvk     Added changes to reflect the new enums for DEM registrations 
22/04/10  pj      Fixed KW-error, by adding Array bounds check for hdr-buff
04/07/10  ssg     Added fix to start Cell_PCH timer when there are pending 
                  PDU's to be ACK'd for SRB-2/3/4.
04/12/10  grk     Fixed compilation error on silver MOB.
                  1. Removed test header inclusion in source files.
                  2. Stubbed L2 function calls.
03/30/10  grk     Modified prototype of DSM WM callback for CMI compliance.
03/30/10  grk     Changes for RLC QTF.
03/22/10  grk     Added debug prints for TM.
02/22/10  av      fixing the missing featurization 
02/09/10  grk     Fixed KW warnings.
02/02/09  av      Moving DSM & DUP FC to UL task  
12/1/09   kvk     Added featurization for setting the MSB of the length for the last 
                  IOVEC for the TTI 
11/30/09  grk     Added fix to use correct LSN in case NAK LIST is split across
                  TTIs
11/30/09  pj      MoB fix: Moved rlci_reset_status_timer_cmd() to rlcdl.c
11/24/09  pj      Added chnages to defer resetting DL status timer from ISR to 
                  UL-task context
11/03/09  av      Fixed compile error 
11/02/09  kvk     Fixed compile error
10/28/09  kvk     Moved modem accelator specific code under feature FEATURE_WCDMA_UL_MODEM_ACC
10/23/09  av      Temperature FC   
10/20/09  pj      Added changes to lock DL mutex before RESET-ing UL-entity
09/19/09  grk     Fixed compiler warning.
09/22/09  pj      Added fix to avoid resetting "rlc_ul_channel_cnt" while logging UM PDUs
09/22/09  pj      Added debug code to print buffer-status info for UM PDUs
10/12/09  kvk     Fixed Lint warnings
10/12/09  ssg     Added fix for Timer_Discard(UM) reconfigaration handling.
10/07/09  av      Multi-client flow control algorithm 
10/06/09  grk     Fixed lint warnings.
10/01/09  grk     Fixed KW warnings.
09/24/09  kvk     1)Added support for DUP item based FC
                  2)Fix for TX window getting stuck when CPU based FC and DSM based FC triggered same time
	          3)Backup and restore mechanism for DSM and WM based FC
09/23/08  ssg     Fix compile error.
09/18/09  grk     Fix to update UL count_c when RESET procedure is terminated 
                  due to one-sided RLC re-establishment(DL).
09/09/09  kvk     Fixed MOB compilation error
09/02/09  kvk     Added support for small item based FC
09/09/09  rmsd    Fixed compilation error.
09/08/09  ssg     Added changes to use new interface files as part of CMI.
04/13/09  pj      Added support for CS-over-HS
09/01/09  kvk     Fixed the defensive check where it is not letting the WINSUFI to
                  go get TX in case new_win_size equals to dl config window
08/23/09  rksd    Fixed windows compiler warnings for MOB to work on offline code.
08/13/09  grk     Changes for bitmap based status tracking.
07/30/09  grk     Added debug code.
07/23/09  av      Log pdu memory corruption due to no mutex protection
07/15/09  grk     Added check to create new timer only if there is none existing.
07/14/09  pj      Added defensive code to avoid Tx of WINSUFI with WS > dl_config_size
07/06/09  av      R99 & L2 Mutex cleanup
07/02/08  grk     Changes to trigger transition from cell_pch state if there
                  is some data in AM retransmission Q or if there is pending 
                  polling request.
07/01/09  av      Large DSM item based RLC FC changes
06/26/08  av      l1-l2 interface cleanup 
05/29/09  ssg     Added support for SNOW_3G algo.
05/27/09  grk     R99/EUL mutex changes to backup the mutex(R99/EUL)
                  being locked and unlock the same.
05/18/09  grk     Merged back fix to recalculate RLC FC variable rlc_app_ws,
                  rlc_dsm_ws,rlc_fc_ws when RLC process RLC MODIFY.
05/15/09  grk     Added changes for R99 mutex.
05/14/09  ssg     Fixed lint error.
05/11/09  ssg     Updated Copyright Information
04/29/09  av      Fixed compiler warnings in L2 
04/14/09  kvk     Fixed the order of UL/DL lock to avoid dead lock between
                  WCDMA L2 UL/WCDMA L2 DL tasks
04/09/09  kvk     Added support for backup and restore FC params for RLC reset params
03/11/09  pj      Fixed Lint-error by properly handling the local variable(traffic_class).
03/11/09  pj      Added F3 to print the traffic of the Data-RAB currently registered 
                  with RLC
03/18/09  grk     Fix to retain the cur_sdu_ptr, in case UL WM is registered 
                  multiple times.
03/17/09  grk     Off-target lint warning fixes.
03/15/09  kvk     Added changes to completely Mutex lock the config/re-config 
                  part and also timer cb routines
03/11/09  pj      Added code changes to have UL/DL-task & EUL-ISR concurrency 
                  protection under light-weight mutex
03/03/09  ssg     Fixed compile error.
03/03/09  ssg     Added F3 for Alternate E-bit.
02/04/09  grk     Featurization fixes for feature FEATURE_DEQUEUE_SDU_TO_PROCESS.
01/30/09  grk     Added NULL checks for RLC timers (creation/usage).
01/29/09  grk     Changes to dequeue first SDU from UL WM before forming PDU
                  out of it.
01/28/09  grk     Merged missed out fixes from taxis to mainline.
01/22/09  grk     Merged L2 mutex changes from taxis to mainline.
13/01/08  grk     Moved PDU building debug messages on UL under debug 
                  variable.
03/03/08  ssg     Added support to alternate e-bit interpretations under the
                  featue flag FEATURE_RLC_ALTERNATE_E_BIT.
12/18/08  ksr     L2 Mutex changes to avoid concurrency issues
12/08/08  ssg     Provide IO vec length in bytes both for 7200 i/f & PP-HW.
12/08/08  ssg     Added fixes for UL-PP-HW.
11/22/08  kvk     Now sharing Mutex with L1 for all L2 to avoid memory concurrency issues
11/21/08  kvk     Replaced INTLOCK/INTFREE with REX_ISR_LOCK/REX_ISR_UNLOCK to avoid timer cb
                  avoid memory concurreny between timer task and WCDMA L2 UL task
10/15/08  kvk     INTLOCK/INTFREE used to avoid memory concurrency between timer cb ISR and
                  L2 DL task
10/17/08  grk     Merged taxis changes to mainline.
10/01/08  grk     1. Compiler Warning fix.
09/08/08  pj      Modified the parameter sent into rlc_um_free_sdu_q() 
                  as channel type (EDCH/ Non-EDCH)
          pj      Added definition to get active UL UM channels
09/02/08  cnp     Replaced intlock/free with wcdma_intlock/free.
          cnp     Replaced tasklock/free with wcdma_tasklock/free.
          cnp     Replaced intlock/free_sav with wcdma_intlock/free_sav.
08/26/08  pj      Added debug code for logging ciphering info within rlc_debug_var  
06/11/08  pj      Added NULL pointer check while accessing watermark ptr
06/11/08  pj      Added fix for mis-alignment access issue
06/09/08  pj      Added fix for mis-alignment access issue
06/06/08  ar      Added RLC UL and DL FC registration functionality during
                  new channel set up (RLC_ESTABLISH).
05/26/08  pj      Added seperate SDU queues for EDCH & Non-EDCH UM channels
05/22/08  ssg     Added fix to use correct log code for UM PDU logging.
05/22/08  ssg     Added fix to consider pending status PDU transmission 
                  also during Cell_PCH timer start.
05/05/08  pj      Added Flag protection for UL logging
04/29/08  ssg     Fixed compiler warning.
04/28/08  ar      Added support for the new rlci_update_tx_win_size()
                  interface in rlci_open_win_cb() and rlci_close_win_cb()
04/28/08  ssg     Added F3 msgs under global debug variable
04/28/08  ssg     Added fix to indicate RRC about the UL-WM registration in
                  Cell_PCH/Ura_PCH states.
04/18/08  ssg     Added deinition to rlc logging related global variable.
                  Fixed LINT errors.
04/11/07  ssg     Changed the included interface file.
03/24/08  pj      Added support for data path optimizations under 
                  FEATURE_WCDMA_UL_DATA_PATH_OPT & FEATURE_WCDMA_DL_DATA_PATH_OPT
03/24/08  ssg     Modified featurization to avoid compile errors.
03/17/08  ar      Added appropriate featurization:FEATURE_DL_CPU_BASED_FC
                  or FEATURE_UL_CPU_BASED_FC for flow control functions and 
                  variables, required for separating UL and DL fc functionality
02/25/08   pj     Initializing the buffer status parameters when lc_id is 
                  invalid, inorder to remove the false F3.
02/25/08   pj     Added Fix to remove Buffer_Overflow klocwork error 
02/01/08   pj     Changed FEATURE_MBMS flag to FEATURE_MODEM_MBMS.
02/01/08  vg      added semaphore approach for EUL logging to restrict
                  logging variable changes in UL IC.
29/01/08  ssg     Added fix to set pdu_type to FLEX_PDUS/FIXED_PDUs when 
                  uplink watermark is not registered.
12/14/07  ssg     Added fix for duplicate resets causing wrong UL-HFN increment.
12/10/07  vg      initialised config_conf result to SUCCESS when no bitmap is set               
                  under FEATURE_MULTIPLE_RLC_OPERATIONS              
12/07/07  vg      added missing check for feature 
                  FEATURE_MULTIPLE_RLC_OPERATIONS
12/06/07  vg      added support for multiple RRC operations handling at the
                  time of channel config req under feature flag 
                  FEATURE_MULTIPLE_RLC_OPERATIONS.
11/30/07  ssg     Added mbms support under FEATURE_MBMS.
09/18/07  vg      modified suspend activity to suspend_stop to support SMC and
                  SRNS senarios.
09/07/07  vg      added check to recalculate rlc_app_ws, rlc_dsm_ws,
                  rlc_fc_ws when RLC process RLC MODIFY.
09/06/07  ssg     Added support to enhanced buffer occupancy computation 
                  under the flag FEATURE_RLC_ENH_BUFFER_STATUS.
08/08/07  sm      Removed F3's which are getting called every TTI
08/07/07  ssg     Featurized rlci_ul_init_buffer_status() under FEATURE_WCDMA_HSUPA.
07/26/07  ssg     Added new interface changes for buffer occupancy on R99
                  channels under FEATURE_WCDMA_HSUPA.
07/06/07  ssg     Added fix to process loop back mode-2 indication from RRC
                  under FEATURE_TM_LB.
05/15/07  vg      removed MSG_HIGH
03/14/07  ttl     Register FC when flow control target LC created and de-register
                  when all target LC removed.
03/12/07  ttl     Lint fix as the data services change the naming of the 
                  associated enum.
02/27/07  ssg     Lint fixes.
02/16/07  ttl     Support new interface of RLC buffer status report.
                  Lint msg fix.
02/13/07  vg      added dynamic pdu support for EUL
02/12/07  ttl     Chage IP_TRF_CLASS_INTERACTIVE to DS_UMTS_QOS_TCLASS_INTR
                  as pdcp_get_traffic_class() changes its output.
02/07/07  ttl     Lint fix.
02/05/07  ttl     Fix Compiling warning as PDCP feature brought in.
02/02/07  ttl     Lint fix.
01/11/07  vg      moved AM/UM logging in task context
12/26/06  vg      fixed NULL pointer access issue which was causing a crash
12/20/06  ttl     Compiler warning fix.
11/28/06  ttl     Reset npdu and nbyte in buffer status report for error handling
                  cases.
11/23/06  ssg     Removed function definition for rlc_ul_suspend_cnf().
                  Added support for suspend req & cnf thru a callback.
11/23/06  vg      added support for UM DL/UL pdu looging under feature FEATURE_UM_PDU_LOGGING
10/09/06  ttl     Reflect the WM size calculation to the suspend offset for SRB2
                  Put RLC in UL_CONFIG_PENDING when enter critical section for
                  PDU building interrupt. Resume the state before leave the 
                  critical section.
10/06/06  ssg     Moving DL-AM from DL_RESET_PENDING to its previous state 
                  after one sided re-establishment, during ongoing RESET
                  procedure.
09/22/06  ssg     Added support for more data pdu logging on user plane 
                  uplink logical channels.
09/22/06  ssg     Modified rlc_rrc_get_hfn_req() to return HFN=0 for LC' which
                  have pending ciphering info.
09/18/06  ttl     Adjust the window size based on QoS.
09/11/06  ttl     Add support to overwrite the HFN being using.
09/06/06  ttl     Call different function to read QoS info FEATURE_DATA_PDCP 
                  is enabled.
08/28/06  ttl     Lint fix.
08/24/06  ttl     Add flow control info for a new added entity.
08/23/06  ssg     Added handling for one side re-establishment under the
                  feature FEATURE_RLC_ONE_SIDE_RE_ESTABLISHMENT
08/15/06  ttl     Use RLC Size stored in RRC OC.
                  Lint fix.
08/11/06  ttl     Put QoS support under FEATURE_CPU_BASED_FLOW_CONTROL_WITH_QOS. 
08/01/06  ttl     Add support to EUL.
07/28/06  ttl     Add support to HFN query.
07/20/06  ttl     Allow buffer status report in AM suspending mode.
06/21/06  ttl     In rlc_process_status_report_req() called by MAC, protect the 
                  invalid LC ID from accessing the control block.
06/02/06  ttl     Check the ciphering activation time of the SRB2 before adding
                  another pending config record. 
                  Add SRB2 suspending support.
                  Reset pdu pointer when l1_l2_dsm_free_packet() is called.
05/09/06  ttl     DSM usage optimization by removeing the DSM DUP item using 
                  in the UL AM PDU building. 
                  This is under FEATURE_WCDMA_BUILD_PDU_DSM_OPT.
04/12/06  ttl     Directly write PDU logging data into logging buffer w/o
                  calling the dsm_extract().
                  Give the failure reason when call sdu_cnf_cb().
                  Replace the math divide w/ bit shifting.
03/27/06  ssg     Special_ind flag during purge is conditionally set.
03/03/06  ttl     Generate Status report as MAC request in recovery L2 ACK.
12/23/05  aw      Obsoleted the  RLC Uplink Statistics v1.
12/16/05  ttl     Fixed a bug of not setting the config cnf result in the LC release.
12/14/05  pdv     CR 83228
11/28/05  aw      Added the function rlc_set_activity_flag.
11/22/05  pdv     Fixing lint errors.
11/07/05  aw      Added the support for reading the release version info
                  from NV data.
11/07/05  aw      Added support for inactivity handling of user plane
                  UM and AM channels under the feature 
                  FEATURE_INACTIVITY_HANDLING.
09/12/05  aw      Added the support for RLC duplicate transmission in RLC
                  statistics log packet.
08/01/05  aw      Added support for timer based SDU discard.
05/31/05  mv      Replaced AM_DATA_START_LC_ID with 
                  RLC_UL_AM_USR_PLANE_START_LC_ID.
05/04/05  mv      Fixed a couple of lint errors.
05/03/05  mv      Added the support for aborting the new ciphering config
                  and reverting it to the old one when requested by RRC.
04/28/05  aw      Fixed the bug of sending Window close SUFI only once.
04/05/05  aw      Removed Log_on_demand feature.
03/02/05  mv      Added the support for indicating SDU failure to RRC
                  for the SDUs that are purged from the WM.
02/22/05  mv      Added the support for purging the SDUs from the UL WM.
                  Added a check to see if purge_wm was set to True   
                  during the reporting of buffer occupancy for TM and UM. 
                  so that no buffer occupancy is reported for these channels. 
8/19/04   mv      Separated the RLC PDU logging for the user plane and 
                  signaling plane logical channels.
08/20/04  mv      Changed some log packet names.
06/30/04  mv      Changed the units of pdu_size used for logging from bytes
                  to bits. 
                  Changed a few messages to print argument names.
06/02/04  mv      Fixed lint errors and warnings.
04/22/04  mv      Fixed the bug of starting the CELL_PCH timer when the 
                  watermark is not registered.
04/22/04  mv      Inserted a check in the PCH timer callback function
                  to check if the watermark pointer is not NULL before
                  calling the non-empty function pointer.
04/12/04  mv      Changed a MSG_HIGH to print an argument. 
03/31/04  mv      Fixed the bug of not sending data indication to RRC in 
                  CELL_PCH state, when a re-transmission is triggered due 
                  to expiry of a poll timer or new data arrives in the 
                  watermark.
02/09/03  ttl     Supported suspend/resume AM timer for Cell_PCH.
                  Moved UE out of the Cell_PCH state when UE is config to go
                  Cell_PCH when there is re-transmission data left to be sent.
01/23/04  mv      Added a QXDM event for RLC window close/open from UE.
01/15/03  mv      Inserted a check in RLC establish, RLC re-establish, and 
                  RLC modify to make sure that the value of MAXDAT is 
                  set greater than 1 since maximum number of transmissions 
                  allowed is MAXDAT - 1.
01/07/04  ttl     Added CELL_PCH support.
                  Fixed the problem of not sending open window sufi 4 times 
                  when re-open the peer side tx window.
12/06/03  ttl     Set the RESET rex sig when RESET is caused by DSM DNE cb 
                  function.
                  Added PDU ciphering parameters logging.
11/14/03  ttl     Fixed the internal states inconsistency after AM re-establish.
10/28/03  ttl     Added rlc_rrc_get_wm_info_req() to give the total number
                  of bytes need to be sent next.
                  Added ciphering algorithm indication.
                  Added re-establishment in UM config handler.
10/16/03  mv      Added a check in the RLC_ESTABLISH for AM mode to 
                  determine if the AM transmission queue was assigned an 
                  AM channel. If there was no AM channel that could be
                  assigned to the transmission queue, a failure is returned.
10/14/03  mv      Added the function rlci_change_peer_tx_win to change the
                  window size of the peer side's transmission window.
10/07/03  ttl     Modified ciphering code to handle two copies of new ciphering
                  config, one copy of current using and two copies of old
                  ciphering config.
09/04/03  ttl     Delay setup AM timer until the AM entity start to tx PDUs.
08/20/03  ttl     Merged branch code into tip.
08/11/03  ttl     Fixed two pending ciphering config problem for RB2.
08/01/03  mv      Added support for RLC re-establish procedure on AM uplink
06/18/03  ttl     Fixed a bug to give wrong RLC information to RRC in Ciphering mode.
05/16/03  ttl     Fixed a bug in two stage flow control.
                  Added PDU trashing test under FEATURE_RLC_TEST.
                  Replace divide w/ bit shifting.
04/21/03  ttl     Added two stages flow control of DSM free items.
03/18/03  ttl     Reset new_cipher_on when the UM channel is config'd.
03/09/03  ttl     Changed WM unregistered message level.
                  Added checking on obtaining the ctrl_blk pointer when call
                  rlci_get_ctl_blk_ptr().
02/14/03  ttl     Fixed Ciphering reconfig problem when there is no data 
                  Tx or Rx in the channel.
01/14/03  ttl     Open peer's tx win after reset caused by the DSM_DNE_CB.
12/10/02  ttl     Set REX SIG RLC_DL_TIMER_BACKUP_SIG in the rlc_ul_buffer_status()
                  for backup timer.
12/09/02  ttl     Remove FEATURE_RLC_MEM_LEVEL.
11/20/02  ttl     Proceeding RLC RESET if the DSM items go below 
                  DSM_MEM_LEVEL_LINK_LAYER_DNE.
                  Implemented DSM_MEM_LEVEL_RLC_MANY and DSM_MEM_LEVEL_RLC_FEW
                  under FEATURE_RLC_MEM_LEVEL.
                  Added Window SUFI retx for opening the peer side tx window.
11/13/02  ttl     Recorded ul_chan_am_pdu.pdu_size as byte size.
                  Changed tot_num_pdu_byte_txd to be byte count.
11/05/02  ttl     Added logging for ciphering config.
                  Fixed the bug of not accepting of re-config of ciphering 
                  parameters when in CIPHER_MOD mode.
                  Fixed the bug of not setting the proper value of HFN when
                  performing RESET procedure.
10/30/02  ttl     Added Ciphering restart feature.
10/06/02  ttl     Added MSG on sending PDU in RACH.
09/16/02  ttl     Removed the Suspend state checking in ciphering config.
                  Reset cipher_mode and cipher_all in UM config.
09/05/02  ttl     Added Ciphering feature.
08/21/02  ttl     Fixed the bug of overwritten the memory when PDU logging
                  is turned on. 
08/16/02  ttl     Added the RLC traffic flow control support for small DSM item pool.
08/13/02  ttl     Added SDU timer discard feature for UM.
08/01/02  ttl     Added rlc_get_data_counters() to give the byte count of sent SDUs
                  and received SDUs.
07/31/02  ttl     Removed rlc_id/Time sig mapping table.
                  Rewrote 10ms base timer, removed timer sig, using timer
                  callback function instead.
                  Removed rlci_ul_am_get_timer_sig(), rlci_ul_am_timer_sig_to_rlc_id(),
                  rlci_ul_am_release_timer_sig().
                  Added SDU discard timer feature for TM.
                  Added the check of WM pointer not being NULL in buffer status 
                  report and PDU building.
07/27/02  ttl     Reset current SDU data starting pointer when registering WM.
06/12/02  ttl     Logging the AM PDU only when the logging mask is on.
05/17/02  ttl     Implementation to handle SUSPENDED/WAITING_TO_SUSPEND state.
                  Added code to handle STOP/CONTINUE in Config_Req from RRC for 
                  AM and UM..
                  Changed some messages contents.
05/14/02  ttl     Naming changed for wcdma_l2_ul_tcb.
05/13/02  ttl     Modified lc_state.
                  Added rlc_open_peer_tx_win() and rlc_close_peer_tx_win() for
                  user plane data application's flow control.
05/08/02  ttl     Fixed the problem of not being able to tx status PDU when UL
                  is using RACH.
04/30/02  ttl     Lowered a message level.
04/24/02  ttl     Rewrote the statistic log code.
04/19/02  ttl     Reset ul_wm_ptr for TM and UM at the call establishment.
04/03/02  ttl     Changed incorrect naming of FACH to RACH.
                  Added Log_on_demand feature.
                  Fixed the bug of accessing to watermark in the non-empty function
                  call in RACH.
03/15/02  sk      Added support for logging UL AM statistics information.
03/13/02  ttl     Fixed the bug of accessing WM before it got registered in RACH.
02/13/02  ttl     Added service de-registration function.
                  Changed the loop counters to be type uint16.
11/30/01  ttl     Moved Report RESET to RRC function to L2 task.
11/17/01  ttl     Added post RESET result to RRC feature.
10/26/01  ttl     Revised messages contents.
09/17/01  ttl     Added RESET complete message.
08/30/01  rc      Cleaned up code and added more messages.Renamed tm_log_ptr,
                  um_log_ptr, am_log_ptr and log_state as ul_tm_log_ptr, 
                  ul_um_log_ptr, ul_am_log_ptr and ul_log_state.
08/28/01  rc      Renamed hfn_cnf as ul_hfn_cnf.
08/27/01  rc      Made structure definitions global to the file. 
08/20/01  rc      Added support for QXDM logging.
08/10/01  rc      Changed some messages to fix the formatting problem in 
                  F3 messages.
06/29/01  ttl     Removed function rlctmr_ul_callback();
                  Added rlci_ul_am_get_timer_sig().
                  Added rlci_ul_am_timer_sig_to_rlc_id().
                  Added rlci_ul_am_release_timer_sig().
                  Changed 10ms timer to use rex sig instead of callback function.
06/28/01  ttl     Added UL AM establish message.
06/28/01  ttl     Change MSG level.
                  fixed rlci_get_ctl_blk_ptr().
06/28/01  rc      Added check for logical channel id  
                  being less than UE_MAX_UL_LOGICAL_CHANNEL.
06/26/01  ttl     Fixed a typo of rlc_ul_register_am_sdu_cnf_cb_req().
                  Modified the msg level for information display.
06/08/01  rc      Removed function rlci_dl_release_all_cnf_handler().
06/07/01  rc      Added support for Releasing all active channels on 
                  the Uplink and Downlink when RRC sends a Request to RLC
                  Release all channels.
06/06/01  ttl     Fixed bug on possible giving invalid ctl_blk_ptr for 
                  rlci_get_ctl_blk_ptr(). 
                  Added 10ms timer callback function.
                  Create 10ms Timer after established AM entity.
06/06/01  rc      Changed Messages in TM and UM Config to display both
                  Channel ID and Table Index for each logical channel.
05/29/01  ttl     Changed naming convention from nchan to nentity for
                  am_config_req.
                  Fixed RESET state transitions.
05/23/01  rc      Fixed warnings found while compiling with ARM.
05/21/01  ttl     Modified code for compiler level 4 warning messages.
05/21/01  rc      Cleaned up code and added more comments.
05/02/01  ttl     Moved action flag out of chan_info structure.
                  Added RESET procedure for TM/UM/AM.
03/01/01  rc      Removed rlcrrctask.h.
04/10/01  rc      Fixed warnings found while compiling for ARM.
04/03/01  rc      Changed messages in crlc_ul_*m_config_handler() to 
                  display logical channel id instead of table index.
04/02/01  rc      Changed commands from RRC_CRLC_CONFIG_*M_UL_CNF to
                  RRC_CRLC_UL_*M_CONFIG_CNF.
03/26/01  ttl     Added rlc_ul_register_am_sdu_cnf_cb_req().
02/28/01  ttl/rc  Added code to send confirms back to RRC after a config.
                  Introduced the mapping table for control block id and 
                  channel id to support Acknowledged mode.                   
01/31/01  rc      Changed rlc_ul_buffer_status() according to the 
                  latest macrlcif.h. The number of channels and the
                  id are not explicitly sent to MAC.
01/08/01  rc      Cleaned up code and fixed some bugs. Modified code to
                  use the data types defined in uecomdef.h
11/22/00  rc      Created file.

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "wcdma_variation.h"
#include "customer.h"
#include "l2ultask.h"
#include "l2dltask.h"
#include "rlcul.h"
#include "rlci.h"
#include "rlcultm.h"
#include "rlculum.h"
#include "rlculam.h"
#include "rrccmd_v.h"
#include "rrcscmgr.h"
#include "macrlcif.h"
#include "event_defs.h"
#include "event.h"
#include "rlcfc.h"
#include "dsumtspdpreg.h"
#include "rlcdl.h"
#include "wrlc_qsh_int.h"

#ifdef FEATURE_RLC_QXDM_LOGGING
  #include "rlcullog.h"
#endif

#ifdef FEATURE_CMI
#include "pdcp.h"
#else
#include "pdcprlcif.h"
#endif

#ifdef FEATURE_QXDM_DSM_TRACING
#include "fc_dsm_trace.h"
#endif

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#define RLC_UL_CHECK_PENDING_DATA_FOR_TX(rlc_ul_table_ptr, index, retx_size) \
  (((rlc_ul_table_ptr->ctrl_blk[index].lc_mode == UE_MODE_ACKNOWLEDGED) && \
            (((rlc_ul_table_ptr->ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_s != \
              rlc_ul_table_ptr->ctrl_blk[index].ctrl.am_ctrl.ul_state.vt_a) && \
              (((TRUE == rlc_ul_table_ptr->ctrl_blk[index].ctrl.am_ctrl.ul_state.poll_next) && \
                (FALSE == rlc_ul_table_ptr->ctrl_blk[index].ctrl.am_ctrl.ul_state.poll_prohibit))|| \
                (retx_size > 0) || (rlc_ul_table_ptr->ctrl_blk[index].lc_type == UE_LOGCHAN_DCCH))) || \
              rlc_ul_table_ptr->ctrl_blk[index].ctrl.am_ctrl.ul_state.ack_pdu)) || \
           ((rlc_ul_table_ptr->ctrl_blk[index].ul_wm_ptr->current_cnt + rlc_ul_table_ptr->ctrl_blk[index].length_sdus_dequeued) != 0) \
           || (NULL != rlc_ul_table_ptr->ctrl_blk[index].cur_sdu_ptr))

#define RLC_UL_UM_USR_PLANE_START_LC_ID 9

/* Indicates this DSM is part of the pre-allocated pool 
 * Not expected to release during the call 
 */
#define L2_DSM_PRE_ALLOC_MASK 0x10000000  
void rlc_check_pre_alloc_dsm(rlci_uplink_table_type *rlc_ul_table_ptr);

/* -----------------------------------------------------------------------
** Variable Definitions
** ----------------------------------------------------------------------- */

/* UL control block */
rlci_uplink_table_type uplink_table[L2_NUM_W_SUBS];
rlc_mutexes_type rlc_mutexes[L2_NUM_W_SUBS];

static int32 frame_qty, cx8_qty;
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
rlc_qsh_metric_data_type rlc_qsh_metric_global[L2_NUM_W_SUBS]; 
qsh_ext_metric_cfg_s wrlc_qsh_metric_cfg_arr[L2_NUM_W_SUBS][WRLC_QSH_METRIC_MAX_STAT_E];
#endif

rtt_delay_global_type rtt_delay_global[L2_NUM_W_SUBS];

/*-------------------------------------------------------------------
FUNCTION l2_validate_as_id

DESCRIPTION
  This API validates the as_id received from other modules for lower and upper bounds.
  Should be called whenever some other module calls RLC/MAC APIs with as_id
--------------------------------------------------------------------*/
void l2_validate_as_id(sys_modem_as_id_e_type as_id)
{
  if ((as_id >= L2_NUM_W_SUBS) || (as_id < L2_SUB_1))
  {
    WRLC_ERR_FATAL(as_id, "AS ID value %d unexpected, max possible %d, min possible %d",
               as_id, L2_NUM_W_SUBS - 1, L2_SUB_1);    
  }
  return;
}

/*===========================================================================

FUNCTION RLCI_GET_UL_CTL_BLK_PTR_BY_INDEX

DESCRIPTION
  Returns Ctrl Blk Ptr from uplink table for specified index 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
rlci_ul_ctrl_blk_type *rlci_get_ul_ctl_blk_ptr_by_index 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  uint8 index
)
{
  rlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;

  if (index < UE_MAX_UL_RLC_ENTITY)
  {
    ctl_blk_ptr = &(rlc_ul_table_ptr->ctrl_blk[index]);
  }

  return ctl_blk_ptr;
}

/*===========================================================================

FUNCTION RLCI_GET_UL_CTL_BLK_INDEX_FOR_RLC_ID

DESCRIPTION
  Returns index of ctrl blk in uplink table for specified RLC ID 

DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint8 rlci_get_ul_ctl_blk_index_for_rlc_id 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_lc_id_type rlc_id
)
{
  uint8 index = INIT_CTL_BLK_ID;
  
  if (rlc_id < UE_MAX_UL_LOGICAL_CHANNEL)
  {
    index = rlc_ul_table_ptr->ul_ctl_blk_id_tbl[rlc_id];
  }

  return index;
}

/*===========================================================================

FUNCTION RLCI_GET_UL_CTL_BLK_PTR_BY_RLC_ID

DESCRIPTION
  Returns Ctrl Blk Ptr from uplink table for specified RLC ID 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
rlci_ul_ctrl_blk_type *rlci_get_ul_ctl_blk_ptr_by_rlc_id 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_lc_id_type rlc_id
)
{
  rlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;
  uint8 index = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, rlc_id);
  if ((INIT_CTL_BLK_ID != index) && (index < UE_MAX_UL_RLC_ENTITY))
  {
    ctl_blk_ptr = &(rlc_ul_table_ptr->ctrl_blk[index]);
  }

  return ctl_blk_ptr;
}


/*===========================================================================

FUNCTION     RLC_QUERY_TRAFFIC_TIME_METRICS

DESCRIPTION  Returns a pointer of type 'rlc_traffic_time_metrics' with delay
             info for the requested delay metric
  
===========================================================================*/
void rlc_query_traffic_time_metrics 
( 
  uint8 sub_ID, uint8 rlc_id, delay_metric_type delay_type, rlc_traffic_time_metrics* rlc_ul_traffic_metrics
)
{
  rlc_delay_metric_type *delay_metrics_lc_ptr;
  sys_modem_as_id_e_type as_id;
  rlc_mutexes_type *rlc_mutex_ptr;
  rtt_delay_global_type *rtt_delay_global_ptr;
  
  as_id = wcdma_convert_subid_to_stack_index(sub_ID); 
  delay_metrics_lc_ptr = NULL;
  
  l2_validate_as_id(as_id);
  
  rlc_mutex_ptr = RLC_GET_MUTEX_PTR(as_id);
  rtt_delay_global_ptr = RLC_GET_RTT_GLOBAL_PTR(as_id);
  
  if (NULL != rlc_ul_traffic_metrics)
  {
    /* Function code starts here */
    if ( (rlc_id >= RLC_UL_AM_START_LC_ID) && (rlc_id < (UE_MAX_AM_ENTITY+RLC_UL_AM_START_LC_ID) ) )
    {
      switch (delay_type)
      {
        case RTT_DELAY:
          delay_metrics_lc_ptr = &(rtt_delay_global_ptr->rtt_metrics[(rlc_id - RLC_UL_AM_START_LC_ID)]);      
        break;
        
        case QUEUING_DELAY:
          delay_metrics_lc_ptr = &(rtt_delay_global_ptr->qdelay_metrics[(rlc_id - RLC_UL_AM_START_LC_ID)]);
        break;
        
        default: 
          WRLC_ERR_FATAL(as_id, "RLC_ERR_RTT_FATAL: Delay Type has Invalid Value: %d", delay_type, 0, 0);
        break;
      }
    }    
    L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex)); 
    
    if ((delay_metrics_lc_ptr != NULL) && (delay_metrics_lc_ptr->num_samples > 0))
    {
      rlc_ul_traffic_metrics->average_delay_in_ms = delay_metrics_lc_ptr->avg_delay;
      rlc_ul_traffic_metrics->max_delay_in_ms     = delay_metrics_lc_ptr->max_delay;
      rlc_ul_traffic_metrics->min_delay_in_ms     = delay_metrics_lc_ptr->min_delay;
      rlc_ul_traffic_metrics->num_samples         = delay_metrics_lc_ptr->num_samples;
      
      delay_metrics_lc_ptr->num_samples = 0;
    }
    else
    {
      rlc_ul_traffic_metrics->average_delay_in_ms = 0;
      rlc_ul_traffic_metrics->max_delay_in_ms     = 0;
      rlc_ul_traffic_metrics->min_delay_in_ms     = 0;
      rlc_ul_traffic_metrics->num_samples         = 0;
    }
    L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
    
    MSG_HIGH("RLC_RTT: Received Query for Delay Metric %d ( 0-RTT / 1-QUEUING). Returning num_samples %d for RLC ID %d",
                       delay_type, rlc_ul_traffic_metrics->num_samples, rlc_id);
    MSG_HIGH("RLC_RTT: Avg Delay %d, Max Delay %d, Min Delay %d", rlc_ul_traffic_metrics->average_delay_in_ms,
                       rlc_ul_traffic_metrics->max_delay_in_ms, rlc_ul_traffic_metrics->min_delay_in_ms);
  }    
}


/*===========================================================================

FUNCTION     RLC_RESET_TRAFFIC_TIME_METRICS

DESCRIPTION  API to DS Layer by which they can reset RTT & Queuing Delay 
             Metrics for all AM Channels.

===========================================================================*/
void rlc_reset_traffic_time_metrics ( uint8 sub_ID, uint8 rlc_id)
{
  sys_modem_as_id_e_type as_id;
  
  as_id = wcdma_convert_subid_to_stack_index(sub_ID); 
  
  l2_validate_as_id(as_id);
  
  if ( (rlc_id >= RLC_UL_AM_START_LC_ID) && (rlc_id < (UE_MAX_AM_ENTITY+RLC_UL_AM_START_LC_ID) ) )
  {
    rlc_ul_reset_rtt_for_lc(RLCI_GET_UL_TBL_PTR(as_id), rlc_id);
  }
  else
  {
    MSG_ERROR("RLC_RTT_ERR: Invalid RLC ID %d received", rlc_id, 0, 0); 
  }
}


/*===========================================================================

FUNCTION     RLC_GET_CURRENT_TIMESTAMP_MS

DESCRIPTION  Returns the current timestamp in resolution of 3.4 ms since 
             the PS Timer API returns a 16-bit value with above resoltuion.
 
===========================================================================*/
uint32 rlc_get_current_timestamp_ms( )
{
  uint32 curr_timestamp_3_4_ms;
  curr_timestamp_3_4_ms = (uint32)RLC_GET_CURRENT_TIMESTAMP();
  
  return (curr_timestamp_3_4_ms);
}


/*===========================================================================

FUNCTION     TIMESTAMP_DIFFERENCE_MS

DESCRIPTION  Returns the difference between the two arguments MODULUS (max value possible)
  
===========================================================================*/
uint32 timestamp_difference_ms ( uint32 timestamp1, uint32 timestamp2 )
{
  uint32 time_diff;
  
  if (timestamp1 >= timestamp2)
  {
    time_diff = (timestamp1-timestamp2);
  }
  else
  {
    /* the maximum possible value for timestamp (in ms) is 0xFFFFFFFF >> 5
       since the uint32 tick value will be converted to ms initially      */
    time_diff = ( timestamp1 + ((0xFFFF) - timestamp2) );
  }
  
  return (time_diff);
}


/*===========================================================================

FUNCTION     RLC_UL_INIT_RTT_TABLE

DESCRIPTION  Initializes the RTT entry table.
  
===========================================================================*/
void rlc_ul_init_rtt_table(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  int i,j;
  rlc_am_rtt_entries_per_lc* rlc_am_rtt_entries_per_lc_ptr;
  rlc_mutexes_type *rlc_mutex_ptr;
  rtt_delay_global_type *rtt_delay_global_ptr;
  
  rlc_mutex_ptr = RLC_GET_MUTEX_PTR(rlc_ul_table_ptr->as_id);
  rtt_delay_global_ptr = RLC_GET_RTT_GLOBAL_PTR(rlc_ul_table_ptr->as_id);
  
  L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
  
  /* Initialize ul_tbl_rtt_index */
  for ( i = 0; i < UE_MAX_AM_ENTITY ; i++ )
  {
    rtt_delay_global_ptr->ul_tbl_rtt_index[i] = 0xFF;
    memset(&(rtt_delay_global_ptr->rtt_metrics[i]), 0, sizeof(rlc_delay_metric_type));
    memset(&(rtt_delay_global_ptr->qdelay_metrics[i]), 0, sizeof(rlc_delay_metric_type));
  }
  
  /* Initialize rlc_am_rtt_table */
  for ( i = 0; i < MAX_NUM_AM_CHANNELS_RTT ; i++ )
  {
    rlc_am_rtt_entries_per_lc_ptr = &(rtt_delay_global_ptr->rlc_am_rtt_table[i]);
    rlc_am_rtt_entries_per_lc_ptr->in_use = FALSE;
    rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc = 0;
    
    for ( j = 0; j < MAX_NUM_RTT_ENTRIES_PER_LC ; j++ )
    {
      rlc_am_rtt_entries_per_lc_ptr->rtt_entries[j].seq_num = 0xFFFF;
      rlc_am_rtt_entries_per_lc_ptr->rtt_entries[j].time_of_tx = 0;
    }
  }
  
  rtt_delay_global_ptr->num_active_rtt_channels = 0;
  
  L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
}


/*===========================================================================

FUNCTION      RLC_UL_RESET_RTT_FOR_LC

DESCRIPTION   Validates that an RTT entry exists for the AM channel, and  
resets the variables upon RESET/Re-EST
  
===========================================================================*/
void rlc_ul_reset_rtt_for_lc( rlci_uplink_table_type *rlc_ul_table_ptr, rlc_lc_id_type lc_id )
{
  uint8 rtt_table_index, i;
  rlc_am_rtt_entries_per_lc* rlc_am_rtt_entries_per_lc_ptr;
  rlc_delay_metric_type *delay_metrics_lc_ptr;
  rlc_mutexes_type *rlc_mutex_ptr;
  rtt_delay_global_type *rtt_delay_global_ptr;
  
  rlc_mutex_ptr = RLC_GET_MUTEX_PTR(rlc_ul_table_ptr->as_id);
  rtt_delay_global_ptr = RLC_GET_RTT_GLOBAL_PTR(rlc_ul_table_ptr->as_id);
  
  if (lc_id < RLC_UL_AM_START_LC_ID)
  {
    WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR_RTT: Request to RESET RTT variables for lc_id %d, which is not an AM Channel", lc_id, 0, 0); 
  }
  
  L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
  
  rtt_table_index = RLC_GET_RTT_TBL_INDEX( rtt_delay_global_ptr, lc_id );
  
  if (rtt_table_index == 0xFF)
  {
    MSG_ERROR("RLC_ERR_RTT: lc_id %d doesn't have an entry. Skipping RTT variables reset request", lc_id, 0, 0);
  }
  else if (rtt_table_index < MAX_NUM_AM_CHANNELS_RTT)
  {
    rlc_am_rtt_entries_per_lc_ptr = &(rtt_delay_global_ptr->rlc_am_rtt_table[rtt_table_index]);
    
    for ( i = 0; i < rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc ; i++)
    {
      rlc_am_rtt_entries_per_lc_ptr->rtt_entries[i].seq_num = 0xFFFF;
      rlc_am_rtt_entries_per_lc_ptr->rtt_entries[i].time_of_tx = 0;
    }
    
    rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc = 0;
  }
  else
  {
    MSG_ERROR("RLC_ERR_RTT: rt_table_index is invalid %d", rtt_table_index, 0, 0);
  }
  
  delay_metrics_lc_ptr = &(rtt_delay_global_ptr->rtt_metrics[(lc_id - RLC_UL_AM_START_LC_ID)]);
  delay_metrics_lc_ptr->avg_delay   = 0;
  delay_metrics_lc_ptr->max_delay   = 0;
  delay_metrics_lc_ptr->num_samples = 0;
  delay_metrics_lc_ptr->min_delay   = 0xFFFFFFFF;
  
  delay_metrics_lc_ptr = &(rtt_delay_global_ptr->qdelay_metrics[(lc_id - RLC_UL_AM_START_LC_ID)]);
  delay_metrics_lc_ptr->avg_delay   = 0;
  delay_metrics_lc_ptr->max_delay   = 0;
  delay_metrics_lc_ptr->num_samples = 0;
  delay_metrics_lc_ptr->min_delay   = 0xFFFFFFFF;  
  
  L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
}


/*===========================================================================

FUNCTION      RLC_UPDATE_SN_FOR_RTT

DESCRIPTION   update the RTT entry for a particular logical channel and SN number
  
===========================================================================*/

void rlc_update_sn_for_rtt(rlci_uplink_table_type *rlc_ul_table_ptr, rlc_lc_id_type rlc_id, uint16 rlc_rtt_sn)
{
  uint8 rtt_table_index, rtt_entry_index;
  rlc_am_rtt_entries_per_lc* rlc_am_rtt_entries_per_lc_ptr;
  rlc_mutexes_type *rlc_mutex_ptr;
  rtt_delay_global_type *rtt_delay_global_ptr;
  
  /* Function code starts here */
  rlc_mutex_ptr = RLC_GET_MUTEX_PTR(rlc_ul_table_ptr->as_id);
  rtt_delay_global_ptr = RLC_GET_RTT_GLOBAL_PTR(rlc_ul_table_ptr->as_id);
  
  rtt_table_index = rtt_entry_index = 0;
  rlc_am_rtt_entries_per_lc_ptr = NULL;
  
  L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
  
  rtt_table_index = RLC_GET_RTT_TBL_INDEX(rtt_delay_global_ptr, rlc_id);
  if ((rtt_table_index == 0xFF) || (rtt_table_index >= MAX_NUM_AM_CHANNELS_RTT))
  {
    WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR_RTT_FATAL: lc_id %d doesn't have an entry or    rtt_table_index %d invalid. Not expected", rlc_id, rtt_table_index, 0);
  }
  else
  {
    rlc_am_rtt_entries_per_lc_ptr = &(rtt_delay_global_ptr->rlc_am_rtt_table[rtt_table_index]);
    
    if (rlc_am_rtt_entries_per_lc_ptr->in_use == TRUE)
    {
      if (rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc < MAX_NUM_RTT_ENTRIES_PER_LC)
      {
        rtt_entry_index = rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc; 
        rlc_am_rtt_entries_per_lc_ptr->rtt_entries[rtt_entry_index].seq_num = rlc_rtt_sn;
        rlc_am_rtt_entries_per_lc_ptr->rtt_entries[rtt_entry_index].time_of_tx = rlc_get_current_timestamp_ms();
        rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc++;
      }
      else
      {
        WL2_MSG_1(LOW, rlc_ul_table_ptr, "RLC_ERR_RTT: RTT Entries full for lc_id %d. Not storing", rlc_id );
      }
    }
    else
    {
      WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR_RTT_FATAL: RTT entry in_use flag is %d for lc_id %d. Not expected", 
                 rlc_am_rtt_entries_per_lc_ptr->in_use, rlc_id, 0);
    }
  }
  
  L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
}


/*===========================================================================
FUNCTION: RLC_UPDATE_DELAY_METRICS

DESCRIPTION:
  This function computes the Round-Trip Time & Queuing Delay, and accordingly 
  updates traffic metrics.
===========================================================================*/

void rlc_update_delay_metrics( rlci_uplink_table_type *rlc_ul_table_ptr, rlc_lc_id_type rlc_id, uint32 tx_timestamp_ms, delay_metric_type delay_type)
{
  uint32 current_delay;
  rlc_delay_metric_type* delay_metrics_lc_ptr;
  rlc_mutexes_type *rlc_mutex_ptr;
  rtt_delay_global_type *rtt_delay_global_ptr;
  
  /* Function code starts here */
  rlc_mutex_ptr = RLC_GET_MUTEX_PTR(rlc_ul_table_ptr->as_id);
  rtt_delay_global_ptr = RLC_GET_RTT_GLOBAL_PTR(rlc_ul_table_ptr->as_id);
  current_delay = timestamp_difference_ms(rlc_get_current_timestamp_ms(), tx_timestamp_ms);
  
  switch (delay_type)
  {
    case RTT_DELAY:
      delay_metrics_lc_ptr = &(rtt_delay_global_ptr->rtt_metrics[(rlc_id - RLC_UL_AM_START_LC_ID)]);      
    break;
    
    case QUEUING_DELAY:
      delay_metrics_lc_ptr = &(rtt_delay_global_ptr->qdelay_metrics[(rlc_id - RLC_UL_AM_START_LC_ID)]);
    break;
    
    default: 
      WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR_RTT_FATAL: Delay Type has Invalid Value: %d", delay_type, 0, 0);
    break;
  }
  
  L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
  if (delay_metrics_lc_ptr->avg_delay > 0)
  {
    delay_metrics_lc_ptr->avg_delay = (( 9*delay_metrics_lc_ptr->avg_delay + current_delay ) /10 );
    
    if ( current_delay < delay_metrics_lc_ptr->min_delay ) 
    {
      delay_metrics_lc_ptr->min_delay = current_delay;
    }
    else if ( current_delay > delay_metrics_lc_ptr->max_delay )
    {
      delay_metrics_lc_ptr->max_delay = current_delay;
    }
  }
  else
  {
    delay_metrics_lc_ptr->avg_delay = current_delay;
    delay_metrics_lc_ptr->min_delay = current_delay;
    delay_metrics_lc_ptr->max_delay = current_delay;
  }
  
  if (delay_metrics_lc_ptr->num_samples < 0xFFFF )
  {
    delay_metrics_lc_ptr->num_samples++;
  }
  L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
}


/*===========================================================================
FUNCTION: RLC_COMPUTE_RTT_FROM_ACK

DESCRIPTION:
  This function checks if ACK has been received for any SNs that we remembered
  time of transmission for, and accordingly updates the RTT traffic metrics.
===========================================================================*/

void rlc_compute_rtt_from_ack( rlci_uplink_table_type *rlc_ul_table_ptr, rlc_lc_id_type rlc_id, uint16 ack_sn)
{
  uint8 rtt_table_index, rtt_entry_index, index;
  rlc_am_rtt_entries_per_lc* rlc_am_rtt_entries_per_lc_ptr;
  rlc_mutexes_type *rlc_mutex_ptr;
  rtt_delay_global_type *rtt_delay_global_ptr;
  
  /* Function code starts here */
  rlc_mutex_ptr = RLC_GET_MUTEX_PTR(rlc_ul_table_ptr->as_id);
  rtt_delay_global_ptr = RLC_GET_RTT_GLOBAL_PTR(rlc_ul_table_ptr->as_id);
  
  L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
  
  rlc_am_rtt_entries_per_lc_ptr = NULL; 
  rtt_table_index = RLC_GET_RTT_TBL_INDEX(rtt_delay_global_ptr, rlc_id);
  
  if ((rtt_table_index == 0xFF) || (rtt_table_index >= MAX_NUM_AM_CHANNELS_RTT))
  {
    WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR_RTT_FATAL: lc_id %d doesn't have an entry, or rtt_table_index value %d invalid. Not expected", rlc_id, rtt_table_index, 0);
  }
  else
  {
    rlc_am_rtt_entries_per_lc_ptr = &(rtt_delay_global_ptr->rlc_am_rtt_table[rtt_table_index]);
    
    if (rlc_am_rtt_entries_per_lc_ptr->in_use == TRUE)
    {
      if ( rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc > 0 )
      {
        index = 0;
        while (( index < rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc) &&
                 RLCI_SEQ_GT(ack_sn,rlc_am_rtt_entries_per_lc_ptr->rtt_entries[index].seq_num,RLC_AM_MODULUS) )
        {
          rlc_update_delay_metrics(rlc_ul_table_ptr, rlc_id, rlc_am_rtt_entries_per_lc_ptr->rtt_entries[index].time_of_tx,
                                 RTT_DELAY);
          index++;
        }
        
        if ( index > 0)
        {
          /* Shift the entries back by 'index' so that entries will begin at index 0 */
          for ( rtt_entry_index = index; rtt_entry_index < rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc ; 
                rtt_entry_index++ )
          {
            rlc_am_rtt_entries_per_lc_ptr->rtt_entries[(rtt_entry_index - index)] = 
            rlc_am_rtt_entries_per_lc_ptr->rtt_entries[rtt_entry_index];
          }
          
          rlc_am_rtt_entries_per_lc_ptr->num_curr_entires_for_lc -= index;
        }
      }
    }
    else
    {
      WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR_RTT_FATAL: RTT entry in_use flag is %d for lc_id %d. Not expected", 
                 rlc_am_rtt_entries_per_lc_ptr->in_use, rlc_id, 0);
    }
  }
  
  /* RELEASE LOCK */
  L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
}

/*===============================================================================================
 FUNCTION NAME  : rlc_profile_dump_start

 DESCRIPTION    : This will note down the time stamp of the fucntion was started

===============================================================================================*/

void rlc_profile_dump_start(sys_modem_as_id_e_type as_id,l2_rlc_profile_data_struct_type *profile_dump)
{
  wl1_get_profile_point(&frame_qty, &cx8_qty,as_id);
  profile_dump->profile_array[profile_dump->dump_idx].frame_qty_start = frame_qty;
  profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_start = (cx8_qty >> 11);
}


/*===============================================================================================
 FUNCTION NAME  : rlc_profile_dump_end

 DESCRIPTION    : This will note down the time stamp of the fucntion was ended.

===============================================================================================*/

void rlc_profile_dump_end(sys_modem_as_id_e_type as_id, l2_rlc_profile_data_struct_type *profile_dump)
{
  uint8 loop_index =0;
  int32 delay = 0;
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  wl1_get_profile_point(&frame_qty, &cx8_qty, as_id);
  profile_dump->profile_array[profile_dump->dump_idx].frame_qty_end = frame_qty;
  profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_end = (cx8_qty >> 11);

  if (profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_end <
      profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_start)
  {
    delay = (150 - profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_start) +
          profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_end;
          
    if (delay >= 90 )
    {
      WL2_MSG_3(ERROR, rlc_ul_table_ptr, "L2 delayed, frame boundary crossed, delay %d start %d, end %d",
                                                  delay,
          profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_start,
          profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_end);
    }
  }
  else
  {
    delay = profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_end - 
            profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_start;
    if (delay >= 90 )
    {
      WL2_MSG_3(ERROR, rlc_ul_table_ptr, "L2 delayed, taking %d bpg start %d, end %d ",
                                                  delay,
          profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_start,
          profile_dump->profile_array[profile_dump->dump_idx].cx8_qty_end);
    }
  }
 
  profile_dump->dump_idx++;
  
  if(profile_dump->dump_idx >= RLC_NUM_PROFILE_SAMPLES)
  {
    profile_dump->dump_idx = 0;
    
    if (rlc_ul_table_ptr->rlc_profile_var.l2_rlc_print_profile_data)
    {
      for(loop_index = 0; loop_index > (RLC_NUM_PROFILE_SAMPLES/4); loop_index++)
      {
        WL2_MSG_8(HIGH, rlc_ul_table_ptr, 
               "rlc_profile_dump: (%d, %d), (%d, %d), (%d, %d), (%d, %d),",
                profile_dump->profile_array[loop_index].cx8_qty_start, profile_dump->profile_array[loop_index].cx8_qty_end,
                profile_dump->profile_array[loop_index+1].cx8_qty_start, profile_dump->profile_array[loop_index+1].cx8_qty_end,
                profile_dump->profile_array[loop_index+2].cx8_qty_start, profile_dump->profile_array[loop_index+2].cx8_qty_end,
                profile_dump->profile_array[loop_index+3].cx8_qty_start, profile_dump->profile_array[loop_index+3].cx8_qty_end
              );
      }
    } 
  }
}

/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*===========================================================================
FUNCTION: L2_ACQ_UL_LW_LOCK

DESCRIPTION:
  API to acquire UL data path lock based on RLC LC id passed.

DEPENDENCIES:
  None

RETURN VALUE:
  Mutex Locked.

SIDE EFFECTS:
  None
===========================================================================*/
rex_crit_sect_type* L2_ACQ_UL_LW_LOCK(sys_modem_as_id_e_type as_id, rlc_lc_id_type rlc_id)
{
  rlc_ul_channel_type channel_type = CHANNEL_MAX;
  rex_crit_sect_type *r99_datapath_mutex_ptr = &(rlc_mutexes[as_id].r99_ul_isr_l2_datapath_mutex);

   /* If LC ID is >= UE_MAX_UL_LOGICAL_CHANNEL lock both EUL and R99 */
  if (rlc_id >= UE_MAX_UL_LOGICAL_CHANNEL)
  {
    REX_ISR_LOCK(r99_datapath_mutex_ptr);
    REX_ISR_LOCK(GET_WL1_EUL_ISR_DATAPATH_MUTEX(as_id));
    return NULL;  
  }

  /* Get channel type from MAC */
  channel_type = mac_ul_get_channel_type(as_id, rlc_id);
  if (CHANNEL_MAX == channel_type)
  {
    return NULL;
  }
  else if (CHANNEL_DCH == channel_type)
  {                                         
    REX_ISR_LOCK(r99_datapath_mutex_ptr);
    return (r99_datapath_mutex_ptr);  
  }
  else
  {                                       
    REX_ISR_LOCK(GET_WL1_EUL_ISR_DATAPATH_MUTEX(as_id));  
    return (GET_WL1_EUL_ISR_DATAPATH_MUTEX(as_id));
  }
}

/*===========================================================================
FUNCTION: L2_ENH_ACQ_UL_LW_UNLOCK

DESCRIPTION:
  API to release UL data path lock based on RLC LC id passed.

DEPENDENCIES:
  None

RETURN VALUE:
  NONE

SIDE EFFECTS:
  None
===========================================================================*/ 
rex_crit_sect_type* L2_ENH_ACQ_UL_LW_UNLOCK(sys_modem_as_id_e_type as_id, rlc_lc_id_type rlc_id)
{
  rlc_ul_channel_type channel_type = CHANNEL_MAX;
  rex_crit_sect_type *mutex_locked;
  rex_crit_sect_type *r99_datapath_mutex_ptr = &(rlc_mutexes[as_id].r99_ul_isr_l2_datapath_mutex);

   /* If LC ID is >= UE_MAX_UL_LOGICAL_CHANNEL lock both EUL and R99 */
  if (rlc_id >= UE_MAX_UL_LOGICAL_CHANNEL)
  {
    REX_ISR_UNLOCK(GET_WL1_EUL_ISR_DATAPATH_MUTEX(as_id));
    REX_ISR_UNLOCK(r99_datapath_mutex_ptr);
    return NULL;  
  }

  /* Get channel type from MAC */
  channel_type = mac_ul_get_channel_type(as_id, rlc_id);
  if (CHANNEL_MAX == channel_type)
  {
    return NULL;
  }
  else 
  {
    if (CHANNEL_DCH == channel_type)
    {
      mutex_locked = r99_datapath_mutex_ptr;
    }
    else
    {
      mutex_locked = GET_WL1_EUL_ISR_DATAPATH_MUTEX(as_id);
    }
    REX_ISR_UNLOCK(mutex_locked);  
    return (mutex_locked );
  }
}

/*===========================================================================
FUNCTION: L2_ACQ_UL_LW_UNLOCK

DESCRIPTION:
  API to release UL data path lock based on RLC LC id passed.

DEPENDENCIES:
  None

RETURN VALUE:
  NONE

SIDE EFFECTS:
  None
===========================================================================*/ 
void L2_ACQ_UL_LW_UNLOCK(rex_crit_sect_type *datapath_mutex)
{

  if (NULL != datapath_mutex)
  {
    REX_ISR_UNLOCK(datapath_mutex);
  }
  return;
}

/*===========================================================================
FUNCTION: RLC_UL_FREE_PREV_TTI_SDU_DSM_ITEM

DESCRIPTION:
  API to release the DSM memory for the UM/TM data that are transmitted in 
  previous TTI.

DEPENDENCIES:
  None

RETURN VALUE:
  NONE

SIDE EFFECTS:
  None
===========================================================================*/ 
void rlc_ul_free_prev_tti_sdu_dsm_item
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_sdu_q_type *sdu_free_q_ptr,
  uint32  max_free_sdu_buff
)
{
  uint16 idx;
  for (idx = 0; ((idx < sdu_free_q_ptr->num_sdu) && 
                                (idx < max_free_sdu_buff) ); idx++ )
  {
    if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_UM_PDU)
    {
      WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC DBG: DSM-FREE PACKET called-on sdu-buffer-ptr 0x%x index %d",
                                                 sdu_free_q_ptr->sdu_buffer_ptr[idx],
                                                 idx);
    }

    dsm_free_packet(&(sdu_free_q_ptr->sdu_buffer_ptr[idx]));
  }
  sdu_free_q_ptr->num_sdu = 0;
}

#ifdef FEATURE_MAC_I
/*===========================================================================
FUNCTION: RLC_UL_UPDATE_MAC_I_CTRL_DSM_ITEM

DESCRIPTION:
  API to free the memory allocated for partial PDU's.

DEPENDENCIES:
  None

RETURN VALUE:
  NONE

SIDE EFFECTS:
  None
===========================================================================*/ 
void rlc_ul_update_mac_i_ctrl_dsm_item (rlci_uplink_table_type *rlc_ul_table_ptr, rlc_ctrl_pdu_type *   mac_i_ctrl_dsm)
{
  if (rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.num_dsm_item < MAX_MAC_I_CTRL_DSM_ITEMS)
  {
    rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.mac_i_ctrl_dsm_idx[rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.num_dsm_item] = mac_i_ctrl_dsm ;
    rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.num_dsm_item++;
  }
  else
  {
    WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "MAC_I: rlc_free_mac_i_ctrl_q is FULL, num %d, max %d", 
              rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.num_dsm_item, MAX_MAC_I_CTRL_DSM_ITEMS, 0);
  }
}
/*===========================================================================
FUNCTION: RLC_UL_FREE_PREV_TTI_MAC_I_CTRL_DSM_ITEM

DESCRIPTION:
  API to release the DSM memory for the UM/TM data that are transmitted in 
  previous TTI.

DEPENDENCIES:
  None

RETURN VALUE:
  NONE

SIDE EFFECTS:
  None
===========================================================================*/ 
void rlc_ul_free_prev_tti_mac_i_ctrl_dsm_item (rlci_uplink_table_type *rlc_ul_table_ptr)
{
  uint16 idx;
  for (idx = 0; (idx < rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.num_dsm_item); idx++ )
  {
    rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.mac_i_ctrl_dsm_idx[idx]->in_use = FALSE ;
    rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.mac_i_ctrl_dsm_idx[idx]->rlc_id = RLC_INVALID_RLC_ID;
  }
  rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.num_dsm_item = 0;
}
#endif /* FEATURE_MAC_I */

#ifdef FEATURE_MAC_I
/*===========================================================================
FUNCTION: RLC_MAC_GET_LI_SIZE_SPECIAL_HE_FLG

DESCRIPTION:
  API used to get the LI length, special HE flag in case of AM and ALT E flag in case UM entity for the 
  given RLC ID.

DEPENDENCIES:
  None

RETURN VALUE:
  NONE

SIDE EFFECTS:
  None
===========================================================================*/ 
void rlc_mac_get_li_size_special_he_flg(sys_modem_as_id_e_type as_id, rlc_lc_id_type rlc_id, uint8 *li_len, boolean *special_he_flg, boolean *alt_e_cfg)
{
  rlci_ul_ctrl_blk_type *ul_ctrl_blk_ptr;
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);

  *li_len = 0;
  *special_he_flg = FALSE;
  *alt_e_cfg = FALSE;
  
  ul_ctrl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id);
  if (ul_ctrl_blk_ptr != NULL) 
  {
    switch(ul_ctrl_blk_ptr->lc_mode)
    {
      case UE_MODE_ACKNOWLEDGED:
      case UE_MODE_ACKNOWLEDGED_DATA:
      case UE_MODE_ACKNOWLEDGED_CONTROL:
       *li_len = ul_ctrl_blk_ptr->ctrl.am_ctrl.ul_state.li_length;
       if((ul_ctrl_blk_ptr->octet_less != TRUE) &&
          (ul_ctrl_blk_ptr->special_ind != TRUE))
       {
         *special_he_flg = ul_ctrl_blk_ptr->ctrl.am_ctrl.ul_cnfg_var->special_value_he_config;
       }
      break;

      case UE_MODE_UNACKNOWLEDGED:
       *li_len = ul_ctrl_blk_ptr->ctrl.um_ctrl.li_length;
       *alt_e_cfg = ul_ctrl_blk_ptr->ctrl.um_ctrl.alternate_e_bit;
        break;

       case UE_MODE_TRANSPARENT:
        *li_len = 0;
        break;

      default:
        WL2_MSG_2(ERROR, rlc_ul_table_ptr, "RLC_ERR: rlc_id %d Invld mode %d",
                                                    rlc_id,
                                                    ul_ctrl_blk_ptr->lc_mode);
         break;
    }
  }
  if((rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_UM_PDU) || (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_AM))
  {
    WL2_MSG_3(HIGH, rlc_ul_table_ptr, "LI_2Byte: rlc_id %d SP_HE %d li_len %d",
                                               rlc_id,
                                               *special_he_flg,
                                               *li_len);
  }
}

/*===========================================================================
FUNCTION: RLCI_ENH_CALC_UL_LI_LENGTH

DESCRIPTION:
 API used to calculate LI length for AM entity based upon RRC configuration.

DEPENDENCIES:
  None

RETURN VALUE:
  NONE

SIDE EFFECTS:
  None
===========================================================================*/ 
uint8 rlci_enh_calc_ul_li_length(rlci_uplink_table_type *rlc_ul_table_ptr, rlc_ul_am_config_type *ul_cnfg_var_ptr, uint8 old_li_length)
{
  rlc_li_e_type am_li_type;
  uint8 li_length;
  
  am_li_type = ul_cnfg_var_ptr->ul_li_size;
  li_length = 0xFF;
  switch(am_li_type)
  {
    case LI_7_BITS:
      li_length = 1;
     break;
     
    case LI_15_BITS:
     li_length = 2;
     break;
     
    case LI_SAME_AS_BEFORE:
       li_length = old_li_length;
        break;
        
    case LI_UNSPECIFIED:
     WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "MAC_I: Invalid LI %d from RRC ", 
                am_li_type, 0, 0);
     break;
     
    default:
     WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_ERR: Invalid AM LI Type %d from RRC",
                                                 am_li_type);
     break;
  }
  
  return (li_length);
}
/*===========================================================================

FUNCTION RLC_UL_UPDATE_CONFIG_PENDIG

DESCRIPTION

  This API used to update the RLC UL AM configuration status.
  
DEPENDENCIES
  None.

RETURN VALUE
  None
 
SIDE EFFECTS
  None.

===========================================================================*/
void  rlc_ul_update_config_pending(sys_modem_as_id_e_type as_id, boolean status)
{
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_CFG: On CMAC_ request, rlc_am_config_pending set to (%d)",
                                             status);
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_am_config_pending = status;
}

#endif

/*===========================================================================

FUNCTION RLC_DUMP_UL_CTL_BLK_ID_TBL

DESCRIPTION
  dumps uplink_table index values of LCs within ul_ctl_blk_id_tbl

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_dump_ul_ctl_blk_id_tbl(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  uint32 i = 0;

  for (i = 0; i < UE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    if (INIT_CTL_BLK_ID != rlc_ul_table_ptr->ul_ctl_blk_id_tbl[i])
    {
      WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC_DBG: UL LC %d is at index %d in uplink_table",
                                                 i, rlc_ul_table_ptr->ul_ctl_blk_id_tbl[i]);
    }
  }
}

/*===========================================================================

FUNCTION RLC_SANITIZE_UL_CTL_BLK_ID_TBL

DESCRIPTION
  Checks for any duplicate uplink_table index values of two different LCs within ul_ctl_blk_id_tbl

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_sanitize_ul_ctl_blk_id_tbl(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  uint32 i = 0;
  uint32 j = 0;

  uint8 *ul_ctl_blk_id_tbl = rlc_ul_table_ptr->ul_ctl_blk_id_tbl;
  for (i = 0; i < UE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    if (INIT_CTL_BLK_ID != ul_ctl_blk_id_tbl[i])
    {
      for (j = (i+1); j < UE_MAX_UL_LOGICAL_CHANNEL; j++)
      {
        if ((INIT_CTL_BLK_ID != ul_ctl_blk_id_tbl[j]) 
          && (ul_ctl_blk_id_tbl[j] == ul_ctl_blk_id_tbl[i]))
        {
          WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR: UL LC %d & LC %d are at same index %d in uplink_table", i, j, ul_ctl_blk_id_tbl[i]);
        }
      }
    }
  }
}

/*===========================================================================

FUNCTION RLC_UL_APP_NEW_WINSUFI

DESCRIPTION
  Assigns new window size required by APP_CLOSE and transmits new window sufi.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_app_new_winsufi
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_lc_id_type rlc_id,
  rlci_peer_tx_win_size_e_type peer_win
)
{
  rlci_ul_ctrl_blk_type *ul_ctrl_blk_ptr = NULL;

  if (rlc_id < RLC_UL_AM_USR_PLANE_START_LC_ID)
  {
    WL2_MSG_1(ERROR, rlc_ul_table_ptr, "No Flow Control on SRB LC asked by PS, %d",
                                                rlc_id);
    return;
  }
  ul_ctrl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id);
  if(!ul_ctrl_blk_ptr)
  {
    WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC LC ID:%d not active",
                                                rlc_id);
    return;
  }
  multi_client_fc_arbitrate(&(ul_ctrl_blk_ptr->ctrl.am_ctrl.multiclient_dl_fc_arbitarator), 
                     (uint16)RLC_DL_FC_ENTITY_PS_WM, peer_win, TRUE);

}

/*===========================================================================

FUNCTION UL_ASSIGN_CTL_BLK

DESCRIPTION
  Assign Uplink control block for an RLC entity.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint8 ul_assign_ctl_blk(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  int i;
  uint8 ret_val = INIT_CTL_BLK_ID;

  /*Locking both EUL and R99 Mutex*/
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  for (i=0; i<UE_MAX_UL_RLC_ENTITY; i++)
  {
    if (rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, i)->lc_state == UL_NULL_STATE)
    {
      ret_val = (uint8)i;
      break;
    }
  }

  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  if (i == UE_MAX_UL_RLC_ENTITY)
  {
    //ERR("Too many RLC entities", 0, 0, 0);
    WL2_MSG_1(ERROR, rlc_ul_table_ptr, "Too many RLC entities, %d",
                                                i);
  }
  
  return ret_val;
}/* ul_assign_ctl_blk() */

/*===========================================================================

FUNCTION RLC_UL_GLOBALS_INIT

DESCRIPTION
  Initializes the RLC Uplink Globals.
  
DEPENDENCIES
  This function should be called at task start up.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_globals_init(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  memset(&(rlc_ul_table_ptr->rlc_qxdm_globals), 0, sizeof(rlci_qxdm_globals_type));
  rlc_ul_table_ptr->rlc_qxdm_globals.num_max_dsm_item_per_tti = RLC_MAX_DSM_ITEM_PER_TTI_VAL;
  rlc_ul_table_ptr->rlc_qxdm_globals.num_max_dsm_item_per_pdu = RLC_MAX_DSM_ITEM_PER_PDU_VAL;
  rlc_ul_table_ptr->rlc_qxdm_globals.mac_i_flex_partial_support = TRUE;

  #ifdef FEATURE_MAC_I
  rlc_ul_table_ptr->rlc_qxdm_globals.rlc_am_poll_sufi_support = TRUE;
  #endif

  #ifdef FEATURE_WCDMA_UL_COMPR
  rlc_ul_table_ptr->rlc_qxdm_globals.rlc_qxdm_ptp_response = RLCI_PTP_MSG_RESERVED;
  rlc_ul_table_ptr->rlc_qxdm_globals.rlc_qxdm_rsp_reason = RLC_PTP_REAS_RSVD;
  #endif
}

/*===========================================================================

FUNCTION UL_ASSIGN_RTT_BLK

DESCRIPTION
  Assign RTT table index an RLC AM entity.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
uint8 ul_assign_rtt_blk(sys_modem_as_id_e_type as_id)
{
  int i;
  uint8 ret_val = 0xFF;
  rtt_delay_global_type *rtt_delay_global_ptr;
  
  rtt_delay_global_ptr = &(rtt_delay_global[as_id]);
  
  for (i=0; i<MAX_NUM_AM_CHANNELS_RTT; i++)
  {
    if( rtt_delay_global_ptr->rlc_am_rtt_table[i].in_use == FALSE)
    {
      ret_val = (uint8)i;
      break;
    }
  }
  
  if (i == MAX_NUM_AM_CHANNELS_RTT)
  {
    WL2_MSG_2(HIGH, RLCI_GET_UL_TBL_PTR(as_id), " RLC_ERR_RTT: RTT Table Full. Num active RTT Channels %d has reached Max %d ", 
                    rtt_delay_global_ptr->num_active_rtt_channels , i ); 
  }
  
  return ret_val;
}

/*===========================================================================

FUNCTION RLC_UL_INIT

DESCRIPTION
  Initializes the RLC Uplink Logical channel table.
  
DEPENDENCIES
  This function should be called at task start up.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_init(sys_modem_as_id_e_type as_id)
{
  int i;
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  dsm_item_type *item_ptr = NULL;
  
  WL2_MSG_0(LOW, rlc_ul_table_ptr, "Initialize RLC UL");
  
  rlc_enh_initialize_mutex(as_id);

  /*-------------------------------------------------------------------------
    Initialize the uplink table.
  -------------------------------------------------------------------------*/
  (void)memset(rlc_ul_table_ptr, 0, sizeof(rlci_uplink_table_type));
  rlc_ul_table_ptr->as_id = as_id;

  /*-------------------------------------------------------------------------
    Initialize the mapping table for control block id and channel id.
  -------------------------------------------------------------------------*/
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_activity_flag = DATA_TRANSFERRED;
  for (i=0; i<UE_MAX_UL_LOGICAL_CHANNEL; i++)
  {
    rlc_ul_table_ptr->ul_ctl_blk_id_tbl[i] = INIT_CTL_BLK_ID;
  }

  for (i=0; i<UE_MAX_AM_ENTITY; i++)
  {
    /* Init AM RESET channel ID table */
    rlc_ul_table_ptr->ul_reset_table.am_reset_id_table[i].rlc_id = INIT_CTL_BLK_ID;
    rlc_ul_table_ptr->ul_reset_table.am_reset_id_table[i].err_type = RLC_RESET;
  }
  rlc_ul_globals_init(rlc_ul_table_ptr);

#ifdef FEATURE_RLC_QXDM_LOGGING

  rex_def_timer(&rlc_ul_table_ptr->rlc_ul_logging.ul_tmr_log_blk, wcdma_l2_ul_get_tcb(as_id), RLC_UL_AM_LOG_TIMER_EXP_SIG);

  (void) rex_set_timer(&rlc_ul_table_ptr->rlc_ul_logging.ul_tmr_log_blk,(uint32)RLC_AM_LOG_TIMER_BASE); 

#endif

  item_ptr = dsm_new_buffer(DSM_DS_LARGE_ITEM_POOL);
    
  if (item_ptr == NULL)
  {
    WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR: Pre-allocated PDU DSM pool allocation failed %d", i, 0, 0);
  }
  item_ptr->used = 0;
  rlc_ul_table_ptr->ul_pdu_build_params.rlc_pdu_dsm_pool.item_ptr = item_ptr;
  rlc_ul_table_ptr->ul_pdu_build_params.rlc_pdu_dsm_pool.pdu_offset = 0;
  rlc_ul_table_ptr->ul_pdu_build_params.rlc_pdu_dsm_pool.num_pdu = 0;


  /*-------------------------------------------------------------------------
    Perform the AM initialization.
  -------------------------------------------------------------------------*/
  rlci_ul_am_init(rlc_ul_table_ptr);

  /*-------------------------------------------------------------------------
    Perform the UM initialization.
  -------------------------------------------------------------------------*/
  rlci_ul_um_init(rlc_ul_table_ptr);

  
  #ifdef FEATURE_WCDMA_HS_RACH
  rlci_ul_tm_init(rlc_ul_table_ptr);
  #endif /*FEATURE_WCDMA_HS_RACH*/
  
  #ifdef FEATURE_MAC_I  
  rlc_ul_table_ptr->rlc_free_mac_i_ctrl_q.num_dsm_item = 0;
  #endif

  rlc_ul_table_ptr->rlc_am_ctrl_pdu_built = 0;
#if defined (FEATURE_WCDMA_QSH_DUMP) || defined (FEATURE_WCDMA_QSH_EVENT_METRIC)
  if (SYS_MODEM_AS_ID_1 == as_id)
  {
    wrlc_qsh_init();  
  }
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  rlc_ul_table_ptr->rlc_reset_info.status = FALSE;
#endif

}/* rlc_ul_init() */

/*===========================================================================

FUNCTION CRLC_UL_CIPHER_CONFIG_HANDLER

DESCRIPTION
  Configures ciphering on one or more Unacknowledged or Acknowledged 
  Mode Channels.  This function configures the ciphering parameters that are 
  recevied from RRC. The ciphering parameters are updated only if the the 
  channel is in UL_DATA_TRANSFER_READY STATE.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_cipher_config_handler
( 
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_cipher_config_type *cipher_ptr  /* Ptr to Uplink Cipher Config.   */
)
{
  rlci_ul_ctrl_blk_type *ctl_blk = NULL;
  int i;
/* ToDo: Move logging part outside LOCK
*/
  L2_MUTEX_LOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_dl_data_process_mutex));
  /*Locking both EUL and R99 Mutex*/
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending = TRUE;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_DBG: recevied UL cipher config for nchan %d ",
                                             cipher_ptr->nchan);

  /*------------------------------------------------------------------------- 
    For each channel indicated in the Ciphering Config Request 
  -------------------------------------------------------------------------*/
  for (i = 0;i < cipher_ptr->nchan; i++)
  {
    /*----------------------------------------------------------------------- 
      Get control block index from logic channel ID.
    -----------------------------------------------------------------------*/
    ctl_blk = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, cipher_ptr->ciphering[i].lc_id);

    /*----------------------------------------------------------------------- 
      If RLC uplink is in suspended state, copy details to the uplink table
      entry.
    -----------------------------------------------------------------------*/     
    if (NULL != ctl_blk)
    {
      /*---------------------------------------------------------------------
        Copy the ciphering configuration details to the uplink_table entry.
      ---------------------------------------------------------------------*/
      if (cipher_ptr->ciphering[i].cipher_mode)
      {
        WL2_MSG_3(LOW, rlc_ul_table_ptr, "UL cipher config, LC %d, HFN %x, act_sn %x",
                                          cipher_ptr->ciphering[i].lc_id,
                                          cipher_ptr->ciphering[i].hfn,
                                          cipher_ptr->ciphering[i].ciph_act_sn);

        /*-------------------------------------------------------------------
          Backup the old ciphering configuration as RRC might abort the
          new configuration if a cell update is triggered.
        -------------------------------------------------------------------*/
        ctl_blk->cipher_backup = ctl_blk->cipher;

        /*---------------------------------------------------------------------
          UNACKNOWLEDGED Mode cipher config.
        ---------------------------------------------------------------------*/
        if (ctl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED)
        {
          switch (ctl_blk->cipher.cipher_mode)
          {
            /*--------------------------------------------------------------
              There is no PDU retx in UM. We don't need to remember the  
              current cipher_key and hfn when the cipher_mode moves from 
              CIPHER_MOD to CIPHER_ON.                                   
            ---------------------------------------------------------------*/
            case NO_CIPHER:
            case CIPHER_ON:
              /*-------------------------------------------------------------
                If the channel is directly config to CIPHER_ON mode before, 
                but not sending any data yet, take the new cipher key id, 
                HFN and ciphering algorithm. The current key, HFN and 
                ciphering algorithm will not be remembered.
              -------------------------------------------------------------*/
              if (ctl_blk->cipher.new_cipher_on)
              {
                ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 7) | 
                                        ctl_blk->ctrl.um_ctrl.vt_us;
              }
              else
              {
                if (RLCI_SEQ_GT(cipher_ptr->ciphering[i].ciph_act_sn, 
                                ctl_blk->ctrl.um_ctrl.vt_us, RLC_UM_MODULUS))
                {
                  ctl_blk->cipher.cipher_mode = 
                    (ctl_blk->cipher.cipher_mode == NO_CIPHER) ?
                    CIPHER_WAIT : CIPHER_MOD;

                  ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
                }
                else
                {
                  ctl_blk->cipher.cipher_mode = CIPHER_ON;
                  ctl_blk->cipher.new_cipher_on = TRUE;
                  ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                  ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 7) | 
                                          ctl_blk->ctrl.um_ctrl.vt_us;
                  WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Cipher ON UL UM, LC %d, key_id %d, cnt %x",
                                                             cipher_ptr->ciphering[i].lc_id, 
                                                             ctl_blk->cipher.cipher_key_id,
                                                             ctl_blk->cipher.count);
                }
                ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
                ctl_blk->cipher.act_sn= cipher_ptr->ciphering[i].ciph_act_sn;
              }
              break;

            case CIPHER_WAIT:
            case CIPHER_MOD:
              ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
              ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
              ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
              break;

            default:
              WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Invalid ciphering state");
          }
        }
        /*---------------------------------------------------------------------
          ACKNOWLEDGED Mode cipher config.
        ---------------------------------------------------------------------*/
        else if (ctl_blk->lc_mode == UE_MODE_ACKNOWLEDGED)
        {
          switch (ctl_blk->cipher.cipher_mode)
          {
            case NO_CIPHER:
            case CIPHER_ON:
              /*-------------------------------------------------------------
                If the channel is directly config to CIPHER_ON mode before, 
                but not sending any data yet, take the new cipher key id, 
                HFN and ciphering algorithm. The current ones will not be 
                remembered.
              -------------------------------------------------------------*/
              if (ctl_blk->cipher.new_cipher_on && ctl_blk->cipher.rb_id != 2)
              {
                ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 12) | 
                                        ctl_blk->ctrl.am_ctrl.ul_state.vt_s;
              }
              else
              {
                /*-----------------------------------------------------------
                  Not reaching the new activation time yet. 
                  Enter CIPHER_WAIT/CIPHER_MOD for NO_CIPHER/CIPHER_ON.
                -------------------------------------------------------------*/
                if (RLCI_SEQ_GT(cipher_ptr->ciphering[i].ciph_act_sn, 
                                ctl_blk->ctrl.am_ctrl.ul_state.vt_s, RLC_AM_MODULUS))
                {
                  ctl_blk->cipher.cipher_mode = 
                    (ctl_blk->cipher.cipher_mode == NO_CIPHER) ?
                    CIPHER_WAIT : CIPHER_MOD;

                  ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                  ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                  ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
                  ctl_blk->cipher.act_sn_new = cipher_ptr->ciphering[i].ciph_act_sn;
                }
                /*-------------------------------------------------------------
                  Reached the new activation time.
                -------------------------------------------------------------*/
                else
                {
                  /*-------------------------------------------------------------
                    Cipher is on when received this new cipher config.
                  -------------------------------------------------------------*/
                  if (ctl_blk->cipher.cipher_mode == CIPHER_ON)
                  {
                    /*-------------------------------------------------------------
                      Old config is active.
                    -------------------------------------------------------------*/
                    if (ctl_blk->cipher.act_old_cfg)
                    {
                      ctl_blk->cipher.act_old_cfg1 = TRUE;
                      ctl_blk->cipher.cipher_key_id_old1 = ctl_blk->cipher.cipher_key_id_old;
                      ctl_blk->cipher.cipher_algo_old1 = ctl_blk->cipher.cipher_algo_old;
                      ctl_blk->cipher.act_sn_old1 = ctl_blk->cipher.act_sn_old;
                      ctl_blk->cipher.count_old1 = ctl_blk->cipher.count_old;
                    }
                    /*-------------------------------------------------------------
                      No old config is active.
                    -------------------------------------------------------------*/
                    else
                    {
                      ctl_blk->cipher.act_old_cfg = TRUE;
                    }
                    /*-------------------------------------------------------------
                      Back up the current cipher config into old cipher config
                      for retx using.
                    -------------------------------------------------------------*/
                    ctl_blk->cipher.cipher_key_id_old = ctl_blk->cipher.cipher_key_id;
                    ctl_blk->cipher.cipher_algo_old = ctl_blk->cipher.cipher_algo;
                    ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
                    /*-----------------------------------------------------------------
                      Since the count_c always move 1 ahead for the next outgoing PDU,
                      We substract the current count_c by 1 to reflect the previous   
                      outgoing PDU.                                                   
                    -----------------------------------------------------------------*/
                    ctl_blk->cipher.count_old = ctl_blk->cipher.count - 1;

                    ctl_blk->cipher.new_cipher_on = TRUE;
                    /*-------------------------------------------------------------
                      Update current active cipher config.
                    -------------------------------------------------------------*/
                    ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                    ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                    ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                    ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 12) | 
                                            ctl_blk->ctrl.am_ctrl.ul_state.vt_s;
                    ctl_blk->cipher.act_sn = cipher_ptr->ciphering[i].ciph_act_sn;
                    ctl_blk->cipher.rb_id = cipher_ptr->ciphering[i].rb_id;
                  }
                  /*-------------------------------------------------------------
                    No Cipher when received this new cipher config.
                  -------------------------------------------------------------*/
                  else
                  {
                    ctl_blk->cipher.new_cipher_on = TRUE;
                    /*-------------------------------------------------------------
                      Update current active cipher config.
                    -------------------------------------------------------------*/
                    ctl_blk->cipher.cipher_key_id = cipher_ptr->ciphering[i].cipher_key_id;
                    ctl_blk->cipher.cipher_algo = cipher_ptr->ciphering[i].ciph_algo;
                    ctl_blk->cipher.hfn = cipher_ptr->ciphering[i].hfn;
                    ctl_blk->cipher.count = (cipher_ptr->ciphering[i].hfn << 12) | 
                                          ctl_blk->ctrl.am_ctrl.ul_state.vt_s;
                    ctl_blk->cipher.act_sn = cipher_ptr->ciphering[i].ciph_act_sn;
                    ctl_blk->cipher.rb_id = cipher_ptr->ciphering[i].rb_id;
                    ctl_blk->cipher.cipher_mode = CIPHER_ON;
                  }
                  WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Cipher ON UL AM, LC %d, key_id %d, cnt %x",
                                                             cipher_ptr->ciphering[i].lc_id, 
                                                             ctl_blk->cipher.cipher_key_id,
                                                             ctl_blk->cipher.count);
                }
              }
              break;

            case CIPHER_WAIT:
            case CIPHER_MOD:
              if ((ctl_blk->ctrl.am_ctrl.am_common->rb_id == 2) &&
                  (ctl_blk->cipher.act_sn_new != cipher_ptr->ciphering[i].ciph_act_sn))
              {
                /*-------------------------------------------------------------
                  Store cipher parameters for 2nd pending ciphering session
                  for SRB 2.
                -------------------------------------------------------------*/
                ctl_blk->cipher.act_sn_new1 = cipher_ptr->ciphering[i].ciph_act_sn;
                ctl_blk->cipher.cipher_key_id_new1 = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo_new1 = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn_new1 = cipher_ptr->ciphering[i].hfn;
                ctl_blk->cipher.two_pending = TRUE;
              }
              else
              {
                /*-------------------------------------------------------------
                  For non-SRB2, there is no 2nd pending session.
                  Update the key id and hfn but use the existing activation 
                  time.
                -------------------------------------------------------------*/
                ctl_blk->cipher.cipher_key_id_new = cipher_ptr->ciphering[i].cipher_key_id;
                ctl_blk->cipher.cipher_algo_new = cipher_ptr->ciphering[i].ciph_algo;
                ctl_blk->cipher.hfn_new = cipher_ptr->ciphering[i].hfn;
              }
              break;

            default:
              WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Invalid Ciphering state");
          }
         
        }
        ctl_blk->cipher.rb_id = cipher_ptr->ciphering[i].rb_id;
      }
      else
      {
        ctl_blk->cipher.cipher_mode = NO_CIPHER;
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL Cipher config, LC %d cipher OFF",
                                                   cipher_ptr->ciphering[i].lc_id);
      }
    }
    else
    {
      WL2_MSG_1(ERROR, rlc_ul_table_ptr, "UL Cipher config,LC %d not up",
                                                  cipher_ptr->ciphering[i].lc_id);
    }
  }
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending = FALSE;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  L2_MUTEX_UNLOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_dl_data_process_mutex));
#ifdef FEATURE_RLC_QXDM_LOGGING
  if (log_status(WCDMA_UL_RLC_V1_CIPH_CFG_LOG_PACKET))
  {
    WL2_MSG_0(LOW, rlc_ul_table_ptr, "LOG UL Cipher config");
    rlc_ul_tx_cipher_log_packet(rlc_ul_table_ptr, cipher_ptr);
  }
  else
  {
    WL2_MSG_0(LOW, rlc_ul_table_ptr, "NO LOG UL Cipher config");
  }
#endif

} /* crlc_ul_cipher_config_handler() */          

/*===========================================================================

FUNCTION CRLC_UL_PERFORM_SUSPEND_ACTIVITY

DESCRIPTION
  Suspends or stop one or more uplink unacknowledged or acknowledged mode channels.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_perform_suspend_or_stop_functionality
( 
  sys_modem_as_id_e_type as_id,
  rlc_ul_stop_suspend_request_type *suspend_stop_ptr,
  rlc_ul_stop_suspend_cnf_type *crlc_suspend_stop_cnf
)
{
  l2_validate_as_id(as_id);
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_ul_ctrl_blk_type *ul_ctrl_blk = NULL;
  uint8 lc_id;                        /* RLC logical Channel ID           */
#ifdef FEATURE_SMC_SRB2_SUSPENSION
  uint32 num_bytes = 0;
#endif //FEATURE_SMC_SRB2_SUSPENSION
  int i;
  rex_crit_sect_type *datapath_mutex = NULL;

  rlci_ul_state_e_type local_state = UL_NULL_STATE;
  
  crlc_suspend_stop_cnf->nchan = 0;

  /*------------------------------------------------------------------------- 
    For each channel indicated in the SUSPEND Request
  -------------------------------------------------------------------------*/
  for (i = 0; i < suspend_stop_ptr->nchan; i++)
  {
    /*----------------------------------------------------------------------- 
      Get the index of the uplink table.
    -----------------------------------------------------------------------*/
    lc_id = suspend_stop_ptr->suspend_stop[i].lc_id;
    ul_ctrl_blk = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, lc_id);

    /*----------------------------------------------------------------------- 
      Make sure the channel is in the right state and mode.
    -----------------------------------------------------------------------*/
    /*lint -save -e655 */
    if ((NULL != ul_ctrl_blk) && 
        (ul_ctrl_blk->lc_state & UL_DATA_TRANSFER_READY) && 
        (ul_ctrl_blk->lc_mode != UE_MODE_TRANSPARENT))
    /*lint -restore */     
    {
      local_state = ul_ctrl_blk->lc_state;
      local_state &= (~UL_DATA_TRANSFER_READY);
      /*lint -restore */
      /* RLC will STOP all rbs on reciving RLC_STOP cmd */
      if(suspend_stop_ptr->ul_cmd_type == RLC_STOP_CMD)
      {
        /*lint -save -e655 */
        local_state |= UL_STOP;
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Stop RLC LC %d",
                                                   suspend_stop_ptr->suspend_stop[i].lc_id);
        datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, lc_id);
        ul_ctrl_blk->lc_state = local_state; 
        
        /*lint -restore */
        if (ul_ctrl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED) 
        {
          crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
            ul_ctrl_blk->ctrl.um_ctrl.vt_us;
        }
        else /* it is UE_MODE_ACKNOWLEDGED */
        {
          crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
            ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s;
        }
        L2_ACQ_UL_LW_UNLOCK(datapath_mutex);	  
      }
      else /* incase of RLC SUSPEND */
      {

      /*--------------------------------------------------------------------- 
        Check if N == 0. 
      ---------------------------------------------------------------------*/
        if (suspend_stop_ptr->suspend_stop[i].n == 0)
        {          
          /*------------------------------------------------------------------- 
            Suspend the channel immediately 
          -------------------------------------------------------------------*/
          /* Turn on UL_SUSPENDED */
          /*lint -save -e655 */
          local_state |= UL_SUSPENDED;
          WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Suspend RLC LC %d",
                                                     suspend_stop_ptr->suspend_stop[i].lc_id);

          datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, lc_id);
          ul_ctrl_blk->lc_state = local_state;
          /*lint -restore */         
          if (ul_ctrl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED) 
          {
            crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
              ul_ctrl_blk->ctrl.um_ctrl.vt_us;
          }
          else /* it is UE_MODE_ACKNOWLEDGED */
          {
            crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
              ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s;
          }
          L2_ACQ_UL_LW_UNLOCK(datapath_mutex);		
        }
        else
        {
          /*------------------------------------------------------------------- 
            Suspend after sending the N PDUS, so set state to 
            WAITING_TO_SUSPEND. 
          -------------------------------------------------------------------*/
          /* Turn on UL_WAITING_TO_SUSPEND bit */
          /*lint -save -e655 */
          local_state |= UL_WAITING_TO_SUSPEND;
          datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, lc_id);
          ul_ctrl_blk->lc_state = local_state;
          /*lint -restore */
  
          if (ul_ctrl_blk->lc_mode == UE_MODE_UNACKNOWLEDGED) 
          {
            /* Save the number of PDUs can be sent before go suspending */
            ul_ctrl_blk->ctrl.um_ctrl.n_to_suspend = suspend_stop_ptr->suspend_stop[i].n;
          }
          else /* it is UE_MODE_ACKNOWLEDGED */ 
          {
            /* Save the number of PDUs can be sent before go suspending */
            ul_ctrl_blk->ctrl.am_ctrl.ul_state.n_to_suspend = 
                                                      suspend_stop_ptr->suspend_stop[i].n;
          }
  #ifdef FEATURE_SMC_SRB2_SUSPENSION
          if (ul_ctrl_blk->ctrl.am_ctrl.am_common->rb_id == 2)
          {
            /* Put RLC under config pending state. This prevent PDU building 
               interrupt to change the WM state when RLC calculates suspend 
               offset */
            /*lint -save -e655 */
            ul_ctrl_blk->lc_state |= UL_CONFIG_PENDING;
            /*lint -restore */
            if(ul_ctrl_blk->ul_wm_ptr != NULL)
            {
              num_bytes = RLCI_TOTAL_BYTES_UL_WM_PLUS_DEQUEUED(ul_ctrl_blk)+
                          ((uint32)RLCI_TOTAL_SDUS_UL_WM_PLUS_DEQUEUED(ul_ctrl_blk)*
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length) +
                          (ul_ctrl_blk->special_ind ?
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0) +
                          (ul_ctrl_blk->octet_less ?
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0);
              
              if (NULL != ul_ctrl_blk->cur_sdu_ptr)
              {
                num_bytes += 
                 ((ul_ctrl_blk->cur_sdu_len - ul_ctrl_blk->cur_sdu_tx_len) + 
                   ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length);
              }

            }
            else
            {
              WL2_MSG_3(HIGH, rlc_ul_table_ptr, "UL WM Unregistered on LC %d LC state %d LC Mode %d ",
                                                         lc_id,
                                                         ul_ctrl_blk->lc_state,
                                                         ul_ctrl_blk->lc_mode);
              num_bytes = (ul_ctrl_blk->special_ind ?
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0) +
                          (ul_ctrl_blk->octet_less ?
                           ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0);
              if (NULL != ul_ctrl_blk->cur_sdu_ptr)
              {
                num_bytes += 
                 ((ul_ctrl_blk->cur_sdu_len - ul_ctrl_blk->cur_sdu_tx_len) + 
                   ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length);
              }
            }
          
            ul_ctrl_blk->ctrl.am_ctrl.ul_state.n_to_suspend += (num_bytes / 
              ((ul_ctrl_blk->ctrl.am_ctrl.ul_cnfg_var->ul_rlc_size - 16) >> 3));
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
            if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_CORRUPTION)
            {            
              WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Padding_DBG: LC %d UL RLC size %d",
               suspend_stop_ptr->suspend_stop[i].lc_id, 
                                                         ul_ctrl_blk->ctrl.am_ctrl.ul_cnfg_var->ul_rlc_size);
            }
#endif
            crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
                (ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s +
                 ul_ctrl_blk->ctrl.am_ctrl.ul_state.n_to_suspend
                ) & RLC_AM_MOD_MASK;
  
            /* Has done the suspend offset calculating. Lift the bar. */
            /*lint -e64 -e641 */
            ul_ctrl_blk->lc_state &= (~UL_CONFIG_PENDING);
            /*lint +e64 +e641 */
          }
          else
  #endif //FEATURE_SMC_SRB2_SUSPENSION
          {
            crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].rlc_sn = 
                    (ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s +
                    suspend_stop_ptr->suspend_stop[i].n) & RLC_AM_MOD_MASK;
          }
          L2_ACQ_UL_LW_UNLOCK(datapath_mutex);		
        }
      }
      
      crlc_suspend_stop_cnf->cnf[crlc_suspend_stop_cnf->nchan].lc_id = lc_id;
      crlc_suspend_stop_cnf->nchan++;
    }
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d not in right state or mode",
                                                 suspend_stop_ptr->suspend_stop[i].lc_id);
    }
  }          
}/* crlc_ul_perform_suspend_or_stop_functionality() */

/*===========================================================================

FUNCTION CRLC_UL_RESUME_CNF

DESCRIPTION
  Confirms suspension of one or more logical channels.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_resume_cnf
( 
  sys_modem_as_id_e_type as_id, 
  rlc_ul_resume_cnf_type *resume_ptr    /* Ptr to resume Cnf Info       */
)
{
  rrc_cmd_type *rrc_rlc_cmd_ptr;

  rrc_rlc_cmd_ptr = rrc_get_rlc_cmd_buf();
  if (rrc_rlc_cmd_ptr != NULL)
  {
   rrc_rlc_cmd_ptr->cmd.crlc_resume_cnf = *resume_ptr;
  
   rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = RRC_CRLC_RESUME_CNF;

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
   #error code not present
#endif
   rrc_put_rlc_cmd (as_id, rrc_rlc_cmd_ptr);
  }
  else 
  { 
    WL2_MSG_0(HIGH, RLCI_GET_UL_TBL_PTR(as_id), "Fail alloc RRC cmd buf");
  }
}/* rlc_ul_resume_cnf() */


/*===========================================================================

FUNCTION CRLC_UL_RESUME_HANDLER

DESCRIPTION
  Resume transmission on one or more uplink unacknowledged or acknowledged 
  mode channels.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_resume_handler
( 
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_resume_type *resume_ptr      /* Ptr to Resume Info                */
)
{
  int   i;
  uint8 array_len = 0;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rlc_ul_resume_cnf_type *resume_cnf_ptr = &(rlc_ul_table_ptr->resume_cnf);
  
  /*Locking both EUL and R99 Mutex*/
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  resume_cnf_ptr->nchan = 0;
  array_len = sizeof(resume_cnf_ptr->channel)/sizeof(rlc_lc_id_type);
  
  for (i = 0; i < resume_ptr->nchan; i++)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, resume_ptr->channel[i]);
    /*lint -save -e64 -e641 -e655 */
    if ((NULL != ctl_blk_ptr) && 
        (ctl_blk_ptr->lc_state & UL_SUSPENDED ||
         ctl_blk_ptr->lc_state & UL_WAITING_TO_SUSPEND ))
    {
      ctl_blk_ptr->lc_state &= (~(UL_SUSPENDED | UL_WAITING_TO_SUSPEND));
      ctl_blk_ptr->lc_state |= UL_DATA_TRANSFER_READY;
      
      if (resume_cnf_ptr->nchan < array_len)
      {
        resume_cnf_ptr->channel[resume_cnf_ptr->nchan] = resume_ptr->channel[i];
        resume_cnf_ptr->nchan++;
      }
    }
    /*lint -restore */
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d not in Suspended State",
                                                 resume_ptr->channel[i]);
    }
  }

  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  /*----------------------------------------------------------------- 
    Send a confirmation back to RRC with the current RLC Resume
    Information..
  -----------------------------------------------------------------*/
  rlc_ul_resume_cnf(rlc_ul_table_ptr->as_id, resume_cnf_ptr);

}/* crlc_ul_resume_handler() */

#ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
/*===========================================================================

FUNCTION RLC_UL_REGISTER_TRIGGER_BASED_RACH_API_ACTIVE_RBS

DESCRIPTION
  Register trigger based RACH API for all the active RB's.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_register_trigger_based_rach_api_active_rbs(mac_ul_table_type *ul_table_ptr)
{
  uint8 i, count;
  boolean is_l1_in_fach;
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(ul_table_ptr->as_id);
  
  is_l1_in_fach = l1m_state_is_fach(ul_table_ptr->as_id);
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  
  if(TRUE == is_l1_in_fach)
  {
    count = rlc_ul_table_ptr->active_ch;
    i = 0;
    while (count > 0 && i < UE_MAX_UL_RLC_ENTITY) 
    {
      ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, i);
      if ((NULL != ctl_blk_ptr) && 
          (UL_NULL_STATE != ctl_blk_ptr->lc_state)) 
      {
        count--;
        if (NULL != ctl_blk_ptr->ul_wm_ptr)
        {
          dsm_set_wm_cb(ctl_blk_ptr->ul_wm_ptr,DSM_WM_NON_EMPTY,TRUE,mac_ul_trigger_tx_req,TRUE,
                                                                  (void *)ul_table_ptr,FALSE,0);
        }
      }
      i++;
    }
    if(0 != count)
    {
      WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "TRIG_RACH: Registeration Failed for %d Active RB's %d",count,0,0);
    }
  }
  else
  {
    WL2_MSG_1(ERROR, rlc_ul_table_ptr, "TRIG_RACH: L1 not in FACH during RACH disable %d",
                                                is_l1_in_fach);
  }
}/* rlc_ul_setup_tx_trigger() */

/*===========================================================================

FUNCTION RLC_UL_REGISTER_TRIGGER_BASED_API_PER_LC

DESCRIPTION
  Register trigger based RACH API or trigger RACH if data is already present 
  for the given RLC ID.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_register_trigger_based_api_per_lc(rlc_lc_id_type rlc_id, 
                                              rlci_ul_ctrl_blk_type *ctrl_blk_ptr,
                                              mac_ul_table_type *ul_table_ptr)
{
  if ((NULL != ctrl_blk_ptr) && (NULL != ctrl_blk_ptr->ul_wm_ptr))
  {
    if ( RLCI_TOTAL_BYTES_UL_WM_PLUS_DEQUEUED(ctrl_blk_ptr) == 0)
    {
      WL2_MSG_2(HIGH, ul_table_ptr, "TRIG_RACH: Register Trigger based RACH API - LC %d, count %d",
                                                 rlc_id,
                                                 RLCI_TOTAL_BYTES_UL_WM_PLUS_DEQUEUED(ctrl_blk_ptr));
      dsm_set_wm_cb(ctrl_blk_ptr->ul_wm_ptr,DSM_WM_NON_EMPTY,TRUE,mac_ul_trigger_tx_req,TRUE,
                                                                 (void *)ul_table_ptr,FALSE,0);
    }
    else
    {
      WL2_MSG_1(HIGH, ul_table_ptr, "TRIG_RACH: Enable RACH as data is already present for - LC %d",
                                                 rlc_id);
      mac_ul_trigger_tx_req(NULL,(void *)ul_table_ptr);
    }
  }
  else
  {
    WL2_MSG_2(ERROR, ul_table_ptr, "TRIG_RACH: Invalid parameter passed for LC_ID: %d 0x%x",
                                                rlc_id, ctrl_blk_ptr);
  }
}
#endif /* FEATURE_WCDMA_TRIGGER_BASED_RACH */

/*===========================================================================

FUNCTION RLC_UL_HFN_CNF

DESCRIPTION
  
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_hfn_cnf
(
  sys_modem_as_id_e_type as_id,
  rlc_ul_hfn_cnf_type *hfn_ptr        /* Ptr to HFN Confirm info           */
)
{
  rrc_cmd_type *rrc_rlc_cmd_ptr;      

  rrc_rlc_cmd_ptr = rrc_get_rlc_cmd_buf();
  if (rrc_rlc_cmd_ptr != NULL)
  {
    rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = RRC_CRLC_UL_HFN_CNF;
    rrc_rlc_cmd_ptr->cmd.crlc_ul_hfn_cnf = *hfn_ptr;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif
    rrc_put_rlc_cmd(as_id, rrc_rlc_cmd_ptr);
  }
  else 
  { 
    WL2_MSG_0(HIGH, RLCI_GET_UL_TBL_PTR(as_id), "Could not get command buf");
  }
}/* rlc_ul_hfn_cnf() */


/*===========================================================================

FUNCTION CRLC_UL_HFN_HANDLER

DESCRIPTION
  Gets the hfn value for one or more uplink unacknowledged or acknowledged 
  mode logical channels
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_hfn_handler 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_hfn_type *hfn_ptr              /* Ptr to HFN Info                 */
)
{
  rlci_ul_ctrl_blk_type      *ctl_blk_ptr;
  uecomdef_logch_mode_e_type lc_mode;   /* Logical Channel Mode            */
  uint16                     index;     /* Index into Downlink Table       */   
  uint16                     count;     /* Number of active Channels       */
  uint8 array_len = 0;
  rlc_ul_hfn_cnf_type *ul_hfn_cfn_ptr = &(rlc_ul_table_ptr->ul_hfn_cnf);
  
/*
  ToDo: need to move outside the LOCK
*/
  /*Locking both EUL and R99 Mutex*/
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  /*------------------------------------------------------------------------
    Initialize the number of channels and index to 0.
  ------------------------------------------------------------------------*/
  ul_hfn_cfn_ptr->nchan = 0;
  index = 0;

  /*------------------------------------------------------------------------
    Get the number of active channels from the uplink table.
  ------------------------------------------------------------------------*/
  count = rlc_ul_table_ptr->active_ch;
  array_len = sizeof(ul_hfn_cfn_ptr->hfn_info)/sizeof(rlc_ul_hfn_info_type);
  /*-------------------------------------------------------------------------
    If the HFN for all unacknowledged and acknowledged is requested, 
    get the HFN for all the active unacknowledged and acknowledged mode
    channels from the uplink table and call the function crlc_ul_hfn_cnf()
    to send the information to RRC.
  -------------------------------------------------------------------------*/
  if (hfn_ptr->all_or_one == RLC_ALL)
  {
    while((count > 0) && (ul_hfn_cfn_ptr->nchan < array_len))
    {
      ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
      if ((NULL != ctl_blk_ptr) && 
      	  (ctl_blk_ptr->lc_state != UL_NULL_STATE))  
      {
        count--;
        lc_mode = ctl_blk_ptr->lc_mode;
        if (lc_mode != UE_MODE_TRANSPARENT)
        {
          if (lc_mode == UE_MODE_UNACKNOWLEDGED)
          {
            ul_hfn_cfn_ptr->hfn_info[ul_hfn_cfn_ptr->nchan].lc_id = 
              ctl_blk_ptr->ctrl.um_ctrl.rlc_id;
          }
          else if (lc_mode == UE_MODE_ACKNOWLEDGED) 
          {
            ul_hfn_cfn_ptr->hfn_info[ul_hfn_cfn_ptr->nchan].lc_id = 
              ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id;
          }
          ul_hfn_cfn_ptr->hfn_info[ul_hfn_cfn_ptr->nchan].hfn = ctl_blk_ptr->cipher.hfn;
          ul_hfn_cfn_ptr->nchan++;
        }
      }
      else
      {
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d does not exist",
                                                   index);
      }
      index++;
    }
    rlc_ul_hfn_cnf(rlc_ul_table_ptr->as_id, ul_hfn_cfn_ptr);
  }
  else
  {
    /*-----------------------------------------------------------------------
      If the HFN for a specific logical channel is requested,get the HFN for 
      the channel, if it is active, and call the function crlc_ul_hfn_cnf()
      to send the information to RRC.
    -----------------------------------------------------------------------*/
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, hfn_ptr->lc_id);
    if ((NULL != ctl_blk_ptr) && 
        (ctl_blk_ptr->lc_state != UL_NULL_STATE) &&
        (ctl_blk_ptr->lc_mode != UE_MODE_TRANSPARENT))
    {
      ul_hfn_cfn_ptr->hfn_info[ul_hfn_cfn_ptr->nchan].lc_id = hfn_ptr->lc_id;
      ul_hfn_cfn_ptr->hfn_info[ul_hfn_cfn_ptr->nchan].hfn  = ctl_blk_ptr->cipher.hfn;
      ul_hfn_cfn_ptr->nchan++;
      rlc_ul_hfn_cnf(rlc_ul_table_ptr->as_id, ul_hfn_cfn_ptr);
    }
    else
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC ID %d is not in the right state or mode",
                                                 hfn_ptr->lc_id);
  }
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);  
}/* crlc_ul_hfn_handler() */

/*===========================================================================

FUNCTION rlci_get_config_action

DESCRIPTION
  this function checks all the bits of the bitmask and return the
  corresponding action set by RRC to RLC at the tme of channel config req
    
DEPENDENCIES
  None.
  
RETURN VALUE
  action type
  
SIDE EFFECTS
  None.

===========================================================================*/

rlc_channel_action_cnf_e_type rlci_get_config_action( rlci_downlink_table_type *rlc_dl_table_ptr, uint16 *cfg_mask)
{
  if ((*cfg_mask & RLC_MODIFY_CFG) == RLC_MODIFY_CFG)
  {
    (*cfg_mask) &=~RLC_MODIFY_CFG; 
    return RLCI_MODIFY;
  }
  else if((*cfg_mask & RLC_STOP_CFG) == RLC_STOP_CFG)
  {
    (*cfg_mask) &=~RLC_STOP_CFG; 
    return RLCI_STOP;
  }
  else if((*cfg_mask & RLC_CONTINUE_CFG) == RLC_CONTINUE_CFG)
  {
    (*cfg_mask) &=~RLC_CONTINUE_CFG; 
    return RLCI_CONTINUE;
  }
  else if((*cfg_mask & RLC_RE_ESTABLISH_CFG) == RLC_RE_ESTABLISH_CFG)
  {
    (*cfg_mask) &=~RLC_RE_ESTABLISH_CFG; 
    return RLCI_RE_ESTABLISH;
  }
  else
  {
    *cfg_mask = 0;
    return RLCI_NO_ACTION;
  }

}


/*===========================================================================

FUNCTION CRLC_UL_TM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Transparent Mode Logical 
  channels. This function is called when RRC sends a CRLC_UL_TM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to UL_DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the 
  UL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL
  state and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  UL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.

    
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/

void crlc_ul_tm_config_handler 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_tm_config_type *tm_ptr  /* Ptr to Transparent Mode Config Info    */
)
{
  rrc_cmd_type *rrc_rlc_cmd_ptr; /* Command buffer to store confirm        */ 
  uint8 index;                   /* Index into Downlink Table              */
  int i;
  rlc_channel_action_cnf_e_type action;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
#ifdef FEATURE_RLC_QXDM_LOGGING
  uint8 j = 0;
  uint8 k = 0;
  rlc_ul_tm_config_type *tm_log_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_tm_log_ptr);
  rlc_ul_log_state_type *log_state_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_log_state);
#endif  

  /*-------------------------------------------------------------------------
    Get a command buffer to store the confirmation information to send to 
    RRC.
  -------------------------------------------------------------------------*/
  rrc_rlc_cmd_ptr = rrc_get_rlc_cmd_buf();
  if (rrc_rlc_cmd_ptr == NULL)
  {
    WL2_MSG_0(HIGH, rlc_ul_table_ptr, "UL TM:Could not get cmd buffer");
    return;
  }
  rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = RRC_CRLC_UL_TM_CONFIG_CNF;
  rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.nchan = tm_ptr->nchan;

  L2_MUTEX_LOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_dl_data_process_mutex));
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending = TRUE;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
   L2_ACQ_DL_LOCK();
#ifdef FEATURE_TM_LB
  if((tm_ptr->nchan == 0) && (tm_ptr->loop_back_mode == RLC_LOOPBACK_MODE_2))
  {
    /*-----------------------------------------------------------------------
      All DTCH TM channels will now operate in loop back mode2.
    -----------------------------------------------------------------------*/
    for(i = 0; i < UE_MAX_UL_TM_CHANNEL; ++i)
    {
      ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, i);
      if((NULL != ctl_blk_ptr) &&
         (ctl_blk_ptr->lc_type == UE_LOGCHAN_DTCH))
      {
        ctl_blk_ptr->ctrl.tm_ctrl.lpm = RLC_LOOPBACK_MODE_2;
      }
    }
  }
#endif /* FEATURE_TM_LB */

  for (i = 0; i < tm_ptr->nchan; i++)
  {
    rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.tm_config_result[i].lc_id = 
                                                  tm_ptr->chan_info[i].lc_id;
    if (tm_ptr->chan_info[i].lc_id >= UE_MAX_UL_LOGICAL_CHANNEL) 
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Invalid UL TM LC Id %d",
                                                 tm_ptr->chan_info[i].lc_id);
      rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                    tm_config_result[i].result = FAILURE;
    }
    else
    {
      switch(tm_ptr->ul_tm_act[i].action)
      {
        case RLC_ESTABLISH:
        /*-------------------------------------------------------------------
          Check if the channel exists. If it does not, get an index 
          into the downlink table and store all the information for the 
          channel. Send a SUCCESS indication in the confirm message to RRC 
        -------------------------------------------------------------------*/
        ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, tm_ptr->chan_info[i].lc_id);
	if ((NULL == ctl_blk_ptr) &&
            ((index = ul_assign_ctl_blk(rlc_ul_table_ptr)) != INIT_CTL_BLK_ID))
        {
          WL2_MSG_2(LOW, rlc_ul_table_ptr, "UL TM Establish LC %d, Index %d",
                                            tm_ptr->chan_info[i].lc_id,
                                            index);

          if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
          {
            rlc_dump_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
          }
          
          rlc_ul_table_ptr->ul_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id] = index;

          if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
          {
            rlc_sanitize_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
          }
          
	  ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
          ctl_blk_ptr->lc_mode = UE_MODE_TRANSPARENT;
          ctl_blk_ptr->lc_type = tm_ptr->chan_info[i].lc_type;
          ctl_blk_ptr->mac_i_ps_sn = RLC_INVALID_SN;
          ctl_blk_ptr->ctrl.tm_ctrl.rlc_id = tm_ptr->chan_info[i].lc_id;
          ctl_blk_ptr->ctrl.tm_ctrl.seg_ind = tm_ptr->chan_info[i].seg_ind;
          ctl_blk_ptr->ctrl.tm_ctrl.act_incl = tm_ptr->ul_tm_act[i].act_incl;
          ctl_blk_ptr->ctrl.tm_ctrl.act_time = tm_ptr->ul_tm_act[i].act_time;
          ctl_blk_ptr->ctrl.tm_ctrl.timer_discard = tm_ptr->chan_info[i].timer_discard;
#ifdef FEATURE_TM_LB
          ctl_blk_ptr->ctrl.tm_ctrl.lpm = RLC_LOOPBACK_NA;
#endif /* FEATURE_TM_LB */
          /*
             Initially LC will be in NULL state, so wait till ISR finishes
          */
          ctl_blk_ptr->lc_state = UL_DATA_TRANSFER_READY;
          ctl_blk_ptr->ul_wm_ptr = NULL;
          
          rlc_ul_table_ptr->active_ch++;
          rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                          tm_config_result[i].result = SUCCESS;
#ifdef FEATURE_RLC_QXDM_LOGGING
          tm_log_ptr->chan_info[j] = tm_ptr->chan_info[i];
          j++;
          log_state_ptr->ul_state[k].rlc_id = tm_ptr->chan_info[i].lc_id;
          log_state_ptr->ul_state[k].lc_state = TM_UL_DATA_TRANSFER_READY;
          k++;
#endif
        }
        else
        {
          /*-----------------------------------------------------------------
            If the channel exists, send a FAILURE indication in the confirm 
            message to RRC.
          -----------------------------------------------------------------*/
          rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                               tm_config_result[i].result = FAILURE;
          WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d exists - Cannot Establish",
                                                     tm_ptr->chan_info[i].lc_id);
        }
        break;
        case RLC_RELEASE:
        /*-------------------------------------------------------------------
          Check if the channel is in NULL state. If it is not, release 
          the channel and send a SUCCESS indication in the confirm message 
          to RRC.  
        -------------------------------------------------------------------*/
        index = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, tm_ptr->chan_info[i].lc_id);
	if (index < UE_MAX_UL_RLC_ENTITY)
        {
          WL2_MSG_2(HIGH, rlc_ul_table_ptr, "UL TM Release LC %d, Index %d",
                                                     tm_ptr->chan_info[i].lc_id,
                                                     index);
          /*
            wait till UL ISR finishes
          */
          ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
          ctl_blk_ptr->lc_state = UL_NULL_STATE;
          
          /* Release discard timer  */
          if(ctl_blk_ptr->tmr_discard_ptr != NULL)
          {
            (void) rex_clr_timer(ctl_blk_ptr->tmr_discard_ptr);
            #ifdef FEATURE_REX_DYNA_MEM
            /* release timer block */
            rex_delete_timer_ex(ctl_blk_ptr->tmr_discard_ptr);
            #endif
            /* Reset timer sig */
            ctl_blk_ptr->tmr_discard_ptr = NULL;
            WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Release SDU discard timer, LC %d",
                                                       tm_ptr->chan_info[i].lc_id);
          }

          rlc_ul_table_ptr->active_ch--;

          if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
          {
            rlc_dump_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
          }

          rlc_ul_table_ptr->ul_ctl_blk_id_tbl[tm_ptr->chan_info[i].lc_id] = INIT_CTL_BLK_ID;

          if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
          {
            rlc_sanitize_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
          }        
          
          rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.tm_config_result[i].result = SUCCESS;
#ifdef FEATURE_RLC_QXDM_LOGGING
          log_state_ptr->ul_state[k].rlc_id = tm_ptr->chan_info[i].lc_id;
          log_state_ptr->ul_state[k].lc_state = RLC_UL_NULL_STATE;
          k++;
#endif
        }
        else
        {
          /*-----------------------------------------------------------------
            If the channel does not exist, send a FAILURE indication in the 
           confirm message to RRC.
          -----------------------------------------------------------------*/
          rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                               tm_config_result[i].result = FAILURE;
          WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d does'nt exist - Cannot Release",
                                                     tm_ptr->chan_info[i].lc_id);
        }
        break;
      case RLC_MODIFY:

        rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                               tm_config_result[i].result = SUCCESS;

        if(tm_ptr->ul_tm_act[i].action == RLC_MODIFY)
        {
          while(tm_ptr->ul_tm_act[i].ul_cfg_mask != 0)
          {
            action = rlci_get_config_action( RLCI_GET_DL_TBL_PTR(rlc_ul_table_ptr->as_id) ,
                                             (&(tm_ptr->ul_tm_act[i].ul_cfg_mask)) );
            switch(action)
            {
              case RLCI_MODIFY:
              /*-------------------------------------------------------------------
                Check if Channel is in NULL state. If it is not, modify
                the channel parameters and send a SUCCESS indication in the
                confirm message to RRC.
              -------------------------------------------------------------------*/
              index = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, tm_ptr->chan_info[i].lc_id);
	      if (index < UE_MAX_UL_RLC_ENTITY)
              {
                WL2_MSG_2(LOW, rlc_ul_table_ptr, "UL TM Modify LC: %d, Index: %d",
                                                  i, index);
                /*
                  Entire TM entity is modified here, so protection is verym uch needed
                */
                ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
                ctl_blk_ptr->lc_type = tm_ptr->chan_info[i].lc_type;
                ctl_blk_ptr->ctrl.tm_ctrl.seg_ind = tm_ptr->chan_info[i].seg_ind;
                ctl_blk_ptr->ctrl.tm_ctrl.act_incl = tm_ptr->ul_tm_act[i].act_incl;
                ctl_blk_ptr->ctrl.tm_ctrl.act_time = tm_ptr->ul_tm_act[i].act_time;
                ctl_blk_ptr->ctrl.tm_ctrl.timer_discard = tm_ptr->chan_info[i].timer_discard;
#ifdef FEATURE_TM_LB
                ctl_blk_ptr->ctrl.tm_ctrl.lpm = RLC_LOOPBACK_NA;
#endif /* FEATURE_TM_LB */
                rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                            tm_config_result[i].result = SUCCESS;
      #ifdef FEATURE_RLC_QXDM_LOGGING
                if (j < UE_MAX_UL_TM_CHANNEL)
                {
                  tm_log_ptr->chan_info[j] = tm_ptr->chan_info[i];
                  j++;
                }
      #endif
              }
              else
              {
                /*-----------------------------------------------------------------
                  If the channel does not exist, send a FAILURE indication in the
                  confirm message to RRC.
                -----------------------------------------------------------------*/
                WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d not in Right State - Cannot Mod",
                                                           tm_ptr->chan_info[i].lc_id);
                rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.tm_config_result[i].result = FAILURE;
              }
              break;
         
              default:
                rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                                   tm_config_result[i].result = FAILURE;
                WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL TM:Invalid Action %d",
                                                           tm_ptr->ul_tm_act[i].action);
            }
          }
        }
        break;

      default:
        rrc_rlc_cmd_ptr->cmd.crlc_ul_tm_config_cnf.
                             tm_config_result[i].result = FAILURE;
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL TM:Invalid Action %d",
                                                   tm_ptr->ul_tm_act[i].action);
      }
    }
  }
  L2_ACQ_DL_UNLOCK();

  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending = FALSE;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  L2_MUTEX_UNLOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_dl_data_process_mutex));
  
#ifdef FEATURE_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    tm_log_ptr->nchan = j;
    rlc_ul_send_tm_log_packet(rlc_ul_table_ptr, tm_log_ptr);
  }
  if (k > 0) 
  {
    log_state_ptr->nchan = k;
    rlc_ul_send_state_log_packet(rlc_ul_table_ptr, log_state_ptr);
  }
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  rrc_put_rlc_cmd(rlc_ul_table_ptr->as_id, rrc_rlc_cmd_ptr);

}/* crlc_ul_tm_config_handler() */


/*===========================================================================

FUNCTION CRLC_UL_UM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of Unacknowledged Mode Logical 
  channels. This function is called when RRC sends a CRLC_UL_UM_CONFIG_REQ
  to RLC. If a new channel is to be established, the function checks if
  the channels exists. If it already exists, and ERR message is dispalyed.
  If the channel does not exist, the downlink table entry for that channel
  is updated and the state is set to UL_DATA_TRANSFER_READY. If a channel
  is to be released,the function checks if the channel is in the 
  UL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL 
  state and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  UL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the
  channel is updated with the configuration received from RRC.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_um_config_handler 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_um_config_type *um_ptr  /* Ptr to Unacknowledged Mode Config Info */
)
{
  rrc_cmd_type *rrc_rlc_cmd_ptr; /* Command Buf for storing Confirm        */
  uint8       index;             /* Index into Uplink Table                */      
  int          i;
  rlc_channel_action_cnf_e_type action;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rlc_mutexes_type *rlc_mutex_ptr = RLC_GET_MUTEX_PTR(rlc_ul_table_ptr->as_id);
#ifdef FEATURE_RLC_QXDM_LOGGING
  uint8 j = 0;
  uint8 k = 0;
  rlc_ul_um_config_type *um_log_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_um_log_ptr);
  rlc_ul_log_state_type *log_state_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_log_state);
  boolean est_or_reest[UE_MAX_UL_UM_CHANNEL];
#endif  
                                                        
  /*-------------------------------------------------------------------------
    Get a command buffer to store the confirmation information to send to 
    RRC.
  -------------------------------------------------------------------------*/
  rrc_rlc_cmd_ptr = rrc_get_rlc_cmd_buf();
  if (rrc_rlc_cmd_ptr == NULL)
  {
    WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Could not get cmd buffer");
    return;
  }
  rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = RRC_CRLC_UL_UM_CONFIG_CNF;
  rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.nchan = um_ptr->nchan;

  L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_dl_data_process_mutex));

  /*Locking both EUL and R99 Mutex*/
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending = TRUE;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  L2_ACQ_DL_LOCK();

  for (i = 0; i < um_ptr->nchan; i++)
  {
    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].lc_id = 
                                                  um_ptr->chan_info[i].lc_id;
    if (um_ptr->chan_info[i].lc_id >= UE_MAX_UL_LOGICAL_CHANNEL) 
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Invalid UL UM LC Id %d",
                                                 um_ptr->chan_info[i].lc_id);
      rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                    um_config_result[i].result = FAILURE;
    }
    else
    {
      switch(um_ptr->ul_um_act[i].action)
      {
        case RLC_ESTABLISH:
          /*-------------------------------------------------------------------
            Check if the channel exists. If it does not, get an index 
            into the downlink table and store all the information for the 
            channel. Send a SUCCESS indication in the confirm message to RRC 
          -------------------------------------------------------------------*/
          ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, um_ptr->chan_info[i].lc_id);
	  if ((NULL == ctl_blk_ptr) &&
              ((index = ul_assign_ctl_blk(rlc_ul_table_ptr)) != INIT_CTL_BLK_ID))
          {
            WL2_MSG_3(LOW, rlc_ul_table_ptr, "UL UM Establish, LC %d Index %d, Alt E-bit = %d",
                                              um_ptr->chan_info[i].lc_id,
                                              index,
                                              um_ptr->chan_info[i].alternate_e_bit);
            if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
            {
              rlc_dump_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
            }

            rlc_ul_table_ptr->ul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id] = index;

            if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
            {
              rlc_sanitize_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
            }          
            
	    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
            ctl_blk_ptr->lc_mode = UE_MODE_UNACKNOWLEDGED;
            ctl_blk_ptr->lc_type = um_ptr->chan_info[i].lc_type;
            ctl_blk_ptr->ctrl.um_ctrl.rlc_id = um_ptr->chan_info[i].lc_id;
            ctl_blk_ptr->ctrl.um_ctrl.act_incl = um_ptr->ul_um_act[i].act_incl;
            ctl_blk_ptr->ctrl.um_ctrl.act_time = um_ptr->ul_um_act[i].act_time;
            ctl_blk_ptr->ctrl.um_ctrl.timer_discard = um_ptr->chan_info[i].timer_discard;
            ctl_blk_ptr->ctrl.um_ctrl.alternate_e_bit = um_ptr->chan_info[i].alternate_e_bit;
            ctl_blk_ptr->cipher.cipher_mode = NO_CIPHER;
            ctl_blk_ptr->cipher.cipher_all = FALSE;
            ctl_blk_ptr->cipher.hfn = 0;
            ctl_blk_ptr->cipher.new_cipher_on = FALSE;
            #ifdef FEATURE_MAC_I
            if(RLC_FLEXIBLE_PDU_SIZE == um_ptr->chan_info[i].ul_flex_pdu_size)
            {
              ctl_blk_ptr->ctrl.um_ctrl.ul_flex_pdu = TRUE;
              
              ctl_blk_ptr->ctrl.um_ctrl.li_length =
                ((um_ptr->chan_info[i].ul_flex_max_pdu_size >> 3) <= 125) ? 1 : 2;
            }
            else
            #endif
            {
#ifdef FEATURE_MAC_I
              ctl_blk_ptr->ctrl.um_ctrl.ul_flex_pdu = FALSE;
#endif
              ctl_blk_ptr->ctrl.um_ctrl.li_length =
                    ((um_ptr->chan_info[i].rlc_size >> 3) <= 125) ? 1 : 2;
            }
            ctl_blk_ptr->ctrl.um_ctrl.sn_wrap = FALSE;
            ctl_blk_ptr->ctrl.um_ctrl.vt_us = 0;
            ctl_blk_ptr->special_ind = FALSE;
            ctl_blk_ptr->octet_less = FALSE;
            ctl_blk_ptr->padding = FALSE;
            
            ctl_blk_ptr->lc_state = UL_DATA_TRANSFER_READY;
            ctl_blk_ptr->ul_wm_ptr = NULL;
            ctl_blk_ptr->cur_sdu_ptr = NULL;
            ctl_blk_ptr->dequeue_sdu_head_ptr = NULL; /* Only used for AM Mode to hold dequeued DSMs */
            ctl_blk_ptr->dequeue_last_ack_ptr = NULL; /* Only used for AM Mode to hold dequeued DSMs */
            ctl_blk_ptr->cur_sdu_len = 0;
            ctl_blk_ptr->cur_sdu_tx_len = 0;
            
            rlc_ul_table_ptr->active_ch++;
            rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                 um_config_result[i].result = SUCCESS;
#ifdef FEATURE_QXDM_DSM_TRACING
            if (ctl_blk_ptr->ctrl.um_ctrl.rlc_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              if (rlc_ul_table_ptr->rlc_dsm_trace_params.num_rlc_non_srb_entities_up++ == 0)
              {
                fc_post_reg_cmd_dsm_trace
                  (DSM_UMTS_TECH,
                   UMTS_RLC_CLIENT,
                   &rlc_fc_get_dsm_trace_cb,
                   (0x5EADBEEF | rlc_ul_table_ptr->as_id << 31)); /* MSB is as_id */
                WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Registered Qxdm DSM tracing");
              }
            }
#endif
  #ifdef FEATURE_RLC_QXDM_LOGGING
            ctl_blk_ptr->ul_pdu_logging = FALSE;
            um_log_ptr->chan_info[j] = um_ptr->chan_info[i];
            est_or_reest[j] = TRUE;
            j++;
            log_state_ptr->ul_state[k].rlc_id = um_ptr->chan_info[i].lc_id;
            log_state_ptr->ul_state[k].lc_state = UM_UL_DATA_TRANSFER_READY;
            k++;
            memset(&(ctl_blk_ptr->ctrl.um_ctrl.ul_chan_um_pdu), 0, sizeof(rlc_ul_chan_um_pdu_type));
  #endif
          }
          else
          {
            /*-----------------------------------------------------------------
              If the channel exists, send a FAILURE indication in the confirm 
              message to RRC.
            -----------------------------------------------------------------*/
            WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d exist or No ctl blk",
                                                       um_ptr->chan_info[i].lc_id);
            rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = FAILURE;
          }
          break;
        case RLC_RELEASE:
	  L2_MUTEX_LOCK(&rlc_mutex_ptr->rlc_channel_release_mutex);
          /*-------------------------------------------------------------------
            Check if the channel is in NULL state. If it is not, release 
            the channel and send a SUCCESS indication in the confirm message 
            to RRC.  
          -------------------------------------------------------------------*/
          index = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, um_ptr->chan_info[i].lc_id);
          if (index < UE_MAX_UL_RLC_ENTITY)
          {
            ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
            ctl_blk_ptr->lc_state = UL_NULL_STATE;
            
            WL2_MSG_2(HIGH, rlc_ul_table_ptr, "UL UM Release, LC %d Index %d",
                                                       um_ptr->chan_info[i].lc_id,
                                                       index);
            /* Release discard timer  */
            if(ctl_blk_ptr->tmr_discard_ptr != NULL)
            {
              (void) rex_clr_timer(ctl_blk_ptr->tmr_discard_ptr);
              #ifdef FEATURE_REX_DYNA_MEM
              /* release timer block */
              rex_delete_timer_ex(ctl_blk_ptr->tmr_discard_ptr);
              #endif 
              /* Reset timer sig */
              ctl_blk_ptr->tmr_discard_ptr = NULL;
              WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Release SDU discard timer, LC %d",
                                                         um_ptr->chan_info[i].lc_id);
            }
            rlc_ul_table_ptr->active_ch--;

            if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
            {
              rlc_dump_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
            }
            rlc_ul_table_ptr->ul_ctl_blk_id_tbl[um_ptr->chan_info[i].lc_id] = INIT_CTL_BLK_ID;

            if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
            {
              rlc_sanitize_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
            }
            
            rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                 um_config_result[i].result = SUCCESS;
#ifdef FEATURE_QXDM_DSM_TRACING
            if (ctl_blk_ptr->ctrl.um_ctrl.rlc_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              if (rlc_ul_table_ptr->rlc_dsm_trace_params.num_rlc_non_srb_entities_up > 0)
              {
                if (--rlc_ul_table_ptr->rlc_dsm_trace_params.num_rlc_non_srb_entities_up == 0)
                {
                  fc_post_deregister_cmd_dsm_trace
                    (DSM_UMTS_TECH,
                     UMTS_RLC_CLIENT);                     
                  WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Deregistered Qxdm DSM tracing");
                }
              }
              else
              {
                WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR::Removing nonsrb entitiy, when num_non_srb entity is zero",0,0,0);
              }
            }
#endif

  #ifdef FEATURE_RLC_QXDM_LOGGING
            log_state_ptr->ul_state[k].rlc_id = um_ptr->chan_info[i].lc_id;
            log_state_ptr->ul_state[k].lc_state = RLC_UL_NULL_STATE;
            k++;
  #endif
          }
          else
          {
            /*-----------------------------------------------------------------
              If the channel does not exist, send a FAILURE indication in the 
              confirm message to RRC.
            -----------------------------------------------------------------*/
            WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d not config'd - No Release",
                                                       um_ptr->chan_info[i].lc_id);
            rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = FAILURE;
          }
          L2_MUTEX_UNLOCK(&rlc_mutex_ptr->rlc_channel_release_mutex);
          break;

        case RLC_MODIFY:
          rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = SUCCESS;

          if(um_ptr->ul_um_act[i].action == RLC_MODIFY)
          {
            while(um_ptr->ul_um_act[i].ul_cfg_mask != 0)
            {
              action = rlci_get_config_action( RLCI_GET_DL_TBL_PTR(rlc_ul_table_ptr->as_id) ,
                                               (&(um_ptr->ul_um_act[i].ul_cfg_mask)) );
              switch(action)
              {
                case RLCI_MODIFY:
                  /*-------------------------------------------------------------------
                    Check if Channel is in NULL state. If it is not, modify
                    the channel parameters and send a SUCCESS indication in the
                    confirm message to RRC.
                  -------------------------------------------------------------------*/
                  index = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, um_ptr->chan_info[i].lc_id);
		  if (index < UE_MAX_UL_RLC_ENTITY)
                  {
                    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
                    ctl_blk_ptr->lc_type = um_ptr->chan_info[i].lc_type;
                    ctl_blk_ptr->ctrl.um_ctrl.act_incl = um_ptr->ul_um_act[i].act_incl;
                    ctl_blk_ptr->ctrl.um_ctrl.act_time = um_ptr->ul_um_act[i].act_time;
                    

                    if (um_ptr->chan_info[i].timer_discard !=
                        ctl_blk_ptr->ctrl.um_ctrl.timer_discard)
                    {
                      /*---------------------------------------------------------
                        if discard timer values changes it update the
                         discard timer pointer
                      ---------------------------------------------------------*/
                      if (um_ptr->chan_info[i].timer_discard == 0)
                      {
                          /*-----------------------------------------------------
                            if discard timer is tunred OFF which was previosly ON 
                          -----------------------------------------------------*/
                        (void) rex_clr_timer(ctl_blk_ptr->tmr_discard_ptr);
                        #ifdef FEATURE_REX_DYNA_MEM
                        rex_delete_timer_ex(ctl_blk_ptr->tmr_discard_ptr);
                        #endif
                        
                        ctl_blk_ptr->tmr_discard_ptr = NULL;
                        ctl_blk_ptr->ctrl.um_ctrl.timer_discard = um_ptr->chan_info[i].timer_discard;

                        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Release SDU discard timer, LC %d",
                                                                   um_ptr->chan_info[i].lc_id);
                      } 
                      else
                      {
                        if (ctl_blk_ptr->ctrl.um_ctrl.timer_discard == 0)
                        {
                          /*-------------------------------------------------------
                          if  discard timer is turned ON which was previosly OFF. 
                          --------------------------------------------------------*/ 
                          ctl_blk_ptr->ctrl.um_ctrl.timer_discard = 
                                                           um_ptr->chan_info[i].timer_discard;
                          rlci_set_sdu_discard_timer(rlc_ul_table_ptr, um_ptr->chan_info[i].lc_id);
                        }
                        else if ((um_ptr->chan_info[i].timer_discard > 
                                  ctl_blk_ptr->ctrl.um_ctrl.timer_discard) &&
                                 (ctl_blk_ptr->tmr_discard_ptr != NULL))
                        {
                          /*-------------------------------------------------------
                          if  discard timer time was increased
                          -------------------------------------------------------*/ 
                          ctl_blk_ptr->ctrl.um_ctrl.timer_discard = um_ptr->chan_info[i].timer_discard;

                          /* set the  timer to (new timer value - elapsed time ) 	*/
                          (void)rex_set_timer(ctl_blk_ptr->tmr_discard_ptr,
                                (um_ptr->chan_info[i].timer_discard - rex_get_timer(ctl_blk_ptr->tmr_discard_ptr)));
                        }
                        else if (ctl_blk_ptr->tmr_discard_ptr != NULL)
                        {
                          /*----------------------------------------------------------------------------
                          if  discard timer time was decreased
                          -----------------------------------------------------------------------------*/ 
                          if (rex_get_timer(ctl_blk_ptr->tmr_discard_ptr) >= 
                              um_ptr->chan_info[i].timer_discard)
                          {
                            /*  if timer count > new timer value */
                            ctl_blk_ptr->ctrl.um_ctrl.timer_discard =um_ptr->chan_info[i].timer_discard;

                            /* calls callback function immediately */
                            (void)rex_set_timer(ctl_blk_ptr->tmr_discard_ptr, 0);
                          }
                          else
                          {
                            /*	if timer count < new timer value */
                            ctl_blk_ptr->ctrl.um_ctrl.timer_discard =um_ptr->chan_info[i].timer_discard;

                            /* set the  timer to (new timer value - elapsed time )	 */
                            (void)rex_set_timer(ctl_blk_ptr->tmr_discard_ptr,(um_ptr->chan_info[i].timer_discard - rex_get_timer(ctl_blk_ptr->tmr_discard_ptr)));
                          }
                        }
                      }
                    }
                    else
                    {
                      ctl_blk_ptr->ctrl.um_ctrl.timer_discard = um_ptr->chan_info[i].timer_discard;
                    }
#ifdef FEATURE_MAC_I
                    if(RLC_FLEXIBLE_PDU_SIZE == um_ptr->chan_info[i].ul_flex_pdu_size)
                    {
  	              ctl_blk_ptr->ctrl.um_ctrl.ul_flex_pdu = TRUE;
                      ctl_blk_ptr->ctrl.um_ctrl.li_length =
                        ((um_ptr->chan_info[i].ul_flex_max_pdu_size >> 3) <= 125) ? 1 : 2;
                    }
                    else
#endif
                    {
#ifdef FEATURE_MAC_I
                      ctl_blk_ptr->ctrl.um_ctrl.ul_flex_pdu = FALSE;
#endif

                      ctl_blk_ptr->ctrl.um_ctrl.li_length =
                          ((um_ptr->chan_info[i].rlc_size >> 3) <= 125) ? 1 : 2;
                    }
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.
                                         um_config_result[i].result = SUCCESS;
          #ifdef FEATURE_RLC_QXDM_LOGGING
                    if (j < UE_MAX_UL_UM_CHANNEL)
                    {
                      um_log_ptr->chan_info[j] = um_ptr->chan_info[i];
                      est_or_reest[j] = FALSE;
                      j++;
                    }
          #endif
                   //L2_ACQ_UL_LW_UNLOCK();
                   WL2_MSG_2(HIGH, rlc_ul_table_ptr, "UL UM Modify LC %d, Index %d",
                                                              um_ptr->chan_info[i].lc_id,
                                                              index);
                   
                  }
                  else
                  {
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d not config'd - No Mod",
                                                               um_ptr->chan_info[i].lc_id);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = FAILURE;
                  }
                  break;
          
                case RLCI_STOP:
		  ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, um_ptr->chan_info[i].lc_id);
                  if (NULL != ctl_blk_ptr)
                  {
                    /* Enter into Stop state. */
                    /*lint -save -e64 -e641 -e655 */
                    ctl_blk_ptr->lc_state |= UL_STOP;
                    /* Remove the Data transfer ready state.  */
                    ctl_blk_ptr->lc_state &= (~UL_DATA_TRANSFER_READY);
                    /*lint -restore */
                    
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = SUCCESS;
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d - Stop",
                                                               um_ptr->chan_info[i].lc_id);
                  }
                  else
                  {
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d not config'd - No Stop",
                                                               um_ptr->chan_info[i].lc_id);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = FAILURE;
                  }
                  break;
          
                case RLCI_CONTINUE:
		  ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, um_ptr->chan_info[i].lc_id);
                  if (NULL != ctl_blk_ptr)
                  {
                    /* Enter into the orignal states before enter Stop state. */
                    /*lint -save -e64 -e641 -e655 */
                    ctl_blk_ptr->lc_state &= (~UL_STOP);
                    ctl_blk_ptr->lc_state |= UL_DATA_TRANSFER_READY;
                    /*lint -restore */
                    
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = SUCCESS;
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d - Continue",
                                                               um_ptr->chan_info[i].lc_id);
                  }
                  else
                  {
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d not config'd - No Continue",
                                                               um_ptr->chan_info[i].lc_id);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = FAILURE;
                  }
                  break;
          
                case RLCI_RE_ESTABLISH:
		  ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, um_ptr->chan_info[i].lc_id);
                  if (NULL != ctl_blk_ptr)
                  {
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d - Re Establish",
                                                               um_ptr->chan_info[i].lc_id);
                    rlci_re_establish_ul_um(rlc_ul_table_ptr, ctl_blk_ptr, &um_ptr->chan_info[i]);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = SUCCESS;
          
          #ifdef FEATURE_RLC_QXDM_LOGGING
                    if (j < UE_MAX_UL_UM_CHANNEL)
                    {
                      um_log_ptr->chan_info[j] = um_ptr->chan_info[i];
                      est_or_reest[j] = TRUE;
                      j++;
                    }
          #endif
                  }
                  else
                  {
                    /*-----------------------------------------------------------------
                      The channel does not exist, send a FAILURE indication in the
                      confirm message to RRC.
                    -----------------------------------------------------------------*/
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d not config'd - No Re-establish",
                                                               um_ptr->chan_info[i].lc_id);
                    rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = FAILURE;
                  }
                  break;
          
          
                default:
                  WL2_MSG_1(HIGH, rlc_ul_table_ptr, "NO ACTION SPECIFIED FOR LC %d ",
                                                             um_ptr->chan_info[i].lc_id);
                  break;
              }
            }
          }
          break;
        default:
          WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL UM:Invalid Action %d",
                                                     um_ptr->ul_um_act[i].action);
          rrc_rlc_cmd_ptr->cmd.crlc_ul_um_config_cnf.um_config_result[i].result = FAILURE;
      }
    }
  }          
  L2_ACQ_DL_UNLOCK();

  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending = FALSE;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_dl_data_process_mutex));
  
#ifdef FEATURE_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    um_log_ptr->nchan = j;
    rlc_ul_send_um_log_packet(rlc_ul_table_ptr, um_log_ptr, est_or_reest);
  }
  if (k > 0) 
  {
    log_state_ptr->nchan = k;
    rlc_ul_send_state_log_packet(rlc_ul_table_ptr, log_state_ptr);
  }
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  rrc_put_rlc_cmd(rlc_ul_table_ptr->as_id, rrc_rlc_cmd_ptr);

}/* crlc_ul_um_config_handler() */

/*===========================================================================
FUNCTION RLC_CLEAR_FRC_UNRECO_FLAGS

DESCRIPTION
  This function clears the flags used to trigger RLC
  Unrecoverable error as there is scheduling on DL.
===========================================================================*/
void rlc_clear_frc_unreco_flags(sys_modem_as_id_e_type as_id)
{
  rlc_lc_id_type          rlc_id;
  rlci_uplink_table_type *rlc_ul_table_ptr;
  rex_crit_sect_type     *datapath_mutex    = NULL;  
  rlci_ul_ctrl_blk_type  *ctl_blk_ptr       = NULL;
  rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  for (rlc_id = RLC_UL_AM_START_LC_ID; rlc_id < UE_MAX_UL_LOGICAL_CHANNEL; rlc_id++)
  {
    if ((ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id)) == NULL)
    {
      /* LC doesnt exists */
      continue;
    }
    else 
    {
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, rlc_id);
      if ((UL_NULL_STATE != ctl_blk_ptr->lc_state) 		          		     	 && 
	  (UE_MODE_ACKNOWLEDGED == ctl_blk_ptr->lc_mode)  			  	         &&
	  (TRUE == ctl_blk_ptr->ctrl.am_ctrl.force_unreco_tmr_stats.poll_sent))
      {
        ctl_blk_ptr->ctrl.am_ctrl.force_unreco_tmr_stats.poll_sent      = FALSE;
        ctl_blk_ptr->ctrl.am_ctrl.force_unreco_tmr_stats.curr_tmr_count = 0;
      }
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    }
  }
}

/*===========================================================================
FUNCTION RLC_FORCE_UNRECO_TMR_CONFIG

DESCRIPTION
  This function handles the config of timer which runs to trigger
  RLC Unrecoverable error. 
===========================================================================*/
void rlc_force_unreco_tmr_config
(
 rlci_uplink_table_type     *rlc_ul_table_ptr, 
 rlci_ul_ctrl_blk_type      *ctl_blk_ptr
)
{    
  uint32                    timer_val, tmr_max_rst, tmr_poll_maxdat, tmr_poll_peri_maxdat;
  rlc_ul_info_type	   *rlc_ul_info_ptr;  
  rlc_frc_trig_unreco_tmr  *frc_trig_unreco_ptr;

  rlc_ul_info_ptr     = &(ctl_blk_ptr->ctrl.am_ctrl.ul_cnfg_var->rlc_info);  
  frc_trig_unreco_ptr = &(ctl_blk_ptr->ctrl.am_ctrl.force_unreco_tmr_stats);

  tmr_max_rst          = rlc_ul_info_ptr->maxrst * rlc_ul_info_ptr->timer_rst;
  tmr_poll_maxdat      = rlc_ul_info_ptr->discard_info.maxdat * rlc_ul_info_ptr->poll_info.timer_poll;
  tmr_poll_peri_maxdat = rlc_ul_info_ptr->discard_info.maxdat * rlc_ul_info_ptr->poll_info.timer_poll_periodic;
  timer_val            = tmr_max_rst + MAX(tmr_poll_maxdat, tmr_poll_peri_maxdat) + MIN_RLC_UNREC_TIMER_VALUE;
  /* Update force RLC unrecoverable structure and reset timer count */
  frc_trig_unreco_ptr->poll_sent       = FALSE;
  frc_trig_unreco_ptr->curr_tmr_count  = 0;
  frc_trig_unreco_ptr->max_tmr_val_cnf = timer_val;
}

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
/*===========================================================================

FUNCTION QSH_UPDATE_AVG_UL_WINDOW_SIZE

===========================================================================*/
void rlc_qsh_update_avg_ul_window_size(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  rlc_lc_id_type rlc_id;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rex_crit_sect_type *datapath_mutex = NULL;
  uint8  num_ul_am_dtch_chns;
  uint32 cumulative_win_size;
  
  num_ul_am_dtch_chns = cumulative_win_size = 0;
  
  for (rlc_id = RLC_UL_AM_START_LC_ID; rlc_id < UE_MAX_UL_LOGICAL_CHANNEL; rlc_id++)
  {
    if (rlc_ul_table_ptr->ul_ctl_blk_id_tbl[rlc_id] == INIT_CTL_BLK_ID)
    {
      /* LC doesnt exists */
      continue;
    }
    else 
    {
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, rlc_id);
      if (((ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id)) != NULL) && 
           ((UL_NULL_STATE != ctl_blk_ptr->lc_state) && 
            (UE_MODE_ACKNOWLEDGED == ctl_blk_ptr->lc_mode)&&
            (UE_LOGCHAN_DTCH == ctl_blk_ptr->lc_type)))
      {
        cumulative_win_size += ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_wins;
        num_ul_am_dtch_chns++;
      }
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    }
  }
  
  if (num_ul_am_dtch_chns)
  {
    rlc_qsh_metric_global[rlc_ul_table_ptr->as_id].rlc_fc_win_size = (uint16)( cumulative_win_size/num_ul_am_dtch_chns );
  }
}
#endif

/*===========================================================================

FUNCTION CRLC_UL_AM_CONFIG_HANDLER

DESCRIPTION
  This function handles the configuration of an Acknowledged Mode entity.
  it is called when RRC sends a CRLC_UL_AM_CONFIG_REQ to RLC. If a new 
  entity is to be established, the function checks if the channels exists. 
  If it already exists, an ERR message is dispalyed. If the entity does not 
  exist, an uplink table entry for this entity is updated and the state is
  set to UL_DATA_TRANSFER_READY. 
  If an entity is to be released,the function checks if the channel is in the 
  UL_DATA_TRANSFER_READY state. If it is, the channel is set to the NULL state
  and the watermark is set to NULL. If the parameters of a channel
  are to be modified ,the function checks if the channel is in the 
  UL_DATA_TRANSFER_READY state. If it is, the downlink table entry for the 
  channel is updated with the configuration received from RRC.


DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_am_config_handler 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_am_config_req_type *am_ptr
)
{
  uint8                          ctl_blk_id, no_channels_established, rtt_tbl_id;
  uint16                         i;
  uint32                         index = 0, act_ul_am_chnl = 0;
  boolean                        is_lc_release = FALSE;
  wcdma_l2_dl_cmd_type          *dl_cmd;
  rlc_am_config_type            *am_cfg_ptr;
  rlc_channel_action_cnf_e_type  action;
  rlci_ul_ctrl_blk_type         *ctl_blk_ptr = NULL;
  uint8                         *ul_ctl_blk_id_tbl = rlc_ul_table_ptr->ul_ctl_blk_id_tbl;
  rlc_mutexes_type              *rlc_mutex_ptr;
  rtt_delay_global_type         *rtt_delay_global_ptr;  
  rlc_lc_id_type                 rlc_id; 
  
#ifdef FEATURE_RLC_QXDM_LOGGING
  uint8                    j = 0, k = 0;
  rlc_am_config_req_type  *am_log_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_am_log_ptr);
  rlc_ul_log_state_type   *log_state_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_log_state);
  boolean est_or_reest[UE_MAX_AM_ENTITY];
#endif  
  
  rlc_mutex_ptr = RLC_GET_MUTEX_PTR(rlc_ul_table_ptr->as_id);
  rtt_delay_global_ptr = RLC_GET_RTT_GLOBAL_PTR(rlc_ul_table_ptr->as_id);
  
  dl_cmd = wcdma_l2_dl_get_cmd_buf();
  if (dl_cmd == NULL)
  {
    WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Could not get cmd buffer");
    return;
  }

  WL2_MSG_1(MED, rlc_ul_table_ptr, "Rlc release version is %d",
                                            l2_get_release_flag_indicator(rlc_ul_table_ptr->as_id));

  L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_dl_data_process_mutex));

  /*Locking both EUL and R99 Mutex*/
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending = TRUE;
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_am_config_pending = TRUE;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

   L2_ACQ_DL_LOCK();

   rlc_ul_table_ptr->rlc_am_win_size_chg = 0;

  dl_cmd->cmd_hdr.cmd_id = RLCI_DL_CONFIG_REQ;
  dl_cmd->cmd_data.dl_am_config_req.am_config_req.nentity = am_ptr->nentity;

  for (i=0; i<am_ptr->nentity; i++)
  {
    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_info[i] = 
                                                am_ptr->am_info[i];
    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].action = 
                                                am_ptr->am_act[i].action;
    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].dl_cfg_mask = am_ptr->am_act[i].dl_cfg_mask;
    am_cfg_ptr = am_ptr->am_info[i];
#if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
    if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_CORRUPTION)
    { 
      WL2_MSG_6(HIGH, rlc_ul_table_ptr,
          "Padding_DBG: UL lc_id %d, action %d, dl_cfg_mask %d, \
           ul_cfg_mask %d, ul_rlc_size %d, dl_rlc_size %d",
           am_cfg_ptr->common_config.ul_data_id, am_ptr->am_act[i].action, 
           am_ptr->am_act[i].dl_cfg_mask, am_ptr->am_act[i].ul_cfg_mask, 
           am_ptr->am_info[i]->rlc_ul_am_config.ul_rlc_size, 
           am_ptr->am_info[i]->rlc_dl_am_config.dl_rlc_size );
    }
#endif
    if (am_cfg_ptr->common_config.ul_data_id >= UE_MAX_UL_LOGICAL_CHANNEL) 
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Invalid UL AM Data LC Id %d",
                                                 am_cfg_ptr->common_config.ul_data_id);
      dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
    }
    else
    {
      switch(am_ptr->am_act[i].action)
      {
        case RLC_ESTABLISH: 
          ctl_blk_id = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, am_cfg_ptr->common_config.ul_data_id);
          if ((ctl_blk_id == INIT_CTL_BLK_ID) &&
              ((ctl_blk_id = ul_assign_ctl_blk(rlc_ul_table_ptr)) != INIT_CTL_BLK_ID))         
          {
            if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
            {
              rlc_dump_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
            }
            
            L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
            if ((rtt_tbl_id = ul_assign_rtt_blk(rlc_ul_table_ptr->as_id)) != 0xFF) 
            {
              rlc_id = am_cfg_ptr->common_config.ul_data_id;
              RLC_SET_RTT_TBL_INDEX(rtt_delay_global_ptr, rlc_id, rtt_tbl_id);
              rtt_delay_global_ptr->rlc_am_rtt_table[rtt_tbl_id].in_use = TRUE;     /* Set in_use = TRUE */
              
              rlc_ul_reset_rtt_for_lc(rlc_ul_table_ptr, rlc_id);  /* Reset the RTT variables for this LC */
              rtt_delay_global_ptr->num_active_rtt_channels++;
              
              if(am_cfg_ptr->common_config.ul_data_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
              {
                WL2_MSG_3(HIGH, rlc_ul_table_ptr, " RLC_RTT: Adding LC %d to RTT Table at Index %d. Num Active RTT Channels %d", 
                          rlc_id , rtt_tbl_id, rtt_delay_global_ptr->num_active_rtt_channels);
              }
            }
            L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_ul_rtt_mutex));
            
            ul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id] = ctl_blk_id;
            ul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_control_id] = ctl_blk_id;

            if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
            {
              rlc_sanitize_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
            }
            
            /*--------------------------------------------------------------------
              Check for the configured value of MAXDAT. Since the maximum number 
              of transmissions allowed for a particular PDU is MAXDAT -1, MAXDAT
              should always be greater than 1.
            --------------------------------------------------------------------*/
            if( (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.discard_mode != RLC_TBE) &&
                (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat < 2) )
            {
              WL2_MSG_1(ERROR, rlc_ul_table_ptr, "Incorrect value of MAXDAT %d",
                                                          am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat);
              dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
              break;
            }
            
            ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, ctl_blk_id);
            no_channels_established = rlci_establish_ul_am(rlc_ul_table_ptr, ctl_blk_ptr, am_cfg_ptr);
            
            if(no_channels_established == 0xff)
            {
               WL2_MSG_0(HIGH, rlc_ul_table_ptr, "no AM channel available");
               dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
               break;
            }

            else
            {
              rlc_ul_table_ptr->active_ch += no_channels_established;
            }

            /*-------------------------------------------------------------------------
              Register DSM flow control CB function for user data channel.
            -------------------------------------------------------------------------*/
            if(am_cfg_ptr->common_config.ul_data_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
            {
#ifdef FEATURE_QXDM_DSM_TRACING
              if (rlc_ul_table_ptr->rlc_dsm_trace_params.num_rlc_non_srb_entities_up++ == 0)
              {
                fc_post_reg_cmd_dsm_trace
                  (DSM_UMTS_TECH,
                   UMTS_RLC_CLIENT,
                   &rlc_fc_get_dsm_trace_cb,
                   (0x5EADBEEF | (rlc_ul_table_ptr->as_id << 31)); /* MSB is as_id */
                WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Registered Qxdm DSM tracing");
              }
#endif
              rlc_dl_multi_client_fc_init(rlc_ul_table_ptr->as_id, &(ctl_blk_ptr->ctrl.am_ctrl));

              /* Register with CFCM Module */
              rlc_cfcm_register(rlc_ul_table_ptr->as_id);

            }

            WL2_MSG_2(LOW, rlc_ul_table_ptr, "UL AM Establish: Data Id=%d,index=%d",
                                              am_cfg_ptr->common_config.ul_data_id,
                                              ctl_blk_id);
  
            dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
  
  #ifdef FEATURE_RLC_QXDM_LOGGING
            ctl_blk_ptr->ul_pdu_logging = FALSE;
            am_log_ptr->am_info[j] = am_cfg_ptr;
            est_or_reest[j] = TRUE;
            j++;
            log_state_ptr->ul_state[k].rlc_id = am_cfg_ptr->common_config.ul_data_id;
            log_state_ptr->ul_state[k].lc_state = AM_UL_CONFIG_PENDING;
            k++;
  #endif
          }
          else
          {
            dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
            WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Channel in using -- Data:%d, Control:%d",
                     am_cfg_ptr->common_config.ul_data_id, 
                                                       am_cfg_ptr->common_config.ul_control_id);
          }
          break;
        case RLC_RELEASE:
          L2_MUTEX_LOCK(&rlc_mutex_ptr->rlc_channel_release_mutex);
          ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, am_cfg_ptr->common_config.ul_data_id);
          if (NULL != ctl_blk_ptr)
          {
            rlc_ul_table_ptr->active_ch -= rlci_release_ul_am(rlc_ul_table_ptr, rlc_dl_table_ptr, ctl_blk_ptr);
	    is_lc_release  = TRUE;

            if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
            {
              rlc_dump_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
            }
            
            ul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_data_id] = INIT_CTL_BLK_ID;
            ul_ctl_blk_id_tbl[am_cfg_ptr->common_config.ul_control_id] = INIT_CTL_BLK_ID;

            if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
            {
              rlc_sanitize_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
            }
            
            for (index = 0, act_ul_am_chnl = 0; index < UE_MAX_UL_LOGICAL_CHANNEL; index++)
            {
              ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
              
              if ((NULL != ctl_blk_ptr) && 
                  ((UL_NULL_STATE != ctl_blk_ptr->lc_state) && 
                  (UE_MODE_ACKNOWLEDGED == ctl_blk_ptr->lc_mode)))
              {
                act_ul_am_chnl++;
                break;
              }
            }
            
            if ((0 == act_ul_am_chnl) && (TRUE == rlc_ul_table_ptr->rlc_ul_am_base_tmr_start))
            {
              /* Since all AM channels are released, stop base ul am timer */
              (void) rex_clr_timer(&rlc_ul_table_ptr->rlc_ul_am_base_tmr);
              rlc_ul_table_ptr->rlc_ul_am_base_tmr_start = FALSE;
              
              WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_TMR_DBG: Stopping UL base timer, active_ul_channels %d",
                                                         rlc_ul_table_ptr->active_ch);

            }
            dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
            WL2_MSG_2(HIGH, rlc_ul_table_ptr, "UL AM LC %d is released, active_ul_channels %d",
                                                       am_cfg_ptr->common_config.ul_data_id,
                                                       rlc_ul_table_ptr->active_ch);
  
  #ifdef FEATURE_RLC_QXDM_LOGGING
            log_state_ptr->ul_state[k].rlc_id = am_cfg_ptr->common_config.ul_data_id;
            log_state_ptr->ul_state[k].lc_state = RLC_UL_NULL_STATE;
            k++;
  #endif

#ifdef FEATURE_QXDM_DSM_TRACING
            if (am_cfg_ptr->common_config.ul_data_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              if (rlc_ul_table_ptr->rlc_dsm_trace_params.num_rlc_non_srb_entities_up > 0)
              {
                if (--rlc_ul_table_ptr->rlc_dsm_trace_params.num_rlc_non_srb_entities_up == 0)
                {
                  fc_post_deregister_cmd_dsm_trace
                    (DSM_UMTS_TECH,
                     UMTS_RLC_CLIENT);                     
                  WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Deregistered Qxdm DSM tracing");
                }
              }
              else
              {
                WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR::Removing nonsrb entitiy, when num_non_srb entity is zero",0,0,0);
              }
            }
#endif


            if(rlc_ul_table_ptr->rlc_acti_non_srb_am_ch &&
               am_cfg_ptr->common_config.ul_data_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              rlc_ul_table_ptr->rlc_acti_non_srb_am_ch--;
              
              if(!rlc_ul_table_ptr->rlc_acti_non_srb_am_ch)
              {
                rlc_cfcm_deregister(rlc_ul_table_ptr->as_id);
              }

            }
          }
          else
          {
            dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
            WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Channel %d not Config'd, No Release",
                                                       am_cfg_ptr->common_config.ul_data_id);
          }
          L2_MUTEX_UNLOCK(&rlc_mutex_ptr->rlc_channel_release_mutex);
          break;
  
        case RLC_MODIFY:
          dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;

          if(am_ptr->am_act[i].action == RLC_MODIFY)
          {
            dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].ul_cfg_mask = 0;
            while(am_ptr->am_act[i].ul_cfg_mask != 0)
            {
              action = rlci_get_config_action( RLCI_GET_DL_TBL_PTR(rlc_ul_table_ptr->as_id) ,
                                               (&(am_ptr->am_act[i].ul_cfg_mask)) );
              ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, am_cfg_ptr->common_config.ul_data_id);
              
              switch(action)
              {
                case RLCI_MODIFY :
                  if (ctl_blk_ptr != NULL)
                  {
                    /*--------------------------------------------------------------------
                      Check for the value of MAXDAT as it might change during a
                      modify procedure. Since the maximum number of transmissions
                      allowed for a particular PDU is MAXDAT -1, MAXDAT should always be
                      greater than 1.
                    --------------------------------------------------------------------*/
                    if( (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.discard_mode != RLC_TBE) &&
                        (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat < 2) )
                    {
                      WL2_MSG_1(ERROR, rlc_ul_table_ptr, "Incorrect value of MAXDAT %d",
                                                                  am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat);
                      dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                      break;
                    }
                    WL2_MSG_1(LOW, rlc_ul_table_ptr, "UL LC %d - Modify",
                                                               am_cfg_ptr->common_config.ul_data_id);
                    
                    ctl_blk_ptr->lc_state |= (UL_CONFIG_PENDING);
                    rlci_modify_ul_am(rlc_ul_table_ptr, ctl_blk_ptr, am_cfg_ptr);
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
                    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].ul_cfg_mask = RLC_MODIFY_CFG;

  #ifdef FEATURE_RLC_QXDM_LOGGING
                    if(j < UE_MAX_AM_ENTITY)
                    {
                      am_log_ptr->am_info[j] = am_cfg_ptr;
                      est_or_reest[j] = FALSE;
                      j++;
                    }
  #endif

  #ifdef FEATURE_WCDMA_UL_COMPR
                    rlc_ul_table_ptr->rlc_ulc_params.num_tti_zero_bo_reported = 0;
  #endif
                  }
                  else
                  {
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Channel %d not Config'd, No Modi",
                                                               am_cfg_ptr->common_config.ul_data_id);
                  }
                  break;
    
                case RLCI_STOP:
                  if (NULL != ctl_blk_ptr)
                  {
                    /*
                      This LC state is checked by ISR for every LC, lets make it
                      atomic
                    */
                    /* Enter into Stop state. */
                    /*lint -save -e64 -e641 -e655 */
                    ctl_blk_ptr->lc_state |= UL_STOP;
                    /* Remove the Data transfer ready state.  */
                    ctl_blk_ptr->lc_state &= (~UL_DATA_TRANSFER_READY);
                    //L2_ACQ_UL_LW_UNLOCK();
                    
                    /*lint -restore */
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d - Stop",
                                                               am_cfg_ptr->common_config.ul_data_id);
                  }
                  else
                  {
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d not config'd - No Stop",
                                                               am_cfg_ptr->common_config.ul_data_id);
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                  }
                  break;
    
                case RLCI_CONTINUE:
                  if (NULL != ctl_blk_ptr)
                  {
                    /*
                      Stop till ISR finishes.
                      This LC state is checked by ISR for every LC, lets make it
                      atomic.
                      
                    */
                    //L2_ACQ_UL_LW_LOCK();
                    /* Enter into the orignal states before enter Stop state. */
                    /*lint -save -e64 -e641 -e655 */ 
                    ctl_blk_ptr->lc_state &= (~UL_STOP);
                    ctl_blk_ptr->lc_state |= UL_DATA_TRANSFER_READY;
                    /*lint -restore */
                    //L2_ACQ_UL_LW_UNLOCK();
                    
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d - Continue",
                                                               am_cfg_ptr->common_config.ul_data_id);
                  }
                  else
                  {
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d not config'd - No Continue",
                                                               am_cfg_ptr->common_config.ul_data_id);
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                  }
                  break;
    
                case RLCI_RE_ESTABLISH:
                  if(NULL != ctl_blk_ptr)
                  {
                    /*--------------------------------------------------------------------
                      Check for the value of MAXDAT as it might change during a 
                      re-establish procedure. Since the maximum number of transmissions 
                      allowed for a particular PDU is MAXDAT -1, MAXDAT should always be 
                      greater than 1.
                    --------------------------------------------------------------------*/
                    if( (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.discard_mode != RLC_TBE) &&
                        (am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat < 2) )
                    {
                      WL2_MSG_1(ERROR, rlc_ul_table_ptr, "Incorrect value of MAXDAT %d",
                                                                  am_cfg_ptr->rlc_ul_am_config.rlc_info.discard_info.maxdat);
                      dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                      break;
        
                    }
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC %d - Re Establish",
                                                               am_cfg_ptr->common_config.ul_data_id);
                    rlci_re_establish_ul_am(rlc_ul_table_ptr, ctl_blk_ptr, am_cfg_ptr);
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = SUCCESS;
                    dl_cmd->cmd_data.dl_am_config_req.am_config_req.am_act[i].ul_cfg_mask = RLC_RE_ESTABLISH_CFG;

  #ifdef FEATURE_RLC_QXDM_LOGGING
                    if(j < UE_MAX_AM_ENTITY)
                    {
                      am_log_ptr->am_info[j] = am_cfg_ptr;
                      est_or_reest[j] = TRUE;
                      j++;
                    }
                    if(k < UE_MAX_UL_LOGICAL_CHANNEL)
                    {
                      log_state_ptr->ul_state[k].rlc_id = am_cfg_ptr->common_config.ul_data_id;
                      log_state_ptr->ul_state[k].lc_state = AM_UL_CONFIG_PENDING;
                      k++;
                    }
  #endif
                  }
                  else
                  {
                    dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
                    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Channel %d not Config'd, Unable to re-establish ",
                                                               am_cfg_ptr->common_config.ul_data_id);
        
                  }
        
                  break;
    
                default:
                  WL2_MSG_1(HIGH, rlc_ul_table_ptr, "NO ACTION SPECIFIED FOR LC %d ",
                                                             am_cfg_ptr->common_config.ul_data_id);
                  break;
              }
            }
          }
          break;
        default:
          dl_cmd->cmd_data.dl_am_config_req.result[i] = FAILURE;
          WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL AM:Invalid Action %d",
                                                     am_ptr->am_act[i].action);
      }
      if((NULL    != ctl_blk_ptr)                                 &&
         (SUCCESS == dl_cmd->cmd_data.dl_am_config_req.result[i]) &&
	 (FALSE   == is_lc_release))
      {
        rlc_force_unreco_tmr_config(rlc_ul_table_ptr, ctl_blk_ptr);
      }
    }
  }          

  /* post config request to Downlink */
  crlc_dl_am_config_handler(rlc_ul_table_ptr, rlc_dl_table_ptr, &(dl_cmd->cmd_data.dl_am_config_req));
  l2_free_cmd_buf(dl_cmd, rlc_dl_table_ptr);
  
  L2_ACQ_DL_UNLOCK();
  
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending = FALSE;
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_am_config_pending = FALSE;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_dl_data_process_mutex));

  WL2_MSG_0(HIGH, rlc_ul_table_ptr, "RLC config pending flags(rlc_config_pending, rlc_am_config_pending)set to FLASE ");
  
  #ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  rlc_qsh_update_avg_ul_window_size(rlc_ul_table_ptr);
  #endif  
#ifdef FEATURE_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    am_log_ptr->nentity = j;
    rlc_ul_send_am_log_packet(rlc_ul_table_ptr, am_log_ptr, est_or_reest);
  }
  if (k > 0) 
  {
    log_state_ptr->nchan = k;
    rlc_ul_send_state_log_packet(rlc_ul_table_ptr, log_state_ptr);
  }
#endif
//  wcdma_l2_dl_put_cmd(dl_cmd);

} /* crlc_ul_am_config_handler() */



/*===========================================================================

FUNCTION  RLCI_SET_SDU_DISCARD_TIMER 

DESCRIPTION
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_set_sdu_discard_timer
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_lc_id_type rlc_id
)
{
  dsm_item_type *sdu_ptr = NULL;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;

  /*For call back parameter, BIT 0 is as_id, BITs 1:8 are rlc_id*/
  uint32 cb_param = ((rlc_ul_table_ptr->as_id & 0x1) | ((rlc_id & 0xFF) << 1));
  
  if(NULL == (ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id)))
  {
    WL2_MSG_0(ERROR, rlc_ul_table_ptr,"RLC_ERR: Ctl_blk_ptr is Null");
    return;
  }
 
  switch (ctl_blk_ptr->lc_mode)
  {
    case UE_MODE_TRANSPARENT:
      if (ctl_blk_ptr->ctrl.tm_ctrl.timer_discard != 0)
      {
        if (NULL == ctl_blk_ptr->tmr_discard_ptr)
        {
          #ifdef FEATURE_REX_DYNA_MEM
          ctl_blk_ptr->tmr_discard_ptr =
            rex_create_timer_ex(rlci_tm_handle_discard_timer_cb, cb_param);
          #endif
          if (NULL == ctl_blk_ptr->tmr_discard_ptr)
          {
            WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Fail, create RLC UL TM %d Discard Timer",
                                                       rlc_id);
            return;
          }
        }

        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Set TM discard timer,LC %d", rlc_id);
        (void) rex_set_timer(ctl_blk_ptr->tmr_discard_ptr, (uint32)ctl_blk_ptr->ctrl.tm_ctrl.timer_discard);

        if ((sdu_ptr = (dsm_item_type *)q_check(ctl_blk_ptr->ul_wm_ptr->q_ptr)) != NULL)
        {
          while (sdu_ptr != NULL)
          {
            /* Put the discard timer stamp on the SDU */
            sdu_ptr->app_field |= 0x020000;
            sdu_ptr = (dsm_item_type *)q_next(ctl_blk_ptr->ul_wm_ptr->q_ptr, &(sdu_ptr->link));
          }
        }
      }
      break;

    case UE_MODE_UNACKNOWLEDGED:
      if (ctl_blk_ptr->ctrl.um_ctrl.timer_discard != 0)
      {
        if (NULL == ctl_blk_ptr->tmr_discard_ptr)
        {
          #ifdef FEATURE_REX_DYNA_MEM
          ctl_blk_ptr->tmr_discard_ptr =
            rex_create_timer_ex(rlci_um_handle_discard_timer_cb, 
                                (uint32)(rlc_id | (rlc_ul_table_ptr->as_id << 31))); /* MSB used to pass as_id */
          #endif
          if (NULL == ctl_blk_ptr->tmr_discard_ptr)
          {
            WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Fail, create RLC UL UM %d Discard Timer",
                                                       rlc_id);
            return;
          }
        }

        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Set UM discard timer, %d",
                                                   rlc_id);
        (void) rex_set_timer(ctl_blk_ptr->tmr_discard_ptr, (uint32)ctl_blk_ptr->ctrl.um_ctrl.timer_discard);
        if (NULL != ctl_blk_ptr->cur_sdu_ptr)
        {
          ctl_blk_ptr->cur_sdu_ptr->app_field |= 0x04;
        }
        if ((sdu_ptr = (dsm_item_type *)q_check(ctl_blk_ptr->ul_wm_ptr->q_ptr)) != NULL)
        {
          while (sdu_ptr != NULL)
          {
            /* Put the discard timer stamp on the SDU */
            sdu_ptr->app_field |= 0x04;
            sdu_ptr = (dsm_item_type *)q_next(ctl_blk_ptr->ul_wm_ptr->q_ptr, &(sdu_ptr->link));
          }
        }
      }
      break;

    case UE_MODE_ACKNOWLEDGED:
      break;

    default:
      WL2_MSG_0(MED, rlc_ul_table_ptr, "UE mode not valid");
  }
}/* rlci_set_sdu_discard_timer() */

uint32 rlci_get_num_retx_bytes(rlci_ul_am_state_type *ul_state_ptr)
{
  uint32 retx_size = 0;  
  if (RLCI_SEQ_GT(ul_state_ptr->vt_s, ul_state_ptr->vt_ms, RLC_AM_MODULUS))
  {
    retx_size = ul_state_ptr->retx_size_reduced;
  }
  else
  {
    retx_size = ul_state_ptr->retx_size;
  }              
  return retx_size;
}
/*===========================================================================

FUNCTION RLC_UL_REG_DATA_IND_IN_FACH

DESCRIPTION
  Register/Deregister Data Indication callback function with RRC
  This function is for CELL_FACH.
  
DEPENDENCIES - None.
  
RETURN VALUE - None.
  
SIDE EFFECTS - None.
===========================================================================*/
void rlc_ul_reg_data_ind_in_fach
(
  sys_modem_as_id_e_type as_id,
  rlc_ul_reg_data_ind_e_type data_ind_reg_type  
)
{
  uint16 count = 0, index;
  uint32 retx_size = 0;  	
  rlci_ul_ctrl_blk_type *ctrl_blk_ptr = NULL;

  l2_validate_as_id(as_id);
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rex_crit_sect_type *rlc_data_ind_reg_mutex_ptr = &(rlc_mutexes[as_id].rlc_data_ind_reg_mutex);

  if (rlc_ul_table_ptr->active_ch > 0)
  { 
    switch(data_ind_reg_type)
    {
      case RLC_REG_DATA_IND_IN_FACH:
      	 WL2_MSG_0(HIGH, rlc_ul_table_ptr, "DATA_IND_FACH: Rcvd Register request"); 	
      	 L2_MUTEX_LOCK(rlc_data_ind_reg_mutex_ptr);
      	 if(rlc_ul_table_ptr->data_ind_in_fach_func_ptr == NULL)
      	 {
      	   rlc_ul_table_ptr->data_ind_in_fach_func_ptr = rrccu_rlc_ul_data_ind;

          /* Check if Data is pending at RLC for transmission */
      	   count = rlc_ul_table_ptr->active_ch;
      	   index = 0;
      	   while((count > 0) && (index < UE_MAX_UL_RLC_ENTITY))
      	   {
            ctrl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
            if (ctrl_blk_ptr->lc_state != UL_NULL_STATE) 
            {
              count--;
              retx_size = rlci_get_num_retx_bytes(&(ctrl_blk_ptr->ctrl.am_ctrl.ul_state));
              if(RLC_UL_CHECK_PENDING_DATA_FOR_TX(rlc_ul_table_ptr, index, retx_size))
              {
                rlc_ul_table_ptr->rlc_ul_data_avail_for_tx = TRUE;
                WL2_MSG_3(HIGH, rlc_ul_table_ptr, "DATA_IND_FACH: Data available for Tx - index %d, retx_size %d, WM cnt %d, ",
                                                           index,
                                                           retx_size,
                                                           RLCI_TOTAL_BYTES_UL_WM_PLUS_DEQUEUED(ctrl_blk_ptr));
                break;
              }
            }
            index++;
      	   }
      	 }
      	 else
      	 {
          WL2_MSG_1(HIGH, rlc_ul_table_ptr, "DATA_IND_FACH: Already registered - %0x%x",
                                                     rlc_ul_table_ptr->data_ind_in_fach_func_ptr);
      	 }
      	 L2_MUTEX_UNLOCK(rlc_data_ind_reg_mutex_ptr);
      	 break;
      	 
      case RLC_UNREG_DATA_IND_IN_FACH:
      	L2_MUTEX_LOCK(rlc_data_ind_reg_mutex_ptr);
      	rlc_ul_table_ptr->data_ind_in_fach_func_ptr = NULL;
      	L2_MUTEX_UNLOCK(rlc_data_ind_reg_mutex_ptr);
      	WL2_MSG_0(HIGH, rlc_ul_table_ptr, "DATA_IND_FACH: Rcvd Unregister request "); 	 
        break;
        
      default:
      	 WL2_MSG_1(ERROR, rlc_ul_table_ptr, "DATA_IND_FACH: Invalid register type %d",
                                                    data_ind_reg_type); 
    }
  }
  else
  {
    WL2_MSG_1(ERROR, rlc_ul_table_ptr, "DATA_IND_FACH: No LC configured at RLC UL %d",
                                                rlc_ul_table_ptr->active_ch);
  }
}

/*===========================================================================

FUNCTION CLRLC_UL_REG_FOR_DATA_IND_HANDLER

DESCRIPTION
  Register Data Indication callback function to registered
  WM's non_empty_func_ptr.  
  This function is for Cell_PCH.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void clrlc_ul_reg_for_data_ind_handler
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_ul_register_for_data_ind_type *ul_reg_data_ind_ptr
)
{
  uint16 num_timer_suspended;
  int i;
  uint32 retx_size = 0;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rlci_ul_am_ctrl_type *am_ctrl_ptr;
  uint8 index;
  mac_ul_table_type *ul_table_ptr = GET_MAC_UL_TABLE_PTR(rlc_ul_table_ptr->as_id);

  num_timer_suspended = 0;

  for (i = 0; i < ul_reg_data_ind_ptr->num_lc; i++)
  {
    index = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, ul_reg_data_ind_ptr->lc_id[i]);
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
    if (NULL != ctl_blk_ptr)
    {
      /*-------------------------------------------------------------------------
        Block Status Timer flag is set. Suspend the 20ms RLC AM timer.
      -------------------------------------------------------------------------*/
      if (ul_reg_data_ind_ptr->block_status_timer &&
          (ctl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED))
      {
        am_ctrl_ptr = &(ctl_blk_ptr->ctrl.am_ctrl);
        am_ctrl_ptr->ul_tmr_sts = RLC_AM_TIMER_STOP;
        ++num_timer_suspended;
        rlci_suspend_dl_am_timer(rlc_dl_table_ptr, am_ctrl_ptr->am_common->dl_data_id);
        
#ifdef FEATURE_WCDMA_UL_COMPR
        if (am_ctrl_ptr->rlc_ptp_ul_comp_config.enabled == TRUE)
        {
          WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_PTP: Moving to PCH - UL COMP Config event Disabled for %d", 
                         am_ctrl_ptr->am_common->dl_data_id);
          rlc_ul_comp_disable_config_internal(rlc_ul_table_ptr, ctl_blk_ptr, FALSE);
        }
#endif
        
      }

      if (ctl_blk_ptr->ul_wm_ptr != NULL)
      {
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Register UL DATA IND, LC %d",
                                                   ul_reg_data_ind_ptr->lc_id[i]);
        
        dsm_set_wm_cb(ctl_blk_ptr->ul_wm_ptr,DSM_WM_NON_EMPTY,TRUE,rrccu_rlc_ul_data_ind,TRUE,
                                                    (void *)&rlc_ul_table_ptr->as_id,FALSE,0);
        retx_size = rlci_get_num_retx_bytes(&(ctl_blk_ptr->ctrl.am_ctrl.ul_state));

        if(ul_table_ptr->rach_info.mac_ul_rach_abrt_rcvd == TRUE)
        {
          rlci_rx_status_changed(rlc_ul_table_ptr, ul_reg_data_ind_ptr->lc_id[i],FALSE);
          WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Moving to PCH - Pending status due to abort is cleared for lc %d",
                                                     ul_reg_data_ind_ptr->lc_id[i]);
        }      
        
        /*------------------------------------------------------------------
          Some PDUs need to be retx ((retx_size > 0) or if there are some 
          PDUs with SN less than vt_s for which ACK has not yet been received
          and polling is configured and poll timer has hit) or there is data
          sitting in the watermark or a control PDU needs to be sent. Set a 
          20ms timer since RRC needs to finish the transition to Cell_PCH 
          state. After the timer expiry, it should call the data indication 
          callback function to leave the Cell_PCH state.
        ------------------------------------------------------------------*/
        if(RLC_UL_CHECK_PENDING_DATA_FOR_TX(rlc_ul_table_ptr, index, retx_size))
        {
          rrccu_rlc_ul_data_ind(NULL,&rlc_ul_table_ptr->as_id);
        }
      }
      else
      {
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "WM not reg'd, LC %d",
                                                   ul_reg_data_ind_ptr->lc_id[i]);
      }

    }
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d not config",
                                                 ul_reg_data_ind_ptr->lc_id[i]);
    } 
  }
  
  WL2_MSG_3(HIGH, rlc_ul_table_ptr, "RLC_TMR_DBG: Number of timer suspended %d, num_of_ch requested %d active_ul_channels %d",
  	    num_timer_suspended,ul_reg_data_ind_ptr->num_lc,rlc_ul_table_ptr->active_ch);
  ul_table_ptr->rach_info.mac_ul_rach_abrt_rcvd = FALSE;

  
}/* clrlc_ul_reg_for_data_ind_handler() */


/*===========================================================================

FUNCTION CLRLC_UL_UNREG_FOR_DATA_IND_HANDLER

DESCRIPTION
  De-Register Data Indication callback function from registered
  WM's non_empty_func_ptr.  
  This function is for Cell_PCH.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void clrlc_ul_unreg_for_data_ind_handler
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_ul_unregister_for_data_ind_type *ul_unreg_data_ind_ptr
)
{
  uint16 num_lc_tmr_res;
  int i;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
#ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
  boolean is_l1_in_fach;
  is_l1_in_fach = l1m_state_is_fach(rlc_ul_table_ptr->as_id);
#endif  
  num_lc_tmr_res = 0;

  for (i = 0; i < ul_unreg_data_ind_ptr->num_lc; i++)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, ul_unreg_data_ind_ptr->lc_id[i]);
    if (NULL != ctl_blk_ptr)
    {

      /*-------------------------------------------------------------------------
        Timer resume flag is set. Resume the timer which is suspended when 
        registering the data_ind callback.
      -------------------------------------------------------------------------*/
      if (ul_unreg_data_ind_ptr->resume_status_timer &&
          (ctl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED))
      {
        ctl_blk_ptr->ctrl.am_ctrl.ul_tmr_sts = RLC_AM_TIMER_RUNNING;
        num_lc_tmr_res++;
        rlci_resume_dl_am_timer(rlc_dl_table_ptr, ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id);
      }
      if ((ctl_blk_ptr->ul_wm_ptr != NULL) 
#ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
      	   && (FALSE == is_l1_in_fach)
#endif
         )
      {
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UNRegister UL DATA IND, LC %d",
                                                   ul_unreg_data_ind_ptr->lc_id[i]);
        dsm_set_wm_cb(ctl_blk_ptr->ul_wm_ptr,DSM_WM_NON_EMPTY,TRUE,NULL,TRUE,NULL,FALSE,0);
      }
      else
      {
        WL2_MSG_2(HIGH, rlc_ul_table_ptr, "WM API not unreg'd, LC %d WM_PTR 0x%x",
                                                   ul_unreg_data_ind_ptr->lc_id[i],
                                                   ctl_blk_ptr->ul_wm_ptr);
      }
    }
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d not config",
                                                 ul_unreg_data_ind_ptr->lc_id[i]);
    } 
  }
  WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC_TMR_DBG: Resumed for %d channels, num_channels requested %d",
                  num_lc_tmr_res,ul_unreg_data_ind_ptr->num_lc);
}/* clrlc_ul_unreg_for_data_ind_handler() */

/*===========================================================================

FUNCTION RLC_CHECK_PENDING_SDU

DESCRIPTION
  Checks if there is any data pending for a given LC, or if there are
  any pending ACKs awaited.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE - if data(new/Retx/Ctrl) is available or ACK is awaited or LC is in RESET PENDING state
  otherwise FALSE
  
SIDE EFFECTS
  None.

===========================================================================*/

boolean rlc_check_pending_sdu
(
  sys_modem_as_id_e_type as_id,
  rlc_lc_id_type lc_id,
  boolean *l2_int_data
)
{
  rlci_ul_ctrl_blk_type   *ctl_blk_ptr = NULL;
  rlci_ul_am_state_type   *ul_state_ptr = NULL;
  rlc_ul_lc_buf_status_type status_info;
  mac_ul_table_type *ul_table_ptr;
  boolean ret_value = FALSE;

  l2_validate_as_id(as_id);

  *l2_int_data = FALSE;

  ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);

  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);

  L2_MUTEX_LOCK(&rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_channel_release_mutex);
  if(((ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, lc_id)) == NULL) ||
     (ctl_blk_ptr->lc_mode != UE_MODE_ACKNOWLEDGED))
  {
    WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_ERR: UL LC ID %d not present or not in AM mode",
                                                lc_id);
  }
  else
  {
    if (ctl_blk_ptr->lc_state < UL_RESET_PENDING)
    {
      ul_state_ptr = &(ctl_blk_ptr->ctrl.am_ctrl.ul_state);
      status_info.new_data_bytes = status_info.retx_data_bytes = status_info.ctrl_data_bytes = 0;
      rlci_enh_get_am_buffer_status(rlc_ul_table_ptr, rlc_dl_table_ptr, ctl_blk_ptr, &status_info, NON_BFI_NON_TVM_QUERY, NULL, ul_table_ptr);
      
      if ((ul_state_ptr->vt_a != ul_state_ptr->vt_s) || 
          ((status_info.new_data_bytes + status_info.retx_data_bytes + status_info.ctrl_data_bytes) > 0))
      {
        WL2_MSG_3(HIGH, rlc_ul_table_ptr, "RLC_API: Data available: New %d, ReTx %d, ctrl %d",
                                                   status_info.new_data_bytes,
                                                   status_info.retx_data_bytes,
                                                   status_info.ctrl_data_bytes);
        WL2_MSG_3(HIGH, rlc_ul_table_ptr, "RLC_API: Pending ACK: vt_a %d, vt_s %d, rlc id %d",
                                                   ul_state_ptr->vt_a,
                                                   ul_state_ptr->vt_s,
                                                   lc_id);
        ret_value = TRUE;
        if (status_info.ctrl_data_bytes)
        {
          *l2_int_data = TRUE;
        }
      }
    }
    else if (ctl_blk_ptr->lc_state & UL_RESET_PENDING)
    {
      ret_value = TRUE;
      *l2_int_data = TRUE;
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_API: LC %d is in RESET PENDING state - return TRUE",
                                                 lc_id);
    }
  } 
  L2_MUTEX_UNLOCK(&rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_channel_release_mutex);
  return (ret_value);
}

/*===========================================================================

FUNCTION RLC_UL_REGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the registration of the watermark for the uplink
  channel.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_register_srvc_handler_sub
(
  sys_modem_as_id_e_type as_id,
  rlc_ul_register_srvc_type * reg_ptr
)
{
  int i;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  ds_umts_qos_tclass_enum_type traffic_class;
  rex_crit_sect_type *datapath_mutex = NULL;
  mac_ul_table_type *ul_table_ptr;
  rlc_wm_reg_rsp_status wm_reg_rsp;
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);
  ul_table_ptr = GET_MAC_UL_TABLE_PTR(as_id);

#ifdef FEATURE_WCDMA_TRIGGER_BASED_RACH
    boolean is_l1_in_fach;
    is_l1_in_fach = l1m_state_is_fach(as_id);
#endif

  L2_ACQ_DL_LOCK();

  WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_WM: Register UL WM called, nchan %d",
                                             reg_ptr->nchan);

  /*-------------------------------------------------------------------------
    Register Uplink Watermark for all Channels indicated in the registration
    pointer.
  -------------------------------------------------------------------------*/
  for (i = 0; i < reg_ptr->nchan; i++)
  {
    wm_reg_rsp = RLC_WM_REQ_INVALID_RSP;
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, reg_ptr->rlc_data[i].lc_id);
    if (NULL != ctl_blk_ptr)
    {
      /*
        no way to indicate this scenario to UL ISR, so wait till ISR completes
      */
      
      WL2_MSG_5(HIGH, rlc_ul_table_ptr,"RLC_WM:UL: RLC-id - %d, WM_ptr %d Lo_Wm_Cnt - %d, Hi_Wm_Cnt - %d, Do_Not_Exceed - %d",
                                        reg_ptr->rlc_data[i].lc_id,
                                        reg_ptr->rlc_data[i].ul_wm_ptr,
                                        reg_ptr->rlc_data[i].ul_wm_ptr->lo_watermark,
                                        reg_ptr->rlc_data[i].ul_wm_ptr->hi_watermark,
                                        reg_ptr->rlc_data[i].ul_wm_ptr->dont_exceed_cnt);
      RLC_ACQ_FC_LOCK(rlc_ul_table_ptr->as_id);
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, reg_ptr->rlc_data[i].lc_id);
      ctl_blk_ptr->ul_wm_ptr = reg_ptr->rlc_data[i].ul_wm_ptr;
      wm_reg_rsp = RLC_WM_REQ_SUCCESS;      

      if (reg_ptr->rlc_data[i].lc_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
      {
        traffic_class = pdcp_get_traffic_class(reg_ptr->rlc_data[i].lc_id);
        WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC DBG: received traffic-class %d for LC-id %d",
                                                   traffic_class,
                                                   reg_ptr->rlc_data[i].lc_id);
        if ((traffic_class != DS_UMTS_QOS_TCLASS_RESERVED) && 
             (traffic_class >= DS_UMTS_QOS_TCLASS_INTR))
        {
          ctl_blk_ptr->ctrl.am_ctrl.ul_state.dl_cpu_base_fc_f = TRUE;
          if(rlci_dl_fc_status(rlc_ul_table_ptr))
          {
            rlci_dl_fc_tx_new_win_sufi(rlc_dl_table_ptr, ctl_blk_ptr);
            WL2_MSG_1(HIGH, rlc_ul_table_ptr, "DL CPU flow control now, LC %d",
                                                       reg_ptr->rlc_data[i].lc_id);
          }

          ctl_blk_ptr->ctrl.am_ctrl.ul_state.ul_cpu_base_fc_f = TRUE;
          if(rlci_ul_fc_status(rlc_ul_table_ptr))
          {
            rlci_ul_fc_calc_new_tx_win_size(rlc_dl_table_ptr, ctl_blk_ptr);
            WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL CPU flow control now, LC %d",
                                                       reg_ptr->rlc_data[i].lc_id);
          }
#ifdef FEATURE_FLOW_CONTROL_WITH_QOS
         ctl_blk_ptr->ctrl.am_ctrl.ul_state.fc_qos_flag = TRUE; 
#endif
        }
        else
        {
          WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC DBG: Invalid traffic-class %d for LC-id %d",
                                                     traffic_class,
                                                     reg_ptr->rlc_data[i].lc_id);
        }
      }

#ifdef  FEATURE_WCDMA_TRIGGER_BASED_RACH
      if (TRUE == is_l1_in_fach)
      {
        rlc_ul_register_trigger_based_api_per_lc(reg_ptr->rlc_data[i].lc_id,ctl_blk_ptr, ul_table_ptr);
      }
#endif   
      /* start SDU discard timer if need  */
      rlci_set_sdu_discard_timer(rlc_ul_table_ptr, reg_ptr->rlc_data[i].lc_id);

      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      RLC_ACQ_FC_UNLOCK(rlc_ul_table_ptr->as_id);
    }
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d not config yet",
                                                 reg_ptr->rlc_data[i].lc_id);
      wm_reg_rsp = RLC_WM_REQ_FAIL_LC_NOT_REG;
    }

    if(reg_ptr->rlc_data[i].wm_reg_ind_cb != NULL)
    {
      WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC_WM: Sending UL WM reg status %d for LC %d",
                         wm_reg_rsp, reg_ptr->rlc_data[i].lc_id);	  
      reg_ptr->rlc_data[i].wm_reg_ind_cb(reg_ptr->rlc_data[i].lc_id, wm_reg_rsp, reg_ptr->rlc_data[i].bearer_context);
    } 
  }

  if ((rrc_get_state(rlc_ul_table_ptr->as_id) == RRC_STATE_CELL_PCH) ||
      (rrc_get_state(rlc_ul_table_ptr->as_id) == RRC_STATE_URA_PCH))
  {
    /*-------------------------------------------------------------------------
      Inform RRC about the LC's registering of WM.
    -------------------------------------------------------------------------*/
    rlc_rrc_req_to_register_for_data_ind(rlc_ul_table_ptr->as_id, reg_ptr);
  }

  L2_ACQ_DL_UNLOCK();

}/* rlc_ul_register_srvc_handler() */
void rlc_ul_register_srvc_handler 
(
  rlc_ul_register_srvc_type *reg_ptr   /* Ptr to WM registration info.     */  
)
{
  rlc_ul_register_srvc_handler_sub(L2_SUB_1,reg_ptr);
}


/*===========================================================================

FUNCTION RLC_UL_DEREGISTER_SRVC_HANDLER

DESCRIPTION
  This function handles the de-registration of the watermark for the uplink
  channel.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_deregister_srvc_handler 
(
  rlc_ul_deregister_srvc_type *de_reg_ptr /* Ptr to WM registration info.     */
)
{
  rlc_ul_deregister_srvc_handler_sub(L2_SUB_1,de_reg_ptr);
}
void rlc_ul_deregister_srvc_handler_sub 
(
  sys_modem_as_id_e_type as_id,
  rlc_ul_deregister_srvc_type *de_reg_ptr /* Ptr to WM registration info.	  */
)
{
  int i;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rex_crit_sect_type *datapath_mutex = NULL;
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlc_wm_reg_rsp_status wm_dereg_rsp;

  L2_ACQ_DL_LOCK();

  /*-------------------------------------------------------------------------
    Register Uplink Watermark for all Channels indicated in the registration
    pointer.
  -------------------------------------------------------------------------*/
  for (i = 0; i < de_reg_ptr->nchan; i++)
  {
    wm_dereg_rsp = RLC_WM_REQ_INVALID_RSP;
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, de_reg_ptr->rlc_id[i]);
    if (NULL != ctl_blk_ptr)
    {
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, de_reg_ptr->rlc_id[i]);
      ctl_blk_ptr->skip_pdu_build = TRUE;		  
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);

      #ifdef FEATURE_WCDMA_UL_COMPR
      L2_MUTEX_LOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_ul_comp_dacc_mutex));
      #endif
      ctl_blk_ptr->ul_wm_ptr = NULL;
      wm_dereg_rsp = RLC_WM_REQ_SUCCESS;
      ctl_blk_ptr->ctrl.am_ctrl.ul_state.dl_cpu_base_fc_f = FALSE;
      ctl_blk_ptr->ctrl.am_ctrl.ul_state.ul_cpu_base_fc_f = FALSE;
#ifdef FEATURE_FLOW_CONTROL_WITH_QOS
       ctl_blk_ptr->ctrl.am_ctrl.ul_state.fc_qos_flag = FALSE; 
#endif
    
      if(ctl_blk_ptr->tmr_discard_ptr != NULL)
      {
        (void) rex_clr_timer(ctl_blk_ptr->tmr_discard_ptr);
        #ifdef FEATURE_REX_DYNA_MEM
        rex_delete_timer_ex(ctl_blk_ptr->tmr_discard_ptr);  /* release timer block */
        #endif
        ctl_blk_ptr->tmr_discard_ptr = NULL;  /* Reset timer sig */
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Release SDU discard timer,LC %d",
                                                    de_reg_ptr->rlc_id[i]);
      }
      
      #ifdef FEATURE_WCDMA_UL_COMPR
      L2_MUTEX_UNLOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_ul_comp_dacc_mutex));
      #endif      
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, de_reg_ptr->rlc_id[i]);
      ctl_blk_ptr->skip_pdu_build = FALSE;
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "DeRegister UL WM, LC %d",
                                                 de_reg_ptr->rlc_id[i]);
    }
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d not config yet",
                                                 de_reg_ptr->rlc_id[i]);
      wm_dereg_rsp = RLC_WM_REQ_FAIL_LC_NOT_REG;
    }
      
    if(de_reg_ptr->cb_info[i].wm_reg_ind_cb != NULL)
    {
      WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC_WM: Sending UL WM dereg status %d for LC %d",
                         wm_dereg_rsp, de_reg_ptr->rlc_id[i]);   
      de_reg_ptr->cb_info[i].wm_reg_ind_cb(de_reg_ptr->rlc_id[i], wm_dereg_rsp, de_reg_ptr->cb_info[i].bearer_context);
    } 
  }
  L2_ACQ_DL_UNLOCK();
} /* rlc_ul_deregister_srvc_handler() */


/*===========================================================================

FUNCTION RLC_UL_REGISTER_AM_SDU_CNF_CB_REQ

DESCRIPTION
  Register the SDU confirm callback function as request by RRC.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_register_am_sdu_cnf_cb_req
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_register_am_sdu_confirm_callback_type *sdu_cnf_cb_req
)
{
  int i;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rex_crit_sect_type *datapath_mutex = NULL;

  for (i = 0; i < sdu_cnf_cb_req->nchan; i++)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id);
    if (NULL != ctl_blk_ptr)
    {
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id);
      ctl_blk_ptr->ctrl.am_ctrl.sdu_cnf_cb = 
                            sdu_cnf_cb_req->sdu_cnf_cb[i].rlc_am_sdu_cnf_cb;
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);

      if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_AM)
      {
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Register UL LC %d SDU Cnf CB",
                                           sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id);
      }
    }
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d not config yet",
                                                 sdu_cnf_cb_req->sdu_cnf_cb[i].lc_id);
    } 
  }
}/* rlc_ul_register_am_sdu_cnf_cb_req() */

#ifdef FEATURE_WCDMA_IGNORE_DATA_RB_DURING_CALL_REL
/*===========================================================================

FUNCTION rlc_ul_check_for_rb_data_allowed

DESCRIPTION
 API to check if PDU building is allowed for a particular RB or not.

DEPENDENCIES
  None.

RETURN VALUE
  Returns TRUE if PDU building is allowed or else returns FALSE.

SIDE EFFECTS
  None.

===========================================================================*/
inline boolean rlc_ul_check_for_rb_data_allowed
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  boolean                 rrc_release_rab_active_flag,
  uecomdef_logchan_e_type lc_type,
  rlc_lc_id_type          rlc_id
)
{
  boolean ret_val;

  ret_val = FALSE;

  /* This is an out of spec implementation where we report BO for user RB's as zero 
     * if RRC is in middle of releasing the RB's  
     */
  if ((FALSE == rrc_release_rab_active_flag) || \
      ((TRUE == rrc_release_rab_active_flag) && \
       ((UE_LOGCHAN_DCCH == lc_type) ||(UE_LOGCHAN_CCCH == lc_type)))
     )
  {
    ret_val = TRUE;
  }
  else
  {
    WL2_MSG_3(ERROR, rlc_ul_table_ptr, "RLC_ERR:NA Return BO as 0 for RLC_ID %d RAB_ACT %d lc_type %d",
                                                rlc_id,
                                                rrc_release_rab_active_flag,
                                                lc_type);
  }
  
  return ret_val;
}
#endif

/*===========================================================================

FUNCTION RLCI_PROCESS_TA_IND

DESCRIPTION
  Processes the QTA/LTA Start/Stop received from RRC

===========================================================================*/
void rlci_process_ta_ind
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_ta_info_type *ta_info_ptr
)
{
  rlc_ta_ind_e_type ta_ind = ta_info_ptr->tune_away_ind;
  rlc_ta_success_e_type ta_succ_ind = ta_info_ptr->tune_away_succ;
  
  if (ta_succ_ind == RLC_FAIL_IND)
  {
    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_TA: TA %d (1:LTA, 3:QTA)stop failed", ta_ind);
    return;
  }

  switch (ta_ind)
  {
    case RLC_LTA_START_IND:
      rlc_proc_lta_start_ind(rlc_ul_table_ptr, rlc_dl_table_ptr, ta_succ_ind);
      break;

    case RLC_LTA_STOP_IND:
      rlc_proc_lta_stop_ind(rlc_ul_table_ptr, rlc_dl_table_ptr, ta_succ_ind);
      break;

    case RLC_QTA_START_IND:
      rlc_proc_qta_start_ind(rlc_ul_table_ptr, rlc_dl_table_ptr, ta_succ_ind);
      break;

    case RLC_QTA_STOP_IND:
      rlc_proc_qta_stop_ind(rlc_ul_table_ptr, rlc_dl_table_ptr, ta_succ_ind);
      break;

    default:
      WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_TA: Invalid value %d for TA Ind", ta_ind);
      break;
  }
}

/*===========================================================================

FUNCTION RLC_PROC_LTA_START_IND

DESCRIPTION
  Processes LTA Start Indication received from RRC

===========================================================================*/
void rlc_proc_lta_start_ind
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_ta_success_e_type ta_succ_ind
)
{
  if (ta_succ_ind == RLC_TA_SUCC_MAX_ENUM)
  {
    rlc_ul_table_ptr->is_tuneaway = TRUE;
  }
}

/*===========================================================================

FUNCTION RLC_PROC_LTA_STOP_IND

DESCRIPTION
  Processes LTA Stop Indication received from RRC

===========================================================================*/
void rlc_proc_lta_stop_ind
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_ta_success_e_type ta_succ_ind
)
{
  rlc_ul_table_ptr->is_tuneaway = FALSE;
  if (ta_succ_ind == RLC_SUCC_IND)
  {
    WL2_MSG_0(HIGH, rlc_ul_table_ptr, "RLC_TA: LTA Stop Success Indication received from RRC");
    rlc_trigger_status_on_am_chnls(rlc_ul_table_ptr, rlc_dl_table_ptr);
  }
}

/*===========================================================================

FUNCTION RLC_PROC_QTA_START_IND

DESCRIPTION
  Processes QTA Start Indication received from RRC

===========================================================================*/
void rlc_proc_qta_start_ind
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_ta_success_e_type ta_succ_ind
)
{
  if (ta_succ_ind == RLC_TA_SUCC_MAX_ENUM)
  {
    rlc_ul_table_ptr->is_tuneaway = TRUE;
  }
}

/*===========================================================================

FUNCTION RLC_PROC_QTA_STOP_IND

DESCRIPTION
  Processes QTA Stop Indication received from RRC

===========================================================================*/
void rlc_proc_qta_stop_ind
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_ta_success_e_type ta_succ_ind
)
{
  rlc_ul_table_ptr->is_tuneaway = FALSE;
}

#ifdef FEATURE_MAC_I
/*===========================================================================

FUNCTION RLC_UL_GET_FLEX_RETX_PDU_INFO_PTR

DESCRIPTION
  API to return pointer to rlc_flex_retx_pdu_info

DEPENDENCIES
  None

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
rlc_flex_retx_pdu_info_type *rlc_ul_get_flex_retx_pdu_info_ptr(sys_modem_as_id_e_type as_id,rlc_lc_id_type    rlc_id)
{
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlc_flex_retx_pdu_info_type *rlc_flex_retx_pdu_info_ptr = NULL;

  if ((rlc_id >= RLC_UL_AM_USR_PLANE_START_LC_ID) && (rlc_id < UE_MAX_UL_LOGICAL_CHANNEL))
  {
    rlc_flex_retx_pdu_info_ptr = rlc_ul_table_ptr->rlc_flex_retx_pdu_info[rlc_id - RLC_UL_AM_USR_PLANE_START_LC_ID];
  }
  return rlc_flex_retx_pdu_info_ptr;
}

/*===========================================================================

FUNCTION RLC_IS_MAC_I_FLEX_PARTIAL_SUPPORT_ENABLED

DESCRIPTION
  Returns TRUE if MAC_I_FLEX_PARTIAL_SUPPORT is enabled

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlc_is_mac_i_flex_partial_support_enabled(sys_modem_as_id_e_type as_id)
{
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);

  return rlc_ul_table_ptr->rlc_qxdm_globals.mac_i_flex_partial_support;
}

/*===========================================================================

FUNCTION RLC_IS_MAC_I_SP_HE_F3_ENABLED

DESCRIPTION
  Returns TRUE if MAC_I_SP_HE_F3 is enabled

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlc_is_mac_i_sp_he_f3_enabled(sys_modem_as_id_e_type as_id)
{
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);

  return rlc_ul_table_ptr->rlc_qxdm_globals.maci_rlc_sp_he_f3_enable;
}

/*===========================================================================

FUNCTION RLC_SET_MAC_I_ENH_BUILD_PDU_DUMP

DESCRIPTION
  Sets MAC_I_ENH_BUILD_PDU_DUMP flag

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_set_mac_i_enh_build_pdu_dump(sys_modem_as_id_e_type as_id, boolean val)
{
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);

  rlc_ul_table_ptr->rlc_qxdm_globals.maci_rlc_enh_build_pdu_dump = val;
}
#endif

/*===========================================================================

FUNCTION RLC_UL_ENH_BUFFER_STATUS

DESCRIPTION
  Gets the buffer status of requested uplink logical channels by MAC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_enh_buffer_status
(
  rlc_ul_lc_info_type    *lc_info_ptr,  /* Report buffer status for */
                                        /* only following channels  */
  rlc_ul_buf_status_type *status_ptr,    /* Pointer to UL Buf.Status */
  rlc_ul_bo_query_source  query_from_bfi_flag,
  boolean           *rlc_wait_for_ack,
  mac_ul_table_type *ul_table_ptr
)
{
  uint8               report_chan_idx;  /* Number of active channels.*/
  uint8               ctl_blk_id;       /* Control Blk ID            */
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rlc_lc_id_type      rlc_id = 0;
  sys_modem_as_id_e_type as_id;
  rlci_uplink_table_type *rlc_ul_table_ptr;
  rlci_downlink_table_type *rlc_dl_table_ptr;
  
#ifdef FEATURE_WCDMA_IGNORE_DATA_RB_DURING_CALL_REL
  boolean             rrc_release_rab_active_flag;
#endif
  
  as_id = ul_table_ptr->as_id;
  rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);
  
#ifdef FEATURE_WCDMA_IGNORE_DATA_RB_DURING_CALL_REL
  rrc_release_rab_active_flag = FALSE;
  
  /* If RLC config pending (or) RRC connection release is active
   * Report 0 buf status to avoid any further calls to 
   * RLC internal structures which are getting (re)configured now 
   */
  if ((TRUE == rrcrcr_is_proc_active(as_id)) && (PS_RABS == rrc_query_mac_rab_status(as_id)))
  {
    rrc_release_rab_active_flag = TRUE;
  } 
#endif
  /* If RLC config pending, report 0 buf status to avoid any further calls to 
   * RLC internal structures which are getting (re)configured now 
   */
  if ((rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending == TRUE) ||
      (rlc_ul_table_ptr->rlc_config_pending_var.rlc_am_config_pending == TRUE))
  {
    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Either config_pending %d or rlc_am_config_pending %d pending - Return BO as Zero",
                                               rlc_ul_table_ptr->rlc_config_pending_var.rlc_config_pending,
                                               rlc_ul_table_ptr->rlc_config_pending_var.rlc_am_config_pending);
    
    (void)memset(status_ptr->chan_info, 0, sizeof(status_ptr->chan_info));
  }
  else
  {
    /*-------------------------------------------------------------------------
  	for each UM and TM active logical channel in the uplink table,
  	get the buffer status. If the WM is being purged then return a
  	zero buffer occupancy on the channel. For AM, block only new 
  	data and allow re-transmissions and status.
    -------------------------------------------------------------------------*/
    for(report_chan_idx = 0; report_chan_idx < lc_info_ptr->nchan; report_chan_idx++)
    {
      if(report_chan_idx<UE_MAX_UL_LOGICAL_CHANNEL)
      {
        rlc_id = lc_info_ptr->rlc_id[report_chan_idx];
      }
      else
      {
        WL2_MSG_2(ERROR, rlc_ul_table_ptr, "Number of active channels %d are out of range. Maximum UL logical channels: %d",
                                                    report_chan_idx,
                                                    (int)UE_MAX_UL_LOGICAL_CHANNEL);
      }
      
      /*-------------------------------------------------------------------------
        Reset status memory for this LC
      -------------------------------------------------------------------------*/
      (void)memset(&(status_ptr->chan_info[rlc_id]), 0, sizeof(rlc_ul_lc_buf_status_type));
      
      if (rlc_id >= UE_MAX_UL_LOGICAL_CHANNEL)
      {
        WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_ERR: RLC LC ID out of Range, %d",
                                                    rlc_id);
        continue;
      }
      ctl_blk_id = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, rlc_id);
      ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, ctl_blk_id);
      if ((NULL != ctl_blk_ptr) &&
          (ctl_blk_ptr->lc_state != UL_NULL_STATE))
      {
#ifdef FEATURE_WCDMA_IGNORE_DATA_RB_DURING_CALL_REL
        if (TRUE == rlc_ul_check_for_rb_data_allowed(rlc_ul_table_ptr, rrc_release_rab_active_flag,
                                           ctl_blk_ptr->lc_type,rlc_id))
#endif
        {
          if ((ctl_blk_ptr->ul_wm_ptr == NULL) && 
              (UE_MODE_ACKNOWLEDGED != ctl_blk_ptr->lc_mode))
          {
            WL2_MSG_1(LOW, rlc_ul_table_ptr, "Buf Status:UL LC %d WM unregistered",
                                                      rlc_id);
        
            if (ctl_blk_ptr->lc_mode == UE_MODE_TRANSPARENT)
            {
              status_ptr->chan_info[rlc_id].npdus = 0;
              status_ptr->chan_info[rlc_id].type = FIXED_PDUS;
            }
            else
            {
              status_ptr->chan_info[rlc_id].nbytes = 0;
              status_ptr->chan_info[rlc_id].type = FLEX_PDUS;
            }        
          }
          else // WM is registered
          {
            switch(ctl_blk_ptr->lc_mode)
            {
              case UE_MODE_TRANSPARENT:
                if((ctl_blk_ptr->lc_state == UL_DATA_TRANSFER_READY) &&
                   (ctl_blk_ptr->skip_pdu_build == FALSE))
                {
                  rlci_get_tm_buffer_status(rlc_ul_table_ptr, ctl_blk_ptr,
                     &(status_ptr->chan_info[rlc_id]), query_from_bfi_flag);
                }
                else
                {
                  if(ctl_blk_ptr->skip_pdu_build == TRUE)
                  {
                    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Skip PDU building for LC %d, ctl_blk_id %d",
                                                               rlc_id,
                                                               ctl_blk_id);
                  }
                  else
                  {
                    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Buf Status:UL LC %d in %x state",
                                                               rlc_id,
                                                               ctl_blk_ptr->lc_state);
                  }
                  status_ptr->chan_info[rlc_id].npdus = 0;
                }
                status_ptr->chan_info[rlc_id].type = FIXED_PDUS;
                break;
                
            case UE_MODE_UNACKNOWLEDGED:
                rlci_get_um_buffer_status(rlc_ul_table_ptr, ctl_blk_ptr, &(status_ptr->chan_info[rlc_id]));
                
                /* We are in the data_transfer_ready state or   */
                /* Waiting_to _suspend state.                   */
                if((ctl_blk_ptr->lc_state &
                    (UL_DATA_TRANSFER_READY | UL_WAITING_TO_SUSPEND)) &&
                    (ctl_blk_ptr->skip_pdu_build == FALSE))
                {
                  status_ptr->chan_info[rlc_id].suspend_pdu_build_f = FALSE;
                }
                else
                {
                  if(ctl_blk_ptr->skip_pdu_build == TRUE)
                  {
                    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Skip PDU build for LC %d, ctl_blk_id %d",
                                                               rlc_id,
                                                               ctl_blk_id);
                  }
                  else
                  {
                    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Buf Status:UL LC %d in %x state",
                                                               rlc_id,
                                                               ctl_blk_ptr->lc_state);
                  }
                  status_ptr->chan_info[rlc_id].suspend_pdu_build_f = TRUE;
                }
                status_ptr->chan_info[rlc_id].type = FLEX_PDUS;
          
                if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & 0x00000020)
                {
                  WL2_MSG_3(HIGH, rlc_ul_table_ptr, "RLC DBG: UM Buf Status:UL LC %d new_data_bytes %d WM_cnt %d",
                                                             rlc_id,
                             status_ptr->chan_info[rlc_id].new_data_bytes, 
                             ctl_blk_ptr->ul_wm_ptr->current_cnt);
                }
                break;
          
              case UE_MODE_ACKNOWLEDGED:
                /*---------------------------------------------------------------
                  The entity is not in the states of Config_pending state,
                  Suspended state or Stop state where there is no data
                  allowed to be sent.
                ----------------------------------------------------------------*/
                /*lint -save -e655 */
                if (!(ctl_blk_ptr->lc_state &
                                      (UL_CONFIG_PENDING | UL_STOP)) &&
                    (!(ctl_blk_ptr->skip_pdu_build)))
                /*lint -restore */
                {
                  status_ptr->chan_info[rlc_id].suspend_pdu_build_f = FALSE;
                }
                else
                {
                  WL2_MSG_2(MED, rlc_ul_table_ptr, "Buf Status:UL LC %d in %x state",
                                                            rlc_id,
                                                            ctl_blk_ptr->lc_state);
                  status_ptr->chan_info[rlc_id].suspend_pdu_build_f = TRUE;
                }
          
                rlci_enh_get_am_buffer_status(rlc_ul_table_ptr, rlc_dl_table_ptr, ctl_blk_ptr, &(status_ptr->chan_info[rlc_id]),
                                        query_from_bfi_flag,rlc_wait_for_ack, ul_table_ptr);
          
                if(rlc_ul_table_ptr->rlc_qxdm_globals.buf_status_log)
                {
                WL2_MSG_6(HIGH, rlc_ul_table_ptr,
                  "Buf Status:UL LC %d, ctl %d win_size %d new data %d, retx %d, suspend_flag %d",
                  rlc_id,status_ptr->chan_info[rlc_id].ctrl_data_bytes,
                  status_ptr->chan_info[rlc_id].effect_win_size, 
                  status_ptr->chan_info[rlc_id].new_data_bytes,
                  status_ptr->chan_info[rlc_id].retx_data_bytes,
                  status_ptr->chan_info[rlc_id].suspend_pdu_build_f );
                }
                status_ptr->chan_info[rlc_id].type = FLEX_PDUS;
                break;
          
              default:
                WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Buf Status:Invalid Mode");
            }/* switch */
          }
        }
      }
    } /* for */
  }

}/* rlc_ul_enh_buffer_status */


/*===========================================================================

FUNCTION RLC_UL_BUILD_PDUS

DESCRIPTION
  Builds uplink PDUs for transmission in a TTI. 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_build_pdus 
(
  sys_modem_as_id_e_type    as_id,
  rlc_ul_frame_format_type *frame_ptr,
  rlc_ul_frame_data_type   *data_ptr,
  uint8                     cfn
)
{
  int i;
  uint16    num_pdus = 0;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  l2_rlc_profile_data_struct_type *profile_var_ptr;
  rlci_uplink_table_type   *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);

#ifdef FEATURE_RLC_QXDM_LOGGING
  uint8 log_index = 0;
  boolean found = FALSE, log_allowed;
  rlc_ul_logging_var_type *rlc_ul_logging_ptr = &(rlc_ul_table_ptr->rlc_ul_logging);
  uint16 k=0;
  rlc_ul_init_am_log_params(rlc_ul_logging_ptr,FALSE);  
#endif

  data_ptr->nchan = 0;

  /*------------------------------------------------------------------------
    Free UM SDUs transmitted in the previous TTI and initialize the queue.
  -------------------------------------------------------------------------*/
  rlc_um_free_sdu_q(rlc_ul_table_ptr, RLC_NON_E_DCH_CHNL);

  rlc_check_pre_alloc_dsm(rlc_ul_table_ptr);
  
  rlc_ul_table_ptr->rlc_am_ctrl_pdu_built = 0;
  
  for (i = 0; i < frame_ptr->nchan; i++)
  {
    /*-----------------------------------------------------------------------
      If the Logical Channel ID exceeds the maximum allowed channel ID,
      move to the next channel.
    -----------------------------------------------------------------------*/            
    if (frame_ptr->chan_fmt[i].rlc_id >= UE_MAX_UL_LOGICAL_CHANNEL) 
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Bld UL PDUS:Invalid LC ID %d",
                                                 frame_ptr->chan_fmt[i].rlc_id);
      continue;
    }
    /*-----------------------------------------------------------------------
      Get the index into the table.
    -----------------------------------------------------------------------*/
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, frame_ptr->chan_fmt[i].rlc_id);
    if ((NULL != ctl_blk_ptr) && 
        (ctl_blk_ptr->lc_state != UL_NULL_STATE))
    {
      if((ctl_blk_ptr->ul_wm_ptr == NULL) &&
         (ctl_blk_ptr->lc_mode != UE_MODE_ACKNOWLEDGED))
      {
        WL2_MSG_1(LOW, rlc_ul_table_ptr, "Bld UL PDUs:LC %d WM unregistered",
                                                  frame_ptr->chan_fmt[i].rlc_id);
        data_ptr->chan_info[data_ptr->nchan].npdus = 0;
        data_ptr->chan_info[data_ptr->nchan].pdu_size = 0;
      }
      else
      {
#ifdef FEATURE_RLC_QXDM_LOGGING
        log_allowed = FALSE;
#endif
        switch (ctl_blk_ptr->lc_mode)
        {
          case UE_MODE_TRANSPARENT:
            /*---------------------------------------------------------------
              Build the PDU and update the structure.
            ---------------------------------------------------------------*/
            if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_TM)
            {
              WL2_MSG_3(HIGH, rlc_ul_table_ptr, "UL TM LC %d, Num PDUs %d, PDU size %d",
                                                         frame_ptr->chan_fmt[i].rlc_id,
                                                         frame_ptr->chan_fmt[i].npdus,
                     frame_ptr->chan_fmt[i].pdu_size[0]);
            }
	    profile_var_ptr = &(rlc_ul_table_ptr->rlc_profile_var.rlc_tm_profile_dump);
            rlc_profile_dump_start(rlc_ul_table_ptr->as_id,profile_var_ptr);
            profile_var_ptr->profile_array[profile_var_ptr->dump_idx].add_on3 = 
                    frame_ptr->chan_fmt[i].npdus;
            rlci_ul_build_tm_pdus(rlc_ul_table_ptr, ctl_blk_ptr, 
                                  &frame_ptr->chan_fmt[i], 
                                  &data_ptr->chan_info[data_ptr->nchan]);
            rlc_profile_dump_end(rlc_ul_table_ptr->as_id, profile_var_ptr);

            break;
          case UE_MODE_UNACKNOWLEDGED:
#ifdef FEATURE_RLC_QXDM_LOGGING
	    log_allowed = log_status(WCMDA_UL_FIXED_UM_PDU_LOG_PACKET_V1);
#endif
	    profile_var_ptr = &(rlc_ul_table_ptr->rlc_profile_var.rlc_um_profile_dump);
            rlc_profile_dump_start(rlc_ul_table_ptr->as_id,profile_var_ptr);
            profile_var_ptr->profile_array[profile_var_ptr->dump_idx].add_on3 = 
	                                     frame_ptr->chan_fmt[i].npdus;

            num_pdus = 
              rlc_ul_enh_build_non_edch_um_pdus(rlc_ul_table_ptr, ctl_blk_ptr,
                                                &frame_ptr->chan_fmt[i],
                                                &data_ptr->chan_info[data_ptr->nchan]                      
                                                #ifdef FEATURE_RLC_QXDM_LOGGING
                                                , log_allowed
                                                #endif 
						);

            data_ptr->chan_info[data_ptr->nchan].npdus = num_pdus;
            rlc_profile_dump_end(rlc_ul_table_ptr->as_id, profile_var_ptr);

#ifdef FEATURE_RLC_QXDM_LOGGING
#ifdef FEATURE_UM_PDU_LOGGING
            if(!(ctl_blk_ptr->ul_pdu_logging) && log_allowed)
            {
              found = TRUE;
              rlc_ul_table_ptr->rlc_ul_logging.rlc_ul_channel_cnt ++;

              ctl_blk_ptr->ctrl.um_ctrl.ul_chan_um_pdu.pdu_size[0]=
                                        frame_ptr->chan_fmt[i].pdu_size[0];
	      ctl_blk_ptr->ctrl.um_ctrl.ul_chan_um_pdu.r99_frame_num = cfn;

              for(k=0;
                  k < data_ptr->chan_info[data_ptr->nchan].npdus &&
                  k < MAX_UL_RLC_LOG_PDUS; k++)
              {
                ctl_blk_ptr->ctrl.um_ctrl.ul_chan_um_pdu.log_data[k] =
                  rlc_copy_to_word(data_ptr->chan_info[data_ptr->nchan].pdu_info[k].pdu_ptr->data_ptr
                                  + data_ptr->chan_info[data_ptr->nchan].pdu_info[k].pdu_offset);
              }
              ctl_blk_ptr->ctrl.um_ctrl.ul_chan_um_pdu.num_pdu = k;
            }
#endif //FEATURE_UM_PDU_LOGGING
#endif //FEATURE_RLC_QXDM_LOGGING

            /*---------------------------------------------------------------
              If traffic is sent on user plane channels, set the activity
              flag to DATA_TRANSFERRED.
            ---------------------------------------------------------------*/
            RLC_SET_DATA_ACTIVITY_FLAG(rlc_ul_table_ptr, frame_ptr->chan_fmt[i].rlc_id, 
                            RLC_UL_UM_USR_PLANE_START_LC_ID);


            break;

          case UE_MODE_ACKNOWLEDGED:
            /*---------------------------------------------------------------
             If data PDUs are being built or going to be built 
             on UL AM user plane channels, set the activity flag to 
             DATA_TRANSFERRED.
            ---------------------------------------------------------------*/
            if(ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s !=
                    ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_a)
            {
              RLC_SET_DATA_ACTIVITY_FLAG(rlc_ul_table_ptr, frame_ptr->chan_fmt[i].rlc_id, 
                    RLC_UL_AM_USR_PLANE_START_LC_ID);
            }

	    /*---------------------------------------------------------------
	     If LC ID is for user plane AM channels, store the info if
             user plane PDU logging is turned on, otherwise store the info
	     if the signaling plane PDU logging is turned on. Check the status here to
	     pass it onto pdu building function
	     ---------------------------------------------------------------*/
#ifdef FEATURE_RLC_QXDM_LOGGING
	     if((frame_ptr->chan_fmt[i].rlc_id >= RLC_UL_AM_USR_PLANE_START_LC_ID) && 
					  log_status(WCDMA_UL_FIXED_AM_USR_PDU_LOG_PACKET_V1))
	     {
	       log_allowed = TRUE;
	       log_index = 1;
	       rlc_ul_logging_ptr->logging_params[log_index][RLCLOG_CH_IDX] ++;
	     }
	     if((frame_ptr->chan_fmt[i].rlc_id < RLC_UL_AM_USR_PLANE_START_LC_ID) && 
					  log_status(WCDMA_UL_FIXED_AM_SIG_PDU_LOG_PACKET_V1))
	     {
	       log_allowed = TRUE;
	       log_index = 0;
	       rlc_ul_logging_ptr->logging_params[log_index][RLCLOG_CH_IDX] ++;
	     }
#endif

	    profile_var_ptr = &(rlc_ul_table_ptr->rlc_profile_var.rlc_am_profile_dump);
            rlc_profile_dump_start(rlc_ul_table_ptr->as_id,profile_var_ptr);
            profile_var_ptr->profile_array[profile_var_ptr->dump_idx].add_on1 |= 
                   (frame_ptr->chan_fmt[i].npdus << 16);
            num_pdus = 
              rlc_ul_enh_build_non_edch_am_pdus(rlc_ul_table_ptr, rlc_dl_table_ptr, ctl_blk_ptr,
                                                &frame_ptr->chan_fmt[i],
                                                &data_ptr->chan_info[data_ptr->nchan]
                                                #ifdef FEATURE_RLC_QXDM_LOGGING
						, log_allowed
						#endif
						);
            data_ptr->chan_info[data_ptr->nchan].npdus = num_pdus;
            rlc_profile_dump_end(rlc_ul_table_ptr->as_id, profile_var_ptr);


#ifdef FEATURE_RLC_QXDM_LOGGING
            if(!(ctl_blk_ptr->ul_pdu_logging) &&
                (log_status(rlc_ul_logging_ptr->logging_params[log_index][RLCLOG_TYPE_IDX])) && 
                (rlc_ul_logging_ptr->logging_params[log_index][RLCLOG_CH_IDX] > 0))
            {
              found = TRUE;
              /*-------------------------------------------------------------
                           Store the number of pdus, pdu size, 4bytes of the pdu, data id, 
                           control id in ul_am_pdu_log for purpose of qcat logging.
                           -------------------------------------------------------------*/
              ctl_blk_ptr->ctrl.am_ctrl.ul_chan_am_pdu.pdu_size[0] = 
                                           frame_ptr->chan_fmt[i].pdu_size[0];
	      ctl_blk_ptr->ctrl.am_ctrl.ul_chan_am_pdu.r99_frame_num = cfn;

              /*-------------------------------------------------------------
                Perform PDU logging after the PDU building. No need to check 
                the UE PDU cap again as it should have done in the PDU building
                -------------------------------------------------------------*/
              for(k=0; 
                  k < data_ptr->chan_info[data_ptr->nchan].npdus &&  
                  k < MAX_UL_RLC_LOG_PDUS;
                  k++)
              {
                ctl_blk_ptr->ctrl.am_ctrl.ul_chan_am_pdu.log_data[k] = 
                    rlc_copy_to_word(data_ptr->chan_info[data_ptr->nchan].pdu_info[k].pdu_ptr->data_ptr
                                    + data_ptr->chan_info[data_ptr->nchan].pdu_info[k].pdu_offset);
              }
              ctl_blk_ptr->ctrl.am_ctrl.ul_chan_am_pdu.num_pdu = k;
            }
            ctl_blk_ptr->ctrl.am_ctrl.ul_chan_am_stat.tot_num_pdu_byte_txd +=
                                         ((frame_ptr->chan_fmt[i].pdu_size[0] >> 3)* 
                                           data_ptr->chan_info[data_ptr->nchan].npdus);
#endif //FEATURE_RLC_QXDM_LOGGING

            if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_PDU_BLDG)
            {
              WL2_MSG_3(HIGH, rlc_ul_table_ptr, "RLC variables, VT_S:%x,VT_MS:%x,VT_A:%x",
              ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s,
              ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_ms,
              ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_a);
            }
            break;
          default:
            WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Build UL PDUs:Invalid Mode "); 
        }
      }
      data_ptr->chan_info[data_ptr->nchan].rlc_id = frame_ptr->chan_fmt[i].rlc_id;
      data_ptr->nchan++;
    }
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Build UL PDUs:LC %d does not exist",
                                                 frame_ptr->chan_fmt[i].rlc_id);
    }
  } /* for */

#ifdef FEATURE_RLC_QXDM_LOGGING

  if (found || rlc_ul_table_ptr->rlc_ul_logging.rlci_ul_pdu_cipher_log.num_pdu)
  {
    (void) rex_set_sigs(wcdma_l2_ul_get_tcb(as_id), RLC_UL_START_LOGGING_SIG);
  }
#endif //FEATURE_RLC_QXDM_LOGGING


}/* rlc_ul_build_pdus() */

/*===========================================================================

FUNCTION RLCI_GET_CTL_BLK_PTR_AM_RB_ID

DESCRIPTION
  Returns Ctrl Pointer, if the rb_id is active and LC Type of AM
  Otherwise return NULL
  
DEPENDENCIES
  None.
  
RETURN VALUE
  AM - Control Blk pointer
  Non-AM - NULL
  
SIDE EFFECTS
  None.

===========================================================================*/
rlci_ul_ctrl_blk_type *rlci_get_ctl_blk_ptr_am_rb_id(rlci_uplink_table_type *rlc_ul_table_ptr, uint8 rb_id)
{
  int32 ctl_blk_id;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;

  for (ctl_blk_id = 0; ctl_blk_id < UE_MAX_UL_RLC_ENTITY; ctl_blk_id++)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, ctl_blk_id);
    if ((NULL != ctl_blk_ptr) &&
    	(ctl_blk_ptr->lc_state != UL_NULL_STATE) && 
        (ctl_blk_ptr->lc_mode>= UE_MODE_ACKNOWLEDGED) && 
        (ctl_blk_ptr->lc_mode <= UE_MODE_ACKNOWLEDGED_CONTROL) &&
        (ctl_blk_ptr->ctrl.am_ctrl.am_common->rb_id == rb_id))
    {
      return ctl_blk_ptr;
    }
  }  
  return NULL;

}/* rlci_get_ctl_blk_ptr() */

/*===========================================================================

FUNCTION RLCi_CHK_UL_RESET_PENDING

DESCRIPTION
  This function checks if the UL is in RESET pending state.

DEPENDENCIES
  None

RETURN VALUE
  Boolean indicating, RESET pending or not.

SIDE EFFECTS
  None
===========================================================================*/

boolean rlci_chk_ul_reset_pending
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_lc_id_type lc_id
)
{
  rlci_ul_ctrl_blk_type *ul_ctrl_blk_ptr;
  boolean               ret_val = FALSE;

  rex_crit_sect_type *datapath_mutex = NULL;

  datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, lc_id);

  ul_ctrl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, lc_id);
  
  /*lint -save -e655 */
  if ((ul_ctrl_blk_ptr != NULL) && 
      (ul_ctrl_blk_ptr->lc_state & (rlci_ul_state_e_type)UL_RESET_PENDING))
  {
    ret_val = TRUE;
  }
  /*lint -restore */

  L2_ACQ_UL_LW_UNLOCK(datapath_mutex);

  return ret_val;
} /* rlci_chk_ul_reset_pending */

/*===========================================================================

FUNCTION RLCI_SET_UL_NUM_SN_LI

DESCRIPTION
  Updates ul_num_sn_li, which indicates the number of SN-Li
  pairs which will formed on UL. This API is used to copy number
  SN-LI pairs calculated in DL to UL. 
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_set_ul_num_sn_li 
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  uint8   lc_id,
  uint16  num_sn_li
)
{
  rlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;

  ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, lc_id);

  if(NULL == ctl_blk_ptr)
  {
    return;
  }
  ctl_blk_ptr->ctrl.am_ctrl.ul_num_sn_li = num_sn_li;

  return;

}/* rlci_get_ctl_blk_ptr() */

/*===========================================================================

FUNCTION RLCI_DL_CONFIG_CNF_HANDLER

DESCRIPTION
  Handle the config confirmation from the Downlink side. This is for AM only.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_dl_config_cnf_handler
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlci_dl_config_cnf_type *cnf_ptr
)
{
  int i;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rrc_cmd_type *rrc_rlc_cmd_ptr;
#ifdef FEATURE_RLC_QXDM_LOGGING
  uint8 j = 0;
  rlc_ul_log_state_type *log_state_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_log_state);
#endif  //FEATURE_RLC_QXDM_LOGGING

  rrc_rlc_cmd_ptr = rrc_get_rlc_cmd_buf();
  if (rrc_rlc_cmd_ptr == NULL)
  {
    WL2_MSG_0(ERROR, rlc_ul_table_ptr, "Could not get cmd buffer");
    return;
  }
  rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = RRC_CRLC_AM_CONFIG_CNF;
  rrc_rlc_cmd_ptr->cmd.crlc_am_config_cnf.nchan = cnf_ptr->nchan;


  for (i=0; i<cnf_ptr->nchan; i++)
  {
    if(i<UE_MAX_UL_AM_DATA_CHANNEL)
    {
    #if ((defined (FEATURE_HSDPA_MAC_EHS)) && (defined (FEATURE_WCDMA_REL7)))
      if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_CORRUPTION)
      { 
        WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Padding_DBG: rb_id %d, UL lc_id %d, action %d",
                                                   cnf_ptr->am_cnf[i].rb_id,
                                                   cnf_ptr->am_cnf[i].ul_data_id,
             cnf_ptr->am_cnf[i].action);
      }
    #endif

      if (cnf_ptr->am_cnf[i].result == SUCCESS)
      {
        if (
            ((cnf_ptr->am_cnf[i].action == RLCI_ESTABLISH) ||
            (cnf_ptr->am_cnf[i].action == RLCI_RE_ESTABLISH) ||
            (cnf_ptr->am_cnf[i].action == RLCI_MODIFY))
            )
        {
          if((ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, cnf_ptr->am_cnf[i].ul_data_id)) == NULL)
          {
            WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC-UL LC Id:%d not found",
                                                        cnf_ptr->am_cnf[i].ul_data_id);
            continue;
          }
  
          if (
              cnf_ptr->am_cnf[i].action == RLCI_ESTABLISH
              )
          {
            //L2_ACQ_UL_LW_LOCK();
            /*--------------------------------------------------------------------- 
              Go to UL_DATA_TRANSFER_READY.
            ----------------------------------------------------------------------*/
            ctl_blk_ptr->lc_state = UL_DATA_TRANSFER_READY;
            //L2_ACQ_UL_LW_UNLOCK();
            
         #ifdef FEATURE_RLC_QXDM_LOGGING
            log_state_ptr->ul_state[j].rlc_id = cnf_ptr->am_cnf[i].ul_data_id;
            log_state_ptr->ul_state[j].lc_state = AM_UL_DATA_TRANSFER_READY;
            j++;
         #endif  //FEATURE_RLC_QXDM_LOGGING
  
            /*--------------------------------------------------------------------- 
              Delay setting up timer until the RLC sends out its first PDU.
            ----------------------------------------------------------------------*/
            ctl_blk_ptr->ctrl.am_ctrl.ul_state.fst_tx_pdu = TRUE;
            ctl_blk_ptr->ctrl.am_ctrl.ul_tmr_sts = RLC_AM_TIMER_UNDEF_STATUS;
            
            if(cnf_ptr->am_cnf[i].ul_data_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
            {
              rlc_ul_table_ptr->rlc_acti_non_srb_am_ch++;
            }
          }
          /* action is RLC_RE_ESTABLISH */
          else if (
                      cnf_ptr->am_cnf[i].action == RLCI_RE_ESTABLISH
                  )
          {
          /* 
            Wait till UL ISR completes
          */
            //L2_ACQ_UL_LW_LOCK();
            /*--------------------------------------------------------------------- 
              Remove UL_CONFIG_PENDING state and go back to the original states.
            ----------------------------------------------------------------------*/
            /*lint -save -e64 -e641 */
            ctl_blk_ptr->lc_state &= (~UL_CONFIG_PENDING);
            /*lint -restore */
            //L2_ACQ_UL_LW_UNLOCK();
            
            /*-----------------------------------------------------------------
              If re-establishment occured during
              1. UE initiated reset, start a new reset procedure
              2. N/W initiated reset, abort the reset procedure
            -----------------------------------------------------------------*/
            /*lint -save -e655 */ 
            if (ctl_blk_ptr->lc_state & UL_RESET_PENDING)
            /*lint -restore */             
            {
              if (ctl_blk_ptr->ctrl.am_ctrl.ul_state.need_reset_ack || 
                  ctl_blk_ptr->ctrl.am_ctrl.ul_state.owe_reset)
              {
                /*-------------------------------------------------------------
                  Turn on this flag for the next TTI to tx RESET PDU.
                -------------------------------------------------------------*/
                ctl_blk_ptr->ctrl.am_ctrl.ul_state.owe_reset = TRUE;
  
                /*-------------------------------------------------------------
                  Turn on RESET Timer.
                -------------------------------------------------------------*/
                ctl_blk_ptr->ctrl.am_ctrl.ul_state.need_reset_ack = TRUE;
              }
              else
              {
                /*
                  Wait till UL ISR completes
                */
                //L2_ACQ_UL_LW_LOCK();
                /*-------------------------------------------------------------
                  This is a case of one side re-establishment(DL), where a 
                  NW initiated RESET procedure is terminated due to DL 
                  re-establishment. Since UE UL has to come out of RESET_PENDING
                  state without sending a RESET_ACK update count_c here..
                -------------------------------------------------------------*/
                ctl_blk_ptr->cipher.hfn   = (ctl_blk_ptr->cipher.count >> 12);
                ctl_blk_ptr->cipher.count = (ctl_blk_ptr->cipher.hfn << 12);
                WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Aborting NW initiated RESET count_c %d, vt_s %d, DL_state(1:rst_pndg) %d",
                                                           ctl_blk_ptr->cipher.count,
                                                           ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s,
                   rlci_chk_dl_reset_pending(rlc_dl_table_ptr, ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id));
                /*lint -save -e64 -e641 */
                ctl_blk_ptr->lc_state &= (~UL_RESET_PENDING);
                /*lint -restore */
                //L2_ACQ_UL_LW_UNLOCK();
  
                rlci_dl_change_state(rlc_dl_table_ptr, ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id);
              }
            }
          }
          else if (cnf_ptr->am_cnf[i].action == RLCI_MODIFY)
          {
            ctl_blk_ptr->lc_state &= (~UL_CONFIG_PENDING);
          }
          /* If RLC FC is active, flow control new rb. */
          if(cnf_ptr->am_cnf[i].ul_data_id >= RLC_UL_AM_USR_PLANE_START_LC_ID)
          {
  
           multi_client_fc_modify_max_flow(&(ctl_blk_ptr->ctrl.am_ctrl.multiclient_dl_fc_arbitarator),
                                            rlci_get_rx_win_size(rlc_dl_table_ptr, ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id),
                                            FALSE);
           ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins = rlci_dl_multi_client_fc_get_win_size(rlc_ul_table_ptr, &(ctl_blk_ptr->ctrl.am_ctrl));
  
            ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws =
              ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins;
  
            if(rlci_dl_fc_status(rlc_ul_table_ptr)
               && ctl_blk_ptr->ctrl.am_ctrl.ul_state.dl_cpu_base_fc_f
               && (!(rlc_ul_table_ptr->rlc_am_win_size_chg & RLC_DL_AM_WIN_SIZE_CHG_FLAG)))
            {
              rlci_dl_fc_tx_new_win_sufi(rlc_dl_table_ptr, ctl_blk_ptr);
            }
  
            ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws =
              ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins;
  
            if(rlci_ul_fc_status(rlc_ul_table_ptr)
               && ctl_blk_ptr->ctrl.am_ctrl.ul_state.ul_cpu_base_fc_f
               && (!(rlc_ul_table_ptr->rlc_am_win_size_chg & RLC_UL_AM_WIN_SIZE_CHG_FLAG)))
            {
              rlci_ul_fc_calc_new_tx_win_size(rlc_dl_table_ptr, ctl_blk_ptr);
            }
          }
        }
        WL2_MSG_2(LOW, rlc_ul_table_ptr, "UL AM LC %d config complete, cnf action %d",
                                          cnf_ptr->am_cnf[i].ul_data_id,
                                          cnf_ptr->am_cnf[i].action);
      }
    }
    else
    {
      WL2_MSG_2(ERROR, rlc_ul_table_ptr, "Index %d to get DL config confirm info exceeds the maximum number of Uplink Data Plane Acknowleged Mode Channels %d",
                                                  i,
                                                  (int)UE_MAX_UL_AM_DATA_CHANNEL);
    }
    if(i<UE_MAX_AM_ENTITY)
    {
      rrc_rlc_cmd_ptr->cmd.crlc_am_config_cnf.am_config_result[i].rb_id =
                                                    cnf_ptr->am_cnf[i].rb_id;
      rrc_rlc_cmd_ptr->cmd.crlc_am_config_cnf.am_config_result[i].result = 
                                                    cnf_ptr->am_cnf[i].result;
    }
    else
    {
      WL2_MSG_2(ERROR, rlc_ul_table_ptr, "Index %d to get config confirm info for an entity exceeds max AM entities suppoted %d",
                                                  i, (int)UE_MAX_AM_ENTITY);
    }
  }

  if (rlc_ul_table_ptr->rlc_am_win_size_chg)
  {
    rlc_fc_post_internal_cfcm_off_cmd(rlc_ul_table_ptr);
  }
  rlc_dl_table_ptr->rlc_dl_logging.rlci_dl_pdu_cipher_log_on = log_status(WCDMA_DL_RLC_V1_PDU_CIPHER_LOG_PACKET);
#ifdef FEATURE_RLC_QXDM_LOGGING
  rlc_ul_table_ptr->rlc_ul_logging.rlci_ul_pdu_cipher_log_on = log_status(WCDMA_UL_RLC_V1_PDU_CIPHER_LOG_PACKET);
#endif //FEATURE_RLC_QXDM_LOGGING

#ifdef FEATURE_RLC_QXDM_LOGGING
  if ( j > 0) 
  {
    log_state_ptr->nchan = j;
    rlc_ul_send_state_log_packet(rlc_ul_table_ptr, log_state_ptr);
  }
#endif //FEATURE_RLC_QXDM_LOGGING
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  rrc_put_rlc_cmd(rlc_ul_table_ptr->as_id, rrc_rlc_cmd_ptr);

}/* rlci_dl_config_cnf_handler() */



/*===========================================================================

FUNCTION CRLC_UL_AM_RESET_HANDLER

DESCRIPTION
  Reset all AM entities that are configured.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_am_reset_handler(rlci_uplink_table_type *rlc_ul_table_ptr, rlci_downlink_table_type *rlc_dl_table_ptr)
{
  int i;
#ifdef FEATURE_RLC_QXDM_LOGGING
  uint8 j = 0;
  rlc_ul_log_state_type *log_state_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_log_state);
#endif //FEATURE_RLC_QXDM_LOGGING
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;

  WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Reset all AM Entities");

  for (i = 0; i < UE_MAX_UL_RLC_ENTITY; i++)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, i);
    if ((NULL != ctl_blk_ptr) &&
    	ctl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
    {
      rlci_reset_ul_am(rlc_ul_table_ptr, rlc_dl_table_ptr, ctl_blk_ptr, TRUE);

#ifdef FEATURE_RLC_QXDM_LOGGING
      log_state_ptr->ul_state[j].rlc_id =
         ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id;
      log_state_ptr->ul_state[j].lc_state = AM_UL_RESET_PENDING;
      j++;
#endif  //FEATURE_RLC_QXDM_LOGGING

    }
  }

#ifdef FEATURE_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    log_state_ptr->nchan = j;
    rlc_ul_send_state_log_packet(rlc_ul_table_ptr, log_state_ptr);
  }
#endif //FEATURE_RLC_QXDM_LOGGING

}/* crlc_ul_am_reset_handler() */


/*===========================================================================

FUNCTION RLCI_SET_POST_RRC_RESET_SIG

DESCRIPTION
  Post RLC_UL_RESET_REPORT_SIG to L2 task. This is due to the MAC UL
  moves to interrupt contents. 
  In interrupt context, the dynamic allocation of memory for RRC 
  command will screw up the heap memory. The REX SIG could have the
  RRC message being posted in the L2 context.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_set_post_rrc_reset_sig
(
  rlci_uplink_table_type *rlc_ul_table_ptr, 
  rlci_ul_ctrl_blk_type  *ctl_blk_ptr,
  rlc_error_e_type        err_type,
  rlc_lc_id_type          rlc_id
)
{
  int i;

  for (i=0; i<UE_MAX_AM_ENTITY; i++)
  {
    /* Found an available entry */
    if (rlc_ul_table_ptr->ul_reset_table.am_reset_id_table[i].rlc_id == INIT_CTL_BLK_ID)
    {
      /* Store RESET information */
      rlc_ul_table_ptr->ul_reset_table.am_reset_id_table[i].rlc_id = rlc_id;
      rlc_ul_table_ptr->ul_reset_table.am_reset_id_table[i].err_type = err_type;
      break;
    }
  }

  ctl_blk_ptr->ctrl.am_ctrl.force_unreco_tmr_stats.curr_tmr_count = 0;
  ctl_blk_ptr->ctrl.am_ctrl.force_unreco_tmr_stats.poll_sent      = FALSE;

  /* Post REX SIG to L2 */
  (void) rex_set_sigs(wcdma_l2_ul_get_tcb(rlc_ul_table_ptr->as_id), RLC_UL_RESET_REPORT_SIG );
}/* rlci_set_post_rrc_reset_sig() */


/*===========================================================================

FUNCTION RLCI_POST_RESET_TO_RRC

DESCRIPTION
  Post the RESET result message to RRC
  The RESET information is stored in the am_reset_id_table. 
  
  This function is called in the L2 context.
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_post_reset_to_rrc(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  am_reset_id_type *am_reset_id_table;
  rrc_cmd_type *rrc_rlc_cmd_ptr;
  int i;
  sys_modem_as_id_e_type as_id;

  am_reset_id_table = rlc_ul_table_ptr->ul_reset_table.am_reset_id_table;
  as_id = rlc_ul_table_ptr->as_id;

  /* Get RRC command buffer */
  for (i=0; i<UE_MAX_AM_ENTITY; i++)
  {
    if (am_reset_id_table[i].rlc_id != INIT_CTL_BLK_ID)
    {
      /* Obtain an RRC commend buffer */
      if ((rrc_rlc_cmd_ptr = rrc_get_rlc_cmd_buf()) != NULL)
      {
        rrc_rlc_cmd_ptr->cmd_hdr.cmd_id = RRC_CRLC_STATUS_IND;
        /* Reset cause: Uplink or Downlink  */
        rrc_rlc_cmd_ptr->cmd.crlc_status_ind.direction = TRUE;
        /* Reset or Reset Fail  */
        rrc_rlc_cmd_ptr->cmd.crlc_status_ind.error_type = am_reset_id_table[i].err_type;
        rrc_rlc_cmd_ptr->cmd.crlc_status_ind.lc_id = am_reset_id_table[i].rlc_id;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
        #error code not present
#endif
        /* Enqueue the allocated buffer into RRC command queue  */
        rrc_put_rlc_cmd (as_id, rrc_rlc_cmd_ptr);
        
        if (RLC_RESET_FAIL == am_reset_id_table[i].err_type)
        {
          #ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
          rlc_qsh_metric_global[as_id].num_rlc_urecov_err++;
          #endif
	  
	  #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
          if(rlc_ul_table_ptr->rlc_reset_info.status == TRUE)
          {
	    QSH_LOG(QSH_CLT_WRLC, QSH_CAT_EVENT, QSH_MSG_TYPE_HIGH, "AS ID %d, RLC_QSH: Max RESET, RLC ID %d", as_id, am_reset_id_table[i].rlc_id);
	    wrlc_qsh_report_event(WRLC_QSH_EVENT_MAX_RESET, as_id);
          }
	  #endif
        }
        
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Post RLC RESET to RRC, LC %d",
                                                   am_reset_id_table[i].rlc_id);
      }
      else 
      { 
        ERR("Could not allocate RRC cmd buf", 0, 0, 0);
      }
      am_reset_id_table[i].rlc_id = INIT_CTL_BLK_ID;
    }
  }
}/* rlci_post_reset_to_rrc() */


/*===========================================================================

FUNCTION RLCI_UL_PROC_RESET_MSG

DESCRIPTION
  Process the RLCI_DL_RESET_DONE and RLCI_UL_RESET_REQ messages from 
  DL AM RLC. 
  
DEPENDENCIES

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_ul_proc_reset_msg
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  wcdma_l2_ul_cmd_enum_type cmd_id, 
  rlc_lc_id_type ul_rlc_data_id
)
{
  boolean is_data_present = FALSE, dummy = FALSE;
  rex_crit_sect_type *datapath_mutex = NULL;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  sys_modem_as_id_e_type as_id = rlc_ul_table_ptr->as_id;
  
  #ifdef FEATURE_RLC_QXDM_LOGGING
  rlc_ul_log_state_type *log_state_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_log_state);
  #endif
    
  ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, ul_rlc_data_id);
  
  if(NULL == ctl_blk_ptr) 
  {
    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Skipping RESET for LC %d since channel is released or not configured",
                                               ul_rlc_data_id);
  }
  else
  {
    if (cmd_id == RLCI_DL_RESET_DONE)
    {
      /* Needs protection here, since updating HFN values leads to wrong ciphering in UL
       */
      datapath_mutex = L2_ACQ_UL_LW_LOCK(as_id, ul_rlc_data_id);
  
      ctl_blk_ptr->cipher.hfn = (ctl_blk_ptr->cipher.count >> 12) + 1;
  
      ctl_blk_ptr->cipher.count = ctl_blk_ptr->cipher.hfn << 12 | 
              ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s;
  
      /* Complete the RESET procedure. Return back to the original state. */
      ctl_blk_ptr->lc_state &= (~UL_RESET_PENDING);
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      
      if((TRUE == l1m_state_is_fach(as_id))&& 
	 (TRUE == l1_mac_rach_enable_trigger_needed(as_id)))
      {
        is_data_present = rlc_check_pending_sdu(as_id,ul_rlc_data_id, &dummy);
	if(TRUE == is_data_present)
	{					  
	  WL2_MSG_0(HIGH, rlc_ul_table_ptr,"TRIG_RACH: Pending SDU present in WM - Trigger L1 to enable BFI");
	  mac_rach_ul_trigger_tx_req(L1_UL_PHYCHAN_ENABLE, as_id);
	}
      }
	  
      WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Complete RESET procedure UL new HFN %x, vt_s %x, LC %d",
                ctl_blk_ptr->cipher.hfn, ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s, ul_rlc_data_id);

      /* Let RRC know that the RLC AM reset */
      rlci_set_post_rrc_reset_sig(rlc_ul_table_ptr, ctl_blk_ptr, RLC_RESET, ul_rlc_data_id);
  
  
#ifdef FEATURE_RLC_QXDM_LOGGING
      log_state_ptr->ul_state[0].rlc_id = ul_rlc_data_id;
      switch (ctl_blk_ptr->lc_state) 
      {
        case UL_DATA_TRANSFER_READY:
          log_state_ptr->ul_state[0].lc_state = AM_UL_DATA_TRANSFER_READY;
          break;
        case UL_WAITING_TO_SUSPEND:
          log_state_ptr->ul_state[0].lc_state = AM_UL_WAITING_TO_SUSPEND;
          break;
        case UL_SUSPENDED:
          log_state_ptr->ul_state[0].lc_state = AM_UL_SUSPENDED;
          break;
        case UL_CONFIG_PENDING:
          log_state_ptr->ul_state[0].lc_state = AM_UL_CONFIG_PENDING;
          break;
        default:
          ERR("Invalid state, QXDM",0,0,0);
      }
      log_state_ptr->nchan = 1;
      rlc_ul_send_state_log_packet(rlc_ul_table_ptr, log_state_ptr);
#endif
  
    }
    else if (cmd_id == RLCI_UL_RESET_REQ)
    {
  
    /*
     *  Lock DL mutex so as to make sure that UL-entity is completely
     *  Reset before DL task resumes
     */
      L2_ACQ_DL_LOCK();
  
      /* Rx RESET request from the peer side via the DL AM RLC. */
      rlci_reset_ul_am(rlc_ul_table_ptr, rlc_dl_table_ptr, ctl_blk_ptr, FALSE);
      rlci_request_tx_reset_ack(rlc_ul_table_ptr, ul_rlc_data_id);
      L2_ACQ_DL_UNLOCK();
  
#ifdef FEATURE_RLC_QXDM_LOGGING
      log_state_ptr->ul_state[0].rlc_id = ul_rlc_data_id;
      log_state_ptr->ul_state[0].lc_state = AM_UL_RESET_PENDING;
      log_state_ptr->nchan = 1;
      rlc_ul_send_state_log_packet(rlc_ul_table_ptr, log_state_ptr);
#endif
    
    }
    else if(cmd_id == RLCI_UL_DUP_RESET_REQ)
    {
      rlci_set_duplicate_reset_rx(rlc_ul_table_ptr, ul_rlc_data_id);
    }
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Unknown UL RLC ID %d reset msg",
                                                 ul_rlc_data_id);
    }
  }
}/* rlci_ul_proc_reset_msg() */


/*===========================================================================

FUNCTION CRLC_RELEASE_ALL_HANDLER

DESCRIPTION
  Releases all Uplink Channels and sends a command to the Downlink to 
  release all downlink channels.

DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void crlc_release_all_handler
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr
 )
{
  uint8 ctrl_blk_id,num_ch_released;               /* Index into the Uplink Table          */
  uint8 i = 0;
  uint32 index = 0, act_ul_am_chnl = 0;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rlc_mutexes_type *rlc_mutex_ptr = RLC_GET_MUTEX_PTR(rlc_ul_table_ptr->as_id);
#ifdef FEATURE_RLC_QXDM_LOGGING
  uint8 j = 0;
  rlc_ul_log_state_type *log_state_ptr = &(rlc_ul_table_ptr->rlc_ul_logging.ul_log_state);
#endif //FEATURE_RLC_QXDM_LOGGING 
  num_ch_released = 0;
  L2_MUTEX_LOCK(&(rlc_mutex_ptr->rlc_dl_data_process_mutex));
  L2_ACQ_DL_LOCK();

  /*Taking FC lock to prevent deadlock with datapath mutex between UL task and DL task*/
  RLC_ACQ_FC_LOCK(rlc_ul_table_ptr->as_id);
  /*Locking both EUL and R99 Mutex*/
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  L2_MUTEX_LOCK(&rlc_mutex_ptr->rlc_channel_release_mutex);
  /*-------------------------------------------------------------------------
    Release all active Uplink Channels.
  -------------------------------------------------------------------------*/
  while (rlc_ul_table_ptr->active_ch > 0)
  {
    /*-----------------------------------------------------------------------
     Check if the channel is in NULL state. If it is not, release the 
     channel.  
    -----------------------------------------------------------------------*/
    ctrl_blk_id = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, i);
    if (ctrl_blk_id < UE_MAX_UL_RLC_ENTITY)
    {
      num_ch_released++;
      /*
         LC in NULL-STATE is checked every time ISR tries to access this LC, So keep
         this part of code under protection to make this an atomic operation
      */
      ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, ctrl_blk_id);
      ctl_blk_ptr->lc_state = UL_NULL_STATE;
      
      /* Release discard timer  */
      if(ctl_blk_ptr->tmr_discard_ptr != NULL)
      {
        (void) rex_clr_timer(ctl_blk_ptr->tmr_discard_ptr);
        /* release timer block */
        #ifdef FEATURE_REX_DYNA_MEM
        rex_delete_timer_ex(ctl_blk_ptr->tmr_discard_ptr);
        #endif
        /* Reset timer sig */
        ctl_blk_ptr->tmr_discard_ptr = NULL;
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Release SDU discard timer, %d",
                                                   i);
      }
      if (ctl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
      {
        rlc_ul_table_ptr->active_ch -= rlci_release_ul_am(rlc_ul_table_ptr, rlc_dl_table_ptr, ctl_blk_ptr);
      }
      else
      {
        rlc_ul_table_ptr->active_ch--;
      }

      if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
      {
        rlc_dump_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
      }

      rlc_ul_table_ptr->ul_ctl_blk_id_tbl[i] = INIT_CTL_BLK_ID;                

      if (RLC_DBG_UL_CTL_BLK_ID_TBL & rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var)
      {
        rlc_sanitize_ul_ctl_blk_id_tbl(rlc_ul_table_ptr);
      }
#ifdef FEATURE_RLC_QXDM_LOGGING
      log_state_ptr->ul_state[j].rlc_id = i;
      log_state_ptr->ul_state[j].lc_state = RLC_UL_NULL_STATE;
      j++;
#endif //FEATURE_RLC_QXDM_LOGGING
    
    }
    i++;
  }
  WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Num UL LC released %d, Active num ul ch %d",
                                     num_ch_released,
                                     rlc_ul_table_ptr->active_ch);

#ifdef FEATURE_RLC_QXDM_LOGGING
  if (j > 0) 
  {
    log_state_ptr->nchan = j;
    rlc_ul_send_state_log_packet(rlc_ul_table_ptr, log_state_ptr);
  }
#endif //FEATURE_RLC_QXDM_LOGGING

#ifdef FEATURE_QXDM_DSM_TRACING
  if (rlc_ul_table_ptr->rlc_dsm_trace_params.num_rlc_non_srb_entities_up > 0)
  {
    fc_post_deregister_cmd_dsm_trace
      (DSM_UMTS_TECH,
       UMTS_RLC_CLIENT);
    WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Deregistered Qxdm DSM tracing");
    rlc_ul_table_ptr->rlc_dsm_trace_params.num_rlc_non_srb_entities_up = 0;
  }
#endif


  if (rlc_ul_table_ptr->rlc_acti_non_srb_am_ch)
  {
    rlc_ul_table_ptr->rlc_acti_non_srb_am_ch = 0;
    rlc_cfcm_deregister(rlc_ul_table_ptr->as_id);
  }

  for (index = 0, act_ul_am_chnl = 0; index < UE_MAX_UL_LOGICAL_CHANNEL; index++)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, index);
    
    if ((NULL != ctl_blk_ptr) && 
        (UL_NULL_STATE != ctl_blk_ptr->lc_state) && 
        (UE_MODE_ACKNOWLEDGED == ctl_blk_ptr->lc_mode))
    {
      act_ul_am_chnl++;
      break;
    }
  }

  if ((0 == act_ul_am_chnl) && (TRUE == rlc_ul_table_ptr->rlc_ul_am_base_tmr_start))
  {
    /* Since all AM channels are released, stop base ul am timer */
    (void) rex_clr_timer(&rlc_ul_table_ptr->rlc_ul_am_base_tmr);
    rlc_ul_table_ptr->rlc_ul_am_base_tmr_start = FALSE;
    
    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_TMR_DBG: Stopping base UL timer, active_ul_channels %d",
                                               rlc_ul_table_ptr->active_ch);

  }
  L2_MUTEX_UNLOCK(&rlc_mutex_ptr->rlc_channel_release_mutex);

  /*-------------------------------------------------------------------------
    Post Release Request to Downlink 
  -------------------------------------------------------------------------*/
  crlc_dl_release_all_handler(rlc_ul_table_ptr, rlc_dl_table_ptr);

  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  RLC_ACQ_FC_UNLOCK(rlc_ul_table_ptr->as_id);
  L2_ACQ_DL_UNLOCK();
  L2_MUTEX_UNLOCK(&(rlc_mutex_ptr->rlc_dl_data_process_mutex));

} /* crlc_release_all_handler() */

/*=========================================================================

FUNCTION RLCI_CHANGE_PEER_TX_WIN

DESCRIPTION

    This function changes the peer side's transmission window size.

    This Peer side tx window size changing is triggered by DL activities
    when the configuring DL Rx Win size is greater than the UE can support.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

=========================================================================*/

void rlci_change_peer_tx_win
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_ul_am_ctrl_type *am_ctrl_blk_ptr,
  uint16 rx_window_size
)
{
  rex_crit_sect_type *datapath_mutex = NULL;

  if (am_ctrl_blk_ptr == NULL)
  {
    return;
  }

/*
  UL-ISR sends a WIN-SUFI based on the status of these flags. So lock it under mutex
*/
  datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, am_ctrl_blk_ptr->am_common->ul_data_id);

  am_ctrl_blk_ptr->ul_state.peer_wins = rx_window_size;
  am_ctrl_blk_ptr->ul_state.tx_wins = TRUE;
  am_ctrl_blk_ptr->ul_state.retx_win_size = TRUE;
  am_ctrl_blk_ptr->ul_state.n_retx_win = 0;
  am_ctrl_blk_ptr->ul_state.retx_win_cnt = 0;

  L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  WL2_MSG_1(LOW, rlc_ul_table_ptr, "Change peer's TX Window, LC %d",
                                            am_ctrl_blk_ptr->am_common->ul_data_id);

} /* rlci_change_peer_tx_win */

/*=========================================================================

FUNCTION RLCI_CHANGE_PEER_TX_WIN_FC

DESCRIPTION

    This function changes the peer side's transmission window size.

    This Peer side tx window size changing is triggered by DL activities
    when, UE is already in FLOW CONTROL due to which UE suggested DL Rx
    window is different than NW Configured initial DL Rx Window. This values
    will be reset to initial configured values due to RESET procedure. Need
    to restore them back to values triggered due to flow control.


DEPENDENCIES
  None.

RETURN VALUE
  window size

SIDE EFFECTS
  None.

=========================================================================*/
uint16 rlci_change_peer_tx_win_fc
(
  rlci_uplink_table_type  *rlc_ul_table_ptr,
  rlci_ul_ctrl_blk_type *ctl_blk_ptr
)
{
  uint16 rlc_win_size = 0;
  rex_crit_sect_type *datapath_mutex = NULL;

  datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id);
  if (ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws != ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins)
  {
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.peer_wins = ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws;
    rlc_win_size = ctl_blk_ptr->ctrl.am_ctrl.ul_state.rlc_fc_ws.rlc_fc_ws;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.tx_wins = TRUE;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.retx_win_size = TRUE;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.n_retx_win = 0;
    ctl_blk_ptr->ctrl.am_ctrl.ul_state.retx_win_cnt = 0;
  }
  L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  WL2_MSG_1(LOW, rlc_ul_table_ptr, "Change peer's TX Window, LC %d",
                                            ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id);
  return rlc_win_size;

} /* rlci_change_peer_tx_win */

/*===========================================================================

FUNCTION RLC_GET_DATA_COUNTERS

DESCRIPTION
  Obtain the number of bytes of user data sent/received to/from the network.

DEPENDENCIES
  None.
  
RETURN VALUE
  FALSE: Channel is not existed.
  TRUE: Obtains data successfully.
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean rlc_get_data_counters
( 
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_downlink_table_type *rlc_dl_table_ptr,
  rlc_lc_id_type rlc_id, 
  uint32 *rx_from_network_byte_cnt, 
  uint32 *tx_to_network_byte_cnt 
)
{
#ifdef FEATURE_RLC_QXDM_LOGGING
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  
  if ((ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id)) == NULL)
  {
    return FALSE;
  }
  
  *tx_to_network_byte_cnt = 
    ctl_blk_ptr->ctrl.am_ctrl.ul_chan_am_stat.tot_num_sdu_byte_txd;
  *rx_from_network_byte_cnt = 
    rlci_get_dl_sdu_byte_cnt(rlc_dl_table_ptr, ctl_blk_ptr->ctrl.am_ctrl.am_common->dl_data_id);
  
  return TRUE;
#else
  return FALSE;
#endif

}/* rlc_get_data_counters() */


#ifdef FEATURE_RLC_QXDM_LOGGING
uint32 rlc_ul_enh_update_log_data(uint32           num_src_desc,
                                  uint32           initial_io_vec_len,
                                  uint32           pdu_size_bytes,
                                  rlc_buff_type    *hdr_list, 
                                  l1_l2_data_type  *l1_ul_tx_buf_ptr,
                                  uint32            data_length_bits)
{
  uint32 log_data, temp_data, num_data_bytes;
  
  log_data = num_data_bytes = temp_data = 0;
  
  WCDMA_MEMCPY(&log_data, 
               sizeof(uint32),
               hdr_list->buff,
               MIN(hdr_list->num_bytes, 4));
  
  if (hdr_list->num_bytes < MIN(4, pdu_size_bytes))
  {
    num_data_bytes = MIN((4 - hdr_list->num_bytes), CONVERT_BITS_TO_BYTES(data_length_bits));
    if (num_data_bytes > 0)
    {
      if ((0 != initial_io_vec_len) && 
         (GET_IO_VEC_LEN(l1_ul_tx_buf_ptr->cur_ul_buf_ptr[num_src_desc].length) > initial_io_vec_len))
      {
       /*-----------------------------------------------------------------------
         new PDU data was added within previous IOVEC.length
       -----------------------------------------------------------------------*/
       WCDMA_MEMCPY(&temp_data, 
                    sizeof(uint32),
                    l1_ul_tx_buf_ptr->cur_ul_buf_ptr[num_src_desc].data_seg_list_ptr + initial_io_vec_len, 
                    num_data_bytes);
      }
      else if ((l1_ul_tx_buf_ptr->num_src_desc > 0) &&
              ((0 == initial_io_vec_len) || (l1_ul_tx_buf_ptr->num_src_desc > ++num_src_desc)))
      {
       /*-----------------------------------------------------------------------
        new PDU data was added within next IOVEC
       -----------------------------------------------------------------------*/
       WCDMA_MEMCPY(&temp_data,
                    sizeof(uint32),
                    l1_ul_tx_buf_ptr->cur_ul_buf_ptr[num_src_desc].data_seg_list_ptr, 
                    num_data_bytes);
      }
    }
    
    log_data |= (uint32) (temp_data << (hdr_list->num_bytes * 8));
  }
  return log_data;
}

/*===========================================================================

FUNCTION RLCI_UL_AM_HANDLE_QXDM_LOG_TIMER

DESCRIPTION
  Collect the AM statistics data and send the statistics log packet out.
    
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlci_ul_am_handle_qxdm_log_timer
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  uint32 ul_log_timer_sig
   /*lint -esym(715,ul_log_timer_sig) */
)
{
  /* Set this log timer tick again */
  (void) rex_set_timer(&rlc_ul_table_ptr->rlc_ul_logging.ul_tmr_log_blk, (uint32)RLC_AM_LOG_TIMER_BASE); 
  
  /*---------------------------------------------------------------------
   Get the log packet with all the statistics information for all the 
   channels to be sent to qxdm for logging purposes every 3seconds.  
   Send the log packet to the qxdm for logging purposes every 3seconds.  
  ---------------------------------------------------------------------*/
  
  if (log_status(WCDMA_UL_RLC_V2_AM_STAT_LOG_PACKET))
  {
    WL2_MSG_0(MED, rlc_ul_table_ptr, "Sending V2 log packet");
    rlc_ul_tx_am_statistic_pkt(rlc_ul_table_ptr);
  }
}/* rlci_ul_am_handle_qxdm_log_timer() */

#endif //FEATURE_RLC_QXDM_LOGGING

/*===========================================================================

FUNCTION RLC_RRC_GET_HFN_REQ

DESCRIPTION
   This function is called by RRC to get latest HFN's for all established
   RLC UM and AM radio-bearers.For cipher pending logical channels a value
   of zero is retuned. RLC populates the * to struct rlc_hfn_req_type.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS:  If HFN's for all UM and AM rb's are populated.
  FAILURE:  If no UM and AM rb's are present.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type rlc_rrc_get_hfn_req
(
  sys_modem_as_id_e_type as_id,
  rlc_hfn_req_type* hfn_req             /*ptr to hfn request type          */
)
{
  uint16  count;                        /* Number of active Channels       */
  uint16  index = 0;
  uint8 array_len = 0;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;

  l2_validate_as_id(as_id);
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);

  /*------------------------------------------------------------------------
    Initialize the number of channels and index to 0.
  ------------------------------------------------------------------------*/
  hfn_req->ul_nchan = 0;

  /*------------------------------------------------------------------------
    Get the number of active channels from the uplink table.
  ------------------------------------------------------------------------*/
  count = rlc_ul_table_ptr->active_ch;
  array_len = sizeof(hfn_req->hfn_per_rb_ul)/sizeof(rlc_hfn_type);
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  
  while((count > 0) && (index < UE_MAX_UL_RLC_ENTITY) && (hfn_req->ul_nchan < array_len))
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
    if ((NULL != ctl_blk_ptr) &&
    	(ctl_blk_ptr->lc_state != UL_NULL_STATE))  
    {
      count--;
      if (ctl_blk_ptr->lc_mode != UE_MODE_TRANSPARENT &&
          ctl_blk_ptr->cipher.cipher_mode != NO_CIPHER)
      {
        if (ctl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED)
        {
          /*------------------------------------------------------------------------
            Remove the LS 7 bits as of SN.
          ------------------------------------------------------------------------*/
          if (ctl_blk_ptr->cipher.cipher_mode == CIPHER_ON)
          {
            hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn_valid = TRUE;
          }
          else
          {
            hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn_valid = FALSE;

          }

          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn = ctl_blk_ptr->cipher.count >> 7;
          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_sn = ctl_blk_ptr->ctrl.um_ctrl.vt_us;
          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_id = ctl_blk_ptr->ctrl.um_ctrl.rlc_id;
        }
        else
        {
          /*------------------------------------------------------------------------
            Remove the LS 12 bits as of SN.
          ------------------------------------------------------------------------*/
          if (ctl_blk_ptr->cipher.cipher_mode == CIPHER_ON)
          {
            hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn_valid = TRUE;
          }
          else
          {
            hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn_valid = FALSE;
          }

          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn = ctl_blk_ptr->cipher.count >> 12;
          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_sn = ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s;
          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_id = ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id;
        }
        
        WL2_MSG_3(HIGH, rlc_ul_table_ptr, "RLC_DBG: RRC req UL Cipher LC %d, rlc_sn %d hfn 0x%x",
                          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_id, 
                          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].rlc_sn,
                          hfn_req->hfn_per_rb_ul[hfn_req->ul_nchan].hfn);

        hfn_req->ul_nchan ++;
      }
    }
    index++;
  }
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  /* Get DL HFN */
  (void) rlci_get_dl_hfn(rlc_dl_table_ptr, hfn_req);

  if (hfn_req->ul_nchan > 0)
  {
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
}/* rlc_rrc_get_hfn_req() */

/*===========================================================================

FUNCTION RLC_RRC_GET_COUNT_C_REQ

DESCRIPTION
   This function is called by RRC to get latest COUNT_C's for all established
   RLC UM and AM radio-bearers.  RLC populates the * to struct 
   rlc_count_c_req_type.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS:  If COUNT_C's for all UM and AM rb's are populated.
  FAILURE:  If no UM and AM rb's are present.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type rlc_rrc_get_count_c_req
(
  sys_modem_as_id_e_type as_id,
  rlc_count_c_req_type* count_c_req     /*ptr to hfn request type          */
)
{
  uint16  count;                        /* Number of active Channels       */
  uint16  index = 0;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;

  l2_validate_as_id(as_id);
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);

  /*------------------------------------------------------------------------
    Initialize the number of channels and index to 0.
  ------------------------------------------------------------------------*/
  count_c_req->ul_nchan = 0;

  /*------------------------------------------------------------------------
    Get the number of active channels from the uplink table.
  ------------------------------------------------------------------------*/
  count = rlc_ul_table_ptr->active_ch;

  while(count > 0 && index < UE_MAX_UL_RLC_ENTITY)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
    if ((NULL != ctl_blk_ptr) && 
    	(ctl_blk_ptr->lc_state != UL_NULL_STATE))  
    {
      count--;
      if ((ctl_blk_ptr->lc_mode != UE_MODE_TRANSPARENT) &&
          (ctl_blk_ptr->cipher.cipher_mode != NO_CIPHER) &&
          (count_c_req->ul_nchan < (UE_MAX_UL_UM_CHANNEL + UE_MAX_AM_ENTITY))) 
      {

        if (ctl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED)
        {
          count_c_req->count_c_per_rb_ul[count_c_req->ul_nchan].count_c = ctl_blk_ptr->cipher.count;
          count_c_req->count_c_per_rb_ul[count_c_req->ul_nchan].rlc_id = ctl_blk_ptr->ctrl.um_ctrl.rlc_id;
        }
        else
        {
        count_c_req->count_c_per_rb_ul[count_c_req->ul_nchan].count_c = ctl_blk_ptr->cipher.count;
        count_c_req->count_c_per_rb_ul[count_c_req->ul_nchan].rlc_id = ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id;
        }
      
        count_c_req->ul_nchan ++;
      }
    }
    index++;
  }

  /* Get DL HFN */
  (void) rlci_get_dl_count_c(rlc_dl_table_ptr, count_c_req);

  if (count_c_req->ul_nchan > 0)
  {
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
}/* rlc_rrc_get_count_c_req() */

/*===========================================================================

FUNCTION RLC_RRC_UL_RLC_SN_REQ

DESCRIPTION
   This function is called by RRC to get Sequence numbers for a 
   particular RB or for a set of RB's.  The sn_req_type * is
   populated by RLC for all the RB's requested by RRC.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS:  If SN's for all requested RB's are populated.
  FAILURE:  If unable to populate SN for requested RB's.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type rlc_rrc_ul_rlc_sn_req
(
  sys_modem_as_id_e_type as_id,
  rlc_ul_sn_req_type* sn_req            /*ptr to SN request type           */
)
{
  uint16  count;                        /* Number of active Channels       */
  uint8  index = 0;
  uint8 array_len = 0;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;

  l2_validate_as_id(as_id);
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  
  /*------------------------------------------------------------------------
    Initialize the number of channels and index to 0.
  ------------------------------------------------------------------------*/
  sn_req->nchan = 0;

  /*------------------------------------------------------------------------
    Get the number of active channels from the uplink table.
  ------------------------------------------------------------------------*/
  count = rlc_ul_table_ptr->active_ch;
  array_len = sizeof(sn_req->rlc_ul_sn)/sizeof(rlc_sn_type);
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  while((count > 0) && (index < UE_MAX_UL_RLC_ENTITY) && (sn_req->nchan < array_len))
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, index);
    if ((NULL != ctl_blk_ptr) &&
    	(ctl_blk_ptr->lc_state != UL_NULL_STATE))  
    {
      count--;
      switch (ctl_blk_ptr->lc_mode)
      {
        case UE_MODE_ACKNOWLEDGED:
        case UE_MODE_ACKNOWLEDGED_DATA:
        case UE_MODE_ACKNOWLEDGED_CONTROL:
          sn_req->rlc_ul_sn[sn_req->nchan].lc_id = ctl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id;
          sn_req->rlc_ul_sn[sn_req->nchan].rlc_sn = ctl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s;
          sn_req->nchan ++;
          break;

        case UE_MODE_UNACKNOWLEDGED:
          sn_req->rlc_ul_sn[sn_req->nchan].lc_id = ctl_blk_ptr->ctrl.um_ctrl.rlc_id;
          sn_req->rlc_ul_sn[sn_req->nchan].rlc_sn = ctl_blk_ptr->ctrl.um_ctrl.vt_us;
          sn_req->nchan ++;
          break;

        case UE_MODE_TRANSPARENT:
          break;
       
        default:
          WL2_MSG_0(MED, rlc_ul_table_ptr, "Invalid UE mode");
      }
    }
    index++;
  }
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  if (sn_req->nchan > 0)
  {
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
}/* rlc_rrc_ul_rlc_sn_req() */


/*===========================================================================

FUNCTION RLC_RRC_GET_WM_INFO_REQ

DESCRIPTION
   This function is called by RRC to get number of bytes in RLC Watermark queue.

DEPENDENCIES
  None.

RETURN VALUE
  RLC returns total number of bytes(uint32) awaiting transmission on the requested
  logical channel.  The following are accounted in bytes:
  1. All SDUs in the water mark Q along with length indicators.
  2. Length indicators, resulting from previous RLC PDU transmission, that indicate
     the previous tx. SDU fits exactly or one octet short of the RLC PDU
     "special_ind" indicates the previouly tx. SDU exactly fits into in AM PDU.
     "octet_less" indicates the previouly tx. SDU is one octet short of AM PDU.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 rlc_rrc_get_wm_info_req
(
  sys_modem_as_id_e_type as_id,

  rlc_lc_id_type lc_id             /* logical channel ID for which length of
                                      WM is needed                          */
)
{
  rlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;
  uint32 num_bytes = 0;
  rex_crit_sect_type *datapath_mutex = NULL;
  rlci_uplink_table_type *rlc_ul_table_ptr ;
  
  l2_validate_as_id(as_id);
  
  rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, lc_id);
  if (NULL != ctl_blk_ptr)
  {
    datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, lc_id);

    if(ctl_blk_ptr->ul_wm_ptr != NULL)
    {
      num_bytes = RLCI_TOTAL_BYTES_UL_WM_PLUS_DEQUEUED(ctl_blk_ptr)+
                ((uint32)RLCI_TOTAL_SDUS_UL_WM_PLUS_DEQUEUED(ctl_blk_ptr)*
                 ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length) +
                (ctl_blk_ptr->special_ind ?
                 ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length : 0) +
                (ctl_blk_ptr->octet_less ?
                 ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length : 0);
      if (NULL != ctl_blk_ptr->cur_sdu_ptr)
      {
        num_bytes += ((ctl_blk_ptr->cur_sdu_len - ctl_blk_ptr->cur_sdu_tx_len)
                 + ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length);
      }
    }
    else
    {
      num_bytes = (ctl_blk_ptr->special_ind ?
                   ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length : 0) +
                  (ctl_blk_ptr->octet_less ?
                   ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length : 0);
      if (NULL != ctl_blk_ptr->cur_sdu_ptr)
      {
        num_bytes += ((ctl_blk_ptr->cur_sdu_len - ctl_blk_ptr->cur_sdu_tx_len)
                 + ctl_blk_ptr->ctrl.am_ctrl.ul_state.li_length);
      }
    }
    L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  }

  WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Payload size, LC %d, num_bytes %d",
                                             lc_id, num_bytes);

  return num_bytes;

}/* rlc_rrc_get_wm_info_req() */


#ifdef FEATURE_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLCI_GET_UL_PDU_CIPHER_LOG_ON

DESCRIPTION
  This function will be call when each PDU is building. This function is to 
  avoid to check the logging bit from the DIAG every time when a PDU is 
  building, variable rlci_ul_pdu_cipher_log_on will be updated at the 
  beginning of building PDUs each TTI. 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE if the PDU ciphering parameters logging is on.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlci_get_ul_pdu_cipher_log_on (rlci_uplink_table_type *rlc_ul_table_ptr)
{
  return rlc_ul_table_ptr->rlc_ul_logging.rlci_ul_pdu_cipher_log_on;
}


/*===========================================================================

FUNCTION RLCI_GET_UL_PDU_CIPHER_LOG_ON

DESCRIPTION
  Provide the access pointer of rlci_ul_pdu_cipher_log data structure.  

DEPENDENCIES
  None.

RETURN VALUE
  pointer of rlci_ul_pdu_cipher_log.

SIDE EFFECTS
  None.

===========================================================================*/
rlc_ul_pdu_cipher_info_type *rlci_get_ul_pdu_cipher_log_ptr(rlci_uplink_table_type *rlc_ul_table_ptr, rlc_lc_id_type ul_rlc_id)
{

  rlc_ul_channel_type channel_type = CHANNEL_MAX;

  channel_type = mac_ul_get_channel_type(rlc_ul_table_ptr->as_id, ul_rlc_id);
  if (CHANNEL_DCH == channel_type)
  {
    return &(rlc_ul_table_ptr->rlc_ul_logging.rlci_ul_pdu_cipher_log);
  }
  else
  {                                       
    return &(rlc_ul_table_ptr->rlc_ul_logging.rlci_eul_pdu_cipher_log);
  }
}
#endif //FEATURE_RLC_QXDM_LOGGING

/*===========================================================================

FUNCTION RLC_UL_REMOVE_SDUS_FROM_WM

DESCRIPTION
  This function removes the SDUs from the UL WM. The number of SDUs that 
  are purged is the minimum of number specified by higher layers and the
  number of SDUs present in the WM.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
rlc_ul_wm_purge_rsp_status rlc_ul_remove_sdus_from_wm
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlci_ul_ctrl_blk_type *ctrl_blk_ptr,
  uint8 num_sdus
)
{
  dsm_item_type *temp_ptr, *chain_head_ptr;
  uint8 num_sdus_to_discard, num_sdus_in_wm, mui;
  uint32 deq_pkt_len, num_sdus_dequeued;
  rlc_ul_wm_purge_rsp_status purge_status;
  
  temp_ptr = chain_head_ptr = NULL;
  num_sdus_to_discard = num_sdus_in_wm = mui = deq_pkt_len = 0;
  purge_status = RLC_PURGE_REQ_FAIL_WM_NOT_REG;
  
  #ifdef FEATURE_WCDMA_UL_COMPR
  L2_MUTEX_LOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_ul_comp_dacc_mutex));
  #endif
  
  if(ctrl_blk_ptr->ul_wm_ptr != NULL)
  {
    num_sdus_in_wm = (uint8)q_cnt(ctrl_blk_ptr->ul_wm_ptr->q_ptr);
  }
  
  if((num_sdus_in_wm != 0) && (ctrl_blk_ptr->ul_wm_ptr != NULL))
  {
    /*-----------------------------------------------------------------------
      Might have to indicate SDU failure to higher layers in future
    -----------------------------------------------------------------------*/
    num_sdus_to_discard = MIN(num_sdus, num_sdus_in_wm);
    num_sdus_dequeued = dsm_multiple_dequeue(ctrl_blk_ptr->ul_wm_ptr, num_sdus_to_discard, &chain_head_ptr, &deq_pkt_len);
    
    WL2_MSG_4(HIGH, rlc_ul_table_ptr, "RLC_WM: Discarding %d SDUs, no_sdu %d, q_cnt %d frm WM, num_sdus_dequeued %d",
                                               num_sdus_to_discard,
                                               num_sdus,
                                               num_sdus_in_wm,
                                               num_sdus_dequeued );
    
    while(deq_pkt_len > 0)
    {
      if (chain_head_ptr == NULL)
      {
        WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_WM: Unexpected NULL Pointer. deq_pkt_len %d, num_sdus_to_discard %d num_sdus_dequeued %d", deq_pkt_len, num_sdus_to_discard, num_sdus_dequeued);
        break;
      }
      else
      {
        temp_ptr = dsm_pkt_chain_delink(&chain_head_ptr);
        deq_pkt_len -= RLC_GET_DSM_LENGTH(temp_ptr);
        
        /*-------------------------------------------------------------------
          If the mode is AM mode, report the failure for tx of the SDUs
          that are purged.
        -------------------------------------------------------------------*/
        if(ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
        {
          if (ctrl_blk_ptr->ctrl.am_ctrl.sdu_cnf_cb != NULL)
          {
            mui = (uint8)((temp_ptr->app_field & 0x0000FF00) >> 8);
            if (temp_ptr->app_field & 0x00000001)
            {
              ctrl_blk_ptr->ctrl.am_ctrl.sdu_cnf_cb(rlc_ul_table_ptr->as_id,
              ctrl_blk_ptr->ctrl.am_ctrl.am_common->ul_data_id, 
              FAILURE, mui, WM_PURGE_REQ);
            }
          }
        }
        
        dsm_free_packet(&temp_ptr);
      }
      num_sdus_to_discard--;
      num_sdus_dequeued--;
    } /*while num_sdus_to_discard > 0 */
    purge_status = RLC_PURGE_REQ_SUCCESS;
  } /* if() */
  else
  {
    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC_WM: WM is empty - num_sdus_in_wm %d,UL_WM_PTR %x ",
                                               num_sdus_in_wm,
                                               ctrl_blk_ptr->ul_wm_ptr);
  }
  WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_WM: Return status %d",
                                             purge_status);
  #ifdef FEATURE_WCDMA_UL_COMPR
  L2_MUTEX_UNLOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_ul_comp_dacc_mutex));
  #endif
  
  return purge_status;
} /* rlc_ul_remove_sdus_from_wm() */

/*===========================================================================

FUNCTION RLC_UL_PURGE_UPLINK_WM

DESCRIPTION
  This function purges the SDUs from the uplink WM for the logical channels
  specified in the UL command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_purge_uplink_wm
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_purge_wm_ind_type *ul_purge_wm_ind
)
{
  rlc_lc_id_type lc_id;
  uint8 ctl_blk_id, i;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr = NULL;
  rex_crit_sect_type *datapath_mutex = NULL;
  rlc_ul_wm_purge_rsp_status purge_status = RLC_PURGE_REQ_FAIL_WM_NOT_REG;

  /*-------------------------------------------------------------------------
    For each logical channel specified, set the semaphore and then purge the 
    WM. This would prevent reporting of buffer status on those
    logical channels when an interrupt comes in for building the PDUs.
  -------------------------------------------------------------------------*/
  for (i=0; i < ul_purge_wm_ind->num_lc_id; i++)
  {
    lc_id = ul_purge_wm_ind->lc_id[i];

    /*-----------------------------------------------------------------------
      Get the index for the control block in the uplink table 
    -----------------------------------------------------------------------*/
    ctl_blk_id = rlci_get_ul_ctl_blk_index_for_rlc_id(rlc_ul_table_ptr, lc_id);
    if(ctl_blk_id < UE_MAX_UL_RLC_ENTITY)
    {
      ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, ctl_blk_id);
      WL2_MSG_3(HIGH, rlc_ul_table_ptr, "RLC_WM: Purge SDUs frm WM, LC %d, id %d, num %d",
                                                 lc_id,
                                                 ctl_blk_id,
                                               ul_purge_wm_ind->num_sdus[i]);

      /*
        In UL-ISR, status of this variable is used as reference. So protect this 
        part of the code, under LW mutex.
      */
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, lc_id);
      ctl_blk_ptr->skip_pdu_build = TRUE;
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      purge_status = rlc_ul_remove_sdus_from_wm(rlc_ul_table_ptr, ctl_blk_ptr, ul_purge_wm_ind->num_sdus[i]);
      
      if(ul_purge_wm_ind->purge_ind_cb != NULL)
      {
        WL2_MSG_3(HIGH, rlc_ul_table_ptr, "RLC_WM: LC %d Response %d context 0x%x",
                                                   lc_id,
                                                   purge_status,
                                                   ul_purge_wm_ind->bearer_context);
        ul_purge_wm_ind->purge_ind_cb(lc_id,purge_status,ul_purge_wm_ind->bearer_context);
      }
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, lc_id);
      ctl_blk_ptr->skip_pdu_build = FALSE;
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    }
    else
    {
      WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_WM: Invalid LC ID %d",
                                                  lc_id);
    }
  } /* for */

} /* rlc_ul_purge_uplink_wm() */

/*===========================================================================

FUNCTION CRLC_UL_ABORT_CIPHER_CONFIG

DESCRIPTION
  This function reverts back to the old ciphering configuration if an abort
  is received from RRC while applying the new configuration due to triggering
  of cell update in RRC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_abort_cipher_config
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_abort_cipher_config_type *abort_cipher_info
)
{
  int i = 0;
  rlci_ul_ctrl_blk_type *ctl_blk = NULL;
  rex_crit_sect_type *datapath_mutex = NULL;

  L2_ACQ_DL_LOCK();
  
  for(i=0;i<abort_cipher_info->nchan;i++)
  {
    /*-----------------------------------------------------------------------
      Get the index to the control block from the logical channel id
      and the control block.
    -----------------------------------------------------------------------*/
    if(NULL == (ctl_blk = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, abort_cipher_info->lc_id[i])))
    {
      WL2_MSG_0(ERROR, rlc_ul_table_ptr, "RLC_ERR: Ctl_blk is Null");
      continue;
    }

    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Abort UL Cipher Config for RbId: %d,LC Id: %d",
                                               ctl_blk->cipher.rb_id,
                                               abort_cipher_info->lc_id[i]);
    /*-----------------------------------------------------------------------
      Revert to the old ciphering configuration.
    -----------------------------------------------------------------------*/
    ctl_blk->cipher = ctl_blk->cipher_backup;

    /*-----------------------------------------------------------------------
      For rb id 2, the sequence number could have gone ahead since the time
      the new SMC was received, so ciphering parameters need to be updated. 
      For other rb id's this would not be the case since they were suspened.
    -----------------------------------------------------------------------*/
    if(ctl_blk->cipher.rb_id == 2)
    {
    /* 
      These parameters are used for ciphering UL-PDUs, so protect this code completely
    */
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, abort_cipher_info->lc_id[i]);
      switch (ctl_blk->cipher.cipher_mode)
      {
        /*-------------------------------------------------------------------
          If the ciphering was on before the new SMC, then re-calculate the
          count_c.
        -------------------------------------------------------------------*/
        case CIPHER_ON:
          /*lint -e514 */
          ctl_blk->cipher.count = (((ctl_blk->cipher.count >> 12) +
            (ctl_blk->ctrl.am_ctrl.ul_state.vt_s < 
                                  (ctl_blk->cipher.count & 0xFFF))) << 12) |
              ctl_blk->ctrl.am_ctrl.ul_state.vt_s;
          /*lint +e514 */
          break;
               
        /*-------------------------------------------------------------------
          If the case was either Cipher mod or cipher wait, then check if the
          sequence number at which new ciphering configuration was to be 
          applied has passed or not. If it has, update the ciphering 
          parameters.
        -------------------------------------------------------------------*/
        case CIPHER_MOD:
        case CIPHER_WAIT:

          /*-----------------------------------------------------------------
            If two confiurations were pending (though it is very unlikely
            to happen) check if the sequence number
            has passed the activation seq no. for the second one.
          -----------------------------------------------------------------*/
          if(ctl_blk->cipher.two_pending)
          {
            if(RLCI_SEQ_GE(ctl_blk->ctrl.am_ctrl.ul_state.vt_s,
                   ctl_blk->cipher.act_sn_new1, RLC_AM_MODULUS))
            {
              /*-------------------------------------------------------------
                If the mode was cipher_mod, save the current config as old1,
                new as old and new1 as current.
              -------------------------------------------------------------*/
              if(ctl_blk->cipher.cipher_mode == CIPHER_MOD)
              {
                ctl_blk->cipher.act_old_cfg1 = TRUE;
                ctl_blk->cipher.cipher_key_id_old1 = 
                                               ctl_blk->cipher.cipher_key_id;
                ctl_blk->cipher.cipher_algo_old1 = 
                                                 ctl_blk->cipher.cipher_algo;
                ctl_blk->cipher.count_old1 = ctl_blk->cipher.count;
                ctl_blk->cipher.act_sn_old1 = ctl_blk->cipher.act_sn;
              }

              /*--------------------------------------------------------------
                Make the new config as old one.
              --------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id_old = 
                                           ctl_blk->cipher.cipher_key_id_new;
              ctl_blk->cipher.cipher_algo_old = 
                                           ctl_blk->cipher.cipher_algo_new;
              ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn_new;
              ctl_blk->cipher.count_old  = ctl_blk->cipher.act_sn_old | 
                                             (ctl_blk->cipher.hfn_new << 12);
              ctl_blk->cipher.act_old_cfg = TRUE;

              /*-------------------------------------------------------------
                Make the new1 config as the current config.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id = 
                                          ctl_blk->cipher.cipher_key_id_new1;
              ctl_blk->cipher.cipher_algo = ctl_blk->cipher.cipher_algo_new1;
              ctl_blk->cipher.hfn = ctl_blk->cipher.hfn_new1;
              ctl_blk->cipher.act_sn = ctl_blk->cipher.act_sn_new1; 
              /*lint -e514 */
              ctl_blk->cipher.count = ctl_blk->ctrl.am_ctrl.ul_state.vt_s | 
               (((ctl_blk->ctrl.am_ctrl.ul_state.vt_s < ctl_blk->cipher.act_sn)+ 
                                    ctl_blk->cipher.hfn_new1) << 12);
              /*lint +e514 */

              /*--------------------------------------------------------------
                Set the ciphering mode to CIPHER_ON and two_pending flag to 
                false.
              --------------------------------------------------------------*/
              ctl_blk->cipher.cipher_mode = CIPHER_ON;
              ctl_blk->cipher.two_pending = FALSE;
              
              WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Cipher ON UL AM, LC %d, key_id %d, cnt %x",
                  ctl_blk->ctrl.am_ctrl.am_common->ul_data_id,
                                                         ctl_blk->cipher.cipher_key_id,
                                                         ctl_blk->cipher.count);
            }
            /*---------------------------------------------------------------
              If vt_s lies between the activation seq no. of new and 
              activation seq no. of new1, then backup old config as old1,
              current as old and new as current.
            ---------------------------------------------------------------*/
            else if(RLCI_SEQ_GE(ctl_blk->ctrl.am_ctrl.ul_state.vt_s,
                    ctl_blk->cipher.act_sn_new, RLC_AM_MODULUS))
            {
              if(ctl_blk->cipher.cipher_mode == CIPHER_MOD)
              {
                /*-----------------------------------------------------------
                  Back up old cipher config if present.
                -----------------------------------------------------------*/
                if (ctl_blk->cipher.act_old_cfg)
                {
                  ctl_blk->cipher.act_old_cfg1 = TRUE;
                  ctl_blk->cipher.cipher_key_id_old1 = 
                                           ctl_blk->cipher.cipher_key_id_old;
                  ctl_blk->cipher.cipher_algo_old1 = 
                                             ctl_blk->cipher.cipher_algo_old;
                  ctl_blk->cipher.count_old1 = ctl_blk->cipher.count_old;
                  ctl_blk->cipher.act_sn_old1 = ctl_blk->cipher.act_sn_old;
                }
                else
                {
                  ctl_blk->cipher.act_old_cfg = TRUE;
                }
                /*-----------------------------------------------------------
                  Backup the current config as old.
                -----------------------------------------------------------*/
                ctl_blk->cipher.cipher_key_id_old = 
                                               ctl_blk->cipher.cipher_key_id;
                ctl_blk->cipher.cipher_algo_old = 
                                                 ctl_blk->cipher.cipher_algo;
                ctl_blk->cipher.count_old = ctl_blk->cipher.count;
                ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
              }
              /*-------------------------------------------------------------
                Apply the new config and set it to the current config.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id = 
                                           ctl_blk->cipher.cipher_key_id_new;
              ctl_blk->cipher.cipher_algo = ctl_blk->cipher.cipher_algo_new;
              ctl_blk->cipher.hfn = ctl_blk->cipher.hfn_new;
              ctl_blk->cipher.act_sn = ctl_blk->cipher.act_sn_new;
              /*lint -e514 */
              ctl_blk->cipher.count = ctl_blk->ctrl.am_ctrl.ul_state.vt_s | 
                (((ctl_blk->ctrl.am_ctrl.ul_state.vt_s < ctl_blk->cipher.act_sn)+ 
                                    ctl_blk->cipher.hfn) << 12);
              /*lint +e514 */
              WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Cipher 1st ON UL AM, LC %d, key_id %d, cnt %x",
                        ctl_blk->ctrl.am_ctrl.am_common->ul_data_id,
                                                         ctl_blk->cipher.cipher_key_id,
                                                         ctl_blk->cipher.count);

              /*-------------------------------------------------------------
                Set the new1 configuration to new.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id_new = 
                                          ctl_blk->cipher.cipher_key_id_new1;
              ctl_blk->cipher.cipher_algo_new = 
                                            ctl_blk->cipher.cipher_algo_new1;
              ctl_blk->cipher.hfn_new = ctl_blk->cipher.hfn_new1;
              ctl_blk->cipher.act_sn_new = ctl_blk->cipher.act_sn_new1;

              /*--------------------------------------------------------------
                Set the Cipher mode to CIPHER_MOD and two_pending flag to 
                false.
              --------------------------------------------------------------*/
              ctl_blk->cipher.cipher_mode = CIPHER_MOD;
              ctl_blk->cipher.two_pending = FALSE;
            }
          }
          /*-----------------------------------------------------------------
            If there were no two pending configurations present and VT_S
            has passed the activation seq no., then apply the new ciphering
            config.
          -----------------------------------------------------------------*/
          else
          {
            if(RLCI_SEQ_GE(ctl_blk->ctrl.am_ctrl.ul_state.vt_s,
                 ctl_blk->cipher.act_sn_new, RLC_AM_MODULUS))
            {
              if(ctl_blk->cipher.cipher_mode == CIPHER_MOD)
              {
                /*-----------------------------------------------------------
                  Back up old cipher config.
                -----------------------------------------------------------*/
                if (ctl_blk->cipher.act_old_cfg)
                {
                  ctl_blk->cipher.act_old_cfg1 = TRUE;
                  ctl_blk->cipher.cipher_key_id_old1 = 
                                           ctl_blk->cipher.cipher_key_id_old;
                  ctl_blk->cipher.cipher_algo_old1 = 
                                             ctl_blk->cipher.cipher_algo_old;
                  ctl_blk->cipher.count_old1 = ctl_blk->cipher.count_old;
                  ctl_blk->cipher.act_sn_old1 = ctl_blk->cipher.act_sn_old;
                }
                else
                {
                  ctl_blk->cipher.act_old_cfg = TRUE;
                }
                /*-----------------------------------------------------------
                  Backup the current config as old.
                -----------------------------------------------------------*/
                ctl_blk->cipher.cipher_key_id_old = 
                                               ctl_blk->cipher.cipher_key_id;
                ctl_blk->cipher.cipher_algo_old = 
                                                 ctl_blk->cipher.cipher_algo;
                ctl_blk->cipher.count_old = ctl_blk->cipher.count;
                ctl_blk->cipher.act_sn_old = ctl_blk->cipher.act_sn;
              } 
              /*-------------------------------------------------------------
                Apply the new ciphering config.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_key_id = 
                                           ctl_blk->cipher.cipher_key_id_new;
              ctl_blk->cipher.cipher_algo = ctl_blk->cipher.cipher_algo_new;
              ctl_blk->cipher.hfn = ctl_blk->cipher.hfn_new;
              ctl_blk->cipher.act_sn = ctl_blk->cipher.act_sn_new;
              /*lint -e514 */
              ctl_blk->cipher.count = ctl_blk->ctrl.am_ctrl.ul_state.vt_s | 
               (((ctl_blk->ctrl.am_ctrl.ul_state.vt_s < ctl_blk->cipher.act_sn) + 
                                    ctl_blk->cipher.hfn_new) << 12);
              /*lint +e514 */
              /*-------------------------------------------------------------
                Set the mode to CIPHER_ON.
              -------------------------------------------------------------*/
              ctl_blk->cipher.cipher_mode = CIPHER_ON;
            }
          }
          break;
            
        case NO_CIPHER: 
          break;

        default:
          WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Invalid Ciphering State");

      }
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    }
  }
  
  L2_ACQ_DL_UNLOCK();
  
}

/*===========================================================================

FUNCTION RLC_QUERY_DATA_CHANNEL_ACTIVITY

DESCRIPTION
   This function is called by RRC to check if there is any activity going
   on in the user plane.

DEPENDENCIES
  None.

RETURN VALUE
  DATA_TRANSFERRED: If there is any activity going on.
  NO_DATA_TRANSFERRED: Whenever RRC resets the flag.

SIDE EFFECTS
  None.

===========================================================================*/

rlc_data_activity_type rlc_query_data_channel_activity
(
  sys_modem_as_id_e_type as_id
)
{
  l2_validate_as_id(as_id);
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC inactivity flag %d",
                                             rlc_ul_table_ptr->rlc_config_pending_var.rlc_activity_flag);
  return rlc_ul_table_ptr->rlc_config_pending_var.rlc_activity_flag;
}

/*===========================================================================

FUNCTION RLC_SET_ACTIVITY_FLAG

DESCRIPTION
  Set the RLC Activity Flag to DATA_TRANSFERRED.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_set_activity_flag
(
  rlci_uplink_table_type *rlc_ul_table_ptr
)
{
  rlc_ul_table_ptr->rlc_config_pending_var.rlc_activity_flag = DATA_TRANSFERRED;
}

/*===========================================================================

FUNCTION RLC_RESET_ACTIVITY_FLAG

DESCRIPTION
   This function is called by RRC to set the activity flag to 
   NO_DATA_TRANSFERRED.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void rlc_reset_activity_flag
(
  sys_modem_as_id_e_type as_id
)
{
  l2_validate_as_id(as_id);
  
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  WL2_MSG_0(HIGH, rlc_ul_table_ptr, "Resetting rlc activity flag");
  L2_ACQ_DL_LOCK();
  /*Locking both EUL and R99 Mutex*/
  (void)L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);

  rlc_ul_table_ptr->rlc_config_pending_var.rlc_activity_flag = NO_DATA_TRANSFERRED;
  (void)L2_ENH_ACQ_UL_LW_UNLOCK(rlc_ul_table_ptr->as_id, UE_MAX_UL_LOGICAL_CHANNEL);
  L2_ACQ_DL_UNLOCK();
}


/*===========================================================================

FUNCTION RLC_UL_NUM_ACTI_NON_SRB_AM_CH

DESCRIPTION
  Returns number of non SRB AM channels established.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlci_ul_num_acti_non_srb_am_ch(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  return rlc_ul_table_ptr->rlc_acti_non_srb_am_ch;
}

/*===========================================================================

FUNCTION L1_L2_DSM_FREE_PACKET

DESCRIPTION
  The DSM item is freed if neither RLC nor L1 is using it.  If it is in use
  with either of these app_field of DSM packet is set to RLC_DI_INUSE_MASK.

 DEPENDENCIES
   None.

 RETURN VALUE
   None.

SIDE EFFECTS
   None.

 ===========================================================================*/
void l1_l2_dsm_free_packet
(
  sys_modem_as_id_e_type as_id,
  dsm_item_type **item_ptr
)
{
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlc_pdu_dsm_type *rlc_pdu_dsm_pool_ptr   = &rlc_ul_table_ptr->ul_pdu_build_params.rlc_pdu_dsm_pool;

  if (*item_ptr == NULL)
  {
    MSG(MSG_SSID_WCDMA_RLC, MSG_LEGACY_HIGH, "PDU ptr is NULL");
  }
  else
  {
    if(rlc_pdu_dsm_pool_ptr->item_ptr== *item_ptr)
    {
      (*item_ptr)->app_field &= ~L2_DSM_INUSE_MASK;
       rlc_pdu_dsm_pool_ptr->pdu_offset= 0;
       (*item_ptr)->used = 0;
       rlc_pdu_dsm_pool_ptr->num_pdu--;
       *item_ptr = NULL;
    }
#ifdef FEATURE_DUAL_WCDMA
    else if (uplink_table[((as_id + 1) & 1)].ul_pdu_build_params.rlc_pdu_dsm_pool.item_ptr == *item_ptr)
    {
      WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "DSM_item passed with wrong sub-id %d %x",as_id,*item_ptr,0);
    }
#endif
    else
    {
      dsm_free_packet(item_ptr);
    }
  }
}

#ifdef FEATURE_SMC_SRB2_SUSPENSION
/*===========================================================================

FUNCTION  RLC_GET_SRB_UL_CIPH_STATUS

DESCRIPTION

  Response to RRC's query for ciphering status.

DEPENDENCIES
  None

RETURN VALUE
  RLC_NO_CIPHER_CFG_PENDING, no ciphering config pending.
  RLC_CIPHER_CFG_PENDING, yes, there is at least one.
  RLC_CIPHER_STATUS_UNSPECIFIED, can not find the LC.

SIDE EFFECTS
  None.

===========================================================================*/
rlc_cipher_cfg_pending_e_type rlc_get_srb_ul_ciph_status
(
  sys_modem_as_id_e_type as_id,
  rlc_lc_id_type  rlc_id,       /* RLC LC ID for SRB2                     */
  uint32          *vt_s,        /* Current VT(S) of UL SRB2               */
  uint32          *act_sn,      /* Act SN of pendinf config,if there is   */
                                /* something pending                      */ 
  uint32          *wm_size      /* WM size in terms of number of RLC PDUs */
)
{
  rlci_ul_ctrl_blk_type *ul_ctrl_blk = NULL;
  uint32 num_bytes;
  uint32 pdu_size_bytes;
  rlc_cipher_cfg_pending_e_type pending_cipher_cfg = RLC_NO_CIPHER_CFG_PENDING;
  rex_crit_sect_type *datapath_mutex = NULL;
  rlci_uplink_table_type *rlc_ul_table_ptr;

  l2_validate_as_id(as_id);

  rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);

  ul_ctrl_blk = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id);
  if (NULL != ul_ctrl_blk)
  {
    datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, rlc_id);
    /* Put RLC under config pending state. This prevent PDU building 
       interrupt to change the WM state when RLC calculates suspend 
       offset */
    /*lint -save -e655 */
    ul_ctrl_blk->lc_state |= UL_CONFIG_PENDING;
    /*lint -restore */
    L2_ACQ_UL_LW_UNLOCK(datapath_mutex);

    if (ul_ctrl_blk->cipher.cipher_mode == CIPHER_WAIT ||
        ul_ctrl_blk->cipher.cipher_mode == CIPHER_MOD)
    {
      pending_cipher_cfg = RLC_CIPHER_CFG_PENDING;
      if (ul_ctrl_blk->cipher.two_pending)
      {
        *act_sn = ul_ctrl_blk->cipher.act_sn_new1;
      }
      else
      {
        *act_sn = ul_ctrl_blk->cipher.act_sn_new;
      }
    }
    *vt_s = ul_ctrl_blk->ctrl.am_ctrl.ul_state.vt_s;
    num_bytes = RLCI_TOTAL_BYTES_UL_WM_PLUS_DEQUEUED(ul_ctrl_blk)+
                ((uint32)RLCI_TOTAL_SDUS_UL_WM_PLUS_DEQUEUED(ul_ctrl_blk)*
                 ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length) +
                (ul_ctrl_blk->special_ind ?
                 ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0) +
                (ul_ctrl_blk->octet_less ?
                 ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length : 0);
    if (NULL != ul_ctrl_blk->cur_sdu_ptr)
    {
      num_bytes += ((ul_ctrl_blk->cur_sdu_len - ul_ctrl_blk->cur_sdu_tx_len) +
                  ul_ctrl_blk->ctrl.am_ctrl.ul_state.li_length);
    }
    datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, rlc_id);
    /* Has done the suspend offset calculating. Lift the bar. */
    /*lint -e64 -e641 */
    ul_ctrl_blk->lc_state &= (~UL_CONFIG_PENDING);
    /*lint +e64 +e641 */
    L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
    
    /* the rlc_size is stored in bits. */
    pdu_size_bytes = ((ul_ctrl_blk->ctrl.am_ctrl.ul_cnfg_var->ul_rlc_size - 16) >> 3);
    *wm_size = (num_bytes / pdu_size_bytes);
    if ((num_bytes % pdu_size_bytes) > 0)
    {
      (*wm_size) += 1;
    }
  }
  else
  {
    WL2_MSG_1(ERROR, rlc_ul_table_ptr, "SRB is not config'd, %d",
                                                rlc_id);
    pending_cipher_cfg = RLC_CIPHER_STATUS_UNSPECIFIED;
  }

  return pending_cipher_cfg;
}
#endif //FEATURE_SMC_SRB2_SUSPENSION

/*===========================================================================

FUNCTION RLC_RRC_GET_LATEST_PENDING_HFN_REQ

DESCRIPTION
   This function is called by RRC to get latest pending HFN for indicated 
   logical channel ID.  
   RLC populates the info to the struct rlc_pending_hfn_req_type.
   If there is a pending configuration for UL alone then 
   dl_pending_hfn_present flag will be set to FALSE and vice-versa.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/FAILURE:  If either dl_pending_hfn_present or ul_pending_hfn_present
                    flag is set to TRUE then function will return SUCCESS.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type rlc_rrc_get_latest_pending_hfn
(
  sys_modem_as_id_e_type as_id,
  rlc_lc_id_type rlc_id,                /* queried RLC logical channel ID */
  rlc_pending_hfn_req_type *pending_hfn
)
{
  rlci_ul_ctrl_blk_type *ul_ctrl_blk = NULL;
  rlci_uplink_table_type *rlc_ul_table_ptr;
  rlci_downlink_table_type *rlc_dl_table_ptr;

  l2_validate_as_id(as_id);
  
  rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);

  /* initial HFN presenting flag. */
  pending_hfn->dl_pending_hfn_present = FALSE;
  pending_hfn->ul_pending_hfn_present = FALSE;

  ul_ctrl_blk = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id);
  if ((NULL != ul_ctrl_blk) &&
      ((ul_ctrl_blk->cipher.cipher_mode == CIPHER_WAIT) || 
       (ul_ctrl_blk->cipher.cipher_mode == CIPHER_MOD)))
  {
    pending_hfn->ul_pending_hfn = (ul_ctrl_blk->cipher.two_pending) ?
        ul_ctrl_blk->cipher.hfn_new1 : ul_ctrl_blk->cipher.hfn_new;
    pending_hfn->ul_pending_hfn_present = TRUE;
  }
  else
  {
    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Invalid RLC UL LC, %d",
                                               rlc_id);
  }

  rlci_get_dl_latest_pending_hfn(rlc_dl_table_ptr, rlc_id, pending_hfn);

  /* Either dl_pending_hfn_present or ul_pending_hfn_present
     flag is set to TRUE then function will return SUCCESS. 
   */
  if (pending_hfn->dl_pending_hfn_present || pending_hfn->ul_pending_hfn_present)
  {
    return SUCCESS;
  }
  else
  {
    return FAILURE;
  }
}

/*===========================================================================

FUNCTION RLC_RRC_OVERWRITE_HFN

DESCRIPTION
   This function is called by RRC to overwrite the HFN of RLC current using.

   This function will be called after RRC re-establish RLC to overwrite the 
   new HFN that RRC just comes up before the traffic can be resumed.

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/FAILURE:  Done successful / no such LC or no ciphering at all for
                    this LC.

SIDE EFFECTS
  None.

===========================================================================*/
uecomdef_status_e_type rlc_rrc_overwrite_hfn
(
  sys_modem_as_id_e_type as_id,
  rlc_lc_id_type rlc_id,                /* queried RLC logical channel ID */
  uint32 hfn,
  rlc_reestablish_e_type direction
)
{
  uecomdef_status_e_type ul_ret = FAILURE;
  uecomdef_status_e_type dl_ret = FAILURE;
  rlci_ul_ctrl_blk_type *ul_ctrl_blk = NULL;
  rex_crit_sect_type *datapath_mutex = NULL;
  rlci_uplink_table_type *rlc_ul_table_ptr;
  rlci_downlink_table_type *rlc_dl_table_ptr;

  l2_validate_as_id(as_id);

  rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);

  /* Perform UL AM overwrite */
  if (direction == RLC_RE_ESTABLISH_DL_UL || direction == RLC_RE_ESTABLISH_UL)
  {
    datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, rlc_id);
    ul_ctrl_blk = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, rlc_id);
    if ((NULL != ul_ctrl_blk) &&
    	(ul_ctrl_blk->lc_mode == UE_MODE_ACKNOWLEDGED) &&
    	(ul_ctrl_blk->cipher.cipher_mode == CIPHER_ON))
    {
      ul_ctrl_blk->cipher.hfn = hfn;
      ul_ctrl_blk->cipher.count = hfn << 12;
      ul_ret = SUCCESS;
    }
    L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
  }

  /* Need to overwrite the DL AM as well. */
  if (direction == RLC_RE_ESTABLISH_DL_UL || direction == RLC_RE_ESTABLISH_DL)
  {
    dl_ret = rlci_dl_overwrite_hfn(rlc_dl_table_ptr, rlc_id, hfn);
  }

  /* SUCCESS is 0; FAILURE is 1.
     Only both are FAILURE, then return FAILURE.
  */
  return (((ul_ret == FAILURE) && (dl_ret == FAILURE)) ? FAILURE : SUCCESS);
}

#ifdef FEATURE_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLC_START_UL_LOGGING

DESCRIPTION
   This function will start logging AM/UM PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_start_ul_logging (rlci_uplink_table_type *rlc_ul_table_ptr)
{
  rlc_ul_logging_var_type *rlc_ul_logging_ptr = &(rlc_ul_table_ptr->rlc_ul_logging);

   /*-------------------------------------------------------------------------
    There is data in at least one channel, and the log mask is turned on
    logging the first 4 bytes of each PDU being transmitted.
  -------------------------------------------------------------------------*/
  if(rlc_ul_logging_ptr->logging_params[RLCLOG_SIG_PDU][RLCLOG_CH_IDX] &&
                        log_status(WCDMA_UL_FIXED_AM_SIG_PDU_LOG_PACKET_V1))
  {
    rlc_ul_tx_am_sig_plane_pdu_log_pkt(rlc_ul_table_ptr,
                              rlc_ul_logging_ptr->logging_params[RLCLOG_SIG_PDU][RLCLOG_CH_IDX], FALSE);
  }
  if(rlc_ul_logging_ptr->logging_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX] &&
                       log_status(WCDMA_UL_FIXED_AM_USR_PDU_LOG_PACKET_V1))
  {
    rlc_ul_tx_am_usr_plane_pdu_log_pkt(rlc_ul_table_ptr,
                              rlc_ul_logging_ptr->logging_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX], FALSE);
  }

  rlc_ul_logging_ptr->logging_params[RLCLOG_SIG_PDU][RLCLOG_TYPE_IDX] = WCDMA_UL_FIXED_AM_SIG_PDU_LOG_PACKET_V1;
  rlc_ul_logging_ptr->logging_params[RLCLOG_USR_PDU][RLCLOG_TYPE_IDX] = WCDMA_UL_FIXED_AM_USR_PDU_LOG_PACKET_V1;
  rlc_ul_logging_ptr->logging_params[RLCLOG_SIG_PDU][RLCLOG_CH_IDX] = 0;
  rlc_ul_logging_ptr->logging_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX] = 0;

#ifdef FEATURE_UM_PDU_LOGGING
  if(rlc_ul_logging_ptr->rlc_ul_channel_cnt !=0)
  {
    rlc_ul_tx_um_pdu_log_pkt(rlc_ul_table_ptr, FALSE);
    rlc_ul_logging_ptr->rlc_ul_channel_cnt = 0;
  }
#endif

  /*-------------------------------------------------------------------------
    PDU ciphering logging is on and there is logging data.
    The logging data is filled in at the time when a PDU is built. It is
    because that the ciphering algorithm is not passed to MAC/L1. Once the
    pdu_cipher_enable flag is off, the cipher parameters stored in the
    pdu_data structure may not be the correct information.
  -------------------------------------------------------------------------*/
  if (rlc_ul_logging_ptr->rlci_ul_pdu_cipher_log.num_pdu)
  {
    rlc_ul_tx_pdu_cipher_log_pkt(rlc_ul_table_ptr, &(rlc_ul_logging_ptr->rlci_ul_pdu_cipher_log));
  }


}

/*===========================================================================

FUNCTION RLC_START_EUL_LOGGING

DESCRIPTION
   This function will start logging AM/UM PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_start_eul_logging (rlci_uplink_table_type *rlc_ul_table_ptr)
{
  rlc_ul_logging_var_type *rlc_ul_logging_ptr = &(rlc_ul_table_ptr->rlc_ul_logging);
  uint16 rlc_um_eul_channel_cnt;

  rlc_um_eul_channel_cnt = rlc_ul_logging_ptr->rlc_eul_channel_cnt;
  rlc_ul_logging_ptr->rlc_eul_channel_cnt = 0;
   /*-------------------------------------------------------------------------
    There is data in at least one channel, and the log mask is turned on
    logging the first 4 bytes of each PDU being transmitted.
  -------------------------------------------------------------------------*/
  if(rlc_ul_logging_ptr->logging_eul_params[RLCLOG_SIG_PDU][RLCLOG_CH_IDX] &&
                        log_status(WCDMA_UL_FIXED_AM_SIG_PDU_LOG_PACKET_V1))
  {
    rlc_ul_tx_am_sig_plane_pdu_log_pkt(rlc_ul_table_ptr,
                              rlc_ul_logging_ptr->logging_eul_params[RLCLOG_SIG_PDU][RLCLOG_CH_IDX], TRUE);
  }
#if defined(FEATURE_MAC_I) && defined(FEATURE_UM_PDU_LOGGING)
    if(((rlc_um_eul_channel_cnt !=0) || (rlc_ul_logging_ptr->logging_eul_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX])) &&
                         log_status(WCDMA_UL_FLEXIBLE_RLC_PDU_LOG_PACKET_V1))
  {
    rlc_enh_ul_flex_pdu_log_pkt(rlc_ul_table_ptr, rlc_ul_logging_ptr->logging_eul_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX]);
  }
#endif

  if(rlc_ul_logging_ptr->logging_eul_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX] &&
                         log_status(WCDMA_UL_FIXED_AM_USR_PDU_LOG_PACKET_V1))
  {
    rlc_ul_tx_am_usr_plane_pdu_log_pkt(rlc_ul_table_ptr,
                                 rlc_ul_logging_ptr->logging_eul_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX], TRUE);
  }

  rlc_ul_logging_ptr->logging_eul_params[RLCLOG_SIG_PDU][RLCLOG_TYPE_IDX] = WCDMA_UL_FIXED_AM_SIG_PDU_LOG_PACKET_V1;
  rlc_ul_logging_ptr->logging_eul_params[RLCLOG_USR_PDU][RLCLOG_TYPE_IDX] = WCDMA_UL_FIXED_AM_USR_PDU_LOG_PACKET_V1;
  rlc_ul_logging_ptr->logging_eul_params[RLCLOG_SIG_PDU][RLCLOG_CH_IDX] = 0;
  rlc_ul_logging_ptr->logging_eul_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX] = 0;

#ifdef FEATURE_UM_PDU_LOGGING
  if(rlc_um_eul_channel_cnt !=0)
  {
    rlc_ul_tx_um_pdu_log_pkt(rlc_ul_table_ptr, TRUE);
  }
#endif

  /*-------------------------------------------------------------------------
    PDU ciphering logging is on and there is logging data.
    The logging data is filled in at the time when a PDU is built. It is
    because that the ciphering algorithm is not passed to MAC/L1. Once the
    pdu_cipher_enable flag is off, the cipher parameters stored in the
    pdu_data structure may not be the correct information.
  -------------------------------------------------------------------------*/
  if (rlc_ul_logging_ptr->rlci_eul_pdu_cipher_log.num_pdu)
  {
    rlc_ul_tx_pdu_cipher_log_pkt(rlc_ul_table_ptr, &(rlc_ul_logging_ptr->rlci_eul_pdu_cipher_log));
  }
}
#endif /*FEATURE_RLC_QXDM_LOGGING*/

/*===========================================================================

FUNCTION CRLC_UL_CONTINUE_HANDLER

DESCRIPTION
  Continue transmission on one or more uplink unacknowledged or acknowledged
  mode channels.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void crlc_ul_continue_handler
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_continue_type *continue_ptr      /* Ptr to continue Info                */
)
{
  int   i;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
  rex_crit_sect_type *datapath_mutex = NULL;

  for (i = 0; i < continue_ptr->nchan; i++)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, continue_ptr->channel[i]);
    /*lint -save -e64 -e641 -e655 */
    if ((NULL != ctl_blk_ptr) &&
        (ctl_blk_ptr->lc_state & UL_STOP))
    {
    /*
      Before restoring the lc-state, wait till ISR completes
    */
      datapath_mutex = L2_ACQ_UL_LW_LOCK(rlc_ul_table_ptr->as_id, continue_ptr->channel[i]);
      /*lint -save -e64 -e641 -e655 */
      ctl_blk_ptr->lc_state &= (~UL_STOP);
      ctl_blk_ptr->lc_state |= UL_DATA_TRANSFER_READY;
      /*lint -restore */
      L2_ACQ_UL_LW_UNLOCK(datapath_mutex);
      
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "UL LC- %d Continue",
                                                 continue_ptr->channel[i] );
    }
    /*lint -restore */
    else
    {
      WL2_MSG_1(HIGH, rlc_ul_table_ptr, "LC %d not in Stop State",
                                                 continue_ptr->channel[i]);
    }
   }

}/* crlc_ul_continue_handler() */

/*===========================================================================

FUNCTION rlc_get_num_set

DESCRIPTION
    - This function gets the nak status indicating whether we have any naks
      pending or not.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number from where to count.
  lsn        - last sequence number till which to count.
  mod        - modulus to use..

RETURN VALUE
  number of 1's in bitmap array

SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlc_get_num_set
(
  uint16                *nak_bitmap,
  uint16                fsn,
  uint16                lsn,
  uint16                mod
)
{
  uint8             cnt = 0;
  uint16            last_sn = 0, num_one = 0;
  rlc_16bit_map_type  bitmap;
  rlc_bitmap_type   sn_bitmap;
/*------------------------------------------------------------------------*/

  if ( !RLCI_SEQ_GE( lsn, fsn, RLC_AM_MODULUS ) )
  {
    return 0xFFFF;
  }

  memset( &sn_bitmap, 0, sizeof(rlc_bitmap_type));
  memset( &bitmap, 0, sizeof(rlc_16bit_map_type));

  /*------------------------------------------------------------------------
   Loop and update status from fn to lsn..
  ------------------------------------------------------------------------*/
  do
  {
    /*----------------------------------------------------------------------
     We store pdu status as an array of 16 bit values, where each bit
     corresponds to a pdu sn. We work on one 16-bit value at a time.

     get the last sn which the 16-bit value having fsn corresponds to. 
    ----------------------------------------------------------------------*/
    cnt = RLCI_GET_AM_COUNT_STATUS_OPT_BITMAP(fsn);
    last_sn = ( fsn + cnt ) & (RLC_AM_MODULUS - 1);  

    /*----------------------------------------------------------------------
      If lsn is greater than the last sn  which the 16-bit value having
      fsn corresponds to (last_lsn), do the processing from fsn to 
      last_lsn, update fsn to point to next sn after last_lsn.
    ----------------------------------------------------------------------*/
    if ( !RLCI_SEQ_GT( lsn, last_sn, RLC_AM_MODULUS ) )
    {
      last_sn = lsn;
    }
    /*----------------------------------------------------------------------
      Get the 16 bit bitmap forsn from fsn to lsn..
      copy this to bit based structure and sum up the bits to get the number
      of 1's in the 16 bit value..
    ----------------------------------------------------------------------*/
  /*lint -e502 */
    sn_bitmap.var = RLCI_GET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, last_sn, mod );
  /*lint +e502 */

    bitmap = sn_bitmap.bit_map;

    if (sn_bitmap.var)
    {
      num_one +=( bitmap.bit0 + bitmap.bit1 + bitmap.bit2 + bitmap.bit3 +
              bitmap.bit4 + bitmap.bit5 + bitmap.bit6 + bitmap.bit7 + 
        bitmap.bit8 + bitmap.bit9 + bitmap.bit10 + bitmap.bit11 + 
        bitmap.bit12 + bitmap.bit13 + bitmap.bit14 + bitmap.bit15 );
    }

    fsn = RLCI_AM_SEQ_INC( last_sn );

  }while (last_sn != lsn);

  return num_one;
}
/*===========================================================================

FUNCTION rlc_ul_get_rtx_status

DESCRIPTION
    - This function gets the nak status from sn fsn to lsn indicating whether
      we have any retx pending or not.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  nak_bitmap - pointer to bitmap array
  fsn        - fist sequence number for which status is needed.
  lsn        - last sequence number for which status is needed.
  mod        - modulus to use..

RETURN VALUE
  rtx status - TRUE - rtx pdus are present.
               FALSE - no rtx pdus.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlc_ul_get_rtx_status
(
  uint16                *nak_bitmap,
  uint16                fsn,
  uint16                lsn,
  uint16                mod 
)
{
  uint8                 cnt = 0;
  uint16         last_sn;
/*------------------------------------------------------------------------*/

  if ( nak_bitmap == NULL )
  {
    ERR( " Bad i/p arg",0,0,0 );
    return FALSE;
  }

  if ( !RLCI_SEQ_GE( lsn, fsn, RLC_AM_MODULUS ) )
  {
    return FALSE;
  }

  /*------------------------------------------------------------------------
   Loop and update status from fn to lsn..
  ------------------------------------------------------------------------*/
  do
  {
    /*----------------------------------------------------------------------
     We store pdu status as an array of 16 bit values, where each bit
     corresponds to a pdu sn. We work on one 16-bit value at a time.
     
     get the last sn which the 16-bit value having fsn corresponds to. 
    ----------------------------------------------------------------------*/
    cnt = RLCI_GET_AM_COUNT_STATUS_OPT_BITMAP(fsn);
    last_sn = ( fsn + cnt ) & (RLC_AM_MODULUS - 1);  

    /*----------------------------------------------------------------------
      If lsn is greater than the last sn  which the 16-bit value having
      fsn corresponds to (last_lsn), update the status for pdus fron fsn to 
      last_lsn, update fsn to point to next sn after last_lsn.
    ----------------------------------------------------------------------*/
    if ( !RLCI_SEQ_GT( lsn, last_sn, RLC_AM_MODULUS ) )
    {
      last_sn = lsn;
    }

   /*----------------------------------------------------------------------
      If we have any bit set between fsn and lsn return TRUE..
    ----------------------------------------------------------------------*/  
  /*lint -e502 */
    if( RLCI_GET_AM_SN_RANGE_NAK_STATUS( nak_bitmap, fsn, last_sn, mod ) )
  /*lint +e502 */
    {
      return TRUE;
    }

    fsn = RLCI_AM_SEQ_INC( last_sn );

  }while (last_sn != lsn);

  return (FALSE);

}
/*===========================================================================

FUNCTION rlc_get_retx_bitmap

DESCRIPTION
    - This function gets the retx bitmap structure maintained on UL.

DEPENDENCIES
  None.

INPUT ARGUMENTS
  lc_id      - logical channel id.

RETURN VALUE
  retx bitmap - pointer to retx bitmap structure.

SIDE EFFECTS
  None.

===========================================================================*/
rlc_opt_status_type *rlc_get_retx_bitmap
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  byte                  lc_id  
)
{
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;
/*------------------------------------------------------------------------*/

  ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, lc_id);
  return( &ctl_blk_ptr->ctrl.am_ctrl.ul_status_opt_bitmap );
}

/*===========================================================================

FUNCTION RLC_UL_GET_UL_HDR

DESCRIPTION

  Returns the pointer to the global header buffer ul_hdr.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
rlc_ul_hdr_buff_type* rlc_get_ul_hdr(rlci_uplink_table_type *rlc_ul_table_ptr, boolean isEul)
{
  if (TRUE == isEul)
  {
    return (&(rlc_ul_table_ptr->ul_pdu_build_params.rlc_eul_hdr));
  }
  else
  {
    return (&(rlc_ul_table_ptr->ul_pdu_build_params.rlc_ul_hdr));
  }
} 

/*===========================================================================

FUNCTION RLC_UL_UPDATE_HDR

DESCRIPTION

  Stores the RLC PDU(AM/UM) header, LIs into the local buffer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_update_hdr
(
  uint8                  length,       /* 1 -> 8-bit, 2 -> 16-bit             */
  uint16                 value,        /* AM PDU/UM PDU/LI header             */
  rlc_buff_type         *hdr_list,     /* Pointer to headers                  */
  rlci_ul_ctrl_blk_type *ctrl_blk_ptr, /* To know the type of channel         */
  rlc_hdr_li_type        hdr_or_li     /* update called to store header or LI */  
#ifdef FEATURE_RLC_QXDM_LOGGING
  , boolean              log_allowed
#endif
)
{
  if (hdr_list->num_bytes > 0)
  {
    /*-----------------------------------------------------------------------
      LI is being stored. Set the E/HE bit of previous hdr before 
      updating the LI
    -----------------------------------------------------------------------*/
    hdr_list->buff[hdr_list->num_bytes - 1] |= RLC_HDR_E_MASK;
  }
  
  if (length == RLC_8_BIT_HDR)
  {
    hdr_list->buff[hdr_list->num_bytes++] = (uint8)value;
  }
  else
  {
    hdr_list->buff[hdr_list->num_bytes++] = ((value & 0xFF00) >> 8);
    hdr_list->buff[hdr_list->num_bytes++] = (value & 0x00FF);
  }

/*for logging Number of LIs per TTI */
#ifdef FEATURE_RLC_QXDM_LOGGING
  if((TRUE == log_allowed) && (RLC_PDU_LI == hdr_or_li))
  {
    if(UE_MODE_UNACKNOWLEDGED == ctrl_blk_ptr->lc_mode)
    {
      ctrl_blk_ptr->ctrl.um_ctrl.ul_chan_um_pdu.num_of_li++;
    }
    else if(UE_MODE_ACKNOWLEDGED == ctrl_blk_ptr->lc_mode)
    {
      ctrl_blk_ptr->ctrl.am_ctrl.ul_chan_am_pdu.num_of_li++;
    }
  }
#endif
} /* rlc_ul_update_hdr */


/*===========================================================================

FUNCTION RLC_DSM_PACK_DATA

DESCRIPTION

  This will pack the amount of bits from the specified byte & bit position
  in the data_ptr of the PDu

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_dsm_pack_data
(
  dsm_item_type *pdu_ptr,   /* Pointer to the PDU dsm item    */
  uint32        pack_data,  /* Data to pack                   */
  uint16        bit_pos,    /* Bit position from the 1st byte */
  uint8         data_len    /* No. of bits to pack            */
)
{
  uint8   temp      = 0xFF;
  uint8   n_bits    = 0;
  uint8   rem_bits  = data_len;
  uint16  byte_pos;

  byte_pos = (uint16)(bit_pos / 8);
  bit_pos &= 0x07;
  
  while (rem_bits > 0)
  {
    if ((bit_pos != 0)|| (rem_bits >=8))
    {
      if(rem_bits < 8)
      {
        n_bits =(8-bit_pos);
        pdu_ptr->data_ptr[byte_pos] &= ((temp << n_bits)|(temp >> (8-n_bits+rem_bits)));
        pdu_ptr->data_ptr[byte_pos] |= ((uint8)(pack_data) << (n_bits-rem_bits));
        n_bits = rem_bits;
      }
      else
      {
        n_bits = (8 - bit_pos);
        pdu_ptr->data_ptr[byte_pos] &= (temp << n_bits); 
        pdu_ptr->data_ptr[byte_pos] |= (uint8)(pack_data >> (rem_bits - n_bits));
      }
      
      bit_pos = 0;
    }
    else if (rem_bits < 8)
    {
      n_bits = rem_bits;
      pdu_ptr->data_ptr[byte_pos] &= (temp >> n_bits);
      pdu_ptr->data_ptr[byte_pos] |= (uint8)(pack_data << (8 - n_bits));
    }
    
    byte_pos++;
    rem_bits-= n_bits;    
  }

} /* rlc_dsm_pack_data */

/*===========================================================================

FUNCTION RLC_UL_FORM_IO_VEC_PER_SDU

DESCRIPTION

  Parses thru the SDU dsm chain to identify the offset with in it, and 
  forms IO vectors for the dsm items corresponding to the PDU payload

DEPENDENCIES
  None.

RETURN VALUE
  Amount of data from this SDU goes as payload to PDU.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlc_ul_form_io_vec_per_sdu
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  dsm_item_type       *dsm_ptr,          /* Start dsm_ptr of the SDU chain */
  uint16              offset,            /* Offset with the SDU chain      */
  uint16              payload_len,       /* Amount of data for the PDU     */
  l1_l2_data_type    *l1_ul_tx_buf_ptr  /* Place to store the IO vectors  */
)
{
  uint32                    len         = 0;
  uint16                    tx_data;
  
  tx_data = payload_len;
  
  /*-------------------------------------------------------------------------
    Find the dsm_item in the sdu chain, where the payload starts.
  -------------------------------------------------------------------------*/
  while ((dsm_ptr != NULL) && (offset >= dsm_ptr->used))
  {
    offset -= dsm_ptr->used;
    dsm_ptr = dsm_ptr->pkt_ptr;
  }
  
  /*-------------------------------------------------------------------------
    Construct the IO vectors for the DSM items in the same SDU, which are
    as part of this PDU payload.
  -------------------------------------------------------------------------*/
  while ((dsm_ptr != NULL) && (payload_len > 0))
  {
    len = MIN((dsm_ptr->used - offset), payload_len);
    
    if (0 == len)
    {
      WL2_MSG_3(ERROR, rlc_ul_table_ptr, "RLC_ERR: Invalid len, skip filling IOVEC, used %d offst %d pyld_len %d",
                                                  dsm_ptr->used,
                                                  offset,
                                                  payload_len);
    }
    else
    {
      /*-----------------------------------------------------------------------
        Store the IO vector for this dsm_item in __data_seg_buf[]
      -----------------------------------------------------------------------*/
      rlc_ul_fill_io_vec(rlc_ul_table_ptr, (dsm_ptr->data_ptr+offset), len, l1_ul_tx_buf_ptr, FALSE);
    }
    
    dsm_ptr = dsm_ptr->pkt_ptr;
    payload_len -= len;
    offset = 0;
  }

  return (tx_data - payload_len);
} /* rlc_ul_form_io_vec_per_sdu */

/*===========================================================================

FUNCTION RLC_UL_FORM_NON_EDCH_IO_VEC_PER_SDU

DESCRIPTION

  Parses thru the SDU dsm chain to identify the offset with in it, and 
  forms IO vectors for the dsm items corresponding to the PDU payload

DEPENDENCIES
  None.

RETURN VALUE
  Amount of data from this SDU goes as payload to PDU.

SIDE EFFECTS
  None.

===========================================================================*/
uint16 rlc_ul_form_non_edch_io_vec_per_sdu
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  dsm_item_type       *dsm_ptr,          /* Start dsm_ptr of the SDU chain */
  uint16              offset,            /* Offset with the SDU chain      */
  uint16              payload_len        /* Amount of data for the PDU     */
)
{
  uint32              len = 0;
  uint16              tx_data;
  rlc_io_vec_list_type *rlc_io_vec_list_ptr = &(rlc_ul_table_ptr->ul_pdu_build_params.rlc_io_vec_list);

  tx_data = payload_len;

  /*-------------------------------------------------------------------------
    Find the dsm_item in the sdu chain, where the payload starts.
  -------------------------------------------------------------------------*/
  while ((dsm_ptr != NULL) && (offset >= dsm_ptr->used))
  {
    offset -= dsm_ptr->used;
    dsm_ptr = dsm_ptr->pkt_ptr;
  }
  
  /*-------------------------------------------------------------------------
    Construct the IO vectors for the DSM items in the same SDU, which are
    as part of this PDU payload.
  -------------------------------------------------------------------------*/
  while ((dsm_ptr != NULL) && (payload_len > 0))
  {
    len = MIN((dsm_ptr->used - offset), payload_len);

    /*-----------------------------------------------------------------------
      Store the IO vector for this dsm_item in rlc_io_vec_list
    -----------------------------------------------------------------------*/
    rlc_io_vec_list_ptr->io_vec[rlc_io_vec_list_ptr->num_io_vecs].ptr = 
                                              dsm_ptr->data_ptr+offset;
    rlc_io_vec_list_ptr->io_vec[rlc_io_vec_list_ptr->num_io_vecs].len = len;
    rlc_io_vec_list_ptr->num_io_vecs++;
    
    dsm_ptr = dsm_ptr->pkt_ptr;
    payload_len -= len;
    offset = 0;
  }

  return (tx_data - payload_len);
} /* rlc_ul_form_non_edch_io_vec_per_sdu */

/*===========================================================================

FUNCTION RLC_CHECK_PRE_ALLOC_DSM

DESCRIPTION

  By the time RLC prepares the new PDUs, all the earlier frame PDUs need to be 
  freed up on the R99 pre-allocated pool. Assuming that, rlc_pdu_dsm_pool.num_item
  is set to zero and the next first PDU is given for R99 PDU building in the
  current TTI

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_check_pre_alloc_dsm(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  uint8 index;
  dsm_item_type *item_ptr = NULL; 
  rlc_pdu_dsm_type *rlc_pdu_dsm_pool_ptr = &(rlc_ul_table_ptr->ul_pdu_build_params.rlc_pdu_dsm_pool);
  
  if ((rlc_pdu_dsm_pool_ptr->pdu_offset != 0) ||
      ((rlc_pdu_dsm_pool_ptr->item_ptr != NULL) && 
       ((rlc_pdu_dsm_pool_ptr->item_ptr->used != 0) ||
        (0 == rlc_pdu_dsm_pool_ptr->item_ptr->references))))
  {
    WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "R99 dsm Pointer not freed pdu_offset %d, dsm_ptr %x, num_pdu", 
               rlc_pdu_dsm_pool_ptr->pdu_offset, rlc_pdu_dsm_pool_ptr->item_ptr, rlc_pdu_dsm_pool_ptr->num_pdu);

  }
  rlc_pdu_dsm_pool_ptr->num_pdu = 0;
}

/*===========================================================================

FUNCTION RLC_UL_GET_PRE_ALLOC_PDU_DSM

DESCRIPTION

  Provides the next available DSM item for the pre allocated pool for
  building the R99 PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  Pointer to the available DSM item form the pool other wise NULL pointer.

SIDE EFFECTS
  None.

===========================================================================*/
dsm_item_type* rlc_ul_get_pre_alloc_pdu_dsm
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  uint16 pdu_len_bytes, uint16 *pdu_offset
)
{
  dsm_item_type *item_ptr = NULL; 
  rlc_pdu_dsm_type *rlc_pdu_dsm_pool_ptr = &(rlc_ul_table_ptr->ul_pdu_build_params.rlc_pdu_dsm_pool);

  if ((rlc_pdu_dsm_pool_ptr->pdu_offset + pdu_len_bytes) <= 
      (rlc_pdu_dsm_pool_ptr->item_ptr->size))
  {
    item_ptr = rlc_pdu_dsm_pool_ptr->item_ptr;
    *pdu_offset = rlc_pdu_dsm_pool_ptr->pdu_offset;
    rlc_pdu_dsm_pool_ptr->pdu_offset += pdu_len_bytes;
    rlc_pdu_dsm_pool_ptr->num_pdu++;
    /*-----------------------------------------------------------------------
     Set this pre-allocated DSM item to IN_USE.
     L1 will not free this item, but turn off the IN_USE bit.
     Only UM & AM PDU built for non edch path use this logic.
     TM PDU will be freed by L1 as usually for both EDCH/NON_EDCH path.
    -----------------------------------------------------------------------*/
    item_ptr->app_field |= L2_DSM_INUSE_MASK;
    item_ptr->app_field |= L2_DSM_PRE_ALLOC_MASK;
  }

  return (item_ptr);
} /* rlc_ul_get_pre_alloc_pdu_dsm */

/*===========================================================================

FUNCTION RLC_UL_COPY_IO_VEC_DATA

DESCRIPTION

  Copies the data from the set of IO vectors into the pre allocated DSM item
  while building R99 pdus.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_copy_io_vec_data
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  dsm_item_type *item_ptr,  /* Pointer to the pre allocated DSM item       */
  uint16         offset      /* Offset with in the DSM item, from where the */
                            /* data copy should begin                      */
)
{
  uint8   i;
  uint16  length = offset;
  rlc_io_vec_list_type *rlc_io_vec_list_ptr = &(rlc_ul_table_ptr->ul_pdu_build_params.rlc_io_vec_list);
  
  for (i = 0; i < rlc_io_vec_list_ptr->num_io_vecs; i++)
  {
    /*-----------------------------------------------------------------------
     Copy the payloads stored as IO vectors into this PDU.
    -----------------------------------------------------------------------*/    
    if ( (item_ptr->size >= (item_ptr->used + rlc_io_vec_list_ptr->io_vec[i].len)) && ((item_ptr->used + rlc_io_vec_list_ptr->io_vec[i].len)>item_ptr->used) )
    {
      WCDMA_MEMCPY(item_ptr->data_ptr + length,
                   (item_ptr->size - item_ptr->used),
                   rlc_io_vec_list_ptr->io_vec[i].ptr,
                   rlc_io_vec_list_ptr->io_vec[i].len);

      item_ptr->used += rlc_io_vec_list_ptr->io_vec[i].len;
    
      length += rlc_io_vec_list_ptr->io_vec[i].len;
    }
    else
    {
      WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR: PRE ALLOC IOVEC MEM OVFLW Ptr 0x%x, Size %d Used %d",
                                        item_ptr,item_ptr->size,item_ptr->used);
    }
  }
  
} /* rlc_ul_copy_io_vec_data */

void rlc_ul_copy_io_vec_data_from_src_desc_idx
(
  rlci_uplink_table_type	*rlc_ul_table_ptr,
  dsm_item_type *item_ptr,  /* Pointer to the pre allocated DSM item       */
  uint16         offset,      /* Offset with in the DSM item, from where the */
                            /* data copy should begin                      */
  l1_l2_data_type           *l1_ul_tx_buf_ptr,
  uint16                src_desc_idx

)
{
  uint8   i;
  uint16  length = offset;
  
  for (i = src_desc_idx; i < l1_ul_tx_buf_ptr->num_src_desc; i++)
  {
    /*-----------------------------------------------------------------------
     Copy the payloads stored as IO vectors into this PDU.
    -----------------------------------------------------------------------*/
    if (item_ptr->size >= (item_ptr->used + l1_ul_tx_buf_ptr->cur_ul_buf_ptr[i].length))
    {
      WCDMA_MEMCPY(item_ptr->data_ptr + length, 
                   (item_ptr->size - item_ptr->used),
                   l1_ul_tx_buf_ptr->cur_ul_buf_ptr[i].data_seg_list_ptr,
                   l1_ul_tx_buf_ptr->cur_ul_buf_ptr[i].length);
      
      item_ptr->used += l1_ul_tx_buf_ptr->cur_ul_buf_ptr[i].length;
      
      length += l1_ul_tx_buf_ptr->cur_ul_buf_ptr[i].length;
    }
    else
    {
      WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR: MACI PRE ALLOC IOVEC MEM OVFLW Ptr 0x%x, Size %d Used %d",
                   item_ptr,item_ptr->size,item_ptr->used);
    }
  }
  
} /* rlc_ul_copy_io_vec_data */

/*===========================================================================

FUNCTION RLC_UL_INIT_IO_VEC_LIST

DESCRIPTION

  Intialising global io_vec_list before storing the set IO vector for 
  while building R99 AM new/re-tx, UM PDUs.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_init_io_vec_list(rlci_uplink_table_type *rlc_ul_table_ptr)
{
  rlc_ul_table_ptr->ul_pdu_build_params.rlc_io_vec_list.num_io_vecs = 0;
} /* rlc_ul_init_io_vec_list */

/*===========================================================================

FUNCTION RLC_UL_FILL_IO_VEC

DESCRIPTION

  Stores the IO vector into the L1 tx buffer

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_fill_io_vec
( 
  rlci_uplink_table_type *rlc_ul_table_ptr, 
  uint8             *data_ptr,          /* Source pointer                 */
  uint32            len,                /* No. of bytes                   */
  l1_l2_data_type  *l1_ul_tx_buf_ptr,  /* Buffer to place IO vector      */
  boolean           last_io_vec         /* TRUE-Last IO vector of this PDU */
                                        /* FALSE- Not the lat Io vector    */
)
{
#ifndef FEATURE_MAC_I
    if ((l1_ul_tx_buf_ptr->num_src_desc > 0) && 
        (data_ptr == 
         (l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc - 1].data_seg_list_ptr +
          l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc - 1].length)))
    {
      /*-----------------------------------------------------------------------
       Optimizing IO vectors, by check if this data is continuous with the 
       previous IO vector data.
      -----------------------------------------------------------------------*/  
      l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc - 1].length += len;
    }
    else
#endif
    {
      l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc].data_seg_list_ptr = data_ptr;
      
      /*-----------------------------------------------------------------------
       Provide length in bytes for  7200 I/F & PP-HW.
      -----------------------------------------------------------------------*/ 
      l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc].length = len;
      
      if (last_io_vec)
      {
        /*-----------------------------------------------------------------------
          Set this as last IO vectors for this PDU.
        -----------------------------------------------------------------------*/      
        SET_PDU_LAST_IO_VEC(l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc].length);
      }
      
      l1_ul_tx_buf_ptr->num_src_desc++;
    }

#ifdef FEATURE_MAC_I
    if (is_maci_debug_f3_enabled(rlc_ul_table_ptr->as_id) & DEBUG_EUL_PDU_BUILDING)
    {
      WL2_MSG_3(HIGH, rlc_ul_table_ptr, "MAC_I: num_src_desc %d, last length %d last ptr 0x%x",
                                                 l1_ul_tx_buf_ptr->num_src_desc,
                l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc - 1].length, 
                l1_ul_tx_buf_ptr->cur_ul_buf_ptr[l1_ul_tx_buf_ptr->num_src_desc - 1].data_seg_list_ptr);
    }
#endif
    
#ifdef RLC_DEBUG_PDU_LOG
    #error code not present
#endif
} /* rlc_ul_fill_io_vec */

#ifdef FEATURE_RLC_QXDM_LOGGING
/*===========================================================================

FUNCTION RLC_UL_INIT_AM_LOG_PARAMS

DESCRIPTION

 Initalizes the logging_params for the AM PDUs.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_init_am_log_params(rlc_ul_logging_var_type *rlc_ul_logging_ptr, boolean is_eul)
{
  static uint8 num_tti = 0;
  if ((num_tti % 50) == 0)
  {
    num_tti = 0;
  /*-------------------------------------------------------------------------
    This array stores the logging parameter info, the type of PDU packet
    user plane or signalling plane and no. of active channels of each
    type of packet.
  -------------------------------------------------------------------------*/
    if(TRUE == is_eul)
    {
      rlc_ul_logging_ptr->logging_eul_params[RLCLOG_SIG_PDU][RLCLOG_TYPE_IDX] = WCDMA_UL_FIXED_AM_SIG_PDU_LOG_PACKET_V1;
      rlc_ul_logging_ptr->logging_eul_params[RLCLOG_USR_PDU][RLCLOG_TYPE_IDX] = WCDMA_UL_FIXED_AM_USR_PDU_LOG_PACKET_V1;
      rlc_ul_logging_ptr->logging_eul_params[RLCLOG_SIG_PDU][RLCLOG_CH_IDX] = 0;
      rlc_ul_logging_ptr->logging_eul_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX] = 0;
    }
    else
    {  
      rlc_ul_logging_ptr->logging_params[RLCLOG_SIG_PDU][RLCLOG_TYPE_IDX] = WCDMA_UL_FIXED_AM_SIG_PDU_LOG_PACKET_V1;
      rlc_ul_logging_ptr->logging_params[RLCLOG_USR_PDU][RLCLOG_TYPE_IDX] = WCDMA_UL_FIXED_AM_USR_PDU_LOG_PACKET_V1;
      rlc_ul_logging_ptr->logging_params[RLCLOG_SIG_PDU][RLCLOG_CH_IDX] = 0;
      rlc_ul_logging_ptr->logging_params[RLCLOG_USR_PDU][RLCLOG_CH_IDX] = 0;
    }
    rlc_ul_logging_ptr->rlci_ul_pdu_cipher_log_on = log_status(WCDMA_UL_RLC_V1_PDU_CIPHER_LOG_PACKET);
  }
  num_tti++;
} /* rlc_ul_init_logging */

/*===========================================================================

FUNCTION RLC_UL_CHECK_AM_LOG_STATUS

DESCRIPTION

  Checks if the user plane/signalling plane PDU logging is turned on or not.

DEPENDENCIES
  None.

RETURN VALUE
  boolean - TRUE  if PDU needs to be logged
            FALSE otherwise.

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlc_ul_check_am_log_status
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_ul_logging_var_type *rlc_ul_logging_ptr,
  rlci_ul_am_ctrl_type    *am_ctrl_ptr, /* Pointer to ul_chan_am_pdu   */
  rlc_lc_id_type          rlc_id,       /* RLC ID                      */
  uint16                  pdu_size      /* RLC PDU SIZE                */
)
{
  uint8   log_index = 0;
  boolean found     = FALSE;
  rlc_ul_chan_am_pdu_type* ul_chan_am_pdu_ptr;

  ul_chan_am_pdu_ptr = &(am_ctrl_ptr->ul_chan_am_pdu);
  
  /*---------------------------------------------------------------
    If LC ID is for user plane AM channels, store the info if
    user plane PDU logging is turned on, otherwise store the info
    if the signaling plane PDU logging is turned on.
  ---------------------------------------------------------------*/
  if((rlc_id >= RLC_UL_AM_USR_PLANE_START_LC_ID) && 
     ((log_status(WCDMA_UL_FIXED_AM_USR_PDU_LOG_PACKET_V1))
      #ifdef FEATURE_MAC_I
      || (log_status(WCDMA_UL_FLEXIBLE_RLC_PDU_LOG_PACKET_V1))
      #endif
      ))
  {
    log_index = 1;
    rlc_ul_logging_ptr->logging_eul_params[log_index][RLCLOG_CH_IDX] ++;
  }

  if((rlc_id < RLC_UL_AM_USR_PLANE_START_LC_ID) && 
     log_status(WCDMA_UL_FIXED_AM_SIG_PDU_LOG_PACKET_V1))
  {
    log_index = 0;
    rlc_ul_logging_ptr->logging_eul_params[log_index][RLCLOG_CH_IDX] ++;
  }

  if(rlc_ul_logging_ptr->logging_eul_params[log_index][RLCLOG_CH_IDX] > 0)
  {
    found = TRUE;
  
    /*---------------------------------------------------------------
     Store pdu size for logging
    ---------------------------------------------------------------*/
    ul_chan_am_pdu_ptr->pdu_size[0] = pdu_size;
    RLC_UL_RESET_LOGGING_GLOBALS(ul_chan_am_pdu_ptr);
  }
  if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_CIPHERING)
  {
    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Cipher: LC %d, Found %d",
                                               found, rlc_id);
  }

  return found;

} /* rlc_ul_am_check_log_status */

#endif /* FEATURE_RLC_QXDM_LOGGING */

/*===========================================================================

FUNCTION RLC_ENH_INDICATE_UL_DATA_TRASH

DESCRIPTION
  L1 indicates the information about PDUs which are trashed due to Config
  related procedures. L1 discards UL data if built during reconfiguration
  to speed up the reconfig time.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
 void rlc_enh_indicate_ul_data_trash(sys_modem_as_id_e_type as_id)
 {
   uint32 idx;
   rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
       
   for (idx = RLC_UL_AM_START_LC_ID; (idx < UE_MAX_UL_LOGICAL_CHANNEL) && (rlc_ul_table_ptr->rlc_am_ctrl_pdu_built); idx++)
   {
     if ((rlc_ul_table_ptr->rlc_am_ctrl_pdu_built & (1 << (idx - RLC_UL_AM_START_LC_ID))) != 0)
     {
       /* This is an AM CONTROL PDU and getting discarded
        * Set the Need to send Control information in RLC entity */       
       rlci_rx_status_changed(rlc_ul_table_ptr, idx, TRUE);
       rlc_ul_table_ptr->rlc_am_ctrl_pdu_built &= (~(1 << (idx - RLC_UL_AM_START_LC_ID)));
       WL2_MSG_2(HIGH, rlc_ul_table_ptr, "RLC_ERR: NA: Setting Need to send Status flag - rlc_id %d rlc_am_ctrl_pdu_built %d",
                                                idx, rlc_ul_table_ptr->rlc_am_ctrl_pdu_built);
     }
   } 
 }
/*===========================================================================

FUNCTION RLC_UL_BUILD_NON_EDCH_PDU

DESCRIPTION

  Builds the R99 PDU using new io vector method of building PDUs

DEPENDENCIES
  None.

RETURN VALUE
  boolean: TRUE  - R99 pdu successfully build
           FASLE - R99 pdu building failed

SIDE EFFECTS
  None.

===========================================================================*/
boolean rlc_ul_build_non_edch_pdu
(
  rlci_uplink_table_type *rlc_ul_table_ptr,
  rlc_buff_type             *hdr_list,   /* Pointer to the headers         */
  rlc_ul_pdu_data_type      *pdu_info,   /* valid if chnl_type = NON_E_DCH */
  uint16                    padding_len  /* PDU padding length             */
  ,uint32                   key_index,   /* Ciphering key_id               */
  boolean                   enabled,     /* TRUE - cipheing applicable     */
                                         /* FALSE - otherwise              */
  uecomdef_wcdma_cipher_algo_e_type cipher_algo,
  uint32                    cnt_c,        /* COUNT_C parameter              */
  uint16                    pdu_size_bytes
)
{
  dsm_item_type *pdu_ptr  = NULL;

  /*---------------------------------------------------------------------
    This is a non E-DCH channel. Select a dsm_item from the 
    pre-allocated pdu_dsm_pool.
  ---------------------------------------------------------------------*/
  pdu_ptr = rlc_ul_get_pre_alloc_pdu_dsm(rlc_ul_table_ptr,pdu_size_bytes,&pdu_info->pdu_offset);
  if (pdu_ptr == NULL)
  {
    WL2_MSG_0(ERROR, rlc_ul_table_ptr, "RLC_ERR: Pre allocated DSM not available");
    return FALSE;
  }

  /*---------------------------------------------------------------------
   Copy the headers formed into this PDU.
  ---------------------------------------------------------------------*/
  WCDMA_MEMCPY(pdu_ptr->data_ptr+pdu_info->pdu_offset, 
               pdu_ptr->size-pdu_info->pdu_offset, 
               hdr_list->buff, 
               hdr_list->num_bytes);
  pdu_ptr->used += hdr_list->num_bytes;
  
  rlc_ul_copy_io_vec_data(rlc_ul_table_ptr, pdu_ptr, hdr_list->num_bytes + pdu_info->pdu_offset);

  pdu_ptr->used += padding_len;

  pdu_info->pdu_ptr = pdu_ptr;

  pdu_info->pdu_cipher_enable = enabled;
  pdu_info->cipher_algo = cipher_algo;
  pdu_info->cipher_key_id = key_index;
  pdu_info->count_c = cnt_c;

  return TRUE;
} /* rlc_ul_build_non_edch_pdu */

/*===========================================================================

FUNCTION RLC_UL_ENH_BUILD_PDUS

DESCRIPTION
  
  Builds uplink E-DCH PDUs for transmission in a particular TTI. In EDCH, 
  there is no TM PDUs.
  This function will be called by MAC and runs in interrupt context.

  PP.SW:

  RLC PDU payloads are provided as a set of IO vectors which will be 
  copied into the data section of the ERAM using mDM.
  
  Headers including MAC-es, TSN, SI, RLC HDR(+LIs) are stored in a header
  buffer as word format and is provided as a single IO vector. These
  are copied into the header section of ERAM using burst interface.
  
  7200A:

  Headers including MAC-es, TSN, SI, RLC HDR(+LIs), RLC PDU payloads are
  all provided as set of IO vectors. These are copied into the ERAM using
  burst interface.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rlc_ul_enh_build_pdus
(
  sys_modem_as_id_e_type as_id,
  rlc_ul_frame_format_type *ul_fmt_ptr, /* Pointer to frame format data */
  uint16 frame_number
)
{
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);
  uint16                               i;
  uint16                               num_words;
  rlci_ul_ctrl_blk_type              *ctrl_blk_ptr;
  rlc_ul_logchan_format_type         *chan_fmt_ptr;
  rlc_ul_hdr_buff_type               *ul_hdr_ptr      = rlc_get_ul_hdr(rlc_ul_table_ptr, TRUE);
  uint32                             data_sec_len_bytes;

#ifdef FEATURE_RLC_QXDM_LOGGING
  boolean found = FALSE;
  boolean log_updated = FALSE;
  rlc_ul_logging_var_type *rlc_ul_logging_ptr = &(rlc_ul_table_ptr->rlc_ul_logging);
  rlc_ul_init_am_log_params(rlc_ul_logging_ptr,TRUE);
#endif // FEATURE_RLC_QXDM_LOGGING

  ul_hdr_ptr->num_valid_words                = 0;
  ul_fmt_ptr->l1_ul_tx_buf->num_src_desc     = 0;
  ul_fmt_ptr->l1_ul_tx_buf->srb_data_present = FALSE;
  rlc_ul_logging_ptr->eul_frame_num          = frame_number;
  
  /* Write the MAC-e header into the ul_hdr */
  num_words = CONVERT_BITS_TO_WORDS(ul_fmt_ptr->mac_e_hdr_size);
  
  /* When only SI needs to be send then mac_e_hdr length would be zero */
  if (num_words != 0)
  {
    if (num_words > MAX_HDR_BUFF_SIZE)
    {
      WL2_MSG_2(ERROR, rlc_ul_table_ptr, "RLC_ERR: total mac_e_hdr_size %d - num_words %d > MAX_HDR_BUF_SIZE",
                                                  ul_fmt_ptr->mac_e_hdr_size,
                                                  num_words);
    }
    
    for (i = 0; num_words > 0; num_words--, i += 4)
    {
      if((ul_hdr_ptr->num_valid_words < MAX_HDR_BUFF_SIZE)&&(i < MAX_MAC_E_HDR_SIZE_BYTES))
      {
        ul_hdr_ptr->buff[ul_hdr_ptr->num_valid_words++] = rlc_copy_to_word(&(ul_fmt_ptr->mac_e_hdr[i]));
      }
    }
  
    /* Trigger L1 to build the Copy Engine & Cipher engine Control Word for MAC-e header */
    if(rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_PP_HW)
    {    
      WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Copy Eng->Mac_E_Hdr: hdr_len_bits %d, data_len_bits %d, offset_len_bits 0, Mac_E_Hdr 0x%x",
                                                 ul_fmt_ptr->mac_e_hdr_size,
                                                 ul_fmt_ptr->mac_e_hdr[0]);
    }
  
    mac_rlc_build_copy_engine_ctrl_info(ul_fmt_ptr->mac_e_hdr_size, 0, 0);
    mac_rlc_build_cipher_engine_ctrl_info(ul_fmt_ptr->mac_e_hdr_size, 0, 
                                         UE_WCDMA_CIPHER_ALGO_NONE, 0, 0,
                                       FALSE, FALSE, RLC_INVALID_COUNT_C);
  }

  /* Free UM SDUs transmitted in the previous TTI and initialize the queue */
  rlc_um_free_sdu_q(rlc_ul_table_ptr, RLC_E_DCH_CHNL);
  
  #ifdef FEATURE_MAC_I
  #ifdef FEATURE_WCDMA_HS_RACH
  rlc_ul_free_prev_tti_sdu_dsm_item(rlc_ul_table_ptr, &(rlc_ul_table_ptr->rlc_free_tm_sdu_q), MAX_TM_SDU_PER_TTI_EDCH);
  #endif /* FEATURE_WCDMA_HS_RACH */
  rlc_ul_free_prev_tti_mac_i_ctrl_dsm_item(rlc_ul_table_ptr);
  #endif /* FEATURE_MAC_I */
  
  /* Initialize the control PDU DSM buffer */
  rlc_ul_init_ctrl_dsm_pool(rlc_ul_table_ptr);
  
  rlc_ul_table_ptr->rlc_am_ctrl_pdu_built = 0;
  
  for (i = 0; i < ul_fmt_ptr->nchan; i++)
  {
    chan_fmt_ptr = &(ul_fmt_ptr->chan_fmt[i]);
    if(NULL == (ctrl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_rlc_id(rlc_ul_table_ptr, chan_fmt_ptr->rlc_id)))
    {
      WL2_MSG_0(ERROR, rlc_ul_table_ptr, "RLC_ERR: Ctrl_blk_ptr is Null");
      continue;
    }

    /* If the LC ID exceeds the maximum allowed channel ID, or if the LC in 
     * NULL_STATE or Uplink WM is not registered move to the next channel
     */
    if (chan_fmt_ptr->rlc_id >= UE_MAX_UL_LOGICAL_CHANNEL) 
    {
      WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_ERR: Enh Build PDU, Invalid LC %d",
                                                  chan_fmt_ptr->rlc_id);
      continue;
    }
    
    if (ctrl_blk_ptr->lc_state == UL_NULL_STATE)
    {
      WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_ERR: Enh Build PDU, LC %d in NULL_STATE %d",
                                                  chan_fmt_ptr->rlc_id);
      continue;
    }
    
    if ((ctrl_blk_ptr->lc_mode >= UE_MODE_ACKNOWLEDGED) &&
        (ctrl_blk_ptr->lc_mode <= UE_MODE_ACKNOWLEDGED_CONTROL))
    {
      #ifdef FEATURE_MAC_I
      if ((mac_hs_ul_get_pdu_type(rlc_ul_table_ptr->as_id) == MAC_E_PDU_HDR) ||
         ((mac_hs_ul_get_pdu_type(rlc_ul_table_ptr->as_id) == MAC_I_PDU_HDR) && 
         (RLC_FIXED_PDU_SIZE == ctrl_blk_ptr->ctrl.am_ctrl.ul_cnfg_var->ul_flex_pdu_size)))
      #endif
      {
        if ((chan_fmt_ptr->npdus > 0) && 
            (chan_fmt_ptr->pdu_size[0] != ctrl_blk_ptr->ctrl.am_ctrl.ul_cnfg_var->ul_rlc_size))
        {
          WRLC_ERR_FATAL(rlc_ul_table_ptr->as_id, "RLC_ERR: Size at RLC %d bits, size at MAC %d bits",
                     ctrl_blk_ptr->ctrl.am_ctrl.ul_cnfg_var->ul_rlc_size,
                     chan_fmt_ptr->pdu_size[0], 0);
        }
      }
    }
    
    if ((ctrl_blk_ptr->ul_wm_ptr == NULL) &&
        (ctrl_blk_ptr->lc_mode != UE_MODE_ACKNOWLEDGED))
    {
      WL2_MSG_1(ERROR, rlc_ul_table_ptr, "RLC_ERR: Enh Build PDU, LC %d WM unregistered",
                                                  ul_fmt_ptr->chan_fmt[i].rlc_id);
      continue;
    }
    if ((UE_LOGCHAN_DCCH == ctrl_blk_ptr->lc_type) ||
        (UE_LOGCHAN_CCCH == ctrl_blk_ptr->lc_type))
    {
      ul_fmt_ptr->l1_ul_tx_buf->srb_data_present = TRUE;
    }
    switch (ctrl_blk_ptr->lc_mode)
    {
      case UE_MODE_ACKNOWLEDGED:
        if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_PDU_BLDG)
        {
          WL2_MSG_3(HIGH, rlc_ul_table_ptr, "UL AM LC %d, Num PDUs %d, PDU size %d",
                                                     chan_fmt_ptr->rlc_id,
                                                     chan_fmt_ptr->npdus,
                                                     chan_fmt_ptr->pdu_size[0]);
        }
        
        /*-------------------------------------------------------------------
         If data PDUs are being built or going to be built on UL AM user 
         plane channels, set the activity flag to DATA_TRANSFERRED.
        -------------------------------------------------------------------*/
        if (ctrl_blk_ptr->ctrl.am_ctrl.ul_state.vt_s !=  ctrl_blk_ptr->ctrl.am_ctrl.ul_state.vt_a)
        {
          RLC_SET_DATA_ACTIVITY_FLAG(rlc_ul_table_ptr, chan_fmt_ptr->rlc_id, RLC_UL_AM_USR_PLANE_START_LC_ID);
        }
        
        #ifdef FEATURE_RLC_QXDM_LOGGING
        found = FALSE;
        if (!(ctrl_blk_ptr->ul_pdu_logging))
        {   
          found = rlc_ul_check_am_log_status(rlc_ul_table_ptr, rlc_ul_logging_ptr, &(ctrl_blk_ptr->ctrl.am_ctrl),
                                             chan_fmt_ptr->rlc_id,
                                             chan_fmt_ptr->pdu_size[0]);
          log_updated |= found;          
        }
        #endif
        
        rlc_ul_enh_build_am_pdus(rlc_ul_table_ptr, rlc_dl_table_ptr, ctrl_blk_ptr, chan_fmt_ptr, 
                                 ul_fmt_ptr->l1_ul_tx_buf
                                 #ifdef FEATURE_RLC_QXDM_LOGGING
                                 , found
                                 #endif
                                 );
      break;

      case UE_MODE_UNACKNOWLEDGED:
        
        if (rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_PDU_BLDG)
        {
          WL2_MSG_3(HIGH, rlc_ul_table_ptr, "UL UM LC %d, npdus %d, pdu_size %d",
                   chan_fmt_ptr->rlc_id,
                                                     chan_fmt_ptr->npdus,
                                                     chan_fmt_ptr->pdu_size[0]);
        }        
#if defined(FEATURE_RLC_QXDM_LOGGING) && defined(FEATURE_UM_PDU_LOGGING)
        found = FALSE;
        if (!(ctrl_blk_ptr->ul_pdu_logging) && 
             (log_status(WCMDA_UL_FIXED_UM_PDU_LOG_PACKET_V1) ||
              log_status(WCDMA_UL_FLEXIBLE_RLC_PDU_LOG_PACKET_V1)))
        {
          log_updated = TRUE;
          found = TRUE;
          rlc_ul_table_ptr->rlc_ul_logging.rlc_eul_channel_cnt++;
        }
#endif 

        rlc_ul_enh_build_um_pdus(rlc_ul_table_ptr, ctrl_blk_ptr, chan_fmt_ptr, 
                                 ul_fmt_ptr->l1_ul_tx_buf
                                 #ifdef FEATURE_RLC_QXDM_LOGGING
				 , found
                                 #endif                                 
                                 );

        /*-------------------------------------------------------------------
          If traffic is sent on user plane channels, set the activity
          flag to DATA_TRANSFERRED.
        -------------------------------------------------------------------*/
        RLC_SET_DATA_ACTIVITY_FLAG(rlc_ul_table_ptr, chan_fmt_ptr->rlc_id, 
                    RLC_UL_UM_USR_PLANE_START_LC_ID);
      break;

#ifdef FEATURE_WCDMA_HS_RACH
      case UE_MODE_TRANSPARENT:
       rlci_enh_ul_build_tm_pdus(rlc_ul_table_ptr, ctrl_blk_ptr, 
                             chan_fmt_ptr, 
                             ul_fmt_ptr->l1_ul_tx_buf);        
        break;
#endif
      default:
        WL2_MSG_1(HIGH, rlc_ul_table_ptr, "RLC_ERR: Enh Build PDU: Invalid Mode LC %d",
                                                   chan_fmt_ptr->rlc_id);

    } /* End switch */
  } /* End for */

  if (ul_fmt_ptr->si_size > 0)
  {
    /*-----------------------------------------------------------------------
      Trigger L1 to build the Copy Engine Control Word for SI field.
    -----------------------------------------------------------------------*/    
    if(rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_PP_HW)
    {    
       WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Copy Eng->SI Hdr: hdr_len_bits %d, data_len_bits %d, offset_len_bits 0, SI_Hdr 0x%x",
                                                  ul_fmt_ptr->si_size,
                                                  ul_fmt_ptr->si);
    }
      
    mac_rlc_build_copy_engine_ctrl_info(ul_fmt_ptr->si_size, 0, 0);

    /*-----------------------------------------------------------------------
      Write the SI field into the ul_hdr.
    -----------------------------------------------------------------------*/
    if (ul_hdr_ptr->num_valid_words < MAX_HDR_BUFF_SIZE)
    {
      ul_hdr_ptr->buff[ul_hdr_ptr->num_valid_words++] = ul_fmt_ptr->si;
    }
    else
    {
      WL2_MSG_3(ERROR, rlc_ul_table_ptr, "RLC_ERR: hdr buff limit reached %d, nchan %d, num_src_dec %d",
                                                  ul_hdr_ptr->num_valid_words,
                                                  ul_fmt_ptr->nchan,
                                                  ul_fmt_ptr->l1_ul_tx_buf->num_src_desc);
    }

    /*-----------------------------------------------------------------------
      Trigger L1 to build the Ciphering Engine Control Word for SI field.
      Note: MAC-e header is not ciphered.
    -----------------------------------------------------------------------*/    
    mac_rlc_build_cipher_engine_ctrl_info(ul_fmt_ptr->si_size, 0, 
                                         UE_WCDMA_CIPHER_ALGO_NONE, 0, 0,
                                         //L1_RLC_CIPHERING_ALGO_NONE, 0, 0,
                                         FALSE, FALSE, RLC_INVALID_COUNT_C);
  }

  /*-------------------------------------------------------------------------
    Trigger L1 to build the Ciphering Engine Control Word for 
    padding/last_uint_indicator.
  -------------------------------------------------------------------------*/
  if (ul_fmt_ptr->padding_size > 0)
  {
    mac_rlc_build_cipher_engine_ctrl_info(ul_fmt_ptr->padding_size, 0, 
                                       UE_WCDMA_CIPHER_ALGO_NONE, 0, 0,
                                       //L1_RLC_CIPHERING_ALGO_NONE, 0, 0,
                                       TRUE, FALSE, RLC_INVALID_COUNT_C);
  } 

  /*-------------------------------------------------------------------------
    Trigger L1 to copy the headers stored in ul_hdr into the header section
    of the ERAM bank. L1 needs the hdr section data in bytes.
  -------------------------------------------------------------------------*/
  if(rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_PP_HW)
  {
    uint16 tmp;
    WL2_MSG_1(HIGH, rlc_ul_table_ptr, "Write Hdr Sec: Total hdr bytes: %d",
                                               (ul_hdr_ptr->num_valid_words<<2));

    /* Array boundary check, the code dumps 3-bytes at a time*/
    for(tmp = 0; ((tmp < ul_hdr_ptr->num_valid_words) && ((tmp + 3) < MAX_HDR_BUFF_SIZE));
        tmp += 3)
    {
      WL2_MSG_3(HIGH, rlc_ul_table_ptr, "Write Hdr Sec: Hdr 0x%x, Hdr 0x%x, Hdr 0x%x",
                                                 ul_hdr_ptr->buff[tmp],
                                                 ul_hdr_ptr->buff[tmp+1],
               ul_hdr_ptr->buff[tmp+2]);
    }
  }

  if (ul_hdr_ptr->num_valid_words > MAX_HDR_BUFF_SIZE)
  {
    WL2_MSG_2(ERROR, rlc_ul_table_ptr, "RLC_ERR: no. of valid words %d exceeded max %d, limiting to MAX",
                                                ul_hdr_ptr->num_valid_words,
                                                MAX_HDR_BUFF_SIZE);
    ul_hdr_ptr->num_valid_words = MAX_HDR_BUFF_SIZE;
  }
  
  (void)mac_rlc_write_hdr_section((ul_hdr_ptr->num_valid_words << 2), (uint8 *)ul_hdr_ptr->buff, as_id);

  /*-------------------------------------------------------------------------
    Calculate the no. of bytes of all IO vectors.
  -------------------------------------------------------------------------*/
  for(i = 0, data_sec_len_bytes = 0; i < ul_fmt_ptr->l1_ul_tx_buf->num_src_desc; i++)
  {
    data_sec_len_bytes +=  GET_IO_VEC_LEN(ul_fmt_ptr->l1_ul_tx_buf->cur_ul_buf_ptr[i].length);
  }

  /*-------------------------------------------------------------------------
    Trigger L1 to copy the RLC Payloads provided as IO vectors into the 
    data section of ERAM bank.
  -------------------------------------------------------------------------*/
  if(rlc_ul_table_ptr->rlc_qxdm_globals.rlc_debug_var & RLC_DBG_UL_PP_HW)
  {
    WL2_MSG_2(HIGH, rlc_ul_table_ptr, "Write Data Sec: total bytes %d, total src_desc %d",
                                               data_sec_len_bytes,
                                               ul_fmt_ptr->l1_ul_tx_buf->num_src_desc);
  }
  
  (void)mac_rlc_write_data_section(data_sec_len_bytes);

#ifdef FEATURE_RLC_QXDM_LOGGING
  if (log_updated || rlc_ul_table_ptr->rlc_ul_logging.rlci_eul_pdu_cipher_log.num_pdu)
  {
    (void) rex_set_sigs(wcdma_l2_ul_get_tcb(as_id), RLC_UL_START_LOGGING_SIG);
  }
#endif /* FEATURE_RLC_QXDM_LOGGING */

} /* rlc_ul_enh_build_pdus */

/*===========================================================================

FUNCTION RLC_COPY_to_WORD

DESCRIPTION
  This function copies data from uint8 pointer into a word(uint32) byte-by-byte
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint32 rlc_copy_to_word(uint8 *src_ptr)
{
  uint32 data_word = 0xDEADDEAD;
  uint8 *word_ptr = (uint8 *) &data_word;
  
  if(src_ptr != NULL)
  {
    *(word_ptr++) = *(src_ptr++);
    *(word_ptr++) = *(src_ptr++);
    *(word_ptr++) = *(src_ptr++);
    *(word_ptr++) = *(src_ptr++);
  }
  
  return data_word;
}

/*===========================================================================

FUNCTION RLCI_GET_ACTIVE_UL_UM_CHNLS

DESCRIPTION
  This function returns the number of active UM channels 
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
uint8 rlci_get_active_ul_um_chnls (rlci_uplink_table_type *rlc_ul_table_ptr)
{
  uint8 act_ul_um_chnl = 0;
  uint32 i;
  uint8 count = rlc_ul_table_ptr->active_ch;
  rlci_ul_ctrl_blk_type *ctl_blk_ptr;

  for(i = 0; (i < UE_MAX_UL_RLC_ENTITY) && (count > 0); i++)
  {
    ctl_blk_ptr = rlci_get_ul_ctl_blk_ptr_by_index(rlc_ul_table_ptr, i);
    if ((NULL != ctl_blk_ptr) &&
    	(ctl_blk_ptr->lc_state != UL_NULL_STATE))
    {
      if(UE_MODE_UNACKNOWLEDGED == ctl_blk_ptr->lc_mode)
      {
        act_ul_um_chnl++;
      }
      count--;
    }
  }

  return act_ul_um_chnl;
}

void rlc_ul_call_data_ind_in_fach_cb(sys_modem_as_id_e_type as_id)
{
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  
  L2_MUTEX_LOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_data_ind_reg_mutex));
  
  if(rlc_ul_table_ptr->data_ind_in_fach_func_ptr != NULL)
  {   
    WL2_MSG_0(HIGH, rlc_ul_table_ptr, "DATA_IND_FACH: Calling DATA IND in FACH CB");
    rlc_ul_table_ptr->data_ind_in_fach_func_ptr(NULL, (void *)&as_id);
  }  
  
  L2_MUTEX_UNLOCK(&(rlc_mutexes[rlc_ul_table_ptr->as_id].rlc_data_ind_reg_mutex));
}


#ifdef FEATURE_QXDM_DSM_TRACING
/*===========================================================================

FUNCTION RLC_FC_GET_DSM_TRACE_CB

DESCRIPTION
  This function returns the number of active UM channels 
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void rlc_fc_get_dsm_trace_cb(
    uint32 cb_data, 
    fc_dsm_trace_info_type *wm_stat_ptr)
{
  uint16 i;
  sys_modem_as_id_e_type as_id = cb_data >> 31;  /* MSB is as_id */
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  dsm_mempool_id_type pool_id;

  cb_data &= 0x7FFFFFFF;
  
  if (wm_stat_ptr == NULL) 
  {
    WL2_MSG_0(ERROR, rlc_ul_table_ptr , "RLC_ERR::NULL wm statistic ptr");
    return;
  }
  if( (cb_data != 0x5EADBEEF ) ||
      (wm_stat_ptr->tech_id != DSM_UMTS_TECH) ||
      (wm_stat_ptr->client_id != UMTS_RLC_CLIENT ))
  {
    WL2_MSG_3(ERROR, rlc_ul_table_ptr , "RLC_ERR::Incorrect CB params %x %d %d",
              cb_data,
              wm_stat_ptr->tech_id,
              wm_stat_ptr->client_id);
    return;
  }
  wm_stat_ptr->num_pools = FC_MAX_POOL_ID;


  for( i=0; i<FC_MAX_POOL_ID; i++)
  {
    switch(i)
    {
    case FC_LARGE_POOL:
      pool_id = DSM_DS_LARGE_ITEM_POOL;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.few_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_FEW) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.many_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_MANY) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.dne_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_LINK_LAYER_DNE) ;
      break;
    case FC_SMALL_POOL:
      pool_id = DSM_DS_SMALL_ITEM_POOL;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.few_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_FEW) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.many_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_MANY) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.dne_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_LINK_LAYER_DNE) ;
      break;
      case FC_DUP_POOL:
      pool_id = DSM_DUP_ITEM_POOL;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.few_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_FEW) ;
      wm_stat_ptr->dsm_counts[i].dsm_threshold_stats.many_threshold
        = DSM_POOL_MEM_EVENT_LEVEL(pool_id, DSM_MEM_LEVEL_RLC_MANY) ;
      break;
    default:
      WL2_MSG_0(HIGH, rlc_ul_table_ptr, "RLC_ERR::Shouldnt happen");
      return;
    }
    wm_stat_ptr->dsm_counts[i].dsm_pool = (fc_dsm_pool_id_type)i;
    dsm_get_pool_stats(pool_id,
                       &(wm_stat_ptr->dsm_counts[i].dsm_pool_counts));
    WCDMA_MEMCPY(&(wm_stat_ptr->dsm_counts[i].dsm_event_hits),
                 sizeof(fc_dsm_event_hits_stat_type),
                 &(rlc_ul_table_ptr->rlc_dsm_trace_params.rlc_fc_dsm_event_hits[i]),
                 sizeof(fc_dsm_event_hits_stat_type));
    memset(&(rlc_ul_table_ptr->rlc_dsm_trace_params.rlc_fc_dsm_event_hits[i]), 0,
           sizeof(fc_dsm_event_hits_stat_type));
  }
}
#endif /*FEATURE_QXDM_DSM_TRACING*/

#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION     rlc_update_ul_am_info_qsh

DESCRIPTION  Updates QSH Channel Info for UL AM Logical Channel
===========================================================================*/
void rlc_update_ul_am_info_qsh( wrlc_qsh_ul_am_ctrl_type_s* qsh_am_ctrl_ptr, 
                                rlci_ul_am_ctrl_type*       am_ctrl_blk_ptr)
{
  qsh_am_ctrl_ptr->ul_state.vt_s    = am_ctrl_blk_ptr->ul_state.vt_s;
  qsh_am_ctrl_ptr->ul_state.vt_a    = am_ctrl_blk_ptr->ul_state.vt_a;
  qsh_am_ctrl_ptr->ul_state.vt_wins = am_ctrl_blk_ptr->ul_state.vt_wins;
  
  qsh_am_ctrl_ptr->ul_cnfg_var.ul_rlc_size          = am_ctrl_blk_ptr->ul_cnfg_var->ul_rlc_size;
  qsh_am_ctrl_ptr->ul_cnfg_var.ul_li_size           = am_ctrl_blk_ptr->ul_state.li_length;
  qsh_am_ctrl_ptr->ul_cnfg_var.ul_flex_min_pdu_size = am_ctrl_blk_ptr->ul_cnfg_var->ul_flex_min_pdu_size;
  qsh_am_ctrl_ptr->ul_cnfg_var.ul_flex_max_pdu_size = am_ctrl_blk_ptr->ul_cnfg_var->ul_flex_max_pdu_size;
}


/*===========================================================================
FUNCTION     rlc_update_ul_um_info_qsh

DESCRIPTION  Updates QSH Channel Info for UL UM Logical Channel
===========================================================================*/
void rlc_update_ul_um_info_qsh( wrlc_qsh_ul_um_ctrl_type_s* qsh_um_ctrl_ptr, 
                                rlci_ul_um_ctrl_type*       um_ctrl_blk_ptr)
{  
  qsh_um_ctrl_ptr->vt_us     = um_ctrl_blk_ptr->vt_us;
  qsh_um_ctrl_ptr->li_length = um_ctrl_blk_ptr->li_length;
}


/*===========================================================================
FUNCTION     rlc_update_dl_am_info_qsh

DESCRIPTION  Updates QSH Channel Info for DL AM Logical Channel
===========================================================================*/
void rlc_update_dl_am_info_qsh( wrlc_qsh_dl_am_ctrl_type_s* qsh_dl_am_ctrl_ptr, 
                                rlci_dl_am_ctrl_type*       dl_am_ctrl_blk_ptr)
{
  qsh_dl_am_ctrl_ptr->dl_state_var.old_vr_r   = dl_am_ctrl_blk_ptr->dl_state_var.old_vr_r;
  qsh_dl_am_ctrl_ptr->dl_state_var.prev_vr_h  = dl_am_ctrl_blk_ptr->dl_state_var.prev_vr_h;
  qsh_dl_am_ctrl_ptr->dl_state_var.vr_r       = dl_am_ctrl_blk_ptr->dl_state_var.vr_r;
  qsh_dl_am_ctrl_ptr->dl_state_var.vr_h       = dl_am_ctrl_blk_ptr->dl_state_var.vr_h;
  qsh_dl_am_ctrl_ptr->dl_state_var.li_length  = dl_am_ctrl_blk_ptr->dl_state_var.li_length;
  qsh_dl_am_ctrl_ptr->dl_state_var.last_sn_processed = dl_am_ctrl_blk_ptr->dl_state_var.last_sn_processed;
  
  qsh_dl_am_ctrl_ptr->dl_ctl_cnfg.dl_rlc_size = dl_am_ctrl_blk_ptr->dl_ctl_cnfg->dl_rlc_size;
  qsh_dl_am_ctrl_ptr->dl_ctl_cnfg.am_li_type  = dl_am_ctrl_blk_ptr->dl_ctl_cnfg->am_li_type;
  qsh_dl_am_ctrl_ptr->dl_ctl_cnfg.am_pdu_size_flexbile = dl_am_ctrl_blk_ptr->dl_ctl_cnfg->am_pdu_size_flexbile;
}


/*===========================================================================
FUNCTION     rlc_update_dl_um_info_qsh

DESCRIPTION  Updates QSH Channel Info for DL UM Logical Channel
===========================================================================*/
void rlc_update_dl_um_info_qsh( wrlc_qsh_dl_um_ctrl_type_s* qsh_dl_um_ctrl_ptr, 
                                rlci_dl_um_ctrl_type*       dl_um_ctrl_blk_ptr)
{
  qsh_dl_um_ctrl_ptr->vr_ur     = dl_um_ctrl_blk_ptr->vr_ur;
  qsh_dl_um_ctrl_ptr->li_length = dl_um_ctrl_blk_ptr->li_length;
}


/*===========================================================================
FUNCTION     rlc_populate_QSH_dumps

DESCRIPTION  Collects the QSH Dumps for RLC, and updates it to the location
             passed in as argument from QSH
===========================================================================*/
void rlc_populate_QSH_dumps (uint8 as_id, void* qsh_dump_ptr)
{
  uint8 ctrl_blk_id, dl_ctrl_blk_id, ul_active_channels, dl_active_channels, index;
  rlci_ul_ctrl_blk_type* ul_ctrl_blk_ptr;
  rlci_dl_ctrl_blk_type* dl_ctrl_blk_ptr;
  wrlc_qsh_ul_ctrl_blk_type_s* rlc_qsh_ul_ctrl_blk_ptr;
  wrlc_qsh_dl_ctrl_blk_type_s* rlc_qsh_dl_ctrl_blk_ptr;
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);
  
  wrlc_qsh_dump_tag_ul_dl_s* rlc_qsh_dump_tag_ptr = (wrlc_qsh_dump_tag_ul_dl_s*)qsh_dump_ptr;
   
  wrlc_qsh_ul_table_type_s* rlc_ul_qsh_ptr = &rlc_qsh_dump_tag_ptr->wcdma_rlc_ul;
  wrlc_qsh_dl_table_type_s* rlc_dl_qsh_ptr = &rlc_qsh_dump_tag_ptr->wcdma_rlc_dl;
  
  
  /* Populate UL Dump Structure */
  
  ul_active_channels = rlc_ul_table_ptr->active_ch;
  index = 0;
  rlc_qsh_dump_tag_ptr->as_id = as_id;
  rlc_ul_qsh_ptr->active_ch =0;
  rlc_dl_qsh_ptr->active_ch =0;
  
  while ( ( ul_active_channels > 0) && 
          ( rlc_ul_qsh_ptr->active_ch < UE_MAX_RLC_ENTITIES_REDUCED_LOG ) )
  {
    /*-----------------------------------------------------------------------
     Check if the channel is in NULL state. If not, update relevant
     information to the QSH structure
    -----------------------------------------------------------------------*/
    ctrl_blk_id = rlc_ul_table_ptr->ul_ctl_blk_id_tbl[index];
    if ((ctrl_blk_id != INIT_CTL_BLK_ID) && (ctrl_blk_id < UE_MAX_UL_RLC_ENTITY))
    {
      ul_ctrl_blk_ptr = &rlc_ul_table_ptr->ctrl_blk[ctrl_blk_id];
      
      if (ul_ctrl_blk_ptr->lc_state != UL_NULL_STATE)
      {
        if (ul_ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
        {
          if(rlc_ul_qsh_ptr->active_ch < UE_MAX_RLC_ENTITIES_REDUCED_LOG)
          {
            rlc_qsh_ul_ctrl_blk_ptr = &rlc_ul_qsh_ptr->ctrl_blk[rlc_ul_qsh_ptr->active_ch];
            rlc_qsh_ul_ctrl_blk_ptr->lc_state = (wrlc_qsh_ul_state_e)ul_ctrl_blk_ptr->lc_state;
            rlc_qsh_ul_ctrl_blk_ptr->rlc_id = index;
            rlc_update_ul_am_info_qsh( &rlc_qsh_ul_ctrl_blk_ptr->ctrl.am_ctrl, 
                                       &ul_ctrl_blk_ptr->ctrl.am_ctrl );
            
            rlc_ul_qsh_ptr->active_ch++;
            
          }
        }
        else if (ul_ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED)
        {
          if(rlc_ul_qsh_ptr->active_ch < UE_MAX_RLC_ENTITIES_REDUCED_LOG)
          {
            rlc_qsh_ul_ctrl_blk_ptr = &rlc_ul_qsh_ptr->ctrl_blk[rlc_ul_qsh_ptr->active_ch];
            rlc_qsh_ul_ctrl_blk_ptr->lc_state = (wrlc_qsh_ul_state_e)ul_ctrl_blk_ptr->lc_state;
            rlc_qsh_ul_ctrl_blk_ptr->rlc_id = index;

            rlc_update_ul_um_info_qsh( &rlc_qsh_ul_ctrl_blk_ptr->ctrl.um_ctrl,
                                       &ul_ctrl_blk_ptr->ctrl.um_ctrl );
            
            rlc_ul_qsh_ptr->active_ch++;
          }
        }
        else if (ul_ctrl_blk_ptr->lc_mode == UE_MODE_TRANSPARENT)
        {
          rlc_ul_qsh_ptr->num_act_tm_ch++;
        }

        /* Decrement the number of active channels remaining to check for */
        ul_active_channels--;
      }
    }

    /* Increment the index to check the next entry in Uplink Table */
    index++;
  }


  /* Populate DL Dump Structure */
  
  dl_active_channels = rlc_dl_table_ptr->active_ch;
  index = 0;
  
  while ( ( dl_active_channels > 0) && 
          ( rlc_dl_qsh_ptr->active_ch < UE_MAX_RLC_ENTITIES_REDUCED_LOG ) )
  {
    /*-----------------------------------------------------------------------
     Check if the channel is in NULL state. If not, update relevant
     information to the QSH structure
    -----------------------------------------------------------------------*/
    ctrl_blk_id = rlc_dl_table_ptr->dl_ctl_blk_id_tbl[index];
    if ((ctrl_blk_id != INIT_CTL_BLK_ID) && (ctrl_blk_id < UE_MAX_DL_RLC_ENTITY))
    {
      dl_ctrl_blk_ptr = &rlc_dl_table_ptr->ctrl_blk[ctrl_blk_id];
      
      if (dl_ctrl_blk_ptr->lc_state != DL_NULL_STATE)
      {
        if (dl_ctrl_blk_ptr->lc_mode == UE_MODE_ACKNOWLEDGED)
        {
          if(rlc_dl_qsh_ptr->active_ch < UE_MAX_RLC_ENTITIES_REDUCED_LOG)
          {
            rlc_qsh_dl_ctrl_blk_ptr = &rlc_dl_qsh_ptr->ctrl_blk[rlc_dl_qsh_ptr->active_ch];
            rlc_qsh_dl_ctrl_blk_ptr->lc_state = (wrlc_qsh_dl_state_e)dl_ctrl_blk_ptr->lc_state;
            rlc_qsh_dl_ctrl_blk_ptr->rlc_id = index;
            rlc_update_dl_am_info_qsh( &rlc_qsh_dl_ctrl_blk_ptr->ctrl.am_ctrl, 
                                       &dl_ctrl_blk_ptr->ctrl.am_ctrl );
            
            rlc_dl_qsh_ptr->active_ch++;
            
          }
        }
        else if (dl_ctrl_blk_ptr->lc_mode == UE_MODE_UNACKNOWLEDGED)
        {
          if(rlc_dl_qsh_ptr->active_ch < UE_MAX_RLC_ENTITIES_REDUCED_LOG)
          {
            rlc_qsh_dl_ctrl_blk_ptr = &rlc_dl_qsh_ptr->ctrl_blk[rlc_dl_qsh_ptr->active_ch];
            rlc_qsh_dl_ctrl_blk_ptr->lc_state = (wrlc_qsh_dl_state_e)dl_ctrl_blk_ptr->lc_state;
            rlc_qsh_dl_ctrl_blk_ptr->rlc_id = index;            
            rlc_update_dl_um_info_qsh( &rlc_qsh_dl_ctrl_blk_ptr->ctrl.um_ctrl,
                                       &dl_ctrl_blk_ptr->ctrl.um_ctrl );
            
            rlc_dl_qsh_ptr->active_ch++;
          }
        }
        else if (dl_ctrl_blk_ptr->lc_mode == UE_MODE_TRANSPARENT)
        {
          rlc_dl_qsh_ptr->num_act_tm_ch++;
        }
        
        /* Decrement the number of active channels remaining to check for */
        dl_active_channels--;
      }
    }
    
    /* Increment the index to check the next entry in Downlink Table */
    index++;
  }
}


/*===========================================================================
FUNCTION     wrlc_qsh_mini_dump_collect

DESCRIPTION  Collects the Mini Dumps for RLC, and updates it to the location
             passed in as argument from QSH
===========================================================================*/
void wrlc_qsh_mini_dump_collect(qsh_client_cb_params_s* qsh_cb_params_ptr)
{
  uint8 sub_idx;
  uint32 tag_size = 0;
  uint8* off_ptr;
  uint32  remaining_size = qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.size_bytes;
  qsh_client_action_done_s cb_done;

  off_ptr = (uint8*) qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.addr;
  tag_size = sizeof(wrlc_qsh_dump_tag_ul_dl_s);

  for (sub_idx = L2_SUB_1; sub_idx < L2_NUM_W_SUBS ; sub_idx++)
  {
    if ((off_ptr != NULL) && (0!= qsh_cb_params_ptr->action_params.dump_collect.dump_tag_mask))
    {
      if((remaining_size) >= tag_size)
      {
        qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)off_ptr, WRLC_QSH_DUMP_TAG_WCDMA_RLC_E , tag_size );
                                     //(tag_size - sizeof(qsh_dump_tag_hdr_s)));
        rlc_populate_QSH_dumps(sub_idx,off_ptr);
      }
      else
      {
        WL2_MSG_3(HIGH, RLCI_GET_UL_TBL_PTR(sub_idx), "wrlc_qsh_cb: insufficient memory - %d bytes for dump collection for RLC QSH Dumps of size %d bytes", remaining_size, tag_size, 0);
      }
    }
    off_ptr += tag_size;
    remaining_size -= tag_size;
  }
  qsh_client_action_done_init(&cb_done);
  cb_done.cb_params_ptr = qsh_cb_params_ptr;
  cb_done.params.dump_collect.size_written_bytes = tag_size;
  cb_done.action_mode_done= QSH_ACTION_MODE_DONE_SYNC;
  qsh_client_action_done(&cb_done);
}
#endif /*FEATURE_WCDMA_QSH_DUMP*/

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*===========================================================================
FUNCTION     wrlc_qsh_report_event

DESCRIPTION  Reports RLC events to QSH
===========================================================================*/
void wrlc_qsh_report_event(wrlc_qsh_event_e event_id, sys_modem_as_id_e_type as_id)
{
  qsh_client_event_notify_params_s  event_notify_params;
  sys_modem_as_id_e_type sub_idx;

  sub_idx = wcdma_convert_stack_index_to_subid(as_id);  

  WL2_MSG_1(HIGH, RLCI_GET_UL_TBL_PTR(as_id), "RLC_QSH: Event %d reported to QSH", event_id);
  qsh_client_event_notify_init(&event_notify_params);
  event_notify_params.client = QSH_CLT_WRLC;
  event_notify_params.id = event_id;
  event_notify_params.event_data = NULL;
  event_notify_params.subs_id = sub_idx;
  qsh_client_event_notify(&event_notify_params);
}
#endif

#if defined(FEATURE_WCDMA_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)
/*===========================================================================
FUNCTION     wrlc_qsh_send_cnf

DESCRIPTION  Seds confirmation to QSH, based on whether command was processed in task context or QSH context
===========================================================================*/
void wrlc_qsh_send_cnf (qsh_client_cb_params_s *wrlc_qsh_cfg)
{
  qsh_client_action_done_s cb_done;
  qsh_client_action_done_init(&cb_done);
  cb_done.cb_params_ptr = wrlc_qsh_cfg;
  if(wrlc_qsh_cfg->action_mode == QSH_ACTION_MODE_SYNC_OPTIONAL)
  {
    cb_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
  }
  qsh_client_action_done(&cb_done);
}

/*===========================================================================
FUNCTION     rlc_populate_QSH_metrics

DESCRIPTION  Populates the QSH Metrics for RLC
===========================================================================*/
boolean rlc_populate_QSH_metrics (uint16 time_elapsed_ms,sys_modem_as_id_e_type as_id)
{
  rlc_qsh_metric_data_type* rlc_qsh_metric_global_ptr = &rlc_qsh_metric_global[as_id];
  wrlc_qsh_metric_data_statistics_s* rlc_qsh_metric_ptr = 
                                     (wrlc_qsh_metric_data_statistics_s*)rlc_qsh_metric_global_ptr->metric_addr;
  rlci_uplink_table_type *rlc_ul_table_ptr = RLCI_GET_UL_TBL_PTR(as_id);
  rlci_downlink_table_type *rlc_dl_table_ptr = RLCI_GET_DL_TBL_PTR(as_id);
  
  boolean ret_val = FALSE;

  if (rlc_ul_table_ptr->active_ch || rlc_dl_table_ptr->active_ch)
  {
    if ( time_elapsed_ms )
    {
      rlc_qsh_metric_global_ptr->rlc_ul_tput = (uint32)( (rlc_qsh_metric_global_ptr->num_pdu_bytes_tx*8)/time_elapsed_ms );
      rlc_qsh_metric_global_ptr->rlc_dl_tput = (uint32)( (rlc_qsh_metric_global_ptr->rlc_dl_tput*8)/time_elapsed_ms );
    }
    rlc_qsh_metric_ptr->num_rlc_rst_dl     = rlc_qsh_metric_global_ptr->num_rlc_rst_dl;
    rlc_qsh_metric_ptr->num_rlc_rst_ul     = rlc_qsh_metric_global_ptr->num_rlc_rst_ul;
    rlc_qsh_metric_ptr->rlc_fc_win_size    = rlc_qsh_metric_global_ptr->rlc_fc_win_size;
    rlc_qsh_metric_ptr->num_rlc_urecov_err = rlc_qsh_metric_global_ptr->num_rlc_urecov_err;
    rlc_qsh_metric_ptr->num_pdu_bytes_tx   = rlc_qsh_metric_global_ptr->num_pdu_bytes_tx;
    rlc_qsh_metric_ptr->num_pdu_bytes_retx = rlc_qsh_metric_global_ptr->num_pdu_bytes_retx;
    rlc_qsh_metric_ptr->rlc_dl_tput        = rlc_qsh_metric_global_ptr->rlc_dl_tput;
    rlc_qsh_metric_ptr->rlc_ul_tput        = rlc_qsh_metric_global_ptr->rlc_ul_tput;
    
    rlc_qsh_metric_global_ptr->num_rlc_rst_dl =0;
    rlc_qsh_metric_global_ptr->num_rlc_rst_ul=0;
    rlc_qsh_metric_global_ptr->num_rlc_urecov_err=0;
    rlc_qsh_metric_global_ptr->num_pdu_bytes_tx=0;
    rlc_qsh_metric_global_ptr->num_pdu_bytes_retx=0;
    rlc_qsh_metric_global_ptr->rlc_dl_tput=0;
    rlc_qsh_metric_global_ptr->rlc_ul_tput=0;
    ret_val = TRUE;
  }
  
  return ret_val;
}


/*===========================================================================
FUNCTION     wrlc_qsh_proc_metric_command

DESCRIPTION  Validates and Processes the RLC Metric Command from QSH
===========================================================================*/
void wrlc_qsh_proc_metric_command(qsh_client_cb_params_s* wrlc_qsh_metric_cfg,sys_modem_as_id_e_type as_id)
{
  uint32 metric_idx = 0;
  boolean commit = FALSE;
  void* next_addr = NULL;
  qsh_client_action_done_s cb_done;
  qsh_client_metric_log_done_s metric_log_done;
  rlc_qsh_metric_data_type *rlc_qsh_metric_ptr = NULL;

  rlc_qsh_metric_ptr = &rlc_qsh_metric_global[as_id];

  if(wrlc_qsh_metric_cfg->action == QSH_ACTION_METRIC_CFG)
  {
    metric_idx= wrlc_qsh_metric_cfg->action_params.metric_cfg.id;
    if(metric_idx < WRLC_QSH_METRIC_MAX_STAT_E)
    {
      if(wrlc_qsh_metric_cfg->action_params.metric_cfg.action == QSH_METRIC_ACTION_START)
      {
        memset(rlc_qsh_metric_ptr,0,sizeof(rlc_qsh_metric_data_type));
        rlc_qsh_metric_ptr->metric_context_id = wrlc_qsh_metric_cfg->action_params.metric_cfg.metric_context_id;
        rlc_qsh_metric_ptr->metric_addr = wrlc_qsh_metric_cfg->action_params.metric_cfg.start_addr;
      }
      else if(wrlc_qsh_metric_cfg->action_params.metric_cfg.action == QSH_METRIC_ACTION_STOP)
      {
        rlc_qsh_metric_ptr->metric_addr = NULL;
      }
    }
    else
    {
      WL2_MSG_1(HIGH, RLCI_GET_UL_TBL_PTR(as_id), "wrlc_qsh_cb: invalid metric id %d passed from QSH", metric_idx);
    }
  }
  else if(wrlc_qsh_metric_cfg->action == QSH_ACTION_METRIC_TIMER_EXPIRY)
  {
    if(rlc_qsh_metric_ptr->metric_addr == NULL)
    {
      WL2_MSG_1(HIGH, RLCI_GET_UL_TBL_PTR(as_id),"wrlc_qsh_cb: metric id %d metric_addr is NULL", metric_idx);
    }
    else
    {
      commit = rlc_populate_QSH_metrics(wrlc_qsh_metric_cfg->action_params.metric_timer_expiry.params[0].time_elapsed_ms,as_id);
  
      if(commit)
      {
        qsh_client_metric_log_done_init(&metric_log_done);
        metric_log_done.client = QSH_CLT_WRLC;
        metric_log_done.metric_id = WRLC_QSH_METRIC_RLC_STAT_E;
        metric_log_done.metric_context_id = rlc_qsh_metric_ptr->metric_context_id;
        metric_log_done.log_reason = QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY;
        
        next_addr = qsh_client_metric_log_done(&metric_log_done);
        if(next_addr == NULL)
        {
          WRLC_ERR_FATAL(as_id, "wrlc_qsh_cb_process: next address returned for metric id %d is NULL", metric_idx, 0, 0);
        }
        rlc_qsh_metric_ptr->metric_addr = next_addr;
      }
    } 
  }

  wrlc_qsh_send_cnf(wrlc_qsh_metric_cfg);  
}

/*===========================================================================
FUNCTION     wrlc_qsh_direct_command

DESCRIPTION  Directs execution to appropriate command handler
===========================================================================*/
void wrlc_qsh_direct_command(qsh_client_cb_params_s *wrlc_qsh_cfg_ptr, sys_modem_as_id_e_type as_id)
{
  switch(wrlc_qsh_cfg_ptr->action)
  {
    case QSH_ACTION_METRIC_CFG:
    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      wrlc_qsh_proc_metric_command(wrlc_qsh_cfg_ptr, as_id);
      break;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    case QSH_ACTION_EVENT_CFG:
      wrlc_qsh_send_cnf(wrlc_qsh_cfg_ptr);
      break;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif

    default:
      WL2_MSG_1(ERROR, RLCI_GET_UL_TBL_PTR(as_id), "RLC_QSH: Unexpected action %d", wrlc_qsh_cfg_ptr->action);
      break;
  }
}

/*===========================================================================
FUNCTION     wrlc_process_qsh_commands

DESCRIPTION  Calls the appropriate QSH action handler/enqueues into appropriate task

       as_id =  SYS_MODEM_AS_ID_NONE - Process for all active subs
       else, process for as_id passed
===========================================================================*/
void wrlc_process_qsh_commands(qsh_client_cb_params_s* qsh_cb_params_ptr, sys_modem_as_id_e_type as_id)
{
  uint8 sub_idx;
  wcdma_l2_ul_cmd_type *rlc_ul_l2_cmd_ptr =NULL;
  
  for (sub_idx = L2_SUB_1; sub_idx < L2_NUM_W_SUBS; sub_idx++)
  {
    if ((as_id == SYS_MODEM_AS_ID_NONE) ||
    	(as_id == sub_idx))
    {
      switch(qsh_cb_params_ptr->action_mode)
      {
        case QSH_ACTION_MODE_SYNC_OPTIONAL:
        {
          rlc_ul_l2_cmd_ptr = wcdma_l2_ul_get_cmd_buf();
          if (rlc_ul_l2_cmd_ptr != NULL)
          {
            rlc_ul_l2_cmd_ptr->cmd_hdr.as_id = sub_idx;
            rlc_ul_l2_cmd_ptr->cmd_hdr.cmd_id = CRLC_QSH_PROCESS_COMMAND_REQ;
            WCDMA_MEMCPY(&rlc_ul_l2_cmd_ptr->cmd_data.wmac_rlc_qsh_cfg,
                         sizeof(qsh_client_cb_params_s),qsh_cb_params_ptr,
                         sizeof(qsh_client_cb_params_s));
            wcdma_l2_ul_put_cmd(rlc_ul_l2_cmd_ptr);
          }
          else
          {
            WL2_MSG_0(HIGH, RLCI_GET_UL_TBL_PTR(sub_idx),"RLC_ERR: Unable to allocate L2 cmd buffer");
          }
        }
        break;
  	  
        case QSH_ACTION_MODE_SYNC_REQUIRED:
          wrlc_qsh_direct_command(qsh_cb_params_ptr, sub_idx);
          break;
  
        default:
          break;
      }
    }
  }
}
#endif

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif
void wrlc_qsh_mdump_set_rlc(sys_modem_as_id_e_type as_id)
{
  #ifdef FEATURE_QSH_MDUMP
  qsh_mdump_set_crash_client(QSH_CLT_WRLC,QSH_CLT_WRLC); 
  qsh_mdump_set_crash_subs_id(QSH_CLT_WRLC,as_id); 
  qsh_mdump_set_crash_rat(QSH_CLT_WRLC,SYS_SYS_MODE_WCDMA);
  qsh_mdump_set_crash_category(QSH_CLT_WRLC,QSH_MDUMP_CRASH_CAT_UNKNOWN);
  #endif
}

#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR

void wrlc_qsh_mdump_collect(sys_modem_as_id_e_type as_id, qsh_client_cb_params_s* qsh_cb_params_ptr)
{
  qsh_client_action_done_s cb_done;
  int i = 0;

  qsh_mdump_collect_high((void*)&(uplink_table[as_id].active_ch), sizeof(uint8));
  for(i=0; i< uplink_table[as_id].active_ch; i++)
  {
   qsh_mdump_collect_high((void*)&(uplink_table[as_id].ctrl_blk[i]), sizeof(rlci_ul_ctrl_blk_type));
  }

  for(i=0; i<4; i++)
  {
    if(uplink_table[as_id].rlc_flex_retx_pdu_info[i] != NULL)
    {
      qsh_mdump_collect_high((void*)(uplink_table[as_id].rlc_flex_retx_pdu_info[i]), sizeof(rlc_flex_retx_pdu_info_type));
    }
  }
  qsh_mdump_collect_high((void*)&(uplink_table[as_id].is_tuneaway), sizeof(boolean));
  qsh_mdump_collect_high((void*)&(uplink_table[as_id].rlc_ulc_params), sizeof(rlc_ulc_params_type));
  qsh_mdump_collect_high((void*)&(uplink_table[as_id].ul_compr_ctrl_pdu_dsm_pool), sizeof(rlc_am_ul_comp_dsm_type));
  qsh_mdump_collect_high((void*)&(uplink_table[as_id].num_dsm_used_in_this_tti), sizeof(uint32));
  qsh_mdump_collect_high((void*)&(uplink_table[as_id].rlc_config_pending_var), sizeof(uplink_table[as_id].rlc_config_pending_var));
  qsh_mdump_collect_high((void*)&(downlink_table[as_id].active_ch), sizeof(uint8));
  
  for(i=0; i< downlink_table[as_id].active_ch; i++)
  {
	qsh_mdump_collect_high((void*)&(downlink_table[as_id].ctrl_blk[i]), sizeof(rlci_dl_ctrl_blk_type));
  }
  qsh_mdump_collect_high((void*)&(downlink_table[as_id].dl_pdu_decode_result), sizeof(boolean));
  qsh_mdump_collect_high((void*)&(downlink_table[as_id].rlc_dl_data_q), sizeof(rlc_dl_data_q_type));
  //qsh_mdump_collect_high((void*)&(downlink_table[as_id].time_since_last_dl_pdu_rcvd), sizeof(uint16));
  qsh_mdump_collect_high((void*)&(downlink_table[as_id].ctch_first_pdu), sizeof(boolean));

  qsh_mdump_prioritize_client(QSH_CLT_WRLC, QSH_CLT_WMAC);
  qsh_mdump_prioritize_client(QSH_CLT_WRLC, QSH_CLT_WL1);
  qsh_mdump_prioritize_client(QSH_CLT_WRLC, QSH_CLT_WRRC);

}
#endif
#if defined(FEATURE_WCDMA_QSH_DUMP) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_QSH_EVENT_NOTIFY_HANDLER)
/*===========================================================================
FUNCTION     wrlc_qsh_cb

DESCRIPTION  This function is called by QSH module
===========================================================================*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif
void wrlc_qsh_cb(qsh_client_cb_params_s* qsh_cb_params_ptr)
{
  sys_modem_as_id_e_type sub_id, as_id;
  uint32 cfg_data;
  if (QSH_CLT_WRLC != qsh_cb_params_ptr->client)
  {
    MSG_2(MSG_SSID_WCDMA_L2, MSG_LEGACY_ERROR, "RLC_QSH: Unexpected Client ID %d received, expected %d",
    	                                        qsh_cb_params_ptr->client, QSH_CLT_WRLC);
    return;
  }

  switch(qsh_cb_params_ptr->action)
  {
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC

    case QSH_ACTION_METRIC_CFG:
      sub_id = qsh_cb_params_ptr->action_params.metric_cfg.subs_id;
      as_id = wcdma_convert_subid_to_stack_index(sub_id);
      l2_validate_as_id(as_id);
      wrlc_process_qsh_commands(qsh_cb_params_ptr, as_id);
      break;

    /*Need to be applied for both subs, so using SYS_MODEM_AS_ID_NONE*/
    case QSH_ACTION_METRIC_TIMER_EXPIRY:
      wrlc_process_qsh_commands(qsh_cb_params_ptr, SYS_MODEM_AS_ID_NONE);
	break;

#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    /*No processing at RLC needed, since RLC events are enabled by default. Using Sub_1 to send confirmation to QSH*/
    case QSH_ACTION_EVENT_CFG:
      wrlc_process_qsh_commands(qsh_cb_params_ptr, SYS_MODEM_AS_ID_1);
      break;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif

#ifdef FEATURE_WCDMA_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
      wrlc_qsh_mini_dump_collect(qsh_cb_params_ptr);
      break;
#endif

#ifdef FEATURE_QSH_MDUMP
     case QSH_ACTION_MDUMP:
       sub_id = qsh_cb_params_ptr->action_params.mdump.crash_subs_id;
       as_id = wcdma_convert_subid_to_stack_index(sub_id);
       //l2_validate_as_id(as_id);
       if ((as_id >= L2_NUM_W_SUBS) || (as_id < L2_SUB_1))
       {
         wrlc_qsh_mdump_collect(SYS_MODEM_AS_ID_1, qsh_cb_params_ptr);		
         wrlc_qsh_mdump_collect(SYS_MODEM_AS_ID_2, qsh_cb_params_ptr);
       }
       else
       {
         wrlc_qsh_mdump_collect(as_id, qsh_cb_params_ptr);
       }
      break;
#endif

    default:
      MSG_1(MSG_SSID_WCDMA_L2, MSG_LEGACY_ERROR, "RLC_QSH: Unexpected Action %d",qsh_cb_params_ptr->action);
      break;
  }
}


/*===========================================================================
FUNCTION     wrlc_qsh_init

DESCRIPTION  Initializes W RLC Client with QSH
===========================================================================*/
void wrlc_qsh_init(void )
{
  uint8 sub_idx;
  qsh_client_reg_s client_reg;
  qsh_ext_metric_cfg_s *metric_cfg_ptr = NULL;
  
  qsh_client_reg_init(&client_reg);

  for (sub_idx = SYS_MODEM_AS_ID_1; sub_idx < L2_NUM_W_SUBS; sub_idx++)
  {
    metric_cfg_ptr = &wrlc_qsh_metric_cfg_arr[sub_idx][WRLC_QSH_METRIC_RLC_STAT_E];
    qsh_client_metric_cfg_init(metric_cfg_ptr,WRLC_QSH_METRIC_MAX_STAT_E);
   
    metric_cfg_ptr->id = WRLC_QSH_METRIC_RLC_STAT_E;
    metric_cfg_ptr->subs_id = wcdma_convert_stack_index_to_subid(sub_idx);
    metric_cfg_ptr->sampling_period_ms = 1000;
    metric_cfg_ptr->fifo.element_size_bytes = sizeof(wrlc_qsh_metric_data_statistics_s);
    metric_cfg_ptr->fifo.element_count_total = 10;
  }
  client_reg.metric_info.metric_cfg_arr_ptr = &wrlc_qsh_metric_cfg_arr[0][0];
  client_reg.metric_info.metric_cfg_count = (WRLC_QSH_METRIC_MAX_STAT_E*L2_NUM_W_SUBS);
  
  client_reg.dump_info.max_size_bytes = (sizeof(wrlc_qsh_dump_tag_ul_dl_s)*L2_NUM_W_SUBS);
 
  client_reg.client= QSH_CLT_WRLC;
  client_reg.client_cb_ptr= wrlc_qsh_cb;
  client_reg.major_ver= WRLC_QSH_MAJOR_VER;
  client_reg.minor_ver= WRLC_QSH_MINOR_VER;
  client_reg.cb_action_support_mask= (QSH_ACTION_DUMP_COLLECT
                                      | QSH_ACTION_METRIC_TIMER_EXPIRY
                                      | QSH_ACTION_METRIC_CFG
                                      #ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH 
                                      | QSH_ACTION_EVENT_CFG
                                      #endif
				      #ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
                                      #error code not present
#endif
#ifdef FEATURE_QSH_MDUMP
                                      | QSH_ACTION_MDUMP 
#endif
				      );
 
  qsh_client_reg(&client_reg);
#ifdef FEATURE_QSH_MDUMP
	qsh_add_thread_id(QSH_CLT_WMAC);
#endif
}
#endif
