/*!
  @file
  lte_rrc_ext_msg.h

  @brief
  This file describes the interfaces to LTE Radio resource Controller (RRC)

   <!-- Doxygen chokes on the @mainpage vs. @page inside a conditional, but the
       following seems to work.  Basically, make this the mainpage if it
       is the only component being built, otherwise, just make it a page and
       let the upper level component/s @ref it. -->
  @if rrc 
  @mainpage LTE Radio Resource Controller (RRC)
  @endif
  @ifnot rrc
  @page LTE Radio Resource Controller (RRC)
  @endif

  RRC module is the heart of the configuration and control for the LTE access 
  stratum. This layer intercepts all the over the air messages and configures 
  all the lower layer modules. This module is also in charge if 
  tracking system information change, connection set up , security and keeping 
  the connection up through mobility control.  This module also acts as the 
  access stratum's interface to the NAS layer. 

  RRC is a single task architecture and consists of a set of co-operating state 
  machines which communicate with each other, and with external modules, via 
  messages with unique message ids (UMIDs). All the messages posted by RRC to 
  modules outside of RRC is always through the message router. Internally 
  almost all of the major modules and procedures of RRC are modeled as state 
  machines. State machines inside of RRC can communicate either synchronously 
  ( i.e without invoking a message router or a CS call) or asynchronously using 
  the message router /CS call. Either way all the received external and internal
   UMIDs of RRC go through a common RRC dispatcher which has the knowledge of 
   the recipient state machines for the various UMIds. 

  <h2>High level architecture</h2>
  <img src='../pics/rrc_arch.jpg'>
  
  <h2>API</h2>
 <h3><i>Message Interfaces</i></h3> 
   -- External API to RRC is defined in @ref lte_rrc_ext_msg.h
  
 <h3><i>Function Call Interfaces</i></h3> 
  This module does not have any function call API.

  <b><i> QXDM Log Parser </i></b>
  - @ref QXDM_Parser_DLL

  @ref umids
  @ref all_umids

*/


