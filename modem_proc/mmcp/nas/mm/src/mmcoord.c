/*===========================================================================
            COPYRIGHT INFORMATION

Copyright (c) 2001,2002,2003 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mm/vcs/mmcoord.c_v   1.35   18 Jul 2002 11:38:12   vdrapkin  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/mm/src/mmcoord.c#2 $ $DateTime: 2021/08/10 07:44:19 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
09/10/14   am      Passing internal cause for internal error while RAB reestablishment
11/28/12   am      Fixing compilation warning. 
09/17/12  us       Added handling for LLC suspension comfirmation in OOS situations.  
12/21/11   gps     FREE FLOAT FEATURE changes       
11/24/11   gps     pended stop_mode_req until abort procedure completes   
10/20/11   rajesh   Added code to support LPM request from CM during LTE RLF when active rat is GW.         
30/08/11   MNK     Add dual RX CSFB support 
08/26/11  rajesh   Code changes to send service indication to CM if interrat to GW from LTE
                   because of CSFB.
08/01/11  rajesh   Corrected esle if check in "mmcoord_process_rr_plmn_select_cnf"
07/21/11  abhi     MM/EMM to remain in OOS/RLF handler if RRC_SERVICE_CNF is received when 
                               stop_mode_cnf is pending       
07/21/11  rajesh   Made changes for memory alloction for mm_wrlf_serving_plmn_ptr,
                   mm_wrlf_pending_rr_message_ptr and mm_pending_mmr_ps_detach_req_ptr.
07/19/11  rajesh   Made changes to reset "reg_req_pended_indication" in OOS/RLF handler whenever
                   we receive plmn list request.
07/07/11   rajesh  Changes made for implementing PS detach timer in NAS.
07/06/11  mrk      Support of Ciphering enabled Indication for CS Voice calls
 07/05/11  sla     In case of ENS SIM,MM will abort the connection when t3240 is running     
07/04/11  abhi     ph_status_change_req will be handled directly by EMM in LTE only build   
06/24/11   rajesh  Made changes to send disable bplmn ind with status FALSE whenever
                         WRLF scenario ends.
08/13/10   MNK     Reset reg_req_pended_indication upon deactivate_cnf 
06/21/10   zren    Updated EMM to include correct MME routing info in conn est 
                   req for TAU after IRAT         
05/14/10   zren    Updated to reset mm_send_mmr_stop_mode_cnf_fp when received 
                   LTE_RRC_DEACTIVATE_CNF during OOS/RLF
05/12/10   zren    Updated to move EMM to EMM_REGISTERED_UPDATE_NEEDED state 
                   when received GW STOP MODE CNG in EMM REGISTERED state
03/11/10   MNK     added LTE NAS EFS read if sim read is successful
04/12/01   jca     Began GMM integration.
04/24/01   sbk     Moved RRC conn establishment functionality to mmrrcconn.c
05/03/01   jca     Added many new functions.
05/11/01   jca     Added GMM timers and call to gmm_load_sim_data().
05/15/01   sbk     Fixed a bug where RRC_SYNC_IND used istead of MM_PAGE_IND
05/18/01   sbk     Pass-on RRC_SERVICE_CNF to MM/GMM in case of RRC_SERVICE_NOT_AVAILABLE
05/23/01   jca     Added functions to process RRC_SERVICE_IND and RRC_CLOSE_SESSION_CNF
06/08/01   jca     Added functions to process MMR_NREG_REQ and RRC_DEACT_CNF.
06/13/01   jca     Deleted RRC_UNIT_DATA_IND and RRC_CLOSE_SESSION_CNF primitives.
                   Added new functions so MMCOORD would work with RR in GSM mode.
                   Added error checking of RRC, RR and REG primitives.
06/18/01   jca     Now sending MMR_ACT_CNF.
08/03/01   jca     Fixed misc bugs found during unit test.
09/18/01   sbk     Intertask primitives are logged and QXDM logging is supported
                   For NREG_REQ messages, do not check the service type on power-down
10/19/01   sbk     Process T3212 timer at the end of cmd processing. Use the new mm_plmn_match
                   for matching plmns
02/20/02   sbk     RRC_DATA_IND is now routed based on the CN domain. Added TC routers
04/30/02   ks      Added processing of timer TIMER_DETACH_REL.
05/02/02   vdr     Added initialization of the MM/GMM security context
06/05/02   ks      Added support for Dual mode operation, Consolidated mmcoord
                   plmn,lac,rac information into single Serving_plmn entity.
06/18/02   ks      moved calling of mm_send_mmr_sim_available_cnf function
                   to the end of mmcoord_process_mmr_sim_available_req()
06/20/02   jca     Only call mm_activate_state() function if MM state = NULL.
07/01/02   jca     Added processing of RR_ABORT_CNF and RR_STOP_GSM_MODE_CNF.
07/03/02   jca     Deleted timers TIMER_PLMN_SELECT and TIMER_PLMN_LIST.
07/08/02   vdr     Cahnged processing of the RR_SERVICE_IND and RR_PLMN_SELECT_CNF according
                   to the requirements of the GMM-LLC interface
07/09/02   ks      Cipher key & cipher key sequence number are now set correctly
                   in rr_sim_inserted_req()
07/12/02   ks      Added checking for mm_sim_card_mode mode before reading the
                   SIM data for rr_sim_inserted_req primitive.
07/18/02   vdr     Renamed FEATURE_GPRS to be FEATURE_GSM_GPRS compilation flag
                   Changed routing of the RR primitives when the MS is operating
                   in GSM/GPRS mode
07/23/02   ks      Added check for RAT or Service change request handling
                   Added new function mmcoord_process_lu_procedure().
08/02/02   ks      Saving the mmr_reg_req message in mmcoord_pending_reg_message.
08/03/02   mks     Added processing for rr_activation_ind
                   messages to support Inter-RAT handover/cell reselection.
08/07/02   ks      Setting the default value for 'cause' field in MMR_REG_CNF
08/13/02   ks      Added HANDOVER cause check during the validation of RR cmd.
08/13/02   mks     Now sending RRC_SIM_INSERTED_REQ to RRC
08/15/02   ks      Checking the SIM card type and then accessing either
                   BCCH or CPBCCH.
08/26/02   jca     Now set mm_service_state to SERVICE_NOT_AVAILABLE when
                   SERVICE_IND is received indicating NO_SERVICE.
08/20/02   cd      In mmcoord_process_mmr_sim_available_req()
                   - corrected populating the gprs_ciphering_key in the
                     rr_sim_inserted_req primitive.
                   - Added setting the ptmsi from SIM values. Added gmm_int.h
09/03/02   mks     Now calling mmsim_get_bcch_information() to access BCCH/CPBCCH
                   allocation information in the SIM.
                   Updated mmcoord_process_rr_service_ind() not to set
                   mm_service_state to SERVICE_NOT_AVAILABLE when SERVICE_IND is
                   received with NO_SERVICE.
09/10/02   ks      If network selection mode is SYS_NETWORK_SELECTION_MODE_LIMITED
                   no need to check if we already are camped on a particular PLMN,
                   because the RR will perform any cell selection in this case.
09/11/02   ks      Removed setting the mm_serving_plmn.mm_service_state
                   to SERVICE_NOT_AVAILABLE, this is already being set in
                   mmumts_process_cell_change() after setting previous_service_state.
09/23/02   mks     Added processing for MMR_ACT_REQ from REG
10/01/02   ks      Added support for the new GSDI interfaces.
10/03/02   mks     Added diagnostic F3 messages to log service state from
                   RR_PLMN_SELECT_CNF & RR_SERVICE_IND primittives
10/07/02   jca     Changes to support CS/PS combined attach procedures.
10/09/02   vdr     Added invalidation of the CS and PS CKSN upon UMTS activation
                   when the MS is running GSM SIM
10/10/02   mks     Now processing RRC_ACTIVATION_IND primitive from RRC, corrected
                   process_rr_service_ind() to use rr_service_ind message instead
                   of rr_plmn_select_cnf while populating the LAC value in the
                   serving PLMN.
10/11/02   ks      Processing the T3212 timer only if the UE is in CS mode only.
                   Added support for Emergency call handling.
10/11/02   cd      In mmcoord_valid_rrc_command(), update validation of RRC_SYNC_IND
                   based on newe RRC_HANDOVER cause
                   Fixed lint errors complaining about negative indentation
10/31/02   ks      Set the BCCH list as not supported, as it is not used by RR.
11/06/02   jca     Fixed code to report limited service status when PLMN found
                   to provide service that does not match requested domain.
11/08/02   cd      Added support for AC blocking
                   Removed lint errors
11/12/02   jca     Added support for RRC_CLOSE_SESSION_CNF.
11/14/02   ks      Added support for DEEP_SLEEP & MODE_CHANGE reasons in
                   mmcoord_process_mmr_stop_mode_req.
11/14/02   ks      Changed order of loading the SIM information.
11/19/02   ks      Added handling of MM_RR_WCDMA_GSM_HO cause in RR_SYNC_IND.
11/25/02   cd      In mmcoord_process_rr_plmn_select_cnf() added check to see
                   if reg is waiting for a confirmation before we send one.
11/25/02   cd      In mm_process_rrc_service_cnf() corrected processing when there is
                   class blocking
11/25/02   mks     Added a check in mmcoord_process_mmr_plmn_search_req() to see
                   if the incoming PLMN search request from REG is an implicit RAT
                   change invocation.
11/26/02   ks      Mapping the Deep sleep cause from REG to RR in
                   mmcoord_process_mmr_stop_mode_req().
12/06/02   jca     Changes to support PS_ONLY<->CS_ONLY mode switches in GSM mode.
12/06/02   mks     Added support for GPRS suspension procedure - Added code to
                   route GPRS suspension notification to GMM.
12/10/02   jca     Set MM substate to PLMN_SEARCH when commanding RR/RRC to
                   search for service.
12/12/02   ks      Replaced mm_sim_valid with mm_serving_plmn.info.sim_state.
01/10/03   jca     Changes to support CS_ONLY<->CS/PS mode switching.
01/10/03   mks     Now properly handling MMR_STOP_MODE_REQ when it is received
                   before SIM initialization.
01/13/03   jca     Now passing RR_SERVICE_IND to both MM and GMM when ms_op_mode
                   equals CS_PS and network op mode info is not available.
01/27/03   jca     Changes to support PS_ONLY<->CS/PS mode switching.
                   Changes to support IMSI Detach via Combined GPRS Detach procedure.
02/07/03   jca     Now sending PLMN search request to RR(C) if dual_mode flag in
                   MMR_REG_REQ changes it's value.
02/07/03   ks      Added support for MODE_CHANGE reason in processing in the
                   MMR_STOP_MODE_REQ primitive.
02/12/03   cd      Modified MSG_HIGH length so that info gets printed in the logs.
                   Added cell_access info to MSG_HIGH for RRC/RR_SERVICE_IND.
                   Added cause info to MSG_HIGH for RRC_SYNC_IND
02/13/03   jca     Ignore RR(C)_SERVICE_IND if waiting for PLMN search confirmation.
02/21/03   ks      Added GSM Deep Sleep support in mmcoord_process_mmr_stop_mode_req.
02/21/03   vdr     Added processing of the new CNM (MS_CC_MM_PS) message set and
                   WMS' requests
02/27/03   vdr     Added RR_REL_IND processing during UMTS to GSM RAT change and
                   routing if the CM primitives.
03/06/03   jca     Placed some FEATURE_WCDMAs around some code to avoid fatal
                   crash when software built for GSM-only.
03/07/03   vdr     Featured out (FEATURE_GSM_GPRS) processing of the MS_CM_MM primitives
04/03/03   cd      Added support for Equivalent PLMN list in mmr_reg_cnf
04/16/03   vdr     Modified auth_handle_sim_authentication_response() function call
                   according to new interface
04/29/03   jca     Added support for change of network mode of operation.
05/01/03   jca     Ignore RR_PLMN_SELECT_CNF if RR_PLMN_SELECT_REQ not pending.
05/23/03   jca     Corrected problems with handling of gmm_previous_network_op_mode.
                   Added support for PS on demand during CS active connection.
06/04/03   ks      Added support for RRC_MODE_CHANGE_CNF for MODE_CHANGE
                   support for UMTS.
06/16/03   jca     Now consistently ERR_FATAL if attempt to allocate memory fails.
07/10/03   jca     If network selection mode is SYS_NETWORK_SELECTION_MODE_LIMITED
                   no need to check if PLMN matches on receipt of RRC_SERVICE_CNF.
07/11/03   mks     Stop Mode procedure now overrides any pending RAT change procedures.
07/16/03   jca     Added support for new RR_GMM_GPRS_RESUMPTION_IND primitive.
07/29/03   jca     Fixed PS Attach-on-demand problem related to clearing of the
                   mm_serv_change_pending boolean flag.
07/31/03   jca     Added input parameter to function mm_activate_state() to indicate
                   whether call to function was due to RAT change.
08/29/03   cd      Added support for passing the band_preference between REG and RR
09/10/03   jca     Now routing RR(C)_SERVICE_IND to GMM if ms_op_mode equals CS_ONLY
                   but gmm_state is set to GMM_DEREGISTERED_INITIATED.
09/26/03   mks     Now processing abort cause RRC_CONNECTION_NOT_AVAILABLE from RRC.
09/30/03   mks     Now processing cause RR_ABORT_REDIAL_NOT_ALLOWED in RR_ABORT_IND.
10/03/03   jca     Added support for network selection mode
                   SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION.
                   Fixed bug where gmm_previous_network_op_mode was not being
                   saved on a loss of service condition.
11/17/03   vdr     Now processing HFN start values upon reception of the RRC_DEACT_CNF,
                   RRC_ABORT_IND and RRC_ABORT_CNF primitives
11/17/03   mks     Added support for TS24.008 specification CR #359.
11/25/03   jca     If a RR/RRC connection is established when MMR_REG_REQ is
                   received, abort the connection before processing the REG_REQ.
12/04/03   jca     Now processing new gsm_map_common_info_incl flag in
                   RRC_SERVICE_CNF and RRC_SERVICE_IND.
12/12/03   ks      Storing GSM sys info by calling mmgsm_store_system_information
                   after processing PLMN_SELECT_CNF, after proper substate transition.
01/21/04   cd      Checking the if the requested RAT is supported before processing a
                   MMR_PLMN_SEARCH_REQ
01/22/04   ks      If gmm_follow_on_request is TRUE do not abort the RRC connection
                   when requesting for a service request.
02/05/04   ks      Now setting the gmm_follow_on_request from the MMR_REG_REQ.
02/13/04   ks      Setting the gmm_follow_on_request from the MMR_REG_REQ only if
                   REG is adding a PS service.
02/27/04   ks      Also setting gmm_follow_on_request when REG requests from
                   NO_SERVICE to PS only service.
03/24/04   vdr     Reset mmcoord_reg_req_pending flag upon reception of the
                   MMR_STOP_MORE_REQ primitive
04/06/04   cd      LCS support
05/07/04   jca     Clear the gmm_follow_on_request flag when a MMR_REG_REQ
                   is received indicating CS-only service request.
05/12/04   cd      Added support for WCDMA multiband feature
05/13/04   cd      Enhancements to CS FOR. Now using the cs_for_request in the
                   mmr_reg req to set the mm_follow_on_request.
05/19/04   cd      Removed dependency on FEATURE_CS_FOLLOW_ON_REQUEST
06/07/04   jca     MM now effectively rejects MMR_REG_REQs while in any MM state
                   associated with having a CS call active or in process of
                   setup or teardown.
06/08/04   cd      LCS support.  Removed validation of rr command for
                   rr_lcs_active_ind
06/14/04   ks      Reset the mm_serv_change_pending to FALSE and revert the MS
                   op mode to registered mode when processing stop_mode_req.
06/23/04   ks      Now storing the ATT,MSCR,SGSNR and cell access information
                   after receiving rr_plmn_select_cnf/rr_service_ind and before
                   calling the mm_handle_msg() and gmm_handle_msg().
06/24/04   jca     rrc_service_cnf.service_info.gsm_map_common_info_incl only applies to
                   data field rrc_service_cnf.service_info.gsm_map_common_info.value.
07/27/04   jca     Abort RR connection if one is active if MMR_REG_REQ is received
                   indicating a change of PLMN (i.e., instead of rejecting REG_REQ).
07/27/04   jca     Abort RR connection if one is active if MMR_REG_REQ is received
                   indicating change of RAT (i.e., instead of rejecting REG_REQ) when
                   LU attempt is in-progress.
07/28/04   jca     rrc_service_ind.service_info.gsm_map_common_info_incl only applies to
                   data field rrc_service_ind.service_info.gsm_map_common_info.value.
07/30/04   jca     Now sending PLMN search request to RR(C) if band_pref mask
                   in MMR_REG_REQ changes it's value.
07/30/04   jca     Abort an LU in-progress if the user changes the requested
                   service domain to PS only.
08/12/04   cd      Replaced FEATURE_LCS by FEATURE_MMGPS
08/25/04   npr     Convey new RR_RESELECTION_IND message.
10/01/04   jca     Ensure mmcoord_pending_reg_message is a MMR_REG_REQ prior to
                   calling function mmcoord_process_mmr_reg_req().
11/16/04   jca     Added delay_sig_con_release flag in RRC_ABORT_REQ.
11/18/04   vdr     Now routing RR_PLMN_SELECT_CNF to gmmllcm.c
11/19/04   jca     Don't store MMR_PLMN_SEARCH_REQ in mmcoord_pending_reg_message
                   structure until it is firmly established that a RAT change
                   will actually occur (i.e., command will not be rejected).
11/16/04   sv      UE performing only RAU and not LU upon change in the LAC.
                   Interface change between RRC and MM - added a new primitive to handle
                   specific scenario's pertaining to handling of CN Info.
12/17/04   vdr     Added authentication cache memory reset upon RRC_SYNC_IND for the
                   following cases: RRC_CIPHERING_ESTABLISHED, RRC_INTEGRITY_ESTABLISHED,
                   RRC_INTEGRITY_CIPHERING_ESTABLISHED, and also when TIMER_T3316 or
                   TIMER_T3218 expire
02/03/05   sv      Removing unwanted checks in validation of RRC_SERVICE_IND_FROM_OTA_MSG
03/31/05   hj      Added support for sending CM_CIPHER_IND to cm, from function
                   mmcoord_process_rrc_sync_ind.
04/12/05   hj      Changed call to mm_send_cm_cipher_ind to reflect new
                   enum type for cipher_domain.
04/12/05   jca     Clear the mm_rat_change_pending and mm_serv_change_pending flags
                   before processing a pending MMR_REG_REQ when RR(C) connection active.
05/04/05   hj      Now aborting RR(C) connection before processing MMR_REG_REQ (for change
                   of domain from CS_PS to PS_ONLY) if mm status indicates not camped.
05/23/05   abe     Set the value of the field auth_failed_bar_cells to FALSE in RR(C)_ABORT_REQ
06/06/05   abe     Added code for CCBS support
06/14/05   vdr     Now invalidating CS & PS CKSN only when UMTS service is available
                   and UE uses GSM SIM
07/21/05   up      Updated mmcoord_valid_reg_command for MMR_EQ_PLMN_CHANGE_IND.
07/22/05   abe     Added handling for the primitive RRC_WTOG_CCO_COMPLETE_IND
                   for the feature FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER
08/14/05   hj      Added handling for monitoring CCCH paging channel when detached from
                   GPRS Services.
08/18/05   jca     Ensure MMR_REG_CNF is first response sent to REG task after a
                   MMR_REG_REQ is received (instead of a MMR_SERVICE_IND).
09/02/05   abe     Sending Location status event to GSTK incase of NO SERVICE in Power on
09/16/05   up      To take care of sending service status change info to GSTK, 
                   gmm_update_mm_serving_plmn_info is used to set service status.
09/09/05   ajt     Update mm_prev_cell_access before processing RRC_SERVICE_IND,
                   RR_SERVICE_IND, RRC_SERVICE_IND_OTA_MSG    
09/16/05   kvk     Now FOR bit will set in GMM attach request if ps_for_request is set to 
                   TRUE even in the ms_op_mode CS+PS
09/23/05   up      Make the MS to LIMITED if registration is not initiated
                   in LIMITED_MANUAL_ACQ mode.
10/08/05   abe     Added code for FEATURE_INTERRAT_PCCO_GTOW
10/10/05   abe     If requested servcie domain is NO_SRV then reject the REG_REQ 
                   with service status NO_SRV and cause NOT_SPECIFIED.
10/11/05   vdr     Now calling mm_send_gstk_no_service_info() when UE goes into deep sleep 
10/16/05   ks      Now sending Location status event to GSTK when LAI changes in
                   LIMITED_SERVICE or NO_IMSI substates.    
10/21/05   up      Reject REG_REQ during PS call if service requested on RPLMN.
10/24/05   up      Send net selection mode in MMR_PLMN_SEARCH_CNF when rejected.
11/04/05   abe     MM should keep MMR_REG_REQ pending while it is waiting for 
                   STOP_MODE_CNF from lower layer. 
11/16/05   hj      Set the PS FOR bit if UE was not registered for PS services 
                   and REG set FOR bit to TRUE.
11/28/05   abe     Guarded the GSTK call with feature flag FEATURE_GSTK
12/01/05   abe     Ignore the pending PLMN LIST REQ if REG_REQ is received.
12/02/05   abe     No need to send mm idle state event to GSTK for NO SERVICE 
                   after receiving RRC_SERVICE_CNF and RR_PLMN_SELECT_CNF               
12/14/05   abe      Supported changes for FEATURE_GSM_DTM
01/02/06   up      Pass the PLMN_SEARCH_ABORT_REQ to RR & RRC to abort
                   Manual BPLMN Search.
02/13/06   up      Not to abort the RR/RRC connection when MANUAL REG_REQ 
                   received on same PLMN.
04/06/06   sn      DTM cell_service_ind changes.
05/09/06   up      If UE mode is changed from WCDMA Only to WCDMA preferred
                   inform the RRC about mode/band changes using new primitive.
06/14/06   up      Called new mm_update_mm_serving_plmn_info for GSTK event.
06/09/06   sn      Added handling of SYS_STOP_MODE_REASON_LPM.
07/14/06   pk     Updated for Feature DSAC
07/27/06   sn      Added Uniform OOS changes.
                   FEATURE_UMTS_UNIFORM_OOS_HANDLING
08/21/06   up      Send CAMP_IND to CM/SD even when UE is already camped on.
                   This will alow CM/SD to fwd the pending call.
15/09/06   sn      Ignore RR_SERVICE_IND if waiting for STOP_MODE_CNF.
09/21/06   pk      Updated for FEATURE_GSM_EDTM
10/13/06   up      Change GMM state to DEREGISTERED when op mode is changed to CS_ONLY.
11/23/06   sn      FEATURE_UMTS_BACKGROUND_FULL_SERVICE_SEARCH
12/18/09   RI      Updates for combined EFS/NV/USIM reads for multimode.
02/12/10   zren    Updated to always abort the existing connection before 
                   sending RRC DEACT REQ

02/16/10   MNK     Add check for processing GW activation indication if in LTE
23/08/11   SLA      Send CELL SERVICE IND to CM only if service status is not blocked at REG
08/25/11   jbk       Stop sending SIM INSRTD to lower layers if SIM load fails
09/21/11   sla     UE will not process RRC_SERVICE_CNF/RR_PLMN_SELECT_CNF when
                   it is waiting for stop mode cnf from the AS       
10/05/11   gps     Initiated rr sim inserted req primitive with all zeros
10/21/11   gps     initiated some primitives with zeros
09/26/11   rajesh  If MM receives "MMCNM_REL_REQ" from CNM when active rat is LTE and UE is
                   in WRLF state then process this message in MM instead of EMM.
11/24/11   gps     pended stop_mode_req until abort procedure completes
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmcp_variation.h"
#include "customer.h"
#include "rex.h"
#include "sys_type.h"
#include "amssassert.h"
#include "sys_cnst_v.h"
#include "sys_stru_v.h"
#include "ms.h"
#include "timers_v.h"
#include "gs_v.h"
#include "ms_timer_v.h"
#include "mm_rr.h"
#include "mm_mm_v.h"
#include "mm_v.h"
#include "mmtask.h"
#include "mmtask_int.h"
#include "mm_umts_v.h"
#include "mm_sim.h"
#include "gmm_v.h"
#include "cm.h"
#include "err.h"
#include "rrcmmif.h"

#if defined (FEATURE_CMCC_SECURE_CALL) || defined(FEATURE_TDSCDMA)
#include "tdsrrcmmif.h"
#endif

#include "cause.h"
#include "log.h"
#include "naslog_v.h"
#include "mmauth.h"
#include "mmrat.h"
#include "gmm_int_v.h"
#include "sm_v.h"
#include "err.h"
#include "mmsecurity.h"
#include "gmmsecurity.h"
#include <stringl/stringl.h>
#include "cm_v.h"
#ifdef FEATURE_LTE
#include "extern_mmtask.h"
#include "emm_database.h"
#endif
#include "mmgsdilib_common.h"
#include "mm_coord.h"
#ifdef FEATURE_GSM_GPRS
#include "gmmllcm.h"
#endif
#ifdef FEATURE_GSTK
#include "mm_gstk.h"
#endif
#include "sys_eplmn_list_v.h"
#ifdef FEATURE_ENHANCED_NW_SELECTION
#include "reg_sim_v.h"
#endif
#include "bit.h"
#include "modem_mem.h"

#include "reg_mm_v.h"
#include "reg_mode.h"
#include "reg_nv.h"

#ifdef FEATURE_TDSCDMA
#include "cm.h"
#endif

#ifdef FEATURE_SEGMENT_LOADING
#include "mcpm_api.h"
#include "mcfg_seg_load.h"
#include "ITDSCDMA.h"
#include "IWCDMA.h"
#include "IWCDMA_ext.h"
#endif

#ifdef FEATURE_MODEM_CONFIG_REFRESH
#include "mcfg_refresh.h"
#endif

#ifdef FEATURE_LTE
#include "emm_timer_sigs.h"
#include "emm_database.h"
#include "emm.h"
#include "emm_reg_handler.h"
#include "emm_esm_handler.h"
#include "esm_v.h"
#include "emm_rrc_handler.h"
#include "emm_security.h"
#include "emm_sim_handler.h"
#include "naslog_v.h"
#include "event.h"
#include "event_defs.h"
#include "emm_utility.h"
#include "mm_multimode_common.h"
#include "emm_mem.h"
#include "emm_esm_handler.h"
#include "emm_cnm_handler.h"
#include "emm_service_request_lib.h"
#include "emm_csfb_handler.h"
#include "emm_irat_if_msg.h"
#include "mmgsdisessionlib_v.h"
#include "lte_rrc_irat_msg.h"
#include "lte_nas_tlb.h"
#include "fs_public.h"
#include "mcfg_fs.h"
#include "fs_lib.h"
#include "reg_state.h"
#include "qsh.h"


extern void gmm_process_refresh_stage_ind( mm_cmd_type *message );
extern void emm_process_timer_expiry(mm_cmd_type *cmd_ptr, emm_ctrl_data_type *emm_ctrl_data_ptr);
void process_rat_change_req(mm_cmd_type *cmd_ptr,mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr);
extern void mm_reset_ul_count_at_generation(void);
#endif

// Transaction ID for stop mode request between REG & MM only
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
uint32 mmr_stop_mode_trans_id_sim[MAX_NAS_STACKS] = {0,0};
#define mmr_stop_mode_trans_id mmr_stop_mode_trans_id_sim[mm_as_id]
#else
uint32 mmr_stop_mode_trans_id = 0;
#endif

#ifdef FEATURE_DUAL_SIM
extern dword  mm_gsm_sel_resel_block_timer_sim[MAX_AS_IDS];
#define mm_gsm_sel_resel_block_timer mm_gsm_sel_resel_block_timer_sim[mm_sub_id]

#else
extern uint8  mm_gsm_sel_resel_block_timer;
#endif

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
#define sys_eplmn_list_equivalent_plmn(a) sys_eplmn_per_nas_stacks_list_equivalent_plmn(a,(sys_modem_as_id_e_type)mm_sub_id,(sys_modem_as_id_e_type)mm_as_id)
#endif

#ifdef FEATURE_DUAL_SIM
#define sys_eplmn_per_nas_region_update(a) sys_eplmn_per_nas_region_update_per_stack(a,(sys_modem_as_id_e_type)mm_as_id)
#endif

#ifdef FEATURE_1XSRLTE
void emm_process_cm_dual_rx_tau_cmd(emm_ctrl_data_type   *emm_ctrl_data_ptr);
#endif


#ifdef FEATURE_DUAL_SIM
void mmcoord_process_mmr_multimode_subs_chgd_req( mm_cmd_type *message );
void mmcoord_process_mmr_dds_switch_ind( mm_cmd_type *message );

extern boolean mm_mn_sglte_svlte_mode;
#endif

#ifdef FEATURE_FEMTO_CSG
void mm_disable_csg_support(void);
#endif


extern sys_gw_conn_status_e_type mm_per_nas_stacks_get_connection_status
(
  sys_modem_as_id_e_type sub_id,
  sys_modem_as_id_e_type      stack_id_nas
);
extern sys_modem_stack_id_e_type mm_set_cm_stack_id(mm_as_id_e_type as_id);

#ifdef FEATURE_QSH_MDUMP
extern void emm_log_mdump_static_vars(); 
extern void emm_log_mdump_vars();
extern void gmm_log_mdump_static_vars();
extern void mm_log_mdump_static_vars();
#endif

#if(defined(TEST_FRAMEWORK) && defined(FEATURE_LTE))
 #error code not present
#endif // TEST_FRAMEWORK

/*#if(defined(TEST_FRAMEWORK) && defined(FEATURE_LTE))
 #include <tf_stub.h>

 TF_PROTO_STUB(rrc_cmd_type *,rrc_get_mm_cmd_buf, void );
 #define rrc_get_mm_cmd_buf TF_STUB(rrc_get_mm_cmd_buf)

 TF_PROTO_STUB(void, rrc_put_mm_cmd, rrc_cmd_type *);
 #define rrc_put_mm_cmd TF_STUB(rrc_put_mm_cmd)

#endif // TEST_FRAMEWORK
*/

#ifdef FEATURE_DUAL_SIM
#define mm_ghdi_nvmem_read(a,b) ghdi_nvmem_read_per_subs(a,b,mm_nv_context_id)
#else
#define mm_ghdi_nvmem_read(a,b) ghdi_nvmem_read(a,b)
#endif


/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
mm_debug_mmr_sim_available_status_e_type mm_debug_mmr_sim_available_status_sim[MAX_NAS_STACKS] = {MM_DEBUG_SIM_AVAILABLE_UNKNOWN, MM_DEBUG_SIM_AVAILABLE_UNKNOWN, MM_DEBUG_SIM_AVAILABLE_UNKNOWN};
#define mm_debug_mmr_sim_available_status mm_debug_mmr_sim_available_status_sim[mm_as_id]

boolean mm_stopped_due_to_mode_cap_change_sim[MAX_NAS_STACKS] = {FALSE, FALSE, FALSE};
#define mm_stopped_due_to_mode_cap_change mm_stopped_due_to_mode_cap_change_sim[mm_as_id]

#elif defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
mm_debug_mmr_sim_available_status_e_type mm_debug_mmr_sim_available_status_sim[MAX_NAS_STACKS] = {MM_DEBUG_SIM_AVAILABLE_UNKNOWN, MM_DEBUG_SIM_AVAILABLE_UNKNOWN};
#define mm_debug_mmr_sim_available_status mm_debug_mmr_sim_available_status_sim[mm_as_id]

boolean mm_stopped_due_to_mode_cap_change_sim[MAX_NAS_STACKS] = {FALSE, FALSE};
#define mm_stopped_due_to_mode_cap_change mm_stopped_due_to_mode_cap_change_sim[mm_as_id]

#else
mm_debug_mmr_sim_available_status_e_type mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_UNKNOWN;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
#ifdef FEATURE_DUAL_SIM
boolean mm_qsh_event_enabled_sim[MAX_AS_IDS][MM_QSH_EVENT_MAX];
#else
boolean mm_qsh_event_enabled[MM_QSH_EVENT_MAX];
#endif
#endif

#ifdef FEATURE_DUAL_SIM

extern uint8  mm_emm_max_attempt_counter_dos_mitigation_sim[MAX_AS_IDS];
#define mm_emm_max_attempt_counter_dos_mitigation mm_emm_max_attempt_counter_dos_mitigation_sim[mm_sub_id]

#else

extern uint8  mm_emm_max_attempt_counter_dos_mitigation;

#endif

#ifdef FEATURE_QSH_MDUMP
#if defined(FEATURE_DUAL_SIM) 
extern byte gmm_irat_lu_pending_sim[MAX_NAS_STACKS];
#define gmm_irat_lu_pending gmm_irat_lu_pending_sim[mm_as_id]

extern boolean nas_emm_tau_cs_rej_sim[MAX_AS_IDS];
#define nas_emm_tau_cs_rej nas_emm_tau_cs_rej_sim[mm_as_id]

extern lte_nas_emm_eps_update_val_type nas_emm_tau_cs_rej_tau_type_sim[MAX_AS_IDS];
#define nas_emm_tau_cs_rej_tau_type nas_emm_tau_cs_rej_tau_type_sim[mm_as_id]

extern lte_3gpp_release_ver_e emm_3gpp_spec_version_sim[MAX_AS_IDS];
#define emm_3gpp_spec_version emm_3gpp_spec_version_sim[mm_as_id]

extern boolean gmm_detach_started_sim[MAX_NAS_STACKS];
#define gmm_detach_started gmm_detach_started_sim[mm_as_id]

extern sys_csg_list_s_type         mm_sim_allowed_csg_list_sim[MAX_NAS_STACKS];
#define                            mm_sim_allowed_csg_list    mm_sim_allowed_csg_list_sim[mm_as_id]

extern mmgsdi_rec_num_type   mm_sim_allowed_csg_num_records_sim[MAX_NAS_STACKS];
#define                           mm_sim_allowed_csg_num_records  mm_sim_allowed_csg_num_records_sim[mm_as_id]

extern mmgsdi_len_type            mm_sim_allowed_csg_record_length_sim[MAX_NAS_STACKS];
#define                           mm_sim_allowed_csg_record_length  mm_sim_allowed_csg_record_length_sim[mm_as_id]

extern byte mm_stored_imeisv_sim[MAX_NAS_STACKS][MAX_IMEI_LEN+2];
#define mm_stored_imeisv (mm_stored_imeisv_sim[mm_as_id])


#else
extern byte gmm_irat_lu_pending;
extern boolean nas_emm_tau_cs_rej;
extern lte_nas_emm_eps_update_val_type nas_emm_tau_cs_rej_tau_type;
extern lte_3gpp_release_ver_e emm_3gpp_spec_version;
extern boolean gmm_detach_started;
extern sys_csg_list_s_type         mm_sim_allowed_csg_list;
extern mmgsdi_rec_num_type   mm_sim_allowed_csg_num_records;
extern mmgsdi_len_type            mm_sim_allowed_csg_record_length;
extern byte mm_stored_imeisv[MAX_IMEI_LEN+2];
#endif

extern byte nas_emm_index;  
extern byte total_num_messages;  
extern boolean detach_reason; 
extern boolean service_request; 
extern lte_nas_sec_info_params_type nas_sec_params; 
extern byte    nas_is_qmss_enabled; 
#endif

#ifdef FEATURE_SGLTE
extern boolean mm_is_cs_timer(timer_id_T timer_id);
extern boolean mm_is_non_sglte_plmn_in_sglte_home(sys_plmn_id_s_type pPlmnId);
extern boolean mm_process_sglte_plmn_state4_gsm_oos(sys_plmn_id_s_type  pPlmnId);
extern mm_sglte_stack_activity_status mm_sglte_current_stack_status ;
extern void mmcoord_process_spliting(void);
extern void  mmcoord_process_merging(void );
extern void mmcoord_process_pstimer_expiry_merging(void);
extern boolean mm_is_sglteplmn_in_sglte_roam(sys_plmn_id_s_type  pPlmnId);
extern boolean mm_process_if_both_stack_has_camped_non_sglte_plmn
                                                                (mm_as_id_e_type in_as_id);
extern boolean  auth_filter_domain_identity ( mm_cmd_type *msg_cmd,rrc_cn_domain_identity_e_type* domain_identity);
extern void mm_copy_cs_database(mm_as_id_e_type dest,mm_as_id_e_type src);
extern void mm_invalidate_cs_stack_database(void);
extern void mmsend_mmr_stack_deact_ind(void);
extern void mmsend_mmr_stack_act_ind(void);
extern void mmsend_mmr_service_stat_ind( mm_as_id_e_type mm_as_id_in);
extern boolean mm_sglte_state4_trigger_split(void);
extern void mm_reset_variables(mm_as_id_e_type src);
#endif
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE
extern void mmcoord_handle_ue_sglte_mode_switch(void);
#endif
#ifdef MMOC_API_PH_STAT_CNF_WITH_STATUS
extern void mmcoord_process_mmr_sim_busy_ind(message);
#endif
#ifdef FEATURE_DUAL_SIM
#ifndef FEATURE_LTE
sys_mode_pref_e_type mm_sim_mode_pref_sim[MAX_NAS_STACKS];
#define  mm_sim_mode_pref mm_sim_mode_pref_sim[mm_as_id]
static sys_mode_pref_e_type mm_sim_mode_pref_local_sim[MAX_NAS_STACKS];
#define  mm_sim_mode_pref_local mm_sim_mode_pref_local_sim[mm_as_id]
#endif
#endif

#if defined(FEATURE_1XSRLTE) || defined(FEATURE_SGLTE)||defined(FEATURE_SVLTE_DUAL_SIM)
extern void mm_send_rrc_ue_mode_change_ind( sys_radio_access_tech_e_type rat,sys_ue_mode_e_type   ue_mode );
extern void emm_send_ue_mode_change_ind(sys_ue_mode_e_type   ue_mode,boolean   ue_submode_is_srlte);
extern void mmcoord_process_mmr_ue_mode_change_ind( mm_cmd_type *message);
#endif

extern void  mmcoord_process_pseudo_list_update_ind(mm_cmd_type *message);

void mm_gmm_process_t3247_expiry(mm_cmd_type* message); 
void mm_process_gsm_block_timer_expiry(mm_cmd_type* message);
void mmcoord_process_rr_plmn_resel_fail_ind(mm_cmd_type* message);
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
#if defined(FEATURE_TRIPLE_SIM)
uint32 mm_subs_capability_mask_sim[MAX_AS_IDS];

#else
uint32 mm_subs_capability_mask_sim[MAX_AS_IDS];
#endif 
#define mm_subs_capability_mask mm_subs_capability_mask_sim[mm_sub_id]
#endif


#ifdef FEATURE_LTE
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
extern boolean mm_multimode_rat_change_pending_sim[MAX_NAS_STACKS];
#define mm_multimode_rat_change_pending mm_multimode_rat_change_pending_sim[mm_as_id]
#else
extern boolean mm_multimode_rat_change_pending;
#endif
#endif
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE

mm_cmd_type          mmcoord_pending_reg_message_sim[MAX_NAS_STACKS];
mm_cmd_type          mmcoord_pending_reg_list_req_message_sim[MAX_NAS_STACKS];
mmr_reg_cnf_s_type   mmcoord_mmr_reg_cnf_sim[MAX_NAS_STACKS];
boolean              mmcoord_reg_req_pending_sim[MAX_NAS_STACKS];
boolean              mm_waiting_for_abort_cnf_sim[MAX_NAS_STACKS];
boolean              mm_stop_mode_req_pended_sim[MAX_NAS_STACKS];
boolean              mm_reg_req_revd_active_call_sim[MAX_NAS_STACKS];

extern sys_radio_access_tech_e_type     mm_service_loss_on_rat_sim[MAX_NAS_STACKS];
#define  mm_service_loss_on_rat mm_service_loss_on_rat_sim[mm_as_id]

extern byte mm_last_paging_cause_sim[MAX_NAS_STACKS];
boolean  mm_cell_changed_to_cs_capable_sim[MAX_NAS_STACKS];


#define  mmcoord_pending_reg_message mmcoord_pending_reg_message_sim[mm_as_id]
#define  mmcoord_pending_reg_list_req_message mmcoord_pending_reg_list_req_message_sim[mm_as_id]
#define  mmcoord_mmr_reg_cnf mmcoord_mmr_reg_cnf_sim[mm_as_id]
#define  mmcoord_reg_req_pending mmcoord_reg_req_pending_sim[mm_as_id]

#define  mm_waiting_for_abort_cnf mm_waiting_for_abort_cnf_sim[mm_as_id]
#define  mm_stop_mode_req_pended mm_stop_mode_req_pended_sim[mm_as_id] 
#define  mm_reg_req_revd_active_call mm_reg_req_revd_active_call_sim[mm_as_id]

#define  mm_last_paging_cause mm_last_paging_cause_sim[mm_as_id]
#define  mm_cell_changed_to_cs_capable (mm_cell_changed_to_cs_capable_sim[mm_as_id])


boolean              mm_dual_standby_mode;
boolean              mm_sglte_mode ;
extern boolean gmm_force_local_detach_sim[MAX_NAS_STACKS];

boolean mmcoord_svc_cnf_pending_sim[MAX_NAS_STACKS];
#define mmcoord_svc_cnf_pending mmcoord_svc_cnf_pending_sim[mm_as_id]

mm_cmd_type mmcoord_pending_svc_cnf_message_sim[MAX_NAS_STACKS];
#define mmcoord_pending_svc_cnf_message mmcoord_pending_svc_cnf_message_sim[mm_as_id]

boolean              mm_lu_pended_nmo_unknown_sim[MAX_NAS_STACKS];
#define mm_lu_pended_nmo_unknown mm_lu_pended_nmo_unknown_sim[mm_as_id]

extern  auth_gsm_cipherkey_type     gsm_kc_128bit_sim[MAX_NAS_STACKS] ;
extern  auth_gsm_cipherkey_type     gsm_gprs_kc_128bit_sim[MAX_NAS_STACKS] ;
extern boolean mm_stop_mode_local_detach_peding_sim[MAX_NAS_STACKS];
#define mm_stop_mode_local_detach_peding mm_stop_mode_local_detach_peding_sim[mm_as_id]

#if defined (FEATURE_WRLF_SYSTEM_SEL) || defined (FEATURE_LTE)
#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
mm_rlf_stop_mode_pending_enum_T mm_rlf_stop_mode_pending_sim[MAX_NAS_STACKS] = 
                                                        {MM_RLF_STOP_MODE_PENDING_NONE,MM_RLF_STOP_MODE_PENDING_NONE,MM_RLF_STOP_MODE_PENDING_NONE};
#define mm_rlf_stop_mode_pending mm_rlf_stop_mode_pending_sim[mm_as_id]
#else
mm_rlf_stop_mode_pending_enum_T mm_rlf_stop_mode_pending_sim[MAX_NAS_STACKS] = {MM_RLF_STOP_MODE_PENDING_NONE,MM_RLF_STOP_MODE_PENDING_NONE};
#define mm_rlf_stop_mode_pending mm_rlf_stop_mode_pending_sim[mm_as_id]
#endif
#endif

#if defined(FEATURE_TRIPLE_SIM) || defined(FEATURE_SGLTE_DUAL_SIM)
static boolean   mm_dual_switch_timer_started_sim[MAX_NAS_STACKS] = {FALSE,FALSE,FALSE};
#define  mm_dual_switch_timer_started  (mm_dual_switch_timer_started_sim[mm_as_id])
#else
static boolean   mm_dual_switch_timer_started_sim[MAX_NAS_STACKS] = {FALSE,FALSE};
#define  mm_dual_switch_timer_started  (mm_dual_switch_timer_started_sim[mm_as_id])
#endif

#ifdef FEATURE_NAS_ECALL
boolean mm_ecall_profile_sim[MAX_NAS_STACKS];
#define mm_ecall_profile mm_ecall_profile_sim[mm_as_id]
#endif
#else

mm_cmd_type          mmcoord_pending_reg_message;
mm_cmd_type          mmcoord_pending_reg_list_req_message;
mmr_reg_cnf_s_type   mmcoord_mmr_reg_cnf;
boolean              mmcoord_reg_req_pending;
boolean              mm_waiting_for_abort_cnf;
boolean              mm_stop_mode_req_pended;
boolean              mm_reg_req_revd_active_call;
static boolean       mm_dual_switch_timer_started = FALSE;

#if defined (FEATURE_WRLF_SYSTEM_SEL) || defined (FEATURE_LTE)
mm_rlf_stop_mode_pending_enum_T mm_rlf_stop_mode_pending = MM_RLF_STOP_MODE_PENDING_NONE;
#endif

boolean mmcoord_svc_cnf_pending;

mm_cmd_type mmcoord_pending_svc_cnf_message;

boolean              mm_lu_pended_nmo_unknown;
boolean              mm_cell_changed_to_cs_capable;
extern  auth_gsm_cipherkey_type     gsm_kc_bit128 ;
extern  auth_gsm_cipherkey_type     gsm_gprs_kc_bit128 ;

extern sys_radio_access_tech_e_type mm_service_loss_on_rat;
extern byte mm_last_paging_cause;
extern boolean        mm_stop_mode_local_detach_peding;

#ifdef FEATURE_NAS_ECALL
boolean mm_ecall_profile = FALSE;
#endif

#endif /* FEATURE_DUAL_SIM || FEATURE_SGLTE*/

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
#define sys_eplmn_list_validate(a) sys_eplmn_per_nas_stacks_list_validate(a,(sys_modem_as_id_e_type)mm_as_id)

#endif

boolean              mm_reg_ind_sent;

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
boolean mm_need_to_process_sim_avail_req_sim[MAX_NAS_STACKS];
#define mm_need_to_process_sim_avail_req mm_need_to_process_sim_avail_req_sim[mm_as_id]

boolean mm_waiting_for_open_session_cnf_sim[MAX_NAS_STACKS];
#define mm_waiting_for_open_session_cnf mm_waiting_for_open_session_cnf_sim[mm_as_id]

extern boolean sys_eplmn_per_nas_stacks_list_equivalent_plmn
(
  sys_plmn_id_s_type plmn,
  sys_modem_as_id_e_type      sub_id,
  sys_modem_as_id_e_type      stack_id_nas
);
#define sys_eplmn_list_equivalent_plmn(a) sys_eplmn_per_nas_stacks_list_equivalent_plmn(a,(sys_modem_as_id_e_type)mm_sub_id,(sys_modem_as_id_e_type)mm_as_id)
#else
boolean mm_need_to_process_sim_avail_req;
boolean mm_waiting_for_open_session_cnf;
extern boolean sys_eplmn_list_equivalent_plmn
(
  sys_plmn_id_s_type plmn
);
#endif


#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_ENHANCED_NW_SELECTION
#define reg_sim_read_ens_flag() reg_sim_per_nas_stacks_read_ens_flag((sys_modem_as_id_e_type)mm_sub_id)
#endif
#endif


#ifdef FEATURE_MBMS
#error code not present
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
wcdma_rlf_state_enum_T mm_wcdma_rlf_state_sim[MAX_NAS_STACKS];
boolean mm_wrlf_rat_change_pending_sim[MAX_NAS_STACKS];
#else
wcdma_rlf_state_enum_T mm_wcdma_rlf_state;
boolean mm_wrlf_rat_change_pending;
#endif

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
mm_cmd_type *mm_wrlf_pending_rr_message_ptr_sim[MAX_NAS_STACKS];
#define mm_wrlf_pending_rr_message_ptr mm_wrlf_pending_rr_message_ptr_sim[mm_as_id]
mm_cmd_type *mm_wrlf_pending_rrc_message_ptr_sim[MAX_NAS_STACKS];
#define mm_wrlf_pending_rrc_message_ptr mm_wrlf_pending_rrc_message_ptr_sim[mm_as_id]

#else
mm_cmd_type *mm_wrlf_pending_rr_message_ptr;
mm_cmd_type *mm_wrlf_pending_rrc_message_ptr;
#endif

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
serving_plmn_info_T *mm_wrlf_serving_plmn_ptr_sim[MAX_NAS_STACKS];
#define mm_wrlf_serving_plmn_ptr mm_wrlf_serving_plmn_ptr_sim[mm_as_id]
#else
serving_plmn_info_T *mm_wrlf_serving_plmn_ptr;
#endif
void mmcoord_process_rrc_service_cnf( mm_cmd_type *message );
#endif

#ifdef FEATURE_SGLTE
#ifdef FEATURE_SGLTE_DUAL_SIM
boolean mm_no_service_ind_during_split_sim[MAX_AS_IDS];
#define mm_no_service_ind_during_split mm_no_service_ind_during_split_sim[mm_sub_id]
#else
boolean mm_no_service_ind_during_split;
#endif
#endif


#ifdef FEATURE_DUAL_SIM


mm_cmd_type *mm_pending_mmr_ps_detach_req_ptr_sim[MAX_NAS_STACKS] = {NULL, NULL};
#define mm_pending_mmr_ps_detach_req_ptr mm_pending_mmr_ps_detach_req_ptr_sim[mm_as_id]


#else


mm_cmd_type *mm_pending_mmr_ps_detach_req_ptr = NULL;


#endif

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
mm_ps_detach_state_enum_T  mm_ps_detach_state_sim[MAX_NAS_STACKS];
#define mm_ps_detach_state mm_ps_detach_state_sim[mm_as_id]
#else
mm_ps_detach_state_enum_T  mm_ps_detach_state = MM_PS_DETACH_STATE_NONE;
#endif

#ifdef FEATURE_CMCC_SECURE_CALL
#if defined FEATURE_TRIPLE_SIM
sys_secure_call_e_type mm_emm_secure_status_type_sim[MAX_NAS_STACKS] ={SYS_SECURE_CALL_NONE,SYS_SECURE_CALL_NONE,SYS_SECURE_CALL_NONE};
#define mm_emm_secure_status_type mm_emm_secure_status_type_sim[mm_as_id]
#elif defined FEATURE_DUAL_SIM
sys_secure_call_e_type mm_emm_secure_status_type_sim[MAX_NAS_STACKS] ={SYS_SECURE_CALL_NONE,SYS_SECURE_CALL_NONE};
#define mm_emm_secure_status_type mm_emm_secure_status_type_sim[mm_as_id]
#else
sys_secure_call_e_type  mm_emm_secure_status_type = SYS_SECURE_CALL_NONE;
#endif
#endif

extern void gmm_handle_gprs_detach_low_level_failure( void );

extern boolean reg_nv_is_imsi_switch_enabled(void);

extern boolean reg_nv_is_wcdma_freq_lock_enabled(void);
extern void mm_log_mm_msg_info(byte pd,byte msg_id);

void gmm_handle_local_gprs_detach( void );
#ifdef FEATURE_LTE
extern boolean reg_mode_check_eplmn_backoff_forbidden
(
  reg_sim_plmn_timer_info_s_type *plmn_timer_info,
  sys_modem_as_id_e_type as_id
);

#endif

#ifdef FEATURE_SGLTE
extern void mmcoord_process_stop_mode_on_hybrid_stack(void);
#endif

#if defined FEATURE_DUAL_SIM 
extern uint8   mm_emm_usim_gprs_counter_sim[MAX_NAS_STACKS];
#define mm_emm_usim_gprs_counter mm_emm_usim_gprs_counter_sim[mm_as_id]

extern uint8   mm_emm_usim_non_gprs_counter_sim[MAX_NAS_STACKS];
#define mm_emm_usim_non_gprs_counter mm_emm_usim_non_gprs_counter_sim[mm_as_id]

#else
extern uint8   mm_emm_usim_gprs_counter;
extern uint8   mm_emm_usim_non_gprs_counter;
#endif

#ifdef FEATURE_LTE
void mmcoord_process_non_acq_param_change_ind(mm_cmd_type *cmd_ptr);
boolean mmcoord_stop_gw(mm_cmd_type *message)
{
   boolean wait_for_ll = TRUE;

   /* Revisit during Connected Mode RLF PLMN Selection                           *
    * We will have two variables - connected_mode_RAT and idle_mode_active_RAT   *
    * Use judiciously                                                            */
   
   mmcoord_reg_req_pending = FALSE;
   mm_rat_change_pending = FALSE;
   mm_reg_waiting_for_reg_cnf = FALSE;
   mm_waiting_for_service_cnf = FALSE;
   mm_wrlf_rat_change_pending = FALSE;

#ifdef FEATURE_NAS_FREE_FLOAT
	if(is_gsm_mode() && mm_timer_status[TIMER_NW_INIT_DETACH] == TIMER_ACTIVE 
#ifdef FEATURE_GPRS_LAYER3_TX_CONFIRM             
					&& gmm_last_signalling_message_type == GMM_DETACH_ACCEPT)
#endif                           
    {
		 
		if(mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF)
		{
			  mm_send_mmr_ps_detach_cnf();
			  mm_ps_detach_state = MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ;
		}
		if(gmm_gprs_attach_pending)
		{
			  gmm_set_substate(GMM_ATTACH_NEEDED);
		}
		gmm_detach_in_progress = FALSE;
		gmmllc_gmm_trigger_llc_event(GMM_TRIGGER_GMM_DETACHED, FALSE, 0 );
		gmmllc_gmm_trigger_llc_event(GMM_TRIGGER_PTMSI_ASSIGNED,FALSE,0) ;
		gmmllc_gmm_trigger_llc_event(GMM_TRIGGER_TLLI_UNASSIGNMENT,FALSE,0);
		mm_stop_timer(TIMER_NW_INIT_DETACH);
	}
#endif

   if(mm_last_sent_serv_req_params_ptr != NULL)
   {
     modem_mem_free(mm_last_sent_serv_req_params_ptr,MODEM_MEM_CLIENT_NAS);
     mm_last_sent_serv_req_params_ptr = NULL;
   }

   if ((mm_serving_plmn.info.active_rat == SYS_RAT_UMTS_RADIO_ACCESS) ||
       (mm_serving_plmn.info.active_rat == SYS_RAT_TDS_RADIO_ACCESS))
   {
      mm_send_rrc_stop_mode_req( RRC_USER_RAT_CHANGE,
                                 mm_serving_plmn.info.active_rat );
      mm_multimode_rat_change_pending = TRUE;
   }
   else if (mm_serving_plmn.info.active_rat == SYS_RAT_GSM_RADIO_ACCESS)
   {
#ifdef FEATURE_SGLTE
      if (MM_IS_IN_SGLTE_MODE && (mm_sglte_current_stack_status  == SGLTE_MM_UPPER_LAYER_SPLITING))
       {
         /* changing stop mode reason to RR_SGLTE_USER_RAT_CHANGE so that RR 
                stop any ongoing interrat activity similar to user rat change*/
         mm_send_rr_stop_mode_req(RR_SGLTE_USER_RAT_CHANGE);
       }
      else
#endif
       {
         mm_send_rr_stop_mode_req(RR_MM_USER_RAT_CHANGE);
       }
      mm_multimode_rat_change_pending = TRUE;
   }   
   else
   {
      wait_for_ll = FALSE;     
   }
   return wait_for_ll;   
}

void mmcoord_locally_detach_gw(void)
{
    mm_state_control(MM_STD_LOCALLY_DEACTIVATE_MM);
    gmm_perform_local_detach(); 
}


/*===========================================================================

FUNCTION mmcoord_locally_deactivate_isr_if_needed

DESCRIPTION  This function check if the ISR is deactivated when 
              UE has PDP activated in LTE and ISR is activated.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_locally_deactivate_isr_if_needed(void)
{
#if defined(FEATURE_LTE)
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);
#endif
  if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS)  == TRUE)
  {

#ifdef FEATURE_SGLTE
  if ( (MM_IS_IN_SGLTE_MODE) && (mm_as_id != mm_ps_stack_id))
  {
    return ;
  }
#endif

#if defined(FEATURE_LTE)
        /* If UE has PDP activated in LTE and ISR is activated, set TIN to GUTI*/
  if((emm_ctrl_data_ptr->TIN == NAS_MM_TIN_RAT_RELATED_TMSI) &&
     (sm_check_isr_deact_status_on_interrat()))
   {
     MSG_HIGH_DS_0(MM_SUB,"=MM= Deactivate ISR due to PDP context update while in LTE");
     emm_set_and_write_tin_type(NAS_MM_TIN_GUTI);
     mm_pending_lu_request=LTE_IRAT_LU;
   }       
#endif
  }

}
/*===========================================================================

FUNCTION MMCOORD_ABORT_RR_CONNECTION

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_abort_rr_connection(mmr_reg_req_e_type type)
{

#ifdef FEATURE_WCDMA	
  if (is_umts_mode())
  {
    rrc_cmd_type *rrc_cmd_ptr = mm_get_rrc_cmd_buf( mm_serving_plmn.info.active_rat );
    if (rrc_cmd_ptr == NULL)
    {
      MSG_FATAL_DS(MM_SUB, "ERROR: Unable to allocate memory", 0,0,0 );
    }

    rrc_cmd_ptr->cmd_hdr.cmd_id = RRC_ABORT_REQ;
    if (((type == MMR_REG_REQ_SYSTEM_CHANGE) || 
        ((mm_timer_status[TIMER_T3340] == TIMER_STOPPED) && 
         (mm_timer_status[TIMER_T3240] == TIMER_STOPPED) && 
         (mm_timer_status[TIMER_DETACH_REL] == TIMER_STOPPED))) && 
       (gmm_anite_gcf_flag == FALSE) 
#ifdef FEATURE_ENHANCED_NW_SELECTION
       &&
       (reg_sim_read_ens_flag() == FALSE)
#endif
       )
    {
      rrc_cmd_ptr->cmd.rrc_abort_req.delay_sig_con_release = FALSE;
    }
    else
    {
      rrc_cmd_ptr->cmd.rrc_abort_req.delay_sig_con_release = TRUE;
    }
    rrc_cmd_ptr->cmd.rrc_abort_req.auth_failed_bar_cells = FALSE;
    rrc_cmd_ptr->cmd.rrc_abort_req.hard_abort = FALSE;

    rrc_cmd_ptr->cmd.rrc_abort_req.abort_cause = RRC_OTHERS;
    send_cmd_to_rrc( rrc_cmd_ptr, mm_serving_plmn.info.active_rat );
  }
  else 
#endif	  
  if (is_gsm_mode())
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM sent RR_ABORT_REQ");
    mm_send_rr_abort_req(FALSE);
  }
}

#endif


#ifdef FEATURE_DUAL_SIM
/*************************************************************
FUNCTION mm_abort_connection_due_to_mplmn_on_other_subs

DESCRIPTION
  This function checks if RAT other than current RAT has connection setup for PS and abort it
  for MPLMN srch is triggered on current RAT

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
 *************************************************************/
void  mm_abort_connection_due_to_mplmn_on_other_subs( void )
{
  mm_as_id_e_type      mm_other_as_id, mm_tmp_as_id;
  mm_tmp_as_id = mm_as_id;

  if (MMCNM_IS_MODE_DUAL_ACTIVE && (mm_mn_sglte_svlte_mode == FALSE))
  {
      return;
  }
  for( mm_other_as_id = MM_AS_ID_1; mm_other_as_id < (mm_as_id_e_type)MAX_NAS_STACKS ; mm_other_as_id++ )
  {
    if( mm_other_as_id != mm_tmp_as_id )
    {
      mm_set_as_id(mm_other_as_id);
      if((is_gsm_mode() == TRUE)&& 
         ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) ||
         (mm_serving_plmn.ms_op_mode ==  SYS_SRV_DOMAIN_PS_ONLY)))
      {
        mm_send_rr_ps_abort_req(MM_RR_PS_ABORT_MANUAL_PLMN_SEARCH);
      }
#ifdef FEATURE_LTE
      if(is_lte_mode())
      {
        emm_ctrl_data_ptr = emm_db_get_ctrl_data();
        if(((EMM_GET_CONNECTION_STATE() == EMM_WAITING_FOR_RRC_CONFIRMATION_STATE ) ||
          ( EMM_GET_CONNECTION_STATE() == EMM_CONNECTED_STATE ))&&
          ( emm_ctrl_data_ptr->emm_sms_active == FALSE )&&
          ( emm_mmtel_status( emm_ctrl_data_ptr ) == FALSE )&&
          ( emm_wms_sms_status( emm_ctrl_data_ptr ) == FALSE )&&
          ((( EMM_GET_STATE() == EMM_SERVICE_REQUEST_INITIATED )&&
          ( emm_ctrl_data_ptr->call_type !=  SYS_LTE_REESTAB_CALLTYPE_VOLTE)&&
          ( emm_ctrl_data_ptr->esr_type_in_progress == NAS_EMM_CSFB_NONE)&&
          ( emm_ctrl_data_ptr->emc_srv_status == LTE_NAS_EMC_SRV_NONE))||
          ((EMM_GET_STATE() == EMM_REGISTERED)&&
           (emm_ctrl_data_ptr->reg_complete_ack_pending == EMM_PENDING_REG_ACK_NONE)
           &&(
           mm_timer_status[EMM_MT_PAGE_EXT_PRI_TIMER] != TIMER_ACTIVE))||
          (EMM_GET_STATE() == EMM_DEREGISTERED)))
        {
#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
          if ((emm_check_if_els_conn_abort_possible(emm_ctrl_data_ptr) == TRUE) &&
              (mm_timer_status[TIMER_ELS_CONN_REL] != TIMER_ACTIVE))
          {
            /* Send ELS connection abort to RRC */
            emm_send_rrc_els_conn_abort_req(emm_ctrl_data_ptr, LTE_RRC_ELS_CONN_ABORT_CAUSE_MME_NOT_SUSPEND);
            mm_start_timer(TIMER_ELS_CONN_REL, DEFAULT_TIMEOUT);
            emm_ctrl_data_ptr->emm_els_conn_abort_reason = ELS_ABORT_ON_MPLMN_SRCH;
          }
          else
#endif
          {
             MSG_HIGH_DS_0(MM_SUB,"=EMM= Abort RRC connection for manual plmn serach request");
             emm_rrc_abort_connection(emm_ctrl_data_ptr, FALSE,LTE_RRC_CONN_ABORT_CAUSE_NORMAL) ;
          }
          if(emm_ctrl_data_ptr->force_tau_during_manual_search == TRUE)
          {
            MSG_HIGH_DS_0(MM_SUB,"=EMM= Set TAU Pending after abort ");
            emm_ctrl_data_ptr->tau_pending_after_msearch_abort = TRUE;
          }
        }
      }
      else
#endif
      if((is_umts_mode() == TRUE) &&
         ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) ||
          (mm_serving_plmn.ms_op_mode ==  SYS_SRV_DOMAIN_PS_ONLY)) &&
         (rrc_connection_status != MMRRC_IDLE ) &&
         (mm_sms_is_active == FALSE)&&
         (gmm_state != GMM_REGISTERED_INITIATED)&&
         (gmm_state != GMM_ROUTING_AREA_UPDATING_INITIATED)&&
         (gmm_state != GMM_DEREGISTERED_INITIATED)&&
         (cs_session_status == MMRRC_IDLE)&&
         (mm_is_mt_page_in_progress() == FALSE)&&
         (mm_state == MM_IDLE))
      {
        MSG_HIGH_DS_0(MM_SUB,"=MM= Received Manual PLMN search req,aborting RRC established for MO data ");
        mm_abort_rr(RRC_PS_DOMAIN_CN_ID, FALSE);
      }
    }
  }
  mm_set_as_id(mm_tmp_as_id);
#ifdef FEATURE_LTE
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
#endif
}
#endif

/*===========================================================================

FUNCTION mmcoord_policy_change_pending

DESCRIPTION  This function evaluates if policy change is pending or PM needs 
              to be suspended
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean mmcoord_policy_change_pending(void)
{
  boolean policy_change_pending = FALSE;
  sys_non_acq_param_s_type  non_acq_param;
  cm_effective_policy_change_reason_e_type policy_change_reason;

  policy_change_reason = cm_effective_policy_changed((sys_modem_as_id_e_type)mm_as_id,mm_serving_plmn.info.active_rat,
                                                      &mm_serving_plmn.info.plmn,
                                                      &non_acq_param);
#ifdef FEATURE_LTE
  MSG_HIGH_DS_4(MM_SUB, "=MM= policy_change_reason %d new voice_domain_pref %d prev voice_domain_pref %d mm_state %d",
                           policy_change_reason, non_acq_param.voice_domain_pref,
                           gmm_reg_pended_lte_cs_param.voice_domain_pref, mm_state);
#endif
  if(policy_change_reason == CM_EFFECTIVE_NON_ACQ_POLICY_CHANGE)
  {
#ifdef FEATURE_LTE
    if(emm_bit_mask_check(non_acq_param.nonAcqValue,
                            (byte)SYS_NONACQ_INFO_VOICE_DOMAIN_PREF) == TRUE)
    {
      gmm_reg_pended_lte_cs_param.is_voice_domain_ie_valid = TRUE;
      gmm_reg_pended_lte_cs_param.voice_domain_pref = non_acq_param.voice_domain_pref;
      MSG_HIGH_DS_2(MM_SUB, "=MM= Voice domain pref change detected voice_domain_pref %d ie_valid %d",
        gmm_reg_pended_lte_cs_param.voice_domain_pref,
        gmm_reg_pended_lte_cs_param.is_voice_domain_ie_valid);
    }
#endif
    policy_change_pending = FALSE;
  }
  else if (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                                  SYS_NETWORK_SELECTION_MODE_LIMITED)
  {
    policy_change_pending = FALSE; 
  }
  else if(mm_timer_status[TIMER_POLICY_CHANGE_PENDING] == TIMER_ACTIVE)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= Policy Change Timer running");
    policy_change_pending = TRUE; 
  }
   /*Dont check for policy change in manual/limited manual if current plmn is not en eplmn of user selected plmn*/
  else if(((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                                  SYS_NETWORK_SELECTION_MODE_MANUAL) || 
           (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                                  SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION)) &&
            !((sys_plmn_match(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn, 
                      mm_serving_plmn.info.plmn) == TRUE) ||
             (sys_eplmn_list_equivalent_plmn(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) && 
               sys_eplmn_list_equivalent_plmn(mm_serving_plmn.info.plmn))))
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= Policy change ignored in manual/limited manual if current plmn is not en eplmn of user selected plmn");
    policy_change_pending = FALSE;
  }
  else if((policy_change_reason == CM_EFFECTIVE_POLICY_CHANGE) &&
          (mm_state == MM_IDLE) 
#ifdef FEATURE_LTE
       && (mm_csfb_state != MM_CSFB_STATE_MO_CALL_ORIG)
#endif
          )
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= Policy change pending");
    policy_change_pending = TRUE;
  }

  return policy_change_pending;
}


/*===========================================================================

FUNCTION  mmcoord_capability_chg_pend_limited_service

DESCRIPTION  Sets MM/GMM to limited service if capability change is pending after receiving 
             service indicaiton/confirmation.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_capability_chg_pend_limited_service
(
  boolean policy_change_pending, boolean plmn_selection_started
)
{

  mmr_cause_e_type cause = MMR_CAUSE_NOT_SPECIFIED;

#ifdef FEATURE_SGLTE
  MSG_HIGH_DS_0(MM_SUB, "=MM= Sent Limited to REG as both stack camp on Non SGLTE PLMN, REG will send Limited Regional to CM");
#else
  MSG_HIGH_DS_2 (MM_SUB, "=MM= Sent Limited to REG as Policy change is pending policy_change_pending %d plmn_selection_started %d ",policy_change_pending,plmn_selection_started);
#endif 

  if(policy_change_pending == TRUE)
  {
    mm_to_only_reg = TRUE;
    cause = MMR_CAUSE_FAILURE_REMAIN_ON_PLMN;
    if(mm_timer_status[TIMER_POLICY_CHANGE_PENDING] != TIMER_ACTIVE)
    {
      mm_send_mmr_camped_ind((boolean)!plmn_selection_started || reg_mode_get_higher_priority_plmn_selection_started((sys_modem_as_id_e_type)mm_sub_id));
    }
    mm_start_timer(TIMER_POLICY_CHANGE_PENDING, DEFAULT_TIMEOUT);
  }
  mm_substate_control( MM_STD_LIMITED_SERVICE );

    /* Set the Serving PLMN service status to Limited */
  mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                     mm_serving_plmn.info.reg_service_domain,
                                     TRUE,
                                     SYS_SRV_STATUS_LIMITED,
                                     TRUE );            

  if (mm_reg_waiting_for_reg_cnf)
  {
   /* Set the REG CNF plmn service state to the Serving PLMN info */
    mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
    mmcoord_mmr_reg_cnf.cause = cause;
    mmcoord_mmr_reg_cnf.service_state.service_status = SYS_SRV_STATUS_LIMITED;  
    mm_send_mmr_reg_cnf();
    mm_reg_waiting_for_reg_cnf = FALSE;
    mm_ready_to_send_reg_cnf   = FALSE;
  }
  else 
  {
    mmr_service_ind_s_type  mmr_service_ind_temp;
    memset(&mmr_service_ind_temp,0x00,sizeof(mmr_service_ind_s_type));
    mmr_service_ind_temp.service_state = mm_serving_plmn.info;
    mmr_service_ind_temp.cause = cause;
    mmr_service_ind_temp.service_state.service_status = SYS_SRV_STATUS_LIMITED;              
    mm_send_mmr_service_ind(&mmr_service_ind_temp);
  }
  gmm_set_substate( GMM_LIMITED_SERVICE );
  mm_to_only_reg = FALSE;
}

#ifdef FEATURE_GSM

/*===========================================================================

FUNCTION 

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mm_send_rr_sim_inserted_req( void )
{
     rr_sim_inserted_req_T   rr_sim_inserted_req;
    /*
    * tell RR that the sim is inserted
    */
    memset(&rr_sim_inserted_req,0x00,sizeof(rr_sim_inserted_req_T));
#if defined(FEATURE_DUAL_SIM) && !defined(FEATURE_TWO_3GPP_SIM)
    if (mm_is_gsm_allowed_on_sub())
#endif
    {
    MSG_HIGH_DS_0(MM_SUB, "=MM= Sending RR_SIM_INSERTED_REQ to RR");

    rr_sim_inserted_req.message_header.message_set = MS_MM_RR;
    rr_sim_inserted_req.message_header.message_id = (int) RR_SIM_INSERTED_REQ;

    /* Store the Access class information */

    memscpy(rr_sim_inserted_req.access_control, MM_SIM_EFACC_SIZE, mm_sim_access_class, MM_SIM_EFACC_SIZE);
    
    /* Store the IMSI value */
    memscpy(rr_sim_inserted_req.imsi, MM_SIM_IMSI_SIZE, &mm_stored_imsi, MM_SIM_IMSI_SIZE);

 
    rr_sim_inserted_req.admin_data = mm_ms_opmode;
#ifdef FEATURE_GAN
#error code not present
#endif 
    /* Store the ciphering Key for GSM */
    memscpy((void*)rr_sim_inserted_req.ciphering_key, MM_SIM_EFKC_SIZE, (void*)auth_gsm_cipherkey.value_data,
             auth_gsm_cipherkey.value_length);
    rr_sim_inserted_req.ciphering_key_seq_no = auth_gsm_cksn;

    /* Store the GPRS cipher key & GPRS cipher key sequence number */
    memscpy((void*)rr_sim_inserted_req.gprs_ciphering_key, MM_SIM_EFKC_GPRS_SIZE, (void*)auth_ps_gprs_cipherkey.value_data,
             MM_SIM_EFKC_GPRS_SIZE);
    rr_sim_inserted_req.gprs_ciphering_key_seq_no = auth_ps_gprs_cksn;

    /* Set the BCCH list as not supported, as it is not used by RR */
    rr_sim_inserted_req.bcch_allocation_valid = FALSE;
    (void)memset (rr_sim_inserted_req.bcch_allocation, 0x00, MM_SIM_EFBCCH_SIZE);

    /* Store the REG PLMN, only if the UE has Service */
    if (mm_serving_plmn.info.service_status == SYS_SRV_STATUS_SRV)
    {
      memscpy(&rr_sim_inserted_req.reg_plmn, sizeof(sys_plmn_id_s_type), &mm_serving_plmn.info.plmn, PLMN_SIZE);
    }

    PUT_IMH_LEN( sizeof( rr_sim_inserted_req_T ) - sizeof(IMH_T),
                 &rr_sim_inserted_req.message_header );

    /* Set the TMSI */
    memscpy(rr_sim_inserted_req.tmsi, MM_SIM_TMSI_SIZE,mm_location_information.tmsi,TMSI_SIZE);


    /* Set the PTMSI */
    memscpy(rr_sim_inserted_req.ptmsi, MM_SIM_PTMSI_SIZE,gmm_stored_ptmsi.digit,PTMSI_SIZE);
    
    /* set the GSM 128 bit generated KC */ 
    memscpy(rr_sim_inserted_req.gsm_kc_128bit, sizeof(mm_gsm_kc_128bit_T),
                                     (void*)gsm_kc_bit128.value_data,MM_GSM_128BIT_KC_SIZE) ;
       /* set the GSM gprs 128 bit generated KC */ 
    memscpy(rr_sim_inserted_req.gsm_gprs_kc_128bit, sizeof(mm_gsm_kc_128bit_T),
                                     (void*)gsm_gprs_kc_bit128.value_data,MM_GSM_128BIT_KC_SIZE) ;

#ifdef FEATURE_GAN
    #error code not present
#endif /* FEATURE_GAN */

#if defined(FEATURE_DUAL_SIM) && !defined(FEATURE_LTE)
    rr_sim_inserted_req.dual_mode = 
            ((mm_sim_mode_pref == SYS_MODE_PREF_GSM_RADIO_ACCESS_PREFERRED) ||
             (mm_sim_mode_pref == SYS_MODE_PREF_UMTS_RADIO_ACCESS_PREFERRED));
    rr_sim_inserted_req.wcdma_allowed = mm_is_umts_allowed_on_sub();
#endif


    mm_send_message((IMH_T *)&rr_sim_inserted_req,GS_QUEUE_RR );
  }
}
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL

/*===========================================================================

FUNCTION MMCOORD_SET_WRLF_STATE

DESCRIPTION
  Sets the wrlf state global variable and allocates or frees the memory pointed by
  mm_wrlf_serving_plmn_ptr according to the state.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_set_wrlf_state(wcdma_rlf_state_enum_T rlf_state)
{
  /*
  ** Whenever WRLF ends inform CM about it by sending
  ** CM_DISABLE_BPLMN_IND with status as FALSE.
  */
  if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE &&
      rlf_state == WCDMA_RLF_IDLE)
  {
    mm_send_rrc_disable_bplmn_ind(FALSE);
  }
  mm_wcdma_rlf_state = rlf_state;
  MSG_HIGH_DS_1(MM_SUB ,"=MM= WCDMA RLF State set to %d",rlf_state);

  if(rlf_state == WCDMA_RLF_STARTED)
  {
    if(( mm_wrlf_serving_plmn_ptr == NULL ) && 
     ((mm_wrlf_serving_plmn_ptr=(serving_plmn_info_T *)modem_mem_calloc(1,sizeof( serving_plmn_info_T ),MODEM_MEM_CLIENT_NAS)) 
            == NULL))
    {
      mm_check_for_null_ptr((void*)mm_wrlf_serving_plmn_ptr);
    }
  }
  if(rlf_state == WCDMA_RLF_IDLE)
  {
    if(mm_wrlf_serving_plmn_ptr != NULL)
    {
      modem_mem_free(mm_wrlf_serving_plmn_ptr,MODEM_MEM_CLIENT_NAS);
      mm_wrlf_serving_plmn_ptr = NULL;
    }
  }

}
/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMR_PLMN_SEARCH_REQ

DESCRIPTION
  While in WRLF state if mm receives PLMN SEARCH REQ from req it simply sends confirmation with result
  as aborted.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_wrlf_mmr_plmn_search_req( mm_cmd_type *message )
{
   mmr_plmn_search_cnf_s_type *mmr_plmn_search_cnf = NULL;

   mmr_plmn_search_cnf = (mmr_plmn_search_cnf_s_type*)modem_mem_calloc(1, sizeof(mmr_plmn_search_cnf_s_type), MODEM_MEM_CLIENT_NAS);

   mm_check_for_null_ptr((void*)mmr_plmn_search_cnf);

   MSG_HIGH_DS_0(MM_SUB,"=MM= MM received MMR_PLMN_SEARCH_REQ during WRLF, sending search confirmation with result aborted");

   if ( mmr_plmn_search_cnf != NULL ) 
   {
     mmr_plmn_search_cnf->network_selection_mode = message->cmd.mmr_plmn_search_req.network_selection_mode;
     mmr_plmn_search_cnf->plmn_search_result = REG_MM_PLMN_SEARCH_ABORTED;
     memset(&mmr_plmn_search_cnf->found_plmn_list, 0x00,sizeof(sys_found_plmn_list_u_type));
     mmr_plmn_search_cnf->transaction_id = message->cmd.mmr_plmn_search_req.transaction_id;
     mmr_plmn_search_cnf->service_search = message->cmd.mmr_plmn_search_req.service_search; 
     mm_send_mmr_plmn_search_cnf(mmr_plmn_search_cnf);

     modem_mem_free(mmr_plmn_search_cnf, MODEM_MEM_CLIENT_NAS);

     mmr_plmn_search_cnf = NULL;
   }
}
#endif
/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMR_PLMN_SEARCH_REQ

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_plmn_search_req( mm_cmd_type *message )
{
  mmr_plmn_search_cnf_s_type *mmr_plmn_search_cnf = NULL;
  reg_mm_plmn_search_status_e_type  search_result = REG_MM_PLMN_SEARCH_SUCCESS;
#if defined(FEATURE_LTE)
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);
#endif

  if (message->cmd.hdr.message_id == (int) MMR_PLMN_SEARCH_REQ)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received MMR_PLMN_SEARCH_REQ");

    if ((mm_waiting_for_stop_mode_cnf)
         || (mm_rat_handler_status == MM_RAT_STOP_GSM_IN_PROGRESS) 
         || (mm_rat_handler_status == MM_RAT_STOP_WCDMA_IN_PROGRESS)
         || (mm_rat_handler_status == MM_RAT_STOP_TDSCDMA_IN_PROGRESS)
        ) 
    {
       MSG_HIGH_DS_0(MM_SUB,"=MM= Pending MMR_PLMN_SEARCH_REQ - MM Waiting for STOP MODE CNF from AS");
       mmcoord_reg_req_pending = FALSE;
#ifdef FEATURE_WRLF_SYSTEM_SEL
       if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE)
       {
         mm_wrlf_rat_change_pending = FALSE;
       }
#endif
       mm_plmn_list_req_pending = TRUE;
       mmcoord_pending_reg_list_req_message.cmd.hdr = message->cmd.hdr;
       mmcoord_pending_reg_list_req_message.cmd.mmr_plmn_search_req =
                   message->cmd.mmr_plmn_search_req;
       return;
    }


#ifdef FEATURE_FEMTO_CSG
     mm_csg_search_type = message->cmd.mmr_plmn_search_req.csg_search_type;
    if((message->cmd.mmr_plmn_search_req.service_search == SYS_SERVICE_SEARCH_CSG) &&
        (mm_is_csg_search_allowed(mm_csg_search_type, mm_csg_selection_config, &search_result) == FALSE))
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= CSG Search not allowed");
      mm_send_empty_mmr_plmn_search_cnf(&(message->cmd.mmr_plmn_search_req), search_result);
      return;
    }
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Received CSG Search Type - %d",mm_csg_search_type);
#endif

#ifdef FEATURE_DUAL_SIM
    if((mm_check_plmn_search_in_progress_on_other_subs() == TRUE) &&
            (message->cmd.mmr_plmn_search_req.network_selection_mode != SYS_NETWORK_SELECTION_MODE_MANUAL))
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= Rejecting PLMN search req as PLMN search is in progress on other sub");
      mm_send_empty_mmr_plmn_search_cnf(&(message->cmd.mmr_plmn_search_req),REG_MM_PLMN_SEARCH_ABORTED);
      return;
    }
#ifdef FEATURE_DUAL_DATA
    else if((mm_check_mms_progress_on_non_dds_sub() == TRUE) &&
             (message->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL) &&
             (message->cmd.mmr_plmn_search_req.service_search == SYS_SERVICE_SEARCH_NONE))
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= Rejecting PLMN search req as MMS is in progress on other sub");
      mm_send_empty_mmr_plmn_search_cnf(&(message->cmd.mmr_plmn_search_req),REG_MM_PLMN_SEARCH_ABORTED_DUE_TO_NO_LOCK);
      return;
    }
#endif
#endif
#if defined (FEATURE_DUAL_SIM) && defined (FEATURE_TDSCDMA)
  else if(mm_check_dual_tds_concurrency(
        &message->cmd.mmr_plmn_search_req.rat_pri_list_info) == TRUE)
  {
    MSG_HIGH_DS_0(MM_SUB, 
      "=MM= Reject search request to be retried with TDS in rat list as other sub TDS is not deactivated");
    mm_send_empty_mmr_plmn_search_cnf(&message->cmd.mmr_plmn_search_req,
                                       REG_MM_PLMN_SEARCH_ABORTED_DUE_TO_NO_LOCK);
    return;
  }
#endif
    /* ---------------------------------------
    ** Check if the RAT change is supported.
    ** ---------------------------------------*/
    if(((mm_serving_plmn.info.active_rat == SYS_RAT_GSM_RADIO_ACCESS) ||
        (mm_serving_plmn.info.active_rat == SYS_RAT_UMTS_RADIO_ACCESS) ||
        (mm_serving_plmn.info.active_rat == SYS_RAT_TDS_RADIO_ACCESS))
       )
    {
      MSG_HIGH_DS_1(MM_SUB ,"=MM= Dual Mode. Sending request on Active RAT %d", mm_serving_plmn.info.active_rat);
      message->cmd.mmr_plmn_search_req.rat = mm_serving_plmn.info.active_rat;
    }
	else if(mmode_nv_setting_for_device_mode == SYS_MODEM_DEVICE_MODE_SINGLE_SIM || mm_sub_is_rat_enabled(SYS_RAT_GSM_RADIO_ACCESS) == FALSE)
	{
      if(mm_checkif_rat_pri_list_has_rat(&message->cmd.mmr_plmn_search_req.rat_pri_list_info,SYS_RAT_GSM_RADIO_ACCESS ) == TRUE)
      {
       message->cmd.mmr_plmn_search_req.rat = SYS_RAT_GSM_RADIO_ACCESS;
      }
      else if(mm_checkif_rat_pri_list_has_rat(&message->cmd.mmr_plmn_search_req.rat_pri_list_info,SYS_RAT_UMTS_RADIO_ACCESS ) == TRUE)
      {
	   message->cmd.mmr_plmn_search_req.rat = SYS_RAT_UMTS_RADIO_ACCESS;
      }
      else if(mm_checkif_rat_pri_list_has_rat(&message->cmd.mmr_plmn_search_req.rat_pri_list_info,SYS_RAT_TDS_RADIO_ACCESS ) == TRUE)
      {
	   message->cmd.mmr_plmn_search_req.rat = SYS_RAT_TDS_RADIO_ACCESS;
      }
      else
      {
	   MSG_HIGH_DS_0(MM_SUB, "=MM=Rat list does not have GWT and so looping back confirmation");
	   mm_send_empty_mmr_plmn_search_cnf(&(message->cmd.mmr_plmn_search_req), search_result);
	   return;
      }
	}
    else
    {
      message->cmd.mmr_plmn_search_req.rat = SYS_RAT_GSM_RADIO_ACCESS;
    }

    mm_ue_pref_rat = message->cmd.mmr_plmn_search_req.rat;
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if((mm_wcdma_rlf_state != WCDMA_RLF_IDLE) &&
       (message->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL) &&
       (message->cmd.mmr_plmn_search_req.service_search == SYS_SERVICE_SEARCH_NONE))
    {
      mm_ue_pref_rat = SYS_RAT_UMTS_RADIO_ACCESS;
      message->cmd.mmr_plmn_search_req.rat = SYS_RAT_UMTS_RADIO_ACCESS;
    }
#endif
    if ( ((mm_ue_pref_rat == SYS_RAT_GSM_RADIO_ACCESS) && (!mm_gsm_supported)) ||
         ((mm_ue_pref_rat == SYS_RAT_TDS_RADIO_ACCESS) && (!mm_tdscdma_supported)) ||
         ((mm_ue_pref_rat == SYS_RAT_UMTS_RADIO_ACCESS) && (!mm_wcdma_supported))
         /*
         ** If PS detach or service change is in progress and MM receives a plmn 
         ** search request then send an empty list to REG with status aborted,
         ** so as to make REG come out of automatic plmn search state
         */
         || (mm_ps_detach_state != MM_PS_DETACH_STATE_NONE)
         || (mm_serv_change_pending)
       )
    {

      mmr_plmn_search_cnf = (mmr_plmn_search_cnf_s_type*)modem_mem_calloc(1, sizeof(mmr_plmn_search_cnf_s_type), MODEM_MEM_CLIENT_NAS);

       mm_check_for_null_ptr((void*)mmr_plmn_search_cnf);       

       MSG_HIGH_DS_1(MM_SUB ,"=MM= RAT change to %d 0=G/1=W not supported or ps detach state is not NONE",mm_ue_pref_rat);
       if ( mmr_plmn_search_cnf != NULL ) 
       {
         mmr_plmn_search_cnf->network_selection_mode = message->cmd.mmr_plmn_search_req.network_selection_mode;
         mmr_plmn_search_cnf->plmn_search_result = REG_MM_PLMN_SEARCH_ABORTED;
         memset(&mmr_plmn_search_cnf->found_plmn_list, 0x00,
                  sizeof(sys_found_plmn_list_u_type));
         mmr_plmn_search_cnf->transaction_id = message->cmd.mmr_plmn_search_req.transaction_id;


         mmr_plmn_search_cnf->service_search = message->cmd.mmr_plmn_search_req.service_search; 

         mm_send_mmr_plmn_search_cnf( mmr_plmn_search_cnf );

         modem_mem_free(mmr_plmn_search_cnf, MODEM_MEM_CLIENT_NAS);

         mmr_plmn_search_cnf = NULL;
       }
    }
    else
    {
       mm_rat_change_pending  = mm_rat_change_requested(mm_ue_pref_rat);
#ifdef FEATURE_LTE
       if((emm_ctrl_data_ptr->l2gcco_state == EMM_LTOG_CCO_IN_PROGRESS)&&
         (message->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
#ifdef FEATURE_SGLTE
           && MM_IS_SGLTE_ON_PS_STACK_ID
#endif
         )
       {
         MSG_HIGH_DS_0(MM_SUB,"=MM= Received Manual PLMN list req during CCO, Set CCO state to aborted");
         emm_ctrl_data_ptr->l2gcco_state = EMM_LTOG_CCO_ABORTED;
       }
#endif
       if ((mm_conn_mode_manual_search_enabled == TRUE) && 
           (message->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL) &&
		   (message->cmd.mmr_plmn_search_req.srch_priority != SYS_SRCH_PRIORITY_LOW))
       {
         if((rrc_connection_status != MMRRC_IDLE) &&
           (is_umts_mode() == TRUE )&&
           (mm_sms_is_active == FALSE)&&
           (gmm_state != GMM_REGISTERED_INITIATED)&&
           (gmm_state != GMM_ROUTING_AREA_UPDATING_INITIATED)&&
           (gmm_state != GMM_DEREGISTERED_INITIATED)&&
           (cs_session_status == MMRRC_IDLE)&&
           (mm_is_mt_page_in_progress() == FALSE)&&
           (mm_state == MM_IDLE))
          {
            MSG_HIGH_DS_0(MM_SUB,"=MM= Received Manual PLMN search req,aborting RRC established for MO data ");
            mm_abort_rr(RRC_PS_DOMAIN_CN_ID, FALSE);
          }
#ifdef FEATURE_DUAL_SIM
          mm_abort_connection_due_to_mplmn_on_other_subs();
#endif
       }
         /* ----------------------------------------------------------------------
         ** Pass PLMN_SEARCH_REQ to both MM & GMM independent of MS operation mode
         ** ---------------------------------------------------------------------- */
         mm_handle_message( message );
         gmm_handle_message( message );
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_mmr_plmn_search_req() */

void mmcoord_process_subsription_ready(void)
{
  boolean                 status = FALSE;
  mmgsdi_session_info_query_type    session_info_query;
  mmgsdi_session_get_info_type      session_get_info_ptr;
#ifdef FEATURE_DUAL_SIM
  sys_modem_as_id_e_type            mm_other_as_id;
#endif
#ifdef FEATURE_SEGMENT_LOADING
  interface_t* ptr_W = NULL;
#endif

#ifdef FEATURE_LTE
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);
#endif

#ifdef FEATURE_SEGMENT_LOADING
   ptr_W = get_wcdma_interface();
#endif

  mm_serving_plmn.info.sim_state = SYS_SIM_STATE_NOT_AVAILABLE;

  mm_need_to_process_sim_avail_req = FALSE;

  /* get SIM CARD MODE */
  session_info_query.query_type = MMGSDI_SESSION_INFO_QUERY_BY_TYPE;
  session_info_query.query_key.session_type = mm_sim_mmgsdi_info.session_type;
  if (MMGSDI_SUCCESS != mmgsdi_session_get_info_sync(session_info_query, &session_get_info_ptr))
  {
    MSG_ERROR_DS(MM_SUB, "=MM= SIM card mode get failed!!!", 0,0,0);
  }
  mm_sim_card_mode = session_get_info_ptr.app_data.app_type;
  
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
  if (mm_sim_mmgsdi_info.slot_id != session_get_info_ptr.slot_id)
  {
    mm_stored_imei[0] = 0;
    for(mm_other_as_id = SYS_MODEM_AS_ID_1 ; mm_other_as_id < MAX_NAS_STACKS ; mm_other_as_id++)
    {
      if (mm_sim_card_mode_sim[mm_other_as_id] == MMGSDI_APP_UNKNOWN)
      {
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_IMEI)
        mm_stored_imei_sim[mm_other_as_id][0]= 0;
#endif
      }
    }
  }
  mm_sim_mmgsdi_info.slot_id = session_get_info_ptr.slot_id;
#endif

  if ((mm_sim_card_mode == MMGSDI_APP_UNKNOWN) || 
      (mm_sim_card_mode == MMGSDI_APP_NONE) ||
      ((session_get_info_ptr.app_state != MMGSDI_APP_STATE_READY)&&
      (session_get_info_ptr.app_state != MMGSDI_APP_STATE_ILLEGAL)&&
      (session_get_info_ptr.app_state != MMGSDI_APP_STATE_WAIT_FOR_SUB_OK)&&
      (session_get_info_ptr.app_state != MMGSDI_APP_STATE_EXT_PERSO_FAILED)&&
      (session_get_info_ptr.app_state != MMGSDI_APP_STATE_READY_FOR_PERSO)))
  {
    MSG_ERROR_DS(MM_SUB, "=MM= SIM either not initialized or invalid,app_type=%d,app_state=%d", mm_sim_card_mode,session_get_info_ptr.app_state,0);
    mm_sim_card_mode = MMGSDI_APP_UNKNOWN;
    mm_serving_plmn.info.sim_state = SYS_SIM_STATE_NOT_AVAILABLE;
    mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_CARD_MODE_UNKNOWN;
    mm_send_mmr_sim_available_cnf();
    return;
  }
#ifdef FEATURE_DUAL_SIM
  else if( mm_emm_is_illegal_me_on_any_sub((sys_modem_as_id_e_type)mm_as_id))
  {
      mm_sim_card_mode = MMGSDI_APP_UNKNOWN;
      mm_serving_plmn.info.sim_state = SYS_SIM_STATE_NOT_AVAILABLE;
      mm_send_mmr_sim_available_cnf();
      (void) mmgsdi_session_manage_illegal_subscription(mm_sim_mmgsdi_info.session_id,
                                                           MMGSDI_SESSION_APP_IS_ILLEGAL);
       mm_illegal_me_sub = ILLEGAL_ME_ON_OTHER_SUB;
      return;
  }
#endif
  else 
  {
    status = mm_load_sim_data();

    if( status )
    {
      mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_LOAD_CARD_MODE_SUCCESS;    
      mm_serving_plmn.info.sim_state = SYS_SIM_STATE_AVAILABLE;

      (void)auth_load_security_context() ;
#ifdef FEATURE_LTE  
      if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS)  == TRUE)
      {
        emm_load_security_context(emm_ctrl_data_ptr);
        (void)emm_update_rplmn(emm_ctrl_data_ptr);
        /* reset the mm nas ul count */
        mm_reset_ul_count_at_generation();
      }
      /* Read all LTE_NAS NV items */
      lte_nas_nv_read(emm_ctrl_data_ptr);
#endif
#ifdef FEATURE_DUAL_DATA
      mm_read_max_active_data();
#endif 
    } 
    else
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= mm_load_sim_data retruned failure ");
      mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_FAILED_LOAD_CARD_MODE_FAILED;
    }
  }

#if defined(FEATURE_DUAL_SIM) && !defined(FEATURE_LTE)
    mm_sim_mode_pref = mm_sim_mode_pref_local;
#endif
  if( status )
  {
    if (mm_wcdma_supported &&
        mm_sub_is_rat_enabled(SYS_RAT_UMTS_RADIO_ACCESS))
    {
      /* Send SIM insertion event to WCDMA RRC */
      mm_send_rrc_sim_inserted_req( SYS_RAT_UMTS_RADIO_ACCESS);
    }
    if (mm_tdscdma_supported &&
        mm_sub_is_rat_enabled(SYS_RAT_TDS_RADIO_ACCESS))
    {
      /* Send SIM insertion event to TD-SCDMA RRC */
      mm_send_rrc_sim_inserted_req( SYS_RAT_TDS_RADIO_ACCESS);
    }
#ifdef FEATURE_GSM
    mm_send_rr_sim_inserted_req();
#endif

#ifdef FEATURE_LTE  
    if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS)  == TRUE)
    {
      /* Send sim insert to E-RRC*/
      emm_send_sim_update(emm_ctrl_data_ptr,TRUE) ; 
      emm_send_rrc_system_update(emm_ctrl_data_ptr) ; 
    } 
#endif
  }

#ifdef FEATURE_LTE
  MSG_HIGH_DS_3( MM_SUB, "=MM= lte accepted VPLMN [%x %x %x]",
       emm_ctrl_data_ptr->emm_lte_accepted_vplmn.identity[0],
       emm_ctrl_data_ptr->emm_lte_accepted_vplmn.identity[1],
       emm_ctrl_data_ptr->emm_lte_accepted_vplmn.identity[2]);
#endif
  mm_send_mmr_sim_available_cnf();
    
}


/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMR_SIM_AVAILABLE_REQ

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_mmr_sim_available_req( mm_cmd_type *message )
{
  if (message->cmd.hdr.message_id == (int) MMR_SIM_AVAILABLE_REQ)
  {
#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
    MSG_HIGH_DS_3( MM_SUB, "=MM= MM received MMR_SIM_AVAILABLE_REQ DS:%d, Overall Feature: %d, Device Mode:%d", 
                                     mm_dual_standby_mode, mm_sub_config, mmode_nv_setting_for_device_mode);
#else
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received MMR_SIM_AVAILABLE_REQ");
#endif

    mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_STARTED;

#ifdef FEATURE_DUAL_SIM
    mm_nv_context_id = message->cmd.mmr_sim_available_req.nv_context_id;
    rex_enter_crit_sect(&mm_sim_crit_sect);
    if((mm_sim_mmgsdi_info_sim[mm_as_id].session_type != 
                      message->cmd.mmr_sim_available_req.mmgsdi_session_type) &&
       (mm_sim_session_matching_with_other_sub(message->cmd.mmr_sim_available_req.mmgsdi_session_type)))
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= Swapping MMGSDI info");
      nas_swap_mmgsdi_info(mm_as_id, mm_sim_mmgsdi_info_sim,mm_waiting_for_open_session_cnf_sim, message->cmd.mmr_sim_available_req.mmgsdi_session_type);
    }
    /* Unexpected but try to recover */ 
    else if(mm_sim_mmgsdi_info_sim[mm_as_id].session_type != 
                      message->cmd.mmr_sim_available_req.mmgsdi_session_type)
    {
      MSG_ERROR_DS(MM_SUB, "=MM= Unexpected session type",
           message->cmd.mmr_sim_available_req.mmgsdi_session_type,0,0);
      mm_sim_mmgsdi_info.session_type = message->cmd.mmr_sim_available_req.mmgsdi_session_type;
      mm_sim_mmgsdi_info.session_id = 0;
      mm_sim_card_mode = MMGSDI_APP_UNKNOWN;
    }
    rex_leave_crit_sect(&mm_sim_crit_sect);

#endif

    if((mm_sim_mmgsdi_info.client_id == 0) || (mm_waiting_for_open_session_cnf))
    {
      MSG_HIGH_DS_3(MM_SUB, "=MM= Waiting for MMGSDI open session %d/client reg response, client:H:0x%x L:0x%x",
                  mm_waiting_for_open_session_cnf, (dword)(mm_sim_mmgsdi_info.client_id >> 32), 
                  (dword)(mm_sim_mmgsdi_info.client_id & 0xFFFFFFFF));
      mm_need_to_process_sim_avail_req = TRUE;
	  mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_PENDED_WAITING_FOR_MMGSDI_SESSION;
    }
    else if (mm_sim_mmgsdi_info.session_id == 0)
    {
#ifdef FEATURE_DUAL_SIM
      if(message->cmd.mmr_sim_available_req.mmgsdi_session_type >= MMGSDI_MAX_SESSION_TYPE_ENUM)
      {
        MSG_FATAL_DS( MM_SUB, "=MM= Invalid MMGSDI session type from CM %d", message->cmd.mmr_sim_available_req.mmgsdi_session_type,0,0 );
      }
      else
#endif
      {
        mm_sim_mmgsdi_info.client_id    = message->cmd.mmr_sim_available_req.mmgsdi_client_id;

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
        if (!mm_sim_open_session(mm_sim_mmgsdi_info.session_type,mm_sim_mmgsdi_info.client_id,(sys_modem_as_id_e_type)mm_as_id))
#else
        if (!mm_sim_open_session(mm_sim_mmgsdi_info.session_type,mm_sim_mmgsdi_info.client_id))
#endif
        {
          MSG_ERROR_DS( MM_SUB, "=MM= MMGSDI OPEN SESSION failed!!!", 0, 0,0 );
		  mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_FAILED_MMGSDI_SESSION_OPEN_FAILED;
          mm_send_mmr_sim_available_cnf();
        }
        else
        {
          mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_PENDED_REQUESTED_MMGSDI_SESSION;
		  MSG_HIGH_DS_0(MM_SUB, "=MM= MMGSDI sessions requested ");
          mm_need_to_process_sim_avail_req = TRUE;
        }
      }

    }
    else
    {
      mm_debug_mmr_sim_available_status = MM_DEBUG_SIM_AVAILABLE_MMGSDI_SESSION_AVAILABLE;
      /* ----------------------------------
      ** Alert MM/GMM to read SIM data and send to RR(C)
      ** ---------------------------------- */
      mmcoord_process_subsription_ready();
    }

  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end of mmcoord_process_mmr_sim_available_req() */
void mmcoord_process_mmgsdi_subs_ready(mm_cmd_type* message)
{

#if(defined(TEST_FRAMEWORK) && defined(FEATURE_LTE))
#error code not present
#endif
  if (message->cmd.hdr.message_id == (int) SIM_MM_SUBS_READY_IND  )
  {

    mm_waiting_for_open_session_cnf = FALSE;

    /* ----------------------------------
    ** Alert MM/GMM to read SIM data and send to RR(C)
    ** ---------------------------------- */
    if(mm_need_to_process_sim_avail_req)
    {
      mmcoord_process_subsription_ready();
    }

  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG message %d", message->cmd.hdr.message_id, 0,0 );
  }
}

#ifdef FEATURE_FEMTO_CSG
/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMGSDI_GET_FILE_ATTR_CNF

DESCRIPTION
  This function process result returned from MMGSDI for get file attr 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_mmgsdi_get_file_attr_cnf(mm_cmd_type* message)
{
  
  if(message->cmd.hdr.message_id == (int) SIM_MM_GET_FILE_ATTR_CNF)
  {
    mmsim_process_get_file_attr_cnf(message); 
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG message %d", message->cmd.hdr.message_id, 0,0 );
  }

}

/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMGSDI_READ_FILE_CNF

DESCRIPTION
    This function process result of READ file from MMGSDI

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_mmgsdi_read_file_cnf(mm_cmd_type* message)
{
  
  if(message->cmd.hdr.message_id == (int) SIM_MM_READ_FILE_CNF)
  {
    mmsim_process_read_file_cnf(message); 
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG message %d", message->cmd.hdr.message_id, 0,0 );
  }

}

/*===========================================================================

FUNCTION MMCOORD_PROCESS_FILE_REFRESH_IND

DESCRIPTION
  This function process EF file refresh indicaiton 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_file_refresh_ind(mm_cmd_type* message)
{

  mmgsdi_file_enum_type  file_name;

  if(message->cmd.hdr.message_id == (int) MMR_FILE_REFRESH_IND)
  {
    file_name = message->cmd.mmr_file_refresh_ind.file_name;
    
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Received Refresh for file %d", file_name);
    switch(file_name)
    {
      case MMGSDI_USIM_OCSGL:
       mmsim_start_read_operator_csg_list();
       break;
  
     default:
       break;
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG message %d", message->cmd.hdr.message_id, 0,0 );
  }
}

#endif 

/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMR_SIM_NOT_AVAILABLE_REQ

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_mmr_sim_not_available_req(mm_cmd_type *message)
{

#ifdef FEATURE_WCDMA
  rrc_cmd_type *rrc_cmd;
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_LTE
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (message->cmd.hdr.message_id == (int) MMR_SIM_NOT_AVAILABLE_REQ)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received MMR_SIM_NOT_AVAILABLE_REQ");  
    
#ifdef FEATURE_WCDMA
    if (mm_wcdma_supported && 
        mm_sub_is_rat_enabled(SYS_RAT_UMTS_RADIO_ACCESS))
    {
      rrc_cmd = mm_get_rrc_cmd_buf( SYS_RAT_UMTS_RADIO_ACCESS );

      if (rrc_cmd == NULL)
      {
        MSG_FATAL_DS( MM_SUB, "=MM= ERROR: Unable to allocate memory", 0,0,0 );
      }
  
      rrc_cmd->cmd_hdr.cmd_id = RRC_INVALIDATE_SIM_DATA_REQ;

      send_cmd_to_rrc( rrc_cmd, SYS_RAT_UMTS_RADIO_ACCESS );
    }

    if (mm_tdscdma_supported && 
        mm_sub_is_rat_enabled(SYS_RAT_TDS_RADIO_ACCESS))
    {
      rrc_cmd = mm_get_rrc_cmd_buf( SYS_RAT_TDS_RADIO_ACCESS );

      if (rrc_cmd == NULL)
      {
        MSG_FATAL_DS( MM_SUB, "=MM= ERROR: Unable to allocate memory", 0,0,0 );
      }

      rrc_cmd->cmd_hdr.cmd_id = RRC_INVALIDATE_SIM_DATA_REQ;

      send_cmd_to_rrc( rrc_cmd, SYS_RAT_TDS_RADIO_ACCESS );
    }
#endif /* FEATURE_WCDMA */

#ifdef FEATURE_GSM

    /* ------------------------------------
    ** Tell RR that there is no SIM anymore
    ** ------------------------------------ */
    mm_send_rr_sim_removed_req();

#endif/* FEATURE_GSM */


#ifdef FEATURE_GSTK
    if (mm_serving_plmn.info.sim_state != SYS_SIM_STATE_CS_PS_INVALID)
    {
      mm_gstk_set_location_status_evt(FALSE);
    }
#endif

/* SIM state is modified on calling emm_unload_sim_data.  
     Any requirment of using old sim state should be put before this function*/

#ifdef FEATURE_LTE
    emm_unload_sim_data(emm_ctrl_data_ptr);
    if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS)  == TRUE)
    {
      emm_send_sim_update(emm_ctrl_data_ptr,TRUE) ;
      emm_send_rrc_forbidden_ta_la_list_reset();
    }
#endif

    /* Set the SIM status of the serving PLMN as not available */    
    mm_serving_plmn.info.sim_state = SYS_SIM_STATE_NOT_AVAILABLE;

    /*Set the IMSI lenght to zero any how IMSI will be read from the SIM when SIM is avaialble*/
    mm_stored_imsi.length = 0;

    /* ----------------------
    ** Clear out the SIM data
    ** ---------------------- */
    mm_sim_card_mode = MMGSDI_APP_UNKNOWN;
    mm_delete_lu_status( NOT_UPDATED );
    mm_clear_forbidden_LAIs();
#ifdef FEATURE_FEMTO_CSG
    /* clear rejected csg list */
    mm_clear_rejected_csg_list();
#endif

    gmm_delete_ps_location_information();
    gmm_delete_gprs_cksn();

    /* --------------------------------------------------------
    ** Send the log packets which updates TMSI and PTMSI values
    ** -------------------------------------------------------- */
    mmsend_nas_ue_dynamic_id_log_packet( LOG_TMSI_TYPE );
    mmsend_nas_ue_dynamic_id_log_packet( LOG_PTMSI_TYPE );

    mm_substate_control( MM_STD_NO_IMSI );
    gmm_set_substate( GMM_NO_IMSI );

    mm_cs_auth_sent_to_card = FALSE;    
    mm_ps_auth_sent_to_card = FALSE;


    /* --------------------------------------------------------------------
    ** The T3212 timer is stopped when the mobile station is deactivated
    ** (i.e., equipment powered down or SIM removed) (3GPP TS 24.008 4.2.2)
    ** -------------------------------------------------------------------- */
    mm_stop_timer( TIMER_T3212 );

#ifdef FEATURE_FEMTO_CSG
    /*SIM not available should not be processed on emm if sub is not multimode sub*/
    if((mm_nv_csg_support > SYS_CSG_SUPPORT_DISABLED) &&
       (mm_sub_is_rat_enabled(SYS_RAT_UMTS_RADIO_ACCESS) ||
        mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS) || 
        mm_sub_is_rat_enabled(SYS_RAT_TDS_RADIO_ACCESS)))
    {
      if(mm_using_allowed_csg_nv == TRUE)
      {
        mm_sim_write_allowed_csg_to_nv(&mm_sim_active_allowed_csg_list);
        mm_using_allowed_csg_nv = FALSE;
      }
      mmsim_reset_csg_data();
      mm_send_rrc_csg_white_list_ind(&mm_sim_active_operator_csg_list, &mm_sim_active_allowed_csg_list);
      mm_send_rr_csg_white_list_ind();
  #ifdef FEATURE_LTE
     if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
     {
       emm_send_rrc_system_update(emm_ctrl_data_ptr);
     }
  #endif 
    }
#endif 

    mm_send_mmr_sim_not_available_cnf();
#ifdef FEATURE_DUAL_SIM
    ghdi_clear_slot_mapping( (sys_modem_as_id_e_type) mm_as_id);
#endif
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG message %d", message->cmd.hdr.message_id, 0,0 );
  }
}



/*===========================================================================

FUNCTION MMCOORD_PEND_MMR_REG_REQ

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_pend_mmr_reg_req( mm_cmd_type *message )
{
  mmcoord_reg_req_pending = TRUE;
  mmcoord_pending_reg_message.cmd.mmr_reg_req = message->cmd.mmr_reg_req;
  mm_reg_req_trans_id--;
}



/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMR_REG_REQ_NOW

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_reg_req_now( mm_cmd_type *message )
{
 
   /* 
     If requested service domain is NO_SRV then reject the MMR_REG_REQ 
     with service status NO_SRV, REG should pass NO_SRV to CM and CM should 
     come back with PLMN selection in LIMITED mode 
  */ 

#if defined(FEATURE_LTE)

  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);
#endif

  mm_ready_to_send_reg_cnf =   FALSE; /* This flag is readied for the new REG_REQ*/
   
  /*Set the attch failed indication flag that will be used later for sending attach failed indication in case of Attach failure*/
  if(((message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ||
       (message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)) &&
      !(( mm_serving_plmn.info.service_status == SYS_SRV_STATUS_SRV) &&
       ((mm_serving_plmn.info.reg_service_domain == SYS_SRV_DOMAIN_CS_PS) || 
       (mm_serving_plmn.info.reg_service_domain == SYS_SRV_DOMAIN_PS_ONLY))) &&
      ((message->cmd.mmr_reg_req.req_service_domain  == SYS_SRV_DOMAIN_CS_PS) || 
      (message->cmd.mmr_reg_req.req_service_domain  == SYS_SRV_DOMAIN_PS_ONLY)))
  {
     mm_mmr_attach_ind_status          = ATTACH_FAILED_IND_REQUIRED;
  }
  else
  {
     mm_mmr_attach_ind_status          = ATTACH_FAILED_IND_NOT_REQUIRED;
  }
   
  if (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_NO_SRV)
  {
    if (mm_serving_plmn.info.sim_state == SYS_SIM_STATE_AVAILABLE)
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= Changing the req_service_domain to CS_ONLY ");
      message->cmd.mmr_reg_req.req_service_domain = SYS_SRV_DOMAIN_CS_ONLY;
    }      
    else
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= Rejecting REG_REQ as Requested service domain is NO_SRV ");
  
      mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
      mmcoord_mmr_reg_cnf.service_state.service_status = SYS_SRV_STATUS_NO_SRV;
      if (mm_managed_roaming_enabled)
      {
        mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_NO_SRV;
      }
#ifdef FEATURE_ENHANCED_NW_SELECTION
      if (reg_sim_read_ens_flag())
      {
        mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_NO_SRV;
      }
#endif

      mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;
      mm_send_mmr_reg_cnf();
      mm_reg_waiting_for_reg_cnf = FALSE;
      mm_ready_to_send_reg_cnf   = FALSE;
      return;
    }
  }

#if defined(FEATURE_DUAL_SIM) 
  if (
#ifdef FEATURE_DUAL_DATA
      (mm_max_active_data == 1) &&
#endif
      (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) && 
       ((message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS) ||
        (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_PS_ONLY)))
  {
    sys_modem_as_id_e_type      mm_other_as_id,mm_as_id_local;
    for(mm_other_as_id = SYS_MODEM_AS_ID_1; mm_other_as_id < MAX_NAS_STACKS ; mm_other_as_id++)
    {
      if ((mm_other_as_id != (sys_modem_as_id_e_type)mm_as_id)&& (gmm_state_sim[mm_other_as_id] != GMM_NULL))
    {
      MSG_HIGH_DS_2(MM_SUB, "=MM=DSDS: Domain pref(%d) changed to allow PS while PS is active on ds%d",
        message->cmd.mmr_reg_req.req_service_domain,mm_other_as_id+1);

      /* Force local detach on the other SUB */
        mm_as_id_local = (sys_modem_as_id_e_type)mm_as_id;
        mm_set_as_id(mm_other_as_id);

      gmm_force_local_detach_sim[mm_as_id] = FALSE;

      if(gmm_state == GMM_DEREGISTERED)
      {
        if(is_gsm_mode())
        {
          byte i;
          for (i=0; i < PTMSI_SIZE; i++)
          {
            gmm_stored_ptmsi.digit[i] = 0xFF;
            gmm_old_ptmsi.digit[i] = 0xFF;
          }
          gmmllc_gmm_trigger_llc_event(GMM_TRIGGER_PTMSI_ASSIGNED,FALSE,0) ;
        }
      }
      else
      {
        gmm_handle_local_gprs_detach();
      }
      mm_set_as_id(mm_as_id_local);
    }
  }
  }

  if ((mm_serving_plmn.ms_op_mode != message->cmd.mmr_reg_req.req_service_domain) &&
      ((message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS) ||
       (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_PS_ONLY)))
  {
    gmm_force_local_detach_sim[mm_sub_id] = FALSE;
  }
#endif 
  /* If request is for Manual mode type user selection, set current EPLMN list as invalid. Set it to valid if request is for
  ** Automatic / Limited routine mode.
  */
  if(message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL &&
     message->cmd.mmr_reg_req.type == MMR_REG_REQ_USER_SELECTION)
  {
    if (sys_eplmn_list_validate(FALSE))
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= EPLMN list is **invalidated**");
      mm_send_rrc_eq_plmn_list_change_ind();
      mm_send_rr_eq_plmn_list_change_ind(); 
#ifdef FEATURE_LTE
      emm_send_multimode_rrc_system_update_from_GW(emm_ctrl_data_ptr);
#endif     
    }

    if(mm_managed_roaming_enabled)
    {
      if((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) ||
         (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY))
      {
        if (gmm_state == GMM_DEREGISTERED ||
            gmm_state == GMM_DEREGISTERED_INITIATED ||
            gmm_state == GMM_REGISTERED_INITIATED)
        {
                    /* -------------------------------------------------------------
            ** The GPRS attach attempt counter shall be reset when:
            **  the MS is in substate ATTEMPTING-TO-ATTACH and
            ** a new routing area is entered;or
            ** an attach is triggered by CM sublayer requests.
            ** (3GPP TS 24.008 section 4.7.3).
            ** ------------------------------------------------------------- */
          gprs_attach_attempt_counter = 0;
          t3310_timeout_counter = 0;
          MSG_HIGH_DS_2( MM_SUB, "=MM= GPRS Attach Attempt counter = %d T3310 timeout counter = %d",gprs_attach_attempt_counter, t3330_timeout_counter);
        }
        else
        {
                     /* -------------------------------------------------------------
            ** The routing area updating attempt counter shall be reset when
            ** a new routing area is entered and the MS is in substate
            ** ATTEMPTING-TO-UPDATE (3GPP TS 24.008 section 4.7.5).
            ** ------------------------------------------------------------- */
          gmm_rau_attempt_counter = 0;
      
          t3330_timeout_counter = 0;
          MSG_HIGH_DS_2( MM_SUB, "=MM= RAU Attempt counter = %d T3330 timeout counter = %d", gmm_rau_attempt_counter,t3330_timeout_counter);
        }
      
        gmm_previous_registration_serving_rai.plmn_id.identity[0]       = 0xFF;
        gmm_previous_registration_serving_rai.plmn_id.identity[1]       = 0xFF;
        gmm_previous_registration_serving_rai.plmn_id.identity[2]       = 0xFF;
        gmm_previous_registration_serving_rai.location_area_code.lac[0] = 0xFF;
        gmm_previous_registration_serving_rai.location_area_code.lac[1] = 0xFF;
        gmm_previous_registration_serving_rai.routing_area_code         = 0xFF;
            
        gmm_previous_T3311_gmm_substate = GMM_NULL_SUBSTATE;
            
        if(mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
        {
              /* -------------------------------------
          ** Reset the MM LU attempt counter
          ** ------------------------------------- */
          mm_clear_lu_attempt_counter();
          MSG_HIGH_DS_1(MM_SUB ,"=MM= MM LU Attempt counter = %d",
                                                                 mm_lu_attempt_counter);
          mm_previous_idle_substate = NULL;
          if( (mm_managed_roaming_retry_lu_in_manual_mode ) &&
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
            ( reg_sim_plmn_in_low_priority_plmn_list(message->cmd.mmr_reg_req.plmn, message->cmd.mmr_reg_req.rat,(sys_modem_as_id_e_type)mm_as_id) ) 
#else
            ( reg_sim_plmn_in_low_priority_plmn_list(message->cmd.mmr_reg_req.plmn, message->cmd.mmr_reg_req.rat) )
#endif
          )
                        
          {
            mm_is_foreground_search_to_be_initiated = FALSE;
          }
        }
      
      }
      else if(mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)
      {
              /* -------------------------------------
           ** Reset the MM LU attempt counter
           ** ------------------------------------- */
        mm_clear_lu_attempt_counter();
        MSG_HIGH_DS_1(MM_SUB ,"=MM= MM LU Attempt counter = %d",
                                                               mm_lu_attempt_counter);
        mm_previous_idle_substate = NULL;
        if( (mm_managed_roaming_retry_lu_in_manual_mode) &&
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
            (reg_sim_plmn_in_low_priority_plmn_list(message->cmd.mmr_reg_req.plmn, message->cmd.mmr_reg_req.rat,(sys_modem_as_id_e_type)mm_as_id)) 
#else
            (reg_sim_plmn_in_low_priority_plmn_list(message->cmd.mmr_reg_req.plmn, message->cmd.mmr_reg_req.rat))
#endif
        )
                        
        {
            mm_is_foreground_search_to_be_initiated = FALSE;
        }
      }
    } 
  }
  else if(message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC ||
          message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)
  {
    if (sys_eplmn_list_validate(TRUE))
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= EPLMN list is now validated");
      mm_send_rrc_eq_plmn_list_change_ind();
      mm_send_rr_eq_plmn_list_change_ind();  
#ifdef FEATURE_LTE
      emm_send_multimode_rrc_system_update_from_GW(emm_ctrl_data_ptr);
#endif
    }
  }
  
  /* ---------------------------------------------------------
  ** Check to see if REG is requesting the CS_FOR be set to 1
  ** ---------------------------------------------------------*/
  if (((message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_ONLY) ||
       (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS))
      &&
      (message->cmd.mmr_reg_req.cs_for_request))
  {
    mm_follow_on_request = TRUE;
  }
  else if ((mm_held_cnm_service_req.present == FALSE)
  #if defined(FEATURE_LTE)
              && (mm_csfb_state != MM_CSFB_STATE_MO_CALL_ORIG)
  #endif
            )
  {
    mm_follow_on_request = FALSE;
  }
  /* ------------------------------------------------------------------------
  ** Set the follow on flag only when a PS service is added, either from NO_SERVICE
  ** or while in CS_SERVICE.  This should not get over written during a Detach Req
  ** from REG after an attach on demand, otherwise the CS call will also be released
  ** ------------------------------------------------------------------------*/
  if (((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_NO_SRV || 
        !((mm_serving_plmn.info.reg_service_domain == SYS_SRV_DOMAIN_PS_ONLY) ||
         (mm_serving_plmn.info.reg_service_domain == SYS_SRV_DOMAIN_CS_PS)))  &&
      (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_PS_ONLY)) ||
      ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) &&
       (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS)) ||
        (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS && 
        (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS ||
         message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_PS_ONLY)))
  {
    gmm_follow_on_request = message->cmd.mmr_reg_req.ps_for_request;
  }
  else
  {
    gmm_follow_on_request = FALSE;
  }
  MSG_HIGH_DS_3(MM_SUB ,"=MM= Setting the ATTACH FAILED IND flag Value = %d CS_FOR = %d PS_FOR = %d",
  	                          mm_mmr_attach_ind_status,
  	                          mm_follow_on_request,
  	                          gmm_follow_on_request);

  mm_reg_req_revd_active_call = FALSE;
  
  mm_ue_pref_rat = message->cmd.mmr_reg_req.rat;


  /* ------------------------------------------------------------------
  ** Activate the MM internal state machine if mm_state is NULL.
  ** ------------------------------------------------------------------ */
  if (mm_state == MM_NULL)
  {
    /* Activate MM internal state machine */
    mm_activate_state( FALSE );
  }

#if defined(FEATURE_LTE)
  if (mm_checkif_rat_pri_list_has_rat(&message->cmd.mmr_reg_req.rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS) ) 
  {   
    emm_activate_lte(); 
  }
#endif 

  /* ----------------------------------------------------------------------
  ** Check if the RAT change or Service domain change is requested.
  ** --------------------------------------------------------------------*/
  mm_rat_change_pending  = mm_rat_change_requested(mm_ue_pref_rat);
  mm_serv_change_pending = mm_service_change_requested(
                             message->cmd.mmr_reg_req.req_service_domain );


#ifdef FEATURE_DUAL_DATA
  if(mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS &&
     message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_ONLY &&
     message->cmd.mmr_reg_req.volte_only == TRUE)
  {
    gmm_force_local_detach_sim[mm_as_id] = TRUE;
  }
#endif

  mm_serving_plmn.ms_op_mode = message->cmd.mmr_reg_req.req_service_domain;

  /* -----------------------------------------------------------------------
  ** Store the command. UE needs to access this command after a RAT change
  ** or initiate new search when LU rejected due to NO_SUITABLE_CELLS_IN_LA
  ** ----------------------------------------------------------------------*/
  mmcoord_pending_reg_message.cmd.mmr_reg_req = message->cmd.mmr_reg_req;
#if defined(FEATURE_LTE)
  /* Save the reg req for EMM */
  if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
  {
    ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
    memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type),
        &message->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
#if defined(FEATURE_DUAL_DATA)
    emm_ctrl_data_ptr->reg_req_pending_ptr->req_service_domain = 
              mm_get_effective_req_srv_domain(message->cmd.mmr_reg_req.req_service_domain,
                  emm_ctrl_data_ptr->reg_req_pending_ptr->volte_only, TRUE);
#endif 
  }
#endif

#ifdef FEATURE_SGLTE
  if(MM_SUB_IS_SGLTE)
  {
    MSG_HIGH_DS_4(MM_SUB, "=MM= SGLTE mm_rat_change_pending = %d MM_IS_IN_SGLTE_MODE= %d mmr_reg_req.type = %d, mm_sglte_current_stack_status = %d  ", 
                                mm_rat_change_pending,MM_IS_IN_SGLTE_MODE,
                                 message->cmd.mmr_reg_req.type,mm_sglte_current_stack_status);
  }
  if ((mm_rat_change_pending == TRUE)&&MM_IS_IN_SGLTE_MODE)
  { 
    if(message->cmd.mmr_reg_req.type == MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY)
     {
        MSG_HIGH_DS_0(MM_SUB,"=MM= Got PS timer expiry ..start merge   ");
        mm_sglte_current_stack_status  = SGLTE_MM_UPPER_LAYER_MERGING;      
        mmsend_mmr_stack_deact_ind();
     }
   }
  else if (!mm_rat_change_pending && MM_IS_IN_SGLTE_MODE && (mm_sglte_current_stack_status  == SGLTE_MM_UPPER_LAYER_MERGING))
  {
    MSG_HIGH_DS_0(MM_SUB,"=MM= merging complete for ps timer in oos rlf ");
    mmcoord_process_pstimer_expiry_merging();
    mm_sglte_current_stack_status = SGLTE_INACTIVE_STATE;
    mmcoord_reg_req_pending = FALSE;
    memset(&mmcoord_pending_reg_message,0x00,sizeof(mm_cmd_type));
    return;
  }  

#endif

  if ((mm_rat_change_pending) || (mm_serv_change_pending))
  {
    if(mm_rat_change_pending || mm_serving_plmn.ms_op_mode  == SYS_SRV_DOMAIN_PS_ONLY)
    {
      mm_initiate_lu_upon_entry_into_new_cell = FALSE;
    }
    if(mm_rat_change_pending || mm_serving_plmn.ms_op_mode  == SYS_SRV_DOMAIN_CS_ONLY)
    {
      gprs_attach_upon_entry_to_a_new_cell = FALSE;
      gmm_rau_upon_entry_to_a_new_cell = FALSE;
    }
    /* ----------------------------------------------------------------------
    ** Pass MMR_REG_REQ to both MM and GMM independent of MS operation mode.
    ** -------------------------------------------------------------------- */
    mm_handle_message ( message );
    gmm_handle_message( message );
  }
  else
  {
    /* ----------------------------------------------------------------------
    ** Pass MMR_REG_REQ to both MM and GMM independent of MS operation mode.
    ** -------------------------------------------------------------------- */
    mm_handle_message ( message );
    gmm_handle_message( message );

    /* ------------------------------------------------------------------------
    ** Continue with Registration if RAT change or service domain change is
    ** not requested. Otherwise the RAT/Service change is performed first and
    ** the LU procedure is initiated.
    ** -----------------------------------------------------------------------*/
    mmcoord_process_lu_procedure( message );
  }
}


/*===========================================================================

FUNCTION MMCOORD_REJECT_MMR_REG_REQ

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_reject_mmr_reg_req( void )
{
  mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_FAILURE_REJECT_REMAIN_ON_PLMN;
  mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
  mm_send_mmr_reg_cnf();

  mm_reg_waiting_for_reg_cnf       = FALSE;
  mm_ready_to_send_reg_cnf         = FALSE;
  mm_needs_to_send_mmr_service_ind = TRUE;
}


/*===========================================================================

FUNCTION MMCOORD_ABORT_RR_CONNECTION_THEN_PROCESS_MMR_REG_REQ

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_abort_rr_connection_then_process_mmr_reg_req( mm_cmd_type *message )
{

#if defined(FEATURE_LTE)

  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
#endif

  mm_rat_change_pending  = FALSE;
  mm_serv_change_pending = FALSE;

#if defined(FEATURE_LTE)

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);
#endif

  if (mm_timer_status[ TIMER_T3240 ] != TIMER_STOPPED)
  {
    mm_stop_timer( TIMER_T3240 );
  }
#if defined ( FEATURE_MMGPS ) || defined ( FEATURE_CGPS_UMTS_CP )
  if (mm_timer_status[ TIMER_T3241 ] != TIMER_STOPPED)
  {
    mm_stop_timer( TIMER_T3241 );
  }
#endif  
  if(rrc_connection_status != MMRRC_RELEASE_IN_PROGRESS)
  {
    if (is_umts_mode())
    {
        rrc_cmd_type *rrc_cmd_ptr = NULL;
  
  #ifdef FEATURE_WCDMA    
        rrc_cmd_ptr = mm_get_rrc_cmd_buf( mm_serving_plmn.info.active_rat );
  #endif /* FEATURE_WCDMA */
  
      if (rrc_cmd_ptr == NULL)
      {
        MSG_FATAL_DS( MM_SUB, "=MM= ERROR: Unable to allocate memory", 0,0,0 );
      }
  
      rrc_cmd_ptr->cmd_hdr.cmd_id = RRC_ABORT_REQ;
      if (((message->cmd.mmr_reg_req.type == MMR_REG_REQ_SYSTEM_CHANGE) ||
           ((mm_timer_status[TIMER_T3340] == TIMER_STOPPED) && 
            (mm_timer_status[TIMER_T3240] == TIMER_STOPPED) && 
            (mm_timer_status[TIMER_DETACH_REL] == TIMER_STOPPED))) && 
         (gmm_anite_gcf_flag == FALSE) 
#ifdef FEATURE_ENHANCED_NW_SELECTION
         &&
         (reg_sim_read_ens_flag() == FALSE)
#endif
         )
      {
        rrc_cmd_ptr->cmd.rrc_abort_req.delay_sig_con_release = FALSE;
      }
      else
      {
        rrc_cmd_ptr->cmd.rrc_abort_req.delay_sig_con_release = TRUE;
      }
      rrc_cmd_ptr->cmd.rrc_abort_req.auth_failed_bar_cells = FALSE;
      rrc_cmd_ptr->cmd.rrc_abort_req.hard_abort = FALSE;
  
      rrc_cmd_ptr->cmd.rrc_abort_req.abort_cause = RRC_OTHERS;
        send_cmd_to_rrc( rrc_cmd_ptr, mm_serving_plmn.info.active_rat );
    }
    else if (is_gsm_mode())
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= MM sent RR_ABORT_REQ");
      mm_send_rr_abort_req(FALSE);
    }
  }

  /* ------------------------------------------------------------------------
  ** Store the MMR_REG_REQ command for processing after ABORT_CNF is received
  ** ------------------------------------------------------------------------ */
  mmcoord_pend_mmr_reg_req(message);
#if defined(FEATURE_LTE)
  /* Save the reg req for EMM */
  if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
  {
    ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
    memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type),
        &message->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
  }
#endif
}


/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMR_REG_REQ

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_reg_req( mm_cmd_type *message )
{

  boolean is_hard_abort_req = FALSE;

#if defined(FEATURE_LTE)
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);
#endif

  if (message->cmd.mmr_reg_req.message_header.message_id == (int) MMR_REG_REQ)
  {
    MSG_HIGH_DS_3( MM_SUB, "=MM= MM received MMR_REG_REQ [%x %x %x]",
       message->cmd.mmr_reg_req.plmn.identity[0],
       message->cmd.mmr_reg_req.plmn.identity[1],
       message->cmd.mmr_reg_req.plmn.identity[2]);
    mm_reg_req_trans_id = message->cmd.mmr_reg_req.trans_id;
/*change made so that reg don't send reg req on gsm after split,while ps on gsm not completed
  in merged condition*/
#ifdef FEATURE_SGLTE
    if (message->cmd.mmr_reg_req.type !=MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY)
#endif
      {
        mm_reg_waiting_for_reg_cnf = TRUE;
      }

    /* Clear any previous NREG pending flag as a new Service is requested */
    mm_nreg_req_received    = FALSE;
    mmcoord_reg_req_pending = FALSE;
    mm_reject_call_on_rai_change = FALSE;
    /*Clear acquisition status variable as we are starting new search*/
#if defined FEATURE_DUAL_SIM || defined FEATURE_1XSRLTE
    mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif
#ifdef FEATURE_LTE
    mm_stop_mode_local_detach_peding = FALSE;
    if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
    {
      if(emm_bit_mask_check(message->cmd.mmr_reg_req.additional_info,
                  (byte)SYS_ADDTNL_INFO_FORCE_RADIO_CAP_UPDATE) == TRUE)
      {
        emm_set_radio_cap_update_state(EMM_RADIO_CAP_UPDATE_PENDING);
        /*
        ** Reset the force capability update flag of additional info so that if REG request is pended
        ** NAS will not think the force radio capability request is sent twice.
        */
        emm_bit_mask_reset(&message->cmd.mmr_reg_req.additional_info,
                               (byte)SYS_ADDTNL_INFO_FORCE_RADIO_CAP_UPDATE);
      }
    }
#endif
    mm_interrat_state = MM_INTERRAT_CHANGE_STATE_TYPE_LO;
#ifdef FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER
	if (WtoGCCO_state == CCO_FAILED)
		{
		  MSG_HIGH_DS_0( MM_SUB, "=MM= Resetting the WtoGCCO_state to CCO_INACTIVE ");
		  WtoGCCO_state = CCO_INACTIVE;
		}
#ifdef FEATURE_INTERRAT_PCCO_GTOW
		else if (GtoWCCO_state == CCO_FAILED )
		{
		  MSG_HIGH_DS_0( MM_SUB, "=MM= Resetting the GtoWPCCO_state to CCO_INACTIVE ");
		  GtoWCCO_state = CCO_INACTIVE;
		}
#endif
#endif
    
    /* If PLMN LIST REQ is pending abort/ignore it */
    mm_plmn_list_req_pending = FALSE;

   /* Cell service indication is to be sent always after mmr_reg_cnf is MM loopback
   If PLMN select request is sent to RRC or RR than this will be reset*/
   mm_mmr_cell_service_ind_pending = TRUE;
   mm_hard_abort_pending           = FALSE;
   mm_hard_abort_deep_sleep_pending = FALSE;
   mm_deep_sleep_pending           = FALSE;
   mm_pended_stop_mode_req.valid  = FALSE;

#if defined (FEATURE_DUAL_SIM) && defined (FEATURE_TDSCDMA)
   if(mm_check_ignore_mmr_reg_req(&message->cmd.mmr_reg_req.rat_pri_list_info) == TRUE)
   {
     return;
   }
#endif

    /*
    ** Pending REG_REQ if MM is waiting for STOP_MODE_CNF
    */
    if ((mm_waiting_for_stop_mode_cnf) 
         || (mm_rat_handler_status == MM_RAT_STOP_GSM_IN_PROGRESS) 
         || (mm_rat_handler_status == MM_RAT_STOP_WCDMA_IN_PROGRESS)
         || (mm_rat_handler_status == MM_RAT_STOP_TDSCDMA_IN_PROGRESS)
       )  
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= Pending MMR_REG_REQ - MM Waiting for STOP MODE CNF from AS or RAT change in Progress");
#if defined(FEATURE_LTE)
      /* Save the reg req for EMM */
     if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
     {
       ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
       memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type),
        &message->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
#if defined(FEATURE_DUAL_DATA)
       emm_ctrl_data_ptr->reg_req_pending_ptr->req_service_domain = 
            mm_get_effective_req_srv_domain(message->cmd.mmr_reg_req.req_service_domain,
            emm_ctrl_data_ptr->reg_req_pending_ptr->volte_only, TRUE);
#endif
     } 
#endif
      mm_ue_pref_rat = message->cmd.mmr_reg_req.rat;
      mmcoord_pend_mmr_reg_req(message);
      /*reset mm_rat_change_pending so that the pending reg_req will be handled afresh after
        receiving stop_mode_cnf from lower layers */
#ifdef FEATURE_SGLTE
      /* For NON SGLTE case it should behave as legacy for SGLTE case only
           in case of upper layer splitting it should not reset the flag  */
      if ((!MM_SUB_IS_SGLTE) ||
         (mm_sglte_current_stack_status != SGLTE_MM_UPPER_LAYER_SPLITING ))
#endif
      {
        mm_rat_change_pending = FALSE;
      }
      return;
    }
#ifdef FEATURE_DUAL_SIM
    else if (
#ifdef FEATURE_DUAL_DATA
             (mm_max_active_data == 1) &&
#endif
             (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) && 
             (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS ||
              message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_PS_ONLY))
    {
      sys_modem_as_id_e_type      mm_other_as_id;
      for(mm_other_as_id = SYS_MODEM_AS_ID_1; mm_other_as_id < MAX_NAS_STACKS ; mm_other_as_id++)
      {
        if ((mm_other_as_id != (sys_modem_as_id_e_type)mm_as_id) && (gmm_state_sim[mm_other_as_id] != GMM_NULL) &&
          (gmm_state_sim[mm_other_as_id] != GMM_DEREGISTERED))
        {
          MSG_HIGH_DS_1(MM_SUB, "=MM= DSDS: Pending MMR_REG_REQ - MM Waiting for Detach completion on ds%d",
             mm_other_as_id+1);

          gmm_force_local_detach_sim[mm_other_as_id] = TRUE;
          mm_ue_pref_rat = message->cmd.mmr_reg_req.rat;
          mmcoord_pend_mmr_reg_req(message);
          return;
        }
      }
    }
#endif
#if defined(FEATURE_LTE) && defined(FEATURE_LTE_REL9)
    if((mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
        &&
       (emm_ctrl_data_ptr->emm_3gpp_spec_ver >= LTE_3GPP_REL9) &&
       (message->cmd.mmr_reg_req.lte_cs_domain_param.is_voice_domain_ie_valid == TRUE) &&
       ((message->cmd.mmr_reg_req.lte_cs_domain_param.voice_domain_pref == 
                                           SYS_VOICE_DOMAIN_PREF_IMS_PS_VOICE_PREFERRED) ||
        (message->cmd.mmr_reg_req.lte_cs_domain_param.voice_domain_pref == 
                                           SYS_VOICE_DOMAIN_PREF_CS_VOICE_PREFERRED)))
    {
      MSG_HIGH_DS_1(MM_SUB ,"=MM= Setting PS voice on EUTRAN supported bit to TRUE voice_domain_pref = %d",
           message->cmd.mmr_reg_req.lte_cs_domain_param.voice_domain_pref);
      emm_ctrl_data_ptr->mmit_parameters.eutran_ps_voice_parameters.is_ps_voice_on_eutran_supported = TRUE;
    }
#endif
    
    /* If deep sleep alt scan is true, start all paused timers */
    if(mm_deep_sleep_alt_scan)
    {
      byte i;
      MSG_HIGH_DS_0(MM_SUB, "=MM= DEEP_SLEEP ALT SCAN. Resume all running timers"); 
      for(i=0;i<MAX_MM_TIMER+1;i++)
      {
        if (mm_timer_status[i] == TIMER_PAUSED)
        {
          if (gs_modify_timer_ext( get_mm_timer_queue(),
                                   i, 
                                   TIMER_MODIFY_RESUME, 
                                   mm_timer_non_deferrable(i)) != GS_SUCCESS)
          {
            MSG_ERROR_DS( MM_SUB, "=MM= Failed to resume timer %d", i, 0, 0);
          }
          else
          {
            MSG_HIGH_DS_1(MM_SUB ,"=MM= Resuming %d", i);
            mm_timer_status[ i ] = TIMER_ACTIVE;
          }  
        }    
      }

      mm_deep_sleep_alt_scan = FALSE;
    }
    /* -------------------------------------------------------
    ** Set the default values for pending MMR_REG_CNF response
    ** ------------------------------------------------------- */
    mmcoord_mmr_reg_cnf.available_plmn_list.length  = 0;
    mmcoord_mmr_reg_cnf.update_equivalent_plmn_list = FALSE;
    memset(&mmcoord_mmr_reg_cnf.equivalent_plmn_list, 0x00, sizeof(sys_plmn_list_s_type));
    mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;

    if(mm_managed_roaming_enabled)
    {
      if((message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS ||
          message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_ONLY))
      {
       mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_LIMITED_REGIONAL;
      }
      else
      {
        mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_NO_SRV;
      }
    }
#ifdef FEATURE_ENHANCED_NW_SELECTION
    if(reg_sim_read_ens_flag())
    {
      if((message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS ||
          message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_ONLY))
      {
       mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_LIMITED_REGIONAL;
      }
      else
      {
        mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_NO_SRV;
      }
    }
#endif

    if((message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ||
       (message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL))
    {
      message->cmd.mmr_reg_req.req_service_domain = mm_sim_allowed_service_domain(message->cmd.mmr_reg_req.req_service_domain);
    }

    /* Set MM and GMM flags to correct value if network selection mode is Manual */
    if(message->cmd.mmr_reg_req.type == MMR_REG_REQ_USER_SELECTION)
    {
	if(!(
#ifdef FEATURE_DUAL_SIM 
           (reg_sim_per_subs_int_voice_forbidden( message->cmd.mmr_reg_req.plmn,(sys_modem_as_id_e_type)mm_sub_id )) ||
           (reg_sim_per_subs_domestic_voice_forbidden( message->cmd.mmr_reg_req.plmn,(sys_modem_as_id_e_type)mm_sub_id ))
#else
           (reg_sim_int_voice_forbidden( message->cmd.mmr_reg_req.plmn )) || 
           (reg_sim_domestic_voice_forbidden( message->cmd.mmr_reg_req.plmn ))
#endif
	  ))
      {
        mm_manual_mode_user_selection_procedure = TRUE;
		mm_user_selection = TRUE;
        if (!(reg_nv_read_data_roaming(
#ifdef FEATURE_DUAL_SIM
              (sys_modem_as_id_e_type)mm_sub_id
#endif         
            ) == SYS_DATA_ROAMING_INTERNATIONAL_OFF))
        {
          gmm_manual_mode_user_selection_procedure = TRUE;
        }
      }
      gmm_delete_gprs_forbidden_rai();
#ifdef FEATURE_LTE
      if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
      {
        mm_set_emm_manual_selection_flag(TRUE);
      }
#endif
    }

    if(message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_NO_SRV)
    {
      MSG_ERROR_DS( MM_SUB, "=MM= Invalid req_service_domain %d",
        message->cmd.mmr_reg_req.req_service_domain, 0,0 );
      mmcoord_reject_mmr_reg_req();  
    }
    else if((mm_disable_g_upon_fallback == TRUE) &&
            (reg_state_get_mcs_pri() == TRUE) &&
            (message->cmd.mmr_reg_req.rat == SYS_RAT_GSM_RADIO_ACCESS) &&
            (cm_get_operator_info((sys_modem_as_id_e_type)mm_sub_id) == OPERATOR_CU) &&
            (MM_IS_NON_DDS_SUB) &&
            (mm_timer_status[GSM_EXTENDED_BLOCK_TIMER] == TIMER_ACTIVE))
    {
        /*loop back no svc*/
        MSG_HIGH_DS_0(MM_SUB,"Looping back no svc on GSM while block timer is runninf in fallback");
        mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
         mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;
         mmcoord_mmr_reg_cnf.available_plmn_list.length = 0;
        mmcoord_mmr_reg_cnf.service_state.service_status = SYS_SRV_STATUS_NO_SRV;
        mmcoord_mmr_reg_cnf.service_state.acq_status= SYS_ACQ_STATUS_SUCCESS;
        mm_send_mmr_reg_cnf();
        mm_reg_waiting_for_reg_cnf       = FALSE;
        mm_ready_to_send_reg_cnf         = FALSE;

    }
    else
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(mm_wcdma_rlf_state == WCDMA_RLF_STARTED)
    {
      mm_ready_to_send_reg_cnf =   FALSE; /* This flag is readied for the new REG_REQ*/
      mm_ue_pref_rat = message->cmd.mmr_reg_req.rat;
      
      /* -----------------------------------------------------------------------
      ** Store the command. UE needs to access this command after a RAT change
      ** or initiate new search when LU rejected due to NO_SUITABLE_CELLS_IN_LA
      ** ----------------------------------------------------------------------*/
      mmcoord_pending_reg_message.cmd.mmr_reg_req = message->cmd.mmr_reg_req;
#if defined(FEATURE_LTE)
      /* Save the reg req for EMM */
     if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
     {
       ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
       memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type),
          &message->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
#if defined(FEATURE_DUAL_DATA)
       emm_ctrl_data_ptr->reg_req_pending_ptr->req_service_domain = 
            mm_get_effective_req_srv_domain(message->cmd.mmr_reg_req.req_service_domain,
                    emm_ctrl_data_ptr->reg_req_pending_ptr->volte_only, TRUE);
#endif 
     }
#endif
      /* ---------------------------------------------------------
      ** Check to see if REG is requesting the CS_FOR be set to 1
      ** ---------------------------------------------------------*/
      if (((message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_ONLY) ||
            (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS))
           &&
           (message->cmd.mmr_reg_req.cs_for_request))
      {
        mm_follow_on_request = TRUE;
        MSG_HIGH_DS_0(MM_SUB, "=MM= REG requested CS_FOR = 1");
      }

      if ((message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_ONLY) &&
         ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) ||
          (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)))
      {
        MSG_HIGH_DS_0(MM_SUB, "=MM= Perform local detach due to RLF state ");
        gmm_handle_local_gprs_detach();
        if(is_lte_mode())
        {
          emm_multimode_detach();
        }
        if (rrc_connection_status != MMRRC_IDLE)
        {
          mm_send_rrc_abort_req_during_wrlf();
          is_hard_abort_req = TRUE;
        }
      }
	  /*In Limited mode, or when coming out of it, UE should not abort RLF due to pref change or rat change*/
	  else if((mm_rat_pri_list_match(&message->cmd.mmr_reg_req.rat_pri_list_info,
                                                      &mm_rat_pri_list_info) == FALSE)
              && (message->cmd.mmr_reg_req.network_selection_mode != SYS_NETWORK_SELECTION_MODE_LIMITED)
              && (mm_last_requested_network_selection_mode != SYS_NETWORK_SELECTION_MODE_LIMITED))
      {
        MSG_HIGH_DS_0(MM_SUB, "=MM= RAT priority list change in RLF state ");
        if (rrc_connection_status != MMRRC_IDLE)
        {
          mm_send_rrc_abort_req_during_wrlf();
          is_hard_abort_req = TRUE;
        }
      }
      else if(((message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS) ||
               (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_PS_ONLY)) &&
              ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) ||
               (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)))
      {
        MSG_HIGH_DS_0(MM_SUB, "=MM= service domain change in RLF state ");

        if (message->cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_PS_ONLY)
        {
          if (rrc_connection_status != MMRRC_IDLE)
          {
            mm_send_rrc_abort_req_during_wrlf();
            is_hard_abort_req = TRUE;
          }
          mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                            SYS_SRV_DOMAIN_NO_SRV,
                                            FALSE,
                                            SYS_SRV_STATUS_LIMITED,
                                            TRUE ); 
        }

        if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)
        {
          gmm_set_state( GMM_DEREGISTERED );
        }
      }

      mm_serving_plmn.ms_op_mode = message->cmd.mmr_reg_req.req_service_domain;

      mm_wrlf_rat_change_pending = mm_rat_change_requested(message->cmd.mmr_reg_req.rat);

      if ((rrc_connection_status == MMRRC_RELEASE_IN_PROGRESS) && 
          (is_hard_abort_req == TRUE))
      {
        mmcoord_reg_req_pending    = TRUE;
        mm_wrlf_rat_change_pending = FALSE;
      }
      else
#ifdef FEATURE_SGLTE
      if (MM_IS_IN_SGLTE_MODE &&(message->cmd.mmr_reg_req.type == MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY)&&
          (mm_wrlf_rat_change_pending == TRUE))
      {
        mm_wrlf_rat_change_pending = FALSE;
        mmcoord_pend_mmr_reg_req(message);
        mm_stop_rat_mode();
      }
      else
#endif

      if(mm_wrlf_rat_change_pending == TRUE)
      {
        mmcoord_pend_mmr_reg_req(message);
        mm_stop_rat_mode();
      }
      else
      {
        mm_send_service_req( message,message->cmd.mmr_reg_req.rat);
      }
    }
    else
#endif
#ifdef FEATURE_SGLTE
  if(MM_IS_IN_SGLTE_STATE_4 && 
     (message->cmd.mmr_reg_req.is_split_needed == TRUE))                  
  {
    mm_if_needed_abort_plmn_search_and_loop_back_to_reg( message->cmd.mmr_reg_req.type);
    MSG_HIGH_DS_0(MM_SUB,"=MM= MMR_REG_REQ_SGLTE_PS_ONLY_ACQUISITION  start split  ");
    mm_sglte_current_stack_status = SGLTE_MM_UPPER_LAYER_SPLITING;
    mm_copy_cs_database(MM_SGLTE_STACK_2, mm_sub_id);      
    mm_send_rr_stop_mode_req(RR_SGLTE_USER_RAT_CHANGE);
    mmsend_mmr_stack_act_ind();
    mm_rat_change_pending  = TRUE;
    ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
    memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type),
        &message->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
    mmcoord_pend_mmr_reg_req(message);
  }    
  else
#endif
    if (rrc_connection_status != MMRRC_IDLE)
    {
     if(((message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) ||
         (message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)) &&
         (!mm_rat_change_requested(message->cmd.mmr_reg_req.rat)) &&

         (mm_rat_pri_list_match(&message->cmd.mmr_reg_req.rat_pri_list_info,
                                                  &mm_rat_pri_list_info) == TRUE) &&
         (!mm_service_change_requested( message->cmd.mmr_reg_req.req_service_domain ))&&
         (mm_already_camped( message)) )
            
     {         
         mmcoord_process_mmr_reg_req_now( message ); 
     }
     else if((mm_service_change_requested( message->cmd.mmr_reg_req.req_service_domain ) )&&
             (!mm_rat_change_requested(message->cmd.mmr_reg_req.rat)) &&

             (mm_rat_pri_list_match(&message->cmd.mmr_reg_req.rat_pri_list_info,
                                                      &mm_rat_pri_list_info) == TRUE) &&

             (mm_already_camped( message)) )
     {
        /* ---------------------------------------------------------
        ** Process change-of-service commands including PS-on-demand
        ** --------------------------------------------------------- */
        switch (message->cmd.mmr_reg_req.req_service_domain)
        {
           case SYS_SRV_DOMAIN_CS_ONLY:
             if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
             {
               if(is_gsm_mode() &&
                  ((mm_state == MM_WAIT_FOR_RR_CONNECTION_LU) ||
                   (mm_state == MM_LOCATION_UPDATE_INITIATED)))
               {
                 mmcoord_abort_rr_connection_then_process_mmr_reg_req( message );
               }
               else
               {
                 mmcoord_process_mmr_reg_req_now( message );
               }
             }
             else /* (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY) */
             {
               mmcoord_abort_rr_connection_then_process_mmr_reg_req( message );
             }
             break;

           case SYS_SRV_DOMAIN_PS_ONLY:
             if (mm_state != MM_IDLE || !mm_already_camped( message))
             {
               mmcoord_abort_rr_connection_then_process_mmr_reg_req( message );
             }
             else
             {
               mmcoord_process_mmr_reg_req_now( message );
             }
             break;

           case SYS_SRV_DOMAIN_CS_PS:
             if((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)&&
                (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)&&
                ((gmm_state == GMM_REGISTERED_INITIATED) || 
                 (gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED)
                )
               )  
             {
              
               mmcoord_abort_rr_connection_then_process_mmr_reg_req( message );
               
             }
             else if((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)&&
                (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1 ||
                (is_gsm_mode() && mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_2))&&
                ((mm_state == MM_WAIT_FOR_RR_CONNECTION_LU) || 
                 (mm_state == MM_LOCATION_UPDATE_INITIATED)
                )
               )  
             {
               mmcoord_abort_rr_connection_then_process_mmr_reg_req( message );
             }
             else
             {
               /* ------------
               ** PS on demand
               ** ------------ */ 
               if(message->cmd.mmr_reg_req.ps_for_request)
               {
                 mm_send_mmr_camped_ind(FALSE);
               }
               mmcoord_process_mmr_reg_req_now( message );
             }
             break;

           default:
             MSG_ERROR_DS( MM_SUB, "=MM= Invalid req_service_domain %d",
                  message->cmd.mmr_reg_req.req_service_domain, 0,0 );
             mmcoord_reject_mmr_reg_req();
             break;
        }
      }
      else
      {
        if ( (mm_in_conn_management_state() == TRUE)

             ||
           (
#ifdef FEATURE_DUAL_SIM
             (sm_is_pdp_active((sys_modem_as_id_e_type)mm_sub_id)) &&
#else
             (sm_is_pdp_active()) &&
#endif
             (mm_serving_plmn.info.service_status == SYS_SRV_STATUS_SRV) &&
             (((reg_nv_is_wcdma_freq_lock_enabled()) && 
               (message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED)) ||
              ((message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC ||
              message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL) &&
             (!mm_rat_change_requested( message->cmd.mmr_reg_req.rat )) &&
             (!mm_service_change_requested( message->cmd.mmr_reg_req.req_service_domain )) &&
             (mm_checkif_rat_pri_list_superset(&message->cmd.mmr_reg_req.rat_pri_list_info,
                                                                              &mm_rat_pri_list_info)) 
#ifdef FEATURE_LTE
              && 
             (!mm_checkif_rat_added_to_rat_pri_list(&message->cmd.mmr_reg_req.rat_pri_list_info,
                                                      &mm_rat_pri_list_info,SYS_RAT_LTE_RADIO_ACCESS))
#endif  
              && 
               (mm_already_camped( message)))))


           )
        {
                    
#ifdef FEATURE_ENHANCED_NW_SELECTION
          if (((reg_sim_read_ens_flag())&&(mmsim_is_ef_rat_present())) &&
              (message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC) &&
              (!mm_service_change_requested( message->cmd.mmr_reg_req.req_service_domain ))&&

#ifdef FEATURE_LTE
              (message->cmd.mmr_reg_req.type == MMR_REG_REQ_EF_RAT_REFRESH)
#else
              (!mm_rat_pri_list_gw_match(&message->cmd.mmr_reg_req.rat_pri_list_info,
                                        &mm_rat_pri_list_info))
#endif
               &&

              (mm_already_camped( message)))
          
          {
          
            gmm_foreground_search_pending = FALSE;
            
            if ((mm_state != MM_IDLE) &&
                (mm_state != MM_WAIT_FOR_RR_CONNECTION_LU) &&
                (mm_state != MM_LOCATION_UPDATE_INITIATED))
            {
               MSG_HIGH_DS_0(MM_SUB,"=MM= Pending MMR_REG_REQ - MM Waiting for connection release before ENS_refresh");
#if defined(FEATURE_LTE)
               /* Save the reg req for EMM */
              if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
              {
                 ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
                 memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type),
                                       &message->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
              }   
#endif
               mmcoord_pend_mmr_reg_req(message);
            }
            else
            {
               /*release the conn by aborting the PS call as in this case we have to send an LU to the network.
                  In any case if we are moving from Dual->single we have to abort the PS call even now. And in 
                  case of Single->dual we are letting the PS call now but that's b'coz we had no obligation 
                  to send an LU. Now, since LU is required we don't want to hold on to the PS call.*/
               mmcoord_abort_rr_connection_then_process_mmr_reg_req( message );
            }
          }
          else
#endif          
          if((reg_nv_is_wcdma_freq_lock_enabled()) &&
             (message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
          {
             MSG_HIGH_DS_0(MM_SUB,"=MM= WCDMA FREQ LOCK - Reject the MMR REG REQ");
             mm_send_mmr_camped_ind(FALSE);
             mmcoord_reject_mmr_reg_req();
          }
          /*Timer T3240 will be running when MM state is #9.Even if network is not releasing the connection, 
          Connection will be released and UE moves to IDLE state at the expiry of the timer T3240.So pend the MMR_REG_REQ
          when UE is in state MM state #9 and process it after the connection release*/
          else if((mm_state == MM_WAIT_FOR_NETWORK_COMMAND) && 
                       ((((mm_serving_plmn.info.active_rat == SYS_RAT_GSM_RADIO_ACCESS) &&
                          (mm_last_paging_cause != (byte)CS_PAGING)) ||
                        ((mm_serving_plmn.info.active_rat == SYS_RAT_TDS_RADIO_ACCESS) &&
                         (mm_last_paging_cause == 0xFF)) ||
                        ((mm_serving_plmn.info.active_rat == SYS_RAT_UMTS_RADIO_ACCESS) &&
                         (mm_last_paging_cause == 0xFF))) && 
                         (is_ps_call_active_while_srv_status_SRV(message) == FALSE)
                  ))
          {
            MSG_HIGH_DS_0(MM_SUB,"=MM= Pending MMR_REG_REQ -Will be processed after the connection release ");
#if defined(FEATURE_LTE)
            /* Save the reg req for EMM */
            if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
            {
              ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
              memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type),
                                    &message->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
            }    
#endif
            mmcoord_pend_mmr_reg_req(message);
            mm_serv_change_pending = FALSE;
            
            if  ( (mm_rat_change_requested(message->cmd.mmr_reg_req.rat)) ||

                  (mm_rat_pri_list_match(&message->cmd.mmr_reg_req.rat_pri_list_info,
                                                       &mm_rat_pri_list_info) == FALSE)||

                  (!mm_already_camped( message)) )
             {
                gmm_foreground_search_pending = FALSE;
             }
          }
          else
          {
            /*
            ** If UE mode is changed from WCDMA Only to WCDMA preferred,
            ** inform the RRC about mode/band changes using new primitive.
            ** New band should be the super set of current band.
            */

            if((!mm_rat_change_requested( message->cmd.mmr_reg_req.rat )) &&
               (!mm_service_change_requested( message->cmd.mmr_reg_req.req_service_domain )) &&
               (mm_checkif_rat_pri_list_superset(&message->cmd.mmr_reg_req.rat_pri_list_info,
                &mm_rat_pri_list_info)) &&
               (mm_already_camped( message)))
            {
              if (is_gsm_mode())
              {
                mm_send_rr_change_mode_ind(message);
              }
#ifdef FEATURE_PREFERRED_MODE_CHANGE
              else
              {
                mm_send_rrc_change_mode_ind(message);
              }
#endif /* FEATURE_PREFERRED_MODE_CHANGE */
            }



            if((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode== SYS_NETWORK_SELECTION_MODE_AUTOMATIC &&
                 message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL) ||
               (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode== SYS_NETWORK_SELECTION_MODE_MANUAL &&
                 message->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC))
            {
              mm_send_nw_sel_mode_reset_ind(message->cmd.mmr_reg_req.network_selection_mode, message->cmd.hdr.message_id);
            } 
            
            MSG_HIGH_DS_0(MM_SUB, "=MM= Reject REG_REQ - CS call or PS Call in-progress");
            mm_reg_req_revd_active_call = TRUE;
            if((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                                                        SYS_NETWORK_SELECTION_MODE_MANUAL) ||
               (mm_plmn_match(mm_serving_plmn.info.plmn, message->cmd.mmr_reg_req.plmn )))
            {
              mmcoord_reject_mmr_reg_req();
            }
            else
            {
              mm_reg_req_trans_id--;
            }
          }

        }
        else
        {
        
          gmm_foreground_search_pending = FALSE;
          
          mmcoord_abort_rr_connection_then_process_mmr_reg_req( message );
        }
      }
    } /* end if ((rrc_connection_established) || (rrc_conn_est_in_progress)) */
    else
    {
      mmcoord_process_mmr_reg_req_now( message );
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG msg %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end of mmcoord_process_mmr_reg_req() */

void mmcoord_process_mmr_gstk_service_ind(mm_cmd_type *message)
{
  if(message->cmd.mmr_gstk_service_ind.service_available == FALSE)
  {
    mm_send_gstk_no_service_info();
  }
  else if(message->cmd.mmr_gstk_service_ind.service_available == TRUE)
  {
    mm_gstk_set_location_status_evt(TRUE);
    mm_send_gstk_mm_idle_info();
    mm_gstk_set_location_status_evt(FALSE);
  }
    
}


/*===========================================================================

FUNCTION   : MMCOORD_PROCESS_LU_PROCEDURE

DESCRIPTION : This function initiates the location update procedure.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_lu_procedure ( mm_cmd_type *message )
{
  boolean already_camped;
  boolean mm_reg_waiting_for_reg_cnf_local;
#if defined(FEATURE_LTE)
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
#endif
#if defined(FEATURE_LTE) && defined(FEATURE_NAS_RAU_ON_MS_RAC_CHANGE)
  reg_sim_plmn_timer_info_s_type plmn_timer_info;
  plmn_timer_info.plmn = message->cmd.mmr_reg_req.plmn;
  plmn_timer_info.timer_count = 0;
  plmn_timer_info.cause = SYS_BLOCK_PLMN_CAUSE_NONE;
#endif

#if defined(FEATURE_LTE)
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);

#endif

  /* ------------------------------------------------------------------
  ** Check if we are already camped on the selected PLMN. No need to
  ** perform this check if the Network Selection mode is LIMITED_SERVICE,
  ** In limited service, the Access stratum camps on any Cell.
  ** ---------------------------------------------------------------- */
  if ((message->cmd.mmr_reg_req.network_selection_mode ==
         SYS_NETWORK_SELECTION_MODE_LIMITED) ||
      (message->cmd.mmr_reg_req.network_selection_mode ==
         SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION) ||

      (mm_rat_pri_list_match(&message->cmd.mmr_reg_req.rat_pri_list_info,
                             &mm_rat_pri_list_info) == FALSE) ||

      ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY) &&
       (mm_serving_plmn.gmm_service_state == SERVICE_NOT_AVAILABLE)) ||
      ((message->cmd.mmr_reg_req.rat == SYS_RAT_GSM_RADIO_ACCESS) &&
       (mm_serv_change_pending)))
  {
    already_camped = FALSE;
  }
  else
  {
    already_camped = mm_already_camped( message );
  }

#ifdef FEATURE_ENHANCED_NW_SELECTION    
  if (reg_sim_read_ens_flag() && ((mmsim_is_ef_rat_present()) ||
     (gmm_nv_disable_call_type & GMM_CALL_TYPE_VOICE_MASK))) 
  {

#ifdef FEATURE_LTE
    if(message->cmd.mmr_reg_req.type == MMR_REG_REQ_EF_RAT_REFRESH)
#else
    if (mm_check_if_rat_pri_list_contains_gw(&mm_rat_pri_list_info) !=
       mm_check_if_rat_pri_list_contains_gw(&message->cmd.mmr_reg_req.rat_pri_list_info))
#endif/* FEATURE_LTE */

    {
      mm_force_lu = TRUE;
      gmm_force_registration = TRUE;
    }
    else
    {
      mm_force_lu = FALSE;
      gmm_force_registration = FALSE;
    }
  }
#endif

#ifdef FEATURE_NAS_RAU_ON_MS_RAC_CHANGE
  /* ---------------------------------------------------------------------
  ** 3GPP spec 24.008 4.7.5.1: In A/Gb mode, the normal routing area updating procedure is initiated 
  ** when the MS needs to update the network with the new MS Radio Access Capability IE 
  ** ------------------------------------------------------------------- */
  if (

      (mm_rat_pri_list_match(&message->cmd.mmr_reg_req.rat_pri_list_info,
                             &mm_rat_pri_list_info_to_nw) == FALSE) &&

      (sys_plmn_id_is_undefined(message->cmd.mmr_reg_req.plmn) == FALSE)&&
#ifdef FEATURE_LTE
      (reg_sim_plmn_rat_forbidden(message->cmd.mmr_reg_req.plmn,SYS_RAT_LTE_RADIO_ACCESS,
                                           (sys_modem_as_id_e_type)mm_sub_id) == FALSE) &&
      (reg_mode_check_eplmn_backoff_forbidden(&plmn_timer_info,
                                           (sys_modem_as_id_e_type)mm_sub_id) == FALSE) &&
#endif
      (gmm_state == GMM_REGISTERED)&&
      (gmm_update_status == GMM_GU1_UPDATED))
  {

    MSG_HIGH_DS_0(MM_SUB, "=MM= Pending RAU as MS RAC has changed rat priority list ");

    gmm_ra_update_pending = TRUE;
	gmm_pending_ra_update_type = gmm_ra_update_type;
  }
#endif

  if (((mm_rat_change_pending) || (mm_serv_change_pending) || 
      (mm_rat_pri_list_match(&message->cmd.mmr_reg_req.rat_pri_list_info, &mm_rat_pri_list_info) == FALSE)) &&
      (mm_timer_status[TIMER_MM_RADIO_RETRY] == TIMER_ACTIVE))
  {
    mm_stop_timer(TIMER_MM_RADIO_RETRY);

    if (mm_lu_start_reason == T3212_EXPIRED || 
        mm_lu_start_reason == LTE_IRAT_LU)
    {
      mm_pending_lu_request = mm_lu_start_reason;
    }   
    mm_substate_control(MM_STD_UPDATE_PENDING);
    mm_previous_idle_substate = NULL;
  }

  /* ---------------------------------------------------------------------
  ** Initiate location update procedure if we are already camped
  ** on the requested PLMN.
  ** ------------------------------------------------------------------- */
  if (already_camped)
  {
    boolean lu_required = FALSE, attach_required = FALSE;
    /* If only network selection mode is changing, update RR/RRC */
    if(mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL ||
       mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)
    {
      mm_send_nw_sel_mode_reset_ind(mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode, mmcoord_pending_reg_message.cmd.hdr.message_id);
    }
    
    mm_send_mmr_camped_ind(FALSE);
    /*
      ** Check for access class blocking restrictions and stop a LU procedure if necessary
      */
    lu_required = mm_check_if_cs_cell_access_allowed(mm_get_cs_cell_access(PROCEDURE_CS_REGISTRATION));
    attach_required = gmm_check_if_ps_cell_access_allowed(gmm_get_ps_cell_access(PROCEDURE_PS_REGISTRATION));     

    if (!attach_required)
    { 
      /*****************************************************************************
      *Reset this flag to false as we are not trying PS registration and if PS fop
      *is to be set,SM will be in SIG_CONNECTION_PENDING state.It will take care 
      *of setting PS fop to true
      *****************************************************************************/ 
      
      gmm_follow_on_request = FALSE;

    }
    
    if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
    {
      mm_ready_to_send_reg_cnf = TRUE;
      gmm_handle_plmn_found( mm_serving_plmn, attach_required);
    }
    else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)
    {
      mm_handle_plmn_found( mm_serving_plmn,
                            lu_required );
    }
    else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
    {
      if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
      {
        mm_ready_to_send_reg_cnf = TRUE;
        gmm_handle_plmn_found( mm_serving_plmn,
                               attach_required );
      }
      else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
      {
        mm_handle_plmn_found( mm_serving_plmn,
                              lu_required );
        gmm_handle_plmn_found( mm_serving_plmn,
                               attach_required );
      }
    }
    else
    {
      MSG_ERROR_DS( MM_SUB, "=MM= Invalid MS op mode", 0,0,0 );
      /* Send rejection back to REG? */
    }
    /* ---------------------------------------------------------------
      ** Sending MM registration status to REG if no MM/GMM procedure is 
      ** triggered because of access restrictions or because it is already in appropriate 
      ** state.
      ** --------------------------------------------------------------- */    
    if (mm_reg_waiting_for_reg_cnf && 
        mm_state == MM_IDLE && 
        (gmm_state == GMM_DEREGISTERED || gmm_state == GMM_REGISTERED || 
         gmm_state == GMM_NULL))
    {
      mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_FAILURE_REMAIN_ON_PLMN;
      mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
      if(mm_mmr_attach_ind_status == ATTACH_FAILED_IND_REQUIRED && attach_required)
      {
        mm_mmr_attach_ind_status = ATTACH_FAILED_IND_WAITING_FOR_GMM;
      }
      mm_send_mmr_reg_cnf();
      if(mm_mmr_attach_ind_status != ATTACH_FAILED_IND_NOT_REQUIRED)
      {
        mm_mmr_attach_ind_status = ATTACH_FAILED_IND_REQUIRED;
      }

      mm_reg_waiting_for_reg_cnf = FALSE;
      mm_ready_to_send_reg_cnf   = FALSE;      
    }    
  }
  else
  {
    /* ---------------------------------
    ** Store ALL parameters from command
    ** --------------------------------- */
    mmcoord_pending_reg_message.cmd.mmr_reg_req = message->cmd.mmr_reg_req;
#if defined(FEATURE_LTE)
    /* Save the reg req for EMM */
    if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
    { 
      ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
      memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr,sizeof(mmr_reg_req_s_type),
                             &message->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
#if defined(FEATURE_DUAL_DATA)
      emm_ctrl_data_ptr->reg_req_pending_ptr->req_service_domain = 
              mm_get_effective_req_srv_domain(message->cmd.mmr_reg_req.req_service_domain, 
                emm_ctrl_data_ptr->reg_req_pending_ptr->volte_only, TRUE);
#endif 
    }
#endif

    if (is_gsm_mode() && gmm_state == GMM_REGISTERED_INITIATED)
    {
      if (
#ifdef FEATURE_NAS_FREE_FLOAT
          (gmmllc_get_glif_grr_service_req_state() == GLIF_SERVICE_CONFIRMED)
#else      
          (gmmllc_get_glif_grr_camp_state() == GLIF_CAMP_ON_PCCCH_CONFIRMED)
#endif
        )
      {
        mm_reg_waiting_for_reg_cnf_local = mm_reg_waiting_for_reg_cnf;
        mm_reg_waiting_for_reg_cnf = FALSE;
        gmm_increment_gprs_attach_attempt_counter();
        mm_reg_waiting_for_reg_cnf = mm_reg_waiting_for_reg_cnf_local;
      }
      else if(    
#ifdef FEATURE_NAS_FREE_FLOAT
                     (gmmllc_get_glif_grr_service_req_state() != GLIF_SERVICE_CONFIRMED)
#else      
                     (gmmllc_get_glif_grr_camp_state() != GLIF_CAMP_ON_PCCCH_CONFIRMED)
#endif
              )
      {
        mm_reg_waiting_for_reg_cnf_local = mm_reg_waiting_for_reg_cnf;
        mm_reg_waiting_for_reg_cnf = FALSE;
        gmm_set_state(GMM_DEREGISTERED);
        mm_stop_timer(TIMER_T3310);
        t3310_timeout_counter = 0;
        mm_reg_waiting_for_reg_cnf = mm_reg_waiting_for_reg_cnf_local;
      }
    }
    else if(is_gsm_mode() && gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED)
    {
#ifdef FEATURE_GPRS_PS_HANDOVER
      if (gmm_psho_status == GMM_PSHO_IN_PROGRESS )
      {
        mm_reg_waiting_for_reg_cnf_local =mm_reg_waiting_for_reg_cnf;
        mm_reg_waiting_for_reg_cnf = FALSE;
        gmm_increment_routing_area_update_attempt_counter (
#ifdef FEATURE_GPRS_LAYER3_TX_CONFIRM
                      FALSE
#endif
                                                                    );
        mm_reg_waiting_for_reg_cnf = mm_reg_waiting_for_reg_cnf_local;
      }
      else
#endif
      {      
        if(
#ifdef FEATURE_NAS_FREE_FLOAT
           (gmmllc_get_glif_grr_service_req_state() == GLIF_SERVICE_CONFIRMED)&&
#else
           (gmmllc_get_glif_grr_camp_state() == GLIF_CAMP_ON_PCCCH_CONFIRMED) &&
#endif
           (gmmllc_get_glif_llc_conn_state() == LLC_SUSPENSION_CONFIRMED)
          )
        {
          mm_reg_waiting_for_reg_cnf_local =mm_reg_waiting_for_reg_cnf;
          mm_reg_waiting_for_reg_cnf = FALSE;
          gmm_increment_routing_area_update_attempt_counter (
#ifdef FEATURE_GPRS_LAYER3_TX_CONFIRM
            FALSE
#endif
                                                          );
          mm_reg_waiting_for_reg_cnf = mm_reg_waiting_for_reg_cnf_local;
        }
        else if(
#ifdef FEATURE_NAS_FREE_FLOAT
                 (gmmllc_get_glif_grr_service_req_state() != GLIF_SERVICE_CONFIRMED)
#else
                 (gmmllc_get_glif_grr_camp_state() != GLIF_CAMP_ON_PCCCH_CONFIRMED)
#endif
                  || (gmmllc_get_glif_llc_conn_state() != LLC_SUSPENSION_CONFIRMED)
               )
        {
          mm_reg_waiting_for_reg_cnf_local =mm_reg_waiting_for_reg_cnf;
          mm_reg_waiting_for_reg_cnf = FALSE;
          gmm_set_state(GMM_REGISTERED);
          mm_stop_timer(TIMER_T3330);
          t3330_timeout_counter = 0;
          mm_reg_waiting_for_reg_cnf = mm_reg_waiting_for_reg_cnf_local;
        }
      }
    }
    mm_substate_control( MM_STD_PLMN_SEARCH );

    if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY ||
        mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
    {
       gmm_set_substate(GMM_PLMN_SEARCH);
    }
    
    /* ------------------------
    ** Send SERVICE_REQ command
    ** ------------------------ */
    if
    (    
     !(mm_checkif_rat_pri_list_has_rat( &message->cmd.mmr_reg_req.rat_pri_list_info ,get_preferred_mode()))

    )
    {
      MSG_FATAL_DS(MM_SUB, "=MM= reg requested rat (%d) and MM requesting RAT are not same (%d)", 
                                           message->cmd.mmr_reg_req.rat,get_preferred_mode(),0);
    }   
    if((mm_waiting_for_service_cnf == TRUE) &&
       (message->cmd.mmr_reg_req.rat == mm_serving_plmn.info.active_rat) &&
       (message->cmd.mmr_reg_req.rat == SYS_RAT_UMTS_RADIO_ACCESS) &&
       (mm_last_sent_serv_req_params_ptr != NULL))
    {
      rrc_mm_service_req_cause_e_type service_req_cause = mm_get_serv_req_cause(message);
      boolean is_ehplmn_to_be_camped = mm_get_is_ehplmn_to_be_camped();
      if((memcmp(&(mm_last_sent_serv_req_params_ptr->plmn), &(message->cmd.mmr_reg_req.plmn), sizeof(sys_plmn_id_s_type)) == 0) &&
          (message->cmd.mmr_reg_req.csg_id == mm_last_sent_serv_req_params_ptr->csg_id) &&

          (mm_rat_pri_list_match(&message->cmd.mmr_reg_req.rat_pri_list_info,
                                 &mm_rat_pri_list_info) == TRUE) &&

          (message->cmd.mmr_reg_req.network_selection_mode == mm_last_requested_network_selection_mode) &&
          (message->cmd.mmr_reg_req.grat_scan_done == mm_last_sent_serv_req_params_ptr->grat_scan_done) &&
          (message->cmd.mmr_reg_req.scan_scope == mm_last_sent_serv_req_params_ptr->scan_scope) &&
          (message->cmd.mmr_reg_req.trm_timeout == mm_last_sent_serv_req_params_ptr->trm_timeout_value) &&
          (is_ehplmn_to_be_camped == mm_last_sent_serv_req_params_ptr->is_ehplmn_to_be_camped)
        )
      {
        if(mm_is_serv_req_cause_changed_leading_opt_plmn_srch( service_req_cause ))
        {
          mm_send_service_req( message,
                                 get_preferred_mode() );
        }
        else
        {
          MSG_HIGH_DS_0(MM_SUB,"=MM= No need to send another rrc_service_req as it will be treated as replica of previous one");
        }
      }    
      else
      {
        mm_send_service_req( message,
                              get_preferred_mode() );
      }
    }
    else
    {      
      mm_send_service_req( message,
                             get_preferred_mode() );
    }  
  }
} /* end mmcoord_process_lu_procedure() */

/*===========================================================================
  
  FUNCTION mm_emm_clear_dos_counters
  
  DESCRIPTION 
	This function resets DoS specific counters to 0
  
  DEPENDENCIES
	None
  
  RETURN VALUE
	None
  
  SIDE EFFECTS
	None
  
===========================================================================*/
void mm_emm_clear_dos_counters(void)
{
	 MSG_HIGH_DS_0(MM_SUB, "=MM= mm_emm_clear_dos_counters()");
	 mm_emm_usim_gprs_counter = 0;
	 mm_emm_usim_non_gprs_counter = 0;
	 if (mm_emm_max_attempt_counter_dos_mitigation > 0)
	 {
#ifdef FEATURE_DUAL_SIM
	   reg_sim_reset_dos_fplmn_list((sys_modem_as_id_e_type)mm_sub_id);
#else
	   reg_sim_reset_dos_fplmn_list();
#endif
	 }
	 if(mm_timer_status[TIMER_T3247] == TIMER_ACTIVE)
	 {
	   mm_stop_timer(TIMER_T3247);
	 }
	 mm_is_sim_ps_invalid_due_to_dos = FALSE;
	 mm_is_sim_cs_invalid_due_to_dos = FALSE;
	 mm_is_sim_to_be_cs_invalid_after_power_off = FALSE;
	 mm_is_sim_to_be_ps_invalid_after_power_off = FALSE;
	 MSG_HIGH_DS_2(MM_SUB, "=MM= mm_emm_usim_gprs_counter= %d,mm_emm_usim_non_gprs_counter =%d", mm_emm_usim_gprs_counter, mm_emm_usim_non_gprs_counter);
}

#ifdef FEATURE_DUAL_SIM
void mm_emm_clear_dos_counters_per_subs(mm_as_id_e_type mm_local_as_id)
{
	 MSG_HIGH_DS_0(MM_SUB, "=MM= mm_emm_clear_dos_counters_per_subs() called");
	 mm_emm_usim_gprs_counter_sim[mm_local_as_id] = 0;
	 mm_emm_usim_non_gprs_counter_sim[mm_local_as_id] = 0;
	 if (mm_emm_max_attempt_counter_dos_mitigation_sim[mm_local_as_id]> 0)
	 {
	   reg_sim_reset_dos_fplmn_list((sys_modem_as_id_e_type)mm_local_as_id);
	 }
	 if(mm_timer_status_sim[mm_local_as_id][TIMER_T3247] == TIMER_ACTIVE)
	 {
	   mm_stop_timer_per_subs(TIMER_T3247, mm_local_as_id);
	 }
	 mm_is_sim_ps_invalid_due_to_dos_sim[mm_local_as_id] = FALSE;
	 mm_is_sim_cs_invalid_due_to_dos_sim[mm_local_as_id] = FALSE;
	 mm_is_sim_to_be_cs_invalid_after_power_off_sim[mm_local_as_id] = FALSE;
	 mm_is_sim_to_be_ps_invalid_after_power_off_sim[mm_local_as_id] = FALSE;
	 MSG_HIGH_DS_2(MM_SUB, "=MM= mm_emm_usim_gprs_counter= %d,mm_emm_usim_non_gprs_counter =%d", mm_emm_usim_gprs_counter_sim[mm_local_as_id], mm_emm_usim_non_gprs_counter_sim[mm_local_as_id]);
}
#endif


/*===========================================================================

FUNCTION MMCOORD_PROCESS_MMR_STOP_MODE_REQ

DESCRIPTION

DEPENDENCIES
  Depends on the REG task to send the DEEP_SLEEP and MODE_CHANGE reason
  in the appropriate state.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_stop_mode_req( mm_cmd_type *message )
{

  rrc_cmd_type *rrc_cmd_p = NULL;
  sys_stop_mode_reason_e_type stop_mode_reason;
  mmr_service_ind_s_type     mmr_service_ind;
#ifdef FEATURE_DUAL_SIM
  mm_as_id_e_type  mm_local_as_id, mm_tmp_as_id;
#endif
#if defined(FEATURE_LTE)
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);
#endif
  stop_mode_reason = SYS_STOP_MODE_REASON_NONE;
  
  mm_send_reg_attach_failed_ind_if_needed();
#if defined(FEATURE_LTE)
  gmm_reset_mcs_client_info();
#endif

  if (((mm_waiting_for_stop_mode_cnf) &&
      ((message->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_DEEP_SLEEP) ||
       (message->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_ALT_SCAN) ||
       (message->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH)
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE     
       ||(message->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_SUSPEND_STACK)
       ||(message->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_UE_MODE_CHANGE)
#endif
       ))
#ifdef FEATURE_SGLTE        
       ||((mm_sglte_current_stack_status == SGLTE_MM_UPPER_LAYER_SPLITING )||
           (mm_sglte_current_stack_status == SGLTE_MM_LOWER_LAYER_SPLITING )) 
#endif          
       || (mm_rat_handler_status == MM_RAT_STOP_GSM_IN_PROGRESS) 
       || (mm_rat_handler_status == MM_RAT_STOP_WCDMA_IN_PROGRESS)
       || (mm_rat_handler_status == MM_RAT_STOP_TDSCDMA_IN_PROGRESS)
       )
  {
    MSG_HIGH_DS_0(MM_SUB,"=MM= Pending MMR_STOP_MODE_REQ - MM Waiting for STOP CNF from AS or SGLTE split in progress");
    mm_pended_stop_mode_req.mmr_stop_mode_req= message->cmd.mmr_stop_mode_req;
    mm_pended_stop_mode_req.valid = TRUE;
    mmcoord_reg_req_pending = FALSE ;
    mm_deep_sleep_pending = FALSE;
    return;
  }
  mmr_stop_mode_trans_id = message->cmd.mmr_stop_mode_req.trans_id;
  mm_pended_stop_mode_req.valid = FALSE;
  /* --------------------------------------------------------------
  ** Stop Mode request overrides any pending RAT/Service change procedures
  ** -------------------------------------------------------------- */
  mm_rat_change_pending = FALSE;

  gmm_foreground_search_pending = FALSE;

  mm_reg_req_revd_active_call = FALSE;

  if (mm_serv_change_pending)
  {
    mm_serv_change_pending = FALSE;

    /* The ms_op_mode should be reverted back to Reg service domain as the
    ** Service pending is set to false and the UE is about to be powered off */
    if(mm_serving_plmn.info.reg_service_domain != SYS_SRV_DOMAIN_NO_SRV)
    {
      mm_serving_plmn.ms_op_mode = mm_serving_plmn.info.reg_service_domain;
    }
    if (((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) || 
         (mm_serving_plmn.info.reg_service_domain == SYS_SRV_DOMAIN_NO_SRV)) &&
        (gmm_state != GMM_NULL) &&
        (message->cmd.mmr_stop_mode_req.stop_mode_reason != SYS_STOP_MODE_REASON_DEEP_SLEEP))
    {
      gmm_gprs_attach_pending = FALSE;
      gmm_set_state( GMM_DEREGISTERED );
    }
  }

  if (message->cmd.hdr.message_id == (int) MMR_STOP_MODE_REQ)
  {
#ifdef FEATURE_GSM_DTM
    mm_serving_plmn.dtm_supported = FALSE;
#endif
    mm_waiting_for_mocall_connect = FALSE;
    mm_serving_plmn.egprs_supported = FALSE;
    mm_serving_plmn.hsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
    mm_reject_call_on_rai_change = FALSE;
    stop_mode_reason = message->cmd.mmr_stop_mode_req.stop_mode_reason;
    MSG_HIGH_DS_1(MM_SUB ,"=MM= MM received MMR_STOP_MODE_REQ, type=%d", stop_mode_reason);

    /* MMR_STOP_MODE_REQUEST must stop any pended MMR_REG_REQ processing */
    mmcoord_reg_req_pending = FALSE ;
    mm_waiting_for_service_cnf = FALSE;
    mm_reg_waiting_for_reg_cnf = FALSE;
    mm_ready_to_send_reg_cnf   = FALSE;
    mm_plmn_list_req_pending = FALSE;	

/*sglte logic for the pending case*/

    /* -------------------------------------------------------------------
    ** If STOP MODE Request is received and there is no Active RAT or the
    ** Mode reason is not valid, send back the STOP_MODE_CNF back to REG.
    ** -----------------------------------------------------------------*/
    if (mm_serving_plmn.info.active_rat == SYS_RAT_NONE)
    {
      MSG_ERROR_DS(MM_SUB, "=MM= STOP Mode req, Invalid request",0,0,0);

      MSG_HIGH_DS_3( MM_SUB, "=MM= RAT = %d, reason = %d, Service_state = %d",
        mm_serving_plmn.info.active_rat, stop_mode_reason,
        mm_serving_plmn.mm_service_state);

      /* Send STOP MODE CNF to REG */
      mm_send_mmr_stop_mode_cnf();
    }
#ifdef FEATURE_SGLTE
    else if((MM_SUB_IS_SGLTE) && 
            (MM_IS_IN_SGLTE_MODE) &&
            (MM_IS_SGLTE_ON_CS_STACK_ID) && 
            ((mm_sglte_current_stack_status ==SGLTE_MM_LOWER_LAYER_MERGING)||
            (mm_sglte_current_stack_status ==SGLTE_MM_UPPER_LAYER_MERGING)))
    {
       
       MSG_HIGH_DS_0(MM_SUB, "=MM= Looping back stop mode cnf as it was on hybrid stack during merge in sglte ");
       mm_send_mmr_stop_mode_cnf();
    }
#endif
    else
    {
      byte null_lai[LAI_SIZE] = { 0xff, 0xff, 0xff, 0, 0 };

      switch (stop_mode_reason)
      {

#ifdef FEATURE_DUAL_SIM
        case SYS_STOP_MODE_REASON_DUAL_SWITCH:
        case SYS_STOP_MODE_REASON_DSDS_SUSPEND:                
#endif
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE
        case SYS_STOP_MODE_REASON_UE_MODE_CHANGE:
        case SYS_STOP_MODE_REASON_SUSPEND_STACK:
#endif
        case SYS_STOP_MODE_REASON_ALT_SCAN:
     
        case SYS_STOP_MODE_REASON_DEEP_SLEEP:
     
          MSG_HIGH_DS_0(MM_SUB, "=MM= DEEP SLEEP/ALT SCAN/DUAL/SGLTE/SUSPEND_STACK MODE SWITCH/DSDS SUSPEND");
          /* If connected mode, then send hard abort while not in WRLF,
             else proceed with deep sleep handling */
#ifdef FEATURE_DUAL_SIM		  
		  if(mm_timer_status[TIMER_MM_DUAL_SWITCH] == TIMER_ACTIVE)
		  {
		    mm_stop_timer(TIMER_MM_DUAL_SWITCH);
		  }
#endif             
          if(rrc_connection_status != MMRRC_IDLE
#ifdef FEATURE_WRLF_SYSTEM_SEL
             &&((!(stop_mode_reason == SYS_STOP_MODE_REASON_ALT_SCAN || 
                   stop_mode_reason == SYS_STOP_MODE_REASON_DEEP_SLEEP)) ||
                (mm_wcdma_rlf_state == WCDMA_RLF_IDLE))
#endif
             )
          {
#ifdef FEATURE_DUAL_SIM          
            if((stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH) &&
                (mm_state == MM_WAIT_FOR_RR_CONNECTION_LU || 
                  mm_state == MM_LOCATION_UPDATE_INITIATED ||
                 (is_umts_mode() && (gmm_state == GMM_REGISTERED_INITIATED || (sm_check_pdp_procedure_pending((sys_modem_as_id_e_type)mm_sub_id))))) && 
                 (!mm_dual_switch_timer_started)
               )
            {
              if (is_umts_mode() && (sm_check_pdp_procedure_pending((sys_modem_as_id_e_type)mm_sub_id)))
              {
                mm_start_timer( TIMER_MM_DUAL_SWITCH, 5000 );
              }
              else
              {
                mm_start_timer( TIMER_MM_DUAL_SWITCH, DEFAULT_TIMEOUT );
              }
              mm_dual_switch_timer_started = TRUE;
              MSG_HIGH_DS_0(MM_SUB, "=MM= Pending MMR_STOP_MODE_REQ - DUALSWITCH received during registarton/PDP Signalling is Ongoing");
              mmcoord_pending_reg_message.cmd.mmr_stop_mode_req= message->cmd.mmr_stop_mode_req;
              break;
            }
#endif          
            if(rrc_connection_status != MMRRC_RELEASE_IN_PROGRESS || (rrc_connection_status == MMRRC_RELEASE_IN_PROGRESS && !MMCNM_IS_MODE_DUAL_STANDBY))
            {
              if(mm_cnm_is_active)
              {
                 mm_send_mmcnm_rel_ind(OTHER_CAUSE,FALSE);
              }
              if(is_umts_mode())
              {            
                  
  #ifdef FEATURE_WCDMA
                  rrc_cmd_p = mm_get_rrc_cmd_buf( mm_serving_plmn.info.active_rat );
  #endif /* FEATURE_WDMA */
  
                if (rrc_cmd_p == NULL)
                {
                  MSG_FATAL_DS( MM_SUB, "=MM= ERROR: Unable to allocate memory", 0,0,0 );
                }               
                rrc_cmd_p->cmd_hdr.cmd_id = RRC_ABORT_REQ;
                rrc_cmd_p->cmd.rrc_abort_req.auth_failed_bar_cells = FALSE;
                rrc_cmd_p->cmd.rrc_abort_req.delay_sig_con_release = FALSE;
  #ifdef FEATURE_DUAL_SIM
                if(
					stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH || stop_mode_reason == SYS_STOP_MODE_REASON_DSDS_SUSPEND ||
                    ((stop_mode_reason == SYS_STOP_MODE_REASON_ALT_SCAN || stop_mode_reason == SYS_STOP_MODE_REASON_DEEP_SLEEP) && MMCNM_IS_MODE_DUAL_STANDBY)
				  )
                {
                  rrc_cmd_p->cmd.rrc_abort_req.hard_abort = FALSE;
                }
                else
  #endif
                {
                  rrc_cmd_p->cmd.rrc_abort_req.hard_abort = TRUE;
                }
  #ifdef FEATURE_DUAL_SIM
                if( stop_mode_reason == SYS_STOP_MODE_REASON_DSDS_SUSPEND)
                {
                  rrc_cmd_p->cmd.rrc_abort_req.abort_cause = RRC_DUAL_STANDBY_PS_ABORT;
                }
                else
  #endif
                {
                rrc_cmd_p->cmd.rrc_abort_req.abort_cause = RRC_OTHERS;
                }
                  send_cmd_to_rrc( rrc_cmd_p, mm_serving_plmn.info.active_rat );
              }
              else
              {
                MSG_HIGH_DS_0(MM_SUB, "=MM= MM sent RR_ABORT_REQ");
                mm_send_rr_abort_req(FALSE);
              }
            }
            mm_dual_switch_timer_started = FALSE;
            mm_previous_idle_substate = NULL;
            mm_stop_timer( TIMER_T3210);
            mm_state_control( MM_STD_RR_CONNECTION_ABORTED );
            /* Mark mm_deep_sleep_pending as TRUE */
#ifdef FEATURE_DUAL_SIM
            if(
				stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH || stop_mode_reason == SYS_STOP_MODE_REASON_DSDS_SUSPEND ||
                ((stop_mode_reason == SYS_STOP_MODE_REASON_ALT_SCAN || stop_mode_reason == SYS_STOP_MODE_REASON_DEEP_SLEEP) && MMCNM_IS_MODE_DUAL_STANDBY)
			  )
            {
              mm_abort_dual_switch_pending = stop_mode_reason;
              if(is_umts_mode())
              {
                mm_start_timer(TIMER_MM_REL,3000);
              }
            }
            else
#endif
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE
            if(stop_mode_reason == SYS_STOP_MODE_REASON_UE_MODE_CHANGE)
            {
              mm_hard_abort_ue_sglte_mode_switch_pending = TRUE;
            }
            else
#endif
            if(stop_mode_reason == SYS_STOP_MODE_REASON_DEEP_SLEEP)
            {
              mm_hard_abort_deep_sleep_pending = TRUE;
            }
            else
            {
              mm_hard_abort_pending = TRUE;
            }
          }
#ifdef FEATURE_DUAL_SIM
          else if (is_gsm_mode() &&
                   (stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH) &&
                   (sm_check_pdp_procedure_pending((sys_modem_as_id_e_type)mm_sub_id)) &&
                   (!mm_dual_switch_timer_started))
          {
            mm_start_timer( TIMER_MM_DUAL_SWITCH, 5000 );
            mm_dual_switch_timer_started = TRUE;
            MSG_HIGH_DS_0(MM_SUB, "=MM= Pending MMR_STOP_MODE_REQ - DUALSWITCH received during PDP procedure is Ongoing");
            mmcoord_pending_reg_message.cmd.mmr_stop_mode_req= message->cmd.mmr_stop_mode_req;
          }
          else if(stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH)
          {
            mmcoord_handle_deep_sleep_dual_switch();
          }
          else if( stop_mode_reason == SYS_STOP_MODE_REASON_DSDS_SUSPEND)
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= DSDS Suspend. Pause all running timers (except periodic timers)"); 
            mmcoord_handle_deep_sleep_alt_scan();
          }
#endif
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE
          else if(stop_mode_reason == SYS_STOP_MODE_REASON_UE_MODE_CHANGE)
          {
            mmcoord_handle_ue_sglte_mode_switch();
          }
#endif
          else if(stop_mode_reason == SYS_STOP_MODE_REASON_DEEP_SLEEP)
          {
            mmcoord_handle_deep_sleep();
          }
          else /* SYS_STOP_MODE_REASON_ALT_SCAN */
          {
            mmcoord_handle_deep_sleep_alt_scan();    
          }
          break;

        case SYS_STOP_MODE_REASON_POWER_OFF:
        case SYS_STOP_MODE_REASON_OFFLINE:
        case SYS_STOP_MODE_REASON_IRAT:
        case SYS_STOP_MODE_REASON_MODE_CHANGE:
#ifdef FEATURE_DUAL_SIM
        case SYS_STOP_MODE_REASON_MODE_CAP_CHANGE:
#endif
        case SYS_STOP_MODE_REASON_LPM:
#ifdef FEATURE_LTE
        case SYS_STOP_MODE_REASON_LOCAL_DETACH:
#endif        
          if( (stop_mode_reason == SYS_STOP_MODE_REASON_LPM)||
              (stop_mode_reason == SYS_STOP_MODE_REASON_POWER_OFF)
            )
          {
            if((stop_mode_reason == SYS_STOP_MODE_REASON_LPM) && 
               ((mm_is_sim_cs_invalid_due_to_dos || mm_is_sim_ps_invalid_due_to_dos) &&
		 	    ((mm_serving_plmn.info.sim_state == SYS_SIM_STATE_NOT_AVAILABLE) ||
				 (mm_serving_plmn.info.sim_state == SYS_SIM_STATE_CS_PS_INVALID))))
            {
				mmgsdi_session_manage_illegal_subscription(mm_sim_mmgsdi_info.session_id,
														   MMGSDI_SESSION_APP_IS_LEGAL);

            }
            mm_emm_clear_dos_counters();
            mm_is_sim_to_be_cs_invalid_after_power_off = FALSE; 
            mm_is_sim_to_be_ps_invalid_after_power_off = FALSE;
            MSG_HIGH_DS_0(MM_SUB,"=MM= resetting mm_is_sim_to_be_cs/ps_invalid during power off/lpm"); 
#ifdef FEATURE_DUAL_SIM
            mm_tmp_as_id = mm_as_id;
            for( mm_local_as_id = MM_AS_ID_1; mm_local_as_id < (mm_as_id_e_type)MAX_NAS_STACKS ; mm_local_as_id++ )
            {
                 mm_set_as_id(mm_local_as_id);
                 if (mm_serving_plmn.info.sim_state == SYS_SIM_STATE_NOT_AVAILABLE)
                 {
                    if (mm_illegal_me_sub == ILLEGAL_ME_ON_CURRENT_SUB)
                    {
                       mm_send_mmr_me_blocked_status_ind(FALSE, mm_local_as_id);
                    }
                    mm_illegal_me_sub = ILLEGAL_ME_SUB_NO_CAUSE;
                 }
            }
            mm_set_as_id(mm_tmp_as_id);

            MSG_HIGH_DS_1(MM_SUB,"=MM= mm_illegal_me_sub cause = %d  mm_illegal_me_sub cause reset during power off/lpm", mm_illegal_me_sub);
#endif
          }
#ifdef FEATURE_LTE
          if( emm_ctrl_data_ptr->l2gcco_state == EMM_LTOG_CCO_IN_PROGRESS
#ifdef FEATURE_SGLTE
           && MM_IS_SGLTE_ON_PS_STACK_ID
#endif
            )
          {
            MSG_HIGH_DS_0(MM_SUB,"=MM= Received Stop mode during CCO, aborting CCO procedure and perform local detach");
            emm_ctrl_data_ptr->l2gcco_state = EMM_LTOG_CCO_ABORTED;
          }

          if(mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ)
          {
            mm_ps_detach_state = MM_PS_DETACH_STATE_NONE;
            mm_ps_detach_type  = SYS_PS_DETACH_TYPE_NONE;
          }

          gmm_foreground_search_pending = FALSE;

          if (mm_sim_card_mode == MMGSDI_APP_UNKNOWN && mm_deep_sleep_pending == TRUE)
          {
            mm_serving_plmn.mm_service_state = SERVICE_NOT_AVAILABLE;
            mm_serving_plmn.gmm_service_state = SERVICE_NOT_AVAILABLE;
          }

/*Reset flags at powerdown.*/         
#ifdef FEATURE_NAS_REL10
#ifdef FEATURE_LTE
          mm_is_lu_to_be_initiated_t3346_ltogw = FALSE;
#endif
#endif
#ifdef FEATURE_NAS_REL11 
          gmm_is_lw_to_g_interrat_in_progress = FALSE;
#endif
#ifdef FEATURE_LTE
          if((stop_mode_reason == SYS_STOP_MODE_REASON_OFFLINE)||
             (stop_mode_reason == SYS_STOP_MODE_REASON_MODE_CHANGE) ||
#ifdef FEATURE_DUAL_SIM
             (stop_mode_reason == SYS_STOP_MODE_REASON_MODE_CAP_CHANGE) ||
#endif
             (stop_mode_reason == SYS_STOP_MODE_REASON_LPM))
          {
            emm_reset_attempt_counter();
          }
#endif

          if(((stop_mode_reason == SYS_STOP_MODE_REASON_LOCAL_DETACH) ||
              ((emm_ctrl_data_ptr->l2gcco_state == EMM_LTOG_CCO_ABORTED)
#ifdef FEATURE_SGLTE
               && MM_IS_SGLTE_ON_PS_STACK_ID
#endif
               ))&&((rrc_connection_status == MMRRC_ACTIVE) || (rrc_connection_status == MMRRC_EST_IN_PROGRESS)))
          {
            /* Pend the stop mode request and abort the connection */
            mm_stop_mode_local_detach_peding = TRUE;
            mmcoord_pending_reg_message.cmd.mmr_stop_mode_req = message->cmd.mmr_stop_mode_req;
            mm_hard_abort_pending = FALSE;
            mm_deep_sleep_alt_scan = FALSE;
            mm_deep_sleep_pending = FALSE;
            mm_hard_abort_deep_sleep_pending = FALSE;
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE
            mm_hard_abort_ue_sglte_mode_switch_pending = FALSE;
#endif
            if (is_gsm_mode())
            {
               mm_abort_rr( RRC_CS_DOMAIN_CN_ID, FALSE );
            }
            else if (is_umts_mode())
            {
#ifdef FEATURE_WCDMA
               rrc_cmd_p = mm_get_rrc_cmd_buf( mm_serving_plmn.info.active_rat );
#endif
               if (rrc_cmd_p == NULL)
               {
                  MSG_FATAL_DS( MM_SUB, "=MM= ERROR: Unable to allocate memory", 0,0,0 );
               }               
               rrc_cmd_p->cmd_hdr.cmd_id = RRC_ABORT_REQ;
               rrc_cmd_p->cmd.rrc_abort_req.auth_failed_bar_cells = FALSE;
               rrc_cmd_p->cmd.rrc_abort_req.delay_sig_con_release = FALSE;
               rrc_cmd_p->cmd.rrc_abort_req.hard_abort = TRUE;
               rrc_cmd_p->cmd.rrc_abort_req.abort_cause = RRC_OTHERS;
               send_cmd_to_rrc( rrc_cmd_p, mm_serving_plmn.info.active_rat );
            }
            mm_state_control( MM_STD_RR_CONNECTION_ABORTED );
          }
          else
#endif
          {
       
            sys_modem_as_id_e_type local_as_id = SYS_MODEM_AS_ID_NONE;  

#ifdef FEATURE_DUAL_SIM
            local_as_id = (sys_modem_as_id_e_type)mm_sub_id;
#endif

            /* --------------------------------------------------------------
            ** If STOP Mode is due to above reasons while deep sleep is pending,
            ** this STOP Mode takes precedence. No STOP Mode Cnf is sent to
            ** REG/CM for deep sleep. If this is not stopped, upper layer will
            ** mistake stop mode cnf for deep sleep to be confirmation for 
            ** power off.
            ** -------------------------------------------------------------- */
            mm_stop_mode_req_pended = FALSE;    
            mm_deep_sleep_pending = FALSE;

#ifdef FEATURE_LTE
            mm_stop_mode_local_detach_peding = FALSE;
#endif
            if(!(mm_timer_status[ TIMER_MM_REL ] == TIMER_ACTIVE))
            {
              if(mm_waiting_for_deact_cnf == TRUE)
              {
                MSG_HIGH_DS_0(MM_SUB,"=MM= Do not start TIMER_MM_REL as we are already waiting for deact cnf from lower layer "); 
              }
              else
              {
                mm_start_timer( TIMER_MM_REL, DEFAULT_TIMEOUT ); 
              }
            }
            mm_hard_abort_pending = FALSE;
            mm_deep_sleep_alt_scan = FALSE;
            mm_hard_abort_deep_sleep_pending = FALSE;
#ifdef FEATURE_DUAL_SIM
            mm_abort_dual_switch_pending = SYS_STOP_MODE_REASON_NONE;;
#endif
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE
            mm_hard_abort_ue_sglte_mode_switch_pending = FALSE;
#endif
            /* If RCR is in progress at RRC or TDSRRC,
             *  i.e. - If API rrcrcr_is_proc_active_sub or tdsrrcrcr_is_proc_active_sub return TRUE,
             *  then don't process STOP_MODE_REQ just yet. Wait for RRC to send RRC_REL_IND.
             */
            if((mm_waiting_for_abort_cnf == TRUE)
#ifdef FEATURE_WCDMA
                   || ((mm_serving_plmn.info.active_rat == SYS_RAT_UMTS_RADIO_ACCESS) && (rrc_connection_status != MMRRC_IDLE) && (rrcrcr_is_proc_active_sub(local_as_id) == TRUE))
#endif
#ifdef FEATURE_TDSCDMA   
                   || ((mm_serving_plmn.info.active_rat == SYS_RAT_TDS_RADIO_ACCESS) && (rrc_connection_status != MMRRC_IDLE) && (tdsrrcrcr_is_proc_active_sub(local_as_id) == TRUE))
#endif
              )
            {
              /*copy the stop_mode_req;*/
              MSG_HIGH_DS_1(MM_SUB,"=MM= Pending MMR_STOP_MODE_REQ - MM Waiting for ABORT CNF from AS, mm_waiting_for_abort_cnf = %d",mm_waiting_for_abort_cnf);
              mmcoord_pending_reg_message.cmd.mmr_stop_mode_req= message->cmd.mmr_stop_mode_req;
              mm_stop_mode_req_pended = TRUE;
              return;
              
            }
            mm_previous_idle_substate = NULL;            

            (void)memscpy( mm_gstk_previous_serving_lai, LAI_SIZE,
                          null_lai,
                          LAI_SIZE );

            /* --------------------------------------------------------------
            ** The STOP Mode reason is due to Poweroff or Offline,
            ** Route MMR_STOP_MODE_REQ to MM and/or GMM based on MS operation mode
            ** -------------------------------------------------------------- */
#if defined(FEATURE_LTE)            
            /*Case scenario when UE got SYS_STOP_MODE_REASON_IRAT 
               and LTE have camped to full service at once */
            if((stop_mode_reason==SYS_STOP_MODE_REASON_IRAT) && 
               (rrc_connection_status == MMRRC_IDLE)&&
               (mm_service_loss_on_multimode_rat == SYS_RAT_LTE_RADIO_ACCESS) && 
               (gmm_state==GMM_REGISTERED)) 
            {
              mm_handle_message(message);
              EMM_MOVES_TO_REGISTERED_STATE(EMM_REGISTERED_NO_CELL_AVAILABLE);
            }
            else
#endif
            if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
            {
              gmm_handle_message( message );
            }
            else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)
            {
#if defined(FEATURE_LTE) && defined(FEATURE_DUAL_DATA)
              if ((gmm_state != GMM_NULL) &&
                  (emm_ctrl_data_ptr->reg_req_pending_ptr != NULL) &&
                  (emm_ctrl_data_ptr->reg_req_pending_ptr->volte_only == TRUE))
              {
                gmm_detach_type =GMM_GPRS_DETACH;
                gmm_handle_gprs_detach_low_level_failure();
                mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,
                   PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
              }
#endif
              mm_handle_message( message );
            }
            else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
            {
              if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
              {
                gmm_handle_message( message );
              }
              else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
              {
                mm_handle_message( message );
                gmm_handle_message( message );
              }
            }
            else
            {
              mm_handle_message ( message );
            }
          }
          break;
        case SYS_STOP_MODE_REASON_LOCAL_CS_PS_DETACH: //FR 33195 
        {
	        mmcoord_reg_req_pending = FALSE;
	        mm_reg_waiting_for_reg_cnf = FALSE;
	        mm_serv_change_pending = FALSE;  
			if(gmm_state != NULL)
			{
	        gmm_detach_type =GMM_GPRS_DETACH;
	        gmm_handle_gprs_detach_low_level_failure();
            mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
			}
			mm_substate_control( MM_STD_LIMITED_SERVICE );
            mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                            SYS_SRV_DOMAIN_NO_SRV,
                                            FALSE,
                                            SYS_SRV_STATUS_LIMITED,
                                            TRUE );

            mmsend_nas_reg_event_packet(PROC_NAS_IMSI_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
            mm_stop_timer( TIMER_T3211 );
            mm_stop_timer( TIMER_T3212 );
            mm_stop_timer( TIMER_T3213 );
#ifdef FEATURE_DUAL_SIM
            mm_stop_timer( TIMER_MM_RADIO_RETRY );
#endif

            mm_stop_timer( TIMER_T3220 );
            mm_initiate_lu_upon_entry_into_new_cell = FALSE;
            MSG_HIGH_DS_0(MM_SUB, "=MM= Sending MMR_SERVIC_IND " );
            mmr_service_ind.cause = MMR_CAUSE_NOT_SPECIFIED;
            mmr_service_ind.service_state = mm_serving_plmn.info;
            mm_send_mmr_service_ind( &mmr_service_ind );
            if((rrc_connection_status == MMRRC_ACTIVE) || (rrc_connection_status == MMRRC_EST_IN_PROGRESS))
            {
              if (is_gsm_mode())
              {
                 mm_abort_rr( RRC_CS_DOMAIN_CN_ID, FALSE );
              }
              else if (is_umts_mode())
              {
#ifdef FEATURE_WCDMA
                rrc_cmd_p = mm_get_rrc_cmd_buf( mm_serving_plmn.info.active_rat );
#endif
	           if (rrc_cmd_p == NULL)
		       {
		         MSG_FATAL_DS( MM_SUB, "=MM= ERROR: Unable to allocate memory", 0,0,0 );
		       }               
		       rrc_cmd_p->cmd_hdr.cmd_id = RRC_ABORT_REQ;
		       rrc_cmd_p->cmd.rrc_abort_req.auth_failed_bar_cells = FALSE;
		       rrc_cmd_p->cmd.rrc_abort_req.delay_sig_con_release = FALSE;
		       rrc_cmd_p->cmd.rrc_abort_req.hard_abort = TRUE;
		       rrc_cmd_p->cmd.rrc_abort_req.abort_cause = RRC_OTHERS;
		       send_cmd_to_rrc( rrc_cmd_p, mm_serving_plmn.info.active_rat );
	         }
	       }
	  }
	 break;
        default:
          MSG_ERROR_DS(MM_SUB, "=MM= Wrong STOP MODE reason = %d",stop_mode_reason,0,0);
        break;
      }
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG msg %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_mmr_stop_mode_req() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_EST_CNF

DESCRIPTION
  Function to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_est_cnf( mm_cmd_type *message )
{
  if (message->cmd.rrc_est_cnf.message_header.message_id == (int) RRC_EST_CNF)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_EST_CNF");

    /* ------------------------------------------------------------------
    ** Pass RRC_EST_CNF to both MM & GMM independent of MS operation mode
    ** ------------------------------------------------------------------ */
    mm_handle_message( message );
    gmm_handle_message( message );
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_est_cnf() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_REL_IND

DESCRIPTION
  Function to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_rel_ind( mm_cmd_type *message )
{
  if (message->cmd.rrc_rel_ind.message_header.message_id == (int) RRC_REL_IND)
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= MM received RRC_REL_IND - rel_cause = %d",
               message->cmd.rrc_rel_ind.rel_cause );

    /* ------------------------------------------------------------------
    ** Pass RRC_REL_IND to both MM & GMM independent of MS operation mode
    ** ------------------------------------------------------------------ */
   if((mm_reg_waiting_for_reg_cnf) &&
      (
#ifdef FEATURE_WRLF_SYSTEM_SEL     
       mm_wcdma_rlf_state == WCDMA_RLF_STARTED ||
#endif
       mm_waiting_for_service_cnf))
    {
        
      mm_reg_waiting_for_reg_cnf = FALSE;
      mm_handle_message( message );
      gmm_handle_message( message );
      mm_reg_waiting_for_reg_cnf = TRUE;
    }
    else
    {
       mm_handle_message( message );
       gmm_handle_message( message );
    }

    if(((mm_stop_mode_req_pended == TRUE) &&(mm_timer_status[ TIMER_MM_REL ] == TIMER_ACTIVE) )
#ifdef FEATURE_DUAL_SIM
       || (mm_timer_status[TIMER_MM_DUAL_SWITCH] == TIMER_ACTIVE) 
#endif
	  )
    {
       mmcoord_process_mmr_stop_mode_req(&mmcoord_pending_reg_message);
    }

  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_est_cnf() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_SYNC_IND

DESCRIPTION
  Function to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_sync_ind( mm_cmd_type *message )
{
  boolean ciphering_enabled = TRUE;

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
  #error code not present
#endif
  
  if (message->cmd.rrc_sync_ind.message_header.message_id == (int) RRC_SYNC_IND)
  {
    MSG_HIGH_DS_2( MM_SUB, "=MM= MM rec'd RRC_SYNC_IND sync_reason %d, Ciphering enabled %d",
               message->cmd.rrc_sync_ind.cause, ciphering_enabled);

    switch (message->cmd.rrc_sync_ind.cause)
    {
      case RRC_CIPHERING_ESTABLISHED :
      case RRC_INTEGRITY_ESTABLISHED :
      case RRC_INTEGRITY_CIPHERING_ESTABLISHED :

        auth_reset_cache_memory(FALSE,NO_TIMER_EXPIRED,
                                      message->cmd.rrc_sync_ind.cn_domain_id) ;

        /* Send CM_CIPHER_IND only if ciphering started or integrity and ciphering were started */
        if ((message->cmd.rrc_sync_ind.cause == RRC_CIPHERING_ESTABLISHED)
               || (message->cmd.rrc_sync_ind.cause == RRC_INTEGRITY_CIPHERING_ESTABLISHED))
        {
          if(message->cmd.rrc_sync_ind.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
          {
             mm_send_cm_cipher_ind(SYS_SRV_DOMAIN_CS_ONLY,ciphering_enabled);
          }
          else
          {
            mm_send_cm_cipher_ind(SYS_SRV_DOMAIN_PS_ONLY,TRUE);
          }
        }
        break ;

#ifdef FEATURE_UMTS_VOICE_CIPHERING_IND
      #error code not present
#endif
      default : /* Nothing to do */
        break ;

    }

    /* ------------------------------------------------
    ** Route SYNC_IND according to CN domain identifier
    ** ------------------------------------------------ */
    if (message->cmd.rrc_sync_ind.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      mm_handle_message( message );
    }
    else /* cn_domain_id == RRC_PS_DOMAIN_CN_ID */
    {
      gmm_handle_message( message );
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_sync_ind() */
/*===========================================================================
 
FUNCTION mmcoord_process_mmr_net_sel_mode_change_ind
 
DESCRIPTION
   This function processes mmr_net_sel_mode_change_ind from REG
 
DEPENDENCIES
   None

RETURN VALUE
   None
 
SIDE EFFECTS
   None
  
===========================================================================*/

void mmcoord_process_mmr_net_sel_mode_change_ind( mm_cmd_type *message )
{
  if (message->cmd.hdr.message_id == (int) MMR_NET_SEL_MODE_CHANGE_IND)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received MMR_NET_SEL_MODE_CHANGE_IND");
    mm_send_nw_sel_mode_reset_ind(message->cmd.mmr_net_sel_mode_change_ind.network_selection_mode, message->cmd.hdr.message_id );
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
}

#ifdef FEATURE_LTE
/*===========================================================================

FUNCTION MMCOORD_UPDATE_RAT_PRI_LIST

DESCRIPTION
  This function updates the new rat priority list info to REG and access stratum.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_update_rat_pri_list(sys_rat_pri_list_info_s_type  rat_pri_list)
{
  emm_ctrl_data_type *emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);
  ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
  mmcoord_pending_reg_message.cmd.mmr_reg_req.rat_pri_list_info = rat_pri_list;
  emm_ctrl_data_ptr->reg_req_pending_ptr->rat_pri_list_info = rat_pri_list;
  mm_send_mmr_update_curr_rat_list_ind(&rat_pri_list);
#ifdef FEATURE_NAS_MODE_CHANGE 
  if (is_gsm_mode())
  {
    rr_change_mode_ind_T rr_change_mode_ind;
    rr_change_mode_ind.message_header.message_set = MS_MM_RR;
    rr_change_mode_ind.message_header.message_id  = (int)RR_CHANGE_MODE_IND;
    rr_change_mode_ind.rat_pri_list_info = rat_pri_list;
    /*lint -save -e778 */
    PUT_IMH_LEN( sizeof( rr_change_mode_ind ) - sizeof(IMH_T),
                  &rr_change_mode_ind.message_header );
    /*lint -restore */
    MSG_HIGH_DS_0(MM_SUB," =MM= MM sent RR_CHANGE_MODE_IND");
#ifdef FEATURE_DUAL_SIM
    mm_update_rat_status(&rat_pri_list,SYS_RAT_ACTIVATION_STATUS_NONE);
#endif
    mm_send_message((IMH_T *)&rr_change_mode_ind, GS_QUEUE_RR );
  }else
#endif
  if(is_umts_mode())
  {
#ifdef FEATURE_WCDMA
    rrc_cmd_type          *rrc_cmd;
    rrc_cmd = mm_get_rrc_cmd_buf( mm_serving_plmn.info.active_rat );
    if (rrc_cmd == NULL)
    {
      MSG_FATAL_DS( MM_SUB, "=MM= ERROR: Unable to allocate memory", 0,0,0 );
    }
    rrc_cmd->cmd_hdr.cmd_id = RRC_CHANGE_MODE_IND;
    rrc_cmd->cmd.rrc_change_mode_ind.rat_pri_list_info = rat_pri_list;
    send_cmd_to_rrc( rrc_cmd, mm_serving_plmn.info.active_rat );
#endif
  }
}


/*===========================================================================

FUNCTION MMCOORD_REEVALUATE_RAT_PRI_LIST

DESCRIPTION
  If UE is camped on a backoff FPLMN or EPLMN of a backoff FPLMN and rat priority list has LTE in it
  then remove LTE rat from the list and update the REG and RRC or RR.

  If UE previously camped on GPRS forbidden PLMN and now moves to an EPLMN where GPRS is not forbidden
  then reenable LTE.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_reevaluate_rat_pri_list(sys_plmn_id_s_type plmn)
{
  reg_sim_plmn_timer_info_s_type plmn_timer_info;
  plmn_timer_info.plmn = plmn;
  plmn_timer_info.timer_count = 0;
  plmn_timer_info.cause = SYS_BLOCK_PLMN_CAUSE_NONE;
  /*
  ** If rat priority list has LTE and PLMN or its EPLMN is backoff forbidden then
  ** remove LTE from rat list, updated REG and also RRC/RR with change mode
  ** indication.
  */
  if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info,SYS_RAT_LTE_RADIO_ACCESS)
    &&(reg_sim_plmn_rat_forbidden(plmn,SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)mm_sub_id) ||
       reg_mode_check_eplmn_backoff_forbidden(&plmn_timer_info,(sys_modem_as_id_e_type)mm_sub_id))
     &&(rrc_connection_status == MMRRC_IDLE))
  {
    mm_remove_rat_from_rat_pri_list(&mm_rat_pri_list_info,SYS_SYS_MODE_LTE);
    mmcoord_update_rat_pri_list(mm_rat_pri_list_info);
  }
  /*
  ** If LTE is previously disabled and current camped PLMN is not LTE forbidden nor it is an EPLMN 
  ** of a backoff forbidden PLMN then reenable LTE.
  */
  else if((mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info,SYS_RAT_LTE_RADIO_ACCESS) == FALSE) &&
          (mm_checkif_rat_pri_list_has_rat
             (&mmcoord_pending_reg_message.cmd.mmr_reg_req.original_rat_pri_list_info,
              SYS_RAT_LTE_RADIO_ACCESS) == TRUE) &&
          (reg_sim_plmn_rat_forbidden(plmn,SYS_RAT_LTE_RADIO_ACCESS,(sys_modem_as_id_e_type)mm_sub_id) == FALSE) &&
          (reg_mode_check_eplmn_backoff_forbidden(&plmn_timer_info,
                                   (sys_modem_as_id_e_type)mm_sub_id) == FALSE) &&
          (mm_per_nas_stacks_get_connection_status((sys_modem_as_id_e_type)mm_sub_id,(sys_modem_as_id_e_type)mm_as_id)
                                                                                            != SYS_GW_CONN_STATUS_CONN)&&
          (mm_checkif_rat_pri_list_superset(&mmcoord_pending_reg_message.cmd.mmr_reg_req.original_rat_pri_list_info,&mm_rat_pri_list_info))&&
           /* In case of force pref change, let REG take the decison to start plmn selection*/
          reg_state_get_force_pref_pending() == FALSE)
  {
    NAS_ENTER_CRIT_SECT(mm_rat_prioritylist_crit_sec);  
    mm_rat_pri_list_info = mmcoord_pending_reg_message.cmd.mmr_reg_req.original_rat_pri_list_info;
    NAS_EXIT_CRIT_SECT(mm_rat_prioritylist_crit_sec);
    emm_send_multimode_rrc_system_update_from_GW(emm_ctrl_data_ptr);
    mmcoord_update_rat_pri_list(mm_rat_pri_list_info);
  }
}
#endif

#ifdef FEATURE_NAS_RAU_ON_MS_RAC_CHANGE
boolean mmcoord_check_if_ms_rat_capability_changed(void)
{
  boolean changed = FALSE;
  /* ---------------------------------------------------------------------
  ** 3GPP spec 24.008 4.7.5.1: In A/Gb mode, the normal routing area updating procedure is initiated 
  ** when the MS needs to update the network with the new MS Radio Access Capability IE 
  ** ------------------------------------------------------------------- */
  if(

      (mm_rat_pri_list_match(&mmcoord_pending_reg_message.cmd.mmr_reg_req.rat_pri_list_info,
                             &mm_rat_pri_list_info_to_nw) == FALSE) &&

      (gmm_state == GMM_REGISTERED)&&
      (gmm_update_status == GMM_GU1_UPDATED))
  {

    MSG_HIGH_DS_0(MM_SUB, "=MM= Pending RAU as MS RAC has changed rat priority list ");

    changed = TRUE;
  }
  return changed;
}
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL

/*===========================================================================

FUNCTION mmcoord_handle_wrlf_rrc_service_cnf

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_handle_wrlf_rrc_service_cnf( mm_cmd_type *message )
{
  boolean                   cs_service_available;
  boolean                   ps_service_available;
  byte                      i;
  serving_plmn_info_T *serving_plmn_info_ptr;
  mm_as_cell_access_status_T     cs_cell_access = MM_AS_ACCESS_ALL_CALLS;
  mm_as_cell_access_status_T     ps_cell_access = MM_AS_ACCESS_ALL_CALLS;
 
  switch (message->cmd.rrc_service_cnf.service_status)
  {
    case MM_AS_NO_SERVICE:
      MSG_HIGH_DS_0(MM_SUB,"=MM= Revd RRC_SERVICE_CNF, NO SERVICE  RLF state");

      mm_serving_plmn.info.plmn.identity[0] = 0xFF;
      mm_serving_plmn.info.plmn.identity[1] = 0xFF;
      mm_serving_plmn.info.plmn.identity[2] = 0xFF;
      mm_serving_plmn.lac.lac[0]            = 0xFF;
      mm_serving_plmn.lac.lac[1]            = 0xFF;
      mm_serving_plmn.network_op_mode       = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
      mm_serving_plmn.gmm_service_state     = SERVICE_NOT_AVAILABLE;
      mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
      mm_serving_plmn.info.no_service_cause = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
      mm_mmr_cell_service_ind_pending = FALSE;
#ifdef FEATURE_FEMTO_CSG
      mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif       
      mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                                mm_serving_plmn.info.reg_service_domain,
                                                FALSE,
                                                SYS_SRV_STATUS_NO_SRV,
                                                FALSE );
      gmm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                           mm_serving_plmn.info.reg_service_domain,
                                           FALSE,
                                           SYS_SRV_STATUS_NO_SRV,
                                          FALSE );
#ifdef FEATURE_DUAL_SIM
      if (message->cmd.rrc_service_cnf.acq_status == SYS_ACQ_STATUS_NO_RESOURCE)
      {
        mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_NO_RESOURCE;
      }
      else
      {
        mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
      }
#elif defined (FEATURE_1XSRLTE)
      mm_serving_plmn.info.acq_status 	  = SYS_ACQ_STATUS_SUCCESS;
#endif

      if (mm_reg_waiting_for_reg_cnf)
      {
        sys_plmn_id_s_type nas_type_plmn;
        dword                                  entry;
        dword                                  num_plmns;
        rrc_avail_plmn_type*                    rrc_plmn_list;
        sys_detailed_plmn_list_info_s_type*    reg_plmn_list;

        mmcoord_mmr_reg_cnf.service_state  = mm_serving_plmn.info;
        num_plmns = message->cmd.rrc_service_cnf.avail_plmn_list.num_plmns;
        mmcoord_mmr_reg_cnf.available_plmn_list.length = num_plmns;
 
        /* Fill the Available PLMN list into the REG_CNF primitive */
        for (entry = 0; entry < num_plmns; entry++ )
        {
          rrc_plmn_list = &message->cmd.rrc_service_cnf.avail_plmn_list.plmn[entry];
          reg_plmn_list = &mmcoord_mmr_reg_cnf.available_plmn_list.info[entry];
 
          nas_type_plmn =
            mmumts_convert_rrc_plmn_id_to_nas_plmn_id(rrc_plmn_list->plmn);
 
          memscpy(reg_plmn_list->plmn.identity, sizeof(sys_plmn_id_s_type), nas_type_plmn.identity, PLMN_SIZE);
          reg_plmn_list->plmn_service_capability = rrc_plmn_list->service_capability;
          reg_plmn_list->signal_quality = rrc_plmn_list->signal_quality;
          reg_plmn_list->signal_strength = rrc_plmn_list->signal_strength;
          reg_plmn_list->rat = rrc_plmn_list->rat;
          reg_plmn_list->plmn_forbidden = FALSE;
          reg_plmn_list->list_category = SYS_DETAILED_PLMN_LIST_CATEGORY_PREFERRED;
        }
        mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;
        mmcoord_mmr_reg_cnf.ue_oos = message->cmd.rrc_service_cnf.ue_oos;
        mm_send_mmr_reg_cnf();
        mm_reg_waiting_for_reg_cnf = FALSE;
        mm_ready_to_send_reg_cnf   = FALSE;
      }
      break;

    case MM_AS_SERVICE_AVAILABLE:
      MSG_HIGH_DS_1(MM_SUB ,"=MM= Revd RRC_SERVICE_CNF, w/ SRV-AC = %d RLF STATE",
                 message->cmd.rrc_service_cnf.service_info.cell_access);

      /* ------------------------------------------------------------
      ** During WRLF state if rrc finds service on rplmn or equivalent plmn
      ** rrc will not send plmn id and network information. So copy the information
      ** from the mm_wrlf_serving_plmn_ptr into the message before processing 
      ** the confirmation normally
      ** ------------------------------------------------------------*/
      if ((message->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[0] == 0xFF) &&
          (message->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[1] == 0xFF) &&
          (message->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id.mcc[2] == 0xFF))
      {

        serving_plmn_info_ptr = mm_wrlf_serving_plmn_ptr;
        //mm_serving_plmn.info = serving_plmn_info_ptr->info;
        message->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id = 
            mmumts_convert_nas_plmn_id_to_rrc_plmn_id(serving_plmn_info_ptr->info.plmn);
        message->cmd.rrc_service_cnf.service_info.gsm_map_common_info_incl = TRUE;
        message->cmd.rrc_service_cnf.service_info.gsm_map_common_info.value[0] = 
            serving_plmn_info_ptr->lac.lac[0];
        message->cmd.rrc_service_cnf.service_info.gsm_map_common_info.value[1] =
           serving_plmn_info_ptr->lac.lac[1];
         i = 0;
        message->cmd.rrc_service_cnf.service_info.num_cn_domains = 0;
        if(serving_plmn_info_ptr->gmm_service_state == SERVICE_AVAILABLE)
        {
          message->cmd.rrc_service_cnf.service_info.num_cn_domains++;
          message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].cn_domain_id = 
                RRC_PS_DOMAIN_CN_ID;
          message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].cn_domain_specific_info.
               gsm_map_info.length = 2;
          message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].cn_domain_specific_info.
               gsm_map_info.value[0] =serving_plmn_info_ptr->rac;
          message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].cn_domain_specific_info.
               gsm_map_info.value[1] = (byte)serving_plmn_info_ptr->network_op_mode;
          i++;
        }
        if((serving_plmn_info_ptr->info.plmn_service_capability == SYS_SRV_CAPABILITY_CS_PS_SERVICE) ||
            (serving_plmn_info_ptr->info.plmn_service_capability == SYS_SRV_CAPABILITY_CS_SERVICE_ONLY))
        {
          message->cmd.rrc_service_cnf.service_info.num_cn_domains++;
          message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].cn_domain_id = 
              RRC_CS_DOMAIN_CN_ID;
          message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].cn_domain_specific_info.
               gsm_map_info.length = 2;
          message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].cn_domain_specific_info.
               gsm_map_info.value[0] = (uint8)((mm_system_information.T3212_timer_value)/(360L * 1000L));
          message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].cn_domain_specific_info.
               gsm_map_info.value[1] =  mm_system_information.ATT;
        }
        message->cmd.rrc_service_cnf.is_cell_id_valid = TRUE;
        message->cmd.rrc_service_cnf.cell_id = mm_serving_cell_id;
        if(message->cmd.rrc_service_cnf.service_info.dsac_present)
        {
          if(cs_session_status == MMRRC_ACTIVE)
          {
            cs_cell_access  = message->cmd.rrc_service_cnf.service_info.cs_cell_access;
            message->cmd.rrc_service_cnf.service_info.cs_cell_access = MM_AS_ACCESS_ALL_CALLS;
          }
          if(ps_session_status == MMRRC_ACTIVE)
          {
            ps_cell_access  = message->cmd.rrc_service_cnf.service_info.ps_cell_access;
            message->cmd.rrc_service_cnf.service_info.ps_cell_access = MM_AS_ACCESS_ALL_CALLS;
          }
        }
#ifdef FEATURE_FEMTO_CSG
        if(is_wcdma_mode())
        {
        message->cmd.rrc_service_cnf.service_info.csg_info = serving_plmn_info_ptr->info.csg_info;
        }
        else
        {
          message->cmd.rrc_service_cnf.service_info.csg_info.csg_id = SYS_CSG_ID_INVALID;
        }
#endif 
        mmcoord_set_wrlf_state(WCDMA_RLF_SERVICE_CNF_PENDING);
        mmcoord_process_rrc_service_cnf(message);
        mmcoord_set_wrlf_state(WCDMA_RLF_IDLE);
        mm_received_suitable_search_end_ind = FALSE;


        if(cs_cell_access != MM_AS_ACCESS_ALL_CALLS)
        {
          message->cmd.rrc_service_cnf.service_info.cs_cell_access = cs_cell_access;
        }
        if(ps_cell_access != MM_AS_ACCESS_ALL_CALLS)
        {
          message->cmd.rrc_service_cnf.service_info.ps_cell_access = ps_cell_access;
        }
#ifdef FEATURE_DSAC
        mm_process_dsac_info_from_rrc_service_cnf(&(message->cmd.rrc_service_cnf.service_info));
#endif
      }
      else if((mm_wrlf_serving_plmn_ptr->info.active_rat != mm_serving_plmn.info.active_rat)
               && mm_is_registration_allowed_on_selected_plmn(message)) 
      {
#ifdef FEATURE_WCDMA

        rrc_cmd_type *rrc_cmd_p = mm_get_rrc_cmd_buf(
                                mm_wrlf_serving_plmn_ptr->info.active_rat );
#endif
#ifdef FEATURE_WCDMA
        if (rrc_cmd_p == NULL)
        {
          MSG_FATAL_DS(MM_SUB, "ERROR: Unable to allocate memory", 0,0,0 );
        }
        mmcoord_set_wrlf_state(WCDMA_RLF_SERVICE_CNF_PENDING);

        if(( mm_wrlf_pending_rrc_message_ptr == NULL ) && 
           ((mm_wrlf_pending_rrc_message_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) 
                    == NULL))
        {
          MSG_FATAL_DS(MM_SUB, "ERROR: Unable to allocate memory", 0,0,0 );
        }
        memscpy( mm_wrlf_pending_rrc_message_ptr, sizeof(mm_cmd_type), message, sizeof(mm_cmd_type));
        rrc_cmd_p->cmd_hdr.cmd_id = RRC_ABORT_REQ;
        rrc_cmd_p->cmd.rrc_abort_req.auth_failed_bar_cells = FALSE;
        rrc_cmd_p->cmd.rrc_abort_req.delay_sig_con_release = FALSE;
        rrc_cmd_p->cmd.rrc_abort_req.hard_abort = TRUE;
        rrc_cmd_p->cmd.rrc_abort_req.abort_cause = RRC_OTHERS;
        MSG_HIGH_DS_0(MM_SUB,"=MM= RLF ABORTING STATE ");
        send_cmd_to_rrc( rrc_cmd_p, mm_wrlf_serving_plmn_ptr->info.active_rat );
#endif /* FEATURE_WCDMA */
      }
      /* ------------------------------------------------------------
      ** During WRLF state if rrc finds limited service on any plmn 
      ** it sends the network information along with plmn id
      ** ------------------------------------------------------------*/
      else
      {
        MSG_HIGH_DS_1(MM_SUB ,"=MM= Limited service available Net_sel_mode = %d ", 
        mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode);
        mm_serving_plmn.info.plmn = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
          message->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id );
    
        if (message->cmd.rrc_service_cnf.service_info.gsm_map_common_info_incl)
        {
          mm_serving_plmn.lac.lac[0] =
            message->cmd.rrc_service_cnf.service_info.gsm_map_common_info.value[0];
          mm_serving_plmn.lac.lac[1] =
            message->cmd.rrc_service_cnf.service_info.gsm_map_common_info.value[1];
        }
    
        ps_service_available = FALSE;
        cs_service_available = FALSE;
    
        for (i=0; i < message->cmd.rrc_service_cnf.service_info.num_cn_domains; i++)
        {
          if (message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                cn_domain_id == RRC_PS_DOMAIN_CN_ID)
          {
            ps_service_available = TRUE;
            mm_serving_plmn.gmm_service_state   = SERVICE_AVAILABLE;
    
            mm_serving_plmn.rac = message->cmd.rrc_service_cnf.service_info.
              cn_domain_info[i].cn_domain_specific_info.gsm_map_info.value[0];
    
            if ((message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                   cn_domain_specific_info.gsm_map_info.value[1] & 0x01) == 0)
            {
              mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_1;
            }
            else
            {
              mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_2;
            }
    
            MSG_HIGH_DS_2( MM_SUB, "=MM= GMM sys info: NMO=%d, RAC=%d",
                       mm_serving_plmn.network_op_mode, mm_serving_plmn.rac);
          }
          else if (message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                    cn_domain_id == RRC_CS_DOMAIN_CN_ID)
          {
            cs_service_available = TRUE;
            mm_serving_plmn.mm_service_state    = SERVICE_AVAILABLE;
          }
        } /* end for */
    
        mm_serving_cell_id = message->cmd.rrc_service_cnf.cell_id;
    
        if (message->cmd.rrc_service_cnf.is_cell_id_valid)
        {
           mm_serving_cell_valid = TRUE;
        }
        else
        {
           mm_serving_cell_valid = FALSE;
        }
    
        if (ps_service_available)
        {
          if (cs_service_available)
          {
            mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
          }
          else
          {
            mm_serving_plmn.info.plmn_service_capability =
              SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
          }
        }
        else 
        {
          mm_serving_plmn.network_op_mode = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
    
          if (cs_service_available)
          {
            mm_serving_plmn.info.plmn_service_capability =
              SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
          }
          else
          {
            mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
          }
        }

        /* Set the Serving PLMN service status to Limited */
        mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                         mm_serving_plmn.info.reg_service_domain,
                                         TRUE,
                                         SYS_SRV_STATUS_LIMITED,
                                         FALSE );            
        mm_send_mmr_camped_ind(FALSE);
        mm_rat_handle_srv_status_update ( SYS_SRV_STATUS_SRV );
        if (mm_reg_waiting_for_reg_cnf)
        {
          /* Set the REG CNF plmn service state to the Serving PLMN info */
          mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
          
          /* Send the Confirmation to the REG task */
          mm_send_mmr_reg_cnf();
          
          mm_reg_waiting_for_reg_cnf = FALSE;
          mm_ready_to_send_reg_cnf   = FALSE;
        }
      }
      break;
  }
}
#endif



/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_SERVICE_CNF

DESCRIPTION
  Function to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_service_cnf( mm_cmd_type *message )
{
  sys_plmn_id_s_type        requested_plmn;
  sys_srv_capability_e_type requested_service_type;
  boolean                   cs_service_available;
  byte                      i;

#ifdef FEATURE_FEMTO_CSG
  sys_csg_id_type             requested_csg;
#endif 
#ifdef FEATURE_SGLTE
  boolean mm_is_sglte_ue_mode_change_pended = FALSE;
#endif
  boolean policy_change_pending = FALSE;
#ifdef FEATURE_LTE
  emm_ctrl_data_type *emm_ctrl_data_ptr_tag = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr_tag != NULL);
#endif  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (message->cmd.hdr.message_id == (int) RRC_SERVICE_CNF)
  {
    if ( mm_waiting_for_deact_cnf || mm_nreg_req_received || mm_waiting_for_stop_mode_cnf)
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= Unexpected RRC_SERVICE_CNF as deact is pending");
      return;
    }

    if(mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ)
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= Waiting for Reg req so ignoring RRC_SERVICE_CNF");
      return;
    }

    mm_waiting_for_service_cnf = FALSE;
    mm_serv_change_pending     = FALSE;
    if(mm_last_sent_serv_req_params_ptr != NULL)
    {
      modem_mem_free(mm_last_sent_serv_req_params_ptr,MODEM_MEM_CLIENT_NAS);
      mm_last_sent_serv_req_params_ptr = NULL;
    }

#ifdef FEATURE_WCDMA_MULTIBAND
    mm_active_band = message->cmd.rrc_service_cnf.active_band;
#endif

    mm_band_indicator = SYS_BAND_CLASS_NONE;

#ifdef FEATURE_GSM_DTM
    mm_serving_plmn.dtm_supported = FALSE;
#endif
    mm_serving_plmn.egprs_supported = FALSE;

    mmcoord_mmr_reg_cnf.ue_oos = message->cmd.rrc_service_cnf.ue_oos;

#ifdef FEATURE_EOOS
    #error code not present
#endif
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(mm_wcdma_rlf_state == WCDMA_RLF_STARTED)
    {
      mmcoord_handle_wrlf_rrc_service_cnf(message);
    }
    else
#endif
    {
#if defined(FEATURE_WRLF_SYSTEM_SEL) || defined(FEATURE_LTE)
      mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
#endif
      /* --------------------------------------------------
      ** Handle the response from RRC after a LAI selection.
      ** Success or failure is reported to REG.  If a failure
      ** RRC reports the current list of LAIs.
      ** ---------------------------------------------------- */
      switch (message->cmd.rrc_service_cnf.service_status)
      {
        case MM_AS_NO_SERVICE:
      
          MSG_HIGH_DS_1(MM_SUB,"=MM= Revd RRC_SERVICE_CNF, NO SERVICE ue_oos flag is %d", message->cmd.rrc_service_cnf.ue_oos);
      
          /*
          ** If during GTW RAT change MM decided to invalidate CKSNs
          ** due to lack of UMTS coverage undo invalidation
          */
      
          mm_serving_plmn.info.plmn.identity[0] = 0xFF;
          mm_serving_plmn.info.plmn.identity[1] = 0xFF;
          mm_serving_plmn.info.plmn.identity[2] = 0xFF;
          mm_serving_plmn.lac.lac[0]            = 0xFF;
          mm_serving_plmn.lac.lac[1]            = 0xFF;
          mm_serving_plmn.network_op_mode       = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
          mm_serving_plmn.gmm_service_state     = SERVICE_NOT_AVAILABLE;
          mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
          mm_mmr_cell_service_ind_pending = FALSE;
          mm_coord_camped_cell_id = 0xFFFF;
         if((mmcoord_pending_reg_message.cmd.hdr.message_id == (int)MMR_REG_REQ) &&
            ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                    SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY) ||
            (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                    SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY)) )
          {
            mm_serving_plmn.info.plmn_reg_type = SYS_REG_DOMAIN_NO_SERVICE;
          }
#ifdef FEATURE_FEMTO_CSG
          mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif 
#ifdef FEATURE_DUAL_SIM
          mm_serving_plmn.info.acq_status = message->cmd.rrc_service_cnf.acq_status;
#elif defined (FEATURE_1XSRLTE)
          mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif
 
           
          /* ---------------------------------------------------------------
          ** Pass RRC_SERVICE_CNF to both MM & GMM independent of MS op mode
          ** --------------------------------------------------------------- */
          mm_handle_message( message );
          gmm_handle_message( message );
      
          break;
      
        case MM_AS_SERVICE_AVAILABLE:
      
          MSG_HIGH_DS_1(MM_SUB ,"=MM= Revd RRC_SERVICE_CNF, w/ SRV-AC = %d",
                     message->cmd.rrc_service_cnf.service_info.cell_access);
/*Reset the flag while registering*/
#ifdef FEATURE_NAS_REL11
          gmm_is_lw_to_g_interrat_in_progress = FALSE;
#endif
          sys_eplmn_per_nas_region_update(FALSE);
          /* -----------------------------------------------------
          ** First store the PLMN where we are currently camped so
          ** we can compare with requested PLMN from MMR_REG_REQ
          ** ----------------------------------------------------- */
#ifdef FEATURE_LTE          
          if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
          {
            emm_rat_change_stop_timers();
            gmm_reset_mcs_client_info();
          }
#endif
          mm_serving_plmn.info.plmn = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
            message->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_id );

#ifndef FEATURE_DSAC          
          mm_system_information.cell_access =
            message->cmd.rrc_service_cnf.service_info.cell_access;
#endif

          gmm_reset_old_ptmsi();
          if (message->cmd.rrc_service_cnf.service_info.gsm_map_common_info_incl)
          {
            mm_serving_plmn.lac.lac[0] =
              message->cmd.rrc_service_cnf.service_info.gsm_map_common_info.value[0];
            mm_serving_plmn.lac.lac[1] =
              message->cmd.rrc_service_cnf.service_info.gsm_map_common_info.value[1];
          }
      
          mm_serving_plmn.gmm_service_state  = SERVICE_NOT_AVAILABLE;
          cs_service_available = FALSE;

          for (i=0; i < message->cmd.rrc_service_cnf.service_info.num_cn_domains; i++)
          {
            if (message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                  cn_domain_id == RRC_PS_DOMAIN_CN_ID)
            {
              mm_serving_plmn.gmm_service_state = SERVICE_AVAILABLE;
      
              mm_serving_plmn.rac = message->cmd.rrc_service_cnf.service_info.
                cn_domain_info[i].cn_domain_specific_info.gsm_map_info.value[0];
      
              if ((message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                     cn_domain_specific_info.gsm_map_info.value[1] & 0x01) == 0)
              {
                mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_1;
              }
              else
              {
                mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_2;
              }
      
              MSG_HIGH_DS_2(MM_SUB, "=MM= GMM sys info: NMO=%d, RAC=%d",
                         mm_serving_plmn.network_op_mode, mm_serving_plmn.rac);
            }
            else if (message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                      cn_domain_id == RRC_CS_DOMAIN_CN_ID)
            {
              cs_service_available = TRUE;
            }
          } /* end for */
      
          mm_serving_cell_id = message->cmd.rrc_service_cnf.cell_id;
      
          if (message->cmd.rrc_service_cnf.is_cell_id_valid)
          {
             mm_serving_cell_valid = TRUE;
             mm_coord_camped_cell_id = (word)(mm_serving_cell_id & (uint32)0x0000FFFF); 
          }
          else
          {
             mm_serving_cell_valid = FALSE;
             mm_coord_camped_cell_id = 0xFFFF;   
          }
      
          if (mm_serving_plmn.gmm_service_state == SERVICE_AVAILABLE)
          {
            if (cs_service_available)
            {
              mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              mm_serving_plmn.info.plmn_service_capability =
                SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
            }
          }
          else /* mm_serving_plmn.gmm_service_state == SERVICE_NOT_AVAILABLE */
          {
            mm_serving_plmn.network_op_mode = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
      
            if (cs_service_available)
            {
              mm_serving_plmn.info.plmn_service_capability =
                SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
            }
            else
            {
              mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
            }
          }
      
          mm_serving_plmn.t3122_running = TRUE;
      
          MSG_HIGH_DS_1(MM_SUB ,"=MM= mm_serving_plmn.t3122_running = %d",mm_serving_plmn.t3122_running);
      
#ifdef FEATURE_FEMTO_CSG
          if(is_wcdma_mode())
          {
            mm_serving_plmn.info.csg_info = message->cmd.rrc_service_cnf.service_info.csg_info;
          }
          else
          {
            mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
          }

          MSG_HIGH_DS_3(MM_SUB, "=MM= CSG: csg_id = %d, hnb_name_length = %d, hybrid_cell=%d", 
                                       mm_serving_plmn.info.csg_info.csg_id, 
                                       mm_serving_plmn.info.csg_info.hnb_name.length,
                                       mm_serving_plmn.info.csg_info.hybrid_cell);
#endif       
      #ifdef FEATURE_DSAC
          mm_process_dsac_info_from_rrc_service_cnf(&(message->cmd.rrc_service_cnf.service_info));
      #endif
#ifdef FEATURE_LTE
          if((gmm_state == GMM_DEREGISTERED || mm_possible_imsi_attach == TRUE) &&
             (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                SYS_NETWORK_SELECTION_MODE_LIMITED) && 
             (mm_serving_plmn.info.sim_state != SYS_SIM_STATE_CS_PS_INVALID) )
          {
            if(emm_validate_attach_plmn(emm_ctrl_data_ptr_tag, &mm_serving_plmn.info.plmn) == FALSE)
            {
              mm_update_mm_serving_plmn_info(SYS_SIM_STATE_CS_PS_INVALID,
                                             SYS_SRV_DOMAIN_NO_SRV,
                                             TRUE,
                                             SYS_SRV_STATUS_LIMITED,
                                             FALSE);
#ifdef FEATURE_DUAL_SIM  
              mm_gmm_set_non_illegal_cause();
#endif
            }
          }
#ifdef FEATURE_LTE_REL9
          if((mm_is_reg_needed_for_mmit_on_GW()) &&
             (mm_service_loss_on_multimode_rat == SYS_RAT_LTE_RADIO_ACCESS))
          {
            MSG_HIGH_DS_0(MM_SUB,"=EMM= RAU/LAU to be performed - Mobility Management for IMS voice termination");
            emm_set_and_write_tin_type(NAS_MM_TIN_GUTI);
            mm_pending_lu_request = LTE_IRAT_LU;
          }
#endif
          mmcoord_locally_deactivate_isr_if_needed();
#endif

          policy_change_pending = mmcoord_policy_change_pending();
          /* ---------------------------------------------------------------
          ** Pass RRC_SERVICE_CNF to both MM & GMM independent of MS op mode
          ** --------------------------------------------------------------- */
          mm_handle_message( message );
          gmm_handle_message( message );
          
          mm_send_mmr_cell_access_ind();
#ifdef FEATURE_LTE
          mmcoord_reevaluate_rat_pri_list(mm_serving_plmn.info.plmn);
          if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
          {
            mm_send_rrc_lte_reselection_status_ind();
          }
          /*
           4.7.5.1 Normal and periodic routing area updating procedure
           in Iu mode and A/Gb mode, after intersystem change from S1 mode, and the GMM receives an indication of "RRC connection failure" 
           from lower layers due to lower layer while in S1 mode;
          */
          
          
          if(emm_ctrl_data_ptr->rau_pending_ll_failure == TRUE)
          {
 	        mm_pend_lu_rau_on_ll_failure_in_lte();
          }
#endif
         
#ifdef FEATURE_NAS_RAU_ON_MS_RAC_CHANGE
          if((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                                          SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)&&
              (sys_plmn_id_is_undefined(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) == TRUE)&&
              (mmcoord_check_if_ms_rat_capability_changed() == TRUE))
          {
            gmm_ra_update_pending = TRUE;
          }
#endif

#ifdef FEATURE_SGLTE
          if ((mm_is_sglteplmn_in_sglte_roam( mm_serving_plmn.info.plmn) == TRUE)
            ||(mm_process_if_both_stack_has_camped_non_sglte_plmn(mm_ps_stack_id)==TRUE))
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM=SGLTE UE MODE CHANGE PENDING ") ;
            mm_is_sglte_ue_mode_change_pended= TRUE;
          }
          else if ( mm_process_sglte_plmn_state4_gsm_oos(mm_serving_plmn.info.plmn)==TRUE )
          {
            MSG_HIGH_DS_0(MM_SUB, "=EMM=LTE got SGLTE PLMN in State4 While GSM OOS ");
          }
#endif

          /* ------------------------------------------------
          ** Confirm requested PLMN matches the acquired PLMN.
          ** If there is a match and the AC does not indicate
          ** an access class restriction
          ** ------------------------------------------------ */
          requested_plmn = mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn;
   #ifdef FEATURE_FEMTO_CSG
          requested_csg = mmcoord_pending_reg_message.cmd.mmr_reg_req.csg_id;
   #endif    
          if ((mmcoord_pending_reg_message.cmd.hdr.message_id == (int)MMR_REG_REQ) &&
              ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                SYS_NETWORK_SELECTION_MODE_LIMITED) &&
              (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY) &&
              (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY) &&
               (((mm_plmn_match(mm_serving_plmn.info.plmn, requested_plmn) ||
                   (reg_mode_get_hplmn_to_selected(
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
                                                           (sys_modem_as_id_e_type)mm_as_id
#endif
                                                          ) &&
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_EQUIVALENT_HPLMN
                (reg_sim_per_subs_is_ehplmn(mm_serving_plmn.info.plmn,(sys_modem_as_id_e_type)mm_sub_id) )
#else
                (sys_plmn_match(reg_sim_per_subs_read_hplmn((sys_modem_as_id_e_type)mm_sub_id), mm_serving_plmn.info.plmn) )
#endif
#else
#ifdef FEATURE_EQUIVALENT_HPLMN
                (reg_sim_is_ehplmn(mm_serving_plmn.info.plmn) )
#else
                (sys_plmn_match(reg_sim_read_hplmn(), mm_serving_plmn.info.plmn) )
#endif
#endif            
                   ))
  #ifdef FEATURE_FEMTO_CSG
                 && (requested_csg == SYS_CSG_ID_INVALID ||
                         requested_csg == mm_serving_plmn.info.csg_info.csg_id)
  #endif 
               ) ||
                (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                   SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)))
#ifdef FEATURE_SGLTE
                  &&( mm_is_sglte_ue_mode_change_pended == FALSE)
#endif
                 && (policy_change_pending == FALSE)
                  &&(cm_per_subs_is_reg_allowed(mm_serving_plmn.info.plmn,
                     mm_serving_plmn.info.active_rat,(sys_modem_as_id_e_type) mm_as_id,mm_set_cm_stack_id(mm_as_id)) == TRUE))
          {
            /* --------------------------------------------------------------------
            ** Confirm cell's service capabilty supports the requested service type
            ** -------------------------------------------------------------------- */
            if (mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain ==
                  SYS_SRV_DOMAIN_CS_ONLY)
            {
              requested_service_type = SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
            }
            else if (mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain ==
                       SYS_SRV_DOMAIN_PS_ONLY)
            {
              requested_service_type = SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
            }
            else if (mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain ==
                       SYS_SRV_DOMAIN_CS_PS)
            {
              requested_service_type = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              MSG_ERROR_DS(MM_SUB, "Unexpected req_service_domain %d",
                    mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain, 0,0 );
              requested_service_type = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
            }
      
            if ((mm_serving_plmn.info.plmn_service_capability !=
                   SYS_SRV_CAPABILITY_CS_PS_SERVICE) &&
                ((mm_serving_plmn.info.plmn_service_capability != requested_service_type) &&
                 (mm_serving_plmn.ms_op_mode != SYS_SRV_DOMAIN_CS_PS)))
            {
      
              if (mm_reg_waiting_for_reg_cnf)
              {
                /* ------------------------------------------------------------
                ** PLMN found with service that does not match requested domain
                ** ------------------------------------------------------------ */
                mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                                mm_serving_plmn.info.reg_service_domain,
                                                TRUE,
                                                SYS_SRV_STATUS_LIMITED,
                                                TRUE );
      
                mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
      
                mm_send_mmr_reg_cnf();
      
                mm_reg_waiting_for_reg_cnf       = FALSE;
                mm_ready_to_send_reg_cnf         = FALSE;
              }
              else
              {
                MSG_ERROR_DS(MM_SUB, "Unexpected RRC_SERVICE_CNF", 0,0,0 );
              }
            }
            else /* (mm_serving_plmn.info.plmn_service_capability == req_service_domain) */
            {
              mm_as_cell_access_status_T cs_cell_access, ps_cell_access;
              boolean lu_required, attach_required;
      
              cs_cell_access = mm_get_cs_cell_access(PROCEDURE_CS_REGISTRATION);
              ps_cell_access = gmm_get_ps_cell_access(PROCEDURE_PS_REGISTRATION);
              lu_required = FALSE;
              attach_required = FALSE;
      
              /*
              ** Check for access class blocking restrictions and stop a LU procedure if necessary
              */
              if (mm_check_if_cs_cell_access_allowed(cs_cell_access) == TRUE)
              {
                lu_required = TRUE;
              }
              if (gmm_check_if_ps_cell_access_allowed(ps_cell_access) == TRUE)
              {
                attach_required = TRUE;
              }
      
              if (!attach_required)
              {
      
                /*****************************************************************************
                *Reset this flag to false as we are not trying PS registration and if PS fop
                *is to be set,SM will be in SIG_CONNECTION_PENDING state.It will take care 
                *of setting PS fop to true
                *****************************************************************************/ 
      
                gmm_follow_on_request = FALSE;
      
              }
      
              /* ---------------------------------------------------------
              ** Now initiate registration attempt according to MS op mode
              ** --------------------------------------------------------- */
              if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY) ||
                  (mm_serving_plmn.info.plmn_service_capability ==
                     SYS_SRV_CAPABILITY_PS_SERVICE_ONLY))
              {
                mm_ready_to_send_reg_cnf = TRUE;
                gmm_handle_plmn_found( mm_serving_plmn, attach_required );
              }
              else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) ||
                       (mm_serving_plmn.info.plmn_service_capability ==
                          SYS_SRV_CAPABILITY_CS_SERVICE_ONLY))
              {
                mm_handle_plmn_found( mm_serving_plmn, lu_required );
              }
              else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
              {
                if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1) 
                {
                  if (attach_required == TRUE)
                  {
                 #ifdef FEATURE_LTE
                    if(mm_csfb_state == MM_CSFB_STATE_MO_CALL_ORIG ||
                         mm_csfb_state == MM_CSFB_STATE_MT_CALL_ORIG)
                    {
                      mm_handle_plmn_found( mm_serving_plmn, lu_required );
                    }
                    else  
                 #endif   
                    { 
                      mm_ready_to_send_reg_cnf = TRUE;
                    }
                    gmm_handle_plmn_found( mm_serving_plmn, attach_required);
                    
                  }
                  else
                  {
                    mm_handle_plmn_found( mm_serving_plmn, lu_required );
                    gmm_handle_plmn_found( mm_serving_plmn, attach_required );                
                  }                         
                }
                else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
                {
                  mm_handle_plmn_found( mm_serving_plmn, lu_required );
                  gmm_handle_plmn_found( mm_serving_plmn, attach_required );
                }
              }
            }
          }
          else /* Requested PLMN does not match the acquired PLMN */
          {
            /* -----------------------------------------------------------
            ** PLMN Changed, the Access stratum might have performed any
            ** Cell selection due to a request for a LIMITED SERVICE
            ** for Emergency calls
            **----------------------------------------------------------*/
            if ((mmcoord_pending_reg_message.cmd.hdr.message_id == (int)MMR_REG_REQ) &&
                ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode
                 == SYS_NETWORK_SELECTION_MODE_LIMITED) ||
                 (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode 
                  == SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION) ||
                (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode
                  == SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY) ||
                (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode
                  == SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY)))          
            {
              MSG_HIGH_DS_0(MM_SUB,"=MM= Limited service req ");
#ifdef FEATURE_NAS_ECALL
           if ((mm_ecall_mode_type == MMR_ECALL_MODE_ECALL_ONLY) &&
                       (((mm_timer_status[TIMER_T3242] == TIMER_STOPPED) &&
                          (mm_timer_status[TIMER_T3243] == TIMER_STOPPED)) ||
                          (mm_is_lu_triggered_in_ecall_mode == FALSE)))
           {
              mm_substate_control( MM_STD_ECALL_INACTIVE );
           }
          else
#endif
           {
              mm_substate_control( MM_STD_LIMITED_SERVICE );
           }
      
              /* Set the Serving PLMN service status to Limited */
              mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                               mm_serving_plmn.info.reg_service_domain,
                                               TRUE,
                                               SYS_SRV_STATUS_LIMITED,
                                               TRUE );            
      
              if (mm_reg_waiting_for_reg_cnf)
              {
                if (mm_managed_roaming_enabled)
                {
                    mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_LIMITED;
                }
      #ifdef FEATURE_ENHANCED_NW_SELECTION
                if (reg_sim_read_ens_flag())
                {
                    mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_LIMITED;
                }
      #endif
                
                /* Set the REG CNF plmn service state to the Serving PLMN info */
                mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
      
                /* Send the Confirmation to the REG task */
                mm_send_mmr_reg_cnf();
      
                mm_reg_waiting_for_reg_cnf = FALSE;
                mm_ready_to_send_reg_cnf   = FALSE;
                /* ----------------------------------------------
                ** Check for a pending request for emergency call
                ** ---------------------------------------------- */
                if ((mm_follow_on_request) &&
                    (mm_held_cnm_service_req.present) &&
                    (mm_held_cnm_service_req.establishment_cause == (int)MM_RR_EMERGENCY_CALL))
                {
                    mm_check_pending_requests();
                }
              }
            
            }
            else if(
#ifdef FEATURE_SGLTE
                  (mm_is_sglte_ue_mode_change_pended==TRUE)||
#endif
                  (policy_change_pending == TRUE) ||
                  (cm_per_subs_is_reg_allowed(mm_serving_plmn.info.plmn,
                     mm_serving_plmn.info.active_rat,(sys_modem_as_id_e_type)mm_as_id,mm_set_cm_stack_id(mm_as_id)) == FALSE))
            {
              mmcoord_capability_chg_pend_limited_service(
                               policy_change_pending, TRUE
                      );
          }
            else
            {
              MSG_ERROR_DS(MM_SUB,"RRC Acquired PLMN or CSG that is not requested", 0,0,0 );
            }
          }
      
          /* Start the T3212 timer only if the ms_op_mode is it is only CS mode */
          if (mm_serving_plmn.ms_op_mode != SYS_SRV_DOMAIN_PS_ONLY)
          {
            /* Handle T3212 */
            mmumts_process_T3212(message);
          }
          break;
      
        default:
          MSG_ERROR_DS(MM_SUB, "Unexpected service_status %d",
                message->cmd.rrc_service_cnf.service_status, 0,0 );
          break;
      } /* end switch (((rrc_service_cnf_T *) message)->service_status) */
      
      
      #ifdef FEATURE_DSAC
      mm_cs_access_barred_to_unbarred = FALSE;
      mm_ps_access_barred_to_unbarred = FALSE;
      #endif 
      
      #ifdef FEATURE_GSTK
          if (message->cmd.rrc_service_cnf.service_status == MM_AS_SERVICE_AVAILABLE)
          {
             if ((mm_state == MM_IDLE) && ((mm_idle_substate == MM_LIMITED_SERVICE) || (mm_idle_substate == MM_NO_IMSI)))
             {
                   byte lai[LAI_SIZE] = { 0xff, 0xff, 0xff, 0, 0 };
      
                   lai[0] = mm_serving_plmn.info.plmn.identity[0];
                   lai[1] = mm_serving_plmn.info.plmn.identity[1];
                   lai[2] = mm_serving_plmn.info.plmn.identity[2];
                   lai[3] = mm_serving_plmn.lac.lac[0];
                   lai[4] = mm_serving_plmn.lac.lac[1];
      
                   if( memcmp(lai, mm_gstk_previous_serving_lai, LAI_SIZE) != 0) /* New LAI */
                   {
                     mm_send_gstk_mm_idle_info() ;
                   }
             }
          }
      #endif
      
      /* Log the Cell info */
      mmsend_mm_info_log_packet();
#ifdef FEATURE_FEMTO_CSG
      mm_serving_csg_id = mm_serving_plmn.info.csg_info.csg_id;
#endif 

    }
  }
  else
  {
    MSG_FATAL_DS(MM_SUB, "Unexpected msg %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_service_cnf() */

#ifdef FEATURE_WRLF_SYSTEM_SEL

/*===========================================================================

FUNCTION mmcoord_handle_wrlf_rr_plmn_select_cnf

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_handle_wrlf_rr_plmn_select_cnf(mm_cmd_type *message )
{
  inter_task_lai_T       found_lai;
  boolean is_cell_forbidden = TRUE;
  boolean is_nonforbid_plmn = TRUE;
  boolean routine_man_selection_failed = FALSE;
  boolean lu_required = FALSE, attach_required = FALSE;

  mm_waiting_for_service_cnf = FALSE;
  mm_serv_change_pending     = FALSE;
  mmcoord_mmr_reg_cnf.ue_oos = message->cmd.rr_plmn_select_cnf.ue_oos;
  MSG_HIGH_DS_1(MM_SUB ,"=MM= ue_oos flag is %d", message->cmd.rr_plmn_select_cnf.ue_oos);
  mm_serving_plmn.hsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
  
#ifdef FEATURE_FEMTO_CSG
   mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif       
    if(mm_disable_g_upon_fallback &&
      (reg_state_get_mcs_pri() == TRUE) && 
     (mm_timer_status[GSM_EXTENDED_BLOCK_TIMER] == TIMER_ACTIVE) &&
     (cm_get_operator_info((sys_modem_as_id_e_type)mm_sub_id) == OPERATOR_CU) &&
     (MM_IS_NON_DDS_SUB))
    {
      message->cmd.rr_plmn_select_cnf.service_state = MM_AS_NO_SERVICE;
      message->cmd.rr_plmn_select_cnf.available_plmn_list.length = 0;
    }
  switch (message->cmd.rr_plmn_select_cnf.service_state)
  {
    case MM_AS_NO_SERVICE:
      MSG_HIGH_DS_0(MM_SUB,"=MM= Revd RR_PLMN_SELECT_CNF, NO SERVICE RLF state");

      mm_serving_plmn.info.plmn.identity[0] = 0xFF;
      mm_serving_plmn.info.plmn.identity[1] = 0xFF;
      mm_serving_plmn.info.plmn.identity[2] = 0xFF;
      mm_serving_plmn.lac.lac[0]       = 0xFF;
      mm_serving_plmn.lac.lac[1]       = 0xFF;
      mm_serving_plmn.network_op_mode = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
      mm_serving_plmn.gmm_service_state   = SERVICE_NOT_AVAILABLE;
      mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
      mm_serving_plmn.info.no_service_cause = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
      mm_serving_plmn.rac = 0xFF ;
#ifdef FEATURE_GSM_DTM
      mm_serving_plmn.dtm_supported = FALSE;
#endif

#ifdef FEATURE_DUAL_SIM
      if (message->cmd.rr_plmn_select_cnf.failure_cause == MM_RR_NO_RESOURCE_AVAILABLE)
      {
        mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_NO_RESOURCE;
      }
      else
      {
        mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
      }
#elif defined (FEATURE_1XSRLTE)
      mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif

      mm_serving_plmn.egprs_supported = FALSE;
      mm_mmr_cell_service_ind_pending = FALSE;
      mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                                mm_serving_plmn.info.reg_service_domain,
                                                FALSE,
                                                SYS_SRV_STATUS_NO_SRV,
                                                FALSE );
      gmm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                           mm_serving_plmn.info.reg_service_domain,
                                           FALSE,
                                           SYS_SRV_STATUS_NO_SRV,
                                           FALSE );
      if (mm_reg_waiting_for_reg_cnf)
      {
        dword                                  entry;
        dword                                  num_plmns;
        num_plmns  = message->cmd.rr_plmn_select_cnf.available_plmn_list.length;
        mmcoord_mmr_reg_cnf.available_plmn_list.length = num_plmns;
 
        for (entry = 0; entry < num_plmns; entry++)
        {
          mmcoord_mmr_reg_cnf.available_plmn_list.info[entry] =
             message->cmd.rr_plmn_select_cnf.available_plmn_list.info[entry];
        }
        mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;
        mmcoord_mmr_reg_cnf.service_state  = mm_serving_plmn.info;
        mm_send_mmr_reg_cnf();
        mm_reg_waiting_for_reg_cnf = FALSE;
        mm_ready_to_send_reg_cnf   = FALSE;
      }
      break;

    case MM_AS_SERVICE_AVAILABLE:
      MSG_HIGH_DS_1(MM_SUB ,"=MM= Revd RR_PLMN_SELECT_CNF w/ SRV-AC=%d RLF state",
               message->cmd.rr_plmn_select_cnf.cell_access);
      
      mm_system_information.ATT = message->cmd.rr_plmn_select_cnf.ATT;
      mm_system_information.mscr = message->cmd.rr_plmn_select_cnf.MSCR;
      mm_band_indicator = message->cmd.rr_plmn_select_cnf.band_indicator;
#ifdef FEATURE_GSM_GPRS
      if (message->cmd.rr_plmn_select_cnf.gprs_sys_info_present)
      {
      mm_system_information.sgsnr = message->cmd.rr_plmn_select_cnf.gprs_sys_info.sgsnr;
      }
      else
      {
      mm_system_information.sgsnr = FALSE;
      }
#endif
      /* Store the cell_access information */
      mm_system_information.cell_access = message->cmd.rr_plmn_select_cnf.cell_access;
      
      /* -----------------------------------------------------
      ** First store the PLMN where we are currently camped so
      ** we can compare with requested PLMN from MMR_REG_REQ
      ** ----------------------------------------------------- */
      mm_serving_plmn.info.plmn.identity[0] =
      message->cmd.rr_plmn_select_cnf.location_area_identification.PLMN_id.identity[0];
      mm_serving_plmn.info.plmn.identity[1] =
      message->cmd.rr_plmn_select_cnf.location_area_identification.PLMN_id.identity[1];
      mm_serving_plmn.info.plmn.identity[2] =
      message->cmd.rr_plmn_select_cnf.location_area_identification.PLMN_id.identity[2];
      
      memscpy ( mm_serving_plmn.lac.lac, LAC_SIZE, message->cmd.rr_plmn_select_cnf.
      location_area_identification.location_area_code, LAC_SIZE );
      
      mm_serving_plmn.info.plmn_service_capability =
      message->cmd.rr_plmn_select_cnf.service_capability;
      
      mm_serving_plmn.gmm_service_state    = SERVICE_NOT_AVAILABLE;
      mm_serving_plmn.network_op_mode  = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
      mm_serving_plmn.rac = 0xFF;
      
#ifdef FEATURE_GSM_GPRS
      if (message->cmd.rr_plmn_select_cnf.gprs_sys_info_present)
      {
        mm_serving_plmn.network_op_mode =
          message->cmd.rr_plmn_select_cnf.gprs_sys_info.nmo;
        mm_serving_plmn.rac             =
        message->cmd.rr_plmn_select_cnf.gprs_sys_info.rac;
      
        /* On receving service cnf always send cell service indication*/
        mm_mmr_cell_service_ind_pending = TRUE;
      
#ifdef FEATURE_GSM_DTM
        mm_serving_plmn.dtm_supported   =
          message->cmd.rr_plmn_select_cnf.gprs_sys_info.dtm_supported;
        MSG_HIGH_DS_1(MM_SUB ,"=MM= DTM=%d",mm_serving_plmn.dtm_supported);
#endif
        mm_serving_plmn.egprs_supported   =
          message->cmd.rr_plmn_select_cnf.gprs_sys_info.egprs_supported;
      
        MSG_HIGH_DS_3(MM_SUB, "=MM= GMM sys info: NMO=%d, RAC=%d, EGPRS=%d",
                   mm_serving_plmn.network_op_mode, mm_serving_plmn.rac, mm_serving_plmn.egprs_supported);
      
      }
      else
#endif /* #ifdef FEATURE_GSM_GPRS */
      { 
        mm_mmr_cell_service_ind_pending = TRUE;
      
 #ifdef FEATURE_GSM_DTM
        mm_serving_plmn.dtm_supported     = FALSE;
#endif
        mm_serving_plmn.egprs_supported   = FALSE;
      }

      mm_serving_plmn.mm_service_state = SERVICE_AVAILABLE;
      mm_serving_plmn.t3122_running =  message->cmd.rr_plmn_select_cnf.t3122_running;
      
      MSG_HIGH_DS_1(MM_SUB ,"=MM= t3122_running is %d",message->cmd.rr_plmn_select_cnf.t3122_running);

      found_lai.PLMN_id.identity[0] = mm_serving_plmn.info.plmn.identity[0];
      found_lai.PLMN_id.identity[1] = mm_serving_plmn.info.plmn.identity[1];
      found_lai.PLMN_id.identity[2] = mm_serving_plmn.info.plmn.identity[2];
      
      found_lai.location_area_code[0] = mm_serving_plmn.lac.lac[0];
      found_lai.location_area_code[1] = mm_serving_plmn.lac.lac[1];

      if ((!mm_check_forbidden_national_roaming( &found_lai )) &&
          (!mm_check_forbidden_regional_roaming( &found_lai )) &&
#ifdef FEATURE_DUAL_SIM
          (!reg_sim_per_subs_plmn_forbidden( mm_serving_plmn.info.plmn,(sys_modem_as_id_e_type)mm_sub_id)) &&
          (!(reg_sim_plmn_gprs_forbidden(mm_serving_plmn.info.plmn,(sys_modem_as_id_e_type)mm_sub_id) &&
#else
          (!reg_sim_plmn_forbidden( mm_serving_plmn.info.plmn )) &&
          (!(reg_sim_plmn_gprs_forbidden(mm_serving_plmn.info.plmn ) &&
#endif        
          (mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_PS_ONLY))))
      {
        is_cell_forbidden = FALSE;
        MSG_HIGH_DS_0(MM_SUB,"=MM= Cell is NOT FORBIDDEN");
      }

      if (!((PLMN_MATCH( found_lai.PLMN_id.identity,
                         mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn.identity )) ||
            (sys_eplmn_list_equivalent_plmn(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) && 
             sys_eplmn_list_equivalent_plmn(mm_serving_plmn.info.plmn)))  &&
          (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
            SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION))
      {
        routine_man_selection_failed = TRUE;
        MSG_HIGH_DS_0(MM_SUB,"=MM= Different PLMN found during MAN_LIMITED_ACQ");
      }

      if ((routine_man_selection_failed) ||
         ((((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                            SYS_NETWORK_SELECTION_MODE_AUTOMATIC )||
          (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                            SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)) ||
          ((!gmm_manual_mode_user_selection_procedure)&&
           ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                            SYS_NETWORK_SELECTION_MODE_MANUAL) ||
            (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                            SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION))))&& 
         (is_cell_forbidden)))
      {
        is_nonforbid_plmn = FALSE;
#ifdef FEATURE_GSM_GPRS
        if ((gmm_state == GMM_REGISTERED) && (gmmllc_get_glif_state() == GMM_IDLE_STATE))
        {
          gmmllc_reinit(GMM_STANDBY_STATE);
        }
         /* Suspend LLC until GMM stays in LIMITED SERVICE */
        gmmllc_gmm_trigger_llc_event(GMM_TRIGGER_LLC_SUSPENSION,FALSE,0) ;
        mm_llc_needs_to_be_resumed = TRUE;
#endif /* #ifdef FEATURE_GSM_GPRS */
      }

      /*
      ** Check for access class blocking restrictions and stop a LU procedure if necessary
      */
      lu_required = mm_check_if_cs_cell_access_allowed(mm_get_cs_cell_access(PROCEDURE_CS_REGISTRATION));
      attach_required = gmm_check_if_ps_cell_access_allowed(gmm_get_ps_cell_access(PROCEDURE_PS_REGISTRATION));

      if(((is_nonforbid_plmn) &&(lu_required || attach_required)) ||
          (((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                            SYS_NETWORK_SELECTION_MODE_LIMITED) ||
             (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                            SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION) ||
             (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                            SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION)) &&
           (mmcoord_pending_reg_message.cmd.mmr_reg_req.cs_for_request == TRUE)))
      {
#ifdef FEATURE_WCDMA
        rrc_cmd_type *rrc_cmd_p = mm_get_rrc_cmd_buf(
                                    mm_wrlf_serving_plmn_ptr->info.active_rat );
           if (rrc_cmd_p == NULL)
           {
             MSG_FATAL_DS(MM_SUB, "ERROR: Unable to allocate memory", 0,0,0 );
           }
        mmcoord_set_wrlf_state(WCDMA_RLF_SERVICE_CNF_PENDING);

           if(( mm_wrlf_pending_rr_message_ptr == NULL ) && 
              ((mm_wrlf_pending_rr_message_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) 
                       == NULL))
           {
             mm_check_for_null_ptr((void*)mm_wrlf_pending_rr_message_ptr);
           }
           memscpy( mm_wrlf_pending_rr_message_ptr, sizeof(mm_cmd_type), message, sizeof(mm_cmd_type));
#ifdef FEATURE_WCDMA
        rrc_cmd_p->cmd_hdr.cmd_id = RRC_ABORT_REQ;
        rrc_cmd_p->cmd.rrc_abort_req.auth_failed_bar_cells = FALSE;
        rrc_cmd_p->cmd.rrc_abort_req.delay_sig_con_release = FALSE;
        rrc_cmd_p->cmd.rrc_abort_req.hard_abort = TRUE;
        rrc_cmd_p->cmd.rrc_abort_req.abort_cause = RRC_OTHERS;
        MSG_HIGH_DS_0(MM_SUB,"=MM= WCDMA RLF ABORTING STATE ");
        send_cmd_to_rrc( rrc_cmd_p, mm_wrlf_serving_plmn_ptr->info.active_rat );
#endif
#endif /* FEATURE_WCDMA */
      }
      else
      {
        MSG_HIGH_DS_1(MM_SUB ,"=MM= PLMN changed, Limited service req OR Net_sel_mode = %d  or Access call barred", 
        mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode);
  
        /* Set the Serving PLMN service status to Limited */
        mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                         mm_serving_plmn.info.reg_service_domain,
                                         TRUE,
                                         SYS_SRV_STATUS_LIMITED,
                                         FALSE );            
         mm_send_mmr_camped_ind(FALSE);
         mm_rat_handle_srv_status_update ( SYS_SRV_STATUS_SRV );
        if (mm_reg_waiting_for_reg_cnf)
        {
          /* Set the REG CNF plmn service state to the Serving PLMN info */
          mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
  
          /* Send the Confirmation to the REG task */
          mm_send_mmr_reg_cnf();
  
          mm_reg_waiting_for_reg_cnf = FALSE;
          mm_ready_to_send_reg_cnf   = FALSE;
        }
      }
      break;
  }
}
#endif


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RR_PLMN_SELECT_CNF

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rr_plmn_select_cnf( mm_cmd_type *message )
{
  sys_plmn_id_s_type        requested_plmn;
  sys_srv_capability_e_type requested_service_type;
#ifdef FEATURE_SGLTE
  boolean mm_is_sglte_ue_mode_change_pended = FALSE;
#endif
  boolean policy_change_pending = FALSE; 
#ifdef FEATURE_LTE
  emm_ctrl_data_type *emm_ctrl_data_ptr_tag = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr_tag != NULL);
#endif  

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (message->cmd.hdr.message_id == (int) RR_PLMN_SELECT_CNF)
  {
#ifdef FEATURE_GSM_BAND_PREF
    mm_active_band = message->cmd.rr_plmn_select_cnf.band_pref;
#endif
    if (!mm_waiting_for_service_cnf || mm_waiting_for_deact_cnf || mm_nreg_req_received || mm_waiting_for_stop_mode_cnf 
              || mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ)
    {
      MSG_HIGH_DS_5( MM_SUB, "=MM= Unexpected RR_PLMN_SELECT_CNF: mm_waiting_for_service_cnf = %d, mm_waiting_for_deact_cnf = %d, mm_nreg_req_received = %d, mm_waiting_for_stop_mode_cnf = %d, mm_ps_detach_state = %d ",
                            mm_waiting_for_service_cnf,mm_waiting_for_deact_cnf,
                            mm_nreg_req_received, mm_waiting_for_stop_mode_cnf, mm_ps_detach_state);
    }

#ifdef FEATURE_WRLF_SYSTEM_SEL
    else if(mm_wcdma_rlf_state == WCDMA_RLF_STARTED)
    {
      mmcoord_handle_wrlf_rr_plmn_select_cnf(message);
    }
#endif
#ifdef FEATURE_SGLTE
    else if ((message->cmd.rr_plmn_select_cnf.error_code == MM_AS_INTERRAT_HANDOVER) &&
          (message->cmd.rr_plmn_select_cnf.service_state == MM_AS_NO_SERVICE))
    {
      MSG_HIGH_DS_1( MM_SUB, "=MM= Received RR_PLMN_SELECT_CNF, NO SERVICE error_code %d", 
                                message->cmd.rr_plmn_select_cnf.error_code);

      mm_waiting_for_service_cnf = FALSE;
      mm_serv_change_pending     = FALSE;
      mmcoord_mmr_reg_cnf.ue_oos = FALSE;
#if defined(FEATURE_WRLF_SYSTEM_SEL) || defined(FEATURE_LTE)
      mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
#endif
      mm_substate_control( MM_STD_NO_SERVICE );
      gmm_set_substate( GMM_NO_CELL_AVAILABLE );
    }
#endif
    else
    {
      mm_waiting_for_service_cnf = FALSE;
      mm_serv_change_pending     = FALSE;
      mmcoord_mmr_reg_cnf.ue_oos = message->cmd.rr_plmn_select_cnf.ue_oos;
      MSG_HIGH_DS_1(MM_SUB ,"=MM= ue_oos flag is %d", message->cmd.rr_plmn_select_cnf.ue_oos);
#ifdef FEATURE_EOOS
      #error code not present
#endif

      mm_serving_plmn.hsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
      
#if defined(FEATURE_WRLF_SYSTEM_SEL) || defined(FEATURE_LTE)
      mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
#endif

#ifdef FEATURE_NAS_FREE_FLOAT
      gmmllc_reset_service_state();
#endif

#ifdef FEATURE_FEMTO_CSG
       mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif       

  if(mm_disable_g_upon_fallback &&
     (reg_state_get_mcs_pri() == TRUE) && 
     (mm_timer_status[GSM_EXTENDED_BLOCK_TIMER] == TIMER_ACTIVE) &&
     (cm_get_operator_info((sys_modem_as_id_e_type)mm_sub_id) == OPERATOR_CU) &&
     (MM_IS_NON_DDS_SUB))
    {
      message->cmd.rr_plmn_select_cnf.service_state = MM_AS_NO_SERVICE;
      message->cmd.rr_plmn_select_cnf.available_plmn_list.length = 0;
    }



      /* --------------------------------------------------
      ** Handle the response from RRC after a LAI selection.
      ** Success or failure is reported to REG.  If a failure
      ** RRC reports the current list of LAIs.
      ** ---------------------------------------------------- */
      switch (message->cmd.rr_plmn_select_cnf.service_state)
      {
        case MM_AS_NO_SERVICE:

          MSG_HIGH_DS_0(MM_SUB, "=MM= Received RR_PLMN_SELECT_CNF, NO SERVICE");

          mm_serving_plmn.info.plmn.identity[0] = 0xFF;
          mm_serving_plmn.info.plmn.identity[1] = 0xFF;
          mm_serving_plmn.info.plmn.identity[2] = 0xFF;
          mm_serving_plmn.lac.lac[0]       = 0xFF;
          mm_serving_plmn.lac.lac[1]       = 0xFF;
          mm_serving_plmn.network_op_mode = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
          mm_serving_plmn.gmm_service_state   = SERVICE_NOT_AVAILABLE;
          mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
          mm_serving_plmn.rac = 0xFF ;
#ifdef FEATURE_SGLTE
         if(MM_SUB_IS_SGLTE &&
            (mmcoord_pending_reg_message.cmd.hdr.message_id == (int)MMR_REG_REQ) &&
            ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                    SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY) ||
            (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                    SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY)) )
          {
            mm_serving_plmn.info.plmn_reg_type = SYS_REG_DOMAIN_NO_SERVICE;
          }
#endif
#ifdef FEATURE_DUAL_SIM
          if (message->cmd.rr_plmn_select_cnf.failure_cause == MM_RR_NO_RESOURCE_AVAILABLE)
          {
            mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_NO_RESOURCE;
          }
          else
          {
            mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
          }
#elif defined (FEATURE_1XSRLTE)
          mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif
#ifdef FEATURE_GSM_DTM
          mm_serving_plmn.dtm_supported = FALSE;
#endif
          mm_serving_plmn.egprs_supported = FALSE;
          mm_mmr_cell_service_ind_pending = FALSE;
          mm_coord_camped_cell_id = 0xFFFF;

          /* ------------------------------------------------------------------
          ** Pass RR_PLMN_SELECT_CNF to both MM & GMM independent of MS op mode
          ** ------------------------------------------------------------------ */
          mm_handle_message( message );
#ifdef FEATURE_GSM_GPRS
          gmm_handle_message( message );
#endif /* #ifdef FEATURE_GSM_GPRS */

          break;

        case MM_AS_SERVICE_AVAILABLE:

#ifdef FEATURE_LTE          
          emm_rat_change_stop_timers();
          gmm_reset_mcs_client_info();
#endif
	  sys_eplmn_per_nas_region_update(FALSE);
          /* Update current camped cell id*/
          mm_serving_plmn.arfcn = message->cmd.rr_plmn_select_cnf.arfcn;
          mm_serving_plmn.bsic = message->cmd.rr_plmn_select_cnf.bsic;
          if(message->cmd.rr_plmn_select_cnf.cell_identity_valid)
          {
            mm_coord_camped_cell_id  = message->cmd.rr_plmn_select_cnf.cell_identity;
          }
          else
          {
            mm_coord_camped_cell_id = 0xFFFF;
          }
		  gmm_force_for_always_on_data=FALSE;
		  mm_dds_switch_while_rrc_conn_active=FALSE;
          MSG_HIGH_DS_5(MM_SUB,"=MM= Received RR_PLMN_SELECT_CNF w/ SRV-AC=%d, camped ARFCN NUM is %d, Band is %d, BSIC is %d, t3122_running is %d", 
					  message->cmd.rr_plmn_select_cnf.cell_access,mm_serving_plmn.arfcn.num,
					  mm_serving_plmn.arfcn.band,mm_serving_plmn.bsic,
                      message->cmd.rr_plmn_select_cnf.t3122_running);

          mm_system_information.ATT = message->cmd.rr_plmn_select_cnf.ATT;
          mm_system_information.mscr = message->cmd.rr_plmn_select_cnf.MSCR;
          mm_band_indicator = message->cmd.rr_plmn_select_cnf.band_indicator;
#ifdef FEATURE_GSM_GPRS
          if (message->cmd.rr_plmn_select_cnf.gprs_sys_info_present)
          {
            mm_system_information.sgsnr = message->cmd.rr_plmn_select_cnf.gprs_sys_info.sgsnr;
          }
#endif
          /* Store the cell_access information */
          mm_system_information.cell_access = message->cmd.rr_plmn_select_cnf.cell_access;
          /* -----------------------------------------------------
          ** First store the PLMN where we are currently camped so
          ** we can compare with requested PLMN from MMR_REG_REQ
          ** ----------------------------------------------------- */
          mm_serving_plmn.info.plmn.identity[0] =
            message->cmd.rr_plmn_select_cnf.location_area_identification.PLMN_id.identity[0];
          mm_serving_plmn.info.plmn.identity[1] =
            message->cmd.rr_plmn_select_cnf.location_area_identification.PLMN_id.identity[1];
          mm_serving_plmn.info.plmn.identity[2] =
            message->cmd.rr_plmn_select_cnf.location_area_identification.PLMN_id.identity[2];

          memscpy ( mm_serving_plmn.lac.lac, LAC_SIZE, message->cmd.rr_plmn_select_cnf.
            location_area_identification.location_area_code, LAC_SIZE );



          mm_serving_plmn.info.plmn_service_capability =
            message->cmd.rr_plmn_select_cnf.service_capability;

          mm_serving_plmn.gmm_service_state    = SERVICE_NOT_AVAILABLE;
          mm_serving_plmn.network_op_mode  = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
          mm_serving_plmn.rac = 0xFF;
#if defined FEATURE_DUAL_SIM || defined FEATURE_1XSRLTE
          mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif

#ifdef FEATURE_GSM_GPRS
          if (message->cmd.rr_plmn_select_cnf.gprs_sys_info_present)
          {
            mm_serving_plmn.network_op_mode =
              message->cmd.rr_plmn_select_cnf.gprs_sys_info.nmo;
            mm_serving_plmn.rac             =
              message->cmd.rr_plmn_select_cnf.gprs_sys_info.rac;

            /* On receving service cnf always send cell service indication*/
            mm_mmr_cell_service_ind_pending = TRUE;

#ifdef FEATURE_GSM_DTM
            mm_serving_plmn.dtm_supported   =
              message->cmd.rr_plmn_select_cnf.gprs_sys_info.dtm_supported;
#endif
            mm_serving_plmn.egprs_supported   =
              message->cmd.rr_plmn_select_cnf.gprs_sys_info.egprs_supported;

            MSG_HIGH_DS_4( MM_SUB, "=MM= DTM=%d and GMM sys info: NMO=%d, RAC=%d, EGPRS=%d",
                       message->cmd.rr_plmn_select_cnf.gprs_sys_info.dtm_supported, 
					   mm_serving_plmn.network_op_mode, mm_serving_plmn.rac, mm_serving_plmn.egprs_supported);

            mm_serving_plmn.gmm_service_state = SERVICE_AVAILABLE;
          }
          else
#endif /* #ifdef FEATURE_GSM_GPRS */
          { 
            mm_mmr_cell_service_ind_pending = TRUE;
    
  #ifdef FEATURE_GSM_DTM
            mm_serving_plmn.dtm_supported     = FALSE;
  #endif
            mm_serving_plmn.egprs_supported   = FALSE;
          }

          mm_serving_plmn.t3122_running =  message->cmd.rr_plmn_select_cnf.t3122_running;

#ifdef FEATURE_LTE
          if((gmm_state == GMM_DEREGISTERED || mm_possible_imsi_attach == TRUE) &&
              (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                 SYS_NETWORK_SELECTION_MODE_LIMITED) && 
              (mm_serving_plmn.info.sim_state != SYS_SIM_STATE_CS_PS_INVALID) )
          {
            if(emm_validate_attach_plmn(emm_ctrl_data_ptr_tag, &mm_serving_plmn.info.plmn) == FALSE)
            {
              mm_update_mm_serving_plmn_info( SYS_SIM_STATE_CS_PS_INVALID,
                                                SYS_SRV_DOMAIN_NO_SRV,
                                                TRUE,
                                                SYS_SRV_STATUS_LIMITED,
                                                FALSE);

#ifdef FEATURE_DUAL_SIM  
              mm_gmm_set_non_illegal_cause();
#endif

            }
          }
#ifdef FEATURE_LTE_REL9
          if((mm_is_reg_needed_for_mmit_on_GW()) &&
             (mm_service_loss_on_multimode_rat == SYS_RAT_LTE_RADIO_ACCESS))
          {
            MSG_HIGH_DS_0(MM_SUB,"=EMM= RAU/LAU to be performed - Mobility Management for IMS voice termination");
            emm_set_and_write_tin_type(NAS_MM_TIN_GUTI);
            mm_pending_lu_request = LTE_IRAT_LU;
          }
#endif
          mmcoord_locally_deactivate_isr_if_needed();
#endif
          policy_change_pending = mmcoord_policy_change_pending();
          /* ------------------------------------------------------------------
          ** Pass RR_PLMN_SELECT_CNF to both MM & GMM independent of MS op mode
          ** ------------------------------------------------------------------ */
#ifdef FEATURE_GSM_GPRS

          if ((gmm_state == GMM_REGISTERED) && (gmmllc_get_glif_state() == GMM_IDLE_STATE))

          {

            gmmllc_reinit(GMM_STANDBY_STATE);

          }

#endif /* #ifdef FEATURE_GSM_GPRS */
          
          mm_handle_message( message );
#ifdef FEATURE_GSM_GPRS
          gmm_handle_message( message );
#endif /* #ifdef FEATURE_GSM_GPRS */
          mm_send_mmr_cell_access_ind();
#ifdef FEATURE_LTE
          mmcoord_reevaluate_rat_pri_list(mm_serving_plmn.info.plmn);
          if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
          {
            mm_send_rr_irat_allowed_status_ind();
          }          
         /*
           4.7.5.1 Normal and periodic routing area updating procedure
           in Iu mode and A/Gb mode, after intersystem change from S1 mode, and the GMM receives an indication of "RRC connection failure" 
           from lower layers due to lower layer while in S1 mode;
         */
          if(emm_ctrl_data_ptr->rau_pending_ll_failure == TRUE)
          {
            mm_pend_lu_rau_on_ll_failure_in_lte();
          }
#endif
 
#ifdef FEATURE_NAS_RAU_ON_MS_RAC_CHANGE
          if((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                                          SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)&&
              (sys_plmn_id_is_undefined(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) == TRUE)&&
              (mmcoord_check_if_ms_rat_capability_changed() == TRUE))
          {
            gmm_ra_update_pending = TRUE;
          }
#endif

          /* ------------------------------------------------
          ** Confirm requested PLMN matches the acquired PLMN
          ** ------------------------------------------------ */
          requested_plmn = mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn;

#ifdef FEATURE_SGLTE
          if (( mm_is_sglteplmn_in_sglte_roam(mm_serving_plmn.info.plmn) == TRUE) || 
          (mm_process_if_both_stack_has_camped_non_sglte_plmn(mm_cs_stack_id)==TRUE)||  
          (mm_is_non_sglte_plmn_in_sglte_home(mm_serving_plmn.info.plmn)==TRUE))
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= SGLTE SGLTE UE MODE change Pending ") ;
            mm_is_sglte_ue_mode_change_pended = TRUE;
          }
          else if (mm_sglte_state4_trigger_split()==TRUE)
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= Spliting in State 4 ,GSM CS only ") ;
            return;
          }
#endif

          if ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                SYS_NETWORK_SELECTION_MODE_LIMITED) &&
              (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY) &&
              (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY) &&                
               ((mm_plmn_match(mm_serving_plmn.info.plmn, requested_plmn) ||
                  (reg_mode_get_hplmn_to_selected(
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
                                                   (sys_modem_as_id_e_type)mm_as_id
#endif
                                                  ) &&
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_EQUIVALENT_HPLMN
                   (reg_sim_per_subs_is_ehplmn(mm_serving_plmn.info.plmn,(sys_modem_as_id_e_type)mm_sub_id) )
#else
                   (sys_plmn_match(reg_sim_per_subs_read_hplmn((sys_modem_as_id_e_type)mm_sub_id), mm_serving_plmn.info.plmn) )
#endif
#else
#ifdef FEATURE_EQUIVALENT_HPLMN
                   (reg_sim_is_ehplmn(mm_serving_plmn.info.plmn) )
#else
                   (sys_plmn_match(reg_sim_read_hplmn(), mm_serving_plmn.info.plmn) )
#endif
#endif             
                  )) ||
                (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                   SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION))
#ifdef FEATURE_SGLTE
                 &&(mm_is_sglte_ue_mode_change_pended == FALSE)
#endif
                 &&(policy_change_pending == FALSE)
                 &&(cm_per_subs_is_reg_allowed(mm_serving_plmn.info.plmn,
                     mm_serving_plmn.info.active_rat,(sys_modem_as_id_e_type)mm_as_id,mm_set_cm_stack_id(mm_as_id)) == TRUE))
          {
            /* --------------------------------------------------------------------
            ** Confirm cell's service capabilty supports the requested service type
            ** -------------------------------------------------------------------- */
            if (mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain ==
                  SYS_SRV_DOMAIN_CS_ONLY)
            {
              requested_service_type = SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
            }
            else if (mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain ==
                       SYS_SRV_DOMAIN_PS_ONLY)
            {
              requested_service_type = SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
            }
            else if (mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain ==
                       SYS_SRV_DOMAIN_CS_PS)
            {
              requested_service_type = SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              MSG_ERROR_DS( MM_SUB, "=MM= Unexpected req_service_domain %d",
                    mmcoord_pending_reg_message.cmd.mmr_reg_req.req_service_domain, 0,0 );
              requested_service_type = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
            }

            if ((mm_serving_plmn.info.plmn_service_capability !=
                   SYS_SRV_CAPABILITY_CS_PS_SERVICE) &&
                ((mm_serving_plmn.info.plmn_service_capability != requested_service_type) &&
                 (mm_serving_plmn.ms_op_mode != SYS_SRV_DOMAIN_CS_PS)))
            {
              if (mm_reg_waiting_for_reg_cnf)
              {
                /* ------------------------------------------------------------
                ** PLMN found with service that does not match requested domain
                ** ------------------------------------------------------------ */
                mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                                mm_serving_plmn.info.reg_service_domain,
                                                TRUE,
                                                SYS_SRV_STATUS_LIMITED,
                                                TRUE );

                mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;

                mm_send_mmr_reg_cnf();

                mm_reg_waiting_for_reg_cnf       = FALSE;
                mm_ready_to_send_reg_cnf         = FALSE;
              }
              else
              {
                MSG_ERROR_DS( MM_SUB, "=MM= Unexpected RR_PLMN_SELECT_CNF", 0,0,0 );
              }
            }
            else /* (mm_serving_plmn.info.plmn_service_capability == req_service_domain) */
            {
              boolean lu_required = FALSE, attach_required = FALSE;
              /*
              ** Check for access class blocking restrictions and stop a LU procedure if necessary
              */
              lu_required = mm_check_if_cs_cell_access_allowed(mm_get_cs_cell_access(PROCEDURE_CS_REGISTRATION));
              attach_required = gmm_check_if_ps_cell_access_allowed(gmm_get_ps_cell_access(PROCEDURE_PS_REGISTRATION));
              
              /* ---------------------------------------------------------
              ** Now initiate registration attempt according to MS op mode
              ** --------------------------------------------------------- */
              if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
              {
#ifdef FEATURE_GSM_GPRS
                mm_ready_to_send_reg_cnf = TRUE;
                gmm_handle_plmn_found( mm_serving_plmn,
                                       attach_required ) ;
#else
                MSG_HIGH_DS_0(MM_SUB, "=MM= Feature GPRS is not supported") ;
#endif /* #ifdef FEATURE_GSM_GPRS */
              }
              else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)
              {
                mm_handle_plmn_found( mm_serving_plmn,
                                      lu_required );
              }
              else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
              {
                if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
                {
#ifdef FEATURE_GSM_GPRS
#ifdef FEATURE_LTE
                  if(mm_csfb_state == MM_CSFB_STATE_MO_CALL_ORIG ||
                       mm_csfb_state == MM_CSFB_STATE_MT_CALL_ORIG)
                  {
                    mm_handle_plmn_found( mm_serving_plmn, lu_required );
                  }
                  else
#endif
                  {
                    mm_ready_to_send_reg_cnf = TRUE;
                  } 
                  gmm_handle_plmn_found( mm_serving_plmn,
                                         attach_required ) ;
#else
                  MSG_HIGH_DS_0(MM_SUB, "=MM= Feature GPRS is not supported") ;
#endif /* #ifdef FEATURE_GSM_GPRS */
                }
                else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
                {
                  mm_handle_plmn_found( mm_serving_plmn,
                                        lu_required );
#ifdef FEATURE_GSM_GPRS
                  gmm_handle_plmn_found( mm_serving_plmn,
                                         attach_required ) ;
#else
                  MSG_HIGH_DS_0(MM_SUB, "=MM= Feature GPRS is not supported") ;
#endif /* #ifdef FEATURE_GSM_GPRS */
                }
              }
              else
              {
                MSG_ERROR_DS( MM_SUB, "=MM= Invalid MS op mode", 0,0,0 );
                /* Send rejection back to REG? */
              }
            }
          }
          else /* Requesed PLMN does not match the acquired PLMN */
          {
            /* -----------------------------------------------------------
            ** PLMN Changed, the Access stratum might have performed any
            ** Cell selection due to a request for a LIMITED SERVICE
            ** for Emergency calls
            **----------------------------------------------------------*/
            if ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode
                 == SYS_NETWORK_SELECTION_MODE_LIMITED) ||
                 (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode 
                 == SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION) ||
                (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                  SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY) ||
                (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                  SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY))
            {
              MSG_HIGH_DS_1(MM_SUB ,"=MM= PLMN changed, Limited service req OR Net_sel_mode = %d ", 
              mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode);
#ifdef FEATURE_NAS_ECALL
              if ((mm_ecall_mode_type == MMR_ECALL_MODE_ECALL_ONLY) &&
                 (((mm_timer_status[TIMER_T3242] == TIMER_STOPPED) &&
                (mm_timer_status[TIMER_T3243] == TIMER_STOPPED)) ||
                (mm_is_lu_triggered_in_ecall_mode == FALSE)))
              {
                  mm_substate_control( MM_STD_ECALL_INACTIVE );
              }
              else
#endif
              {
                 mm_substate_control( MM_STD_LIMITED_SERVICE );
              }


              /* Set the Serving PLMN service status to Limited */
              mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                               mm_serving_plmn.info.reg_service_domain,
                                               TRUE,
                                               SYS_SRV_STATUS_LIMITED,
                                               TRUE );            

              if (mm_reg_waiting_for_reg_cnf)
              {
                if (mm_managed_roaming_enabled)
                {
                    mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_LIMITED;
                }
#ifdef FEATURE_ENHANCED_NW_SELECTION
                if (reg_sim_read_ens_flag())
                {
                    mm_serving_plmn.info.cs_service_status = SYS_SRV_STATUS_LIMITED;
                }
#endif
                /* Set the REG CNF plmn service state to the Serving PLMN info */
                mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;

                /* Send the Confirmation to the REG task */
                mm_send_mmr_reg_cnf();

                mm_reg_waiting_for_reg_cnf = FALSE;
                mm_ready_to_send_reg_cnf   = FALSE;
                /* ----------------------------------------------
                ** Check for a pending request for emergency call
                ** ---------------------------------------------- */
                if ((mm_follow_on_request) &&
                    (mm_held_cnm_service_req.present) &&
                    (mm_held_cnm_service_req.establishment_cause == (int)MM_RR_EMERGENCY_CALL))
                {
                    mm_check_pending_requests();
                }
              }
            }
            else if (
#ifdef FEATURE_SGLTE
            (mm_is_sglte_ue_mode_change_pended==TRUE)||
#endif
            (policy_change_pending == TRUE) ||
            (cm_per_subs_is_reg_allowed(mm_serving_plmn.info.plmn,
                     mm_serving_plmn.info.active_rat,(sys_modem_as_id_e_type)mm_as_id,mm_set_cm_stack_id(mm_as_id)) == FALSE))
            {
            mmcoord_capability_chg_pend_limited_service(
                             policy_change_pending, TRUE
                    );
          }
          else
          {
            MSG_ERROR_DS(MM_SUB, "=MM= RR Acquired PLMN that is not requested",0,0,0);
          }
         }

          /* Check if PLMN,RAC and LAC informed to REG is same as serving PLMN and cell service indication
           is pending then send cell service indication  */
          if ((mm_mmr_cell_service_ind_pending) && 
               (mmcoord_mmr_reg_cnf.only_for_reg == FALSE) &&
               (sys_plmn_match( mmcoord_mmr_reg_cnf.service_state.plmn, mm_serving_plmn.info.plmn)) &&
               (mmcoord_mmr_reg_cnf.rac == mm_serving_plmn.rac) 
#ifdef FEATURE_GSM_BAND_PREF
               &&(mmcoord_mmr_reg_cnf.service_state.lac == b_unpackw(mm_serving_plmn.lac.lac,0,16))
#endif /*FEATURE_GSM_BAND_PREF*/
               &&(mmcoord_mmr_reg_cnf.service_state.active_rat == mm_serving_plmn.info.active_rat))
          {
            mm_send_mmr_cell_service_ind();     
          }
#ifdef FEATURE_WRLF_SYSTEM_SEL
          mmcoord_set_wrlf_state(WCDMA_RLF_IDLE);
          mm_received_suitable_search_end_ind = FALSE;
#endif
#ifdef FEATURE_GSM_GPRS
#ifdef FEATURE_NAS_FREE_FLOAT
                    gmmllc_send_service_req_if_needed();
#endif
#endif
          break;

        default:
          MSG_ERROR_DS( MM_SUB, "=MM= Unexpected error code %d",
                message->cmd.rr_plmn_select_cnf.error_code, 0,0 );
          break;
      } /* end switch (((rrc_act_cnf_T *) message)->service_status) */

#ifdef FEATURE_GSTK
          if (message->cmd.rr_plmn_select_cnf.service_state == MM_AS_SERVICE_AVAILABLE)
          {
                 if ((mm_state == MM_IDLE) && ((mm_idle_substate == MM_LIMITED_SERVICE) || (mm_idle_substate == MM_NO_IMSI)))
                 {
                   byte lai[LAI_SIZE] = { 0xff, 0xff, 0xff, 0, 0 };

                   lai[0] = mm_serving_plmn.info.plmn.identity[0];
                   lai[1] = mm_serving_plmn.info.plmn.identity[1];
                   lai[2] = mm_serving_plmn.info.plmn.identity[2];
                   lai[3] = mm_serving_plmn.lac.lac[0];
                   lai[4] = mm_serving_plmn.lac.lac[1];
  
                   if( memcmp(lai, mm_gstk_previous_serving_lai, LAI_SIZE) != 0) /* New LAI */
                   {
                         mm_send_gstk_mm_idle_info() ;
                   }
                 }
          }
#endif

      /* Log the Cell info */
      mmsend_mm_info_log_packet();

#ifdef FEATURE_FEMTO_CSG
      mm_serving_csg_id = mm_serving_plmn.info.csg_info.csg_id;
#endif 
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected msg %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rr_plmn_select_cnf() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_SERVICE_IND

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_service_ind( mm_cmd_type *message )
{
  boolean                 cs_service_available, ps_dereg_pended =FALSE, mmcoord_cell_id_changed = FALSE, mm_reg_ind_sent = FALSE;
  boolean  mm_to_only_reg_local = FALSE;
  boolean  mm_reg_cnf_only_for_reg_local = FALSE;
  mmr_service_ind_s_type  mmr_service_ind;
  byte                    i;
#ifdef FEATURE_DUAL_SIM
  sys_modem_as_id_e_type      mm_other_as_id, mm_as_id_local;
#endif
  mmrrc_connection_status_T rrc_conn_state_local = MMRRC_IDLE; 
#ifdef FEATURE_SEGMENT_LOADING
  interface_t* ptr_W = NULL;
#endif
  boolean policy_change_pending = FALSE; 

#ifdef FEATURE_LTE
  mm_csfb_state_type      csfb_state = mm_csfb_state;
  emm_ctrl_data_type *emm_ctrl_data_ptr_tag = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr_tag != NULL);
#endif  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#ifdef FEATURE_SEGMENT_LOADING
   ptr_W = get_wcdma_interface();
#endif
  if (message->cmd.hdr.message_id == (int) RRC_SERVICE_IND)
  {
#ifdef FEATURE_WCDMA_MULTIBAND
    mm_active_band = message->cmd.rrc_service_ind.active_band;
#endif
    mm_band_indicator = SYS_BAND_CLASS_NONE;
#ifdef FEATURE_GSM_DTM
    mm_serving_plmn.dtm_supported = FALSE;
#endif
    mm_serving_plmn.egprs_supported = FALSE;
    mm_reject_call_on_rai_change = FALSE;
    if (mm_waiting_for_service_cnf || mm_waiting_for_stop_mode_cnf || mm_waiting_for_deact_cnf)
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= Unexpected RRC_SERVICE_IND");
    }
#ifdef FEATURE_WRLF_SYSTEM_SEL
    else if((message->cmd.rrc_service_ind.service_status == MM_AS_NO_SERVICE) &&
               (message->cmd.rrc_service_ind.no_service_cause == SYS_NO_SVC_CAUSE_RLF))
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= Revd RRC_SERVICE_IND, NO SERVICE CAUSE RLF");
      /* --------------------------------------------
      ** Set the rlf state to started and copy the present serving
      ** plmn info into mm_wrlf_serving_plmn variable
      ** -------------------------------------------- */
          mmcoord_set_wrlf_state(WCDMA_RLF_STARTED);

      memscpy(mm_wrlf_serving_plmn_ptr, sizeof(serving_plmn_info_T),&mm_serving_plmn,sizeof(serving_plmn_info_T));

      mm_serving_plmn.gmm_service_state     = SERVICE_NOT_AVAILABLE;
      mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
      mm_serving_plmn.network_op_mode    = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
      mm_mmr_cell_service_ind_pending = FALSE;
      mm_serving_plmn.mm_service_state = SERVICE_NOT_AVAILABLE;
      mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_RLF;
      mm_serving_plmn.info.suitable_search_time = message->cmd.rrc_service_ind.suitable_search_time;

#ifdef FEATURE_DUAL_SIM
      if (message->cmd.rrc_service_ind.acq_status == SYS_ACQ_STATUS_NO_RESOURCE)
      {
        mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_NO_RESOURCE;
      }
      else
      {
        mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
      }
#elif defined (FEATURE_1XSRLTE)
      mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif


#ifdef FEATURE_FEMTO_CSG
      mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif       
      mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                                mm_serving_plmn.info.reg_service_domain,
                                                FALSE,
                                                SYS_SRV_STATUS_NO_SRV,
                                                FALSE );
      gmm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                           mm_serving_plmn.info.reg_service_domain,
                                           FALSE,
                                           SYS_SRV_STATUS_NO_SRV,
                                           FALSE );
      mm_rat_handle_srv_status_update ( SYS_SRV_STATUS_NO_SRV );
      gmm_previous_gprs_service_state  = SERVICE_NOT_AVAILABLE;
      // send service indication or cnf to REG
       if ((mm_reg_waiting_for_reg_cnf) || 
          (
#ifdef FEATURE_LTE
          (mm_ps_detach_state != MM_PS_DETACH_STATE_DETACH_REQ_PENDING) &&
          ((emm_ctrl_data_ptr->reg_req_pended_indication && 
            mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
            || mmcoord_reg_req_pending) &&
#else           
          ((mm_ps_detach_state != MM_PS_DETACH_STATE_DETACH_REQ_PENDING) &&
          (mmcoord_reg_req_pending)) &&
#endif
          (mmcoord_pending_reg_message.cmd.hdr.message_set == MS_MM_REG) &&
          (mmcoord_pending_reg_message.cmd.hdr.message_id  ==  (int)MMR_REG_REQ)))
      {
        if(mmcoord_reg_req_pending
#ifdef FEATURE_LTE
            ||
           (emm_ctrl_data_ptr->reg_req_pended_indication && 
            mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
#endif
           )
        {
          mm_reg_req_trans_id = mmcoord_pending_reg_message.cmd.mmr_reg_req.trans_id;
        }
        mmcoord_mmr_reg_cnf.service_state  = mm_serving_plmn.info;
        mm_send_mmr_reg_cnf();

        mm_reg_waiting_for_reg_cnf = FALSE;
        mm_ready_to_send_reg_cnf   = FALSE;
      }
      else
      {
        mmr_service_ind.service_state = mm_serving_plmn.info;
        mmr_service_ind.cause = MMR_CAUSE_NOT_SPECIFIED;
        mm_send_mmr_service_ind( &mmr_service_ind );
      }

      mm_serving_plmn.info.plmn.identity[0] = 0xFF;
      mm_serving_plmn.info.plmn.identity[1] = 0xFF;
      mm_serving_plmn.info.plmn.identity[2] = 0xFF;
      mm_serving_plmn.lac.lac[0]            = 0xFF;
      mm_serving_plmn.lac.lac[1]            = 0xFF;
      mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
      if (gmm_state == GMM_DEREGISTERED_INITIATED)
      {
        gmm_handle_gprs_detach_low_level_failure();
        mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
      }
      else
      {
        /* Notify the SM that the Service is lost */
        gmm_sm_send_con_release_ind(SIG_CONNECTION_RELEASE);
      }
       //no recovery window needed for RLF when LPM is pending
        if (((mmcoord_pending_reg_message.cmd.hdr.message_set == (int) MS_MM_REG) &&
            (mmcoord_pending_reg_message.cmd.hdr.message_id == (int) MMR_STOP_MODE_REQ)) &&
             mm_timer_status[TIMER_MM_REL]==TIMER_ACTIVE)
         {
           MSG_HIGH_DS_0(MM_SUB, "=MM= Restart timer with smaller value in case of RLF if stop mode is pending");
           mm_start_timer( TIMER_MM_REL, 500 );  
         }
     //gracefully handle CCO state in RLF case

#ifdef FEATURE_GSM_GPRS
     
       if ((mm_interrat_state == MM_UMTS_TO_GSM_INTERRAT_CHANGE_IN_PROGRESS) ||
           (mm_interrat_state == MM_GSM_TO_UMTS_INTERRAT_CHANGE_IN_PROGRESS))
       {

#ifdef FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER

        if (WtoGCCO_state == CCO_FAILED )
         {
           MSG_HIGH_DS_0( MM_SUB, "=MM= Resetting the WtoGCCO_state to CCO_INACTIVE ");
           WtoGCCO_state = CCO_INACTIVE;
         }
#endif
#ifdef FEATURE_INTERRAT_PCCO_GTOW
         if (GtoWCCO_state == CCO_FAILED )
         {
           MSG_HIGH_DS_0( MM_SUB, "=MM= Resetting the GtoWCCO_state to CCO_INACTIVE ");
           GtoWCCO_state = CCO_INACTIVE;
         }
#endif
       }
     
#endif /* #ifdef FEATURE_GSM_GPRS */

      
    }
#endif /* FEATURE_WRLF_SYSTEM_SEL */
    else
    {
#if defined(FEATURE_WRLF_SYSTEM_SEL) || defined(FEATURE_LTE)
      mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
#endif
      switch (message->cmd.rrc_service_ind.service_status)
      {
        case MM_AS_NO_SERVICE:
          mm_serving_plmn.info.plmn.identity[0] = 0xFF;
          mm_serving_plmn.info.plmn.identity[1] = 0xFF;
          mm_serving_plmn.info.plmn.identity[2] = 0xFF;
          mm_serving_plmn.lac.lac[0]            = 0xFF;
          mm_serving_plmn.lac.lac[1]            = 0xFF;
          mm_serving_plmn.gmm_service_state     = SERVICE_NOT_AVAILABLE;
          mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
          mm_serving_plmn.network_op_mode    = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
          mm_mmr_cell_service_ind_pending = FALSE;
#ifdef FEATURE_FEMTO_CSG
          mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif       
          mm_coord_camped_cell_id = 0xFFFF;   
          mm_lu_pended_nmo_unknown = FALSE;

          gmm_foreground_search_pending = FALSE;
          mm_service_ind_blocked_due_to_irat_cs_reg = FALSE;

#ifdef FEATURE_DUAL_SIM
          if (message->cmd.rrc_service_ind.acq_status == SYS_ACQ_STATUS_NO_RESOURCE)
          {
            mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_NO_RESOURCE;
			if(mm_reg_waiting_for_reg_cnf)
			{
			  mmcoord_mmr_reg_cnf.ue_oos = TRUE;
			}
          }
          else
          {
            mm_serving_plmn.info.acq_status       = SYS_ACQ_STATUS_SUCCESS;
          }
#elif defined (FEATURE_1XSRLTE)
          mm_serving_plmn.info.acq_status       = SYS_ACQ_STATUS_SUCCESS;
#endif
          MSG_HIGH_DS_0(MM_SUB,"=MM= Revd RRC_SERVICE_IND, NO SERVICE");
          break;

        case MM_AS_SERVICE_AVAILABLE:

          MSG_HIGH_DS_1(MM_SUB ,"=MM= Revd RRC_SERVICE_IND w/ SRV-AC = %d",
                     message->cmd.rrc_service_ind.service_info.cell_access);
          rrc_conn_state_local = rrc_connection_status;
          /* Store the cell_access information */
          mm_prev_cell_access = mm_get_cs_cell_access(PROCEDURE_CS_REGISTRATION);
          
#ifndef FEATURE_DSAC          
          mm_system_information.cell_access =
            message->cmd.rrc_service_ind.service_info.cell_access;
#endif
          if((message->cmd.rrc_service_ind.is_cell_id_valid) &&
             (sys_plmn_match(mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
              message->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id ),mm_serving_plmn.info.plmn)) &&
             (message->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[0] == mm_serving_plmn.lac.lac[0] &&
              message->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[1] == mm_serving_plmn.lac.lac[1])&&
             (mm_serving_cell_id != message->cmd.rrc_service_ind.cell_id))
          {
             mmcoord_cell_id_changed = TRUE;
          }
          /* --------------------------------------------
          ** Store the LAI where we are currently camped
          ** -------------------------------------------- */
          mm_serving_plmn.info.plmn = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
            message->cmd.rrc_service_ind.service_info.selected_plmn.plmn_id );
#if defined FEATURE_DUAL_SIM ||defined FEATURE_1XSRLTE
          mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif
          gmm_reset_old_ptmsi();

          if (message->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl)
          {
            mm_serving_plmn.lac.lac[0] =
              message->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[0];
            mm_serving_plmn.lac.lac[1] =
              message->cmd.rrc_service_ind.service_info.gsm_map_common_info.value[1];
          }

          mm_serving_plmn.gmm_service_state = SERVICE_NOT_AVAILABLE;
          cs_service_available = FALSE;

          for (i=0; i < message->cmd.rrc_service_ind.service_info.num_cn_domains; i++)
          {
            if (message->cmd.rrc_service_ind.service_info.cn_domain_info[i].
                  cn_domain_id == RRC_PS_DOMAIN_CN_ID)
            {
              mm_serving_plmn.gmm_service_state = SERVICE_AVAILABLE;

              mm_serving_plmn.rac = message->cmd.rrc_service_ind.service_info.
                cn_domain_info[i].cn_domain_specific_info.gsm_map_info.value[0];

              if ((message->cmd.rrc_service_ind.service_info.cn_domain_info[i].
                     cn_domain_specific_info.gsm_map_info.value[1] & 0x01) == 0)
              {
                mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_1;
              }
              else
              {
                mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_2;
              }

              MSG_HIGH_DS_2( MM_SUB, "=MM= GMM sys info: NMO=%d, RAC=%d",
                         mm_serving_plmn.network_op_mode, mm_serving_plmn.rac);
            }
            else if (message->cmd.rrc_service_ind.service_info.cn_domain_info[i].
                      cn_domain_id == RRC_CS_DOMAIN_CN_ID)
            {
              cs_service_available = TRUE;
            }
          } /* end for */

          mm_serving_plmn.t3122_running = TRUE;

          MSG_HIGH_DS_1(MM_SUB ,"=MM= mm_serving_plmn.t3122_running = %d",mm_serving_plmn.t3122_running);

#ifdef FEATURE_FEMTO_CSG
          if(is_wcdma_mode())
          {
            mm_serving_plmn.info.csg_info = message->cmd.rrc_service_ind.service_info.csg_info;
          }
          else
          {
            mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
          }

          MSG_HIGH_DS_3(MM_SUB, "=MM= CSG: csg_id = %d, hnb_name_length = %d, is_cell_hybrid = %d", 
                                       mm_serving_plmn.info.csg_info.csg_id, 
                                       mm_serving_plmn.info.csg_info.hnb_name.length,
                                       mm_serving_plmn.info.csg_info.hybrid_cell);
#endif       
          mm_serving_cell_id = message->cmd.rrc_service_ind.cell_id;

          if (message->cmd.rrc_service_ind.is_cell_id_valid)
          {
             mm_serving_cell_valid = TRUE;
             mm_coord_camped_cell_id = (word)(mm_serving_cell_id & (uint32)0x0000FFFF);
          }
          else
          {
             mm_serving_cell_valid = FALSE;
             mm_coord_camped_cell_id = 0xFFFF;
          }

          if(cs_service_available &&
             (mm_serving_plmn.info.plmn_service_capability != SYS_SRV_CAPABILITY_CS_SERVICE_ONLY) &&
             (mm_serving_plmn.info.plmn_service_capability != SYS_SRV_CAPABILITY_CS_PS_SERVICE) &&
             (mm_serving_plmn.ms_op_mode == (sys_srv_domain_e_type)SYS_SRV_CAPABILITY_CS_PS_SERVICE))
          {
            mm_cell_changed_to_cs_capable = TRUE;
          }

          if (mm_serving_plmn.gmm_service_state == SERVICE_AVAILABLE)
          {
            if (cs_service_available)
            {
              mm_serving_plmn.info.plmn_service_capability =
                SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              mm_serving_plmn.info.plmn_service_capability =
                SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
            }
          }
          else /* mm_serving_plmn.gmm_service_state == SERVICE_NOT_AVAILABLE */
          {
            mm_serving_plmn.network_op_mode   = PS_DOMAIN_SYS_INFO_UNAVAILABLE;

            if (cs_service_available)
            {
              mm_serving_plmn.info.plmn_service_capability =
                SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
            }
            else
            {
              mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
            }
          }

#ifdef FEATURE_LTE
          if((gmm_state == GMM_DEREGISTERED || mm_possible_imsi_attach == TRUE) &&
              (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                  SYS_NETWORK_SELECTION_MODE_LIMITED) && 
              (mm_serving_plmn.info.sim_state != SYS_SIM_STATE_CS_PS_INVALID) )
          {
            if(emm_validate_attach_plmn(emm_ctrl_data_ptr_tag, &mm_serving_plmn.info.plmn) == FALSE)
            {
              mm_update_mm_serving_plmn_info( SYS_SIM_STATE_CS_PS_INVALID,
                                        SYS_SRV_DOMAIN_NO_SRV,
                                        TRUE,
                                        SYS_SRV_STATUS_LIMITED,
                                        FALSE);
#ifdef FEATURE_DUAL_SIM  
              mm_gmm_set_non_illegal_cause();
#endif
            }
          }
          mmcoord_locally_deactivate_isr_if_needed();
#endif

#ifdef FEATURE_DSAC
          (void) mm_process_dsac_info_from_rrc_service_ind(&(message->cmd.rrc_service_ind.service_info));
#endif
          mm_send_mmr_cell_access_ind();
#ifdef FEATURE_LTE
          mmcoord_reevaluate_rat_pri_list(mm_serving_plmn.info.plmn);
          if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
          {
            mm_send_rrc_lte_reselection_status_ind();
          }
#endif

#ifdef FEATURE_NAS_RAU_ON_MS_RAC_CHANGE
          if((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                                          SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)&&
              (sys_plmn_id_is_undefined(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) == TRUE)&&
              (mmcoord_check_if_ms_rat_capability_changed() == TRUE))
          {
            gmm_ra_update_pending = TRUE;
          }
#endif

          policy_change_pending = mmcoord_policy_change_pending();


#ifdef FEATURE_SGLTE
          if((policy_change_pending == TRUE)
            || (mm_process_if_both_stack_has_camped_non_sglte_plmn(mm_ps_stack_id)==TRUE) 
            ||  (mm_is_sglteplmn_in_sglte_roam(mm_serving_plmn.info.plmn ) == TRUE))
#else
          if(policy_change_pending == TRUE)
#endif 
            
            {
            mmcoord_capability_chg_pend_limited_service(policy_change_pending, FALSE);
            return;
        }
          break;

        default:
          MSG_ERROR_DS( MM_SUB, "=MM= Unexpected service_status %d",
                message->cmd.rrc_service_ind.service_status, 0,0 );
          break;
      } /* end switch (message->cmd.rrc_service_ind.service_status) */
      /* -----------------------------------------------------------------
      ** Route RRC_SERVICE_IND to MM and/or GMM based on MS operation mode
      ** ----------------------------------------------------------------- */
     if(cm_per_subs_is_reg_allowed(mm_serving_plmn.info.plmn,
                     mm_serving_plmn.info.active_rat,(sys_modem_as_id_e_type)mm_as_id,mm_set_cm_stack_id(mm_as_id)) == FALSE)
     {
            MSG_HIGH_DS_0( MM_SUB, "=MM= Sent Limited as don't register when CT disabled.");
            mm_substate_control( MM_STD_LIMITED_SERVICE );

              /* Set the Serving PLMN service status to Limited */
            mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                               mm_serving_plmn.info.reg_service_domain,
                                               TRUE,
                                               SYS_SRV_STATUS_LIMITED,
                                               TRUE );            
             if (mm_reg_waiting_for_reg_cnf)
             {
               /* Set the REG CNF plmn service state to the Serving PLMN info */
               mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
               mmcoord_mmr_reg_cnf.service_state.service_status = SYS_SRV_STATUS_LIMITED;    
               mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;            
               mm_send_mmr_reg_cnf();
               mm_reg_waiting_for_reg_cnf = FALSE;
               mm_ready_to_send_reg_cnf   = FALSE;
             }
             else 
             {
               mmr_service_ind_s_type  mmr_service_ind_temp;
               mmr_service_ind_temp.service_state = mm_serving_plmn.info;
               mmr_service_ind_temp.service_state.service_status = SYS_SRV_STATUS_LIMITED;    
               mmr_service_ind_temp.cause = MMR_CAUSE_NOT_SPECIFIED;               
               mm_send_mmr_service_ind(&mmr_service_ind_temp);
             }    
             gmm_set_substate( GMM_LIMITED_SERVICE );
			 if ((mm_held_cnm_service_req.present) && 
                  (mm_held_cnm_service_req.establishment_cause == (int) MM_RR_EMERGENCY_CALL) &&
                  ((mm_held_cnm_service_req.interat_status == MM_AS_INTERRAT_RESELECTION) || 
                  (mm_held_cnm_service_req.interat_status == MM_AS_INTERRAT_CC_ORDER)))
             {
               mm_check_pending_requests();
               MSG_HIGH_DS_1(MM_SUB ,"=MM= Processing Pending E-call after moving to different cell of INTERRAT: %d", 
                           mm_held_cnm_service_req.interat_status);
               mm_held_cnm_service_req.interat_status = MM_AS_SUCCESS ;
             }
     }
     else 
     {
      if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
      {
        gmm_handle_message( message );
      }
     else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) &&
               (gmm_state != GMM_DEREGISTERED_INITIATED) && (gmm_detach_after_rau_pending == FALSE))
      {
        ps_dereg_pended = FALSE;
#ifdef FEATURE_DUAL_SIM
        for(mm_other_as_id = SYS_MODEM_AS_ID_1; mm_other_as_id < MAX_NAS_STACKS ; mm_other_as_id++)
        {
          if((mm_other_as_id != (sys_modem_as_id_e_type)mm_as_id) && 
             (message->cmd.rrc_service_ind.service_status == MM_AS_NO_SERVICE)&&
             (mmcoord_reg_req_pending_sim[mm_as_id] == TRUE) &&
             (gmm_force_local_detach_sim[mm_other_as_id] == TRUE) && 
             (gmm_state_sim[mm_other_as_id] == GMM_DEREGISTERED_INITIATED))
          {          
             MSG_HIGH_DS_0(MM_SUB,"=MM= recieved service indication with no service during DDS switch");
             mm_serving_plmn.mm_service_state = SERVICE_NOT_AVAILABLE;
             ps_dereg_pended = TRUE;
             mm_as_id_local = (sys_modem_as_id_e_type)mm_as_id;
             mm_set_as_id(mm_other_as_id);
             gmm_handle_gprs_detach_low_level_failure();
             mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
             mm_set_as_id(mm_as_id_local);
            }
          }  
#endif
        if(!ps_dereg_pended)
        {
          mm_handle_message( message );
        }
      }
      else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) ||
               (gmm_state == GMM_DEREGISTERED_INITIATED) ||
               (gmm_detach_after_rau_pending == TRUE))
      {
        if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
        {
#ifdef FEATURE_LTE
          mm_csfb_process_service_ind_nmo1(message);
#endif /*FEATURE_LTE*/ 
          gmm_handle_message( message );
        }
        else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
        {
          mm_handle_message( message );
          gmm_handle_message( message );
        }
      }
      else
      {
        MSG_FATAL_DS( MM_SUB, "=MM= Invalid MS op mode %d", (int)mm_serving_plmn.ms_op_mode, 0,0 );
      }
     }

      if (message->cmd.rrc_service_ind.service_status == MM_AS_SERVICE_AVAILABLE)
      {
         if (mm_needs_to_send_mmr_service_ind || (mmcoord_cell_id_changed && !mm_reg_ind_sent))
         {
            mm_reg_cnf_only_for_reg_local = mmcoord_mmr_reg_cnf.only_for_reg;       
            mm_to_only_reg_local = mm_to_only_reg;
            if((mmcoord_cell_id_changed && !mm_reg_ind_sent) && !mm_needs_to_send_mmr_service_ind)
            {
              mm_to_only_reg = TRUE;
            }
           /*************************************************************
           * Block the service indication to REG in the following cases:*
           * 1. LU is going on.                                         *
           * 2. RAU is going on and no HO.                              * 
           * During Inter-RAT HO UI has to be updated ASAP even though  *
           * RAU is going on.                                           * 
           *************************************************************/
           if ((mm_state == MM_WAIT_FOR_RR_CONNECTION_LU) ||
               (mm_state == MM_LOCATION_UPDATE_INITIATED) ||
               (((mm_state != MM_CONNECTION_ACTIVE) &&
                 (mm_state != MM_WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION) &&
                 (mm_state != MM_WAIT_FOR_RR_CONNECTION_MM) 
#ifdef FEATURE_LTE
                 && !((csfb_state == MM_CSFB_STATE_MT_CALL_ORIG)&&
                      (mm_state == MM_WAIT_FOR_NETWORK_COMMAND))
#endif
                 )&&
                ((gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED)||
                 (gmm_state == GMM_REGISTERED_INITIATED))))
           {
              MSG_HIGH_DS_0(MM_SUB, "=MM= REG will be updated about new LA after registration completion");  
              mm_needs_to_send_mmr_service_ind = FALSE;
           }
           else if(mmcoord_cell_id_changed && !mm_needs_to_send_mmr_service_ind && mm_reg_waiting_for_reg_cnf)
           {
             MSG_HIGH_DS_0(MM_SUB, "=MM= REG will be updated about new LA after registration completion");
           }
           else
           {
             mmr_service_ind.cause = MMR_CAUSE_NOT_SPECIFIED;
             mmr_service_ind.service_state = mm_serving_plmn.info;
             mm_send_mmr_service_ind( &mmr_service_ind );
           }
           mm_to_only_reg = mm_to_only_reg_local;
           if((mmcoord_mmr_reg_cnf.only_for_reg == mm_reg_cnf_only_for_reg_local == TRUE))
           {
             mmcoord_mmr_reg_cnf.only_for_reg = TRUE;
           }
           else
           {
             mmcoord_mmr_reg_cnf.only_for_reg = FALSE;
           }
         }
         if ((mm_state == MM_IDLE) && ((mm_idle_substate == MM_LIMITED_SERVICE) || (mm_idle_substate == MM_NO_IMSI)))
         {
           byte lai[LAI_SIZE] = { 0xff, 0xff, 0xff, 0, 0 };

           lai[0] = mm_serving_plmn.info.plmn.identity[0];
           lai[1] = mm_serving_plmn.info.plmn.identity[1];
           lai[2] = mm_serving_plmn.info.plmn.identity[2];
           lai[3] = mm_serving_plmn.lac.lac[0];
           lai[4] = mm_serving_plmn.lac.lac[1];
           
           if( memcmp(lai, mm_gstk_previous_serving_lai, LAI_SIZE) != 0) /* New LAI */
           {
#ifdef FEATURE_GSTK
             mm_send_gstk_mm_idle_info() ;
#endif
           }
         }

         /* Store system information */
         mmumts_process_cell_change( message->cmd.rrc_service_ind.service_status,
                                     message->cmd.rrc_service_ind.service_info,NORMAL_SERVICE_MESSAGE);
                                     
         /* Store the current LAI information*/
         gmm_current_lai.PLMN_id = mm_serving_plmn.info.plmn;
         memscpy(gmm_current_lai.location_area_code, LAC_SIZE, mm_serving_plmn.lac.lac, LAC_SIZE);
         gmm_current_rac = mm_serving_plmn.rac;                                     
         mm_cell_changed_to_cs_capable = FALSE;

         /* Start the T3212 timer only if the ms_op_mode is it is only CS mode */
         if (mm_serving_plmn.ms_op_mode != SYS_SRV_DOMAIN_PS_ONLY)
         {
           /* Handle T3212 */
           mmumts_process_T3212(message);
         }
#ifndef FEATURE_SEGMENT_LOADING
         if(rrc_conn_state_local == rrc_connection_status)
         {
#ifdef FEATURE_DUAL_WCDMA
           rrc_reset_mcm_irat_resel_act_cause_sub((sys_modem_as_id_e_type)mm_sub_id);
#else
           rrc_reset_mcm_irat_resel_act_cause();
#endif 
           MSG_HIGH_DS_0(MM_SUB, "=MM= sent reset establishment cause  to rrc as no signaling procedure was started");
         }
#else
         if((rrc_conn_state_local == rrc_connection_status)&&(ptr_W != NULL))
         {
#ifdef FEATURE_DUAL_WCDMA
           IWCDMA_rrc_reset_mcm_irat_resel_act_cause_sub(ptr_W, (sys_modem_as_id_e_type)mm_sub_id);
#else
           IWCDMA_rrc_reset_mcm_irat_resel_act_cause(ptr_W);
#endif 
           MSG_HIGH_DS_0(MM_SUB, "=MM= sent reset establishment cause  to rrc as no signaling procedure was started");
         }
#endif
      }
      
      /* Log the Cell info */
      mmsend_mm_info_log_packet();
#ifdef FEATURE_FEMTO_CSG
      mm_serving_csg_id = mm_serving_plmn.info.csg_info.csg_id;
#endif 

    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RRC msg %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_service_ind() */

 /*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_SERVICE_IND_FROM_OTA_MSG

DESCRIPTION
  Note: This primitive is expected by MM in RRC Connected Mode only. However
  even if received with NO Service condition we are currently handling to
  prevent any issues.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_service_ind_from_ota_msg( mm_cmd_type *message )
{
  //boolean                 cs_service_available = TRUE;
  mmr_service_ind_s_type  mmr_service_ind;
  byte                    i;

#ifdef FEATURE_LTE
  mm_csfb_state_type      csfb_state = mm_csfb_state;
#endif

  boolean policy_change_pending = FALSE; 
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (message->cmd.hdr.message_id == (int) RRC_SERVICE_IND_FROM_OTA_MSG)
  {
#ifdef FEATURE_WCDMA_MULTIBAND
    mm_active_band = message->cmd.rrc_service_ind_from_ota_msg.active_band;
#endif
    mm_band_indicator = SYS_BAND_CLASS_NONE;
#ifdef FEATURE_GSM_DTM
    mm_serving_plmn.dtm_supported = FALSE;
#endif
    mm_serving_plmn.egprs_supported = FALSE;
#if defined(FEATURE_WRLF_SYSTEM_SEL) || defined(FEATURE_LTE)
    mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_NORMAL;
    mm_serving_plmn.info.suitable_search_time = 0;
#endif
    mm_manual_mode_irat_ho_in_non_user_sel_cell = FALSE;
    if (mm_waiting_for_service_cnf || mm_waiting_for_stop_mode_cnf)
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= Unexpected RRC_SERVICE_IND_FROM_OTA_MSG");
    }
    else
    {
      switch (message->cmd.rrc_service_ind_from_ota_msg.service_status)
      {
        /* -----------------------------------------------------------
        ** This is NOT expected as this primitive is to be used by RRC
        ** in RRC Connected Mode only. We are still handling this for now
        ** --------------------------------------------------------- */
        case MM_AS_NO_SERVICE:
          mm_serving_plmn.info.plmn.identity[0] = 0xFF;
          mm_serving_plmn.info.plmn.identity[1] = 0xFF;
          mm_serving_plmn.info.plmn.identity[2] = 0xFF;
          mm_serving_plmn.lac.lac[0]            = 0xFF;
          mm_serving_plmn.lac.lac[1]            = 0xFF;
          mm_serving_plmn.gmm_service_state     = SERVICE_NOT_AVAILABLE;
          mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
          mm_serving_plmn.network_op_mode    = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
#ifdef FEATURE_FEMTO_CSG
          mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif
          mm_mmr_cell_service_ind_pending = FALSE;
          mm_coord_camped_cell_id  = 0xFFFF;


          gmm_foreground_search_pending   = FALSE;
          mm_service_ind_blocked_due_to_irat_cs_reg = FALSE;

          MSG_HIGH_DS_0(MM_SUB,"=MM= Received RRC_SERVICE_IND_FROM_OTA_MSG, NO SERVICE which is Not Expected");
          break;

        case MM_AS_SERVICE_AVAILABLE:

          MSG_HIGH_DS_1(MM_SUB ,"=MM= Revd RRC_SERVICE_IND_FROM_OTA_MSG w/ SRV-AC = %d",
                     message->cmd.rrc_service_ind_from_ota_msg.service_info.cell_access);

          /* Store the cell_access information */
          mm_prev_cell_access = mm_get_cs_cell_access(PROCEDURE_CS_REGISTRATION);
#ifndef FEATURE_DSAC          
          mm_system_information.cell_access =
            message->cmd.rrc_service_ind_from_ota_msg.service_info.cell_access;
#endif
          /* ------------------------------------------------
          ** Common NAS Sys Info, PS specific NAS Sys Info and
          ** CS specific NAS Sys Info - all the three can be
          ** optional in this primitive. If they are not present
          ** the existing information shall be considered valid
          ** by MM. IMP: absence of any of the sys info's do not
          ** mean that cell does not support that service
          ** ----------------------------------------------- */

          /* --------------------------------------------
          ** Store the LAI where we are currently camped 
          ** if RRC is indicating a valid plmn id.
          ** -------------------------------------------- */
          if ((message->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[0] == 0xFF) &&
              (message->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[1] == 0xFF) &&
              (message->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id.mcc[2] == 0xFF))
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= Invalid PLMN Id. All 3 MCCs are 0xFF");
          }
          else
          {    
            mm_serving_plmn.info.plmn = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
              message->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_id );
          }    
#if defined FEATURE_DUAL_SIM || defined FEATURE_1XSRLTE
          mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif

          if (message->cmd.rrc_service_ind_from_ota_msg.service_info.gsm_map_common_info_incl)
          {
            mm_serving_plmn.lac.lac[0] =
              message->cmd.rrc_service_ind_from_ota_msg.service_info.gsm_map_common_info.value[0];
            mm_serving_plmn.lac.lac[1] =
              message->cmd.rrc_service_ind_from_ota_msg.service_info.gsm_map_common_info.value[1];
          }

          /* ------------------------------------------------------
          ** If CN Domain Info not present in this primitive use the
          ** existing Common/CS/PS NAS sys info
          ** ---------------------------------------------------- */
#ifdef FEATURE_NO_REG_UNTIL_NMO_KNOWN_AFTER_HANDOVER
          if (mm_interrat_state == MM_GSM_TO_UMTS_INTERRAT_CHANGE_IN_PROGRESS)
          {
            mm_serving_plmn.gmm_service_state = SERVICE_NOT_AVAILABLE;
          }
#endif
          for (i=0; i < message->cmd.rrc_service_ind_from_ota_msg.service_info.num_cn_domains; i++)
          {
            if (message->cmd.rrc_service_ind_from_ota_msg.service_info.cn_domain_info[i].
                  cn_domain_id == RRC_PS_DOMAIN_CN_ID)
            {
              mm_serving_plmn.gmm_service_state = SERVICE_AVAILABLE;

              mm_serving_plmn.rac = message->cmd.rrc_service_ind_from_ota_msg.service_info.
                cn_domain_info[i].cn_domain_specific_info.gsm_map_info.value[0];

              if ((message->cmd.rrc_service_ind_from_ota_msg.service_info.cn_domain_info[i].
                     cn_domain_specific_info.gsm_map_info.value[1] & 0x01) == 0)
              {
                mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_1;
              }
              else
              {
                mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_2;
              }

              MSG_HIGH_DS_2( MM_SUB, "=MM= GMM sys info: NMO=%d, RAC=%d",
                         mm_serving_plmn.network_op_mode, mm_serving_plmn.rac);
            }
            else if (message->cmd.rrc_service_ind_from_ota_msg.service_info.cn_domain_info[i].
                      cn_domain_id == RRC_CS_DOMAIN_CN_ID)
            {
              mm_serving_plmn.mm_service_state = SERVICE_AVAILABLE;
            }
          } /* end for */
#ifdef FEATURE_FEMTO_CSG
          if(is_wcdma_mode())
          {
            mm_serving_plmn.info.csg_info = message->cmd.rrc_service_ind_from_ota_msg.service_info.csg_info;
          }
          else
          {
            mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
          }

          MSG_HIGH_DS_2(MM_SUB, "=MM= CSG: csg_id = %d, hnb_name_length = %d", 
          mm_serving_plmn.info.csg_info.csg_id, mm_serving_plmn.info.csg_info.hnb_name.length);
#endif 
          mm_serving_plmn.t3122_running = TRUE;

          MSG_HIGH_DS_1(MM_SUB ,"=MM= mm_serving_plmn.t3122_running = %d",mm_serving_plmn.t3122_running);

          mm_serving_cell_id = message->cmd.rrc_service_ind_from_ota_msg.cell_id;

          if (message->cmd.rrc_service_ind_from_ota_msg.is_cell_id_valid)
          {
             mm_serving_cell_valid = TRUE;
             mm_coord_camped_cell_id = (word)(mm_serving_cell_id & (uint32)0x0000FFFF);
          }
          else
          { 
             mm_serving_cell_valid = FALSE;
             mm_coord_camped_cell_id = 0xFFFF;
          }

          if((mm_serving_plmn.mm_service_state == SERVICE_AVAILABLE) &&
             (mm_serving_plmn.info.plmn_service_capability != SYS_SRV_CAPABILITY_CS_SERVICE_ONLY) &&
             (mm_serving_plmn.info.plmn_service_capability != SYS_SRV_CAPABILITY_CS_PS_SERVICE))
          {
            mm_cell_changed_to_cs_capable = TRUE;
          }

          /* ------------------------------------------------------------
          ** If any domain specific info is not present in this primitive
          ** from RRC, use the already existing information to perform any
          ** check to verify service
          ** --------------------------------------------------------- */
          if (mm_serving_plmn.gmm_service_state == SERVICE_AVAILABLE)
          {
            if (mm_serving_plmn.mm_service_state == SERVICE_AVAILABLE)
            {
              mm_serving_plmn.info.plmn_service_capability =
                SYS_SRV_CAPABILITY_CS_PS_SERVICE;
            }
            else
            {
              mm_serving_plmn.info.plmn_service_capability =
                SYS_SRV_CAPABILITY_PS_SERVICE_ONLY;
            }
          }
          else /* mm_serving_plmn.gmm_service_state == SERVICE_NOT_AVAILABLE */
          {
            mm_serving_plmn.network_op_mode   = PS_DOMAIN_SYS_INFO_UNAVAILABLE;

            if (mm_serving_plmn.mm_service_state == SERVICE_AVAILABLE)
            {
              mm_serving_plmn.info.plmn_service_capability =
                SYS_SRV_CAPABILITY_CS_SERVICE_ONLY;
            }
            else
            {
              mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
            }
          }
#ifdef FEATURE_NO_REG_UNTIL_NMO_KNOWN_AFTER_HANDOVER
          if ((mm_serving_plmn.gmm_service_state == SERVICE_NOT_AVAILABLE) &&
              (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS))
          {
            mm_lu_pended_nmo_unknown = TRUE;
          }
#endif
          /* Start the T3212 timer only if the ms_op_mode is it is only CS mode */
          if (mm_serving_plmn.ms_op_mode != SYS_SRV_DOMAIN_PS_ONLY)
          {
            /* Handle T3212 */
            mmumts_process_T3212(message);
          }
#ifdef FEATURE_DSAC
          (void) mm_process_dsac_info_from_rrc_service_ind(&(message->cmd.rrc_service_ind_from_ota_msg.service_info));
#endif
          mm_send_mmr_cell_access_ind();
#ifdef FEATURE_LTE
          if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
          {
            mm_send_rrc_lte_reselection_status_ind();
          }
          mmcoord_locally_deactivate_isr_if_needed();
#endif

          policy_change_pending = mmcoord_policy_change_pending();

          if(policy_change_pending == TRUE)
          {
           mmcoord_capability_chg_pend_limited_service(policy_change_pending, FALSE);
           return ;
        }

		  if ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL ||
				   	    mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION) && 
				   	    (!(PLMN_MATCH(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn.identity,mm_serving_plmn.info.plmn.identity) 
							  || (sys_eplmn_list_equivalent_plmn(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) 
                                  && sys_eplmn_list_equivalent_plmn(mm_serving_plmn.info.plmn)))) && 
                        ((mm_state == MM_CONNECTION_ACTIVE) ||
                         (mm_state == MM_WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION) ||
                         (mm_state ==MM_WAIT_FOR_RR_CONNECTION_MM)))
		    {
              MSG_HIGH_DS_0(MM_SUB, "=MM= Only for UI is TRUE");
              mm_manual_mode_irat_ho_in_non_user_sel_cell = TRUE;
			}

          break;

        default:
          MSG_ERROR_DS( MM_SUB, "=MM= Unexpected service_status %d",
                message->cmd.rrc_service_ind_from_ota_msg.service_status, 0,0 );
          break;
      } /* end switch (message->cmd.rrc_service_ind_from_ota_msg.service_status) */

      /* -----------------------------------------------------------------
      ** Route RRC_SERVICE_IND_FROM_OTA_MSG to MM and/or GMM based on MS operation mode
      ** ----------------------------------------------------------------- */
      if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
      {
        gmm_handle_message( message );
      }
      else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) &&
               (gmm_state != GMM_DEREGISTERED_INITIATED))
      {
        mm_handle_message( message );
      }
      else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) ||
               (gmm_state == GMM_DEREGISTERED_INITIATED))
      {
        if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
        {
#ifdef FEATURE_LTE
          mm_csfb_process_service_ind_nmo1(message);
#endif /*FEATURE_LTE*/ 
          gmm_handle_message( message );
        }
        else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
        {
          mm_handle_message( message );
          gmm_handle_message( message );
        }
      }
      else
      {
        MSG_FATAL_DS( MM_SUB, "=MM= Invalid MS op mode %d", (int)mm_serving_plmn.ms_op_mode, 0,0 );
      }

      if (message->cmd.rrc_service_ind_from_ota_msg.service_status == MM_AS_SERVICE_AVAILABLE)
      {
         if (mm_needs_to_send_mmr_service_ind)
         {
           /*************************************************************
           * Block the service indication to REG in the following cases:*
           * 1. LU is going on.                                         *
           * 2. RAU is going on and no HO.                              * 
           * During Inter-RAT HO UI has to be updated ASAP even though  *
           * RAU is going on.                                           * 
           *************************************************************/
           if ((mm_state == MM_WAIT_FOR_RR_CONNECTION_LU) ||
               (mm_state == MM_LOCATION_UPDATE_INITIATED) ||
               (((mm_state != MM_CONNECTION_ACTIVE) &&
                 (mm_state != MM_WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION) &&
                 (mm_state != MM_WAIT_FOR_RR_CONNECTION_MM)
#ifdef FEATURE_LTE
                 && !((csfb_state == MM_CSFB_STATE_MT_CALL_ORIG)&&
                      (mm_state == MM_WAIT_FOR_NETWORK_COMMAND))
#endif
                 ) &&
                ((gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED)||
                 (gmm_state == GMM_REGISTERED_INITIATED))))
           {
             MSG_HIGH_DS_0(MM_SUB, "=MM= REG will be updated about new LA after registration completion");  
             mm_needs_to_send_mmr_service_ind = FALSE;
           }
           else
           {
             mmr_service_ind.cause = MMR_CAUSE_NOT_SPECIFIED;
             mmr_service_ind.service_state = mm_serving_plmn.info;
             mm_send_mmr_service_ind( &mmr_service_ind );
           }
         } 
         /* Store system information */
         mmumts_process_cell_change( message->cmd.rrc_service_ind_from_ota_msg.service_status,
                                     message->cmd.rrc_service_ind_from_ota_msg.service_info,OTA_SERVICE_IND_MESSAGE);
         /* Store the current LAI information*/
         gmm_current_lai.PLMN_id = mm_serving_plmn.info.plmn;
         memscpy(gmm_current_lai.location_area_code, LAC_SIZE, mm_serving_plmn.lac.lac, LAC_SIZE);
         gmm_current_rac = mm_serving_plmn.rac;                                     
         mm_cell_changed_to_cs_capable = FALSE;
      }

      /* Log the Cell info */
      mmsend_mm_info_log_packet();
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RRC msg %d", message->cmd.hdr.message_id, 0,0 );
  }

} /* end mmcoord_process_rrc_service_ind_from_ota_msg() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RR_SERVICE_IND

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rr_service_ind( mm_cmd_type *message )
{
  mmr_service_ind_s_type  mmr_service_ind;
#ifdef FEATURE_LTE
  mm_csfb_state_type  csfb_state = mm_csfb_state;
#endif
#ifdef FEATURE_DUAL_SIM
  sys_modem_as_id_e_type      mm_other_as_id,mm_as_id_local;
#endif

  boolean  ps_dereg_pended = FALSE;
  boolean  page_retry =FALSE;
  boolean  mmcoord_cell_id_changed = FALSE;
  boolean  mm_to_only_reg_local =FALSE;
  boolean  mm_reg_cnf_only_for_reg_local = FALSE;
  boolean policy_change_pending = FALSE; 

#ifdef FEATURE_LTE
  emm_ctrl_data_type *emm_ctrl_data_ptr_tag = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr_tag != NULL);
#endif  

  mm_reg_ind_sent = FALSE;

  if (message->cmd.hdr.message_id == (int) RR_SERVICE_IND)
  {
#ifdef FEATURE_GSM_BAND_PREF
    mm_active_band = message->cmd.rr_service_ind.band_pref;
#endif
     
    if (mm_waiting_for_service_cnf || mm_waiting_for_stop_mode_cnf || mm_waiting_for_deact_cnf)
    {
#ifdef FEATURE_SGLTE
      if(mm_sglte_current_stack_status == SGLTE_MM_UPPER_LAYER_SPLITING && message->cmd.rr_service_ind.service_state == MM_AS_NO_SERVICE)
      {
        mm_no_service_ind_during_split = TRUE;
		MSG_HIGH_DS_1( MM_SUB, "=MM= NO service indication will be processed after split - %d", mm_no_service_ind_during_split);
      }	
#endif
#ifdef FEATURE_GPRS_PS_HANDOVER
      MSG_HIGH_DS_1(MM_SUB ,"=MM= Ignoring RR_SERVICE_IND - psho_status %d", gmm_psho_status);
      gmm_psho_status = GMM_PSHO_NONE;
#else
      MSG_HIGH_DS_0(MM_SUB, "=MM= Unexpected RR_SERVICE_IND");
#endif
    }
#ifdef FEATURE_WRLF_SYSTEM_SEL
    else if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE)
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= Ignoring RR_SERVICE_IND in RLF as deep sleep request from CM is expected");
    }
#endif
    else
    {
      service_state_type mm_saved_previous_service_state = mm_serving_plmn.mm_service_state ;
      mm_serving_plmn.hsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
#if defined(FEATURE_WRLF_SYSTEM_SEL) || defined(FEATURE_LTE)
      mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
#endif
#ifdef FEATURE_FEMTO_CSG
      mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif       
      switch (message->cmd.rr_service_ind.service_state)
      {
        case MM_AS_NO_SERVICE:

          MSG_HIGH_DS_0(MM_SUB, "=MM= Revd RR_SERVICE_IND, NO SERVICE");
#ifdef FEATURE_NAS_FREE_FLOAT
          gmmllc_reset_service_state();
#endif
          mm_serving_plmn.info.plmn.identity[0] = 0xFF;
          mm_serving_plmn.info.plmn.identity[1] = 0xFF;
          mm_serving_plmn.info.plmn.identity[2] = 0xFF;
          mm_serving_plmn.lac.lac[0]            = 0xFF;
          mm_serving_plmn.lac.lac[1]            = 0xFF;
          mm_serving_plmn.gmm_service_state     = SERVICE_NOT_AVAILABLE;
          mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
          mm_serving_plmn.network_op_mode  = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
          mm_serving_plmn.rac = 0xFF ;
          mm_coord_camped_cell_id = 0xFFFF;

#ifdef FEATURE_DUAL_SIM
         if (message->cmd.rr_service_ind.failure_cause == MM_RR_NO_RESOURCE_AVAILABLE)
         {
           mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_NO_RESOURCE;
		   if(mm_reg_waiting_for_reg_cnf)
		   {
		     mmcoord_mmr_reg_cnf.ue_oos = TRUE;
		   }
         }
         else
         {
           mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
         }
#elif defined(FEATURE_1XSRLTE)
         mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif
#ifdef FEATURE_GSM_DTM
          mm_serving_plmn.dtm_supported = FALSE;
#endif
          mm_serving_plmn.egprs_supported = FALSE;
          mm_mmr_cell_service_ind_pending = FALSE;
          mm_lu_pended_nmo_unknown = FALSE;
          
          gmm_foreground_search_pending   = FALSE;
          mm_service_ind_blocked_due_to_irat_cs_reg = FALSE;

#ifdef FEATURE_GPRS_PS_HANDOVER
          if (gmm_psho_status == GMM_PSHO_STARTED)
          {
            gmm_psho_status = GMM_PSHO_NONE;
            MSG_FATAL_DS( MM_SUB, "=MM= Revd  RR_SERVICE_IND :NO SERVICE - not expected",0,0,0 );
          }
#endif   
#ifdef FEATURE_GPRS_LAYER3_TX_CONFIRM
          gmm_transaction_id++;
#endif       
          /* Cancel obsolete CS & PS authentication attempts */
          gmm_cancel_authentication() ;
          mm_cancel_authentication() ;
          break;

        case MM_AS_SERVICE_AVAILABLE:

          MSG_HIGH_DS_1(MM_SUB ,"=MM= Revd RR_SERVICE_IND w/ SRV-AC=%d",
                     message->cmd.rr_service_ind.cell_access);

          if(message->cmd.rr_service_ind.cell_identity_valid)
          {
            if(sys_plmn_match(message->cmd.rr_service_ind.location_area_identification.PLMN_id,mm_serving_plmn.info.plmn) &&
               (message->cmd.rr_service_ind.location_area_identification.location_area_code[0] == mm_serving_plmn.lac.lac[0] &&
                message->cmd.rr_service_ind.location_area_identification.location_area_code[1] == mm_serving_plmn.lac.lac[1])&&
                mm_coord_camped_cell_id != message->cmd.rr_service_ind.cell_identity)
            {
              mmcoord_cell_id_changed = TRUE;
            }
            mm_coord_camped_cell_id = message->cmd.rr_service_ind.cell_identity;
          }
          else
          {
             mm_coord_camped_cell_id = 0xFFFF;
          }
			gmm_force_for_always_on_data=FALSE;
			mm_dds_switch_while_rrc_conn_active=FALSE;
          //only cell change no LA changed
          if (mm_retry_page_rsp == TRUE && 
#ifdef FEATURE_DUAL_SIM                
            (cm_get_operator_info((sys_modem_as_id_e_type)mm_sub_id) == OPERATOR_CMCC) &&
#else
            (cm_get_operator_info(SYS_MODEM_AS_ID_1) == OPERATOR_CMCC) &&
#endif
             message->cmd.rr_service_ind.cell_changed == TRUE && 
             mm_timer_status[TIMER_MM_PAGE_RETRY] != TIMER_STOPPED &&
             sys_plmn_match(message->cmd.rr_service_ind.location_area_identification.PLMN_id,mm_serving_plmn.info.plmn) &&
             (message->cmd.rr_service_ind.location_area_identification.location_area_code[0] == mm_serving_plmn.lac.lac[0] &&
                message->cmd.rr_service_ind.location_area_identification.location_area_code[1] == mm_serving_plmn.lac.lac[1]) && 
                message->cmd.rr_service_ind.cell_access != MM_AS_ACCESS_NO_CALLS && 
                (message->cmd.rr_service_ind.service_capability== SYS_SRV_CAPABILITY_CS_SERVICE_ONLY ||message->cmd.rr_service_ind.service_capability == SYS_SRV_CAPABILITY_CS_PS_SERVICE))
          {
             MSG_HIGH_DS_4(MM_SUB, "=MM= Checking replay in GSM : cell_changed = %d mm_retry_page_rsp = %d cell_access = %d service_capability = %d",message->cmd.rr_service_ind.cell_changed,mm_retry_page_rsp,message->cmd.rr_service_ind.cell_access,message->cmd.rr_service_ind.service_capability);
             page_retry = TRUE;
          }
      /*resetting flags if LA changed for native GSM calls*/
         if (mm_pending_lu_request != LTE_IRAT_LU && mm_state == MM_IDLE &&
            (!sys_plmn_match(message->cmd.rr_service_ind.location_area_identification.PLMN_id,mm_serving_plmn.info.plmn) || 
             !(message->cmd.rr_service_ind.location_area_identification.location_area_code[0] == mm_serving_plmn.lac.lac[0] &&
              message->cmd.rr_service_ind.location_area_identification.location_area_code[1] == mm_serving_plmn.lac.lac[1])))
              {
                MSG_HIGH_DS_1(MM_SUB, "=MM= LAI changed, don't replay page anymore mm_retry_page_rsp = %d",mm_retry_page_rsp); 
  				#ifdef FEATURE_GSM_CSMT                   
		        if (mm_retry_page_rsp == TRUE)
				#endif
		        {
		           mm_stop_timer(TIMER_MM_PAGE_RETRY);
		        }
               mm_last_page_type = PAGE_TYPE_NONE; 
               mm_retry_page_rsp = FALSE;
              }
          mm_serving_plmn.arfcn = message->cmd.rr_service_ind.arfcn;
          mm_serving_plmn.bsic = message->cmd.rr_service_ind.bsic;
          MSG_HIGH_DS_3(MM_SUB,"=MM= camped ARFCN NUM is %d, Band is %d and BSIC is %d", 
                      mm_serving_plmn.arfcn.num,mm_serving_plmn.arfcn.band,mm_serving_plmn.bsic);

          /* Store the ATT, MSCR and SGSNR information before routing to MM/GMM */

          mm_system_information.ATT = message->cmd.rr_service_ind.ATT;
          mm_system_information.mscr = message->cmd.rr_service_ind.MSCR;
          mm_band_indicator = message->cmd.rr_service_ind.band_indicator;
#ifdef FEATURE_GSM_GPRS
          if (message->cmd.rr_service_ind.gprs_sys_info_present)
          {
            mm_system_information.sgsnr = message->cmd.rr_service_ind.gprs_sys_info.sgsnr;
          }
#endif
#ifdef FEATURE_GPRS_PS_HANDOVER
          if (gmm_psho_status == GMM_PSHO_STARTED)
          {
            if ((message->cmd.rr_service_ind.gprs_sys_info_present) && 
                (message->cmd.rr_service_ind.ps_handover_indicator))
            {
              gmm_psho_status = GMM_PSHO_IN_PROGRESS;
            }
            else
            {
              gmm_psho_status = GMM_PSHO_NONE;
              MSG_FATAL_DS( MM_SUB, "=MM= Revd  RR_SERVICE_IND : PSHO or GPRS - not present",0,0,0 );            
            }
          }
          else
          {
            if ((gmm_psho_status == GMM_PSHO_IN_PROGRESS) && (message->cmd.rr_service_ind.cell_changed))
            {
              gmm_psho_status = GMM_PSHO_NONE;
              MSG_HIGH_DS_0(MM_SUB,"=MM= Revd  RR_SERVICE_IND : non-PSHO cell change");          
            }
            else if (message->cmd.rr_service_ind.ps_handover_indicator)
            {
              gmm_psho_status = GMM_PSHO_NONE;
              MSG_FATAL_DS( MM_SUB, "=MM= Revd  RR_SERVICE_IND : PSHO -present but not expected here",0,0,0 );          
            }
          }
#endif 
          /* Store the cell_access information */
          mm_prev_cell_access = mm_get_cs_cell_access(PROCEDURE_CS_REGISTRATION);
          mm_system_information.cell_access = message->cmd.rr_service_ind.cell_access;
          mm_send_mmr_cell_access_ind();
          /* --------------------------------------------
          ** Store the LAI where we are currently camped
          ** -------------------------------------------- */
          mm_serving_plmn.info.plmn.identity[0] =
            message->cmd.rr_service_ind.location_area_identification.PLMN_id.identity[0];
          mm_serving_plmn.info.plmn.identity[1] =
            message->cmd.rr_service_ind.location_area_identification.PLMN_id.identity[1];
          mm_serving_plmn.info.plmn.identity[2] =
            message->cmd.rr_service_ind.location_area_identification.PLMN_id.identity[2];

          memscpy ( mm_serving_plmn.lac.lac, LAC_SIZE,
                   message->cmd.rr_service_ind.location_area_identification.location_area_code,
                   LAC_SIZE );

          mm_serving_plmn.info.plmn_service_capability =
            message->cmd.rr_service_ind.service_capability;

          mm_serving_plmn.t3122_running =  message->cmd.rr_service_ind.t3122_running;

          MSG_HIGH_DS_1(MM_SUB ,"=MM= t3122_running is %d",message->cmd.rr_service_ind.t3122_running);

          mm_serving_plmn.network_op_mode = PS_DOMAIN_SYS_INFO_UNAVAILABLE;

#if defined FEATURE_DUAL_SIM || defined FEATURE_1XSRLTE
          mm_serving_plmn.info.acq_status = SYS_ACQ_STATUS_SUCCESS;
#endif

          policy_change_pending = mmcoord_policy_change_pending();

#ifdef FEATURE_SGLTE
         if((policy_change_pending == TRUE) ||
             ((mm_process_if_both_stack_has_camped_non_sglte_plmn(mm_cs_stack_id)== TRUE)||
             (mm_is_sglteplmn_in_sglte_roam(mm_serving_plmn.info.plmn ) == TRUE)||
             (mm_is_non_sglte_plmn_in_sglte_home(mm_serving_plmn.info.plmn)==TRUE)))
#else
         if(policy_change_pending == TRUE)
#endif      
        
          {
            mmcoord_capability_chg_pend_limited_service(policy_change_pending, FALSE);
             return;
          }
#ifdef FEATURE_SGLTE
          else if (mm_sglte_state4_trigger_split()==TRUE)
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= Spliting in State 4 ,GSM CS only ") ;
            return;
          }
 #endif
#ifdef FEATURE_GSM_GPRS
          if (message->cmd.rr_service_ind.gprs_sys_info_present)
          {
            mm_serving_plmn.network_op_mode = message->cmd.rr_service_ind.gprs_sys_info.nmo;
            mm_serving_plmn.rac             = message->cmd.rr_service_ind.gprs_sys_info.rac;
            if(
#ifdef FEATURE_GSM_DTM
               mm_serving_plmn.dtm_supported != message->cmd.rr_service_ind.gprs_sys_info.dtm_supported ||
#endif
               mm_serving_plmn.egprs_supported != message->cmd.rr_service_ind.gprs_sys_info.egprs_supported)
            {
              mm_mmr_cell_service_ind_pending = TRUE;
            }
#ifdef FEATURE_GSM_DTM
            mm_serving_plmn.dtm_supported   = message->cmd.rr_service_ind.gprs_sys_info.dtm_supported;
            MSG_HIGH_DS_1(MM_SUB ,"=MM= DTM=%d",mm_serving_plmn.dtm_supported);
#endif
            mm_serving_plmn.egprs_supported = message->cmd.rr_service_ind.gprs_sys_info.egprs_supported;

            MSG_HIGH_DS_3( MM_SUB, "=MM= GMM sys info: NMO=%d, RAC=%d, EGPRS=%d",
                       mm_serving_plmn.network_op_mode, mm_serving_plmn.rac, mm_serving_plmn.egprs_supported );

            mm_serving_plmn.gmm_service_state = SERVICE_AVAILABLE;
          }
          else
#endif /* #ifdef FEATURE_GSM_GPRS */
          {
            mm_serving_plmn.gmm_service_state = SERVICE_NOT_AVAILABLE;
            mm_serving_plmn.rac               = 0xFF;
            if(
#ifdef FEATURE_GSM_DTM
               mm_serving_plmn.dtm_supported != FALSE ||
#endif
               mm_serving_plmn.egprs_supported != FALSE)
            {
              mm_mmr_cell_service_ind_pending = TRUE;
            }
#ifdef FEATURE_GSM_DTM
            mm_serving_plmn.dtm_supported     = FALSE;
#endif
            mm_serving_plmn.egprs_supported   = FALSE;
          }
#ifdef FEATURE_LTE
          if((gmm_state == GMM_DEREGISTERED || mm_possible_imsi_attach == TRUE) &&
             (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                SYS_NETWORK_SELECTION_MODE_LIMITED) && 
             (mm_serving_plmn.info.sim_state != SYS_SIM_STATE_CS_PS_INVALID) )
          {
            if(emm_validate_attach_plmn(emm_ctrl_data_ptr_tag, &mm_serving_plmn.info.plmn) == FALSE)
            {
              mm_update_mm_serving_plmn_info( SYS_SIM_STATE_CS_PS_INVALID,
                                 SYS_SRV_DOMAIN_NO_SRV,
                                 TRUE,
                                 SYS_SRV_STATUS_LIMITED,
                                 FALSE);
#ifdef FEATURE_DUAL_SIM  
              mm_gmm_set_non_illegal_cause();
#endif
            }
          }

          if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
          {
            mm_send_rr_irat_allowed_status_ind();
          }
          mmcoord_locally_deactivate_isr_if_needed();
#endif
#ifdef FEATURE_LTE
          mmcoord_reevaluate_rat_pri_list(mm_serving_plmn.info.plmn);
          if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
          {
            mm_send_rr_irat_allowed_status_ind();
          }
#endif

#ifdef FEATURE_NAS_RAU_ON_MS_RAC_CHANGE
          if((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                                          SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION)&&
              (sys_plmn_id_is_undefined(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) == TRUE)&&
              (mmcoord_check_if_ms_rat_capability_changed() == TRUE))
          {
            gmm_ra_update_pending = TRUE;
          }
#endif

           if(page_retry && mm_state == MM_IDLE)
           {
             MSG_HIGH_DS_1(MM_SUB, "=MM= Resend page response after RACH failure with type %d",mm_last_page_type); 
             mm_send_paging_response(
#ifdef FEATURE_LTE_TO_GSM
                mm_last_page_type
#endif
                            );
              mm_stop_timer(TIMER_MM_PAGE_RETRY);
              mm_last_page_type = PAGE_TYPE_NONE; //resetting to dafault after first try
              mm_retry_page_rsp = FALSE;
           }
          break;

        default:
          MSG_ERROR_DS( MM_SUB, "=MM= Unexpected service_state %d",
                message->cmd.rr_service_ind.service_state, 0,0 );
          break;
      } /* end switch (((rrc_act_cnf_T *) message)->service_status) */

      /* ----------------------------------------------------------------
      ** Route RR_SERVICE_IND to MM and/or GMM based on MS operation mode
      ** ---------------------------------------------------------------- */
     if(cm_per_subs_is_reg_allowed(mm_serving_plmn.info.plmn,
                     mm_serving_plmn.info.active_rat,(sys_modem_as_id_e_type)mm_as_id,mm_set_cm_stack_id(mm_as_id)) == FALSE)
     {
            MSG_HIGH_DS_0( MM_SUB, "=MM= Sent Limited as don't register when CT disabled.");
            mm_substate_control( MM_STD_LIMITED_SERVICE );

              /* Set the Serving PLMN service status to Limited */
            mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                               mm_serving_plmn.info.reg_service_domain,
                                               TRUE,
                                               SYS_SRV_STATUS_LIMITED,
                                               TRUE );            
            if (mm_reg_waiting_for_reg_cnf)
             {
               /* Set the REG CNF plmn service state to the Serving PLMN info */
               mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
               mmcoord_mmr_reg_cnf.service_state.service_status = SYS_SRV_STATUS_LIMITED;          
               mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;            
               mm_send_mmr_reg_cnf();
               mm_reg_waiting_for_reg_cnf = FALSE;
               mm_ready_to_send_reg_cnf   = FALSE;
             }
             else 
             {
               mmr_service_ind_s_type  mmr_service_ind_temp;
               mmr_service_ind_temp.service_state = mm_serving_plmn.info;
               mmr_service_ind_temp.service_state.service_status = SYS_SRV_STATUS_LIMITED;   
               mmr_service_ind_temp.cause = MMR_CAUSE_NOT_SPECIFIED;               
               mm_send_mmr_service_ind(&mmr_service_ind_temp);
             }    
             gmm_set_substate( GMM_LIMITED_SERVICE );
			 if ((mm_held_cnm_service_req.present) && 
                  (mm_held_cnm_service_req.establishment_cause == (int) MM_RR_EMERGENCY_CALL) &&
                  ((mm_held_cnm_service_req.interat_status == MM_AS_INTERRAT_RESELECTION) || 
                  (mm_held_cnm_service_req.interat_status == MM_AS_INTERRAT_CC_ORDER)))
             {
               mm_check_pending_requests();
               MSG_HIGH_DS_1(MM_SUB ,"=MM= Processing Pending E-call after moving to different cell of INTERRAT: %d", 
                           mm_held_cnm_service_req.interat_status);
               mm_held_cnm_service_req.interat_status = MM_AS_SUCCESS ;
             }
     }
     else 
     {
      if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
      {
#ifdef FEATURE_GSM_GPRS
        gmm_handle_message( message );
#else
        MSG_HIGH_DS_0(MM_SUB, "=MM= Feature GPRS is not supported") ;
#endif /* #ifdef FEATURE_GSM_GPRS */
      }
      else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) &&
               (gmm_state != GMM_DEREGISTERED_INITIATED))
      {
        ps_dereg_pended = FALSE;
#ifdef FEATURE_DUAL_SIM
        for(mm_other_as_id = SYS_MODEM_AS_ID_1; mm_other_as_id < MAX_NAS_STACKS ; mm_other_as_id++)
        {
          if((mm_other_as_id != (sys_modem_as_id_e_type)mm_as_id) &&
             (message->cmd.rrc_service_ind.service_status == MM_AS_NO_SERVICE)&&
             (mmcoord_reg_req_pending_sim[mm_sub_id] == TRUE) &&
             (gmm_force_local_detach_sim[mm_other_as_id] == TRUE) &&
             (gmm_state_sim[mm_other_as_id] == GMM_DEREGISTERED_INITIATED))
        {
            MSG_HIGH_DS_0(MM_SUB,"=MM= recieved service indication with no service during DDS switch");
            mm_serving_plmn.mm_service_state = SERVICE_NOT_AVAILABLE;
            ps_dereg_pended = TRUE;
            mm_as_id_local = (sys_modem_as_id_e_type)mm_as_id;
            mm_set_as_id(mm_other_as_id);
            gmm_handle_gprs_detach_low_level_failure();
            mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
            mm_set_as_id(mm_as_id_local);
          }
        }
#endif
        if(!ps_dereg_pended)
        {
          mm_handle_message( message );
        }
      }
      else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) ||
               (gmm_state == GMM_DEREGISTERED_INITIATED))
      {
        if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
        {
#ifdef FEATURE_LTE
          mm_csfb_process_service_ind_nmo1(message);
#endif /*FEATURE_LTE*/ 
          gmm_handle_message( message );
        }
        else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
        {
          mm_handle_message( message );
#ifdef FEATURE_GSM_GPRS
          gmm_handle_message( message );
#else
          MSG_HIGH_DS_0(MM_SUB, "=MM= Feature GPRS is not supported") ;
#endif /* #ifdef FEATURE_GSM_GPRS */
        }
      }
      else
      {
        MSG_FATAL_DS( MM_SUB, "=MM= Invalid MS op mode %d", (int)mm_serving_plmn.ms_op_mode, 0,0 );
      }
     }

      /* ----------------------------------------------------------------
      ** Store the System Information provided in the Service IND message
      ** ------------------------------------------------------------- */
      mmgsm_store_system_information( message );

      if (message->cmd.rr_service_ind.service_state == MM_AS_SERVICE_AVAILABLE)
      {
        /* Store the current LAI information*/
        gmm_current_lai.PLMN_id = mm_serving_plmn.info.plmn;
        memscpy(gmm_current_lai.location_area_code, LAC_SIZE, mm_serving_plmn.lac.lac, LAC_SIZE);
        gmm_current_rac = mm_serving_plmn.rac;
      }

#ifdef FEATURE_GSM_DTM
      /*************************************************************************
      ** With DTM Feature in RR, RR can read new RAC during handover in connected
      ** mode (e.g. voice call), however, it cannot read NMO. Due to this, 
      ** RR_SERVICE_IND_FROM_OTA_MSG indicates change of RA, however, no change of
      ** NMO. Change of NMO is indicated in RR_SERVICE_IND following camping after
      ** connection release. However, NAS was designed to honour change of NMO 
      ** only if RA is also changed. To handle this particular case of DTM, NAS 
      ** is modified to honour change of NMO if No Change in RA is detected.
      **************************************************************************/      
      if (((message->cmd.rr_service_ind.gprs_sys_info_present) &&
           (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) &&
           (mm_serving_plmn.network_op_mode != gmm_previous_network_op_mode) &&
           (gmm_previous_network_op_mode != PS_DOMAIN_SYS_INFO_UNAVAILABLE)) &&      
           
          !((gmm_rai_changed( gmm_previous_serving_rai.location_area_code,
                              gmm_previous_serving_rai.routing_area_code,
                              mm_serving_plmn.lac, mm_serving_plmn.rac )) ||
            (gmm_plmn_changed( gmm_previous_serving_rai.plmn_id,
                                    mm_serving_plmn.info.plmn))) &&
                                    
          ((gmm_state == GMM_REGISTERED) || 
           (gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED)) &&
           (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                            SYS_NETWORK_SELECTION_MODE_LIMITED)&&
           (cm_per_subs_is_reg_allowed(mm_serving_plmn.info.plmn,
                     mm_serving_plmn.info.active_rat,(sys_modem_as_id_e_type)mm_as_id,mm_set_cm_stack_id(mm_as_id)) == TRUE))
      {
        MSG_HIGH_DS_0(MM_SUB, "=MM= NMO Change w/o change of RA");      
#ifdef FEATURE_NAS_REL10        
        if(((mm_timer_status[TIMER_T3346] == TIMER_STOPPED)&&
           (mm_timer_status[TIMER_T3246] == TIMER_STOPPED))
#ifdef FEATURE_NAS_REL11
/* ---------------------------- ---------------------------- ---------------------------- ----------------------------
**24.008: 4.7.5.1.5 (j)	Timer T3346 is running
**The MS shall not start the routing area updating procedure unless the MS is accessing the network with access class 11 ? 15
**4.4.4.9 (j)Timer T3246 is running
**The MM connection establishment shall not be initiated unless the MS is accessing the network with access class 11 ? 15. 
** ---------------------------- ---------------------------- ---------------------------- ----------------------------*/
            || ((mm_timer_status[TIMER_T3346] == TIMER_ACTIVE || mm_timer_status[TIMER_T3246] == TIMER_ACTIVE)&&
                (mm_is_rrc_access_class_11_to_15() == TRUE))
#endif
            )
        {
          gmm_change_of_network_mode_of_operation();      
        }
        else if( mm_timer_status[TIMER_T3246] == TIMER_STOPPED
/* ---------------------------- ---------------------------- ---------------------------- ----------------------------
**4.4.4.9 (j)Timer T3246 is running
**The MM connection establishment shall not be initiated unless the MS is accessing the network with access class 11 ? 15. 
** ---------------------------- ---------------------------- ---------------------------- ----------------------------*/
#ifdef FEATURE_NAS_REL11
                  ||((mm_timer_status[TIMER_T3246] == TIMER_ACTIVE)&&
                       (mm_is_rrc_access_class_11_to_15() == TRUE))
#endif
               )
        {
          gmm_change_of_nmo_during_t3346_active();
        }
#else
        gmm_change_of_network_mode_of_operation();      
#endif 
      }
#endif 
      if (message->cmd.rr_service_ind.service_state ==  MM_AS_SERVICE_AVAILABLE) 
      {
        if (mm_needs_to_send_mmr_service_ind || (mmcoord_cell_id_changed && !mm_reg_ind_sent))
        {
          mm_reg_cnf_only_for_reg_local = mmcoord_mmr_reg_cnf.only_for_reg;       
          mm_to_only_reg_local = mm_to_only_reg;
          if((mmcoord_cell_id_changed && !mm_reg_ind_sent) && !mm_needs_to_send_mmr_service_ind)
          {
            mm_to_only_reg = TRUE;
          }
          /*************************************************************
          * Block the service indication to REG in the following cases:*
          * 1. LU is going on.                                         *
          * 2. RAU is going on and no HO.                              * 
          * During Inter-RAT HO UI has to be updated ASAP even though  *
          * RAU is going on.                                           * 
          *************************************************************/
          if ((mm_state == MM_WAIT_FOR_RR_CONNECTION_LU) ||
              (mm_state == MM_LOCATION_UPDATE_INITIATED) ||
              (((mm_state != MM_CONNECTION_ACTIVE) &&
                (mm_state != MM_WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION) &&
                (mm_state != MM_WAIT_FOR_RR_CONNECTION_MM)
#ifdef FEATURE_LTE
                 && !((csfb_state == MM_CSFB_STATE_MT_CALL_ORIG)&&
                      (mm_state == MM_WAIT_FOR_NETWORK_COMMAND))
#endif
                ) &&
               ((gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED)||
                (gmm_state == GMM_REGISTERED_INITIATED))))
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= REG will be updated about new LA after registration completion");  
            mm_needs_to_send_mmr_service_ind = FALSE;
          }
          else if(mmcoord_cell_id_changed && !mm_needs_to_send_mmr_service_ind && mm_reg_waiting_for_reg_cnf)
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= REG will be updated about new LA after registration completion");  
          }
          else
          {
            mmr_service_ind.cause = MMR_CAUSE_NOT_SPECIFIED;
            mmr_service_ind.service_state = mm_serving_plmn.info;
            mm_send_mmr_service_ind( &mmr_service_ind );
          }
          mm_to_only_reg = mm_to_only_reg_local;
          if((mmcoord_mmr_reg_cnf.only_for_reg == mm_reg_cnf_only_for_reg_local == TRUE))
          {
            mmcoord_mmr_reg_cnf.only_for_reg = TRUE;
          }
          else
          {
            mmcoord_mmr_reg_cnf.only_for_reg = FALSE;
          }

        }
      }
      mmcoord_cell_id_changed = FALSE;
      if ((mm_mmr_cell_service_ind_pending) && 
           (mmcoord_mmr_reg_cnf.only_for_reg == FALSE) &&
             (sys_plmn_match( mmcoord_mmr_reg_cnf.service_state.plmn, mm_serving_plmn.info.plmn)) &&
             (mmcoord_mmr_reg_cnf.rac == mm_serving_plmn.rac)
#ifdef FEATURE_GSM_BAND_PREF
             &&(mmcoord_mmr_reg_cnf.service_state.lac == b_unpackw(mm_serving_plmn.lac.lac,0,16))
#endif /*FEATURE_GSM_BAND_PREF*/
             &&(mmcoord_mmr_reg_cnf.service_state.active_rat == mm_serving_plmn.info.active_rat))
      {
        mm_send_mmr_cell_service_ind();     
      } 
      if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) &&
          (mm_saved_previous_service_state == mm_serving_plmn.mm_service_state) &&
          (is_gsm_mode() && message->cmd.rr_service_ind.cell_changed) && mm_state == MM_IDLE)
      {
#ifdef FEATURE_GSTK
        mm_send_gstk_mm_idle_info() ;
#endif
      }

      /* Log the Cell info */
      mmsend_mm_info_log_packet();
#ifdef FEATURE_FEMTO_CSG
      mm_serving_csg_id = mm_serving_plmn.info.csg_info.csg_id;
#endif 

    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RR msg %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rr_service_ind() */



/*===========================================================================

FUNCTION MMCOORD_PROCESS_RR_SERVICE_IND_FROM_OTA_MSG

DESCRIPTION
  Note: This primitive is expected by MM in RR Connected Mode only. However
  even if received with NO Service condition we are currently handling to
  prevent any issues.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

#ifdef FEATURE_GSM_GPRS
#ifdef FEATURE_GSM_DTM

void mmcoord_process_rr_service_ind_from_ota_msg( mm_cmd_type *message )
{
   mmr_service_ind_s_type  mmr_service_ind;
#ifdef FEATURE_LTE
   mm_csfb_state_type  csfb_state = mm_csfb_state;
#endif

   boolean policy_change_pending = FALSE; 

   boolean   mmcoord_cell_id_changed = FALSE;
   boolean   mm_to_only_reg_local =FALSE;
   boolean  mm_reg_cnf_only_for_reg_local = FALSE;
   mm_reg_ind_sent = FALSE;

  if (message->cmd.hdr.message_id == (int) RR_SERVICE_IND_FROM_OTA_MSG)
  {
#ifdef FEATURE_GSM_BAND_PREF
    mm_active_band = message->cmd.rr_service_ind_from_ota_msg.band_pref;
#endif
    if (mm_waiting_for_service_cnf || mm_waiting_for_stop_mode_cnf)
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= Unexpected RR_SERVICE_IND_FROM_OTA_MSG");
    }
    else
    { 
      mm_manual_mode_irat_ho_in_non_user_sel_cell = FALSE;
      mm_serving_plmn.hsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;            
#if defined(FEATURE_WRLF_SYSTEM_SEL) || defined(FEATURE_LTE)
      mm_serving_plmn.info.no_service_cause  = SYS_NO_SVC_CAUSE_NORMAL;
      mm_serving_plmn.info.suitable_search_time = 0;
#endif
#ifdef FEATURE_FEMTO_CSG
      mm_serving_plmn.info.csg_info.csg_id = SYS_CSG_ID_INVALID;
#endif       
      switch (message->cmd.rr_service_ind_from_ota_msg.service_state)
      {
        /* -----------------------------------------------------------
        ** This is NOT expected as this primitive is to be used by RR
        ** in RR Connected Mode only. We are still handling this for now
        ** --------------------------------------------------------- */
        case MM_AS_NO_SERVICE:
          
          mm_serving_plmn.info.plmn.identity[0] = 0xFF;
          mm_serving_plmn.info.plmn.identity[1] = 0xFF;
          mm_serving_plmn.info.plmn.identity[2] = 0xFF;
          mm_serving_plmn.lac.lac[0]            = 0xFF;
          mm_serving_plmn.lac.lac[1]            = 0xFF;
          mm_serving_plmn.gmm_service_state     = SERVICE_NOT_AVAILABLE;
          mm_serving_plmn.info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
          mm_serving_plmn.network_op_mode    = PS_DOMAIN_SYS_INFO_UNAVAILABLE;
          mm_serving_plmn.rac = 0xFF ;

          mm_serving_plmn.dtm_supported = FALSE;
          mm_serving_plmn.egprs_supported = FALSE;

          mm_mmr_cell_service_ind_pending = FALSE;

          mm_coord_camped_cell_id  = 0xFFFF;

          gmm_foreground_search_pending   = FALSE;
          mm_service_ind_blocked_due_to_irat_cs_reg = FALSE;

          MSG_HIGH_DS_0(MM_SUB, "=MM= Revd RR_SERVICE_IND_FROM_OTA_MSG, NO SERVICE");
          MSG_ERROR_DS( MM_SUB, "=MM= RR_SERVICE_IND_FROM_OTA_MSG Not Expected with NO SERVICE", 0,0,0 );          
          break;

        case MM_AS_SERVICE_AVAILABLE:

          if(message->cmd.rr_service_ind_from_ota_msg.cell_identity_valid)
          {
            if(sys_plmn_match(message->cmd.rr_service_ind_from_ota_msg.location_area_identification.PLMN_id,mm_serving_plmn.info.plmn) &&
               (message->cmd.rr_service_ind_from_ota_msg.location_area_identification.location_area_code[0] == mm_serving_plmn.lac.lac[0] &&
                message->cmd.rr_service_ind_from_ota_msg.location_area_identification.location_area_code[1] == mm_serving_plmn.lac.lac[1])&&
                mm_coord_camped_cell_id != message->cmd.rr_service_ind_from_ota_msg.cell_identity)
            {
              mmcoord_cell_id_changed = TRUE;
            }
            mm_coord_camped_cell_id = message->cmd.rr_service_ind_from_ota_msg.cell_identity;
          }
          else
          {
            mm_coord_camped_cell_id = 0xFFFF;
          }
		  gmm_force_for_always_on_data=FALSE;
		  mm_dds_switch_while_rrc_conn_active=FALSE;

          mm_serving_plmn.arfcn = message->cmd.rr_service_ind_from_ota_msg.arfcn;
          mm_serving_plmn.bsic = message->cmd.rr_service_ind_from_ota_msg.bsic;
          MSG_HIGH_DS_4(MM_SUB,"=MM= Revd RR_SERVICE_IND_FROM_OTA_MSG w/ SRV-AC=%d camped ARFCN NUM is %d, Band is %d and BSIC is %d",
			          message->cmd.rr_service_ind_from_ota_msg.cell_access,
                      mm_serving_plmn.arfcn.num,mm_serving_plmn.arfcn.band,mm_serving_plmn.bsic);

          /* Store the ATT, MSCR and SGSNR information before routing to MM/GMM */

/*    The ATT flag should be stored only when RRC/RR connection does'nt exist and when RRC/RR connection
        exists we get the service indication message in the service_ind_from_OTA message, which we should 
        ignore.
          
*/          
          /*
          ** During HO from MSC-A (connection initiated in MSC-A, R99) to MSC-B (R98 or erlier), 
          ** MSCR should remain same as received in MSC-A. MSCR is not given in dedicated mode.
          ** So MSCR received in OTA MSG is ignored in NAS. MSC-B worked as relay MSC (29.002).
          ** mm_system_information.mscr = message->cmd.rr_service_ind_from_ota_msg.MSCR;
          */

          if (message->cmd.rr_service_ind_from_ota_msg.gprs_sys_info_present)
          {
            mm_system_information.sgsnr = message->cmd.rr_service_ind_from_ota_msg.gprs_sys_info.sgsnr;
          }

          /* Store the cell_access information */
          mm_prev_cell_access = mm_system_information.cell_access;
          mm_system_information.cell_access = message->cmd.rr_service_ind_from_ota_msg.cell_access;
          mm_band_indicator = message->cmd.rr_service_ind_from_ota_msg.band_indicator;
          mm_send_mmr_cell_access_ind();
          /* --------------------------------------------
          ** Store the LAI where we are currently camped
          ** -------------------------------------------- */
          mm_serving_plmn.info.plmn.identity[0] =
            message->cmd.rr_service_ind_from_ota_msg.location_area_identification.PLMN_id.identity[0];
          mm_serving_plmn.info.plmn.identity[1] =
            message->cmd.rr_service_ind_from_ota_msg.location_area_identification.PLMN_id.identity[1];
          mm_serving_plmn.info.plmn.identity[2] =
            message->cmd.rr_service_ind_from_ota_msg.location_area_identification.PLMN_id.identity[2];

          memscpy ( mm_serving_plmn.lac.lac, LAC_SIZE,
                   message->cmd.rr_service_ind_from_ota_msg.location_area_identification.location_area_code,
                   LAC_SIZE );

          mm_serving_plmn.info.plmn_service_capability =
            message->cmd.rr_service_ind_from_ota_msg.service_capability;

          mm_serving_plmn.t3122_running =  message->cmd.rr_service_ind_from_ota_msg.t3122_running;

          MSG_HIGH_DS_1(MM_SUB ,"=MM= t3122_running is %d",message->cmd.rr_service_ind_from_ota_msg.t3122_running);

          if (message->cmd.rr_service_ind_from_ota_msg.gprs_sys_info_present)
          {
            /* As RR cannot read NMO in connected mode, making current NMO same as previous NMO */          
            if((gmm_previous_network_op_mode != PS_DOMAIN_SYS_INFO_UNAVAILABLE))
            {
              mm_serving_plmn.network_op_mode = gmm_previous_network_op_mode;
            }
            else
            {
              mm_serving_plmn.network_op_mode = NETWORK_OP_MODE_2;
            }
              
            mm_serving_plmn.rac             = message->cmd.rr_service_ind_from_ota_msg.gprs_sys_info.rac;
            if(mm_serving_plmn.dtm_supported != message->cmd.rr_service_ind_from_ota_msg.gprs_sys_info.dtm_supported ||
               mm_serving_plmn.egprs_supported != message->cmd.rr_service_ind_from_ota_msg.gprs_sys_info.egprs_supported )
            {
              mm_mmr_cell_service_ind_pending = TRUE;
            }
            mm_serving_plmn.dtm_supported   = message->cmd.rr_service_ind_from_ota_msg.gprs_sys_info.dtm_supported;
            mm_serving_plmn.egprs_supported = message->cmd.rr_service_ind_from_ota_msg.gprs_sys_info.egprs_supported;

            mm_serving_plmn.gmm_service_state = SERVICE_AVAILABLE;

            MSG_HIGH_DS_4( MM_SUB, "=MM= GMM sys info: NMO=%d, RAC=%d, DTM=%d, EGPRS=%d",
                       mm_serving_plmn.network_op_mode, mm_serving_plmn.rac, mm_serving_plmn.dtm_supported, mm_serving_plmn.egprs_supported );

          }
          if ((mm_serving_plmn.network_op_mode != NETWORK_OP_MODE_1) &&
               (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS))
          {
            mm_lu_pended_nmo_unknown = TRUE;
          }       
#ifdef FEATURE_LTE
          if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
          {
            mm_send_rr_irat_allowed_status_ind();
          }
          mmcoord_locally_deactivate_isr_if_needed();
#endif      

          policy_change_pending = mmcoord_policy_change_pending();

          if(policy_change_pending == TRUE)
          {
            mmcoord_capability_chg_pend_limited_service(policy_change_pending, FALSE);
            return;
          }
				   if ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL ||
				   	    mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION) && 
				   	    (!(PLMN_MATCH(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn.identity,mm_serving_plmn.info.plmn.identity) 
							  || (sys_eplmn_list_equivalent_plmn(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) 
                          && sys_eplmn_list_equivalent_plmn(mm_serving_plmn.info.plmn)))) && 
                        ((mm_state == MM_CONNECTION_ACTIVE) ||
                         (mm_state == MM_WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION) ||
                         (mm_state ==MM_WAIT_FOR_RR_CONNECTION_MM)))
					{
				       MSG_HIGH_DS_0(MM_SUB, "=MM= Only for UI is TRUE");
                       mm_manual_mode_irat_ho_in_non_user_sel_cell = TRUE;
                    }


          break;

        default:
          MSG_ERROR_DS( MM_SUB, "=MM= Unexpected service_state %d",
                message->cmd.rr_service_ind_from_ota_msg.service_state, 0,0 );
          break;
      } /* end switch (message->cmd.rr_service_ind_from_ota_msg.service_state) */

      /* ----------------------------------------------------------------
      ** Route RR_SERVICE_IND_FROM_OTA_MSG to MM and/or GMM based on MS operation mode
      ** ---------------------------------------------------------------- */
      if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
      {
        gmm_handle_message( message );
      }
      else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) &&
               (gmm_state != GMM_DEREGISTERED_INITIATED))
      {
        mm_handle_message( message );
      }
      else if ((mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS) ||
               (gmm_state == GMM_DEREGISTERED_INITIATED))
      {
        if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
        {
          gmm_handle_message( message );
        }
        else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
        {
          mm_handle_message( message );
          gmm_handle_message( message );
        }
      }
      else
      {
        MSG_FATAL_DS( MM_SUB, "=MM= Invalid MS op mode %d", (int)mm_serving_plmn.ms_op_mode, 0,0 );
      }

      /* ----------------------------------------------------------------
      ** Store the System Information provided in the Service IND message
      ** ------------------------------------------------------------- */
      mmgsm_store_system_information( message );

      if (message->cmd.rr_service_ind_from_ota_msg.service_state ==  MM_AS_SERVICE_AVAILABLE) 
      {
        if (mm_needs_to_send_mmr_service_ind || (mmcoord_cell_id_changed && !mm_reg_ind_sent))
        {
          mm_to_only_reg_local = mm_to_only_reg;
          mm_reg_cnf_only_for_reg_local = mmcoord_mmr_reg_cnf.only_for_reg;
          if ((mmcoord_cell_id_changed && !mm_reg_ind_sent) && !mm_needs_to_send_mmr_service_ind)
          {
            mm_to_only_reg = TRUE;
          }
          /*************************************************************
          * Block the service indication to REG in the following cases:*
          * 1. LU is going on.                                         *
          * 2. RAU is going on and no HO.                              * 
          * During Inter-RAT HO UI has to be updated ASAP even though  *
          * RAU is going on.                                           * 
          *************************************************************/
          if ((mm_state == MM_WAIT_FOR_RR_CONNECTION_LU) ||
              (mm_state == MM_LOCATION_UPDATE_INITIATED) ||
              (((mm_state != MM_CONNECTION_ACTIVE) &&
                (mm_state != MM_WAIT_FOR_ADDITIONAL_OUTGOING_MM_CONNECTION)
#ifdef FEATURE_LTE
                 && (mm_state != MM_WAIT_FOR_RR_CONNECTION_MM)
                 && !((csfb_state == MM_CSFB_STATE_MT_CALL_ORIG)&&
                      (mm_state == MM_WAIT_FOR_NETWORK_COMMAND))
#endif
                ) &&
               ((gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED)||
                (gmm_state == GMM_REGISTERED_INITIATED))))
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= REG will be updated about new LA after registration completion");  
            mm_needs_to_send_mmr_service_ind = FALSE;
          }
          else if(mmcoord_cell_id_changed && !mm_needs_to_send_mmr_service_ind && mm_reg_waiting_for_reg_cnf)
          {
            MSG_HIGH_DS_0(MM_SUB, "=MM= REG will be updated about new LA after registration completion");  
          }
          else
          {
            mmr_service_ind.cause = MMR_CAUSE_NOT_SPECIFIED;
            mmr_service_ind.service_state = mm_serving_plmn.info;
            mm_send_mmr_service_ind( &mmr_service_ind );
          }
          mm_to_only_reg = mm_to_only_reg_local;
          mmcoord_mmr_reg_cnf.only_for_reg = mm_reg_cnf_only_for_reg_local;
        }
      }
      mmcoord_cell_id_changed = FALSE;
      if ((mm_mmr_cell_service_ind_pending) && 
             (mmcoord_mmr_reg_cnf.only_for_reg == FALSE) &&
             (sys_plmn_match( mmcoord_mmr_reg_cnf.service_state.plmn, mm_serving_plmn.info.plmn)) &&
             (mmcoord_mmr_reg_cnf.rac == mm_serving_plmn.rac) &&
             (mmcoord_mmr_reg_cnf.service_state.lac == b_unpackw(mm_serving_plmn.lac.lac,0,16)) &&
             (mmcoord_mmr_reg_cnf.service_state.active_rat == mm_serving_plmn.info.active_rat))
      {
        mm_send_mmr_cell_service_ind();     
      } 

      if (message->cmd.rr_service_ind_from_ota_msg.service_state == MM_AS_SERVICE_AVAILABLE)
      {
        /* Store the current LAI information*/
        gmm_current_lai.PLMN_id = mm_serving_plmn.info.plmn;
        memscpy(gmm_current_lai.location_area_code, LAC_SIZE, mm_serving_plmn.lac.lac, LAC_SIZE);
        gmm_current_rac = mm_serving_plmn.rac;
      }

      /* Log the Cell info */
      mmsend_mm_info_log_packet();
#ifdef FEATURE_FEMTO_CSG
      mm_serving_csg_id = mm_serving_plmn.info.csg_info.csg_id;
#endif 

    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RR msg %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rr_service_ind_from_ota_msg() */

#endif /* FEATURE_GSM_DTM */
#endif /* FEATURE_GSM_GPRS */




/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_PLMN_LIST_CNF

DESCRIPTION
  Function to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_plmn_list_cnf( mm_cmd_type *message )
{
  if (message->cmd.rrc_plmn_list_cnf.message_header.message_id == (int) RRC_PLMN_LIST_CNF)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_PLMN_LIST_CNF");

    /* -----------------------------------------------------------------------
    ** Pass RRC_LAI_LIST_CNF to both MM & GMM independent of MS operation mode
    ** ----------------------------------------------------------------------- */
    mm_handle_message( message );
    gmm_handle_message( message );
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RRC message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_lai_list_cnf() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_ABORT_IND

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_abort_ind( mm_cmd_type *message )
{
  if (message->cmd.rrc_abort_ind.message_header.message_id == (int) RRC_ABORT_IND)
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= MM received RRC_ABORT_IND with cause %d",message->cmd.rrc_abort_ind.abort_cause);

#if defined FEATURE_SGLTE
      /* Puting this check for the case when RRC abort received after PS merged to GSM
              STAT4 check will take care that active rat will GSM , and as it is sglte mode message from Wrrc not expected
          */
    if ( MM_IS_IN_SGLTE_STATE_4 )
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= RRC ABORT received after PS merged to GSM , ignoring RRC_ABORT_IND ");
    } 
    else
#endif

    if( (!is_umts_mode()) && (rrc_connection_status == MMRRC_IDLE))
    {
      /*putting this here from above because not want to set for SGLTE and
        keep the existing behaviour for other case  */
      if(mm_waiting_for_abort_cnf == TRUE)
      {
        mm_waiting_for_abort_cnf = FALSE;
      }
      MSG_HIGH_DS_0(MM_SUB,"=MM= MM received RRC_ABORT_IND when not in UMTS mode so ignoring it");
    }
    else
    {
      /*putting this here from above because not want to set for SGLTE and
        keep the existing behaviour for other case  */
      if(mm_waiting_for_abort_cnf == TRUE)
      {
        mm_waiting_for_abort_cnf = FALSE;
      }
      /* --------------------------------------------------------------------
      ** Pass RRC_ABORT_IND to both MM & GMM independent of MS operation mode
      ** -------------------------------------------------------------------- */
      mm_handle_message( message );
      gmm_handle_message( message );
    }

    if(mm_sim_card_mode == MMGSDI_APP_USIM)
    {
      /* ----------------------------------------------------------
      ** Save the HFN values to the SIM on a RRC release indication
      ** ---------------------------------------------------------- */
      auth_handle_hfn_release_ind(
        message->cmd.rrc_abort_ind.hfn_cs, message->cmd.rrc_abort_ind.hfn_ps) ;
    }
    if(((mm_stop_mode_req_pended == TRUE) &&(mm_timer_status[ TIMER_MM_REL ] == TIMER_ACTIVE))
#ifdef FEATURE_DUAL_SIM
        || (mm_timer_status[TIMER_MM_DUAL_SWITCH] == TIMER_ACTIVE)
#endif

	)
    {
        mmcoord_process_mmr_stop_mode_req(&mmcoord_pending_reg_message);
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_abort_ind() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_PAGE_IND

DESCRIPTION
  Function to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_page_ind( mm_cmd_type *message )
{
#ifdef FEATURE_DUAL_SIM
  sys_modem_as_id_e_type local_sub_id = SYS_MODEM_AS_ID_1;
  boolean is_multisim_device = TRUE;
#endif
  if (message->cmd.rrc_page_ind.message_header.message_id == (int) RRC_PAGE_IND)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_PAGE_IND");

    if (message->cmd.rrc_page_ind.page_ue_id == RRC_PAGE_WITH_PTMSI)
    {
      event_report_payload(EVENT_NAS_MESSAGE_RECEIVED, 1, (void *)&message->cmd.rrc_page_ind.paging_cause);
    }
#ifdef FEATURE_DUAL_SIM
    if(mmode_nv_setting_for_device_mode != SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY && 
        mmode_nv_setting_for_device_mode != SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY)
    {
      is_multisim_device = FALSE;
    }
    for(local_sub_id = SYS_MODEM_AS_ID_1;local_sub_id < MAX_AS_IDS; local_sub_id++)
    {
      if (mm_sub_config_sim[local_sub_id] != SYS_OVERALL_FEATURE_MODE_MULTISIM)
      {
        is_multisim_device = FALSE;
      }
    }
    if((is_multisim_device ==  TRUE) && 
       ((cm_call_is_there_call_on_other_sub(
              (sys_modem_as_id_e_type)mm_as_id,
               CM_CALL_TYPE_VOICE_MASK | CM_CALL_TYPE_EMERG_MASK ) == TRUE) &&
          (mm_emm_wlan_call_in_progress( 
             emm_db_get_ctrl_data_per_subs(MM_OTHER_SUB(mm_as_id))) == FALSE)
         ))
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= ignoring page on this sub as CM is starting a voice call on other sub");
      return;
    }
#endif

    /* --------------------------------------------
    ** Route page according to CN domain identifier
    ** -------------------------------------------- */
    if (message->cmd.rrc_page_ind.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      if((cm_get_ui_ready_status() == FALSE) && 
	  	  ((message->cmd.rrc_page_ind.paging_cause == RRC_PAGE_CAUSE_TERM_CONV_CALL)||
	  	   (message->cmd.rrc_page_ind.paging_cause == RRC_PAGE_CAUSE_TERM_STREAM_CALL)||
	  	   (message->cmd.rrc_page_ind.paging_cause == RRC_PAGE_CAUSE_TERM_INTERACT_CALL)||
	  	   (message->cmd.rrc_page_ind.paging_cause == RRC_PAGE_CAUSE_TERM_BACKGND_CALL)
	  	  )
	  	 )
      {
        MSG_HIGH_DS_0(MM_SUB,"=MM= UI is not ready ignore MT CS CALL");
      }
      else
      {
        mm_handle_message( message );
      }
    }
    else /* cn_domain_id == RRC_PS_DOMAIN_CN_ID */
    { 
      if(cm_get_ui_ready_status() == TRUE)
      { 
        gmm_handle_message( message );
      }
      else
      {
        MSG_HIGH_DS_0(MM_SUB,"=MM= UI is not ready ignore PS page");
      }
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_page_ind() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_DATA_IND

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_data_ind( mm_cmd_type *message )
{
  byte *nas_msg_ptr;
  rrc_cn_domain_identity_e_type    cn_domain_identity;
  uint16 mm_max_octet_L3_msg;
  uint16 length,ota_length;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if (message->cmd.rrc_data_ind.message_header.message_id == (int) RRC_DATA_IND)
  {
    cn_domain_identity = message->cmd.rrc_data_ind.cn_domain_id;
    nas_msg_ptr = message->cmd.rrc_data_ind.nas_msg.value;
    if (message->cmd.rrc_data_ind.nas_msg.length >=2) 
    {
      MSG_HIGH_DS_2( MM_SUB, "=MM= MM recved RRC_DATA_IND %x-%x (pd/type)",
                        nas_msg_ptr[0], nas_msg_ptr[1]);
    }
    else
    {
      MSG_HIGH_DS_0( MM_SUB, "=MM= MM recved RRC_DATA_IND ");
    }
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_DATA)
    if((cn_domain_identity == RRC_PS_DOMAIN_CN_ID) &&
       (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY) &&
       (ps_session_status == MMRRC_RELEASE_IN_PROGRESS))
    {
      MSG_HIGH_DS_0( MM_SUB,
            "=MM= Ignore PS RRC_DATA_IND on CS only SUB - PS release in progress");
      return;
    }
#endif

    if ( cn_domain_identity == RRC_CS_DOMAIN_CN_ID )
    {
      mm_max_octet_L3_msg = gmm_max_L3_msg_length(MM_CS_DOMAIN);
    }
    else 
    {
      mm_max_octet_L3_msg = gmm_max_L3_msg_length(MM_PS_DOMAIN);
    }

    if (message->cmd.rrc_data_ind.nas_msg.length <= mm_max_octet_L3_msg)
    {
      if( cn_domain_identity == RRC_CS_DOMAIN_CN_ID)
      {
        /* ------------------------
        ** Translate message header
        ** ------------------------ */
        message->cmd.rr_data_ind.message_header.message_set =
          message->cmd.rrc_data_ind.message_header.message_set;
        message->cmd.rr_data_ind.message_header.message_id  = (int) MM_DATA_IND;
        message->cmd.rr_data_ind.message_header.message_len_lsb =
          (byte) message->cmd.rrc_data_ind.nas_msg.length;
        message->cmd.rr_data_ind.message_header.message_len_msb = 0;

        /* --------------------
        ** Copy the OTA message
        ** -------------------- */
        (void) memscpy( message->cmd.rr_data_ind.layer3_message, MAX_OCTETS_L3_MESSAGE,
                       message->cmd.rrc_data_ind.nas_msg.value,
                       message->cmd.rrc_data_ind.nas_msg.length );
 
         mm_send_ota_msg_log_packet( CN_TO_UE_DIRECTION,
                                    message->cmd.rr_data_ind.message_header.message_len_lsb,
                                    message->cmd.rr_data_ind.layer3_message);
         mm_handle_message( message );
      }
      else
      {
         /* ------------------------
         ** Translate message header
         ** ------------------------ */
         message->cmd.mm_data_ind.message_header.message_set =
         message->cmd.rrc_data_ind.message_header.message_set;
         message->cmd.mm_data_ind.message_header.message_id  = (int) MM_DATA_IND;

         PUT_IMH_LEN(message->cmd.rrc_data_ind.nas_msg.length,&message->cmd.mm_data_ind.message_header);

         length = (uint16) message->cmd.rrc_data_ind.nas_msg.length;

         ota_length = MIN(length,mm_max_octet_L3_msg);
         

         message->cmd.mm_data_ind.L3_data =  modem_mem_calloc(1, ota_length, MODEM_MEM_CLIENT_NAS);

         mm_check_for_null_ptr((void*)message->cmd.mm_data_ind.L3_data);

          /* --------------------
          ** Copy the OTA message
          ** -------------------- */
         (void) memscpy( message->cmd.mm_data_ind.L3_data, mm_max_octet_L3_msg,
                         message->cmd.rrc_data_ind.nas_msg.value,
                         message->cmd.rrc_data_ind.nas_msg.length );

          mm_send_ota_msg_log_packet( CN_TO_UE_DIRECTION,
                                      length,
                                      message->cmd.mm_data_ind.L3_data);
          gmm_handle_message( message );

          if ( message->cmd.mm_data_ind.L3_data != NULL)
          {

            modem_mem_free( message->cmd.mm_data_ind.L3_data , MODEM_MEM_CLIENT_NAS);

          }
        }     
    }
    else
    {
      MSG_ERROR_DS( MM_SUB, "=MM= NOT IMPLEMENTED", 0,0,0 );
    }

    /* --------------------
    ** Free the msg pointer
    ** -------------------- */

    modem_mem_free( nas_msg_ptr , MODEM_MEM_CLIENT_NAS);

  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_data_ind() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_OPEN_SESSION_CNF

DESCRIPTION
  Function to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_open_session_cnf( mm_cmd_type *message )
{
  if (message->cmd.rrc_open_session_cnf.message_header.message_id ==
        (int) RRC_OPEN_SESSION_CNF)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_OPEN_SESSION_CNF");

    /* ------------------------------------------------------------
    ** Route RRC_OPEN_SESSION_CNF according to CN domain identifier
    ** ------------------------------------------------------------ */
    if (message->cmd.rrc_open_session_cnf.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      mm_handle_message( message );
    }
    else /* cn_domain_id == RRC_PS_DOMAIN_CN_ID */
    {
      gmm_handle_message( message );
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_open_session_cnf() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_CLOSE_SESSION_IND

DESCRIPTION
  Function to

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_close_session_ind( mm_cmd_type *message )
{
  if (message->cmd.rrc_close_session_ind.message_header.message_id ==
        (int) RRC_CLOSE_SESSION_IND)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_CLOSE_SESSION_IND");

    /* -------------------------------------------------------------
    ** Route RRC_CLOSE_SESSION_IND according to CN domain identifier
    ** ------------------------------------------------------------- */
    if((mm_reg_waiting_for_reg_cnf) &&
       (
#ifdef FEATURE_WRLF_SYSTEM_SEL
       mm_wcdma_rlf_state == WCDMA_RLF_STARTED ||
#endif
       mm_waiting_for_service_cnf))
    {
      mm_reg_waiting_for_reg_cnf = FALSE;
      
      if (message->cmd.rrc_close_session_ind.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
      {
        mm_handle_message( message );
      }
      else /* cn_domain_id == RRC_PS_DOMAIN_CN_ID */
      {
        gmm_handle_message( message );
      }
      mm_reg_waiting_for_reg_cnf = TRUE;
     
    }
    else
    {
    if (message->cmd.rrc_close_session_ind.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      mm_handle_message( message );
    }
    else /* cn_domain_id == RRC_PS_DOMAIN_CN_ID */
    {
      gmm_handle_message( message );
    }
  }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_close_session_ind() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_CLOSE_SESSION_CNF

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_close_session_cnf( mm_cmd_type *message )
{
  if (message->cmd.rrc_close_session_cnf.message_header.message_id ==
        (int) RRC_CLOSE_SESSION_CNF)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_CLOSE_SESSION_CNF");

    /* -------------------------------------------------------------
    ** Route RRC_CLOSE_SESSION_CNF according to CN domain identifier
    ** ------------------------------------------------------------- */
    if (message->cmd.rrc_close_session_cnf.cn_domain_id == RRC_CS_DOMAIN_CN_ID)
    {
      mm_handle_message( message );
    }
    else /* cn_domain_id == RRC_PS_DOMAIN_CN_ID */
    {
      gmm_handle_message( message );
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_close_session_cnf() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_ABORT_CNF

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_abort_cnf( mm_cmd_type *message )
{
  if (message->cmd.rrc_abort_cnf.message_header.message_id == (int) RRC_ABORT_CNF)
  {

    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_ABORT_CNF");
    mm_waiting_for_abort_cnf = FALSE;
#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(mm_wcdma_rlf_state == WCDMA_RLF_SERVICE_CNF_PENDING)
    {
      mm_reg_waiting_for_reg_cnf = FALSE;
      mm_handle_message( message );
      gmm_handle_message( message );
      mm_reg_waiting_for_reg_cnf = TRUE;
      mm_waiting_for_service_cnf = TRUE;

      if(mm_wrlf_pending_rr_message_ptr != NULL)
      {
        mmcoord_process_rr_plmn_select_cnf(mm_wrlf_pending_rr_message_ptr);
        modem_mem_free(mm_wrlf_pending_rr_message_ptr,MODEM_MEM_CLIENT_NAS);
        mm_wrlf_pending_rr_message_ptr = NULL;
      }
      else if(mm_wrlf_pending_rrc_message_ptr != NULL)
      {
        mmcoord_process_rrc_service_cnf(mm_wrlf_pending_rrc_message_ptr);
        mmcoord_set_wrlf_state(WCDMA_RLF_IDLE);
        mm_received_suitable_search_end_ind = FALSE;
        modem_mem_free(mm_wrlf_pending_rrc_message_ptr,MODEM_MEM_CLIENT_NAS);
        mm_wrlf_pending_rrc_message_ptr = NULL;
      }
    }
    else
#endif
    {
          /* --------------------------------------------------------------------
      ** Pass RRC_ABORT_CNF to both MM & GMM independent of MS operation mode
      ** -------------------------------------------------------------------- */

       mm_handle_message( message );
       gmm_handle_message( message );
    }
    if(mm_sim_card_mode == MMGSDI_APP_USIM)
    {
      /* ----------------------------------------------------------
      ** Save the HFN values to the SIM on a RRC release indication
      ** ---------------------------------------------------------- */
      auth_handle_hfn_release_ind(
        message->cmd.rrc_abort_cnf.hfn_cs, message->cmd.rrc_abort_cnf.hfn_ps) ;
    }
    if(((mm_stop_mode_req_pended == TRUE) &&(mm_timer_status[ TIMER_MM_REL ] == TIMER_ACTIVE))
#ifdef FEATURE_DUAL_SIM
			   || (mm_timer_status[TIMER_MM_DUAL_SWITCH] == TIMER_ACTIVE)
#endif

	  )
    {
      mmcoord_process_mmr_stop_mode_req(&mmcoord_pending_reg_message);
 
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_abort_cnf() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_DEACT_CNF

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_deact_cnf( mm_cmd_type *message )
{
  if (message->cmd.hdr.message_id)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_DEACT_CNF");

    /* --------------------------------------------------------------------
    ** Pass RRC_DEACT_CNF to both MM & GMM independent of MS operation mode
    ** -------------------------------------------------------------------- */
    mm_handle_message( message );
    gmm_handle_message( message );

    if(mm_sim_card_mode == MMGSDI_APP_USIM)
    {
      /* ----------------------------------------------------------
      ** Save the HFN values to the SIM on a RRC release indication
      ** ---------------------------------------------------------- */
      auth_handle_hfn_release_ind(
        message->cmd.rrc_deact_cnf.hfn_cs, message->cmd.rrc_deact_cnf.hfn_ps) ;
    }
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_deact_cnf() */


/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_STOP_MODE_CNF

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_rrc_stop_mode_cnf( mm_cmd_type *message )
{
  if (message->cmd.hdr.message_id)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RRC_STOP_WCDMA_MODE_CNF");

    /* --------------------------------------------------------------------
    ** Pass RRC_STOP_WCDMA_MODE_CNF to both MM & GMM independent of MS operation mode
    ** -------------------------------------------------------------------- */
    mm_handle_message( message );
    gmm_handle_message( message );
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
} /* end mmcoord_process_rrc_deact_cnf() */


/*===========================================================================

FUNCTION MMCOORD_VALID_RRC_COMMAND

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean mmcoord_valid_rrc_command( mm_cmd_type *message )
{
  boolean valid_command;
  byte    i;
  mm_as_cell_access_status_T cs_cell_access, ps_cell_access;
  mm_as_page_rsp_e_type page_rsp_alowed;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  valid_command = TRUE;

  if (((message->cmd.hdr.message_set == MS_MM_RRC)
#ifdef FEATURE_SEGMENT_LOADING
&&(get_wcdma_interface())
#endif
  )
||
      ((message->cmd.hdr.message_set == MS_MM_TDSRRC)
#ifdef FEATURE_SEGMENT_LOADING
&&(get_tdscdma_interface())
#endif
))
  {
    switch (message->cmd.hdr.message_id)
    {
      case RRC_REL_IND:
        if ((message->cmd.rrc_rel_ind.rel_cause != RRC_REL_CAUSE_NORMAL) &&
            (message->cmd.rrc_rel_ind.rel_cause != RRC_REL_CAUSE_UNSPEC) &&
            (message->cmd.rrc_rel_ind.rel_cause != RRC_REL_CAUSE_PRE_EMPTIVE) &&
            (message->cmd.rrc_rel_ind.rel_cause != RRC_REL_CAUSE_CONGESTION) &&
            (message->cmd.rrc_rel_ind.rel_cause != RRC_REL_CAUSE_RE_ESTABLISH_REJECT) &&
            (message->cmd.rrc_rel_ind.rel_cause != RRC_REL_USER_INACTIVITY) &&
            (message->cmd.rrc_rel_ind.rel_cause != RRC_DIRECTED_SIGNALLING_REESTABLISHMENT) &&
            (message->cmd.rrc_rel_ind.rel_cause != RRC_REL_DEEP_FADE)
#ifdef FEATURE_CONNECTED_MODE_SERV_IND
            &&
            (message->cmd.rrc_rel_ind.rel_cause != RRC_REL_CAUSE_OOS)            
#endif            
            )
        {
          valid_command = FALSE;
        }
        break;

      case RRC_SYNC_IND:
        if ((message->cmd.rrc_sync_ind.cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
            (message->cmd.rrc_sync_ind.cn_domain_id != RRC_PS_DOMAIN_CN_ID))
        {
          valid_command = FALSE;
        }

        if (message->cmd.rrc_sync_ind.rab_id_present)
        {
          if ((message->cmd.rrc_sync_ind.rab_info.action != RAB_ESTABLISHED) &&
              (message->cmd.rrc_sync_ind.rab_info.action != RAB_RELEASED) &&
              (message->cmd.rrc_sync_ind.rab_info.action != RAB_RECONFIGURED))
          {
            valid_command = FALSE;
          }
        }

        if ((message->cmd.rrc_sync_ind.cause != RRC_RAB_ESTABLISHED) &&
            (message->cmd.rrc_sync_ind.cause != RRC_CIPHERING_ESTABLISHED) &&
            (message->cmd.rrc_sync_ind.cause != RRC_LOOPBACK_ESTABLISHED) &&
            (message->cmd.rrc_sync_ind.cause != RRC_INTEGRITY_CIPHERING_ESTABLISHED) &&
            (message->cmd.rrc_sync_ind.cause != RRC_INTEGRITY_ESTABLISHED) &&
            (message->cmd.rrc_sync_ind.cause != RRC_HANDOVER))
        {
          valid_command = FALSE;
        }
        break;

      case RRC_SERVICE_CNF:
        if ((message->cmd.rrc_service_cnf.service_status != MM_AS_NO_SERVICE) &&
            (message->cmd.rrc_service_cnf.service_status != MM_AS_SERVICE_AVAILABLE))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rrc_service_cnf.service_status == MM_AS_SERVICE_AVAILABLE) &&
            (message->cmd.rrc_service_cnf.service_info.gsm_map_common_info_incl))
        {
          /* -------------------
          ** Verify service info
          ** ------------------- */
          if ((message->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_type !=
                 RRC_GSM_MAP_PLMN_SEL) &&
              (message->cmd.rrc_service_cnf.service_info.selected_plmn.plmn_type !=
                 RRC_GSM_MAP_ANSI_41_PLMN_SEL))
          {
            valid_command = FALSE;
          }

          if (message->cmd.rrc_service_cnf.service_info.gsm_map_common_info.length < 2)
          {
            valid_command = FALSE;
          }

          if ((message->cmd.rrc_service_cnf.service_info.num_cn_domains == 0) ||
              (message->cmd.rrc_service_cnf.service_info.num_cn_domains >
                 RRC_MAX_CN_DOMAINS))
          {
            valid_command = FALSE;
          }
          else
          {
            for (i=0; i < message->cmd.rrc_service_cnf.service_info.num_cn_domains; i++)
            {
              if ((message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                     cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
                  (message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                     cn_domain_id != RRC_PS_DOMAIN_CN_ID))
              {
                valid_command = FALSE;
              }
              else if (message->cmd.rrc_service_cnf.service_info.cn_domain_info[i].
                         cn_domain_specific_info.gsm_map_info.length < 2)
              {
                valid_command = FALSE;
              }
            }
          }

          cs_cell_access = mm_get_cs_cell_access_from_rrc_service_info(&(message->cmd.rrc_service_cnf.service_info));
          ps_cell_access = mm_get_ps_cell_access_from_rrc_service_info(&(message->cmd.rrc_service_cnf.service_info));

          valid_command = mm_check_if_cell_access_valid(cs_cell_access);
          if (valid_command == TRUE)
          {
            valid_command = mm_check_if_cell_access_valid(ps_cell_access);
          }  
          page_rsp_alowed = mm_get_page_resp_info_from_rrc_service_info(&(message->cmd.rrc_service_cnf.service_info));
          valid_command = mm_check_if_page_rsp_allowed_valid(page_rsp_alowed);        
        }

        if (message->cmd.rrc_service_cnf.avail_plmn_list.num_plmns > RRC_MAX_PLMNS)
        {
          valid_command = FALSE;
        }
        break;

      case RRC_SERVICE_IND:
        if ((message->cmd.rrc_service_ind.service_status != MM_AS_SERVICE_AVAILABLE) &&
            (message->cmd.rrc_service_ind.service_status != MM_AS_NO_SERVICE))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rrc_service_ind.service_status == MM_AS_SERVICE_AVAILABLE) &&
            (message->cmd.rrc_service_ind.service_info.gsm_map_common_info_incl))
        {
          /* -------------------
          ** Verify service info
          ** ------------------- */
          if ((message->cmd.rrc_service_ind.service_info.selected_plmn.plmn_type !=
                 RRC_GSM_MAP_PLMN_SEL) &&
              (message->cmd.rrc_service_ind.service_info.selected_plmn.plmn_type !=
                 RRC_GSM_MAP_ANSI_41_PLMN_SEL))
          {
            valid_command = FALSE;
          }

          if (message->cmd.rrc_service_ind.service_info.gsm_map_common_info.length < 2)
          {
            valid_command = FALSE;
          }

          if ((message->cmd.rrc_service_ind.service_info.num_cn_domains == 0) ||
              (message->cmd.rrc_service_ind.service_info.num_cn_domains >
                 RRC_MAX_CN_DOMAINS))
          {
            valid_command = FALSE;
          }
          else
          {
            for (i=0; i < message->cmd.rrc_service_ind.service_info.num_cn_domains; i++)
            {
              if ((message->cmd.rrc_service_ind.service_info.cn_domain_info[i].
                     cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
                  (message->cmd.rrc_service_ind.service_info.cn_domain_info[i].
                     cn_domain_id != RRC_PS_DOMAIN_CN_ID))
              {
                valid_command = FALSE;
              }
              else if (message->cmd.rrc_service_ind.service_info.cn_domain_info[i].
                         cn_domain_specific_info.gsm_map_info.length < 2)
              {
                valid_command = FALSE;
              }
            }
          }


          cs_cell_access = mm_get_cs_cell_access_from_rrc_service_info(&(message->cmd.rrc_service_ind.service_info));
          ps_cell_access = mm_get_ps_cell_access_from_rrc_service_info(&(message->cmd.rrc_service_ind.service_info));

          valid_command = mm_check_if_cell_access_valid(cs_cell_access);
          if (valid_command == TRUE)
          {
            valid_command = mm_check_if_cell_access_valid(ps_cell_access);
          }
          page_rsp_alowed = mm_get_page_resp_info_from_rrc_service_info(&(message->cmd.rrc_service_ind.service_info));
          valid_command = mm_check_if_page_rsp_allowed_valid(page_rsp_alowed);         
        }
        break;

      case RRC_SERVICE_IND_FROM_OTA_MSG:
        if ((message->cmd.rrc_service_ind_from_ota_msg.service_status != MM_AS_SERVICE_AVAILABLE) &&
            (message->cmd.rrc_service_ind_from_ota_msg.service_status != MM_AS_NO_SERVICE))
        {
          valid_command = FALSE;
        }

        /* -------------------
        ** Verify service info
        ** ------------------- */
        if ((message->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_type !=
               RRC_GSM_MAP_PLMN_SEL) &&
            (message->cmd.rrc_service_ind_from_ota_msg.service_info.selected_plmn.plmn_type !=
               RRC_GSM_MAP_ANSI_41_PLMN_SEL))
        {
          valid_command = FALSE;
        }

        /* ---------------------------------------
        ** Number of CN domains can be '0'
        ** -------------------------------------*/
        if ( (message->cmd.rrc_service_ind_from_ota_msg.service_info.num_cn_domains) >
               RRC_MAX_CN_DOMAINS)
        {
          valid_command = FALSE;
        }
        else
        {
          for (i=0; i < message->cmd.rrc_service_ind_from_ota_msg.service_info.num_cn_domains; i++)
          {
            if ((message->cmd.rrc_service_ind_from_ota_msg.service_info.cn_domain_info[i].
                   cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
                (message->cmd.rrc_service_ind_from_ota_msg.service_info.cn_domain_info[i].
                   cn_domain_id != RRC_PS_DOMAIN_CN_ID))
            {
              valid_command = FALSE;
            }
            else if (message->cmd.rrc_service_ind_from_ota_msg.service_info.cn_domain_info[i].
                       cn_domain_specific_info.gsm_map_info.length < 1)
            {
              valid_command = FALSE;
            }
          }
        }

        cs_cell_access = mm_get_cs_cell_access_from_rrc_service_info(&(message->cmd.rrc_service_ind_from_ota_msg.service_info));
        ps_cell_access = mm_get_ps_cell_access_from_rrc_service_info(&(message->cmd.rrc_service_ind_from_ota_msg.service_info));

        valid_command = mm_check_if_cell_access_valid(cs_cell_access);
        if (valid_command == TRUE)
        {
          valid_command = mm_check_if_cell_access_valid(ps_cell_access);
        }
        page_rsp_alowed = mm_get_page_resp_info_from_rrc_service_info(&(message->cmd.rrc_service_ind_from_ota_msg.service_info));
        valid_command = mm_check_if_page_rsp_allowed_valid(page_rsp_alowed);       
        break;

      case RRC_PLMN_LIST_CNF:
        if (message->cmd.rrc_plmn_list_cnf.avail_plmn_list.num_plmns > RRC_MAX_PLMNS)
        {
          valid_command = FALSE;
        }
        break;

      case RRC_ABORT_IND:
        if ((message->cmd.rrc_abort_ind.abort_cause != RRC_FAIL_TO_CAMP_ON) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_INTERNAL_ERROR) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_UNSUPPORTED_REDIRECTION) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_MAX_ACCESS_FAILURE) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_FAILURE_DUE_TO_CONGESTION) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_FAILURE_UNSPECIFIED) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_UNSUPPORTED_INITIAL_UE_ID) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_CONNECTION_NOT_AVAILABLE) &&
#ifdef FEATURE_DUAL_SIM
            (message->cmd.rrc_abort_ind.abort_cause != RRC_NO_RESOURCE_AVAILABLE) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_ABORT_CONN_REJECT_WAIT_TMR) &&
#endif
#ifdef FEATURE_SILENT_REDIAL_AFTER_RA_LA_MISMATCH
            (message->cmd.rrc_abort_ind.abort_cause != RRC_ABORT_LAI_RAI_CHANGED) &&
#endif
            (message->cmd.rrc_abort_ind.abort_cause != RRC_AC_NOT_ALLOWED_CS_DOMAIN) &&
            (message->cmd.rrc_abort_ind.abort_cause != RRC_AC_NOT_ALLOWED_PS_DOMAIN))
        {
          valid_command = FALSE;
        }
        break;

      case RRC_PAGE_IND:
        if ((message->cmd.rrc_page_ind.cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
            (message->cmd.rrc_page_ind.cn_domain_id != RRC_PS_DOMAIN_CN_ID))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rrc_page_ind.paging_cause != RRC_PAGE_CAUSE_TERM_CONV_CALL) &&
            (message->cmd.rrc_page_ind.paging_cause !=
               RRC_PAGE_CAUSE_TERM_STREAM_CALL) &&
            (message->cmd.rrc_page_ind.paging_cause !=
               RRC_PAGE_CAUSE_TERM_INTERACT_CALL) &&
            (message->cmd.rrc_page_ind.paging_cause !=
               RRC_PAGE_CAUSE_TERM_BACKGND_CALL) &&
            (message->cmd.rrc_page_ind.paging_cause != RRC_PAGE_CAUSE_SMS) &&
            (message->cmd.rrc_page_ind.paging_cause !=
               RRC_PAGE_CAUSE_HIGH_PRI_SIGNALLING) &&
            (message->cmd.rrc_page_ind.paging_cause !=
               RRC_PAGE_CAUSE_LOW_PRI_SIGNALLING) &&
            (message->cmd.rrc_page_ind.paging_cause !=
               RRC_PAGE_CAUSE_UNKNOWN))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rrc_page_ind.page_ue_id != RRC_PAGE_WITH_IMSI) &&
            (message->cmd.rrc_page_ind.page_ue_id != RRC_PAGE_WITH_TMSI) &&
            (message->cmd.rrc_page_ind.page_ue_id != RRC_PAGE_WITH_PTMSI))
        {
          valid_command = FALSE;
        }
        break;

      case RRC_DATA_IND:
        if ((message->cmd.rrc_data_ind.cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
            (message->cmd.rrc_data_ind.cn_domain_id != RRC_PS_DOMAIN_CN_ID))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rrc_data_ind.nas_msg.length == 0) ||
            (message->cmd.rrc_data_ind.nas_msg.value == NULL))
        {
          valid_command = FALSE;
        }
        break;

      case RRC_OPEN_SESSION_CNF:
        if ((message->cmd.rrc_open_session_cnf.status != RRC_OPEN_SESS_SUCCESS) &&
            (message->cmd.rrc_open_session_cnf.status != RRC_OPEN_SESS_FAILURE) &&
            (message->cmd.rrc_open_session_cnf.status != RRC_OPEN_SESS_FAILURE_AC_BARD) &&
            (message->cmd.rrc_open_session_cnf.status != RRC_OPEN_SESS_FAILURE_CONN_REL_IN_PROG))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rrc_open_session_cnf.cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
            (message->cmd.rrc_open_session_cnf.cn_domain_id != RRC_PS_DOMAIN_CN_ID))
        {
          valid_command = FALSE;
        }
        break;

      case RRC_CLOSE_SESSION_IND:
        if ((message->cmd.rrc_close_session_ind.cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
            (message->cmd.rrc_close_session_ind.cn_domain_id != RRC_PS_DOMAIN_CN_ID))
        {
          valid_command = FALSE;
        }
        break;

      case RRC_CLOSE_SESSION_CNF:
        if ((message->cmd.rrc_close_session_cnf.cn_domain_id != RRC_CS_DOMAIN_CN_ID)  &&
            (message->cmd.rrc_close_session_cnf.cn_domain_id != RRC_PS_DOMAIN_CN_ID))
        {
          valid_command = FALSE;
        }
        break;

      case RRC_FAST_DORMANCY_CNF:
        if ((message->cmd.rrc_fast_dormancy_cnf.status != RRC_FAST_DORMANCY_SUCCESS_APP_NO_RETRY)  &&
            (message->cmd.rrc_fast_dormancy_cnf.status != RRC_FAST_DORMANCY_SUCCESS_APP_RETRY) &&
            (message->cmd.rrc_fast_dormancy_cnf.status != RRC_FAST_DORMANCY_FAILURE_APP_RETRY) )
        {
          valid_command = FALSE;
        }
        break;

      case RRC_ABORT_CNF:
      case RRC_DEACT_CNF:
      case RRC_EST_CNF:
      case RRC_ACTIVATION_IND:
      case RRC_STOP_WCDMA_MODE_CNF:
      case RRC_MODE_CHANGE_CNF:
#ifdef FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER
      case RRC_WTOG_CCO_COMPLETE_IND:
#endif /* FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER */
#ifdef FEATURE_HSDPA
      case RRC_HSDPA_HSUPA_CELL_IND:
#ifdef FEATURE_HSPA_CALL_STATUS_IND
      case RRC_HSPA_CALL_STATUS_IND:
#endif
#endif
#ifdef FEATURE_OOSC_SERVICE_STATUS_UI_UPDATE
      case RRC_CONNECTED_MODE_SERVICE_STATUS_IND:
#endif
#ifdef FEATURE_WRLF_SYSTEM_SEL
      case RRC_DISABLE_BPLMN_IND:
      case RRC_SUITABLE_SEARCH_END_IND:
#endif
#ifdef FEATURE_LTE
      case RRC_SRVCC_HANDOVER_FAILURE_IND:
#endif
      case RRC_PCH_STATE_IND:      
        /* -----------------------------------------------------
        ** These primitives contain no parameters to error check
        ** ----------------------------------------------------- */
        break;
#ifdef FEATURE_MBMS
      #error code not present
#endif

      default:
        valid_command = FALSE;
    } /* emd switch (message->cmd.hdr.message_id) */
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message set %d", message->cmd.hdr.message_set, 0,0 );
  }

  if (!valid_command)
  {
    MSG_ERROR_DS( MM_SUB, "=MM= Invalid RRC cmd %d", message->cmd.hdr.message_id, 0,0 );
  }

  return (valid_command);

} /* end mmcoord_valid_rrc_command() */


/*===========================================================================

FUNCTION MMCOORD_VALID_RR_COMMAND

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean mmcoord_valid_rr_command( mm_cmd_type *message )
{
  boolean valid_command;

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  valid_command = TRUE;

  if (message->cmd.hdr.message_set == MS_MM_RR)
  {
    switch (message->cmd.hdr.message_id)
    {
      case RR_ABORT_IND:
        if ((message->cmd.rr_abort_ind.RR_abort_cause != REASON_NOT_AVAILABLE) &&
            (message->cmd.rr_abort_ind.RR_abort_cause != ACCESS_CLASS_NOT_ALLOWED) &&
            (message->cmd.rr_abort_ind.RR_abort_cause != RANDOM_ACCESS_FAILURE) &&
            (message->cmd.rr_abort_ind.RR_abort_cause != OTHER_REASON) &&
            (message->cmd.rr_abort_ind.RR_abort_cause != RR_CONNECTION_FAILURE) &&
            (message->cmd.rr_abort_ind.RR_abort_cause != RR_ABORT_REDIAL_NOT_ALLOWED) &&
            (message->cmd.rr_abort_ind.RR_abort_cause != REASON_INTERRAT_RESELECTION) &&
            (message->cmd.rr_abort_ind.RR_abort_cause != RR_ABORT_LAI_CHANGED)&&
            (message->cmd.rr_abort_ind.RR_abort_cause != RR_ABORT_CON_EST_NOT_ATTEMPTED) 
#ifdef FEATURE_GTOW_REDIRECTION_AFTER_CONN_RELEASE
            && (message->cmd.rr_abort_ind.RR_abort_cause != RR_ABORT_GTOW_REDIRECTION_IN_PROGRESS)
#endif
            &&(message->cmd.rr_abort_ind.RR_abort_cause != RR_ABORT_IA_REJECT_RECEIVED)

            )
         {
          valid_command = FALSE;
        }
        break;

      case RR_PLMN_LIST_CNF:
        if (message->cmd.rr_plmn_list_cnf.available_plmn_list.length >
             SYS_PLMN_LIST_MAX_LENGTH)
        {
          valid_command = FALSE;
        }
        break;


      case RR_PLMN_SELECT_CNF:
        if ((message->cmd.rr_plmn_select_cnf.error_code != MM_AS_SUCCESS) &&
            (message->cmd.rr_plmn_select_cnf.error_code != MM_AS_CONNECTED) &&
            (message->cmd.rr_plmn_select_cnf.error_code != MM_AS_INTERRAT_RESELECTION) &&
            (message->cmd.rr_plmn_select_cnf.error_code != MM_AS_INTERRAT_HANDOVER))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rr_plmn_select_cnf.service_state != MM_AS_NO_SERVICE) &&
            (message->cmd.rr_plmn_select_cnf.service_state != MM_AS_SERVICE_AVAILABLE))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rr_plmn_select_cnf.cell_access != MM_AS_ACCESS_NORMAL_ONLY) &&
            (message->cmd.rr_plmn_select_cnf.cell_access != MM_AS_ACCESS_EMERGENCY_ONLY) &&
            (message->cmd.rr_plmn_select_cnf.cell_access != MM_AS_ACCESS_NO_CALLS) &&
            (message->cmd.rr_plmn_select_cnf.cell_access != MM_AS_ACCESS_ALL_CALLS))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rr_plmn_select_cnf.service_capability !=
               SYS_SRV_CAPABILITY_NOT_AVAILABLE) &&
            (message->cmd.rr_plmn_select_cnf.service_capability !=
               SYS_SRV_CAPABILITY_CS_SERVICE_ONLY) &&
            (message->cmd.rr_plmn_select_cnf.service_capability !=
               SYS_SRV_CAPABILITY_PS_SERVICE_ONLY) &&
            (message->cmd.rr_plmn_select_cnf.service_capability !=
               SYS_SRV_CAPABILITY_CS_PS_SERVICE))
        {
          valid_command = FALSE;
        }

        if (message->cmd.rr_plmn_select_cnf.available_plmn_list.length >
              SYS_PLMN_LIST_MAX_LENGTH)
        {
          valid_command = FALSE;
        }
        break;

      case RR_REL_IND:
        /* ------------------------------------------------------
        ** We could check RR_cause field but MM never looks at it
        ** ------------------------------------------------------ */
        break;

      case RR_SERVICE_IND:
        if ((message->cmd.rr_service_ind.service_state != MM_AS_NO_SERVICE) &&
            (message->cmd.rr_service_ind.service_state != MM_AS_SERVICE_AVAILABLE))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rr_service_ind.cell_access != MM_AS_ACCESS_NORMAL_ONLY) &&
            (message->cmd.rr_service_ind.cell_access != MM_AS_ACCESS_EMERGENCY_ONLY) &&
            (message->cmd.rr_service_ind.cell_access != MM_AS_ACCESS_NO_CALLS) &&
            (message->cmd.rr_service_ind.cell_access != MM_AS_ACCESS_ALL_CALLS))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rr_service_ind.service_capability !=
               SYS_SRV_CAPABILITY_NOT_AVAILABLE) &&
            (message->cmd.rr_service_ind.service_capability !=
               SYS_SRV_CAPABILITY_CS_SERVICE_ONLY) &&
            (message->cmd.rr_service_ind.service_capability !=
               SYS_SRV_CAPABILITY_PS_SERVICE_ONLY) &&
            (message->cmd.rr_service_ind.service_capability !=
               SYS_SRV_CAPABILITY_CS_PS_SERVICE))
        {
          valid_command = FALSE;
        }
        break;

#ifdef FEATURE_GSM_DTM
      case RR_SERVICE_IND_FROM_OTA_MSG:
        if ((message->cmd.rr_service_ind_from_ota_msg.service_state != MM_AS_NO_SERVICE) &&
            (message->cmd.rr_service_ind_from_ota_msg.service_state != MM_AS_SERVICE_AVAILABLE))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rr_service_ind_from_ota_msg.cell_access != MM_AS_ACCESS_NORMAL_ONLY) &&
            (message->cmd.rr_service_ind_from_ota_msg.cell_access != MM_AS_ACCESS_EMERGENCY_ONLY) &&
            (message->cmd.rr_service_ind_from_ota_msg.cell_access != MM_AS_ACCESS_NO_CALLS) &&
            (message->cmd.rr_service_ind_from_ota_msg.cell_access != MM_AS_ACCESS_ALL_CALLS))
        {
          valid_command = FALSE;
        }

        if ((message->cmd.rr_service_ind_from_ota_msg.service_capability !=
               SYS_SRV_CAPABILITY_NOT_AVAILABLE) &&
            (message->cmd.rr_service_ind_from_ota_msg.service_capability !=
               SYS_SRV_CAPABILITY_CS_SERVICE_ONLY) &&
            (message->cmd.rr_service_ind_from_ota_msg.service_capability !=
               SYS_SRV_CAPABILITY_PS_SERVICE_ONLY) &&
            (message->cmd.rr_service_ind_from_ota_msg.service_capability !=
               SYS_SRV_CAPABILITY_CS_PS_SERVICE))
        {
          valid_command = FALSE;
        }
        break;
#endif /* #ifdef FEATURE_GSM_DTM */


      case RR_SYNC_IND:
        if ((message->cmd.rr_sync_ind.sync_reason != MM_RR_CIPHERING) &&
            (message->cmd.rr_sync_ind.sync_reason != MM_RR_RES_ASS) &&
            (message->cmd.rr_sync_ind.sync_reason != MM_RR_CHANNEL_MODE_MODIFIED) &&
            (message->cmd.rr_sync_ind.sync_reason != MM_RR_HANDOVER ) &&
            (message->cmd.rr_sync_ind.sync_reason != MM_RR_WCDMA_GSM_HO))
        {
          valid_command = FALSE;
        }

        /* ----------------------------------------------------------
        ** We could check channel_mode field but MM never looks at it
        ** ---------------------------------------------------------- */

        if ((message->cmd.rr_sync_ind.channel_type != FULL_RATE_TRAFFIC) &&
            (message->cmd.rr_sync_ind.channel_type != HALF_RATE_TRAFFIC) &&
            (message->cmd.rr_sync_ind.channel_type != SDCCH_4) &&
            (message->cmd.rr_sync_ind.channel_type != SDCCH_8))
        {
          valid_command = FALSE;
        }
        break;

      case RR_STOP_GSM_MODE_CNF:
        if ((message->cmd.rr_stop_gsm_mode_cnf.status != MM_AS_SUCCESS) &&
            (message->cmd.rr_stop_gsm_mode_cnf.status != MM_AS_CONNECTED) &&
            (message->cmd.rr_stop_gsm_mode_cnf.status != MM_AS_INTERRAT_RESELECTION) &&
            (message->cmd.rr_stop_gsm_mode_cnf.status != MM_AS_INTERRAT_HANDOVER))
         {
          valid_command = FALSE;
        }
        break;

      case RR_ABORT_CNF:
#ifdef FEATURE_DUAL_SIM
      case RR_DS_DATA_IND:
      case RR_PS_ABORT_CNF:
#else
      case RR_DATA_IND:
#endif
      case RR_DEACT_CNF:
      case RR_EST_CNF:
      case RR_PAGING_IND:
      case RR_ACTIVATION_IND:
#ifdef FEATURE_GSM_GPRS
      case RR_GMM_GPRS_SUSPENSION:
      case RR_GMM_GPRS_RESUMPTION_IND:
      case RR_RESELECTION_IND:
      case RR_GPRS_PCCCH_CAMP_CNF:
#ifdef FEATURE_NAS_FREE_FLOAT
      case RR_GMM_GPRS_SERVICE_CNF :
#endif
      case RR_GMM_GPRS_MULTISLOT_CLASS_IND:      
#endif
#ifdef FEATURE_INTERRAT_PCCO_GTOW
      case RR_GTOW_CCO_COMPLETE_IND:
#endif /* FEATURE_INTERRAT_PCCO_GTOW */
#if defined (FEATURE_GSM_EDTM) || defined (FEATURE_GAN)
      case RR_EST_IND:
#endif
#ifdef FEATURE_GPRS_PS_HANDOVER
      case RR_PSHO_IND:
#endif
      case RR_GMM_GPRS_MS_RA_CAP_REQ:
#ifdef FEATURE_LTE
      case RR_SRVCC_HANDOVER_FAILURE_IND:
#endif
        /* -------------------------------------------
        ** No error checking on this set of primitives
        ** ------------------------------------------- */
        break;
#ifdef FEATURE_DUAL_SIM
      case RR_MULTIMODE_SUBS_CHGD_CNF:
      case RR_PREEMPTION_IND:
        break;
#endif
      case RR_PS_STATUS_CHANGE_IND:
      case RR_GMM_RETX_PDU_TUNEAWAY_FAIL_IND:
      case RR_PLMN_RESEL_FAIL_IND:
        break;


      default:
        valid_command = FALSE;
    } /* emd switch (message->cmd.hdr.message_id) */
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message set %d", message->cmd.hdr.message_set, 0,0 );
  }

  if (!valid_command)
  {
    MSG_ERROR_DS( MM_SUB, "=MM= Invalid RR cmd %d", message->cmd.hdr.message_id, 0,0 );
  }

  return (valid_command);

} /* end mmcoord_valid_rr_command() */


/*===========================================================================

FUNCTION MMCOORD_VALID_REG_COMMAND

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean mmcoord_valid_reg_command( mm_cmd_type *message )
{
  boolean valid_command;
#ifdef FEATURE_LTE
  log_lte_nas_emm_incoming_event_type event;
#endif
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  valid_command = TRUE;

  if (message->cmd.hdr.message_set == MS_MM_REG)
  {
    switch (message->cmd.hdr.message_id)
    {
      case MMR_REG_REQ:
        if (!mmr_reg_req_is_valid (&message->cmd.mmr_reg_req))
        {
          valid_command = FALSE;
        }
        break;

      case MMR_GSTK_SERVICE_IND:
         break;
      case MMR_EQ_PLMN_CHANGE_IND:
        if (!mmr_eq_plmn_change_ind_is_valid(&message->cmd.mmr_eq_plmn_change_ind))
        {
          valid_command = FALSE;
        }
        break;

      case MMR_STOP_MODE_REQ:
        if (!mmr_stop_mode_req_is_valid (&message->cmd.mmr_stop_mode_req))
        {
          valid_command = FALSE;
        }
        break;

      case MMR_PLMN_SEARCH_REQ:
        if (!mmr_plmn_search_req_is_valid (&message->cmd.mmr_plmn_search_req))
        {
          valid_command = FALSE;
        }
        break;

      case MMR_SIM_AVAILABLE_REQ:
        if (!mmr_sim_available_req_is_valid (&message->cmd.mmr_sim_available_req))
        {
          valid_command = FALSE;
        }
        break;

      case MMR_SIM_NOT_AVAILABLE_REQ:
        if (!mmr_sim_not_available_req_is_valid (&message->cmd.mmr_sim_not_available_req))
        {
          valid_command = FALSE;
        }
        break;

      case MMR_PLMN_SEARCH_ABORT_REQ:
        /* No parameter is there. No error checking is needed. Always message is valid. */
        break;

      case MMR_PH_STATUS_CHANGE_REQ:
        #ifdef FEATURE_LTE
        event.emm_in_msg = (byte)MMR_PH_STATUS_CHANGE_REQ;
        event_report_payload(EVENT_LTE_REG_INCOMING_MSG,sizeof(event), (void*)&event);  
        #endif
        if (!mmr_ph_status_change_req_is_valid(&message->cmd.mmr_ph_status_change_req))
        {
          valid_command = FALSE;
        }
        break;
        
#ifdef FEATURE_OOSC_USER_ACTION
      case MMR_EXIT_FROM_PWR_SAVE:
          /* No parameter is there. No error checking is needed. Always message is valid. */
        break;
#endif


#ifdef FEATURE_DUAL_SIM
      case MMR_DS_STAT_CHGD_REQ:
        break;

      case MMR_DS_TUNEAWAY_STATUS_CHANGE_IND:
        break;

      case MMR_MULTIMODE_SUBS_CHGD_REQ:
        break;

      case MMR_DDS_SWITCH_IND:
        break;
#endif
#ifdef FEATURE_LTE
      case MMR_GW_PLMN_SEL_END_IND:
        break;
#endif

#ifdef FEATURE_NAS_ECALL
      case MMR_ECALL_MODE_IND:
        if ((message->cmd.mmr_ecall_mode_ind.mode_type != MMR_ECALL_MODE_NORMAL) &&
            (message->cmd.mmr_ecall_mode_ind.mode_type != MMR_ECALL_MODE_ECALL_ONLY))
        {
          valid_command = FALSE;
          MSG_HIGH_DS_1(MM_SUB ,"=MM= ECALL mode type (%d) not supported",
                     message->cmd.mmr_ecall_mode_ind.mode_type);
        }
        break;

      case MMR_PROFILE_TYPE_IND:
        break;
#endif

      case MMR_REFRESH_STAGE_IND:
        if ( message->cmd.mmr_refresh_stage_ind.refresh_stage >= MMGSDI_REFRESH_STAGE_ENUM_MAX )
        {
          valid_command = FALSE;
          MSG_HIGH_DS_1(MM_SUB ,"=MM= Refresh stage (%d) not supported",
                     message->cmd.mmr_refresh_stage_ind.refresh_stage);
        }          
        break; 

      case MMR_PS_DETACH_REQ:
        if (message->cmd.mmr_ps_detach_req.detach_type <= SYS_PS_DETACH_TYPE_NONE &&
             message->cmd.mmr_ps_detach_req.detach_type >= SYS_PS_DETACH_TYPE_MAX)
        {
          valid_command = FALSE;
          MSG_HIGH_DS_1(MM_SUB ,"=MM= MMR_PS_DETACH_REQ  type (%d) not supported",
                     message->cmd.mmr_ps_detach_req.detach_type);
        }
        break;

#ifdef FEATURE_LTE
      case MMR_SET_DRX_REQ:
        /* No error checking needed */
        break;

      case MMR_GET_DRX_REQ:
        /* No error checking needed */
        break;      

      case MMR_FILE_REFRESH_IND:
       /*No error checking needed */
       break;
#endif /*FEATURE_LTE*/
      case MMR_NET_SEL_MODE_CHANGE_IND:
          /*No error checking needed */
        break;      
      case MMR_SIM_STATE_UPDATE_IND:
        /* No error checking needed */
        break;
      case MMR_HIGH_PRIORITY_ACTIVITY_IND:
        /* No error checking needed */
        break;
#ifdef FEATURE_DUAL_SIM
      case MMR_T3245_EXPIRY_IND:
        /* No error checking needed */
        break;
#endif
      case MMR_CLEAR_LAI_REJECT_LIST_REQ:
        if (!mmr_clear_lai_reject_list_req_is_valid(&message->cmd.mmr_clear_lai_reject_list_req))
        {
          valid_command = FALSE;
        }
        break;
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE || defined FEATURE_SVLTE_DUAL_SIM
      case MMR_UE_MODE_CHANGE_IND :
        if (!mmr_ue_mode_change_ind_is_valid(&(message->cmd.mm_ue_mode_ind)))
        {
          valid_command = FALSE;
        }
        break;
#endif
#ifdef FEATURE_DUAL_SIM
      case MMR_DATA_PRIORITY_IND :
        if (!mmr_data_priority_ind_is_valid(&(message->cmd.mmr_data_prio_ind)))
        {
          valid_command = FALSE;
        }
        break;
#endif
#ifdef FEATURE_CMCC_SECURE_CALL
      case MMR_SECURE_CALL_IND:
        if(!mmr_secure_call_ind_is_valid(&(message->cmd.mmr_secure_call_ind)))
        { 
          valid_command = FALSE;
        }
        break;
#endif

        case MMR_PSEUDO_LIST_UPDATE_IND :
        if (!mmr_pseudo_list_update_is_valid(&(message->cmd.mmr_pseudo_list_update_ind)))
        {
           valid_command = FALSE;
        }
        break;

#ifdef MMOC_API_PH_STAT_CNF_WITH_STATUS
     case MMR_SIM_BUSY_IND:
     break;
#endif

#ifdef FEATURE_FEMTO_CSG
      case MMR_CSG_SELECT_CONFIG_UPDATE_IND:
        if (!mmr_csg_sel_cfg_update_ind_is_valid (&message->cmd.mmr_csg_sel_cfg_update_ind))
        {
          valid_command = FALSE;
        }
        break;
#endif
      default:
        valid_command = FALSE;
    } /* emd switch (message->cmd.hdr.message_id) */
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message set %d", message->cmd.hdr.message_set, 0,0 );
  }

  if (!valid_command)
  {
    MSG_ERROR_DS( MM_SUB, "=MM= Invalid REG cmd %d", message->cmd.hdr.message_id, 0,0 );
  }

  return (valid_command);

} /* end mmcoord_valid_reg_command() */

/*===========================================================================

FUNCTION mmcoord_process_mmr_ph_status_change_req

DESCRIPTION
  This function processes MMR_PH_STATUS_CHANGE_REQ from REG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_ph_status_change_req( mm_cmd_type *message )
{

  if (message->cmd.hdr.message_id == (int) MMR_PH_STATUS_CHANGE_REQ)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received MMR_PH_STATUS_CHANGE_REQ");

	mm_emm_clear_dos_counters();
	mm_is_sim_to_be_cs_invalid_after_power_off = FALSE; 
	mm_is_sim_to_be_ps_invalid_after_power_off = FALSE;
	MSG_HIGH_DS_0(MM_SUB,"=MM= resetting mm_is_sim_to_be_cs/ps_invalid during lpm"); 

	if(!reg_nv_is_lpm_power_off_enabled() && message->cmd.mmr_ph_status_change_req.stop_mode_reason == SYS_STOP_MODE_REASON_LPM_POWER_OFF)
	{

	  mm_send_mmr_ph_status_change_cnf();
	  return;
	}

    /* --------------------------------------------------------------
    ** Stop Mode request overrides any pending RAT/Service change procedures
    ** -------------------------------------------------------------- */
    mm_rat_change_pending = FALSE;

    /* MMR_STOP_MODE_REQUEST must stop any pended MMR_REG_REQ processing */
    mmcoord_reg_req_pending = FALSE ;

    mm_deep_sleep_pending = FALSE;

#ifdef FEATURE_SGLTE    
    mm_cs_stack_id = mm_ps_stack_id;
#endif
    /* --------------------------------------------------------------------
    ** Pass MMR_PH_STATUS_CHANGE_REQ to MM to deactivate stack.
    ** -------------------------------------------------------------------- */
#ifdef FEATURE_NAS_REL10

     /* At poweroff write the T3346 /T3246 remaing value to the EFS*/
     mm_write_backoff_remaining_info_to_efs();

#endif
    mm_handle_message( message );
  }
  else
  {
    MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
}
   
#ifdef FEATURE_NAS_ECALL
/*===========================================================================

FUNCTION mmcoord_process_mmr_ecall_mode_ind

DESCRIPTION
  This function processes MMR_ECALL_MODE_IND from REG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_mmr_ecall_mode_ind(mm_cmd_type *message)
{
  mmr_service_ind_s_type mmr_service_ind;
  inter_task_lai_T       new_lai;
  boolean lu_required = FALSE,attach_required = FALSE;
  inter_task_lai_T   mm_serving_lai_local;
  boolean routine_man_selection_failed = FALSE;
#ifdef FEATURE_FEMTO_CSG
   boolean   is_manual_csg_selection_success = FALSE;
#endif 


  memscpy((void *)&mm_serving_lai_local, sizeof(inter_task_lai_T),(void *)&mm_serving_lai[0],LAI_SIZE) ;

  if ((mm_ecall_mode_type == MMR_ECALL_MODE_NORMAL) && 
      (message->cmd.mmr_ecall_mode_ind.mode_type == MMR_ECALL_MODE_ECALL_ONLY))
  {
    mm_ecall_mode_type = MMR_ECALL_MODE_ECALL_ONLY;
   
    if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
    {
      gmm_handle_message( message );
    }
    else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)
    {
      mm_handle_message( message );
    }
    else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
    {
      if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
      {
        gmm_handle_message( message );
      }
      else /* NETWORK_OP_MODE_2 or PS_DOMAIN_SYS_INFO_UNAVAILABLE */
      {
        mm_handle_message( message );
        gmm_handle_message( message );
      }
    }
  }
  else if ((mm_ecall_mode_type == MMR_ECALL_MODE_ECALL_ONLY) && 
          (message->cmd.mmr_ecall_mode_ind.mode_type == MMR_ECALL_MODE_NORMAL))
  {
    mm_ecall_mode_type = MMR_ECALL_MODE_NORMAL;
    mm_stop_timer(TIMER_T3242);
    mm_stop_timer(TIMER_T3243);
   

    lu_required = mm_check_if_cs_cell_access_allowed(mm_get_cs_cell_access(PROCEDURE_CS_REGISTRATION));
    attach_required = gmm_check_if_ps_cell_access_allowed(gmm_get_ps_cell_access(PROCEDURE_PS_REGISTRATION)); 

    if (!((PLMN_MATCH( mm_serving_lai,
          mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn.identity ) ||
          (sys_eplmn_list_equivalent_plmn(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) && 
           sys_eplmn_list_equivalent_plmn(mm_serving_plmn.info.plmn)))
#ifdef FEATURE_FEMTO_CSG
          && (mm_is_csg_in_white_list(mm_serving_plmn.info.plmn, &mm_serving_plmn.info.csg_info))
#endif 
      ) &&
        (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION)) 
    {
       routine_man_selection_failed = TRUE;
       MSG_HIGH_DS_0(MM_SUB,"=MM= Different PLMN found during MAN_LIMITED_ACQ");
    }


    if (mm_waiting_for_stop_mode_cnf)
    {
      if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
      {
        gmm_set_substate(GMM_ATTACH_NEEDED);
      }
      else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)
      {
        mm_substate_control(MM_STD_UPDATE_PENDING);
      }
      else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
      {
        if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
        {
          gmm_set_substate(GMM_ATTACH_NEEDED);
        }
        else
        {
          mm_substate_control(MM_STD_UPDATE_PENDING);
          gmm_set_substate(GMM_ATTACH_NEEDED);
        }
      }
    }
    else if (mm_waiting_for_service_cnf)
    {
      MSG_HIGH_DS_0(MM_SUB,"=MM= Do regitration after receiving service cnf");
    }
    else if ((routine_man_selection_failed) ||
              (((((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)||
                   (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                     SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION))&&
                    (!mm_manual_mode_user_selection_procedure ||
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE             
                         mm_per_subs_check_lai_in_reject_list(mm_serving_lai_local, (sys_modem_as_id_e_type) mm_as_id)
#else
                         mm_check_lai_in_reject_list(mm_serving_lai_local)
#endif          
                         )) ||
                            ((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC)||
                          (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode == 
                           SYS_NETWORK_SELECTION_MODE_LIMITED_ROUTINE_ACQUISITION))) &&
             ((mm_check_forbidden_national_roaming(&new_lai)) ||
              (mm_check_forbidden_regional_roaming(&new_lai)) ||
#ifdef FEATURE_DUAL_SIM
             (reg_sim_per_subs_plmn_forbidden( mm_serving_plmn.info.plmn, (sys_modem_as_id_e_type)mm_sub_id ))
#else
             (reg_sim_plmn_forbidden( mm_serving_plmn.info.plmn ))
#endif
#ifdef FEATURE_FEMTO_CSG
             || (!mm_is_csg_in_white_list(mm_serving_plmn.info.plmn, &mm_serving_plmn.info.csg_info))
#endif 
)
 #ifdef FEATURE_FEMTO_CSG
              && (!is_manual_csg_selection_success)
#endif 
               ))
    {
      MSG_HIGH_DS_0(MM_SUB, "=MM= LAI is in a forbidden list");
      if (mm_reg_waiting_for_reg_cnf)
      {
         mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
         mmcoord_mmr_reg_cnf.service_state.service_status = SYS_SRV_STATUS_NO_SRV;
         mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;
         mm_send_mmr_reg_cnf();
         mm_reg_waiting_for_reg_cnf = FALSE;
         mm_ready_to_send_reg_cnf   = FALSE;
      }
      else
      {
         mmr_service_ind.service_state = mm_serving_plmn.info;
         mmr_service_ind.service_state.service_status = SYS_SRV_STATUS_NO_SRV;
         mmr_service_ind.cause = MMR_CAUSE_NOT_SPECIFIED;
         mm_send_mmr_service_ind( &mmr_service_ind );
      }
    }
    else
    {
      if((gmm_state == GMM_DEREGISTERED_INITIATED) ||
         ((gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED) && gmm_detach_after_rau_pending))
      {
        gmm_handle_gprs_detach_low_level_failure();
        mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE); 
        gmm_detach_after_rau_pending = FALSE;
      }
      if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_PS_ONLY)
      {
        gmm_handle_plmn_found(mm_serving_plmn,attach_required);
      }
      else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_ONLY)
      {
        mm_handle_plmn_found(mm_serving_plmn,lu_required);
      } 
      else if (mm_serving_plmn.ms_op_mode == SYS_SRV_DOMAIN_CS_PS)
      {
        if (mm_serving_plmn.network_op_mode == NETWORK_OP_MODE_1)
        {
          gmm_handle_plmn_found(mm_serving_plmn,attach_required);
        }
        else
        {
          mm_handle_plmn_found(mm_serving_plmn,lu_required);
          gmm_handle_plmn_found(mm_serving_plmn,attach_required);
        }
      }
    }
  } 
}   

/*===========================================================================
FUNCTION    mmcoord_process_mmr_profile_type_ind

DESCRIPTION
  This function would be called when REG sends the profile_ind to MM
  informing if it's a eCall profile or not.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmcoord_process_mmr_profile_type_ind(mm_cmd_type *message)
{
   if (message->cmd.mmr_profile_type_ind.profile_type == MMGSDI_PROFILE_TYPE_EMERGENCY)
   {
     mm_ecall_profile = TRUE;
   }
   else
   {
     mm_ecall_profile = FALSE;
   }
}

#endif /*end FEATURE_NAS_ECALL*/

#ifdef FEATURE_DUAL_SIM
void mmcoord_process_mmr_t3245_expiry_ind(mm_cmd_type *message) 
{
   if (message->cmd.mmr_t3245_expiry_ind.message_header.message_id == MMR_T3245_EXPIRY_IND)
   {
      MSG_HIGH_DS_0(MM_SUB,"=MM= MM received MMR_T3245_EXPIRY_IND ");
      mm_is_sim_to_be_cs_invalid_after_power_off = FALSE;
      mm_is_sim_to_be_ps_invalid_after_power_off = FALSE;
   }
}
#endif

#ifdef FEATURE_HSDPA
/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_HSDPA_HSUPA_CELL_IND

DESCRIPTION
  This function processes RRCHSDPA_HSUPA_CELL_IND from RRC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_rrc_hsdpa_hsupa_cell_ind(mm_cmd_type *message)
{
   if(message->cmd.hdr.message_id == (int) RRC_HSDPA_HSUPA_CELL_IND)
   {
     MSG_HIGH_DS_0(MM_SUB,"=MM= MM received RRC_HSDPA_HSUPA_CELL_IND");
     mm_serving_plmn.hsdpa_hsupa_ind = message->cmd.rrc_3g_availability_cell_ind.hsdpa_hsupa_ind;

     if((mm_serving_plmn.info.service_status == SYS_SRV_STATUS_NO_SRV) &&
        (mm_serving_plmn.gmm_service_state == SERVICE_NOT_AVAILABLE))
     {
       MSG_HIGH_DS_0(MM_SUB,"=MM= Ignoring RRC_HSDPA_HSUPA_CELL due to No service");
       mm_serving_plmn.hsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
       return;
     }
      if((mmcoord_mmr_reg_cnf.only_for_reg == FALSE) &&
         (sys_plmn_match( mmcoord_mmr_reg_cnf.service_state.plmn, mm_serving_plmn.info.plmn)) &&
         (mmcoord_mmr_reg_cnf.rac == mm_serving_plmn.rac) &&
         (mmcoord_mmr_reg_cnf.service_state.lac == b_unpackw(mm_serving_plmn.lac.lac,0,16)) &&
         (mmcoord_mmr_reg_cnf.service_state.active_rat == mm_serving_plmn.info.active_rat))
     {
       mm_send_mmr_cell_service_ind();  
     }
     else
     {
       mm_mmr_cell_service_ind_pending = TRUE;
     }
   }
   else
   {
      MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
   }
} /* end mmcoord_process_rrc_hsdpa_hsupa_cell_ind */

#ifdef FEATURE_HSPA_CALL_STATUS_IND
/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_HSPA_CALL_STATUS_IND

DESCRIPTION
  This function processes RRC_HSPA_CALL_STATUS_IND from RRC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void mmcoord_process_rrc_hspa_call_status_ind(const mm_cmd_type *message)
{
   mmr_hspa_call_status_ind_s_type hspa_call_status_ind;
   if(message->cmd.hdr.message_id == (int) RRC_HSPA_CALL_STATUS_IND)
   {
     hspa_call_status_ind.hs_call_status_ind = message->cmd.rrc_hspa_call_status_ind.hs_call_status_ind;
     mm_send_mmr_hspa_call_status_ind(&hspa_call_status_ind);
   }
   else
   {
      MSG_FATAL_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
   }
} /* end mmcoord_process_rrc_hspa_call_status_ind */
#endif
#endif

#ifdef FEATURE_LTE
/*===========================================================================

FUNCTION MMCOORD_PROCESS_RRC_PCH_STATE_IND

DESCRIPTION
  This function processes RRC_PCH_STATE_IND from RRC

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void mmcoord_process_rrc_pch_state_ind(const mm_cmd_type *message)
{
  MSG_HIGH_DS_1(MM_SUB ,"=MM= receieved RRC_PCH_STATE_IND pch_state %d",
         message->cmd.rrc_pch_state_ind.pch_state_info.is_in_pch_state);
  mm_rrc_pch_state_info = message->cmd.rrc_pch_state_ind.pch_state_info;
}

#endif

#ifdef FEATURE_OOSC_SERVICE_STATUS_UI_UPDATE
static void mmcoord_process_rrc_connected_mode_service_status_ind(const mm_cmd_type *message)
{
  mmr_conn_mode_service_status_ind_s_type conn_mode_service_status_ind;
  if(message->cmd.hdr.message_id == (int)RRC_CONNECTED_MODE_SERVICE_STATUS_IND)
  {
    conn_mode_service_status_ind.conn_mode_ss_ind.service_status = message->cmd.rrc_conn_mode_service_status_ind.service_status;
    conn_mode_service_status_ind.conn_mode_ss_ind.selected_plmn =  mmumts_convert_rrc_plmn_id_to_nas_plmn_id(message->cmd.rrc_conn_mode_service_status_ind.selected_plmn.plmn_id);
    conn_mode_service_status_ind.conn_mode_ss_ind.rat = message->cmd.rrc_conn_mode_service_status_ind.rat;
    conn_mode_service_status_ind.conn_mode_ss_ind.signal_quality = message->cmd.rrc_conn_mode_service_status_ind.signal_quality;
    conn_mode_service_status_ind.conn_mode_ss_ind.signal_strength = message->cmd.rrc_conn_mode_service_status_ind.signal_strength;
#ifdef FEATURE_OOSC_USER_ACTION
    conn_mode_service_status_ind.conn_mode_ss_ind.is_connected_mode_oos = message->cmd.rrc_conn_mode_service_status_ind.is_connected_mode_oos;
#endif

    mm_send_mmr_conn_mode_service_status_ind(&conn_mode_service_status_ind);
  }
  else
  {
    MSG_FATAL_DS(MM_SUB, "=MM= Unexpected message %d",message->cmd.hdr.message_id,0,0);
  }
}
#endif

#ifdef FEATURE_GPRS_PS_HANDOVER
/*************************************************************
FUNCTION mmcoord_process_rr_psho_ind

DESCRIPTION
  This function processes RR_PSHO_IND from RR

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
 *************************************************************/
 static void mmcoord_process_rr_psho_ind(const mm_cmd_type *message)
{      
   if(is_gsm_mode())
   {
     gmm_psho_status = GMM_PSHO_STARTED;
     
     MSG_HIGH_DS_0(MM_SUB, "=MM= MM received RR_PSHO_IND");    
     
     if (message->cmd.rr_psho_ind.nas_container_present)
     {
       memscpy(gmm_psho_nas_container, PSHO_NAS_CNT_SIZE,message->cmd.rr_psho_ind.nas_container.data,PSHO_NAS_CNT_SIZE);
       
       gmmllc_gmm_trigger_llc_event( GMM_TRIGGER_PSHO_STARTED, FALSE, 0 );     
     }
     else
     {
        mm_send_rr_psho_cnf();
     }
   }
   else
   {
     MSG_FATAL_DS( MM_SUB, "=MM= Message expected only in G RAT", 0, 0,0 );
   }
} /* end mmcoord_process_rr_psho_ind */
#endif


/*************************************************************
FUNCTION mm_set_mm_controls_resel_dec_mt_cs_call

DESCRIPTION
  This function will extract CNM reselection control status
  and based on that resets MM reselection control status for MT calls.

  Note that secondlast byte of MMCNM_DATA_REQ contains this info.
  And Last byte contains as_id which is already decoded before control comes here.
  So, effectively, last byte in array at this time contains cnm resel control byte.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
 *************************************************************/
void mm_set_mm_controls_resel_dec_mt_cs_call(mm_cmd_type *message)
{
  uint16 len;
  GET_IMH_LEN(len, &message->cmd.hdr);
  if(message->cmd.mmcnm_data_req.data[len-1] == TRUE)
  {
    MSG_HIGH_DS_0(MM_SUB, " =MM= Resetting mm_controls_resel_dec_mt_cs_call");
    mm_controls_resel_dec_mt_cs_call = FALSE;
  }
  PUT_IMH_LEN(len - sizeof(byte), &message->cmd.hdr);
}


/*************************************************************
FUNCTION mmcoord_process_rr_ps_status_change_ind

DESCRIPTION
  This function processes RR_PS_STATUS_CHANGE_IND from RR
    
DEPENDENCIES
  None
    
RETURN VALUE
  None
    
SIDE EFFECTS
  None
*************************************************************/
static void mmcoord_process_rr_ps_status_change_ind(const mm_cmd_type *message)
{      
  gmm_is_ps_active = message->cmd.rr_ps_status_change_ind.ps_is_active;  
  MSG_HIGH_DS_1( MM_SUB, "=MM= MM received RR_PS_STATUS_CHANGE_IND, ps_status_change_ind = %d",  message->cmd.rr_ps_status_change_ind.ps_is_active);    
     
} /* end mmcoord_process_rr_psho_ind */

#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
/*************************************************************
FUNCTION mm_get_as_id_from_cnm_data_req

DESCRIPTION
  This function will extract SIM_ID from MMCNM_DATA_REQ and also adjusts
  the length of the message after excluding as_id byte

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
 *************************************************************/

mm_as_id_e_type mm_get_as_id_from_cnm_data_req(mm_cmd_type *message)
{
  mm_as_id_e_type sub_id;
  uint16 len;
  GET_IMH_LEN(len, &message->cmd.hdr);
  sub_id = (mm_as_id_e_type) message->cmd.mmcnm_data_req.data[len-1];
  if(IS_NOT_VALID_SUB_ID((sys_modem_as_id_e_type)sub_id))
  {
    MSG_FATAL_DS(MM_SUB, "=MM= Invalid AS ID: %d", sub_id+1,0,0);
  }
  PUT_IMH_LEN(len - sizeof(byte), &message->cmd.hdr);
  return sub_id;
}

/*************************************************************
FUNCTION mm_set_as_id

DESCRIPTION
  This function will set mm_as_id, mm_sub_id and mm_message_id 
  based on the as_id passed as parameter.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
 *************************************************************/
void mm_set_as_id(mm_as_id_e_type as_id)
{
  mm_as_id = as_id;
  mm_sub_id = mm_sub_id_stack[as_id];
  mm_message_as_id = (byte)(as_id + 1);
}



/*************************************************************
FUNCTION mm_get_as_id

DESCRIPTION
  This function will extract SIM_ID from all incoming primitives and enables  processing of  the primitive 
  in the context of extracted SIM_ID

DEPENDENCIES
  None

RETURN VALUE
  TRUE if AS ID is valid

SIDE EFFECTS
  None
 *************************************************************/
boolean mm_get_as_id(mm_cmd_type *message)
{
  boolean status = TRUE;
  sys_modem_as_id_e_type	  mm_as_id_local;
#ifdef FEATURE_SGLTE       
  rrc_cn_domain_identity_e_type  domain_identity;
  dword client_ref;
#endif
  switch (message->cmd.hdr.message_set)
  {

    case MS_MM_RR :
#ifdef FEATURE_DUAL_SIM
      switch (message->cmd.hdr.message_id)
      {
        case RR_PLMN_SELECT_CNF:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_plmn_select_cnf.as_id;
          break;

        case RR_SERVICE_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_service_ind.as_id;
          break;
#ifdef FEATURE_GSM_DTM
        case RR_SERVICE_IND_FROM_OTA_MSG:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_service_ind.as_id;
          break;
#endif /* #ifdef FEATURE_GSM_DTM */
        case RR_ACTIVATION_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_activation_ind.as_id;
          break;
        case RR_EST_CNF:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_est_cnf.as_id;
          break;
        case RR_DS_DATA_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_ds_data_ind.as_id;
          break;
        case RR_PLMN_LIST_CNF:
          mm_sub_id = (mm_as_id_e_type) message->cmd.rr_plmn_list_cnf.as_id;
          break;
        case RR_STOP_GSM_MODE_CNF:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_stop_gsm_mode_cnf.as_id;
          break;
#ifdef FEATURE_INTERRAT_PCCO_GTOW
        case RR_GTOW_CCO_COMPLETE_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_gtow_cco_complete_ind.as_id;
          break;
#endif /* FEATURE_INTERRAT_PCCO_GTOW */
        case RR_REL_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_rel_ind.as_id;
          break;
        case RR_DEACT_CNF:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_deact_cnf.as_id;
          break;
        case RR_ABORT_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_abort_ind.as_id;
          break;
        case RR_ABORT_CNF:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_abort_cnf.as_id;
          break;
        case RR_SYNC_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_sync_ind.as_id;
          break;
        case RR_PAGING_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_paging_ind.as_id;
          break;
#ifdef FEATURE_GSM_GPRS
       case RR_GMM_GPRS_SUSPENSION:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_gmm_gprs_suspension.as_id;
          break;
       case RR_GMM_GPRS_RESUMPTION_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_gmm_gprs_resumption_ind.as_id;
          break;
        case RR_RESELECTION_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_reselection_ind.as_id;
          break;
#ifdef FEATURE_NAS_FREE_FLOAT
        case RR_GMM_GPRS_SERVICE_CNF:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_gprs_service_cnf.as_id;
          break;
#endif

        case RR_GPRS_PCCCH_CAMP_CNF:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_gprs_pccch_camp_cnf.as_id;
          break;

        case RR_GMM_GPRS_MULTISLOT_CLASS_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_gmm_gprs_multislot_class_ind.as_id;
          break;

#endif
#if defined (FEATURE_GSM_EDTM) || defined (FEATURE_GAN)
        case RR_EST_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_est_ind.as_id;
          break;
#endif
#ifdef FEATURE_GPRS_PS_HANDOVER
        case RR_PSHO_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_psho_ind.as_id;
          break;
#endif
        case RR_PS_ABORT_CNF:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_ps_abort_cnf.as_id;
          break;
        case RR_GMM_GPRS_MS_RA_CAP_REQ:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_gmm_gprs_ms_ra_cap_req.as_id;
          break;
        case RR_SRVCC_HANDOVER_FAILURE_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_srvcc_handover_failure_ind.as_id;
          break;
#ifdef FEATURE_DUAL_SIM
        case RR_MULTIMODE_SUBS_CHGD_CNF:
          break;
#endif
        case RR_PS_STATUS_CHANGE_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_ps_status_change_ind.as_id;
          break;
      
        case RR_PREEMPTION_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_preemption_ind.as_id;
          break;

        case RR_PLMN_RESEL_FAIL_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_plmn_resel_fail_ind.as_id;
          break;

        case RR_GMM_RETX_PDU_TUNEAWAY_FAIL_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.rr_gmm_retx_pdu_tuneaway_fail_ind.as_id;
          break;

        default:
          MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RR cmd %d", message->cmd.hdr.message_id, 0,0 );
      }
#endif  
      mm_as_id = mm_sub_id; 
#ifdef FEATURE_SGLTE
      if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
      {
        if(MM_SUB_IS_SGLTE)
        {
          if(MM_IS_IN_SGLTE_MODE && 
              ((message->cmd.hdr.message_id ==RR_ACTIVATION_IND ) ||
              (message->cmd.hdr.message_id ==RR_GMM_GPRS_MS_RA_CAP_REQ )))
          {
            mm_as_id = mm_ps_stack_id;
          }
          else
          {
            mm_as_id = mm_cs_stack_id;
          }       
        }
      }
#endif
      break;
#if defined ( FEATURE_MMGPS ) || defined ( FEATURE_CGPS_UMTS_CP )
    case MS_MM_LSM:
      switch (message->cmd.hdr.message_id)
      {
          case LCS_ACTIVE_IND:/*NO SIM ID*/
            /* Control plane GPS is supported only in SS mode, just make sure we are camped here */
#if defined(FEATURE_TRIPLE_SIM) || defined (FEATURE_DUAL_SIM)||defined(FEATURE_SGLTE_DUAL_SIM)
            for(mm_as_id_local = (sys_modem_as_id_e_type)MM_AS_ID_1; mm_as_id_local < MAX_NAS_STACKS ; mm_as_id_local++)
            {
			  mm_as_id = (mm_as_id_e_type)mm_as_id_local;
              mm_sub_id = mm_sub_id_stack[mm_as_id];
              if(MM_SUB_IS_SGLTE_SUB(mm_sub_id))
              {
                mm_as_id = mm_cs_stack_id;
                break;
              }
              else
              {
                if (mm_serving_plmn_sim[mm_as_id].info.active_rat != SYS_RAT_NONE)
                {
                  break;
                }
              }
            }
            if(mm_as_id_local >= MAX_NAS_STACKS)
            {
              /*If there is no service on any stack assign default value as as_id_1*/
              mm_as_id = MM_AS_ID_1;
              mm_sub_id = MM_AS_ID_1; 
            }
#elif defined FEATURE_SGLTE
            mm_as_id = mm_cs_stack_id;
#endif
            break;
          default:
            MSG_FATAL_DS( MM_SUB, "=MM= Unexpected LSM cmd %d", message->cmd.hdr.message_id, 0,0 );
      }
      break;
#endif /* end of (FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP) */

  case MS_MM_RRC:
  case MS_MM_TDSRRC:
#ifdef FEATURE_DUAL_SIM
    switch (message->cmd.hdr.message_id)
    {
      case RRC_EST_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_est_cnf.as_id;
        break;
      case RRC_REL_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_rel_ind.as_id;
        break;
      case RRC_SYNC_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_sync_ind.as_id;
        break;
      case RRC_SERVICE_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_service_cnf.as_id;
        break;
      case RRC_SERVICE_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_service_ind.as_id;
        break;
      case RRC_SERVICE_IND_FROM_OTA_MSG:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_service_ind.as_id;
        break;
      case RRC_PLMN_LIST_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_plmn_list_cnf.as_id;
        break;
      case RRC_ABORT_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_abort_ind.as_id;
        break;
      case RRC_PAGE_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_page_ind.as_id;
        break;
      case RRC_DATA_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_data_ind.as_id;
        break;
      case RRC_OPEN_SESSION_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_open_session_cnf.as_id;
        break;
      case RRC_CLOSE_SESSION_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_close_session_ind.as_id;
        break;
      case RRC_CLOSE_SESSION_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_close_session_cnf.as_id;
        break;
      case RRC_ABORT_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_abort_cnf.as_id;
        break;
      case RRC_MODE_CHANGE_CNF:
      case RRC_DEACT_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_deact_cnf.as_id;
        break;
#ifdef FEATURE_INTERRAT_HANDOVER_WTOG
      case RRC_STOP_WCDMA_MODE_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_stop_wcdma_mode_cnf.as_id;
        break;
#endif
      case RRC_ACTIVATION_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_activation_ind.as_id;
        break;
#ifdef FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER
      case RRC_WTOG_CCO_COMPLETE_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_wtog_cco_complete_ind.as_id;
        break;
#endif /* FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER */
#ifdef FEATURE_HSDPA
      case RRC_HSDPA_HSUPA_CELL_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_3g_availability_cell_ind.as_id;
        break;
#ifdef FEATURE_HSPA_CALL_STATUS_IND
      case RRC_HSPA_CALL_STATUS_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_hspa_call_status_ind.as_id;
        break;
#endif
#endif
#ifdef FEATURE_OOSC_SERVICE_STATUS_UI_UPDATE
      case RRC_CONNECTED_MODE_SERVICE_STATUS_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_conn_mode_service_status_ind.as_id;
        break;
#endif
#ifdef FEATURE_MBMS
      #error code not present
#endif
      case RRC_FAST_DORMANCY_CNF:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_fast_dormancy_cnf.as_id;
        break;
        case RRC_SUITABLE_SEARCH_END_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_suitable_search_end_ind.as_id;
        break;
        case RRC_DISABLE_BPLMN_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_disable_bplmn_ind.as_id;
        break;
        case RRC_SRVCC_HANDOVER_FAILURE_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_srvcc_handover_failure_ind.as_id;
        break;
        case RRC_PCH_STATE_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.rrc_pch_state_ind.as_id;
        break;
      default:
         MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RRC cmd %d", message->cmd.hdr.message_id, 0,0 );
    }
#endif
    mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
    if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
    {
      if(MM_SUB_IS_SGLTE)
      {
        mm_as_id = mm_ps_stack_id;
      }
    }
#endif 
    break;

  case MS_CC_MM_PS :
  case MS_CC_MM:
#ifdef FEATURE_DUAL_SIM
    switch (message->cmd.hdr.message_id)
    {
      case MMCNM_DATA_REQ:
        mm_sub_id = mm_get_as_id_from_cnm_data_req(message);
        break;
      case MMCNM_EST_REQ:
        mm_sub_id = (mm_as_id_e_type)message->cmd.mmcnm_est_req.as_id;
        break;
      case MMCC_REEST_REQ:
        mm_sub_id = (mm_as_id_e_type)message->cmd.mmcc_reest_req.as_id;
        break;
      case MMCNM_REL_REQ:
        mm_sub_id = (mm_as_id_e_type)message->cmd.mmcnm_rel_req.as_id;
        break;
      case MMCNM_UNIT_DATA_REQ:/*DATA REQ handled differently*/
        break; 
#ifdef FEATURE_CCBS
      case MMCNM_PROMPT_RES:
        mm_sub_id = (mm_as_id_e_type)message->cmd.mmcnm_prompt_res.as_id;
        break;
#endif
      default:
         MSG_FATAL_DS( MM_SUB, "=MM= Unexpected CNM CC cmd %d %d", message->cmd.hdr.message_id, message->cmd.hdr.message_set,0 );
    }
#endif
    mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
    if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
    {
      if(MM_SUB_IS_SGLTE)
      {
        mm_as_id = mm_cs_stack_id;
      }
    }
#endif 
    break;
  case MS_MM_REG:
    switch (message->cmd.hdr.message_id)
    {
      case MMR_REG_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_reg_req.as_id;
        break;
      case MMR_GSTK_SERVICE_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_gstk_service_ind.as_id;
        break;
      case MMR_EQ_PLMN_CHANGE_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_eq_plmn_change_ind.as_id;
        break;
      case MMR_SIM_AVAILABLE_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_sim_available_req.as_id;
        break;
      case MMR_SIM_NOT_AVAILABLE_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_sim_not_available_req.as_id;
        break;
      case MMR_STOP_MODE_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_stop_mode_req.as_id;
        break;
      case MMR_PLMN_SEARCH_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_plmn_search_req.as_id;
        break;
      case MMR_PLMN_SEARCH_ABORT_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_plmn_search_abort_reg.as_id;
        break;
      case MMR_PH_STATUS_CHANGE_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_ph_status_change_req.as_id;
        break;
       case MMR_SIM_STATE_UPDATE_IND:
         mm_as_id = (mm_as_id_e_type)message->cmd.mmr_sim_state_update_ind.as_id;
        break;
#ifdef FEATURE_OOSC_USER_ACTION
      case MMR_EXIT_FROM_PWR_SAVE:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_exit_from_pwr_save.as_id;
        break;
#endif
#ifdef FEATURE_DUAL_SIM
      case MMR_DS_STAT_CHGD_REQ:/*NO SIM ID*/
        break;
      case MMR_DS_TUNEAWAY_STATUS_CHANGE_IND: /*NO SIM ID*/
        break;

      case MMR_MULTIMODE_SUBS_CHGD_REQ:
        break;

      case MMR_DDS_SWITCH_IND: /*NO SIM ID*/
        break;
#endif
#ifdef FEATURE_LTE
      case MMR_GW_PLMN_SEL_END_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_gw_plmn_sel_end_ind.as_id;
        break;
#endif

#ifdef FEATURE_FEMTO_CSG
      case MMR_FILE_REFRESH_IND:
       mm_as_id = (mm_as_id_e_type)message->cmd.mmr_file_refresh_ind.as_id;
        break;
#endif
      case MMR_NET_SEL_MODE_CHANGE_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_net_sel_mode_change_ind.as_id;
        break;
        
      case MMR_CLEAR_LAI_REJECT_LIST_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_clear_lai_reject_list_req.as_id;
        break;

#if defined(FEATURE_DUAL_SIM) || defined(FEATURE_SGLTE)
      case MMR_PS_DETACH_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_ps_detach_req.as_id;
        break;
#endif
#if (defined(FEATURE_DUAL_SIM) && defined(FEATURE_LTE)) || defined(FEATURE_SGLTE)
      case MMR_SET_DRX_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_set_drx_req.as_id;
        break;
      case MMR_GET_DRX_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_get_drx_req.as_id;
        break;

      case MMR_BLOCK_PLMN_REQ:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_block_plmn_req.as_id;
        break;
#endif
      case MMR_UE_MODE_CHANGE_IND :
        mm_as_id = (mm_as_id_e_type)message->cmd.mm_ue_mode_ind.as_id;
        break;
#ifdef FEATURE_DUAL_SIM
    case MMR_DATA_PRIORITY_IND :
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_data_prio_ind.as_id;
        break;
#endif

      case MMR_PSEUDO_LIST_UPDATE_IND:                
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_pseudo_list_update_ind.as_id;
        break;
#ifdef FEATURE_CMCC_SECURE_CALL
      case MMR_SECURE_CALL_IND: 
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_secure_call_ind.as_id;
        break;
#endif
#ifdef MMOC_API_PH_STAT_CNF_WITH_STATUS
      case MMR_SIM_BUSY_IND:
        mm_as_id =(mm_as_id_e_type)message->cmd.mmr_sim_busy_ind.as_id;   
        break;
#endif

#ifdef FEATURE_FEMTO_CSG
      case MMR_CSG_SELECT_CONFIG_UPDATE_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_csg_sel_cfg_update_ind.as_id;
        break;
#endif

#ifdef FEATURE_NAS_ECALL
      case MMR_ECALL_MODE_IND:
       mm_as_id = (mm_as_id_e_type)message->cmd.mmr_ecall_mode_ind.as_id;
       break;

      case MMR_PROFILE_TYPE_IND:
       mm_as_id = (mm_as_id_e_type)message->cmd.mmr_profile_type_ind.as_id;
       break;
#endif

      case MMR_REFRESH_STAGE_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_refresh_stage_ind.as_id;
        break;

      case MMR_HIGH_PRIORITY_ACTIVITY_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_high_priority_activity_ind.as_id;
        break;

#ifdef FEATURE_DUAL_SIM
      case MMR_T3245_EXPIRY_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmr_t3245_expiry_ind.as_id;
        break;
#endif

       default:
#ifdef FEATURE_DUAL_SIM
        MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG cmd %d", message->cmd.hdr.message_id, 0,0 );
#else
        MSG_ERROR_DS( MM_SUB, "=MM= Unexpected REG cmd %d, routing it to Main stack", message->cmd.hdr.message_id, 0,0 );
        mm_as_id = mm_ps_stack_id;
#endif
    }
    if(!IS_NOT_VALID_STACK_ID(mm_as_id))
    {
      mm_sub_id = mm_sub_id_stack[mm_as_id];
    }
    break;

    case MS_TIMER:
      if (message->cmd.hdr.message_id == MID_TIMER_EXPIRY)
      {
#if defined FEATURE_DUAL_SIM || defined FEATURE_SGLTE
        /* Extract AS ID from client data portion of expiry */
        mm_as_id = (mm_as_id_e_type)(message->cmd.timer_expiry.data);
        if(!IS_NOT_VALID_STACK_ID(mm_as_id))
        {
          mm_sub_id = mm_sub_id_stack[mm_as_id];
#ifdef FEATURE_SGLTE
          /* route CS timer expiry which may have been started before merge/split */
          if ((MM_IS_IN_SGLTE_STATE_4) || 
              ((MM_IS_IN_SGLTE_MODE) && 
               (MM_IS_SGLTE_ON_PS_STACK_ID) &&
               (mm_is_cs_timer(message->cmd.timer_expiry.timer_id))))
          {
            mm_as_id = mm_cs_stack_id;
          }
#endif
        }
#endif 
      }
      else
      {
        MSG_ERROR_DS( MM_SUB, "=MM= Unexpected timer type %d", message->cmd.hdr.message_id, 0,0 );
      } 
      break;

  case MS_GSDI:
    switch (message->cmd.hdr.message_id)
    {
      case SIM_MM_USIM_AUTH_CNF:
       mm_as_id =  (mm_as_id_e_type)message->cmd.sim_mm_gsdi_cnf.usim_auth_cnf.as_id;
       if(!IS_NOT_VALID_STACK_ID(mm_as_id))
       {
         mm_sub_id = mm_sub_id_stack[mm_as_id];
#ifdef FEATURE_SGLTE
          if(MM_SUB_IS_SGLTE)
          {
            //auth_filter_domain_identity(message,&domain_identity);
            client_ref = message->cmd.sim_mm_gsdi_cnf.message_header.client_ref;
            domain_identity = (rrc_cn_domain_identity_e_type)((client_ref&0x80000000)>>31);
            if (mm_sglte_mode==TRUE) 
            {
              if(RRC_CS_DOMAIN_CN_ID == domain_identity)
              {
                mm_as_id=mm_cs_stack_id;
              }
              else if(RRC_PS_DOMAIN_CN_ID == domain_identity)
              {
                mm_as_id=mm_ps_stack_id;
              }
            }
          }
 #endif       
       }
         break;

      case SIM_MM_GSM_ALGO_CNF:
        mm_as_id = (mm_as_id_e_type)message->cmd.sim_mm_gsdi_cnf.run_gsm_alg_cnf.as_id;
        mm_sub_id = mm_sub_id_stack[mm_as_id];
#ifdef FEATURE_SGLTE
        if(MM_SUB_IS_SGLTE)
        {
      //auth_filter_domain_identity(message,&domain_identity);
       client_ref = message->cmd.sim_mm_gsdi_cnf.message_header.client_ref;
       domain_identity = (rrc_cn_domain_identity_e_type)((client_ref&0x80000000)>>31);
       if (mm_sglte_mode==TRUE) 
         {  
            if(RRC_CS_DOMAIN_CN_ID == domain_identity)
            {
              mm_as_id=mm_cs_stack_id;
            }
            else if(RRC_PS_DOMAIN_CN_ID == domain_identity)
            {
              mm_as_id=mm_ps_stack_id;
            }
         } 
        }
 #endif  
         break;

      case SIM_MM_SUBS_READY_IND:
        mm_as_id =  (mm_as_id_e_type)message->cmd.sim_subs_ready_ind.as_id;
        mm_sub_id = mm_sub_id_stack[mm_as_id];
#ifdef FEATURE_SGLTE
        if(MM_SUB_IS_SGLTE)
        {
        mm_as_id = mm_ps_stack_id;
        }
#endif
        break;

#ifdef FEATURE_FEMTO_CSG
      case SIM_MM_READ_FILE_CNF:
       mm_as_id =  (mm_as_id_e_type)message->cmd.mmgsdi_read_file_cnf.as_id;
       mm_sub_id = mm_sub_id_stack[mm_as_id];
       break;

      case SIM_MM_GET_FILE_ATTR_CNF:
        mm_as_id = (mm_as_id_e_type)message->cmd.mmgsdi_get_file_attr_cnf.as_id;
        mm_sub_id = mm_sub_id_stack[mm_as_id];
        break;
#endif 
      case SIM_MM_PURGE_GUTI_IND:
        mm_as_id = (mm_as_id_e_type)message->cmd.gsdi_guti_purge_ind.as_id;
        mm_sub_id = mm_sub_id_stack[mm_as_id];
        break;

        default:
        MSG_FATAL_DS( MM_SUB, "=MM= Unexpected GSDI cmd %d", message->cmd.hdr.message_id, 0,0 );
    }
    break;

  case MS_GMM_SMS:
    break;

  case MS_GMM_SM:
#ifdef FEATURE_DUAL_SIM
    switch (message->cmd.hdr.message_id)
    {
      case GMMSM_ESTABLISH_REQ:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.gmmsm_establish_req.as_id;
        break;
      case GMMSM_UNITDATA_REQ:
        mm_sub_id = (mm_as_id_e_type)message->cmd.gmmsm_unitdata_req.as_id;
        break;
      case GMMSM_PS_CALL_STATUS_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.gmmsm_ps_call_status_ind.as_id;
        break;
      case GMMSM_FAST_DORMANCY_REQ:
      case GMMSM_CLOSE_SESSION_REQ:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.gmmsm_session_close_req.as_id;
        break;
#ifdef FEATURE_GSM_GPRS
      case GMMSM_RAT_RESUME_CNF:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.gmmsm_rat_resume_cnf.as_id;
        break;
#endif
      default:
        MSG_FATAL_DS( MM_SUB, "=MM= Unexpected SM cmd %d", message->cmd.hdr.message_id, 0,0 );
    }
#endif
    mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
    if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
    {
      if(MM_SUB_IS_SGLTE)
      {
        mm_as_id = mm_ps_stack_id;
      }
    }
#endif 
    break;

  case MS_GMM_RABM:
#ifdef FEATURE_DUAL_SIM
    switch (message->cmd.hdr.message_id)
    {
      case GMMRABM_REESTABLISH_REQ:
          mm_sub_id =  (mm_as_id_e_type)message->cmd.gmmrabm_reestablish_req.as_id;
        break;
      case GMMRABM_RAB_REL_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.gmmrabm_rab_rel_ind.as_id;
        break;
#ifdef FEATURE_NAS_REL12
        case GMMRABM_RAB_EST_IND:
          mm_sub_id = (mm_as_id_e_type)message->cmd.gmmrabm_rab_est_ind.as_id;
          break;
#endif
      default:
        MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RABM cmd %d", message->cmd.hdr.message_id, 0,0 );
    }
#endif
    mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
    if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
    {
      if(MM_SUB_IS_SGLTE)
      {
        mm_as_id = mm_ps_stack_id;
      }
    }
#endif 
    break;

  case MS_GMM_TC:
  case MS_MM_TC:
#ifndef FEATURE_DUAL_WCDMA
       if( !(mm_sub_is_rat_enabled(SYS_RAT_UMTS_RADIO_ACCESS) || mm_sub_is_rat_enabled(SYS_RAT_TDS_RADIO_ACCESS)))
       {
           mm_as_id = MM_OTHER_SUB(mm_as_id);
	   mm_sub_id = mm_sub_id_stack[mm_as_id];
	   MSG_HIGH_DS_2(MM_SUB,"=MM= TC MSG RCVD : Mapped to Multimode SUB : mm_as_id : %d, mm_sub_id : %d",mm_as_id, mm_sub_id );
       }
#else
    switch (message->cmd.hdr.message_id)
    {
      case MMTC_UNITDATA_REQ: 
      case GMMTC_UNITDATA_REQ:
        mm_sub_id =  (mm_as_id_e_type)(message->cmd.gmmtc_unitdata_req.as_id);
        break;
      default:
        MSG_FATAL_DS( MM_SUB, "=MM= Unexpected TC cmd %d", message->cmd.hdr.message_id, 0,0 );
    }
    mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
    if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
    {
      if(MM_SUB_IS_SGLTE)
      {
        mm_as_id = mm_ps_stack_id;
      }
    }
#endif

#endif

    break;

#ifdef FEATURE_GSM_GPRS
  case MS_LLC_GMM :
#ifdef FEATURE_DUAL_SIM
    switch (message->cmd.hdr.message_id)
    {
      case GLLCMM_LLGMM_STATUS_IND: 
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llgmm_status_ind.as_id;
        break;
      case GLLCMM_LLGMM_TRIGGER_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llgmm_trigger_ind.as_id;
        break;
      case GLLCMM_LLGMM_RDY_TMR_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llgmm_rdy_tmr_ind.as_id;
        break;
      case GLLCMM_LL_UNITDATA_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.ll_unitdata_ind.as_id;
        break;
      case GLLCMM_LL_XID_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.ll_xid_ind.as_id;
        break;
      case GLLCMM_LL_XID_CNF:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.ll_xid_cnf.as_id;
        break;
      case GLLCMM_LL_RESET_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.ll_reset_ind.as_id;
        break;
      case GLLCMM_LL_STATUS_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.ll_status_ind.as_id;
        break;
      case GLLCMM_LLGMM_SUSPEND_CNF:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llgmm_suspend_cnf.as_id;
        break;
      case GLLCMM_LLGMM_NEW_TLLI_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llgmm_new_tlli_ind.as_id;
        break;
#ifdef FEATURE_GPRS_PS_HANDOVER  
      case GLLCMM_LLGMM_PSHO_CNF:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llgmm_psho_cnf.as_id;
        break;
#endif
      case GLLCSMS_LL_UNITDATA_IND: 
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llsms_unitdata_ind.as_id;
        break;
      case GLLCSMS_LL_XID_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llsms_xid_ind.as_id;
        break;
      case GLLCSMS_LL_XID_CNF:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llsms_xid_cnf.as_id;
        break;
      case GLLCSMS_LL_RESET_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llsms_reset_ind.as_id;
        break;
      case GLLCSMS_LL_STATUS_IND:
        mm_sub_id =  (mm_as_id_e_type)message->cmd.llsms_status_ind.as_id;
        break;
      default:
        MSG_FATAL_DS( MM_SUB, "=MM= Unexpected LLC cmd %d", message->cmd.hdr.message_id, 0,0 );
    }
#endif
    mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
    if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
    {
      if(MM_SUB_IS_SGLTE)
      {
        mm_as_id = mm_ps_stack_id;
      }
    }
#endif

    break;

  case MS_CM_MM:
    switch (message->cmd.hdr.message_id)
    {
      case MMCM_PS_DATA_AVAILABLE_REQ:
        mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_ps_data_ready_req.as_id;
        mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
        if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
        {
          if(MM_SUB_IS_SGLTE)
          {
            mm_as_id = mm_ps_stack_id;
          }
        }
#endif
        break;
#if (defined(FEATURE_DUAL_SIM) && defined(FEATURE_LTE)) || defined(FEATURE_SGLTE)
      case MMCM_DUAL_RX_1XCSFB_TAU_REQ:
        mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_dual_rx_1xcsfb_tau_req.as_id;
        mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
        if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
        {
          if(MM_SUB_IS_SGLTE)
          {
            mm_as_id = mm_ps_stack_id;
          }
        }
#endif
        break;
#ifdef FEATURE_LTE_REL9
      case MMCM_IMS_REGISTRATION_INFO:
        mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_ims_registration_info.as_id;
        mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
        if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
        {
          if(MM_SUB_IS_SGLTE)
          {
            mm_as_id = mm_ps_stack_id;
          }
        }
 #endif
        break;
#endif
#endif

#ifdef FEATURE_NAS_ECALL
       case MMCM_ECALL_INACTIVITY_TRIGGER:
         mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_ecall_inactivity_trigger.as_id;
         mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
         if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
         {
           if(MM_SUB_IS_SGLTE)
           {
             mm_as_id = mm_ps_stack_id;
           } 
         }
#endif	   	
        break;
       case MMCM_ECALL_REMAINING_TIMER_VALUE:
         mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_ecall_remaining_timer_value.as_id;
         mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
         if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
         {
           if(MM_SUB_IS_SGLTE)
           {
             mm_as_id = mm_ps_stack_id;
           } 
         }
#endif	   
         break;
#endif
#ifdef FEATURE_LTE
    case MMCM_UE_CAPABILITIES_INFO:
      mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_ue_capabilities_info.as_id;
      mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
      if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
      {
        if(MM_SUB_IS_SGLTE)
        {
          mm_as_id = mm_ps_stack_id;
        }
      }
#endif
        break;

    case MMCM_RPM_UPDATE_FTAI_LIST:
      mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_update_ftai_list.as_id;
      mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
      if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
      {
        if(MM_SUB_IS_SGLTE)
        {
          mm_as_id = mm_ps_stack_id;
        }
      }
#endif
      break;
#endif

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
    case MMCM_LTE_SUSPEND_REQ:
        mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_lte_suspend_req.as_id;
        mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
        if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
        {
          if(MM_SUB_IS_SGLTE)
          {
            mm_as_id = mm_ps_stack_id;
          }
        }
#endif
        break;
    case MMCM_LTE_RESUME_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_lte_resume_ind.as_id;
        mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
        if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
        {
          if(MM_SUB_IS_SGLTE)
          {
            mm_as_id = mm_ps_stack_id;
          }
        }
#endif
        break;
#endif

    case MMCM_MMTEL_CALL_INFO:
        mm_sub_id = (mm_as_id_e_type)message->cmd.cm_mm_mmtel_call_info.as_id;
        mm_as_id = mm_sub_id;
#ifdef FEATURE_SGLTE
        if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
        {
          if(MM_SUB_IS_SGLTE)
          {
            mm_as_id = mm_ps_stack_id;
          }
        }
 #endif
        break;

      default:
        MSG_FATAL_DS( MM_SUB, "=MM= Unexpected CM cmd %d", message->cmd.hdr.message_id, 0,0 );
        break;
    }
    break;

#endif /* #ifdef FEATURE_GSM_GPRS */
  case MS_WMS_MM :
#ifdef FEATURE_DUAL_SIM
    switch (message->cmd.hdr.message_id)
    {
      case MM_SMS_BEARER_SELECTION_CMD:
        mm_sub_id = (mm_as_id_e_type)message->cmd.sms_bearer_selection_req.as_id;
        break;
       case MM_SMS_STATUS_CMD:
         mm_sub_id = (mm_as_id_e_type)message->cmd.wms_mm_sms_call_info.as_id;
         break;
      default:
        MSG_FATAL_DS( MM_SUB, "=MM= Unexpected WMS cmd %d", message->cmd.hdr.message_id, 0,0 );
        break;
    }
#endif
    mm_as_id = mm_sub_id;
   #ifdef FEATURE_SGLTE
    if(!IS_NOT_VALID_SUB_ID(mm_sub_id))
    {
      if(MM_SUB_IS_SGLTE)
      {
        mm_as_id = mm_cs_stack_id;
      }
    }
#endif
    break;

  case MS_MM_MM:
    switch (message->cmd.hdr.message_id)
    {
#ifdef FEATURE_MODEM_CONFIG_REFRESH
      case MM_MCFG_REFRESH_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.mcfg_refresh_ind.as_id;
        break;
#endif
#ifdef FEATURE_QSH_EVENT_METRIC
      case MM_QSH_MESSAGE_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.qsh_message_ind.param.action_params.metric_cfg.subs_id;
        break;
#endif
      case MM_NON_ACQ_PARAM_CHANGE_IND:
        mm_sub_id = (mm_as_id_e_type)message->cmd.non_acq_param_change_ind.as_id;
        break;

      default:
        MSG_ERROR_DS_1( MM_SUB, "=MM= Unknown message = %d", 
                                            message->cmd.hdr.message_id);
        break;
    }
    mm_as_id = mm_sub_id;
    break;

  default:
    MSG_ERROR_DS( MM_SUB, "=MM= Unexpected message set %d", message->cmd.hdr.message_set, 0,0 );
    break;
  }

  if(IS_NOT_VALID_STACK_ID(mm_as_id) || 
      (IS_NOT_VALID_SUB_ID(mm_sub_id)))
  {
    MSG_ERROR_DS(MM_SUB, "=MM=Invalid AS ID:%d - Ignoring message MS: %d   MSG_ID: %d", 
            mm_as_id,message->cmd.hdr.message_set, message->cmd.hdr.message_id);
    status = FALSE;

    /* leave SUB ID with a valid value */
    mm_as_id =  (mm_as_id_e_type)SYS_MODEM_AS_ID_1;
    mm_sub_id = (mm_as_id_e_type)SYS_MODEM_AS_ID_1;
  }
  mm_message_as_id = (byte)(mm_as_id + 1);
  return status;

}
#endif



/*===========================================================================
FUNCTION mmcoord_process_mmr_eq_plmn_change_ind

DESCRIPTION
  This function sends Equivalent PLMN list change ind to access stratum

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmcoord_process_mmr_eq_plmn_change_ind(void)
{
#if defined(FEATURE_LTE)
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
#endif
  mm_send_rrc_eq_plmn_list_change_ind();
  mm_send_rr_eq_plmn_list_change_ind();
#if defined(FEATURE_LTE)
  if(!is_lte_mode())
  {
    emm_send_multimode_rrc_system_update_from_GW(emm_ctrl_data_ptr);
  }
  if(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
  {
    if(is_umts_mode())
    {
      mm_send_rrc_lte_reselection_status_ind();
    }
    else if (is_gsm_mode())
    {
      mm_send_rr_irat_allowed_status_ind();
    }
  } 
#endif
}


/*===========================================================================
FUNCTION mmcoord_perform_local_ps_detach

DESCRIPTION
  This function performs local ps detach and sends detach confirmation

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmcoord_perform_local_ps_detach(void)
{
  mm_stop_timer(TIMER_MM_PS_DETACH);
  mm_ps_detach_state = MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ;
  gmm_detach_type =GMM_GPRS_DETACH;
  gmm_handle_gprs_detach_low_level_failure();
  mm_send_mmr_ps_detach_cnf();
}


/*===========================================================================

FUNCTION mmcoord_process_mmr_ps_detach_req

DESCRIPTION
  This function processes MMR_PS_DETACH_REQ from REG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_mmr_ps_detach_req(mm_cmd_type *message)
{

#ifdef FEATURE_LTE
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);
#endif

  MSG_HIGH_DS_2(MM_SUB, "=MM= Received MMR_PS_DETACH_REQ detach_type %d, gmm state %d",
            message->cmd.mmr_ps_detach_req.detach_type,gmm_state);

  mm_ps_detach_state = MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ;
  mm_ps_detach_type = message->cmd.mmr_ps_detach_req.detach_type;
  mm_stop_timer(TIMER_POLICY_CHANGE_PENDING);
  if(mm_timer_status[TIMER_MM_PS_DETACH] != TIMER_ACTIVE)
  {
    mm_start_timer(TIMER_MM_PS_DETACH, DEFAULT_TIMEOUT);
  }

#ifdef FEATURE_LTE
  if( emm_ctrl_data_ptr->l2gcco_state == EMM_LTOG_CCO_IN_PROGRESS
#ifdef FEATURE_SGLTE
     && MM_IS_SGLTE_ON_PS_STACK_ID
#endif    
    )
  {
    MSG_HIGH_DS_0(MM_SUB,"=MM= Received PS DETACH request during CCO, set CCO state to aborted");
    emm_ctrl_data_ptr->l2gcco_state = EMM_LTOG_CCO_ABORTED;
    mm_csfb_rej_pending_call(message);
  }
#endif

  mm_send_reg_attach_failed_ind_if_needed(); 
  gmm_foreground_search_pending = FALSE;

  if((mm_ps_detach_type == SYS_PS_DETACH_TYPE_LOCAL_DETACH) &&
      (gmm_state != GMM_NULL))
  {
    //Ignore pending REG request.
    mmcoord_reg_req_pending = FALSE;
    mm_reg_waiting_for_reg_cnf = FALSE;
    mm_serv_change_pending = FALSE;
    mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
    mmcoord_perform_local_ps_detach();
    mm_ps_detach_state = MM_PS_DETACH_STATE_NONE;
    if(((ps_session_status == MMRRC_EST_IN_PROGRESS) || (ps_session_status == MMRRC_ACTIVE))&&
       ((rrc_connection_status == MMRRC_ACTIVE) || (rrc_connection_status == MMRRC_EST_IN_PROGRESS)))
    {
      mm_abort_rr (RRC_PS_DOMAIN_CN_ID, TRUE);
    }
    return;
  }

  switch(gmm_state)
  {
    case GMM_NULL:
    case GMM_DEREGISTERED:

#ifdef FEATURE_WRLF_SYSTEM_SEL
      if((mm_wcdma_rlf_state == WCDMA_RLF_IDLE) 
            || (mm_received_suitable_search_end_ind == TRUE ))
#endif
      {
	    if((mm_waiting_for_stop_mode_cnf == TRUE)
            || (mm_rat_handler_status == MM_RAT_STOP_GSM_IN_PROGRESS) 
            || (mm_rat_handler_status == MM_RAT_STOP_WCDMA_IN_PROGRESS)
            || (mm_rat_handler_status == MM_RAT_STOP_TDSCDMA_IN_PROGRESS)
          )  
	    {
	      mm_ps_detach_state = MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF;
	      mmcoord_reg_req_pending = FALSE;
	      MSG_HIGH_DS_0(MM_SUB,"=MM= Received PS DETACH request so ignoring previously pended MMR_REG request");
	    }
        else
        {
          mm_stop_timer(TIMER_MM_PS_DETACH);
          if (gmm_gprs_attach_pending)
          {
            gmm_set_substate(GMM_ATTACH_NEEDED);
          }
          gmm_gprs_attach_pending = FALSE;
          mm_send_mmr_ps_detach_cnf();
          mm_ps_detach_state = MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ;
        }
      }
#ifdef FEATURE_WRLF_SYSTEM_SEL	
      else
      {
        mm_reg_waiting_for_reg_cnf = FALSE;

        if(( mm_pending_mmr_ps_detach_req_ptr == NULL ) && 
           ((mm_pending_mmr_ps_detach_req_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) 
                    == NULL))
        {
          mm_check_for_null_ptr((void*)mm_pending_mmr_ps_detach_req_ptr);
        }
        memscpy( mm_pending_mmr_ps_detach_req_ptr, sizeof(mm_cmd_type), message, sizeof(mm_cmd_type));

        mm_ps_detach_state = MM_PS_DETACH_STATE_DETACH_REQ_PENDING;

        mm_clear_pending_attach_req( );
      }
#endif	
      break;
  
    case GMM_REGISTERED_INITIATED:
    case GMM_ROUTING_AREA_UPDATING_INITIATED:
    case GMM_REGISTERED:
    case GMM_SERVICE_REQUEST_INITIATED:
      if((mm_deep_sleep_alt_scan && mm_hard_abort_pending) ||
         (mm_waiting_for_stop_mode_cnf)||
         (mm_waiting_for_abort_cnf == TRUE)
          || (mm_rat_handler_status == MM_RAT_STOP_GSM_IN_PROGRESS) 
          || (mm_rat_handler_status == MM_RAT_STOP_WCDMA_IN_PROGRESS)
          || (mm_rat_handler_status == MM_RAT_STOP_TDSCDMA_IN_PROGRESS)
         )  
      {
#ifdef FEATURE_WRLF_SYSTEM_SEL	      
        if ((mm_waiting_for_abort_cnf == TRUE) 

        && (mm_wcdma_rlf_state != WCDMA_RLF_IDLE) 
        && (mm_received_suitable_search_end_ind == TRUE )
           )
        {
          mmcoord_perform_local_ps_detach();
          if (mm_pending_mmr_ps_detach_req_ptr != NULL) 
          {

            modem_mem_free(mm_pending_mmr_ps_detach_req_ptr,MODEM_MEM_CLIENT_NAS);
            mm_pending_mmr_ps_detach_req_ptr = NULL;
          }
        }
        else
#endif		
        {
          //Ignore pending REG request.
          mmcoord_reg_req_pending = FALSE;
          mm_reg_waiting_for_reg_cnf = FALSE;
          mm_serv_change_pending = FALSE;
          MSG_HIGH_DS_3(MM_SUB, "=MM= Pend PS detach mm_deep_sleep_alt_scan %d or mm_waiting_for_stop_mode_cnf %d or mm_waiting_for_abort_cnf %d",
            mm_deep_sleep_alt_scan,mm_waiting_for_stop_mode_cnf,mm_waiting_for_abort_cnf );
          if(( mm_pending_mmr_ps_detach_req_ptr == NULL ) && 
           ((mm_pending_mmr_ps_detach_req_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) 
                    == NULL))
          {
            mm_check_for_null_ptr((void*)mm_pending_mmr_ps_detach_req_ptr);
          }
          memscpy( mm_pending_mmr_ps_detach_req_ptr, sizeof(mm_cmd_type), message, sizeof(mm_cmd_type));
          mm_ps_detach_state = MM_PS_DETACH_STATE_DETACH_REQ_PENDING;
          mm_clear_pending_attach_req( );
        }
      }
      else
      {
#ifdef FEATURE_SGLTE
        if (MM_IS_IN_SGLTE_MODE &&
            (gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED))
        {
          mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
          mmcoord_perform_local_ps_detach();
        }
        else
#endif
#if defined(FEATURE_LTE) && defined(FEATURE_DUAL_DATA)
        if((emm_ctrl_data_ptr->reg_req_pending_ptr != NULL) &&
           (emm_ctrl_data_ptr->reg_req_pending_ptr->volte_only == TRUE))
        {
          mmcoord_perform_local_ps_detach();
        }
        else
#endif
        if ((gmm_state == GMM_REGISTERED) &&
            (gmm_substate == GMM_ATTEMPTING_TO_UPDATE))
        {
          //Ignore pending REG request.
          mmcoord_reg_req_pending = FALSE;
          mm_reg_waiting_for_reg_cnf = FALSE;
          mm_serv_change_pending = FALSE;
          memscpy(&mmcoord_pending_reg_message, sizeof(mm_cmd_type),message,sizeof(mm_cmd_type));
          if (gmm_processing_directed_signalling_reestablishment)
          {
            mm_ps_detach_state = MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF;
#if defined (FEATURE_GSM) || defined (FEATURE_WCDMA)
            gmm_initiate_gprs_detach(message);
#endif /*#if defined (FEATURE_GSM) || defined (FEATURE_WCDMA)*/
          }
          else
          {
#ifdef FEATURE_WRLF_SYSTEM_SEL
            if((mm_wcdma_rlf_state == WCDMA_RLF_IDLE) 
                   || (mm_received_suitable_search_end_ind == TRUE ))
#endif
            {
              mmsend_nas_reg_event_packet(PROC_NAS_MO_DETACH_EVENT,PROC_END,PROC_END_LOWER_LAYER_FAILURE,MT_DETACH_TYPE_NONE,MT_DETACH_CAUSE_NONE);
              mmcoord_perform_local_ps_detach();
            }
#ifdef FEATURE_LTE
            else
            {
              if(( mm_pending_mmr_ps_detach_req_ptr == NULL ) && 
                  ((mm_pending_mmr_ps_detach_req_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) == NULL))
              {
                mm_check_for_null_ptr((void*)mm_pending_mmr_ps_detach_req_ptr);
              }
              memscpy( mm_pending_mmr_ps_detach_req_ptr, sizeof(mm_cmd_type), message, sizeof(mm_cmd_type));
              mm_ps_detach_state = MM_PS_DETACH_STATE_DETACH_REQ_PENDING;
              mm_clear_pending_attach_req( );
            }
#endif
          }
        }
        else
        {
          //Ignore pending REG request.
          mmcoord_reg_req_pending = FALSE;
          mm_reg_waiting_for_reg_cnf = FALSE;
          mm_serv_change_pending = FALSE;  
          memscpy(&mmcoord_pending_reg_message, sizeof(mm_cmd_type),message,sizeof(mm_cmd_type));
          mm_ps_detach_state = MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF;
#if defined (FEATURE_GSM) || defined (FEATURE_WCDMA)
#ifdef FEATURE_WRLF_SYSTEM_SEL
          if((mm_wcdma_rlf_state == WCDMA_RLF_IDLE) 
                 || (mm_received_suitable_search_end_ind == TRUE ))
#endif
          {
            gmm_initiate_gprs_detach(message);
          }
#ifdef FEATURE_WRLF_SYSTEM_SEL
          else
          {
            if(( mm_pending_mmr_ps_detach_req_ptr == NULL ) && 
              ((mm_pending_mmr_ps_detach_req_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) 
                      == NULL))
            {
              mm_check_for_null_ptr((void*)mm_pending_mmr_ps_detach_req_ptr);
            }
            memscpy( mm_pending_mmr_ps_detach_req_ptr, sizeof(mm_cmd_type), message, sizeof(mm_cmd_type));
            mm_ps_detach_state = MM_PS_DETACH_STATE_DETACH_REQ_PENDING;
            mm_clear_pending_attach_req( );
          }
#endif
          if( (mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF) &&
		  	  !(gmm_state == GMM_DEREGISTERED_INITIATED || gmm_state == GMM_DEREGISTERED || gmm_state == GMM_NULL))
          {
            if(( mm_pending_mmr_ps_detach_req_ptr == NULL ) && 
              ((mm_pending_mmr_ps_detach_req_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) 
                      == NULL))
            {
              mm_check_for_null_ptr((void*)mm_pending_mmr_ps_detach_req_ptr);
            }
            memscpy( mm_pending_mmr_ps_detach_req_ptr, sizeof(mm_cmd_type), message, sizeof(mm_cmd_type));
            mm_ps_detach_state = MM_PS_DETACH_STATE_DETACH_REQ_PENDING;
          }

#endif
        }
      }
      break;
  
    case GMM_DEREGISTERED_INITIATED:
      mm_ps_detach_state = MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF;
      break;
  }
}

  
/*===========================================================================
  
  FUNCTION mmcoord_process_mmr_high_priority_activity_ind
  
  DESCRIPTION
      This function processes MMR_HIGH_PRIORITY_ACTIVITY_IND from REG
  
  DEPENDENCIES
      None
  
  RETURN VALUE
      None
  
  SIDE EFFECTS
      None
  
  ===========================================================================*/
 void mmcoord_process_mmr_high_priority_activity_ind(mm_cmd_type *message)
 {
  
    boolean      gmm_ps_dereg = message->cmd.mmr_high_priority_activity_ind.local_detach;

#if defined(FEATURE_LTE)
    emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
    emm_ctrl_data_ptr = emm_db_get_ctrl_data();
    ASSERT(emm_ctrl_data_ptr != NULL);
#endif

    if(message->cmd.hdr.message_id == (int) MMR_HIGH_PRIORITY_ACTIVITY_IND)
    {
      MSG_HIGH_DS_1(MM_SUB,"=MM= MM received MMR_HIGH_PRIORITY_ACTIVITY_IND local_detach: %d ", gmm_ps_dereg );

        if (gmm_ps_dereg &&
            (((is_umts_mode() || is_gsm_mode()) &&
               (mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF ||
                mm_ps_detach_state == MM_PS_DETACH_STATE_DETACH_REQ_PENDING) &&
               (gmm_state != GMM_NULL) &&
               (gmm_state != GMM_DEREGISTERED)) 
#if defined(FEATURE_LTE) 
            ||
              ((mm_serving_plmn.info.active_rat == SYS_RAT_LTE_RADIO_ACCESS) &&
               (emm_ctrl_data_ptr->ps_detach_info.status != EMM_PS_DETACH_NONE) &&
               (EMM_GET_STATE() != EMM_NULL) &&
               (EMM_GET_STATE() != EMM_DEREGISTERED))
#endif
              )
            )
        {
          MSG_HIGH_DS_0(MM_SUB, "=MM= User action: CM Waiting for Detach completion ");
          gmm_abort_ps_detach = TRUE;
        }
        else
        {
          gmm_abort_ps_detach = FALSE;
        }
 
    }
 }


#ifdef FEATURE_LTE
/*===========================================================================

FUNCTION mmcoord_process_mmr_get_drx_req

DESCRIPTION
  This function processes MMR_GET_DRX_REQ from REG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_mmr_set_drx_req(mm_cmd_type *message)
{
  if (message->cmd.mmr_reg_req.message_header.message_id == (int) MMR_SET_DRX_REQ)
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= MM received MMR_SET_DRX_REQ %d",  message->cmd.mmr_set_drx_req.drx_coefficient);

    gmm_handle_message(message);
  }
  else
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Unexpected REG msg %d", message->cmd.hdr.message_id);
  }
}
#endif /*FEATURE_LTE*/

#ifdef FEATURE_FEMTO_CSG
/*===========================================================================

FUNCTION mmcoord_process_csg_select_config_update_ind

DESCRIPTION
  This function processes MMR_CSG_SELECT_CONFIG_UPDATE_IND from REG

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_csg_select_config_update_ind(mm_cmd_type *message)
{
  if(message->cmd.hdr.message_id == (int) MMR_CSG_SELECT_CONFIG_UPDATE_IND)
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Received MMR_CSG_SELECT_CONFIG_UPDATE_IND - %d",
                message->cmd.mmr_csg_sel_cfg_update_ind.csg_selection_config);

    mm_csg_selection_config = 
           message->cmd.mmr_csg_sel_cfg_update_ind.csg_selection_config;
  }
  else
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Unexpected REG msg %d", 
                         message->cmd.hdr.message_id);
  }
}
#endif

#ifdef FEATURE_DUAL_SIM
/*===========================================================================

FUNCTION mmcoord_process_mmr_data_prioirty_indication

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_mmr_data_prioirty_indication(mm_cmd_type *message)
{
  if(message->cmd.hdr.message_id == (int) MMR_DATA_PRIORITY_IND)
  {
    mm_ds_proc_type = message->cmd.mmr_data_prio_ind.proc_type;
    mm_send_procedure_ind(mm_evaluate_proc_type());
    MSG_HIGH_DS_1(MM_SUB,"=MM= MM received MMR_DATA_PRIORITY_IND proc type %d",mm_ds_proc_type);
  }
  else
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Unexpected REG msg %d", 
                         message->cmd.hdr.message_id);
  }  
}
#endif 

#ifdef FEATURE_CMCC_SECURE_CALL
void mmcoord_process_mmr_secure_call_ind(mm_cmd_type *message)   
{
  if(message->cmd.hdr.message_id == (int) MMR_SECURE_CALL_IND)
  {
    MSG_HIGH_DS_1(MM_SUB,"=MM= MM received MMR_SECURE_CALL_IND type = %d", message->cmd.mmr_secure_call_ind.type);
    if (mm_serving_plmn.info.active_rat == SYS_RAT_TDS_RADIO_ACCESS)
    {
        if (( message->cmd.mmr_secure_call_ind.type == SYS_SECURE_CALL_START) && 
          (mm_emm_secure_status_type != SYS_SECURE_CALL_START))
        {
          mm_send_rrc_secure_call_ind(RRC_SECURE_VOICE_ENABLE);
        }
        else if (( message->cmd.mmr_secure_call_ind.type == SYS_SECURE_CALL_END) && 
                 (mm_emm_secure_status_type == SYS_SECURE_CALL_START))
        {
          mm_send_rrc_secure_call_ind(RRC_SECURE_VOICE_NULL);
        }
    }
    mm_emm_secure_status_type = message->cmd.mmr_secure_call_ind.type;
  }
  else
  {
   MSG_HIGH_DS_1(MM_SUB ,"=MM= Unexpected REG msg %d", 
                        message->cmd.hdr.message_id);
  }

}
#endif

/*===========================================================================

FUNCTION MMCOORD_ROUTE_MM_MESSAGE

DESCRIPTION
  This function routes the given message to the MM entity and/or
  the GMM entity.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_route_mm_message( mm_cmd_type *message,   mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr)
{
#if defined(FEATURE_LTE)

  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;

  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  ASSERT(emm_ctrl_data_ptr != NULL);

#endif
#if defined(FEATURE_DUAL_SIM) && !defined(FEATURE_LTE)
  if (!mm_get_as_id(message))
  {
    mm_update_debug_buffer();
    return;
  }
  mm_update_debug_buffer();
#endif

#ifndef FEATURE_LTE
  if((message->cmd.hdr.message_set == MS_CC_MM ||
       message->cmd.hdr.message_set == MS_CC_MM_PS) &&
       (message->cmd.hdr.message_id == MMCNM_DATA_REQ))
  {
     mm_set_mm_controls_resel_dec_mt_cs_call(message);
  }
#endif

  mm_rat_handle_new_msg(message);
  switch (message->cmd.hdr.message_set)
  {
#ifdef FEATURE_GSM
    case MS_MM_RR:
      if (mmcoord_valid_rr_command( message ))
      {
        /* ------------------------------------------------
        ** Handle RR connection establishment and tear down
        ** ------------------------------------------------ */
        coord_rrc_conn_management( message );

        switch (message->cmd.hdr.message_id)
        {
          case RR_PLMN_SELECT_CNF:
#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
          if( mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE)
          {
             MSG_HIGH_DS_0(MM_SUB,"WRLF Stop mode confirmation is pending ignoring the PLMN SELECT CNF ");  
          }
          else
#endif
          {
            mmcoord_process_rr_plmn_select_cnf( message );
#ifdef FEATURE_GSM_GPRS
            gmmllc_handle_grr_event( message );
#endif /* #ifdef FEATURE_GSM_GPRS */
          }
            break;

          case RR_SERVICE_IND:
            mmcoord_process_rr_service_ind( message );
            break;

#ifdef FEATURE_GSM_DTM
          case RR_SERVICE_IND_FROM_OTA_MSG:
            mmcoord_process_rr_service_ind_from_ota_msg( message );
            break;
#endif /* #ifdef FEATURE_GSM_DTM */

        case RR_ACTIVATION_IND:
#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
          if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE)
          {
            MSG_HIGH_DS_1(MM_SUB ,"=MM= Ignoring RR_ACTIVATION_IND message rlf state %d", mm_wcdma_rlf_state);
          }
          else
#endif
          {
          /* MNK*/
#ifdef FEATURE_LTE
            if(is_lte_mode())/*Check to see if the current RAT is LTE*/
            {
#ifdef FEATURE_SGLTE
              if(mm_sglte_activation_ind_allowed() == FALSE)
              {
                MSG_HIGH_DS_0(MM_SUB,"=MM= SGLTE mode - ACTIVATION_IND not allowed");
              }
              else
#endif
              if(emm_ctrl_data_ptr->ignore_GW_activation_ind == FALSE)
              {
                MSG_HIGH_DS_0(MM_SUB,"=MM= MM received RR_ACTIVATION_IND activating G, enter i-RAT from LTE to G");
                mm_emm_process_LTE_to_GW_rat_change(message, mm_emm_dispatcher_fn_ptr, emm_ctrl_data_ptr);
              }
              else
              {
                MSG_HIGH_DS_0(MM_SUB,"=MM= Ignoring RR_ACTIVATION_IND as ignore_GW_activation_ind is TRUE");
              }
            }
            else
            {
#endif
              mm_handle_message( message );
#ifdef FEATURE_LTE
            }
#endif
          }
          break;

          case RR_EST_CNF:
#ifdef FEATURE_DUAL_SIM
          case RR_DS_DATA_IND:
#else
          case RR_DATA_IND:
#endif
          case RR_PLMN_LIST_CNF:
            mm_handle_message( message );
            break;
          case RR_STOP_GSM_MODE_CNF:
#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
             if( mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE)
             {
               MSG_HIGH_DS_0(MM_SUB,"WRLF Stop mode confirmation is pending ignoring the STOP_MODE_CNF ");  
             }
             else
#endif
             {
               mm_handle_message( message );
             }
             break;            
#ifdef FEATURE_INTERRAT_PCCO_GTOW
          case RR_GTOW_CCO_COMPLETE_IND:
            mm_handle_message( message );
            break;          
#endif /* FEATURE_INTERRAT_PCCO_GTOW */

          case RR_REL_IND:
            mm_handle_message( message ) ;

#ifdef FEATURE_GSM_GPRS
            gmm_handle_message( message );
#endif /* #ifdef FEATURE_GSM_GPRS */
            if (mmcoord_reg_req_pending)            
            {
              mmcoord_reg_req_pending = FALSE;            
              if (mmcoord_pending_reg_message.cmd.hdr.message_id == (int) MMR_REG_REQ)
              {
                mmcoord_process_mmr_reg_req( &mmcoord_pending_reg_message );
              }
            }

            break;

          case RR_DEACT_CNF:
            mm_handle_message( message );
#ifdef FEATURE_GSM_GPRS
            gmm_handle_message( message );
#endif /* #ifdef FEATURE_GSM_GPRS */

            break;   

          case RR_ABORT_IND:
            mm_handle_message( message );
#ifdef FEATURE_GSM_GPRS
            gmm_handle_message( message );
#endif /* #ifdef FEATURE_GSM_GPRS */

            if (mmcoord_reg_req_pending)            
            {
               mmcoord_reg_req_pending = FALSE;            
               if (mmcoord_pending_reg_message.cmd.hdr.message_id == (int) MMR_REG_REQ)
               {
                  mmcoord_process_mmr_reg_req( &mmcoord_pending_reg_message );
               }
            }

            break;

#ifdef FEATURE_DUAL_SIM
          case RR_PREEMPTION_IND:
            mm_handle_message( message );
            break;
#endif 
          case RR_ABORT_CNF:
            mm_handle_message( message );
#ifdef FEATURE_GSM_GPRS
            gmm_handle_message( message );
#endif /* #ifdef FEATURE_GSM_GPRS */
            if (mmcoord_reg_req_pending)
            {
              mmcoord_reg_req_pending = FALSE; 
              if(mm_timer_status[TIMER_T3211] == TIMER_ACTIVE)
              {
                MSG_HIGH_DS_0(MM_SUB,"=MM= Restarting Timer T3211 with smaller value, as LU failed due to upper layer activity ");
                mm_start_timer(TIMER_T3211, 500);
              }			  
              if (mmcoord_pending_reg_message.cmd.hdr.message_id == (int) MMR_REG_REQ)
              {
                mmcoord_process_mmr_reg_req( &mmcoord_pending_reg_message );
              }
            }
            break;

          case RR_SYNC_IND:
#ifdef FEATURE_GSM_GPRS
            if (is_gsm_mode())
            {
              if ((message->cmd.rr_sync_ind.sync_reason == MM_RR_HANDOVER) ||
                 (message->cmd.rr_sync_ind.sync_reason == MM_RR_WCDMA_GSM_HO))
              {
                gmm_handle_message( message ) ;
                mm_handle_message( message );
              }
              else
              {
                mm_handle_message( message );
              }
            }
            else
#endif
            {
              mm_handle_message( message );
            }
            break;

          case RR_PAGING_IND:
#ifdef FEATURE_GSM_GPRS
            if (is_gsm_mode())
            {
              if ((message->cmd.rr_paging_ind.paging_type == CS_PAGING)||
                  (message->cmd.rr_paging_ind.paging_type == CS_PAGING_WITH_IMSI))
              {
                mm_handle_message( message ) ;
              }
              else
              {
                if(cm_get_ui_ready_status() == TRUE)
                {
                  gmm_handle_message( message ) ;
                }
                else
                {
                  MSG_HIGH_DS_0(MM_SUB,"=MM= UI is not ready ignore PS page") ;
                }
              }
            }
            else
#endif
            {
              mm_handle_message( message );
            }
            break;
#if defined (FEATURE_GSM_EDTM) || defined (FEATURE_GAN)
          case RR_EST_IND:
            if (is_gsm_mode())
            {
              mm_handle_message(message);
            }
            else
            {
              MSG_ERROR_DS(MM_SUB, "=MM= RR command unexpected in UMTS mode",0,0,0);
            }
            break;
#endif
#ifdef FEATURE_GSM_GPRS
#ifdef FEATURE_DUAL_SIM
          case RR_PS_ABORT_CNF:
#endif
          case RR_GMM_GPRS_SUSPENSION:
          case RR_GMM_GPRS_RESUMPTION_IND:
          case RR_RESELECTION_IND:
#ifdef FEATURE_NAS_FREE_FLOAT
          case RR_GMM_GPRS_SERVICE_CNF:
#else
          case RR_GPRS_PCCCH_CAMP_CNF:
#endif
          case RR_GMM_GPRS_MULTISLOT_CLASS_IND:     
          case RR_GMM_GPRS_MS_RA_CAP_REQ:
          case RR_GMM_RETX_PDU_TUNEAWAY_FAIL_IND:
            gmm_handle_message( message );
            break;
#endif
#ifdef FEATURE_GPRS_PS_HANDOVER
          case RR_PSHO_IND:
            mmcoord_process_rr_psho_ind( message );
            break;
#endif
#ifdef FEATURE_LTE
          case RR_SRVCC_HANDOVER_FAILURE_IND:
            mm_send_cm_mm_srvcc_handover_failure_ind();
            break;
#endif
 #ifdef FEATURE_DUAL_SIM
          case RR_MULTIMODE_SUBS_CHGD_CNF:
            MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received RR_MULTIMODE_SUBS_CHGD_CNF");
            mm_send_mmr_multimode_subs_chgd_cnf();
            break;
#endif
          case RR_PS_STATUS_CHANGE_IND:
            MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received RR_PS_STATUS_CHANGE_IND");
            mmcoord_process_rr_ps_status_change_ind(message);      
            break;
         case RR_PLMN_RESEL_FAIL_IND:
           
            mmcoord_process_rr_plmn_resel_fail_ind(message);      
            break;
          default:
            MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RR cmd %d", message->cmd.hdr.message_id, 0,0 );
        }
      }
      break;
#endif /*FEATURE_GSM*/      

#if defined ( FEATURE_MMGPS ) || defined ( FEATURE_CGPS_UMTS_CP )
    case MS_MM_LSM:
        switch (message->cmd.hdr.message_id)
        {
          case LCS_ACTIVE_IND:
            mm_handle_message( message );
            break;

          default:
            MSG_FATAL_DS( MM_SUB, "=MM= Unexpected LSM cmd %d", message->cmd.hdr.message_id, 0,0 );
        }
       break;
#endif /* end of (FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP) */
#ifdef FEATURE_WCDMA
    case MS_MM_RRC:
    case MS_MM_TDSRRC:
      if (mmcoord_valid_rrc_command( message ))
      {
        /* ---------------------------------------------------------
        ** Handle RRC connection/session establishment and tear down
        ** --------------------------------------------------------- */
        coord_rrc_conn_management( message );

        switch (message->cmd.hdr.message_id)
        {
          case RRC_EST_CNF:
            mmcoord_process_rrc_est_cnf( message );
            break;

          case RRC_REL_IND:
#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
            if( mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE )
            {
             MSG_HIGH_DS_0(MM_SUB,"WRLF Stop mode confirmation is pending ignoring the REL IND ");
            }
            else
#endif
            {
              mmcoord_process_rrc_rel_ind( message );
			  
              if (mmcoord_reg_req_pending)           
              {
#ifdef FEATURE_LTE		      
                if(emm_ctrl_data_ptr->rrc_deact_req.deact_cnf_pending == TRUE)
                {
                   MSG_HIGH_DS_0(MM_SUB,"=MM= Pended Reg Req to be processed after LTE stack deact ");
                }
                else
#endif			
                {
                  mmcoord_reg_req_pending = FALSE;            
                  if (mmcoord_pending_reg_message.cmd.hdr.message_id == (int) MMR_REG_REQ)
                  {
                    mmcoord_process_mmr_reg_req( &mmcoord_pending_reg_message );
                  }
                }
              }

			  
      		  if(mm_ps_detach_state == MM_PS_DETACH_STATE_DETACH_REQ_PENDING)
			  {
                            /* If MM is waiting for abort confirmation, then clear that variable as connection released*/
			    mm_waiting_for_abort_cnf = FALSE;
				if(mm_pending_mmr_ps_detach_req_ptr != NULL)
				{
				  mmcoord_process_mmr_ps_detach_req(mm_pending_mmr_ps_detach_req_ptr);
				  if(mm_ps_detach_state != MM_PS_DETACH_STATE_DETACH_REQ_PENDING)
    			  {
				    modem_mem_free(mm_pending_mmr_ps_detach_req_ptr,MODEM_MEM_CLIENT_NAS);
							  mm_pending_mmr_ps_detach_req_ptr = NULL;
                  }
                }
			  }
              
            }
            break;

          case RRC_SYNC_IND:
            mmcoord_process_rrc_sync_ind( message );
            break;

          case RRC_SERVICE_CNF:
#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
            if( mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE )
            {
             MSG_HIGH_DS_0(MM_SUB,"WRLF Stop mode confirmation is pending ignoring the RRC SERVICE CNF ");
            }
            else
#endif
            {
              mmcoord_process_rrc_service_cnf( message );
            }
            break;

          case RRC_SERVICE_IND:
            mmcoord_process_rrc_service_ind( message );
            break;

          case RRC_SERVICE_IND_FROM_OTA_MSG:
            mmcoord_process_rrc_service_ind_from_ota_msg( message );
            break;

          case RRC_PLMN_LIST_CNF:
            mmcoord_process_rrc_plmn_list_cnf (message );
            break;

          case RRC_ABORT_IND:
            mmcoord_process_rrc_abort_ind( message );
            break;

          case RRC_PAGE_IND:
            mmcoord_process_rrc_page_ind( message );
            break;

          case RRC_DATA_IND:
            mmcoord_process_rrc_data_ind( message );
            break;

          case RRC_OPEN_SESSION_CNF:
            mmcoord_process_rrc_open_session_cnf( message );
            break;

          case RRC_FAST_DORMANCY_CNF:
            gmm_handle_message( message );
            break;

          case RRC_CLOSE_SESSION_IND:
#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
            if( mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE )
            {
             MSG_HIGH_DS_0(MM_SUB,"WRLF Stop mode confirmation is pending ignoring the CLOSE SESSION IND ");
            }
            else
#endif
#ifdef FEATURE_LTE
           if ((is_lte_mode()) && 
               (emm_ctrl_data_ptr->rrc_deact_req.deact_cnf_pending == TRUE))
           {
              MSG_HIGH_DS_0(MM_SUB,"=MM= Pended Reg Request to be processed after LTE stack deact ");
           }
           else
#endif
            {
              mmcoord_process_rrc_close_session_ind( message );
            
              if ((mmcoord_reg_req_pending) &&
                  (message->cmd.rrc_close_session_ind.cn_domain_id == RRC_CS_DOMAIN_CN_ID))            
              {
                mmcoord_reg_req_pending = FALSE;            
                if (mmcoord_pending_reg_message.cmd.hdr.message_id == (int) MMR_REG_REQ)
                {
                  mmcoord_process_mmr_reg_req( &mmcoord_pending_reg_message );
                }
              }
            }
            break;

          case RRC_CLOSE_SESSION_CNF:
#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
            if( mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE )
            {
              MSG_HIGH_DS_0(MM_SUB,"WRLF Stop mode confirmation is pending ignoring the CLOSE SESSION CNF ");
            }
            else
#endif
#ifdef FEATURE_LTE
            if ((is_lte_mode()) && 
                (emm_ctrl_data_ptr->rrc_deact_req.deact_cnf_pending == TRUE))
            {
               MSG_HIGH_DS_0(MM_SUB,"=MM= Pended Reg Request to be processed after LTE stack deact ");
            }
            else
#endif
            {
              mmcoord_process_rrc_close_session_cnf( message );
            
              if ((mmcoord_reg_req_pending) &&
                  (message->cmd.rrc_close_session_ind.cn_domain_id == RRC_CS_DOMAIN_CN_ID))            
              {
                mmcoord_reg_req_pending = FALSE;            
                if (mmcoord_pending_reg_message.cmd.hdr.message_id == (int) MMR_REG_REQ)
                {
                  mmcoord_process_mmr_reg_req( &mmcoord_pending_reg_message );
                }
              }
            } 
            break;

          case RRC_ABORT_CNF:
            mmcoord_process_rrc_abort_cnf( message );

#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
            if(mm_rlf_stop_mode_pending == MM_WCDMA_RLF_STOP_MODE_PENDING)
            {
              mm_rlf_stop_mode_pending = MM_RLF_STOP_MODE_PENDING_NONE;
              mmcoord_route_mm_message(&mmcoord_pending_reg_message,mm_emm_dispatcher_fn_ptr);
            }
#endif
            if ((mmcoord_reg_req_pending) 
                && 
                (message->cmd.rrc_abort_cnf.conn_rel_cause != RRC_UE_INIT_DORMANCY_UE_IN_OOS) &&
                (message->cmd.rrc_abort_cnf.conn_rel_cause != RRC_UE_INIT_DORMANCY_ABORT_SUCCESS)
#ifdef FEATURE_DUAL_SIM
                && (message->cmd.rrc_abort_cnf.conn_rel_cause != RRC_REL_DUAL_STANDBY_PS_ABORT_SUCCESS)
#endif

#ifdef FEATURE_LTE
                 /*
                         **Do not process pending GW REG request on abort confirmation,
                         **wait for LTE deactivation to complete to process the pending REG request.
                 */

                   &&(!((emm_ctrl_data_ptr->rrc_deact_req.deact_cnf_pending == TRUE) ||
                        (emm_ctrl_data_ptr->rrc_deact_req.deact_req_pending == TRUE) 
                          || emm_ctrl_data_ptr->rrc_active == TRUE))
                
#endif
               )
            {
              mmcoord_reg_req_pending = FALSE;  
              if(mm_timer_status[TIMER_T3211] == TIMER_ACTIVE)
              {
                MSG_HIGH_DS_0(MM_SUB,"=MM= Restarting Timer T3211 with smaller value, as LU failed due to upper layer activity ");
                mm_start_timer(TIMER_T3211, 500);
              }
              if (mmcoord_pending_reg_message.cmd.hdr.message_id == (int) MMR_REG_REQ)
              {
                mmcoord_process_mmr_reg_req( &mmcoord_pending_reg_message );
              }
            }
            else if(mm_ps_detach_state == MM_PS_DETACH_STATE_DETACH_REQ_PENDING)
            {
              if(mm_pending_mmr_ps_detach_req_ptr != NULL)
              {
                mmcoord_process_mmr_ps_detach_req(mm_pending_mmr_ps_detach_req_ptr);
                if(mm_ps_detach_state != MM_PS_DETACH_STATE_DETACH_REQ_PENDING)
                {
                modem_mem_free(mm_pending_mmr_ps_detach_req_ptr,MODEM_MEM_CLIENT_NAS);
                mm_pending_mmr_ps_detach_req_ptr = NULL;
              }
              }
            }
            break;

          case RRC_DEACT_CNF:
          case RRC_MODE_CHANGE_CNF:
            mmcoord_process_rrc_deact_cnf( message );
            break;

          case RRC_STOP_WCDMA_MODE_CNF:
#if defined(FEATURE_WRLF_SYSTEM_SEL) && !defined (FEATURE_LTE)
            if( mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE )
            {
              if(mm_waiting_for_stop_mode_cnf)
              {
                mm_waiting_for_stop_mode_cnf = FALSE;
                mm_send_rrc_hard_abort_req( mm_wrlf_serving_plmn_ptr->info.active_rat );
                MSG_HIGH_DS_0(MM_SUB,"Send hard abort request for processing stop mode request");
              }
              else
              {
                MSG_HIGH_DS_0(MM_SUB,"WRLF Stop mode confirmation is pending ignoring the RRC STOP CNF ");
              }
            }
            else
#endif
            {
              mmcoord_process_rrc_stop_mode_cnf( message );
            }
            break;

        case RRC_ACTIVATION_IND:
          /* MNK*/
#ifdef FEATURE_LTE
         if(is_lte_mode())/*Check to see if the current RAT is LTE*/
         {
           if(emm_ctrl_data_ptr->ignore_GW_activation_ind == FALSE)
           {
             MSG_HIGH_DS_0(MM_SUB,"=MM= received RRC_ACTIVATION_IND activating, enter i-RAT from LTE to W/T");
             mm_emm_process_LTE_to_GW_rat_change(message, mm_emm_dispatcher_fn_ptr, emm_ctrl_data_ptr);
           }
           else
           {
             MSG_HIGH_DS_0(MM_SUB,"=MM= Ignoring RRC_ACTIVATION_IND as ignore_GW_activation_ind is TRUE");
           }
         }
         else
         {
#endif
           mm_handle_message ( message );
#ifdef FEATURE_LTE
         }
#endif
            break;

 #ifdef FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER
          case RRC_WTOG_CCO_COMPLETE_IND:
            mm_handle_message ( message );
            break;
 #endif /* FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER */

 
 #ifdef FEATURE_HSDPA
          case RRC_HSDPA_HSUPA_CELL_IND:
            mmcoord_process_rrc_hsdpa_hsupa_cell_ind(message);
            break;
            
#ifdef FEATURE_HSPA_CALL_STATUS_IND
          case RRC_HSPA_CALL_STATUS_IND:
            mmcoord_process_rrc_hspa_call_status_ind(message);
            break;
 #endif
 #endif
#ifdef FEATURE_OOSC_SERVICE_STATUS_UI_UPDATE
          case RRC_CONNECTED_MODE_SERVICE_STATUS_IND:
            mmcoord_process_rrc_connected_mode_service_status_ind(message);
            break;
#endif
 #ifdef FEATURE_MBMS
          #error code not present
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL
          case RRC_SUITABLE_SEARCH_END_IND:
            mm_send_rrc_suitable_search_end_ind();
            break;

          case RRC_DISABLE_BPLMN_IND:
            mm_send_rrc_disable_bplmn_ind(message->cmd.rrc_disable_bplmn_ind.disable_status);
            break;
#endif
#ifdef FEATURE_LTE
          case RRC_SRVCC_HANDOVER_FAILURE_IND:
            mm_send_cm_mm_srvcc_handover_failure_ind();
            break;
#endif
          case RRC_PCH_STATE_IND:
#ifdef FEATURE_LTE
            mmcoord_process_rrc_pch_state_ind(message);
#endif
            break;

          default:
            MSG_FATAL_DS( MM_SUB, "=MM= Unexpected RRC cmd %d", message->cmd.hdr.message_id, 0,0 );
        }
      }
      break;
#endif /*FEATURE_WCDMA*/      

    case MS_CC_MM:
    case MS_CC_MM_PS :
      /* --------------------
      ** All messages from CM
      ** -------------------- */
      mm_handle_message( message );
      break;

    case MS_MM_REG:
      if (mmcoord_valid_reg_command( message ))
      {
        switch (message->cmd.hdr.message_id)
        {
          case MMR_REG_REQ:
            mmcoord_process_mmr_reg_req( message );
            break;
          case MMR_GSTK_SERVICE_IND:
            mmcoord_process_mmr_gstk_service_ind( message );
            break;

          case MMR_EQ_PLMN_CHANGE_IND:
            mmcoord_process_mmr_eq_plmn_change_ind();
            break;

          case MMR_SIM_AVAILABLE_REQ:
            mmcoord_process_mmr_sim_available_req ( message );
            break;

          case MMR_SIM_NOT_AVAILABLE_REQ:
            mmcoord_process_mmr_sim_not_available_req ( message );
            break;

          case MMR_STOP_MODE_REQ:
            mmcoord_process_mmr_stop_mode_req( message );
            break;

          case MMR_PLMN_SEARCH_REQ:
            mmcoord_process_mmr_plmn_search_req( message );
            break;

          case MMR_PLMN_SEARCH_ABORT_REQ:
            if (mm_plmn_search_in_progress == TRUE)
            {
              MSG_HIGH_DS_0(MM_SUB,"=MM= MM received MMR_PLMN_SEARCH_ABORT_REQ");
              if (is_umts_mode())
              {
                mm_send_rrc_bplmn_search_abort_req();
              }
              else
              {
                mm_send_rr_bplmn_search_abort_req();
              }
            }
            else
            {
              MSG_HIGH_DS_0(MM_SUB,"=MM= MMR_PLMN_SEARCH_ABORT_REQ is not processed as PLMN search is not in progress");
            }
            break;

          case MMR_PH_STATUS_CHANGE_REQ:
            mmcoord_process_mmr_ph_status_change_req( message );
            break;
            
          case MMR_REFRESH_STAGE_IND:
            gmm_process_refresh_stage_ind( message );
            break; 

#ifdef FEATURE_OOSC_USER_ACTION
          case MMR_EXIT_FROM_PWR_SAVE:
            mmcoord_process_mmr_exit_from_pwr_save( message );
            break;
#endif
#ifdef FEATURE_DUAL_SIM
          case MMR_DS_STAT_CHGD_REQ:
            mmcoord_process_mmr_ds_stat_chgd_req( message );
            break;

#ifndef FEATURE_LTE
          case MMR_MULTIMODE_SUBS_CHGD_REQ:
            mmcoord_process_mmr_multimode_subs_chgd_req( message );
            break;
#endif
          case MMR_DDS_SWITCH_IND:
            mmcoord_process_mmr_dds_switch_ind( message );
            break;
#endif
#if defined(FEATURE_DUAL_SIM)
          case MMR_DS_TUNEAWAY_STATUS_CHANGE_IND:
#ifdef FEATURE_WCDMA
            MSG_HIGH_DS_0(MM_SUB,"=MM= MM received MMR_DS_TUNEAWAY_STATUS_CHANGE_IND");
            mm_send_rrc_ds_tuneaway_status_change_ind
            (
              message->cmd.mmr_ds_tuneaway_status_change_ind.tuneaway_status
            );
#else
            MSG_HIGH_DS_0(MM_SUB,"=MM= MM ignoring MMR_DS_TUNEAWAY_STATUS_CHANGE_IND");
#endif /* #ifdef FEATURE_WCDMA */
            break;
#endif

#ifdef FEATURE_NAS_ECALL 
          case MMR_ECALL_MODE_IND:
            mmcoord_process_mmr_ecall_mode_ind(message);
            break; 

          case MMR_PROFILE_TYPE_IND:
           mmcoord_process_mmr_profile_type_ind(message);
           break;
#endif

        case MMR_PS_DETACH_REQ:
          mmcoord_process_mmr_ps_detach_req(message);
          break; 

#ifdef FEATURE_LTE
          case MMR_SET_DRX_REQ:
            mmcoord_process_mmr_set_drx_req(message);
            break;

          case MMR_GET_DRX_REQ:
            mm_send_mmr_get_drx_cnf();
            break;
#endif
#ifdef FEATURE_FEMTO_CSG
          case MMR_FILE_REFRESH_IND:
            mmcoord_process_file_refresh_ind(message);
            break;
#endif 
          case MMR_NET_SEL_MODE_CHANGE_IND:
          /* If only network selection mode is changing, update RR/RRC */
            mmcoord_process_mmr_net_sel_mode_change_ind(message);
            break;
            
          case MMR_SIM_STATE_UPDATE_IND:
            mm_handle_message( message );
            break;

          case MMR_CLEAR_LAI_REJECT_LIST_REQ:
            mm_clear_lai_reject_list();
            break;

#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE || defined FEATURE_SVLTE_DUAL_SIM
         case MMR_UE_MODE_CHANGE_IND:
            mmcoord_process_mmr_ue_mode_change_ind(message);
            break;
#endif
#ifdef FEATURE_DUAL_SIM
         case MMR_DATA_PRIORITY_IND:
           mmcoord_process_mmr_data_prioirty_indication(message);
           break;
#endif

         case MMR_PSEUDO_LIST_UPDATE_IND :
             mmcoord_process_pseudo_list_update_ind(message);
             break;
#ifdef FEATURE_CMCC_SECURE_CALL
         case MMR_SECURE_CALL_IND:
             mmcoord_process_mmr_secure_call_ind(message); 
             break;
#endif
#ifdef MMOC_API_PH_STAT_CNF_WITH_STATUS
         case MMR_SIM_BUSY_IND:
          mmcoord_process_mmr_sim_busy_ind(message);
            break;
#endif

#ifdef FEATURE_FEMTO_CSG
          case MMR_CSG_SELECT_CONFIG_UPDATE_IND:
            mmcoord_process_csg_select_config_update_ind(message);
            break;
#endif 

          case MMR_HIGH_PRIORITY_ACTIVITY_IND:
            mmcoord_process_mmr_high_priority_activity_ind(message);
            break; 

#ifdef FEATURE_DUAL_SIM
          case MMR_T3245_EXPIRY_IND: 
            mmcoord_process_mmr_t3245_expiry_ind(message);
            break;
#endif

          default:
            MSG_FATAL_DS( MM_SUB, "=MM= Unexpected REG cmd %d", message->cmd.hdr.message_id, 0,0 );
        }
      }
      break;

    case MS_TIMER:
      if (message->cmd.hdr.message_id == MID_TIMER_EXPIRY)
      {
        /* check if the timer is stopped or restarted after expiry is posted due to race condition */
        if ((mm_timer_status[message->cmd.timer_expiry.timer_id] 
                                                               == TIMER_ACTIVE)
            &&
            ((dword) gs_enquire_timer_ext (
                    get_mm_timer_queue(), 
                    message->cmd.timer_expiry.timer_id,
                    mm_timer_non_deferrable(message->cmd.timer_expiry.timer_id)
                   ) == 0))
        {
          /* ---------------------------------------------
          ** Send timer message off to appropriate handler
          ** --------------------------------------------- */
          switch (message->cmd.timer_expiry.timer_id)
          {
            case TIMER_T3220:
            case TIMER_T3210:
            case TIMER_T3211:
            case TIMER_T3212:
            case TIMER_T3213:
            case TIMER_T3214:
            case TIMER_T3216:
            case TIMER_MM_FORBIDDEN_LAI_CLEAR:
            case TIMER_T3230:
            case TIMER_SCRI_PAGING:
            case TIMER_T3240:
  #if defined ( FEATURE_MMGPS ) || defined ( FEATURE_CGPS_UMTS_CP )
            case TIMER_T3241:
  #endif /* end of (FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP) */
  #ifdef FEATURE_NAS_ECALL
            case  TIMER_T3242:
            case  TIMER_T3243:
  #endif
            case TIMER_MM_REL:
            case TIMER_FRLTE:
  #ifdef FEATURE_DUAL_SIM
            case TIMER_MM_RADIO_RETRY:
  #endif
            case TIMER_MM_PAGE_RETRY:
  #ifdef FEATURE_ENHANCED_NW_SELECTION
            case TIMER_FOREGROUND_SEARCH:
            case TIMER_FORCE_OOS:
  #endif
#ifdef FEATURE_NAS_REL10
          case TIMER_T3246:
#endif
#ifdef FEATURE_FEMTO_CSG
          case TIMER_FORBIDDEN_OPERATOR_CSG_LIST:
#endif 
          case TIMER_CSMT_GUARD:
          case TIMER_POLICY_CHANGE_PENDING:
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_TDSCDMA)
          case TIMER_TDS_RAT_DEACTIVATION:
#endif
#ifdef FEATURE_DUAL_SIM
          case TIMER_MM_DUAL_SWITCH:
#endif
              mm_handle_message( message );
              break;

          case  TIMER_T3247:
             mm_gmm_process_t3247_expiry(message);
             break;
          case  GSM_EXTENDED_BLOCK_TIMER:
             mm_process_gsm_block_timer_expiry(message);
             break;
            case TIMER_T3302:
            case TIMER_T3310:
            case TIMER_T3311:
            case TIMER_T3312:
            case TIMER_T3314:
            case TIMER_T3317:
#ifdef FEATURE_NAS_REL12_SPEC_CHANGE
            /* Spec 24.008, Spec CR 2488 
             * If the service request attempt counter is greater than or equal to 5, 
             * the MS shall not attempt service request for TIMER_T3325 unless it 
             * is in response to paging from the network.
             */
            case TIMER_T3325:
#endif
            case TIMER_T3318:
            case TIMER_T3320:
            case TIMER_T3321:
            case TIMER_T3330:
            case TIMER_POWERDOWN:
            case TIMER_DETACH_REL:
            case TIMER_T3319:
            case TIMER_T3350: 
#ifdef FEATURE_NAS_FREE_FLOAT
          case TIMER_NW_INIT_DETACH:
#endif
  #ifdef FEATURE_DUAL_SIM
            case TIMER_GMM_RADIO_RETRY:
  #endif
#if defined(FEATURE_LTE)
            case TIMER_T3323: 
  #endif

            case TIMER_MM_PS_DETACH: 
  #ifdef FEATURE_NAS_REL7
            case TIMER_T3340:
            case TIMER_PS_CONN_REL:
  #endif
#ifdef FEATURE_NAS_REL10
          case TIMER_T3346:
#endif
              gmm_handle_message( message );
              break;

          case TIMER_FORBID_TDS_RESELECTION:
              gmm_handle_message( message );
              break;
  
            case TIMER_T3316 :
              auth_reset_cache_memory(TRUE,
                        message->cmd.timer_expiry.timer_id,RRC_PS_DOMAIN_CN_ID) ;
              break ;
            case TIMER_T3218 :
  
              auth_reset_cache_memory(TRUE,
                        message->cmd.timer_expiry.timer_id,RRC_CS_DOMAIN_CN_ID) ;
              break ;
  
            default:
              MSG_ERROR_DS( MM_SUB, "=MM= Unexpected timer_id %d", message->cmd.timer_expiry.timer_id, 0,0 );
              break;
          }
        }
      }
      else
      {
        MSG_ERROR_DS( MM_SUB, "=MM= Unexpected timer type %d", message->cmd.hdr.message_id, 0,0 );
      }
      break;

    case MS_GSDI:
         switch (message->cmd.hdr.message_id)
         {
            case SIM_MM_USIM_AUTH_CNF:
            case SIM_MM_GSM_ALGO_CNF:
               MSG_HIGH_DS_1(MM_SUB ,"=MM= MM Received GSDI_CNF: %X", message->cmd.hdr.message_id);
               auth_handle_sim_authentication_response( message, 0 );
               break;

            case SIM_MM_SUBS_READY_IND:
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received SIM_MM_SUBS_READY_IND");
               mmcoord_process_mmgsdi_subs_ready( message );
               break;

#ifdef FEATURE_FEMTO_CSG
            case SIM_MM_GET_FILE_ATTR_CNF:
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received SIM_MM_GET_FILE_ATTR_CNF");
               mmcoord_process_mmgsdi_get_file_attr_cnf( message );
               break;

           case SIM_MM_READ_FILE_CNF:
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received SIM_MM_READ_FILE_CNF");
               mmcoord_process_mmgsdi_read_file_cnf( message );
               break;
#endif 
           case SIM_MM_PURGE_GUTI_IND:
#if defined(FEATURE_LTE)
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received SIM_MM_PURGE_GUTI_IND");			   
               emm_process_guti_purge_ind(message);
#endif
               break;
            default:
               MSG_FATAL_DS( MM_SUB, "=MM= Unexpected GSDI cmd %d", message->cmd.hdr.message_id, 0,0 );
         }
      break;

    case MS_GMM_SMS:
    case MS_GMM_SM:
    case MS_GMM_RABM:
    case MS_GMM_TC:
      gmm_handle_message( message );
      break;

    case MS_MM_TC:
       mm_handle_message( message );
      break;

#ifdef FEATURE_GSM_GPRS
    case MS_LLC_GMM :
      gmmllc_handle_llc_event( message ) ;
      break ;

    case MS_CM_MM:
      mm_handle_message(message);	
      gmm_handle_message( message );
      break ;

#endif /* #ifdef FEATURE_GSM_GPRS */

     case MS_WMS_MM :
       switch(message->cmd.hdr.message_id)
       {
         case MM_SMS_BEARER_SELECTION_CMD:
           mm_handle_message( message );
		   break;

         case MM_SMS_STATUS_CMD:
#if defined(FEATURE_LTE)
           emm_process_wms_sms_info(message,emm_ctrl_data_ptr);
#endif
           break;
         default:
            MSG_FATAL_DS( MM_SUB, "=MM= Unexpected WMS cmd %d", message->cmd.hdr.message_id, 0,0 );
       }
      break ;
#ifdef FEATURE_LTE
    case MS_MM_MM:
       switch(message->cmd.hdr.message_id)
       {
         case MM_NON_ACQ_PARAM_CHANGE_IND:
           mmcoord_process_non_acq_param_change_ind(message);
           break;
         default:
           MSG_HIGH_DS_1(MM_SUB,"Unexpected Message id %d", message->cmd.hdr.message_id);
           break;
       }
       break;
#endif
    default:
      MSG_ERROR_DS( MM_SUB, "=MM= Unexpected message set %d", message->cmd.hdr.message_set, 0,0 );
      break;
  } /* end switch (message->cmd.hdr.message_set) */
} /* end mmcoord_route_mm_message() */

/*===========================================================================

FUNCTION MM_CHECK_IF_CELL_ACCESS_VALID

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  TRUE if passed parameter is valid,
  FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/

boolean mm_check_if_cell_access_valid(mm_as_cell_access_status_T cell_access)
{
  boolean valid_command = TRUE;
  if ((cell_access != MM_AS_ACCESS_NORMAL_ONLY) &&
      (cell_access != MM_AS_ACCESS_EMERGENCY_ONLY) &&
      (cell_access != MM_AS_ACCESS_NO_CALLS) &&
      (cell_access != MM_AS_ACCESS_ALL_CALLS))
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Invalid cell_access: %d",cell_access);
    valid_command = FALSE;
  }
  return valid_command;
}

/*===========================================================================

FUNCTION MM_CHECK_IF_PAGE_RSP_ALLLOWED_VALID

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  TRUE if passed parameter is valid,
  FALSE otherwise.

SIDE EFFECTS
  None

===========================================================================*/
boolean mm_check_if_page_rsp_allowed_valid(mm_as_page_rsp_e_type page_rsp_allowed)
{
  boolean valid_command = TRUE;
  if ((page_rsp_allowed != MM_AS_CS_PS_ALLOWED) &&
      ( page_rsp_allowed != MM_AS_CS_ALLOWED) &&
      (page_rsp_allowed != MM_AS_PS_ALLOWED) &&
      (page_rsp_allowed != MM_AS_NONE))
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Invalid PAGE RSP ALLOWED: %d",page_rsp_allowed);
    valid_command = FALSE;
  }
  return valid_command;
}
void mmcoord_handle_deep_sleep_alt_scan(void)
{
  /* if FEATURE_LTE is defined stopping of MM and EMM timers is
   *  done in "mmcoord_route_mm_emm_messages()" handler 
   */
#ifndef FEATURE_LTE
  byte i;
  MSG_HIGH_DS_0(MM_SUB, "=MM= ALT SCAN. Pause all running timers (except periodic timers)"); 
  for(i=0;i<MAX_MM_TIMER+1;i++)
  {
    if (mm_timer_status[i] == TIMER_ACTIVE && 
        (i != TIMER_T3212 && i != TIMER_T3312 &&
         i != TIMER_T3346 && i != TIMER_T3246 ) &&
        (i != TIMER_MM_FORBIDDEN_LAI_CLEAR))
    {
      if (gs_modify_timer_ext(
                               get_mm_timer_queue(),
                               i,
                               TIMER_MODIFY_PAUSE,
                               mm_timer_non_deferrable(i)) != GS_SUCCESS)
      {
        MSG_ERROR_DS( MM_SUB, "=MM= Failed to pause timer %d", i, 0, 0);
      }
      else
      {
        MSG_HIGH_DS_1(MM_SUB ,"=MM= Pausing %d", i);
        mm_timer_status[ i ] = TIMER_PAUSED;
      }  
    }
    /* Mark deep sleep alt scan flag as true, so that in next CM service req. 
    ** turn ON all paused timers. */
    mm_deep_sleep_alt_scan = TRUE;      
  }
#endif

  if (mm_serving_plmn.info.active_rat == SYS_RAT_GSM_RADIO_ACCESS)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= Send RR_STOP_MODE_REQ for Deep sleep Alt Scan");
    mm_deep_sleep_pending = TRUE;
    mm_send_rr_stop_mode_req(RR_MM_DEEP_SLEEP);
  }
  else
  {
    /*----------------------------------------------------------
    ** Send the STOP MODE REQUEST to WCDMA side for Deep sleep
    ** --------------------------------------------------------*/
    MSG_HIGH_DS_0(MM_SUB, "=MM= Send RRC_STOP_MODE_REQ for Deep sleep Alt Scan");
    mm_deep_sleep_pending = TRUE;
    mm_send_rrc_stop_mode_req( RRC_DEEP_SLEEP,
                               mm_serving_plmn.info.active_rat );
  }
}
void mmcoord_handle_deep_sleep(void)
{
   if (mm_serving_plmn.info.active_rat == SYS_RAT_GSM_RADIO_ACCESS)
   {
	 MSG_HIGH_DS_0(MM_SUB, "=MM= Send RR_STOP_MODE_REQ for Deep sleep");
	 mm_deep_sleep_pending = TRUE;
	 mm_send_rr_stop_mode_req(RR_MM_DEEP_SLEEP);
   }
   else
   {
	 /*----------------------------------------------------------
	 ** Send the STOP MODE REQUEST to WCDMA side for Deep sleep
	 ** --------------------------------------------------------*/
	 MSG_HIGH_DS_0(MM_SUB, "=MM= Send RRC_STOP_MODE_REQ for Deep sleep");
    mm_deep_sleep_pending = TRUE;
    mm_send_rrc_stop_mode_req( RRC_DEEP_SLEEP,
                               mm_serving_plmn.info.active_rat );
   }
   
#if defined(FEATURE_GSTK) && !defined(FEATURE_EOOS)
   if (!reg_nv_is_imsi_switch_enabled())
   {
	 mm_send_gstk_no_service_info();
  }
#endif
}
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE
/*===========================================================================

FUNCTION mmcoord_handle_ue_sglte_mode_switch

DESCRIPTION
  This function handles DUAL MODE Switch.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_handle_ue_sglte_mode_switch(void)
{
  byte index;
  MSG_HIGH_DS_0(MM_SUB, "=MM= Sglte mode SWITCH: Pausing only required MM  timers "); 
  
  /* Stop all MM timers */
  for(index = 0; index <= MAX_MM_TIMER; index++)
  {
    if(mm_timer_status[index] != TIMER_STOPPED)
    {
      if(index != EMM_FORBIDDEN_TAI_CLEAR_TIMER &&
         index != TIMER_MM_FORBIDDEN_LAI_CLEAR  &&
         index != TIMER_RADIO_CAP_ENQ 
           &&!((index == TIMER_T3412 || index == TIMER_T3423
                    || index == TIMER_T3442 || index == TIMER_T3346
                    || index == TIMER_T3246 || index == TIMER_T3312
                    || index == TIMER_T3212 || index == TIMER_T3323
                    || index == TIMER_T3402 || index == TIMER_T3302 )))
      {
        mm_stop_timer(index);
        mm_timer_status[index] = TIMER_STOPPED;
      }
    }
  }

  if (mm_serving_plmn.info.active_rat == SYS_RAT_GSM_RADIO_ACCESS)
  {
    mm_deep_sleep_pending = TRUE;
    MSG_HIGH_DS_0(MM_SUB, "=MM= Send RR_STOP_MODE_REQ for UE SGLTE MODE SWITCH");
    mm_send_rr_stop_mode_req(RR_UE_SGLTE_MODE_SWITCH);
  }
  else if (mm_serving_plmn.info.active_rat == SYS_RAT_UMTS_RADIO_ACCESS)
  {
    /*----------------------------------------------------------
    ** Send the STOP MODE REQUEST to WCDMA side for UE SGLTE MODE SWITCH
    ** --------------------------------------------------------*/
    mm_deep_sleep_pending = TRUE;
    MSG_HIGH_DS_0(MM_SUB, "=MM= Send RRC_STOP_MODE_REQ for UE SGLTE MODE SWITCH");
    mm_send_rrc_stop_mode_req( RRC_DEEP_SLEEP,                              
                               mm_serving_plmn.info.active_rat );
  }
  else 
  {
    /*----------------------------------------------------------
    ** Send the STOP MODE REQUEST to TD-CDMA side for UE SGLTE MODE SWITCH
    ** --------------------------------------------------------*/
    mm_deep_sleep_pending = TRUE;
    MSG_HIGH_DS_0(MM_SUB, "=MM= Send RRC_STOP_MODE_REQ for UE SGLTE MODE SWITCH");
    mm_send_rrc_stop_mode_req( RRC_UE_SGLTE_MODE_SWITCH,
                               mm_serving_plmn.info.active_rat );
  }
}
#endif

#ifdef FEATURE_DUAL_SIM

/*===========================================================================

FUNCTION mmcoord_handle_deep_sleep_dual_switch

DESCRIPTION
  This function handles DUAL MODE Switch.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_handle_deep_sleep_dual_switch(void)
{
  byte i;

  MSG_HIGH_DS_0(MM_SUB, "=MM= DUAL SWITCH: Pause all running timers (except periodic timers)"); 

  for(i=0;i<MAX_MM_TIMER+1;i++)
  {
    if (mm_timer_status[i] == TIMER_ACTIVE && 
        (i != TIMER_T3212 && i != TIMER_T3312 &&
         i != TIMER_T3346 && i != TIMER_T3246 ) &&
        (i != EMM_FORBIDDEN_TAI_CLEAR_TIMER) &&
        (i != TIMER_RADIO_CAP_ENQ) &&
        (i != TIMER_MM_FORBIDDEN_LAI_CLEAR))
    {
      if (gs_modify_timer_ext(
                               get_mm_timer_queue(),
                               i,
                               TIMER_MODIFY_PAUSE,
                               mm_timer_non_deferrable(i)) != GS_SUCCESS)
      {
        MSG_ERROR_DS( MM_SUB, "=MM= Failed to pause timer %d", i, 0, 0);
      }
      else
      {
        MSG_HIGH_DS_1(MM_SUB ,"=MM= Pausing %d", i);
        mm_timer_status[ i ] = TIMER_PAUSED;
      }  
    }
    /* Mark deep sleep alt scan flag as true, so that in next CM service req. 
    ** turn ON all paused timers. */
    mm_deep_sleep_alt_scan = TRUE;      
  }

  if (mm_serving_plmn.info.active_rat == SYS_RAT_GSM_RADIO_ACCESS)
  {
    mm_deep_sleep_pending = TRUE;
    MSG_HIGH_DS_0(MM_SUB, "=MM= Send RR_STOP_MODE_REQ for DUAL STANDBY SWITCH");
    mm_send_rr_stop_mode_req(RR_MM_DUAL_SWITCH);
  }
  else
  {
    /*----------------------------------------------------------
    ** Send the STOP MODE REQUEST to WCDMA side for Deep sleep
    ** --------------------------------------------------------*/
    mm_deep_sleep_pending = TRUE;
    MSG_HIGH_DS_0(MM_SUB, "=MM= Send RRC_STOP_MODE_REQ for DUAL STANDBY SWITCH");
    mm_send_rrc_stop_mode_req( RRC_DUAL_SWITCH,
                               mm_serving_plmn.info.active_rat );
  }
}

/*===========================================================================

FUNCTION mmcoord_process_mmr_ds_stat_chgd_req

DESCRIPTION
  This function sends change in DUAL STANDBY STATUS to RR(C)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_ds_stat_chgd_req( mm_cmd_type *message )
{
  mm_as_id_e_type as_id;
  
  if (message->cmd.hdr.message_id == (int) MMR_DS_STAT_CHGD_REQ)
  {
    mm_dual_standby_mode = message->cmd.mmr_ds_stat_chgd_req.dual_standby_mode;
    mmode_nv_setting_for_device_mode = message->cmd.mmr_ds_stat_chgd_req.device_mode;
    MSG_HIGH_DS_2( MM_SUB, "=MM= MM received MMR_DS_STAT_CHGD_REQ: Standby mode =%d, Device Mode = %d", 
                                   mm_dual_standby_mode,mmode_nv_setting_for_device_mode);

#ifdef FEATURE_FEMTO_CSG
    if(mm_nv_enable_csg == FALSE && mmode_nv_setting_for_device_mode > SYS_MODEM_DEVICE_MODE_SINGLE_SIM)
    {
      mm_disable_csg_support();
    }
#endif

    for (as_id = MM_AS_ID_1; as_id < (mm_as_id_e_type)MAX_AS_IDS; as_id++)
    {
      mm_set_as_id(as_id);
      mm_conn_mode_manual_search_enabled = mm_nv_conn_mode_manual_search;
      if ((mm_nv_disable_conn_mode_mplmn == FALSE)&&
          ((mmode_nv_setting_for_device_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY) ||
           (mmode_nv_setting_for_device_mode == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY))&&
          (message->cmd.mmr_ds_stat_chgd_req.dual_standby_mode_info != SYS_MODEM_DS_PREF_SINGLE_STANDBY))
      {
         mm_conn_mode_manual_search_enabled = TRUE;
      }
    }
    if((mmode_nv_setting_for_device_mode == SYS_MODEM_DEVICE_MODE_DUAL_SIM_DUAL_STANDBY) ||
       (mmode_nv_setting_for_device_mode == SYS_MODEM_DEVICE_MODE_TRIPLE_SIM_TRIPLE_STANDBY))
    {
      mm_is_multisim_device = TRUE;
    }

    mm_set_as_id(MM_AS_ID_1);

    /* --------------------------------------------------------------------
    ** Pass MMR_DS_STAT_CHGD_REQ to RR(C).
    ** -------------------------------------------------------------------- */
    mm_send_ds_stat_chgd_ind(message);
    mm_send_mmr_ds_stat_chgd_cnf();
  }
  else
  {
    MSG_ERROR_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
}


/*===========================================================================

FUNCTION mm_emm_init_multimode_stack_subs_chgd_req

DESCRIPTION
  This function initializes the new multimode stack and sends the SIM and SYSTEM
  updates to access stratum on the new multimode stack.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#ifdef FEATURE_DUAL_SIM

void mm_emm_init_stack_subs_chgd_req
(
#ifdef FEATURE_LTE
  emm_ctrl_data_type *emm_ctrl_data_ptr,
#endif
  uint32 old_sub_mask
)
{
  if(mm_sim_card_mode != MMGSDI_APP_UNKNOWN)
  {
#ifdef FEATURE_LTE
    if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS) == TRUE)
    {   
      if((old_sub_mask & SYS_SYS_MODE_MASK_LTE) == FALSE)
      {
        if (emm_ctrl_data_ptr->imsi_not_native == TRUE)
        {
          emm_ctrl_data_ptr->imsi_not_native = FALSE;
          MSG_HIGH_DS_0(MM_SUB,"=EMM= Notify T3402 default value to clients");
          emm_ctrl_data_ptr->t3402_value = EMM_DEFAULT_T3402;
          emm_ctrl_data_ptr->t3402_attach_rej_param.is_t3402_recv_attach_rej = FALSE;
          /* delete EPS security context */
          MSG_HIGH_DS_0(MM_SUB,"=EMM= Deleting EPSNSC and EPSLOCI from EFS ");
          emm_delete_efs_security_context();
          (void)mcfg_fs_delete( "/nvm/alpha/modem/nas/lte_nas_eps_loci", MCFG_FS_TYPE_EFS, (mcfg_fs_sub_id_e_type)mm_sub_id);
          emm_invalidate_mobility_info(EMM_STATUS_NOT_UPDATED, 
                                           SYS_SIM_STATE_AVAILABLE, 
                                           emm_ctrl_data_ptr, FALSE) ;
        }
        (void)emm_load_epsloci_data(emm_ctrl_data_ptr,FALSE);
        emm_load_security_context(emm_ctrl_data_ptr);
        (void)emm_update_rplmn(emm_ctrl_data_ptr);
        /* reset the mm nas ul count */
        mm_reset_ul_count_at_generation();
      }
      emm_send_sim_update(emm_ctrl_data_ptr,TRUE) ; 
      emm_send_rrc_system_update(emm_ctrl_data_ptr) ;
    }
#endif
#ifdef FEATURE_FEMTO_CSG
    if((mm_nv_csg_support > SYS_CSG_SUPPORT_DISABLED)&&
        ((mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS) == TRUE) ||
         (mm_sub_is_rat_enabled(SYS_RAT_UMTS_RADIO_ACCESS) == TRUE)) &&
          ((mm_sim_allowed_csg_list_read_started == FALSE) &&
           (mm_sim_operator_csg_list_read_started == FALSE)))
   {
    mm_sim_read_csg_list();
   }
#endif

    /* Send SIM data to RRC as we have not sent earlier */
    if(mm_wcdma_supported &&
       (mm_sub_is_rat_enabled(SYS_RAT_UMTS_RADIO_ACCESS) == TRUE))
    {
      mm_send_rrc_sim_inserted_req( SYS_RAT_UMTS_RADIO_ACCESS );
    }
#ifdef FEATURE_TDSCDMA
    if (mm_tdscdma_supported &&
       (mm_sub_is_rat_enabled(SYS_RAT_TDS_RADIO_ACCESS) == TRUE))
    {
      mm_send_rrc_sim_inserted_req( SYS_RAT_TDS_RADIO_ACCESS );
    }
#endif /* FEATURE_TDSCDMA */
   if(mm_sub_is_rat_enabled(SYS_RAT_GSM_RADIO_ACCESS) == TRUE)
   {
      mm_send_rr_sim_update_req();
   }
    /*Sending forbidden LAI list to W/TDS-RRC on supported sub so that both NAS & lower layers are in sync*/
#if defined(FEATURE_WCDMA) || defined(FEATURE_TDSCDMA)
        mm_send_rrc_forbidden_lai_update();
#endif
  }
  else
  {
    
#ifdef FEATURE_LTE
    if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS) == TRUE)
    {
      emm_send_sim_update(emm_ctrl_data_ptr,TRUE);
    }
#endif
  }
#ifdef FEATURE_LTE
  if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS) == TRUE)
  {
    emm_send_t3402_changed_ind(emm_ctrl_data_ptr->t3402_value);
  }
#endif
}

#endif
/*===========================================================================

FUNCTION mmcoord_process_mmr_multimode_subs_chgd_req

DESCRIPTION
  This function sends subs capability to RR

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_multimode_subs_chgd_req( mm_cmd_type *message )
{
  uint32 mm_subs_capability_mask_old[MAX_AS_IDS];
  mm_as_id_e_type as_id;
#ifdef FEATURE_LTE 
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
#endif 

  if (message->cmd.hdr.message_id == (int) MMR_MULTIMODE_SUBS_CHGD_REQ)
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received MMR_MULTIMODE_SUBS_CHGD_REQ: ");
    /* --------------------------------------------------------------------
    ** Pass MMR_MULTIMODE_SUBS_CHGD_REQ to RR
    ** -------------------------------------------------------------------- */
    mm_send_rr_multimode_subs_chgd_req(&message->cmd.mmr_multimode_subs_chgd_req);
    mm_send_rrc_multimode_subs_chgd_ind(&message->cmd.mmr_multimode_subs_chgd_req);

    /* Deallocate memory for previous LTE SUB and allocate for new SUB */
#ifdef FEATURE_LTE
    emm_send_rrc_multimode_subs_chgd_ind(&message->cmd.mmr_multimode_subs_chgd_req);

    for (as_id = MM_AS_ID_1; as_id < (mm_as_id_e_type)MAX_AS_IDS; as_id++)
    {
      emm_ctrl_data_ptr = emm_db_get_ctrl_data_per_subs(as_id);
      //if LTE is part of sub capability then allocate memory for EMM for this sub if not allocated
      if(message->cmd.mmr_multimode_subs_chgd_req.subs_capability_mask[as_id] & SYS_SYS_MODE_MASK_LTE)
      {
        if((mm_subs_capability_mask_sim[as_id] & SYS_SYS_MODE_MASK_LTE) == 0)
        {
          emm_ctrl_data_mem_alloc(emm_ctrl_data_ptr);
          mm_send_sm_multimode_subs_chgd_req(as_id);
        }
      }
      else /* If L is not part of sub capability then free the memory */
      {
        if(mm_subs_capability_mask_sim[as_id] & SYS_SYS_MODE_MASK_LTE)
        {
          if (emm_ctrl_data_ptr->emm_connection_state == EMM_CONNECTED_STATE)
          {
            MSG_FATAL_DS(MM_SUB,"Received Multimode SUB change with L removed on sub %d when LTE is connected",
                                        as_id,0,0);
          }
          emm_ctrl_data_mem_free(emm_ctrl_data_ptr);
        }
      }
    }
#endif 
    memscpy(mm_subs_capability_mask_old,sizeof(mm_subs_capability_mask_old),
           mm_subs_capability_mask_sim, sizeof(mm_subs_capability_mask_sim));

    memscpy(mm_subs_capability_mask_sim,sizeof(mm_subs_capability_mask_sim),
           message->cmd.mmr_multimode_subs_chgd_req.subs_capability_mask,
           sizeof(message->cmd.mmr_multimode_subs_chgd_req.subs_capability_mask));

    for (as_id = MM_AS_ID_1; as_id < (mm_as_id_e_type)MAX_AS_IDS; as_id++)
    {
      if(mm_subs_capability_mask_sim[as_id] != mm_subs_capability_mask_old[as_id])
      {
        mm_set_as_id(as_id);
#ifdef FEATURE_LTE
        emm_ctrl_data_ptr = emm_db_get_ctrl_data_per_subs(mm_sub_id);
#endif
        mm_emm_init_stack_subs_chgd_req(
     #ifdef FEATURE_LTE
         emm_ctrl_data_ptr,
     #endif
         mm_subs_capability_mask_old[as_id]
        );
        if(mm_sub_is_rat_enabled(mm_last_active_rat_stopped) == FALSE)
        {
          mm_last_active_rat_stopped = SYS_RAT_NONE;
        }
        if(mm_sub_is_rat_enabled(mm_serving_plmn.info.active_rat) == FALSE)
        {
          mm_serving_plmn.info.active_rat = SYS_RAT_NONE;
        }
      }
    }
    mm_set_as_id(MM_AS_ID_1);
  } 
  else
  {
    MSG_ERROR_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
}


/*===========================================================================

FUNCTION mmcoord_process_mmr_dds_switch_ind

DESCRIPTION
  This function informs target DDS switch subscription ID.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_dds_switch_ind( mm_cmd_type *message )
{
  
  sys_modem_as_id_e_type    mm_other_as_id;
  sys_modem_as_id_e_type    target_dds_local = message->cmd.mmr_dds_switch_ind.target_dds_sub;
#ifdef FEATURE_DUAL_SIM
  gmm_force_for_always_on_data_sim[SYS_MODEM_AS_ID_1]=FALSE;
  gmm_force_for_always_on_data_sim[SYS_MODEM_AS_ID_2]=FALSE;
  mm_dds_switch_while_rrc_conn_active_sim[SYS_MODEM_AS_ID_1]=FALSE;
  mm_dds_switch_while_rrc_conn_active_sim[SYS_MODEM_AS_ID_2]=FALSE;
  
  if( message->cmd.mmr_dds_switch_ind.always_on_data==TRUE && gmm_per_subs_always_on_data_enabled(target_dds_local)==TRUE)
  {
	gmm_force_for_always_on_data_sim[target_dds_local]=TRUE;
	if(rrc_connection_status_sim[target_dds_local]!=  MMRRC_IDLE)
     {
      mm_dds_switch_while_rrc_conn_active_sim[target_dds_local]=TRUE;
	 }
  }
#endif 

  if (message->cmd.hdr.message_id == (int) MMR_DDS_SWITCH_IND)
  {
     MSG_HIGH_DS_2(MM_SUB, "=MM= MM received MMR_DDS_SWITCH_IND , target DDS: %d, always_on_data: %d", target_dds_local, message->cmd.mmr_dds_switch_ind.always_on_data);

#ifdef FEATURE_LTE
    emm_send_rrc_dds_sub_info_ind(target_dds_local);
#endif
    mm_send_rr_dds_sub_ind(target_dds_local);
    mm_send_rrc_dds_sub_ind(target_dds_local);
#ifdef FEATURE_DUAL_DATA
    mm_dds_sub = (mm_as_id_e_type)target_dds_local;   
#endif

    for(mm_other_as_id = SYS_MODEM_AS_ID_1; mm_other_as_id < MAX_NAS_STACKS ; mm_other_as_id++)
    {
      gmm_force_local_detach_sim[mm_other_as_id] = FALSE;

      if ((((gmm_state_sim[mm_other_as_id] != GMM_NULL) &&
             (gmm_state_sim[mm_other_as_id] != GMM_DEREGISTERED))
#if defined(FEATURE_LTE) 
          || ((mm_serving_plmn_sim[mm_other_as_id].info.active_rat == SYS_RAT_LTE_RADIO_ACCESS) &&
              ((emm_db_get_ctrl_data_per_subs(mm_other_as_id)->emm_state) != EMM_NULL) &&
              ((emm_db_get_ctrl_data_per_subs(mm_other_as_id)->emm_state) != EMM_DEREGISTERED))
#endif
           ))
      {
         MSG_HIGH_DS_1(MM_SUB, "=MM= DDS Switch: MM Waiting for Detach completion on ds%d",
               mm_other_as_id+1);
          gmm_force_local_detach_sim[mm_other_as_id] = TRUE;

      }
    }
  }
}


#endif
#ifdef FEATURE_LTE
/*===========================================================================

FUNCTION mmcoord_process_gw_plmn_sel_end_ind

DESCRIPTION
  This function clears the CSFB state and sends call end ind to CM

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_process_gw_plmn_sel_end_ind(void)
{
  if(mm_csfb_state == MM_CSFB_STATE_MO_CALL_ORIG)
  {
    emm_ctrl_data_type *emm_ctrl_data_ptr = emm_db_get_ctrl_data();
#ifdef FEATURE_SMS_ENHANCED_DOMAIN_SELECTION
    mm_send_mmcnm_est_rej( MM_REJECT_CAUSE, MM_REJ_GW_NOT_AVAILABLE, MMCC_SMS_CS);
#else
    mm_send_mmcnm_est_rej( MM_REJECT_CAUSE, MM_REJ_GW_NOT_AVAILABLE );
#endif
    mm_follow_on_request = FALSE;
    mm_csfb_set_state(MM_CSFB_STATE_NULL);
    emm_abort_and_clean_csfb_call(emm_ctrl_data_ptr);
  } 
}
#endif

#ifdef FEATURE_OOSC_USER_ACTION
/*===========================================================================

FUNCTION mmcoord_process_mmr_exit_from_pwr_save

DESCRIPTION
  This function processes MMR_EXIT_FROM_PWR_SAVE from REG 
  as part of UOOS Connected mode UI Update

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mmcoord_process_mmr_exit_from_pwr_save( mm_cmd_type *message )
{

  if (is_umts_mode() && (message->cmd.hdr.message_id == (int) MMR_EXIT_FROM_PWR_SAVE))
  {
    MSG_HIGH_DS_0(MM_SUB, "=MM= MM received MMR_EXIT_FROM_PWR_SAVE");

    /* --------------------------------------------------------------------
    ** Pass MMR_EXIT_FROM_PWR_SAVE to RRC.
    ** -------------------------------------------------------------------- */
    mm_send_rrc_exit_from_pwr_save();
  }
  else
  {
    MSG_ERROR_DS( MM_SUB, "=MM= Unexpected message %d", message->cmd.hdr.message_id, 0,0 );
  }
}
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL

void mm_process_stop_mode_req_during_wrlf
(
  mm_cmd_type *mm_cmd,
#ifdef FEATURE_LTE
  emm_ctrl_data_type  *emm_ctrl_data_ptr_arg,
#endif
  mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr
)
{
  ASSERT(mm_emm_dispatcher_fn_ptr != NULL);
#ifdef FEATURE_LTE
  ASSERT(emm_ctrl_data_ptr_arg != NULL); 
#endif
  mm_rlf_stop_mode_pending = MM_WCDMA_RLF_STOP_MODE_PENDING;
  mmcoord_pending_reg_message.cmd.mmr_stop_mode_req = mm_cmd->cmd.mmr_stop_mode_req;
  mm_wrlf_rat_change_pending = FALSE;
  mmcoord_reg_req_pending = FALSE;
  
#ifdef FEATURE_LTE
  mm_multimode_rat_change_pending = FALSE;
  emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;
#endif
  if(mm_waiting_for_stop_mode_cnf && (mm_serving_plmn.info.active_rat == mm_wrlf_serving_plmn_ptr->info.active_rat))
  {
    MSG_HIGH_DS_0(MM_SUB," =MM= mm alreay waiting for stop cnf, so pend this new stop mode req");
  }
  else
  {
    mm_waiting_for_stop_mode_cnf = FALSE;
    mm_send_rrc_hard_abort_req( mm_wrlf_serving_plmn_ptr->info.active_rat );
  }
#ifdef FEATURE_LTE
  *mm_emm_dispatcher_fn_ptr = mm_emm_oos_rlf_msg_handler;
#endif
}

#endif

#ifdef FEATURE_LTE

/*===========================================================================
FUNCTION    emm_get_esm_as_id

DESCRIPTION
  This function processes incoming esm messages to get the as_id.
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void emm_get_esm_as_id
(
  void               *emm_message_router_cmd
)
{
#if defined(FEATURE_DUAL_SIM)
  emm_cmd_type   *emm_msgr_cmd_ptr = NULL; 
  emm_msgr_cmd_ptr = (emm_cmd_type *)emm_message_router_cmd;
  mm_sub_id = MSGR_GET_VARIANT(&(emm_msgr_cmd_ptr->emm_msgr_cmd.msg_hdr));
  MSGR_CLEAR_VARIANT(emm_msgr_cmd_ptr->emm_msgr_cmd.msg_hdr.id);
  if(IS_NOT_VALID_SUB_ID(mm_sub_id))
  {
    MSG_FATAL_DS(MM_SUB, "ERR FATAL Invalid AS ID:%d ESM UMID %d ",mm_sub_id,emm_msgr_cmd_ptr->emm_msgr_cmd.msg_hdr.id,0);
  }
#endif 
#ifdef FEATURE_SGLTE
  if(MM_SUB_IS_SGLTE)
  {
    mm_as_id = mm_ps_stack_id;
  }
  else
#endif
  {
    mm_as_id = mm_sub_id;
  }
  mm_message_as_id = (byte)(mm_as_id + 1);
}


/*===========================================================================

FUNCTION EMM_GET_RRC_AS_ID

DESCRIPTION
  This function gets as_id from the RRC primitive.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*lint -save -e550 Symbol 'dummy_return_val' not accessed */

void emm_get_rrc_as_id
(
  void *rrc_message_router_cmd
)
{
#ifdef FEATURE_DUAL_SIM
  emm_rrc_cmd_type   *rrc_msgr_cmd;
  rrc_msgr_cmd = (emm_rrc_cmd_type*)rrc_message_router_cmd;
  mm_sub_id = MSGR_GET_VARIANT(&(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr));
  MSGR_CLEAR_VARIANT(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id);
  if(IS_NOT_VALID_SUB_ID(mm_sub_id))
  {
    MSG_FATAL_DS(MM_SUB, "ERR FATAL Invalid AS ID:%d RRC UMID %d ",mm_sub_id,rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id,0);
  }
#endif 
#ifdef FEATURE_SGLTE
  if(MM_SUB_IS_SGLTE)
  {
    mm_as_id = mm_ps_stack_id;
  }
  else
#endif
  {
    mm_as_id = mm_sub_id;
  }
  mm_message_as_id = (byte)(mm_as_id + 1);
}


/*===========================================================================

FUNCTION EMM_GET_TLB_AS_ID

DESCRIPTION
  This function gets as_id from the TLB primitive.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*lint -save -e550 Symbol 'dummy_return_val' not accessed */

void emm_get_tlb_as_id
(
  void *cmd_ptr
)
{
#ifdef FEATURE_DUAL_SIM
  tlb_cmd_type *emm_cmd_ptr;
  emm_cmd_ptr = (tlb_cmd_type*)cmd_ptr;

  mm_sub_id = MSGR_GET_VARIANT(&(emm_cmd_ptr->cmd.hdr));
  MSGR_CLEAR_VARIANT(emm_cmd_ptr->cmd.hdr.id);
  if(IS_NOT_VALID_SUB_ID(mm_sub_id))
  {
    MSG_FATAL_DS(MM_SUB, "ERR FATAL Invalid AS ID:%d TLB UMID %d ",mm_sub_id,emm_cmd_ptr->cmd.hdr.id,0);
  }
#endif

#ifdef FEATURE_SGLTE
  if(MM_SUB_IS_SGLTE)
  {
    mm_as_id = mm_ps_stack_id;
  }
  else
#endif
  {
    mm_as_id = mm_sub_id;
  }
  mm_message_as_id = (byte)(mm_as_id + 1);
}


/*===========================================================================

FUNCTION EMM_GET_IRAT_CSFB_AS_ID

DESCRIPTION
  This function gets as_id from the 1x CSFB primitive.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*lint -save -e550 Symbol 'dummy_return_val' not accessed */

void emm_get_irat_csfb_as_id
(
  void *cmd_ptr
)
{
#ifdef FEATURE_DUAL_SIM
  emm_csfb_cmd_type  *csfb_cmd_ptr = NULL;

  csfb_cmd_ptr = (emm_csfb_cmd_type *)cmd_ptr; 

  mm_sub_id = MSGR_GET_VARIANT(&(csfb_cmd_ptr->cmd.msg_hdr));
  MSGR_CLEAR_VARIANT(csfb_cmd_ptr->cmd.msg_hdr.id);
  if(IS_NOT_VALID_SUB_ID(mm_sub_id))
  {
    MSG_FATAL_DS(MM_SUB, "ERR FATAL Invalid AS ID:%d CSFB UMID %d ",mm_sub_id,csfb_cmd_ptr->cmd.msg_hdr.id,0);
  }
#endif
#ifdef FEATURE_SGLTE
  if(MM_SUB_IS_SGLTE)
  {
    mm_as_id = mm_ps_stack_id;
  }
  else
#endif
  {
    mm_as_id = mm_sub_id;
  }
  mm_message_as_id = (byte)(mm_as_id + 1);
}

/*===========================================================================

FUNCTION EMM_GET_GPS_AS_ID

DESCRIPTION
  This function gets as_id from the GPS primitive.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
/*lint -save -e550 Symbol 'dummy_return_val' not accessed */

void emm_get_gps_as_id
(
  void *cmd_ptr
)
{
#ifdef FEATURE_DUAL_SIM
  emm_gps_cmd_type *emm_cmd_ptr = NULL;
  emm_cmd_ptr = (emm_gps_cmd_type*)cmd_ptr; 
  mm_sub_id = MSGR_GET_VARIANT(&(emm_cmd_ptr->gps_msgr_cmd.msg_hdr));
  MSGR_CLEAR_VARIANT(emm_cmd_ptr->gps_msgr_cmd.msg_hdr.id);
  if(IS_NOT_VALID_SUB_ID(mm_sub_id))
  {
    MSG_FATAL_DS(MM_SUB, "ERR FATAL Invalid AS ID:%d GPS UMID %d ",mm_sub_id,emm_cmd_ptr->gps_msgr_cmd.msg_hdr.id,0);
  }
#endif
#ifdef FEATURE_SGLTE
  if(MM_SUB_IS_SGLTE)
  {
    mm_as_id = mm_ps_stack_id;
  }
  else
#endif
  {
    mm_as_id = mm_sub_id;
  }
  mm_message_as_id = (byte)(mm_as_id + 1);
}

/*===========================================================================

FUNCTION EMM_POPULATE_MM_MSG_INFO

DESCRIPTION
  This function populates variable mm_msg_info for EMM messages for debugging purposes.

DEPENDENCIES
  None

SIDE EFFECTS
  None

===========================================================================*/

void emm_populate_mm_msg_info(void *rrc_message_router_cmd)
{
  emm_rrc_cmd_type   *rrc_msgr_cmd;
  ASSERT(rrc_message_router_cmd != (void *)NULL);
  rrc_msgr_cmd = (emm_rrc_cmd_type*)rrc_message_router_cmd;

  switch(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id)
  {
    case LTE_RRC_CONN_EST_CNF:
    case LTE_RRC_UL_DATA_CNF:
    case LTE_RRC_DEACTIVATE_CNF:
    case LTE_RRC_ACTIVATION_IND:
    case LTE_RRC_NAS_UMTS_KEY_IND:
    case LTE_RRC_NAS_GSM_KEY_IND:
    case LTE_RRC_NAS_LTE_KEY_IND:
    case LTE_RRC_SERVICE_IND:
    case LTE_RRC_BARRING_UPDATE_IND:
    case LTE_RRC_PESUDO_RESEL_LOW_PRIORITY_IND:
    case LTE_RRC_CONN_REL_IND:
    case LTE_RRC_DL_DATA_IND:
    case LTE_RRC_PAGE_IND:
    case LTE_RRC_ACTIVE_EPS_BEARER_UPDATE_IND:
    case LTE_RRC_CONN_EST_TIMER_UPDATE_IND:
    case LTE_RRC_PLMN_SEARCH_CNF:
    case LTE_RRC_IRAT_TUNNEL_UL_MSG_CNF:
#ifdef FEATURE_LTE_TO_HDR_OH
    case LTE_RRC_IRAT_HDR_TUNNEL_UL_MSG_CNF:
#endif
#ifdef FEATURE_LTE_TO_HDR_OH
    case LTE_RRC_IRAT_TUNNEL_DL_MSG_IND:
    case LTE_RRC_IRAT_HDR_TUNNEL_DL_MSG_IND:
#endif
    case LTE_RRC_1XCSFB_HO_STARTED_IND:
    case LTE_RRC_1XCSFB_HO_FAILED_IND:
    case LTE_RRC_1XSRVCC_HO_STARTED_IND:
    case LTE_RRC_1XSRVCC_HO_FAILED_IND:
    case LTE_RRC_CCO_NACC_COMPLETED_IND:
#ifdef FEATURE_FEMTO_CSG
    case LTE_RRC_CSG_INFO_UPDATE_IND:
#endif 
    case LTE_RRC_SIB6_INFO_IND:
    case LTE_RRC_SIB7_INFO_IND:
    case LTE_RRC_ALL_TRM_PRIO_LOWERED_IND:
#ifdef FEATURE_DUAL_DATA
    case LTE_RRC_CDRX_STATUS_IND:
#endif
      if(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id == LTE_RRC_ACTIVATION_IND)
      {
        mm_msg_info = (byte)rrc_msgr_cmd->rrc_msgr_cmd.rrc_act_ind.act_reason;
      }
      else if(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id == LTE_RRC_SERVICE_IND)
      {
        /*
        mm_msg_info : bit 0 :svc_status
                      bit 1 : acq_status
                      bit 4-2: no_svc_cause
        */
        mm_msg_info = (byte)(rrc_msgr_cmd->rrc_msgr_cmd.rrc_service_ind.svc_status);
        mm_msg_info = (mm_msg_info|(((rrc_msgr_cmd->rrc_msgr_cmd.rrc_service_ind.acq_status)&0x01)<<1));
        mm_msg_info = (mm_msg_info|(((rrc_msgr_cmd->rrc_msgr_cmd.rrc_service_ind.no_svc_cause)&0x07)<<2));
      }
      else if(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id == LTE_RRC_BARRING_UPDATE_IND)
      {
        mm_msg_info = rrc_msgr_cmd->rrc_msgr_cmd.rrc_barring_update_ind.emergency_access_barred;
      }
      else if(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id == LTE_RRC_CONN_EST_CNF)
      {
        mm_msg_info = (byte)rrc_msgr_cmd->rrc_msgr_cmd.rrc_conn_est_cnf.est_status;
      }      
      else if(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id == LTE_RRC_UL_DATA_CNF)
      {
        mm_msg_info = (byte)rrc_msgr_cmd->rrc_msgr_cmd.rrc_ul_data_cnf.status;
      }
      break;  

    default:
      break ;
  }
}

/*===========================================================================

FUNCTION EMM_PROCESS_IRAT_OUTGOING_ESM_MESSAGES

DESCRIPTION
  This function overrides the function emm_process_outgoing_esm_messages()
  It is called only during IRAT

DEPENDENCIES
  None

RETURN VALUE
  TRUE

SIDE EFFECTS
  None

===========================================================================*/
boolean emm_process_irat_outgoing_esm_message
(  
  void                  *emm_message_router_cmd,
  emm_ctrl_data_type    *emm_ctrl_data_ptr,
  mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr
)
{
  emm_cmd_type                        *emm_msgr_cmd = NULL; 
  log_lte_nas_emm_incoming_event_type  event;
  emm_failure_type                     emm_failure_cause;

  ASSERT(emm_message_router_cmd != NULL);
  ASSERT(emm_ctrl_data_ptr != NULL); 
  /*initialise it*/
  memset((void *)&emm_failure_cause,0,sizeof(emm_failure_type));

  emm_msgr_cmd = (emm_cmd_type *)emm_message_router_cmd; 

  MSG_HIGH_DS_1(MM_SUB ,"=EMM= Rcved ESM msg - %d during L->GW RAT change",
           emm_msgr_cmd->emm_msgr_cmd.msg_hdr.id);

  switch(emm_msgr_cmd->emm_msgr_cmd.msg_hdr.id)
  {
    case NAS_EMM_SERVICE_REQ:
      emm_failure_cause.cause_type = LTE_NAS_IRAT_INVALID_STATE;
      emm_build_and_send_failure_ind(LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE, 
                                     LTE_NAS_NO_FAILURE, 
                                     emm_msgr_cmd->emm_msgr_cmd.emm_service_req.trans_id,
                                     emm_failure_cause);
      event.emm_in_msg = (dword)NAS_EMM_SERVICE_REQ;
      break ;
    case NAS_EMM_DATA_REQ:
      emm_failure_cause.cause_type = LTE_NAS_IRAT_INVALID_STATE;
      emm_build_and_send_failure_ind(LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE, 
                                     LTE_NAS_NO_FAILURE, 
                                     emm_msgr_cmd->emm_msgr_cmd.emm_service_req.trans_id,
                                     emm_failure_cause);
      event.emm_in_msg = (dword)NAS_EMM_DATA_REQ;
      break ;
    case NAS_EMM_DETACH_CMD:
      /* Need revist */
      event.emm_in_msg = (dword)NAS_EMM_DETACH_CMD;
      break;
    case NAS_EMM_EPS_BEARER_STATUS_CMD:
      emm_process_esm_eps_bearer_status_cmd(
        (emm_eps_bearer_status_cmd_T *)&emm_msgr_cmd->emm_msgr_cmd,
                                        emm_ctrl_data_ptr);
      event.emm_in_msg = (dword)NAS_EMM_EPS_BEARER_STATUS_CMD;
      break;
#ifdef FEATURE_LTE_REL9
    case NAS_EMM_EMC_SRV_STATUS_CMD:
      emm_process_emc_srv_status_cmd(
      &emm_msgr_cmd->emm_msgr_cmd.emm_emc_srv_status_cmd, emm_ctrl_data_ptr);
      event.emm_in_msg = (dword)NAS_EMM_EMC_SRV_STATUS_CMD;
      break;
#endif
#ifdef FEATURE_DUAL_SIM
    case NAS_EMM_PS_CALL_STATUS_CMD:
      emm_process_esm_ps_call_status_cmd(&emm_msgr_cmd->emm_msgr_cmd.emm_ps_call_status_ind,emm_ctrl_data_ptr);
      break;
#endif

    default:
      event.emm_in_msg = (dword)emm_msgr_cmd->emm_msgr_cmd.msg_hdr.id;
      MSG_ERROR_DS(MM_SUB,"=EMM= Unknown ESM message - %d",
          emm_msgr_cmd->emm_msgr_cmd.msg_hdr.id,0,0);
      break ;
  }

  event_report_payload(EVENT_LTE_EMM_INCOMING_MSG,
                       sizeof(event), 
                       (void*)&event);

  return TRUE;

}/* emm_process_irat_outgoing_esm_message */

/*===========================================================================

FUNCTION EMM_DISCARD_OOS_RLF_LTE_ESM_PRIMITIVE

DESCRIPTION
  This function prints out the LTE ESM messages that are being discarded (this is for debugging purposes only)

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void emm_discard_oos_rlf_lte_esm_primitive( emm_cmd_type  *emm_msgr_cmd_ptr)
{
  ASSERT(emm_msgr_cmd_ptr != NULL);

  switch(emm_msgr_cmd_ptr->emm_msgr_cmd.msg_hdr.id)
  {
    case NAS_EMM_1XCSFB_ESR_CALL_ABORT_REQ:
      MSG_ERROR_DS(MM_SUB,"NAS_EMM_1XCSFB_ESR_CALL_ABORT_REQ is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    default:
      MSG_ERROR_DS(MM_SUB,"=EMM= Unknown ESM message",0,0,0) ;
      break ;
  }
}

/*===========================================================================

FUNCTION EMM_PROCESS_OOS_RLF_OUTGOING_ESM_MESSAGES

DESCRIPTION
  This function overrides the function emm_process_outgoing_esm_messages()
  It is called only during OOS/RLF

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
boolean emm_process_oos_rlf_outgoing_esm_message 
(  
  void                  *emm_message_router_cmd,
  emm_ctrl_data_type    *emm_ctrl_data_ptr,
  mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr
)
{
  emm_cmd_type   *emm_msgr_cmd_ptr = NULL; 
  emm_state_type  emm_state    = EMM_INVALID_STATE ;
  log_lte_nas_emm_incoming_event_type  event;
  emm_failure_type                     emm_failure_cause;

  ASSERT(emm_message_router_cmd != NULL);
  ASSERT(emm_ctrl_data_ptr != NULL);

  /*initialise it*/
  memset((void *)&emm_failure_cause,0,sizeof(emm_failure_type));

  emm_msgr_cmd_ptr = (emm_cmd_type *)emm_message_router_cmd;

  MSG_HIGH_DS_1(MM_SUB ,"=EMM= Rcved ESM msg - %d during OOS/RLF",
           emm_msgr_cmd_ptr->emm_msgr_cmd.msg_hdr.id);

  switch(emm_msgr_cmd_ptr->emm_msgr_cmd.msg_hdr.id)
  {
    case NAS_EMM_SERVICE_REQ:
      emm_failure_cause.cause_type = LTE_NAS_IRAT_INVALID_STATE;
      emm_build_and_send_failure_ind(LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE, 
                                     LTE_NAS_NO_FAILURE, 
                                     emm_msgr_cmd_ptr->emm_msgr_cmd.emm_service_req.trans_id,
                                     emm_failure_cause);
      break ;
    case NAS_EMM_DATA_REQ:
      emm_failure_cause.cause_type = LTE_NAS_IRAT_INVALID_STATE;
      emm_build_and_send_failure_ind(LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE, 
                                     LTE_NAS_NO_FAILURE, 
                                     emm_msgr_cmd_ptr->emm_msgr_cmd.emm_service_req.trans_id,
                                     emm_failure_cause);
      break ;
    case NAS_EMM_DETACH_CMD:
      emm_state = EMM_GET_STATE();
      switch(emm_state)        
      {
        case EMM_DEREGISTERED:
        case EMM_INVALID_STATE:
          MSG_HIGH_DS_0(MM_SUB,"=EMM= received NAS_EMM_DETACH_CMD when EMM is in EMM_DEREGISTERED state, this message shall be discarded");
          break;
        case EMM_NULL:
          MSG_ERROR_DS(MM_SUB,"=EMM= is in EMM_NULL state, unable to locally deactivate EMM, this case is unexpected",0,0,0);
          break;
        default:
          MSG_HIGH_DS_0(MM_SUB,"=EMM= received NAS_EMM_DETACH_CMD during RLF/OOS, trying to locally deactivate EMM");
          if(emm_ctrl_data_ptr->plmn_service_state->service_status == 
                                                            SYS_SRV_STATUS_SRV)
          {
            emm_ctrl_data_ptr->plmn_service_state->service_status = 
                                                                SYS_SRV_STATUS_LIMITED;
          }
          emm_ctrl_data_ptr->plmn_service_state->reg_service_domain = SYS_SRV_DOMAIN_NO_SRV;
          emm_ctrl_data_ptr->srv_domain_registered = SYS_SRV_DOMAIN_NO_SRV;
          emm_build_and_send_detach_ind(EMM_DETACHED);  
          EMM_MOVES_TO_DEREGISTERED_STATE(EMM_DEREGISTERED_NO_CELL_AVAILABLE) ; 
          if (emm_ctrl_data_ptr->rrc_active != TRUE)
          {
            /*Forcefully gmm state to dereg when CM send PDN disconnect and LTE is not active*/
            gmm_set_state(GMM_DEREGISTERED);
          }
          MSG_HIGH_DS_0(MM_SUB,"=EMM= is now locally deactivated");
          break;
      } 
      break;
    case NAS_EMM_EPS_BEARER_STATUS_CMD:
      emm_process_esm_eps_bearer_status_cmd(
        (emm_eps_bearer_status_cmd_T *)&emm_msgr_cmd_ptr->emm_msgr_cmd,
                                        emm_ctrl_data_ptr);
      event.emm_in_msg = (dword)NAS_EMM_EPS_BEARER_STATUS_CMD;
      event_report_payload(EVENT_LTE_EMM_INCOMING_MSG, 
                           sizeof(event), (void*)&event);
      break;
#ifdef FEATURE_LTE_REL9
    case NAS_EMM_EMC_SRV_STATUS_CMD:
      emm_process_emc_srv_status_cmd(
                        &emm_msgr_cmd_ptr->emm_msgr_cmd.emm_emc_srv_status_cmd,
                         emm_ctrl_data_ptr);
      event.emm_in_msg = (dword)NAS_EMM_EMC_SRV_STATUS_CMD;
      event_report_payload(EVENT_LTE_EMM_INCOMING_MSG,sizeof(event), (void*)&event);
      break;
#endif
#ifdef FEATURE_DUAL_SIM
    case NAS_EMM_PS_CALL_STATUS_CMD:
      emm_process_esm_ps_call_status_cmd(&emm_msgr_cmd_ptr->emm_msgr_cmd.emm_ps_call_status_ind,emm_ctrl_data_ptr);
      break;
#endif
    case NAS_EMM_1XCSFB_ESR_CALL_REQ:
      emm_build_and_send_esr_failure_ind(AS_REJECT_CAUSE,AS_REJ_LRRC_LOWER_LAYER_FAILURE,emm_msgr_cmd_ptr->emm_msgr_cmd.emm_1xCSFB_esr_call_req.trans_id);
      break;
    default:
      emm_discard_oos_rlf_lte_esm_primitive(emm_msgr_cmd_ptr);
      break;
  }
  return TRUE;
}

/*===============================================================================

FUNCTION  EMM_GET_REG_REQ_PENDING_STATUS

DESCRIPTION
  
DEPENDENCIES
  None

RETURN VALUE
  returns the reg request pending status

SIDE EFFECTS
  None

=================================================================================*/
boolean emm_get_reg_req_pending_status
(
  void
)
{
  return mmcoord_reg_req_pending; 
}

/*===========================================================================

FUNCTION EMM_DISCARD_OOS_RLF_LTE_RRC_PRIMITIVE

DESCRIPTION
  This function prints out the LTE RRC messages that are being discarded (this is for debugging purposes only)

DEPENDENCIES
  None

RETURN VALUE
  NONE

SIDE EFFECTS
  None

===========================================================================*/
void emm_discard_oos_rlf_lte_rrc_primitive(emm_rrc_cmd_type *rrc_msgr_cmd)
{
  emm_ctrl_data_type		  *emm_ctrl_data_ptr;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != (emm_ctrl_data_type *)NULL);
  ASSERT(rrc_msgr_cmd != NULL);

  switch (rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id)
  {
    case LTE_RRC_UL_DATA_CNF:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_UL_DATA_CNF is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break ;
    case LTE_RRC_ACTIVATION_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_ACTIVATION_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break ;
    case LTE_RRC_NAS_UMTS_KEY_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_NAS_UMTS_KEY_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_NAS_GSM_KEY_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_NAS_GSM_KEY_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
   case LTE_RRC_NAS_LTE_KEY_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_NAS_LTE_KEY_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_DL_DATA_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_DL_DATA_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      emm_free_dsm_memory_in_rrc_data_ind((lte_rrc_dl_data_ind_s *)\
                                          &(rrc_msgr_cmd->rrc_msgr_cmd.rrc_dl_data_ind));
      break ;
    case LTE_RRC_PAGE_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_PAGE_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break ;
    case LTE_RRC_ACTIVE_EPS_BEARER_UPDATE_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_ACTIVE_EPS_BEARER_UPDATE_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break ;
    case LTE_RRC_CONN_EST_TIMER_UPDATE_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_CONN_EST_TIMER_UPDATE_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break ;
    case LTE_RRC_PLMN_SEARCH_CNF:
       emm_ctrl_data_ptr->plmn_search_in_progress = FALSE;/*RESET PLMN SEARCH state if search was aborted by LRRC*/
       MSG_ERROR_DS(MM_SUB,"LTE_RRC_PLMN_SEARCH_CNF is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_IRAT_TUNNEL_UL_MSG_CNF:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_IRAT_TUNNEL_UL_MSG_CNF is received during OOS/RLF, this message shall be currently discarded",0,0,0);
#ifdef FEATURE_LTE_TO_HDR_OH
    case LTE_RRC_IRAT_HDR_TUNNEL_UL_MSG_CNF:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_IRAT_HDR_TUNNEL_UL_MSG_CNF is received during OOS/RLF, this message shall be currently discarded",0,0,0);
#endif
      break;
    case LTE_RRC_IRAT_TUNNEL_DL_MSG_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_IRAT_TUNNEL_DL_MSG_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
#ifdef FEATURE_LTE_TO_HDR_OH
    case LTE_RRC_IRAT_HDR_TUNNEL_DL_MSG_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_IRAT_HDR_TUNNEL_DL_MSG_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
#endif
    case LTE_RRC_1XCSFB_HO_STARTED_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_1XCSFB_HO_STARTED_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_1XCSFB_HO_FAILED_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_1XCSFB_HO_FAILED_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_1XSRVCC_HO_FAILED_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_1XSRVCC_HO_FAILED_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_1XSRVCC_HO_STARTED_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_1XSRVCC_HO_STARTED_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_SIB6_INFO_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_SIB6_INFO_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_SIB7_INFO_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_SIB7_INFO_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
    case LTE_RRC_ALL_TRM_PRIO_LOWERED_IND:
      MSG_ERROR_DS(MM_SUB,"LTE_RRC_ALL_TRM_PRIO_LOWERED_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
      break;
#ifdef FEATURE_DUAL_DATA
     case LTE_RRC_CDRX_STATUS_IND:
       MSG_ERROR_DS(MM_SUB,"LTE_RRC_CDRX_STATUS_IND is received during OOS/RLF, this message shall be currently discarded",0,0,0);
       break;
#endif
    default:
      MSG_ERROR_DS(MM_SUB,"=EMM= Unknown RRC primitive",0,0,0) ;
      break ;
  }
}

//#ifdef FEATURE_LTE
/*===========================================================================

FUNCTION EMM_PROCESS_OOS_RLF_RRC_PRIMITIVE

DESCRIPTION
  This function overrides the function emm_process_rrc_primitive() in mmtask.c
  It is called only during IRAT

DEPENDENCIES
  None

RETURN VALUE
  TRUE: If SERV_CONF on LTE
  FALSE: iF N0_SERV on LTE

SIDE EFFECTS
  None

===========================================================================*/
/*lint -save -e550 Symbol 'dummy_return_val' not accessed */

boolean emm_process_oos_rlf_rrc_primitive
(
  void                  *rrc_message_router_cmd,
  emm_ctrl_data_type    *emm_ctrl_data_ptr,
  mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr
)
{
  emm_rrc_cmd_type   *rrc_msgr_cmd;

  boolean            new_rat_active = FALSE;
  emm_failure_type    emm_failure_cause;

  ASSERT(rrc_message_router_cmd != (void *)NULL);
  ASSERT(emm_ctrl_data_ptr != (emm_ctrl_data_type *)NULL);
  memset((void *)&emm_failure_cause,0,sizeof(emm_failure_type));
  rrc_msgr_cmd = (emm_rrc_cmd_type*)rrc_message_router_cmd;

  switch(rrc_msgr_cmd->rrc_msgr_cmd.msg_hdr.id)
  {
  /*************************************************
              PROCESS LTE_RRC MESSAGES
  *************************************************/
  case LTE_RRC_CONN_EST_CNF:
    emm_process_connection_confirmation(&rrc_msgr_cmd->rrc_msgr_cmd.rrc_conn_est_cnf,
                                        emm_ctrl_data_ptr);
    break ;
  case LTE_RRC_CONN_REL_IND: 
    MSG_HIGH_DS_0(MM_SUB,"=EMM= Received LTE_RRC_CONN_REL_IND during OOS/RLF");
    (void)emm_process_rrc_primitive(rrc_msgr_cmd,emm_ctrl_data_ptr,mm_emm_dispatcher_fn_ptr);     
    break;
  case LTE_RRC_DEACTIVATE_CNF:
    /*Reset the MM_SEND_MMR_STOP_MODE_CNF_FP to point to MM_SEND_MMR_STOP_MODE_CNF() function*/
    emm_ctrl_data_ptr->mm_send_mmr_stop_mode_cnf_fp = mm_send_mmr_stop_mode_cnf;
    emm_ctrl_data_ptr->ignore_GW_activation_ind = FALSE;
    emm_ctrl_data_ptr->rrc_deact_req.deact_cnf_pending = FALSE;
    emm_ctrl_data_ptr->last_reg_cause = MMR_CAUSE_NONE;
	emm_ctrl_data_ptr->is_syncup_tau_for_local_bear_deact_needed = FALSE;
    MSG_HIGH_DS_0(MM_SUB,"=EMM= Received LTE_RRC_DEACTIVATE_CNF");
    if(emm_ctrl_data_ptr->rlf_state == FALSE)
    {
      /* Cancel authentication in LTE NAS as LTE has been deactivated */
      emm_cancel_authentication(); 
      if(emm_ctrl_data_ptr->ongoing_detach_procedure == TRUE)
      {
        emm_ps_detach_clean_up(emm_ctrl_data_ptr);
      }
    }
#ifdef FEATURE_SGLTE
    if(MM_IS_IN_SGLTE_MODE &&
       (mm_sglte_current_stack_status == SGLTE_MM_UPPER_LAYER_MERGING) && 
       (((mmcoord_reg_req_pending == TRUE) &&
        (mmcoord_pending_reg_message.cmd.mmr_reg_req.type != MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY))||
       (emm_ctrl_data_ptr->reg_req_pended_indication == TRUE )||
       (mm_pended_stop_mode_req.valid == TRUE)||(mm_plmn_list_req_pending == TRUE)||
       (mm_ps_detach_state == MM_PS_DETACH_STATE_DETACH_REQ_PENDING)))
    {
       /* if REG overrides merge, merge context before processing new REG command  */
       mmcoord_process_pstimer_expiry_merging();
    }
#endif
    if(emm_ctrl_data_ptr->ps_detach_info.status == EMM_PS_DETACH_REQ_PENDING)
    {
      emm_process_ps_detach_req(emm_ctrl_data_ptr->ps_detach_info.ps_detach_req, 
                                emm_ctrl_data_ptr);
    }
    else if(emm_ctrl_data_ptr->ps_detach_info.status == 
                                                     EMM_PS_DETACH_CNF_PENDING)
    {
      emm_ps_detach_clean_up(emm_ctrl_data_ptr);  
    }

    /*If there is a pending SERVICE_CNF, that means this DEACTIVATE_CNF is for the DEACTIVATE_REQ sent 
      from EMM (as soon as SERVICE_CNF for G/W was received")*/
    if(mmcoord_svc_cnf_pending)
    {
      /*Route all incoming messages to normal_msg_handler, now new RAT is ACTIVE*/
      *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler; 
#ifdef FEATURE_DUAL_SIM
      mm_update_rat_status(&mm_rat_pri_list_info ,SYS_RAT_ACTIVATION_STATUS_DEACTIVATED);
#endif
      mmcoord_route_mm_message(&mmcoord_pending_svc_cnf_message,mm_emm_dispatcher_fn_ptr); 
      mmcoord_svc_cnf_pending = FALSE;
      break;
    }

#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(mm_rlf_stop_mode_pending == MM_WCDMA_RLF_STOP_MODE_PENDING)
    {
      MSG_HIGH_DS_0(MM_SUB,"Stop mode confirmation is pending ignoring LTE_RRC_DEACTIVATE_CNF");
    }
    else
#endif
    /*If there is a pending stop mode req, that means this DEACTIVATE_CNF is for the DEACTIVATE_REQ sent 
      from EMM (this will be observed in lte RLF when active rat is not LTE and stop mode request is received")*/
    if(mm_rlf_stop_mode_pending == MM_LTE_RLF_STOP_MODE_PENDING)
    {
      /*Route all incoming messages to normal_msg_handler, now new RAT is ACTIVE*/
      *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
      (void)mmcoord_route_mm_emm_messages(&mmcoord_pending_reg_message, emm_ctrl_data_ptr, mm_emm_dispatcher_fn_ptr); 
      mm_rlf_stop_mode_pending = MM_RLF_STOP_MODE_PENDING_NONE;
      break;
    }

    /*If there is a pending REG_REQ, that means this DEACTIVATE_CNF is for the DEACTIVATE_REQ sent 
      from EMM (as soon as REG_REQ for G/W was received")*/
    else if((mmcoord_reg_req_pending == TRUE) || 
            (mm_plmn_list_req_pending == TRUE) ||
            (mm_pended_stop_mode_req.valid == TRUE)||
            (mm_ps_detach_state == MM_PS_DETACH_STATE_DETACH_REQ_PENDING))
    {
      MSG_HIGH_DS_2(MM_SUB,"=MM= Processing pending REG_REQ for G/W/TDS =%d ,PLMN Search Req= %d ",
                                                         mmcoord_reg_req_pending,mm_plmn_list_req_pending);
#if defined FEATURE_WRLF_SYSTEM_SEL|| defined FEATURE_SGLTE
      if( mm_wcdma_rlf_state == WCDMA_RLF_STARTED
#ifdef FEATURE_SGLTE
          ||(mm_sglte_current_stack_status == SGLTE_MM_UPPER_LAYER_MERGING)
          ||((mmcoord_pending_reg_message.cmd.mmr_reg_req.type == MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY) &&
               (mmcoord_reg_req_pending == TRUE))
#endif
        )
      {
        *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
      }
#endif

      emm_service_loss_on_lte();

      /* UE goes to GW. Nee to set EMM substate to NO CELL AVAILABLE. */
      if(EMM_GET_STATE() == EMM_DEREGISTERED)
      {
        EMM_MOVES_TO_DEREGISTERED_STATE(EMM_DEREGISTERED_NO_CELL_AVAILABLE);
      }
      else if(EMM_GET_STATE() == EMM_REGISTERED)
      {
        EMM_MOVES_TO_REGISTERED_STATE(EMM_REGISTERED_NO_CELL_AVAILABLE);
      }
      else
      {
        MSG_HIGH_DS_1(MM_SUB,"=EMM=  - EMM state %d",EMM_GET_STATE());
      }

        /* If UE is in LTE RLF and stop mode reqason is alt scan then donot clean the message buffer */
      if((emm_ctrl_data_ptr->rrc_deact_req.deact_reason != SYS_STOP_MODE_REASON_ALT_SCAN) ||
         (EMM_GET_CONNECTION_STATE() != EMM_CONNECTED_STATE))
      {
        /* LTE stack is deactivated. Clean up message buffer. */
        MSG_HIGH_DS_1(MM_SUB ,"=EMM= LTE stack is deactivated, clean up message buffer, stop_mode_reason %d",
                                                        emm_ctrl_data_ptr->rrc_deact_req.deact_reason);
        emm_clean_msg_buffer(emm_ctrl_data_ptr);
      }

      if(emm_ctrl_data_ptr->rlf_state == FALSE)
      {
        /* Leaving LTE. Clean up PDN CONN REQ for ATTACH if necessary */
		emm_failure_cause.cause_type = LTE_NAS_IRAT_INVALID_STATE;
        emm_release_attach_pdn_conn_req(emm_ctrl_data_ptr, emm_failure_cause);
        /* Free the memory allocated dynamically */
        emm_procedure_message_mem_free(emm_ctrl_data_ptr);
      }

      if(mmcoord_reg_req_pending == TRUE )
      {
#ifdef FEATURE_DUAL_SIM
        if((mmcoord_pending_reg_message.cmd.hdr.message_set == MS_MM_REG) &&
           (mmcoord_pending_reg_message.cmd.hdr.message_id == MMR_REG_REQ))
        {
          mm_update_rat_status(&mmcoord_pending_reg_message.cmd.mmr_reg_req.rat_pri_list_info,
                                                           SYS_RAT_ACTIVATION_STATUS_DEACTIVATED);
        }
#endif
        mmcoord_route_mm_message(&mmcoord_pending_reg_message,mm_emm_dispatcher_fn_ptr); 
      }
      else if (mm_pended_stop_mode_req.valid)
      {
        *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
        mmcoord_pending_reg_message.cmd.mmr_stop_mode_req= mm_pended_stop_mode_req.mmr_stop_mode_req;
        mmcoord_route_mm_message(&mmcoord_pending_reg_message,mm_emm_dispatcher_fn_ptr); 
      }
      else if(mm_ps_detach_state == MM_PS_DETACH_STATE_DETACH_REQ_PENDING)
      {
        *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
        if(mm_pending_mmr_ps_detach_req_ptr != NULL)
        {
           mmcoord_process_mmr_ps_detach_req(mm_pending_mmr_ps_detach_req_ptr);
           modem_mem_free(mm_pending_mmr_ps_detach_req_ptr,MODEM_MEM_CLIENT_NAS);
           mm_pending_mmr_ps_detach_req_ptr = NULL;
        }
      }
      else /* list req pending */
      {
        *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
        mmcoord_route_mm_message(&mmcoord_pending_reg_list_req_message,mm_emm_dispatcher_fn_ptr);
        mm_serving_plmn.info.service_status = SYS_SRV_STATUS_NO_SRV;
      }
    }
    else if(emm_ctrl_data_ptr->reg_req_pended_indication == TRUE)
    {
#ifdef FEATURE_SGLTE
      /* if this service request was received while merge was in progress, allow split before processing service request*/
      if (MM_IS_IN_SGLTE_MODE && is_gsm_mode())
      {
        process_rat_change_req(emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr,
                               mm_emm_dispatcher_fn_ptr);
      }
      else
#endif
      if(is_gsm_mode() || is_umts_mode())
      {
        process_rat_change_req(emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr,
                                mm_emm_dispatcher_fn_ptr);
      }
      else
      {
        MSG_HIGH_DS_0(MM_SUB,"=EMM= Reset reg_req_pended_indication to FALSE");
        emm_process_reg_command(emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr,emm_ctrl_data_ptr);  
        /*Reset the message handler function to point to the normal message handler*/
        *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;  
      }
    }
    else if(emm_ctrl_data_ptr->pending_reg_cmd_ptr != NULL)
    {
      emm_ctrl_data_ptr->plmn_service_state->service_status = 
                                                         SYS_SRV_STATUS_NO_SRV;

      /* Process the pending cmd received during RRC deactivation */
      switch(emm_ctrl_data_ptr->pending_reg_cmd_ptr->cmd.hdr.message_id)
      {
        case MMR_PLMN_SEARCH_REQ:
          emm_process_reg_command(emm_ctrl_data_ptr->pending_reg_cmd_ptr,
                                  emm_ctrl_data_ptr);
          break;
        case MMR_PLMN_SEARCH_ABORT_REQ:
          emm_send_empty_plmnsearch_cnf(
             &emm_ctrl_data_ptr->pending_reg_cmd_ptr->cmd.mmr_plmn_search_req,
             REG_MM_PLMN_SEARCH_ABORTED);
          reg_service_report(MMR_CAUSE_NOT_SPECIFIED,FALSE,emm_ctrl_data_ptr);
          break;
        default:
          MSG_HIGH_DS_1(MM_SUB ,"=MM= Discarding unexpected pending REG cmd 0x%x", 
               emm_ctrl_data_ptr->pending_reg_cmd_ptr->cmd.hdr.message_id);
          break;
      }

      /* Free the memory */
      emm_modem_mem_free(emm_ctrl_data_ptr->pending_reg_cmd_ptr,
                              emm_ctrl_data_ptr);
      emm_ctrl_data_ptr->pending_reg_cmd_ptr = NULL;
      
      /* Go back to normal handler */
      *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
    }
    else /*This case is possible only when neither RATs are active and similar to initial bring up...*/
    {
      /*Redirect all incoming messages to go thru mm_emm_normal_msg_handler,
      as OOS/RLF scenario is no longer valid*/
      *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
      (void) emm_process_rrc_primitive(rrc_msgr_cmd,emm_ctrl_data_ptr,mm_emm_dispatcher_fn_ptr);
    }
    break;
  case LTE_RRC_SERVICE_IND: 

#ifdef FEATURE_WRLF_SYSTEM_SEL
    if(mm_rlf_stop_mode_pending == MM_WCDMA_RLF_STOP_MODE_PENDING)
    {
      MSG_HIGH_DS_0(MM_SUB,"Stop mode confirmation is pending ignoring LTE_RRC_SERVICE_IND");
    }
    else
#endif
    {
      /*Check for RAT change conf, if so, send the conf back to REG and quit this loop*/           
      /*-------------------------------------------
             Verify if LTE_RRC sent SERV_CONF
      -------------------------------------------*/
      if(rrc_msgr_cmd->rrc_msgr_cmd.rrc_service_ind.svc_status == LTE_RRC_SVC_STATUS_AVAILABLE)
      {
        /*-------------------------------------------
                    GW_LOCAL_DETACH
        -------------------------------------------*/            
        /*------------- SET THE BOOLEAN VALUE AND HENCE COME OUT OF THE WHILE LOOP in MMCOORD.C ---------*/
        new_rat_active = TRUE;
      }
      (void)emm_process_rrc_primitive(rrc_msgr_cmd,emm_ctrl_data_ptr,mm_emm_dispatcher_fn_ptr);
    }
    break;

  case LTE_RRC_CCO_NACC_COMPLETED_IND: 
    (void) emm_process_rrc_primitive(rrc_msgr_cmd,emm_ctrl_data_ptr,mm_emm_dispatcher_fn_ptr);
    break;

  case LTE_RRC_BARRING_UPDATE_IND:
  case LTE_RRC_PESUDO_RESEL_LOW_PRIORITY_IND:
    (void) emm_process_rrc_primitive(rrc_msgr_cmd,emm_ctrl_data_ptr,mm_emm_dispatcher_fn_ptr);
    break;

  default:
    emm_discard_oos_rlf_lte_rrc_primitive(rrc_msgr_cmd);   
    break;
  }
  return new_rat_active;
}
#endif //FEATURE_LTE

/*lint -restore */

/*===============================================================================

FUNCTION PROCESS_RAT_CHANGE_REQ

DESCRIPTION
  This function does all the necessary processing when the RAT changes. 
DEPENDENCIES
  None

RETURN VALUE
NONE:

WHEN WILL THIS FUNCTION COME OUT OF PROCESSING?
*********************************************************************************
There are 2 situations:

In OOS:
After MMR_REG_REQ for new RAT is processed, it is the responsibility 
of the main MM_TASK to handle and route all incoming messages from there on,so 
exit the IRAT process block and hence the control goes back to main MM TASK            

In RLF:

*********************************************************************************

SIDE EFFECTS
  None

=================================================================================*/
#ifdef FEATURE_LTE
void process_rat_change_req(mm_cmd_type *cmd_ptr,mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr)
{
  emm_failure_type emm_failure_cause;
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
  emm_state_type  emm_state = EMM_INVALID_STATE;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();

  memset((void *)&emm_failure_cause,0,sizeof(emm_failure_type));

  ASSERT(emm_ctrl_data_ptr != NULL);
  ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
  ASSERT(cmd_ptr != (mm_cmd_type *)NULL);
  switch(cmd_ptr->cmd.hdr.message_set)
  {
    case MS_MM_REG:
      switch(cmd_ptr->cmd.hdr.message_id)
      {
        case MMR_REG_REQ: // RAT CHANGE DUE TO OOS/RLF   
          if(cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_LTE_RADIO_ACCESS) /*LTE is requested, stop GW*/
          {
            if ((mm_rat_handler_status == MM_RAT_STOP_GSM_IN_PROGRESS) 
                || (mm_rat_handler_status == MM_RAT_STOP_WCDMA_IN_PROGRESS)
                || (mm_rat_handler_status == MM_RAT_STOP_TDSCDMA_IN_PROGRESS)
                || (mm_deep_sleep_pending))
            {
              /*Lower layer rat change procedure between g and w is going on and MM receives reg request to 
              activate lte pend the req. */
              MSG_HIGH_DS_0(MM_SUB,"=EMM= Buffering MMR_REG_REQ received during W2G IRAT");
              mm_irat_pending_reg_cmd_ptr_mem_alloc(cmd_ptr);
            }
            else
            {
              mm_reg_waiting_for_reg_cnf = FALSE;
              mm_serv_change_pending = FALSE;
              mm_reject_call_on_rai_change = FALSE;
#ifdef FEATURE_SGLTE
              if(MM_SUB_IS_SGLTE)
              {
              MSG_HIGH_DS_3(MM_SUB, "=MM= Process rat change  network_selection_mode = %d ,mmr_reg_req.type = %d  in state4 = %d  ", cmd_ptr->cmd.mmr_reg_req.network_selection_mode,
                                                          cmd_ptr->cmd.mmr_reg_req.type,MM_IS_IN_SGLTE_STATE_4);
              }
             if(MM_IS_IN_SGLTE_STATE_4 && 
                 (cmd_ptr->cmd.mmr_reg_req.is_split_needed == TRUE)) 
             {
                mm_if_needed_abort_plmn_search_and_loop_back_to_reg( cmd_ptr->cmd.mmr_reg_req.type);
                MSG_HIGH_DS_0(MM_SUB,"=MM= MMR_REG_REQ_SGLTE_PS_ONLY_ACQUISITION  splitting.  ");
                mm_sglte_current_stack_status  = SGLTE_MM_UPPER_LAYER_SPLITING ;
                mm_copy_cs_database(MM_SGLTE_STACK_2, mm_sub_id);
               /*send act indication due to new design*/      
                mmsend_mmr_stack_act_ind();
               (void)mmcoord_stop_gw(cmd_ptr);
             }
             else
#endif 
              if (((rrc_connection_status == MMRRC_ACTIVE) || (rrc_connection_status == MMRRC_EST_IN_PROGRESS)) 
  #ifdef FEATURE_WRLF_SYSTEM_SEL
                   && (mm_wcdma_rlf_state == WCDMA_RLF_IDLE)
  #endif
                 )
              {
                if(mm_in_conn_management_state() == TRUE) 
                {

                     if((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode== SYS_NETWORK_SELECTION_MODE_AUTOMATIC &&
                          cmd_ptr->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL) ||
                         (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode== SYS_NETWORK_SELECTION_MODE_MANUAL &&
                          cmd_ptr->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_AUTOMATIC))
                     {
                       mm_send_nw_sel_mode_reset_ind(cmd_ptr->cmd.mmr_reg_req.network_selection_mode, cmd_ptr->cmd.hdr.message_id);
                     } 

                     MSG_HIGH_DS_0(MM_SUB, "=MM= Reject REG_REQ - CS call or PS Call in-progress");
                     mm_reg_req_revd_active_call = TRUE; 
                     if((mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode !=
                                                     SYS_NETWORK_SELECTION_MODE_MANUAL) ||
                      (mm_plmn_match(mm_serving_plmn.info.plmn, cmd_ptr->cmd.mmr_reg_req.plmn )))
                     {
                       mmcoord_reject_mmr_reg_req();
                     }
                     else
                     {
                       mm_reg_waiting_for_reg_cnf = TRUE;
                       mm_reg_req_trans_id--;
                     }
                     return;
                }
                else
                {
                  mmcoord_abort_rr_connection(cmd_ptr->cmd.mmr_reg_req.type);
                }
              }  
#if defined(FEATURE_WCDMA) && defined(FEATURE_WRLF_SYSTEM_SEL)
              else if ((rrc_connection_status != MMRRC_IDLE) &&
			  	       (mm_wcdma_rlf_state == WCDMA_RLF_STARTED) &&
			  	       (mm_rat_pri_list_match(&cmd_ptr->cmd.mmr_reg_req.rat_pri_list_info,
                                                      &mm_rat_pri_list_info) == FALSE) &&
                       (cmd_ptr->cmd.mmr_reg_req.network_selection_mode != SYS_NETWORK_SELECTION_MODE_LIMITED) &&
                       (mm_last_requested_network_selection_mode != SYS_NETWORK_SELECTION_MODE_LIMITED))
              {
                mm_send_rrc_abort_req_during_wrlf();
              }
#endif
              else if ((rrc_connection_status == MMRRC_RELEASE_IN_PROGRESS)
#ifdef FEATURE_WRLF_SYSTEM_SEL
                       &&(mm_wcdma_rlf_state == WCDMA_RLF_IDLE)
#endif
              )
              {
                MSG_HIGH_1("EMM: Pending reg_req in connection state %d",rrc_connection_status);
              }
              else
              {
                /*Send STOP_MODE to GW*/
                (void)mmcoord_stop_gw(cmd_ptr);
              }
              emm_ctrl_data_ptr->reg_req_pended_indication = TRUE ;
              // Reset this flag as we will not be processing any pending reg_req in G/W now.
              mmcoord_reg_req_pending = FALSE;
              mmcoord_svc_cnf_pending = FALSE;
              memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr, sizeof(mmr_reg_req_s_type),&cmd_ptr->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
              /*Temp solution -- hnam revisit*/
            
              emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr = &mmcoord_pending_reg_message;     
              memscpy(emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr, sizeof(mm_cmd_type),cmd_ptr,sizeof(mm_cmd_type));
            }
          }
          else if((cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_GSM_RADIO_ACCESS)||/*G/W is requested, stop LTE*/
                  (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_TDS_RADIO_ACCESS) ||
                  (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_UMTS_RADIO_ACCESS))
          {
            /*Save the G/W REG_REQ in the pending list*/
#if defined(FEATURE_LTE)
            if(emm_ctrl_data_ptr->reg_req_pended_indication == TRUE)
            {
              MSG_HIGH_DS_0(MM_SUB,"=EMM= L2GW/TDS IRAT - Clearing pending L REG REQ");
              emm_reg_container = NULL;
              reg_service_report = (emm_reg_service_report_type)emm_send_reg_ind;
              emm_ctrl_data_ptr->reg_req_pended_indication = FALSE;
            }
            /* Save the reg req for EMM */
            memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr, sizeof(mmr_reg_req_s_type),
                                    &cmd_ptr->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
#endif
           mmcoord_pend_mmr_reg_req(cmd_ptr);

#ifdef FEATURE_SGLTE
            if (cmd_ptr->cmd.mmr_reg_req.type !=MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY)
#endif
            {           
              mm_reg_waiting_for_reg_cnf = TRUE;
            }
            /*Set the Function pointer "mm_send_mmr_stop_mode_cnf_fp" to NULL, 
            so that MM_SEND_MMR_STOP_MODE_CNF() function is not called during OOS/RLF*/
#ifdef FEATURE_SGLTE
  if(MM_SUB_IS_SGLTE)
  {
  MSG_HIGH_DS_2(MM_SUB, "=MM= Process RAT change .mmcoord_reg_req_pending = %d mmr_reg_req.type = %d   ", mmcoord_reg_req_pending,
                                                              cmd_ptr->cmd.mmr_reg_req.type);
  }
  if ((mmcoord_reg_req_pending == TRUE)&&MM_IS_IN_SGLTE_MODE)
   { 
     if(cmd_ptr->cmd.mmr_reg_req.type == MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY)
     {
       MSG_HIGH_DS_0(MM_SUB,"=MM= MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY  Merging....  ");
       mm_sglte_current_stack_status  = SGLTE_MM_UPPER_LAYER_MERGING;      
       mmsend_mmr_stack_deact_ind();
       emm_ctrl_data_ptr->last_rrc_service_ind_ptr->t311_rem_time = 0;
     }
   } 
#endif
            emm_ctrl_data_ptr->mm_send_mmr_stop_mode_cnf_fp = NULL;

            emm_state = EMM_GET_STATE();

            if(emm_ctrl_data_ptr->rlf_state == FALSE)
            {
              emm_cancel_authentication();
            }

            /* Stop any ongoing procedure first */
            switch(emm_state)
            {
              case EMM_SERVICE_REQUEST_INITIATED:
              case EMM_TRACKING_AREA_UPDATING_INITIATED:
                if(emm_ctrl_data_ptr->rlf_state == FALSE)
                {
                  emm_failure_cause.cause_type = LTE_NAS_IRAT_INVALID_STATE;
                  if(emm_state == EMM_SERVICE_REQUEST_INITIATED)
                  {
                    mm_stop_timer(TIMER_T3411);
                    mm_stop_timer(TIMER_T3402);
                    emm_abort_service_request(emm_ctrl_data_ptr, 
                                       FALSE, 
                                       TRUE, 
                                       LTE_NAS_SERVICE_REQ_FAILED_NO_THROTTLE,emm_failure_cause);
                 }
                 else
                 {
                   mm_stop_timer(TIMER_T3430);
                   emm_remove_release_client(emm_tau_request_release_indication);
                   EMM_MOVES_TO_REGISTERED_STATE(EMM_REGISTERED_NORMAL_SERVICE);
                 }
                 /* Process any pending messages */
                 emm_process_unsent_messages(CONN_CNF_LTE_RRC_CONN_EST_FAILURE,emm_failure_cause, 
                                            emm_ctrl_data_ptr);      
#ifdef FEATURE_LTE_TO_1X
                /* Process all pending irat uplink messages */
                emm_process_pending_irat_msg(LTE_NAS_IRAT_INVALID_STATE,
                                             LTE_NAS_IRAT_FAILURE_CAUSE_NONE,
                                             LTE_NAS_IRAT_ACCESS_BARRING_NONE,
                                             emm_ctrl_data_ptr);
#endif
                }
                break;
              case EMM_REGISTERED_INITIATED:
				emm_failure_cause.cause_type = LTE_NAS_IRAT_INVALID_STATE;
                if(emm_ctrl_data_ptr->rlf_state == FALSE)
                {
                  mm_stop_timer(TIMER_T3410);
                  emm_remove_release_client(emm_attach_release_indication);
                  if(EMM_GET_SUBSTATE() == EMM_WAITING_FOR_NW_RESPONSE)
                  {
                    emm_release_attach_pdn_conn_req(emm_ctrl_data_ptr, emm_failure_cause);
                  }
                  else
                  {
                  emm_build_and_send_detach_ind(EMM_DETACHED);
                }
                EMM_MOVES_TO_DEREGISTERED_STATE(EMM_DEREGISTERED_NORMAL_SERVICE);
                emm_clean_msg_buffer(emm_ctrl_data_ptr);
                }
                break;
              case EMM_DEREGISTERED_INITIATED:
                if((emm_ctrl_data_ptr->detach_reason != NORMAL_DETACH) ||
                   (EMM_GET_CONNECTION_STATE() != EMM_CONNECTED_STATE) ||
                   (emm_ctrl_data_ptr->ongoing_detach_type == IMSI_DETACH_MO_DETACH))
                {
                  emm_abort_mo_detach(emm_ctrl_data_ptr, FALSE);
                  EMM_MOVES_TO_DEREGISTERED_STATE(EMM_DEREGISTERED_NORMAL_SERVICE);
                  emm_clean_msg_buffer(emm_ctrl_data_ptr);
                }
                break;
              default:
                break;
            }
            emm_init_rrc_deact_req(SYS_STOP_MODE_REASON_ALT_SCAN, emm_ctrl_data_ptr);
          }
          /*Route all incoming messages during OOS/RLF, to mm_emm_oos_rlf_msg_handler*/
          *mm_emm_dispatcher_fn_ptr = mm_emm_oos_rlf_msg_handler;
          break;  
      }
  }
}
void mmcoord_process_manual_search_in_rlf(mm_cmd_type *cmd_ptr,mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr)
{
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
  
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  
  /*Save the G/W REG_REQ in the pending list*/
  memscpy(&mmcoord_pending_reg_list_req_message, sizeof(mm_cmd_type),cmd_ptr,sizeof(mm_cmd_type));
  mm_plmn_list_req_pending = TRUE;
  /*Set the Function pointer "mm_send_mmr_stop_mode_cnf_fp" to NULL, 
  so that MM_SEND_MMR_STOP_MODE_CNF() function is not called during OOS/RLF*/
  emm_init_rrc_deact_req(SYS_STOP_MODE_REASON_ALT_SCAN, emm_ctrl_data_ptr);
  /*Route all incoming messages during OOS/RLF, to mm_emm_oos_rlf_msg_handler*/
          *mm_emm_dispatcher_fn_ptr = mm_emm_oos_rlf_msg_handler;
}
/*===========================================================================

FUNCTION MMCOORD_PAUSE_ACTIVE_MM_TIMERS

DESCRIPTION
 This function is used to pause MM timers when stop mode ALT SCAN is received. 

DEPENDENCIES
  None

RETURN VALUE
  TRUE

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_pause_active_mm_timers(void)
{
  byte i;
  for(i=0;i<MAX_MM_TIMER+1;i++)
  {
    if (mm_timer_status[i] == TIMER_ACTIVE && 
        (i != TIMER_T3212 && i != TIMER_T3312 && i != TIMER_T3412 &&
         i != TIMER_T3402 && i !=  TIMER_T3302 && i != TIMER_T3442 &&
         i != TIMER_T3346 && i != TIMER_T3246 ) &&
        (i != EMM_FORBIDDEN_TAI_CLEAR_TIMER) &&
        (i != TIMER_MM_FORBIDDEN_LAI_CLEAR))
    {
      if( (emm_ctrl_data_ptr->rlf_state == TRUE) &&
          (i == TIMER_T3410 || i == TIMER_T3430 || i == TIMER_T3417))
      {
        MSG_HIGH_DS_1(MM_SUB, "Keep timer %d running due to RLF", i);
      }
      else if (gs_modify_timer_ext(
                               get_mm_timer_queue(),
                               i,
                               TIMER_MODIFY_PAUSE,
                               mm_timer_non_deferrable(i)) != GS_SUCCESS)
      {
        MSG_ERROR_DS(MM_SUB, "Failed to pause timer %d", i, 0, 0);
      }
      else
      {
        MSG_HIGH_DS_1(MM_SUB ,"=MM= Pausing %d", i);
        mm_timer_status[ i ] = TIMER_PAUSED;
      }  
    }
  }
}

/*===========================================================================

FUNCTION MMCOORD_RESUME_PAUSED_MM_TIMERS

DESCRIPTION
 This function is used to resume paused MM timers. 

DEPENDENCIES
  None

RETURN VALUE
  TRUE

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_resume_paused_mm_timers(void)
{
  byte i;
  for(i=0;i<MAX_MM_TIMER+1;i++)
  {
    if (mm_timer_status[i] == TIMER_PAUSED)
    {
      if (gs_modify_timer_ext(
                               get_mm_timer_queue(),
                               i,
                               TIMER_MODIFY_RESUME,
                               mm_timer_non_deferrable(i)) != GS_SUCCESS)
      {
        MSG_ERROR_DS(MM_SUB, "Failed to resume timer %d", i, 0, 0);
      }
      else
      {
        MSG_HIGH_DS_1(MM_SUB ,"=MM= Resuming %d", i);
        mm_timer_status[ i ] = TIMER_ACTIVE;
      }  
    }    
  }
}
#endif

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================
FUNCTION MMCOORD_PROCESS_MCFG_REFRESH_IND

DESCRIPTION
  This function processes the refresh indication from MCFG.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmcoord_process_mcfg_refresh_ind
( 
  mm_cmd_type *cmd_ptr 
)
{
  ASSERT(cmd_ptr != NULL);

  mm_process_mcfg_refresh();
  gmm_process_mcfg_refresh();
#ifdef FEATURE_LTE
  emm_process_mcfg_refresh();
#endif
  mcfg_refresh_done(cmd_ptr->cmd.mcfg_refresh_ind.reference_id);
}
#endif

#ifdef FEATURE_QSH_EVENT_METRIC
/*===========================================================================
FUNCTION MMCOORD_PROCESS_QSH_METRICS_IND

DESCRIPTION
  This function processes the refresh indication from MCFG.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void mmcoord_process_qsh_message_ind
( 
  mm_cmd_type *cmd_ptr 
)
{
  qsh_client_action_done_s client_action_done;
 
  mm_qsh_metric_e metric_id = 
    (mm_qsh_metric_e)(cmd_ptr->cmd.qsh_message_ind.param.action_params.metric_cfg.id);

  MSG_HIGH_DS_1(MM_SUB, "=MM= Received MM_QSH_MESSAGE_IND metric_id %d", metric_id);

  switch(metric_id)
  {
    case MM_QSH_METRIC_SIGNALING:
      {
        mm_qsh_debug_metrics.signaling_metric_context_id = 
          cmd_ptr->cmd.qsh_message_ind.param.action_params.metric_cfg.metric_context_id;
        if(cmd_ptr->cmd.qsh_message_ind.param.action_params.metric_cfg.action == QSH_METRIC_ACTION_START)
        {
          mm_qsh_debug_metrics.ota_signaling_ptr = 
             (mm_qsh_metric_signaling_s *)cmd_ptr->cmd.qsh_message_ind.param.action_params.metric_cfg.start_addr;
        }
        else
        {
          mm_qsh_debug_metrics.ota_signaling_ptr = NULL;
        }
      }
      break;

    case MM_QSH_METRIC_DEBUG_BUFFER:
      {
        mm_qsh_debug_metrics.debug_buffer_metric_context_id = 
           cmd_ptr->cmd.qsh_message_ind.param.action_params.metric_cfg.metric_context_id;
        if(cmd_ptr->cmd.qsh_message_ind.param.action_params.metric_cfg.action == QSH_METRIC_ACTION_START)
        {
          mm_qsh_debug_metrics.debug_buffer_ptr = 
                 (mm_qsh_metric_debug_buffer_s *)cmd_ptr->cmd.qsh_message_ind.param.action_params.metric_cfg.start_addr;
        }
        else
        {
          mm_qsh_debug_metrics.debug_buffer_ptr = NULL;
        }
      }
      break;

    default:
     break;
  }
  qsh_client_action_done_init(&client_action_done);
  client_action_done.cb_params_ptr = &cmd_ptr->cmd.qsh_message_ind.param;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_ASYNC;
  qsh_client_action_done(&client_action_done);
}
#endif


#ifdef FEATURE_LTE 
/*===========================================================================

FUNCTION      MMCOORD_READ_AND_UPDATE_LOCI_AUTH_KEYS

DESCRIPTION
      reads ps keys if mode has changed from CAMP_ONLY to AUTO/MANUAL
      reads cs keys if domain has changed from PS to CS or CS/PS.
 
      updates to lower layers if keys are read
 
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void mmcoord_read_and_update_loci_auth_keys
(
  mmr_reg_req_s_type  mmr_reg_req,
  emm_ctrl_data_type *emm_ctrl_data_ptr_arg
)
{
  ASSERT(emm_ctrl_data_ptr_arg != NULL);
  if(emm_ctrl_data_ptr_arg->reg_req_pending_ptr == NULL)
  {
    MSG_ERROR_DS(MM_SUB,"=EMM= Wrong sub this should not happen, return from function mmcoord_read_and_update_loci_auth_keys", 
                                                                                                                        0,0,0 );
    return;
  }
  /* read PS key for SGLTE as QSC could have modified when MSM is OOS */
  if((emm_ctrl_data_ptr_arg->reg_req_pending_ptr->network_selection_mode == 
                   SYS_NETWORK_SELECTION_MODE_AUTOMATIC_CAMP_ONLY) ||
     (emm_ctrl_data_ptr_arg->reg_req_pending_ptr->network_selection_mode == 
                        SYS_NETWORK_SELECTION_MODE_MANUAL_CAMP_ONLY))
  {
    switch(mmr_reg_req.network_selection_mode)
    {
      case SYS_NETWORK_SELECTION_MODE_MANUAL:
      case SYS_NETWORK_SELECTION_MODE_AUTOMATIC:
        gmm_load_psloci_data();
        auth_load_ps_domain_keys();

        /* update PS the keys to GWT RAT's  */
        mm_send_rr_sim_update_req();

        mm_send_rrc_sim_update_req( RRC_PS_DOMAIN_CN_ID, 
                                    SYS_RAT_UMTS_RADIO_ACCESS );
        mm_send_rrc_sim_update_req( RRC_PS_DOMAIN_CN_ID, 
                                    SYS_RAT_TDS_RADIO_ACCESS );
        break;
      default:
        break;
    }
  }

  /* read CS keys for SG to NON-SGLTE mode*/
  if((emm_ctrl_data_ptr_arg->plmn_service_state->reg_service_domain
                                      == SYS_SRV_DOMAIN_PS_ONLY) &&
     ((mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_PS) ||
      (mmr_reg_req.req_service_domain == SYS_SRV_DOMAIN_CS_ONLY)))
  {
    mm_load_csloci_data();
    auth_load_cs_domain_keys();

    /* update the CS keys to GWT RAT's  */
    mm_send_rr_sim_update_req();

    mm_send_rrc_sim_update_req( RRC_CS_DOMAIN_CN_ID, 
                                SYS_RAT_UMTS_RADIO_ACCESS );
    mm_send_rrc_sim_update_req( RRC_CS_DOMAIN_CN_ID, 
                                SYS_RAT_TDS_RADIO_ACCESS );
  }  
}

/*===========================================================================

FUNCTION MMCOORD_IRAT_ROUTE_MM_EMM_MESSSAGES

DESCRIPTION
  This function overrides the function mmcoord_route_mm_emm_messages()
  It is called only during IRAT

DEPENDENCIES
  None

RETURN VALUE
  TRUE

SIDE EFFECTS
  None

===========================================================================*/
boolean mmcoord_route_oos_rlf_mm_emm_messages 
(
  void        *command_ptr,
#ifdef FEATURE_LTE  
  emm_ctrl_data_type *emm_ctrl_data_ptr_arg,
  #endif
  mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr
)
{
  boolean     new_rat_active = FALSE;
  mm_cmd_type *mm_cmd = NULL;
  mm_cmd = (mm_cmd_type*)mm_get_cmd_buf();
  if(mm_cmd == NULL)
  {
    MSG_FATAL_DS(MM_SUB,"=MM= Error: Not able to Allocate mm_cmd_type Buffer", 0, 0, 0);
  }

  ASSERT(command_ptr != (void *)NULL);
#ifdef FEATURE_LTE
  if (mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
  {
    ASSERT(emm_ctrl_data_ptr_arg != (emm_ctrl_data_type *)NULL);
    ASSERT(emm_ctrl_data_ptr_arg->reg_req_pending_ptr != NULL);
  }
#endif

  memset(mm_cmd,0,sizeof(mm_cmd_type));
  memscpy(mm_cmd,sizeof( mm_cmd_type ),command_ptr,sizeof( mm_cmd_type ));

  switch(mm_cmd->cmd.hdr.message_set)
  {
    case MS_MM_REG:
      switch(mm_cmd->cmd.hdr.message_id)
      {
        /*************************************************
              PROCESS REG MESSAGES (LTE/WCDMA/GSM)
        *************************************************/
        case MMR_REG_REQ:
          mm_irat_pending_reg_cmd_ptr_mem_free();
          mm_plmn_list_req_pending = FALSE;
          mm_stop_timer(TIMER_POLICY_CHANGE_PENDING);
          if((mm_cmd->cmd.mmr_reg_req.type != MMR_REG_REQ_ACQ_GW)
#if defined(FEATURE_DUAL_SIM) && defined (FEATURE_DUAL_DATA)
              &&
              (emm_ctrl_data_ptr_arg->esr_tau_pending_conn_rel == FALSE)
#endif
             )
          {
            mm_csfb_rej_pending_call(command_ptr);
          }
#if defined (FEATURE_DUAL_SIM) && defined (FEATURE_TDSCDMA)
          if(mm_check_ignore_mmr_reg_req(&mm_cmd->cmd.mmr_reg_req.rat_pri_list_info) == TRUE)
          {
             return FALSE;
          }
#endif
          if(mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ)
          {
            mm_ps_detach_state = MM_PS_DETACH_STATE_NONE;
            mm_ps_detach_type  = SYS_PS_DETACH_TYPE_NONE;
          }
          if(emm_ctrl_data_ptr_arg->ps_detach_info.status == EMM_PS_DETACH_WAITING_FOR_REG_REQ)
          {
            emm_ctrl_data_ptr_arg->ps_detach_info.status = EMM_PS_DETACH_NONE;
          }
          if(emm_ctrl_data_ptr->suitable_search_end_pending == TRUE)
          {
            emm_send_reg_t311_timer_expiry_ind();
            emm_ctrl_data_ptr->suitable_search_end_pending = FALSE;
          }

          if((EMM_GET_STATE() == EMM_DEREGISTERED) &&
             (EMM_GET_SUBSTATE() == EMM_DEREGISTERED_PLMN_SEARCH)) 
          {
            EMM_SET_SUBSTATE(EMM_DEREGISTERED_NO_CELL_AVAILABLE);  
          }

#ifdef FEATURE_LTE 
          /* if the device is sglte enabled */
          if(emm_ctrl_data_ptr_arg->is_sglte_device == TRUE)
          {
            /* update the loci and auth keys if needed */
            mmcoord_read_and_update_loci_auth_keys(mm_cmd->cmd.mmr_reg_req,
                                                   emm_ctrl_data_ptr_arg);
          }
#endif

          if((mm_cmd->cmd.mmr_reg_req.rat == SYS_RAT_GSM_RADIO_ACCESS)||
             (mm_cmd->cmd.mmr_reg_req.rat == SYS_RAT_UMTS_RADIO_ACCESS) ||
             (mm_cmd->cmd.mmr_reg_req.rat == SYS_RAT_TDS_RADIO_ACCESS))
          {
            /*Legacy code handles the DEACTIVATION process (between G & W),
             so just route this REG_REQ message to legacy code.*/
            if (is_umts_mode() || is_gsm_mode())
            {
              emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;          
              /* Reset mm_multimode_rat_change_pending, as we will not be 
               processing the pending request in LTE any more.*/
              mm_multimode_rat_change_pending = FALSE;      
              if (mmcoord_svc_cnf_pending == TRUE)
              {
                mmcoord_pend_mmr_reg_req(mm_cmd);

                // Reset this flag as we will not be processing any pending reg_req in G/W now.
                mmcoord_svc_cnf_pending = FALSE;
                memset(&mmcoord_pending_svc_cnf_message,0x00, sizeof(mm_cmd_type));
              }
              else if ((EMM_GET_CONNECTION_STATE() != EMM_CONNECTED_STATE) &&
                  (emm_ctrl_data_ptr->aborting_conn != TRUE))
              {
                *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
                mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
              }           
              else
              {
               /* this will happen for LTE RLF. Abort LRRC connection if PS is being transfered to G*/
 #ifdef FEATURE_SGLTE
                if(mm_cmd->cmd.mmr_reg_req.type == MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY)
                {
                  mmcoord_pend_mmr_reg_req(mm_cmd);
                  mm_reg_waiting_for_reg_cnf = TRUE;
                  emm_rat_change_stop_timers();
                  emm_init_rrc_deact_req(SYS_STOP_MODE_REASON_IRAT, emm_ctrl_data_ptr_arg);
              }           
                else
#endif 
                {
              mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
            }
              }
            }
            else if(is_lte_mode())
            {              
#ifdef FEATURE_LTE
             /*If G/W is active and REG_REQ == LTE, then call process_rat_change() to DEACTIVATE G/W*/
             process_rat_change_req(mm_cmd,mm_emm_dispatcher_fn_ptr);
#endif
            }
			else
			{
			  mmcoord_reg_req_pending = FALSE;
			  if(EMM_GET_CONNECTION_STATE() != EMM_CONNECTED_STATE)
              {
                *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
			  }
			  mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
			}
          }
#ifdef FEATURE_LTE 
          else if(mm_cmd->cmd.mmr_reg_req.rat == SYS_RAT_LTE_RADIO_ACCESS)
          {
            /*If LTE is active and REG_REQ == G/W, then call process_rat_change() to DEACTIVATE LTE*/
            if (is_umts_mode() || is_gsm_mode())
            {
              if( EMM_GET_CONNECTION_STATE() == EMM_RELEASING_RRC_CONNECTION_STATE && 
                 (emm_ctrl_data_ptr->rlf_state == TRUE)
                )
              {
                emm_ctrl_data_ptr->reg_req_pended_indication = TRUE ;
                // Reset this flag as we will not be processing any pending reg_req in G/W now.
                mmcoord_reg_req_pending = FALSE;
                mmcoord_svc_cnf_pending = FALSE;
                memscpy(emm_ctrl_data_ptr->reg_req_pending_ptr, sizeof(mmr_reg_req_s_type),&mm_cmd->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));         
                emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr = &mmcoord_pending_reg_message;     
                memscpy(emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr, sizeof(mm_cmd_type),mm_cmd,sizeof(mm_cmd_type));
              }
			  else
			  {
                process_rat_change_req(mm_cmd,mm_emm_dispatcher_fn_ptr);
			  }
            }      
            else if(is_lte_mode())
            {
              emm_ctrl_data_ptr_arg->reg_req_pended_indication = TRUE ;
              mmcoord_reg_req_pending = FALSE;
              mm_reg_waiting_for_reg_cnf = FALSE;

              memscpy(emm_ctrl_data_ptr_arg->reg_req_pending_ptr, sizeof(mmr_reg_req_s_type),&mm_cmd->cmd.mmr_reg_req,sizeof(mmr_reg_req_s_type));
              emm_ctrl_data_ptr_arg->reg_req_pending_mm_cmd_ptr = &mmcoord_pending_reg_message;
              memscpy(emm_ctrl_data_ptr_arg->reg_req_pending_mm_cmd_ptr, sizeof(mm_cmd_type),command_ptr,sizeof(mm_cmd_type));
            }
			else
			{
			  *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
              /*There is no RAT change, route the REG message to LTE_EMM module*/
              emm_process_reg_command( mm_cmd, emm_ctrl_data_ptr_arg );
            }
          }
#endif /*FEATURE_LTE*/
          break;
        case MMR_PLMN_SEARCH_REQ:
          mm_irat_pending_reg_cmd_ptr_mem_free();
#ifdef FEATURE_WRLF_SYSTEM_SEL
          if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE && !(mm_wcdma_rlf_state != WCDMA_RLF_IDLE && 
               mm_cmd->cmd.mmr_plmn_search_req.service_search == SYS_SERVICE_SEARCH_NONE
              && mm_cmd->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL))
          {
            mmcoord_process_wrlf_mmr_plmn_search_req(mm_cmd);
          }
          else
#endif
#ifdef FEATURE_LTE 
          if(emm_ctrl_data_ptr->suitable_search_end_pending == TRUE)
          {
            emm_send_reg_t311_timer_expiry_ind();
            emm_ctrl_data_ptr->suitable_search_end_pending = FALSE;
          }

          if(is_lte_mode())
          {
            boolean reg_req_pended = mmcoord_reg_req_pending;
            /* PLMN SEARCH REQ calls off REG REQ */
            mmcoord_reg_req_pending = FALSE;
            mm_reg_waiting_for_reg_cnf = FALSE;
            emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;
            reg_service_report = (emm_reg_service_report_type)emm_send_reg_ind;
            MSG_HIGH_DS_2(MM_SUB,"=EMM= PLMN SRCH REQ in OOS - RRC DEACT status %d, %d",
                     emm_ctrl_data_ptr_arg->rrc_deact_req.deact_req_pending,
                     emm_ctrl_data_ptr_arg->rrc_deact_req.deact_cnf_pending);

            if((reg_req_pended) && 
               (mm_checkif_rat_pri_list_has_rat(
               &(emm_ctrl_data_ptr_arg->reg_req_pending_ptr->rat_pri_list_info),SYS_RAT_LTE_RADIO_ACCESS)==FALSE )&&
               (((emm_ctrl_data_ptr_arg->aborting_conn==TRUE) &&
               (emm_ctrl_data_ptr_arg->rrc_deact_req.deact_req_pending == TRUE)) ||
               (emm_ctrl_data_ptr_arg->rrc_deact_req.deact_cnf_pending == TRUE)))
            {
              MSG_HIGH_DS_0(MM_SUB,"=EMM= LTE not in RAT List - Proc PLMN SRCH REQ later");
              mm_plmn_list_req_pending = TRUE;
              mmcoord_pending_reg_list_req_message.cmd.hdr = mm_cmd->cmd.hdr;
              mmcoord_pending_reg_list_req_message.cmd.mmr_plmn_search_req = mm_cmd->cmd.mmr_plmn_search_req;
              mmcoord_pending_reg_list_req_message.cmd.mmr_plmn_search_req.rat = emm_ctrl_data_ptr_arg->reg_req_pending_ptr->rat; 
            }
#ifdef FEATURE_SGLTE
            else if(MM_IS_IN_SGLTE_MODE &&(mm_sglte_current_stack_status ==SGLTE_MM_UPPER_LAYER_MERGING))
            {
              MSG_HIGH_DS_0(MM_SUB,"=EMM= Stack Merge in progress - Pending PLMN SRCH REQ to be processed later");
              mm_plmn_list_req_pending = TRUE;
              mmcoord_pending_reg_list_req_message.cmd.hdr = mm_cmd->cmd.hdr;
              mmcoord_pending_reg_list_req_message.cmd.mmr_plmn_search_req = mm_cmd->cmd.mmr_plmn_search_req;
              mmcoord_pending_reg_list_req_message.cmd.mmr_plmn_search_req.rat = emm_ctrl_data_ptr_arg->reg_req_pending_ptr->rat; 
            }
#endif
            else if(emm_ctrl_data_ptr_arg->rrc_deact_req.deact_cnf_pending == TRUE)
            {
              MSG_HIGH_DS_0(MM_SUB,"=EMM= RRC DEACT CNF pending - Proc PLMN SRCH REQ later");
              /* Cache PLMN search req and stay in OOS handler to wait RRC 
                 DEACT CNF */    
              if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE && 
                  mm_cmd->cmd.mmr_plmn_search_req.service_search == SYS_SERVICE_SEARCH_NONE
                  && mm_cmd->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
              {
                MSG_HIGH_DS_0(MM_SUB, "=MM= Pending MMR_PLMN_SEARCH_REQ - MM Waiting for STOP MODE CNF from AS");
                mmcoord_reg_req_pending = FALSE;
                mm_wrlf_rat_change_pending = FALSE;
                mm_plmn_list_req_pending = TRUE;
                mmcoord_pending_reg_list_req_message.cmd.hdr = mm_cmd->cmd.hdr;
                mmcoord_pending_reg_list_req_message.cmd.mmr_plmn_search_req = mm_cmd->cmd.mmr_plmn_search_req;
              }
              else
              {
                emm_cache_reg_cmd(mm_cmd, emm_ctrl_data_ptr_arg);
              }
            }
            else
            {
              /* No need to deact RRC any more. Process PLMN search req in LTE. */
              emm_ctrl_data_ptr_arg->rrc_deact_req.deact_req_pending = FALSE;
              /* Process PLMN search req now and quit OOS hanlder */
              emm_process_reg_command(mm_cmd, emm_ctrl_data_ptr_arg);
              *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
              emm_ctrl_data_ptr_arg->mm_send_mmr_stop_mode_cnf_fp = mm_send_mmr_stop_mode_cnf;
            }
          }
          else
#endif /*FEATURE_LTE*/
          {
            emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE ;
            if(emm_ctrl_data_ptr->rlf_state == TRUE &&
               mm_cmd->cmd.mmr_plmn_search_req.service_search == SYS_SERVICE_SEARCH_NONE
               && mm_cmd->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
            {
               emm_rrc_abort_connection(emm_ctrl_data_ptr, FALSE,LTE_RRC_CONN_ABORT_CAUSE_NORMAL) ;
            }
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); //Route incoming message to GW module
            *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
          }
          break;
        case MMR_PLMN_SEARCH_ABORT_REQ:
#ifdef FEATURE_LTE
          if(is_lte_mode())
          {
            if((emm_ctrl_data_ptr_arg->pending_reg_cmd_ptr != NULL) &&
               (emm_ctrl_data_ptr_arg->pending_reg_cmd_ptr->cmd.hdr.message_id
                   == MMR_PLMN_SEARCH_REQ))
            {
              /* No need to process pending PLMN SEARCH REQ any more. Cache the
                 ABORT REQ. Send empty PLMN SEARCH CNF and quits OOS handler 
                 upon RRC DEACT CNF. */
              emm_cache_reg_cmd(mm_cmd, emm_ctrl_data_ptr_arg);
            }
            else
            {
              /* Only expected PLMN SEARCH ABORT REQ in OOS handler is during 
                 RRC deactivation and a PLMN SEARCH REQ is pending */
              MSG_HIGH_DS_0(MM_SUB,"=EMM= Discarding unexpected PLMN SRCH ABORT");
            }
          }
          else
#endif
          {
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          }
          break;
        case MMR_SIM_NOT_AVAILABLE_REQ:
          /*Redirect all incoming messages to go thru mm_emm_normal_msg_handler,
          as OOS/RLF scenario is no longer valid*/
          *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
          /*Reset the MM_SEND_MMR_STOP_MODE_CNF_FP to point to MM_SEND_MMR_STOP_MODE_CNF() function*/
          emm_ctrl_data_ptr_arg->mm_send_mmr_stop_mode_cnf_fp = mm_send_mmr_stop_mode_cnf;
          /*When MMR_SIM_NOT_AVAILABLE_REQ is received during IRAT, 
          come out of the inner while loop and hand over the control 
          to MAIN forever loop in mmtask.c to handle the messages 
          from there on*/
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          break;
        case MMR_SIM_AVAILABLE_REQ:
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          break;
        case MMR_STOP_MODE_REQ:
        if(mm_timer_status[TIMER_MM_PAGE_RETRY] == TIMER_ACTIVE)
        {
          mm_stop_timer(TIMER_MM_PAGE_RETRY);
          mm_last_page_type = PAGE_TYPE_NONE; 
          mm_retry_page_rsp = FALSE;
        }
          mm_stop_timer(TIMER_POLICY_CHANGE_PENDING);
          mm_stop_timer(TIMER_FRLTE);

#ifdef FEATURE_SGLTE
          if(MM_IS_IN_SGLTE_MODE && ((mm_sglte_current_stack_status == SGLTE_MM_UPPER_LAYER_MERGING)||
                                      (mm_sglte_current_stack_status == SGLTE_MM_UPPER_LAYER_SPLITING)))
          {
             MSG_HIGH_DS_0(MM_SUB, "=MM= Pending MMR_STOP_MODE_REQ - MM Waiting for Merge/Split to complete...");
             mm_pended_stop_mode_req.mmr_stop_mode_req = mm_cmd->cmd.mmr_stop_mode_req;
             mm_pended_stop_mode_req.valid = TRUE;
             mmcoord_reg_req_pending = FALSE;
             emm_ctrl_data_ptr->reg_req_pended_indication = FALSE;
             mm_plmn_list_req_pending = FALSE;
             break;
          }
#endif
          mm_csfb_rej_pending_call(command_ptr);
          mm_irat_pending_reg_cmd_ptr_mem_free();
          emm_ctrl_data_ptr->suitable_search_end_pending = FALSE;
          gmm_reset_mcs_client_info();

          /* Pause MM timers for stop mode ALT SCAN*/           
          if((mm_cmd->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                                SYS_STOP_MODE_REASON_ALT_SCAN)
#ifdef FEATURE_SGLTE
           ||(mm_cmd->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                                SYS_STOP_MODE_REASON_SUSPEND_STACK)
#endif
#ifdef FEATURE_DUAL_SIM
           ||(mm_cmd->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                                SYS_STOP_MODE_REASON_DSDS_SUSPEND)
#endif
           ||(mm_cmd->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                                SYS_STOP_MODE_REASON_DUAL_SWITCH))
          {
#if defined FEATURE_DUAL_SIM && defined FEATURE_LTE
            if (!(emm_ctrl_data_ptr->emm_state == EMM_REGISTERED_INITIATED &&
	  	          emm_ctrl_data_ptr->emm_substate == EMM_WAITING_FOR_NW_RESPONSE &&
	  	          mm_timer_status[TIMER_T3410] == TIMER_ACTIVE &&
	  	          mm_cmd->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH))
				{
				  MSG_HIGH_DS_0(MM_SUB,"=MM= ALT SCAN. Pause all running timers (except periodic timers)"); 
                  mmcoord_pause_active_mm_timers();
			    }
#else
            MSG_HIGH_DS_0(MM_SUB,"=MM= ALT SCAN. Pause all running timers (except periodic timers)"); 
            mmcoord_pause_active_mm_timers();  
#endif
            /* Mark deep sleep alt scan flag as true, so that in next CM service req. 
            ** turn ON all paused timers. */
            mm_deep_sleep_alt_scan = TRUE; 
          }
          else
          {
            mm_deep_sleep_alt_scan = FALSE;
          }

          /*Reset the MM_SEND_MMR_STOP_MODE_CNF_FP to point to MM_SEND_MMR_STOP_MODE_CNF() function*/
          emm_ctrl_data_ptr_arg->mm_send_mmr_stop_mode_cnf_fp = mm_send_mmr_stop_mode_cnf; 

#ifdef FEATURE_WRLF_SYSTEM_SEL
          /* If WRLF state is not idle and MM receives stop mode request with cause other
          ** than deep sleep or alt scan, then pend the stop mode request and send an hard
          ** abort to RRC to come out of WRLF state
          */
          if((mm_cmd->cmd.mmr_stop_mode_req.stop_mode_reason != 
                                              SYS_STOP_MODE_REASON_ALT_SCAN) &&
             (mm_cmd->cmd.mmr_stop_mode_req.stop_mode_reason != 
                                              SYS_STOP_MODE_REASON_DEEP_SLEEP)&&
               (mm_wcdma_rlf_state == WCDMA_RLF_STARTED))
          {
            mm_process_stop_mode_req_during_wrlf(mm_cmd,
#ifdef FEATURE_LTE              
                emm_ctrl_data_ptr_arg,
#endif
                mm_emm_dispatcher_fn_ptr);
          } else
#endif
          if(is_lte_mode())
          {
            mmcoord_reg_req_pending = FALSE;
            mm_reg_waiting_for_reg_cnf = FALSE;
            MSG_HIGH_DS_0(MM_SUB,"=MM= MM received  MMR_STOP_MODE_REQ, sending it to EMM");
            if((emm_ctrl_data_ptr_arg->rrc_deact_req.deact_cnf_pending == FALSE)||
               (EMM_GET_CONNECTION_STATE() != EMM_CONNECTED_STATE))
            {
              emm_process_reg_command( mm_cmd,emm_ctrl_data_ptr_arg ); 
              *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
            }
            else
            {
              mm_rlf_stop_mode_pending = MM_LTE_RLF_STOP_MODE_PENDING;
              mmcoord_pending_reg_message.cmd.mmr_stop_mode_req = mm_cmd->cmd.mmr_stop_mode_req;
              MSG_HIGH_DS_0(MM_SUB,"=MM= MM received MMR_STOP_MODE_REQ during LRLF while waiting for LRRC DEACT CNF pend the request");
              mmcoord_reg_req_pending = FALSE;
              emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;
              // Reset this function pointer as in this scenario LTE RRC should not send stop mode confiramtion to CM.
              emm_ctrl_data_ptr_arg->mm_send_mmr_stop_mode_cnf_fp = NULL;
            }
          }
          else if (is_umts_mode() || is_gsm_mode())
          {
            emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;
            mm_multimode_rat_change_pending = FALSE;
            /* If LTE RLF recovery is going and active rat is not LTE and NAS receives stop mode request
            with any cause other than alt-scan then first abort and deactivate LRRC and process 
            stop mode request after that in G or W. Please look at analysis section of CR 313242 to know the
            scenarios covered by this change.
            */
            if((emm_ctrl_data_ptr_arg->l2gcco_state == EMM_LTOG_CCO_INACTIVE )&&(EMM_GET_CONNECTION_STATE() == EMM_CONNECTED_STATE) &&
			   (mm_cmd->cmd.mmr_stop_mode_req.stop_mode_reason != SYS_STOP_MODE_REASON_ALT_SCAN))
            {
              mm_rlf_stop_mode_pending = MM_LTE_RLF_STOP_MODE_PENDING;
              mmcoord_pending_reg_message.cmd.mmr_stop_mode_req = mm_cmd->cmd.mmr_stop_mode_req;
              MSG_HIGH_DS_0(MM_SUB,"=MM= MM received MMR_STOP_MODE_REQ during LRLF and active rat is not lte, pend the request");
              emm_init_rrc_deact_req(SYS_STOP_MODE_REASON_IRAT, emm_ctrl_data_ptr_arg);
              // Reset this function pointer as in this scenario LTE RRC should not send stop mode confiramtion to CM.
              emm_ctrl_data_ptr_arg->mm_send_mmr_stop_mode_cnf_fp = NULL;
              // Reset this flag so as to not initiate the service request on new rat on receiving stop mode confirmation.
              mm_rat_change_pending = FALSE;
              mm_waiting_for_stop_mode_cnf = FALSE;
            }
            else
            {
              MSG_HIGH_DS_0(MM_SUB,"=MM= MM received  MMR_STOP_MODE_REQ, sending it to MM");
              (void)mmcoord_route_mm_message((void *)mm_cmd, mm_emm_dispatcher_fn_ptr);
              *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
            }
          }
          else
          {      
            MSG_HIGH_DS_0(MM_SUB,"=MM= MM received  MMR_STOP_MODE_REQ, sending a cnf back immediately");
            (void)mmcoord_route_mm_message((void *)mm_cmd, mm_emm_dispatcher_fn_ptr);
            /*Set REG_REQ pending message to 0*/
            mmcoord_reg_req_pending = FALSE;
            *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
          }
          break;

        case MMR_PS_DETACH_REQ:

#ifdef FEATURE_SGLTE
          if(MM_IS_IN_SGLTE_MODE && mm_sglte_current_stack_status == SGLTE_MM_UPPER_LAYER_MERGING)
          {
            /* REG overrides merge, so store and process after merge completion */
            mmcoord_reg_req_pending = FALSE;
            emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;
            mm_reg_waiting_for_reg_cnf = FALSE;

            MSG_HIGH_DS_0(MM_SUB,"=MM= Pending PS Detach Req as upper layer merge in Progress");
            if(( mm_pending_mmr_ps_detach_req_ptr == NULL ) && 
              ((mm_pending_mmr_ps_detach_req_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) 
                        == NULL))
            {
              mm_check_for_null_ptr((void*)mm_pending_mmr_ps_detach_req_ptr);
            }
            memscpy( mm_pending_mmr_ps_detach_req_ptr, sizeof(mm_cmd_type), &mm_cmd, sizeof(mm_cmd_type));
            mm_ps_detach_state = MM_PS_DETACH_STATE_DETACH_REQ_PENDING;
          }
          else
#endif
          {
            MSG_HIGH_DS_0(MM_SUB,"=EMM= Received MMR_PS_DETACH_REQ, change the main routine handler to mm_emm_normal_msg_handler");
            // No need to change the handler as we are not ending RLF here. Even if LTE gets removed and device moves to other RAT
            // LRRC will still stay connected till T311 expiry. If we stay in the OOS handler, any new acquisition, PS detach or stop
            // mode request from CM will be handled accordingly in OOS handler, and LTE RLF will be ended.
            mmcoord_reg_req_pending = FALSE;
            emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;
            mm_reg_waiting_for_reg_cnf = FALSE;
            if(is_gsm_mode() || is_umts_mode())
            {
              if(emm_ctrl_data_ptr->rlf_state == TRUE)
              {
                if(( mm_pending_mmr_ps_detach_req_ptr == NULL ) && 
                ((mm_pending_mmr_ps_detach_req_ptr=(mm_cmd_type *)modem_mem_calloc(1,sizeof( mm_cmd_type ),MODEM_MEM_CLIENT_NAS)) 
                    == NULL))
                {
                  mm_check_for_null_ptr((void*)mm_pending_mmr_ps_detach_req_ptr);
                }
                memscpy( mm_pending_mmr_ps_detach_req_ptr, sizeof(mm_cmd_type), mm_cmd, sizeof(mm_cmd_type));
                mm_ps_detach_state = MM_PS_DETACH_STATE_DETACH_REQ_PENDING;
                emm_init_rrc_deact_req(SYS_STOP_MODE_REASON_IRAT, emm_ctrl_data_ptr);
				emm_send_reg_t311_timer_expiry_ind();
              }
              else
              {
                mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
              }
            }
            else
            {
              emm_process_reg_command(mm_cmd, emm_ctrl_data_ptr_arg );    
            }
          }
          break;

        case MMR_SET_DRX_REQ:
            MSG_HIGH_DS_0(MM_SUB,"=MM= MM received MMR_SET_DRX_REQ in OOS_RLF handler, return failure");
            mm_send_mmr_set_drx_cnf(FALSE);
            break;

        case MMR_GET_DRX_REQ:
            mm_send_mmr_get_drx_cnf();
            break;

        case MMR_HIGH_PRIORITY_ACTIVITY_IND:
            if(is_gsm_mode() || is_umts_mode())
            {
              mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
            }
            else
            {
              emm_process_reg_command(mm_cmd, emm_ctrl_data_ptr_arg );    
            }
            break;

#ifdef FEATURE_FEMTO_CSG
        case MMR_FILE_REFRESH_IND:
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
            break;
#endif 
        case MMR_EQ_PLMN_CHANGE_IND:
        case MMR_SIM_STATE_UPDATE_IND:
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
            break;

        case MMR_BLOCK_PLMN_REQ: /*This will always be processed by EMM*/
            emm_process_reg_command( mm_cmd, emm_ctrl_data_ptr_arg );   
            break;

        case MMR_CLEAR_LAI_REJECT_LIST_REQ:
          emm_process_reg_command( mm_cmd, emm_ctrl_data_ptr_arg );
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          break;

#ifdef FEATURE_DUAL_SIM
        case MMR_DS_STAT_CHGD_REQ:
        case MMR_DS_TUNEAWAY_STATUS_CHANGE_IND:
        case MMR_MULTIMODE_SUBS_CHGD_REQ:
        case MMR_DDS_SWITCH_IND:
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          break;

        case MMR_DATA_PRIORITY_IND:
          if ((is_lte_mode() ||
               emm_ctrl_data_ptr_arg->rlf_state) 
#ifdef FEATURE_WRLF_SYSTEM_SEL
              /* Received RRC_REL_IND while waiting for LTE_RRC_DEACTIVATE_CNF while in RLF state */
              &&(mm_wcdma_rlf_state == WCDMA_RLF_IDLE)
#endif
             )
          {
            emm_process_reg_command( mm_cmd, emm_ctrl_data_ptr_arg );
          }
          else
          {
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          }
          break;
#endif
#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE ||defined FEATURE_SVLTE_DUAL_SIM
        case MMR_UE_MODE_CHANGE_IND:
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          break;
#endif
#ifdef FEATURE_CMCC_SECURE_CALL
        case MMR_SECURE_CALL_IND:  
         mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
         break;

#endif

#ifdef FEATURE_FEMTO_CSG
         case MMR_CSG_SELECT_CONFIG_UPDATE_IND:
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
            break;
#endif 

        default:
          MSG_ERROR_DS(MM_SUB,"NAS received msg %d during INTER_RAT, this message shall be currently discarded",mm_cmd->cmd.hdr.message_id,0,0);
          break;
      }
      break;
    
    case MS_MM_RRC:
    case MS_MM_TDSRRC:
      switch(mm_cmd->cmd.hdr.message_id)
      {
        /*************************************************
           PROCESS STOP_MODE_CNF MESSAGES (GSM/WCDMA)
        *************************************************/
        case RRC_STOP_WCDMA_MODE_CNF:
          if(mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE)
          {
            if(mm_waiting_for_stop_mode_cnf)
            {
              mm_waiting_for_stop_mode_cnf = FALSE;
              mm_send_rrc_hard_abort_req( mm_wrlf_serving_plmn_ptr->info.active_rat );
              MSG_HIGH_DS_0(MM_SUB,"Send hard abort request for processing stop mode request");
            }
            else
            {
              MSG_HIGH_DS_0(MM_SUB,"Stop mode confirmation is pending ignoring RRC_STOP_WCDMA_MODE_CNF");
            }
          }
          else
          {
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
            if(mm_irat_pending_reg_cmd_ptr != NULL &&
               mm_rat_handler_status == MM_RAT_INITIAL)
            {
              WtoGCCO_state = CCO_INACTIVE;
  #ifdef FEATURE_INTERRAT_PCCO_GTOW
              GtoWCCO_state = CCO_INACTIVE;
  #endif  
              process_rat_change_req(mm_irat_pending_reg_cmd_ptr,mm_emm_dispatcher_fn_ptr);
              mm_irat_pending_reg_cmd_ptr_mem_free();
            }
            else if(mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ)
            {
              MSG_HIGH_DS_0(MM_SUB,"=EMM= PS Detach State is waiting for reg req ignoring pending reg req");
              emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;
              if(EMM_GET_CONNECTION_STATE() != EMM_CONNECTED_STATE)
			  {
			    *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
              }
            }
            /*If there is a pending REG_REQ, that means this STOP_MODE_CNF is for the STOP_MODE_REQ sent 
              from MM (as soon as REG_REQ for LTE was received")*/
            else if(emm_ctrl_data_ptr_arg->reg_req_pended_indication == TRUE)
            {
              /* As soon as we receive STOP_MODE_CNF, set this flag to FALSE, so that in the future if
              REG_REQ comes for G/W, then MM will not discard this REG_REQ (assuming that STOP_MODE_CNF hasn't come yet)
              In case we run into any issues later on, we might think of calling mmcoord_locally_detach_gw()*/
              mm_waiting_for_stop_mode_cnf = FALSE;
              MSG_HIGH_DS_0(MM_SUB,"=MM= Received STOP_MODE_CNF");
#ifdef FEATURE_SGLTE
      /* if this service request was received while merge was in progress, allow split before processing service request*/
              if (MM_IS_IN_SGLTE_MODE && is_gsm_mode())
              {
                process_rat_change_req(emm_ctrl_data_ptr->reg_req_pending_mm_cmd_ptr,
                                       mm_emm_dispatcher_fn_ptr);
              }
              else
#endif
              {
                MSG_HIGH_DS_0(MM_SUB,"=EMM= now processing the pending REG_REQ for LTE");
#ifdef FEATURE_DUAL_SIM
                mm_update_rat_status(
                    &emm_ctrl_data_ptr_arg->reg_req_pending_mm_cmd_ptr->cmd.mmr_reg_req.rat_pri_list_info,
                    SYS_RAT_ACTIVATION_STATUS_DEACTIVATED);
#endif

                emm_process_reg_command(emm_ctrl_data_ptr_arg->reg_req_pending_mm_cmd_ptr,emm_ctrl_data_ptr_arg);  
                /*Reset the message handler function to point to the normal message handler*/
                *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;  
              }
            }    
            else if(mmcoord_reg_req_pending == TRUE)
            {
              mmcoord_route_mm_message(&mmcoord_pending_reg_message,mm_emm_dispatcher_fn_ptr);   
            }   
          }
          break;

        case RRC_REL_IND:
#ifdef FEATURE_WRLF_SYSTEM_SEL
          if(mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE)
          {
            MSG_HIGH_DS_0(MM_SUB,"Ignoring RRC_REL_IND as we are waiting for RRC_ABORT_CNF, stopmode pending during WRLF");
          }
          else
#endif
          if(!is_lte_mode()
#ifdef FEATURE_WRLF_SYSTEM_SEL
             /* Received RRC_REL_IND while waiting for LTE_RRC_DEACTIVATE_CNF while in RLF state */
             ||(mm_wcdma_rlf_state != WCDMA_RLF_IDLE)
#endif
            )
          { 
            /*While in G/W mode, any GRR/WRR STOP_MODE_CNF messages need to be rerouted to GMM*/
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
          }
          break;
        /*************************************************
            PROCESS SERV_IND MESSAGES (GSM/WCDMA)
        *************************************************/
        case RRC_SERVICE_IND:
        case RRC_SERVICE_CNF:
          if(mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE)
          {
            MSG_HIGH_DS_0(MM_SUB,"Stop mode confirmation is pending ignoring the service confirmation from rrc");
          }
          /*Check for RAT change conf, if so, send the conf back to REG and quit this loop*/
          //Follow LTE detach procedure after SERV_CONF on GW            
          else if((mm_cmd->cmd.hdr.message_id == RRC_SERVICE_CNF)&&
             (mm_cmd->cmd.rrc_service_cnf.service_status == MM_AS_SERVICE_AVAILABLE) &&
             (mm_waiting_for_stop_mode_cnf == FALSE))
          {
#ifdef FEATURE_LTE
            /* Service found on W. No need to maintain T3411/T3402 anymore. */
            emm_rat_change_stop_timers();
            gmm_reset_mcs_client_info();
            /*============================================================================
                If UE is in the connected mode and previously received 
                NO_SERVICE on LTE, then UE entered RLF, in this case 
                just send rrc_detach with a cause value SYS_STOP_MODE_REASON_IRAT
                Upon, reception of the DEACT_CNF, process MM_AS_SERVICE_AVAILABLE message  
                Also, put the received SERVICE_CNF as pending messsage, 
                so that it is processed after DEACT_CNF is received.
            ==============================================================================*/
            if((EMM_GET_CONNECTION_STATE() == EMM_CONNECTED_STATE) ||
               (emm_ctrl_data_ptr->aborting_conn == TRUE))
            {
              mmcoord_svc_cnf_pending =  TRUE;
              memscpy(&mmcoord_pending_svc_cnf_message, sizeof(mm_cmd_type),mm_cmd,sizeof(mm_cmd_type));
              emm_init_rrc_deact_req(SYS_STOP_MODE_REASON_IRAT, emm_ctrl_data_ptr_arg);
            }
            /*=======================================================
                If UE is in the Idle state and previously received 
                NO_SERVICE on LTE, then UE entered OOS, in this case 
                just locally detach EMM and come out of the internal 
                while loop
            =========================================================*/      
            else
            {
              /*-------------------------------------------
                            LTE_LOCAL_DETACH
              -------------------------------------------*/
              new_rat_active = TRUE; //Set this flag to TRUE, as service is found in G/W      
              mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
              /*Reset pending REG_REQ*/
              mmcoord_reg_req_pending = FALSE;      
              /*Route all incoming messages to normal_msg_handler, now new RAT is ACTIVE*/
              *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
              //mmcoord_locally_detach_gw();
              /*Reset the MM_SEND_MMR_STOP_MODE_CNF_FP to point to MM_SEND_MMR_STOP_MODE_CNF() function*/
              emm_ctrl_data_ptr_arg->mm_send_mmr_stop_mode_cnf_fp = mm_send_mmr_stop_mode_cnf;
            }
#endif 
          }
          else
          {
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          }
          
          break;
        
#ifdef FEATURE_WRLF_SYSTEM_SEL
        case RRC_SUITABLE_SEARCH_END_IND:
          mm_send_rrc_suitable_search_end_ind();
          break;
        
        case RRC_DISABLE_BPLMN_IND:
          mm_send_rrc_disable_bplmn_ind(mm_cmd->cmd.rrc_disable_bplmn_ind.disable_status);
          break;
#endif

        case RRC_ABORT_CNF:
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
#ifdef FEATURE_WRLF_SYSTEM_SEL
          if(mm_rlf_stop_mode_pending == MM_WCDMA_RLF_STOP_MODE_PENDING)
          {
            mm_rlf_stop_mode_pending = MM_RLF_STOP_MODE_PENDING_NONE;
            if(is_lte_mode())
            {
              emm_process_reg_command(&mmcoord_pending_reg_message, emm_ctrl_data_ptr_arg );     
            }
            else
            {
              mmcoord_route_mm_message(&mmcoord_pending_reg_message,mm_emm_dispatcher_fn_ptr); 
            }
            /*Route all incoming messages to normal_msg_handler, now new RAT is ACTIVE*/
           *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
          }else
#endif
          if(emm_ctrl_data_ptr_arg->reg_req_pended_indication == TRUE)
          {
            process_rat_change_req(emm_ctrl_data_ptr_arg->reg_req_pending_mm_cmd_ptr,
                                                      mm_emm_dispatcher_fn_ptr);
          }
          else if((is_gsm_mode() || is_umts_mode()) && 
                  (mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_TO_SEND_CNF))
          {
             *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
          }
          break;
        case RRC_ABORT_IND:
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
          break;
        case RRC_CLOSE_SESSION_CNF:
        case RRC_CLOSE_SESSION_IND:
#ifdef FEATURE_WRLF_SYSTEM_SEL
          if(mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE)
          {
            MSG_HIGH_DS_0(MM_SUB,"Ignoring close session as we are waiting for RRC_ABORT_CNF, stopmode pending during WRLF");
          }
          else if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE || mm_reg_req_revd_active_call == TRUE)
          {
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
            MSG_HIGH_DS_2(MM_SUB,"mm_wcdma_rlf_state=%d mm_reg_req_revd_active_call=%d",mm_wcdma_rlf_state,mm_reg_req_revd_active_call);
          }else
#endif
          {
            MSG_ERROR_DS(MM_SUB,"NAS received msg with msg set %d msg id %d during INTER_RAT, this message shall be currently discarded",
                mm_cmd->cmd.hdr.message_set,mm_cmd->cmd.hdr.message_id,0);
          }
          break;

          case RRC_SRVCC_HANDOVER_FAILURE_IND:
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
            break;

          case RRC_WTOG_CCO_COMPLETE_IND:
           mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
           break;

        default:
          MSG_ERROR_DS(MM_SUB,"NAS received msg with msg set %d msg id %d during INTER_RAT, this message shall be currently discarded",
                       mm_cmd->cmd.hdr.message_set,mm_cmd->cmd.hdr.message_id,0);
          break;
      }
      break;

    case MS_MM_RR:
      switch(mm_cmd->cmd.hdr.message_id)
      {
        /*************************************************
           PROCESS STOP_MODE_CNF MESSAGES (GSM/WCDMA)
        *************************************************/
        case RR_STOP_GSM_MODE_CNF:
          if(mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE)
          {
            MSG_HIGH_DS_0(MM_SUB,"Stop mode confirmation is pending ignoring RR_STOP_GSM_MODE_CNF");
          }
          else
          { 
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
            if(mm_irat_pending_reg_cmd_ptr != NULL)
            {
              WtoGCCO_state = CCO_INACTIVE;
  #ifdef FEATURE_INTERRAT_PCCO_GTOW
              GtoWCCO_state = CCO_INACTIVE;
  #endif  
              process_rat_change_req(mm_irat_pending_reg_cmd_ptr,mm_emm_dispatcher_fn_ptr);
              mm_irat_pending_reg_cmd_ptr_mem_free();
            }
            else if(mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ)
            {
              MSG_HIGH_DS_0(MM_SUB,"=EMM= PS Detach State is waiting for reg req ignoring pending reg req");
              emm_ctrl_data_ptr_arg->reg_req_pended_indication = FALSE;
			  if(EMM_GET_CONNECTION_STATE() != EMM_CONNECTED_STATE)
			  {
                *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
			  }
            }
            /*If there is a pending REG_REQ, that means this STOP_MODE_CNF is for the STOP_MODE_REQ sent 
              from MM (as soon as REG_REQ for LTE was received")*/
            else if(emm_ctrl_data_ptr_arg->reg_req_pended_indication == TRUE)
            {
              /* As soon as we receive STOP_MODE_CNF, set this flag to FALSE, so that in the future if
              REG_REQ comes for G/W, then MM will not discard this REG_REQ (assuming that STOP_MODE_CNF hasn't come yet)
              In case we run into any issues later on, we might think of calling mmcoord_locally_detach_gw()*/
              mm_waiting_for_stop_mode_cnf = FALSE;
              MSG_HIGH_DS_0(MM_SUB,"=MM= Received STOP_MODE_CNF & now processing the pending REG_REQ for LTE");
#ifdef FEATURE_DUAL_SIM
              mm_update_rat_status(
               &emm_ctrl_data_ptr_arg->reg_req_pending_mm_cmd_ptr->cmd.mmr_reg_req.rat_pri_list_info,
               SYS_RAT_ACTIVATION_STATUS_DEACTIVATED);
#endif
              emm_process_reg_command(emm_ctrl_data_ptr_arg->reg_req_pending_mm_cmd_ptr,emm_ctrl_data_ptr_arg);  
              /*Reset the message handler function to point to the normal message handler*/
              *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;       
            }   
            else if(mmcoord_reg_req_pending == TRUE)
            {
              mmcoord_route_mm_message(&mmcoord_pending_reg_message,mm_emm_dispatcher_fn_ptr);   
            } 
          }         
          break;

        case RR_REL_IND:
          if(!is_lte_mode())
          { 
            /*While in G/W mode, any GRR/WRR STOP_MODE_CNF messages need to be rerouted to GMM*/
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
          }
          break;
        /*************************************************
            PROCESS SERV_IND MESSAGES (GSM/WCDMA)
        *************************************************/
        case RR_SERVICE_IND:
        case RR_PLMN_SELECT_CNF:
        case RR_EQ_PLMN_LIST_CHANGE_IND:
          if((mm_rlf_stop_mode_pending != MM_RLF_STOP_MODE_PENDING_NONE)&&
             ((mm_cmd->cmd.hdr.message_id == RR_PLMN_SELECT_CNF) ||
             (mm_cmd->cmd.hdr.message_id == RR_SERVICE_IND)))
          {
            MSG_HIGH_DS_0(MM_SUB,"Stop mode confirmation is pending ignoring the select confirmation from rr");
          }
          /*Check for RAT change conf, if so, send the conf back to REG and quit this loop*/
          //Follow LTE detach procedure after SERV_CONF on GW            
          else if((mm_cmd->cmd.hdr.message_id == RR_PLMN_SELECT_CNF)&&
              (mm_cmd->cmd.rr_plmn_select_cnf.service_state ==  MM_AS_SERVICE_AVAILABLE) && 
              (mm_waiting_for_stop_mode_cnf == FALSE))
          {
#ifdef FEATURE_LTE
            /* Service found on W. No need to maintain T3411/T3402 anymore. */
            emm_rat_change_stop_timers();
            gmm_reset_mcs_client_info();
            /*============================================================================
                If UE is in the connected mode and previously received 
                NO_SERVICE on LTE, then UE entered RLF, in this case 
                just send rrc_detach with a cause value SYS_STOP_MODE_REASON_IRAT
                Upon, reception of the DEACT_CNF, process MM_AS_SERVICE_AVAILABLE message  
                Also, put the received SERVICE_CNF as pending messsage, 
                so that it is processed after DEACT_CNF is received.
            ==============================================================================*/
            if ((EMM_GET_CONNECTION_STATE() == EMM_CONNECTED_STATE) ||
                (emm_ctrl_data_ptr->aborting_conn == TRUE))
            {
              mmcoord_svc_cnf_pending =  TRUE;
              memscpy(&mmcoord_pending_svc_cnf_message, sizeof(mm_cmd_type),mm_cmd,sizeof(mm_cmd_type));
              emm_init_rrc_deact_req(SYS_STOP_MODE_REASON_IRAT, emm_ctrl_data_ptr_arg);
            }
            /*=======================================================
                If UE is in the Idle state and previously received 
                NO_SERVICE on LTE, then UE entered OOS, in this case 
                just locally detach EMM and come out of the internal 
                while loop
            =========================================================*/      
            else
            {
              /*-------------------------------------------
                            LTE_LOCAL_DETACH
              -------------------------------------------*/
              new_rat_active = TRUE; //Set this flag to TRUE, as service is found in G/W      
              mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
              /*Reset pending REG_REQ*/
              mmcoord_reg_req_pending = FALSE;      
              /*Route all incoming messages to normal_msg_handler, now new RAT is ACTIVE*/
              *mm_emm_dispatcher_fn_ptr = mm_emm_normal_msg_handler;
              //mmcoord_locally_detach_gw();
              /*Reset the MM_SEND_MMR_STOP_MODE_CNF_FP to point to MM_SEND_MMR_STOP_MODE_CNF() function*/
              emm_ctrl_data_ptr_arg->mm_send_mmr_stop_mode_cnf_fp = mm_send_mmr_stop_mode_cnf;
            }
#endif 
          }
          else
          {
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          }
          
          break;

        case RR_ABORT_CNF:
        case RR_ABORT_IND:
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); 
          if(emm_ctrl_data_ptr_arg->reg_req_pended_indication == TRUE)
          {
            process_rat_change_req(emm_ctrl_data_ptr_arg->reg_req_pending_mm_cmd_ptr,
                                                      mm_emm_dispatcher_fn_ptr);
          }
          break;

          case RR_PLMN_LIST_CNF:
            MSG_HIGH_DS_0(MM_SUB,"=MM= Received RR_PLMN_LIST_CNF in OOS RLF handler");
            if(is_gsm_mode())
            {
              mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr); //Route incoming message to GW module
            }
            break;
          case RR_GMM_GPRS_MULTISLOT_CLASS_IND:
          case RR_GMM_RETX_PDU_TUNEAWAY_FAIL_IND:
          case RR_PLMN_RESEL_FAIL_IND: 
            MSG_HIGH_DS_0(MM_SUB,"=MM= received multislot indication in OOS RLF handler");
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
            break;

        case RR_SRVCC_HANDOVER_FAILURE_IND:
#ifdef FEATURE_DUAL_SIM
        case RR_MULTIMODE_SUBS_CHGD_CNF:
#endif
        case RR_PS_STATUS_CHANGE_IND:
        case RR_GMM_GPRS_MS_RA_CAP_REQ:
          mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          break;
        default:
          MSG_ERROR_DS(MM_SUB,"NAS received msg with msg set %d msg id %d during INTER_RAT, this message shall be currently discarded",
                   mm_cmd->cmd.hdr.message_set,mm_cmd->cmd.hdr.message_id,0);
          break;
      }
      break;

    case MS_CC_MM:
    case MS_CC_MM_PS:

      switch(mm_cmd->cmd.hdr.message_id)
      {
        case MMCNM_EST_REQ:
        #ifdef FEATURE_SMS_ENHANCED_DOMAIN_SELECTION        
          mm_send_mmcnm_est_rej(MM_REJECT_CAUSE, MM_REJ_NO_SERVICE,MMCC_SMS_CS);
        #else
          mm_send_mmcnm_est_rej(MM_REJECT_CAUSE, MM_REJ_NO_SERVICE);
        #endif
          break;

        case  MMCNM_REL_REQ:          
          if(is_lte_mode()
#ifdef FEATURE_WRLF_SYSTEM_SEL
             && (mm_wcdma_rlf_state == WCDMA_RLF_IDLE)
#endif
            )
          {
            emm_process_cnm_command(mm_cmd, emm_ctrl_data_ptr_arg);     
          }
          else
          {
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          }
          break;

#ifdef FEATURE_WRLF_SYSTEM_SEL
        case  MMCNM_DATA_REQ:
          if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE)
          {
            mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
          }
          else
          {
            MSG_ERROR_DS(MM_SUB,"NAS received MN/CNM msg  set: %d id: %d during INTER_RAT, this message shall be currently discarded",
                     mm_cmd->cmd.hdr.message_set,mm_cmd->cmd.hdr.message_id,0);
          }
          break;
#endif

        default:
          MSG_ERROR_DS(MM_SUB,"NAS received MN/CNM msg  set: %d id: %d during INTER_RAT, this message shall be currently discarded",
                   mm_cmd->cmd.hdr.message_set,mm_cmd->cmd.hdr.message_id,0);
          break;

      }
      break;

    case MS_TIMER:
      /* T3346 is common to MM/EMM. depending on active rat route it */
      if(((mm_cmd->cmd.timer_expiry.timer_id >= MIN_MM_TIMER && 
              mm_cmd->cmd.timer_expiry.timer_id <= MAX_MM_ONLY_TIMER)
         &&
         ((mm_cmd->cmd.timer_expiry.timer_id != TIMER_T3346) 
           || 
          ((is_umts_mode() || is_gsm_mode()))) )
          || ((is_umts_mode() || is_gsm_mode()) &&(mm_cmd->cmd.timer_expiry.timer_id == TIMER_T3247)))
      {
        mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr );
      }
      else
      {
        emm_process_timer_expiry(mm_cmd, emm_ctrl_data_ptr_arg);
      }
      break;
    case MS_GSDI:
      mmcoord_route_mm_message(mm_cmd, mm_emm_dispatcher_fn_ptr);
      break;

    case MS_GMM_SM:
      switch(mm_cmd->cmd.hdr.message_id)
      {
        case GMMSM_ESTABLISH_REQ:
        case GMMSM_UNITDATA_REQ:
        case GMMSM_CLOSE_SESSION_REQ:
#ifdef FEATURE_DUAL_SIM
        case GMMSM_PS_CALL_STATUS_IND:
#endif
          mmcoord_route_mm_message(mm_cmd, mm_emm_dispatcher_fn_ptr);
          break;

        case GMMSM_RAT_RESUME_CNF:
          if(mm_irat_pending_reg_cmd_ptr != NULL)
          {
            mmcoord_route_mm_message(mm_cmd, mm_emm_dispatcher_fn_ptr);
            WtoGCCO_state = CCO_INACTIVE;
#ifdef FEATURE_INTERRAT_PCCO_GTOW
            GtoWCCO_state = CCO_INACTIVE;
#endif  
            process_rat_change_req(mm_irat_pending_reg_cmd_ptr,mm_emm_dispatcher_fn_ptr);
            mm_irat_pending_reg_cmd_ptr_mem_free();
          }
          else
          {
            MSG_HIGH_DS_0(MM_SUB,"Ignoring GMMSM_RAT_RESUME_CNF in OOS/RLF handler");
          }
          break;

        case GMMSM_FAST_DORMANCY_REQ:
          MSG_HIGH_DS_0(MM_SUB,"Ignoring GMMSM_FAST_DORMANCY_REQ in OOS/RLF handler");
          break;

        default:
          MSG_ERROR_DS(MM_SUB,"Unexpected SM to GMM message ID %d",mm_cmd->cmd.hdr.message_id,0,0);
          break;
      }
      break;

    case MS_GMM_RABM:
      switch(mm_cmd->cmd.hdr.message_id)
      {
        case GMMRABM_RAB_REL_IND:
        case GMMRABM_REESTABLISH_REQ:

#ifdef FEATURE_NAS_REL12
        case GMMRABM_RAB_EST_IND:
#endif
          mmcoord_route_mm_message(mm_cmd, mm_emm_dispatcher_fn_ptr);
          break;

        default:
          MSG_ERROR_DS(MM_SUB,"Unexpected RABM to GMM message ID %d",mm_cmd->cmd.hdr.message_id,0,0);
          break;
      }
      break;

    case MS_LLC_GMM:
        switch(mm_cmd->cmd.hdr.message_id) 
        {
          case GLLCMM_LLGMM_SUSPEND_CNF :
          case GLLCMM_LL_XID_IND:
          case GLLCMM_LLGMM_RDY_TMR_IND:
             MSG_HIGH_DS_1(MM_SUB, "=MM= GLIF: Rcvd GLLCMM_LLGMM_SUSPEND_CNF/GLLCMM_LL_XID_IND/GLLCMM_LLGMM_RDY_TMR_IND  Id: %d ",mm_cmd->cmd.hdr.message_id) ;           
             mmcoord_route_mm_message(mm_cmd,mm_emm_dispatcher_fn_ptr);
             break ;

          default:
             MSG_ERROR_DS(MM_SUB,"Unexpected LLC to GMM message ID %d",mm_cmd->cmd.hdr.message_id,0,0);
             break;
          
        }
       break;

     case MS_MM_MM:
       switch(mm_cmd->cmd.hdr.message_id)
       {
#ifdef FEATURE_MODEM_CONFIG_REFRESH
         case MM_MCFG_REFRESH_IND:
           mmcoord_process_mcfg_refresh_ind(mm_cmd);
           break;
#endif
#ifdef FEATURE_QSH_EVENT_METRIC
         case MM_QSH_MESSAGE_IND:
           mmcoord_process_qsh_message_ind(mm_cmd);
           break;
#endif
         case MM_NON_ACQ_PARAM_CHANGE_IND:
           if(is_lte_mode())
            {
              emm_process_non_acq_param_change_ind(mm_cmd,emm_ctrl_data_ptr_arg);
            }
            else
            {
               mmcoord_route_mm_message( mm_cmd,mm_emm_dispatcher_fn_ptr );
            }
           break;
         default:
          break;
       }
       break;
     case MS_CM_MM:
        switch(mm_cmd->cmd.hdr.message_id)
        {
           case MMCM_MMTEL_CALL_INFO:
             emm_process_mmtel_call_status_info(mm_cmd,emm_ctrl_data_ptr_arg);
             break;
           case MMCM_RPM_UPDATE_FTAI_LIST:
             emm_process_update_ftai_list_cmd(&mm_cmd->cmd.cm_mm_update_ftai_list,
                                                                   emm_ctrl_data_ptr_arg );
             break;

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
           case MMCM_LTE_SUSPEND_REQ:
             emm_process_cm_lte_suspend_req(emm_ctrl_data_ptr_arg);
             break;

           case MMCM_LTE_RESUME_IND:
             emm_process_cm_lte_resume_ind(emm_ctrl_data_ptr_arg);
             break;
#endif

#ifdef FEATURE_1XSRLTE
          case MMCM_DUAL_RX_1XCSFB_TAU_REQ:
            emm_process_cm_dual_rx_tau_cmd(emm_ctrl_data_ptr_arg);
           break;
#endif
           default:
             MSG_ERROR_DS_1(MM_SUB,"Unexpected CM to MM message ID %d",mm_cmd->cmd.hdr.message_id);
             break;
        }
        break;


    case MS_WMS_MM:
      mmcoord_route_mm_message(mm_cmd, mm_emm_dispatcher_fn_ptr);
      break; 

    default:
      MSG_ERROR_DS(MM_SUB,"NAS received msg with msg set %d msg id %d during INTER_RAT, this message shall be currently discarded",
                   mm_cmd->cmd.hdr.message_set,mm_cmd->cmd.hdr.message_id,0);
      break;
  }
  mm_free_cmd_buf(mm_cmd);
  mm_cmd = NULL;
  return new_rat_active;
}

#endif


/*===========================================================================

FUNCTION MM_EMM_PROCESS_REG_MESSAGES

DESCRIPTION
  This function processes all the reg messages received by EMM and MM. 
DEPENDENCIES
  None

RETURN VALUE
  TRUE

SIDE EFFECTS
  None

===========================================================================*/

boolean mmcoord_route_mm_emm_messages
(
  void        *command_ptr,
#ifdef FEATURE_LTE   
  emm_ctrl_data_type *emm_ctrl_data_ptr_arg,
#endif
  mm_emm_dispatcher_fp **mm_emm_dispatcher_fn_ptr
)
{
   mm_cmd_type        *cmd_ptr;
   ASSERT(command_ptr != (void *)NULL);
#ifdef FEATURE_LTE
   ASSERT(emm_ctrl_data_ptr_arg != (emm_ctrl_data_type *)NULL);
#endif

   cmd_ptr = (mm_cmd_type*)command_ptr;   


   switch (cmd_ptr->cmd.hdr.message_set)
   {
     case MS_MM_REG:      
       switch(cmd_ptr->cmd.hdr.message_id)
       {
        case MMR_PLMN_SEARCH_REQ:     
#if defined (FEATURE_DUAL_SIM) && defined (FEATURE_TDSCDMA)
         mm_reg_req_ignored = FALSE;
#endif
#ifdef FEATURE_WRLF_SYSTEM_SEL
        if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE && !(mm_wcdma_rlf_state != WCDMA_RLF_IDLE && 
                 cmd_ptr->cmd.mmr_plmn_search_req.service_search == SYS_SERVICE_SEARCH_NONE
               && cmd_ptr->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL))
        {
          mmcoord_process_wrlf_mmr_plmn_search_req(cmd_ptr);
        }
        else
#endif
        {
#ifdef FEATURE_LTE 
            if(emm_ctrl_data_ptr->suitable_search_end_pending == TRUE)
            {
              emm_send_reg_t311_timer_expiry_ind();
              emm_ctrl_data_ptr->suitable_search_end_pending = FALSE;
            }
            if(is_lte_mode())
            {
#ifdef FEATURE_WRLF_SYSTEM_SEL
              if(mm_wcdma_rlf_state != WCDMA_RLF_IDLE && cmd_ptr->cmd.mmr_plmn_search_req.service_search == SYS_SERVICE_SEARCH_NONE
                 && cmd_ptr->cmd.mmr_plmn_search_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL)
              {
                mmcoord_process_manual_search_in_rlf(cmd_ptr,mm_emm_dispatcher_fn_ptr);
              }
              else if ((emm_ctrl_data_ptr->rrc_deact_req.deact_req_pending == TRUE )||
                       (emm_ctrl_data_ptr->rrc_deact_req.deact_cnf_pending == TRUE))
              {
                emm_cache_reg_cmd(cmd_ptr, emm_ctrl_data_ptr);
                MSG_HIGH_DS_0(MM_SUB, "=EMM= LTE is being deactivated,Pending Search request");
              }
              else
#endif
              {
                emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg );   
              }
            }
            else if(mm_serving_plmn.info.active_rat == SYS_RAT_NONE &&
                    ((mm_checkif_rat_pri_list_has_rat(&cmd_ptr->cmd.mmr_plmn_search_req.rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS) == TRUE) &&
                     (mm_checkif_rat_pri_list_has_rat(&cmd_ptr->cmd.mmr_plmn_search_req.rat_pri_list_info, SYS_RAT_UMTS_RADIO_ACCESS) == FALSE) &&
                     (mm_checkif_rat_pri_list_has_rat(&cmd_ptr->cmd.mmr_plmn_search_req.rat_pri_list_info, SYS_RAT_GSM_RADIO_ACCESS) == FALSE) && 
                     (mm_checkif_rat_pri_list_has_rat(&cmd_ptr->cmd.mmr_plmn_search_req.rat_pri_list_info, SYS_RAT_TDS_RADIO_ACCESS) == FALSE)))
            {
            /*Setting active rat to L if L onle search req comes so that L is deacted on stop mode/lpm or fresh serv req*/
              mm_serving_plmn.info.active_rat = SYS_RAT_LTE_RADIO_ACCESS;
              cmd_ptr->cmd.mmr_plmn_search_req.rat = SYS_RAT_LTE_RADIO_ACCESS;
              emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg );
            }
            else
#endif/*FEATURE_LTE*/ 
            {
              mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            }
        }
        break ;
            
        case MMR_REG_REQ:
#ifdef FEATURE_FEMTO_CSG
          if((mm_nv_csg_support > SYS_CSG_SUPPORT_DISABLED) && 
             (cmd_ptr->cmd.mmr_reg_req.network_selection_mode == SYS_NETWORK_SELECTION_MODE_MANUAL_CSG ||
               cmd_ptr->cmd.mmr_reg_req.type == MMR_REG_REQ_CSG_SELECTION_APP) &&
             (mm_sim_allowed_csg_list_read_started == TRUE ||
              mm_sim_operator_csg_list_read_started == TRUE))
          {
             MSG_HIGH_DS_0(MM_SUB,"=MM= Ignone REG Req - CSG reading not complete yet, on CSG reading send ignore IND to REG");
             mm_reg_req__pended_due_to_csg_read = TRUE;
             return TRUE;
          }
#endif 	  
          mm_plmn_list_req_pending = FALSE;
          mm_stop_timer(TIMER_POLICY_CHANGE_PENDING);
#if defined (FEATURE_DUAL_SIM) && defined (FEATURE_TDSCDMA)
          if(mm_check_ignore_mmr_reg_req(&cmd_ptr->cmd.mmr_reg_req.rat_pri_list_info) == TRUE)
          {
             return FALSE;
          }
#endif

#ifdef FEATURE_LTE  
#if defined(FEATURE_DUAL_SIM) && defined (FEATURE_DUAL_DATA)
          if(!MM_IS_NON_DDS_SUB && 
             (emm_ctrl_data_ptr->reg_req_pending_ptr != NULL  && 
			  mm_rat_pri_list_match(&emm_ctrl_data_ptr->reg_req_pending_ptr->rat_pri_list_info,
              &cmd_ptr->cmd.mmr_reg_req.rat_pri_list_info) == FALSE)&& 
             mm_checkif_rat_pri_list_has_rat(&cmd_ptr->cmd.mmr_reg_req.rat_pri_list_info, SYS_RAT_TDS_RADIO_ACCESS))
         {
       	    emm_ctrl_data_ptr_arg->esr_tau_pending_conn_rel = FALSE;
         }
#endif

          if(cmd_ptr->cmd.mmr_reg_req.type != MMR_REG_REQ_ACQ_GW
#if defined(FEATURE_DUAL_SIM) && defined (FEATURE_DUAL_DATA)
             &&
             mm_emm_supl_domain_switch_in_progress(&cmd_ptr->cmd.mmr_reg_req,
                               emm_ctrl_data_ptr_arg) == FALSE
             &&
             emm_ctrl_data_ptr_arg->esr_tau_pending_conn_rel == FALSE
#endif
             && !((mm_csfb_state == MM_CSFB_STATE_MO_CALL_ORIG) &&
                 is_lte_mode() &&
                 (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_LTE_RADIO_ACCESS) &&
                 (emm_ctrl_data_ptr->reg_req_pending_ptr != NULL) && ((emm_ctrl_data_ptr->reg_req_pending_ptr->req_service_domain == cmd_ptr->cmd.mmr_reg_req.req_service_domain) &&
                 (emm_ctrl_data_ptr->reg_req_pending_ptr->volte_only != cmd_ptr->cmd.mmr_reg_req.volte_only)) &&
                 (emm_ctrl_data_ptr->esr_type_in_progress == NAS_EMM_3GPP_CSFB))
            )
          {
            mm_csfb_rej_pending_call(command_ptr); 
          }

          if(emm_ctrl_data_ptr->suitable_search_end_pending == TRUE)
          {
            emm_send_reg_t311_timer_expiry_ind();
            emm_ctrl_data_ptr->suitable_search_end_pending = FALSE;
          }

          if(mm_ps_detach_state == MM_PS_DETACH_STATE_WAITING_FOR_REG_REQ)
          {
            mm_ps_detach_state = MM_PS_DETACH_STATE_NONE;
            mm_ps_detach_type  = SYS_PS_DETACH_TYPE_NONE;
          }

          if(emm_ctrl_data_ptr_arg->ps_detach_info.status == EMM_PS_DETACH_WAITING_FOR_REG_REQ)
          {
            emm_ctrl_data_ptr_arg->ps_detach_info.status = EMM_PS_DETACH_NONE;
          }

          if((EMM_GET_STATE() == EMM_DEREGISTERED) &&
             (EMM_GET_SUBSTATE() == EMM_DEREGISTERED_PLMN_SEARCH)) 
          {
            EMM_SET_SUBSTATE(EMM_DEREGISTERED_NO_CELL_AVAILABLE);  
          }

          if( emm_ctrl_data_ptr_arg->l2gcco_state == EMM_LTOG_CCO_IN_PROGRESS
#ifdef FEATURE_SGLTE
            && MM_IS_SGLTE_ON_PS_STACK_ID
#endif
            )
          {
            MSG_HIGH_DS_0(MM_SUB,"=MM= Received REG request during CCO, set CCO state to aborted");
            emm_ctrl_data_ptr_arg->l2gcco_state = EMM_LTOG_CCO_ABORTED;
            /* If keys are not derived set service loss on lte to true, so as to derive the keys on select cnf.
               This is to cover scenario where service req is received after CCO and before service ind.
               gmm state check is needed as we set keys_valid to false once rau is started.
            */
            if(gmm_state != GMM_ROUTING_AREA_UPDATING_INITIATED)
            {
              emm_service_loss_on_lte();
            }
          }

          /* if the device is sglte enabled */
          if(emm_ctrl_data_ptr_arg->is_sglte_device == TRUE)
          {
            /* update the loci and auth keys if needed */
            mmcoord_read_and_update_loci_auth_keys(cmd_ptr->cmd.mmr_reg_req,
                                                   emm_ctrl_data_ptr_arg);
          }
#endif
          if(is_lte_mode())/*Check to see if the current RAT is LTE*/
          {
            //Check to see if the requested RAT is G/W
            if((cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_GSM_RADIO_ACCESS)||
               (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_TDS_RADIO_ACCESS) ||
               (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_UMTS_RADIO_ACCESS)) 
            {
              /*********************************************
                     MMR_REG_REQ != Current_RAT (LTE);
                   this indicates RAT change indication
              **********************************************/
              MSG_HIGH_DS_0(MM_SUB,"EMM received MMR_REG_REQ to activate G/W/TDS, processing RAT change request");
#ifdef FEATURE_LTE
#ifdef FEATURE_SGLTE
              if (MM_SUB_IS_SGLTE && (mm_wcdma_rlf_state==WCDMA_RLF_STARTED)&& 
                  (cmd_ptr->cmd.mmr_reg_req.type==MMR_REG_REQ_SGLTE_PSTIMER_EXPIRY))
              {
                mm_send_rrc_abort_req_during_wrlf();
                memscpy(&mmcoord_pending_reg_message, sizeof(mm_cmd_type),cmd_ptr,sizeof(mm_cmd_type));
               *mm_emm_dispatcher_fn_ptr = mm_emm_oos_rlf_msg_handler;
                emm_ctrl_data_ptr_arg->reg_req_pended_indication = TRUE;
              }
              else
#endif 
              {
                process_rat_change_req(cmd_ptr,mm_emm_dispatcher_fn_ptr);
              }
#endif
            }
            else if(cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_LTE_RADIO_ACCESS)
            {
#ifdef FEATURE_LTE
              /*There is no RAT change, route the REG message to LTE_EMM module*/
              emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg ); 
#endif
            }
            else
            {
              MSG_ERROR_DS(MM_SUB,"Invalid RAT sent in REG_REQ",0,0,0);
            }
          }
          else if (is_umts_mode() || is_gsm_mode()) //If current RAT is G/W/T
          {
            if(cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_LTE_RADIO_ACCESS) //if requested RAT is LTE
            {
              /**********************************************
                   MMR_REG_REQ != Current_RAT (GSM/WCDMA);
                    this indicates RAT change indication
              **********************************************/
#ifdef FEATURE_LTE
              MSG_HIGH_DS_0(MM_SUB,"MM received MMR_REG_REQ to activate LTE, processing RAT change request");
              process_rat_change_req(cmd_ptr,mm_emm_dispatcher_fn_ptr);
              #endif
            }
            else if((cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_GSM_RADIO_ACCESS)||
                    (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_TDS_RADIO_ACCESS) ||
               (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_UMTS_RADIO_ACCESS))
            {
              /************************************************************************
                GMM takes care of routing the RAT changes b/w GSM and WCDMA (if any), 
                       just forward this REG_REQ to mmcoord_route_mm_message()
              ************************************************************************/
                   mm_multimode_rat_change_pending = FALSE;
              mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
            }
            else
            {
              MSG_ERROR_DS(MM_SUB,"Invalid RAT sent in REG_REQ",0,0,0);
            }
          }
          else /*REG_REQ at powerup*/
          {
            /*----------------------------------------------------------------
               During power up, none of the RATs will be active, so assume 
               that the REG_REQ RAT is the active RAT. SERV_IND shall also 
               be handled with this REG_REQ RAT              
            ---------------------------------------------------------------*/

            // Reset this flag as we will not be processing any pending reg_req in G/W now (SIM removal case).
            mmcoord_reg_req_pending = FALSE;

            if((cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_GSM_RADIO_ACCESS)||
               (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_TDS_RADIO_ACCESS) ||
               (cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_UMTS_RADIO_ACCESS))
            {
              mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
            }
            else if(cmd_ptr->cmd.mmr_reg_req.rat == SYS_RAT_LTE_RADIO_ACCESS)
            {
#ifdef FEATURE_LTE
              emm_process_reg_command(cmd_ptr,emm_ctrl_data_ptr_arg); 
#endif
            }
            else
            {
              MSG_ERROR_DS(MM_SUB,"During UE POWERUP, NAS received invalid REG_REQ for RAT selection, discarding this message",0,0,0);
            }
          }
          break ;
            
        case MMR_SIM_AVAILABLE_REQ:
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            break ;
            
        case MMR_SIM_NOT_AVAILABLE_REQ:
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            break ;
            
        case MMR_STOP_MODE_REQ:
        if(mm_timer_status[TIMER_MM_PAGE_RETRY] == TIMER_ACTIVE)
        {
          mm_stop_timer(TIMER_MM_PAGE_RETRY);
          mm_last_page_type = PAGE_TYPE_NONE; 
          mm_retry_page_rsp = FALSE;
        }
            mm_stop_timer(TIMER_POLICY_CHANGE_PENDING);
            mm_stop_timer(TIMER_FRLTE);
#ifdef FEATURE_LTE
            if(emm_ctrl_data_ptr->suitable_search_end_pending == TRUE)
            {
              emm_send_reg_t311_timer_expiry_ind();
              emm_ctrl_data_ptr->suitable_search_end_pending = FALSE;
            }
            gmm_reset_mcs_client_info();
#endif
#if defined (FEATURE_DUAL_SIM) && defined (FEATURE_TDSCDMA)
            mm_reg_req_ignored = FALSE;
#endif
            if(((cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
               SYS_STOP_MODE_REASON_ALT_SCAN) || 
               (cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason ==
               SYS_STOP_MODE_REASON_DEEP_SLEEP )
#ifdef FEATURE_SGLTE
              || (cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason ==
               SYS_STOP_MODE_REASON_SUSPEND_STACK )
#endif
               )
               && (mm_deep_sleep_alt_scan == TRUE)
               && (mm_serving_plmn.info.active_rat != SYS_RAT_LTE_RADIO_ACCESS)
               && (mm_waiting_for_stop_mode_cnf==FALSE)
              )
             {
               MSG_HIGH_DS_0(MM_SUB,"=MM= looping back Stop Mode Req as already in ALT scan");
              /*we'll not be processing any pended reg_req now */
               mmcoord_reg_req_pending = FALSE;
#ifdef FEATURE_LTE
               emm_ctrl_data_ptr->reg_req_pended_indication = FALSE ;
#endif
               mmr_stop_mode_trans_id = cmd_ptr->cmd.mmr_stop_mode_req.trans_id;
               mm_send_mmr_stop_mode_cnf();
             }
            else
             {
#ifdef FEATURE_LTE
            mm_csfb_rej_pending_call(command_ptr);
            if((cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                                SYS_STOP_MODE_REASON_ALT_SCAN)
#ifdef FEATURE_SGLTE
            || (cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                           SYS_STOP_MODE_REASON_SUSPEND_STACK)
#endif
#ifdef FEATURE_DUAL_SIM
               || (cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_DSDS_SUSPEND)               
#endif
               || (cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_DUAL_SWITCH))
            {
#if defined FEATURE_DUAL_SIM && defined FEATURE_LTE
              if (!(emm_ctrl_data_ptr->emm_state == EMM_REGISTERED_INITIATED &&
                    emm_ctrl_data_ptr->emm_substate == EMM_WAITING_FOR_NW_RESPONSE &&
                    mm_timer_status[TIMER_T3410] == TIMER_ACTIVE &&
                    cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == SYS_STOP_MODE_REASON_DUAL_SWITCH))
                  {
                    MSG_HIGH_DS_0(MM_SUB,"=MM= ALT SCAN. Pause all running timers (except periodic timers)"); 
                    mmcoord_pause_active_mm_timers();
                  }
#else
                MSG_HIGH_DS_0(MM_SUB,"=MM= ALT SCAN. Pause all running timers (except periodic timers)"); 
                mmcoord_pause_active_mm_timers(); 
#endif
              /* Mark deep sleep alt scan flag as true, so that in next CM service req. 
              ** turn ON all paused timers. */
              mm_deep_sleep_alt_scan = TRUE; 
            }
            else
            {
              mm_deep_sleep_alt_scan = FALSE;
            }
#endif /*FEATURE_LTE*/

#ifdef FEATURE_DUAL_SIM
            if(cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_MODE_CAP_CHANGE)
            {
              mm_stopped_due_to_mode_cap_change = TRUE;
            }
            else if((cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_MODE_CHANGE)
                  ||(cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_MODE_CHANGE_FTM) 
                  ||(cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_POWER_OFF) 
                  ||(cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_OFFLINE) 
                  ||(cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_LPM)  
#ifdef FEATURE_LTE
                  ||(cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_LOCAL_DETACH)
#endif
                  ||(cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason == 
                                              SYS_STOP_MODE_REASON_LPM_POWER_OFF))
            {
              mm_stopped_due_to_mode_cap_change = FALSE;
            }
#endif

#ifdef FEATURE_WRLF_SYSTEM_SEL
            /* If WRLF state is not idle and MM receives stop mode request with cause other
            ** than deep sleep or alt scan, then pend the stop mode request and send an hard
            ** abort to RRC to come out of WRLF state
            */
            if((cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason != 
                                                SYS_STOP_MODE_REASON_ALT_SCAN) &&
               (cmd_ptr->cmd.mmr_stop_mode_req.stop_mode_reason != 
                                                SYS_STOP_MODE_REASON_DEEP_SLEEP)&&
               (mm_wcdma_rlf_state == WCDMA_RLF_STARTED))
            {
              mm_process_stop_mode_req_during_wrlf(cmd_ptr,
#ifdef FEATURE_LTE
              emm_ctrl_data_ptr_arg,
#endif
              mm_emm_dispatcher_fn_ptr);
            }
            else
#endif
#ifdef FEATURE_LTE
            if((is_lte_mode()) && 
               (!((rrc_connection_status == MMRRC_RELEASE_IN_PROGRESS) && 
               (mmcoord_reg_req_pending))))
            {
              emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg );   
            }
            else
#endif
            {
              mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            }
             }
            break ;
            
        case MMR_PLMN_SEARCH_ABORT_REQ:
    #ifdef FEATURE_LTE            
            if(is_lte_mode())
            {
              if((emm_ctrl_data_ptr_arg->pending_reg_cmd_ptr != NULL) &&
               (emm_ctrl_data_ptr_arg->pending_reg_cmd_ptr->cmd.hdr.message_id
                   == MMR_PLMN_SEARCH_REQ))
              { 
                /*No need to process pending PLMN SEARCH REQ any more. Cache the
                                 ABORT REQ. Send empty PLMN SEARCH CNF 
                                 upon RRC DEACT CNF. */
                emm_cache_reg_cmd(cmd_ptr, emm_ctrl_data_ptr_arg);
              }
              else
              {
                emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg ); 
              }  
            }
            else
    #endif /*FEATURE_LTE*/            
            {
              mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            }

            break ;      
            
        case MMR_PH_STATUS_CHANGE_REQ:        
            
            /* If UMTS or GSM is enabled then this message will always be processed first by MM*/
#if defined(FEATURE_WCDMA) || defined(FEATURE_GSM)
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
#elif defined(FEATURE_LTE)
            emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg );   
#endif
            break;



#ifdef FEATURE_LTE
        case MMR_PS_DETACH_REQ:
        case MMR_HIGH_PRIORITY_ACTIVITY_IND:
            if(is_lte_mode())
            {
              emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg );   
            }
            else
            {
              mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            }
            break;

        case MMR_SET_DRX_REQ:
            if(is_gsm_mode() || is_umts_mode())
            {
              mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            }
            else
            {
              emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg );   
            }
            break;

         case MMR_GET_DRX_REQ: /*This is always process by GMM */
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            break;

         case MMR_BLOCK_PLMN_REQ: /*This will always be processed by EMM*/
            emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg );   
            break;
#endif /*FEATURE_LTE*/

#ifdef FEATURE_FEMTO_CSG
         case MMR_FILE_REFRESH_IND:
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            break;
#endif 
        case MMR_NET_SEL_MODE_CHANGE_IND:
#ifdef FEATURE_LTE      
            if(is_lte_mode())
            {
              emm_process_reg_command(cmd_ptr,emm_ctrl_data_ptr_arg); 
            }
            else
#endif          
            {
              mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            }
           break;
        case MMR_SIM_STATE_UPDATE_IND:
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
            break;

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_LTE)
        case MMR_MULTIMODE_SUBS_CHGD_REQ:
            mmcoord_process_mmr_multimode_subs_chgd_req(cmd_ptr);
            break;
#endif
#ifdef FEATURE_DUAL_SIM
        case MMR_DDS_SWITCH_IND:
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
            break;
#endif
#ifdef FEATURE_LTE
        case MMR_GW_PLMN_SEL_END_IND:
          mmcoord_process_gw_plmn_sel_end_ind();
          break;
#endif
        case MMR_CLEAR_LAI_REJECT_LIST_REQ:
#ifdef FEATURE_LTE 
          if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS))
          {
            emm_process_reg_command(cmd_ptr,emm_ctrl_data_ptr_arg );
          }
#endif
          mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
          break;
#ifdef MMOC_API_PH_STAT_CNF_WITH_STATUS
       case MMR_SIM_BUSY_IND:

#ifdef FEATURE_LTE      
            if(is_lte_mode())
            {
              emm_process_reg_command(cmd_ptr,emm_ctrl_data_ptr_arg); 
            }
         else
#endif  
           {
           mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 

           }
       break;   
#endif

#ifdef FEATURE_FEMTO_CSG
         case MMR_CSG_SELECT_CONFIG_UPDATE_IND:
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
            break;
#endif 
#ifdef FEATURE_DUAL_SIM
         case MMR_DATA_PRIORITY_IND:
#ifdef FEATURE_LTE
          if ((is_lte_mode() ||
               emm_ctrl_data_ptr_arg->rlf_state) 
#ifdef FEATURE_WRLF_SYSTEM_SEL
              /* Received RRC_REL_IND while waiting for LTE_RRC_DEACTIVATE_CNF while in RLF state */
              &&(mm_wcdma_rlf_state == WCDMA_RLF_IDLE)
#endif
             )
          {
            emm_process_reg_command( cmd_ptr, emm_ctrl_data_ptr_arg );
          }
          else
#endif
          {
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
          }
          break;
#endif

         case MMR_REFRESH_STAGE_IND:
#ifdef FEATURE_LTE
           if(is_lte_mode()) /*Check to see if the current RAT is LTE*/
           {
             emm_process_reg_command(cmd_ptr,emm_ctrl_data_ptr_arg); 
           }
           else
#endif
           {
             mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
           }
           break;

        default:
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);          
       }
       break;

    case MS_MM_RRC:
    case MS_MM_TDSRRC:
      switch(cmd_ptr->cmd.hdr.message_id)
      {
        case RRC_ABORT_CNF:
          mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr); 
          break;

        default:
            mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
            break;
       }
       break;

     case MS_TIMER:
       /* T3346 is common to MM/EMM. depending on active rat route it */
       if(((cmd_ptr->cmd.timer_expiry.timer_id >= MIN_MM_TIMER && 
               cmd_ptr->cmd.timer_expiry.timer_id <= MAX_MM_ONLY_TIMER)
          &&
          (((cmd_ptr->cmd.timer_expiry.timer_id != TIMER_T3346) &&
             (cmd_ptr->cmd.timer_expiry.timer_id != TIMER_POLICY_CHANGE_PENDING) ) ||
           ((is_umts_mode() || is_gsm_mode()))))||
           ((is_umts_mode() || is_gsm_mode() || mm_serving_plmn.info.active_rat == SYS_RAT_NONE) && (cmd_ptr->cmd.timer_expiry.timer_id == TIMER_T3247)))
       {
         mmcoord_route_mm_message( cmd_ptr,mm_emm_dispatcher_fn_ptr );        
       }
#ifdef FEATURE_LTE
       else
       {
         emm_process_timer_expiry(cmd_ptr, emm_ctrl_data_ptr_arg);
       }
#endif /*FEATURE_LTE*/
       break;
#ifdef FEATURE_LTE  
     case MS_CC_MM:
     case MS_CC_MM_PS:
       switch(cmd_ptr->cmd.hdr.message_id)
       {
         case  MMCNM_REL_REQ:          
         case  MMCNM_DATA_REQ:
           if(is_lte_mode()
#ifdef FEATURE_WRLF_SYSTEM_SEL
              && (mm_wcdma_rlf_state == WCDMA_RLF_IDLE)
#endif
             )
           {
             emm_process_cnm_command(cmd_ptr, emm_ctrl_data_ptr_arg);    
           }
           else
           {
             mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
           }
           break;

         default:
           if(is_lte_mode())
           {
             emm_process_cnm_command(cmd_ptr, emm_ctrl_data_ptr_arg);   
           }
           else
           {
             mmcoord_route_mm_message(cmd_ptr,mm_emm_dispatcher_fn_ptr);
           }
           break;
       }
       break;
    case MS_CM_MM:
       switch(cmd_ptr->cmd.hdr.message_id)
      {
#ifdef FEATURE_1XSRLTE
          case MMCM_DUAL_RX_1XCSFB_TAU_REQ:
            emm_process_cm_dual_rx_tau_cmd(emm_ctrl_data_ptr_arg);
           break;
#endif
         case MMCM_UE_CAPABILITIES_INFO:
           emm_process_ue_capabilities_info( cmd_ptr,emm_ctrl_data_ptr_arg );
           break;
#ifdef FEATURE_LTE
         case MMCM_RPM_UPDATE_FTAI_LIST:
           emm_process_update_ftai_list_cmd(&cmd_ptr->cmd.cm_mm_update_ftai_list,
                                                                   emm_ctrl_data_ptr_arg );
           break;
#endif

#ifdef FEATURE_CRAT_MSIM_TEST_FRAMEWORK
          case MMCM_LTE_SUSPEND_REQ:
            emm_process_cm_lte_suspend_req(emm_ctrl_data_ptr_arg);
           break;

          case MMCM_LTE_RESUME_IND:
            emm_process_cm_lte_resume_ind(emm_ctrl_data_ptr_arg);
           break;
#endif
          case MMCM_MMTEL_CALL_INFO:
            emm_process_mmtel_call_status_info(cmd_ptr,emm_ctrl_data_ptr_arg);
            break;

          case MMCM_PS_DATA_AVAILABLE_REQ:
            mmcoord_route_mm_message( cmd_ptr,mm_emm_dispatcher_fn_ptr );
            break;

#ifdef FEATURE_NAS_ECALL
          case MMCM_ECALL_INACTIVITY_TRIGGER:
          case MMCM_ECALL_REMAINING_TIMER_VALUE:
            mmcoord_route_mm_message( cmd_ptr,mm_emm_dispatcher_fn_ptr );
            break;
#endif
#ifdef FEATURE_LTE_REL9
          case MMCM_IMS_REGISTRATION_INFO:
            if(is_lte_mode())
            {
             emm_process_ims_voice_tag_reg_status_ind(cmd_ptr,emm_ctrl_data_ptr_arg);
            }
            else
            {
              mmcoord_route_mm_message( cmd_ptr,mm_emm_dispatcher_fn_ptr );  
            }
            break;
#endif
       }
      break;
#endif /*FEATURE_LTE*/  

     case MS_MM_MM:
       switch(cmd_ptr->cmd.hdr.message_id)
       {
#ifdef FEATURE_MODEM_CONFIG_REFRESH
         case MM_MCFG_REFRESH_IND:
           mmcoord_process_mcfg_refresh_ind(cmd_ptr);
           break;
#endif
#ifdef FEATURE_QSH_EVENT_METRIC
         case MM_QSH_MESSAGE_IND:
           mmcoord_process_qsh_message_ind(cmd_ptr);
           break;
#endif
#ifdef FEATURE_LTE
         case MM_NON_ACQ_PARAM_CHANGE_IND:
            if(is_lte_mode())
            {
              emm_process_non_acq_param_change_ind(cmd_ptr,emm_ctrl_data_ptr_arg);
            }
            else
            {
               mmcoord_route_mm_message( cmd_ptr,mm_emm_dispatcher_fn_ptr );
            }
           break;
#endif
         default:
          break;
       }
       break;

     case MS_GSDI:
            /*Fall through*/
     default:
       mmcoord_route_mm_message( cmd_ptr,mm_emm_dispatcher_fn_ptr );                 
       break;
   }   
   return TRUE;
} /* end mm_emm_process_reg_messages() */

#if defined(FEATURE_QSH_EVENT_METRIC) || defined(FEATURE_QSH_DUMP) || defined(FEATURE_QSH_EVENT_NOTIFY_TO_QSH)

#ifdef FEATURE_QSH_DUMP
/*===========================================================================
Function: mm_emm_get_qsh_dump_data()

Description:
MM task QSH dump data for for Dump collection action.

============================================================================*/
void mm_emm_get_qsh_dump_data
(
  mm_qsh_dump_data_s_type *dump_data_ptr, 
  mm_qsh_dump_tag_e tag
)
{
  mm_as_id_e_type as_id = MM_AS_ID_1;
  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_data_ptr,(uint16)tag,
                                sizeof(mm_qsh_dump_data_s_type));
  if(tag == MM_QSH_DUMP_TAG_AS_ID_2)
  {
    as_id = MM_AS_ID_2;
  }
#ifdef FEATURE_LTE
  emm_qsh_dump_data_get(&(dump_data_ptr->emm_debug_data),as_id);
#endif
  mm_qsh_dump_data_get(&(dump_data_ptr->mm_debug_data),as_id);
}


/*===========================================================================
Function: mm_get_qsh_dump_debug_buffer()

Description:
MM Task QSH debug buffer for Dump collection action
============================================================================*/
void mm_get_qsh_dump_debug_buffer
(
  mm_qsh_dump_debug_buffer_s_type *dump_data_ptr, 
  mm_qsh_dump_tag_e tag
)
{
  qsh_client_dump_tag_hdr_init((qsh_dump_tag_hdr_s *)dump_data_ptr,(uint16)tag,
                                sizeof(mm_qsh_dump_debug_buffer_s_type));
  mm_emm_qsh_dump_debug_buffer_get(dump_data_ptr->debug_buffer);
}

/*===========================================================================
Function: mm_populate_qsh_dump_data()

Description:
MM Task QSH Dump collection action
============================================================================*/
void mm_populate_qsh_dump_data
(
  qsh_client_cb_params_s *param
)
{
  qsh_client_action_done_s client_action_done;
  byte *dump_ptr;
  size_t total_size_written = 0; 
  size_t remaining_size = param->action_params.dump_collect.dump_iovec.size_bytes;
  dump_ptr = (byte *)(param->action_params.dump_collect.dump_iovec.addr);
  if(QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask,MM_QSH_DUMP_TAG_AS_ID_1))
  {
    if(remaining_size >= sizeof(mm_qsh_dump_data_s_type))
    {
      mm_emm_get_qsh_dump_data((mm_qsh_dump_data_s_type *)dump_ptr, MM_QSH_DUMP_TAG_AS_ID_1);
      dump_ptr = dump_ptr + (sizeof(mm_qsh_dump_data_s_type));
      remaining_size -= sizeof(mm_qsh_dump_data_s_type);
      total_size_written += sizeof(mm_qsh_dump_data_s_type);
    }
  }
  if(QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask,MM_QSH_DUMP_TAG_AS_ID_2))
  {
    if(remaining_size >= sizeof(mm_qsh_dump_data_s_type))
    {
      mm_emm_get_qsh_dump_data((mm_qsh_dump_data_s_type *)dump_ptr,MM_QSH_DUMP_TAG_AS_ID_2);
      dump_ptr = dump_ptr + (sizeof(mm_qsh_dump_data_s_type));
      remaining_size -= sizeof(mm_qsh_dump_data_s_type);
      total_size_written += sizeof(mm_qsh_dump_data_s_type);
    }
  }
  if(QSH_DUMP_TAG_ENABLED(param->action_params.dump_collect.dump_tag_mask,MM_QSH_DUMP_TAG_DEBUG_BUFFER))
  {
    if(remaining_size >= sizeof(mm_qsh_dump_debug_buffer_s_type))
    {
      mm_get_qsh_dump_debug_buffer((mm_qsh_dump_debug_buffer_s_type *)dump_ptr,MM_QSH_DUMP_TAG_DEBUG_BUFFER);
      dump_ptr = dump_ptr + (sizeof(mm_qsh_dump_debug_buffer_s_type));
      remaining_size -= sizeof(mm_qsh_dump_debug_buffer_s_type);
      total_size_written += sizeof(mm_qsh_dump_debug_buffer_s_type);
    }
  }
  qsh_client_action_done_init(&client_action_done);
  client_action_done.cb_params_ptr = param; 
  client_action_done.params.dump_collect.size_written_bytes = total_size_written;
  client_action_done.action_mode_done = QSH_ACTION_MODE_DONE_SYNC;
  qsh_client_action_done(&client_action_done);
}
#endif


#ifdef FEATURE_QSH_MDUMP  
/*===========================================================================
FUNCTION mm_qsh_mdump_handler
  
DESCRIPTION
  This function collects the mdump for NAS


DEPENDENCIES
  None


RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
QSH_MDUMP_FN_ATTR 
void mm_qsh_mdump_handler()
{

#if defined FEATURE_DUAL_SIM
  mm_as_id_e_type temp_as_id;
  temp_as_id = mm_as_id;
#endif

  /* Collect all the bare minimum globals needed for debugging */

  mm_log_mdump_static_vars();
  gmm_log_mdump_static_vars();
  emm_log_mdump_static_vars();
  emm_log_mdump_vars();


#if defined FEATURE_DUAL_SIM
  for (mm_as_id = MM_AS_ID_1; mm_as_id < (mm_as_id_e_type)MAX_NAS_STACKS; mm_as_id++)
#endif
  {

  qsh_mdump_collect_high(&gmm_attach_type,sizeof(gmm_attach_type));
  qsh_mdump_collect_high(&nas_emm_power_off_detach_type,sizeof(nas_emm_power_off_detach_type));
  qsh_mdump_collect_high(&nas_emm_tau_cs_rej,sizeof(nas_emm_tau_cs_rej));
  qsh_mdump_collect_high(&nas_emm_tau_cs_rej_tau_type,sizeof(nas_emm_tau_cs_rej_tau_type));
  qsh_mdump_collect_high(&emm_3gpp_spec_version,sizeof(emm_3gpp_spec_version));
  qsh_mdump_collect_high(&gmm_current_lai,sizeof(gmm_current_lai));
  qsh_mdump_collect_high(&gmm_current_rac,sizeof(gmm_current_rac));
  qsh_mdump_collect_high(&gmm_detach_after_rau_pending,sizeof(gmm_detach_after_rau_pending));
  qsh_mdump_collect_high(&gmm_detach_in_progress,sizeof(gmm_detach_in_progress));
  qsh_mdump_collect_high(&gmm_detach_started,sizeof(gmm_detach_started));
  qsh_mdump_collect_high(&gmm_detach_type,sizeof(gmm_detach_type));
  qsh_mdump_collect_high(&gmm_follow_on_request,sizeof(gmm_follow_on_request));
  qsh_mdump_collect_high(&gmm_force_registration,sizeof(gmm_force_registration));
  qsh_mdump_collect_high(&gmm_foreground_search_pending,sizeof(gmm_foreground_search_pending));
  qsh_mdump_collect_high(&gmm_gprs_attach_pending,sizeof(gmm_gprs_attach_pending));
  qsh_mdump_collect_high(&gmm_gprs_detach_pending,sizeof(gmm_gprs_detach_pending));
  qsh_mdump_collect_high(&gmm_integrity_protection_activated,sizeof(gmm_integrity_protection_activated));
  qsh_mdump_collect_high(&gmm_irat_lu_pending,sizeof(gmm_irat_lu_pending));
  qsh_mdump_collect_high(&gmm_is_congestion_due_to_rau,sizeof(gmm_is_congestion_due_to_rau));
  qsh_mdump_collect_high(&gmm_is_lte_advertised_in_last_reg,sizeof(gmm_is_lte_advertised_in_last_reg));

  #ifdef FEATURE_NAS_REL11
  qsh_mdump_collect_high(&gmm_is_lw_to_g_interrat_in_progress,sizeof(gmm_is_lw_to_g_interrat_in_progress));
  #endif

  qsh_mdump_collect_high(&gmm_is_prev_mode_connected,sizeof(gmm_is_prev_mode_connected));
  qsh_mdump_collect_high(&gmm_is_suspended,sizeof(gmm_is_suspended));
  qsh_mdump_collect_high(&gmm_last_signalling_message_type,sizeof(gmm_last_signalling_message_type));
  qsh_mdump_collect_high(&gmm_last_signalling_sent_rat,sizeof(gmm_last_signalling_sent_rat));
  qsh_mdump_collect_high(&gmm_pending_ra_update_type,sizeof(gmm_pending_ra_update_type));
  qsh_mdump_collect_high(&gmm_periodic_rau_pending,sizeof(gmm_periodic_rau_pending));
  qsh_mdump_collect_high(&gmm_power_down_active,sizeof(gmm_power_down_active));
  qsh_mdump_collect_high(&gmm_previous_service_state,sizeof(gmm_previous_service_state));
  qsh_mdump_collect_high(&gmm_previous_serving_rai,sizeof(gmm_previous_serving_rai));
  qsh_mdump_collect_high(&gmm_ps_fop_req,sizeof(gmm_ps_fop_req));
  qsh_mdump_collect_high(&gmm_ps_fop_resp,sizeof(gmm_ps_fop_resp));
  qsh_mdump_collect_high(&gmm_reject_cause,sizeof(gmm_reject_cause));
  qsh_mdump_collect_high(&gmm_state,sizeof(gmm_state));
  qsh_mdump_collect_high(&mm_state,sizeof(mm_state));
  qsh_mdump_collect_high(&gmm_substate,sizeof(gmm_substate));
  qsh_mdump_collect_high(&gmm_update_status,sizeof(gmm_update_status));
  qsh_mdump_collect_high(&gprs_attach_attempt_counter,sizeof(gprs_attach_attempt_counter));
  qsh_mdump_collect_high(&gprs_attach_upon_entry_to_a_new_cell,sizeof(gprs_attach_upon_entry_to_a_new_cell));
  qsh_mdump_collect_high(&t3310_timeout_counter,sizeof(t3310_timeout_counter));
  qsh_mdump_collect_high(&t3330_timeout_counter,sizeof(t3330_timeout_counter));
  qsh_mdump_collect_high(&gmm_imeisv_required,sizeof(gmm_imeisv_required));
  qsh_mdump_collect_high(&gmm_edge_multislot_class,sizeof(gmm_edge_multislot_class));
  qsh_mdump_collect_high(&mm_gstk_send_location_status,sizeof(mm_gstk_send_location_status));
  qsh_mdump_collect_high(&mm_csfb_mt_call_info,sizeof(mm_csfb_mt_call_info));
  qsh_mdump_collect_high(&mm_csfb_state,sizeof(mm_csfb_state));
  qsh_mdump_collect_high(&mm_mapped_gw_identity,sizeof(mm_mapped_gw_identity));
  qsh_mdump_collect_high(&mm_service_loss_on_multimode_rat,sizeof(mm_service_loss_on_multimode_rat));
  qsh_mdump_collect_high(&mm_last_paging_cause,sizeof(mm_last_paging_cause));
  qsh_mdump_collect_high(&mm_subs_capability_mask,sizeof(mm_subs_capability_mask));
  qsh_mdump_collect_high(&mm_nreg_release_pending,sizeof(mm_nreg_release_pending));
  qsh_mdump_collect_high(&mm_3gpp_t3246_plmn_id,sizeof(mm_3gpp_t3246_plmn_id));
  qsh_mdump_collect_high(&mm_lu_pended_nmo_unknown,sizeof(mm_lu_pended_nmo_unknown));
  qsh_mdump_collect_high(&mm_ps_detach_state,sizeof(mm_ps_detach_state));
  qsh_mdump_collect_high(&mm_reg_req_revd_active_call,sizeof(mm_reg_req_revd_active_call));
  qsh_mdump_collect_high(&mm_rlf_stop_mode_pending,sizeof(mm_rlf_stop_mode_pending));
  qsh_mdump_collect_high(&mm_stop_mode_req_pended,sizeof(mm_stop_mode_req_pended));
  qsh_mdump_collect_high(&mm_waiting_for_abort_cnf,sizeof(mm_waiting_for_abort_cnf));
  qsh_mdump_collect_high(&call_type_for_rrc_est_req,sizeof(call_type_for_rrc_est_req));
  qsh_mdump_collect_high(&mm_waiting_for_open_session_cnf,sizeof(mm_waiting_for_open_session_cnf));
  qsh_mdump_collect_high(&mm_wcdma_rlf_state,sizeof(mm_wcdma_rlf_state));
  qsh_mdump_collect_high(&mm_wrlf_rat_change_pending,sizeof(mm_wrlf_rat_change_pending));
  qsh_mdump_collect_high(&(mmcoord_mmr_reg_cnf.ue_oos),sizeof(mmcoord_mmr_reg_cnf.ue_oos)); 
  qsh_mdump_collect_high(&(mmcoord_mmr_reg_cnf.only_for_reg),sizeof(mmcoord_mmr_reg_cnf.only_for_reg));
  qsh_mdump_collect_high(&(mmcoord_mmr_reg_cnf.cause),sizeof(mmcoord_mmr_reg_cnf.cause));
  qsh_mdump_collect_high(&mmcoord_reg_req_pending,sizeof(mmcoord_reg_req_pending));
  qsh_mdump_collect_high(&mmcoord_svc_cnf_pending,sizeof(mmcoord_svc_cnf_pending));
  qsh_mdump_collect_high(&full_srv_req_in_emerg,sizeof(full_srv_req_in_emerg));
  qsh_mdump_collect_high(&GtoWCCO_state,sizeof(GtoWCCO_state));
  qsh_mdump_collect_high(&mm_abort_dual_switch_pending,sizeof(mm_abort_dual_switch_pending));
  qsh_mdump_collect_high(&mm_band_pref,sizeof(mm_band_pref));
  qsh_mdump_collect_high(&mm_cnm_is_active,sizeof(mm_cnm_is_active));
  qsh_mdump_collect_high(&mm_cnm_rel_req_received,sizeof(mm_cnm_rel_req_received));
  qsh_mdump_collect_high(&mm_cnm_srv_est_cause,sizeof(mm_cnm_srv_est_cause));
  qsh_mdump_collect_high(&mm_connection_activated,sizeof(mm_connection_activated));
  qsh_mdump_collect_high(&mm_connection_type,sizeof(mm_connection_type));
  qsh_mdump_collect_high(&mm_cs_access_barred_to_unbarred,sizeof(mm_cs_access_barred_to_unbarred));
  qsh_mdump_collect_high(&mm_csfb_is_blind_redir_to_lte_req_to_be_sent,sizeof(mm_csfb_is_blind_redir_to_lte_req_to_be_sent));
  qsh_mdump_collect_high(&mm_csfb_service_ind_status,sizeof(mm_csfb_service_ind_status));
  qsh_mdump_collect_high(&mm_csg_search_type,sizeof(mm_csg_search_type));
  qsh_mdump_collect_high(&mm_current_ue_mode,sizeof(mm_current_ue_mode));
  qsh_mdump_collect_high(&mm_deep_sleep_alt_scan,sizeof(mm_deep_sleep_alt_scan));
  qsh_mdump_collect_high(&mm_deep_sleep_pending,sizeof(mm_deep_sleep_pending));
  qsh_mdump_collect_high(&mm_dual_mode,sizeof(mm_dual_mode));
  qsh_mdump_collect_high(&mm_follow_on_proceed,sizeof(mm_follow_on_proceed));
  qsh_mdump_collect_high(&mm_follow_on_request,sizeof(mm_follow_on_request));
  qsh_mdump_collect_high(&mm_force_lu,sizeof(mm_force_lu));
  qsh_mdump_collect_high(&mm_hard_abort_pending,sizeof(mm_hard_abort_pending));
  qsh_mdump_collect_high(&mm_held_cnm_service_req,sizeof(mm_held_cnm_service_req));
  qsh_mdump_collect_high(&mm_idle_substate,sizeof(mm_idle_substate));
  qsh_mdump_collect_high(&mm_imsi_is_native,sizeof(mm_imsi_is_native));
  qsh_mdump_collect_high(&mm_initiate_lu_upon_entry_into_new_cell,sizeof(mm_initiate_lu_upon_entry_into_new_cell));
  qsh_mdump_collect_high(&mm_integrity_protected,sizeof(mm_integrity_protected));
  qsh_mdump_collect_high(&mm_is_cs_connected_due_to_srvcc,sizeof(mm_is_cs_connected_due_to_srvcc));
  qsh_mdump_collect_high(&mm_is_foreground_search_to_be_initiated,sizeof(mm_is_foreground_search_to_be_initiated));
  qsh_mdump_collect_high(&mm_last_active_rat_stopped,sizeof(mm_last_active_rat_stopped));
  qsh_mdump_collect_high(&mm_lu_reject_cause,sizeof(mm_lu_reject_cause));
  qsh_mdump_collect_high(&mm_lu_start_reason,sizeof(mm_lu_start_reason));
  qsh_mdump_collect_high(&mm_managed_roaming_enabled,sizeof(mm_managed_roaming_enabled));
  qsh_mdump_collect_high(&mm_multimode_rat_change_pending,sizeof(mm_multimode_rat_change_pending));
  qsh_mdump_collect_high(&mm_nreg_req_received,sizeof(mm_nreg_req_received));
  qsh_mdump_collect_high(&mm_nv_nas_rel_compliance,sizeof(mm_nv_nas_rel_compliance));
  qsh_mdump_collect_high(&mm_op_mode,sizeof(mm_op_mode));
  qsh_mdump_collect_high(&mm_pended_stop_mode_req,sizeof(mm_pended_stop_mode_req));
  qsh_mdump_collect_high(&mm_rat_pri_list_info,sizeof(mm_rat_pri_list_info));
  qsh_mdump_collect_high(&mm_serving_plmn,sizeof(mm_serving_plmn));
  qsh_mdump_collect_high(&mm_t3246_backoff_timer_value,sizeof(mm_t3246_backoff_timer_value));
  qsh_mdump_collect_high(&mm_stored_per_ms_t3212_value,sizeof(mm_stored_per_ms_t3212_value));
  qsh_mdump_collect_high(&mm_serving_cell_id,sizeof(mm_serving_cell_id));
  qsh_mdump_collect_high(mm_previous_serving_lai,sizeof(mm_previous_serving_lai)); 
  qsh_mdump_collect_high(&mm_pending_lu_request,sizeof(mm_pending_lu_request));
  qsh_mdump_collect_high(&mm_pending_lu_type,sizeof(mm_pending_lu_type));
  qsh_mdump_collect_high(&mm_possible_imsi_attach,sizeof(mm_possible_imsi_attach));
  qsh_mdump_collect_high(&mm_previous_idle_substate,sizeof(mm_previous_idle_substate));
  qsh_mdump_collect_high(&mm_ps_access_barred_to_unbarred,sizeof(mm_ps_access_barred_to_unbarred));
  qsh_mdump_collect_high(&mm_ps_detach_type,sizeof(mm_ps_detach_type));
  qsh_mdump_collect_high(&mm_rat_change_pending,sizeof(mm_rat_change_pending));
  qsh_mdump_collect_high(&mm_rat_handler_status,sizeof(mm_rat_handler_status));
  qsh_mdump_collect_high(&mm_ready_to_send_reg_cnf,sizeof(mm_ready_to_send_reg_cnf));
  qsh_mdump_collect_high(&mm_received_suitable_search_end_ind,sizeof(mm_received_suitable_search_end_ind));
  qsh_mdump_collect_high(&mm_reg_cause,sizeof(mm_reg_cause));
  qsh_mdump_collect_high(&mm_reg_waiting_for_reg_cnf,sizeof(mm_reg_waiting_for_reg_cnf));
  qsh_mdump_collect_high(&mm_reject_cause,sizeof(mm_reject_cause));
  qsh_mdump_collect_high(&mm_rejected_csg_list,sizeof(mm_rejected_csg_list));
  qsh_mdump_collect_high(&mm_serv_change_pending,sizeof(mm_serv_change_pending));
  qsh_mdump_collect_high(&mm_service_ind_blocked_due_to_cs_reg,sizeof(mm_service_ind_blocked_due_to_cs_reg));
  qsh_mdump_collect_high(&mm_service_ind_blocked_due_to_irat_cs_reg,sizeof(mm_service_ind_blocked_due_to_irat_cs_reg));
  qsh_mdump_collect_high(mm_serving_lai,sizeof(mm_serving_lai));  
  qsh_mdump_collect_high(&mm_sms_is_active,sizeof(mm_sms_is_active));
  qsh_mdump_collect_high(&mm_sms_preferred_domain_id,sizeof(mm_sms_preferred_domain_id));
  qsh_mdump_collect_high(&mm_stop_mode_local_detach_peding,sizeof(mm_stop_mode_local_detach_peding));
  qsh_mdump_collect_high(&mm_store_integrity_protected,sizeof(mm_store_integrity_protected));
  qsh_mdump_collect_high(&mm_stored_imsi,sizeof(mm_stored_imsi));
  qsh_mdump_collect_high(&mm_sub_config,sizeof(mm_sub_config));
  qsh_mdump_collect_high(mm_timer_status,sizeof(mm_timer_status)); 
  qsh_mdump_collect_high(&mm_to_only_reg,sizeof(mm_to_only_reg));
  qsh_mdump_collect_high(&mm_waiting_for_csmt_response,sizeof(mm_waiting_for_csmt_response));
  qsh_mdump_collect_high(&mm_waiting_for_mocall_connect,sizeof(mm_waiting_for_mocall_connect));
  qsh_mdump_collect_high(&mm_waiting_for_deact_cnf,sizeof(mm_waiting_for_deact_cnf));
  qsh_mdump_collect_high(&mm_waiting_for_ph_status_change_cnf,sizeof(mm_waiting_for_ph_status_change_cnf));
  qsh_mdump_collect_high(&mm_waiting_for_service_cnf,sizeof(mm_waiting_for_service_cnf));
  qsh_mdump_collect_high(&mm_waiting_for_stop_mode_cnf,sizeof(mm_waiting_for_stop_mode_cnf));
  qsh_mdump_collect_high(&WtoGCCO_state,sizeof(WtoGCCO_state));
  qsh_mdump_collect_high(&mm_plmn_list_req_pending,sizeof(mm_plmn_list_req_pending));
  qsh_mdump_collect_high(&mm_plmn_search_in_progress,sizeof(mm_plmn_search_in_progress));
  qsh_mdump_collect_high(&mm_waiting_for_list_cnf_type,sizeof(mm_waiting_for_list_cnf_type));
  qsh_mdump_collect_high(&mm_interrat_state,sizeof(mm_interrat_state));
  qsh_mdump_collect_high(&mm_service_loss_on_rat,sizeof(mm_service_loss_on_rat));
  qsh_mdump_collect_high(&gmm_rat_chng_pended_msg_cause,sizeof(gmm_rat_chng_pended_msg_cause));
  qsh_mdump_collect_high(&gmm_rat_chng_pended_msg_valid,sizeof(gmm_rat_chng_pended_msg_valid));
  qsh_mdump_collect_high(&mm_cs_domain_possible_reest,sizeof(mm_cs_domain_possible_reest));
  qsh_mdump_collect_high(&mm_ps_domain_possible_reest,sizeof(mm_ps_domain_possible_reest));
  qsh_mdump_collect_high(&pending_gmm_msg_after_release,sizeof(pending_gmm_msg_after_release));
  qsh_mdump_collect_high(&pending_gmm_msg_cause,sizeof(pending_gmm_msg_cause));
  qsh_mdump_collect_high(pending_gmm_msg,sizeof(pending_gmm_msg));
  qsh_mdump_collect_high(&pending_gmm_msg_valid,sizeof(pending_gmm_msg_valid));
  qsh_mdump_collect_high(&pending_mm_msg_after_release,sizeof(pending_mm_msg_after_release));
  qsh_mdump_collect_high(&pending_mm_msg_cause,sizeof(pending_mm_msg_cause));
  qsh_mdump_collect_high(pending_mm_msg,sizeof(pending_mm_msg));
  qsh_mdump_collect_high(&pending_mm_msg_valid,sizeof(pending_mm_msg_valid));
  qsh_mdump_collect_high(&ps_session_status,sizeof(ps_session_status));
  qsh_mdump_collect_high(&rrc_connection_status,sizeof(rrc_connection_status));
  qsh_mdump_collect_high(&cs_session_status,sizeof(cs_session_status));
  qsh_mdump_collect_high(&mm_sim_active_allowed_csg_list,sizeof(mm_sim_active_allowed_csg_list));
  qsh_mdump_collect_high(&mm_sim_active_csg_indicator_list,sizeof(mm_sim_active_csg_indicator_list));
  qsh_mdump_collect_high(&mm_sim_active_operator_csg_list,sizeof(mm_sim_active_operator_csg_list));
  qsh_mdump_collect_high(&mm_sim_allowed_csg_list_read_started,sizeof(mm_sim_allowed_csg_list_read_started));
  qsh_mdump_collect_high(&mm_sim_allowed_csg_list,sizeof(mm_sim_allowed_csg_list));
  qsh_mdump_collect_high(&mm_sim_allowed_csg_num_records,sizeof(mm_sim_allowed_csg_num_records));
  qsh_mdump_collect_high(&mm_sim_allowed_csg_record_length,sizeof(mm_sim_allowed_csg_record_length));
  qsh_mdump_collect_high(&mm_sim_card_mode,sizeof(mm_sim_card_mode));
  qsh_mdump_collect_high(&mm_sim_mmgsdi_info,sizeof(mm_sim_mmgsdi_info));
  qsh_mdump_collect_high(emm_ctrl_data_ptr,sizeof(emm_ctrl_data_ptr));
}

#if defined FEATURE_DUAL_SIM
  mm_as_id = temp_as_id;
#endif

#if defined(FEATURE_DUAL_SIM) ||defined(FEATURE_SGLTE)
  qsh_mdump_collect_high(&mm_sim_max_subscription,sizeof(mm_sim_max_subscription)); 
#endif


  qsh_mdump_collect_high(&emm_disable_geran_cap,sizeof(emm_disable_geran_cap)); 
  qsh_mdump_collect_high(&mm_as_id,sizeof(mm_as_id)); 
  qsh_mdump_collect_high(&mm_reg_ind_sent,sizeof(mm_reg_ind_sent)); 
  qsh_mdump_collect_high(&mm_retry_em_call_on_access_blocked_cell,sizeof(mm_retry_em_call_on_access_blocked_cell)); 
  qsh_mdump_collect_high(mm_emm_debug_buffer,sizeof(mm_emm_debug_buffer)); 
  qsh_mdump_collect_high(&mm_emm_debug_buffer_index,sizeof(mm_emm_debug_buffer_index)); 
  qsh_mdump_collect_high(&mm_emm_is_qrd_device,sizeof(mm_emm_is_qrd_device)); 
  qsh_mdump_collect_high(&mm_emm_manual_search_timer_value,sizeof(mm_emm_manual_search_timer_value)); 
  qsh_mdump_collect_high(&nas_emm_index,sizeof(nas_emm_index));
  qsh_mdump_collect_high(&total_num_messages,sizeof(total_num_messages));  
  qsh_mdump_collect_high(&detach_reason,sizeof(detach_reason)); 
  qsh_mdump_collect_high(&service_request,sizeof(service_request)); 
  qsh_mdump_collect_high(&nas_sec_params,sizeof(nas_sec_params)); 
  qsh_mdump_collect_high(&mmode_nv_setting_for_device_mode,sizeof(mmode_nv_setting_for_device_mode));
  qsh_mdump_collect_high(&nas_is_qmss_enabled,sizeof(nas_is_qmss_enabled)); 
  qsh_mdump_collect_high(&mm_optimised_ps_signalling_connection_release,sizeof(mm_optimised_ps_signalling_connection_release)); 
  qsh_mdump_collect_high(&mm_managed_roaming_retry_lu_in_manual_mode,sizeof(mm_managed_roaming_retry_lu_in_manual_mode)); 
  qsh_mdump_collect_high(&mm_nv_conn_mode_manual_search,sizeof(mm_nv_conn_mode_manual_search)); 
  qsh_mdump_collect_high(&mm_dual_standby_mode,sizeof(mm_dual_standby_mode)); 
  qsh_mdump_collect_high(&mm_sub_id,sizeof(mm_sub_id)); 

}
#endif


#ifdef FEATURE_QSH_EVENT_METRIC
/*===========================================================================
Function: mm_post_qsh_message_ind()

Description:
Post a message to own task on QSH call back
============================================================================*/

void mm_post_qsh_message_ind(qsh_client_cb_params_s *param)
{
  mm_cmd_type *mm_cmd_p;
  if ((mm_cmd_p = mm_get_cmd_buf()) != NULL)
  {
      /*
      ** Construct the MM_QSH_MESSAGE_IND command.
      */
      mm_cmd_p->cmd.qsh_message_ind.message_header.message_set = MS_MM_MM;
      mm_cmd_p->cmd.qsh_message_ind.message_header.message_id = MM_QSH_MESSAGE_IND;

      mm_cmd_p->cmd.qsh_message_ind.message_header.message_len_lsb =
                 (sizeof(mm_qsh_message_ind_s_type) - sizeof(IMH_T)) % 256;
      mm_cmd_p->cmd.qsh_message_ind.message_header.message_len_msb =
                 (sizeof(mm_qsh_message_ind_s_type) - sizeof(IMH_T)) / 256;

      mm_cmd_p->cmd.qsh_message_ind.param = *param;
      /*
      ** Put on MM command queue
      */
      mm_put_cmd(mm_cmd_p);
  }
  else
  {
    MSG_FATAL_DS(MM_SUB,"=MM= Error: Not able to Allocate mm_cmd_type Buffer", 0, 0, 0);
  }
}

#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
/*===========================================================================
Function: mm_configure_qsh_event()
Description: Configure (Enable/disable) MM events for QSH 
============================================================================*/
void mm_configure_qsh_event
(
  qsh_client_cb_params_s *param
)
{
  byte action = (byte)(param->action_params.cfg.cmd_code & 0x000000FF); /* 1st byte from LSB */
  byte evt = (byte)((param->action_params.cfg.cmd_code >> 8) & 0x000000FF); /* 2nd byte from LSB */
#ifdef FEATURE_DUAL_SIM
  byte sub_id = (byte)((param->action_params.cfg.cmd_code >> 16) & 0x000000FF); /* 3rd byte from LSB */
#endif

  if((evt >= (byte)MM_QSH_EVENT_MAX) || 
#ifdef FEATURE_DUAL_SIM
     (sub_id >= MAX_AS_IDS) || 
#endif
     (action > 1))
  {
#ifdef FEATURE_DUAL_SIM
    MSG_ERROR_DS_3(MM_SUB,"=MM= Invalid QSH configuration for Event %d SUB ID %d action %d", evt, sub_id, action);
#else
    MSG_ERROR_DS_2(MM_SUB,"=MM= Invalid QSH configuration for Event %d action %d", evt, action);
#endif
    return;
  }

#ifdef FEATURE_DUAL_SIM
  mm_qsh_event_enabled_sim[sub_id][evt] = (boolean)(action ^ 0x01); /* 0 is enable, 1 is disable */
#else
  mm_qsh_event_enabled[evt] = (boolean)(action ^ 0x01);
#endif

}
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif

/*===========================================================================
Function: mm_qsh_debug_callback()

Description:
Registered QSH call back

Note:
   All the functions that can be called for QSH dump collect should use attribute MM_QSH_ATTR_UNCOMPRESS
   as part of function prototype. Otherwise functions wont be accessible in error fatal context if they are compressed
============================================================================*/
#ifdef FEATURE_QSH_MDUMP 
		QSH_MDUMP_FN_ATTR
#endif
void mm_qsh_debug_callback
(
  qsh_client_cb_params_s *param
)
{
  ASSERT(param != NULL);
  switch(param->action)
  {
#ifdef FEATURE_QSH_EVENT_METRIC
    case QSH_ACTION_METRIC_CFG:
      mm_post_qsh_message_ind(param);
      break;
#endif
#ifdef FEATURE_QSH_DUMP
    case QSH_ACTION_DUMP_COLLECT:
      mm_populate_qsh_dump_data(param);
      break;
#endif

#ifdef FEATURE_QSH_MDUMP 
     case QSH_ACTION_MDUMP:
      mm_qsh_mdump_handler();
      break;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_TO_QSH
    case QSH_ACTION_CFG:
      mm_configure_qsh_event(param);
      break;
#endif

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
     #error code not present
#endif


    default:
      break;
  }
}

#endif

#if defined FEATURE_SGLTE || defined FEATURE_1XSRLTE || defined FEATURE_SVLTE_DUAL_SIM
#ifdef FEATURE_GSM
void mm_send_rr_ue_mode_change_ind(sys_ue_mode_e_type   ue_mode)
{
   rr_ue_mode_ind_T   rr_ue_mode_ind;
    memset(&rr_ue_mode_ind,0x00,sizeof(rr_ue_mode_ind_T));
       MSG_HIGH_DS_0(MM_SUB, "=MM= Sending RR_MODE_CHANGE_IND to RR");
        rr_ue_mode_ind.message_header.message_set = MS_MM_RR;
        rr_ue_mode_ind.message_header.message_id = (int) RR_UE_MODE_IND;
        rr_ue_mode_ind.ue_mode = ue_mode;
        PUT_IMH_LEN( sizeof( rr_ue_mode_ind_T ) - sizeof(IMH_T),
                 &rr_ue_mode_ind.message_header );
        mm_send_message((IMH_T *)&rr_ue_mode_ind,GS_QUEUE_RR );
}
#endif

void mmcoord_process_mmr_ue_mode_change_ind( mm_cmd_type *message)
{
  sys_ue_mode_e_type             mm_ue_mode;
  byte    null_lai[LAI_SIZE] = { 0xff, 0xff, 0xff, 0, 0 };

#ifdef FEATURE_SGLTE  
  boolean mode_changed = FALSE;

  /*
  ** Subscription ID represents the SIM subscription. Currently  subscription ID
  ** for SGLTE device will only be one, so setting the variable accordingly. We have modify
  ** in future once we have two subscriptions in SGLTE device.
  */
  mm_sub_id_stack[MM_SGLTE_STACK_2] = mm_sub_id;
#endif
  if (message->cmd.hdr.message_id == (int) MMR_UE_MODE_CHANGE_IND)
  {
    mm_ue_mode = message->cmd.mm_ue_mode_ind.ue_mode;
#ifdef FEATURE_SGLTE
    if(mm_ue_mode != mm_current_ue_mode)
    {
      if(mm_current_ue_mode == SYS_UE_MODE_SGLTE_HOME || mm_current_ue_mode == SYS_UE_MODE_SGLTE_TEST_ONLY) 
      {
        if((mm_ue_mode != SYS_UE_MODE_SGLTE_HOME) && (mm_ue_mode != SYS_UE_MODE_SGLTE_TEST_ONLY)&&
           (!MM_IS_IN_SGLTE_STATE_4))
        {
          mode_changed = TRUE;
        }
      }
      else if(mm_ue_mode == SYS_UE_MODE_SGLTE_HOME || mm_ue_mode == SYS_UE_MODE_SGLTE_TEST_ONLY)
      {
        mode_changed = TRUE;
        mm_sub_config = SYS_OVERALL_FEATURE_MODE_SGLTE;
      }
    }
#endif

    mm_current_ue_mode = mm_ue_mode;

#if defined(FEATURE_1XSRLTE) || defined(FEATURE_SGLTE)
    emm_update_sub_ue_mode(message->cmd.mm_ue_mode_ind.ue_mode_substate_srlte);
#endif
    MSG_HIGH_DS_2( MM_SUB, "=MM= MM received MMR_UE_MODE_CHANGE_IND with ue mode %d and sub ue mode %d",
                                                                    mm_ue_mode,message->cmd.mm_ue_mode_ind.ue_mode_substate_srlte);
#ifdef FEATURE_SGLTE
    if ( (SYS_UE_MODE_SGLTE_HOME == mm_ue_mode)|| (mm_ue_mode==SYS_UE_MODE_SGLTE_TEST_ONLY) )
     {
       mm_sglte_mode=TRUE;
       HYBRID_STACK_DATA_COPY(mm_sim_card_mode_sim[mm_sub_id],mm_sim_card_mode_sim[mm_sub_id])
       //HYBRID_STACK_DATA_COPY(mm_sim_mode_pref_sim[MM_AS_ID_1],mm_sim_mode_pref_sim[MM_AS_ID_1])
       HYBRID_STACK_DATA_COPY(mm_sim_mmgsdi_info_sim[mm_sub_id],mm_sim_mmgsdi_info_sim[mm_sub_id])
       mm_serving_plmn_sim[MM_SGLTE_STACK_2].info.sim_state = 
                                                    mm_serving_plmn_sim[mm_sub_id].info.sim_state;
       mm_sms_preferred_domain_id = CS_PREFERRED;
       if ((mm_serving_plmn_sim[mm_sub_id].info.active_rat == SYS_RAT_GSM_RADIO_ACCESS )||
           (mm_serving_plmn_sim[mm_sub_id].info.active_rat == SYS_RAT_UMTS_RADIO_ACCESS ))
       {
         mm_serving_plmn_sim[mm_sub_id].info.active_rat = SYS_RAT_NONE;
       }
       mm_cs_stack_id = MM_SGLTE_STACK_2;
       mm_ps_stack_id = mm_sub_id;
       if(mode_changed)
       {
         mm_copy_cs_database(MM_SGLTE_STACK_2, mm_sub_id);
         mmcoord_process_spliting();
       }
       mm_idle_transition_timer_sim[mm_cs_stack_id] = NULL;
       memscpy(mm_previous_serving_lai_sim[mm_cs_stack_id],LAI_SIZE,null_lai,LAI_SIZE);
       gmm_current_rac_sim[mm_ps_stack_id] = 255;
       gmm_previous_T3311_gmm_substate_sim[mm_ps_stack_id] = GMM_NULL_SUBSTATE; 
       mm_send_gstk_mm_event(GSTK_IO_MM_SGLTE_IND , mm_ps_stack_id); 
     }
     else
     {
       mm_sms_preferred_domain_id = mm_nv_sms_preferred_domain_id;
       mm_ps_stack_id = mm_sub_id;
       mm_sglte_mode=FALSE;
       mm_sglte_current_stack_status  = SGLTE_INACTIVE_STATE;      
       if(mode_changed)
       {
         mmcoord_process_merging();
       }
       mm_cs_stack_id = mm_sub_id;
       mm_idle_transition_timer_sim[mm_cs_stack_id] = NULL;
       memscpy(mm_previous_serving_lai_sim[mm_cs_stack_id],LAI_SIZE,null_lai,LAI_SIZE);
       gmm_previous_T3311_gmm_substate_sim[mm_ps_stack_id] = GMM_NULL_SUBSTATE; 
       gmm_previous_registration_serving_rai_sim[mm_ps_stack_id].plmn_id.identity[0] = 0xFF;
       gmm_previous_registration_serving_rai_sim[mm_ps_stack_id].plmn_id.identity[1] = 0xFF;
       gmm_previous_registration_serving_rai_sim[mm_ps_stack_id].plmn_id.identity[2] = 0xFF;
       gmm_previous_registration_serving_rai_sim[mm_ps_stack_id].location_area_code.lac[0] = 0xFF;
       gmm_previous_registration_serving_rai_sim[mm_ps_stack_id].location_area_code.lac[1] = 0xFE;
       gmm_previous_registration_serving_rai_sim[mm_ps_stack_id].routing_area_code = 0xFF;

       gmm_current_rac_sim[mm_ps_stack_id] = 255;      
       mm_send_gstk_mm_event(GSTK_IO_MM_CSFB_IND , mm_ps_stack_id);      
     }
#else
     gmm_previous_T3311_gmm_substate = GMM_NULL_SUBSTATE;
     gmm_previous_registration_serving_rai.plmn_id.identity[0] = 0xFF;
     gmm_previous_registration_serving_rai.plmn_id.identity[1] = 0xFF;
     gmm_previous_registration_serving_rai.plmn_id.identity[2] = 0xFF;
     gmm_previous_registration_serving_rai.location_area_code.lac[0] = 0xFF;
     gmm_previous_registration_serving_rai.location_area_code.lac[1] = 0xFE;
     gmm_previous_registration_serving_rai.routing_area_code = 0xFF;

     memscpy(mm_previous_serving_lai,LAI_SIZE,null_lai,LAI_SIZE);
#endif
    if (mm_wcdma_supported &&
	   (mm_sub_is_rat_enabled(SYS_RAT_UMTS_RADIO_ACCESS)  == TRUE))
    {
      /* Send UE mode change indication to WCDMA RRC */
      mm_send_rrc_ue_mode_change_ind(SYS_RAT_UMTS_RADIO_ACCESS,mm_ue_mode);
    }
   
    if (mm_tdscdma_supported &&
	   (mm_sub_is_rat_enabled(SYS_RAT_TDS_RADIO_ACCESS)  == TRUE))
    {
      /* Send UE mode change indication to TD-SCDMA RRC */
      mm_send_rrc_ue_mode_change_ind(SYS_RAT_TDS_RADIO_ACCESS,mm_ue_mode);
    }
#ifdef FEATURE_GSM
    if(mm_sub_is_rat_enabled(SYS_RAT_GSM_RADIO_ACCESS)  == TRUE)
	{
      mm_send_rr_ue_mode_change_ind(mm_ue_mode);
	}
#endif

#ifdef FEATURE_LTE  
     if(mm_sub_is_rat_enabled(SYS_RAT_LTE_RADIO_ACCESS)  == TRUE)
	 {
       emm_send_ue_mode_change_ind(mm_ue_mode,message->cmd.mm_ue_mode_ind.ue_mode_substate_srlte);
	 }
#endif     
  }
}
#endif


void  mmcoord_process_pseudo_list_update_ind(mm_cmd_type *message)
{

  if (message->cmd.hdr.message_id == (int) MMR_PSEUDO_LIST_UPDATE_IND)
  {
    sys_radio_access_tech_e_type     rat;
    rat = message->cmd.mmr_pseudo_list_update_ind.rat;
    if(rat == SYS_RAT_TDS_RADIO_ACCESS || rat == SYS_RAT_GSM_RADIO_ACCESS || rat == SYS_RAT_UMTS_RADIO_ACCESS)
    {
      if(rat == SYS_RAT_GSM_RADIO_ACCESS && mm_serving_plmn.info.active_rat == SYS_RAT_GSM_RADIO_ACCESS)
      {
        mm_send_rr_pseudo_list_update_ind(message->cmd.mmr_pseudo_list_update_ind.can_use_pseudo);
      }
#ifdef FEATURE_TDSCDMA
      else if(rat == SYS_RAT_TDS_RADIO_ACCESS && mm_serving_plmn.info.active_rat == SYS_RAT_TDS_RADIO_ACCESS)
      {
        mm_send_tdsrrc_pseudo_list_update_ind(message->cmd.mmr_pseudo_list_update_ind.can_use_pseudo);
      }
#endif
      else if(rat == SYS_RAT_UMTS_RADIO_ACCESS && mm_serving_plmn.info.active_rat == SYS_RAT_UMTS_RADIO_ACCESS)
      {
        mm_send_rrc_pseudo_list_update_ind(message->cmd.mmr_pseudo_list_update_ind.can_use_pseudo);
      }
      else
      {
        MSG_HIGH_DS_2(MM_SUB, "=MM= Dropping message, active rat %d Rat sent by reg ", mm_serving_plmn.info.active_rat,rat);
      }
    }
    else
    {
      MSG_FATAL_DS(MM_SUB, "=MM= Wrong Rat is sent by REG for Pseudo list update indication", 0,0,0 );
    }
  }

}


#ifdef FEATURE_SGLTE
void  mmcoord_process_merging(void )
{
  mm_as_id_e_type mm_as_id_local = MM_AS_ID_1;
#ifdef FEATURE_LTE  
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
#endif
  MSG_HIGH_DS_0(MM_SUB, "=MM= Processing Merging ");
  mm_sglte_current_stack_status  = SGLTE_INACTIVE_STATE; 
  /* change the mm serving operation mode to cs ps */
  mm_serving_plmn_sim[mm_ps_stack_id].ms_op_mode = SYS_SRV_DOMAIN_CS_PS;
  mm_serving_plmn_sim[mm_cs_stack_id].ms_op_mode = SYS_SRV_DOMAIN_NO_SRV;
   /* copy cs data base */
  mm_copy_cs_database(mm_ps_stack_id,mm_cs_stack_id);
  gmmllc_reset_service_state(); 
  if ((mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain == SYS_SRV_DOMAIN_PS_ONLY ) &&
      (mm_serving_plmn_sim[mm_cs_stack_id].info.reg_service_domain == SYS_SRV_DOMAIN_CS_ONLY ))
  {
   mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain = SYS_SRV_DOMAIN_CS_PS;
   mmcoord_mmr_reg_cnf_sim[mm_ps_stack_id].service_state.reg_service_domain    =  SYS_SRV_DOMAIN_CS_PS;
  }
  else if ((mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain == SYS_SRV_DOMAIN_NO_SRV ) &&
      (mm_serving_plmn_sim[mm_cs_stack_id].info.reg_service_domain == SYS_SRV_DOMAIN_CS_ONLY ))
      {
        mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain = SYS_SRV_DOMAIN_CS_ONLY;
        mmcoord_mmr_reg_cnf_sim[mm_ps_stack_id].service_state.reg_service_domain    =  SYS_SRV_DOMAIN_CS_ONLY;
      }

  if ( mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state !=SYS_SIM_STATE_NOT_AVAILABLE  )
  {
    if((mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state== SYS_SIM_STATE_AVAILABLE) &&
          (mm_serving_plmn_sim[mm_cs_stack_id].info.sim_state== SYS_SIM_STATE_CS_INVALID))
    {
      mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state = SYS_SIM_STATE_CS_INVALID;
    }
    else if((mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state== SYS_SIM_STATE_PS_INVALID) &&
         (mm_serving_plmn_sim[mm_cs_stack_id].info.sim_state== SYS_SIM_STATE_CS_INVALID))
    {
      mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state = SYS_SIM_STATE_CS_PS_INVALID;
    }
    
  }

  if(mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state == SYS_SIM_STATE_PS_INVALID)
  {
    mmcoord_pending_reg_message_sim[mm_ps_stack_id].cmd.mmr_reg_req.network_selection_mode = mmcoord_pending_reg_message_sim[mm_cs_stack_id].cmd.mmr_reg_req.network_selection_mode;
    mmcoord_pending_reg_message_sim[mm_ps_stack_id].cmd.mmr_reg_req.plmn = mmcoord_pending_reg_message_sim[mm_cs_stack_id].cmd.mmr_reg_req.plmn;
    mmcoord_pending_reg_message_sim[mm_ps_stack_id].cmd.mmr_reg_req.rat = mmcoord_pending_reg_message_sim[mm_cs_stack_id].cmd.mmr_reg_req.rat;
#ifdef FEATURE_LTE
    if(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL)
    {
      emm_ctrl_data_ptr->reg_req_pending_ptr->network_selection_mode = mmcoord_pending_reg_message_sim[mm_cs_stack_id].cmd.mmr_reg_req.network_selection_mode;
      emm_ctrl_data_ptr->reg_req_pending_ptr->plmn = mmcoord_pending_reg_message_sim[mm_cs_stack_id].cmd.mmr_reg_req.plmn;
      emm_ctrl_data_ptr->reg_req_pending_ptr->rat = mmcoord_pending_reg_message_sim[mm_cs_stack_id].cmd.mmr_reg_req.rat;
    }
#endif
  }

  mm_serving_plmn_sim[mm_ps_stack_id].info.active_rat  =  mm_serving_plmn_sim[mm_cs_stack_id].info.active_rat  ;
  MSG_HIGH_DS_1(MM_SUB,  "=MM= Merging Sim state ", mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state);
  /* atfer merge mains atck wont wait for any service cnf from lower layers.*/
  mm_waiting_for_service_cnf_sim[mm_ps_stack_id]  = FALSE; 

  /* make stake 2 database as deactivated look */
  mm_invalidate_cs_stack_database();
  if(gmm_state_sim[mm_ps_stack_id] == GMM_DEREGISTERED)
  {
    gmm_substate_sim[mm_ps_stack_id] = GMM_ATTACH_NEEDED;
  }
  else
  {
    gmm_substate_sim[mm_ps_stack_id] = GMM_UPDATE_NEEDED;
  }
  gmm_sglte_reset_ps_attempt_counters();


    gmm_previous_gprs_service_state_sim[mm_ps_stack_id]=SERVICE_NOT_AVAILABLE;
#ifdef FEATURE_SGLTE  
  mm_cs_auth_sent_to_card_sim[mm_ps_stack_id] = FALSE;
  mm_cs_auth_sent_to_card_sim[mm_cs_stack_id] = FALSE;
  mm_ps_auth_sent_to_card_sim[mm_ps_stack_id] = FALSE;
  mm_ps_auth_sent_to_card_sim[mm_cs_stack_id] = FALSE;
#endif  
    /* make cs stack id and ps stack id as one*/  
  mm_cs_stack_id = mm_ps_stack_id;
    /* Adding this to do rAu even on the same PLMN*/
  gmm_force_registration_sim[mm_ps_stack_id]= TRUE;
  gmm_current_rac_sim[mm_ps_stack_id] = 255; 
  gmm_previous_network_op_mode_sim[mm_ps_stack_id]= PS_DOMAIN_SYS_INFO_UNAVAILABLE;
  mm_send_gstk_mm_event(GSTK_IO_MM_SGLTE_MERGE_IND ,mm_ps_stack_id); 
#ifdef FEATURE_SGLTE
  if (emm_ctrl_data_ptr->TIN== NAS_MM_TIN_RAT_RELATED_TMSI)
  {
    emm_set_and_write_tin_type(NAS_MM_TIN_P_TMSI); 
  }
#endif

#ifdef FEATURE_GSM_GPRS
  mm_as_id_local = mm_as_id;
  mm_set_as_id(mm_ps_stack_id);
   if ((gmm_state == GMM_REGISTERED) ||
       (gmm_state == GMM_ROUTING_AREA_UPDATING_INITIATED) ||
       (gmm_state == GMM_SERVICE_REQUEST_INITIATED))
  {
    gmmllc_reinit(GMM_STANDBY_STATE) ;
}
  mm_set_as_id(mm_as_id_local);
#endif

}

void mmcoord_process_spliting(void )
{
  mm_as_id_e_type  mm_local_as_id;
#ifdef FEATURE_LTE  
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr->reg_req_pending_ptr != NULL);
#endif

  MSG_HIGH_DS_0(MM_SUB, "=MM= processing Spliting");
  mm_sglte_current_stack_status  = SGLTE_INACTIVE_STATE;
  /* make cs ps stack id as different one */
  mm_cs_stack_id = MM_SGLTE_STACK_2;  
  if (mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain == SYS_SRV_DOMAIN_CS_PS)
  {
    mm_serving_plmn_sim[mm_cs_stack_id].info.reg_service_domain = SYS_SRV_DOMAIN_CS_ONLY;
    mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain = SYS_SRV_DOMAIN_PS_ONLY;
    mmcoord_mmr_reg_cnf_sim[mm_cs_stack_id].service_state.reg_service_domain=SYS_SRV_DOMAIN_CS_ONLY;
    mmcoord_mmr_reg_cnf_sim[mm_ps_stack_id].service_state.reg_service_domain=SYS_SRV_DOMAIN_PS_ONLY;
  }
  else if (mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain == SYS_SRV_DOMAIN_CS_ONLY)
  {
    mm_serving_plmn_sim[mm_cs_stack_id].info.reg_service_domain = SYS_SRV_DOMAIN_CS_ONLY;
    mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain = SYS_SRV_DOMAIN_NO_SRV;
    /*added this condition ,so that cm start recovery*/
    mm_serving_plmn_sim[mm_ps_stack_id].info.service_status = SYS_SRV_STATUS_LIMITED;
    mmcoord_mmr_reg_cnf_sim[mm_cs_stack_id].service_state.reg_service_domain=SYS_SRV_DOMAIN_CS_ONLY;
    mmcoord_mmr_reg_cnf_sim[mm_ps_stack_id].service_state.reg_service_domain=SYS_SRV_DOMAIN_NO_SRV;
  }
  else if (mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain == SYS_SRV_DOMAIN_PS_ONLY)
  {
    mm_serving_plmn_sim[mm_cs_stack_id].info.reg_service_domain = SYS_SRV_DOMAIN_NO_SRV;
    /*making change to reflect correct service status to cm on stack 2*/
    mm_serving_plmn_sim[mm_cs_stack_id].info.service_status = SYS_SRV_STATUS_LIMITED;
    mm_serving_plmn_sim[mm_ps_stack_id].info.reg_service_domain = SYS_SRV_DOMAIN_PS_ONLY;
    mmcoord_mmr_reg_cnf_sim[mm_cs_stack_id].service_state.reg_service_domain=SYS_SRV_DOMAIN_NO_SRV;
    mmcoord_mmr_reg_cnf_sim[mm_ps_stack_id].service_state.reg_service_domain=SYS_SRV_DOMAIN_PS_ONLY;
  }
  if(mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state== SYS_SIM_STATE_AVAILABLE) 
  {
    mm_serving_plmn_sim[mm_cs_stack_id].info.sim_state = SYS_SIM_STATE_AVAILABLE;
  }
  else if(mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state== SYS_SIM_STATE_CS_PS_INVALID) 
  {
    mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state = SYS_SIM_STATE_PS_INVALID;
    mm_serving_plmn_sim[mm_cs_stack_id].info.sim_state = SYS_SIM_STATE_CS_INVALID;
  }
  else if(mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state== SYS_SIM_STATE_PS_INVALID )
  {
    mm_serving_plmn_sim[mm_cs_stack_id].info.sim_state = SYS_SIM_STATE_AVAILABLE;
  }
  else if (mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state== SYS_SIM_STATE_CS_INVALID)
  {
    mm_serving_plmn_sim[mm_ps_stack_id].info.sim_state = SYS_SIM_STATE_AVAILABLE;
    mm_serving_plmn_sim[mm_cs_stack_id].info.sim_state = SYS_SIM_STATE_CS_INVALID;
  }

#ifdef FEATURE_SGLTE
  mm_cs_auth_sent_to_card_sim[mm_ps_stack_id] = FALSE;
  mm_cs_auth_sent_to_card_sim[mm_cs_stack_id] = FALSE;
  mm_ps_auth_sent_to_card_sim[mm_ps_stack_id] = FALSE;
  mm_ps_auth_sent_to_card_sim[mm_cs_stack_id] = FALSE;
#endif
//here need logic for domain

  /* After stack split , reset any pending service cnf from lower layers in hybrid stack*/
  mm_waiting_for_service_cnf_sim[mm_cs_stack_id]  = FALSE; 

  /* change the corresponding mmerving mode to ps only and stacke2 to cs only */
   /* copy cs data base */
 
  mm_serving_plmn_sim[mm_ps_stack_id].ms_op_mode = SYS_SRV_DOMAIN_PS_ONLY;
  mm_serving_plmn_sim[mm_cs_stack_id].ms_op_mode = SYS_SRV_DOMAIN_CS_ONLY;
        /* New changes for the combined TAU */
  mmcoord_pending_reg_message_sim[mm_ps_stack_id].cmd.mmr_reg_req.req_service_domain = SYS_SRV_DOMAIN_PS_ONLY;
  emm_ctrl_data_ptr->reg_req_pending_ptr->req_service_domain = SYS_SRV_DOMAIN_PS_ONLY;
  if ( mm_serving_plmn_sim[mm_ps_stack_id].info.active_rat == SYS_RAT_GSM_RADIO_ACCESS )
  {
    mm_serving_plmn_sim[mm_ps_stack_id].info.active_rat = SYS_RAT_NONE;
  }
  mm_serving_plmn_sim[mm_cs_stack_id].info.active_rat = SYS_RAT_GSM_RADIO_ACCESS;
 //mm_serving_plmn_sim[mm_cs_stack_id].mm_service_state = mm_serving_plmn_sim[mm_ps_stack_id].mm_service_state;
  mm_serving_plmn_sim[mm_ps_stack_id].mm_service_state = SERVICE_NOT_AVAILABLE;
  //mm_serving_plmn_sim[mm_cs_stack_id].mm_service_state=SERVICE_AVAILABLE;
 // mm_idle_substate_sim[mm_cs_stack_id]= MM_NORMAL_SERVICE;
  mm_local_as_id = mm_as_id;
  mm_set_as_id(MM_SGLTE_STACK_2);
  gmmllc_gmm_trigger_llc_event( GMM_TRIGGER_TLLI_UNASSIGNMENT, FALSE, 0 );

  if ((mm_state == MM_IDLE) &&
      ((mm_idle_substate == MM_NORMAL_SERVICE) ||
       (mm_idle_substate == MM_ATTEMPTING_TO_UPDATE)))
  {
#ifdef FEATURE_NAS_REL10
    if(mm_is_per_ms_t3212_present)
    {
      if((mm_stored_per_ms_t3212_value != 0) &&
         (mm_timer_status[TIMER_T3212] == TIMER_STOPPED))
      {
        mm_start_timer( TIMER_T3212,
                        mm_stored_per_ms_t3212_value);
      }
    }
    else
#endif
    if ((mm_system_information.T3212_timer_value != 0 ) &&
        (mm_timer_status[TIMER_T3212] == TIMER_STOPPED))
    {
        /* ------------------------------------------------------
        ** If the (T3212) timer is not already started, the timer
        ** is started each time the mobile enters the MM_IDLE
        ** substate NORMAL_SERVICE or ATTEMPTING_TO_UPDATE
        ** (24.008 section 4.2).
        ** ------------------------------------------------------ */

      mm_start_timer( TIMER_T3212,
                       mm_system_information.T3212_timer_value );
    }
  }
  mm_set_as_id(mm_local_as_id);
#ifdef FEATURE_LTE  
  /* this has been added for the prevention of the unnecessary TAU req*/
  emm_ctrl_data_ptr->sg_association_to_be_established = NAS_EMM_SG_ASSOC_NOT_REQ;
  if (emm_ctrl_data_ptr->TIN== NAS_MM_TIN_RAT_RELATED_TMSI)
  {
    emm_set_and_write_tin_type(NAS_MM_TIN_P_TMSI); 
  }
#endif
  mm_reset_variables(mm_ps_stack_id);
  gmm_current_rac_sim[mm_ps_stack_id] = 255;
  gmm_force_registration_sim[mm_ps_stack_id]= TRUE;
  mm_send_gstk_mm_event(GSTK_IO_MM_SGLTE_SPLIT_IND ,mm_cs_stack_id);

  if(mm_no_service_ind_during_split)
  {
      mm_no_service_ind_during_split = FALSE;
	  mm_serving_plmn_sim[mm_cs_stack_id].info.service_status = SYS_SRV_STATUS_NO_SRV;
      mm_serving_plmn_sim[mm_cs_stack_id].info.plmn.identity[0] = 0xFF;
      mm_serving_plmn_sim[mm_cs_stack_id].info.plmn.identity[1] = 0xFF;
      mm_serving_plmn_sim[mm_cs_stack_id].info.plmn.identity[2] = 0xFF;
      mm_serving_plmn_sim[mm_cs_stack_id].lac.lac[0]			 = 0xFF;
      mm_serving_plmn_sim[mm_cs_stack_id].lac.lac[1]			 = 0xFF;
      mm_serving_plmn_sim[mm_cs_stack_id].gmm_service_state	 = SERVICE_NOT_AVAILABLE;
      mm_serving_plmn_sim[mm_cs_stack_id].mm_service_state 	= SERVICE_NOT_AVAILABLE;
      mm_serving_plmn_sim[mm_cs_stack_id].info.plmn_service_capability = SYS_SRV_CAPABILITY_NOT_AVAILABLE;
      mm_serving_plmn_sim[mm_cs_stack_id].network_op_mode	= PS_DOMAIN_SYS_INFO_UNAVAILABLE;
      mm_serving_plmn_sim[mm_cs_stack_id].rac = 0xFF ;
  }
}

void mmcoord_process_pstimer_expiry_merging(void)
{
   rr_change_mode_ind_T rr_change_mode_ind;   

   memscpy(&rr_change_mode_ind.rat_pri_list_info, sizeof(sys_rat_pri_list_info_s_type),
          &mm_rat_pri_list_info_sim[mm_ps_stack_id],
          sizeof(sys_rat_pri_list_info_s_type));
 rr_change_mode_ind.service_domain = SYS_SRV_DOMAIN_CS_PS;
 /* Removing from here because of new design */
 /* mmsend_mmr_stack_deact_ind();*/
  mmcoord_process_merging();
  mmsend_mmr_service_stat_ind(mm_ps_stack_id);
  
   rr_change_mode_ind.message_header.message_set = MS_MM_RR;
   rr_change_mode_ind.message_header.message_id  = (int)RR_CHANGE_MODE_IND;
   PUT_IMH_LEN( sizeof( rr_change_mode_ind ) - sizeof(IMH_T),
                 &rr_change_mode_ind.message_header );
   /*lint -restore */
   MSG_HIGH_DS_0(MM_SUB," =MM= MM sent RR_CHANGE_MODE_IND");

   mm_send_message( &rr_change_mode_ind.message_header, GS_QUEUE_RR );
}

void mmcoord_process_stop_mode_on_hybrid_stack(void)
{
  mm_as_id_e_type local_as_id;
  local_as_id = mm_as_id;
  mm_set_as_id(mm_cs_stack_id);
  mm_abort_rr( RRC_CS_DOMAIN_CN_ID, FALSE);
  mm_state_control( MM_STD_RR_CONNECTION_ABORTED );
  mm_nreg_req_received = TRUE;
  /* ---------------------------------------------------------------------
       ** Set ATT flag to FALSE so we don't attempt to perform multiple DETACHs
       **------------------------------------------------------------------- */
  mm_system_information.ATT = FALSE;
  mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                   SYS_SRV_DOMAIN_NO_SRV,
                                   TRUE,
                                   SYS_SRV_STATUS_LIMITED,
                                   TRUE );
  mm_set_as_id(local_as_id);
}

#endif


void mm_timer_policy_change_pending_expiry(void)
{
  mmr_service_ind_s_type mmr_service_ind;
  if(mm_serving_plmn.info.service_status != SYS_SRV_STATUS_NO_SRV)
  {
     mmr_service_ind.service_state = mm_serving_plmn.info;
     mmr_service_ind.cause = MMR_CAUSE_PLMN_SEL_CALL_END;
     mm_send_mmr_service_ind( &mmr_service_ind );
  }
}


#ifdef MMOC_API_PH_STAT_CNF_WITH_STATUS
void  mmcoord_process_mmr_sim_busy_ind(mm_cmd_type* message)
{
  mm_is_sim_busy =  message->cmd.mmr_sim_busy_ind.is_sim_busy;

    mm_handle_sim_busy_ind(FALSE);
    gmm_handle_sim_busy_ind(FALSE);


}
#endif

void mm_gmm_process_t3247_expiry(mm_cmd_type* message)
{
  mmr_service_ind_s_type     mmr_service_ind;
  boolean is_plmn_gprs_dos_forbidden = FALSE;
  boolean is_lai_dos_forbidden = FALSE;
#if defined(FEATURE_LTE)
  emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
  emm_ctrl_data_ptr = emm_db_get_ctrl_data();
  ASSERT(emm_ctrl_data_ptr != NULL);
#endif
   mm_timer_status[TIMER_T3247] = TIMER_STOPPED;

   if (
#ifdef FEATURE_DUAL_SIM
   reg_sim_is_plmn_dos_gprs_forbidden(mm_serving_plmn.info.plmn,(sys_modem_as_id_e_type)mm_sub_id)
     
#else
    
   reg_sim_is_plmn_dos_gprs_forbidden(mm_serving_plmn.info.plmn)
#endif
       )
   {
       is_plmn_gprs_dos_forbidden = TRUE;
	   MSG_HIGH_DS_1(MM_SUB, "=MM= is_plmn_gprs_dos_forbidden = %d", is_plmn_gprs_dos_forbidden);
   }
   if (mm_is_lai_forbidden_due_to_dos_mitigation((inter_task_lai_T *) &mm_serving_lai) ||
#ifdef FEATURE_DUAL_SIM
                                 reg_sim_is_plmn_dos_forbidden(mm_serving_plmn.info.plmn,(sys_modem_as_id_e_type)mm_sub_id)
#else
                                  reg_sim_is_plmn_dos_forbidden(mm_serving_plmn.info.plmn)
#endif
       )
   {
       is_lai_dos_forbidden = TRUE;
	   MSG_HIGH_DS_1(MM_SUB, "=MM= is_lai_dos_forbidden = %d", is_lai_dos_forbidden);
   }
   MSG_HIGH_DS_2(MM_SUB, "=MM= mm_emm_max_attempt_counter_dos_mitigation is %d and service status is %d",mm_emm_max_attempt_counter_dos_mitigation, mm_serving_plmn.info.service_status);

   mm_clear_dos_non_dos_forbidden_LAIs(TRUE);
   mm_clear_t3247_specific_lai_reject_list();
   
#if defined(FEATURE_LTE)
#ifdef FEATURE_DUAL_SIM 
   if(mm_subs_capability_mask_sim[mm_sub_id] & SYS_SYS_MODE_MASK_LTE)
#endif
   {
     emm_clear_forbidden_tai_list(emm_ctrl_data_ptr->rrc_forbidden_list_ptr,TRUE);
     emm_clear_forbidden_for_service_list(emm_ctrl_data_ptr->forbidden_for_service_list_ptr,TRUE);
     emm_clear_manual_forbidden_tai_list(TRUE);
   }
#endif

#ifdef FEATURE_DUAL_SIM
   reg_sim_cleanup_dos_fplmn_list_on_t3247_exp((sys_modem_as_id_e_type)mm_sub_id);
#else
   reg_sim_cleanup_dos_fplmn_list_on_t3247_exp();
#endif

   if ( mm_emm_max_attempt_counter_dos_mitigation > 0)
   {
     if (mm_serving_plmn.info.service_status  == SYS_SRV_STATUS_SRV)
      {
         if(is_plmn_gprs_dos_forbidden)
         {
 #ifdef FEATURE_LTE
            mmcoord_reevaluate_rat_pri_list(mm_serving_plmn.info.plmn);
#endif
            if(gmm_state == GMM_DEREGISTERED)
            {
               gmm_set_substate(GMM_ATTEMPTING_TO_ATTACH);
               gmm_initiate_gprs_attach();
            }
            else if(gmm_state == GMM_REGISTERED)
            {
               gmm_set_substate(GMM_ATTEMPTING_TO_UPDATE);
               gmm_initiate_routing_area_update();
            }
         }
      }
      else if ((is_lai_dos_forbidden || is_plmn_gprs_dos_forbidden) &&
                mm_serving_plmn.info.service_status  == SYS_SRV_STATUS_LIMITED)
      {
         if(mm_in_conn_management_state() || mm_is_mt_page_in_progress())
         {
            MSG_HIGH_DS_0(MM_SUB,"Send No service after Call end since call is going on");
			mm_need_to_send_no_srv_on_call_end_due_to_dos = TRUE;
         }
         else
         {
            if(mm_plmn_search_in_progress )
            {
               if (is_umts_mode())
               {
                  mm_send_rrc_bplmn_search_abort_req();
               }
               else
               {
                  mm_send_rr_bplmn_search_abort_req();
               }
            }
            mm_substate_control( MM_STD_NO_SERVICE );
            gmm_set_substate(GMM_NO_CELL_AVAILABLE);
            mm_serving_plmn.mm_service_state = SERVICE_NOT_AVAILABLE;
            mm_serving_plmn.gmm_service_state = SERVICE_NOT_AVAILABLE;
            mm_update_mm_serving_plmn_info( mm_serving_plmn.info.sim_state,
                                            mm_serving_plmn.info.reg_service_domain,
                                            TRUE,
                                            SYS_SRV_STATUS_NO_SRV,
                                            FALSE );
			MSG_HIGH_DS_0(MM_SUB,"Sending No service after T3247 expiry");
            if(mm_reg_waiting_for_reg_cnf)
            {
                mmcoord_mmr_reg_cnf.cause = MMR_CAUSE_NOT_SPECIFIED;
               /* Set the PLMN service stat information in the REG_CNF primitive */
                mmcoord_mmr_reg_cnf.service_state = mm_serving_plmn.info;
                mm_send_mmr_reg_cnf();
                mm_reg_waiting_for_reg_cnf = FALSE;
                mm_ready_to_send_reg_cnf   = FALSE;
            }
            else
            {
                mmr_service_ind.cause = MMR_CAUSE_NOT_SPECIFIED;
                mmr_service_ind.service_state = mm_serving_plmn.info;
                mm_send_mmr_service_ind( &mmr_service_ind );
            }
         }
      }
   }
   else if( mm_emm_max_attempt_counter_dos_mitigation == 0)
   {
#ifdef FEATURE_DUAL_SIM
      if (mm_emm_is_illegal_me_on_any_sub((sys_modem_as_id_e_type)mm_as_id))
      {
         MSG_HIGH_DS_0(MM_SUB,"=MM= Don't send legal subscription upon T3247 expiry as received ILLEGAL ME on any other SUB");
      }
      else
#endif
      {
         if ( (mm_serving_plmn.info.sim_state == SYS_SIM_STATE_PS_INVALID && mm_is_sim_ps_invalid_due_to_dos) ||
          (mm_serving_plmn.info.sim_state == SYS_SIM_STATE_CS_INVALID && mm_is_sim_cs_invalid_due_to_dos) )
     {
#if defined(FEATURE_LTE)
        emm_ctrl_data_ptr->plmn_service_state->sim_state = SYS_SIM_STATE_AVAILABLE;
#endif
        mm_update_sim_state(SYS_SIM_STATE_AVAILABLE);
        mm_send_mmr_sim_state_revalidate_ind(SYS_SIM_STATE_AVAILABLE);
     }
     else if ((mm_is_sim_cs_invalid_due_to_dos || mm_is_sim_ps_invalid_due_to_dos) &&
              ((mm_serving_plmn.info.sim_state == SYS_SIM_STATE_NOT_AVAILABLE) ||
              (mm_serving_plmn.info.sim_state == SYS_SIM_STATE_CS_PS_INVALID)))
     {
        mmgsdi_session_manage_illegal_subscription(mm_sim_mmgsdi_info.session_id, MMGSDI_SESSION_APP_IS_LEGAL);
        if(mm_is_sim_cs_invalid_due_to_dos  && mm_is_sim_ps_invalid_due_to_dos == FALSE)
        {
           mm_is_sim_to_be_ps_invalid_after_power_off = TRUE;
        }
        else if(mm_is_sim_cs_invalid_due_to_dos  == FALSE && mm_is_sim_ps_invalid_due_to_dos)
        {
           mm_is_sim_to_be_cs_invalid_after_power_off = TRUE;
        }
     }
     else
     {
        MSG_HIGH_DS_1(MM_SUB, "=MM= Illegal sim_state %d", mm_serving_plmn.info.sim_state); 
     }
     mm_is_sim_cs_invalid_due_to_dos = FALSE;
     mm_is_sim_ps_invalid_due_to_dos = FALSE;
	 MSG_HIGH_DS_2(MM_SUB, "=MM= SIM need to be ps invalid after power off %d,SIM need to be cs invalid after power off %d", mm_is_sim_to_be_ps_invalid_after_power_off,mm_is_sim_to_be_cs_invalid_after_power_off); 
	}
  }
}
  
void mm_emm_reset_dos_counters_per_plmn
(
	sys_plmn_id_s_type		plmn
#ifdef FEATURE_LTE
	,lte_nas_emm_cause_type  emm_cause
	,lte_nas_message_id 	 msg_id
#endif
	,boolean				 is_eps_only
	,mm_cn_domain_type_T cn_domain_id
	,byte					 mm_gmm_msg_id
  )
  {
	 boolean apply_t3247_feature = mm_get_t3247_behavior_enabled();
	 MSG_HIGH_DS_4(MM_SUB, "=MM= mm_emm_reset_dos_counters(): PLMN = %x %x %x, is_eps_only %d",
				   plmn.identity[0], plmn.identity[1], plmn.identity[2], is_eps_only);
  
	 if (apply_t3247_feature && (mm_emm_max_attempt_counter_dos_mitigation > 0) )
	 {
#ifdef FEATURE_LTE
	   if(is_lte_mode())
		  {
		switch (msg_id)
		{
		  case ATTACH_ACCEPT:
		  case TRACKING_AREA_UPDATE_ACCEPT:
				mm_emm_usim_gprs_counter = 0;
				if (FALSE == is_eps_only)
				{
				   mm_emm_usim_non_gprs_counter = 0;
				}
  
#ifdef FEATURE_DUAL_SIM
				reg_sim_reset_plmn_counter_in_fplmn_list(plmn, (sys_modem_as_id_e_type)mm_sub_id);
#else
				reg_sim_reset_plmn_counter_in_fplmn_list(plmn);
#endif
  
				MSG_HIGH_DS_2(MM_SUB, "=MM= mm_emm_usim_gprs_counter= %d,mm_emm_usim_non_gprs_counter =%d", mm_emm_usim_gprs_counter, mm_emm_usim_non_gprs_counter);
				break;
  
			default:
			  break;
		}
	   }
#endif
	   if(is_gsm_mode() || is_umts_mode())
	   {
		  if(cn_domain_id == MM_CS_DOMAIN_CN_ID)
		  {
					switch (mm_gmm_msg_id) 
					{
					  case LOCATION_UPDATING_ACCEPT:
						   mm_emm_usim_non_gprs_counter = 0;
#ifdef FEATURE_DUAL_SIM
                           reg_sim_remove_plmn_from_dos_mitigation_fplmn_list(plmn, (sys_modem_as_id_e_type)mm_sub_id);
#else
                           reg_sim_remove_plmn_from_dos_mitigation_fplmn_list(plmn);
#endif
			  
							MSG_HIGH_DS_2(MM_SUB, "=MM= mm_emm_usim_gprs_counter= %d,mm_emm_usim_non_gprs_counter =%d", mm_emm_usim_gprs_counter, mm_emm_usim_non_gprs_counter);
						   break;
					  default:
						   break;
					  }
		  }
		  else
		  {
				switch (mm_gmm_msg_id)
					{
					  case GMM_ROUTING_AREA_UPDATE_ACCEPT:
					  case GMM_ATTACH_ACCEPT:
							mm_emm_usim_gprs_counter = 0;
							if (FALSE == is_eps_only)
							{
							   mm_emm_usim_non_gprs_counter = 0;
							}
			  
#ifdef FEATURE_DUAL_SIM
							reg_sim_reset_plmn_counter_in_fplmn_list(plmn, (sys_modem_as_id_e_type)mm_sub_id);
#else
							reg_sim_reset_plmn_counter_in_fplmn_list(plmn);
#endif
			  
							MSG_HIGH_DS_2(MM_SUB, "=MM= mm_emm_usim_gprs_counter= %d,mm_emm_usim_non_gprs_counter =%d", mm_emm_usim_gprs_counter, mm_emm_usim_non_gprs_counter);
							break;
			  
						default:
						  break;
					}
  
		  }
	   }
	 }
	 return;
  }


void mm_process_gsm_block_timer_expiry(mm_cmd_type* message)
{
    if (message->cmd.timer_expiry.timer_id == GSM_EXTENDED_BLOCK_TIMER)
    {
      mm_stop_timer(GSM_EXTENDED_BLOCK_TIMER);
     
         mm_send_rrc_g_meas_stop_ind(FALSE);
      #ifdef FEATURE_LTE
         emm_send_lrrc_g_meas_stop_ind(FALSE);
      #endif
    }
    else
    {
       MSG_ERROR_DS_1(MM_SUB ,"=MM= Unexpected Timer expiry %d", message->cmd.timer_expiry.timer_id);
    }
    return;
}
void mmcoord_process_rr_plmn_resel_fail_ind(mm_cmd_type* message)
{

  if (message->cmd.rr_plmn_resel_fail_ind.message_header.message_id == (int) RR_PLMN_RESEL_FAIL_IND && mm_disable_g_upon_fallback)
  {
     rr_plmn_resel_fail_ind_T   lai_info;
     inter_task_lai_T           sent_lai;
     mm_as_id_e_type            current_as_id;
     // sys_plmn_id_s_type         sent_plmn;
     current_as_id = mm_as_id;
     mm_set_as_id((mm_as_id_e_type)message->cmd.rr_plmn_resel_fail_ind.as_id);
     memscpy(&lai_info, sizeof(lai_info),&message->cmd.rr_plmn_resel_fail_ind,sizeof(rr_plmn_resel_fail_ind_T));
     
     sent_lai.PLMN_id.identity[0] = lai_info.lai.PLMN_id.identity[0];
     sent_lai.PLMN_id.identity[1] = lai_info.lai.PLMN_id.identity[1];
     sent_lai.PLMN_id.identity[2] = lai_info.lai.PLMN_id.identity[2];
     sent_lai.location_area_code[0] = lai_info.lai.location_area_code[0];
     sent_lai.location_area_code[1] = lai_info.lai.location_area_code[1];

    
     MSG_HIGH_DS_3(MM_SUB ,"=MM= MM received RR_PLMN_RESEL_FAIL_IND on sub %d mm_disable_g_upon_fallback=%d DDS sub=%d",(mm_as_id_e_type)message->cmd.rr_plmn_resel_fail_ind.as_id,mm_disable_g_upon_fallback,mm_dds_sub);
     MSG_HIGH_DS_2(MM_SUB , "=MM= RR_PLMN_RESEL_FAIL_IND recived LAC %d %d",sent_lai.location_area_code[0],sent_lai.location_area_code[1]);
     /*Check with our flai and if conditions meet, that is the LAI sent by them is CU HPLMN on nDDS, start the timer*/  
   
     if ((mm_disable_g_upon_fallback == TRUE) &&
          (cm_get_operator_info((sys_modem_as_id_e_type)mm_sub_id) == OPERATOR_CU) &&
          (MM_IS_NON_DDS_SUB) &&
       //   (mm_is_plmn_an_hplmn(sent_lai.PLMN_id,lai_info.as_id)) && // we dont need this
          (mm_check_forbidden_national_roaming(&sent_lai)) &&
          (mm_timer_status[GSM_EXTENDED_BLOCK_TIMER] == TIMER_STOPPED))
       {
          mm_start_timer(GSM_EXTENDED_BLOCK_TIMER,DEFAULT_TIMEOUT);
   
       
            mm_send_rrc_g_meas_stop_ind(TRUE);
         #ifdef FEATURE_LTE
            emm_send_lrrc_g_meas_stop_ind(TRUE);
        #endif
       }
     mm_set_as_id(current_as_id);
  }
 
}
#ifdef FEATURE_LTE
/*===========================================================================
 FUNCTION MMCOORD_PROCESS_NON_ACQ_PARAM_CHANGE_IND
 
 DESCRIPTION
   This function processes the non acq params change indication
 
 DEPENDENCIES
   None
 
 RETURN VALUE
   None
 
 SIDE EFFECTS
   None
 ===========================================================================*/
void mmcoord_process_non_acq_param_change_ind
(
  mm_cmd_type *message
)
{
  if (message->cmd.non_acq_param_change_ind.message_header.message_id == (int) MM_NON_ACQ_PARAM_CHANGE_IND)
  {
 
      MSG_HIGH_DS_0(MM_SUB ,"=MM= MM received MM_NON_ACQ_PARAM_CHANGE_IND");
      gmm_handle_message(message);
    
  }
  else
  {
    MSG_HIGH_DS_1(MM_SUB ,"=MM= Unexpected MM msg %d", message->cmd.hdr.message_id);
  }
}
#endif
