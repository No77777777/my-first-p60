/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                  TM Core

GENERAL DESCRIPTION
  This module makes up the TM-Core module.

EXTERNALIZED FUNCTIONS


INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2006 - 2012 Qualcomm Technologies Incorporated.
Qualcomm Confidential and Proprietary. All Rights Reserved.
Copyright (c) 2013 - 2015 Qualcomm Atheros, Inc.
Qualcomm Atheros Confidential and Proprietary. All Rights Reserved.
Copyright (c) 2016 - 2022 Qualcomm Technologies, Inc. 
All Rights Reserved.
Confidential and Proprietary - Qualcomm Technologies, Inc.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/src/tm_core.c#45 $ 
  $DateTime: 2025/02/25 04:54:33 $ 
  $Author: pwbldsvc $ 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/29/21   sai     Mapping LPPM abort reason to proper PD END status
03/01/22   jv      Disable 911 client during L->W CSFB early rcvr end
12/15/21   sai     Added new NV for ODCPI and change in ODCPI back off logic
06/10/20   pa      Add support for PDR Mag Cal State
11/17/21   dy      added handling of pd event while disabling the secondary client 
09/14/20   ska     Reading supl udp select in carrier specific efs nv
07/13/20   skm     Ensure we reg/dereg with TX MGR by reading correct Sub's NV for nDDS AGPS session.
07/08/20   skm     ShortCodes SpecialRequest support for START_REQ and GET_BEST_POS 
06/19/20   mj      Do not remove protocol client from LDM when RCVR is turning OFF. Let protocol initiate the cleanup
06/02/20   mj      Handle E911 CPI fix qualification when E911 switches to main
03/06/20   mj      Send TM_STOP_REASON_COMPLETED instead of E911 when apptrack is switched
01/28/20   anksha  Added support for navic assist download
01/28/20   jvn     To fix stack overflow, moving local variables to heap
01/17/20   sj      Send LPPe capabilities when E911 simulate NV 1991 is set
12/17/19   jvn     Stop CPI on standalone session termination only in non-concurrent state
11/08/19   py      Klockwork fix to check pointer is not NULL before accessing
12/04/19   nath    Fix to avoid aborting single shot after session continue req.
12/02/19   mj      Do not switch clients when not in concurrent mode
12/12/19   py      Clearing UMTS SUPL cached position on UMTS OTA RESET
11/01/19   jvn     Fix to disable active client,once MTLR is received in non-concurrent case
11/04/19   py      Navic assistance data deletion
10/16/19   mj      Start CPI request when E911 client is enabled
10/17/19   mj      E911 end handling during concurrency
10/15/19   ska     Added a condition to check active_session while calling abort_recover in 
                   tm_core_reset_location_service_done_handler and allocated dynamic memory to ext_status_info_type 
10/03/19   nath    Changes related to NV74210 when NV74191 is 1
09/30/19   skm     Support for 911 nDDS on DSDS targets 
09/09/19   nath    Almanac and ephimeris flag reporting in NAVIC
08/20/19   mj      APMP changes to call GERA or MGP depending on GERA feature
08/12/19   sk      Added a NV to set second boundary aligned NMEA Fix report
08/06/19   mj      Change E911 state to previous state before abort_recover for CSFB
07/31/19   mj      Handle removal of client only if active handle matches for stop session
07/05/19   mj      Handle sub ready event to read IMSI for SUPL
06/20/19   skm     Z-Axis support for DBH
06/19/19   sj      Fix UMTS CP session abort logic on Srv System change.
07/03/19   mj      Add concurrency handling during LTE -> 1x transition
06/14/19   nath    Heading validity check added in best avail report
06/10/19   py      If WWAN is unavailable sent out GENERAL ERROR as status
06/06/19   sj      Disable AGNSS for 5G NR.
05/23/19   py      Fill emergency serving system and asid for Emerg. call END event
05/14/19   mj      Recheck concurrency conditions when MO-LBS TBF>1
05/09/19   mj      Fix concurrency issue during CSFB L->W/G
05/01/19   mj      Get config values from respective subs for Privacy NVs
04/30/19   mj      Abort recover after hard reset is complete
04/29/19   sj      Add GNSS Diag Buffer feature
04/23/19   gk      Address notifications issues wrt SUPL PO and ES bit
04/12/19   py      On sub-refresh and dd sub change , notify protocol with sub
04/09/19   mj      Deprecate NV1930
03/29/19   py      Handle gracefully if Command CMD_INJECT_WIFI_ON_DEMAND is failed
02/19/19   skm     SUPL over WIFI phase 2
02/15/19   mj      Location Privacy Feature
01/17/19   nath    Rearranged wifi scan result array to filter the valid mac addresses.
01/14/19   mj      SV filter support for E911 MSA main client during concurrency
01/10/19   skm     Use the residual time for QOS when switching from secondary to primary client
12/24/18   py      Added support to delete TGD data
12/19/18   nath    Fix to send saved measurement on resume after OOS in a WCDMA CP session.
11/30/18   skm     911 VoWIFI IoT fix for NO_SRV on WIFI
10/15/18   nath    Added a NV to control dynamic HEPE usage
09/25/18   nath    Added PQDTM change
08/07/18   skm     AGPS feature Support Mask
07/20/18   skm     Boeing CaaS Feature
07/17/18   nath    added tm_core_check_sub_capability to enhance debugging
07/25/18    sj      Abort Early Start, LPP CP, UP NILR session if serving rat changes from LTE->OOS->W.
06/04/18   sj      Do not allow concurrency in 1x/HDR
05/30/18   py      change data type of velHor & velVert in extended report to float for precision
04/25/18   mj      Handle case TM_STOP_REASON_SUPL_SESS_PREEMT during stop session
04/11/18   py      Protocols notified on DD sub change
04/02/18   mj      E911 Apptracking Concurrency Support Phase 2
04/04/18   py      CarrierPhaseUnc copied to pdsm_SVMeasDst
03/28/18   gk      Always enable cycleslip count in the measurement report
03/20/18   skm     Supl E911 over Wifi
03/09/18   py     Remove "FEATURE_MMGSDI_SESSION_LIB" dependency. True always.
02/26/18   mj      Changes to allow MO apptrack while in E911 state when 1930 is LOW priority
01/30/18   mj      Changes to allow MO apptrack while in E911 state when 1930 is LOW priority
12/12/17   muk     Add support to output numClockResets
12/07/17   ap      Code to test SRVCC handover cross case using DTF
11/02/17   sj      Remove ECBM state, rely on CM MSG Router Emergency Mode status messages
07/19/17   skm     Turn Early receiver OFF at 911 call end so that apptracking sessions are accepted.
05/30/17   kpn     Nascent version to check the functionality of DTF for SM
12/14/17   mj      Block sessions during SARF 
10/30/17   nath    Updated loc_unc_horizontal,alongAxisUnc and perpAxisUnc to float
10/24/17   rk      In L+L mode choose the correct srv_system field to know the RAT.
10/09/17   ak      Changes to start early RX start session on connected RAT.
07/25/17   ak      Changes to clean-up LPC/LDM on RAT change
07/13/17   skm     Check for 911 session ON when turning rcvr OFF on GERA client Disable from LM
06/16/17   mj      Changes to not remove E911 client from LDM when E911 call ends
06/15/17   kpn     Correct parts of code to pass the appropriate AS ID
05/30/17   kpn     Nascent version to check the functionality of DTF for SM
06/06/17   mj      Fix issue where VX fails with E911 concurrency code changes
05/24/17   ak      Code optimization and code review comments addressed for E911 concurrency feature.
05/21/17   ak      Trigger CPI/Time injection on ME/PE recovery.
05/19/17   gk      Dont request wishlist during a tech sel or best pos
05/08/17   rn      Modified NV for LPP12 2 timer feature config to support LPP CP.
05/05/17   kpn     Clear cache of constellations whose meas blk is not received
04/14/17   mj      Fix to allow E911 to continue when apptrack ends in concurrent mode
04/14/17   skm     stop GET_BEST_POS from triggering TCP connection request to download AD
03/30/17   sum     Added support for Horizontal velocity greater than 0x7fff in tm_core_xlate_pos_to_ext_status()
03/27/17   mj      Fix to avoid injecting wishlist info during MSA in concurrent mode
03/21/17   gk      Allow Xtra injection during 911
03/10/17   gk      Disable Support for IONO in AGPS
03/10/17   gk      Disable Support for UTC model in AGPS
02/23/17   mj      Support for GNSS Engine Monitoring
02/21/17   kpn     Support of 1x CP on Sub2
02/17/17   skm     Add SRN (BTLE) and UBP (Baro) support for Lppe 2
01/24/17   kpn     L+L changes and support for E911 CP session on non-DD sub
01/16/17   muk     Fix for Meas Sequence ordering
01/15/17   gk      LPPM SV status should be mapped to TRACK
01/04/17   sum     Removed unnecessary call to tm_sim_mmgsdi_init(),same client ID and sessions handle can be used in all SIM swap cases
11/22/16   rn      Added Xtra Integrity support.
11/16/16   mj      E911 and apptracking concurrency support
11/04/16   muk     Enable QZSS meas and Poly data
10/26/16   nl      SET/ GET mismatch fix for nmea_ext_sentence_mask 
10/25/16   rk      Start & stop inconsistency tracking when ODCPI request is placed.
09/09/16   rk      Simulated e911 status isn't propagated to LM
09/06/16   mj      Fix issue when there is no session on going, TM did not remove client from GERA
09/01/16   yh      Convert position heading from Radian to Degree
08/30/16   skm     Added NV for default LPP12 2 timer feature config
08/30/16   kpn     Add slot id into mmsgsdi global session id table to support pbm_session_record_read()
08/12/16   mj      Initialize Leap Second Info struct during tm_core init
07/15/16   mj      Indicate the source of assistance data sent to LM
07/05/16   rk      Pass all measurements to protocols. It will decide which measurements should be
                   reported to server.
06/30/16   gk      LPPe config changes cleanup
06/20/16   mj      Support to retrieve Leap Second from MGP
05/26/16   muk     Send all the Poly reports to active Loc_MW session
05/25/16   mj      Add separate masks for health and IONO for GAL and BDS
05/15/16   ss      Removing CPI request based on Punc and AD download failure
                     CPI will be requested in GNSS session only based on TechSel Rules
05/07/16   gk      cache and use the right subid when refresh is received
05/01/16   muk     Change NV73542, to Enable Gnss Measurement by Default 
04/27/16   ak      Populate ENU velocity and uncertainity in position report
04/15/16   ld      Added NHz NV support  
04/04/16   mj      Add GERA support
03/25/16   pk      During OTA Reset, Call PGI api to reset SIB8/16 Decode
02/29/16   muk     Add support to output numClockResets
01/20/16   mj      Add a new flag b_e911_session to indicate e911 session in progress
01/05/16   mc      Dynamically allocate variables for huge structures to reduce stack usage
01/04/16   mj      Move changes to check sub id sanity to the call back function
11/25/15   mc      removed debug message in tm_core_is_on_demand_session_on()
02/17/16   mj      Add support to handle bestPosition fixes from LM
02/17/16   mj      Added NV support to enable/disable CPI injections and early exit logic
02/17/16   gk      During E911 force injection of refL
02/07/16   gk      Add LPPe config support
10/27/15   mj      Handle OTA Reset for UMTS
10/20/15   gk      request CPI for Standalone sessions
09/16/15   rk      Use the pre-defined GLONASS & 4G position method bits in NV 1920 to
                   enabled or advertise GLONASS & 4G position method support respectively.
09/16/15   skm     BDS Health delete fix
08/26/15   rk      Added new OTDOA stop engine request processing support.
08/26/15   muk     TL 1.0 integration
08/05/15   rh      Moved carrier-specific NV 1920 (pos mode support) to TM core 
08/04/15   js      Default Enablement of XTRA download 
07/31/15   mj      Changes to fix E911 on second SUB in G+G config
07/08/15   rh      Changed NV refresh to be done on one specifc sub
07/03/15   jv      Added QZSS Support
06/25/15   gk      Abort SVLTE session when 1x CP starts only when there is a ongoing session
06/22/15   gk      Write leap second change to NV at GPS off if there was a change during the session
06/09/15   ssu      Adding NV for GM filtering of WLAN freq. 
06/09/15   mj      Add support for W+W feature
04/30/15   mj      Add support for early session start for WCDMA for E911
04/16/15   jv      Lowering priority of some messages from MSG_ERROR to MSG_MED
04/06/15   mj      Add new function tm_core_is_active_sess_mo
03/14/15   rh      Added support for MultiSIM NV
01/16/15   rh      Fixed missing SVInfo issue
12/18/14   rh      Fixed wrong merge for LPP CP transmission delay EFS NV read
12/14/14   gk      new session events to PDAPI
11/27/14   jv      expand PDSM_PA_DELETE_HEALTH to GLO, BDS and GAL too.
11/29/14   ssu     GM LOWI Integration
11/26/14   am      GAL lm delete request initialization was added
03/12/14   gk      Handle OTA reset when session is active. consider as end of the session
11/25/14   gk      NV item for greater than 24 AD 
11/14/14   gk      NV item for terminating SUPL after final fix 
11/13/14   rh      Added configurable LPP CP transmission delay (via EFS NV)
10/25/14   jv      Added Galileo Support
09/10/14   gk      Added delete throttle timer to prevent B-b delete commands
08/25/14   jv      16-bit SVID Support
08/19/14   ah      Copy pointer contents of GPS state info response once received
08/02/14   gk      do not abort early rcvr session with a NI session is started. 
07/21/14   ssu     Support for GM WWAN motion detector
06/07/14   ah      Map Premium Service NV value with correct SAP enablement value
05/20/14   hs      Replacing the the idx type from gnss_ to bcn_ for TDP support
05/20/14   ssu     Removing redundant code. 
05/19/14   gk     E911 NI during MO should be accepted
05/15/14   rh      Fixed Wiper not stopped issue if session is triggered as MSB initially
04/24/14   yy      Change prisoner sv to gnss sv blacklist
04/24/14   rh      Added delete prisoner SV support
04/17/14   skm     NV Async Read changes
04/01/14   ssu    Addressing a few KW errors .
04/01/14   ssu    Addressing a few KW errors .
03/26/14   ah      Added trigger of GTP WWAN session if PUNC>25km for Get Avail WWAN Pos
02/21/14   ssu     Adding NV for configuring motion sensing distance in case of challenging GPS env.
02/20/14   ssu     Adding a NV for modifying the medium responsiveness backoff value and gps challenging env backoff config.
03/10/14   ah      Added support for Set Premium Services Cfg & Get Avail WWAN Pos
02/10/14   ssu     Adding a NV for modifying the rate at which GM requests CPI injection. 
01/31/14   ss      Supporting Max acceptable GF PUNC confguration from QMI
01/27/14   mj      Remove featurization to fix issue where SGLTE code was featured out
01/13/14   rh      Added requesting XTRA download during SUPL feature 
01/10/14   ss      Supporting High Responsiveness Geofencing Config
12/23/13   mj      Remove FEATURE_DUAL_SIM and FEATURE_TRIPLE_SIM
12/02/13   ah      Support GPS State Info requests for multiple clients simultaneously
09/27/13   mj      TSTS support: Define tertiary mmgsdi sessions for triple SIM
09/12/13   mj      Additional check for app_type while retrieving mmgsdi session id
06/26/13   rh      Fixed a bug related to PA Delete bitmask which BDS-only deletion is failing
06/13/13   rh      Used same pdsm_GnssNavSvInfo in SM_API & PDAPI for SV info
06/12/13   ah      Added measurement report support for BDS standalone (not yet for AGNSS)
06/11/13   mj      Report intermediate fixes for protocols when sysd is enabled
06/07/13   rh      Added assistance data deletion support for BDS
06/07/13   rh      Removed diag timer and NO_FIX report handling
03/20/13   mj      Modified callback logging
03/01/13   kj      Move to the DOG Heartbeat mechanism on DIME and beyond
01/31/13   mj      Added new handler tm_core_api_callback_handler and tm_core_api_cb_timer_handler
02/21/13   mj      Adding missed code for GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES NV read support
01/22/13   ah      Support extraction of MPG event from union tm_info_s_type
01/06/13   gk     ECID changes
12/27/12   rk      Update fix report translation function to copy sensor (gyro/accel) 
                   bias report structure. 
12/04/12   rk      CR 408753 - Changes relating to extended SV parameter enhancement in GNSSFixReport.
11/06/12   mj      Fixed issue where NV_CGPS_UTC_GPS_TIME_OFFSET_I was not updated
10/09/12   rh      Added a few error code mappings for IS801 session
09/07/12   mj      Added tm_core_external_cell_db_proc and support for existing cell db proc for SGLTE
09/03/12   rh      Added support for NI message injection
08/01/12   mj      Added handler for GET PARAM request (tm_core_get_param_msg_handler)
07/18/12   mj      Added GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES NV read support
06/11/12   rk      Added GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT NV read support 
04/18/12   ssu     Using sm_PositionReportStructType when reporting GPS state from ME. 
03/01/12   rh      Added tm_core_handle_engine_error_recovery_report()
02/16/12   gk      Altitude also reported in float now
02/06/12   gk      LPP enablement via PDAPI
11/23/11   ssu     Adding GFC_QMI feature.
12/13/11   gk      LPP related fixes
11/14/11   gk      Added new variable for PDAPI position inj req event
07/26/11   rb      Added handling for ptr set to NULL in TM_PHONE_EVENT_DSDS_SUBS_CHANGED  
06/03/11   gk      Clear cached position  when delete param or gps lock is sent
06/16/11   rh      Added TM_PHONE_EVENT_OTASP_STATUS_COMMITTED event processing
05/18/11   rc      Lowering priority of some standalone messages from MSG_HIGH to MSG_LOW
05/03/11   gk      cache the positin info from AD request
01/03/11   gk      Eph throttling rework
11/23/10   rh      Added TM feature to send NO_FIX as Intermediate Report
10/05/10   ns      Port CL1333689: Save status of gps state info req by SYSD.
09/22/10   MG      Not Generating gpgga, gprmc when giving out Wi-Fi fixes.
08/23/10   gk      Added new handling for optimal modes
06/06/10   rb      Add tm_core_time_validity to determine if GPS time is valid
06/22/10   gk      Request postiion injection if MSB fails
06/03/10   bp      adding smart MSB support for V1/V2
05/25/10   gk      Add new NV item for XTRA validity
04/07/10   rb      Update UTC Time Offset NV item when needed
04/02/10   gk      Added an event callback to inject position
03/03/10   ns      DSDS Support
01/13/10   gk      end session forces receiver off if OEM client.
01/08/10   gk      add rf loss to c/no for the PDAPI measurement report.
01/06/10   jlp     Changed CGPS_WHOLE_EARTH_HEPE to CGPS_WHOLE_EARTH_CEP99.
12/02/09   gk      intermediate position fixes
11/11/09   gk      set optional flags for final position, also set the uncs correctly
11/10/09   atien   Check for com failure if allowing
                   MSB fallback to Standalone with XTRA download if no XTRA data
10/12/09   gk      Force receiver off
09/30/09   gk      Support ODP 2.0
07/27/09   gk      tm_core_translate_end_status() - translate TM core end session
                   error into PDSM PD End Event error reason
08/11/09   gk      Addition ODP 2.0 changes
07/27/09   gk      tm_core_translate_end_status() - translate TM core end session
                   error into PDSM PD End Event error reason
07/23/09   gk      Support ODP 2.0
07/23/09   gk      Added DELETE_SV_NO_EXIST flag for delete params
07/13/09   gk      process end session command even when no TM session
07/10/09   atien   Spirent requested feature:  allow XTRA dload after MSB dload comm failure
07/09/09   gk      ODP 1.1 related fixes for ready mode
07/08/09   gk      undo the previous change for not reporting >1 intermediate positions
06/10/09   gk      Added Get UTC offset from NV
06/10/09   gk      ODP 1.1 support
06/04/09   gk      other intermediate position changes
05/14/09   gk      fixed gps state info handling
04/16/09   gk      Fill in xtra data correctly in the gps state
04/09/09   ns      REF LR intermediate reported routed to tm_qwip_core.c to check
                   Wifi fix status.
03/27/09   jd      Added a generic abort handling mechanism for SysD
04/02/09   ns      support for sending diag info to sysd.
03/12/09   ns      Init Sysd triggered flag
03/04/09   ns      Added support for passing gps state info to qwip
02/04/09   atien   MO-LR vs AD Call Flow Select support
01/13/09   atien   XTRA-T support
01/25/09   ns      Added support for Qwip
09/15/08   gk      Rename the on-demand struct to sm_ from cgps_
08/26/08   gk      On-Demand GPS Lock Changes
08/20/08   cl      Set op_request flag in session continue request.
08/13/08   gk      Enable force receiver off
08/08/08   cl      Enable non-encrypted lat/long when security is activated
07/30/08   cl      Set elevation/azimuth only when u_SvDirValid is valid
07/11/08   cl      Inform 1x/UMTS modules when switching serving systems
07/02/08   ank     Added LBS support for CDG OMH R-UIM Spec
04/25/08   gk      On-demand changes
04/04/08   cl      Inform 1x_up when entering UMTS system
03/13/08   cl      For standalone pos, not encrypt.
02/06/08   cl      Added accommodating "forced TM termination".
01/22/08   gk      fixed compiler warning that didnt init the vel correctly
01/21/08   cl      Allow overlapping dload and fix (fix and dload) sessions
11/13/07   gk      do not set heading to 0 if speed is 0
10/30/07   cl      Added smart MSB logic
09/14/07    gk     NMEA CONFIG set parameters support
08/14/07   ank     Inform MGP about nonexistent SVs.
09/11/07   cl      Populate ext_data when security is not enabled
08/01/07   rw      Add support for XTRA feature
08/02/07   lt      Added handling of SV-not-exist message.
07/30/07   gk      Add handling for last position
07/26/07   cl      Populate client id for gps start and end events
07/25/07   cl      Add more mapping from interal end status to external end status
07/17/07   lt      L1 interface.
06/27/07   lt      Added the passing of WCDMA in-service to UMTS submodules.
06/18/07   ld      Added support for Deleting Cell DB Information.
06/12/07   cl      Only allow PD API access when TM task is active
05/30/07   lt      Mods. to make TM's RRC and MM iface symmetrical to CM and RR.
05/27/07   rw      Add support for JGPS (KDDI) module
05/23/07   mr      Added call to generate NMEA PSTIS string
05/16/07   mr      Removed NMEA string generation for intermediate fixes
05/16/07   mr      Added calls to generate NMEA string for MSA fix from server
04/24/07   ank     Change to use CGPS_SRV_SYS enum in place of TM_CM_SRV_SYS.
04/11/07   lt      Ready for activating UMTS CGPS protocol modules.
04/04/07   gk/mr   PC Time injection handling
04/03/07   cl      Feed LR to MGP.
02/15/07   cl      Moved pdsm_getbuf in tm_core_lm_sess_info_handler
02/15/07   cl      Add support to inject pre-fix ppm
02/01/07   cl      Add feature to check if CDMA is supported
02/01/07   lt      Added UTC model handling
01/31/07   cl      Added LCS handling
12/04/06   cl      Added NV functionality
12/04/06   cl      pd api event report interface change
10/13/06   cl      Change to use SM_TM for dog report
07/12/06   cl      Initial version

============================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "gps_variation.h"
#include "comdef.h"     /* Definition for basic types and macros */
#include "customer.h"   /* Customer configuration file */
#include <stdio.h>

#include "msg.h"
#include "event.h"
#include "aries_os_api.h"
#include "aries_ipc_diag_log.h"
#include "sm_nv.h"
#include "sm_log.h"
#include "tm_data.h"    /* Definition of TM task data structure */
#include "lm_api.h"
#ifdef FEATURE_CGPS_PDCOMM
#include "pd_comms_api.h"
#endif /* FEATURE_CGPS_PDCOMM */
#include "tm_pdapi_client.h"
#include "tm_cm_iface.h"
#include "tm_ds_iface.h"
#include "tm_prtl_iface.h"
#include "tm_standalone.h"
#include "tm_diag.h"
#include <math.h>
#include "time_svc.h"
#include "tm_nv.h"
#include "memory.h"
#include "gps_common.h"
#include "lsmp_api.h"
#include "tm_ruim.h"
#include "sm_util.h"
#include "tm_spoofDetector_api.h"

#ifdef FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH /* this would be defined in custwcdma.h */
#undef FEATURE_WCDMA_GPS_CELL_ID_READING /* this would be defined in custcgps.h */
#endif

#include "mmgsdisessionlib.h"

#ifdef FEATURE_TM_SECURITY
#include "tm_security.h"
#endif /* FEATURE_TM_SECURITY */

#if defined (FEATURE_SUPPORT_CDMA_GPS_PROTOCOLS) && !defined (FEATURE_GNSS_LOW_MEMORY)
#include "tm_1x_cp.h"
#include "tm_1x_up.h"
#endif /* FEATURE_SUPPORT_CDMA_GPS_PROTOCOLS && ! FEATURE_GNSS_LOW_MEMORY */
#include "tm_nmea.h"
#include "pdapibuf.h"
#include "sm_dm_event.h"

#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
#include "tm_umts_common_utils.h"
#ifdef FEATURE_CGPS_UMTS_UP
#include "tm_umts_up_core_iface.h"
#ifdef FEATURE_USER_PLANE_MRL
#include "tm_l1_iface.h"
#include "l1lsmif.h"
#endif
#endif
#ifdef FEATURE_CGPS_UMTS_CP
#include "tm_umts_cp_core_iface.h"
#include "tm_mm_iface.h"
#ifdef FEATURE_CGPS_UMTS_CP_WCDMA
#include "tm_rrc_iface.h"
#endif
#endif
#endif


#ifdef FEATURE_CGPS_XTRA
#include "tm_xtra.h"
#endif /* FEATURE_CGPS_XTRA */

#include "mgp_api.h"

#ifdef FEATURE_CGPS_XTRA_T
#include "tm_xtra_t.h"
#endif /* FEATURE_CGPS_XTRA_T */

#define INV_SQRT_2 0.7071067811865
/* The Punc value threshold for Xtra T download request ( in meters ) */
#define PUNC_THRESHOLD_FOR_XTRA_T 25000

#include "sm_nv_efs.h"
#include "tm_xspi.h"

#include "tm_sim_utils.h"

#ifdef FEATURE_CGPS_XTRA_T
#include "tle_api.h"
#endif

#include "loc_wwan_me_api.h"

#include "gm_api.h"
#include "gfc_qmi.h"
#include "gnss_calendar_util.h"
#ifdef FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM
#error code not present
#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */

#include "tech_sel_api.h"
#ifndef FEATURE_GNSS_LEAN_BUILD
#include "tm_umts_common_utils.h"
#include "tm_auxtech_iface.h"
#endif /* ! FEATURE_GNSS_LEAN_BUILD */


#ifdef FEATURE_GNSS_GERA_ENABLE
#error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
#include "fusion_csm_api.h"

#include "gnss_consts.h"
#include "tm_loc_processing_client.h"
#include "le_iface.h"
#ifdef FEATURE_GNSS_LPPE
#include "tm_slim_iface.h"
#endif

#include "wle_api.h"
#ifdef FEATURE_GNSS_DTF
#include "tm_dtf.h"
#endif
#include "gnss_diag_buf.h"

#include "tm_umts_up_supl_comm.h"

tm_core_device_config_s_type tm_core_device_config = { 0 };
tm_core_cached_pos_s_type z_cached_pos_info = { 0 };
tm_core_cached_ref_pos_s_type z_cached_ref_pos_info = { 0 };
tm_core_cached_meas_s_type z_cached_meas_info = { 0 };
tm_core_cached_meas_s_type *p_meas_cache = &z_cached_meas_info;

#ifndef FEATURE_GNSS_LEAN_BUILD
extern tm_lpc_control_data_s_type *z_lpc_control_data;
#endif /* ! FEATURE_GNSS_LEAN_BUILD */
#define TM_LOC_MAX_HIGH_HEPE_THRESHOLD 250

extern void tm_lpp_tx_mgr_init(uint8 u_client_handle);
extern void tm_lpp_cp_prtl_iface_init(void);
static lm_mo_mt_mode_e_type tm_core_mo_mt_mode(uint32 q_sessHandle);
void tm_mapping_between_sess_start_sess_continue(boolean v_start_to_continue, tm_sess_req_start_param_s_type *pz_sess_start_param,
                                                 tm_sess_req_continue_param_s_type *pz_sess_continue_req_param);
static ldm_client_id_e_type tm_core_map_lm_to_ldm_client(lm_ldm_client_disable_type e_lm_ldm_client_id);
static void tm_core_concurrent_mode_e911_service_interaction(tm_prtl_type prtl_type, cgps_SrvSystemType e_srv_system);
static boolean tm_core_abort_e911_in_concurrency(const int prtlTableIndex, const tm_sess_handle_type e_ldm_sess_handle);
static boolean tm_allow_mo_supl_during_concurrency(void);

/*===========================================================================
                 Data Definition
===========================================================================*/
#define PA_AIDING_DATA_DELETE_MASK  (  PDSM_PA_DELETE_TIME    \
                                     | PDSM_PA_DELETE_POS     \
                                     | PDSM_PA_DELETE_EPH     \
                                     | PDSM_PA_DELETE_ALM     \
                                     | PDSM_PA_DELETE_IONO    \
                                     | PDSM_PA_DELETE_UTC     \
                                     | PDSM_PA_DELETE_HEALTH  \
                                     | PDSM_PA_DELETE_SVDIR   \
                                     | PDSM_PA_DELETE_SVSTEER \
                                     | PDSM_PA_DELETE_SADATA  \
                                     | PDSM_PA_DELETE_RTI     \
                                     | PDSM_PA_DELETE_CELLDB_INFO \
                                     | PDSM_PA_DELETE_ALM_CORR \
                                     | PDSM_PA_DELETE_FREQ_BIAS_EST \
                                     | PDSM_PA_DELETE_SV_NO_EXIST \
                                     | PDSM_PA_DELETE_ALM_CORR_GLO \
                                     | PDSM_PA_DELETE_ALM_GLO \
                                     | PDSM_PA_DELETE_ALM_SBAS \
                                     | PDSM_PA_DELETE_EPH_GLO \
                                     | PDSM_PA_DELETE_EPH_SBAS \
                                     | PDSM_PA_DELETE_SVDIR_GLO \
                                     | PDSM_PA_DELETE_SVDIR_SBAS \
                                     | PDSM_PA_DELETE_SVSTEER_GLO \
                                     | PDSM_PA_DELETE_SVSTEER_SBAS \
                                     | PDSM_PA_DELETE_TIME_GLO \
                                     | PDSM_PA_DELETE_TIME_GPS \
                                     | PDSM_PA_DELETE_TIME_BDS \
                                     | PDSM_PA_DELETE_EPH_BDS \
                                     | PDSM_PA_DELETE_ALM_BDS \
                                     | PDSM_PA_DELETE_SVDIR_BDS \
                                     | PDSM_PA_DELETE_SVSTEER_BDS \
                                     | PDSM_PA_DELETE_ALM_CORR_BDS \
                                     | PDSM_PA_DELETE_IONO_QZSS \
                                     | PDSM_PA_DELETE_EPH_QZSS \
                                     | PDSM_PA_DELETE_ALM_QZSS \
                                     | PDSM_PA_DELETE_SVDIR_QZSS \
                                     | PDSM_PA_DELETE_SVSTEER_QZSS \
                                     | PDSM_PA_DELETE_HEALTH_QZSS \
                                     | PDSM_PA_DELETE_ALM_CORR_QZSS \
                                     | PDSM_PA_DELETE_TIME_GAL \
                                     | PDSM_PA_DELETE_EPH_GAL \
                                     | PDSM_PA_DELETE_ALM_GAL \
                                     | PDSM_PA_DELETE_SVDIR_GAL \
                                     | PDSM_PA_DELETE_SVSTEER_GAL \
                                     | PDSM_PA_DELETE_ALM_CORR_GAL \
                                     | PDSM_PA_DELETE_CLOCK_INFO \
                                     | PDSM_PA_DELETE_HEALTH_GLO\
                                     | PDSM_PA_DELETE_HEALTH_BDS\
                                     | PDSM_PA_DELETE_HEALTH_GAL\
                                     | PDSM_PA_DELETE_IONO_BDS\
                                     | PDSM_PA_DELETE_IONO_GAL\
                                     | PDSM_PA_DELETE_MB_DATA\
                                     )

#define PA_AIDING_DATA_DELETE_MASK_EXT  (  PDSM_PA_EXT_DELETE_TGD_DATA_GPS    \
                                     | PDSM_PA_EXT_DELETE_TGD_DATA_QZSS\
                                     | PDSM_PA_EXT_DELETE_TGD_DATA_GLO\
                                     | PDSM_PA_EXT_DELETE_TGD_DATA_BDS\
                                     | PDSM_PA_EXT_DELETE_TGD_DATA_GAL\
                                     | PDSM_PA_EXT_DELETE_HEALTH_NAVIC\
                                     | PDSM_PA_EXT_DELETE_IONO_NAVIC\
                                     | PDSM_PA_EXT_DELETE_TIME_NAVIC\
                                     | PDSM_PA_EXT_DELETE_GNSS_SV_BLACKLIST_NAVIC\
                                     | PDSM_PA_EXT_DELETE_ALM_CORR_NAVIC\
                                     | PDSM_PA_EXT_DELETE_SVSTEER_NAVIC\
                                     | PDSM_PA_EXT_DELETE_SVDIR_NAVIC\
                                     | PDSM_PA_EXT_DELETE_ALM_NAVIC\
                                     | PDSM_PA_EXT_DELETE_EPH_NAVIC\
                                     )

#define PA_AIDING_DATA_INJECT_MASK  (PDSM_PA_TIME_UNC | PDSM_PA_POS_UNC | PDSM_PA_TIME_OFFSET | PDSM_PA_POS_OFFSET )

#define C_MIN_EPH_HAVE_THRESHOLD 8
#define C_MAX_EPH_NEED_THRESHOLD 1
#define C_MIN_SV_IN_VIEW_THRESHOLD 6

/* for GLONASS, use the same ratio as used by GPS, adj. threshold with the total number   
   of SVs in GLONASS being 24*/
#define C_MIN_GLO_EPH_HAVE_THRESHOLD 6
#define C_MAX_GLO_EPH_NEED_THRESHOLD 1
#define C_MIN_GLO_SV_IN_VIEW_THRESHOLD 5

/* using 75% of the entire constellation, same percentage as   
  C_COMPLETE_CONSTALLATION_ALM_THRESHOLD to GPS */
#define C_COMPLETE_CONSTALLATION_GLO_ALM_THRESHOLD 18

#define TM_CORE_MAX_NUM_API_CALLBACKS (10)
#define TM_CORE_API_CALLBACK_INTERVAL_MS (1000)

#define TM_CORE_CLIENT_BITMASK(x) (1 << (x))

/* Define Leap Second offset uncertainity during module init */
#define TM_CORE_LEAP_SEC_INIT_UNC (1)

/* TBF 1sec */
#define TM_CORE_TBF_ONE_SECOND_MS (1000)

/* This is a global variable which stores all the information of TM-core */
tm_core_param_s_type     tm_core_info;

/* EFS cert can be updated anytime to program SUPL CERT for a new subscription.
   This boolean will force an EFS read of SUPL cert if PDAPI wrote new SUPL CERT into EFS.
*/
boolean  tm_use_efs_for_supl_cert = TRUE;

/* Store current DD Subscription ID */
sys_modem_as_id_e_type      tm_current_dd_subs_id =  SYS_MODEM_AS_ID_NONE;

/* Table to store MMGSDI session info for each card */
static tm_mmgsdi_session_id_table_entry_s_type tm_mmgsdi_session_id_table[DSDS_MAX_SESS_TYPES] =
{ { MMGSDI_GW_PROV_PRI_SESSION },
  { MMGSDI_1X_PROV_PRI_SESSION },
  { MMGSDI_GW_PROV_SEC_SESSION },
  { MMGSDI_1X_PROV_SEC_SESSION },
  { MMGSDI_GW_PROV_TER_SESSION },
  { MMGSDI_1X_PROV_TER_SESSION },
};

/* Define nominal Time-Between-Fixes to be 1000ms
   Most applications use this. 
   For NHz operation, the TBF will be less than 1000ms.
   If NHz mode is not enable, default the TBF to 1000ms
*/
#define TM_CORE_NOMINAL_TBF_MS (1000)

/* NV buffer, used to read NV items */
static nv_item_type      tm_nv_read_buffer;

/* Counter for EVENT_GPS_PD_FIX_START */
uint16 tm_core_event_counter = 0;

static const uint8 u_DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const uint8 u_DaysInMonthLeap[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static void  tm_core_xlate_gps_state_to_ext_status(tm_core_gps_state_info_s_type *p_gps_state_info,
                                                   pdsm_pd_ext_gps_state_s_type *pd_gps_state_info);
static void tm_core_xlate_pd_ext_pos_to_qmi(const sm_FixInfoStructType *p_GnssFixRpt,
                                            pdsm_pd_ext_pos_rpt_s_type    *p_PdsmExtPos);

static float tm_core_calculate_2d_punc(const sm_FixInfoStructType *pz_GnssFixRpt);

/*Boolean to track whether reset location service is in progress*/
static boolean tm_reset_location_service_in_progress = FALSE;

static boolean tm_core_sess_req_continue_for_emergency_session(tm_prtl_type                       prtl_type,
                                                               tm_sess_handle_type                sess_handle,
                                                               tm_sess_req_start_param_s_type  sess_continue_param);

#ifdef FEATURE_GNSS_GERA_ENABLE
#error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

static void tm_core_send_ldm_rcvr_change(lm_gera_rcvr_mode_change_reason_e_type e_rcvr_change_reason, boolean v_forceMgpOff);
static boolean tm_core_stop_emergency_session(void);

#define TM_MAX_PUNC_VALUE_FOR_POS_INJ_REQ 10000 /* 10km */

static uint8 tm_cb_timer_counter = 0;
static uint8 tm_call_back_log[GPS_MAX_CALLBACK_CLIENTS];

static boolean b_otdoa_sess_start_sent = FALSE;
static boolean v_no_force_idle_for_secondary_client = FALSE;
static boolean v_enable_secondary_client_on_ldm = TRUE;
static prtl_sess_info_param_u_type *up_cache_sess_info_param = NULL;
static boolean b_emerg_sess_start_sent = FALSE;

#define TM_CORE_RESIDUAL_TIMER_THRESHOLD_MS (1000)

tm_sess_handle_type tm_core_get_ni_sess_handle(void);

static void tm_core_abort_concurrent_sessions(tm_sess_stop_reason_e_type e_abort_reason);

/*===========================================================================

FUNCTION tm_core_update_session_state

DESCRIPTION
  This function determines if a state transition request is valid and
  set tm session state accordingly.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_update_session_state
(
   tm_core_session_state_e_type new_state
   )
{
  if (new_state != tm_core_info.session_info.session_state)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core state transition [%d] -> [%d] for handle [%d]",
          tm_core_info.session_info.session_state,
          new_state,
          tm_core_info.session_info.active_sess_handle);

    tm_core_info.session_info.session_state = new_state;
  }
}

/*===========================================================================

FUNCTION tm_core_read_carrier_specific_nv

DESCRIPTION
  This function read non-EFS NV items which may be carrier-specific
  (at bootup time or during SIM hot swap)
  Will read from hardware and may save to NV cache(if available)
  
  It will refresh NV for a specific sub

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void tm_core_read_carrier_specific_nv(sys_modem_as_id_e_type sub)
{
  if ((sub < SM_CONFIG_MIN_SUB_ID) || (sub > SM_CONFIG_MAX_SUB_ID))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Read CS NV invalid sub %d", sub);
    return;
  }

  /* GPS lock (NV #452 GPS LOCK) */
  (void)sm_nv_read_from_hw_sub_and_cache(NV_GPS1_LOCK_I, (void *)&tm_nv_read_buffer, sub);
  tm_core_info.config_info.subs_cfgs[sub].gps_lock = (pdsm_gps_lock_e_type)tm_nv_read_buffer.gps1_lock;

  /* GPS NMEA String Format (NV #6789  GNSS NMEA Sentence Type ID) */
  (void)sm_nv_read_from_hw_sub_and_cache(NV_GNSS_NMEA_SENTENCE_TYPE_I, (void *)&tm_nv_read_buffer, sub);
  tm_core_info.config_info.subs_cfgs[sub].nmea_output_format = (pdsm_pa_nmea_type)tm_nv_read_buffer.gnss_nmea_sentence_type;

  /* Glonass NMEA Extended String Format (NV #6790 GNSS NMEA Extended Sentence Type ID) */
  (void)sm_nv_read_from_hw_sub_and_cache(NV_GNSS_NMEA_EXTENDED_SENTENCE_TYPE_I, (void *)&tm_nv_read_buffer, sub);
  tm_core_info.config_info.subs_cfgs[sub].nmea_output_format |= ((pdsm_pa_nmea_type)tm_nv_read_buffer.gnss_nmea_extended_sentence_type) << PDAPI_NMEA_NUM_LEGACY;
  /* GAGNS is deprecated, so always force disable */
  tm_core_info.config_info.subs_cfgs[sub].nmea_output_format &= ~PDAPI_NMEA_GAGNS;

  /* Positioning Modes Supported (NV 1920)  */
  (void)sm_nv_read_from_hw_sub_and_cache(NV_AAGPS_POSITIONING_MODES_SUPPORTED_I, (void *)&tm_nv_read_buffer, sub);
  tm_core_info.config_info.subs_cfgs[sub].positioning_modes_supported = tm_nv_read_buffer.aagps_positioning_modes_supported;
}

/*===========================================================================

FUNCTION tm_core_read_carrier_specific_efs_nv

DESCRIPTION
  This function read EFS NV items which may be carrier-specific
  (at bootup time or during SIM hot swap)
  Will read from hardware and may save to EFS NV cache(if available)
  
  It will refresh NV for a specific sub.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void tm_core_read_carrier_specific_efs_nv(sys_modem_as_id_e_type sub)
{
  uint64 temp_sentence_type = 0ull;

  if ((sub < SM_CONFIG_MIN_SUB_ID) || (sub > SM_CONFIG_MAX_SUB_ID))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Read CS EFS NV invalid sub %d", sub);
    return;
  }

  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_SELECT,
      sizeof(uint8), &tm_core_info.config_info.subs_cfgs[sub].gnss_sm_udp_allow, sub) == FALSE)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Read EFS SUPL UDP failed (sub=%d)", sub);

    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].gnss_sm_udp_allow = GNSS_NV_EFS_SM_TM_SUPL_UDP_ENABLE_DEFAULT;
  }
      
  /* NV #65811 GNSS 1X UP SUPL Enable */
  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE,
                                       sizeof(uint8), &tm_core_info.config_info.subs_cfgs[sub].gnss_1x_up_supl_enable, sub) == FALSE)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Read EFS NV 1x UP failed (sub=%d)", sub);

    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].gnss_1x_up_supl_enable = GNSS_NV_EFS_SM_TM_1X_UP_SUPL_ENABLE_DEFAULT;
  }

  /* NV #067225 Assisted GPS LPP Positioning Protocol Select */
  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_LPP_ENABLE,
                                       sizeof(uint8), &tm_core_info.config_info.subs_cfgs[sub].gnss_lte_lpp_enable, sub) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].gnss_lte_lpp_enable = GNSS_NV_EFS_SM_TM_LPP_ENABLE_DEFAULT;
  }

  /* GNSS AGPS RAT Preference (NV #67217 GNSS AGPS RAT Preference) */
  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_AGPS_RAT_PREF_CONFIG,
                                       sizeof(cgps_nv_agps_rat_pref_config), &tm_core_info.config_info.subs_cfgs[sub].agps_rat_pref,
                                       sub) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].agps_rat_pref.umts_lte_prefered = GNSS_NV_EFS_SM_AGPS_RAT_PREF_UMTS_LTE_PREFERED;
    tm_core_info.config_info.subs_cfgs[sub].agps_rat_pref.agps_rat_enable_bit_mask = GNSS_NV_EFS_SM_AGPS_RAT_ENABLE_BIT_MASK;
  }

  /* NV #70192  Assisted GLONASS Positioning Protocol Select */
  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_ASSISTED_GLO_PROTOCOL_SELECT,
                                       sizeof(uint32), &tm_core_info.config_info.subs_cfgs[sub].asst_glonass_pos_protocol,
                                       sub) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].asst_glonass_pos_protocol = GNSS_NV_EFS_SM_TM_ASST_GLO_PROTOCOL_DEFAULT;
  }


  /* NV #067225 Assisted GPS LPP Positioning Protocol Select */
  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_LPPE_UP_CONFIG,
                                       sizeof(uint32), &tm_core_info.config_info.subs_cfgs[sub].gnss_lte_lppe_up_config, sub) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].gnss_lte_lppe_up_config = GNSS_NV_EFS_SM_TM_LPPE_UP_ENABLE_DEFAULT;
  }
  /* NV ? LPP CP LPPe configuration */
  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_LPPE_CP_CONFIG,
                                       sizeof(uint32), &tm_core_info.config_info.subs_cfgs[sub].gnss_lte_lppe_cp_config, sub) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].gnss_lte_lppe_cp_config = GNSS_NV_EFS_SM_TM_LPPE_CP_DEFAULT;
  }

  /* NV #73603 GNSS SM LPP Grtr 24 OTDOA AD support */
  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_SUPPORT,
                              sizeof(uint8), &tm_core_info.config_info.subs_cfgs[sub].u_gnss_lpp_grt_24_otdoa) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].u_gnss_lpp_grt_24_otdoa = GNSS_NV_EFS_SM_LPP_GRT_24_OTDOA_AD_DEFAULT;
  }
  /* NV# 73897 GNSS SM LPP Rel12 2 timer support */
  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_LPP_2TIMER_SUPPORT, sizeof(uint8),
                                       &tm_core_info.config_info.subs_cfgs[sub].u_gnss_lpp_rel12_2timer_support))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.subs_cfgs[sub].u_gnss_lpp_rel12_2timer_support = GNSS_NV_EFS_SM_LPP_2TIMER_SUPPORT_DEFAULT;
  }

#ifdef FEATURE_ABDS_RRLP_SUPL
  /* NV 73951 ABDS protocol configuration */
  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_ASSISTED_BDS_PROTOCOL_ENABLE_MASK,
                                       sizeof(uint32), &tm_core_info.config_info.subs_cfgs[sub].q_sm_assisted_bds_protocol_enable_mask, sub) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].q_sm_assisted_bds_protocol_enable_mask =
       GNSS_NV_EFS_SM_ASSISTED_BDS_PROTOCOL_ENABLE_MASK_DEFAULT;
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Read Failed. Sub%d: BDSEnable=%x", sub,
          (int32)tm_core_info.config_info.subs_cfgs[sub].q_sm_assisted_bds_protocol_enable_mask);
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Read Worked. Sub%d: BDSEnable=%x", sub,
          (int32)tm_core_info.config_info.subs_cfgs[sub].q_sm_assisted_bds_protocol_enable_mask);
  }
#endif

  /* NV 74140 configures, new SM AGPS features
   1) 0x00000001 : Enables SUPL over VoWIFI for 911
   2) 0xFFFFFFFE : Reserved for future AGPS features.*/
  if (FALSE == (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT, sizeof(uint32),
                                                 &tm_core_info.config_info.subs_cfgs[sub].q_agps_feature_support, sub)))
  {
    tm_core_info.config_info.subs_cfgs[sub].q_agps_feature_support = GNSS_NV_EFS_SM_TM_AGPS_FEATURE_SUPPORT_DEFAULT;
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SM Features. Sub%d: Features=%x", sub,
          (int32)tm_core_info.config_info.subs_cfgs[sub].q_agps_feature_support);
  }

  /* NV 74234 Specifies time in seconds to extend E911 state */

  if (FALSE == sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW,
                                                sizeof(uint32), &tm_core_info.config_info.subs_cfgs[sub].q_emergency_extension_window, sub))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.subs_cfgs[sub].q_emergency_extension_window = GNSS_NV_EFS_SM_EMERGENCY_EXTENSION_WINDOW_DEFAULT;
  }

  /* NV 74235 Represents OEM controls to config GNSS session request and notification to HLOS
                                            bit0 = CP_NILR_OEM_CTRL
                                            bit1 = SUPL_NI_OEM_CTRL
                                            bit2 = NFW_OEM_CTRL 
                                            bit3 = CP_NILR_NOTIFY_OEM_CTRL
                                            bit4 = NFW_NOTIFY_OEM_CTRL */
  if (FALSE == sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM,
                                                sizeof(uint32), &tm_core_info.config_info.subs_cfgs[sub].q_gnss_lock_ctrl_for_oem, sub))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.subs_cfgs[sub].q_gnss_lock_ctrl_for_oem = GNSS_NV_EFS_SM_GNSS_LOCK_CTRL_FOR_OEM_DEFAULT;
  }

  /* for debugging purpose */
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sub%d: 1xUP=%d LPP=%d", sub,
        (int32)tm_core_info.config_info.subs_cfgs[sub].gnss_1x_up_supl_enable,
        (int32)tm_core_info.config_info.subs_cfgs[sub].gnss_lte_lpp_enable);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "   GLO=%d RAT=0x%x(LTE=%d)",
        (int32)tm_core_info.config_info.subs_cfgs[sub].asst_glonass_pos_protocol,
        (uint32)tm_core_info.config_info.subs_cfgs[sub].agps_rat_pref.agps_rat_enable_bit_mask,
        (int32)tm_core_info.config_info.subs_cfgs[sub].agps_rat_pref.umts_lte_prefered);

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "   UDP=%d Emergency=0x%x",
        (int32)tm_core_info.config_info.subs_cfgs[sub].gnss_sm_udp_allow,
        (uint32)tm_core_info.config_info.subs_cfgs[sub].gnss_sm_emergency_support_config);

  if (sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_CONTROL_DATUM_NMEA_SENTENCE,
                                       sizeof(uint8), &tm_core_info.config_info.subs_cfgs[sub].u_control_nmea_sentence,
                                       sub) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.subs_cfgs[sub].u_control_nmea_sentence = GNSS_NV_EFS_SM_TM_WGS84_DATUM_NMEA_SENTENCE;
  }

  /* NV 74264 Extended NMEA Sentence Mask */
  if (FALSE == sm_nv_efs_reg_item_read_from_sub(GNSS_NV_EFS_SM_TM_EXTENDED_NMEA_SENTENCE_MASK_V02,
                                                sizeof(uint64), &temp_sentence_type, sub))
  {
    temp_sentence_type = GNSS_NV_EFS_SM_TM_EXTENDED_NMEA_SENTENCE_MASK_V02_DEFAULT;
  }
  tm_core_info.config_info.subs_cfgs[sub].nmea_output_format = temp_sentence_type << (PDAPI_NMEA_NUM_LEGACY + PDAPI_NMEA_NUM_EXTENDED);

  }

/*===========================================================================

FUNCTION tm_core_read_nv

DESCRIPTION
  This function read NV items to set TM-Core configuration info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_read_nv(void)
{
  uint32 j, k;
  sys_modem_as_id_e_type sub;
  tech_sel_odcpi_timers_s_type tech_sel_odcpi_timers;

  /***************************************************************************
  * Non-EFS NV Items
  ***************************************************************************/
  sm_nv_init();

  for (sub = SM_CONFIG_MIN_SUB_ID; sub <= tm_core_info.config_info.max_valid_subs_id; sub++)
  {
    /* read carrier-specific NVs */
    tm_core_read_carrier_specific_nv(sub);
  }

  /* used by UMTS to determine CP/UP in an MO session */
  (void)sm_nv_read(NV_CGPS_MO_METHOD_I, (void *)&tm_nv_read_buffer);
  tm_core_info.config_info.u_mo_method = (uint32)tm_nv_read_buffer.cgps_mo_method;

  /* NMEA string reporting type */
  (void)sm_nv_read(NV_CGPS_NMEA_CONFIG_INFO_I, (void *)&tm_nv_read_buffer);
  tm_core_info.config_info.nmea_config_type.pd_nmea_port_type = (pdsm_pa_nmea_port_e_type)tm_nv_read_buffer.cgps_nmea_config_info.nmea_port_type;

  tm_core_info.config_info.nmea_config_type.pd_nmea_reporting_type =  (pdsm_pa_nmea_reporting_e_type)tm_nv_read_buffer.cgps_nmea_config_info.nmea_reporting_type;

  /* Smart MSB */
  (void)sm_nv_read(NV_GPS1_DYNAMIC_MODE_I, (void *)&tm_nv_read_buffer);
  tm_core_info.config_info.smart_msb = (boolean)tm_nv_read_buffer.gps1_dynamic_mode;

  /* MOLR vs AD Call Flow Select */
  tm_core_info.config_info.call_flow_select_mask = 0;

  (void)sm_nv_read(NV_AAGPS_2G_MO_LRSUPPORT_I, (void *)&tm_nv_read_buffer);
  if (tm_nv_read_buffer.aagps_2g_mo_lrsupport == NV_AAGPS_3G_MO_LRSUPPORT_V)
  {
    tm_core_info.config_info.call_flow_select_mask |= PDSM_PA_CALL_FLOW_SELECT_2G;
  }

  (void)sm_nv_read(NV_AAGPS_3G_MO_LRSUPPORT_I, (void *)&tm_nv_read_buffer);
  if (tm_nv_read_buffer.aagps_3g_mo_lrsupport == NV_AAGPS_3G_MO_LRSUPPORT_V)
  {
    tm_core_info.config_info.call_flow_select_mask |= PDSM_PA_CALL_FLOW_SELECT_3G;
  }

#if 0
  /* disable till nvitems.h is updated */
  /* 1xUP MSA Disable */
  (void)sm_nv_read(NV_GNSS_1XUP_MSA_TRUSTED_MODE_DISABLE_I, (void *)&tm_nv_read_buffer);
  tm_core_info.config_info.up_1x_msa_disable = (boolean) tm_nv_read_buffer.gnss_1xup_msa_trusted_mode_disable;
#endif
  tm_core_info.config_info.up_1x_msa_disable = FALSE;


  /* initialize the new NV items that use EFS here */

  (void)sm_nv_read(NV_AAGPS_DEVELOPMENT_TEST_CONTROL5_I, (void *)&tm_nv_read_buffer);
  tm_core_info.config_info.aagps_development_test_control5 = (uint32)tm_nv_read_buffer.aagps_development_test_control5;

  /* OEM feature mask (NV 7165 32 bits) will be read by LM & TM */
  tm_core_info.config_info.accept_1xcp_mt_over_mo = FALSE;
  (void)sm_nv_read(NV_GNSS_OEM_FEATURE_MASK_I, (void *)&tm_nv_read_buffer);
  if (tm_nv_read_buffer.gnss_oem_feature_mask & NV_GNSS_OEM_FEATURE_MASK_3_1XCPMT)
  {
    tm_core_info.config_info.accept_1xcp_mt_over_mo = TRUE; /* will accept 1X CP MT even if MO is active */
  }
  if (tm_nv_read_buffer.gnss_oem_feature_mask & NV_GNSS_OEM_FEATURE_MASK_6_SUPLINIT_NOTIFY)
  {
    tm_core_info.config_info.b_supl_notify_to_ap = TRUE; /* will accept SUPL INIT even if GPS is locked */
  }

  if (tm_nv_read_buffer.gnss_oem_feature_mask & NV_GNSS_OEM_FEATURE_MASK_7_AUTO_ACCEPT_MTLR_IN_EMERGENCY)
  {
    tm_core_info.config_info.b_auto_accept_mtlr = TRUE; /* will accept MTLR during emergency even if GPS is locked */
  }

  if (tm_nv_read_buffer.gnss_oem_feature_mask & NV_GNSS_OEM_FEATURE_MASK_9_KILL_PERIODIC_AT_EMERG_END)
  {
    tm_core_info.config_info.b_end_lpp_periodic = TRUE; /* will end LPP CP periodic session after emergency end */
  }

  if ((tm_nv_read_buffer.gnss_oem_feature_mask &
          NV_GNSS_OEM_FEATURE_MASK_15_GF_CELL_CPI_ENABLE_CFG) == 0)
  {
    tm_core_info.config_info.gm_config.v_sm_gm_accept_cell_cpi_fixes = FALSE;
  }
  else
  {
    tm_core_info.config_info.gm_config.v_sm_gm_accept_cell_cpi_fixes = TRUE;
  }

  if (tm_nv_read_buffer.gnss_oem_feature_mask & NV_GNSS_OEM_FEATURE_MASK_13_END_SUPL_AFTER_FINAL_FIX)
  {
    tm_core_info.config_info.b_end_supl_after_final_fix = TRUE; /* will end SUPL session after final fix is reported to AP in MO case */
  }

  tm_core_info.config_info.q_gnss_oem_feature_mask = (uint32)tm_nv_read_buffer.gnss_oem_feature_mask;

  (void)sm_nv_read(NV_MOB_MODEL_I, (void *)&tm_nv_read_buffer);
  tm_core_info.config_info.fusion_csm_config.u_ChipsetVer = tm_nv_read_buffer.mob_model;

  /***************************************************************************
  * EFS NV Items
  ***************************************************************************/
  sm_nv_efs_init();

  for (sub = SM_CONFIG_MIN_SUB_ID; sub <= tm_core_info.config_info.max_valid_subs_id; sub++)
  {
    /* read carrier-specific EFS NVs */
    tm_core_read_carrier_specific_efs_nv(sub);
  }

  if (sm_nv_efs_reg_item_read(CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE,
                              sizeof(uint16),
                              &tm_core_info.config_info.preferred_xtra_age) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.preferred_xtra_age = CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_SEND_NO_FIX_REPORT,
                              sizeof(uint8), &tm_core_info.config_info.send_no_fix_report) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.send_no_fix_report = GNSS_NV_EFS_SM_SEND_NO_FIX_REPORT_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_CONFIG_QMI_SERVICE_EXTERNAL,
                              sizeof(uint32), &tm_core_info.config_info.qmi_cfg.enableMask) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.qmi_cfg.enableMask = GNSS_NV_EFS_CONFIG_QMI_SERVICE_EXTERNAL_DEFAULT;
  }

  /* All premium services in disabled state */
  memset(tm_core_info.config_info.premium_services_config,
         PDSM_PA_PREMIUM_SVC_CFG_DISABLED,
         sizeof(tm_core_info.config_info.premium_services_config));
  /* But read premium service SAP config from NV */
  if (sm_nv_efs_reg_item_read(
        GNSS_NV_EFS_SM_CONFIG_PREMIUM_SERVICES,
        sizeof(uint32),
        &tm_core_info.config_info.premium_services_config[PDSM_PA_PREMIUM_SVC_SAP]) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.premium_services_config[PDSM_PA_PREMIUM_SVC_SAP] = GNSS_NV_EFS_SM_CONFIG_SAP_PREMIUM_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS,
                              sizeof(uint8), &tm_core_info.config_info.gnss_lpp_cp_timeout_secs) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gnss_lpp_cp_timeout_secs = GNSS_NV_EFS_SM_TM_LPP_CP_TRANSPORT_TIMEOUT_SECS_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TM_EMERGENCY_PROTOCOL_SELECT,
                              sizeof(uint32), &tm_core_info.config_info.gnss_emergency_protocol_non_lte) == FALSE)
  {
    /* if returned false for any reason, force it to default value which is CP*/
    tm_core_info.config_info.gnss_emergency_protocol_non_lte = 0;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_GNSS_UNAVAIL_IND_TIMEOUT,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.sm_gm_gnss_unavail_ind_timeout) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_gnss_unavail_ind_timeout = GNSS_NV_EFS_SM_GM_GNSS_UNAVAIL_IND_TIMEOUT_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_MOTION_DETECTION_SOURCES,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.sm_gm_motion_detection_sources) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_motion_detection_sources = GNSS_NV_EFS_SM_GM_MOTION_DETECTION_SOURCES_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_POSITION_SOURCES,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.sm_gm_position_sources) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_position_sources = GNSS_NV_EFS_SM_GM_POSITION_SOURCES_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_GNSS_POS_QOS_SESS_TIMEOUT,
                              sizeof(uint8), &tm_core_info.config_info.gm_config.sm_gm_gnss_pos_qos_sess_timeout) == FALSE)
  {
    /* if returned false for any reason, force it to default value which is GNSS_NV_EFS_SM_GM_GNSS_POS_QOS_SESS_TIMEOUT_DEFAULT*/
    tm_core_info.config_info.gm_config.sm_gm_gnss_pos_qos_sess_timeout = GNSS_NV_EFS_SM_GM_GNSS_POS_QOS_SESS_TIMEOUT_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TM_WIFI_WAIT_TIMEOUT_SEC,
                              sizeof(uint8), &tm_core_info.config_info.wifi_wait_timeout_sec) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.wifi_wait_timeout_sec = GNSS_NV_EFS_SM_TM_WIFI_WAIT_TIMEOUT_SEC_DEFAULT;
  }
  else if (tm_core_info.config_info.wifi_wait_timeout_sec > TM_WIFI_WAIT_TIMEOUT_SEC_MAX)
  {
    tm_core_info.config_info.wifi_wait_timeout_sec = TM_WIFI_WAIT_TIMEOUT_SEC_MAX;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_GNSS_MAX_POS_UNC_ACCEPTED,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.sm_gm_gnss_max_pos_unc_accepted) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_gnss_max_pos_unc_accepted = GNSS_NV_EFS_SM_GM_GNSS_MAX_POS_UNC_ACCEPTED_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_USE_NW_ASST_FIXES,
                              sizeof(uint8), &tm_core_info.config_info.gm_config.sm_gm_use_nw_asst_fixes) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_use_nw_asst_fixes = GNSS_NV_EFS_SM_GM_USE_NW_ASST_FIXES_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_HIGH_RESPONSIVENESS_CONFIG,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.sm_gm_high_responsiveness_config) == FALSE)
  {
    /* if returned false for any reason, force it to default value which is GNSS_NV_EFS_SM_GM_GNSS_POS_QOS_SESS_TIMEOUT_DEFAULT*/
    tm_core_info.config_info.gm_config.sm_gm_high_responsiveness_config = GNSS_NV_EFS_SM_GM_HIGH_RESPONSIVENESS_CONFIG_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_ALLOW_MO_SUPL_DURING_CONCURRENCY,
                              sizeof(uint32), &tm_core_info.config_info.q_allow_mo_supl_during_concurrency) == FALSE)
  {
    tm_core_info.config_info.q_allow_mo_supl_during_concurrency = GNSS_NV_EFS_SM_ALLOW_MO_SUPL_DURING_CONCURRENCY_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_CPI_REQUEST_RATE,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.sm_gm_cpi_request_rate) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_cpi_request_rate = GNSS_NV_EFS_SM_GM_CPI_REQUEST_RATE_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_MED_RESP_BACKOFF,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.sm_gm_med_resp_backoff) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_med_resp_backoff = GNSS_NV_EFS_SM_GM_MED_RESP_BACKOFF_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_CONFIG,
                              sizeof(gm_chal_gps_env_backoff_config_s_type), &tm_core_info.config_info.gm_config.sm_gm_chal_gps_env_bo_config) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_chal_gps_env_bo_config.q_backoff_min = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_MIN_DEFAULT;
    tm_core_info.config_info.gm_config.sm_gm_chal_gps_env_bo_config.q_backoff_max = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_BO_MAX_DEFAULT;

  }
  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_CONFIG,
                              sizeof(gm_chal_gps_env_ms_dist_config_s_type),
                              &tm_core_info.config_info.gm_config.sm_gm_chal_gps_env_ms_dist_config) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_chal_gps_env_ms_dist_config.q_dist_used_for_gps = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_GPS_DEFAULT;
    tm_core_info.config_info.gm_config.sm_gm_chal_gps_env_ms_dist_config.q_dist_used_for_cpi = GNSS_NV_EFS_SM_GM_CHAL_GPS_ENV_MS_DIST_CPI_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_MS_SPEED_CONFIG,
                              sizeof(gm_ms_speed_config_s_type),
                              &tm_core_info.config_info.gm_config.sm_gm_motion_state_speed_config) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_motion_state_speed_config.gm_ms_walk_speed = GNSS_NV_EFS_SM_GM_MS_WALK_SPEED_DEFAULT;
    tm_core_info.config_info.gm_config.sm_gm_motion_state_speed_config.gm_ms_run_speed = GNSS_NV_EFS_SM_GM_MS_RUN_SPEED_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(
        GNSS_NV_EFS_SM_GM_IOD_CONFIG_SWITCHES_MASK,
        sizeof(tm_core_info.config_info.gm_config.q_sm_gm_iod_config_switch_mask),
        &tm_core_info.config_info.gm_config.q_sm_gm_iod_config_switch_mask) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.q_sm_gm_iod_config_switch_mask = GNSS_NV_EFS_SM_GM_IOD_CONFIG_SWITCHES_MASK_DEFAULT;
    tm_core_info.config_info.gm_config.q_sm_gm_iod_config_switch_mask = 5;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_COMP_LOGGING_RATE,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.q_gm_comp_logging_rate) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.q_gm_comp_logging_rate = GNSS_NV_EFS_SM_GM_COMP_LOGGING_RATE_DEFAULT;
  }

  /* Ensure the QMI reporting is read. Location Feature Manager reads this NV and provides the
     Value via tm_core_lfm_config_update() for SV Poly reporting*/
  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TM_MEAS_QMI_REPORT_CONTROL,
                              sizeof(uint8), &tm_core_info.config_info.gnss_meas_rep_qmi_out_control) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gnss_meas_rep_qmi_out_control = GNSS_NV_EFS_SM_TM_MEAS_QMI_REPORT_DEFAULT;
    if (tm_core_info.config_info.gnss_meas_rep_qmi_out_control & SM_GNSS_BMAP_QMI_MEAS_REPORTING)
    {
      tm_core_info.config_info.t_gnss_reporting_config |= PDSM_GNSS_MEASUREMENT_REPORT;
    }
    if (tm_core_info.config_info.gnss_meas_rep_qmi_out_control & SM_GNSS_BMAP_QMI_SV_POLY_REPORTING)
    {
      tm_core_info.config_info.t_gnss_reporting_config |= PDSM_GNSS_SV_INFORMATION_REPORT;
    }
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_LOWI_REGISTER_ENABLE,
                              sizeof(boolean), &tm_core_info.config_info.gm_config.sm_gm_lowi_reg_enable) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_lowi_reg_enable = GNSS_NV_EFS_SM_GM_LOWI_REG_ENABLE_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_LOWI_SUBSCRIPTION_MASK,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.sm_gm_lowi_sub_mask) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_lowi_sub_mask = GNSS_NV_EFS_SM_GM_LOWI_SUB_MASK_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_LOWI_REG_CONFIG,
                              sizeof(tm_core_info.config_info.gm_config.sm_gm_lowi_reg_config),
                              &tm_core_info.config_info.gm_config.sm_gm_lowi_reg_config) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.sm_gm_lowi_reg_config.q_enable_reg_timer_mask = GNSS_NV_EFS_SM_GM_ENABLE_TIMER_MASK_DEFAULT;
    tm_core_info.config_info.gm_config.sm_gm_lowi_reg_config.q_short_term_timer_max_retry_cnt = GNSS_NV_EFS_SM_GM_SHORT_TERM_TIMER_MAX_RETRY_CNT_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_FILTER_WLAN_FREQ_MASK,
                              sizeof(uint32), &tm_core_info.config_info.gm_config.q_sm_gm_filter_wlan_freq_mask) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gm_config.q_sm_gm_filter_wlan_freq_mask = GNSS_NV_EFS_SM_GM_FILTER_WLAN_FREQ_MASK_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CONFIG,
                              sizeof(uint32), &tm_core_info.config_info.gnss_sm_emergency_support_config) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gnss_sm_emergency_support_config = GNSS_NV_EFS_SM_TM_EMERGENCY_SUPPORT_CONFIG_DEFAULT;
  }

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY,
                              sizeof(tm_config_agnss_transmission_delay_s_type),
                              &tm_core_info.config_info.agnss_trans_delay_config) == FALSE)
  {
    for (k = 0; k < GNSS_NV_TM_TRANS_DELAY_MAX_INDEX; k++)
    {
      for (j = 0; j < GNSS_NV_TM_TRANS_DELAY_MAX_PLANE; j++)
      {
        tm_core_info.config_info.agnss_trans_delay_config.trans_delay[k][j] = GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY_DEFAULT;
      }
    }
  }
  /* basic sanity to make sure protocol transmission delay is within range */
  for (k = 0; k < GNSS_NV_TM_TRANS_DELAY_MAX_INDEX; k++)
  {
    for (j = 0; j < GNSS_NV_TM_TRANS_DELAY_MAX_PLANE; j++)
    {
      if (tm_core_info.config_info.agnss_trans_delay_config.trans_delay[k][j] >
             GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY_MAX)
      {
        tm_core_info.config_info.agnss_trans_delay_config.trans_delay[k][j] =
           GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY_DEFAULT;
      }
    }
  }

  {
    uint32 tech_sel_lppm_nv_config = GNSS_NV_EFS_SM_TECH_SEL_LPPM_CONFIG_DEFAULT;
    if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TECH_SEL_LPPM_CONFIG,
                                sizeof(uint32), &tech_sel_lppm_nv_config) == FALSE)
    {
      /* if returned false for any reason, force it to default value */
      tech_sel_lppm_nv_config = GNSS_NV_EFS_SM_TECH_SEL_LPPM_CONFIG_DEFAULT;
    }
    techSel_handleLppmNvConfig(tech_sel_lppm_nv_config);
  }

  {
    uint8 tech_sel_lppm_nv_tbm_profile = GNSS_NV_EFS_SM_TECH_SEL_LPPM_TBM_PROFILE_DEFAULT;
    if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TECH_SEL_LPPM_TBM_PROFILE,
                                sizeof(uint8), &tech_sel_lppm_nv_tbm_profile) == FALSE)
    {
      /* if returned false for any reason, force it to default value */
      tech_sel_lppm_nv_tbm_profile = GNSS_NV_EFS_SM_TECH_SEL_LPPM_TBM_PROFILE_DEFAULT;
    }
    techSel_handleLppmNvTbmProfile(tech_sel_lppm_nv_tbm_profile);
  }

  {
    uint32 tech_sel_wifi_assit_nv_config = GNSS_NV_EFS_SM_TECH_SEL_WIFI_ASSIST_CONFIG_DEFAULT;
    if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TECH_SEL_WIFI_ASSIST_CONFIG,
                                sizeof(uint32), &tech_sel_wifi_assit_nv_config) == FALSE)
    {
      /* if returned false for any reason, force it to default value */
      tech_sel_wifi_assit_nv_config = GNSS_NV_EFS_SM_TECH_SEL_WIFI_ASSIST_CONFIG_DEFAULT;
    }
    techSel_handleWifiAssistNvConfig(tech_sel_wifi_assit_nv_config);
  }

  {	
    if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TECH_SEL_ODCPI_TIMERS,
                                sizeof(tech_sel_odcpi_timers_s_type), &tech_sel_odcpi_timers) == FALSE)
    {
      /* if returned false for any reason, force it to default value */
      tech_sel_odcpi_timers.q_posWaittimer    = GNSS_NV_TECHSEL_WIFI_POS_WAIT_MSECS_DEFAULT; 
      tech_sel_odcpi_timers.q_weakHysttimer   = GNSS_NV_TECHSEL_WIFI_WEAK_HYST_MSECS_DEFAULT;
      tech_sel_odcpi_timers.q_unknowHysttimer = GNSS_NV_TECHSEL_WIFI_UNKNOWN_HYST_MSECS_DEFAULT;
      tech_sel_odcpi_timers.q_odcpiBackOfftimer   = GNSS_NV_TECHSEL_ODCPI_BACKOFF_MSECS_DEFAULT;
    }
    techSel_updateOdcpiTimerConfigFromNv(&tech_sel_odcpi_timers);
  }

#ifdef FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM
  #error code not present
#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */

  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_CONCURRENT_SESSION_SUPPORT_CONFIG,
                              sizeof(uint32), &tm_core_info.config_info.gnss_sm_concurrent_sessions_support_config) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gnss_sm_concurrent_sessions_support_config = GNSS_NV_EFS_SM_CONCURRENT_SESSION_SUPPORT_CONFIG_DEFAULT;
  }

  /* Read the GNSS NHz Config */
  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_NHZ_CONFIG,
                              sizeof(uint32), &tm_core_info.config_info.gnss_nhz_config) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    tm_core_info.config_info.gnss_nhz_config = GNSS_NV_EFS_SM_NHZ_CONFIG_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_FUSION_CSM_DATA_UPLOAD_THRESHOLD, sizeof(uint16),
                                       &tm_core_info.config_info.fusion_csm_config.w_sm_fusion_csm_data_upload_threshold_mins))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.fusion_csm_config.w_sm_fusion_csm_data_upload_threshold_mins =
       GNSS_NV_EFS_SM_FUSION_CSM_DATA_UPLOAD_THRESHOLD_MINS_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_FUSION_CSM_SB_BUFFER_SIZE, sizeof(uint16),
                                       &tm_core_info.config_info.fusion_csm_config.w_sm_fusion_csm_sb_buffer_size))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.fusion_csm_config.w_sm_fusion_csm_sb_buffer_size =
       GNSS_NV_EFS_SM_FUSION_CSM_SB_BUFFER_SIZE_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_FUSION_CSM_MAX_SSID_SUPPORTED, sizeof(uint16),
                                       &tm_core_info.config_info.fusion_csm_config.w_sm_fusion_csm_max_ssid_supported))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.fusion_csm_config.w_sm_fusion_csm_max_ssid_supported =
       GNSS_NV_EFS_SM_FUSION_CSM_MAX_SSID_SUPPORTED_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_FUSION_CSM_VENUE_TRACKING_RESP_SEC, sizeof(uint16),
                                       &tm_core_info.config_info.fusion_csm_config.w_sm_fusion_csm_venue_tracking_resp_sec))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.fusion_csm_config.w_sm_fusion_csm_venue_tracking_resp_sec =
       GNSS_NV_EFS_SM_FUSION_CSM_VENUE_TRACKING_RESP_SEC_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_FUSION_CSM_OPERATION_MODE, sizeof(uint32),
                                       &tm_core_info.config_info.fusion_csm_config.u_OperationMode))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.fusion_csm_config.u_OperationMode =
       GNSS_NV_EFS_SM_FUSION_CSM_OPERATION_MODE_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_FUSION_BARO_WEIGHT, sizeof(uint8),
                                       &tm_core_info.config_info.fusion_csm_config.u_BaroFilterWeight))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.fusion_csm_config.u_BaroFilterWeight =
       GNSS_NV_EFS_SM_FUSION_CSM_BARO_FILTER_WEIGHT_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_FUSION_BARO_SAMPLE_RATE_HZ, sizeof(uint8),
                                       &tm_core_info.config_info.fusion_csm_config.u_BaroSampleRateHz))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.fusion_csm_config.u_BaroSampleRateHz =
       GNSS_NV_EFS_SM_FUSION_BARO_SAMPLE_RATE_HZ_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TEST_CONTROL, sizeof(uint32),
                                       &tm_core_info.config_info.fusion_csm_config.q_TestControlMask))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.fusion_csm_config.q_TestControlMask =
       GNSS_NV_EFS_SM_TEST_CONTROL_DEFAULT_MASK;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_GM_DRIVE_TO_PED_PREDICTOR_ENABLE,
                                       sizeof(boolean), &tm_core_info.config_info.gm_config.sm_gm_drive_to_ped_predictor_enable))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.gm_config.sm_gm_drive_to_ped_predictor_enable = GNSS_NV_EFS_SM_GM_DRIVE_TO_PED_PREDICTOR_ENABLE_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_DISABLE_DYN_HEPE_FOR_BETTER_TTFF,
                                       sizeof(boolean), &tm_core_info.config_info.u_disable_Dynamic_HEPE_usage))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.u_disable_Dynamic_HEPE_usage = GNSS_NV_EFS_DYN_HEPE_DISABLED_FOR_BETTER_TTFF_DEFAULT;
  }

  if (FALSE == sm_nv_efs_reg_item_read(GNSS_NV_EFS_INTEGER_SECOND_BOUNDARY_ALIGNED_NMEA_REPORT,
                                       sizeof(boolean), &tm_core_info.config_info.u_gnss_second_boundary_aligned_NMEA))
  {
    /*If NV read fails, force it to default*/
    tm_core_info.config_info.u_gnss_second_boundary_aligned_NMEA = GNSS_NV_EFS_SM_SECOND_BOUNDARY_ALIGNED_NMEA_DEFAULT;
  }
}

/*===========================================================================

FUNCTION tm_core_refresh_nv_notify_protocols

DESCRIPTION
  This function notifies protocols to refresh carrier-specific NVs (mainly 
  for SIM hot swap and DD change).

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
static void tm_core_refresh_nv_notify_protocols(sys_modem_as_id_e_type sub, bool read_from_hw)
{
  prtl_event_u_type  nv_refresh_event_payload;
  int prtlTableIndex = -1;

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Sending NV Refresh to protcolos for sub %d Read_from_hw %d",
        sub, read_from_hw);

  /*Send the PRTL_EVENT_TYPE_CARRIER_NV_REFRESH for SUPL module*/
  nv_refresh_event_payload.nv_refresh_payload.read_from_hw = read_from_hw;
  nv_refresh_event_payload.nv_refresh_payload.sub = sub;

  prtlTableIndex = tm_core_get_prtl_idx_by_type(TM_PRTL_TYPE_UMTS_UP);
  if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
  {
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp != NULL)
    {
      (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp(PRTL_EVENT_TYPE_CARRIER_NV_REFRESH,
                                                                        &nv_refresh_event_payload);
    }
  }

  /*Send the PRTL_EVENT_TYPE_CARRIER_NV_REFRESH for LPP CP module*/
  prtlTableIndex = tm_core_get_prtl_idx_by_type(TM_PRTL_TYPE_LPP_CP);
  if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
  {
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp != NULL)
    {
      (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp(PRTL_EVENT_TYPE_CARRIER_NV_REFRESH,
                                                                        &nv_refresh_event_payload);
    }
  }

  /*Send the PRTL_EVENT_TYPE_CARRIER_NV_REFRESH for 1X UP module*/
  prtlTableIndex = tm_core_get_prtl_idx_by_type(TM_PRTL_TYPE_1X_UP);
  if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
  {
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp != NULL)
    {
      (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp(PRTL_EVENT_TYPE_CARRIER_NV_REFRESH,
                                                                        &nv_refresh_event_payload);
    }
  }

  }


/*===========================================================================

FUNCTION tm_core_refresh_carrier_specific_nv

DESCRIPTION
  This function refresh carrier-specific NVs (mainly for SIM hot swap) for a
  specific subscription (passed in p_data)

  sm_nv_read() will always read from HW and save in cache. 
  sm_nv_read_ex() may read from cache if available.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_refresh_carrier_specific_nv(void *p_data)
{
  int i, j;

#ifdef FEATURE_MODEM_CONFIG_REFRESH
  sys_modem_as_id_e_type sub;

  if (p_data == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NV Refresh NULL ptr");
    return;
  }

  /* only parameter is uint8 sub for now (0~2) */
  sub = (sys_modem_as_id_e_type)(*((uint8 *)p_data));

  /* read carrier-specific NV & EFS NVs only for one SUB */
  tm_core_read_carrier_specific_nv(sub);

  tm_core_read_carrier_specific_efs_nv(sub);

  /* if current DD sub is being refreshed, protocols need to be notified as well */
  if (sub == tm_current_dd_subs_id)
  {
    tm_core_refresh_nv_notify_protocols(sub, TRUE);
  }

  /* notify MCFG that NV refresh is done, each sub will be refreshed one by one */
  tm_ruim_carrier_nv_refresh_done(sub);

#endif /* FEATURE_MODEM_CONFIG_REFRESH */


  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY,
                              sizeof(tm_config_agnss_transmission_delay_s_type),
                              &tm_core_info.config_info.agnss_trans_delay_config) == FALSE)
  {
    for (i = 0; i < GNSS_NV_TM_TRANS_DELAY_MAX_INDEX; i++)
    {
      for (j = 0; j < GNSS_NV_TM_TRANS_DELAY_MAX_PLANE; j++)
      {
        tm_core_info.config_info.agnss_trans_delay_config.trans_delay[i][j] = GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY_DEFAULT;
      }
    }
  }
  /* basic sanity to make sure protocol transmission delay is within range */
  for (i = 0; i < GNSS_NV_TM_TRANS_DELAY_MAX_INDEX; i++)
  {
    for (j = 0; j < GNSS_NV_TM_TRANS_DELAY_MAX_PLANE; j++)
    {
      if (tm_core_info.config_info.agnss_trans_delay_config.trans_delay[i][j] >
             GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY_MAX)
      {
        tm_core_info.config_info.agnss_trans_delay_config.trans_delay[i][j] =
           GNSS_NV_EFS_SM_TM_CONFIG_AGNSS_TRANSMISSION_DELAY_DEFAULT;
      }
    }
  }

#ifdef FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM
  #error code not present
#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */

}

/*===========================================================================

FUNCTION tm_core_read_new_subs_imsi

DESCRIPTION
  This function notifies SUPL of new ready sub. So that it can read the new
  subs IMSI

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_read_new_subs_imsi(void)
{
  prtl_event_u_type  read_new_imsi_payload = { 0 };
  int prtlTableIndex = -1;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Sending read new imsi event to SUPL for DD sub");

  /*Send the PRTL_EVENT_TYPE_READ_NEW_IMSI for SUPL module*/
  read_new_imsi_payload.read_new_imsi_payload.v_read_new_imsi = TRUE;
  prtlTableIndex = tm_core_get_prtl_idx_by_type(TM_PRTL_TYPE_UMTS_UP);
  if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
  {
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp != NULL)
    {
      (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp(PRTL_EVENT_TYPE_READ_NEW_IMSI,
                                                                        &read_new_imsi_payload);
    }
  }
}

#ifdef FEATURE_UIM_SUPPORT_LBS
/*===========================================================================

FUNCTION tm_core_ruim_read

DESCRIPTION
  This function read NV items to set TM-Core configuration info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_ruim_read(void)
{
  /* Smart MSB */
  (void)tm_core_nv_async_read(NV_GPS1_DYNAMIC_MODE_I, (void *)&tm_nv_read_buffer);
  /* Read the available Position Options.     */
  (void)tm_core_nv_async_read(NV_GPS1_SEEDPOS_OPTION_I, (void *)&tm_nv_read_buffer);
}
/*===========================================================================

FUNCTION tm_core_ruim_read

DESCRIPTION
  This function is called  to set TM-Core configuration info structure, after
  Async NV read for NV items is complete

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_ruim_read_complete(tm_core_ruim_read_buffer *tm_core_ruim_buff)
{
  uint16 w_CellDBPosOption = 0xFFFF;
  /* Smart MSB */
  tm_core_info.config_info.smart_msb = (boolean)tm_core_ruim_buff->dynamic_mode;
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "SMART MSB %d", (int)tm_core_info.config_info.smart_msb, 0, 0);
  /* Read the available Position Options.     */
  if (tm_core_ruim_buff->seed_pos_options)
  {
    w_CellDBPosOption = (uint16)tm_core_ruim_buff->seed_pos_options;
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Seed POSITION optiond %d", (int)w_CellDBPosOption, 0, 0);
  }
  mgp_SeedPosOptionInfoUpdate(w_CellDBPosOption);
}

#endif /* FEATURE_UIM_SUPPORT_LBS */


/*===========================================================================

FUNCTION tm_core_get_gps_lock_state

DESCRIPTION
  This function determines if gps is locked

RETURN VALUE
  TRUE if gps is locked, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_gps_lock_state(void)
{
  if (TM_CORE_CONFIG_DD(gps_lock) != PDSM_GPS_LOCK_NONE)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}


/*===========================================================================

FUNCTION tm_core_is_internal_session_on

DESCRIPTION
  This function determines if internal session is ongoing

RETURN VALUE
  TRUE if internal session is running, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_internal_session_on(void)
{
  if (tm_core_info.internal_session)
  {
    return TRUE;
  }
  else
    return FALSE;
}

/*===========================================================================

FUNCTION tm_core_is_periodic_ppm_session_on

DESCRIPTION
  This function determines if periodic PPM session is ongoing

RETURN VALUE
  TRUE if periodic PPM is running, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_periodic_ppm_session_on(void)
{
#ifdef FEATURE_GNSS_PERIODIC_PPM
  #error code not present
#else
  return FALSE;
#endif
}

/*===========================================================================

FUNCTION tm_core_get_periodic_ppm_session_handle

DESCRIPTION
  This function provides the special P.PPM session handle.

RETURN VALUE
  TM_SESS_HANDLE_NONE         : If P.PPM is not ON.
  TM_SESS_HANDLE_PERIODIC_PPM : Periodic PPM Session handle if P.PPM is on


  if P.PPM is OFF.
DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
tm_sess_handle_type tm_core_get_periodic_ppm_session_handle(void)
{
  if (tm_core_is_periodic_ppm_session_on())
  {
    return TM_SESS_HANDLE_PERIODIC_PPM;
  }
  else
  {
    return TM_SESS_HANDLE_NONE;
  }
}


#ifdef FEATURE_TM_SECURITY

/*===========================================================================

FUNCTION tm_core_read_security_setting

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_read_security_setting(void)
{
  tm_sec_enable_data_s_type  sec_enable_data;

  /******************************************
     Enable/Disable Security based on NV
  ******************************************/
  if (sm_nv_read(NV_GPSONE_PASSWORD_I, (void *)&tm_nv_read_buffer) == TRUE)
  {
    /* If Security NV has been written, Always enable security feature */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Enabling secure PD API layer ", 0, 0, 0);

    sec_enable_data.base_ssd.ssd_length = (uint8)tm_nv_read_buffer.gpsone_password.password_len;

    /* If length is not valid, tm_sec_enable will create random SSDs */
    if ((sec_enable_data.base_ssd.ssd_length > 0)
        && (sec_enable_data.base_ssd.ssd_length <= PDSM_MAX_SSD_LEN))
    {
      memscpy((void *)sec_enable_data.base_ssd.ssd_data,
              sizeof(sec_enable_data.base_ssd.ssd_data),
              (void *)tm_nv_read_buffer.gpsone_password.password,
              sec_enable_data.base_ssd.ssd_length);
    }

    /* Read the Random number update/challenge rate */
    (void)sm_nv_read(NV_GPS1_SEC_UPDATE_RATE_I, (void *)&tm_nv_read_buffer);
    sec_enable_data.rand_update_rate.rate = tm_nv_read_buffer.gps1_sec_update_rate;

    sec_enable_data.data_validity_mask = TM_SEC_DATA_VALIDITY_SSD | TM_SEC_DATA_VALIDITY_RAND;

    tm_sec_enable(&sec_enable_data, GPSONE_SEC_FLAG);
  }
  else
  {
    /* If Security NV has never been written, disable security feature */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Disabling secure PD API layer ", 0, 0, 0);
  }
}
#endif /* FEATURE_TM_SECURITY */


#ifdef FEATURE_CGPS_XTRA


/*===========================================================================
FUNCTION tm_core_is_xtra_operation_allowed

DESCRIPTION
Check if assisted mode GPS session are in progress.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
boolean tm_core_is_xtra_operation_allowed
(
   void
   )
{
  /* Local Variable */
  boolean           ret_val = FALSE;

  /* Allow XTRA data or time dload during idle, standalone, or diag sessions */
  if (tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_NONE
      || tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_STANDALONE
      || tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_DIAG
      )
  {
    ret_val = TRUE;
  }
  else if (tm_core_info.session_info.msb_xtra_allowed == TRUE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "XTRA is allowed for MSB AGPS or after MSB DLOAD comm failue", 0, 0, 0);
    ret_val = TRUE;
  }
  /* Else if the current session is 1x UP, 1x CP, UMTS UP, and UMTS CP */
  else if (tm_core_info.session_info.session_state == TM_CORE_SESS_STATE_INIT)
  {
    ret_val = TRUE;
  }
  else if (
#ifdef  FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
           (TRUE == tm_umts_common_utils_cm_emergency_call_status()) ||
#endif //FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
           (TRUE == tm_agps_emergency_call_simulate())
           )
  {
    ret_val = TRUE;
  }

  return ret_val;
} /* End tm_core_is_xtra_operation_allowed()*/


/*===========================================================================

FUNCTION tm_xtra_init_and_read_nv

DESCRIPTION
  This function reads NV items to set TM-XTRA configuration info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_xtra_init_and_read_nv
(
   void
   )
{
  /* Local Variables */
  tm_xtra_state_type *xtra_state_ptr;
  nv_item_type       nv_write_buffer;

  /*----------------------------------------------------------------------
     Read all XTRA NV items
  ----------------------------------------------------------------------*/

  /* Enable/Disable XTRA module */
  (void)sm_nv_read(NV_GPS1_XTRA_ENABLED_I, (void *)&tm_nv_read_buffer);

#ifdef __GRIFFON__
  tm_nv_read_buffer.gps1_xtra_enabled = TRUE;
#endif  // __GRIFFON__

  if ((boolean)tm_nv_read_buffer.gps1_xtra_enabled)
  {
    /*Dynamically allocate memory for XTRA state pointer*/
    if (!tm_xtra_init_state_ptr())
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Dynamic memory allocation for XTRA failed. Disabling XTRA feature", 0, 0, 0);
      nv_write_buffer.gps1_xtra_enabled = 0;
      (void)tm_core_nv_write(NV_GPS1_XTRA_ENABLED_I, (void *)&nv_write_buffer);

      return;
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "XTRA feature disabled", 0, 0, 0);
    return;
  }

  xtra_state_ptr = tm_xtra_get_state_ptr();

  if (xtra_state_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "XTRA STATE PTR found NULL", 0, 0, 0);
    return;
  }

  xtra_state_ptr->xtra_enabled = (boolean)tm_nv_read_buffer.gps1_xtra_enabled;

  /* Number of hours between automatic downloads */
  (void)sm_nv_read(NV_GPS1_XTRA_DOWNLOAD_INTERVAL_I, (void *)&tm_nv_read_buffer);
  xtra_state_ptr->download_interval
     = (uint16)tm_nv_read_buffer.gps1_xtra_download_interval;

  /* Number of unsuccessful download attempts before aborting (xtra client) */
  (void)sm_nv_read(NV_GPS1_XTRA_NUM_DOWNLOAD_ATTEMPTS_I, (void *)&tm_nv_read_buffer);
  xtra_state_ptr->num_download_attempts
     = (uint8)tm_nv_read_buffer.gps1_xtra_num_download_attempts;

  /* Number of minutes between download attempts */
  (void)sm_nv_read(NV_GPS1_XTRA_TIME_BETWEEN_ATTEMPTS_I, (void *)&tm_nv_read_buffer);
  xtra_state_ptr->time_between_attempts
     = (uint8)tm_nv_read_buffer.gps1_xtra_time_between_attempts;

  /* Enable/disable automatic periodic downloads of gpsOneXTRA files */
  (void)sm_nv_read(NV_GPS1_XTRA_AUTO_DOWNLOAD_ENABLED_I, (void *)&tm_nv_read_buffer);
  xtra_state_ptr->auto_download_enabled
     = (boolean)tm_nv_read_buffer.gps1_xtra_auto_download_enabled;

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
  /* Primary XTRA Server URL */
  (void)sm_nv_read(NV_GPS1_XTRA_PRIMARY_SERVER_URL_I, (void *)&tm_nv_read_buffer);
  memscpy((void *)&xtra_state_ptr->primary_data_server_url[0],
          sizeof(xtra_state_ptr->primary_data_server_url),
          (const void *)&tm_nv_read_buffer.gps1_xtra_primary_server_url[0],
          sizeof(tm_nv_read_buffer.gps1_xtra_primary_server_url));

  /* Secondary XTRA Server URL */
  (void)sm_nv_read(NV_GPS1_XTRA_SECONDARY_SERVER_URL_I, (void *)&tm_nv_read_buffer);
  memscpy((void *)&xtra_state_ptr->secondary_data_server_url[0],
          sizeof(xtra_state_ptr->secondary_data_server_url),
          (const void *)&tm_nv_read_buffer.gps1_xtra_secondary_server_url[0],
          sizeof(tm_nv_read_buffer.gps1_xtra_secondary_server_url));

  /* Tertiary gpsOneXTRA Server URL */
  (void)sm_nv_read(NV_GPS1_XTRA_TERTIARY_SERVER_URL_I, (void *)&tm_nv_read_buffer);
  memscpy((void *)&xtra_state_ptr->tertiary_data_server_url[0],
          sizeof(xtra_state_ptr->tertiary_data_server_url),
          (const void *)&tm_nv_read_buffer.gps1_xtra_tertiary_server_url[0],
          sizeof(tm_nv_read_buffer.gps1_xtra_tertiary_server_url));
#else
  /* Primary XTRA Server URL */
  memscpy((void *)&xtra_state_ptr->primary_data_server_url[0],
          sizeof(xtra_state_ptr->primary_data_server_url),
          (const void *)NV_AAGPS_DEFAULT_XTRA_PRIMARY_SERVER_URL,
          sizeof(NV_AAGPS_DEFAULT_XTRA_PRIMARY_SERVER_URL));

  /* Secondary XTRA Server URL */
  memscpy((void *)&xtra_state_ptr->secondary_data_server_url[0],
          sizeof(xtra_state_ptr->secondary_data_server_url),
          (const void *)NV_AAGPS_DEFAULT_XTRA_SECONDARY_SERVER_URL,
          sizeof(NV_AAGPS_DEFAULT_XTRA_SECONDARY_SERVER_URL));

  /* Tertiary gpsOneXTRA Server URL */
  memscpy((void *)&xtra_state_ptr->tertiary_data_server_url[0],
          sizeof(xtra_state_ptr->tertiary_data_server_url),
          (const void *)NV_AAGPS_DEFAULT_XTRA_TERTIARY_SERVER_URL,
          sizeof(NV_AAGPS_DEFAULT_XTRA_TERTIARY_SERVER_URL));
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */


  /*----------------------------------------------------------------------
     Read all XTRA time NV items
  ----------------------------------------------------------------------*/
  /* Enable/Disable XTRA time info */
  (void)sm_nv_read(NV_GPS1_XTRA_TIME_INFO_ENABLED_I, (void *)&tm_nv_read_buffer);
  xtra_state_ptr->time_info_enabled
     = (boolean)tm_nv_read_buffer.gps1_xtra_time_info_enabled;

  /* One-way delay failover threshold */
  (void)sm_nv_read(NV_GPS1_XTRA_TIME_INFO_DELAY_THRESH_I, (void *)&tm_nv_read_buffer);
  xtra_state_ptr->time_info_oneway_delay_thresh
     = (uint32)tm_nv_read_buffer.gps1_xtra_time_info_delay_thresh;

  /* time uncertainty threshold */
  (void)sm_nv_read(NV_GPS1_XTRA_TIME_INFO_UNC_THRESH_I, (void *)&tm_nv_read_buffer);
  xtra_state_ptr->time_info_uncertainty_thresh
     = (uint32)tm_nv_read_buffer.gps1_xtra_time_info_unc_thresh;

#ifdef FEATURE_INTERNAL_USE_FFABUILDS_ONLY
  /* Primary SNTP Server URL */
  (void)sm_nv_read(NV_GPS1_XTRA_PRIMARY_SNTP_SERVER_URL_I, (void *)&tm_nv_read_buffer);
  memscpy((void *)&xtra_state_ptr->primary_sntp_server_url[0],
          sizeof(xtra_state_ptr->primary_sntp_server_url),
          (const void *)&tm_nv_read_buffer.gps1_xtra_primary_sntp_server_url[0],
          sizeof(tm_nv_read_buffer.gps1_xtra_primary_sntp_server_url));

  /* Secondary SNTP Server URL */
  (void)sm_nv_read(NV_GPS1_XTRA_SECONDARY_SNTP_SERVER_URL_I, (void *)&tm_nv_read_buffer);
  memscpy((void *)&xtra_state_ptr->secondary_sntp_server_url[0],
          sizeof(xtra_state_ptr->secondary_sntp_server_url),
          (const void *)&tm_nv_read_buffer.gps1_xtra_secondary_sntp_server_url[0],
          sizeof(tm_nv_read_buffer.gps1_xtra_secondary_sntp_server_url));

  /* Tertiary SNTP Server URL */
  (void)sm_nv_read(NV_GPS1_XTRA_TERTIARY_SNTP_SERVER_URL_I, (void *)&tm_nv_read_buffer);
  memscpy((void *)&xtra_state_ptr->tertiary_sntp_server_url[0],
          sizeof(xtra_state_ptr->tertiary_sntp_server_url),
          (const void *)&tm_nv_read_buffer.gps1_xtra_tertiary_sntp_server_url[0],
          sizeof(tm_nv_read_buffer.gps1_xtra_tertiary_sntp_server_url));
#else
  /* Primary SNTP Server URL */
  memscpy((void *)&xtra_state_ptr->primary_sntp_server_url[0],
          sizeof(xtra_state_ptr->primary_sntp_server_url),
          (const void *)NV_AAGPS_DEFAULT_XTRA_PRIMARY_SNTP_SERVER_URL,
          sizeof(NV_AAGPS_DEFAULT_XTRA_PRIMARY_SNTP_SERVER_URL));

  /* Secondary SNTP Server URL */
  memscpy((void *)&xtra_state_ptr->secondary_sntp_server_url[0],
          sizeof(xtra_state_ptr->secondary_sntp_server_url),
          (const void *)NV_AAGPS_DEFAULT_XTRA_SECONDARY_SNTP_SERVER_URL,
          sizeof(NV_AAGPS_DEFAULT_XTRA_SECONDARY_SNTP_SERVER_URL));

  /* Tertiary SNTP Server URL */
  memscpy((void *)&xtra_state_ptr->tertiary_sntp_server_url[0],
          sizeof(xtra_state_ptr->tertiary_sntp_server_url),
          (const void *)NV_AAGPS_DEFAULT_XTRA_TERTIARY_SNTP_SERVER_URL,
          sizeof(NV_AAGPS_DEFAULT_XTRA_TERTIARY_SNTP_SERVER_URL));
#endif /* FEATURE_INTERNAL_USE_FFABUILDS_ONLY */

#ifdef FEATURE_GNSS_XTRA3
  /* XTRA3 last key index */
  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_XTRA_LAST_XTRA3_KEY_INDEX,
                              sizeof(uint16),
                              (void *)&xtra_state_ptr->xtra3_last_key_index) == FALSE)
  {
    /* if returned false for any reason, force it to default value */
    xtra_state_ptr->xtra3_last_key_index = GNSS_NV_EFS_SM_XTRA_LAST_XTRA3_KEY_INDEX_DEFAULT;
  }
#endif /* FEATURE_GNSS_XTRA3 */

  tm_xtra_init();
  /* UAX3 Config */
  xtra_state_ptr->uax3_config = GNSS_NV_EFS_SM_TM_UAX3_CONFIG_DEFAULT;
  if (sm_nv_efs_reg_item_read(GNSS_NV_EFS_SM_TM_UAX3_CONFIG,
                              sizeof(uint32),
                              (void *)&xtra_state_ptr->uax3_config) == FALSE)
  {

    TM_XTRA_MSG_ERROR("Failed to read UAX3  config %d from NV",
                      xtra_state_ptr->uax3_config, 0, 0);

  }
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "UAX3 Config value from NV = %u", xtra_state_ptr->uax3_config);
  /* send the value to MGP */
  mgp_UpdateUax3Config(xtra_state_ptr->uax3_config);

  /* Initialize XTRA module */

  return;
} /* End tm_xtra_read_nv() */

/*===========================================================================
FUNCTION tm_core_handle_xtra_download_req

DESCRIPTION
Handle the XTRA download request recieved from PE via LM

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
void tm_core_handle_xtra_download_req(const sm_ReqXtraStructType *pz_xtraAssistReq)
{
  if (NULL == pz_xtraAssistReq)
  {
    TM_XTRA_MSG_ERROR("NULL Ipc request received", 0, 0, 0);
    return;
  }

  if (SM_REQ_TYPE_GET_NAVIC_EPH_ASSIST != pz_xtraAssistReq->e_XtraType)
  {
    TM_XTRA_MSG_ERROR("Unhandled Xtra req type %d received ", pz_xtraAssistReq->e_XtraType, 0, 0);
    return;
  }

  tm_xtra3_navic_assist_handle_download_req(pz_xtraAssistReq);
}

#ifdef FEATURE_UIM_SUPPORT_LBS
/*===========================================================================

FUNCTION tm_xtra_ruim_init_and_read

DESCRIPTION
  This function reads NV items to set TM-XTRA configuration info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_xtra_ruim_init_and_read
(
   void
   )
{
  /*----------------------------------------------------------------------
     Read all XTRA NV items
  ----------------------------------------------------------------------*/

  /* Enable/Disable XTRA module */
  (void)tm_core_nv_async_read(NV_GPS1_XTRA_ENABLED_I, (void *)&tm_nv_read_buffer);

  /* Number of hours between automatic downloads */
  (void)tm_core_nv_async_read(NV_GPS1_XTRA_DOWNLOAD_INTERVAL_I, (void *)&tm_nv_read_buffer);


  /* Number of unsuccessful download attempts before aborting (xtra client) */
  (void)tm_core_nv_async_read(NV_GPS1_XTRA_NUM_DOWNLOAD_ATTEMPTS_I, (void *)&tm_nv_read_buffer);


  /* Number of minutes between download attempts */
  (void)tm_core_nv_async_read(NV_GPS1_XTRA_TIME_BETWEEN_ATTEMPTS_I, (void *)&tm_nv_read_buffer);


  /* Enable/disable automatic periodic downloads of gpsOneXTRA files */
  (void)tm_core_nv_async_read(NV_GPS1_XTRA_AUTO_DOWNLOAD_ENABLED_I, (void *)&tm_nv_read_buffer);


  /* Primary XTRA Server URL */
  (void)tm_core_nv_async_read(NV_GPS1_XTRA_PRIMARY_SERVER_URL_I, (void *)&tm_nv_read_buffer);


  /* Secondary XTRA Server URL */
  (void)tm_core_nv_async_read(NV_GPS1_XTRA_SECONDARY_SERVER_URL_I, (void *)&tm_nv_read_buffer);


  /* Tertiary gpsOneXTRA Server URL */
  (void)tm_core_nv_async_read(NV_GPS1_XTRA_TERTIARY_SERVER_URL_I, (void *)&tm_nv_read_buffer);

  return;
}
/*===========================================================================

FUNCTION tm_xtra_ruim_read_complete

DESCRIPTION
  This function indicates NV Read completion and set TM-XTRA configuration 
  info structure.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_xtra_ruim_read_complete(tm_xtra_ruim_nv_buffer *ruim_nv_buffer)
{

  tm_xtra_state_type *xtra_state_ptr;
  nv_item_type       nv_write_buffer;

  if (ruim_nv_buffer == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "XTRA RUIM NV Read from failed.", 0, 0, 0);
    return;
  }

  xtra_state_ptr = tm_xtra_get_state_ptr();

  if ((boolean)ruim_nv_buffer->xtra_enabled || xtra_state_ptr == NULL)
  {
    /*Dynamically allocate memory for XTRA state pointer*/
    if (!tm_xtra_init_state_ptr())
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Dynamic memory allocation for XTRA failed. Disabling XTRA feature", 0, 0, 0);
      nv_write_buffer.gps1_xtra_enabled = 0;
      (void)tm_core_nv_write(NV_GPS1_XTRA_ENABLED_I, (void *)&nv_write_buffer);

      return;
    }
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "XTRA FEATURE ENABLE/DISABLE: NV Async READ : %d", (int)ruim_nv_buffer->xtra_enabled, 0, 0);
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "XTRA feature disabled", 0, 0, 0);
    return;
  }

  xtra_state_ptr->xtra_enabled = (boolean)ruim_nv_buffer->xtra_enabled;

  /* Number of hours between automatic downloads */
  xtra_state_ptr->download_interval
     = (uint16)ruim_nv_buffer->download_interval;

  /* Number of unsuccessful download attempts before aborting (xtra client) */
  xtra_state_ptr->num_download_attempts
     = (uint8)ruim_nv_buffer->num_download_attempts;

  /* Number of minutes between download attempts */
  xtra_state_ptr->time_between_attempts
     = (uint8)ruim_nv_buffer->time_between_attempts;

  /* Enable/disable automatic periodic downloads of gpsOneXTRA files */
  xtra_state_ptr->auto_download_enabled
     = (boolean)ruim_nv_buffer->auto_download_enabled;

  /* Primary XTRA Server URL */
  memscpy((void *)&xtra_state_ptr->primary_data_server_url[0],
          sizeof(xtra_state_ptr->primary_data_server_url),
          (const void *)&(ruim_nv_buffer->xtra_server_primary[0]),
          sizeof(ruim_nv_buffer->xtra_server_primary));

  /* Secondary XTRA Server URL */
  memscpy((void *)&xtra_state_ptr->secondary_data_server_url[0],
          sizeof(xtra_state_ptr->secondary_data_server_url),
          (const void *)&(ruim_nv_buffer->xtra_server_secondary[0]),
          sizeof(ruim_nv_buffer->xtra_server_secondary));

  /* Tertiary gpsOneXTRA Server URL */
  memscpy((void *)&xtra_state_ptr->tertiary_data_server_url[0],
          sizeof(xtra_state_ptr->tertiary_data_server_url),
          (const void *)&(ruim_nv_buffer->xtra_server_tertiary[0]),
          sizeof(ruim_nv_buffer->xtra_server_tertiary));

  mgp_RuimXtraInit();

  /* Initialize XTRA module */
  tm_xtra_init();

  return;
}
#endif /* FEATURE_UIM_SUPPORT_LBS */
#endif /* FEATURE_CGPS_XTRA */



#ifdef FEATURE_GNSS_PERIODIC_PPM
#error code not present
#endif


/*===========================================================================

FUNCTION tm_core_set_current_subs_cfg

DESCRIPTION
  This function is used to se the currently active subscription-specific
  configuration for MultiSIM EFS NVs.

  Usually DD sub configuration, if phone is OOS, active configuration will be 
  the last used configuration

  This should be the only function within SM to update the currently active
  subscription-specific configuration when DD sub is changed

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void tm_core_set_current_subs_cfg(sys_modem_as_id_e_type dd_subs, boolean reConfig)
{
  if ((dd_subs < SM_CONFIG_MIN_SUB_ID) || (dd_subs > tm_core_info.config_info.max_valid_subs_id))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "DD sub %d out of range 0~%d (Keeping current %d)",
          dd_subs, tm_core_info.config_info.max_valid_subs_id, tm_core_info.config_info.active_subs_cfg);
  }
  else
  {
    if (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE)
    {
      // one active session is going on, print out warning
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "DD changed during active session (state=%d handle=0x%x DD=%d)",
            tm_core_info.session_info.session_state,
            tm_core_info.session_info.active_sess_handle, dd_subs);
    }

    /* within valid range 0~MAX_SUB-1 0~1/0~2, set lower layer NV Subs ID */
    sm_nv_set_current_sub_id(dd_subs);

    /* Let protocol refresh their own carrier-specific NVs for DD-sub since it's changed.
       If SUPL over nDDS feature is currently in action, ie, 911 i called on nDD SUB,
       then the protocols will refresh/update their NV's according to their internal logic.
       reConfig:
                TRUE  = NV REFRESH for DD Subscription event
                FALSE = 911 on nDDS scenario only update tm_cores active_subs_cfg value.*/

    if ((uint32)dd_subs != tm_core_info.config_info.active_subs_cfg)
    {
      tm_core_info.config_info.active_subs_cfg = (uint32)dd_subs;

        tm_core_refresh_nv_notify_protocols(dd_subs, FALSE);
      }
    }
  }

/*===========================================================================

FUNCTION tm_core_init

DESCRIPTION
  This function is to initialize TM-Core data structures.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_init(void)
{
  int i;
  nv_item_type      z_nv_qwip_read_buffer;
  int index;
  uint8 sub = 0;

  techSel_Init();

  if (tm_core_device_config.tm_cm_device_info == TM_CORE_DEVICE_MODE_SS)
  {
    tm_core_info.config_info.max_valid_subs_id = SM_CONFIG_MIN_SUB_ID;
    sm_nv_set_max_sub_id(SM_CONFIG_MIN_SUB_ID); // Single SIM only
  }
  else
  {
    tm_core_info.config_info.max_valid_subs_id = SM_CONFIG_MAX_SUB_ID;
    sm_nv_set_max_sub_id(SM_CONFIG_MAX_SUB_ID); // DSDS or TSTS
  }

  tm_core_info.diag_buffer_hdl = gnss_diag_buffer_init(C_GNSS_DIAG_BUF_ID_SM);

  /* set the DD sub configuration pointer to default sub at boot-up asap to avoid NULL pointer */
  tm_core_set_current_subs_cfg(SM_CONFIG_MIN_SUB_ID, TRUE);

  /* Read NV settings */
  tm_core_read_nv();

  if (tm_core_info.config_info.fusion_csm_config.q_TestControlMask & GNSS_NV_EFS_SM_FUSION_TEST_CONTROL_STORE_BARO_TO_EFS)
  {
    techSel_handleSensorNvConfig(tm_core_info.config_info.fusion_csm_config.u_BaroSampleRateHz,
                                 tm_core_info.config_info.fusion_csm_config.u_BaroFilterWeight,
                                 TRUE);
  }
  else
  {
    techSel_handleSensorNvConfig(tm_core_info.config_info.fusion_csm_config.u_BaroSampleRateHz,
                                 tm_core_info.config_info.fusion_csm_config.u_BaroFilterWeight,
                                 FALSE);
  }

  /* Initialize Leap Second Info */
  memset((void *)&tm_core_info.config_info.z_LeapSecondInfo, 0,
         sizeof(tm_core_info.config_info.z_LeapSecondInfo));
  tm_core_info.config_info.z_LeapSecondInfo.u_DeltaLeapSec = GPS_UTC_OFFSET;
  /* Intialize the uncertainity to 1. When ME updates, this value will have to correct uncertainity */
  tm_core_info.config_info.z_LeapSecondInfo.u_DeltaLeapSecUnc = TM_CORE_LEAP_SEC_INIT_UNC;

  /* Initialize non-NV config info. Default enable GPS */
  tm_core_info.config_info.sv_poly_rep_constell_config = 0;
  tm_core_info.config_info.gnss_meas_rep_constell_config = 0;

  /* Initialize SARF status to disabled at init */
  tm_core_info.config_info.v_sarf_status = FALSE;
  memset((void *)&tm_core_info.config_info.z_GnssPersistSV, 0,
         sizeof(tm_core_info.config_info.z_GnssPersistSV));

  /* Initialize session info */
  tm_core_info.session_info.active_sess_handle      = TM_SESS_HANDLE_NONE;
  tm_core_info.session_info.op_mode                 = TM_SESS_OPERATION_MODE_NONE;
  tm_core_info.session_info.op_requested            = TM_OP_REQ_NONE;
  tm_core_info.session_info.from_protocol           = TM_PRTL_TYPE_NONE;
  tm_core_info.session_info.pd_param.client_type    = PDSM_CLIENT_TYPE_NONE;
  tm_core_info.session_info.pd_param.client_id      = -1;
  tm_core_info.session_info.mo_sess_handle_seed     = 0;
  tm_core_info.session_info.session_event_firing    = TRUE;
  tm_core_info.session_info.eph_svmask              = 0;
  tm_core_info.session_info.alm_svmask              = 0;
  tm_core_info.session_info.queued_request          = FALSE;
  tm_core_info.session_info.q_FixTimeRemainingMSec    = 0;
  tm_core_info.session_info.msb_xtra_allowed        = FALSE;
  tm_core_info.session_info.req_xtra_trig_by_agps   = FALSE;
  tm_core_info.session_info.end_odp_session         = FALSE;
  tm_core_info.session_info.force_eph_download      = FALSE;
  tm_core_info.session_info.glo_force_eph_download  = FALSE;
  tm_core_info.session_info.bds_force_eph_download  = FALSE;
  tm_core_info.session_info.gal_force_eph_download  = FALSE;
  tm_core_info.session_info.eph_need_sv_mask = 0;
  tm_core_info.session_info.glo_eph_need_sv_mask = 0;
  tm_core_info.session_info.bds_eph_need_sv_mask = 0;
  tm_core_info.session_info.gal_eph_need_sv_mask = 0;
  tm_core_info.session_info.sess_info_valid_flags = 0;
  tm_core_info.session_info.receiver_off_needed     = FALSE;
  tm_core_info.session_info.iono                    = 1; /* iono invalid, 1 means MGP has
                                                         requested iono from AGPS */
  tm_core_info.session_info.utc                     = 1; /* utc invalid, 1 means MGP has
                                                         requested utc from AGPS */
  tm_core_info.session_info.health_svmask           = 0;
  tm_core_info.session_info.valid_svmask            = 0;
  tm_core_info.session_info.sbas_eph_svmask         = 0;
  tm_core_info.session_info.sbas_alm_svmask         = 0;
  tm_core_info.session_info.glo_eph_svmask          = 0;
  tm_core_info.session_info.glo_alm_svmask          = 0;
  tm_core_info.session_info.bds_eph_svmask          = 0;
  tm_core_info.session_info.bds_alm_svmask          = 0;

  tm_core_info.session_info.qzss_eph_svmask         = 0;
  tm_core_info.session_info.qzss_alm_svmask         = 0;
  tm_core_info.session_info.gal_eph_svmask          = 0;
  tm_core_info.session_info.gal_alm_svmask          = 0;
  tm_core_info.session_info.navic_eph_svmask        = 0;
  tm_core_info.session_info.navic_alm_svmask        = 0;
  tm_core_info.ext_opt_field_mask = 0;

  tm_core_info.session_info.v_session_started_for_e911 = FALSE;
  tm_core_info.session_info.q_num_fixes             = 0;
  tm_core_info.session_info.z_lppm_mode.e_PowerMode = TM_SESS_POWER_MODE_NORMAL;
  tm_core_info.session_info.z_lppm_mode.q_TbmMsecs  = 0;
  tm_core_info.cs_on_demand_active                  = FALSE;
  tm_core_info.ext_inj_allowed                      = TRUE;

  tm_core_info.w_RecvdMeasBlkMask                   = 0;

  memset((void *)&tm_core_info.pd_ext_data, 0, sizeof(tm_core_info.pd_ext_data));
  memset((void *)&tm_core_info.gps_meas_blk, 0, sizeof(tm_core_info.gps_meas_blk));
  memset((void *)&tm_core_info.glo_meas_blk, 0, sizeof(tm_core_info.glo_meas_blk));
  memset((void *)&tm_core_info.bds_meas_blk, 0, sizeof(tm_core_info.bds_meas_blk));
  memset((void *)&tm_core_info.qzss_meas_blk, 0, sizeof(tm_core_info.qzss_meas_blk));
  memset((void *)&tm_core_info.gal_meas_blk, 0, sizeof(tm_core_info.gal_meas_blk));

  tm_core_clear_cached_pos();
  tm_core_clear_cached_ref_pos();

  /* Initialize the call back logs */
  memset((void *)tm_call_back_log, 0, sizeof(tm_call_back_log));

  tm_core_info.e_cached_rcvr_state = C_GPS_RCVR_STATE_OFF;

  tm_core_update_session_state(TM_CORE_SESS_STATE_INIT);

  /* Initialize TM-Core / Protocol Interface */
  for (i = 0; i < TM_PRTL_NUM; i++)
  {
    tm_core_info.prtl_func_cb_table[i].init_fp             = NULL;
    tm_core_info.prtl_func_cb_table[i].start_sess_req_fp   = NULL;
    tm_core_info.prtl_func_cb_table[i].stop_sess_req_fp    = NULL;
    tm_core_info.prtl_func_cb_table[i].sess_req_data_fp    = NULL;
    tm_core_info.prtl_func_cb_table[i].sess_info_fp        = NULL;
#ifdef  FEATURE_CGPS_PDCOMM
    tm_core_info.prtl_func_cb_table[i].data_pipe_handle_fp = NULL;
#endif /* FEATURE_CGPS_PDCOMM */
    tm_core_info.prtl_func_cb_table[i].timer_cb_fp         = NULL;
    tm_core_info.prtl_func_cb_table[i].event_cb_fp         = NULL;
    tm_core_info.prtl_func_cb_table[i].general_query_fp    = NULL;
  }

  /* Setup init function for supported protocols */
  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_STANDALONE].init_fp = tm_standalone_prtl_init;

#if defined (FEATURE_SUPPORT_CDMA_GPS_PROTOCOLS) && !defined (FEATURE_GNSS_LOW_MEMORY)
  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].init_fp      = tm_1xup_prtl_init;
  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_CP].init_fp      = tm_1x_cp_prtl_init;
#endif /* FEATURE_SUPPORT_CDMA_GPS_PROTOCOLS && ! FEATURE_GNSS_LOW_MEMORY */

  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_DIAG].init_fp       = tm_diag_prtl_init;

#ifdef FEATURE_CGPS_UMTS_UP
  /*** DO NOT reverse the order of UP and CP, since some common actions only need
     to be initialized once, and if both are defined, they are done in UP */
  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].init_fp = tm_umts_up_prtl_iface_init;

#ifdef FEATURE_USER_PLANE_MRL
  tm_l1_iface_init();
#endif

#endif /* FEATURE_CGPS_UMTS_UP */

  /* Init the GFC_QMI module */
  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_GFC_QMI].init_fp = tm_gfc_qmi_prtl_init;

#ifdef FEATURE_CGPS_XTRA_T
  /* Init the XTRA-T module */
  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_XTRA_T].init_fp = tm_xtra_t_init;
#endif

#if defined (FEATURE_CGPS_UMTS_CP) && !defined (FEATURE_GNSS_LOW_MEMORY)
  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].init_fp = tm_umts_cp_prtl_iface_init;

  tm_mm_iface_init();
#endif /* FEATURE_CGPS_UMTS_CP && ! FEATURE_GNSS_LOW_MEMORY */

#if defined (FEATURE_CGPS_UMTS_CP_WCDMA) || defined ( FEATURE_WCDMA_GPS_CELL_ID_READING ) || defined (FEATURE_WCDMA_CM_CID_UPDATE_IN_DCH)
  tm_rrc_iface_init(); /* registers call back with RRC */
#endif
#if !defined(FEATURE_GNSS_SA) && defined(FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
  tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].init_fp  = tm_lpp_cp_prtl_iface_init;

  /* Initialize the Transaction Manager */
  tm_lpp_tx_mgr_init(0);
#endif /*!FEATURE_GNSS_SA && FEATURE_CGPS_LTE_CELLDB && !FEATURE_GNSS_LOW_MEMORY*/

#if (defined(FEATURE_CGPS_UMTS_UP_SUPL) || defined(FEATURE_CGPS_XTRA_T))
#ifdef FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM
  #error code not present
#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */
#endif /* FEATURE_CGPS_UMTS_UP_SUPL || FEATURE_CGPS_XTRA_T */

  /* Initialize supported protocol sub-modules */
  for (i = 0; i < TM_PRTL_NUM; i++)
  {
    if (tm_core_info.prtl_func_cb_table[i].init_fp != NULL)
    {
      tm_core_info.prtl_func_cb_table[i].init_fp();
    }
  }

#ifdef FEATURE_TM_SECURITY
  /* Initialize security layer */
  tm_sec_init();

  /* Make sure tm_sec_init is called before tm_core_read_security_nv */
  tm_core_read_security_setting();
#endif /* FEATURE_TM_SECURITY */

  if (tm_core_info.config_info.nmea_config_type.pd_nmea_port_type != PDSM_PA_NMEA_PORT_TYPE_NONE)
  {
    tm_nmea_diag_open_port((tm_nmea_diag_open_port_id_e_type)tm_core_info.config_info.nmea_config_type.pd_nmea_port_type);
  }
  else
  {
    tm_nmea_diag_close_port(NULL);
  }

  /* Initialize OEM DRE Tunnel Detection Enablement control flag */
  tm_core_info.config_info.oemdre_td_ctrl = FALSE;

#ifdef FEATURE_CGPS_XTRA
  /* Make sure tm_sec_init is called before tm_core_read_security_nv */
  tm_xtra_init_and_read_nv();
#endif /* FEATURE_CGPS_XTRA */


  tm_core_info.internal_session = FALSE;


  /* initialize XSPI module */
  tm_xspi_init_and_read_nv();

#ifdef FEATURE_GNSS_PERIODIC_PPM
  #error code not present
#endif

#ifdef FEATURE_GNSS_LPPE
  tm_auxTechSrnScanReportsInit();
#endif


  tm_core_info.tm_task_info.task_active = TRUE;
  tm_core_info.tm_task_info.v_mc_task_ready = FALSE;
  q_init(&tm_core_info.sm_set_param_q);
  tm_core_info.b_sm_set_param_q_initialized = TRUE;


  /* Initialise MMGSDI table with default session info */
  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type);
       index++)
  {
    tm_mmgsdi_session_id_table[index].asubs_id = SYS_MODEM_AS_ID_NONE;
    tm_mmgsdi_session_id_table[index].session_id = (mmgsdi_session_id_type)MMGSDI_INVALID_SESSION_ID;

    tm_mmgsdi_session_id_table[index].session_id_set = FALSE;
  }

  /* initialize the delete all throttle timer */
  tm_core_info.tm_delete_param_throttle_timer = os_TimerCreate((uint32)TM_CORE_TIMER_ID_DELETE_THROTTLE_ID, (uint32)THREAD_ID_SM_TM);

#if 0
  //TODO :: Remove
  tm_core_info.tech_sel_sim_tbm_hyst_timer = os_TimerCreate((uint32)TM_CORE_TIMER_ID_TECH_SEL_SIM_TBM_HYST,(uint32)THREAD_ID_SM_TM);
#endif

#ifdef FEATURE_GNSS_DTF
  tm_qsh_init();
#endif

  sdm_spoofDetector_init();
}

/*===========================================================================

FUNCTION tm_core_get_prtl_idx_by_type

DESCRIPTION
  This function returns protocol array index based on protcol type

DEPENDENCIES

RETURN VALUE
  -1, if protocol is not supported.
  Otherwise, array index for the protocol type

SIDE EFFECTS

===========================================================================*/
int tm_core_get_prtl_idx_by_type(tm_prtl_type prtl_type)
{
  switch (prtl_type)
  {
    case TM_PRTL_TYPE_STANDALONE:
      return TM_CORE_PRTL_CB_ARR_INDEX_STANDALONE;
#ifndef FEATURE_GNSS_LOW_MEMORY
    case TM_PRTL_TYPE_1X_UP:
      return TM_CORE_PRTL_CB_ARR_INDEX_1X_UP;

    case TM_PRTL_TYPE_1X_CP:
      return TM_CORE_PRTL_CB_ARR_INDEX_1X_CP;

    case TM_PRTL_TYPE_UMTS_CP:
      return TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP;
#endif /* ! FEATURE_GNSS_LOW_MEMORY - For fall back to standalone*/
#ifdef FEATURE_CGPS_UMTS_UP
    case TM_PRTL_TYPE_UMTS_UP:
      return TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP;
#endif
    case TM_PRTL_TYPE_DIAG:
      return TM_CORE_PRTL_CB_ARR_INDEX_DIAG;

#ifdef FEATURE_GNSS_PERIODIC_PPM
    #error code not present
#endif
    case TM_PRTL_TYPE_GFC_QMI:
      return TM_CORE_PRTL_CB_ARR_INDEX_GFC_QMI;

#ifdef FEATURE_CGPS_XTRA_T
    case TM_PRTL_TYPE_XTRA_T:
      return TM_CORE_PRTL_CB_ARR_INDEX_XTRA_T;
#endif

#if !defined(FEATURE_GNSS_SA) && defined(FEATURE_CGPS_LTE_CELLDB) && !defined (FEATURE_GNSS_LOW_MEMORY)
    case TM_PRTL_TYPE_LPP_CP:
      return TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP;
#endif

    case TM_PRTL_TYPE_CSM:
      return TM_CORE_PRTL_CB_ARR_INDEX_CSM;

    default:
      return -1;
  }
}

/*===========================================================================

FUNCTION tm_core_get_prtl_idx_by_lcs_cmd_type

DESCRIPTION
  This function returns protocol array index based on user response type.

DEPENDENCIES

RETURN VALUE
  protol array index if lcs_cmd_type is supported
  -1, otherwise

SIDE EFFECTS

===========================================================================*/
int tm_core_get_prtl_idx_by_lcs_cmd_type(pdsm_lcs_info_cmd_e_type  lcs_info_cmd_type)
{
  switch (lcs_info_cmd_type)
  {
    case PDSM_LCS_CMD_TYPE_VX_REQ:
    case PDSM_LCS_CMD_TYPE_VX_CONFIG:
    case PDSM_LCS_CMD_TYPE_SET_VX_CONFIG:
    case PDSM_LCS_CMD_TYPE_VX_SERVICE_INTERACTION:
      return TM_CORE_PRTL_CB_ARR_INDEX_1X_UP;

    case PDSM_LCS_CMD_TYPE_UMTS_CP_REQ:
      return TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP;

    case PDSM_LCS_CMD_TYPE_SUPL_REQ:
      return TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP;

    default:
      return -1;
  }
}

/*===========================================================================

FUNCTION tm_core_new_mo_sess_handle

DESCRIPTION
  This function generates a new MO session handle for incoming MO fix request.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
tm_sess_handle_type tm_core_new_mo_sess_handle(void)
{
  tm_sess_handle_type mo_sess_handle;

  tm_core_info.session_info.mo_sess_handle_seed %= 8;

  mo_sess_handle = 1 << tm_core_info.session_info.mo_sess_handle_seed;

  tm_core_info.session_info.mo_sess_handle_seed++;

  return mo_sess_handle;
}

/*===========================================================================
FUNCTION  tm_E911LTEOn

DESCRIPTION
  Checks whether it is an E911 call and LTE.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if it is both E911 and LTE.
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tm_E911LTEOn()
{
#if !defined ( FEATURE_GNSS_SA ) && !defined ( FEATURE_GNSS_LEAN_BUILD )
  tm_cm_phone_state_info_s_type z_phone_state_info;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_E911LTEOn called");

  memset((void *)&z_phone_state_info, 0, sizeof(z_phone_state_info));
  tm_cm_iface_get_phone_state(&z_phone_state_info);

  /* Check for E911 and LTE status. Real E911 */
  if ((C_EMERGENCY_HANDLING_NULL != tm_umts_common_utils_emergency_effect()) &&
      (CGPS_SRV_SYS_LTE == ((z_phone_state_info.emerg_srv_system) & CGPS_SRV_SYS_LTE)))
  {
    return TRUE;
  }

  /* Check for simulated E911 status and LTE */
  if ((C_EMERGENCY_HANDLING_NULL != tm_umts_common_utils_emergency_effect()) &&
      (z_phone_state_info.srv_system & CGPS_SRV_SYS_LTE) && 
      tm_agps_emergency_call_simulate())
  {
    return TRUE;
  }
#endif /* ! FEATURE_GNSS_LEAN_BUILD */
  return FALSE;
}

/*===========================================================================
FUNCTION  tm_E911WLANOn

DESCRIPTION
  Function to checks 
  1) if E911 call is on WLAN.
  2) LPP is enabled
  3) if SUPL during 911 over VoWIFI is enabled

DEPENDENCIES
  None

RETURN VALUE
  TRUE if call is E911, on VoWIFI and SUPL over VoWIFI is enabled.
  FALSE otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tm_E911WLANOn()
{
#ifndef FEATURE_GNSS_LEAN_BUILD
  tm_cm_phone_state_info_s_type z_phone_state_info;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_E911WLANOn called");

  memset((void *)&z_phone_state_info, 0, sizeof(z_phone_state_info));
  tm_cm_iface_get_phone_state(&z_phone_state_info);

  /* Check for E911 and WLAN status
    In 911 over VoWIFI on Call ORIG event, CM might not know the RAT on which the 
    call will be brought up, (L/WLAN), and it is updated only after Call CONNECT.
    However NW can push SUPL INIT before the call goes to CONNECTED state, hence 
    we need to check the DS RAT for the 911 call in such cases. So we check Both
    CM and DS RAT information*/

  if ((0 != TM_CORE_FEATURE_BIT_IS_SET(TM_SUPL_VOWIFI_EMERGENCY_ENABLE)) &&
      (TM_CM_E911_STATE_NONE != z_phone_state_info.e911_state) &&
      ((CGPS_SRV_SYS_WLAN == z_phone_state_info.emerg_conn_srv_system) ||
       (CGPS_SRV_SYS_WLAN == tm_ds_get_emergency_srv_sys())) &&
      (tm_core_get_lte_lpp_up_enable() || tm_core_get_nr_lpp_up_enable()))
  {
    return TRUE;
  }

#endif /* ! FEATURE_GNSS_LEAN_BUILD */
  return FALSE;
}

/*===========================================================================

FUNCTION tm_core_get_lte_lpp_up_enable

DESCRIPTION
  This function determines if LPP protocol on LTE is enabled in NV 

 

RETURN VALUE
  TRUE if LPP is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_lte_lpp_up_enable(void)
{
  if (TM_CORE_CONFIG_DD(gnss_lte_lpp_enable) & PDSM_PA_LPP_IN_LTE_UP_ENABLE)
  {
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION tm_core_get_nr_lpp_up_enable

DESCRIPTION
  This function determines if LPP protocol on NR is enabled in NV 

 

RETURN VALUE
  TRUE if LPP is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_nr_lpp_up_enable(void)
{
  if (TM_CORE_CONFIG_DD(gnss_lte_lpp_enable) & PDSM_PA_LPP_IN_NR5G_SA_UP_ENABLE)
  {
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION tm_core_get_nr_lpp_cp_enable

DESCRIPTION
  This function determines if LPP protocol on NR is enabled in NV 



RETURN VALUE
  TRUE if LPP is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_nr_lpp_cp_enable(void)
{
  if (TM_CORE_CONFIG_DD(gnss_lte_lpp_enable) & PDSM_PA_LPP_IN_NR5G_SA_CP_ENABLE)
  {
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION tm_core_lppe_up_precondition

DESCRIPTION
  This function decides if E911 LTE/WLAN call is on and LPPe is enabled 

RETURN VALUE
  boolean  True - E911 LTE/WLAN call on , otherwise FALSE.

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_lppe_up_precondition(void)
{
  uint32 q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_up_config);
  boolean const cv_e911LteOn = tm_E911LTEOn();
  boolean const cv_e911WLANOn = tm_E911WLANOn();

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPPe UP cfg info %d e911LTEOn %d e911WLANOn %d",
        q_ConfigInfo,
        cv_e911LteOn,
        cv_e911WLANOn);

  /*LPPe is only enabled during E911 & main LPPE bit is set*/
  if (cv_e911WLANOn && ((q_ConfigInfo & PDSM_PA_LPPE_ENABLE) != 0))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 WLAN on, LPPE UP set");
    return (TRUE);
  }
  /* LPPE is only enabled during E911 & main LPPE bit set any other technology */
  else if (cv_e911LteOn && ((q_ConfigInfo & PDSM_PA_LPPE_ENABLE) != 0))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 LTE on, LPPE UP set");
    return (TRUE);
  }

  return (FALSE);
}

/*===========================================================================

FUNCTION tm_core_lppe_cp_precondition

DESCRIPTION
  This function decides if E911 LTE call is on and LPPe is enabled

RETURN VALUE
  boolean  True - E911 LTE call on , otherwise FALSE.

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_lppe_cp_precondition(void)
{
  uint32 q_ConfigInfo = TM_CORE_CONFIG_DD(gnss_lte_lppe_cp_config);
  boolean const cv_e911LteOn = tm_E911LTEOn();

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPPe CP cfg info %d e911lte %d",
        q_ConfigInfo,
        cv_e911LteOn);

  /* LPPE is only enabled during E911 & main LPPE bit set any other technology */
  if (cv_e911LteOn && ((q_ConfigInfo & PDSM_PA_LPPE_ENABLE) != 0))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 LTE on, LPPE CP set");
    return (TRUE);
  }
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 LTE call not on CP");
  return (FALSE);
}


/*===========================================================================
FUNCTION tm_core_get_lpp_cp_enable

DESCRIPTION
  This function determines if LPP Control Plane protocol is enabled in NV 

 

RETURN VALUE
  TRUE if LPP is enabled, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_lpp_cp_enable(void)
{
  if ((TM_CORE_CONFIG_DD(gnss_lte_lpp_enable) & PDSM_PA_LPP_IN_LTE_CP_ENABLE) ||
      (TM_CORE_CONFIG_DD(gnss_lte_lpp_enable) & PDSM_PA_LPP_IN_NR5G_SA_CP_ENABLE))
  {
    return TRUE;
  }
  return FALSE;
}


/*===========================================================================
FUNCTION tm_core_get_gnss_emergency_protocol_non_lte

DESCRIPTION
  This function determines if Control Plane or UP protocol is used for E911 on non-LTE 

 

RETURN VALUE
  TRUE if UP is enabled, FALSE if CP

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
uint32 tm_core_get_gnss_emergency_protocol_non_lte(void)
{
  return tm_core_info.config_info.gnss_emergency_protocol_non_lte;
}

/*===========================================================================

FUNCTION tm_core_get_active_prtl_module

DESCRIPTION
  This function checks the acquired system and selects a proper protocol
  sub-module to handle incoming MO fix request.

DEPENDENCIES

RETURN VALUE
  protocol sub-module type.

SIDE EFFECTS

===========================================================================*/
tm_prtl_type tm_core_get_active_prtl_module(pdsm_mt_initiator_e_type  mt_initiator)
{
  cgps_SrvSystemType srv_system;
  tm_cm_phone_state_info_s_type phone_state_info;

  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  if (TM_CM_E911_STATE_NONE == phone_state_info.e911_state)
  {
    srv_system = phone_state_info.srv_system;
  }
  else if ((CGPS_SRV_SYS_WLAN == tm_ds_get_emergency_srv_sys()) ||
           (CGPS_SRV_SYS_WLAN == phone_state_info.emerg_conn_srv_system))
  {
    srv_system = CGPS_SRV_SYS_WLAN;
  }
  else
  {
    srv_system = phone_state_info.emerg_srv_system;
  }

  switch (srv_system)
  {
    case CGPS_SRV_SYS_CDMA:
    case CGPS_SRV_SYS_HDR:
      if (TM_CORE_CONFIG_DD(gnss_1x_up_supl_enable))
      {
        return TM_PRTL_TYPE_UMTS_UP;
      }
      return TM_PRTL_TYPE_1X_UP;
    case CGPS_SRV_SYS_GSM:
    case CGPS_SRV_SYS_WCDMA:
    case CGPS_SRV_SYS_GW:
      if (mt_initiator == MT_NA) /* not network-initiated */
      {
        if (tm_core_info.config_info.u_mo_method == MO_UP)
          return TM_PRTL_TYPE_UMTS_UP;
        else
          return TM_PRTL_TYPE_UMTS_CP;
      }
      else /* network-initiated */
      {
        if (mt_initiator == MT_UP)
          return TM_PRTL_TYPE_UMTS_UP;
        else
          return TM_PRTL_TYPE_UMTS_CP;
      }

    case CGPS_SRV_SYS_LTE:
    case CGPS_SRV_SYS_WLAN:
    case CGPS_SRV_SYS_NR5G:
      if (mt_initiator == MT_NA) /* not network-initiated */
      {
        if (tm_core_info.config_info.u_mo_method == MO_UP)
          return TM_PRTL_TYPE_UMTS_UP;
        else
          return TM_PRTL_TYPE_NONE;
      }
      else /* network-initiated */
      {
        if (mt_initiator == MT_UP)
        {
          return TM_PRTL_TYPE_UMTS_UP;
        }
        else if ((mt_initiator == MT_CP) && (tm_core_get_lpp_cp_enable() == TRUE))
        {
          return TM_PRTL_TYPE_LPP_CP;
        }
      }
      return TM_PRTL_TYPE_NONE;

    case CGPS_SRV_SYS_TDSCDMA:
      return TM_PRTL_TYPE_UMTS_UP; /* for now only SUPL for TDSCDMA */

    default:
      return TM_PRTL_TYPE_NONE;
  }
}

/*===========================================================================

FUNCTION tm_core_api_cb_timer_handler

DESCRIPTION
  This function is to handle api callbacks in TM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_api_cb_timer_handler(void)
{
  tm_api_notifier_type api_notifier;

  api_notifier.pNotifierCb = NULL;
  api_notifier.clientIndex = 0;
  api_notifier.broadcast = TRUE;

  if (tm_cb_timer_counter < TM_CORE_MAX_NUM_API_CALLBACKS)
  {
    tm_core_api_callback_handler(&api_notifier);
    tm_cb_timer_counter++;
  }
  /* Timer expired, print the call back logs */
  else if (tm_cb_timer_counter == TM_CORE_MAX_NUM_API_CALLBACKS)
  {
    gnss_display_cb_logs(tm_call_back_log, (uint16)THREAD_ID_SM_TM);
  }
}

/*===========================================================================

FUNCTION tm_core_timer_handler

DESCRIPTION
  This function is to figure out which TM-Core timer has expired and call proper
  timer handler.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_timer_handler(const os_TimerExpiryType *timer_data)
{
  if (timer_data != NULL)
  {
    switch (timer_data->q_TimerId)
    {
      case TM_CORE_TIMER_ID_API_CALLBACK:
        tm_core_api_cb_timer_handler();
        break;

#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
      case TM_AUXTECH_WIFI_ON_DEMAND_REQ_TIMER_ID:
        tm_auxtech_wifi_on_demand_req_timer_expired(timer_data->q_TimerId);
        break;
#endif // FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS

#ifdef FEATURE_USE_GNSS_DIAG_BUF
      case TM_DIAG_BUFFER_RESET_TO_STREAMING_TIMER_ID:
        tm_diag_buf_timer_dispatch(timer_data->q_TimerId);
        break;
#endif

      default:
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Unknown timer expired %u", timer_data->q_TimerId);
        break;
    }
  }
  else /* null pointer check */
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can not proess null timer msg");
  }
}

/*===========================================================================

FUNCTION tm_core_get_prtl_idx_by_timer_id

DESCRIPTION
  This function returns protocol array index based on timer ID

DEPENDENCIES

RETURN VALUE
  -1, if timer ID can not be found.
  Otherwise, protocol array index for the timer ID.

SIDE EFFECTS

===========================================================================*/
int tm_core_get_prtl_idx_by_timer_id(uint32 timer_id)
{
  switch (timer_id & TM_CORE_TIMER_DISPATCH_MASK)
  {
    case TM_STANDALONE_TIMER_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_STANDALONE;

    case TM_1X_UP_TIMER_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_1X_UP;

    case TM_1X_CP_TIMER_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_1X_CP;

    case TM_UMTS_UP_TIMER_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP;

    case TM_UMTS_CP_TIMER_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP;

    case TM_DIAG_TIMER_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_DIAG;

#ifdef FEATURE_GNSS_PERIODIC_PPM
    #error code not present
#endif
    case TM_LPP_CP_TIMER_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP;

    default:
      return -1;
  }
}

/*===========================================================================

FUNCTION tm_core_timer_dispatcher

DESCRIPTION
  This function is to figure out which TM software sub-module the timer expiry event
  belongs to and call the proper timer handler (provided to TM-Core by each sub-module)

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_timer_dispatcher(void *timer_data)
{
  os_TimerExpiryType *timer_param = (os_TimerExpiryType *)timer_data;
  int prtlTableIndex;

  if (timer_param != NULL)
  {
    switch (timer_param->q_TimerId & TM_CORE_TIMER_DISPATCH_MASK)
    {
      case TM_CORE_TIMER_ID_RANGE:
        tm_core_timer_handler(timer_param);
        break;

      case TM_PDAPI_TIMER_ID_RANGE:
        //  pdApi_TimerHandler(timer_param);
        break;

      case TM_CM_IFACE_TIMER_ID_RANGE:
        tm_cm_handle_timer_expiry(timer_param);
        break;

#ifdef FEATURE_TM_SECURITY
      case TM_SEC_LAYER_TIMER_ID_RANGE:
        tm_sec_handle_timer_expiry(timer_param);
        break;
#endif /* FEATURE_TM_SECURITY */

#ifdef FEATURE_CGPS_XTRA
      case TM_XTRA_TIMER_ID_RANGE:
        tm_xtra_handle_timer_expiry(timer_param);
        break;
#endif /* FEATURE_CGPS_XTRA */

      case TM_XSPI_ID_RANGE:
        tm_xspi_handle_timer_expiry(timer_param);
        break;

#ifdef FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM
      #error code not present
#endif /* FEATURE_RD_SUPL_HSLP_N_CERTS_FRM_USIM */

      case TM_STANDALONE_TIMER_ID_RANGE:
      case TM_1X_UP_TIMER_ID_RANGE:
      case TM_1X_CP_TIMER_ID_RANGE:
      case TM_UMTS_UP_TIMER_ID_RANGE:
      case TM_UMTS_CP_TIMER_ID_RANGE:
      case TM_DIAG_TIMER_ID_RANGE:
      case TM_LPP_CP_TIMER_ID_RANGE:
        /* Timer for protocol sub-modules */
        prtlTableIndex = tm_core_get_prtl_idx_by_timer_id(timer_param->q_TimerId);

        if ((prtlTableIndex >= 0) && (prtlTableIndex < TM_PRTL_NUM))
        {
          if (tm_core_info.prtl_func_cb_table[prtlTableIndex].timer_cb_fp != NULL)
          {
            tm_core_info.prtl_func_cb_table[prtlTableIndex].timer_cb_fp(timer_data);
          }
        }
        break;

#ifdef FEATURE_GNSS_PERIODIC_PPM
      #error code not present
#endif

      case TM_TECHSEL_TIMER_ID_RANGE:
        techSel_ReportTimerExpiry(timer_param->q_TimerId);
        break;

      case TM_CORE_TIMER_ID_DELETE_THROTTLE_ID:
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "DELETE timer expired", 0, 0, 0);
        break;

      case TM_LPC_TIMER_ID_RANGE:
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Location processing secondary client timer expired", 0, 0, 0);
        tm_lpc_timer_expiry_handler(timer_param->q_TimerId);
        break;
#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
      case TM_FUSION_CSM_TIMER_ID_RANGE :
        fusion_csm_ReportTimerExpiry(timer_param->q_TimerId, timer_param->q_Param);
        break;
#endif // FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
      case TM_DIAG_PFA_TEST_ID_RANGE:
        tm_diag_PfaTestSanityTimeout();
        break;
      default:
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can not find proper timer handler", 0, 0, 0);
        break;
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can not dispatch null timer msg", 0, 0, 0);
  }
  return;
}
/*===========================================================================

FUNCTION tm_core_session_active

DESCRIPTION
  This function tells if Tm-Core is in any active PD session (MI or MT).

  If there is an active PD on-demand session, we return FALSE

  If there is an active Periodic PPM session, we return FALSE

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_session_active(void)
{
  /* check if there is a GPS session or on-demand session is active*/
  if (tm_core_info.session_info.session_state == TM_CORE_SESS_STATE_INIT)
  {
    return FALSE;
  }
  else
  {
    return TRUE; /*lint !e506 */
  }
}

/*===========================================================================

FUNCTION tm_core_session_ok_to_start_new_session

DESCRIPTION
  This function tells if Tm-Core is willing to start a new PD session

  If there is an already existing session, this function will return FALSE
  EXCEPT in the case of:~
    If there is an active PD on-demand session, we return FALSE
    If there is an active Periodic PPM session, we return FALSE

  A PD session will always preempt these two types of sessions.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_session_ok_to_start_new_session(void)
{
  /* check if there is a GPS session or on-demand session is active*/
  if (tm_core_info.session_info.session_state == TM_CORE_SESS_STATE_INIT)
  {
    return TRUE;
  }
  else
  {
    return FALSE; /*lint !e506 */
  }
}


/*===========================================================================

FUNCTION tm_core_translate_end_status

DESCRIPTION
  This function translate internal abort reason to external end status.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
pdsm_pd_end_e_type tm_core_translate_end_status
(
   tm_sess_stop_reason_e_type  abort_reason
   )
{
  pdsm_pd_end_e_type ret_val = PDSM_PD_END_SESS_NONE;

  switch (abort_reason)
  {
    case TM_STOP_REASON_E911:
      ret_val = PDSM_PD_END_E911;
      break;

    case TM_STOP_REASON_USER_TERMINATED:
      ret_val = PDSM_PD_END_CLIENT_END;
      break;

    case TM_STOP_REASON_PRM_TIMEOUT:
    case TM_STOP_REASON_PPM_TIMEOUT:
    case TM_STOP_REASON_FIX_TIMEOUT:
      ret_val = PDSM_PD_END_TIMEOUT;
      break;
    case TM_STOP_REASON_GENERAL_ERROR:
      ret_val = PDSM_PD_END_FIX_ERROR;
      break;

    case TM_STOP_REASON_MPC_REJECTED:
      ret_val = PDSM_PD_END_VX_AUTH_FAIL;
      break;

    case TM_STOP_REASON_PDE_NO_CON:
      ret_val = PDSM_PD_END_NO_CON;
      break;
    case TM_STOP_REASON_PDE_CON_FAIL:
      ret_val = PDSM_PD_END_CON_FAIL;
      break;
    case TM_STOP_REASON_PDE_REJECT:
      ret_val = PDSM_PD_END_PDE_REJECT;
      break;
    case TM_STOP_REASON_STALE_BS_INFO:
      ret_val = PDSM_PD_END_STALE_BS_INFO;
      break;
    case TM_STOP_REASON_PPM_POST_FIX_TIMEOUT:
      ret_val = PDSM_PD_END_PPM_POST_FIX_TIMEOUT;
      break;
    case TM_STOP_REASON_PPM_PRE_FIX_TIMEOUT:
      ret_val = PDSM_PD_END_PPM_PRE_FIX_TIMEOUT;
      break;
    case TM_STOP_REASON_IS801_SERVER_TIMEOUT:
      ret_val = PDSM_PD_END_IS801_SERVER_TIMEOUT;
      break;
    case TM_STOP_REASON_IS801_PAYLOAD_ERROR:
      ret_val = PDSM_PD_END_IS801_PAYLOAD_ERROR;
      break;
    case TM_STOP_REASON_IS801_SEND_FAILURE:
      ret_val = PDSM_PD_END_IS801_SEND_FAILURE;
      break;
    case TM_STOP_REASON_PDE_END_SESSION:
      ret_val = PDSM_PD_END_PDE_END_SESSION;
      break;

      /*------------------------------------------------------------------
          JGPS specific end status reason
      ------------------------------------------------------------------*/
    case TM_STOP_REASON_JGPS_POSRESP_NG_LIS:
      ret_val = PDSM_PD_END_JGPS_POSRESP_NG_LIS;
      break;

    case TM_STOP_REASON_JGPS_POSRESP_NG_LSU:
      ret_val = PDSM_PD_END_JGPS_POSRESP_NG_LSU;
      break;

    case TM_STOP_REASON_JGPS_POSRESP_NG_MSI:
      ret_val = PDSM_PD_END_JGPS_POSRESP_NG_MSI;
      break;

    case TM_STOP_REASON_JGPS_STOPPOS_NG_LIS:
      ret_val = PDSM_PD_END_JGPS_STOPPOS_NG_LIS;
      break;

    case TM_STOP_REASON_JGPS_STOPPOS_NG_MSI:
      ret_val = PDSM_PD_END_JGPS_STOPPOS_NG_MSI;
      break;

    case TM_STOP_REASON_JGPS_POSCOMP_NG:
      ret_val = PDSM_PD_END_JGPS_POSCOMP_NG;
      break;

    case TM_STOP_REASON_JGPS_TIMEOUT_T04:
      ret_val = PDSM_PD_END_JGPS_TIMEOUT_T04;
      break;

    case TM_STOP_REASON_JGPS_TIMEOUT_STOPPOS:
      ret_val = PDSM_PD_END_JGPS_TIMEOUT_STOPPOS;
      break;

    case TM_STOP_REASON_JGPS_TIMEOUT_T02:
      ret_val = PDSM_PD_END_JGPS_TIMEOUT_T02;
      break;

    case TM_STOP_REASON_JGPS_TIMEOUT_IS801:
      ret_val = PDSM_PD_END_JGPS_TIMEOUT_IS801;
      break;

    case TM_STOP_REASON_JGPS_IS801_LRREJ:      /* LR Reject  reception */
    case TM_STOP_REASON_JGPS_IS801_AAREJ:      /* AA Reject  reception */
    case TM_STOP_REASON_JGPS_IS801_EPHREJ:     /* EPH Reject  reception */
    case TM_STOP_REASON_JGPS_IS801_ALMREJ:     /* ALM Reject  reception */
    case TM_STOP_REASON_JGPS_IS801_SEEDREJ:    /* SEED Reject  reception */
      ret_val = PDSM_PD_END_JGPS_IS801_LRREJ;
      break;

    case TM_STOP_REASON_JGPS_IS801_AA_FAIL:
      ret_val = PDSM_PD_END_JGPS_IS801_AA_FAIL;
      break;

    case TM_STOP_REASON_JGPS_PPP:
      ret_val = PDSM_PD_END_JGPS_PPP;
      break;

    case TM_STOP_REASON_JGPS_TCP_MS:
      ret_val = PDSM_PD_END_JGPS_TCP_MS;
      break;

    case TM_STOP_REASON_JGPS_TCP_SERV:
      ret_val = PDSM_PD_END_JGPS_TCP_SERV;
      break;

    case TM_STOP_REASON_JGPS_ACQRESP_NG_LIS:
      ret_val = PDSM_PD_END_JGPS_ACQRESP_NG_LIS;
      break;

    case TM_STOP_REASON_JGPS_ACQRESP_NG_LSU:
      ret_val = PDSM_PD_END_JGPS_ACQRESP_NG_LSU;
      break;

    case TM_STOP_REASON_JGPS_ACQRESP_NG_MSI:
      ret_val = PDSM_PD_END_JGPS_ACQRESP_NG_MSI;
      break;

    case TM_STOP_REASON_JGPS_STOPACQ_NG_LIS:
      ret_val = PDSM_PD_END_JGPS_STOPACQ_NG_LIS;
      break;

    case TM_STOP_REASON_JGPS_STOPACQ_NG_MSI:
      ret_val = PDSM_PD_END_JGPS_STOPACQ_NG_MSI;
      break;

    case TM_STOP_REASON_JGPS_TIMEOUT_T20:
      ret_val = PDSM_PD_END_JGPS_TIMEOUT_T20;
      break;

    case TM_STOP_REASON_JGPS_TIMEOUT_STOPACQ:
      ret_val = PDSM_PD_END_JGPS_TIMEOUT_STOPACQ;
      break;

    case  TM_STOP_REASON_SS_MOLR_ERRORS_SYSTEM_FAILURE:
      ret_val = PDSM_PD_END_SS_MOLR_ERRORS_SYSTEM_FAILURE; /* mapped to corresponding SS-molr-error error code */
      break;
    case  TM_STOP_REASON_SS_MOLR_ERRORS_UNEXPECTED_DATA_VALUE:
      ret_val = PDSM_PD_END_SS_MOLR_ERRORS_UNEXPECTED_DATA_VALUE;
      break;
    case  TM_STOP_REASON_SS_MOLR_ERRORS_DATA_MISSING:
      ret_val  = PDSM_PD_END_SS_MOLR_ERRORS_DATA_MISSING;
      break;
    case  TM_STOP_REASON_SS_MOLR_ERRORS_FACILITY_NOT_SUPPORTED:
      ret_val  = PDSM_PD_END_SS_MOLR_ERRORS_FACILITY_NOT_SUPPORTED;
      break;
    case  TM_STOP_REASON_SS_MOLR_ERRORS_SS_SUBSCRIPTION_VIOLATION:
      ret_val  = PDSM_PD_END_SS_MOLR_ERRORS_SS_SUBSCRIPTION_VIOLATION;
      break;
    case  TM_STOP_REASON_SS_MOLR_ERRORS_POSITION_METHOD_FAILURE:
      ret_val  = PDSM_PD_END_SS_MOLR_ERRORS_POSITION_METHOD_FAILURE;
      break;
    case  TM_STOP_REASON_SS_MOLR_ERRORS_UNDEFINED:
      ret_val  = PDSM_PD_END_SS_MOLR_ERRORS_UNDEFINED;
      break;

    case TM_STOP_REASON_ON_RESET_LOCATION_SERVICE:
      ret_val = PDSM_PD_END_RESET_LOCATION_SERVICE;
      break;
    case TM_STOP_REASON_BACKGROUND_LPPM_CANNOT_ENGAGE:
      ret_val =  PDSM_PD_END_ERR_STATE;
      break;
    default:
      ret_val = PDSM_PD_END_SESS_NONE;
      break;
  }


  /* Translate JGPS specific error codes */
  /*lint -e{56} valid types */
  /*lint -e{58} valid types */
  if (abort_reason >= TM_STOP_REASON_JGPS_DMSB_ERROR
      && abort_reason <= (TM_STOP_REASON_JGPS_DPOSCOMP_NG
                          + PDSM_PD_DLOAD_EPHEMERIS
                          + PDSM_PD_DLOAD_ALMANAC
                          + PDSM_PD_DLOAD_LOCATION)
      )
  {
    ret_val = (pdsm_pd_end_e_type)abort_reason;
  }

  return ret_val;
}

/*===========================================================================

FUNCTION tm_core_inform_pd_event

DESCRIPTION
  This function report PD event

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void tm_core_inform_pd_event(prtl_event_cb_f_type *p_event_cb_fp, pdsm_pd_event_type const cpd_event)
{
  prtl_event_u_type event_payload;

  if (p_event_cb_fp != NULL)
  {
    event_payload.pd_event_payload.pd_event = cpd_event;
    p_event_cb_fp(PRTL_EVENT_TYPE_PD, &event_payload);
  }
}

/*===========================================================================

FUNCTION tm_core_req_lm_recvr_off

DESCRIPTION
  This function request LM to turn off the receiver

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void tm_core_req_lm_recvr_off()
{
  lm_request_info_s_type lm_info_req_param;

  if (pdsmclient_get_client_id_map(tm_core_info.session_info.pd_param.client_id) == PDSM_CLIENT_TYPE_OEM)
  {
    lm_info_req_param.e_req_type = LM_REQUEST_FORCE_RECVR_OFF;
  }
  else
  {
    lm_info_req_param.e_req_type = LM_REQUEST_RECVR_OFF;
  }

  if ((tm_core_info.session_info.op_mode != TM_SESS_OPERATION_MODE_OTDOA) ||
      (tm_core_info.session_info.op_mode  == TM_SESS_OPERATION_MODE_OTDOA &&
       tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_UP_NI))
  {
    (void)lm_request(&lm_info_req_param);
  }
}

/*===========================================================================

FUNCTION tm_core_lm_sess_stop_req

DESCRIPTION
  This function request LM to stop the session

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void tm_core_lm_sess_stop_req(tm_sess_handle_type const active_sess_handle,
                                     uint32 q_req_mask,  boolean const cv_force_mgp_off)
{
  lm_session_request_info_s_type lm_sess_req_param;

  lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_STOP;
  lm_sess_req_param.q_handle   = active_sess_handle;
  lm_sess_req_param.q_req_mask = q_req_mask;

  if (LM_SESSION_REQUEST_FIX == lm_sess_req_param.q_req_mask)
  {
    lm_sess_req_param.z_request.z_fix_request.u_force_mgp_off = cv_force_mgp_off;
  }
  lm_session_request(&lm_sess_req_param);
}

/*===========================================================================

FUNCTION tm_core_abort_recover_send_pd_event_end
DESCRIPTION
  This function sends pd event end.

DEPENDENCIES

RETURN VALUE
    None

SIDE EFFECTS

===========================================================================*/

static pdsm_pd_end_e_type tm_core_abort_recover_send_pd_event_end
(
   tm_sess_stop_reason_e_type  abort_reason,
   boolean                     fire_event
   )
{
  tm_pdapi_pd_event_s_type     z_pd_event_type = { 0 };

  if (tm_core_session_active())
  {
    z_pd_event_type.pd_event_data.pd_info_type.client_id = tm_core_info.session_info.pd_param.client_id;
    z_pd_event_type.pd_event_data.pd_info_type.pd_info.end_status = PDSM_PD_END_SESS_NONE;

    /* Fire End event if fix does not complete successfully */
    if ((abort_reason != TM_STOP_REASON_COMPLETED) &&
        (fire_event == TRUE))
    {
      if (tm_core_info.session_info.pd_param.pd_option.session == PDSM_PD_SESS_TYPE_DATA_DLOAD)
      {
        z_pd_event_type.e_pd_event = PDSM_PD_DLOAD_EVENT_END;
      }
      else
      {
        z_pd_event_type.e_pd_event = PDSM_PD_EVENT_END;
      }
      z_pd_event_type.pd_event_data.pd_info_type.pd_info.end_status = tm_core_translate_end_status(abort_reason);
      tm_pdapi_pd_event_callback(&z_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);
    }
    return z_pd_event_type.pd_event_data.pd_info_type.pd_info.end_status;
  }
  else
  {
    z_pd_event_type.pd_event_data.pd_info_type.client_id = tm_core_info.session_info.pd_param.client_id;
    if (tm_core_info.session_info.pd_param.pd_option.session == PDSM_PD_SESS_TYPE_DATA_DLOAD)
    {
      z_pd_event_type.e_pd_event = PDSM_PD_DLOAD_EVENT_END;
    }
    else
    {
      z_pd_event_type.e_pd_event = PDSM_PD_EVENT_END;
    }
    z_pd_event_type.pd_event_data.pd_info_type.pd_info.end_status = tm_core_translate_end_status(abort_reason);
    tm_pdapi_pd_event_callback(&z_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);

    return PDSM_PD_END_SESS_NONE;
  }
}
/*===========================================================================

FUNCTION tm_core_abort_recover_send_pd_event_done

DESCRIPTION
  This function sends pd event done.

DEPENDENCIES

RETURN VALUE
    None

SIDE EFFECTS

===========================================================================*/
static void tm_core_abort_recover_send_pd_event_done
(
   tm_sess_stop_reason_e_type    abort_reason,
   boolean                       fire_event,
   pdsm_client_id_type           client_id,
   int                           prtlTableIndex
   )
{
  tm_pdapi_pd_event_s_type       z_pd_event_type = { 0 };

  if (TRUE == fire_event)
  {
    z_pd_event_type.pd_event_data.pd_info_type.client_id = client_id;
    if (PDSM_PD_SESS_TYPE_DATA_DLOAD == tm_core_info.session_info.pd_param.pd_option.session)
    {
      z_pd_event_type.e_pd_event = PDSM_PD_EVENT_DLOAD_DONE;
      tm_pdapi_pd_event_callback(&z_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);
    }
    else
    {
      z_pd_event_type.e_pd_event = PDSM_PD_EVENT_DONE;
      tm_pdapi_pd_event_callback(&z_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);

      /* Check prtl_func_cb_table bounds */
      if ((prtlTableIndex >= 0) && (prtlTableIndex < TM_PRTL_NUM))
      {
        /* This is added for internal Vx NI client */
        if (tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp != NULL)
        {
          tm_core_inform_pd_event(tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp,
                                  PDSM_PD_EVENT_DONE);
        }
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Attempt for an out of bound access to prtl_func_cb_table with index = %d", prtlTableIndex, 0, 0);
      }
      if (b_emerg_sess_start_sent == TRUE)
      {
       tm_core_send_pdapi_emerg_sess_type(PDSM_PD_EXT_STATUS_EMERG_SESS_DONE);
       b_emerg_sess_start_sent = FALSE;
      }
    }
  }
  if ((fire_event == TRUE) ||
      (abort_reason == TM_STOP_REASON_INTERNAL_SESSION_DELETE_PARAM))
  {
    /*Send Done event to GM core as well*/
    z_pd_event_type.pd_event_data.pd_info_type.client_id = client_id;
    z_pd_event_type.e_pd_event = PDSM_PD_EVENT_DLOAD_DONE;
    gm_update_pd_event(z_pd_event_type.e_pd_event, abort_reason);
  }
}


/*===========================================================================

FUNCTION tm_core_abort_recover

DESCRIPTION
  This function performs TM-Core software recovery.

DEPENDENCIES

RETURN VALUE
    None

SIDE EFFECTS

===========================================================================*/
void tm_core_abort_recover
(
   boolean                       ask_prtl,
   tm_sess_stop_reason_e_type    abort_reason
   )
{
  int                            prtlTableIndex = -1;
  boolean                        prtl_abort_session = TRUE;
  pdsm_client_id_type            client_id;
  tm_pdapi_pd_event_s_type       *pz_pd_event_type = NULL;
  boolean                        fire_event;
  boolean                        fire_nmea                = TRUE;
  boolean                        v_CpiSpecialReq          = FALSE;
  pdsm_client_type_e_type           client_type;
  tm_cm_phone_state_info_s_type phone_state_info;
  pdsm_pd_end_e_type            e_end_status = PDSM_PD_END_SESS_NONE;
  techSel_PosSessInfo  z_sess_info = { 0 };
  lm_mo_mt_mode_e_type  e_mo_mt = tm_core_mo_mt_mode(tm_core_info.session_info.active_sess_handle);

#ifdef FEATURE_GNSS_GERA_ENABLE
  #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

  ldm_client_id_e_type            e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type            e_secondary_client_id = LDM_CLIENT_ID_MAX;
  tm_loc_processing_client_info        *p_z_lpc_client     = NULL;

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Abort recover called");

  /* Init */
  pz_pd_event_type = (tm_pdapi_pd_event_s_type *)os_MemAlloc(sizeof(tm_pdapi_pd_event_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_pd_event_type)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cannot allocate memory for tm_pdapi_pd_event_s_type in tm_core_abort_recover");
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Falling back on stack memory");
  }
  else
  {
    memset(pz_pd_event_type, 0, sizeof(*pz_pd_event_type));
  }
  tm_cm_iface_get_phone_state(&phone_state_info);

  /* Get main and secondary client  from LDM */
  tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

  p_z_lpc_client     = lpc_get_loc_processing_client(e_main_client_id);

  if (NULL == p_z_lpc_client)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC: Main client not found");
  }
  else if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
  {
    /* Set the correct pdapi_client id */
    tm_core_info.session_info.pd_param.client_id = p_z_lpc_client->pd_clientId;
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: PDAPI client id %d set for main client %d",
    tm_core_info.session_info.pd_param.client_id, e_main_client_id);
  }
  

#ifdef FEATURE_CGPS_XTRA
  /* Handle the abort reason for XTRA sessions */
  tm_xtra_handle_core_abort_recover(abort_reason);
#endif /* FEATURE_CGPS_XTRA */
  z_sess_info.v_is_mo = (LM_MO_MT_MODE_MO  == e_mo_mt);

  z_sess_info.q_tbf_msec = tm_core_info.session_info.pd_param.pd_option.fix_rate.time_between_fixes_ms;
  z_sess_info.q_num_fixes = tm_core_info.session_info.pd_param.pd_option.fix_rate.num_fixes;
  z_sess_info.e_mode = tm_core_info.session_info.op_mode;
  z_sess_info.z_power_mode = tm_core_info.session_info.z_lppm_mode;

  z_sess_info.v_is_emergency = (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911);

  z_sess_info.v_is_internal = (tm_core_info.internal_session == TRUE);

  /* Check if tm_core is in active state or if on-demand session is on or if we got a set params
  for delete parms abort the session */
  if (tm_core_session_active())
  {
    prtlTableIndex = tm_core_get_prtl_idx_by_type(tm_core_info.session_info.from_protocol);

    if (((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM)) ||
        (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911))
    {

#if !defined ( FEATURE_GNSS_SA ) && !defined ( FEATURE_GNSS_LEAN_BUILD )
      /*Call STOP CPI only if we are in non concurrent mode
        For concurrent mode the protocol stop session will take care of 
        calling STOP CPI*/
      if ( ( phone_state_info.e911_state != TM_CM_E911_STATE_CONCURRENT ) &&
           ( FALSE == tm_lpc_client_special_req_check() ) )
      {
        /* Stop ODCPI inject if started earlier */
        tm_stopCPI(TM_AUXTECH_REQ_ID_LPPE);
#ifdef FEATURE_GNSS_LPPE
        le_TrackPosConsistencyStartStop(FALSE);
#endif /* FEATURE_GNSS_LPPE */
      }



#endif /* ! FEATURE_GNSS_LEAN_BUILD */

      /* If an abort request need a protocol sub-modules' consent, only proceed if a protocol sub-moulde allows it */
      if ((ask_prtl) && (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_E911))
      {
        /* Set reason to general error for now, if return value */
        if (tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
        {
          prtl_abort_session =
             tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp(
             tm_core_info.session_info.active_sess_handle,
             PRTL_SESS_STOP_SINGLE_FIX,
             abort_reason);

          if (prtl_abort_session == TRUE)
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Protocol aborted the session", 0, 0, 0);
#ifdef FEATURE_GNSS_GERA_ENABLE
            #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Protocol continue with the session", 0, 0, 0);
          }
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No protocol callback installed 2", 0, 0, 0);
#ifdef FEATURE_GNSS_GERA_ENABLE
          #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
        }
      }  /* if(ask_prtl == TRUE)  */
      else if ((ask_prtl == TRUE) &&
               (phone_state_info.e911_state != TM_CM_E911_STATE_NONE) &&
               (phone_state_info.emerg_srv_system == CGPS_SRV_SYS_WCDMA) &&
               (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911) &&
               (abort_reason != TM_STOP_REASON_LTE_TO_UMTS_CSFB))
      {
        /* Sending a callback to SUPL to replace the certificate in the cache with
              the one sent in the set_parameters request */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Aborting WCDMA CP emergency session reason %d", abort_reason, 0, 0);
        prtlTableIndex = tm_core_get_prtl_idx_by_type(TM_PRTL_TYPE_UMTS_CP);

        if ((prtlTableIndex > -1) &&
            (prtlTableIndex < TM_PRTL_NUM) &&
            tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
        {
          prtl_abort_session =
             tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp(
             tm_core_info.session_info.active_sess_handle,
             PRTL_SESS_STOP_SINGLE_FIX,
             abort_reason);

          if (prtl_abort_session == TRUE)
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Protocol aborted the session", 0, 0, 0);
#ifdef FEATURE_GNSS_GERA_ENABLE
            #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
          }
        }
      }
      else
      {

#ifdef FEATURE_GNSS_GERA_ENABLE
        #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
        tm_core_update_session_state(TM_CORE_SESS_STATE_RESOURCE_CLEANUP);

        fire_event = tm_core_info.session_info.session_event_firing;
        fire_nmea  = TRUE;

        if (tm_core_is_internal_session_on())
        {
          fire_event = FALSE;
          fire_nmea  = FALSE;

          /* Notify the GM if the HLOS has ended the session ( USER_TERMINATED ) or
             if the HLOS has started a new session in which case the internal session is terminated (ABORT_INTERNAL_SESSION)*/
          if ((abort_reason == TM_STOP_REASON_USER_TERMINATED) ||
              (abort_reason == TM_STOP_REASON_ABORT_INTERNAL_SESSION) ||
              (abort_reason == TM_STOP_REASON_INTERNAL_SESSION_DELETE_PARAM))
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sending abort reason %d to GM ", abort_reason, 0, 0);
            gm_position_fix_request_fail(GEOFENCE_POS_FIX_SRC_GNSS);
          }
        }

        if (NULL != pz_pd_event_type)
        {
          pz_pd_event_type->pd_event_data.pd_info_type.client_id = tm_core_info.session_info.pd_param.client_id;
          pz_pd_event_type->pd_event_data.pd_info_type.pd_info.end_status = PDSM_PD_END_SESS_NONE;

          /* Fire End event if fix does not complete successfully */
          if ((abort_reason != TM_STOP_REASON_COMPLETED) &&
              (fire_event == TRUE))
          {
            if (tm_core_info.session_info.pd_param.pd_option.session == PDSM_PD_SESS_TYPE_DATA_DLOAD)
            {
              pz_pd_event_type->e_pd_event = PDSM_PD_DLOAD_EVENT_END;
            }
            else
            {
              pz_pd_event_type->e_pd_event = PDSM_PD_EVENT_END;
            }
            pz_pd_event_type->pd_event_data.pd_info_type.pd_info.end_status = tm_core_translate_end_status(abort_reason);
            tm_pdapi_pd_event_callback(pz_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);
          }
          e_end_status = pz_pd_event_type->pd_event_data.pd_info_type.pd_info.end_status;
        }
        else
        {
          e_end_status = tm_core_abort_recover_send_pd_event_end(abort_reason, fire_event);
        }
        client_type = pdsmclient_get_client_type_map(tm_core_info.session_info.pd_param.client_id);

        /* if abort reason is delete parms, check if on demand is enabled. we need to force the receiver
        to go off before honoring the delete parms.
        if the user terminated the session, check if the client type is OEM. we
        need to turn off the rcvr off immediately */
        if ((abort_reason == TM_STOP_REASON_USER_TERMINATED) &&
            (client_type == PDSM_CLIENT_TYPE_OEM)
            )
        {
          tm_core_lm_sess_stop_req(tm_core_info.session_info.active_sess_handle, LM_SESSION_REQUEST_FIX, TRUE);
        }
        /* When we receive an abort from the protocols with ask_prtl as FALSE , we
           kill the app tracking logic in LM if requested */
        else if ((abort_reason == TM_STOP_REASON_USER_TERMINATED) &&
                 tm_core_info.session_info.receiver_off_needed)
        {
          tm_core_lm_sess_stop_req(tm_core_info.session_info.active_sess_handle, LM_SESSION_REQUEST_FIX, FALSE);
        }
        /* Clean up the session when we do CSFB from LTE to UMTS */
        else if ((TM_STOP_REASON_LTE_TO_UMTS_CSFB == abort_reason) &&
                 tm_core_info.session_info.receiver_off_needed)
        {
          tm_core_lm_sess_stop_req(tm_core_info.session_info.active_sess_handle, LM_SESSION_REQUEST_FIX, FALSE);
        }
        else if (TM_STOP_REASON_BACKGROUND_LPPM_CANNOT_ENGAGE == abort_reason)
        {
          tm_core_lm_sess_stop_req(tm_core_info.session_info.active_sess_handle, LM_SESSION_REQUEST_FIX, FALSE);
        }
        else if (TM_STOP_REASON_TASK_STOP == abort_reason)
        {
          /* When we receive TASK_STOP. Ensure MGP OFF is issued by setting the 
          cv_force_mgp_off flag to TRUE*/
          tm_core_lm_sess_stop_req(tm_core_info.session_info.active_sess_handle, LM_SESSION_REQUEST_FIX, TRUE);
        }
        /* for OTDOA only, LM is not involved. No need to start LM session */
        if ((tm_core_info.session_info.op_mode != TM_SESS_OPERATION_MODE_OTDOA) ||
            (tm_core_info.session_info.op_mode  == TM_SESS_OPERATION_MODE_OTDOA &&
             tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_UP_NI))
        {
          tm_core_lm_sess_stop_req(tm_core_info.session_info.active_sess_handle, LM_SESSION_REQUEST_SESSION, FALSE);
        }

        /*Generate PSTIS NMEA string indicating session end*/
        if ((TM_CORE_CONFIG_DD(nmea_output_format) & PDAPI_NMEA_PSTIS) &&
            (tm_core_info.config_info.nmea_config_type.pd_nmea_reporting_type == PDSM_PA_NMEA_REPORT_AT_1HZ_INTERVAL) &&
            (fire_nmea == TRUE))
        {
          tm_nmea_pstis_put();
        }

        /*Cleanup the cached measurement and ref position information*/
        tm_core_clear_cached_meas();
        tm_core_clear_cached_ref_pos();

        /* Fire EVENT_GPS_PD_FIX_END */
        sm_report_event_gps_fix_end(e_end_status);

        client_id  = tm_core_info.session_info.pd_param.client_id;

        /*Disable LDM client if concurrency NV is disabled */
        if ((tm_core_get_concurrency_support_config() & TM_E911_CONCURRENCY_ENABLE) == 0)
        {
          tm_lpc_disable_client_no_active_protocol();
        }

        /* Go back to clean state */
        tm_core_info.session_info.active_sess_handle      = TM_SESS_HANDLE_NONE;
        tm_core_info.session_info.op_mode                 = TM_SESS_OPERATION_MODE_NONE;
        tm_core_info.session_info.z_lppm_mode.e_PowerMode = TM_SESS_POWER_MODE_NONE;
        tm_core_info.session_info.z_lppm_mode.q_TbmMsecs  = 0;
        tm_core_info.session_info.op_requested            = TM_OP_REQ_NONE;
        tm_core_info.session_info.from_protocol           = TM_PRTL_TYPE_NONE;
        tm_core_info.session_info.pd_param.client_type    = PDSM_CLIENT_TYPE_NONE;
        tm_core_info.session_info.pd_param.client_id      = -1;
        tm_core_info.session_info.session_event_firing    = TRUE;
        tm_core_info.session_info.eph_svmask              = 0;
        tm_core_info.session_info.alm_svmask              = 0;
        tm_core_info.session_info.msb_xtra_allowed        = FALSE;
        tm_core_info.session_info.req_xtra_trig_by_agps   = FALSE;
        tm_core_info.session_info.end_odp_session         = FALSE;
        tm_core_info.session_info.force_eph_download      = FALSE;
        tm_core_info.session_info.glo_force_eph_download  = FALSE;
        tm_core_info.session_info.bds_force_eph_download  = FALSE;
        tm_core_info.session_info.gal_force_eph_download  = FALSE;
        tm_core_info.session_info.eph_need_sv_mask = 0;
        tm_core_info.session_info.glo_eph_need_sv_mask = 0;
        tm_core_info.session_info.bds_eph_need_sv_mask = 0;
        tm_core_info.session_info.gal_eph_need_sv_mask = 0;
        tm_core_info.session_info.sess_info_valid_flags = 0;

        tm_core_info.session_info.q_ReqClientMask         = SM_GPS_STATE_INFO_REQ_TYPE_NONE;
        tm_core_info.session_info.receiver_off_needed     = FALSE;
        tm_core_info.session_info.iono                    = 1; /* iono invalid, 1 means MGP has
                                                               requested iono from AGPS */
        tm_core_info.session_info.utc                     = 1; /* utc invalid, 1 means MGP has
                                                               requested utc from AGPS */
        tm_core_info.session_info.health_svmask           = 0;
        tm_core_info.session_info.valid_svmask            = 0;
        tm_core_info.session_info.sbas_eph_svmask         = 0;
        tm_core_info.session_info.sbas_alm_svmask         = 0;
        tm_core_info.session_info.glo_eph_svmask          = 0;
        tm_core_info.session_info.glo_alm_svmask          = 0;
        tm_core_info.session_info.bds_eph_svmask          = 0;
        tm_core_info.session_info.bds_alm_svmask          = 0;
        tm_core_info.session_info.gal_eph_svmask          = 0;
        tm_core_info.session_info.gal_alm_svmask          = 0;
        tm_core_info.session_info.navic_eph_svmask        = 0;
        tm_core_info.session_info.navic_alm_svmask        = 0;
        tm_core_info.session_info.v_session_started_for_e911 = FALSE;
        tm_core_info.internal_session = FALSE;
        tm_core_info.session_info.q_num_fixes             = 0;

        tm_core_update_session_state(TM_CORE_SESS_STATE_INIT);

        /* Fire Done event */
        if (NULL != pz_pd_event_type)
        {
          if (TRUE == fire_event)
          {
            pz_pd_event_type->pd_event_data.pd_info_type.client_id = client_id;
            if (PDSM_PD_SESS_TYPE_DATA_DLOAD == tm_core_info.session_info.pd_param.pd_option.session)
            {
              pz_pd_event_type->e_pd_event = PDSM_PD_EVENT_DLOAD_DONE;
              tm_pdapi_pd_event_callback(pz_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);
            }
            else
            {
              pz_pd_event_type->e_pd_event = PDSM_PD_EVENT_DONE;
              tm_pdapi_pd_event_callback(pz_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);

              /* Check prtl_func_cb_table bounds */
              if ((prtlTableIndex >= 0) && (prtlTableIndex < TM_PRTL_NUM))
              {
                /* This is added for internal Vx NI client */
                if (NULL != tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp)
                {
                  tm_core_inform_pd_event(tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp,
                                          PDSM_PD_EVENT_DONE);
                }
              }
              else
              {
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Attempt for an out of bound access to prtl_func_cb_table with index = %d", prtlTableIndex, 0, 0);
              }
            if (b_emerg_sess_start_sent == TRUE)
            {
              tm_core_send_pdapi_emerg_sess_type(PDSM_PD_EXT_STATUS_EMERG_SESS_DONE);
              b_emerg_sess_start_sent = FALSE;
            }
            }

          }
          if ((TRUE == fire_event) ||
              (TM_STOP_REASON_INTERNAL_SESSION_DELETE_PARAM == abort_reason)
              )
          {
            /*Send Done event to GM core as well*/
            pz_pd_event_type->pd_event_data.pd_info_type.client_id = client_id;
            pz_pd_event_type->e_pd_event = PDSM_PD_EVENT_DLOAD_DONE;
            gm_update_pd_event(pz_pd_event_type->e_pd_event, abort_reason);
          }
        }
        else
        {
          tm_core_abort_recover_send_pd_event_done(abort_reason, fire_event, client_id, prtlTableIndex);
        }
#ifdef FEATURE_GNSS_GERA_ENABLE
        #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

        techSel_ReportPosSessionStop(&z_sess_info, abort_reason);
        if (tm_core_info.session_info.queued_request == TRUE)
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Resume pending get position fix request", 0, 0, 0);
          (void)tm_core_resume_get_pos();
        }
      } /* If(ask_prtl == FALSE) */
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM-Core not in active state", 0, 0, 0);

#ifdef FEATURE_GNSS_GERA_ENABLE
    #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

    /* send the end session to all protocols. for UMTS-CP, we need to reset the allow-rrc bit in the cases
           where we havnet received the MC yet for either 3rd party mo-lr or MT sessions*/
    for (prtlTableIndex = 0; prtlTableIndex < TM_CORE_PRTL_CB_ARR_INDEX_MAX; prtlTableIndex++)
    {

      /* If an abort request is coming from the user, send it to protocol modules */
      if (abort_reason == TM_STOP_REASON_USER_TERMINATED || abort_reason == TM_STOP_REASON_ON_RESET_LOCATION_SERVICE ||
          abort_reason == TM_STOP_REASON_TASK_STOP)
      {
        /* Set reason to general error for now, if return value */
        if (tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
        {
          prtl_abort_session = tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp(tm_core_info.session_info.active_sess_handle,
                                                                                                PRTL_SESS_STOP_SINGLE_FIX,
                                                                                                abort_reason);
          if (prtl_abort_session == TRUE)
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Protocol aborted the session", 0, 0, 0);
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Protocol continue with the session", 0, 0, 0);
          }
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No protocol callback installed 3", 0, 0, 0);
        }
      }
    }
    if (abort_reason == TM_STOP_REASON_USER_TERMINATED || abort_reason == TM_STOP_REASON_ON_RESET_LOCATION_SERVICE ||
        abort_reason == TM_STOP_REASON_TASK_STOP)
    {
      /* inform LM to turn off the receiver if user sent an end session */
      /*Do Force Receiver off only if OEM client. Else to normal receiver off*/
      if (TM_STOP_REASON_TASK_STOP != abort_reason)
      {
        tm_core_req_lm_recvr_off();
      }
      else
      {
        /* When we receive TASK_STOP. Ensure MGP OFF is issued by setting the 
        cv_force_mgp_off flag to TRUE*/
        tm_core_lm_sess_stop_req(tm_core_info.session_info.active_sess_handle, LM_SESSION_REQUEST_FIX, TRUE);
      }

      if (NULL != pz_pd_event_type)
      {
        pz_pd_event_type->pd_event_data.pd_info_type.client_id = tm_core_info.session_info.pd_param.client_id;
        if (tm_core_info.session_info.pd_param.pd_option.session == PDSM_PD_SESS_TYPE_DATA_DLOAD)
        {
          pz_pd_event_type->e_pd_event = PDSM_PD_DLOAD_EVENT_END;
        }
        else
        {
          pz_pd_event_type->e_pd_event = PDSM_PD_EVENT_END;
        }
        pz_pd_event_type->pd_event_data.pd_info_type.pd_info.end_status = tm_core_translate_end_status(abort_reason);
        tm_pdapi_pd_event_callback(pz_pd_event_type, TM_PDAPI_SESS_TYPE_NONE);
      }
      else
      {
        tm_core_abort_recover_send_pd_event_end(abort_reason, fire_event);
      }
    }

    /*If any of the NMEA strings are enabled, generate the proprietary NMEA message*/
    if ((TM_CORE_CONFIG_DD(nmea_output_format) != 0) &&
        (tm_core_info.config_info.nmea_config_type.pd_nmea_reporting_type == PDSM_PA_NMEA_REPORT_AT_1HZ_INTERVAL))
    {
      /*Generate PSTIS NMEA string indicating session start*/
      if (TM_CORE_CONFIG_DD(nmea_output_format) & PDAPI_NMEA_PSTIS)
      {
        tm_nmea_pstis_put();
      }
    }

    /*Cleanup the cached measurement and ref position information*/
    tm_core_clear_cached_meas();
    tm_core_clear_cached_ref_pos();

    /*Disable LDM client if concurrency NV is disabled */
    if ((tm_core_get_concurrency_support_config() & TM_E911_CONCURRENCY_ENABLE) == 0)
    {
      tm_lpc_disable_client_no_active_protocol();
    }

    /* Go back to clean state */
    tm_core_info.session_info.active_sess_handle      = TM_SESS_HANDLE_NONE;
    tm_core_info.session_info.op_mode                 = TM_SESS_OPERATION_MODE_NONE;
    tm_core_info.session_info.z_lppm_mode.e_PowerMode = TM_SESS_POWER_MODE_NONE;
    tm_core_info.session_info.z_lppm_mode.q_TbmMsecs  = 0;
    tm_core_info.session_info.op_requested            = TM_OP_REQ_NONE;
    tm_core_info.session_info.from_protocol           = TM_PRTL_TYPE_NONE;
    tm_core_info.session_info.pd_param.client_type    = PDSM_CLIENT_TYPE_NONE;
    tm_core_info.session_info.pd_param.client_id      = -1;
    tm_core_info.session_info.eph_svmask              = 0;
    tm_core_info.session_info.alm_svmask              = 0;
    tm_core_info.session_info.q_ReqClientMask         = SM_GPS_STATE_INFO_REQ_TYPE_NONE;
    tm_core_info.session_info.msb_xtra_allowed        = FALSE;
    tm_core_info.session_info.req_xtra_trig_by_agps   = FALSE;
    tm_core_info.session_info.receiver_off_needed     = FALSE;
    tm_core_info.session_info.end_odp_session         = FALSE;
    tm_core_info.session_info.force_eph_download      = FALSE;
    tm_core_info.session_info.glo_force_eph_download  = FALSE;
    tm_core_info.session_info.bds_force_eph_download  = FALSE;
    tm_core_info.session_info.gal_force_eph_download  = FALSE;
    tm_core_info.session_info.eph_need_sv_mask = 0;
    tm_core_info.session_info.glo_eph_need_sv_mask = 0;
    tm_core_info.session_info.bds_eph_need_sv_mask = 0;
    tm_core_info.session_info.gal_eph_need_sv_mask = 0;
    tm_core_info.session_info.sess_info_valid_flags = 0;
    tm_core_info.session_info.v_session_started_for_e911 = FALSE;
    tm_core_info.internal_session = FALSE;
    tm_core_info.session_info.q_num_fixes             = 0;
    b_emerg_sess_start_sent = FALSE;
    tm_core_update_session_state(TM_CORE_SESS_STATE_INIT);

#ifdef FEATURE_GNSS_GERA_ENABLE
    #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

  }

  if (NULL != pz_pd_event_type)
  {
    (void)os_MemFree((void **)&pz_pd_event_type);
  }
}

static lm_mo_mt_mode_e_type tm_core_mo_mt_mode(uint32 q_sessHandle)
{

  /* LPP CP Session handle may conflict with the MO session handle if included in the next if condition, hence check seperately */
  if (q_sessHandle == TM_SESS_HANDLE_LPP_CP_NI)
  {
    return LM_MO_MT_MODE_MT;
  }
  /* Set the call type to MO/MT based on the session handle. E911 session type should be considered as MT so the HEPE checks at
   timeout are ignored */
  if (0 != (q_sessHandle &
               (TM_SESS_HANDLE_1X_CP_MT | TM_SESS_HANDLE_1X_UP_NI |
                   TM_SESS_HANDLE_UMTS_CP_NI | TM_SESS_HANDLE_UMTS_UP_NI |
                   TM_SESS_HANDLE_E911)))
  {
    return LM_MO_MT_MODE_MT;
  }

  return LM_MO_MT_MODE_MO;
}

/*===========================================================================

FUNCTION tm_core_convert_op_mode

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
lm_gps_mode_e_type tm_core_convert_op_mode(tm_sess_operation_mode_e_type op_mode,
                                           tm_prtl_type                  prtl_type)
{

  tm_cm_phone_state_info_s_type phone_state_info;
  cgps_SrvSystemType selected_srv_system = CGPS_SRV_SYS_OTHER;

  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  /* Based on the UE mode (emergency or VAS) select the correct serving system field */
  if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
  {
    if ((CGPS_SRV_SYS_WLAN == tm_ds_get_emergency_srv_sys()) ||
        (CGPS_SRV_SYS_WLAN == phone_state_info.emerg_conn_srv_system))
    {
      selected_srv_system = CGPS_SRV_SYS_WLAN;
    }
    else
    {
      selected_srv_system = phone_state_info.emerg_srv_system;
    }
  }
  else
  {
    selected_srv_system = phone_state_info.srv_system;
  }

  switch (op_mode)
  {
    case TM_SESS_OPERATION_MODE_MSA:
      if ((
           (tm_cm_iface_is_serving_system_avail(CGPS_SRV_SYS_CDMA)) ||
           (tm_cm_iface_is_serving_system_avail(CGPS_SRV_SYS_HDR))
           ) &&
          (
           (prtl_type == TM_PRTL_TYPE_1X_CP) ||
           (prtl_type == TM_PRTL_TYPE_1X_UP)
           )
          )
      {
        /*If 1x is available and protocol for this sesstion is 1x,
          then use 1x_MSA*/
        return LM_GPS_MODE_1X_MSA;
      }
      else if ((selected_srv_system == CGPS_SRV_SYS_WCDMA) ||
               (selected_srv_system == CGPS_SRV_SYS_GSM) ||
               (selected_srv_system == CGPS_SRV_SYS_LTE) ||
               (selected_srv_system == CGPS_SRV_SYS_TDSCDMA) ||
               (selected_srv_system == CGPS_SRV_SYS_WLAN)
               )
      {
        return LM_GPS_MODE_MSA;
      }
      else if (selected_srv_system == CGPS_SRV_SYS_NR5G)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "5GNR system, add relevant changes");
        return LM_GPS_MODE_NONE;
      }

      else
      {
        return LM_GPS_MODE_1X_MSA;
      }
#ifdef FEATURE_GNSS_PERIODIC_PPM
    #error code not present
#endif
    case TM_SESS_OPERATION_MODE_MSB:
    case TM_SESS_OPERATION_MODE_MSB_OPT:
      return LM_GPS_MODE_MSB;

    case TM_SESS_OPERATION_MODE_STANDALONE:
      return LM_GPS_MODE_STANDALONE;

    case TM_SESS_OPERATION_MODE_FTM:
      return LM_GPS_MODE_FTM;

    case TM_SESS_OPERATION_MODE_ODP:
      return LM_GPS_MODE_ODP;

    case TM_SESS_OPERATION_MODE_OTDOA:
      return LM_GPS_MODE_MSA;
    default:
      return LM_GPS_MODE_NONE;
  }
}

/*===========================================================================

FUNCTION tm_core_map_dm_event_op_mode

DESCRIPTION
  This function maps correct op mode for reporting to QXDM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
sm_event_pd_fix_start_payload_e_type tm_core_map_dm_event_op_mode
(
   tm_sess_operation_mode_e_type op_mode
   )
{
  switch (op_mode)
  {
    case TM_SESS_OPERATION_MODE_MSA:
    case TM_SESS_OPERATION_MODE_OTDOA:
      return MS_ASSISTED_OP;

    default:
      return MS_BASED_OP;
  }
}

/*===========================================================================

FUNCTION tm_core_fix_time_get

DESCRIPTION
  This function gets Fix Time information in the extended PD INFO buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_core_fix_time_get(const sm_NavPositionStructType *nav_ptr)
{
  uint32 q_Msecs, q_Hours, q_Mins, q_Secs, q_Tenths;


  if ((nav_ptr == NULL) || (nav_ptr->q_UtcTimeMs == 0L))
  {
    qword           milliseconds;
    qword           timestamp;
    me_Clock        z_Clock;
    uint32          UtcFixTime, gpsLeapSec;

    (void)time_get(timestamp);
    gps_total_msec(timestamp, milliseconds);
    gps_week_ms(milliseconds, &(z_Clock.z_Time.w_GpsWeek),
                &(z_Clock.z_Time.q_GpsMsec));
    /* Check for week rollover */
    gps_MsecWeekLimit((int32 *)&z_Clock.z_Time.q_GpsMsec, &z_Clock.z_Time.w_GpsWeek);
    gpsLeapSec =   cgps_GetNvUtcOffset(); /* See the above warning */

    if (z_Clock.z_Time.q_GpsMsec / 1000L < gpsLeapSec)
    {
      UtcFixTime = z_Clock.z_Time.q_GpsMsec +
         (WEEK_SECS - gpsLeapSec) * 1000L;
    }
    else
    {
      UtcFixTime = z_Clock.z_Time.q_GpsMsec - gpsLeapSec * 1000L;
    }
    q_Msecs    = UtcFixTime % DAY_MSECS;
  }
  else
  {
    q_Msecs    = nav_ptr->q_UtcTimeMs % DAY_MSECS;
  }

  /* Break into hours, mins, secs and secs/10 */
  q_Hours     = q_Msecs / HOUR_MSECS;
  q_Msecs     -= q_Hours * HOUR_MSECS;

  q_Mins      = q_Msecs / MIN_MSECS;
  q_Msecs     -= q_Mins * MIN_MSECS;

  q_Secs      = q_Msecs / SEC_MSECS;
  q_Msecs     -= q_Secs * SEC_MSECS;

  q_Tenths    = q_Msecs / TENTH_MSECS;


  tm_core_info.pd_ext_data.hour   = (uint8)q_Hours;
  tm_core_info.pd_ext_data.minute = (uint8)q_Mins;
  tm_core_info.pd_ext_data.sec    = (uint16)(q_Secs * 100 + q_Tenths * 10);

} /* tm_core_fix_time_get */


/*===========================================================================

FUNCTION tm_core_date_get

DESCRIPTION
  This function gets Date information in the
  NMEA Extended PD INFO buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_core_date_get(const sm_NavPositionStructType *nav_ptr)
{
  uint32 q_UtcDays, q_FourYears, q_Month,  q_Year;
  uint16 w_UtcWn;
  const uint8 *p_DaysInMonth;

  q_UtcDays   = 0;
  q_Month     = 0;
  q_Year      = 0;

  if (nav_ptr != NULL)
  {
    /* UTC time and GPS time are generally in the same week. Handle the
       13 to 14 second time span when this is not actually the case. GPS
       time leads UTC time (at least in our lifetimes), so checking for
       UTC bigger than GPS is sufficient */

    w_UtcWn = nav_ptr->w_GpsWeek;

    if (nav_ptr->q_GpsTimeMs < nav_ptr->q_UtcTimeMs)
    {
      w_UtcWn--;
    }

    q_UtcDays = (7 * w_UtcWn) + (nav_ptr->q_UtcTimeMs / DAY_MSECS);

    /* This is the number of days since the GPS clock started on
       Jan 6th, 1980. Add 5 days to make day 0 == Jan 1st */

    q_UtcDays += 5;

    /* How many 4 year cycles have occured. */
    q_FourYears = q_UtcDays / (4 * 365 + 1);

    /* q_UtcDays represents the # of days into this 4 year period */
    q_UtcDays   -= q_FourYears * (4 * 365 + 1);

    q_Year = 1980 + q_FourYears * 4;

    /* Day 0 to 365 of q_FourYears is in the leap year */
    if (q_UtcDays >= 366)
    {
      uint32 q_YearsInPeriod;

      /* This year is not a leap year */
      q_Year    += 1;
      q_UtcDays -= 366;

      q_YearsInPeriod = q_UtcDays / 365;
      q_Year    += q_YearsInPeriod;
      q_UtcDays -= q_YearsInPeriod * 365;

      p_DaysInMonth = u_DaysInMonth;
    }
    else
    {
      /* This year is a leap year */
      p_DaysInMonth = u_DaysInMonthLeap;
    }

    /* Subtract days in months for this year. */
    for (q_Month = 1;
         q_Month < 12 && q_UtcDays >= *p_DaysInMonth; q_Month++)
    {
      q_UtcDays -= *p_DaysInMonth;
      p_DaysInMonth++;
    }

    /* Need to add 1 to account for the fact the days in month
       start at 1 and not 0 */
    q_UtcDays++;
  }

  tm_core_info.pd_ext_data.date = (uint32)((q_UtcDays * 1000000) + (q_Month * 10000) + q_Year);

} /* tm_core_date_get */

/*===========================================================================

FUNCTION tm_core_pos_get

DESCRIPTION
  This function update the GPS Position information in the
  NMEA Extended PD INFO buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_core_pos_get(const sm_NavPositionStructType *nav_ptr)
{
  int64 d_Lat = 0;
  int64 d_Lon = 0;
  FLT   f_MagDev = 0.0;

  if (nav_ptr == NULL)
  {
    tm_core_info.pd_ext_data.lat   = d_Lat;
    tm_core_info.pd_ext_data.lon   = d_Lon;
    tm_core_info.pd_ext_data.magnetic_variation = (int64)f_MagDev;
  }
  else
  {
    d_Lat  = (int64)(nav_ptr->d_PosLla[0] * (180.0 / C_PI) * (double)PDSM_LATLONG_SCALE + 0.5);
    d_Lon  = (int64)(nav_ptr->d_PosLla[1] * (180.0 / C_PI) * (double)PDSM_LATLONG_SCALE + 0.5);

    tm_core_info.pd_ext_data.lat = d_Lat;
    tm_core_info.pd_ext_data.lon = d_Lon;

    if (nav_ptr->u_MagDevGood == TRUE)
    {
      tm_core_info.pd_ext_data.magnetic_variation = (int64)(nav_ptr->f_MagDeviation * 10.0 + 0.5);  /* [x10 deg] */
    }
    else
    {
      /* If Mag Dev is not available, attempt to compute it */
      if (cgps_MagDeviation(nav_ptr->d_PosLla[0], nav_ptr->d_PosLla[1], &f_MagDev) == TRUE)
      {
        tm_core_info.pd_ext_data.magnetic_variation = (int64)(f_MagDev * 10.0 + 0.5);
      }
      else
      {
        tm_core_info.pd_ext_data.magnetic_variation = 0;
      }
    }

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Position: MagDev: %d, MagDevGood: %d",
          tm_core_info.pd_ext_data.magnetic_variation,
          nav_ptr->u_MagDevGood, 0);
  }

} /* tm_core_pos_get */

/*===========================================================================

FUNCTION tm_core_velocity_get

DESCRIPTION
  This function udpates the velocity information in the
  NMEA Extended PD INFO buffer.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_core_velocity_get(const sm_GpsFixRptStructType *fix_ptr)
{
  double  d_NorthVel, d_EastVel, d_Knots, d_Speed, d_Azimuth;
  const sm_NavPositionStructType *nav_ptr = &(fix_ptr->z_NavPos);
  uint8 loop_count = 0;

  if ((nav_ptr == NULL) ||
      !nav_ptr->z_PosFlags.b_IsVelValid)
  {
    tm_core_info.pd_ext_data.velocity_include   = 0;
    tm_core_info.pd_ext_data.speed_knot         = 0;
    tm_core_info.pd_ext_data.speed_kmh          = 0;
    tm_core_info.pd_ext_data.heading_true       = 0;
    tm_core_info.ext_opt_field_mask             = 0;
  }
  else
  {
    tm_core_info.pd_ext_data.velocity_include = 1;

    tm_util_calculate_heading(fix_ptr, &d_Azimuth);

    d_EastVel  = nav_ptr->f_VelEnu[0];
    d_NorthVel = nav_ptr->f_VelEnu[1];

    /* East, North, Up velocity in m/s  */
    for (loop_count = 0; loop_count < 3; loop_count++)
    {
      tm_core_info.pd_ext_data.f_VelocityENUMps[loop_count] = nav_ptr->f_VelEnu[loop_count];
    }

    /* East, North, Up velocity uncertainty in m/s  */
    for (loop_count = 0; loop_count < 2; loop_count++)
    {
      tm_core_info.pd_ext_data.f_VelocityUncENUMps[loop_count] = nav_ptr->f_VuncEastNorthMps[loop_count];
    }

    tm_core_info.pd_ext_data.f_VelocityUncENUMps[2] = nav_ptr->f_VuncVertMps;

    tm_core_info.pd_ext_data.heading_true = (uint16)(d_Azimuth * 10.0 + 0.5);        /* [x10 deg] */

    if (fix_ptr->z_NavPos.z_PosFlags.b_IsHeadingValid)
    {
      tm_core_info.ext_opt_field_mask = PDSM_PD_HEADING_VALID;
    }

    d_Speed = sqrt(d_NorthVel * d_NorthVel + d_EastVel * d_EastVel);
    if (d_Speed > TM_CORE_MIN_VELOCTY)
    {
      d_Knots = d_Speed * (3600.0 / 1852.0);

      /* Azimuth moves from north (0) to east (90), wrapping around.
       * Note that we are ignoring the vertical velocity.
      */

      tm_core_info.pd_ext_data.speed_knot   = (uint32)(d_Knots * 10.0 + 0.5);          /* [x10 knots] */
      tm_core_info.pd_ext_data.speed_kmh    = (uint32)((d_Speed * 3.6) * 10.0 + 0.5);  /* [x10 km/h]  */
    }
    else  /* Speed is too slow for a meaningful course. */
    {
      tm_core_info.pd_ext_data.speed_knot = 0;
      tm_core_info.pd_ext_data.speed_kmh  = 0;
    }
  }
} /* tm_core_velocity_get */


/*===========================================================================

FUNCTION tm_sv_in_view_get

DESCRIPTION
  This function updates the SV in view info in the NMEA Extended PD INFO buffer.
  SVs of all constellations that MGP is using must be updated.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tm_sv_in_view_get(gnss_MeasBlkStructType *p_MeasBlk, uint8 u_MaxChan)
{
  uint8  u_i;        /* u_i iterates over satellites stored in the meas blk structures in tm_core_info */
  uint8  u_Chan;   /* u_Chan iterates over all satellites stored in one of the meas blk structures in tm_core_info */
  uint32 mask;
  pdsm_sv_inview_s_type      *p_Sv      = NULL;
  gnss_MeasStructType    *p_zMeas = NULL;

  p_zMeas = p_MeasBlk->z_Gnss;
  u_i = tm_core_info.pd_ext_data.num_sv_in_view;  // current index

  for (u_Chan = 0; (u_Chan < p_MeasBlk->z_GnssHeader.u_NumSvs && u_Chan < u_MaxChan) && (u_i < PDSM_GNSS_MAX_SATELLITES); u_Chan++)
  {
    mask = p_zMeas[u_Chan].q_MeasStatus & (SM_MEAS_STATUS_INVALID_FLAGS | SM_MEAS_STATUS_SM_VALID);

    if (mask == SM_MEAS_STATUS_SM_VALID)
    {
      p_Sv = &tm_core_info.pd_ext_data.sv[u_i];
      /* PRN, elevation, azimuth and SNR (C/No): */
      p_Sv->sv_num       = p_zMeas[u_Chan].w_Sv;

      if (p_zMeas[u_Chan].u_SvDirValid && (p_zMeas[u_Chan].f_Elevation > 0))
      {
        p_Sv->elev         = (uint8)(p_zMeas[u_Chan].f_Elevation * C_RAD_TO_DEG);

        /* azimuth_snr contains azimuth + snr infomation, multiply azimuth by 100 and add SNR */
        /* azimuth can be obtained by doing azimuth_snr / 100 and snr can be obtained by doing azimuth % 100 */
        p_Sv->azimuth_snr  = (uint16)(p_zMeas[u_Chan].f_Azimuth * C_RAD_TO_DEG) * 100;

        if (p_zMeas[u_Chan].w_Cno)
        {
          p_Sv->azimuth_snr += (uint16)(p_zMeas[u_Chan].w_Cno / 10);
        }
      }
      else
      {
        p_Sv->elev  = 0;
        p_Sv->azimuth_snr = 0;
      }

      u_i++;
    }
  }

  tm_core_info.pd_ext_data.num_sv_in_view = u_i;

} /* tm_sv_in_view_get() */


/* Populates Jammer Indicator List from Position Report Data */
static boolean tm_core_populate_jammer_indicator(
   const sm_NavPositionStructType *p_NavPos,
   pdsm_gnss_jammerIndicatorStructType *p_JamIndList)
{
  uint8 u_I;
  pdsm_LocGnssSignalEnumType e_Signal;

  if (NULL == p_NavPos || NULL == p_JamIndList)
  {
    TM_ERROR_2("tm_core_populate_jammer_indicator NULL input 0x%X 0x%X",
               p_NavPos, p_JamIndList);
    return FALSE;
  }

  /* Initialize the AGC and BP to INT32_MAX */
  for (u_I = 0; u_I < PD_LOC_GNSS_SIGNAL_TYPE_MAX; u_I++)
  {
    p_JamIndList[u_I].l_agcMetricDb = INT32_MAX;
    p_JamIndList[u_I].l_bpMetricDb = INT32_MAX;
  }

  for (u_I = 0; u_I < GNSS_GMB_IDX_TOTAL; u_I++)
  {
    switch (u_I)
    {
      case GNSS_GMB_IDX_GPS:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GPS_L1CA;
        break;

      case GNSS_GMB_IDX_QZSS_SBAS:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L1CA;
        break;

      case GNSS_GMB_IDX_GLO:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G1;
        break;

      case GNSS_GMB_IDX_BDS:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B1_I;
        break;

      case GNSS_GMB_IDX_GAL:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E1_C;
        break;

      case GNSS_GMB_IDX_GPS_L2C:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GPS_L2C_L;
        break;

      case GNSS_GMB_IDX_GPS_L5Q:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GPS_L5_Q;
        break;

      case GNSS_GMB_IDX_GLO_G2:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GLONASS_G2;
        break;

      case GNSS_GMB_IDX_BDS_B2:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2_I;
        break;

      case GNSS_GMB_IDX_BDS_B2A:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_BEIDOU_B2A_Q;
        break;

      case GNSS_GMB_IDX_GAL_E5A:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_GALILEO_E5A_Q;
        break;

      case GNSS_GMB_IDX_QZSS_L2C:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L2C_L;
        break;

      case GNSS_GMB_IDX_QZSS_L5Q:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_QZSS_L5_Q;
        break;

      case GNSS_GMB_IDX_NAVIC_L5:
        e_Signal = PD_LOC_GNSS_SIGNAL_TYPE_NAVIC_L5;
        break;

      default:
        continue;
    }

    p_JamIndList[e_Signal].l_agcMetricDb = p_NavPos->l_AgcMetric[u_I];
    p_JamIndList[e_Signal].l_bpMetricDb = p_NavPos->l_BpJammerMetric[u_I];
  }
  return TRUE;
}

/*===========================================================================

FUNCTION tm_core_generate_ext_pd_data

DESCRIPTION
  This function generates extended pd data

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_generate_ext_pd_data(const sm_FixInfoStructType *fix_ptr)
{
  memset((void *)&tm_core_info.pd_ext_data, 0, sizeof(pdsm_pd_data_ext_s_type));
  tm_core_info.ext_opt_field_mask = 0;

  tm_core_fix_time_get(&(fix_ptr->z_NavPos));

  tm_core_date_get(&(fix_ptr->z_NavPos));

  if (fix_ptr == NULL)
  {
    tm_core_info.pd_ext_data.fix_type       = PDSM_PD_FIX_TYPE_UNKNOWN;
    tm_core_info.pd_ext_data.fix_quality    = PDSM_PD_FIX_QUALITY_UNKNOWN;
    tm_core_info.pd_ext_data.selection_type = PDSM_PD_FIX_SELECTION_UNKNOWN;
  }
  else
  {
    /* Update Lat/Long */
    tm_core_pos_get(&(fix_ptr->z_NavPos));

    /* Height Above Ellipsoid (HAE) */
    tm_core_info.pd_ext_data.altitude = (int32)(fix_ptr->z_NavPos.d_PosLla[2] * 10.0 + 0.5);

    /* Velocity estimate */
    tm_core_velocity_get(fix_ptr);

    /* 1 - no fix, 2 - 2D, 3 - 3D */

    if (fix_ptr->z_NavPos.z_PosFlags.b_Is3D)
    {
      tm_core_info.pd_ext_data.fix_type = PDSM_PD_FIX_TYPE_3D;
    }
    else
    {
      tm_core_info.pd_ext_data.fix_type = PDSM_PD_FIX_TYPE_2D;
    }

    /* hdop, pdop, vdop [x10] */
    tm_core_info.pd_ext_data.h_dop = (uint32)(fix_ptr->z_NavPos.f_HDOP * 10.0 + 0.5);
    tm_core_info.pd_ext_data.p_dop = (uint32)(fix_ptr->z_NavPos.f_PDOP * 10.0 + 0.5);
    tm_core_info.pd_ext_data.v_dop = (uint32)(fix_ptr->z_NavPos.f_VDOP * 10.0 + 0.5);
    tm_core_info.pd_ext_data.t_dop = (uint32)(fix_ptr->z_NavPos.f_TDOP * 10.0 + 0.5);
    tm_core_info.pd_ext_data.g_dop = (uint32)(fix_ptr->z_NavPos.f_GDOP * 10.0 + 0.5);  /* Calcullation done as in case of HDOP */

    //<<<[???  by Liao] How do we get this info??? Hardcode it now

    /* Manual or Auto 2D/3D mode */
    //pe_ConfigGet( &z_peConfig );

    //if (z_peConfig.u_AltitudeHold == PE_ALT_FILTERED ||
    //    z_peConfig.u_AltitudeHold == PE_ALT_HOLD_AUTO  )
    //{
    //  tm_core_info.pd_ext_data.selection_type = PDSM_PD_FIX_SELECTION_AUTO;
    //}
    //else
    //{
    //  tm_core_info.pd_ext_data.selection_type = PDSM_PD_FIX_SELECTION_MANUAL;
    //}
    tm_core_info.pd_ext_data.selection_type = PDSM_PD_FIX_SELECTION_AUTO;
    //>>>[??? End]

    /* GPS quality issues */
    if (fix_ptr->z_NavPos.z_PosFlags.b_IsValid)
    {
      //      if ((fix_ptr->z_NavPos.z_PosFlags..w_PositionFlags & NF_DIFFERENTIAL) ||
      //         /* Set the MS-Assisted Fixes as DGPS fixes also */
      //          (fix_ptr->z_NavSolution.w_PositionFlags & NF_EXTERNAL_UPDATE))
      {
        tm_core_info.pd_ext_data.fix_quality = PDSM_PD_FIX_QUALITY_DGPS;  /* DGPS fix */
      }
      //      else
      {
        //        tm_core_info.pd_ext_data.fix_quality = PDSM_PD_FIX_QUALITY_GPS;  /* Non DGPS fix */
      }
    }
    else
    {
      tm_core_info.pd_ext_data.fix_quality = PDSM_PD_FIX_QUALITY_UNKNOWN; /* No fix at all */
    }
  } /* fix available? */

  /* Populate pd_ext_data's SV info array */
  tm_core_info.pd_ext_data.num_sv_in_view = 0;
  tm_sv_in_view_get(&tm_core_info.gps_meas_blk, N_ACTIVE_GPS_CHAN);
  tm_sv_in_view_get(&tm_core_info.glo_meas_blk, N_ACTIVE_GLO_CHAN);
  tm_sv_in_view_get(&tm_core_info.bds_meas_blk, N_ACTIVE_BDS_CHAN);
  tm_sv_in_view_get(&tm_core_info.qzss_meas_blk, N_ACTIVE_CHAN);
  tm_sv_in_view_get(&tm_core_info.gal_meas_blk, N_ACTIVE_GAL_CHAN);
  tm_sv_in_view_get(&tm_core_info.navic_meas_blk, N_ACTIVE_NAVIC_CHAN);

  tm_core_info.pd_ext_data.v_JammerIndicatorListValid =
     tm_core_populate_jammer_indicator(&fix_ptr->z_NavPos, tm_core_info.pd_ext_data.z_JammerIndicatorList);
}


static lm_wishlist_src_req_e_type tm_core_xlate_prtl_to_wishlist_src
(
   tm_sess_operation_mode_e_type op_mode
   )
{
  switch (op_mode)
  {
    /* Stateful query for MSB */
    case(TM_SESS_OPERATION_MODE_MSB):
      return LM_ASSIST_DATA_REQ_MSB;

      /* Always a stateless query everything else */
    case(TM_SESS_OPERATION_MODE_STANDALONE):
    case(TM_SESS_OPERATION_MODE_ODP):
    case(TM_SESS_OPERATION_MODE_MSA):
    case(TM_SESS_OPERATION_MODE_OTDOA):
    case(TM_SESS_OPERATION_MODE_NONE):
    case(TM_SESS_OPERATION_MODE_MSB_OPT):
    case(TM_SESS_OPERATION_MODE_FTM):
    case(TM_SESS_OPERATION_MODE_PERIODIC_PPM):
    default:
      return LM_ASSIST_DATA_REQ_QUERY;
  }
}

static boolean tm_core_sess_req_start_update_lm_info_data
(
   lm_gps_mode_e_type      e_op_mode,
   cgps_SrvSystemType      srv_system
   )
{
  lm_info_s_type  lm_info_data;

  /* tell MGP to start LTE time transfer need API from Bora */
  lm_info_data.e_info_type = LM_SESSION_INFO_SESSION_TYPE;
  lm_info_data.z_info.z_sess_info.e_mode = e_op_mode;
  lm_info_data.z_info.z_sess_info.e_srv_system = srv_system;

  return lm_info(&lm_info_data);
}

/*===========================================================================
    
FUNCTION tm_core_start_CPI
    
DESCRIPTION
  This function starts CPI request when E911 client is added as main or 
  secondary.
      
RETURN VALUE
  None
    
DEPENDENCIES
  None
    
SIDE EFFECTS
    
===========================================================================*/
void tm_core_start_CPI(tm_prtl_type prtl_type, 
                       tm_sess_handle_type sess_handle, 
                       pdsm_pd_position_special_req_e_type e_specialReq)
{
#ifdef FEATURE_GNSS_LPPE
  /* Update the AuxTech with correct client-id to get the best pos report in the correct
     protocol module */
  if  ((TM_PRTL_TYPE_UMTS_UP == prtl_type) && tm_core_lppe_up_precondition() &&
       tm_core_lppe_up_dbh_enabled())
  {
    if (tm_startCPIRequest(TRUE, TM_AUXTECH_REQ_ID_LPPE_UP))
    {
      le_TrackPosConsistencyStartStop(TRUE);
    }
  }
  else if ((TM_PRTL_TYPE_LPP_CP == prtl_type) && tm_core_lppe_cp_precondition() &&
           tm_core_lppe_cp_dbh_enabled())
  {
    if (tm_startCPIRequest(TRUE, TM_AUXTECH_REQ_ID_LPPE_CP))
    {
      le_TrackPosConsistencyStartStop(TRUE);
    }
  }
  else if ((TM_SESS_HANDLE_E911 == sess_handle) && tm_core_lppe_dbh_enabled())
  {
    if (tm_startCPIRequest(TRUE, TM_AUXTECH_REQ_ID_LPPE))
    {
      le_TrackPosConsistencyStartStop(TRUE);
    }
  }
#endif

#if !defined ( FEATURE_GNSS_SA ) && !defined ( FEATURE_GNSS_LEAN_BUILD )
  if ( PDSM_SESSION_SPECIAL_REQ_SHORT_CODES == e_specialReq )
  {
    if (tm_startCPIRequest(TRUE, TM_AUXTECH_REQ_ID_EMERGENCY_ORIG))
    {
      le_TrackPosConsistencyStartStop(TRUE);
    }
  }
#endif /* ! FEATURE_GNSS_SA && ! FEATURE_GNSS_LEAN_BUILD */

}

/*===========================================================================

FUNCTION tm_core_sess_req_start

DESCRIPTION
  This function is to figure out if TM-Core accepts session start request from
  protocol sub-module.

DEPENDENCIES

RETURN VALUE
  true  - if the session was started successfully
  false - otherwise

SIDE EFFECTS

===========================================================================*/
boolean tm_core_sess_req_start(tm_prtl_type                    prtl_type,
                               tm_sess_handle_type             sess_handle,
                               tm_sess_req_start_param_s_type  sess_start_param)
{
  boolean retVal = FALSE;
#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
  boolean const                        cv_emergency_on   = tm_umts_common_utils_emergency_on();
#else
  boolean const                        cv_emergency_on   = 0;
#endif /* FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS */

  lm_session_request_info_s_type       lm_sess_req_param;
  lm_request_info_s_type               lm_info_req_param;
  sm_event_pd_fix_start_payload_e_type dm_op_mode;
  tm_cm_phone_state_info_s_type        phone_state_info;
  pdsm_delete_parms_type               delete_param;
  FLT f_punc;
  FLT f_tunc;
  lm_info_s_type                       *p_lm_info_data = NULL;
#ifdef FEATURE_GNSS_GERA_ENABLE
  #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
  pdsm_client_id_type                  pd_client_id = tm_core_info.session_info.pd_param.client_id;
  ldm_client_id_e_type                 e_ldm_client_id = LDM_CLIENT_ID_MAX;
  int                    prtl_tbl_idx = -1;
  boolean                v_is_msa_sess = FALSE;
  tm_loc_processing_client_info        *p_z_lpc_client;
  tm_sess_req_continue_param_s_type    z_sess_continue_req_param;
  uint32                               q_ldm_query_mask = 0;
  ldm_client_id_e_type           e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type           e_secondary_client_id = LDM_CLIENT_ID_MAX;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core entering function [tm_core_sess_req_start]  sess_handle is %d, prtl is %d power_mode is %d",
        sess_handle, prtl_type, sess_start_param.lppm_mode.e_PowerMode);

  memset((void *)&lm_sess_req_param, 0, sizeof(lm_session_request_info_s_type));
  memset((void *)&lm_info_req_param, 0, sizeof(lm_request_info_s_type));
  memset((void *)&z_sess_continue_req_param, 0, sizeof(tm_sess_req_continue_param_s_type));

  tm_cm_iface_get_phone_state(&phone_state_info);

  if (prtl_type != TM_PRTL_TYPE_STANDALONE)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Default to power_mode M2 for protocol %d", prtl_type);
    sess_start_param.lppm_mode.e_PowerMode = TM_SESS_POWER_MODE_NORMAL;
    sess_start_param.lppm_mode.q_TbmMsecs = 0;
  }

  if (tm_core_is_reset_location_service_in_progress())
  {
    boolean session_allowed = FALSE;

    /*If reset location service is in progress, only sessions that are allowed
      are emergency sessions.*/
    if (cv_emergency_on)
    {
      if ((sess_handle == TM_SESS_HANDLE_1X_CP_MT) ||
          (sess_handle == TM_SESS_HANDLE_UMTS_CP_NI) ||
          (sess_handle == TM_SESS_HANDLE_LPP_CP_NI) ||
          (sess_handle == TM_SESS_HANDLE_UMTS_UP_NI)
          )
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Allow emergency call flows during location service reset", 0, 0, 0);
        session_allowed = TRUE;
      }
    }

    if (session_allowed == FALSE)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Not allowing positioning sessions during location service reset", 0, 0, 0);
      return (FALSE);
    }
  }
#ifdef FEATURE_JCDMA_GPSONE
  /* Abort reason is for JGPS module to end 1x UP during 1x CP */
  if (sess_start_param.abort_curr_sess == TRUE)
  {
    /* Abort any current sessions and recover to allow other session type */
    tm_core_abort_recover(TRUE, TM_STOP_REASON_JGPS_ABORT_SESS); /*lint !e506 !e730 */
  }
#endif /* FEATURE_JCDMA_GPSONE */

  if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
  {
    /* Check concurrency only for E911 early start or incoming NI sessions */
    if (TM_PRTL_TYPE_STANDALONE != prtl_type)
    {

      /* Check for mode changes from MSB to MSA */
      if (TM_SESS_OPERATION_MODE_MSA == sess_start_param.op_mode)
      {
        /* Get main and secondary client  from LDM */
        tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);
        if (LDM_CLIENT_ID_APPTRK == e_secondary_client_id)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Concurrency NOT allowed when MSA is main client. End apptrack.");

          /* Send PDAPI EVENTS before aborting the Apptrack session */
          (void)tm_lpc_generate_pd_event(LDM_CLIENT_ID_APPTRK, TM_STOP_REASON_E911);

          /* Disable apptrack client from LDM*/
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client successfully disabled at LDM");

            tm_cm_iface_set_E911_state(phone_state_info.prev_e911_state);
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                  "LDM:Reverted E911 state to prev E911 state = %d", phone_state_info.prev_e911_state);
            /* Get updated E911 state */
            tm_cm_iface_get_phone_state(&phone_state_info);
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack client failed to be disabled at LDM");
            return FALSE;
          }
          goto E911_IN_NON_CONCURR_MODE;
        }
      }

      if (TM_SESS_OPERATION_MODE_MSA == sess_start_param.op_mode)
      {
        v_is_msa_sess = TRUE;
      }

      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:TM checking for Concurrent mode");
      if (tm_core_is_e911_concurrency_allowed(LDM_CLIENT_ID_E911, v_is_msa_sess))
      {
        if ((ldm_client_query_state() & LDM_CLIENT_E911_MASK) != 0)
        {
          p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_E911);

          if (NULL == p_z_lpc_client)
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client not found to update the data");
            return FALSE;
          }

          /* Check for service interaction cases during concurrent sessions.
                      If the ongoing E911 session and the new E911 session have different session handles, then 
                      service interaction handling must be done. If the handles are the same, then its the same session
                      and just an update on the session params would be enough */

          if ((p_z_lpc_client->sessionHandle != TM_SESS_HANDLE_E911) && /* If it has TM_SESS_HANDLE_E911 handle, then its the initial E911 session */
              (p_z_lpc_client->sessionHandle != sess_handle)) /* If handles dont match, then we enter service interaction scenario */
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC: Handling E911 service interaction during concurrent sessions");
            /* Check for service interaction and stop ongoing E911 session which started in concurrent mode*/
            tm_core_concurrent_mode_e911_service_interaction(prtl_type, phone_state_info.srv_system);

            /* Add this new E911 client as secondary client:
                If E911 was the main client, aborting the main client would make Apptrack as main client. So, this new E911 would be added as secondary.
                If E911 was secondary client, then aborting the seconday would end the E911 session. So re-add E911 with new session param as secondary */
            if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(LDM_CLIENT_ID_E911, prtl_type, sess_handle, &sess_start_param, pd_client_id, LDM_CLIENT_TYPE_SECONDARY))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: E911 client enabled at LDM in concurrent mode");
              /* Sending aiding data if handle is UMTS_UP_NI */
              if (((TM_OP_REQ_AIDING_DATA_STATUS & sess_start_param.op_req) != 0) &&
                  (TM_SESS_OPERATION_MODE_MSB == sess_start_param.op_mode) &&
                  ((TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
                   (TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
                   (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle)))
              {
                /* Get cached aiding data from apptrack and provide it to SUPL call back */
                prtl_tbl_idx = tm_core_get_prtl_idx_by_type(prtl_type);
                if ((prtl_tbl_idx >= 0) && (prtl_tbl_idx < TM_PRTL_NUM))
                {
                  if (tm_core_info.prtl_func_cb_table[prtl_tbl_idx].sess_info_fp)
                  {
                    (void)tm_core_info.prtl_func_cb_table[prtl_tbl_idx].sess_info_fp(sess_handle, PRTL_SESS_INFO_STATUS, up_cache_sess_info_param);
                    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Cached aiding data sent to UMTS_UP_NI E911 session");
                  }
                  else
                  {
                    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: sess_info_fp is NULL");
                  }
                }
                else
                {
                  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: prtl_tbl_idx %d is out of bounds", prtl_tbl_idx);
                }
              }
              return (TRUE);
            }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 failed to enable at LDM");
              return (FALSE);
            }
          }
          else
          {
            /* Update LPC here only for secondary client */
            if (LDM_CLIENT_TYPE_SECONDARY == p_z_lpc_client->e_clientId_type)
            {
              tm_cm_iface_set_E911_state(TM_CM_E911_STATE_CONCURRENT);
              /* Get updated E911 state */
              tm_cm_iface_get_phone_state(&phone_state_info);

              tm_mapping_between_sess_start_sess_continue(TRUE, &sess_start_param, &z_sess_continue_req_param);

              if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_update_client(LDM_CLIENT_ID_E911, prtl_type, sess_handle, &z_sess_continue_req_param))
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:In concurrent mode, Secondary client E911 updated during emergency continue");

                /* Sending aiding data if handle is UMTS_UP_NI */
                if (((TM_OP_REQ_AIDING_DATA_STATUS & sess_start_param.op_req) != 0) &&
                    (TM_SESS_OPERATION_MODE_MSB == sess_start_param.op_mode) &&
                    ((TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
                     (TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
                     (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle)))
                {
                  /* Get cached aiding data from apptrack and provide it to SUPL call back */
                  prtl_tbl_idx = tm_core_get_prtl_idx_by_type(prtl_type);
                  if ((prtl_tbl_idx >= 0) && (prtl_tbl_idx < TM_PRTL_NUM))
                  {
                    if (tm_core_info.prtl_func_cb_table[prtl_tbl_idx].sess_info_fp)
                    {
                      (void)tm_core_info.prtl_func_cb_table[prtl_tbl_idx].sess_info_fp(sess_handle, PRTL_SESS_INFO_STATUS, up_cache_sess_info_param);
                      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Cached aiding data sent to UMTS_UP_NI E911 session");
                    }
                    else
                    {
                      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: sess_info_fp is NULL");
                    }
                  }
                  else
                  {
                    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: prtl_tbl_idx %d is out of bounds", prtl_tbl_idx);
                  }
                }
                return (TRUE);
              }
              else
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 client FAILED to update during emergency continue");
                return (FALSE);
              }
            }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 already enabled as main client. Continue...");
            }
          }
        }
        else
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:TM in E911 Concurrent mode");
          tm_cm_iface_set_E911_state(TM_CM_E911_STATE_CONCURRENT);
          /* Get updated E911 state */
          tm_cm_iface_get_phone_state(&phone_state_info);
          pd_client_id = tm_core_info.pz_concurrent_client_info.client_id;

          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(LDM_CLIENT_ID_E911, prtl_type, sess_handle, &sess_start_param, pd_client_id, LDM_CLIENT_TYPE_SECONDARY))
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: E911 client enabled at LDM in concurrent mode");
            /* Sending aiding data if handle is UMTS_UP_NI */
            if (((TM_OP_REQ_AIDING_DATA_STATUS & sess_start_param.op_req) != 0) &&
                (TM_SESS_OPERATION_MODE_MSB == sess_start_param.op_mode) &&
                ((TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
                 (TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
                 (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle)))
            {
              /* Get cached aiding data from apptrack and provide it to SUPL call back */
              prtl_tbl_idx = tm_core_get_prtl_idx_by_type(prtl_type);
              if ((prtl_tbl_idx >= 0) && (prtl_tbl_idx < TM_PRTL_NUM))
              {
                if (tm_core_info.prtl_func_cb_table[prtl_tbl_idx].sess_info_fp)
                {
                  (void)tm_core_info.prtl_func_cb_table[prtl_tbl_idx].sess_info_fp(sess_handle, PRTL_SESS_INFO_STATUS, up_cache_sess_info_param);
                  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Cached aiding data sent to UMTS_UP_NI E911 session");
                }
                else
                {
                  MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: sess_info_fp is NULL");
                }
              }
              else
              {
                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: prtl_tbl_idx %d is out of bounds", prtl_tbl_idx);
              }
            }
            return (TRUE);
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 failed to enable at LDM");
            return (FALSE);
          }
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Concurrency NOT allowed");

        /* Check for service interaction with E911 */
        if ((ldm_client_query_state() & LDM_CLIENT_E911_MASK) != 0 && (TRUE == v_enable_secondary_client_on_ldm))
        {
          /* Check for service interaction and stop ongoing E911 session which started in concurrent mode*/
          tm_core_concurrent_mode_e911_service_interaction(prtl_type, phone_state_info.srv_system);
        }

        /* Since concurrency has failed, remove apptrack */
        if ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0)
        {
          /* Clean up the ongoing apptrack session in TM */
          tm_core_abort_recover(TRUE, TM_STOP_REASON_E911);

          /* Disable apptrack client from LDM*/
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client successfully disabled at LDM");
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client with handle already disabled at LDM");
          }

          if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
          {
            /* Only if we are in concurrent change to previous. Else leave it in E911 state since E911 is starting as main client*/
            tm_cm_iface_set_E911_state(phone_state_info.prev_e911_state);

            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                  "LDM:Reverted E911 state to prev E911 state = %d", phone_state_info.prev_e911_state);
            /* Get updated E911 state */
            tm_cm_iface_get_phone_state(&phone_state_info);
          }
        }
      }
    }
    else if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
    {
      if ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Incoming Apptrack. TM in Concurrent mode");

        p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_APPTRK);

        if (NULL == p_z_lpc_client)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client not found to update the data");
          return FALSE;
        }

        /* Update LPC here only for secondary client */
        if (LDM_CLIENT_TYPE_SECONDARY == p_z_lpc_client->e_clientId_type)
        {
          pd_client_id = tm_core_info.pz_concurrent_client_info.client_id;

          /* Update apptrack client in LPC */
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(LDM_CLIENT_ID_APPTRK, prtl_type, sess_handle, &sess_start_param, pd_client_id, LDM_CLIENT_TYPE_SECONDARY))
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Apptrack client updated at LDM in concurrent mode");
            return (TRUE);
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack failed to enable at LDM");
            return (FALSE);
          }
        }
        else
        {
          /* Apptrack is the main client, LPC and LM is updated later in this function */
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack already enabled as main client. Continue...");
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Incoming Apptrack during E911 state. TM in Concurrent mode");

        if ((ldm_client_query_state() & LDM_CLIENT_E911_MASK) != 0)
        {
          //E911 client present in LDM. Add apptrack client as secondary.
          pd_client_id = tm_core_info.pz_concurrent_client_info.client_id;

          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(LDM_CLIENT_ID_APPTRK, prtl_type, sess_handle, &sess_start_param, pd_client_id, LDM_CLIENT_TYPE_SECONDARY))
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Apptrack client enabled as secondary");
            return (TRUE);
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack failed to enable as secondary");
            return (FALSE);
          }
        }
        else
        {
          // In E911 state, but E911 client not in LDM. So add apptrack as main client.
          pd_client_id = tm_core_info.session_info.pd_param.client_id;

          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(LDM_CLIENT_ID_APPTRK, prtl_type, sess_handle, &sess_start_param, pd_client_id, LDM_CLIENT_TYPE_MAIN))
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Apptrack client enabled as main");
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack failed to enable as main");
            return (FALSE);
          }
        }
      }
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Not in concurrent mode");
    }
  }

E911_IN_NON_CONCURR_MODE:

  if (cv_emergency_on)
  {
    boolean active_sess_is_ni =
       ((tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_LPP_CP_NI) ||
        (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_1X_CP_MT) ||
        (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_UP_NI) ||
        (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_CP_NI));

    boolean new_sess_prtl_ni  =
       ((prtl_type == TM_PRTL_TYPE_UMTS_CP) ||
        (prtl_type == TM_PRTL_TYPE_1X_CP) ||
        (prtl_type == TM_PRTL_TYPE_UMTS_UP) ||
        (prtl_type == TM_PRTL_TYPE_LPP_CP));

    if (active_sess_is_ni && new_sess_prtl_ni)
    {
      /* emergency session was ongoing on LPP_CP or SUPL, now a new GPS session was started on a different RAT
         started by the network, continue the session with the new session handle */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Emergency session on going on %d, new protocol %d. Abort ongoing ",
            tm_core_info.session_info.active_sess_handle, prtl_type, 0);
      /* Abort any current sessions and recover to allow other session type */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_E911); /*lint !e506 !e730 */
    }

#ifdef FEATURE_MO_GPS_FOR_ECALL
    if (!active_sess_is_ni && new_sess_prtl_ni &&
        (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_E911) &&
        (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE))
    {
      /* MO session was ongoing in E911, however network session started on 
       * W/1X/LPP. Abort the ongoing session and accept the new session */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Abort ongoing session. Handle(%d), New protocol (%d)",
            tm_core_info.session_info.active_sess_handle, prtl_type, 0);
      /* Abort any current sessions and recover to allow other session type */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_E911); /*lint !e506 !e730 */
    }
#endif

    if ((tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE) &&
        ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) &&
         ((prtl_type == TM_PRTL_TYPE_UMTS_CP) ||
          (prtl_type == TM_PRTL_TYPE_1X_CP))) &&
        (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE)
        )
    {
      /* emergency session was started on SVLTE when camped on LTE, however network session started on W/1X
         abort the ongoing session and accept the new session */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Emergency session on going on %d, new protocol %d. Abort ongoing ",
            tm_core_info.session_info.active_sess_handle, prtl_type, 0);
      /* Abort any current sessions and recover to allow other session type */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_E911); /*lint !e506 !e730 */
    }

    /* if MO is ongoing and a new NI session is started */
    if (((tm_core_info.session_info.active_sess_handle < TM_SESS_HANDLE_1X_CP_MT) &&
         (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE)) &&
        ((prtl_type == TM_PRTL_TYPE_UMTS_CP) ||
         (prtl_type == TM_PRTL_TYPE_1X_CP) ||
         (prtl_type == TM_PRTL_TYPE_LPP_CP) ||
         (prtl_type == TM_PRTL_TYPE_UMTS_UP)))
    {
      /* MO was ongoing, however network session started on W/G/1X/LTE E911 session
         abort the ongoing session and accept the new session. */
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "MO session on going on %d, new E911 session started by protocol %d. Abort ongoing ",
            tm_core_info.session_info.active_sess_handle, prtl_type, 0);
      /* Abort any current sessions and recover to allow other session type */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_E911); /*lint !e506 !e730 */
    }
  }

#ifdef FEATURE_GNSS_PERIODIC_PPM
  #error code not present
#endif

  if (tm_core_is_internal_session_on() == TRUE)
  {
    /*Abort the ongoing internal session*/
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Aborting on-going internal session", 0, 0, 0);
    tm_core_abort_recover(TRUE, TM_STOP_REASON_ABORT_INTERNAL_SESSION);
  }

  if (cv_emergency_on &&
      (tm_core_is_ni_resp_pending() == TRUE) &&
      ((sess_handle == TM_SESS_HANDLE_UMTS_CP_NI) ||
       (sess_handle == TM_SESS_HANDLE_LPP_CP_NI)))
  {
    /* NI response is pending from AP, but a new CP NI LR came in, we need to honor CP NILR and when
       the NI Response comes back, we need to ensure CP NI LR is not preempted */
    tm_core_set_ni_resp_pending(FALSE);
  }

  tm_core_info.session_info.v_session_started_for_e911 = cv_emergency_on;

  /* Accept requests when there is no active session, by starting a new LM-TM
   * session */
  if ((tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_NONE)
      && (tm_core_info.session_info.session_state == TM_CORE_SESS_STATE_INIT))
  {

#ifdef FEATURE_GNSS_GERA_ENABLE
    #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

    pd_client_id = tm_core_info.session_info.pd_param.client_id;
    q_ldm_query_mask = ldm_client_query_state();

    if (TM_CM_E911_STATE_ORIG == phone_state_info.e911_state)
    {
      if (TRUE == v_enable_secondary_client_on_ldm)
      {
        if ((q_ldm_query_mask & LDM_CLIENT_E911_MASK) == 0)
        {
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(LDM_CLIENT_ID_E911, prtl_type, sess_handle, &sess_start_param, pd_client_id, LDM_CLIENT_TYPE_MAIN))
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 client enabled at LDM as main client");
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "E911 client failed to enable at LDM");
            return (FALSE);
          }
        }
        else /* E911 client already present in LDM. Convert to main client and update it */
        {
          if (tm_lpc_client_update_client_type(LDM_CLIENT_ID_E911, LDM_CLIENT_TYPE_MAIN))
          {
            /* Clear LPC control data for secondary client */
            tm_lpc_clear_control_data();

            tm_mapping_between_sess_start_sess_continue(TRUE, &sess_start_param, &z_sess_continue_req_param);

            if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_update_client(LDM_CLIENT_ID_E911, prtl_type, sess_handle, &z_sess_continue_req_param))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:In concurrent mode, Secondary client E911 updated during emergency");
            }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 client FAILED to update during emergency");
              return (FALSE);
            }
          }
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Secondary client %d FAILED to switch as main client!", LDM_CLIENT_ID_E911);
            return (FALSE);
          }
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 client already enabled at LDM. No need to request LPC");
      }
    }
    else if (TM_CM_E911_STATE_NONE == phone_state_info.e911_state)
    {
      if (sess_start_param.num_fixes > 1)
      {
        e_ldm_client_id = LDM_CLIENT_ID_APPTRK;
      }
      else
      {
        e_ldm_client_id = LDM_CLIENT_ID_SINGLE_SHOT;
      }

      if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(e_ldm_client_id, prtl_type, sess_handle, &sess_start_param, pd_client_id, LDM_CLIENT_TYPE_MAIN))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Client %d enabled at LDM", e_ldm_client_id);
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Client %d failed to enable at LDM as main client", e_ldm_client_id);
        return (FALSE);
      }
    }
    else if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
    {
      if (sess_start_param.num_fixes > 1)
      {
        e_ldm_client_id = LDM_CLIENT_ID_APPTRK;
      }
      else
      {
        e_ldm_client_id = LDM_CLIENT_ID_E911;
      }


      if ((LDM_CLIENT_ID_APPTRK == e_ldm_client_id) &&
          (pd_client_id < 1)) /* client id just got initialized after abort_recover */
      {
        p_z_lpc_client = lpc_get_loc_processing_client(e_ldm_client_id);

        if (NULL == p_z_lpc_client)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client not found to update the data");
          return FALSE;
        }

        /* Set the correct pd client id from LPC database*/
        pd_client_id = tm_lpc_get_pdsm_client_id_for_ldm_client(e_ldm_client_id);
        tm_core_info.session_info.pd_param.client_id = pd_client_id;
      }

      if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(e_ldm_client_id, prtl_type, sess_handle, &sess_start_param, pd_client_id, LDM_CLIENT_TYPE_MAIN))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Client %d enabled at LDM", e_ldm_client_id);
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Client %d failed to enable at LDM as main client", e_ldm_client_id);
        return (FALSE);
      }
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unsupported E911 state %d ", phone_state_info.e911_state);
      return (FALSE);
    }


    /* TM-Core accepts the incoming request, so update TM-Core session info */
    tm_core_info.session_info.active_sess_handle = sess_handle;
    tm_core_info.session_info.from_protocol      = prtl_type;
    tm_core_info.session_info.op_mode            = sess_start_param.op_mode;

    tm_core_update_session_state(TM_CORE_SESS_STATE_ACCESS_GRANTED);

    /*
      Now request session start to LM
      If life is good, LM should accept it.
      If there is something wrong, LM rejects it (asynchronously), we need to make sure we do recovery properly
    */
    lm_sess_req_param.q_handle = sess_handle;
    lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_START;
    lm_sess_req_param.q_req_mask = (uint32)LM_SESSION_REQUEST_SESSION;

    if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
        ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0))
    {
      /* If in concurrent and if apptrack is in progress, apptrack is the main client. LM should not run in E911 mode */
      lm_sess_req_param.z_request.z_session_request.b_e911_session = FALSE;
    }
    else
    {
      lm_sess_req_param.z_request.z_session_request.b_e911_session =
         tm_core_info.session_info.v_session_started_for_e911;
    }

    lm_sess_req_param.z_request.z_session_request.v_no_force_idle = v_no_force_idle_for_secondary_client;

    /* Get main and secondary client  from LDM */
    tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

    if ((LDM_CLIENT_ID_E911 == e_main_client_id) && (TM_SESS_OPERATION_MODE_MSA == sess_start_param.op_mode))
    {
      p_z_lpc_client = lpc_get_loc_processing_client(e_main_client_id);

      if (NULL == p_z_lpc_client)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Client %d not found in database", e_main_client_id);
        return FALSE;
      }
      lm_sess_req_param.z_request.z_session_request.v_aa_not_needed = p_z_lpc_client->v_filterAA;
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: AA not needed bool set to %d for client %d", lm_sess_req_param.z_request.z_session_request.v_aa_not_needed, e_main_client_id);
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPC: AA not needed bool not set for client %d", e_main_client_id);
    }

    if ((tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_1X_CP)
        || (tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_UMTS_CP)
        || (tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_LPP_CP))
    {
      lm_sess_req_param.z_request.z_session_request.e_up_cp = LM_UP_CP_MODE_CP;
    }
    else
    {
      lm_sess_req_param.z_request.z_session_request.e_up_cp = LM_UP_CP_MODE_UP;
    }


    lm_sess_req_param.z_request.z_session_request.e_mode = tm_core_convert_op_mode(sess_start_param.op_mode, prtl_type);
    lm_sess_req_param.z_request.z_session_request.e_mo_mt = tm_core_mo_mt_mode(sess_handle);
    lm_sess_req_param.z_request.z_session_request.q_num_fixes = sess_start_param.num_fixes;
    lm_sess_req_param.z_request.z_session_request.q_tbf = sess_start_param.tbf_ms;

    (void)tm_ConvertToLmLppmMode(&sess_start_param.lppm_mode, &lm_sess_req_param.z_request.z_session_request.z_lppm_config);

    /*To be safe, copy the same in LPPM request as well. Won't be used by LM*/
    lm_sess_req_param.z_request.z_lppm_request = lm_sess_req_param.z_request.z_session_request.z_lppm_config;

    tm_core_info.session_info.z_lppm_mode = sess_start_param.lppm_mode;
    tm_core_info.session_info.q_num_fixes = lm_sess_req_param.z_request.z_session_request.q_num_fixes;
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Setting LM num_fixes = %u", tm_core_info.session_info.q_num_fixes);

    /* Check to see if NHz operating mode is supported.
       If it is not, restrict the TBF to 1000ms minimum 
    */
    if (!(tm_core_info.config_info.gnss_nhz_config & GNSS_NHZ_ENABLED)
        && (sess_start_param.tbf_ms < TM_CORE_NOMINAL_TBF_MS))
    {
      lm_sess_req_param.z_request.z_session_request.q_tbf = TM_CORE_NOMINAL_TBF_MS;
    }

    /* Determine if data demod is allow for MSB */
    if (sess_start_param.op_mode == TM_SESS_OPERATION_MODE_MSB ||
        sess_start_param.op_mode == TM_SESS_OPERATION_MODE_MSB_OPT)
    {
      if ((tm_core_get_active_prtl_module(MT_NA) == TM_PRTL_TYPE_1X_UP)
          && (tm_core_info.config_info.smart_msb == FALSE))
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Data demod disabled", 0, 0, 0);
        lm_sess_req_param.z_request.z_session_request.u_do_data_demod = FALSE;
      }
      else
      {
        lm_sess_req_param.z_request.z_session_request.u_do_data_demod = TRUE;
      }
    }
    else
    {
      lm_sess_req_param.z_request.z_session_request.u_do_data_demod = TRUE;
    }

    p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_APPTRK);

    if ((tm_core_info.config_info.u_disable_Dynamic_HEPE_usage & TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF) &&
        (TM_LOC_MAX_HIGH_HEPE_THRESHOLD > sess_start_param.accuracy_threshold) &&
        (LDM_CLIENT_ID_APPTRK == e_ldm_client_id) && (TRUE == p_z_lpc_client->v_isFinalFix))
    {
      sess_start_param.accuracy_threshold = TM_LOC_MAX_HIGH_HEPE_THRESHOLD;
    }

    if (sess_start_param.op_req & TM_OP_REQ_PPM)
    {
      lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_CDMA;
      lm_sess_req_param.z_request.z_cdma_request.q_timeout = sess_start_param.ppm_qos * 1000;
    }

    if (sess_start_param.op_req & TM_OP_REQ_PRM)
    {
      lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_GPS;
      lm_sess_req_param.z_request.z_gps_request.q_timeout = sess_start_param.prm_qos * 1000;
      lm_sess_req_param.z_request.z_gps_request.u_qos     = sess_start_param.prm_prq;
      lm_sess_req_param.z_request.z_gps_request.multi_report_msa = sess_start_param.multi_report_msa;
    }

    if (sess_start_param.op_req & TM_OP_REQ_LR)
    {
      lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_FIX;
      lm_sess_req_param.z_request.z_fix_request.q_timeout = sess_start_param.lr_qos * 1000;
      lm_sess_req_param.z_request.z_fix_request.q_qos_accuracy_threshold  = sess_start_param.accuracy_threshold;
      if (sess_start_param.lppm_mode.e_PowerMode > TM_SESS_POWER_MODE_NORMAL)
      {
        /*Turn Off dynamic HEPE for Background LPPM Modes*/
        lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = FALSE;
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Session Start: Dynamic HEPE Disabled");
      }
      else
      {
        lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = sess_start_param.dynamic_qos_enabled;
      }
    }

    /* Check to see whether Dynamic HEPE needs to enabled or disabled */
    if (((sess_start_param.op_mode == TM_SESS_OPERATION_MODE_MSB) || (sess_start_param.op_mode == TM_SESS_OPERATION_MODE_MSB_OPT) ||
         (sess_start_param.op_mode == TM_SESS_OPERATION_MODE_STANDALONE)) &&
        (TM_CM_E911_STATE_NONE == phone_state_info.e911_state) &&
        ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0) &&
        (tm_core_info.config_info.u_disable_Dynamic_HEPE_usage & TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "DYN_HEPE_DISABLED_FOR_BETTER_TTFF", 0);
      lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = FALSE;
    }
    /* for MO OTDOA no need to start LM session */
    if ((sess_start_param.op_mode != TM_SESS_OPERATION_MODE_OTDOA) ||
        (sess_start_param.op_mode == TM_SESS_OPERATION_MODE_OTDOA &&
         tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_UP_NI))
    {
      retVal = lm_session_request(&lm_sess_req_param);
      tm_core_info.session_info.op_requested |= sess_start_param.op_req;
      if (lm_sess_req_param.q_req_mask & LM_SESSION_REQUEST_FIX)
      {
        tm_core_report_fix_start_to_techsel();
      }
    }
    else
    {
      tm_core_info.session_info.op_requested = TM_OP_REQ_NONE;
      retVal = TRUE;
    }

    /* Fire Evt PD_FIX start, req Aiding Data Wishlist, Ref Loc, and start the
     * diag pos timer only if session is not PERIODIC_PPM */
    if ((sess_handle != TM_SESS_HANDLE_PERIODIC_PPM) &&
        (retVal == TRUE)) /*lint !e506 !e731 */
    {
      /* Fire EVENT_GPS_PD_FIX_START */
      dm_op_mode = tm_core_map_dm_event_op_mode(tm_core_info.session_info.op_mode);
      sm_report_event_gps_fix_start(tm_core_event_counter++, dm_op_mode);

      /*
         Always get aiding data wishlist update
         so that we can populate alm_svmask and eph_svmask in PRM report.
         However, if protocol modules does not ask for aiding data update,
         don't forward aiding data wishlist to protocol modules,
         when it comes back from MGP
      */
      lm_info_req_param.e_req_type = LM_REQUEST_AIDING_DATA_WISHLIST;
      lm_info_req_param.z_request.e_wishlist_req_type =
         tm_core_xlate_prtl_to_wishlist_src(sess_start_param.op_mode);
      /* for OTDOA no need to start LM session */
      if (sess_start_param.op_mode != TM_SESS_OPERATION_MODE_OTDOA)
      {
        retVal = lm_request(&lm_info_req_param);
      }

      if (sess_start_param.op_req & TM_OP_REQ_REF_LOC)
      {
        lm_info_req_param.e_req_type = LM_REQUEST_POS_ESTIMATE;
        retVal = lm_request(&lm_info_req_param);
      }
      tm_core_info.session_info.q_FixTimeRemainingMSec = tm_core_info.session_info.pd_param.pd_qos.gps_session_timeout * 1000;
      if (sess_start_param.op_mode == TM_SESS_OPERATION_MODE_MSB ||
          sess_start_param.op_mode == TM_SESS_OPERATION_MODE_MSA)
      {
        /* Query TM-CM module to get current acquired system */
        tm_cm_iface_get_phone_state(&phone_state_info);
        if ((phone_state_info.srv_system == CGPS_SRV_SYS_LTE) ||
            (phone_state_info.emerg_conn_srv_system == CGPS_SRV_SYS_WLAN) ||
            (CGPS_SRV_SYS_WLAN == tm_ds_get_emergency_srv_sys()))
        {
          p_lm_info_data = (lm_info_s_type *)os_MemAlloc(sizeof(lm_info_s_type), OS_MEM_SCOPE_TASK);

          if (NULL != p_lm_info_data)
          {
            memset(p_lm_info_data, 0, sizeof(*p_lm_info_data));

            /* tell MGP to start LTE time transfer need API from Bora */
            p_lm_info_data->e_info_type = LM_SESSION_INFO_SESSION_TYPE;
            p_lm_info_data->z_info.z_sess_info.e_mode = tm_core_convert_op_mode(sess_start_param.op_mode, prtl_type);
            p_lm_info_data->z_info.z_sess_info.e_srv_system = phone_state_info.srv_system;
            retVal = lm_info(p_lm_info_data);

            (void)os_MemFree((void **)&p_lm_info_data);
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cannot allocate memory for lm_info_s_type in tm_core_sess_req_start");
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Falling back on to stack memory");

            retVal = tm_core_sess_req_start_update_lm_info_data(tm_core_convert_op_mode(sess_start_param.op_mode, prtl_type), phone_state_info.srv_system);
          }
        }
      }
#ifdef FEATURE_CGPS_XTRA_T
      if (sess_start_param.op_mode == TM_SESS_OPERATION_MODE_STANDALONE)
      {
        /* TODO : ahmad to add new TLE API call here! */
      }
#endif
    }

    if (retVal == FALSE)
    {
      /* LM does not allow the request, abort and recover */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    }
  }
  else if ((tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911) &&
           ((prtl_type == TM_PRTL_TYPE_LPP_CP) ||
            (prtl_type == TM_PRTL_TYPE_UMTS_UP) ||
            (prtl_type == TM_PRTL_TYPE_UMTS_CP)))
  {
    /* emergency session was started at start of emergency call indication. now the GPS session was 
       started by the network, continue the session with the new session handle */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Emergency session on going ", 0, 0, 0);

    tm_core_sess_req_continue_for_emergency_session(prtl_type, sess_handle, sess_start_param);
    retVal = TRUE;
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Reject start req because TM-Core already is in another session", 0, 0, 0);
    retVal = FALSE;
  }

  return (retVal);
}


/*===========================================================================

FUNCTION tm_core_sess_req_continue_for_emergency_session

DESCRIPTION
  This function accepts session continue request for an emergency session. At 
  start of an emergency call, TM starts a dummy E911 session with LM, once the
  network starts positioning, TM will send this continue session with updated
  session handle and session parameters.
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

boolean tm_core_sess_req_continue_for_emergency_session(tm_prtl_type prtl_type,
                                                        tm_sess_handle_type                sess_handle,
                                                        tm_sess_req_start_param_s_type  sess_start_param)
{


  lm_session_request_info_s_type lm_sess_req_param;
  lm_request_info_s_type         lm_info_req_param;
  tm_op_req_type                 new_op_req;
  tm_op_req_type                 existing_op_req;
  tm_cm_phone_state_info_s_type phone_state_info;
  boolean retVal = TRUE;
  tm_sess_req_continue_param_s_type z_sess_continue_req_param;
  tm_loc_processing_client_info     *p_z_lpc_client;
  ldm_client_id_e_type           e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type           e_secondary_client_id = LDM_CLIENT_ID_MAX;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM-Core Recv'd Sess Continue Req for emergency %lu %lu, %u", prtl_type, sess_handle, tm_core_info.session_info.session_state);

  memset((void *)&lm_sess_req_param, 0, sizeof(lm_session_request_info_s_type));
  memset((void *)&lm_info_req_param, 0, sizeof(lm_request_info_s_type));
  memset((void *)&z_sess_continue_req_param, 0, sizeof(tm_sess_req_continue_param_s_type));

  if (prtl_type != TM_PRTL_TYPE_STANDALONE)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Default to power_mode M2 for protocol %d", prtl_type);
    sess_start_param.lppm_mode.e_PowerMode = TM_SESS_POWER_MODE_NORMAL;
    sess_start_param.lppm_mode.q_TbmMsecs = 0;
  }

  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
  {
    if ((TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
        (TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
        (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle) ||
        (TM_SESS_HANDLE_E911     == sess_handle) ||
        (TM_SESS_HANDLE_1X_CP_MT   == sess_handle) ||
        (TM_SESS_HANDLE_1X_UP_NI   == sess_handle))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:In E911 Emergency continue. Concurrent mode, sess_handle 0X%x", sess_handle);

      if (TM_SESS_OPERATION_MODE_MSA == sess_start_param.op_mode)
      {
        /* Get main and secondary client  from LDM */
        tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

        if (LDM_CLIENT_ID_E911 == e_secondary_client_id) /* Apptrack is main client */
        {
          if ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0)
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:TM checking for Concurrent mode in request continue");
            if (tm_core_is_e911_concurrency_allowed(LDM_CLIENT_ID_E911, TRUE))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Concurrent mode");
            }
            else
            {
              /* Clean up the ongoing apptrack session in TM */
              tm_core_abort_recover(TRUE, TM_STOP_REASON_E911);

              /* Disable apptrack client from LDM*/
              if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
              {
                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client with handle %u successfully disabled at LDM", sess_handle);
              }
              else
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack client already disabled at LDM");
              }

              tm_cm_iface_set_E911_state(phone_state_info.prev_e911_state);

              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                    "LDM:Reverted E911 state to prev E911 state = %d and abort ongoing apptrack", phone_state_info.prev_e911_state);
              /* Get updated E911 state */
              tm_cm_iface_get_phone_state(&phone_state_info);

              /* Switch the secondary client to main client */
              if (tm_lpc_client_update_client_type(LDM_CLIENT_ID_E911, LDM_CLIENT_TYPE_MAIN))
              {
                /* Clear LPC control data for secondary client */
                tm_lpc_clear_control_data();

                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Secondary client %d switched as main client!", LDM_CLIENT_ID_E911);

                MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Session Continue params: op_mode %d, num_fixes %d, op_req %d, tbf_ms %d",
                      sess_start_param.op_mode, sess_start_param.num_fixes, sess_start_param.op_req, sess_start_param.tbf_ms);
              }
              else
              {
                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Secondary client %d FAILED to switch as main client!", LDM_CLIENT_ID_E911);
                return FALSE;
              }
            }
          }
        }
        else
        { /* E911 is main client, Apptrack is secondary */

          if (LDM_CLIENT_ID_APPTRK == e_secondary_client_id)
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Concurrency NOT allowed when E911 MSA is main client. End apptrack.");

            /* Send PDAPI EVENTS before aborting the Apptrack session */
            (void)tm_lpc_generate_pd_event(LDM_CLIENT_ID_APPTRK, TM_STOP_REASON_E911);

            /* Disable apptrack client from LDM*/
            if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client successfully disabled at LDM");

              tm_cm_iface_set_E911_state(phone_state_info.prev_e911_state);
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                    "LDM:Reverted E911 state to prev E911 state = %d", phone_state_info.prev_e911_state);
              /* Get updated E911 state */
              tm_cm_iface_get_phone_state(&phone_state_info);
            }
            else
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack client failed to be disabled at LDM");
              return FALSE;
            }
          }

          MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Session Continue params: op_mode %d, num_fixes %d, op_req %d, tbf_ms %d",
                sess_start_param.op_mode, sess_start_param.num_fixes, sess_start_param.op_req, sess_start_param.tbf_ms);
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Concurrent mode");
      }
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Emergency Continue called for non-emergency handle %d", sess_handle);
    }
  }

  tm_mapping_between_sess_start_sess_continue(TRUE, &sess_start_param, &z_sess_continue_req_param);

  if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_update_client(LDM_CLIENT_ID_E911, prtl_type, sess_handle, &z_sess_continue_req_param))
  {
    if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
    {
      p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_E911);

      if (NULL == p_z_lpc_client)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:Client not found to update the data");
        return FALSE;
      }

      if (LDM_CLIENT_TYPE_SECONDARY == p_z_lpc_client->e_clientId_type)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:In concurrent mode, Secondary client E911 updated during emergency continue");
        return (TRUE);
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 main client updated during emergency continue");
      }
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Not in E911 concurrent mode, E911 client updated during emergency continue");
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 client FAILED to update during emergency continue");
    return (FALSE);
  }

  /* TM-Core accepts the incoming request, so update TM-Core session info */
  tm_core_info.session_info.active_sess_handle = sess_handle;
  tm_core_info.session_info.from_protocol      = prtl_type;

  /* If operation mode has been changed (Optimal mode fallback sessions, or E911 that starts with MSB and now the network
     requests MSA for example ), notify LM to change the mode*/
  if ((tm_core_info.session_info.op_mode != sess_start_param.op_mode)
      && (sess_start_param.op_mode != TM_SESS_OPERATION_MODE_OTDOA))
  {

    MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "Exiting op mode (%u) != cont.op_mode (%u), tm_core.op_requested = %x",
          tm_core_info.session_info.op_mode,
          sess_start_param.op_mode,
          tm_core_info.session_info.op_requested,
          sess_start_param.op_req);

    tm_core_info.session_info.op_mode = sess_start_param.op_mode;
    lm_sess_req_param.q_handle = sess_handle;
    lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_CONTINUE;
    lm_sess_req_param.q_req_mask = (uint32)LM_SESSION_REQUEST_SESSION;
    lm_sess_req_param.z_request.z_session_request.e_mode = tm_core_convert_op_mode(sess_start_param.op_mode, prtl_type);
    lm_sess_req_param.z_request.z_session_request.e_mo_mt = tm_core_mo_mt_mode(sess_handle);

    /* Determine if data demod is allow for MSB */
    if (sess_start_param.op_mode == TM_SESS_OPERATION_MODE_MSB)
    {
      if ((tm_core_get_active_prtl_module(MT_NA) == TM_PRTL_TYPE_1X_UP)
          && (tm_core_info.config_info.smart_msb == FALSE))
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Data demod disabled", 0, 0, 0);
        lm_sess_req_param.z_request.z_session_request.u_do_data_demod = FALSE;
      }
      else
      {
        lm_sess_req_param.z_request.z_session_request.u_do_data_demod = TRUE;
      }
    }
    else
    {
      lm_sess_req_param.z_request.z_session_request.u_do_data_demod = TRUE;
    }
    retVal = lm_session_request(&lm_sess_req_param);
  }

  if (retVal)
  {
    lm_sess_req_param.q_req_mask = 0;

    /* 
       For operatons which have not been previously started, use start to LM
       For operatons which have already been started, use continue to LM
    */
    existing_op_req = sess_start_param.op_req & tm_core_info.session_info.op_requested;
    new_op_req      = sess_start_param.op_req & ~(existing_op_req);

    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "continue_param.op_req = %x, tm_core_info.session_info.op_requested = %x",
          sess_start_param.op_req,
          tm_core_info.session_info.op_requested);

    p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_APPTRK);

    if ((tm_core_info.config_info.u_disable_Dynamic_HEPE_usage & TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF) &&
        (TM_LOC_MAX_HIGH_HEPE_THRESHOLD > sess_start_param.accuracy_threshold) &&
        ((NULL != p_z_lpc_client) && (TRUE == p_z_lpc_client->v_isFinalFix)))
    {
      sess_start_param.accuracy_threshold = TM_LOC_MAX_HIGH_HEPE_THRESHOLD;
    }

    /* 
       For operatons which have not been previously started, use start to LM
    */
    if (new_op_req & (TM_OP_REQ_PPM | TM_OP_REQ_PRM | TM_OP_REQ_LR))
    {
      lm_sess_req_param.q_handle   = sess_handle;
      lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_START;

      if (new_op_req & TM_OP_REQ_PPM)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_CDMA;
        lm_sess_req_param.z_request.z_cdma_request.q_timeout = sess_start_param.ppm_qos * 1000;
      }

      if (new_op_req & TM_OP_REQ_PRM)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_GPS;
        lm_sess_req_param.z_request.z_gps_request.q_timeout = sess_start_param.prm_qos * 1000;
        lm_sess_req_param.z_request.z_gps_request.u_qos     = sess_start_param.prm_prq;
        lm_sess_req_param.z_request.z_gps_request.multi_report_msa = sess_start_param.multi_report_msa;
      }

      if (new_op_req & TM_OP_REQ_LR)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_FIX;
        lm_sess_req_param.z_request.z_fix_request.q_timeout = sess_start_param.lr_qos * 1000;
        lm_sess_req_param.z_request.z_fix_request.q_qos_accuracy_threshold  = sess_start_param.accuracy_threshold;
        lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled  = sess_start_param.dynamic_qos_enabled;
      }
      retVal = lm_session_request(&lm_sess_req_param);
      if (lm_sess_req_param.q_req_mask & LM_SESSION_REQUEST_FIX)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
            "Fix Request for New Emergency Session. Don't notify TechSel");
      }

    }

    tm_core_info.session_info.op_requested |= new_op_req;


    /* 
       For operatons which have already been previously started, use continue to LM
    */
    if (existing_op_req & (TM_OP_REQ_PPM | TM_OP_REQ_PRM | TM_OP_REQ_LR))
    {
      lm_sess_req_param.q_handle   = sess_handle;
      lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_CONTINUE;

      if (existing_op_req & TM_OP_REQ_PPM)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_CDMA;
        lm_sess_req_param.z_request.z_cdma_request.q_timeout = sess_start_param.ppm_qos * 1000;
      }

      if (existing_op_req & TM_OP_REQ_PRM)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_GPS;
        lm_sess_req_param.z_request.z_gps_request.q_timeout = sess_start_param.prm_qos * 1000;
        lm_sess_req_param.z_request.z_gps_request.u_qos     = sess_start_param.prm_prq;
        lm_sess_req_param.z_request.z_gps_request.multi_report_msa = sess_start_param.multi_report_msa;
      }

      if (existing_op_req & TM_OP_REQ_LR)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_FIX;
        lm_sess_req_param.z_request.z_fix_request.q_timeout = sess_start_param.lr_qos * 1000;
        lm_sess_req_param.z_request.z_fix_request.q_qos_accuracy_threshold  = sess_start_param.accuracy_threshold;
        lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled  = sess_start_param.dynamic_qos_enabled;
      }
      retVal = lm_session_request(&lm_sess_req_param);
      if (lm_sess_req_param.q_req_mask & LM_SESSION_REQUEST_FIX)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
            "Fix Request for Existing Emergency Session. Don't notify TechSel");
      }
    }
  }

  if (retVal == TRUE) /*lint !e506 !e731 */
  {
    if (sess_start_param.op_req & TM_OP_REQ_AIDING_DATA_STATUS)
    {
      lm_info_req_param.e_req_type = LM_REQUEST_AIDING_DATA_WISHLIST;
      lm_info_req_param.z_request.e_wishlist_req_type =
         tm_core_xlate_prtl_to_wishlist_src(sess_start_param.op_mode);

      retVal = lm_request(&lm_info_req_param);
    }

    if (sess_start_param.op_req & TM_OP_REQ_REF_LOC)
    {
      lm_info_req_param.e_req_type = LM_REQUEST_POS_ESTIMATE;
      retVal = lm_request(&lm_info_req_param);
    }
    if (sess_start_param.op_req & TM_OP_REQ_RF_INFO)
    {
      lm_info_req_param.e_req_type = LM_REQUEST_RF_INFO;

      lm_info_req_param.z_request.z_rf_info_request.e_serv_sys_type
         =  phone_state_info.srv_system;
      retVal = lm_request(&lm_info_req_param);
    }
  }
  return retVal;
}

/*===========================================================================

FUNCTION tm_core_sess_req_continue

DESCRIPTION
  This function is to figure out if TM-Core accepts session continue request from
  protocol sub-module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_sess_req_continue(tm_prtl_type                       prtl_type,
                                  tm_sess_handle_type                sess_handle,
                                  tm_sess_req_continue_param_s_type  sess_continue_param)
{
  boolean retVal = TRUE;
  lm_session_request_info_s_type lm_sess_req_param;
  lm_request_info_s_type         lm_info_req_param;
  tm_op_req_type                 new_op_req;
  tm_op_req_type                 existing_op_req;
  tm_cm_phone_state_info_s_type phone_state_info;
  ldm_client_id_e_type           e_ldm_client_id = LDM_CLIENT_ID_MAX;
  boolean                        is_concurrent = FALSE;
  uint32                         q_ldm_query_mask = 0;
  int                            prtl_tbl_idx = -1;
  tm_sess_req_start_param_s_type z_sess_start_req_param;
  pdsm_client_id_type            pd_client_id;
  ldm_client_id_e_type           e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type           e_secondary_client_id = LDM_CLIENT_ID_MAX;
  tm_loc_processing_client_info  *p_z_lpc_client;
  (void)prtl_type;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM-Core Recv'd Sess Continue Req: %lu, %lu, %u", prtl_type, sess_handle, tm_core_info.session_info.session_state);

  memset((void *)&lm_sess_req_param, 0, sizeof(lm_session_request_info_s_type));
  memset((void *)&lm_info_req_param, 0, sizeof(lm_request_info_s_type));
  memset((void *)&phone_state_info,  0, sizeof(tm_cm_phone_state_info_s_type));
  memset((void *)&z_sess_start_req_param, 0, sizeof(tm_sess_req_start_param_s_type));

  if (prtl_type != TM_PRTL_TYPE_STANDALONE)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Default to power_mode M2 for protocol %d", prtl_type);
    sess_continue_param.lppm_mode.e_PowerMode = TM_SESS_POWER_MODE_NORMAL;
    sess_continue_param.lppm_mode.q_TbmMsecs = 0;
  }

  /* Query TM-CM module to get current phone status */
  tm_cm_iface_get_phone_state(&phone_state_info);

  q_ldm_query_mask = ldm_client_query_state();

  if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:In E911; set appropriate client ID for 0X%x sess_handle", sess_handle);
    e_ldm_client_id = LDM_CLIENT_ID_E911;

    if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
    {
      if ((TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
          (TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
          (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle) ||
          (TM_SESS_HANDLE_E911       == sess_handle) ||
          (TM_SESS_HANDLE_1X_CP_MT   == sess_handle) ||
          (TM_SESS_HANDLE_1X_UP_NI   == sess_handle))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:In E911 concurrent; set appropriate client ID for 0X%x sess_handle", sess_handle);
        e_ldm_client_id = LDM_CLIENT_ID_E911;

        /* Check for concurrency for cases when mode changes after request capabilities */
        if (TM_SESS_OPERATION_MODE_MSA == sess_continue_param.op_mode)
        {
          /* Get main and secondary client  from LDM */
          tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

          if (LDM_CLIENT_ID_E911 == e_secondary_client_id) /* Apptrack is main client */
          {
            if ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0)
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:TM checking for Concurrent mode in request continue");
              if (tm_core_is_e911_concurrency_allowed(LDM_CLIENT_ID_E911, TRUE))
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Concurrent mode");
                is_concurrent  = TRUE;
              }
              else
              {
                /* Clean up the ongoing apptrack session in TM */
                tm_core_abort_recover(TRUE, TM_STOP_REASON_E911);

                /* Disable apptrack client from LDM*/
                if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
                {
                  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client with handle %u successfully disabled at LDM", sess_handle);
                }
                else
                {
                  MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client with handle already disabled at LDM");
                }

                tm_cm_iface_set_E911_state(phone_state_info.prev_e911_state);

                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                      "LDM:Reverted E911 state to prev E911 state = %d and abort ongoing apptrack", phone_state_info.prev_e911_state);
                /* Get updated E911 state */
                tm_cm_iface_get_phone_state(&phone_state_info);

                /* Switch the secondary client to main client */
                if (tm_lpc_client_update_client_type(LDM_CLIENT_ID_E911, LDM_CLIENT_TYPE_MAIN))
                {
                  /* Clear LPC control data for secondary client */
                  tm_lpc_clear_control_data();

                  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Secondary client %d switched as main client!", LDM_CLIENT_ID_E911);

                  /* TM-Core accepts the incoming request, so update TM-Core session info */
                  tm_core_info.session_info.active_sess_handle = sess_handle;
                  tm_core_info.session_info.from_protocol      = prtl_type;
                  tm_core_info.session_info.session_state      = TM_CORE_SESS_STATE_WAIT_AUTH;
                  tm_core_info.session_info.pd_param.pd_option.operation_mode            = PDSM_SESSION_OPERATION_MSASSISTED;

                  /* Retrive Client info to update sess_continue parameters from session start parameters from LPC DB*/
                  p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_E911);

                  if (NULL == p_z_lpc_client)
                  {
                    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:E911 client not found");
                    return FALSE;
                  }

                  sess_continue_param.num_fixes  = p_z_lpc_client->sess_params.num_fixes;
                  sess_continue_param.tbf_ms = p_z_lpc_client->sess_params.tbf_ms;

                  MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Session Continue params: op_mode %d, num_fixes %d, op_req %d, tbf_ms %d",
                        sess_continue_param.op_mode, sess_continue_param.num_fixes, sess_continue_param.op_req, sess_continue_param.tbf_ms);
                }
                else
                {
                  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Secondary client %d FAILED to switch as main client!", LDM_CLIENT_ID_E911);
                  return FALSE;
                }
              }
            }
          }
          else
          { /* E911 is main client */

            if (LDM_CLIENT_ID_APPTRK == e_secondary_client_id)
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Concurrency NOT allowed when MSA is main client. End apptrack.");

              /* Send PDAPI EVENTS before aborting the Apptrack session */
              (void)tm_lpc_generate_pd_event(LDM_CLIENT_ID_APPTRK, TM_STOP_REASON_E911);

              /* Disable apptrack client from LDM*/
              if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client successfully disabled at LDM");

                tm_cm_iface_set_E911_state(phone_state_info.prev_e911_state);
                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED,
                      "LDM:Reverted E911 state to prev E911 state = %d", phone_state_info.prev_e911_state);
                /* Get updated E911 state */
                tm_cm_iface_get_phone_state(&phone_state_info);
              }
              else
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack client failed to be disabled at LDM");
                return FALSE;
              }
            }

            MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Session Continue params: op_mode %d, num_fixes %d, op_req %d, tbf_ms %d",
                  sess_continue_param.op_mode, sess_continue_param.num_fixes, sess_continue_param.op_req, sess_continue_param.tbf_ms);
          }
        }
        else
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Concurrent mode");
          is_concurrent  = TRUE;
        }
      }
      else
      {
        if ((q_ldm_query_mask & LDM_CLIENT_APPTRK_MASK) != 0)
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Continue req in concurrent mode; set appropriate apptrack client ID for 0X%x sess_handle", sess_handle);
          e_ldm_client_id = LDM_CLIENT_ID_APPTRK;
          is_concurrent  = TRUE;
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Continue request neither for E911 or apptrack in concurrent mode %d", sess_handle);
          return FALSE;
        }
      }
    }
  }
  else
  {
    if ((q_ldm_query_mask & LDM_CLIENT_APPTRK_MASK) != 0)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Recieved continue request for AppTrack in legacy mode");
      e_ldm_client_id = LDM_CLIENT_ID_APPTRK;
    }
    else if ((q_ldm_query_mask & LDM_CLIENT_SINGLE_SHOT_MASK) != 0)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Recieved continue request for single-shot in legacy mode");
      e_ldm_client_id = LDM_CLIENT_ID_SINGLE_SHOT;
    }
      else if ((q_ldm_query_mask & LDM_CLIENT_E911_MASK) != 0)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Recieved continue request for NI client in legacy mode");
      e_ldm_client_id = LDM_CLIENT_ID_E911;
    }
  
  }

  /* Retrive Client info to update sess_continue parameters from session start parameters from LPC DB
    Ensure we copy the specialRequest Type*/
  if ( ( LDM_CLIENT_ID_APPTRK == e_ldm_client_id ) ||
       ( LDM_CLIENT_ID_SINGLE_SHOT == e_ldm_client_id ) )
  {
    p_z_lpc_client = lpc_get_loc_processing_client(e_ldm_client_id);

    if (NULL == p_z_lpc_client)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:client not found");
    }
    else
    {
      sess_continue_param.specialReqType.e_SpecialReq = PDSM_SESSION_DONOTUSE_SPECIAL_REQ_MAX; 
      if (PDSM_SESSION_SPECIAL_REQ_SHORT_CODES == p_z_lpc_client->sess_params.specialReqType.e_SpecialReq) 
      {
         sess_continue_param.specialReqType = p_z_lpc_client->sess_params.specialReqType;
      }
    }
  }

  if (LDM_CLIENT_ID_MAX != e_ldm_client_id)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Update client %d", e_ldm_client_id);
    if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_update_client(e_ldm_client_id, prtl_type, sess_handle, &sess_continue_param))
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:%d Client with handle %u successfully updated for continue at LDM", e_ldm_client_id, sess_handle);

      /* Get main and secondary client  from LDM */
      tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

      if (TRUE == is_concurrent)
      {
        if ((LDM_CLIENT_ID_E911 == e_ldm_client_id) &&
            (LDM_CLIENT_ID_E911 == e_secondary_client_id))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 Client with handle %u successfully updated for continue in concurrent mode", sess_handle);
          if (((sess_continue_param.op_req & TM_OP_REQ_AIDING_DATA_STATUS) != 0) &&
              (TM_SESS_OPERATION_MODE_MSB == sess_continue_param.op_mode))
          {
            /*Send cached aiding data wishlist*/
            /* Get cached aiding data from apptrack and provide it to SUPL call back */
            prtl_tbl_idx = tm_core_get_prtl_idx_by_type(prtl_type);
            if ((prtl_tbl_idx >= 0) && (prtl_tbl_idx < TM_PRTL_NUM))
            {
              if (tm_core_info.prtl_func_cb_table[prtl_tbl_idx].sess_info_fp)
              {
                (void)tm_core_info.prtl_func_cb_table[prtl_tbl_idx].sess_info_fp(sess_handle, PRTL_SESS_INFO_STATUS, up_cache_sess_info_param);
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Cached aiding data sent to UMTS_UP_NI E911 session");
              }
              else
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: sess_info_fp is NULL");
              }
            }
            else
            {
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: prtl_tbl_idx %d is out of bounds", prtl_tbl_idx);
            }
          }
          return TRUE;
        }
        else if ((LDM_CLIENT_ID_APPTRK == e_ldm_client_id) &&
                 (LDM_CLIENT_ID_APPTRK == e_secondary_client_id))
        {
          return TRUE;
        }
      }
    }
    else
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:%d Client with handle %u failed to be updated at LDM", e_ldm_client_id, sess_handle);
#ifdef   FEATURE_GNSS_DTF
      if ((FALSE == tm_dtf_events_db.pad_received.status) || (FALSE == b_tm_dtf_srvcc_ho_test_triggered))
      {
        return FALSE;
      }
#else
      return FALSE;
#endif
    }
  }
  else
  {
    if ((TM_PRTL_TYPE_1X_UP == prtl_type) ||
        (TM_PRTL_TYPE_DIAG == prtl_type))
    {
      /* Convert session continue params to session start params */
      tm_mapping_between_sess_start_sess_continue(FALSE, &z_sess_start_req_param, &sess_continue_param);

      /* Since tm_core session start has not been called, LDM client is not added. Add LDM client now*/
      if (z_sess_start_req_param.num_fixes > 1)
      {
        e_ldm_client_id = LDM_CLIENT_ID_APPTRK;
      }
      else
      {
        e_ldm_client_id = LDM_CLIENT_ID_SINGLE_SHOT;
      }

      if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
          (LDM_CLIENT_ID_SINGLE_SHOT == e_ldm_client_id))
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Single shot not allowed in concurrent mode");
        return (FALSE);
      }

      pd_client_id = tm_core_info.session_info.pd_param.client_id;

      if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_enable_client(e_ldm_client_id, prtl_type, sess_handle, &z_sess_start_req_param, pd_client_id, LDM_CLIENT_TYPE_MAIN))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Client %d enabled at LDM", e_ldm_client_id);
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Client %d failed to enable at LDM", e_ldm_client_id);
        return (FALSE);
      }
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Unknown client ID");
      return FALSE;
    }
  }

#ifdef FEATURE_GNSS_PERIODIC_PPM
  #error code not present
#endif
  /*
     This is the case where protocol sub-modules need to perform authetication procedure
     Protocol sub-modules finished authenticaion process and now is ready to kick off PD session
     At this time, end-to-end session to LM has NOT been established yet
  */

  /* TM-Core accepts the incoming request, so update TM-Core session info */
  if ((TM_SESS_HANDLE_E911 == tm_core_info.session_info.active_sess_handle) &&
      (prtl_type == TM_PRTL_TYPE_UMTS_CP) &&
      (sess_continue_param.op_req  != TM_OP_REQ_DEL_ASSIST)
      )
  {
    tm_core_info.session_info.active_sess_handle = sess_handle;
    tm_core_info.session_info.from_protocol      = prtl_type;
  }

  p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_APPTRK);

  if ((tm_core_info.config_info.u_disable_Dynamic_HEPE_usage & TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF) &&
      (TM_LOC_MAX_HIGH_HEPE_THRESHOLD > sess_continue_param.accuracy_threshold) &&
      ((NULL != p_z_lpc_client) && (TRUE == p_z_lpc_client->v_isFinalFix)))
  {
    sess_continue_param.accuracy_threshold = TM_LOC_MAX_HIGH_HEPE_THRESHOLD;
  }

  if ((sess_handle == tm_core_info.session_info.active_sess_handle) &&
      (tm_core_info.session_info.session_state == TM_CORE_SESS_STATE_WAIT_AUTH))
  {
    tm_core_update_session_state(TM_CORE_SESS_STATE_ACCESS_GRANTED);

    /* Update information in TM-Core */
    tm_core_info.session_info.op_mode            = sess_continue_param.op_mode;

    /*
      Now request session start to LM
      If life is good, LM should accept it.
      If there is something wrong, LM rejects it (asynchronously), we need to make sure we do recovery properly
    */

    lm_sess_req_param.q_handle = sess_handle;
    /*
     Since smart MSB is supported for 1x non trusted, we send REQUEST_CONTINUE for the MSB case and
     REQUEST_START for everything else.
    */
    if (tm_core_info.session_info.pd_param.pd_option.operation_mode == PDSM_SESSION_OPERATION_MSBASED)
    {
      lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_CONTINUE;
    }
    else
    {
      lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_START;
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Setting continue to START: mode=%u",
            tm_core_info.session_info.pd_param.pd_option.operation_mode);
    }
    lm_sess_req_param.q_req_mask = (uint32)LM_SESSION_REQUEST_SESSION;
    lm_sess_req_param.z_request.z_session_request.e_mode = tm_core_convert_op_mode(sess_continue_param.op_mode, prtl_type);
    lm_sess_req_param.z_request.z_session_request.e_mo_mt = tm_core_mo_mt_mode(sess_handle);
    lm_sess_req_param.z_request.z_session_request.q_num_fixes = sess_continue_param.num_fixes;
    lm_sess_req_param.z_request.z_session_request.q_tbf = sess_continue_param.tbf_ms;

    (void)tm_ConvertToLmLppmMode(&sess_continue_param.lppm_mode, &lm_sess_req_param.z_request.z_session_request.z_lppm_config);
    lm_sess_req_param.z_request.z_lppm_request = lm_sess_req_param.z_request.z_session_request.z_lppm_config;

    tm_core_info.session_info.q_num_fixes = lm_sess_req_param.z_request.z_session_request.q_num_fixes;
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Setting LM num_fixes = %u",
          tm_core_info.session_info.q_num_fixes);

    if ((tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_1X_CP)
        || (tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_UMTS_CP))
    {
      lm_sess_req_param.z_request.z_session_request.e_up_cp = LM_UP_CP_MODE_CP;
    }
    else
    {
      lm_sess_req_param.z_request.z_session_request.e_up_cp = LM_UP_CP_MODE_UP;
    }

    /*
      For the MSB mode, we fall through to the next (ACCESS_GRANTED) case, and set these request
      parameters there.
    */
    if (tm_core_info.session_info.pd_param.pd_option.operation_mode != PDSM_SESSION_OPERATION_MSBASED)
    {
      if (sess_continue_param.op_req & TM_OP_REQ_PPM)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_CDMA;
        lm_sess_req_param.z_request.z_cdma_request.q_timeout = sess_continue_param.ppm_qos * 1000;
      }

      if (sess_continue_param.op_req & TM_OP_REQ_PRM)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_GPS;
        lm_sess_req_param.z_request.z_gps_request.q_timeout = sess_continue_param.prm_qos * 1000;
        lm_sess_req_param.z_request.z_gps_request.u_qos     = sess_continue_param.prm_prq;
        lm_sess_req_param.z_request.z_gps_request.multi_report_msa = sess_continue_param.multi_report_msa;
      }

      if (sess_continue_param.op_req & TM_OP_REQ_LR)
      {
        lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_FIX;
        lm_sess_req_param.z_request.z_fix_request.q_timeout = sess_continue_param.lr_qos * 1000;
        lm_sess_req_param.z_request.z_fix_request.q_qos_accuracy_threshold  = sess_continue_param.accuracy_threshold;

        if (sess_continue_param.lppm_mode.e_PowerMode > TM_SESS_POWER_MODE_NORMAL)
        {
          /*Turn Off dynamic HEPE for Background LPPM Modes*/
          lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = FALSE;
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Session Continue: Dynamic HEPE Disabled");
        }
        else
        {
          lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = sess_continue_param.dynamic_qos_enabled;
        }
      }
    }

    /* Check to see whether Dynamic HEPE needs to enabled or disabled */
    if (((sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB) || (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB_OPT) ||
         (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_STANDALONE)) &&
        (TM_CM_E911_STATE_NONE == phone_state_info.e911_state) &&
        ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0) &&
        (tm_core_info.config_info.u_disable_Dynamic_HEPE_usage & TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "DYN_HEPE_DISABLED_FOR_BETTER_TTFF", 0);
      lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = FALSE;
    }

    /* Determine if data demod is allow for MSB */
    if (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB)
    {
      if ((tm_core_get_active_prtl_module(MT_NA) == TM_PRTL_TYPE_1X_UP)
          && (tm_core_info.config_info.smart_msb == FALSE))
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Data demod disabled", 0, 0, 0);
        lm_sess_req_param.z_request.z_session_request.u_do_data_demod = FALSE;
      }
      else
      {
        lm_sess_req_param.z_request.z_session_request.u_do_data_demod = TRUE;
      }
    }
    else
    {
      lm_sess_req_param.z_request.z_session_request.u_do_data_demod = TRUE;
    }
    retVal = lm_session_request(&lm_sess_req_param);
    if (lm_sess_req_param.q_req_mask & LM_SESSION_REQUEST_FIX)
    {
      tm_core_report_fix_start_to_techsel();
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Fix Request. Notify TechSel");

    }
    /*
     For the MSB case, the op_requested field is populated in the next block instead of here.
     We do so because the next block expects the tm_core_info.session_info.op_requested value
     to be untouched since the start of the function.
    */
    if (tm_core_info.session_info.pd_param.pd_option.operation_mode != PDSM_SESSION_OPERATION_MSBASED)
    {
      tm_core_info.session_info.op_requested |= sess_continue_param.op_req;
    }
    if (retVal == TRUE) /*lint !e506 !e731 */
    {
      /*
      Always get aiding data wishlist update
      so that we can populate alm_svmask and eph_svmask in PRM report.
      However, if protocol modules does not ask for aiding data update,
      don't forward aiding data wishlist to protocol modules,
      when it comes back from MGP
      */
      lm_info_req_param.e_req_type = LM_REQUEST_AIDING_DATA_WISHLIST;
      lm_info_req_param.z_request.e_wishlist_req_type =
         tm_core_xlate_prtl_to_wishlist_src(sess_continue_param.op_mode);

      retVal = lm_request(&lm_info_req_param);

      /*
      For the MSB case, these lm_request operations are sent in the next block instead of here,
      since we fall through to that block anyways.
      */
      if (tm_core_info.session_info.pd_param.pd_option.operation_mode != PDSM_SESSION_OPERATION_MSBASED)
      {
        if (sess_continue_param.op_req & TM_OP_REQ_REF_LOC)
        {
          lm_info_req_param.e_req_type = LM_REQUEST_POS_ESTIMATE;
          retVal = lm_request(&lm_info_req_param);
        }

        if (sess_continue_param.op_req & TM_OP_REQ_RF_INFO)
        {
          /* Query TM-CM module to get current acquired system */
          tm_cm_iface_get_phone_state(&phone_state_info);

          lm_info_req_param.e_req_type = LM_REQUEST_RF_INFO;
#ifdef FEATURE_CGPS_CDMA_IF
          if (phone_state_info.srv_system == CGPS_SRV_SYS_CDMA)
          {
            /* If HDR Call is connected, overwrite the srv_sys to HDR */
            if (tm_cm_iface_hdr_call_connected() == TRUE)
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "HDR call connected", 0, 0, 0);
              phone_state_info.srv_system = CGPS_SRV_SYS_HDR;
            }
          }
#endif /* FEATURE_CGPS_CDMA_IF */
          lm_info_req_param.z_request.z_rf_info_request.e_serv_sys_type
             =  phone_state_info.srv_system;
          retVal = lm_request(&lm_info_req_param);
        }
      }
    }

    if (retVal == FALSE)
    {
      /* LM does not allow the request, abort and recover */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      return retVal;
    }
    /* for the non MSB cases we return right here so as to not fall through to the next block*/
    if (tm_core_info.session_info.pd_param.pd_option.operation_mode != PDSM_SESSION_OPERATION_MSBASED)
    {
      return retVal;
    }
  }
  /*
    Protocol sub-modules has already start a end-to-end session to LM,
    It now calls continue to perform the call flow.

    Note: we intentionally fall through from the previous case into this block for MSB.
    The fall through happens since tm_core_info.session_info.session_state was set to
    TM_CORE_SESS_STATE_ACCESS_GRANTED in the previous block.
    Most of the requested aiding data flags (PPM, PRM, LR) are populated here.
    For the case when we fallback from V2 to SA, only the LR request flag is set in the
    incoming request.
  */
  if ((sess_handle == tm_core_info.session_info.active_sess_handle)
      && (tm_core_info.session_info.session_state == TM_CORE_SESS_STATE_ACCESS_GRANTED))
  {
#ifdef FEATURE_CGPS_XTRA
    if (tm_xtra_is_enable() &&
        (tm_core_info.session_info.msb_xtra_allowed == FALSE) &&
        (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB))
    {
      /* code below (featurized by FEATURE_CGPS_XTRA) is for feature controlled by OEM_FEAUTRE_NV(7165) bit 0x200:
         request XTRA download if XTRA data is invalid when MO MSB SUPL is initiated
         For 1x UP (MO or NI), controlled by bit 0x800 of NV 7165 */
      if (((tm_core_info.config_info.q_gnss_oem_feature_mask & NV_GNSS_OEM_FEATURE_MASK_10_REQ_XTRA_DURING_SUPL) &&
           (tm_core_mo_mt_mode(sess_handle) == LM_MO_MT_MODE_MO)) ||
          ((tm_core_info.config_info.q_gnss_oem_feature_mask & NV_GNSS_OEM_FEATURE_MASK_12_REQ_XTRA_DURING_1XUP) &&
           (tm_core_mo_mt_mode(sess_handle) == LM_MO_MT_MODE_MO)))
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Allow XTRA for AGPS MSB session (oemmask=0x%x handle=0x%x)",
              tm_core_info.config_info.q_gnss_oem_feature_mask, sess_handle, 0);
        tm_core_info.session_info.msb_xtra_allowed = TRUE;
        if (tm_xtra_data_validity_check(GPS_TIME_CHECK_SOURCE_PER_FIX))
        {
          // Successfully sent time request to MGP, set the flag
          tm_core_info.session_info.req_xtra_trig_by_agps = TRUE;
        }
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "No XTRA for MSB session (oemmask=0x%x opmode=%d handle=0x%x)",
              tm_core_info.config_info.q_gnss_oem_feature_mask, sess_continue_param.op_mode, sess_handle);
      }
    }
#endif /* FEATURE_CGPS_XTRA */

    /*
      Now forward continue request to LM
      If life is good, LM should accept it.
      If there is something wrong, LM rejects it (asynchronously), we need to make sure we do recovery properly
    */

    if (tm_core_info.session_info.op_mode == TM_SESS_OPERATION_MODE_MSB_OPT &&
        sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB)
    {
      /* if protocol sent a continue for the optimal modes or MSB, just continue */
      tm_core_info.session_info.op_mode            = sess_continue_param.op_mode;
    }
    else
    {
      /* If operation mode has been changed (Optimal mode fallback sessions, or E911 that starts with MSB and now the network
         requests MSA for example ), notify LM to change the mode*/
      if ((tm_core_info.session_info.op_mode != sess_continue_param.op_mode)
          && (sess_continue_param.op_mode != TM_SESS_OPERATION_MODE_OTDOA))
      {

        MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED,
              "Exiting op mode (%u) != cont.op_mode (%u), tm_core.op_requested = %x",
              tm_core_info.session_info.op_mode,
              sess_continue_param.op_mode,
              tm_core_info.session_info.op_requested,
              sess_continue_param.op_req);

        tm_core_info.session_info.op_mode = sess_continue_param.op_mode;
        lm_sess_req_param.q_handle = sess_handle;
        lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_CONTINUE;
        lm_sess_req_param.q_req_mask = (uint32)LM_SESSION_REQUEST_SESSION;
        lm_sess_req_param.z_request.z_session_request.e_mode = tm_core_convert_op_mode(sess_continue_param.op_mode, prtl_type);
        lm_sess_req_param.z_request.z_session_request.e_mo_mt = tm_core_mo_mt_mode(sess_handle);

        /* Determine if data demod is allow for MSB */
        if (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB)
        {
          if ((tm_core_get_active_prtl_module(MT_NA) == TM_PRTL_TYPE_1X_UP)
              && (tm_core_info.config_info.smart_msb == FALSE))
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Data demod disabled", 0, 0, 0);
            lm_sess_req_param.z_request.z_session_request.u_do_data_demod = FALSE;
          }
          else
          {
            lm_sess_req_param.z_request.z_session_request.u_do_data_demod = TRUE;
          }
        }
        else
        {
          lm_sess_req_param.z_request.z_session_request.u_do_data_demod = TRUE;
        }
        retVal = lm_session_request(&lm_sess_req_param);
      }

      if (retVal)
      {
        lm_sess_req_param.q_req_mask = 0;

        /*
           For operatons which have not been previously started, use start to LM
           For operatons which have already been started, use continue to LM
        */
        existing_op_req = sess_continue_param.op_req & tm_core_info.session_info.op_requested;
        new_op_req      = sess_continue_param.op_req & ~(existing_op_req);

        MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
              "continue_param.op_req = %lu, tm_core_info.session_info.op_requested = %lu",
              sess_continue_param.op_req, tm_core_info.session_info.op_requested);

        /*
           For operatons which have not been previously started, use start to LM
        */
        if (new_op_req & (TM_OP_REQ_PPM | TM_OP_REQ_PRM | TM_OP_REQ_LR))
        {
          lm_sess_req_param.q_handle   = sess_handle;
          lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_START;

          if (new_op_req & TM_OP_REQ_PPM)
          {
            lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_CDMA;
            lm_sess_req_param.z_request.z_cdma_request.q_timeout = sess_continue_param.ppm_qos * 1000;
          }

          if (new_op_req & TM_OP_REQ_PRM)
          {
            lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_GPS;
            lm_sess_req_param.z_request.z_gps_request.q_timeout = sess_continue_param.prm_qos * 1000;
            lm_sess_req_param.z_request.z_gps_request.u_qos     = sess_continue_param.prm_prq;
            lm_sess_req_param.z_request.z_gps_request.multi_report_msa = sess_continue_param.multi_report_msa;
          }

          if (new_op_req & TM_OP_REQ_LR)
          {
            lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_FIX;
            lm_sess_req_param.z_request.z_fix_request.q_timeout = sess_continue_param.lr_qos * 1000;
            lm_sess_req_param.z_request.z_fix_request.q_qos_accuracy_threshold  = sess_continue_param.accuracy_threshold;

            if (sess_continue_param.lppm_mode.e_PowerMode > TM_SESS_POWER_MODE_NORMAL)
            {
              /*Turn Off dynamic HEPE for Background LPPM Modes*/
              lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = FALSE;
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "New Op. Session Continue: Dynamic HEPE Disabled");
            }
            else
            {
              lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = sess_continue_param.dynamic_qos_enabled;
            }
          }

          /* Check to see whether Dynamic HEPE needs to enabled or disabled */
          if (((sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB) || (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB_OPT) ||
               (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_STANDALONE)) &&
              (TM_CM_E911_STATE_NONE == phone_state_info.e911_state) &&
              ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0) &&
              (tm_core_info.config_info.u_disable_Dynamic_HEPE_usage & TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF))
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "DYN_HEPE_DISABLED_FOR_BETTER_TTFF", 0);
            lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = FALSE;
          }

          retVal = lm_session_request(&lm_sess_req_param);
          if (lm_sess_req_param.q_req_mask & LM_SESSION_REQUEST_FIX)
          {
            tm_core_report_fix_start_to_techsel();
          }
        }

        tm_core_info.session_info.op_requested |= new_op_req;


        /*
           For operations which have already been previously started, use continue to LM
        */
        if (existing_op_req & (TM_OP_REQ_PPM | TM_OP_REQ_PRM | TM_OP_REQ_LR))
        {
          lm_sess_req_param.q_handle   = sess_handle;
          lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_CONTINUE;

          if (existing_op_req & TM_OP_REQ_PPM)
          {
            lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_CDMA;
            lm_sess_req_param.z_request.z_cdma_request.q_timeout = sess_continue_param.ppm_qos * 1000;
          }

          if (existing_op_req & TM_OP_REQ_PRM)
          {
            lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_GPS;
            lm_sess_req_param.z_request.z_gps_request.q_timeout = sess_continue_param.prm_qos * 1000;
            lm_sess_req_param.z_request.z_gps_request.u_qos     = sess_continue_param.prm_prq;
            lm_sess_req_param.z_request.z_gps_request.multi_report_msa = sess_continue_param.multi_report_msa;
          }

          if (existing_op_req & TM_OP_REQ_LR)
          {
            lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_FIX;
            lm_sess_req_param.z_request.z_fix_request.q_timeout = sess_continue_param.lr_qos * 1000;
            lm_sess_req_param.z_request.z_fix_request.q_qos_accuracy_threshold  = sess_continue_param.accuracy_threshold;
            if (sess_continue_param.lppm_mode.e_PowerMode > TM_SESS_POWER_MODE_NORMAL)
            {
              /*Turn Off dynamic HEPE for Background LPPM Modes*/
              lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = FALSE;
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Ex Op. Session Continue: Dynamic HEPE Disabled");
            }
            else
            {
              lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = sess_continue_param.dynamic_qos_enabled;
            }
          }

          /* Check to see whether Dynamic HEPE needs to enabled or disabled */
          if (((sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB) || (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_MSB_OPT) ||
               (sess_continue_param.op_mode == TM_SESS_OPERATION_MODE_STANDALONE)) &&
              (TM_CM_E911_STATE_NONE == phone_state_info.e911_state) &&
              ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0) &&
              (tm_core_info.config_info.u_disable_Dynamic_HEPE_usage & TM_DYN_HEPE_DISABLED_FOR_BETTER_TTFF))
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "DYN_HEPE_DISABLED_FOR_BETTER_TTFF", 0);
            lm_sess_req_param.z_request.z_fix_request.u_dynamic_qos_enabled = FALSE;
          }
          retVal = lm_session_request(&lm_sess_req_param);
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Existing Operation Request. Not sending FIX start to TechSel");
        }
      }
    }

    if (retVal == TRUE) /*lint !e506 !e731 */
    {
      if (sess_continue_param.op_req & TM_OP_REQ_AIDING_DATA_STATUS)
      {
        lm_info_req_param.e_req_type = LM_REQUEST_AIDING_DATA_WISHLIST;
        lm_info_req_param.z_request.e_wishlist_req_type =
           tm_core_xlate_prtl_to_wishlist_src(sess_continue_param.op_mode);

        retVal = lm_request(&lm_info_req_param);
      }

      if (sess_continue_param.op_req & TM_OP_REQ_REF_LOC)
      {
        lm_info_req_param.e_req_type = LM_REQUEST_POS_ESTIMATE;
        retVal = lm_request(&lm_info_req_param);
      }
      if (sess_continue_param.op_req & TM_OP_REQ_RF_INFO)
      {
        /* Query TM-CM module to get current acquired system */
        tm_cm_iface_get_phone_state(&phone_state_info);

        lm_info_req_param.e_req_type = LM_REQUEST_RF_INFO;

#ifdef FEATURE_CGPS_CDMA_IF
        if (phone_state_info.srv_system == CGPS_SRV_SYS_CDMA)
        {
          /* If HDR Call is connected, overwrite the srv_sys to HDR */
          if (tm_cm_iface_hdr_call_connected() == TRUE)
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "HDR call connected", 0, 0, 0);
            phone_state_info.srv_system = CGPS_SRV_SYS_HDR;
          }
        }
#endif /* FEATURE_CGPS_CDMA_IF */
        lm_info_req_param.z_request.z_rf_info_request.e_serv_sys_type
           =  phone_state_info.srv_system;
        retVal = lm_request(&lm_info_req_param);
      }
    }

    if (retVal == FALSE)
    {
      /* LM does not allow the request, abort and recover */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    }
  }
  /*
    Return error if either the session handle is invalid or the session state was
    neither WAIT_AUTH nor GRANTED.
 */
  if ((sess_handle != tm_core_info.session_info.active_sess_handle) ||
      ((tm_core_info.session_info.session_state != TM_CORE_SESS_STATE_WAIT_AUTH) &&
       (tm_core_info.session_info.session_state != TM_CORE_SESS_STATE_ACCESS_GRANTED)))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Continue req rejected", 0, 0, 0);
    retVal = FALSE;
  }

  return retVal;
}

/*===========================================================================

FUNCTION tm_core_sess_req_polling_data

DESCRIPTION
  This function is used to poll LM is data is available

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_sess_req_polling_data(tm_prtl_type                           prtl_type,
                                      tm_sess_handle_type                    sess_handle,
                                      tm_sess_req_polling_data_param_s_type  polling_data_param)
{
  boolean     retVal         = FALSE;

  lm_session_request_info_s_type lm_sess_req_param;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM-Core Recv'd Polling Data Req", 0, 0, 0);

  memset((void *)&lm_sess_req_param, 0, sizeof(lm_session_request_info_s_type));

  /*
   * Only accept if
   *** continue req belongs to active session and session is in access granted state OR
   *** continue req belongs to Periodic PPM Session
   */
  if (
      ((sess_handle == tm_core_info.session_info.active_sess_handle) &&
       (tm_core_info.session_info.session_state == TM_CORE_SESS_STATE_ACCESS_GRANTED))
      )
  {
    if (polling_data_param.op_req & TM_OP_REQ_PPM)
    {
      lm_sess_req_param.q_handle    = tm_core_info.session_info.active_sess_handle;
      lm_sess_req_param.e_req_type  = LM_SESSION_REQUEST_CONTINUE;
      lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_CDMA_GET_RESULTS;
      retVal = lm_session_request(&lm_sess_req_param);
    }

    if (retVal == FALSE)
    {
      /* LM does not allow the request, abort and recover */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Polling Data req rejected", 0, 0, 0);
    retVal = FALSE;
  }

  return retVal;
}
/*===========================================================================

FUNCTION tm_core_msb_sess_handover

DESCRIPTION
  This function is called when standalone module decide other protocol sub-module
  should be handling MSB session.

  It pickup the current active protocol sub-module to proceed with MSB session.

  The transition is invisible to applicatons

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_msb_sess_handover(void)
{
  int                                prtl_tbl_idx = -1;
  tm_prtl_type                       active_prtl;
  prtl_start_sess_req_result_e_type  req_result;
  tm_sess_req_start_param_s_type     start_actions;
  tm_sess_req_continue_param_s_type  continue_param;
  boolean                            retVal = FALSE;

  /* the client type here does not matter, as long as it is not NI */
  active_prtl = tm_core_get_active_prtl_module(MT_NA);

  prtl_tbl_idx = tm_core_get_prtl_idx_by_type(active_prtl);

  if ((prtl_tbl_idx >= 0) && (prtl_tbl_idx < TM_PRTL_NUM))
  {
    if (tm_core_info.prtl_func_cb_table[prtl_tbl_idx].start_sess_req_fp)
    {
      req_result = tm_core_info.prtl_func_cb_table[prtl_tbl_idx].start_sess_req_fp(
         tm_core_info.session_info.active_sess_handle,
         &(tm_core_info.session_info.pd_param),
         &start_actions);

      if (req_result == PRTL_START_SESS_REQ_ALLOWED)
      {
        /* TM-Core is already in a active session, we call continue to do seemless transition */
        continue_param.op_mode            = start_actions.op_mode;
        continue_param.op_req             = start_actions.op_req;
        continue_param.accuracy_threshold = start_actions.accuracy_threshold;
        continue_param.ppm_qos            = start_actions.ppm_qos;
        continue_param.prm_qos            = start_actions.prm_qos;
        continue_param.lr_qos             = start_actions.lr_qos;
        continue_param.dynamic_qos_enabled  = start_actions.dynamic_qos_enabled;
        continue_param.multi_report_msa     = start_actions.multi_report_msa;
        continue_param.lppm_mode            = start_actions.lppm_mode;
        tm_core_info.session_info.from_protocol      = active_prtl;
        continue_param.specialReqType       = start_actions.specialReqType;

        retVal = tm_core_sess_req_continue(active_prtl,
                                           tm_core_info.session_info.active_sess_handle,
                                           continue_param);
      }
      else if (req_result == PRTL_START_SESS_REQ_WAIT)
      {
        /* TM-Core is already in a active session, we call continue to do seemless transition */
        continue_param.op_mode            = start_actions.op_mode;
        continue_param.op_req             = start_actions.op_req;
        continue_param.accuracy_threshold = start_actions.accuracy_threshold;
        continue_param.ppm_qos            = start_actions.ppm_qos;
        continue_param.prm_qos            = start_actions.prm_qos;
        continue_param.lr_qos             = start_actions.lr_qos;
        continue_param.dynamic_qos_enabled = start_actions.dynamic_qos_enabled;
        continue_param.multi_report_msa    = start_actions.multi_report_msa;
        continue_param.lppm_mode           = start_actions.lppm_mode;
        tm_core_info.session_info.from_protocol      = active_prtl;
        continue_param.specialReqType       = start_actions.specialReqType;

        retVal = tm_core_sess_req_continue(active_prtl, tm_core_info.session_info.active_sess_handle, continue_param);

        /* If prototocol return PRTL_START_SESS_REQ_WAIT, set tm-core session state to wait */
        /* Protocol sub-modules should call session continue to continue this session */
        tm_core_info.session_info.from_protocol      = active_prtl;
        tm_core_update_session_state(TM_CORE_SESS_STATE_WAIT_AUTH);
      }
      else
      {
        /* If protocol return PRTL_START_SESS_REQ_NOT_ALLOWED, report to PD API */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Protocol sub-module rejects start session request", 0, 0, 0);
        retVal = FALSE;
      }
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid prtlTableIndex", 0, 0, 0);
      retVal = FALSE;
    }

  }
  else
  {

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid prtlTableIndex", 0, 0, 0);
    retVal = FALSE;
  }

  if (retVal == FALSE)
  {
    //<<<[+ by Liao] Revist to clean LM and protocols properly
    tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    //>>>[+ End]
  }
}


/*===========================================================================

FUNCTION tm_core_sess_req_stop

DESCRIPTION
  This function is to figure out if TM-Core accepts stop request from
  protocol sub-module.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_sess_req_stop(tm_prtl_type                       prtl_type,
                              tm_sess_handle_type                sess_handle,
                              tm_sess_req_stop_param_s_type      sess_stop_param)
{
  boolean retVal = FALSE;
  lm_session_request_info_s_type lm_sess_req_param;
  tm_cm_phone_state_info_s_type phone_state_info;
  uint32 q_ldm_query_mask = 0;
  tm_loc_processing_client_info        *p_z_lpc_client;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM-Core Recv'd Sess Stop Req from %d, sess_handle %d", prtl_type, sess_handle, 0);

  tm_cm_iface_get_phone_state(&phone_state_info);


  /* Case when download is allowed in concurrent mode */
  if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) &&
      (TM_STOP_REASON_MSB_NEED_DLOAD == sess_stop_param.stop_reason)
      )
  {
    /* Do not abort the session */
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:MSB download is requested. Not disabling client");
  }
  else if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
  {
    if ((TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
        (TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
        (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle) ||
        (TM_SESS_HANDLE_E911       == sess_handle) ||
        (TM_SESS_HANDLE_1X_CP_MT   == sess_handle) ||
        (TM_SESS_HANDLE_1X_UP_NI   == sess_handle))

    {
      p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_E911);

      if (NULL == p_z_lpc_client)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:E911 client not found");
        return FALSE;
      }

      if (LDM_CLIENT_TYPE_SECONDARY == p_z_lpc_client->e_clientId_type)
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 Secondary Client with handle %u to be disabled", sess_handle);
        if (sess_stop_param.stop_type == TM_STOP_TYPE_SESSION)
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 session stop reason %d; generate PD event", sess_stop_param.stop_reason);
          (void)tm_lpc_generate_pd_event(LDM_CLIENT_ID_E911, sess_stop_param.stop_reason);
        }

        if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_E911))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 Client with handle %u successfully disabled at LDM", sess_handle);
          return TRUE;
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 Client with handle %u failed to be disabled at LDM", sess_handle);
          return FALSE;
        }
      }
      else if (LDM_CLIENT_TYPE_MAIN == p_z_lpc_client->e_clientId_type)
      {
        /* Set the correct pdapi_client id */
        tm_core_info.session_info.pd_param.client_id = p_z_lpc_client->pd_clientId;

        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 Main Client with handle %u to be disabled", sess_handle);
        if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_E911))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 Client with handle %u successfully disabled at LDM", sess_handle);
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 Client with handle %u failed to be disabled at LDM", sess_handle);
          return FALSE;
        }
        (void)tm_core_switch_secondary_to_main_client(LM_MGP_RCVR_OFF_TM_SESS_END, FALSE);
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: Client type not set for E911 Client with handle %u failed to be disabled at LDM", sess_handle);
        return FALSE;
      }
    }
    else if ((TM_SESS_HANDLE_FORCE_STOP == sess_handle) &&
             (TM_STOP_TYPE_SESSION == sess_stop_param.stop_type))
    {
      /*About to start a protocol session;protocol forced session termination.
        In concurrent mode do nothing.*/
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: In concurrent mode, recieved TM_SESS_HANDLE_FORCE_STOP.");
      return TRUE;
    }
    else
    {
      p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_APPTRK);

      if (NULL == p_z_lpc_client)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC: Apptrack client not found");
        return FALSE;
      }

      if (LDM_CLIENT_TYPE_MAIN == p_z_lpc_client->e_clientId_type)
      {
        /* Set the correct pdapi_client id */
        tm_core_info.session_info.pd_param.client_id = p_z_lpc_client->pd_clientId;

        if (((tm_core_info.session_info.q_num_fixes > 1) || (tm_core_info.session_info.q_num_fixes < 1)) &&
            (TM_STOP_REASON_COMPLETED != sess_stop_param.stop_reason) &&
            (TM_STOP_REASON_SUPL_SESS_PREEMT != sess_stop_param.stop_reason))
        {
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client with handle %u successfully disabled at LDM", sess_handle);
          }
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack client with handle %u failed to be disabled at LDM", sess_handle);
            return FALSE;
          }
        }
        else if (tm_core_info.session_info.q_num_fixes == 1)
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Single shot Client with handle %u cannot be in concurrent state", sess_handle);
          return FALSE;
        }
      }
      else if (LDM_CLIENT_TYPE_SECONDARY == p_z_lpc_client->e_clientId_type)
      {
        if (sess_stop_param.stop_type == TM_STOP_TYPE_SESSION)
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack session stop reason %d; generate PD event", sess_stop_param.stop_reason);
          (void)tm_lpc_generate_pd_event(LDM_CLIENT_ID_APPTRK, sess_stop_param.stop_reason);
        }

        if (TM_STOP_REASON_COMPLETED != sess_stop_param.stop_reason)
        {
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client with handle %u successfully disabled at LDM", sess_handle);
            return TRUE;
          }
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack client with handle %u failed to be disabled at LDM", sess_handle);
            return FALSE;
          }
        }
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM: Client type not set for Apptrack client with handle %u failed to be disabled at LDM", sess_handle);
        return FALSE;
      }
    }
  }
  else
  {
    if ((TM_STOP_REASON_MSB_NEED_DLOAD != sess_stop_param.stop_reason) &&
        ((sess_handle == tm_core_info.session_info.active_sess_handle) ||
         (sess_handle == TM_SESS_HANDLE_FORCE_STOP))
        )
    {
      q_ldm_query_mask = ldm_client_query_state();

      if ((q_ldm_query_mask & LDM_CLIENT_SINGLE_SHOT_MASK) != 0)
      {
        if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_SINGLE_SHOT))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Single shot client with handle %u successfully disabled at LDM", sess_handle);
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Single shot Client with handle %u failed to be disabled at LDM", sess_handle);
          return FALSE;
        }
      }
      else if ((q_ldm_query_mask & LDM_CLIENT_E911_MASK) != 0)
      {
        if ((TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
            (TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
            (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle) ||
            (TM_SESS_HANDLE_E911       == sess_handle) ||
            (TM_SESS_HANDLE_1X_CP_MT   == sess_handle) ||
            (TM_SESS_HANDLE_1X_UP_NI   == sess_handle))

        {
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_E911))
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 client with handle %u successfully disabled at LDM", sess_handle);
          }
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 client with handle %u failed to be disabled at LDM", sess_handle);
            return FALSE;
          }
        }
      }
      else if ((q_ldm_query_mask & LDM_CLIENT_APPTRK_MASK) != 0)

      {
        if (TRUE == tm_core_is_internal_session_on())
        {
          /* Abort internal sessions without looking at any handle */
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client (Internal session) with handle %u successfully disabled at LDM", sess_handle);
          }
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack client (Internal session) with handle %u failed to be disabled at LDM", sess_handle);
            return FALSE;
          }
        }
        else if ((TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
                 (TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
                 (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle) ||
                 (TM_SESS_HANDLE_E911       == sess_handle) ||
                 (TM_SESS_HANDLE_1X_CP_MT   == sess_handle) ||
                 (TM_SESS_HANDLE_1X_UP_NI   == sess_handle))

        {
          /* During switch from concurrent to non-concurrent mode, protocol will stop the session, but if E911 client is already removed and apptrack is on going, 
                      we dont want apptracking to be aborted */
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client cannot have this handle %d. Not aborting ongoing apptrack session", sess_handle);
          return TRUE;
        }
        else if (TM_STOP_REASON_COMPLETED != sess_stop_param.stop_reason)
        {
          if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack client with handle %u successfully disabled at LDM", sess_handle);
          }
          else
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Apptrack client with handle %u failed to be disabled at LDM", sess_handle);
            return FALSE;
          }
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: No client in LDM to be disabled. Continue to cleanup TM/LM");
      }
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:MSB download is requested OR active handle does not match. Not disabling client");
    }
  }
  memset((void *)&lm_sess_req_param, 0, sizeof(lm_session_request_info_s_type));

  q_ldm_query_mask = ldm_client_query_state();
  
  /* If stop req belongs to active session and session is not in init state */
  if ((sess_handle == tm_core_info.session_info.active_sess_handle)
      && (tm_core_info.session_info.session_state != TM_CORE_SESS_STATE_INIT))
  {
#ifdef FEATURE_GNSS_LPPE
    if ( ( phone_state_info.e911_state != TM_CM_E911_STATE_CONCURRENT ) &&
         ( FALSE == tm_lpc_client_special_req_check() ) )
    {
      /* If protocol session will close the TM session in the end. Additionally ensure the ODCPI
         is also stopped if running .*/
      tm_stopCPI(TM_AUXTECH_REQ_ID_LPPE);
      le_TrackPosConsistencyStartStop(FALSE);
    }
#endif /* FEATURE_GNSS_LPPE */
    if (sess_stop_param.stop_type == TM_STOP_TYPE_OPERATION)
    {
      if (sess_stop_param.op_to_stop & (TM_OP_REQ_PPM | TM_OP_REQ_PRM | TM_OP_REQ_LR))
      {
        lm_sess_req_param.e_req_type = LM_SESSION_REQUEST_STOP;
        lm_sess_req_param.q_handle   = sess_handle;

        if (sess_stop_param.op_to_stop & TM_OP_REQ_PPM)
        {
          lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_CDMA;
        }

        if (sess_stop_param.op_to_stop & TM_OP_REQ_PRM)
        {
          lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_GPS;
        }

        if (sess_stop_param.op_to_stop & TM_OP_REQ_LR)
        {
          lm_sess_req_param.q_req_mask |= (uint32)LM_SESSION_REQUEST_FIX;
        }

        tm_core_info.session_info.op_requested &= ~sess_stop_param.op_to_stop;
        retVal = lm_session_request(&lm_sess_req_param);
      }

      if (retVal == FALSE)
      {
        tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      }
    }
    else if (sess_stop_param.stop_type == TM_STOP_TYPE_SESSION)
    {
      /* MSB session handover logic
         Only allow session handover when all the following conditions are satisfied
         1. OP mode is MSB
         2. Phone is not in OoS (because data download is needed)
      */
      if (sess_stop_param.stop_reason == TM_STOP_REASON_MSB_NEED_DLOAD)
      {
        tm_core_msb_sess_handover();
      }
      else
      {
        /* No need to abort protocol sub-module again, because protocol sub-module initiated the abort */
        tm_core_abort_recover(FALSE, sess_stop_param.stop_reason); /*lint !e506 !e730 */
      }
      retVal = TRUE; /*lint !e506 */
    }
  }
  else if ((sess_handle == TM_SESS_HANDLE_FORCE_STOP)
           && (tm_core_info.session_info.session_state != TM_CORE_SESS_STATE_INIT)
           && (sess_stop_param.stop_type == TM_STOP_TYPE_SESSION))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Protocl forced session termination", 0, 0, 0);
    tm_core_abort_recover(FALSE, sess_stop_param.stop_reason); /*lint !e506 !e730 */
    retVal = TRUE;
  }
  /* Remove any secondary client when protocol clears TM-core session */         
  else if ((q_ldm_query_mask & LDM_CLIENT_E911_MASK) != 0)
  {      
    if ((TM_SESS_HANDLE_UMTS_CP_NI == sess_handle) ||
        (TM_SESS_HANDLE_UMTS_UP_NI == sess_handle) ||
        (TM_SESS_HANDLE_LPP_CP_NI  == sess_handle) ||
        (TM_SESS_HANDLE_E911       == sess_handle) ||
        (TM_SESS_HANDLE_1X_CP_MT   == sess_handle) ||
        (TM_SESS_HANDLE_1X_UP_NI   == sess_handle))
    {
      p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_E911);

      if (NULL == p_z_lpc_client)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LPC:E911 client not found");
        return FALSE;
      }
     
      if (LDM_CLIENT_TYPE_SECONDARY == p_z_lpc_client->e_clientId_type)
      {      
        if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_E911))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 client with handle %u successfully disabled at LDM", sess_handle);
          retVal = TRUE;
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 client with handle %u failed to be disabled at LDM", sess_handle);
          return FALSE;
        }
      }
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Stop req rejected", 0, 0, 0);
    retVal = FALSE;
  }
#ifdef FEATURE_CGPS_XTRA_T
  /* Notify TLE's XTM task that thre is no active AGPS session. */
  if (retVal == TRUE)
  {
    if (sess_stop_param.stop_type == TM_STOP_TYPE_SESSION)
    {
      (void)tle_SendAGPSEvents(C_TLE_EVENT_AGPS_PDCOMM_DISCONNECTED);
    }

  }
#endif
  return retVal;
}


/*===========================================================================

FUNCTION tm_core_recv_data

DESCRIPTION
  This function is to handle data sent from protocol sub-modules.

DEPENDENCIES

RETURN VALUE
  TURE: If TM-Core sub-module accepts it
  FALSE: Otherwise

SIDE EFFECTS

===========================================================================*/
boolean tm_core_recv_data(tm_prtl_type                     prtl_type,
                          tm_sess_handle_type              sess_handle,
                          tm_post_data_e_type              data_type,
                          const tm_post_data_payload_type  *data_payload_ptr
                          )
{
  boolean                  retVal        = FALSE;
  lm_info_s_type           *lm_data_ptr  = NULL;
  sm_FixInfoStructType      z_fix;
  gnss_AssistDataUnionType *p_AssistData = NULL;
  sm_gm_pos_fix_info       fix_info;
  tm_cm_phone_state_info_s_type phone_state_info;

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM-Core Recv'd data type %d from protocol %d", data_type, prtl_type);

  if (data_payload_ptr == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL ptr");
    return (FALSE);
  }

  memset((void *)&phone_state_info, 0, sizeof(phone_state_info));
  tm_cm_iface_get_phone_state(&phone_state_info);

  /* Post Data is allowed only when session is in access granted state */
  if ((TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state) ||
      ((sess_handle == tm_core_info.session_info.active_sess_handle)
       && (tm_core_info.session_info.session_state == TM_CORE_SESS_STATE_ACCESS_GRANTED)))
  {
    lm_data_ptr = (lm_info_s_type *)pdsm_getbuf(sizeof(lm_info_s_type));

    if (lm_data_ptr == NULL)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Fail to get buffer");
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      return (FALSE);
    }
    memset(lm_data_ptr, 0, sizeof(*lm_data_ptr));
    lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistDataSource = GNSS_ASSIST_DATA_SOURCE_AGNSS;
    p_AssistData = &(lm_data_ptr->z_info.z_aiding_data.z_assist_data.z_AssistData);
    switch (data_type)
    {
      case TM_POST_DATA_TYPE_AA:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_ACQ_ASSIST;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_SvAcqAssistInfo = data_payload_ptr->gnss_aa_data;
        /*If in concurrent mode maintain a mask for list of SVs assistance was provided.*/
        if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Update aiding info for E911 for GPS");
          tm_lpc_update_for_aiding_data(LDM_CLIENT_ID_E911,
                                        MGP_GNSS_TYPE_GPS,
                                        &(p_AssistData->z_SvAcqAssistInfo));
        }
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_SV_DIR:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_SV_DIR;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_GnssSvDirInfo = data_payload_ptr->gnss_sv_dir;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_SA:
        /*AHSIAU_OBSOLETE?
lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_SA_DATA;
lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
p_AssistData->z_SADataInfo = data_payload_ptr->sa_data;
retVal = lm_info(lm_data_ptr);
*/      break;

      case TM_POST_DATA_TYPE_IONO:
#ifndef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_IONO;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_IonoInfo = data_payload_ptr->iono_data;
        retVal = lm_info(lm_data_ptr);
#else
        #error code not present
#endif
        break;

      case TM_POST_DATA_TYPE_EPH:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_EPH;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_EphInfo = data_payload_ptr->eph_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_ALM:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_ALM;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_AlmInfo = data_payload_ptr->alm_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_SV_NO_EXIST:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_SV_NO_EXIST;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_SvNoExistInfo = data_payload_ptr->sv_no_exist_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_GNSS_SV_NO_EXIST:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_SV_NO_EXIST;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GLO;
        p_AssistData->z_GloNonExistenceInfo = data_payload_ptr->sv_no_exist_glo_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_SEED:
      case TM_POST_DATA_TYPE_LR:

        /*Generate NMEA*/
        if (data_type == TM_POST_DATA_TYPE_LR)
        {

          /* even if no NMEA output, still need to call this function
             as z_fix will be used in tm_core_generate_ext_pd_data() call */
          tm_nmea_get_gps_fix_from_refpos(&(data_payload_ptr->seed_data),
                                          &z_fix);

          tm_core_trigger_fix_related_NMEA(&z_fix, FALSE);

          /* Generate extended pd data */
          tm_core_generate_ext_pd_data(&z_fix);

          /*Send the LR to Geofence module*/
          fix_info.fix_type = GEOFENCE_POS_FIX_TYPE_FINAL;
          fix_info.y_pos_fix_ext_info.z_gnss_fix_ext_info.b_is_nw_asst_fix = TRUE;
          fix_info.fix_src = GEOFENCE_POS_FIX_SRC_GNSS;
          fix_info.pos_fix = z_fix;

          (void)gm_position_update(&fix_info);
        }

        /*Send seed or reference position over to LM*/
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_REF_LOC;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;

        /* force the network position injection during emergency */
        if (phone_state_info.e911_state == TM_CM_E911_STATE_NONE)
        {
          lm_data_ptr->z_info.z_aiding_data.z_assist_data.u_Force = FALSE;
        }
        else
        {
          /* Force to TRUE only if CPI injections are allowed through NV */
          if (tm_core_get_emergency_support_config() & TM_E911_CPI_INJECTIONS_ENABLE)
          {
            lm_data_ptr->z_info.z_aiding_data.z_assist_data.u_Force = TRUE;
          }
          else
          {
            lm_data_ptr->z_info.z_aiding_data.z_assist_data.u_Force = FALSE;
          }
        }
        p_AssistData->z_RefLocInfo = data_payload_ptr->seed_data;
        retVal = lm_info(lm_data_ptr);
        
        /* Update LPC when RefLoc is received */
        tm_lpc_refloc_received(phone_state_info.e911_state);
        
        /* Force TM to sleep for 2ms so that LM can finish processing ref Loc.
        Without this delay LM is unable to consume IPCs as fast as TM sends them
        resulting in IPC mem exhaustion. Tests have shown that LM can sometimes take
        upto 2ms to finish processing ref Loc.
        */
        (void)os_SleepMs_NonDeferrable(2);
        break;

      case TM_POST_DATA_TYPE_REFTIME:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_REF_TIME;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_RefTimeInfo = data_payload_ptr->reftime_data;
        retVal = lm_info(lm_data_ptr);
        /* Force TM to sleep for 2ms so that LM can finish processing ref Time.
        Without this delay LM is unable to consume IPCs as fast as TM sends them
        resulting in IPC mem exhaustion. Tests have shown that LM can sometimes take
        upto 2ms to finish processing refTime.
        */
        (void)os_SleepMs_NonDeferrable(2);
        break;

      case TM_POST_DATA_TYPE_UTC_MODEL:
#ifdef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
        #error code not present
#else
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_UTC;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_UtcInfo = data_payload_ptr->utc_model_data;
        retVal = lm_info(lm_data_ptr);
#endif /* FEATURE_CGPS_FW_NON_STANDARD_SUPPORT */
        break;

      case TM_POST_DATA_TYPE_PREFIX_PPM:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_PRE_FIX_PPM;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_PreFixPpmInfo = data_payload_ptr->ppm_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_SV_UNHEALTHY:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_SV_UNHEALTHY;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_SvUnhealthyInfo = data_payload_ptr->sv_unhealthy_data;
        retVal = lm_info(lm_data_ptr);
        break;

        /**** Glonass assistance data *****/
      case TM_POST_DATA_TYPE_REFTIME_GLO:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_REF_TIME;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GLO;
        p_AssistData->z_GloRefTimeInfo = data_payload_ptr->reftime_glo_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_EPH_GLO:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_EPH;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GLO;
        p_AssistData->z_GloEphInfo = data_payload_ptr->eph_glo_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_ALM_GLO:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_ALM;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GLO;
        p_AssistData->z_GloAlmInfo = data_payload_ptr->alm_glo_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_UTC_MODEL_GLO:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_UTC;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GLO;
        p_AssistData->z_GloUtcInfo = data_payload_ptr->utc_model_glo_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_TIME_MODEL_GLO:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_GNSS_TIME_MODEL;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GLO;
        p_AssistData->z_TimeModelInfo = data_payload_ptr->gnss_time_model_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_AA_GLO:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_ACQ_ASSIST;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GLO;
        p_AssistData->z_SvAcqAssistInfo = data_payload_ptr->gnss_aa_data;
        /*If in concurrent mode maintain a mask for list of SVs assistance was provided.*/
        if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Update aiding info for E911 for GLO");
          tm_lpc_update_for_aiding_data(LDM_CLIENT_ID_E911,
                                        MGP_GNSS_TYPE_GLO,
                                        &(p_AssistData->z_SvAcqAssistInfo));
        }
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_SV_DIR_GLO:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_SV_DIR;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GLO;
        p_AssistData->z_GnssSvDirInfo = data_payload_ptr->gnss_sv_dir;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_GNSS_SV_HEALTH_INFO:
        MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,
              "Sending GNSS (%d) Unhealhty SV info to LM 0x%x",
              data_payload_ptr->gnss_sv_health_info.e_GnssType,
              data_payload_ptr->gnss_sv_health_info.q_BadSvMask);
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_ADV_HEALTH;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = data_payload_ptr->gnss_sv_health_info.e_GnssType;
        if (data_payload_ptr->gnss_sv_health_info.e_GnssType == MGP_GNSS_TYPE_GLO)
        {
          p_AssistData->z_GloHealthInfo = data_payload_ptr->gnss_sv_health_info;
          retVal = lm_info(lm_data_ptr);
        }
        else if (data_payload_ptr->gnss_sv_health_info.e_GnssType == MGP_GNSS_TYPE_BDS)
        {
          p_AssistData->z_BdsAdvHealthInfo = data_payload_ptr->gnss_sv_health_info;
          retVal = lm_info(lm_data_ptr);
        }
        break;

      case TM_POST_DATA_TYPE_REFPOS_RPT:
        tm_nmea_get_gps_fix_from_refpos(&(data_payload_ptr->seed_data),
                                        &z_fix);

        /* Generate extended pd data */
        tm_core_generate_ext_pd_data(&z_fix);

        /*Send the LR to Geofence module*/
        fix_info.fix_type = GEOFENCE_POS_FIX_TYPE_FINAL;
        fix_info.y_pos_fix_ext_info.z_gnss_fix_ext_info.b_is_nw_asst_fix = TRUE;
        fix_info.fix_src = GEOFENCE_POS_FIX_SRC_GNSS;
        fix_info.pos_fix = z_fix;

        (void)gm_position_update(&fix_info);

        break;

      case TM_POST_DATA_TYPE_OTDOA_AD:
#ifdef FEATURE_LOC_LTE_OTDOA
        // send to SM_OTDOA module.
        tm_core_info.session_info.op_requested |= TM_OP_REQ_OTDOA;
        retVal = sm_loc_lte_otdoa_start_cmd(&data_payload_ptr->otdoa_assist_data.otdoa_assist_data,
                                            data_payload_ptr->otdoa_assist_data.q_operation_mode,
                                            data_payload_ptr->otdoa_assist_data.q_sess_type,
                                            data_payload_ptr->otdoa_assist_data.e_as_id);
        b_otdoa_sess_start_sent = TRUE;
#else
        retVal = TRUE; /* no-op for otdoa start req. when feature is off. Its not an error.*/
#endif /* FEATURE_LOC_LTE_OTDOA */
        break;

      case TM_POST_DATA_TYPE_OTDOA_STOP:
#ifdef FEATURE_LOC_LTE_OTDOA
        /* Send OTDOA stop command if the engine is running */
        if (TRUE == b_otdoa_sess_start_sent)
        {
          retVal = sm_loc_lte_otdoa_stop_cmd(data_payload_ptr->otdoa_stop.e_stop_reason,
                                             prtl_type,
                                             data_payload_ptr->otdoa_stop.e_as_id);
          b_otdoa_sess_start_sent = FALSE;
        }
        else
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "OTDOA engine already stopped or not started ");
          retVal = TRUE; /* If OTDOA engine already stopped or not started isn't an error case. */
        }
#else
        retVal = TRUE; /* no-op for otdoa start req. when feature is off. Its not an error.*/
#endif /* FEATURE_LOC_LTE_OTDOA */
        break;

      case TM_POST_DATA_TYPE_FTA_DATA:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_FTA_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_GPS;
        p_AssistData->z_FTAInfo = data_payload_ptr->fta_assist_data;
        retVal = lm_info(lm_data_ptr);
        break;

#ifdef FEATURE_ABDS_RRLP_SUPL
        /* Handle BDS AD! */
      case TM_POST_DATA_TYPE_REFTIME_BDS:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_REF_TIME;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_BDS;

        p_AssistData->z_BdsRefTime.e_SourceOfTime = C_TIME_SRC_EXT_INJECT; /*!< Source of the time */
        /*!< BDS week number at reference tick [weeks] */
        p_AssistData->z_BdsRefTime.w_BdsWeek   =
           data_payload_ptr->gnss_reftime_data.w_gnss_day / DAYS_IN_ONE_WEEK;
        /*!< BDS msec at reference tick [msecs] */
        p_AssistData->z_BdsRefTime.q_BdsMsec   =
           ((data_payload_ptr->gnss_reftime_data.w_gnss_day % DAYS_IN_ONE_WEEK) * GPS_SECONDS_IN_DAY
            + data_payload_ptr->gnss_reftime_data.q_gnss_tod_sec) * SECS_TO_MSECS;
        p_AssistData->z_BdsRefTime.f_ClkTimeBias  = 0;  /*!< Clock bias [msecs]. Constrait to >-0.5 and <0.5 */
        p_AssistData->z_BdsRefTime.f_ClkTimeUncMs =
           data_payload_ptr->gnss_reftime_data.q_gnss_tod_unc_ms;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_EPH_BDS:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_EPH;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_BDS;
        p_AssistData->z_BdsEphInfo = data_payload_ptr->eph_bds_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_ALM_BDS:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_ALM;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_BDS;
        p_AssistData->z_BdsAlmInfo = data_payload_ptr->alm_bds_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_TIME_MODEL_BDS:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_GNSS_TIME_MODEL;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_BDS;
        p_AssistData->z_TimeModelInfo = data_payload_ptr->gnss_time_model_data;
        retVal = lm_info(lm_data_ptr);
        break;

      case TM_POST_DATA_TYPE_BDS_GLOBAL_IONO:
        lm_data_ptr->e_info_type = LM_SESSION_INFO_AIDING_DATA;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_AssistanceType = MGP_GNSS_ASSIST_DATA_IONO;
        lm_data_ptr->z_info.z_aiding_data.z_assist_data.e_GnssType = MGP_GNSS_TYPE_BDS;
        p_AssistData->z_BdsGlobalIonoInfo = data_payload_ptr->global_iono_bds_data;
        retVal = lm_info(lm_data_ptr);
        break;
#endif

      default:
        break;
    }

    (void)pdsm_freebuf((char *)lm_data_ptr);

    if (retVal == TRUE) /*lint !e506 !e731 */
    {
      /* Notify PDAPI if needed */

      /* Fire Diag Events */
    }
    else
    {
      /* LM does not allow the request, abort and recover */
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      retVal = FALSE;
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Data rejected. Sess %lu, Active %lu, State %d", sess_handle,
          tm_core_info.session_info.active_sess_handle, tm_core_info.session_info.session_state);
    retVal = FALSE;
  }
  return (retVal);
}

/*===========================================================================

FUNCTION tm_core_recv_event

DESCRIPTION
  This function is to handle PD API Events sent from protocol sub-modules.

DEPENDENCIES

RETURN VALUE
  TRUE: If TM-Core sub-module accepts it
  FALSE: Otherwise

SIDE EFFECTS

===========================================================================*/
boolean tm_core_recv_event
(
   tm_prtl_type               prtl_type,
   tm_sess_handle_type        sess_handle,
   pdsm_pd_event_type         pd_event,
   pdsm_pd_info_s_type        *pd_info_ptr,
   gnss_NavSlnFlagsStructType *p_PosSrcFlags
   )
{
  tm_pdapi_pd_event_s_type p_pd_info_type;
  boolean req_pos = FALSE;
  pdsm_ext_status_info_s_type   pd_ext_status_info;
  utc_time z_utc_time;
  (void)prtl_type;

  /* Init */
  memset(&p_pd_info_type, 0, sizeof(p_pd_info_type));

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM-Core Recv'd PD events (%d) from protocol", pd_event, 0, 0);


  if (pd_info_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null pd_info_ptr", 0, 0, 0);
    return FALSE;
  }

  if ((pd_event == PDSM_PD_EVENT_COMM_FAILURE) &&
      ((tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_1X_UP) ||
       (tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_UMTS_UP)) &&
      (tm_core_info.session_info.op_mode == TM_SESS_OPERATION_MODE_MSB) &&
      (tm_core_info.session_info.msb_xtra_allowed == FALSE))
  {
#ifdef FEATURE_CGPS_XTRA
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "XTRA - allow XTRA after DLOAD COMM failure", 0, 0, 0);
    tm_core_info.session_info.msb_xtra_allowed = TRUE;

    if (!tm_xtra_data_validity_check(GPS_TIME_CHECK_SOURCE_PER_FIX))
    {
#ifdef FEATURE_CGPS_XTRA_T
      if (!tm_xtra_invoke_xtrat(PDSM_XTRA_CMD_ERR_NONE))
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to invoke XTRA-T for MSB fallback", 0, 0, 0);
      }
      else
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Invoke XTRA-T for MSB fallback", 0, 0, 0);
      }
#endif /* FEATURE_CGPS_XTRA_T */
    }
    else
    {
      // Successfully sent time request to MGP, clear the flag
      tm_core_info.session_info.req_xtra_trig_by_agps = FALSE;
    }

#endif /* FEATURE_CGPS_XTRA */
  }



  if ((pd_event & PDSM_PD_EVENT_POSITION) ||
      (pd_event & PDSM_PD_EVENT_VELOCITY) ||
      (pd_event & PDSM_PD_EVENT_HEIGHT))
  {
    /* For MSA Sessions, see if we have UTCOffset available and can compute UTCTime */
    if (tm_core_info.session_info.op_mode == TM_SESS_OPERATION_MODE_MSA)
    {
      if (tm_core_get_utc_offset((uint8 *)&pd_info_ptr->pd_info.pd_data.gpsUtcOffset))
      {
        pd_info_ptr->pd_info.pd_data.opt_field_mask |= PDSM_PD_UTCOFFSET_VALID;
      }
      else
      {
        pd_info_ptr->pd_info.pd_data.opt_field_mask &= ~PDSM_PD_UTCOFFSET_VALID;
      }

      /* Compute UTC Time, regardless of if the offset is valid or not */
      pd_info_ptr->pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_UTC_TIME_VALID;
      z_utc_time = tm_util_construct_utc_time(pd_info_ptr->pd_info.pd_data.ext_pos_data.timestamp_gps_week,
                                              pd_info_ptr->pd_info.pd_data.ext_pos_data.timestamp_tow_ms,
                                              pd_info_ptr->pd_info.pd_data.ext_pos_data.timestamp_tow_ms -
                                              pd_info_ptr->pd_info.pd_data.gpsUtcOffset * SEC_MSECS);

      pd_info_ptr->pd_info.pd_data.ext_pos_data.utc_time.hour = (uint8)z_utc_time.q_hours;
      pd_info_ptr->pd_info.pd_data.ext_pos_data.utc_time.minute = (uint8)z_utc_time.q_mins;
      pd_info_ptr->pd_info.pd_data.ext_pos_data.utc_time.sec = (uint16)(z_utc_time.q_secs * 100);

      pd_info_ptr->pd_info.pd_data.ext_pos_data.utc_time.date =
         (uint32)((z_utc_time.q_utc_days * 1000000) + (z_utc_time.q_month * 10000) + z_utc_time.q_year);
    }

    /* Copy ext data over */
    memscpy((void *)&(pd_info_ptr->pd_info.pd_ext_data),
            sizeof(pd_info_ptr->pd_info.pd_ext_data),
            (void *)&(tm_core_info.pd_ext_data),
            sizeof(tm_core_info.pd_ext_data));

    if (tm_core_info.ext_opt_field_mask & PDSM_PD_HEADING_VALID)
      pd_info_ptr->pd_info.pd_data.ext_opt_field_mask |= PDSM_PD_HEADING_VALID;

    if (p_PosSrcFlags == NULL)
    {
      tm_pdapi_pos_log_generation(pd_event,
                                  pd_info_ptr,
                                  0,
                                  0,
                                  TM_PDAPI_SESS_TYPE_NONE);
    }
    else
    {
      tm_pdapi_pos_log_generation(pd_event,
                                  pd_info_ptr,
                                  p_PosSrcFlags->b_IsSft,
                                  p_PosSrcFlags->b_IsKf,
                                  TM_PDAPI_SESS_TYPE_NONE);
    }

#ifdef FEATURE_TM_SECURITY

#ifdef FEATURE_GPSONE_SECURITY_OPEN_STANDALONE
    if (tm_core_info.session_info.pd_param.pd_option.operation_mode != PDSM_SESSION_OPERATION_STANDALONE_ONLY)
    {
#endif /* FEATURE_GPSONE_SECURITY_OPEN_STANDALONE */
      /* Reset pd_ext_data */
      memset((void *)&(tm_core_info.pd_ext_data), 0, sizeof(pdsm_pd_data_ext_s_type));
      tm_core_info.ext_opt_field_mask = 0;

      /* Encrypt PD Data if security layer is enabled */
      tm_sec_encrypt_pd_info(pd_info_ptr);
#ifdef FEATURE_GPSONE_SECURITY_OPEN_STANDALONE
    }
    else
    {
      pd_info_ptr->pd_info.encrypt_data.encrypt_alg = PDSM_PD_CRYPT_ALG_NONE;
    }
#endif /* FEATURE_GPSONE_SECURITY_OPEN_STANDALONE */

#else

    pd_info_ptr->pd_info.encrypt_data.encrypt_alg = PDSM_PD_CRYPT_ALG_NONE;

#endif /* FEATURE_TM_SECURITY */

    z_cached_pos_info.cached_pos_valid = TRUE;
    memscpy(&z_cached_pos_info.pd_info, sizeof(z_cached_pos_info.pd_info), pd_info_ptr, sizeof(*pd_info_ptr));
    z_cached_pos_info.pd_event_type = pd_event;
    if (p_PosSrcFlags == NULL)
    {
      memset(&z_cached_pos_info.z_pos_flags, 0, sizeof(z_cached_pos_info.z_pos_flags));
    }
    else
    {
      z_cached_pos_info.z_pos_flags = *p_PosSrcFlags;
    }
    pd_info_ptr->pd_info.pd_data.ext_pos_data.spoof_report = sdm_GetSpoofReport();
    if (0 != pd_info_ptr->pd_info.pd_data.ext_pos_data.spoof_report.spoof_type_mask)
    {
      TM_HIGH_2("FinalFix SpoofReport T:0x%x Tick:%lu",
                pd_info_ptr->pd_info.pd_data.ext_pos_data.spoof_report.spoof_type_mask,
                pd_info_ptr->pd_info.pd_data.ext_pos_data.spoof_report.last_spoof_event_timetick);
    }
    tm_pdapi_pos_event_callback(pd_event, pd_info_ptr, TM_PDAPI_SESS_TYPE_NONE);

  }
  else
  {
    p_pd_info_type.e_pd_event = pd_event;
    p_pd_info_type.pd_event_data.pd_info_type = *pd_info_ptr;
    tm_pdapi_pd_event_callback(&p_pd_info_type, TM_PDAPI_SESS_TYPE_NONE);

#ifdef FEATURE_CGPS_XTRA_T
    /* Notify TLE's XTM task that AGPS session is in progress. */
    if (pd_event == PDSM_PD_EVENT_COMM_CONNECTED)
    {
      (void)tle_SendAGPSEvents(C_TLE_EVENT_AGPS_PDCOMM_CONNECTED);
    }
#endif

  }
  return TRUE;
}

/*===========================================================================

FUNCTION tm_core_translate_lm_error

DESCRIPTION
  This function is to translate LM error to tm_core - prtl error cause

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
tm_sess_stop_reason_e_type tm_core_translate_lm_error
(
   tm_session_error_cause_e_type lm_err
   )
{
  switch (lm_err)
  {
    case TM_ERROR_CAUSE_PPM_TIMEOUT:
      return TM_STOP_REASON_PPM_TIMEOUT;

    case TM_ERROR_CAUSE_PRM_TIMEOUT:
      return TM_STOP_REASON_PRM_TIMEOUT;

    case TM_ERROR_CAUSE_FIX_TIMEOUT:
      return TM_STOP_REASON_FIX_TIMEOUT;

    default:
      return TM_STOP_REASON_GENERAL_ERROR;
  }
}

/*===========================================================================

FUNCTION tm_core_lm_sess_req_handler

DESCRIPTION
  This function is to handle sess request from LM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_lm_sess_req_handler(void *lm_sess_req_data)
{
  tm_session_request_s_type *sess_req_ptr = (tm_session_request_s_type *)lm_sess_req_data;
  int prtlTableIndex = tm_core_get_prtl_idx_by_type(tm_core_info.session_info.from_protocol);
  tm_sess_stop_reason_e_type stop_reason;

  if (sess_req_ptr)
  {
    switch (sess_req_ptr->e_request_type)
    {
      case TM_REQUEST_ERROR_NOTIFY:
        if (sess_req_ptr->q_handle == tm_core_info.session_info.active_sess_handle)
        {
          /* Send abort request to Protocol sub-module, and let protocl sub-module decide what to do */

          if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
          {
            if (tm_core_is_internal_session_on() == TRUE)
            {
              /* An internal session is going ON . Could be started from GM . Send the fix fail
               indication to it */
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sending position fix fail update to GM . Reason %d",
                    sess_req_ptr->z_request.e_errorCause, 0, 0);
              gm_position_fix_fail_update(GEOFENCE_POS_FIX_SRC_GNSS, GEOFENCE_POS_FIX_FAIL_REASON_TIMEOUT);

            }
            /* Set reason to general error for now, if return value */
            if (tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Got error notification from LM, foward to protocol modules", 0, 0, 0);
              stop_reason = tm_core_translate_lm_error(sess_req_ptr->z_request.e_errorCause);
              tm_core_abort_recover(TRUE, stop_reason); /*lint !e506 !e730 */
            }
            else
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No protocol callback installed 4", 0, 0, 0);
              tm_core_abort_recover(FALSE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
            }
          }
          else
          {
            if (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911)
            {
              /* E911 is a dummy protocol, no protocol call backs are needed. Just clean up the session 
                 if no actual LPP session was rcvd from the network after E911 */
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "E911 ended without network request. Cleanup session", 0, 0, 0);
              /* Cleanup E911 from LDM as well */
              if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(LDM_CLIENT_ID_E911))
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 Client with handle successfully disabled at LDM");
              }
              else
              {
                MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:E911 Client with handle failed to be disabled at LDM");
              }
              tm_core_abort_recover(TRUE, TM_STOP_REASON_COMPLETED); /*lint !e506 !e730 */
            }

            else
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid prtl index", 0, 0, 0);
            }
          }
        } /* sess_req_ptr->q_handle == tm_core_info.session_info.active_sess_handle */
        else if (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE)
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LM - TM state not in sync, abort everything", 0, 0, 0);
          tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
        }
        break;

      case TM_REQUEST_SA_DATA:
        if (sess_req_ptr->q_handle == tm_core_info.session_info.active_sess_handle)
        {
          /* Send abort request to Protocol sub-module, and let protocl sub-module decide what to do */

          if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
          {
            /* Set reason to general error for now, if return value */
            if (tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_req_data_fp)
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Got SA request from LM", 0, 0, 0);
              (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_req_data_fp(
                 tm_core_info.session_info.active_sess_handle,
                 PRTL_AIDING_DATA_REQ_SA
                 );
            }
            else
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No protocol callback installed 5", 0, 0, 0);
              tm_core_abort_recover(FALSE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
            }
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid prtl index", 0, 0, 0);
          }
        } /* sess_req_ptr->q_handle == tm_core_info.session_info.active_sess_handle */
        else if (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE)
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LM - TM state not in sync, abort everything", 0, 0, 0);
          tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
        }

        break;

      default:
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unknown LM Sess Requst", 0, 0, 0);
        break;
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null sess_req_ptr", 0, 0, 0);
    tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
  }
}

/*===========================================================================

FUNCTION tm_core_xlate_sv_state

DESCRIPTION 
Translates the SV state from the measurement report to PDAPI state 

DEPENDENCIES 
None 

RETURN VALUE 
PDSM state 

SIDE EFFECTS

===========================================================================*/
pdsm_LocSvSearchStatusEnumT tm_core_xlate_sv_state(gnss_SvStateEnumType in_sv_state)
{
  switch (in_sv_state)
  {
    case C_GNSS_SV_STATE_IDLE:
      return PDSM_SV_STATE_IDLE;

    case C_GNSS_SV_STATE_SEARCH:
      return PDSM_SV_STATE_SEARCH;

    case C_GNSS_SV_STATE_SEARCH_VERIFY:
      return PDSM_SV_STATE_SEARCH_VERIFY;

    case C_GNSS_SV_STATE_GLO_10MSEC_BIT_EDGE:
      return PDSM_SV_STATE_GLO_10MSEC_BIT_EDGE;

    case C_GNSS_SV_STATE_BIT_EDGE:
      return PDSM_SV_STATE_BIT_EDGE;

    case C_GNSS_SV_STATE_VERIFY_TRACK:
      return PDSM_SV_STATE_VERIFY_TRACK;

    case C_GNSS_SV_STATE_TRACK:
      return PDSM_SV_STATE_TRACK;

    case C_GNSS_SV_STATE_DPO_TRACK:
      return PDSM_SV_STATE_DPO_TRACK;

    case C_GNSS_SV_STATE_LPPM_SEARCH:
      return PDSM_SV_STATE_LPPM_SEARCH;

    default:
      return PDSM_SV_STATE_IDLE;
  }
}


/*===========================================================================

FUNCTION tm_core_xlate_pos_to_ext_status

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_xlate_pos_to_ext_status
(
   pdsm_pd_ext_pos_s_type  *dest_buf_ptr,
   sm_FixInfoStructType    *source_buf_ptr,
   uint32                  remaing_fix_time
   )
{
  tm_lr_resp_type    *lr_resp_ptr = NULL;
  qword              qw_time;
  uint32             orig_time;
  uint32             gps_utc_offset;
  pdsm_xtra_download_info_s_type p_download_info;
  boolean xtra_valid = TRUE;
  float              f_tmp;
  float              f_MagDev = 0;

  if ((dest_buf_ptr == NULL) || (source_buf_ptr == NULL))
  {
    TM_ERROR_2("tm_core_xlate_pos_to_ext_status() NULL PTR 0x%p 0x%p", dest_buf_ptr, source_buf_ptr);
    return;
  }

  lr_resp_ptr = (tm_lr_resp_type *)pdsm_getbuf(sizeof(tm_lr_resp_type)); /*lint !e826 */

  if (lr_resp_ptr != NULL)
  {

    memset(lr_resp_ptr, 0, sizeof(tm_lr_resp_type));
    dest_buf_ptr->fixTimeRemaining = (uint32)remaing_fix_time / 1000; // time in seconds

    /* sanity check: If pos is not valid, SvInfo should be invalid */
    if (!source_buf_ptr->z_NavPos.z_PosFlags.b_IsValid)
    {
      dest_buf_ptr->pd_data.z_NavSvInfo.u_SvInfoValid = FALSE;
    }

    if (tm_util_convert_lr(source_buf_ptr, lr_resp_ptr))
    {
      qw_time[0] = lr_resp_ptr->timestamp[0];
      qw_time[1] = lr_resp_ptr->timestamp[1];

      (void)qw_div_by_power_of_2(qw_time, qw_time, 16); /* obtain time in 1.25 ms */

      /* obtain time in second's */
      (void)qw_div(qw_time, qw_time, 800);

      /* Save the current time, so we can check for underflow/overflow */
      orig_time = qw_time[0];

      /* Now clear the lower 14 bits of time stamp */
      qw_time[0] &= 0xffffc000;

      /* Now copy the time reference provided from PDE.
      */
      qw_time[0] |= (lr_resp_ptr->time_ref_cdma & 0x3fff);

      /* Check for Overflow/Underflow
      */
      if ((int32)(qw_time[0] - orig_time) > 0x2000)
      {
        qw_time[0] -= 0x4000;
      }
      else if ((int32)(orig_time - qw_time[0]) > 0x2000)
      {
        qw_time[0] += 0x4000;
      }

      dest_buf_ptr->pd_data.meas_data              = NULL;
      dest_buf_ptr->pd_data.pm_data                = NULL;
      dest_buf_ptr->pd_data.lat                    = lr_resp_ptr->latitude;
      dest_buf_ptr->pd_data.lon                    = lr_resp_ptr->longitude;
      dest_buf_ptr->pd_data.loc_uncertainty_a      = lr_resp_ptr->loc_uncrtnty_a;
      dest_buf_ptr->pd_data.loc_uncertainty_ang    = lr_resp_ptr->loc_uncrtnty_ang;
      dest_buf_ptr->pd_data.loc_uncertainty_p      = lr_resp_ptr->loc_uncrtnty_p;

      dest_buf_ptr->pd_data.time_stamp = qw_time[0];
      dest_buf_ptr->pd_data.opt_field_mask = PDSM_PD_UNCERTAINTY_VALID;

      dest_buf_ptr->pd_data.ext_opt_field_mask = PDSM_PD_LAT_LONG_VALID;
      dest_buf_ptr->pd_data.ext_opt_field_mask |= PDSM_PD_GPS_TIME_VALID;
      dest_buf_ptr->pd_data.ext_opt_field_mask |= PDSM_PD_UTC_TIME_VALID;

      /* Copy msec field received from PE
      */
      if (lr_resp_ptr->msec_incl)
      {
        dest_buf_ptr->pd_data.opt_field_mask |= PDSM_PD_MSEC_VALID;
        dest_buf_ptr->pd_data.time_stamp_msec = lr_resp_ptr->timestamp_msec;
      }

      /* Check if Velocity info is available.
      */
      // always populate velocity fields even if it's invalid
      dest_buf_ptr->pd_data.velocity_hor       = lr_resp_ptr->velocity_hor;
      dest_buf_ptr->pd_data.heading            = lr_resp_ptr->heading;

      /* multiply by 100.0 to convert speed from m/s to cm/s */

      f_tmp = 100.0 * sqrt(source_buf_ptr->z_NavPos.f_VelEnu[0] *
                           source_buf_ptr->z_NavPos.f_VelEnu[0] +
                           source_buf_ptr->z_NavPos.f_VelEnu[1] *
                           source_buf_ptr->z_NavPos.f_VelEnu[1]);

      dest_buf_ptr->pd_data.ext_pos_data.f_velHorU  = (uint16)f_tmp;
      dest_buf_ptr->pd_data.ext_pos_data.f_velHor  = (int16)f_tmp;

      if (f_tmp > INT16_MAX)
      {
        dest_buf_ptr->pd_data.ext_pos_data.f_velHor = INT16_MAX;
      }
      else if (f_tmp < (float)(TM_CORE_MIN_VELOCTY * 100))
      {
        dest_buf_ptr->pd_data.ext_pos_data.f_velHor = 0;
        dest_buf_ptr->pd_data.ext_pos_data.f_velHorU = 0;
      }

      /* Calculate horizontal velocity uncertainty(cm/s) */
      f_tmp = tm_util_calc_vel_horiz_uncertainty(&(source_buf_ptr->z_NavPos)) * 100.0;
      if (f_tmp > (float)CGPS_MAX_UINT16)
      {
        dest_buf_ptr->pd_data.ext_pos_data.velUncHoriz = CGPS_MAX_UINT16;
      }
      else
      {
        dest_buf_ptr->pd_data.ext_pos_data.velUncHoriz = (uint16)f_tmp;
      }

      dest_buf_ptr->pd_data.velocity_ver = lr_resp_ptr->velocity_ver;

      if ((source_buf_ptr->z_NavPos.f_VelEnu[2] * 100) > CGPS_MAX_INT16)
      {
        dest_buf_ptr->pd_data.ext_pos_data.f_velVert = CGPS_MAX_INT16;
      }
      else
      {
        dest_buf_ptr->pd_data.ext_pos_data.f_velVert = source_buf_ptr->z_NavPos.f_VelEnu[2] * 100.0;
      }


      /* Calculate vertical velocity uncertainty(cm/s) */
      f_tmp = tm_util_calc_vel_vert_uncertainty(&(source_buf_ptr->z_NavPos)) * 100.0;
      if (f_tmp > (float)CGPS_MAX_UINT16)
      {
        dest_buf_ptr->pd_data.ext_pos_data.velUncVert = CGPS_MAX_UINT16;
      }
      else
      {
        dest_buf_ptr->pd_data.ext_pos_data.velUncVert = (uint16)f_tmp;
      }

      /* Applicable only in case of Unpropagated fix */
      if (source_buf_ptr->z_NavPos.u_MagDevGood == TRUE)
      {
        dest_buf_ptr->pd_data.ext_pos_data.magnetic_deviation = (int64)(source_buf_ptr->z_NavPos.f_MagDeviation * 10.0 + 0.5);  /* [x10 deg] */
      }
      else
      {
        /* If Mag Dev is not available, attempt to compute it */
        if (cgps_MagDeviation(source_buf_ptr->z_NavPos.d_PosLla[0], source_buf_ptr->z_NavPos.d_PosLla[1], &f_MagDev) == TRUE)
        {
          dest_buf_ptr->pd_data.ext_pos_data.magnetic_deviation = (int64)(f_MagDev * 10.0 + 0.5);
        }
        else
        {
          dest_buf_ptr->pd_data.ext_pos_data.magnetic_deviation = 0;
        }
      }

      MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "Position: MagDev: %d, MagDevGood: %d",
            dest_buf_ptr->pd_data.ext_pos_data.magnetic_deviation,
            source_buf_ptr->z_NavPos.u_MagDevGood, 0);

      /* Applicable only in case of Unpropagated fix */
      dest_buf_ptr->pd_data.ext_pos_data.f_VelocityENUMps[0] = source_buf_ptr->z_NavPos.f_VelEnu[0];
      dest_buf_ptr->pd_data.ext_pos_data.f_VelocityENUMps[1] = source_buf_ptr->z_NavPos.f_VelEnu[1];
      dest_buf_ptr->pd_data.ext_pos_data.f_VelocityENUMps[2] = source_buf_ptr->z_NavPos.f_VelEnu[2];

      MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "Velocity: f_VelocityENUMps[0]: %d, f_VelocityENUMps[1]: %d, f_VelocityENUMps[2]: %d",
            dest_buf_ptr->pd_data.ext_pos_data.f_VelocityENUMps[0],
            dest_buf_ptr->pd_data.ext_pos_data.f_VelocityENUMps[1],
            dest_buf_ptr->pd_data.ext_pos_data.f_VelocityENUMps[2]);

      /* Applicable only in case of Unpropagated fix */
      dest_buf_ptr->pd_data.ext_pos_data.f_VelocityUncENUMps[0] = source_buf_ptr->z_NavPos.f_VuncEastNorthMps[0];
      dest_buf_ptr->pd_data.ext_pos_data.f_VelocityUncENUMps[1] = source_buf_ptr->z_NavPos.f_VuncEastNorthMps[1];
      dest_buf_ptr->pd_data.ext_pos_data.f_VelocityUncENUMps[2] = source_buf_ptr->z_NavPos.f_VuncVertMps;

      MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED, "Velocity: f_VelocityUncENUMps[0]: %d, f_VelocityUncENUMps[1]: %d, f_VelocityUncENUMps[2]: %d",
            dest_buf_ptr->pd_data.ext_pos_data.f_VelocityUncENUMps[0],
            dest_buf_ptr->pd_data.ext_pos_data.f_VelocityUncENUMps[1],
            dest_buf_ptr->pd_data.ext_pos_data.f_VelocityUncENUMps[2]);

      if (lr_resp_ptr->velocity_incl)
      {
        dest_buf_ptr->pd_data.opt_field_mask     |= PDSM_PD_VELOCITY_VALID;
        dest_buf_ptr->pd_data.ext_opt_field_mask    |= PDSM_PD_EXT_VELOCITY_VALID;

        if (lr_resp_ptr->heading_valid)
        {
          dest_buf_ptr->pd_data.ext_opt_field_mask |= PDSM_PD_HEADING_VALID;
        }

        /* Overloaded fix type for VX spec's.
        */
        if (lr_resp_ptr->fix_type)
        {
          dest_buf_ptr->pd_data.fix_type     = TRUE;
        }
        else
        {
          dest_buf_ptr->pd_data.fix_type = FALSE;
        }
      } /* If velocity included is TRUE */

      /* Check if Height info is available.
      */
      if (lr_resp_ptr->height_incl)
      {
        dest_buf_ptr->pd_data.opt_field_mask     |= PDSM_PD_ALTITUDE_VALID;
        dest_buf_ptr->pd_data.altitude           = lr_resp_ptr->height;
        dest_buf_ptr->pd_data.ext_pos_data.f_altitude = lr_resp_ptr->f_altitude;
        dest_buf_ptr->pd_data.loc_uncertainty_v  = lr_resp_ptr->loc_uncrtnty_v;
      }
      else
      {
        dest_buf_ptr->pd_data.ext_pos_data.f_altitude           = 0;
        dest_buf_ptr->pd_data.altitude           = 0.0;
        dest_buf_ptr->pd_data.loc_uncertainty_v  = 0;
      }

      /* Initialize pos info */
      dest_buf_ptr->pd_data.position_type      = PDSM_PD_POSITION_TYPE_NEW;
      dest_buf_ptr->pd_data.positioning_source = 0;

      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_PosFlags.b_IsGpsUsed) ?
         PDSM_PD_POSITION_SOURCE_GPS : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_PosFlags.b_IsGloUsed) ?
         PDSM_PD_POSITION_SOURCE_GLO : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_PosFlags.b_IsBdsUsed) ?
         PDSM_PD_POSITION_SOURCE_BDS     : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_PosFlags.b_IsQzssUsed) ?
         PDSM_PD_POSITION_SOURCE_QZSS     : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_PosFlags.b_IsGalUsed) ?
         PDSM_PD_POSITION_SOURCE_GAL     : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_PosFlags.v_NavicUsed) ?
         PDSM_PD_POSITION_SOURCE_NAVIC   : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_SmPosFlags.b_IsWifi) ?
         PDSM_PD_POSITION_SOURCE_WIFI    : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_SmPosFlags.b_IsCellId) ?
         PDSM_PD_POSITION_SOURCE_CELLID  : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_SmPosFlags.b_IsSensors) ?
         PDSM_PD_POSITION_SOURCE_SENSORS : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_SmPosFlags.b_IsRefLoc) ?
         PDSM_PD_POSITION_SOURCE_DEFAULT : 0;
      dest_buf_ptr->pd_data.positioning_source |= (source_buf_ptr->z_NavPos.z_SmPosFlags.b_IsCoarsePos) ?
         PDSM_PD_POSITION_SOURCE_EPI : 0;

      if (tm_core_info.session_info.op_mode == TM_SESS_OPERATION_MODE_MSA ||
          tm_core_info.session_info.op_mode == TM_SESS_OPERATION_MODE_OTDOA)
      {
        dest_buf_ptr->pd_data.position_mode = PDSM_PD_POSITION_MODE_MSASSISTED;
      }
      else
      {
        dest_buf_ptr->pd_data.position_mode = PDSM_PD_POSITION_MODE_STANDALONE;
      }

      dest_buf_ptr->pd_data.loc_uncertainty_conf = LOC_UNC_CONFIDENCE;

      dest_buf_ptr->pd_data.opt_field_mask       |= PDSM_PD_UTCOFFSET_VALID;

      /* Set gpsUtcOffset in milliseconds */
      if (source_buf_ptr->z_NavPos.q_GpsTimeMs >= source_buf_ptr->z_NavPos.q_UtcTimeMs)
      {
        gps_utc_offset = source_buf_ptr->z_NavPos.q_GpsTimeMs - source_buf_ptr->z_NavPos.q_UtcTimeMs;
      }
      else
      {
        gps_utc_offset = source_buf_ptr->z_NavPos.q_GpsTimeMs + MS_PER_WEEK - source_buf_ptr->z_NavPos.q_UtcTimeMs;
      }

      dest_buf_ptr->pd_data.gpsUtcOffset         = (int8)(gps_utc_offset / 1000);

    }

    dest_buf_ptr->pd_data.opt_field_mask |= PDSM_PD_EXT_STATUS_VALID;
    dest_buf_ptr->pd_data.ext_pos_data.lat = (int32)(C_2_32_OVER_PI * source_buf_ptr->z_NavPos.d_PosLla[0]);
    dest_buf_ptr->pd_data.ext_pos_data.lon = (int32)(C_2_31_OVER_PI * source_buf_ptr->z_NavPos.d_PosLla[1]);
    if (source_buf_ptr->z_NavPos.f_ErrorEllipse[1] > CGPS_WHOLE_EARTH_UNC)
    {
      source_buf_ptr->z_NavPos.f_ErrorEllipse[1] = CGPS_WHOLE_EARTH_UNC;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Error Ellipse along greater than earth radius", 0, 0, 0);
    }
    if (source_buf_ptr->z_NavPos.f_ErrorEllipse[2] > CGPS_WHOLE_EARTH_UNC)
    {
      source_buf_ptr->z_NavPos.f_ErrorEllipse[2] = CGPS_WHOLE_EARTH_UNC;
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Error Ellipse perp greater than earth radius", 0, 0, 0);
    }
    dest_buf_ptr->pd_data.ext_pos_data.f_loc_unc_horizontal = sqrt((source_buf_ptr->z_NavPos.f_ErrorEllipse[1] *
                                                                    source_buf_ptr->z_NavPos.f_ErrorEllipse[1]) + (source_buf_ptr->z_NavPos.f_ErrorEllipse[2] *
                                                                                                                   source_buf_ptr->z_NavPos.f_ErrorEllipse[2]));
    dest_buf_ptr->pd_data.ext_pos_data.f_alongAxisUnc = source_buf_ptr->z_NavPos.f_ErrorEllipse[1];
    dest_buf_ptr->pd_data.ext_pos_data.f_perpAxisUnc = source_buf_ptr->z_NavPos.f_ErrorEllipse[2];
    dest_buf_ptr->pd_data.ext_pos_data.timestamp_gps_week = source_buf_ptr->z_NavPos.w_GpsWeek;
    dest_buf_ptr->pd_data.ext_pos_data.timestamp_tow_ms = source_buf_ptr->z_NavPos.q_GpsTimeMs;
    dest_buf_ptr->pd_data.ext_pos_data.time_stamp_unc = (uint32)source_buf_ptr->z_NavPos.f_ClockBiasUncMs;
    dest_buf_ptr->pd_data.ext_pos_data.h_dop = (uint32)(source_buf_ptr->z_NavPos.f_HDOP * 10.0 + 0.5);
    dest_buf_ptr->pd_data.ext_pos_data.v_dop = (uint32)(source_buf_ptr->z_NavPos.f_VDOP * 10.0 + 0.5);
    dest_buf_ptr->pd_data.ext_pos_data.p_dop = (uint32)(source_buf_ptr->z_NavPos.f_PDOP * 10.0 + 0.5);
    dest_buf_ptr->pd_data.ext_pos_data.t_dop = (uint32)(source_buf_ptr->z_NavPos.f_TDOP * 10.0 + 0.5);
    dest_buf_ptr->pd_data.ext_pos_data.g_dop = (uint32)(source_buf_ptr->z_NavPos.f_GDOP * 10.0 + 0.5);

    if (tm_util_calculate_utc(source_buf_ptr, &dest_buf_ptr->pd_data.ext_pos_data.utc_time, NULL) == FALSE)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GPS/GLO time both invalid, UTC invalid! GPSWeek=%d GLO4Year=%d",
            source_buf_ptr->z_NavPos.w_GpsWeek,
            source_buf_ptr->z_NavPos.u_GloNumFourYear, 0);
      dest_buf_ptr->pd_data.ext_opt_field_mask &= ~(PDSM_PD_UTC_TIME_VALID);
    }

    xtra_valid = tm_xtra_set_xtra_time_and_validity(&p_download_info);
    /* Init the time for user.
    */
    if (xtra_valid)
    {
      dest_buf_ptr->pd_data.ext_opt_field_mask |= PDSM_PD_XTRA_DATA_VALID;
      dest_buf_ptr->pd_data.ext_pos_data.xtra_start_gps_minutes = p_download_info.startGpsMinutes;
      dest_buf_ptr->pd_data.ext_pos_data.xtra_start_gps_week = p_download_info.startGpsWeek;
      dest_buf_ptr->pd_data.ext_pos_data.xtra_valid_duration_hours = p_download_info.validDurationHours;
    }
    /* time uncertainty */
    dest_buf_ptr->pd_data.ext_pos_data.gps_time_unc_ms = source_buf_ptr->z_NavPos.f_ClockBiasUncMs;

    if (source_buf_ptr->z_NavPos.w_GpsWeek == C_GPS_WEEK_UNKNOWN)
    {
      // If GPS time is unknown, mark all GPS-time related fields as invalid for Intermediate Report
      dest_buf_ptr->pd_data.opt_field_mask &= ~(PDSM_PD_UTCOFFSET_VALID | PDSM_PD_MSEC_VALID);
      dest_buf_ptr->pd_data.ext_opt_field_mask &= ~(PDSM_PD_GPS_TIME_VALID);
    }
      
    /* Extended time source */
    dest_buf_ptr->pd_data.ext_pos_data.extended_time_source =
       (pdsm_extended_time_src_e_type)source_buf_ptr->z_NavPos.u_InitGpsTimeSrc;

    dest_buf_ptr->pd_data.e_HoriRelIndicator =
       source_buf_ptr->z_NavPos.e_HoriRelIndicator;
    dest_buf_ptr->pd_data.e_VertRelIndicator =
       source_buf_ptr->z_NavPos.e_VertRelIndicator;
    dest_buf_ptr->pd_data.w_SensorAidingIndicator =
       source_buf_ptr->z_NavPos.w_SensorAidingIndicator;
    dest_buf_ptr->pd_data.w_SensorDataUsageMask =
       source_buf_ptr->z_NavPos.w_SensorDataUsageMask;

    /* heading & heading uncertainty */
    dest_buf_ptr->pd_data.ext_pos_data.heading_rad = source_buf_ptr->z_NavPos.f_HeadingRad;
    dest_buf_ptr->pd_data.ext_pos_data.heading_unc_rad = source_buf_ptr->z_NavPos.f_HeadingUncRad;
    dest_buf_ptr->pd_data.ext_pos_data.gnss_heading_rad = source_buf_ptr->z_NavPos.f_GnssHeadingRad;
    dest_buf_ptr->pd_data.ext_pos_data.gnss_heading_unc_rad = source_buf_ptr->z_NavPos.f_GnssHeadingUncRad;

    /* Spoof Report */
    dest_buf_ptr->pd_data.ext_pos_data.spoof_report = sdm_GetSpoofReport();

    /* GNSS - SVs, FineSpeec & GoodParity */
    dest_buf_ptr->pd_data.z_NavSvInfo = source_buf_ptr->z_NavSvInfo;

    /* Sensor bias report */
    dest_buf_ptr->pd_data.z_OnDeviceSensorParam.w_SeqenceNumber = source_buf_ptr->z_OnDeviceSensorParam.w_SeqenceNumber;

    if (source_buf_ptr->z_OnDeviceSensorParam.z_AccelBias.w_SensorValidMask != 0)
    {
      dest_buf_ptr->pd_data.z_OnDeviceSensorParam.z_AccelBias.w_SensorValidMask = source_buf_ptr->z_OnDeviceSensorParam.z_AccelBias.w_SensorValidMask;
      dest_buf_ptr->pd_data.z_OnDeviceSensorParam.z_AccelBias.f_XaxisValue = source_buf_ptr->z_OnDeviceSensorParam.z_AccelBias.f_XaxisValue;
      dest_buf_ptr->pd_data.z_OnDeviceSensorParam.z_AccelBias.f_YaxisValue = source_buf_ptr->z_OnDeviceSensorParam.z_AccelBias.f_YaxisValue;
      dest_buf_ptr->pd_data.z_OnDeviceSensorParam.z_AccelBias.f_ZaxisValue = source_buf_ptr->z_OnDeviceSensorParam.z_AccelBias.f_ZaxisValue;
    }

    if (source_buf_ptr->z_OnDeviceSensorParam.z_GyroBias.w_SensorValidMask != 0)
    {
      dest_buf_ptr->pd_data.z_OnDeviceSensorParam.z_GyroBias.w_SensorValidMask = source_buf_ptr->z_OnDeviceSensorParam.z_GyroBias.w_SensorValidMask;
      dest_buf_ptr->pd_data.z_OnDeviceSensorParam.z_GyroBias.f_XaxisValue = source_buf_ptr->z_OnDeviceSensorParam.z_GyroBias.f_XaxisValue;
      dest_buf_ptr->pd_data.z_OnDeviceSensorParam.z_GyroBias.f_YaxisValue = source_buf_ptr->z_OnDeviceSensorParam.z_GyroBias.f_YaxisValue;
      dest_buf_ptr->pd_data.z_OnDeviceSensorParam.z_GyroBias.f_ZaxisValue = source_buf_ptr->z_OnDeviceSensorParam.z_GyroBias.f_ZaxisValue;
    }

    dest_buf_ptr->pd_data.ext_pos_data.v_JammerIndicatorListValid =
       tm_core_populate_jammer_indicator(&source_buf_ptr->z_NavPos,
                                         dest_buf_ptr->pd_data.ext_pos_data.z_JammerIndicatorList);

    (void)pdsm_freebuf((char *)lr_resp_ptr);
  }
}


/*===========================================================================

FUNCTION tm_core_xlate_best_avail_pos_to_ext_status

DESCRIPTION convert a BestAvailPos fix report report to pd_ext_status report

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_xlate_best_avail_pos_to_ext_status
(
   pdsm_pd_ext_pos_s_type    *dest_buf_ptr,
   sm_FixInfoStructType      *source_buf_ptr,
   uint32                    remaing_fix_time
   )
{
  if (dest_buf_ptr == NULL || source_buf_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "NULL ptr!", 0, 0, 0);
    return;
  }

  memset(dest_buf_ptr, 0, sizeof(pdsm_pd_ext_pos_s_type));

  tm_core_xlate_pos_to_ext_status(dest_buf_ptr, source_buf_ptr, remaing_fix_time);

  // sanity checking for VALID flags since this is BestAvailPos
  if (!source_buf_ptr->z_NavPos.z_PosFlags.b_IsValid)
  {
    /* position invalid ? */
    dest_buf_ptr->pd_data.ext_opt_field_mask  &= ~(PDSM_PD_LAT_LONG_VALID);
    dest_buf_ptr->pd_data.opt_field_mask      &= ~(PDSM_PD_UNCERTAINTY_VALID);
    dest_buf_ptr->pd_data.loc_uncertainty_conf = 0; // otherwise PE: 39, ME: 63
    return;
  }
  if (!source_buf_ptr->z_NavPos.z_SmPosFlags.b_IsPuncValid)
  {
    /* PUNC invalid ? */
    dest_buf_ptr->pd_data.opt_field_mask &= ~(PDSM_PD_UNCERTAINTY_VALID);
    dest_buf_ptr->pd_data.loc_uncertainty_conf = 0; // otherwise PE: 39, ME: 63
  }
  if (!source_buf_ptr->z_NavPos.z_SmPosFlags.b_IsGpsTimeValid)
  {
    /* GPS time invalid ? mark all related fields invalid */
    dest_buf_ptr->pd_data.opt_field_mask &= ~(PDSM_PD_UTCOFFSET_VALID | PDSM_PD_MSEC_VALID);
    dest_buf_ptr->pd_data.ext_opt_field_mask &= ~(PDSM_PD_GPS_TIME_VALID);
  }
  if (!source_buf_ptr->z_NavPos.z_PosFlags.b_IsVelValid)
  {
    /* velocity invalid ? */
    dest_buf_ptr->pd_data.opt_field_mask &= ~(PDSM_PD_VELOCITY_VALID);
  }
  if (!source_buf_ptr->z_NavPos.z_PosFlags.b_Is3D)
  {
    /* Altitude invalid ? */
    dest_buf_ptr->pd_data.opt_field_mask &= ~(PDSM_PD_ALTITUDE_VALID);
    // make sure altitude is 0 (tm_util_convert_lr may put in 500)
    dest_buf_ptr->pd_data.ext_pos_data.f_altitude           = 0.0;
    dest_buf_ptr->pd_data.altitude           = 0;
    dest_buf_ptr->pd_data.loc_uncertainty_v  = 0;
    dest_buf_ptr->pd_data.fix_type = FALSE; // 2D fix
  }
  if (!source_buf_ptr->z_NavPos.z_PosFlags.b_IsHeadingValid)
  {
    /* heading invalid ? */
    dest_buf_ptr->pd_data.ext_opt_field_mask &= ~(PDSM_PD_HEADING_VALID);
  }

  /* RH TODO Add pos inject req if PUNC is too large (in original diag pos) */
  /* Spoof Report */
  dest_buf_ptr->pd_data.ext_pos_data.spoof_report = sdm_GetSpoofReport();
}

/*===========================================================================

FUNCTION tm_core_stop_emergency_session

DESCRIPTION
  This function stops the tm-core session started to keep the RCVR ON before SM receives MO or NI
  trigger. Ensures we turns off GPS receiver/MGP engine upon e911 call ends.

DEPENDENCIES

RETURN VALUE:
  TRUE  :Session Stop accepted by TM.
  FALSE :Session Stop rejected by TM.

SIDE EFFECTS

===========================================================================*/
static boolean tm_core_stop_emergency_session(void)
{
  boolean v_accepted     = FALSE;

  /*Turning OFF RCVR on 911 call end is not applicable to all targets
    return FALSE in case this is not needed*/

  tm_sess_req_param_u_type      req_param = { 0 };
  tm_sess_req_stop_param_s_type *p_stop_params = NULL;
  ldm_client_id_e_type  e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type  e_secondary_client_id = LDM_CLIENT_ID_MAX;

  /*
   *  Create a pointer to the STOP SESSION request parameters to make
   *  the code a bit cleaner and easier to read later in this function.
   */
  p_stop_params = &req_param.stop_param;

  /* If the MGP RCVR was started as part of Early reviver ON process we
   * should ensure we send STOP Session to TM core to turn OFF MGP in the
   * case 911 call ends before an NI AGPS session originates/starts. If there
   * is no AGPS session, TM's active session handle remains as HANDLE_911
   * and Protocol stop_sess_req() will not stop the session and MGP remains ON.
   */
  if (TM_SESS_HANDLE_E911 == tm_core_info.session_info.active_sess_handle)
  {
    p_stop_params->stop_type = TM_STOP_TYPE_SESSION;
    p_stop_params->stop_reason = TM_STOP_REASON_USER_TERMINATED;

    v_accepted = tm_sess_req(TM_PRTL_TYPE_E911,
                             tm_core_info.session_info.active_sess_handle,
                             TM_SESS_REQ_STOP,
                             &req_param);

    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "E911 RCVR STOP status: %u", v_accepted);

    /* When E911 call is ended in early RCVR mode, and device is in concurrent mode, 
       initiate a switching of clients from TM */

    /* Get main and secondary client  from LDM */
    tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

    if ((LDM_CLIENT_ID_MAX == e_main_client_id) && (LDM_CLIENT_ID_APPTRK == e_secondary_client_id))
    {
      (void)tm_core_switch_secondary_to_main_client(LM_MGP_RCVR_OFF_TM_SESS_END, FALSE);
    }
  }
#if !defined ( FEATURE_GNSS_SA )
  else if (TM_SESS_HANDLE_NONE == tm_core_info.session_info.active_sess_handle)
  {
    /*On 911 Call orig, We start CPI. On 911 call end. 
      Ensure CPI is stopped if started.*/
    tm_stopCPI(TM_AUXTECH_REQ_ID_EMERGENCY_ORIG);
  }
#endif /* ! FEATURE_GNSS_SA */
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM Session will be cleaned by PRTL module :%u",
          tm_core_info.session_info.active_sess_handle);
  }
  return (v_accepted);
}

/*===========================================================================

FUNCTION tm_core_lm_sess_update_handler

DESCRIPTION
  This function is to handle sess update from LM

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_lm_sess_update_handler(void *lm_sess_update_data)
{
  tm_session_update_info_s_type *sess_update_ptr = NULL;

  if (lm_sess_update_data == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL Pointer passed", 0, 0, 0);
    return;
  }

  sess_update_ptr = (tm_session_update_info_s_type *)lm_sess_update_data;
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Recieved data from LM :%d", sess_update_ptr->e_update_type, 0, 0);
  if (sess_update_ptr)
  {
    // Dont check the session Handle for SV POLY, as its not per TM session.
    if ((sess_update_ptr->e_update_type == TM_INFO_SV_POLY_REPORT) || /* TODO: Poly report needs to be handled even when not in a session */
        (sess_update_ptr->q_handle == tm_core_info.session_info.active_sess_handle))
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Received location info from LM. Handle %d ActiveHandle %d UpdateType %d",
            sess_update_ptr->q_handle,
            tm_core_info.session_info.active_sess_handle,
            sess_update_ptr->e_update_type);
      ldm_distribute_location_info(sess_update_ptr);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Rejecting Invalid Handle %d ActiveHandle %d UpdateType %d",
            sess_update_ptr->q_handle,
            tm_core_info.session_info.active_sess_handle,
            sess_update_ptr->e_update_type);
    }
  }
}

/*===========================================================================

FUNCTION tm_core_lm_sess_info_handler

DESCRIPTION
  This function is to handle session information from LM. The session info
  is available assitance data status from PE (CD).

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_lm_sess_info_handler(void *lm_sess_info_data)
{
  tm_info_s_type *sess_info_ptr = (tm_info_s_type *)lm_sess_info_data;
  int prtlTableIndex = tm_core_get_prtl_idx_by_type(tm_core_info.session_info.from_protocol);
  prtl_sess_info_param_u_type  *sess_info_param_ptr = NULL;
  sm_InternalAssistDataStatusStructType *p_sm_AssistDataStatus = NULL;
  uint32 q_long_word2 = 0;
  uint8  u_sv_cnt = 0, u_i;
  boolean retVal = FALSE;
  prtl_event_u_type event_payload;
  pdsm_ext_status_info_s_type   *pz_pd_ext_status_info;
  nv_item_type                  nv_write_buffer;
  tm_cm_phone_state_info_s_type   z_phone_state_info;
  ldm_client_id_e_type            e_ldm_client_id = LDM_CLIENT_ID_MAX;
  lm_request_info_s_type               lm_info_req_param;
#ifdef FEATURE_GNSS_GERA_ENABLE
  #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
  pz_pd_ext_status_info = (pdsm_ext_status_info_s_type *)os_MemAlloc(sizeof(pdsm_ext_status_info_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_pd_ext_status_info)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cannot allocate memory for pdsm_ext_status_info_s_type in tm_core_lm_sess_info_handler");
    tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
    return;
  }

  memset((void *)&lm_info_req_param, 0, sizeof(lm_request_info_s_type));
  memset(pz_pd_ext_status_info, 0, sizeof(*pz_pd_ext_status_info));

  if (sess_info_ptr)
  {
    sess_info_param_ptr = (prtl_sess_info_param_u_type *)pdsm_getbuf(sizeof(prtl_sess_info_param_u_type));

    if (sess_info_param_ptr == NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to get buffer for sess_info_param_ptr", 0, 0, 0);
      (void)os_MemFree((void **)&pz_pd_ext_status_info);
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      return;
    }

    switch (sess_info_ptr->e_info_type)
    {
      case TM_INFO_AIDING_DATA_WISHLIST:
        {
          memset(&z_phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
          tm_cm_iface_get_phone_state(&z_phone_state_info);

          if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
          {
            /* Set reason to general error for now, if return value */
            if (tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp)
            {
              memscpy((void *)&(sess_info_param_ptr->aiding_data_status),
                      sizeof(sess_info_param_ptr->aiding_data_status),
                      (void *)&(sess_info_ptr->z_info.z_wishlist),
                      sizeof(sess_info_ptr->z_info.z_wishlist));

              p_sm_AssistDataStatus = &(sess_info_param_ptr->aiding_data_status.z_assist_data_status);

              MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TmCore proc. LM wishlist: 0x%X (GPS) 0x%X (GLONASS) 0x%X (BDS), 0x%X (QZSS)",
                    p_sm_AssistDataStatus->q_WishListMask,
                    p_sm_AssistDataStatus->z_GloAssistData.q_WishListMask,
                    p_sm_AssistDataStatus->z_BdsAssistData.q_WishListMask,
                    p_sm_AssistDataStatus->z_QzssAssistData.q_WishListMask);

              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TmCore proc. LM wishlist: 0x%lX (GAL)", p_sm_AssistDataStatus->z_GalAssistData.q_WishListMask,
                    0, 0);

              tm_core_info.session_info.eph_svmask = p_sm_AssistDataStatus->q_EphHaveList;
              tm_core_info.session_info.alm_svmask = p_sm_AssistDataStatus->q_AlmHaveList;
              tm_core_info.session_info.iono = 0;
              if (p_sm_AssistDataStatus->q_WishListMask & C_GPS_ASSIS_IONO)
              {
                tm_core_info.session_info.iono = 1;
              }
#ifdef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
              #error code not present
#endif

              tm_core_info.session_info.utc = 0;
              if (p_sm_AssistDataStatus->q_WishListMask & C_GPS_ASSIS_UTC)
              {
                tm_core_info.session_info.utc = 1;
              }
#ifdef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
              #error code not present
#endif
              tm_core_info.session_info.health_svmask = p_sm_AssistDataStatus->q_HealthHaveList;
              tm_core_info.session_info.glo_health_svmask = p_sm_AssistDataStatus->z_GloAssistData.q_HealthHaveList;
              tm_core_info.session_info.bds_health_svmask = p_sm_AssistDataStatus->z_BdsAssistData.t_HealthHaveList;
              tm_core_info.session_info.qzss_health_svmask = p_sm_AssistDataStatus->z_QzssAssistData.q_HealthHaveList;
              tm_core_info.session_info.gal_health_svmask = p_sm_AssistDataStatus->z_GalAssistData.t_HealthHaveList;

              tm_core_info.session_info.glo_eph_svmask = p_sm_AssistDataStatus->z_GloAssistData.q_EphHaveList;
              tm_core_info.session_info.glo_alm_svmask = p_sm_AssistDataStatus->z_GloAssistData.q_AlmHaveList;

              tm_core_info.session_info.bds_eph_svmask = p_sm_AssistDataStatus->z_BdsAssistData.t_EphHaveList;
              tm_core_info.session_info.bds_alm_svmask = p_sm_AssistDataStatus->z_BdsAssistData.t_AlmHaveList;

              tm_core_info.session_info.qzss_eph_svmask = p_sm_AssistDataStatus->z_QzssAssistData.q_EphHaveList;
              tm_core_info.session_info.qzss_alm_svmask = p_sm_AssistDataStatus->z_QzssAssistData.q_AlmHaveList;
              tm_core_info.session_info.gal_eph_svmask = p_sm_AssistDataStatus->z_GalAssistData.t_EphHaveList;
              tm_core_info.session_info.gal_alm_svmask = p_sm_AssistDataStatus->z_GalAssistData.t_AlmHaveList;
              tm_core_info.session_info.navic_eph_svmask = p_sm_AssistDataStatus->z_NavicAssistData.t_EphHaveList;
              tm_core_info.session_info.navic_alm_svmask = p_sm_AssistDataStatus->z_NavicAssistData.t_AlmHaveList;

              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Debug: TmCore proc. LM eph havelist: %x alm havelist %x (NAVIC)",
                    tm_core_info.session_info.navic_eph_svmask, tm_core_info.session_info.navic_alm_svmask, 0);


              /* find out if GPS almanac assistance is needed */
              if (p_sm_AssistDataStatus->q_WishListMask & C_GPS_ASSIS_ALM)
              {
                /* when PE sets this flag, it means there is at least one SV having "need_alm"
                   status. find out how many SVs have valid almanac */
                u_sv_cnt = tm_util_num_bits_set_in_long(p_sm_AssistDataStatus->q_AlmHaveList);

                if (u_sv_cnt >= C_COMPLETE_CONSTALLATION_ALM_THRESHOLD)
                {
                  /* enough alm., consider having a complete constallation */
                  p_sm_AssistDataStatus->q_WishListMask &= ~C_GPS_ASSIS_ALM;
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LM's alm. req suppressed by TM-core: %u", u_sv_cnt, 0, 0);
                }
                else
                {
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TmCore passing alm. req. to protocol", 0, 0, 0);
                }

              }

              /* find out if GLONASS almanac assistance is needed */
              if (p_sm_AssistDataStatus->z_GloAssistData.q_WishListMask & C_GPS_ASSIS_ALM)
              {
                /* when PE sets this flag, it means there is at least one SV having "need_alm"
                   status. find out how many SVs have valid almanac */
                u_sv_cnt = tm_util_num_bits_set_in_long(p_sm_AssistDataStatus->z_GloAssistData.q_AlmHaveList);

                if (u_sv_cnt >= C_COMPLETE_CONSTALLATION_GLO_ALM_THRESHOLD)
                {
                  /* enough alm., consider having a complete constallation */
                  p_sm_AssistDataStatus->z_GloAssistData.q_WishListMask &= ~C_GPS_ASSIS_ALM;
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LM's GLO alm. req suppressed by TM-core: %u", u_sv_cnt, 0, 0);
                }
                else
                {
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TmCore passing GLO alm. req. to protocol", 0, 0, 0);
                }

              }

              // RH TODO: add code here to support BDS Alm/Eph etc
              // JV TODO: add code here to support QZSS Alm/Eph etc (future)

              /* TBD set ephemeris/almanac needed flags here for Glonass also */
              /* find out if ephemeris assistance is needed */
              /* when PE sets this flag, it means there is at least one SV having "need_eph"  status */
              if (p_sm_AssistDataStatus->q_WishListMask & C_GPS_ASSIS_NAVMODL ||
                  tm_core_info.session_info.force_eph_download == TRUE)
              {
                uint8 u_min_eph_have_threshold = C_MIN_EPH_HAVE_THRESHOLD;
                uint8 u_max_eph_need_threshold = C_MAX_EPH_NEED_THRESHOLD;
                uint8 u_num_eph_have_known_in_view = 0; /* number of SVs known in view needing eph. */
                uint8 u_num_eph_need_known_in_view = 0; /* number of SVs know in view having eph. */
                uint8 u_test_decision = FALSE;

                /* count number of known in-view SVs */
                u_sv_cnt = 0;
                for (u_i = 0; u_i < 32; u_i++)
                {
                  if (p_sm_AssistDataStatus->q_VisibleSVList & 1 << u_i)
                  {
                    u_sv_cnt++;
                  }
                }

                if (u_sv_cnt < C_MIN_EPH_HAVE_THRESHOLD)
                {
                  /* this would cause eph. request if any of the in-view SV needs eph. */
                  u_min_eph_have_threshold = u_sv_cnt;
                  u_max_eph_need_threshold = 0;
                }

                /* The "need list" and "have list" are not gated with any elevation information,
                   use the visibility information (known visible) to remove any not-known-to-be-in-view SVs
                   from these lists. count number of known in-view need-eph SVs */
                u_num_eph_need_known_in_view = tm_util_num_bits_set_in_long(p_sm_AssistDataStatus->q_EphNeedList &
                                                                               p_sm_AssistDataStatus->q_VisibleSVList);
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Force eph download flag is %d", tm_core_info.session_info.force_eph_download, 0, 0);

                /* count number of known in-view have-eph SVs */
                u_num_eph_have_known_in_view = tm_util_num_bits_set_in_long(p_sm_AssistDataStatus->q_EphHaveList &
                                                                               p_sm_AssistDataStatus->q_VisibleSVList);
                tm_core_info.gps_state_info.visible_svmask = u_num_eph_have_known_in_view;
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Visible SVs in view %d", u_num_eph_have_known_in_view, 0, 0);
                /* decide if request of eph. is to be forwarded to the protocol module */
                if (((u_num_eph_need_known_in_view >= u_max_eph_need_threshold) &&
                     (u_num_eph_have_known_in_view < u_min_eph_have_threshold)
                     ) ||
                    (u_sv_cnt < C_MIN_SV_IN_VIEW_THRESHOLD)
                    )
                {

                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GPS Eph. req to protocol-sub: Nd=%x,HV=%x,VB=%x",
                        p_sm_AssistDataStatus->q_EphNeedList,
                        p_sm_AssistDataStatus->q_EphHaveList,
                        p_sm_AssistDataStatus->q_VisibleSVList);

                  u_test_decision = TRUE;

                  if (tm_core_info.session_info.force_eph_download == FALSE)
                  {
                    tm_core_info.session_info.force_eph_download = TRUE;
                    /* save the eph need request from PE because when then protocol asks again, this will
                          be not be set again */
                    tm_core_info.session_info.eph_need_sv_mask = p_sm_AssistDataStatus->q_EphNeedList;
                    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Force-GPS-eph_dld set to TRUE", 0, 0, 0);
                  }
                  else
                  {
                    /* if force eph flag was set, make sure we copy the request mask that was saved above. also
                    the wish list needs to be updated with the nav model request */
                    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Force-GPS-eph_dld TRUE, ignore suppress, NeedList", tm_core_info.session_info.eph_need_sv_mask, 0, 0);
                    p_sm_AssistDataStatus->q_WishListMask |= C_GPS_ASSIS_NAVMODL;
                    p_sm_AssistDataStatus->q_EphNeedList =  tm_core_info.session_info.eph_need_sv_mask;
                  }
                }
                else
                {
                  if (tm_core_info.session_info.force_eph_download == FALSE)
                  {
                    p_sm_AssistDataStatus->q_WishListMask &= ~C_GPS_ASSIS_NAVMODL;
                    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LM's GPS eph. req suppressed: Nd=%x,HV=%x,VB=%x",
                          p_sm_AssistDataStatus->q_EphNeedList,
                          p_sm_AssistDataStatus->q_EphHaveList,
                          p_sm_AssistDataStatus->q_VisibleSVList);
                    u_test_decision = FALSE;
                  }
                  else
                  {
                    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Force-GPS-eph-download TRUE, ignore suppression, overriding need list: %x",
                          tm_core_info.session_info.eph_need_sv_mask);
                    u_test_decision = TRUE;

                    /* use the eph need list from the previous report becuase the 2nd request from the protocol
                        will get a empty need list. we need to make sure protocol gets the right eph mask */
                    p_sm_AssistDataStatus->q_WishListMask |= C_GPS_ASSIS_NAVMODL;
                    p_sm_AssistDataStatus->q_EphNeedList =  tm_core_info.session_info.eph_need_sv_mask;
                  }
                }

                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Revised wishlist 0x%X (GPS)", p_sm_AssistDataStatus->q_WishListMask);

                {
                  uint8 i;
                  sm_log_eph_download_data logdata;
                  memset(&logdata, 0, sizeof(logdata));
                  logdata.u_TestDecision  = u_test_decision;
                  logdata.q_EphHaveList   = p_sm_AssistDataStatus->q_EphHaveList;
                  logdata.q_VisibleSVList = p_sm_AssistDataStatus->q_VisibleSVList;
                  logdata.q_SvNoExistMask = p_sm_AssistDataStatus->q_SvNoExistMask;
                  logdata.u_min_eph_have_threshold = u_min_eph_have_threshold;
                  logdata.u_max_eph_need_threshold = u_max_eph_need_threshold;
                  logdata.u_num_eph_have_known_in_view = u_num_eph_have_known_in_view;
                  logdata.u_num_eph_need_known_in_view = u_num_eph_need_known_in_view;
                  logdata.u_sv_cnt        = u_sv_cnt;
                  logdata.w_OldestToe     = p_sm_AssistDataStatus->w_Toe;
                  logdata.w_GpsWeek       = p_sm_AssistDataStatus->w_GpsWeek;
                  if (p_sm_AssistDataStatus->u_EphInfoIncl)
                  {
                    for (i = 0; i < N_SV; i++)
                    {
                      logdata.z_SvEphIode[i].w_Sv   = p_sm_AssistDataStatus->z_SvEphIode[i].w_Sv;
                      logdata.z_SvEphIode[i].w_Iode = p_sm_AssistDataStatus->z_SvEphIode[i].w_Iode;
                    }
                  }
                  sm_log_eph_download(&logdata);
                  /* log_pff_msg( THREAD_ID_SM_TM, TM_CORE_EPH_RANDOMIZATION_DEBUG_LOG, sizeof(logdata), (byte *)&logdata ); */
                }

              } /* close if-LM-indicate-at-least-one-SV-needing-eph */
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "health noex 0x%x, health have 0x%x, health bad 0x%x", p_sm_AssistDataStatus->q_HealthNoExList, p_sm_AssistDataStatus->q_HealthHaveList, p_sm_AssistDataStatus->q_HealthBadList);


              /* GLONASS eph. request decision */
              /* find out if ephemeris assistance is needed */
              /* when PE sets this flag, it means there is at least one SV having "need_eph"  status */
              if (p_sm_AssistDataStatus->z_GloAssistData.q_WishListMask & C_GPS_ASSIS_NAVMODL ||
                  tm_core_info.session_info.glo_force_eph_download == TRUE)
              {
                uint8 u_min_eph_have_threshold = C_MIN_GLO_EPH_HAVE_THRESHOLD;
                uint8 u_max_eph_need_threshold = C_MAX_GLO_EPH_NEED_THRESHOLD;
                uint8 u_num_eph_have_known_in_view = 0; /* number of SVs known in view needing eph. */
                uint8 u_num_eph_need_known_in_view = 0; /* number of SVs know in view having eph. */
                //uint8 u_test_decision = FALSE;  /* Removing set-but-not-used variable */

                /* count number of known in-view SVs */
                u_sv_cnt = 0;
                for (u_i = 0; u_i < 32; u_i++)
                {
                  if (p_sm_AssistDataStatus->z_GloAssistData.q_VisibleSVList & 1 << u_i)
                  {
                    u_sv_cnt++;
                  }
                }

                if (u_sv_cnt < C_MIN_GLO_EPH_HAVE_THRESHOLD)
                {
                  /* this would cause eph. request if any of the in-view SV needs eph. */
                  u_min_eph_have_threshold = u_sv_cnt;
                  u_max_eph_need_threshold = 0;
                }

                /* The "need list" and "have list" are not gated with any elevation information,
                   use the visibility information (known visible) to remove any not-known-to-be-in-view SVs 
                   from these lists. count number of known in-view need-eph SVs */
                u_num_eph_need_known_in_view = tm_util_num_bits_set_in_long(p_sm_AssistDataStatus->z_GloAssistData.q_EphNeedList &
                                                                               p_sm_AssistDataStatus->z_GloAssistData.q_VisibleSVList);
                MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Forced-GLO-eph_dld flag is %d, sv cnt %u",
                      tm_core_info.session_info.glo_force_eph_download, u_sv_cnt);

                /* count number of known in-view have-eph SVs */
                u_num_eph_have_known_in_view = tm_util_num_bits_set_in_long(p_sm_AssistDataStatus->z_GloAssistData.q_EphHaveList &
                                                                               p_sm_AssistDataStatus->z_GloAssistData.q_VisibleSVList);

                // tm_core_info.gps_state_info.visible_svmask = u_num_eph_have_known_in_view;  /* no counterpart for GLO yet */

                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Visible GLO SVs in view %d", u_num_eph_have_known_in_view, 0, 0);
                /* decide if request of eph. is to be forwarded to the protocol module */
                if (((u_num_eph_need_known_in_view >= u_max_eph_need_threshold) &&
                     (u_num_eph_have_known_in_view < u_min_eph_have_threshold)
                     ) ||
                    (u_sv_cnt < C_MIN_GLO_SV_IN_VIEW_THRESHOLD)
                    )
                {
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "GLO Eph. req to protocol-sub: Nd=%x,HV=%x,VB=%x",
                        p_sm_AssistDataStatus->z_GloAssistData.q_EphNeedList,
                        p_sm_AssistDataStatus->z_GloAssistData.q_EphHaveList,
                        p_sm_AssistDataStatus->z_GloAssistData.q_VisibleSVList);

                  //u_test_decision = TRUE;  /* Removing set-but-not-used variable */

                  if (tm_core_info.session_info.glo_force_eph_download == FALSE)
                  {
                    tm_core_info.session_info.glo_force_eph_download = TRUE;
                    /* save the eph need request from PE because when then protocol asks again, this will
                          be not be set again */
                    tm_core_info.session_info.glo_eph_need_sv_mask = p_sm_AssistDataStatus->z_GloAssistData.q_EphNeedList;
                    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Forced-GLO-eph_dld set to TRUE", 0, 0, 0);
                  }
                  else
                  {
                    /* if force eph flag was set, make sure we copy the request mask that was saved above. also
                    the wish list needs to be updated with the nav model request */
                    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Force-GLO-eph_dld TRUE, ignore suppress, NeedList", tm_core_info.session_info.glo_eph_need_sv_mask, 0, 0);
                    p_sm_AssistDataStatus->z_GloAssistData.q_WishListMask |= C_GPS_ASSIS_NAVMODL;
                    p_sm_AssistDataStatus->z_GloAssistData.q_EphNeedList =  tm_core_info.session_info.glo_eph_need_sv_mask;
                  }
                }
                else
                {
                  if (tm_core_info.session_info.glo_force_eph_download == FALSE)
                  {
                    p_sm_AssistDataStatus->z_GloAssistData.q_WishListMask &= ~C_GPS_ASSIS_NAVMODL;
                    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LM's GLO eph. req suppressed: Nd=%x,HV=%x,VB=%x",
                          p_sm_AssistDataStatus->z_GloAssistData.q_EphNeedList,
                          p_sm_AssistDataStatus->z_GloAssistData.q_EphHaveList,
                          p_sm_AssistDataStatus->z_GloAssistData.q_VisibleSVList);
                    //u_test_decision = FALSE;  /* Removing set-but-not-used variable */
                  }
                  else
                  {
                    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Force-GLO-eph_dld TRUE, ignore suppress, NeedList=%x",
                          tm_core_info.session_info.glo_eph_need_sv_mask);
                    //u_test_decision = TRUE;  /* Removing set-but-not-used variable */

                    /* use the eph need list from the previous report becuase the 2nd request from the protocol
                        will get a empty need list. we need to make sure protocol gets the right eph mask */
                    p_sm_AssistDataStatus->z_GloAssistData.q_WishListMask |= C_GPS_ASSIS_NAVMODL;
                    p_sm_AssistDataStatus->z_GloAssistData.q_EphNeedList =  tm_core_info.session_info.glo_eph_need_sv_mask;
                  }
                }

                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Revised wishlist 0x%X (GLO)",
                      p_sm_AssistDataStatus->z_GloAssistData.q_WishListMask);

                /* TBD: may need to handle sm_log_eph_download() for GLONASS */

              } /* close if-LM-indicate-at-least-one-SV-needing-GLO-eph */

              /* TBD: note current sm_api does not have q_HealthNoExList member */
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "GLO: health-have 0x%x, health-ad 0x%x",
                    p_sm_AssistDataStatus->z_GloAssistData.q_HealthHaveList,
                    p_sm_AssistDataStatus->z_GloAssistData.q_HealthBadList,
                    0);

              /* end GLONASS eph. request decision*/

              if (p_sm_AssistDataStatus->z_TimeUnc.u_Valid)
              {
                tm_core_info.gps_state_info.flags |= SM_GPS_STATE_TUNC_VALID;
                tm_core_info.gps_state_info.time_unc_us = p_sm_AssistDataStatus->z_TimeUnc.f_TimeUnc;
                tm_core_info.session_info.sess_info_valid_flags = TM_CORE_SESS_INFO_CACHED_INFO_STATE_VALID;
              }
              if (p_sm_AssistDataStatus->q_WishListMask & C_GPS_ASSIS_REFTIME)
              {
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "PE forces TM to request refT", 0, 0, 0);
              }
              else if ((p_sm_AssistDataStatus->z_TimeUnc.u_Valid == FALSE) ||
                       (p_sm_AssistDataStatus->z_TimeUnc.f_TimeUnc > 10000)
                       )
              {
                /* this will force the standalone module to flag as "need dload" if UMTS; it will
                   then be checked in UMTS using time unc. threhold if validity is not false */
                p_sm_AssistDataStatus->q_WishListMask |= C_GPS_ASSIS_REFTIME;
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "PE info. shows time invalid or unc > 10s", 0, 0, 0);
              }

              if (tm_core_info.session_info.op_requested & TM_OP_REQ_AIDING_DATA_STATUS)
              {
                tm_core_info.session_info.op_requested &= ~TM_OP_REQ_AIDING_DATA_STATUS;

                MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Wishlist to protocol-module: 0x%X (GPS) 0x%X (GLONASS)",
                      sess_info_param_ptr->aiding_data_status.z_assist_data_status.q_WishListMask,
                      sess_info_param_ptr->aiding_data_status.z_assist_data_status.z_GloAssistData.q_WishListMask);

                retVal = tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp(tm_core_info.session_info.active_sess_handle,
                                                                                      PRTL_SESS_INFO_STATUS,
                                                                                      sess_info_param_ptr);

                if (TM_CM_E911_STATE_CONCURRENT == z_phone_state_info.e911_state)
                {
                  if (NULL != up_cache_sess_info_param)
                  {
                    memscpy((void *)&(up_cache_sess_info_param->aiding_data_status),
                            sizeof(up_cache_sess_info_param->aiding_data_status),
                            (void *)&(sess_info_param_ptr->aiding_data_status),
                            sizeof(sess_info_param_ptr->aiding_data_status));
                    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Aiding data cached");
                  }
                  else
                  {
                    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Aiding data not cached");
                  }
                }

                if (retVal == FALSE)
                {
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Protocol rejects Aiding Data Status", 0, 0, 0);
                  tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
                }
              }
              else
              {
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Drop Aiding data status received for GET_STATE_INFO clients", 0, 0, 0);
              }
            }
            else
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No protocol callback installed 11", 0, 0, 0);
            }
          }
          if (tm_core_info.session_info.q_ReqClientMask & (TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE)))
          {
            uint8 u_num_eph_have_known_in_view = 0; /* number of SVs know in view having eph. */
            sm_OnDemandPosPuncTuncStruct on_demand_info;
            memset(&on_demand_info, 0, sizeof(on_demand_info));
            memscpy((void *)&(sess_info_param_ptr->aiding_data_status),
                    sizeof(sess_info_param_ptr->aiding_data_status),
                    (void *)&(sess_info_ptr->z_info.z_wishlist),
                    sizeof(sess_info_ptr->z_info.z_wishlist));

            p_sm_AssistDataStatus = &(sess_info_param_ptr->aiding_data_status.z_assist_data_status);
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "health noex 0x%x, health have 0x%x, health bad 0x%x", p_sm_AssistDataStatus->q_HealthNoExList, p_sm_AssistDataStatus->q_HealthHaveList, p_sm_AssistDataStatus->q_HealthBadList);
            tm_core_info.gps_state_info.alm_svmask = p_sm_AssistDataStatus->q_AlmHaveList;
            tm_core_info.gps_state_info.eph_svmask = p_sm_AssistDataStatus->q_EphHaveList;
            tm_core_info.gps_state_info.health_svmask = p_sm_AssistDataStatus->q_HealthHaveList;
            tm_core_info.gps_state_info.health_bad_svmask = p_sm_AssistDataStatus->q_HealthBadList;
            tm_core_info.gps_state_info.health_noex_svmask = p_sm_AssistDataStatus->q_HealthNoExList;
            tm_core_info.gps_state_info.health_unk_svmask = p_sm_AssistDataStatus->q_HealthNeedList;
            tm_core_info.gps_state_info.iono_valid = 1;

            q_long_word2 = p_sm_AssistDataStatus->q_EphHaveList & p_sm_AssistDataStatus->q_VisibleSVList;

            tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_EPH_VALID;
            tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_ALM_VALID;
            tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_IONO_VALID;
            tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_VISIBLE_SV_VALID;
            tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_HEALTH_VALID;

            u_num_eph_have_known_in_view = 0;
            for (u_i = 0; u_i < 32; u_i++)
            {
              if (q_long_word2 & 1 << u_i)
              {
                u_num_eph_have_known_in_view++;
              }
            }
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Visible SVs with eph 0x%x, eph list 0x%x, visible sv list 0x%x", u_num_eph_have_known_in_view,
                  p_sm_AssistDataStatus->q_EphHaveList,
                  p_sm_AssistDataStatus->q_AlmHaveList);
            tm_core_info.gps_state_info.visible_svmask = p_sm_AssistDataStatus->q_VisibleSVList;
            if (p_sm_AssistDataStatus->q_WishListMask & C_GPS_ASSIS_IONO)
            {
              tm_core_info.gps_state_info.iono_valid = 0;
            }

            // todo. need to hook up with the pv mgr
            tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].w_TmRcvdDataMask |= TM_RCVD_ASSIST_DATA_FROM_PE;
            if (tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].w_TmRcvdDataMask == TM_RCVD_ALL)
            {
              pz_pd_ext_status_info->client_id = tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].q_ClientId;
              tm_core_xlate_gps_state_to_ext_status(&tm_core_info.gps_state_info, &(pz_pd_ext_status_info->ext_status_info.ext_gps_state_type));
              tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_GPS_STATE_INFO, pz_pd_ext_status_info);
              tm_core_info.session_info.q_ReqClientMask &= ~(TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE));
              tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].w_TmRcvdDataMask = 0;
            }
          }
        }
        break;

      case TM_INFO_PD_API_EVENT:
        {
          tm_pdapi_pd_event_s_type *pz_pd_event_info;

          pz_pd_event_info = (tm_pdapi_pd_event_s_type *)os_MemAlloc(sizeof(tm_pdapi_pd_event_s_type), OS_MEM_SCOPE_TASK);
          if (NULL == pz_pd_event_info)
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cannot allocate memory for tm_pdapi_pd_event_s_type in tm_core_lm_sess_info_handler");
          }
          else
          {
            memset(pz_pd_event_info, 0, sizeof(*pz_pd_event_info));
          }
          if (sess_info_ptr->z_info.e_pdapi_event == TM_PD_EVENT_GPS_BEGIN)
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: Received event TM_PD_EVENT_GPS_BEGIN from LM", 0, 0, 0);
            /*update the cached MGP receiver sate*/
            tm_core_info.e_cached_rcvr_state = C_GPS_RCVR_STATE_ON;

            if (NULL != pz_pd_event_info)
            {
              pz_pd_event_info->e_pd_event = PDSM_PD_EVENT_GPS_BEGIN;
              pz_pd_event_info->pd_event_data.pd_info_type.client_id = tm_core_info.session_info.pd_param.client_id;

              tm_pdapi_pd_event_callback(pz_pd_event_info, TM_PDAPI_SESS_TYPE_NONE);
            }
          }
          else if (sess_info_ptr->z_info.e_pdapi_event == TM_PD_EVENT_GPS_DONE)
          {

            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: Received event TM_PD_EVENT_GPS_DONE from LM", 0, 0, 0);
            /*update the cached MGP receiver state*/
            tm_core_info.e_cached_rcvr_state = C_GPS_RCVR_STATE_OFF;

            if (NULL != pz_pd_event_info)
            {
              pz_pd_event_info->e_pd_event = PDSM_PD_EVENT_GPS_DONE;
              pz_pd_event_info->pd_event_data.pd_info_type.client_id = tm_core_info.session_info.pd_param.client_id;

              tm_pdapi_pd_event_callback(pz_pd_event_info, TM_PDAPI_SESS_TYPE_NONE);
            }
          }
          else if (sess_info_ptr->z_info.e_pdapi_event == TM_PD_EVENT_GPS_IDLE)
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM: Received event TM_PD_EVENT_GPS_IDLE from LM", 0, 0, 0);
            /*update the cached MGP receiver sate. There is no corresponding PDAPI event
             to be generated*/
            tm_core_info.e_cached_rcvr_state = C_GPS_RCVR_STATE_IDLE;
          }

#ifdef FEATURE_GNSS_GERA_ENABLE
          #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

          if (NULL != pz_pd_event_info)
          {
            (void)os_MemFree((void **)&pz_pd_event_info);
          }
        }
        break;

      case TM_NMEA_DATA:
        {
          if (TM_NMEA_DEBUG == sess_info_ptr->z_info.z_nmea_data.e_sentence_type)
          {
            tm_nmea_debug(&(sess_info_ptr->z_info.z_nmea_data));
          }
        }

        break;

      case TM_INFO_POS_ESTIMATE:
        {
          boolean u_valid_fix = TRUE;
          if (!sess_info_ptr->z_info.z_pos_estimate.z_GpsFixRpt.z_NavPos.z_PosFlags.b_IsValid)
          {
            if ((sess_info_ptr->z_info.z_pos_estimate.z_GpsFixRpt.z_NavPos.w_GpsWeek == C_GPS_WEEK_UNKNOWN) &&
                (sess_info_ptr->z_info.z_pos_estimate.z_GpsFixRpt.z_NavPos.q_GpsTimeMs == 0))
            {
              u_valid_fix = FALSE;
            }
          }
          if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Rcvd Pos Estimate result from MGP, fix valid %d", u_valid_fix, 0, 0);
            if (u_valid_fix)
            {
              /* Set reason to general error for now, if return value */
              if (tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp)
              {
                memscpy((void *)&(sess_info_param_ptr->ref_lr_data),
                        sizeof(sess_info_param_ptr->ref_lr_data),
                        (void *)&(sess_info_ptr->z_info.z_pos_estimate),
                        sizeof(sess_info_ptr->z_info.z_pos_estimate));
                retVal = tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp(tm_core_info.session_info.active_sess_handle,
                                                                                      PRTL_SESS_INFO_REF_LR,
                                                                                      sess_info_param_ptr);
                if (retVal == FALSE)
                {
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Protocol rejects Ref Loc data", 0, 0, 0);
                  tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
                }
              }

              else
              {
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No protocol callback installed 12", 0, 0, 0);
              }
            }
          }
          else if (tm_core_info.session_info.q_ReqClientMask != SM_GPS_STATE_INFO_REQ_TYPE_NONE)
          {
            sm_OnDemandPosPuncTuncStruct on_demand_info;

            memset(&on_demand_info, 0, sizeof(on_demand_info));
            memscpy((void *)&(sess_info_param_ptr->ref_lr_data),
                    sizeof(sess_info_param_ptr->ref_lr_data),
                    (void *)&(sess_info_ptr->z_info.z_pos_estimate),
                    sizeof(sess_info_ptr->z_info.z_pos_estimate));
            if (u_valid_fix)
            {
              memscpy((void *)&(sess_info_param_ptr->ref_lr_data),
                      sizeof(sess_info_param_ptr->ref_lr_data),
                      (void *)&(sess_info_ptr->z_info.z_pos_estimate),
                      sizeof(sess_info_ptr->z_info.z_pos_estimate));
              tm_core_info.gps_state_info.lat = (int32)(C_2_32_OVER_PI * sess_info_param_ptr->ref_lr_data.z_NavPos.d_PosLla[0]);
              tm_core_info.gps_state_info.lon = (int32)(C_2_31_OVER_PI * sess_info_param_ptr->ref_lr_data.z_NavPos.d_PosLla[1]);
              tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_POS_VALID;
              tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_ALT_VALID;
              tm_core_info.gps_state_info.altitude = (int32)(sess_info_param_ptr->ref_lr_data.z_NavPos.d_PosLla[2] * 10.0 + 0.5);
              tm_core_info.gps_state_info.loc_unc_vertical = (uint16)sess_info_param_ptr->ref_lr_data.z_NavPos.f_FilteredAltUnc;
              tm_core_info.gps_state_info.pos_unc = (uint32)sqrt((sess_info_param_ptr->ref_lr_data.z_NavPos.f_ErrorEllipse[1] *
                                                                  sess_info_param_ptr->ref_lr_data.z_NavPos.f_ErrorEllipse[1]) +
                                                                 (sess_info_param_ptr->ref_lr_data.z_NavPos.f_ErrorEllipse[2] *
                                                                  sess_info_param_ptr->ref_lr_data.z_NavPos.f_ErrorEllipse[2]));
              if (!sess_info_param_ptr->ref_lr_data.z_NavPos.z_PosFlags.b_Is3D)
              {
                tm_core_info.gps_state_info.flags &= ~PDSM_PD_EXT_GPS_STATE_ALT_VALID;
              }
            }
            else
            {
              tm_core_info.gps_state_info.flags &= ~PDSM_PD_EXT_GPS_STATE_POS_VALID;
              tm_core_info.gps_state_info.flags &= ~PDSM_PD_EXT_GPS_STATE_ALT_VALID;
            }

            if (tm_core_info.session_info.q_ReqClientMask & (TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE)))
            {
              tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].w_TmRcvdDataMask |= TM_RCVD_POS_FROM_PE;
              if (tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].w_TmRcvdDataMask == TM_RCVD_ALL)
              {
                pz_pd_ext_status_info->client_id = tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].q_ClientId;
                tm_core_xlate_gps_state_to_ext_status(&tm_core_info.gps_state_info, &(pz_pd_ext_status_info->ext_status_info.ext_gps_state_type));
                tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_GPS_STATE_INFO, pz_pd_ext_status_info);
                tm_core_info.session_info.q_ReqClientMask &= ~(TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE));
                tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].w_TmRcvdDataMask = 0;
              }
            }
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid prtl index, can not report aiding data status", 0, 0, 0);
          }
        }
        break;

      case TM_RF_INFO:

        if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
        {
          /* Set reason to general error for now, if return value */
          if (tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp)
          {

            memscpy((void *)&(sess_info_param_ptr->rf_info_data),
                    sizeof(sess_info_param_ptr->rf_info_data),
                    (void *)&(sess_info_ptr->z_info.z_rf_info.z_rf_info_struct),
                    sizeof(sess_info_ptr->z_info.z_rf_info.z_rf_info_struct));

            retVal = tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp(tm_core_info.session_info.active_sess_handle,
                                                                                  PRTL_SESS_INFO_RF_INFO,
                                                                                  sess_info_param_ptr);
            if (retVal == FALSE)
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Protocol rejects RF info data", 0, 0, 0);
              tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
            }
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No protocol callback installed 13", 0, 0, 0);
          }
        }
        else
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid prtl index, can not report aiding data status", 0, 0, 0);
        }
        break;

      case TM_GPS_TIME:

#ifdef FEATURE_CGPS_XTRA
        tm_xtra_gps_time_report_handler(
           &(sess_info_ptr->z_info.z_gps_time_info.z_gps_time_info_struct.GpsClock.z_Time)
           );
#endif /* FEATURE_CGPS_XTRA */
        break;

      case TM_INFO_MGP_EVENT:
        {
          tm_info_mgp_event_s_type *sess_info_ptr = (tm_info_mgp_event_s_type *)lm_sess_info_data;

          switch (sess_info_ptr->z_mgp_event.e_EventType)
          {
            case SM_EVENT_PE_NEW_NAVDATA:
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "NEW EPH Decoded 0x%x", sess_info_ptr->z_mgp_event.q_EventData);
              tm_core_info.session_info.eph_svmask |= 1 << (sess_info_ptr->z_mgp_event.q_EventData - 1);
              break;

            case SM_EVENT_PE_NEW_ALMANAC:
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "NEW ALM Decoded %x", sess_info_ptr->z_mgp_event.q_EventData);
              tm_core_info.session_info.alm_svmask |= 1 << (sess_info_ptr->z_mgp_event.q_EventData - 1);
              break;

            case SM_EVENT_PE_NEW_IONO_UTC:
              if (sess_info_ptr->z_mgp_event.q_EventData > 0)
              {
                MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "NEW IONO/UTC Decoded %d", sess_info_ptr->z_mgp_event.q_EventData);
              }
              break;

            case SM_EVENT_ME_CONSTELLATION_CFG_BAND1:
            case SM_EVENT_ME_CONSTELLATION_CFG_BAND5:

              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Constellation Config Event Received from ME 0x%x", sess_info_ptr->z_mgp_event.e_EventType);
#ifdef FEATURE_XTRA3_DYN_URL_SELECTION
              tm_xtra3_update_active_constellation_config_info(&sess_info_ptr->z_mgp_event);
#endif
                break;

            case SM_EVENT_ME_BOOTUP_CAL_STATUS:
                tm_core_info.tm_task_info.v_mc_task_ready = tm_GetMgpReadiness(sess_info_ptr->z_mgp_event.q_EventData);
                if (TRUE == tm_core_info.tm_task_info.v_mc_task_ready)
                {
                  tm_task_init_notifier_cb(THREAD_ID_MGP_MC);
                }
                break;

            default:
              break;
          }
        }
        break;

      case TM_INFO_OEMDRE_TD_CTRL:

        /* Set the OEM DRE Tunnel Detection Enablement control flag */
        if (sess_info_ptr->z_info.e_oemdre_td_ctrl == TM_OEMDRE_TD_CTRL_ON)
        {
          tm_core_info.config_info.oemdre_td_ctrl = TRUE;
        }
        else
        {
          tm_core_info.config_info.oemdre_td_ctrl = FALSE;
        }

        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "OEMDRETD: NEW CTRL Flag decoded as %d", sess_info_ptr->z_info.e_oemdre_td_ctrl, 0, 0);
        break;

#ifdef FEATURE_GNSS_GERA_ENABLE
      #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

      case TM_INFO_FIX_DEBUG:
        if (TM_CORE_CONFIG_DD(nmea_output_format) & PDAPI_NMEA_DEBUG)
        {
          tm_nmea_fix_debug_report(&(sess_info_ptr->z_info.z_fix_debug_info));
        }
        break;

        /* In concurrent mode; main client ends. Restart secondary client as main in LM
        */
      case TM_INFO_TM_START_LDM_SECONDARY_CLIENT:
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Received request to switch secondary client to main client");
        (void)tm_core_switch_secondary_to_main_client(sess_info_ptr->z_info.z_lm_ldm_client_disable.e_rcvr_off_reason,
                                                      sess_info_ptr->z_info.z_lm_ldm_client_disable.u_forceMgpOff);

        break;

      case TM_INFO_TM_DISABLE_LDM_CLIENT:
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Received request to disable client from LM");

        e_ldm_client_id = tm_core_map_lm_to_ldm_client(sess_info_ptr->z_info.z_lm_ldm_client_disable.e_lm_ldm_client_id);

        if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(e_ldm_client_id))
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Client %u successfully disabled at LDM", e_ldm_client_id);
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Client %u failed to disable at LDM", e_ldm_client_id);
        }
        break;

      case TM_INFO_REPORT_SARF_STATUS:

        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Received SARF notification from LM = %u", sess_info_ptr->z_info.v_sarf_status);

        if (TRUE ==  sess_info_ptr->z_info.v_sarf_status)
        {
          tm_core_info.config_info.v_sarf_status = TRUE;

          /* Clean up TM core sessions if any */
          tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR);
        }
        else
        {
          tm_core_info.config_info.v_sarf_status = FALSE;
        }

        break;

      case TM_INFO_GET_ENGINE_MONITOR_CONFIG:
        tm_core_engine_monitor_report_get_handler(&(sess_info_ptr->z_info.v_engine_monitor_config));
        break;

      case TM_INFO_REPORT_ENGINE_MONITOR_STATUS:
        tm_core_handle_engine_monitor_status_report(&(sess_info_ptr->z_info.t_engine_monitor_status));
        break;

      case TM_INFO_REPORT_ME_METRICS:
        pz_pd_ext_status_info->ext_status_type = PDSM_EXT_STATUS_REPORT_ME_METRICS;
        memscpy((void *)&(pz_pd_ext_status_info->ext_status_info.ext_report_ME_metrics),
                sizeof(pz_pd_ext_status_info->ext_status_info.ext_report_ME_metrics),
                (void *)&(sess_info_ptr->z_info.z_ME_metrics),
                sizeof(sess_info_ptr->z_info.z_ME_metrics));
        tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_REPORT_ME_METRICS, pz_pd_ext_status_info);
        break;

      case TM_INFO_TDM_EN_UPDATE:
        {
          tm_xtra3_update_precise_xtra_enablement_status(sess_info_ptr->z_info.b_isTdmEnabled);
        }
        break;

      default:
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unsupported TM info type %u", sess_info_ptr->e_info_type);
        break;

    }  /* switch(sess_info_ptr->e_info_type) */

    (void)pdsm_freebuf((char *)sess_info_param_ptr);
  }
  (void)os_MemFree((void **)&pz_pd_ext_status_info);
}

/*===========================================================================

FUNCTION tm_core_delete_inject_aiding_data

DESCRIPTION
  This function is called to delete or inject aiding data.

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_delete_inject_aiding_data
(
   pdsm_delete_parms_type *delete_param_ptr,
   boolean force_rcvr_off,
   boolean delete_all,
   pdsm_pa_e_type delete_type
   )
{
  lm_request_info_s_type lm_delete_req_info = { 0 };
  lm_request_info_s_type lm_delete_req_info_qzss = { 0 };
  lm_request_info_s_type lm_delete_req_info_glo = { 0 };
  lm_request_info_s_type lm_delete_req_info_bds = { 0 };
  lm_request_info_s_type lm_delete_req_info_gal = { 0 };
  lm_request_info_s_type lm_delete_req_info_navic = { 0 };
  lm_request_info_s_type lm_delete_req_info_sbas = { 0 };
  lm_info_s_type         *lm_inject_req_info_ptr = NULL;
  boolean b_delete_mgp_gps = FALSE;
  boolean b_delete_mgp_glo = FALSE;
  boolean b_delete_mgp_bds = FALSE;
  boolean b_delete_mgp_qzss = FALSE;
  boolean b_delete_mgp_gal = FALSE;
  boolean b_delete_mgp_navic = FALSE;
  boolean b_delete_mgp_sbas = FALSE;
  char b_Buffer[250];

  if (delete_param_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null delete_param_ptr", 0, 0, 0);
    return;
  }


  (void)snprintf(b_Buffer, sizeof(b_Buffer), "Deletion mask: %llx, Deletion ext mask: %llx, %d, %d, %d \r\n",
        delete_param_ptr->pdsm_delete_parms_flags, 
        delete_param_ptr->pdsm_delete_parms_flags_ext,
        force_rcvr_off, delete_all, delete_type);
  MSG_SPRINTF_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "%s", b_Buffer);  

  if (delete_all)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting ALL aiding data", 0, 0, 0);

    lm_delete_req_info.e_req_type = LM_REQUEST_DELETE_MGP_INFO;
    lm_delete_req_info.z_request.z_delete_request.delete_all = TRUE;
    lm_delete_req_info.z_request.z_delete_request.force_rcvr_off = force_rcvr_off;

    /* Deletes GPS and Glonass data */
    lm_delete_req_info.z_request.z_delete_request.z_mgp_info.e_GnssType = MGP_GNSS_TYPE_ALL;

    tm_core_clear_cached_pos();
    (void)lm_request(&lm_delete_req_info);
    return;
  }

  if ((delete_param_ptr->pdsm_delete_parms_flags & PA_AIDING_DATA_DELETE_MASK) ||
      (delete_param_ptr->pdsm_delete_parms_flags_ext & PA_AIDING_DATA_DELETE_MASK_EXT) ||
      (delete_param_ptr->q_delete_xtra_int_mask & PDSM_PA_DELETE_XTRA_INTEGRITY_ALL))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting aiding data", 0, 0, 0);

    lm_delete_req_info.e_req_type = LM_REQUEST_DELETE_MGP_INFO;
    lm_delete_req_info.z_request.z_delete_request.delete_all = FALSE;
    lm_delete_req_info.z_request.z_delete_request.force_rcvr_off = force_rcvr_off;

    lm_delete_req_info_qzss.e_req_type = LM_REQUEST_DELETE_MGP_INFO;
    lm_delete_req_info_qzss.z_request.z_delete_request.delete_all = FALSE;
    lm_delete_req_info_qzss.z_request.z_delete_request.force_rcvr_off = force_rcvr_off;

    lm_delete_req_info_glo.e_req_type = LM_REQUEST_DELETE_MGP_INFO;
    lm_delete_req_info_glo.z_request.z_delete_request.delete_all = FALSE;
    lm_delete_req_info_glo.z_request.z_delete_request.force_rcvr_off = force_rcvr_off;

    lm_delete_req_info_bds.e_req_type = LM_REQUEST_DELETE_MGP_INFO;
    lm_delete_req_info_bds.z_request.z_delete_request.delete_all = FALSE;
    lm_delete_req_info_bds.z_request.z_delete_request.force_rcvr_off = force_rcvr_off;

    lm_delete_req_info_gal.e_req_type = LM_REQUEST_DELETE_MGP_INFO;
    lm_delete_req_info_gal.z_request.z_delete_request.delete_all = FALSE;
    lm_delete_req_info_gal.z_request.z_delete_request.force_rcvr_off = force_rcvr_off;

    lm_delete_req_info_navic.e_req_type = LM_REQUEST_DELETE_MGP_INFO;
    lm_delete_req_info_navic.z_request.z_delete_request.delete_all = FALSE;
    lm_delete_req_info_navic.z_request.z_delete_request.force_rcvr_off = force_rcvr_off;

    lm_delete_req_info_sbas.e_req_type = LM_REQUEST_DELETE_MGP_INFO;
    lm_delete_req_info_sbas.z_request.z_delete_request.delete_all = FALSE;
    lm_delete_req_info_sbas.z_request.z_delete_request.force_rcvr_off = force_rcvr_off;

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_TIME)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting Time", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteClockMask = C_DELETE_INFO_CLOCK_INFO;
      /* Time/clock info is independent of GPS/GLO, so technically we don't need to mark GPS.
         But we need this otherwise no call will be made to delete data. */
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_CLOCK_INFO)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GPS/GLONASS/BDS/GAL/NAVIC Time using mask 0x%8x", delete_param_ptr->delete_clock_info_mask);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteClockMask = delete_param_ptr->delete_clock_info_mask;
      /* Clock is structure is common for GPS and Glo, hence calling either delete is fine*/
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_TIME_GPS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GPS Time", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteClockMask |= C_DELETE_INFO_CLOCK_GPSTIME_EST;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_POS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting Pos", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeletePosMask = C_DELETE_INFO_POS_INFO;
      /* Position is independent of GPS/GLO, so technically we don't need to mark GPS.
         But we need this otherwise no call will be made to delete data. */
      b_delete_mgp_gps = TRUE;
      tm_core_clear_cached_pos();
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_EPH)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GPS EPH", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteGpsEphMask = delete_param_ptr->eph_sv_mask;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GPS ALM", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteGpsAlmMask = delete_param_ptr->alm_sv_mask;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_CORR)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GPS XTRA", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.u_DeleteXtra = TRUE;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_IONO)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting IONO", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.v_DeleteIono = TRUE;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_MB_DATA)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting Multiband Data", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.v_DeleteMultiband = TRUE;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_RTI)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting RTI", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteRtiMask = C_DELETE_INFO_RTI_INFO;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_HEALTH)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GPS HEALTH", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteGpsSvHealthMask = (uint32)C_DELETE_INFO_GPS_SV_HEALTH_INFO;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVDIR)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV DIR GPS", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.t_DeleteSvDirMask = (uint64)C_DELETE_INFO_SV_DIR_INFO & 0x00000000FFFFFFFFULL;
      b_delete_mgp_gps = TRUE;
    }
    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVSTEER)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV STEER GPS", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.t_DeleteSvSteerMask = (uint64)C_DELETE_INFO_SV_STEER_INFO & 0x00000000ffffffffULL;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SADATA)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV DATA", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteSaDataMask = C_DELETE_INFO_SA_DATA_INFO;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_FREQ_BIAS_EST)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting Freq Bias Est ", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.u_DeleteFreqBiasEst = C_DELETE_INFO_FREQ_BIAS_EST;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_CELLDB_INFO)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting CellDB information. Mask %08X", delete_param_ptr->delete_celldb_mask, 0, 0);

      /* Translate the Delete Mask from PDAPI definitions to GPS core mask definitions */
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask = 0;

      /* Celldb is independent of GPS/GLO, so technically we don't need to mark GPS.
         But we need this otherwise no call will be made to delete data. */
      b_delete_mgp_gps = TRUE;

      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_POS_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_POS;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_LATEST_GPS_POS_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_LATEST_GPS_POS;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_OTA_POS_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_OTA_POS;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_EXT_REF_POS_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_EXT_REF_POS;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_TIMETAG_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_TIMETAG;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_CELLID_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_CELLID;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_CACHED_CELLID_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_CACHED_CELLID;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_LAST_SRV_CELL_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_LAST_SRV_CELL;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_CUR_SRV_CELL_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_CUR_SRV_CELL;
      }
      if (delete_param_ptr->delete_celldb_mask & PDSM_PA_DELETE_CELLDB_MASK_NEIGHBOR_INFO_INFO)
      {
        lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteCellDBMask |= C_DELETE_INFO_CELLDB_NEIGHBOR_INFO;
      }
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SV_NO_EXIST)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting SV No Exist ", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.t_DeleteSvNoExistMask = C_DELETE_INFO_SV_NON_EXIST;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GPS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting GNSS SV blacklist GPS", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.t_DeleteBlacklistGpsMask = (uint64)delete_param_ptr->gps_gnss_sv_blacklist_mask;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_UTC)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting UTC", 0, 0, 0);
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.u_DeleteUtc = TRUE;
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_TGD_DATA_GPS)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GPS Group Delay");
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteTgdMask |= (1UL << C_DELETE_INFO_TGD_GPS);
      b_delete_mgp_gps = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_TIME_GLO)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GLO Time", 0, 0, 0);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.q_DeleteClockMask |= C_DELETE_INFO_CLOCK_GLOTIME_EST;
      b_delete_mgp_glo = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_EPH_GLO)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting Glo EPH, mask:0X%X",
            delete_param_ptr->glo_eph_sv_mask);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.q_DeleteGloEphMask = delete_param_ptr->glo_eph_sv_mask;
      b_delete_mgp_glo = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_GLO)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting Glo ALM, mask:0X%X",
            delete_param_ptr->glo_alm_sv_mask);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.q_DeleteGloAlmMask = delete_param_ptr->glo_alm_sv_mask;
      b_delete_mgp_glo = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_CORR_GLO)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GLO XTRA", 0, 0, 0);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.u_DeleteXtra = TRUE;
      b_delete_mgp_glo = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVDIR_GLO)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV DIR GLO", 0, 0, 0);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.t_DeleteSvDirMask = C_DELETE_INFO_SV_DIR_INFO;
      b_delete_mgp_glo = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVSTEER_GLO)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV STEER GLO", 0, 0, 0);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.t_DeleteSvSteerMask = C_DELETE_INFO_SV_STEER_INFO;
      b_delete_mgp_glo = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GLO)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting GNSS SV blacklist GLO, mask:0X%X",
            delete_param_ptr->glo_gnss_sv_blacklist_mask);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.t_DeleteBlacklistGloMask = delete_param_ptr->glo_gnss_sv_blacklist_mask;
      b_delete_mgp_glo = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_HEALTH_GLO)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting GNSS SV Health GLO", 0, 0, 0);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.q_DeleteGloSvHealthMask = (uint32)C_DELETE_INFO_GLO_SV_HEALTH_INFO;
      b_delete_mgp_glo = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_UTC)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting UTC", 0, 0, 0);
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.u_DeleteUtc = TRUE;
      b_delete_mgp_glo = TRUE;
    }
    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_TGD_DATA_GLO)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GLO Group Delay");
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteTgdMask |= (1UL << C_DELETE_INFO_TGD_GLO);
      b_delete_mgp_glo = TRUE;
    }
    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_TIME_BDS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting BDS Time", 0, 0, 0);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.q_DeleteClockMask |= C_DELETE_INFO_CLOCK_BDSTIME_EST;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_EPH_BDS)
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting BDS EPH, mask:0X%X %X",
            (delete_param_ptr->bds_eph_sv_mask >> 32) & 0xFFFFFFFF,
            delete_param_ptr->bds_eph_sv_mask & 0xFFFFFFFF);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.t_DeleteBdsEphMask = delete_param_ptr->bds_eph_sv_mask;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_BDS)
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting BDS ALM, mask:0X%X %X",
            (delete_param_ptr->bds_alm_sv_mask >> 32) & 0xFFFFFFFF,
            delete_param_ptr->bds_alm_sv_mask & 0xFFFFFFFF);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.t_DeleteBdsAlmMask = delete_param_ptr->bds_alm_sv_mask;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_CORR_BDS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting BDS XTRA ", 0, 0, 0);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.u_DeleteXtra = TRUE;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVDIR_BDS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV DIR BDS", 0, 0, 0);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.t_DeleteBdsSvDirMask = C_DELETE_INFO_BDS_SV_DIR_INFO;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVSTEER_BDS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV STEER BDS", 0, 0, 0);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.t_DeleteBdsSvSteerMask = C_DELETE_INFO_BDS_SV_STEER_INFO;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_GNSS_SV_BLACKLIST_BDS)
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting GNSS SV blacklist BDS, mask:0X%X %X",
            (delete_param_ptr->bds_gnss_sv_blacklist_mask >> 32) & 0xFFFFFFFF,
            delete_param_ptr->bds_gnss_sv_blacklist_mask & 0xFFFFFFFF);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.t_DeleteBlacklistBdsMask = delete_param_ptr->bds_gnss_sv_blacklist_mask;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_HEALTH_BDS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting GNSS SV HEALTH BDS", 0, 0, 0);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.t_DeleteBdsSvHealthMask = C_DELETE_INFO_BDS_SV_HEALTH_INFO;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_IONO_BDS)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting BDS IONO");
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.v_DeleteIono = TRUE;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_UTC)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting UTC", 0, 0, 0);
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.u_DeleteUtc = TRUE;
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_TGD_DATA_BDS)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting BDS Group Delay");
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteTgdMask |= (1UL << C_DELETE_INFO_TGD_BDS);
      b_delete_mgp_bds = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_EPH_QZSS)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting QZSS EPH, mask:0X%X",
            delete_param_ptr->qzss_eph_sv_mask);
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.q_DeleteQzssEphMask = delete_param_ptr->qzss_eph_sv_mask;
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_QZSS)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting QZSS ALM, mask:0X%X",
            delete_param_ptr->qzss_alm_sv_mask);
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.q_DeleteQzssAlmMask = delete_param_ptr->qzss_alm_sv_mask;
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_CORR_QZSS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting QZSS XTRA", 0, 0, 0);
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.u_DeleteXtra = TRUE;
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_IONO_QZSS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting QZSS IONO", 0, 0, 0);
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.v_DeleteIono = TRUE;
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_HEALTH_QZSS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting QZSS HEALTH", 0, 0, 0);
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.q_DeleteQzssSvHealthMask = (uint32)C_DELETE_INFO_QZSS_SV_HEALTH_INFO;
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVDIR_QZSS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV DIR QZSS", 0, 0, 0);
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.t_DeleteSvDirMask = (uint64)C_DELETE_INFO_SV_DIR_INFO << 32;
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVSTEER_QZSS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV STEER QZSS", 0, 0, 0);
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.t_DeleteSvSteerMask = (uint64)C_DELETE_INFO_SV_STEER_INFO << 32;
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_GNSS_SV_BLACKLIST_QZSS)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting GNSS SV blacklist QZSS, mask:0X%X",
            delete_param_ptr->qzss_gnss_sv_blacklist_mask);
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.t_DeleteBlacklistGpsMask = ((uint64)delete_param_ptr->qzss_gnss_sv_blacklist_mask) << 32;
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_TGD_DATA_QZSS)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting QZSS Group Delay");
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteTgdMask |= (1UL << C_DELETE_INFO_TGD_QZSS);
      b_delete_mgp_qzss = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_TIME_GAL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GAL Time", 0, 0, 0);
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.q_DeleteClockMask |= C_DELETE_INFO_CLOCK_GALTIME_EST;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_EPH_GAL)
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting GAL EPH, mask:0X%X %X",
            (delete_param_ptr->gal_eph_sv_mask >> 32) & 0xFFFFFFFF,
            delete_param_ptr->gal_eph_sv_mask & 0xFFFFFFFF);
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.t_DeleteGalEphMask = delete_param_ptr->gal_eph_sv_mask;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_GAL)
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting GAL ALM, mask:0X%X %X",
            (delete_param_ptr->gal_alm_sv_mask >> 32) & 0xFFFFFFFF,
            delete_param_ptr->gal_alm_sv_mask & 0xFFFFFFFF);
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.t_DeleteGalAlmMask = delete_param_ptr->gal_alm_sv_mask;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_CORR_GAL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GAL XTRA ", 0, 0, 0);
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.u_DeleteXtra = TRUE;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVDIR_GAL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV DIR GAL", 0, 0, 0);
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.t_DeleteGalSvDirMask = C_DELETE_INFO_GAL_SV_DIR_INFO;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVSTEER_GAL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV STEER GAL", 0, 0, 0);
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.t_DeleteGalSvSteerMask = C_DELETE_INFO_GAL_SV_STEER_INFO;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_GNSS_SV_BLACKLIST_GAL)
    {
      MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting GNSS SV blacklist GAL, mask:0X%X %X",
            (delete_param_ptr->gal_gnss_sv_blacklist_mask >> 32) & 0xFFFFFFFF,
            delete_param_ptr->gal_gnss_sv_blacklist_mask & 0xFFFFFFFF);
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.t_DeleteBlacklistGalMask = delete_param_ptr->gal_gnss_sv_blacklist_mask;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_HEALTH_GAL)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting GNSS SV HEALTH GAL");
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.t_DeleteGalSvHealthMask = C_DELETE_INFO_GAL_SV_HEALTH_INFO;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_IONO_GAL)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting GAL IONO");
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.v_DeleteIono = TRUE;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_UTC)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting UTC", 0, 0, 0);
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.u_DeleteUtc = TRUE;
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_TGD_DATA_GAL)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting GAL Group Delay");
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteTgdMask |= (1UL << C_DELETE_INFO_TGD_GAL);
      b_delete_mgp_gal = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_TIME_NAVIC)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting NAVIC Time");
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.q_DeleteClockMask |= C_DELETE_INFO_CLOCK_NAVICTIME_EST;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_EPH_NAVIC)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting NAVIC EPH and Autonav correction, mask : 0X%X", delete_param_ptr->navic_eph_sv_mask);
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.w_DeleteNavicEphMask = delete_param_ptr->navic_eph_sv_mask;
      /*Since Autonav is NavIC specific so there is no mask in QMI to delete it from controll point.
        As requested by PE team Delete it with navIC ephemerish*/
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.w_DeleteNavicAnavDcMask = delete_param_ptr->navic_eph_sv_mask;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_ALM_NAVIC)
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting NAVIC ALM, mask : %X", delete_param_ptr->navic_alm_sv_mask);
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.w_DeleteNavicAlmMask = delete_param_ptr->navic_alm_sv_mask;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_ALM_CORR_NAVIC)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting NAVIC XTRA ");
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.u_DeleteXtra = TRUE;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_SVDIR_NAVIC)
    {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting NAVIC SV DIR");
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.w_DeleteNavicSvDirMask = C_DELETE_INFO_NAVIC_SV_DIR_INFO;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_EXT_DELETE_SVSTEER_NAVIC)
    {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting NAVIC SV STEER");
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.w_DeleteNavicSvSteerMask = C_DELETE_INFO_NAVIC_SV_STEER_INFO;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_GNSS_SV_BLACKLIST_NAVIC)
    {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Deleting NAVIC SV blacklist , mask : %X", delete_param_ptr->navic_gnss_sv_blacklist_mask);
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.w_DeleteBlacklistNavicMask = delete_param_ptr->navic_gnss_sv_blacklist_mask;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_HEALTH_NAVIC)
    {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting NAVIC SV HEALTH NAVIC");
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.w_DeleteNavicNavHealthMask = C_DELETE_INFO_NAVIC_SV_HEALTH_INFO;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_IONO_NAVIC)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting NAVIC IONO");
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.v_DeleteIono = TRUE;
      b_delete_mgp_navic = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags_ext & PDSM_PA_EXT_DELETE_TGD_DATA_NAVIC)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting NAVIC TGD");
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.q_DeleteTgdMask |= (1UL << C_DELETE_INFO_TGD_NAVIC);
      b_delete_mgp_navic = TRUE;
    }

    /*Start SBAS*/
    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_EPH_SBAS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SBAS EPH", 0, 0, 0);
      lm_delete_req_info_sbas.z_request.z_delete_request.z_mgp_info.q_DeleteSbasEphMask = delete_param_ptr->sbas_eph_sv_mask;
      b_delete_mgp_sbas = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_ALM_SBAS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SBAS ALM", 0, 0, 0);
      lm_delete_req_info_sbas.z_request.z_delete_request.z_mgp_info.q_DeleteSbasAlmMask = delete_param_ptr->sbas_alm_sv_mask;
      b_delete_mgp_sbas = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVDIR_SBAS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV DIR SBAS", 0, 0, 0);
      lm_delete_req_info_sbas.z_request.z_delete_request.z_mgp_info.q_DeleteSbasSvDirMask = C_DELETE_INFO_SBAS_SV_DIR_INFO;
      b_delete_mgp_sbas = TRUE;
    }

    if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_DELETE_SVSTEER_SBAS)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "Deleting SV STEER SBAS", 0, 0, 0);
      lm_delete_req_info_sbas.z_request.z_delete_request.z_mgp_info.q_DeleteSbasSvSteerMask = C_DELETE_INFO_SBAS_SV_STEER_INFO;
      b_delete_mgp_sbas = TRUE;
    }
    /*End SBAS*/

    if (delete_param_ptr->q_delete_xtra_int_mask & PDSM_PA_DELETE_XTRA_INTEGRITY_GPS)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting GPS XTRA Integrity File");
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.v_DeleteXtraIntegrity = TRUE;
      b_delete_mgp_gps = TRUE;
    }
    if (delete_param_ptr->q_delete_xtra_int_mask & PDSM_PA_DELETE_XTRA_INTEGRITY_GLO)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting GLO XTRA Integrity File");
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.v_DeleteXtraIntegrity = TRUE;
      b_delete_mgp_glo = TRUE;
    }
    if (delete_param_ptr->q_delete_xtra_int_mask & PDSM_PA_DELETE_XTRA_INTEGRITY_BDS)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting BDS XTRA Integrity File");
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.v_DeleteXtraIntegrity = TRUE;
      b_delete_mgp_bds = TRUE;
    }
    if (delete_param_ptr->q_delete_xtra_int_mask & PDSM_PA_DELETE_XTRA_INTEGRITY_GAL)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting GAL XTRA Integrity File");
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.v_DeleteXtraIntegrity = TRUE;
      b_delete_mgp_gal = TRUE;
    }
    if (delete_param_ptr->q_delete_xtra_int_mask & PDSM_PA_DELETE_XTRA_INTEGRITY_QZSS)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting QZSS XTRA Integrity File");
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.v_DeleteXtraIntegrity = TRUE;
      b_delete_mgp_qzss = TRUE;
    }
    if (delete_param_ptr->q_delete_xtra_int_mask & PDSM_PA_DELETE_XTRA_INTEGRITY_NAVIC)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Deleting NAVIC XTRA Integrity File");
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.v_DeleteXtraIntegrity = TRUE;
      b_delete_mgp_navic = TRUE;
    }


    MSG_6(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Const supported %d,%d,%d,%d,%d, %d",
          b_delete_mgp_gps, b_delete_mgp_glo, b_delete_mgp_bds, b_delete_mgp_qzss, b_delete_mgp_gal, b_delete_mgp_navic);

    /* Delete GPS specific data */
    if (b_delete_mgp_gps)
    {
      lm_delete_req_info.z_request.z_delete_request.z_mgp_info.e_GnssType = MGP_GNSS_TYPE_GPS;
      (void)lm_request(&lm_delete_req_info);
    }

    /* Delete QZSS specific data */
    if (b_delete_mgp_qzss)
    {
      lm_delete_req_info_qzss.z_request.z_delete_request.z_mgp_info.e_GnssType = MGP_GNSS_TYPE_QZSS;
      (void)lm_request(&lm_delete_req_info_qzss);
    }
    /* Delete GLO specific data */
    if (b_delete_mgp_glo)
    {
      lm_delete_req_info_glo.z_request.z_delete_request.z_mgp_info.e_GnssType = MGP_GNSS_TYPE_GLO;
      (void)lm_request(&lm_delete_req_info_glo);
    }

    /* Delete BDS specific data */
    if (b_delete_mgp_bds)
    {
      lm_delete_req_info_bds.z_request.z_delete_request.z_mgp_info.e_GnssType = MGP_GNSS_TYPE_BDS;
      (void)lm_request(&lm_delete_req_info_bds);
    }

    /* Delete GAL specific data */
    if (b_delete_mgp_gal)
    {
      lm_delete_req_info_gal.z_request.z_delete_request.z_mgp_info.e_GnssType = MGP_GNSS_TYPE_GAL;
      (void)lm_request(&lm_delete_req_info_gal);
    }

    /* Delete NavIC specific data */
    if (b_delete_mgp_navic)
    {
      lm_delete_req_info_navic.z_request.z_delete_request.z_mgp_info.e_GnssType = MGP_GNSS_TYPE_NAVIC;
      (void)lm_request(&lm_delete_req_info_navic);
    }

    /* Delete SBAS specific data */
    if (b_delete_mgp_sbas)
    {
      lm_delete_req_info_sbas.z_request.z_delete_request.z_mgp_info.e_GnssType = MGP_GNSS_TYPE_SBAS;
      (void)lm_request(&lm_delete_req_info_sbas);
    }


  }

  if (delete_param_ptr->pdsm_delete_parms_flags & PA_AIDING_DATA_INJECT_MASK)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Inject aiding data", 0, 0, 0);

    lm_inject_req_info_ptr = (lm_info_s_type *)pdsm_getbuf(sizeof(lm_info_s_type));

    if (lm_inject_req_info_ptr != NULL)
    {
      if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_TIME_UNC)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Injecting time unc", 0, 0, 0);
        lm_inject_req_info_ptr->e_info_type = LM_SESSION_INFO_TIME_UNC;
        if (delete_type == PDSM_PA_DELETE_PARAMS_EX1)
        {
          lm_inject_req_info_ptr->z_info.f_new_time_unc = delete_param_ptr->f_time_unc;
        }
        else
        {
          lm_inject_req_info_ptr->z_info.f_new_time_unc = delete_param_ptr->time_unc;
        }
        (void)lm_info(lm_inject_req_info_ptr);
      }

      if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_POS_UNC)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Injecting POS unc", 0, 0, 0);
        lm_inject_req_info_ptr->e_info_type = LM_SESSION_INFO_POS_UNC;

        if (delete_type == PDSM_PA_DELETE_PARAMS_EX1)
        {
          lm_inject_req_info_ptr->z_info.f_new_pos_unc = delete_param_ptr->pos_unc;
        }
        else
        {
          lm_inject_req_info_ptr->z_info.f_new_pos_unc = delete_param_ptr->pos_unc;
        }
        (void)lm_info(lm_inject_req_info_ptr);
      }

      if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_TIME_OFFSET)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Injecting time offset", 0, 0, 0);
        lm_inject_req_info_ptr->e_info_type = LM_SESSION_INFO_TIME_OFFSET;

        if (delete_type == PDSM_PA_DELETE_PARAMS_EX1)
        {
          lm_inject_req_info_ptr->z_info.f_time_offset = delete_param_ptr->f_time_offset;
        }
        else
        {
          lm_inject_req_info_ptr->z_info.f_time_offset = delete_param_ptr->time_offset;
        }
        (void)lm_info(lm_inject_req_info_ptr);
      }

      if (delete_param_ptr->pdsm_delete_parms_flags & PDSM_PA_POS_OFFSET)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Injecting POS offset", 0, 0, 0);
        lm_inject_req_info_ptr->e_info_type = LM_SESSION_INFO_POS_OFFSET;
        lm_inject_req_info_ptr->z_info.q_pos_offset = delete_param_ptr->pos_offset;
        (void)lm_info(lm_inject_req_info_ptr);
      }

      (void)pdsm_freebuf((char *)lm_inject_req_info_ptr);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to get buffer for lm_inject_req_info_ptr", 0, 0, 0);
    }

  }

  return;
}

/*===========================================================================

FUNCTION tm_core_lm_set_param_req

DESCRIPTION
This function is called to send SET param requests to LM

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_lm_set_param_req
(
   pdsm_pa_info_type *pz_PaInfo,
   pdsm_pa_e_type e_PaSetType
   )
{
  switch (e_PaSetType)
  {
    case PDSM_PA_CONSTELLATION_CONTROL_CONFIG:
      {
        lm_request_info_s_type lm_set_param_info = { 0 };
        lm_request_set_param_s_type *lm_set_param = &lm_set_param_info.z_request.z_set_param_request;
        lm_set_param_info.e_req_type = LM_REQUEST_SET_PARAM;
        lm_set_param->e_param_type = LM_SET_PARAM_GNSS_CONSTELLATION_CONFIG;
        lm_set_param->z_param_value.z_ConstellConfig = pz_PaInfo->z_constell_cfg;
        (void)lm_request(&lm_set_param_info);
        break;
      }

    default:
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "SetParam Type %d not supported", e_PaSetType);
        break;
      }
  } /* switch (e_PaSetType) */
}

/*===========================================================================

FUNCTION tm_core_inject_ext_time

DESCRIPTION
  This function is called to inject external time into MGP through LM

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_inject_ext_time
(
   pdsm_pd_external_time_info_type *ext_time_ptr
   )
{
  lm_request_info_s_type lm_ext_time_info;

  if (ext_time_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null ext_time_ptr", 0, 0, 0);
    return;
  }

  lm_ext_time_info.e_req_type = LM_REQUEST_SLOW_CLK_TIME_INJECT;

  lm_ext_time_info.z_request.z_slow_clk_inject_request.z_slow_clk_time.t_Milliseconds  =
     ext_time_ptr->time_msec;

  lm_ext_time_info.z_request.z_slow_clk_inject_request.z_slow_clk_time.q_Uncertainty  =
     ext_time_ptr->time_unc_msec;

  lm_ext_time_info.z_request.z_slow_clk_inject_request.z_slow_clk_time.e_TimeSource  =
     (gps_SlowClkExtTimeSourceType)ext_time_ptr->time_source;

  if (ext_time_ptr->time_is_gps)
  {
    lm_ext_time_info.z_request.z_slow_clk_inject_request.z_slow_clk_time.e_SlowClkSrc  =
       GNSS_SLOW_CLOCK_SRC_GPS;
  }
  else
  {
    lm_ext_time_info.z_request.z_slow_clk_inject_request.z_slow_clk_time.e_SlowClkSrc  =
       GNSS_SLOW_CLOCK_SRC_UTC;
  }

  /* force source to be either GPS or UTC */
  lm_ext_time_info.z_request.z_slow_clk_inject_request.e_source =
     ext_time_ptr->time_is_gps ? GNSS_SLOW_CLOCK_SRC_GPS : GNSS_SLOW_CLOCK_SRC_UTC;

  lm_ext_time_info.z_request.z_slow_clk_inject_request.z_slow_clk_time.u_DiscontinuityCnt  =
     ext_time_ptr->discontinuity_count;

  lm_ext_time_info.z_request.z_slow_clk_inject_request.z_slow_clk_time.b_ForceFlag  =
     ext_time_ptr->force_flag;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Injecting External time from source: %d", (int)ext_time_ptr->time_source, 0, 0);

  (void)lm_request(&lm_ext_time_info);
}

/*===========================================================================

FUNCTION tm_core_inject_ext_pos

DESCRIPTION
  This function is called to inject external position into MGP through LM

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_convert_ext_pos_to_fix_rpt(pdsm_pd_external_position_info_type *p_ext_pos_ptr,
                                        sm_FixInfoStructType *p_fix_rpt)
{
  boolean u_sanity_ok = TRUE;
  sm_gm_pos_fix_info fix_info;
  float  horizontal_unc = 0;
  uint8  horizontal_conf = 0;
  float  f_scale_up_to_99pct_conf;

  if ((p_ext_pos_ptr == NULL) || (NULL == p_fix_rpt))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null Ext Pos Ptr", 0, 0, 0);
    return;
  }

  memset(p_fix_rpt, 0, sizeof(*p_fix_rpt));

  /* Update validity flags */
  p_fix_rpt->z_NavPos.z_PosFlags.b_IsExternal = TRUE;
  p_fix_rpt->z_NavPos.z_PosFlags.b_IsCoarsePos = TRUE;

  /* Set WIFI source flag */
  if (p_ext_pos_ptr->source == PDSM_PD_EXTERN_COARSE_POS_SOURCE_WIFI)
  {
    p_fix_rpt->z_NavPos.z_PosFlags.b_IsWifiGenerated = TRUE;
  }

  p_fix_rpt->z_NavPos.z_PosFlags.b_IsVelValid = FALSE;
  /* Velocity valid is set to false */
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_LOW, "CPI injection velocity valid = %d", p_fix_rpt->z_NavPos.z_PosFlags.b_IsVelValid, 0, 0);

  /* Heading valid is set to false */
  p_fix_rpt->z_NavPos.z_PosFlags.b_IsHeadingValid = FALSE;

  /* Convert valid, coarse pos time stamp to GPS week and GPS msec */
  if (p_ext_pos_ptr->opt_field_mask & PDSM_PD_EXTERN_COARSE_POS_GPS_TIME_VALID)
  {
    U32 q_CoarsePosGpsTotalSec;
    if (p_ext_pos_ptr->time_info_type == PDSM_PD_EXTERN_COARSE_POS_TIME_GPS)
    {
      q_CoarsePosGpsTotalSec = p_ext_pos_ptr->timestamp_sec;
      p_fix_rpt->z_NavPos.w_GpsWeek =
         (U16)(q_CoarsePosGpsTotalSec / (U32)WEEK_SECS);
      p_fix_rpt->z_NavPos.q_GpsTimeMs =
         (q_CoarsePosGpsTotalSec % (U32)WEEK_SECS) * 1000;
      p_fix_rpt->z_NavPos.q_UtcTimeMs =
         ((p_ext_pos_ptr->timestamp_sec - (U32)cgps_GetNvUtcOffset()) % (U32)WEEK_SECS) * 1000;

#ifndef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
      /* mark the Time as valid */
      p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsGpsTimeValid = TRUE;
#endif

      MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "CPI injection extern time type= GPS GPS Week = %u, GPS Ms = %u, GPS_Time_valid:%d",
            p_fix_rpt->z_NavPos.w_GpsWeek, p_fix_rpt->z_NavPos.q_GpsTimeMs,
            p_fix_rpt->z_NavPos.q_UtcTimeMs,
            p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsGpsTimeValid);
    }
    else if (p_ext_pos_ptr->time_info_type == PDSM_PD_EXTERN_COARSE_POS_TIME_UTC)
    {
      q_CoarsePosGpsTotalSec = p_ext_pos_ptr->timestamp_sec
         + (U32)cgps_GetNvUtcOffset();
      p_fix_rpt->z_NavPos.w_GpsWeek =
         (U16)(q_CoarsePosGpsTotalSec / (U32)WEEK_SECS);
      p_fix_rpt->z_NavPos.q_GpsTimeMs =
         (q_CoarsePosGpsTotalSec % (U32)WEEK_SECS) * 1000;
      p_fix_rpt->z_NavPos.q_UtcTimeMs =
         (p_ext_pos_ptr->timestamp_sec % (U32)WEEK_SECS) * 1000;

#ifndef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
      /* mark the Time as valid */
      p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsGpsTimeValid = TRUE;
#endif

      MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "CPI injection extern time type= UTC GPS Week = %u, GPS Ms = %u, UTC ms = %u, GPS_Time_valid:%d",
            p_fix_rpt->z_NavPos.w_GpsWeek, p_fix_rpt->z_NavPos.q_GpsTimeMs,
            p_fix_rpt->z_NavPos.q_UtcTimeMs,
            p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsGpsTimeValid);

    }
    else
    {
      p_fix_rpt->z_NavPos.w_GpsWeek = C_GPS_WEEK_UNKNOWN;
      p_fix_rpt->z_NavPos.q_GpsTimeMs = 0;
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "ExternCoarsePosPut: No valid gps time info", 0, 0, 0);
    p_fix_rpt->z_NavPos.w_GpsWeek = C_GPS_WEEK_UNKNOWN;
    p_fix_rpt->z_NavPos.q_GpsTimeMs = 0;
  }

  if (p_ext_pos_ptr->opt_field_mask & PDSM_PD_EXTERN_COARSE_POS_LAT_LONG_VALID)
  {
    float  horizontal_unc = 0;
    uint8  horizontal_conf = 0;

    /* mark the position as valid */
    p_fix_rpt->z_NavPos.z_PosFlags.b_IsValid = TRUE;

    /* mark the position as valid for DBH use.
       smPosFlags is used for DBH*/
    p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsValid = TRUE;

    /* mark the position as CPI */
    p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsCoarsePos = TRUE;

    /* Convert to radians from degrees */
    p_fix_rpt->z_NavPos.d_PosLla[0] = (DBL)p_ext_pos_ptr->latitude * DEG2RAD;

    /* Convert to radians from degrees */
    p_fix_rpt->z_NavPos.d_PosLla[1] = (DBL)p_ext_pos_ptr->longitude * DEG2RAD;

    p_fix_rpt->z_NavPos.d_PosLla[2] = p_ext_pos_ptr->altitude;

    /*Check from where to pick up Horizontal Unc and Horizontal Confidence*/
    if (p_ext_pos_ptr->opt_field_mask & PDSM_PD_EXTERN_COARSE_POS_RAW_CIRCULAR_UNC_VALID)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Taking Hor Unc and Confidence from Raw field", 0, 0, 0);
      horizontal_unc = p_ext_pos_ptr->loc_unc_hor_raw;
      if (p_ext_pos_ptr->opt_field_mask & PDSM_PD_EXTERN_COARSE_POS_RAW_HORIZ_CONF_VALID)
      {
        horizontal_conf = p_ext_pos_ptr->confidence_hor_raw;
      }
      else
      {
        horizontal_conf = 50;
      }
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Not Taking Hor Unc and Confidence from Raw field", 0, 0, 0);
      horizontal_unc = p_ext_pos_ptr->loc_unc_hor;
      if (p_ext_pos_ptr->opt_field_mask & PDSM_PD_EXTERN_COARSE_POS_HORIZ_CONF_VALID)
      {
        horizontal_conf = p_ext_pos_ptr->confidence_hor;
      }
      else
      {
        horizontal_conf = 50;
      }
    }

    f_scale_up_to_99pct_conf = tm_util_get_cep_scale_ratio_from_pct((float)horizontal_conf * 0.01f, 0.99f);

    /* Update the error ellipse for horizontal position uncertainty and ensure
     the result is within bounds */
    p_fix_rpt->z_NavPos.f_ErrorEllipse[0] = 0.0;
    p_fix_rpt->z_NavPos.f_ErrorEllipse[1] = (FLT)((float)horizontal_unc * (float)f_scale_up_to_99pct_conf)
       * (FLT)(1. / 3.03); /*CEP 99% -> Error Ellipse 39%*/

    p_fix_rpt->z_NavPos.f_ErrorEllipse[2] = p_fix_rpt->z_NavPos.f_ErrorEllipse[1];

    /* Set the latitude and longitude position uncertaintes as the error ellipse
     directly. The semi-major axis corresponds to longitude b/c alpha is 0. */
    p_fix_rpt->z_NavPos.f_PuncLatLonMeters[0] = p_fix_rpt->z_NavPos.f_ErrorEllipse[2];
    p_fix_rpt->z_NavPos.f_PuncLatLonMeters[1] = p_fix_rpt->z_NavPos.f_ErrorEllipse[1];

    p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsPuncValid = TRUE;

    /* Horizontal reliability */
    if (p_ext_pos_ptr->e_HoriRelIndicator == PDSM_POSITION_RELIABILITY_NOT_SET)
    {
      p_fix_rpt->z_NavPos.e_HoriRelIndicator = PDSM_POSITION_RELIABILITY_LOW;
    }
    else
    {
      p_fix_rpt->z_NavPos.e_HoriRelIndicator = p_ext_pos_ptr->e_HoriRelIndicator;
    }

    /* Update vertical position uncertainties */
    if ((p_ext_pos_ptr->opt_field_mask & PDSM_PD_EXTERN_COARSE_POS_ALT_VALID)
        && (p_ext_pos_ptr->e_VertRelIndicator != PDSM_POSITION_RELIABILITY_VERY_LOW))
    {
      /* mark the Altitude as valid for DBH use.
         smPosFlags is used for DBH*/
      p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsAltValid = TRUE;

      /* If HAE, accept uncertainty directly */
      if (p_ext_pos_ptr->alt_def == PDSM_PD_EXTERN_COARSE_POS_ALT_HAE)
      {
        p_fix_rpt->z_NavPos.f_PuncVertMeters = (FLT)p_ext_pos_ptr->loc_unc_vert;
      }
      else
      {
        /* On receipt of an altitude in MSL from coarse position, increase uncertainty
          to compensate difference between MSL height and ellipsoidal height */
        /* Otherwise assume MSL and increase uncertainty. Optimally, the
          altitude reference could be adjusted to HAE by accounting for geoid
          in PE. This would avoid the unnecessary uncertainty increase. */
        p_fix_rpt->z_NavPos.f_PuncVertMeters =
           (FLT)p_ext_pos_ptr->loc_unc_vert + (FLT)100.0;
      }
      /* Vertical reliability */
      if (p_ext_pos_ptr->e_VertRelIndicator == PDSM_POSITION_RELIABILITY_NOT_SET)
      {
        p_fix_rpt->z_NavPos.e_VertRelIndicator = PDSM_POSITION_RELIABILITY_LOW;
      }
      else
      {
        p_fix_rpt->z_NavPos.e_VertRelIndicator = p_ext_pos_ptr->e_VertRelIndicator;
      }
      /* Capture the Altitude Source */
      p_fix_rpt->z_NavPos.e_AltitudeSource = p_ext_pos_ptr->e_AltitudeSource;
    }

    /* Compute the 3D RSS position uncertainty in terms of the components and
     ensure the result is within bounds */
    p_fix_rpt->z_NavPos.f_Punc3dMeters =
       (FLT)sqrt(p_fix_rpt->z_NavPos.f_ErrorEllipse[1] * p_fix_rpt->z_NavPos.f_ErrorEllipse[1]
                 + p_fix_rpt->z_NavPos.f_ErrorEllipse[2] * p_fix_rpt->z_NavPos.f_ErrorEllipse[2]
                 + p_fix_rpt->z_NavPos.f_PuncVertMeters * p_fix_rpt->z_NavPos.f_PuncVertMeters);

    /* Set the total RSS velocity uncertainty to maximum */
    p_fix_rpt->z_NavPos.f_Vunc3dMps = (FLT)30.0;
  }
  else if ((p_ext_pos_ptr->opt_field_mask & PDSM_PD_EXTERN_COARSE_POS_ALT_VALID)
           && (p_ext_pos_ptr->e_VertRelIndicator != PDSM_POSITION_RELIABILITY_VERY_LOW))
  {
    /* mark the Altitude as valid for DBH use.
       smPosFlags is used for DBH*/
    p_fix_rpt->z_NavPos.z_SmPosFlags.b_IsAltValid = TRUE;

    /* Process the altitude information, ensuring ECEF and LLA positions are
     consistent */
    p_fix_rpt->z_NavPos.d_PosLla[2] = p_ext_pos_ptr->altitude;

    /* If HAE, accept uncertainty directly */
    if (p_ext_pos_ptr->alt_def == PDSM_PD_EXTERN_COARSE_POS_ALT_HAE)
    {
      p_fix_rpt->z_NavPos.f_PuncVertMeters = (FLT)p_ext_pos_ptr->loc_unc_vert;
    }
    else
    {
      /* Otherwise assume MSL and increase uncertainty. Optimally, the
        altitude reference could be adjusted to HAE by accounting for geoid
        in PE. This would avoid the unnecessary uncertainty increase. */
      p_fix_rpt->z_NavPos.f_PuncVertMeters =
         (FLT)p_ext_pos_ptr->loc_unc_vert + (FLT)100.0;
    }

    /* Vertical reliability */
    if (p_ext_pos_ptr->e_VertRelIndicator == PDSM_POSITION_RELIABILITY_NOT_SET)
    {
      p_fix_rpt->z_NavPos.e_VertRelIndicator = PDSM_POSITION_RELIABILITY_LOW;
    }
    else
    {
      p_fix_rpt->z_NavPos.e_VertRelIndicator = p_ext_pos_ptr->e_VertRelIndicator;
    }
  }

  return;
}


/*===========================================================================

FUNCTION tm_core_inject_ext_pos

DESCRIPTION
  This function is called to inject external position into MGP through LM

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_inject_ext_pos
(
   pdsm_cmd_s_type *p_cmd_type
   )
{
  pdsm_pd_external_position_info_type *ext_pos_ptr;
  lm_request_info_s_type lm_ext_pos_info;
  gnss_ExternCoarsePosInjectionType   *p_lm_extern_coase_pos_inj;
  sm_FixInfoStructType *p_fix_rpt = NULL;
  sm_FixReportStructType z_FixRpt = { 0 };

  lm_request_info_s_type       lm_info_req_param;
  tm_pdapi_pd_event_s_type     pd_event_info;
  boolean                      u_sanity_ok = TRUE;
  float                        f_scale_up_to_99pct_conf;

  ext_pos_ptr = &p_cmd_type->cmd.pd.info.ext_pos_info;

  sm_log_extern_coarse_pos_inj_start(p_cmd_type); /* DM log */
  sm_report_event_coase_pos_inj_start(); /* DM event */

  memset((void *)&lm_info_req_param, 0, sizeof(lm_request_info_s_type));
  memset(&pd_event_info, 0, sizeof(tm_pdapi_pd_event_s_type));

  pd_event_info.pd_event_data.pd_info_type.client_id =
     p_cmd_type->cmd.pd.client_id;

  if (ext_pos_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null ext_pos_ptr", 0, 0, 0);
    u_sanity_ok = FALSE;
  }
  else
  {
    if (!(ext_pos_ptr->opt_field_mask &
             PDSM_PD_EXTERN_COARSE_POS_LAT_LONG_VALID) &&
        !(ext_pos_ptr->opt_field_mask &
             PDSM_PD_EXTERN_COARSE_POS_ALT_VALID)
        )
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "No hor. or vert. pos", 0, 0, 0);
      u_sanity_ok = FALSE; /* no hor. or vert. pos. info., reject */
    }
    if ((ext_pos_ptr->opt_field_mask &
            PDSM_PD_EXTERN_COARSE_POS_HORIZ_CONF_VALID) &&
        ((ext_pos_ptr->confidence_hor == 0) ||
         (ext_pos_ptr->confidence_hor > 100)))
    {
      u_sanity_ok = FALSE;
    }
    if ((ext_pos_ptr->opt_field_mask &
            PDSM_PD_EXTERN_COARSE_POS_CIRCULAR_UNC_VALID) &&
        (ext_pos_ptr->loc_unc_hor == 0))
    {
      u_sanity_ok = FALSE; /* hor. unc = 0 unreasonable, reject */
    }
    if ((ext_pos_ptr->opt_field_mask &
            PDSM_PD_EXTERN_COARSE_POS_ALT_VALID) &&
        ((ext_pos_ptr->loc_unc_vert == 0) ||
         (ext_pos_ptr->confidence_vert == 0) ||
         (ext_pos_ptr->confidence_vert > 100)
         )
        )
    {
      u_sanity_ok = FALSE; /* vert. unc = 0 unreasonable, reject */
    }
    if ((ext_pos_ptr->opt_field_mask &
            PDSM_PD_EXTERN_COARSE_POS_ELLIPTICAL_UNC_VALID) &&
        ((ext_pos_ptr->horizontal_unc_elliptical.semimajor == 0) ||
         (ext_pos_ptr->horizontal_unc_elliptical.semiminor == 0)))
    {
      u_sanity_ok = FALSE;
    }
  }

  if (!u_sanity_ok) /* reject the injection command due to sanity failure */
  {
    pd_event_info.e_pd_event = PDSM_PD_EVENT_COARSE_POS_INJ_ERROR;
    tm_pdapi_pd_event_callback(&pd_event_info, TM_PDAPI_SESS_TYPE_NONE);
    return;
  }

  p_fix_rpt = (sm_FixInfoStructType *)os_MemAlloc(sizeof(sm_FixInfoStructType), OS_MEM_SCOPE_TASK);
  if (p_fix_rpt != NULL)
  {
    tm_core_convert_ext_pos_to_fix_rpt(ext_pos_ptr, p_fix_rpt);

    /*Send to GFC_QMI*/
    tm_gfc_qmi_inject_ext_pos(p_fix_rpt);

    /*Send to TechSel*/
    techSel_ReportPosFix(p_fix_rpt);

#if !defined(FEATURE_CGPS_FW_NON_STANDARD_SUPPORT) && !defined(FEATURE_GNSS_SA)
    if (PDSM_PD_EXTERN_COARSE_POS_SOURCE_OTHER == ext_pos_ptr->source)
    {      
      memscpy(&z_FixRpt.z_FixInfo, sizeof(sm_FixInfoStructType),
              (sm_FixInfoStructType *)p_fix_rpt, sizeof(sm_FixInfoStructType));

      MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Source: Pos= %d ,Alt = %d, ALT_reliability = %d, Validity: Pos=%d, Alt=%d",
            ext_pos_ptr->source,
            z_FixRpt.z_FixInfo.z_NavPos.e_AltitudeSource,
            z_FixRpt.z_FixInfo.z_NavPos.e_VertRelIndicator,
            z_FixRpt.z_FixInfo.z_NavPos.z_SmPosFlags.b_IsValid,
            z_FixRpt.z_FixInfo.z_NavPos.z_SmPosFlags.b_IsAltValid);

      tm_CacheDBHPosition(z_FixRpt);
    }
#endif /* !FEATURE_CGPS_FW_NON_STANDARD_SUPPORT && !FEATURE_GNSS_SA */

    /*Free away with the Fix Rpt pointer. We are done with it*/
    os_MemFree((void **)&p_fix_rpt);
  }

  /* regardless if the corresponding position info. is valid, it does not hurt
     to rescale the confidence to 99 if 100 is given */
  if (ext_pos_ptr->confidence_hor == 100)
  {
    ext_pos_ptr->confidence_hor = 99;
  }
  if (ext_pos_ptr->confidence_hor_raw == 100)
  {
    ext_pos_ptr->confidence_hor_raw = 99;
  }
  if (ext_pos_ptr->confidence_vert == 100)
  {
    ext_pos_ptr->confidence_vert = 99;
  }

  /* populate the LM interface structure */

  p_lm_extern_coase_pos_inj =
     &(lm_ext_pos_info.z_request.z_extern_coarse_pos_inject_request.z_extern_coarse_pos);

  lm_ext_pos_info.e_req_type = LM_REQUEST_EXTERNAL_COARSE_POS_INJECT;

  p_lm_extern_coase_pos_inj->opt_field_mask = (gps_extern_coarse_pos_opt_field)ext_pos_ptr->opt_field_mask;

  p_lm_extern_coase_pos_inj->time_info_type = (gps_extern_coarse_pos_time_stamp_e_type)ext_pos_ptr->time_info_type;

  p_lm_extern_coase_pos_inj->timestamp_sec = ext_pos_ptr->timestamp_sec;

  p_lm_extern_coase_pos_inj->source = (gps_extern_coarse_pos_source_e_type)ext_pos_ptr->source;

  p_lm_extern_coase_pos_inj->network_source = EXTERN_COARSE_POS_NETWORK_SOURCE_UNDEFINED; /* default to undefined */
  if (1 == ext_pos_ptr->m.posSrcProviderValid)
  {
    switch (ext_pos_ptr->src_provider)
    {
      case PDSM_PD_EXTERN_CPI_SRC_EXTERNAL :
        p_lm_extern_coase_pos_inj->network_source = EXTERN_COARSE_POS_NETWORK_SOURCE_EXTERNAL;
        break;
      case PDSM_PD_EXTERN_CPI_SRC_INTERNAL :
        p_lm_extern_coase_pos_inj->network_source = EXTERN_COARSE_POS_NETWORK_SOURCE_INTERNAL;
        break;
      default:
        /* Do nothing */
        break;
    }
  }

  p_lm_extern_coase_pos_inj->latitude = ext_pos_ptr->latitude;

  p_lm_extern_coase_pos_inj->longitude = ext_pos_ptr->longitude;

  /* scale position unc. to that of 99% confidence level */
  f_scale_up_to_99pct_conf = tm_util_get_cep_scale_ratio_from_pct((float)ext_pos_ptr->confidence_hor * 0.01f, 0.99f);

  p_lm_extern_coase_pos_inj->loc_unc_hor = ((float)ext_pos_ptr->loc_unc_hor * (float)f_scale_up_to_99pct_conf);

  f_scale_up_to_99pct_conf = tm_util_get_cep_scale_ratio_from_pct((float)ext_pos_ptr->confidence_hor_raw * 0.01f, 0.99f);

  p_lm_extern_coase_pos_inj->loc_unc_hor_raw = ((float)ext_pos_ptr->loc_unc_hor_raw * (float)f_scale_up_to_99pct_conf);

  p_lm_extern_coase_pos_inj->alt_def = (gps_extern_coarse_pos_alt_def_e_type)ext_pos_ptr->alt_def;

  p_lm_extern_coase_pos_inj->altitude = ext_pos_ptr->altitude;

  /* TBD: scale position unc. to that of 99% confidence level */
  p_lm_extern_coase_pos_inj->loc_unc_vert = ext_pos_ptr->loc_unc_vert;

  p_lm_extern_coase_pos_inj->e_HoriRelIndicator = ext_pos_ptr->e_HoriRelIndicator;
  p_lm_extern_coase_pos_inj->e_VertRelIndicator = ext_pos_ptr->e_VertRelIndicator;

  p_lm_extern_coase_pos_inj->e_AltitudeSource   = ext_pos_ptr->e_AltitudeSource;
  p_lm_extern_coase_pos_inj->e_AltHorLinkage    = ext_pos_ptr->e_AltHorLinkage;
  p_lm_extern_coase_pos_inj->e_AltUncCoverage   = ext_pos_ptr->e_AltUncCoverage;

  p_lm_extern_coase_pos_inj->t_TimeTickMsec = cgps_TimeTickGetMsec();

  p_lm_extern_coase_pos_inj->horizontal_unc_elliptical.semiMajorUnc = ext_pos_ptr->horizontal_unc_elliptical.semimajor * f_scale_up_to_99pct_conf;
  p_lm_extern_coase_pos_inj->horizontal_unc_elliptical.semiMinorUnc = ext_pos_ptr->horizontal_unc_elliptical.semiminor * f_scale_up_to_99pct_conf;
  p_lm_extern_coase_pos_inj->horizontal_unc_elliptical.semiMajorAzimuthDegrees = ext_pos_ptr->horizontal_unc_elliptical.semimajor_azimuth;

  /* Determine if ext inj is allowed */
  if (TRUE == tm_core_session_active())
  {
    if (TRUE == tm_core_info.cs_on_demand_active && TRUE == tm_core_info.internal_session)
    {
      tm_core_info.ext_inj_allowed = FALSE;
    }
    else
    {
      tm_core_info.ext_inj_allowed = TRUE;
    }

    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core_info.internal_session=%d, tm_core_info.ext_inj_allowed=%d",
          tm_core_info.internal_session, tm_core_info.ext_inj_allowed, 0);
  }
  else
  {
    if (TRUE == tm_core_info.cs_on_demand_active)
    {
      tm_core_info.ext_inj_allowed = FALSE;
    }
    else
    {
      tm_core_info.ext_inj_allowed = TRUE;
    }
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Injecting external coarse pos if allowed(%d)", tm_core_info.ext_inj_allowed, 0, 0);

  if (TRUE == tm_core_info.ext_inj_allowed && lm_request(&lm_ext_pos_info))
  {
    /* Request the current GPS time from MGP */
    //lm_info_req_param.e_req_type = LM_REQUEST_POS_ESTIMATE;

    /* Send request to LM->MGP */
    //lm_request(&lm_info_req_param);

    pd_event_info.e_pd_event = PDSM_PD_EVENT_COARSE_POS_INJ_DONE;
    tm_pdapi_pd_event_callback(&pd_event_info, TM_PDAPI_SESS_TYPE_NONE);
    sm_report_event_coase_pos_inj_end(DM_EVT_EXTERN_COARSE_POS_INJ_END_REASON_DONE);
  }

  else
  {
    pd_event_info.e_pd_event = PDSM_PD_EVENT_COARSE_POS_INJ_ERROR;
    tm_pdapi_pd_event_callback(&pd_event_info, TM_PDAPI_SESS_TYPE_NONE);
    sm_report_event_coase_pos_inj_end(DM_EVT_EXTERN_COARSE_POS_INJ_END_REASON_ERROR);
  }

}

/*===========================================================================

FUNCTION tm_core_get_gps_state

DESCRIPTION
  This function is called to get the GPS Receiver state information

DEPENDENCIES

RETURN

SIDE EFFECTS

===========================================================================*/
void tm_core_get_gps_state
(
   tm_core_get_gps_state_param_s_type *p_get_gps_state_param
   )
{
  boolean ret_val = FALSE;
  lm_request_info_s_type               lm_info_req_param;
  pdsm_cmd_s_type *p_cmd_type;
  boolean error = FALSE, request_wishlist = FALSE, request_pos_estimate = TRUE;

  if (NULL == p_get_gps_state_param ||
      p_get_gps_state_param->source > SM_GPS_STATE_INFO_REQ_TYPE_MAX)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core_get_gps_state] passed invalid paramaters",
          0, 0, 0);
    return;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core entering function [tm_core_get_gps_state] source %d",
        p_get_gps_state_param->source, 0, 0);

  memset((void *)&lm_info_req_param, 0, sizeof(lm_request_info_s_type));

  tm_core_info.session_info.q_ReqClientMask |= (TM_CORE_CLIENT_BITMASK(p_get_gps_state_param->source));
  tm_core_info.session_info.z_ReqClients[p_get_gps_state_param->source].w_TmRcvdDataMask = FALSE;

  p_cmd_type = p_get_gps_state_param->cmd.p_cmd_type;

  switch (p_get_gps_state_param->source)
  {
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_TECH_SEL:
      /* Indicate that the data needs to be sent to PDAPI = FALSE */
      tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_TECH_SEL].q_ClientId = 0xFFFF;
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE:
      /* Indicate that the data needs to be sent to PDAPI = TRUE */
      memset(&tm_core_info.gps_state_info, 0, sizeof(tm_core_info.gps_state_info));
      tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].q_ClientId = p_cmd_type->cmd.pd.client_id;
      request_wishlist = TRUE;
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_QMI:
      /* Indicate that the data needs to be sent to PDAPI = TRUE */
      tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_QMI].q_ClientId = p_cmd_type->cmd.pd.client_id;
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_XTRA_T:
      /* Indicate that the data needs to be sent to PDAPI = FALSE */
      tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_XTRA_T].q_ClientId = 0xFFFF;
      request_pos_estimate = FALSE;
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_TLE_AVAIL_POS_QMI:
      /* Indicate that the data needs to be sent to PDAPI = TRUE */
      tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_TLE_AVAIL_POS_QMI].q_ClientId = p_cmd_type->cmd.pd.client_id;
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_GNSS_AVAIL_POS_QMI:
      /* Indicate that the data needs to be sent to PDAPI = TRUE */
      tm_core_info.session_info.z_ReqClients[p_get_gps_state_param->source].q_ClientId = p_cmd_type->cmd.pd.client_id;
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_DBH:
      tm_core_info.session_info.z_ReqClients[p_get_gps_state_param->source].q_ClientId = 0xFFFF;
      break;
    default:
      error = TRUE;
      break;
  }

  /* get the position, time, punc and tunc from MGP */
  if (FALSE == error)
  {
    mgp_GetGPSStateInfo(p_get_gps_state_param->source);

    if (request_wishlist)
    {
      lm_info_req_param.e_req_type = LM_REQUEST_AIDING_DATA_WISHLIST;
      lm_info_req_param.z_request.e_wishlist_req_type =
         tm_core_xlate_prtl_to_wishlist_src(TM_SESS_OPERATION_MODE_NONE);

      ret_val = lm_request(&lm_info_req_param);
    }

    if (request_pos_estimate)
    {
#ifdef FEATURE_CGPS_ODP_2_0
      // need to ask PE for the position now not MGP
      lm_info_req_param.e_req_type = LM_REQUEST_POS_ESTIMATE;
      ret_val = lm_request(&lm_info_req_param);
#endif
    }
  }

  if ((error == TRUE) || (FALSE == ret_val))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_Core: COuld not request WISH List info(err %d, retval %d Src %d)",
          error, ret_val, p_get_gps_state_param->source);
  }
}

/*===========================================================================

FUNCTION tm_core_get_prtl_idx_by_tr_id

DESCRIPTION
  This function is to get protocol array index by transaction ID

DEPENDENCIES

RETURN VALUE
  -1, if transaction ID mask can not be found.
  Otherwise, protocol array index for the transaction ID

SIDE EFFECTS

===========================================================================*/
int tm_core_get_prtl_idx_by_tr_id(uint32 transaction_id)
{
  switch (transaction_id & TM_CORE_TRANSACTION_ID_DISPATCH_MASK)
  {
    case TM_1X_UP_TRANSACTION_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_1X_UP;

    case TM_UMTS_UP_TRANSACTION_ID_RANGE:
      return TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP;

    default:
      return -1;
  }
}

/*===========================================================================

FUNCTION tm_core_pdcomm_evt_handler

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_pdcomm_evt_handler(uint32 msg_id, void *p_Data)
{
  uint32  *tr_id_ptr;
  int prtlTableIndex;

  if (p_Data == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null p_Data", 0, 0, 0);
    return;
  }

  tr_id_ptr = (uint32 *)p_Data;

  prtlTableIndex = tm_core_get_prtl_idx_by_tr_id(*tr_id_ptr);

  if ((prtlTableIndex >= 0) && (prtlTableIndex < TM_PRTL_NUM))
  {
#ifdef FEATURE_CGPS_PDCOMM
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].data_pipe_handle_fp != NULL)
    {
      (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].data_pipe_handle_fp(
         (pd_comms_client_ipc_message_type)(msg_id - TM_CORE_PDCOMM_MSG_ID_BASE),
         p_Data);
    }
#endif
  }
  return;
}

/*===========================================================================

FUNCTION tm_core_get_prtl_idx_by_gm_tr_id

DESCRIPTION
  This function is to get protocol array index by transaction ID

DEPENDENCIES

RETURN VALUE
  -1, if transaction ID mask can not be found.
  Otherwise, protocol array index for the transaction ID

SIDE EFFECTS

===========================================================================*/
int tm_core_get_prtl_idx_by_gm_tr_id(uint32 transaction_id)
{
  switch (transaction_id & TM_CORE_TRANSACTION_ID_DISPATCH_MASK)
  {
    case TM_UMTS_UP_GM_TRANSACTION_ID_BASE:
      return TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP;
    case TM_GFC_QMI_GM_TRANSACTION_ID_BASE:
      return TM_CORE_PRTL_CB_ARR_INDEX_GFC_QMI;
#ifdef FEATURE_CGPS_XTRA_T
    case TM_XTRA_T_GM_TRANSACTION_ID_BASE:
      return TM_CORE_PRTL_CB_ARR_INDEX_XTRA_T;
#endif
    case TM_CSM_GM_TRANSACTION_ID_BASE:
      return TM_CORE_PRTL_CB_ARR_INDEX_CSM;
    default:
      return -1;
  }
}

/*===========================================================================

FUNCTION tm_core_gm_evt_handler

DESCRIPTION

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_gm_evt_handler(uint32 msg_id, void *p_Data)
{
  uint32  *tr_id_ptr;
  int prtlTableIndex;

  if (p_Data == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null p_Data", 0, 0, 0);
    return;
  }

  tr_id_ptr = (uint32 *)p_Data;

  prtlTableIndex = tm_core_get_prtl_idx_by_gm_tr_id(*tr_id_ptr);

  if ((prtlTableIndex >= 0) && (prtlTableIndex < TM_PRTL_NUM))
  {
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].gm_event_handle_fp != NULL)
    {
      (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].gm_event_handle_fp(
         (uint32)(msg_id - TM_CORE_GM_MSG_ID_BASE),
         p_Data);
    }
  }
  return;
}

/*===========================================================================

FUNCTION tm_core_cm_broadcast_event

DESCRIPTION
  This function is called by TM-CM module to inform TM-Core phone events

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_cm_broadcast_event
(
   uint32                        prtl_mask,
   prtl_event_u_type            *prtl_event_payload_ptr
   )
{
  int i;
  uint32 prtl_type = 0;
  int prtlTableIndex;

  if (prtl_event_payload_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "prtl_event_payload_ptr NULL", 0, 0, 0);
    return;
  }

  for (i  = 0; i < TM_PRTL_NUM; i++)
  {
    prtl_type = prtl_mask & (1 << i);

    if (prtl_type)
    {
      prtlTableIndex = tm_core_get_prtl_idx_by_type(prtl_type);

      if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
      {
        if (tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp != NULL)
        {
          (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp(
             PRTL_EVENT_TYPE_PHONE,
             prtl_event_payload_ptr);
        }
      }
    }
  }
}

/*===========================================================================

FUNCTION tm_core_cm_notify_event

DESCRIPTION
  This function is called by TM-CM module to inform TM-Core phone events

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_cm_notify_event
(
   tm_phone_event_e_type          phone_event_type,
   tm_phone_event_payload_u_type  *evt_payload_ptr
   )
{
  boolean prtl_abort_session = TRUE;
  int prtlTableIndex = -1;
  prtl_event_u_type  *event_payload_ptr = NULL;
  ldm_client_id_e_type e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type e_secondary_client_id = LDM_CLIENT_ID_MAX;
  sm_gm_srv_sys_notification_type z_gm_srv_sys_notify;
  tm_cm_phone_state_info_s_type phone_state_info;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "CM event %d", phone_event_type, 0, 0);
  event_payload_ptr = (prtl_event_u_type *)pdsm_getbuf(sizeof(prtl_event_u_type));
  if (event_payload_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cant get buffer", 0, 0, 0);
    return;
  }

  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

  switch (phone_event_type)
  {
    case TM_PHONE_EVENT_E911_CALL_ORIG:

      if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
      {
        /* Emergency call!! Stop all ongoing GPS Sessions */
        tm_core_abort_recover(TRUE, TM_STOP_REASON_E911); /*lint !e506 !e730 */
      }

      event_payload_ptr->phone_event_payload.event = phone_event_type;

      if (evt_payload_ptr)
      {
        event_payload_ptr->phone_event_payload.payload.emerg_call_info.emerg_call_asid = evt_payload_ptr->emerg_call_info.emerg_call_asid;
        event_payload_ptr->phone_event_payload.payload.emerg_call_info.emerg_call_sys_mode = evt_payload_ptr->emerg_call_info.emerg_call_sys_mode;
      }
      tm_core_report_911_state_to_techsel();
      tm_core_cm_broadcast_event(TM_PRTL_TYPE_ALL, event_payload_ptr);

#if !defined ( FEATURE_GNSS_SA ) && !defined ( FEATURE_GNSS_LEAN_BUILD )
      /* Request CPI injection on 911 ORIG.
         If SUPL WIFI is enabled, request WIFI injection as well*/
      if ((TM_CM_E911_STATE_ORIG == phone_state_info.e911_state) ||
          (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state))
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Start CPI.E911 ORIG");
        tm_startCPIRequest(TRUE, TM_AUXTECH_REQ_ID_EMERGENCY_ORIG);

        if (0 != TM_CORE_FEATURE_BIT_IS_SET(TM_SUPL_VOWIFI_EMERGENCY_ENABLE))
        {
          /* get Emergency data call APN information from DS*/
          tm_ds_get_emergency_sys_info();
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Start SUPL WIFI Scan.E911 ORIG");
          /*start ondemand wifi meas injection*/
          tm_reqOnDemandWifiScan(FALSE, TM_AUXTECH_REQ_ID_SUPLWLAN, PDSM_CLIENT_TYPE_NI);
        }
      }
#endif /* ! FEATURE_GNSS_LEAN_BUILD */

      break;

    case TM_PHONE_EVENT_E911_CALL_END:
      event_payload_ptr->phone_event_payload.event = phone_event_type;
      if (NULL != evt_payload_ptr)
      {
        event_payload_ptr->phone_event_payload.payload.emerg_call_info.emerg_call_asid = evt_payload_ptr->emerg_call_info.emerg_call_asid;
        event_payload_ptr->phone_event_payload.payload.emerg_call_info.emerg_call_sys_mode = evt_payload_ptr->emerg_call_info.emerg_call_sys_mode;
      }
      (void)tm_core_stop_emergency_session();
      tm_core_cm_broadcast_event(TM_PRTL_TYPE_ALL, event_payload_ptr);
      break;

    case TM_PHONE_EVENT_IN_1X_COVERAGE:
#ifdef FEATURE_CGPS_UMTS_CP
      if (NULL != evt_payload_ptr)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Sys_mode: %d, Prev_sys_mode: %d", evt_payload_ptr->ss_info.prev_sys_mode, evt_payload_ptr->ss_info.sys_mode, 0);
        if ((evt_payload_ptr->ss_info.prev_sys_mode == SYS_SYS_MODE_LTE) && (evt_payload_ptr->ss_info.sys_mode == SYS_SYS_MODE_CDMA))
        {
          tm_cm_phone_state_info_s_type phone_state_info;
          /* Query TM-CM module to get current acquired system */
          tm_cm_iface_get_phone_state(&phone_state_info);

          if (phone_state_info.e911_state != TM_CM_E911_STATE_CONCURRENT)
          {
            if ((tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911) ||
                (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_LPP_CP_NI) ||
                (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_UP_NI))
            {
              /* if we are going to CSFB, cancel the ongoing fake emergency session */
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "RAT Change from LTE to 1X. Stop ongoing session.", 0, 0, 0);
              tm_core_abort_recover(TRUE, TM_STOP_REASON_SYS_MODE_CHANGE); /*lint !e506 !e730 */
            }
          }
          else /* If in concurrent mode */
          {
            /* If we moved from LTE to CDMA while in E911 concurrent state,
             * tear down any concurrent apptrack,
             * reset the tm_cm_iface e911 state to what it was before */

            MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "RAT Change from LTE to 1X. Stop ongoing concurrent sessions");
            tm_core_abort_concurrent_sessions(TM_STOP_REASON_SYS_MODE_CHANGE);
          }
        }
      }
#endif /* FEATURE_CGPS_UMTS_CP */
      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event   = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }

      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }

#ifdef FEATURE_GNSS_PERIODIC_PPM
      #error code not present
#endif
      break;

    case TM_PHONE_EVENT_TC_UP:
    case TM_PHONE_EVENT_TC_EXIT:
    case TM_PHONE_EVENT_NEW_SS_INFO:
      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event   = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;

    case TM_PHONE_EVENT_IN_UMTS_COVERAGE:
    case TM_PHONE_EVENT_IN_TDS_COVERAGE:
#ifdef FEATURE_CGPS_UMTS_CP
      if (NULL != evt_payload_ptr)
      {
        tm_cm_phone_state_info_s_type phone_state_info;
        /* Query TM-CM module to get current acquired system */
        tm_cm_iface_get_phone_state(&phone_state_info);

        MSG_4(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM Prev_sys_mode: %u, Sys_mode: %u active_sess %d before_oos %d",
              evt_payload_ptr->ss_info.prev_sys_mode,
              evt_payload_ptr->ss_info.sys_mode,
              tm_core_info.session_info.active_sess_handle,
              tm_core_info.session_info.e_srv_system_before_oos);


        /* If we transition from (LTE -> G/W/T OR LTE -> OOS -> G/W/T) AND 
         * E911 early start/LPP CP/an E911 SUPL NI session is ON
         * Kill that session. We only want to kill E911 SUPL NI and not non-E911 NI because if
         * TCP conn is still up, we can continue that session */
        if (
            ((evt_payload_ptr->ss_info.prev_sys_mode == SYS_SYS_MODE_LTE)
             || ((evt_payload_ptr->ss_info.prev_sys_mode == SYS_SYS_MODE_NO_SRV) &&
                 (tm_core_info.session_info.e_srv_system_before_oos == CGPS_SRV_SYS_LTE)))

            && ((evt_payload_ptr->ss_info.sys_mode == SYS_SYS_MODE_GSM) ||
                (evt_payload_ptr->ss_info.sys_mode == SYS_SYS_MODE_WCDMA) ||
                (evt_payload_ptr->ss_info.sys_mode == SYS_SYS_MODE_TDS)
#ifdef FEATURE_NR5G
                || (evt_payload_ptr->ss_info.sys_mode == SYS_SYS_MODE_NR5G)
#endif /* FEATURE_NR5G */
                )
            )

        {
          if ((tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911) ||
              (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_LPP_CP_NI) ||
              ((phone_state_info.e911_state != TM_CM_E911_STATE_NONE) &&
               (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_UP_NI)))
          {
            if (TM_CM_E911_STATE_CONCURRENT != phone_state_info.e911_state)
            {
              /* Abort ongoing LPP CP MTLR, E911 early start for LTE If Rat changes from LTE to W.
               * Abort ongoing UP NILR & E911, if Rat changes from LTE to W */
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "RAT Change from LTE to W/G. Stop ongoing session.");
              tm_core_abort_recover(TRUE, TM_STOP_REASON_LTE_TO_UMTS_CSFB); /*lint !e506 !e730 */

			  /*CR3142996 :In case of UMTS CSFB,Early rcvr session/prtl ession would be aborted but this wont disable the main client in lpc.
			  If no new session is triggered on W (For eg:W disabled in 1920) and if a new apptrack session session is initiated ,
			  it will join as secondary client and wont trigger TM session and may fail to get a fix as no client is running the TM session.
			  So explicitily disabling the main client so that when Apptrack starts it will start the session as main client. */
			    
			  tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);
			  (void)tm_lpc_disable_client(e_main_client_id);
            }
            else
            {
              /* If we moved from LTE to WCDMA while in E911 concurrent state, 
               * tear down any concurrent apptrack, 
               * reset the tm_cm_iface e911 state to what it was before */
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "RAT Change from LTE to W/G. Stop ongoing concurrent sessions");
              tm_core_abort_concurrent_sessions(TM_STOP_REASON_LTE_TO_UMTS_CSFB);
            }
          }

          if (phone_state_info.e911_state != TM_CM_E911_STATE_NONE)
          {
            /* Send an event to do an E911 early session start during CSFB for UMTS*/
            if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp != NULL)
            {
              MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Sending early session start during CSFB from L to W");
              event_payload_ptr->phone_event_payload.event = phone_event_type;
              (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(
                 PRTL_EVENT_TYPE_PHONE,
                 event_payload_ptr);
            }
          }
        }
      }
#endif /*FEATURE_CGPS_UMTS_CP*/
      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }

      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;


    case TM_PHONE_EVENT_TC_CONNECTED:
    case TM_PHONE_EVENT_TC_DISCONNECTED:
      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_CP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_CP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
#ifdef FEATURE_GNSS_PERIODIC_PPM
      #error code not present
#endif

      break;

    case TM_PHONE_EVENT_MT_MSG:
      if ((NULL != evt_payload_ptr) &&
          (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_CP].event_cb_fp != NULL))
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        event_payload_ptr->phone_event_payload.payload.dbm_msg.length = evt_payload_ptr->dbm_msg.length;
        memscpy((void *)(event_payload_ptr->phone_event_payload.payload.dbm_msg.payload),
                sizeof(event_payload_ptr->phone_event_payload.payload.dbm_msg.payload),
                (const void *)evt_payload_ptr->dbm_msg.payload, evt_payload_ptr->dbm_msg.length);
        /* Copy Sub ID while calling the 1x CP event callback function. */
        event_payload_ptr->phone_event_payload.payload.dbm_msg.as_id = evt_payload_ptr->dbm_msg.as_id;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_CP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;

    case TM_PHONE_EVENT_SENT_MSG_STATUS:
      if ((NULL != evt_payload_ptr) &&
          (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_CP].event_cb_fp != NULL))
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        event_payload_ptr->phone_event_payload.payload.sent_msg_info.msg_status = evt_payload_ptr->sent_msg_info.msg_status;
        event_payload_ptr->phone_event_payload.payload.sent_msg_info.user_data  = evt_payload_ptr->sent_msg_info.user_data;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_CP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;

    case TM_PHONE_EVENT_SS_SRV_STATUS:
    case TM_PHONE_EVENT_SS_SRV_DOMAIN:
      if ((NULL != evt_payload_ptr) &&
          (phone_event_type == TM_PHONE_EVENT_SS_SRV_STATUS))
      {
        /* For the following service transitions abort a currently ongoing UMTS CP session */
        /* Valid transitions are: From G to W,OOS,L or From W to G,L*/
        if ((evt_payload_ptr->ss_info.prev_sys_mode != evt_payload_ptr->ss_info.sys_mode) &&
            ((SYS_SYS_MODE_GSM == evt_payload_ptr->ss_info.prev_sys_mode) ||
             ((SYS_SYS_MODE_WCDMA == evt_payload_ptr->ss_info.prev_sys_mode) && (SYS_SYS_MODE_NO_SRV != evt_payload_ptr->ss_info.sys_mode))
             )
            )
        {
          if (tm_core_session_active())
          {
            prtlTableIndex = tm_core_get_prtl_idx_by_type(tm_core_info.session_info.from_protocol);

            if ((prtlTableIndex > -1) &&
                (prtlTableIndex < TM_PRTL_NUM) &&
                (prtlTableIndex == TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP))
            {
              if (tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
              {
                MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "sys_mode change from %d to %d. Abort ongoing UMTS CP assisted sess",
                      evt_payload_ptr->ss_info.prev_sys_mode, evt_payload_ptr->ss_info.sys_mode, 0);

                prtl_abort_session = tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp(
                   tm_core_info.session_info.active_sess_handle,
                   PRTL_SESS_STOP_SINGLE_FIX,
                   TM_STOP_REASON_SYS_MODE_CHANGE);
                if (prtl_abort_session == TRUE)
                {
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "UMTS CP sess aborted due to sys_mode chg", 0, 0, 0);
                }
                else
                {
                  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "UMTS CP sess could not abort", 0, 0, 0);
                }
              }
            }
          }
        }
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "UMTS CP sess not aborted if sys_mode chg to OOS %d", evt_payload_ptr->ss_info.sys_mode);
        // Update the SRV system changed information to GM module
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Serving system %d update to GM", evt_payload_ptr->ss_info.sys_mode, 0, 0);
        z_gm_srv_sys_notify.e_curr_srv_sys = evt_payload_ptr->ss_info.sys_mode;
        gm_srv_sys_update(&z_gm_srv_sys_notify);
      }

      if ((NULL != evt_payload_ptr) &&
          (phone_event_type == TM_PHONE_EVENT_SS_SRV_STATUS) &&
          (SYS_SYS_MODE_WLAN == evt_payload_ptr->ss_info.sys_mode) &&
          (phone_state_info.e911_state != TM_CM_E911_STATE_NONE))
      {
        if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp != NULL)
        {
          event_payload_ptr->phone_event_payload.event = phone_event_type;
          memscpy((void *)&(event_payload_ptr->phone_event_payload.payload.ss_info),
                  sizeof(event_payload_ptr->phone_event_payload.payload.ss_info),
                  (const void *)&(evt_payload_ptr->ss_info), sizeof(evt_payload_ptr->ss_info));
          (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp(
             PRTL_EVENT_TYPE_PHONE,
             event_payload_ptr);
        }
      }
      else if ((NULL != evt_payload_ptr) &&
               (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp != NULL))
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        memscpy((void *)&(event_payload_ptr->phone_event_payload.payload.ss_info),
                sizeof(event_payload_ptr->phone_event_payload.payload.ss_info),
                (const void *)&(evt_payload_ptr->ss_info), sizeof(evt_payload_ptr->ss_info));
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;

#ifdef FEATURE_CGPS_UMTS_CP
    case TM_PHONE_EVENT_LCS_MOLR:
    case TM_PHONE_EVENT_LCS_MOLR_COMPLETED:
    case TM_PHONE_EVENT_LCS_MTLR_NTFY_RES:
      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;

    case TM_PHONE_EVENT_LCS_MTLR_NTFY_IND:
      if ((NULL != evt_payload_ptr) &&
          (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp != NULL))
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        memscpy((void *)&(event_payload_ptr->phone_event_payload.payload.sups_data),
                sizeof(event_payload_ptr->phone_event_payload.payload.sups_data),
                (const void *)&(evt_payload_ptr->sups_data), sizeof(evt_payload_ptr->sups_data));
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;

    case TM_PHONE_EVENT_LCS_MOLR_CONF:
      if ((NULL != evt_payload_ptr) &&
          (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp != NULL))
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        memscpy((void *)&(event_payload_ptr->phone_event_payload.payload.sups_data),
                sizeof(event_payload_ptr->phone_event_payload.payload.sups_data),
                (const void *)&(evt_payload_ptr->sups_data), sizeof(evt_payload_ptr->sups_data));
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;
#endif /* FEATURE_CGPS_UMTS_CP */
    case TM_PHONE_EVENT_OOS:
    case TM_PHONE_EVENT_SYS_MODE_LPM:
      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
#ifdef FEATURE_GNSS_PERIODIC_PPM
      #error code not present
#endif /* FEATURE_GNSS_PERIODIC_PPM */
      tm_core_info.session_info.e_srv_system_before_oos = tm_cm_iface_get_prev_srv_system();

      {
        cgps_CellInfoCachedStruct cell_db_data;

        memset((void *)&cell_db_data, 0, sizeof(cell_db_data));

        cell_db_data.e_ActiveRAT = CELLDB_ACTIVERAT_NONE;

        /*Send the update to Geofencing module*/
        (void)gm_cell_db_update(CELLDB_ACTIVERAT_NONE, &cell_db_data);


        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Serving system %d update to GM", SYS_SYS_MODE_NO_SRV, 0, 0);
        z_gm_srv_sys_notify.e_curr_srv_sys = SYS_SYS_MODE_NO_SRV;
        gm_srv_sys_update(&z_gm_srv_sys_notify);
      }
      break;



    case TM_PHONE_EVENT_DSDS_SUBS_CHANGED:
      if (tm_core_session_active())
      {
        // If subscription changed, end the current session.
        prtlTableIndex = tm_core_get_prtl_idx_by_type(tm_core_info.session_info.from_protocol);

        if ((prtlTableIndex > -1) &&
            (prtlTableIndex < TM_PRTL_NUM) &&
            (prtlTableIndex == TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP))
        {
          if (tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
          {
#ifdef FEATURE_CGPS_UMTS_CP
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "DSDS Subscription changed. Stop UP Session", 0, 0, 0);
#endif

            prtl_abort_session = tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp(
               tm_core_info.session_info.active_sess_handle,
               PRTL_SESS_STOP_SINGLE_FIX,
               TM_STOP_REASON_SYS_MODE_CHANGE);
            if (prtl_abort_session == TRUE)
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "UMTS UP sess aborted due to dsds subscription change", 0, 0, 0);
            }
            else
            {
              MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "UMTS UP sess could not abort", 0, 0, 0);
            }
          }
        }
      }
      break;


    case TM_PHONE_EVENT_OTASP_STATUS_COMMITTED:
      /* Send OTASP Status Committed event to 1X UP for potential MSID update */
      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].event_cb_fp != NULL)
      {
        event_payload_ptr->phone_event_payload.event   = phone_event_type;
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_1X_UP].event_cb_fp(
           PRTL_EVENT_TYPE_PHONE,
           event_payload_ptr);
      }
      break;

    case TM_PHONE_EVENT_HO_COMPLETE:

      if ((tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_E911) ||
          (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_1X_CP_MT) ||
          (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_CP_NI) ||
          (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_LPP_CP_NI) ||
          (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_UMTS_UP_NI))
      {
        /* if we are going to CSFB, cancel the ongoing fake emergency session */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SRVCC handover. Stop ongoing session.", 0, 0, 0);
        tm_core_abort_recover(TRUE, TM_STOP_REASON_SYS_MODE_CHANGE); /*lint !e506 !e730 */
      }
      break;

    case TM_PHONE_EVENT_E911_CALL_CONNECT:
      /* sys mode stored in the cached Emergency call info is different from that pointed in the CM event, end early rx start
         and start on correct RAT*/
      if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Emergency call connected");
        if (TRUE == tm_core_stop_emergency_session())
        {
          event_payload_ptr->phone_event_payload.event = phone_event_type;

          /*Start a new early rcvr on session*/
          tm_core_cm_broadcast_event(TM_PRTL_TYPE_ALL, event_payload_ptr);

          if (CGPS_SRV_SYS_LTE == phone_state_info.emerg_srv_system)
          {
            /* if we are on LTE, start a TM Core session to turn on rcvr early */
            (void)tm_core_start_emergency_session();
          }
        }
        else
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Early RX on session not in progress");
        }
      }
      break;

    default:
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Phone event %d not handled", phone_event_type, 0, 0);
      break;
  }
  (void)pdsm_freebuf((char *)event_payload_ptr);
}


/*===========================================================================

FUNCTION tm_core_rr_notify_event

DESCRIPTION
  This function is called by TM-RR IFACE to inform TM-Core of RR events

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_rr_notify_event(prtl_rr_event_s_type *p_event)
{
  (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(
     PRTL_EVENT_TYPE_RR,
     (prtl_event_u_type *)p_event);
}

/*===========================================================================

FUNCTION tm_core_rrc_notify_event

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming RRC message.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void tm_core_rrc_notify_event(prtl_rrc_event_s_type *p_event)
{

  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(
       PRTL_EVENT_TYPE_RRC,
       (prtl_event_u_type *)p_event);
  }

}


/*===========================================================================

FUNCTION tm_core_mm_notify_event

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming MM message.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void tm_core_mm_notify_event(prtl_mm_event_s_type  *p_event)
{

  if (NULL == p_event)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer passed to tm_core_mm_notify_event");
    return;
  }

#if !defined(FEATURE_GNSS_SA)
  if (MM_CGPS_MSG_EVENT_RESET_UE_POS_INFO ==
         ((prtl_event_u_type *)p_event)->mm_event_payload.mm_event_data_ptr->e_event_type)
  {
    /* LCS is supported only in single SIM mode. In DSDS mode, CP session is possible only on Sub1.
       In any case it can be understood to be the DD Sub whether in single SIM mode or DSDS. */
    tm_handle_ota_reset(PRTL_EVENT_TYPE_UMTS_OTA_GPS_RESET, tm_cm_dsds_get_dd_subs());
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "OTA delete assist. processed");
    return; /* The protocol func cb for UMTS UP and CP are already handled */
  }

  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(
       PRTL_EVENT_TYPE_MM,
       (prtl_event_u_type *)p_event);
  }

  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
       PRTL_EVENT_TYPE_MM,
       (prtl_event_u_type *)p_event);
  }

#endif // if!defined(FEATURE_GNSS_SA)

}


/*===========================================================================

FUNCTION tm_core_ota_reset_notify_event

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of either
  an incoming OTA GPS reset OR OTDOA reset message. 

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_ota_reset_notify_event(prtl_event_e_type const prtlEvent)
{

  if ((PRTL_EVENT_TYPE_LTE_OTA_GPS_RESET != prtlEvent) &&
      (PRTL_EVENT_TYPE_LTE_OTA_OTDOA_RESET != prtlEvent) &&
      (PRTL_EVENT_TYPE_UMTS_OTA_GPS_RESET != prtlEvent))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unsupported event %d for OTA RESET", prtlEvent);
    return;
  }

  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(prtlEvent, NULL);
  }

  if ((PRTL_EVENT_TYPE_LTE_OTA_GPS_RESET == prtlEvent) ||
      (PRTL_EVENT_TYPE_LTE_OTA_OTDOA_RESET == prtlEvent))
  {
    if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp != NULL)
    {
      (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp(prtlEvent, NULL);
    }
  }
  else if (PRTL_EVENT_TYPE_UMTS_OTA_GPS_RESET == prtlEvent)
  {
    if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp != NULL)
    {
      (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_CP].event_cb_fp(prtlEvent, NULL);
    }
  }
}


/*===========================================================================

FUNCTION tm_core_cell_db_proc

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming cell database update message.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void  tm_core_cell_db_proc(void *cell_db_info_ptr)
{
  prtl_event_u_type  *event_payload_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM got CellDB update", 0, 0, 0);

  /* allocate buffer for sending event to protocol submodule */
  event_payload_ptr = (prtl_event_u_type *)pdsm_getbuf(sizeof(prtl_event_u_type));
  if (event_payload_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can't get buffer", 0, 0, 0);
    return;
  }

  event_payload_ptr->cell_db_event_payload.cell_db_event = PRTL_MAIN_MODEM_CELL_DB_UPD;
  event_payload_ptr->cell_db_event_payload.cell_db_event_payload.main_modem_cell_db_payload.cell_db_event_data_ptr =
     &(((tm_cell_info_update_type *)cell_db_info_ptr)->cellInfoCached);
  event_payload_ptr->cell_db_event_payload.cell_db_event_payload.main_modem_cell_db_payload.e_TriggerRAT =
     (((tm_cell_info_update_type *)cell_db_info_ptr)->e_TriggerRAT);


  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
       PRTL_EVENT_TYPE_CELL_DB,
       event_payload_ptr);
  }
  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp(
       PRTL_EVENT_TYPE_CELL_DB,
       event_payload_ptr);
  }

  /*Send the CellDB update to Geofencing module*/
  (void)gm_cell_db_update(
     event_payload_ptr->cell_db_event_payload.cell_db_event_payload.main_modem_cell_db_payload.e_TriggerRAT,
     event_payload_ptr->cell_db_event_payload.cell_db_event_payload.main_modem_cell_db_payload.cell_db_event_data_ptr);

  /*Send the CellDB update to Fusion CSM module*/
  (void)fusion_csm_CellDbUpdate(
     event_payload_ptr->cell_db_event_payload.cell_db_event_payload.main_modem_cell_db_payload.e_TriggerRAT,
     event_payload_ptr->cell_db_event_payload.cell_db_event_payload.main_modem_cell_db_payload.cell_db_event_data_ptr);

  (void)pdsm_freebuf((char *)event_payload_ptr);

}


/*===========================================================================

FUNCTION tm_core_lpp_cp_evt_handler

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming LPP Control Plane Protocol events.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_lpp_cp_evt_handler(uint32 msg_id, void *lpp_cp_evt_info_ptr)
{
  prtl_event_u_type  *event_payload_ptr = NULL;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM got LPP CP Protocol event update", 0, 0, 0);

  /* allocate buffer for sending event to protocol submodule */
  event_payload_ptr = (prtl_event_u_type *)pdsm_getbuf(sizeof(prtl_event_u_type));
  if (event_payload_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can't get buffer", 0, 0, 0);
    return;
  }

  // copy the LPP CP event payload (carried in IPC payload)
  memscpy((void *)&event_payload_ptr->lpp_cp_event_payload.lpp_cp_event_data,
          sizeof(event_payload_ptr->lpp_cp_event_payload.lpp_cp_event_data),
          (void *)lpp_cp_evt_info_ptr,
          sizeof(event_payload_ptr->lpp_cp_event_payload.lpp_cp_event_data));
  event_payload_ptr->lpp_cp_event_payload.lpp_cp_event_id =
     (prtl_lpp_cp_event_e_type)(msg_id - TM_CORE_MSG_ID_NAS_EVENT_BASE);

  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp(
       PRTL_EVENT_TYPE_LPP_CP,
       event_payload_ptr);
  }

  (void)pdsm_freebuf((char *)event_payload_ptr);

} /* tm_core_lpp_cp_evt_handler */
/*===========================================================================

FUNCTION tm_core_l1_notify_event

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming message from L1.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void tm_core_l1_notify_event(prtl_l1_event_s_type *p_event)
{

  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
       PRTL_EVENT_TYPE_L1,
       (prtl_event_u_type *)p_event);
  }
#ifdef FEATURE_CGPS_UMTS_CELLDB
#ifdef FEATURE_CGPS_WCDMA
  /* Send the WCDMA cell meas to GM module */
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL: Send WCDMA cell meas to GM", 0, 0, 0);
  gm_wcdma_cell_meas_update(((prtl_event_u_type *)p_event)->l1_event_payload.l1_event_data_ptr->u.l1_cell_meas_update_ptr);
#endif /* FEATURE_CGPS_WCDMA */
#endif /* FEATURE_CGPS_UMTS_CELLDB */

}


/*===========================================================================

FUNCTION tm_core_lte_l1_notify_event

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of
  an incoming message from LTE L1.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

void tm_core_lte_l1_notify_event(prtl_lte_l1_event_s_type *p_event)
{

  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
       PRTL_EVENT_TYPE_LTE_L1,
       (prtl_event_u_type *)p_event);
  }
#ifdef FEATURE_CGPS_LTE_CELLDB
  /* Send the LTE cell meas to GM module */
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "SUPL: Send LTE cell meas to GM", 0, 0, 0);
  gm_lte_cell_meas_update(((prtl_event_u_type *)p_event)->lte_l1_event_payload.l1_event_data_ptr->u.l1_cell_meas_update_ptr);
#endif
}



/*===========================================================================

FUNCTION tm_core_get_last_position

DESCRIPTION
  This function is called by TmCore to return a cached position

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_last_position(tm_core_cached_pos_s_type *pd_cached_pos)
{
  if (z_cached_pos_info.cached_pos_valid == TRUE)
  {
    memscpy(pd_cached_pos, sizeof(*pd_cached_pos), &z_cached_pos_info, sizeof(z_cached_pos_info));
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================

FUNCTION tm_core_xlate_gps_state_to_ext_status

DESCRIPTION
  This function is called by TmCore to translate a gps state information
to external status

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void  tm_core_xlate_gps_state_to_ext_status(tm_core_gps_state_info_s_type *p_gps_state_info,
                                            pdsm_pd_ext_gps_state_s_type *pd_gps_state_info)
{
  pdsm_xtra_download_info_s_type p_download_info;
  boolean xtra_valid;

  pd_gps_state_info->flags = p_gps_state_info->flags;
  pd_gps_state_info->lat = p_gps_state_info->lat;
  pd_gps_state_info->lon = p_gps_state_info->lon;
  pd_gps_state_info->altitude = p_gps_state_info->altitude;
  pd_gps_state_info->f_loc_unc_horizontal = p_gps_state_info->pos_unc;
  pd_gps_state_info->loc_unc_vertical = p_gps_state_info->loc_unc_vertical;

  pd_gps_state_info->timestamp_tow_ms = p_gps_state_info->timestamp_tow_ms;
  pd_gps_state_info->timestamp_gps_week = p_gps_state_info->timestamp_gps_week;  /*GPS week number */
  /*Time uncertainty in milli seconds, floor at 20 ms */
  if (p_gps_state_info->time_unc_us < 20000)
  {
    pd_gps_state_info->time_unc_ms = 20;
  }
  else
  {
    pd_gps_state_info->time_unc_ms = p_gps_state_info->time_unc_us / 1000;
  }
  pd_gps_state_info->engine_state = p_gps_state_info->engine_state; /*  GPS engine state on/off. always present, no flag */
  pd_gps_state_info->eph_svmask = p_gps_state_info->eph_svmask; /* SV Mask for the ephemeris */
  pd_gps_state_info->alm_svmask = p_gps_state_info->alm_svmask; /* SV Mask for the Almanac */
  pd_gps_state_info->iono_valid = p_gps_state_info->iono_valid; /* IONO */
  pd_gps_state_info->health_svmask = p_gps_state_info->health_svmask; /* SV mask for the health */
  pd_gps_state_info->visible_svmask = p_gps_state_info->visible_svmask; /* sv mask for visible SVs*/
  xtra_valid = tm_xtra_set_xtra_time_and_validity(&p_download_info);
  if (xtra_valid)
  {
    pd_gps_state_info->flags |= PDSM_PD_EXT_GPS_STATE_XTRA_VALID;
    pd_gps_state_info->xtra_start_gps_week = tm_core_info.gps_state_info.start_gps_week = p_download_info.startGpsWeek;
    pd_gps_state_info->xtra_start_gps_minutes = tm_core_info.gps_state_info.start_gps_minutes = p_download_info.startGpsMinutes;
    pd_gps_state_info->xtra_valid_duration_hours = tm_core_info.gps_state_info.valid_duration_hours =
       p_download_info.validDurationHours;
  }
}


/*===========================================================================

FUNCTION tm_core_xlate_pd_ext_pos_to_qmi

DESCRIPTION
  This function is called by TM Core to translate the SM position report
  structure to a QMI defined position report.
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
static void tm_core_xlate_pd_ext_pos_to_qmi
(
   const sm_FixInfoStructType *p_GnssFixRpt,
   pdsm_pd_ext_pos_rpt_s_type    *p_PdsmExtPos
   )
{


#ifndef FEATURE_GNSS_LEAN_BUILD

  double f_tmp = 0, f_vel_east = 0, f_vel_north = 0;
  uint32 i = 0, count = 0;
  float f_Punc;
  pdsm_utc_time_s_type z_pdsm_utc_time;

  /* UTC timestamp in millisecond since Jan 1st, 1970 */
  uint64 t_timestamp_utc_ms, t_timestamp_utc_s;

  f_Punc = tm_core_calculate_2d_punc(p_GnssFixRpt);
  memset(p_PdsmExtPos, 0, sizeof(pdsm_pd_ext_pos_rpt_s_type));
  if (p_GnssFixRpt->z_NavPos.z_PosFlags.b_IsValid)
  {
    p_PdsmExtPos->sessionStatus = eLOC_ENG_POS_SESS_STATUS_SUCCESS_V01;
    // Convert the latitude from radians into degrees.
    // radians = degrees * (PI/180)
    // degrees = (radians * 180)/PI

    /* Latitude */
    p_PdsmExtPos->latitude = (p_GnssFixRpt->z_NavPos.d_PosLla[0] * 180) / C_PI;
    p_PdsmExtPos->latitude_valid = TRUE;

    /*   Longitude */
    // Convert the longitude from radians into degrees.
    p_PdsmExtPos->longitude =  (p_GnssFixRpt->z_NavPos.d_PosLla[1] * 180) / C_PI;
    p_PdsmExtPos->longitude_valid = TRUE;

    /*   Circular Horizontal Position Uncertainty */
    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsPuncValid)
    {
      p_PdsmExtPos->horUncCircular = f_Punc;
      p_PdsmExtPos->horUncCircular_valid = TRUE;
    }

    /*  Horizontal Elliptical Uncertainty (Semi-Minor Axis) */
    p_PdsmExtPos->horUncEllipseSemiMajor = p_GnssFixRpt->z_NavPos.f_ErrorEllipse[2];
    p_PdsmExtPos->horUncEllipseSemiMajor_valid = TRUE;

    /*  Horizontal Elliptical Uncertainty (Semi-Major Axis) */
    p_PdsmExtPos->horUncEllipseSemiMinor = p_GnssFixRpt->z_NavPos.f_ErrorEllipse[1];
    p_PdsmExtPos->horUncEllipseSemiMinor_valid = TRUE;
    /*  Elliptical Horizontal Uncertainty Azimuth */
    p_PdsmExtPos->horUncEllipseOrientAzimuth = p_GnssFixRpt->z_NavPos.f_ErrorEllipse[0];
    p_PdsmExtPos->horUncEllipseOrientAzimuth_valid = TRUE;

    /*  Horizontal circular confidence */
    p_PdsmExtPos->horCircularConfidence = 63; // Hard coded as of now
    p_PdsmExtPos->horCircularConfidence_valid = TRUE;

    /*  Horizontal Elliptical Confidence */
    p_PdsmExtPos->horEllipticalConfidence = 39;
    p_PdsmExtPos->horEllipticalConfidence_valid = TRUE;

    /*  Horizontal Reliability */
    switch (p_GnssFixRpt->z_NavPos.e_HoriRelIndicator)
    {
      case PDSM_POSITION_RELIABILITY_NOT_SET:
        /* Not set, reliability info is not present */
        p_PdsmExtPos->horReliability = eLOC_ENG_RELIABILITY_NOT_SET_V01;
        break;

      case PDSM_POSITION_RELIABILITY_VERY_LOW:
        /* Not checked, use at user's own risk */
        p_PdsmExtPos->horReliability = eLOC_ENG_RELIABILITY_VERY_LOW_V01;
        break;

      case PDSM_POSITION_RELIABILITY_LOW:
        /* Limited cross-checked */
        p_PdsmExtPos->horReliability = eLOC_ENG_RELIABILITY_LOW_V01;
        break;

      case PDSM_POSITION_RELIABILITY_MEDIUM:
        /* Limited cross-checked */
        p_PdsmExtPos->horReliability = eLOC_ENG_RELIABILITY_MEDIUM_V01;
        break;

      case PDSM_POSITION_RELIABILITY_HIGH:
        /* Robust cross-checked */
        p_PdsmExtPos->horReliability = eLOC_ENG_RELIABILITY_HIGH_V01;
        break;

      default:
        /* Default reliability . Not set */
        p_PdsmExtPos->horReliability = eLOC_ENG_RELIABILITY_NOT_SET_V01;
        break;
    }
    p_PdsmExtPos->horReliability_valid = TRUE;

    /* Compute horizontal velocity from components and scale
    * to units of 0.25 m/s.
    */

    f_vel_east = p_GnssFixRpt->z_NavPos.f_VelEnu[0];
    f_vel_north = p_GnssFixRpt->z_NavPos.f_VelEnu[1];

    f_tmp = sqrt(f_vel_east * f_vel_east + f_vel_north * f_vel_north);

    if (f_tmp < TM_CORE_MIN_VELOCTY)
    {
      p_PdsmExtPos->horSpeed = 0;
    }
    else
    {
      p_PdsmExtPos->horSpeed =
         (float)tm_util_nearest_long(4.0 * f_tmp);
    }
    p_PdsmExtPos->horSpeed_valid = TRUE;

    /*  Horizontal Speed Uncertainty */
    p_PdsmExtPos->horSpeedUnc = (float)f_tmp;
    /* suds84. This is actually 3d uncertainty??*/
    p_PdsmExtPos->horSpeedUnc_valid = TRUE;

    /*  Altitude With Respect to Ellipsoid */
    p_PdsmExtPos->altitudeWrtEllipsoid = p_GnssFixRpt->z_NavPos.d_PosLla[2];
    p_PdsmExtPos->altitudeWrtEllipsoid_valid = TRUE;

    /*  Altitude With Respect to Sea Level */
    p_PdsmExtPos->altitudeWrtMeanSeaLevel = p_GnssFixRpt->z_NavPos.d_PosLla[2] + 500;
    p_PdsmExtPos->altitudeWrtMeanSeaLevel_valid = TRUE;

    /*  Vertical Uncertainty */
    p_PdsmExtPos->vertUnc = p_GnssFixRpt->z_NavPos.f_VuncVertMps;
    p_PdsmExtPos->vertUnc_valid = TRUE;

    /*  Vertical Confidence */
    p_PdsmExtPos->vertConfidence = 68;
    p_PdsmExtPos->vertConfidence_valid = TRUE;

    /*  Vertical Reliability */
    switch (p_GnssFixRpt->z_NavPos.e_VertRelIndicator)
    {
      case PDSM_POSITION_RELIABILITY_NOT_SET:
        /* Not set, reliability info is not present */
        p_PdsmExtPos->vertReliability = eLOC_ENG_RELIABILITY_NOT_SET_V01;
        break;

      case PDSM_POSITION_RELIABILITY_VERY_LOW:
        /* Not checked, use at user's own risk */
        p_PdsmExtPos->vertReliability = eLOC_ENG_RELIABILITY_VERY_LOW_V01;
        break;

      case PDSM_POSITION_RELIABILITY_LOW:
        /* Limited cross-checked */
        p_PdsmExtPos->vertReliability = eLOC_ENG_RELIABILITY_LOW_V01;
        break;

      case PDSM_POSITION_RELIABILITY_MEDIUM:
        /* Limited cross-checked */
        p_PdsmExtPos->vertReliability = eLOC_ENG_RELIABILITY_MEDIUM_V01;
        break;

      case PDSM_POSITION_RELIABILITY_HIGH:
        /* Robust cross-checked */
        p_PdsmExtPos->vertReliability = eLOC_ENG_RELIABILITY_HIGH_V01;
        break;

      default:
        /* Default reliability . Not set */
        p_PdsmExtPos->vertReliability = eLOC_ENG_RELIABILITY_NOT_SET_V01;
        break;
    }
    p_PdsmExtPos->vertReliability_valid = TRUE;

    /*  Vertical Speed */
    p_PdsmExtPos->vertSpeed = p_GnssFixRpt->z_NavPos.f_VelEnu[2];
    p_PdsmExtPos->vertSpeed_valid = TRUE;

    /*  Vertical Speed Uncertainty */
    p_PdsmExtPos->vertSpeedUnc = p_GnssFixRpt->z_NavPos.f_VuncVertMps;

    p_PdsmExtPos->vertSpeedUnc_valid = TRUE;

    /*  Heading */
    if (p_GnssFixRpt->z_NavPos.z_PosFlags.b_IsHeadingValid)
    {
      p_PdsmExtPos->heading = (p_GnssFixRpt->z_NavPos.f_HeadingRad * 180) / C_PI;
      p_PdsmExtPos->heading_valid = TRUE;
    }

    /*  Heading Uncertainty */
    p_PdsmExtPos->headingUnc = (p_GnssFixRpt->z_NavPos.f_HeadingUncRad * 180) / C_PI;
    p_PdsmExtPos->headingUnc_valid = TRUE;

    /*  Magnetic Deviation */
    p_PdsmExtPos->magneticDeviation = p_GnssFixRpt->z_NavPos.f_MagDeviation;
    p_PdsmExtPos->magneticDeviation_valid = TRUE;

    /*  Technology Used */
    p_PdsmExtPos->technologyMask_valid = TRUE;
    p_PdsmExtPos->technologyMask = 0;

    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsCellId)
    {
      p_PdsmExtPos->technologyMask |= LOC_ENG_POS_TECH_MASK_CELLID_V01;
    }

    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsWifi)
    {
      p_PdsmExtPos->technologyMask |= LOC_ENG_POS_TECH_MASK_WIFI_V01;
    }

    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsSensors)
    {
      p_PdsmExtPos->technologyMask |= LOC_ENG_POS_TECH_MASK_SENSORS_V01;
    }

    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsSatellite)
    {
      p_PdsmExtPos->technologyMask |= LOC_ENG_POS_TECH_MASK_SATELLITE_V01;
    }

    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsRefLoc)
    {
      p_PdsmExtPos->technologyMask |= LOC_ENG_POS_TECH_MASK_REFERENCE_LOCATION_V01;
    }

    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsCoarsePos)
    {
      p_PdsmExtPos->technologyMask |= LOC_ENG_POS_TECH_MASK_INJECTED_COARSE_POSITION_V01;
    }

    /*Set the technology valid mask to FALSE if we could not get the technology used*/
    if (p_PdsmExtPos->technologyMask == 0)
    {
      p_PdsmExtPos->technologyMask_valid = FALSE;
    }

    /* Position dilution of precision.*/
    p_PdsmExtPos->DOP.PDOP = p_GnssFixRpt->z_NavPos.f_PDOP;
    /* Horizontal dilution of precision.*/
    p_PdsmExtPos->DOP.HDOP = p_GnssFixRpt->z_NavPos.f_HDOP;
    /* Vertical dilution of precision. */
    p_PdsmExtPos->DOP.VDOP = p_GnssFixRpt->z_NavPos.f_VDOP;

    p_PdsmExtPos->DOP_valid = TRUE;

    /*  UTC Timestamp */
    if (tm_util_calculate_utc(p_GnssFixRpt, &z_pdsm_utc_time, NULL) == TRUE)
    {
      // We have the UTC time in YearMonthHourMinuteSecond calculated from GPS time ( Since Jan 6 1980 ) .
      // Convert the UTC time we have into UNIX timestamp ( Since Jan1 1970 )
      gnss_GetUnixEpochFromUTC(&z_pdsm_utc_time,
                               &t_timestamp_utc_s);
      /* Scale it to milliseconds, as the TimeStamp now is in seconds. */
      t_timestamp_utc_ms = t_timestamp_utc_s * 1000;
      /* Now lets add more precision.
         utc_time.sec --> is seconds scaled by 100.
                          ( ie. 1sec is reported as 100 &
                            1.99 sec is reported as 199 which is 1990msec)
         GetUnixEpochFromUTC gives us back in seconds. So we can add the tenths
         of a second and hundredth of a second.
   
         Note: Currently the precision of hundredth of a second is lost in
               translation in SM.
      */
      t_timestamp_utc_ms +=
         ((z_pdsm_utc_time.sec % 100) * 10);
      p_PdsmExtPos->timestampUtc = t_timestamp_utc_ms;
      p_PdsmExtPos->timestampUtc_valid = TRUE;
    }
    else
    {
      p_PdsmExtPos->timestampUtc_valid = FALSE;
    }

    /*  GPS Time */
    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsGpsTimeValid)
    {
      /* Week number of the fix*/
      p_PdsmExtPos->gpsTime.gpsWeek = p_GnssFixRpt->z_NavPos.w_GpsWeek;
      p_PdsmExtPos->gpsTime.gpsTimeOfWeekMs = p_GnssFixRpt->z_NavPos.q_GpsTimeMs;
      p_PdsmExtPos->gpsTime_valid = TRUE;
    }

    /*  Time Uncertainty */
    if (p_GnssFixRpt->z_NavPos.z_SmPosFlags.b_IsTuncValid)
    {
      p_PdsmExtPos->timeUnc = p_GnssFixRpt->z_NavPos.f_ClockBiasUncMs;
      p_PdsmExtPos->timeUnc_valid = TRUE;
    }

    /*  Time Source */
    /* Convert the time source which is of the type pdsm_extended_time_src_e_type into
       the type used by extended status report */
    switch (p_GnssFixRpt->z_NavPos.u_InitGpsTimeSrc)
    {
      case PDSM_GNSS_TIME_SRC_UNKNOWN:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_UNKNOWN_V01;
        break;
      case PDSM_GNSS_TIME_SRC_RESERVED:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_SYSTEM_TIMETICK_V01;
        break;
      case PDSM_GNSS_TIME_SRC_NETWORK_TIME_TRANSFER:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_NETWORK_TIME_TRANSFER_V01;
        break;
      case PDSM_GNSS_TIME_SRC_NETWORK_TIME_TAGGING:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_NETWORK_TIME_TAGGING_V01;
        break;
      case PDSM_GNSS_TIME_SRC_EXTERNAL_INPUT:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_EXTERNAL_INPUT_V01;
        break;
      case PDSM_GNSS_TIME_SRC_GPS_TOW_DECODE:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_TOW_DECODE_V01;
        break;
      case PDSM_GNSS_TIME_SRC_TOW_CONFIRMED:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_TOW_CONFIRMED_V01;
        break;
      case PDSM_GNSS_TIME_SRC_TOW_WEEK_CONFIRMED:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_TOW_AND_WEEK_CONFIRMED_V01;
        break;
      case PDSM_GNSS_TIME_SRC_NAV_SOLUTION:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_NAV_SOLUTION_V01;
        break;
      case PDSM_GNSS_TIME_SRC_SOLVE_FOR_TIME:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_SOLVE_FOR_TIME_V01;
        break;
      case PDSM_GNSS_TIME_SRC_GLO_TOW_DECODE:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_GLO_TOW_DECODE_V01;
        break;
      case PDSM_GNSS_TIME_SRC_TIME_TRANSFORM :
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_TIME_TRANSFORM_V01;
        break;
      case PDSM_GNSS_TIME_SRC_WCDMA_SLEEP_TIME_TAGGING:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_WCDMA_SLEEP_TIME_TAGGING_V01;
        break;
      case PDSM_GNSS_TIME_SRC_GSM_SLEEP_TIME_TAGGING:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_GSM_SLEEP_TIME_TAGGING_V01;
        break;
      case PDSM_GNSS_TIME_SRC_QZSS_TOW_DECODE:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_QZSS_TOW_DECODE_V01;
        break;
      case  PDSM_GNSS_TIME_SRC_BDS_TOW_DECODE:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_BDS_TOW_DECODE_V01;
        break;
#if 0 //QZSS Phase 2
      case  PDSM_GNSS_TIME_SRC_QZSS_TOW_DECODE:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_QZSS_TOW_DECODE_V01;
        break;
#endif
      case  PDSM_GNSS_TIME_SRC_GAL_TOW_DECODE:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_GAL_TOW_DECODE_V01;
        break;
      default:
        p_PdsmExtPos->timeSrc = eLOC_ENG_TIME_SRC_INVALID_V01;
        break;
    }
    if (p_PdsmExtPos->timeSrc != eLOC_ENG_TIME_SRC_INVALID_V01)
    {
      p_PdsmExtPos->timeSrc_valid = TRUE;
    }

    /*  Sensor Data Usage */
    p_PdsmExtPos->sensorDataUsage.usageMask = p_GnssFixRpt->z_NavPos.w_SensorDataUsageMask;
    p_PdsmExtPos->sensorDataUsage.aidingIndicatorMask = p_GnssFixRpt->z_NavPos.w_SensorAidingIndicator;
    p_PdsmExtPos->sensorDataUsage_valid = TRUE;

    /*  SV's Used to Calculate the Fix */
    //LOC_ENG_MAX_SV_USED_LIST_LENGTH_V01;

    count = 0;  /* up to 80 SVs allowed in QMI, won't overflow (16+14+16=46) */
    // Pack the SV used information ( GNSS and GLO SVs ) into the same array for the extended status callback
    // GPS SVs used information
    for (i = 0; i < ARR_SIZE(p_GnssFixRpt->z_NavSvInfo.z_GpsSvInfo); i++)
    {
      if ((p_GnssFixRpt->z_NavSvInfo.z_GpsSvInfo[i].q_SvInfoMask & (PDSM_GNSS_SV_INFO_USED_IN_POS_FIX | PDSM_GNSS_SV_INFO_USED_IN_VEL_FIX)) &&
          (count < LOC_ENG_MAX_SV_USED_LIST_LENGTH_V01))
      {
        p_PdsmExtPos->gnssSvUsedList[count++] = (uint16)p_GnssFixRpt->z_NavSvInfo.z_GpsSvInfo[i].w_SvId;
      }
    }
    // GLO SVs used information
    for (i = 0; i < ARR_SIZE(p_GnssFixRpt->z_NavSvInfo.z_GloSvInfo); i++)
    {
      if ((p_GnssFixRpt->z_NavSvInfo.z_GloSvInfo[i].q_SvInfoMask & (PDSM_GNSS_SV_INFO_USED_IN_POS_FIX | PDSM_GNSS_SV_INFO_USED_IN_VEL_FIX)) &&
          (count < LOC_ENG_MAX_SV_USED_LIST_LENGTH_V01))
      {
        p_PdsmExtPos->gnssSvUsedList[count++] = (uint16)p_GnssFixRpt->z_NavSvInfo.z_GloSvInfo[i].w_SvId;
      }
    }
    // BDS SVs used information
    for (i = 0; i < ARR_SIZE(p_GnssFixRpt->z_NavSvInfo.z_BdsSvInfo); i++)
    {
      if ((p_GnssFixRpt->z_NavSvInfo.z_BdsSvInfo[i].q_SvInfoMask & (PDSM_GNSS_SV_INFO_USED_IN_POS_FIX | PDSM_GNSS_SV_INFO_USED_IN_VEL_FIX)) &&
          (count < LOC_ENG_MAX_SV_USED_LIST_LENGTH_V01))
      {
        p_PdsmExtPos->gnssSvUsedList[count++] = (uint16)p_GnssFixRpt->z_NavSvInfo.z_BdsSvInfo[i].w_SvId;
      }
    }

    // QZSS SVs used information
    for (i = 0; i < ARR_SIZE(p_GnssFixRpt->z_NavSvInfo.z_QzssSvInfo); i++)
    {
      if ((p_GnssFixRpt->z_NavSvInfo.z_QzssSvInfo[i].q_SvInfoMask & (PDSM_GNSS_SV_INFO_USED_IN_POS_FIX | PDSM_GNSS_SV_INFO_USED_IN_VEL_FIX)) &&
          (count < LOC_ENG_MAX_SV_USED_LIST_LENGTH_V01))
      {
        p_PdsmExtPos->gnssSvUsedList[count++] = (uint16)p_GnssFixRpt->z_NavSvInfo.z_QzssSvInfo[i].w_SvId;
      }
    }

    // GAL SVs used information
    for (i = 0; i < ARR_SIZE(p_GnssFixRpt->z_NavSvInfo.z_GalSvInfo); i++)
    {
      if ((p_GnssFixRpt->z_NavSvInfo.z_GalSvInfo[i].q_SvInfoMask & (PDSM_GNSS_SV_INFO_USED_IN_POS_FIX | PDSM_GNSS_SV_INFO_USED_IN_VEL_FIX)) &&
          (count < LOC_ENG_MAX_SV_USED_LIST_LENGTH_V01))
      {
        p_PdsmExtPos->gnssSvUsedList[count++] = (uint16)p_GnssFixRpt->z_NavSvInfo.z_GalSvInfo[i].w_SvId;
      }
    }

    // Navic SVs used information
    for (i = 0; i < ARR_SIZE(p_GnssFixRpt->z_NavSvInfo.z_NavicSvInfo); i++)
    {
      if ((p_GnssFixRpt->z_NavSvInfo.z_NavicSvInfo[i].q_SvInfoMask & (PDSM_GNSS_SV_INFO_USED_IN_POS_FIX | PDSM_GNSS_SV_INFO_USED_IN_VEL_FIX)) &&
          (count < LOC_ENG_MAX_SV_USED_LIST_LENGTH_V01))
      {
        p_PdsmExtPos->gnssSvUsedList[count++] = (uint16)p_GnssFixRpt->z_NavSvInfo.z_NavicSvInfo[i].w_SvId;
      }
    }

    p_PdsmExtPos->gnssSvUsedList_len = count;
    p_PdsmExtPos->gnssSvUsedList_valid = TRUE;

    p_PdsmExtPos->bap_spoof_report = sdm_GetSpoofReport().spoof_type_mask;

    /*No active session while processing get_best_pos. Clear Report*/
    if (FALSE == tm_core_session_active())
    {
      sdm_ClearSpoofReport();
    }
  } // end of if (p_GnssFixRpt->z_NavPos.z_PosFlags.b_IsValid)

  else
  {
    p_PdsmExtPos->sessionStatus = eLOC_ENG_SESS_STATUS_GENERAL_FAILURE_V01;
  }
#endif
}

/*===========================================================================

FUNCTION tm_core_calculate_2d_punc

DESCRIPTION
  This function is used to calculate the 2D Punc from the
  gnss_NavSolutionStructType data.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
float tm_core_calculate_2d_punc(const sm_FixInfoStructType *pz_GnssFixRpt)
{
  float f_Punc = CGPS_WHOLE_EARTH_CEP99;
  if (pz_GnssFixRpt->z_NavPos.z_PosFlags.b_IsValid)
  {
    f_Punc = (FLT)sqrt(pz_GnssFixRpt->z_NavPos.f_ErrorEllipse[1] *
                       pz_GnssFixRpt->z_NavPos.f_ErrorEllipse[1] +
                       pz_GnssFixRpt->z_NavPos.f_ErrorEllipse[2] *
                       pz_GnssFixRpt->z_NavPos.f_ErrorEllipse[2]);
  }
  return f_Punc;
}

/*===========================================================================
tm_core_internal_update_gps_state_data

Description:
   This function is used to update the internal state of TM's GPS state info data.

Parameters: 
  p_gnss_fix_rpt: The position report from MGP.  

Return value: 
  Void.
=============================================================================*/
void tm_core_internal_update_gps_state_data(const sm_FixInfoStructType *p_gnss_fix_rpt)
{
  float f_Punc = 0;

  f_Punc = tm_core_calculate_2d_punc(p_gnss_fix_rpt);
  /* Convert latitude from radians to degrees and scale. */
  if (p_gnss_fix_rpt->z_NavPos.z_PosFlags.b_IsValid)
  {
    tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_POS_VALID;
  }

  if (p_gnss_fix_rpt->z_NavPos.f_PuncVertMeters < 10000)
  {
    tm_core_info.gps_state_info.loc_unc_vertical = (uint16)p_gnss_fix_rpt->z_NavPos.f_PuncVertMeters;
    tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_ALT_VALID;
  }
  else
  {
    tm_core_info.gps_state_info.loc_unc_vertical = 10000;
  }
  tm_core_info.gps_state_info.lat = (int32)(p_gnss_fix_rpt->z_NavPos.d_PosLla[0] * C_2_32_OVER_PI);

  /* Convert longitude from radians to degrees and scale */
  tm_core_info.gps_state_info.lon = (int32)(p_gnss_fix_rpt->z_NavPos.d_PosLla[1] * C_2_31_OVER_PI);
  tm_core_info.gps_state_info.pos_unc  = (uint32)f_Punc;
  tm_core_info.gps_state_info.altitude = (int16)(tm_util_nearest_long(p_gnss_fix_rpt->z_NavPos.d_PosLla[2]));
  tm_core_info.gps_state_info.engine_state = tm_core_info.e_cached_rcvr_state;

  tm_core_info.gps_state_info.loc_unc_vertical = (uint16)(p_gnss_fix_rpt->z_NavPos.f_PuncVertMeters);
  if (p_gnss_fix_rpt->z_NavPos.z_SmPosFlags.b_IsGpsTimeValid)
  {
    tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_TIME_MS_VALID;
    tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_TIME_WN_VALID;
  }
  tm_core_info.gps_state_info.timestamp_gps_week = p_gnss_fix_rpt->z_NavPos.w_GpsWeek;
  tm_core_info.gps_state_info.timestamp_tow_ms = p_gnss_fix_rpt->z_NavPos.q_UtcTimeMs;

  if (p_gnss_fix_rpt->z_NavPos.z_SmPosFlags.b_IsTuncValid)
  {
    tm_core_info.gps_state_info.flags |= PDSM_PD_EXT_GPS_STATE_TUNC_VALID;
  }
  tm_core_info.gps_state_info.time_unc_us = (uint32)(p_gnss_fix_rpt->z_NavPos.f_ClockBiasUncMs);
}

/*===========================================================================
tm_core_handle_gps_state_info_req_type_query_state

Description:
   This function is used to handle the GPS state information repsonse to the request made by
   querying from PDAPI. 

Parameters: 
  p_gnss_fix_rpt: The position report from MGP.  

Return value: 
  Void.
=============================================================================*/
static void tm_core_handle_gps_state_info_req_type_query_state(const sm_FixInfoStructType *p_gnss_fix_rpt)
{
  pdsm_ext_status_info_s_type  pd_ext_status_info;
  tm_core_internal_update_gps_state_data(p_gnss_fix_rpt);

  if (tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].w_TmRcvdDataMask == TM_RCVD_ALL)
  {
    pd_ext_status_info.ext_status_type = PDSM_EXT_STATUS_GPS_STATE_INFO;
    tm_core_xlate_gps_state_to_ext_status(&tm_core_info.gps_state_info,
                                          &pd_ext_status_info.ext_status_info.ext_gps_state_type);
    tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_GPS_STATE_INFO, &pd_ext_status_info);
    tm_core_info.session_info.q_ReqClientMask &= ~(TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE));
    tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE].w_TmRcvdDataMask = 0;
  }
}


/*===========================================================================
tm_core_handle_gps_state_info_req_type_tech_sel

Description:
   This function is used to handle the GPS state information repsonse to the request made by
   Tech Sel

Parameters: 
  p_gnss_fix_rpt: The position report from MGP. 

Return value: 
  Void.
=============================================================================*/
static void tm_core_handle_gps_state_info_req_type_tech_sel(sm_FixInfoStructType *p_gnss_fix_rpt)
{
  /*Send Best Avail Pos to TechSel*/
  (void)techSel_ReportPosFix(p_gnss_fix_rpt);
  tm_core_info.session_info.q_ReqClientMask &= ~(TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_TECH_SEL));
  tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_TECH_SEL].w_TmRcvdDataMask = 0;
}

/*===========================================================================
tm_core_handle_gps_state_info_req_type_best_avail_pos_qmi

Description:
   This function is used to handle the GPS state information repsonse to the request made for
   best available position from  PDAPI.

Parameters: 
  p_gnss_fix_rpt: The position report from MGP. 

Return value: 
  Void.
=============================================================================*/
static void tm_core_handle_gps_state_info_req_type_best_avail_pos_qmi(const sm_FixInfoStructType *p_gnss_fix_rpt)
{
  pdsm_ext_status_info_s_type  pd_ext_status_info;
  pdsm_pd_cmd_err_e_type      e_cmd_err = PDSM_PD_CMD_ERR_NOERR;
  //pdsm_cmd_s_type *p_cmd_type;
#ifdef FEATURE_CGPS_XTRA_T
  float                       f_Punc = tm_core_calculate_2d_punc(p_gnss_fix_rpt);
#endif

  // This is in response from ME to the get best available position request from PDAPI .
  // Convert the data into the format of QMI and send it as an extended status event callback.
  pd_ext_status_info.client_id       = tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_QMI].q_ClientId;
  pd_ext_status_info.ext_status_type = PDSM_EXT_STATUS_BEST_AVAIL_POS_REPORT;
  tm_core_xlate_pd_ext_pos_to_qmi(p_gnss_fix_rpt,
                                  &pd_ext_status_info.ext_status_info.ext_pos_report_qmi_type);
  e_cmd_err = tm_core_get_best_avail_pos_err_check();
  if (e_cmd_err == PDSM_PD_CMD_ERR_NOERR)
  {
    tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_BEST_AVAIL_POS_REPORT, &pd_ext_status_info);
#ifdef FEATURE_CGPS_XTRA_T
    tle_StartKeepWarmPeriod();
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core PUNC:%u", (uint32)f_Punc, 0, 0);
    if (f_Punc > PUNC_THRESHOLD_FOR_XTRA_T)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core: initiate a GTP session", 0, 0, 0);
      // Punc is greater than 25 Km.
      // Start the Xtra-T download session.
      tle_StartPuncBasedSession(f_Punc);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core: No need to initiate GTP session", 0, 0, 0);
    }
#endif
  }
  else
  {
    // Notify the client that there was an error
    //tm_pd_client_cmd_status( p_cmd_type, e_cmd_err );
  }

  tm_core_info.session_info.q_ReqClientMask &= ~(TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_QMI));
  tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_QMI].w_TmRcvdDataMask = 0;
}

/*===========================================================================
tm_core_handle_gps_state_info_req_type_avail_wwan_pos_qmi

Description:
   This function is used to handle the GPS state information repsonse to the request made for
   available WWAN position from PDAPI.

Parameters:
  p_gnss_fix_rpt: The position report from MGP.

Return value:
  Void.
=============================================================================*/
static void tm_core_handle_gps_state_info_req_type_avail_wwan_pos_qmi(const sm_FixInfoStructType *p_gnss_fix_rpt)
{
  pdsm_ext_status_info_s_type pd_ext_status_info;
  pdsm_pd_cmd_err_e_type      e_cmd_err = PDSM_PD_CMD_ERR_NOERR;
#ifdef FEATURE_CGPS_XTRA_T
  float                       f_Punc = tm_core_calculate_2d_punc(p_gnss_fix_rpt);
#endif

  // This is in response from ME to the get available WWAN position request from PDAPI.
  // Convert the data into the format of QMI and send it as an extended status event callback.
  pd_ext_status_info.client_id       = tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_TLE_AVAIL_POS_QMI].q_ClientId;
  pd_ext_status_info.ext_status_type = PDSM_EXT_STATUS_AVAIL_WWAN_POS_REPORT;
  tm_core_xlate_pd_ext_pos_to_qmi(p_gnss_fix_rpt,
                                  &pd_ext_status_info.ext_status_info.ext_pos_report_qmi_type);
  e_cmd_err = tm_core_get_best_avail_pos_err_check();
  if (e_cmd_err == PDSM_PD_CMD_ERR_NOERR)
  {
    tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_AVAIL_WWAN_POS_REPORT, &pd_ext_status_info);
#ifdef FEATURE_CGPS_XTRA_T
    tle_StartKeepWarmPeriod();
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core PUNC:%u", (uint32)f_Punc, 0, 0);
    if (f_Punc > PUNC_THRESHOLD_FOR_XTRA_T)
    {
      // Punc is greater than 25 Km.
      // Start the Xtra-T download session.
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core: initiate a GTP session", 0, 0, 0);
      tle_StartPuncBasedSession(f_Punc);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core: No need to initiate GTP session", 0, 0, 0);
    }
#endif
  }

  tm_core_info.session_info.q_ReqClientMask &= ~(TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_GET_TLE_AVAIL_POS_QMI));
  tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_TLE_AVAIL_POS_QMI].w_TmRcvdDataMask = 0;
}
/*===========================================================================
tm_core_handle_gps_state_info_req_type_available_pos_qmi

Description:
   This function is used to handle the GPS state information repsonse to the request made for
   available GNSS position from PDAPI.

Parameters:
  p_gnss_fix_rpt: The position report from MGP.
  b_ReqType:    The position report source type from MGP.

Return value:
  Void.
=============================================================================*/
static void tm_core_handle_gps_state_info_req_type_available_pos_qmi(const sm_FixInfoStructType *p_gnss_fix_rpt, sm_ReqEnumType b_ReqType)
{
  pdsm_ext_status_info_s_type pd_ext_status_info = { 0 };
  pdsm_pd_cmd_err_e_type      e_cmd_err = PDSM_PD_CMD_ERR_NOERR;

  if ((NULL == p_gnss_fix_rpt) ||
      (b_ReqType >= GNSS_ARRAY_SIZEOF(tm_core_info.session_info.z_ReqClients)))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_core: NULL p_gnss_fix_rpt? %d, ReqType %d", NULL == p_gnss_fix_rpt, b_ReqType);
    return;
  }

#ifdef FEATURE_CGPS_XTRA_T
  float  f_Punc = tm_core_calculate_2d_punc(p_gnss_fix_rpt);
#endif

  // This is in response from ME to the get available position request from PDAPI.
  // Convert the data into the format of QMI and send it as an extended status event callback.
  pd_ext_status_info.client_id       = tm_core_info.session_info.z_ReqClients[b_ReqType].q_ClientId;
  pd_ext_status_info.ext_status_type = PDSM_EXT_STATUS_AVAIL_POS_REPORT;
  tm_core_xlate_pd_ext_pos_to_qmi(p_gnss_fix_rpt,
                                  &pd_ext_status_info.ext_status_info.ext_pos_report_qmi_type);
  e_cmd_err = tm_core_get_best_avail_pos_err_check();
  if (e_cmd_err == PDSM_PD_CMD_ERR_NOERR)
  {
    tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_AVAIL_POS_REPORT, &pd_ext_status_info);
#ifdef FEATURE_CGPS_XTRA_T
    tle_StartKeepWarmPeriod();
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core PUNC:%u", (uint32)f_Punc, 0, 0);
    if (f_Punc > PUNC_THRESHOLD_FOR_XTRA_T)
    {
      // Punc is greater than 25 Km.
      // Start the Xtra-T download session.
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core: initiate a GTP session", 0, 0, 0);
      tle_StartPuncBasedSession(f_Punc);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core: No need to initiate GTP session", 0, 0, 0);
    }
#endif
  }

  tm_core_info.session_info.q_ReqClientMask &= ~(TM_CORE_CLIENT_BITMASK(b_ReqType));
  tm_core_info.session_info.z_ReqClients[b_ReqType].w_TmRcvdDataMask = 0;
}

/*===========================================================================

FUNCTION tm_core_report_gps_state_info

DESCRIPTION
  This function is called by MGP to return the gps state info 
  for on-demand positioning and user requested state info
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_report_gps_state_info(void *pos_data)
{
  sm_FixReportStructType   z_GpsBestAvailPos = { 0 };
  sm_FixInfoStructType *p_FixInfo = &z_GpsBestAvailPos.z_FixInfo;


  /* Function arguments sanity check */
  if (NULL == pos_data)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_core_report_gps_state_info: NULL argument received");
    return;
  }

  /* Copy contents of pointer argument passed by MGP before it gets overwritten */
  z_GpsBestAvailPos = *((sm_FixReportStructType *)pos_data);

  /* Pointer arguments sanity check */
  if (z_GpsBestAvailPos.b_ReqType > SM_GPS_STATE_INFO_REQ_TYPE_MAX)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_core_report_gps_state_info: NULL pointer argument received");
    return;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core_report_gps_state_info: Rcvd response for request %d from MGP", z_GpsBestAvailPos.b_ReqType);

  tm_core_info.session_info.z_ReqClients[z_GpsBestAvailPos.b_ReqType].w_TmRcvdDataMask |= TM_RCVD_TUNC_FROM_ME;
  tm_core_info.session_info.z_ReqClients[z_GpsBestAvailPos.b_ReqType].w_TmRcvdDataMask |= TM_RCVD_POS_FROM_PE;

  switch (z_GpsBestAvailPos.b_ReqType)
  {
    case SM_GPS_STATE_INFO_REQ_TYPE_QUERY_STATE:
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Gps state info for query state recv. ");
      tm_core_handle_gps_state_info_req_type_query_state(p_FixInfo);
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_TECH_SEL:
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Gps state info for Tech Sel ");
      tm_core_handle_gps_state_info_req_type_tech_sel(p_FixInfo);
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_QMI:
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Gps state info for Best avail Pos recv for QMI. ");
      tm_core_handle_gps_state_info_req_type_best_avail_pos_qmi(p_FixInfo);
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_TLE_AVAIL_POS_QMI:
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Gps state info for Avail WWAN Pos recv for QMI. ");
      tm_core_handle_gps_state_info_req_type_avail_wwan_pos_qmi(p_FixInfo);
      break;
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_GNSS_AVAIL_POS_QMI:
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_REFLOC_AVAIL_POS_QMI:
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_COARSE_AVAIL_POS_QMI:
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_WIFI_AVAIL_POS_QMI:
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_GILE_AVAIL_POS_QMI:
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Gps state info for Avail Pos recv for QMI. ");
      tm_core_handle_gps_state_info_req_type_available_pos_qmi(p_FixInfo, z_GpsBestAvailPos.b_ReqType);
      break;
#ifdef FEATURE_CGPS_XTRA_T
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_XTRA_T:
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Gps state info for Best avail Pos recv for XTRA_T. ");
      tm_xtra_t_handle_best_avail_pos(p_FixInfo);

      tm_core_info.session_info.q_ReqClientMask &= ~(TM_CORE_CLIENT_BITMASK(SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_XTRA_T));
      tm_core_info.session_info.z_ReqClients[SM_GPS_STATE_INFO_REQ_TYPE_GET_BEST_AVAIL_POS_XTRA_T].w_TmRcvdDataMask = 0;
      break;
#endif
#ifndef FEATURE_GNSS_LEAN_BUILD
#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
    case SM_GPS_STATE_INFO_REQ_TYPE_GET_DBH:
      /* DBH Position reported by PE as part of GET BEST POS
         is not shipped as DBH. This case results in
         a NO-OP. Position cached from CPI injection is used
         instead*/
#ifdef FEATURE_CGPS_FW_NON_STANDARD_SUPPORT
      #error code not present
#endif
      break;
#endif /* FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS */
#endif /* ! FEATURE_GNSS_LEAN_BUILD */
    default:
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Get GPS State unknown Req Type %d", z_GpsBestAvailPos.b_ReqType);
      break;
  }
}


/*===========================================================================

FUNCTION tm_core_get_utc_offset

DESCRIPTION
  This function returns the UTC offset obtained from MGP. If UTC offset is invalid, then the UTC offset will contain
  default value from MGP.

RETURN VALUE
  Boolean: TRUE: If UTC offset is valid
                FALSE: If UTC offset is invalid

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean tm_core_get_utc_offset(uint8 *p_utc_offset)
{
  boolean v_leap_sec_valid = FALSE;

  *p_utc_offset = tm_core_info.config_info.z_LeapSecondInfo.u_DeltaLeapSec;

  if (0 == tm_core_info.config_info.z_LeapSecondInfo.u_DeltaLeapSecUnc)
  {
    v_leap_sec_valid = TRUE;
  }

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Leap Second = %u, valid = %u",
        tm_core_info.config_info.z_LeapSecondInfo.u_DeltaLeapSec, v_leap_sec_valid);

  return v_leap_sec_valid;
}

/*===========================================================================

FUNCTION tm_core_get_call_flow_select_mask

DESCRIPTION
  This function returns the UMTS CP MOLR vs AD call flow select mask in TM data

RETURN VALUE
  uint8 call flow select mask

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
uint8 tm_core_get_call_flow_select_mask(void)
{
  return (tm_core_info.config_info.call_flow_select_mask);
}

/*===========================================================================

FUNCTION tm_core_save_ref_position

DESCRIPTION
  This function saves the reference position received from the network

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_save_ref_position(pdsm_ext_status_info_s_type   *pd_ext_status_info)
{

  z_cached_ref_pos_info.cached_ref_pos_valid = TRUE;
  memscpy(&z_cached_ref_pos_info.pd_ext_status_info, sizeof(z_cached_ref_pos_info.pd_ext_status_info),
          pd_ext_status_info, sizeof(*pd_ext_status_info));

  z_cached_ref_pos_info.ref_pos_reported = FALSE;
}

/*===========================================================================

FUNCTION tm_core_get_efs_supl_cert_status

DESCRIPTION
  This function tells the caller whether it must use SUPL Cert from EFS or not.

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_efs_supl_cert_status(void)
{
  return (tm_use_efs_for_supl_cert);
}
/*===========================================================================

FUNCTION tm_core_set_efs_supl_cert_status

DESCRIPTION
 IF tm_use_efs_for_supl_cert == TRUE, supl session must read supl cert from EFS.

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_set_efs_supl_cert_status(boolean new_efs_cert_value)
{
  tm_use_efs_for_supl_cert = new_efs_cert_value;
}

/*===========================================================================

FUNCTION tm_core_get_current_dd_subs_id

DESCRIPTION
  This function returns the ID for current DD subscription

RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
sys_modem_as_id_e_type tm_core_get_current_dd_subs_id(void)
{
  return (tm_current_dd_subs_id);
}

/*===========================================================================

FUNCTION tm_core_check_sub_capability

DESCRIPTION
  This function provides the sub capability of the desired sub
 
RETURN VALUE
boolean

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_check_sub_capability(sys_modem_as_id_e_type sub,
                                     sys_sys_mode_e_type sys_mode)
{
  sys_sys_mode_mask_e_type   sub_info;

  if ((sub < SM_CONFIG_MIN_SUB_ID) || (sub > SM_CONFIG_MAX_SUB_ID))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Invalid parameter sub");
    return FALSE;
  }

  if ((sys_mode <= SYS_SYS_MODE_NONE) || (sys_mode >= SYS_SYS_MODE_MAX))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Invalid parameter sys_mode");
    return FALSE;
  }

  sub_info = tm_cm_get_sub_capability(sub);

  if ((sub_info <= SYS_SYS_MODE_MASK_NONE) || (sub_info >= SYS_SYS_MODE_MASK_MAX))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Invalid parameter sub_info");
    return FALSE;
  }

  if ((sub_info & BM(sys_mode)) == BM(sys_mode))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/*===========================================================================

FUNCTION tm_core_set_current_dd_subs_id

DESCRIPTION
  This function updates the stored value for current DD subscription

  This should be the only function to update TM's DD sub info, all triggering
  conditions should eventually lead to this function to update DD sub.
  (If not, carrier-specific EFS NVs may fail)
 
RETURN VALUE


DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_set_current_dd_subs_id(sys_modem_as_id_e_type asid, boolean reConfig)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "DD sub being changed to %d (from %d); Force ReConf: %d",
        asid, tm_current_dd_subs_id, reConfig);

  tm_current_dd_subs_id = asid;

  /* set currently active subs-specific configuration after DD change */
  tm_core_set_current_subs_cfg(asid, reConfig);
}


/*====================================================================
FUNCTION tm_mmgsdi_set_session_id

DESCRIPTION

  This function updates the tm mmgsdi array with session id info for
  the given session type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_mmgsdi_set_session_id
(
   mmgsdi_session_type_enum_type tm_mmgsdi_session_type,
   mmgsdi_session_id_type session_id
   )
{
  int index;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if (tm_mmgsdi_session_type == tm_mmgsdi_session_id_table[index].session_type)
    {
      tm_mmgsdi_session_id_table[index].session_id = session_id;
      tm_mmgsdi_session_id_table[index].session_id_set = TRUE;
      break;
    }
  }
} /* tm_mmgsdi_set_session_id */

/*====================================================================
FUNCTION tm_mmgsdi_get_session_id_status

DESCRIPTION

  This function informs whether the session id for a given
  mmgsdi session type has been set or not.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : Session ID was programmed earlier
  FALSE : Session ID not programmed.

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_get_session_id_status
(
   mmgsdi_session_type_enum_type tm_mmgsdi_session_type
   )
{
  int index;
  boolean status = FALSE;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if (tm_mmgsdi_session_type == tm_mmgsdi_session_id_table[index].session_type)
    {
      status = tm_mmgsdi_session_id_table[index].session_id_set;
      break;
    }
  }
  return (status);
} /* tm_mmgsdi_get_session_id_status */

/*====================================================================
FUNCTION tm_mmgsdi_close_session_id

DESCRIPTION

  This function close a particular session id and reset its status

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : Session ID was closed successfully
  FALSE : Session ID not found

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_close_session_id
(
   mmgsdi_session_id_type session_id
   )
{
  int index;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if (session_id == tm_mmgsdi_session_id_table[index].session_id)
    {
      tm_mmgsdi_session_id_table[index].session_id = 0; // reset to 0
      tm_mmgsdi_session_id_table[index].session_id_set = FALSE;
      return TRUE;
    }
  }

  return FALSE;
}

/*====================================================================
FUNCTION tm_mmgsdi_get_session_id_ptr

DESCRIPTION

  This function returns a pointer to the session ID for the specified
  session type.  At most one session ID is associated with each session
  type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : If session id was found
  FALSE : Session id for this session type not found.

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_get_session_id_ptr
(
   mmgsdi_session_type_enum_type tm_mmgsdi_session_type,
   mmgsdi_session_id_type     **tm_mmgsdi_session_id_ptr
   )
{
  int index;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if (tm_mmgsdi_session_type == tm_mmgsdi_session_id_table[index].session_type)
    {
      *tm_mmgsdi_session_id_ptr = &(tm_mmgsdi_session_id_table[index].session_id);
      return TRUE;
    }
  }

  return FALSE;

} /* tm_mmgsdi_get_session_id_ptr */

/*====================================================================
FUNCTION tm_mmgsdi_get_sim_info_from_asid

DESCRIPTION

  This function returns a pointer to the session ID for the specified
  session type.  At most one session ID is associated with each session
  type.


DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : If session id was found
  FALSE : Session id for this session type not found.

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_get_sim_info_from_asid
(
   sys_modem_as_id_e_type      asubs_id,
   mmgsdi_session_id_type     *tm_mmgsdi_session_id_ptr,
   mmgsdi_app_enum_type        *app_type_ptr,
   mmgsdi_app_enum_type      app_type
   )
{
  int index;
  boolean status = FALSE;
  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if ((asubs_id == tm_mmgsdi_session_id_table[index].asubs_id) &&
        (app_type == tm_mmgsdi_session_id_table[index].app_type) &&
        (tm_mmgsdi_session_id_table[index].session_id_set == TRUE)
        )
    {
      *tm_mmgsdi_session_id_ptr = tm_mmgsdi_session_id_table[index].session_id;
      *app_type_ptr = tm_mmgsdi_session_id_table[index].app_type;
      status = TRUE;
      break;
    }

  }

  return status;

} /* tm_mmgsdi_get_sim_info_from_asid */

/*====================================================================
FUNCTION tm_mmgsdi_set_subscription_id

DESCRIPTION

  This function updates the tm mmgsdi session info table with the
  subscription id assigned to a given session type.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_mmgsdi_set_subscription_id
(
   mmgsdi_session_type_enum_type tm_mmgsdi_session_type,
   sys_modem_as_id_e_type        asubs_id
   )
{
  int index;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if (tm_mmgsdi_session_type == tm_mmgsdi_session_id_table[index].session_type)
    {
      tm_mmgsdi_session_id_table[index].asubs_id = asubs_id;
      break;
    }
  }
} /* tm_mmgsdi_set_subscription_id */


/*====================================================================
FUNCTION tm_mmgsdi_get_subscription_id

DESCRIPTION
  This function gets the subscription id assigned to a given session ID
  from tm mmgsdi session info table.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE
  TRUE : subscription id had been found and returned
  FALSE : failed to find subscription id for a particular session id.

SIDE EFFECTS
  none

=====================================================================*/
boolean tm_mmgsdi_get_subscription_id
(
   mmgsdi_session_id_type     session_id,
   sys_modem_as_id_e_type     *asubs_id
   )
{
  int index;

  if (asubs_id == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "asusbs_id is NULL", 0, 0, 0);
    return FALSE;
  }

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if (session_id == tm_mmgsdi_session_id_table[index].session_id)
    {
      *asubs_id = tm_mmgsdi_session_id_table[index].asubs_id;
      return TRUE;
    }
  }

  return FALSE;

} /* tm_mmgsdi_get_subscription_id */


/*====================================================================
FUNCTION tm_mmgsdi_set_app_type

DESCRIPTION

  This function updates the tm mmgsdi session info table with the
  app type for a given session id.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_mmgsdi_set_app_type
(
   mmgsdi_session_id_type session_id,
   mmgsdi_app_enum_type         app_type
   )
{
  int index;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if ((session_id == tm_mmgsdi_session_id_table[index].session_id) &&
        (tm_mmgsdi_session_id_table[index].session_id_set == TRUE))
    {
      tm_mmgsdi_session_id_table[index].app_type = app_type;
      break;
    }
  }
} /* tm_mmgsdi_set_app_type */

/*====================================================================
FUNCTION tm_mmgsdi_get_app_type_from_asid

DESCRIPTION

  This function returns the app type for the active session corresponding to
  the sub id. If the sub id matches but the session is not active, it returns
  MMGSDI_APP_NONE.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
mmgsdi_app_enum_type tm_mmgsdi_get_app_type_from_asid
(
   sys_modem_as_id_e_type     asubs_id
   )
{
  int index;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if ((asubs_id == tm_mmgsdi_session_id_table[index].asubs_id) &&
        (tm_mmgsdi_session_id_table[index].session_id_set == TRUE))
    {
      return tm_mmgsdi_session_id_table[index].app_type;
    }
  }
  return MMGSDI_APP_NONE;
} /* tm_mmgsdi_get_app_type_from_sub_id */


/*====================================================================
FUNCTION tm_mmgsdi_set_slot_id

DESCRIPTION

  This function updates the tm mmgsdi session info table with the
  slot id for a given session id.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_mmgsdi_set_slot_id
(
   mmgsdi_session_id_type session_id,
   mmgsdi_slot_id_enum_type slot_id
   )
{
  int index;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if ((session_id == tm_mmgsdi_session_id_table[index].session_id) &&
        (tm_mmgsdi_session_id_table[index].session_id_set == TRUE))
    {
      tm_mmgsdi_session_id_table[index].slot_id = slot_id;
      return;
    }
  }
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid Session ID %d passed or the passed session ID is not set !!", session_id);
} /* tm_mmgsdi_set_slot_id */

/*====================================================================
FUNCTION tm_mmgsdi_get_slot_id_from_asid

DESCRIPTION

  This function returns the slot id for the session corresponding to the
  sub_id. If the sub id matches but the session is not active, it returns
  MMGSDI_SLOT_NONE.

DEPENDENCIES
  This function is to be used only with the session based MMGSDI.

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
mmgsdi_slot_id_enum_type tm_mmgsdi_get_slot_id_from_asid
(
   sys_modem_as_id_e_type     asubs_id
   )
{
  int index;

  for (index = 0; index < sizeof(tm_mmgsdi_session_id_table) / sizeof(tm_mmgsdi_session_id_table_entry_s_type); index++)
  {
    if ((asubs_id == tm_mmgsdi_session_id_table[index].asubs_id) &&
        (tm_mmgsdi_session_id_table[index].session_id_set == TRUE))
    {
      return tm_mmgsdi_session_id_table[index].slot_id;
    }
  }
  return MMGSDI_SLOT_NONE;
} /* tm_mmgsdi_get_slot_id_from_asid */


/*====================================================================
FUNCTION tm_core_get_xtra_validity_age

DESCRIPTION

  This function gets the NV item for the xtra validity age

DEPENDENCIES

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_core_get_xtra_validity_age(uint16 *p_age)
{

  if (p_age)
  {
    *p_age = tm_core_info.config_info.preferred_xtra_age;
  }

}

/*====================================================================
FUNCTION tm_core_set_xtra_validity_age

DESCRIPTION

  This function sets the NV item for the xtra validity age

DEPENDENCIES

RETURN VALUE


SIDE EFFECTS
  none

=====================================================================*/
void tm_core_set_xtra_validity_age(uint16 w_age)
{
  if (sm_nv_efs_reg_item_write(CGPS_NV_EFS_SM_XTRA_PREFERRED_MAX_VALID_AGE,
                                  sizeof(uint16), (void *)&w_age) == FALSE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Xtra Max pref validity age efs write failed", 0, 0, 0);
  }
  else
  {
    tm_core_info.config_info.preferred_xtra_age = w_age;
  }
}

/*====================================================================
FUNCTION tm_core_set_xtra_uim_subscription

DESCRIPTION

  This function sets the UIM subscription data maintained by XTRA

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
  none

=====================================================================*/
void tm_core_set_xtra_uim_subscription(const void         * p_info)
{
  tm_xtra3_uim_data_s_type* p_uim_data = tm_xtra3_get_uim_data_record();
  sys_modem_as_id_e_type    e_as_id   = SYS_MODEM_AS_ID_NONE;
  const mmgsdi_event_data_type  *p_ruimEventInfo = (mmgsdi_event_data_type*)p_info;

  if (NULL == p_uim_data)
  {
    MSG_3(MSG_SSID_GPS,MSG_LEGACY_MED,"Null xtra uim data record", 0, 0, 0);
    return;
  }

  if (NULL == p_ruimEventInfo)
  {
    MSG_3(MSG_SSID_GPS,MSG_LEGACY_MED,"Null p_info", 0, 0, 0);
    return;
  }

  switch(p_ruimEventInfo->data.subscription_ready.app_info.slot)
  {
    case MMGSDI_SLOT_1:
    {
      p_uim_data[TM_XTRA3_UIM_CARD_0].u_asId = (uint8)SYS_MODEM_AS_ID_1;
      p_uim_data[TM_XTRA3_UIM_CARD_0].b_subcriptionReady = true;
      break;
    }
    case MMGSDI_SLOT_2:
    {
      p_uim_data[TM_XTRA3_UIM_CARD_1].u_asId = (uint8)SYS_MODEM_AS_ID_2;
      p_uim_data[TM_XTRA3_UIM_CARD_1].b_subcriptionReady = true;
      break;
    }
    case MMGSDI_SLOT_3:
    {
      p_uim_data[TM_XTRA3_UIM_CARD_2].u_asId = (uint8)SYS_MODEM_AS_ID_3;
      p_uim_data[TM_XTRA3_UIM_CARD_2].b_subcriptionReady = true;
      break;
    }
    default:
      MSG_3(MSG_SSID_GPS,MSG_LEGACY_MED,"Unexpected slotid = %u",
            p_ruimEventInfo->data.subscription_ready.app_info.slot, 0, 0);
  };
}
/*====================================================================
FUNCTION tm_core_set_xtra_srv_sys_info
DESCRIPTION
  This function sets PLMN info for the provided srv system information
DEPENDENCIES
RETURN VALUE
SIDE EFFECTS
  none
=====================================================================*/
void tm_core_set_xtra_srv_sys_info(const tm_cm_mm_ss_info_s_type * const ss_info)
{
  uint16  w_PlmnMcc = 0;
  uint16 *p_MccList = tm_xtra3_get_srv_sys_mcc_List();
  uint8   u_Stack = 0;
  if (NULL == ss_info)
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED,"Null args", 0, 0, 0);
    return;
  }
  if (NULL == p_MccList)
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED,"Null mcc list", 0, 0, 0);
    return;
  }
  /*For a given SubId , Need to get the PLMN of the Stack which is 
    in Serving state and is operational. Since this requirement for
    XTRA is currently to identify indian PLMN and Modify Navic downloads
    we are more interesetd in UMTS systems.
  */
  for (u_Stack = 0; u_Stack < ss_info->number_of_stacks; u_Stack++)
  {
    MSG_3(MSG_SSID_GPS, MSG_LEGACY_MED,"isOperational = %d srvStatus = %d sysId = %d",
          ss_info->multisim_stack_info[u_Stack].is_operational,
          ss_info->multisim_stack_info[u_Stack].srv_status,
          ss_info->multisim_stack_info[u_Stack].sys_id.id_type);
    if (ss_info->multisim_stack_info[u_Stack].is_operational &&
       SYS_SRV_STATUS_SRV == ss_info->multisim_stack_info[u_Stack].srv_status &&
       SYS_SYS_ID_TYPE_UMTS == ss_info->multisim_stack_info[u_Stack].sys_id.id_type)
    {
      sys_plmn_id_s_type z_SysPlmn = ss_info->multisim_stack_info[u_Stack].sys_id_id_plmn;

      w_PlmnMcc = ((uint16)(z_SysPlmn.identity[0] & 0x0F)*100) + \
                  ((uint16)((z_SysPlmn.identity[0] & 0xF0) >> 4)*10) + \
                  ((uint16)(z_SysPlmn.identity[1] & 0x0F));
      break;
    }
  }

  MSG_3(MSG_SSID_GPS,MSG_LEGACY_MED,"sub %d plmnMcc %d", ss_info->asubs_id, w_PlmnMcc, 0);
  switch (ss_info->asubs_id)
  {
    case SYS_MODEM_AS_ID_1:
    {
      p_MccList[TM_XTRA3_UIM_CARD_0] = w_PlmnMcc;
      break;
    }
    case SYS_MODEM_AS_ID_2:
    {
      p_MccList[TM_XTRA3_UIM_CARD_1] = w_PlmnMcc;
      break;
    }
    case SYS_MODEM_AS_ID_3:
    {
      p_MccList[TM_XTRA3_UIM_CARD_2] = w_PlmnMcc;
      break;
    }
    default:
    {
      MSG_3(MSG_SSID_GPS,MSG_LEGACY_MED,"Unhandled sub id %d", ss_info->asubs_id, 0, 0);
      break;
    }
  }
}

/*===========================================================================

FUNCTION tm_core_time_validity

DESCRIPTION
   Reports whether the time from time_get_uptime_ms is valid or not

DEPENDENCIES

RETURN VALUE
  boolean: TRUE - time is valid
           FALSE - time is not valid

SIDE EFFECTS
  None

===========================================================================*/
boolean tm_core_time_validity()
{


#ifndef FEATURE_GNSS_LEAN_BUILD

  time_type curr_time;
  time_get_uptime_ms(curr_time);

  if (curr_time[1] > MSB_MSEC_ELAPSED_SINCE_DEFAULT)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TSG: Time is valid", 0, 0, 0);
    return TRUE;
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TSG: Time is invalid", 0, 0, 0);
    return FALSE;
  }


#else
  return FALSE;
#endif

}

/*===========================================================================

FUNCTION tm_core_check_mt_up_allowed

DESCRIPTION
  Checks whether a mobile terminated User plane
  session can be allowed or not based on the service set as Designated Data
  by the user. E.g. MT V1/V2 will only be allowed if designated data is 1x.
  Similarly MT SUPL will only be allowed if designated data is GSM.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_check_mt_up_allowed
(
   tm_prtl_type  prtl_type
   )
{
  boolean              single_standby = tm_cm_dsds_at_in_single_standby();
  boolean              retval         = FALSE;
  sys_sys_mode_e_type  dd_sys_mode    = SYS_SYS_MODE_NO_SRV;

  /* Consider only 1xUP and UMTS_UP protocol types for MT UP sessions, return
   * false for all other protocols.
   * In Single standby mode, always honor an MT req, since there is only one
   * active subscription service, and its available for data. */
  switch (prtl_type)
  {
    case(TM_PRTL_TYPE_1X_UP):
      {
        if (single_standby)
        {
          retval = TRUE;
        }
        else
        {
          /* DD must be CDMA for an MT 1xUP req to be honoured */
          dd_sys_mode = tm_cm_get_sys_mode_for_as_id(tm_cm_dsds_get_dd_subs());
          if (dd_sys_mode == SYS_SYS_MODE_CDMA)
            retval = TRUE;
        }
        break;
      }

    case(TM_PRTL_TYPE_UMTS_UP):
      {
        if (single_standby)
        {
          retval = TRUE;
        }
        else
        {
          /* DD must be W, G or WG for an MT UMTS UP req to be honoured */
          dd_sys_mode = tm_cm_get_sys_mode_for_as_id(tm_cm_dsds_get_dd_subs());
          if (dd_sys_mode == SYS_SYS_MODE_GSM ||
              dd_sys_mode == SYS_SYS_MODE_WCDMA ||
              dd_sys_mode == SYS_SYS_MODE_GW)
          {
            retval = TRUE;
          }
        }
        break;
      }

      /* MT UP req for these prtcls is always an error */
    case(TM_PRTL_TYPE_STANDALONE):
    case(TM_PRTL_TYPE_1X_CP):
    case(TM_PRTL_TYPE_UMTS_CP):
    case(TM_PRTL_TYPE_DIAG):
    default:
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unexpected MT UP request by prtl %d", prtl_type, 0, 0);
      }
      break;
  } /* switch(prtl_type) */

  return retval;
}

/*===========================================================================

FUNCTION tm_agps_emergency_call_simulate

DESCRIPTION
   Reports whether emergency call simulation is enabled or not

DEPENDENCIES

RETURN VALUE
  boolean: TRUE - emergency call simulation enabled
           FALSE - emergency call simulation disabled

SIDE EFFECTS
  None

===========================================================================*/

boolean tm_agps_emergency_call_simulate(void)
{
  if ((tm_core_info.config_info.aagps_development_test_control5 & NV_AAGPS_TEST5_EMERGENCY_CALL_SIM) != 0)
  {
    return (TRUE);
  }
  else
  {
    return (FALSE);
  }
}

/*===========================================================================
FUNCTION tm_core_clear_cached_pos

DESCRIPTION Clears the RAM copy of the position

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_clear_cached_pos(void)
{
  TM_MED_0("Clearing cached position");
  memset((void *)&z_cached_pos_info, 0, sizeof(tm_core_cached_pos_s_type));
}

/*===========================================================================
FUNCTION tm_core_clear_cached_ref_pos

DESCRIPTION Clears the RAM copy of the ref position

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS
===========================================================================*/
void tm_core_clear_cached_ref_pos(void)
{
  TM_MED_0("Clearing cached ref position");
  memset((void *)&z_cached_ref_pos_info, 0, sizeof(tm_core_cached_ref_pos_s_type));
}
/*===========================================================================

FUNCTION tm_core_clear_cached_meas

DESCRIPTION
Function to clean up cached measurement report. Measurement is cached to
generate NMEA sentences.

DEPENDENCIES
NONE
INPUT PARAMS
NONE
RETURN VALUE
NONE

SIDE EFFECTS

===========================================================================*/
void tm_core_clear_cached_meas()
{
  TM_MED_0("Clearing cached measurements");
  memset((void *)&z_cached_meas_info, 0, sizeof(tm_core_cached_meas_s_type));
}
/*===========================================================================
FUNCTION tm_core_is_standalone_active

DESCRIPTION
Check if Standalone session is in progress.

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
boolean tm_core_is_standalone_active(void)
{
  if (tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_STANDALONE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
} /* tm_core_is_standalone_active */


/*===========================================================================
FUNCTION tm_core_get_asst_glo_lpp_up_enable

DESCRIPTION
  This function determines the positioning protocol for Assisted GLONASS U-plane
  is LPP.

RETURN VALUE
  TRUE if LPP is selected, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_asst_glo_lpp_up_enable(void)
{
    if (tm_umts_common_glonass_enabled() &&
        (TM_CORE_CONFIG_DD(asst_glonass_pos_protocol) & PDSM_PA_ASST_GLO_PROTOCOL_LPP_UP))
    {
      return TRUE;
    }
  return FALSE;
}

/*===========================================================================
FUNCTION tm_core_get_asst_glo_lpp_cp_enable

DESCRIPTION
  This function determines the positioning protocol for Assisted GLONASS C-plane
  is LPP.

RETURN VALUE
  TRUE if LPP is selected, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_asst_glo_lpp_cp_enable(void)
{
  if (tm_umts_common_glonass_enabled() &&
      (TM_CORE_CONFIG_DD(asst_glonass_pos_protocol) & PDSM_PA_ASST_GLO_PROTOCOL_LPP_CP))
  {
    return TRUE;
  }
  return FALSE;
}

/*===========================================================================
FUNCTION tm_core_lpp_grt_24_admeas_enabled

DESCRIPTION
  Return if the LPP greater than 24 AD & RSTD measurement processing is
  supported by UE.

RETURN VALUE
  TRUE if LPP > 24 AD feature is supported LPP, FALSE otherwise

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_lpp_grt_24_admeas_enabled(void)
{
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "NV ITEM for > 24 AD_Meas configured as %d", TM_CORE_CONFIG_DD(u_gnss_lpp_grt_24_otdoa));
  return ((0 == TM_CORE_CONFIG_DD(u_gnss_lpp_grt_24_otdoa)) ? FALSE : TRUE);
}

/*===========================================================================

FUNCTION tm_core_otdoa_evt_handler

DESCRIPTION
  This function receives OTDOA measurements from the OTDOA interface and sends
  them to the appropriate protocol
 

RETURN VALUE
  None 

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_otdoa_evt_handler(sm_loc_lte_otdoa_meas_info_type *p_otdoa_measurement_info)
{
  int prtlTableIndex;
  prtl_sess_info_param_u_type   *sess_info_param_ptr = NULL;
  boolean retVal = FALSE;
  tm_sess_handle_type sess_handle = tm_core_info.session_info.active_sess_handle;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Received OTDOA measurements from SM_LOC", 0, 0, 0);
  if (p_otdoa_measurement_info == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No OTDOA measurment info", 0, 0, 0);
    return;
  }
  prtlTableIndex = tm_core_get_prtl_idx_by_type(p_otdoa_measurement_info->u_sess_type);

  if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
  {

    sess_info_param_ptr = (prtl_sess_info_param_u_type *)pdsm_getbuf(sizeof(prtl_sess_info_param_u_type));

    if (sess_info_param_ptr == NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to get buffer for sess_info_param_ptr", 0, 0, 0);
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      return;
    }

    /* send the data to the protocol regardless of if this is a TM session */
    /* Set reason to general error for now, if return value */
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp)
    {
      /* Internal p_meas_info pointer storage is owned by LOC OTDOA & persistant until the SM LPP CP session ends */
      memscpy((void *)&(sess_info_param_ptr->otdoa_data), sizeof(sess_info_param_ptr->otdoa_data),
              (void *)(p_otdoa_measurement_info), sizeof(*p_otdoa_measurement_info));

      if (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE)
      {
        tm_core_info.session_info.op_requested &= ~TM_OP_REQ_OTDOA;
      }
      sess_handle = tm_core_get_ni_sess_handle();
      retVal = tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp(sess_handle,
                                                                            PRTL_SESS_INFO_OTDOA,
                                                                            sess_info_param_ptr);
      if (retVal == FALSE)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Could not send the data to protocol", 0, 0, 0);
      }
    }

    (void)pdsm_freebuf((char *)sess_info_param_ptr);
  }
}

#if defined FEATURE_WCDMA_GPS_CELL_ID_READING
/*===========================================================================

FUNCTION tm_core_rrc_cell_info_dch_report_handler

DESCRIPTION
  This function handles the rrc cell info DCH report.

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_rrc_cell_info_dch_report_handler(tm_rrc_cell_info_dch_report_type *rrc_cell_info_p)
{
  /*Pass the cell info to protocol modules*/

  /*Pass it to SUPL module*/
  tm_rrc_iface_cell_info_dch_report(rrc_cell_info_p->cell_id_info, rrc_cell_info_p->status);

  /*Pass it to GM module*/
  gm_rrc_cell_info_dch_report(rrc_cell_info_p->cell_id_info, rrc_cell_info_p->status);

#ifdef FEATURE_CGPS_XTRA_T
  /*Pass it to XTRA-T*/
  tle_rrc_cell_info_dch_report(rrc_cell_info_p->cell_id_info, rrc_cell_info_p->status);
#endif

}
#endif /*FEATURE_WCDMA_GPS_CELL_ID_READING*/

/*===========================================================================
FUNCTION tm_core_is_agps_allowed_on_serv_sys

DESCRIPTION
Check if AGPS is allowed on a serving system

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
boolean tm_core_is_agps_allowed_on_serv_sys(cgps_SrvSystemType serv_sys)
{
  uint16 rat_mask = 0;

  switch (serv_sys)
  {
    case CGPS_SRV_SYS_CDMA:
      rat_mask = TM_CORE_NV_RAT_ENABLE_CDMA_MASK;
      break;
    case CGPS_SRV_SYS_HDR:
      rat_mask = TM_CORE_NV_RAT_ENABLE_HDR_MASK;
      break;
    case CGPS_SRV_SYS_GSM:
      rat_mask = TM_CORE_NV_RAT_ENABLE_GSM_MASK;
      break;
    case CGPS_SRV_SYS_WCDMA:
      rat_mask = TM_CORE_NV_RAT_ENABLE_WCDMA_MASK;
      break;
    case CGPS_SRV_SYS_LTE:
    case CGPS_SRV_SYS_WLAN:
      rat_mask = TM_CORE_NV_RAT_ENABLE_LTE_MASK;
      break;
    case CGPS_SRV_SYS_TDSCDMA:
      rat_mask = TM_CORE_NV_RAT_ENABLE_TDSCDMA_MASK;
      break;
    case CGPS_SRV_SYS_NR5G:
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Nr5G NV bit needs to be enabled");

    case CGPS_SRV_SYS_NONE:
    case CGPS_SRV_SYS_OTHER:
      rat_mask = TM_CORE_NV_RAT_ENABLE_ALL_MASK;
      break;
    default:
      rat_mask = 0;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "NV Enable bit mask %d Rat Mask %d Rat %d",
        TM_CORE_CONFIG_DD(agps_rat_pref).agps_rat_enable_bit_mask,
        rat_mask,
        serv_sys);
  if (TM_CORE_CONFIG_DD(agps_rat_pref).agps_rat_enable_bit_mask & rat_mask)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "AGPS on %d RAT allowed", serv_sys);
    return TRUE;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "AGPS on %d RAT not allowed", serv_sys);
  return FALSE;
}


/*===========================================================================
FUNCTION tm_core_is_umts_prefered

DESCRIPTION
Check if UMTS/LTE is prefered over other RAT's

INPUT PARAMS
None

OUTPUT PARAMS
None

RETURN VALUE
XTRA command status
===========================================================================*/
boolean tm_core_is_umts_prefered(void)
{
  if (TM_CORE_CONFIG_DD(agps_rat_pref).umts_lte_prefered)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "UMTS LTE prefered over other RAT", 0, 0, 0);
    return TRUE;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "UMTS LTE not prefered over other RAT", 0, 0, 0);
  return FALSE;
}

/*===========================================================================
FUNCTION tm_core_is_ni_standalone

DESCRIPTION
  This function checks if this is an 1x UP NI triggered session which has fallen
  back to standalone
  
DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_ni_standalone(void)
{
  tm_prtl_type active_prtl;
  active_prtl = tm_core_get_active_prtl_module(tm_core_info.session_info.queued_get_pos_param.mt_initiator);

  if ((tm_core_info.session_info.from_protocol == TM_PRTL_TYPE_STANDALONE) &&
      (active_prtl == TM_PRTL_TYPE_1X_UP) &&
      (tm_core_info.session_info.active_sess_handle == TM_SESS_HANDLE_1X_UP_NI))
    return TRUE;
  else
    return FALSE;
} /* tm_core_is_ni_standalone */

/*===========================================================================
FUNCTION tm_core_reset_location_service_in_progress

DESCRIPTION
  This function checks if reset location service is in progress
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_reset_location_service_in_progress(void)
{
  return tm_reset_location_service_in_progress;
}

/*===========================================================================
FUNCTION tm_core_reset_location_service_in_progress

DESCRIPTION
  This function checks if reset location service is in progress
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_set_reset_location_service_in_progress(boolean reset_location_service_in_progress)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Setting Reset Location Services in progress to %d", reset_location_service_in_progress, 0, 0);
  tm_reset_location_service_in_progress = reset_location_service_in_progress;
}

/*===========================================================================

FUNCTION tm_core_reset_location_service_done_handler

DESCRIPTION
  This function handles the reset locaiton service done indication

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_reset_location_service_done_handler(pdsm_pd_ext_reset_location_service_status_e_type *status)
{
  pdsm_ext_status_e_type e_ext_status_event = PDSM_EXT_STATUS_RESET_LOCATION_SERVICE;
  pdsm_ext_status_info_s_type *ext_status_info_type = NULL;

  ext_status_info_type = (pdsm_ext_status_info_s_type *)os_MemAlloc(sizeof(pdsm_ext_status_info_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == ext_status_info_type)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Cannot allocate memory for pdsm_ext_status_info_s_type in tm_core_reset_location_service_done_handler");
    return;
  }

  memset(ext_status_info_type, 0, sizeof(*ext_status_info_type));

  ext_status_info_type->ext_status_type = PDSM_EXT_STATUS_RESET_LOCATION_SERVICE;
  ext_status_info_type->ext_status_info.ext_reset_location_service_status_type = *status;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Reset Location Service Done Status %d", *status, 0, 0);

  /*Send Event that Reset Location Service is done*/
  sm_report_event_reset_location_service_done((uint32) * status);


  /*Send indication to PDAPI*/
  tm_pdapi_ext_status_event_callback(e_ext_status_event, ext_status_info_type);

  /*Record that reset location service is completed*/
  tm_core_set_reset_location_service_in_progress(FALSE);

  /*Clean up any ongoing session*/
  if (tm_core_session_active())
  {
    tm_core_abort_recover(TRUE, TM_STOP_REASON_ON_RESET_LOCATION_SERVICE);
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "tm_core_reset_location_service_done_handler: Not calling abort_recover as no active session present");
  }

  (void)os_MemFree((void **)&ext_status_info_type);
}

/*===========================================================================
FUNCTION tm_core_handle_engine_error_recovery_report

DESCRIPTION
  This function handles engine error recovery report (usually it'll be sent to
  QMI)
  
DEPENDENCIES

RETURN VALUE
  TRUE if success

SIDE EFFECTS

===========================================================================*/
boolean tm_core_handle_engine_error_recovery_report(gnss_engine_err_recovery_rpt *pErrReport)
{
  boolean v_ret_val = FALSE;
  tm_cm_phone_state_info_s_type phone_state_info;
  if (pErrReport == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL engine error recovery report");
    return FALSE;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Recovery:Reset type %d", pErrReport->reset_type);

  /*Get the phone state to check for E911 state*/
  memset(&phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&phone_state_info);

#if defined(__GRIFFON__) || !defined(FEATURE_CGPS_LBS_TASK)
  if ((tm_core_session_active()) &&
      ((eCQSVC_ENGINE_ERROR_RECOVERY_RESET_POSITION_ENGINE == pErrReport->reset_type) ||
       (eCQSVC_ENGINE_ERROR_RECOVERY_RESET_GPS_SUBFRAME_ALIGNMENT == pErrReport->reset_type) ||
       (eCQSVC_ENGINE_ERROR_RECOVERY_RESET_DECODED_TIME_INCONSISTENCY == pErrReport->reset_type)))
  {
    /*FULL error recovery; trigger CPI and time injection.*/
#ifdef FEATURE_CGPS_XTRA
    if (tm_xtra_data_validity_check(GPS_TIME_CHECK_SOURCE_PER_FIX))
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Recovery:sent time request to MGP");
      // Successfully sent time request to MGP, clear the flag
      tm_core_info.session_info.req_xtra_trig_by_agps = FALSE;
      v_ret_val = TRUE;
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Recovery:Failed to send time request to MGP");
    }
#endif /* FEATURE_CGPS_XTRA */

#if !defined ( FEATURE_GNSS_SA ) && !defined ( FEATURE_GNSS_LEAN_BUILD )
    if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
    {
      v_ret_val = tm_startCPIRequest(TRUE, TM_AUXTECH_REQ_ID_RECOVERY);
    }
    else
    {
      v_ret_val = tm_startCPIRequest(FALSE, TM_AUXTECH_REQ_ID_RECOVERY);
    }
#endif /* ! FEATURE_GNSS_LEAN_BUILD */

    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Error recovery report return %d", v_ret_val);
    return v_ret_val;
  }
  else
  {
    return TRUE;
  }
#else
  return gnss_CQmiSvcSendEngineErrorRecoveryReport(pErrReport);
#endif
}

/*===========================================================================
FUNCTION tm_core_handle_engine_dpo_status_report

DESCRIPTION
  This function handles engine DPO status report from MGP(usually it'll be sent to
  QMI)
  
DEPENDENCIES

RETURN VALUE
  TRUE if success

SIDE EFFECTS

===========================================================================*/
extern void tm_core_handle_engine_dpo_status_report(sm_DpoStatusReportStructType *p_DpoStatusReport)
{
  pdsm_ext_status_info_s_type   pd_ext_status_info;
  memset(&pd_ext_status_info, 0, sizeof(pdsm_ext_status_info_s_type));
  if (p_DpoStatusReport == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL DPO status report from MGP", 0, 0, 0);
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Rcvd DPO status report from MGP Engaged %x DisEngagement Reason %x", p_DpoStatusReport->dpo_status_mask, p_DpoStatusReport->dpo_disengage_reason, 0);
    pd_ext_status_info.ext_status_type = PDSM_EXT_STATUS_DPO_STATUS;
    memscpy(&pd_ext_status_info.ext_status_info.ext_dpo_status_report_type,
            sizeof(pd_ext_status_info.ext_status_info.ext_dpo_status_report_type),
            p_DpoStatusReport, sizeof(*p_DpoStatusReport));
    /*Send the DPO status indication to Geofence Module*/
    gm_update_dpo_status_indication(p_DpoStatusReport);
  }
  pd_ext_status_info.client_id =  tm_core_info.session_info.pd_param.client_id;
  tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_DPO_STATUS,
                                     &pd_ext_status_info);

}

/*===========================================================================
FUNCTION tm_core_handle_best_avail_pos

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  TRUE if best avail pos was requested
SIDE EFFECTS

===========================================================================*/
boolean tm_core_handle_best_avail_pos
(
   tm_core_get_best_avail_pos_param_s_type *p_get_best_avail_pos_param
   )
{
  tm_core_get_gps_state_param_s_type get_gps_state_param;

  if (p_get_best_avail_pos_param == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null input pointer", 0, 0, 0);
    return FALSE;
  }
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Calling tm_core_handle_best_avail_pos() for source %d ", p_get_best_avail_pos_param->source, 0, 0);

  get_gps_state_param.source = p_get_best_avail_pos_param->source;
  get_gps_state_param.cmd.p_cmd_type = p_get_best_avail_pos_param->cmd.p_cmd_type;
  tm_core_get_gps_state(&get_gps_state_param);
  return TRUE; /*lint !e506 */
}

/*===========================================================================
FUNCTION tm_core_handle_avail_wwan_pos

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  TRUE if avail WWAN pos was requested
SIDE EFFECTS

===========================================================================*/
boolean tm_core_handle_avail_wwan_pos
(
   tm_core_get_avail_wwan_pos_param_s_type *p_GetAvailWwanPosParam
   )
{
  tm_core_get_gps_state_param_s_type z_GetGpsStateParam;

  if (p_GetAvailWwanPosParam == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null input pointer", 0, 0, 0);
    return FALSE;
  }
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Calling tm_core_handle_avail_wwan_pos() for source %d", p_GetAvailWwanPosParam->source, 0, 0);

  z_GetGpsStateParam.source = p_GetAvailWwanPosParam->source;
  z_GetGpsStateParam.cmd.p_cmd_type = p_GetAvailWwanPosParam->cmd.p_cmd_type;
  tm_core_get_gps_state(&z_GetGpsStateParam);
  return TRUE; /*lint !e506 */
}

/*===========================================================================

FUNCTION tm_core_get_param_msg_handler

DESCRIPTION
  This function handles the get param ipc msg, and sends an ipc msg as a response
  with the param value retrieved.
  
DEPENDENCIES

RETURN VALUE TRUE/FALSE

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_param_msg_handler(void *p_Data)
{
  os_IpcMsgType             *ipc_msg_ptr = NULL;
  lsmp_get_msg_type         *get_msg = NULL;

  uint32  length = 0;
  boolean status = FALSE;
  uint8   i      = 0;

  lsmp_premium_services_param_type *premium_services = NULL;
  lsmp_device_config_param_s_type *device_config = NULL;

  if (!p_Data)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL input p_Data ptr", 0, 0, 0);
    return status;
  }

  get_msg = (lsmp_get_msg_type *)p_Data;

  switch (get_msg->param)
  {
    case LSMP_PREMIUM_SERVICES_TYPE:
      {
        length = sizeof(lsmp_premium_services_param_type);
        ipc_msg_ptr = os_IpcCreate(length, IPC_ALLOCATION_DYNAMIC, (uint32)get_msg->thread_id);
        if (!ipc_msg_ptr)
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "IPC message allocation failed", 0, 0, 0);
          return status;
        }
        premium_services = (lsmp_premium_services_param_type *)ipc_msg_ptr->p_Data;

        /* Map Premium Service NV value with correct SAP enablement value */
        if (PDSM_PA_PREMIUM_SVC_CFG_DISABLED == tm_core_info.config_info.premium_services_config[PDSM_PA_PREMIUM_SVC_SAP])
        {
          *premium_services = LSMP_PREMIUM_SERVICES_TYPE_DISABLE;
        }
        else if (PDSM_PA_PREMIUM_SVC_CFG_ENABLED_PREMIUM == tm_core_info.config_info.premium_services_config[PDSM_PA_PREMIUM_SVC_SAP])
        {
          *premium_services = LSMP_PREMIUM_SERVICES_TYPE_ENABLE_PREMIUM;
        }
        else
        {
          /* Default case is Enable Basic */
          *premium_services = LSMP_PREMIUM_SERVICES_TYPE_ENABLE_BASIC;
        }
      }
      break;

    case LSMP_DEVICE_CONFIG_TYPE:
      {
        length = sizeof(lsmp_device_config_param_s_type);
        ipc_msg_ptr = os_IpcCreate(length, IPC_ALLOCATION_DYNAMIC, (uint32)get_msg->thread_id);

        if (!ipc_msg_ptr)
        {
          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "IPC message allocation failed", 0, 0, 0);
          return status;
        }
        device_config = (lsmp_device_config_param_s_type *)ipc_msg_ptr->p_Data;
        device_config->lsmp_device_mode = (lsmp_device_mode_e_type)(tm_core_device_config.tm_cm_device_info);
        device_config->lsmp_overall_device_feature = tm_core_device_config.tm_cm_overall_device_feature;
        device_config->lsmp_no_of_sims = tm_core_device_config.no_of_sims;
        device_config->lsmp_no_of_active_subs = tm_core_device_config.no_of_active_subs;
        for (i = 0; i < tm_core_device_config.no_of_sims; i++)
        {
          device_config->lsmp_subs_feature[i] = tm_core_device_config.tm_cm_subs_feature[i];
          device_config->lsmp_subs_ue_mode[i] = tm_core_device_config.tm_cm_subs_ue_mode[i];
        }
      }
      break;

    default:
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unknown parameter type sent", 0, 0, 0);

      return status;
  }

  ipc_msg_ptr->q_MsgId  = get_msg->msg_id;
  status    = os_IpcSend(ipc_msg_ptr, (uint32)get_msg->thread_id);

  if (status == FALSE)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "Error in sending LSMP GET PARAM response msg_id=%d, to thread id=%d", ipc_msg_ptr->q_MsgId, ipc_msg_ptr->q_DestThreadId, 0);
    status = os_IpcDelete(ipc_msg_ptr);
    ipc_msg_ptr = NULL;
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "Sending LSMP GET PARAM response msg_id=%d, to thread id=%d", ipc_msg_ptr->q_MsgId, ipc_msg_ptr->q_DestThreadId, 0);
    status = TRUE;
  }

  return status;
}

/*===========================================================================
FUNCTION tm_core_enable_DpoStatusReport

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  TRUE is DPO Status Report was enabled

SIDE EFFECTS

===========================================================================*/
boolean tm_core_enable_DpoStatusReport(boolean enable)
{


#ifndef FEATURE_GNSS_LEAN_BUILD
  return mgp_EnableDpoStatusReport(enable);

#else
  return FALSE;
#endif

}


/*===========================================================================
FUNCTION tm_core_handle_inject_ni_msg

DESCRIPTION

DEPENDENCIES

RETURN VALUE
  TRUE if NI message has been injected successfully

SIDE EFFECTS

===========================================================================*/
boolean tm_core_handle_inject_ni_msg(pdsm_ni_message_s_type *ni_msg)
{
  boolean bRetVal = TRUE;
  prtl_event_u_type  *event_payload_ptr = NULL;

  if (ni_msg == NULL || ni_msg->len <= 0 || ni_msg->ni_data == NULL)
  {
    return FALSE;
  }

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM got NI message injection (type=%d len=%d)", ni_msg->type, ni_msg->len, 0);

  /* allocate buffer for sending event to protocol submodule */
  event_payload_ptr = (prtl_event_u_type *)pdsm_getbuf(sizeof(prtl_event_u_type));
  if (event_payload_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can't get buffer", 0, 0, 0);
    bRetVal = FALSE;
  }
  else
  {
    switch (ni_msg->type)
    {
      case NI_MESSAGE_SUPL:
        event_payload_ptr->sms_event_payload.length = ni_msg->len;
        event_payload_ptr->sms_event_payload.data = ni_msg->ni_data; // this is a pointer to buffer (from pdsm_getbuf)

        if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
        {
          (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
             PRTL_EVENT_TYPE_SMS, event_payload_ptr);
        }
        break;

        /* probably can add V1/V2 NI message here in the future */
      default:
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unknown NI message type %d", ni_msg->type, 0, 0);
        bRetVal = FALSE;

    } /* switch  */

    (void)pdsm_freebuf((char *)event_payload_ptr);  // free this event buffer
  }

  /* free buffer if something is wrong, otherwise buffer will be freed by PDCOMM WMS */
  if (bRetVal == FALSE && ni_msg->ni_data != NULL)
  {
    os_MemFree((void **)&(ni_msg->ni_data));  // free this NI message buffer
    ni_msg->ni_data = NULL;
  }

  return bRetVal;
}

boolean tm_core_convert_wifi_on_demand_scan_to_wiper_ap_set
(pdsm_wifi_on_demand_info_list_s_type *p_wifi_on_demand_msg,
 t_wiper_ap_set_struct_type *pz_wifi_scan_info)
{
  uint8 u_ap_index = 0;

  if ((NULL == p_wifi_on_demand_msg) ||
      (NULL == pz_wifi_scan_info)
      )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null pointers passed while converting Wifi scan info", 0, 0, 0);
    return FALSE;
  }

  pz_wifi_scan_info->num_of_aps = p_wifi_on_demand_msg->z_wifi_ap_scan_info.q_num_wifi_aps;
  pz_wifi_scan_info->e_wifi_scan_src = PDSM_WIFI_SCAN_SRC_HLOS;
  if ((pz_wifi_scan_info->num_of_aps > 0) &&
      ((NULL == p_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr) ||
       (NULL == p_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr)
       )
      )
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null Wifi ap ptr passed for a non empty scan", 0, 0, 0);
    pz_wifi_scan_info->num_of_aps = 0;
  }

  for (u_ap_index = 0; u_ap_index < pz_wifi_scan_info->num_of_aps; u_ap_index++)
  {
    pz_wifi_scan_info->ap_info[u_ap_index].meas_age = -1;
    memscpy((void *)pz_wifi_scan_info->ap_info[u_ap_index].mac_addr,
            WIPER_MAC_ADDR_LENGTH,
            p_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr[u_ap_index].mac_address,
            WIPER_MAC_ADDR_LENGTH);
    if (p_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr[u_ap_index].wifiInfoMask &
           LOC_ENG_WIFI_APINFO_MASK_AP_RSSI_V01)
    {
      pz_wifi_scan_info->ap_info[u_ap_index].rssi =
         p_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr[u_ap_index].ap_rssi;

    }
    pz_wifi_scan_info->ap_info[u_ap_index].freq =
       p_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[u_ap_index].frequency;

    memscpy((void *)pz_wifi_scan_info->ap_info[u_ap_index].ssid,
            WIPER_SSID_STR_LENGTH,
            p_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[u_ap_index].ssid,
            QMI_LOC_MAX_WIFI_AP_SSID_STR_LENGTH_V01);

  }
  /*Fill channel info*/
  /* The wifi scans from HLOS are assumed to be FULL scans */
  pz_wifi_scan_info->z_wlan_channel_info.q_2_4_Ghz_channel_mask = 0xFFFF;

  return TRUE;
}

/*===========================================================================
FUNCTION  tm_ack_inject_wifi_scans

DESCRIPTION
  Send an Acknowledgement for wifi report messages.

DEPENDENCIES
  This function should be called before re-request PDSM_PD_EVENT_REQUEST_WIFI_INFO_ON_DEMAND_INJECTION message is send

PARAMETERS
  e_ackStatus - Ack status. 
  l_clientId - Client id. 

RETURN VALUE
  none
SIDE EFFECTS
  None
===========================================================================*/
void tm_ack_inject_wifi_scans(locEngStatusEnumT_v01 e_ackStatus, pdsm_client_id_type l_clientId)
{
  tm_pdapi_pd_event_s_type  *pz_event_info = NULL;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_ack_inject_wifi_scans called with status %d", e_ackStatus);

  /* Use dynamic allocation to save call stack memory */
  pz_event_info = (tm_pdapi_pd_event_s_type *)os_MemAlloc(sizeof(tm_pdapi_pd_event_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_event_info)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Could not allocate memory", 0, 0, 0);
    return;
  }

  memset(pz_event_info, 0, sizeof(*pz_event_info));
  pz_event_info->e_pd_event = PDSM_PD_EVENT_ACK_WIFI_INFO_ON_DEMAND_INJECTION;
  pz_event_info->pd_event_data.pd_info_type.client_id = l_clientId;

  pz_event_info->pd_event_data.pd_info_type.pd_info.wifi_inj_ack.status = e_ackStatus;
  tm_pdapi_pd_event_callback(pz_event_info, TM_PDAPI_SESS_TYPE_NONE);

  /* free this event buffer */
  os_MemFree((void **)&pz_event_info);
}

/*===========================================================================
FUNCTION tm_core_handle_inject_wifi_scans

DESCRIPTION
  Handle the WiFi measurement report from APSS/LocMw. This function is called
  to report both free and on-demand wifi scan reports.

DEPENDENCIES

RETURN VALUE
  TRUE if WiFi measurement report has been injected successfully

SIDE EFFECTS

===========================================================================*/
boolean tm_core_handle_inject_wifi_scans(pdsm_wifi_on_demand_info_list_s_type *pz_wifi_on_demand_msg)
{
  boolean                    bRetVal           = FALSE;
  prtl_event_u_type          *pz_event_payload = NULL;
  t_wiper_ap_set_struct_type z_wifi_scan_info  = { 0 };
  boolean                    v_reportValid     = FALSE;
  locEngStatusEnumT_v01      e_ackStatus       = eLOC_ENG_GENERAL_FAILURE_V01;
  pdsm_client_id_type        l_clientId        = -1;
  uint32                     q_i;
  uint32                     q_validIndex   = 0;
  uint8_t                    allZeroMacId[6] = { 0 };

  if (pz_wifi_on_demand_msg == NULL || pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr == NULL
      || pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null pointer passed into tm_core_handle_inject_wifi_scans", 0, 0, 0);
    return (FALSE);
  }

  MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_MED, "WiFi scans. Aps %u, onDemand %d, first %d, first add %d, err %d",
        pz_wifi_on_demand_msg->z_wifi_ap_scan_info.q_num_wifi_aps, pz_wifi_on_demand_msg->v_on_demand_scan,
        NULL == pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr,
        NULL == pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr,
        pz_wifi_on_demand_msg->e_errorCause);

  /* Check for validity of report */
  if (((pz_wifi_on_demand_msg->z_wifi_ap_scan_info.q_num_wifi_aps > 0) &&
       (pz_wifi_on_demand_msg->z_wifi_ap_scan_info.q_num_wifi_aps <= QMI_LOC_WIFI_MAX_REPORTED_APS_PER_MSG_V01))
      ||
      (pz_wifi_on_demand_msg->e_errorCause != eLOC_ENG_WLAN_AP_ERR_NONE_V01)
      )
  {
    v_reportValid = TRUE;
    /* In case of valid wifi report data, send ack status as success */
    e_ackStatus = eLOC_ENG_SUCCESS_V01;
  }

  /* Even though report data is invalid send ack status as successful for free scan */
  if (FALSE == pz_wifi_on_demand_msg->v_on_demand_scan)
  {
    e_ackStatus = eLOC_ENG_SUCCESS_V01;
  }

#ifndef FEATURE_GNSS_LEAN_BUILD
#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
  /* In case of on-demand wifi scan report, get the Wifi Client Id used in the last on-demand wifi scan request. */
  if (TRUE == pz_wifi_on_demand_msg->v_on_demand_scan)
  {
    l_clientId = tm_wifiClientIdGet();
  }
#endif //FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
#endif /* ! FEATURE_GNSS_LEAN_BUILD */

  /* Send an ack for this wifi ap report*/
  tm_ack_inject_wifi_scans(e_ackStatus, l_clientId);

  /* Exit the function in case of invalid report */
  if (FALSE == v_reportValid)
  {
    return (FALSE);
  }

  /*Sent to protocols module only if on demand scans and if SUPL-MLID is enabled */
  if ((pz_wifi_on_demand_msg->v_on_demand_scan)
#ifndef FEATURE_GNSS_LEAN_BUILD
#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
      && (TM_AUXTECH_REQ_ID_SUPLMLID == tm_wifiReqIdGet())
#endif /* FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS */
#endif /* ! FEATURE_GNSS_LEAN_BUILD */
      )
  {
    /* allocate buffer for sending event to protocol submodule */
    pz_event_payload = (prtl_event_u_type *)os_MemAlloc(sizeof(prtl_event_u_type), OS_MEM_SCOPE_TASK);
    if (NULL == pz_event_payload)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can't get buffer", 0, 0, 0);
    }
    else
    {
      pz_event_payload->wifi_scan_event_payload.z_wifi_scan_info = *(pz_wifi_on_demand_msg);

      if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
      {
        (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
           PRTL_EVENT_TYPE_WIFI_SCAN_RESULT, pz_event_payload);
      }
      /* free the event buffer */
      os_MemFree((void **)&pz_event_payload);
      bRetVal = TRUE;
    }
  }
  else
  {
    bRetVal = TRUE;
  }

#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
  {
    uint32                     e_wifiReqId       = TM_AUXTECH_REQ_ID_NONE;

    e_wifiReqId = tm_wifiReqIdGet();

    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Wifi Requestor ID: %d", e_wifiReqId);
    /*Rearrange pz_wifi_on_demand_msg, to keep the info corresponding to valid mac addresses on top*/
    for (q_i = 0; q_i < pz_wifi_on_demand_msg->z_wifi_ap_scan_info.q_num_wifi_aps; ++q_i)
    {
      if (memcmp(allZeroMacId, pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr[q_i].mac_address, sizeof(allZeroMacId)) != 0)
      {
        pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr[q_validIndex] = pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_ptr[q_i];
        pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_validIndex] = pz_wifi_on_demand_msg->z_wifi_ap_scan_info.first_wifi_ap_add_info_ptr[q_i];
        q_validIndex++;
      }
    }
    /*Update the q_num_wifi_aps with the new valid number of Wifi APs*/
    pz_wifi_on_demand_msg->z_wifi_ap_scan_info.q_num_wifi_aps = q_validIndex;
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Valid Wifi APs after filtering: %d", pz_wifi_on_demand_msg->z_wifi_ap_scan_info.q_num_wifi_aps);

#ifdef FEATURE_GNSS_LPPE
    /* Cache the wifi scan results, choose the correct requestor ID.*/
    if ((TM_AUXTECH_REQ_ID_LPPE_UP == e_wifiReqId) ||
        (TM_AUXTECH_REQ_ID_LPPE_CP == e_wifiReqId))
    {
      /* Cache the wifi scan results*/
      tm_cacheWifiScanReports(*pz_wifi_on_demand_msg, e_wifiReqId);
    }
#endif /*FEATURE_GNSS_LPPE*/

    if (TM_AUXTECH_REQ_ID_SUPLWLAN == e_wifiReqId)
    {
      tm_cacheWifiScanReports(*pz_wifi_on_demand_msg, e_wifiReqId);
    }
  }
#endif //FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS

  /*Inject to Geofencing module*/
  if (tm_core_convert_wifi_on_demand_scan_to_wiper_ap_set(pz_wifi_on_demand_msg, &z_wifi_scan_info))
  {
    gm_inject_wifi_scan_indication(&z_wifi_scan_info);
  }

  return (bRetVal);
}



/*===========================================================================

FUNCTION tm_core_external_cell_db_proc

DESCRIPTION
  This function is called by TmCore's IPC processor to inform TmCore of 
  an incoming external cell database update message. 
  
DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/

boolean tm_core_external_cell_db_proc(pdsm_cell_info_s_type *external_cell_db_info_ptr)
{
  prtl_event_u_type  *event_payload_ptr = NULL;
  boolean ret_val = FALSE;

  //Update tm_cm with external RAT changes
  tm_cm_process_external_srv_changed(external_cell_db_info_ptr);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM got external CellDB update", 0, 0, 0);


  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "External cell info: MCC = %d, MNC = %d, LAC = %d",
        external_cell_db_info_ptr->cellInfo.gsmCell.MCC,
        external_cell_db_info_ptr->cellInfo.gsmCell.MNC,
        external_cell_db_info_ptr->cellInfo.gsmCell.LAC);

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "External cell info: CID = %d, serviceStatus = %d, RAT = %d",
        external_cell_db_info_ptr->cellInfo.gsmCell.CID,
        external_cell_db_info_ptr->serviceStatus,
        external_cell_db_info_ptr->activeRAT);


  /* allocate buffer for sending event to protocol submodule */
  event_payload_ptr = (prtl_event_u_type *)pdsm_getbuf(sizeof(prtl_event_u_type));
  if (event_payload_ptr == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Can't get buffer", 0, 0, 0);
    return FALSE;
  }

  event_payload_ptr->cell_db_event_payload.cell_db_event = PRTL_EXTERNAL_CELL_DB_UPD;
  event_payload_ptr->cell_db_event_payload.cell_db_event_payload.external_modem_cell_db_payload.cellInfo =
     external_cell_db_info_ptr->cellInfo;
  event_payload_ptr->cell_db_event_payload.cell_db_event_payload.external_modem_cell_db_payload.serviceStatus =
     external_cell_db_info_ptr->serviceStatus;
  event_payload_ptr->cell_db_event_payload.cell_db_event_payload.external_modem_cell_db_payload.activeRAT =
     external_cell_db_info_ptr->activeRAT;


  if (tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp != NULL)
  {
    (void)tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp(
       PRTL_EVENT_TYPE_CELL_DB,
       event_payload_ptr);
    ret_val = TRUE;
  }
  else
  {
    ret_val = FALSE;
  }

  (void)pdsm_freebuf((char *)event_payload_ptr);

  return ret_val;

}

/*===========================================================================

FUNCTION tm_core_api_callback_handler

DESCRIPTION
  This function handles calling the client callbacks

DEPENDENCIES

RETURN VALUE TRUE/FALSE

SIDE EFFECTS

===========================================================================*/

boolean tm_core_api_callback_handler(const tm_api_notifier_type *p_api_notifier)
{

  uint8 errCode = 0;
  uint8 cb_index;
  static uint8 broadcast_counter = 0;
  static uint8 tm_index = 0;
  uint8 cb_bitmap = 0;
  const uint8 CLIENT_INDEX_SHIFT_VALUE = 4;
  const uint8 BROADCAST_SHIFT_VALUE = 6;
  const uint8 TM_SHIFT_VALUE = 7;

  if (p_api_notifier)
  {
    if (p_api_notifier->broadcast)
    {
      if (broadcast_counter == 0)
      {
        /* Create a high priority timer once for calling api callbacks */
        tm_core_info.tm_task_info.api_callback_timer =
           os_TimerCreateInCgpsTimerGroup((uint32)TM_CORE_TIMER_ID_API_CALLBACK, (uint32)THREAD_ID_SM_TM);
      }
      broadcast_counter++;

      /* Call all callbacks in the array if registered */
      for (cb_index = 0; cb_index < TM_UNSUPPORTED_CLIENT_INDEX; cb_index++)
      {
        if (pz_SmNotifierCb[cb_index] != NULL)
        {
          pz_SmNotifierCb[cb_index]();

          /* Log the call back status */
          /* bit 0-3 for timer, bit 4-5 for client index, bit 6 for broadcast or not, bit 7 for TM/GM */
          cb_bitmap |= tm_cb_timer_counter;
          cb_bitmap |= (cb_index << CLIENT_INDEX_SHIFT_VALUE);
          cb_bitmap |= ((uint8)p_api_notifier->broadcast) << BROADCAST_SHIFT_VALUE;
          cb_bitmap |= ((uint8)1) << TM_SHIFT_VALUE;

          if (tm_index < GPS_MAX_CALLBACK_CLIENTS)
          {
            tm_call_back_log[tm_index] = cb_bitmap;
            tm_index++;
          }
          else
          {
            errCode = 3;
          }

          /* Set to NULL after callback */
          pz_SmNotifierCb[cb_index] = NULL;
        }
      }

      if (os_TimerStart(tm_core_info.tm_task_info.api_callback_timer, TM_CORE_API_CALLBACK_INTERVAL_MS, 0) == FALSE)
      {
        ERR_FATAL("Failed start api_callback timer!", 0, 0, 0);
        return FALSE;
      }
    }
    else
    {
      /* Call clients callback right away since we receive this IPC only if TM is ready */
      p_api_notifier->pNotifierCb();

      /* Log the call back status */
      /* bit 0-3 for timer (0xF if not broadcast), bit 4-5 for client index, bit 6 for broadcast or not, bit 7 for TM/GM */
      cb_bitmap |= 0x0F;
      cb_bitmap |= (p_api_notifier->clientIndex << CLIENT_INDEX_SHIFT_VALUE);
      cb_bitmap |= ((uint8)p_api_notifier->broadcast) << BROADCAST_SHIFT_VALUE;
      cb_bitmap |= ((uint8)1) << TM_SHIFT_VALUE;

      if (tm_cb_timer_counter < TM_CORE_MAX_NUM_API_CALLBACKS)
      {
        /* We are here only during initial 10 seconds after bootup.
                  At this time, the timer is still running. Log it in buffer to display the callback logs after timer is done*/
        if (tm_index < GPS_MAX_CALLBACK_CLIENTS)
        {
          tm_call_back_log[tm_index] = cb_bitmap;
          tm_index++;
        }
        else
        {
          errCode = 2;
        }
      }
      else
      {
        /* We are here after we are done running timer after bootup.
               Timer is done, display the callback log now */
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Callback Log report for TM: 0x%x", cb_bitmap, 0, 0);
      }

      /* Set pz_SmNotifierCb array index to NULL after callback */
      pz_SmNotifierCb[p_api_notifier->clientIndex] = NULL;
    }
  }
  else
  { /* p_api_notifier == NULL */
    errCode = 1;
  }

  if (errCode > 0)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "tm_core_api_callback_handler failed with error code = %d", errCode, 0, 0);
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}


/*===========================================================================
FUNCTION tm_core_start_emergency_session

DESCRIPTION
  This function starts a new GPS session with TM-LM for a emergency positioning
  at the start of the emergency call. This is to basically do an early receiver
  ON because, there might be a UP/CP GPS session from network. Once the network
  sends a SUPL/CP GPS session, the session will be updated.

DEPENDENCIES

RETURN VALUE
  TRUE if session continue ok

SIDE EFFECTS

===========================================================================*/

boolean tm_core_start_emergency_session(void)
{
  boolean                                   accepted = 0;
  tm_sess_req_param_u_type                  req_param;
  uint8                                     acc_thresh = 0;
  tm_sess_req_start_param_s_type            *p_start_params = NULL;


  /*
   *  Create a pointer to the START SESSION request parameters to make
   *  the code a bit cleaner and easier to read later in this function.
   */
  p_start_params = &req_param.start_param;

  /*
   *  Start out with an accuracy threshold of 0 meters when starting
   *  an MT session so that the MGP receiver will stay on indefinitely.
   *  
   *  The accuracy threshold will get set to the desired value later
   *  when we receive the Request Location message from the E-SMLC.
   */
  acc_thresh = 0;

#ifdef FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS
  /* If both LPP UP and LPP CP disabled, OR
   * LTE MSB disabled 
   * dont do early start */
  if (((tm_core_get_lte_lpp_up_enable() == FALSE) &&
       (tm_core_get_lpp_cp_enable() == FALSE)) ||
      (tm_umts_common_is_lte_msb_enabled() == FALSE))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Early rcvr not started in emergency on LTE due to NV", 0, 0, 0);
    return TRUE;
  }
#endif /* FEATURE_SUPPORT_UMTS_GPS_PROTOCOLS */

  p_start_params->op_mode = TM_SESS_OPERATION_MODE_MSB;
  p_start_params->op_req  = TM_OP_REQ_LR | TM_OP_REQ_AIDING_DATA_STATUS;
  p_start_params->lr_qos  = TM_MAX_EMERGENCY_SESSION_TIMEOUT;

  /*
   *  Set up parameters that are common to all modes
   */
  p_start_params->abort_curr_sess     = TRUE; /* ayt_todo */
  p_start_params->num_fixes           = 1;
  p_start_params->tbf_ms              = 1000;
  p_start_params->accuracy_threshold  = acc_thresh;
  p_start_params->ppm_qos             = 0;
  p_start_params->prm_prq             = 0;
  p_start_params->dynamic_qos_enabled = FALSE;
  p_start_params->multi_report_msa = FALSE;

  /*
   *  Tell TM-Core to start a session now. Use a dummy protocol type. THis protocol type will
      be changed once the network starts a LPP CP or UP session.
   */
  accepted = tm_sess_req(TM_PRTL_TYPE_E911,
                         TM_SESS_HANDLE_E911,
                         TM_SESS_REQ_START,
                         &req_param);

  if (accepted)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_CORE: tm_sess_req() STARTED", 0, 0, 0);
    tm_core_send_pdapi_emerg_sess_type(PDSM_PD_EXT_STATUS_EMERG_SESS_START);

  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_CORE: tm_sess_req() REJECTED", 0, 0, 0);
  }

  return (accepted);
}

/*===========================================================================

FUNCTION tm_core_ecid_evt_handler

DESCRIPTION
  This function receives ECID measurements from the ECID interface and sends
  them to the appropriate protocol
 

RETURN VALUE
  None 

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_ecid_evt_handler(gnss_lpp_ecid_meas_info_type *p_ecid_measurement_info)
{
  int prtlTableIndex;
  prtl_sess_info_param_u_type   *sess_info_param_ptr = NULL;
  boolean retVal = FALSE;
  tm_sess_handle_type sess_handle = tm_core_info.session_info.active_sess_handle;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Received ECID measurements from ML1", 0, 0, 0);
  if (p_ecid_measurement_info == NULL)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No ECID measurment info", 0, 0, 0);
    return;
  }
  prtlTableIndex = tm_core_get_prtl_idx_by_type(p_ecid_measurement_info->q_sess_type);

  if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
  {

    sess_info_param_ptr = (prtl_sess_info_param_u_type *)pdsm_getbuf(sizeof(prtl_sess_info_param_u_type));

    if (sess_info_param_ptr == NULL)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Failed to get buffer for sess_info_param_ptr", 0, 0, 0);
      tm_core_abort_recover(TRUE, TM_STOP_REASON_GENERAL_ERROR); /*lint !e506 !e730 */
      return;
    }

    /* send the data to the protocol regardless of if this is a TM session */
    /* Set reason to general error for now, if return value */
    if (tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp)
    {
      memscpy((void *)&(sess_info_param_ptr->ecid_data),
              sizeof(sess_info_param_ptr->ecid_data),
              &p_ecid_measurement_info->z_meas_info,
              sizeof(p_ecid_measurement_info->z_meas_info));

      if (tm_core_info.session_info.active_sess_handle != TM_SESS_HANDLE_NONE)
      {
        tm_core_info.session_info.op_requested &= ~TM_OP_REQ_ECID;
      }

      sess_handle = tm_core_get_ni_sess_handle();
      retVal = tm_core_info.prtl_func_cb_table[prtlTableIndex].sess_info_fp(sess_handle,
                                                                            PRTL_SESS_INFO_ECID,
                                                                            sess_info_param_ptr);
      if (retVal == FALSE)
      {
        MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Could not send the data to protocol", 0, 0, 0);
      }
    }

    (void)pdsm_freebuf((char *)sess_info_param_ptr);
  }
}

/*===========================================================================

FUNCTION tm_core_retrieve_imsi

DESCRIPTION
  This function is called by TLE to request for IMSI retrieval

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_retrieve_imsi(tm_RetrieveIMSIType *tm_retrieve_imsi_info_ptr)
{
  cgps_IMSIType z_IMSIInfo;
  tm_RetrieveIMSICb pCallback = tm_retrieve_imsi_info_ptr->pz_RetrieveIMSICb;

  memset(&z_IMSIInfo, 0x0, sizeof(z_IMSIInfo));

  z_IMSIInfo.sub = tm_retrieve_imsi_info_ptr->sub;

  if (!tm_sim_retrieve_imsi(&z_IMSIInfo))
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "IMSI retrieval failed", 0, 0, 0);
  }
  else
  {
    if (NULL != pCallback)
    {
      (*pCallback)(&z_IMSIInfo);
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Null callback passed", 0, 0, 0);
    }
  }
}

/*===========================================================================

FUNCTION tm_copy_prm_to_sm_meas_struct

DESCRIPTION
  This function copies the ME provided PRM data to SM GNSS Measurement report struct.

DEPENDENCIES

RETURN VALUE
  Return GPS or GLONASS measurement index upon success, else return invalid if
  PRM data isn't of GPS or GLONASS type.
SIDE EFFECTS

===========================================================================*/
int32 tm_copy_prm_to_sm_meas_struct(const sm_GpsMeasRptStructType *const p_prm_data,
                                    SmGnssMeasReportStructType *p_gnss_meas_report,
                                    boolean const cv_StatusCheckNeeded)
{
  if ((p_prm_data != NULL) && (p_gnss_meas_report != NULL))
  {
    gnss_MeasBlkSrcEnumType e_System = sm_util_get_master_constellation(p_prm_data->z_MeasBlk.z_GnssHeader.e_MeasBlkSrc);
    p_gnss_meas_report->m.s_StatusCheckNeeded = cv_StatusCheckNeeded;

    if (GNSS_MEAS_BLK_SRC_GPS == e_System)
    {
      /* Set measurement report bitmap to indicate GPS meas is copied over (below) */
      p_gnss_meas_report->u_meas_reports_bmap |= SM_GNSS_MEAS_REPORTS_BMAP_GPS;

      memset(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS], 0,
             sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS]));
      sm_GnssMeas_memscpy(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS],
                          sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GPS]),
                          p_prm_data);

      return SM_GNSS_MEAS_SYSTEM_GPS;
    }
    else if (GNSS_MEAS_BLK_SRC_GLO == e_System)
    {
      /* Set measurement report bitmap to indicate GLONASS meas is copied over (below) */
      p_gnss_meas_report->u_meas_reports_bmap |= SM_GNSS_MEAS_REPORTS_BMAP_GLONASS;

      memset(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS], 0,
             sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS]));
      sm_GnssMeas_memscpy(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS],
                          sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GLONASS]),
                          p_prm_data);

      return SM_GNSS_MEAS_SYSTEM_GLONASS;
    }
    else if (GNSS_MEAS_BLK_SRC_BDS == e_System)
    {
      /* Set measurement report bitmap to indicate BDS meas is copied over (below) */
      p_gnss_meas_report->u_meas_reports_bmap |= SM_GNSS_MEAS_REPORTS_BMAP_BDS;

      memset(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_BDS], 0,
             sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_BDS]));
      sm_GnssMeas_memscpy(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_BDS],
                          sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_BDS]),
                          p_prm_data);

      return SM_GNSS_MEAS_SYSTEM_BDS;
    }
    else if (GNSS_MEAS_BLK_SRC_GAL == e_System)
    {
      /* Set measurement report bitmap to indicate GAL meas is copied over (below) */
      p_gnss_meas_report->u_meas_reports_bmap |= SM_GNSS_MEAS_REPORTS_BMAP_GAL;

      memset(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GAL], 0,
             sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GAL]));
      sm_GnssMeas_memscpy(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GAL],
                          sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_GAL]),
                          p_prm_data);

      return SM_GNSS_MEAS_SYSTEM_GAL;
    }
    else if (GNSS_MEAS_BLK_SRC_QZSS_SBAS == e_System)
    {
      /* Set measurement report bitmap to indicate QZSS/SBAS meas is copied over (below) */
      p_gnss_meas_report->u_meas_reports_bmap |= SM_GNSS_MEAS_REPORTS_BMAP_QZSS;

      memset(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_QZSS], 0,
             sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_QZSS]));
      sm_GnssMeas_memscpy(&p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_QZSS],
                          sizeof(p_gnss_meas_report->z_meas_reports[SM_GNSS_MEAS_SYSTEM_QZSS]),
                          p_prm_data);

      return SM_GNSS_MEAS_SYSTEM_QZSS;
    }
  }
  return SM_GNSS_MEAS_SYSTEM_INVALID;
}

/*===========================================================================
FUNCTION tm_core_send_pdapi_emerg_sess_type

DESCRIPTION
  This function sends a PDAPI event of the type of ongoing GPS session during
  emergency positioning

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_send_pdapi_emerg_sess_type(pdsm_ext_status_emerg_sess_e_type sess_type)
{
  pdsm_ext_status_e_type e_ext_status_event = PDSM_EXT_STATUS_EMERG_POS_SESSION_TYPE;
  pdsm_ext_status_info_s_type ext_status_info_type;

  memset((void *)&ext_status_info_type, 0, sizeof(pdsm_ext_status_info_s_type));
  ext_status_info_type.client_id = pdsm_ni_client_id_get();
  ext_status_info_type.ext_status_type = PDSM_EXT_STATUS_EMERG_POS_SESSION_TYPE;
  ext_status_info_type.ext_status_info.ext_emerg_sess_type.sess_type = sess_type;
  /*Send indication to PDAPI*/

  if ((sess_type == PDSM_PD_EXT_STATUS_EMERG_SESS_START) && (b_emerg_sess_start_sent == FALSE))
  {
  tm_pdapi_ext_status_event_callback(e_ext_status_event, &ext_status_info_type);
    b_emerg_sess_start_sent = TRUE;
}
  if (sess_type != PDSM_PD_EXT_STATUS_EMERG_SESS_START)
  {
    tm_pdapi_ext_status_event_callback(e_ext_status_event, &ext_status_info_type);
  }
}


/*===========================================================================
FUNCTION tm_core_is_supl_ni_proc

DESCRIPTION
  This function checks if this is an SUPL NI trigger should be reported to AP always

DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_supl_ni_proc(void)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_CORE: SUPL NI setting %d", tm_core_info.config_info.b_supl_notify_to_ap, 0, 0);
  return (tm_core_info.config_info.b_supl_notify_to_ap);
}

/*===========================================================================
FUNCTION tm_core_mtlr_auto_accept

DESCRIPTION
  This function checks if this is an MTLR should be accepted without AP notification
  during emergency

DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
boolean tm_core_mtlr_auto_accept(void)
{
  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "TM_CORE: MTLR setting %d", tm_core_info.config_info.b_auto_accept_mtlr, 0, 0);
  return (tm_core_info.config_info.b_auto_accept_mtlr);
}

/*===========================================================================

FUNCTION tm_core_get_gm_engine_config

DESCRIPTION
  This function returns the GM Engine configuration

RETURN VALUE
  The timeout value in secs.

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_get_gm_engine_config(geofence_engine_config_s_type **gm_config)
{
  *gm_config = &(tm_core_info.config_info.gm_config);
}


/*===========================================================================
FUNCTION tm_core_set_ni_resp_pending

DESCRIPTION
  This function checks if this is an SUPL NI trigger should be reported to AP always
  
DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
void tm_core_set_ni_resp_pending(boolean b_pending)
{
  tm_core_info.b_pending_ni_resp_from_client =  b_pending;
}


/*===========================================================================
FUNCTION tm_core_is_ni_resp_pending

DESCRIPTION
  This function checks if this is an SUPL NI trigger should be reported to AP always
  
DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_ni_resp_pending(void)
{
  return (tm_core_info.b_pending_ni_resp_from_client);
}

/*===========================================================================
FUNCTION tm_core_get_lpp_periodic_end_flag

DESCRIPTION
  This function checks if LPP periodic session should end after emergency call endss
  
DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
extern boolean tm_core_get_lpp_periodic_end_flag(void)
{
  return (tm_core_info.config_info.b_end_lpp_periodic);
}

/********************************************************************
*
* tm_handle_ota_reset
*
* Function description:
*  Handles the OTA GPS reset and OTDOA reset messages
*
* Parameters: 
*   prtlEvent: Protocol Event type 
*   e_as_id  : Corresponding Subscription ID.
* Return:
*  none
*  
*********************************************************************
*/
void tm_handle_ota_reset(prtl_event_e_type const prtlEvent, sys_modem_as_id_e_type const e_as_id)
{
  tm_cm_phone_state_info_s_type phone_state_info;
  pdsm_delete_parms_type        delete_param;

  if ((PRTL_EVENT_TYPE_LTE_OTA_GPS_RESET != prtlEvent) &&
      (PRTL_EVENT_TYPE_LTE_OTA_OTDOA_RESET != prtlEvent) &&
      (PRTL_EVENT_TYPE_UMTS_OTA_GPS_RESET != prtlEvent))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unsupported event %d for OTA RESET", prtlEvent);
    return;
  }

  memset((void *)&phone_state_info, 0, sizeof(phone_state_info));
  /* Query TM-CM module to get current acquired system */
  tm_cm_iface_get_phone_state(&phone_state_info);

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "RESET initiated for OTA/OTDOA type: %d for srv_system %d",
        prtlEvent, phone_state_info.srv_system);
  if ((phone_state_info.e911_state != TM_CM_E911_STATE_NONE) &&
      (tm_core_session_active()))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM_CORE: OTA/OTDOA Reset during emergency not allowed");
    return;
  }

  if (tm_core_session_active())
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "OTA/OTDOA Reset during session. Abort ongoing session");
    tm_core_abort_recover(TRUE, TM_STOP_REASON_PDE_END_SESSION);
  }

  if ((PRTL_EVENT_TYPE_LTE_OTA_GPS_RESET == prtlEvent) ||
      (PRTL_EVENT_TYPE_UMTS_OTA_GPS_RESET == prtlEvent))
  {
    if (tm_core_info.e_cached_rcvr_state != C_GPS_RCVR_STATE_OFF)
    {
      mgp_ReceiverUpdate(MGP_RCVR_COMMAND_FORCED_IDLE, NULL);
      mgp_ReceiverUpdate(MGP_RCVR_COMMAND_FORCED_OFF, NULL);
    }
    memset(&delete_param, 0, sizeof(pdsm_delete_parms_type));

    tm_core_ota_reset_notify_event(prtlEvent);
    tm_core_delete_inject_aiding_data(&delete_param, FALSE, TRUE, PDSM_PA_DELETE_PARAMS_EX);

    /* For LTE OTA GPS Reset, also refresh SIB8/16 Info */
    if (PRTL_EVENT_TYPE_LTE_OTA_GPS_RESET == prtlEvent)
    {
#ifndef FEATURE_GNSS_LEAN_BUILD
      mgp_pgi_SendSibTimeRefreshMsg(e_as_id);
#endif /* ! FEATURE_GNSS_LEAN_BUILD */
    }
  }
  else if (PRTL_EVENT_TYPE_LTE_OTA_OTDOA_RESET == prtlEvent)
  {
    tm_core_ota_reset_notify_event(prtlEvent);
  }
}

/********************************************************************
* tm_core_handle_wifi_attachment_status_ind
*
* Function description:
* Handles Wifi attachment status indication info
*
* Parameters: 
*  p_wifi_attachment_status_ind_info - Pointer to Wifi attachment status inidcation info stuct
*
* Return:
* void
*  
*********************************************************************
*/
void tm_core_handle_wifi_attachment_status_ind(pdsm_wifi_attachment_status_ind_info *p_wifi_attachment_status_ind_info)
{
  gm_wifi_attachment_status_ind_s_type z_gm_wifi_attach_info;
  memset(&z_gm_wifi_attach_info, 0, sizeof(z_gm_wifi_attach_info));

  /* Convert attachment info into GM wifi attachment info*/

  memscpy(&z_gm_wifi_attach_info.z_attach_info,
          sizeof(z_gm_wifi_attach_info.z_attach_info),
          p_wifi_attachment_status_ind_info,
          sizeof(pdsm_wifi_attachment_status_ind_info));
  /* Indicate that the attachment info is from HLOS */
  z_gm_wifi_attach_info.e_src = GM_WIFI_ATTACH_INFO_SRC_HLOS;

  (void)gm_inject_wifi_attach_status_ind(&z_gm_wifi_attach_info);

#ifdef FEATURE_GNSS_FUSION_CSM
  (void)fusion_csm_InjectWifiAttachStatus(p_wifi_attachment_status_ind_info);
#endif /* FEATURE_GNSS_FUSION_CSM */

}

void tm_core_handle_wifi_enabled_status_ind(pdsm_wifi_enabled_status_ind_type e_wifi_enabled_status)
{
  gm_inject_wifi_enabled_status_ind(e_wifi_enabled_status);
#ifdef FEATURE_GNSS_FUSION_CSM
  fusion_csm_InjectWifiEnabledStatus(e_wifi_enabled_status);
#endif /* FEATURE_GNSS_FUSION_CSM */
}

/*===========================================================================
FUNCTION tm_core_get_end_supl_after_final_fix

DESCRIPTION
  This function checks SUPL can be terminated after final fix is sent
  
DEPENDENCIES

RETURN VALUE
  TRUE if NV is enabled, FALSE otherwise.

SIDE EFFECTS

===========================================================================*/
boolean tm_core_get_end_supl_after_final_fix_nv(void)
{
  return (tm_core_info.config_info.b_end_supl_after_final_fix);
}

/*===========================================================================
FUNCTION tm_core_handle_wifi_fix_response

DESCRIPTION
  This function handles Wifi fix Response

DEPENDENCIES

RETURN VALUE
  VOID

SIDE EFFECTS

===========================================================================*/
void tm_core_handle_wifi_fix_response(tm_wifi_fix_response_type *pz_wifi_fix_resp)
{
  if (NULL == pz_wifi_fix_resp)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null pointer in handling Wifi fix reponse", 0, 0, 0);
    return;
  }

  if (SM_REQ_TYPE_WIFI_POS_FOR_BLE == pz_wifi_fix_resp->e_wifi_pos_src)
  {
    sm_gm_pos_fix_info fix_info = { 0 };
    /*Send the Wifi fix reponse to Geofencing engine*/
    fix_info.y_pos_fix_ext_info.z_wifi_fix_ext_info.z_wifi_result = pz_wifi_fix_resp->z_wifi_result;
    fix_info.fix_src = GEOFENCE_POS_FIX_SRC_LPW;
    fix_info.pos_fix = pz_wifi_fix_resp->z_PosFixRpt;
    fix_info.fix_type = GEOFENCE_POS_FIX_TYPE_FINAL;

    (void)gm_position_update(&fix_info);
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unknown Source %d", pz_wifi_fix_resp->e_wifi_pos_src, 0, 0);
  }
}

/*===========================================================================
  tm_core_query_for_lppm_status

  Description:

     This function is used to query for Lppm status

 Parameters:
    LPPM Power Mode Config

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean  tm_core_query_for_lppm_status(tm_sess_power_mode_s_type z_PowerMode)
{
  lm_request_info_s_type lm_info_req_param = { 0 };

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Request to Query Lppm for M%d, TBM %u",
        z_PowerMode.e_PowerMode, z_PowerMode.q_TbmMsecs, 0);
  lm_info_req_param.e_req_type = LM_REQUEST_LPPM_STATUS;
  (void)tm_ConvertToLmLppmMode(&z_PowerMode, &lm_info_req_param.z_request.z_lppm_status_request);

  return lm_request(&lm_info_req_param);
}

/*===========================================================================
  tm_core_lppm_start

  Description:

     This function is put engine in LPPM mode.
     Should be called only when engine is not in LPPM mode.

 Parameters:
    LPPM Power Mode Config

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_lppm_start(tm_sess_power_mode_s_type z_PowerMode)
{
  lm_session_request_info_s_type lm_sess_req_param = { 0 };

  lm_sess_req_param.q_handle    = tm_core_info.session_info.active_sess_handle;
  lm_sess_req_param.e_req_type  = LM_SESSION_REQUEST_START;
  lm_sess_req_param.q_req_mask = (uint32)LM_SESSION_REQUEST_LPPM;

  /*LM Request structure has 2 places where LPPM Request can be populated*/
  /*One is within z_request.z_session.z_lppm_config*/
  /*Other is z_request.z_lppm_request*/
  /*Populate both with same info*/
  (void)tm_ConvertToLmLppmMode(&z_PowerMode, &lm_sess_req_param.z_request.z_session_request.z_lppm_config);
  lm_sess_req_param.z_request.z_lppm_request = lm_sess_req_param.z_request.z_session_request.z_lppm_config;

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPPM Start Request: M%d TBM %d",
        z_PowerMode.e_PowerMode, z_PowerMode.q_TbmMsecs);
  return lm_session_request(&lm_sess_req_param);
}

/*===========================================================================
  tm_core_lppm_stop

  Description:

     This function is put engine out of LPPM mode

 Parameters:
    void

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_lppm_stop(void)
{
  lm_session_request_info_s_type lm_sess_req_param = { 0 };

  lm_sess_req_param.q_handle    = tm_core_info.session_info.active_sess_handle;
  lm_sess_req_param.e_req_type  = LM_SESSION_REQUEST_STOP;
  lm_sess_req_param.q_req_mask = (uint32)LM_SESSION_REQUEST_LPPM;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LPPM Stop Request", 0, 0, 0);

  return lm_session_request(&lm_sess_req_param);
}

/*===========================================================================
  tm_core_modify_lppm

  Description:

     This function is used to modify LPPM (Power Mode and/or TBM)

 Parameters:
    LPPM Power Mode Config

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_modify_lppm(tm_sess_power_mode_s_type z_PowerMode)
{
  lm_session_request_info_s_type lm_sess_req_param = { 0 };

  lm_sess_req_param.q_handle    = tm_core_info.session_info.active_sess_handle;
  lm_sess_req_param.e_req_type  = LM_SESSION_REQUEST_CONTINUE;
  lm_sess_req_param.q_req_mask  = (uint32)LM_SESSION_REQUEST_LPPM;

  /*LM Request structure has 2 places where LPPM Request can be populated*/
  /*One is within z_request.z_session.z_lppm_config*/
  /*Other is z_request.z_lppm_request*/
  /*Populate both with same info*/
  if (FALSE == tm_ConvertToLmLppmMode(&z_PowerMode, &lm_sess_req_param.z_request.z_session_request.z_lppm_config))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_core_modify_lppm: Conversion from TM-LM failed");
    return FALSE;
  }

  lm_sess_req_param.z_request.z_lppm_request = lm_sess_req_param.z_request.z_session_request.z_lppm_config;
  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Modify LPPM: Mode %d TBM %u", z_PowerMode.e_PowerMode, z_PowerMode.q_TbmMsecs);
  return lm_session_request(&lm_sess_req_param);
}

/*===========================================================================
  tm_core_handle_start_cpi

  Description:

     This function handles start CPI request.
     The only called of this function shall be the Tech Selector.

 Parameters:
    v_isEmergency: Is Emergency CPI

  Return value: 
    boolean: Pass\Fail
 
=============================================================================*/
boolean tm_core_handle_start_cpi(boolean v_isEmergency)
{
  tm_pdapi_pd_event_s_type pd_event_info = { 0 };

  pd_event_info.e_pd_event = PDSM_PD_EVENT_WPS_NEEDED;
  pd_event_info.pd_event_data.pd_info_type.client_id = pdsm_ni_client_id_get();
  pd_event_info.pd_event_data.pd_info_type.pd_info.qwip_data.request_type = PDSM_PD_QWIP_START_PERIODIC_HI_FREQ_FIXES;
  pd_event_info.pd_event_data.pd_info_type.pd_info.qwip_data.tbf_ms = 1000;
  pd_event_info.pd_event_data.pd_info_type.pd_info.qwip_data.v_is_emergency_mode = v_isEmergency;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "STARTING CPI", 0, 0, 0);
  tm_pdapi_pd_event_callback(&pd_event_info, TM_PDAPI_SESS_TYPE_NONE);

  return TRUE;

}

/*===========================================================================
  tm_core_handle_stop_cpi

  Description:
  
     This function handles stop CPI request.
     The only called of this function shall be the Tech Selector.

 Parameters:
    void

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
boolean tm_core_handle_stop_cpi(boolean v_isEmergency)
{
  tm_pdapi_pd_event_s_type pd_event_info = { 0 };

  pd_event_info.e_pd_event = PDSM_PD_EVENT_WPS_NEEDED;
  pd_event_info.pd_event_data.pd_info_type.client_id = pdsm_ni_client_id_get();
  pd_event_info.pd_event_data.pd_info_type.pd_info.qwip_data.request_type = PDSM_PD_QWIP_STOP_PERIODIC_FIXES;
  pd_event_info.pd_event_data.pd_info_type.pd_info.qwip_data.tbf_ms = 0;
  pd_event_info.pd_event_data.pd_info_type.pd_info.qwip_data.v_is_emergency_mode = v_isEmergency;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "STOPPING CPI", 0, 0, 0);
  tm_pdapi_pd_event_callback(&pd_event_info, TM_PDAPI_SESS_TYPE_NONE);

  return TRUE;
}

#ifdef FEATURE_GNSS_GERA_ENABLE
#error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */

/*===========================================================================

FUNCTION tm_core_config_external_time_transfer

DESCRIPTION
  This function request GERA to enable/disable AP-MP time trasfer

RETURN VALUE
  Boolean: TRUE:  Request successfully sent to MGP
            FALSE: Request failed to be sent to MGP

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_config_external_time_transfer(uint8 *u_EnableFlag)
{
  boolean retVal = FALSE;

  if (NULL == u_EnableFlag)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer sent to tm_core_config_external_time_transfer");
    return FALSE;
  }

#ifdef FEATURE_GNSS_GERA_ENABLE
  #error code not present
#else
  return (mgp_ConfigExtTimeTransferMode(*u_EnableFlag));
#endif /* FEATURE_GNSS_GERA_ENABLE */
}

/*===========================================================================

FUNCTION tm_core_get_emergency_support_config

DESCRIPTION
  This function returns the configured values for CPI and Early exit enable/disable 

DEPENDENCIES

RETURN VALUE UINT32, where bit 0 indicates if CPI enabled and bit 1 indicates Early exit logic enabled.

SIDE EFFECTS

===========================================================================*/

uint32 tm_core_get_emergency_support_config(void)
{
  return (tm_core_info.config_info.gnss_sm_emergency_support_config);
}
/*===========================================================================

FUNCTION tm_core_get_umpts_up_event_cb_fp

DESCRIPTION
  This function returns UMTS UP event callback function pointer.

DEPENDENCIES

RETURN VALUE
  UMTS UP event callback function pointer.

SIDE EFFECTS

===========================================================================*/
prtl_event_cb_f_type* tm_core_get_umpts_up_event_cb_fp(void)
{
  return tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_UMTS_UP].event_cb_fp;
}

/*===========================================================================
  tm_core_report_sess_start_to_techsel

  Description:

     This function reports session start to TechSel

 Parameters:
    tm_sess_req_start_param_s_type* : Pointer to session start request param
    tm_sess_handle_type: Active Session Handle

  Return value: 
    boolean: Pass\Fail

=============================================================================*/
void tm_core_report_sess_start_to_techsel(const tm_sess_req_start_param_s_type *pz_sess_start_param,
                                          const tm_sess_handle_type q_sess_handle)
{
  techSel_PosSessInfo  z_sess_info = { 0 };
  tm_cm_phone_state_info_s_type phone_state_info;

  z_sess_info.q_tbf_msec = pz_sess_start_param->tbf_ms;
  z_sess_info.q_num_fixes = pz_sess_start_param->num_fixes;
  z_sess_info.e_mode = pz_sess_start_param->op_mode;
  z_sess_info.z_power_mode = pz_sess_start_param->lppm_mode;
  z_sess_info.v_is_mo = FALSE;
  z_sess_info.v_is_internal = FALSE;
  z_sess_info.v_is_emergency = FALSE;

  tm_cm_iface_get_phone_state(&phone_state_info);

  if (LM_MO_MT_MODE_MO  == tm_core_mo_mt_mode(q_sess_handle))
  {
    z_sess_info.v_is_mo = TRUE;
  }
  if (tm_core_info.internal_session)
  {
    z_sess_info.v_is_internal = TRUE;
  }
  if (phone_state_info.e911_state != TM_CM_E911_STATE_NONE)
  {
    z_sess_info.v_is_emergency = TRUE;
  }

  (void)techSel_ReportPosSessionStart(&z_sess_info);
}

/*===========================================================================
tm_core_report_fix_start_to_techsel

Description:

This function reports fix start to TechSel

Parameters:

Return value:

=============================================================================*/
void tm_core_report_fix_start_to_techsel(void)
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TechSel Notified of Fix Start");
  (void)techSel_ReportPosFixStart();
}

/*===========================================================================
tm_core_report_911_state_to_techsel

Description:

This function reports start of E911 to TechSel so that it can terminate LPPM

Parameters:

Return value: None

=============================================================================*/
void tm_core_report_911_state_to_techsel(void)
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TechSel Notified of E911 Start");
  (void)techSel_ReportE911Start();
}

/*===========================================================================

FUNCTION tm_core_get_lpp_cp_event_cb_fp
  
DESCRIPTION
  This function returns LPP CP event callback function pointer.

DEPENDENCIES

RETURN VALUE
  LPP CP event callback function pointer.

SIDE EFFECTS

===========================================================================*/
prtl_event_cb_f_type* tm_core_get_lpp_cp_event_cb_fp(void)
{
  return tm_core_info.prtl_func_cb_table[TM_CORE_PRTL_CB_ARR_INDEX_LPP_CP].event_cb_fp;
}




/*===========================================================================

FUNCTION tm_core_cache_leap_second_info

DESCRIPTION
  This function caches the Leap Second Info into TM, sent from ME

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_cache_leap_second_info(mgp_LeapSecondInfoStructType *pz_LeapSecInfo)
{

  if (NULL == pz_LeapSecInfo)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer sent to tm_core_cache_leap_second_info");
    return;
  }

  /* Cache leap second info from ME */
  tm_core_info.config_info.z_LeapSecondInfo = *pz_LeapSecInfo;

  MSG_5(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Leap Second Info Updated: Leap Sec: %u, Leap Sec Unc: %u, Leap Sec Src: %u, UTC valid: %u, Correction: %d",
        pz_LeapSecInfo->u_DeltaLeapSec, pz_LeapSecInfo->u_DeltaLeapSecUnc, pz_LeapSecInfo->u_DeltaLeapSecSrc, pz_LeapSecInfo->u_UtcValid, pz_LeapSecInfo->b_Correction);
}

/*===========================================================================
FUNCTION tm_core_lpp_earlytimer_enabled

DESCRIPTION
  Return if the LPP Rel12 2Timer feature is enabled. If enabled the Early
  Response timer in Qos is processed, else it is dropped. By default the
  feature is disabled.

PARAMETER
  tm_core_config_2timer_support_enum_type e_2timer_support : used to 
    distinguish which bit to read in the u_gnss_lpp_rel12_2timer_support field.

RETURN VALUE
  TRUE  :If LPP Rel12 2Timer feature is enabled.
  FALSE :If disabled

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

boolean tm_core_lpp_earlytimer_enabled(
   tm_core_config_2timer_support_enum_type e_2timer_support)
{
  boolean v_ret = FALSE;

  MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "NV ITEM for Rel12 2Timer configured as %d,%d",
        TM_CORE_CONFIG_DD(u_gnss_lpp_rel12_2timer_support), e_2timer_support);

  switch (e_2timer_support)
  {
    case TM_CORE_CONFIG_2TIMER_LPP_UP:
      v_ret = (0 != (TM_CORE_CONFIG_DD(u_gnss_lpp_rel12_2timer_support) &
                        TM_CORE_CONFIG_2TIMER_LPP_UP_BITMASK)) ? TRUE : FALSE;
      break;
    case TM_CORE_CONFIG_2TIMER_LPP_CP:
      v_ret = (0 != (TM_CORE_CONFIG_DD(u_gnss_lpp_rel12_2timer_support) &
                        TM_CORE_CONFIG_2TIMER_LPP_CP_BITMASK)) ? TRUE : FALSE;
      break;
  }
  return v_ret;
}

/*===========================================================================

FUNCTION tm_core_get_engine_monitor_config

DESCRIPTION
  This function sends the GET request for engine monitor config value to  LM

DEPENDENCIES

RETURN VALUE
 void

SIDE EFFECTS

===========================================================================*/
void tm_core_get_engine_monitor_config(void)
{
  lm_request_info_s_type  lm_info_req_param;
  memset(&lm_info_req_param, 0, sizeof(lm_info_req_param));
  lm_info_req_param.e_req_type = LM_REQUEST_GET_ENGINE_MONITOR_CONFIG;
  (void)lm_request(&lm_info_req_param);
}

/*===========================================================================

FUNCTION tm_core_set_engine_monitor_config

DESCRIPTION
  This function sends the SET request for engine monitor config value to  LM

DEPENDENCIES

RETURN VALUE
 void

SIDE EFFECTS

===========================================================================*/
void tm_core_set_engine_monitor_config(const boolean v_engine_monitor_config)
{
  lm_request_info_s_type  lm_info_req_param;
  memset(&lm_info_req_param, 0, sizeof(lm_info_req_param));
  lm_info_req_param.e_req_type = LM_REQUEST_SET_ENGINE_MONITOR_CONFIG;
  lm_info_req_param.z_request.v_engine_monitor_enabled = v_engine_monitor_config;
  (void)lm_request(&lm_info_req_param);
}

/*===========================================================================
FUNCTION tm_core_inform_prtl_nv_op

DESCRIPTION
  Update protocol about NV READ operation.

RETURN VALUE
  None
DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_inform_prtl_nv_op(prtl_ruim_nv_read_group *u_NvGroup)
{
  int prtlTableIndex = 0;
  prtl_ruim_nv_read_group u_informNvGrp;
  prtl_event_u_type       event_payload;

  if (NULL == u_NvGroup)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NV RD Complete Operation null ptr");
    return;
  }

  u_informNvGrp = *u_NvGroup;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Notify PRTL's NV OP complete group: %d", u_informNvGrp);

  switch (u_informNvGrp)
  {
    case PRTL_EVENT_RUIM_CURR_NAM_READ_OP:
      /*Send the PRTL_EVENT_TYPE_RUIM_NV_READ for 1X module*/
      event_payload.ruim_read_nv_payload.ruim_item_group = PRTL_EVENT_RUIM_CURR_NAM_READ_OP;
      prtlTableIndex = tm_core_get_prtl_idx_by_type(TM_PRTL_TYPE_1X_UP);
      if ((prtlTableIndex > -1) && (prtlTableIndex < TM_PRTL_NUM))
      {
        if (tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp != NULL)
        {
          (void)tm_core_info.prtl_func_cb_table[prtlTableIndex].event_cb_fp(
             PRTL_EVENT_TYPE_RUIM_NV_READ,
             &event_payload);
        }
      }
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "CURR NAM READ complete update to TM 1X UP");
      break;
    default:
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NV RD invalid NV group");
      break;
  }

  return;
}

/*===========================================================================
FUNCTION  tm_ack_inject_srn_scan_info_msg

DESCRIPTION
  Send an Acknowledgement for SRN scan report messages.

DEPENDENCIES

PARAMETERS
  e_ackStatus - Ack status.

RETURN VALUE
  none
SIDE EFFECTS
  None
===========================================================================*/
static void tm_ack_inject_srn_scan_info_msg(locEngStatusEnumT_v01 e_ackStatus)
{
  tm_pdapi_pd_event_s_type  *pz_event_info = NULL;

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "tm_ack_inject_srn_scan_info_msg called with status %d", e_ackStatus);

  /* Use dynamic allocation to save call stack memory */
  pz_event_info = (tm_pdapi_pd_event_s_type *)os_MemAlloc(sizeof(tm_pdapi_pd_event_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_event_info)
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Could not allocate memory", 0, 0, 0);
    return;
  }

  memset(pz_event_info, 0, sizeof(*pz_event_info));
  pz_event_info->e_pd_event = PDSM_PD_EVENT_ACK_SRN_INFO_INJECTION;
  pz_event_info->pd_event_data.pd_info_type.client_id = -1;

  pz_event_info->pd_event_data.pd_info_type.pd_info.srn_inj_ack.status = e_ackStatus;
  tm_pdapi_pd_event_callback(pz_event_info, TM_PDAPI_SESS_TYPE_NONE);

  /* free this event buffer */
  os_MemFree((void **)&pz_event_info);
}

/*===========================================================================
FUNCTION tm_core_handle_inject_srn_scan_msg

DESCRIPTION
  Handle Srn ap scan report. This function is called to report
  Srn scans (BT,BTLE,NFC..).
DEPENDENCIES

RETURN VALUE
  TRUE if Srn Scan message has been injected successfully

SIDE EFFECTS

===========================================================================*/
boolean tm_core_handle_inject_srn_scan_msg(pdsm_srn_scan_info_list_s_type *pz_srn_scan_msg)
{
  boolean bRetVal = FALSE;
  prtl_event_u_type  *pz_event_payload = NULL;
  boolean v_reportValid = FALSE;
  locEngStatusEnumT_v01 e_ackStatus = eLOC_ENG_GENERAL_FAILURE_V01;
  pdsm_client_id_type l_clientId = -1;

  if (pz_srn_scan_msg == NULL)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null pointer passed into tm_core_handle_inject_srn_scan_msg");
    return FALSE;
  }

  /* Check for validity of report */
  if ((0 == pz_srn_scan_msg->z_srn_ap_scan_info.q_num_srn_aps) ||
      (pz_srn_scan_msg->z_srn_ap_scan_info.q_num_srn_aps > LOC_ENG_SRN_MAX_REPORTED_APS_PER_MSG_V01) ||
      (NULL == pz_srn_scan_msg->z_srn_ap_scan_info.srn_ap_ptr))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Invalid Srn-scan-result-injection (numWifiAps=%u first_wifi_ap_ptr=%x)",
          pz_srn_scan_msg->z_srn_ap_scan_info.q_num_srn_aps,
          pz_srn_scan_msg->z_srn_ap_scan_info.srn_ap_ptr);
  }
  else
  {
    v_reportValid = TRUE;
    /* In case of valid wifi report data, send ack status as success */
    e_ackStatus = eLOC_ENG_SUCCESS_V01;
  }

  /* Send an ack for this srn ap scan report*/
  tm_ack_inject_srn_scan_info_msg(e_ackStatus);

  /* Exit the function in case of invalid report */
  if (FALSE == v_reportValid)
  {
    return FALSE;
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "TM got Srn-scan-result injection (numWifiAps=%u)",
        pz_srn_scan_msg->z_srn_ap_scan_info.q_num_srn_aps);



#ifdef FEATURE_GNSS_LPPE
  /* Cache the wifi scan results, choose the correct requestor ID.*/
  if ((TM_AUXTECH_REQ_ID_LPPE_UP == tm_SrnReqIdGet()) ||
      (TM_AUXTECH_REQ_ID_LPPE_CP == tm_SrnReqIdGet()))
  {
    /* Cache the wifi scan results*/
    tm_cacheSrnScanReports(*pz_srn_scan_msg, tm_SrnReqIdGet());
  }
#endif /*FEATURE_GNSS_LPPE*/

  return bRetVal;
}

/*===========================================================================

FUNCTION tm_core_handle_sensor_info

DESCRIPTION
  This function forwards the SLIM Sensor message to TM AUX Tech.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/

void tm_core_handle_sensor_info(void *p_SlimMsg)
{

  if (NULL == p_SlimMsg)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Null SlimMsg PTR");
    return;
  }

  techSel_ProcessSensorReport((techSel_SensorReportType *)p_SlimMsg);
}

/*===========================================================================

FUNCTION tm_core_nmea_debug_info

DESCRIPTION
  This function processes NMEA debug string in TM And forwards
  it to HLOS for debugging purpose. Should not be mixed with NMEA strings
  generated with Measurement reports.

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_nmea_debug_info(sm_DebugNMEA *pz_DebugNMEACustom)
{
  if (NULL == pz_DebugNMEACustom)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL Nmea string");
    return;
  }

  if (TM_CORE_CONFIG_DD(nmea_output_format) & PDAPI_NMEA_DEBUG)
  {
    tm_nmea_debug_report(pz_DebugNMEACustom);
  }
}

/*===========================================================================

FUNCTION tm_core_is_e911_concurrency_allowed

DESCRIPTION
  This function calls MGP API to decide if an E911 session can be started in 
  concurrent mode.

PARAMETERS
  e_incoming_client: New incoming client for which concurrency check is done to get into concurrent mode
  v_is_msa_session: TRUE if concurrency check is done on an ongoing MSA session. 
                              FALSE if ongoing is a non-MSA session

RETURN VALUE
  boolean

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_e911_concurrency_allowed(ldm_client_id_e_type e_incoming_client, boolean v_is_msa_session)
{
  boolean v_concurrency_allowed = FALSE;
  boolean v_cleanup_supl_mo = FALSE;
  tm_cm_phone_state_info_s_type phone_state_info;

  tm_cm_iface_get_phone_state(&phone_state_info);

  if ((LDM_CLIENT_ID_E911 == e_incoming_client) &&
      ((CGPS_SRV_SYS_CDMA == phone_state_info.srv_system) ||
       (CGPS_SRV_SYS_HDR  == phone_state_info.srv_system)))
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED,
          "LDM:Concurrency unsupported in srv_system (%d) conc state (%d)",
          phone_state_info.srv_system, phone_state_info.e911_state);
    return v_concurrency_allowed;
  }

  /* Check if NV is enabled for concurrency */
  if ((tm_core_get_concurrency_support_config() & TM_E911_CONCURRENCY_ENABLE) != 0)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:E911 Concurrency feature NV enabled");

    if (LDM_CLIENT_ID_E911 == e_incoming_client)
    {
      /* Check if apptracking client is enabled at LDM */
      if ((ldm_client_query_state() & LDM_CLIENT_APPTRK_MASK) != 0)
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrk in progress in LDM");

        v_cleanup_supl_mo = tm_allow_mo_supl_during_concurrency();

        /* Check if TBF == 1s. TBF > 1 is not supported for concurrency */
        if (TM_CORE_TBF_ONE_SECOND_MS == tm_core_info.session_info.pd_param.pd_option.fix_rate.time_between_fixes_ms)
        {
        /* Check with MGP for goodness condition for MSA sessions only */
        if (TRUE == v_is_msa_session)
        {
          if (0 != mgp_CheckNavContinueDuringE911())
          {
            v_concurrency_allowed = TRUE;
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:MGP goodness conditions met, OK to support concurrency");
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:MGP goodness conditions NOT met. Concurrency NOT allowed");
          }
        }
        else
        {
          v_concurrency_allowed = TRUE;
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:No goodness check done for non-MSA sess, OK to support concurrency");
        }
        if (TRUE == v_cleanup_supl_mo)
        {
          v_concurrency_allowed = FALSE;
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Ongoing app track SUPL, Not OK to support concurrency");
        }
      }
      else
      {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack TBF > 1s. Concurrency not allowed. TBF == %d",
                tm_core_info.session_info.pd_param.pd_option.fix_rate.time_between_fixes_ms);
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack not in progress. Concurrency NOT allowed");
      }
    }
    else if (LDM_CLIENT_ID_APPTRK == e_incoming_client)
    {
      if (TM_CM_E911_STATE_ORIG == phone_state_info.e911_state)
      {
        v_concurrency_allowed = TRUE;
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Incoming Apptrack session while in E911 state, OK to support concurrency");
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Not in E911 orig state. Concurrency NOT allowed");
      }
    }
  }

  return v_concurrency_allowed;
}

/*===========================================================================

FUNCTION tm_core_update_e911_concurrency_to_lm

DESCRIPTION
  This function updates LM on the current state of E911 concurrent session

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_update_e911_concurrency_to_lm(boolean v_e911_concurrency_ongoing)
{
  lm_info_s_type   lm_info_data;
  lm_info_data.e_info_type = LM_SESSION_INFO_CONCURRENCY;
  lm_info_data.z_info.v_concurrent_e911_ongoing = v_e911_concurrency_ongoing;
  (void)lm_info(&lm_info_data);
}

/*===========================================================================

FUNCTION tm_mapping_between_sess_start_sess_continue

DESCRIPTION
  This function maps from session start params to session continue params and vice -versa

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_mapping_between_sess_start_sess_continue(boolean v_start_to_continue, tm_sess_req_start_param_s_type *pz_sess_start_param,
                                                 tm_sess_req_continue_param_s_type *pz_sess_continue_req_param)
{
  if ((NULL == pz_sess_start_param) ||
      (NULL == pz_sess_continue_req_param))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL value sent to tm_mapping_between_sess_start_sess_continue");
    return;
  }

  if (TRUE == v_start_to_continue)
  {
    pz_sess_continue_req_param->op_mode = pz_sess_start_param->op_mode;
    pz_sess_continue_req_param->op_req = pz_sess_start_param->op_req;
    pz_sess_continue_req_param->num_fixes = pz_sess_start_param->num_fixes;
    pz_sess_continue_req_param->tbf_ms = pz_sess_start_param->tbf_ms;
    pz_sess_continue_req_param->accuracy_threshold = pz_sess_start_param->accuracy_threshold;
    pz_sess_continue_req_param->ppm_qos = pz_sess_start_param->ppm_qos;
    pz_sess_continue_req_param->prm_qos = pz_sess_start_param->prm_qos;
    pz_sess_continue_req_param->lr_qos = pz_sess_start_param->lr_qos;
    pz_sess_continue_req_param->prm_prq = pz_sess_start_param->prm_prq;
    pz_sess_continue_req_param->dynamic_qos_enabled = pz_sess_start_param->dynamic_qos_enabled;
    pz_sess_continue_req_param->multi_report_msa = pz_sess_start_param->multi_report_msa;
    pz_sess_continue_req_param->lppm_mode = pz_sess_start_param->lppm_mode;
    pz_sess_continue_req_param->specialReqType = pz_sess_start_param->specialReqType;
  }
  else
  {
    pz_sess_start_param->op_mode = pz_sess_continue_req_param->op_mode;
    pz_sess_start_param->op_req = pz_sess_continue_req_param->op_req;
    pz_sess_start_param->num_fixes = pz_sess_continue_req_param->num_fixes;
    pz_sess_start_param->tbf_ms = pz_sess_continue_req_param->tbf_ms;
    pz_sess_start_param->accuracy_threshold = pz_sess_continue_req_param->accuracy_threshold;
    pz_sess_start_param->ppm_qos = pz_sess_continue_req_param->ppm_qos;
    pz_sess_start_param->prm_qos = pz_sess_continue_req_param->prm_qos;
    pz_sess_start_param->lr_qos = pz_sess_continue_req_param->lr_qos;
    pz_sess_start_param->prm_prq = pz_sess_continue_req_param->prm_prq;
    pz_sess_start_param->dynamic_qos_enabled = pz_sess_continue_req_param->dynamic_qos_enabled;
    pz_sess_start_param->multi_report_msa = pz_sess_continue_req_param->multi_report_msa;
    pz_sess_start_param->lppm_mode = pz_sess_continue_req_param->lppm_mode;
    pz_sess_start_param->specialReqType = pz_sess_continue_req_param->specialReqType;
  }
}


/*===========================================================================

FUNCTION tm_core_map_lm_to_ldm_client

DESCRIPTION
  This function maps ldm_client id sent from LM to LDM client id used in TM

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
ldm_client_id_e_type tm_core_map_lm_to_ldm_client(lm_ldm_client_disable_type e_lm_ldm_client_id)
{
  switch (e_lm_ldm_client_id)
  {
    case LM_LDM_E911_CLIENT:
      return LDM_CLIENT_ID_E911;
    case LM_LDM_APPTRACK_CLIENT:
      return LDM_CLIENT_ID_APPTRK;
    case LM_LDM_SINGLE_SHOT_CLIENT:
      return LDM_CLIENT_ID_SINGLE_SHOT;
    default:
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Unknown LM LDM client id %u", e_lm_ldm_client_id);
      return LDM_CLIENT_ID_APPTRK;
  }
}

/*===========================================================================

FUNCTION tm_core_send_ldm_rcvr_change

DESCRIPTION
  This function notifies LM that it can turn OFF the RCVR after GERA has removed the client and session is cleaned up.

DEPENDENCIES

RETURN VALUE
 void

SIDE EFFECTS

===========================================================================*/
void tm_core_send_ldm_rcvr_change(lm_gera_rcvr_mode_change_reason_e_type e_rcvr_change_reason, boolean v_forceMgpOff)
{
  lm_request_info_s_type  lm_info_req_param;
  memset(&lm_info_req_param, 0, sizeof(lm_info_req_param));
  lm_info_req_param.e_req_type = LM_REQUEST_LDM_RECVR_CHANGE;
  lm_info_req_param.z_request.e_gera_rcvr_off.v_force_mgp_off = v_forceMgpOff;
  lm_info_req_param.z_request.e_gera_rcvr_off.e_rcvr_off_reason = e_rcvr_change_reason;
  (void)lm_request(&lm_info_req_param);
}

/*===========================================================================

FUNCTION tm_core_get_concurrency_support_config

DESCRIPTION
  This function returns the configured values for E911 concurrency 

DEPENDENCIES

RETURN VALUE UINT32, where bit 0 indicates if E911 concurrency with apptrack is enabled or not

SIDE EFFECTS

===========================================================================*/

uint32 tm_core_get_concurrency_support_config(void)
{
  return (tm_core_info.config_info.gnss_sm_concurrent_sessions_support_config);
}

/*===========================================================================

FUNCTION tm_core_mem_alloc_for_cache_aiding_data

DESCRIPTION
  This function either allocates memory for frees memory to cache aiding data during UMTS_UP_NI sessions 
  in concurrency mode. During concurrency, aiding data needs to be sent to UMTS_UP_NI sessions specially
  as its not the primary client running in LM.

PARAMS
  v_mem_alloc: TRUE means mem is allocated, FALSE means free the allocated mem
DEPENDENCIES

RETURN VALUE 
   None

SIDE EFFECTS

===========================================================================*/
void tm_core_mem_alloc_for_cache_aiding_data(boolean v_mem_alloc)
{
  if (TRUE ==  v_mem_alloc)
  {
    up_cache_sess_info_param  = (prtl_sess_info_param_u_type *)os_MemAlloc(sizeof(prtl_sess_info_param_u_type), OS_MEM_SCOPE_TASK);

    if (NULL == up_cache_sess_info_param)
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Cannot allocate memory for prtl_sess_info_param_u_type in tm_core_mem_alloc_for_cache_aiding_data");
      return;
    }

    memset(up_cache_sess_info_param, 0, sizeof(*up_cache_sess_info_param));
  }
  else
  {
    if (NULL != up_cache_sess_info_param)
    {
      (void)os_MemFree((void **)&up_cache_sess_info_param);
      up_cache_sess_info_param = NULL;
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM:Cache memory already freed");
    }
  }
}

/*===========================================================================

FUNCTION tm_core_mem_alloc_for_cache_aiding_data

DESCRIPTION
  This function is used to handle service interactions during E911 concurrent session with apptracking.  

PARAMS
  prtl_type: Protocol type of the new session
  e_srv_system: Current serving system
DEPENDENCIES

RETURN VALUE 
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_concurrent_mode_e911_service_interaction(tm_prtl_type prtl_type, cgps_SrvSystemType e_srv_system)
{
  boolean v_active_sess_is_ni = FALSE;
  boolean v_new_sess_prtl_ni  = FALSE;
  boolean v_prtl_abort_session = FALSE;
  int     prtlTableIndex       = -1;
  tm_prtl_type                    e_ldm_prtl_type = TM_PRTL_TYPE_NONE;
  tm_sess_handle_type             e_ldm_sess_handle = TM_SESS_HANDLE_NONE;
  tm_sess_req_start_param_s_type  z_ldm_sess_start_param;

  memset(&z_ldm_sess_start_param, 0, sizeof(tm_sess_req_start_param_s_type));
  if (FALSE == (tm_lpc_get_client_info(LDM_CLIENT_ID_E911,
                                       &e_ldm_prtl_type,
                                       &e_ldm_sess_handle,
                                       &z_ldm_sess_start_param)))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: No service interaction in concurrent mode");
    return;
  }

  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: Service interaction in concurrent mode");

  v_active_sess_is_ni =
     ((e_ldm_sess_handle == TM_SESS_HANDLE_LPP_CP_NI) ||
      (e_ldm_sess_handle == TM_SESS_HANDLE_1X_CP_MT) ||
      (e_ldm_sess_handle == TM_SESS_HANDLE_UMTS_UP_NI) ||
      (e_ldm_sess_handle == TM_SESS_HANDLE_UMTS_CP_NI));

  v_new_sess_prtl_ni  =
     ((prtl_type == TM_PRTL_TYPE_UMTS_CP) ||
      (prtl_type == TM_PRTL_TYPE_1X_CP) ||
      (prtl_type == TM_PRTL_TYPE_UMTS_UP) ||
      (prtl_type == TM_PRTL_TYPE_LPP_CP));

  prtlTableIndex = tm_core_get_prtl_idx_by_type(e_ldm_prtl_type);

  if ((prtlTableIndex > -1) &&
      (prtlTableIndex < TM_PRTL_NUM) &&
      (v_active_sess_is_ni) &&
      (v_new_sess_prtl_ni))
  {
    /* emergency session was ongoing on LPP_CP or SUPL, now a new GPS session was started on a different RAT
       started by the network, continue the session with the new session handle */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Emergency session on going on %d, new protocol %d. Abort ongoing ",
          e_ldm_sess_handle, prtl_type, 0);

    if (TRUE == (tm_core_abort_e911_in_concurrency(prtlTableIndex, e_ldm_sess_handle)))
    {
      return;
    }
  }

#ifdef FEATURE_MO_GPS_FOR_ECALL
  if (!v_active_sess_is_ni && v_new_sess_prtl_ni &&
      (e_ldm_sess_handle != TM_SESS_HANDLE_E911) &&
      (e_ldm_sess_handle != TM_SESS_HANDLE_NONE))
  {
    /* MO session was ongoing in E911, however network session started on
       * W/1X/LPP. Abort the ongoing session and accept the new session */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Abort ongoing session. Handle(%d), New protocol (%d)",
          e_ldm_sess_handle, prtl_type, 0);

    if (TRUE == (tm_core_abort_e911_in_concurrency(prtlTableIndex, e_ldm_sess_handle)))
    {
      return;
    }
  }
#endif

  if ((e_ldm_sess_handle != TM_SESS_HANDLE_NONE) &&
      ((e_srv_system == CGPS_SRV_SYS_LTE) &&
       ((prtl_type == TM_PRTL_TYPE_UMTS_CP) ||
        (prtl_type == TM_PRTL_TYPE_1X_CP))))
  {
    /* emergency session was started on SVLTE when camped on LTE, however network session started on W/1X
       abort the ongoing session and accept the new session */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Emergency session on going on %d, new protocol %d. Abort ongoing ",
          tm_core_info.session_info.active_sess_handle, prtl_type, 0);
    if (TRUE == (tm_core_abort_e911_in_concurrency(prtlTableIndex, e_ldm_sess_handle)))
    {
      return;
    }
  }

  /* if MO is ongoing and a new NI session is started */
  if (((e_ldm_sess_handle < TM_SESS_HANDLE_1X_CP_MT) &&
       (e_ldm_sess_handle != TM_SESS_HANDLE_NONE)) &&
      ((prtl_type == TM_PRTL_TYPE_UMTS_CP) ||
       (prtl_type == TM_PRTL_TYPE_1X_CP) ||
       (prtl_type == TM_PRTL_TYPE_LPP_CP) ||
       (prtl_type == TM_PRTL_TYPE_UMTS_UP)))
  {
    /* MO was ongoing, however network session started on W/G/1X/LTE E911 session
       abort the ongoing session and accept the new session. */
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "MO session on going on %d, new E911 session started by protocol %d. Abort ongoing ",
          tm_core_info.session_info.active_sess_handle, prtl_type, 0);

    if (TRUE == (tm_core_abort_e911_in_concurrency(prtlTableIndex, e_ldm_sess_handle)))
    {
      return;
    }
  }
}

/*===========================================================================

FUNCTION tm_core_abort_e911_in_concurrency

DESCRIPTION
  This function is used to end an ongoing E911 session in concurrency mode 

PARAMS
  prtlTableIndex: Protocol table index
  e_ldm_sess_handle: Session handle of the E911 session in LDM
DEPENDENCIES

RETURN VALUE 
   Boolean: TRUE: Successful, FALSE: Failure

SIDE EFFECTS

===========================================================================*/
boolean tm_core_abort_e911_in_concurrency(const int prtlTableIndex, const tm_sess_handle_type e_ldm_sess_handle)
{
  boolean v_prtl_abort_session = FALSE;
  boolean v_ret_val            = FALSE;

  if (tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp)
  {
    v_prtl_abort_session =
       tm_core_info.prtl_func_cb_table[prtlTableIndex].stop_sess_req_fp(
       e_ldm_sess_handle,
       PRTL_SESS_STOP_SINGLE_FIX,
       TM_STOP_REASON_E911);

    if (v_prtl_abort_session == TRUE)
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Protocol aborted the session", 0, 0, 0);
      v_ret_val = TRUE;
    }
    else
    {
      MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Protocol continue with the session", 0, 0, 0);
    }
  }
  else
  {
    MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "No protocol callback installed 2", 0, 0, 0);
  }

  return v_ret_val;
}

/*===========================================================================

FUNCTION tm_core_switch_secondary_to_main_client

DESCRIPTION
   This function starts LM-TM session with E911 parameters when apptrack ends 
   first in concurrent mode.

PARAMS

DEPENDENCIES

RETURN VALUE 
   Boolean: TRUE: Successful, FALSE: Failure

SIDE EFFECTS

===========================================================================*/
void tm_core_switch_secondary_to_main_client(lm_gera_rcvr_mode_change_reason_e_type e_rcvr_change_reason,
                                             boolean                        u_forceMgpOff)
{
  tm_prtl_type                    e_ldm_prtl_type = TM_PRTL_TYPE_NONE;
  tm_sess_handle_type             e_ldm_sess_handle = TM_SESS_HANDLE_NONE;
  tm_sess_req_start_param_s_type  z_ldm_sess_start_param;
  tm_cm_phone_state_info_s_type   z_phone_state_info;
  ldm_client_id_e_type            e_ldm_client_id = LDM_CLIENT_ID_MAX;
  boolean                         v_ldm_leave_rcvr_on = FALSE;
  ldm_client_id_e_type            e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type            e_secondary_client_id = LDM_CLIENT_ID_MAX;
  uint32                          q_ldm_cli_state_mask = 0;
  uint32                          q_residual_time = 0;
  boolean                         v_is_switch_valid = FALSE;
  lpc_op_status_e_type            e_operation_status = LPC_OP_STATUS_ERROR_UNKNOWN;

  memset(&z_phone_state_info, 0, sizeof(tm_cm_phone_state_info_s_type));
  tm_cm_iface_get_phone_state(&z_phone_state_info);

  /* Get main and secondary client  from LDM */
  tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

  if ((LM_MGP_RCVR_IDLE_REASON_SESS_END == e_rcvr_change_reason) ||
      (LM_MGP_RCVR_OFF_REASON_SESS_END == e_rcvr_change_reason) ||
      (LM_MGP_GERA_RCVR_OFF_REASON_SHUTOFF_TIMER == e_rcvr_change_reason))
  {
    q_ldm_cli_state_mask = lpc_map_client_id_to_status_mask(e_main_client_id);
    if ((LDM_CLIENT_ID_MAX != e_main_client_id) &&
        ((ldm_client_query_state() & q_ldm_cli_state_mask) != 0))
    {
      if (TRUE == (tm_lpc_get_client_info(e_main_client_id,
                                          &e_ldm_prtl_type,
                                          &e_ldm_sess_handle,
                                          &z_ldm_sess_start_param)))
      {
        if ((TM_SESS_HANDLE_E911 == e_ldm_sess_handle) ||
            (TM_SESS_HANDLE_NONE == e_ldm_sess_handle))
        {
          MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: Switching from main client %d to secondary client %d for early RCVR E911 client.",
                e_main_client_id, e_secondary_client_id);
        }
        else
        {
          MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: Not switching from main client %d to secondary client %d yet. Expecting TM stop sess to initiate the switch",
                e_main_client_id, e_secondary_client_id);
          return;
        }
      }
      else
      {
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: tm_lpc_get_client_info failed");
      }
    }
    else
    {
      MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: Main client not present. Go ahead and switch to secondary client anyway");
    }
  }
  else if ((TM_CM_E911_STATE_CONCURRENT != z_phone_state_info.e911_state) &&
           (LDM_CLIENT_ID_MAX == e_main_client_id) && (LDM_CLIENT_ID_MAX == e_secondary_client_id))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Not in concurrent mode and no active clients. Switching not needed. Notify LM to change to RCVR state to %d",
          e_rcvr_change_reason);
    /* Send MGP RCVR change request to LM */
    tm_core_send_ldm_rcvr_change(e_rcvr_change_reason, u_forceMgpOff);
    return;
  }
  else if ((LM_MGP_GERA_RCVR_OFF_REASON_FIX_END == e_rcvr_change_reason) && (LDM_CLIENT_ID_MAX != e_main_client_id) && (LDM_CLIENT_ID_MAX == e_secondary_client_id))
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LDM: Switching not needed when the request comes from LM but main is active and secondary is inactive.");
    return;
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Handle secondary to main client switch. Change reason %d, Force option %d", e_rcvr_change_reason, u_forceMgpOff);
  }

  memset(&z_ldm_sess_start_param, 0, sizeof(tm_sess_req_start_param_s_type));

  v_no_force_idle_for_secondary_client = TRUE;

  q_ldm_cli_state_mask = lpc_map_client_id_to_status_mask(e_main_client_id);

  /* Disable main client from LDM if present */
  if ((LDM_CLIENT_ID_MAX != e_main_client_id) &&
      ((ldm_client_query_state() & q_ldm_cli_state_mask) != 0))
  {
    if (LPC_OP_STATUS_REQUEST_SUCCESS == tm_lpc_disable_client(e_main_client_id))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Main client %u successfully disabled at LDM", e_main_client_id);
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Main client %u failed to disable at LDM", e_main_client_id);
      v_no_force_idle_for_secondary_client = FALSE;
      return;
    }
  }
  else
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Main client %u already disabled at LDM. Go ahead and switch the secondary client %d",
          e_main_client_id, e_secondary_client_id);
  }

  q_ldm_cli_state_mask = lpc_map_client_id_to_status_mask(e_secondary_client_id);

  /* Set TM to previous E911 state since concurrency ended. This works only when switching E911 from secondary to main.
        When switching Apptrack from secondary to main, we should remain in concurrent mode. */
  if ((LDM_CLIENT_ID_E911 == e_secondary_client_id) ||
      (LDM_CLIENT_ID_MAX == e_secondary_client_id)) /* When there is no longer E911 client, get out of concurrent mode (since no apptrack/E911 in progress) */
  {
    tm_cm_iface_set_E911_state(z_phone_state_info.prev_e911_state);
  }

  if ((LDM_CLIENT_ID_MAX != e_secondary_client_id) &&
      ((ldm_client_query_state() & q_ldm_cli_state_mask) != 0))
  {
    if (TRUE == (tm_lpc_get_client_info(e_secondary_client_id,
                                        &e_ldm_prtl_type,
                                        &e_ldm_sess_handle,
                                        &z_ldm_sess_start_param)))
    {
#ifndef FEATURE_GNSS_LEAN_BUILD
      if ((LDM_CLIENT_ID_E911 == e_secondary_client_id) &&
          (TM_SESS_OPERATION_MODE_MSB == z_ldm_sess_start_param.op_mode))
      {
        /* During transition from secondary to main, save the remaining time [fix timeout] from the LocTimer, if the LocTimer is running.
           PRM timer neednt be reloaded with the residual time since sending cached meas shall be taken care of in the call flow timer expiry.
           This check for NULL control data was added to resolve a crash during monkey test, In this case LPC control data was NULL
           Which was causing the crash, In case if the data is NULL client switch from secondary to primary is aborted*/

        if (NULL != z_lpc_control_data)
        {
          q_residual_time = os_TimerCheck(z_lpc_control_data->p_location_Timer);

          if (q_residual_time > TM_CORE_RESIDUAL_TIMER_THRESHOLD_MS)
          {
            if (z_ldm_sess_start_param.op_req & TM_OP_REQ_LR)
            {
              /*Filling the value of lr_qos in seconds*/
              z_ldm_sess_start_param.lr_qos = q_residual_time / 1000;
              MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Residual time left is %d ms", q_residual_time);
            }
            v_is_switch_valid = TRUE;
          }
          MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:q_residual_time : %d v_is_switch_valid : %d ", q_residual_time, v_is_switch_valid);
        }
        
        if (!v_is_switch_valid)
        {
          /* Either we have empty control data or we have less than 1s left, Do not switch the secondary client to main. 
             If controll data is NULL there is no point switching secondary client to primary.
             If less than 1 sec left LM will not send out a fix if timeout is 0s.Let the LPC timer timeout and send out the final fix */
          
          v_no_force_idle_for_secondary_client = FALSE;

          /* When residual timer is 0, then disable secondary client */
          if (0 == q_residual_time)
          {
            e_operation_status = tm_lpc_disable_client(e_secondary_client_id);            
          }

          MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Abort switching secondary client %d to main. Rcvr change reason %d. Disable client status %d",
                e_secondary_client_id, e_rcvr_change_reason, e_operation_status);
          
          /* Send MGP RCVR change request to LM */
          tm_core_send_ldm_rcvr_change(e_rcvr_change_reason, u_forceMgpOff);
          return;
        }
      }
#endif /* ! FEATURE_GNSS_LEAN_BUILD */

      tm_lpc_stop_timer_request();

      /* Switch the secondary client to main client */
      if (tm_lpc_client_update_client_type(e_secondary_client_id, LDM_CLIENT_TYPE_MAIN))
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Secondary client %d switched as main client!", e_secondary_client_id);

        if (LDM_CLIENT_ID_E911 == e_secondary_client_id)
        {
          /* Trigger a new TM/LM session once main client is ended */
          v_enable_secondary_client_on_ldm = FALSE;

          if (TM_SESS_OPERATION_MODE_MSA == z_ldm_sess_start_param.op_mode)
          {
            (void)tm_lpc_client_update_aa_filter_pref(e_secondary_client_id, TRUE);
          }

          if (FALSE == tm_core_sess_req_start(e_ldm_prtl_type, e_ldm_sess_handle, z_ldm_sess_start_param))
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Secondary client %d FAILED to START as main client! Num_fixes = %d, TBF_ms = %d",
                  e_secondary_client_id, z_ldm_sess_start_param.num_fixes, z_ldm_sess_start_param.tbf_ms);
          }
          else
          {
            MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Secondary client %d session started as main! Num_fixes = %d, TBF_ms = %d",
                  e_secondary_client_id, z_ldm_sess_start_param.num_fixes, z_ldm_sess_start_param.tbf_ms);
            
            /* Update LM with E911 fix qualification parameters after the switch */
            tm_lpc_update_lm_with_E911_fix_qualification(e_secondary_client_id); 
            
            v_ldm_leave_rcvr_on = TRUE;
          }
          v_enable_secondary_client_on_ldm = TRUE;
        }
        else if (LDM_CLIENT_ID_APPTRK == e_secondary_client_id)
        {
          /* Send PDAPI EVENTS before aborting the Apptrack session */
          (void)tm_lpc_generate_pd_event(LDM_CLIENT_ID_APPTRK, TM_STOP_REASON_COMPLETED);
          /* Reset TM core and wait for LOC to send the next apptrack request to continue */
          tm_core_abort_recover(TRUE, TM_STOP_REASON_COMPLETED);
          v_ldm_leave_rcvr_on = TRUE;
        }
        else
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Unsupported client id %d. FAILED to switch as main client!", e_secondary_client_id);
        }
        
        /* Clear LPC control data for secondary client */
        tm_lpc_clear_control_data();
      }
      else
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Secondary client %d FAILED to switch as main client!", e_secondary_client_id);
      }
    }
    else
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "LDM:Secondary client %d session will not be STARTED! Client not enabled", e_secondary_client_id);
    }
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Secondary client %d not enabled in LDM", e_secondary_client_id);
  }

  if (FALSE == v_ldm_leave_rcvr_on)
  {
    MSG_2(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Secondary client %d need not be started. Send IPC to LM to change RCVR state to %d.", e_secondary_client_id, e_rcvr_change_reason);
    /* Send MGP RCVR change request to LM */
    tm_core_send_ldm_rcvr_change(e_rcvr_change_reason, u_forceMgpOff);
  }
  else
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Leave RCVR in ON state. Secondary client %d session has started at TM/LM", e_secondary_client_id);
  }
  v_no_force_idle_for_secondary_client = FALSE;
}

/*===========================================================================

FUNCTION tm_core_get_fusion_csm_engine_config

DESCRIPTION
  This function returns the Fusion CSM configuration

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_get_fusion_csm_config(fusion_csm_config_s_type *pz_fusionCsmConfig)
{
  if (pz_fusionCsmConfig != NULL)
  {
    memscpy(pz_fusionCsmConfig, sizeof(*pz_fusionCsmConfig),
            &tm_core_info.config_info.fusion_csm_config,
            sizeof(tm_core_info.config_info.fusion_csm_config));
  }
}

/*===========================================================================

FUNCTION tm_core_update_fusion_cs_mode

DESCRIPTION
  This function updates the fusion cs mode

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_update_fusion_cs_mode(boolean b_OnDemandActive)
{
  if (tm_core_info.cs_on_demand_active == b_OnDemandActive)
  {
    return;
  }

  tm_core_info.cs_on_demand_active = b_OnDemandActive;

  MSG_3(MSG_SSID_GPSSM, MSG_LEGACY_MED, "b_OnDemandActive=%d", b_OnDemandActive, 0, 0);

  wle_update_wifi_pos_inj_allowed(!b_OnDemandActive);
}

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/*===========================================================================

FUNCTION tm_core_is_active_sess_mo

DESCRIPTION
  Checks if current on-going sess is MO AppTrack/Standalone

RETURN VALUE
  TRUE: If the on-going session is MO AppTrack/Standalone
  FALSE: If the on-going session is not MO AppTrack/Standalone

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
boolean tm_core_is_active_sess_mo()
{
  boolean retVal = FALSE;
  tm_sess_handle_type sess_handle = TM_SESS_HANDLE_NONE;

  sess_handle = tm_core_info.session_info.active_sess_handle;

  if ((sess_handle > TM_SESS_HANDLE_NONE) &&
      (sess_handle < TM_SESS_HANDLE_1X_CP_MT))
  {
    retVal = TRUE;
  }
  return retVal;
}
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

#ifdef FEATURE_GNSS_NON_STANDARD_SUPPORT
/*===========================================================================

FUNCTION tm_core_disable_APMP

DESCRIPTION
  This function disables APMP in GERA as notified by MC

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_disable_APMP(void)
{
#ifdef FEATURE_GNSS_GERA_ENABLE
  #error code not present
#endif /* FEATURE_GNSS_GERA_ENABLE */
}
#endif /* FEATURE_GNSS_NON_STANDARD_SUPPORT */

/*===========================================================================

FUNCTION tm_core_active_session_mode

DESCRIPTION
  Function to get the Session mode for the session in TM Core.

RETURN VALUE
  Return : TM OP_Mode if a session is active.

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
tm_sess_operation_mode_e_type tm_core_active_session_mode()
{
  return (tm_core_info.session_info.op_mode);
}



/*===========================================================================

FUNCTION tm_core_get_sarf_status

DESCRIPTION
  This function returns the current SARF status from TM config 

DEPENDENCIES

RETURN VALUE boolean: TRUE: RF test in progress; FALSE: No RF test in progress

SIDE EFFECTS

===========================================================================*/

boolean tm_core_get_sarf_status(void)
{
  return (tm_core_info.config_info.v_sarf_status);
}

/*===========================================================================
FUNCTION tm_core_handle_engine_monitor_status_report

DESCRIPTION
This function handles engine monitor status report from MGP

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void tm_core_handle_engine_monitor_status_report(uint64 const *const cpt_EngMonitorStatusRpt)
{
  pdsm_ext_status_info_s_type ext_status_info_type = { 0 };

  if (NULL == cpt_EngMonitorStatusRpt)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_Core: Null cpt_EngMonitorStatusRpt");
    return;
  }

  memset((void *)&ext_status_info_type, 0, sizeof(pdsm_ext_status_info_s_type));

  ext_status_info_type.client_id = tm_core_info.session_info.pd_param.client_id;
  ext_status_info_type.ext_status_type = PDSM_EXT_STATUS_ENGINE_MONITOR_REPORT;
  ext_status_info_type.ext_status_info.ext_engine_monitor_report.t_engine_monitor_status = *cpt_EngMonitorStatusRpt;

  /*Send indication to PDAPI*/
  tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_ENGINE_MONITOR_REPORT, &ext_status_info_type);
}

/*===========================================================================

FUNCTION tm_core_get_ni_sess_handle

DESCRIPTION
  This function returns the NI session handle during concurrency 

DEPENDENCIES

RETURN VALUE 
sess_handle 

SIDE EFFECTS

===========================================================================*/
tm_sess_handle_type tm_core_get_ni_sess_handle(void)
{
  ldm_client_id_e_type           e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type           e_secondary_client_id = LDM_CLIENT_ID_MAX;
  tm_sess_handle_type sess_handle = tm_core_info.session_info.active_sess_handle;
  tm_cm_phone_state_info_s_type phone_state_info;

  tm_cm_iface_get_phone_state(&phone_state_info);

  if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
  {
    tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

    if (LDM_CLIENT_ID_E911 == e_secondary_client_id)
    {
      sess_handle = tm_lpc_get_sess_handle_for_ldm_client(e_secondary_client_id);
    }
  }
  return sess_handle;
}

/*===========================================================================

FUNCTION tm_core_lfm_config_update

DESCRIPTION
This function handles Feature status update receoved from Location Feature 
Manager 

RETURN VALUE
  None

DEPENDENCIES
  None

SIDE EFFECTS

===========================================================================*/
void tm_core_lfm_config_update(sm_LFMConfigUpdateStructType *pz_LFMfeatureStatus)
{
  if (NULL == pz_LFMfeatureStatus)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "NULL pointer LFM Feature update");
    return;
  }

  if (SM_LFM_FEATURE_SV_POLY ==  pz_LFMfeatureStatus->e_featureType)
  {
    /* If feature is Disabled uncheck SV POLY reporting
       If feature enabled set SV POLY reporting */
    if (FALSE == pz_LFMfeatureStatus->v_status)
    {
      tm_core_info.config_info.gnss_meas_rep_qmi_out_control &= ~SM_GNSS_BMAP_QMI_SV_POLY_REPORTING;
    }
    else
    {
      tm_core_info.config_info.gnss_meas_rep_qmi_out_control |= SM_GNSS_BMAP_QMI_SV_POLY_REPORTING;
    }
  }

  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "gnss_meas_rep_qmi_out_control %d", tm_core_info.config_info.gnss_meas_rep_qmi_out_control);
  return;
}

/*
******************************************************************************
* tm_construct_location_notification_ind
*
* Function description:
*  Constructs location notification message that indicate the
*  protocol type, location info that was shiped outside modem, location response
*  that indicate if the session was rejected, accepted location shiped and accepted
*  with no location shiped, etc.
*
* PARAMS:
*  tm_location_notification_s_type tm_location_notification
*
* Return value: None
******************************************************************************
*/
void tm_construct_location_notification_ind(tm_location_notification_s_type const tm_location_notification)
{
  tm_cm_phone_state_info_s_type             phone_state_info = { 0 };
  pdsm_ext_status_info_s_type               *pz_ext_status_info_type = NULL;
  pdsm_pd_location_notification_info_s_type z_location_notification_info = { 0 };
  char                                      *p_str;

  if ((tm_location_notification.e_sub_id < SM_CONFIG_MIN_SUB_ID) || (tm_location_notification.e_sub_id > SM_CONFIG_MAX_SUB_ID))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Privacy: Invalid sub_id %d", tm_location_notification.e_sub_id);
    return;
  }

  if (tm_location_notification.e_pdapi_client_type != PDSM_CLIENT_TYPE_NI)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Privacy: Not sending location notification for Non-NI sessions. Prtl_type = %d", tm_location_notification.q_prtl_type);
    return;
  }

  tm_cm_iface_get_phone_state(&phone_state_info);

  switch (tm_location_notification.q_prtl_type)
  {
    case TM_PRTL_TYPE_UMTS_UP:
      {
        if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)  /* E911 mode */
        {
          if ((TRUE == tm_location_notification.v_location_privacy_override_set) && /* PO SET */
              (FALSE == tm_location_notification.v_emergency_bit_set)) /* SUPL INIT has emergency BIT SET to FALSE */
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Privacy: Not sending location notification for %d during SUPL PO and ES bit set to FALSE", tm_location_notification.q_prtl_type);
            return;
          }
        }
        else
        {
          if (TRUE == tm_location_notification.v_location_privacy_override_set)
          {
            MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Privacy: Not sending location notification for %d since PO set to TRUE", tm_location_notification.q_prtl_type);
            return;
          }
        }

        z_location_notification_info.e_protocol_stack = PDSM_PD_LOC_PRTL_SUPL;

        p_str = "NI_SUPL";
        (void)GNSS_STRLCPY((char *)(z_location_notification_info.loc_requestor_id),
                           p_str, PDSM_PD_MAX_REQUESTOR_ID_LEN);
        p_str = "NI";
        (void)GNSS_STRLCPY((char *)(z_location_notification_info.client_id),
                           p_str, PDSM_PD_MAX_CLIENT_ID_LEN);
        break;
      }

    case TM_PRTL_TYPE_UMTS_CP:
    case TM_PRTL_TYPE_LPP_CP:
    case TM_PRTL_TYPE_1X_CP:
    case TM_PRTL_TYPE_1X_UP:
      {
        if ((tm_core_info.config_info.subs_cfgs[tm_location_notification.e_sub_id].q_gnss_lock_ctrl_for_oem & PDSM_PA_CP_NILR_NOTIFY_OEM_CTRL) == 0)
        {
          MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Privacy: Not sending location notification for %d. OEM NV CTRL set to Dont Allow", tm_location_notification.q_prtl_type);
          return;
        }

        z_location_notification_info.e_protocol_stack = PDSM_PD_LOC_PRTL_CTRL_PLANE;

        p_str = "NI_CP";
        (void)GNSS_STRLCPY((char *)(z_location_notification_info.loc_requestor_id),
                           p_str, PDSM_PD_MAX_REQUESTOR_ID_LEN);
        p_str = "NI";
        (void)GNSS_STRLCPY((char *)(z_location_notification_info.client_id),
                           p_str, PDSM_PD_MAX_CLIENT_ID_LEN);
        break;
      }

    default:
      {
        MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Privacy: Unsupported Protocol %d, sending notification with PDSM_PD_LOC_PRTL_OTHER option", tm_location_notification.q_prtl_type);
        z_location_notification_info.e_protocol_stack = PDSM_PD_LOC_PRTL_OTHER;
      }
  }

  z_location_notification_info.e_location_requestor = PDSM_PD_LOC_REQUESTOR_CARRIER;

  z_location_notification_info.e_location_info = tm_location_notification.e_location_info_type;

  z_location_notification_info.e_location_response = tm_location_notification.e_location_response;

  if (TM_CM_E911_STATE_NONE != phone_state_info.e911_state)
  {
    z_location_notification_info.v_e911_state = TRUE;
  }
  else
  {
    z_location_notification_info.v_e911_state = FALSE;
  }

  z_location_notification_info.v_cached_location = FALSE;
  z_location_notification_info.v_client_id_valid = TRUE;

  p_str = "NI";
  (void)GNSS_STRLCPY((char *)(z_location_notification_info.client_id),
                     p_str, PDSM_PD_MAX_CLIENT_ID_LEN);

  MSG_7(MSG_SSID_GPSSM, MSG_LEGACY_MED, "Privacy: Notification: protocol = %d, requestor = %d, response = %d, location_info = %d, e911 = %d, is_cached = %d, is_client_id_valid = %d",
        z_location_notification_info.e_protocol_stack,
        z_location_notification_info.e_location_requestor,
        z_location_notification_info.e_location_response,
        z_location_notification_info.e_location_info,
        z_location_notification_info.v_e911_state,
        z_location_notification_info.v_cached_location,
        z_location_notification_info.v_client_id_valid);

  /* Send location privacy notification */
  pz_ext_status_info_type = (pdsm_ext_status_info_s_type *)os_MemAlloc(sizeof(pdsm_ext_status_info_s_type), OS_MEM_SCOPE_TASK);
  if (NULL == pz_ext_status_info_type)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "Privacy: Cannot allocate memory for pdsm_ext_status_info_s_type in tm_construct_location_notification_ind");
    return;
  }
  else
  {
    memset(pz_ext_status_info_type, 0, sizeof(*pz_ext_status_info_type));

    pz_ext_status_info_type->ext_status_type = PDSM_EXT_STATUS_LOC_NOTIFICATION_REPORT;

    if (tm_location_notification.e_pdapi_client_id != 0)
    {
      pz_ext_status_info_type->client_id = tm_location_notification.e_pdapi_client_id;
    }
    else
    {
      pz_ext_status_info_type->client_id = pdsm_ni_client_id_get();
    }

    pz_ext_status_info_type->ext_status_info.z_ext_location_notification_info = z_location_notification_info;
    tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_LOC_NOTIFICATION_REPORT, pz_ext_status_info_type);
    (void)os_MemFree((void **)&pz_ext_status_info_type);
  }
}

/*===========================================================================
FUNCTION  tm_allow_mo_supl_during_concurrency

DESCRIPTION
  Decides if MO-SUPL and E911 early RCVR session can run concurrently or not

DEPENDENCIES

PARAMETERS
  None.

RETURN VALUE
  Boolean
  
SIDE EFFECTS
  None
===========================================================================*/
static boolean tm_allow_mo_supl_during_concurrency(void)
{
  tm_loc_processing_client_info *pz_loc_client_info;
  ldm_client_id_e_type           e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type           e_secondary_client_id = LDM_CLIENT_ID_MAX;
  boolean                        v_cleanup_supl_mo = FALSE;

  /* check if the ongoing app-track is a MSB with active prtocol session */
  tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);
  if (LDM_CLIENT_ID_APPTRK == e_main_client_id)
  {
    pz_loc_client_info = lpc_get_loc_processing_client(e_main_client_id);
    if ((NULL != pz_loc_client_info) &&
        (TM_PRTL_TYPE_UMTS_UP == pz_loc_client_info->prtl_type))
    {
      if ((tm_core_info.config_info.q_allow_mo_supl_during_concurrency & TM_CORE_ALLOW_MO_SUPL_DURING_CONCURRENCY) == 0)
      {
        /*NV72533*/
        MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: Aborting MO-SUPL apptrack due to NV");
        v_cleanup_supl_mo = TRUE;
      }
      else
      {
        /* MO-SUPL allowed only if the ongoing E911 session is in early RCVR mode */
        if (LDM_CLIENT_ID_E911 == e_secondary_client_id)
        {
          pz_loc_client_info = lpc_get_loc_processing_client(e_secondary_client_id);
          if ((NULL != pz_loc_client_info) &&
              (TM_SESS_HANDLE_E911 == pz_loc_client_info->sessionHandle))
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: MO-SUPL apptrack and E911 early RCVR mode can be supported");
            v_cleanup_supl_mo = FALSE;
          }
          else
          {
            MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: MO-SUPL apptrack and E911 cannot be supported");
            v_cleanup_supl_mo = TRUE;
          }
        }
        else if (LDM_CLIENT_ID_MAX == e_secondary_client_id)
        {
          MSG(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM: E911 client not initialized yet. Allow MO-SUPL to continue");
          v_cleanup_supl_mo = FALSE;
        }
      }
    }
  }
  return v_cleanup_supl_mo;
}

/*
******************************************************************************
* tm_core_abort_concurrent_sessions
*
* Function description:
*  Aborts secondary and primary sessions if active
*
* PARAMS:
*  Abort reason: tm_sess_stop_reason_e_type e_abort_reason
*
* Return value: None
******************************************************************************
*/
static void tm_core_abort_concurrent_sessions(tm_sess_stop_reason_e_type e_abort_reason)
{
  ldm_client_id_e_type e_main_client_id = LDM_CLIENT_ID_MAX;
  ldm_client_id_e_type e_secondary_client_id = LDM_CLIENT_ID_MAX;
  tm_cm_phone_state_info_s_type phone_state_info = { 0 };

  tm_cm_iface_get_phone_state(&phone_state_info);

  /* Get main and secondary client from LDM */
  tm_lpc_get_main_secondary_clients(&e_main_client_id, &e_secondary_client_id);

  tm_lpc_generate_pd_event(e_secondary_client_id,e_abort_reason);

  /* Terminate the secondary client first */
  if (LPC_OP_STATUS_REQUEST_SUCCESS != tm_lpc_disable_client(e_secondary_client_id))
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
          "LDM:Failed to disable secondary client %d", e_secondary_client_id);
  }
  tm_cm_iface_set_E911_state(phone_state_info.prev_e911_state);

  /* Then Terminate the main client */
  tm_core_abort_recover(TRUE, e_abort_reason); /*lint !e506 !e730 */

  /*CR-3142996 : When RAT changes from L->W, Early rcvr session/prtl session is aborted with reason TM_STOP_REASON_LTE_TO_UMTS_CSFB,
  but this wont remove the main client client in lpc list.If no early rcvr session/prtl session is triggered on the new RAT (for eg: if 1920 disabled for W) and 
  if MO session is running as secondary client in concurrency it may fail to get a fix as no client is running TM session.
  So explicitily calling a disable for the main client below so that when Apptrack is scheduled it will start as main client*/
   
  (void)tm_lpc_disable_client(e_main_client_id);

}

/*===========================================================================
FUNCTION tm_send_sess_type_pdapi_event
DESCRIPTION
  This function sends a session type event to PDAPI

DEPENDENCIES

RETURN VALUE
  Active Sess Handle

SIDE EFFECTS

===========================================================================*/
void tm_send_sess_type_pdapi_event(uint32 sess_req_type,
                                   pdsm_pd_comm_protocol_e_type protocol_type)
{

  pdsm_ext_status_e_type e_ext_status_event = PDSM_EXT_STATUS_SESS_REQ_TYPE;
  pdsm_ext_status_info_s_type ext_status_info_type = { 0 };

  ext_status_info_type.client_id = tm_core_info.session_info.pd_param.client_id;
  ext_status_info_type.ext_status_type = PDSM_EXT_STATUS_SESS_REQ_TYPE;

  ext_status_info_type.ext_status_info.ext_session_req_type.sess_type = sess_req_type;
  ext_status_info_type.ext_status_info.ext_session_req_type.protocol_type = protocol_type;
  /*Send indication to PDAPI*/
  tm_pdapi_ext_status_event_callback(e_ext_status_event, &ext_status_info_type);
}


/*===========================================================================
FUNCTION  tm_core_send_otdoa_ext_status_event 

DESCRIPTION
  Sends a PDAPI_EXT_EVENT to PDAPI

Parameters:

  event type: the event to be sent to PDAPI
  event_structure: data to be sent for the event
===========================================================================*/
void tm_core_send_otdoa_ext_status_event(sm_loc_lte_otdoa_pdapi_event_type sess_event_type)
{
  pdsm_ext_status_e_type e_ext_status_event = PDSM_EXT_STATUS_OTDOA_SESSION_INFO_TYPE;
  pdsm_ext_status_info_s_type ext_status_info_type = { 0 };

  ext_status_info_type.client_id =  tm_core_info.session_info.pd_param.client_id;
  ext_status_info_type.ext_status_type = PDSM_EXT_STATUS_OTDOA_SESSION_INFO_TYPE;

  switch (sess_event_type)
  {
    case SM_LOC_LTE_OTDOA_PDAPI_EVENT_OTDOA_START:
      ext_status_info_type.ext_status_info.ext_otdoa_session_info_type.otdoa_validity_flags |= PDSM_OTDOA_SESS_TYPE_FLAG_VALID;
      ext_status_info_type.ext_status_info.ext_otdoa_session_info_type.e_sess_type = PDSM_OTDOA_SESS_BEGIN;
      /*Send indication to PDAPI*/
      tm_pdapi_ext_status_event_callback(e_ext_status_event, &ext_status_info_type);
      b_otdoa_sess_start_sent = TRUE;
      break;
    case SM_LOC_LTE_OTDOA_PDAPI_EVENT_OTDOA_STOP:
      if (TRUE == b_otdoa_sess_start_sent)
      {
        ext_status_info_type.ext_status_info.ext_otdoa_session_info_type.otdoa_validity_flags |= PDSM_OTDOA_SESS_TYPE_FLAG_VALID;
        ext_status_info_type.ext_status_info.ext_otdoa_session_info_type.e_sess_type = PDSM_OTDOA_SESS_DONE;
        /*Send indication to PDAPI*/
        tm_pdapi_ext_status_event_callback(e_ext_status_event, &ext_status_info_type);
        b_otdoa_sess_start_sent = FALSE;
      }
      break;
    default:
      break;
  }
}

/**
* @fn tm_HandleGnssTimeConversionInfo
* @brief Interface to report the GNSS Time Conversion params
*
* This function reports the GNSS Time conversion parameters
* used/generated by ME to PDAPI client. ME generates 1 report
* which is delivered as is to PDAPI.
*
* @param [in] pz_GnssTimeConvParamReport : GNSS time conversion
*        param report
*
* @retval None
*/
void tm_HandleGnssTimeConversionInfo(const gnss_TimeConversionParamType *pz_GnssTimeConvParamReport)
{

  pdsm_ext_status_info_s_type        ext_status_info_type = { 0 };
  pdsm_GnssTimeConversionParamType   *pz_GnssTimeParams = NULL;
  pdsm_TimeConvValidityMaskType      u_ValidMask = 0;

  if (NULL == pz_GnssTimeConvParamReport)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_ERROR, "TM_Core: Null cpt_EngMonitorStatusRpt");
    return;
  }

  if ((TM_SESS_OPERATION_MODE_MSA == tm_core_info.session_info.op_mode) ||
      (0 == (tm_core_info.config_info.t_gnss_reporting_config & PDSM_GNSS_TIME_CONV_PARAM_REPORT)))
  {
    TM_ERROR_1("Session Mode MSA %d / or TimeConversionParamInfo Report not enabled",
               (TM_SESS_OPERATION_MODE_MSA == tm_core_info.session_info.op_mode));
    return;
  }

  ext_status_info_type.client_id = tm_core_info.session_info.pd_param.client_id;
  ext_status_info_type.ext_status_type = PDSM_EXT_STATUS_TIME_CONV_PARAM_REPORT;

  /*Pointer for easy access*/
  pz_GnssTimeParams = &ext_status_info_type.ext_status_info.ext_time_conv_param;


  /* Leap Second Change Knowledge - this field is always valid */
  u_ValidMask |= PDSM_TIME_CONV_PARAM_LS_CHANGE_KNOWLEDGE;
  pz_GnssTimeParams->e_LeapSecChangeKnowledge =
     pz_GnssTimeConvParamReport->e_LeapSecChangeKnowledge;

  if (0 != pz_GnssTimeConvParamReport->z_GpsToUtcTimeConvParam.u_Valid)
  {
    /*ME sends LeasSecChangeKnowledge as part of GPS TO UTC struct
    PDAPI to LBS separates this into 2 different TLV's*/

    /* GPS to UTC */
    u_ValidMask |= PDSM_TIME_CONV_PARAM_GPS_TO_UTC;

    memscpy((void *)&(pz_GnssTimeParams->z_GpsToUtcTimeConvParam.z_UtcInfo),
            sizeof(pdsm_GpsUtcInfoRptStructType),
            (const void *)&(pz_GnssTimeConvParamReport->z_GpsToUtcTimeConvParam.z_UtcInfo),
            sizeof(gps_UtcInfoRptStructType));

    pz_GnssTimeParams->z_GpsToUtcTimeConvParam.x_TimeofLeapSecChange =
       pz_GnssTimeConvParamReport->z_GpsToUtcTimeConvParam.x_TimeofLeapSecChange;
  }

  /* GLO to UTC and GPS */
  if (0 != pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.z_Mask.u_FilledMask)
  {
    u_ValidMask |= PDSM_TIME_CONV_PARAM_GLO_TO_UTC_GPS;

    if (0 != pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.z_Mask.z_FilledMask.u_N4Flag)
      pz_GnssTimeParams->z_GloToUtcGpsTimeConvParam.u_N4 =
         pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.u_N4;

    if (0 != pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.z_Mask.z_FilledMask.u_KpFlag)
      pz_GnssTimeParams->z_GloToUtcGpsTimeConvParam.u_Kp =
         pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.u_Kp;

    if (0 != pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.z_Mask.z_FilledMask.u_NtFlag)
      pz_GnssTimeParams->z_GloToUtcGpsTimeConvParam.w_Nt =
         pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.w_Nt;

    if (0 != pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.z_Mask.z_FilledMask.u_TaoGpsFlag)
      pz_GnssTimeParams->z_GloToUtcGpsTimeConvParam.q_TaoGps =
         pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.q_TaoGps;

    if (0 != pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.z_Mask.z_FilledMask.u_FreqNumFlag)
      pz_GnssTimeParams->z_GloToUtcGpsTimeConvParam.b_FreqNum =
         pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.b_FreqNum;

    pz_GnssTimeParams->z_GloToUtcGpsTimeConvParam.u_Src =
       pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.u_Src;
    pz_GnssTimeParams->z_GloToUtcGpsTimeConvParam.q_TauC =
       pz_GnssTimeConvParamReport->z_GloToUtcGpsTimeConvParam.q_TaoC;
  }

  /* GAL to UTC */
  if (0 != pz_GnssTimeConvParamReport->z_GalToUtcTimeConvParam.u_Valid)
  {
    u_ValidMask |= PDSM_TIME_CONV_PARAM_GAL_TO_UTC;

    memscpy((void *)&(pz_GnssTimeParams->z_GalToUtcTimeConvParam.z_GalUtcInfo),
            sizeof(pdsm_GalUnscaledUtcOffsetStructType),
            (const void *)&(pz_GnssTimeConvParamReport->z_GalToUtcTimeConvParam.z_GalUtcInfo),
            sizeof(gal_UnscaledUtcOffsetStructType));
  }

  /* GAL to GPS */
  if (0 != pz_GnssTimeConvParamReport->z_GalToGpsTimeConvParam.u_Valid)
  {
    u_ValidMask |= PDSM_TIME_CONV_PARAM_GAL_TO_GPS;

    pz_GnssTimeParams->z_GalToGpsTimeConvParam.u_Wn0G =
       pz_GnssTimeConvParamReport->z_GalToGpsTimeConvParam.u_Wn0G;
    pz_GnssTimeParams->z_GalToGpsTimeConvParam.u_T0G =
       pz_GnssTimeConvParamReport->z_GalToGpsTimeConvParam.u_T0G;
    pz_GnssTimeParams->z_GalToGpsTimeConvParam.x_A0G =
       pz_GnssTimeConvParamReport->z_GalToGpsTimeConvParam.x_A0G;
    pz_GnssTimeParams->z_GalToGpsTimeConvParam.x_A1G =
       pz_GnssTimeConvParamReport->z_GalToGpsTimeConvParam.x_A1G;
    pz_GnssTimeParams->z_GalToGpsTimeConvParam.w_GalWeek =
       pz_GnssTimeConvParamReport->z_GalToGpsTimeConvParam.w_GalWeek;
    pz_GnssTimeParams->z_GalToGpsTimeConvParam.q_GalMsec =
       pz_GnssTimeConvParamReport->z_GalToGpsTimeConvParam.q_GalMsec;
  }

  /* BDS to UTC */
  if (0 != pz_GnssTimeConvParamReport->z_BdsToUtcTimeConvParam.u_Valid)
  {
    u_ValidMask |= PDSM_TIME_CONV_PARAM_BDS_TO_UTC;

    memscpy((void *)&(pz_GnssTimeParams->z_BdsToUtcTimeConvParam.z_BdsUtcInfo),
            sizeof(pdsm_BdsUnscaledUtcOffsetStructType),
            (const void *)&(pz_GnssTimeConvParamReport->z_BdsToUtcTimeConvParam.z_BdsUtcInfo),
            sizeof(bds_UnscaledUtcOffsetStructType));
  }

  /* BDS to GPS/GLO/GAL */
  if (0 != pz_GnssTimeConvParamReport->z_BdsToUtcTimeConvParam.u_Valid)
  {
    u_ValidMask |= PDSM_TIME_CONV_PARAM_BDS_TO_GNSS;
    pz_GnssTimeParams->z_BdsToGnssTimeConvParam.w_BdsWeek =
       pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.w_BdsWeek;
    pz_GnssTimeParams->z_BdsToGnssTimeConvParam.q_BdsMsec =
       pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.q_BdsMsec;

    /* bits: 0 -- GPS-related offset is valid
    1 -- GLO-related offset is valid
    2 -- GAL-related offset is valid
    3-7 -- unused */
#define GNSS_GPS_OFFSET_VALID (0x01)
#define GNSS_GLO_OFFSET_VALID (0x02)
#define GNSS_GAL_OFFSET_VALID (0x04)

    /*GPS-related offset*/
    if (GNSS_GPS_OFFSET_VALID & pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.u_TimeOffsetValidMask)
    {
      pz_GnssTimeParams->z_BdsToGnssTimeConvParam.x_A0Gps =
         pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.x_A0Gps;
      pz_GnssTimeParams->z_BdsToGnssTimeConvParam.x_A1Gps =
         pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.x_A1Gps;
    }
    /*GLO-related offset*/
    if (GNSS_GLO_OFFSET_VALID & pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.u_TimeOffsetValidMask)
    {
      pz_GnssTimeParams->z_BdsToGnssTimeConvParam.x_A0Glo =
         pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.x_A0Glo;
      pz_GnssTimeParams->z_BdsToGnssTimeConvParam.x_A1Glo =
         pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.x_A1Glo;
    }
    /*GAL-related offset*/
    if (GNSS_GAL_OFFSET_VALID & pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.u_TimeOffsetValidMask)
    {
      pz_GnssTimeParams->z_BdsToGnssTimeConvParam.x_A0Gal =
         pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.x_A0Gal;
      pz_GnssTimeParams->z_BdsToGnssTimeConvParam.x_A1Gal =
         pz_GnssTimeConvParamReport->z_BdsToGnssTimeConvParam.x_A1Gal;
    }
  }

  /* Set the validity Mask for the payload*/
  pz_GnssTimeParams->u_ValidMask = u_ValidMask;

  /*Send indication to PDAPI*/
  tm_pdapi_ext_status_event_callback(PDSM_EXT_STATUS_TIME_CONV_PARAM_REPORT, &ext_status_info_type);
}
/*===========================================================================
FUNCTION  tm_core_abort_apptrack_in_concurr_state 

DESCRIPTION
  Aborts apptrack session in concurrent state

Parameters:
  None
===========================================================================*/
void tm_core_abort_apptrack_in_concurr_state(void)
{
  tm_loc_processing_client_info              *p_z_lpc_client;
  tm_cm_phone_state_info_s_type               phone_state_info = { 0 };

  p_z_lpc_client = lpc_get_loc_processing_client(LDM_CLIENT_ID_APPTRK);

  if (NULL == p_z_lpc_client)
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC: No apptrack client to disable");
    return;
  }

  /* Terminate apptrack when its secondary client */
  if (LDM_CLIENT_TYPE_SECONDARY == p_z_lpc_client->e_clientId_type)
  {
    MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_MED, "LDM:Apptrack session stop reason %d; generate PD event", TM_STOP_REASON_E911);
    (void)tm_lpc_generate_pd_event(LDM_CLIENT_ID_APPTRK, TM_STOP_REASON_E911);

    if (LPC_OP_STATUS_REQUEST_SUCCESS != tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK))
    {
      MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_ERROR,
            "LDM:Failed to disable secondary client %d", LDM_CLIENT_ID_APPTRK);
    }
  }
  else if (LDM_CLIENT_TYPE_MAIN == p_z_lpc_client->e_clientId_type)
  {
    /* Terminate apptrack when its main client */
    tm_core_abort_recover(TRUE, TM_STOP_REASON_USER_TERMINATED); 

    /* Disable apptrack if not already done (when no apptrack session in progress)*/
    (void)tm_lpc_disable_client(LDM_CLIENT_ID_APPTRK);

    /* Initiate switch if still in concurrent mode */
    tm_cm_iface_get_phone_state(&phone_state_info);
    if (TM_CM_E911_STATE_CONCURRENT == phone_state_info.e911_state)
    {
      (void)tm_core_switch_secondary_to_main_client(LM_MGP_RCVR_OFF_TM_SESS_END, FALSE);
    }
  }
  else
  {
    MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "LPC:No apptrack client to disable");
  } 
}

/*===========================================================================
FUNCTION tm_core_set_e911_ni_supl_on_dv

DESCRIPTION
Sets the flag to TRUE if ndds 911 SUPL session is in progress

DEPENDENCIES

RETURN VALUE
                                                 
SIDE EFFECTS

===========================================================================*/ 
void tm_core_set_e911_ni_supl_on_dv(boolean v_e911_ni_supl_on_dv)

  {
  MSG_1(MSG_SSID_GPSSM, MSG_LEGACY_HIGH, "Set E911 NI SUPL on DV sub flag : %d",v_e911_ni_supl_on_dv);
  tm_core_info.session_info.v_e911_ni_supl_on_dv =  v_e911_ni_supl_on_dv;
  }

/*===========================================================================
FUNCTION tm_core_is_ni_e911_supl_on_dv_in_progress
                                        
DESCRIPTION
returns TRUE if Emergency nDD NI SUPL session is in progress 

DEPENDENCIES

RETURN VALUE
  TRUE if NI SUPL on DV is in progress
  FALSE in other cases
                                                 
SIDE EFFECTS

===========================================================================*/ 
boolean tm_core_is_e911_ni_supl_on_dv_in_progress(void)
{
  return tm_core_info.session_info.v_e911_ni_supl_on_dv;
}                                             

/*=============================================================================
  
FUNCTION
  tm_core_report_pdr_mag_cal_state
  
DESCRIPTION
  This function handles the calibration state of Magnetometer received from PE.
  
DEPENDENCIES
  None.
  
PARAMETERS
  sm_MagCalStateStructType - Pointer to the structure,
  pz_MagCalState containing Magnetometer's Calibrated State
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.
  
=============================================================================*/

void tm_core_report_pdr_mag_cal_state(sm_MagCalStateStructType *pz_MagCalState)
{
  MSG(MSG_SSID_GPSSM, MSG_LEGACY_HIGH,"tm_core_report_pdr_mag_cal_state: Received Magnetometer's Calibration State");
  /*
    To-do:
    Handle Mag Cal State IPC from PE.
  */
}