/*===========================================================================

  Copyright (c) 2008 Qualcomm Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //commercial/MPSS.TA.3.0.c1/Main/modem_proc/stubs/api/lte_rrc_ext_msg.h#3 $

when       who       what, where, why
--------   ---       ---------------------------------------------------------- 
10/01/13   kp        Added LTE Load Balancing for VoLTE Call interface changes NAS-RRC. 
08/07/13   kp        Added DSDS interface changes for NAS-RRC. 
03/13/12   kp        Added code to perform ac-barring for AC-11 to 15 from param provided 
                     by NAS when SIB2 says no AC barring for AC-11 to 15.  
03/12/12   kp        Added code to provide ac-barring info from SIB2 to NAS in srv_ind  
11/14/11   kp        removed nas_msg_container from lte_rrc_conn_est_req_s structure 
                     as we have moved to use DSM ptrs.
10/03/11   sureshs   Added SSAC params to camped cell info
09/30/11   gpotnuru  Added new plmn search status LTE_RRC_PLMN_SEARCH_ABORTED_ON_RLF
08/25/11   zz        Added RRC-DS and RRC-QMI_NAS interfaces for eMBMS
05/27/11   vatsac    Added support using for ims emergency from Rel9 SIB1
05/25/11   np        Define interface for LTE key gen for W2L PSHO
05/13/11   vatsac    Added forbidden_manual_ta_list to lte_rrc_service_req_s and
                     lte_rrc_system_update_s
09/10/10   np        Switch to DSM items for NAS DT messages
02/01/11   mm        Added definitions for passing CMAS notification to upper layers
01/21/11   vatsac    Added service req type field
12/08/10   np        Add corresponding abort reason after aborting and releasing conn.
12/06/10   np        Add abort cause to lte_rrc_conn_abort_req_s for L2W PSHO
11/08/10   np        Define interface for UMTS key gen for L2W PSHO
09/10/10   np        e1xCSFB Interfaces for HO to 1xCP
08/25/10   np        Add Redirection Information to Connection Release for CSFB
08/05/10   amit      Added LTE_RRC_ETWS_MSG_ID_LIST_REQ and corresponding structures
06/28/10   amit      Added PLMN to etws_prim_ind and etws_sec_ind 
06/23/10   np        Consolidate IRAT Tunnel into unified message
06/21/10   np        Add payload to lte_rrc_irat_ul_msg_req_s
06/17/10   np        IRAT Tunnel Support for 1xCSFB Pre-Reg
05/10/10   da        Added Msgs for NMR
04/21/10   sureshs   Added use_oos field to Service Indication
02/22/10   amit      Added indications for ETWS primary and secondary notifications
02/08/10   sureshs   Added Network Select Mode Reset Request
02/02/10   vatsac    Seperating out IRAT msgs from lte_rrc_ext_msg.h
02/02/10   mm        Added a SIB_READ_ERROR cause for Connection Release
02/02/10   vatsac    Removed support for irat_info in service req
01/26/10   np        W IRAT Capabilities placeholder
01/26/10   np        Generic definition of PLMN for IRAT
01/24/10   sk        Added support for G to L Reselection
01/15/10   np        W -> L IRAT Resel/Redir UMIDs
01/14/10   sk        Added dummy data types for LTE  -> G reselection and 
                     LTE --> G redirection
01/13/10   sk        Removed placeholders for LTE --> DO reselection/redirection
01/07/10   np        Minor update to lte_rrc_lte_to_w_resel_req_s
01/07/10   np        Add stub payload for  L -> W Redir
12/22/09   np        Added stub types for WCDMA RRC Redir Interface
12/18/09   np        Added stub types for WCDMA RRC Resel Interface
12/15/09   np        Removed the temp LTE -> 1x IRAT placeholders
12/06/09   sk        Added dummy types for LTE to DO reselection
10/19/09   sureshs   Added RAT priority list to Service Request
09/21/09   np        Added dummy data types for LTE to 1X Redirection support.
09/18/09   sureshs   Added CDMA support fields to IRAT info
09/17/09   sureshs   Added no_svc_cause field to Service Indication
09/10/09   sureshs   Added quality field to found PLMN list element
09/02/09   sk        Added dummy data types for LTE to DO Redirection support.
08/26/09   sureshs   Added t311_rem_time field to service indication
08/20/09   vatsac    Removed redirection related info from NAS service ind 
08/05/09   sureshs   Changed dl_arfcn in redir info to uint32 for March spec
07/29/09   vatsac    Added cell_identity in lte_rrc_conn_mode_cell_change_ind_s
07/28/09   vatsac    Added cell_identity in lte_rrc_service_ind_s to NAS
07/01/09   sureshs   Added some comments
05/22/09   sureshs   Changed LTE_RRC_MAX_NUM_TRACKING_AREAS to 20
05/04/09   vatsac    Updated lte_rrc_conn_rel_reason_e, lte_rrc_service_ind_s
                     and lte_rrc_deactivate_req_s to support inter-RAT redir
04/20/09   dd        Modified nas_count field to uint32 in security_info
04/07/09   sureshs   Added search_done field to service indication
01/19/09   sureshs   Added support for multiple access classes
01/14/09   sureshs   Added found plmn type and list to include rssi
12/18/08   sureshs   Added new fields in Cell Change Ind
12/18/08   dd        Added ConnEstTimerUpdateInd. Added is_high_priority boolean
                     flag to ConnEstReq
12/10/08   vatsac    Added a new value to enum lte_rrc_ul_data_cnf_status_e
                     and increased MAX_NAS_MSG_CONTAINER_SIZE to 4K
12/09/08   vatsac    Increased size of trans_id for msgs to/from NAS
12/09/08   sureshs   Conn Abort and Svc indication have phy_cell_id fields,
                     trans_id in svc req and ind increased to 32 bits
12/08/08   vatsac    Increased MAX_NAS_MSG_CONTAINER_SIZE to 2K
12/08/08   sureshs   Added EPLMN list to Service Request, removed ehpln valid
                     flag
12/05/08   sureshs   Removed Conn Abort Cnf
11/26/08   sureshs   Removed cause field in Conn Abort Req
11/24/08   sureshs   Added PLMN to TA struct
11/18/08   sureshs   Made EHPLMN into a list
11/17/08   sureshs   Clarified size of EPLMN list
11/13/08   sureshs   Enhanced field in UL DATA CNF
11/05/08   ask       Ported RRC to POSIX APIs
11/03/08   sureshs   Renamed new_scan to scan_is_new
11/03/08   sureshs   Made list into single entry for selected PLMN in svc ind
10/29/08   sureshs   Added EHPLMN to service request, and new payload for
                     System Update message
10/16/08   sureshs   Incorporated changes to NAS interface after meeting
10/13/08   dd        Updated ConnEstCnf and expanded the conn establishment 
                     status codes
10/10/08   dd        Added access class to SIMUpdate
10/06/08   sureshs   Added selected band field
10/06/08   sureshs   Removed num_mcc_digits
10/04/08   sureshs   Included sys.h, added band preference bit mask
10/03/08   sureshs   Added num_mcc_digits field to lte_rrc_plmn_s and made the
                     max number of digits as defines
10/01/08   vr        Changed PLMN to PLMN List in Service Ind to NAS
09/11/08   sureshs   Added cell barring reqd field to Conn Abort Req
09/09/08   dd        Added est_failure to conn est status enum 
09/09/08   sureshs   Added load balancing required flag to conn released ind
08/27/08   vr        Added typedefs for cell id and tracking area code
07/03/08   sureshs   Initial version
===========================================================================*/

