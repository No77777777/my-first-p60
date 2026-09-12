/*===========================================================================

                                  R R C D A T A

GENERAL DESCRIPTION
 The RRCDATA module consists of RRC CONNECTION ESTABLISHMENT PROCEDURE 
 implementation. The details are from section 8.1.3 of 3GPP TS 25.331
 V3.4.1 (2000-9) specification



Copyright (c) 2000-2009 Qualcomm Technologies, Inc.  All Rights Reserved.
QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/RRC/vcs/rrcdata.c_v   1.46   10 Jul 2002 09:51:12   rjeenaga  $   
$Header: //components/rel/wcdma.mpss/8.4.0/rrc/src/rrcdata.c#17 $    $DateTime: 2022/09/20 03:17:26 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/20/22   ha      Made changes to call correct WL1 API to trigger FRLTE after SS
07/14/20   ak      Made changes for FR 61612
07/14/20   gcs     Made changes for FR54317
06/02/20   sr      Changes to downgrade priority to BGT for temporary DDS switch
05/22/20   sp      Made changes to avoid backing up heap memory to minidump
05/21/20    gc     Made changes to block FRLTE for specific PLMNs
18/5/20   gcs     Added NV support to disable/enable feature FRLTE for LTOW CSFB due to supplementary service and Made changes to trigger FRLTE only if NV is enabled
04/29/20   gcs     Made changes for FR 57593
04/21/20   sas     FR 47827: Changes to forecast frequency list to AP to eliminate LCD HW interference.
02/17/20   ak      Made changes to correct the HSRACH de-featurization.
09/22/17   sp      Mini dump phase-2 changes
09/03/19   ak      Made changes to fix compialtion error.
11/20/18   ak      Made changes for FR 53313
09/26/18   gcs     Made Changes to trigger FRLTE for Supplementary Service due to CSFB
05/01/18   sp      Made changes to report state change event to QSH
03/28/18   sp      Made changes to avoid saving back to back unlock_by/cphy_get_modem_stats commands in crash debug buffers
03/28/18   sp      Debug code to use ULOG info to catch cctrch issues
03/20/18   gc      Initialize curr_csp_scan to fix KW errors
03/21/18   sp      Made changes to ignore OTA with duplicate trid
03/21/18   nr      Added code to support LTA during activation time
02/27/18   sas     Changes to handle new proctypes from NAS and map them to ACCESS priority.
02/20/18   gc      Changes to not downgrade priority if CS RABs are up
01/30/18   sas     Supervision timer changes in LTA gap.
01/25/18   sas     Changes to start PRIORITY_CHANGE_TIMER on receiving SYS_PROC_TYPE_NONE_DELAYED to delay the downgrade to BGT.
01/18/18   sr      Changes to allow page LTA during RCE signaling
01/16/18   sp      Made changes to add procedure/substate for ML framework
01/16/18   sp      ML Log analysis initial check-in
12/11/17   ak      Made changes to fix the KW error.
11/21/17   sp      Made changes to provide an API which returns WCDMA release version
11/02/17   sp      Made changes to save DDS sub id indicated by NAS
08/29/17   sg       Made changes to remove the GCF NV 947 read and get the value of this NV through an api
01/30/17   sg      Made changes to set DCHSUPA category based on DCHSDPA category and update proper HSUPA category
09/09/16   sp      Made changes to report CU cause to QSH
07/12/16   nr      Made changes to report cell reselection failure under general failure events to QSH
06/30/16   sg      Made changes for F3 reduction
06/27/16   sp      Added support to report general failure events to QSH
06/10/16   sp      Made changes to save metric config start address into a new db for dual sim cases.
06/03/16   sp      Changes for saving log packet in crashdump
05/30/16   vs      Changes to add LOCK_WAIT_TIMEOUT event as a part of DTF
05/26/16   ha      Made changes to report Inter freq HO failure to QSH
05/26/16   sg      Made changes to reduce F3s
05/24/16   vs      Changes to add SIB_READ_FAILURE event as a part of DTF
05/20/16   sr      f3 reduction changes for RCE/UECI/DT
05/19/15   sn      f3 reduction changes for cell resel, unlock handling, pg1
05/12/16   sn      f3 reduction changes BPLMN scenarios
05/11/16   nr      Added support for QSH handler framework
05/04/16   sn      f3 density reduction changes
04/27/16   nr      Made changes to control FEATURE_SIB7_IN_CONNECTING through a global variable
04/19/16   nr      Made changes to remove redundant F3s
04/15/16   sn      Changes for f3 logging reduction
04/14/16   sas     Safe check to handle proc_ind immedietely only if TX lock is held.
04/06/16   sp      Made changes to update DRX cycle length to QSH by using proper API
03/10/16   nr      Fixed KW errors
02/11/16   sp      Made Changes to log misc commands in crash debug info
02/11/16   vs      Made changes to provide crash debug support to save TDS-RRC commads
01/22/16   nr      Made changes to remove irrelevant F3s
01/12/16   nr      Made changes to report failure events to QSH
01/13/16   sp      Made changes to avoid reporting invalid parameters in serving cell metrics during RLF
01/05/16   sp      Made changes to report cell-id to QSH whenever GPS cell ID request is honored.
01/04/16   sp      Made changes to fix QSH Active set metric, UL OTA parameters metrics 
12/21/15   sp      Made changes to disable QSH events by default
12/16/15   nr      Fixing compilation error related to QSH
12/04/15   sp      Made changes to use uint32 for cell-id instead of uint16
11/05/15   sp      Made changes to add call end metrics - FR#30856
10/29/15   vi      Made changes to add as_id to RSSI APIs
09/29/15   us      Fixed compilation errors got after enabling ATLAS and RFM_PATH feature
10/20/15   gv      Made changes to pass CSP as procedure id while checking for camping status when 
                   a new proc ind is received from NAS
10/15/15   sp      Made changes to add resel failure, rrc conn rel events
09/29/15   sp      Made changes to modify qsh log reason as timer expiry for timer based events
09/29/15   sp      Made changes to ignore QSH timer expiry call back when RRC is in Lock wait state 
09/22/15   sr      Changes were made to map the loopack procedure to ACCESS priority
09/18/15   as      Made changes to indicate RAB typ correctly for CS-DATA
09/14/15   sp      Removed CU substate check in rrc_check_if_any_proc_waiting_for_l2_ack_for_pch_transition function.
09/14/15   sp      Made changes to log configuration info to QSH structures in inter task commands.
09/08/15   sp      Made changes to populate UL/DL Rb rates to PDCP
08/27/15   sg      Made changes to introduce a wrapper to NAS to indicate UE HSUPA category
09/01/15   gv      Made changes to register for messages from LTE on both the subscriptions
08/21/15   gv      Made changes to modify TRM priority only when in camped state with active connection
08/17/15   gv      Made changes to report WCDMA stats on the sub on which it is actually camped
07/21/15   gv      Made changes to make use of right priority for voice call
07/15/15   sn      Changes for FR 28274:NAS_GET_NEGOTIATED_DRX_REQ_MSG
07/13/15   sa      Made changes for enabling the FET with CS with PS0x0 configuration 
07/08/15   sr      TRM-U changes for NAS-RRC interface
07/07/15   sg      Made changes to add PSC to timers and counters metric
07/03/15   ad      Made changes to remove DC 23 code
06/01/15   sp      Made changes to remove debug F3s added for QSH 
06/30/15   sp      Fixed QSH compilation errors
06/30/15   sp      Fixed compilation errors with QSH feature enabled
06/26/15   sp      Made changes to save procedure substates for LTE commands and to skip logging frequent internal commands
06/22/15   sr      Made changes to not change priority to TRAFFIC for TA disabled cases
06/18/15   sp      Made changes for ULOG framework
06/16/15   vg      Made changes to remove compilation warnings due to double parentheses.
06/03/15   aa      Made changes to allow CU retransmissions N302 times for single sim cases
05/25/15   sa      Made changes to support eAMR for FR25951 Single VoiceSession
05/12/15   vg      Made changes to support W CB/BMC for W/L/G + W/G DSDS if feature FEATURE_UMTS_BMC_MSIM
                   is defined.
05/01/15   vi      Made changes to get rrc tcb from rcinit context
04/27/15   sp      Removed W+W temporary hacks
04/21/15   jd      cleanup of unused wl1 api.
04/21/15   sr      TRM Unification changes from WCDMA RRC
04/15/15   sad     QTA clean up changes
04/13/15   sa      Removed the FEATURE_ENHANCED_AMR, this feature will add later as part of FR25951
04/08/15   sa      Made changes to support FR25951_SingleVoiceSession
03/19/15   as      RRC-L2 interface changes for passing AS ID as part of L2 commands
03/12/15   bc      Changes to fix W+W compilation errors
02/28/15   rsr     W + W Phase 2 Checkin.
01/22/15   bc      Changes to provide NV control to allow TA for loop back call and CS Data call
02/21/15   sa      Mainline the FEATURE_VOC_AMR_WB feature
02/19/15   sas     Made changes to add a new API which takes as_id as agrument and calls rrcdormancy_check_if_nw_support_fast_dormancy()
02/19/15   sn      Changes to allow LTA even after PS RABs are released but not before
                   they are established
02/14/15   sa      Made changes for QTF
02/09/15   as      SUB_ID changes for L2-RRC interface
01/12/15   sp      Made changes to save one byte status in cmd_ptr to crash debug global
12/31/14   bc      Cahgnes to replace event_report_payload with WRRC_EVENT_REPORT_PAYLOAD.
12/30/14   bc      Replacing ERR_FATAL with MSG_ERR0_FATAL, MSG_ERR1_FATAL, MSG_ERR2_FATAL.
11/25/14   as      Fixed compilation errors
11/10/14   sp      Made changes to save procedure substates in crash debug info
10/30/14   vi       Made changes to increase maximum value of t304 timer to 10 sec
10/22/14   gv      Made changes to have connected mode feature set independent of TA status 
10/13/14   as      Fixed compiler warnings
10/10/14   sr      Made changes to trigger tranistion to idle if TA duraitonis mroe than re-establishment timer values
10/07/14   sr      Made changes to check if UMI has received IE "ue_ConnTimersAndConstants" before initializing the parameters
10/06/14   gv      Made changes to follow same feature set in DSDA irrespective of number of active subs
09/19/14   sg      Made changes to send SCRI after CU confirm
09/23/14   sn      Changes to lower TRM priority to BG trafic for TA disabled cases
                   to allow GSM page decodes in dual receive targets
09/09/14   vi      Made changes to send old ECIO value if rssi_valid flag is set to FALSE by WL1
09/03/14   sp      Made changes to save MCM/ASU Otas in crash debug info
08/04/14   sp      Made changes to fix compilation errors
07/24/14   sp      Made changes to skip saving RCR OTA in crash debug global if there is identity mismatch
07/21/14   as      Made changes to check-in F3 reduction chnages
07/18/14   sp      Made changes to add prefix to enum values in rrc_crash_debug_msg_e_type to avoid name conficts
07/15/14   ac      NV reorganization
12/30/13   sg      Made changes to add initialization of local variables and bound conditions 
                   to access array to avoid warnings and invalid memory access. 
06/30/14   vi      Made changes to get rid of wcdma_is_ue_in_dual_active for better clarity
06/27/14   sp      Made changes to correct LLC command index in crash debug info
06/18/14   as      Made changes to delay reselection while waiting for L2 ACK for PCH transition
06/19/14   rmn     Made changes for DR-DSDS Feature
05/28/14   bc      Made changes to ignore the RSSI received from WL1 when UE is in connecting state.
05/26/14   sp      Made changes to add DCHSUPA support in call status indication
05/20/14   rmn     Made changes to save DS Tuneaway Status Change indication in connected state based on the concurrency NV
05/20/14   sr      Made changes to not send STOP REQ to GSM when no GSM bands are supported
05/19/14   vg      Made changes to send HO_START only when voice HHO is triggered.
05/16/14   bc      Made changes to initialize ue conn_timers and constants when ue_conn_timers_and_constants_ptr
                   is not present in SIB1 after selecting a new cell.
05/16/14   vi      Made changes to support MOB framework
05/14/14   ymu     Feature name FEATURE_WCDMA_DC_HSUPA_FRAME replacement with FEATURE_WCDMA_DC_HSUPA 
04/23/14   bc      Made changes to send cell info to LTE on receiving UE capability request
                   and also to send cell info WTOL PSHO request.
05/06/14   geg     Use safer versions of memcpy() and memmove()
05/05/14   as      Made changes for proper trch id validation for CS data call
18/04/14   ad      Made changes to adaptively calcualte T304 for GCF TCs
04/25/14   vi      Made changes to handle LTE deprioritization request in W-RRC
04/11/14   vi      Made changes to disable FE_FACH capability in DSDS/DSDA mode.
04/10/14   sp      Made changes to clear re-establish entity when a reconfig procedure is cleared
04/10/14   sg     Made changes to correct the feature flag used for rel 10 spec CRs
04/02/14   vg      Made changes to Systematically log all the internal and external primitive handled in RRC and corresponding top level action.
03/14/14   bc      Made changes to send partial plmn results to NAS during manual plmn search
02/04/14   sg      Made code changes for 3c for latest interface
03/14/14   sp      Made changes to correct llc command ids saved in rrc crash debug info
03/14/14   sp      Made changes to init re-establish entity on Procedure cleanup
03/13/14   sp      Removed code under FEATURE_WCDMA_HS_PCH feature flag
02/13/14   ad      Made changes to remove redundant F3s
03/08/14   vg      Made changes to reduce the stack size usage of some RRC functions which has the usage more than 500 bytes.
02/03/14   as      Made changes to backup internal/external cmds for crash debugging
01/30/14   sn      WRRC changes for FR 15265
01/29/14   sa      Made the changes to set the AS Rel indicator as REL-9 by default for DI.3.0 PLs only
01/09/14   sp      Removed code under FEATURE_CS_VOICE_OVER_HSPA feature
05/29/13   ad      Added new fields for tx agc,rx agc and dl bler.and serving cell info is provided in dch state
12/30/13   sr      WRRC support for new requirements for LWG DSDS
11/08/13   ad      Made changes to indicate ho_type as part of codec info to CM
12/12/13   rmn     Mainlined feature flag FEATURE_INTERRAT_HANDOVER_GTOW
10/12/13   sa      Made changes to update the default_cfg #24 len
12/03/13   sg      Made changes to update review comments for 3C 
12/03/13   mn      Moved all declarations to top of the code.
12/03/13   sp      Made changes to add nv support and to save time ticks for 
                   rr to/from rrc mm to/from rrc and to add msg_type for crash debug ota changes
11/21/13   rmn     Made changes so that RRC doesn't req WRM functionality in Single Sim mode
09/16/13   sn      G2W TA feature changes
07/09/13   bc      Changes to not indicate CSG support in IDT when CSG is disabled
                   through NV
11/14/13   sa      DC feature support will be controlled thru UE Cat
11/14/13   sp      Made changes for QMI Extension for Detailed Protocol Stack Information 
11/14/13   sa      Made the changes for Rel-10 spec CRCR4566 under FR16756
11/13/13   sa      Mainlined the FEATURE_MVS_UNDER_GHDI
11/04/13   as      Made changes to error fatal if crash debug ota memory is not freed for non HO cases
10/30/13   as      Made changes to add feature code for FEATURE_WCDMA_3C_HSDPA
10/29/13   as      Fixed compilation error
10/27/13   as      Made changes to backup OTAs for crash debugging
10/17/13   ad      Changes for DC HSDPA feature with 3C interface
10/07/13   gv      Made changes to indicate ok for tuneaway irrespective of UE camped status
09/16/13   mp      Added code for REL10 signalling under feature FEATURE_WCDMA_REL10
08/13/13   sr      Made changes to not set TRM extension flag when in FACH state
08/13/13   as      Added support for FEATURE_TRIPLE_SIM
08/26/13   bc      Added changes to indicate the current rrc version when the respective api is called.
08/26/13   as      Made changes to provide API to L2 to get CS + PS call status
08/14/13   vg      Made changes to implement a new diag event EVENT_WCDMA_RRC_FAILURE
07/31/13   vi      Mainlining the MODEM_STATISTICS feature
07/09/13   bc      Changes to not indicate CSG support in IDT when CSG is disabled
                   through NV
07/08/13   sn      Changes for LTE CSG feature
06/27/13   gv      Made changes to buffer the MM and RR commands for crash debugging
06/27/13   sa      Added changes to support call type mask for AMR with PS0x0 config
06/20/13   sa      Made changes for Default config in CELL_FACH
06/10/13   gv      Made changes to store correct PPAC access class restrictions for AC > 10
05/20/13   sn      Changes to consider W CSG feature as disabled if there is 
                   no whitelist update from NAS
05/22/13   sa      Made the changes for spec CR5118
03/28/13   ad      Made changes to send codec info to CM when CS RAB is up
04/29/13   sa      Made code changes for eAMR supprot 
05/15/13   as      Made changes to pass whether PO3 is signalled or pre-set
05/11/13   vi      Made changes to send last received RSSI value in case of Interfrequency Hard Handover
05/10/13   sg      Made changes to fix the compilation errors
04/25/13   sg      Added code for REL9 under FEATURE_WCDMA_REL9
04/08/13   sr      Made changes to send valid RSSI measurements to CM when WCDMA is in standby mode
03/22/13   mp      Removed tmc.h include file
03/14/13   md      Made changes to provide active subscription ID to NAS APIs for DSDA
02/15/13   gv      Made changes to handle tuneaway only if UE is camped
02/05/13   ad      Made changes to release the vocoder when rab is removed from 
                   established rabs
01/28/13   sn      Changes to pass RRC event correctly to upper layers in UI event info
01/23/13   gv      Made changes to reduce logging
01/22/13   sr      Fixed KW errors
09/01/13   ad      Added featurization for MVS code
01/03/13   ad      Made changes to reduce the redundant F3s
12/07/12   sn      CSG feature changes
12/06/12   db      Made changes to send RRC_HSPA_CALL_STATUS_IND for EFACH Redesign
12/05/12   pm      Made changes to send RRC_HSPA_CALL_STATUS_IND when EFACH cell is up
12/04/12   ad      Made changes to send start of SRVCC indication to CM 
11/08/12   sn      Changes to include LTE cells in modem statistics
11/08/12   pm      Made changed to remove rrcasn1_ie.h from rrcdata.h to avoid SU API
                   access violations.
10/30/12   gv      Added code to support suspend and resume of BPLMN searches from LTE to WCDMA
10/29/12   gv      Mainline the changes under FEATURE_PPAC
10/26/12   gv      Mainline the changes under FEATURE_3GPP_FAST_DORMANCY_ENHANCEMENT
10/26/12   gv      Corrected changes under FEATURE_WCDMA_DIME_SW
10/26/12   db      EFACH Redesign changes
10/18/12   gv      Mainline the changes under FEATURE_UMTS_SIR_PATHLOSS
10/18/12   gv      Replace FEATURE_DUAL_SIM_WCDMA_TUNEAWAY with FEATURE_DUAL_SIM
10/08/12   gv      Made changes to undefine DIME SW feature to facilitate umid registration
10/04/12   pm      Removed unnecessary comments
10/02/12   sa      Fixed the compilation warnings
09/26/12   pm      Made changes to reset the ordered codec when procedure is cleared
08/23/12   md      Made changes to remove free floating featurization in fun 
                   rrc_return_hsdpa_category_value.
07/24/12   geg     Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
05/02/12   rgn     Changed look up string for RC init
04/26/12   as      Added code to support feature NB/WB handover support
03/28/12   zr      Adding feature definitions for Dime
05/14/12   db      Corrected F3 to avoid confusion
04/10/12   gv      Made changes to remove unused heap allocation and related unused code
02/23/12   sks     Added support for WTOL PSHO.
02/22/12   ad      Removed references to third party
01/23/12   sn      Changes for E-UTRA detection
01/12/12   gv      Added code to support FEATURE_PPAC
01/10/12   gv      Changed the variable names in modem stat req
12/30/11   ad      Fixed compilation warnings
12/16/11   gv      Fixed compilation issue
12/13/11   md      Corrected mapping function for RRC procedure name to log 
                   WCDMA_RRC_PROTOCOL_ERRORS packet.
12/12/11   gv      Added code to support free floating of tasks
12/09/11   ad      Added code to log WCDMA_RRC_PROTOCOL_ERRORS packet
12/05/11   pm      Added code to support cell id request by CM in DCH state"
10/25/11   gv      Fixed compilation issue
10/20/11   gv      Added function to send a rab-rel-ind after RRC_REL_IND for 
                   PS rabs
10/14/11   rl      Made changes for WCDMA Security Setting independent of NV Items.
09/28/11   as      Made changes to allow tuneaway if PS Rab is not present and RRC
                   Connection is established
09/07/11   gv      Added code to support fast dormancy enhancement featur
08/30/11   ad      Made changes to handle default configuration 23
08/18/11   ad      Removing misleading prints for MMAL
07/28/11   su      Added code changes to indicate the 64QAM configuration status
                   as part of RRC_HSPA_CALL_STATUS_IND.
07/28/11   gv      Added code to support FEATURE_UIM_DISABLE_GSDI_INTERFACE
06/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.99.50
05/19/11   kp      Made changes to set mbms_srv_id to RRC_INVALID_SERVICE_ID
06/02/11   ad      Added MMAL changes to return the UL trans id in OTA
06/02/11   ad      Added MMAL changes to return the trch type for the OTA
05/24/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.98.50
05/16/11   ad      Fixed compilation errors for NON-HSDPA targets
05/10/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.97.50
05/05/11   ad      Fixed compilation errors
04/28/11   ad      Added protection code for MMAL changes
04/26/11   rl      Support for transition from TMC heap to Modem Heap in WCDMA.
04/26/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.96.50
04/21/11   ad      Fixed compilation warnings
04/21/11   as      Made changes to reject OTA if ordered code from OTA
                   is different from derived one
04/20/11   ad      Fixed compilation errors
04/15/11   ad      Modified code for RRC utility function
04/11/11   ad      Added function which returns the message type,
                   transaction ID,etc. under FEATURE_MMAL macro
03/29/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.94.50
03/28/11   sks     Corrected featurization issue when reporting BLER values to CM.
03/21/11   rl      Added code to support L2W CGI
03/17/11   su      Added default case to the switch statement for 
                   UL/DL UE category passed to DS.
03/14/11   su      Fixed compilation warning.
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/10/11   as      Added debug prints for DSIM tuneaway
03/10/11   rl      Assigned the max channel data rate of 384kbps when the 
                   current data rate is zero.
03/14/11   su      Added code changes to give UL/DL UE supported CAT and 
                   max. data rate possible to DS.
02/15/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.91.50
02/11/11   su      Added code to support setting UE HSDPA category 
                   through QXDM command.
02/07/11   vg      added rrc change for CMI4
02/11/11   rl      Fixed the compiler warning
02/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/28/11   sks     Added support for LTOW PSHO.
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
11/30/10   as      Added changes to update tuneaway status to trm
11/29/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.85.50
11/16/10   as      Added code to send Subscription id in CM RRC Status Ind
11/15/10   as      Added code to not allow DS tuneaway in Loob back mode
11/08/10   su      Added Code changes to indicate the correct CAT to L1.
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
11/04/10   su      Corrected the Typo from FEATURE_WCMDA_COMBINED_MIMO_AND_64QAM
                   to FEATURE_WCDMA_COMBINED_MIMO_AND_64QAM
10/29/10   su      Made changes to remove WCDMA_TASKLOCK() usage from RRC code base.
10/13/10   su      Changed RRC_WCMDA_COMBINED_MIMO_AND_64QAM to 
                   FEATURE_WCMDA_COMBINED_MIMO_AND_64QAM
10/08/10   su      Added code to support RRC_WCMDA_COMBINED_MIMO_AND_64QAM
10/08/10   as      Added changes to Enable/Disbale DS Tuneaway using NV Item
10/05/10   as      Added code to retain lock after chaning priority to BG_TRAFFIC
                   for FEATURE_DUAL_SIM
09/27/10   sk/np   Code to register ded prio related UMIDs      
09/20/10   prk/kp  Added code to support FEATURE_WCDMA_HS_FACH
09/17/10   su      Featurizing MVS related code
11/03/10   rl      Fixed review comments/compiler warnings
10/30/10   rl      Objective systems migration changes check in 
                   on code base VU_MODEM_WCDMA_RRC.01.80.00
09/13/10   as      Added code to support FEATURE_DUAL_SIM
07/29/10   su      Fixed compilation errors on MOB
07/26/10   su      Made the changes to interpret CS RAB type independent of 
                   FEATURE_SCUDIF_ENHANCEMENT.
07/25/10   kp      Fixed compiler warnings.
06/29/10   rm      Added code to support LTE<->WCDMA Inter RAT
06/29/10   as      Removed feature flag WCDMA_UNIT_TEST for rrc stub code
06/18/10   as      Fixed compiler warnings
06/18/10   prk     Fixed compiler warnings.
06/15/10   gkg     Made changes to default the RRC NV version based on defined feature flag
                   if RRC NV version is set incorrectly.
05/10/10   gkg     Fixed Compile Warnings.
04/26/10   ss      Added code changes for Fast Dormancy feature under feature flag
                   FEATURE_3GPP_FAST_DORMANCY
03/31/10   rm      Added code to report HS CQI to CM on BER timer expiry
03/17/10   gkg     To comply with CMI, featurize Callback function pointer 
                   rrccu_rlc_ul_data_ind with feature FEATURE_DSM_WM_CB
03/30/10   rm      Added code to respond to higher layers with previously received AS ID
                   when AS ID at RRC was invalidated
03/23/10   rm      Moved reporting of GSM cells for FEATURE_MODEM_STATISTICS under
                   FEATURE INTERRAT HANDOVER WTOG
03/12/10   rm      Added code to support FEATURE_DUAL_SIM
02/11/10   rm      Fixed compilation errors when FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY and
                   FEATURE_UMTS_SIR_PATHLOSS are defined
02/10/10   as      Added feature falg WCDMA_UNIT_TEST for rrc stub code
12/22/09   gkg     Made changes for CSoHS so that NV 6878 controls CSoHS operation.
12/05/09   gkg     Made changes for CSoHSPA under feature FEATURE_CS_VOICE_OVER_HSPA.
12/14/09   rm      Added support to report GSM monitored cells for FEATURE_MODEM_STATISTICS
12/07/09   ss      Changes to add Stub code under feature flag TEST_FRAMEWORK
12/01/09   dm      Made changes to ignore CTCH configuration present in SIB6 
                   and use the info contained in SIB5 in connected mode.
12/01/09   gkg     Made changes to disable BMC service based on feature FEATURE_WCDMA_DISABLE_BMC
                   even though the actual serving cell does support BMC.
11/26/09   gkg     Made changes for returning the HSDPA CAT to L1 when DC-HSDPA
                   feature is enabled and HSDPA CAT is set to 24 by NV. In this case,
                   HSDPA CAT is decided based on Call Type.
11/07/09   gkg     Fixed LINT warnings.
11/02/09   rm      Added support for FEATURE_MODEM_STATISTICS
06/24/09   gkg     Added support for API for getting the DC HSPDA Status. 
                   Made changes to send DC-HSDPA+ status to upper layers
                   in command RRC_HSPA_CALL_STATUS_IND to MM.
10/07/09   gkg     Changes for REL8 under FEATURE_WCDMA_REL8
10/01/09   gkg     Made changes so that CS RAB Type is provided to MM in case
                   a RAB is released.
09/11/09   ps      Made changes for reducing the internal RRC command size 
                   under FEATURE_RRC_REDUCE_CMD_SIZE
08/14/09   ps      Added support to send HSDPA+ status also when MAC EHS is enabled
                   through RRC_HSPA_CALL_STATUS_IND
08/17/09   sks     Added code pass on the BLER value from L1 to CM on 3 sec RSSI BER timer expiry.
08/03/09   kp      Changes to obsolete gs_alloc/gs_free under feature flag FEATURE MODEM HEAP
06/24/09   ps      Added support for API for getting the HSPA PLUS Status
06/09/09   ps      Fixed compiler warnings
06/08/09   dm      Added support for freeing 100k sib heap when WCDMA stack is 
                   inactive as part of ZI reduction activity. code changes are under
                   FEATURE_WCDMA_RRC_ZI_OPT_PHASE_2.
06/05/09   ps      Made changes to not start the T305 timer if value is infinity
05/29/09   ps      Made changes to make T305 timer as 24 hrs
05/10/09   kp      Added support for demand paging RRC code using pragma.
05/08/09   gkg     When Loopback Mode is enabled for a VT RAB, RRC will notify all the RBs 
                   of the VT RAB to the TC module. But in case, VT RAB is used for VT Call, 
                   only the first RB will be notified to VT App.
05/08/09   ss      Updated Copyright Information
04/29/09   ss      Made changes to compiler warnings on gcc compilers.
03/28/09   rm      CMI Featurisation
03/24/09   ps      Made changes that once OTA provides RLC size, then it
                   can be changed only through OTA, only in case of R3 OTA
                   RLC size will be calculated from the TF of the Trch
03/24/09   rm      On BER timer expiry while leaving connected mode is in progress,
                   instead of reporting ZERO RSSI to CM, stored value of RSSI is reported.
03/02/09   kp      Removed NV GCF dependency for feature FEATURE NETWORK SHARING.
12/08/08   ps      Made changes to set the MAC-HS(R5/R6 HS-DSCH )CAT 
                   according to global variable "rrc_machs_cat" when the 
                   HSDPA CAT is above 12 for R7
11/28/08   gkg     Made Changes for new MVS-RRC Interface to (de)configure vocoder
                   for AMR Voice Calls.
11/21/08   ps      Modified rrc_return_hsdpa_category_value() to return the 
                   HS category according to MIMO action to L1
10/10/08   ps      Removal of feature FEATURE_GTOW_HO_TO_UTRAN_SPOOF_ENABLE
                    As the Spoof  is no longer required for Anritsu
09/12/08   ps      Added code for REl7 under FEATURE_WCDMA_REL7
06/30/08   ns      Made the necessary changes to enable the functionality of the feature 
                   FEATURE NETWORK SHARING only when the GCF NV is set
06/13/08   gkg     Added function rrc_get_idx_in_rrc_est_rabs_for_rab_id().
                   It returns index in global rrc_est_rabs for a given rab id.
03/28/08   ps      Added code to register with RLC for UL data ind , if 
                   the watermark queue for that LC ID is registered in PCH state
03/24/08   pk      Added support for reporting SIR & Pathloss to CM.
03/24/08   pk      Fixed compilation error with feature FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
                   enabled.
03/06/08   da      Changed FEATURE_MBMS to FEATURE_MODEM_MBMS
22/02/08   ps/gkg  To support Spec CR 3160, DC 12 is removed for rel99 and rel5. 
                   And to support Spec CR 3182, puncturing limit is changed for
                   DC 15, 18, 19, 20 and 21.
21/02/08   gkg     Fixed array index out-of-bound warnings reported by Klocwork.
01/17/08   ps      Made correction to remove the validation for release of
                   Non-existing RBs through Cu Cnf
01/08/08   ps/gkg  Made changes to query the RAb type from Mn even for 
                   Handover To utran msg.
12/31/07   ps      Made change to default rrc_nv_rel_indicator to R6 whenever
                   FEATURE_WCDMA_REL6 is defined and UE fails to read NV item
12/21/07   da      Support for FEATURE_MBMS
12/14/07   rm      Changed ERR to MSG_HIGH in case of serving cell SIB is invalid
12/12/07   gkg     Added support for VT->Voice fallback and vice versa
                   when "RAB Info to Reconfig " IE is not present in the RB-Reconfig/RB-Release
                   msg  for SCUDIF under feature flag FEATURE_SCUDIF_ENHANCEMENT
12/10/07   da      Made change to default rrc_nv_rel_indicator to R6 whenever
                   FEATURE WCDMA HSUPA is defined.
11/26/07   gkg     Added support for VT->Voice fallback and vice versa
                   by RB-Reconfig msg for SCUDIF.Also added changes to 
                   support VT->Voice fallback using RB-Setup msg.
                   Changes under feature flag FEATURE_SCUDIF_ENHANCEMENT
11/23/07   pk      Added support for a function to extract the RAC given the 
                   NAS CN Domain System Information List.
11/21/07   rc/vm   Added support for FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY feature.
10/30/07   ps      Added support for DC 14.Modified DC 16 as according to 
                   spec cr 3100 .Also modified DC 11 and 12 to
                   send Power Information as computed as in spec
10/23/07   ps      Added support for DC 20 through Rb-Setup msg
10/23/07   pk      Added support for network sharing feature.Changes are under feature flag 
                   FEATURE NETWORK SHARING.
10/19/07   pk      On BER timer expiry while FACH->FACH/PCH state transition is
                   in progress, instead of reporting ZERO RSSI to CM, stored value
                   of RSSI is reported.
09/18/07   rm      Updated the data for Default Configs 11 and 12
                   as per Spec CR 2823
08/28/07   kp      Added support for capturing RACH transmission Statistic.
07/27/07   da      Put code changes for HSPA call status ind under feature 
                   flag FEATURE_HSPA_CALL_STATUS_IND
07/13/07   da      Added support for HSPA call status indication.
05/23/07   ps      Added support for "RB Info to Change" IE in RB-Setup msg.
                   Change are made under feature flag 
                   FEATURE_DEFAULT_CONFIG_IN_RB_SETUP.
04/24/07   kp      Added support for maintaining RACH transmission statistics
                   which is needed to evaluate performance in TCXO Shutdown.
                   This is currently TBD. Its values are set to Zero for now.
04/17/07   pk      Added support for maintaining intra frequency cell reselection,
                   WTOG cell reselection and OOS statistics which are needed to evaluate 
                   performance in TCXO Shutdown.
04/10/07   ps      Added support for default configs sent through Rb-Setup msg
                   The support for default config 6 has also
04/03/07   da      Changed F3 msg to reflect correct PDCP NV item number.
03/09/07   vm      Added support for FEATURE_RRC_HEAP.
03/02/07   ps      Added support for WB-AMR .Also added support for  
                   default configs 13, 16 and 21 
02/07/07   ps      Made correction in DC 22 for RLC timer
02/05/07   da      Fix for NULL pointer derefences reported by Klocwork. 
02/05/07   ps      Added support for default config 15,18and 19
01/25/07   segu    Added support to inform EcIo along with RSSI while reporting to CM
12/11/06   segu    Added code changes not put previous valid RSSI to NAS/UI when 
                   is_valid_rssi flag is FALSE and not the 1st 3S UI BER timer 
                   expired
11/30/06   sm      Fixed Lint error
11/30/06   sm      Added E-DCH cell indicator support under NV 'rrc_nv_rel_indicator'
11/17/06   sm      Added support for E-DCH cell indicator
10/25/06   sgk     Moved function rrc_check_if_wcdma_can_be_stopped under 
                   FEATURE_WCDMA_TRM. 
10/18/06   ps      Added support for default config 22
10/13/06   sgk     Added 7600 support under feature
                   FEATURE_UMTS_1X_HANDOVER_UMTSMSM
09/12/06   sm/da   Fix code review comments and general clean-up for REL6 and HSUPA
09/11/06   vk      Defined global structure rlc_size_change_in_progress. Also
                   implemented functions to initialize RLC size
                   structure as well as get RLC Size Change status. Changes
                   have been featurized under FEATURE RLC ONE SIDE RE ESTABLISHMENT
08/29/06   sm/da   Made changes required for R6 ASN.1 migration.  Also merged
                   FEATURE_WCDMA_REL6 and FEATURE WCDMA HSUPA.
08/23/06   ps      Added support for default config 10
08/21/06   tkk     Added SRB#5 support.
08/03/06   da      Changed the default value of global var rrc_pdcp_enabled [used to enable
                   feature PDCP] to false. So in case of NV item read 
                   failure..., default value rrc_pdcp_enabled is FALSE [i.e PDCP disabled].          
08/02/06   tkk     Added "rrc_msg_failure_cause" global variable which holds
                   the appropriate failure cause encountered during message
                   processing.
07/07/06   sgk     Fixed lint errors.
06/26/06   tkk     Added functions"rrc_find_if_voice_rab_exists_in_established_rabs",
                   "rrc_update_rb_info_to_existing_voice_rab" to be able to
                   support lower to higher AMR rate switching through third RB.
06/15/06   vk      Added a new function rrc_get_hsdpa_cell_capability_indicator()
                   to indicate if the cell supports HSDPA. Defined global variable
                   hsdpa_hsupa_ind that stores HSDPA/HSUPA status. Also added
                   new fn rrc_send_3g_cell_capability_information that sends the
                   HSDPA Capability indication to MM
06/13/06   da      Made change to func validate_rb_id_in_release_list() to return
                   RRC_RB_NOT_PRESENT only when SRB 0, 1, 2, or 3 are in the 
                   release list.  All other are valid even if they are not in 
                   established rabs.
06/05/06   sgk     Made changes to include rel5 non HSDPA functionality under 
                   FEATURE REL5 while retaining the HSDPA specifc processing 
                   under FEATURE HSDPA.
05/09/06   sgk     RVDS related typecast fix, return value of  log_alloc typecast 
                   to (WCDMA_RRC_PROTOCOL_ERRORS_type *).
05/08/06   segu    Added support for RSSI optimazation
04/18/06    da     Modified code to support migration to Mar-06 Rel-5 ASN1 +
                   R6 Hacks
03/10/06    da     Added Initial support for PDCP under FEATURE_UMTS_PDCP
03/08/06   tkk/sk  Added deregistration of MVS-RLC queues on RAB reestablishment
                   timer expiry for CS voice RAB.
02/28/06  da/sm    Modified code to support migration to Dec-05 Rel-6 ASN1
02/23/06   ss/tkk  Added function "rrc_get_amr_mode_bitrate" to return the
                   AMR rate type (low bit rate or high bitrate) from AMR rate.
12/08/05   vk      Updated the function rrc_hsdpa_initial_validation() to remove
                   some of the old HSDPA validations to comply to spec CR 2393
                   for HSDPA. This would result in Reconfiguration Msg not
                   getting rejected if network specified the FDD information
                   in DL HS-PDSCH information for the next state to be non
                   HSDPA
11/30/05   da      Put the definition of local_rrc_rel_indicator under feature
                   FEATURE_REL_ID_NV_SUPPORT in func rrc_get_wcdma_rrc_version_nv.
                   Also included rrcnv.h under same feature.
11/23/05   vr      Bug fix in Interrat HO optimization support.Call IHO function to 
                   indicate release of Voice RAB.Earlier this was done from RBR/CU
                   but was not working because established RABs were cleared by then.
11/23/05   da      Fix for compiler warning on Saber
11/14/05   ttl     Remove define checking of NV_WCDMA_RRC_VERSION_I since it is an enum.
11/10/05   da      Put NV item NV_WCDMA_RRC_VERSION_I reading under feature FEATURE_REL_ID_NV_SUPPORT
11/10/05   sm      Added function rrc_get_wcdma_rrc_version_nv() to set global variable
                   rrc_nv_rel_indicator using NV Item NV_WCDMA_RRC_VERSION_I
11/04/05   vk      Update a debug statement for HSDPA
11/04/05  da/vr    Made changes to detect UE inactivity and take appropriate actions.
09/14/05   tkk     Added support for N312 & N315 Rel5 extensions.
09/12/05   ss      Moved the function of translating internal establishment cause to the 
                   cause transmitted over the air, hence added the function
                   rrc_translate_establishment_cause.
09/06/05   da      Took out rrcscmgr.h outside of FEATURE HSDPA in the header file list.
09/02/05   da      Added 3 new APIs to be used by upper layers.  
                   1.rrc_get_downlink_rb_rate() 2. rrc_get_uplink_rb_rate()
                   3.rrc_is_wcdma_active()
08/24/05   ss      Moved default config variables 0,1,3 from rrciho.c and added
                   default configurations 4,7,8 & 9.
08/21/05   bu      Updated rrc_bcch_and_nas_plmns_match() to use
                   sys_plmn_match() for NAS and BCCH PLMN comparison.
07/16/05   ss      Added support to translate N313 timer.
07/15/05   vk      Function rrc_translate_hrnti() now returns a SUCCESS/FAILURE
                   instead of void. Change name of element dl_rlc_um_rlc_l1_size_present
                   to dl_rlc_um_rlc_li_size_present
07/15/05   da      Added func rrc_get_lac_id() that returns the LAC id for serving cell.
07/-7/05   vr      Do not support Dynamic Power Class feature if GCF flag is turned ON
05/03/05   da      Added func rrc_get_gcf_flag_status that returns gcf_flag_status 
                   to MAC.
04/13/05   da      Added validation of N and K in SIB 5/6 if CTCH is present
04/06/05   vr      Include rrclogging.h unconditionally for logging on all platforms
04/06/05   da      Added new utility function is_ctch_present_in_serving_cell()
03/11/05   vk      Merged HSDPA code in Mainline from RRC_MSM6275 branch under FEATURE HSDPA
03/09/05   kc      Added support to translate N312, N315 from v3a0 ext.  Also added support
                   to update N312, N315 for v3a0 ext.
03/02/05  ttl      Support RRC ASN1 Release 5.
12/06/04   da      Added functions to check for rb-id validity:
                   validate_rb_id_in_reconfig_list() and validate_rb_id_in_release_list()  
11/23/04   sm      Added utility functions to get/set IE's in est-rabs struct
10/08/04   sk      Added a new function rrc_find_rab_for_rb_in_est_rabs() to get 
                   the rab_id associated with the rb id from established rab variable.
08/24/04   jh      Updated for new mvssup_wcdma_set_dl_channel() interface.
08/19/04   sgk     Removed  initialization of 
                   rrc_est_rabs.rabs[count].re_est_timer to RRC_INVALID_REEST_TMR
                   in function rrc_initialize_established_rabs as LHS is an
                   enum taking values 0,1 while RRC_INVALID_REEST_TMR is 2 
                   to fix lint error Type mismatch (assignment) (int/enum)
                   Changed rb_srch to type rrc_rb_search_e_type from boolean
                   in function rrc_find_rb_in_est_rabs to fix lint error 
                   Type mismatch (return) (enum {...} = unsigned char) and 
                   lint warning Converting enum to int.
                   Typecast cn_domain_id to rrc_CN_DomainIdentity during comparison 
                   in function rrc_check_rabs_associated_with_cn_domain 
                   to solve lint error 'bad type'.
                   Typecast  RRC_INT_ACCESS_CLASS_MAX_INDEX  to int in function
                   rrc_translate_access_class_barred_list to fix lint warning 
                   'Converting enum to int'.       
07/15/04   jh      Moved rrc_validate_frequency_info() to a different file.
07/13/04   sk      Added more debug prints to get rab info during addition
                   and clearing
07/02/04   ttl     Check the RB id when releasing MVS.
07/01/04   ttl     Rlease MVS when the corresponding RBs released.
06/16/04   ttl     Added tmc_heap_sib, tmc_heap_sib_mem_buffer.
06/11/04   jh      Fixed issue with not reinitializing return value in
                   rrc_validate_frequency_info().
06/04/04   jh      Re-enabled rrc_validate_frequency_info() and changed it
                   to use #defined constants instead of hardcoded numbers.
06/01/04   sk      Added clean up of RBs when clearing rab.
05/17/04   jh      Temporarily disabled rrc_validate_frequency_info().
05/12/04   jh      Fixed rrc_validate_frequency_info() to work when UL UARFCN
                   is not specified.
05/06/04   jh      Added function rrc_validate_frequency_info() to check
                   range of UL and DL UARFCN parameters.
04/20/04   sk      Added initialization of re_est_timer and  num_rbs_for_rab
                   fields during initialization
                   Added new function
                   rrc_get_rabs_in_established_rabs_for_given_domain(domain_id) 
                   that returns rabs for a particular domain
03/15/04   bu      Removed the use of FEATURE DUALMODE BASELINE. This feature is
                   always defined (even for singlemode builds).
02/11/04   svk     Added new function for URA-ID translation
12/04/03   vn      Added an array to hold timestamps at which the various
                   RRC signalling messages are received.
07/29/03   sk      Added a new function to find rb in established_rabs variable
06/05/03   bu      Fixed rrc_bcch_plmns_match().
06/02/03   bu      Fixed the comparison of BCCH PLMNs in rrc_bcch_plmns_match().
05/08/03   jh      Fixed rrc_return_cs_data_call_parms() to choose the largest
                   ntfi value instead of the first non-zero one.
04/07/03   sk      Fixed index overflowing problem while adding Radio Bearers
                   to RRC_EST_RABS.
02/14/03   kc      Changed feature defn from INTERRAT_HANDOVER_GTOW_RRC to 
                   INTERRAT_HANDOVER_GTOW
02/13/03   jh      Added rrc_translate_urnti_short() function to expand a
                   short U-RNTI into a normal one for processing by existing
                   functions.
02/05/03   sk      March2002Rel specific changes 
01/29/03   sk      Lint error cleanup (some typecastings to match the type
                   of lvalue)
12/18/02   bu      Corrected the MSG_HIGHs in rrc_bcch_and_nas_plmns_match().
12/12/02   jh      Updated NUM_OF_BITS_FOR_CCCH_MSG_TYPE and
                   NUM_OF_BITS_FOR_DCCH_MSG_TYPE to
                   NUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE and
                   NUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE.  Added
                   rrc_get_ul_ccch_message_type() and
                   rrc_get_ul_dcch_message_type() functions.
12/09/02   bu      Removed the checks for NAS and BCCH plmn being HPLMNs in 
                   rrc_bcch_and_nas_plmns_match().
10/14/02   jh      Fixed link error from using old rrcllc_is_ordered_config_set
                   function.
10/04/02   jh      Fixed rrc_return_cs_data_call_parms() to include proper
                   TrCH id to index mapping, correct selection of ordered
                   config vs. current config, and better error checking.
10/03/02   jh      Added rrc_return_cs_data_call_parms() function to return
                   the SDU size and number of SDUs per TTI for circuit-switched
                   data calls.
08/15/02   vn      Changed PLMN Comparison for Non-HPLMNs to compare only the
                   number of digits actually present in the PLMN coming in MIB.
07/10/02   rj      Included cmwll.h instead of cmll.h
06/13/02   rj      Removed rrc_u_rnti_internal and rrc_u_rnti_short from this
                   module since the same  information is available in the
                   ORDERED_CONFIG.
05/16/02   jh      Added AC info to global data.
04/09/02   rj      Added function to check if RABs are established for
                   any particular CN Domain. Fixed a bug in function
                   rrc_check_rbs_associated_with_re_est_timer.
04/08/02   vn      Added additional debug info in rrc_bcch_and_nas_plmns_match.
03/29/02   rj      Moved function  that appends START value to RRCSMC module.
03/25/02   rj      Updated functions rrc_translate_gsm_map_imsi and 
                   rrc_translate_imei to avoid memory leak if IE is directly
                   passed by reference from ASN1 decoded messsage.
03/05/02   rj      Renamed varible rrc_u_rnti to rrc_u_rnti_internal since 
                   VC++ debugger is confusing rrc_u_rnti variable with  
                   rrc_U_RNTI type declaration.
03/01/02   rj      Added code to return RRC state in function
                   rrc_translate_state_indicator_to_state
02/28/02   rj      Added functions to check presence of RABs associated with
                   a Re-establishment timer and to delete RBs associated with
                   a particular re-establishment timer. Also moved function
                   that appends START values from RRCRCE module since it is
                   common with Cell Update procedure. Added a function to
                   external RRC state indicator to internal RRC state. Also
                   Added Conversion function for Timer T305.
02/14/02   rj      Deleted variable rrc_c_rnti from RRCDATA module since 
                   Ordered Config maintains the same.
02/05/02   kc      Added var and function to update/initialize 
                   ue_conn_timers_and_constants
12/18/01   kc      Added functions to convert TMSI and PTMSI to integer.
12/12/01   vn      Convert IMSI, IMEI and Initial UE ID to a 64 bit value.
11/05/01   xfg     Removed the range check for RSSI, and always reports a 
                   RSSI to CM whenever the reporting timer (3 seconds) expires.
10/31/01   vn      Added new PLMN comparison utilities to compare between
                   NAS and BCCH PLMNs and between 2 BCCH PLMNs.
09/28/01   kmp     Fixed a bug in rrc_convert_gsm_map_imsi_into_int_value().
                   Fixed a bug in rrc_convert_imei_into_int_value().
09/18/01   xfg     Uncommented out l1_get_rssi() call in rrc_rssi_ber_timeout_handler
09/14/01   rj      Modified functions rrc_get_dl_dcch_message_type and 
                   rrc_get_dl_ccch_message_type to return message choice 
                   similar to rrcasn1.h
09/14/01   xfg     Added function rrc_rssi_ber_timeout_handler to process the
                   RSSI & BER reporting timeout
09/12/01   ram     In function rrc_return_lc_info_for_rab used the functions
                   rrclcm_get_dl_rlc_lc_id and rrclcm_get_ul_rlc_lc_id to get
                   the required logical channels.
08/30/01   rj      Moved functions rrc_get_dl_dcch_message_type
                   and rrc_get_dl_ccch_message_type from RRC dispatcher since
                   these are required by other RRC procedures. Added comments
                   in the conversion functions.
08/28/01   kiran   Added function rrc_log_protocol_error to support logging of
                   protocol errors.  Right now it supports only QXDM logging
08/13/01   ram     Added function rrc_return_lc_info_for_rab to support getting
                   logical channel ids for user-plane RABs when requested by
                   user-plane entities. Right now it supports only CS data and
                   PS rabs.
07/27/01   rj      Modified rrc_convert_initial_ue_identity_into_int_value 
                   function to use global data.
07/27/01   rj      Modified release cause type to internal format from the external
                   format. Modified IMSI, IMEI and Initial UE Id conversion
                   functions from internal types to integers.
07/25/01   rj      Added a function rrc_convert_gsm_map_imsi_into_int_value to
                   conevrt ASN1 format IMSI to an integer value to be used by
                   RRC LLC.
07/20/01   rj      Added a new variable for V304 counter.
05/29/01   kmp     Updated rrc_translate_urnti().
05/17/01   ram     The functions to access the variable ESTABLISHED_RABS were
                   using the RAB id as the index to access the array. This
                   won't work for higher RAB numbers, so updated all the
                   functions to use a loop to find the correct index.
                   Added a new function rrc_remove_rb_from_established_rabs
                   which removes one RB from the variable ESTABLISHED_RABS.
05/16/01   vn      Updated rrc_translate_gsm_map_plmn_id to work with the new
                   rrc_plmn_identity_type. 
05/08/01   ram     Corrected rrc_translate_cell_id function to work for all
                   bit lengths. Corrected dates in previous edit history lines.
04/30/01   rj      The Representation of variable rrc_capability_update_requirement
                   is modfied to internal format.
04/29/01   rj      Fixed error in previous check in.
04/29/01   rj      Modified definitions of C-RNTI and U-RNTI to internal format.
                   Added definitions for IMSI, TMSI and PTMSI. Added functions
                   to translate external format of  C-RNTI and U-RNTI into
                   internal format.
04/27/01   ram     Updated function rrc_translate_cell_id with the correct
                   interpretation of the ossBitString type given by ASN.1.
                   The function converts a bitstring of upto 32 bits into
                   a uint32 type.
04/26/01   ram     Added support for the UE Variable ESTABLISHED_RABS. 
                   Provided access functions for RRC procedures to 
                   add RABs to or remove RABs from this variable.
03/30/01   rj      Added functions rrc_translate_imei and 
                   rrc_translate_gsm_map_imsi. Modified RRC Connection 
                   Establishment cause and Initial UE Identity from external
                   format to internal.
03/23/01   ram     Added functions rrc_translate_gsm_map_plmn_id and rrc_translate
                   cell_id to translate the external (ASN.1) format of PLMN
                   and cell ids to the internal format used by RRC modules.
03/19/01   vn      Added rrc_translate_access_class_barred_list to translate 
                   access class barred list from ASN1 to RRC internal format.
02/12/01   rj      Added varibales rrc_protocol_error_information, rrc_v_308,
                   and rrc_release_cause for RRC Connection Release procedure.
01/25/01   rj      Moved rrc_dpch_info_present to RRCRCE module since is is
                   local to the RRCRCE module.
01/19/01   rj      Modified variables in this file global from LOCAL static.
12/22/00   rj      Added EDIT HISTORY Header
12/22/00   rj      Added variables for C-RNTI, U_RNTI, DPCH info present and 
                   capability update requirement that are required by the RRC
                   Connection Establishment procedure.
11/22/00   rj      Created file.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "wcdma_variation.h"
#include "rex.h"
#include "err.h"
#include "rrcllc.h"
#include "rrcllcoc.h"
#include "rrcdata_v.h"
#include "rrclcm.h"
#include "uecomdef.h"
#include "rrctmr.h"
#include "cmwll.h"
#include "rrclbt.h"
#include "l1rrcif.h"
#include "rrcmmif.h"
#include "rrcscmgr.h"
#include "rrclogging.h"
#include "bit.h"
#include "mmtask.h"
#include "rlcrrcif.h"
#include "rrcasu.h"

#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif

#ifdef FEATURE_INACTIVITY_HANDLING
#include "rrcmisc.h"
#include "rrcscmgr.h"
#endif /* FEATURE_INTACTIVITY_HANDLING */
#include "rrciho.h"
#include "rrcsibdb.h"
#include "mm_umts.h"
#include "rrcmcm.h"

#include "rrcnv.h"
#include "rrcccm.h"
#include "rrccspi.h"

#include "rrcmnif.h"
#include "rrcrbcommon.h"

#include "rlcdsapif.h"
#include "rrcasn1Def.h"

#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
#include "msgr_rex.h"
#include "rrccmd_v.h"
#include "rrcsigs.h"
#include "rrccmdi.h"
#endif

#include "rrcdormancy.h"
#ifdef FEATURE_DUAL_SIM
#include "sm.h"
#include "rrcrce.h"
#endif
#include "rrcrce.h"
#include "rrcrcr.c"
#include "rrcrbreconfig.h"
#include "rrctcreconfig.h"
#include "rrctfcc.h"
#include "rrcpcreconfig.h"
#include "rrcrbr.h"
#include "rrcsmc.h"
#include "rrccho.h"
#include "rrcasn1util.h"
#include "rrcdispatcher.h"
#include "macrrcif_v.h"

#ifdef FEATURE_WCDMA_SECURITY_SETTINGS
#include "rrcsmc.h"

#endif

#include "rrcmeas.h"
#include "sd.h"
#include "rfm_device_types.h"
#include "rfm.h"

#include "rcinit.h"
#include "rrcenhstatecfg.h"
#include "rrchsrachcfg.h"
#include "rrcueci.h"
#include "rrcumi.h"
#include "rrccui.h"
#include "rrcsibdbi.h"
#include "rrculog.h"
#include "rrcmeasi.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "wrrc_qsh_int.h"
#include "rrclog.h"
#include "wcdma_common_api.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define UNAVAILABLE_BER 0xff
#define RLC_AM_HDR_SIZE 16

#define RRC_ROUNDED_AVERAGE(sum,count) ((sum/count)+(((sum%count)<(count/2))?0:1))

/*===========================================================================

      Variables belonging to RRC layers        

===========================================================================*/
#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
#ifdef FEATURE_DUAL_WCDMA
  rrc_buffer_ota_info_type rrc_buffer_ota_info_sim[MAX_RRC_STACKS]={0};
#else
  rrc_buffer_ota_info_type rrc_buffer_ota_info;
#endif /*FEATURE_DUAL_WCDMA*/

#endif  /*FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME*/

/*.SSGCCS globals to update RRC information to QMI client.*/
ssgccs_3g_cell_info_msg_handler cell_info_3g_msg_handler = NULL;
ssgccs_3g_rrc_connection_request_msg_handler connection_request_3g_msg_handler = NULL;
ssgccs_3g_rrc_connection_reject_msg_handler connection_reject_3g_msg_handler = NULL;
ssgccs_3g_security_mode_command_msg_handler security_mode_command_3g_msg_handler = NULL;
ssgccs_3g_rrc_connection_release_msg_handler connection_release_3g_msg_handler = NULL;

/*Set this global to enable reading SIB7 in connecting state*/
boolean update_sib7_in_connecting = TRUE;

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#ifdef FEATURE_DUAL_WCDMA
  rrc_qsh_events rrc_qsh_events_db_in_use_sim[MAX_RRC_STACKS]={0};
#else
  rrc_qsh_events rrc_qsh_events_db={0};
#endif
#endif

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
#define WRRC_QSH_MAX_ACCUMULATED_RLS 255
#ifdef FEATURE_DUAL_WCDMA
uint8 num_rls_change_event_count_in_use_sim[MAX_RRC_STACKS]; /*maintained for updating qsh stats*/
rrc_qsh_ul_ota_param_s uplink_ota_in_use_sim[MAX_RRC_STACKS];
boolean rrc_qsh_dl_committed_in_use_sim[MAX_RRC_STACKS];
rrc_qsh_metric_db_type rrc_qsh_metric_db_in_use_sim[MAX_RRC_STACKS][WRRC_QSH_METRIC_MAX+1];
#define num_rls_change_event_count num_rls_change_event_count_in_use_sim[WAS_ID_M]

extern cell_info_type cell_id_dch_info_sim[MAX_RRC_STACKS];
#define cell_id_dch_info cell_id_dch_info_sim[WAS_ID_M]
#else
uint8 num_rls_change_event_count;
rrc_qsh_ul_ota_param_s uplink_ota;
boolean rrc_qsh_dl_committed;
rrc_qsh_metric_db_type rrc_qsh_metric_db[WRRC_QSH_METRIC_MAX+1];

/* 
   If dual wcdma is not enabled, cmapi configures both sub metrics
   Below global is used to save the start address and metric context ids for each sub
   Active W sub db will be copied to rrc_qsh_metric_db whenever DDS switch happens 
*/
#ifdef FEATURE_DUAL_SIM
rrc_qsh_metric_db_type rrc_qsh_metric_db_sub[WRRC_MAX_NUM_SUBS][WRRC_QSH_METRIC_MAX+1];
#endif
extern cell_info_type cell_id_dch_info;
#endif
#endif
#ifdef FEATURE_WCDMA_QSH_DUMP
uint8 *wrrc_dump_start_addr;
uint32 wrrc_dump_tag_mask;
#endif


#ifdef FEATURE_DUAL_WCDMA 
/* ESTABLISHED_RABS */
rrc_established_rabs_type rrc_est_rabs_sim[MAX_RRC_STACKS];

uint8 rrc_nv_rel_indicator_sim[MAX_RRC_STACKS];

#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
msgr_client_t wrrc_msgr_client_sim[MAX_RRC_STACKS];
msgr_id_t rrc_rex_q_id_sim[MAX_RRC_STACKS];
#endif

/* WCDMA RRC TCB handle */
static rex_tcb_type* wcdma_rrc_tcb_ptr_sim[MAX_RRC_STACKS] = {RCINIT_NULL};

rrcdata_misc_info_type rrcdata_misc_info_sim[MAX_RRC_STACKS];
rrcdata_timers_counters_info_type rrcdata_timers_counters_info_sim[MAX_RRC_STACKS];
rrcdata_logging_info_type rrcdata_logging_info_sim[MAX_RRC_STACKS];
rrcdata_sim_related_info_type rrcdata_sim_related_info_sim[MAX_RRC_STACKS];
rrcdata_hs_info_type rrcdata_hs_info_sim[MAX_RRC_STACKS];
rrc_cap_related_info_type rrc_cap_related_info_sim[MAX_RRC_STACKS];
rrcdata_codec_info_type rrcdata_codec_info_sim[MAX_RRC_STACKS];
#ifdef FEATURE_FEMTO_CSG
rrc_csg_int_data_type rrc_csg_int_data_sim[MAX_RRC_STACKS];
#endif

/* Externed Variables */
extern boolean nv_fake_security_enabled_sim[MAX_RRC_STACKS];

/* #defines to globals */
#define rrc_est_rabs rrc_est_rabs_sim[WAS_ID_M]
#define rrc_nv_rel_indicator rrc_nv_rel_indicator_sim[WAS_ID_M]
#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
#define wrrc_msgr_client wrrc_msgr_client_sim[WAS_ID_M]
#endif
#define wcdma_rrc_tcb_ptr wcdma_rrc_tcb_ptr_sim[WAS_ID_M]
#define rrcdata_misc_info rrcdata_misc_info_sim[WAS_ID_M]
#define rrcdata_timers_counters_info rrcdata_timers_counters_info_sim[WAS_ID_M]
#define rrcdata_logging_info rrcdata_logging_info_sim[WAS_ID_M]
#define rrcdata_sim_related_info rrcdata_sim_related_info_sim[WAS_ID_M]
#define rrcdata_hs_info rrcdata_hs_info_sim[WAS_ID_M]
#define rrc_cap_related_info rrc_cap_related_info_sim[WAS_ID_M]
#define rrcdata_codec_info rrcdata_codec_info_sim[WAS_ID_M]
#ifdef FEATURE_FEMTO_CSG
#define rrc_csg_int_data rrc_csg_int_data_sim[WAS_ID_M]
#endif
#define nv_fake_security_enabled nv_fake_security_enabled_sim[WAS_ID_M]

#else /* FEATURE_DUAL_WCDMA */
/* ESTABLISHED_RABS */
rrc_established_rabs_type rrc_est_rabs;
extern boolean nv_fake_security_enabled;
uint8 rrc_nv_rel_indicator;

#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
msgr_client_t wrrc_msgr_client;
#endif

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
#ifdef FEATURE_DUAL_WCDMA
extern rrc_sibdb_type sibdb_sim[MAX_RRC_STACKS];
extern rrc_csp_int_data_type  rrc_csp_int_data_sim[MAX_RRC_STACKS];
extern trm_unlock_event_enum_t rrcwrm_unlock_type_sim[MAX_RRC_STACKS];
extern rrc_dsds_int_data_type rrc_dsds_int_data_sim[MAX_RRC_STACKS];
extern rrc_qsh_ul_ota_param_s uplink_ota_in_use_sim[MAX_RRC_STACKS];
extern rrcllcoc_int_data_type rrcllcoc_int_data_sim[MAX_RRC_STACKS];
#else
extern rrc_sibdb_type sibdb;  
extern rrc_csp_int_data_type  rrc_csp_int_data;
#ifdef FEATURE_DUAL_SIM
extern trm_unlock_event_enum_t rrcwrm_unlock_type;
extern rrc_dsds_int_data_type rrc_dsds_int_data;
#endif
extern rrc_qsh_ul_ota_param_s uplink_ota;
extern rrcllcoc_int_data_type rrcllcoc_int_data;
#endif
#endif
/* WCDMA RRC TCB handle */
static rex_tcb_type* wcdma_rrc_tcb_ptr = RCINIT_NULL;

rrcdata_misc_info_type rrcdata_misc_info;
rrcdata_timers_counters_info_type rrcdata_timers_counters_info;
rrcdata_logging_info_type rrcdata_logging_info;
rrcdata_sim_related_info_type rrcdata_sim_related_info;
rrcdata_hs_info_type rrcdata_hs_info;
rrc_cap_related_info_type rrc_cap_related_info;
rrcdata_codec_info_type rrcdata_codec_info;
#ifdef FEATURE_FEMTO_CSG
rrc_csg_int_data_type rrc_csg_int_data;
#endif
#endif /* FEATURE_DUAL_WCDMA  */

/* Struct containing all the default configurations*/
rrc_default_config_info_type default_config_info[NUM_DEFAULT_CONFIGS];

#ifdef FEATURE_WCDMA_DEFAULT_CFG_CELLFACH

  /* Struct containing all the default configurations*/
const uint8 rrcrbrc_default_config_0_dc_fach[] = 
  {0x3A, 0x78, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0A, 0x81, 0x89, 0x03, 0x28, 0x40, 0x00, 0x48, 0x42, 0xC3, 0xFA, 0x18, 0x5B, 0xF1, 0xE0, 0x0D, 0x04, 0x32, 0x8C, 0x00, 0x14, 0x8C, 0x4C, 0x3F, 0xA1, 0x85, 0xBF, 0x1E, 0x00, 0xD0, 0x43, 0x29, 0x40, 0x02, 0x49, 0x00, };
  /*hardcode the number of bytes in default config 0 for FACH */
const uint8 rrcrbrc_default_config_0_len = 43;
#endif

  /*SIB16 format of saved Default Configuration 0 as specified in
    13.7 of 25.331
   */
const uint8 default_config_0[] = 
  { 
    0x14, 0x0D, 0x23, 0x40, 0x02, 0x10, 0x01, 0x0B,
    0x3D, 0x28, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40,
    0x0A, 0x50, 0x03, 0x13, 0x3D, 0x2A, 0x80, 0x44,
    0x56, 0x92, 0x43, 0x40, 0x12, 0x90, 0x04, 0x04,
    0x71, 0x06, 0x00, 0x60, 0x02, 0x90, 0x04, 0x00,
    0x00, 0x40, 0x00, 0x31, 0x29, 0x5F, 0x80, 0x00,
    0x80, 0x44, 0x11, 0x1C, 0xFB, 0x30, 0x40, 0x40,
    0xCA, 0x1F, 0xB2, 0x00
  
  };

  /*hardcode the number of bytes in default config 0*/
const uint8 default_config_0_len = 60;

const uint8 default_config_1[] = 
  {
    0x14, 0x0D, 0x23, 0x40, 0x02, 0x10, 0x01, 0x0B,
    0x3D, 0x28, 0x80, 0x40, 0x56, 0x92, 0x03, 0x40,
    0x0A, 0x50, 0x03, 0x13, 0x3D, 0x2A, 0x80, 0x40,
    0x56, 0x92, 0x03, 0x40, 0x12, 0x90, 0x04, 0x04,
    0x71, 0x06, 0x00, 0x60, 0x02, 0x90, 0x04, 0x00,
    0x00, 0x40, 0x00, 0x31, 0x29, 0x5F, 0x80, 0x00,
    0x00, 0x44, 0x11, 0x1C, 0xFB, 0x30, 0x40, 0x40,
    0xCA, 0x1F, 0xA8
  };

  /*hardcode the number of bytes in default config 1*/
const uint8 default_config_1_len = 59;

 

const uint8 default_config_3[] =
  {
    0x14, 0x0D, 0x23, 0x41, 0x82, 0x10, 0x61, 0x0B,
    0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x41,
    0x8A, 0x50, 0x63, 0x13, 0x3D, 0x2A, 0x80, 0x44,
    0x56, 0x92, 0x43, 0x41, 0x92, 0x90, 0x64, 0x84,
    0x71, 0x06, 0x00, 0x60, 0x00, 0x57, 0x10, 0x60,
    0x16, 0x00, 0x46, 0x71, 0x06, 0x02, 0x60, 0x0A,
    0x90, 0x14, 0x00, 0x00, 0x40, 0x20, 0x0C, 0x04,
    0x01, 0x41, 0x00, 0xB0, 0x12, 0x0A, 0x4B, 0x49,
    0x89, 0x35, 0x2B, 0x95, 0xF8, 0x30, 0x04, 0x42,
    0x88, 0x00, 0x27, 0x02, 0x0A, 0x20, 0x47, 0x8E,
    0x81, 0x10, 0x0C, 0xE1, 0x11, 0xDE, 0x80, 0x84,
    0x01, 0xE0, 0x44, 0x5D, 0x40, 0x32, 0x01, 0x10,
    0x44, 0x73, 0xEC, 0xC7, 0x00, 0x11, 0x00, 0x00,
    0x40, 0x9C, 0x08, 0x28, 0x81, 0x1E, 0x3A, 0xA8,
    0x06, 0x08, 0x14, 0x20, 0x38, 0x70, 0xE5, 0x40
  };

  /*hardcode the number of bytes in default config 3*/
const uint8 default_config_3_len = 120;

const uint8 default_config_4[] = 
  { 
    0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 
    0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 
    0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44, 
    0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
    0x71, 0x06, 0x00, 0x60, 0x00, 0x81, 0x01, 0x60, 
    0x91, 0x49, 0x24, 0xD2, 0x89, 0x52, 0x8F, 0x02, 
    0x01, 0x02, 0xD2, 0x88, 0x60, 0x2B, 0x36, 0x09, 
    0x00, 0x88, 0x22, 0x39, 0xF6, 0x61, 0x80, 0x81, 
    0x20, 0x0C, 0x19, 0x76, 0x98
 };
  
  /*hardcode the number of bytes in default config 7*/
const uint8 default_config_4_len = 69;
  

  /* Default config 6 ,  64kbps conv. CS- data + 3.4 kbps signalling */
const uint8 default_config_6[] = 
  { 
    0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 0x3D, 0x2A,
    0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 0x8A, 0x50, 0x23, 0x13,
    0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 
    0x24, 0x04, 0x71, 0x06, 0x00, 0x20, 0x00, 0x80, 0x00, 0xE2, 
    0x54, 0xC9, 0xCA, 0x3C, 0x08, 0x02, 0x0B, 0x5A, 0x12, 0x02, 
    0xA9, 0x60, 0x90, 0x08, 0x82, 0x23, 0x9F, 0x66, 0x18, 0x08, 
    0x12, 0x00, 0xC1, 0x97, 0x67, 0x80
  };
  
  /*hardcode the number of bytes in default config 6*/
const uint8 default_config_6_len = 66;

const uint8 default_config_7[] = 
  { 
    0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 
    0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 
    0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44, 
    0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
    0x71, 0x06, 0x00, 0x20, 0x00, 0x81, 0x00, 0xE0, 
    0x91, 0x49, 0x24, 0xCA, 0xFC, 0x08, 0x04, 0x0B, 
    0x4A, 0x11, 0x2A, 0x46, 0x09, 0x00, 0x88, 0x22, 
    0x39, 0xF6, 0x61, 0x80, 0x81, 0x58, 0x0C, 0x19, 
    0x7E, 0xBC
 };
  
  /*hardcode the number of bytes in default config 7*/
const uint8 default_config_7_len = 66;

const uint8 default_config_8[] = 
  { 
   0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 
   0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40,
   0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44, 
   0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
   0x71, 0x06, 0x00, 0x60, 0x00, 0x81, 0x01, 0x60, 
   0x91, 0x49, 0x24, 0xD2, 0x89, 0x52, 0x8F, 0x02, 
   0x01, 0x02, 0xD2, 0x88, 0x60, 0x29, 0xA6, 0x09, 
   0x00, 0x88, 0x22, 0x39, 0xF6, 0x61, 0x80, 0x81, 
   0x58, 0x0C, 0x19, 0x7E, 0x98
  };
 
  
  /*hardcode the number of bytes in default config 8*/
const uint8 default_config_8_len = 69;


const uint8 default_config_9[] = 
  { 
    0x14,0x0D,0x23,0x40,0x82,0x10,0x21,0x0B,
    0x3D,0x2A,0x80,0x44,0x56,0x92,0x43,0x40,
    0x8A,0x50,0x23,0x13,0x3D,0x2A,0x80,0x44,
    0x56,0x92,0x43,0x40,0x92,0x90,0x24,0x04,
    0x71,0x06,0x00,0x60,0x00,0x81,0x02,0x60,
    0x91,0x49,0x24,0xD2,0x89,0x54,0xB2,0x5D,
    0x30,0x99,0x28,0xF0,0x20,0x10,0x2D,0x29,
    0x06,0x08,0x62,0x29,0x06,0x09,0x00,0x88,
    0x22,0x39,0xF6,0x61,0x80,0x81,0x58,0x0C,
    0x19,0x7E,0x78
  };
  
  /*hardcode the number of bytes in default config 9*/
const uint8 default_config_9_len = 75;
  
const uint8 default_config_10[] = 
  { 
    0x14,0x0D,0x23,0x41,0x82,0x10,0x61,0x0B,0x3D,0x2A,
    0x80,0x44,0x56,0x92,0x43,0x41,0x8A,0x50,0x63,0x13,
    0x3D,0x2A,0x80,0x44,0x56,0x92,0x43,0x41,0x92,0x90,
    0x64,0x84,0x71,0x06,0x00,0x20,0x00,0x57,0x10,0x60,
    0x12,0x00,0x46,0x71,0x06,0x02,0x20,0x08,0x83,0x02,
    0xE0,0x09,0x01,0x48,0x42,0x43,0xD2,0x2C,0x93,0xB4,
    0x9E,0x24,0xF5,0x28,0x89,0x4B,0x4A,0x92,0x5D,0xCA,
    0xFC,0x18,0x02,0x51,0x44,0x00,0x13,0x81,0x02,0xA0,
    0x20,0x6E,0x04,0x12,0xC0,0x82,0x88,0x11,0xE3,0xA0,
    0x44,0x83,0x38,0x00,0x35,0x02,0x07,0xE0,0x41,0x50,
    0x08,0x33,0x81,0x1D,0xE8,0x08,0x42,0x1E,0x00,0x03,
    0xC0,0x22,0xEA,0x01,0x90,0x48,0x80,0x04,0x40,0x47,
    0x3E,0xCC,0x70,0x01,0x28,0x00,0x04,0x09,0xC0,0x81,
    0x50,0x10,0x37,0x02,0x09,0x60,0x41,0x44,0x08,0xF1,
    0xD5,0x40,0x30,0x40,0xA1,0x01,0xC3,0x97,0x2A,0x00
  };

  /*hardcode the number of bytes in default config 10*/
const uint8 default_config_10_len = 150;

/*
  Updated the data for Default Configs 11 and 12 as per Spec CR 2823
*/
const uint8 default_config_11[] = 
{
    /* This is basically dump of Rel-5 RB setup message
       with contents set to match default configuration 11.
       RB setup contains only relevant IEs which are of 
       interest to us -
          SRB Info to setup, RAB Info to setup, 
          UL & DL AddReconf Transport channel information,
          UL & DL common transport channel information, 
          UL Power, DL Common Info & DL Common Info per RL 
    */

    0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0xB1, 
    0xC0, 0xD2, 0x0D, 0x06, 0x08, 0x40, 0xC2, 0x16, 0x7A, 0x55, 
    0x00, 0x88, 0x03, 0x5A, 0x4D, 0x06, 0x83, 0x14, 0xA0, 0x63, 
    0x14, 0x23, 0x41, 0x92, 0x90, 0x32, 0x9C, 0xC0, 0x40, 0x40,
    0x00, 0x44, 0x23, 0x88, 0x1A, 0x00, 0x00, 0x80, 0x00, 0xAE,
    0x20, 0x68, 0x10, 0x02, 0x02, 0x03, 0x38, 0x81, 0xA0, 0x80,
    0x08, 0x10, 0x5C, 0x06, 0x05, 0xC0, 0x12, 0x02, 0x90, 0x84,
    0x87, 0xA4, 0x59, 0x27, 0x69, 0x3C, 0x49, 0xEA, 0x51, 0x12,
    0x96, 0x95, 0x24, 0xBB, 0x95, 0xF8, 0x01, 0x88, 0x16, 0x00,
    0x00, 0x20, 0x10, 0x06, 0x02, 0x00, 0xC0, 0x38, 0x0A, 0x04,
    0x01, 0x20, 0x50, 0x16, 0x20, 0x48, 0x00, 0x00, 0x80, 0x40,
    0x18, 0x0C, 0x03, 0x80, 0x80, 0x40, 0x12, 0x05, 0x08, 0x0E, 
    0x00, 0x00, 0x20, 0x10, 0x0C, 0x03, 0x80, 0x60, 0x40, 0x12, 
    0x20, 0x28, 0x00, 0x00, 0x80, 0xC0, 0x38, 0x04, 0x04, 0x0C, 
    0x01, 0x28, 0x82, 0x01, 0x09, 0xC0, 0xA1, 0x50, 0x14, 0x37, 
    0x02, 0x87, 0x40, 0x51, 0x04, 0x0A, 0xF1, 0xD0, 0x22, 0x41, 
    0x8C, 0x02, 0x1A, 0x81, 0x43, 0xF0, 0x28, 0x98, 0x05, 0x18, 
    0xC0, 0xAE, 0xF4, 0x04, 0x20, 0x0A, 0x02, 0x2A, 0xEA, 0x01, 
    0x90, 0x08, 0x82, 0x2B, 0x9F, 0x6A, 0x86, 0x0B, 0x80, 0x00, 
    0x20, 0x80, 0x78, 0x58, 0x76, 0x3C, 0x1E, 0x91, 0x09, 0x65, 
    0x23, 0xB9, 0xE0, 0xF2, 0x80, 0x43, 0xA3, 0x96, 0x6B, 0x45, 
    0xAA, 0xF1, 0x86, 0xCA, 0x77, 0x92, 0x00, 0x12, 0x80, 0x00, 
    0x50, 0x9C, 0x0A, 0x15, 0x01, 0x43, 0x70, 0x28, 0x74, 0x05, 
    0x10, 0x40, 0xAF, 0x1D, 0x57, 0x02, 0x83, 0x5C, 0x12, 0x15, 
    0x70, 0x68, 0x75, 0xC2, 0x02, 0x00, 0x0C, 0x22, 0xBC, 0x75, 
    0x5C, 0xA4, 0x1A, 0x70, 0x08, 0x00, 0x14, 0xD4, 0xD9, 0x08, 
    0x05, 0x0A, 0x00, 0x81, 0x00, 0x20, 0x20, 0x20, 0x69, 0x98, 
    0x00
};
  
/*hardcode the number of bytes in default config 11*/
const uint32 default_config_11_len = 271; 

const uint8 default_config_12[] = 
{
    /* This is basically dump of Rel-5 RB setup message
       with contents set to match default configuration 12.
       RB setup contains only relevant IEs which are of 
       interest to us -
          SRB Info to setup, RAB Info to setup, 
          UL & DL AddReconf Transport channel information,
          UL & DL common transport channel information, 
          UL Power, DL Common Info & DL Common Info per RL 
    */
    0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0xB1,
    0xC0, 0xD2, 0x0D, 0x04, 0x08, 0x40, 0x82, 0x16, 0x7A, 0x55,
    0x00, 0x88, 0x03, 0x5A, 0x4D, 0x06, 0x82, 0x14, 0xA0, 0x43,
    0x14, 0x23, 0x41, 0x12, 0x90, 0x22, 0x98, 0xC0, 0x40, 0x30, 
    0x00, 0x42, 0x23, 0x88, 0x1A, 0x00, 0x00, 0x80, 0x00, 0xAE, 
    0x20, 0x68, 0x10, 0x02, 0x02, 0x17, 0x01, 0x81, 0x70, 0x04, 
    0x80, 0xA4, 0x21, 0x21, 0xE9, 0x16, 0x48, 0xEA, 0x47, 0x92,
    0x3E, 0x92, 0x64, 0x96, 0xA4, 0xD1, 0x27, 0x65, 0x7E, 0x00,
    0x62, 0x05, 0x80, 0x00, 0x08, 0x04, 0x01, 0x80, 0x80, 0x30, 
    0x0E, 0x02, 0x81, 0x00, 0x48, 0x14, 0x05, 0x88, 0x12, 0x00, 
    0x00, 0x20, 0x10, 0x06, 0x03, 0x00, 0xE0, 0x20, 0x10, 0x04, 
    0x81, 0x42, 0x03, 0x80, 0x00, 0x08, 0x04, 0x03, 0x00, 0xE0, 
    0x18, 0x10, 0x04, 0x88, 0x0A, 0x00, 0x00, 0x20, 0x30, 0x0E,
    0x01, 0x01, 0x02, 0x00, 0x4A, 0x1E, 0x80, 0x42, 0x70, 0x28, 
    0x54, 0x05, 0x0D, 0xC0, 0xA1, 0xD0, 0x14, 0x3D, 0x02, 0xBC, 
    0x74, 0x08, 0x90, 0x57, 0x00, 0x86, 0xA0, 0x50, 0xFC, 0x0A,
    0x26, 0x01, 0x45, 0x70, 0x2B, 0xBD, 0x01, 0x10, 0x08, 0x82,
    0x2B, 0x9F, 0x6A, 0x86, 0x0B, 0x80, 0x00, 0x20, 0x80, 0x78, 
    0x58, 0x3A, 0x1E, 0x0F, 0x89, 0x85, 0xA3, 0x41, 0xD8, 0xF0,
    0x7A, 0x44, 0x25, 0x94, 0x8B, 0x25, 0xA2, 0xD9, 0x88, 0xD2, 
    0x70, 0x3B, 0x8E, 0x00, 0x12, 0x80, 0x00, 0x50, 0x9C, 0x0A, 
    0x15, 0x01, 0x43, 0x70, 0x28, 0x74, 0x05, 0x0F, 0x40, 0xAF, 
    0x1D, 0x57, 0x02, 0x83, 0x5C, 0x12, 0x15, 0x60, 0x60, 0x80, 
    0x03, 0x08, 0xAF, 0x1D, 0x4A, 0x41, 0xA7, 0x00, 0x80, 0x01, 
    0x4D, 0x4D, 0x90, 0x80, 0x50, 0xA0, 0x08, 0x10, 0x02, 0x02, 
    0x02, 0x06, 0x99, 0x80

};

/*hardcode the number of bytes in default config 12 */
const uint32 default_config_12_len = 254;

const uint8 default_config_13[] =
{
  0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0xA1,
  0xC0, 0xD2, 0x0D, 0x06, 0x08, 0x40, 0xC2, 0x16, 0x7A, 0x55,
  0x00, 0x88, 0x03, 0x5A, 0x49, 0x06, 0x83, 0x14, 0xA0, 0x63,
  0x14, 0x23, 0x41, 0x92, 0x90, 0x32, 0x9C, 0xC0, 0x40, 0x40,
  0x00, 0x44, 0x23, 0x88, 0x1A, 0x00, 0x08, 0x80, 0x00, 0xAE, 
  0x20, 0x68, 0x10, 0x22, 0x02, 0x03, 0x38, 0x81, 0xA0, 0x80, 
  0x88, 0x10, 0x5C, 0x04, 0x04, 0xC0, 0x48, 0x29, 0x1D, 0x26, 
  0xA5, 0x34, 0xA8, 0x95, 0x52, 0xDA, 0x61, 0x4C, 0xE5, 0x7E, 
  0x00, 0x42, 0x02, 0x80, 0x00, 0x08, 0x04, 0x02, 0x80, 0xC0, 
  0x38, 0x80, 0xE0, 0x00, 0x02, 0x01, 0x00, 0x60, 0x28, 0x0C, 
  0x03, 0x81, 0x02, 0x04, 0x80, 0x00, 0x08, 0x04, 0x01, 0x80, 
  0x80, 0x28, 0x0C, 0x03, 0x81, 0x00, 0x48, 0xC0, 0x12, 0x09, 
  0x00, 0x00, 0xA0, 0x08, 0x1B, 0x01, 0x04, 0x00, 0x20, 0x90, 
  0x04, 0x78, 0xE8, 0x11, 0x19, 0xB4, 0x00, 0x09, 0xC0, 0x41, 
  0xC4, 0x08, 0x6D, 0x00, 0x8E, 0xF4, 0x04, 0x20, 0x0F, 0x00, 
  0x0F, 0xFC, 0x06, 0x40, 0x22, 0x08, 0x8E, 0xA5, 0xAA, 0x18, 
  0x26, 0x00, 0x00, 0x81, 0xC1, 0xA1, 0x30, 0xA0, 0x54, 0x36, 
  0x21, 0x13, 0x8A, 0x05, 0x22, 0xF1, 0xA8, 0xEC, 0x78, 0x3D, 
  0x21, 0x92, 0x49, 0xE4, 0x80, 0x04, 0x80, 0x00, 0x10, 0x28, 
  0x02, 0x06, 0xC0, 0x41, 0x00, 0x08, 0x24, 0x01, 0x1E, 0x3A, 
  0xAC, 0x05, 0x04, 0x09, 0x08, 0x0D, 0x0E, 0x10, 0x10, 0x00, 
  0x61, 0x11, 0xE6, 0x1A, 0xE5, 0x20, 0xD3, 0x84, 0x40, 0x00,
  0xA6, 0xA6, 0xC8, 0x40, 0x28, 0x50, 0x04, 0x00, 
};
const uint32 default_config_13_len =228;

const uint8 default_config_14[] = 
{
    /* This is basically dump of Rel-5 RB setup message
       with contents set to match default configuration 14.
       RB setup contains only relevant IEs which are of 
       interest to us -
          SRB Info to setup, RAB Info to setup, 
          UL & DL AddReconf Transport channel information,
          UL & DL common transport channel information, 
          UL Power, DL Common Info  
    */

 0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x54, 0xA1,
 0xC0, 0xD2, 0x0D, 0x10, 0x08, 0x42, 0x02, 0x16, 0x7C, 0x33, 
 0x0B, 0x26, 0x3C, 0x03, 0x38, 0x4C, 0x34, 0x40, 0xA5, 0x08, 
 0x18, 0xA1, 0x1A, 0x20, 0x94, 0x84, 0x14, 0xE6, 0x02, 0x02, 
 0x00, 0x02, 0x21, 0x1C, 0x40, 0xC0, 0x04, 0x00, 0x05, 0x71, 
 0x03, 0x00, 0x90, 0x01, 0x19, 0xC4, 0x0C, 0x04, 0x40, 0x0A, 
 0xE0, 0x30, 0x2E, 0x00, 0x90, 0x14, 0x84, 0x24, 0x3D, 0x22,
 0xC9, 0x3B, 0x49, 0xE2, 0x4F, 0x52, 0x88, 0x94, 0xB4, 0xA9, 
 0x25, 0xDC, 0xAF, 0xC0, 0x0C, 0x40, 0x50, 0x00, 0x01, 0x00, 
 0x80, 0x60, 0x1C, 0x08, 0x10, 0x1C, 0x00, 0x00, 0x40, 0x20, 
 0x0C, 0x06, 0x01, 0xC0, 0x80, 0x24, 0x40, 0x90, 0x00, 0x01, 
 0x00, 0x80, 0x30, 0x10, 0x06, 0x01, 0xC0, 0x80, 0x24, 0x0A, 
 0x10, 0x2C, 0x00, 0x00, 0x40, 0x20, 0x0C, 0x04, 0x01, 0x40, 
 0x60, 0x1C, 0x08, 0x02, 0x40, 0xA0, 0x2C, 0x60, 0x09, 0x45, 
 0x10, 0x00, 0x4E, 0x04, 0x0A, 0x80, 0x81, 0xB8, 0x10, 0x3D,
 0x02, 0x0A, 0x20, 0x47, 0x8E, 0x81, 0x12, 0x0C, 0xE0, 0x00, 
 0xD4, 0x08, 0x1F, 0x81, 0x05, 0x70, 0x20, 0xCE, 0x04, 0x77, 
 0xA0, 0x21, 0x08, 0x78, 0x00, 0x0F, 0x00, 0x8B, 0xA8, 0x10, 
 0x40, 0x22, 0x08, 0x8E, 0x7D, 0xAA, 0x18, 0x2F, 0x00, 0x48,
 0x0A, 0x42, 0x12, 0x1E, 0x91, 0x64, 0x9D, 0xA4, 0xF1, 0x27,
 0xA9, 0x44, 0x4A, 0x5A, 0x54, 0x92, 0xEE, 0x97, 0x84, 0xBC,
 0xA6, 0x01, 0x30, 0xE9, 0x8E, 0x4D, 0x9A, 0x6D, 0x13, 0x6A,
 0x9B, 0xC4, 0xE1, 0xA7, 0x29, 0x3D, 0xE5, 0x7E, 0x12, 0x00,
 0x12, 0x80, 0x00, 0x40, 0x9C, 0x08, 0x15, 0x01, 0x03, 0x70, 
 0x20, 0x7A, 0x04, 0x14, 0x40, 0x8F, 0x1D, 0x56, 0x02, 0x82,
 0x04, 0x84, 0x10, 0x90, 0x08, 0x08, 0x00, 0x30, 0x88, 0xF3,
 0x0A, 0x0D, 0x38, 0x44, 0x00, 0x0A, 0x6A, 0xEC, 0x80, 0x0A,
 0x14, 0x01, 0x00 

};

/*hardcode the number of bytes in default config 14 */
const uint32 default_config_14_len = 273;


/* Default config 15,  7.95 kbps speech +   3.4 kbps signalling */
const uint8 default_config_15[] = 
{
    0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19, 
    0x85, 0x93, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
    0x99, 0xF0, 0xCC, 0x2C, 0x98, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
    0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x15, 0xC4, 
    0x18, 0x05, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x98, 0x02, 0x20, 
    0x40, 0x58, 0x24, 0x52, 0xA9, 0x64, 0xBA, 0x6C, 0xAF, 0xC1, 
    0x80, 0x22, 0x12, 0xC0, 0x01, 0x38, 0x10, 0x4B, 0x02, 0x3C, 
    0x74, 0x08, 0x80, 0x54, 0x08, 0x8E, 0xF4, 0x04, 0x20, 0x0F, 
    0x00, 0x0B, 0xA8, 0x10, 0x40, 0x22, 0x08, 0x8E, 0x7D, 0x98, 
    0xE0, 0x02, 0x20, 0x00, 0x08, 0x13, 0x81, 0x04, 0xB0, 0x23, 
    0xC7, 0x55, 0x00, 0xC1, 0x02, 0x84, 0x11, 0x22, 0x5E, 0xA8, 
};


/*hardcode the number of bytes in default config 15 */
const uint8 default_config_15_len = 110;
/* Default config 18, 12.2 kbps speech + 13.6 kbps signalling */
const uint8 default_config_18[] = 
{
    0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3D, 0x19, 
    0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
    0x99, 0xE8, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
    0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x15, 0xC4, 
    0x18, 0x05, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x98, 0x02, 0x20, 
    0x80, 0xA8, 0x09, 0x05, 0x25, 0xA4, 0xC4, 0x9A, 0x95, 0xCA, 
    0xBC, 0xB0, 0x96, 0x53, 0x22, 0x65, 0x4E, 0x08, 0x30, 0x04, 
    0x42, 0x88, 0x00, 0x27, 0x02, 0x0A, 0x20, 0x47, 0x8E, 0x81, 
    0x10, 0x0C, 0xE1, 0x11, 0xDE, 0x80, 0x84, 0x01, 0xE0, 0x44, 
    0x5D, 0x40, 0x82, 0x01, 0x11, 0x06, 0x08, 0x62, 0x1C, 0xFB, 
    0x31, 0xC0, 0x04, 0x40, 0x00, 0x10, 0x27, 0x02, 0x0A, 0x20, 
    0x47, 0x8E, 0xAA, 0x01, 0x82, 0x05, 0x08, 0x22, 0x44, 0xB7, 
    0x56, 
};

/*hardcode the number of bytes in default config 18 */
const uint8 default_config_18_len = 121;

/* Default config 19, 12.2/7.95/5.9/4.75  kbps speech +
  * 13.6 kbps signalling
  */
const uint8 default_config_19[] = 
{
    0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3D, 0x19, 
    0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
    0x99, 0xE8, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
    0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x08, 0x00, 0x15, 0xC4, 
    0x18, 0x04, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x88, 0x02, 0x20, 
    0xC1, 0x08, 0x02, 0x40, 0x52, 0x10, 0x90, 0xF4, 0x8B, 0x24, 
    0xED, 0x27, 0x89, 0x3D, 0x4A, 0x22, 0x52, 0xD2, 0xA4, 0x97, 
    0x72, 0xAF, 0x2F, 0x09, 0x79, 0x4D, 0xA2, 0x6D, 0x53, 0xE0, 
    0x83, 0x00, 0x4A, 0x28, 0x80, 0x02, 0x70, 0x20, 0x54, 0x04, 
    0x0D, 0xC0, 0x82, 0x58, 0x10, 0x51, 0x02, 0x3C, 0x74, 0x08, 
    0x90, 0x67, 0x00, 0x06, 0xA0, 0x40, 0xFC, 0x08, 0x2A, 0x01, 
    0x06, 0x70, 0x23, 0xBD, 0x01, 0x08, 0x43, 0xC0, 0x00, 0x78, 
    0x04, 0x5D, 0x40, 0x82, 0x09, 0x10, 0x00, 0x88, 0x6C, 0x10, 
    0xC4, 0x39, 0xF6, 0x63, 0x80, 0x09, 0x40, 0x00, 0x20, 0x4E, 
    0x04, 0x0A, 0x80, 0x81, 0xB8, 0x10, 0x4B, 0x02, 0x0A, 0x20, 
    0x47, 0x8E, 0xAA, 0x01, 0x82, 0x05, 0x08, 0x22, 0x44, 0xB7, 
    0x56, 
};

/*hardcode the number of bytes in default config 18 */
const uint8 default_config_19_len = 161;
/* Default config 22,  13.6 kbps signalling */

const uint8 default_config_16[] = 
{
    0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19,
    0x85, 0x93, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
    0x99, 0xF0, 0xCC, 0x2C, 0x98, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
    0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x15, 0xC4, 
    0x18, 0x05, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x98, 0x02, 0x20, 
    0x80, 0x98, 0x09, 0x05, 0x23, 0xA4, 0xD4, 0xA6, 0x95, 0x12, 
    0xAA, 0x5B, 0x4C, 0x29, 0x9C, 0xAF, 0xC1, 0x80, 0x24, 0x12, 
    0x00, 0x01, 0x40, 0x10, 0x36, 0x02, 0x08, 0x00, 0x41, 0x20, 
    0x08, 0xF1, 0xD0, 0x22, 0x33, 0x68, 0x00, 0x13, 0x80, 0x83, 
    0x88, 0x10, 0xDA, 0x01, 0x1D, 0xE8, 0x08, 0x40, 0x1E, 0x00, 
    0x1F, 0xF8, 0x20, 0x80, 0x44, 0x11, 0x1D, 0x4B, 0x31, 0xC0, 
    0x04, 0x80, 0x00, 0x10, 0x28, 0x02, 0x06, 0xC0, 0x41, 0x00, 
    0x08, 0x24, 0x01, 0x1E, 0x3A, 0xA8, 0x06, 0x08, 0x14, 0x20, 
    0x89, 0x12, 0xDD, 0x40
    
};

/*hardcode the number of bytes in default config 16 */
const uint8 default_config_16_len = 134;

/* Default config 20 ,  64kbps conv. CS- data + 13.6 kbps signalling */

const uint8 default_config_20[] = 
{
  0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19, 
  0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
  0x99, 0xF0, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
  0xA4, 0x41, 0x01, 0x1C, 0x41, 0x80, 0x08, 0x00, 0x20, 0x40, 
  0x68, 0x24, 0x52, 0x49, 0x32, 0x8F, 0x28, 0x96, 0x4C, 0x20, 
  0x40, 0x10, 0x5A, 0xD0, 0x90, 0x15, 0x4B, 0x20, 0x80, 0x44, 
  0x41, 0x82, 0x18, 0x87, 0x3E, 0xCC, 0x30, 0x10, 0x24, 0x08, 
  0x91, 0x2E, 0x4F, 
};

/*hardcode the number of bytes in default config 20 */
const uint8 default_config_20_len = 73;

const uint8 default_config_21[] = 
{
  0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19, 
  0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
  0x99, 0xF0, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
  0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x15, 0xC4, 
  0x18, 0x05, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x98, 0x02, 0x20, 
  0xC0, 0xE8, 0x02, 0x40, 0x52, 0x0E, 0x90, 0xD4, 0x89, 0xA4, 
  0x51, 0x22, 0xA9, 0x1B, 0x49, 0x0A, 0x49, 0xCA, 0xBC, 0x94, 
  0x24, 0xA5, 0x27, 0x89, 0x3D, 0x4A, 0x82, 0x0C, 0x01, 0x20, 
  0x90, 0x00, 0x0A, 0x00, 0x81, 0xB0, 0x10, 0x40, 0x02, 0x09, 
  0x00, 0x47, 0x8E, 0x81, 0x11, 0x9B, 0x40, 0x00, 0x9C, 0x04, 
  0x1C, 0x40, 0x86, 0xD0, 0x08, 0xEF, 0x40, 0x42, 0x00, 0xF0, 
  0x00, 0xFF, 0xC1, 0x04, 0x02, 0x22, 0x0C, 0x10, 0xC4, 0x3A, 
  0x96, 0x63, 0x80, 0x09, 0x00, 0x00, 0x20, 0x50, 0x04, 0x0D, 
  0x80, 0x82, 0x00, 0x10, 0x48, 0x02, 0x3C, 0x75, 0x50, 0x0C, 
  0x10, 0x28, 0x41, 0x12, 0x25, 0xBA, 0xB0, 
};

/*hardcode the number of bytes in default config 21 */
const uint8 default_config_21_len = 147;

/* Default config 22,  13.6 kbps signalling */
const uint8 default_config_22[] = 
{
    0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19,
    0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
    0x99, 0xF0, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
    0xA4, 0x41, 0x01, 0x1C, 0x41, 0x80, 0x18, 0x00, 0xA4, 0x01, 
    0x00, 0x00, 0x10, 0x00, 0x0C, 0x4A, 0x57, 0xE0, 0x08, 0x00, 
    0x11, 0x04, 0x47, 0x3E, 0xCC, 0x11, 0x12, 0x2B, 0x97, 0xEA,
    0x00
};
/*hardcode the number of bytes in default config 22 */
const uint8 default_config_22_len = 61;

const uint8 default_config_17[] = 
{
    /* This is basically dump of Rel-6 RB setup message
       with contents set to match default configuration 17.
       RB setup contains only relevant IEs which are of 
       interest to us -
          SRB Info to setup,
          UL & DL AddReconf Transport channel information,
    */

    0x3A, 0x60, 0x00, 0x00, 0x04, 0x02, 0x54, 0x4C, 0x7C, 0x53, 0x0B, 0x12, 0x1C, 0x21, 0x58, 0x4C, 0x34, 0x80, 0x60, 0x20, 0x4C, 0x05, 
    0x0B, 0x1F, 0x14, 0xC2, 0xC4, 0x87, 0x08, 0x56, 0x13, 0x0D, 0x10, 0x10, 0x08, 0x0B, 0x00, 0xC0, 0xC9, 0x03, 0x40, 0x02, 0x02, 0x00, 
    0xC0, 0x00, 0xF1, 0xE2, 0x28, 0x00, 0x02, 0x84, 0x0A, 0xEA, 0x10, 0x25, 0x80, 0x49, 0x80
};

/*hardcode the number of bytes in default config 17 */
const uint8 default_config_17_len = 59;

#ifdef FEATURE_WCDMA_REL10
const uint8 default_config_24[] = 
{
    0x14, 0x0D, 0x23, 0x41, 0x82, 0x10, 0x61, 0x0B, 0x3D, 0x2A, 0x80, 
    0x74, 0x56, 0x92, 0x43, 0x41, 0x8A, 0x50, 0x63, 0x13, 0x3D, 0x2A, 
    0x80, 0x74, 0x56, 0x92, 0x43, 0x41, 0x92, 0x90, 0x64, 0x84, 0x71, 
    0x06, 0x00, 0x20, 0x00, 0x57, 0x10, 0x60, 0x12, 0x00, 0x46, 0x71, 
    0x06, 0x02, 0x20, 0x08, 0x83, 0x02, 0xE0, 0x09, 0x01, 0x48, 0x42, 
    0x43, 0xD2, 0x2C, 0x93, 0xB4, 0x9E, 0x24, 0xF5, 0x28, 0x89, 0x4B, 
    0x4A, 0x92, 0x5D, 0xCA, 0xFC, 0x18, 0x02, 0x51, 0x44, 0x00, 0x13, 
    0x81, 0x02, 0xA0, 0x20, 0x6E, 0x04, 0x0F, 0x40, 0x82, 0x88, 0x11, 
    0xE3, 0xA0, 0x44, 0x83, 0x38, 0x00, 0x35, 0x02, 0x07, 0xE0, 0x41, 
    0x5C, 0x08, 0x33, 0x81, 0x1D, 0xE8, 0x08, 0x42, 0x1E, 0x00, 0x03, 
    0xC0, 0x22, 0xEA, 0x01, 0x90, 0x48, 0x80, 0x04, 0x40, 0x47, 0x3E, 
    0xCC, 0x70, 0x01, 0x28, 0x00, 0x04, 0x09, 0xC0, 0x81, 0x50, 0x10, 
    0x37, 0x02, 0x09, 0x60, 0x41, 0x44, 0x08, 0xF1, 0xD5, 0x40, 0x30, 
    0x40, 0xA1, 0x01, 0xC3, 0x97, 0x2A, 0x00, 
};
/*hardcode the number of bytes in default config 24 */
const uint8 default_config_24_len = 150;
#endif
/* variable to indicate WCDMA RRC RELEASE (REL99/REL-5/REL-6).
 * This variable will be set using the value of NV item 'NV_WCDMA_RRC_VERSION_I'
 */

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */



/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this �. 
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */

/*============================================================================
FUNCTION: rrcllc_get_64QAM_status()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

boolean rrcllc_get_64QAM_status
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  
  ordered_config_type *config_ptr;
  
  if(config_ptr_in_use == ORDERED_CONFIG)
  {
    config_ptr = ordered_config_ptr;
  }
  else
  {
    config_ptr = current_config_ptr;
  }
  
  WRRC_MSG1_MED("64QAM:64QAM Status is  %d",config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured);
  return(config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured);
}
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION rrc_return_hsdpa_category_value

DESCRIPTION
   RRC inform L1 about the HS cat to be used
   If Cat is below 12, 
       always send the HS category as Set.
   If the CAT is above 12
        If MAC-HS is set then send the HS cat as 8 for MAC-HS
          else
        According to spec  CR 3812
        If MAC-EHS is set
             If Cat is 24 and DC_HSDPA is ACTIVE  we indicate the CAT to  L1 as 24
                   else
             we indicate the rrc_macehs_cat(which is based on Feature defined) as CAT to L1 
             If CAT is 18
                 If MIMO is active we indicate HS cat as 16(Only MIMO used not 64 QAM)
                   else
                 If MIMO is not active send HS cat as 14 (Only 64QAM used not MIMO).

  
DEPENDENCIES
  None.
  
RETURN VALUE
  HS cat to be used.
  
SIDE EFFECTS
  None.

===========================================================================*/
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

uint8 rrc_return_hsdpa_category_value
(
  sys_modem_as_id_e_type wrrc_as_id,
  mac_hs_e_type machs_or_ehs
)
{
  uint8 nv_cat = rrcnv_data.nv_hsdpa_category;

#ifdef FEATURE_WCDMA_DC_HSDPA
  ordered_config_type *config_ptr;
  uint8 num_sec_carriers = 0;
  if(config_ptr_in_use == ORDERED_CONFIG)
  {
    config_ptr = ordered_config_ptr;
  }
  else
  {
    config_ptr = current_config_ptr;
  }
  num_sec_carriers = rrcllcpcie_return_num_sec_hsdpa_carriers(wrrc_as_id,config_ptr);
#endif  
  
  if(machs_or_ehs == MAC_EHS)
  {
#ifdef FEATURE_WCDMA_DC_HSDPA
#ifdef FEATURE_WCDMA_3C_HSDPA
    if(rrcnv_data.nv_hsdpa_category == 29) 
    {  
      if(num_sec_carriers == 0)
      {  
        nv_cat= rrc_macehs_cat;
      }
      else if(num_sec_carriers == NUM_OF_SEC_CARRIERS_2C)
      {
        nv_cat = 24;
      }
    }
    else
#endif /*FEATURE_WCDMA_3C_HSDPA*/
    if(rrcnv_data.nv_hsdpa_category == 24) 
    {                    
      if (num_sec_carriers == 0)
      {
        nv_cat= rrc_macehs_cat;
      }
    }
    else
#endif /*FEATURE_WCDMA_DC_HSDPA*/
    {
      if (rrcnv_data.nv_hsdpa_category > rrc_macehs_cat)
      {
        nv_cat= rrc_macehs_cat; 
      }
    }
#ifdef FEATURE_WCDMA_MIMO

    if(nv_cat == 18)
    {
      l1_dl_hs_mimo_action_enum_type mimo_action 
                    =rrcllc_get_mimo_action_in_config(wrrc_as_id);
      if(((rrcllc_get_mimo_status(wrrc_as_id) == RRC_MIMO_ACTIVE) && (mimo_action != L1_MIMO_STOP) ) ||
          (mimo_action == L1_MIMO_START)
            )
      {
        nv_cat= 16; 
      }
      else
      {
        nv_cat= 14; 
      }
    }    
#endif
  }
  /*If MAc-hs call is configured, the HSDPA CAT can be either */
  else if (machs_or_ehs == MAC_HS)
  {
    if(rrcnv_data.nv_hsdpa_category > 12 )
    {
#ifdef FEATURE_WCDMA_DC_HSDPA
      if((rrcnv_data.nv_hsdpa_category >= 24) && (config_ptr->l1_hsdpa_info.num_sec_hs_carrier != 0))
        {                    
          if(config_ptr->l1_hsdpa_info.num_sec_hs_carrier == NUM_OF_SEC_CARRIERS_2C)
        {
          nv_cat = 24;
        }
      }
      else
#endif /*FEATURE_WCDMA_DC_HSDPA*/
      {
        /*Keeping it 8 for now, needs to be changed to 10 later*/
        nv_cat = rrc_machs_cat;
      }
    }
    else
    {
      nv_cat= rrcnv_data.nv_hsdpa_category;
    }
  }
  else
  {
    WRRC_MSG1_ERROR("Invalid HS type passed by l1 %d",machs_or_ehs);
  }
  
  WRRC_MSG3_MED("HS Cat returned to l1 %d RRC NV cat ver %d, MAC HS type %d ",nv_cat,rrcnv_data.nv_hsdpa_category, machs_or_ehs);

  return nv_cat;
}
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


/*===========================================================================

FUNCTION rlc_rrc_req_to_register_for_data_ind

DESCRIPTION
   RLC informs RRC about the list of logical channels registered for watermark 
  in Cell_PCH/Ura_PCH states.RRC will then send CRLC_UL_REG_FOR_DATA_IND_REQ
  for registering the call back function for these LC Id s
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

void rlc_rrc_req_to_register_for_data_ind
(
  sys_modem_as_id_e_type wrrc_as_id,
  rlc_ul_register_srvc_type *rlc_lc_id_list
)
{
  uint32 idx;
  rrc_cmd_type *llc_cmd_ptr;                     /* Pointer to the RRC Command */
  
  l2_ul_req_cmd * llc_cmd_l2_ptr = NULL;
  for(idx =0; idx < rlc_lc_id_list->nchan ; idx++)
  {
    switch(rrclcm_get_ul_rb_id(wrrc_as_id,rlc_lc_id_list->rlc_data[idx].lc_id))
    {
      case CCCH_RADIO_BEARER_ID:
      case DCCH_UM_RADIO_BEARER_ID:
      case DCCH_AM_RADIO_BEARER_ID:
      case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
      case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
        WRRC_MSG0_HIGH("Ignore registering SRBs as it as already been registered");         
        /* Since SRBS cant be registered by anyonelse other than RRC, ignore 
         * registering for this request
         */
        return;

      default:
        break;
    }
  }
  
  /*send CRLC DATA IND REQ to RLC via rrcllc */
  llc_cmd_ptr = rrc_get_int_cmd_buf(wrrc_as_id);
 
  RRC_GET_POINTER_L2_REQ_VALUE(llc_cmd_ptr->cmd.rrc_llc_req.l2_ul_req,llc_cmd_l2_ptr);
 
 /* Initialize the LLC command header first */
  llc_cmd_ptr->cmd_hdr.cmd_id= RRC_LLC_CMD_REQ;
  llc_cmd_ptr->cmd.rrc_llc_req.procedure = RRC_PROCEDURE_CU;
  llc_cmd_ptr->cmd.rrc_llc_req.cmd_dest = LAYER2_RLC_UL;

  /* Now fill the L2 cmd header */
  llc_cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id = CRLC_UL_REG_FOR_DATA_IND_REQ;
  llc_cmd_l2_ptr->ul_reg_data_ind.block_status_timer=TRUE;
  for(idx =0; idx < rlc_lc_id_list->nchan ; idx++)
  {
    llc_cmd_l2_ptr->ul_reg_data_ind.lc_id[idx] = rlc_lc_id_list->rlc_data[idx].lc_id;
  }

  /* rab_count loop */
  llc_cmd_l2_ptr->ul_reg_data_ind.num_lc= (uint16)rlc_lc_id_list->nchan;
  WRRC_MSG1_HIGH("registering with RLC for %d RBs", rlc_lc_id_list->nchan);
  rrc_put_int_cmd(wrrc_as_id, llc_cmd_ptr); 
}
/***/ __WCDMA_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*====================================================================
FUNCTION: rrc_translate_n312

DESCRIPTION:
  This function translates the enum for timer N312 into an int16 value.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
static uint16 rrc_translate_n312(sys_modem_as_id_e_type wrrc_as_id,rrc_N_312 val)
{
  switch (val)
  {
    case rrc_N_312_s1:
      return 1;
    case rrc_N_312_s50:
      return 50;
    case rrc_N_312_s100:
      return 100;
    case rrc_N_312_s200:
      return 200;
    case rrc_N_312_s400:
      return 400;
    case rrc_N_312_s600:
      return 600;
    case rrc_N_312_s800:
      return 800;
    case rrc_N_312_s1000:
      return 1000;
    default:
      WRRC_MSG1_ERROR("Invalid N312 = %d", val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
}
/*====================================================================
FUNCTION: rrc_translate_n312_v3a0ext

DESCRIPTION:
  This function translates the enum for timer N312 into an int16 value.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
static uint16 rrc_translate_n312_v3a0ext(sys_modem_as_id_e_type wrrc_as_id,rrc_N_312ext val)
{
  switch (val)
  {
    case rrc_N_312ext_s2:
      return 2;
    case rrc_N_312ext_s4:
      return 4;
    case rrc_N_312ext_s10:
      return 10;
    case rrc_N_312ext_s20:
      return 20;
    default:
      WRRC_MSG1_ERROR("Invalid N312 = %d", val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
}
/*====================================================================
FUNCTION: rrc_translate_n315

DESCRIPTION:
  This function translates the enum for timer N315 into an int16 value.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
static uint16 rrc_translate_n315(sys_modem_as_id_e_type wrrc_as_id,rrc_N_315 val)
{
  switch (val)
  {
    case rrc_N_315_s1:
      return 1;
    case rrc_N_315_s50:
      return 50;
    case rrc_N_315_s100:
      return 100;
    case rrc_N_315_s200:
      return 200;
    case rrc_N_315_s400:
      return 400;
    case rrc_N_315_s600:
      return 600;
    case rrc_N_315_s800:
      return 800;
    case rrc_N_315_s1000:
      return 1000;
    default:
      WRRC_MSG1_ERROR("Invalid N315 = %d",val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
}
/*====================================================================
FUNCTION: rrc_translate_n315_v3a0ext

DESCRIPTION:
  This function translates the enum for timer N315 into an int16 value.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
static uint16 rrc_translate_n315_v3a0ext(sys_modem_as_id_e_type wrrc_as_id,rrc_N_315ext val)
{
  switch (val)
  {
    case rrc_N_315ext_s2:
      return 2;
    case rrc_N_315ext_s4:
      return 4;
    case rrc_N_315ext_s10:
      return 10;
    case rrc_N_315ext_s20:
      return 20;
    default:
      WRRC_MSG1_ERROR("Invalid N315 = %d",val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
}
/*===========================================================================

FUNCTION          RRC_TRANSLATE_ACCESS_CLASS_BARRED_LIST

DESCRIPTION       This function is responsible for translating the access 
                  class barred list from the ASN1 defined linked list format
                  to RRC internal array structure.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void rrc_translate_access_class_barred_list(sys_modem_as_id_e_type wrrc_as_id,rrc_AccessClassBarredList 
                                             *sib_access_class_barred_list_ptr,
                                            rrc_int_access_class_barred_list_type
                                             *access_class_barred_list_ptr)
{
  uint32 i=0,index=0,n_value=0;

  if(access_class_barred_list_ptr == NULL)
  {
    WRRC_ERR0_FATAL("AC barred list is NULL");
  }

  for (i=0; i<(uint16)RRC_INT_ACCESS_CLASS_MAX_INDEX; i++)
  {
    access_class_barred_list_ptr->barred[i] = FALSE;
  }

  index = 0;
  n_value = sib_access_class_barred_list_ptr->n;
  
  while(n_value != 0)
  {
    /* Check for all access classes */
    if(sib_access_class_barred_list_ptr->elem[index] == rrc_AccessClassBarred_barred)
    {
      access_class_barred_list_ptr->barred[index] = TRUE;
    }
    /* Move to the next access class entry */
    n_value--;
    index++;
  }

} /* rrc_translate_access_class_barred_list */

/*===========================================================================

FUNCTION          RRC_TRANSLATE_PPAC_ACCESS_CLASS_BARRED_LIST

DESCRIPTION       This function is responsible for translating the access 
                  class barred list from the ASN1 defined linked list format
                  to RRC internal array structure.
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void rrc_translate_ppac_access_class_barred_list(sys_modem_as_id_e_type wrrc_as_id,rrc_LocationRegistrationAccessClassBarredList 
                                             *sib_access_class_barred_list_ptr,
                                            rrc_int_access_class_barred_list_type
                                             *access_class_barred_list_ptr)
{
  uint32 elem_index = 0,index = 0,n_value = 0;

  if(access_class_barred_list_ptr == NULL)
  {
    WRRC_ERR0_FATAL("AC barred list is NULL");
  }

  for (index=0; index < (uint16)RRC_INT_ACCESS_CLASS_MAX_INDEX; index++)
  {
    access_class_barred_list_ptr->barred[index] = FALSE;
  }

  index = 0;
  n_value = sib_access_class_barred_list_ptr->n;
  
  while(n_value != 0)
  {
    if(index == RRC_INT_ACCESS_CLASS_10_INDEX)
    {
      index++;
      continue;
    }
/* Check for all access classes */
    if(sib_access_class_barred_list_ptr->elem[elem_index] == rrc_AccessClassBarred_barred)
    {
      access_class_barred_list_ptr->barred[index] = TRUE;
    }
    /* Move to the next access class entry */
    n_value--;
    index++;
    elem_index++;
  }
} /* rrc_translate_ppac_access_class_barred_list */

/*===========================================================================

FUNCTION          RRC_TRANSLATE_GSM_MAP_PLMN_ID


DESCRIPTION       

  This function translates a GSM-MAP PLMN Id type defined by ASN.1 into
  the internal type used by RRC and RRC-MM interface. 

DEPENDENCIES

  Both the internal and external PLMN pointers must be initialized.
  If either one is NULL, the convertion will fail. MCC must have 3
  digits and MNC must have either 2 or 3 digits in the external
  PLMN type. Otherwise the convertion will fail.

RETURN VALUE
                  
  None

SIDE EFFECTS
                  
  None

===========================================================================*/
void rrc_translate_gsm_map_plmn_id
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_PLMN_Identity  *ext_plmn,      /* Pointer to external PLMN type */
  rrc_plmn_identity_type  *int_plmn  /* Pointer to internal PLMN type */
)
{

  rrc_MCC *mcc_ptr;         /* Point to MCC            */
  rrc_MNC *mnc_ptr;         /* Point to MNC            */
  uint32 count;            /* Local counter           */
  uint32 index=0, n_value=0;

  /* Make sure the internal and external pointers are initialized */
  if( ext_plmn == NULL )
  {
    WRRC_MSG0_ERROR("External PLMN not initialized");
    return;
  }
  if( int_plmn == NULL )
  {
    WRRC_MSG0_ERROR("Internal PLMN not initialized");
    return;
  }
  
  /* Initialize the number of MNC digits to zero */
  int_plmn->num_mnc_digits = 0;

  /* Convert the MCC. MCC must have 3 digits. If external MCC
  does not have 3 digits, indicate an error.  MCC is arranged
  as a recursive structure with the first structure pointing
  to the MSDigit. */
  mcc_ptr = &(ext_plmn->mcc);
  index = 0;
  n_value = mcc_ptr->n;

  for( count=0; count<3; count++ )
  {
    if((n_value == 0) && (count == 0))
    {
      WRRC_MSG0_ERROR("MCC out of range");
      return;
    }
    int_plmn->mcc[count] = (uint8)mcc_ptr->elem[index];
    n_value--;
    index++;
  }

  /* Read the MNC. MNC must have 2 or 3 digits. If external MNC
  does not have 2 or 3 digits, indicate an error. */

  if( ext_plmn->mnc.n == 2)
  {
    /* MNC has 2 digits */
    int_plmn->num_mnc_digits = 2;
    /* Initialize the last byte in array to 0xf as per the RRC-MM interface */
    int_plmn->mnc[2] = 0xf;
  }
  else
  {
    /* MNC has 3 digits */
    int_plmn->num_mnc_digits = 3;
  }

  /* Loop through and add the available digits */
  mnc_ptr = &(ext_plmn->mnc);
  index = 0;
  n_value = mnc_ptr->n;

  for( count=0; count<int_plmn->num_mnc_digits; count++ )
  {
    if((n_value == 0) && (count == 0))
    {
      WRRC_MSG0_ERROR("MNC out of range");
      return;
    }
    int_plmn->mnc[count] = (uint8)mnc_ptr->elem[index];
    n_value--;
    index++;
  }
  
}


/*===========================================================================

FUNCTION          rrc_translate_gsm_map_plmn_id_with_optional_mcc


DESCRIPTION       

  This function translates a GSM-MAP PLMN Id with optinal MCC type defined by ASN.1 into
  the internal type used by RRC and RRC-MM interface. And also indicates whether the MCC 
  of the externel PLMN type is present or not.

DEPENDENCIES

  Both the internal and external PLMN pointers must be initialized.
  If either one is NULL, the convertion will fail. MCC of external
  PLMN type is optional. Otherwise the convertion will fail.

RETURN VALUE
                  
  None

SIDE EFFECTS
                  
  None

===========================================================================*/
void rrc_translate_gsm_map_plmn_id_with_optional_mcc
(
  sys_modem_as_id_e_type wrrc_as_id, 
  boolean *is_mcc_valid,
  rrc_PLMN_IdentityWithOptionalMCC_r6  *ext_plmn,      /* Pointer to external PLMN type */
  rrc_plmn_identity_type  *int_plmn  /* Pointer to internal PLMN type */
)
{

  rrc_MCC mcc_ptr;         /* Point to MCC            */
  rrc_MNC mnc_ptr;         /* Point to MNC            */
  uint32 count;            /* Local counter           */
  uint32 n_value=0, n_index=0;

  /* Make sure the internal and external pointers are initialized */
  if( ext_plmn == NULL )
  {
    WRRC_MSG0_ERROR("External PLMN not initialized");
    return;
  }
  if( int_plmn == NULL )
  {
    WRRC_MSG0_ERROR("Internal PLMN not initialized");
    return;
  }
  
  /* Initialize the number of MNC digits to zero */
  int_plmn->num_mnc_digits = 0;

  /* Convert the MCC. MCC must have 3 digits. If external MCC
  does not have 3 digits, indicate an error.  MCC is arranged
  as a recursive structure with the first structure pointing
  to the MSDigit. */
  if(ext_plmn->m.mccPresent)
  {
    mcc_ptr = ext_plmn->mcc;

    n_value = mcc_ptr.n;
    for( count=0; count<3; count++ )
    {
      if( n_value == 0 )
      {
        WRRC_MSG0_ERROR("MCC out of range");
        return;
      }
      int_plmn->mcc[count] = (uint8)mcc_ptr.elem[n_index];
      n_index++;
      n_value--;
    }
    *is_mcc_valid = TRUE;
  }
  else
  {
    *is_mcc_valid = FALSE;
  }

  /* Read the MNC. MNC must have 2 or 3 digits. If external MNC
  does not have 2 or 3 digits, indicate an error. */

  /* First check if there are 3 digits */
  if( ext_plmn->mnc.n == 0 )
  {
    WRRC_MSG0_ERROR("MNC has no digit");
    return;
  }

  if( ext_plmn->mnc.n == 1 )
  {
    WRRC_MSG0_ERROR("MNC has only 1 digit");
    return;
  }
  if( ext_plmn->mnc.n == 2 )
  {
    /* MNC has 2 digits */
    int_plmn->num_mnc_digits = 2;
    /* Initialize the last byte in array to 0xf as per the RRC-MM interface */
    int_plmn->mnc[2] = 0xf;
  }
  else
  {
    /* MNC has 3 digits */
    int_plmn->num_mnc_digits = 3;
  }

  /* Loop through and add the available digits */
  mnc_ptr = ext_plmn->mnc;
  n_index = 0;
  n_value = mnc_ptr.n;
  
  for( count=0; count<int_plmn->num_mnc_digits; count++ )
  {
    if( n_value == 0 )
    {
      WRRC_MSG0_ERROR("MNC out of range");
      return;
    }
    int_plmn->mnc[count] = (uint8)mnc_ptr.elem[n_index];
    n_index++;
    n_value--;
  }
  
}



/*===========================================================================

FUNCTION          RRC_TRANSLATE_CELL_ID


DESCRIPTION       

  This function translates a Cell Id type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  The algorithm used assumes that the processor is Little Endian.
  
RETURN VALUE
                  
  The Cell_id converted to the internal type rrc_int_cell_id_type

SIDE EFFECTS
                  
  None

===========================================================================*/
rrc_int_cell_id_type rrc_translate_cell_id
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_CellIdentity  *ext_cell_id         /* Pointer to external PLMN type */
)
{
  uint32 count;                    /* Local counter       */
  rrc_int_cell_id_type cell_id=0;  /* To save the cell id */
  uint32 num_bytes;                /* number of full bytes in string */
  uint32 num_shift;                /* number of bits to shift the result.
                                   This is based on the number of signigicant
                                   bits in the string */

  /* ossBitString represents an item by indicating the
  number of significant bits in the "length" and putting
  the value starting with the MSB 8 bits, in a unsigned
  char array. The length may not be octet alligned. For
  example if the length is 20 bits, value[1] will contain
  8 MSB bits, value[2] will contain the 2nd 8 MSB bits
  and value[3] will have the 2 lsb bits in the ms and 2
  ms bit positions. */

  /* First find the number of bits that don't
  contain any data. Since the length may not be
  octet alligned, there may be some bits that
  don't have any data and need to be shifted out. */

  num_shift = (8 - ((ext_cell_id->numbits)%8));
  if( num_shift == 8 )
  {
    /* Bits are octet alligned, no shifting needed */ 
    num_shift = 0;
    num_bytes = ((ext_cell_id->numbits)/8);
  }
  else
  {
    num_bytes = ((ext_cell_id->numbits)/8) + 1;
  }

  if( num_bytes > 4)
  {
    WRRC_MSG0_ERROR("cell_id longer than 32 bits!");
    return( cell_id );
  }

  /* Now loop through and copy all the given bits.
  NOTE: Little Endian is assumed here. If Big Endian
  is used, we may need to change this. */
  for(count=0; count < num_bytes; count++)
  {
    /* Left shift each byte from the char pointer
    to the uint32 item. */
    cell_id = cell_id << 8;
    cell_id = (cell_id | (ext_cell_id->data[count]));
  }

  /* Finally right shift to remove the bits with no data */
  cell_id = (cell_id >> num_shift);

  return(cell_id);
}

/*===========================================================================

FUNCTION          RRC_TRANSLATE_GSM_MAP_IMSI


DESCRIPTION       

  This function translates a GSM-MAP IMSI type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  Both the internal and external IMSI pointers must be initialized.
  If either one is NULL, the convertion will fail.

RETURN VALUE
                  
  None

SIDE EFFECTS
                  
  None

===========================================================================*/
void rrc_translate_gsm_map_imsi 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_IMSI_GSM_MAP  *ext_imsi,/* Pointer to external IMSI type */
  rrc_imsi_gsm_map_type  *int_imsi   /* Pointer to internal IMSI type */
)
{
  uint32 i=0, index = 0;                          /* Local parameter to store the 
                                        index value */
  rrc_IMSI_GSM_MAP  *local_ext_imsi;
                                    /* Local  to external IMSI type */

  /* Assign external pointer to local pointer */
  local_ext_imsi = ext_imsi;

  /* Make sure the internal and external pointers are initialized */
  if( ext_imsi == NULL )
  {
    WRRC_MSG0_ERROR("External IMSI not initialized");
    return;
  }

  if( int_imsi == NULL )
  {
    WRRC_MSG0_ERROR("Internal IMSI not initialized");
    return;
  }

  while (ext_imsi->n > index)
  {
    int_imsi->value[i++] = ext_imsi->elem[index++];

    if(i >= RRC_MAX_GSMMAP_IMSI_DIGITS)
    {
      break;
    }   
  }

  /* Set the length of the IMSI */
  int_imsi->num_digits = i;

  /* Restore the original pointer */
  ext_imsi = local_ext_imsi;
} /* rrc_translate_gsm_map_imsi */

/*===========================================================================

FUNCTION          RRC_TRANSLATE_IMEI


DESCRIPTION       

  This function translates IMEI type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  Both the internal and external IMEI pointers must be initialized.
  If either one is NULL, the convertion will fail.

RETURN VALUE
                  
  None

SIDE EFFECTS
                  
  None

===========================================================================*/
void rrc_translate_imei
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_IMEI  *ext_imei,        /* Pointer to external IMEI type */
  rrc_imei_gsm_map_type  *int_imei   /* Pointer to internal IMEI type */
)
{
  uint32 i=0, index=0;                          /* Local parameter to store the 
                                        index value */
  struct rrc_IMEI  *local_ext_imei;  /* Local pointer to external IMEI type */

  /* Make sure the internal and external pointers are initialized */
  if( ext_imei == NULL )
  {
    WRRC_MSG0_ERROR("External IMSI not initialized");
    return;
  }
  
  if( int_imei == NULL )
  {
    WRRC_MSG0_ERROR("Internal IMSI not initialized");
    return;
  }

  local_ext_imei = ext_imei;
  while (ext_imei->n > index)
  {
    int_imei->value[i++] = ext_imei->elem[index++];
    
    if(i >= RRC_NUM_IMEI_DIGITS)
    {
      break;
    }
  }

  /* Restore the head pointer of the linked list */  
  ext_imei = local_ext_imei;

} /* rrc_translate_imei */



/*===========================================================================

FUNCTION   RRC_INITIALIZE_ESTABLISHED_RABS

DESCRIPTION

  This function initializes the "ESTABLISHED_RABS" variable.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_initialize_established_rabs(sys_modem_as_id_e_type wrrc_as_id)
{
  uint32 count; /* Local counter */

  for( count=0; count<MAX_RAB_TO_SETUP; count++ )
  {
    /* Initialize each RAB id with an invalid value to show that
    it's not used currently. */
    rrc_est_rabs.rabs[count].rab_id = RRC_INVALID_RAB_ID;
    rrc_est_rabs.rabs[count].num_rbs_for_rab = 0;
    rrc_est_rabs.rabs[count].rab_type = RRCRB_UNKNOWN_RAB;
  }
  for( count=0; count<MAX_SRB_SETUP; count++ )
  {
    /* For each SRB, initialize the status to the default
    value of STARTED. */
    rrc_est_rabs.srbs[count].srb_status = RRC_RB_STARTED;
  }
  /* Reset the current codec to the default mode */
  rrc_set_current_codec_mode(wrrc_as_id, WCDMA_IVOCODER_ID_AMR);
  rrc_set_ordered_codec_mode(wrrc_as_id, WCDMA_IVOCODER_ID_NONE);
  /* All other values will be initialized when RABs are added
  to this variable. */
}


/*===========================================================================

FUNCTION   rrcdata_init_procedure

DESCRIPTION


        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrcdata_init_procedure(sys_modem_as_id_e_type wrrc_as_id)
{

  rrcdata_misc_info.ota_msg_version = MSG_INVALID;

  /* Initialize to not indicated by default */
  rrcdata_misc_info.ps_rb_re_establish_indicator = FALSE;

  rrcdata_misc_info.rrc_msg_failure_cause = RRC_MSG_VALID;

  rrcdata_hs_info.hsdpa_action = HSDPA_NOOP;
  rrcdata_hs_info.hsdpa_status = HSDPA_INACTIVE;
  rrcdata_hs_info.hsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
  
  #ifdef FEATURE_HSPA_CALL_STATUS_IND
  rrcdata_hs_info.prev_hspa_call_status = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
  rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
  #endif /* FEATURE_HSPA_CALL_STATUS_IND */

  rrcdata_logging_info.wcdma_rrc_systematic_logging_enabled_nv = FALSE;

  /* This contains the current Codec mode being used	*/
  rrcdata_codec_info.rrc_active_codec = WCDMA_IVOCODER_ID_AMR; 
  /* Contains codec info received in the OTA msg, so that it can be sent to MAC in ordered config  */
  rrcdata_codec_info.rrc_ota_codec_mode_status = WCDMA_IVOCODER_ID_NONE;

#ifdef WCDMA_RRC_INDICATE_CODEC_TO_CM
  rrcdata_codec_info.ordered_codec_valid = FALSE;
  rrcdata_codec_info.is_hho_started = FALSE;
#endif

#ifdef FEATURE_FEMTO_CSG
  rrc_csg_int_data.is_whitelist_valid = FALSE;
#endif

}


#ifdef FEATURE_UMTS_PDCP
/*===========================================================================

FUNCTION   RRC_INITIALIZE_PDCP_SN_INFO

DESCRIPTION

  This function initializes the "PDCP_SN_INFO" variable.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_initialize_pdcp_sn_info(sys_modem_as_id_e_type wrrc_as_id)
{
  uint8 count;

  for(count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    rrcdata_misc_info.rrc_pdcp_sn_info.rrc_rb_with_pdcp_info[count].rb_id = RRC_INVALID_RB_ID;
  }
  MSG_LOW("Suppressing lint error:function for future use %d",rrcdata_misc_info.rrc_pdcp_sn_info.rrc_rb_with_pdcp_info[0].rb_id, 0, 0);
}

#endif /* FEATURE_UMTS_PDCP */

/*===========================================================================

FUNCTION  RRC_RETURN_LC_INFO_FOR_RAB_SUB

DESCRIPTION

  This function returns the logical channel ids for a given RAB.
  The calling function must initialize a structure of type
  rrc_user_plane_lc_info_type and fill in the RAB id before
  passing the pointer to this function. This function will fill
  in the LC ids for uplink and downlink for that RAB to the given
  pointer.
    
DEPENDENCIES

  Currently this is only supported for CS data RABs and PS RABs.
  This function must not be used for any other type of RABs at
  present.
 
RETURN VALUE

  A a value of type rrc_rab_status_e_type. RRC_RAB_NOT_FOUND is returned
  if the given RAB is not a valid RAB that has been setup by RRC. RRC_RAB_FOUND
  is returned if the RAB is valid.

SIDE EFFECTS

  None.

===========================================================================*/
rrc_rab_search_e_type rrc_return_lc_info_for_rab_sub
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_user_plane_lc_info_type *ptr
)
{
  uint32 count; /* Local counter */
  uint32 count2;/* Local counter */
  boolean match_found = FALSE; /* Indicate if RAB is found */
  boolean lc_id_error = FALSE; /* Indicate if there is an error in getting
                               the LC ids. */

  /* Make sure the given pointer is initialized */
  if( ptr == NULL)
  {
    WRRC_MSG0_ERROR("User-plane ptr invalid!");
    return(RRC_RAB_NOT_FOUND);
  }
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( rrc_est_rabs.rabs[count].rab_id == (ptr->rab_id) )
    {
      /* We have a match. Now fill in the relevant values. */
      match_found = TRUE;

      /* Check if there is only one RB for this RAB. 
      Currently for data calls there is only one RB per
      RAB. */
      if( rrc_est_rabs.rabs[count].num_rbs_for_rab > MAX_RB_PER_RAB )
      {
        WRRC_MSG0_ERROR("Too many RBs for data RAB");
        return(RRC_RAB_NOT_FOUND);
      }
      /* However, go ahead and fill in all the required
      data */
      for( count2=0; count2 < (rrc_est_rabs.rabs[count].num_rbs_for_rab) ; count2++)
      {

        /* The logical channel manager keeps track of the logical channels
        assigned to each RB. Call the appropriate function in lcm to get the
        lc ids. We have to pass in the given RB id. There is one function
        for downlink lc id and one for uplink lc id. */
        ptr->dl_lc_id[count2] = 
          rrclcm_get_dl_rlc_lc_id(wrrc_as_id,
            (rrc_RB_Identity)(rrc_est_rabs.rabs[count].rb_for_rab[count2].rb_id)
                                 );

        if( ptr->dl_lc_id[count2] == RRCLCM_RLC_LC_ID_NOT_FOUND )
        {
          WRRC_MSG1_ERROR("LC id not found for RAB %d", ptr->rab_id);
          lc_id_error = TRUE;
          break; /* Break out of the for loop */
        }

        /* Do the same thing for the uplink lc id. */
        ptr->ul_lc_id[count2] = 
          rrclcm_get_ul_rlc_lc_id(wrrc_as_id,
            (rrc_RB_Identity)(rrc_est_rabs.rabs[count].rb_for_rab[count2].rb_id)
                                 );

        if( ptr->ul_lc_id[count2] == RRCLCM_RLC_LC_ID_NOT_FOUND )
        {
          WRRC_MSG1_ERROR("LC id not found for RAB %d", ptr->rab_id);
          lc_id_error = TRUE;
          break; /* Break out of the for loop */
        }
#ifdef FEATURE_SCUDIF_ENHANCEMENT
        #error code not present
#endif
      }

      if( lc_id_error == TRUE )
      {
        return(RRC_RAB_NOT_FOUND);
      }
      else
      {
        /* We got the right LC ids, so fill in the count */
        ptr->num_dl_lc_ids_for_rab = 
          (uint8)rrc_est_rabs.rabs[count].num_rbs_for_rab;
        ptr->num_ul_lc_ids_for_rab = 
          (uint8)rrc_est_rabs.rabs[count].num_rbs_for_rab;
      }
      
      break; /* Out of the outer for loop */
    }/* End if (rab matched) */
  }/* end for */

  if( match_found == FALSE )
  {
    WRRC_MSG1_ERROR("RAB %d not established", ptr->rab_id); 
    return( RRC_RAB_NOT_FOUND );
  }
  else
  {
    return( RRC_RAB_FOUND );
  }
}

/*===========================================================================

FUNCTION  RRC_RETURN_LC_INFO_FOR_RAB

DESCRIPTION

  This function returns the logical channel ids for a given RAB.
  The calling function must initialize a structure of type
  rrc_user_plane_lc_info_type and fill in the RAB id before
  passing the pointer to this function. This function will fill
  in the LC ids for uplink and downlink for that RAB to the given
  pointer.
    
DEPENDENCIES

  Currently this is only supported for CS data RABs and PS RABs.
  This function must not be used for any other type of RABs at
  present.
 
RETURN VALUE

  A a value of type rrc_rab_status_e_type. RRC_RAB_NOT_FOUND is returned
  if the given RAB is not a valid RAB that has been setup by RRC. RRC_RAB_FOUND
  is returned if the RAB is valid.

SIDE EFFECTS

  None.

===========================================================================*/
rrc_rab_search_e_type rrc_return_lc_info_for_rab
(
  rrc_user_plane_lc_info_type *ptr
)
{
  return rrc_return_lc_info_for_rab_sub(SYS_MODEM_AS_ID_1,ptr);
}

/*===========================================================================

FUNCTION   RRC_ADD_RAB_TO_ESTABLISHED_RABS

DESCRIPTION

  This function adds a new RAB to the ESTABLISHED_RABS variable.
  The caller must verify that the RAB is not already in the
  variable by calling RRC_FIND_RAB_IN_ESTABLISHED_RABS.
        
DEPENDENCIES

  All the relevant information for the RAB must be included
  in the pointer that's passed in to this function.
  
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_add_rab_to_established_rabs
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_est_rab_info_type *ptr /* Pointer to the RAB information */
)
{
  uint32 count; /* Local counter */

  uint32 index = 0; /* Index into the variable ESTABLISHED_RABS    */
  boolean index_found=FALSE; /* Indicates if an index is found */

  /* Loop around to find the first slot that's not used */
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( rrc_est_rabs.rabs[count].rab_id == RRC_INVALID_RAB_ID)
    {
      /* This index is not used, it can be used for this RAB */
      index_found=TRUE;
      index=count;
      break; /* Break out of the for loop */
    }
  }

  if( index_found == FALSE )
  {
    WRRC_MSG0_ERROR("Cannot add RAB to ESTABLISHED_RABS, no space!");
    return;
  }

  /* Copy all the relevant data into the variable. */
  rrc_est_rabs.rabs[index].cn_domain = ptr->cn_domain;
  rrc_est_rabs.rabs[index].rab_id = ptr->rab_id;
  rrc_est_rabs.rabs[index].rab_type = ptr->rab_type;  
  rrc_est_rabs.rabs[index].re_est_timer = ptr->re_est_timer;
  rrc_est_rabs.rabs[index].rab_id_is_gsm_map = ptr->rab_id_is_gsm_map;
  rrc_est_rabs.rabs[index].num_rbs_for_rab = ptr->num_rbs_for_rab;

  WRRC_MSG6_HIGH("RAB add id:%d,rbs:%d,idx:%d domain:%d,re-est tmr:%d,gsm_map:%d",
           rrc_est_rabs.rabs[index].rab_id,
           rrc_est_rabs.rabs[index].num_rbs_for_rab,
           index,
           rrc_est_rabs.rabs[index].cn_domain,
           rrc_est_rabs.rabs[index].re_est_timer,
           rrc_est_rabs.rabs[index].rab_id_is_gsm_map
           );
  if( ptr->num_rbs_for_rab > MAX_RB_PER_RAB )
  {
    /* More RBs than can be mapped to a RAB, just return */
    WRRC_MSG0_ERROR("MAX RBs per RAB count exceeded!");
    return;
  }

  /* Copy all RB related data for this RAB */
  for( count=0; count < MAX_RB_PER_RAB; count++ )
  {
    if( count < ptr->num_rbs_for_rab )
    {
      rrc_est_rabs.rabs[index].rb_for_rab[count].rb_id = ptr->rb_for_rab[count].rb_id;
      rrc_est_rabs.rabs[index].rb_for_rab[count].rb_status 
        = ptr->rb_for_rab[count].rb_status;
      rrc_est_rabs.rabs[index].rb_for_rab[count].subflow 
        = ptr->rb_for_rab[count].subflow;
    }
    else
    {
      /* All other RBs for this RAB are unused */
      rrc_est_rabs.rabs[index].rb_for_rab[count].rb_id = RRC_INVALID_RB_ID;
    }
  }
#ifdef FEATURE_INACTIVITY_HANDLING
  if (rrc_est_rabs.rabs[index].cn_domain == RRC_PS_DOMAIN_CN_ID)
  {
    if (rrc_get_state(wrrc_as_id) == RRC_STATE_CELL_DCH) 
    {
      WRRC_MSG1_HIGH("Adding PS RAB, Restarting Inactivity timer %d",INACTIVITY_TIMER_IN_MS);
      /* Restart incativity handling timer */
      rrctmr_start_timer(wrrc_as_id,RRCTMR_INACTIVITY_TIMER, INACTIVITY_TIMER_IN_MS);
    }
    else
    {
      WRRC_MSG3_HIGH("Adding PS RAB, Restarting Inactivity timer %d + remain T305 %d actual T305 timer = %d",
               INACTIVITY_TIMER_IN_MS,
               rrctmr_get_remaining_time(wrrc_as_id,RRCTMR_T_305_TIMER),
               rrc_convert_t305_to_ms(wrrc_as_id,RRC_GET_T305()));
      /* Restart incativity handling timer */
      if(rrc_convert_t305_to_ms(wrrc_as_id,RRC_GET_T305()) != RRCTMR_INFINITY_VAL )
      {
        rrctmr_start_timer(wrrc_as_id,RRCTMR_INACTIVITY_TIMER, 
                         (unsigned long)(INACTIVITY_TIMER_IN_MS + rrctmr_get_remaining_time(wrrc_as_id,RRCTMR_T_305_TIMER))); 
      }
      else
      {
        /*If T305 timer was infinity then the timer wouldnt have been started . In thats case start
           timer of 5 mins plus 24 hrs*/
         rrctmr_start_timer(wrrc_as_id,RRCTMR_INACTIVITY_TIMER, 
                         (unsigned long)(INACTIVITY_TIMER_IN_MS + RRCTMR_INFINITY_VAL)); 
      }
    }
    (void)rrcmisc_handle_ps_rab_inactivity(sys_modem_as_id_e_type wrrc_as_id, 1);
  }
#endif /* FEATURE_INTACTIVITY_HANDLING */

}



/*===========================================================================

FUNCTION   RRC_FIND_RB_IN_EST_RABS

DESCRIPTION

  This function validates the rb_ids in the received RBR Message.
  For user plane RB ids it checks whether the rb to be released
  is present in the established rbs. If not this function returns FALSE.

DEPENDENCIES

  None.
 
RETURN VALUE

  Boolean 
          RRC_RB_FOUND     :rb-id exists in the ESTABLISHED_RAB variable.
          RRC_RB_NOT_FOUND :rb-id does not exist in the ESTABLISHED_RAB variable.

SIDE EFFECTS

  None.

===========================================================================*/
rrc_rb_search_e_type rrc_find_rb_in_est_rabs
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RB_Identity rb_id
)
{
  uint32 rb_count;
  uint32 rab_count;
  rrc_rb_search_e_type rb_srch = RRC_RB_NOT_PRESENT;
   /* Check whether this RB exists in established_rab db */
  for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
  {
    if (
         ((rrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)|| 
           (rrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID))
          &&
          ((rrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) &&
           (rrc_est_rabs.rabs[rab_count].rab_id !=  RRC_INVALID_RAB_ID))
        )
    {
      /*it is a valid established rab, now compare each rb-id in this established rab with 
      the rb-id received in the message */
      for (rb_count=0; rb_count<rrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++)
      {
        if (rrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == rb_id)
        {
          rb_srch = RRC_RB_PRESENT;
          break;
        }
      }
    } /*end of if*/
    if (rb_srch == RRC_RB_PRESENT)
    {
      break;
    }
  }/* rab_count loop */
  return rb_srch;
}

/*===========================================================================

FUNCTION   RRC_FIND_RAB_FOR_RB_IN_EST_RABS_SUB

DESCRIPTION

  This function validates the rb_ids in the received RBR Message.
  For user plane RB ids it checks whether the rb
  is present in the established rbs. It returns the corresponding RAB id

DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_rab_id_type rab_id

SIDE EFFECTS

  None.

===========================================================================*/
rrc_rab_id_type rrc_find_rab_for_rb_in_est_rabs_sub
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint8 rb_id
)
{
  uint32 rb_count;
  uint32 rab_count;
  rrc_rb_search_e_type rb_srch = RRC_RB_NOT_PRESENT;
  rrc_rab_id_type rab_id = RRC_INVALID_RAB_ID;

   /* Check whether this RB exists in established_rab db */
  for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
  {
    if (
         ((rrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)|| 
           (rrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID))
          &&
          ((rrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) &&
           (rrc_est_rabs.rabs[rab_count].rab_id !=  RRC_INVALID_RAB_ID))
        )
    {
      /*it is a valid established rab, now compare each rb-id in this established rab with 
      the rb-id received in the message */
      for (rb_count=0; rb_count<rrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++)
      {
        if (rrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == rb_id)
        {
          rab_id = rrc_est_rabs.rabs[rab_count].rab_id;
          rb_srch = RRC_RB_PRESENT;
          break;
        }
      }
    } /*end of if*/
    if (rb_srch == RRC_RB_PRESENT)
    {
      break;
    }
  }/* rab_count loop */
  return rab_id;
}


/*===========================================================================

FUNCTION   RRC_FIND_RAB_FOR_RB_IN_EST_RABS

DESCRIPTION

  This function validates the rb_ids in the received RBR Message.
  For user plane RB ids it checks whether the rb
  is present in the established rbs. It returns the corresponding RAB id

DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_rab_id_type rab_id

SIDE EFFECTS

  None.

===========================================================================*/
rrc_rab_id_type rrc_find_rab_for_rb_in_est_rabs
(
  uint8 rb_id
)
{
  return rrc_find_rab_for_rb_in_est_rabs_sub(SYS_MODEM_AS_ID_1, rb_id);
}


/*===========================================================================

FUNCTION   RRC_FIND_RAB_FOR_RB_IN_EST_RABS

DESCRIPTION

  This function validates the rb_ids in the received RBR Message.
  For user plane RB ids it checks whether the rb
  is present in the established rbs. It returns the corresponding cn_domain

DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_rab_id_type rab_id

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_find_cn_domain_id_for_rb_in_est_rabs
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RB_Identity rb_id,
  rrc_cn_domain_identity_e_type *rrc_cn_domain_id
)
{
  uint32 rb_count=0;
  uint32 rab_count=0;
  
   /* Check whether this RB exists in established_rab db */
  for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
  {
    if (
         ((rrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)|| 
           (rrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID))
          &&
          ((rrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) &&
           (rrc_est_rabs.rabs[rab_count].rab_id !=  RRC_INVALID_RAB_ID))
        )
    {
      /* It is a valid established rab, now compare each rb-id in this established rab with 
           the rb-id received in the message */
      for (rb_count=0; rb_count<rrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++)
      {
        if (rrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == rb_id)
        {
          if(rrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)
          {
            rrc_est_rabs.rabs[rab_count].cn_domain = RRC_CS_DOMAIN_CN_ID;
            *rrc_cn_domain_id = RRC_CS_DOMAIN_CN_ID;
          }
          else
          {
            rrc_est_rabs.rabs[rab_count].cn_domain = RRC_PS_DOMAIN_CN_ID;
            *rrc_cn_domain_id = RRC_PS_DOMAIN_CN_ID;
          }
          return TRUE;          
        }
      }
    } /*end of if*/
  }/* rab_count loop */
  return FALSE;
}


/*===========================================================================

FUNCTION   RRC_FIND_RAB_IN_ESTABLISHED_RABS

DESCRIPTION

  This function checks if the given RAB id is present in the
  "ESTABLISHED_RABS" variable. If so, it returns RRC_RAB_FOUND.
  Else it returns RRC_RAB_NOT_FOUND.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
rrc_rab_search_e_type rrc_find_rab_in_established_rabs
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_rab_id_type rab_id /* Id of the received RAB */
)
{
  uint32 count; /* Local Counter */
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( rrc_est_rabs.rabs[count].rab_id == rab_id )
    {
      /* We have a match */
      return( RRC_RAB_FOUND );
    }
  }
  /* If we get here, we don't have the RAB stored */
  return( RRC_RAB_NOT_FOUND );
}         


/*===========================================================================

FUNCTION   RRC_FIND_IF_VOICE_RAB_EXISTS_IN_ESTABLISHED_RABS

DESCRIPTION

  This function checks if the given RAB id is present in the
  "ESTABLISHED_RABS" variable & belongs to CS domain. 
  If so, it returns TRUE.
  Else it returns FALSE.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean
rrc_find_if_voice_rab_exists_in_established_rabs(sys_modem_as_id_e_type wrrc_as_id,rrc_rab_id_type rab_id /* Id of the received RAB */)
{
  boolean voice_rab_already_exists = FALSE;
  uint32 count; /* Local Counter */
  
  for(count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if( rrc_est_rabs.rabs[count].rab_id == rab_id && 
        rrc_est_rabs.rabs[count].rab_type == RRCRB_CS_VOICE_RAB)
    {
      /* We have a match */
      voice_rab_already_exists = TRUE;
      break;
    }
  }

  return voice_rab_already_exists;
} /* rrc_find_if_voice_rab_exists_in_established_rabs */


/*===========================================================================

FUNCTION   RRC_UPDATE_RB_INFO_TO_EXISTING_VOICE_RAB

DESCRIPTION

  This function checks if the given RAB id is present in the
  "ESTABLISHED_RABS" variable & belongs to CS domain. 
  If so, it updates the RB info.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void
rrc_update_rb_info_to_existing_rab(sys_modem_as_id_e_type wrrc_as_id,rrc_est_rab_info_type *rbe_ptr)
{
  uint32 count; /* Local Counter */
  uint32 i = 0;
  
  for(count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if( rrc_est_rabs.rabs[count].rab_id == rbe_ptr->rab_id   && 
        rrc_est_rabs.rabs[count].rab_type == rbe_ptr->rab_type)
    {
      /* We have a match */
      for (; i < rbe_ptr->num_rbs_for_rab; i++)
      {
        WRRC_MSG2_HIGH("Added rb id %d for rab %d", rrc_est_rabs.rabs[count].rab_id, rbe_ptr->rb_for_rab[i].rb_id);
        rrc_est_rabs.rabs[count].rb_for_rab[rrc_est_rabs.rabs[count].num_rbs_for_rab].rb_id = rbe_ptr->rb_for_rab[i].rb_id;
        rrc_est_rabs.rabs[count].rb_for_rab[rrc_est_rabs.rabs[count].num_rbs_for_rab].rb_status = rbe_ptr->rb_for_rab[i].rb_status;
        rrc_est_rabs.rabs[count].rb_for_rab[rrc_est_rabs.rabs[count].num_rbs_for_rab].subflow = rbe_ptr->rb_for_rab[i].subflow;
        rrc_est_rabs.rabs[count].num_rbs_for_rab++;
      }
      break;
    }
  }
} /* rrc_update_rb_info_to_existing_voice_rab */


/*===========================================================================

FUNCTION   rrc_get_rabs_in_established_rabs_for_given_domain

DESCRIPTION

  This function gets all RAB ids and there corresponding info for
  a particular CN dimain and returns.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_established_rabs_for_a_domain_type structure.

SIDE EFFECTS

  None.

===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
  __attribute__((section(".uncompressible.text")))
#endif
rrc_established_rabs_for_a_domain_type rrc_get_rabs_in_established_rabs_for_given_domain
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_cn_domain_identity_e_type cn_domain /* Id of the received RAB */
)
{
  uint32 count; /* Local Counter */
  uint32 rab_count=0; /* Local Counter */
  rrc_established_rabs_for_a_domain_type rab_info;
  
  memset(rab_info.rabs, 0, (sizeof(rrc_est_rab_info_type) * MAX_RAB_TO_SETUP));
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if((rrc_est_rabs.rabs[count].cn_domain == cn_domain) &&
       (rrc_est_rabs.rabs[count].num_rbs_for_rab > 0))
    {
      /* We have a match */
      WCDMA_MEMCPY(&(rab_info.rabs[rab_count]),
                   sizeof(rrc_est_rab_info_type),
                   &(rrc_est_rabs.rabs[count]), 
                   sizeof(rrc_est_rab_info_type));
      rab_count ++; 
    }
  }
  rab_info.num_rabs = rab_count;
  /* If we get here, we don't have the RAB stored */
  return( rab_info );
} /* rrc_get_rabs_in_established_rabs_for_given_domain */        


/*===========================================================================

FUNCTION   VALIDATE_RB_ID_IN_RECONFIG_LIST

DESCRIPTION

  This function validates the rb_ids in the received RB Reconfig Message.
  If signalling RB id is present it returns RRC_RB_PRESENT. For user plane RB ids
  and SRB4 it checks whether the rb to be reconfigured or affected is present in the 
  established rbs. If not this function returns RRC_RB_NOT_PRESENT.

DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_rb_search_e_type 
          RRC_RB_PRESENT     :rb-id exists in the ESTABLISHED_RAB variable or
                              it is a signalling RB.
          RRC_RB_NOT_PRESENT :rb-id is not a signalling RB and it also
                              does not exist in the ESTABLISHED_RAB variable.

SIDE EFFECTS

  None.

===========================================================================*/

rrc_rb_search_e_type validate_rb_id_in_reconfig_list(sys_modem_as_id_e_type wrrc_as_id,rrc_RB_Identity rb_id)
{
  rrc_rb_search_e_type rb_srch = RRC_RB_NOT_PRESENT;
  /*check whether it is signalling rbid. */
  /*Do not include rb-id0 in the check because it is for CCCH */
  if (((rb_id > CCCH_RADIO_BEARER_ID) && (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)) ||
     RRC_RB_PRESENT == rrc_find_rb_in_est_rabs(wrrc_as_id,rb_id))
  {
    rb_srch = RRC_RB_PRESENT;
  }
  else if(rrcllc_has_config_srb5(wrrc_as_id,CURRENT_CONFIG, rb_id) == TRUE)
  {
    rb_srch = RRC_RB_PRESENT;
  }
  return rb_srch;
}

/*===========================================================================

FUNCTION   VALIDATE_RB_ID_IN_RELEASE_LIST

DESCRIPTION

  This function validates the rb_ids in the received RB Release Message.
  If the release is for rb-id 0 through 3 then this returns RRC_RB_NOT_PRESENT
  because SRB0-SRB3 should not be in the release list. For user plane RB ids and RB4,
  it checks whether the rb to be released is present in the established rbs. 
  If not this function returns RRC_RB_NOT_PRESENT.

DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_rb_search_e_type 
          RRC_RB_PRESENT     :rb-id exists in the ESTABLISHED_RAB variable or
                              it is a signalling RB.
          RRC_RB_NOT_PRESENT :rb-id is signalling RB <= SRB3 or 
                              rb-id is not a signalling RB and it also
                              does not exist in the ESTABLISHED_RAB variable.

SIDE EFFECTS

  None.

===========================================================================*/


rrc_rb_search_e_type validate_rb_id_in_release_list(sys_modem_as_id_e_type wrrc_as_id,rrc_RB_Identity rb_id)
{
  rrc_rb_search_e_type rb_srch = RRC_RB_PRESENT;
  /*SRB0 through SRB3 should not be part of the release list*/
  if ((rb_id == CCCH_RADIO_BEARER_ID) ||
      (rb_id == DCCH_UM_RADIO_BEARER_ID) ||
      (rb_id == DCCH_AM_RADIO_BEARER_ID) ||
      (rb_id == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID))
  {
    rb_srch = RRC_RB_NOT_PRESENT;
  }
  return rb_srch;
}


/*===========================================================================

FUNCTION   RRC_REMOVE_RAB_FROM_ESTABLISHED_RABS

DESCRIPTION

  This function removes the given RAB id if it's present in the
  "ESTABLISHED_RABS" variable. 
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_remove_rab_from_established_rabs
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_rab_id_type rab_id /* Id of the RAB */
)
{
  uint32 count; /* Local Counter */
  boolean rab_found=FALSE; /* To keep track of whether the
                           rab_id was found */
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( rrc_est_rabs.rabs[count].rab_id == rab_id )
    {
      WRRC_MSG6_HIGH("RAB removed id:%d,rbs:%d,idx:%d, domain:%d,re-est tmr:%d,gsm_map:%d",
            rrc_est_rabs.rabs[count].rab_id,
            rrc_est_rabs.rabs[count].num_rbs_for_rab,
            count,
            rrc_est_rabs.rabs[count].cn_domain,
            rrc_est_rabs.rabs[count].re_est_timer,
            rrc_est_rabs.rabs[count].rab_id_is_gsm_map
            );
     
      if((rrc_est_rabs.rabs[count].rab_type == RRCRB_CS_VOICE_RAB) &&
           (rrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID))
      {
        
        /* Deconfigure MVS to deregister RLC queues for AMR Call.*/
        rrc_deconfigure_voice_adapter(wrrc_as_id);
        WRRC_MSG0_HIGH("Clear RABs -- Release WV Adapter chan AND reset the AMR codec status");

        /* Reset the current codec to the default mode */
        rrc_set_current_codec_mode(wrrc_as_id, WCDMA_IVOCODER_ID_AMR);
        rrc_set_ordered_codec_mode(wrrc_as_id, WCDMA_IVOCODER_ID_NONE);
      }
      
      /* Make the RAB ID invalid */
      rrc_est_rabs.rabs[count].rab_id = RRC_INVALID_RAB_ID;
      rrc_est_rabs.rabs[count].rab_type = RRCRB_UNKNOWN_RAB;
      rrc_est_rabs.rabs[count].num_rbs_for_rab = 0;
      rab_found=TRUE;
      break;
    }
  }
  if( rab_found == FALSE )
  {
    WRRC_MSG0_ERROR("RAB not found in ESTABLISHED_RABS!");
  }
}




/*===========================================================================

FUNCTION   RRC_REMOVE_RB_FROM_ESTABLISHED_RABS

DESCRIPTION

  This function removes the given RB id if it's present in the
  "ESTABLISHED_RABS" variable. If the removal of the RB causes
  a RAB to have no RBs mapped to it, the RAB will also be
  removed. 
        
DEPENDENCIES

  Pointer to hold the results must be initialized by the
  calling function.
 
RETURN VALUE

  None. (Results ptr is filled out by this function).

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_remove_rb_from_established_rabs
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32 rb_id,                  /* RB id to be removed */
  rrc_rb_remove_result_type *ptr /* Pointer for holding the results */
)
{
  uint32 count;  /* Local Counter */
  uint32 count1; /* Local Counter */

  /* Look into each RAB that has been established */
  for( count=0; count < MAX_RAB_TO_SETUP; count++ )
  {
    /* If the RAB is valid and it has atleast one RB
    mapped to it, we should try to find a match for this RB */
    if( (rrc_est_rabs.rabs[count].rab_id != RRC_INVALID_RAB_ID) &&
        (rrc_est_rabs.rabs[count].num_rbs_for_rab > 0)
      )
    {
      /* This RAB is valid. Look at each RB within the RAB */
      for( count1=0; count1<MAX_RB_PER_RAB; count1++ )
      {
        if( rrc_est_rabs.rabs[count].rb_for_rab[count1].rb_id == rb_id )
        {
          /* Match is found. Remove this RB and decrement
          the number of RBs for this RAB */
          WRRC_MSG3_HIGH("RB removed rbid:%d, rabid:%d, domain:%d",
                     rb_id,
                     rrc_est_rabs.rabs[count].rab_id,
                     rrc_est_rabs.rabs[count].cn_domain
                   );
          rrc_est_rabs.rabs[count].rb_for_rab[count1].rb_id = RRC_INVALID_RB_ID;
          rrc_est_rabs.rabs[count].num_rbs_for_rab--;
          if( rrc_est_rabs.rabs[count].num_rbs_for_rab == 0 )
          {
            /* This is an AMR RAB. Release the AMR WM. It could be possible */
            /* to attempt to release the AMR WM when the CS-data is         */
            /* releasing. But this should have no harm.                     */
            if ((rrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID) &&
                (rrc_est_rabs.rabs[count].rab_type == RRCRB_CS_VOICE_RAB))
            {
              /* Deconfigure MVS to deregister RLC queues for AMR Call.*/
              rrc_deconfigure_voice_adapter(wrrc_as_id);
              WRRC_MSG0_HIGH("Clear RABs -- Release MVS chan AND reset the AMR codec status");

              /* Reset the current codec to the default mode */
              rrc_set_current_codec_mode(wrrc_as_id, WCDMA_IVOCODER_ID_AMR);
              rrc_set_ordered_codec_mode(wrrc_as_id, WCDMA_IVOCODER_ID_NONE);
            }
            ptr->rab_type = rrc_est_rabs.rabs[count].rab_type;
            ptr->rab_id = rrc_est_rabs.rabs[count].rab_id;
            ptr->cn_domain = rrc_est_rabs.rabs[count].cn_domain;
            rrc_est_rabs.rabs[count].rab_id = RRC_INVALID_RAB_ID;
            rrc_est_rabs.rabs[count].rab_type = RRCRB_UNKNOWN_RAB;            
            ptr->result = RRC_RB_AND_RAB_REMOVED;

#ifdef FEATURE_INACTIVITY_HANDLING
            if (rrc_est_rabs.rabs[count].cn_domain == RRC_PS_DOMAIN_CN_ID)
            {
              if (rrc_get_state(wrrc_as_id) == RRC_STATE_CELL_DCH) 
              {
                WRRC_MSG0_HIGH("Deleting PS RAB, Restarting inact timer");
                rrctmr_start_timer(wrrc_as_id,RRCTMR_INACTIVITY_TIMER, INACTIVITY_TIMER_IN_MS);
              }
              else if (rrc_get_state(wrrc_as_id) == RRC_STATE_CELL_FACH)
              {
                WRRC_MSG3_HIGH("Deleting PS RAB, Restarting Inactivity timer %d + remain T305 %d actual T305 value %d",
                         INACTIVITY_TIMER_IN_MS,
                         rrctmr_get_remaining_time(wrrc_as_id,RRCTMR_T_305_TIMER),
                         rrc_convert_t305_to_ms(wrrc_as_id,RRC_GET_T305()));
                /* Restart incativity handling timer */
               if(rrc_convert_t305_to_ms(wrrc_as_id,RRC_GET_T305()) != RRCTMR_INFINITY_VAL )
               {
                 rrctmr_start_timer(wrrc_as_id,RRCTMR_INACTIVITY_TIMER, 
                                  (unsigned long)(INACTIVITY_TIMER_IN_MS + rrctmr_get_remaining_time(wrrc_as_id,RRCTMR_T_305_TIMER))); 
               }
               else
               {
                 /*If T305 timer was infinity then the timer wouldnt have been started . In thats case start
                    timer of 5 mins plus 24 hrs*/
                 rrctmr_start_timer(wrrc_as_id,RRCTMR_INACTIVITY_TIMER, 
                                  (unsigned long)(INACTIVITY_TIMER_IN_MS + RRCTMR_INFINITY_VAL)); 
               }
              }
              (void)rrcmisc_handle_ps_rab_inactivity(sys_modem_as_id_e_type wrrc_as_id, 1);
            }
#endif /* FEATURE_INTACTIVITY_HANDLING */
            return;
          }
          else
          {
            ptr->result = RRC_RB_REMOVED;
            /*After releasing RBs, if RAB still exists, update rab_id in the return pointer.*/
            ptr->rab_id = rrc_est_rabs.rabs[count].rab_id;
            ptr->rab_type = rrc_est_rabs.rabs[count].rab_type;
            return;
          }
        }
      }
    }
  }
  /* If we get here, we never found that RB id */
  ptr->result = RRC_RB_NOT_FOUND;
}         








/*===========================================================================

FUNCTION   rrc_send_rab_rel_ind_on_ps_release

DESCRIPTION

  This function will send a rab-rel-ind which will follow immediately after 
  RRC_REL_IND for PS rabs.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_send_rab_rel_ind_on_ps_release(sys_modem_as_id_e_type wrrc_as_id)
{
  int32 count; /* Local counter */
  rabm_cmd_type *rabm_ptr; /* To send the RABMAS_RAB_RELEASE_IND  command */
  
  for( count=0; count < MAX_RAB_TO_SETUP; count++ )
  {
    if( (rrc_est_rabs.rabs[count].rab_id != RRC_INVALID_RAB_ID) &&
          (rrc_est_rabs.rabs[count].cn_domain == RRC_PS_DOMAIN_CN_ID))
    {
      if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
      {
        WRRC_ERR0_FATAL("Can't send RABM cmd - out of memory!");
      }
      rabm_ptr->header.cmd_id = RABMAS_RAB_RELEASE_IND;
      rabm_ptr->header.message_set = MS_RABM_RRC;
      rabm_ptr->cmd.rrc_release_ind.rabid = (rabid_T)(rrc_est_rabs.rabs[count].rab_id);
  #ifdef FEATURE_DUAL_SIM
      rabm_ptr->cmd.rrc_release_ind.as_id = rrc_get_as_id(wrrc_as_id);
  #endif
      /* Put the command on RABM queue */
      WRRC_MSG1_HIGH("Sending RABMAS_RAB_RELEASE_IND to RABM rabid %d", rrc_est_rabs.rabs[count].rab_id);
      rrc_crash_debug_save_misc_cmds(wrrc_as_id,RRC_RABMAS_RAB_RELEASE_IND);
      rabm_put_cmd(rabm_ptr);
    }
  }
}

/*===========================================================================

FUNCTION   RRC_CLEAR_ESTABLISHED_RABS

DESCRIPTION

  This function clears the "ESTABLISHED_RABS" variable. All the RABS
  will be removed from the variable. This is to be used when the RRC
  connection is released or if all the RABs have to be released due to
  some failure condition.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_clear_established_rabs(sys_modem_as_id_e_type wrrc_as_id)
{
  uint32 count; /* Local counter */
   
  /* Make all RAB Ids invalid to indicate the indices
  are no longer used. */
  for( count=0; count < MAX_RAB_TO_SETUP; count++ )
  {
    if( (rrc_est_rabs.rabs[count].rab_id != RRC_INVALID_RAB_ID) &&
        (rrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID)  &&
        (rrc_est_rabs.rabs[count].rab_type == RRCRB_CS_VOICE_RAB)
      )
    {
         
      /* Deconfigure MVS to deregister RLC queues for AMR Call.*/
      rrc_deconfigure_voice_adapter(wrrc_as_id);
      WRRC_MSG0_HIGH("WV_WRRC: Clear RABs -- Release WV Adapater chan AND reset the AMR codec status");
    }
    rrc_est_rabs.rabs[count].rab_id = RRC_INVALID_RAB_ID;
    rrc_est_rabs.rabs[count].num_rbs_for_rab = 0;
    rrc_est_rabs.rabs[count].rab_type = RRCRB_UNKNOWN_RAB;
  }

      rrc_set_current_codec_mode(wrrc_as_id, WCDMA_IVOCODER_ID_AMR);
      rrc_set_ordered_codec_mode(wrrc_as_id, WCDMA_IVOCODER_ID_NONE);

}

/*===========================================================================

FUNCTION          RRC_TRANSLATE_URNTI


DESCRIPTION       

  This function translates U-RNTI type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  Both the internal and external U-RNTI pointers must be initialized.
  If either one is NULL, the convertion will fail.

RETURN VALUE
                  
  None

SIDE EFFECTS
                  
  None

===========================================================================*/
void rrc_translate_urnti
(
  sys_modem_as_id_e_type wrrc_as_id, 
  struct rrc_U_RNTI  *ext_urnti,     /* Pointer to external U-RNTI type */
  rrc_int_u_rnti_type  *int_urnti    /* Pointer to internal U-RNTI type */
)

{
  uint32 temp_srnti;                 /* Local parameter to store the 
                                        S-RNTI value */
  /* Make sure the internal and external pointers are initialized */
  if( ext_urnti == NULL )
  {
    WRRC_MSG0_ERROR("External URNTI not initialized");
    return;
  }
  if( int_urnti == NULL )
  {
    WRRC_MSG0_ERROR("Internal URNTI not initialized");
    return;
  }

  if(ext_urnti->srnc_Identity.numbits != 12 )
  {
    WRRC_MSG1_ERROR("Received SRNC Identity Length: %d is invalid",
               ext_urnti->srnc_Identity.numbits);
  }
  if(ext_urnti->s_RNTI.numbits != 20 )
  {
    WRRC_MSG1_ERROR("Received S_RNTI Length: %d is invalid",
               ext_urnti->s_RNTI.numbits);
  }
  /* Convert 12 bit SRNC identify into internal format */
  *int_urnti = *(ext_urnti->srnc_Identity.data) << 8;
  *int_urnti |= *(ext_urnti->srnc_Identity.data + 1);

  /* Move the 12 bit SRNC Identity value to the MSB side */
  *int_urnti = *int_urnti << 16;

  /* Make sure the remaining bits of the int_urnti are clear */
  *int_urnti &= 0xFFF00000;

  /* Convert 20 bit S-RNTI value into internal format */
  temp_srnti = *(ext_urnti->s_RNTI.data) << 8;
  temp_srnti |= *(ext_urnti->s_RNTI.data + 1);
  temp_srnti = temp_srnti << 8;
  temp_srnti |= *(ext_urnti->s_RNTI.data + 2);

  /* Get the SRNTI into the lower 20 bits */
  temp_srnti = temp_srnti >> 4;

  /* Append SRNC Identity and S-RNTI to build U-RNTI */
  *int_urnti = *int_urnti | temp_srnti;

}

/*===========================================================================

FUNCTION          RRC_TRANSLATE_URNTI_SHORT


DESCRIPTION       

  This function translates U-RNTI Short type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  Both the internal and external U-RNTI pointers must be initialized.
  If either one is NULL, the convertion will fail.

RETURN VALUE
                  
  None

SIDE EFFECTS
                  
  None

===========================================================================*/
void rrc_translate_urnti_short
(
  sys_modem_as_id_e_type wrrc_as_id, 
  struct rrc_U_RNTI_Short  *ext_urnti,   /* Pointer to external U-RNTI type */
  rrc_int_u_rnti_type      *int_urnti    /* Pointer to internal U-RNTI type */
)
{
  struct rrc_U_RNTI urnti;
  uint32 srnti;


  /* We need space allocated to hold the expanded 20-bit version of the S-RNTI
     that's about to be passed to the rrc_translate_urnti() function.  That's
     what the array above is for, so we need to hook it up to the struct
     we've declared. */
  memset(&(urnti.s_RNTI.data), 0, sizeof(urnti.s_RNTI.data));
  //Sumit
  //urnti.s_RNTI.data = srnti_bits;

  /* Convert the "S-RNTI 2" to an "S-RNTI" by padding with 10 zero bits in
     the most significant positions, and then call the existing function
     that translates a U-RNTI from the ASN.1 format to the RRC internal
     format. */
  urnti.s_RNTI.numbits = 20;
  srnti = *(ext_urnti->s_RNTI_2.data) << 8;
  srnti |= *(ext_urnti->s_RNTI_2.data + 1);
  /* Move over 6 bits so we have 10 bits of zero-padding at the top instead
     of 16. */
  srnti <<= 6;
  *(urnti.s_RNTI.data) = (uint8)((srnti & 0xff000000) >> 24);
  *(urnti.s_RNTI.data + 1) = (uint8)((srnti & 0x00ff0000) >> 16);
  *(urnti.s_RNTI.data + 2) = (uint8)((srnti & 0x0000ff00) >> 8);
  urnti.srnc_Identity.numbits = ext_urnti->srnc_Identity.numbits;
  WCDMA_MEMCPY(urnti.srnc_Identity.data,
               sizeof(urnti.srnc_Identity.data),
               ext_urnti->srnc_Identity.data,
               sizeof(urnti.srnc_Identity.data));
  rrc_translate_urnti(wrrc_as_id,&urnti, int_urnti);
}

/*===========================================================================

FUNCTION          RRC_TRANSLATE_CRNTI


DESCRIPTION       

  This function translates C-RNTI type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  Both the internal and external C-RNTI pointers must be initialized.
  If either one is NULL, the convertion will fail.

RETURN VALUE
                  
  None

SIDE EFFECTS
                  
  None

===========================================================================*/
void rrc_translate_crnti
(
  sys_modem_as_id_e_type wrrc_as_id, 
  ASN1BitStr32  *ext_crnti,          /* Pointer to external C-RNTI type */
  rrc_int_c_rnti_type  *int_crnti    /* Pointer to internal C-RNTI type */
)

{
                               
  /* Make sure the internal and external pointers are initialized */
  if( ext_crnti == NULL )
  {
    WRRC_MSG0_ERROR("External CRNTI not initialized");
    return;
  }
  if( int_crnti == NULL )
  {
    WRRC_MSG0_ERROR("Internal CRNTI not initialized");
    return;
  }

  if(ext_crnti->numbits!= 16 )
  {
    WRRC_MSG1_ERROR("Received C-RNTI Length: %d is invalid",
               ext_crnti->numbits);
  }

  /* Convert 16 bit C-RNTI into internal format */
  *int_crnti = *(ext_crnti->data) << 8;
  *int_crnti |= *(ext_crnti->data+ 1);

}

/*===========================================================================

FUNCTION          RRC_TRANSLATE_URA_ID


DESCRIPTION       

  This function translates URA_ID type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  Both the internal and external URA_ID pointers must be initialized.
  If either one is NULL, the conversion will return .

RETURN VALUE
                  
  None

SIDE EFFECTS
                  
  None

===========================================================================*/
void rrc_translate_ura_id
(
  sys_modem_as_id_e_type wrrc_as_id, 
  ASN1BitStr32  *ext_ura_id,          /* Pointer to external C-RNTI type */
  rrc_int_ura_id_type  *int_ura_id    /* Pointer to internal C-RNTI type */
)

{
                               
  /* Make sure the internal and external pointers are initialized */
  if( ext_ura_id == NULL )
  {
    WRRC_MSG0_ERROR("External URA_ID not initialized");
    return;
  }
  if( int_ura_id == NULL )
  {
    WRRC_MSG0_ERROR("Internal URA_ID not initialized");
    return;
  }

  if(ext_ura_id->numbits!= 16 )
  {
    WRRC_MSG1_ERROR("Received URA_ID Length: %d is invalid",
               ext_ura_id->data);
  }

  /* Convert 16 bit URA_ID into internal format */
  *int_ura_id = *(ext_ura_id->data) << 8;
  *int_ura_id |= *(ext_ura_id->data + 1);

}



/*===========================================================================

FUNCTION          RRC_CONVERT_GSM_MAP_IMSI_INTO_INT_VALUE


DESCRIPTION       

  This function translates a GSM-MAP IMSI internal type  into
  an interger value type used by RRC LLC. 

DEPENDENCIES
  None

RETURN VALUE
                  
  uint64: returns an integer value of the IMSI

SIDE EFFECTS
                  
  None

===========================================================================*/
uint64 rrc_convert_gsm_map_imsi_into_int_value 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_imsi_gsm_map_type  int_imsi    /* Pointer to internal IMSI type */
)
{
  /* local index */
  uint32 idx;                        /* Local index */

  /* IMSI return value */
  uint64 value = 0;

  for(idx = 0; idx < int_imsi.num_digits; idx++)
  {
    value = (value * 10) + int_imsi.value[idx];
  }

  return value;
} /* rrc_convert_gsm_map_imsi_into_int_value() */
/*===========================================================================

FUNCTION          RRC_CONVERT_PTMSI_MAP_INTO_INT_VALUE


DESCRIPTION

  This function translates a PTMSI internal type  into
  an interger value type used by RRC LLC.

  FROM 25.304
  For UE-Id types "TMSI and LAI (GSM-MAP)" or "P-TMSI and RAI (GSM-MAP)", only
  the TMSI or P-TMSI parts given as Bitstring(32) shall be used, and "Initial
  UE Identity" shall be interpreted as a binary representation of an integer
  number, where the first bit in the Bitstring represents the highest order bit.

DEPENDENCIES
  None

RETURN VALUE

  uint64: returns an integer value of the PTMSI

SIDE EFFECTS

  None

===========================================================================*/
uint64 rrc_convert_ptmsi_map_into_int_value
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_ptmsi_gsm_map_type  int_ptmsi    /* Pointer to internal PTMSI type */
)
{
  /* local index */
  uint32 idx;                        /* Local index */

  /* IMSI return value */
  uint64 value = 0;

  for(idx = 0; idx < RRC_MAX_GSMMAP_PTMSI_LENGTH; idx++)
  {
    value = value << 8;
    value = int_ptmsi.value[idx] | value;
  }

  return value;
} /* rrc_convert_ptmsi_map_into_int_value() */
/*===========================================================================

FUNCTION          RRC_CONVERT_TMSI_MAP_INTO_INT_VALUE


DESCRIPTION       

  This function translates a TMSI internal type  into
  an interger value type used by RRC LLC. 
  
  FROM 25.304 
  For UE-Id types "TMSI and LAI (GSM-MAP)" or "P-TMSI and RAI (GSM-MAP)", only 
  the TMSI or P-TMSI parts given as Bitstring(32) shall be used, and "Initial 
  UE Identity" shall be interpreted as a binary representation of an integer 
  number, where the first bit in the Bitstring represents the highest order bit.  

DEPENDENCIES
  None

RETURN VALUE
                  
  uint64: returns an integer value of the TMSI

SIDE EFFECTS
                  
  None

===========================================================================*/
uint64 rrc_convert_tmsi_map_into_int_value 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_tmsi_gsm_map_type  int_tmsi    /* Pointer to internal TMSI type */
)
{
  /* local index */
  uint32 idx;                        /* Local index */

  /* IMSI return value */
  uint64 value = 0;

  for(idx = 0; idx < RRC_MAX_GSMMAP_TMSI_LENGTH; idx++)
  {
    value = value << 8;
    value = int_tmsi.value[idx] | value;
  }

  return value;
} /* rrc_convert_tmsi_map_into_int_value() */
/*===========================================================================

FUNCTION          RRC_CONVERT_IMEI_INTO_INT_VALUE


DESCRIPTION       
  This function translates a IMEI internal type  into  an interger value.

DEPENDENCIES
  None

RETURN VALUE
                  
  uint64: returns an integer value of the IMEI

SIDE EFFECTS
                  
  None

===========================================================================*/
uint64 rrc_convert_imei_into_int_value 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_imei_gsm_map_type  int_imei    /* Local variable internal IMEI type */

)
{
  /* local index */
  uint32 idx;

  /* IMEI return value */
  uint64 value = 0;

  /* get the integer value of the imei */
  for(idx = 0; idx < RRC_NUM_IMEI_DIGITS ; idx++)
  {
    value = (value * 10) + int_imei.value[idx];
  }

  return value;
} /* rrc_convert_imei_into_int_value() */

/*===========================================================================

FUNCTION rrc_convert_initial_ue_identity_into_int_value

DESCRIPTION
  This function converts Initial UE Identity from internal format to integer value.


DEPENDENCIES
  None

RETURN VALUE
  uint32: returns an integer value of the Initial UE dentity

SIDE EFFECTS
  None
===========================================================================*/
uint64 rrc_convert_initial_ue_identity_into_int_value
(
  sys_modem_as_id_e_type wrrc_as_id
)
{

  uint64 value;                                /* Local parameter to store the 
                                                  IMSI value */    
               
  switch(rrcdata_sim_related_info.rrc_initial_ue_identity_choice)
  {

    case RRC_IMSI_UE_ID:
      /* Convert IMSI value to the integer value */
      value = rrc_convert_gsm_map_imsi_into_int_value(wrrc_as_id,
                rrcdata_sim_related_info.rrc_initial_ue_identity.imsi);
      break;
 
    case RRC_TMSI_LAI_UE_ID:
      /* Convert tmsi_lai value to the integer value */
      value = rrc_convert_tmsi_map_into_int_value(wrrc_as_id,
                rrcdata_sim_related_info.rrc_initial_ue_identity.tmsi_lai.tmsi);      
      break;

    case RRC_PTMSI_RAI_UE_ID:
      /* Convert ptmsi_lai value to the integer value */
      value = rrc_convert_ptmsi_map_into_int_value(wrrc_as_id,
                rrcdata_sim_related_info.rrc_initial_ue_identity.ptmsi_rai.ptmsi);      
      break;

    case RRC_IMEI_UE_ID:
      /* Convert IMEI value to the integer value */
      value = rrc_convert_imei_into_int_value(wrrc_as_id,rrcdata_sim_related_info.rrc_initial_ue_identity.imei);      
      break; 

    default:
      WRRC_MSG1_ERROR("Unknown Intital UE Identity is sent",
           rrcdata_sim_related_info.rrc_initial_ue_identity_choice);
      value = 0;
      break;
  }      
  return (value);
} /* rrc_convert_initial_ue_identity_into_int_value */


/*===========================================================================

FUNCTION rrc_log_map_protocol_cause

DESCRIPTION
  This function maps the asn1 cause to an internal macros

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
uint8 rrc_log_map_protocol_cause
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_ProtocolErrorCause prot_err_cause
)
{
#if(!defined(T_WINNT))

uint8 protocol_error_cause = RRCLOG_ASN1_ENC_ERR;

  switch (prot_err_cause)
  {
    case rrc_ProtocolErrorCause_asn1_ViolationOrEncodingError:
      protocol_error_cause = RRCLOG_ASN1_ENC_ERR;
      break;

    case rrc_ProtocolErrorCause_messageTypeNonexistent:
      protocol_error_cause = RRCLOG_BAD_MSG_TYPE;
      break;

    case rrc_ProtocolErrorCause_messageNotCompatibleWithReceiverState:
      protocol_error_cause = RRCLOG_INCOMPAT_RX_STATE;
      break;

    case rrc_ProtocolErrorCause_ie_ValueNotComprehended:
      protocol_error_cause = RRCLOG_BAD_IE;
      break;

    case rrc_ProtocolErrorCause_informationElementMissing:
      protocol_error_cause = RRCLOG_COND_IE_ERROR;
      break;

    case rrc_ProtocolErrorCause_messageExtensionNotComprehended:
      protocol_error_cause = RRCLOG_BAD_MSG_EXTENSION;
      break;

    default:
      WRRC_MSG1_ERROR("unknown prot_err_cause %d",prot_err_cause);
      break;
  }

WRRC_MSG1_HIGH("RRC prot_err_cause %d",prot_err_cause);
return (protocol_error_cause);

#else  
#error code not present
#endif 
}/*rrc_log_map_protocol_cause*/

/*===========================================================================

FUNCTION rrc_log_map_procedure_name

DESCRIPTION
  This function maps RRC procedure name to Apex procedure name

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
uint8 rrc_log_map_procedure_name
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint8 rrc_procedure            /* The RRC procedure */
)
{
#if(!defined(T_WINNT))

  rrc_proc_e_type rrc_proc = RRC_PROCEDURE_NONE;           /* RRC procedure */

  switch((rrc_proc_e_type)rrc_procedure)
  {

    case RRC_PROCEDURE_CSP:
      rrc_proc = RRC_PROCEDURE_NONE;
      break;

    case RRC_PROCEDURE_SIB:
      rrc_proc = RRC_PROCEDURE_CSP;
      break;

    case RRC_PROCEDURE_PG2:
      rrc_proc = RRC_PROCEDURE_CCM;
      break;

    case RRC_PROCEDURE_MCMR:
      rrc_proc = RRC_PROCEDURE_SIB;
      break;

    case RRC_PROCEDURE_RCE:
      rrc_proc = RRC_PROCEDURE_PG1;
      break;

    case RRC_PROCEDURE_RCR:
      rrc_proc = RRC_PROCEDURE_RCE;
      break;

    case RRC_PROCEDURE_UECI:
      rrc_proc = RRC_PROCEDURE_RCR;
      break;

    case RRC_PROCEDURE_UECE:
      rrc_proc = RRC_PROCEDURE_RCRE;
      break;

    case RRC_PROCEDURE_IDT:
      rrc_proc = RRC_PROCEDURE_UECI;
      break;

    case RRC_PROCEDURE_UDT:
      rrc_proc = RRC_PROCEDURE_UECE;
      break;

    case RRC_PROCEDURE_DDT:
      rrc_proc = RRC_PROCEDURE_IDT;
      break;

    case RRC_PROCEDURE_SCR:
      rrc_proc = RRC_PROCEDURE_UDT;
      break;

    case RRC_PROCEDURE_SCRR:
      rrc_proc = RRC_PROCEDURE_DDT;
      break;

    case RRC_PROCEDURE_CC:
      rrc_proc = RRC_PROCEDURE_PG2;
      break;

    case RRC_PROCEDURE_RBE:
      rrc_proc = RRC_PROCEDURE_SMC;
      break;

    case RRC_PROCEDURE_RBRC:
      rrc_proc = RRC_PROCEDURE_SCR;
      break;

    case RRC_PROCEDURE_RBR:
      rrc_proc = RRC_PROCEDURE_SCRR;
      break;

    case RRC_PROCEDURE_TCR:
      rrc_proc = RRC_PROCEDURE_CC;
      break;

    case RRC_PROCEDURE_PCR:
      rrc_proc = RRC_PROCEDURE_RBE;
      break;

    case RRC_PROCEDURE_TFC:
      rrc_proc = RRC_PROCEDURE_RBRC;
      break;

    case RRC_PROCEDURE_CU:
      rrc_proc = RRC_PROCEDURE_RBR;
      break;

    case RRC_PROCEDURE_UU:
      rrc_proc = RRC_PROCEDURE_TCR;
      break;

    case RRC_PROCEDURE_UMI:
      rrc_proc = RRC_PROCEDURE_TFC;
      break;

    case RRC_PROCEDURE_ASU:
      rrc_proc = RRC_PROCEDURE_PCR;
      break;

    case RRC_PROCEDURE_ISHFU:
      rrc_proc = RRC_PROCEDURE_CU;
      break;

    case RRC_PROCEDURE_ISHTU:
      rrc_proc = RRC_PROCEDURE_UU;
      break;

    case RRC_PROCEDURE_ISCFU:
      rrc_proc = RRC_PROCEDURE_UMI;
      break;

    case RRC_PROCEDURE_ISCTU:
      rrc_proc = RRC_PROCEDURE_ASU;
      break;

    case RRC_PROCEDURE_PG1:
      rrc_proc = RRC_PROCEDURE_ISHTU;
      break;

    case RRC_PROCEDURE_SMC:
      rrc_proc = RRC_PROCEDURE_ISHFU;
      break;
    default:
      WRRC_MSG1_ERROR("Protocol error logging is not supported for %d",rrc_procedure);
      break;
  }
  WRRC_MSG1_HIGH("RRC procedure is %d",rrc_procedure);

  return (uint8)rrc_proc;
#else  
#error code not present
#endif 
}/*rrc_log_map_procedure_name*/
/*===========================================================================

FUNCTION rrc_log_protocol_error

DESCRIPTION
  This function logs the RRC Failure Cause and Protocol error cause if applicable.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void rrc_log_protocol_error
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint8 rrc_state,                /* The RRC state */
  uint8 rrc_procedure,            /* The RRC procedure */
  uint8 failure_cause,            /* The failure cause */
  uint8 protocol_error_cause     /* The protocol error cause */  
)
{

  rrclog_wcdma_rrc_failure_event_type log_failure_event;  /* Local variable to store Log record */

  log_failure_event.rrc_state                 = rrc_state;
  log_failure_event.rrc_proc                  = rrc_procedure;  /*Is call to rrc_log_map_procedure_name(rrc_procedure) needed?;*/
  log_failure_event.rrc_failure_cause         = failure_cause;
  
  if ((failure_cause == RRCLOG_failureCause_protocolError) || 
      (failure_cause == RRCLOG_InterRatfailureCause_protocolError)
     )
  {
    log_failure_event.protocol_error_cause      = protocol_error_cause;
  }
  else
  {
    log_failure_event.protocol_error_cause      = RRCLOG_PROTOCOL_ERROR_NA;
  }
  /* Is it good to pass local variable to below function argument. 
     In the existing code it is already happenning like this at other places,
     so should be OK here as well */    
  WRRC_EVENT_REPORT_PAYLOAD(EVENT_WCDMA_RRC_FAILURE, sizeof(log_failure_event),
                            (void *)&log_failure_event);

}/*rrc_log_protocol_error*/
/*===========================================================================

FUNCTION    rrc_get_dl_ccch_message_type 

DESCRIPTION
  This function looks into choice field and identifies the Message type and 
  then returns the index to to the Message Type.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns index to the Message type if successful. Otherwise it
          returns 0xFF.

SIDE EFFECTS
  None

===========================================================================*/
uint8 rrc_get_dl_ccch_message_type(sys_modem_as_id_e_type wrrc_as_id,
  uint8 *dl_sdu,                            /* Pointer received byte stream */
  uint32 dl_sdu_length                      /* Length of SDU received */
)
{      
  
  uint8 msg_type = 0xFF;                  /* Stores type of DL CCCH message  
                                             and initialized to none. */ 
  uint16 message_type_pos;                /* Local Variable to store message 
                                             type position */                                 
  
  /* Check whether Integrity Check info is present */
  if(0x00 == b_unpackb(dl_sdu, INTEGRITY_CHECK_INFO_PRESENT_POS, 
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (dl_sdu_length * 8) < 
        (NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         NUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE) )
    {
      return (msg_type);
    }
    message_type_pos = INTEGRITY_CHECK_INFO_PRESENT_POS +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    /*Peek into byte stream and get the message choice */
    msg_type = 
      b_unpackb(dl_sdu, message_type_pos,
                NUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE);
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if( 0x01 == b_unpackb(dl_sdu, INTEGRITY_CHECK_INFO_PRESENT_POS, 
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (dl_sdu_length * 8) < 
        (NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
         NUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE) )
    {
      return (msg_type);
    }
    message_type_pos = INTEGRITY_CHECK_INFO_PRESENT_POS +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO;

    /*Peek into byte stream and get the message choice */
     msg_type =
      b_unpackb(dl_sdu, message_type_pos,
                NUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE);
  }
  else
  {
   
    WRRC_ERR1_FATAL("Invalid Integrity Check Info Present %d",
                msg_type);
  }
  /* Return value is incremented by 1 because OSS uses choice 1 for the first choice. 
     Actually choice is 0 while peeking into the message */
  return(msg_type + 1);
} /* end function rrc_get_dl_ccch_message_type */
/*===========================================================================

FUNCTION    rrc_get_dl_dcch_message_type 

DESCRIPTION
  This function looks into choice field and identifies the Message type and 
  then returns the index to to the Message Type.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns index to the Message type if successful. Otherwise it
          returns 0xFF.
SIDE EFFECTS
  None

===========================================================================*/
uint8 rrc_get_dl_dcch_message_type(sys_modem_as_id_e_type wrrc_as_id,
  uint8 *dl_sdu,                            /* Pointer received byte stream */
  uint32 dl_sdu_length                      /* Length of SDU received */
)
{      
  
  uint8 msg_type = 0xFF;                  /* Stores type of DL DCCH message  
                                             and initialized to none. */

  uint16 message_type_pos;                /* Local Variable to store message 
                                             type position */      
                                    
  /* Check whether Integrity Check info is present */
  if(0x00 == b_unpackb(dl_sdu, INTEGRITY_CHECK_INFO_PRESENT_POS, 
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (dl_sdu_length * 8) < 
        (NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         NUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE) )
    {
      return (msg_type);
    }

    message_type_pos = INTEGRITY_CHECK_INFO_PRESENT_POS +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    /*Peek into byte stream and get the message choice */
    msg_type = 
      b_unpackb(dl_sdu, message_type_pos,
                NUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE);
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if( 0x01 == b_unpackb(dl_sdu, INTEGRITY_CHECK_INFO_PRESENT_POS, 
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( ( dl_sdu_length * 8) < 
        (NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
         NUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE) )
    {
      return (msg_type);
    }

    message_type_pos = INTEGRITY_CHECK_INFO_PRESENT_POS +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT+
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO;
    /*Peek into byte stream and get the message choice */
     msg_type = 
      b_unpackb(dl_sdu, message_type_pos,
                NUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE);
  }
  else
  {
   
    WRRC_ERR1_FATAL("Invalid Integrity Check Info Present %d",
                msg_type);
  }
  /* Return value is incremented by 1 because OSS uses choice 1 for the first choice. 
     Actually choice is 0 while peeking into the message */
  return(msg_type + 1);
} /* end function rrc_get_dl_dcch_message_type */

/*===========================================================================

FUNCTION    rrc_get_ul_ccch_message_type 

DESCRIPTION
  This function looks into choice field and identifies the Message type and 
  then returns the index to to the Message Type.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns index to the Message type if successful. Otherwise it
          returns 0xFF.

SIDE EFFECTS
  None

===========================================================================*/
uint8 rrc_get_ul_ccch_message_type(sys_modem_as_id_e_type wrrc_as_id,
  uint8 *ul_sdu,                            /* Pointer received byte stream */
  uint32 ul_sdu_length                      /* Length of SDU received */
)
{      
  
  uint8 msg_type = 0xFF;                  /* Stores type of UL CCCH message  
                                             and initialized to none. */ 
  uint16 message_type_pos;                /* Local Variable to store message 
                                             type position */                                 
  
  /* Check whether Integrity Check info is present */
  if(0x00 == b_unpackb(ul_sdu, INTEGRITY_CHECK_INFO_PRESENT_POS, 
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (ul_sdu_length * 8) < 
        (NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         NUM_OF_BITS_FOR_UL_CCCH_MSG_TYPE) )
    {
      return (msg_type);
    }
    message_type_pos = INTEGRITY_CHECK_INFO_PRESENT_POS +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    /*Peek into byte stream and get the message choice */
    msg_type = 
      b_unpackb(ul_sdu, message_type_pos,
                NUM_OF_BITS_FOR_UL_CCCH_MSG_TYPE);
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if( 0x01 == b_unpackb(ul_sdu, INTEGRITY_CHECK_INFO_PRESENT_POS, 
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (ul_sdu_length * 8) < 
        (NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
         NUM_OF_BITS_FOR_UL_CCCH_MSG_TYPE) )
    {
      return (msg_type);
    }
    message_type_pos = INTEGRITY_CHECK_INFO_PRESENT_POS +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO;

    /*Peek into byte stream and get the message choice */
     msg_type =
      b_unpackb(ul_sdu, message_type_pos,
                NUM_OF_BITS_FOR_UL_CCCH_MSG_TYPE);
  }
  else
  {
   
    WRRC_ERR1_FATAL("Invalid Integrity Check Info Present %d",
                msg_type);
  }
  /* Return value is incremented by 1 because OSS uses choice 1 for the first choice. 
     Actually choice is 0 while peeking into the message */
  return(msg_type + 1);
} /* end function rrc_get_ul_ccch_message_type */
/*===========================================================================

FUNCTION    rrc_get_ul_dcch_message_type 

DESCRIPTION
  This function looks into choice field and identifies the Message type and 
  then returns the index to to the Message Type.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns index to the Message type if successful. Otherwise it
          returns 0xFF.
SIDE EFFECTS
  None

===========================================================================*/
uint8 rrc_get_ul_dcch_message_type(sys_modem_as_id_e_type wrrc_as_id,
  uint8 *ul_sdu,                            /* Pointer received byte stream */
  uint32 ul_sdu_length                      /* Length of SDU received */
)
{      
  
  uint8 msg_type = 0xFF;                  /* Stores type of UL DCCH message  
                                             and initialized to none. */

  uint16 message_type_pos;                /* Local Variable to store message 
                                             type position */      
                                    
  /* Check whether Integrity Check info is present */
  if(0x00 == b_unpackb(ul_sdu, INTEGRITY_CHECK_INFO_PRESENT_POS, 
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (ul_sdu_length * 8) < 
        (NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         NUM_OF_BITS_FOR_UL_DCCH_MSG_TYPE) )
    {
      return (msg_type);
    }

    message_type_pos = INTEGRITY_CHECK_INFO_PRESENT_POS +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    /*Peek into byte stream and get the message choice */
    msg_type = 
      b_unpackb(ul_sdu, message_type_pos,
                NUM_OF_BITS_FOR_UL_DCCH_MSG_TYPE);
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if( 0x01 == b_unpackb(ul_sdu, INTEGRITY_CHECK_INFO_PRESENT_POS, 
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( ( ul_sdu_length * 8) < 
        (NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
         NUM_OF_BITS_FOR_UL_DCCH_MSG_TYPE) )
    {
      return (msg_type);
    }

    message_type_pos = INTEGRITY_CHECK_INFO_PRESENT_POS +
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT+
                       NUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO;
    /*Peek into byte stream and get the message choice */
     msg_type = 
      b_unpackb(ul_sdu, message_type_pos,
                NUM_OF_BITS_FOR_UL_DCCH_MSG_TYPE);
  }
  else
  {
   
    WRRC_ERR1_FATAL("Invalid Integrity Check Info Present %d",
                msg_type);
  }
  /* Return value is incremented by 1 because OSS uses choice 1 for the first choice. 
     Actually choice is 0 while peeking into the message */
  return(msg_type + 1);
} /* end function rrc_get_ul_dcch_message_type */


/*===========================================================================

FUNCTION  rrc_update_periodic_wcdma_UI_events_info

DESCRIPTION

  This function is invoked when the RSSI&BER repoting timer expires. It calls the L1 to get RSSI and BER info,
  activet set info, monitored set info and populates the global data to report to CM

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void rrc_update_periodic_wcdma_UI_events_info
(
  sys_modem_as_id_e_type wrrc_as_id 
#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
  ,rrc_cmd_type *cmd_ptr 
#endif
)
{
  l1_serving_cell_power_info_type l1_serving_cell_power_info;
  l1_active_set_info_type l1_active_set_info;
  boolean call_cm_api = FALSE;
  rrcllc_oc_set_status_e_type oc_status = rrcllc_get_ordered_config_status_wo_f3(wrrc_as_id);
  rrcllc_toc_usage_e_type toc_usage = rrcllc_get_toc_usage(wrrc_as_id);
  rrc_state_e_type rrc_current_state = rrc_get_state(wrrc_as_id);

  if((RRC_IS_WCDMA_UI_EVENT_ENABLED(SYS_WCDMA_RRC_STATE)))
  {
    switch(rrc_current_state)
    {
      case RRC_STATE_DISCONNECTED:
      case RRC_STATE_CONNECTING:
        rrcdata_logging_info.wcdma_UI_event_info.wcdma_event_info.wcdma_rrc_state = SYS_WCDMA_RRC_STATE_DISCONNECTED;
        break;
  
      case RRC_STATE_CELL_FACH:
        rrcdata_logging_info.wcdma_UI_event_info.wcdma_event_info.wcdma_rrc_state = SYS_WCDMA_RRC_STATE_CELL_FACH;
        break;
  
      case RRC_STATE_CELL_DCH:
        rrcdata_logging_info.wcdma_UI_event_info.wcdma_event_info.wcdma_rrc_state = SYS_WCDMA_RRC_STATE_CELL_DCH;
        break;
  
      case RRC_STATE_CELL_PCH:
        rrcdata_logging_info.wcdma_UI_event_info.wcdma_event_info.wcdma_rrc_state = SYS_WCDMA_RRC_STATE_CELL_PCH;
        break;
  
      case RRC_STATE_URA_PCH:
        rrcdata_logging_info.wcdma_UI_event_info.wcdma_event_info.wcdma_rrc_state = SYS_WCDMA_RRC_STATE_URA_PCH;
        break;
  
      default:
        WRRC_MSG0_ERROR("We should never come here");
        break;
    }
  
    rrcdata_logging_info.wcdma_UI_event_info.bitmask |=SYS_WCDMA_RRC_STATE;
  
  }

  if((RRC_IS_WCDMA_UI_EVENT_ENABLED(SYS_WCDMA_SERVING_CELL_POWER_INFO_BIT_MASK)) &&
    ((rrc_current_state != RRC_STATE_CELL_DCH) ||
    ((rrc_current_state == RRC_STATE_CELL_DCH) &&
      ((oc_status != OC_SET_FOR_DCH_FACH_TRANS ) && (oc_status != OC_SET_FOR_DCH_CELL_PCH_TRANS)
        && (oc_status != OC_SET_FOR_DCH_URA_PCH_TRANS)) &&
      (toc_usage != TOC_FOR_OOS)))
  )
  {
    wcdma_serving_cell_power_info_type *serving_cell_power_info ;
    serving_cell_power_info = &(rrcdata_logging_info.wcdma_UI_event_info.wcdma_event_info.wcdma_serving_cell_power_info);
    if (rrc_ccm_get_curr_camping_status(wrrc_as_id,RRC_PROCEDURE_CSP) == RRC_CAMPED_ON)
    { 
#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
      /* Copy the Signalling mesaage payload to the log record */
      WCDMA_MEMCPY((void *)&(l1_serving_cell_power_info), 
                   sizeof(l1_serving_cell_power_info_type),
                   (void *)(cmd_ptr->cmd.modem_stat_rsp.l1_serving_cell_power_info),
                   sizeof(l1_serving_cell_power_info_type));
#else
      l1_get_serving_cell_power(&l1_serving_cell_power_info,wrrc_as_id);
#endif
      if(l1_serving_cell_power_info.is_valid)
      {
        serving_cell_power_info->psc = l1_serving_cell_power_info.cell_info.psc;
        serving_cell_power_info->rscp = l1_serving_cell_power_info.cell_info.rscp;
        serving_cell_power_info->ecio = l1_serving_cell_power_info.cell_info.ecio;
        serving_cell_power_info->rx_agc = l1_serving_cell_power_info.rx_agc;
        serving_cell_power_info->tx_agc = l1_serving_cell_power_info.tx_agc;
        serving_cell_power_info->dl_bler = l1_serving_cell_power_info.dl_bler;
        serving_cell_power_info->squal = l1_serving_cell_power_info.cell_info.squal;
        serving_cell_power_info->srxlev = l1_serving_cell_power_info.cell_info.srxlev;		
      }
      else
      {
        WRRC_MSG0_ERROR("FTD_DBG: Serving cell info invalid from L1.");
      }
    }
    else
    {
      serving_cell_power_info->psc = 0xFFFF; /*INVALID*/
      serving_cell_power_info->rscp = -121;
      serving_cell_power_info->ecio = -31;
      serving_cell_power_info->rx_agc = -106;
      serving_cell_power_info->tx_agc = -57;
      serving_cell_power_info->dl_bler = 0xFFFF;
      serving_cell_power_info->squal = 0; 
      serving_cell_power_info->srxlev = 0; 
    }
    call_cm_api = TRUE;
    rrcdata_logging_info.wcdma_UI_event_info.bitmask |=SYS_WCDMA_SERVING_CELL_POWER_INFO_BIT_MASK;
    WRRC_MSG1_MED("FTD_DBG:Serving cell power info bitmask set, bit_mask %x ",rrcdata_logging_info.wcdma_UI_event_info.bitmask);
  }

  if((RRC_IS_WCDMA_UI_EVENT_ENABLED(SYS_WCDMA_ACTIVE_SET_INFO_BIT_MASK)) &&
      (rrc_current_state == RRC_STATE_CELL_DCH) &&
      ((oc_status != OC_SET_FOR_DCH_FACH_TRANS ) && (oc_status != OC_SET_FOR_DCH_CELL_PCH_TRANS)
        && (oc_status != OC_SET_FOR_DCH_URA_PCH_TRANS)) &&
      (toc_usage != TOC_FOR_OOS))
  {
    uint8 aset_idx =0;
    uint8 rl_idx = 0;
    ordered_config_type *cfg_ptr_in_use = rrcllc_get_config_ptr_in_use(wrrc_as_id);
    wcdma_active_set_info_type *aset_info = &(rrcdata_logging_info.wcdma_UI_event_info.wcdma_event_info.wcdma_active_set_info);
#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
          /* Copy the Signalling mesaage payload to the log record */
    WCDMA_MEMCPY((void *)&(l1_active_set_info),
                 sizeof(l1_active_set_info_type),
                 (void *)(cmd_ptr->cmd.modem_stat_rsp.l1_active_set_info),
                 sizeof(l1_active_set_info_type));
#else
    l1_get_active_set_info(&l1_active_set_info,wrrc_as_id);
#endif
    aset_info->num_rls = l1_active_set_info.num_rls;
    for(aset_idx = 0; (aset_idx < l1_active_set_info.num_rls)&& (aset_idx < L1_ASET_MAX ); aset_idx++)
    {
      aset_info->psc[aset_idx] = l1_active_set_info.psc[aset_idx];
      aset_info->rscp[aset_idx] = l1_active_set_info.rscp[aset_idx];
      aset_info->ecio[aset_idx] = l1_active_set_info.ecio[aset_idx];
      aset_info->cell_id[aset_idx] = WCDMA_INVALID_CELL_ID;
      for(rl_idx = 0 ; rl_idx < L1_MAX_RL; rl_idx++)
      {
        if((cfg_ptr_in_use->l1_dl_chan_parms.dl_per_rl[rl_idx].pri_cpich_scr_code == l1_active_set_info.psc[aset_idx])
            &&(rrc_cell_params_int_data.cell_id_per_rl[rl_idx] != WCDMA_INVALID_CELL_ID))
        {
          aset_info->cell_id[aset_idx] = rrc_cell_params_int_data.cell_id_per_rl[rl_idx];
          break;
        }
      }
    }

    aset_info->rx_agc = l1_active_set_info.rx_agc;
    aset_info->tx_agc = l1_active_set_info.tx_agc;
    aset_info->dl_bler = l1_active_set_info.dl_bler;
    aset_info->sf = l1_active_set_info.sf;
    aset_info->phy_chan_type = l1_active_set_info.phy_chan_type;
    aset_info->slot_format = l1_active_set_info.slot_format;
    aset_info->is_compressed_mode_on = l1_cm_query_compressed_mode_active(wrrc_as_id);
    
    call_cm_api = TRUE;
    rrcdata_logging_info.wcdma_UI_event_info.bitmask |=SYS_WCDMA_ACTIVE_SET_INFO_BIT_MASK;
    WRRC_MSG5_HIGH("FTD_DBG: Active set info sf:%d phy_chan_type:%d slot_format:%d is_compressed_mode_on:%d bit_mask %x",
           aset_info->sf, aset_info->phy_chan_type, aset_info->slot_format, aset_info->is_compressed_mode_on, rrcdata_logging_info.wcdma_UI_event_info.bitmask);
    
  }

  if(RRC_IS_WCDMA_UI_EVENT_ENABLED(SYS_WCDMA_MONITORED_SET_BIT_MASK))
  {
    l1_monitored_set_info_type *l1_monitored_set_info_ptr = (l1_monitored_set_info_type *)rrc_malloc(sizeof(l1_monitored_set_info_type));    
    wcdma_monitored_set_info_type *monitored_set_info = &(rrcdata_logging_info.wcdma_UI_event_info.wcdma_event_info.wcdma_monitored_set_info);
    uint8 freq_idx, cell_idx;
#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
    /* Copy the Signalling mesaage payload to the log record */
    WCDMA_MEMCPY((void *)l1_monitored_set_info_ptr,
                 sizeof(l1_monitored_set_info_type),
                 (void *)(cmd_ptr->cmd.modem_stat_rsp.l1_monitored_set_info),
                 sizeof(l1_monitored_set_info_type));
#else
    l1_get_monitored_set_info(l1_monitored_set_info_ptr,wrrc_as_id);
#endif
    monitored_set_info->num_of_freqs = l1_monitored_set_info_ptr->num_of_freqs;
    if(monitored_set_info->num_of_freqs >L1_MAX_FREQ)
    {
      WRRC_MSG0_ERROR("FTD_DBG:Num of freq more than 3");
      monitored_set_info->num_of_freqs = L1_MAX_FREQ;
    }
    for(freq_idx = 0; freq_idx < l1_monitored_set_info_ptr->num_of_freqs && freq_idx < L1_MAX_FREQ ; freq_idx++)
    {
      monitored_set_info->freq[freq_idx].uarfcn = l1_monitored_set_info_ptr->freq[freq_idx].uarfcn;
      monitored_set_info->freq[freq_idx].num_of_cells = l1_monitored_set_info_ptr->freq[freq_idx].num_of_cells;
      if(monitored_set_info->freq[freq_idx].num_of_cells > l1_MAX_MONITORED_CELLS_PER_FREQ)
      {
        WRRC_MSG0_ERROR("FTD_DBG:Cells more than l1_MAX_MONITORED_CELLS_PER_FREQ");
        monitored_set_info->freq[freq_idx].num_of_cells  = l1_MAX_MONITORED_CELLS_PER_FREQ;
      }
      for(cell_idx =0; cell_idx < l1_monitored_set_info_ptr->freq[freq_idx].num_of_cells && cell_idx < l1_MAX_MONITORED_CELLS_PER_FREQ ; cell_idx++)
      {
        monitored_set_info->freq[freq_idx].cell[cell_idx].psc = l1_monitored_set_info_ptr->freq[freq_idx].cell_info[cell_idx].psc;
        monitored_set_info->freq[freq_idx].cell[cell_idx].rscp = l1_monitored_set_info_ptr->freq[freq_idx].cell_info[cell_idx].rscp;
        monitored_set_info->freq[freq_idx].cell[cell_idx].ecio = l1_monitored_set_info_ptr->freq[freq_idx].cell_info[cell_idx].ecio;
        monitored_set_info->freq[freq_idx].cell[cell_idx].squal = l1_monitored_set_info_ptr->freq[freq_idx].cell_info[cell_idx].squal;
        monitored_set_info->freq[freq_idx].cell[cell_idx].srxlev = l1_monitored_set_info_ptr->freq[freq_idx].cell_info[cell_idx].srxlev;
        monitored_set_info->freq[freq_idx].cell[cell_idx].rank = l1_monitored_set_info_ptr->freq[freq_idx].cell_info[cell_idx].rank;
        monitored_set_info->freq[freq_idx].cell[cell_idx].set = l1_monitored_set_info_ptr->freq[freq_idx].cell_info[cell_idx].set;
      }
    }
    monitored_set_info->num_gsm_cells = 0;
    if(rrcmcm_is_dualmode_enabled_no_f3(wrrc_as_id))
    {
      monitored_set_info->num_gsm_cells = l1_monitored_set_info_ptr->gsm_cells_info.number_of_cells;
      if(l1_monitored_set_info_ptr->gsm_cells_info.number_of_cells > L1_MAX_MONITORED_GSM_CELLS)
      {
        WRRC_MSG0_HIGH("FTD_DBG:GSM cells more than L1_MAX_MONITORED_GSM_CELLS");
        monitored_set_info->num_gsm_cells = L1_MAX_MONITORED_GSM_CELLS;
      }
      for(cell_idx = 0; cell_idx<l1_monitored_set_info_ptr->gsm_cells_info.number_of_cells && cell_idx < L1_MAX_MONITORED_GSM_CELLS; cell_idx++)
      {
        monitored_set_info->gsm_cell[cell_idx].arfcn = CONVERT_ARFCN_TO_NUM(l1_monitored_set_info_ptr->gsm_cells_info.interrat_measurements[cell_idx].bcch_arfcn);
        monitored_set_info->gsm_cell[cell_idx].bsic_bcc = l1_monitored_set_info_ptr->gsm_cells_info.interrat_measurements[cell_idx].bsic_bcc;
        monitored_set_info->gsm_cell[cell_idx].bsic_ncc = l1_monitored_set_info_ptr->gsm_cells_info.interrat_measurements[cell_idx].bsic_ncc;
        monitored_set_info->gsm_cell[cell_idx].rssi = l1_monitored_set_info_ptr->gsm_cells_info.interrat_measurements[cell_idx].rssi_dBm_x16;
        monitored_set_info->gsm_cell[cell_idx].srxlev = l1_monitored_set_info_ptr->gsm_cells_info.interrat_measurements[cell_idx].srxlev;
        monitored_set_info->gsm_cell[cell_idx].rank = l1_monitored_set_info_ptr->gsm_cells_info.interrat_measurements[cell_idx].rank;
      }
    }
#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
    monitored_set_info->num_lte_cells = 0;
    if(rrcmcm_is_lte_mode_enabled_wo_f3(wrrc_as_id))
    {
      monitored_set_info->num_lte_cells = l1_monitored_set_info_ptr->lte_cells_info.number_of_cells;
      if(l1_monitored_set_info_ptr->lte_cells_info.number_of_cells > WCDMA_MAX_MONITORED_LTE_CELLS)
      {
        WRRC_MSG0_HIGH("FTD_DBG:LTE cells more than L1_MAX_MONITORED_LTE_CELLS");
        monitored_set_info->num_lte_cells = WCDMA_MAX_MONITORED_LTE_CELLS;
      }
      for(cell_idx = 0; cell_idx<l1_monitored_set_info_ptr->lte_cells_info.number_of_cells && cell_idx < WCDMA_MAX_MONITORED_LTE_CELLS; cell_idx++)
      {
        monitored_set_info->lte_cell[cell_idx].earfcn = l1_monitored_set_info_ptr->lte_cells_info.interrat_measurements[cell_idx].earfcn;
        monitored_set_info->lte_cell[cell_idx].is_tdd_freq = l1_monitored_set_info_ptr->lte_cells_info.interrat_measurements[cell_idx].is_tdd_freq;
        monitored_set_info->lte_cell[cell_idx].cell_id = l1_monitored_set_info_ptr->lte_cells_info.interrat_measurements[cell_idx].cell_id;
        monitored_set_info->lte_cell[cell_idx].rsrp = l1_monitored_set_info_ptr->lte_cells_info.interrat_measurements[cell_idx].rsrp;
        monitored_set_info->lte_cell[cell_idx].rsrq = l1_monitored_set_info_ptr->lte_cells_info.interrat_measurements[cell_idx].rsrq;
        monitored_set_info->lte_cell[cell_idx].s_rxlev = l1_monitored_set_info_ptr->lte_cells_info.interrat_measurements[cell_idx].s_rxlev;
        WRRC_MSG6_HIGH(
         "FTD_DBG: EARFCN %d is_tdd_freq %d, cell_id %d rsrp=%d rsrq=%d s_rxlev=%d",
           monitored_set_info->lte_cell[cell_idx].earfcn, monitored_set_info->lte_cell[cell_idx].is_tdd_freq,
           monitored_set_info->lte_cell[cell_idx].cell_id, monitored_set_info->lte_cell[cell_idx].rsrp,
           monitored_set_info->lte_cell[cell_idx].rsrq, monitored_set_info->lte_cell[cell_idx].s_rxlev);
      }
    }
#endif
    call_cm_api = TRUE;
    rrcdata_logging_info.wcdma_UI_event_info.bitmask |=SYS_WCDMA_MONITORED_SET_BIT_MASK;
    WRRC_MSG1_MED("FTD_DBG:Monitored Set info bitmask set, bit_mask %x ",rrcdata_logging_info.wcdma_UI_event_info.bitmask);
    rrc_free(l1_monitored_set_info_ptr);
  }
  if(call_cm_api)
  {
    /*Call the CM API*/
#ifdef FEATURE_DUAL_SIM
    cm_per_subs_stats_set_modem_info_signal(SYS_MODEM_MODULE_WCDMA, rrc_get_as_id(wrrc_as_id));
#else
    cm_stats_set_modem_info_signal(SYS_MODEM_MODULE_WCDMA);
#endif
    WRRC_MSG1_HIGH("FTD_DBG: Reported Periodic Events, bitmask %x",rrcdata_logging_info.wcdma_UI_event_info.bitmask);
  }
}

#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
/*===========================================================================

FUNCTION  rrc_process_modem_stat_info

DESCRIPTION
  This function processes the modem_stat_info received from L1
  and updates WCDMA UI events

DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void rrc_process_modem_stat_info
(
  sys_modem_as_id_e_type wrrc_as_id, 
  /* Pointer to command */
  rrc_cmd_type *cmd_ptr 
)
{
  /* WRRC_MSG0_HIGH_OPT("function rrc_process_modem_stat_info is called");*/

  if(RRC_CPHY_MODEM_STATISTICS_RSP != cmd_ptr->cmd_hdr.cmd_id)
  {
    WRRC_MSG1_HIGH("Invalid cmd %d",cmd_ptr->cmd_hdr.cmd_id);
    return;
  }

  rrc_update_periodic_wcdma_UI_events_info(wrrc_as_id, cmd_ptr);

  //start the timer again
  rrctmr_start_timer(wrrc_as_id, RRCTMR_RSSI_BER_TIMER,
                      RRCTMR_RSSI_BER_REPORT_TIME );
}
/*===========================================================================

FUNCTION   rrc_post_cmd_to_l1_to_get_modem_stat_info

DESCRIPTION
  This function posts command to L1 to get serving cell power info
  active set info and monitored set info

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void rrc_post_cmd_to_l1_to_get_modem_stat_info(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Used when sending CPHY_IDLE_REQ */
  l1_ext_cmd_type *l1_cmd_ptr = l1_get_cmd_buf();
  
  /* Fill in the Command Header */
  l1_cmd_ptr->cmd_hdr.cmd_id = CPHY_GET_MODEM_STATISTICS_REQ;

  /* L1 allocates memory for embd pointers based on bmsk. We currently 
   * set all in bmsk and hence free all the three embedded pointers.
   * In future if bmask is to be changed freeing should be taken care
   */
  l1_cmd_ptr->cmd.get_modem_stat_req.modem_stat_req_bmsk = 
     WCDMA_GET_SERVING_CELL_POWER_INFO | WCDMA_GET_ACTIVE_SET_INFO |
     WCDMA_GET_MONITORED_SET_INFO;

  rrc_send_l1_cmd(wrrc_as_id, l1_cmd_ptr);
}
#endif /* FEATURE_WCDMA_FREE_FLOATING_TASK */

/*===========================================================================

FUNCTION  RRC_RSSI_BER_TIMEOUT_HANDLER

DESCRIPTION

  This function is invoked when the RSSI&BER repoting timer expires. It calls
  to L1 to get RSSI and BER info. If the value of RSSI or BER is changed,
  the function will send a RRC-CM interface primitive to inform the changes.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void rrc_rssi_ber_timeout_handler(sys_modem_as_id_e_type wrrc_as_id)
{

  /* Note: currently, the function only reports RSSI. It will require changes
     if BER is available to report */

  // byte new_ber;

#ifdef FEATURE_EUTRA_DETECT
  sys_eutra_status_enum_type eutra_status = SYS_EUTRA_DETECTION_UNSUPPORTED;
#endif

#ifdef FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
  #error code not present
#else

  l1_rssi_info_struct_type l1_rssi;

#endif
  
  rrc_state_e_type rrc_current_state;
  cm_rpt_type *cmd_ptr;
  rrc_csp_scan_e_type curr_csp_scan = rrccsp_get_curr_scan_no_f3(wrrc_as_id);
  rrc_proc_e_type proc;
  rrcllc_oc_process_state_e_type process_status;

#ifdef FEATURE_DUAL_SIM
  /* Do not report RSSI when RRC sends NO SERVICE to NAS because of no resource */
  if(rrc_get_oos_due_to_no_resource_flag_status(wrrc_as_id))
  {
    // start the timer again
    rrctmr_start_timer(wrrc_as_id, RRCTMR_RSSI_BER_TIMER,
                       RRCTMR_RSSI_BER_REPORT_TIME );
    return;
  }
#endif

#ifdef FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
  #error code not present
#else
  /* Need L1 constants here from srchparm.h */
  /* Coded hard values to make lint happy */ 
  l1_rssi.pathloss = 0xFF;    /*Value of pathloss not valid */
  l1_rssi.sir = 0xFF;         /*Value of Sir not valid */
#endif

  rrc_current_state = rrc_get_state(wrrc_as_id);
  /*Read RSSI value from L1 always when RRC state is DISCONNECTED */
  if (rrc_current_state == RRC_STATE_DISCONNECTED 
      || rrc_current_state == RRC_STATE_CONNECTING)
  {

#ifdef FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
    #error code not present
#else

    /* Get RSSI value from L1 */
    l1_get_rssi(&l1_rssi,wrrc_as_id); /* get the RSSI from L1 */
    if(l1_rssi.is_valid_rssi == FALSE && ((curr_csp_scan == RRC_CSP_LEAVING_CONN_MODE_SCAN) ||
       (rrcmcm_get_rrc_mode(wrrc_as_id) == RRC_MODE_STANDBY) || (rrc_current_state == RRC_STATE_CONNECTING)))
    {
      /* retain old RSSI value */
      WRRC_MSG4_MED( 
            "RSSI: is_valid_rssi=%d, rssi_val=%d, curr_csp_scan=%d, rrc_mode=%d ", 
            l1_rssi.is_valid_rssi,rrcdata_logging_info.rrc_rssi, curr_csp_scan,rrcmcm_get_rrc_mode(wrrc_as_id));
    }
    else
    {
      rrcdata_logging_info.rrc_rssi = l1_rssi.rssi_value; 
      #ifdef FEATURE_UMTS_REPORT_ECIO
      /* Get ecio value from L1*/
      rrcdata_logging_info.rrc_ecio = l1_rssi.ecio;
      #endif
      WRRC_MSG3_MED("RSSI: Called L1 func new rssi val %d rrc St: %d , is_valid_ RSSI %d", 
               rrcdata_logging_info.rrc_rssi, rrc_current_state, l1_rssi.is_valid_rssi);
    }
#endif

  }
  else   
  {

#ifdef FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
#error code not present
#else

    l1_get_rssi(&l1_rssi,wrrc_as_id);

    if(l1_rssi.is_valid_rssi == TRUE)
    {
      rrcdata_logging_info.rrc_rssi = l1_rssi.rssi_value;
#ifdef FEATURE_UMTS_REPORT_ECIO
      /* Get ecio value from L1*/
      rrcdata_logging_info.rrc_ecio = l1_rssi.ecio;
#endif
    }
    else
    {
      if((curr_csp_scan == RRC_CSP_OUT_OF_SERVICE_AREA_SCAN)||
          ((OC_SET_FOR_DCH_FACH_TRANS == 
             rrcllc_get_ordered_config_state_and_proc(wrrc_as_id, &proc,
                                                &process_status
                                              ) ) && (proc == RRC_PROCEDURE_CU))
      ) /* Apply RSSI value to zero */
      {
        rrcdata_logging_info.rrc_rssi = MIN_RSCP_RL_FAILURE_OOS;
      }
      else if(!((curr_csp_scan == RRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN) ||
                (curr_csp_scan == RRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN) ||
                (curr_csp_scan == RRC_CSP_CELL_DCH_TO_URA_PCH_SCAN) ||
                (curr_csp_scan == RRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN) ||
                (curr_csp_scan == RRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN) ||
                (curr_csp_scan == RRC_CSP_CELL_FACH_TO_URA_PCH_SCAN) ||
                (rrcmcm_get_rrc_mode(wrrc_as_id) == RRC_MODE_STANDBY) ||
                ((rrcllc_get_ordered_config_status(wrrc_as_id) != OC_NOT_SET) &&  ordered_config_ptr->is_hho)))
      {
        rrcdata_logging_info.rrc_rssi = l1_rssi.rssi_value;
      }
         WRRC_MSG4_MED(
         "RSSI: is_valid_rssi=%d, rssi_val=%d, curr_csp_scan=%d, is_hard_handover=%d",
                                                                    l1_rssi.is_valid_rssi,
                                                                    rrcdata_logging_info.rrc_rssi,
                                                                    curr_csp_scan,ordered_config_ptr->is_hho);
      }

 #endif /* FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY */
  } 
  /* Get BER from L1 */
  // new_ber = l1_get_ber();

#ifndef FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
  if((!l1_rssi.is_valid_rssi) || (rrc_current_state != RRC_STATE_CELL_DCH))
  {
    l1_rssi.enchs_avg_cqi_val = 0xFF;
    l1_rssi.enchs_avg_cqi_num_sample = 0xFFFF;
  }
#endif

#ifdef FEATURE_EUTRA_DETECT
  if(rrc_check_eutradetect_ie(wrrc_as_id))
  {
    switch(rrc_l1_get_eutra_detect_status(wrrc_as_id))
    {
      case EUTRA_CELLS_DETECTED:
        eutra_status = SYS_EUTRA_CELL_PRESENT;
        break;
      case EUTRA_CELLS_NOT_DETECTED:
        eutra_status = SYS_EUTRA_CELL_NOT_PRESENT;
        break;
      case EUTRA_CELL_DETECT_NOT_STARTED:
        eutra_status = SYS_EUTRA_DETECTION_UNKNOWN;
        break;
      default:
        WRRC_MSG0_ERROR("EUTRA_DETECT: Invalid status returned by l1");
        break;
    }
  }
  else
  {
    eutra_status = SYS_EUTRA_DETECTION_UNSUPPORTED;
  }
#endif

#ifdef FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
 #error code not present
#else
WRRC_MSG7_HIGH("RSSI: is_valid_rssi=%d, rssi_val=%d, curr_csp_scan=%d, rrc_mode=%d,rrc St: %d HS_CQI: Avg CQI %d , num samples %d", 
                l1_rssi.is_valid_rssi,
                rrcdata_logging_info.rrc_rssi,
                curr_csp_scan,
                rrcmcm_get_rrc_mode(wrrc_as_id),
                rrc_current_state,
                l1_rssi.enchs_avg_cqi_val,
                l1_rssi.enchs_avg_cqi_num_sample
                );
#endif
  /* get a CM command buffer */
  if ((cmd_ptr = cm_get_cmd_buf()) != NULL)
  {
    /* format the primitive and send to the destination task */
#ifdef FEATURE_DUAL_SIM
    cmd_ptr->asubs_id = rrc_get_as_id(wrrc_as_id);
#endif

    cmd_ptr->hdr.cmd = CM_RRC_STATUS_IND;

    cmd_ptr->cmd.signal_status.bit_error_rate = UNAVAILABLE_BER;

#ifndef FEATURE_WCDMA_DATA_FOR_UI_BAR_DISPLAY
#ifdef FEATURE_MMODE_CMI_4
    cmd_ptr->cmd.signal_status.signal_strength = rrcdata_logging_info.rrc_rssi;
#else
    cmd_ptr->cmd.signal_status.signal_strength = (byte)rrc_rssi;
#endif
    #ifdef FEATURE_UMTS_REPORT_ECIO
    cmd_ptr->cmd.signal_status.ecio = rrcdata_logging_info.rrc_ecio;
    WRRC_MSG1_HIGH("ECIO value being sent to CM =%d",rrcdata_logging_info.rrc_ecio);
    #endif
    
    cmd_ptr->cmd.signal_status.pathloss = l1_rssi.pathloss;
    cmd_ptr->cmd.signal_status.sir = l1_rssi.sir;

    #ifdef FEATURE_UMTS_UI_REPORT_BLER
    #error code not present
#endif
    cmd_ptr->cmd.signal_status.avg_cqi_val = l1_rssi.enchs_avg_cqi_val;
    cmd_ptr->cmd.signal_status.avg_cqi_num_sample = l1_rssi.enchs_avg_cqi_num_sample;
    WRRC_MSG2_MED("HS_CQI: Avg CQI %d , num samples %d ",cmd_ptr->cmd.signal_status.avg_cqi_val,
        cmd_ptr->cmd.signal_status.avg_cqi_num_sample);

#else
    #error code not present
#endif

    WRRC_MSG2_HIGH(" Pathloss: %d, SIR: %d are reported to CM",
                                                         cmd_ptr->cmd.signal_status.pathloss,
                                                         cmd_ptr->cmd.signal_status.sir);

#ifdef FEATURE_EUTRA_DETECT
    WRRC_MSG1_HIGH("EUTRA_DETECT: Filling eutra status:%d",eutra_status);
    cmd_ptr->cmd.signal_status.eutra_detect_status = eutra_status;
#endif
    cm_put_cmd(cmd_ptr);

    MSG_MED("Tx CM_RRC_STATUS_IND to CM RSSI: %d ",rrcdata_logging_info.rrc_rssi,0,0);

  }

#ifdef FEATURE_WCDMA_FREE_FLOATING_TASK
  rrc_post_cmd_to_l1_to_get_modem_stat_info(wrrc_as_id);
  return;
#else
  rrc_update_periodic_wcdma_UI_events_info(wrrc_as_id);
#endif /* FEATURE_WCDMA_FREE_FLOATING_TASK */

  // start the timer again
  rrctmr_start_timer(wrrc_as_id, RRCTMR_RSSI_BER_TIMER,
                      RRCTMR_RSSI_BER_REPORT_TIME );

  return;

} /* End of rrc_rssi_ber_timeout_handler */


/*===========================================================================
FUNCTION  rrc_bcch_and_nas_plmns_match

DESCRIPTION
  This function does a matching of BCCH and NAS supplied PLMNs.

DEPENDENCIES
  Dependency on NAS supplied routines.

RETURN VALUE
  TRUE:  If NAS and BCCH PLMNs match.
  FALSE: If NAS and BCCH PLMNs do not match.

SIDE EFFECTS
  The two passed PLMNs have to be in the correct order. The first parameter
  is expected as the PLMN received on BCCH and the second one is NAS supplied
  PLMN.
  This utility is specifically for comparison between NAS and BCCH PLMNs
  and should NEVER be used for PLMN comparison between 2 NAS supplied PLMNs
  or 2 BCCH PLMNs. 
===========================================================================*/
boolean   rrc_bcch_and_nas_plmns_match
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_plmn_identity_type  bcch_plmn,
  rrc_plmn_identity_type  nas_plmn
)
{
  return sys_plmn_match(mmumts_convert_rrc_plmn_id_to_nas_plmn_id(nas_plmn),
                        mmumts_convert_rrc_plmn_id_to_nas_plmn_id(bcch_plmn));
}  /* rrc_bcch_and_nas_plmns_match */


/*===========================================================================
FUNCTION  rrc_bcch_plmns_match

DESCRIPTION
  This function does a matching of two PLMNs obtained from BCCH.
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE:  If BCCH PLMNs match.
  FALSE: If BCCH PLMNs do not match.

SIDE EFFECTS
  The two passed PLMNs have to be both from BCCH. For comparison between a
  NAS supplied PLMN and a BCCH PLMN, DO NOT call this function.
===========================================================================*/
boolean rrc_bcch_plmns_match(sys_modem_as_id_e_type wrrc_as_id,rrc_plmn_identity_type plmn1, rrc_plmn_identity_type plmn2)
{
  byte i;

  for(i = 0; i < RRC_MAX_MCC_DIGITS; i++)
  {
    if(plmn1.mcc[i] != plmn2.mcc[i])
    {
      return(FALSE);
    }
  }

  if(plmn1.num_mnc_digits == plmn2.num_mnc_digits)
  {
    for(i = 0; i < plmn1.num_mnc_digits; i++)
    {
      if(plmn1.mnc[i] != plmn2.mnc[i])
      {
        return(FALSE);
      }
    }
  }
  else
  {
    return(FALSE);
  }

  return(TRUE);
}
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC

/*===========================================================================
FUNCTION wrrc_qsh_log_timers_and_const_params

DESCRIPTION
  This function logs timer parameters

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_timers_and_const_params(sys_modem_as_id_e_type wrrc_as_id, wrrc_qsh_int_action_e_type action)
{
  rrc_T_300 t_300 = 0;
  rrc_N_300 n_300 = 0;
  wrrc_qsh_metric_tmr_and_const_s tmr_and_const_params;
  
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr != NULL)
  {
    switch(action)
    { 
      case WRRC_METRIC_LOG:		 
        if(rrc_sibdb_return_sib_for_srv_cell(wrrc_as_id, rrc_SIB1) != NULL)
        {
          (void)rrcrce_get_t300_from_serv_cell_db(wrrc_as_id, &t_300);  
          rrcrce_get_n300_from_serv_cell_db(wrrc_as_id, &n_300);
        }
        
        /* memset to 0 to make sure padding between fields are inited */
        memset(&tmr_and_const_params, 0 , sizeof(wrrc_qsh_metric_tmr_and_const_s));
        /* Copying QSH header to make sure memcmp gives comparison of only timer and const params*/
        WCDMA_MEMCPY(&tmr_and_const_params, sizeof(qsh_metric_hdr_s), rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr, sizeof(qsh_metric_hdr_s));
        
        tmr_and_const_params.t313 = (uint8)RRC_GET_T313();
        tmr_and_const_params.t300 = (uint8)t_300;
        tmr_and_const_params.t302 = (uint8)RRC_GET_T302(); 		 
        tmr_and_const_params.n313 = (uint8)RRC_GET_N313();
        tmr_and_const_params.n315 = (uint8)RRC_GET_N315();
        tmr_and_const_params.n300 = (uint8)n_300;
        tmr_and_const_params.n302 = (uint8)RRC_GET_N302(); 	 
        tmr_and_const_params.t314 = (uint8)RRC_GET_T314(); 		 
        tmr_and_const_params.psc = (uint16)rrc_ccm_get_curr_camped_psc(wrrc_as_id);
        
        if(memcmp(&tmr_and_const_params, rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr, sizeof(wrrc_qsh_metric_tmr_and_const_s)))
        {
          WCDMA_MEMCPY(rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr, sizeof(wrrc_qsh_metric_tmr_and_const_s), 
          &tmr_and_const_params, sizeof(wrrc_qsh_metric_tmr_and_const_s));
          
          rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS,QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

          /* copy the same contents to data ptr for future comparison */
          WCDMA_MEMCPY(rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr, sizeof(wrrc_qsh_metric_tmr_and_const_s), 
          &tmr_and_const_params, sizeof(wrrc_qsh_metric_tmr_and_const_s));
        }
        break;
      case WRRC_METRIC_START:
        /* Intialize the data */
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->t313 = 0;
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->t300 = 0;
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->t302 = 0; 
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->n313 = 0;
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->n315 = 0;
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->n300 = 0;
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->n302 = 0; 
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->t314 = 0;			 
        ((wrrc_qsh_metric_tmr_and_const_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS].data_ptr)->psc = 0; 
        break;
      default:
        break;
    }
  }
}
#endif
/*===========================================================================
FUNCTION  rrc_update_ue_conn_timers_and_constants

DESCRIPTION
  This function updates rrc_ue_conn_timers_and_constants based on updates
  received in sib1 or Utran Mobility Info messages
  
DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  Updates the global variable rrc_ue_conn_timers_and_constants
===========================================================================*/
uecomdef_status_e_type rrc_update_ue_conn_timers_and_constants
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_UE_ConnTimersAndConstants* ue_conn_timers_and_constants_ptr,
  rrc_UE_ConnTimersAndConstants_v3a0ext * ext_ptr,
  rrc_UE_ConnTimersAndConstants_v860ext *ue_conn_timers_and_constants_v860ext_ptr
)
{
  if(ue_conn_timers_and_constants_ptr != NULL)
  {
    /* copy the updated ue_conn_timers_and_constants to global variable
     * rrc_ue_conn_timers_and_constants.  incase bit mask is not set for
     * a particular timer or constant then set it to default as all params
     * are mandatory default
     */
  
    WRRC_MSG0_HIGH("Updating Connected Mode Timers/Const");
  
    
   
  
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_301=  
      ue_conn_timers_and_constants_ptr->t_301;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_301=  
      ue_conn_timers_and_constants_ptr->n_301;  
  
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_302=  
      ue_conn_timers_and_constants_ptr->t_302;  
    
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_302=  
      ue_conn_timers_and_constants_ptr->n_302;  
    
  
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_304=  
      ue_conn_timers_and_constants_ptr->t_304;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_304=  
      ue_conn_timers_and_constants_ptr->n_304;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_305=  
      ue_conn_timers_and_constants_ptr->t_305;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_307=  
      ue_conn_timers_and_constants_ptr->t_307;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_308=  
      ue_conn_timers_and_constants_ptr->t_308;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_309=  
      ue_conn_timers_and_constants_ptr->t_309;

    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_310=  
        ue_conn_timers_and_constants_ptr->t_310;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_310=  
      ue_conn_timers_and_constants_ptr->n_310;  

    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_311=  
      ue_conn_timers_and_constants_ptr->t_311;  
 
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_312=  
      ue_conn_timers_and_constants_ptr->t_312;  
    
    rrcdata_timers_counters_info.conn_timers_and_constants.n312=  
      rrc_translate_n312(wrrc_as_id,ue_conn_timers_and_constants_ptr->n_312);  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_313=  
      ue_conn_timers_and_constants_ptr->t_313;  
  
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_313=  
      ue_conn_timers_and_constants_ptr->n_313;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_314=  
      ue_conn_timers_and_constants_ptr->t_314;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_315=  
      ue_conn_timers_and_constants_ptr->t_315;  
    
    rrcdata_timers_counters_info.conn_timers_and_constants.n315 = rrc_translate_n315(wrrc_as_id,ue_conn_timers_and_constants_ptr->n_315);
   
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_316=  
      ue_conn_timers_and_constants_ptr->t_316;  
    
    rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_317=  
      ue_conn_timers_and_constants_ptr->t_317;  
   
  }
  /* If UMI has IE "contants and timers", SIB1 data shouldn't be overwritten */
  else if(rrcumi_check_ue_timer_constants_update_status(wrrc_as_id,TRUE)
          == RRC_CONN_TIMER_AND_CONSTANTS_UPDATE_REQ)
  {
    /* Initialize UE connected mode timers and constants when rrc_UE_ConnTimersAndConstants
       is not present in SIB1 to make sure that all the timers are set to MD */
    rrc_initialize_ue_conn_timers_and_constants(wrrc_as_id);
  }

  if(ue_conn_timers_and_constants_v860ext_ptr != NULL)
  {
    /* No need to STOP original T323 timer.
        * If dormancy is disabled then might be Timer (T323) is running because of SRNS
        * Relocation or CELL_PCH/URA_PCH state guard timers.
        * Stop them as dormancy can be triggered just after timer updation */
    
    if(ue_conn_timers_and_constants_v860ext_ptr->m.t_323Present)
    {
      rrcdormancy_update_dormancy_params(wrrc_as_id,ue_conn_timers_and_constants_v860ext_ptr->t_323);
    }
    else
    {
      rrcdormancy_disable_fast_dormancy(wrrc_as_id);
      rrcdormancy_stop_or_restart_timer_T323(wrrc_as_id,RRCDORM_STOP_T323_TIMER_VALUE,
        RRCDORMANCY_T323_TIMER_INVALID);
    }
  }
  else
  {
    rrcdormancy_disable_fast_dormancy(wrrc_as_id);
    rrcdormancy_stop_or_restart_timer_T323(wrrc_as_id,RRCDORM_STOP_T323_TIMER_VALUE,
      RRCDORMANCY_T323_TIMER_INVALID);
  }
  
  if(ext_ptr != NULL)
  {

    /* copy the updated ue_conn_timers_and_constants to global variable
     * rrc_ue_conn_timers_and_constants.  incase bit mask is not set for
     * a particular timer or constant then set it to default as all params
     * are mandatory default
     */
  
    WRRC_MSG0_HIGH("Updating Connected Mode Ext Timers/Const");
  
    if(ext_ptr->m.n_312Present)
    {
      rrcdata_timers_counters_info.conn_timers_and_constants.n312 = rrc_translate_n312_v3a0ext(wrrc_as_id,ext_ptr->n_312);
    }
  
    if(ext_ptr->m.n_315Present)
    {
      rrcdata_timers_counters_info.conn_timers_and_constants.n315 = rrc_translate_n315_v3a0ext(wrrc_as_id,ext_ptr->n_315);
    }
  }
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  /* update change in timers to QSH */
  wrrc_qsh_log_timers_and_const_params(wrrc_as_id, WRRC_METRIC_LOG);
#endif
  return SUCCESS;

}/*rrc_update_ue_conn_timers_and_constants*/
/*===========================================================================
FUNCTION  rrc_initialize_ue_conn_timers_and_constants

DESCRIPTION
  This function Initializes rrc_ue_conn_timers_and_constants to default values
  as all parameters are mandatory default
  
DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  Initializes the global variable rrc_ue_conn_timers_and_constants with
  default values as all parameters are Mandatory Default
===========================================================================*/
void rrc_initialize_ue_conn_timers_and_constants
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  WRRC_MSG0_HIGH("Initializing Connected Mode Timers/Const");

  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_301= rrc_T_301_ms2000;  
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_301= 2; 
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_302= rrc_T_302_ms4000; 
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_302= 3;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_304= rrc_T_304_ms2000;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_304= 2;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_305= rrc_T_305_m30; 
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_307= rrc_T_307_s30;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_308= rrc_T_308_ms160;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_309= 5;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_310= rrc_T_310_ms160;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_310= 4;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_311= rrc_T_311_ms2000;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_312= 1;
  rrcdata_timers_counters_info.conn_timers_and_constants.n312= rrc_translate_n312(wrrc_as_id,rrc_N_312_s1);
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_313= 3;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.n_313= rrc_N_313_s20;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_314= rrc_T_314_s12;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_315= rrc_T_315_s180;
  rrcdata_timers_counters_info.conn_timers_and_constants.n315= rrc_translate_n315(wrrc_as_id,rrc_N_315_s1);
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_316= rrc_T_316_s30;
  rrcdata_timers_counters_info.rrc_ue_conn_timers_and_constants.t_317= rrc_T_317_infinity4;
}/*rrc_initialize_ue_conn_timers_and_constants*/

/*===========================================================================

FUNCTION RRC_CHECK_RBS_ASSOCIATED_WITH_RE_EST_TIMER

DESCRIPTION

  This function looks into the ESTABLISHED_RABS and checks if there are any
  RBs associated with the particular re-estblishment timer
  
DEPENDENCIES


RETURN VALUE
  Returns TRUE if there are any RBs associated with that particular 
  re_establishment timer. Otherwise it returns FALSE.
 

SIDE EFFECTS

  None

===========================================================================*/
boolean rrc_check_rbs_associated_with_re_est_timer
(    
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_Re_EstablishmentTimer re_est_timer
)
{
  uint32 count;
  boolean match_found = FALSE;    /* Local varible used to update the match
                                     for Re_establishment timer */
   
  /* First check if there are RABs assoicated with re_establishment timer */
  for (count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if( (rrc_est_rabs.rabs[count].re_est_timer == re_est_timer) &&
        (rrc_est_rabs.rabs[count].num_rbs_for_rab > 0)  )
    {
      WRRC_MSG3_HIGH("RAB found id:%d,rbs:%d,re-est tmr:%d",
               rrc_est_rabs.rabs[count].rab_id,
               rrc_est_rabs.rabs[count].num_rbs_for_rab,
               re_est_timer
               );
      match_found = TRUE;
      break;
    }
  }
  return(match_found);
} /* rrc_check_rbs_associated_with_re_est_timer */

/*===========================================================================

FUNCTION RRC_CHECK_RABS_ASSOCIATED_WITH_CN_DOMAIN

DESCRIPTION

  This function looks into the ESTABLISHED_RABS and checks if there are any
  RABs associated with the particular CN Domain
  
DEPENDENCIES


RETURN VALUE
  Returns TRUE if there are any RABs associated with that particular 
  CN Domain. Otherwise it returns FALSE.
 

SIDE EFFECTS

  None

===========================================================================*/
boolean rrc_check_rabs_associated_with_cn_domain
(    
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_cn_domain_identity_e_type rrc_cn_domain_id
)
{
  uint32 count;
  boolean match_found = FALSE;    /* Local varible used to update the match
                                     for CN Domian Id */
   
  /* First check if there are RABs assoicated with CN Domain Id */
  for (count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if((rrc_est_rabs.rabs[count].cn_domain == rrc_cn_domain_id) &&
        (rrc_est_rabs.rabs[count].num_rbs_for_rab > 0)  )
    {
      match_found = TRUE;
      break;
    }
  }
  return(match_found);
} /* rrc_check_rabs_associated_with_cn_domain */

/*===========================================================================

FUNCTION RRC_CONVERT_T305_TO_MS

DESCRIPTION
  This function converts the rrc_T_305 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
uint32 rrc_convert_t305_to_ms
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_T_305 rrccu_t_305                    /* enum for Timer T305 */
)
{
  uint32    timer_value;                         /* Timer Value in milli seconds */

  switch (rrccu_t_305)
  {
    case rrc_T_305_noUpdate:

      timer_value = RRCTMR_INFINITY_VAL;          /* This may approximately  equal to
                                            24 hrs */
      break;
    case rrc_T_305_m5:
      timer_value = 5 * 60 * 1000;
      break;      
    case rrc_T_305_m10:
      timer_value = 10 * 60 * 1000;
      break;
    case rrc_T_305_m30:
      timer_value = 30 * 60 * 1000;
      break;
    case rrc_T_305_m60:
      timer_value = 60 * 60 * 1000;
      break;
    case rrc_T_305_m120:
      timer_value = 120 * 60 * 1000;
      break; 
    case rrc_T_305_m360:
      timer_value = 360 * 60 * 1000;
      break;
    case rrc_T_305_m720:
      timer_value = 720 * 60 * 1000;
      break;
    default:
      WRRC_MSG0_HIGH("Unknown Timer value received ");
      timer_value = RRCTMR_INFINITY_VAL;         /* Set to no periodic Cell Update */
      break;
  }
  return (timer_value);
} /* rrc_convert_t305_to_ms*/

/*===========================================================================

FUNCTION RRC_TRANSLATE_STATE_INDICATOR_TO_STATE

DESCRIPTION
  This function converts the Received RRC state indicator into internal
  RRC state format
  
DEPENDENCIES
  None

RETURN VALUE
  rrc_state_e_type : returns the RRC state
  
SIDE EFFECTS

  None

===========================================================================*/
rrc_state_e_type rrc_translate_state_indicator_to_state
(    
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RRC_StateIndicator state_indicator
)
{
  rrc_state_e_type state = RRC_STATE_MAX ;       /* Local variable to store the state */
  switch(state_indicator)
  {
    case rrc_RRC_StateIndicator_cell_DCH:
      state = RRC_STATE_CELL_DCH;
      break;
    case rrc_RRC_StateIndicator_cell_FACH:
      state = RRC_STATE_CELL_FACH;
      break;
    case rrc_RRC_StateIndicator_cell_PCH:
      state = RRC_STATE_CELL_PCH;
      break;
    case rrc_RRC_StateIndicator_ura_PCH:
      state = RRC_STATE_URA_PCH;
      break;
    default:
      WRRC_MSG1_HIGH("Inivalid State indicator: %d ",state_indicator);
      break;
  }
  return(state);
} /* rrc_translate_state_indicator_to_state*/

/*===========================================================================

FUNCTION RRC_RETURN_CS_DATA_CALL_PARMS_SUB

DESCRIPTION

  This function fills in the SDU size and number of SDUs per TTI for the
  associated RAB id.
  
DEPENDENCIES


RETURN VALUE
  Returns FAILURE is the RAB id is not found.  Otherwise returns SUCCESS.
 

SIDE EFFECTS

  None

===========================================================================*/
rrc_status_e_type rrc_return_cs_data_call_parms_sub
(
  sys_modem_as_id_e_type wrrc_as_id,
  uint32                  rab_id,
  rrc_cs_data_parms_type *cs_data_parms_ptr
)
{
  int i, j, k;
  rrc_RB_Identity rb_id;
  rlc_lc_id_type lc_id;
  tr_ch_id_type trch_id = RRCLLC_TRCH_ID_MAX_VALUE + 1;
  uint8 trch_idx = UE_MAX_TRCH;
  ordered_config_type *cfg_ptr;
  uint16 max_ntblks = 0;

  if(cs_data_parms_ptr == NULL)
  {
    WRRC_MSG1_ERROR("NULL cs_data_parms_ptr passed to rrc_return_cs_data_call_parms()",
        0);
    return RRC_STATUS_FAILURE;
  }

  if(rrcllc_get_ordered_config_status_wo_f3(wrrc_as_id) != OC_NOT_SET)
  {
    cfg_ptr = ordered_config_ptr;
  }
  else
  {
    cfg_ptr = current_config_ptr;
  }

  for(i = 0; i < MAX_RAB_TO_SETUP; i++)
  {
    if(rrc_est_rabs.rabs[i].rab_id == rab_id )
    {
      if(rrc_est_rabs.rabs[i].num_rbs_for_rab != 1)
      {
        WRRC_MSG1_ERROR("RAB id %d has more than one RB mapped for CS data call. Choosing first RB.",
            rrc_est_rabs.rabs[i].rab_id);
      }
      rb_id = rrc_est_rabs.rabs[i].rb_for_rab[0].rb_id;
      lc_id = rrclcm_get_ul_rlc_lc_id(wrrc_as_id,rb_id);
      for(j = 0; j < UE_MAX_TRCH; j++)
      {
        for(k = 0; k < MAX_DED_LOGCHAN_PER_TRCH; k++)
        {
          if(cfg_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[j].dlchan_info[k].rlc_id ==
             lc_id)
          {
            trch_id = cfg_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[j].trch_id;
            break;
          }
        }

        if(trch_id < RRCLLC_TRCH_ID_MAX_VALUE + 1)
        {
          break;
        }
      }

      if(trch_id == RRCLLC_TRCH_ID_MAX_VALUE + 1)
      {
        WRRC_MSG1_ERROR("trch_id not found for lc_id %d", lc_id);
        return RRC_STATUS_FAILURE;
      }

      for(i = 0; i < UE_MAX_TRCH; i++)
      {
        if(cfg_ptr->ul_dch_trch_info[i].trch_id == trch_id)
        {
          trch_idx = (uint8)i;
          break;
        }
      }

      if(trch_idx == UE_MAX_TRCH)
      {
        WRRC_MSG1_ERROR("Index not found for trch_id %d", trch_id);
        return RRC_STATUS_FAILURE;
      }

      cs_data_parms_ptr->sdu_size =
        cfg_ptr->mac_ul_parms.tfcs_ptr->tfs_info[trch_idx].tf_info[0].tb_size;
      for(i = 0; i < cfg_ptr->mac_ul_parms.tfcs_ptr->tfs_info[trch_idx].ntf; i++)
      {
        /* Use the largest ntblks field. */
        if (cfg_ptr->mac_ul_parms.tfcs_ptr->tfs_info[trch_idx].tf_info[i].ntblks
            > max_ntblks)
        {
          max_ntblks =
            cfg_ptr->mac_ul_parms.tfcs_ptr->tfs_info[trch_idx].tf_info[i].ntblks;
        }
      }
      cs_data_parms_ptr->num_sdu_per_tti = max_ntblks;
      return RRC_STATUS_SUCCESS;
    }
  }

  return RRC_STATUS_FAILURE;
} /* rrc_return_cs_data_call_parms_sub */

/*===========================================================================

FUNCTION RRC_RETURN_CS_DATA_CALL_PARMS

DESCRIPTION

  This function fills in the SDU size and number of SDUs per TTI for the
  associated RAB id.
  
DEPENDENCIES


RETURN VALUE
  Returns FAILURE is the RAB id is not found.  Otherwise returns SUCCESS.
 

SIDE EFFECTS

  None

===========================================================================*/
rrc_status_e_type rrc_return_cs_data_call_parms
(
  uint32                  rab_id,
  rrc_cs_data_parms_type *cs_data_parms_ptr
)
{
  return rrc_return_cs_data_call_parms_sub(SYS_MODEM_AS_ID_1, rab_id, cs_data_parms_ptr);
} /* rrc_return_cs_data_call_parms */

/*===========================================================================

FUNCTION rrc_is_dl_ps_zero_tb_size_configured

DESCRIPTION

  This function fills in the transport block size per PS RAB

  
DEPENDENCIES


RETURN VALUE
  Returns TRUE if PS RAB id doesn't find the size.  Otherwise returns FAIL.
 

SIDE EFFECTS

  None

===========================================================================*/
rrc_ps_data_rate_e_type rrc_is_dl_ps_zero_tb_size_configured_for_all_rabs
(
sys_modem_as_id_e_type wrrc_as_id
)
{
  int i;
  rrc_ps_data_rate_e_type ps_zero_rb_found = RRC_PS_DATA_RATE_RAB_NOT_CONFIGURED;
  rrc_RB_Identity rb_id;

  for(i = 0; i < MAX_RAB_TO_SETUP; i++)
  {
    if (RRCRB_PS_DATA_RAB  == rrc_est_rabs.rabs[i].rab_type)
    {
      ps_zero_rb_found = RRC_PS_DATA_RATE_ZERO_CONFIGURED;
      rb_id = rrc_est_rabs.rabs[i].rb_for_rab[0].rb_id;

      if(rrcllc_get_dl_ded_rate(wrrc_as_id, RRC_STATE_CELL_DCH,rb_id) != 0)
      {
        ps_zero_rb_found = RRC_PS_DATA_RATE_NON_ZERO_CONFIGURED;
        break;
      }
    }
  }
  return ps_zero_rb_found;
}

/*===========================================================================

FUNCTION   rrc_is_voice_rab_present

DESCRIPTION

  This function indicates whether any voice RAB is present or not in est_rabs database.
  This function is going to be used by MAC/RRC to verify whether Voice RAB is present 
  or not.
  This function has been modified such that it returns TRUE only if CS_VOICE_RAB is of DCH type.        
DEPENDENCIES

None

RETURN VALUE

  TRUE: if voice RAB exist in est_rabs database
  FALSE: if voice RAB doesnt exist in est_rabs database

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_is_voice_rab_present (sys_modem_as_id_e_type wrrc_as_id)
{
  uint8 count; /* Local Counter */
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( RRCRB_CS_VOICE_RAB == rrc_est_rabs.rabs[count].rab_type)
    {
      /* We have a match */
      return( TRUE );
    }
  }
  return(FALSE);
}

/*===========================================================================

FUNCTION   rrc_is_cs_data_rab_present

DESCRIPTION

  This function indicates whether any cs data RAB is present or not in est_rabs database.
  This function is going to be used by RRC to verify whether cs data RAB is present 
  or not.
  This function has been modified such that it returns TRUE only if RRCRB_CS_DATA_RAB is of DCH type.        
DEPENDENCIES

None

RETURN VALUE

  TRUE: if cs data RAB exist in est_rabs database
  FALSE: if cs data RAB doesnt exist in est_rabs database

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_is_cs_data_rab_present (sys_modem_as_id_e_type wrrc_as_id)
{
  uint8 count; /* Local Counter */
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( RRCRB_CS_DATA_RAB == rrc_est_rabs.rabs[count].rab_type)
    {
        /* We have a match */
        return( TRUE );
    }
  }
  return(FALSE);
}

/*===========================================================================

FUNCTION   rrc_is_cs_data_rab_being_configured_or_already_active

DESCRIPTION
           This function checks whehter CS-data rab is already active or being configured

DEPENDENCIES

None

RETURN VALUE

TRUE:  CS-RAB is present
FALSE: CS-RAB is not present

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_is_cs_data_rab_being_configured_or_already_active (sys_modem_as_id_e_type wrrc_as_id)
{
  boolean cs_data_rab_already_active = FALSE, rbe_cs_data_configured = FALSE, rrciho_cs_data_configured = FALSE, cs_data_rab_status = FALSE;
  rrc_proc_e_type proc;
  rrcllc_oc_process_state_e_type process_status;
  rrcllc_oc_set_status_e_type oc_status = OC_NOT_SET;

  oc_status = rrcllc_get_ordered_config_state_and_proc_wo_f3(wrrc_as_id, &proc,
                                          &process_status
                                        );
  if(OC_NOT_SET != oc_status)
  {
    if(RRC_PROCEDURE_RBE == proc)
    {
      rbe_cs_data_configured = rrcrbe_is_cs_data_rab_present(wrrc_as_id);
    }
    else if(RRC_PROCEDURE_IHO == proc)
    {
      rrciho_cs_data_configured = rrciho_is_cs_data_rab_present(wrrc_as_id);
    }
  }
  cs_data_rab_already_active = rrc_is_cs_data_rab_present(wrrc_as_id);

  if(rbe_cs_data_configured || rrciho_cs_data_configured || cs_data_rab_already_active)
  {
    WRRC_MSG3_HIGH("cs_data_rab_already_active %d, rbe_cs_data_configured %d,  rrciho_cs_data_configured %d",cs_data_rab_already_active, rbe_cs_data_configured, rrciho_cs_data_configured);  
    return TRUE;
  }
  else
  {
    return FALSE;
  }
 
}

/*===========================================================================

FUNCTION   rrc_get_rb_id_list_associated_with_voice_rab

DESCRIPTION

  This function gives the list of RB ids associated with a Voice RAB.
        
DEPENDENCIES

  None.

ARGUMENTS
  *num_rb: number of rb associated with Voice RAB, set to 0 if Voice RAB is not established.
                set 'num_rb' to NULL if this function needs to be called to know whether AMR is 
                established or not.
  rb_id_list: list of rbIds associated with Voice RAB

RETURN VALUE

  boolean: TRUE if Voice RAB is established elase FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_get_rb_id_list_associated_with_voice_rab 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32 *num_rb,
  uint32 rb_id_list[MAX_RB_PER_RAB]
)
{
  uint8 count, rb_index; /* Local counters */

  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( RRCRB_CS_VOICE_RAB == rrc_est_rabs.rabs[count].rab_type)
    {
      if (NULL != num_rb)
      {
        *num_rb = rrc_est_rabs.rabs[count].num_rbs_for_rab;
  
        for (rb_index=0; rb_index<(*num_rb); rb_index++)
        {
          rb_id_list[rb_index] = rrc_est_rabs.rabs[count].rb_for_rab[rb_index].rb_id;
        }
      }

      return(TRUE);
    }
  }

  *num_rb = 0;
  return(FALSE);
}

/*===========================================================================

FUNCTION   rrc_get_rab_type_for_given_rb_id 

DESCRIPTION

  This function returns the RAB type of a RAB associated with a given RB id. This function
  returns RRCRB_UNKNOWN_RAB if there is no RAB exist for a given RB id.
        
DEPENDENCIES

  None.

ARGUMENTS
  rbId: rb Id

RETURN VALUE

  rrcrb_rab_e_type: rab type

SIDE EFFECTS

  None.

===========================================================================*/
rrcrb_rab_e_type rrc_get_rab_type_for_given_rb_id 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32 rb_id
)
{
  uint8 count, rb_index; /* Local counters */

  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    for (rb_index=0; rb_index<rrc_est_rabs.rabs[count].num_rbs_for_rab; rb_index++)
    {
      if (rb_id == rrc_est_rabs.rabs[count].rb_for_rab[rb_index].rb_id)
      {
        return(rrc_est_rabs.rabs[count].rab_type);
      }
    }
  }

  /* if it comes here then it means that given rb_id is not part of any established RABs */
  return(RRCRB_UNKNOWN_RAB);
}

/*===========================================================================

FUNCTION   rrc_get_rab_type_for_given_rab_id 

DESCRIPTION

  This function returns the RAB type of a given RAB id. This function returns
  RRCRB_UNKNOWN_RAB if RAB with a given RAB id is not found.
        
DEPENDENCIES

  None.

ARGUMENTS
  rab_Id: RAB Id

RETURN VALUE

  rrcrb_rab_e_type: rab type

SIDE EFFECTS

  None.

===========================================================================*/
rrcrb_rab_e_type rrc_get_rab_type_for_given_rab_id 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32 rab_id
)
{
  uint8 count; /* Local counters */

  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if (rab_id == rrc_est_rabs.rabs[count].rab_id)
    {
      return(rrc_est_rabs.rabs[count].rab_type);
    }
  }

  /* if it comes here then it means that given rab_id is not part of any established RABs */
  return(RRCRB_UNKNOWN_RAB);
}
/*===========================================================================

FUNCTION   rrc_get_idx_in_rrc_est_rabs_for_rab_id

DESCRIPTION

  This function returns the index in rrc_est_rabs for a given rab_id.
        
ARGUMENTS

RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
uint8 rrc_get_idx_in_rrc_est_rabs_for_rab_id(sys_modem_as_id_e_type wrrc_as_id,rrc_rab_id_type rab_id)
{
  uint8 idx=0;
  for(idx=0; idx<MAX_RAB_TO_SETUP;idx++)
  {
    if(rrc_est_rabs.rabs[idx].rab_id == rab_id)
    {
      WRRC_MSG2_MED("RAB id %d found in EST_RABs at idx %d",rab_id,idx);    
      return idx;
    }
  }
  if(idx == MAX_RAB_TO_SETUP)
  {
    WRRC_MSG1_ERROR("RAB id %d not found in EST_RABs",rab_id);    		
  }
  return MAX_RAB_TO_SETUP;
}

/*===========================================================================

FUNCTION   rrc_is_voice_rab_present_for_given_reest_timer 

DESCRIPTION

  This function returns whether voice RAB is present for a given re-establishment timer.
  This function also returns the rab-id of voice RAB it present.
        
DEPENDENCIES

  None.

ARGUMENTS
  re_est_timer: timer for which voice RAB needs to be determined
  *rab_id_ptr: returns RAB id if voice RAB with a given re-establishment timer is present,
                     else returns RRC_INVALID_RAB_ID.
                     Specify this argument as NULL if rab_id is not needed.

RETURN VALUE

  boolean: TRUE if voice RAB with a given re-establishment timer found otherwise FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_is_voice_rab_present_for_given_reest_timer 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_Re_EstablishmentTimer re_est_timer,
  rrc_rab_id_type *rab_id_ptr
)
{
  uint8 count; /* Local counters */

  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if (RRCRB_CS_VOICE_RAB == rrc_est_rabs.rabs[count].rab_type)
    {
      if (rrc_est_rabs.rabs[count].re_est_timer == re_est_timer)
      {
        if (NULL != rab_id_ptr)
        {
          *rab_id_ptr = rrc_est_rabs.rabs[count].rab_id;
        }
        return(TRUE);
      }
      /* there cant be more than one RAB for voice, hence break */
      break;
    }
  }

  if (NULL != rab_id_ptr)
  {
    *rab_id_ptr = RRC_INVALID_RAB_ID;
  }
  /* if it comes here then it means that given rab_id is not part of any established RABs */
  return(FALSE);
}

/*===========================================================================

FUNCTION   rrc_update_rab_type_in_rab_info

DESCRIPTION
 This function is updates the rab type in 'rrc_est_rab_info_type' structure
 using information stored in ASN1 'rrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'rrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  rrc_RAB_InformationSetup *: pointer to ASN1 structure,
  rrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

rrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
rrcrb_rab_e_type rrc_update_rab_type_in_rab_info
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RAB_InformationSetup *rab_info_setup_ptr,
  rrc_est_rab_info_type *est_rab_info_ptr 
)
{
  rrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (rrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = RRCRB_PS_DATA_RAB;
  }
  else if (rrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = rrcrb_translate_gsm_map_rab_id(wrrc_as_id,(rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity ));
      
#ifdef FEATURE_DUAL_SIM
    mn_rab_type = rrcmn_verify_rab_subs( (radio_access_bearer_T)rab_id,rrc_get_as_id(wrrc_as_id));
#else
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
    WRRC_MSG1_HIGH("MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if ( rrclbt_is_lb_on(wrrc_as_id) == TRUE )
    {
      /* Set Rab type to Data */
      MSG_LOW("RAB setup for Loopback Test", 0, 0, 0);
      est_rab_info_ptr->rab_type = RRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type  =RRCRB_CS_DATA_RAB ;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)
    {
      est_rab_info_ptr->rab_type = RRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = RRCRB_UNKNOWN_RAB;
      WRRC_MSG1_ERROR("Invalid cn_domain value: %d", cn_domain);
    }
  }
  else
  {
    WRRC_MSG1_ERROR("Invalid Cn domain %d",cn_domain);
  }


  WRRC_MSG3_MED("Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
    est_rab_info_ptr->rab_type, RRCRB_CS_VOICE_RAB, RRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}

/*===========================================================================

FUNCTION   rrc_update_rab_type_in_rab_info_r5

DESCRIPTION
 This function is updates the rab type in 'rrc_est_rab_info_type' structure
 using information stored in ASN1 'rrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'rrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  rrc_RAB_InformationSetup *: pointer to ASN1 structure,
  rrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

rrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
rrcrb_rab_e_type rrc_update_rab_type_in_rab_info_r5
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RAB_InformationSetup_r5 *rab_info_setup_ptr,
  rrc_est_rab_info_type *est_rab_info_ptr 
)
{
  rrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (rrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = RRCRB_PS_DATA_RAB;
  }
  else if (rrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = rrcrb_translate_gsm_map_rab_id(wrrc_as_id,(rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity));
#ifdef FEATURE_DUAL_SIM
    mn_rab_type = rrcmn_verify_rab_subs( (radio_access_bearer_T)rab_id,rrc_get_as_id(wrrc_as_id));
#else
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
    WRRC_MSG1_HIGH("MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if ( rrclbt_is_lb_on(wrrc_as_id) == TRUE )
    {
      /* Set Rab type to Data */
      MSG_LOW("RAB setup for Loopback Test", 0, 0, 0);
      est_rab_info_ptr->rab_type = RRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type  =RRCRB_CS_DATA_RAB ;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)
    {
      est_rab_info_ptr->rab_type = RRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = RRCRB_UNKNOWN_RAB;
      WRRC_MSG1_ERROR("Invalid cn_domain value: %d", cn_domain);
    }
  }
  else
  {
    WRRC_MSG1_ERROR("Invalid Cn domain %d",cn_domain);
  }

  WRRC_MSG3_MED("Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
    est_rab_info_ptr->rab_type, RRCRB_CS_VOICE_RAB, RRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}

/*===========================================================================

FUNCTION   rrc_get_3g_cell_capability_indicator

DESCRIPTION

  This function will indicate if the current cell supports HSDPA.
        
DEPENDENCIES

  None
 
RETURN VALUE

  None
SIDE EFFECTS

  None.

===========================================================================*/
sys_hs_ind_e_type rrc_get_3g_cell_capability_indicator (sys_modem_as_id_e_type wrrc_as_id)
{
  sys_hs_ind_e_type cell_capability_indicator = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
  rrc_SysInfoType5 *sib5_ptr = NULL;

  if(!rrcsib_is_serving_cell_sib_vaild(wrrc_as_id))
  {
    WRRC_MSG0_HIGH("Serving Cell Index is not Valid");
    return cell_capability_indicator;
  }

  sib5_ptr = (rrc_SysInfoType5*)  rrc_sibdb_return_sib_for_srv_cell(wrrc_as_id,rrc_SIB5);
  
  if (sib5_ptr == NULL)
  {
    WRRC_MSG0_ERROR("SIB5 Ptr NULL");
    return cell_capability_indicator;
  }

  /* Parse SIB5 to check if the cell supports HSDPA */
  if ((sib5_ptr->m.v4b0NonCriticalExtensionsPresent) &&
      (sib5_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionsPresent) &&
      (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.m.v650NonCriticalExtensionsPresent) && 
       (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.
       v650NonCriticalExtensions.m.v680NonCriticalExtensionsPresent))
  {
    if ((sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.
       v680NonCriticalExtensions.m.sysInfoType5_v680extPresent) &&
       (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.
       v680NonCriticalExtensions.sysInfoType5_v680ext.m.hsdpa_CellIndicatorPresent))
    {
      WRRC_MSG1_HIGH("HSDPA CAPABLE PSC %d",rrc_ccm_get_curr_camped_psc(wrrc_as_id));
      cell_capability_indicator =  SYS_HS_IND_HSDPA_SUPP_CELL;
    }

    if (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.
       v680NonCriticalExtensions.m.v690NonCriticalExtensionsPresent)
    {
      if (rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL6)
      {
        if (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.
         v680NonCriticalExtensions.v690NonCriticalExtensions.sysInfoType5_v690ext.m.edch_CellIndicatorPresent)
        {
          if (SYS_HS_IND_HSDPA_SUPP_CELL == cell_capability_indicator)
          {
            WRRC_MSG1_HIGH("HSUPA(and HSDPA) CAPABLE PSC %d",rrc_ccm_get_curr_camped_psc(wrrc_as_id));
            cell_capability_indicator = SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL;
          }
          else
          {
            WRRC_MSG1_HIGH("HSUPA CAPABLE PSC %d",rrc_ccm_get_curr_camped_psc(wrrc_as_id));
            cell_capability_indicator = SYS_HS_IND_HSUPA_SUPP_CELL;
          }
        }
      }
    }

  }
  else
  {
    WRRC_MSG1_HIGH("HSDPACAPABLE NOT PSC %d",rrc_ccm_get_curr_camped_psc(wrrc_as_id));
    cell_capability_indicator = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
  }

  return cell_capability_indicator;

}

/*===========================================================================

FUNCTION          RRC_SEND_3G_CELL_CAPABILITY_INFORMATION

DESCRIPTION       This function sends 3G cell related info to MM

DEPENDENCIES
                  None
                  
RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void rrc_send_3g_cell_capability_information(sys_modem_as_id_e_type wrrc_as_id,sys_hs_ind_e_type hsdpa_hsupa_indication)
{
  mm_cmd_type *mm_buf_ptr;

  WRRC_MSG0_HIGH("Dispatching out MM cmd / Send RRC_HSDPA_HSUPA_CELL_IND");
  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_rrc_get_cmd_buf(RRC_HSDPA_HSUPA_CELL_IND)) == NULL)
  {
    WRRC_ERR0_FATAL("No MM buffer for RRC_HSDPA_HSUPA_CELL_IND");
  }

#ifdef FEATURE_DUAL_SIM
   mm_buf_ptr->cmd.rrc_3g_availability_cell_ind.as_id = rrc_get_as_id(wrrc_as_id);
#endif

  mm_buf_ptr->cmd.rrc_3g_availability_cell_ind.hsdpa_hsupa_ind = hsdpa_hsupa_indication;

  /* Put the command on MM queue */
  rrc_send_rrc_message_to_mm(wrrc_as_id,mm_buf_ptr);
  
} /* rrc_send_3g_cell_capability_information */
#ifdef FEATURE_HSPA_CALL_STATUS_IND
/*===========================================================================

FUNCTION          RRC_SEND_HSPA_CALL_STATUS_INFO

DESCRIPTION       This function sends information on whether HSDPA and HSUPA
                  calls are in progress or not

DEPENDENCIES
                  None
                  
RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void rrc_send_hspa_call_status_info
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  mm_cmd_type *mm_buf_ptr;
   
  ordered_config_type * config_ptr = NULL;


  if(config_ptr_in_use == ORDERED_CONFIG)
  {
    config_ptr = ordered_config_ptr;
  }
  else
  {
    config_ptr = current_config_ptr;
  }
  rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;


  if (rrcdata_hs_info.hsdpa_status == HSDPA_ACTIVE) 
  {
    if(config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_EHS )
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_SUPP_CELL;
    }
    else
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPA_SUPP_CELL;
    }
  }

#ifdef FEATURE_WCDMA_HS_FACH
  if((rrcllc_get_hs_status_in_e_fach(wrrc_as_id,config_ptr) == TRUE) &&
     (config_ptr->efach_hrnti_status == DEDICATED_HRNTI)
    )
  {
    rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_SUPP_CELL;
  }
#endif

  if ((rrc_is_eul_active(wrrc_as_id)==TRUE)
#ifdef FEATURE_WCDMA_HS_RACH
       || (rrchsrach_get_common_edch_transmission(wrrc_as_id,config_ptr) == TRUE)
#endif
      ) 
  {
    if (rrcdata_hs_info.curr_hspa_call_status == SYS_HS_IND_HSDPA_SUPP_CELL) 
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL;
    }
    else if (rrcdata_hs_info.curr_hspa_call_status == SYS_HS_IND_HSDPAPLUS_SUPP_CELL) 
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_HSUPA_SUPP_CELL;
    }
    else
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSUPA_SUPP_CELL;
    }
  }

  if((rrcllc_get_64QAM_status(wrrc_as_id) == TRUE) && 
     ((rrcdata_hs_info.hsdpa_status == HSDPA_ACTIVE)
#ifdef FEATURE_WCDMA_HS_FACH
        || ((rrcllc_get_hs_status_in_e_fach(wrrc_as_id,config_ptr) == TRUE) &&
            (config_ptr->efach_hrnti_status == DEDICATED_HRNTI)
           )
#endif
      )
     )
  {
    if((rrc_is_eul_active(wrrc_as_id)==TRUE)
#ifdef FEATURE_WCDMA_HS_RACH
       ||(rrchsrach_get_common_edch_transmission(wrrc_as_id,config_ptr) == TRUE)
#endif
      )
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_64QAM_HSUPA_SUPP_CELL;
    }
    else
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_64QAM_SUPP_CELL;
    }
  }

#ifdef FEATURE_WCDMA_DC_HSDPA
  /*Update the call status only if DC-HS is set to active.*/
  if(rrcllcpcie_return_num_sec_hsdpa_carriers(wrrc_as_id,config_ptr) != 0)
  {
    /*Whether MAC-HS or MAC-EHS is active with EUL, update call status as same.
    it might be changed later if DC-HS status needed separately for MAC-HS or EHS.*/
    if((rrc_is_eul_active(wrrc_as_id)==TRUE)
#ifdef FEATURE_WCDMA_HS_RACH
       ||(rrchsrach_get_common_edch_transmission(wrrc_as_id,config_ptr) == TRUE)
#endif
      )
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_DC_HSDPAPLUS_HSUPA_SUPP_CELL;
    }
    else
    {
      rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_DC_HSDPAPLUS_SUPP_CELL;
    }
    WRRC_MSG1_HIGH("Updated Call status is %d",rrcdata_hs_info.curr_hspa_call_status);
    if (rrcdata_hs_info.prev_hspa_call_status != rrcdata_hs_info.curr_hspa_call_status) 
    {
      /*DL DC configured case, Update CMapi about current cfgd freqs*/
      rrcmeas_send_qmi_current_freqs_info_ind(wrrc_as_id, RRC_DOWNLINK_SDU_IND);
    }
  }
#endif /*FEATURE_WCDMA_DC_HSDPA*/

#ifdef FEATURE_WCDMA_DC_HSUPA
  if(TRUE == config_ptr->sec_e_dch_transmission)
  {
    rrcdata_hs_info.curr_hspa_call_status = SYS_HS_IND_DC_HSDPAPLUS_DC_HSUPA_SUPP_CELL;
  }
#endif

  if (rrcdata_hs_info.prev_hspa_call_status != rrcdata_hs_info.curr_hspa_call_status) 
  {

    if(((rrcdata_hs_info.prev_hspa_call_status == SYS_HS_IND_DC_HSDPAPLUS_HSUPA_SUPP_CELL) ||
        (rrcdata_hs_info.prev_hspa_call_status == SYS_HS_IND_DC_HSDPAPLUS_SUPP_CELL)) &&
       ((rrcdata_hs_info.curr_hspa_call_status != SYS_HS_IND_DC_HSDPAPLUS_HSUPA_SUPP_CELL) &&
        (rrcdata_hs_info.curr_hspa_call_status != SYS_HS_IND_DC_HSDPAPLUS_SUPP_CELL)))
    {
      /*DL DC deconfigured case, Update CMapi about current cfgd freqs*/
      rrcmeas_send_qmi_current_freqs_info_ind(wrrc_as_id, RRC_DOWNLINK_SDU_IND);
    }

    /* Get a buffer from MM task */
    if((mm_buf_ptr = mm_rrc_get_cmd_buf(RRC_HSPA_CALL_STATUS_IND)) == NULL)
    {
      WRRC_ERR0_FATAL("No MM buffer for RRC_HSPA_CALL_STATUS_IND");
    }
    
    mm_buf_ptr->cmd.rrc_hspa_call_status_ind.hs_call_status_ind = rrcdata_hs_info.curr_hspa_call_status;
    
    rrcdata_hs_info.prev_hspa_call_status = rrcdata_hs_info.curr_hspa_call_status;

#ifdef FEATURE_DUAL_SIM
    mm_buf_ptr->cmd.rrc_hspa_call_status_ind.as_id = rrc_get_as_id(wrrc_as_id);
#endif

    WRRC_MSG1_HIGH("Dispatching out MM cmd / Send RRC_HSPA_CALL_STATUS_IND %d", rrcdata_hs_info.curr_hspa_call_status);

    /* Put the command on MM queue */
    rrc_send_rrc_message_to_mm(wrrc_as_id,mm_buf_ptr);
  } 

} /* rrc_send_hspa_call_status_info */
#endif /* FEATURE_HSPA_CALL_STATUS_IND */

/*===========================================================================

FUNCTION   rrc_indicate_layer1_next_config_is_for_voice

DESCRIPTION

  This function will indicate layer1 that next configuration is for AMR by calling some 
  feature specific call back function provided by layer1.
        
DEPENDENCIES

  None
 
RETURN VALUE

  None
SIDE EFFECTS

  None.

===========================================================================*/
void rrc_indicate_layer1_next_config_is_for_voice (sys_modem_as_id_e_type wrrc_as_id)
{
  /* If this rab is voice rab then disable ul tfc restriction based 
     on total power.
  */
  WRRC_MSG0_MED("Disable UL TX PWR restriction -AMR is going to be established ");

}

/*===========================================================================

FUNCTION   rrc_indicate_layer1_next_config_will_release_voice_rab

DESCRIPTION

  This function will indicate layer1 that next configuration to physical layer
  will release AMR. This is done by calling some feature specific call back
  function provided by layer1.
        
DEPENDENCIES

  None
 
RETURN VALUE

  None
SIDE EFFECTS

  None.

===========================================================================*/
void rrc_indicate_layer1_next_config_will_release_voice_rab (sys_modem_as_id_e_type wrrc_as_id)
{
  /* If this rab is voice rab then disable ul tfc restriction based 
     on total power.
  */
  WRRC_MSG0_HIGH("Enable UL TX PWR restriction - AMR to be released");
}

/*===========================================================================

FUNCTION   rrc_is_given_rb_rel_list_releases_all_rb_for_voice_rab

DESCRIPTION

  This function determines whether the given 'rrc_RB_InformationReleaseList'
  will result in release of voice call or not.
        
DEPENDENCIES

  None
 
RETURN VALUE

  TRUE: If voice RAB is not present in est_rabs or if rb_release_list will result
        in releasing all RBs associated with RAB.
  FALSE: If voice RAB still exist after processing rb_release_list.

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_is_given_rb_rel_list_releases_all_rb_for_voice_rab 
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RB_InformationReleaseList *rb_release_list_ptr
)
{
  uint32 count,index,n_value; /* local counter */
  uint32 num_rb, temp_num_rb;
  uint32 rb_id_list[MAX_RB_PER_RAB];

  n_value = rb_release_list_ptr->n;
  if (TRUE == rrc_get_rb_id_list_associated_with_voice_rab(wrrc_as_id,&num_rb, rb_id_list))
  {
    temp_num_rb = num_rb;
    index = 0;
    while(n_value != 0)
    {
      for (count=0; count<num_rb; count++)
      {
        if (rb_id_list[count] == rb_release_list_ptr->elem[index])
        {
          --temp_num_rb;
          break;
        }
      }
      /* Advance the Linked List pointer */
      n_value--;
      index++;
    }

    /* if num_rb is not 0 at this point then it means voice RAB still exist */
    if (0 != temp_num_rb)
    {
      return(FALSE);
    }
    else
    {
      WRRC_MSG0_HIGH("Given configuration will release all RBs associated with Voice RAB");
    }
  }
  /* if we reach this point then it means that either voice RAB is not present or 
     all RBs associated with voice RAB are going to be released 
  */
  return(TRUE);
}


/*===========================================================================

FUNCTION rrc_set_hsdpa_action

DESCRIPTION

  This function sets HSDPA action in global variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_set_hsdpa_action
(
  sys_modem_as_id_e_type wrrc_as_id, 
  hsdpa_action_enum_type action
)
{
  if(action != rrcdata_hs_info.hsdpa_action)
  {
    WRRC_MSG1_HIGH("RRCHS:Set HSDPA Action to %d", action);
  }
  rrcdata_hs_info.hsdpa_action = action;
}

/*===========================================================================

FUNCTION rrc_get_hsdpa_action

DESCRIPTION

  This function returns HSDPA action variable from global variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
hsdpa_action_enum_type rrc_get_hsdpa_action
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  WRRC_MSG1_MED("RRCHS:HSDPA Action returned is %d", rrcdata_hs_info.hsdpa_action);
  return rrcdata_hs_info.hsdpa_action;
}

/*===========================================================================

FUNCTION rrc_get_hsdpa_action_wo_f3

DESCRIPTION

  This function returns HSDPA action variable from global variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
__attribute__((section(".uncompressible.text")))
#endif
hsdpa_action_enum_type rrc_get_hsdpa_action_wo_f3
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_hs_info.hsdpa_action;
}


/*===========================================================================

FUNCTION rrc_set_hsdpa_status

DESCRIPTION

  This function sets HSDPA Status in HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_set_hsdpa_status(sys_modem_as_id_e_type wrrc_as_id,rrc_hsdpa_status_enum_type status)
{
  if(status != rrcdata_hs_info.hsdpa_status)
  {
    WRRC_MSG1_HIGH("RRCHS:NEW HSDPA Status %d", status);
  }
  rrcdata_hs_info.hsdpa_status = status;
}

/*===========================================================================

FUNCTION rrc_get_hsdpa_sts

DESCRIPTION

  This function returns HSDPA Status from HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

rrc_hsdpa_status_enum_type rrc_get_hsdpa_sts
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  WRRC_MSG1_MED("RRCHS:HSDPA Status returned is %d", rrcdata_hs_info.hsdpa_status);
  return rrcdata_hs_info.hsdpa_status;
}
/*===========================================================================

FUNCTION rrc_get_hsdpa_sts_wo_f3

DESCRIPTION

  This function returns HSDPA Status from HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
	__attribute__((section(".uncompressible.text")))
#endif
rrc_hsdpa_status_enum_type rrc_get_hsdpa_sts_wo_f3
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_hs_info.hsdpa_status;
}


/*===========================================================================

FUNCTION rrc_get_hsdpa_status_sub

DESCRIPTION

  This function returns HSDPA Status from HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

rrc_hsdpa_status_enum_type rrc_get_hsdpa_status_sub
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrc_get_hsdpa_sts(wrrc_as_id);
}

/*===========================================================================

FUNCTION rrc_get_hsdpa_status

DESCRIPTION

  This function returns HSDPA Status from HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
__attribute__((section(".uncompressible.text")))
#endif
rrc_hsdpa_status_enum_type rrc_get_hsdpa_status
(
  void
)
{
  return rrc_get_hsdpa_sts(SYS_MODEM_AS_ID_1);
}


/*===========================================================================

FUNCTION rrc_get_hsdpa_plus_status

DESCRIPTION

  This function returns HSDPA Status from HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  HSDPA PLUS Status  of type rrc_hsdpa_plus_status_enum_type
 
SIDE EFFECTS

  None

===========================================================================*/

rrc_hsdpa_plus_status_enum_type rrc_get_hsdpa_plus_status
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  ordered_config_type * config_ptr = NULL;
  rrc_hsdpa_plus_status_enum_type hsdpa_plus_status = HSDPA_PLUS_INACTIVE;
  if(rrcdata_hs_info.hsdpa_status == HSDPA_ACTIVE )
  {
    if(config_ptr_in_use == ORDERED_CONFIG)
    {
      config_ptr = ordered_config_ptr;
    }
    else
    {
      config_ptr = current_config_ptr;
    }
    
    if(config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_EHS )
    {
      hsdpa_plus_status =  HSDPA_PLUS_ACTIVE;
    }
     
  }

  WRRC_MSG2_HIGH("HSDPA PLUS status %d HSDPA status %d ",hsdpa_plus_status,rrcdata_hs_info.hsdpa_status);
  return hsdpa_plus_status;
}

/*====================================================================
FUNCTION: rrc_set_rlc_size_change_status()

DESCRIPTION:
  This function gets the RLC PDU Size per RB

DEPENDENCIES:
  None

RETURN VALUE:
  RLC Size

SIDE EFFECTS:
  None.
====================================================================*/
void rrc_set_rlc_size_change_status(sys_modem_as_id_e_type wrrc_as_id,rrc_proc_e_type proc_id, boolean rlc_size_change_status,
                                       rrc_RB_Identity rb_id, rlc_reestablish_e_type direction)
{
  WRRC_MSG3_HIGH("NewProc %d rlcsizestatus %d, OldProc %d",proc_id, rlc_size_change_status,
    rrcdata_misc_info.rlc_size_change_in_progress.proc_id);
  
  rrcdata_misc_info.rlc_size_change_in_progress.proc_id = proc_id;
  rrcdata_misc_info.rlc_size_change_in_progress.is_rlc_size_changed = rlc_size_change_status;
  rrcdata_misc_info.rlc_size_change_in_progress.rb_id = rb_id;
  rrcdata_misc_info.rlc_size_change_in_progress.direction = direction;
}

/*====================================================================
FUNCTION: rrc_get_rlc_size_change_status_for_procedure_id()

DESCRIPTION:
  This function gets the RLC Size Change Status based on the Procedure Id

DEPENDENCIES:
  None

RETURN VALUE:
  RLC Size

SIDE EFFECTS:
  None.
====================================================================*/
boolean rrc_get_rlc_size_change_status_for_procedure_id(sys_modem_as_id_e_type wrrc_as_id,rrc_proc_e_type proc_id)
{

  WRRC_MSG3_HIGH("Procedure rrc_proc_e_type_value%d, RLC Size Proc Id rrc_proc_e_type_value%d, Status %d", proc_id,
          rrcdata_misc_info.rlc_size_change_in_progress.proc_id,rrcdata_misc_info.rlc_size_change_in_progress.is_rlc_size_changed);

  if (rrcdata_misc_info.rlc_size_change_in_progress.proc_id == RRC_PROCEDURE_NONE)
  {
    return FALSE;
  }
  else if (proc_id == rrcdata_misc_info.rlc_size_change_in_progress.proc_id)
  {
    return rrcdata_misc_info.rlc_size_change_in_progress.is_rlc_size_changed;
  }
  else
  {
     return rrcdata_misc_info.rlc_size_change_in_progress.is_rlc_size_changed;
  }
     
}
/*====================================================================
FUNCTION: rrc_check_and_init_rrc_reestab_entity()

DESCRIPTION:
 This function clears out cheks and clears reestab_entity in OC and CC

DEPENDENCIES:
  None

RETURN VALUE:
  None

SIDE EFFECTS:
  None.
====================================================================*/
void rrc_check_and_init_rrc_reestab_entity(sys_modem_as_id_e_type wrrc_as_id,rrc_proc_e_type proc_id)
{ 

  boolean init_reestab_entity = FALSE;

  if(proc_id == RRC_PROCEDURE_CU)
  { 
    /* Do not clear re-estab entity if it is set by reconfig procedure */
    if(rrcdata_misc_info.rlc_size_change_in_progress.proc_id == RRC_PROCEDURE_NONE)
    {
     init_reestab_entity = TRUE;
    }
  }        
  else if((proc_id == RRC_PROCEDURE_RBRC) || 
          (proc_id == RRC_PROCEDURE_RBE) ||
          (proc_id == RRC_PROCEDURE_RBR) ||
          (proc_id == RRC_PROCEDURE_TCR) ||
          (proc_id == RRC_PROCEDURE_PCR)  )
  {
    init_reestab_entity = TRUE;
  }

  if((init_reestab_entity == TRUE) && (ordered_config_ptr != NULL) && (current_config_ptr != NULL))
  {
     rrcllc_init_reestablish_entity(wrrc_as_id, ORDERED_AND_CURRENT_CONFIG);
  } 
}

/*===========================================================================

FUNCTION rrc_initialize_rlc_size_change

DESCRIPTION

  This function initializes the RLC Size Change structure
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
void rrc_initialize_rlc_size_change
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  rrcdata_misc_info.rlc_size_change_in_progress.proc_id = RRC_PROCEDURE_NONE;
  rrcdata_misc_info.rlc_size_change_in_progress.is_rlc_size_changed = FALSE;
  rrcdata_misc_info.rlc_size_change_in_progress.direction = RLC_RE_ESTABLISH_NONE;
  rrcdata_misc_info.rlc_size_change_in_progress.rb_id = RRC_INVALID_RB_ID;
}

/*===========================================================================

FUNCTION rrc_initialize_rlc_size_per_rb

DESCRIPTION

  This function initializes the RLC Size per RB in the global structure
  rlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_initialize_rlc_size_per_rb
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  uint8 cnt = 0;
  rrcdata_misc_info.rlc_size_per_rb.no_of_rb = 0;
  for (cnt = 0; cnt < MAX_RB; cnt++)
  {
    rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rlc_dl_entity = RRC_DL_INVALID_RLC;
    rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rb_id = RRC_INVALID_RB_ID;
    rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size_present = FALSE;
     rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size = INVALID_DL_AM_PDU_RLC_SIZE;

    rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_li_size_present = FALSE;
    rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size = RRC_DL_UM_INVALID_RLC;

  }
  rrcdata_misc_info.ps_rb_re_establish_indicator = FALSE;

}


/*===========================================================================

FUNCTION rrc_get_dl_am_rlc_pdu_size

DESCRIPTION

  This function finds the DL AM RLC PDU Size as per the RB specified from the 
  global structure rlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  DL AM RLC PDU Size if found else INVALID_DL_AM_PDU_RLC_SIZE
 

SIDE EFFECTS

  None

===========================================================================*/

uint32 rrc_get_dl_am_rlc_pdu_size
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RB_Identity rb_id
)
{
  uint32 pdu_size = INVALID_DL_AM_PDU_RLC_SIZE;
  /* Check if rb present in rlc_size_per_rb */
  uint8 cnt = 0;
  for (cnt = 0; cnt < rrcdata_misc_info.rlc_size_per_rb.no_of_rb; cnt++)
  {
    if ((rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rb_id == rb_id) &&
        (rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rlc_dl_entity == RRC_DL_AM_RLC) &&
        (rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size_present == TRUE) &&
        (rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.
         dl_rlc_am_pdu_size != INVALID_DL_AM_PDU_RLC_SIZE))
    {
      return rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size;
    }
  }
  WRRC_MSG1_ERROR("RRC:Not found RLC DL AM Size RB %d", rb_id);
  return pdu_size;

}


/*===========================================================================

FUNCTION rrc_get_dl_um_li

DESCRIPTION

  This function finds the DL UM LI as per the RB specified from the 
  global structure rlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  DL UM LI Size if found else INVALID_DL_AM_PDU_RLC_SIZE
 

SIDE EFFECTS

  None

===========================================================================*/

rrc_rlc_dl_um_li_e_type rrc_get_dl_um_li
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RB_Identity rb_id
)
{
  rrc_rlc_dl_um_li_e_type um_li = RRC_DL_UM_INVALID_RLC;
  /* Check if rb present in rlc_size_per_rb */
  uint8 cnt = 0;
  for (cnt = 0; cnt < rrcdata_misc_info.rlc_size_per_rb.no_of_rb; cnt++)
  {
    if ((rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rb_id == rb_id) &&
        (rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rlc_dl_entity == RRC_DL_UM_RLC) &&
        (rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_li_size_present == TRUE) &&
        (rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size
         != RRC_DL_UM_INVALID_RLC))
    {
      WRRC_MSG2_MED("RRCHS:Found RB %d,RLC DL UM LI 0:7/1:15 %d", 
               rb_id,rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size); 
      return rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size;
    }
  }
  WRRC_MSG1_ERROR("RRCHS:Not found RLC DL UM LI for RB %d", rb_id);
  return um_li;

}

/*===========================================================================

FUNCTION rrc_set_dl_am_rlc_pdu_size

DESCRIPTION

  This function sets the DL AM RLC PDU Size as per the RB specified in the 
  global structure rlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  DL AM RLC PDU Size if found else INVALID_DL_AM_PDU_RLC_SIZE
 

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type rrc_set_dl_am_rlc_pdu_size
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RB_Identity rb_id,
  uint32 dl_rlc_am_pdu_size
)
{
  /* Check if rb present in rlc_size_per_rb */
  uint8 cnt = 0;
  
  for (cnt = 0; cnt < MAX_RB; cnt++)
  {
    if (rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rb_id == RRC_INVALID_RB_ID)
    {
      /* Set RB Id & RLC Entity */
      rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rb_id = rb_id;
      rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rlc_dl_entity = RRC_DL_AM_RLC;
      rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size_present = TRUE;
      rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size = dl_rlc_am_pdu_size;
      rrcllc_set_am_dl_rlc_size_ota(wrrc_as_id,rb_id,dl_rlc_am_pdu_size,RRC_FIXED_PDU_SIZE_SET);     
      rrcdata_misc_info.rlc_size_per_rb.no_of_rb++;
      
      
      WRRC_MSG2_MED("RRCHS:RB %d,RLC DL AM PDU Size %d set", 
        rb_id,rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size); 
      return SUCCESS;
    }
  }
  
  WRRC_MSG2_ERROR("RRCHS:Cannot set RLC DL AM PDU Size for RB %d", rb_id,dl_rlc_am_pdu_size);
  return FAILURE;

}


/*===========================================================================

FUNCTION rrc_set_dl_um_li

DESCRIPTION

  This function sets the DL UM LI indication as per the RB specified in the 
  global structure rlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  Success/Failure
 

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type rrc_set_dl_um_li
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RB_Identity rb_id,
  rrc_DL_UM_RLC_LI_size dl_rlc_um_li
)
{
  uint8 cnt = 0;

  for (cnt = 0; cnt < MAX_RB; cnt++)
  {
    if (rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rb_id == RRC_INVALID_RB_ID)
    {
      /* Set RB Id & RLC Entity */
      rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rb_id = rb_id;
      rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].rlc_dl_entity = RRC_DL_UM_RLC;
      rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_li_size_present = TRUE;

      rrcdata_misc_info.rlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size = 
        (dl_rlc_um_li == rrc_DL_UM_RLC_LI_size_size7) ? RRC_DL_UM_RLC_LI_SIZE_7: RRC_DL_UM_RLC_LI_SIZE_15;

      rrcdata_misc_info.rlc_size_per_rb.no_of_rb++;
      
      return SUCCESS;
    }
  }
  
  WRRC_MSG1_ERROR("RRCHS:Cannot set DL RLC UM LI for RB %d", rb_id);
  return FAILURE;

}

/*===========================================================================

FUNCTION          RRC_TRANSLATE_HRNTI


DESCRIPTION       

  This function translates HRNTI type defined by ASN.1 into
  the internal type used by RRC. 

DEPENDENCIES

  Both the internal and external HRNTI pointers must be initialized.
  If either one is NULL, the convertion will fail.

RETURN VALUE
                  
  Success/Failure

SIDE EFFECTS
                  
  None

===========================================================================*/
uecomdef_status_e_type rrc_translate_hrnti
(
  sys_modem_as_id_e_type wrrc_as_id, 
  ASN1BitStr32  *ext_hrnti,          /* Pointer to external HRNTI type */
  uint16  *int_hrnti    /* Pointer to internal HRNTI type */
)
{
  /* Make sure the internal and external pointers are initialized */
  if( ext_hrnti == NULL )
  {
    WRRC_MSG0_ERROR("External HRNTI not initialized");
    return FAILURE;
  }
  
  if(ext_hrnti->numbits!= 16 )
  {
    WRRC_MSG1_ERROR("Received HRNTI Length: %d is invalid",
               ext_hrnti->numbits);
    return FAILURE;
  }

  /* Convert 16 bit HRNTI into internal format */
  *int_hrnti = *(ext_hrnti->data) << 8;
  *int_hrnti |= *(ext_hrnti->data + 1);

  return SUCCESS;
}


/*===========================================================================

FUNCTION RRC_HSDPA_INITIAL_VALIDATION

DESCRIPTION

  This function does the initial level validation of HSDPA.
  
DEPENDENCIES


RETURN VALUE
  Success/Failure
 

SIDE EFFECTS

  None

===========================================================================*/
uecomdef_status_e_type rrc_hsdpa_initial_validation
(    
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RRC_StateIndicator rrc_state_from_msg,
   OSS_UINT32 rrc_mode_Specific_Info_tdd_chosen
)
{
  uecomdef_status_e_type status = SUCCESS;
  MSG_LOW("RRC State %d Msg State %d", rrc_get_state(wrrc_as_id), rrc_state_from_msg,0);
  switch (rrc_get_state(wrrc_as_id))
  {
    case RRC_STATE_CELL_FACH:
    case RRC_STATE_CELL_DCH:
      if(rrc_mode_Specific_Info_tdd_chosen)
      {
        WRRC_MSG0_ERROR("RRCHS:TDD choice in HSPDSCH");
        status = FAILURE;
      }
      
      break;
  
    case RRC_STATE_CELL_PCH:
    case RRC_STATE_URA_PCH:
    default:
      WRRC_MSG0_ERROR("RRCHS:Invalid state for HSDPA check");
      status = FAILURE;
      break;
      
  } /* End of switch */
  
  return status;
}


/*===========================================================================
FUNCTION   rrc_is_ps_domain_rab_present

DESCRIPTION
  This function returns TRUE if any PS RAB is present in EST_RABs.
        
DEPENDENCIES
  None

ARGUMENTS:
  None

RETURN VALUE
  TRUE - PS RAB is present.
  FALSE - PS RAB isn't present.

SIDE EFFECTS
  None.
===========================================================================*/
boolean rrc_is_ps_domain_rab_present
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  uint8 rab_idx = 0;

  for(rab_idx = 0; rab_idx < MAX_RAB_TO_SETUP; rab_idx++)
  {
    if((rrc_est_rabs.rabs[rab_idx].rab_id != RRC_INVALID_RAB_ID) && 
        (rrc_est_rabs.rabs[rab_idx].cn_domain == RRC_PS_DOMAIN_CN_ID))
    {
      return TRUE;
    }
  }
  return FALSE;
}


/*===========================================================================

FUNCTION   rrc_update_rab_type_in_rab_info_r7

DESCRIPTION
 This function is updates the rab type in 'rrc_est_rab_info_type' structure
 using information stored in ASN1 'rrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'rrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  rrc_RAB_InformationSetup *: pointer to ASN1 structure,
  rrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

rrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
rrcrb_rab_e_type rrc_update_rab_type_in_rab_info_r7
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RAB_InformationSetup_r7 *rab_info_setup_ptr,
  rrc_est_rab_info_type *est_rab_info_ptr 
)
{
  rrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (rrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = RRCRB_PS_DATA_RAB;
  }
  else if (rrc_CN_DomainIdentity_cs_domain == cn_domain)
  {

    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = rrcrb_translate_gsm_map_rab_id(wrrc_as_id,(rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity));
#ifdef FEATURE_DUAL_SIM
    mn_rab_type = rrcmn_verify_rab_subs( (radio_access_bearer_T)rab_id ,rrc_get_as_id(wrrc_as_id));
#else
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
    WRRC_MSG1_HIGH( "MN RAB type %d",mn_rab_type);//auto-gen, to change remove '' and first param

    /* Check if it is in loopback test - do we really need this check ? */
    if ( rrclbt_is_lb_on(wrrc_as_id) == TRUE )
    {
      /* Set Rab type to Data */
      MSG_LOW("RAB setup for Loopback Test", 0, 0, 0);
      est_rab_info_ptr->rab_type = RRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type = RRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)
    {
      est_rab_info_ptr->rab_type = RRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = RRCRB_UNKNOWN_RAB;
      WRRC_MSG1_ERROR("Invalid cn_domain value: %d", cn_domain);
    }
  }
  else
  {
    WRRC_MSG1_ERROR("Invalid Cn domain %d",cn_domain);
  }
  WRRC_MSG3_MED("Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
    est_rab_info_ptr->rab_type, RRCRB_CS_VOICE_RAB, RRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}

/*===========================================================================

FUNCTION RRC_HSDPA_INITIAL_VALIDATION_r6

DESCRIPTION

  This function does the initial level validation of HSDPA.
  
DEPENDENCIES


RETURN VALUE
  Success/Failure
 

SIDE EFFECTS

  None

===========================================================================*/
uecomdef_status_e_type rrc_hsdpa_initial_validation_r6
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RRC_StateIndicator rrc_state_from_msg,
  rrc_DL_HSPDSCH_Information_r6* hspdsch_info_ptr
)
{
  uecomdef_status_e_type status = SUCCESS;
  MSG_LOW("RRC State %d Msg State %d", rrc_get_state(wrrc_as_id), rrc_state_from_msg,0);
  switch (rrc_get_state(wrrc_as_id))
  {
  case RRC_STATE_CELL_FACH:
  case RRC_STATE_CELL_DCH:
    if (hspdsch_info_ptr->modeSpecificInfo.t == T_rrc_DL_HSPDSCH_Information_r6_modeSpecificInfo_tdd)
    {
      WRRC_MSG0_ERROR("RRCHS:TDD choice in HSPDSCH");
      status = FAILURE;
    }
    
    break;

  case RRC_STATE_CELL_PCH:
  case RRC_STATE_URA_PCH:
  default:
    WRRC_MSG0_ERROR("RRCHS:Invalid state for HSDPA check");
    status = FAILURE;
    break;
    
  } /* End of switch */
  
  return status;
}

/*===========================================================================

FUNCTION   rrc_update_rab_type_in_rab_info_r6

DESCRIPTION
 This function is updates the rab type in 'rrc_est_rab_info_type' structure
 using information stored in ASN1 'rrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'rrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  rrc_RAB_InformationSetup *: pointer to ASN1 structure,
  rrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

rrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
rrcrb_rab_e_type rrc_update_rab_type_in_rab_info_r6
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RAB_InformationSetup_r6 *rab_info_setup_ptr,
  rrc_est_rab_info_type *est_rab_info_ptr 
)
{
  rrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (rrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = RRCRB_PS_DATA_RAB;
  }
  else if (rrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = rrcrb_translate_gsm_map_rab_id(wrrc_as_id,((rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity) ));

#ifdef FEATURE_DUAL_SIM
    mn_rab_type = rrcmn_verify_rab_subs( (radio_access_bearer_T)rab_id ,rrc_get_as_id(wrrc_as_id));
#else
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
    WRRC_MSG1_HIGH("MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if ( rrclbt_is_lb_on(wrrc_as_id) == TRUE )
    {
      /* Set Rab type to Data */
      WRRC_MSG0_HIGH("RAB setup for Loopback Test");
      est_rab_info_ptr->rab_type = RRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)    
    {
      est_rab_info_ptr->rab_type  =RRCRB_CS_DATA_RAB ;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL) 
    {
      est_rab_info_ptr->rab_type = RRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = RRCRB_UNKNOWN_RAB;
      WRRC_MSG1_ERROR("Invalid cn_domain value: %d", cn_domain);
    }
  }
  else
  {
    WRRC_MSG1_ERROR("Invalid Cn domain %d",cn_domain);
  }

  WRRC_MSG3_MED("Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
    est_rab_info_ptr->rab_type, RRCRB_CS_VOICE_RAB, RRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}


/*===========================================================================

FUNCTION          RRC_TRANSLATE_ERNTI


DESCRIPTION       

  This function translates Primary and Secondary ERNTI type defined by ASN.1 
  into the internal type used by RRC. 

DEPENDENCIES

  Both the internal and external Primary ERNTI pointers must be initialized.
  If either one is NULL, the convertion will fail.

RETURN VALUE
                  
  Success/Failure

SIDE EFFECTS
                  
  None

===========================================================================*/
uecomdef_status_e_type rrc_translate_ernti
(
  sys_modem_as_id_e_type wrrc_as_id, 
  ASN1BitStr32  *ext_ernti,          /* Pointer to external ERNTI type */
  uint16  *int_ernti    /* Pointer to internal ERNTI type */
)
{
  /* Make sure the internal and external pointers are initialized */
  if( ext_ernti == NULL )
  {
    WRRC_MSG0_ERROR("External ERNTI not initialized");
    return FAILURE;
  }
  
  if(ext_ernti->numbits!= 16 )
  {
    WRRC_MSG1_ERROR("Received ERNTI Length: %d is invalid",
               ext_ernti->numbits);
    return FAILURE;
  }

  /* Convert 16 bit ERNTI into internal format */
  *int_ernti = *(ext_ernti->data) << 8;
  *int_ernti |= *(ext_ernti->data+ 1);

  return SUCCESS;
}


/*===========================================================================
FUNCTION   is_ctch_present_in_serving_cell

DESCRIPTION
  This function returns TRUE if CBS DRX info is present on current cell otherwise
  returns false.
 
DEPENDENCIES
  None.
 
RETURN VALUE
  boolean

SIDE EFFECTS
  None.
===========================================================================*/
boolean is_ctch_present_in_serving_cell(  sys_modem_as_id_e_type wrrc_as_id,rrc_state_e_type next_state)
{
  rrc_SysInfoType5    *sib5_ptr = NULL;
  boolean ans = FALSE;      /* Return Value */
  boolean sib5_in_use = TRUE;  /* indicates whether sib 5 or 6 is in use */
  uint32 K = 0;      /* CBS frame offset */
  uint32 N = 0;     /* period of CTCH allocation */
#ifdef FEATURE_DUAL_SIM
  uint16 nv_value = rrc_get_concurrency_nv(wrrc_as_id);
#endif

#ifdef FEATURE_WCDMA_DISABLE_BMC
  /*If we would like to disable BMC based on this feature, simply return FALSE.*/
  WRRC_MSG0_HIGH("Disable BMC.");
  return FALSE;
#endif /*FEATURE_WCDMA_DISABLE_BMC*/
  
#ifdef FEATURE_DUAL_SIM
#ifndef FEATURE_UMTS_BMC_MSIM
  if(rrc_dsds_int_data.wcdma_is_ue_in_dual_standby)
  {
    WRRC_MSG0_HIGH("DSIM: BMC is not supported as DSIM is active");
    return FALSE;
  }
#else
  if(CHECK_CONCURRENCY_FOR_W_BMC(nv_value))
  {
    WRRC_MSG0_HIGH("DSIM: BMC is not supported as concurrency NV is disabled");
    return FALSE;
  }
#endif
#endif
  sib5_ptr = (rrc_SysInfoType5 *)
             rrc_sibdb_return_sib_for_srv_cell(wrrc_as_id, rrc_SIB5);
  if (sib5_ptr == NULL)
  {
    WRRC_MSG0_ERROR("SIB 5 not yet in SIB data base");
    return FALSE;
  }
  
  if (next_state == RRC_STATE_DISCONNECTED || next_state == RRC_STATE_CELL_PCH || next_state == RRC_STATE_URA_PCH)
  {
    /* according to 25.331 if any one of the fach in the sib-5 list has CTCH then cbs_drx 
    must be present */
    WRRC_MSG2_HIGH("CBS/BMC/CTCH presence status in curr cell %d next_state %d", RRCCHECK_SIB_MSG_IE_PRESENT(sib5_ptr,rrc_SysInfoType5_,cbs_DRX_Level1Information), next_state);
    if(RRCCHECK_SIB_MSG_IE_PRESENT(sib5_ptr,rrc_SysInfoType5_,cbs_DRX_Level1Information))
    {
      ans = TRUE;
    }
    else
    {
      ans = FALSE;
    }
  }
  
  /* validate the values of N and K */
  if (ans == TRUE)
  {
      K = sib5_ptr->cbs_DRX_Level1Information.cbs_FrameOffset;
      N = sib5_ptr->cbs_DRX_Level1Information.ctch_AllocationPeriod;
    
    /*CBS frame offset should not be equal or greater than period of CTCH allocation
    M  TTI <= N <= 256 where MTTI is number of radio frames in the TTI of the FACH used for CTCH
    and 0 <= K <= N - 1 */
    if(N == 0 || N > 256 || N <= K)
    {
      WRRC_MSG3_ERROR("Invalid N:%d and K:%d in CBS DRX info. %d",N,K,sib5_in_use);
      ans = FALSE;
    }
  }

  return ans;
}     

/*===========================================================================

FUNCTION rrc_get_lac_id 

DESCRIPTION
  This function returns lac id for the serving cell.

  
DEPENDENCIES
  None

RETURN VALUE
  SUCCESS or FAILURE
  
SIDE EFFECTS

  None

===========================================================================*/
uecomdef_status_e_type rrc_get_lac_id(sys_modem_as_id_e_type wrrc_as_id,rrc_lai_type *lai) 
{

  /* Pointer to SIB 5*/
  rrc_SysInfoType1    *sib1_ptr = NULL;

  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;

  unsigned char  *data_ptr;
  uint32         i;

  sib1_ptr = (rrc_SysInfoType1 *)
      rrc_sibdb_return_sib_for_srv_cell(wrrc_as_id, rrc_SIB1);

  if( sib1_ptr == NULL )
  {
    /* SIB3 for this PLMN, CEll ID combination not present */
    WRRC_MSG0_ERROR("SIB1 not present. Returning");
    return(FAILURE);
  } /* SIB1 not present */


  /* The SIB1 is present */
  nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
  /* Check if the RRC MM interface structure array can hold this info */
  if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    WRRC_MSG1_ERROR("Not enough space for Common NAS Sys Info (%d bytes)",
                        sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
    return(FAILURE);
  }

  /* Fill in the Common NAS system info now */
  data_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
  for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
  {
    nas_common_info.value[i] =  *data_ptr;
    data_ptr ++;
  }

  /* Get the LAI status from local utility */
  /* TBD: Call MM utility to initialize lai first. */
  if(mm_extract_lac_from_sib_info(lai, &nas_common_info) == FALSE)
  {
    WRRC_ERR0_FATAL("Could not extract LAI from NAS Common sys info");
  }

  return (SUCCESS);
 
}


/*==========================================================================

FUNCTION rrc_translate_establishment_cause

DESCRIPTION
  This function gets equivalent external RRC Connection Establishment cause
  for the internal RRC Connection Established cause received from the NAS 
  through RRC_EST_REQ primitive.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  rrc_translate_establishment_cause
(
   sys_modem_as_id_e_type wrrc_as_id, 
   rrc_establish_cause_e_type est_cause_int,
   rrc_EstablishmentCause *est_cause_ext
)
{
   rrc_EstablishmentCause rrc_establishment_cause_ext;
                                      /* RRC Connection Establishment cause
                                         to be sent over the air */
  switch (est_cause_int)
  {
    case RRC_EST_ORIG_CONV_CALL:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_originatingConversationalCall;
      break;
    case RRC_EST_ORIG_STREAM_CALL:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_originatingStreamingCall;
      break;
    case RRC_EST_ORIG_INTERACT_CALL:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_originatingInteractiveCall;
      break;
    case RRC_EST_ORIG_BACKGND_CALL:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_originatingBackgroundCall;
      break;
    case RRC_EST_ORIG_SUBSCRIBED_TRF_CALL:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_originatingSubscribedTrafficCall;
      break;
    case RRC_EST_TERM_CONV_CALL:
      rrc_establishment_cause_ext = 
          rrc_EstablishmentCause_terminatingConversationalCall;
      break;
    case RRC_EST_TERM_STREAM_CALL:
      rrc_establishment_cause_ext = 
          rrc_EstablishmentCause_terminatingStreamingCall;
      break;
    case RRC_EST_TERM_INTERACT_CALL:
      rrc_establishment_cause_ext = 
          rrc_EstablishmentCause_terminatingInteractiveCall;
      break;
    case RRC_EST_TERM_BACKGND_CALL:
      rrc_establishment_cause_ext = 
          rrc_EstablishmentCause_terminatingBackgroundCall;
      break;
    case RRC_EST_EMERGENCY_CALL:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_emergencyCall;
      break;
    case RRC_EST_INTER_RAT_CELL_RESELECT:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_interRAT_CellReselection;
      break;
    case RRC_EST_INTER_RAT_CELL_CHANGE_ORDER:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_interRAT_CellChangeOrder;
      break;
    case RRC_EST_REGISTRATION:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_registration;
      break;
    case RRC_EST_DETACH:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_detach;
      break;
    case RRC_EST_HI_PRI_SIGNALLING:
      rrc_establishment_cause_ext = 
        rrc_EstablishmentCause_originatingHighPrioritySignalling;
      break;
    case RRC_EST_LOW_PRI_SIGNALLING:
      rrc_establishment_cause_ext = 
        rrc_EstablishmentCause_originatingLowPrioritySignalling;
      break;
    case RRC_EST_CALL_RE_ESTABLISH:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_callRe_establishment;
      break;
    case RRC_EST_TERM_HI_SIGNALLING:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_terminatingHighPrioritySignalling;
      break;
    case RRC_EST_TERM_LOW_SIGNALLING:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_terminatingLowPrioritySignalling;
      break;
    case RRC_EST_TERM_UNKNOWN:
      rrc_establishment_cause_ext = rrc_EstablishmentCause_terminatingCauseUnknown;
      break;
    default:
      /* rrc_establishment_cause_ext = rrc_establishment_cause; */
      /* The above was changed to stop Lint from complaining. The following would never get executed */
      rrc_establishment_cause_ext = rrc_EstablishmentCause_spare1;
      WRRC_MSG1_ERROR("Invalid RRC Conn Est cause: %d", rrcdata_misc_info.rrc_establishment_cause);
      break;
  }

  WRRC_MSG2_MED("mm cause %d, translated cause %d",est_cause_int,rrc_establishment_cause_ext);
  *est_cause_ext = rrc_establishment_cause_ext;
  return;

} /* rrc_translate_establishment_cause */




/*====================================================================
FUNCTION: rrc_translate_n313

DESCRIPTION:
  This function translates the enum for timer N313 into an int16 value.

DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
uint16 rrc_translate_n313(sys_modem_as_id_e_type wrrc_as_id,rrc_N_313 val)
{
  switch (val)
  {
    case rrc_N_313_s1:
      return 1;
    case rrc_N_313_s2:
      return 2;
    case rrc_N_313_s4:
      return 4;
    case rrc_N_313_s10:
      return 10;
    case rrc_N_313_s20:
      return 20;
    case rrc_N_313_s50:
      return 50;
    case rrc_N_313_s100:
      return 100;
    case rrc_N_313_s200:
      return 200;
    default:
      WRRC_MSG1_ERROR("Invalid N313 = %d", val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
} /* rrc_translate_n313 */


/*====================================================================
FUNCTION: rrc_get_downlink_rab_rate

DESCRIPTION:
  This function returns the downlink rate of the RAB passed in

DEPENDENCIES:
  None

RETURN VALUE:
  uint32

SIDE EFFECTS:
====================================================================*/
uint32 rrc_get_downlink_rb_rate(sys_modem_as_id_e_type wrrc_as_id,uint8 lc_id)
{

  uint32 rate = 0;
  uint8 rb_id = 0;

  rb_id = rrclcm_get_dl_rb_id(wrrc_as_id,lc_id);

  if (rb_id == RRCLCM_RB_ID_NOT_ALLOCATED) 
  {
    WRRC_MSG1_ERROR("Wrong LC_ID passed in %d",lc_id);
  }
  else
  {
    rate = rrcllc_get_dl_ded_rate(wrrc_as_id,rrc_get_state(wrrc_as_id), rb_id);
    if(rate == 0)
    {      
      WRRC_MSG1_ERROR("downlink rate for rb_id %d is set to 384000", rb_id);
      rate = 384000;
    }
  }

  WRRC_MSG2_HIGH("downlink rate for rb_id %d is %d", rb_id, rate);
  return rate;
}

/*====================================================================
FUNCTION: rrc_get_uplink_rab_rate

DESCRIPTION:
  This function returns the uplink rate of the RAB passed in

DEPENDENCIES:
  None

RETURN VALUE:
  uint32

SIDE EFFECTS:
====================================================================*/
uint32 rrc_get_uplink_rb_rate(sys_modem_as_id_e_type wrrc_as_id,uint8 lc_id)
{
  uint32 rate = 0;
  uint8 rb_id = 0;

  rb_id = rrclcm_get_ul_rb_id(wrrc_as_id,lc_id);

  if (rb_id == RRCLCM_RB_ID_NOT_ALLOCATED) 
  {
    WRRC_MSG1_ERROR("Wrong LC_ID passed in %d",lc_id);
  }
  else
  {
    rate = rrcllc_get_ul_ded_rate(wrrc_as_id,rrc_get_state(wrrc_as_id),  rb_id);
    if(rate == 0)
    {      
      WRRC_MSG1_ERROR("uplink rate for rb_id %d is set to 384000", rb_id);
      rate = 384000;
    }
  }

  WRRC_MSG2_HIGH("uplink rate for rb_id %d is %d", rb_id, rate);
  return rate;
}

/*====================================================================
FUNCTION: rrc_is_wcdma_active_sub

DESCRIPTION:
  This function returns true of W is active, meaning MCM rrc_mode
  is  returns FALSE otherwise

DEPENDENCIES:
  None

RETURN VALUE:
  uint32

SIDE EFFECTS:
====================================================================*/
boolean rrc_is_wcdma_active_sub(sys_modem_as_id_e_type wrrc_as_id)
{
  rrc_mode_e_type mcm_rrc_mode = rrcmcm_get_rrc_mode(wrrc_as_id);

  if (mcm_rrc_mode == RRC_MODE_ACTIVE) 
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*====================================================================
FUNCTION: is_wcdma_active

DESCRIPTION:
  This function returns true of W is active, meaning MCM rrc_mode
  is  returns FALSE otherwise

DEPENDENCIES:
  None

RETURN VALUE:
  uint32

SIDE EFFECTS:
====================================================================*/
boolean rrc_is_wcdma_active(void)
{
  return (rrc_is_wcdma_active_sub(SYS_MODEM_AS_ID_1));
}


/*====================================================================
FUNCTION: rrc_update_constants_n312_and_n315_r5

DESCRIPTION:
  This function updates the global constants for N312 & N315 for R5.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
uint16 rrc_update_constants_n312_and_n315_r5
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_UE_ConnTimersAndConstants_r5 *conn_timers
)
{
  if(conn_timers != NULL)
  {
    if(RRCCHECK_TIMER_CONST_PRESENT_R5(conn_timers,rrc_UE_ConnTimersAndConstants_r5_,t_301))
    { 
      switch (conn_timers->n_312)
      { 
      case rrc_N_312_r5_s1:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  1;
        break;
      case rrc_N_312_r5_s2:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  2;
        break;
      case rrc_N_312_r5_s4:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  4;
        break;
      case rrc_N_312_r5_s10:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  10;
        break;
      case rrc_N_312_r5_s20:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  20;
        break;
      case rrc_N_312_r5_s50:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  50;
        break;
      case rrc_N_312_r5_s100:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  100;
        break;
      case rrc_N_312_r5_s200:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  200;
        break;
      case rrc_N_312_r5_s400:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  400;
        break;
      case rrc_N_312_r5_s600:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  600;
        break;
      case rrc_N_312_r5_s800:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  800;
        break;
      case rrc_N_312_r5_s1000:
        rrcdata_timers_counters_info.conn_timers_and_constants.n312 =  1000;
        break;
      default:
        WRRC_MSG1_ERROR("Invalid N312 = %d", conn_timers->n_312);
        return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
      }  
    } 

    if(RRCCHECK_TIMER_CONST_PRESENT_R5(conn_timers,rrc_UE_ConnTimersAndConstants_r5_,n_315))
    { 
      switch(conn_timers->n_315)
      { 
      case rrc_N_315_r5_s1:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 1;
        break;
      case rrc_N_315_r5_s2:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315=  2;
        break;
      case rrc_N_315_r5_s4:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 4;
        break;
      case rrc_N_315_r5_s10:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 10;
        break;
      case rrc_N_315_r5_s20:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 20;
        break;
      case rrc_N_315_r5_s50:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 50;
        break;
      case rrc_N_315_r5_s100:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 100;
        break;
      case rrc_N_315_r5_s200:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 200;
        break;
      case rrc_N_315_r5_s400:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 400;
        break;
      case rrc_N_315_r5_s600:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 600;
        break;
      case rrc_N_315_r5_s800:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 800;
        break;
      case rrc_N_315_r5_s1000:
        rrcdata_timers_counters_info.conn_timers_and_constants.n315 = 1000;
        break;
      default:
        WRRC_MSG1_ERROR("Invalid N315 = %d", conn_timers->n_315);
        return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
      } 
    } 
  } 

  return 0;
} /* rrc_update_constants_n312_and_n315_r5 */

/*===========================================================================

FUNCTION rrc_print_wcdma_rrc_version_vn

DESCRIPTION
  This  function prints the ASN1 release version supported by the UE as per 
  NV item "NV_WCDMA_RRC_VERSION_I" i.e. global variable 'rrc_nv_rel_indicator'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_print_supported_asn1_rel_version(sys_modem_as_id_e_type wrrc_as_id)
{
  /* since ASN1 is back-ward compatible, hence if Rel-6 is set then it indicates that 
     Rel-5 and R99 is automatically supported */
  WRRC_MSG3_HIGH("NV rrcVersion (value: %d) indicates rrcASN1 Rel-6: %d, Rel-5: %d and R99 support",
    rrc_nv_rel_indicator,
    (rrc_nv_rel_indicator&RRC_NV_REL_INDICATOR_REL6)>>1,
    (rrc_nv_rel_indicator&RRC_NV_REL_INDICATOR_REL5));
}
/*===========================================================================

FUNCTION wcdma_rrc_get_3gpp_release_version_sub

DESCRIPTION
This function returns the wcdma release version.

DEPENDENCIES
  None.

RETURN VALUE
For REL99 WCDMA_RELEASE_99
For REL5  WCDMA_RELEASE_5
For REL6  WCDMA_RELEASE_6
For REL7  WCDMA_RELEASE_7
For REL8  WCDMA_RELEASE_8
For REL9  WCDMA_RELEASE_9
For REL10 WCDMA_RELEASE_10
For REL11 WCDMA_RELEASE_11
for REL12 WCDMA_RELEASE_12
===========================================================================*/

wcdma_rrc_release_version_e_type wcdma_rrc_get_3gpp_release_version_sub(sys_modem_as_id_e_type wrrc_as_id)
{
  if(rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL10)
  {
    return WCDMA_RELEASE_10;
  }
  else if(rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL9)
  {
    return WCDMA_RELEASE_9;
  }
  else if(rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL8)
  {
    return WCDMA_RELEASE_8;
  }
  else if(rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL7)
  {
    return WCDMA_RELEASE_7;
  }
  else if(rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL6)
  {
    return WCDMA_RELEASE_6;
  }
  else if(rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL5)
  {
    return WCDMA_RELEASE_5;
  }
  else if(rrc_nv_rel_indicator & RRC_NV_REL_INDICATOR_REL99)
  {
    return WCDMA_RELEASE_99;
  }
  else
  {
    return WCDMA_RELEASE_99;
  }
}
/*===========================================================================

FUNCTION rrc_return_wcdma_rrc_version_sub

DESCRIPTION
This function returns the wcdma rrc version to gps.

DEPENDENCIES
  None.

RETURN VALUE
For REL99 we return 0
For REL5 we return  1
For REL6 we return  3
For REL7 we retrun  7
For REL8 we return 15
===========================================================================*/
uint8 rrc_return_wcdma_rrc_version_sub(sys_modem_as_id_e_type wrrc_as_id)
{
  RRC_VALIDATE_SUB_ID(wrrc_as_id);
  return rrc_nv_rel_indicator;
}

/*===========================================================================

FUNCTION rrc_return_wcdma_rrc_version

DESCRIPTION
This function returns the wcdma rrc version to gps.

DEPENDENCIES
  None.

RETURN VALUE
For REL99 we return 0
For REL5 we return  1
For REL6 we return  3
For REL7 we retrun  7
For REL8 we return 15
===========================================================================*/
uint8 rrc_return_wcdma_rrc_version(void)
{
  /*RRC_VALIDATE_FUNC_CALL();*/
  return rrc_return_wcdma_rrc_version_sub(SYS_MODEM_AS_ID_1);
}

/*===========================================================================

FUNCTION rrc_get_wcdma_rrc_version_nv

DESCRIPTION
  This  function gets the NV item NV_WCDMA_RRC_VERSION_I and stores it in 
  rrc global variable 'rrc_nv_rel_indicator'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_get_wcdma_rrc_version_nv
(
  sys_modem_as_id_e_type wrrc_as_id
)
{

  uint8 local_rrc_rel_indicator = 0;

/*Initialize rrc_nv_rel_indicator as per the highest release feature defined. 
It will be overwritten if RRC version is read from NV. So basically it will be used by UTF.*/
#ifdef FEATURE_WCDMA_REL10
    rrc_nv_rel_indicator =
  RRC_NV_REL_INDICATOR_REL5|RRC_NV_REL_INDICATOR_REL6|RRC_NV_REL_INDICATOR_REL7|
  RRC_NV_REL_INDICATOR_REL8|RRC_NV_REL_INDICATOR_REL9|RRC_NV_REL_INDICATOR_REL10;
#elif defined(FEATURE_WCDMA_REL9)
    rrc_nv_rel_indicator =
  RRC_NV_REL_INDICATOR_REL5|RRC_NV_REL_INDICATOR_REL6|RRC_NV_REL_INDICATOR_REL7|
  RRC_NV_REL_INDICATOR_REL8|RRC_NV_REL_INDICATOR_REL9;
#else
    rrc_nv_rel_indicator =
  RRC_NV_REL_INDICATOR_REL5|RRC_NV_REL_INDICATOR_REL6|RRC_NV_REL_INDICATOR_REL7|
  RRC_NV_REL_INDICATOR_REL8;
#endif /* FEATURE_WCDMA_REL9 */
  /*read wcdma rrc release indicator from NV*/
  wnv_get_nv_msim(WNV_ID_RRC_VERSION_I_FOR_RRC, &local_rrc_rel_indicator, sizeof(local_rrc_rel_indicator), wrrc_as_id);
  WRRC_MSG1_HIGH("NV_WCDMA_RRC_VERSION_I(nv item number 3649) is %d", local_rrc_rel_indicator);
  
  WRRC_MSG1_HIGH("Set rrc_nv_rel_indicator to %d [0-R99, 1-R5, 2-R6, 3-R7, 4-R8, 5-R9, 6-R10]", local_rrc_rel_indicator);
  /* use stored nv variable value to set rrc global variable */
  switch (local_rrc_rel_indicator)
  {
    case 0:
      rrc_nv_rel_indicator = RRC_NV_REL_INDICATOR_REL99;
      break;
      
    case 1:
      rrc_nv_rel_indicator = RRC_NV_REL_INDICATOR_REL99|RRC_NV_REL_INDICATOR_REL5;
      break;
      
    case 2:
      rrc_nv_rel_indicator = RRC_NV_REL_INDICATOR_REL99|RRC_NV_REL_INDICATOR_REL5|RRC_NV_REL_INDICATOR_REL6;
      break;

    case 3:
      rrc_nv_rel_indicator = RRC_NV_REL_INDICATOR_REL99|RRC_NV_REL_INDICATOR_REL5|RRC_NV_REL_INDICATOR_REL6|RRC_NV_REL_INDICATOR_REL7;
      break;

    case 4:
      rrc_nv_rel_indicator = RRC_NV_REL_INDICATOR_REL99|RRC_NV_REL_INDICATOR_REL5|RRC_NV_REL_INDICATOR_REL6|RRC_NV_REL_INDICATOR_REL7|RRC_NV_REL_INDICATOR_REL8;
      break;
#ifdef FEATURE_WCDMA_REL9
    case 5:
      rrc_nv_rel_indicator = RRC_NV_REL_INDICATOR_REL99|RRC_NV_REL_INDICATOR_REL5|RRC_NV_REL_INDICATOR_REL6|RRC_NV_REL_INDICATOR_REL7|RRC_NV_REL_INDICATOR_REL8|RRC_NV_REL_INDICATOR_REL9;
      break;
#endif /* FEATURE_WCDMA_REL9 */
#ifdef FEATURE_WCDMA_REL10
    case 6:
      rrc_nv_rel_indicator = RRC_NV_REL_INDICATOR_REL99|RRC_NV_REL_INDICATOR_REL5|RRC_NV_REL_INDICATOR_REL6|RRC_NV_REL_INDICATOR_REL7|RRC_NV_REL_INDICATOR_REL8|RRC_NV_REL_INDICATOR_REL9|RRC_NV_REL_INDICATOR_REL10;
      break;
#endif /* FEATURE_WCDMA_REL10 */
    default:
      break;
  }
}

#ifdef FEATURE_UMTS_PDCP
/*===========================================================================

FUNCTION rrc_get_wcdma_rrc_pdcp_disabled_nv

DESCRIPTION
  This  function gets the NV item NV_WCDMA_RRC_PDCP_DISABLED_I and stores it in 
  rrc global variable 'rrc_pdcp_enabled'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void rrc_get_wcdma_rrc_pdcp_disabled_nv
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  boolean local_rrc_pdcp_disabled_ind;
  rrc_cap_related_info.rrc_pdcp_enabled = FALSE;

  /*read wcdma rrc release indicator from NV*/
  wnv_get_nv_msim(WNV_ID_WCDMA_RRC_PDCP_DISABLED_I, &local_rrc_pdcp_disabled_ind, sizeof(local_rrc_pdcp_disabled_ind), wrrc_as_id);
  WRRC_MSG1_HIGH("NV_WCDMA_RRC_PDCP_DISABLED_I(nv item number 4173) is %d", local_rrc_pdcp_disabled_ind);
  
  /* use stored nv variable value to set rrc global variable */
  if (!local_rrc_pdcp_disabled_ind)
  {
    rrc_cap_related_info.rrc_pdcp_enabled = TRUE;
  }
  
  WRRC_MSG1_HIGH("Set rrc_pdcp_enabled to %d", rrc_cap_related_info.rrc_pdcp_enabled);

}
#endif /* FEATURE_UMTS_PDCP*/

/*===========================================================================

FUNCTION rrc_get_amr_mode_bitrate

DESCRIPTION
  This  function takes amr mode as argument & returns the AMR rate category

DEPENDENCIES
  None.

RETURN VALUE
  rrc_amr_mode_bitrate_e_type.

===========================================================================*/
rrc_amr_mode_bitrate_e_type rrc_get_amr_mode_bitrate(sys_modem_as_id_e_type wrrc_as_id,uint32 amr_mode)
{
  rrc_amr_mode_bitrate_e_type  amr_bitrate = RRC_AMR_MODE_MAX;

  switch(amr_mode)
  {
  case WCDMA_IVOCAMR_CODEC_MODE_0475:
  case WCDMA_IVOCAMR_CODEC_MODE_0515:
  case WCDMA_IVOCAMR_CODEC_MODE_0590:
  case WCDMA_IVOCAMR_CODEC_MODE_0670:
  case WCDMA_IVOCAMR_CODEC_MODE_0740:
  case WCDMA_IVOCAMR_CODEC_MODE_0795:
    amr_bitrate = RRC_AMR_MODE_LOW_BITRATE;
    break;

  case WCDMA_IVOCAMR_CODEC_MODE_1020:
  case WCDMA_IVOCAMR_CODEC_MODE_1220:
    amr_bitrate = RRC_AMR_MODE_HIGH_BITRATE;
    break;

  default:
    break;
  }

  return amr_bitrate;
} /* rrc_get_amr_mode_bitrate */

/*===========================================================================

FUNCTION rrc_get_dl_amr_mode_given_rbs

DESCRIPTION
  This  function returns that if the mode has changed calculating rates 
  corresponding RB Ids.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

===========================================================================*/
boolean rrc_get_dl_amr_mode_given_rbs
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32 *prev_mode, 
  uint32 *new_mode,
  rrc_rb_info_for_amr_type *amr_rb_info_ptr
)
{
  boolean mode_changed = FALSE;
  uint32 i = 0;
  uint32 amr_mode = WCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;

  /* Stored RAB info should point to CS RAB to continue */
  if(amr_rb_info_ptr->rab_type == RRCRB_CS_VOICE_RAB)
  {
    /* Get previous rate from current configuration */
    for(i =0; i < amr_rb_info_ptr->num_rbs_for_rab; i++)
    {
      /* Any meaningful AMR mode corresponding to Voice RAB (RB) is 
         implicitly treated as correct rate for the other RBs */
      if((amr_mode = rrcllc_get_dl_amr_mode_for_rb(wrrc_as_id,amr_rb_info_ptr->rb_id[i], current_config_ptr)) != WCDMA_IVOCAMRWB_CODEC_MODE_UNDEF)
      {
        *prev_mode = amr_mode;
        break;       
      }
    }

    /* Get new rate from ordered configuration */
    for(i =0; i < amr_rb_info_ptr->num_rbs_for_rab; i++)
    {
      /* Any meaningful AMR mode corresponding to Voice RAB (RB) is 
         implicitly treated as correct rate for the other RBs */
      if((amr_mode = rrcllc_get_dl_amr_mode_for_rb(wrrc_as_id,amr_rb_info_ptr->rb_id[i], ordered_config_ptr)) != WCDMA_IVOCAMRWB_CODEC_MODE_UNDEF)
      {
        *new_mode = amr_mode;
        break;       
      }
    }

    if(*prev_mode != *new_mode)
    {
      mode_changed = TRUE;
    }
  }
  else
  {
    WRRC_MSG0_ERROR("This function shouldn't have been called w/o CS RAB");
  }
  
  return mode_changed;
} /* rrc_get_dl_amr_mode_given_rbs */

/*===========================================================================

FUNCTION   rrc_find_any_rab_already_established_rab_type

DESCRIPTION

  This function checks any RAB is already present in
  "ESTABLISHED_RABS" variable
  If so, it returns TRUE.
  It will also return the Rab Type and the CN Domain used by the 
   established Rab
  Else it returns FALSE.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  boolean. TRUE: If a RAB already exists
     else returns FALSE

SIDE EFFECTS

  None.

===========================================================================*/
boolean 
rrc_find_any_rab_already_established_rab_type
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_cn_domain_identity_e_type *rab_domain,
  rrcrb_rab_e_type *rab_type
)
{
  uint32 rab_count;
  for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
  {
    if ((rrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) &&
        (rrc_est_rabs.rabs[rab_count].rab_id !=  RRC_INVALID_RAB_ID))
    {
      *rab_type = rrc_est_rabs.rabs[rab_count].rab_type;
      *rab_domain = rrc_est_rabs.rabs[rab_count].cn_domain;
       return TRUE;
    }
  }
  *rab_type = RRCRB_UNKNOWN_RAB;
  return FALSE;
}


/*===========================================================================

FUNCTION rrc_check_if_wcdma_can_be_stopped

DESCRIPTION
  This  function is called by L1 to check if WCDMA stack can be stopped ie PNR
  (Point of no return) has not been reached. If PNR has not been reached,
  the function calls  cm function to post a special stop to RRC and returns
  TRUE to L1. 

  If PNR has been reached the function returns FALSE.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

===========================================================================*/
boolean rrc_check_if_wcdma_can_be_stopped
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  if (!(rrcdata_misc_info.rrc_multimode_pt_of_no_return_reached))
  {
    WRRC_MSG0_HIGH("PNR not reached stopping WCDMA stack");
    /* for 7200 only builds that use TRM need to add ERR_FATAL if rf resources 
     * are not granted to L1 */
#ifdef FEATURE_UMTS_1X_HANDOVER_UMTSMSM     
    #error code not present
#endif    
    return TRUE;
  }

  WRRC_MSG0_HIGH("PNR reached continue as normal");
  return FALSE;
}

#ifdef WCDMA_RRC_INDICATE_CODEC_TO_CM

/*===========================================================================

FUNCTION rrc_get_ordered_codec_status

DESCRIPTION
  This  function returns that the ordred codec mode status

DEPENDENCIES
  None.

RETURN VALUE
  ordered codec status

===========================================================================*/
boolean rrc_get_ordered_codec_status
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  WRRC_MSG1_MED(" The Ordered codec status is returned is %d",rrcdata_codec_info.ordered_codec_valid);
  return rrcdata_codec_info.ordered_codec_valid;
}


/*===========================================================================

FUNCTION rrc_set_ordered_codec_status

DESCRIPTION
  This  function sets the ordered codec status
DEPENDENCIES
  None.

RETURN VALUE
 None

===========================================================================*/
void rrc_set_ordered_codec_status(sys_modem_as_id_e_type wrrc_as_id,boolean flag)
{
   if((rrcdata_codec_info.ordered_codec_valid != flag) || (flag  == TRUE))
   {
     WRRC_MSG1_HIGH(" The Ordered codec status is set as %d",flag);
   }
   rrcdata_codec_info.ordered_codec_valid = flag;   
}
#endif
/*===========================================================================

FUNCTION rrc_get_current_codec_mode

DESCRIPTION
  This  function returns that the Current codec mode type that is set 

DEPENDENCIES
  None.

RETURN VALUE
  uint32: rteurns the  Current codec mode type that is set 

===========================================================================*/
#ifdef FEATURE_WCDMA_QSH_DUMP
  __attribute__((section(".uncompressible.text")))
#endif
uint32 rrc_get_current_codec_mode
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_codec_info.rrc_active_codec;
}


/*===========================================================================

FUNCTION rrc_set_current_codec_mode

DESCRIPTION
  This  function sets the current codec mode with value configured in MVS
DEPENDENCIES
  None.

RETURN VALUE
 None

===========================================================================*/
void rrc_set_current_codec_mode
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32 nas_sync_indicator
)
{
   rrcdata_codec_info.rrc_active_codec = nas_sync_indicator;   
   WRRC_MSG1_HIGH(" The current codec mode is set as 0x%x",rrcdata_codec_info.rrc_active_codec);
}

/*===========================================================================

FUNCTION  rrc_translate_nas_sync_indicator

DESCRIPTION This function translates a Nas-Syncronisation-indicator defined by
 ASN.1 into the internal type used by RRC. 

DEPENDENCIES

  The algorithm used assumes that the processor is Little Endian.
  
RETURN VALUE
                  
  Nas-Syncronisation-indicator converted to the internal type rrc_codec_types


===========================================================================*/
uint32 rrc_translate_nas_sync_indicator
(
  sys_modem_as_id_e_type wrrc_as_id, 
  ASN1BitStr32  *nas_sync_indicator         /* Pointer to external PLMN type */
)
{
  rrc_codec_types codec_type = RRC_UMTS_AMR;           
 
  if(nas_sync_indicator->numbits> 4 )
  {
    WRRC_MSG0_ERROR("nas_sync_indicator longer than 4 bits!");
    return( WCDMA_IVOCODER_ID_NONE);
  }

  codec_type = (rrc_codec_types)((nas_sync_indicator->data[0] >> nas_sync_indicator->numbits)& 0x0F);
  WRRC_MSG1_MED(" Nas sync indicator translated %d ",codec_type);
  if((codec_type == RRC_UMTS_AMR) || (codec_type == RRC_UMTS_AMR_2))
  {
    return(WCDMA_IVOCODER_ID_AMR);
  }
  else if(codec_type == RRC_UMTS_AMR_WB)
  {
    return(WCDMA_IVOCODER_ID_AMRWB);
  }
  else
  {
    WRRC_MSG1_ERROR(" Unknown type of codec %d",nas_sync_indicator);
    return WCDMA_IVOCODER_ID_NONE;
  }
}

/*===========================================================================

FUNCTION  rrc_set_ordered_codec_mode

DESCRIPTION This function sets the codec mode ordered in the ota messages

DEPENDENCIES
  None
  
RETURN VALUE          
  None

===========================================================================*/
void rrc_set_ordered_codec_mode
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint32 nas_sync_ind
)
{
  rrcdata_codec_info.rrc_ota_codec_mode_status = nas_sync_ind;
}

/*===========================================================================

FUNCTION  rrc_get_ordered_codec_mode

DESCRIPTION This function sets the codec mode ordered in the ota messages

DEPENDENCIES
  None
  
RETURN VALUE          
  None

===========================================================================*/
uint32 rrc_get_ordered_codec_mode
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_codec_info.rrc_ota_codec_mode_status;
}

/*===========================================================================

FUNCTION  rrc_debug_stats

DESCRIPTION This function increments the debug statistics

DEPENDENCIES
  None
  
RETURN VALUE
  None

===========================================================================*/
void rrc_debug_stats
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_debug_stats_enum_type rrc_debug_cause
)
{
  switch(rrc_debug_cause)
  {
    case INTRA_FREQ_CEL_RESEL_SUCCESS:
      rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_resel_success_cnt++;
    break;
    
    case INTRA_FREQ_CEL_RESEL_NBCCH_SETUP_FAILURE:
      rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt++;
    break;
    
    case INTRA_FREQ_CEL_RESEL_SIB_WAIT_TIMEOUT:
      rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt++;
    break;
    
    case INTRA_FREQ_CEL_RESEL_TRANSITION_FAILURE:
      rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_transition_fail_cnt++;
    break;
    
    case INTRA_FREQ_CEL_RESEL_FAILED_OTHER_CAUSE:
      rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_failed_other_causes_cnt++;
    break;
    
    case WTOG_CEL_RESEL_TRIGGERED:
      rrcdata_logging_info.rrc_debug_wtog_resel_stats.wtog_resel_cnt++;
    break;
    
    case WTOG_CEL_RESEL_FAILURE:
      rrcdata_logging_info.rrc_debug_wtog_resel_stats.wtog_resel_failure_cnt++;
    break;
    
    case OOS_IN_IDLE_MODE:
      rrcdata_logging_info.rrc_debug_oos_stats.oos_idle_cnt++;
    break;
    
    case OOS_IN_CONNECTED_MODE:
      rrcdata_logging_info.rrc_debug_oos_stats.oos_connected_cnt++;
    break;
    
    case RACH_SUCCFUL_RRC_RETRY:
      rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.succful_rrc_tran_retry_cnt++;
    break;
    
    case RACH_RRC_RETRY_FAIL_DUE_TO_CMAC_STATUS_IND:
      rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_cmac_status_ind_cnt++;
      break;
      
    case RACH_RRC_RETRY_FAIL_DUE_TO_OTHER_CAUSE:
      rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_others_cnt++;
      break;
      
    default:
      WRRC_MSG1_HIGH("ERR: Invalid type :%d ",rrc_debug_cause);
  }
}

/*===========================================================================

FUNCTION  rrc_debug_clear_stats

DESCRIPTION This function clears all the debug statistics

DEPENDENCIES
  None
  
RETURN VALUE
  None

===========================================================================*/
void rrc_debug_clear_stats
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_resel_success_cnt = 0;
  rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt = 0;
  rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt = 0;
  rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_transition_fail_cnt = 0;
  rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_failed_other_causes_cnt = 0;
  rrcdata_logging_info.rrc_debug_wtog_resel_stats.wtog_resel_cnt = 0;
  rrcdata_logging_info.rrc_debug_wtog_resel_stats.wtog_resel_failure_cnt = 0;
  rrcdata_logging_info.rrc_debug_oos_stats.oos_idle_cnt= 0;
  rrcdata_logging_info.rrc_debug_oos_stats.oos_connected_cnt= 0;
  rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.succful_rrc_tran_retry_cnt = 0;
  rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_cmac_status_ind_cnt= 0;
  rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_others_cnt= 0;
}

/*===========================================================================

FUNCTION  rrc_debug_get_cel_resel_success_cnt

DESCRIPTION   This function gives the intra frequency cell reselection success count

DEPENDENCIES
  None
  
RETURN VALUE  Intra frequency reselection success count since last power up

===========================================================================*/
uint32 rrc_debug_get_cel_resel_success_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_resel_success_cnt;
}

/*===========================================================================

FUNCTION  rrc_debug_get_cel_resel_nbcch_set_up_fail_cnt

DESCRIPTION  This function gives the intra frequency cell reselection fail count due to 
 N-BCCH set up failure.

DEPENDENCIES
  None
  
RETURN  Intra frequency reselection failure count due to N-BCCH set up failue since last power up

===========================================================================*/
uint32 rrc_debug_get_cel_resel_nbcch_set_up_fail_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt;
}

/*===========================================================================

FUNCTION  rrc_debug_get_cel_resel_sib_wait_timeout_cnt

DESCRIPTION  This function gives the intra frequency cell reselection fail count due to 
 SIB wait time out.

DEPENDENCIES
  None
  
RETURN   Intra frequency reselection failure count due to SIB wait time out since last power up

===========================================================================*/
uint32 rrc_debug_get_cel_resel_sib_wait_timeout_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt;
}

/*===========================================================================

FUNCTION  rrc_debug_get_cel_resel_transition_fail_cnt

DESCRIPTION  This function gives the intra frequency cell reselection fail count due to 
 transition failure(s-criteria failure).

DEPENDENCIES
  None
  
RETURN  Intra frequency reselection failure count due to transition failures since last power up 

===========================================================================*/
uint32 rrc_debug_get_cel_resel_transition_fail_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_transition_fail_cnt;
}

/*===========================================================================

FUNCTION  rrc_debug_get_cel_resel_failed_other_causes_cnt

DESCRIPTION  This function gives the intra frequency cell reselection fail count due to 
 the causes other than n-bcch setup failure, sib wait time out and transition failure.

DEPENDENCIES
  None
  
RETURN  Intra frequency reselection failure count due to other causes since last power up

===========================================================================*/
uint32 rrc_debug_get_cel_resel_failed_other_causes_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_intra_freq_resel_stats.intra_freq_failed_other_causes_cnt;
}

/*===========================================================================

FUNCTION  rrc_debug_get_wtog_cel_resel_success_cnt

DESCRIPTION  This function gives the number of times the WTOG cell reselection is triggered 

DEPENDENCIES
  None
  
RETURN  WTOG cell reselection count since last power up

===========================================================================*/
uint32 rrc_debug_get_wtog_cel_resel_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return (rrcdata_logging_info.rrc_debug_wtog_resel_stats.wtog_resel_cnt);
}

/*===========================================================================

FUNCTION  rrc_debug_get_wtog_cel_resel_fail_cnt

DESCRIPTION  This function gives the WTOG cell reselection fail count.

DEPENDENCIES
  None
  
RETURN  WTOG cell reselection failure count since last power up

===========================================================================*/
uint32 rrc_debug_get_wtog_cel_resel_fail_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_wtog_resel_stats.wtog_resel_failure_cnt;
}

/*===========================================================================

FUNCTION  rrc_debug_get_oos_idle_mode_cnt

DESCRIPTION  This function gives how many times the OOS is triggered in idle mode.

DEPENDENCIES
  None
  
RETURN  Number of times OOS is triggered in idle mode since last power up

===========================================================================*/
uint32 rrc_debug_get_oos_idle_mode_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_oos_stats.oos_idle_cnt;
}

/*===========================================================================

FUNCTION  rrc_debug_get_oos_connected_mode_cnt

DESCRIPTION This function gives how many times the OOS is triggered in connected mode.

DEPENDENCIES
  None
  
RETURN  Number of times OOS is triggered in connected mode since last power up

===========================================================================*/
uint32 rrc_debug_get_oos_connected_mode_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_oos_stats.oos_connected_cnt;
}
/*===========================================================================

FUNCTION  rrc_debug_get_succful_rrc_trans_retry_cnt

DESCRIPTION This function gives counter value which is  incremented 
when RRC is successful in receiving RRC procedure level response
message from N/W for any RRC message sent on common channel i.e. RACH. 
.
DEPENDENCIES
  None
  
RETURN  Number of times RRC is successful in receiving RRC procedure level response
message from N/W since last power up

===========================================================================*/
uint32 rrc_debug_get_succful_rrc_trans_retry_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.succful_rrc_tran_retry_cnt;
}
/*===========================================================================

FUNCTION  rrc_debug_get_rrc_retry_fail_due_to_l2_ack_ind_cnt

DESCRIPTION This function gives counter value which is incremented 
when MAC hits max re-try while RACHing & sends CMAC_STATUS_IND
with cause as hit MAX re-try.
.
DEPENDENCIES
  None
  
RETURN  Number of times max RACH transmission hit by MAC.
===========================================================================*/
uint32 rrc_debug_get_rrc_retry_fail_due_to_l2_ack_ind_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_cmac_status_ind_cnt;
}
/*===========================================================================

FUNCTION  rrc_debug_get_rrc_retry_fail_due_to_others_cnt

DESCRIPTION This function gives counter value which is incremented 
when RRC retransmit message Bcoz of receiving L2 ACK IND with cause
failure for previously attempted transmission of RRC message on RACH 
.
DEPENDENCIES
  None
  
RETURN   Number of times RRC transmission or retry fail due to L2 ACK IND.
===========================================================================*/
uint32 rrc_debug_get_rrc_retry_fail_due_to_others_cnt
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return rrcdata_logging_info.rrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_others_cnt;
}

/*===========================================================================

FUNCTION  rrc_extract_rac_from_sib_info

DESCRIPTION This function extracts RAC from NAS CN Domain System Information List.

DEPENDENCIES
  None
  
RETURN VALUE          
  TRUE: RAC is found
  FALSE: RAC is not found

===========================================================================*/
boolean  rrc_extract_rac_from_sib_info
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_rai_type *rai,
  rrc_CN_DomainSysInfoList  *cn_domain_sys_info_list_ptr
)
{
  //Extract RAC from NAS CN domain specific info in SIB1
  uint32 num_cn_domains, index=0, n_value=0;
  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_domain_spec_info;
  boolean rac_found = FALSE;

  /* Fill in the CN domain specific NAS system info now */
  num_cn_domains = 0;
  n_value = cn_domain_sys_info_list_ptr->n;
  if(n_value == 0)
    return FALSE;

  do
  {
    /* First check if Core network is GSM MAP. */
    memset(nas_domain_spec_info.value, 0 , RRC_MAX_GSM_INFO_OCTET_COUNT);
    
    if(cn_domain_sys_info_list_ptr->elem[index].cn_Type.t == 
                                      T_rrc_CN_DomainSysInfo_cn_Type_gsm_MAP)
    {
      unsigned char  *data_ptr;
      uint32          i;
  
      if(cn_domain_sys_info_list_ptr->elem[index].cn_DomainIdentity == rrc_CN_DomainIdentity_ps_domain)
      {
        WRRC_MSG0_MED("PS CN Domain specific info is present");

        if(cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP == NULL)
        {
          WRRC_MSG0_ERROR("GSM MAP not present");
          return FALSE;
        }
        
        /* Check if the RRC MM interface structure array can hold this info */
        if(cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP->numocts > 
            RRC_MAX_GSM_INFO_OCTET_COUNT)
        {
          WRRC_MSG1_ERROR("Not enough space for NAS Sys Info (%d bytes)", 
                                                              cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP->numocts);
          return FALSE;
        }
  
        data_ptr = cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP->data;
  
        for( i = 0; i < cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP->numocts; i ++)
        {
          nas_domain_spec_info.value[i] = *data_ptr;
          data_ptr ++;
        }
        rai->rac[0] = nas_domain_spec_info.value[0];
        rac_found = TRUE;
      }
      num_cn_domains ++;
    } /* If CN type is GSM MAP */ 
    n_value--;
    index++;
  }while((n_value != 0) && 
        (num_cn_domains < RRC_MAX_CN_DOMAINS));
  
  WRRC_MSG1_MED("RAC found %d [0-not found,1-found]",rac_found);
  if(!rac_found)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}
#ifdef FEATURE_SCUDIF_ENHANCEMENT
#error code not present
#endif/* FEATURE_SCUDIF_ENHANCEMENT*/

/*===========================================================================

FUNCTION   rrc_is_nw_shared

DESCRIPTION

 This function will indicate if this N/W is shared or not.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
boolean rrc_is_nw_shared
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_MasterInformationBlock *mib_ptr
)
{
  boolean status = FALSE;

  if(RRC_IS_NETWORK_SHARED(mib_ptr))
  {
    WRRC_MSG0_HIGH("Network is shared");
    status = TRUE;
  }
  else
  {
    status = FALSE;
  }
  return status;
}

/*===========================================================================

FUNCTION rrc_malloc_for_external_cmd

DESCRIPTION
  This function is called by the external modules like NAS in order to allocate 
  dynamic memory. This function simply calls mem_malloc and selects suitable
  heap and returs pointer to the allocated memory.

DEPENDENCIES
  The memory manager must have been previously initialized.

RETURN VALUE
  A pointer to the allocated memory if the memory was successfully allocated,
  otherwise NULL.

SIDE EFFECTS
  None.

===========================================================================*/
void * rrc_malloc_for_external_cmd(size_t size)
{
  void * return_ptr =  NULL;
  
  return_ptr =  modem_mem_calloc(1,size,MODEM_MEM_CLIENT_WCDMA_RRC_CMD_CRIT);

  if(return_ptr == NULL)
  {
    ERR_FATAL("Failed to allocate memory of size 0x%x for RRC ext cmd",size, 0, 0);
  }

  return(return_ptr);
}

/*===========================================================================

FUNCTION   rrc_update_rab_type_in_rab_info_r8

DESCRIPTION
 This function is updates the rab type in 'rrc_est_rab_info_type' structure
 using information stored in ASN1 'rrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'rrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  rrc_RAB_InformationSetup *: pointer to ASN1 structure,
  rrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

rrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
rrcrb_rab_e_type rrc_update_rab_type_in_rab_info_r8
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_RAB_InformationSetup_r8 *rab_info_setup_ptr,
  rrc_est_rab_info_type *est_rab_info_ptr 
)
{
  rrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (rrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = RRCRB_PS_DATA_RAB;
  }
  else if (rrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = rrcrb_translate_gsm_map_rab_id(wrrc_as_id,(rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity));
      
#ifdef FEATURE_DUAL_SIM
    mn_rab_type = rrcmn_verify_rab_subs( (radio_access_bearer_T)rab_id ,rrc_get_as_id(wrrc_as_id));
#else
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
    WRRC_MSG1_MED("MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if ( rrclbt_is_lb_on(wrrc_as_id) == TRUE )
    {
      /* Set Rab type to Data */
      MSG_LOW("RAB setup for Loopback Test", 0, 0, 0);
      est_rab_info_ptr->rab_type = RRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type = RRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)
    {
      est_rab_info_ptr->rab_type = RRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = RRCRB_UNKNOWN_RAB;
      WRRC_MSG1_ERROR("Invalid cn_domain value: %d", cn_domain);
    }
  }
  else
  {
    WRRC_MSG1_ERROR("Invalid Cn domain %d",cn_domain);
  }

  WRRC_MSG3_MED("Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
    est_rab_info_ptr->rab_type, RRCRB_CS_VOICE_RAB, RRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}

/*===========================================================================

FUNCTION WCDMA_SET_MODEM_STATS_MASK_SUB

DESCRIPTION
  This function updates Events bitmask stored at RRC

DEPENDENCIES
  None

RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
void wcdma_set_modem_stats_mask_sub (sys_modem_as_id_e_type wrrc_as_id, uint16 bit_mask)
{
  RRC_VALIDATE_SUB_ID(wrrc_as_id);
  WCDMA_RRC_TASK_LOCK();
  rrcdata_logging_info.wcdma_UI_event_bit_mask = bit_mask;
  WRRC_MSG1_HIGH("FTD_DBG: Bitmask sent by CM %x",rrcdata_logging_info.wcdma_UI_event_bit_mask);
  WCDMA_RRC_TASK_FREE();
}

/*===========================================================================

FUNCTION WCDMA_SET_MODEM_STATS_MASK 

DESCRIPTION
  This function updates Events bitmask stored at RRC

DEPENDENCIES
  None

RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
void wcdma_set_modem_stats_mask (uint16 bit_mask)
{
  /*RRC_VALIDATE_FUNC_CALL();*/
  wcdma_set_modem_stats_mask_sub(SYS_MODEM_AS_ID_1, bit_mask);
}


/*===========================================================================

FUNCTION WCDMA_GET_MODEM_STATS_INFO_SUB

DESCRIPTION
  This function copies the WCDMA event fields to the stucture provided as arguments

DEPENDENCIES
  None

RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
void wcdma_get_modem_stats_info_sub (sys_modem_as_id_e_type wrrc_as_id, wcdma_UI_event_info_type *wcdma_info)
{
  RRC_VALIDATE_SUB_ID(wrrc_as_id);
  WCDMA_RRC_TASK_LOCK();
  WRRC_MSG0_MED("FTD_DBG: Populating modem stats info");
  WCDMA_MEMCPY(wcdma_info,
               sizeof(wcdma_UI_event_info_type),
               &(rrcdata_logging_info.wcdma_UI_event_info),
               sizeof(wcdma_UI_event_info_type));
  rrcdata_logging_info.wcdma_UI_event_info.bitmask = 0;
  WCDMA_RRC_TASK_FREE();
}

/*===========================================================================

FUNCTION WCDMA_GET_MODEM_STATS_INFO 

DESCRIPTION
  This function copies the WCDMA event fields to the stucture provided as arguments

DEPENDENCIES
  None

RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
void wcdma_get_modem_stats_info (wcdma_UI_event_info_type *wcdma_info)
{
  /*RRC_VALIDATE_FUNC_CALL();*/
  wcdma_get_modem_stats_info_sub(SYS_MODEM_AS_ID_1, wcdma_info);
}

#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION RRC_SET_AS_ID

DESCRIPTION           This function sets the AS ID provided
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void rrc_set_as_id
(
  sys_modem_as_id_e_type wrrc_as_id, 
  sys_modem_as_id_e_type as_id
)
{
  rrc_dsds_int_data.rrc_sim_id_recvd = as_id;
  WRRC_MSG1_HIGH("DSIM: Setting AS ID as %d",rrc_dsds_int_data.rrc_sim_id_recvd);
}

/*===========================================================================

FUNCTION RRC_GET_AS_ID

DESCRIPTION           This function returns as_id received from NAS if received
                             If not received, returns last received AS ID
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
sys_modem_as_id_e_type rrc_get_as_id
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  if(!rrc_dsds_int_data.rrc_is_sim_id_valid)
  {
    WRRC_MSG1_HIGH("DSIM: SIM INSERTED REQ not received, use theAS ID %d",rrc_dsds_int_data.rrc_sim_id_recvd);
  }
  return (rrc_dsds_int_data.rrc_sim_id_recvd);
}
/*===========================================================================

FUNCTION  rrc_check_send_change_priority

DESCRIPTION

  This function sends change priority request to WRM if necessary.
    
DEPENDENCIES
 This functon is called during addition , modification and deletion of RABs
  None.
 
RETURN VALUE

  Lock status.

SIDE EFFECTS

  None.

===========================================================================*/
void rrc_check_send_change_priority
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  trm_reason_enum_t trm_priority;

  if(RRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  {
    WRRC_MSG1_ERROR("RRC_PROCEDURE_IND for proc type %d received in conn mode OOS", rrc_dsds_int_data.nas_proc_type);
  }
  else if(((rrc_is_voice_rab_present(wrrc_as_id))||(rrc_is_cs_data_rab_present(wrrc_as_id))) &&
          (rrc_dsds_int_data.rrc_trm_priority == TRM_TRAFFIC))
  {
    WRRC_MSG0_HIGH("DSIM_TUNEAWAY: Do not downgrade the priority from TRAFFIC as CS RAB is up.");
  }
  /* If NAS sends RRC_PROCEDURE_IND before RRC moves to connected mode */
  else if(WRM_CLI_STATE_IS_IN_LOCK(WCDMA_TRM, WRM_PRIMARY_CLIENT_RRC, wrrc_as_id) &&
          rrcrce_is_rce_procedure_active(wrrc_as_id) &&
          (RRC_CAMPED_ON == rrc_ccm_get_curr_camping_status(wrrc_as_id,RRC_PROCEDURE_CSP))
         )
  {
    trm_priority = rrc_determine_trm_priority(wrrc_as_id);
    WRRC_MSG1_HIGH("DSIM_TUNEAWAY: Change priority to %d",trm_priority);
    rrc_wrm_change_priority(wrrc_as_id, trm_priority);
    wrm_retain_lock_for_primary(wrrc_as_id);
  }
  else
  {
    WRRC_MSG0_MED("DSIM_TUNEAWAY: Ignoring Change priority");
  }
}

/*===========================================================================

FUNCTION  rrc_determine_trm_priority

DESCRIPTION

  This function checks whether UE allows tune away or not
    
DEPENDENCIES
  None.
 
RETURN VALUE

 None
 
SIDE EFFECTS

  None.

===========================================================================*/
trm_reason_enum_t rrc_determine_trm_priority(sys_modem_as_id_e_type wrrc_as_id)
{
  if((rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_HIGH_PRIORITY_PS_CALL) || 
     (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_CS_CALL) || 
     (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_SUPPLEMENTARY_SERVICE) ||
     (rrc_csp_int_data.rrc_csfb_call_status) ||
     ((rrc_is_voice_rab_present(wrrc_as_id))||(rrc_is_cs_data_rab_present(wrrc_as_id))))
  {
    return TRM_TRAFFIC;
  }
  else if ((rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_CS_REGISTRATION) ||
           (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_PS_REGISTRATION) ||
           (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_PS_CALL_SIGNALING) ||
           (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_SMS) ||
           (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_TEST_LOOPBACK) ||
           (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_MMS)           ||
           (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_NON_IMS_OTHERS)||
           (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_RS_RA)
          )
  {
    return TRM_ACCESS;
  }
  else
  {
    return TRM_BG_TRAFFIC;
  }
}
/*===========================================================================

FUNCTION  rrc_process_dds_sub_ind

DESCRIPTION

  This function processes the DDS sub id indication from NAS
    
DEPENDENCIES
  None.
 
RETURN VALUE

 None
 
SIDE EFFECTS

  None.

===========================================================================*/
void rrc_process_dds_sub_id_ind(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type  *cmd_ptr)
{
  rrc_dsds_int_data.dds_sub_id = cmd_ptr->cmd.rrc_dds_sub_ind.dds_sub_id;
  WRRC_MSG1_HIGH("RRC_DDS_SUB_IND received with dds sub id %d", rrc_dsds_int_data.dds_sub_id);
}
/*===========================================================================

FUNCTION  rrc_get_dds_sub_id

DESCRIPTION

  This function returns the DDS sub id
    
DEPENDENCIES
  None.
 
RETURN VALUE

 sys_modem_as_id_e_type
 
SIDE EFFECTS

  None.

===========================================================================*/
sys_modem_as_id_e_type rrc_get_dds_sub_id(sys_modem_as_id_e_type wrrc_as_id)
{
  return rrc_dsds_int_data.dds_sub_id;
}

/*===========================================================================

FUNCTION  rrc_process_proc_ind

DESCRIPTION

  This function processes the procedure indication received from NAS
    
DEPENDENCIES
  None.
 
RETURN VALUE

 None
 
SIDE EFFECTS

  None.

===========================================================================*/
void rrc_process_proc_ind(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type  *cmd_ptr)
{

  if((!rrc_dsds_int_data.rrc_lte_capability_ind) && (cmd_ptr->cmd.rrc_procedure_ind.proc_type == SYS_PROC_TYPE_NONE) && (FALSE == rrcdt_is_ps_domain_open(wrrc_as_id)))
  {
    WRRC_MSG0_HIGH("RRC_PROCEDURE_END_IND received in non DDS sub, don't move to BGT");
    return;
  }

  rrc_dsds_int_data.nas_proc_type = cmd_ptr->cmd.rrc_procedure_ind.proc_type;
  WRRC_MSG1_HIGH("RRC_PROCEDURE_IND received with proc type %d", rrc_dsds_int_data.nas_proc_type);

  /* Voting for APS off */
  wrm_conn_mode_update_aps_support(WRM_APS_CLIENT_RRC, FALSE, wrrc_as_id);  

  if(RRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  {
    WRRC_MSG1_ERROR("RRC_PROCEDURE_IND for proc type %d received in conn mode OOS", rrc_dsds_int_data.nas_proc_type);
  }
  
  else if((WRM_STATE_IS_IN_LOCK(WCDMA_TRM, wrrc_as_id) || WRM_STATE_IS_LOCK_REQ(WCDMA_TRM_TX,wrrc_as_id)) &&
          (rrcrce_is_rce_procedure_active(wrrc_as_id)) && 
          (RRC_CAMPED_ON == rrc_ccm_get_curr_camping_status(wrrc_as_id,RRC_PROCEDURE_CSP))
         )
  {
    if(((rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_NONE) && (!rrc_is_ps_domain_rab_present(wrrc_as_id))) ||
       (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_NONE_DELAYED))
    {
      WRRC_MSG0_HIGH("RRC_PROCEDURE_END_IND received when PS RABs are not up (or) with Delayed reset");
      rrctmr_start_timer(wrrc_as_id, RRCTMR_TRM_PRIORITY_CHANGE_TIMER,rrcnv_data.rrc_change_trm_priority_tmr_nv);
    }
#ifdef FEATURE_HA_MAINLINE_ONLY_ENABLE
    /* NAS will make sure send PROC update indication only when CS call is ended and CS/PS Registartion is further going ahead */
    else if((!mm_is_call_in_progress(wrrc_as_id)) && (rrc_dsds_int_data.rrc_trm_priority == TRM_TRAFFIC))
    {
      WRRC_MSG2_HIGH("RRC_PROCEDURE_IND for proc type %d received when mm_is_call_in_progress %d", rrc_dsds_int_data.nas_proc_type,mm_is_call_in_progress(wrrc_as_id));
      if(rrc_csp_int_data.rrc_csfb_call_status == TRUE)
      {
        WRRC_MSG0_HIGH("Resetting CSFB call status to downgrade priority when CS call is switched");
        rrc_csp_int_data.rrc_csfb_call_status = FALSE;
      }
      rrc_check_send_change_priority(wrrc_as_id);
    }
#endif
    else
    {
#ifdef FEATURE_WCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
      if(((mm_per_subs_is_ss_in_progress(wrrc_as_id)) && (rrcnv_data.rrc_fast_return_to_lte_after_csfb_ss_nv)) && (rrc_csp_int_data.csfb_extended_status == CSFB_STARTED) && (rrc_dsds_int_data.nas_proc_type == SYS_PROC_TYPE_NONE) &&(!(rrccsp_block_frlte_for_curr_camped_plmn(wrrc_as_id))))
      {
       rrcrce_post_internal_csfb_abort_cmd(wrrc_as_id);
      }
#endif
      rrc_check_send_change_priority(wrrc_as_id);
    }
  }
  else
  {
    /* Currently handled only when RRC is holding the lock and when W is in connected state */
  }
}

#endif
#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
/*===========================================================================
FUNCTION rrc_register_with_msgr_for_client_and_register_umids

DESCRIPTION
  This function rregisters RRC with MSGR and registers for UMIDs it receives
  
DEPENDENCIES
  None
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void rrc_register_with_msgr_for_client_and_register_umids
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  errno_enum_type msgr_error = E_SUCCESS;
  uint8 variant = wrrc_as_id;
  msgr_id_t rrc_rex_q_id;

  msgr_client_create(&wrrc_msgr_client);
  msgr_error = 
  msgr_client_add_rex_q(&wrrc_msgr_client,
                                        wcdma_rrc_get_tcb(wrrc_as_id),
                                        RRC_LTE_RRC_CMD_Q_SIG,
                                        &rrc_cmd_q.rrc_lte_cmd_q,
                                        &rrc_cmd_q.rrc_lte_free_cmd_q,
                                        /*sizeof(q_link_type),*/ offsetof(wcdma_rrc_lte_irat_cmd_type,cmd),
                                        MSGR_NO_OFFSET,
                                        sizeof(wcdma_rrc_lte_irat_cmd_type),
                                        &rrc_rex_q_id
                                       );
  if(msgr_error != E_SUCCESS)
  {
    WRRC_MSG1_HIGH("WTOL: Cannot register the RRC rex client with MSGR, err cause %d",msgr_error);
    return;
  }

  /* In the case of 1x+W with DUAL_WCDMA undefined, we will need to register for UMIDs on sub2. Therefore in generic we added
   * support to register for UMIDs on both subs. This is because LTE started sending all the messages using variant APIs
   */
#ifndef FEATURE_DUAL_WCDMA
  for(variant = SYS_MODEM_AS_ID_1; variant <= SYS_MODEM_AS_ID_2; variant++)
#endif
  {
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_RESEL_REQ, variant);

#ifdef FEATURE_FEMTO_CSG
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_RESEL_RSP, variant);
#endif

    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_ABORT_RESEL_REQ, variant);


    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_REDIR_REQ, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_ABORT_REDIR_REQ, variant);

    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_PLMN_SRCH_REQ, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ, variant);

    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_UTRA_CAPABILITIES_REQ, variant);

    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_PSHO_REQ, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_ABORT_PSHO_REQ, variant);
  
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_RESEL_FAILED_RSP, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_ABORT_RESEL_RSP, variant);


    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_REDIR_FAILED_RSP, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_ABORT_REDIR_RSP, variant);


    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_PLMN_SRCH_RSP, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_ABORT_PLMN_SRCH_RSP, variant);

    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_EUTRA_CAPABILITIES_RSP, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_NW_REQ_EUTRA_CAPABILITIES_RSP, variant);
    /* Register for Dedicated Priorities related UMIDs */
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_GET_DEDICATED_PRI_RSP, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_GET_DEDICATED_PRI_REQ, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_CLEAR_DEDICATED_PRI_IND, variant);

    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_GET_CGI_REQ, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_ABORT_CGI_REQ, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_PLMN_SRCH_SUSPEND_IND, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_GET_PLMN_PRTL_RESULTS_REQ, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_LTE_DEPRI_FREQ_REQ, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_UTRA_SERVING_CELL_INFO_REQ, variant);

    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_PSHO_RSP, variant);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_WCDMA_ABORT_PSHO_RSP, variant);
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, LTE_RRC_ACQ_DB_EARFCN_RSP, variant);
#endif
  }
#if defined(FEATURE_THOR_MODEM) || defined(FEATURE_WCDMA_ATLAS_MODEM) || defined(FEATURE_TABASCO_MODEM)
  msgr_error = 
    msgr_client_add_rex_q(&wrrc_msgr_client,
                                        wcdma_rrc_get_tcb(wrrc_as_id),
                                        RRC_QMI_CMD_Q_SIG,
                                        &rrc_cmd_q.rrc_qmi_cmd_q,
                                        &rrc_cmd_q.rrc_qmi_free_cmd_q,
                                        /*sizeof(q_link_type),*/ offsetof(wcdma_rrc_qmi_cmd_type,cmd),
                                        MSGR_NO_OFFSET,
                                        sizeof(wcdma_rrc_qmi_cmd_type),
                                        &rrc_rex_q_id
                                       );

  if(msgr_error != E_SUCCESS)
  {
    WRRC_ERR1_FATAL("QMI: Cannot register the RRC rex client with MSGR, err cause %d",msgr_error);
  }

#ifdef FEATURE_DUAL_WCDMA
  (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_QMI_IDLE_DRX_INFO_REQ, wrrc_as_id);
#else
  (void)msgr_register( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_QMI_IDLE_DRX_INFO_REQ);
    (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_QMI_PRIORITIZE_LTE_REQ, variant);
#endif

#endif
   (void)msgr_register_variant( MSGR_WCDMA_RRC, &wrrc_msgr_client, rrc_rex_q_id, WCDMA_RRC_QMI_CURRENT_FREQS_INFO_REQ, variant);

}
#endif


#ifdef FEATURE_WCDMA_SET_HS_CAT
/*===========================================================================
FUNCTION rrc_qxdm_set_hs_cat

DESCRIPTION
  This function sets the HSDPA category through QXDM command. 
  It also validates the CAT sent in QXDM command. 
  
DEPENDENCIES
  None
RETURN VALUE
  Boolean - TRUE/FALSE

SIDE EFFECTS
  None
===========================================================================*/
boolean rrc_qxdm_set_hs_cat
(
  sys_modem_as_id_e_type wrrc_as_id,
  uint8 hs_cat
)
{
  WRRC_MSG2_HIGH(" QXDM RRC HS CAT: received %d in state %d",hs_cat, rrc_get_state(wrrc_as_id));
  if (rrc_get_state(wrrc_as_id) != RRC_STATE_DISCONNECTED)
  {
    return FALSE;
  }

#if  (defined FEATURE_HSPA_PLUS_CAT29)||(defined(FEATURE_HSPA_PLUS_CAT24) || defined(FEATURE_HSPA_PLUS_CAT20) \
   || defined(FEATURE_HSPA_PLUS_CAT18) || defined(FEATURE_HSPA_PLUS_CAT16) \
   || defined (FEATURE_HSPA_PLUS_CAT14) || defined (FEATURE_HSPA_PLUS_CAT10) \
   || defined (FEATURE_HSPA_PLUS_CAT9) || defined (FEATURE_HSPA_PLUS_CAT8) \
   || defined(FEATURE_HSPA_PLUS_CAT6))

  if ((hs_cat == 8) || (hs_cat == 6) || (hs_cat == 12)
#ifdef FEATURE_HSPA_PLUS_CAT24
       ||(hs_cat == 24)
#endif /*FEATURE_HSPA_PLUS_CAT24*/
#ifdef FEATURE_HSPA_PLUS_CAT9
       ||(hs_cat == 9)
#endif
#ifdef FEATURE_HSPA_PLUS_CAT10
       || (hs_cat == 10)
#endif
       || (hs_cat > 8)
      )
  {
    rrcnv_data.nv_hsdpa_category= hs_cat;
    return TRUE;
  }
  else
  {
    return FALSE;
  }

#endif

}

#endif



/*===========================================================================
FUNCTION rrc_get_dl_ded_rate_for_ds_sub

DESCRIPTION

  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

rrc_data_rate_info_type rrc_get_data_rate_for_ds_sub
(
  sys_modem_as_id_e_type wrrc_as_id
)
{

  rrc_data_rate_info_type rrc_ds_data_rate;
  rrc_ds_data_rate.dl_ded_rate = 0;
  rrc_ds_data_rate.ul_ded_rate = 0;
  rrc_ds_data_rate.dl_category = rrcnv_data.nv_hsdpa_category;
  rrc_ds_data_rate.ul_category = rrcnv_data.wcdma_nv_hsupa_category;

  switch(rrc_ds_data_rate.dl_category)
  {
    case 6:
      /* 3.6 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 3600000;
      break;
    
    case 8:
      /* 7.2 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 7200000;
      break;
      
    case 9:
      /* 10.1 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 10100000;
      break;

    case 10:
      /* 14.4 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 14400000;
      break;

    case 11:
      /* 0.9 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 900000;
      break;

    case 12:
      /* 1.8 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 1800000;
      break;

    case 13:
      /* 17.6 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 17600000;
      break;

    case 14:
      /* 21.1 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 21100000;
      break;

    case 15:
      /* 23.4 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 23400000;
      break;

    case 16:
      /* 28 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 28000000;
      break;

    case 17:
      /* 23.4 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 23400000;
      break;

    case 18:
      /* 28 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 28000000;
      break;

    case 19:
      /* 35.3 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 35300000;
      break;

    case 20:
      /* 42.2 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 42200000;
      break;

    case 21:
      /* 23.4 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 23400000;
      break;

    case 22:
      /* 28 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 28000000;
      break;

    case 23:
      /* 35.3 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 35300000;
      break;

    case 24:
      /* 42.2 Mbps */
      rrc_ds_data_rate.dl_ded_rate = 42200000;
      break;

    default:
      WRRC_MSG1_ERROR("Unsupported DL UE category %d", rrc_ds_data_rate.dl_category);
      break;

  }

  switch(rrc_ds_data_rate.ul_category)
  {
    case 1:
      /* 0.7110 Mbps */
      rrc_ds_data_rate.ul_ded_rate = 711000;
      break;

    case 2:
      /* 1.3990 Mbps*/
      rrc_ds_data_rate.ul_ded_rate = 1399000;
      break;

    case 3:
      /* 1.4484 Mpbs */
      rrc_ds_data_rate.ul_ded_rate = 1448400;
      break;

    case 4:
      /*2.8860 Mbps*/
      rrc_ds_data_rate.ul_ded_rate = 2886000;
      break;

    case 5:
      /* 2.0000 Mbps */
      rrc_ds_data_rate.ul_ded_rate = 2000000;
      break;

    case 6:
      /* 5.7420 Mbps */
      rrc_ds_data_rate.ul_ded_rate = 5742000;
      break;

    case 7:
#ifdef FEATURE_WCDMA_DC_HSUPA
    case 8:
#endif
      /* 11.4980 Mbps */
      rrc_ds_data_rate.ul_ded_rate = 11498000;
      break;

    default:
      WRRC_MSG1_ERROR("Unsupported UL UE category %d", rrc_ds_data_rate.ul_category);
      break;

  }
  return(rrc_ds_data_rate);
  
}


/*===========================================================================
FUNCTION rrc_get_dl_ded_rate_for_ds

DESCRIPTION

  
DEPENDENCIES
  None
  
RETURN VALUE
  None
  
SIDE EFFECTS
  None
===========================================================================*/

rrc_data_rate_info_type rrc_get_data_rate_for_ds
(
  void
)
{
  return(rrc_get_data_rate_for_ds_sub(SYS_MODEM_AS_ID_1));
}
/*===========================================================================

FUNCTION rrc_get_ul_transaction_id

DESCRIPTION
  This function will extract the transaction ID from the OTA

DEPENDENCIES


RETURN VALUE
  uint8

SIDE EFFECTS
  None
===========================================================================*/

static uint32 rrc_get_ul_transaction_id(sys_modem_as_id_e_type wrrc_as_id,void *msg_ptr,uecomdef_logchan_e_type rrclog_ul_lc)
{
  
  uint32 transactionid = 0xFFFFFFFF;                       /* Variable for transaction id */
  rrc_UL_DCCH_Message *dcch_msg_ptr=NULL;    /* Pointer to Downlink DCCH */
  rrc_UL_CCCH_Message *ccch_msg_ptr = NULL; /* Pointer to  downlink CCCH message */   

  if(rrclog_ul_lc == UE_LOGCHAN_DCCH)
  {
    dcch_msg_ptr = (rrc_UL_DCCH_Message *)msg_ptr;

    if(dcch_msg_ptr!=NULL)
    {
      if(dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_activeSetUpdateComplete)
      {
        transactionid = dcch_msg_ptr->message.u.activeSetUpdateComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_activeSetUpdateFailure)
      {
        transactionid = dcch_msg_ptr->message.u.activeSetUpdateFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_cellChangeOrderFromUTRANFailure)
      {
        transactionid = dcch_msg_ptr->message.u.cellChangeOrderFromUTRANFailure.u.r3.cellChangeOrderFromUTRANFailure_r3.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_counterCheckResponse  )
      {
        transactionid = dcch_msg_ptr->message.u.counterCheckResponse.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_handoverFromUTRANFailure)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_measurementControlFailure )
      {
        transactionid = dcch_msg_ptr->message.u.measurementControlFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_physicalChannelReconfigurationComplete)
      {
          transactionid = dcch_msg_ptr->message.u.physicalChannelReconfigurationComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_physicalChannelReconfigurationFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.physicalChannelReconfigurationFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_radioBearerReconfigurationComplete )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReconfigurationComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_radioBearerReconfigurationFailure )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReconfigurationFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_radioBearerReleaseComplete )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReleaseComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_radioBearerReleaseFailure )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReleaseFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_radioBearerSetupComplete )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerSetupComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_radioBearerSetupFailure )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_rrcConnectionReleaseComplete )
      {
        transactionid = dcch_msg_ptr->message.u.rrcConnectionReleaseComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_rrcConnectionSetupComplete )
      {
        transactionid = dcch_msg_ptr->message.u.rrcConnectionSetupComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_rrcStatus )
      {
        if(dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.t == T_rrc_ProtocolErrorMoreInformation_diagnosticsType_type1)
        {
          switch(dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->t)
          {
            case T_rrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError:
            case T_rrc_ProtocolErrorMoreInformation_type1_messageTypeNonexistent:
              /*Do nothing */
              break;
        
            case T_rrc_ProtocolErrorMoreInformation_type1_messageNotCompatibleWithReceiverState:
              transactionid = dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
                u.messageNotCompatibleWithReceiverState->rrc_TransactionIdentifier;
              break;
        
            case T_rrc_ProtocolErrorMoreInformation_type1_ie_ValueNotComprehended:
              transactionid = dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
                u.ie_ValueNotComprehended->rrc_TransactionIdentifier;
              break;
        
            case T_rrc_ProtocolErrorMoreInformation_type1_conditionalInformationElementError:
              transactionid = dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
                u.conditionalInformationElementError->rrc_TransactionIdentifier;
              break;
        
            case T_rrc_ProtocolErrorMoreInformation_type1_messageExtensionNotComprehended:
              transactionid = dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
                u.messageExtensionNotComprehended->rrc_TransactionIdentifier ;
              break;
        
            case T_rrc_ProtocolErrorMoreInformation_type1_spare1:
            case T_rrc_ProtocolErrorMoreInformation_type1_spare2:
              /*Do Nothing */
              break;
        
            default:
              WRRC_MSG1_HIGH("Invalid protocol Error Choice: %d",dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->t);
              break;
          } 
        }
        else
        {
          WRRC_MSG1_HIGH("Invalid diagnosticsType Choice: %d",dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.t);
        }
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_securityModeComplete  )
      {
        transactionid = dcch_msg_ptr->message.u.securityModeComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_securityModeFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.securityModeFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_transportChannelReconfigurationComplete  )
      {
        transactionid = dcch_msg_ptr->message.u.transportChannelReconfigurationComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_transportChannelReconfigurationFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.transportChannelReconfigurationFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_transportFormatCombinationControlFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.transportFormatCombinationControlFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_ueCapabilityInformation  )
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityInformation.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_utranMobilityInformationConfirm  )
      {
        transactionid = dcch_msg_ptr->message.u.utranMobilityInformationConfirm.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_rrc_UL_DCCH_MessageType_utranMobilityInformationFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.utranMobilityInformationFailure.rrc_TransactionIdentifier;
      }
    }
  }
  else if (rrclog_ul_lc == UE_LOGCHAN_CCCH)
  {
    ccch_msg_ptr = (rrc_UL_CCCH_Message *) msg_ptr;

    if(ccch_msg_ptr!=NULL)
    {
      if(ccch_msg_ptr->message.t == T_rrc_UL_CCCH_MessageType_cellUpdate )
      {
        if (ccch_msg_ptr->message.u.cellUpdate.m.failureCausePresent)
        {
          transactionid = ccch_msg_ptr->message.u.cellUpdate.failureCause.rrc_TransactionIdentifier;
        }
      }
      else if(ccch_msg_ptr->message.t == T_rrc_UL_CCCH_MessageType_uraUpdate )
      {
        if(ccch_msg_ptr->message.u.uraUpdate.protocolErrorIndicator.t == T_rrc_ProtocolErrorIndicatorWithMoreInfo_errorOccurred)
        {
          transactionid = ccch_msg_ptr->message.u.uraUpdate.protocolErrorIndicator.u.errorOccurred->rrc_TransactionIdentifier;
        }
      }
    }
  }
  return transactionid;

}
/*===========================================================================

FUNCTION rrc_get_dl_transaction_id

DESCRIPTION
  This function will extract the transaction ID from the OTA

DEPENDENCIES


RETURN VALUE
  uint8

SIDE EFFECTS
  None
===========================================================================*/

static uint32 rrc_get_dl_transaction_id(sys_modem_as_id_e_type wrrc_as_id,rrc_cmd_type *cmd_ptr)
{

  void *decoded_msg=NULL;
  uint32 transactionid = 0xFFFFFFFF;                       /* Variable for transaction id */
  uint8 dl_sdu_type = 0xFF;
  rrc_DL_DCCH_Message *dcch_msg_ptr=NULL;    /* Pointer to Downlink DCCH */
  rrc_DL_CCCH_Message *ccch_msg_ptr = NULL; /* Pointer to  downlink CCCH message */   
  dl_sdu_type = cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type;
  decoded_msg = cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  if(dl_sdu_type == rrc_DL_DCCH_Message_PDU)
  {
    dcch_msg_ptr = (rrc_DL_DCCH_Message *)decoded_msg;
  
    if(dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_activeSetUpdate)
    {
      if (dcch_msg_ptr->message.u.activeSetUpdate.t == T_rrc_ActiveSetUpdate_r3)
      {
        transactionid = dcch_msg_ptr->message.u.activeSetUpdate.u.r3.activeSetUpdate_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.activeSetUpdate.t == T_rrc_ActiveSetUpdate_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.activeSetUpdate.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_assistanceDataDelivery)
    {
      if (dcch_msg_ptr->message.u.assistanceDataDelivery.t == T_rrc_AssistanceDataDelivery_r3)
      {
        transactionid = dcch_msg_ptr->message.u.assistanceDataDelivery.u.r3.assistanceDataDelivery_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.assistanceDataDelivery.t == T_rrc_AssistanceDataDelivery_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.assistanceDataDelivery.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_cellChangeOrderFromUTRAN)
    {
      if (dcch_msg_ptr->message.u.cellChangeOrderFromUTRAN.t == T_rrc_CellChangeOrderFromUTRAN_r3)
      {
        transactionid = dcch_msg_ptr->message.u.cellChangeOrderFromUTRAN.u.r3.cellChangeOrderFromUTRAN_IEs.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.cellChangeOrderFromUTRAN.t == T_rrc_CellChangeOrderFromUTRAN_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.cellChangeOrderFromUTRAN.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_cellUpdateConfirm)
    {
      if (dcch_msg_ptr->message.u.cellUpdateConfirm.t == T_rrc_CellUpdateConfirm_r3)
      {
        transactionid = dcch_msg_ptr->message.u.cellUpdateConfirm.u.r3.cellUpdateConfirm_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.cellUpdateConfirm.t == T_rrc_CellUpdateConfirm_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_counterCheck)
    {
      if (dcch_msg_ptr->message.u.counterCheck.t == T_rrc_CounterCheck_r3)
      {
        transactionid = dcch_msg_ptr->message.u.counterCheck.u.r3.counterCheck_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.counterCheck.t == T_rrc_CounterCheck_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.counterCheck.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_downlinkDirectTransfer)
    {
      if (dcch_msg_ptr->message.u.downlinkDirectTransfer.t == T_rrc_DownlinkDirectTransfer_r3)
      {
        transactionid = dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.downlinkDirectTransfer_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.downlinkDirectTransfer.t == T_rrc_DownlinkDirectTransfer_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.downlinkDirectTransfer.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM )
    {
      if (dcch_msg_ptr->message.u.handoverFromUTRANCommand_GSM.t == T_rrc_HandoverFromUTRANCommand_GSM_r3)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANCommand_GSM.u.r3.handoverFromUTRANCommand_GSM_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.handoverFromUTRANCommand_GSM.t == T_rrc_HandoverFromUTRANCommand_GSM_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANCommand_GSM.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_CDMA2000)
    {
      if (dcch_msg_ptr->message.u.handoverFromUTRANCommand_CDMA2000.t == T_rrc_HandoverFromUTRANCommand_CDMA2000_r3)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANCommand_CDMA2000.u.r3.handoverFromUTRANCommand_CDMA2000_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.handoverFromUTRANCommand_CDMA2000.t == T_rrc_HandoverFromUTRANCommand_CDMA2000_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANCommand_CDMA2000.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_measurementControl)
    {
      if (dcch_msg_ptr->message.u.measurementControl.t == T_rrc_MeasurementControl_r3)
      {
        transactionid = dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.measurementControl.t == T_rrc_MeasurementControl_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_pagingType2)
    {
      transactionid = dcch_msg_ptr->message.u.pagingType2.rrc_TransactionIdentifier;
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_physicalChannelReconfiguration)
    {
      if (dcch_msg_ptr->message.u.physicalChannelReconfiguration.t == T_rrc_PhysicalChannelReconfiguration_r3)
      {
        transactionid = dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.r3.physicalChannelReconfiguration_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.physicalChannelReconfiguration.t == T_rrc_PhysicalChannelReconfiguration_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_physicalSharedChannelAllocation )
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_radioBearerReconfiguration )
    {
      if (dcch_msg_ptr->message.u.radioBearerReconfiguration.t == T_rrc_RadioBearerReconfiguration_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReconfiguration.u.r3.radioBearerReconfiguration_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.radioBearerReconfiguration.t == T_rrc_RadioBearerReconfiguration_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_radioBearerRelease)
    {
      if (dcch_msg_ptr->message.u.radioBearerRelease.t == T_rrc_RadioBearerRelease_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerRelease.u.r3.radioBearerRelease_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.radioBearerRelease.t == T_rrc_RadioBearerRelease_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerRelease.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_radioBearerSetup)
    {
      if (dcch_msg_ptr->message.u.radioBearerSetup.t == T_rrc_RadioBearerSetup_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerSetup.u.r3.radioBearerSetup_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.radioBearerSetup.t == T_rrc_RadioBearerSetup_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerSetup.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_rrcConnectionRelease)
    {
      if (dcch_msg_ptr->message.u.rrcConnectionRelease.t == T_rrc_RRCConnectionRelease_r3)
      {
        transactionid = dcch_msg_ptr->message.u.rrcConnectionRelease.u.r3.rrcConnectionRelease_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.rrcConnectionRelease.t == T_rrc_RRCConnectionRelease_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_securityModeCommand)
    {
      if (dcch_msg_ptr->message.u.securityModeCommand.t == T_rrc_SecurityModeCommand_r3)
      {
        transactionid = dcch_msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.securityModeCommand.t == T_rrc_SecurityModeCommand_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.securityModeCommand.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_signallingConnectionRelease)
    {
      if (dcch_msg_ptr->message.u.signallingConnectionRelease.t == T_rrc_SignallingConnectionRelease_r3)
      {
        transactionid = dcch_msg_ptr->message.u.signallingConnectionRelease.u.r3.signallingConnectionRelease_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.signallingConnectionRelease.t == T_rrc_SignallingConnectionRelease_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.signallingConnectionRelease.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_transportChannelReconfiguration)
    {
      if (dcch_msg_ptr->message.u.transportChannelReconfiguration.t == T_rrc_TransportChannelReconfiguration_r3)
      {
        transactionid = dcch_msg_ptr->message.u.transportChannelReconfiguration.u.r3.transportChannelReconfiguration_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.transportChannelReconfiguration.t == T_rrc_TransportChannelReconfiguration_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_transportFormatCombinationControl)
    {
      transactionid = dcch_msg_ptr->message.u.transportFormatCombinationControl.rrc_TransactionIdentifier;
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_ueCapabilityEnquiry)
    {
      if (dcch_msg_ptr->message.u.ueCapabilityEnquiry.t == T_rrc_UECapabilityEnquiry_r3)
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.ueCapabilityEnquiry_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.ueCapabilityEnquiry.t == T_rrc_UECapabilityEnquiry_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_ueCapabilityInformationConfirm)
    {
      if (dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.t == T_rrc_UECapabilityInformationConfirm_r3)
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.u.r3.ueCapabilityInformationConfirm_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.t == T_rrc_UECapabilityInformationConfirm_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_uplinkPhysicalChannelControl)
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_utranMobilityInformation)
    {
      if (dcch_msg_ptr->message.u.utranMobilityInformation.t == T_rrc_UTRANMobilityInformation_r3)
      {
        transactionid = dcch_msg_ptr->message.u.utranMobilityInformation.u.r3.utranMobilityInformation_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.utranMobilityInformation.t == T_rrc_UTRANMobilityInformation_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GERANIu)
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_mbmsModifiedServicesInformation)
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_etwsPrimaryNotificationWithSecurity)
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)
    {
      //Not supported
    }
  }
  else if (dl_sdu_type == rrc_DL_CCCH_Message_PDU)
  {
    ccch_msg_ptr = (rrc_DL_CCCH_Message *) decoded_msg;
    if(ccch_msg_ptr->message.t == T_rrc_DL_CCCH_MessageType_cellUpdateConfirm )
    {
      if (ccch_msg_ptr->message.u.cellUpdateConfirm.t == T_rrc_CellUpdateConfirm_CCCH_r3)
      {
        transactionid = ccch_msg_ptr->message.u.cellUpdateConfirm.u.r3.cellUpdateConfirm_r3.rrc_TransactionIdentifier;
      }
      else if(ccch_msg_ptr->message.u.cellUpdateConfirm.t == T_rrc_CellUpdateConfirm_CCCH_later_than_r3)
      {
        transactionid = ccch_msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (ccch_msg_ptr->message.t  == T_rrc_DL_CCCH_MessageType_rrcConnectionReject)
    {
      if(ccch_msg_ptr->message.u.rrcConnectionReject.t == T_rrc_RRCConnectionReject_r3)	
      {
        transactionid= ccch_msg_ptr->message.u.rrcConnectionReject.u.r3.rrcConnectionReject_r3.rrc_TransactionIdentifier;
      }
      else if(ccch_msg_ptr->message.u.rrcConnectionReject.t == T_rrc_RRCConnectionReject_later_than_r3)
      {
        transactionid = ccch_msg_ptr->message.u.rrcConnectionReject.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (ccch_msg_ptr->message.t  == T_rrc_DL_CCCH_MessageType_rrcConnectionRelease)
    {
      if(ccch_msg_ptr->message.u.rrcConnectionRelease.t == T_rrc_RRCConnectionRelease_CCCH_r3)
      {
        transactionid= ccch_msg_ptr->message.u.rrcConnectionRelease.u.r3.rrcConnectionRelease_CCCH_r3.rrcConnectionRelease.rrc_TransactionIdentifier;
      }
      else if(ccch_msg_ptr->message.u.rrcConnectionRelease.t == T_rrc_RRCConnectionRelease_CCCH_later_than_r3)
      {
       transactionid = ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (ccch_msg_ptr->message.t  == T_rrc_DL_CCCH_MessageType_rrcConnectionSetup)
    {
      if(ccch_msg_ptr->message.u.rrcConnectionSetup.t == T_rrc_RRCConnectionSetup_r3)
      {
        transactionid= ccch_msg_ptr->message.u.rrcConnectionSetup.u.r3.rrcConnectionSetup_r3.rrc_TransactionIdentifier;
      }
      else if(ccch_msg_ptr->message.u.rrcConnectionSetup.t == T_rrc_RRCConnectionSetup_later_than_r3)
      {
        transactionid = ccch_msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (ccch_msg_ptr->message.t == T_rrc_DL_CCCH_MessageType_uraUpdateConfirm)
    {
      if(ccch_msg_ptr->message.u.uraUpdateConfirm.t != T_rrc_URAUpdateConfirm_CCCH_later_than_r3)
      {
        transactionid = ccch_msg_ptr->message.u.uraUpdateConfirm.u.r3.uraUpdateConfirm_CCCH_r3.uraUpdateConfirm.rrc_TransactionIdentifier;
      }
      else
      {
        //Not supported
      }
    }
  }
  return transactionid;

}
/*===========================================================================

FUNCTION rrc_get_ul_log_message

DESCRIPTION
  This function will log the channel type, transaction ID and message type from OTA.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_get_ul_log_message(sys_modem_as_id_e_type wrrc_as_id, pdu_buf_type encoded_sdu,uint8 channel_type,uint8 message_type,uint8 rb_id,void *msg_ptr,uecomdef_logchan_e_type rrclog_ul_lc)
{
  log_message_type log_message;
  rlc_lc_id_type ul_rlc_id = RRCLCM_RLC_LC_ID_NOT_FOUND;
  log_wcdma_trch_type log_wcdma_trch = WCDMA_TRCH_INVALID;

  if(rrcdata_logging_info.rrc_mmal_log_cb_func!=NULL)
  {
    if((encoded_sdu.value==NULL)||(channel_type==0xFF)||(message_type==0xFF))
    {
      return;
    }
    log_message.direction = 1;
    log_message.channel_type = channel_type;
    log_message.msg_type = message_type;
    log_message.raw_sdu = encoded_sdu.value;
    log_message.raw_sdu_length = encoded_sdu.length;

    if((rrclog_ul_lc == UE_LOGCHAN_DCCH) || (rrclog_ul_lc == UE_LOGCHAN_CCCH))
    {
      log_message.transactionid = rrc_get_ul_transaction_id(wrrc_as_id,msg_ptr,rrclog_ul_lc);	
    }

    ul_rlc_id = rrclcm_get_ul_rlc_lc_id(wrrc_as_id, rb_id);

    if(RRCLCM_RLC_LC_ID_NOT_FOUND!= ul_rlc_id)
    {
      log_wcdma_trch =  l2_get_trch_type_for_given_rb_id(wrrc_as_id, rb_id,1,ul_rlc_id);
      if(WCDMA_TRCH_INVALID != log_wcdma_trch)
      {
        log_message.transport_channel_type = log_wcdma_trch;
      }
    }
    rrcdata_logging_info.rrc_mmal_log_cb_func(&log_message);
  }
}

/*===========================================================================

FUNCTION rrc_get_dl_log_message

DESCRIPTION
  This function will log the channel type, transaction ID and message type from OTA.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_get_dl_log_message(sys_modem_as_id_e_type wrrc_as_id,rrc_cmd_type *cmd_ptr)
{
  log_message_type dl_log_message;
  uint8 channel_type;
  uint8 rb_id;
  uecomdef_logchan_e_type rrclog_dl_lc;
  rlc_lc_id_type dl_rlc_id = RRCLCM_RLC_LC_ID_NOT_FOUND;
  log_wcdma_trch_type log_wcdma_trch = WCDMA_TRCH_INVALID;
  dl_log_message.transactionid = 0xFFFFFFFF;

  if(rrcdata_logging_info.rrc_mmal_log_cb_func!=NULL)
  {
    //check for valid input
    if((cmd_ptr->cmd.downlink_sdu_ind.decoded_msg==NULL)||(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu==NULL))
    {
      return;
    }

    dl_log_message.direction = 0;

    if(RRCLCM_FAILURE== rrclcm_get_dl_lc_type(wrrc_as_id,cmd_ptr->cmd.downlink_sdu_ind.lc_id,
                                 &rrclog_dl_lc))
    {
      return;
    }

    /* Determine and store RB and channel type info */
    rb_id = (uint8)rrclcm_get_dl_rb_id(wrrc_as_id,cmd_ptr->cmd.downlink_sdu_ind.lc_id);
    switch(rrclog_dl_lc)
    {
      case UE_LOGCHAN_BCCH:         /* Broadcast Control Channel */
        switch(rb_id)
        {
          case BCCH_S_RADIO_BEARER_ID:
          case BCCH_N_RADIO_BEARER_ID:
            channel_type = RRCLOG_SIG_DL_BCCH_BCH;
            break;
  
          case BCCH_FACH_RADIO_BEARER_ID:
            channel_type = RRCLOG_SIG_DL_BCCH_FACH;
            break;
  
          default:
            WRRC_MSG1_ERROR("Invalid RB Id: %d for BCCH Log Channel",
                 rb_id);
            channel_type = 0xFF;
            break;
        }
        break;
  
      case UE_LOGCHAN_PCCH:         /* Paging Control Channel.*/
        channel_type = RRCLOG_SIG_DL_PCCH;
        break;
  
      case UE_LOGCHAN_CCCH:         /* Common Control Channel.*/
        channel_type = RRCLOG_SIG_DL_CCCH;
        break;
  
      case UE_LOGCHAN_DCCH:         /* Dedicated Control Channel.*/
        channel_type = RRCLOG_SIG_DL_DCCH;
        break;
  
      default:
        channel_type = 0xFF;
        break;
    }
  
    if(channel_type==0xFF)
    {
      return;
    }

    dl_log_message.channel_type = channel_type;
    dl_log_message.msg_type = rrc_get_dl_asn1_message_type(wrrc_as_id, cmd_ptr);

    if(dl_log_message.msg_type==0xFF)
    {
      return;
    }

    dl_log_message.raw_sdu = cmd_ptr->cmd.downlink_sdu_ind.dl_sdu;
    dl_log_message.raw_sdu_length = cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length;		
  
    if((channel_type == RRCLOG_SIG_DL_CCCH)	||(channel_type == RRCLOG_SIG_DL_DCCH))
    {
      dl_log_message.transactionid = rrc_get_dl_transaction_id(wrrc_as_id, cmd_ptr);
    }
  
    if(dl_log_message.transactionid==0xFF)
    {
      return;
    }

    if(channel_type == RRCLOG_SIG_DL_BCCH_BCH)
    {
      dl_log_message.transport_channel_type = WCDMA_TRCH_BCH;
    }
    else if (channel_type == RRCLOG_SIG_DL_BCCH_FACH)
    {
#ifdef FEATURE_WCDMA_HS_FACH        
      if(TRUE == rrcllc_get_hs_status_in_e_fach(wrrc_as_id,current_config_ptr))
      {
        dl_log_message.transport_channel_type = WCDMA_TRCH_HSDSCH;
      }
      else
#endif
      {
        dl_log_message.transport_channel_type = WCDMA_TRCH_FACH;
      }
    }
    else if (channel_type == RRCLOG_SIG_DL_PCCH)
    {
#ifdef FEATURE_WCDMA_HS_FACH    
      if(TRUE == rrcllc_get_hs_status_in_e_fach(wrrc_as_id,current_config_ptr))
      {
        dl_log_message.transport_channel_type = WCDMA_TRCH_HSDSCH;
      }
      else
#endif
      {
        dl_log_message.transport_channel_type = WCDMA_TRCH_PCH;
      }
    }
    else
    {
      dl_rlc_id = rrclcm_get_dl_rlc_lc_id(wrrc_as_id, rb_id);
      if(RRCLCM_RLC_LC_ID_NOT_FOUND!= dl_rlc_id)
      {
        log_wcdma_trch = l2_get_trch_type_for_given_rb_id(wrrc_as_id, rb_id,0,dl_rlc_id);
        if(WCDMA_TRCH_INVALID != log_wcdma_trch)
        {
          dl_log_message.transport_channel_type = log_wcdma_trch;
        }
        }
      }
  }
  return;
}
/*===========================================================================

FUNCTION rrc_register_mmal_cb

DESCRIPTION
  This function is the API to register the function pointer for MMAL.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_register_mmal_cb
(
  RRC_MMAL_LOG_CB_TYPE  *cb_ptr
)
{
  /*Hack */
#ifdef FEATURE_DUAL_WCDMA
  sys_modem_as_id_e_type wrrc_as_id = SYS_MODEM_AS_ID_1;
#endif
  rrcdata_logging_info.rrc_mmal_log_cb_func=cb_ptr;
}
/*===========================================================================

FUNCTION rrc_malloc_interlayer_cmd

DESCRIPTION
  This function is called by the RRC modules in order to allocate 
  dynamic memory.
  If FEATURE_WCDMA_USE_MODEM_HEAP
  is defined , we allocate from the modem heap with in sub layers 
  of UMTS, else we allocate the tmc heap.

DEPENDENCIES

RETURN VALUE
  A pointer to the allocated memory if the memory was successfully allocated,
  otherwise NULL.

SIDE EFFECTS
  None.

===========================================================================*/
void* rrc_malloc_interlayer_cmd
(
  sys_modem_as_id_e_type wrrc_as_id, 
  size_t size
)
{
  void *return_ptr = NULL;
  return_ptr = modem_mem_calloc(1,size,
                MODEM_MEM_CLIENT_WCDMA_EXT_CMD_CRIT);

  if(return_ptr == NULL)
  {
    WRRC_ERR1_FATAL("Failed to allocate memory of size 0x%x",size);
  }

  return (return_ptr);
}

/*===========================================================================

FUNCTION rrc_free_interlayer_cmd

DESCRIPTION
  This function is called by the RRC Modules in order to free dynamic
  memory.
  If FEATURE_WCDMA_USE_MODEM_HEAP
  is defined , we free from the modem heap with in sub layers 
  of UMTS else we free the tmc heap.

DEPENDENCIES
  The memory manager must have been previously initialized.

RETURN VALUE
  A pointer to the allocated memory if the memory was successfully allocated,
  otherwise NULL.

SIDE EFFECTS
  None.

===========================================================================*/
void rrc_free_interlayer_cmd
(
  void *ptr
)
{
  modem_mem_free(ptr, MODEM_MEM_CLIENT_WCDMA_EXT_CMD_CRIT);
}

/*===========================================================================

FUNCTION   rrc_check_psrab_data_is_present

DESCRIPTION
  This function check for whether the WM size corresponding to
  any of the rb_id is zero or not.

DEPENDENCIES
  None.

RETURN VALUE
  FALSE, if WM_size for all the rb_id is 0.
  TRUE, if WM_size for any one of the PS rb_id is non-zero. 

SIDE EFFECTS
  None.
===========================================================================*/
boolean rrc_check_psrab_data_is_present
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  uint32 rab_index=0,i=0;    /* Used if there is more than 1 RAB in the message */
  uint32 local_rlc_wm_size;

   /* Check whether this RAB exists in established_rab db */
  for (rab_index=0; rab_index<MAX_RAB_TO_SETUP; rab_index++)
  {
    if ((rrc_est_rabs.rabs[rab_index].cn_domain == RRC_PS_DOMAIN_CN_ID) &&
         (rrc_est_rabs.rabs[rab_index].num_rbs_for_rab > 0) &&
         (rrc_est_rabs.rabs[rab_index].rab_id !=  RRC_INVALID_RAB_ID)
        )
    {
      for(i=0; i< rrc_est_rabs.rabs[rab_index].num_rbs_for_rab; i++)
      {
        local_rlc_wm_size = rlc_rrc_get_wm_info_req(wrrc_as_id, rrclcm_get_ul_rlc_lc_id(wrrc_as_id,
                                                  rrc_est_rabs.rabs[rab_index].rb_for_rab[i].rb_id));
        if (local_rlc_wm_size != 0)
        {
          WRRC_MSG1_HIGH("WM size is not 0 for rb_id%d",rrc_est_rabs.rabs[rab_index].rb_for_rab[i].rb_id);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

#ifdef FEATURE_WCDMA_SECURITY_SETTINGS

/*===========================================================================

FUNCTION rrc_qxdm_integrity_enable_diagcmd

DESCRIPTION
 This function handles setting the integrity flag enable/disable.

DEPENDENCIES
  None.

RETURN VALUE
  uint8

===========================================================================*/

uint8 rrc_qxdm_integrity_enable_diagcmd
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint8 enable
)
{  

  WRRC_MSG3_HIGH("Recevied the QXDM cmd integrity setting as enable/disable %d in state %d mode %d",enable, rrc_get_state(wrrc_as_id), rrcmcm_get_rrc_mode(wrrc_as_id));
  if((rrc_get_state(wrrc_as_id) == RRC_STATE_DISCONNECTED) &&
      (rrcmcm_get_rrc_mode(wrrc_as_id) == RRC_MODE_ACTIVE))
  {
    if(enable == TRUE)
    {
      rrcsmc_ext_data.nv_integrity_enabled = TRUE;
    }
    else
    {
      rrcsmc_ext_data.nv_integrity_enabled = FALSE;
    }
    /*if integrity is enabled, then fake security should be disabled.. only one of the 2
      nv items should be turned on*/
    if(rrcsmc_ext_data.nv_integrity_enabled == 1)
    {
      nv_fake_security_enabled = 0;
    }
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/*===========================================================================

FUNCTION rrc_qxdm_ciphering_enable_diagcmd

DESCRIPTION
 This function handles setting the ciphering flag enable/disable.

DEPENDENCIES
  None.

RETURN VALUE
  uint8

===========================================================================*/

uint8 rrc_qxdm_ciphering_enable_diagcmd
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint8 enable
)
{  

  WRRC_MSG3_HIGH("Recevied the QXDM cmd ciphering setting as enable/disable %d in state %d mode %d",enable, rrc_get_state(wrrc_as_id), rrcmcm_get_rrc_mode(wrrc_as_id));
  if((rrc_get_state(wrrc_as_id) == RRC_STATE_DISCONNECTED) &&
      (rrcmcm_get_rrc_mode(wrrc_as_id) == RRC_MODE_ACTIVE))
  {
    
    if(enable == TRUE)
    {
      rrcsmc_ext_data.nv_ciphering_enabled = TRUE;
      rrcsmc_ext_data.nv_ciphering_enabled_status = TRUE;
    }
    else
    {
      rrcsmc_ext_data.nv_ciphering_enabled = FALSE;
    }
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION rrc_qxdm_fakesecurity_enable_diagcmd

DESCRIPTION
 This function handles setting the fake security flag enable/disable.

DEPENDENCIES
  None.

RETURN VALUE
  uint8

===========================================================================*/
uint8 rrc_qxdm_fakesecurity_enable_diagcmd
(
  sys_modem_as_id_e_type wrrc_as_id, 
  uint8 enable
)
{  
  WRRC_MSG3_HIGH("Recevied the QXDM cmd fakesecurity setting as enable/disable %d in state %d mode %d",enable, rrc_get_state(wrrc_as_id), rrcmcm_get_rrc_mode(wrrc_as_id));
  if((rrc_get_state(wrrc_as_id) == RRC_STATE_DISCONNECTED) &&
      (rrcmcm_get_rrc_mode(wrrc_as_id) == RRC_MODE_ACTIVE))
  {
    if(enable == TRUE)
    {
      nv_fake_security_enabled = TRUE;      
    }
    else
    {
      nv_fake_security_enabled = FALSE;
    }
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

#endif /*FEATURE_WCDMA_SECURITY_SETTINGS*/


/*===========================================================================
 FUNCTION     WCDMA_RRC_INIT_TCB

 DESCRIPTION
   This function populates the cached WCDMA L2 DL TCB pointer through rcinit. 

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   An assertion will fail if the lookup fails.
===========================================================================*/
void wcdma_rrc_init_tcb(sys_modem_as_id_e_type wrrc_as_id)
{
  if (wcdma_rrc_tcb_ptr == RCINIT_NULL)
  {
    /* Need to get right TCB ptr based on was id that is passed 
     * Argument passed to rcinit_lookup_rextask() may need a change 
     */
#ifdef FEATURE_DUAL_WCDMA
   if(wrrc_as_id == SYS_MODEM_AS_ID_2)
   {
     wcdma_rrc_tcb_ptr = rcinit_lookup_rextask("rrc_1");
   }
   else
#endif
   {
     wcdma_rrc_tcb_ptr = rcinit_lookup_rextask("rrc_0"); 
   }

    if (wcdma_rrc_tcb_ptr == RCINIT_NULL)
    {
      WRRC_MSG0_ERROR("Lookup for wcdma_rrc_tcb failed");
      ASSERT(0);
    }
  }
}
/*===========================================================================
 FUNCTION     WCDMA_RRC_GET_TCB

 DESCRIPTION
   This function returns a pointer to the WCDMA L1 TCB. 

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   A pointer to the WCDMA RRC TCB.

 SIDE EFFECTS
   An assertion will fail if the lookup fails.
===========================================================================*/
rex_tcb_type* wcdma_rrc_get_tcb(sys_modem_as_id_e_type wrrc_as_id)
{

  if (wcdma_rrc_tcb_ptr == RCINIT_NULL)
  {
    WRRC_MSG0_ERROR("Trying to get tcb before WCDMA RRC is initialized");
    wcdma_rrc_init_tcb(wrrc_as_id);
  }
  return wcdma_rrc_tcb_ptr;
}
#ifdef WCDMA_RRC_INDICATE_CODEC_TO_CM
/*===========================================================================

FUNCTION rrc_send_codec_info_on_activation_timer_expiry

DESCRIPTION
  This function sends codec info to CM on activation timer expiry for L1

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void rrc_send_codec_info_on_activation_timer_expiry(sys_modem_as_id_e_type wrrc_as_id)
{
  boolean ordered_codec_status_flag = FALSE;
  uint32 ordered_codec_info = rrc_get_ordered_codec_mode(wrrc_as_id);
  uint32 current_codec_info = rrc_get_current_codec_mode(wrrc_as_id);

  WRRC_MSG2_HIGH_OPT("function rrc_send_codec_info_on_activation_timer_expiry is called when ordered_codec_info = %d and current_codec_info = %d", ordered_codec_info, current_codec_info);
  ordered_codec_status_flag = rrc_get_ordered_codec_status(wrrc_as_id);

  if(!rrciho_is_gtow_ho_active(wrrc_as_id))
  {
    if((TRUE == ordered_codec_status_flag))
    {
      if(FALSE == rrc_is_voice_rab_present(wrrc_as_id) || (ordered_codec_info != current_codec_info))
      {
        /*Sending codec info to CM, and now we are sending the sampling rate as AMR NB by default*/
        rrc_send_codec_info_to_cm(wrrc_as_id,ordered_codec_info,RRC_SAMPLING_RATE_NB);
      }
      else
      {
        WRRC_MSG3_HIGH("is_voice_rab %d, ordered codec %d current codec %d",rrc_is_voice_rab_present(wrrc_as_id),ordered_codec_info,current_codec_info);
      }
      rrc_set_ordered_codec_status(wrrc_as_id, FALSE);
    }
    else
    {
      WRRC_MSG2_MED("Activation type is %d, ordered config status %d",ordered_config_ptr->act_time_type,ordered_codec_status_flag);
    }
  }
}
/*===========================================================================

FUNCTION rrc_register_rrc_codec_ho_info_cb_sub

DESCRIPTION
  This function is the API to register the function pointer for CM call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_register_rrc_codec_ho_info_cb_sub
(
  sys_modem_as_id_e_type wrrc_as_id,
  RRC_CODEC_HO_INFO_CB_FUNC_TYPE *cb_ptr
)
{
  RRC_VALIDATE_SUB_ID(wrrc_as_id);
  rrcdata_codec_info.rrc_codec_ho_info_cb_func = cb_ptr;
  WRRC_MSG1_HIGH("CM  registering %x with RRC",cb_ptr);
}

/*===========================================================================

FUNCTION rrc_register_rrc_codec_ho_info_cb

DESCRIPTION
  This function is the API to register the function pointer for CM call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_register_rrc_codec_ho_info_cb
(
  RRC_CODEC_HO_INFO_CB_FUNC_TYPE *cb_ptr
)
{
  /*RRC_VALIDATE_FUNC_CALL();*/
  rrc_register_rrc_codec_ho_info_cb_sub(SYS_MODEM_AS_ID_1, cb_ptr);
}


/*===========================================================================

FUNCTION rrc_deregister_rrc_codec_ho_info_cb_sub

DESCRIPTION
  This function is the API to de-register the function pointer for CM call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_deregister_rrc_codec_ho_info_cb_sub (sys_modem_as_id_e_type wrrc_as_id)
{
  RRC_VALIDATE_SUB_ID(wrrc_as_id);
  if(rrcdata_codec_info.rrc_codec_ho_info_cb_func != NULL)
  {
    rrcdata_codec_info.rrc_codec_ho_info_cb_func = NULL;
    WRRC_MSG0_HIGH("CM  de-registering with RRC");
  }
}

/*===========================================================================

FUNCTION rrc_deregister_rrc_codec_ho_info_cb

DESCRIPTION
  This function is the API to de-register the function pointer for CM call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_deregister_rrc_codec_ho_info_cb (void)
{
  /*RRC_VALIDATE_FUNC_CALL();*/
  rrc_deregister_rrc_codec_ho_info_cb_sub(SYS_MODEM_AS_ID_1);
}
/*===========================================================================

FUNCTION rrc_send_hho_cmd_to_cm

DESCRIPTION


  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void rrc_send_hho_cmd_to_cm(sys_modem_as_id_e_type wrrc_as_id,sys_codec_ho_cmd cmd,sys_ho_type_e_type ho_type)
{
  sys_codec_ho_info sys_mode_codec_ho_info = {0};

  /* The structure sys_codec_ho_info needs additional field as id */
#if defined (FEATURE_DUAL_SIM) || defined (FEATURE_DUAL_WCDMA)
  sys_mode_codec_ho_info.as_id = rrc_get_as_id(wrrc_as_id);
#endif

  WRRC_MSG3_HIGH("Trying to send HHO cmd to CM: %d rrc_codec_ho_info_cb_func status 0x%x, is_hho_started status %d",cmd, rrcdata_codec_info.rrc_codec_ho_info_cb_func, rrcdata_codec_info.is_hho_started);
  if((cmd != SYS_HO_START)&&(rrcdata_codec_info.is_hho_started))
  {
    rrcdata_codec_info.is_hho_started = FALSE;
    if(rrcdata_codec_info.rrc_codec_ho_info_cb_func != NULL)
    {
      sys_mode_codec_ho_info.cmd = cmd;
      sys_mode_codec_ho_info.ho_type = ho_type;
      rrcdata_codec_info.rrc_codec_ho_info_cb_func(sys_mode_codec_ho_info);
    }
  }
  else if (cmd == SYS_HO_START)
  {
      rrcdata_codec_info.is_hho_started = TRUE;
    if(rrcdata_codec_info.rrc_codec_ho_info_cb_func != NULL)
    {
      sys_mode_codec_ho_info.cmd = cmd;
      sys_mode_codec_ho_info.ho_type = ho_type;
      rrcdata_codec_info.rrc_codec_ho_info_cb_func(sys_mode_codec_ho_info);
    }
  }
}

/*===========================================================================

FUNCTION rrc_send_codec_info_to_cm

DESCRIPTION


  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void rrc_send_codec_info_to_cm(sys_modem_as_id_e_type wrrc_as_id,uint32 codec,uint32 amr_nb_sampling_rate)
{
  sys_codec_ho_info sys_mode_codec_ho_info;

  sys_mode_codec_ho_info.cmd = SYS_SPEECH_CODEC_INFO;  

/* The structure sys_codec_ho_info needs additional field as id */
#if defined (FEATURE_DUAL_SIM) || defined (FEATURE_DUAL_WCDMA)
  sys_mode_codec_ho_info.as_id = rrc_get_as_id(wrrc_as_id);
#endif

  if(codec == WCDMA_IVOCODER_ID_AMR)
  {
    sys_mode_codec_ho_info.speech_codec = SYS_SPEECH_CODEC_AMR_NB;
    sys_mode_codec_ho_info.speech_enc_samp_freq = amr_nb_sampling_rate;
  }
  else if(codec == WCDMA_IVOCODER_ID_AMRWB)
  {
    sys_mode_codec_ho_info.speech_codec = SYS_SPEECH_CODEC_AMR_WB;
    sys_mode_codec_ho_info.speech_enc_samp_freq = RRC_SAMPLING_RATE_WB;
  }
  else
  {
    WRRC_MSG1_ERROR("Invalid codec type received %d ",codec);
    return;
  }
  if(rrcdata_codec_info.rrc_codec_ho_info_cb_func != NULL)
  {    
    rrcdata_codec_info.rrc_codec_ho_info_cb_func(sys_mode_codec_ho_info);
    WRRC_MSG1_HIGH("Sent codec info to CM %d ",codec);
  }
}
#endif
/*===========================================================================
FUNCTION rrc_get_predefined_po3_status

DESCRIPTION
  This function returns TRUE if prefined PO3 is configured by NW especially for inter RAT HO cases.
  This function is valid for DCH state only.
 

DEPENDENCIES
  None

RETURN VALUE
  TRUE: Predefined PO3 is configured
  FALSE: If PO3 is signalled from NW OTA

SIDE EFFECTS
  None
===========================================================================*/
boolean rrc_get_predefined_po3_status(sys_modem_as_id_e_type wrrc_as_id)
{

  ordered_config_type *config_ptr;
  
  if(config_ptr_in_use == ORDERED_CONFIG)
  {
    config_ptr = ordered_config_ptr;
  }
  else
  {
    config_ptr = current_config_ptr;
    WRRC_MSG0_ERROR("get predefined called when OC is not set");
  }
  
  WRRC_MSG1_MED("predefined PO3 status  %d",config_ptr->predef_po3_status);
  return(config_ptr->predef_po3_status);

}

/*===========================================================================
FUNCTION rrc_set_predefined_po3_status

DESCRIPTION
  This function sets the predefined po3 status
 

DEPENDENCIES
  This function should be called if OC is set

RETURN VALUE
 None
SIDE EFFECTS
  None
===========================================================================*/
void rrc_set_predefined_po3_status(sys_modem_as_id_e_type wrrc_as_id,boolean status)
{
  if((status == TRUE) || (ordered_config_ptr->predef_po3_status != status))
  {
    WRRC_MSG1_HIGH("set predefined PO3 status  %d",status);
  }
  ordered_config_ptr->predef_po3_status = status;
}

/*===========================================================================
 FUNCTION     rrc_crash_debug_set_rrc_to_mm_cmd_id

 DESCRIPTION
   This functions sets the command sent from RRC to MM into the
   crash debug info

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_crash_debug_set_rrc_to_mm_cmd_id(sys_modem_as_id_e_type wrrc_as_id,uint32 cmd_id)
{
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_index < RRC_CRASH_DEBUG_MM_CMD_CNT)
  {
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_index].cmd_id = (rrc_to_mm_sink_e_type)cmd_id;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_index].timeticks = (int64)timetick_get();
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_index + 1) % RRC_CRASH_DEBUG_MM_CMD_CNT;
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
}

/*===========================================================================
 FUNCTION     rrc_crash_debug_set_rrc_to_rr_cmd_id

 DESCRIPTION
   This functions sets the command sent from RRC to RR into the
   crash debug info

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_crash_debug_set_rrc_to_rr_cmd_id(sys_modem_as_id_e_type wrrc_as_id,uint32 cmd_id)
{
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_index<RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
  {
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_index].cmd_id = (rrc_rr_cmd_e_type)cmd_id;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_index].timeticks = (int64)timetick_get();
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;

}

/*===========================================================================
FUNCTION     rrc_crash_debug_set_mm_to_rrc_cmd_id
  
DESCRIPTION
   This functions sets the command sent from MM to RRC into the
   crash debug info

DEPENDENCIES
   None.

PARAMETERS
   None.
  
RETURN VALUE
   None.
  
SIDE EFFECTS
   None
  ===========================================================================*/
void rrc_crash_debug_set_mm_to_rrc_cmd_id(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr)
{
/* Skip saving RRC_LAI_REJECT_LIST_IND and RRC_DATA_REQ*/
 if((cmd_ptr->cmd_hdr.cmd_id == RRC_LAI_REJECT_LIST_IND) ||
    (cmd_ptr->cmd_hdr.cmd_id == RRC_DATA_REQ))  
 {
   rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
   return;
 }
 if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index<RRC_CRASH_DEBUG_MM_CMD_CNT)
  {
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index].cmd_id = cmd_ptr->cmd_hdr.cmd_id;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index].timeticks = (int64)timetick_get();
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index].proc_id = (uint8)RRC_PROCEDURE_NONE;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index].substate = RRC_INVALID_PROC_SUBSTATE;
    /* Save the first byte of the cmd */
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index].status = (uint8)cmd_ptr->cmd.rrc_est_req.cause;
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index + 1) % RRC_CRASH_DEBUG_MM_CMD_CNT;
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = MM_TO_RRC_CMD;
}

/*===========================================================================
 FUNCTION     rrc_crash_debug_set_rr_to_rrc_cmd_id

 DESCRIPTION
   This functions sets the command sent from RR to RRC into the
   crash debug info

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_crash_debug_set_rr_to_rrc_cmd_id(sys_modem_as_id_e_type wrrc_as_id,rrc_rr_cmd_e_type cmd_id)
{
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
  {
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index].cmd_id = cmd_id;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index].timeticks = (int64)timetick_get();
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index].proc_id = (uint8)RRC_PROCEDURE_NONE;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index].substate = RRC_INVALID_PROC_SUBSTATE;    
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = RR_TO_RRC_CMD;
}
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
/*===========================================================================
 FUNCTION     rrc_qsh_log_rrc_to_mm_params

 DESCRIPTION
   This functions logs rrc to mm command params to QSH globals

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
static void wrrc_qsh_log_rrc_to_mm_params(sys_modem_as_id_e_type wrrc_as_id,mm_cmd_type * cmd_ptr)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_MM_CMD].data_ptr != NULL)
  {
    rrc_mm_u_type* rrc_mm_config;
    /* Take only uint16 from message id which is uint 32 */
    ((wrrc_qsh_metric_rrc_to_mm_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_MM_CMD].data_ptr)->msg_id = (qsh_rrc_mm_cmd_e_type)(cmd_ptr->cmd.hdr.message_id & 0xFFFF);
  
    /* Get the address of QSH config field to a local variable */
    rrc_mm_config = &(((wrrc_qsh_metric_rrc_to_mm_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_MM_CMD].data_ptr)->config);
    memset(rrc_mm_config, 0,sizeof(rrc_mm_u_type));
    switch(cmd_ptr->cmd.hdr.message_id)
    {
      case RRC_REL_IND:
        rrc_mm_config->rrc_rel_ind.rel_cause = cmd_ptr->cmd.rrc_rel_ind.rel_cause;
        break;
        
      case RRC_SYNC_IND:
        rrc_mm_config->rrc_sync_ind.cause = (uint8)(cmd_ptr->cmd.rrc_sync_ind.cause);
        rrc_mm_config->rrc_sync_ind.cn_domain_id = (boolean)(cmd_ptr->cmd.rrc_sync_ind.cn_domain_id);
        rrc_mm_config->rrc_sync_ind.ciphering_enabled = cmd_ptr->cmd.rrc_sync_ind.ciphering_enabled;
        break;
        
      case RRC_SERVICE_CNF:
        rrc_mm_config->rrc_service_cnf.service_status = (uint8)(cmd_ptr->cmd.rrc_service_cnf.service_status);
        rrc_mm_config->rrc_service_cnf.status = cmd_ptr->cmd.rrc_service_cnf.status;
        rrc_mm_config->rrc_service_cnf.wcdma_search_done = cmd_ptr->cmd.rrc_service_cnf.wcdma_search_done;
        rrc_mm_config->rrc_service_cnf.ue_oos = cmd_ptr->cmd.rrc_service_cnf.ue_oos;
        break;
        
      case RRC_SERVICE_IND:
        rrc_mm_config->rrc_service_ind.service_status = (uint8)(cmd_ptr->cmd.rrc_service_ind.service_status);
        rrc_mm_config->rrc_service_ind.acq_status = (uint8)(cmd_ptr->cmd.rrc_service_ind.acq_status);
        rrc_mm_config->rrc_service_ind.no_service_cause= (uint8)(cmd_ptr->cmd.rrc_service_ind.no_service_cause);
        break;
        
      case RRC_PAGE_IND:
        rrc_mm_config->rrc_page_ind.cn_domain_id = (boolean)(cmd_ptr->cmd.rrc_page_ind.cn_domain_id);
        rrc_mm_config->rrc_page_ind.paging_cause = (uint8)(cmd_ptr->cmd.rrc_page_ind.paging_cause);
        break;
        
      default:
        /* Leave out the IMT_H field Log next 4 bytes in the uint32 field later for config_id */
        WCDMA_MEMCPY(&(rrc_mm_config->gen_config),sizeof(qsh_gen_u32_config_type),((char*)(&(cmd_ptr->cmd))+ sizeof(IMH_T)),sizeof(qsh_gen_u32_config_type));
        break;
    }
  
    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_RRC_TO_MM_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  }
}
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif /* FEATURE_QSH_EVENT_NOTIFY_HANDLER */
/*===========================================================================
 FUNCTION     rrc_send_rrc_message_to_mm

 DESCRIPTION
   This functions sends the RRC message to MM command queue

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/

void rrc_send_rrc_message_to_mm(sys_modem_as_id_e_type wrrc_as_id,mm_cmd_type *cmd_ptr)
{
  WRRC_MSG1_HIGH("RRC_TO_MM: command id:%x",cmd_ptr->cmd.hdr.message_id);
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    if(cmd_ptr->cmd.hdr.message_id != RRC_DATA_IND)
    {
      rrc_crash_debug_set_rrc_to_mm_cmd_id(wrrc_as_id,cmd_ptr->cmd.hdr.message_id);
    }
  }
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  wrrc_qsh_log_rrc_to_mm_params(wrrc_as_id,cmd_ptr);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  /* WRRC_MSG1_HIGH_OPT("Dispatching out MM cmd rrc_to_mm_sink_e_type_value%d",cmd_ptr->cmd.hdr.message_id ); */
  mm_put_cmd(cmd_ptr);
}
/*===========================================================================

FUNCTION          rrc_query_mrab_status


DESCRIPTION

  This function checks if CS voice and PS RAB is present 

DEPENDENCIES

  None

RETURN VALUE

Boolean

SIDE EFFECTS

  None

===========================================================================*/
boolean rrc_query_mrab_status
(
  sys_modem_as_id_e_type wrrc_as_id
)
{
  return ((rrc_query_mac_rab_status(wrrc_as_id) == CS_PLUS_PS_RABS) ||(rrc_is_voice_rab_present(wrrc_as_id) && rrc_is_ps_domain_rab_present(wrrc_as_id)));
}
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
/*===========================================================================
FUNCTION rrc_qsh_map_ota_msg_id
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  wrrc_qsh_ota_msg_id
SIDE EFFECTS
  None

===========================================================================*/

static wrrc_qsh_ota_msg_id rrc_qsh_map_ota_msg_id(sys_modem_as_id_e_type wrrc_as_id, uint8 sdu_type, uint8 message_choice)
{ 
  wrrc_qsh_ota_msg_id ota_msg_id = WRRC_MESSAGETYPE_INVALID;
  
  switch(sdu_type)
  {
    
    case rrc_UL_CCCH_Message_PDU:
      switch (message_choice)
      {
        case T_rrc_UL_CCCH_MessageType_cellUpdate:
          ota_msg_id = WRRC_UL_CCCH_MESSAGETYPE_CELLUPDATE;
          break;
        case T_rrc_UL_CCCH_MessageType_rrcConnectionRequest:
          ota_msg_id = WRRC_UL_CCCH_MESSAGETYPE_RRCCONNECTIONREQUEST;
          break;
        case T_rrc_UL_CCCH_MessageType_uraUpdate:
          ota_msg_id =	WRRC_UL_CCCH_MESSAGETYPE_URAUPDATE;
          break;
        default:
          break;
      }
    break;
    
    case rrc_DL_CCCH_Message_PDU:
      switch (message_choice)
      {
        case T_rrc_DL_CCCH_MessageType_cellUpdateConfirm:
          ota_msg_id = WRRC_DL_CCCH_MESSAGETYPE_CELLUPDATECONFIRM;
          break;
        case T_rrc_DL_CCCH_MessageType_rrcConnectionReject:
          ota_msg_id = WRRC_DL_CCCH_MESSAGETYPE_RRCCONNECTIONREJECT;
          break;
        case T_rrc_DL_CCCH_MessageType_rrcConnectionRelease:
          ota_msg_id = WRRC_DL_CCCH_MESSAGETYPE_RRCCONNECTIONRELEASE;
          break;
        case T_rrc_DL_CCCH_MessageType_rrcConnectionSetup:
          ota_msg_id = WRRC_DL_CCCH_MESSAGETYPE_RRCCONNECTIONSETUP;
          break;
        case T_rrc_DL_CCCH_MessageType_uraUpdateConfirm:
          ota_msg_id = WRRC_DL_CCCH_MESSAGETYPE_URAUPDATECONFIRM;
          break;
        default:
          break;
      }
      break;
    
    case rrc_DL_DCCH_Message_PDU:
      switch (message_choice)
      {
        case T_rrc_DL_DCCH_MessageType_activeSetUpdate:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_ACTIVESETUPDATE;
          break;
        case T_rrc_DL_DCCH_MessageType_assistanceDataDelivery:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_ASSISTANCEDATADELIVERY;
          break;
        case T_rrc_DL_DCCH_MessageType_cellChangeOrderFromUTRAN:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_CELLCHANGEORDERFROMUTRAN;
          break;
        case T_rrc_DL_DCCH_MessageType_cellUpdateConfirm:			
          ota_msg_id =	WRRC_DL_DCCH_MESSAGETYPE_CELLUPDATECONFIRM;
          break;
        case T_rrc_DL_DCCH_MessageType_counterCheck:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_COUNTERCHECK;
          break;
        case T_rrc_DL_DCCH_MessageType_downlinkDirectTransfer:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_DOWNLINKDIRECTTRANSFER;
          break;
        case T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANCOMMAND_GSM;
          break;
        case T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_CDMA2000:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANCOMMAND_CDMA2000;
          break;
        case T_rrc_DL_DCCH_MessageType_measurementControl:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_MEASUREMENTCONTROL;
          break;
        case T_rrc_DL_DCCH_MessageType_pagingType2:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_PAGINGTYPE2;
          break;
        case T_rrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATION;
          break;
        case T_rrc_DL_DCCH_MessageType_physicalSharedChannelAllocation:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_PHYSICALSHAREDCHANNELALLOCATION;
          break;
        case T_rrc_DL_DCCH_MessageType_radioBearerReconfiguration:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATION;
          break;
        case T_rrc_DL_DCCH_MessageType_radioBearerRelease:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_RADIOBEARERRELEASE;
          break;
        case T_rrc_DL_DCCH_MessageType_radioBearerSetup:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_RADIOBEARERSETUP;
          break;
        case T_rrc_DL_DCCH_MessageType_rrcConnectionRelease:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_RRCCONNECTIONRELEASE;
          break;
        case T_rrc_DL_DCCH_MessageType_securityModeCommand:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_SECURITYMODECOMMAND;
          break;
        case T_rrc_DL_DCCH_MessageType_signallingConnectionRelease:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_SIGNALLINGCONNECTIONRELEASE;
          break;
        case T_rrc_DL_DCCH_MessageType_transportChannelReconfiguration:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATION;
          break;
        case T_rrc_DL_DCCH_MessageType_transportFormatCombinationControl:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_TRANSPORTFORMATCOMBINATIONCONTROL;
          break;
        case T_rrc_DL_DCCH_MessageType_ueCapabilityEnquiry:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_UECAPABILITYENQUIRY;
          break;
        case T_rrc_DL_DCCH_MessageType_ueCapabilityInformationConfirm:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_UECAPABILITYINFORMATIONCONFIRM;
          break;
        case T_rrc_DL_DCCH_MessageType_uplinkPhysicalChannelControl:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_UPLINKPHYSICALCHANNELCONTROL;
          break;
        case T_rrc_DL_DCCH_MessageType_uraUpdateConfirm:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_URAUPDATECONFIRM;
          break;
        case T_rrc_DL_DCCH_MessageType_utranMobilityInformation:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_UTRANMOBILITYINFORMATION;
          break;
        case T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GERANIu:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANCOMMAND_GERANIU;
          break;
        case T_rrc_DL_DCCH_MessageType_mbmsModifiedServicesInformation:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_MBMSMODIFIEDSERVICESINFORMATION;
          break;
        case T_rrc_DL_DCCH_MessageType_etwsPrimaryNotificationWithSecurity:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_ETWSPRIMARYNOTIFICATIONWITHSECURITY;
          break;
        case T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANCOMMAND_EUTRA;
          break;
        case T_rrc_DL_DCCH_MessageType_ueInformationRequest:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_UEINFORMATIONREQUEST;
          break;
        case T_rrc_DL_DCCH_MessageType_loggingMeasurementConfiguration:
          ota_msg_id = WRRC_DL_DCCH_MESSAGETYPE_LOGGINGMEASUREMENTCONFIGURATION;
          break; 
        default:
          break;
      }
      break;
    
    case rrc_UL_DCCH_Message_PDU:
      switch (message_choice)
      {
        case T_rrc_UL_DCCH_MessageType_activeSetUpdateComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATECOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_activeSetUpdateFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATEFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_cellChangeOrderFromUTRANFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_CELLCHANGEORDERFROMUTRANFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_counterCheckResponse:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_COUNTERCHECKRESPONSE;
          break;
        case T_rrc_UL_DCCH_MessageType_handoverToUTRANComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_HANDOVERTOUTRANCOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_initialDirectTransfer:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_INITIALDIRECTTRANSFER;
          break;
        case T_rrc_UL_DCCH_MessageType_handoverFromUTRANFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_HANDOVERFROMUTRANFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_measurementControlFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_MEASUREMENTCONTROLFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_measurementReport:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_MEASUREMENTREPORT;
          break;
        case T_rrc_UL_DCCH_MessageType_physicalChannelReconfigurationComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATIONCOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_physicalChannelReconfigurationFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATIONFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_radioBearerReconfigurationComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATIONCOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_radioBearerReconfigurationFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATIONFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_radioBearerReleaseComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRELEASECOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_radioBearerReleaseFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRELEASEFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_radioBearerSetupComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERSETUPCOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_radioBearerSetupFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERSETUPFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_rrcConnectionReleaseComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RRCCONNECTIONRELEASECOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_rrcConnectionSetupComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RRCCONNECTIONSETUPCOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_rrcStatus:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_RRCSTATUS;
          break;
        case T_rrc_UL_DCCH_MessageType_securityModeComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_SECURITYMODECOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_securityModeFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_SECURITYMODEFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_signallingConnectionReleaseIndication:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_SIGNALLINGCONNECTIONRELEASEINDICATION;
          break;
        case T_rrc_UL_DCCH_MessageType_transportChannelReconfigurationComplete:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATIONCOMPLETE;
          break;
        case T_rrc_UL_DCCH_MessageType_transportChannelReconfigurationFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATIONFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_transportFormatCombinationControlFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTFORMATCOMBINATIONCONTROLFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_ueCapabilityInformation:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_UECAPABILITYINFORMATION; 
          break;
        case T_rrc_UL_DCCH_MessageType_uplinkDirectTransfer:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_UPLINKDIRECTTRANSFER;
          break;
        case T_rrc_UL_DCCH_MessageType_utranMobilityInformationConfirm:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_UTRANMOBILITYINFORMATIONCONFIRM;
          break;
        case T_rrc_UL_DCCH_MessageType_utranMobilityInformationFailure:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_UTRANMOBILITYINFORMATIONFAILURE;
          break;
        case T_rrc_UL_DCCH_MessageType_mbmsModificationRequest:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_MBMSMODIFICATIONREQUEST;
          break;
        case T_rrc_UL_DCCH_MessageType_ul_DCCH_MessageType_ext:
          ota_msg_id = WRRC_UL_DCCH_MESSAGETYPE_UL_DCCH_MESSAGETYPE_EXT;
          break;
      }
      break;
    case rrc_BCCH_FACH_Message_PDU:
      switch (message_choice)
      {
        case T_rrc_BCCH_FACH_MessageType_systemInformationChangeIndication:
          ota_msg_id = WRRC_BCCH_FACH_MESSAGETYPE_SYSTEMINFORMATIONCHANGEINDICATION;
          break;
        default:
          break;
      }
      break;
    case rrc_BCCH_BCH_Message_PDU:
      switch (message_choice)
      {
        case T_rrc_BCCH_FACH_MessageType_systemInformation:
          ota_msg_id = WRRC_BCCH_FACH_MESSAGETYPE_SYSTEMINFORMATION;
          break;
        default:
          break;
      }
      break;
    case rrc_PCCH_Message_PDU:
      switch (message_choice)
      {
        case T_rrc_PCCH_MessageType_pagingType1:
          ota_msg_id = WRRC_PCCH_MESSAGETYPE_PAGINGTYPE1;
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
  return ota_msg_id;
}
#endif
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_misc_cmds
  
   DESCRIPTION
       This function saves the misc commands to rrc_crash_debug_info global
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     as_id, cmd_id
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void rrc_crash_debug_save_misc_cmds(sys_modem_as_id_e_type wrrc_as_id,misc_cmds_enum_type cmd_id)
{
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_misc_cmds.index < RRC_CRASH_DEBUG_MISC_CMD_CNT)
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_misc_cmds.misc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_misc_cmds.index].cmd_id = cmd_id;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_misc_cmds.misc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_misc_cmds.index].timeticks = (int64)timetick_get();
    }
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_misc_cmds.index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_misc_cmds.index + 1) % RRC_CRASH_DEBUG_MISC_CMD_CNT;
  }
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_log_pkt_info

   DESCRIPTION
       This function saves the pased OTA into global. Before saving if already another OTA is present
       that has to be freed.
  
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     OTA pointer, OTA lengh, OTA type
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_log_pkt_info
(
  sys_modem_as_id_e_type wrrc_as_id,
  uint8* log_data_ptr, 
  uint16 length, 
  log_pkt_e_type log_pkt_type
)
{
  if(length == 0)
  {
    /* Size of log packet passed is 0. Return from here */
    return;
  }
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    uint8 index = rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_log_pkt.index;
    if(index < RRC_CRASH_DEBUG_LOG_PKT_CNT)
    {
      rrc_crash_debug_log_pkt_store_type* log_store_ptr;
      log_store_ptr = &(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_log_pkt.log[index]);
      
      /* Free the current pointer if it is not NULL */
      if(log_store_ptr->log_data_ptr != NULL)
      {
        rrc_free(log_store_ptr->log_data_ptr);
      }

      /* Allocate new memory of required length */
      log_store_ptr->log_data_ptr = (uint8*)rrc_malloc(length);

      /* Copy the log packet data to the new pointer */
      WCDMA_MEMCPY(log_store_ptr->log_data_ptr,
                   length,
                   log_data_ptr,
                   length);
      log_store_ptr->timeticks = (int64)timetick_get();
      log_store_ptr->length = length;
      log_store_ptr->log_pkt_type = log_pkt_type;
    }
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_log_pkt.index = (index + 1) % RRC_CRASH_DEBUG_LOG_PKT_CNT;
  }
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_ota
  
   DESCRIPTION
       This function saves the pased OTA into global. Before saving if already another OTA is present
       that has to be freed.
  
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     OTA pointer, OTA lengh, OTA type
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_ota(sys_modem_as_id_e_type wrrc_as_id,uint8* sdu, uint16 sdu_length, rrc_crash_debug_ota_pdu_type pdu_type,uint8 msg_type, 
                              rrc_proc_e_type proc_id)
{
  uint8 *debug_index;
  rrc_crash_debug_ota *ota_ptr;

#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  /* Consider only meas complete and asu complete in if case, failure msg type will go to else case but will not be logged */
  if((((pdu_type == RRC_DCCH_UL_SDU) && (msg_type == T_rrc_UL_DCCH_MessageType_measurementReport)) ||
  ((pdu_type == RRC_DCCH_UL_SDU) && (msg_type == T_rrc_UL_DCCH_MessageType_activeSetUpdateComplete)) ||
  ((pdu_type == RRC_DCCH_UL_SDU) && (msg_type == T_rrc_UL_DCCH_MessageType_measurementControlFailure)) ||
  ((pdu_type == RRC_DCCH_UL_SDU) && (msg_type == T_rrc_UL_DCCH_MessageType_activeSetUpdateFailure))) && 
  (uplink_ota.ul_ota_type == RRC_OTA_INVALID))
  {
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->msg_id =
      rrc_qsh_map_ota_msg_id(wrrc_as_id, rrc_UL_DCCH_Message_PDU, msg_type);
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_MEAS_ASU_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
    }
  }
  else if(((pdu_type == RRC_DCCH_UL_SDU) || (pdu_type == RRC_CCCH_UL_SDU)) && (uplink_ota.ul_ota_type == RRC_OTA_INVALID))
  {
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->msg_id =  
      rrc_qsh_map_ota_msg_id(wrrc_as_id, ((pdu_type == RRC_DCCH_UL_SDU)? rrc_UL_DCCH_Message_PDU :rrc_UL_CCCH_Message_PDU), msg_type);
      
      /* All UL messages in send UL sdu are committed here except failure OTAs */
      /* Failure OTA will be committed after a corresponding DL OTA is committed @ dispatcher */
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
    }
  }
#endif
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    if(pdu_type == RRC_DCCH_UL_SDU && (msg_type == T_rrc_UL_DCCH_MessageType_measurementReport))
    {
       /* Do not save Measurement Reports */
       return; 
    }
    /*Save the following OTAs in a different buffer as these are quite frequent*/
      /* 1. Active set update */
      /* 2. Active set Update complete */
      /* 3. Measurement control Message*/
    else if((pdu_type == RRC_DCCH_DL_SDU && ((msg_type == T_rrc_DL_DCCH_MessageType_activeSetUpdate)||(msg_type == T_rrc_DL_DCCH_MessageType_measurementControl))) ||
       (pdu_type == RRC_DCCH_UL_SDU && (msg_type == T_rrc_UL_DCCH_MessageType_activeSetUpdateComplete)))
      {
      debug_index = &rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.meas_asu_index;
      ota_ptr = &rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.meas_asu_message[*debug_index];         
    }
    else 
        {
      debug_index = &rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.rrc_crash_debug_index;
      ota_ptr = &rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[*debug_index];
        }
    if((debug_index != NULL) && (*debug_index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT) && (ota_ptr != NULL))
    {
      if(ota_ptr->rrc_crash_debug_otas != NULL)
      {
        rrc_free(ota_ptr->rrc_crash_debug_otas);
      }
      ota_ptr->rrc_crash_debug_otas = (uint8 *) rrc_malloc(sdu_length);
      WCDMA_MEMCPY(ota_ptr->rrc_crash_debug_otas,sdu_length,sdu,sdu_length);
      ota_ptr->sdu_type = pdu_type;
      ota_ptr->rrc_ota_length = sdu_length;
      ota_ptr->timeticks = (int64)timetick_get();
      ota_ptr->proc_id = (uint8)proc_id;
      ota_ptr->substate = rrc_get_current_substate_for_proc(wrrc_as_id,proc_id);
      /* Increment the index */
      *debug_index = (*debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
    }
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_free_log_pkt_info
  
   DESCRIPTION
       This funtion free the memory allocated for all the log packets

   DEPENDENCIES
     None.
  
   PARAMETERS
     None.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_free_log_pkt_info(sys_modem_as_id_e_type wrrc_as_id)
{
  uint8 idx = 0;

  for(idx = 0; idx < RRC_CRASH_DEBUG_LOG_PKT_CNT; idx++)
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_log_pkt.log[idx].log_data_ptr != NULL)
    {
      rrc_free(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_log_pkt.log[idx].log_data_ptr);
    }
  }
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_free_ota_msgs
  
   DESCRIPTION
       This funtion free the memory allocated for all the OTA messages
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     None.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_free_ota_msgs(sys_modem_as_id_e_type wrrc_as_id)
{
  uint8 idx = 0;

  for(idx = 0; idx < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT; idx++)
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[idx].rrc_crash_debug_otas != NULL)
    {
      rrc_free(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[idx].rrc_crash_debug_otas);
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[idx].rrc_crash_debug_otas = NULL;
    }
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.meas_asu_message[idx].rrc_crash_debug_otas != NULL)
    {
      rrc_free(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.meas_asu_message[idx].rrc_crash_debug_otas);
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.meas_asu_message[idx].rrc_crash_debug_otas = NULL;
    }
  }
}


/*===========================================================================
FUNCTION rrc_get_wcdma_rrc_systematic_logging_enabled_nv

DESCRIPTION
  This  function gets the NV item NV_WCDMA_RRC_SYSTEMATIC_LOGGING_ENABLED and stores it in 
  rrc global variable 'rrc_default_cfg_in_cell_fach_nv'

DEPENDENCIES
  None.

RETURN VALUE
  None.
===========================================================================*/
/*
void rrc_get_wcdma_rrc_systematic_logging_enabled_nv
(
  void
)
{

  if(NV_DONE_S == (rrc_get_nv( NV_WCDMA_RRC_SYSTEMATIC_LOGGING_ENABLED, 
                                  (nv_item_type *) &wcdma_rrc_systematic_logging_enabled_nv)))
  {
    WRRC_MSG0_HIGH("NV_WCDMA_RRC_SYSTEMATIC_LOGGING_ENABLED (NV item number %d) is %d",
        NV_WCDMA_RRC_SYSTEMATIC_LOGGING_ENABLED,wcdma_rrc_systematic_logging_enabled_nv,0);
  }
  else
  {
    WRRC_MSG0_ERROR("NV_WCDMA_RRC_SYSTEMATIC_LOGGING_ENABLED not created in NV yet");
    wcdma_rrc_systematic_logging_enabled_nv = TRUE; 
  }
  return;
} 
*/

/*===========================================================================
   FUNCTION     rrc_crash_debug_save_l1_to_rrc_cmds
  
   DESCRIPTION
       This funtion save l1 to rrc commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     rrc_cmd_e_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_l1_to_rrc_cmds(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr)
{

  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    /* Skip saving RRC_CPHY_MODEM_STATISTICS_RSP */
    if(cmd_ptr->cmd_hdr.cmd_id != RRC_CPHY_MODEM_STATISTICS_RSP)
    {
      if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
      {
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index].cmd_id = cmd_ptr->cmd_hdr.cmd_id;
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index].timeticks = (int64)timetick_get();
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index].proc_id = (uint8)RRC_PROCEDURE_NONE;
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index].substate = RRC_INVALID_PROC_SUBSTATE;
        /* Save the first byte of the cmd */
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index].status = (uint8)cmd_ptr->cmd.idle_cnf.status;        
      }
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = L1_TO_RRC_CMD;
    }
    else
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
    }
  }
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_l2_to_rrc_cmds
  
   DESCRIPTION
       This funtion save l2 to rrc commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     rrc_cmd_e_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_l2_to_rrc_cmds(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr)
{

  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    /*Skip saving RRC_CMAC_STATUS_IND */
    if(cmd_ptr->cmd_hdr.cmd_id  != RRC_CMAC_STATUS_IND)
    {
      if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
      { 
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index].cmd_id = cmd_ptr->cmd_hdr.cmd_id ;
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index].timeticks = (int64)timetick_get();
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index].proc_id = RRC_PROCEDURE_NONE;
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index].substate = RRC_INVALID_PROC_SUBSTATE;
        /* Currently we are not saving status for L2 commands */
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index].status = (uint8)cmd_ptr->cmd.crlc_ul_hfn_cnf.nchan;
        
      }
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = L2_TO_RRC_CMD;
    }
    else
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
    }
  }
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_internal_cmds
  
   DESCRIPTION
       This funtion saves rrc internal commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     rrc_cmd_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_internal_cmds(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr)
{
  
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    if(cmd_ptr->cmd_hdr.cmd_id == RRC_LLC_CMD_REQ)
    {
      switch(cmd_ptr->cmd.rrc_llc_req.cmd_dest)
      {
        case LAYER1_PHY:
          rrc_crash_debug_save_l1_llc_cmds(wrrc_as_id,cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id);
          break;
        case LAYER2_MAC_UL:
        case LAYER2_RLC_UL:
          rrc_crash_debug_save_l2_llc_ul_cmds(wrrc_as_id,cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id);
          break;
        case LAYER2_MAC_DL:
        case LAYER2_RLC_DL:
          rrc_crash_debug_save_l2_llc_dl_cmds(wrrc_as_id,cmd_ptr->cmd.rrc_llc_req.l2_dl_cmd.cmd_id);
          break;
        default:
          break;
      }
    }         
    else if(cmd_ptr->cmd_hdr.cmd_id != RRC_DOWNLINK_SDU_IND)
    {
      /* Skip saving frequently occuring commands */
      if(cmd_ptr->cmd_hdr.cmd_id != RRC_TIMER_FOR_RSSI_BER_EXPIRED_IND &&
         cmd_ptr->cmd_hdr.cmd_id != RRC_TEN_MIN_SIB_TIMER_UPDATE_IND 
#ifdef FEATURE_FEMTO_CSG
         && cmd_ptr->cmd_hdr.cmd_id != RRC_CSG_ASF_SRCH_TIMER_EXPIRY_IND 
#endif
#if defined(FEATURE_WCDMA_QSH_EVENT_METRIC) || defined(FEATURE_WCDMA_QSH_DUMP)
         && cmd_ptr->cmd_hdr.cmd_id != RRC_QSH_PROCESS_CMD
#endif
      )
      {
/* 
   Unlock by timer expiry is a very frequent command. So to avoid this command from occupying all the buffers we store only the first and the last
   unlock by commands if they come back to back.
   
   check if 
         previous two indices hold the same command then copy the current instance to the previous instance
         else
         save the command to new index
*/
#ifdef FEATURE_DUAL_SIM
        if(cmd_ptr->cmd_hdr.cmd_id == RRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND)
        {
          /* Previous Index */
          uint8 prev_idx = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index + RRC_CRASH_DEBUG_INTERNAL_CMD_CNT -1) %
                            RRC_CRASH_DEBUG_INTERNAL_CMD_CNT;
          /* Previuos to previous index */
          uint8 next_prev_idx = (prev_idx + RRC_CRASH_DEBUG_INTERNAL_CMD_CNT - 1) % RRC_CRASH_DEBUG_INTERNAL_CMD_CNT;
          /* Check if previous two indices hold the same command */
          if((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[prev_idx].cmd_id == RRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND)
             &&
             (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[next_prev_idx].cmd_id == RRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND))
          {
            /* If Yes set the current index as previous index so that previous index is overwritten */
            rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index = prev_idx;
          }
        }
#endif
        if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index < RRC_CRASH_DEBUG_INTERNAL_CMD_CNT)
        {
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index].cmd_id = cmd_ptr->cmd_hdr.cmd_id;
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index].timeticks = (int64)timetick_get();
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index].proc_id = (uint8)RRC_PROCEDURE_NONE;
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index].substate = RRC_INVALID_PROC_SUBSTATE;
          /* Save the first byte of the cmd */
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index].status = (uint8)cmd_ptr->cmd.downlink_sdu_ind.lc_id;
        }
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INT_CMD;
        if(cmd_ptr->cmd_hdr.cmd_id == RRC_STATE_CHANGE_IND)
        {
         /* As state change ind is processed by many procedures, save the proc id/substate as none */
         rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
        }
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index + 1) % RRC_CRASH_DEBUG_INTERNAL_CMD_CNT;
      }
      else
      {
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
      }
    }
  }
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_rrc_to_l2_ul_cmds
  
   DESCRIPTION
       This funtion saves rrc to l2 commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     wcdma_l2_ul_cmd_enum_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_rrc_to_l2_ul_cmds(sys_modem_as_id_e_type wrrc_as_id,wcdma_l2_ul_cmd_enum_type cmd_id)
{
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
  { 
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index].dest = L2_UL;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index].cmds.l2_ul_cmd = cmd_id;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index].timeticks = (int64)timetick_get();
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_rrc_to_l2_ul_cmds
  
   DESCRIPTION
       This funtion saves rrc to l2 commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     wcdma_l2_dl_cmd_enum_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_rrc_to_l2_dl_cmds(sys_modem_as_id_e_type wrrc_as_id,wcdma_l2_dl_cmd_enum_type cmd_id)
{
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
  { 
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index].dest = L2_DL;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index].cmds.l2_dl_cmd = cmd_id;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index].timeticks = (int64)timetick_get();
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.rrc_to_l2_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_rrc_to_l1_cmds
  
   DESCRIPTION
       This funtion saves rrc to l1 commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     l1_cmd_enum_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_rrc_to_l1_cmds(sys_modem_as_id_e_type wrrc_as_id,l1_cmd_enum_type cmd_id)
{
  if(cmd_id != CPHY_SIB_SCHED_CHANGE_IND)
  {

/* 
  CPHY_GET_MODEM_STATISTICS_REQ is a very frequent command. So to avoid this command from occupying all the buffers we store only the first and the last
  instance ofcommands if they come back to back.

  check 
       if 
       previous two indices hold the same command then copy the current instance to the previous instance
       else
       save the command to new index
*/

    if(cmd_id == CPHY_GET_MODEM_STATISTICS_REQ)
    {
      /* Previous index */
      uint8 prev_idx = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_index + RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT -1) %
                            RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
      /* Previous to previous index */
      uint8 next_prev_idx = (prev_idx + RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT - 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;

      /* Check if previous two indices hold the same command */
      if((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_cmds[prev_idx].cmd_id == CPHY_GET_MODEM_STATISTICS_REQ)
          &&
         (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_cmds[next_prev_idx].cmd_id == CPHY_GET_MODEM_STATISTICS_REQ))
      {
        /* If Yes set the current index as previous index so that the previous index gets overwritten */
        rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_index = prev_idx;
      }
    }
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_index].cmd_id = cmd_id;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_index].timeticks = (int64)timetick_get();
    }
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_lte_to_rrc_cmds
  
   DESCRIPTION
       This funtion saves lte to rrc commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     uint32.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_lte_to_rrc_cmds(sys_modem_as_id_e_type wrrc_as_id,uint32 cmd_id)
{

  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {

    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index < RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT)
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index].cmd_id = cmd_id;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index].timeticks = (int64)timetick_get();
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index].proc_id = (uint8)RRC_PROCEDURE_NONE;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index].substate = RRC_INVALID_PROC_SUBSTATE;
    }
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index + 1) % RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = LTE_TO_RRC_CMD;
  }
}
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
/*===========================================================================
   FUNCTION     wrrc_qsh_log_rrc_to_lte_params
  
   DESCRIPTION
       This funtion logs rrc to lte parameters to QSH globals
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     uint32.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
static void wrrc_qsh_log_rrc_to_lte_params(sys_modem_as_id_e_type wrrc_as_id, uint32 cmd_id)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_LTE_CMD].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_rrc_to_lte_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_LTE_CMD].data_ptr)->msg_id = (qsh_lte_rrc_cmd_e_type)(cmd_id & 0xFFFF);

    /* Config field is not needed for RRC to LTE cmds as we dont see anything important field important here.
       Command ID should be good enough for now to debug 
    */

    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_RRC_TO_LTE_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  
  }
}
#endif
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_rrc_to_lte_cmds
  
   DESCRIPTION
       This funtion saves rrc to lte commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     uint32.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_rrc_to_lte_cmds(sys_modem_as_id_e_type wrrc_as_id,uint32 cmd_id)
{

  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {

    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_index < RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT)
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_index].cmd_id = cmd_id;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_index].timeticks = (int64)timetick_get();
    }
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_index + 1) % RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
  }
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  wrrc_qsh_log_rrc_to_lte_params(wrrc_as_id,cmd_id);
#endif
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_l1_llc_cmds
  
   DESCRIPTION
       This funtion saves l1 llc commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     l1_cmd_enum_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void rrc_crash_debug_save_l1_llc_cmds(sys_modem_as_id_e_type wrrc_as_id,l1_cmd_enum_type cmd_id)
{
  /* Skip saving CPHY_SIB_SCHED_CHANGE_IND  */
  if(cmd_id != CPHY_SIB_SCHED_CHANGE_IND
     &&
     cmd_id != CPHY_GET_MODEM_STATISTICS_REQ)  
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].dest = L1_PHY;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].cmds.l1_llc_cmd = cmd_id;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].timeticks = (int64)timetick_get();
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].substate = rrcllc_get_current_substate(wrrc_as_id);
    }
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_l2_llc_ul_cmds
  
   DESCRIPTION
       This funtion saves l2 llc commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     wcdma_l2_ul_cmd_enum_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_l2_llc_ul_cmds(sys_modem_as_id_e_type wrrc_as_id,wcdma_l2_ul_cmd_enum_type cmd_id)
{
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
  {
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].dest = L2_UL;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].cmds.l2_llc_ul_cmd = cmd_id;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].timeticks = (int64)timetick_get();
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].substate = rrcllc_get_current_substate(wrrc_as_id);
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_l2_llc_dl_cmds
  
   DESCRIPTION
       This funtion saves l2 llc commands in rrc_crash_debug_info
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     wcdma_l2_dl_cmd_enum_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_save_l2_llc_dl_cmds(sys_modem_as_id_e_type wrrc_as_id,wcdma_l2_dl_cmd_enum_type cmd_id)
{
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
  {
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].dest = L2_DL;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].cmds.l2_llc_dl_cmd = cmd_id;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].timeticks = (int64)timetick_get();
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index].substate = rrcllc_get_current_substate(wrrc_as_id);
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_remove_previous_ota
  
   DESCRIPTION
       This function deletes the previously saved OTA. This is used to remove saved OTAs in crash debug
   global. 
  
   DEPENDENCIES
     None.
  
   PARAMETERS
    None
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void rrc_crash_debug_remove_previous_ota(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Decrement the index to free the previous ota */
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.rrc_crash_debug_index = 
  ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.rrc_crash_debug_index+RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT-1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT);
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.rrc_crash_debug_index].rrc_crash_debug_otas != NULL)
  {
    rrc_free(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.rrc_crash_debug_index].rrc_crash_debug_otas);
  }
  rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.rrc_crash_debug_index].timeticks = 0;
}
/*===========================================================================
   FUNCTION     wrrc_qsh_log_rrc_to_l1_params
  
   DESCRIPTION
       This function logs rrc to l1 command parameters to QSH globals
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     l1_ext_cmd_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void wrrc_qsh_log_rrc_to_l1_params(sys_modem_as_id_e_type wrrc_as_id,l1_ext_cmd_type *cmd_ptr)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_L1_CMD].data_ptr != NULL)
  {
    rrc_l1_u_type *rrc_l1_config;

    /* Populate the message ID directly to QSH global */
    ((wrrc_qsh_metric_rrc_to_l1_cmd_s *)rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_L1_CMD].data_ptr)->msg_id = (qsh_rrc_l1_cmd_e_type)(cmd_ptr->cmd_hdr.cmd_id & 0xFFFF);

    /* Get the address of QSH config field to a local pointer */
    rrc_l1_config = &(((wrrc_qsh_metric_rrc_to_l1_cmd_s *)rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_L1_CMD].data_ptr)->config);
    memset(rrc_l1_config, 0,sizeof(rrc_l1_u_type));
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
               
      case CPHY_FREQ_SCAN_REQ:
        rrc_l1_config->freq_scan.step = (uint8)(cmd_ptr->cmd.freq_scan.step);
        rrc_l1_config->freq_scan.band = (uint8)(cmd_ptr->cmd.freq_scan.band);
        rrc_l1_config->freq_scan.srch = (uint8)(cmd_ptr->cmd.freq_scan.srch);
        break;

      case CPHY_ACQ_REQ:
        rrc_l1_config->acq.freq = (uint16)(cmd_ptr->cmd.acq.freq);
        rrc_l1_config->acq.acq_type = (uint8)(cmd_ptr->cmd.acq.acq_type);
        rrc_l1_config->acq.acq_mode = (uint8)(cmd_ptr->cmd.acq.acq_mode);
        break;
      
      case CPHY_SETUP_REQ:
        rrc_l1_config->setup.req_mask = cmd_ptr->cmd.setup.req_mask;
        break;

      case CPHY_CELL_TRANSITION_REQ:
        rrc_l1_config->cell_trans.req_mask = cmd_ptr->cmd.cell_trans.setup_cmd.req_mask;
        break;

      case CPHY_CELL_BAR_REQ:
        rrc_l1_config->cell_bar.t_barred = cmd_ptr->cmd.cell_bar.t_barred;
        rrc_l1_config->cell_bar.l1_cell_bar_info = (uint8)(cmd_ptr->cmd.cell_bar.l1_cell_bar_info);
        break;
      
      default:
        WCDMA_MEMCPY(&(rrc_l1_config->gen_config),sizeof(qsh_gen_u32_config_type),&(cmd_ptr->cmd),sizeof(qsh_gen_u32_config_type));
        break;
    } 

    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_RRC_TO_L1_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  
  }
}
/*===========================================================================
   FUNCTION     rrc_send_l1_cmd
  
   DESCRIPTION
       This is a wrapper funtion to send l1 commands and to save the command id to crash
debug info.
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     l1_ext_cmd_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_send_l1_cmd(sys_modem_as_id_e_type wrrc_as_id,l1_ext_cmd_type *cmd_ptr)
{

#ifdef FEATURE_DUAL_SIM
  cmd_ptr->cmd_hdr.sys_modem_as_id = rrc_get_as_id(wrrc_as_id);
#endif

  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    rrc_crash_debug_save_rrc_to_l1_cmds(wrrc_as_id,cmd_ptr->cmd_hdr.cmd_id);
  }
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
  wrrc_qsh_log_rrc_to_l1_params(wrrc_as_id,cmd_ptr);
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    rrc_qsh_report_llc_event(wrrc_as_id,cmd_ptr);
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  WRRC_MSG1_HIGH("Dispatching out/Sending L1 cmd l1_cmd_enum_type_value0x%x", (uint32)(cmd_ptr->cmd_hdr.cmd_id));
  wl1_put_cmd(cmd_ptr, wrrc_as_id);
  if((cmd_ptr->cmd_hdr.cmd_id == CPHY_MEASUREMENT_REQ)
      &&
    ((rrctmr_get_remaining_time(wrrc_as_id,RRCTMR_CELL_DEPRI_TIMER)!=0) && !(rrcmisc_int_data.rrc_cell_depri_db.depri_req_sent))
      &&
      ((rrc_get_state(wrrc_as_id) == RRC_STATE_DISCONNECTED)
         ||
       (rrc_get_state(wrrc_as_id) == RRC_STATE_CELL_FACH)
         ||
       (rrc_get_state(wrrc_as_id) == RRC_STATE_CELL_PCH)
         ||
       (rrc_get_state(wrrc_as_id) == RRC_STATE_URA_PCH)))
  {
    rrc_send_depri_req(wrrc_as_id,TRUE,FALSE);
    rrcmisc_int_data.rrc_cell_depri_db.depri_req_sent = TRUE;
  }
}
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*===========================================================================
   FUNCTION     rrc_qsh_initialize_events
  
   DESCRIPTION
     This function initializes the events to be reported.
     1. All frequent events are disabled by default
     2. All rare events are enabled by default
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     None.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void rrc_qsh_initialize_events(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Disable all events by default*/
  rrc_qsh_events_db.acq_start.status       = FALSE; 
  rrc_qsh_events_db.freq_scan.status       = FALSE;
  rrc_qsh_events_db.activation_wndw.status = FALSE;
  rrc_qsh_events_db.wait_for_l2_ack.status = FALSE;
  rrc_qsh_events_db.sib_read.status        = FALSE;
  rrc_qsh_events_db.non_irat_resel.status  = FALSE;
  rrc_qsh_events_db.irat_resel.status      = FALSE;
  rrc_qsh_events_db.dch_fach_trans.status  = FALSE;
  rrc_qsh_events_db.suspend_start          = FALSE;
  rrc_qsh_events_db.resume_start           = FALSE;

  rrc_qsh_events_db.wtog_ho.status            = FALSE;
  rrc_qsh_events_db.gtow_ho.status            = FALSE;
  rrc_qsh_events_db.ltow_ho.status            = FALSE;
  rrc_qsh_events_db.cco_start.status          = FALSE;
  rrc_qsh_events_db.dch_cell_pch_trans.status = FALSE;
  rrc_qsh_events_db.resel_fail.status         = FALSE;
  rrc_qsh_events_db.conn_rel.status           = FALSE;
  rrc_qsh_events_db.wtol_ho.status            = FALSE;
  rrc_qsh_events_db.w2x_irat_ho_fail.status   = FALSE;
  rrc_qsh_events_db.inter_freq_ho_fail.status = FALSE;
  rrc_qsh_events_db.failure_ota_msg.status    = FALSE;
  rrc_qsh_events_db.sib_decode_failure.status = FALSE;
  rrc_qsh_events_db.lock_wait_timeout.status  = FALSE;
  rrc_qsh_events_db.rach_failure.status       = FALSE;
  rrc_qsh_events_db.gen_failure.status        = FALSE;
  rrc_qsh_events_db.state_change.status       = TRUE; /* Enable state change event by default as this is used by external modules */
}
/*===========================================================================
   FUNCTION     rrc_qsh_report_sib_event
  
   DESCRIPTION

  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_sib_event(sys_modem_as_id_e_type wrrc_as_id,rrc_qsh_sib_read_start_type_e start_type)
{
  if((rrc_qsh_events_db.sib_read.status == TRUE) && 
     (rrc_qsh_events_db.sib_read.start_type == start_type))
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_INFO,"Sub:%d Sib read start. Type %d",(wrrc_as_id+1),start_type);
    rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_SIB_READ_START);
  }
}
/*===========================================================================
   FUNCTION     rrc_qsh_report_irat_resel_event
  
   DESCRIPTION

  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_irat_resel_event(sys_modem_as_id_e_type wrrc_as_id, rrc_qsh_irat_resel_type_e resel_type)
{
  if((rrc_qsh_events_db.irat_resel.status == TRUE) && 
     (rrc_qsh_events_db.irat_resel.type == resel_type))
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_INFO,"Sub:%d IRAT reselection started. Type %d",(wrrc_as_id+1),resel_type);
    rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_IRAT_RESEL_START);
  }
}

/*===========================================================================
   FUNCTION     rrc_qsh_report_non_irat_resel_event
  
   DESCRIPTION

  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_non_irat_resel_event(sys_modem_as_id_e_type wrrc_as_id,rrc_qsh_non_irat_resel_type_e resel_type)
{
  if((rrc_qsh_events_db.non_irat_resel.status == TRUE) &&
     (rrc_qsh_events_db.non_irat_resel.resel_type == resel_type))
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_INFO,"Sub:%d Non IRAT reselection started. Type %d",(wrrc_as_id+1),resel_type);
    rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_NON_IRAT_RESEL_START);
  }
}
/*===========================================================================
   FUNCTION     rrc_qsh_report_resel_fail_event
  
   DESCRIPTION

  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_resel_fail_event(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Report Reselection failure event to QSH */
  if(rrc_qsh_events_db.resel_fail.status == TRUE)
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d Reselection fail event",(wrrc_as_id+1));
    rrc_qsh_report_event(wrrc_as_id,WRRC_QSH_EVENT_CELL_RESEL_FAILURE);
  }
  else
  {
    rrc_qsh_report_generic_failure_event(wrrc_as_id,WRRC_QSH_EVENT_CELL_RESEL_FAILURE);
  }
}

/*===========================================================================
   FUNCTION     rrc_qsh_report_w2x_ho_fail_event
  
   DESCRIPTION
    This function report WCDMA to GSM/LTE/1X handover failure to QSH
  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_w2x_irat_ho_fail_event(sys_modem_as_id_e_type wrrc_as_id, uint8 handover_type)
{
  /* Report W2X ho failure event to QSH if it is configured*/
  if(rrc_qsh_events_db.w2x_irat_ho_fail.status == TRUE)
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d W to X IRAT Handover fail event. Type %d",(wrrc_as_id+1),handover_type);
    rrc_qsh_report_event(wrrc_as_id,WRRC_QSH_EVENT_W2X_IRAT_HO_FAILURE);
  }
  else
  {
    rrc_qsh_report_generic_failure_event(wrrc_as_id,WRRC_QSH_EVENT_W2X_IRAT_HO_FAILURE);
  }
}

/*===========================================================================
   FUNCTION     rrc_qsh_report_interfreq_ho_fail_event
  
   DESCRIPTION
    This function report inter frequency handover failure to QSH
  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_inter_freq_ho_fail_event(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Report inter freq ho failure event to QSH */
  if(rrc_qsh_events_db.inter_freq_ho_fail.status == TRUE)
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d Inter frequency Handover fail event",(wrrc_as_id+1));
    rrc_qsh_report_event(wrrc_as_id,WRRC_QSH_EVENT_INTER_FREQ_HO_FAILURE);
  }
  else
  {
    rrc_qsh_report_generic_failure_event(wrrc_as_id,WRRC_QSH_EVENT_INTER_FREQ_HO_FAILURE);
  }
}

/*===========================================================================
   FUNCTION     rrc_qsh_report_rach_failure_event
  
   DESCRIPTION
    This function reports rach failure event to QSH
  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_rach_failure_event(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Report inter freq ho failure event to QSH */
  if(rrc_qsh_events_db.rach_failure.status == TRUE)
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d Rach failure event",(wrrc_as_id+1));
    rrc_qsh_report_event(wrrc_as_id,WRRC_QSH_EVENT_RACH_FAILURE);
  }
  else
  {
    rrc_qsh_report_generic_failure_event(wrrc_as_id,WRRC_QSH_EVENT_RACH_FAILURE);
  }
}
/*===========================================================================
   FUNCTION     rrc_qsh_report_generic_failure_event
  
   DESCRIPTION
    This function reports generic failure event to QSH
  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_generic_failure_event(sys_modem_as_id_e_type wrrc_as_id,wrrc_qsh_event_e event_id)
{
  /* Report inter freq ho failure event to QSH */
  if(rrc_qsh_events_db.gen_failure.status == TRUE)
  {
    WRRC_MSG2_HIGH("EVENT_FIELD_DEBUG_ANY_FAILURE: Sub:%d General failure event:%d",(wrrc_as_id+1),event_id);
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d General failure event:%d",(wrrc_as_id+1),event_id);
    rrc_qsh_report_event(wrrc_as_id,WRRC_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE);
  }
}
/*===========================================================================
   FUNCTION     rrc_qsh_report_failure_ota_msg_event
  
   DESCRIPTION
    This function reports to QSH when failure OTA message events like 
    ActiveSetUpdateFailure or RadioBearerSetupFailure occurs
  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_failure_ota_msg_event(sys_modem_as_id_e_type wrrc_as_id, wrrc_qsh_ota_msg_id failure_ota_type)
{
  /* Report event to QSH if event is configured*/
  if(rrc_qsh_events_db.failure_ota_msg.status == TRUE)
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d Failure OTA message received. Type %d",(wrrc_as_id+1),failure_ota_type);
    rrc_qsh_report_event(wrrc_as_id,WRRC_QSH_EVENT_OTA_MESSAGE_FAILURE);
  }
  else
  {
    rrc_qsh_report_generic_failure_event(wrrc_as_id,WRRC_QSH_EVENT_OTA_MESSAGE_FAILURE);
  }
}

/*===========================================================================
   FUNCTION     rrc_qsh_report_rcr_event
  
   DESCRIPTION

  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_rcr_event(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Report RRC connection release with unspecified cause event to QSH */
  if(rrc_qsh_events_db.conn_rel.status == TRUE)
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d RRC connection released cause UNSPECIFIED",(wrrc_as_id+1));
    rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_CONN_REL_UNSPEC_CAUSE);
  }
  else
  {
    rrc_qsh_report_generic_failure_event(wrrc_as_id,WRRC_QSH_EVENT_CONN_REL_UNSPEC_CAUSE);
  }
}

/*===========================================================================
   FUNCTION     rrc_qsh_report_llc_event
  
   DESCRIPTION

  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_llc_event(sys_modem_as_id_e_type wrrc_as_id, l1_ext_cmd_type *cmd_ptr)
{
  if(cmd_ptr->cmd_hdr.cmd_id == CPHY_ACQ_REQ)
  {
    if((rrc_qsh_events_db.acq_start.status == TRUE) && 
       (rrc_qsh_events_db.acq_start.pld.freq == cmd_ptr->cmd.acq.freq) &&
       ((cmd_ptr->cmd.acq.acq_type == L1_WCDMA_FREQ_ONLY) 
        || 
        (rrc_qsh_events_db.acq_start.pld.psc == cmd_ptr->cmd.acq.scr_code[0])
       )
      )
    {
      QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_INFO,"Sub:%d ACQ REQ freq %d psc %d",(wrrc_as_id+1),rrc_qsh_events_db.acq_start.pld.freq,rrc_qsh_events_db.acq_start.pld.psc);
      rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_ACQ);
    }
  }
  else if(cmd_ptr->cmd_hdr.cmd_id == CPHY_FREQ_SCAN_REQ)
  {
    if((rrc_qsh_events_db.freq_scan.status == TRUE)
       &&
        (((rrc_qsh_events_db.freq_scan.step == RRC_QSH_CSP_RAW_SCAN) && 
          (cmd_ptr->cmd.freq_scan.step == L1_FREQ_RAW_SCAN))
       ||
        ((rrc_qsh_events_db.freq_scan.step == RRC_QSH_CSP_FINE_SCAN)  && 
          (cmd_ptr->cmd.freq_scan.step == L1_FREQ_FINE_SCAN))
       ||
        ((rrc_qsh_events_db.freq_scan.step == RRC_QSH_CSP_ADDL_CHLS_SCAN) && 
          (cmd_ptr->cmd.freq_scan.step == L1_FREQ_ADDITIONAL_CHANNEL_SCAN))
       ||
        ((rrc_qsh_events_db.freq_scan.step == RRC_QSH_CSP_LIST_SCAN) && 
          (cmd_ptr->cmd.freq_scan.step == L1_FREQ_LIST_SCAN))))
    {
      QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_INFO,"Sub:%d Freq scan req",(wrrc_as_id+1));
      rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_FREQ_SCAN);    
    }
  }
  else if(cmd_ptr->cmd_hdr.cmd_id == CPHY_SUSPEND_WCDMA_MODE_REQ)
  {
    if(rrc_qsh_events_db.suspend_start == TRUE)
    {
      QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_INFO,"Sub:%d Suspend start",(wrrc_as_id+1));
      rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_SUSPEND_START);
    }
  }
  else if(cmd_ptr->cmd_hdr.cmd_id == CPHY_RESUME_WCDMA_MODE_REQ)
  {
    if(rrc_qsh_events_db.resume_start == TRUE)
    {
      QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_INFO,"Sub:%d Resume start",(wrrc_as_id+1));
      rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_RESUME_START);
    }
  }
}
    
/*===========================================================================
 FUNCTION     rrc_qsh_report_wait_l2_ack_event

 DESCRIPTION


 DEPENDENCIES
   None.

 PARAMETERS

 RETURN VALUE
   None.

 SIDE EFFECTS

===========================================================================*/
void rrc_qsh_report_wait_l2_ack_event(sys_modem_as_id_e_type wrrc_as_id, rrc_qsh_wait_for_l2_ack_type_e ack_type)
{
  if(rrc_qsh_events_db.wait_for_l2_ack.status == TRUE)
  {
    rrcllc_oc_set_status_e_type oc_status = rrcllc_get_ordered_config_status_wo_f3(wrrc_as_id);
    
    if(
        ((rrc_qsh_events_db.wait_for_l2_ack.type == RRC_WAIT_L2_ACK_FOR_SMC) &&
         (ack_type == RRC_WAIT_L2_ACK_FOR_SMC))
        ||
        ((rrc_qsh_events_db.wait_for_l2_ack.type == RRC_WAIT_L2_ACK_FOR_PCH_TRANS) 
          && 
           ((oc_status == OC_SET_FOR_FACH_CELL_PCH_TRANS)|| (oc_status == OC_SET_FOR_FACH_URA_PCH_TRANS) ||
            (oc_status == OC_SET_FOR_DCH_CELL_PCH_TRANS) || (oc_status == OC_SET_FOR_DCH_URA_PCH_TRANS) ||
            ((rrccu_return_cu_substate(wrrc_as_id) != RRCCU_INITIAL) 
              &&
               (
                (rrccu_return_ordered_state(wrrc_as_id) == rrc_RRC_StateIndicator_cell_PCH) 
                 || 
                (rrccu_return_ordered_state(wrrc_as_id) == rrc_RRC_StateIndicator_ura_PCH)
               )
           ))
        )
        ||
        ((rrc_qsh_events_db.wait_for_l2_ack.type == RRC_WAIT_L2_ACK_FOR_SRNS) && rrcrb_is_srns_in_progress(wrrc_as_id))
      )
    {
      QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_INFO,"Sub:%d Waiting L2 ack",(wrrc_as_id+1));
      rrc_qsh_report_event(wrrc_as_id, WRRC_QSH_EVENT_WAIT_FOR_L2_ACK);
    }
  }
}

/*===========================================================================
   FUNCTION     rrc_qsh_report_event
  
   DESCRIPTION

  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_qsh_report_event(sys_modem_as_id_e_type wrrc_as_id, wrrc_qsh_event_e event_id)
{
  qsh_client_event_notify_params_s  event_notify_params;
  qsh_client_event_notify_init(&event_notify_params);
  event_notify_params.client = QSH_CLT_WRRC;
  event_notify_params.id = event_id;
  event_notify_params.event_data = NULL;
  event_notify_params.subs_id = rrc_get_as_id(wrrc_as_id);
  qsh_client_event_notify(&(event_notify_params));
}
/*===========================================================================
 FUNCTION     rrc_qsh_report_sib_read_failure_event

 DESCRIPTION  This funtion reports QSH whenever there is SIB read failure 

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_qsh_report_sib_decode_failure_event(sys_modem_as_id_e_type wrrc_as_id, rrc_sib_status_e_type failure_cause)
{
  if((rrc_qsh_events_db.sib_decode_failure.status == TRUE) && 
     (rrc_qsh_events_db.sib_decode_failure.failure_cause == failure_cause))
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d Sib decode failure with cause %d",(wrrc_as_id+1),failure_cause);
    rrc_qsh_report_event(wrrc_as_id,WRRC_QSH_EVENT_SIB_DECODE_FAILURE);
  }
  else
  {
    rrc_qsh_report_generic_failure_event(wrrc_as_id,WRRC_QSH_EVENT_SIB_DECODE_FAILURE);
  }
}

/*===========================================================================
 FUNCTION     rrc_qsh_report_lock_wait_timeout_event

 DESCRIPTION  This funtion reports QSH whenever there is wrm lock wait timer expires without acuiring the lock 

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_qsh_report_lock_wait_timeout_event(sys_modem_as_id_e_type wrrc_as_id)
{
  if(rrc_qsh_events_db.lock_wait_timeout.status == TRUE)
  {
    QSH_LOG(QSH_CLT_WRRC,QSH_CAT_EVENT,QSH_MSG_TYPE_HIGH,"Sub:%d Sib decode failure with cause %d",(wrrc_as_id+1));
    rrc_qsh_report_event(wrrc_as_id,WRRC_QSH_EVENT_LOCK_WAIT_TIMEOUT);
  }
  else
  {
    rrc_qsh_report_generic_failure_event(wrrc_as_id,WRRC_QSH_EVENT_LOCK_WAIT_TIMEOUT);
  }
}
/*===========================================================================
   FUNCTION     rrc_qsh_configure_events
  
   DESCRIPTION

  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void rrc_qsh_configure_events(sys_modem_as_id_e_type wrrc_as_id, qsh_client_cfg_s event_cfg)
{

  boolean status = FALSE;
  qsh_event_action_e action = (qsh_event_action_e)((event_cfg.cmd_code >> QSH_EVENT_CMD_CODE_CFG) & 0xFF);
  wrrc_qsh_event_e event_id = (wrrc_qsh_event_e)((event_cfg.cmd_code >> QSH_EVENT_CMD_CODE_EVT_ID) & 0xFF);
  /*wrrc_as_id is extracted from the CFG cmd before calling rrc_qsh_configure_events*/
  uint8 num_words = (uint8)((event_cfg.cmd_code >> QSH_EVENT_CMD_CODE_NUM_WRDS) & 0xFF);
  
  MSG_HIGH("EVENT_DEBUG: Event configured. event id: %d event action: %d[0:enable|1:disable] num_words: %d", event_id, action, num_words);
  if(action == QSH_EVENT_ACTION_ENABLE)
  {
    status = TRUE;
  }
  else if(action == QSH_EVENT_ACTION_DISABLE)
  {
    status = FALSE;
  }
  else
  {
    WRRC_MSG1_ERROR("Bad Action:%d",action);
    return;
  }
  switch(event_id)
  {
    case WRRC_QSH_EVENT_ACQ:
      if(status == TRUE)
      {
        if(num_words >= 2)
        {
          rrc_qsh_events_db.acq_start.pld.freq = (event_cfg.cfg_data_1 & 0xFFFF);
          rrc_qsh_events_db.acq_start.pld.psc  = (event_cfg.cfg_data_2 & 0xFFFF);
        }
        else
        {
          WRRC_MSG1_ERROR("Bad Payload:%d",num_words);
          return;
        }
      }
      rrc_qsh_events_db.acq_start.status = status; 
      break;
    
    case WRRC_QSH_EVENT_FREQ_SCAN:
      if(status == TRUE)
      {
        if(num_words >= 1)
        {
          rrc_qsh_events_db.freq_scan.step = (rrc_qsh_csp_freq_scan_e_type)(event_cfg.cfg_data_1 & 0xFF);
        }
        else
        {
          WRRC_MSG1_ERROR("Bad Payload:%d",num_words);
          return;
        }
      }
      rrc_qsh_events_db.freq_scan.status = status;
      break;
    
    case WRRC_QSH_EVENT_ACTIVATON_WNDW_START:
      if(status == TRUE)
      {
        if(num_words >= 1)
        {
          rrc_qsh_events_db.activation_wndw.start_type = (rrc_qsh_act_wndw_start_type_e)(event_cfg.cfg_data_1 & 0xFF);
        }
        else
        {
          WRRC_MSG1_ERROR("Bad Payload:%d",num_words);
          return;
        }
      }
      rrc_qsh_events_db.activation_wndw.status = status;
      break;
    
    case WRRC_QSH_EVENT_WTOG_HO_REQ:
      rrc_qsh_events_db.wtog_ho.status = status;
      break;
    
    case WRRC_QSH_EVENT_GTOW_HO_REQ:
      rrc_qsh_events_db.gtow_ho.status = status;
      break;
    
    case WRRC_QSH_EVENT_LTOW_HO_REQ:
      rrc_qsh_events_db.ltow_ho.status = status;
      break;
    
    case WRRC_QSH_EVENT_CCO_START:
      rrc_qsh_events_db.cco_start.status = status;
      break;
    
    case WRRC_QSH_EVENT_WAIT_FOR_L2_ACK:
      if(status == TRUE)
      {
        if(num_words >= 1)
        {
          rrc_qsh_events_db.wait_for_l2_ack.type = (rrc_qsh_wait_for_l2_ack_type_e)(event_cfg.cfg_data_1 & 0xFF);
        }
        else
        {
          WRRC_MSG1_ERROR("Bad Payload:%d",num_words);
          return;
        }
      }
      rrc_qsh_events_db.wait_for_l2_ack.status = status;
      break;
    
    case WRRC_QSH_EVENT_SIB_READ_START:
      if(status == TRUE)
      {
        if(num_words >= 1)
        {
          rrc_qsh_events_db.sib_read.start_type = (rrc_qsh_sib_read_start_type_e)(event_cfg.cfg_data_1 & 0xFF);
        }
        else
        {
          WRRC_MSG1_ERROR("Bad Payload:%d",num_words);
          return;
        }
      }
      rrc_qsh_events_db.sib_read.status = status;
      break;
    
    case WRRC_QSH_EVENT_NON_IRAT_RESEL_START:
      if(status == TRUE)
      {
        if(num_words >= 1)
        {
          rrc_qsh_events_db.non_irat_resel.resel_type = (rrc_qsh_non_irat_resel_type_e)(event_cfg.cfg_data_1 & 0xFF);
        }
        else
        {
          WRRC_MSG1_ERROR("Bad Payload:%d",num_words);
          return;
        }
      }
      rrc_qsh_events_db.non_irat_resel.status = status;
      break;
    
    case WRRC_QSH_EVENT_IRAT_RESEL_START:
      if(status == TRUE)
      {
        if(num_words >= 1)
        {
          rrc_qsh_events_db.irat_resel.type = (rrc_qsh_irat_resel_type_e)(event_cfg.cfg_data_1 & 0xFF);
        }
        else
        {
          WRRC_MSG1_ERROR("Bad Payload:%d",num_words);
          return;
        }
      }
      rrc_qsh_events_db.irat_resel.status = status;
      break;
    
    case WRRC_QSH_EVENT_DCH_CELL_PCH_TRANS:
      rrc_qsh_events_db.dch_cell_pch_trans.status = status;
      break;
    
    case WRRC_QSH_EVENT_DCH_FACH_TRANS:
      rrc_qsh_events_db.dch_fach_trans.status = status;
      break;

    case WRRC_QSH_EVENT_SUSPEND_START:
      rrc_qsh_events_db.suspend_start = status;
      break;

    case WRRC_QSH_EVENT_RESUME_START:
      rrc_qsh_events_db.resume_start = status;
      break;
      
    case WRRC_QSH_EVENT_CELL_RESEL_FAILURE:
      rrc_qsh_events_db.resel_fail.status = status;
      break;
      
    case WRRC_QSH_EVENT_CONN_REL_UNSPEC_CAUSE:
      rrc_qsh_events_db.conn_rel.status = status;
      break;

    case WRRC_QSH_EVENT_WTOL_HO_REQ:
      rrc_qsh_events_db.wtol_ho.status = status;
      break;

    case WRRC_QSH_EVENT_W2X_IRAT_HO_FAILURE:
      rrc_qsh_events_db.w2x_irat_ho_fail.status = status;
      break;
        
    case WRRC_QSH_EVENT_INTER_FREQ_HO_FAILURE:
      rrc_qsh_events_db.inter_freq_ho_fail.status = status;
      break;
    
    case WRRC_QSH_EVENT_OTA_MESSAGE_FAILURE:
      rrc_qsh_events_db.failure_ota_msg.status = status;
      break;  

    case WRRC_QSH_EVENT_SIB_DECODE_FAILURE:
      rrc_qsh_events_db.sib_decode_failure.status = status;
      rrc_qsh_events_db.sib_decode_failure.failure_cause = (rrc_sib_status_e_type)(event_cfg.cfg_data_1 & 0xFF);
      break;

    case WRRC_QSH_EVENT_LOCK_WAIT_TIMEOUT:
      rrc_qsh_events_db.lock_wait_timeout.status = status;
      break;

    case WRRC_QSH_EVENT_RACH_FAILURE:
      rrc_qsh_events_db.rach_failure.status = status;
      break;

    case WRRC_QSH_EVENT_FIELD_DEBUG_ANY_FAILURE:
      rrc_qsh_events_db.gen_failure.status = status;
      break;

    case WRRC_QSH_EVENT_RRC_STATE_CHANGE:
      rrc_qsh_events_db.state_change.status = status;
      break;

    default:
      WRRC_MSG0_ERROR("EVENT_DEBUG: Invalid event id");
      break;
  }
}

#endif

#if defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC)
/*===========================================================================
   FUNCTION     wrrc_qsh_report_state_change_event
  
   DESCRIPTION
     Reports State change event to QSH
  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
     None
  ===========================================================================*/
void wrrc_qsh_report_state_change_event(sys_modem_as_id_e_type wrrc_as_id, rrc_state_e_type new_state)
{
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  if(rrc_qsh_events_db.state_change.status == TRUE)
#endif
  {
    qsh_client_event_notify_params_s  event_notify_params;
    qsh_client_event_notify_init(&event_notify_params);
    event_notify_params.client = QSH_CLT_WRRC;
    event_notify_params.id = WRRC_QSH_EVENT_RRC_STATE_CHANGE;
    event_notify_params.event_data = (void *)new_state;
    event_notify_params.subs_id = rrc_get_as_id(wrrc_as_id);
    WRRC_MSG1_MED("WRRC_QSH_EVENT:Reporting State change %d",new_state);
    qsh_client_event_notify(&(event_notify_params));
  }
}
#endif /*defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH) || defined(FEATURE_WCDMA_QSH_EVENT_METRIC)*/
/*===========================================================================
   FUNCTION     rrc_send_l1_ff_cmd
  
   DESCRIPTION
       This is a wrapper funtion to send l1 ff commands and to save the command id to crash
debug info.
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     l1_ext_cmd_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_send_l1_ff_cmd(sys_modem_as_id_e_type wrrc_as_id,l1_ext_cmd_type *cmd_ptr)
{

#ifdef FEATURE_DUAL_SIM
  cmd_ptr->cmd_hdr.sys_modem_as_id = rrc_get_as_id(wrrc_as_id);
#endif
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    rrc_crash_debug_save_rrc_to_l1_cmds(wrrc_as_id,cmd_ptr->cmd_hdr.cmd_id);
  }
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  wl1_put_ff_cmd(cmd_ptr, wrrc_as_id);
}

/*===========================================================================
   FUNCTION     rrc_send_l2_ul_cmd
  
   DESCRIPTION
       This is a wrapper funtion to send l2 ul commands and to save the command id to crash
debug info.
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     wcdma_l2_ul_cmd_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_send_l2_ul_cmd(sys_modem_as_id_e_type wrrc_as_id,wcdma_l2_ul_cmd_type *cmd_ptr)
{
  
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    /*Save L2 UL command in crash debug info */
    rrc_crash_debug_save_rrc_to_l2_ul_cmds(wrrc_as_id, cmd_ptr->cmd_hdr.cmd_id);
  }
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  cmd_ptr->cmd_hdr.as_id = wrrc_as_id;
  wcdma_l2_ul_put_cmd(cmd_ptr);
}
/*===========================================================================
   FUNCTION     rrc_send_l2_dl_cmd
  
   DESCRIPTION
       This is a wrapper funtion to send l2 dl commands and to save the command id to crash
debug info.
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     wcdma_l2_dl_cmd_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_send_l2_dl_cmd(sys_modem_as_id_e_type wrrc_as_id,wcdma_l2_dl_cmd_type *cmd_ptr)
{
  
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    /*Save L2 DL command in crash debug info */
    rrc_crash_debug_save_rrc_to_l2_dl_cmds(wrrc_as_id,cmd_ptr->cmd_hdr.cmd_id);
  }
  cmd_ptr->cmd_hdr.as_id = wrrc_as_id;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif
  wcdma_l2_dl_put_cmd(cmd_ptr);
}
#ifdef FEATURE_DUAL_SIM
/*===========================================================================
 FUNCTION     rrc_get_concurrency_nv()

 DESCRIPTION

 DEPENDENCIES
   None

 PARAMETERS
   None

 RETURN VALUE
   None

 SIDE EFFECTS

===========================================================================*/
uint16 rrc_get_concurrency_nv(sys_modem_as_id_e_type wrrc_as_id)
{
  uint16 nv_value = 0xFFFF;

  switch(rrc_dsds_int_data.wcdma_device_mode)
  {
    case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY:
      nv_value = rrcnv_data.rrc_dsds_concurrency;
      break;
    case SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_ACTIVE:
      nv_value = rrcnv_data.rrc_dsda_concurrency;
      break;
#ifdef FEATURE_TRIPLE_SIM 
    case SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY:
      nv_value = rrcnv_data.rrc_tsts_concurrency;
      break;
#endif
    default:
      break;
  }

  return nv_value;

}

/*===========================================================================
 FUNCTION     rrc_get_max_ta_duration

 DESCRIPTION
             This function calculates the maximum possible TA duration after which 
             RRC connection can be released. This is calculated based on re-establishment timer (T314/T315)
             and out of synch timers (N313/T313)

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   uint32.

 SIDE EFFECTS
   None

===========================================================================*/
uint32 rrc_get_max_ta_duration(sys_modem_as_id_e_type wrrc_as_id)
{
  uint16 rrc_n_313      = rrc_translate_n313(wrrc_as_id,RRC_GET_N313());
  uint8  rrc_timer_313  = RRC_GET_T313();
  uint32 rrc_timer_315  = rrccu_convert_t315_to_ms(wrrc_as_id,RRC_GET_T315());
  uint32 rrc_timer_302  = rrccu_convert_t302_to_ms(wrrc_as_id, RRC_GET_T302());
  uint8  rrc_n_302      = RRC_GET_N302();
  uint32 cu_tx_time     = (rrc_n_302+1)*rrc_timer_302;

  rrc_state_e_type rrc_state = rrc_get_state(wrrc_as_id);
  if(rrc_state == RRC_STATE_CELL_DCH)
  {
    uint32 duration_ms = 160 + (rrc_n_313-1)*10 + (rrc_timer_313 * 1000) + ((cu_tx_time > rrc_timer_315)? rrc_timer_315:cu_tx_time);
    return (duration_ms > 16000)? duration_ms : 16000;
  }
  else if ((rrc_state == RRC_STATE_CELL_FACH) ||
           (rrc_state == RRC_STATE_CONNECTING))
  {
    return 16000; /*Max 16secs in FACH state*/
  }
  else /*Shouldnt be entering here*/
  {
    WRRC_MSG1_ERROR("Function call in wrong RRC state:%d",rrc_state);
    return 0xFFFFFFFF;
  }
}
#endif /*FEATURE_DUAL_SIM*/

/*===========================================================================
 FUNCTION     rrc_check_gsm_bands_supported

 DESCRIPTION
   This function checks if device supports GSM bands

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   boolean

 SIDE EFFECTS
   None

===========================================================================*/

boolean rrc_check_gsm_bands_supported (sys_modem_as_id_e_type wrrc_as_id)
{
  sd_ss_band_pref_e_type band_pref = SD_SS_BAND_PREF_NONE;

  #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_RF_USES_RFM_PATH_ARCH)
  rfm_path_type path;
  #endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_RF_USES_RFM_PATH_ARCH) */

  rfm_device_enum_type i;
  
  // TODO: Use rfm_get_num_devices() once it becomes available.  Use '4' for now

  for(i= RFM_DEVICE_0; i<= RFM_DEVICE_3; i++)
  {
    #if defined(FEATURE_WCDMA_ATLAS_MODEM) && defined(FEATURE_RF_USES_RFM_PATH_ARCH)
    path.rfm_device = i;
    path.sig_path = 0;
    band_pref |= (sd_ss_band_pref_e_type) rfm_get_band_mask(path);
    #else  /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_RF_USES_RFM_PATH_ARCH) */
    band_pref |= (sd_ss_band_pref_e_type) rfm_get_band_mask(i);
    #endif /* (FEATURE_WCDMA_ATLAS_MODEM) && (FEATURE_RF_USES_RFM_PATH_ARCH) */
  }

  band_pref = band_pref &
             ( SD_SS_BAND_PREF_GSM_450 |
               SD_SS_BAND_PREF_GSM_480 |
               SD_SS_BAND_PREF_GSM_750 |
               SD_SS_BAND_PREF_GSM_850 |
               SD_SS_BAND_PREF_GSM_EGSM_900 |
               SD_SS_BAND_PREF_GSM_PGSM_900 |
               SD_SS_BAND_PREF_GSM_RGSM_900 |
               SD_SS_BAND_PREF_GSM_DCS_1800 |
               SD_SS_BAND_PREF_GSM_PCS_1900);

  if(band_pref != SD_SS_BAND_PREF_NONE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

  /*===========================================================================
 FUNCTION     rrc_get_minimum_t304_reqd

 DESCRIPTION
   This functions gets the minimum T304 required for successful transmission of UECI under good radio conditions

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   uint32.

 SIDE EFFECTS
   None

===========================================================================*/

uint32 rrc_get_minimum_t304_reqd(sys_modem_as_id_e_type wrrc_as_id,uint16 sdu_length)
{
  uint32 min_t304 = 0;
  rrc_state_e_type  rrc_state = rrc_get_state(wrrc_as_id);
  ordered_config_type *config_ptr = ((config_ptr_in_use == ORDERED_CONFIG)? ordered_config_ptr : current_config_ptr);
  
  /*For HSRACH and EDCH UL transmission would be good so adaptive calc wont be needed*/
  if(((rrc_state == RRC_STATE_CELL_DCH)  && (config_ptr->e_dch_transmission == TRUE))
#ifdef FEATURE_WCDMA_HS_RACH
      ||
    ((rrc_state ==RRC_STATE_CELL_FACH )
    && 
    (config_ptr->common_edch_transmission== TRUE))
#endif
    || (rrcllc_get_ordered_config_status_wo_f3(wrrc_as_id) !=OC_NOT_SET ))
  {
   /*Changing the maximum value to 10sec since GCF tests are failing due to more number of LTE bands.*/
    min_t304 = 10000;
  }
  else
  {
    min_t304 = (rrc_get_num_pdu(wrrc_as_id,sdu_length) * rrc_get_tti_configured(wrrc_as_id))+1000;
    /*Changing the maximum value to 10sec since GCF tests are failing due to more number of LTE bands.*/
    if((min_t304 > 10000) || (min_t304 == 1000))
    {
      min_t304 = 10000;
    }
  }

  WRRC_MSG1_HIGH("T304_DEBUG: T304 Min %d",min_t304);
    
  return(min_t304);
}

/*===========================================================================
 FUNCTION     rrc_get_num_pdu

 DESCRIPTION
   This functions calculates the number of PDUs that will be transmitted by RLC for a single UECI SDU

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   uint32.

 SIDE EFFECTS
   None

===========================================================================*/

uint32 rrc_get_num_pdu(sys_modem_as_id_e_type wrrc_as_id,uint16 sdu_length)
{
  uint32 pdu_size = 0;
  uint8 entity_idx = 0,num_pdus = 0;
  ordered_config_type *config_ptr = ((config_ptr_in_use == ORDERED_CONFIG)? ordered_config_ptr : current_config_ptr);

  /* SRBs are always with fixed size, */
  for(entity_idx = 0; entity_idx < UE_MAX_AM_ENTITY; entity_idx++)
  {
    if(config_ptr->rlc_am_parms.am_info[entity_idx]->common_config.rb_id == DCCH_AM_RADIO_BEARER_ID)
    {
       pdu_size = config_ptr->rlc_am_parms.am_info[entity_idx]->rlc_ul_am_config.ul_rlc_size;
       break;
    }
  }

  num_pdus = (sdu_length * 8) /(pdu_size - RLC_AM_HDR_SIZE);

  WRRC_MSG2_HIGH("T304_DEBUG: pdu_size %d Num PDUs %d",pdu_size, num_pdus);
  
  return (num_pdus);
}

/*===========================================================================
 FUNCTION     rrc_get_tti_configured

 DESCRIPTION
   This functions gets the tti configured in milli sec

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   uint32.

 SIDE EFFECTS
   None

===========================================================================*/
  
uint32 rrc_get_tti_configured (sys_modem_as_id_e_type wrrc_as_id)
{

  uint32 tti_configured = 0;
  uint8 trch_idx, cnt;
  ordered_config_type *config_ptr = ((config_ptr_in_use == ORDERED_CONFIG)? ordered_config_ptr : current_config_ptr);
  rrc_state_e_type  rrc_state = rrc_get_state(wrrc_as_id);
  uint32 trch_count;
  uint32 dlch_count;
    
  
  if(rrc_state == RRC_STATE_CELL_DCH)
  {
    for(trch_idx = 0; ((trch_idx < config_ptr->ul_cctrch.dch_info.ndchs) && (tti_configured == 0)); trch_idx++)
    {
      for(cnt = 0; cnt< config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].ndlchan; cnt++)
      {
        if(config_ptr->ul_cctrch.dch_info.dch_info[trch_idx].dlchan_info[cnt].rb_id == DCCH_AM_RADIO_BEARER_ID)
        {
          tti_configured = rrc_convert_tti_to_ms(wrrc_as_id,config_ptr->ul_tfcs.tfs_info[trch_idx].tti_info); 
          WRRC_MSG1_HIGH("T304_DEBUG: DCH configured tti	%d",tti_configured);
          break;
        }
      }
    }
  }
  else if (rrc_state == RRC_STATE_CELL_FACH)
  {
    for(trch_count = 0; ((trch_count < config_ptr->ul_tfcs.nchan) && (tti_configured == 0));trch_count ++)
    {
      for(dlch_count = 0;(dlch_count < config_ptr->mac_ul_parms.cctrch_ptr->rach_info.ndlchan); dlch_count ++)
      {
        if(config_ptr->mac_ul_parms.cctrch_ptr->rach_info.dlchan_info[dlch_count].rb_id == DCCH_AM_RADIO_BEARER_ID)
        {
          tti_configured = rrc_convert_tti_to_ms(wrrc_as_id,config_ptr->ul_tfcs.tfs_info[trch_count].tti_info);
          break;
        } /* if rb_id is found */
      } /* for all dedicated logical channel*/
    }
  }
  WRRC_MSG1_HIGH("T304_DEBUG: tti_configured	%d",tti_configured);
  return tti_configured;
}
/*===========================================================================
   FUNCTION     rrc_check_if_any_proc_waiting_for_l2_ack_for_pch_transition
  
   DESCRIPTION
     This function checks if any proceudre is waiting for L2 ACK for PCH transition
  
   DEPENDENCIES
     None.
  
   PARAMETERS

  
   RETURN VALUE
     TRUE  - If any proceudre is waiting for L2 ACK
     FALSE - not waiting for L2 ACK
  
   SIDE EFFECTS
  
  ===========================================================================*/
boolean rrc_check_if_any_proc_waiting_for_l2_ack_for_pch_transition
(
  sys_modem_as_id_e_type wrrc_as_id, 
  rrc_proc_e_type *proc_id_ptr
)
{
  rrc_RRC_StateIndicator cu_ordered_state = rrccu_return_ordered_state(wrrc_as_id);
  rrc_proc_e_type                 proc_id;
  rrcllc_oc_process_state_e_type  process_state;  
  rrcllc_oc_set_status_e_type     oc_status;
  boolean status = FALSE;
  
  *proc_id_ptr = RRC_PROCEDURE_NONE;
  oc_status = rrcllc_get_ordered_config_state_and_proc_wo_f3(wrrc_as_id,&proc_id, &process_state);

  if((oc_status == OC_SET_FOR_FACH_CELL_PCH_TRANS)||
                 (oc_status == OC_SET_FOR_FACH_URA_PCH_TRANS))
  {
    *proc_id_ptr = proc_id;
    status = TRUE;
  }
  else if((rrccu_return_cu_substate(wrrc_as_id) != RRCCU_INITIAL)
           &&
            ((cu_ordered_state == rrc_RRC_StateIndicator_cell_PCH ) 
              || 
             (cu_ordered_state == rrc_RRC_StateIndicator_ura_PCH))
         )
  {
    *proc_id_ptr = RRC_PROCEDURE_CU;
    status = TRUE;
  }
  return status;
}
/*===========================================================================
   FUNCTION     rrc_check_if_proc_waiting_for_l2_ack_for_pch_transition
  
   DESCRIPTION
       This function checks if any procedure is waiting for L2 ACK for PCH state transition
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     rrc_proc_e_type.
  
   RETURN VALUE
   uint8 
  
   SIDE EFFECTS
  
  ===========================================================================*/
boolean rrc_check_if_proc_waiting_for_l2_ack_for_pch_transition(sys_modem_as_id_e_type wrrc_as_id)
{
  rrccu_substate_e_type cu_sub_state = rrccu_return_cu_substate(wrrc_as_id);
  rrc_RRC_StateIndicator cu_ordered_state = rrccu_return_ordered_state(wrrc_as_id);
  rrcllc_oc_set_status_e_type     oc_status;
  boolean status = FALSE;
  
  oc_status = rrcllc_get_ordered_config_status_wo_f3(wrrc_as_id);

  if((oc_status == OC_SET_FOR_FACH_CELL_PCH_TRANS)||(oc_status == OC_SET_FOR_FACH_URA_PCH_TRANS)
  ||(oc_status == OC_SET_FOR_DCH_CELL_PCH_TRANS)||(oc_status == OC_SET_FOR_DCH_URA_PCH_TRANS))
  {
    status = TRUE;
  }
  else if((RRCCU_WAIT_FOR_L2_ACK  == cu_sub_state) && ((cu_ordered_state == rrc_RRC_StateIndicator_cell_PCH ) || (cu_ordered_state == rrc_RRC_StateIndicator_ura_PCH)))
  {
    status = TRUE;
  }
  return status;
}
/*===========================================================================
   FUNCTION     rrc_crash_debug_log_proc_substates
  
   DESCRIPTION
       This funtion saves history of procedure substates
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     rrc_proc_e_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrc_crash_debug_log_proc_substates(sys_modem_as_id_e_type wrrc_as_id,rrc_proc_e_type proc_id, uint8 substate, rrc_cmd_type *cmd_ptr)
{
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest == LTE_TO_RRC_CMD)
  {
    rrc_crash_debug_save_proc_substates(wrrc_as_id,proc_id,substate,cmd_ptr->cmd.wcdma_rrc_lte_irat_cmd.cmd.msg_hdr.id);
  }
  else
  {
    rrc_crash_debug_save_proc_substates(wrrc_as_id,proc_id,substate,cmd_ptr->cmd_hdr.cmd_id);
  }
}
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif
/*===========================================================================
   FUNCTION     rrc_crash_debug_save_proc_substates
  
   DESCRIPTION
       This funtion saves history of procedure substates
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     rrc_proc_e_type.
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/
  void rrc_crash_debug_save_proc_substates(sys_modem_as_id_e_type wrrc_as_id,rrc_proc_e_type proc_id, uint8 substate, rrc_cmd_e_type cmd_id)
{
  uint8 index;
  rrc_debug_cmd_type *debug_cmd_ptr;
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    switch(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest)
    {
      case MM_TO_RRC_CMD:
        index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index+RRC_CRASH_DEBUG_MM_CMD_CNT-1) % RRC_CRASH_DEBUG_MM_CMD_CNT;
        debug_cmd_ptr = &rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[index];
        break;
      case RR_TO_RRC_CMD:
        index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index+RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT-1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
        if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[index].cmd_id == cmd_id)
        {
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[index].proc_id = (uint8)proc_id;
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[index].substate = substate;
        }
        debug_cmd_ptr = NULL;
        break;
      case INT_CMD:
        index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index+RRC_CRASH_DEBUG_INTERNAL_CMD_CNT-1) % RRC_CRASH_DEBUG_INTERNAL_CMD_CNT;
        debug_cmd_ptr = &rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[index];
        break;
      case L1_TO_RRC_CMD:
        index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index+RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT-1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
        debug_cmd_ptr = &rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[index];
        break;       
      case L2_TO_RRC_CMD:
        index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_index+RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT-1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
        debug_cmd_ptr = &rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l2_cmds.l2_to_rrc_debug_cmds[index];
        break; 
      case LTE_TO_RRC_CMD:
        index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index+RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT-1) % RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT;
        if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[index].cmd_id == cmd_id)
        {
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[index].proc_id = (uint8)proc_id;
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[index].substate = substate;
        }
        debug_cmd_ptr = NULL;
        break; 
      case TDSRRC_TO_RRC_CMD:
        index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_index+RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT-1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
        if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_cmds[index].cmd_id == cmd_id)
        {
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_cmds[index].proc_id = (uint8)proc_id;
          rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_cmds[index].substate = substate;
        }
        debug_cmd_ptr = NULL;
        break;
      case INVALID_CMD:
      default:
        debug_cmd_ptr = NULL;
        break;    
    }
    if((debug_cmd_ptr != NULL) && (debug_cmd_ptr->cmd_id == cmd_id))
    {
      debug_cmd_ptr->proc_id = (uint8)proc_id;
      debug_cmd_ptr->substate = substate;
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif
    } 
  }
}
/*===========================================================================
   FUNCTION     rrc_get_current_substate
  
   DESCRIPTION
       This function returns the current substate of a given procedure
debug info.
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     rrc_proc_e_type.
  
   RETURN VALUE
   uint8 
  
   SIDE EFFECTS
  
  ===========================================================================*/

uint8 rrc_get_current_substate_for_proc(sys_modem_as_id_e_type wrrc_as_id,rrc_proc_e_type proc_id)
{
  switch (proc_id)
  {
    
    case RRC_PROCEDURE_CSP:
      return (uint8)rrc_get_csp_substate(wrrc_as_id);
      break;

    case RRC_PROCEDURE_RCE:
      return (uint8)rrcrce_get_rce_sub_state(wrrc_as_id);
      break;

    case RRC_PROCEDURE_RCR:
      return (uint8)rrcrcr_get_current_sub_state(wrrc_as_id);
      break;

    case RRC_PROCEDURE_RBE:
      return (uint8)rrcrbe_get_current_sub_state(wrrc_as_id);
      break;

    case RRC_PROCEDURE_RBRC:
      return (uint8)rrcrbrc_get_current_sub_state(wrrc_as_id);
      break;

    case RRC_PROCEDURE_TCR:
      return (uint8)rrctcrc_get_current_sub_state(wrrc_as_id);
      break;

    case RRC_PROCEDURE_TFC:
      return (uint8)rrctfcc_get_current_substate(wrrc_as_id);
      break;

    case RRC_PROCEDURE_PCR:
      return (uint8)rrcpcrc_get_current_sub_state(wrrc_as_id);
      break;

    case RRC_PROCEDURE_RBR:
      return (uint8)rrcrbr_get_current_sub_state(wrrc_as_id);
      break;

    case RRC_PROCEDURE_UECI:
      return (uint8)rrcueci_get_current_substate(wrrc_as_id);
      break;

    case RRC_PROCEDURE_SMC:
     return (uint8)rrcsmc_get_smc_sub_state(wrrc_as_id);
      break;

    case RRC_PROCEDURE_MCMR:
      return (uint8)rrcmeas_get_substate(wrrc_as_id);
      break;               
    case RRC_PROCEDURE_UMI:
      return (uint8)rrcumi_get_current_substate(wrrc_as_id);
      break; 

    case RRC_PROCEDURE_CU:
      return (uint8)rrccu_return_cu_substate(wrrc_as_id);
      break;            

    case RRC_PROCEDURE_IHO:
      return (uint8)rrciho_get_current_substate(wrrc_as_id);
      break;
    case RRC_PROCEDURE_CHO:
     return (uint8)rrccho_get_current_substate(wrrc_as_id);
      break;
    case RRC_PROCEDURE_SIB:
    case RRC_PROCEDURE_CHK:
    case RRC_PROCEDURE_PG2:
    case RRC_PROCEDURE_PG1:
    case RRC_PROCEDURE_UECE:
    case RRC_PROCEDURE_DDT:
    case RRC_PROCEDURE_SCR:
    case RRC_PROCEDURE_ASU:
#ifdef FEATURE_WCDMA_ETWS
    case RRC_PROCEDURE_BMC:
#endif
    case RRC_PROCEDURE_NONE:
    default:
      return RRC_INVALID_PROC_SUBSTATE;
      break;
  }
}

/*===========================================================================
   FUNCTION     rrc_check_if_nw_support_fd
  
   DESCRIPTION
       This function takes as_id as argument and internally calls 
	   rrcdormancy_check_if_nw_support_fast_dormancy()
  
   DEPENDENCIES
     None.
  
   PARAMETERS
     sys_modem_as_id_e_type
  
   RETURN VALUE
     boolean 
  
   SIDE EFFECTS
  
  ===========================================================================*/

boolean rrc_check_if_nw_support_fd( sys_modem_as_id_e_type wrrc_as_id)
{
  return rrcdormancy_check_if_nw_support_fast_dormancy(wrrc_as_id);
}
/*===========================================================================

FUNCTION rrc_register_wv_adapter_info_sub

DESCRIPTION
  This function is the API to register the function pointer for WV adapater call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_register_wv_adapter_info_cb_sub
(
  sys_modem_as_id_e_type wrrc_as_id,
  wcdma_ivoice_cmd_open_t *open_cmd
 
)
{
  *open_cmd->ret_handle = open_cmd->asid;
  rrcdata_codec_info.rrc_wcdma_call_open_info_cb_func = open_cmd->event_cb;
  WRRC_MSG1_HIGH("WV_WRRC: registering %x with wcdma voice adapter",open_cmd->event_cb);
}
/*===========================================================================

FUNCTION rrc_register_wv_adapter_info_sub

DESCRIPTION
  This function is the API to register the function pointer for WV adapater call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_register_wv_adapter_info_cb
(
  wcdma_ivoice_cmd_open_t *open_cmd
)
{
  sys_modem_as_id_e_type wrrc_as_id = open_cmd->asid;
  rrc_register_wv_adapter_info_cb_sub(wrrc_as_id,open_cmd);
}

/*===========================================================================

FUNCTION rrc_vsession_process_start

DESCRIPTION
  This function is the API to check Voice Session started

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/

void rrc_wv_session_process_start_sub(
  sys_modem_as_id_e_type wrrc_as_id,
  wcdma_ivoice_cmd_start_t *start_cmd
)
{
  WRRC_MSG0_HIGH("WV_WRRC: VOICE SESSION STARTED");
}
/*===========================================================================

FUNCTION rrc_vsession_process_start

DESCRIPTION
  This function is the API to check Voice Session started

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/

void rrc_wv_session_process_start(
wcdma_ivoice_cmd_start_t *start_cmd
)
{
  sys_modem_as_id_e_type wrrc_as_id = start_cmd->handle;
  rrc_wv_session_process_start_sub(wrrc_as_id,start_cmd);
}
/*===========================================================================

FUNCTION rrc_vsession_process_stop

DESCRIPTION
  This function is the API to check Voice Session stopped

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_wv_session_process_stop_sub( 
  sys_modem_as_id_e_type wrrc_as_id,
  wcdma_ivoice_cmd_stop_t *stop_cmd
  )
{
  WRRC_MSG0_HIGH("WV_WRRC: VOICE SESSION STOPPED");
}
/*===========================================================================

FUNCTION rrc_vsession_process_stop

DESCRIPTION
  This function is the API to check Voice Session stopped

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_wv_session_process_stop(
wcdma_ivoice_cmd_stop_t *stop_cmd
)
{
  sys_modem_as_id_e_type wrrc_as_id = stop_cmd->handle;
  rrc_wv_session_process_stop_sub(wrrc_as_id,stop_cmd);
}
/*===========================================================================

FUNCTION rrc_deregister_wcdma_call_info_cb

DESCRIPTION
  This function is the API to de-register the function pointer for MVS call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_deregister_wv_adapter_info_cb_sub (
  sys_modem_as_id_e_type wrrc_as_id,
  wcdma_ivoice_cmd_close_t *close_cmd
)
{
  if(rrcdata_codec_info.rrc_wcdma_call_open_info_cb_func != NULL)
  {
    rrcdata_codec_info.rrc_wcdma_call_open_info_cb_func = NULL;
    WRRC_MSG0_HIGH("WV_WRRC: de-registering with voice adapter");
  }
}
/*===========================================================================

FUNCTION rrc_deregister_wcdma_call_info_cb

DESCRIPTION
  This function is the API to de-register the function pointer for MVS call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_deregister_wv_adapter_info_cb(
  wcdma_ivoice_cmd_close_t *close_cmd
)
{
   //WRRC_ERR0_FATAL("rrc_deregister_wv_adapter_info_cb Crash");
  sys_modem_as_id_e_type wrrc_as_id = close_cmd->handle;
  rrc_deregister_wv_adapter_info_cb_sub(wrrc_as_id,close_cmd);
}
/*===========================================================================

FUNCTION rrc_check_nas_rab_modified

DESCRIPTION
  
DEPENDENCIES

RETURN VALUE
  TRUE/FALSE
 
SIDE EFFECTS

  None

===========================================================================*/


boolean rrc_check_nas_rab_modified(sys_modem_as_id_e_type wrrc_as_id)
{
boolean return_value = FALSE;

#ifdef FEATURE_SCUDIF_ENHANCEMENT
#error code not present
#endif/*FEATURE_SCUDIF_ENHANCEMENT*/

return return_value;
}
/*===========================================================================

FUNCTION rrc_wva_send_sample_rate_sub

DESCRIPTION
  This function is the API to check voice sampling rate for AMR NB

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_wva_send_sample_rate_sub
( 
  sys_modem_as_id_e_type wrrc_as_id,
  wcdma_ivoice_cmd_send_sample_rate_t *wva_sample_rate
)
{
  if(TRUE == rrc_is_voice_rab_present(wrrc_as_id))
  {
    rrc_send_codec_info_to_cm(wrrc_as_id, wva_sample_rate->vocoder_id, wva_sample_rate->sample_rate);
  }
  else
  {
    WRRC_MSG0_HIGH("WVA_WRRC:Ignoring update codec info from WVA as voice rab is not up ");
  }
}
/*===========================================================================

FUNCTION rrc_wva_send_sample_rate

DESCRIPTION
  This function is the API to check and notified the voice sampling rate.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void rrc_wva_send_sample_rate(wcdma_ivoice_cmd_send_sample_rate_t *sample_rate)
{
  sys_modem_as_id_e_type wrrc_as_id = sample_rate->handle;
  rrc_wva_send_sample_rate_sub(wrrc_as_id,sample_rate);
}
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC

/*===========================================================================
FUNCTION wrrc_qsh_log_resel_cel_sel_params

DESCRIPTION
  This function logs RLF OOS RRC connection reject parameters

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_rlf_oos_reject_params(sys_modem_as_id_e_type wrrc_as_id, wrrc_rlf_oos_con_reject_enum_type update_type, wrrc_qsh_int_action_e_type action)
{
  
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_RLF_OOS].data_ptr != NULL)
  {
    switch(action)
    {
      case WRRC_METRIC_LOG:
        /* Commit already populated values when timer expiry happens
             Get the next address from QSH API and initialize the data */
        rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_RLF_OOS,QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY);
        /* Fall through to Intialize the next address */
      case WRRC_METRIC_START:
        /* Intialize the data */
        ((wrrc_qsh_metric_rlf_oos_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RLF_OOS].data_ptr)->num_rlf = 0;
        ((wrrc_qsh_metric_rlf_oos_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RLF_OOS].data_ptr)->num_oos = 0;
        ((wrrc_qsh_metric_rlf_oos_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RLF_OOS].data_ptr)->num_con_reject = 0;
        break;
      case WRRC_METRIC_STOP:
        break;
      case WRRC_METRIC_UPDATE:
        switch(update_type)
        {
          case WRRC_RLF:
            ((wrrc_qsh_metric_rlf_oos_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RLF_OOS].data_ptr)->num_rlf++;
            break;
          case WRRC_OOS:
            ((wrrc_qsh_metric_rlf_oos_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RLF_OOS].data_ptr)->num_oos++;
            break;
          case WRRC_CON_REJ:
            ((wrrc_qsh_metric_rlf_oos_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RLF_OOS].data_ptr)->num_con_reject++;
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
  }
}
/*===========================================================================
FUNCTION wrrc_qsh_log_aset_params

DESCRIPTION
  This function logs ASET size parameters

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_aset_params(sys_modem_as_id_e_type wrrc_as_id, wrrc_qsh_int_action_e_type action)
{
  ordered_config_type* config_ptr;
  uint8 current_avg_aset_size = 0;
  uint8 current_total_num_rls = 0;
  
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr != NULL)
  {
    switch(action)
    {
      case WRRC_METRIC_UPDATE: 
        if(rrcllc_cmd_under_process.next_rrc_state == RRC_STATE_CELL_DCH)
        {
          if(OC_NOT_SET != rrcllc_get_ordered_config_status_wo_f3(wrrc_as_id))
          {
            config_ptr = ordered_config_ptr;
          }
          else
          {
            config_ptr = (config_ptr_in_use==ORDERED_CONFIG)?ordered_config_ptr:current_config_ptr;
          }
          if((rrcllc_cmd_under_process.procedure == RRC_PROCEDURE_RCE) ||
          (rrcllc_cmd_under_process.procedure == RRC_PROCEDURE_RBE) ||
          (rrcllc_cmd_under_process.procedure == RRC_PROCEDURE_RBRC) ||
          (rrcllc_cmd_under_process.procedure == RRC_PROCEDURE_RBR) ||
          (rrcllc_cmd_under_process.procedure == RRC_PROCEDURE_TCR) ||
          (rrcllc_cmd_under_process.procedure == RRC_PROCEDURE_PCR) ||
          (rrcllc_cmd_under_process.procedure == RRC_PROCEDURE_CU) ||
          (rrcllc_cmd_under_process.procedure == RRC_PROCEDURE_ASU))
          {
            /* Whenver there is a change in num rls only then we need to increment the num_rls_change_event_count */
            if(ordered_config_ptr->l1_dl_chan_parms.num_rl != current_config_ptr->l1_dl_chan_parms.num_rl)
            {
              /* If roll over happens due to addion, keep it at max value */
              if((((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl + config_ptr->l1_dl_chan_parms.num_rl) < ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl)
              {
                ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl = WRRC_QSH_MAX_ACCUMULATED_RLS;
              }
              else
              {
                ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl+=
                config_ptr->l1_dl_chan_parms.num_rl;
                num_rls_change_event_count++;
              }
            }
          }
        }
        if(num_rls_change_event_count == WRRC_QSH_MAX_ACCUMULATED_RLS)
        {
          num_rls_change_event_count = 0;
          ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl = 0;
        }
        break;
      case WRRC_METRIC_LOG:
        if(rrc_get_state(wrrc_as_id) == RRC_STATE_CELL_DCH) /*Log only for DCH state ?*/
        {
          if(num_rls_change_event_count == 0)
          {
            ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl = 0;
          }
          else
          {
            current_total_num_rls = ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl;
            current_avg_aset_size = RRC_ROUNDED_AVERAGE(current_total_num_rls,num_rls_change_event_count);
           ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl = current_avg_aset_size;            
          }
          /* Commit already populated values when timer expiry happens
          Get the next address from QSH API and initialize the data 
          */
          rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_ASET,QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY);
          /* Set the num rls in the next address to the current total rls value so that average aset size is computed as long as we are in cell_DCH state*/
          ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl = current_total_num_rls;
        }
        else
        {
          ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl = 0;
          num_rls_change_event_count = 0;
        }
        break;
      case WRRC_METRIC_START:
        /* Intialize the data */
        ((wrrc_qsh_metric_aset_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_ASET].data_ptr)->num_rl = 0;
        num_rls_change_event_count = 0;
        break;
      default:
        break;
    }
  }
}
/*===========================================================================
FUNCTION rrc_qsh_commit_data

DESCRIPTION
  This function commits data to QSH and 


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/

void rrc_qsh_commit_data(sys_modem_as_id_e_type wrrc_as_id, uint8 metric_idx , qsh_client_metric_log_reason_e reason)
{  
  qsh_client_metric_log_done_s qsh_data;
  qsh_client_metric_log_done_init(&qsh_data);
  qsh_data.client = QSH_CLT_WRRC;
  qsh_data.metric_id = metric_idx;
  qsh_data.log_reason = reason;
  qsh_data.metric_context_id = rrc_qsh_metric_db[metric_idx].metric_context_id;
  rrc_qsh_metric_db[metric_idx].data_ptr = qsh_client_metric_log_done(&qsh_data);
  if(rrc_qsh_metric_db[metric_idx].data_ptr == NULL)
  {
     ERR_FATAL("RRC_QSH:next address returned for metric id %d is NULL", metric_idx, 0, 0);
  }
}
/*===========================================================================
FUNCTION wrrc_qsh_log_call_end_data

DESCRIPTION
  This function logs call end data params to QSH


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/

void wrrc_qsh_log_conn_end_info(sys_modem_as_id_e_type wrrc_as_id, wrrc_qsh_int_action_e_type action,
                                wrrc_call_end_data_log_type *call_end_ptr)
{
  wrrc_qsh_metric_call_end_info_s_type * rrc_qsh_call_end_ptr;
  rrc_qsh_call_end_ptr = ((wrrc_qsh_metric_call_end_info_s_type*)rrc_qsh_metric_db[WRRC_QSH_METRIC_CONN_END_INFO].data_ptr);
  
  if(rrc_qsh_call_end_ptr != NULL)
  {
    switch(action)
    {
      case WRRC_METRIC_LOG:
        
        if(call_end_ptr != NULL)
        {
  
          /* Based on the type, update the cause into corresponding unions */
          switch(call_end_ptr->type)
          {
            case WRRC_CALL_END_TYPE_RRC_CONNECTION_RELEASE:
            rrc_qsh_call_end_ptr->type = SYS_CALL_END_EVENT_RX_RRC_RELEASE;
            rrc_qsh_call_end_ptr->cause.conn_rel_cause = call_end_ptr->cause.conn_rel_cause;
            break;
            
            case WRRC_CALL_END_TYPE_RLF:
            rrc_qsh_call_end_ptr->type = SYS_CALL_END_EVENT_RLF_OR_WEAK_SIGNAL;
            rrc_qsh_call_end_ptr->cause.rlf_cause = call_end_ptr->cause.rlf_cause;
            break;
  
            case WRRC_CALL_END_TYPE_HO_FAIL:
            rrc_qsh_call_end_ptr->type = SYS_CALL_END_EVENT_HANDOVER_FAIL;
            rrc_qsh_call_end_ptr->cause.ho_fail_type = call_end_ptr->cause.ho_fail_type;
            break;
  
            case WRRC_CALL_END_TYPE_RRC_CONNECTION_EST_FAILURE:
            rrc_qsh_call_end_ptr->type = SYS_CALL_END_EVENT_RLF_OR_WEAK_SIGNAL;
            rrc_qsh_call_end_ptr->cause.rlf_cause = SYS_RLF_WEAK_SIGNAL_RANDOM_ACCESS_FAILURE;
            break;
            
            default:
              /* Not expected to hit the default case */
            break;
          }
          /* All parameters are updated, commit the data to QSH */
          rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_CONN_END_INFO, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
        }
        break;

      case WRRC_METRIC_START:
        /* Initialize the structures */
        rrc_qsh_call_end_ptr->type = SYS_CALL_END_EVENT_MAX;
        break;
        
      default:
        /* Not expected to hit the default case */
        break;
    }
  }
}
/*===========================================================================
FUNCTION wrrc_qsh_init_serving_cell_param

DESCRIPTION
  This function serving cell metrics

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/

static void wrrc_qsh_init_serving_cell_param(sys_modem_as_id_e_type wrrc_as_id, wrrc_qsh_metric_serving_cell_s* serving_cell_metrics)
{
  if(serving_cell_metrics != NULL)
  {
    memset(serving_cell_metrics, 0, sizeof(wrrc_qsh_metric_serving_cell_s));
    serving_cell_metrics->cell_id = WCDMA_INVALID_CELL_ID;
    serving_cell_metrics->comp_mode_status = FALSE;
    serving_cell_metrics->cpc_drx_status = FALSE;
    serving_cell_metrics->cpc_dtx_status = FALSE;
    serving_cell_metrics->csg_indication = FALSE;
    serving_cell_metrics->drx_cycle = 0xFFFF;
    serving_cell_metrics->fdpch_status = FALSE;
    serving_cell_metrics->hsdpa_call_status = FALSE;
    serving_cell_metrics->hsdpa_cell_capability = NON_HSDPA_CAPABLE;
    serving_cell_metrics->hsupa_call_status = FALSE;
    serving_cell_metrics->hsupa_cell_capability = NON_HSUPA_CAPABLE;
    serving_cell_metrics->psc = 0xFFFF;
    serving_cell_metrics->rrc_state = 0xF;
    serving_cell_metrics->uarfcn = 0xFFFF;
    serving_cell_metrics->ul_interference = 0XFF;
  }
}
#ifdef FEATURE_DUAL_SIM
#ifndef FEATURE_DUAL_WCDMA
/*===========================================================================
FUNCTION rrc_qsh_update_metric_database

DESCRIPTION
  This function is updates metric database if W subscription is switched from slot1 to slot 2 or viceversa


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_update_metric_database(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Get the alternate sub ID */
  sys_modem_as_id_e_type alt_sub_id = ((wrrc_as_id == SYS_MODEM_AS_ID_1) ? SYS_MODEM_AS_ID_2 : SYS_MODEM_AS_ID_1);
  wrrc_qsh_metric_e metric_id = 0;

  WRRC_MSG2_HIGH("WRRC_SS_METRIC_DBG:W SUB Switch from %d to %d",alt_sub_id, wrrc_as_id);
  
  for(metric_id = 0; metric_id < WRRC_QSH_METRIC_MAX; metric_id++)
  {
    rrc_qsh_metric_db_sub[alt_sub_id][metric_id] = rrc_qsh_metric_db[metric_id];
    rrc_qsh_metric_db[metric_id] = rrc_qsh_metric_db_sub[wrrc_as_id][metric_id];
  }

}
#endif
#endif
/*===========================================================================
FUNCTION rrc_process_qsh_cmd

DESCRIPTION
  This function is updates metric/collects dumps/notifies events


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/

void rrc_process_qsh_cmd(sys_modem_as_id_e_type wrrc_as_id, qsh_client_cb_params_s* rrc_qsh_cmd)
{
  uint16 metric_id = WRRC_QSH_METRIC_MAX;
  qsh_client_action_done_s cb_done_ptr;
  wrrc_qsh_int_action_e_type action = WRRC_METRIC_NONE;
  uint8 local_idx;

  switch(rrc_qsh_cmd->action)
  {
    case QSH_ACTION_METRIC_CFG:
      metric_id = rrc_qsh_cmd->action_params.metric_cfg.id;
      WRRC_MSG2_HIGH("METRIC_DEBUG: Metric ID:%d Metric CFG ACTION: %d [0-STRT, 1-STOP]",metric_id,rrc_qsh_cmd->action_params.metric_cfg.action);
      
      if(metric_id < WRRC_QSH_METRIC_MAX)
      {
        if(rrc_qsh_cmd->action_params.metric_cfg.action == QSH_METRIC_ACTION_START)
        {
          action = WRRC_METRIC_START;
          if(rrc_qsh_cmd->action_params.metric_cfg.start_addr == NULL)
          {
            WRRC_MSG1_ERROR("RRC_QSH: invalid start address passed ", rrc_qsh_cmd->action_params.metric_cfg.start_addr);
          }
#ifdef FEATURE_DUAL_SIM
#ifndef FEATURE_DUAL_WCDMA

          if(rrc_qsh_cmd->action_params.metric_cfg.subs_id < WRRC_MAX_NUM_SUBS)
          {
            rrc_qsh_metric_db_sub[rrc_qsh_cmd->action_params.metric_cfg.subs_id][metric_id].data_ptr = 
                                      rrc_qsh_cmd->action_params.metric_cfg.start_addr;
            rrc_qsh_metric_db_sub[rrc_qsh_cmd->action_params.metric_cfg.subs_id][metric_id].metric_context_id = 
                                      rrc_qsh_cmd->action_params.metric_cfg.metric_context_id;
          }
          
          if(rrc_qsh_cmd->action_params.metric_cfg.subs_id != rrc_get_as_id(wrrc_as_id))
          {
             break;
          }
          else
#endif
#endif
          {
            rrc_qsh_metric_db[metric_id].data_ptr = rrc_qsh_cmd->action_params.metric_cfg.start_addr;
            rrc_qsh_metric_db[metric_id].metric_context_id = rrc_qsh_cmd->action_params.metric_cfg.metric_context_id;
          }
        }
        else if(rrc_qsh_cmd->action_params.metric_cfg.action == QSH_METRIC_ACTION_STOP)
        {
          action = WRRC_METRIC_STOP;
#ifdef FEATURE_DUAL_SIM
#ifndef FEATURE_DUAL_WCDMA
          if(rrc_qsh_cmd->action_params.metric_cfg.subs_id < WRRC_MAX_NUM_SUBS)
          {
            rrc_qsh_metric_db_sub[rrc_qsh_cmd->action_params.metric_cfg.subs_id][metric_id].data_ptr = NULL;
            rrc_qsh_metric_db_sub[rrc_qsh_cmd->action_params.metric_cfg.subs_id][metric_id].metric_context_id = 
                                      rrc_qsh_cmd->action_params.metric_cfg.metric_context_id;
          }
          
          if(rrc_qsh_cmd->action_params.metric_cfg.subs_id != rrc_get_as_id(wrrc_as_id))
          {
             break;
          }
          else
#endif
#endif
          {
            rrc_qsh_metric_db[metric_id].data_ptr = NULL;
            rrc_qsh_metric_db[metric_id].metric_context_id = rrc_qsh_cmd->action_params.metric_cfg.metric_context_id;
          }
        }
        
        /*	Initialize metric related parameters */
        switch(metric_id)
        {
          case WRRC_QSH_METRIC_RESEL_CEL_SEL:
            wrrc_qsh_log_cell_resel_stats(wrrc_as_id, IRAT_INVALID, action); 
            break;
          case WRRC_QSH_METRIC_RLF_OOS:
            wrrc_qsh_log_rlf_oos_reject_params(wrrc_as_id, WRRC_UPDATE_INVALD, action);
            break;
          case WRRC_QSH_METRIC_HO:
            wrrc_qsh_log_iho_stats(wrrc_as_id, IRAT_INVALID, action);
            break;
          case WRRC_QSH_METRIC_ASET:
            wrrc_qsh_log_aset_params(wrrc_as_id, action);
            break;
          case WRRC_QSH_METRIC_TIMERS_AND_CONSTANTS:
            wrrc_qsh_log_timers_and_const_params(wrrc_as_id, action);
            break;
          case WRRC_QSH_METRIC_SERVING:
            /* When metric start is configured, to have some valid serving cell params, 
            if RRC mode is active, update serving cell metric is called */
            wrrc_qsh_init_serving_cell_param(wrrc_as_id, 
                                              (wrrc_qsh_metric_serving_cell_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr);
            if(rrcmcm_get_rrc_mode(wrrc_as_id) == RRC_MODE_ACTIVE)
            {
              wrrc_qsh_update_serving_cell_metrics(wrrc_as_id);
            }
            break;
          case WRRC_QSH_METRIC_CONN_END_INFO:
            wrrc_qsh_log_conn_end_info(wrrc_as_id, action, NULL);
            break;
          case WRRC_QSH_METRIC_MULTI_RAB_STATUS:
            wrrc_qsh_log_mrab_status(wrrc_as_id, action);
            break;
          default:
            /*Everything else is event based */
            break;
        }
      }
      else
      {
        WRRC_MSG1_ERROR("RRC_QSH: invalid metric id %d passed from QSH", metric_id);
      }
      break;
    
    case QSH_ACTION_METRIC_TIMER_EXPIRY:

      for(local_idx=0;local_idx<rrc_qsh_cmd->action_params.metric_timer_expiry.metric_id_count;local_idx++)
      {
        metric_id = rrc_qsh_cmd->action_params.metric_timer_expiry.params[local_idx].metric_id;
        /* Process only metric expiries for the active sub */
#ifdef FEATURE_DUAL_SIM
        if(rrc_get_as_id(wrrc_as_id) == rrc_qsh_cmd->action_params.metric_timer_expiry.params[local_idx].subs_id)
#endif
        {
          switch(metric_id)
          {
            case WRRC_QSH_METRIC_RESEL_CEL_SEL:
              wrrc_qsh_log_cell_resel_stats(wrrc_as_id, IRAT_INVALID, WRRC_METRIC_LOG); 
              break;
            case WRRC_QSH_METRIC_RLF_OOS:
              wrrc_qsh_log_rlf_oos_reject_params(wrrc_as_id, WRRC_UPDATE_INVALD, WRRC_METRIC_LOG);
              break;
            case WRRC_QSH_METRIC_HO:
              wrrc_qsh_log_iho_stats(wrrc_as_id, IRAT_INVALID, WRRC_METRIC_LOG);
              break;
            case WRRC_QSH_METRIC_ASET:
              wrrc_qsh_log_aset_params(wrrc_as_id, WRRC_METRIC_LOG);
              break;
            default:
              /*Everything else is event based */
              break;
          }
        }
      }
      break;
      
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    case QSH_ACTION_CFG:

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
      #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/
      {
      rrc_qsh_configure_events(wrrc_as_id, rrc_qsh_cmd->action_params.cfg);
      }
      break;
#endif /*FEATURE_QSH_EVENT_NOTIFY_TO_QSH*/

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
    #error code not present
#endif
      
    default:
    /* We don't support any other action*/ 
      WRRC_MSG1_ERROR("RRC_QSH:Invalid action",rrc_qsh_cmd->action);
      break;
  }

  /* for timer expiries, qsh_client_action_done neednt be called */
  if((rrc_qsh_cmd->action == QSH_ACTION_METRIC_CFG) 
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    || (rrc_qsh_cmd->action == QSH_ACTION_CFG)
#endif 
    )
  {
    /*  QSH should be intimated that our metric configuration is updated */
    qsh_client_action_done_init(&cb_done_ptr);
    cb_done_ptr.cb_params_ptr = rrc_qsh_cmd;
    if(rrc_qsh_cmd->action_mode == QSH_ACTION_MODE_SYNC_REQUIRED)
    {
      cb_done_ptr.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
    }
    else
    {
      cb_done_ptr.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
    }
    qsh_client_action_done(&cb_done_ptr);
  }
}

/*===========================================================================
FUNCTION wrrc_qsh_log_rrc_to_rr_params

DESCRIPTION
  This function logs rrc to rr params

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_rrc_to_rr_params(sys_modem_as_id_e_type wrrc_as_id, uint32 msg_id, qsh_gen_u32_config_type* msg_ptr)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_RR_CMD].data_ptr != NULL)
  {
    qsh_gen_u32_config_type* rrc_rr_config;

    /* Populate the msg id to the QSH global directly */
    ((wrrc_qsh_metric_rrc_to_rr_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_RR_CMD].data_ptr)->msg_id = (qsh_rrc_rr_cmd_e_type)(msg_id & 0xFFFF);

    /* Get the address of QSH config field to a local pointer */
    rrc_rr_config = &(((wrrc_qsh_metric_rrc_to_rr_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_TO_RR_CMD].data_ptr)->config);
    memset(rrc_rr_config, 0,sizeof(qsh_gen_u32_config_type));
    /* Leave out the header and Log next 4 bytes in the uint32 field */
    WCDMA_MEMCPY(rrc_rr_config,sizeof(qsh_gen_u32_config_type),((char*)msg_ptr+sizeof(IMH_T)),sizeof(qsh_gen_u32_config_type));

    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id,WRRC_QSH_METRIC_RRC_TO_RR_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  }
}
/*===========================================================================
FUNCTION wrrc_qsh_log_l1_rrc_cmd_params

DESCRIPTION
  This function logs internal command parameters

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_l1_rrc_cmd_params(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type* cmd_ptr)
{

  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_L1_TO_RRC_CMD].data_ptr != NULL)
  {
    l1_rrc_u_type *l1_rrc_config;
    
    /* Populate the msg id to the QSH global directly */
    ((wrrc_qsh_metric_l1_to_rrc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_L1_TO_RRC_CMD].data_ptr)->msg_id = (qsh_l1_rrc_cmd_e_type)(cmd_ptr->cmd_hdr.cmd_id & 0xFFFF);

    /* Get the address of QSH config field to a local pointer */
    l1_rrc_config = &(((wrrc_qsh_metric_l1_to_rrc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_L1_TO_RRC_CMD].data_ptr)->config);
    memset(l1_rrc_config, 0,sizeof(l1_rrc_u_type));
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
      case RRC_CPHY_ACQ_CNF:
        l1_rrc_config->acq_cnf.status = cmd_ptr->cmd.acq_cnf.status;
        l1_rrc_config->acq_cnf.fail_type = (uint8)cmd_ptr->cmd.acq_cnf.fail_type;
        l1_rrc_config->acq_cnf.freq = cmd_ptr->cmd.acq_cnf.freq;
        break;
          
      case RRC_CPHY_SETUP_CNF:
        l1_rrc_config->setup_cnf.status = cmd_ptr->cmd.setup_cnf.status;
        l1_rrc_config->setup_cnf.status_type= (uint8)cmd_ptr->cmd.setup_cnf.status_type;
        break;
          
      case RRC_CPHY_ERROR_IND:
        l1_rrc_config->err_ind.err = (uint8)cmd_ptr->cmd.err_ind.err;
        l1_rrc_config->err_ind.cmd_id = (qsh_rrc_l1_cmd_e_type)cmd_ptr->cmd.err_ind.cmd_id;
        break;
          
      case RRC_CPHY_CELL_RESELECTION_IND:
        l1_rrc_config->cell_rsel_ind.status = (boolean)cmd_ptr->cmd.cell_rsel_ind.status;
        l1_rrc_config->cell_rsel_ind.cell = (uint8)cmd_ptr->cmd.cell_rsel_ind.cell;
        if(cmd_ptr->cmd.cell_rsel_ind.cell == L1_RESEL_WCDMA)
        {
          l1_rrc_config->cell_rsel_ind.freq = cmd_ptr->cmd.cell_rsel_ind.u.wcdma_info.freq;
        }
        break;

      case RRC_CPHY_CELL_TRANSITION_CNF:
        l1_rrc_config->cell_transition_cnf.status = cmd_ptr->cmd.cell_transition_cnf.status;
        l1_rrc_config->cell_transition_cnf.cell = (uint8)cmd_ptr->cmd.cell_transition_cnf.cell_rsel_ind.cell;
        if(cmd_ptr->cmd.cell_transition_cnf.cell_rsel_ind.cell == L1_RESEL_WCDMA)
        {
          l1_rrc_config->cell_rsel_ind.freq = cmd_ptr->cmd.cell_rsel_ind.u.wcdma_info.freq;
        }
        break;

      case RRC_CPHY_MEASUREMENT_IND:
        l1_rrc_config->meas_ind.meas_id = (uint8)cmd_ptr->cmd.meas_ind.meas_rpt->meas_id;
        l1_rrc_config->meas_ind.rpt_crit = (uint8)cmd_ptr->cmd.meas_ind.meas_rpt->rpt_crit;
        l1_rrc_config->meas_ind.meas_type = (uint8)cmd_ptr->cmd.meas_ind.meas_rpt->meas_type;
        break;
     
      case RRC_CPHY_MEAS_CTRL_CNF:
        l1_rrc_config->meas_ctrl_cnf.meas_id = cmd_ptr->cmd.meas_ctrl_cnf.meas_id;
        l1_rrc_config->meas_ctrl_cnf.status = cmd_ptr->cmd.meas_ctrl_cnf.status;
        break;
        
      default:
        /* By default Copy the first 4 bytes from the command to the QSH global */
        WCDMA_MEMCPY(&(l1_rrc_config->gen_config),sizeof(qsh_gen_u32_config_type),&(cmd_ptr->cmd),sizeof(qsh_gen_u32_config_type));
        break;
    }

    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id,WRRC_QSH_METRIC_L1_TO_RRC_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

  }
}

/*===========================================================================
FUNCTION wrrc_qsh_log_int_cmd_params

DESCRIPTION
  This function logs internal command parameters

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_int_cmd_params(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type* cmd_ptr)
{
  if(cmd_ptr->cmd_hdr.cmd_id == RRC_LLC_CMD_REQ)
  {
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_LLC_CMD].data_ptr != NULL)
    {
      qsh_gen_u32_config_type* rrc_llc_config;

      /* Get the address of QSH config field to a local pointer */
      rrc_llc_config = &(((wrrc_qsh_metric_llc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_LLC_CMD].data_ptr)->config);
      memset(rrc_llc_config, 0,sizeof(qsh_gen_u32_config_type));
      switch(cmd_ptr->cmd.rrc_llc_req.cmd_dest)
      {
        case LAYER1_PHY:
          ((wrrc_qsh_metric_llc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_LLC_CMD].data_ptr)->msg_id = 
                   (qsh_rrc_llc_cmd_e_type)(0x1000|(cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id & 0xFFFF));
          break;
        case LAYER2_MAC_UL:
        case LAYER2_RLC_UL:
         ((wrrc_qsh_metric_llc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_LLC_CMD].data_ptr)->msg_id = 
               (qsh_rrc_llc_cmd_e_type)(0x2000|(cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id & 0xFFFF));
          break;
        case LAYER2_MAC_DL:
        case LAYER2_RLC_DL:
         ((wrrc_qsh_metric_llc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_LLC_CMD].data_ptr)->msg_id = 
               (qsh_rrc_llc_cmd_e_type)(0x3000|(cmd_ptr->cmd.rrc_llc_req.l2_dl_cmd.cmd_id & 0xFFFF));
          break;
        default:
          break;
      }

      /*Log first 4 bytes in the uint32 field */  
      WCDMA_MEMCPY(rrc_llc_config,sizeof(qsh_gen_u32_config_type),&(cmd_ptr->cmd),sizeof(qsh_gen_u32_config_type));
      
      /* We are done now. Commit the data to QSH */
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_LLC_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL); 
    }
  }
  else
  {
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_INTERNAL_CMD].data_ptr != NULL)
    {
      if((cmd_ptr->cmd_hdr.cmd_id != RRC_DOWNLINK_SDU_IND) &&
         (cmd_ptr->cmd_hdr.cmd_id != RRC_LLC_CMD_REQ) &&
         (cmd_ptr->cmd_hdr.cmd_id != RRC_TIMER_FOR_RSSI_BER_EXPIRED_IND)
        )
      {
        qsh_gen_u32_config_type* rrc_int_config;
  
          /* Populate the msg id to the QSH global directly */
        ((wrrc_qsh_metric_internal_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_INTERNAL_CMD].data_ptr)->msg_id = (qsh_rrc_int_cmd_e_type)(cmd_ptr->cmd_hdr.cmd_id & 0xFFFF);
  
        /* Get the address of QSH config field to a local pointer */
        rrc_int_config = &(((wrrc_qsh_metric_internal_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_INTERNAL_CMD].data_ptr)->config);
        memset(rrc_int_config, 0,sizeof(qsh_gen_u32_config_type));
        /* Blindly Log first 4 bytes in the uint32 field */  
        WCDMA_MEMCPY(rrc_int_config,sizeof(qsh_gen_u32_config_type),&(cmd_ptr->cmd),sizeof(qsh_gen_u32_config_type));
        rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_INTERNAL_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL); 
      }     
    }
  }
}
/*===========================================================================
FUNCTION wrrc_qsh_log_cell_resel_stats

DESCRIPTION
  This function logs cell sel/resel fail stats

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_cell_resel_stats(sys_modem_as_id_e_type wrrc_as_id, wrrc_mobility_rat_enum_type resel_type, wrrc_qsh_int_action_e_type action)
{


  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr != NULL)
  {
    switch(action)
	{
    case WRRC_METRIC_UPDATE:
      switch(resel_type)
      {
        case WCDMA_RAT_MOB:
          ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_u_to_u_resel++;
          break;
        case LTE_RAT_MOB:
          ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_u_to_l_resel++;
          break;
        case GSM_RAT_MOB:
          ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_u_to_g_resel++;
          break;
        case CELL_RESEL_FAIL:
          ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_resel_fail++;
           break;
        case CELL_SEL_FAIL:
          ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_cel_sel_fail++;
          break;
        default:
          /* Do nothing */
        break;
      }
      break;
    case WRRC_METRIC_LOG:
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_RESEL_CEL_SEL, QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY);
      /* Fall through to Intialize the next address */
    case WRRC_METRIC_START:
      /* Intialize the data */
      ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_u_to_u_resel = 0;
      ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_u_to_l_resel = 0;
      ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_u_to_g_resel = 0;	   
      ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_resel_fail = 0;	   
      ((wrrc_qsh_metric_resel_cel_sel_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RESEL_CEL_SEL].data_ptr)->num_cel_sel_fail = 0;
       break;
     default:
     break;
    }
  }
}

/*===========================================================================
FUNCTION wrrc_qsh_log_cell_resel_stats

DESCRIPTION
This function logs cell sel/resel fail stats

DEPENDENCIES
None


RETURN VALUE
None


SIDE EFFECTS
None

===========================================================================*/
void wrrc_qsh_log_iho_stats(sys_modem_as_id_e_type wrrc_as_id, wrrc_mobility_rat_enum_type rat_type, wrrc_qsh_int_action_e_type action)
{

  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr != NULL)
  {
    switch(action)
    {
      case WRRC_METRIC_UPDATE:
      {
        switch(rat_type)
        {
          case WCDMA_RAT_MOB:
            ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_u_to_u_hho++;
            break;
          case LTE_RAT_MOB:
            ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_u_to_l_ho++;
            break;
          case GSM_RAT_MOB:
            ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_u_to_g_ho++;
            break;
          case LTE_RAT_SRVCC_MOB:
            ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_srvcc++;
            break;
          case LTE_RAT_CSFB_MOB:
            ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_csfb++;
            break;
          case IRAT_MOB_FAIL:
            ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_inter_rat_ho_fail++;
            break;
          default:
            break;
        }
      }
      break;
      case WRRC_METRIC_LOG:
       rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_HO, QSH_CLIENT_METRIC_LOG_REASON_TIMER_EXPIRY);
   /* Fall through to Intialize the next address */
      case WRRC_METRIC_START:
       /* Intialize the data */
       ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_u_to_u_hho = 0;
       ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_u_to_l_ho = 0;
       ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_u_to_g_ho = 0;
       ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_srvcc=0;
       ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_csfb = 0;
       ((wrrc_qsh_metric_ho_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_HO].data_ptr)->num_inter_rat_ho_fail = 0;
        break;
      default:
        break;
    }
  }
}
/*===========================================================================
FUNCTION wrrc_qsh_log_rrc_state_stats

DESCRIPTION
  This function logs conn reject stats

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_rrc_state_stats(sys_modem_as_id_e_type wrrc_as_id, rrc_state_e_type new_state)
{

  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_STATE].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_rrc_state_history*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RRC_STATE].data_ptr)->rrc_state = (uint8)new_state;
    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_RRC_STATE, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

  }
}


/*===========================================================================
FUNCTION wrrc_qsh_log_mm_rrc_cmd_params

DESCRIPTION
  This function mm to rrc commands

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_mm_rrc_cmd_params(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type* cmd_ptr)
{

  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MM_TO_RRC_CMD].data_ptr != NULL)
  {
    mm_rrc_u_type *mm_rrc_config;
  
    /* Populate the msg id to the QSH global directly */
    ((wrrc_qsh_metric_mm_to_rrc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MM_TO_RRC_CMD].data_ptr)->msg_id = (qsh_mm_rrc_cmd_e_type)(cmd_ptr->cmd_hdr.cmd_id & 0xFFFF);

    /* Get the address of QSH config field to a local pointer */
    mm_rrc_config = &(((wrrc_qsh_metric_mm_to_rrc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MM_TO_RRC_CMD].data_ptr)->config);
    memset(mm_rrc_config, 0,sizeof(mm_rrc_u_type));
    /* Populate the QSH structure corresponding to the command */
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
      case RRC_EST_REQ:
        mm_rrc_config->rrc_est_req.call_type = (uint8)(cmd_ptr->cmd.rrc_est_req.call_type);
        mm_rrc_config->rrc_est_req.cause= (uint8)(cmd_ptr->cmd.rrc_est_req.cause);
        mm_rrc_config->rrc_est_req.cn_domain_id = (boolean)(cmd_ptr->cmd.rrc_est_req.cn_domain_id);
        break;
        
      case RRC_SERVICE_REQ:
        mm_rrc_config->rrc_service_req.service_req_cause = (uint8)(cmd_ptr->cmd.rrc_service_req.service_req_cause);
        break;
        
      case RRC_OPEN_SESSION_REQ:
        mm_rrc_config->open_sess_req.est_cause = (uint8)(cmd_ptr->cmd.open_sess_req.est_cause);
        mm_rrc_config->open_sess_req.call_type = (uint8)(cmd_ptr->cmd.open_sess_req.call_type);
        mm_rrc_config->open_sess_req.cn_domain_id = (boolean)(cmd_ptr->cmd.open_sess_req.cn_domain_id);
        break;
        
      case RRC_ABORT_REQ:
        mm_rrc_config->rrc_abort_req.delay_sig_con_release = cmd_ptr->cmd.rrc_abort_req.delay_sig_con_release;
        mm_rrc_config->rrc_abort_req.auth_failed_bar_cells = cmd_ptr->cmd.rrc_abort_req.auth_failed_bar_cells;
        mm_rrc_config->rrc_abort_req.hard_abort = cmd_ptr->cmd.rrc_abort_req.hard_abort;
        mm_rrc_config->rrc_abort_req.abort_cause = (uint8)cmd_ptr->cmd.rrc_abort_req.abort_cause;
        break;
        
      case RRC_DS_STATUS_CHANGE_IND:
        mm_rrc_config->rrc_ds_status_change_ind.device_mode = (uint8)cmd_ptr->cmd.rrc_ds_status_change_ind.device_mode;
        mm_rrc_config->rrc_ds_status_change_ind.dual_standby_mode_info = (uint8)cmd_ptr->cmd.rrc_ds_status_change_ind.dual_standby_mode_info;
        mm_rrc_config->rrc_ds_status_change_ind.dual_standby_mode = cmd_ptr->cmd.rrc_ds_status_change_ind.dual_standby_mode;
        break;
        
      default:
        /* By default Copy the first 4 bytes from the command to the QSH global */
        WCDMA_MEMCPY(&(mm_rrc_config->gen_config),sizeof(qsh_gen_u32_config_type),&(cmd_ptr->cmd),sizeof(qsh_gen_u32_config_type));
        break;
    }

    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_MM_TO_RRC_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

  }
}
/*===========================================================================
FUNCTION wrrc_qsh_log_rr_rrc_cmd_params

DESCRIPTION
  This function rr to rrc commands

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_rr_rrc_cmd_params(sys_modem_as_id_e_type wrrc_as_id, rrc_rr_cmd_type* cmd_ptr)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_RR_TO_RRC_CMD].data_ptr != NULL)
  {
    rr_rrc_u_type *rr_rrc_config;

    /* Populate the msg id to the QSH global directly */
    ((wrrc_qsh_metric_rr_to_rrc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RR_TO_RRC_CMD].data_ptr)->msg_id = (qsh_rr_rrc_cmd_e_type)(cmd_ptr->cmd_hdr.cmd_id & 0xFFFF);

    /* Get the address of QSH config field to a local pointer */
    rr_rrc_config = &(((wrrc_qsh_metric_rr_to_rrc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_RR_TO_RRC_CMD].data_ptr)->config);
    memset(rr_rrc_config, 0,sizeof(rr_rrc_u_type));
    /* Populate the QSH structure corresponding to the command */
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
      case RR_INTERRAT_HANDOVER_CNF:
        rr_rrc_config->interrat_handover_cnf.handover_status = (uint8)(cmd_ptr->cmd.interrat_handover_cnf.handover_status);
        rr_rrc_config->interrat_handover_cnf.failure_cause = (uint8)(cmd_ptr->cmd.interrat_handover_cnf.failure_cause);
        break;
        
      case RR_INTERRAT_REDIRECT_REJ:
        rr_rrc_config->interrat_redirect_rej.failure_reason = (uint8)(cmd_ptr->cmd.interrat_redirect_rej.failure_reason);
        rr_rrc_config->interrat_redirect_rej.uoos_gsm_search_done = cmd_ptr->cmd.interrat_redirect_rej.uoos_gsm_search_done;
        break;
        
      case RR_INTERRAT_HANDOVER_REQ:
        rr_rrc_config->interrat_handover_req.cipher_enabled = (uint8)(cmd_ptr->cmd.interrat_handover_req.cipher_enabled);
        rr_rrc_config->interrat_handover_req.network_select_mode = (uint8)(cmd_ptr->cmd.interrat_handover_req.network_select_mode);
        rr_rrc_config->interrat_handover_req.csfb_call_status = cmd_ptr->cmd.interrat_handover_req.csfb_call_status;
        break;
        
      default:
        /*AS ID field is the first field for all RR to RRC commands. Leave out this field and blindly copy the next 4 bytes to QSH global */
        WCDMA_MEMCPY(&(rr_rrc_config->gen_config),sizeof(qsh_gen_u32_config_type),((char*)(&(cmd_ptr->cmd))+sizeof(sys_modem_as_id_e_type)),sizeof(qsh_gen_u32_config_type));
        break;
    }

    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_RR_TO_RRC_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

  }
}
/*===========================================================================
FUNCTION wrrc_qsh_log_lte_rrc_cmd_params

DESCRIPTION
  This function lte to rrc commands

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_log_lte_rrc_cmd_params(sys_modem_as_id_e_type wrrc_as_id, wcdma_rrc_lte_irat_cmd_type* lte_cmd_ptr)
{

  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_LTE_TO_RRC_CMD].data_ptr != NULL)
  {
    qsh_lte_rrc_config_u_type *lte_rrc_config;
     
     /* Populate the msg id to the QSH global directly */
    ((wrrc_qsh_metric_lte_to_rrc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_LTE_TO_RRC_CMD].data_ptr)->msg_id = (qsh_lte_rrc_cmd_e_type)(lte_cmd_ptr->cmd.msg_hdr.id & 0xFFFF);

    /* Get the address of QSH config field to a local pointer */
    lte_rrc_config = &(((wrrc_qsh_metric_lte_to_rrc_cmd_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_LTE_TO_RRC_CMD].data_ptr)->config);
    memset(lte_rrc_config, 0,sizeof(qsh_gen_u32_config_type));

    switch(lte_cmd_ptr->cmd.msg_hdr.id)
    {
      case LTE_RRC_WCDMA_RESEL_FAILED_RSP:
        lte_rrc_config->resel_fail.time_to_reselect = 
            lte_cmd_ptr->cmd.lte_rrc_wcdma_resel_failed_rsp.time_to_reselect;
        lte_rrc_config->resel_fail.cause = 
            (qsh_lte_rrc_irat_to_lte_failure_cause_type_e)(lte_cmd_ptr->cmd.lte_rrc_wcdma_resel_failed_rsp.cause);
        break;
      case LTE_RRC_WCDMA_PLMN_SRCH_RSP: 
        lte_rrc_config->plmn_srch_rsp.srch_done = 
            lte_cmd_ptr->cmd.lte_rrc_wcdma_plmn_srch_rsp.srch_done;
        lte_rrc_config->plmn_srch_rsp.partial_srch = 
            lte_cmd_ptr->cmd.lte_rrc_wcdma_plmn_srch_rsp.partial_srch;
        lte_rrc_config->plmn_srch_rsp.hplmn_found = 
            lte_cmd_ptr->cmd.lte_rrc_wcdma_plmn_srch_rsp.hplmn_found;
        lte_rrc_config->plmn_srch_rsp.no_fp_info = 
            lte_cmd_ptr->cmd.lte_rrc_wcdma_plmn_srch_rsp.no_fp_info;
        break;
      case LTE_RRC_WCDMA_PSHO_RSP:
        lte_rrc_config->psho_rsp.status = 
            (qsh_lte_rrc_irat_to_lte_psho_status_type_e)(lte_cmd_ptr->cmd.lte_rrc_wcdma_psho_rsp.ho_status);
        lte_rrc_config->psho_rsp.fail_cause = 
            (qsh_lte_rrc_irat_to_lte_psho_failure_cause_type_e)(lte_cmd_ptr->cmd.lte_rrc_wcdma_psho_rsp.failure_cause);
        break;
      default:
        /*MSH_HDR field is the first field for all LTE to RRC commands. Leave out this field and blindly copy the next 4 bytes to QSH global */
        WCDMA_MEMCPY(&(lte_rrc_config->gen_conf),sizeof(qsh_gen_u32_config_type),((char*)(&(lte_cmd_ptr->cmd))+sizeof(msgr_hdr_s)),sizeof(qsh_gen_u32_config_type));
        break;
        
    }
    /* We are done now. Commit the data to QSH */
    rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_LTE_TO_RRC_CMD, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  }
}
/*===========================================================================
FUNCTION wrrc_qsh_update_serving_cell_metrics

DESCRIPTION
  This function serving cell metrics

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
void wrrc_qsh_update_serving_cell_metrics(sys_modem_as_id_e_type wrrc_as_id)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr != NULL)
  {
    /* Update serving cell metrics only if UE is camped on */
    if(RRC_CAMPED_ON == rrc_ccm_get_curr_camping_status(wrrc_as_id, RRC_PROCEDURE_NONE))
    {
      wrrc_qsh_metric_serving_cell_s serving_cell_metrics; 
      uint16 freq =0, psc =0;
      uint8 idx;
      uint8* len;
      ordered_config_type* config_ptr = rrcllc_get_config_ptr_in_use(wrrc_as_id);
      sys_hs_ind_e_type hs_ind;  
      rrc_hsdpa_status_enum_type hsdpa_status = rrc_get_hsdpa_sts(wrrc_as_id);
      
      WRRC_MSG0_MED("METRIC_DEBUG: wrrc_qsh_update_serving_cell_metrics ");
      /* Intialize the local variable before updating the current values */
      wrrc_qsh_init_serving_cell_param(wrrc_as_id, &serving_cell_metrics);
      
      if((rrc_get_state(wrrc_as_id) == RRC_STATE_CELL_DCH) && (TRUE == l1_get_reference_cell_info(&freq, &psc, wrrc_as_id)))
      {
        serving_cell_metrics.uarfcn = freq;
        serving_cell_metrics.psc = psc;
        WRRC_MSG2_HIGH("Freq %d and PSC %d returned from L1.",freq, psc);
        for(idx=0; idx < L1_MAX_RL; idx++)	
        {
          if((psc == config_ptr->l1_dl_chan_parms.dl_per_rl[idx].pri_cpich_scr_code) &&
          (rrc_cell_params_int_data.cell_id_per_rl[idx] != WCDMA_INVALID_CELL_ID))
          {
            serving_cell_metrics.cell_id = rrc_cell_params_int_data.cell_id_per_rl[idx];
            break;
          }
        }
        /* If OTA doesnt provide cell id, pick cell id from current camped cell id if freq and PSC matches */
        if((serving_cell_metrics.cell_id == ((WCDMA_INVALID_CELL_ID))) && (psc == rrc_ccm_get_curr_camped_psc(wrrc_as_id)) && (freq == rrc_ccm_get_curr_camped_freq(wrrc_as_id)))
        {
          serving_cell_metrics.cell_id = rrc_ccm_get_curr_camped_cell_id(wrrc_as_id);
        }

        /*
          During HHO cases, cell_id_dch_info db has the valid cell ID. This gets updated 
          when GPS cell ID request is received. Pick the cell-ID from this data base if cell_id is invalid
        */
        if((serving_cell_metrics.cell_id == WCDMA_INVALID_CELL_ID) &&
           (rrc_get_state(wrrc_as_id) == RRC_STATE_CELL_DCH) &&
           (cell_id_dch_info.cell_id != WCDMA_INVALID_CELL_ID) &&
           (freq == cell_id_dch_info.uarfcn_dl) &&
           (psc == cell_id_dch_info.psc))
        {
          serving_cell_metrics.cell_id = cell_id_dch_info.cell_id;
        }
        serving_cell_metrics.ul_interference = 0Xff;	
        serving_cell_metrics.hsdpa_call_status = ((hsdpa_status == HSDPA_ACTIVE)?TRUE:FALSE);
        serving_cell_metrics.hsupa_call_status = ((config_ptr->e_dch_transmission == TRUE)?TRUE:FALSE);
      }
      else
      {
        /* If we are not camped on, then assumption is we are storing prev camped freq */
        /* Test for DCH to FACH */
        serving_cell_metrics.uarfcn = rrc_ccm_get_curr_camped_freq(wrrc_as_id);
        serving_cell_metrics.cell_id = (uint32)rrc_ccm_get_curr_camped_cell_id(wrrc_as_id);
        serving_cell_metrics.psc = rrc_ccm_get_curr_camped_psc(wrrc_as_id);
        serving_cell_metrics.hsdpa_cell_capability = NON_HSDPA_CAPABLE; 	
        serving_cell_metrics.hsupa_cell_capability = NON_HSUPA_CAPABLE;
        hs_ind = rrc_get_3g_cell_capability_indicator(wrrc_as_id);	
        if((hs_ind == SYS_HS_IND_HSDPA_SUPP_CELL) || (hs_ind == SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL))
        {
          serving_cell_metrics.hsdpa_cell_capability = HSDPA_CAPABLE; 	
        }
        if((hs_ind == SYS_HS_IND_HSUPA_SUPP_CELL) || (hs_ind == SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL))
        {
          serving_cell_metrics.hsupa_cell_capability = HSUPA_CAPABLE; 	
        }
        serving_cell_metrics.ul_interference = config_ptr->rrc_ul_pwr_init_parms.ul_interference;
        serving_cell_metrics.csg_indication = ((rrc_ccm_get_curr_camped_csg_id(wrrc_as_id)!= SYS_CSG_ID_INVALID)? TRUE: FALSE);
      }
      serving_cell_metrics.fdpch_status = config_ptr->l1_dl_chan_parms.use_fdpch;
      serving_cell_metrics.cpc_dtx_status = config_ptr->cpc_dtx_drx_status;
      serving_cell_metrics.cpc_drx_status = config_ptr->cpc_drx_status;  
      serving_cell_metrics.comp_mode_status = rrcllc_is_cm_active(wrrc_as_id, config_ptr);
      serving_cell_metrics.drx_cycle = (uint16)(rrcllc_get_drx_info(wrrc_as_id));
      serving_cell_metrics.rrc_state = rrc_get_state(wrrc_as_id);
      
      /* 
         UL Interference value can change very rapidly. To avoid committing serving cell params 
         too often, current UL interference value is copied to qsh pointer before comparing for changes 
      */
      ((wrrc_qsh_metric_serving_cell_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr)->ul_interference = serving_cell_metrics.ul_interference;

      /* Retain the header contents from qsh data pointer */
      WCDMA_MEMCPY(&serving_cell_metrics, sizeof(qsh_metric_hdr_s), rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr, sizeof(qsh_metric_hdr_s));

      /* If any of the fields change commit data to QSH */
      if(memcmp(&serving_cell_metrics,rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr,
      sizeof(wrrc_qsh_metric_serving_cell_s)))
      {
        /*Copy the local data to QSH pointer */
        WCDMA_MEMCPY(rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr,sizeof(wrrc_qsh_metric_serving_cell_s),&serving_cell_metrics,
        sizeof(wrrc_qsh_metric_serving_cell_s));
        
        WRRC_MSG1_MED("SS_WRRC_METRIC_DBG:cell_id %d",serving_cell_metrics.cell_id);
        /*Contents are updated. Commit data to QSH */
        rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_SERVING, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
        
        /*Set the contents of next address to whatever we currenlty have */
        WCDMA_MEMCPY(rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr, sizeof(wrrc_qsh_metric_serving_cell_s),&serving_cell_metrics,
        sizeof(wrrc_qsh_metric_serving_cell_s));
      }
    }
  }
}
/*===========================================================================
FUNCTION rrc_update_cell_id_in_serving_cell_metrics

DESCRIPTION
  This function updates cell_ID to QSH if there is any change

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/

void rrc_update_cell_id_in_serving_cell_metrics(sys_modem_as_id_e_type wrrc_as_id,uint32 cell_id)
{
  wrrc_qsh_metric_serving_cell_s serving_cell_metrics;
  wrrc_qsh_metric_serving_cell_s *rrc_qsh_serving_cell_metric_ptr;
  rrc_qsh_serving_cell_metric_ptr = ((wrrc_qsh_metric_serving_cell_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr);
  
  if(rrc_qsh_serving_cell_metric_ptr != NULL)
  {
    /* If cell ID is not same as previous, report this to qsh */
    if((cell_id != WCDMA_INVALID_CELL_ID)
        &&
       (rrc_qsh_serving_cell_metric_ptr->cell_id != cell_id))
    {
       /* Copy the cell ID to QSH pointer */
       rrc_qsh_serving_cell_metric_ptr->cell_id = cell_id;

       /* Copy the current contents of current serving cell metrics to local memory */
       WCDMA_MEMCPY(&serving_cell_metrics,sizeof(wrrc_qsh_metric_serving_cell_s),rrc_qsh_serving_cell_metric_ptr,
                    sizeof(wrrc_qsh_metric_serving_cell_s));
       
       WRRC_MSG1_MED("SS_WRRC_METRIC_DBG:cell_id %d",cell_id);
       /* Cell ID is updated. Commit data to QSH */
       rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_SERVING, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);

       /* Get the address of next circular buffer pointer to local */
       rrc_qsh_serving_cell_metric_ptr = ((wrrc_qsh_metric_serving_cell_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_SERVING].data_ptr);
       /*Set the contents of next address to whatever we currenlty have */
       WCDMA_MEMCPY(rrc_qsh_serving_cell_metric_ptr, sizeof(wrrc_qsh_metric_serving_cell_s),&serving_cell_metrics,
                    sizeof(wrrc_qsh_metric_serving_cell_s));
    }
  }
}

/*===========================================================================
FUNCTION wrrc_qsh_log_mrab_status

DESCRIPTION
  This function logs mrab status

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/

void wrrc_qsh_log_mrab_status(sys_modem_as_id_e_type wrrc_as_id,wrrc_qsh_int_action_e_type action)
{
  
  wrrc_qsh_metric_multi_rab_status_s_type *rrc_qsh_mrab_status_ptr;
  rrc_qsh_mrab_status_ptr = ((wrrc_qsh_metric_multi_rab_status_s_type*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MULTI_RAB_STATUS].data_ptr);
  
  if(rrc_qsh_mrab_status_ptr != NULL)
  {
    /* Get the current multi rab status */
    sys_rab_status_e_type current_mrab_status = ((sys_rab_status_e_type)rrc_query_mac_rab_status(wrrc_as_id));
    
    switch(action)
    {
      case WRRC_METRIC_LOG:
        /* If multi rab status is not same as previous, report this to qsh */
        if(rrc_qsh_mrab_status_ptr->mrab_status != current_mrab_status)
        {
           /* Copy the local data to QSH pointer */
           rrc_qsh_mrab_status_ptr->mrab_status = current_mrab_status;
        
           /* Contents are updated. Commit data to QSH */
           rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_MULTI_RAB_STATUS, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
        
           /* Set the contents of next address to whatever we currently have */
           rrc_qsh_mrab_status_ptr = ((wrrc_qsh_metric_multi_rab_status_s_type*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MULTI_RAB_STATUS].data_ptr);
           rrc_qsh_mrab_status_ptr->mrab_status = current_mrab_status;
        }
        break;
      
      case WRRC_METRIC_START:
        rrc_qsh_mrab_status_ptr->mrab_status = current_mrab_status;
        /* Report status to qsh if rabs are already present */
        if(current_mrab_status != SYS_RAB_STATUS_NO_RAB)
        {
          rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_MULTI_RAB_STATUS, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
        }
        break;

      default:
        break;
    }
  }
}
/*===========================================================================
FUNCTION wrrc_qsh_log_cell_update_cause

DESCRIPTION
  This function logs Cell update cause

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/

void wrrc_qsh_log_cell_update_cause(sys_modem_as_id_e_type wrrc_as_id, uint8 cu_cause)
{
  wrrc_qsh_metric_cell_update_cause_s_type *rrc_qsh_cu_cause_ptr;
  rrc_qsh_cu_cause_ptr = ((wrrc_qsh_metric_cell_update_cause_s_type*)rrc_qsh_metric_db[WRRC_QSH_METRIC_CELL_UPDATE_CAUSE].data_ptr);

  if(rrc_qsh_cu_cause_ptr != NULL)
  {
    rrc_qsh_cu_cause_ptr->cu_cause = cu_cause;
    rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_CELL_UPDATE_CAUSE, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
  }
}

#endif


#ifdef FEATURE_WCDMA_QSH_DUMP
/*===========================================================================
FUNCTION wrrc_qsh_dump_conn_mode_globals

DESCRIPTION
  This function serving cell metrics

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
__attribute__((section(".uncompressible.text")))
size_t wrrc_qsh_dump_conn_mode_globals(sys_modem_as_id_e_type wrrc_as_id, uint8* dump_addr, size_t size)
{

  wrrc_qsh_dump_tag_conn_mode_globals conn_mode_globals;

  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_addr,WRRC_QSH_DUMP_TAG_CONN_MODE_GLOBALS,
                                   (sizeof(wrrc_qsh_dump_tag_conn_mode_globals)));

  WCDMA_MEMCPY(&conn_mode_globals,
               sizeof(wrrc_qsh_dump_tag_conn_mode_globals),
               (wrrc_qsh_dump_tag_conn_mode_globals*)dump_addr, sizeof(wrrc_qsh_dump_tag_conn_mode_globals));

  conn_mode_globals.rrc_mode = rrcmcm_get_rrc_mode(wrrc_as_id);
  conn_mode_globals.rrc_curr_state = rrc_get_state(wrrc_as_id);
  conn_mode_globals.rrc_qsh_config_ptr_in_use = config_ptr_in_use;
  if(ordered_config_ptr != NULL)
  {
    conn_mode_globals.rrc_oc_state = ordered_config_ptr->rrc_state_indicator;
    conn_mode_globals.oc_rnti_valid = ordered_config_ptr->mac_dl_parms.rnti_info.rnti_valid;
    conn_mode_globals.oc_hrnti_status = ordered_config_ptr->hrnti_status;
    conn_mode_globals.oc_efach_hrnti_status = ordered_config_ptr->efach_hrnti_status;
    conn_mode_globals.oc_hs_action_in_e_fach = ordered_config_ptr->hs_action_in_e_fach;
    conn_mode_globals.dest_freq = ordered_config_ptr->dest_freq;
#ifdef FEATURE_WCDMA_HS_RACH
    conn_mode_globals.oc_l1_hs_rach_req_mask = ordered_config_ptr->l1_hs_rach_req_mask;
#endif
    #ifdef FEATURE_WCDMA_DC_HSUPA
    conn_mode_globals.sec_eul_is_hho=(ordered_config_ptr->sec_eul_is_hho & 0x01);
    #endif
    conn_mode_globals.dest_psc_present = (ordered_config_ptr->dest_psc_present  & 0x01);
#ifdef FEATURE_WCDMA_HS_RACH
    conn_mode_globals.hs_rach_to_eul_trans = (ordered_config_ptr->hs_rach_to_eul_trans & 0x01);
    conn_mode_globals.common_edch_transmission = (ordered_config_ptr->common_edch_transmission & 0x01);
    conn_mode_globals.hspa_rnti_stored_cell_pch = (ordered_config_ptr->hspa_rnti_stored_cell_pch & 0x01);
#endif
    conn_mode_globals.cpc_dtx_drx_status = (ordered_config_ptr->cpc_dtx_drx_status & 0x01);
    conn_mode_globals.cpc_drx_status = (ordered_config_ptr->cpc_drx_status & 0x01);
    conn_mode_globals.e_dch_transmission = (ordered_config_ptr->e_dch_transmission & 0x01);
    conn_mode_globals.hs_status_in_e_fach = ordered_config_ptr->hs_status_in_e_fach;
  }
  if(current_config_ptr != NULL)
  {
    conn_mode_globals.rrc_cc_state = current_config_ptr->rrc_state_indicator;
    conn_mode_globals.cc_rnti_valid = current_config_ptr->mac_dl_parms.rnti_info.rnti_valid;
    conn_mode_globals.cc_hrnti_status = current_config_ptr->hrnti_status;
    conn_mode_globals.cc_efach_hrnti_status = current_config_ptr->efach_hrnti_status;
    conn_mode_globals.cc_hs_action_in_e_fach = current_config_ptr->hs_action_in_e_fach;
#ifdef FEATURE_WCDMA_HS_RACH
    conn_mode_globals.cc_l1_hs_rach_req_mask = current_config_ptr->l1_hs_rach_req_mask;
#endif
  }
  conn_mode_globals.rrc_oc_set_status = ordered_config.set_status;
  conn_mode_globals.rrc_oc_set_proc = ordered_config.set_by_proc;
  conn_mode_globals.rrc_oc_process_state = ordered_config.process_state;
  conn_mode_globals.toc_usage = transition_config.toc_usage;
  conn_mode_globals.hsdpa_action = rrc_get_hsdpa_action_wo_f3(wrrc_as_id); 
  conn_mode_globals.hsdpa_status = rrc_get_hsdpa_sts_wo_f3(wrrc_as_id);
  conn_mode_globals.is_sib5_change_in_progress = rrcllc_get_sib5_change_in_progress_flag_wo_f3(wrrc_as_id);
  conn_mode_globals.ctch_status = rrcllc_semi_permanent_data.dl_phy_chan.ctch_status;
  conn_mode_globals.revert_old_config = rrcllc_cmd_under_process.chan_config.l1_req_list.l1_req[0].cmd.setup.revert_old_config;


 if(sizeof(conn_mode_globals) >= size)
 {
   WCDMA_MEMCPY((wrrc_qsh_dump_tag_conn_mode_globals*)dump_addr, 
           size,&conn_mode_globals, size);
   return size;
 }
 else
 {
   WCDMA_MEMCPY((wrrc_qsh_dump_tag_conn_mode_globals*)dump_addr, 
     sizeof(conn_mode_globals),&conn_mode_globals, sizeof(conn_mode_globals));
   return ((size_t)sizeof(conn_mode_globals));
 }
}

/*===========================================================================
FUNCTION wrrc_qsh_dump_cctrch_debug

DESCRIPTION
  This function serving cell metrics

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
  __attribute__((section(".uncompressible.text")))
static size_t wrrc_qsh_dump_cctrch_debug(sys_modem_as_id_e_type wrrc_as_id, uint8* dump_addr, size_t size)
{
  wrrc_qsh_dump_tag_cctrch_debug cctrch_debug;
  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_addr,WRRC_QSH_DUMP_TAG_CCTRCH_DEBUG,
                                   (sizeof(wrrc_qsh_dump_tag_cctrch_debug)));
  WCDMA_MEMCPY(&cctrch_debug, sizeof(wrrc_qsh_dump_tag_cctrch_debug),
      (wrrc_qsh_dump_tag_cctrch_debug*)dump_addr, sizeof(wrrc_qsh_dump_tag_cctrch_debug));

  cctrch_debug.used_cctrch_bm = 0; /* To be filled properly later */
  cctrch_debug.to_be_freed_cctrch_bm = 0;
  cctrch_debug.pccpch_is_up = rrcllc_semi_permanent_data.dl_phy_chan.pccpch_is_up;
  cctrch_debug.n_pccpch_is_up = rrcllc_semi_permanent_data.dl_phy_chan.n_pccpch_is_up;
  cctrch_debug.sccpch_is_up = rrcllc_semi_permanent_data.dl_phy_chan.sccpch_is_up;
  cctrch_debug.dl_dpch_is_up = rrcllc_semi_permanent_data.dl_phy_chan.dl_dpch_is_up;
  cctrch_debug.fach_included = rrcllc_semi_permanent_data.dl_phy_chan.fach_included;
  cctrch_debug.pch_included = rrcllc_semi_permanent_data.dl_phy_chan.pch_included;
  cctrch_debug.pch_included_for_sccpch_with_ctch = rrcllc_semi_permanent_data.dl_phy_chan.pch_included_for_sccpch_with_ctch;

  if(sizeof(cctrch_debug) >= size)
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_cctrch_debug*)dump_addr, size, &cctrch_debug, size);
    return size;
  }
  else
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_cctrch_debug*)dump_addr, sizeof(cctrch_debug),
      &cctrch_debug, sizeof(cctrch_debug));
    return ((size_t)sizeof(cctrch_debug));
  }
}

/*===========================================================================
FUNCTION wrrc_qsh_dump_misc_proc_debug

DESCRIPTION
  This function dumps misc globals

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
 __attribute__((section(".uncompressible.text")))

static size_t wrrc_qsh_dump_misc_proc_debug(sys_modem_as_id_e_type wrrc_as_id, uint8* dump_addr, size_t size)
{

  wrrc_qsh_dump_tag_misc_proc misc_globals;

  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_addr,WRRC_QSH_DUMP_TAG_MISC_PROC_DEBUG,
                                   (sizeof(wrrc_qsh_dump_tag_misc_proc)));
 
  WCDMA_MEMCPY(&misc_globals,sizeof(wrrc_qsh_dump_tag_misc_proc),
    (wrrc_qsh_dump_tag_misc_proc*)dump_addr, sizeof(wrrc_qsh_dump_tag_misc_proc));

  misc_globals.rrcmeas_meas_id_bitmask = rrcmeas_get_meas_id_bitmask(wrrc_as_id);

  misc_globals.config_id = rrciho_return_default_config_id(wrrc_as_id);
  misc_globals.rrcmeas_current_substate= (uint8)rrcmeas_return_substate(wrrc_as_id);
  misc_globals.rrcidt_substate = (uint8)rrcdt_return_idt_substate(wrrc_as_id);
  misc_globals.rrcudt_substate = (uint8)rrcdt_return_udt_substate(wrrc_as_id);
  misc_globals.rrcscrr_substate = (uint8)rrcdt_return_scrr_substate(wrrc_as_id);
  misc_globals.rrciho_state= (uint8)rrciho_return_iho_substate(wrrc_as_id);
  misc_globals.rrc_ho_config_type = (uint8)rrciho_return_config_type(wrrc_as_id);
  misc_globals.rrccho_substate = (uint8)rrccho_get_current_substate(wrrc_as_id);
  misc_globals.status = (uint8)rrccho_return_cho_status(wrrc_as_id);
  misc_globals.failure_reason = (uint8)rrccho_return_cho_failure_reason(wrrc_as_id);
  misc_globals.rrcueci_substate = (uint8)rrcueci_return_ueci_substate(wrrc_as_id);
  misc_globals.asu_substate = (uint8)rrcasu_return_curr_substate(wrrc_as_id);
  misc_globals.umi_substate = (uint8)rrcumi_return_substate(wrrc_as_id);
  misc_globals.rrcsmc_substate = (uint8)rrcsmc_return_substate(wrrc_as_id);
  misc_globals.rrcrcr_substate = (uint8)rrcrcr_return_substate(wrrc_as_id);
  misc_globals.rrc_test_mode = (uint8)rrclbt_return_test_mode(wrrc_as_id);
  misc_globals.lbt_substate = (uint8)rrclbt_return_lbt_substate(wrrc_as_id);
//  misc_globals.couec_substate= rrccouec_return_substate();
#ifdef FEATURE_MVS
#ifdef FEATURE_VOC_AMR_WB
  misc_globals.rrc_dl_amr_mode = (uint8)rrc_get_current_codec_mode(wrrc_as_id); /* Need to review this */
#endif
#endif
  misc_globals.ho_from_gsm_eutra = rrciho_return_interrat_type(wrrc_as_id);
  misc_globals.rrcdt_int_data = (uint8)rrcdt_return_cs_domain_open(wrrc_as_id);
  misc_globals.umi_for_srns_relocation = rrcumi_is_srns_in_progress(wrrc_as_id);
  misc_globals.rrciho_abort = rrciho_return_iho_abort(wrrc_as_id);

  if(sizeof(misc_globals) >= size)
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_misc_proc*)dump_addr,size, &misc_globals, size);
    return size;
  }
  else
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_misc_proc*)dump_addr, sizeof(misc_globals),&misc_globals, sizeof(misc_globals));
    return ((size_t)sizeof(misc_globals));
  }

}
/*===========================================================================

FUNCTION   wrrc_qsh_dump_csp_ccm_sib_rce_globals

DESCRIPTION

  This function gets csp/ccm/sib globals
DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_established_rabs_for_a_domain_type structure.

SIDE EFFECTS

  None.

===========================================================================*/
__attribute__((section(".uncompressible.text")))
size_t wrrc_qsh_dump_csp_ccm_sib_rce_globals(sys_modem_as_id_e_type wrrc_as_id, uint8* dump_addr, size_t size)
{
  wrrc_qsh_dump_tag_csp_ccm_sib_rce csp_ccm_sib_rce_globals;
  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_addr,WRRC_QSH_DUMP_TAG_CSP_CCM_SIB_RCE_DEBUG,
                                     (sizeof(wrrc_qsh_dump_tag_csp_ccm_sib_rce)));

  WCDMA_MEMCPY(&csp_ccm_sib_rce_globals,sizeof(wrrc_qsh_dump_tag_csp_ccm_sib_rce),
  (wrrc_qsh_dump_tag_csp_ccm_sib_rce*)dump_addr, sizeof(wrrc_qsh_dump_tag_csp_ccm_sib_rce));
  
  csp_ccm_sib_rce_globals.num_stored_cells_sib_db = sibdb.num_stored_cells;
  csp_ccm_sib_rce_globals.active_cell_index = sibdb.active_cell_index;
  csp_ccm_sib_rce_globals.last_mm_primitive = rrc_csp_int_data.last_mm_primitive; /* rrc_csp_mm_primitive_e_type */
  csp_ccm_sib_rce_globals.curr_type_acq = rrc_csp_int_data.curr_type_acq; /* rrc_csp_acq_e_type */
  csp_ccm_sib_rce_globals.is_valid_nw_sharing = rrc_csp_int_data.curr_acq_entry.is_valid_nw_sharing; 
#ifdef FEATURE_WCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
  csp_ccm_sib_rce_globals.csfb_extended_status = rrc_csp_int_data.csfb_extended_status; /* rrccsp_csfb_status_e_type */
#endif
#if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
  csp_ccm_sib_rce_globals.interrat_type = rrc_csp_int_data.interrat_type; /* rrc_csp_interrat_rat_e_type */
  csp_ccm_sib_rce_globals.bplmn_interrat_type = rrc_csp_int_data.bplmn_interrat_type; /* rrc_csp_interrat_rat_e_type */
#endif
  csp_ccm_sib_rce_globals.bplmn_suspend_proc = rrc_csp_int_data.bplmn_suspend_proc; /* rrc_proc_e_type */
  csp_ccm_sib_rce_globals.bplmn_suspend_cause = rrc_csp_int_data.bplmn_suspend_cause; /* rrc_bplmn_suspend_cause_e_type */
  csp_ccm_sib_rce_globals.rrc_csp_substate = rrc_get_csp_substate(wrrc_as_id); /* rrc_csp_substate_e_type */	
  csp_ccm_sib_rce_globals.curr_scan = rrccsp_get_curr_scan_no_f3(wrrc_as_id);/* rrc_csp_scan_e_type */
  csp_ccm_sib_rce_globals.pending_type = rrc_csp_int_data.pending_type;/* rrc_csp_pending_e_type */
  csp_ccm_sib_rce_globals.cell_camp_status = (uint8)rrcccm_get_curr_cell_camped_status(wrrc_as_id);/* rrc_new_cell_status_e_type */
  csp_ccm_sib_rce_globals.camping_status = rrcccm_get_camping_status(wrrc_as_id);/* rrc_ccm_cell_camping_status_valid_e_type */
  csp_ccm_sib_rce_globals.rrc_ccm_substate = rrcccm_return_substate(wrrc_as_id);/* rrc_ccm_substate_e_type */

  if(sib_events_ptr!=NULL)  
  {
    csp_ccm_sib_rce_globals.srv_bcch_action = sib_events_ptr->srv_bcch.bcch_action;/* sib_bcch_action_e_type */
    csp_ccm_sib_rce_globals.ngh_bcch_action = sib_events_ptr->ngh_bcch.bcch_action;/* sib_bcch_action_e_type */
    csp_ccm_sib_rce_globals.event0_active = sib_events_ptr->event[SIB_SRV_INDEX].event_active & 0x01;
    csp_ccm_sib_rce_globals.event1_active = sib_events_ptr->event[SIB_NGH_INDEX].event_active & 0x01;

  }
  
  csp_ccm_sib_rce_globals.rrcrce_substate = rrcrce_get_rce_sub_state(wrrc_as_id);/* rrcrce_substate_e_type */
  csp_ccm_sib_rce_globals.rrc_establishment_cause = rrcdata_misc_info.rrc_establishment_cause; /* rrc_establish_cause_e_type */
  csp_ccm_sib_rce_globals.rrc_v_300 = rrcdata_timers_counters_info.rrc_v_300;
  csp_ccm_sib_rce_globals.rrc_release_cause = rrcdata_misc_info.rrc_release_cause; /* rrc_rel_cause_e_type */
  csp_ccm_sib_rce_globals.rrcrce_mm_abort_cause = rrcrce_get_mm_abort_cause(wrrc_as_id);/* rrc_abort_cause_e_type */
  csp_ccm_sib_rce_globals.init_cell_sel_ind_during_wtointerrat = rrc_csp_int_data.init_cell_sel_ind_during_wtointerrat & 0x01 ;
  csp_ccm_sib_rce_globals.bplmn_guard_srch_tmr_expired = rrc_csp_int_data.bplmn_guard_srch_tmr_expired & 0x01 ;
  csp_ccm_sib_rce_globals.bplmn_srch_tmr_expired = rrc_csp_int_data.bplmn_srch_tmr_expired & 0x01 ; 
#ifdef FEATURE_3GPP_CSFB
  csp_ccm_sib_rce_globals.rrc_csfb_call_status = rrc_csp_int_data.rrc_csfb_call_status & 0x01 ;
#endif
  csp_ccm_sib_rce_globals.rrcrce_send_abort_cnf = rrcrce_return_send_abort_cnf(wrrc_as_id) & 0x01 ;
  csp_ccm_sib_rce_globals.rrcrce_abort_ind_sent = rrcrce_return_abort_ind_sent_status(wrrc_as_id);
  csp_ccm_sib_rce_globals.update_sibs_event_pending = rrcsib_return_update_sibs_event_pending(wrrc_as_id) & 0x01 ;
  csp_ccm_sib_rce_globals.sib_db_empty = sibdb.database_empty & 0x01;
  csp_ccm_sib_rce_globals.ue_in_deep_sleep = rrc_csp_int_data.ue_in_deep_sleep & 0x01;
  csp_ccm_sib_rce_globals.acq_db_empty = rrc_csp_int_data.acq_db_empty & 0x01;
  csp_ccm_sib_rce_globals.bplmn_new_srch = rrc_csp_int_data.bplmn_new_srch & 0x01;
  csp_ccm_sib_rce_globals.bplmn_srch_cnf = rrc_csp_int_data.bplmn_srch_cnf & 0x01;
  csp_ccm_sib_rce_globals.do_lfs_scan = rrc_csp_int_data.do_lfs_scan & 0x01;
  csp_ccm_sib_rce_globals.stop_pending_for_wtog_abort = rrc_csp_int_data.stop_pending_for_wtog_abort & 0x01;
  csp_ccm_sib_rce_globals.allow_lte_resel = rrc_csp_int_data.allow_lte_resel & 0x01;
  csp_ccm_sib_rce_globals.bplmn_data_saved = rrc_csp_int_data.bplmn_data_saved & 0x01;
  csp_ccm_sib_rce_globals.ue_in_conn_oos = rrc_csp_int_data.ue_in_conn_oos & 0x01;
  csp_ccm_sib_rce_globals.dch_to_fach_pch_tmr_expd = rrc_csp_int_data.dch_to_fach_pch_tmr_expd & 0x01;

  if(sizeof(csp_ccm_sib_rce_globals) >= size)
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_csp_ccm_sib_rce*)dump_addr,size, &csp_ccm_sib_rce_globals, size);
    return size;
  }
  else
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_csp_ccm_sib_rce*)dump_addr, 
      sizeof(csp_ccm_sib_rce_globals), &csp_ccm_sib_rce_globals, sizeof(csp_ccm_sib_rce_globals));
    return ((size_t)sizeof(csp_ccm_sib_rce_globals));
  }
}
/*===========================================================================

FUNCTION   wrrc_qsh_dump_mcm_globals

DESCRIPTION

  This function gets mcm globals
DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_established_rabs_for_a_domain_type structure.

SIDE EFFECTS

  None.

===========================================================================*/
__attribute__((section(".uncompressible.text")))

size_t wrrc_qsh_dump_mcm_globals(sys_modem_as_id_e_type wrrc_as_id, uint8* dump_addr, size_t size)
{
  wrrc_qsh_dump_tag_mcm mcm_globals;
   qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_addr,WRRC_QSH_DUMP_TAG_MCM_DEBUG,
                                   (sizeof(wrrc_qsh_dump_tag_mcm)));

  WCDMA_MEMCPY(&mcm_globals,sizeof(wrrc_qsh_dump_tag_mcm),
          (wrrc_qsh_dump_tag_mcm*)dump_addr, sizeof(wrrc_qsh_dump_tag_mcm));
  
  mcm_globals.rrcmcm_cmd_store_mm_cmdid = rrcmcm_return_rrcmcm_cmd_flag(wrrc_as_id);/* rrc_cmd_e_type */
  mcm_globals.rrcmcm_cmd_store_interat_resel_rr_cmdid = rrcmcm_return_gsm_irat_cmd(wrrc_as_id);/* rrc_rr_cmd_e_type */
#ifdef FEATURE_LTE_TO_WCDMA
  mcm_globals.rrcmcm_cmd_store_lte_irat_cmdid = rrcmcm_return_lte_irat_cmd(wrrc_as_id);/* rrc_cmd_e_type */
#endif
  mcm_globals.rrcmcm_state = rrcmcm_return_mcm_state(wrrc_as_id);/* rrcmcm_state_e_type */  
  mcm_globals.rrcmcm_cmd_store_abort_flag = rrcmcm_return_rrcmcm_abort_flag(wrrc_as_id);/* rrcmcm_abort_flag_e_type */
  mcm_globals.rrcmcm_procedure_id = rrcmcm_return_procedure_id(wrrc_as_id);/* rrcmcm_abort_flag_e_type */ /* Destination procedure for change mode confirmation */
  mcm_globals.rrcmcm_stop_cause = rrcmcm_return_stop_cause(wrrc_as_id);/* rrc_stop_cause_e_type */
#ifdef FEATURE_DUAL_SIM
  mcm_globals.qsh_rrcwrm_unlock_type = rrcwrm_get_unlock_type(wrrc_as_id); /* trm_unlock_event_enum_t */
  mcm_globals.wcdma_device_mode = rrc_dsds_int_data.wcdma_device_mode; /* sys_modem_device_mode_e_type */
  mcm_globals.wcdma_is_ue_in_dual_standby = rrc_dsds_int_data.wcdma_is_ue_in_dual_standby & 0x01;
  mcm_globals.wcdma_is_ue_in_sxlte_mode = rrc_dsds_int_data.wcdma_is_ue_in_sxlte_mode & 0x01;
#endif
  mcm_globals.rrc_dual_mode_enabled = (rrcmcm_is_dualmode_enabled_wo_f3(wrrc_as_id) & 0x01);
  #if defined (FEATURE_WCDMA_TO_LTE) || defined (FEATURE_LTE_TO_WCDMA)
  mcm_globals.rrc_lte_mode_enabled = (rrcmcm_is_lte_mode_enabled_wo_f3(wrrc_as_id) & 0x01);
  #endif
  mcm_globals.rrc_is_ue_in_sglte_mode = rrcmcm_return_ue_in_sg_lte_mode(wrrc_as_id) & 0x01;
  mcm_globals.rrcmcm_send_stop_cnf_to_mm_flag = rrcmcm_return_send_stop_cnf_to_mm_flag(wrrc_as_id) &0x01;
  mcm_globals.rrcmcm_est_req_saved_flag = rrcmcm_return_est_req_saved_flag(wrrc_as_id) & 0x01;

  if(sizeof(mcm_globals) >= size)
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_mcm*)dump_addr, size, &mcm_globals, size);
    return size;
  }
  else
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_mcm*)dump_addr, sizeof(mcm_globals), &mcm_globals, sizeof(mcm_globals));
    return ((size_t)sizeof(mcm_globals));
  }
  
}

/*===========================================================================
FUNCTION wrrc_qsh_dump_ota_and_cmds

DESCRIPTION
  This function dumps otas and cmds

DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
__attribute__((section(".uncompressible.text")))
size_t wrrc_qsh_dump_ota_and_cmds(sys_modem_as_id_e_type wrrc_as_id, uint8* dump_addr, size_t size)
{
  wrrc_qsh_dump_tag_cmds_otas_debug cmds_otas_debug;
  uint8 idx;
  uint8 curr_idx = 0;
  uint16 offset = 0;
  uint8 cnt = 0;
  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_addr,WRRC_QSH_DUMP_TAG_CMD_OTAS_DEBUG,
      (sizeof(wrrc_qsh_dump_tag_cmds_otas_debug)));
  
  WCDMA_MEMCPY(&cmds_otas_debug,sizeof(wrrc_qsh_dump_tag_cmds_otas_debug),
    (wrrc_qsh_dump_tag_cmds_otas_debug*)dump_addr, sizeof(wrrc_qsh_dump_tag_cmds_otas_debug));
  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_index + RRC_CRASH_DEBUG_MM_CMD_CNT - RRC_CRASH_DEBUG_OPT_MM_CMD_CNT) % RRC_CRASH_DEBUG_MM_CMD_CNT);
  
  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_MM_CMD_CNT; idx++)
  {
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[idx].cmd_id=
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[curr_idx].cmd_id;
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.mm_to_rrc_debug_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_MM_CMD_CNT);
  }

  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_index + RRC_CRASH_DEBUG_MM_CMD_CNT - RRC_CRASH_DEBUG_OPT_MM_CMD_CNT) % RRC_CRASH_DEBUG_MM_CMD_CNT);

  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_MM_CMD_CNT; idx++)
  {
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_cmds[idx].cmd_id=
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_cmds[curr_idx].cmd_id;
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_mm_cmds.rrc_to_mm_debug_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_MM_CMD_CNT);
  }

  /* Filling RR cmds (rrc_crash_debug_rr_cmds)*/
  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_index + RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT - RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT);

  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT; idx++)
  {
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[idx].cmd_id=
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[curr_idx].cmd_id;
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rr_to_rrc_debug_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT);
  }
  /* Filling RR cmds (rrc_crash_debug_rr_cmds)*/

  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_index + RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT - RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT);

  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT; idx++)
  {
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_cmds[idx].cmd_id=
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_cmds[curr_idx].cmd_id;
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_rr_cmds.rrc_to_rr_debug_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT);
  }

  /* Filling LTE cmds */
  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_index + RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT - RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT) % RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT);

  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT; idx++)
  {
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[idx].cmd_id=
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[curr_idx].cmd_id;
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.lte_to_rrc_debug_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT);
  }
  /* Filling RR cmds (rrc_crash_debug_rr_cmds)*/
  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_index + RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT - RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT) % RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT);

  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_IRAT_RRC_CMD_CNT; idx++)
  {
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_cmds[idx].cmd_id=
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_cmds[curr_idx].cmd_id;
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_lte_cmds.rrc_to_lte_debug_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_LTE_IRAT_CMD_CNT);
  }
        
  
  /* Filling internal cmds */
  if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index > RRC_CRASH_DEBUG_OPT_INTERNAL_CMD_CNT)
  {
    curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index - RRC_CRASH_DEBUG_OPT_INTERNAL_CMD_CNT) % RRC_CRASH_DEBUG_INTERNAL_CMD_CNT);
  }
  else
  {
    curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.rrc_crash_debug_index + RRC_CRASH_DEBUG_INTERNAL_CMD_CNT - RRC_CRASH_DEBUG_OPT_INTERNAL_CMD_CNT) % RRC_CRASH_DEBUG_INTERNAL_CMD_CNT);
  }  
  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_INTERNAL_CMD_CNT; idx++)
  {
     cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_int_cmds.int_cmd[idx].cmd_id=
     rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[curr_idx].cmd_id;
     cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_int_cmds.int_cmd[idx].timeticks=
     (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_int_cmds.int_cmd[curr_idx].timeticks) >> 8) & 0xFFFF);
     curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_INTERNAL_CMD_CNT) ; 
  }
  
  /* Filling L1 cmd*/
  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_index + RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT - RRC_CRASH_DEBUG_OPT_L1_RRC_CMD_CNT) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT);
 
  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_L1_RRC_CMD_CNT; idx++)
  {
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[idx].cmd_id=
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[curr_idx].cmd_id;
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.l1_to_rrc_debug_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT); 
  }
  
  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_index + RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT - RRC_CRASH_DEBUG_OPT_L1_RRC_CMD_CNT) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT);

  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_L1_RRC_CMD_CNT; idx++)
  {
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_cmds[idx].cmd_id=
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_cmds[curr_idx].cmd_id;
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_l1_cmds.rrc_to_l1_debug_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)  ;
  }

  /* Filling LLC cmd */
  curr_idx = ((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.index + RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT - RRC_CRASH_DEBUG_OPT_LLC_CMD_CNT) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT);
  
  for(idx= 0; idx<RRC_CRASH_DEBUG_OPT_LLC_CMD_CNT; idx++)
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[curr_idx].dest == L1_PHY)
    {
      cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_llc_cmds.rrc_llc_cmds[idx].l1_llc_cmd = 
      	rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[curr_idx].cmds.l1_llc_cmd;
    }
    else if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[curr_idx].dest == L2_UL)
    {
      cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_llc_cmds.rrc_llc_cmds[idx].l1_llc_cmd = 
        (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[curr_idx].cmds.l2_llc_ul_cmd)|0xf000;
    }
    else 
    {
      cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_llc_cmds.rrc_llc_cmds[idx].l1_llc_cmd =
        (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[curr_idx].cmds.l2_llc_dl_cmd)|0x0f00;
    }
    cmds_otas_debug.rrc_crash_debug_info_opt.rrc_crash_debug_llc_cmds.rrc_llc_cmds[idx].timeticks=
    (((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_llc_cmds.rrc_llc_cmds[curr_idx].timeticks) >> 8) & 0xFFFF);
    curr_idx= ((curr_idx+1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)  ;
  }  
    
  /* Filling from last received OTA */
  curr_idx = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.rrc_crash_debug_index + RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT - 1)%RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
  offset = 0;
  cnt = 0; /* Protection to avoid infinite looping */
  while(((offset+sizeof(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].sdu_type)+
    sizeof(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length)+
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length) < RRC_CRASH_DEBUG_OPT_OTA_SIZE) && 
    (cnt < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)) /* Make changes to fill the truncated OTA also */
  {
    if((rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length > 0) && 
      (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_crash_debug_otas != NULL))
    {
      WCDMA_MEMCPY(&(cmds_otas_debug.ota[offset]),sizeof(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].sdu_type),
      &(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].sdu_type), sizeof(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].sdu_type));
      offset += sizeof(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].sdu_type);
      WCDMA_MEMCPY(&(cmds_otas_debug.ota[offset]),sizeof(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length),
      &(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length),sizeof(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length));
      offset += sizeof(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length);
      WCDMA_MEMCPY(&(cmds_otas_debug.ota[offset]),rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length,
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_crash_debug_otas,rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length);		
      offset += rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[curr_idx].rrc_ota_length;
    }
    if(curr_idx == 0)
    {
      curr_idx = RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT-1;
    }
    else
    {
      curr_idx = curr_idx-1;
    }
    cnt++;
  }
  if(sizeof(cmds_otas_debug) >= size)
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_cmds_otas_debug*)dump_addr,size, &cmds_otas_debug, size);
    return size;
  }
  else
  {
    WCDMA_MEMCPY((wrrc_qsh_dump_tag_cmds_otas_debug*)dump_addr, sizeof(cmds_otas_debug),
      &cmds_otas_debug, sizeof(cmds_otas_debug));
    return ((size_t)sizeof(cmds_otas_debug));
  }
}

#endif
#if defined(FEATURE_WCDMA_QSH_EVENT_METRIC) || defined(FEATURE_WCDMA_QSH_DUMP)

/*===========================================================================
FUNCTION wrrc_qsh_cb
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_QSH_MDUMP
QSH_MDUMP_FN_ATTR
#endif
void wrrc_qsh_cb(qsh_client_cb_params_s* qsh_cb_params_ptr)
{
  uint32 tag_bitmask = 0,tag_id = 0;
  rrc_cmd_type *cmd_ptr;
  uint32 tag_size;
  uint8* dump_addr;
  size_t size_used = 0;
  size_t size_ret = 0;
  size_t size_given = 0;
  sys_modem_as_id_e_type wrrc_as_id = SYS_MODEM_AS_ID_1;
  qsh_client_action_done_s cb_done_ptr;

#ifdef FEATURE_WCDMA_QSH_DUMP
  if(qsh_cb_params_ptr->action == QSH_ACTION_DUMP_COLLECT)
  {
    dump_addr = (uint8*)qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.addr; 
    size_given = qsh_cb_params_ptr->action_params.dump_collect.dump_iovec.size_bytes;
    wrrc_dump_start_addr = dump_addr;
    tag_bitmask = qsh_cb_params_ptr->action_params.dump_collect.dump_tag_mask;
    wrrc_dump_tag_mask = tag_bitmask;
    tag_id = 0;
    for(wrrc_as_id=SYS_MODEM_AS_ID_1; wrrc_as_id<MAX_RRC_STACKS; wrrc_as_id++)
    {
      if(size_given != 0)
      {
        while(tag_bitmask != 0)
        {
          if((tag_bitmask & (1 << tag_id)) && (size_used < size_given))
          {
            if(tag_id < WRRC_QSH_DUMP_TAG_MAX)
            {
              switch(tag_id)
              {
                case WRRC_QSH_DUMP_TAG_CONN_MODE_GLOBALS:
                  size_ret = wrrc_qsh_dump_conn_mode_globals(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                case WRRC_QSH_DUMP_TAG_NV_VALUES:
                  size_ret = wrrc_qsh_dump_nv_vals(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;   
                  break;
                case WRRC_QSH_DUMP_TAG_CSP_CCM_SIB_RCE_DEBUG:
                  size_ret = wrrc_qsh_dump_csp_ccm_sib_rce_globals(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                case WRRC_QSH_DUMP_TAG_LLC_DEBUG:
                  size_ret = wrrc_qsh_dump_llc_debug(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                case WRRC_QSH_DUMP_TAG_CMD_OTAS_DEBUG:
                  size_ret = wrrc_qsh_dump_ota_and_cmds(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                case WRRC_QSH_DUMP_TAG_CU_DEBUG:
                  size_ret = wrrc_qsh_dump_cu_debug(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                case WRRC_QSH_DUMP_TAG_RB_PROC_DEBUG:
                  size_ret = wrrc_qsh_dump_rb_proc_debug(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                case WRRC_QSH_DUMP_TAG_MCM_DEBUG:
                  size_ret = wrrc_qsh_dump_mcm_globals(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                case WRRC_QSH_DUMP_TAG_MISC_PROC_DEBUG:
                  size_ret = wrrc_qsh_dump_misc_proc_debug(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                case WRRC_QSH_DUMP_TAG_CCTRCH_DEBUG:
                  size_ret = wrrc_qsh_dump_cctrch_debug(wrrc_as_id, dump_addr, size_given-size_used);
                  dump_addr+=size_ret;
                  size_used += size_ret;
                  break;
                default:
                  break;
              }
            }
          }
          tag_bitmask &= (~(1 << tag_id));
          tag_id++;
        }
      }
      /* tag bit mask is set again for the next sub */
      tag_bitmask = qsh_cb_params_ptr->action_params.dump_collect.dump_tag_mask;
      tag_id = 0;
    }
    qsh_client_action_done_init(&cb_done_ptr);
    cb_done_ptr.cb_params_ptr = qsh_cb_params_ptr;
    cb_done_ptr.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
    cb_done_ptr.params.dump_collect.size_written_bytes = size_used;
    qsh_client_action_done(&cb_done_ptr);
    return ; /* dumps should be collected in qsh context */
  }
  
  /* If it is metric cfg, post cmd only to the respective sub task */
  else 
#endif

  if(qsh_cb_params_ptr->action == QSH_ACTION_METRIC_CFG)
  {
    wrrc_as_id = qsh_cb_params_ptr->action_params.metric_cfg.subs_id;
    if(qsh_cb_params_ptr->action_mode == QSH_ACTION_MODE_SYNC_REQUIRED)
    {
      WRRC_MSG0_HIGH("METRIC_DEBUG: QSH_ACTION:CFG, QSH_ACTION_MODE:SYNC_REQUIRED");
      //SUBAG, Need mutex protection, but it is difficult to add here. Inform with QSH if they will call with SYN REQD for STOP.
      rrc_process_qsh_cmd(wrrc_as_id, qsh_cb_params_ptr);
    }
    else
    {
      cmd_ptr = (rrc_cmd_type *)rrc_malloc(sizeof(qsh_client_cb_params_s) + sizeof(rrc_cmd_hdr_type));
      cmd_ptr->cmd_hdr.cmd_id = RRC_QSH_PROCESS_CMD; /* Fill in the Command Id */
      WCDMA_MEMCPY(&(cmd_ptr->cmd.rrc_qsh_cmd),sizeof(qsh_client_cb_params_s),qsh_cb_params_ptr,sizeof(qsh_client_cb_params_s));		
      rrc_put_int_cmd(wrrc_as_id, cmd_ptr);
    }
  }
#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
  else if(qsh_cb_params_ptr->action == QSH_ACTION_CFG)
  {
    wrrc_as_id = (sys_modem_as_id_e_type)((qsh_cb_params_ptr->action_params.cfg.cmd_code >> QSH_EVENT_CMD_CODE_SUB_ID)& 0xFF);
#ifdef FEATURE_DUAL_WCDMA
    if((wrrc_as_id >= 0) && (wrrc_as_id < MAX_RRC_STACKS))
#endif
    {
      /* Check with QSH if action mode will be SYNC required for event config */
      if(qsh_cb_params_ptr->action_mode == QSH_ACTION_MODE_SYNC_REQUIRED)
      {
        rrc_process_qsh_cmd(wrrc_as_id, qsh_cb_params_ptr);
      }
      else
      {
        cmd_ptr = (rrc_cmd_type *)rrc_malloc(sizeof(qsh_client_cb_params_s) + sizeof(rrc_cmd_hdr_type));
        cmd_ptr->cmd_hdr.cmd_id = RRC_QSH_PROCESS_CMD; /* Fill in the Command Id */
        WCDMA_MEMCPY(&(cmd_ptr->cmd.rrc_qsh_cmd),sizeof(qsh_client_cb_params_s),qsh_cb_params_ptr,sizeof(qsh_client_cb_params_s));		
        rrc_put_int_cmd(wrrc_as_id, cmd_ptr);
      }
    }
  }
#endif
  /* If it is timer expiry post cmds to the task which is holding lock for W+W case*/
  else if(qsh_cb_params_ptr->action == QSH_ACTION_METRIC_TIMER_EXPIRY)
  {
    /* Timer expiry is common for both the subs */
    /* call process qsh cmd api once for each task  if action mode is SYNC REQD */
    /* Post cmd to both the tasks if sync mode is not SYNC REQD */
    for(wrrc_as_id=SYS_MODEM_AS_ID_1; wrrc_as_id<MAX_RRC_STACKS; wrrc_as_id++)
    {
      if(qsh_cb_params_ptr->action_mode == QSH_ACTION_MODE_SYNC_REQUIRED)
      {
        rrc_process_qsh_cmd(wrrc_as_id, qsh_cb_params_ptr);
      }
      else
      {
        /* If RRC is waiting for lock request and the QSH action is for timer expiry, ignore the call back */
        if(IS_RRC_IN_LOCK_REQ_STATE)
        { 
          WRRC_MSG1_HIGH("METRIC_DEBUG: WRRC in LOCK REQ, action: %d, call back is ignored for sub %d",wrrc_as_id);
        }
        else
        {
          cmd_ptr = (rrc_cmd_type *)rrc_malloc(sizeof(qsh_client_cb_params_s) + sizeof(rrc_cmd_hdr_type));
          cmd_ptr->cmd_hdr.cmd_id = RRC_QSH_PROCESS_CMD; /* Fill in the Command Id */
          WCDMA_MEMCPY(&(cmd_ptr->cmd.rrc_qsh_cmd),sizeof(qsh_client_cb_params_s),qsh_cb_params_ptr,sizeof(qsh_client_cb_params_s));		
          rrc_put_int_cmd(wrrc_as_id, cmd_ptr);
        }
      }
    }
  }
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
  #error code not present
#endif /*FEATURE_QSH_EVENT_NOTIFY_HANDLER*/
#ifdef FEATURE_QSH_MDUMP
  else if(qsh_cb_params_ptr->action == QSH_ACTION_MDUMP)
  {
    wrrc_as_id = qsh_cb_params_ptr->action_params.mdump.crash_subs_id;
#ifdef FEATURE_DUAL_WCDMA
    if((wrrc_as_id >= 0) && (wrrc_as_id < MAX_RRC_STACKS))
#endif
    {
#ifdef FEATURE_DUAL_WCDMA
      sys_modem_as_id_e_type wrrc_alt_sub = (wrrc_as_id == SYS_MODEM_AS_ID_1)?SYS_MODEM_AS_ID_2:SYS_MODEM_AS_ID_1;
#endif
      /*Collect all the global variables needed based on the current state*/
      wrrc_qsh_mdump_collect(wrrc_as_id);
#ifdef FEATURE_DUAL_WCDMA
      /*Collect the data for alternate subscription*/
      wrrc_qsh_mdump_collect(wrrc_alt_sub);
#endif
}
    else
    {
      /*Sub id provided by qsh is invalid, collect sub-1 first followed by sub-2*/
      wrrc_qsh_mdump_collect(SYS_MODEM_AS_ID_1);
#ifdef FEATURE_DUAL_WCDMA
      wrrc_qsh_mdump_collect(SYS_MODEM_AS_ID_2);
#endif
    }
/* No need to notify action done */
  }
#endif /*FEATURE_QSH_MDUMP*/
}
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION wrrc_collect_mdump
  
DESCRIPTION
  This function collects the mdump for wrrc


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR 
void wrrc_qsh_mdump_collect(sys_modem_as_id_e_type wrrc_as_id)
{
  /* Collect all the bare minimum globals needed for debugging */
  qsh_mdump_collect_high(&(rrcdata_logging_info.rrc_crash_debug_info),sizeof(rrc_crash_debug_info_type));
  qsh_mdump_collect_high(wcdma_rrc_tcb_ptr,sizeof(rex_tcb_type));

  wrrc_qsh_mdump_collect_rrcmcm_data(wrrc_as_id);
  wrrc_qsh_mdump_collect_rrciho_data(wrrc_as_id);
  wrrc_qsh_mdump_collect_rrcwrm_data(wrrc_as_id);
  wrrc_qsh_mdump_collect_rrccsp_data(wrrc_as_id);
  wrrc_qsh_mdump_collect_rrcnv_data(wrrc_as_id);
  wrrc_qsh_mdump_collect_rrcdispatcher_data(wrrc_as_id);
  wrrc_qsh_mdump_collect_rrccho_data(wrrc_as_id);

  /*If WRRC was active, collect configuration related globals*/
  if(rrcmcm_get_rrc_mode(wrrc_as_id) != RRC_MODE_INACTIVE)
  {
    /* Prioritize WL1 client if RRC is active */
    qsh_mdump_prioritize_client(QSH_CLT_WRRC,QSH_CLT_WFW);
    qsh_mdump_prioritize_client(QSH_CLT_WRRC,QSH_CLT_WL1);
    qsh_mdump_prioritize_client(QSH_CLT_WRRC,QSH_CLT_WRLC);
    qsh_mdump_prioritize_client(QSH_CLT_WRRC,QSH_CLT_WMAC);


    wrrc_qsh_mdump_collect_rrcdata_globals(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcllc_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrclcm_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcllcoc_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrccu_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcrbcommon_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcsmc_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcasn1_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcasu_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcrbe_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcrbreconfig_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcpcreconfig_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrctcreconfig_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcrbr_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrctfcc_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcumi_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcmisc_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcscmgr_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcllcpcie_data(wrrc_as_id);

    wrrc_qsh_mdump_collect_rrcrce_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcdt_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcrcr_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcccm_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcueci_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcmeas_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcdormancy_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcsibdb_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcueci_data(wrrc_as_id);
    wrrc_qsh_mdump_collect_rrcff_data(wrrc_as_id);
  }
}
#endif
/*===========================================================================
FUNCTION wrrc_qsh_metric_config_start
  
DESCRIPTION
  This function is called at rrc task init to configure metrics by default.


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/

uint32 wrrc_qsh_get_max_size_for_dump(void)
{
  uint32 size = 0;
  size = sizeof(wrrc_qsh_dump_tag_conn_mode_globals)+
          sizeof(wrrc_qsh_dump_tag_nv_values)+
          sizeof(wrrc_qsh_dump_tag_csp_ccm_sib_rce)+
          sizeof(wrrc_qsh_dump_tag_llc_debug)+
          sizeof(wrrc_qsh_dump_tag_cmds_otas_debug)+
          sizeof(wrrc_qsh_dump_tag_cu)+
          sizeof(wrrc_qsh_dump_tag_rb_proc)+
          sizeof(wrrc_qsh_dump_tag_mcm)+
          sizeof(wrrc_qsh_dump_tag_misc_proc)+
          sizeof(wrrc_qsh_dump_tag_cctrch_debug);
  
  return size;
}

#endif
#ifdef FEATURE_WCDMA_QSH_EVENT_METRIC
/* Logging all DL SDU IND */
/*===========================================================================
FUNCTION rrcqsh_get_sec_hsdpa_status
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrcqsh_get_sec_hsdpa_status(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  /*dchsdpa status*/
  if(rrcllcpcie_return_num_sec_hsdpa_carriers(wrrc_as_id, ordered_config_ptr))
  {
    overall_ota_info_ptr->dchsdpa_status = 1; 
  }
  else
  {
    overall_ota_info_ptr->dchsdpa_status = 0; 
  }
}
/*===========================================================================
FUNCTION rrcqsh_get_hsdpa_status
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrcqsh_get_hsdpa_status(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  /* hsdpa_status*/
  if(((rrcdata_hs_info.hsdpa_action != HSDPA_STOP) && (rrcdata_hs_info.hsdpa_status == HSDPA_ACTIVE)) || (rrcdata_hs_info.hsdpa_action == HSDPA_START))
  {
    overall_ota_info_ptr->hsdpa_status = 1; 
  }
  else
  {
    overall_ota_info_ptr->hsdpa_status = 0;
  }
}
#ifdef FEATURE_WCDMA_DC_HSUPA
/*===========================================================================
FUNCTION rrc_get_sec_edch_transmission
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrc_get_sec_edch_transmission(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  /*sec_edch_transmission*/
  overall_ota_info_ptr->sec_edch_transmission = (ordered_config_ptr->sec_e_dch_transmission); 
}
#endif
/*===========================================================================
FUNCTION rrc_get_edch_transmission
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrc_get_edch_transmission(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  /* edch_transmission */
  overall_ota_info_ptr->edch_transmission = (ordered_config_ptr->e_dch_transmission);
}
/*===========================================================================
FUNCTION rrc_get_dtx_drx_status
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrc_get_dtx_drx_status(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  /* cpc_dtx_drx_status */
  overall_ota_info_ptr->cpc_dtx_drx_status = (ordered_config_ptr->cpc_dtx_drx_status);
}

/*===========================================================================
FUNCTION rrc_get_hho_type
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrc_get_hho_type(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id , rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  /* hho_type */
  if(ordered_config_ptr->rrc_state_indicator == RRC_STATE_CELL_DCH)
  {
   if(ordered_config_ptr->dest_freq_present)
   {
     if(ordered_config_ptr->l1_dl_chan_parms.dl_common.dpch_common.timing_ind == L1_TIMING_IND_INIT)
     {
       overall_ota_info_ptr->hho_type = WRRC_TIMING_INIT;
     }
     else if(ordered_config_ptr->l1_dl_chan_parms.dl_common.dpch_common.timing_ind == L1_TIMING_IND_MAINTAIN)
     {
       overall_ota_info_ptr->hho_type = WRRC_TIMING_MAINTAINED;
     }
   }
   else
   {
     overall_ota_info_ptr->hho_type = WRRC_NON_HHO;
   }
  }
  else if(ordered_config_ptr->dest_freq_present)
  {
    overall_ota_info_ptr->hho_type = WRRC_REDIRECTION;
  }
}
/*===========================================================================
FUNCTION rrc_get_oc_set_status
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
static void rrc_get_oc_set_status(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  /* oc set status or state indicator */
  overall_ota_info_ptr->oc_set_status = ordered_config.set_status;	
}
  
/*===========================================================================
FUNCTION rrcqsh_get_num_rbs_deleted
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrcqsh_get_num_rbs_deleted(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  /*RB can be deleted only by RBRelease or CUC */
  /* rrccu_rb_release_info */
  if(proc_id == RRC_PROCEDURE_RBR)
  {
    overall_ota_info_ptr->num_rb_rel = rrcrbr_get_num_rbs_to_release(wrrc_as_id);
  }
  else if(proc_id == RRC_PROCEDURE_CU)
  {
    overall_ota_info_ptr->num_rb_rel = rrccu_get_num_rb_rel(wrrc_as_id);
  }
}
/*===========================================================================
FUNCTION rrc_get_num_rbs_recfg
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrc_get_num_rbs_recfg(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  if(proc_id == RRC_PROCEDURE_RBRC)
  {
    overall_ota_info_ptr->num_rb_recfg = rrcrbrc_get_num_rbs_reccfg(wrrc_as_id);  
  }
  else if(proc_id == RRC_PROCEDURE_RBR)
  {
    overall_ota_info_ptr->num_rb_recfg = rrcrbr_get_num_rbs_to_recfg(wrrc_as_id);  
  }
  /* Other procedures doesnt have direct info from globals */
  /* for RBE, pick from rrc_cmd, To be done */
}
/*===========================================================================
FUNCTION rrc_srns_relocation
  
DESCRIPTION
  This function is called by QSH module to collect events/metrics/dumps
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

static void rrc_srns_relocation(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_overall_ota_info_s* overall_ota_info_ptr)
{
  if(proc_id == RRC_PROCEDURE_RBE)
  {
    overall_ota_info_ptr->srns_relocation_invloved = rrbrbe_srns_relocation(wrrc_as_id);
  }
  else if(proc_id == RRC_PROCEDURE_RBRC)
  {
    overall_ota_info_ptr->srns_relocation_invloved = rrcrbrc_get_srns_relocation(wrrc_as_id);
  }
  else if(proc_id == RRC_PROCEDURE_PCR)
  {
    overall_ota_info_ptr->srns_relocation_invloved = rrcpcr_get_srns_relocation(wrrc_as_id);
  }
  else if(proc_id == RRC_PROCEDURE_RBR)
  {
    overall_ota_info_ptr->srns_relocation_invloved = rrcrbr_get_srns_relocation(wrrc_as_id);
  }
  else if(proc_id == RRC_PROCEDURE_TCR)
  {
    overall_ota_info_ptr->srns_relocation_invloved = rrctcr_get_srns_relocation(wrrc_as_id);
  }
  else if(proc_id == RRC_PROCEDURE_CU)
  {
    overall_ota_info_ptr->srns_relocation_invloved = rrccu_get_cuc_for_srns_relocation(wrrc_as_id);
  }
  /* ASU RCE can do SRNS ?? */
}

/*===========================================================================
FUNCTION rrc_qsh_log_paging_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_paging_params(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id, rrc_log_pg_ota *page_info)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr!= NULL)
  {
    if(proc_id == RRC_PROCEDURE_PG1)
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.paging_type1.sys_info_mod = page_info->sys_info_mod ;   
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.paging_type1.page_record_match = page_info->page_record_match;   
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.paging_type1.cn_id = page_info->cn_id;                  
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.paging_type1.paging_cause = page_info->paging_cause; 
    }
    else
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.paging_type2.sys_info_mod = page_info->sys_info_mod ;       
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.paging_type2.page_record_match = page_info->page_record_match;     
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.paging_type2.cn_id = page_info->cn_id;                
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.paging_type2.paging_cause = page_info->paging_cause;   
    }
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_conn_rej_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_conn_rej_params(sys_modem_as_id_e_type wrrc_as_id, rrc_log_rrc_conn_rej_ota *conn_rej_params)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_rej_ota.conn_rej_cause = conn_rej_params->conn_rej_cause ;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_rej_ota.wait_time = conn_rej_params->wait_time ;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_rej_ota.redir_info_present = conn_rej_params->redir_info_present ;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_rej_ota.redir_rat = conn_rej_params->redir_rat ;                     
  }  
}

/*===========================================================================
FUNCTION rrc_qsh_log_conn_rel_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_conn_rel_params(sys_modem_as_id_e_type wrrc_as_id, rrc_log_rrc_conn_rel_ota *conn_rel_params)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_rel_ota.conn_rel_cause = conn_rel_params->conn_rel_cause ;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_rel_ota.redir_info_present = conn_rel_params->redir_info_present ;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_rel_ota.redir_rat = conn_rel_params->redir_rat ;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_ho_to_gsm_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_ho_to_gsm_params(sys_modem_as_id_e_type wrrc_as_id, rrc_log_rrc_ho_gsm_ota *ho_gsm_params)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ho_to_gsm.freq_band = ho_gsm_params->freq_band ;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ho_to_gsm.ho_msg_type = ho_gsm_params->ho_msg_type ;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_scri_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_scri_params(sys_modem_as_id_e_type wrrc_as_id, uint8 scri_cause, uint8 domain_id)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.sig_conn_rel_ind.cn_id = domain_id;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.sig_conn_rel_ind.scri_cause = scri_cause;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_scrr_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_scrr_params(sys_modem_as_id_e_type wrrc_as_id, uint8 domain_id)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.sig_conn_rel.cn_id = domain_id;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_idt_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_idt_params(sys_modem_as_id_e_type wrrc_as_id, uint8 est_cause, uint8 domain_id)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.idt_params.cn_id = domain_id;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.idt_params.est_cause = est_cause;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_conn_req_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_conn_req_params(sys_modem_as_id_e_type wrrc_as_id, rrc_log_rrc_conn_req_ota *conn_req_params)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_req_ota.est_cause = conn_req_params->est_cause;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_req_ota.as_rel_indicator = conn_req_params->as_rel_indicator;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_req_ota.cn_id = conn_req_params->cn_id;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_conn_setup_compl_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_conn_setup_compl_params(sys_modem_as_id_e_type wrrc_as_id, uint16 cont_length)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.conn_setup_compl_params.lte_container_length = cont_length;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_cco_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_cco_params(sys_modem_as_id_e_type wrrc_as_id, rrc_log_rrc_cco_ota *cco_params)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cco_params.BCCH_ARFCN.num = cco_params->BCCH_ARFCN.num;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cco_params.BCCH_ARFCN.band = cco_params->BCCH_ARFCN.band;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cco_params.BSIC_NCC = cco_params->BSIC_BCC;   
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cco_params.BSIC_BCC = cco_params->BSIC_NCC;   
  }
}


/*===========================================================================
FUNCTION rrc_qsh_log_cco_fail_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_cco_fail_params(sys_modem_as_id_e_type wrrc_as_id, uint32 fail_cause)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cco_fail_params.cco_fail_cause = fail_cause;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_mcm_fail_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_mcm_fail_params(sys_modem_as_id_e_type wrrc_as_id, uint32 fail_cause)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mcf_params.mcf_fail_cause = fail_cause;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_ho_from_utran_fail_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_ho_from_utran_fail_params(sys_modem_as_id_e_type wrrc_as_id, uint32 fail_cause)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ho_from_utran_fail_params.fail_cause = fail_cause;   
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_meas_rpt_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_meas_rpt_params(sys_modem_as_id_e_type wrrc_as_id, rrc_log_rrc_mrm_ota *mrm_params)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mrm_ota.meas_id = mrm_params->meas_id;   
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mrm_ota.meas_type = mrm_params->meas_type;   
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mrm_ota.meas_rpt_type = mrm_params->meas_rpt_type;   
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mrm_ota.event_id = mrm_params->event_id;               
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mrm_ota.num_cells_rpt = mrm_params->num_cells_rpt;                                    
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_meas_ctrl_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrc_qsh_log_meas_ctrl_params(sys_modem_as_id_e_type wrrc_as_id, rrc_log_rrc_mcm_ota *mcm_params)
{
  if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mcm_ota.meas_id = mcm_params->meas_id;   
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mcm_ota.meas_type = mcm_params->meas_type;   
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mcm_ota.meas_rpt_type = mcm_params->meas_rpt_type;   
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mcm_ota.meas_event_id_bmask = mcm_params->meas_event_id_bmask;                 
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_overall_ota_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

void rrc_qsh_log_overall_ota_params(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id)
{

  rrc_overall_ota_info_s* overall_ota_info_ptr = NULL;;
  if((proc_id == RRC_PROCEDURE_RBE) || (proc_id == RRC_PROCEDURE_RBR) || 
     (proc_id == RRC_PROCEDURE_RBRC) || (proc_id == RRC_PROCEDURE_PCR) ||
     (proc_id == RRC_PROCEDURE_TCR))
  {
    overall_ota_info_ptr = &((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.rb_recfg.overall_ota;
  }
  else if(proc_id == RRC_PROCEDURE_RCE)
  {
    overall_ota_info_ptr = &((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.con_setup.overall_ota;
  }
  else if(proc_id == RRC_PROCEDURE_CU)
  {
    overall_ota_info_ptr = &((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cuc_params.overall_ota;
  }
  else if(proc_id == RRC_PROCEDURE_IHO)
  {
    overall_ota_info_ptr = &((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ho_to_utran_params.overall_ota;
  }
  else if(proc_id == RRC_PROCEDURE_ASU)
  {
    overall_ota_info_ptr = &((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.asu_info.overall_ota;
  }

  if(overall_ota_info_ptr == NULL)
    return;

  /* init all params for this proc id */
  memset(overall_ota_info_ptr, 0, sizeof(rrc_overall_ota_info_s));
  	
  /* num_rb_add */
  rrcqsh_get_num_rbs_added(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  /* num_rb_rel */
  rrcqsh_get_num_rbs_deleted(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  /* num_rb_recfg */
  rrc_get_num_rbs_recfg(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  /*rrc_srns_relocation*/
  rrc_srns_relocation(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  /* rlc size type */
  rrc_get_rlc_size_type(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  /* one_sided_re_est */
  rrc_get_one_sided_re_est_flag(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  /*mapping */
  rrc_get_mapping_combo_rb(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  /*mapping */
  rrc_get_mapping_combo_srb(wrrc_as_id, overall_ota_info_ptr);
  
  rrc_get_oc_set_status(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  rrc_get_hho_type(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  rrc_get_dtx_drx_status(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  rrc_get_edch_transmission(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
#ifdef FEATURE_WCDMA_DC_HSUPA
  rrc_get_sec_edch_transmission(wrrc_as_id, proc_id, overall_ota_info_ptr);
#endif
  rrcqsh_get_hsdpa_status(wrrc_as_id, proc_id, overall_ota_info_ptr);
  
  rrcqsh_get_sec_hsdpa_status(wrrc_as_id, proc_id, overall_ota_info_ptr);

}

/*===========================================================================
FUNCTION rrc_qsh_log_ota_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

void rrc_qsh_log_ota_params(sys_modem_as_id_e_type wrrc_as_id, rrc_proc_e_type proc_id)
{

  if((proc_id == RRC_PROCEDURE_ASU) || (proc_id == RRC_PROCEDURE_MCMR))
  {
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr == NULL)
    return;
  }  
  else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr == NULL)
  {
    return;
  }

  if((proc_id == RRC_PROCEDURE_RBE) || (proc_id == RRC_PROCEDURE_RBR) || 
     (proc_id == RRC_PROCEDURE_RBRC) || (proc_id == RRC_PROCEDURE_PCR) ||
     (proc_id == RRC_PROCEDURE_TCR) || (proc_id == RRC_PROCEDURE_RCE) ||
     (proc_id == RRC_PROCEDURE_CU) || (proc_id == RRC_PROCEDURE_ASU) ||
     (proc_id == RRC_PROCEDURE_IHO))
  {
    if((ordered_config.set_status != OC_NOT_SET) && (ordered_config.set_by_proc == proc_id))
    {
      rrc_qsh_log_overall_ota_params(wrrc_as_id, proc_id);
    }
  }
  if(proc_id == RRC_PROCEDURE_CU)
  {
    rrcqsh_log_cuc_ota_params(wrrc_as_id);
  }
  if(proc_id == RRC_PROCEDURE_RCE)
  {  
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.con_setup.cap_requests = rrcdata_misc_info.rrc_capability_update_requirement.system_specific_cap_update_req_present; 
  }
  if(proc_id == RRC_PROCEDURE_SMC)
  {
    rrcqsh_log_smc_ota_params(wrrc_as_id);
  }
  if(proc_id == RRC_PROCEDURE_UMI)
  {
    rrcqsh_log_umi_ota_params(wrrc_as_id);
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_ota_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

void rrc_qsh_log_ul_ota(sys_modem_as_id_e_type wrrc_as_id)
{
  switch (uplink_ota.ul_ota_type)
  {
    case RRC_OTA_UL_FAILURE:
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->msg_id = uplink_ota.msg_id;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.failure_cause = uplink_ota.ul_ota.ul_ota_parms.failure_cause;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.ul_integrity_info_present = uplink_ota.ul_ota.ul_ota_parms.ul_integrity_info_present;		
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);
    }
    break;

    case RRC_OTA_CU:
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->msg_id = uplink_ota.msg_id;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params.cell_update_cause = uplink_ota.ul_ota.cu_params.cell_update_cause;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params.est_cause = uplink_ota.ul_ota.cu_params.est_cause;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params.rsi = uplink_ota.ul_ota.cu_params.rsi;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params.rue_on_drb = uplink_ota.ul_ota.cu_params.rue_on_drb;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params.rue_on_srb = uplink_ota.ul_ota.cu_params.rue_on_srb;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params.security_revert_status = uplink_ota.ul_ota.cu_params.security_revert_status;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params.t314_expired = uplink_ota.ul_ota.cu_params.t314_expired;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params.t315_expired = uplink_ota.ul_ota.cu_params.t315_expired;
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);		
    }
    break;

    case RRC_OTA_ASU:
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
    {
      ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->msg_id = uplink_ota.msg_id;
      ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.ul_asu_info.failure_cause= uplink_ota.ul_ota.ul_asu_info.failure_cause;
      ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.ul_asu_info.trans_id = uplink_ota.ul_ota.ul_asu_info.trans_id;
      ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.ul_asu_info.ul_integrity_info_present = uplink_ota.ul_ota.ul_asu_info.ul_integrity_info_present;
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_MEAS_ASU_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL); 
    }
    break;

    case RRC_OTA_UL_CPLT:
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->msg_id = uplink_ota.msg_id;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.count_c_activation = uplink_ota.ul_ota.ul_ota_parms.count_c_activation;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.ul_integrity_info_present = uplink_ota.ul_ota.ul_ota_parms.ul_integrity_info_present;
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.ul_counter_sync_info = uplink_ota.ul_ota.ul_ota_parms.ul_counter_sync_info;
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);	
    }
    break;

    case RRC_OTA_IHO:
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)  
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->msg_id = uplink_ota.msg_id;	
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ho_from_utran_fail_params.fail_cause = uplink_ota.ul_ota.ho_from_utran_fail_params.fail_cause;	 
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);				
    }
    break;

    case RRC_OTA_CHO:
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)  
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->msg_id = uplink_ota.msg_id;	
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cco_fail_params.cco_fail_cause = uplink_ota.ul_ota.cco_fail_params.cco_fail_cause;	 
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL);				
    }
    break;

    case RRC_OTA_MEAS:
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)	
    {
      ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->msg_id = uplink_ota.msg_id;	
      ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.mcf_params.mcf_fail_cause = uplink_ota.ul_ota.mcf_params.mcf_fail_cause; 	
      rrc_qsh_commit_data(wrrc_as_id, WRRC_QSH_METRIC_MEAS_ASU_OTA, QSH_CLIENT_METRIC_LOG_REASON_EVENT_INTERNAL); 		
    }
    break;

    default:
    break;
  }  
  memset(&uplink_ota, 0, sizeof(rrc_qsh_ul_ota_param_s));
}

/*===========================================================================
FUNCTION rrc_qsh_log_dl_ota_msg_id
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

void rrc_qsh_log_dl_ota_msg_id(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr, rrc_proc_e_type rrc_proc_name)
{
  uint8 message_choice=0;

  switch (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  {
    case rrc_DL_CCCH_Message_PDU:
      message_choice = rrc_get_dl_ccch_message_type(wrrc_as_id, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    break;
  
    case rrc_DL_DCCH_Message_PDU:
      message_choice = rrc_get_dl_dcch_message_type(wrrc_as_id, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
    break;

    case rrc_PCCH_Message_PDU:
      message_choice = T_rrc_PCCH_MessageType_pagingType1;
    break;

    case rrc_BCCH_FACH_Message_PDU:
      message_choice = T_rrc_BCCH_FACH_MessageType_systemInformationChangeIndication; 
    break;

    case rrc_BCCH_BCH_Message_PDU:
      message_choice = T_rrc_BCCH_FACH_MessageType_systemInformation;
    break;

    default:
    break;
  } 

  if((rrc_proc_name == RRC_PROCEDURE_ASU) || (rrc_proc_name == RRC_PROCEDURE_MCMR))
  {
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
    {
      ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->msg_id = rrc_qsh_map_ota_msg_id(wrrc_as_id, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type, message_choice);
    }
  }
  else 
  {
    if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
    {
      ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->msg_id = rrc_qsh_map_ota_msg_id(wrrc_as_id, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type, message_choice);
    }
  }
}

/*===========================================================================
FUNCTION rrc_qsh_log_dl_ota_msg_id
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

boolean rrc_qsh_prevent_dl_ota_param_log(sys_modem_as_id_e_type wrrc_as_id, rrc_cmd_type *cmd_ptr)
{
  uint8 sdu_type;
  uint8 message_choice;
  
  sdu_type = (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == rrc_DL_DCCH_Message_PDU)?RRC_DCCH_DL_SDU : RRC_CCCH_DL_SDU;

  if(sdu_type == RRC_CCCH_DL_SDU)
  {
    message_choice = rrc_get_dl_ccch_message_type(wrrc_as_id, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
  }
  else
  {
    message_choice = rrc_get_dl_dcch_message_type(wrrc_as_id, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
  }

  if(((sdu_type == RRC_DCCH_DL_SDU)&&(message_choice == T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM))||
    ((sdu_type == RRC_DCCH_DL_SDU) && (message_choice == T_rrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)) ||
    ((sdu_type == RRC_CCCH_DL_SDU) && (message_choice == T_rrc_DL_CCCH_MessageType_rrcConnectionReject)))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
/*===========================================================================
FUNCTION rrcqsh_log_rbreconfig_ul_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_rbreconfig_ul_complete_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerReconfigurationComplete,
  rrc_RadioBearerReconfigurationComplete,count_C_ActivationTime))
  {
    ul_ota_parms->count_c_activation = ptr->message.u.radioBearerReconfigurationComplete.count_C_ActivationTime;
  }
  if (RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerReconfigurationComplete,
  rrc_RadioBearerReconfigurationComplete,ul_CounterSynchronisationInfo))
  {	 
    ul_ota_parms->ul_counter_sync_info= 1;
  }
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present= 1;
  }
}
/*===========================================================================
FUNCTION rrcqsh_log_rbreconfig_ul_fail_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_rbreconfig_ul_fail_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present = 1;
  }
  ul_ota_parms->failure_cause = ptr->message.u.radioBearerReconfigurationFailure.failureCause.t;
}
/*===========================================================================
FUNCTION rrcqsh_log_rbr_ul_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_rbr_ul_complete_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if (RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerReleaseComplete,
  rrc_RadioBearerReleaseComplete,ul_CounterSynchronisationInfo))
  {	 
    ul_ota_parms->ul_counter_sync_info= 1;
  }
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present= 1;
  }
}
/*===========================================================================
FUNCTION rrcqsh_log_rbr_ul_fail_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_rbr_ul_fail_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  { 
    ul_ota_parms->ul_integrity_info_present = 1;
  }
  ul_ota_parms->failure_cause = ptr->message.u.radioBearerReleaseFailure.failureCause.t;
}
/*===========================================================================
FUNCTION rrcqsh_log_tcrecfg_ul_complete_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_tcrecfg_ul_complete_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.transportChannelReconfigurationComplete,
  rrc_TransportChannelReconfigurationComplete,count_C_ActivationTime))
  {
    ul_ota_parms->count_c_activation= ptr->message.u.transportChannelReconfigurationComplete.count_C_ActivationTime;
  }
  if (RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.transportChannelReconfigurationComplete,
  rrc_TransportChannelReconfigurationComplete,ul_CounterSynchronisationInfo))
  {  
    ul_ota_parms->ul_counter_sync_info= 1;
  }
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present= 1;
  }
}
/*===========================================================================
FUNCTION rrcqsh_log_tcrecfg_ul_fail_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_tcrecfg_ul_fail_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present = 1;
  }
  ul_ota_parms->failure_cause = ptr->message.u.transportChannelReconfigurationFailure.failureCause.t;
}
/*===========================================================================
FUNCTION rrcqsh_log_pcrecfg_ul_complete_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_pcrecfg_ul_complete_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.physicalChannelReconfigurationComplete,
  rrc_PhysicalChannelReconfigurationComplete,count_C_ActivationTime))
  {
    ul_ota_parms->count_c_activation= ptr->message.u.physicalChannelReconfigurationComplete.count_C_ActivationTime;
  }
  if (RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.physicalChannelReconfigurationComplete,
  rrc_PhysicalChannelReconfigurationComplete,ul_CounterSynchronisationInfo))
  {  
    ul_ota_parms->ul_counter_sync_info= 1;
  }
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present= 1;
  }
}
/*===========================================================================
FUNCTION rrcqsh_log_pcrecfg_ul_fail_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_pcrecfg_ul_fail_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present = 1;
  }
  ul_ota_parms->failure_cause = ptr->message.u.physicalChannelReconfigurationFailure.failureCause.t;
}
/*===========================================================================
FUNCTION rrcqsh_log_rbe_ul_complete_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_rbe_ul_complete_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerSetupComplete,
  rrc_RadioBearerSetupComplete,count_C_ActivationTime))
  {
    ul_ota_parms->count_c_activation= ptr->message.u.radioBearerSetupComplete.count_C_ActivationTime;
  }

  if (RRC_MSG_COMMON_BITMASK_IE(ptr->message.u.radioBearerSetupComplete,
  rrc_RadioBearerSetupComplete,ul_CounterSynchronisationInfo))
  {    
    ul_ota_parms->ul_counter_sync_info= 1;
  }

  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present= 1;
  }
}
/*===========================================================================
FUNCTION rrcqsh_log_rbe_ul_fail_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_rbe_ul_fail_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms,  rrc_UL_DCCH_Message *ptr)
{
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present = 1;
  }
  ul_ota_parms->failure_cause = ptr->message.u.radioBearerSetupFailure.failureCause.t;
}
/*===========================================================================
FUNCTION rrcqsh_log_cell_update_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_cell_update_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_cu_ota_parms_s *cu_ota_parms,  rrc_UL_CCCH_Message *msg_ptr)
{
  if(msg_ptr->message.u.cellUpdate.m.failureCausePresent)
  {
    cu_ota_parms->failure_cause = msg_ptr->message.u.cellUpdate.failureCause.failureCause.t;		
  }
  cu_ota_parms->cell_update_cause =msg_ptr->message.u.cellUpdate.cellUpdateCause;
  cu_ota_parms->rue_on_srb= msg_ptr->message.u.cellUpdate.am_RLC_ErrorIndicationRb2_3or4;
  cu_ota_parms->rue_on_drb= msg_ptr->message.u.cellUpdate.am_RLC_ErrorIndicationRb5orAbove;
  cu_ota_parms->t314_expired = msg_ptr->message.u.cellUpdate.rb_timer_indicator.t314_expired;
  cu_ota_parms->t315_expired = msg_ptr->message.u.cellUpdate.rb_timer_indicator.t315_expired;
  cu_ota_parms->est_cause= rrc_qsh_get_establishment_cause(wrrc_as_id);
  cu_ota_parms->rsi= rrc_qsh_get_reconfig_status_ind(wrrc_as_id);
  cu_ota_parms->security_revert_status= rrc_qsh_get_security_revert_status(wrrc_as_id);
}
/*===========================================================================
FUNCTION rrcqsh_log_smc_complete_ota
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None
===========================================================================*/

void rrcqsh_log_smc_complete_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_ota_parms_s *ul_ota_parms)
{
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_ota_parms->ul_integrity_info_present = 1;
  }
}
/*===========================================================================
FUNCTION rrcqsh_log_asu_ul_complete_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_asu_ul_complete_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_asu_ota_parms_s *ul_asu_info,  rrc_UL_DCCH_Message *ptr)

{
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_asu_info->ul_integrity_info_present= 1;
  }
  ul_asu_info->trans_id= ptr->message.u.activeSetUpdateComplete.rrc_TransactionIdentifier;
}

/*===========================================================================
FUNCTION rrcqsh_log_asu_ul_fail_ota
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/
void rrcqsh_log_asu_ul_fail_ota(sys_modem_as_id_e_type wrrc_as_id, rrc_ul_asu_ota_parms_s *ul_asu_info,  rrc_UL_DCCH_Message *ptr)
{
  if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
  {
    ul_asu_info->ul_integrity_info_present = 1;
  }
  ul_asu_info->failure_cause =  ptr->message.u.activeSetUpdateFailure.failureCause.t;
  ul_asu_info->trans_id = ptr->message.u.activeSetUpdateFailure.rrc_TransactionIdentifier;
}


/*===========================================================================
FUNCTION rrcqsh_log_uplink_dcch_ota_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

void rrcqsh_log_uplink_dcch_ota_params(sys_modem_as_id_e_type wrrc_as_id, wrrc_qsh_ota_msg_id msg_id, rrc_UL_DCCH_Message *ptr)
{
  /* Initalize the parameters */

  if( (rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
      &&
      ((msg_id == WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATECOMPLETE)
        ||
       (msg_id == WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATEFAILURE)))
  {
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.ul_asu_info.failure_cause = 0;
    ((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.ul_asu_info.ul_integrity_info_present = 0;
  }
  else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
  {
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.count_c_activation = 0;
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.failure_cause = 0;
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.ul_integrity_info_present = 0;
    ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.ul_counter_sync_info = 0;
  }
  
  switch (msg_id)
  {
    case WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERSETUPFAILURE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_FAILURE;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERSETUPFAILURE;
        rrcqsh_log_rbe_ul_fail_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        rrcqsh_log_rbe_ul_fail_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
      }
      break;	

     case WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERSETUPCOMPLETE:
       if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
       {
         rrcqsh_log_rbe_ul_complete_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
       }
       break;

     case WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATIONFAILURE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_FAILURE;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATIONFAILURE;
        rrcqsh_log_rbreconfig_ul_fail_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        rrcqsh_log_rbreconfig_ul_fail_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
      }
      break;

    case WRRC_UL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATIONCOMPLETE:
    if(rrc_qsh_dl_committed == FALSE)
    {
      uplink_ota.ul_ota_type = RRC_OTA_UL_CPLT;
      uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATIONCOMPLETE;
      rrcqsh_log_pcrecfg_ul_complete_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
    }
    else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
    {
      rrcqsh_log_pcrecfg_ul_complete_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
    }
    break;

    case WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATIONCOMPLETE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_CPLT;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRECONFIGURATIONCOMPLETE;
        rrcqsh_log_rbreconfig_ul_complete_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {    
        rrcqsh_log_rbreconfig_ul_complete_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
      }
      break;

    case WRRC_UL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATIONFAILURE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_FAILURE;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_PHYSICALCHANNELRECONFIGURATIONFAILURE;
        rrcqsh_log_pcrecfg_ul_fail_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        rrcqsh_log_pcrecfg_ul_fail_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
      }
      break;

    case WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRELEASEFAILURE:					
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_FAILURE;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRELEASEFAILURE;
        rrcqsh_log_rbr_ul_fail_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {   
        rrcqsh_log_rbr_ul_fail_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
      }
      break;

    case WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRELEASECOMPLETE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_CPLT;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_RADIOBEARERRELEASECOMPLETE;
        rrcqsh_log_rbr_ul_complete_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        rrcqsh_log_rbr_ul_complete_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
      }
      break;

    case WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATIONFAILURE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_FAILURE;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATIONFAILURE;
        rrcqsh_log_tcrecfg_ul_fail_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        rrcqsh_log_tcrecfg_ul_fail_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
      }
      break;

     case WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATIONCOMPLETE:
       if(rrc_qsh_dl_committed == FALSE)
       {
         uplink_ota.ul_ota_type = RRC_OTA_UL_CPLT;
         uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_TRANSPORTCHANNELRECONFIGURATIONCOMPLETE;
         rrcqsh_log_tcrecfg_ul_complete_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms), ptr);
       }
       else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
       {
         rrcqsh_log_tcrecfg_ul_complete_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms), ptr);
       }
       break;

     case WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATECOMPLETE:
       if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
       {
         rrcqsh_log_asu_ul_complete_ota(wrrc_as_id, &(((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.ul_asu_info), ptr);
       }
       break;

     case WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATEFAILURE:
       if(rrc_qsh_dl_committed == FALSE)
       {
         uplink_ota.ul_ota_type = RRC_OTA_ASU;
         uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_ACTIVESETUPDATEFAILURE;
         rrcqsh_log_asu_ul_fail_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_asu_info), ptr);
       }
       else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr != NULL)
       {
         rrcqsh_log_asu_ul_fail_ota(wrrc_as_id, &(((wrrc_qsh_metric_meas_asu_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_MEAS_ASU_OTA].data_ptr)->asu_meas_info.ul_asu_info), ptr);
       }
       break;

    case WRRC_UL_DCCH_MESSAGETYPE_HANDOVERTOUTRANCOMPLETE:
      if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        if(ptr->message.u.handoverToUTRANComplete.m.count_C_ActivationTimePresent)
        {
          ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.count_c_activation= ptr->message.u.handoverToUTRANComplete.count_C_ActivationTime;
        }
      }
      break;

    case WRRC_UL_DCCH_MESSAGETYPE_RRCCONNECTIONRELEASECOMPLETE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_CPLT;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_RRCCONNECTIONRELEASECOMPLETE;
        if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
        {
          uplink_ota.ul_ota.ul_ota_parms.ul_integrity_info_present = 1;
        }
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
        {
          ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.ul_integrity_info_present= 1;
        }
      }
      break;

    case WRRC_UL_DCCH_MESSAGETYPE_SECURITYMODECOMPLETE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_CPLT;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_SECURITYMODECOMPLETE;
        rrcqsh_log_smc_complete_ota(wrrc_as_id, &(uplink_ota.ul_ota.ul_ota_parms));
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        rrcqsh_log_smc_complete_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms));
      }
      break;

    case WRRC_UL_DCCH_MESSAGETYPE_UTRANMOBILITYINFORMATIONFAILURE:
      /* PHY channel failure means already a DL OTA is logged */
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_UL_FAILURE;
        uplink_ota.msg_id = WRRC_UL_DCCH_MESSAGETYPE_UTRANMOBILITYINFORMATIONFAILURE;
        if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
        {
          uplink_ota.ul_ota.ul_ota_parms.ul_integrity_info_present = 1;
        }
        uplink_ota.ul_ota.ul_ota_parms.failure_cause = ptr->message.u.utranMobilityInformationFailure.failureCause.t;
        /* Commit data should happen after dl SDU is committed */
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        if(RRCSMC_IP_STARTED == rrcsmc_check_integrity_status(wrrc_as_id))
        {
          ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.ul_integrity_info_present= 1;
        }
        ((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.ul_ota_parms.failure_cause= ptr->message.u.utranMobilityInformationFailure.failureCause.t; 	
      }
      break;
    default:
      break;
  }
}


/*===========================================================================
FUNCTION rrcqsh_log_uplink_ccch_ota_params
  
DESCRIPTION
DEPENDENCIES
  None
RETURN VALUE
  None
SIDE EFFECTS
  None

===========================================================================*/

void rrcqsh_log_uplink_ccch_ota_params(sys_modem_as_id_e_type wrrc_as_id, wrrc_qsh_ota_msg_id msg_id, rrc_UL_CCCH_Message *msg_ptr)
{
  
  switch (msg_id)
  {
    case WRRC_UL_CCCH_MESSAGETYPE_CELLUPDATE:
      if(rrc_qsh_dl_committed == FALSE)
      {
        uplink_ota.ul_ota_type = RRC_OTA_CU;
        uplink_ota.msg_id = WRRC_UL_CCCH_MESSAGETYPE_CELLUPDATE;
        rrcqsh_log_cell_update_ota(wrrc_as_id, &(uplink_ota.ul_ota.cu_params), msg_ptr);
      }
      else if(rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr != NULL)
      {
        rrcqsh_log_cell_update_ota(wrrc_as_id, &(((wrrc_qsh_metric_ota_s*)rrc_qsh_metric_db[WRRC_QSH_METRIC_OTA].data_ptr)->ota_info.cu_params), msg_ptr);
      }
      break;
    default:
      break;

  }
}
#endif
/*===========================================================================
FUNCTION rrc_ext_get_wcdma_hsupa_category
  
DESCRIPTION This function is used to get HSUPA category supported by UE

NAS shall call this API to know the UE HSUPA category based on which NAS encode/decode 
the additional IEs of QoS parameters in NAS OTA for Packet Switched calls.
NAS OTA can be PDP Activation Request/ PDP Activation Accept/PDP Modification Request 

DEPENDENCIES
  None
RETURN VALUE
  uint8
SIDE EFFECTS
  None

===========================================================================*/
uint8 rrc_ext_get_wcdma_hsupa_category(sys_modem_as_id_e_type wrrc_as_id)
{
#ifdef FEATURE_WCDMA_DC_HSUPA
  if(TRUE == rrcueci_nv_supports_dc_hsupa(wrrc_as_id))
  {
    return (uint8)rrcnv_data.wcdma_nv_hsupa_category;
  }
  else
#endif
  {
    return MIN(rrcnv_data.wcdma_nv_hsupa_category,RRC_WCMDA_HSUPA_DEFAULT_CATEGORY);
  }
}
/*===========================================================================

FUNCTION rrc_pdcp_report_rb_rates

DESCRIPTION

  This function reports Radio bearer UL/DL data rates to DS layer

  
DEPENDENCIES


RETURN VALUE
None
 

SIDE EFFECTS

  None

===========================================================================*/

void rrc_pdcp_report_rb_rates(sys_modem_as_id_e_type wrrc_as_id,boolean meas_rpt_case)
{
  uint8 rab_index=0,rb_idx = 0;
  rrc_state_e_type rrc_state;
  rrcllc_oc_set_status_e_type oc_status = rrcllc_get_ordered_config_status_wo_f3(wrrc_as_id);
  pdcp_rrc_rb_data_rate_info_type pdcp_rb_rate = {0};

  switch (oc_status) 
  {
    case OC_SET_FOR_CELL_FACH:
    case OC_SET_FOR_DCH_FACH_TRANS:
      rrc_state = RRC_STATE_CELL_FACH;
      break;
    case OC_SET_FOR_CELL_DCH:
      rrc_state = RRC_STATE_CELL_DCH;
      break;
    case OC_SET_FOR_URA_PCH:
    case OC_SET_FOR_DCH_URA_PCH_TRANS:
    case OC_SET_FOR_FACH_URA_PCH_TRANS:
      rrc_state = RRC_STATE_URA_PCH;
      break;
    case OC_SET_FOR_CELL_PCH:
    case OC_SET_FOR_DCH_CELL_PCH_TRANS:
    case OC_SET_FOR_FACH_CELL_PCH_TRANS:
      rrc_state = RRC_STATE_CELL_PCH;
      break;
    default:
      rrc_state = rrc_get_state(wrrc_as_id);
      break;
  }
  /* For meas reports change, state_change is not yet 
     processed, so use the state as cell_FACH
  */
  if(meas_rpt_case == TRUE)
  {
    rrc_state = RRC_STATE_CELL_FACH;
  }
  if((rrc_state == RRC_STATE_CELL_PCH) || 
       (rrc_state == RRC_STATE_URA_PCH)
     )
  {
    WRRC_MSG0_HIGH("PDCP_RB_RATES:Do not report rates as state is cell_PCH");
    return;
  }

  pdcp_rb_rate.num_rbs = 0;
  
  for (rab_index = 0; ((rab_index < MAX_RAB_TO_SETUP) && (pdcp_rb_rate.num_rbs < MAX_RAB_TO_SETUP)); rab_index++)
  {
    if ((rrc_est_rabs.rabs[rab_index].cn_domain == RRC_PS_DOMAIN_CN_ID) &&
        (rrc_est_rabs.rabs[rab_index].num_rbs_for_rab > 0) &&
        (rrc_est_rabs.rabs[rab_index].rab_id !=  RRC_INVALID_RAB_ID))
    {
      for(rb_idx = 0; ((rb_idx< rrc_est_rabs.rabs[rab_index].num_rbs_for_rab) && (rb_idx < MAX_RB_PER_RAB)); rb_idx++)
      {
        /* Fill in the Command Data */

        if(pdcp_rb_rate.num_rbs < MAX_RAB_TO_SETUP)
        {
          
          pdcp_rb_rate.num_rbs++;
          
          pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].rb_id = 
             rrc_est_rabs.rabs[rab_index].rb_for_rab[rb_idx].rb_id;
          pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].rlc_dl_id = 
             rrclcm_get_dl_rlc_lc_id(wrrc_as_id,rrc_est_rabs.rabs[rab_index].rb_for_rab[rb_idx].rb_id);
          pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].rlc_ul_id = 
             rrclcm_get_ul_rlc_lc_id(wrrc_as_id,rrc_est_rabs.rabs[rab_index].rb_for_rab[rb_idx].rb_id);
          pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].rb_dl_rate = 
             rrcllc_get_dl_ded_rate(wrrc_as_id,rrc_state, rrc_est_rabs.rabs[rab_index].rb_for_rab[rb_idx].rb_id);
          pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].rb_ul_rate = 
             rrcllc_get_ul_ded_rate(wrrc_as_id,rrc_state, rrc_est_rabs.rabs[rab_index].rb_for_rab[rb_idx].rb_id);
#ifdef FEATURE_WCDMA_UL_COMPR
          /* This field is for future use only. 
           * For now just filling NV value but in future it may require the change 
           * to reflect instantaneous compression status. RRC can not know this on its own.
           * Also change may then required to use this interface also for compression status change
          */
          pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].ul_dacc_status = 
             rrcdt_nv_supports_ul_comp(wrrc_as_id); 
#else
          pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].ul_dacc_status = 
             FALSE;
#endif

          WRRC_MSG4_HIGH("PDCP_RB_RATES:RB ID %d DL rate %d UL rate %d state %d",pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].rb_id,
            pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].rb_dl_rate,pdcp_rb_rate.rb_data_rate_info[pdcp_rb_rate.num_rbs - 1].rb_ul_rate,rrc_state);

        }
        else
        {
           break;
        }
      }
    }
  }

  if(pdcp_rb_rate.num_rbs > 0)
  {
    comp_cmd_type  *pdcp_ptr;
    /* Get PDCP command buffer */
    if ((pdcp_ptr = comp_get_cmd_buf()) == NULL)
    {
      WRRC_MSG0_ERROR("NULL Ptr when sending CPDCP_CONFIG_REQ ");
      return;
    }
    /* Fill in the Command Header */
    pdcp_ptr->hdr.cmd_id = PDCP_RRC_DATA_RATE_IND;
#ifdef FEATURE_DUAL_WCDMA
    pdcp_ptr->hdr.subs_id = wrrc_as_id;
#endif
    pdcp_ptr->cmd.pdcp_rrc_data_rate_info = pdcp_rb_rate; 
    /* Send command to PDCP */
    comp_put_cmd(pdcp_ptr);
  }
  
}
/*===========================================================================
 FUNCTION     rrc_crash_debug_set_rrc_to_tdsrrc_cmd_id

 DESCRIPTION
   This functions sets the command sent from TDSRRC to RRC into the
   crash debug info

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_crash_debug_set_rrc_to_tdsrrc_cmd_id(sys_modem_as_id_e_type wrrc_as_id, wrrc_tdsrrc_rrc_cmd_e_type cmd_id)
{
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.rrc_to_tdsrrc_debug_index<RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.rrc_to_tdsrrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.rrc_to_tdsrrc_debug_index].cmd_id = cmd_id;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.rrc_to_tdsrrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.rrc_to_tdsrrc_debug_index].timeticks = (int64)timetick_get();
    }
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.rrc_to_tdsrrc_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.rrc_to_tdsrrc_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = INVALID_CMD;
  }
}
/*===========================================================================
 FUNCTION     rrc_crash_debug_set_tdsrrc_to_rrc_cmd_id

 DESCRIPTION
   This functions sets the command sent from RRC to TDSRRC into the
   crash debug info

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_crash_debug_set_tdsrrc_to_rrc_cmd_id(sys_modem_as_id_e_type wrrc_as_id, rrc_tdsrrc_cmd_e_type cmd_id)
{
  if(!(rrcnv_data.rrc_disable_crash_debug_info_nv & NV_CRASH_DEBUG_INFO_DISABLE))
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_index < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT)
    {
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_index].cmd_id = cmd_id;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_index].timeticks = (int64)timetick_get();
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_index].proc_id = (uint8)RRC_PROCEDURE_NONE;
      rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_cmds[rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_index].substate = RRC_INVALID_PROC_SUBSTATE;
    }
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_index = (rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_tdsrrc_cmds.tdsrrc_to_rrc_debug_index + 1) % RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;
    rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_cmd_dest = TDSRRC_TO_RRC_CMD;
  }
}
/*===========================================================================
 FUNCTION     rrc_log_nv_status

 DESCRIPTION
   This functions logs the status of all connected mode NVs

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_log_nv_status(sys_modem_as_id_e_type wrrc_as_id)
{
#ifdef FEATURE_WCDMA_HS_RACH
 WRRC_MSG9_HIGH("rrc_nv_rel_indicator: %d, concurrency nv: 0x%x, rrc_feature_nv_item_value:0x%x, device_mode: %d[0-SS, 1-DSDS, 2- DSDA, 3-TSTS], HSUPA categ: %d, HSDPA categ: %d, CPC disable: 0x%x, Cell supports HS-FACH: %d, HS-RACH: %d", 
                  rrc_nv_rel_indicator, rrc_get_concurrency_nv(wrrc_as_id), rrcnv_data.rrc_feature_nv_item_value, 
                  rrc_dsds_int_data.wcdma_device_mode, rrcnv_data.wcdma_nv_hsupa_category, rrcnv_data.nv_hsdpa_category, rrcnv_data.rrc_disable_cpc_fdpch_nv,rrcenhstate_camped_cell_supports_e_fach(wrrc_as_id), rrchsrach_camped_cell_supports_hsrach(wrrc_as_id));
#else
  WRRC_MSG8_HIGH("rrc_nv_rel_indicator: %d, concurrency nv: 0x%x, rrc_feature_nv_item_value:0x%x, device_mode: %d[0-SS, 1-DSDS, 2- DSDA, 3-TSTS], HSUPA categ: %d, HSDPA categ: %d, CPC disable: 0x%x, Cell supports HS-FACH: %d", 
              rrc_nv_rel_indicator, rrc_get_concurrency_nv(wrrc_as_id), rrcnv_data.rrc_feature_nv_item_value, 
              rrc_dsds_int_data.wcdma_device_mode, rrcnv_data.wcdma_nv_hsupa_category, rrcnv_data.nv_hsdpa_category, rrcnv_data.rrc_disable_cpc_fdpch_nv,rrcenhstate_camped_cell_supports_e_fach(wrrc_as_id));
#endif
#ifdef FEATURE_WCDMA_W2L_PSEUDO_MEAS
  WRRC_MSG1_HIGH("Pseudo meas feature is  %d", rrcnv_data.rrc_pseudo_meas_info.feature_enabled);
  WRRC_MSG1_HIGH("Rxagc trigger %d", rrcnv_data.rrc_pseudo_meas_triggers_info.triggers_for_pseudo_meas);
  WRRC_MSG1_HIGH("RSRP is %d", rrcnv_data.rrc_pseudo_meas_rsrp_rsrq_info.rsrp);
  WRRC_MSG1_HIGH("RRSRQ is %d", rrcnv_data.rrc_pseudo_meas_rsrp_rsrq_info.rsrq);
#endif
}

/*===========================================================================
 FUNCTION     rrc_log_feature_state_variables_status

 DESCRIPTION
   This functions logs the status of all connected mode feature configuration status

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_log_feature_state_variables_status(sys_modem_as_id_e_type wrrc_as_id, ordered_config_type* config_ptr)
{

  WRRC_MSG7_HIGH("config_ptr_in_use: %d, HSDPA ACTION: %d, HSDPA_STATUS: %d, SEC_HSDPA_CARRIERS: %d, EDCH: %d, CPC_DTX_DRX: %d, CPC_DRX: %d", 
  config_ptr_in_use, rrcdata_hs_info.hsdpa_action, rrcdata_hs_info.hsdpa_status, rrcllcpcie_return_num_sec_hsdpa_carriers(wrrc_as_id, config_ptr),
  config_ptr->e_dch_transmission,config_ptr->cpc_dtx_drx_status, config_ptr->cpc_drx_status);
  
#ifdef FEATURE_WCDMA_HS_RACH
  WRRC_MSG5_HIGH("EFACH_STATUS: %d, EFACH_ACTION: %d, HSRACH ACTION: %d, common_edch_tx: %d, hspa_rnti_stored_cell_pch: %d", 
  config_ptr->hs_status_in_e_fach, config_ptr->hs_action_in_e_fach,
  config_ptr->mac_hs_rach_action, config_ptr->common_edch_transmission, config_ptr->hspa_rnti_stored_cell_pch);
#else
  WRRC_MSG2_HIGH("EFACH_STATUS: %d, EFACH_ACTION: %d", 
  config_ptr->hs_status_in_e_fach, config_ptr->hs_action_in_e_fach);
#endif

#ifdef FEATURE_WCDMA_DC_HSUPA
WRRC_MSG1_HIGH("SEC_EDCH: %d",config_ptr->sec_e_dch_transmission);
#endif
}
/*===========================================================================
 FUNCTION     rrc_log_rb_mapping_info

 DESCRIPTION
   This functions logs all the rbs established and its lch, trch mapping

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/
void rrc_log_rb_mapping_info(sys_modem_as_id_e_type wrrc_as_id, ordered_config_type* config_ptr, boolean save_in_crash_debug)
{

  uint8 cnt, idx, combo_idx;

  /* UL DCH */
  for(idx = 0; idx < UE_MAX_TRCH; idx++)
  {
    if((config_ptr->ul_dch_trch_info[idx].dch_present) &&
    (config_ptr->ul_dch_trch_idx_in_use[idx] == TRUE))
    {
      for (cnt = 0; cnt < config_ptr->ul_cctrch.dch_info.dch_info[idx].ndlchan; cnt++) 
      {
        WRRC_MSG3_MED("UL DCH trch_id: %d, rb_id: %d, mac_id:%d ",
        config_ptr->ul_cctrch.dch_info.dch_info[idx].trch_id,config_ptr->ul_cctrch.dch_info.dch_info[idx].dlchan_info[cnt].rb_id,
        config_ptr->ul_cctrch.dch_info.dch_info[idx].dlchan_info[cnt].mac_id);
      } 
    }
  }
  
  /* EDCH */
  for(idx=0; idx<config_ptr->mac_e_config.num_e_mac_d_flow; idx++)
  {
    for (cnt=0;cnt < ordered_config_ptr->mac_e_config.e_mac_d_flow_info[idx].num_logch; cnt++)
    {
      WRRC_MSG3_MED("FLOW ID: %d mapped to lch id: %d, RB ID: %d",
      config_ptr->mac_e_config.e_mac_d_flow_info[idx].e_mac_d_flow_id, 
      config_ptr->mac_e_config.e_mac_d_flow_info[idx].e_ul_logch_info[cnt].lc_id,
      config_ptr->mac_e_config.e_mac_d_flow_info[idx].e_ul_logch_info[cnt].rb_id);		 
    }
  }


  /* DL DCH */

  for(idx = 0; idx < UE_MAX_TRCH; idx++)
  {
    if((config_ptr->dl_dch_trch_info[idx].dch_present) &&
    (config_ptr->dl_dch_trch_idx_in_use[idx] == TRUE))
    {
      for (cnt = 0; cnt < config_ptr->dl_dch_info[idx].ndlchan; cnt++) 
      {
        WRRC_MSG3_MED("DL DCH trch_id: %d, rb_id: %d, mac_id:%d ",
        config_ptr->dl_dch_info[idx].trch_id,config_ptr->dl_dch_info[idx].dlchan_info[cnt].rb_id,
        config_ptr->dl_dch_info[idx].dlchan_info[cnt].mac_id);
      } 
    }
  }
	
  /* EHS */
  for (cnt =0; cnt <config_ptr->mac_dl_parms.ndlchan_macehs; cnt++)
  {
    if ((config_ptr->mac_ehs_lc_id_in_use[cnt] == TRUE) &&
    (config_ptr->dl_macehs_logchan_info[cnt].mac_id != INVALID_LOGICAL_CHANNEL_ID))
    {
      WRRC_MSG3_MED("EHS Queue id: %d, mac_id: %d, rb_id: %d",
      config_ptr->dl_macehs_logchan_info[cnt].queue_id, 
      config_ptr->dl_macehs_logchan_info[cnt].mac_id,
      config_ptr->dl_macehs_logchan_info[cnt].rb_id);
    }
  }
/* EFACH */
  
  for (cnt =0; cnt <config_ptr->mac_dl_parms.efach_ndlchans; cnt++)
  {
    if ((config_ptr->efach_dl_lc_id_in_use[cnt] == TRUE) &&
    (config_ptr->efach_dl_logchan_info[cnt].mac_id != INVALID_LOGICAL_CHANNEL_ID))
    {
      WRRC_MSG3_MED("EFACH EHS Queue id: %d, mac_id: %d, rb_id: %d",
      config_ptr->efach_dl_logchan_info[cnt].queue_id, 
      config_ptr->efach_dl_logchan_info[cnt].mac_id,
      config_ptr->efach_dl_logchan_info[cnt].rb_id);
    }
  }

/* Standalone Hanging mapping Mapping*/
  rrc_log_standalone_hanging_rb_info(wrrc_as_id, save_in_crash_debug);
/* Hanging mapping Mapping*/
  rrc_log_hanging_rb_info(wrrc_as_id, save_in_crash_debug);

}

/*===========================================================================
 FUNCTION     rrc_log_rlc_am_info

 DESCRIPTION
   This functions logs important AM info

 DEPENDENCIES
   None.

 PARAMETERS
   None.

 RETURN VALUE
   None.

 SIDE EFFECTS
   None

===========================================================================*/

void rrc_log_rlc_am_info(sys_modem_as_id_e_type wrrc_as_id)
{

uint8 idx = 0;
uint8 cc_idx = 0;
uint32 old_size = 0xFFFFFFFF;
  /* AM info */
  for(idx=0; idx<ordered_config_ptr->rlc_am_parms.nentity; idx++)
  {
    cc_idx = get_am_index_in_config_ptr(wrrc_as_id, ordered_config_ptr->rlc_am_parms.am_info[idx]->common_config.rb_id, current_config_ptr);
    if(cc_idx != UE_MAX_AM_ENTITY)
    {
      old_size = current_config_ptr->am_config[cc_idx].rlc_dl_am_config.dl_rlc_size;
    }
    WRRC_MSG6_HIGH("RLC AM info entity_idx: %d, RB id: %d, re_est_entity: %d, PDU type: %d, DL rlc size: %d,old_size: %d",
    idx, ordered_config_ptr->rlc_am_parms.am_info[idx]->common_config.rb_id, ordered_config_ptr->am_config[idx].reestab_entity, 
    ordered_config_ptr->rb_am_rlc_size_ota[idx].rrc_pdu_type, ordered_config_ptr->am_config[idx].rlc_dl_am_config.dl_rlc_size,old_size);
  }
}
#ifdef FEATURE_QSH_MDUMP
/*===========================================================================
FUNCTION wrrc_qsh_mdump_collect_rrcdata_globals

DESCRIPTION
This function dumps the Data globals to QSH after a crash

DEPENDENCIES
  None


RETURN VALUE
None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR
void wrrc_qsh_mdump_collect_rrcdata_globals(sys_modem_as_id_e_type wrrc_as_id)
{
  uint8 idx = 0;
  qsh_mdump_collect_high(&rrcdata_timers_counters_info,sizeof(rrcdata_timers_counters_info));
  qsh_mdump_collect_high(&rrcdata_hs_info,sizeof(rrcdata_hs_info));
  qsh_mdump_collect_high(&rrcdata_codec_info,sizeof(rrcdata_codec_info));
  qsh_mdump_collect_high(&rrc_est_rabs,sizeof(rrc_est_rabs));
  qsh_mdump_collect_high(&rrcdata_misc_info,sizeof(rrcdata_misc_info));
  qsh_mdump_collect_high(&nv_fake_security_enabled,sizeof(nv_fake_security_enabled));

/* 
   Complete heap is available as of now with mini dump. 
   So skip dumping heap content for now 
*/
#if 0
//Save the heap pointers for crash debug ota messages
  for(idx=0; idx < RRC_CRASH_DEBUG_EXTERNAL_CMD_CNT;idx++)
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[idx].rrc_crash_debug_otas != NULL)
    {
      qsh_mdump_collect_high(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[idx].rrc_crash_debug_otas,
                             rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_ota.ota_message[idx].rrc_ota_length);
    }
  }
  for(idx=0; idx < RRC_CRASH_DEBUG_LOG_PKT_CNT;idx++)
  {
    if(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_log_pkt.log[idx].log_data_ptr != NULL)
    {
      qsh_mdump_collect_high(rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_log_pkt.log[idx].log_data_ptr,
                             rrcdata_logging_info.rrc_crash_debug_info.rrc_crash_debug_log_pkt.log[idx].length);
    }
  }
#endif
}
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif
#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif
#ifdef FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME
/*===========================================================================
   FUNCTION     rrcdata_buffer_ota_during_activation_time
  
   DESCRIPTION
       This function is used to buffer the ota messages received during activation time
       It backup OTA message in ecoded format. Heap memory is allcoated for OTA message.
   DEPENDENCIES
     None.
  
   PARAMETERS
     rrc_cmd_type
  
   RETURN VALUE
     void 
  
   SIDE EFFECTS
  
  ===========================================================================*/
void rrcdata_buffer_ota_during_activation_time(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr,                       /* Pointer to RRC command */
  rrc_proc_e_type procedure

)
{
  if(TRUE == rrc_buffer_ota_info.rrc_buffer_ota_message)
  {
    if(NULL == rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.dl_sdu)
    {
      WRRC_MSG1_HIGH("BUF_OTA:Backup encoded message for procedure %d", procedure);
      /*Buffered OTA type is of rrc_cmd_int_type instead of rrc_cmd_type, as get int cmd buf allocates memory of type rrc_cmd_int_type and this is also same as dl sdu ind allocated memroy*/
      WCDMA_MEMCPY(&rrc_buffer_ota_info.rrc_buffered_ota,sizeof(rrc_buffer_ota_info.rrc_buffered_ota),cmd_ptr,sizeof(rrc_cmd_int_type));
      rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.decoded_msg = NULL;
      rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.dl_sdu = (uint8 *) rrc_malloc(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      WCDMA_MEMCPY(rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.dl_sdu,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,
                  cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      rrc_buffer_ota_info.procedure = procedure;
    }
    else
    {
      WRRC_MSG2_ERROR("BUF_OTA: OTA message %d received when procedure %d is already backedup ",procedure, rrc_buffer_ota_info.procedure);
    }
  }
}

/*===========================================================================

FUNCTION rrcdata_process_buffered_ota_message

DESCRIPTION

  This function process the backedup message and corresponding procedure entry function is called.
  It also takes care of creating DL SDU message as every procedures expected the same.

  
DEPENDENCIES


RETURN VALUE
None
 

SIDE EFFECTS

  None

===========================================================================*/

void rrcdata_process_buffered_ota_message(
  sys_modem_as_id_e_type wrrc_as_id,
  rrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
   rrc_cmd_type  *rrc_cmd_ptr;


  if( rrcdata_check_if_ota_is_buffered(wrrc_as_id))
  {
    rrc_cmd_ptr = rrc_get_int_cmd_buf(wrrc_as_id);
    WRRC_MSG1_HIGH("BUF_OTA:rrcdata_process_buffered_ota_message: Creating local cmd with OTA message for proc porcessing 0x%0x", rrc_cmd_ptr);
    /* Fill in the local command header.*/

    WCDMA_MEMCPY(rrc_cmd_ptr, 
                 sizeof(rrc_cmd_int_type),
                 &rrc_buffer_ota_info.rrc_buffered_ota, 
                 sizeof(rrc_buffer_ota_info.rrc_buffered_ota));
    rrc_cmd_ptr->cmd_hdr.cmd_id = RRC_DOWNLINK_SDU_IND;
  
    rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.dl_sdu = NULL;
    /* Call the ASN.1 decoder to decode the message. */
    rrc_cmd_ptr->cmd.downlink_sdu_ind.decoded_msg =
    rrcasn1_decode_pdu( wrrc_as_id,
                         rrc_cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,(int )
                         rrc_cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type,
                         rrc_cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length
                      );
    WRRC_MSG1_HIGH("BUF_OTA:asn decode ptr 0x%0x", rrc_cmd_ptr->cmd.downlink_sdu_ind.decoded_msg);

   rrcdata_misc_info.rrc_cfn_before_decode[rrc_buffer_ota_info.procedure] = wl1_seq_get_cfn(wrrc_as_id);

   /* Reset the transaction ID*/
   rrc_buffer_ota_info.transaction_id = RRCRB_INVALID_TRANSACTION_ID;
   
    // procedure entery function
    switch(rrc_buffer_ota_info.procedure)
    {
      case RRC_PROCEDURE_RBE:
        rrcrbe_procedure_event_handler(wrrc_as_id, rrc_cmd_ptr );
        break;
      
      case RRC_PROCEDURE_RBRC:
        rrcrbrc_procedure_event_handler(wrrc_as_id, rrc_cmd_ptr );
        break;
      
      case RRC_PROCEDURE_TCR:
        rrctcrc_procedure_event_handler(wrrc_as_id, rrc_cmd_ptr );
        break;

      case RRC_PROCEDURE_PCR:
        rrcpcrc_procedure_event_handler(wrrc_as_id, rrc_cmd_ptr );
        break;
      
      case RRC_PROCEDURE_RBR:
        rrcrbr_procedure_event_handler(wrrc_as_id, rrc_cmd_ptr );
        break;

      case RRC_PROCEDURE_ASU:
        rrcasu_procedure_event_handler(wrrc_as_id, rrc_cmd_ptr );
        break;
        
      default:
        WRRC_MSG0_ERROR("BUF_OTA:Invalid procedure id passed. something wrong");
      break;
    }
    
    WRRC_MSG1_HIGH("BUF_OTA:trying to free 0x%0x", rrc_cmd_ptr);
    rrc_free_cmd_buf_sub(wrrc_as_id,rrc_cmd_ptr ); 
    rrcrbcommon_set_failure_cause(wrrc_as_id);
  }

  /*Incase of TFCC and RRC SMC are waiting for reconfiguratin to be complete,
       Send reconfiguration complete ind if OC set is failed or set notfiy_smc flag so that procedure(TFCC/SMC) are notified when OC is cleared
   */
  if((rrctfcc_is_reconfig_in_progress(wrrc_as_id) == TRUE) || 
     (rrcsmc_is_reconfig_in_progress(wrrc_as_id) == TRUE))
  {
    if(rrcllc_get_ordered_config_status(wrrc_as_id) == OC_NOT_SET)
    {
     /*Post reconfig complete ind if any procedure is pending to be processed and OTA processing is failed*/
      rrcllc_post_reconfig_complete_ind_for_failure(wrrc_as_id);
    }
    else
    {
      rrcllc_register_for_reconfig_cnf(wrrc_as_id);
    }
   
  }

  rrc_buffer_ota_info.rrc_buffer_ota_message = FALSE;
  rrcdata_reset_buffered_ota_info(wrrc_as_id);
  WRRC_MSG0_HIGH("BUF_OTA:Buffered message is processed");
  return;
}

/*===========================================================================

FUNCTION rrcdata_post_buffer_ota_after_activation_time_expiry

DESCRIPTION

  This function posts cmd which triggers buffered OTA processing

  
DEPENDENCIES


RETURN VALUE
None
 

SIDE EFFECTS

  None

===========================================================================*/

void rrcdata_post_buffer_ota_after_activation_time_expiry(sys_modem_as_id_e_type wrrc_as_id)
{
  rrc_cmd_type  *rrc_cmd_ptr;

  if(rrcdata_check_if_ota_is_buffered(wrrc_as_id))
  {
    rrc_cmd_ptr = rrc_get_int_cmd_buf(wrrc_as_id);
  WRRC_MSG0_HIGH("BUF_OTA:Posting RRC_POST_BUFFER_OTA_IND");
  /* Fill in the local command header.*/
  rrc_cmd_ptr->cmd_hdr.cmd_id = RRC_POST_BUFFER_OTA_IND;
  
    /* Send the internal command */
  rrc_put_int_cmd( wrrc_as_id, rrc_cmd_ptr);
  }  
  else
  {
    WRRC_MSG0_ERROR("BUF_OTA: No Procedure is backedup");
  }
  return;
}

/*===========================================================================

FUNCTION rrcdata_reset_buffed_ota_info

DESCRIPTION

  This function backup OTA message in ecoded format. Heap memory is allcoated for saving OTA message.

  
DEPENDENCIES


RETURN VALUE
None
 

SIDE EFFECTS

  None

===========================================================================*/

void rrcdata_reset_buffered_ota_info(sys_modem_as_id_e_type wrrc_as_id)
{
  if(rrc_buffer_ota_info.rrc_buffer_ota_message == TRUE)
  {
    WRRC_MSG0_HIGH("BUF_OTA:rrcdata_reset_buffered_ota_info");
    if(rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.dl_sdu != NULL)
    {
      //Free the memory allocated for the received downlink SDU.
      rrc_free( rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.dl_sdu );
    }

    if( rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.decoded_msg != NULL)
    {
      WRRC_MSG0_ERROR("BUF_OTA: Something wrong. decoded_msg is supposed to be NULL ");
    }
  }
  rrc_buffer_ota_info.rrc_buffer_ota_message = FALSE;
  rrc_buffer_ota_info.transaction_id = RRCRB_INVALID_TRANSACTION_ID;
  rrc_buffer_ota_info.rrc_buffered_ota_activation_time_expired_during_lta = FALSE;
  rrc_buffer_ota_info.procedure = RRC_PROCEDURE_NONE;
  rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.dl_sdu = NULL;
  rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.decoded_msg = NULL;
  rrctmr_stop_timer(wrrc_as_id, RRCTMR_OTA_BUFFER_ACTIVATION_TIMER);
  return;
}
/*===========================================================================

FUNCTION rrcdata_check_buffer_ota_during_activatin_time_nv_support

DESCRIPTION

  This function returns buffer_ota_during_activation nv value

  
DEPENDENCIES


RETURN VALUE
None
 

SIDE EFFECTS

  None

===========================================================================*/

uint8 rrcdata_check_buffer_ota_during_activation_time_nv_support(sys_modem_as_id_e_type wrrc_as_id)
{

  return(rrcnv_data.wcdma_rrc_support_lta.enable_lta_during_act_time);
}

/*===========================================================================

FUNCTION rrcdata_check_buffer_ota_during_activation_time_feature_support

DESCRIPTION

  This function returns TRUE if DUAL SIM is configured.

  
DEPENDENCIES


RETURN VALUE
None
 

SIDE EFFECTS

  None

===========================================================================*/

boolean rrcdata_check_buffer_ota_during_activation_time_feature_support(sys_modem_as_id_e_type wrrc_as_id)
{
  return((rrcdata_check_buffer_ota_during_activation_time_nv_support(wrrc_as_id)) &&
         (rrc_dsds_int_data.wcdma_is_ue_in_dual_standby == TRUE)); 

}

/*===========================================================================

FUNCTION rrcdata_check_if_ota_is_buffered

DESCRIPTION

  This function returns TRUE if OTA is buffered to be proceseed after activatin time

  
DEPENDENCIES


RETURN VALUE
None
 

SIDE EFFECTS

  None

===========================================================================*/

boolean rrcdata_check_if_ota_is_buffered(sys_modem_as_id_e_type wrrc_as_id)
{

  return((TRUE == rrc_buffer_ota_info.rrc_buffer_ota_message) &&
         (NULL != rrc_buffer_ota_info.rrc_buffered_ota.cmd.downlink_sdu_ind.dl_sdu));
}
#endif  /*FEATURE_WCDMA_ALLOW_LTA_DURING_ACT_TIME*/
/*========================================================================
FUNCTION register_ssgccs_3g_cell_info_msg_handler

DESCRIPTION
    Register callback handler for cell info

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_cell_info_msg_handler(void* msg_handler)
{
  cell_info_3g_msg_handler = (ssgccs_3g_cell_info_msg_handler) msg_handler;

} /* register_ssgccs_3g_cell_info_msg_handler() */

/*========================================================================
FUNCTION register_ssgccs_3g_rrc_connection_request_msg_handler

DESCRIPTION
    Register callback handler for RRC connection request

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_rrc_connection_request_msg_handler(void* msg_handler)
{
  connection_request_3g_msg_handler = (ssgccs_3g_rrc_connection_request_msg_handler) msg_handler;
} /* register_ssgccs_3g_rrc_connection_request_msg_handler() */
/*========================================================================
FUNCTION register_ssgccs_3g_rrc_connection_reject_msg_handler

DESCRIPTION
    Register callback handler for RRC connection reject

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_rrc_connection_reject_msg_handler(void* msg_handler)
{
  connection_reject_3g_msg_handler = (ssgccs_3g_rrc_connection_reject_msg_handler) msg_handler;
} /* register_ssgccs_3g_rrc_connection_reject_msg_handler() */

/*========================================================================
FUNCTION register_ssgccs_3g_security_mode_command_msg_handler

DESCRIPTION
    Register callback handler for security mode command

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_security_mode_command_msg_handler(void* msg_handler)
{
  security_mode_command_3g_msg_handler = (ssgccs_3g_security_mode_command_msg_handler) msg_handler;
} /* register_ssgccs_3g_security_mode_command_msg_handler() */

/*========================================================================
FUNCTION register_ssgccs_3g_rrc_connection_release_msg_handler

DESCRIPTION
    Register callback handler for RRC connection release

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_rrc_connection_release_msg_handler(void* msg_handler)
{
  connection_release_3g_msg_handler = (ssgccs_3g_rrc_connection_release_msg_handler) msg_handler;
} /* register_ssgccs_3g_rrc_connection_release_msg_handler()*/