#ifndef LTE_RRC_EXT_MSG_H
#define LTE_RRC_EXT_MSG_H

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/



/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/

#define LTE_RRC_MAX_MCC_DIGITS 3 /*!< Maximum number of digits in MCC */
#define LTE_RRC_MAX_MNC_DIGITS 3 /*!< Maximum number of digits in MNC */

/* Type for Global Cell id */
typedef uint32 lte_rrc_global_cell_id_t;

/*! @brief Data structure for PLMN type
*/
typedef struct
{
  uint8 mcc[LTE_RRC_MAX_MCC_DIGITS]; /*!< Mobile country code */
  uint8 num_mnc_digits; /*!< Number of digits in MNC */
  uint8 mnc[LTE_RRC_MAX_MNC_DIGITS]; /*!< Mobile network code */
} lte_rrc_plmn_s;

/*! @brief Message to trigger fresh SIB8/16 decode
 */
typedef struct
{
  msgr_hdr_struct_type msg_hdr;
 
} lte_rrc_refresh_system_time_info_req_s;

/*! @brief Array of msg UMIDs that are destined for the external mb
*/
typedef enum 
{
  /* Requests */
  LTE_RRC_CONN_EST_REQ,
  LTE_RRC_UL_DATA_REQ,
  LTE_RRC_IRAT_TUNNEL_UL_MSG_REQ,
  LTE_RRC_IRAT_HDR_UL_TUNNEL_MSG_REQ,
  LTE_RRC_DEACTIVATE_REQ,
  LTE_RRC_BAND_PRI_CHANGE_REQ,
  LTE_RRC_GET_BAND_PRI_LIST_REQ,
  LTE_RRC_CONN_ABORT_REQ,
  LTE_RRC_SERVICE_REQ,
  LTE_RRC_CELL_BAR_REQ,
  LTE_RRC_FORBIDDEN_TA_LIST_RESET_REQ,
  LTE_RRC_SYSTEM_UPDATE_REQ,
  LTE_RRC_SIM_UPDATE_REQ,
  LTE_RRC_DRX_INFO_REQ,
  LTE_MAC_RELEASE_RESOURCES_IND,
  LTE_RRC_UE_MODE_IND,
  LTE_RRC_DEPRI_REQUESTED_PLMN_IND,
  LTE_RRC_G_RESEL_REQ,
  LTE_RRC_G_ABORT_RESEL_REQ,
  LTE_RRC_G_REDIR_REQ,
  LTE_RRC_G_ABORT_REDIR_REQ,
  LTE_RRC_eHRPD_RESEL_REQ,
  LTE_RRC_eHRPD_ABORT_RESEL_REQ,
  LTE_RRC_eHRPD_REDIR_REQ,
  LTE_RRC_eHRPD_ABORT_REDIR_REQ,
  LTE_RRC_DSDS_TRM_PRIORITY_REQ,
  LTE_RRC_DS_STATUS_CHANGE_REQ,

  LTE_RRC_1X_RESEL_REQ,
  LTE_RRC_1X_ABORT_RESEL_REQ,

  LTE_RRC_NW_SEL_MODE_RESET_REQ,
  LTE_RRC_EUTRA_CAPABILITIES_REQ,
  LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_REQ,
  LTE_RRC_WCDMA_PLMN_SRCH_REQ,
  LTE_RRC_WCDMA_ABORT_PLMN_SRCH_REQ,
  LTE_RRC_WCDMA_SUSPEND_PLMN_SRCH_IND,
  LTE_RRC_WCDMA_GET_PLMN_PRTL_RESULTS_REQ,
  LTE_RRC_TDSCDMA_PLMN_SRCH_REQ,
  LTE_RRC_TDSCDMA_ABORT_PLMN_SRCH_REQ,
  LTE_RRC_TDSCDMA_SUSPEND_PLMN_SRCH_IND,
  LTE_RRC_TDSCDMA_GET_PLMN_PRTL_RESULTS_REQ,
  LTE_RRC_G_PLMN_SRCH_REQ,
  LTE_RRC_G_ABORT_PLMN_SRCH_REQ,
  LTE_RRC_G_SUSPEND_PLMN_SRCH_IND,
  LTE_RRC_G_GET_PLMN_PRTL_RESULTS_REQ,
  LTE_RRC_WCDMA_RESEL_REQ,
  LTE_RRC_WCDMA_PSHO_REQ,
  LTE_RRC_WCDMA_REDIR_REQ,
  LTE_RRC_WCDMA_ABORT_RESEL_REQ,
  LTE_RRC_WCDMA_ABORT_PSHO_REQ,
  LTE_RRC_WCDMA_ABORT_REDIR_REQ,
  LTE_RRC_TDSCDMA_RESEL_REQ,
  LTE_RRC_TDSCDMA_PSHO_REQ,
  LTE_RRC_TDSCDMA_REDIR_REQ,
  LTE_RRC_TDSCDMA_ABORT_RESEL_REQ,
  LTE_RRC_TDSCDMA_ABORT_PSHO_REQ,
  LTE_RRC_TDSCDMA_ABORT_REDIR_REQ,
  LTE_RRC_PLMN_SEARCH_REQ,
  LTE_RRC_PLMN_SEARCH_ABORT_REQ,
  LTE_RRC_NMR_INFO_REQ,
  LTE_RRC_AVOIDANCE_REQ,
  LTE_RRC_ACQ_DB_EARFCN_REQ,
  LTE_RRC_PLMN_SWITCH_REQ, 
  
  /* TO DO: Remove after WMS removes reference to the old UMID */
  LTE_RRC_ETWS_MSG_ID_LIST_REQ,
  LTE_RRC_MSG_ID_LIST_REQ,
  LTE_RRC_GET_DEDICATED_PRI_REQ,
  LTE_RRC_IM3_BACKOFF_APPLIED_REQ,

  LTE_RRC_EMBMS_ENABLE_REQ, 
  LTE_RRC_EMBMS_DISABLE_REQ, 
  LTE_RRC_EMBMS_ACT_TMGI_REQ, 
  LTE_RRC_EMBMS_DEACT_TMGI_REQ, 
  LTE_RRC_EMBMS_ACT_DEACT_TMGI_REQ,
  LTE_RRC_EMBMS_AVAIL_TMGI_LIST_REQ, 
  LTE_RRC_EMBMS_AVAIL_SAI_LIST_REQ,
  LTE_RRC_EMBMS_SIGNAL_STRENGTH_REQ,
  LTE_RRC_CMAPI_EMBMS_COV_STATUS_REQ,
  LTE_RRC_CMAPI_EMBMS_DATAMCS_REQ,
  LTE_RRC_CELL_LOCK_REQ,
  LTE_RRC_GET_SERV_CELL_SIB_REQ,
  LTE_RRC_MTC_CFG_REQ,
  LTE_RRC_UE_CAP_CFG_REQ,
  LTE_RRC_EMBMS_SERVICE_INTEREST_INFO_REQ,
  LTE_RRC_IDLE_DRX_INFO_REQ,
  LTE_RRC_NW_REQ_EUTRA_CAPABILITIES_REQ,
  LTE_RRC_REFRESH_SYSTEM_TIME_INFO_REQ,
  
  /* Confirms */
  LTE_CPHY_START_CNF,
  LTE_CPHY_STOP_CNF,
  LTE_CPHY_COMMON_CFG_CNF,
  LTE_CPHY_DEDICATED_CFG_CNF,
  LTE_CPHY_HANDOVER_CNF,
  LTE_CPHY_ABORT_CNF,
  LTE_CPHY_CON_RELEASE_CNF,
  LTE_CPHY_ACQ_CNF,
  LTE_CPHY_SYSTEM_SCAN_CNF,
  LTE_CPHY_BAND_SCAN_CNF,
  LTE_CPHY_CELL_SELECT_CNF,
  LTE_CPHY_IDLE_MEAS_CFG_CNF,
  LTE_CPHY_CONN_MEAS_CFG_CNF,
  LTE_CPHY_SERV_CELL_MEAS_CNF,
  LTE_CPHY_TDD_CFG_CNF,
  LTE_CPHY_SUSPEND_CNF,
  LTE_CPHY_RESUME_CNF,
  LTE_CPHY_BPLMN_CELL_CNF,
  LTE_CPHY_BPLMN_STOP_CNF,
  LTE_CPHY_BPLMN_SUSPEND_CNF,
  LTE_CPHY_NMR_INFO_CNF,
  LTE_CPHY_MBSFN_SIGNAL_STRENGTH_CNF,
  LTE_CPHY_BEST_MBMS_NEIGH_FREQ_CNF,
  LTE_CPHY_UEINFO_RPT_CNF,
  LTE_CPHY_UEINFO_CONN_FAILURE_RPT_CNF,
  LTE_CPHY_RELEASE_TRM_CNF,

  LTE_MAC_CFG_CNF,
  LTE_MAC_ACCESS_CNF,
  LTE_MAC_ACCESS_ABORT_CNF,
  LTE_MAC_START_CNF,
  LTE_MAC_STOP_CNF,
  LTE_MAC_RACH_RPT_CNF,

  LTE_RLCDL_CFG_CNF,
  LTE_RLCUL_CFG_CNF,
  LTE_RLCUL_START_CNF,
  LTE_RLCUL_STOP_CNF,
  LTE_CPHY_HARQ_ACK_TX_STS_CNF,
  LTE_RLCDL_START_CNF,
  LTE_RLCDL_STOP_CNF,

  LTE_PDCPDL_CFG_CNF,
  LTE_PDCPUL_CFG_CNF,
  LTE_PDCPUL_SDU_CNF,
  LTE_PDCPDL_RECFG_PREP_CNF,
  LTE_PDCPUL_RECFG_PREP_CNF,
  LTE_PDCPUL_COUNTER_CNF,
  LTE_PDCPDL_COUNTER_CNF,
  LTE_PDCPUL_START_CNF,
  LTE_PDCPUL_STOP_CNF,
  LTE_PDCPDL_START_CNF,
  LTE_PDCPDL_STOP_CNF,

  /* Responses */
  LTE_RRC_ACTIVATION_RSP,         /* Activation Rsp from NAS */
  LTE_RRC_FROM_CM_ACTIVATION_RSP, /* Activation Rsp from CM */
  LTE_RRC_UTRA_SERVING_CELL_INFO_RSP, 
  LTE_RRC_NAS_UMTS_KEY_RSP,
  LTE_RRC_NAS_GSM_KEY_RSP,

  /* IRAT To G UMIDs */
  GERAN_GRR_LTE_ABORT_REDIR_RSP,
  GERAN_GRR_LTE_ABORT_RESEL_RSP,
  GERAN_GRR_LTE_REDIR_FAILED_RSP,
  GERAN_GRR_LTE_RESEL_FAILED_RSP,
  GERAN_GRR_LTE_CS_CAPABILITIES_RSP,
  GERAN_GRR_LTE_PS_CAPABILITIES_RSP,
  GERAN_GRR_LTE_PLMN_SRCH_RSP,
  GERAN_GRR_LTE_ABORT_PLMN_SRCH_RSP,
  GERAN_GRR_LTE_DEDICATED_PRIORITIES_RSP,
  GERAN_GRR_LTE_GET_CGI_RSP,
  GERAN_GRR_LTE_ABORT_CGI_RSP,
  GERAN_GRR_LTE_CCO_RSP,
  GERAN_GRR_LTE_ABORT_CCO_RSP,
  GERAN_GRR_LTE_HO_FAILED_RSP,
  GERAN_GRR_LTE_ABORT_HO_RSP,

  HDR_CP_LTE_REDIR_FAILED_RSP,
  HDR_CP_LTE_RESEL_FAILED_RSP,
  HDR_CP_LTE_ACTIVE_HO_FAILED_RSP,
  HDR_CP_LTE_REDIR_ABORT_RSP,
  HDR_CP_LTE_RESEL_ABORT_RSP,
  HDR_CP_LTE_ACTIVE_HO_ABORT_RSP,
  HDR_CP_LTE_1X_HRPD_CAPABILITIES_RSP,
  HDR_CP_LTE_REVERSE_TIMING_TRANSFER_RSP,
  HDR_CP_LTE_REVERSE_TIMING_TRANSFER_ABORT_RSP,
  HDR_CP_LTE_GET_CGI_RSP,

  LTE_RRC_NAS_LTE_KEY_RSP,

  /* Indications */
  LTE_CPHY_DL_WEAK_IND,
  LTE_CPHY_CELL_RESEL_IND,
  LTE_CPHY_IRAT_ASF_NEEDED_IND,
  LTE_CPHY_OOS_IND,
  LTE_CPHY_RL_FAILURE_IND,
  LTE_CPHY_BPLMN_COMPLETE_IND,
  LTE_CPHY_BPLMN_TIME_AVAIL_IND,
  LTE_CPHY_MEAS_REPORT_IND,
  LTE_CPHY_IRAT_CGI_START_IND,
  LTE_CPHY_IRAT_CGI_END_IND,
  LTE_CPHY_MCCH_CHANGE_NOTIFICATION_IND,
  LTE_RRC_INTERFREQ_LIST_UPDATE_INDI,
  LTE_MAC_RANDOM_ACCESS_PROBLEM_IND,

  LTE_RLCUL_MAX_RETX_IND,
  LTE_L2_RLF_IND,
  LTE_PDCPUL_RL_FAIL_IND,
  LTE_FC_SHUTDOWN_IND,
  LTE_PDCPUL_CONN_REL_IND,

  LTE_RRC_CLEAR_DEDICATED_PRI_IND,
  HDR_CP_HRPD_PREREG_STATUS_IND,
  LTE_CPHY_RF_AVAILABLE_IND,
  LTE_CPHY_RF_UNAVAILABLE_IND,
  LTE_CPHY_TCELL_RF_UNAVAILABLE_IND,
  LTE_CPHY_RACH_MSG1_SCHED_IND,

  LTE_CPHY_IFREQ_OTDOA_IND,
  MM_CM_IMS_EMERG_CAP_SUPP_IND,
  MM_CM_EMERGENCY_MODE_STATUS_IND,
  MM_CM_VOICE_CALL_STATUS_IND,
  WMS_SMS_STATUS_IND,
  LTE_CPHY_SCELL_MEAS_REPORT_IND,
  LTE_RRC_WIFI_MEAS_RPT_IND,
  LTE_CPHY_TTL_COR_IND,
  LTE_CPHY_UPDATE_MOBILITY_STATE_IND,

  /* Supervisory */
  LTE_RRC_SPR_LOOPBACK,
  LTE_RRC_SPR_THREAD_KILL,

  GERAN_GRR_LTE_ABORT_CCO_FAILURE_PROCEDURE_IND,
  LTE_RRC_CSFB_CALL_STATUS_IND,
  LTE_RRC_UE_CAP_CHANGE_IND,
  LTE_CPHY_UTC_TIME_UPDATE_CNF,
  LTE_CPHY_UE_INFO_MDT_REPORT_IND,
  
  LTE_TLB_RRC_CFG_REQ,
  LTE_TLB_LB_OPEN_IND,
  LTE_TLB_RRC_LOCATION_UPDATE_IND,
  
  /* POLICYMAN_CFG_UPDATE_MSIM_IND */
  POLICYMAN_CFG_UPDATE_MSIM_IND,
  
  LTE_RRC_MODE_PREF_UPDATE_IND,

#ifdef FEATURE_LTE_TO_1X
  /* Protecting 1x CP UMIDs under feature for 1x compile out */
  ONEX_CP_LTE_GET_CGI_RSP,
  ONEX_CP_LTE_ABORT_REDIR_RSP,
  ONEX_CP_LTE_ABORT_RESEL_RSP,
  ONEX_CP_LTE_RESEL_FAILED_RSP,
  ONEX_CP_LTE_REDIR_FAILED_RSP,
  ONEX_CP_LTE_1xCSFB_CALL_REL_IND,
  ONEX_CP_LTE_1xCSFB_HO_FAILED_RSP,
  ONEX_CP_LTE_1xSRVCC_HO_FAILED_RSP,
  ONEX_CP_LTE_ABORT_1xCSFB_HO_RSP,
#endif

#ifdef FEATURE_LTE_TO_WCDMA
  WCDMA_RRC_LTE_GET_DEDICATED_PRI_RSP,
  WCDMA_RRC_LTE_UTRA_CAPABILITIES_RSP,
  WCDMA_RRC_LTE_ABORT_REDIR_RSP,
  WCDMA_RRC_LTE_ABORT_RESEL_RSP,
  WCDMA_RRC_LTE_RESEL_FAILED_RSP,
  WCDMA_RRC_LTE_RESEL_RSP,
  WCDMA_RRC_LTE_REDIR_FAILED_RSP,
  WCDMA_RRC_LTE_PLMN_SRCH_RSP,
  WCDMA_RRC_LTE_ABORT_PLMN_SRCH_RSP,
  WCDMA_RRC_LTE_PSHO_RSP,
  WCDMA_RRC_LTE_ABORT_PSHO_RSP,
  WCDMA_RRC_LTE_GET_CGI_RSP,
  WCDMA_RRC_LTE_ABORT_CGI_RSP,
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
  TDSCDMA_RRC_LTE_GET_DEDICATED_PRI_RSP,
  TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_RSP,
  TDSCDMA_RRC_LTE_ABORT_REDIR_RSP,
  TDSCDMA_RRC_LTE_ABORT_RESEL_RSP,
  TDSCDMA_RRC_LTE_RESEL_FAILED_RSP,
  TDSCDMA_RRC_LTE_IRAT_INFO_IND,
  TDSCDMA_RRC_LTE_REDIR_FAILED_RSP,
  TDSCDMA_RRC_LTE_PLMN_SRCH_RSP,
  TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_RSP,
  TDSCDMA_RRC_LTE_PSHO_RSP,
  TDSCDMA_RRC_LTE_ABORT_PSHO_RSP,
  TDSCDMA_RRC_LTE_GET_CGI_RSP,
  TDSCDMA_RRC_LTE_ABORT_CGI_RSP,
#endif  

} lte_rrc_umid_type;

#endif /* LTE_RRC_EXT_MSG_H */

