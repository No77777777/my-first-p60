/*===========================================================================

                                  R R C D A T A

GENERAL DESCRIPTION
 The RRCDATA module consists of RRC CONNECTION ESTABLISHMENT PROCEDURE 
 implementation. The details are from section 8.1.3 of 3GPP TS 25.331
 V3.4.1 (2000-9) specification



Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================
                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcdata.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/14/12   as      Added code to support feature B/WB handover support
02/10/12   mkh     Reverting prev changes (CL: 2199119)
02/10/12   mkh     Changes to Return the current DL and UL data rate based on 
                   RRC internal stored category. On behalf of JZ.
12/10/11   ttl     Added TDSL1_TPC_STEP_SIZE_0DB and removed TDSL1_NUM_TPC_STEP_SIZE from
                   tdsTpcStepSize[].
10/13/11   mkh     Added log version to RRC log packets                              
07/07/11   mkh     Renamed log pkts to TDSxxx
06/16/11   bj      Added code to handle REL9 version in function tdsrrc_get_tds_rrc_version_nv.
03/10/11   bj      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/17/11  zwj      Added E-DCH info for HSUPA.
11/24/10   yzh     Add ((FEATURE_TDSCDMA_64QAM) || (FEATURE_TDSCDMA_MIMO)) in tdsrrc_macehs_cat
                   which set value in FEATURE_TDSCDMA_64QAM or FEATURE_TDSCDMA_MIMO.
11/18/10   ysh     Save Prach info Transport Format Set. 3GPP-8.6.5.1(3.2.32)
                               Add some rom table declare.
11/04/10   yzh     (3GPP 10.3.6.21) modify Downlink DPCH info for each RL from FDD to TDD.
11/01/10   yzh     (3GPP 8.6.6.6) modify Uplink DPCH info from FDD to TDD
10/30/10   ysh     IE"Downlink information for each radio link" info save.  3GPP-8.6.6.4(3.2.13).
10/26/10   zwj     Added code to handle REL4 OTA Message.

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "tdscdma_variation.h"
#include "rex.h"
#include "err.h"
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcdata_v.h"
#include "tdsrrcdata.h"
#include "tdsrrclcm.h"
#include "tdsuecomdef.h"
#include "tdsrrctmr.h"
#include "cmwll.h"
#include "tdsrrclbt.h"
#include "tdsl1rrcif.h"
#include "tdsrrcscmgr.h"
#include "tdsrrclogging.h"
#include "bit.h"
#include "mmtask.h"
#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
#include "rlcrrcif.h"
#endif
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif

#include "tdsrrciho.h"
#include "tdsrrcsibdb.h"
#include "mm_umts.h"
#include "tdsrrcmcm.h"
#include "tdsrrcrce.h"

#include "tdsrrcnv.h"

#include "tdsrrcccm.h"
#include "tdsrrccspi.h"

#include "rrcmnif.h"
#include "tdsrrcrbcommon.h"

#include "tdsrlcdsapif.h"
#include "tdsrrc_dependancy.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#include "msgr_rex.h"
#include "tdsrrcsigs.h"
#include "tdsrrccmdi.h"

#include "tdsrrcdormancy.h"
#include "tdsrrcasn1util.h"


#ifdef FEATURE_TDSCDMA_EUTRA_DETECT
#include "tdsrrcmeas.h"
#endif
#include "tdsCFAParser_i.h"
#include "tdsrrcrce.h"
#include "sm.h"

#ifdef FEATURE_QSH_EVENT_METRIC
#include "tdsrrcqsh.h"
#endif

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
#include "tdsrrcsmc.h"
#endif
#include "tdsrrcwrm.h"
//#include "tdssrch.h"

/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

===========================================================================*/

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSUNAVAILABLE_BER 0xff

/* For serving cell signal status indication info */
/*!< Serving cell signal status resolution 
     10 means resolution is 0.1 */
#define TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION 10
/*!< Minimum serving cell RSSI in dBm */
#define TDSRRC_SCELL_SIGNAL_STATUS_MIN_RSSI -120
/*!< Minimum serving cell RSCP in dBm */
#define TDSRRC_SCELL_SIGNAL_STATUS_MIN_RSCP -120

/*===========================================================================

      Variables belonging to RRC layers        

===========================================================================*/
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
extern boolean tdsrrcmisc_rcvd_sim_ins_req;
extern boolean tdsrrcmisc_rcvd_sim_ins_req_internal[TDSCDMA_NUM_SUBS];
#endif

//#if defined FEATURE_TDSCDMA_TRM
  boolean tdsrrc_multimode_pt_of_no_return_reached = FALSE;
//#endif
tdsrrc_ProtocolErrorIndicator tdsrrc_protocol_error_indicator;

tdsrrc_ProtocolErrorInformation  tdsrrc_protocol_error_information;

unsigned short tdsrrc_v_300;

unsigned short tdsrrc_v_308;

unsigned short tdsrrc_v_304;

unsigned short tdsrrc_v_302;

tdsrrc_msg_ver_enum_type tdsota_msg_version = TDSMSG_INVALID;
rrc_establish_cause_e_type tdsrrc_establishment_cause;

rrc_initial_ue_id_choice_e_type tdsrrc_initial_ue_identity_choice;

rrc_initial_ue_id_type tdsrrc_initial_ue_identity;

boolean tdsrrc_protocol_error_reject;      /* Indicates whether a severe 
                                           protocol error */
tdsrrc_capability_update_requirement_type tdsrrc_capability_update_requirement;

rrc_rel_cause_e_type tdsrrc_release_cause;

/* Indicates if valid TMSI is present */
boolean                     tdsrrc_tmsi_valid;
/* Indicates if valid PTMSI is present */
boolean                     tdsrrc_ptmsi_valid;
/* Indicates if valid IMSI is present */
boolean                     tdsrrc_imsi_valid;
 
rrc_tmsi_gsm_map_type       tdsrrc_tmsi;

rrc_ptmsi_gsm_map_type      tdsrrc_ptmsi;

rrc_imsi_gsm_map_type       tdsrrc_imsi;

/* stores the access classes for the UE */
rrc_lo_access_class_e_type  tdsrrc_lo_access_class;
rrc_hi_access_class_type    tdsrrc_hi_access_class;

/* stores the admin data received in SIM INSERTED REQ */
byte                        tdsrrc_rcvd_admin_data;

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
/* Indicates if valid TMSI is present */
boolean                     tdsrrc_tmsi_valid_internal[TDSCDMA_NUM_SUBS];
/* Indicates if valid PTMSI is present */
boolean                     tdsrrc_ptmsi_valid_internal[TDSCDMA_NUM_SUBS];
/* Indicates if valid IMSI is present */
boolean                     tdsrrc_imsi_valid_internal[TDSCDMA_NUM_SUBS];
 
rrc_tmsi_gsm_map_type       tdsrrc_tmsi_internal[TDSCDMA_NUM_SUBS];

rrc_ptmsi_gsm_map_type      tdsrrc_ptmsi_internal[TDSCDMA_NUM_SUBS];

rrc_imsi_gsm_map_type       tdsrrc_imsi_internal[TDSCDMA_NUM_SUBS];

/* stores the access classes for the UE */
rrc_lo_access_class_e_type  tdsrrc_lo_access_class_internal[TDSCDMA_NUM_SUBS];
rrc_hi_access_class_type    tdsrrc_hi_access_class_internal[TDSCDMA_NUM_SUBS];

/* stores the admin data received in SIM INSERTED REQ */
byte                        tdsrrc_rcvd_admin_data_internal[TDSCDMA_NUM_SUBS];
#endif

/* ESTABLISHED_RABS */
tdsrrc_established_rabs_type tdsrrc_est_rabs;

#ifdef FEATURE_UMTS_PDCP 
/* PDCP_SN_INFO 25.331 section 13.4.15 */
tdsrrc_pdcp_sn_info_type tdsrrc_pdcp_sn_info;
#endif /* FEATURE_UMTS_PDCP */

/*stores the current Conn Timers and constants.  updated in tdsrrcdata.c file*/
tdsrrc_UE_ConnTimersAndConstants tdsrrc_ue_conn_timers_and_constants;

//stores in translated format.  needed to reduce function overhead 
//by calling procedures every time
tdsrrc_conn_timers_and_constants tdsconn_timers_and_constants;

/* Stores timestamp (in CFN ) when a Downlink SDU is received */
uint16  tdsrrc_cfn_before_decode[TDSMAX_NUM_OF_PROCEDURES];

tdshsdpa_action_enum_type tdshsdpa_action = TDSHSDPA_NOOP;
tdsrrc_hsdpa_status_enum_type tdshsdpa_status = TDSHSDPA_INACTIVE;
sys_hs_ind_e_type tdshsdpa_hsupa_ind = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
#ifdef FEATURE_HSPA_CALL_STATUS_IND
sys_hs_ind_e_type tdsprev_hspa_call_status = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
sys_hs_ind_e_type tdscurr_hspa_call_status = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
#endif /* FEATURE_HSPA_CALL_STATUS_IND */

tdsrlc_size_per_rb_struct_type tdsrlc_size_per_rb;

/* Initialize to not indicated by default */
boolean tdsps_rb_re_establish_indicator = FALSE;

/* RLC SIZE Change Structure */
tdsrlc_size_change_struct_type tdsrlc_size_change_in_progress;


#if defined(FEATURE_3GPP_FAST_DORMANCY)
extern tdsrrcdormancy_conn_timers_and_constants_type tdsrrcdormancy_conn_timers_and_constants;
#endif

boolean tds_is_ue_in_dual_standby = FALSE;
boolean tdsrrc_is_sim_id_valid = FALSE;
sys_modem_as_id_e_type tdsrrc_sim_id_recvd = SYS_MODEM_AS_ID_1;

tdsrrc_ds_tuneaway_status_change_ind_type tds_ds_tuneaway_status_change_ind = { FALSE };

boolean tdsrrc_skip_dynamic_power_class;

tdsrrcllc_msg_failure_status_e_type tdsrrc_msg_failure_cause = TDSRRC_MSG_VALID;

/*tds rrc debug global var*/
tdsrrc_debug_global_var_type tdsrrc_debug_global;

TDSRRC_MMAL_LOG_CB_TYPE *tdsrrc_mmal_log_cb_func;

#ifdef FEATURE_3GPP_CSFB_TDS
extern boolean tdsrrc_csfb_call_status;
#endif

#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
  extern boolean tdsrrcsmc_is_fresh_in_ota;
#endif

tdsrrcdata_codec_info_type_s tdsrrcdata_codec_info;

#if defined(FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)

/* Struct containing all the default configurations*/
tdsrrc_default_config_info_type tdsdefault_config_info[TDSNUM_DEFAULT_CONFIGS];

  /*SIB16 format of saved Default Configuration 0 as specified in
    13.7 of 25.331
   */
  uint8 tdsdefault_config_0[] = 
  { 
  /* FDD
    0x14, 0x0D, 0x23, 0x40, 0x02, 0x10, 0x01, 0x0B,
    0x3D, 0x28, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40,
    0x0A, 0x50, 0x03, 0x13, 0x3D, 0x2A, 0x80, 0x44,
    0x56, 0x92, 0x43, 0x40, 0x12, 0x90, 0x04, 0x04,
    0x71, 0x06, 0x00, 0x60, 0x02, 0x90, 0x04, 0x00,
    0x00, 0x40, 0x00, 0x31, 0x29, 0x5F, 0x80, 0x00,
    0x80, 0x44, 0x11, 0x1C, 0xFB, 0x30, 0x40, 0x40,
    0xCA, 0x1F, 0xB2, 0x00
    */

  /* TDD 
  0x14, 0x0D, 0x23, 0x40, 0x02, 0x10, 0x01, 0x0B,
  0x3D, 0x28, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40,
  0x0A, 0x50, 0x03, 0x13, 0x3D, 0x2A, 0x80, 0x44,
  0x56, 0x92, 0x43, 0x40, 0x12, 0x90, 0x04, 0x04,
  0x71, 0x06, 0x00, 0x60, 0x00, 0xE2, 0x00, 0x00,
  0x31, 0x29, 0xF9, 0x00, 0x01, 0x00, 0x88, 0x22,
  0x39, 0xF6, 0xC4, 0x30, 0x04, 0x04, 0x0C, 0xB8,
  0xF8, 0xF1, 0xE3, 0xC0
*/
  
  /* RB2:timeRST=100, should 300*/
  0x14, 0x0D, 0x23, 0x40, 0x02, 0x10, 0x01, 0x0B,
  0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40,
  0x0A, 0x50, 0x03, 0x13, 0x3D, 0x2A, 0x80, 0x44,
  0x56, 0x92, 0x43, 0x40, 0x12, 0x90, 0x04, 0x04,
  0x71, 0x06, 0x00, 0x60, 0x00, 0xE2, 0x00, 0x00,
  0x31, 0x29, 0xF9, 0x00, 0x01, 0x00, 0x88, 0x22,
  0x39, 0xF6, 0xC4, 0x30, 0x04, 0x04, 0x0C, 0xB8,
  0xF8, 0xF1, 0xE3, 0xC0
  };

  /*hardcode the number of bytes in default config 0*/
  uint8 tdsdefault_config_0_len = 60;

  uint8 tdsdefault_config_1[] = 
  {
  /* FDD
    0x14, 0x0D, 0x23, 0x40, 0x02, 0x10, 0x01, 0x0B,
    0x3D, 0x28, 0x80, 0x40, 0x56, 0x92, 0x03, 0x40,
    0x0A, 0x50, 0x03, 0x13, 0x3D, 0x2A, 0x80, 0x40,
    0x56, 0x92, 0x03, 0x40, 0x12, 0x90, 0x04, 0x04,
    0x71, 0x06, 0x00, 0x60, 0x02, 0x90, 0x04, 0x00,
    0x00, 0x40, 0x00, 0x31, 0x29, 0x5F, 0x80, 0x00,
    0x00, 0x44, 0x11, 0x1C, 0xFB, 0x30, 0x40, 0x40,
    0xCA, 0x1F, 0xA8
    */

  /* TDD 
  0x14, 0x0D, 0x23, 0x40, 0x02, 0x10, 0x01, 0x0B,
  0x3D, 0x28, 0x80, 0x40, 0x56, 0x92, 0x03, 0x40,
  0x0A, 0x50, 0x03, 0x13, 0x3D, 0x2A, 0x80, 0x40,
  0x56, 0x92, 0x03, 0x40, 0x12, 0x90, 0x04, 0x04,
  0x71, 0x06, 0x00, 0x60, 0x00, 0xE2, 0x00, 0x00,
  0x31, 0x29, 0xF9, 0x00, 0x00, 0x00, 0x88, 0x22,
  0x39, 0xF6, 0xC4, 0x30, 0x04, 0x04, 0x0C, 0xB8,
  0xF8, 0x61, 0xE1, 0x80
  */

  /* RB2:timeRST=100, should 300*/
  0x14, 0x0D, 0x23, 0x40, 0x02, 0x10, 0x01, 0x0B,
  0x3D, 0x2A, 0x80, 0x40, 0x56, 0x92, 0x03, 0x40,
  0x0A, 0x50, 0x03, 0x13, 0x3D, 0x2A, 0x80, 0x40,
  0x56, 0x92, 0x03, 0x40, 0x12, 0x90, 0x04, 0x04,
  0x71, 0x06, 0x00, 0x60, 0x00, 0xE2, 0x00, 0x00,
  0x31, 0x29, 0xF9, 0x00, 0x00, 0x00, 0x88, 0x22,
  0x39, 0xF6, 0xC4, 0x30, 0x04, 0x04, 0x0C, 0xB8,
  0xF8, 0x61, 0xE1, 0x80
  };

  /*hardcode the number of bytes in default config 1*/
  /* FDD
  uint8 tdsdefault_config_1_len = 59;
  */
  /* TDD */
  uint8 tdsdefault_config_1_len = 60;

  uint8 tdsdefault_config_3[] =
  {
  /* FDD
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
    */

  /* TDD */
  0x14, 0x0D, 0x23, 0x41, 0x82, 0x10, 0x61, 0x0B,
  0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x41,
  0x8A, 0x50, 0x63, 0x13, 0x3D, 0x2A, 0x80, 0x44,
  0x56, 0x92, 0x43, 0x41, 0x92, 0x90, 0x64, 0x84,
  0x71, 0x06, 0x00, 0x60, 0x00, 0x57, 0x10, 0x60,
  0x16, 0x00, 0x46, 0x71, 0x06, 0x02, 0x60, 0x08,
  0xE2, 0x01, 0x00, 0xB0, 0x12, 0x0A, 0x4B, 0x49,
  0x89, 0x35, 0x2B, 0x9F, 0x90, 0x60, 0x08, 0x85, 
  0x10, 0x00, 0x4E, 0x04, 0x14, 0x40, 0x8F, 0x1D, 
  0x02, 0x20, 0x19, 0xC2, 0x23, 0xBD, 0x01, 0x08,
  0x03, 0xC0, 0x88, 0xBA, 0x80, 0x64, 0x02, 0x20,
  0x88, 0xE7, 0xDB, 0x10, 0xC0, 0x70, 0x01, 0x10,
  0x00, 0x04, 0x09, 0xC0, 0x82, 0x88, 0x11, 0xE3,
  0xAA, 0x80, 0x60, 0x81, 0x42, 0x03, 0x87, 0xC7,
  0xD2, 0x8F, 0x4A, 0x00
  };

  /*hardcode the number of bytes in default config 3*/
  /* FDD
  uint8 tdsdefault_config_3_len = 120;
  */

/* TDD */
  uint8 tdsdefault_config_3_len = 116;

  uint8 tdsdefault_config_4[] = 
  { 
  /* FDD
    0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 
    0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 
    0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44, 
    0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
    0x71, 0x06, 0x00, 0x60, 0x00, 0x81, 0x01, 0x60, 
    0x91, 0x49, 0x24, 0xD2, 0x89, 0x52, 0x8F, 0x02, 
    0x01, 0x02, 0xD2, 0x88, 0x60, 0x2B, 0x36, 0x09, 
    0x00, 0x88, 0x22, 0x39, 0xF6, 0x61, 0x80, 0x81, 
    0x20, 0x0C, 0x19, 0x76, 0x98
    */

  /* TDD */
  0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B,
  0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40,
  0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44,
  0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
  0x71, 0x06, 0x00, 0x60, 0x00, 0xE2, 0x00, 0x80, 
  0xB0, 0x48, 0xA4, 0x92, 0x69, 0x44, 0xA9, 0xF9, 
  0x02, 0x01, 0x02, 0xD2, 0x88, 0x60, 0x2B, 0x36,
  0x09, 0x00, 0x88, 0x22, 0x39, 0xF6, 0xC4, 0x30, 
  0x0C, 0x04, 0x09, 0x00, 0x60, 0xF1, 0xF4, 0xC3, 
  0xD3, 0x00
 };
  
  /*hardcode the number of bytes in default config 7*/
  /* FDD
  uint8 tdsdefault_config_4_len = 69;
  */
  
  /* TDD */
  uint8 tdsdefault_config_4_len = 74;

  /* Default config 6 ,  64kbps conv. CS- data + 3.4 kbps signalling */
  uint8 tdsdefault_config_6[] = 
  { 
  /* FDD
    0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 0x3D, 0x2A,
    0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 0x8A, 0x50, 0x23, 0x13,
    0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 
    0x24, 0x04, 0x71, 0x06, 0x00, 0x20, 0x00, 0x80, 0x00, 0xE2, 
    0x54, 0xC9, 0xCA, 0x3C, 0x08, 0x02, 0x0B, 0x5A, 0x12, 0x02, 
    0xA9, 0x60, 0x90, 0x08, 0x82, 0x23, 0x9F, 0x66, 0x18, 0x08, 
    0x12, 0x00, 0xC1, 0x97, 0x67, 0x80
    */

  /* TDD */
  0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 0x3D, 0x2A, 
  0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 0x8A, 0x50, 0x23, 0x13,
  0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 
  0x24, 0x04, 0x71, 0x06, 0x00, 0x20, 0x00, 0xE2, 0x00, 0x00, 
  0x71, 0x2A, 0x64, 0xE7, 0xE4, 0x08, 0x02, 0x0B, 0x5A, 0x12, 
  0x02, 0xA9, 0x60, 0x90, 0x08, 0x82, 0x23, 0x9F, 0x6C, 0x43, 
  0x00, 0xC0, 0x40, 0x90, 0x06, 0x0F, 0x1F, 0x2C, 0x3C, 0xB0
  };
  
  /*hardcode the number of bytes in default config 6*/
  /* FDD 
  uint8 tdsdefault_config_6_len = 66; */

  /* TDD */
  uint8 tdsdefault_config_6_len = 70;

  uint8 tdsdefault_config_7[] = 
  { 
  /* FDD
    0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 
    0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 
    0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44, 
    0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
    0x71, 0x06, 0x00, 0x20, 0x00, 0x81, 0x00, 0xE0, 
    0x91, 0x49, 0x24, 0xCA, 0xFC, 0x08, 0x04, 0x0B, 
    0x4A, 0x11, 0x2A, 0x46, 0x09, 0x00, 0x88, 0x22, 
    0x39, 0xF6, 0x61, 0x80, 0x81, 0x58, 0x0C, 0x19, 
    0x7E, 0xBC
    */

  /* TDD */
  0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B,
  0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40,
  0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44,
  0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04,
  0x71, 0x06, 0x00, 0x20, 0x00, 0xE2, 0x00, 0x80,
  0x70, 0x48, 0xA4, 0x92, 0x67, 0xE4, 0x08, 0x04,
  0x0B, 0x4A, 0x11, 0x2A, 0x46, 0x09, 0x00, 0x88,
  0x22, 0x39, 0xF6, 0xC4, 0x30, 0x0C, 0x04, 0x0A, 
  0xC0, 0x60, 0xF1, 0xF3, 0xE3, 0xCE, 0x00
 };
  
  /*hardcode the number of bytes in default config 7*/
  /* FDD
  uint8 tdsdefault_config_7_len = 66; */

  /* TDD */
  uint8 tdsdefault_config_7_len = 71;

  uint8 tdsdefault_config_8[] = 
  { 
  /* FDD
   0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 
   0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40,
   0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44, 
   0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
   0x71, 0x06, 0x00, 0x60, 0x00, 0x81, 0x01, 0x60, 
   0x91, 0x49, 0x24, 0xD2, 0x89, 0x52, 0x8F, 0x02, 
   0x01, 0x02, 0xD2, 0x88, 0x60, 0x29, 0xA6, 0x09, 
   0x00, 0x88, 0x22, 0x39, 0xF6, 0x61, 0x80, 0x81, 
   0x58, 0x0C, 0x19, 0x7E, 0x98
   */
  /* TDD */
  0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 
  0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 
  0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44, 
  0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
  0x71, 0x06, 0x00, 0x60, 0x00, 0xE2, 0x00, 0x80, 
  0xB0, 0x48, 0xA4, 0x92, 0x69, 0x44, 0xA9, 0xF9, 
  0x02, 0x01, 0x02, 0xD2, 0x88, 0x60, 0x29, 0xA6,
  0x09, 0x00, 0x88, 0x22, 0x39, 0xF6, 0xC4, 0x30,
  0x0C, 0x04, 0x0A, 0xC0, 0x60, 0xF1, 0xF4, 0xC3, 
  0xD3, 0x00
  };
 
  
  /*hardcode the number of bytes in default config 8*/
  /* FDD
  uint8 tdsdefault_config_8_len = 69; */
  /* TDD */
  uint8 tdsdefault_config_8_len = 74;


  uint8 tdsdefault_config_9[] = 
  { 
  /* FDD
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
    */
  /* TDD */
  0x14, 0x0D, 0x23, 0x40, 0x82, 0x10, 0x21, 0x0B, 
  0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x40, 
  0x8A, 0x50, 0x23, 0x13, 0x3D, 0x2A, 0x80, 0x44,
  0x56, 0x92, 0x43, 0x40, 0x92, 0x90, 0x24, 0x04, 
  0x71, 0x06, 0x00, 0x60, 0x00, 0xE2, 0x00, 0x81,
  0x30, 0x48, 0xA4, 0x92, 0x69, 0x44, 0xAA, 0x59,
  0x2E, 0x98, 0x4C, 0x9F, 0x90, 0x20, 0x10, 0x2D, 
  0x29, 0x06, 0x08, 0x62, 0x29, 0x06, 0x09, 0x00, 
  0x88, 0x22, 0x39, 0xF6, 0xC4, 0x30, 0x0C, 0x04, 
  0x0A, 0xC0, 0x60, 0xF1, 0xF5, 0x03, 0xD4, 0x00
  };
  
  /*hardcode the number of bytes in default config 9*/
  /* FDD
  uint8 tdsdefault_config_9_len = 75;
  */
  /* TDD */
  uint8 tdsdefault_config_9_len = 80;
  
  uint8 tdsdefault_config_10[] = 
  { 
  /* FDD
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
    */
  /* TDD */
  0x14, 0x0D, 0x23, 0x41, 0x82, 0x10, 0x61, 0x0B, 0x3D, 0x2A, 
  0x80, 0x44, 0x56, 0x92, 0x43, 0x41, 0x8A, 0x50, 0x63, 0x13, 
  0x3D, 0x2A, 0x80, 0x44, 0x56, 0x92, 0x43, 0x41, 0x92, 0x90, 
  0x64, 0x84, 0x71, 0x06, 0x00, 0x20, 0x00, 0x57, 0x10, 0x60, 
  0x12, 0x00, 0x46, 0x71, 0x06, 0x02, 0x20, 0x08, 0xE2, 0x01,
  0x81, 0x70, 0x04, 0x80, 0xA4, 0x21, 0x21, 0xE9, 0x16, 0x49, 
  0xDA, 0x4F, 0x12, 0x7A, 0x94, 0x44, 0xA5, 0xA5, 0x49, 0x2E, 
  0xE7, 0xE4, 0x18, 0x02, 0x51, 0x44, 0x00, 0x13, 0x81, 0x02,
  0xA0, 0x20, 0x6E, 0x04, 0x12, 0xC0, 0x82, 0x88, 0x11, 0xE3, 
  0xA0, 0x44, 0x83, 0x38, 0x00, 0x35, 0x02, 0x07, 0xE0, 0x41,
  0x50, 0x08, 0x33, 0x81, 0x1D, 0xE8, 0x08, 0x42, 0x1E, 0x00,
  0x03, 0xC0, 0x22, 0xEA, 0x01, 0x90, 0x48, 0x80, 0x04, 0x40, 
  0x47, 0x3E, 0xD8, 0x86, 0x03, 0x80, 0x09, 0x40, 0x00, 0x20, 
  0x4E, 0x04, 0x0A, 0x80, 0x81, 0xB8, 0x10, 0x4B, 0x02, 0x0A,
  0x20, 0x47, 0x8E, 0xAA, 0x01, 0x82, 0x05, 0x08, 0x0E, 0x1F, 
  0x1C, 0x07, 0xAD, 0x00
  };

  /*hardcode the number of bytes in default config 10*/
  /* FDD
  uint8 tdsdefault_config_10_len = 150;
  */
  /* TDD */
  uint8 tdsdefault_config_10_len = 154;
  

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT

/*
  Updated the data for Default Configs 11 and 12 as per Spec CR 2823
*/
uint8 tdsdefault_config_11[] = 
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
/* FDD
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
    */
  /* TDD 
  0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0x91, 
  0xC0, 0xD2, 0x0D, 0x06, 0x08, 0x40, 0xC2, 0x16, 0x7A, 0x55,
  0x00, 0x88, 0x03, 0x5A, 0x4D, 0x06, 0x83, 0x14, 0xA0, 0x63, 
  0x14, 0x23, 0x41, 0x92, 0x90, 0x32, 0x9C, 0xC0, 0x40, 0x40, 
  0x00, 0x44, 0x23, 0x88, 0x1A, 0x00, 0x00, 0x80, 0x00, 0xAE, 
  0x20, 0x68, 0x10, 0x02, 0x02, 0x03, 0x38, 0x81, 0xA0, 0x80, 
  0x08, 0x10, 0x1E, 0x20, 0x18, 0x17, 0x00, 0x48, 0x0A, 0x42, 
  0x12, 0x1E, 0x91, 0x64, 0x9D, 0xA4, 0xF1, 0x27, 0xA9, 0x44, 
  0x4A, 0x5A, 0x54, 0x92, 0xEE, 0x7E, 0x40, 0x0F, 0x81, 0x02, 
  0xC0, 0x00, 0x04, 0x02, 0x00, 0xC0, 0x40, 0x18, 0x07, 0x01, 
  0x40, 0x80, 0x24, 0x0A, 0x02, 0xF8, 0x10, 0x24, 0x00, 0x00, 
  0x40, 0x20, 0x0C, 0x06, 0x01, 0xC0, 0x40, 0x20, 0x09, 0x02, 
  0xB8, 0x10, 0x1C, 0x00, 0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 
  0xC0, 0x80, 0x27, 0x08, 0x0A, 0x00, 0x00, 0x20, 0x30, 0x0E, 
  0x01, 0x01, 0x03, 0x00, 0x4A, 0x20, 0x80, 0x42, 0x70, 0x28, 
  0x54, 0x05, 0x0D, 0xC0, 0xA1, 0xD0, 0x14, 0x41, 0x02, 0xBC, 
  0x74, 0x08, 0x90, 0x63, 0x00, 0x86, 0xA0, 0x50, 0xFC, 0x0A, 
  0x26, 0x01, 0x46, 0x30, 0x2B, 0xBD, 0x01, 0x08, 0x02, 0x80, 
  0x8A, 0xBA, 0x80, 0x64, 0x02, 0x20, 0x8A, 0xE7, 0xDB, 0x88, 
  0x03, 0x05, 0xC0, 0x00, 0x10, 0x40, 0x3C, 0x2C, 0x3B, 0x1E, 
  0x0F, 0x48, 0x84, 0xB2, 0x91, 0xDC, 0xF0, 0x79, 0x40, 0x21,
  0xD1, 0xCB, 0x35, 0xA2, 0xD5, 0x78, 0xC3, 0x65, 0x3B, 0xC9,
  0x00, 0x09, 0x40, 0x00, 0x28, 0x4E, 0x05, 0x0A, 0x80, 0xA1, 
  0xB8, 0x14, 0x3A, 0x02, 0x88, 0x20, 0x57, 0x8E, 0xAB, 0x81, 
  0x41, 0xAE, 0x09, 0x0A, 0xB8, 0x34, 0x3A, 0xE1, 0x01, 0x00, 
  0x06, 0x11, 0x5E, 0x3A, 0xAE, 0x51, 0x43, 0x00, 0x1A, 0x61, 
  0x44, 0xD3, 0x0C, 0x0D, 0x30, 0x80
*/
  /* TDD---CCSA */
  0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0x91,
  0xC0, 0xD2, 0x0D, 0x06, 0x08, 0x40, 0xC2, 0x16, 0x7A, 0x55,
  0x00, 0x88, 0x03, 0x5A, 0x4D, 0x06, 0x83, 0x14, 0xA0, 0x63,
  0x14, 0x23, 0x41, 0x92, 0x90, 0x32, 0x9C, 0xC0, 0x40, 0x40,
  0x00, 0x44, 0x23, 0x88, 0x18, 0x00, 0x80, 0x00, 0xAE, 0x20,
  0x60, 0x12, 0x00, 0x23, 0x38, 0x81, 0x80, 0x88, 0x01, 0x1E,
  0x20, 0x18, 0x17, 0x00, 0x48, 0x0A, 0x42, 0x12, 0x1E, 0x91,
  0x64, 0x9D, 0xA4, 0xF1, 0x27, 0xA9, 0x44, 0x4A, 0x5A, 0x54,
  0x92, 0xC2, 0x7E, 0x40, 0x0F, 0x81, 0x02, 0xC0, 0x00, 0x04,
  0x02, 0x00, 0xC0, 0x40, 0x18, 0x07, 0x01, 0x40, 0x80, 0x24,
  0x0A, 0x02, 0xF8, 0x10, 0x24, 0x00, 0x00, 0x40, 0x20, 0x0C,
  0x06, 0x01, 0xC0, 0x40, 0x20, 0x09, 0x02, 0xB8, 0x10, 0x1C,
  0x00, 0x00, 0x40, 0x20, 0x18, 0x07, 0x00, 0xC0, 0x80, 0x27,
  0x08, 0x0A, 0x00, 0x00, 0x20, 0x30, 0x0E, 0x01, 0x01, 0x03,
  0x00, 0x4A, 0x20, 0x80, 0x42, 0x70, 0x28, 0x54, 0x05, 0x0D,
  0xC0, 0xA1, 0xD0, 0x14, 0x41, 0x02, 0xBC, 0x74, 0x08, 0x90,
  0x63, 0x00, 0x86, 0xA0, 0x50, 0xFC, 0x0A, 0x26, 0x01, 0x46,
  0x30, 0x2B, 0xBD, 0x01, 0x08, 0x02, 0x80, 0x8A, 0xBA, 0x80,
  0x64, 0x02, 0x20, 0x8A, 0xE7, 0xDB, 0x88, 0x03, 0x05, 0xC0,
  0x00, 0x10, 0x40, 0x3C, 0x2C, 0x3B, 0x1E, 0x0F, 0x48, 0x84,
  0xB2, 0x91, 0xDC, 0xF0, 0x79, 0x40, 0x21, 0xD3, 0x0B, 0xD5,
  0xA2, 0xD5, 0x78, 0xC3, 0x77, 0xB6, 0x89, 0x00, 0x09, 0x40,
  0x00, 0x28, 0x4E, 0x05, 0x0A, 0x80, 0xA1, 0xB8, 0x14, 0x3A,
  0x02, 0x88, 0x20, 0x57, 0x8E, 0xAB, 0x81, 0x41, 0xAE, 0x09,
  0x0A, 0xB8, 0x34, 0x3A, 0xE1, 0x01, 0x00, 0x06, 0x11, 0x5E,
  0x3A, 0xAE, 0x51, 0x43, 0x00, 0x1A, 0x61, 0x44, 0xD3, 0x0C,
  0x0D, 0x30, 0x80
};
  
/*hardcode the number of bytes in default config 11*/
/* FDD
uint32 tdsdefault_config_11_len = 271; 
*/
/* TDD */
uint32 tdsdefault_config_11_len = 263;


uint8 tdsdefault_config_12[] = 
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
    /* FDD
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
    */

  /* TDD
  0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0x91,
  0xC0, 0xD2, 0x0D, 0x04, 0x08, 0x40, 0x82, 0x16, 0x7A, 0x55,
  0x00, 0x88, 0x03, 0x5A, 0x4D, 0x06, 0x82, 0x14, 0xA0, 0x43,
  0x14, 0x23, 0x41, 0x12, 0x90, 0x22, 0x98, 0xC0, 0x40, 0x30,
  0x00, 0x42, 0x23, 0x88, 0x1A, 0x00, 0x00, 0x80, 0x00, 0xAE,
  0x20, 0x68, 0x10, 0x02, 0x02, 0x07, 0x88, 0x06, 0x05, 0xC0,
  0x12, 0x02, 0x90, 0x84, 0x87, 0xA4, 0x59, 0x23, 0xA9, 0x1E,
  0x48, 0xFA, 0x49, 0x92, 0x5A, 0x93, 0x44, 0x9D, 0x9F, 0x90,
  0x03, 0xC2, 0x05, 0x80, 0x00, 0x08, 0x04, 0x01, 0x80, 0x80,
  0x30, 0x0E, 0x02, 0x81, 0x00, 0x48, 0x14, 0x05, 0xE1, 0x02,
  0x40, 0x00, 0x04, 0x02, 0x00, 0xC0, 0x60, 0x1C, 0x04, 0x02,
  0x00, 0x90, 0x2B, 0x08, 0x0E, 0x00, 0x00, 0x20, 0x10, 0x0C,
  0x03, 0x80, 0x60, 0x40, 0x13, 0x84, 0x05, 0x00, 0x00, 0x10,
  0x18, 0x07, 0x00, 0x80, 0x81, 0x00, 0x25, 0x0F, 0x40, 0x21,
  0x38, 0x14, 0x2A, 0x02, 0x86, 0xE0, 0x50, 0xE8, 0x0A, 0x1E,
  0x81, 0x5E, 0x3A, 0x04, 0x48, 0x2B, 0x80, 0x43, 0x50, 0x28,
  0x7E, 0x05, 0x13, 0x00, 0xA2, 0xB8, 0x15, 0xDE, 0x80, 0x88,
  0x04, 0x41, 0x15, 0xCF, 0xB7, 0x10, 0x06, 0x0B, 0x80, 0x00,
  0x20, 0x80, 0x78, 0x58, 0x3A, 0x1E, 0x0F, 0x89, 0x85, 0xA3,
  0x41, 0xD8, 0xF0, 0x7A, 0x44, 0x25, 0x94, 0x8B, 0x25, 0xA2,
  0xD9, 0x88, 0xD2, 0x70, 0x3B, 0x8E, 0x00, 0x12, 0x80, 0x00,
  0x50, 0x9C, 0x0A, 0x15, 0x01, 0x43, 0x70, 0x28, 0x74, 0x05,
  0x0F, 0x40, 0xAF, 0x1D, 0x57, 0x02, 0x83, 0x5C, 0x12, 0x15,
  0x60, 0x60, 0x80, 0x03, 0x08, 0xAF, 0x1D, 0x4A, 0x65, 0x04,
  0x03, 0x4C, 0x28, 0x9A, 0x60, 0x8D, 0x30, 0x80
*/
  /* TDD---CCSA */

  0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0x91,
  0xC0, 0xD2, 0x0D, 0x04, 0x08, 0x40, 0x82, 0x16, 0x7A, 0x55,
  0x00, 0x88, 0x03, 0x5A, 0x4D, 0x06, 0x82, 0x14, 0xA0, 0x43,
  0x14, 0x23, 0x41, 0x12, 0x90, 0x22, 0x98, 0xC0, 0x40, 0x30,
  0x00, 0x42, 0x23, 0x88, 0x18, 0x00, 0x80, 0x00, 0xAE, 0x20,
  0x60, 0x12, 0x00, 0x27, 0x88, 0x06, 0x05, 0xC0, 0x12, 0x02,
  0x90, 0x84, 0x87, 0xA4, 0x59, 0x23, 0xA9, 0x1E, 0x48, 0xFA,
  0x49, 0x92, 0x5A, 0x93, 0x44, 0x9D, 0x9F, 0x90, 0x03, 0xC2,
  0x05, 0x80, 0x00, 0x08, 0x04, 0x01, 0x80, 0x80, 0x30, 0x0E,
  0x02, 0x81, 0x00, 0x48, 0x14, 0x05, 0xE1, 0x02, 0x40, 0x00,
  0x04, 0x02, 0x00, 0xC0, 0x60, 0x1C, 0x04, 0x02, 0x00, 0x90,
  0x2B, 0x08, 0x0E, 0x00, 0x00, 0x20, 0x10, 0x0C, 0x03, 0x80,
  0x60, 0x40, 0x13, 0x84, 0x05, 0x00, 0x00, 0x10, 0x18, 0x07,
  0x00, 0x80, 0x81, 0x00, 0x25, 0x0F, 0x40, 0x21, 0x38, 0x14,
  0x2A, 0x02, 0x86, 0xE0, 0x50, 0xE8, 0x0A, 0x1E, 0x81, 0x5E,
  0x3A, 0x04, 0x48, 0x2B, 0x80, 0x43, 0x50, 0x28, 0x7E, 0x05,
  0x13, 0x00, 0xA2, 0xB8, 0x15, 0xDE, 0x80, 0x88, 0x04, 0x41,
  0x15, 0xCF, 0xB7, 0x10, 0x06, 0x0B, 0x80, 0x00, 0x20, 0x80,
  0x78, 0x58, 0x3A, 0x1E, 0x0F, 0x89, 0x46, 0xE3, 0x41, 0xD8,
  0xF0, 0x7A, 0x44, 0x25, 0x94, 0x8B, 0x25, 0xA2, 0xD9, 0x88,
  0xD2, 0x70, 0x3B, 0x8E, 0x00, 0x12, 0x80, 0x00, 0x50, 0x9C,
  0x0A, 0x15, 0x01, 0x43, 0x70, 0x28, 0x74, 0x05, 0x0F, 0x40,
  0xAF, 0x1D, 0x57, 0x02, 0x83, 0x5C, 0x12, 0x15, 0x60, 0x60,
  0x80, 0x03, 0x08, 0xAF, 0x1D, 0x4A, 0x65, 0x04, 0x03, 0x4C,
  0x28, 0x9A, 0x60, 0x8D, 0x30, 0x80

};

/*hardcode the number of bytes in default config 12 */
/* FDD
uint32 tdsdefault_config_12_len = 254;
*/
/* TDD */
uint32 tdsdefault_config_12_len = 246;


uint8 tdsdefault_config_13[] =
{
/* FDD
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
  */
  /* TDD 
0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0x01,
0xC0, 0xD2, 0x0D, 0x06, 0x08, 0x40, 0xC2, 0x16, 0x7A, 0x55,
0x00, 0x88, 0x03, 0x5A, 0x49, 0x06, 0x83, 0x14, 0xA0, 0x63,
0x14, 0x23, 0x41, 0x92, 0x90, 0x32, 0x9C, 0xC0, 0x40, 0x40,
0x00, 0x44, 0x23, 0x88, 0x1A, 0x00, 0x08, 0x80, 0x00, 0xAE,
0x20, 0x68, 0x10, 0x22, 0x02, 0x03, 0x38, 0x81, 0xA0, 0x80,
0x88, 0x10, 0x1E, 0x20, 0x10, 0x13, 0x01, 0x20, 0xA4, 0x74,
0x9A, 0x94, 0xD2, 0xA2, 0x55, 0x4B, 0x69, 0x85, 0x33, 0x9F,
0x90, 0x02, 0xC2, 0x02, 0x80, 0x00, 0x08, 0x04, 0x02, 0x80,
0xC0, 0x3E, 0x10, 0x1C, 0x00, 0x00, 0x40, 0x20, 0x0C, 0x05,
0x01, 0x80, 0x70, 0x23, 0x08, 0x12, 0x00, 0x00, 0x20, 0x10,
0x06, 0x02, 0x00, 0xA0, 0x30, 0x0E, 0x04, 0x01, 0x23, 0x00,
0x48, 0x24, 0x00, 0x02, 0x80, 0x20, 0x6C, 0x04, 0x10, 0x00,
0x82, 0x40, 0x11, 0xE3, 0xA0, 0x44, 0x66, 0xD0, 0x00, 0x27,
0x01, 0x07, 0x10, 0x21, 0xB4, 0x02, 0x3B, 0xD0, 0x10, 0x80,
0x3C, 0x00, 0x3F, 0xF0, 0x19, 0x00, 0x88, 0x22, 0x3A, 0x96,
0xE2, 0x00, 0xC1, 0x30, 0x00, 0x04, 0x0E, 0x0D, 0x09, 0x85,
0x02, 0xA1, 0xB1, 0x08, 0x9C, 0x50, 0x29, 0x17, 0x8D, 0x47,
0x63, 0xC1, 0xE9, 0x0C, 0x92, 0x4F, 0x24, 0x00, 0x24, 0x00,
0x00, 0x81, 0x40, 0x10, 0x36, 0x02, 0x08, 0x00, 0x41, 0x20,
0x08, 0xF1, 0xD5, 0x60, 0x28, 0x20, 0x48, 0x40, 0x68, 0x70,
0x80, 0x80, 0x03, 0x08, 0x8F, 0x30, 0xD7, 0x2A
*/

/* TDD CCSA */
0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x55, 0x01,
0xC0, 0xD2, 0x0D, 0x06, 0x08, 0x40, 0xC2, 0x16, 0x7A, 0x55,
0x00, 0x88, 0x03, 0x5A, 0x49, 0x06, 0x83, 0x14, 0xA0, 0x63,
0x14, 0x23, 0x41, 0x92, 0x90, 0x32, 0x9C, 0xC0, 0x40, 0x40,
0x00, 0x42, 0x23, 0x88, 0x1A, 0x00, 0x08, 0x80, 0x00, 0xAE,
0x20, 0x68, 0x10, 0x22, 0x02, 0x07, 0x88, 0x04, 0x04, 0xC0,
0x48, 0x29, 0x1D, 0x26, 0xA5, 0x34, 0xA8, 0x95, 0x52, 0xDA,
0x61, 0x4C, 0xE7, 0xE4, 0x00, 0xB0, 0x80, 0xA0, 0x00, 0x02,
0x01, 0x00, 0xA0, 0x30, 0x0F, 0x84, 0x07, 0x00, 0x00, 0x10,
0x08, 0x03, 0x01, 0x40, 0x60, 0x1C, 0x08, 0xC2, 0x04, 0x80,
0x00, 0x08, 0x04, 0x01, 0x80, 0x80, 0x28, 0x0C, 0x03, 0x81,
0x00, 0x48, 0x80, 0x12, 0x09, 0x00, 0x00, 0xA0, 0x08, 0x1B,
0x01, 0x04, 0x00, 0x20, 0x90, 0x04, 0x78, 0xE8, 0x11, 0x19,
0xB4, 0x00, 0x09, 0xC0, 0x41, 0xC4, 0x08, 0x6D, 0x00, 0x8E,
0xF4, 0x06, 0x40, 0x22, 0x08, 0x8E, 0xA5, 0xB8, 0x80, 0x30,
0x4C, 0x00, 0x01, 0x03, 0x83, 0x42, 0x61, 0x40, 0xA8, 0x6C,
0x42, 0x27, 0x14, 0x0A, 0x45, 0xE3, 0x51, 0xD8, 0xF0, 0x7A,
0x43, 0x24, 0x93, 0xC7, 0x00, 0x09, 0x00, 0x00, 0x20, 0x50,
0x04, 0x0D, 0x80, 0x82, 0x00, 0x10, 0x48, 0x02, 0x3C, 0x75,
0x58, 0x0A, 0x08, 0x1A, 0x1C, 0x20, 0x20, 0x00, 0xC2, 0x23,
0xCC, 0x35, 0xCA, 0x80

};
/* FDD
uint32 tdsdefault_config_13_len =228;
*/
/* TDD 
uint32 tdsdefault_config_13_len =218;
*/

/* TDD CCSA */
uint32 tdsdefault_config_13_len =204;


uint8 tdsdefault_config_14[] = 
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

/* FDD
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
*/
/* TDD */
0x9D, 0x1E, 0x35, 0x75, 0x0B, 0xA4, 0x00, 0x19, 0x54, 0x01,
0xC0, 0xD2, 0x0D, 0x10, 0x08, 0x42, 0x02, 0x16, 0x7C, 0x33,
0x0B, 0x26, 0x3C, 0x03, 0x38, 0x4C, 0x34, 0x40, 0xA5, 0x08,
0x18, 0xA1, 0x1A, 0x20, 0x94, 0x84, 0x14, 0xE6, 0x02, 0x02,
0x00, 0x02, 0x21, 0x1C, 0x40, 0xC0, 0x04, 0x00, 0x05, 0x71,
0x03, 0x00, 0x90, 0x01, 0x19, 0xC4, 0x0C, 0x04, 0x40, 0x08,
0xF1, 0x00, 0xC0, 0xB8, 0x02, 0x40, 0x52, 0x10, 0x90, 0xF4,
0x8B, 0x24, 0xED, 0x27, 0x89, 0x3D, 0x4A, 0x22, 0x52, 0xD2,
0xA4, 0x97, 0x73, 0xF2, 0x00, 0x7C, 0x08, 0x0A, 0x00, 0x00,
0x20, 0x10, 0x0C, 0x03, 0x81, 0x1C, 0x08, 0x0E, 0x00, 0x00,
0x20, 0x10, 0x06, 0x03, 0x00, 0xE0, 0x40, 0x13, 0xC0, 0x81,
0x20, 0x00, 0x02, 0x01, 0x00, 0x60, 0x20, 0x0C, 0x03, 0x81,
0x00, 0x48, 0x15, 0x84, 0x0B, 0x00, 0x00, 0x10, 0x08, 0x03,
0x01, 0x00, 0x50, 0x18, 0x07, 0x02, 0x00, 0x90, 0x28, 0x0B,
0x18, 0x02, 0x51, 0x44, 0x00, 0x13, 0x81, 0x02, 0xA0, 0x20,
0x6E, 0x04, 0x0F, 0x40, 0x82, 0x88, 0x11, 0xE3, 0xA0, 0x44,
0x83, 0x38, 0x00, 0x35, 0x02, 0x07, 0xE0, 0x41, 0x5C, 0x08,
0x33, 0x81, 0x1D, 0xE8, 0x08, 0x42, 0x1E, 0x00, 0x03, 0xC0,
0x22, 0xEA, 0x04, 0x10, 0x08, 0x82, 0x23, 0x9F, 0x6E, 0x20,
0x0C, 0x17, 0x00, 0x00, 0x41, 0x00, 0xF0, 0xB0, 0xEC, 0x78,
0x3D, 0x22, 0x12, 0xCA, 0x47, 0x73, 0xC1, 0xE5, 0x00, 0x87,
0x47, 0x2C, 0xD6, 0x8B, 0x55, 0xE3, 0x0D, 0x94, 0xEF, 0x24,
0x00, 0x25, 0x00, 0x00, 0x81, 0x38, 0x10, 0x2A, 0x02, 0x06,
0xE0, 0x40, 0xF4, 0x08, 0x28, 0x81, 0x1E, 0x3A, 0xAC, 0x05,
0x04, 0x09, 0x08, 0x21, 0x20, 0x10, 0x10, 0x00, 0x61, 0x11,
0xE6, 0x18
};

/*hardcode the number of bytes in default config 14 */
/* FDD
uint32 tdsdefault_config_14_len = 273;
*/
/* TDD */
uint32 tdsdefault_config_14_len = 252;

#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */


/* Default config 15,  7.95 kbps speech +   3.4 kbps signalling */
uint8 tdsdefault_config_15[] = 
{
/* FDD
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
*/

/* TDD */
  0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19,
  0x85, 0x93, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18,
  0x99, 0xF0, 0xCC, 0x2C, 0x98, 0xF2, 0x70, 0x98, 0xD1, 0x04,
  0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x15, 0xC4,
  0x18, 0x05, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x98, 0x02, 0x38,
  0x80, 0x20, 0x2C, 0x12, 0x29, 0x54, 0xB2, 0x5D, 0x36, 0x7E,
  0x41, 0x80, 0x22, 0x12, 0xC0, 0x01, 0x38, 0x10, 0x4B, 0x02,
  0x3C, 0x74, 0x08, 0x80, 0x54, 0x08, 0x8E, 0xF4, 0x04, 0x20,
  0x0F, 0x00, 0x0B, 0xA8, 0x10, 0x40, 0x22, 0x08, 0x8E, 0x7D,
  0xB1, 0x0C, 0x07, 0x00, 0x11, 0x00, 0x00, 0x40, 0x9C, 0x08,
  0x25, 0x81, 0x1E, 0x3A, 0xA8, 0x06, 0x08, 0x14, 0x20, 0x89,
  0x1C, 0x7D, 0x50, 0xF5, 0x40
};


/*hardcode the number of bytes in default config 15 */
/* FDD
uint8 tdsdefault_config_15_len = 110;
*/
/* TDD */
uint8 tdsdefault_config_15_len = 115;

/* Default config 18, 12.2 kbps speech + 13.6 kbps signalling */
uint8 tdsdefault_config_18[] = 
{
/* FDD
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
  */

/* TDD */
0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3D, 0x19,
0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18,
0x99, 0xE8, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04,
0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x15, 0xC4,
0x18, 0x05, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x98, 0x02, 0x38,
0x80, 0x40, 0x54, 0x04, 0x82, 0x92, 0xD2, 0x62, 0x4D, 0x4A,
0xE7, 0xE5, 0x84, 0xB2, 0x99, 0x13, 0x2A, 0x70, 0x48, 0x30,
0x04, 0x42, 0x88, 0x00, 0x27, 0x02, 0x0A, 0x20, 0x47, 0x8E,
0x81, 0x10, 0x0C, 0xE1, 0x11, 0xDE, 0x80, 0x84, 0x01, 0xE0,
0x44, 0x5D, 0x40, 0x82, 0x01, 0x11, 0x06, 0x08, 0x62, 0x1C,
0xFB, 0x62, 0x18, 0x0E, 0x00, 0x22, 0x00, 0x00, 0x81, 0x38,
0x10, 0x51, 0x02, 0x3C, 0x75, 0x50, 0x0C, 0x10, 0x28, 0x41,
0x12, 0x30, 0x1B, 0xC0, 0x00
};

/*hardcode the number of bytes in default config 18 */
/* FDD
uint8 tdsdefault_config_18_len = 121;
*/
/* TDD */
uint8 tdsdefault_config_18_len = 125;


/* Default config 19, 12.2/7.95/5.9/4.75  kbps speech +
  * 13.6 kbps signalling
  */
uint8 tdsdefault_config_19[] = 
{
/* FDD
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
  */

/* TDD */
0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3D, 0x19,
0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18,
0x99, 0xE8, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04,
0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x08, 0x00, 0x15, 0xC4,
0x18, 0x04, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x88, 0x02, 0x38,
0x80, 0x60, 0x84, 0x01, 0x20, 0x29, 0x08, 0x48, 0x7A, 0x45,
0x92, 0x76, 0x93, 0xC4, 0x9E, 0xA5, 0x11, 0x29, 0x69, 0x52,
0x4B, 0xB9, 0xF9, 0x78, 0x4B, 0xCA, 0x6D, 0x13, 0x6A, 0x9F,
0x04, 0x83, 0x00, 0x4A, 0x28, 0x80, 0x02, 0x70, 0x20, 0x54,
0x04, 0x0D, 0xC0, 0x82, 0x58, 0x10, 0x51, 0x02, 0x3C, 0x74,
0x08, 0x90, 0x67, 0x00, 0x06, 0xA0, 0x40, 0xFC, 0x08, 0x2A,
0x01, 0x06, 0x70, 0x23, 0xBD, 0x01, 0x08, 0x43, 0xC0, 0x00,
0x78, 0x04, 0x5D, 0x40, 0x82, 0x09, 0x10, 0x00, 0x88, 0x6C,
0x10, 0xC4, 0x39, 0xF6, 0xC4, 0x30, 0x1C, 0x00, 0x4A, 0x00,
0x01, 0x02, 0x70, 0x20, 0x54, 0x04, 0x0D, 0xC0, 0x82, 0x58,
0x10, 0x51, 0x02, 0x3C, 0x75, 0x50, 0x0C, 0x10, 0x28, 0x41,
0x12, 0x30, 0x1B, 0xC0, 0x00
};

/*hardcode the number of bytes in default config 18 */
/* FDD
uint8 tdsdefault_config_19_len = 161;
*/
/* TDD */
uint8 tdsdefault_config_19_len = 165;

/* Default config 22,  13.6 kbps signalling */

uint8 tdsdefault_config_16[] = 
{
/* FDD
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
*/
    
/* TDD */
  0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19,
  0x85, 0x93, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18,
  0x99, 0xF0, 0xCC, 0x2C, 0x98, 0xF2, 0x70, 0x98, 0xD1, 0x04,
  0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x15, 0xC4,
  0x18, 0x05, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x98, 0x02, 0x38,
  0x80, 0x40, 0x4C, 0x04, 0x82, 0x91, 0xD2, 0x6A, 0x53, 0x4A,
  0x89, 0x55, 0x2D, 0xA6, 0x14, 0xCE, 0x7E, 0x41, 0x80, 0x24,
  0x12, 0x00, 0x01, 0x40, 0x10, 0x36, 0x02, 0x08, 0x00, 0x41,
  0x20, 0x08, 0xF1, 0xD0, 0x22, 0x33, 0x68, 0x00, 0x13, 0x80,
  0x83, 0x88, 0x10, 0xDA, 0x01, 0x1D, 0xE8, 0x08, 0x40, 0x1E,
  0x00, 0x1F, 0xF8, 0x20, 0x80, 0x44, 0x11, 0x1D, 0x4B, 0x62,
  0x18, 0x0E, 0x00, 0x24, 0x00, 0x00, 0x81, 0x40, 0x10, 0x36,
  0x02, 0x08, 0x00, 0x41, 0x20, 0x08, 0xF1, 0xD5, 0x40, 0x30,
  0x40, 0xA1, 0x04, 0x48, 0xE3, 0x80, 0xC0, 0x00
};

/*hardcode the number of bytes in default config 16 */
/* FDD
 uint8 tdsdefault_config_16_len = 134;
 */
 /* TDD */
uint8 tdsdefault_config_16_len = 138;


/* Default config 20 ,  64kbps conv. CS- data + 13.6 kbps signalling */
uint8 tdsdefault_config_20[] = 
{
/* FDD
  0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19, 
  0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
  0x99, 0xF0, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
  0xA4, 0x41, 0x01, 0x1C, 0x41, 0x80, 0x08, 0x00, 0x20, 0x40, 
  0x68, 0x24, 0x52, 0x49, 0x32, 0x8F, 0x28, 0x96, 0x4C, 0x20, 
  0x40, 0x10, 0x5A, 0xD0, 0x90, 0x15, 0x4B, 0x20, 0x80, 0x44, 
  0x41, 0x82, 0x18, 0x87, 0x3E, 0xCC, 0x30, 0x10, 0x24, 0x08, 
  0x91, 0x2E, 0x4F, 
*/

/* TDD */
0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19,
0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18,
0x99, 0xF0, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04,
0xA4, 0x41, 0x01, 0x1C, 0x41, 0x80, 0x08, 0x00, 0x38, 0x80,
0x20, 0x34, 0x12, 0x29, 0x24, 0x99, 0xF9, 0x44, 0xB2, 0x61,
0x20, 0x40, 0x10, 0x5A, 0xD0, 0x90, 0x15, 0x4B, 0x20, 0x80,
0x44, 0x41, 0x82, 0x18, 0x87, 0x3E, 0xD8, 0x86, 0x01, 0x80,
0x81, 0x20, 0x44, 0x8C, 0x08, 0x0C, 0x00
};

/*hardcode the number of bytes in default config 20 */
/* FDD
uint8 tdsdefault_config_20_len = 73;
*/
/* TDD */
uint8 tdsdefault_config_20_len = 77;


uint8 tdsdefault_config_21[] = 
{
/* FDD
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
*/
/* TDD */
0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19,
0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18,
0x99, 0xF0, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04,
0xA4, 0x41, 0x21, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x15, 0xC4,
0x18, 0x05, 0x80, 0x11, 0x9C, 0x41, 0x80, 0x98, 0x02, 0x38,
0x80, 0x60, 0x74, 0x01, 0x20, 0x29, 0x07, 0x48, 0x6A, 0x44,
0xD2, 0x28, 0x91, 0x54, 0x8D, 0xA4, 0x85, 0x24, 0xE7, 0xE4,
0xA1, 0x25, 0x29, 0x3C, 0x49, 0xEA, 0x54, 0x12, 0x0C, 0x01,
0x20, 0x90, 0x00, 0x0A, 0x00, 0x81, 0xB0, 0x10, 0x40, 0x02,
0x09, 0x00, 0x47, 0x8E, 0x81, 0x11, 0x9B, 0x40, 0x00, 0x9C,
0x04, 0x1C, 0x40, 0x86, 0xD0, 0x08, 0xEF, 0x40, 0x42, 0x00,
0xF0, 0x00, 0xFF, 0xC1, 0x04, 0x02, 0x22, 0x0C, 0x10, 0xC4,
0x3A, 0x96, 0xC4, 0x30, 0x1C, 0x00, 0x48, 0x00, 0x01, 0x02,
0x80, 0x20, 0x6C, 0x04, 0x10, 0x00, 0x82, 0x40, 0x11, 0xE3,
0xAA, 0x80, 0x60, 0x81, 0x42, 0x08, 0x91, 0x81, 0x01, 0x80
};

/*hardcode the number of bytes in default config 21 */
/* FDD
uint8 tdsdefault_config_21_len = 147;
*/
/* TDD */
uint8 tdsdefault_config_21_len = 150;


/* Default config 22,  13.6 kbps signalling */
uint8 tdsdefault_config_22[] = 
{
/* FDD
    0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19,
    0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18, 
    0x99, 0xF0, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04, 
    0xA4, 0x41, 0x01, 0x1C, 0x41, 0x80, 0x18, 0x00, 0xA4, 0x01, 
    0x00, 0x00, 0x10, 0x00, 0x0C, 0x4A, 0x57, 0xE0, 0x08, 0x00, 
    0x11, 0x04, 0x47, 0x3E, 0xCC, 0x11, 0x12, 0x2B, 0x97, 0xEA,
    0x00
*/
/* TDD */
0x14, 0x0D, 0x23, 0x44, 0x02, 0x11, 0x01, 0x0B, 0x3E, 0x19,
0x85, 0x89, 0x1E, 0x4E, 0x13, 0x1A, 0x20, 0x52, 0x88, 0x18,
0x99, 0xF0, 0xCC, 0x2C, 0x48, 0xF2, 0x70, 0x98, 0xD1, 0x04,
0xA4, 0x41, 0x01, 0x1C, 0x41, 0x80, 0x18, 0x00, 0x38, 0x80,
0x00, 0x0C, 0x4A, 0x7E, 0x40, 0x10, 0x00, 0x22, 0x08, 0x8E,
0x7D, 0xB1, 0x0C, 0x01, 0x11, 0x22, 0xBE, 0x3E, 0x18, 0x78,
0x60
};
/*hardcode the number of bytes in default config 22 */
/* FDD
uint8 tdsdefault_config_22_len = 61;
*/
/* TDD */
uint8 tdsdefault_config_22_len = 61;


/* UPA or DPA on SRB, now unsupport */
uint8 tdsdefault_config_17[] = 
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
uint8 tdsdefault_config_17_len = 59;

/* UPA or DPA on SRB, now unsupport */
uint8 tdsdefault_config_23[] = 
{
    /* This is basically dump of Rel-6 RB setup message
       with contents set to match default configuration 23.
       RB setup contains only relevant IEs which are of 
       interest to us -
          SRB Info to setup,
          UL & DL AddReconf Transport channel information,
    */
0x3A, 0x60, 0x00, 0x00, 0x04, 0x02, 0x54, 0x4C, 0x7C, 0x53, 0x0B, 0x12, 0x1C, 0x21, 0x58, 0x4C, 0x34, 0xB0, 0x60, 0x20, 0x4C, 0xC5, 0x0B,
0x1F, 0x14, 0xC2, 0xC4, 0x87, 0x08, 0x56, 0x13, 0x0D, 0x1C, 0x10, 0x08, 0x0B, 0x30, 0xC0, 0xC9, 0x03, 0x43, 0x02, 0x02, 0x00, 0xCC, 0x00, 
0xF1, 0xFA, 0x28, 0x04, 0x0A, 0xEA, 0x11, 0xA5, 0x80, 0x49, 0x80, 

};

/*hardcode the number of bytes in default config 23 */
uint8 tdsdefault_config_23_len = 57;

#endif /* FEATURE_TDSCDMA_DEFAULT_CONFIG_SUPPORT */

/* variable to indicate TDS RRC RELEASE (REL99/REL-5/REL-6).
 * This variable will be set using the value of NV item 'NV_WCDMA_RRC_VERSION_I'
 */
uint8 tdsrrc_nv_rel_indicator;

#ifdef FEATURE_UMTS_PDCP
boolean tdsrrc_pdcp_enabled;
#endif /* FEATURE_UMTS_PDCP*/

#ifdef FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
int16  tdsrrc_rscp;
int16  tdsrrc_ecio;
int16  tdsrrc_rssi;
boolean tdsrrc_use_ecio_for_ui_display;
#else
uint8 tdsrrc_rssi;
#endif

msgr_client_t  tdsrrc_msgr_client;
msgr_id_t      tdsrrc_rex_q_id;

uint8   tdsMaxSynUlTrans[4] = 
{
/* tdsrrc_SYNC_UL_Info_r4_max_SYNC_UL_Transmissions_tr1     */    1,
/* tdsrrc_SYNC_UL_Info_r4_max_SYNC_UL_Transmissions_tr2     */    2,
/* tdsrrc_SYNC_UL_Info_r4_max_SYNC_UL_Transmissions_tr4     */    4,
/* tdsrrc_SYNC_UL_Info_r4_max_SYNC_UL_Transmissions_tr8     */    8
};

int8  tdsTddPrachCode_R4[28][2] = 
{
/*                       SF, chcode */
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc4_1,    */{TDSL1_SF_4,1},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc4_2,    */{TDSL1_SF_4,2},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc4_3,    */{TDSL1_SF_4,3},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc4_4,    */{TDSL1_SF_4,4},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc8_1,    */{TDSL1_SF_8,1},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc8_2,    */{TDSL1_SF_8,2},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc8_3,    */{TDSL1_SF_8,3},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc8_4,    */{TDSL1_SF_8,4},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc8_5,    */{TDSL1_SF_8,5},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc8_6,    */{TDSL1_SF_8,6},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc8_7,    */{TDSL1_SF_8,7},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc8_8,    */{TDSL1_SF_8,8},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_1,   */{TDSL1_SF_16,1},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_2,   */{TDSL1_SF_16,2},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_3,   */{TDSL1_SF_16,3},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_4,   */{TDSL1_SF_16,4},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_5,   */{TDSL1_SF_16,5},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_6,   */{TDSL1_SF_16,6},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_7,   */{TDSL1_SF_16,7},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_8,   */{TDSL1_SF_16,8},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_9,   */{TDSL1_SF_16,9},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_10,  */{TDSL1_SF_16,10},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_11,  */{TDSL1_SF_16,11},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_12,  */{TDSL1_SF_16,12},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_13,  */{TDSL1_SF_16,13},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_14,  */{TDSL1_SF_16,14},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_15,  */{TDSL1_SF_16,15},
/* tdsrrc_TDD_PRACH_CCode_LCR_r4_cc16_16   */{TDSL1_SF_16,16}
};

uint8 tdsMidambleConfiguration[9] = {
  0,
  TDSL1_MIDAMBLE_CONFIG_2,
  TDSL1_MIDAMBLE_CONFIG_4,
  TDSL1_MIDAMBLE_CONFIG_6,
  TDSL1_MIDAMBLE_CONFIG_8,
  TDSL1_MIDAMBLE_CONFIG_10,
  TDSL1_MIDAMBLE_CONFIG_12,
  TDSL1_MIDAMBLE_CONFIG_14,
  TDSL1_MIDAMBLE_CONFIG_16
};

/* TDS RRC TCB handle */
static rex_tcb_type* tds_rrc_tcb_ptr = RCINIT_NULL; 


rex_tcb_type tds_rrc_tcb;  // for Triton compilation only, TBD

STATIC msgr_umid_type tdsrrc_msgr_x2t_umid_list[] =
{
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
  TDSCDMA_RRC_LTE_RESEL_REQ,
  TDSCDMA_RRC_LTE_ABORT_RESEL_REQ,
  TDSCDMA_RRC_LTE_REDIR_REQ,
  TDSCDMA_RRC_LTE_ABORT_REDIR_REQ,
  TDSCDMA_RRC_LTE_PLMN_SRCH_REQ,
  TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ,
  TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_REQ,
#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
  TDSCDMA_RRC_LTE_PSHO_REQ,
  TDSCDMA_RRC_LTE_ABORT_PSHO_REQ,
#endif
  LTE_RRC_TDSCDMA_RESEL_FAILED_RSP,
  LTE_RRC_TDSCDMA_ABORT_RESEL_RSP,
  LTE_RRC_TDSCDMA_REDIR_FAILED_RSP,
  LTE_RRC_TDSCDMA_ABORT_REDIR_RSP,
  LTE_RRC_TDSCDMA_PLMN_SRCH_RSP,
  LTE_RRC_TDSCDMA_ABORT_PLMN_SRCH_RSP,
  LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_RSP,
  LTE_RRC_TDSCDMA_GET_DEDICATED_PRI_RSP,
  TDSCDMA_RRC_LTE_GET_DEDICATED_PRI_REQ,
  TDSCDMA_RRC_LTE_CLEAR_DEDICATED_PRI_IND,
  TDSCDMA_RRC_LTE_GET_CGI_REQ,
  TDSCDMA_RRC_LTE_ABORT_CGI_REQ,
  TDSCDMA_RRC_LTE_DEPRI_FREQ_REQ,
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
  LTE_RRC_TDSCDMA_PSHO_RSP,
  LTE_RRC_TDSCDMA_ABORT_PSHO_RSP,
  TDSCDMA_RRC_LTE_PLMN_SRCH_SUSPEND_IND,
  TDSCDMA_RRC_LTE_GET_PLMN_PRTL_RESULTS_REQ,
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_RLF_REPORT_ENHANCEMENT
  LTE_RRC_UTRA_SERVING_CELL_INFO_REQ ,
#endif  
#endif

  TDSCDMA_RRC_GPS_POS_CELL_INFO_REQ,
  TDSCDMA_RRC_GET_CONFIG_REQ,
  TDSCDMA_RRC_SET_CONFIG_REQ,
  TDSCDMA_RRC_QMI_DRX_LEVEL_REQ, 
};

#define TDSRRC_MSGR_X2T_UMID_CNT (sizeof(tdsrrc_msgr_x2t_umid_list)/sizeof(msgr_umid_type))

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
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */


#ifdef FEATURE_TDSCDMA_64QAM
/*============================================================================
FUNCTION: tdsrrcllc_get_64QAM_status()

DESCRIPTION:


DEPENDENCIES:
  None.

RETURN VALUE:
  None

SIDE EFFECTS:
=============================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

boolean tdsrrcllc_get_64QAM_status
(
   void
)
{
  
  tdsordered_config_type *config_ptr;
  
  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"64QAM:64QAM Status is  %d",config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured);
  return(config_ptr->l1_hsdpa_info.hs_dsch_info.hs_64_qam_configured);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif

void tds_rrc_init_tcb(void)
{
  if (tds_rrc_tcb_ptr == RCINIT_NULL)
  {
    tds_rrc_tcb_ptr = rex_self(); 
    if (tds_rrc_tcb_ptr == RCINIT_NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Lookup for tds_rrc_tcb failed");
      ASSERT(0);
    }
  } 
}

rex_tcb_type* tds_rrc_get_tcb(void)
{
  if (tds_rrc_tcb_ptr == RCINIT_NULL)
  {
      if (rcinit_lookup_rextask("tds_rrc") != rex_self())
      {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRC get tcb init called from other task context");
          ASSERT(0);
      }
      else
      {
         tds_rrc_init_tcb();
      }
  }
  return tds_rrc_tcb_ptr;  
}


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
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

void tdsrlc_rrc_req_to_register_for_data_ind
(
  tdsrlc_ul_register_srvc_type *rlc_lc_id_list
)
{
  uint32 idx;
  tdsrrc_cmd_type *llc_cmd_ptr;                     /* Pointer to the RRC Command */
  
  for(idx =0; idx < rlc_lc_id_list->nchan ; idx++)
  {
    switch(tdsrrclcm_get_ul_rb_id(rlc_lc_id_list->rlc_data[idx].lc_id))
    {
      case CCCH_RADIO_BEARER_ID:
      case DCCH_UM_RADIO_BEARER_ID:
      case DCCH_AM_RADIO_BEARER_ID:
      case DCCH_DT_HIGH_PRI_RADIO_BEARER_ID:
      case DCCH_DT_LOW_PRI_RADIO_BEARER_ID:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore registering SRBs as it as already been registered");         
        /* Since SRBS cant be registered by anyonelse other than RRC, ignore 
         * registering for this request
         */
        return;
                                   
      default:
        break;
    }
  }
  
  /*send CRLC DATA IND REQ to RLC via rrcllc */
  if((llc_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Initialize the LLC command header first */
    tdsl2_ul_req_cmd * llc_cmd_l2_ptr = NULL;
    tdsrrc_get_pointer_l2_req_value(&llc_cmd_ptr->cmd.rrc_llc_req.l2_ul_req, &llc_cmd_l2_ptr);

    /* Initialize the LLC command header first */
    llc_cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    llc_cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_CU;
    llc_cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER2_RLC_UL;

 
    /* Now fill the L2 cmd header */
    llc_cmd_ptr->cmd.rrc_llc_req.l2_ul_cmd.cmd_id = TDSCRLC_UL_REG_FOR_DATA_IND_REQ;
    llc_cmd_l2_ptr->ul_reg_data_ind.block_status_timer=TRUE;

    for(idx =0; idx < rlc_lc_id_list->nchan ; idx++)
    {
      llc_cmd_l2_ptr->ul_reg_data_ind.lc_id[idx] = rlc_lc_id_list->rlc_data[idx].lc_id;
    }
    /* rab_count loop */
    llc_cmd_l2_ptr->ul_reg_data_ind.num_lc= (uint16)rlc_lc_id_list->nchan;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"registering with RLC for %d RBs",rlc_lc_id_list->nchan);
    tdsrrc_put_int_cmd(llc_cmd_ptr); 
  }
  else
  {
    ERR_FATAL("RRC Unable to get cmd buf for RLC cmd",0,0,0);
  }
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
tds_UI_event_info_type tds_UI_event_info;
uint16 tds_UI_event_bit_mask = 0x0000;
#endif
/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*====================================================================
FUNCTION: tdsrrc_translate_n312

DESCRIPTION:
  This function translates the enum for timer N312 into an int16 value.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
static uint16 tdsrrc_translate_n312(tdsrrc_N_312 val)
{
  switch (val)
  {
    case tdsrrc_N_312_s1:
      return 1;
    case tdsrrc_N_312_s50:
      return 50;
    case tdsrrc_N_312_s100:
      return 100;
    case tdsrrc_N_312_s200:
      return 200;
    case tdsrrc_N_312_s400:
      return 400;
    case tdsrrc_N_312_s600:
      return 600;
    case tdsrrc_N_312_s800:
      return 800;
    case tdsrrc_N_312_s1000:
      return 1000;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid N312 = %d",val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
}
/*====================================================================
FUNCTION: tdsrrc_translate_n312_v3a0ext

DESCRIPTION:
  This function translates the enum for timer N312 into an int16 value.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
static uint16 tdsrrc_translate_n312_v3a0ext(tdsrrc_N_312ext val)
{
  switch (val)
  {
    case tdsrrc_N_312ext_s2:
      return 2;
    case tdsrrc_N_312ext_s4:
      return 4;
    case tdsrrc_N_312ext_s10:
      return 10;
    case tdsrrc_N_312ext_s20:
      return 20;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid N312 = %d",val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
}
/*====================================================================
FUNCTION: tdsrrc_translate_n315

DESCRIPTION:
  This function translates the enum for timer N315 into an int16 value.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
static uint16 tdsrrc_translate_n315(tdsrrc_N_315 val)
{
  switch (val)
  {
    case tdsrrc_N_315_s1:
      return 1;
    case tdsrrc_N_315_s50:
      return 50;
    case tdsrrc_N_315_s100:
      return 100;
    case tdsrrc_N_315_s200:
      return 200;
    case tdsrrc_N_315_s400:
      return 400;
    case tdsrrc_N_315_s600:
      return 600;
    case tdsrrc_N_315_s800:
      return 800;
    case tdsrrc_N_315_s1000:
      return 1000;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid N315 = %d",val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
}
/*====================================================================
FUNCTION: tdsrrc_translate_n315_v3a0ext

DESCRIPTION:
  This function translates the enum for timer N315 into an int16 value.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
static uint16 tdsrrc_translate_n315_v3a0ext(tdsrrc_N_315ext val)
{
  switch (val)
  {
    case tdsrrc_N_315ext_s2:
      return 2;
    case tdsrrc_N_315ext_s4:
      return 4;
    case tdsrrc_N_315ext_s10:
      return 10;
    case tdsrrc_N_315ext_s20:
      return 20;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid N315 = %d",val);
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
void tdsrrc_translate_access_class_barred_list(tdsrrc_AccessClassBarredList 
                                             *sib_access_class_barred_list_ptr,
                                            tdsrrc_int_access_class_barred_list_type
                                             *access_class_barred_list_ptr)
{
  uint32 i=0,index=0,n_value=0;

  if(access_class_barred_list_ptr == NULL)
  {
    ERR_FATAL("AC barred list is NULL",0,0,0);
  }

  for (i=0; i<(uint16)TDSRRC_INT_ACCESS_CLASS_MAX_INDEX; i++)
  {
    access_class_barred_list_ptr->barred[i] = FALSE;
  }

  index = 0;
  n_value = sib_access_class_barred_list_ptr->n;
  
  while(n_value != 0)
  {
    /* Check for all access classes */
    if(sib_access_class_barred_list_ptr->elem[index] == tdsrrc_AccessClassBarred_barred)
    {
      access_class_barred_list_ptr->barred[index] = TRUE;
    }
    /* Move to the next access class entry */
    n_value--;
    index++;
  }

} /* tdsrrc_translate_access_class_barred_list */


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
void tdsrrc_translate_gsm_map_plmn_id
(
  tdsrrc_PLMN_Identity  *ext_plmn,      /* Pointer to external PLMN type */
  rrc_plmn_identity_type  *int_plmn  /* Pointer to internal PLMN type */
)
{

  tdsrrc_MCC *mcc_ptr;         /* Point to MCC            */
  tdsrrc_MNC *mnc_ptr;         /* Point to MNC            */
  uint32 count;            /* Local counter           */
  uint32 index=0, n_value=0;

  /* Make sure the internal and external pointers are initialized */
  if( ext_plmn == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"External PLMN not initialized");
    return;
  }
  if( int_plmn == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Internal PLMN not initialized");
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
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCC out of range");
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
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MNC out of range");
      return;
    }
    int_plmn->mnc[count] = (uint8)mnc_ptr->elem[index];
    n_value--;
    index++;
  }
  
}


/*===========================================================================

FUNCTION          tdsrrc_translate_gsm_map_plmn_id_with_optional_mcc


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
void tdsrrc_translate_gsm_map_plmn_id_with_optional_mcc
(
  boolean *is_mcc_valid,
  tdsrrc_PLMN_IdentityWithOptionalMCC_r6  *ext_plmn,      /* Pointer to external PLMN type */
  rrc_plmn_identity_type  *int_plmn  /* Pointer to internal PLMN type */
)
{

  tdsrrc_MCC mcc_ptr;         /* Point to MCC            */
  tdsrrc_MNC mnc_ptr;         /* Point to MNC            */
  uint32 count;            /* Local counter           */
  uint32 n_value=0, n_index=0;

  /* Make sure the internal and external pointers are initialized */
  if( ext_plmn == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"External PLMN not initialized");
    return;
  }
  if( int_plmn == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Internal PLMN not initialized");
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
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"MCC out of range");
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
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"MNC has no digit");
    return;
  }

  if( ext_plmn->mnc.n == 1 )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"MNC has only 1 digit");
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
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MNC out of range");
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
                  
  The Cell_id converted to the internal type tdsrrc_int_cell_id_type

SIDE EFFECTS
                  
  None

===========================================================================*/
tdsrrc_int_cell_id_type tdsrrc_translate_cell_id
(
  tdsrrc_CellIdentity  *ext_cell_id         /* Pointer to external PLMN type */
)
{
  uint32 count;                    /* Local counter       */
  tdsrrc_int_cell_id_type cell_id=0;  /* To save the cell id */
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
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"cell_id longer than 32 bits!");
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
void tdsrrc_translate_gsm_map_imsi 
(
  tdsrrc_IMSI_GSM_MAP  *ext_imsi,/* Pointer to external IMSI type */
  rrc_imsi_gsm_map_type  *int_imsi   /* Pointer to internal IMSI type */
)
{
  uint32 i=0, index = 0;                          /* Local parameter to store the 
                                        index value */
  tdsrrc_IMSI_GSM_MAP  *local_ext_imsi;
                                    /* Local  to external IMSI type */

  /* Assign external pointer to local pointer */
  local_ext_imsi = ext_imsi;

  /* Make sure the internal and external pointers are initialized */
  if( ext_imsi == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"External IMSI not initialized");
    return;
  }
  if( int_imsi == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Internal IMSI not initialized");
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
} /* tdsrrc_translate_gsm_map_imsi */

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
void tdsrrc_initialize_established_rabs( void )
{
  uint32 count; /* Local counter */

  for( count=0; count<MAX_RAB_TO_SETUP; count++ )
  {
    /* Initialize each RAB id with an invalid value to show that
    it's not used currently. */
    tdsrrc_est_rabs.rabs[count].rab_id = TDSRRC_INVALID_RAB_ID;
    tdsrrc_est_rabs.rabs[count].num_rbs_for_rab = 0;
    tdsrrc_est_rabs.rabs[count].rab_type = TDSRRCRB_UNKNOWN_RAB;
  }
  for( count=0; count<MAX_SRB_SETUP; count++ )
  {
    /* For each SRB, initialize the status to the default
    value of STARTED. */
    tdsrrc_est_rabs.srbs[count].srb_status = TDSRRC_RB_STARTED;
  }

  /* Reset the current codec to the default mode */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"reset the AMR codec status");
  tdsrrc_set_current_codec_mode(TDSCDMA_IVOCODER_ID_AMR);
  tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);

  /* All other values will be initialized when RABs are added
  to this variable. */
}

/*===========================================================================

FUNCTION   tdsrrcdata_init_procedure

DESCRIPTION
  Initialize variables defined in tdsrrcdata.c
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcdata_init_procedure( void )
{
  /* This contains the current Codec mode being used	*/
  tdsrrcdata_codec_info.rrc_active_codec = TDSCDMA_IVOCODER_ID_AMR; 

  /* Contains codec info received in the OTA msg, so that it can be sent to MAC in ordered config  */
  tdsrrcdata_codec_info.rrc_ota_codec_mode_status = TDSCDMA_IVOCODER_ID_NONE;

  /* Reset the callback function */
  tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func = NULL;

#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
  tdsrrcdata_codec_info.ordered_codec_valid = FALSE;
  tdsrrcdata_codec_info.is_hho_started = FALSE;
  tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func = NULL;
#endif

}

/*===========================================================================

FUNCTION  RRC_RETURN_LC_INFO_FOR_RAB

DESCRIPTION

  This function returns the logical channel ids for a given RAB.
  The calling function must initialize a structure of type
  tdsrrc_user_plane_lc_info_type and fill in the RAB id before
  passing the pointer to this function. This function will fill
  in the LC ids for uplink and downlink for that RAB to the given
  pointer.
    
DEPENDENCIES

  Currently this is only supported for CS data RABs and PS RABs.
  This function must not be used for any other type of RABs at
  present.
 
RETURN VALUE

  A a value of type rrc_rab_status_e_type. TDSRRC_RAB_NOT_FOUND is returned
  if the given RAB is not a valid RAB that has been setup by RRC. TDSRRC_RAB_FOUND
  is returned if the RAB is valid.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_rab_search_e_type tdsrrc_return_lc_info_for_rab
(
  tdsrrc_user_plane_lc_info_type *ptr
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
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"User-plane ptr invalid!");
    return(TDSRRC_RAB_NOT_FOUND);
  }
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( tdsrrc_est_rabs.rabs[count].rab_id == (ptr->rab_id) )
    {
      /* We have a match. Now fill in the relevant values. */
      match_found = TRUE;

      /* Check if there is only one RB for this RAB. 
      Currently for data calls there is only one RB per
      RAB. */
      if( tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > 1 )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"More than 1 RB for data RAB");
      }
      if( tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > MAX_RB_PER_RAB )
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Too many RBs for data RAB");
        return(TDSRRC_RAB_NOT_FOUND);
      }
      /* However, go ahead and fill in all the required
      data */
      for( count2=0; count2 < (tdsrrc_est_rabs.rabs[count].num_rbs_for_rab) ; count2++)
      {

        /* The logical channel manager keeps track of the logical channels
        assigned to each RB. Call the appropriate function in lcm to get the
        lc ids. We have to pass in the given RB id. There is one function
        for downlink lc id and one for uplink lc id. */
        ptr->dl_lc_id[count2] = 
          tdsrrclcm_get_dl_rlc_lc_id(
            (tdsrrc_RB_Identity)(tdsrrc_est_rabs.rabs[count].rb_for_rab[count2].rb_id)
                                 );

        if( ptr->dl_lc_id[count2] == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"LC id not found for RAB %d",ptr->rab_id);
          lc_id_error = TRUE;
          break; /* Break out of the for loop */
        }

        /* Do the same thing for the uplink lc id. */
        ptr->ul_lc_id[count2] = 
          tdsrrclcm_get_ul_rlc_lc_id(
            (tdsrrc_RB_Identity)(tdsrrc_est_rabs.rabs[count].rb_for_rab[count2].rb_id)
                                 );

        if( ptr->ul_lc_id[count2] == TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"LC id not found for RAB %d",ptr->rab_id);
          lc_id_error = TRUE;
          break; /* Break out of the for loop */
        }
#ifdef FEATURE_SCUDIF_ENHANCEMENT
        #error code not present
#endif
      }

      if( lc_id_error == TRUE )
      {
        return(TDSRRC_RAB_NOT_FOUND);
      }
      else
      {
        /* We got the right LC ids, so fill in the count */
        ptr->num_dl_lc_ids_for_rab = 
          (uint8)tdsrrc_est_rabs.rabs[count].num_rbs_for_rab;
        ptr->num_ul_lc_ids_for_rab = 
          (uint8)tdsrrc_est_rabs.rabs[count].num_rbs_for_rab;
      }
      
      break; /* Out of the outer for loop */
    }/* End if (rab matched) */
  }/* end for */

  if( match_found == FALSE )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"RAB %d not established",ptr->rab_id); 
    return( TDSRRC_RAB_NOT_FOUND );
  }
  else
  {
    return( TDSRRC_RAB_FOUND );
  }
}

#ifdef FEATURE_DUAL_WCDMA

/*===========================================================================

FUNCTION  tdsrrc_return_lc_info_for_rab_sub

DESCRIPTION

  This function returns the logical channel ids for a given RAB.
  The calling function must initialize a structure of type
  tdsrrc_user_plane_lc_info_type and fill in the RAB id before
  passing the pointer to this function. This function will fill
  in the LC ids for uplink and downlink for that RAB to the given
  pointer.
    
DEPENDENCIES

  Currently this is only supported for CS data RABs and PS RABs.
  This function must not be used for any other type of RABs at
  present.
 
RETURN VALUE

  A a value of type rrc_rab_status_e_type. TDSRRC_RAB_NOT_FOUND is returned
  if the given RAB is not a valid RAB that has been setup by RRC. TDSRRC_RAB_FOUND
  is returned if the RAB is valid.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_rab_search_e_type tdsrrc_return_lc_info_for_rab_sub
(
  sys_modem_as_id_e_type as_id,
  tdsrrc_user_plane_lc_info_type *ptr
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return tdsrrc_return_lc_info_for_rab(ptr);
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
rrc_rab_id_type tdsrrc_find_rab_for_rb_in_est_rabs_sub
(
  sys_modem_as_id_e_type as_id,
  uint8 rb_id
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return tdsrrc_find_rab_for_rb_in_est_rabs(rb_id);
}
#endif


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
void tdsrrc_add_rab_to_established_rabs
(
  tdsrrc_est_rab_info_type *ptr /* Pointer to the RAB information */
)
{
  uint32 count; /* Local counter */

  uint32 index = 0; /* Index into the variable ESTABLISHED_RABS    */
  boolean index_found=FALSE; /* Indicates if an index is found */

  /* Loop around to find the first slot that's not used */
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( tdsrrc_est_rabs.rabs[count].rab_id == TDSRRC_INVALID_RAB_ID)
    {
      /* This index is not used, it can be used for this RAB */
      index_found=TRUE;
      index=count;
      break; /* Break out of the for loop */
    }
  }

  if( index_found == FALSE )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Cannot add RAB to ESTABLISHED_RABS,no space!");
    return;
  }

  /* Copy all the relevant data into the variable. */
  tdsrrc_est_rabs.rabs[index].cn_domain = ptr->cn_domain;
  tdsrrc_est_rabs.rabs[index].rab_id = ptr->rab_id;
  tdsrrc_est_rabs.rabs[index].rab_type = ptr->rab_type;  
  tdsrrc_est_rabs.rabs[index].re_est_timer = ptr->re_est_timer;
  tdsrrc_est_rabs.rabs[index].rab_id_is_gsm_map = ptr->rab_id_is_gsm_map;
  tdsrrc_est_rabs.rabs[index].num_rbs_for_rab = ptr->num_rbs_for_rab;


  TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB add id:%d,rbs:%d,idx:%d",
           tdsrrc_est_rabs.rabs[index].rab_id,
           tdsrrc_est_rabs.rabs[index].num_rbs_for_rab,
           index
           );
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB add domain:%d,re-est tmr:%d,gsm_map:%d",
           tdsrrc_est_rabs.rabs[index].cn_domain,
           tdsrrc_est_rabs.rabs[index].re_est_timer,
           tdsrrc_est_rabs.rabs[index].rab_id_is_gsm_map
           );
  if( ptr->num_rbs_for_rab > MAX_RB_PER_RAB )
  {
    /* More RBs than can be mapped to a RAB, just return */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"MAX RBs per RAB count exceeded!");
    return;
  }

  /* Copy all RB related data for this RAB */
  for( count=0; count < MAX_RB_PER_RAB; count++ )
  {
    if( count < ptr->num_rbs_for_rab )
    {
      tdsrrc_est_rabs.rabs[index].rb_for_rab[count].rb_id = ptr->rb_for_rab[count].rb_id;
      tdsrrc_est_rabs.rabs[index].rb_for_rab[count].rb_status 
        = ptr->rb_for_rab[count].rb_status;
      tdsrrc_est_rabs.rabs[index].rb_for_rab[count].subflow 
        = ptr->rb_for_rab[count].subflow;
    }
    else
    {
      /* All other RBs for this RAB are unused */
      tdsrrc_est_rabs.rabs[index].rb_for_rab[count].rb_id = TDSRRC_INVALID_RB_ID;
    }
  }
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
          TDSRRC_RB_NOT_FOUND :rb-id does not exist in the ESTABLISHED_RAB variable.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_rb_search_e_type tdsrrc_find_rb_in_est_rabs
(
  tdsrrc_RB_Identity rb_id
)
{
  uint32 rb_count;
  uint32 rab_count;
  tdsrrc_rb_search_e_type rb_srch = TDSRRC_RB_NOT_PRESENT;
   /* Check whether this RB exists in established_rab db */
  for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
  {
    if (
         ((tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)|| 
           (tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID))
          &&
          ((tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) &&
           (tdsrrc_est_rabs.rabs[rab_count].rab_id !=  TDSRRC_INVALID_RAB_ID))
        )
    {
      /*it is a valid established rab, now compare each rb-id in this established rab with 
      the rb-id received in the message */
      for (rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++)
      {
        if (tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == rb_id)
        {
          rb_srch = TDSRRC_RB_PRESENT;
          break;
        }
      }
    } /*end of if*/
    if (rb_srch == TDSRRC_RB_PRESENT)
    {
      break;
    }
  }/* rab_count loop */
  return rb_srch;
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
rrc_rab_id_type tdsrrc_find_rab_for_rb_in_est_rabs
(
  uint8 rb_id
)
{
  uint32 rb_count;
  uint32 rab_count;
  tdsrrc_rb_search_e_type rb_srch = TDSRRC_RB_NOT_PRESENT;
  rrc_rab_id_type rab_id = TDSRRC_INVALID_RAB_ID;
   /* Check whether this RB exists in established_rab db */
  for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
  {
    if (
         ((tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)|| 
           (tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID))
          &&
          ((tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) &&
           (tdsrrc_est_rabs.rabs[rab_count].rab_id !=  TDSRRC_INVALID_RAB_ID))
        )
    {
      /*it is a valid established rab, now compare each rb-id in this established rab with 
      the rb-id received in the message */
      for (rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++)
      {
        if (tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == rb_id)
        {
          rab_id = tdsrrc_est_rabs.rabs[rab_count].rab_id;
          rb_srch = TDSRRC_RB_PRESENT;
          break;
        }
      }
    } /*end of if*/
    if (rb_srch == TDSRRC_RB_PRESENT)
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
  is present in the established rbs. It returns the corresponding cn_domain

DEPENDENCIES

  None.
 
RETURN VALUE

  rrc_rab_id_type rab_id

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_find_cn_domain_id_for_rb_in_est_rabs
(
  tdsrrc_RB_Identity rb_id,
  rrc_cn_domain_identity_e_type *rrc_cn_domain_id
)
{
  uint32 rb_count=0;
  uint32 rab_count=0;
  
   /* Check whether this RB exists in established_rab db */
  for (rab_count=0; rab_count<MAX_RAB_TO_SETUP; rab_count++)
  {
    if (
         ((tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)|| 
           (tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID))
          &&
          ((tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0) &&
           (tdsrrc_est_rabs.rabs[rab_count].rab_id !=  TDSRRC_INVALID_RAB_ID))
        )
    {
      /* It is a valid established rab, now compare each rb-id in this established rab with 
           the rb-id received in the message */
      for (rb_count=0; rb_count<tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab; rb_count++)
      {
        if (tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id == rb_id)
        {
          if(tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_CS_DOMAIN_CN_ID)
          {
            tdsrrc_est_rabs.rabs[rab_count].cn_domain = RRC_CS_DOMAIN_CN_ID;
            *rrc_cn_domain_id = RRC_CS_DOMAIN_CN_ID;
          }
          else
          {
            tdsrrc_est_rabs.rabs[rab_count].cn_domain = RRC_PS_DOMAIN_CN_ID;
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
  "ESTABLISHED_RABS" variable. If so, it returns TDSRRC_RAB_FOUND.
  Else it returns TDSRRC_RAB_NOT_FOUND.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_rab_search_e_type tdsrrc_find_rab_in_established_rabs
(
  rrc_rab_id_type rab_id /* Id of the received RAB */
)
{
  uint32 count; /* Local Counter */
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( tdsrrc_est_rabs.rabs[count].rab_id == rab_id )
    {
      /* We have a match */
      return( TDSRRC_RAB_FOUND );
    }
  }
  /* If we get here, we don't have the RAB stored */
  return( TDSRRC_RAB_NOT_FOUND );
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
tdsrrc_find_if_voice_rab_exists_in_established_rabs(rrc_rab_id_type rab_id /* Id of the received RAB */)
{
  boolean voice_rab_already_exists = FALSE;
  uint32 count; /* Local Counter */
  
  for(count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if( tdsrrc_est_rabs.rabs[count].rab_id == rab_id && 
        tdsrrc_est_rabs.rabs[count].rab_type == TDSRRCRB_CS_VOICE_RAB)
    {
      /* We have a match */
      voice_rab_already_exists = TRUE;
      break;
    }
  }

  return voice_rab_already_exists;
} /* tdsrrc_find_if_voice_rab_exists_in_established_rabs */


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
tdsrrc_update_rb_info_to_existing_rab(tdsrrc_est_rab_info_type *rbe_ptr)
{
  uint32 count; /* Local Counter */
  uint32 i = 0;
  
  for(count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if( tdsrrc_est_rabs.rabs[count].rab_id == rbe_ptr->rab_id   && 
        tdsrrc_est_rabs.rabs[count].rab_type == rbe_ptr->rab_type)
    {
      /* We have a match */
      for (; i < rbe_ptr->num_rbs_for_rab; i++)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Added rb id %d for rab %d",tdsrrc_est_rabs.rabs[count].rab_id,rbe_ptr->rb_for_rab[i].rb_id);
        tdsrrc_est_rabs.rabs[count].rb_for_rab[tdsrrc_est_rabs.rabs[count].num_rbs_for_rab].rb_id = rbe_ptr->rb_for_rab[i].rb_id;
        tdsrrc_est_rabs.rabs[count].rb_for_rab[tdsrrc_est_rabs.rabs[count].num_rbs_for_rab].rb_status = rbe_ptr->rb_for_rab[i].rb_status;
        tdsrrc_est_rabs.rabs[count].rb_for_rab[tdsrrc_est_rabs.rabs[count].num_rbs_for_rab].subflow = rbe_ptr->rb_for_rab[i].subflow;

        tdsrrc_est_rabs.rabs[count].num_rbs_for_rab++;
      }
      break;
    }
  }
} /* rrc_update_rb_info_to_existing_voice_rab */

/*===========================================================================

FUNCTION   VALIDATE_RB_ID_IN_RECONFIG_LIST

DESCRIPTION

  This function validates the rb_ids in the received RB Reconfig Message.
  If signalling RB id is present it returns TDSRRC_RB_PRESENT. For user plane RB ids
  and SRB4 it checks whether the rb to be reconfigured or affected is present in the 
  established rbs. If not this function returns TDSRRC_RB_NOT_PRESENT.

DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_rb_search_e_type 
          TDSRRC_RB_PRESENT     :rb-id exists in the ESTABLISHED_RAB variable or
                              it is a signalling RB.
          TDSRRC_RB_NOT_PRESENT :rb-id is not a signalling RB and it also
                              does not exist in the ESTABLISHED_RAB variable.

SIDE EFFECTS

  None.

===========================================================================*/

tdsrrc_rb_search_e_type tdsrrc_validate_rb_id_in_reconfig_list(tdsrrc_RB_Identity rb_id)
{
  tdsrrc_rb_search_e_type rb_srch = TDSRRC_RB_NOT_PRESENT;
  /*check whether it is signalling rbid. */
  /*Do not include rb-id0 in the check because it is for CCCH */
  if (((rb_id > CCCH_RADIO_BEARER_ID) && (rb_id <= DCCH_DT_LOW_PRI_RADIO_BEARER_ID)) ||
     TDSRRC_RB_PRESENT == tdsrrc_find_rb_in_est_rabs(rb_id))
  {
    rb_srch = TDSRRC_RB_PRESENT;
  }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  else if(tdsrrcllc_has_config_srb5(TDSCURRENT_CONFIG, rb_id) == TRUE)
  {
    rb_srch = TDSRRC_RB_PRESENT;
  }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
  return rb_srch;
}

/*===========================================================================

FUNCTION   VALIDATE_RB_ID_IN_RELEASE_LIST

DESCRIPTION

  This function validates the rb_ids in the received RB Release Message.
  If the release is for rb-id 0 through 3 then this returns TDSRRC_RB_NOT_PRESENT
  because SRB0-SRB3 should not be in the release list. For user plane RB ids and RB4,
  it checks whether the rb to be released is present in the established rbs. 
  If not this function returns TDSRRC_RB_NOT_PRESENT.

DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrc_rb_search_e_type 
          TDSRRC_RB_PRESENT     :rb-id exists in the ESTABLISHED_RAB variable or
                              it is a signalling RB.
          TDSRRC_RB_NOT_PRESENT :rb-id is signalling RB <= SRB3 or 
                              rb-id is not a signalling RB and it also
                              does not exist in the ESTABLISHED_RAB variable.

SIDE EFFECTS

  None.

===========================================================================*/


tdsrrc_rb_search_e_type tdsrrc_validate_rb_id_in_release_list(tdsrrc_RB_Identity rb_id)
{
  tdsrrc_rb_search_e_type rb_srch = TDSRRC_RB_PRESENT;
  /*SRB0 through SRB3 should not be part of the release list*/
  if ((rb_id == CCCH_RADIO_BEARER_ID) ||
      (rb_id == DCCH_UM_RADIO_BEARER_ID) ||
      (rb_id == DCCH_AM_RADIO_BEARER_ID) ||
      (rb_id == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID))
  {
    rb_srch = TDSRRC_RB_NOT_PRESENT;
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
void tdsrrc_remove_rab_from_established_rabs
(
  rrc_rab_id_type rab_id /* Id of the RAB */
)
{
  uint32 count; /* Local Counter */
  boolean rab_found=FALSE; /* To keep track of whether the
                           rab_id was found */
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( tdsrrc_est_rabs.rabs[count].rab_id == rab_id )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB removed id:%d,rbs:%d,idx:%d",
               tdsrrc_est_rabs.rabs[count].rab_id,
               tdsrrc_est_rabs.rabs[count].num_rbs_for_rab,
               count
               );
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB removed domain:%d,re-est tmr:%d,gsm_map:%d",
               tdsrrc_est_rabs.rabs[count].cn_domain,
               tdsrrc_est_rabs.rabs[count].re_est_timer,
               tdsrrc_est_rabs.rabs[count].rab_id_is_gsm_map
               );
     
      if((tdsrrc_est_rabs.rabs[count].rab_type == TDSRRCRB_CS_VOICE_RAB) &&
           (tdsrrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID))
      {
        /* Deconfigure MVS to deregister RLC queues for AMR Call.*/
        tdsrrc_deconfigure_voice_adapter();
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clear RABs -- Release MVS chan");

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"reset the AMR codec status");
        /* Reset the current codec to the default mode */
        tdsrrc_set_current_codec_mode(TDSCDMA_IVOCODER_ID_AMR);
        tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
      }
      
      /* Make the RAB ID invalid */
      tdsrrc_est_rabs.rabs[count].rab_id = TDSRRC_INVALID_RAB_ID;
      tdsrrc_est_rabs.rabs[count].rab_type = TDSRRCRB_UNKNOWN_RAB;
      tdsrrc_est_rabs.rabs[count].num_rbs_for_rab = 0;
      rab_found=TRUE;
      break;
    }
  }
  if( rab_found == FALSE )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RAB not found in ESTABLISHED_RABS!");
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
void tdsrrc_remove_rb_from_established_rabs
(
  tdsrrc_RB_Identity rb_id,                  /* RB id to be removed */
  tdsrrc_rb_remove_result_type *ptr /* Pointer for holding the results */
)
{
  uint32 count;  /* Local Counter */
  uint32 count1; /* Local Counter */


  /* Look into each RAB that has been established */
  for( count=0; count < MAX_RAB_TO_SETUP; count++ )
  {
    /* If the RAB is valid and it has atleast one RB
    mapped to it, we should try to find a match for this RB */
    if( (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID) &&
        (tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > 0)
      )
    {
      /* This RAB is valid. Look at each RB within the RAB */
      for( count1=0; count1<MAX_RB_PER_RAB; count1++ )
      {
        if( tdsrrc_est_rabs.rabs[count].rb_for_rab[count1].rb_id == rb_id )
        {
          /* Match is found. Remove this RB and decrement
          the number of RBs for this RAB */
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"RB removed rbid:%d, rabid:%d, domain:%d",
                     rb_id,
                     tdsrrc_est_rabs.rabs[count].rab_id,
                     tdsrrc_est_rabs.rabs[count].cn_domain
                   );
          tdsrrc_est_rabs.rabs[count].rb_for_rab[count1].rb_id = TDSRRC_INVALID_RB_ID;

          tdsrrc_est_rabs.rabs[count].num_rbs_for_rab--;
          if( tdsrrc_est_rabs.rabs[count].num_rbs_for_rab == 0 )
          {
            /* This is an AMR RAB. Release the AMR WM. It could be possible */
            /* to attempt to release the AMR WM when the CS-data is         */
            /* releasing. But this should have no harm.                     */
            if ((tdsrrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID) &&
                (tdsrrc_est_rabs.rabs[count].rab_type == TDSRRCRB_CS_VOICE_RAB))
            {
              {            
                /* Deconfigure MVS to deregister RLC queues for AMR Call.*/
                tdsrrc_deconfigure_voice_adapter();
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clear RABs -- Release MVS chan");
              } 
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"reset the AMR codec status");
              /* Reset the current codec to the default mode */
              tdsrrc_set_current_codec_mode(TDSCDMA_IVOCODER_ID_AMR);
              tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);
            }

            /* No more RBs mapped to this RAB. It should be removed.
            First save the rab_id and cn id in the results pointer. Then remove
            the rab id */
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB remved id:%d, index:%d, domain:%d",
                     tdsrrc_est_rabs.rabs[count].rab_id,
                     count,
                     tdsrrc_est_rabs.rabs[count].cn_domain
                     );

            ptr->rab_type = tdsrrc_est_rabs.rabs[count].rab_type;
            ptr->rab_id = tdsrrc_est_rabs.rabs[count].rab_id;
            ptr->cn_domain = tdsrrc_est_rabs.rabs[count].cn_domain;
            tdsrrc_est_rabs.rabs[count].rab_id = TDSRRC_INVALID_RAB_ID;
            tdsrrc_est_rabs.rabs[count].rab_type = TDSRRCRB_UNKNOWN_RAB;            
            ptr->result = TDSRRC_RB_AND_RAB_REMOVED;
            return;
          }
          else
          {
            ptr->result = TDSRRC_RB_REMOVED;
            /*After releasing RBs, if RAB still exists, update rab_id in the return pointer.*/
            ptr->rab_id = tdsrrc_est_rabs.rabs[count].rab_id;
            ptr->rab_type = tdsrrc_est_rabs.rabs[count].rab_type;
            return;
          }
        }
      }
    }
  }
  /* If we get here, we never found that RB id */
  ptr->result = TDSRRC_RB_NOT_FOUND;
}         








/*===========================================================================

FUNCTION   tdsrrc_send_rab_rel_ind_on_ps_release

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
void tdsrrc_send_rab_rel_ind_on_ps_release(void)
{
  int32 count; /* Local counter */
  rabm_cmd_type *rabm_ptr; /* To send the RABMAS_RAB_RELEASE_IND  command */
  
  for( count=0; count < MAX_RAB_TO_SETUP; count++ )
  {
    if( (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID) &&
          (tdsrrc_est_rabs.rabs[count].cn_domain == RRC_PS_DOMAIN_CN_ID))
    {
      if ((rabm_ptr = rabm_get_cmd_buf()) == NULL)
      {
        ERR_FATAL("Can't send RABM cmd - out of memory!",0,0,0);
      }
      rabm_ptr->header.cmd_id = RABMAS_RAB_RELEASE_IND;
      rabm_ptr->header.message_set = MS_RABM_RRC;
      rabm_ptr->cmd.rrc_release_ind.rabid = (rabid_T)(tdsrrc_est_rabs.rabs[count].rab_id);
  #ifdef FEATURE_DUAL_SIM
      rabm_ptr->cmd.rrc_release_ind.as_id = tdsrrc_get_as_id();
  #endif
      /* Put the command on RABM queue */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending RABMAS_RAB_RELEASE_IND to RABM rabid %d",tdsrrc_est_rabs.rabs[count].rab_id);
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
void tdsrrc_clear_established_rabs( void )
{
  uint32 count; /* Local counter */
   
  /* Make all RAB Ids invalid to indicate the indices
  are no longer used. */
  for( count=0; count < MAX_RAB_TO_SETUP; count++ )
  {
    if( (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID) &&
        (tdsrrc_est_rabs.rabs[count].cn_domain == RRC_CS_DOMAIN_CN_ID)  &&
        (tdsrrc_est_rabs.rabs[count].rab_type == TDSRRCRB_CS_VOICE_RAB)
      )
    {
      /* Deconfigure MVS to deregister RLC queues for AMR Call.*/
      tdsrrc_deconfigure_voice_adapter();
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clear RABs -- Release MVS chan");

     /* Reset the current codec to the default mode */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"reset the AMR codec status");
      tdsrrc_set_current_codec_mode(TDSCDMA_IVOCODER_ID_AMR);
      tdsrrc_set_ordered_codec_mode(TDSCDMA_IVOCODER_ID_NONE);

    }
    tdsrrc_est_rabs.rabs[count].rab_id = TDSRRC_INVALID_RAB_ID;
    tdsrrc_est_rabs.rabs[count].num_rbs_for_rab = 0;
    tdsrrc_est_rabs.rabs[count].rab_type = TDSRRCRB_UNKNOWN_RAB;
  }
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

void tdsrrc_translate_urnti
(
  struct tdsrrc_U_RNTI  *ext_urnti,     /* Pointer to external U-RNTI type */
  tdsrrc_int_u_rnti_type  *int_urnti    /* Pointer to internal U-RNTI type */
)

{
  uint32 temp_srnti;                 /* Local parameter to store the 
                                        S-RNTI value */
  /* Make sure the internal and external pointers are initialized */
  if( ext_urnti == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"External URNTI not initialized");
    return;
  }
  if( int_urnti == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Internal URNTI not initialized");
    return;
  }

  if(ext_urnti->srnc_Identity.numbits != 12 )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received SRNC Identity Length: %d is invalid",ext_urnti->srnc_Identity.numbits);
  }
  if(ext_urnti->s_RNTI.numbits != 20 )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received S_RNTI Length: %d is invalid",ext_urnti->s_RNTI.numbits);
  }
  /* Convert 12 bit SRNC identify into internal format */
  *int_urnti = *(ext_urnti->srnc_Identity.data) << 8;
  *int_urnti |= (tdsrrc_int_u_rnti_type)*(ext_urnti->srnc_Identity.data + 1);

  /* Move the 12 bit SRNC Identity value to the MSB side */
  *int_urnti = *int_urnti << 16;

  /* Make sure the remaining bits of the int_urnti are clear */
  *int_urnti &= 0xFFF00000;

  /* Convert 20 bit S-RNTI value into internal format */
  temp_srnti = *(ext_urnti->s_RNTI.data) << 8;
  temp_srnti |= (uint32)*(ext_urnti->s_RNTI.data + 1);
  temp_srnti = temp_srnti << 8;
  temp_srnti |= (uint32)*(ext_urnti->s_RNTI.data + 2);

  /* Get the SRNTI into the lower 20 bits */
  temp_srnti = temp_srnti >> 4;

  /* Append SRNC Identity and S-RNTI to build U-RNTI */
  *int_urnti = *int_urnti | temp_srnti;

}

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
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
void tdsrrc_translate_urnti_short
(
  struct tdsrrc_U_RNTI_Short  *ext_urnti,   /* Pointer to external U-RNTI type */
  tdsrrc_int_u_rnti_type      *int_urnti    /* Pointer to internal U-RNTI type */
)
{
  struct tdsrrc_U_RNTI urnti;
  uint32 srnti;


  /* We need space allocated to hold the expanded 20-bit version of the S-RNTI
     that's about to be passed to the tdsrrc_translate_urnti() function.  That's
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
  srnti |= (uint32)*(ext_urnti->s_RNTI_2.data + 1);
  /* Move over 6 bits so we have 10 bits of zero-padding at the top instead
     of 16. */
  srnti <<= 6;
  *(urnti.s_RNTI.data) = (uint8)((srnti & 0xff000000) >> 24);
  *(urnti.s_RNTI.data + 1) = (uint8)((srnti & 0x00ff0000) >> 16);
  *(urnti.s_RNTI.data + 2) = (uint8)((srnti & 0x0000ff00) >> 8);
  urnti.srnc_Identity.numbits = ext_urnti->srnc_Identity.numbits;
  memscpy(urnti.srnc_Identity.data, sizeof(urnti.srnc_Identity.data), ext_urnti->srnc_Identity.data,sizeof(ext_urnti->srnc_Identity.data));
  tdsrrc_translate_urnti(&urnti, int_urnti);
}
#endif /* FEATURE_INTERRAT_HANDOVER_G2TD */

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
void tdsrrc_translate_crnti
(
  ASN1BitStr32  *ext_crnti,          /* Pointer to external C-RNTI type */
  tdsrrc_int_c_rnti_type  *int_crnti    /* Pointer to internal C-RNTI type */
)

{
                               
  /* Make sure the internal and external pointers are initialized */
  if( ext_crnti == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"External CRNTI not initialized");
    return;
  }
  if( int_crnti == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Internal CRNTI not initialized");
    return;
  }

  if(ext_crnti->numbits!= 16 )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received C-RNTI Length: %d is invalid",ext_crnti->numbits);
  }

  /* Convert 16 bit C-RNTI into internal format */
  *int_crnti = *(ext_crnti->data) << 8;
  *int_crnti |= (tdsrrc_int_c_rnti_type)*(ext_crnti->data+ 1);

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
void tdsrrc_translate_ura_id
(
  ASN1BitStr32  *ext_ura_id,          /* Pointer to external C-RNTI type */
  tdsrrc_int_ura_id_type  *int_ura_id    /* Pointer to internal C-RNTI type */
)

{
                               
  /* Make sure the internal and external pointers are initialized */
  if( ext_ura_id == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"External URA_ID not initialized");
    return;
  }
  if( int_ura_id == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Internal URA_ID not initialized");
    return;
  }

  if(ext_ura_id->numbits!= 16 )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received URA_ID Length: %d is invalid",ext_ura_id->data);
  }

  /* Convert 16 bit URA_ID into internal format */
  *int_ura_id = *(ext_ura_id->data) << 8;
  *int_ura_id |= (tdsrrc_int_ura_id_type)*(ext_ura_id->data + 1);

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
uint64 tdsrrc_convert_gsm_map_imsi_into_int_value 
(
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
} /* tdsrrc_convert_gsm_map_imsi_into_int_value() */
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
uint64 tdsrrc_convert_ptmsi_map_into_int_value
(
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
} /* tdsrrc_convert_ptmsi_map_into_int_value() */
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
uint64 tdsrrc_convert_tmsi_map_into_int_value 
(
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
} /* tdsrrc_convert_tmsi_map_into_int_value() */
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
uint64 tdsrrc_convert_imei_into_int_value 
(
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
} /* tdsrrc_convert_imei_into_int_value() */

/*===========================================================================

FUNCTION tdsrrc_convert_initial_ue_identity_into_int_value

DESCRIPTION
  This function converts Initial UE Identity from internal format to integer value.


DEPENDENCIES
  None

RETURN VALUE
  uint32: returns an integer value of the Initial UE dentity

SIDE EFFECTS
  None
===========================================================================*/
uint64 tdsrrc_convert_initial_ue_identity_into_int_value
(
  void
)
{

  uint64 value;                                /* Local parameter to store the 
                                                  IMSI value */    
               
  switch(tdsrrc_initial_ue_identity_choice)
  {

    case RRC_IMSI_UE_ID:
      /* Convert IMSI value to the integer value */
      value = tdsrrc_convert_gsm_map_imsi_into_int_value(
                tdsrrc_initial_ue_identity.imsi);
      break;
 
    case RRC_TMSI_LAI_UE_ID:
      /* Convert tmsi_lai value to the integer value */
      value = tdsrrc_convert_tmsi_map_into_int_value(
                tdsrrc_initial_ue_identity.tmsi_lai.tmsi);      
      break;

    case RRC_PTMSI_RAI_UE_ID:
      /* Convert ptmsi_lai value to the integer value */
      value = tdsrrc_convert_ptmsi_map_into_int_value(
                tdsrrc_initial_ue_identity.ptmsi_rai.ptmsi);      
      break;

    case RRC_IMEI_UE_ID:
      /* Convert IMEI value to the integer value */
      value = tdsrrc_convert_imei_into_int_value(tdsrrc_initial_ue_identity.imei);      
      break; 

    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unknown Intital UE Identity is sent");
      value = 0;
      break;
  }      
  return (value);
} /* tdsrrc_convert_initial_ue_identity_into_int_value */

/*===========================================================================

FUNCTION tdsrrc_log_map_protocol_cause

DESCRIPTION
  This function maps the asn1 cause to an internal macros

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
uint8 tdsrrc_log_map_protocol_cause
(
  tdsrrc_ProtocolErrorCause prot_err_cause
)
{
#if(!defined(T_WINNT))

uint8 protocol_error_cause = TDSRRCLOG_ASN1_ENC_ERR;

  switch (prot_err_cause)
  {
    case tdsrrc_ProtocolErrorCause_asn1_ViolationOrEncodingError:
      protocol_error_cause = TDSRRCLOG_ASN1_ENC_ERR;
      break;

    case tdsrrc_ProtocolErrorCause_messageTypeNonexistent:
      protocol_error_cause = TDSRRCLOG_BAD_MSG_TYPE;
      break;

    case tdsrrc_ProtocolErrorCause_messageNotCompatibleWithReceiverState:
      protocol_error_cause = TDSRRCLOG_INCOMPAT_RX_STATE;
      break;

    case tdsrrc_ProtocolErrorCause_ie_ValueNotComprehended:
      protocol_error_cause = TDSRRCLOG_BAD_IE;
      break;

    case tdsrrc_ProtocolErrorCause_informationElementMissing:
      protocol_error_cause = TDSRRCLOG_COND_IE_ERROR;
      break;

    case tdsrrc_ProtocolErrorCause_messageExtensionNotComprehended:
      protocol_error_cause = TDSRRCLOG_BAD_MSG_EXTENSION;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"unknown prot_err_cause %d",prot_err_cause);
      break;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC prot_err_cause %d",prot_err_cause);
  return (protocol_error_cause);

#else  
#error code not present
#endif 
}/*rrc_log_map_protocol_cause*/

/*===========================================================================

FUNCTION tdsrrc_log_map_procedure_name

DESCRIPTION
  This function maps RRC procedure name to Apex procedure name

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
uint8 tdsrrc_log_map_procedure_name
(
  uint8 rrc_procedure            /* The RRC procedure */
)
{
#if(!defined(T_WINNT))

  tdsrrc_proc_e_type rrc_proc = TDSRRC_PROCEDURE_NONE;           /* RRC procedure */

  switch((tdsrrc_proc_e_type)rrc_procedure)
  {

    case TDSRRC_PROCEDURE_CSP:
      rrc_proc = TDSRRC_PROCEDURE_NONE;
      break;

    case TDSRRC_PROCEDURE_SIB:
      rrc_proc = TDSRRC_PROCEDURE_CSP;
      break;

    case TDSRRC_PROCEDURE_PG2:
      rrc_proc = TDSRRC_PROCEDURE_CCM;
      break;

    case TDSRRC_PROCEDURE_MCMR:
      rrc_proc = TDSRRC_PROCEDURE_SIB;
      break;

    case TDSRRC_PROCEDURE_RCE:
      rrc_proc = TDSRRC_PROCEDURE_PG1;
      break;

    case TDSRRC_PROCEDURE_RCR:
      rrc_proc = TDSRRC_PROCEDURE_RCE;
      break;

    case TDSRRC_PROCEDURE_UECI:
      rrc_proc = TDSRRC_PROCEDURE_RCR;
      break;

    case TDSRRC_PROCEDURE_UECE:
      rrc_proc = TDSRRC_PROCEDURE_RCRE;
      break;

    case TDSRRC_PROCEDURE_IDT:
      rrc_proc = TDSRRC_PROCEDURE_UECI;
      break;

    case TDSRRC_PROCEDURE_UDT:
      rrc_proc = TDSRRC_PROCEDURE_UECE;
      break;

    case TDSRRC_PROCEDURE_DDT:
      rrc_proc = TDSRRC_PROCEDURE_IDT;
      break;

    case TDSRRC_PROCEDURE_SCR:
      rrc_proc = TDSRRC_PROCEDURE_UDT;
      break;

    case TDSRRC_PROCEDURE_SCRR:
      rrc_proc = TDSRRC_PROCEDURE_DDT;
      break;

    case TDSRRC_PROCEDURE_CC:
      rrc_proc = TDSRRC_PROCEDURE_PG2;
      break;

    case TDSRRC_PROCEDURE_RBE:
      rrc_proc = TDSRRC_PROCEDURE_SMC;
      break;

    case TDSRRC_PROCEDURE_RBRC:
      rrc_proc = TDSRRC_PROCEDURE_SCR;
      break;

    case TDSRRC_PROCEDURE_RBR:
      rrc_proc = TDSRRC_PROCEDURE_SCRR;
      break;

    case TDSRRC_PROCEDURE_TCR:
      rrc_proc = TDSRRC_PROCEDURE_CC;
      break;

    case TDSRRC_PROCEDURE_PCR:
      rrc_proc = TDSRRC_PROCEDURE_RBE;
      break;

    case TDSRRC_PROCEDURE_TFC:
      rrc_proc = TDSRRC_PROCEDURE_RBRC;
      break;

    case TDSRRC_PROCEDURE_CU:
      rrc_proc = TDSRRC_PROCEDURE_RBR;
      break;

    case TDSRRC_PROCEDURE_UU:
      rrc_proc = TDSRRC_PROCEDURE_TCR;
      break;

    case TDSRRC_PROCEDURE_UMI:
      rrc_proc = TDSRRC_PROCEDURE_TFC;
      break;

   /* case TDSRRC_PROCEDURE_ASU:
      rrc_proc = TDSRRC_PROCEDURE_PCR;
      break;*/

    case TDSRRC_PROCEDURE_ISHFU:
      rrc_proc = TDSRRC_PROCEDURE_CU;
      break;

    case TDSRRC_PROCEDURE_ISHTU:
      rrc_proc = TDSRRC_PROCEDURE_UU;
      break;

    case TDSRRC_PROCEDURE_ISCFU:
      rrc_proc = TDSRRC_PROCEDURE_UMI;
      break;

    case TDSRRC_PROCEDURE_ISCTU:
      rrc_proc = TDSRRC_PROCEDURE_ISCTU;
      break;

    case TDSRRC_PROCEDURE_PG1:
      rrc_proc = TDSRRC_PROCEDURE_ISHTU;
      break;

    case TDSRRC_PROCEDURE_SMC:
      rrc_proc = TDSRRC_PROCEDURE_ISHFU;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Protocol error logging is not supported for %d",rrc_procedure);
      break;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC procedure is %d",rrc_procedure);

  return (uint8)rrc_proc;
#else  
#error code not present
#endif 
}/*rrc_log_map_procedure_name*/
/*===========================================================================

FUNCTION tdsrrc_log_protocol_error

DESCRIPTION
  This function logs the protocol error

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_log_protocol_error
(
  uint8 rrc_state,                /* The RRC state */
  uint8 rrc_procedure,            /* The RRC procedure */
  uint8 failure_cause,            /* The failure cause */
  uint8 protocol_error_cause     /* The protocol error cause */  
)
{
#if(!defined(T_WINNT))
  uint32 log_record_length;                     /* Local variable to store Log 
                                                   Record Length */
  tdscdma_rrc_protocol_errors_log_packet_type *log_record_ptr;  /* Local variable to store Log
                                                   record */

  /*Calculate the record length.  the 4 is added because the protocol_error_type
    takes four uint8 variables */
  log_record_length = 
    FPOS(tdscdma_rrc_protocol_errors_log_packet_type, rrc_state) + 4;

  /*Allocate memory for Log Record. */
  log_record_ptr = (tdscdma_rrc_protocol_errors_log_packet_type *)log_alloc( TDSCDMA_RRC_PROTOCOL_ERRORS_LOG_PACKET, log_record_length);
  
  /* Make sure memory allocation is successful before to proceed with 
     logging */
  if(log_record_ptr != NULL)
  {
    log_record_ptr->version              = TDSRRC_LOG_VERSION;
    log_record_ptr->rrc_state            = rrc_state;
    log_record_ptr->rrc_procedure        = tdsrrc_log_map_procedure_name(rrc_procedure);
    log_record_ptr->failure_cause        = failure_cause;
    log_record_ptr->protocol_error_cause = protocol_error_cause;
        
    TDSRRC_MSG0(MSG_LEGACY_MED,"Logging TDSCDMA_RRC_PROTOCOL_ERRORS_LOG_PACKET");

    /*commit the log record */
    log_commit(log_record_ptr);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"diag is not enabled");
  }

#else  
#error code not present
#endif 
}/*tdsrrc_log_protocol_error*/

/*===========================================================================

FUNCTION tdsrrc_log_acq_info

DESCRIPTION
  This function logs the acq info

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_log_acq_info
(
  int16                    freq_index,
  uint8                    num_freqs,
  uint8                    acq_mode
)
{
  uint32 log_record_length;                     /* Local variable to store Log 
                                                   Record Length */
  uint8 i = 0;
  tdscdma_rrc_acq_info_log_packet_type *log_record_ptr;  /* Local variable to store Log*/
  uint8 temp_ptr[TDSL1_MAX_SCAN_FREQ+1];
  
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"logging ACQ INFO, : freq_index %d,num_freqs %d ",freq_index,num_freqs);
  /* If freq_index>0, means this is not the first time to send ACQ_REQ during this ACQ cycle*/
  if (freq_index > 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"Do not send log packet due to it is not the first ACQ");
    return;
  }

  log_record_length = 
    FPOS(tdscdma_rrc_acq_info_log_packet_type, num_freq) + 1 + (num_freqs-freq_index)*2 +
    (num_freqs-freq_index) + 1;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"logging ACQ INFO alloc length %d",log_record_length);
  /*Allocate memory for Log Record. */
  log_record_ptr = (tdscdma_rrc_acq_info_log_packet_type *)log_alloc( TDSCDMA_RRC_ACQ_INFO_LOG_PACKET, log_record_length);
  
  /* Make sure memory allocation is successful before to proceed with 
     logging */
  if(log_record_ptr != NULL)
  {
    log_record_ptr->version              = TDSRRC_LOG_VERSION;
    for (i = freq_index; i < num_freqs; i++)
    {
       log_record_ptr->uarfcn[i-freq_index] = tdsrrc_csp_int_data.acq_freq_list.freq_list[i];
    }
    log_record_ptr->num_freq             = num_freqs - freq_index;
    /*set source[] and acq_mode to an array temp_ptr[], then append the content to log_record_ptr*/
    for (i = freq_index; i < num_freqs; i++)
    {
       temp_ptr[i-freq_index] = (uint8)tdsrrc_csp_int_data.acq_freq_list.source[i];
    }
    temp_ptr[i-freq_index] = (uint8)acq_mode;
    // for KW only
    if (num_freqs-freq_index < TDSL1_MAX_SCAN_FREQ) 
    {
    memscpy((&(log_record_ptr->uarfcn[0])+(num_freqs-freq_index)), (i-freq_index+1), temp_ptr,(i-freq_index+1));
    }
   
    TDSRRC_MSG0(MSG_LEGACY_MED,"Logging TDSCDMA_RRC_ACQ_INFO_LOG_PACKET");

    /*commit the log record */
    log_commit(log_record_ptr);
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"diag is not enabled");
  }
}
/*===========================================================================

FUNCTION tdsrrc_log_dsds_lta_info

DESCRIPTION
  This function logs the dsds lta info


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_log_dsds_lta_info
(
  void
)
{
  uint32 log_record_length;                     /* Local variable to store Log 
                                                   Record Length */
  tdscdma_dsds_rrc_lta_log_packet_type *log_record_ptr;  /* Local variable to store Log*/
                                                   
  log_record_length = 
    FPOS(tdscdma_dsds_rrc_lta_log_packet_type, lta_end_time) + 8;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"logging ACQ INFO alloc length %d",log_record_length);
  log_record_ptr = (tdscdma_dsds_rrc_lta_log_packet_type *)log_alloc( TDSCDMA_RRC_DSDS_LTA_LOG_PACKET, log_record_length);
  if(log_record_ptr != NULL)
  {
    lta_log_record.version              = 22;  
    lta_log_record.lta_start_time       = lta_start_time;
    lta_log_record.lta_end_time         = lta_end_time;
    lta_log_record.rrc_state            = (uint8)ta_state;
    lta_log_record.lta_duration_in_clk  = lta_end_time - lta_start_time;
    if (lta_log_record.lta_duration_in_clk < 0)
    {
       lta_log_record.lta_duration_in_clk += 0xFFFFFFFFFFFFFFFF;
    }
    lta_log_record.lta_duration_in_ms = (uint32)(TIMETICK_MS_FROM_SCLK(lta_log_record.lta_duration_in_clk));
    TDSRRC_MSG0(MSG_LEGACY_MED,"Logging TDSCDMA_DSDS_RRC_LTA_LOG_PACKET");
    memscpy((void *)(&(log_record_ptr->version)), (sizeof(tdscdma_dsds_rrc_lta_log_packet_type)-sizeof(log_hdr_type)),
	    (void *)(&(lta_log_record.version)), (sizeof(tdscdma_dsds_rrc_lta_log_packet_type)-sizeof(log_hdr_type)));
    /*commit the log record */
    log_commit(log_record_ptr);

  }
  else
  {
  
    TDSRRC_MSG0(MSG_LEGACY_MED,"diag is not enabled");
  } 

}
/*===========================================================================

FUNCTION  tdsrrc_event_log_dsds_ta_block

DESCRIPTION

  This show the event that LTA/QTA is block

DEPENDENCIES
  None.
 
RETURN VALUE

  Lock status.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_event_log_dsds_ta_block
(
  void
)
{
   tdsrrclog_ta_block_event_type ta_block_event;
   ta_block_event.ta_type = (uint8)ta_type;
   ta_block_event.block_reason = (uint8)ta_block_reason;
   event_report_payload(EVENT_TDSCDMA_RRC_DSDS_TA_BLOCK,
                       sizeof(ta_block_event),
                       (void *)&ta_block_event);
   TDSRRC_MSG2(MSG_LEGACY_HIGH,"DSDS TA BLOCK: ta_type = %d,block reason = %d",ta_block_event.ta_type,ta_block_event.block_reason);
}
/*===========================================================================

FUNCTION    tdsrrc_get_dl_ccch_message_type 

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
uint8 tdsrrc_get_dl_ccch_message_type(
  uint8 *dl_sdu,                            /* Pointer received byte stream */
  uint32 dl_sdu_length                      /* Length of SDU received */
)
{      
  
  uint8 msg_type = 0xFF;                  /* Stores type of DL CCCH message  
                                             and initialized to none. */ 
  uint16 message_type_pos;                /* Local Variable to store message 
                                             type position */                                 
  
  /* Check whether Integrity Check info is present */
  if(0x00 == b_unpackb(dl_sdu, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (dl_sdu_length * 8) < 
        (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE) )
    {
      return (msg_type);
    }
    message_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    /*Peek into byte stream and get the message choice */
    msg_type = 
      b_unpackb(dl_sdu, message_type_pos,
                TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE);
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if( 0x01 == b_unpackb(dl_sdu, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (dl_sdu_length * 8) < 
        (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
         TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE) )
    {
      return (msg_type);
    }
    message_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO;

    /*Peek into byte stream and get the message choice */
     msg_type =
      b_unpackb(dl_sdu, message_type_pos,
                TDSNUM_OF_BITS_FOR_DL_CCCH_MSG_TYPE);
  }
  else
  {
   
    ERR_FATAL("Invalid Integrity Check Info Present %d",msg_type,0,0);
  }
  /* Return value is incremented by 1 because OSS uses choice 1 for the first choice. 
     Actually choice is 0 while peeking into the message */
  return(msg_type + 1);
} /* end function tdsrrc_get_dl_ccch_message_type */
/*===========================================================================

FUNCTION    tdsrrc_get_dl_dcch_message_type 

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
uint8 tdsrrc_get_dl_dcch_message_type(
  uint8 *dl_sdu,                            /* Pointer received byte stream */
  uint32 dl_sdu_length                      /* Length of SDU received */
)
{      
  
  uint8 msg_type = 0xFF;                  /* Stores type of DL DCCH message  
                                             and initialized to none. */

  uint16 message_type_pos;                /* Local Variable to store message 
                                             type position */      
                                    
  /* Check whether Integrity Check info is present */
  if(0x00 == b_unpackb(dl_sdu, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (dl_sdu_length * 8) < 
        (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         TDSNUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE) )
    {
      return (msg_type);
    }

    message_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    /*Peek into byte stream and get the message choice */
    msg_type = 
      b_unpackb(dl_sdu, message_type_pos,
                TDSNUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE);
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if( 0x01 == b_unpackb(dl_sdu, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( ( dl_sdu_length * 8) < 
        (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
         TDSNUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE) )
    {
      return (msg_type);
    }

    message_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT+
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO;
    /*Peek into byte stream and get the message choice */
     msg_type = 
      b_unpackb(dl_sdu, message_type_pos,
                TDSNUM_OF_BITS_FOR_DL_DCCH_MSG_TYPE);
  }
  else
  {
   
    ERR_FATAL("Invalid Integrity Check Info Present %d",msg_type,0,0);
  }
  /* Return value is incremented by 1 because OSS uses choice 1 for the first choice. 
     Actually choice is 0 while peeking into the message */
  return(msg_type + 1);
} /* end function tdsrrc_get_dl_dcch_message_type */

/*===========================================================================

FUNCTION    tdsrrc_get_ul_ccch_message_type 

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
uint8 tdsrrc_get_ul_ccch_message_type(
  uint8 *ul_sdu,                            /* Pointer received byte stream */
  uint32 ul_sdu_length                      /* Length of SDU received */
)
{      
  
  uint8 msg_type = 0xFF;                  /* Stores type of UL CCCH message  
                                             and initialized to none. */ 
  uint16 message_type_pos;                /* Local Variable to store message 
                                             type position */                                 
  
  /* Check whether Integrity Check info is present */
  if(0x00 == b_unpackb(ul_sdu, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (ul_sdu_length * 8) < 
        (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         TDSNUM_OF_BITS_FOR_UL_CCCH_MSG_TYPE) )
    {
      return (msg_type);
    }
    message_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    /*Peek into byte stream and get the message choice */
    msg_type = 
      b_unpackb(ul_sdu, message_type_pos,
                TDSNUM_OF_BITS_FOR_UL_CCCH_MSG_TYPE);
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if( 0x01 == b_unpackb(ul_sdu, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (ul_sdu_length * 8) < 
        (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
         TDSNUM_OF_BITS_FOR_UL_CCCH_MSG_TYPE) )
    {
      return (msg_type);
    }
    message_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO;

    /*Peek into byte stream and get the message choice */
     msg_type =
      b_unpackb(ul_sdu, message_type_pos,
                TDSNUM_OF_BITS_FOR_UL_CCCH_MSG_TYPE);
  }
  else
  {
   
    ERR_FATAL("Invalid Integrity Check Info Present %d",msg_type,0,0);
  }
  /* Return value is incremented by 1 because OSS uses choice 1 for the first choice. 
     Actually choice is 0 while peeking into the message */
  return(msg_type + 1);
} /* end function tdsrrc_get_ul_ccch_message_type */
/*===========================================================================

FUNCTION    tdsrrc_get_ul_dcch_message_type 

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
uint8 tdsrrc_get_ul_dcch_message_type(
  uint8 *ul_sdu,                            /* Pointer received byte stream */
  uint32 ul_sdu_length                      /* Length of SDU received */
)
{      
  
  uint8 msg_type = 0xFF;                  /* Stores type of UL DCCH message  
                                             and initialized to none. */

  uint16 message_type_pos;                /* Local Variable to store message 
                                             type position */      
                                    
  /* Check whether Integrity Check info is present */
  if(0x00 == b_unpackb(ul_sdu, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( (ul_sdu_length * 8) < 
        (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         TDSNUM_OF_BITS_FOR_UL_DCCH_MSG_TYPE) )
    {
      return (msg_type);
    }

    message_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT;

    /*Peek into byte stream and get the message choice */
    msg_type = 
      b_unpackb(ul_sdu, message_type_pos,
                TDSNUM_OF_BITS_FOR_UL_DCCH_MSG_TYPE);
  }
  /* The integrity Check Info is present, Skip 36 bits for message type */
  else if( 0x01 == b_unpackb(ul_sdu, TDSINTEGRITY_CHECK_INFO_PRESENT_POS, 
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT) )
  {
    /* Check if there are enough bits to peek into the message type */
    if( ( ul_sdu_length * 8) < 
        (TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT +
         TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO +
         TDSNUM_OF_BITS_FOR_UL_DCCH_MSG_TYPE) )
    {
      return (msg_type);
    }

    message_type_pos = TDSINTEGRITY_CHECK_INFO_PRESENT_POS +
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO_PRESENT+
                       TDSNUM_OF_BITS_FOR_INTEGRITY_CHECK_INFO;
    /*Peek into byte stream and get the message choice */
     msg_type = 
      b_unpackb(ul_sdu, message_type_pos,
                TDSNUM_OF_BITS_FOR_UL_DCCH_MSG_TYPE);
  }
  else
  {
   
    ERR_FATAL("Invalid Integrity Check Info Present %d",msg_type,0,0);
  }
  /* Return value is incremented by 1 because OSS uses choice 1 for the first choice. 
     Actually choice is 0 while peeking into the message */
  return(msg_type + 1);
} /* end function tdsrrc_get_ul_dcch_message_type */


#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
/*===========================================================================

FUNCTION  tdsrrc_update_periodic_tds_UI_events_info

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
static void tdsrrc_update_periodic_tds_UI_events_info
(
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  tdsrrc_cmd_type *cmd_ptr 
#else
  void
#endif
)
{
  tdsl1_serving_cell_power_info_type l1_serving_cell_power_info;
  tdsl1_active_set_info_type l1_active_set_info;
  tdsl1_monitored_set_info_type l1_monitored_set_info;
  boolean call_cm_api = FALSE;
  tdsrrcllc_oc_set_status_e_type oc_status = tdsrrcllc_get_ordered_config_status();
  tdsrrcllc_toc_usage_e_type toc_usage = tdsrrcllc_get_toc_usage();
  tdsrrc_state_e_type rrc_current_state = tdsrrc_get_state();

  if((RRC_IS_TDS_UI_EVENT_ENABLED(SYS_WCDMA_SERVING_CELL_POWER_INFO_BIT_MASK)) &&
    (rrc_current_state != TDSRRC_STATE_CELL_DCH))
  {
    tds_serving_cell_power_info_type *serving_cell_power_info ;
    serving_cell_power_info = &(tds_UI_event_info.wcdma_event_info.wcdma_serving_cell_power_info);
    if (tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_CSP) == TDSRRC_CAMPED_ON)
    { 
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
      /* Copy the Signalling mesaage payload to the log record */
      memscpy((void *)&(l1_serving_cell_power_info), 
             sizeof(tdsl1_serving_cell_power_info_type),
             (void *)(cmd_ptr->cmd.modem_stat_rsp.l1_serving_cell_power_info),
             sizeof(tdsl1_serving_cell_power_info_type));
#else
      tds_l1_get_serving_cell_power(&l1_serving_cell_power_info);
#endif
      if(l1_serving_cell_power_info.is_valid)
      {
        serving_cell_power_info->cpid = l1_serving_cell_power_info.cell_info.cpid;
        serving_cell_power_info->rscp = l1_serving_cell_power_info.cell_info.rscp;
        serving_cell_power_info->ecio = l1_serving_cell_power_info.cell_info.ecio;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"FTD_DBG: Serving cell info invalid from L1.");
      }
    }
    else
    {
      serving_cell_power_info->cpid = 0xFF; /*TDSINVALID*/
      serving_cell_power_info->rscp = -121;
      serving_cell_power_info->ecio = -31;
    }
    call_cm_api = TRUE;
    tds_UI_event_info.bitmask |=SYS_WCDMA_SERVING_CELL_POWER_INFO_BIT_MASK;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"FTD_DBG:Serving cell power info bitmask set,bit_mask %x ",tds_UI_event_info.bitmask);

  }

  if((RRC_IS_TDS_UI_EVENT_ENABLED(SYS_WCDMA_ACTIVE_SET_INFO_BIT_MASK)) &&
      (rrc_current_state == TDSRRC_STATE_CELL_DCH) &&
      ((oc_status != TDSOC_SET_FOR_DCH_FACH_TRANS ) && (oc_status != TDSOC_SET_FOR_DCH_CELL_PCH_TRANS)
        && (oc_status != TDSOC_SET_FOR_DCH_URA_PCH_TRANS)) &&
      (toc_usage != TDSTOC_FOR_OOS))
  {
    uint8 aset_idx =0;
    uint8 rl_idx = 0;
    tdsordered_config_type * cfg_ptr_in_use = tdsrrcllc_get_config_ptr_in_use();
    tds_active_set_info_type *aset_info = &(tds_UI_event_info.wcdma_event_info.wcdma_active_set_info);
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
          /* Copy the Signalling mesaage payload to the log record */
    memscpy((void *)&(l1_active_set_info),
           sizeof(tdsl1_active_set_info_type),
           (void *)(cmd_ptr->cmd.modem_stat_rsp.l1_active_set_info),
           sizeof(tdsl1_active_set_info_type));
#else
    tds_l1_get_active_set_info(&l1_active_set_info);
#endif
    aset_info->num_rls = l1_active_set_info.num_rls;
    for(aset_idx = 0; (aset_idx < l1_active_set_info.num_rls)&& (aset_idx < TDSL1_ASET_MAX ); aset_idx++)
    {
      aset_info->cpid[aset_idx] = l1_active_set_info.cpid[aset_idx];
      aset_info->rscp[aset_idx] = l1_active_set_info.rscp[aset_idx];
      aset_info->ecio[aset_idx] = l1_active_set_info.ecio[aset_idx];
      aset_info->cell_id[aset_idx] = TDS_INVALID_CELL_ID;
      /* In TDD, Radio Link is only one, tdscell_id_per_rl just have [0] */
      aset_info->cell_id[aset_idx] = tdscell_id_per_rl[0];
    }
    call_cm_api = TRUE;
    tds_UI_event_info.bitmask |=SYS_WCDMA_ACTIVE_SET_INFO_BIT_MASK;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"FTD_DBG:Active Set info bitmask set,bit_mask %x ",tds_UI_event_info.bitmask);
  }

  if(RRC_IS_TDS_UI_EVENT_ENABLED(SYS_WCDMA_MONITORED_SET_BIT_MASK))
  {
    tds_monitored_set_info_type *monitored_set_info = &(tds_UI_event_info.wcdma_event_info.wcdma_monitored_set_info);
    uint8 freq_idx, cell_idx;
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
    /* Copy the Signalling mesaage payload to the log record */
    memscpy((void *)&(l1_monitored_set_info), 
           sizeof(tdsl1_monitored_set_info_type),
             (void *)(cmd_ptr->cmd.modem_stat_rsp.l1_monitored_set_info),
           sizeof(tdsl1_monitored_set_info_type));
#else
    tds_l1_get_monitored_set_info(&l1_monitored_set_info);
#endif
    monitored_set_info->num_of_freqs = l1_monitored_set_info.num_of_freqs;
    if(monitored_set_info->num_of_freqs >TDSL1_MAX_FREQ)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"FTD_DBG:Num of freq more than 3");
      monitored_set_info->num_of_freqs = TDSL1_MAX_FREQ;
    }
    for(freq_idx = 0; freq_idx < l1_monitored_set_info.num_of_freqs && freq_idx < TDSL1_MAX_FREQ ; freq_idx++)
    {
      monitored_set_info->freq[freq_idx].uarfcn = l1_monitored_set_info.freq[freq_idx].uarfcn;
      monitored_set_info->freq[freq_idx].num_of_cells = l1_monitored_set_info.freq[freq_idx].num_of_cells;
      if(monitored_set_info->freq[freq_idx].num_of_cells > TDSL1_MAX_MONITORED_CELLS_PER_FREQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"FTD_DBG:Cells more than TDSL1_MAX_MONITORED_CELLS_PER_FREQ");
        monitored_set_info->freq[freq_idx].num_of_cells  = TDSL1_MAX_MONITORED_CELLS_PER_FREQ;
      }
      for(cell_idx =0; cell_idx < l1_monitored_set_info.freq[freq_idx].num_of_cells && cell_idx < TDSL1_MAX_MONITORED_CELLS_PER_FREQ ; cell_idx++)
      {
        monitored_set_info->freq[freq_idx].cell[cell_idx].cpid = l1_monitored_set_info.freq[freq_idx].cell_info[cell_idx].cpid;
        monitored_set_info->freq[freq_idx].cell[cell_idx].rscp = l1_monitored_set_info.freq[freq_idx].cell_info[cell_idx].rscp;
        monitored_set_info->freq[freq_idx].cell[cell_idx].ecio = l1_monitored_set_info.freq[freq_idx].cell_info[cell_idx].ecio;
      }
    }
    monitored_set_info->num_gsm_cells = 0;
    if(tdsrrcmcm_is_dualmode_enabled())
    {
      monitored_set_info->num_gsm_cells = l1_monitored_set_info.gsm_cells_info.number_of_cells;
      if(l1_monitored_set_info.gsm_cells_info.number_of_cells > TDSL1_MAX_MONITORED_GSM_CELLS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"FTD_DBG:GSM cells more than TDSL1_MAX_MONITORED_GSM_CELLS");
        monitored_set_info->num_gsm_cells = TDSL1_MAX_MONITORED_GSM_CELLS;
      }
      for(cell_idx = 0; cell_idx<l1_monitored_set_info.gsm_cells_info.number_of_cells && cell_idx < TDSL1_MAX_MONITORED_GSM_CELLS; cell_idx++)
      {
        monitored_set_info->gsm_cell[cell_idx].arfcn = CONVERT_ARFCN_TO_NUM(l1_monitored_set_info.gsm_cells_info.interrat_measurements[cell_idx].bcch_arfcn);
        monitored_set_info->gsm_cell[cell_idx].bsic_bcc = l1_monitored_set_info.gsm_cells_info.interrat_measurements[cell_idx].bsic_bcc;
        monitored_set_info->gsm_cell[cell_idx].bsic_ncc = l1_monitored_set_info.gsm_cells_info.interrat_measurements[cell_idx].bsic_ncc;
        monitored_set_info->gsm_cell[cell_idx].rssi = l1_monitored_set_info.gsm_cells_info.interrat_measurements[cell_idx].rssi_dBm_x16;
      }
    }
    call_cm_api = TRUE;
    tds_UI_event_info.bitmask |=SYS_WCDMA_MONITORED_SET_BIT_MASK;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"FTD_DBG:Monitored Set info bitmask set,bit_mask %x ",tds_UI_event_info.bitmask);
  }
  if(call_cm_api)
  {
    /*Call the CM API*/
    cm_stats_set_modem_info_signal(SYS_MODEM_MODULE_WCDMA);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"FTD_DBG: Reported Periodic Events,bitmask %x",tds_UI_event_info.bitmask);
  }
}

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/*===========================================================================

FUNCTION  tdsrrc_process_modem_stat_info

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
void tdsrrc_process_modem_stat_info
(
  /* Pointer to command */
  tdsrrc_cmd_type *cmd_ptr                      
)
{

  if(TDSRRC_CPHY_MODEM_STATISTICS_RSP != cmd_ptr->cmd_hdr.cmd_id)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid cmd %d",cmd_ptr->cmd_hdr.cmd_id);
    return;
  }

  tdsrrc_update_periodic_tds_UI_events_info(cmd_ptr);

  //start the timer again
  tdsrrctmr_start_timer( TDSRRCTMR_RSSI_BER_TIMER,
                      TDSRRCTMR_RSSI_BER_REPORT_TIME );
}
/*===========================================================================

FUNCTION   tdsrrc_post_cmd_to_l1_to_get_modem_stat_info

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
static void tdsrrc_post_cmd_to_l1_to_get_modem_stat_info(void)
{
  /* Used when sending CPHY_IDLE_REQ */
  tdsl1_ext_cmd_type *l1_cmd_ptr;

  if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("NULL Ptr when sending CPHY_IDLE_REQ",0,0,0);
  }
  
  /* Fill in the Command Header */
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSCPHY_GET_MODEM_STATISTICS_REQ;

  /* L1 allocates memory for embd pointers based on bmsk. We currently 
   * set all in bmsk and hence free all the three embedded pointers.
   * In future if bmask is to be changed freeing should be taken care
   */
  l1_cmd_ptr->cmd.get_modem_stat_req.modem_stat_req_bmsk = 
     TDSCDMA_GET_SERVING_CELL_POWER_INFO | TDSCDMA_GET_ACTIVE_SET_INFO |
     TDSCDMA_GET_MONITORED_SET_INFO;

  /* Now send the primitive to L1 */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSCPHY_GET_MODEM_STATISTICS_REQ primitive");

#ifdef FEATURE_DUAL_SIM
  l1_cmd_ptr->cmd_hdr.sys_modem_as_id = tdsrrc_get_as_id();
#endif

  tdsl1_put_cmd(l1_cmd_ptr);
}
#endif /* FEATURE_TDSCDMA_FREE_FLOATING_TASK */
#endif

/*===========================================================================

FUNCTION  RRC_SEND_SCELL_SIGNAL_STATUS_IND

DESCRIPTION

  This function is invoked when the RSSI&BER repoting timer expires. It calls
  L1 to get serving cell signal status info (RSSI/RSCP/ECIO/SINR/BLER) and send
  to upper layer who register UMID TDSCDMA_RRC_SCELL_SIGNAL_STATUS_IND through
  MSGR.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_send_scell_signal_status_ind( void )
{
  tds_l1_scell_signal_status_ind_type l1_scell_signal_status_ind;
  tds_rrc_scell_signal_status_ind_type rrc_scell_signal_status_ind;
  errno_enum_type status;
  tdsrrc_state_e_type rrc_current_state;
  int32 tmp_rssi;
  int32 tmp_rscp;
  int32 tmp_ecio;
  int32 tmp_sinr;

  memset(&rrc_scell_signal_status_ind,0,sizeof(tds_rrc_scell_signal_status_ind_type));

#ifdef FEATURE_DUAL_SIM
  rrc_scell_signal_status_ind.as_id = tdsrrc_get_as_id();
#endif

  /* Get signal status value from L1 through api function below */
  tdssrch_get_serving_cell_signal_strength_info(&l1_scell_signal_status_ind);

  /* L1 sends RSSI/RSCP/ECIO values in 1/256 dBm resolution,
     RRC needs to convert them to 0.1 dBm resolution */
  tmp_rssi = (int32)l1_scell_signal_status_ind.rssi;
  tmp_rscp = (int32)l1_scell_signal_status_ind.rscp;
  tmp_ecio = (int32)l1_scell_signal_status_ind.ecio;

  /* L1 sends SINR values in 1/1024 dB resolution,
     RRC needs to convert them to 0.1 dB resolution */
  tmp_sinr = (int32)l1_scell_signal_status_ind.sinr;

  /* Convert from 1/256 to 0.1 resolution (multiply by 10 then divide by 256) */
  rrc_scell_signal_status_ind.rssi = (int16)((tmp_rssi * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION) >> 8);
  rrc_scell_signal_status_ind.rscp = (int16)((tmp_rscp * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION) >> 8);
  rrc_scell_signal_status_ind.ecio = (int16)((tmp_ecio * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION) >> 8);

  /* Convert from 1/1024 to 0.1 resolution (multiply by 10 then divide by 1024) */
  rrc_scell_signal_status_ind.sinr = (int16)((tmp_sinr * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION) >> 10);

  /* Bound values if out of bound */
  /* RSSI [-120, -25]*10 dBm */
  if(rrc_scell_signal_status_ind.rssi < (TDSRRC_SCELL_SIGNAL_STATUS_MIN_RSSI * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION))
  {
    rrc_scell_signal_status_ind.rssi = (int16)(TDSRRC_SCELL_SIGNAL_STATUS_MIN_RSSI * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION);
  }
  /* RSCP [-120, -25]*10 dBm */
  if(rrc_scell_signal_status_ind.rscp < (TDSRRC_SCELL_SIGNAL_STATUS_MIN_RSCP * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION))
  {
    rrc_scell_signal_status_ind.rscp = (int16)(TDSRRC_SCELL_SIGNAL_STATUS_MIN_RSCP * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION);
  }
  /* Other values are already bounded in L1, no need to check */
  rrc_scell_signal_status_ind.bler = l1_scell_signal_status_ind.bler;

  /* Set in_dch flag if current RRC state is in DCH */
  rrc_current_state = tdsrrc_get_state();
  if(rrc_current_state == TDSRRC_STATE_CELL_DCH)
  {
    rrc_scell_signal_status_ind.in_dch = TRUE;
  }
  else
  {
    rrc_scell_signal_status_ind.in_dch = FALSE;
  }

  TDSRRC_MSG6(MSG_LEGACY_HIGH,"states: rrc %d, csp %d, scan %d, rce %d, cu %d, mode %d",
              tdsrrc_get_state(),
              tdsrrccsp_get_csp_substate(),
              tdsrrccsp_get_curr_scan(),
              tdsrrcrce_get_rce_sub_state(),
              tdsrrccu_return_cu_substate(),
              tdsrrcmcm_get_rrc_mode());

  TDSRRC_MSG6(MSG_LEGACY_HIGH,"rssi = %d, rscp = %d, ecio = %d, in_dch = %d, sinr = %d, bler = %d",
              rrc_scell_signal_status_ind.rssi, rrc_scell_signal_status_ind.rscp,
              rrc_scell_signal_status_ind.ecio, rrc_scell_signal_status_ind.in_dch,
              rrc_scell_signal_status_ind.sinr, rrc_scell_signal_status_ind.bler);

  /* Initialize message needs to be sent to upper layer */
  msgr_init_hdr(&rrc_scell_signal_status_ind.msg_hdr, MSGR_TDSCDMA_RRC, TDSCDMA_RRC_SCELL_SIGNAL_STATUS_IND);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&rrc_scell_signal_status_ind.msg_hdr, (uint8)tdsrrc_get_as_id());
#else
  msgr_set_hdr_inst(&rrc_scell_signal_status_ind.msg_hdr, SYS_AS_ID_TO_INST_ID(tdsrrc_get_as_id()));
#endif
#endif
  status = msgr_send(&rrc_scell_signal_status_ind.msg_hdr, sizeof(tds_rrc_scell_signal_status_ind_type));

  tdsrrc_print_trm_status();
  
#ifdef FEATURE_DUAL_SIM
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Send TDSCDMA_RRC_SCELL_SIGNAL_STATUS_IND, status = %d, as_id = %d", 
              status,
              rrc_scell_signal_status_ind.as_id);
#else
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Send TDSCDMA_RRC_SCELL_SIGNAL_STATUS_IND, status = %d", status);
#endif
}

/*===========================================================================

FUNCTION  tdsrrc_check_L1_accumulated_rscp

DESCRIPTION

  This function calls L1 to get serving cell signal status info (RSSI/RSCP/ECIO/SINR/BLER)
  and return RSCP *10

DEPENDENCIES

  None.

RETURN VALUE

  RSCP *10

SIDE EFFECTS

  None.

===========================================================================*/
int16 tdsrrc_check_L1_accumulated_rscp( void )
{
  tds_l1_scell_signal_status_ind_type l1_scell_signal_status_ind;

  int32 tmp_rscp;
  int16 accu_rscp;


  /* Get signal status value from L1 through api function below */
  tdssrch_get_serving_cell_signal_strength_info(&l1_scell_signal_status_ind);

  /* L1 sends RSSI/RSCP/ECIO values in 1/256 dBm resolution,
     RRC needs to convert them to 0.1 dBm resolution */
  tmp_rscp = (int32)l1_scell_signal_status_ind.rscp;

  /* Convert from 1/256 to 0.1 resolution (multiply by 10 then divide by 256) */
  accu_rscp = (int16)((tmp_rscp * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION) >> 8);

  /* RSCP [-120, -25]*10 dBm */
  if(accu_rscp< (TDSRRC_SCELL_SIGNAL_STATUS_MIN_RSCP * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION))
  {
    accu_rscp = (int16)(TDSRRC_SCELL_SIGNAL_STATUS_MIN_RSCP * TDSRRC_SCELL_SIGNAL_STATUS_RESOLUTION);
  }


  TDSRRC_MSG6(MSG_LEGACY_HIGH,"states: rrc %d, csp %d, scan %d, rce %d, cu %d, mode %d",
              tdsrrc_get_state(),
              tdsrrccsp_get_csp_substate(),
              tdsrrccsp_get_curr_scan(),
              tdsrrcrce_get_rce_sub_state(),
              tdsrrccu_return_cu_substate(),
              tdsrrcmcm_get_rrc_mode());

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Current RSCP = %d",accu_rscp);

  return accu_rscp;

}

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

void tdsrrc_rssi_ber_timeout_handler( void )
{
  /* This function sends serving cell signal status to upper layer
     who registers TDSCDMA_RRC_SCELL_SIGNAL_STATUS_IND */
  tdsrrc_send_scell_signal_status_ind();

#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  tdsrrc_post_cmd_to_l1_to_get_modem_stat_info();
  return;
#else
  tdsrrc_update_periodic_tds_UI_events_info();
#endif /* FEATURE_TDSCDMA_FREE_FLOATING_TASK */
#endif

  // start the timer again
  tdsrrctmr_start_timer( TDSRRCTMR_RSSI_BER_TIMER,
                      TDSRRCTMR_RSSI_BER_REPORT_TIME );

  return;

} /* End of tdsrrc_rssi_ber_timeout_handler */


/*===========================================================================
FUNCTION  tdsrrc_bcch_and_nas_plmns_match

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
boolean   tdsrrc_bcch_and_nas_plmns_match
(
  rrc_plmn_identity_type  bcch_plmn,
  rrc_plmn_identity_type  nas_plmn
)
{
  if(sys_plmn_match(mmumts_convert_rrc_plmn_id_to_nas_plmn_id(nas_plmn),
                    mmumts_convert_rrc_plmn_id_to_nas_plmn_id(bcch_plmn)
                   )
    )
  {
    return(TRUE);
  }
  else
  {
    TDSRRC_MSG6(MSG_LEGACY_HIGH,"NAS and BCCH MCC-MNC are differ. NAS MCC %d MNC %d MNC digits %d, BCCH MCC %d MNC %d MNC digits %d ",
               TDSRRC_CSP_GET_INT_MCC(nas_plmn), TDSRRC_CSP_GET_INT_MNC(nas_plmn), nas_plmn.num_mnc_digits,
               TDSRRC_CSP_GET_INT_MCC(bcch_plmn), TDSRRC_CSP_GET_INT_MNC(bcch_plmn), bcch_plmn.num_mnc_digits);
    return(FALSE);
  }

}  /* tdsrrc_bcch_and_nas_plmns_match */


/*===========================================================================
FUNCTION  tdsrrc_bcch_plmns_match

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
boolean tdsrrc_bcch_plmns_match(rrc_plmn_identity_type plmn1, rrc_plmn_identity_type plmn2)
{
  byte i;

  for(i = 0; i < RRC_MAX_MCC_DIGITS; i++)
  {
    if(plmn1.mcc[i] != plmn2.mcc[i])
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH MCC differs");
      return(FALSE);
    }
  }

  //This code is to satify KW error only.
  if (plmn1.num_mnc_digits > RRC_MAX_MCC_DIGITS)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"KW: num_mnc_digits of plmn1 is out of bound,num_mnc_digits of plmn1 %d",plmn1.num_mnc_digits);
    plmn1.num_mnc_digits = RRC_MAX_MCC_DIGITS;
  }

  if (plmn2.num_mnc_digits > RRC_MAX_MCC_DIGITS)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"KW: num_mnc_digits of plmn2 is out of bound,num_mnc_digits of plmn2 %d",plmn2.num_mnc_digits);
    plmn2.num_mnc_digits = RRC_MAX_MCC_DIGITS;
  }

  //This code is to satify KW error only.
  if((plmn1.num_mnc_digits <= RRC_MAX_MCC_DIGITS) && (plmn1.num_mnc_digits == plmn2.num_mnc_digits))
  {
    for(i = 0; i < plmn1.num_mnc_digits; i++)
    {
      if(plmn1.mnc[i] != plmn2.mnc[i])
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH MNC differs");
        return(FALSE);
      }
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH num MNC digits differ");
    return(FALSE);
  }

  return(TRUE);
}
/*===========================================================================
FUNCTION  tdsrrc_update_ue_conn_timers_and_constants

DESCRIPTION
  This function updates tdsrrc_ue_conn_timers_and_constants based on updates
  received in sib1 or Utran Mobility Info messages
  
DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  Updates the global variable tdsrrc_ue_conn_timers_and_constants
===========================================================================*/
uecomdef_status_e_type tdsrrc_update_ue_conn_timers_and_constants
(
  tdsrrc_UE_ConnTimersAndConstants* ue_conn_timers_and_constants_ptr,
  tdsrrc_UE_ConnTimersAndConstants_v3a0ext * ext_ptr
#if defined(FEATURE_3GPP_FAST_DORMANCY)
  ,tdsrrc_UE_ConnTimersAndConstants_v860ext *ue_conn_timers_and_constants_v860ext_ptr
#endif
)
{
  if(ue_conn_timers_and_constants_ptr != NULL)
  {
    /* copy the updated ue_conn_timers_and_constants to global variable
     * tdsrrc_ue_conn_timers_and_constants.  incase bit mask is not set for
     * a particular timer or constant then set it to default as all params
     * are mandatory default
     */
  
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updating Connected Mode Timers/Const");
  



      tdsrrc_ue_conn_timers_and_constants.t_301=  
      ue_conn_timers_and_constants_ptr->t_301;  
    
      tdsrrc_ue_conn_timers_and_constants.n_301=  
        ue_conn_timers_and_constants_ptr->n_301;  
  
      tdsrrc_ue_conn_timers_and_constants.t_302=  
        ue_conn_timers_and_constants_ptr->t_302;  
    
    
      tdsrrc_ue_conn_timers_and_constants.n_302=  
        ue_conn_timers_and_constants_ptr->n_302;  
    
  
      tdsrrc_ue_conn_timers_and_constants.t_304=  
        ue_conn_timers_and_constants_ptr->t_304;  
    
      tdsrrc_ue_conn_timers_and_constants.n_304=  
        ue_conn_timers_and_constants_ptr->n_304;  
    
      tdsrrc_ue_conn_timers_and_constants.t_305=  
        ue_conn_timers_and_constants_ptr->t_305;  
    
      tdsrrc_ue_conn_timers_and_constants.t_307=  
        ue_conn_timers_and_constants_ptr->t_307;  
    
      tdsrrc_ue_conn_timers_and_constants.t_308=  
        ue_conn_timers_and_constants_ptr->t_308;  
    
      tdsrrc_ue_conn_timers_and_constants.t_309=  
        ue_conn_timers_and_constants_ptr->t_309;  
    
      tdsrrc_ue_conn_timers_and_constants.t_310=  
        ue_conn_timers_and_constants_ptr->t_310;  
    
      tdsrrc_ue_conn_timers_and_constants.n_310=  
        ue_conn_timers_and_constants_ptr->n_310;  

      tdsrrc_ue_conn_timers_and_constants.t_311=  
        ue_conn_timers_and_constants_ptr->t_311;  
 
      tdsrrc_ue_conn_timers_and_constants.t_312=  
        ue_conn_timers_and_constants_ptr->t_312;  
    
      tdsconn_timers_and_constants.n312=  
        tdsrrc_translate_n312(ue_conn_timers_and_constants_ptr->n_312);  
    
      tdsrrc_ue_conn_timers_and_constants.t_313=  
        ue_conn_timers_and_constants_ptr->t_313;  
  
      tdsrrc_ue_conn_timers_and_constants.n_313=  
        ue_conn_timers_and_constants_ptr->n_313;  
    
      tdsrrc_ue_conn_timers_and_constants.t_314=  
        ue_conn_timers_and_constants_ptr->t_314;  
    
      tdsrrc_ue_conn_timers_and_constants.t_315=  
        ue_conn_timers_and_constants_ptr->t_315;  
    
      tdsconn_timers_and_constants.n315 = tdsrrc_translate_n315(ue_conn_timers_and_constants_ptr->n_315);
   
      tdsrrc_ue_conn_timers_and_constants.t_316=  
        ue_conn_timers_and_constants_ptr->t_316;  
    
      tdsrrc_ue_conn_timers_and_constants.t_317=  
        ue_conn_timers_and_constants_ptr->t_317;  
   
    }
#if defined(FEATURE_3GPP_FAST_DORMANCY)
  if(ue_conn_timers_and_constants_v860ext_ptr != NULL)
  {
    /* No need to STOP original T323 timer.
        * If dormancy is disabled then might be Timer (T323) is running because of SRNS
        * Relocation or CELL_PCH/URA_PCH state guard timers.
        * Stop them as dormancy can be triggered just after timer updation */
    
    if(ue_conn_timers_and_constants_v860ext_ptr->m.t_323Present)
    {
      tdsrrcdormancy_conn_timers_and_constants.T323_nw_support = TRUE;
      tdsrrcdormancy_conn_timers_and_constants.T323_duration = 
        ue_conn_timers_and_constants_v860ext_ptr->t_323;      
      tdsrrcdormancy_conn_timers_and_constants.T323_current_status = 
        tdsrrcdormancy_stop_n_update_t323_timer_status();
    }
    else
    {
      tdsrrcdormancy_disable_fast_dormancy();
      tdsrrcdormancy_stop_or_restart_timer_T323(TDSRRCDORM_STOP_T323_TIMER_VALUE,
        TDSRRCDORMANCY_T323_TIMER_INVALID);
    }
  }
  else
  {
    tdsrrcdormancy_disable_fast_dormancy();
    tdsrrcdormancy_stop_or_restart_timer_T323(TDSRRCDORM_STOP_T323_TIMER_VALUE,
      TDSRRCDORMANCY_T323_TIMER_INVALID);
  }
  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Updating T323 NW:%d Duration:%d Status:%d", 
   tdsrrcdormancy_conn_timers_and_constants.T323_nw_support,
   tdsrrcdormancy_conn_timers_and_constants.T323_duration,
   tdsrrcdormancy_conn_timers_and_constants.T323_current_status);
  
#endif

  if(ext_ptr != NULL)
  {

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updating Connected Mode Ext Timers/Const");
    /* copy the updated ue_conn_timers_and_constants to global variable
     * tdsrrc_ue_conn_timers_and_constants.  incase bit mask is not set for
     * a particular timer or constant then set it to default as all params
     * are mandatory default
     */
  
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updating Connected Mode Ext Timers/Const");
  
    if(ext_ptr->m.n_312Present)
    {
      tdsconn_timers_and_constants.n312 = tdsrrc_translate_n312_v3a0ext(ext_ptr->n_312);
    }
  
    if(ext_ptr->m.n_315Present)
    {
      tdsconn_timers_and_constants.n315 = tdsrrc_translate_n315_v3a0ext(ext_ptr->n_315);
    }
  }

#ifdef FEATURE_QSH_EVENT_METRIC
  tdsrrcqsh_timer_and_constants_update();
#endif

  return SUCCESS;

}/*tdsrrc_update_ue_conn_timers_and_constants*/
/*===========================================================================
FUNCTION  tdsrrc_initialize_ue_conn_timers_and_constants

DESCRIPTION
  This function Initializes tdsrrc_ue_conn_timers_and_constants to default values
  as all parameters are mandatory default
  
DEPENDENCIES
  None

RETURN VALUE
 None

SIDE EFFECTS
  Initializes the global variable tdsrrc_ue_conn_timers_and_constants with
  default values as all parameters are Mandatory Default
===========================================================================*/
void tdsrrc_initialize_ue_conn_timers_and_constants
(
  void
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initializing Connected Mode Timers/Const");

  tdsrrc_ue_conn_timers_and_constants.t_301= tdsrrc_T_301_ms2000;  
  tdsrrc_ue_conn_timers_and_constants.n_301= 2; 
  tdsrrc_ue_conn_timers_and_constants.t_302= tdsrrc_T_302_ms4000; 
  tdsrrc_ue_conn_timers_and_constants.n_302= 3;
  tdsrrc_ue_conn_timers_and_constants.t_304= tdsrrc_T_304_ms2000;
  tdsrrc_ue_conn_timers_and_constants.n_304= 2;
  tdsrrc_ue_conn_timers_and_constants.t_305= tdsrrc_T_305_m30; 
  tdsrrc_ue_conn_timers_and_constants.t_307= tdsrrc_T_307_s30;
  tdsrrc_ue_conn_timers_and_constants.t_308= tdsrrc_T_308_ms160;
  tdsrrc_ue_conn_timers_and_constants.t_309= 5;
  tdsrrc_ue_conn_timers_and_constants.t_310= tdsrrc_T_310_ms160;
  tdsrrc_ue_conn_timers_and_constants.n_310= 4;
  tdsrrc_ue_conn_timers_and_constants.t_311= tdsrrc_T_311_ms2000;
  tdsrrc_ue_conn_timers_and_constants.t_312= 1;
  tdsconn_timers_and_constants.n312= tdsrrc_translate_n312(tdsrrc_N_312_s1);
  tdsrrc_ue_conn_timers_and_constants.t_313= 3;
  tdsrrc_ue_conn_timers_and_constants.n_313= tdsrrc_N_313_s20;
  tdsrrc_ue_conn_timers_and_constants.t_314= tdsrrc_T_314_s12;
  tdsrrc_ue_conn_timers_and_constants.t_315= tdsrrc_T_315_s180;
  tdsconn_timers_and_constants.n315= tdsrrc_translate_n315(tdsrrc_N_315_s1);
  tdsrrc_ue_conn_timers_and_constants.t_316= tdsrrc_T_316_s30;
  tdsrrc_ue_conn_timers_and_constants.t_317= tdsrrc_T_317_infinity4;
  tdsrrc_skip_dynamic_power_class = FALSE;
}/*tdsrrc_initialize_ue_conn_timers_and_constants*/

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
boolean tdsrrc_check_rbs_associated_with_re_est_timer
(    
  tdsrrc_Re_EstablishmentTimer re_est_timer
)
{
  uint32 count;
  boolean match_found = FALSE;    /* Local varible used to update the match
                                     for Re_establishment timer */
   
  /* First check if there are RABs assoicated with re_establishment timer */
  for (count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if( (tdsrrc_est_rabs.rabs[count].re_est_timer == re_est_timer) &&
        (tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > 0)  )
    {
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"RAB found id:%d,rbs:%d,re-est tmr:%d",
               tdsrrc_est_rabs.rabs[count].rab_id,
               tdsrrc_est_rabs.rabs[count].num_rbs_for_rab,
               re_est_timer
               );
      match_found = TRUE;
      break;
    }
  }
  return(match_found);
} /* tdsrrc_check_rbs_associated_with_re_est_timer */

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
boolean tdsrrc_check_rabs_associated_with_cn_domain
(    
  rrc_cn_domain_identity_e_type rrc_cn_domain_id
)
{
  uint32 count;
  boolean match_found = FALSE;    /* Local varible used to update the match
                                     for CN Domian Id */
   
  /* First check if there are RABs assoicated with CN Domain Id */
  for (count = 0; count < MAX_RAB_TO_SETUP; count++)
  {
    if((tdsrrc_est_rabs.rabs[count].cn_domain == rrc_cn_domain_id) &&
        (tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > 0)  )
    {
      match_found = TRUE;
      break;
    }
  }
  return(match_found);
} /* tdsrrc_check_rabs_associated_with_cn_domain */

/*===========================================================================

FUNCTION RRC_CONVERT_T305_TO_MS

DESCRIPTION
  This function converts the tdsrrc_T_305 enums to milliseconds

DEPENDENCIES
  None

RETURN VALUE
  uint32

SIDE EFFECTS
  None
===========================================================================*/
uint32 tdsrrc_convert_t305_to_ms
(
  tdsrrc_T_305 rrccu_t_305                    /* enum for Timer T305 */
)
{
  uint32    timer_value;                         /* Timer Value in milli seconds */

  switch (rrccu_t_305)
  {
    case tdsrrc_T_305_noUpdate:

      timer_value = TDSRRCTMR_INFINITY_VAL;          /* This may approximately  equal to
                                            24 hrs */
      break;
    case tdsrrc_T_305_m5:
      timer_value = 5 * 60 * 1000;
      break;      
    case tdsrrc_T_305_m10:
      timer_value = 10 * 60 * 1000;
      break;
    case tdsrrc_T_305_m30:
      timer_value = 30 * 60 * 1000;
      break;
    case tdsrrc_T_305_m60:
      timer_value = 60 * 60 * 1000;
      break;
    case tdsrrc_T_305_m120:
      timer_value = 120 * 60 * 1000;
      break; 
    case tdsrrc_T_305_m360:
      timer_value = 360 * 60 * 1000;
      break;
    case tdsrrc_T_305_m720:
      timer_value = 720 * 60 * 1000;
      break;
    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unknown Timer value received ");
      timer_value = TDSRRCTMR_INFINITY_VAL;         /* Set to no periodic Cell Update */
      break;
  }
  return (timer_value);
} /* tdsrrc_convert_t305_to_ms*/

/*===========================================================================

FUNCTION RRC_TRANSLATE_STATE_INDICATOR_TO_STATE

DESCRIPTION
  This function converts the Received RRC state indicator into internal
  RRC state format
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrc_state_e_type : returns the RRC state
  
SIDE EFFECTS

  None

===========================================================================*/
tdsrrc_state_e_type tdsrrc_translate_state_indicator_to_state
(    
  tdsrrc_RRC_StateIndicator state_indicator
)
{
  tdsrrc_state_e_type state = TDSRRC_STATE_MAX ;       /* Local variable to store the state */
  switch(state_indicator)
  {
    case tdsrrc_RRC_StateIndicator_cell_DCH:
      state = TDSRRC_STATE_CELL_DCH;
      break;
    case tdsrrc_RRC_StateIndicator_cell_FACH:
      state = TDSRRC_STATE_CELL_FACH;
      break;
    case tdsrrc_RRC_StateIndicator_cell_PCH:
      state = TDSRRC_STATE_CELL_PCH;
      break;
    case tdsrrc_RRC_StateIndicator_ura_PCH:
      state = TDSRRC_STATE_URA_PCH;
      break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Inivalid State indicator: %d ",state_indicator);
      break;
  }
  return(state);
} /* tdsrrc_translate_state_indicator_to_state*/

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
tdsrrc_status_e_type tdsrrc_return_cs_data_call_parms
(    
  uint32                  rab_id,
  tdsrrc_cs_data_parms_type *cs_data_parms_ptr
)
{
  int i, j, k;
  tdsrrc_RB_Identity rb_id;
  rlc_lc_id_type lc_id;
  tr_ch_id_type tdstrch_id = TDSRRCLLC_TRCH_ID_MAX_VALUE + 1;
  uint8 trch_idx = TDSUE_MAX_TRCH;
  tdsordered_config_type *cfg_ptr;
  uint16 max_ntblks = 0;

  if(cs_data_parms_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"NULL cs_data_parms_ptr passed to tdsrrc_return_cs_data_call_parms()");
    return TDSRRC_STATUS_FAILURE;
  }

  if(tdsrrcllc_get_ordered_config_status() != TDSOC_NOT_SET)
  {
    cfg_ptr = tdsordered_config_ptr;
  }
  else
  {
    cfg_ptr = tdscurrent_config_ptr;
  }

  for(i = 0; i < MAX_RAB_TO_SETUP; i++)
  {
    if(tdsrrc_est_rabs.rabs[i].rab_id == rab_id )
    {
      if(tdsrrc_est_rabs.rabs[i].num_rbs_for_rab != 1)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"RAB id %d has more than one RB mapped for CS data call. Choosing first RB.",tdsrrc_est_rabs.rabs[i].rab_id);
      }
      rb_id = tdsrrc_est_rabs.rabs[i].rb_for_rab[0].rb_id;
      lc_id = tdsrrclcm_get_ul_rlc_lc_id(rb_id);
      for(j = 0; j < TDSUE_MAX_TRCH; j++)
      {
        for(k = 0; k < MAX_DED_LOGCHAN_PER_TRCH; k++)
        {
          if(cfg_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[j].dlchan_info[k].rlc_id ==
             lc_id)
          {
            tdstrch_id = cfg_ptr->mac_ul_parms.cctrch_ptr->dch_info.dch_info[j].trch_id;
            break;
          }
        }

        if(tdstrch_id < TDSRRCLLC_TRCH_ID_MAX_VALUE)
        {
          break;
        }
      }

      if(tdstrch_id == TDSRRCLLC_TRCH_ID_MAX_VALUE)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"trch_id not found for lc_id %d",lc_id);
        return TDSRRC_STATUS_FAILURE;
      }

      for(i = 0; i < TDSUE_MAX_TRCH; i++)
      {
        if(cfg_ptr->ul_dch_trch_info[i].trch_id == tdstrch_id)
        {
          trch_idx = (uint8)i;
          break;
        }
      }

      if(trch_idx == TDSUE_MAX_TRCH)
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Index not found for trch_id %d",tdstrch_id);
        return TDSRRC_STATUS_FAILURE;
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
      return TDSRRC_STATUS_SUCCESS;
    }
  }

  return TDSRRC_STATUS_FAILURE;
} /* tdsrrc_return_cs_data_call_parms */

#ifdef FEATURE_DUAL_WCDMA

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
tdsrrc_status_e_type tdsrrc_return_cs_data_call_parms_sub
( 
  sys_modem_as_id_e_type as_id,
  uint32                  rab_id,
  tdsrrc_cs_data_parms_type *cs_data_parms_ptr
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return tdsrrc_return_cs_data_call_parms(rab_id,cs_data_parms_ptr);
}
#endif
/*===========================================================================

FUNCTION   tdsrrc_is_voice_rab_present

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
boolean tdsrrc_is_voice_rab_present (void)
{
  uint8 count; /* Local Counter */
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( TDSRRCRB_CS_VOICE_RAB == tdsrrc_est_rabs.rabs[count].rab_type)
    {
      /* We have a match */
      return( TRUE );
    }
  }
  return(FALSE);
}
/*===========================================================================

FUNCTION   tdsrrc_is_cs_data_rab_present

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
boolean tdsrrc_is_cs_data_rab_present (void)
{
  uint8 count; /* Local Counter */
  
  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( TDSRRCRB_CS_DATA_RAB == tdsrrc_est_rabs.rabs[count].rab_type)
    {
        /* We have a match */
        return( TRUE );
    }
  }
  return(FALSE);
}

/*===========================================================================

FUNCTION   tdsrrc_get_rb_id_list_associated_with_voice_rab

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
boolean tdsrrc_get_rb_id_list_associated_with_voice_rab 
(
  uint32 *num_rb,
  uint32 rb_id_list[MAX_RB_PER_RAB]
)
{
  uint8 count, rb_index; /* Local counters */

  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if( TDSRRCRB_CS_VOICE_RAB == tdsrrc_est_rabs.rabs[count].rab_type)
    {
      if (NULL != num_rb)
      {
        *num_rb = tdsrrc_est_rabs.rabs[count].num_rbs_for_rab;
  
        for (rb_index=0; rb_index<(*num_rb); rb_index++)
        {
          rb_id_list[rb_index] = tdsrrc_est_rabs.rabs[count].rb_for_rab[rb_index].rb_id;
        }
      }

      return(TRUE);
    }
  }

  *num_rb = 0;
  return(FALSE);
}

/*===========================================================================

FUNCTION   tdsrrc_get_rab_type_for_given_rb_id 

DESCRIPTION

  This function returns the RAB type of a RAB associated with a given RB id. This function
  returns TDSRRCRB_UNKNOWN_RAB if there is no RAB exist for a given RB id.
        
DEPENDENCIES

  None.

ARGUMENTS
  rbId: rb Id

RETURN VALUE

  tdsrrcrb_rab_e_type: rab type

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_e_type tdsrrc_get_rab_type_for_given_rb_id 
(
  uint32 rb_id
)
{
  uint8 count, rb_index; /* Local counters */

  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    for (rb_index=0; rb_index<tdsrrc_est_rabs.rabs[count].num_rbs_for_rab; rb_index++)
    {
      if (rb_id == tdsrrc_est_rabs.rabs[count].rb_for_rab[rb_index].rb_id)
      {
        return(tdsrrc_est_rabs.rabs[count].rab_type);
      }
    }
  }

  /* if it comes here then it means that given rb_id is not part of any established RABs */
  return(TDSRRCRB_UNKNOWN_RAB);
}

/*===========================================================================

FUNCTION   tdsrrc_get_rab_type_for_given_rab_id 

DESCRIPTION

  This function returns the RAB type of a given RAB id. This function returns
  TDSRRCRB_UNKNOWN_RAB if RAB with a given RAB id is not found.
        
DEPENDENCIES

  None.

ARGUMENTS
  rab_Id: RAB Id

RETURN VALUE

  tdsrrcrb_rab_e_type: rab type

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_e_type tdsrrc_get_rab_type_for_given_rab_id 
(
  uint32 rab_id
)
{
  uint8 count; /* Local counters */

  for(count=0; count<MAX_RAB_TO_SETUP; count++)
  {
    if (rab_id == tdsrrc_est_rabs.rabs[count].rab_id)
    {
      return(tdsrrc_est_rabs.rabs[count].rab_type);
    }
  }

  /* if it comes here then it means that given rab_id is not part of any established RABs */
  return(TDSRRCRB_UNKNOWN_RAB);
}
/*===========================================================================

FUNCTION   tdsrrc_get_idx_in_rrc_est_rabs_for_rab_id

DESCRIPTION

  This function returns the index in tdsrrc_est_rabs for a given rab_id.
        
ARGUMENTS

RETURN VALUE

SIDE EFFECTS

  None.

===========================================================================*/
uint8 tdsrrc_get_idx_in_rrc_est_rabs_for_rab_id(rrc_rab_id_type rab_id)
{
  uint8 idx=0;
  for(idx=0; idx<MAX_RAB_TO_SETUP;idx++)
  {
    if(tdsrrc_est_rabs.rabs[idx].rab_id == rab_id)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RAB id %d found in EST_RABs at idx %d",rab_id,idx);    
      return idx;
    }
  }
  return MAX_RAB_TO_SETUP;
}

/*===========================================================================

FUNCTION   tdsrrc_update_rab_type_in_rab_info

DESCRIPTION
 This function is updates the rab type in 'tdsrrc_est_rab_info_type' structure
 using information stored in ASN1 'tdsrrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'tdsrrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  tdsrrc_RAB_InformationSetup *: pointer to ASN1 structure,
  tdsrrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

tdsrrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_e_type tdsrrc_update_rab_type_in_rab_info
(
  tdsrrc_RAB_InformationSetup *rab_info_setup_ptr,
  tdsrrc_est_rab_info_type *est_rab_info_ptr 
)
{
  tdsrrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (tdsrrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = TDSRRCRB_PS_DATA_RAB;
  }
  else if (tdsrrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = tdsrrcrb_translate_gsm_map_rab_id(
     rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity );
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
    mn_rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if( tdsrrclbt_is_lb_on() == TRUE )
    {
      /* Set Rab type to Data */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB setup for Loopback Test");
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB ;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_UNKNOWN_RAB;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cn_domain value: %d",cn_domain);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Cn domain %d",cn_domain);
  }
   

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
            est_rab_info_ptr->rab_type, TDSRRCRB_CS_VOICE_RAB, TDSRRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}

/*===========================================================================

FUNCTION   tdsrrc_update_rab_type_in_rab_info_r5

DESCRIPTION
 This function is updates the rab type in 'tdsrrc_est_rab_info_type' structure
 using information stored in ASN1 'tdsrrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'tdsrrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  tdsrrc_RAB_InformationSetup *: pointer to ASN1 structure,
  tdsrrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

tdsrrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_e_type tdsrrc_update_rab_type_in_rab_info_r5
(
  tdsrrc_RAB_InformationSetup_r5 *rab_info_setup_ptr,
  tdsrrc_est_rab_info_type *est_rab_info_ptr 
)
{
  tdsrrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (tdsrrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = TDSRRCRB_PS_DATA_RAB;
  }
  else if (tdsrrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = tdsrrcrb_translate_gsm_map_rab_id(
    rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
    mn_rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if ( tdsrrclbt_is_lb_on() == TRUE )
    {
      /* Set Rab type to Data */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB setup for Loopback Test");
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB ;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_UNKNOWN_RAB;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cn_domain value: %d",cn_domain);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Cn domain %d",cn_domain);
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
            est_rab_info_ptr->rab_type, TDSRRCRB_CS_VOICE_RAB, TDSRRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}

/*===========================================================================

FUNCTION   tdsrrc_get_3g_cell_capability_indicator

DESCRIPTION

  This function will indicate if the current cell supports HSDPA.
        
DEPENDENCIES

  None
 
RETURN VALUE

  None
SIDE EFFECTS

  None.

===========================================================================*/
sys_hs_ind_e_type tdsrrc_get_3g_cell_capability_indicator (void)
{
  sys_hs_ind_e_type cell_capability_indicator = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;
  tdsrrc_SysInfoType5 *sib5_ptr = NULL;

  if(!tdsrrcsibdb_is_serving_cell_sib_vaild())
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Serving Cell Index is not Valid");
    return cell_capability_indicator;
  }

  sib5_ptr = (tdsrrc_SysInfoType5*)  tdsrrcsibdb_return_sib_for_srv_cell
    (tdsrrc_SIB5);
  
  if (sib5_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB5 Ptr NULL");
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
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"HSDPA CAPABLE CPID %d",tdsrrcccm_get_curr_camped_cpid());
      cell_capability_indicator =  SYS_HS_IND_HSDPA_SUPP_CELL;
    }

#ifdef FEATURE_TDSCDMA_HSUPA
    if (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.
       v680NonCriticalExtensions.m.v690NonCriticalExtensionsPresent)
    {
      if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
      {
        if (sib5_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtensions.v650NonCriticalExtensions.
         v680NonCriticalExtensions.v690NonCriticalExtensions.sysInfoType5_v690ext.m.edch_CellIndicatorPresent)
        {
          if (SYS_HS_IND_HSDPA_SUPP_CELL == cell_capability_indicator)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"HSUPA(and HSDPA) CAPABLE CPID %d",tdsrrcccm_get_curr_camped_cpid());
            cell_capability_indicator = SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL;
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"HSUPA CAPABLE CPID %d",tdsrrcccm_get_curr_camped_cpid());
            cell_capability_indicator = SYS_HS_IND_HSUPA_SUPP_CELL;
          }
        }
      }
    }
#endif /* FEATURE_TDSCDMA_HSUPA */
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"HSDPACAPABLE NOT CPID %d",tdsrrcccm_get_curr_camped_cpid());
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
void tdsrrc_send_3g_cell_capability_information(sys_hs_ind_e_type hsdpa_hsupa_indication)
{
  mm_cmd_type *mm_buf_ptr;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Send RRC_HSDPA_HSUPA_CELL_IND = %d",hsdpa_hsupa_indication);
  /* Get a buffer from MM task */
  if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_HSDPA_HSUPA_CELL_IND)) == NULL)
  {
    ERR_FATAL("No MM buffer for RRC_HSDPA_HSUPA_CELL_IND",0,0,0);
  }

#ifdef FEATURE_DUAL_SIM
   mm_buf_ptr->cmd.rrc_3g_availability_cell_ind.as_id = tdsrrc_get_as_id();
#endif

  mm_buf_ptr->cmd.rrc_3g_availability_cell_ind.hsdpa_hsupa_ind = hsdpa_hsupa_indication;

  /* Put the command on MM queue */
  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_mm_put_cmd(mm_buf_ptr);
  }
  mm_put_cmd(mm_buf_ptr);
#ifdef FEATURE_TDSCDMA_HSUPA  
  tdsrrc_event_log_hsupa_support_status(hsdpa_hsupa_indication);
#endif  
} /* tdsrrc_send_3g_cell_capability_information */
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
void tdsrrc_send_hspa_call_status_info
(
  void
)
{
  mm_cmd_type *mm_buf_ptr;
   
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
  tdsordered_config_type * config_ptr = NULL;


  if(tdsconfig_ptr_in_use == TDSORDERED_CONFIG)
  {
    config_ptr = tdsordered_config_ptr;
  }
  else
  {
    config_ptr = tdscurrent_config_ptr;
  }
#endif
  tdscurr_hspa_call_status = SYS_HS_IND_HSDPA_HSUPA_UNSUPP_CELL;


  if (tdshsdpa_status == TDSHSDPA_ACTIVE) 
  {
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    if(config_ptr->mac_dl_parms.mac_hs_or_ehs == MAC_EHS )
    {
      tdscurr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_SUPP_CELL;
    }
    else
#endif
    {
      tdscurr_hspa_call_status = SYS_HS_IND_HSDPA_SUPP_CELL;
    }

  }

#ifdef FEATURE_TDSCDMA_HSUPA
  if (tdsrrc_is_eul_active()) 
  {
    if (tdscurr_hspa_call_status == SYS_HS_IND_HSDPA_SUPP_CELL) 
    {
      tdscurr_hspa_call_status = SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL;
    }
#ifdef FEATURE_TDSCDMA_HSDPA_MAC_EHS
    else if (tdscurr_hspa_call_status == SYS_HS_IND_HSDPAPLUS_SUPP_CELL) 
    {
      tdscurr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_HSUPA_SUPP_CELL;
    }
#endif
    else
    {
      tdscurr_hspa_call_status = SYS_HS_IND_HSUPA_SUPP_CELL;
    }
  }
#endif /*FEATURE_TDSCDMA_HSUPA */
#ifdef FEATURE_TDSCDMA_64QAM
  if((tdsrrcllc_get_64QAM_status() == TRUE) && (tdshsdpa_status == TDSHSDPA_ACTIVE))
  {
    if(tdsrrc_is_eul_active())
    {
      tdscurr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_64QAM_HSUPA_SUPP_CELL;
    }
    else
    {
      tdscurr_hspa_call_status = SYS_HS_IND_HSDPAPLUS_64QAM_SUPP_CELL;
    }
  }
#endif

#ifdef FEATURE_TDS_DC_HSDPA
  /*Update the call status only if DC-HS is set to active.*/
  if(tdsrrcllc_get_dc_hsdpa_status() == TDSRRC_DC_HSDPA_ACTIVE)
  {
    /*Whether MAC-HS or MAC-EHS is active with EUL, update call status as same.
    it might be changed later if DC-HS status needed separately for MAC-HS or EHS.*/
    if((tdscurr_hspa_call_status == SYS_HS_IND_HSUPA_SUPP_CELL) ||
      (tdscurr_hspa_call_status == SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL) ||
      (tdscurr_hspa_call_status == SYS_HS_IND_HSDPAPLUS_HSUPA_SUPP_CELL))
    {
      tdscurr_hspa_call_status = SYS_HS_IND_DC_HSDPAPLUS_HSUPA_SUPP_CELL;
    }
    else
    {
      tdscurr_hspa_call_status = SYS_HS_IND_DC_HSDPAPLUS_SUPP_CELL;
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Updated Call status is %d",tdscurr_hspa_call_status);
  }
#endif /*FEATURE_TDS_DC_HSDPA*/

  if (tdsprev_hspa_call_status != tdscurr_hspa_call_status) 
  {

    /* Get a buffer from MM task */
    if((mm_buf_ptr = mm_tdsrrc_get_cmd_buf(RRC_HSPA_CALL_STATUS_IND)) == NULL)
    {
      ERR_FATAL("No MM buffer for RRC_HSPA_CALL_STATUS_IND",0,0,0);
    }
    
    mm_buf_ptr->cmd.rrc_hspa_call_status_ind.hs_call_status_ind = tdscurr_hspa_call_status;
    
    tdsprev_hspa_call_status = tdscurr_hspa_call_status;

#ifdef FEATURE_DUAL_SIM
    mm_buf_ptr->cmd.rrc_hspa_call_status_ind.as_id = tdsrrc_get_as_id();
#endif

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Send RRC_HSPA_CALL_STATUS_IND %d",tdscurr_hspa_call_status);

    /* Put the command on MM queue */
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
      tdscfa_mm_put_cmd(mm_buf_ptr);
    }
    mm_put_cmd(mm_buf_ptr);
  } 

} /* tdsrrc_send_hspa_call_status_info */
#endif /* FEATURE_HSPA_CALL_STATUS_IND */

/*===========================================================================

FUNCTION   tdsrrc_indicate_layer1_next_config_is_for_voice

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
void tdsrrc_indicate_layer1_next_config_is_for_voice (void)
{

  /* If this rab is voice rab then disable ul tfc restriction based 
     on total power.
  */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Disable UL TX PWR restriction -AMR is going to be established ");
  tdsl1_disable_ul_tfc_pwr_restriction();


}

/*===========================================================================

FUNCTION   tdsrrc_indicate_layer1_next_config_will_release_voice_rab

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
void tdsrrc_indicate_layer1_next_config_will_release_voice_rab (void)
{

  /* If this rab is voice rab then disable ul tfc restriction based 
     on total power.
  */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Enable UL TX PWR restriction - AMR to be released");
  tdsl1_enable_ul_tfc_pwr_restriction();


}

/*===========================================================================

FUNCTION   tdsrrc_is_given_rb_rel_list_releases_all_rb_for_voice_rab

DESCRIPTION

  This function determines whether the given 'tdsrrc_RB_InformationReleaseList'
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
boolean tdsrrc_is_given_rb_rel_list_releases_all_rb_for_voice_rab 
(
  tdsrrc_RB_InformationReleaseList *rb_release_list_ptr
)
{
  uint32 count,index,n_value; /* local counter */
  uint32 num_rb, temp_num_rb;
  uint32 rb_id_list[MAX_RB_PER_RAB];

  n_value = rb_release_list_ptr->n;
  if (TRUE == tdsrrc_get_rb_id_list_associated_with_voice_rab(&num_rb, rb_id_list))
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
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Given configuration will release all RBs associated with Voice RAB");
    }
  }
  /* if we reach this point then it means that either voice RAB is not present or 
     all RBs associated with voice RAB are going to be released 
  */
  return(TRUE);
}


/*===========================================================================

FUNCTION tdsrrc_set_hsdpa_action

DESCRIPTION

  This function sets HSDPA action in global variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrc_set_hsdpa_action
(
  tdshsdpa_action_enum_type action
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Set HSDPA Action to %d",action);
  tdshsdpa_action = action;
}

/*===========================================================================

FUNCTION tdsrrc_get_hsdpa_action

DESCRIPTION

  This function returns HSDPA action variable from global variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

tdshsdpa_action_enum_type tdsrrc_get_hsdpa_action
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:HSDPA Action returned is %d",tdshsdpa_action);
  return tdshsdpa_action;
}


/*===========================================================================

FUNCTION tdsrrc_set_hsdpa_status

DESCRIPTION

  This function sets HSDPA Status in HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrc_set_hsdpa_status(tdsrrc_hsdpa_status_enum_type status)
{
  tdshsdpa_status = status;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:NEW HSDPA Status %d",status);
}

/*===========================================================================

FUNCTION tdsrrc_get_hsdpa_status

DESCRIPTION

  This function returns HSDPA Status from HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

tdsrrc_hsdpa_status_enum_type tdsrrc_get_hsdpa_status
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:HSDPA Status is %d.0= INACTIVE,1=ACTIVE,2=SUSPEND",tdshsdpa_status);
  return tdshsdpa_status;
}
#ifdef FEATURE_DUAL_WCDMA

/*===========================================================================

FUNCTION tdsrrc_get_hsdpa_status

DESCRIPTION

  This function returns HSDPA Status from HSDPA STATUS variable.
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

tdsrrc_hsdpa_status_enum_type tdsrrc_get_hsdpa_status_sub
(
  sys_modem_as_id_e_type as_id
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return tdsrrc_get_hsdpa_status();
}

/*===========================================================================

FUNCTION RRC_GET_HSUPA_STATUS

DESCRIPTION

  This function returns the HSUPA Status Variable.
  Created for DATA team using.
  
DEPENDENCIES


RETURN VALUE

  HSUPA Status 

SIDE EFFECTS

  None

===========================================================================*/

tdsrrc_hsupa_status_enum_type tdsrrc_get_hsupa_status_sub
(
  sys_modem_as_id_e_type as_id
)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return tdsrrc_get_hsupa_status();
}

#endif

/*===========================================================================

FUNCTION RRC_GET_HSUPA_STATUS

DESCRIPTION

  This function returns the HSUPA Status Variable.
  Created for DATA team using.
  
DEPENDENCIES


RETURN VALUE

  HSUPA Status 

SIDE EFFECTS

  None

===========================================================================*/

tdsrrc_hsupa_status_enum_type tdsrrc_get_hsupa_status
(
  void
)
{
#ifdef FEATURE_TDSCDMA_HSUPA
  if(tdsrrc_is_eul_active())
  {
    return TDSHSUPA_ACTIVE;
  }
  else
  {
    return TDSHSUPA_INACTIVE;
  }

#else
  return TDSHSUPA_INACTIVE;
#endif  

}




#ifdef FEATURE_TDS_DC_HSDPA
/*===========================================================================
FUNCTION tdsrrc_get_hsdpa_plus_status

DESCRIPTION
  This function returns DC HSDPA Status
  
DEPENDENCIES

RETURN VALUE
  DC HSDPA status of type tdsrrc_dchsdpa_status_enum_type

SIDE EFFECTS
  None
===========================================================================*/
tdsrrc_dchsdpa_status_enum_type tdsrrc_get_dchsdpa_status
(
  void
)
{
  tdsrrc_dchsdpa_status_enum_type dchsdpa_status = TDSDCHSDPA_INACTIVE;

  if((tdshsdpa_status == TDSHSDPA_ACTIVE) && 
    (tdsrrcllc_get_dc_hsdpa_status() == TDSRRC_DC_HSDPA_ACTIVE))
  {
    dchsdpa_status = TDSDCHSDPA_ACTIVE;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"HSDPA Status %d,DC-HSDPA Status %d",tdshsdpa_status,dchsdpa_status);
  return dchsdpa_status;
}
#endif/*FEATURE_TDS_DC_HSDPA*/


/*====================================================================
FUNCTION: tdsrrc_set_rlc_size_change_status()

DESCRIPTION:
  This function gets the RLC PDU Size per RB

DEPENDENCIES:
  None

RETURN VALUE:
  RLC Size

SIDE EFFECTS:
  None.
====================================================================*/
void tdsrrc_set_rlc_size_change_status(tdsrrc_proc_e_type proc_id, boolean rlc_size_change_status,
                                       tdsrrc_RB_Identity rb_id, tdsrlc_reestablish_e_type direction)
{
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"NewProc %d rlcsizestatus %d, OldProc %d",proc_id, rlc_size_change_status,
    tdsrlc_size_change_in_progress.proc_id);
  
  tdsrlc_size_change_in_progress.proc_id = proc_id;
  tdsrlc_size_change_in_progress.is_rlc_size_changed = rlc_size_change_status;
  tdsrlc_size_change_in_progress.rb_id = rb_id;
  tdsrlc_size_change_in_progress.direction = direction;
}

/*====================================================================
FUNCTION: tdsrrc_get_rlc_size_change_status_for_procedure_id()

DESCRIPTION:
  This function gets the RLC Size Change Status based on the Procedure Id

DEPENDENCIES:
  None

RETURN VALUE:
  RLC Size

SIDE EFFECTS:
  None.
====================================================================*/
boolean tdsrrc_get_rlc_size_change_status_for_procedure_id(tdsrrc_proc_e_type proc_id)
{
  if (tdsrlc_size_change_in_progress.proc_id == TDSRRC_PROCEDURE_NONE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Procedure Id is TDSRRC_PROCEDURE_NONE");
    return FALSE;
  }
  else if (proc_id == tdsrlc_size_change_in_progress.proc_id)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Procedure %d,rlcsizechangestatus %d match",proc_id,tdsrlc_size_change_in_progress.is_rlc_size_changed);

    return tdsrlc_size_change_in_progress.is_rlc_size_changed;
  }
  else
  {
     TDSRRC_MSG3(MSG_LEGACY_HIGH,"Procedure %d, RLC Size Proc Id %d, Status %d", proc_id,
             tdsrlc_size_change_in_progress.proc_id,tdsrlc_size_change_in_progress.is_rlc_size_changed);
     return tdsrlc_size_change_in_progress.is_rlc_size_changed;
  }
     
}

/*===========================================================================

FUNCTION tdsrrc_initialize_rlc_size_change

DESCRIPTION

  This function initializes the RLC Size Change structure
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_initialize_rlc_size_change
(
  void
)
{
  tdsrlc_size_change_in_progress.proc_id = TDSRRC_PROCEDURE_NONE;
  tdsrlc_size_change_in_progress.is_rlc_size_changed = FALSE;
  tdsrlc_size_change_in_progress.direction = TDSRLC_RE_ESTABLISH_NONE;
  tdsrlc_size_change_in_progress.rb_id = TDSRRC_INVALID_RB_ID;
}

/*===========================================================================

FUNCTION tdsrrc_initialize_rlc_size_per_rb

DESCRIPTION

  This function initializes the RLC Size per RB in the global structure
  tdsrlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  None
 

SIDE EFFECTS

  None

===========================================================================*/

void tdsrrc_initialize_rlc_size_per_rb
(
  void
)
{
  uint8 cnt = 0;
  tdsrlc_size_per_rb.no_of_rb = 0;
  for (cnt = 0; cnt < TDS_MAX_RB; cnt++)
  {
    tdsrlc_size_per_rb.rb_specified[cnt].rlc_dl_entity = TDSRRC_DL_INVALID_RLC;
    tdsrlc_size_per_rb.rb_specified[cnt].rb_id = TDSRRC_INVALID_RB_ID;
    tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size_present = FALSE;
     tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size = TDSINVALID_DL_AM_PDU_RLC_SIZE;

    tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_li_size_present = FALSE;
    tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size = TDSRRC_DL_UM_INVALID_RLC;

  }

  tdsps_rb_re_establish_indicator = FALSE;

}


/*===========================================================================

FUNCTION tdsrrc_get_dl_am_rlc_pdu_size

DESCRIPTION

  This function finds the DL AM RLC PDU Size as per the RB specified from the 
  global structure tdsrlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  DL AM RLC PDU Size if found else TDSINVALID_DL_AM_PDU_RLC_SIZE
 

SIDE EFFECTS

  None

===========================================================================*/

uint32 tdsrrc_get_dl_am_rlc_pdu_size
(
  tdsrrc_RB_Identity rb_id
)
{
  uint32 pdu_size = TDSINVALID_DL_AM_PDU_RLC_SIZE;
  /* Check if rb present in tdsrlc_size_per_rb */
  uint8 cnt = 0;
  for (cnt = 0; cnt < tdsrlc_size_per_rb.no_of_rb; cnt++)
  {
    if ((tdsrlc_size_per_rb.rb_specified[cnt].rb_id == rb_id) &&
        (tdsrlc_size_per_rb.rb_specified[cnt].rlc_dl_entity == TDSRRC_DL_AM_RLC) &&
        (tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size_present == TRUE) &&
        (tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.
         dl_rlc_am_pdu_size != TDSINVALID_DL_AM_PDU_RLC_SIZE))
    {
      TDSRRC_MSG2(MSG_LEGACY_MED,"RRC:Found RB %d,RLC DL AM PDU Size %d",rb_id,tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size); 
      return tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRC:Not found RLC DL AM Size RB %d",rb_id);
  return pdu_size;

}


/*===========================================================================

FUNCTION tdsrrc_get_dl_um_li

DESCRIPTION

  This function finds the DL UM LI as per the RB specified from the 
  global structure tdsrlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  DL UM LI Size if found else TDSINVALID_DL_AM_PDU_RLC_SIZE
 

SIDE EFFECTS

  None

===========================================================================*/

tdsrrc_rlc_dl_um_li_e_type tdsrrc_get_dl_um_li
(
  tdsrrc_RB_Identity rb_id
)
{
  tdsrrc_rlc_dl_um_li_e_type um_li = TDSRRC_DL_UM_INVALID_RLC;
  /* Check if rb present in tdsrlc_size_per_rb */
  uint8 cnt = 0;
  for (cnt = 0; cnt < tdsrlc_size_per_rb.no_of_rb; cnt++)
  {
    if ((tdsrlc_size_per_rb.rb_specified[cnt].rb_id == rb_id) &&
        (tdsrlc_size_per_rb.rb_specified[cnt].rlc_dl_entity == TDSRRC_DL_UM_RLC) &&
        (tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_li_size_present == TRUE) &&
        (tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size
         != TDSRRC_DL_UM_INVALID_RLC))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:Found RB %d,RLC DL UM LI 0:7/1:15 %d",rb_id,tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size); 
      return tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Not found RLC DL UM LI for RB %d",rb_id);
  return um_li;

}

/*===========================================================================

FUNCTION tdsrrc_set_dl_am_rlc_pdu_size

DESCRIPTION

  This function sets the DL AM RLC PDU Size as per the RB specified in the 
  global structure tdsrlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  DL AM RLC PDU Size if found else TDSINVALID_DL_AM_PDU_RLC_SIZE
 

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrc_set_dl_am_rlc_pdu_size
(
  tdsrrc_RB_Identity rb_id,
  uint32 dl_rlc_am_pdu_size
)
{
  /* Check if rb present in tdsrlc_size_per_rb */
  uint8 cnt = 0;
  
  TDSRRC_MSG1(MSG_LEGACY_MED,"RRCHS:Set RB %d in tdsrlc_size_per_rb",rb_id);
  for (cnt = 0; cnt < TDS_MAX_RB; cnt++)
  {
    if (tdsrlc_size_per_rb.rb_specified[cnt].rb_id == TDSRRC_INVALID_RB_ID)
    {
      /* Set RB Id & RLC Entity */
      tdsrlc_size_per_rb.rb_specified[cnt].rb_id = rb_id;
      tdsrlc_size_per_rb.rb_specified[cnt].rlc_dl_entity = TDSRRC_DL_AM_RLC;
      tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size_present = TRUE;
      tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size = dl_rlc_am_pdu_size;
#ifdef FEATURE_WCDMA_STORE_AM_RLC_SIZE_FROM_OTA
      tdsrrcllc_set_am_dl_rlc_size_ota(rb_id,dl_rlc_am_pdu_size,TDSRRC_FIXED_PDU_SIZE_SET);     
#endif
      tdsrlc_size_per_rb.no_of_rb++;
      
      
      TDSRRC_MSG2(MSG_LEGACY_MED,"RRCHS:RB %d,RLC DL AM PDU Size %d set",rb_id,tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_am_pdu_info.dl_rlc_am_pdu_size); 
      return SUCCESS;
    }
  }
  
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Cannot set RLC DL AM PDU Size for RB %d",rb_id);
  return FAILURE;

}


/*===========================================================================

FUNCTION tdsrrc_set_dl_um_li

DESCRIPTION

  This function sets the DL UM LI indication as per the RB specified in the 
  global structure tdsrlc_size_per_rb
  
DEPENDENCIES


RETURN VALUE
  Success/Failure
 

SIDE EFFECTS

  None

===========================================================================*/

uecomdef_status_e_type tdsrrc_set_dl_um_li
(
  tdsrrc_RB_Identity rb_id,
  tdsrrc_DL_UM_RLC_LI_size dl_rlc_um_li
)
{
  uint8 cnt = 0;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCHS:Set RB %d in tdsrlc_size_per_rb for DL UM L1",rb_id);
  for (cnt = 0; cnt < TDS_MAX_RB; cnt++)
  {
    if (tdsrlc_size_per_rb.rb_specified[cnt].rb_id == TDSRRC_INVALID_RB_ID)
    {
      /* Set RB Id & RLC Entity */
      tdsrlc_size_per_rb.rb_specified[cnt].rb_id = rb_id;
      tdsrlc_size_per_rb.rb_specified[cnt].rlc_dl_entity = TDSRRC_DL_UM_RLC;
      tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_li_size_present = TRUE;

      tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size = 
        (dl_rlc_um_li == tdsrrc_DL_UM_RLC_LI_size_size7) ? TDSRRC_DL_UM_RLC_LI_SIZE_7: TDSRRC_DL_UM_RLC_LI_SIZE_15;

      tdsrlc_size_per_rb.no_of_rb++;
      
      
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRCHS:RB %d,DL RLC UM LI %d set in tdsrlc_size_per_rb",rb_id,tdsrlc_size_per_rb.rb_specified[cnt].u.dl_rlc_um_pdu_info.dl_rlc_um_rlc_l1_size); 
      return SUCCESS;
    }
  }
  
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"RRCHS:Cannot set DL RLC UM LI for RB %d",rb_id);
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
uecomdef_status_e_type tdsrrc_translate_hrnti
(
  ASN1BitStr32  *ext_hrnti,          /* Pointer to external HRNTI type */
  uint16  *int_hrnti    /* Pointer to internal HRNTI type */
)
{
  /* Make sure the internal and external pointers are initialized */
  if( ext_hrnti == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"External HRNTI not initialized");
    return FAILURE;
  }
  
  if(ext_hrnti->numbits!= 16 )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received HRNTI Length: %d is invalid",ext_hrnti->numbits);
    return FAILURE;
  }

  /* Convert 16 bit HRNTI into internal format */
  *int_hrnti = *(ext_hrnti->data) << 8;
  *int_hrnti |= (uint16)*(ext_hrnti->data + 1);

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
uecomdef_status_e_type tdsrrc_hsdpa_initial_validation
(    
  tdsrrc_RRC_StateIndicator rrc_state_from_msg,
   boolean rrc_mode_Specific_Info_fdd_chosen
)
{
  uecomdef_status_e_type status = SUCCESS;
  TDSRRC_MSG2(MSG_LEGACY_LOW,"RRC State %d Msg State %d",tdsrrc_get_state(),rrc_state_from_msg);
  switch (tdsrrc_get_state())
  {
    case TDSRRC_STATE_CELL_FACH:
    case TDSRRC_STATE_CELL_DCH:
      if(rrc_mode_Specific_Info_fdd_chosen)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD choice in HSPDSCH");
        status = FAILURE;
      }
      
      break;
  
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
    default:
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid state for HSDPA check");
      status = FAILURE;
      break;
      
  } /* End of switch */
  
  return status;
}



/*===========================================================================

FUNCTION   tdsrrc_update_rab_type_in_rab_info_r7

DESCRIPTION
 This function is updates the rab type in 'tdsrrc_est_rab_info_type' structure
 using information stored in ASN1 'tdsrrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'tdsrrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  tdsrrc_RAB_InformationSetup *: pointer to ASN1 structure,
  tdsrrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

tdsrrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_e_type tdsrrc_update_rab_type_in_rab_info_r7
(
  tdsrrc_RAB_InformationSetup_r7 *rab_info_setup_ptr,
  tdsrrc_est_rab_info_type *est_rab_info_ptr 
)
{
  tdsrrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (tdsrrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = TDSRRCRB_PS_DATA_RAB;
  }
  else if (tdsrrc_CN_DomainIdentity_cs_domain == cn_domain)
  {

    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = tdsrrcrb_translate_gsm_map_rab_id(rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
      
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
    mn_rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MN RAB type %d",mn_rab_type);//auto-gen, to change remove '' and first param

    /* Check if it is in loopback test - do we really need this check ? */
    if ( tdsrrclbt_is_lb_on() == TRUE )
    {
      /* Set Rab type to Data */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB setup for Loopback Test");
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)    
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_VOICE_RAB ;
    }
    else
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_UNKNOWN_RAB;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cn_domain value: %d",cn_domain);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Cn domain %d",cn_domain);
  }  
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
            est_rab_info_ptr->rab_type, TDSRRCRB_CS_VOICE_RAB, TDSRRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}


/*===========================================================================
FUNCTION   tdsrrc_is_ps_domain_rab_present

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
boolean tdsrrc_is_ps_domain_rab_present
(
  void
)
{
  uint8 rab_idx = 0;

  for(rab_idx = 0; rab_idx < MAX_RAB_TO_SETUP; rab_idx++)
  {
    if((tdsrrc_est_rabs.rabs[rab_idx].rab_id != TDSRRC_INVALID_RAB_ID) && 
        (tdsrrc_est_rabs.rabs[rab_idx].cn_domain == RRC_PS_DOMAIN_CN_ID))
    {
      return TRUE;
    }
  }
  return FALSE;
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
uecomdef_status_e_type tdsrrc_hsdpa_initial_validation_r6
(    
  tdsrrc_RRC_StateIndicator rrc_state_from_msg,
  tdsrrc_DL_HSPDSCH_Information_r6* hspdsch_info_ptr
)
{
  uecomdef_status_e_type status = SUCCESS;
  TDSRRC_MSG2(MSG_LEGACY_LOW,"RRC State %d Msg State %d",tdsrrc_get_state(),rrc_state_from_msg);
  switch (tdsrrc_get_state())
  {
  case TDSRRC_STATE_CELL_FACH:
  case TDSRRC_STATE_CELL_DCH:
    if (hspdsch_info_ptr->modeSpecificInfo.t == T_tdsrrc_DL_HSPDSCH_Information_r6_modeSpecificInfo_fdd)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:FDD choice in HSPDSCH");
      status = FAILURE;
    }
    
    break;

  case TDSRRC_STATE_CELL_PCH:
  case TDSRRC_STATE_URA_PCH:
  default:
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"RRCHS:Invalid state for HSDPA check");
    status = FAILURE;
    break;
    
  } /* End of switch */
  
  return status;
}

/*===========================================================================

FUNCTION   tdsrrc_update_rab_type_in_rab_info_r6

DESCRIPTION
 This function is updates the rab type in 'tdsrrc_est_rab_info_type' structure
 using information stored in ASN1 'tdsrrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'tdsrrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  tdsrrc_RAB_InformationSetup *: pointer to ASN1 structure,
  tdsrrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

tdsrrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_e_type tdsrrc_update_rab_type_in_rab_info_r6
(
  tdsrrc_RAB_InformationSetup_r6 *rab_info_setup_ptr,
  tdsrrc_est_rab_info_type *est_rab_info_ptr 
)
{
  tdsrrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (tdsrrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = TDSRRCRB_PS_DATA_RAB;
  }
  else if (tdsrrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = tdsrrcrb_translate_gsm_map_rab_id(
    rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
      
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
    mn_rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if ( tdsrrclbt_is_lb_on() == TRUE )
    {
      /* Set Rab type to Data */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RAB setup for Loopback Test");
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)    
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB ;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL) 
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_UNKNOWN_RAB;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cn_domain value: %d",cn_domain);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Cn domain %d",cn_domain);
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
            est_rab_info_ptr->rab_type, TDSRRCRB_CS_VOICE_RAB, TDSRRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}


#ifdef FEATURE_TDSCDMA_HSUPA
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
uecomdef_status_e_type tdsrrc_translate_ernti
(
  ASN1BitStr32  *ext_ernti,          /* Pointer to external ERNTI type */
  uint16  *int_ernti    /* Pointer to internal ERNTI type */
)
{
  /* Make sure the internal and external pointers are initialized */
  if( ext_ernti == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"External ERNTI not initialized");
    return FAILURE;
  }
  
  if(ext_ernti->numbits!= 16 )
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Received ERNTI Length: %d is invalid",ext_ernti->numbits);
    return FAILURE;
  }

  /* Convert 16 bit ERNTI into internal format */
  *int_ernti = *(ext_ernti->data) << 8;
  *int_ernti |= *(ext_ernti->data+ 1);

  return SUCCESS;
}

#endif /* FEATIRE_WCDMA_HSUPA*/

#if 0
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
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
boolean is_ctch_present_in_serving_cell(tdsrrc_state_e_type next_state)
{
  tdsrrc_SysInfoType5    *sib5_ptr = NULL;
  boolean ans = FALSE;      /* Return Value */
  boolean sib5_in_use = TRUE;  /* indicates whether sib 5 or 6 is in use */
  uint32 K = 0;      /* CBS frame offset */
  uint32 N = 0;     /* period of CTCH allocation */

  
  sib5_ptr = (tdsrrc_SysInfoType5 *)
             tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB5);
  if (sib5_ptr == NULL)
  {
    ERR_FATAL("SIB 5 not yet in SIB data base",0,0,0);
    return FALSE;
  }
  
  if (next_state == TDSRRC_STATE_DISCONNECTED || next_state == TDSRRC_STATE_CELL_PCH || next_state == TDSRRC_STATE_URA_PCH)
  {
    /* according to 25.331 if any one of the fach in the sib-5 list has CTCH then cbs_drx 
    must be present */
    if(TDSRRCCHECK_SIB_MSG_IE_PRESENT(sib5_ptr,tdsrrc_SysInfoType5,cbs_DRX_Level1Information))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CBS DRX info present in curr cell. next_state %d",next_state);
      ans = TRUE;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CTCH not present in curr cell. next_state %d",next_state);
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
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid N:%d and K:%d in CBS DRX info. %d",N,K,sib5_in_use);
      ans = FALSE;
    }
  }

  return ans;
}
#endif
#endif

/*===========================================================================

FUNCTION tdsrrc_get_lac_id 

DESCRIPTION
  This function returns lac id for the serving cell.

  
DEPENDENCIES
  None

RETURN VALUE
  SUCCESS or FAILURE
  
SIDE EFFECTS

  None

===========================================================================*/
uecomdef_status_e_type tdsrrc_get_lac_id(rrc_lai_type *lai) 
{

  /* Pointer to SIB 5*/
  tdsrrc_SysInfoType1    *sib1_ptr = NULL;

  /* This stores the GSM-MAP NAS system info */
  rrc_gsm_map_nas_sys_info_type   nas_common_info;

  unsigned char  *tdsdata_ptr;
  uint32         i;

  sib1_ptr = (tdsrrc_SysInfoType1 *)
      tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB1);

  if( sib1_ptr == NULL )
  {
    /* SIB3 for this PLMN, CEll ID combination not present */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB1 not present. Returning");
    return(FAILURE);
  } /* SIB1 not present */


  /* The SIB1 is present */
  nas_common_info.length = (uint32)sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts;
  /* Check if the RRC MM interface structure array can hold this info */
  if(sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts > RRC_MAX_GSM_INFO_OCTET_COUNT)
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for Common NAS Sys Info (%d bytes)",sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts);
    return(FAILURE);
  }

  /* Fill in the Common NAS system info now */
  tdsdata_ptr = sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.data;
  for( i = 0; i < sib1_ptr->cn_CommonGSM_MAP_NAS_SysInfo.numocts; i ++)
  {
    nas_common_info.value[i] =  *tdsdata_ptr;
    tdsdata_ptr ++;
  }

  /* Get the LAI status from local utility */
  /* TBD: Call MM utility to initialize lai first. */
  if(mm_extract_lac_from_sib_info(lai, &nas_common_info) == FALSE)
  {
    ERR_FATAL("Could not extract LAI from NAS Common sys info",0,0,0);
  }

  return (SUCCESS);
 
}


/*==========================================================================

FUNCTION tdsrrc_translate_establishment_cause

DESCRIPTION
  This function gets equivalent external RRC Connection Establishment cause
  for the internal RRC Connection Established cause received from the NAS 
  through TDSRRC_EST_REQ primitive.
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void  tdsrrc_translate_establishment_cause
(
   rrc_establish_cause_e_type est_cause_int,
   tdsrrc_EstablishmentCause *est_cause_ext
   
)
{
   tdsrrc_EstablishmentCause rrc_establishment_cause_ext;
                                      /* RRC Connection Establishment cause
                                         to be sent over the air */
  switch (est_cause_int)
  {
    case RRC_EST_ORIG_CONV_CALL:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_originatingConversationalCall;
      break;
    case RRC_EST_ORIG_STREAM_CALL:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_originatingStreamingCall;
      break;
    case RRC_EST_ORIG_INTERACT_CALL:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_originatingInteractiveCall;
      break;
    case RRC_EST_ORIG_BACKGND_CALL:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_originatingBackgroundCall;
      break;
    case RRC_EST_ORIG_SUBSCRIBED_TRF_CALL:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_originatingSubscribedTrafficCall;
      break;
    case RRC_EST_TERM_CONV_CALL:
      rrc_establishment_cause_ext = 
          tdsrrc_EstablishmentCause_terminatingConversationalCall;
      break;
    case RRC_EST_TERM_STREAM_CALL:
      rrc_establishment_cause_ext = 
          tdsrrc_EstablishmentCause_terminatingStreamingCall;
      break;
    case RRC_EST_TERM_INTERACT_CALL:
      rrc_establishment_cause_ext = 
          tdsrrc_EstablishmentCause_terminatingInteractiveCall;
      break;
    case RRC_EST_TERM_BACKGND_CALL:
      rrc_establishment_cause_ext = 
          tdsrrc_EstablishmentCause_terminatingBackgroundCall;
      break;
    case RRC_EST_EMERGENCY_CALL:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_emergencyCall;
      break;
    case RRC_EST_INTER_RAT_CELL_RESELECT:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_interRAT_CellReselection;
      break;
    case RRC_EST_INTER_RAT_CELL_CHANGE_ORDER:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_interRAT_CellChangeOrder;
      break;
    case RRC_EST_REGISTRATION:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_registration;
      break;
    case RRC_EST_DETACH:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_detach;
      break;
    case RRC_EST_HI_PRI_SIGNALLING:
      rrc_establishment_cause_ext = 
        tdsrrc_EstablishmentCause_originatingHighPrioritySignalling;
      break;
    case RRC_EST_LOW_PRI_SIGNALLING:
      rrc_establishment_cause_ext = 
        tdsrrc_EstablishmentCause_originatingLowPrioritySignalling;
      break;
    case RRC_EST_CALL_RE_ESTABLISH:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_callRe_establishment;
      break;
    case RRC_EST_TERM_HI_SIGNALLING:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_terminatingHighPrioritySignalling;
      break;
    case RRC_EST_TERM_LOW_SIGNALLING:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_terminatingLowPrioritySignalling;
      break;
    case RRC_EST_TERM_UNKNOWN:
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_terminatingCauseUnknown;
      break;
    default:
      /* rrc_establishment_cause_ext = tdsrrc_establishment_cause; */
      /* The above was changed to stop Lint from complaining. The following would never get executed */
      rrc_establishment_cause_ext = tdsrrc_EstablishmentCause_spare1;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid RRC Conn Est cause: %d",tdsrrc_establishment_cause);
      break;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"mm cause %d,translated cause %d",est_cause_int,rrc_establishment_cause_ext);
  *est_cause_ext = rrc_establishment_cause_ext;
  return;

} /* tdsrrc_translate_establishment_cause */




/*====================================================================
FUNCTION: tdsrrc_translate_n313

DESCRIPTION:
  This function translates the enum for timer N313 into an int16 value.

DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
uint16 tdsrrc_translate_n313(tdsrrc_N_313 val)
{
  switch (val)
  {
    case tdsrrc_N_313_s1:
      return 1;
    case tdsrrc_N_313_s2:
      return 2;
    case tdsrrc_N_313_s4:
      return 4;
    case tdsrrc_N_313_s10:
      return 10;
    case tdsrrc_N_313_s20:
      return 20;
    case tdsrrc_N_313_s50:
      return 50;
    case tdsrrc_N_313_s100:
      return 100;
    case tdsrrc_N_313_s200:
      return 200;
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid N313 = %d",val);
      return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
  }
} /* tdsrrc_translate_n313 */


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
uint32 tdsrrc_get_downlink_rb_rate(uint8 lc_id)
{

  uint32 rate = 0;
  uint8 rb_id = 0;

  rb_id = tdsrrclcm_get_dl_rb_id(lc_id);

  if (rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED) 
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Wrong LC_ID passed in %d",lc_id);
  }
  else
  {
    rate = tdsrrcllc_get_dl_ded_rate(tdsrrc_get_state(), rb_id);
    if(rate == 0)
    {      
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"downlink rate for rb_id %d is set to 384000",rb_id);
      rate = 384000;
    }
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"downlink rate for rb_id %d is %d",rb_id,rate);
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
uint32 tdsrrc_get_uplink_rb_rate(uint8 lc_id)
{
  uint32 rate = 0;
  uint8 rb_id = 0;

  rb_id = tdsrrclcm_get_ul_rb_id(lc_id);

  if (rb_id == TDSRRCLCM_RB_ID_NOT_ALLOCATED) 
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Wrong LC_ID passed in %d",lc_id);
  }
  else
  {
    rate = tdsrrcllc_get_ul_ded_rate(tdsrrc_get_state(),  rb_id);
    if(rate == 0)
    {      
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"uplink rate for rb_id %d is set to 384000",rb_id);
      rate = 384000;
    }
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"uplink rate for rb_id %d is %d",rb_id,rate);
  return rate;
}

/*====================================================================
FUNCTION: is_tds_active

DESCRIPTION:
  This function returns true of W is active, meaning MCM tdsrrc_mode
  is  returns FALSE otherwise

DEPENDENCIES:
  None

RETURN VALUE:
  uint32

SIDE EFFECTS:
====================================================================*/
boolean rrc_is_tds_active(void)
{

  tdsrrc_mode_e_type mcm_rrc_mode = tdsrrcmcm_get_rrc_mode();

  if (mcm_rrc_mode == TDSRRC_MODE_ACTIVE) 
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}
#ifdef FEATURE_DUAL_WCDMA

/*====================================================================
FUNCTION: is_tds_active

DESCRIPTION:
  This function returns true of W is active, meaning MCM tdsrrc_mode
  is  returns FALSE otherwise

DEPENDENCIES:
  None

RETURN VALUE:
  uint32

SIDE EFFECTS:
====================================================================*/
boolean rrc_is_tds_active_sub(sys_modem_as_id_e_type as_id)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return rrc_is_tds_active();
}
#endif
/*====================================================================
FUNCTION: tdsrrc_update_constants_n312_and_n315_r5

DESCRIPTION:
  This function updates the global constants for N312 & N315 for R5.
  
DEPENDENCIES:
  None

RETURN VALUE:
  uint16 value corresponding to the enum passed, 0 if invalid

SIDE EFFECTS:
====================================================================*/
uint16 tdsrrc_update_constants_n312_and_n315_r5
(
tdsrrc_UE_ConnTimersAndConstants_r5 *conn_timers
)
{
  if(conn_timers != NULL)
  {
    if(TDSRRCCHECK_TIMER_CONST_PRESENT_R5(conn_timers,rrc_UE_ConnTimersAndConstants_r5_,t_301))
    { 
      switch (conn_timers->n_312)
      { 
      case tdsrrc_N_312_r5_s1:
        tdsconn_timers_and_constants.n312 =  1;
        break;
      case tdsrrc_N_312_r5_s2:
        tdsconn_timers_and_constants.n312 =  2;
        break;
      case tdsrrc_N_312_r5_s4:
        tdsconn_timers_and_constants.n312 =  4;
        break;
      case tdsrrc_N_312_r5_s10:
        tdsconn_timers_and_constants.n312 =  10;
        break;
      case tdsrrc_N_312_r5_s20:
        tdsconn_timers_and_constants.n312 =  20;
        break;
      case tdsrrc_N_312_r5_s50:
        tdsconn_timers_and_constants.n312 =  50;
        break;
      case tdsrrc_N_312_r5_s100:
        tdsconn_timers_and_constants.n312 =  100;
        break;
      case tdsrrc_N_312_r5_s200:
        tdsconn_timers_and_constants.n312 =  200;
        break;
      case tdsrrc_N_312_r5_s400:
        tdsconn_timers_and_constants.n312 =  400;
        break;
      case tdsrrc_N_312_r5_s600:
        tdsconn_timers_and_constants.n312 =  600;
        break;
      case tdsrrc_N_312_r5_s800:
        tdsconn_timers_and_constants.n312 =  800;
        break;
      case tdsrrc_N_312_r5_s1000:
        tdsconn_timers_and_constants.n312 =  1000;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid N312 = %d",conn_timers->n_312);
        return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
      }  
    } 

    if(TDSRRCCHECK_TIMER_CONST_PRESENT_R5(conn_timers,rrc_UE_ConnTimersAndConstants_r5_,n_315))
    { 
      switch(conn_timers->n_315)
      { 
      case tdsrrc_N_315_r5_s1:
        tdsconn_timers_and_constants.n315 = 1;
        break;
      case tdsrrc_N_315_r5_s2:
        tdsconn_timers_and_constants.n315=  2;
        break;
      case tdsrrc_N_315_r5_s4:
        tdsconn_timers_and_constants.n315 = 4;
        break;
      case tdsrrc_N_315_r5_s10:
        tdsconn_timers_and_constants.n315 = 10;
        break;
      case tdsrrc_N_315_r5_s20:
        tdsconn_timers_and_constants.n315 = 20;
        break;
      case tdsrrc_N_315_r5_s50:
        tdsconn_timers_and_constants.n315 = 50;
        break;
      case tdsrrc_N_315_r5_s100:
        tdsconn_timers_and_constants.n315 = 100;
        break;
      case tdsrrc_N_315_r5_s200:
        tdsconn_timers_and_constants.n315 = 200;
        break;
      case tdsrrc_N_315_r5_s400:
        tdsconn_timers_and_constants.n315 = 400;
        break;
      case tdsrrc_N_315_r5_s600:
        tdsconn_timers_and_constants.n315 = 600;
        break;
      case tdsrrc_N_315_r5_s800:
        tdsconn_timers_and_constants.n315 = 800;
        break;
      case tdsrrc_N_315_r5_s1000:
        tdsconn_timers_and_constants.n315 = 1000;
        break;
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid N315 = %d",conn_timers->n_315);
        return 0; /*lint !e527  This default case is flagged unreachable, since
                              we cover all the enum values in the case
                              clauses, but we want it here anyway for extra
                              safety. */
      } 
    } 
  } 

  return 0;
} /* tdsrrc_update_constants_n312_and_n315_r5 */

/*===========================================================================

FUNCTION tdsrrc_print_supported_asn1_rel_version

DESCRIPTION
  This  function prints the ASN1 release version supported by the UE as per 
  NV item "NV_WCDMA_RRC_VERSION_I" i.e. global variable 'tdsrrc_nv_rel_indicator'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrc_print_supported_asn1_rel_version(void)
{
  /* since ASN1 is back-ward compatible, hence if Rel-6 is set then it indicates that 
     Rel-5 and R99 is automatically supported */
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"NV rrcVersion (value: %d) indicates rrcASN1 Rel-6: %d, Rel-5: %d and R4 support",
    tdsrrc_nv_rel_indicator,
    (tdsrrc_nv_rel_indicator&TDSRRC_NV_REL_INDICATOR_REL6)>>1,
    (tdsrrc_nv_rel_indicator&TDSRRC_NV_REL_INDICATOR_REL5));
}

/*===========================================================================

FUNCTION tdsrrc_get_tds_rrc_version_nv

DESCRIPTION
  This  function gets the NV item TDS_RRC_NV_RRC_VERSION and stores it in 
  rrc global variable 'tdsrrc_nv_rel_indicator'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrc_get_tds_rrc_version_nv
(
void
)
{
  uint32 local_rrc_rel_indicator = 0;

/*Initialize tdsrrc_nv_rel_indicator as per the highest release feature defined. 
It will be overwritten if RRC version is read from NV. So basically it will be used by UTF.*/
#ifdef FEATURE_TDSCDMA_REL9
      tdsrrc_nv_rel_indicator =
    TDSRRC_NV_REL_INDICATOR_REL5|TDSRRC_NV_REL_INDICATOR_REL6|
    TDSRRC_NV_REL_INDICATOR_REL7|TDSRRC_NV_REL_INDICATOR_REL8|
    TDSRRC_NV_REL_INDICATOR_REL9;

#else
    tdsrrc_nv_rel_indicator =
  TDSRRC_NV_REL_INDICATOR_REL5|TDSRRC_NV_REL_INDICATOR_REL6|TDSRRC_NV_REL_INDICATOR_REL7;
  
  #endif

  /*read tds rrc release indicator from NV*/
  if (tdsrrcnv_efs_read(TDS_RRC_NV_RRC_VERSION, 
                        (byte *) &local_rrc_rel_indicator,
                        sizeof(local_rrc_rel_indicator)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_RRC_VERSION is %d",local_rrc_rel_indicator);
  }
  else /*NV couldn't be read successfully.*/
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_RRC_VERSION,
                                   (byte *) &local_rrc_rel_indicator,
                                   sizeof(local_rrc_rel_indicator),
                                   TRUE))
    {
       /* Mismatch of NV item */
       local_rrc_rel_indicator = 2; /* value 2 indicates REL7, and default value is for REL7 */
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_RRC_VERSION item error. Setting to default: %d",local_rrc_rel_indicator);
    }

    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Could Not Read TDS_RRC_NV_RRC_VERSION. Setting to default: %d",local_rrc_rel_indicator);
  }
  
  /* use stored nv variable value to set rrc global variable */
  switch (local_rrc_rel_indicator)
  {
    case 0:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set tdsrrc_nv_rel_indicator to R4");
      tdsrrc_nv_rel_indicator = TDSRRC_NV_REL_INDICATOR_REL4;
      break;
      
    case 1:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set tdsrrc_nv_rel_indicator to REL-5");
      tdsrrc_nv_rel_indicator = TDSRRC_NV_REL_INDICATOR_REL4|TDSRRC_NV_REL_INDICATOR_REL5;
      break;
      
    case 2:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set tdsrrc_nv_rel_indicator to REL-7");
      tdsrrc_nv_rel_indicator = TDSRRC_NV_REL_INDICATOR_REL4|TDSRRC_NV_REL_INDICATOR_REL5|TDSRRC_NV_REL_INDICATOR_REL6|TDSRRC_NV_REL_INDICATOR_REL7;
      break;

#ifdef FEATURE_TDSCDMA_REL9
    case 3:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Set tdsrrc_nv_rel_indicator to REL-9");
      tdsrrc_nv_rel_indicator = TDSRRC_NV_REL_INDICATOR_REL4|TDSRRC_NV_REL_INDICATOR_REL5|TDSRRC_NV_REL_INDICATOR_REL6|TDSRRC_NV_REL_INDICATOR_REL7|TDSRRC_NV_REL_INDICATOR_REL8|TDSRRC_NV_REL_INDICATOR_REL9;
      break;
#endif /*FEATURE_TDSCDMA_REL9*/

    default:
      tdsrrc_nv_rel_indicator = TDSRRC_NV_REL_INDICATOR_REL4|TDSRRC_NV_REL_INDICATOR_REL5|TDSRRC_NV_REL_INDICATOR_REL6|TDSRRC_NV_REL_INDICATOR_REL7;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"In default: Set tdsrrc_nv_rel_indicator to %d",tdsrrc_nv_rel_indicator);
      break;
  }
}

/*===========================================================================

FUNCTION tdsrrc_get_3gpp_release_version_ext

DESCRIPTION

  This function returns the 3gpp release version.
  
DEPENDENCIES


RETURN VALUE

  tdsrrc_rel_version_enum  

SIDE EFFECTS

  None

===========================================================================*/
tdsrrc_rel_version_enum tdsrrc_get_3gpp_release_version_ext()
{
  if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9)
  {
    return TDSRRC_REL_VERSION_9;
  }
  else if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8)
  {
    return TDSRRC_REL_VERSION_8;
  }
  else if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7)
  {
    return TDSRRC_REL_VERSION_7;
  }
  else if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    return TDSRRC_REL_VERSION_6;
  }
  else if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
  {
    return TDSRRC_REL_VERSION_5;
  }
  else if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL4)
  {
    return TDSRRC_REL_VERSION_4;
  }
  else
  {
    return TDSRRC_REL_VERSION_7;
  }
}


#ifdef FEATURE_UMTS_PDCP
/*===========================================================================

FUNCTION tdsrrc_get_tds_rrc_pdcp_disabled_nv

DESCRIPTION
  This  function gets the NV item NV_WCDMA_RRC_PDCP_DISABLED_I and stores it in 
  rrc global variable 'tdsrrc_pdcp_enabled'

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrc_get_tds_rrc_pdcp_disabled_nv
(
void
)
{
  boolean local_rrc_pdcp_disabled_ind = TRUE;
  tdsrrc_pdcp_enabled = FALSE;

  if (tdsrrcnv_efs_read(TDS_RRC_NV_RRC_PDCP_DISABLED, 
                        (byte *) &local_rrc_pdcp_disabled_ind,
                        sizeof(local_rrc_pdcp_disabled_ind)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_RRC_PDCP_DISABLED is %d",local_rrc_pdcp_disabled_ind);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_RRC_PDCP_DISABLED,
	    						   (byte *) &local_rrc_pdcp_disabled_ind,
								   sizeof(local_rrc_pdcp_disabled_ind),
								   TRUE))
    {

      local_rrc_pdcp_disabled_ind = FALSE;
	  TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_RRC_PDCP_DISABLED item error. Setting to: %d",local_rrc_pdcp_disabled_ind);
    }
	TDSRRC_MSG1(MSG_LEGACY_ERROR,"Could Not Read TDS_RRC_NV_RRC_PDCP_DISABLED,setting to %d",local_rrc_pdcp_disabled_ind);
  }
  
  /* use stored nv variable value to set rrc global variable */
  if (!local_rrc_pdcp_disabled_ind)
  {
    tdsrrc_pdcp_enabled = TRUE;
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Set tdsrrc_pdcp_enabled to %d",tdsrrc_pdcp_enabled);

}
#endif /* FEATURE_UMTS_PDCP*/

/*===========================================================================

FUNCTION tdsrrc_get_amr_mode_bitrate

DESCRIPTION
  This  function takes amr mode as argument & returns the AMR rate category

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrc_amr_mode_bitrate_e_type.

===========================================================================*/
tdsrrc_amr_mode_bitrate_e_type tdsrrc_get_amr_mode_bitrate(uint32 amr_mode)
{
  tdsrrc_amr_mode_bitrate_e_type  amr_bitrate = TDSRRC_AMR_MODE_MAX;

  switch(amr_mode)
  {
  case TDSCDMA_IVOCAMR_CODEC_MODE_0475:
  case TDSCDMA_IVOCAMR_CODEC_MODE_0515:
  case TDSCDMA_IVOCAMR_CODEC_MODE_0590:
  case TDSCDMA_IVOCAMR_CODEC_MODE_0670:
  case TDSCDMA_IVOCAMR_CODEC_MODE_0740:
  case TDSCDMA_IVOCAMR_CODEC_MODE_0795:
    amr_bitrate = TDSRRC_AMR_MODE_LOW_BITRATE;
    break;

  case TDSCDMA_IVOCAMR_CODEC_MODE_1020:
  case TDSCDMA_IVOCAMR_CODEC_MODE_1220:
    amr_bitrate = TDSRRC_AMR_MODE_HIGH_BITRATE;
    break;

  default:
    break;
  }

  return amr_bitrate;
} /* tdsrrc_get_amr_mode_bitrate */

/*===========================================================================

FUNCTION tdsrrc_get_dl_amr_mode_given_rbs

DESCRIPTION
  This  function returns that if the mode has changed calculating rates 
  corresponding RB Ids.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

===========================================================================*/
boolean tdsrrc_get_dl_amr_mode_given_rbs
(
  uint32 *prev_mode, 
  uint32 *new_mode,
  tdsrrc_rb_info_for_amr_type *amr_rb_info_ptr
)
{
  boolean mode_changed = FALSE;
  uint32 i = 0;
  uint32 amr_mode = TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF;

  /* Stored RAB info should point to CS RAB to continue */
  if(amr_rb_info_ptr->rab_type == TDSRRCRB_CS_VOICE_RAB)
  {
    /* Get previous rate from current configuration */
    for(i =0; i < amr_rb_info_ptr->num_rbs_for_rab; i++)
    {
      /* Any meaningful AMR mode corresponding to Voice RAB (RB) is 
         implicitly treated as correct rate for the other RBs */
      if((amr_mode = tdsrrcllc_get_dl_amr_mode_for_rb(amr_rb_info_ptr->rb_id[i], tdscurrent_config_ptr)) != TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF)
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
      if((amr_mode = tdsrrcllc_get_dl_amr_mode_for_rb(amr_rb_info_ptr->rb_id[i], tdsordered_config_ptr)) != TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF)
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
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"This function shouldn't have been called w/o CS RAB");
  }
  
  return mode_changed;
} /* tdsrrc_get_dl_amr_mode_given_rbs */

//#if defined FEATURE_TDSCDMA_TRM
/*===========================================================================

FUNCTION tdsrrc_check_if_tds_can_be_stopped

DESCRIPTION
  This  function is called by L1 to check if TDS stack can be stopped ie PNR
  (Point of no return) has not been reached. If PNR has not been reached,
  the function calls  cm function to post a special stop to RRC and returns
  TRUE to L1. 

  If PNR has been reached the function returns FALSE.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

===========================================================================*/
boolean tdsrrc_check_if_tds_can_be_stopped
(
  void
)
{
  if (!tdsrrc_multimode_pt_of_no_return_reached)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PNR not reached stopping TDS stack");
    /* for 7200 only builds that use TRM need to add ERR_FATAL if rf resources 
     * are not granted to L1 */
    return TRUE;
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"PNR reached continue as normal");
  return FALSE;
}

//#endif

#if defined(FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM)
/*===========================================================================

FUNCTION tdsrrc_get_ordered_codec_status

DESCRIPTION
  This  function returns that the ordred codec mode status

DEPENDENCIES
  None.

RETURN VALUE
  ordered codec status

===========================================================================*/
boolean tdsrrc_get_ordered_codec_status
(
  void
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH," The Ordered codec status is returned is %d",tdsrrcdata_codec_info.ordered_codec_valid);
  return tdsrrcdata_codec_info.ordered_codec_valid;
}


/*===========================================================================

FUNCTION tdsrrc_set_ordered_codec_status

DESCRIPTION
  This  function sets the ordered codec status
DEPENDENCIES
  None.

RETURN VALUE
 None

===========================================================================*/
void tdsrrc_set_ordered_codec_status(boolean flag)
{
   tdsrrcdata_codec_info.ordered_codec_valid = flag;   
   TDSRRC_MSG1(MSG_LEGACY_HIGH," The Ordered codec status is set as %d", flag);
}
#endif

/*===========================================================================

FUNCTION tdsrrc_get_current_codec_mode

DESCRIPTION
  This  function returns that the Current codec mode type that is set 

DEPENDENCIES
  None.

RETURN VALUE
  uint32: return the Current codec mode type that is set 

===========================================================================*/
uint32 tdsrrc_get_current_codec_mode
(
  void
)
{
  return tdsrrcdata_codec_info.rrc_active_codec;
}


/*===========================================================================

FUNCTION tdsrrc_set_current_codec_mode

DESCRIPTION
  This  function sets the current codec mode with value configured in MVS
DEPENDENCIES
  None.

RETURN VALUE
 None

===========================================================================*/
void tdsrrc_set_current_codec_mode
(
  uint32 nas_sync_indicator
)
{
   tdsrrcdata_codec_info.rrc_active_codec = nas_sync_indicator;   
   TDSRRC_MSG1(MSG_LEGACY_HIGH," The current codec mode is set as %d",tdsrrcdata_codec_info.rrc_active_codec);
}

/*===========================================================================

FUNCTION  tdsrrc_set_ordered_codec_mode

DESCRIPTION This function sets the codec mode ordered in the ota messages

DEPENDENCIES
  None
  
RETURN VALUE          
  None

===========================================================================*/
void tdsrrc_set_ordered_codec_mode
(
  uint32 nas_sync_ind
)
{
  tdsrrcdata_codec_info.rrc_ota_codec_mode_status = nas_sync_ind;
}

/*===========================================================================

FUNCTION  tdsrrc_get_ordered_codec_mode

DESCRIPTION This function get the codec mode ordered in the ota messages

DEPENDENCIES
  None
  
RETURN VALUE          
  uint32: codec mode

===========================================================================*/
uint32 tdsrrc_get_ordered_codec_mode
(
  void
)
{
  return tdsrrcdata_codec_info.rrc_ota_codec_mode_status;
}



/*===========================================================================


FUNCTION  tdsrrc_translate_nas_sync_indicator

DESCRIPTION This function translates a Nas-Syncronisation-indicator defined by
 ASN.1 into the internal type used by RRC. 

DEPENDENCIES

  The algorithm used assumes that the processor is Little Endian.
  
RETURN VALUE
                  
  Nas-Syncronisation-indicator converted to the internal type tdsrrc_codec_types


===========================================================================*/
uint32 tdsrrc_translate_nas_sync_indicator
(
  ASN1BitStr32  *nas_sync_indicator         /* Pointer to external PLMN type */
)
{
  tdsrrc_codec_types codec_type = TDSRRC_UMTS_AMR;           
 
  if(nas_sync_indicator->numbits> 4 )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"nas_sync_indicator longer than 4 bits!");
    return( TDSCDMA_IVOCODER_ID_NONE);
  }

  codec_type = (tdsrrc_codec_types)((nas_sync_indicator->data[0] >> nas_sync_indicator->numbits)& 0x0F);
  TDSRRC_MSG1(MSG_LEGACY_HIGH," Nas sync indicator translated %d ",codec_type);
  if((codec_type == TDSRRC_UMTS_AMR) || (codec_type == TDSRRC_UMTS_AMR_2))
  {
    return(TDSCDMA_IVOCODER_ID_AMR);
  }
  else if(codec_type == TDSRRC_UMTS_AMR_WB)
  {
    return(TDSCDMA_IVOCODER_ID_AMRWB);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR," Unknown type of codec %d",nas_sync_indicator);
    return TDSCDMA_IVOCODER_ID_NONE;
  }
}

/*===========================================================================

FUNCTION  tdsrrc_debug_stats

DESCRIPTION This function increments the debug statistics

DEPENDENCIES
  None
  
RETURN VALUE
  None

===========================================================================*/
/*===========================================================================

FUNCTION  tdsrrc_debug_stats

DESCRIPTION This function increments the debug statistics

DEPENDENCIES
  None
  
RETURN VALUE
  None

===========================================================================*/
void tdsrrc_debug_stats
(
  tdsrrc_debug_stats_enum_type rrc_debug_cause
)
{
  switch(rrc_debug_cause)
  {
    case TDSINTRA_FREQ_CEL_RESEL_SUCCESS:
      tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_resel_success_cnt++;
      break;
    
    case TDSINTRA_FREQ_CEL_RESEL_NBCCH_SETUP_FAILURE:
      tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt++;
      break;
    
    case TDSINTRA_FREQ_CEL_RESEL_SIB_WAIT_TIMEOUT:
      tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt++;
      break;
    
    case TDSINTRA_FREQ_CEL_RESEL_TRANSITION_FAILURE:
      tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_transition_fail_cnt++;
      break;
    
    case TDSINTRA_FREQ_CEL_RESEL_FAILED_OTHER_CAUSE:
      tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_failed_other_causes_cnt++;
      break;
    
    case TD2G_CEL_RESEL_TRIGGERED:
      tdsrrc_debug_global.tdsrrc_debug_td2g_resel_stats.tdstog_resel_cnt++;
      break;
    
    case TD2G_CEL_RESEL_FAILURE:
      tdsrrc_debug_global.tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt++;
      break;
    
    case TDSOOS_IN_IDLE_MODE:
      tdsrrc_debug_global.tdsrrc_debug_oos_stats.oos_idle_cnt++;
      break;
    
    case TDSOOS_IN_CONNECTED_MODE:
      tdsrrc_debug_global.tdsrrc_debug_oos_stats.oos_connected_cnt++;
      break;
    
    case TDSRACH_SUCCFUL_RRC_RETRY:
      tdsrrc_debug_global.tdsrrc_debug_rach_transmission_stats_stats.succful_rrc_tran_retry_cnt++;
      break;
    
    case TDSRACH_RRC_RETRY_FAIL_DUE_TO_CMAC_STATUS_IND:
      tdsrrc_debug_global.tdsrrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_cmac_status_ind_cnt++;
      break;
      
    case TDSRACH_RRC_RETRY_FAIL_DUE_TO_OTHER_CAUSE:
      tdsrrc_debug_global.tdsrrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_others_cnt++;
      break;

    case TDSINTER_FREQ_CEL_RESEL_SUCCESS:
        tdsrrc_debug_global.tdsrrc_debug_inter_freq_resel_stats.inter_freq_resel_success_cnt++;
        break;

    case TDSINTER_FREQ_CEL_RESEL_NBCCH_SETUP_FAILURE:
      tdsrrc_debug_global.tdsrrc_debug_inter_freq_resel_stats.inter_freq_nbcch_setup_failure_cnt++;
      break;
    
    case TDSINTER_FREQ_CEL_RESEL_SIB_WAIT_TIMEOUT:
      tdsrrc_debug_global.tdsrrc_debug_inter_freq_resel_stats.inter_freq_sib_wait_timeout_cnt++;
      break;
    
    case TDSINTER_FREQ_CEL_RESEL_TRANSITION_FAILURE:
      tdsrrc_debug_global.tdsrrc_debug_inter_freq_resel_stats.inter_freq_transition_fail_cnt++;
      break;
    
    case TDSINTER_FREQ_CEL_RESEL_FAILED_OTHER_CAUSE:
      tdsrrc_debug_global.tdsrrc_debug_inter_freq_resel_stats.inter_freq_failed_other_causes_cnt++;
      break;

    case TDSOOS_IN_CONNECTED_MODE_CELL_FACH:
      tdsrrc_debug_global.tdsrrc_debug_oos_stats.oos_connected_cnt_cell_fach++;
      break;
      
    case TDSOOS_IN_CONNECTED_MODE_CELL_DCH:
      tdsrrc_debug_global.tdsrrc_debug_oos_stats.oos_connected_cnt_cell_dch++;
      break;
      
    case TDSOOS_IN_CONNECTED_MODE_CELL_PCH:
      tdsrrc_debug_global.tdsrrc_debug_oos_stats.oos_connected_cnt_cell_pch++;
      break;
      
    case TDSOOS_IN_CONNECTED_MODE_URA_PCH:
      tdsrrc_debug_global.tdsrrc_debug_oos_stats.oos_connected_cnt_ura_pch++;
      break;

     case TD2G_CEL_RESEL_FAILURE_NBCCH_SETUP_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt_NBCCH_SETUP_FAILURE++;
        break;

     case TD2G_CEL_RESEL_FAILURE_SIB_WAIT_TIMEOUT:
        tdsrrc_debug_global.tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt_SIB_WAIT_TIMEOUT++;
        break;

     case TD2G_CEL_RESEL_FAILURE_ACQ_FAILURE:
       tdsrrc_debug_global.tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt_ACQ_FAILURE++;
       break;

     case TD2G_CEL_RESEL_FAILURE_FAILED_OTHER_CAUSE:
        tdsrrc_debug_global.tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt_OTHER_CAUSE++;
        break;

     case TD2L_CEL_RESEL_TRIGGERED:
        tdsrrc_debug_global.tdsrrc_debug_td2l_resel_stats.tdstol_resel_cnt++;
        break;
        
     case TD2L_CEL_RESEL_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt++;
        break;
        
     case TD2L_CEL_RESEL_FAILURE_NBCCH_SETUP_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt_NBCCH_SETUP_FAILURE++;
        break;

     case TD2L_CEL_RESEL_FAILURE_SIB_WAIT_TIMEOUT:
        tdsrrc_debug_global.tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt_SIB_WAIT_TIMEOUT++;
        break;

     case TD2L_CEL_RESEL_FAILURE_ACQ_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt_ACQ_FAILURE++;
        break;

     case TD2L_CEL_RESEL_FAILURE_FAILED_OTHER_CAUSE:
        tdsrrc_debug_global.tdsrrc_debug_td2l_resel_stats.tdstol_resel_failure_cnt_OTHER_CAUSE++;
        break;

     case G2TD_CEL_RESEL_TRIGGERED:
        tdsrrc_debug_global.tdsrrc_debug_g2td_resel_stats.g2td_resel_cnt++;
        break;

    case G2TD_CEL_RESEL_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_g2td_resel_stats.g2td_resel_failure_cnt++;
        break;

    case G2TD_CEL_RESEL_FAILURE_ACQ_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_g2td_resel_stats.g2td_resel_failure_cnt_ACQ_FAILURE++;
        break;

    case G2TD_CEL_RESEL_FAILURE_NBCCH_SETUP_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_g2td_resel_stats.g2td_resel_failure_cnt_NBCCH_SETUP_FAILURE++;
        break;

    case G2TD_CEL_RESEL_FAILURE_SIB_WAIT_TIMEOUT:
        tdsrrc_debug_global.tdsrrc_debug_g2td_resel_stats.g2td_resel_failure_cnt_SIB_WAIT_TIMEOUT++;
        break;

    case G2TD_CEL_RESEL_FAILURE_FAILED_OTHER_CAUSE:
        tdsrrc_debug_global.tdsrrc_debug_g2td_resel_stats.g2td_resel_failue_cnt_OTHER_CAUSE++;
        break;

    case L2TD_CEL_RESEL_TRIGGERED:
        tdsrrc_debug_global.tdsrrc_debug_l2td_resel_stats.l2td_resel_cnt++;
        break;

    case L2TD_CEL_RESEL_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_l2td_resel_stats.l2td_resel_failure_cnt++;
        break;

    case L2TD_CEL_RESEL_FAILURE_ACQ_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_l2td_resel_stats.l2td_resel_failure_cnt_ACQ_FAILURE++;
        break;

    case L2TD_CEL_RESEL_FAILURE_NBCCH_SETUP_FAILURE:
        tdsrrc_debug_global.tdsrrc_debug_l2td_resel_stats.l2td_resel_failure_cnt_NBCCH_SETUP_FAILURE++;
        break;

    case L2TD_CEL_RESEL_FAILURE_SIB_WAIT_TIMEOUT:
        tdsrrc_debug_global.tdsrrc_debug_l2td_resel_stats.l2td_resel_failure_cnt_SIB_WAIT_TIMEOUT++;
        break;

    case L2TD_CEL_RESEL_FAILURE_FAILED_OTHER_CAUSE:
       tdsrrc_debug_global.tdsrrc_debug_l2td_resel_stats.l2td_resel_failue_cnt_OTHER_CAUSE++;
       break;

    case TDSRRC_ACQ_TRIGGERED:
        tdsrrc_debug_global.tdsrrc_debug_acq_stats.td_acq_attemp_cnt++;
        break;

    case TDSRRC_ACQ_SUCCEED:
        tdsrrc_debug_global.tdsrrc_debug_acq_stats.td_acq_attemp_succeed_cnt++;
        break;

    case TDSRRC_ACQ_FAILED:
        tdsrrc_debug_global.tdsrrc_debug_acq_stats.td_acq_attemp_failed_cnt++;
        break;

    case TDSRRC_BPLMN_TD2TD:
        tdsrrc_debug_global.tdsrrc_debug_bplmn_stats.td2td_bplmn_cnt++;
        break;
        
    case TDSRRC_BPLMN_TD2G:
        tdsrrc_debug_global.tdsrrc_debug_bplmn_stats.td2g_bplmn_cnt++;
        break;

    case TDSRRC_BPLMN_TD2L:
        tdsrrc_debug_global.tdsrrc_debug_bplmn_stats.td2l_bplmn_cnt++;
        break;

    case TDSRRC_BPLMN_G2TD:
        tdsrrc_debug_global.tdsrrc_debug_bplmn_stats.g2td_bplmn_cnt++;
        break;

    case TDSRRC_BPLMN_L2TD:
        tdsrrc_debug_global.tdsrrc_debug_bplmn_stats.l2td_bplmn_cnt++;
        break;

    case TDSRRC_RCE_ATTEMP:
        tdsrrc_debug_global.tdsrrc_debug_rce_stats.tdsrrc_rce_attemp_cnt++;
        break;
        
    case TDSRRC_RCE_SUCCEED:
        tdsrrc_debug_global.tdsrrc_debug_rce_stats.tdsrrc_rce_succeed_cnt++;
        break;

    case TDSRRC_RCE_FAIL:
        tdsrrc_debug_global.tdsrrc_debug_rce_stats.tdsrrc_rce_failed_cnt++;
        break;
        
    case TDSRRC_G2TD_FAST_RETURN_TRIGGERED:
        tdsrrc_debug_global.tdsrrc_debug_gtot_fast_return_stats.fast_return_trigger_cnt++;
        break;
        
    case TDSRRC_G2TD_FAST_RETURN_SUCCEED:
        tdsrrc_debug_global.tdsrrc_debug_gtot_fast_return_stats.fast_return_succeed_cnt++;
        break;
    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"ERR: Invalid type :%d ",rrc_debug_cause);
      break;
  }
}

/*===========================================================================

FUNCTION  tdsrrc_debug_clear_stats

DESCRIPTION This function clears all the debug statistics

DEPENDENCIES
  None
  
RETURN VALUE
  None

===========================================================================*/
void tdsrrc_debug_clear_stats
(
  void
)
{
#if 0
  tdsrrc_debug_intra_freq_resel_stats.intra_freq_resel_success_cnt = 0;
  tdsrrc_debug_intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt = 0;
  tdsrrc_debug_intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt = 0;
  tdsrrc_debug_intra_freq_resel_stats.intra_freq_transition_fail_cnt = 0;
  tdsrrc_debug_intra_freq_resel_stats.intra_freq_failed_other_causes_cnt = 0;
  tdsrrc_debug_td2g_resel_stats.tdstog_resel_cnt = 0;
  tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt = 0;
  tdsrrc_debug_oos_stats.oos_idle_cnt= 0;
  tdsrrc_debug_oos_stats.oos_connected_cnt= 0;
  tdsrrc_debug_rach_transmission_stats_stats.succful_rrc_tran_retry_cnt = 0;
  tdsrrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_cmac_status_ind_cnt= 0;
  tdsrrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_others_cnt= 0;
  #endif
      /* Intra frequency cell reselection statistics */
    memset(&tdsrrc_debug_global,0,sizeof(tdsrrc_debug_global_var_type));
}

/*===========================================================================

FUNCTION  tdsrrc_debug_get_global_var

DESCRIPTION   This function gives the pointer of tdsrrc_debug_global

DEPENDENCIES
  None
  
RETURN VALUE  Intra frequency reselection success count since last power up

===========================================================================*/
void tdsrrc_debug_get_global_var
(
    tdsrrc_debug_global_var_type** tdsrrc_debug_ptr
)
{
    *tdsrrc_debug_ptr = &tdsrrc_debug_global;
}


/*===========================================================================

FUNCTION  tdsrrc_debug_get_cel_resel_success_cnt

DESCRIPTION   This function gives the intra frequency cell reselection success count

DEPENDENCIES
  None
  
RETURN VALUE  Intra frequency reselection success count since last power up

===========================================================================*/
uint32 tdsrrc_debug_get_cel_resel_success_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_resel_success_cnt;
}

/*===========================================================================

FUNCTION  tdsrrc_debug_get_cel_resel_nbcch_set_up_fail_cnt

DESCRIPTION  This function gives the intra frequency cell reselection fail count due to 
 N-BCCH set up failure.

DEPENDENCIES
  None
  
RETURN  Intra frequency reselection failure count due to N-BCCH set up failue since last power up

===========================================================================*/
uint32 tdsrrc_debug_get_cel_resel_nbcch_set_up_fail_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_nbcch_setup_failure_cnt;
}

/*===========================================================================

FUNCTION  tdsrrc_debug_get_cel_resel_sib_wait_timeout_cnt

DESCRIPTION  This function gives the intra frequency cell reselection fail count due to 
 SIB wait time out.

DEPENDENCIES
  None
  
RETURN   Intra frequency reselection failure count due to SIB wait time out since last power up

===========================================================================*/
uint32 tdsrrc_debug_get_cel_resel_sib_wait_timeout_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_sib_wait_timeout_cnt;
}

/*===========================================================================

FUNCTION  tdsrrc_debug_get_cel_resel_transition_fail_cnt

DESCRIPTION  This function gives the intra frequency cell reselection fail count due to 
 transition failure(s-criteria failure).

DEPENDENCIES
  None
  
RETURN  Intra frequency reselection failure count due to transition failures since last power up 

===========================================================================*/
uint32 tdsrrc_debug_get_cel_resel_transition_fail_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_transition_fail_cnt;
}

/*===========================================================================

FUNCTION  tdsrrc_debug_get_cel_resel_failed_other_causes_cnt

DESCRIPTION  This function gives the intra frequency cell reselection fail count due to 
 the causes other than n-bcch setup failure, sib wait time out and transition failure.

DEPENDENCIES
  None
  
RETURN  Intra frequency reselection failure count due to other causes since last power up

===========================================================================*/
uint32 tdsrrc_debug_get_cel_resel_failed_other_causes_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_intra_freq_resel_stats.intra_freq_failed_other_causes_cnt;
}

/*===========================================================================

FUNCTION  rrc_debug_get_wtog_cel_resel_success_cnt

DESCRIPTION  This function gives the number of times the TDSTOG cell reselection is triggered 

DEPENDENCIES
  None
  
RETURN  TDSTOG cell reselection count since last power up

===========================================================================*/
uint32 rrc_debug_get_td2g_cel_resel_cnt
(
  void
)
{
  return (tdsrrc_debug_global.tdsrrc_debug_td2g_resel_stats.tdstog_resel_cnt);
}

/*===========================================================================

FUNCTION  rrc_debug_get_td2g_cel_resel_fail_cnt

DESCRIPTION  This function gives the TDSTOG cell reselection fail count.

DEPENDENCIES
  None
  
RETURN  TDSTOG cell reselection failure count since last power up

===========================================================================*/
uint32 rrc_debug_get_td2g_cel_resel_fail_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_td2g_resel_stats.tdstog_resel_failure_cnt;
}

/*===========================================================================

FUNCTION  tdsrrc_debug_get_oos_idle_mode_cnt

DESCRIPTION  This function gives how many times the OOS is triggered in idle mode.

DEPENDENCIES
  None
  
RETURN  Number of times OOS is triggered in idle mode since last power up

===========================================================================*/
uint32 tdsrrc_debug_get_oos_idle_mode_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_oos_stats.oos_idle_cnt;
}

/*===========================================================================

FUNCTION  tdsrrc_debug_get_oos_connected_mode_cnt

DESCRIPTION This function gives how many times the OOS is triggered in connected mode.

DEPENDENCIES
  None
  
RETURN  Number of times OOS is triggered in connected mode since last power up

===========================================================================*/
uint32 tdsrrc_debug_get_oos_connected_mode_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_oos_stats.oos_connected_cnt;
}
/*===========================================================================

FUNCTION  tdsrrc_debug_get_succful_rrc_trans_retry_cnt

DESCRIPTION This function gives counter value which is  incremented 
when RRC is successful in receiving RRC procedure level response
message from N/W for any RRC message sent on common channel i.e. RACH. 
.
DEPENDENCIES
  None
  
RETURN  Number of times RRC is successful in receiving RRC procedure level response
message from N/W since last power up

===========================================================================*/
uint32 tdsrrc_debug_get_succful_rrc_trans_retry_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_rach_transmission_stats_stats.succful_rrc_tran_retry_cnt;
}
/*===========================================================================

FUNCTION  tdsrrc_debug_get_rrc_retry_fail_due_to_l2_ack_ind_cnt

DESCRIPTION This function gives counter value which is incremented 
when MAC hits max re-try while RACHing & sends CMAC_STATUS_IND
with cause as hit MAX re-try.
.
DEPENDENCIES
  None
  
RETURN  Number of times max RACH transmission hit by MAC.
===========================================================================*/
uint32 tdsrrc_debug_get_rrc_retry_fail_due_to_l2_ack_ind_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_cmac_status_ind_cnt;
}
/*===========================================================================

FUNCTION  tdsrrc_debug_get_rrc_retry_fail_due_to_others_cnt

DESCRIPTION This function gives counter value which is incremented 
when RRC retransmit message Bcoz of receiving L2 ACK IND with cause
failure for previously attempted transmission of RRC message on RACH 
.
DEPENDENCIES
  None
  
RETURN   Number of times RRC transmission or retry fail due to L2 ACK IND.
===========================================================================*/
uint32 tdsrrc_debug_get_rrc_retry_fail_due_to_others_cnt
(
  void
)
{
  return tdsrrc_debug_global.tdsrrc_debug_rach_transmission_stats_stats.rrc_retry_fail_due_to_others_cnt;
}

/*===========================================================================

FUNCTION  tdsrrc_extract_rac_from_sib_info

DESCRIPTION This function extracts RAC from NAS CN Domain System Information List.

DEPENDENCIES
  None
  
RETURN VALUE          
  TRUE: RAC is found
  FALSE: RAC is not found

===========================================================================*/
boolean  tdsrrc_extract_rac_from_sib_info
(
  rrc_rai_type *rai,
  tdsrrc_CN_DomainSysInfoList  *cn_domain_sys_info_list_ptr
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
                                      T_tdsrrc_CN_DomainSysInfo_cn_Type_gsm_MAP)
    {
      unsigned char  *tdsdata_ptr;
      uint32          i;
  
      if(cn_domain_sys_info_list_ptr->elem[index].cn_DomainIdentity == tdsrrc_CN_DomainIdentity_ps_domain)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PS CN Domain specific info is present");

        if(cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"GSM MAP not present");
          return FALSE;
        }
        
        /* Check if the RRC MM interface structure array can hold this info */
        if(cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP->numocts > 
            RRC_MAX_GSM_INFO_OCTET_COUNT)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Not enough space for NAS Sys Info (%d bytes)",cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP->numocts);
          return FALSE;
        }
  
        tdsdata_ptr = cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP->data;
  
        for( i = 0; i < cn_domain_sys_info_list_ptr->elem[index].cn_Type.u.gsm_MAP->numocts; i ++)
        {
          nas_domain_spec_info.value[i] = *tdsdata_ptr;
          tdsdata_ptr ++;
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
  
  if(!rac_found)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"No RAC is found");
    return FALSE;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RAC is found");
    return TRUE;
  }
}

/* this table use to convert from ASN1 to OC
    refer to 3GPP: 10.3.6.91) */
uint8 tdsTpcStepSize[4]= {
    TDSL1_TPC_STEP_SIZE_0DB,
    TDSL1_TPC_STEP_SIZE_1DB,
    TDSL1_TPC_STEP_SIZE_2DB,
    TDSL1_TPC_STEP_SIZE_3DB,
};
/* this table use to convert from ASN1 to OC
    refer to 3GPP: 10.3.6.41 */

uint8 tdsMidambleAllocMode[4] = {
    0,
    TDSL1_DEFAULT_MIDAMBLE,
    TDSL1_COMMON_MIDAMBLE,
    TDSL1_UE_SPECIFIC_MIDAMBLE
};

#ifdef FEATURE_SCUDIF_ENHANCEMENT
#error code not present
#endif/* FEATURE_SCUDIF_ENHANCEMENT*/

/*===========================================================================

FUNCTION   tdsrrc_is_nw_shared

DESCRIPTION

 This function will indicate if this N/W is shared or not.
        
DEPENDENCIES

  None.
 
RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_is_nw_shared
(
  tdsrrc_MasterInformationBlock *mib_ptr
)
{
  boolean status = FALSE;

  if(TDSRRC_IS_NETWORK_SHARED(mib_ptr))
  {
    status = TRUE;
  }
  else
  {
    status = FALSE;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"NWS: is_nw_shared = %d",status);
  return status;
}


/*===========================================================================

FUNCTION tdsrrc_malloc_for_external_cmd

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
void * tdsrrc_malloc_for_external_cmd(size_t size)
{
  void * return_ptr =  NULL;
  
  return_ptr = 
       modem_mem_calloc(1,size,
          MODEM_MEM_CLIENT_TDSCDMA_RRC_CMD_CRIT);
  

  if(return_ptr == NULL)
  {
    ERR_FATAL("Failed to allocate memory of size 0x%x for RRC ext cmd",size,0,0);
  }

  return(return_ptr);
}

#ifdef FEATURE_TDSCDMA_REL8
/*===========================================================================

FUNCTION   tdsrrc_update_rab_type_in_rab_info_r8

DESCRIPTION
 This function is updates the rab type in 'tdsrrc_est_rab_info_type' structure
 using information stored in ASN1 'tdsrrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'tdsrrc_RAB_InformationSetup' is already validated.

ARGUMENTS:

  tdsrrc_RAB_InformationSetup *: pointer to ASN1 structure,
  tdsrrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

tdsrrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_e_type tdsrrc_update_rab_type_in_rab_info_r8
(
  tdsrrc_RAB_InformationSetup_r8 *rab_info_setup_ptr,
  tdsrrc_est_rab_info_type *est_rab_info_ptr 
)
{
  tdsrrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (tdsrrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = TDSRRCRB_PS_DATA_RAB;
  }
  else if (tdsrrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = tdsrrcrb_translate_gsm_map_rab_id(rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);
      
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
    mn_rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if ( tdsrrclbt_is_lb_on() == TRUE )
    {
      /* Set Rab type to Data */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB setup for Loopback Test");
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_VOICE_RAB ;
    }
    else
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_UNKNOWN_RAB;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cn_domain value: %d",cn_domain);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Cn domain %d",cn_domain);
  }  

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
            est_rab_info_ptr->rab_type, TDSRRCRB_CS_VOICE_RAB, TDSRRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}

#endif /*FEATURE_TDSCDMA_REL8*/

#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
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
void tdsrrc_set_modem_stats_mask (uint16 bit_mask)
{
  TDS_TASKLOCK();
  tds_UI_event_bit_mask = bit_mask;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"FTD_DBG: Bitmask sent by CM %x",tds_UI_event_bit_mask);
  TDS_TASKFREE();
}

/*===========================================================================

FUNCTION WCDMA_GET_MODEM_STATS_INFO 

DESCRIPTION
  This function copies the TDSCDMA event fields to the stucture provided as arguments

DEPENDENCIES
  None

RETURN VALUE
  None


SIDE EFFECTS
  None

===========================================================================*/
void tdsrrc_get_modem_stats_info (tds_UI_event_info_type *tds_info)
{
  TDS_TASKLOCK();
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"FTD_DBG: Populating modem stats info");
  memscpy(tds_info, sizeof(tds_UI_event_info_type), &(tds_UI_event_info),sizeof(tds_UI_event_info_type));
  tds_UI_event_info.bitmask = 0;
  TDS_TASKFREE();
}
#endif

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
sys_modem_as_id_e_type tdsrrc_get_as_id
(
  void
)
{
#ifdef FEATURE_DUAL_SIM
  if(!tdsrrc_is_sim_id_valid)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: SIM INSERTED REQ not received,use theAS ID %d",tdsrrc_sim_id_recvd);
  }
  return (tdsrrc_sim_id_recvd);
#else
  return (SYS_MODEM_AS_ID_1);
#endif
}

/*===========================================================================

FUNCTION TDSRRC_SET_AS_ID

DESCRIPTION           This function sets the AS ID provided
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrc_set_as_id
(
  sys_modem_as_id_e_type as_id
)
{
  tdsrrc_is_sim_id_valid = TRUE;
  tdsrrc_sim_id_recvd = as_id;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: Setting AS ID as %d",tdsrrc_sim_id_recvd);
}

#ifdef FEATURE_TDSCDMA_EFS_NEW_API
/*===========================================================================

FUNCTION  TDSRRC_GET_EFS_STORAGE_SUB_ID

DESCRIPTION

  This function returns the MCFG sub id value for EFS file access using MCFG APIs. It converts current as_id from
  sys_modem_as_id_e_type to mcfg_fs_sub_id_e_type and returns the result.

DEPENDENCIES

  None.

RETURN VALUE

  sub id of type mcfg_fs_sub_id_e_type for EFS file access

SIDE EFFECTS

  None.

===========================================================================*/
mcfg_fs_sub_id_e_type tdsrrc_get_efs_storage_sub_id
(
  void
)
{
  /* TDS always access the EFS storage mapped to the primary subscription */
  return MCFG_FS_SUBID_0;
}
#endif

/*===========================================================================

FUNCTION  tdsrrc_ok_for_tune_away

DESCRIPTION

  This function checks whether UE allows tune away or not
    
DEPENDENCIES
  None.
 
RETURN VALUE

 None
 
SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_ok_for_tune_away(void)
{
  boolean is_lb_on,is_cs_rab_present;
  boolean is_cs_present = FALSE;
  tdsrrcrce_rrc_conn_status_e_type rce_status = tdsrrcrce_get_rrc_connection_status();
  is_cs_rab_present = ((tdsrrc_query_mac_rab_status() == TDSCS_PLUS_PS_RABS) || (tdsrrc_query_mac_rab_status() == TDSCS_RABS)
#ifdef FEATURE_3GPP_CSFB_TDS
                          || tdsrrc_csfb_call_status
#endif
                         );
  is_lb_on = tdsrrclbt_is_lb_on();
  /* Some times ,NW release the CS call and release the CS domain signaling connection ,but not
     release the CS RAB.*/
   is_cs_present =  (TRM_TRAFFIC == tdsrrcwrm_proc_map_priority());
   
  if(
#if 0    
   /*this function is called at some place to decide our trm priority, 
    e.g., BG_TAFFIC or ACCESS_URGENT. We should not raise priority based on this tuneaway_status flag to prevent other sub's unlock request, 
    because we need to allow other sub MO call even tuneaway is disabled
    (tds_ds_tuneaway_status_change_ind.tuneaway_status == TRUE)&&*/
#endif
   (is_cs_present == FALSE)  /* CS call is not present*/
   /* RRC allow the QTA and LTA when RCE is waiting for wait timer out. */
   && ((TDSRRCRCE_RRC_CONN_ESTABLISHED == rce_status )||(TDSRRCRCE_WAIT_FOR_WAIT_TIMER_TO_EXPIRE == tdsrrcrce_get_rce_sub_state())) /*RRC Connection is present */
   && (tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_CSP) == TDSRRC_CAMPED_ON) /* UE is camped */
    )
  {
    return TRUE;
  }
  else
  {
    TDSRRC_MSG5(MSG_LEGACY_HIGH,
      "tuneaway enabled =%d, is_cs_present: %d, is_lb_on: %d, is_cs_rab_present: %d, rce connection status:%d", 
      tds_ds_tuneaway_status_change_ind.tuneaway_status,is_cs_present,is_lb_on,is_cs_rab_present,rce_status);
#ifdef FEATURE_3GPP_CSFB_TDS
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"csfb_call_status: %d",tdsrrc_csfb_call_status);
#endif
    return FALSE;
  }
}
/*===========================================================================

FUNCTION TDSRRC_IS_CS_ACTIVE

DESCRIPTION           This function checks if any CS activity is ongoing after connection setup completed
  
DEPENDENCIES
  None.
  
RETURN VALUE
  BOOLEAN.
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrrc_is_cs_active(void)
{
  boolean voice_rab_present = tdsrrc_is_voice_rab_present();
  boolean cs_data_rab_present = tdsrrc_is_cs_data_rab_present();
  boolean cs_domain_open = tdsrrcdt_is_cs_domain_open();

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"voice_rab_present %d, cs_data_rab_present %d, cs_domain_open %d",
  	          voice_rab_present,cs_data_rab_present,cs_domain_open);
#ifdef FEATURE_3GPP_CSFB_TDS
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"csfb call %d", tdsrrc_csfb_call_status);
#endif

  if(voice_rab_present || cs_data_rab_present || cs_domain_open
#ifdef FEATURE_3GPP_CSFB_TDS
	 || tdsrrc_csfb_call_status
#endif  	 
  	)
  	{
      return TRUE;
  	}
  return FALSE;
}

/*===========================================================================

FUNCTION tdsrrc_is_traffic_active

DESCRIPTION           This function checks if any traffic priority is ongoing.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  BOOLEAN.
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrrc_is_traffic_active(void)
{

  trm_reason_enum_t priority = tdsrrcwrm_proc_map_priority();

  TDSRRC_MSG1(MSG_LEGACY_HIGH," TRM_U:priority is %d ",priority);
#ifdef FEATURE_3GPP_CSFB_TDS
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"csfb call %d", tdsrrc_csfb_call_status);
#endif

  if(TRM_TRAFFIC == priority
#ifdef FEATURE_3GPP_CSFB_TDS
	 || tdsrrc_csfb_call_status
#endif  	 
  	)
  	{
      return TRUE;
  	}
  return FALSE;
}


/*===========================================================================

FUNCTION tdsrrc_is_access_active

DESCRIPTION           This function checks if any access priority is ongoing.
  
DEPENDENCIES
  None.
  
RETURN VALUE
  BOOLEAN.
  
SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrrc_is_access_active(void)
{
  trm_reason_enum_t priority = tdsrrcwrm_proc_map_priority();

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TRM_U: priority %d for access",priority);
#ifdef FEATURE_3GPP_CSFB_TDS
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"csfb call %d", tdsrrc_csfb_call_status);
#endif

  if((TRM_ACCESS == priority)
#ifdef FEATURE_3GPP_CSFB_TDS
	 && !tdsrrc_csfb_call_status
#endif  	 
  	)
  	{
      return TRUE;
  	}
  return FALSE;
}
/*===========================================================================

FUNCTION  tdsrrc_check_send_change_priority

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
void tdsrrc_check_send_change_priority
(
  void
)
{
  trm_reason_enum_t priority;
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state(); 
  boolean is_irat_proc = FALSE;

  if((rrc_state == TDSRRC_STATE_CELL_DCH) || (rrc_state == TDSRRC_STATE_CELL_FACH)||(TDSRRC_STATE_CONNECTING == rrc_state)
    ||((TDSRRC_STATE_DISCONNECTED == rrc_state)&& 
        ((TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF == tdsrrcrce_get_rce_sub_state())||
         (TDSRRCRCE_WAIT_FOR_SIB7_CURRENT == tdsrrcrce_get_rce_sub_state()))))
  {

    if (tdsrrc_is_traffic_active())
    {
      priority = TRM_TRAFFIC;
    }
    else if (tdsrrc_is_access_active())
    {
      priority = TRM_ACCESS;
    }
    else
    {
      priority = TRM_BG_TRAFFIC;
    }

  /* T2X cell resel failure in CONNECTING state, change priority directly instead of release & reacq lock. */
  /* Connecting + wait timer, need to be hanlded in "else if", otherwise it is not BG_Traffic if TRM_U is not defined*/
   if(((TDSRRC_STATE_DISCONNECTED == rrc_state)&& 
        ((TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF == tdsrrcrce_get_rce_sub_state())||
         (TDSRRCRCE_WAIT_FOR_SIB7_CURRENT == tdsrrcrce_get_rce_sub_state()))))
   {
    is_irat_proc = TRUE;
   }
   tdsrrc_wrm_change_priority(priority,is_irat_proc);
   tdrm_retain_lock(TDRM_CLIENT_RRC);

   TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY:	priority to %d at state %d at rce sub state %d ",priority,rrc_state,tdsrrcrce_get_rce_sub_state());
  }
#if 0
  /* T2X cell resel failure in CONNECTING state, change priority directly instead of release & reacq lock. */
  /* Connecting + wait timer, need to be hanlded in "else if", otherwise it is not BG_Traffic if TRM_U is not defined*/
  if(((TDSRRC_STATE_CONNECTING == rrc_state)&&(TDSRRCRCE_WAIT_FOR_WAIT_TIMER_TO_EXPIRE != tdsrrcrce_get_rce_sub_state()))||
      ((TDSRRC_STATE_DISCONNECTED == rrc_state)&& 
        ((TDSRRCRCE_WAIT_FOR_RB0_SETUP_CNF == tdsrrcrce_get_rce_sub_state())||
         (TDSRRCRCE_WAIT_FOR_SIB7_CURRENT == tdsrrcrce_get_rce_sub_state()))))
  {
    priority = tdsrrcwrm_proc_map_priority();

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to %d",priority);
    tdsrrc_wrm_change_priority(priority, TRUE);
    
    tdrm_retain_lock(TDRM_CLIENT_RRC);

  }
  else if((rrc_state == TDSRRC_STATE_CELL_DCH) || (rrc_state == TDSRRC_STATE_CELL_FACH)||((rrc_state == TDSRRC_STATE_CONNECTING)&&(TDSRRCRCE_WAIT_FOR_WAIT_TIMER_TO_EXPIRE == tdsrrcrce_get_rce_sub_state())))
  {
    if(tdsrrc_ok_for_tune_away() != TRUE)
    {
      if(tdsrrc_is_traffic_active() == TRUE)
      {
	    /*Change TRM priority to Traffic*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to TRM_TRAFFIC");
        tdsrrc_wrm_change_priority(TRM_TRAFFIC, FALSE);	
        tdsrrcwrm_update_tune_away_status(TRM_TRAFFIC);
      }
      /* Call retain lock if change priority to TRM_ACCESS so that TRM may call unlock callback again to restart QTA/LTA procedure
       * due to we already stop original unlock_by timer when release lock in previous change priority procedure 
       */
	  tdrm_retain_lock(TDRM_CLIENT_RRC);
    }
    else if (tdsrrc_is_access_active())
    {
      /*Change TRM priority to Access*/
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to TRM_ACCESS");


      tdsrrc_wrm_change_priority(TRM_ACCESS, FALSE);

      /* Call retain lock if change priority to TRM_ACCESS so that TRM may call unlock callback again to restart QTA/LTA procedure
       * due to we already stop original unlock_by timer when release lock in previous change priority procedure 
       */
	  tdrm_retain_lock(TDRM_CLIENT_RRC);

        tdsrrcwrm_update_tune_away_status(TRM_ACCESS);

	  }
    else
    {
      /*Change TRM priority to BG Traffic*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Change priority to TRM_BG_TRAFFIC");
      tdsrrc_wrm_change_priority(TRM_BG_TRAFFIC, FALSE);
      tdrm_retain_lock(TDRM_CLIENT_RRC);
      tdsrrcwrm_update_tune_away_status(TRM_BG_TRAFFIC);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: Ignoring Change priority in PCH state %d",rrc_state);
  }
#endif
}


/*===========================================================================

FUNCTION   tdsrrc_update_rab_type_in_rab_info_r4

DESCRIPTION
 This function is updates the rab type in 'tdsrrc_est_rab_info_type' structure
 using information stored in ASN1 'tdsrrc_RAB_InformationSetupList'.
        
DEPENDENCIES

  There is no error checking done in this function. Its assumed that ASN1
  structure 'tdsrrc_RAB_InformationSetup' is already validated.

  The function is copy from tdsrrc_update_rab_type_in_rab_info_r5

ARGUMENTS:

  tdsrrc_RAB_InformationSetup *: pointer to ASN1 structure,
  tdsrrc_est_rab_info_type *: pointer to internal RRC est_rab structure in which this 
                                       function will fill appropriate rab_type.

RETURN VALUE

tdsrrcrb_rab_e_type: returns the rab_type which is filled in est_rab_info

SIDE EFFECTS

  None.

===========================================================================*/
tdsrrcrb_rab_e_type tdsrrc_update_rab_type_in_rab_info_r4
(
  tdsrrc_RAB_InformationSetup_r4 *rab_info_setup_ptr,
  tdsrrc_est_rab_info_type *est_rab_info_ptr 
)
{
  tdsrrc_CN_DomainIdentity cn_domain;

  cn_domain = rab_info_setup_ptr->rab_Info.cn_DomainIdentity;

  if (tdsrrc_CN_DomainIdentity_ps_domain == cn_domain)
  {
    est_rab_info_ptr->rab_type = TDSRRCRB_PS_DATA_RAB;
  }
  else if (tdsrrc_CN_DomainIdentity_cs_domain == cn_domain)
  {
    rrc_rab_id_type rab_id;
    rrcmn_rab_status_T mn_rab_type;
    rab_id = tdsrrcrb_translate_gsm_map_rab_id(
        rab_info_setup_ptr->rab_Info.rab_Identity.u.gsm_MAP_RAB_Identity);

#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_DUAL_ACTIVE)
    mn_rab_type = rrcmn_verify_rab_subs((radio_access_bearer_T)rab_id,tdsrrc_get_as_id());
#else   
    mn_rab_type = rrcmn_verify_rab( (radio_access_bearer_T)rab_id );
#endif

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"MN RAB type %d",mn_rab_type);

    /* Check if it is in loopback test - do we really need this check ? */
    if ( tdsrrclbt_is_lb_on() == TRUE )
    {
      /* Set Rab type to Data */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"RAB setup for Loopback Test");
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB;
    }
    else if(mn_rab_type == RRCMN_CS_DATA_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_DATA_RAB ;
    }
    else if(mn_rab_type == RRCMN_CS_VOICE_CALL)
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_CS_VOICE_RAB;
    }
    else
    {
      est_rab_info_ptr->rab_type = TDSRRCRB_UNKNOWN_RAB;
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid cn_domain value: %d",cn_domain);
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Cn domain %d",cn_domain);
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Configured RAB type:%d (%d-CS-Voice, %d-PS-Data, other CS-Data)",
            est_rab_info_ptr->rab_type, TDSRRCRB_CS_VOICE_RAB, TDSRRCRB_PS_DATA_RAB);

  return(est_rab_info_ptr->rab_type);
}

/*===========================================================================

FUNCTION tdsrrc_print_plmn_id

DESCRIPTION           Prints PLMN I.D
  
DEPENDENCIES
  None.
  
RETURN VALUE
  None.
  
SIDE EFFECTS
  None.

===========================================================================*/
void   tdsrrc_print_plmn_id
(
  rrc_plmn_identity_type plmn_id
)
{
  /*Print MMC here*/
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"NWS:PLMN I.D mcc[0]=0x%x, mcc[1]=0x%x, mcc[2]=0x%x",
                  plmn_id.mcc[0],plmn_id.mcc[1],plmn_id.mcc[2]);

  /*Print MNC here*/
  if(plmn_id.num_mnc_digits == 2)
  {
    /*Number of MNC digits is 2*/
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"NWS:mnc[0]=0x%x,mnc[1]=0x%x",plmn_id.mnc[0],plmn_id.mnc[1]);
  }
  else if(plmn_id.num_mnc_digits == 3)
  {
    /*Number of MNC digits is 3*/
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"NWS:mnc[0]=0x%x, mnc[1]=0x%x, mnc[2]=0x%x",
                       plmn_id.mnc[0],plmn_id.mnc[1],plmn_id.mnc[2]);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"NSW:Wrong number of MNC digits = %d",plmn_id.num_mnc_digits);
  }
}

uint16 tdsRucchTimer[] = {
               20,       /* ms */
               40,       /* ms */
               60,       /* ms */
               80,       /* ms */
              120,       /* ms */
              160,       /* ms */
              200,       /* ms */
              240,       /* ms */
              280,       /* ms */
              320,       /* ms */
              400,       /* ms */
              500,       /* ms */
              600,       /* ms */
              800,       /* ms */
              1000,      /* ms */
              2000       /* ms */
};
uint16 tdsTWaitimer[] = {
                5,       /* ms */
               40,       /* ms */
               80,       /* ms */
              160,       /* ms */
              320,       /* ms */
              640,       /* ms */
              1000,      /* ms */
              2000       /* ms */
};
uint8 tdsSITimer[] = {
                5,       /* ms */
               20,       /* ms */
               40,       /* ms */
               60,       /* ms */
               80,       /* ms */
              160,       /* ms */
              200        /* ms */
};
uint16 tdsRetransTimerForSI[] = {
           10,          /* retransTimerForSchedInfo_ms10 = 0, */
           15,          /* retransTimerForSchedInfo_ms15 = 1, */
           20,          /* retransTimerForSchedInfo_ms20 = 2, */
           25,          /* retransTimerForSchedInfo_ms25 = 3, */
           30,          /* retransTimerForSchedInfo_ms30 = 4, */
           35,          /* retransTimerForSchedInfo_ms35 = 5, */
           40,          /* retransTimerForSchedInfo_ms40 = 6, */
           45,          /* retransTimerForSchedInfo_ms45 = 7, */
           50,          /* retransTimerForSchedInfo_ms50 = 8, */
           55,          /* retransTimerForSchedInfo_ms55 = 9, */
           60,          /* retransTimerForSchedInfo_ms60 = 10, */
           65,          /* retransTimerForSchedInfo_ms65 = 11, */
           70,          /* retransTimerForSchedInfo_ms70 = 12, */
           75,          /* retransTimerForSchedInfo_ms75 = 13, */
           80,          /* retransTimerForSchedInfo_ms80 = 14, */
           85,          /* retransTimerForSchedInfo_ms85 = 15, */
           90,          /* retransTimerForSchedInfo_ms90 = 16, */
           95,          /* retransTimerForSchedInfo_ms95 = 17, */
           100,         /* retransTimerForSchedInfo_ms100 = 18, */
           110,         /* retransTimerForSchedInfo_ms110 = 19, */
           120,         /* retransTimerForSchedInfo_ms120 = 20, */
           140,         /* retransTimerForSchedInfo_ms140 = 21, */
           160,         /* retransTimerForSchedInfo_ms160 = 22, */
           200,         /* retransTimerForSchedInfo_ms200 = 23, */
           240,         /* retransTimerForSchedInfo_ms240 = 24, */
           280,         /* retransTimerForSchedInfo_ms280 = 25, */
           320,         /* retransTimerForSchedInfo_ms320 = 26, */
           400,         /* retransTimerForSchedInfo_ms400 = 27, */
           480,         /* retransTimerForSchedInfo_ms480 = 28, */
           560          /* retransTimerForSchedInfo_ms560 = 29  */
  };



/*===========================================================================
FUNCTION tdsrrc_register_with_msgr_for_client_and_register_umids

DESCRIPTION
  This function rregisters RRC with MSGR and registers for UMIDs it receives
  
DEPENDENCIES
  None
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_register_with_msgr_for_client_and_register_umids
(
  void
)
{
  errno_enum_type msgr_error;

  msgr_client_create(&tdsrrc_msgr_client);
  msgr_error = msgr_client_add_rex_q(&tdsrrc_msgr_client,
                                     tds_rrc_get_tcb(),
                                     TDSRRC_MSGR_SIG,
                                     &tdsrrc_msgr_cmd_q,
                                     &tdsrrc_msgr_free_cmd_q,
                                     /*sizeof(q_link_type),*/ offsetof(tds_rrc_msgr_cmd_type,cmd),
                                     MSGR_NO_OFFSET,
                                     sizeof(tds_rrc_msgr_cmd_type),
                                     &tdsrrc_rex_q_id
                                    );

  if(msgr_error != E_SUCCESS)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TD2L: Cannot register the RRC rex client with MSGR,err cause %d",msgr_error);
    return;
  }

  (void)msgr_register_block_variant(MSGR_TDSCDMA_RRC,
                                    &tdsrrc_msgr_client,
                                    tdsrrc_rex_q_id,
                                    tdsrrc_msgr_x2t_umid_list,
                                    TDSRRC_MSGR_X2T_UMID_CNT,
                                    (uint8)SYS_MODEM_AS_ID_1);
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  (void)msgr_register_block_variant(MSGR_TDSCDMA_RRC,
                                    &tdsrrc_msgr_client,
                                    tdsrrc_rex_q_id,
                                    tdsrrc_msgr_x2t_umid_list,
                                    TDSRRC_MSGR_X2T_UMID_CNT,
                                    (uint8)SYS_MODEM_AS_ID_2);
#endif
}

/*===========================================================================
FUNCTION tdsrrc_deregister_with_msgr

DESCRIPTION
  This function delete the registered MSGR client
  
DEPENDENCIES
  None
RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_deregister_with_msgr(void)
{
   errno_enum_type msgr_error;

   msgr_error = msgr_client_delete(&tdsrrc_msgr_client);

   if(msgr_error != E_SUCCESS)
   {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TD2L: Cannot delete the RRC rex client with MSGR,err cause %d",msgr_error);
   }  
}


#ifdef FEATURE_TDSCDMA_ULDL_LOG_SUPPORT
/*===========================================================================

FUNCTION tdsrrc_get_ul_transaction_id

DESCRIPTION
  This function will extract the transaction ID from the OTA

DEPENDENCIES


RETURN VALUE
  uint8

SIDE EFFECTS
  None
===========================================================================*/

static uint32 tdsrrc_get_ul_transaction_id(void *msg_ptr,uecomdef_logchan_e_type rrclog_ul_lc)
{
  
  uint32 transactionid = 0xFFFFFFFF;                       /* Variable for transaction id */
  tdsrrc_UL_DCCH_Message *dcch_msg_ptr=NULL;    /* Pointer to Downlink DCCH */
  tdsrrc_UL_CCCH_Message *ccch_msg_ptr = NULL; /* Pointer to  downlink CCCH message */   

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MMAL: rrc_get_ul_transaction_id fn called ");	
  
  if(rrclog_ul_lc == UE_LOGCHAN_DCCH)
  {
    dcch_msg_ptr = (tdsrrc_UL_DCCH_Message *)msg_ptr;

    if(dcch_msg_ptr!=NULL)
    {
      if(dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_activeSetUpdateComplete)
      {
        transactionid = dcch_msg_ptr->message.u.activeSetUpdateComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_activeSetUpdateFailure)
      {
        transactionid = dcch_msg_ptr->message.u.activeSetUpdateFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_cellChangeOrderFromUTRANFailure)
      {
        transactionid = dcch_msg_ptr->message.u.cellChangeOrderFromUTRANFailure.u.r3.cellChangeOrderFromUTRANFailure_r3.rrc_TransactionIdentifier;
      }	
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_counterCheckResponse  )
      {
        transactionid = dcch_msg_ptr->message.u.counterCheckResponse.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_handoverFromUTRANFailure)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_measurementControlFailure )
      {
        transactionid = dcch_msg_ptr->message.u.measurementControlFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationComplete)
      {
          transactionid = dcch_msg_ptr->message.u.physicalChannelReconfigurationComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_physicalChannelReconfigurationFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.physicalChannelReconfigurationFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationComplete )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReconfigurationComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_radioBearerReconfigurationFailure )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReconfigurationFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_radioBearerReleaseComplete )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReleaseComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_radioBearerReleaseFailure )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReleaseFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_radioBearerSetupComplete )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerSetupComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_radioBearerSetupFailure )
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerSetupFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_rrcConnectionReleaseComplete )
      {
        transactionid = dcch_msg_ptr->message.u.rrcConnectionReleaseComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_rrcConnectionSetupComplete )
      {
        transactionid = dcch_msg_ptr->message.u.rrcConnectionSetupComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_rrcStatus )
      {
        if(dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.t == T_tdsrrc_ProtocolErrorMoreInformation_diagnosticsType_type1)
        {
          switch(dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->t)
          {
            case T_tdsrrc_ProtocolErrorMoreInformation_type1_asn1_ViolationOrEncodingError:
            case T_tdsrrc_ProtocolErrorMoreInformation_type1_messageTypeNonexistent:
              /*Do nothing */
              break;
        
            case T_tdsrrc_ProtocolErrorMoreInformation_type1_messageNotCompatibleWithReceiverState:
              transactionid = dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
                u.messageNotCompatibleWithReceiverState->rrc_TransactionIdentifier;
              break;
        
            case T_tdsrrc_ProtocolErrorMoreInformation_type1_ie_ValueNotComprehended:
              transactionid = dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
                u.ie_ValueNotComprehended->rrc_TransactionIdentifier;
              break;
        
            case T_tdsrrc_ProtocolErrorMoreInformation_type1_conditionalInformationElementError:
              transactionid = dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
                u.conditionalInformationElementError->rrc_TransactionIdentifier;
              break;
        
            case T_tdsrrc_ProtocolErrorMoreInformation_type1_messageExtensionNotComprehended:
              transactionid = dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->
                u.messageExtensionNotComprehended->rrc_TransactionIdentifier ;
              break;
        
            case T_tdsrrc_ProtocolErrorMoreInformation_type1_spare1:
            case T_tdsrrc_ProtocolErrorMoreInformation_type1_spare2:
              /*Do Nothing */
              break;
        
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid protocol Error Choice: %d",dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.u.type1->t);
              break;
          } 
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid diagnosticsType Choice: %d",dcch_msg_ptr->message.u.rrcStatus.protocolErrorInformation.diagnosticsType.t);
        }
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_securityModeComplete  )
      {
        transactionid = dcch_msg_ptr->message.u.securityModeComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_securityModeFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.securityModeFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_transportChannelReconfigurationComplete  )
      {
        transactionid = dcch_msg_ptr->message.u.transportChannelReconfigurationComplete.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_transportChannelReconfigurationFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.transportChannelReconfigurationFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_transportFormatCombinationControlFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.transportFormatCombinationControlFailure.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_ueCapabilityInformation  )
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityInformation.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_utranMobilityInformationConfirm  )
      {
        transactionid = dcch_msg_ptr->message.u.utranMobilityInformationConfirm.rrc_TransactionIdentifier;
      }
      else if (dcch_msg_ptr->message.t == T_tdsrrc_UL_DCCH_MessageType_utranMobilityInformationFailure  )
      {
        transactionid = dcch_msg_ptr->message.u.utranMobilityInformationFailure.rrc_TransactionIdentifier;
      }
    }	
  }
  else if (rrclog_ul_lc == UE_LOGCHAN_CCCH)
  {
    ccch_msg_ptr = (tdsrrc_UL_CCCH_Message *) msg_ptr;

    if(ccch_msg_ptr!=NULL)	
    {
      if(ccch_msg_ptr->message.t == T_tdsrrc_UL_CCCH_MessageType_cellUpdate )
      {
        if (ccch_msg_ptr->message.u.cellUpdate.m.failureCausePresent)
        {
          transactionid = ccch_msg_ptr->message.u.cellUpdate.failureCause.rrc_TransactionIdentifier;
        }
      }
      else if(ccch_msg_ptr->message.t == T_tdsrrc_UL_CCCH_MessageType_uraUpdate )
      {
        if(ccch_msg_ptr->message.u.uraUpdate.protocolErrorIndicator.t == T_tdsrrc_ProtocolErrorIndicatorWithMoreInfo_errorOccurred)
        {
          transactionid = ccch_msg_ptr->message.u.uraUpdate.protocolErrorIndicator.u.errorOccurred->rrc_TransactionIdentifier;
        }
      }
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH," MMAL UL Transaction ID %d ",transactionid );
  return transactionid;

}
/*===========================================================================

FUNCTION tdsrrc_get_dl_transaction_id

DESCRIPTION
  This function will extract the transaction ID from the OTA

DEPENDENCIES


RETURN VALUE
  uint8

SIDE EFFECTS
  None
===========================================================================*/

static uint32 tdsrrc_get_dl_transaction_id(tdsrrc_cmd_type *cmd_ptr)
{

  void *decoded_msg=NULL;
  uint32 transactionid = 0xFFFFFFFF;                       /* Variable for transaction id */
  uint8 dl_sdu_type = 0xFF;
  tdsrrc_DL_DCCH_Message *dcch_msg_ptr=NULL;    /* Pointer to Downlink DCCH */
  tdsrrc_DL_CCCH_Message *ccch_msg_ptr = NULL; /* Pointer to  downlink CCCH message */   
  dl_sdu_type = cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type;
  decoded_msg = cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"MMAL: rrc_get_dl_transaction_id fn called ");	

  if(dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU)
  {
    dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)decoded_msg;
  
    if(dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_activeSetUpdate)
    {
      if (dcch_msg_ptr->message.u.activeSetUpdate.t == T_tdsrrc_ActiveSetUpdate_r3)
      {
        transactionid = dcch_msg_ptr->message.u.activeSetUpdate.u.r3.activeSetUpdate_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.activeSetUpdate.t == T_tdsrrc_ActiveSetUpdate_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.activeSetUpdate.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_assistanceDataDelivery)
    {
      if (dcch_msg_ptr->message.u.assistanceDataDelivery.t == T_tdsrrc_AssistanceDataDelivery_r3)
      {
        transactionid = dcch_msg_ptr->message.u.assistanceDataDelivery.u.r3.assistanceDataDelivery_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.assistanceDataDelivery.t == T_tdsrrc_AssistanceDataDelivery_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.assistanceDataDelivery.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_cellChangeOrderFromUTRAN)
    {
      if (dcch_msg_ptr->message.u.cellChangeOrderFromUTRAN.t == T_tdsrrc_CellChangeOrderFromUTRAN_r3)
      {
        transactionid = dcch_msg_ptr->message.u.cellChangeOrderFromUTRAN.u.r3.cellChangeOrderFromUTRAN_IEs.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.cellChangeOrderFromUTRAN.t == T_tdsrrc_CellChangeOrderFromUTRAN_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.cellChangeOrderFromUTRAN.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }	
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm)
    {
      if (dcch_msg_ptr->message.u.cellUpdateConfirm.t == T_tdsrrc_CellUpdateConfirm_r3)
      {
        transactionid = dcch_msg_ptr->message.u.cellUpdateConfirm.u.r3.cellUpdateConfirm_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.cellUpdateConfirm.t == T_tdsrrc_CellUpdateConfirm_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_counterCheck)
    {
      if (dcch_msg_ptr->message.u.counterCheck.t == T_tdsrrc_CounterCheck_r3)
      {
        transactionid = dcch_msg_ptr->message.u.counterCheck.u.r3.counterCheck_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.counterCheck.t == T_tdsrrc_CounterCheck_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.counterCheck.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_downlinkDirectTransfer)
    {
      if (dcch_msg_ptr->message.u.downlinkDirectTransfer.t == T_tdsrrc_DownlinkDirectTransfer_r3)
      {
        transactionid = dcch_msg_ptr->message.u.downlinkDirectTransfer.u.r3.downlinkDirectTransfer_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.downlinkDirectTransfer.t == T_tdsrrc_DownlinkDirectTransfer_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.downlinkDirectTransfer.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM )
    {
      if (dcch_msg_ptr->message.u.handoverFromUTRANCommand_GSM.t == T_tdsrrc_HandoverFromUTRANCommand_GSM_r3)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANCommand_GSM.u.r3.handoverFromUTRANCommand_GSM_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.handoverFromUTRANCommand_GSM.t == T_tdsrrc_HandoverFromUTRANCommand_GSM_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANCommand_GSM.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_CDMA2000)
    {
      if (dcch_msg_ptr->message.u.handoverFromUTRANCommand_CDMA2000.t == T_tdsrrc_HandoverFromUTRANCommand_CDMA2000_r3)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANCommand_CDMA2000.u.r3.handoverFromUTRANCommand_CDMA2000_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.handoverFromUTRANCommand_CDMA2000.t == T_tdsrrc_HandoverFromUTRANCommand_CDMA2000_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.handoverFromUTRANCommand_CDMA2000.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_measurementControl)
    {
      if (dcch_msg_ptr->message.u.measurementControl.t == T_tdsrrc_MeasurementControl_r3)
      {
        transactionid = dcch_msg_ptr->message.u.measurementControl.u.r3.measurementControl_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.measurementControl.t == T_tdsrrc_MeasurementControl_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.measurementControl.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_pagingType2)
    {
      transactionid = dcch_msg_ptr->message.u.pagingType2.rrc_TransactionIdentifier;
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration)
    {
      if (dcch_msg_ptr->message.u.physicalChannelReconfiguration.t == T_tdsrrc_PhysicalChannelReconfiguration_r3)
      {
        transactionid = dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.r3.physicalChannelReconfiguration_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.physicalChannelReconfiguration.t == T_tdsrrc_PhysicalChannelReconfiguration_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_physicalSharedChannelAllocation )
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration )
    {
      if (dcch_msg_ptr->message.u.radioBearerReconfiguration.t == T_tdsrrc_RadioBearerReconfiguration_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReconfiguration.u.r3.radioBearerReconfiguration_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.radioBearerReconfiguration.t == T_tdsrrc_RadioBearerReconfiguration_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerReconfiguration.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease)
    {
      if (dcch_msg_ptr->message.u.radioBearerRelease.t == T_tdsrrc_RadioBearerRelease_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerRelease.u.r3.radioBearerRelease_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.radioBearerRelease.t == T_tdsrrc_RadioBearerRelease_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerRelease.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup)
    {
      if (dcch_msg_ptr->message.u.radioBearerSetup.t == T_tdsrrc_RadioBearerSetup_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerSetup.u.r3.radioBearerSetup_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.radioBearerSetup.t == T_tdsrrc_RadioBearerSetup_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.radioBearerSetup.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_rrcConnectionRelease)
    {
      if (dcch_msg_ptr->message.u.rrcConnectionRelease.t == T_tdsrrc_RRCConnectionRelease_r3)
      {
        transactionid = dcch_msg_ptr->message.u.rrcConnectionRelease.u.r3.rrcConnectionRelease_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.rrcConnectionRelease.t == T_tdsrrc_RRCConnectionRelease_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_securityModeCommand)
    {
      if (dcch_msg_ptr->message.u.securityModeCommand.t == T_tdsrrc_SecurityModeCommand_r3)
      {
        transactionid = dcch_msg_ptr->message.u.securityModeCommand.u.r3.securityModeCommand_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.securityModeCommand.t == T_tdsrrc_SecurityModeCommand_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.securityModeCommand.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_signallingConnectionRelease)
    {
      if (dcch_msg_ptr->message.u.signallingConnectionRelease.t == T_tdsrrc_SignallingConnectionRelease_r3)
      {
        transactionid = dcch_msg_ptr->message.u.signallingConnectionRelease.u.r3.signallingConnectionRelease_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.signallingConnectionRelease.t == T_tdsrrc_SignallingConnectionRelease_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.signallingConnectionRelease.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration)
    {
      if (dcch_msg_ptr->message.u.transportChannelReconfiguration.t == T_tdsrrc_TransportChannelReconfiguration_r3)
      {
        transactionid = dcch_msg_ptr->message.u.transportChannelReconfiguration.u.r3.transportChannelReconfiguration_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.transportChannelReconfiguration.t == T_tdsrrc_TransportChannelReconfiguration_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_transportFormatCombinationControl)
    {
      transactionid = dcch_msg_ptr->message.u.transportFormatCombinationControl.rrc_TransactionIdentifier;
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_ueCapabilityEnquiry)
    {
      if (dcch_msg_ptr->message.u.ueCapabilityEnquiry.t == T_tdsrrc_UECapabilityEnquiry_r3)
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.r3.ueCapabilityEnquiry_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.ueCapabilityEnquiry.t == T_tdsrrc_UECapabilityEnquiry_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityEnquiry.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_ueCapabilityInformationConfirm)
    {
      if (dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.t == T_tdsrrc_UECapabilityInformationConfirm_r3)
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.u.r3.ueCapabilityInformationConfirm_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.t == T_tdsrrc_UECapabilityInformationConfirm_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.ueCapabilityInformationConfirm.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_uplinkPhysicalChannelControl)
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation)
    {
      if (dcch_msg_ptr->message.u.utranMobilityInformation.t == T_tdsrrc_UTRANMobilityInformation_r3)
      {
        transactionid = dcch_msg_ptr->message.u.utranMobilityInformation.u.r3.utranMobilityInformation_r3.rrc_TransactionIdentifier;
      }
      else if(dcch_msg_ptr->message.u.utranMobilityInformation.t == T_tdsrrc_UTRANMobilityInformation_later_than_r3)
      {
        transactionid = dcch_msg_ptr->message.u.utranMobilityInformation.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GERANIu)
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_mbmsModifiedServicesInformation)
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_etwsPrimaryNotificationWithSecurity)
    {
      //Not supported
    }
    else if (dcch_msg_ptr->message.t == T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA)
    {
      //Not supported
    }
  }
  else if (dl_sdu_type == tdsrrc_DL_CCCH_Message_PDU)
  {
    ccch_msg_ptr = (tdsrrc_DL_CCCH_Message *) decoded_msg;
    if(ccch_msg_ptr->message.t == T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm )
    {
      if (ccch_msg_ptr->message.u.cellUpdateConfirm.t == T_tdsrrc_CellUpdateConfirm_CCCH_r3)
      {
        transactionid = ccch_msg_ptr->message.u.cellUpdateConfirm.u.r3.cellUpdateConfirm_r3.rrc_TransactionIdentifier;
      }
      else if(ccch_msg_ptr->message.u.cellUpdateConfirm.t == T_tdsrrc_CellUpdateConfirm_CCCH_later_than_r3)
      {
        transactionid = ccch_msg_ptr->message.u.cellUpdateConfirm.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (ccch_msg_ptr->message.t  == T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject)
    {
      if(ccch_msg_ptr->message.u.rrcConnectionReject.t == T_tdsrrc_RRCConnectionReject_r3)	
      {
        transactionid= ccch_msg_ptr->message.u.rrcConnectionReject.u.r3.rrcConnectionReject_r3.rrc_TransactionIdentifier;
      }
      else if(ccch_msg_ptr->message.u.rrcConnectionReject.t == T_tdsrrc_RRCConnectionReject_later_than_r3)
      {
        transactionid = ccch_msg_ptr->message.u.rrcConnectionReject.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (ccch_msg_ptr->message.t  == T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease)
    {
      if(ccch_msg_ptr->message.u.rrcConnectionRelease.t == T_tdsrrc_RRCConnectionRelease_CCCH_r3)
      {
        transactionid= ccch_msg_ptr->message.u.rrcConnectionRelease.u.r3.rrcConnectionRelease_CCCH_r3.rrcConnectionRelease.rrc_TransactionIdentifier;
      }
      else if(ccch_msg_ptr->message.u.rrcConnectionRelease.t == T_tdsrrc_RRCConnectionRelease_CCCH_later_than_r3)
      {
       transactionid = ccch_msg_ptr->message.u.rrcConnectionRelease.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (ccch_msg_ptr->message.t  == T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup)
    {
      if(ccch_msg_ptr->message.u.rrcConnectionSetup.t == T_tdsrrc_RRCConnectionSetup_r3)
      {
        transactionid= ccch_msg_ptr->message.u.rrcConnectionSetup.u.r3.rrcConnectionSetup_r3.rrc_TransactionIdentifier;
      }
      else if(ccch_msg_ptr->message.u.rrcConnectionSetup.t == T_tdsrrc_RRCConnectionSetup_later_than_r3)
      {
        transactionid = ccch_msg_ptr->message.u.rrcConnectionSetup.u.later_than_r3.rrc_TransactionIdentifier;
      }
    }
    else if (ccch_msg_ptr->message.t == T_tdsrrc_DL_CCCH_MessageType_uraUpdateConfirm)
    {
      if(ccch_msg_ptr->message.u.uraUpdateConfirm.t != T_tdsrrc_URAUpdateConfirm_CCCH_later_than_r3)
      {
        transactionid = ccch_msg_ptr->message.u.uraUpdateConfirm.u.r3.uraUpdateConfirm_CCCH_r3.uraUpdateConfirm.rrc_TransactionIdentifier;
      }
      else
      {
        //Not supported
      }
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH," MMAL DL Transaction ID %d ",transactionid );
  return transactionid;

}


/*===========================================================================

FUNCTION tdsrrc_get_ul_log_message

DESCRIPTION
  This function will log the channel type, transaction ID and message type from OTA.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_get_ul_log_message( tdspdu_buf_type encoded_sdu,uint8 channel_type,uint8 message_type,uint8 rb_id,void *msg_ptr,uecomdef_logchan_e_type rrclog_ul_lc)
{
  static tdslog_message_type log_message;
  rlc_lc_id_type ul_rlc_id = TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
  log_tdscdma_trch_type log_tdscdma_trch = TDSCDMA_TRCH_INVALID;

  if(tdsrrc_mmal_log_cb_func!=NULL)
  {
    if((encoded_sdu.value==NULL)||(channel_type==0xFF)||(message_type==0xFF))
    {
      TDSRRC_MSG3(MSG_LEGACY_ERROR,"MMAL Invalid input unencoded msg %x channel_type %d message_type %d",encoded_sdu.value,channel_type,message_type);
      return;
    }
    log_message.direction = 1;
    log_message.channel_type = channel_type;
    log_message.msg_type = message_type;
    log_message.raw_sdu = encoded_sdu.value;
    log_message.raw_sdu_length = encoded_sdu.length;

    if((rrclog_ul_lc == UE_LOGCHAN_DCCH) || (rrclog_ul_lc == UE_LOGCHAN_CCCH))
    {
      log_message.transactionid = tdsrrc_get_ul_transaction_id(msg_ptr,rrclog_ul_lc);
    }
    
    ul_rlc_id = tdsrrclcm_get_ul_rlc_lc_id( rb_id);

    if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND!= ul_rlc_id)
    {
      /* comment out until tdsl2_get_trch_type_for_given_rb_id is implemented in L2 */
      /*log_tdscdma_trch = tdsl2_get_trch_type_for_given_rb_id(rb_id,1,ul_rlc_id);*/
      if(TDSCDMA_TRCH_INVALID != log_tdscdma_trch)
      {
        log_message.transport_channel_type = log_tdscdma_trch;
      }
      else
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"MMAL Invalid TRCH type %d for RLC ID %d ",log_tdscdma_trch,ul_rlc_id);
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MMAL UL RLC ID not found for RB ID %d ",rb_id);
    }

    TDSRRC_MSG2(MSG_LEGACY_HIGH, " MMAL direction %d,msg_type %d",log_message.direction,log_message.transactionid);
    TDSRRC_MSG2(MSG_LEGACY_HIGH," MMAL transaction id %d chan type %d",log_message.msg_type,log_message.channel_type);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"MMAL: UL Transport channel type %d for RB ID %d",log_message.transport_channel_type,rb_id);
    rrc_mmal_log_cb_func(&log_message);
  }
}

/*===========================================================================

FUNCTION tdsrrc_get_dl_log_message

DESCRIPTION
  This function will log the channel type, transaction ID and message type from OTA.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_get_dl_log_message(tdsrrc_cmd_type *cmd_ptr)
{
  static tdslog_message_type dl_log_message;
  uint8 channel_type;
  uint8 rb_id;
  uecomdef_logchan_e_type rrclog_dl_lc;
  rlc_lc_id_type dl_rlc_id = TDSRRCLCM_RLC_LC_ID_NOT_FOUND;
  log_tdscdma_trch_type log_tdscdma_trch = TDSCDMA_TRCH_INVALID;
  dl_log_message.transactionid = 0xFFFFFFFF;

  if(tdsrrc_mmal_log_cb_func!=NULL)
  {
    //check for valid input
    if((cmd_ptr->cmd.downlink_sdu_ind.decoded_msg==NULL)||(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu==NULL))
    {
      TDSRRC_MSG2(MSG_LEGACY_ERROR,"MMAL Invalid input decoded msg %x encoded msg %x",cmd_ptr->cmd.downlink_sdu_ind.decoded_msg,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu);
      return;
    }

    dl_log_message.direction = 0;

    if(TDSRRCLCM_FAILURE== tdsrrclcm_get_dl_lc_type(cmd_ptr->cmd.downlink_sdu_ind.lc_id,
                                 &rrclog_dl_lc))
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MMAL rrclcm_get_dl_lc_type returned FAILURE ");
      return;
    }
  	
    /* Determine and store RB and channel type info */
    rb_id = (uint8)tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id);
    switch(rrclog_dl_lc)
    {
      case UE_LOGCHAN_BCCH:         /* Broadcast Control Channel */
        switch(rb_id)
        {
          case BCCH_S_RADIO_BEARER_ID:
          case BCCH_N_RADIO_BEARER_ID:
            channel_type = TDSRRCLOG_SIG_DL_BCCH_BCH;
            break;
  
          case BCCH_FACH_RADIO_BEARER_ID:
            channel_type = TDSRRCLOG_SIG_DL_BCCH_FACH;
            break;
  
          default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid RB Id: %d for BCCH Log Channel",rb_id);
            channel_type = 0xFF;
            break;
        }
        break;
  
      case UE_LOGCHAN_PCCH:         /* Paging Control Channel.*/
        channel_type = TDSRRCLOG_SIG_DL_PCCH;
        break;
  
      case UE_LOGCHAN_CCCH:         /* Common Control Channel.*/
        channel_type = TDSRRCLOG_SIG_DL_CCCH;
        break;
  
      case UE_LOGCHAN_DCCH:         /* Dedicated Control Channel.*/
        channel_type = TDSRRCLOG_SIG_DL_DCCH;
        break;
  
      default:
        channel_type = 0xFF;
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"MMAL Sig Msg rcvd on invalid Log Chan Type %d",rrclog_dl_lc);
        break;
    }
  
    if(channel_type==0xFF)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MMAL channel_type %d ",channel_type);
      return;
    }

    dl_log_message.channel_type = channel_type;
    dl_log_message.msg_type = tdsrrcdispatch_get_dl_asn1_message_type(cmd_ptr);

    if(dl_log_message.msg_type==0xFF)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MMAL dl_log_message.msg_type %d ",dl_log_message.msg_type);
      return;
    }

    dl_log_message.raw_sdu = cmd_ptr->cmd.downlink_sdu_ind.dl_sdu;
    dl_log_message.raw_sdu_length = cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length;
  
    if((channel_type == TDSRRCLOG_SIG_DL_CCCH)	||(channel_type == TDSRRCLOG_SIG_DL_DCCH))
    {
      dl_log_message.transactionid = tdsrrc_get_dl_transaction_id(cmd_ptr);
    }
  
    if(dl_log_message.transactionid==0xFF)
    {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"MMAL dl_log_message.transactionid %d ",dl_log_message.transactionid);
      return;
    }

    if(channel_type == TDSRRCLOG_SIG_DL_BCCH_BCH)
    {
      dl_log_message.transport_channel_type = TDSCDMA_TRCH_BCH;
    }
    else if (channel_type == TDSRRCLOG_SIG_DL_BCCH_FACH)
    {
      {
        dl_log_message.transport_channel_type = TDSCDMA_TRCH_FACH;
      }
    }
    else if (channel_type == TDSRRCLOG_SIG_DL_PCCH)
    {
      {
        dl_log_message.transport_channel_type = TDSCDMA_TRCH_PCH;
      }
    }
    else
    {
      dl_rlc_id = tdsrrclcm_get_dl_rlc_lc_id( rb_id);
      if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND!= dl_rlc_id)
      {
        /* comment out until tdsl2_get_trch_type_for_given_rb_id is implemented in L2 */
        /*log_tdscdma_trch = tdsl2_get_trch_type_for_given_rb_id(rb_id,0,dl_rlc_id);*/
        if(TDSCDMA_TRCH_INVALID != log_tdscdma_trch)
        {
          dl_log_message.transport_channel_type = log_tdscdma_trch;
        }
        else
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"MMAL Invalid TRCH type %d for RLC ID %d ",log_tdscdma_trch,dl_rlc_id);
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"MMAL DL RLC ID not found for RB ID %d ",rb_id);
      }
    }

    TDSRRC_MSG2(MSG_LEGACY_HIGH, " MMAL direction %d,msg_type %d",dl_log_message.direction,dl_log_message.transactionid); 
    TDSRRC_MSG2(MSG_LEGACY_HIGH," MMAL transaction id %d chan type %d",dl_log_message.msg_type,dl_log_message.channel_type); 
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"MMAL: DL Transport channel type %d for RB ID %d",dl_log_message.transport_channel_type,rb_id);
    rrc_mmal_log_cb_func(&dl_log_message);
  }
  return;			
}
#endif
/*===========================================================================

FUNCTION tdsrrc_register_mmal_cb

DESCRIPTION
  This function is the API to register the function pointer for MMAL.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_register_mmal_cb
(
  TDSRRC_MMAL_LOG_CB_TYPE  *cb_ptr
)
{
  tdsrrc_mmal_log_cb_func = cb_ptr;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"MMAL registering %x with RRC",cb_ptr);
}

#ifdef FEATURE_TDSCDMA_3GPP_FAST_DORMANCY_ENHANCEMENT
/*===========================================================================

FUNCTION   tdsrrc_check_psrab_data_is_present

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
boolean tdsrrc_check_psrab_data_is_present
(
  void
)
{
  uint32 rab_index=0,i=0;    /* Used if there is more than 1 RAB in the message */
  uint32 local_rlc_wm_size;

   /* Check whether this RAB exists in established_rab db */
  for (rab_index=0; rab_index<MAX_RAB_TO_SETUP; rab_index++)
  {
    if ((tdsrrc_est_rabs.rabs[rab_index].cn_domain == RRC_PS_DOMAIN_CN_ID) &&
         (tdsrrc_est_rabs.rabs[rab_index].num_rbs_for_rab > 0) &&
         (tdsrrc_est_rabs.rabs[rab_index].rab_id !=  RRC_INVALID_RAB_ID)
        )
    {
      for(i=0; i< tdsrrc_est_rabs.rabs[rab_index].num_rbs_for_rab; i++)
      {
        local_rlc_wm_size = rlc_rrc_get_wm_info_req(tdsrrclcm_get_ul_rlc_lc_id(
                                                  tdsrrc_est_rabs.rabs[rab_index].rb_for_rab[i].rb_id));
        if (local_rlc_wm_size != 0)
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"WM size is not 0 for rb_id%d",rrc_est_rabs.rabs[rab_index].rb_for_rab[i].rb_id);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}
#endif
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
/*===========================================================================

FUNCTION tdsrrc_send_codec_info_on_activation_timer_expiry

DESCRIPTION
  This function sends codec info to CM on activation timer expiry for L1

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_send_codec_info_on_activation_timer_expiry(void)
{
  boolean ordered_codec_status_flag = FALSE;
  uint32 ordered_codec_info = tdsrrc_get_ordered_codec_mode();
  uint32 current_codec_info = tdsrrc_get_current_codec_mode();

  ordered_codec_status_flag = tdsrrc_get_ordered_codec_status();
  #ifdef FEATURE_INTERRAT_HANDOVER_G2TD
  if(!tdsrrciho_is_g2td_ho_active())
  #endif
  {
    if((TRUE == ordered_codec_status_flag))
    {
      if(FALSE == tdsrrc_is_voice_rab_present() || (ordered_codec_info != current_codec_info))
      {
        /*Sending codec info to CM*/
        tdsrrc_send_codec_info_to_cm(ordered_codec_info, TDSRRC_SAMPLING_RATE_NB);
      }
      else
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"is_voice_rab %d, ordered codec %d current codec %d",tdsrrc_is_voice_rab_present(),ordered_codec_info,current_codec_info);
      }
      tdsrrc_set_ordered_codec_status(FALSE);
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Activation type is %d,ordered config status %d",tdsordered_config_ptr->act_time_type,ordered_codec_status_flag);
    }
  }
}
/*===========================================================================

FUNCTION tdsrrc_register_rrc_codec_ho_info_cb

DESCRIPTION
  This function is the API to register the function pointer for CM call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_register_rrc_codec_ho_info_cb
(
  TDSRRC_CODEC_HO_INFO_CB_FUNC_TYPE *cb_ptr
)
{
  tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func = cb_ptr;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CM  registering %x with RRC",cb_ptr);
}
/*===========================================================================

FUNCTION tdsrrc_deregister_rrc_codec_ho_info_cb

DESCRIPTION
  This function is the API to de-register the function pointer for CM call back.

DEPENDENCIES


RETURN VALUE
  log_message_type

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_deregister_rrc_codec_ho_info_cb (void)
{
  if(tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func != NULL)
  {
    tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func = NULL;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CM  de-registering with RRC");
  }

}
/*===========================================================================

FUNCTION tdsrrc_send_hho_cmd_to_cm

DESCRIPTION


  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_send_hho_cmd_to_cm(sys_codec_ho_cmd cmd)
{
  sys_codec_ho_info sys_mode_codec_ho_info = {0};

  sys_mode_codec_ho_info.as_id = tdsrrc_get_as_id();

  if((cmd != SYS_HO_START)&&(tdsrrcdata_codec_info.is_hho_started))
  {
    tdsrrcdata_codec_info.is_hho_started = FALSE;
    if(tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func != NULL)
    {
      sys_mode_codec_ho_info.cmd = cmd;
      tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func(sys_mode_codec_ho_info);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent HHO cmd to CM: %d ",cmd);
    }
  }
  else if (cmd == SYS_HO_START)
  {
    tdsrrcdata_codec_info.is_hho_started = TRUE;
    if(tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func != NULL)
    {
      sys_mode_codec_ho_info.cmd = cmd;
      tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func(sys_mode_codec_ho_info);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sent HHO cmd to CM: %d ",cmd);
    }
  }
}

/*===========================================================================

FUNCTION tdsrrc_send_codec_info_to_cm

DESCRIPTION


  
DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_send_codec_info_to_cm(uint32 codec, uint32 amr_nb_sampling_rate)
{
  sys_codec_ho_info sys_mode_codec_ho_info;

  sys_mode_codec_ho_info.cmd = SYS_SPEECH_CODEC_INFO;  

  sys_mode_codec_ho_info.as_id = tdsrrc_get_as_id();
  
  if(codec == TDSCDMA_IVOCODER_ID_AMR)
  {
    sys_mode_codec_ho_info.speech_codec = SYS_SPEECH_CODEC_AMR_NB;
    sys_mode_codec_ho_info.speech_enc_samp_freq = amr_nb_sampling_rate;
  }
  else if(codec == TDSCDMA_IVOCODER_ID_AMRWB)
  {
    sys_mode_codec_ho_info.speech_codec = SYS_SPEECH_CODEC_AMR_WB;
    sys_mode_codec_ho_info.speech_enc_samp_freq = TDSRRC_SAMPLING_RATE_WB;
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid codec type received 0x%x ",codec);
    return;
  }
  if(tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func != NULL)
  {    
    tdsrrcdata_codec_info.rrc_codec_ho_info_cb_func(sys_mode_codec_ho_info);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sent codec info to CM: codec 0x%x, samp_freq %d", codec, sys_mode_codec_ho_info.speech_enc_samp_freq);
  }
}

#else
/*Dummy function for CM de/registration*/

/*===========================================================================

FUNCTION tdsrrc_register_rrc_codec_ho_info_cb -- Dummy API

DESCRIPTION
  

DEPENDENCIES


RETURN VALUE
  

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_register_rrc_codec_ho_info_cb
(
  TDSRRC_CODEC_HO_INFO_CB_FUNC_TYPE *cb_ptr
)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CM  registering %x with RRC. This is DUMMY API!",cb_ptr);
}
/*===========================================================================

FUNCTION tdsrrc_deregister_rrc_codec_ho_info_cb -- Dummy API

DESCRIPTION
  

DEPENDENCIES


RETURN VALUE
  

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_deregister_rrc_codec_ho_info_cb (void)
{

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CM  de-registering with RRC. This is DUMMY API!");
}
#endif

/*===========================================================================

FUNCTION tdsrrc_tv_send_sample_rate

DESCRIPTION

  This function is the API to check and notified the voice sampling rate.

DEPENDENCIES


RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_tv_send_sample_rate(
  tdscdma_ivoice_cmd_send_sample_rate_t *sample_rate_cmd
)
{
  sys_modem_as_id_e_type rrc_as_id = sample_rate_cmd->handle;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: prepare to send sample rate with as_id %d",rrc_as_id);

#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
  if(TRUE == tdsrrc_is_voice_rab_present())
  {
     tdsrrc_send_codec_info_to_cm(tdsrrc_get_current_codec_mode(), 
                                  sample_rate_cmd->sample_rate);
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TV_TRRC: Ignoring update codec info from NAS as voice rab is not up ",0,0,0);
  }
#else
  TDSRRC_MSG3(MSG_LEGACY_ERROR,"This is dummy API function, rate:%d", sample_rate_cmd->sample_rate, 0, 0);
#endif
}
/*Reserved for future use*/
#ifdef FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO
/*===========================================================================

FUNCTION  tdsrrc_validate_srns_relocation_info

DESCRIPTION
  This function is used to check if the necessary IEs are all included in the
  OTA messages. 

DEPENDENCIES


RETURN VALUE
  TRUE: the OTA is used to trigger a SRNS relocation
  FALSE: Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrc_validate_srns_relocation_info(tdsrrc_cmd_type *cmd_ptr, uint8 msg_type)
{
  tdsrrc_DL_DCCH_Message     *dcch_msg_ptr = NULL;   /* Pointer to the DL DCCH message */
  tdsrrc_DL_CCCH_Message     *ccch_msg_ptr = NULL; /* Pointer to  downlink CCCH message */

  dcch_msg_ptr = (tdsrrc_DL_DCCH_Message *)cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;

  if (TRUE == tdsrrcsmc_is_integrity_prot_mode_info_present(
                                            cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, msg_type))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"FRESH included for msg_type %d,it is for SRNS relocation",msg_type);
    return TRUE;
  }
  switch (msg_type)
  {

     case T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm:
       /*For CU, does not need to check dl_counter_sync_info_present as it has been taken care already*/
      ccch_msg_ptr = (tdsrrc_DL_CCCH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
      if(TRUE == tdsrrccu_validate_srns_relocation_info(ccch_msg_ptr, tdsrrc_DL_CCCH_Message_PDU))
      {
        return TRUE;
      }
      break;

     case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
      if(TRUE == tdsrrccu_validate_srns_relocation_info(dcch_msg_ptr, tdsrrc_DL_DCCH_Message_PDU))
      {
        return TRUE;
      }
      break;

     case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"RBReconfiguration should not call this function as its implementation works fine.");
       return TRUE;
       break;

     case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
       
       if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL4))  
       {
         
         if(TRUE == tdsrrcrbr_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerRelease), TDSMSG_REL4))
         {
           return TRUE;
         }
       }
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
           (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
              T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL5)))
       {
         if(TRUE == tdsrrcrbr_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerRelease), TDSMSG_REL5))
         {
            return TRUE;
         }
       }
       /* Check if Rel 6 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL6)))
       {
         if(TRUE == tdsrrcrbr_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerRelease), TDSMSG_REL6))
         {
            return TRUE;
         }
       }
       /* Check if Rel 7 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL7)))
       {
         if(TRUE == tdsrrcrbr_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerRelease), TDSMSG_REL7))
         {
            return TRUE;
         }
       }
#ifdef FEATURE_TDSCDMA_REL8
       /* Check if Rel 8 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL8)))
       {
         if(TRUE == tdsrrcrbr_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerRelease), TDSMSG_REL8))
         {
            return TRUE;
         }
       }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
       /* Check if Rel 9 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
              T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease, TDSMSG_REL9)))
       {
         if(TRUE == tdsrrcrbr_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerRelease), TDSMSG_REL9))
         {
            return TRUE;
         }
       }
#endif /* FEATURE_TDSCDMA_REL9 */
      else
      {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wrong RBR version,expected IP check failure");
          return FALSE;
      }
      break;

     case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:

       if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL4))  
       {
         
         if(TRUE == tdsrrcrbe_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerSetup), TDSMSG_REL4))
         {
           return TRUE;
         }
       }
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
           (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
              T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL5)))
       {
         if(TRUE == tdsrrcrbe_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerSetup), TDSMSG_REL5))
         {
            return TRUE;
         }
       }
       /* Check if Rel 6 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL6)))
       {
         if(TRUE == tdsrrcrbe_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerSetup), TDSMSG_REL6))
         {
            return TRUE;
         }
       }
       /* Check if Rel 7 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL7)))
       {
         if(TRUE == tdsrrcrbe_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerSetup), TDSMSG_REL7))
         {
            return TRUE;
         }
       }
#ifdef FEATURE_TDSCDMA_REL8
       /* Check if Rel 8 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL8)))
       {
         if(TRUE == tdsrrcrbe_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerSetup), TDSMSG_REL8))
         {
            return TRUE;
         }
       }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
       /* Check if Rel 9 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
              T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup, TDSMSG_REL9)))
       {
         if(TRUE == tdsrrcrbe_validate_srns_relocation_info(&(dcch_msg_ptr->message.u.radioBearerSetup), TDSMSG_REL9))
         {
            return TRUE;
         }
       }
#endif /* FEATURE_TDSCDMA_REL9 */
      else
      {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wrong RBE version,expected IP check failure");
          return FALSE;
      }

       break;

     case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
       if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL4))  
       {

         if(TRUE == tdsrrctcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.r4.transportChannelReconfiguration_r4), TDSMSG_REL4))
         {
           return TRUE;
         }
       }
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
           (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
              T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL5)))
       {
         if(TRUE == tdsrrctcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.r5.transportChannelReconfiguration_r5), TDSMSG_REL5))
         {
            return TRUE;
         }
       }
       /* Check if Rel 6 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL6)))
       {
         if(TRUE == tdsrrctcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.
                                                              criticalExtensions.u.r6.transportChannelReconfiguration_r6), TDSMSG_REL6))
         {
            return TRUE;
         }
       }
       /* Check if Rel 7 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL7)))
       {
         if(TRUE == tdsrrctcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.
                                                              criticalExtensions.u.criticalExtensions.u.r7.transportChannelReconfiguration_r7), TDSMSG_REL7))
         {
            return TRUE;
         }
       }
#ifdef FEATURE_TDSCDMA_REL8
       /* Check if Rel 8 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL8)))
       {
         if(TRUE == tdsrrctcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                                                                   criticalExtensions.u.criticalExtensions.u.r8.transportChannelReconfiguration_r8), TDSMSG_REL8))
         {
            return TRUE;
         }
       }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
       /* Check if Rel 9 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
              T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration, TDSMSG_REL9)))
       {
         if(TRUE == tdsrrctcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.transportChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                                                                   criticalExtensions.u.criticalExtensions.u.
                                                                   criticalExtensions.u.r9.transportChannelReconfiguration_r9), TDSMSG_REL9))
         {
            return TRUE;
         }
       }
#endif /* FEATURE_TDSCDMA_REL9 */
      else
      {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wrong PCRC version,expected IP check failure");
          return FALSE;
      }
      return FALSE;
      break;  

     case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
       if (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
         T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL4))  
       {
         
         if(TRUE == tdsrrcpcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.r4.physicalChannelReconfiguration_r4), TDSMSG_REL4))
         {
           return TRUE;
         }
       }
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5) &&
           (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
              T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL5)))
       {
         if(TRUE == tdsrrcpcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.r5.physicalChannelReconfiguration_r5), TDSMSG_REL5))
         {
            return TRUE;
         }
       }
       /* Check if Rel 6 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL6)))
       {
         if(TRUE == tdsrrcpcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.
                                                              criticalExtensions.u.r6.physicalChannelReconfiguration_r6), TDSMSG_REL6))
         {
            return TRUE;
         }
       }
       /* Check if Rel 7 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL7) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL7)))
       {
         if(TRUE == tdsrrcpcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.
                                                              criticalExtensions.u.criticalExtensions.u.r7.physicalChannelReconfiguration_r7), TDSMSG_REL7))
         {
            return TRUE;
         }
       }
#ifdef FEATURE_TDSCDMA_REL8
       /* Check if Rel 8 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL8) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
             T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL8)))
       {
         if(TRUE == tdsrrcpcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                                                                   criticalExtensions.u.criticalExtensions.u.r8.physicalChannelReconfiguration_r8), TDSMSG_REL8))
         {
            return TRUE;
         }
       }
#endif /* FEATURE_TDSCDMA_REL8 */
#ifdef FEATURE_TDSCDMA_REL9
       /* Check if Rel 9 contents are present */
       else if ((tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL9) &&
            (SUCCESS == tdsrrc_dcch_msg_com_critical_ext_present(dcch_msg_ptr, 
              T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration, TDSMSG_REL9)))
       {
         if(TRUE == tdsrrcpcrc_validate_srns_relocation_info_new(&(dcch_msg_ptr->message.u.physicalChannelReconfiguration.u.later_than_r3.
                                                                   criticalExtensions.u.criticalExtensions.u.criticalExtensions.u.
                                                                   criticalExtensions.u.criticalExtensions.u.
                                                                   criticalExtensions.u.r9.physicalChannelReconfiguration_r9), TDSMSG_REL9))
         {
            return TRUE;
         }
       }
#endif /* FEATURE_TDSCDMA_REL9 */
      else
      {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wrong PCRC version,expected IP check failure");
          return FALSE;
      }
       return FALSE;
       break;

     case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"CMCC agreed NOT use UMI to trigger SRNS relocation.");
       return FALSE;
       break;

     case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"CMCC agreed NOT use URAUpdateConfirm to trigger SRNS relocation.");
       return FALSE;
       break;

     default:
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"msg_type %d,it should not be included for SRNS relocation OTA",msg_type);
       return FALSE;

  }
  return FALSE;

}


/*===========================================================================

FUNCTION  tdsrrc_is_fresh_in_reconfiguration_message

DESCRIPTION
  This function is used to check if the FRESH value is in the
  OTA messages.
 
  The function should be called after ASN.1 decoding, i.e.
  in the procedure modules. 

DEPENDENCIES


RETURN VALUE
  TRUE: the OTA has the FRESH carried in. 
  FALSE: Otherwise

SIDE EFFECTS
  None
===========================================================================*/
boolean tdsrrc_is_fresh_in_reconfiguration_message(void)
{
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Fresh in last OTA is %d",tdsrrcsmc_is_fresh_in_ota);
  return tdsrrcsmc_is_fresh_in_ota; 
}

#endif /*FEATURE_TDSCDMA_SRNS_WO_DL_COUTER_SYNC_INFO*/

#ifdef FEATURE_TDSCDMA_HSUPA
/*===========================================================================

FUNCTION tdsrrc_event_log_hsupa_support_status

DESCRIPTION

  Event logging for HSUPA support status
  
DEPENDENCIES


RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_event_log_hsupa_support_status(sys_hs_ind_e_type hsdpa_hsupa_indication)
{
  tdsrrclog_hsupa_support_status_type hsupa_status;

  hsupa_status.primary_freq = (uint16)tdsrrcccm_get_curr_camped_freq();
  hsupa_status.cpid = (uint8)tdsrrcccm_get_curr_camped_cpid();

  if ((hsdpa_hsupa_indication == SYS_HS_IND_HSUPA_SUPP_CELL) ||
      (hsdpa_hsupa_indication == SYS_HS_IND_HSDPA_HSUPA_SUPP_CELL) ||
      (hsdpa_hsupa_indication == SYS_HS_IND_HSDPAPLUS_HSUPA_SUPP_CELL) ||
      (hsdpa_hsupa_indication == SYS_HS_IND_DC_HSDPAPLUS_HSUPA_SUPP_CELL) ||
      (hsdpa_hsupa_indication == SYS_HS_IND_HSDPAPLUS_64QAM_HSUPA_SUPP_CELL)) 
  {
     hsupa_status.upa_indicator = 1;
  }
  else
  {
     hsupa_status.upa_indicator = 0;
  }

  event_report_payload(EVENT_TDSCDMA_RRC_HSUPA_SUPPORT_STATUS,
                       sizeof(hsupa_status),
                       (void *)&hsupa_status);

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"HSUPA_EVENT: primary_freq = %d, cpid = %d, support status = %d", 
              hsupa_status.primary_freq, hsupa_status.cpid, hsupa_status.upa_indicator);
}
#endif

/*===========================================================================

FUNCTION tdsrrc_get_num_prach_power_increasing_times

DESCRIPTION

  This function is called by L1 to either get V300 value during connection
  setup or V302 value during cell update
  
DEPENDENCIES


RETURN VALUE

  Number of times UE increases PRACH power

SIDE EFFECTS

  None

===========================================================================*/
uint8 tdsrrc_get_num_prach_power_increasing_times(void)
{
  /* Return tdsrrc_v_300 if during connection setup, otherwise return tdsrrc_v_302
     return values should be started from 0
  */
  if(tdsrrc_get_state() == TDSRRC_STATE_CONNECTING)
  {
    if (tdsrrc_v_300 == 0) 
    {
      return (uint8)tdsrrc_v_300;
    }
    else
    {
      return ((uint8)tdsrrc_v_300 - 1);
    }
  }
  else
  {
    if (tdsrrc_v_302 == 0) 
    {
      return (uint8)tdsrrc_v_302;
    }
    else
    {
      return ((uint8)tdsrrc_v_302 - 1);
    }
  }
}

/*===========================================================================

FUNCTION tdsrrc_public_mem_malloc

DESCRIPTION

  This function is called by segment loading platform to use RRC memory alloc
  
DEPENDENCIES


RETURN VALUE

  A pointer to the allocated memory if the memory was successfully allocated,
  otherwise NULL.

SIDE EFFECTS

  None

===========================================================================*/
void *tdsrrc_public_mem_malloc(size_t size)
{
  return tdsrrc_malloc(size);
}

/*===========================================================================

FUNCTION tdsrrc_public_mem_free

DESCRIPTION

  This function is called by segment loading platform to use RRC memory free
  
DEPENDENCIES


RETURN VALUE

  The memory manager must have been previously initialized.

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_public_mem_free(void *ptr)
{
  tdsrrc_free(ptr);
}

/*===========================================================================

FUNCTION tdsrrc_max_allowed_ps_data_rate

DESCRIPTION

  This function is used to acquire UL maximum allowed data rate for
  PS service.
  
DEPENDENCIES


RETURN VALUE

  UL maximum allowed data rate in kbps

SIDE EFFECTS

  None

===========================================================================*/
uint32 tdsrrc_max_allowed_ps_data_rate(void)
{
  uint32 rate = 0;
  uint8 rab_count, rb_count;

  /* If UE is in idle or dormancy, return 0 */
  if ((tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED) ||
      (tdsrrc_get_state() == TDSRRC_STATE_CONNECTING) ||
      (tdsrrcdt_is_ps_domain_open() == FALSE))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"RRC state %d or RRC is in dormancy,UL PS data rate %d",tdsrrc_get_state(),rate);
    return rate;
  }

  /* Loop established RABs and calculate UL data rate for PS RAB only */
  for (rab_count = 0; 
       (rab_count < MAX_RAB_TO_SETUP) && (tdsrrc_est_rabs.rabs[rab_count].rab_id != TDSRRC_INVALID_RAB_ID); 
       rab_count++) 
  {
    if ((tdsrrc_est_rabs.rabs[rab_count].cn_domain == RRC_PS_DOMAIN_CN_ID) &&
        (tdsrrc_est_rabs.rabs[rab_count].num_rbs_for_rab > 0))
    {
      for (rb_count = 0; rb_count < MAX_RB_PER_RAB; rb_count++) 
      {
	if (tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id != TDSRRC_INVALID_RB_ID) 
        {
          rate += tdsrrcllc_get_ul_ps_data_rate(tdsrrc_get_state(), tdsrrc_est_rabs.rabs[rab_count].rb_for_rab[rb_count].rb_id);
        }
      }
    }
  }
  // Convert from bps to kbps
  return rate/1000;
}

/*===========================================================================
FUNCTION tdsrrc_get_ul_rate_info

DESCRIPTION

  This function is called by DS to acquire UL maximum allowed data rate for
  PS service and plus_flag.
  
DEPENDENCIES


RETURN VALUE

  UL rate info for DS

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_get_ul_rate_info(tdsrrc_ul_rate_info_type *ul_rate_info_ptr)
{
  if(ul_rate_info_ptr != NULL) 
  {
    ul_rate_info_ptr->max_allowed_ul_rate = tdsrrc_max_allowed_ps_data_rate();
    ul_rate_info_ptr->plus_flag = tdsl1_get_ul_plus_flag();
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"DS API: max ul rate: %dkbps,plus_flag = %d",ul_rate_info_ptr->max_allowed_ul_rate,ul_rate_info_ptr->plus_flag);
  }
  else
  {
     TDSRRC_MSG0(MSG_LEGACY_ERROR,"DS API: ul_rate_info_ptr is NULL!");
  }
}

#ifdef FEATURE_DUAL_WCDMA

/*===========================================================================
FUNCTION tdsrrc_get_ul_rate_info

DESCRIPTION

  This function is called by DS to acquire UL maximum allowed data rate for
  PS service and plus_flag.
  
DEPENDENCIES


RETURN VALUE

  UL rate info for DS

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_get_ul_rate_info_sub(sys_modem_as_id_e_type as_id, tdsrrc_ul_rate_info_type *ul_rate_info_ptr)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR, "AS-id mismatch %d,internal %d",as_id,tdsrrc_get_as_id());
  }
  return tdsrrc_get_ul_rate_info(ul_rate_info_ptr);
}
#endif
/*===========================================================================
FUNCTION tdsrrc_get_ul_tx_power_range

DESCRIPTION

  This function is called by DS to get UL Tx power range
  
DEPENDENCIES


RETURN VALUE

  UL Tx power range (LOW/MEDIUM/HIGH)

SIDE EFFECTS

  None

===========================================================================*/
tdsrrc_ul_tx_power_range_e_type tdsrrc_get_ul_tx_power_range(void)
{
  tdsrrc_ul_tx_power_range_e_type ul_tx_power_range;
  ul_tx_power_range = (tdsrrc_ul_tx_power_range_e_type) tdsl1_get_ul_tx_power_range();
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DS API: ul_tx_power_range: %d",ul_tx_power_range);

  return ul_tx_power_range;
}

/*===========================================================================

FUNCTION tdsrrc_get_ul_data_rate_for_rab

DESCRIPTION

  This function returns the UL data rate for 
  DS. 
  
DEPENDENCIES


RETURN VALUE

  Actual Rate if RAB exists
  0 Otherwise 

SIDE EFFECTS

  None

===========================================================================*/

uint32 tdsrrc_get_ul_data_rate_for_rab(
   uint8 rab_id,
   sys_modem_as_id_e_type subs_id  /*Assume DSDS is always on for interface*/
)
{
  uint8 count = 0; 
  uint8 count1= 0;
  tdsrrc_state_e_type rrc_state;
  tdsrrcllc_oc_set_status_e_type     oc_status;
  tdsrrc_proc_e_type                 proc_id;
  tdsrrcllc_oc_process_state_e_type  process_state;  

#ifdef FEATURE_DUAL_SIM  
  if (subs_id != tdsrrc_get_as_id())
  {

    TDSRRC_MSG3(MSG_LEGACY_ERROR,"as_id:%d wrong for rab:%d, current id is:%d. Return all ZERO",
                    subs_id, rab_id, tdsrrc_get_as_id());
    return 0;
  }
#else
  if (subs_id != 0)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"as_id:%d is not 0 for rab:%d in NON-DSDS case. Ignore the as_id",subs_id,rab_id);
  }
#endif

    oc_status = tdsrrcllc_get_ordered_config_state_and_proc(&proc_id, &process_state);

  switch (oc_status) 
  {
    case TDSOC_SET_FOR_CELL_FACH:
    case TDSOC_SET_FOR_DCH_FACH_TRANS:
      rrc_state = TDSRRC_STATE_CELL_FACH;
      break;
    case TDSOC_SET_FOR_CELL_DCH:
      rrc_state = TDSRRC_STATE_CELL_DCH;
      break;
    case TDSOC_SET_FOR_URA_PCH:
    case TDSOC_SET_FOR_DCH_URA_PCH_TRANS:
    case TDSOC_SET_FOR_FACH_URA_PCH_TRANS:
      rrc_state = TDSRRC_STATE_URA_PCH;
      break;
    case TDSOC_SET_FOR_CELL_PCH:
    case TDSOC_SET_FOR_DCH_CELL_PCH_TRANS:
    case TDSOC_SET_FOR_FACH_CELL_PCH_TRANS:
      rrc_state = TDSRRC_STATE_CELL_PCH;
      break;
    default:
      rrc_state = tdsrrc_get_state();
      break;
  }
  
  for (count = 0; count < MAX_RAB_TO_SETUP; count ++) 
  {
    if((tdsrrc_est_rabs.rabs[count].rab_id == rab_id)
        && (tdsrrc_est_rabs.rabs[count].rab_id != TDSRRC_INVALID_RAB_ID) 
        && (tdsrrc_est_rabs.rabs[count].num_rbs_for_rab > 0)
        && (RRC_PS_DOMAIN_CN_ID == tdsrrc_est_rabs.rabs[count].cn_domain))
    {
      for (count1 = 0; ((count1 < tdsrrc_est_rabs.rabs[count].num_rbs_for_rab) &&
                        (count1 < MAX_RB_PER_RAB)) ; count1++) 
      {
        return tdsrrcllc_get_ul_ded_rate(rrc_state, (uint16)tdsrrc_est_rabs.rabs[count].rb_for_rab[count1].rb_id); 

      }
    }
  }
  return 0;
}

#ifdef FEATURE_MODEM_CONFIG_REFRESH
/*===========================================================================

FUNCTION tdsrrc_register_mcfg_refresh_notify

DESCRIPTION

  This function registers notify callback for MCFG refresh event. 
  
DEPENDENCIES


RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_register_mcfg_refresh_notify(void)
{
    mcfg_refresh_registration_s_type reg_info;

	reg_info.order = MCFG_REFRESH_ORDER_50;
	reg_info.slot_mask = MCFG_REFRESH_INDEX_MASK_NONE;
	reg_info.sub_mask = MCFG_REFRESH_INDEX_MASK_ALL;
	reg_info.cb = tdsrrc_mcfg_refresh_cb;
	if(mcfg_refresh_register(&reg_info))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"registered mcfg refresh cb successfully");
    }
	else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"failed to register mcfg refresh cb");
    }
}

/*===========================================================================

FUNCTION tdsrrc_mcfg_refresh_cb

DESCRIPTION

  This function is called when MCFG refresh event happens. 
  
DEPENDENCIES


RETURN VALUE

TRUE - successfully saved mcfg_refresh info to tdsrrc context.
FALSE - otherwise.

SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrc_mcfg_refresh_cb(mcfg_refresh_info_s_type *p_info)
{
  tdsrrc_cmd_type *cmd_ptr;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrc_mcfg_refresh_cb is called");

  /* Get a RRC internal command buffer */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) == NULL)
  {
    ERR_FATAL("No CMD buffer for TDSRRC_MCFG_REFRESH_IND",0,0,0);
  }

  /* Fill in the command id  */
  cmd_ptr->cmd_hdr.cmd_id = TDSRRC_MCFG_REFRESH_IND;

  memscpy(&cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info,sizeof(mcfg_refresh_info_s_type),
  	      p_info,sizeof(mcfg_refresh_info_s_type));
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"sending TDSRRC_MCFG_REFRESH_IND to internal queue");
  /* Put the command on the internal queue */
  tdsrrc_put_int_cmd( cmd_ptr );
  return TRUE;
}

/*===========================================================================

FUNCTION tdsrrc_process_mcfg_refresh_ind

DESCRIPTION

  This function processes tdsrc internal msg TDSRRC_MCFG_REFRESH_IND, and calls MCFG API to notify TDS completes
  handling MCFG refresh event. Currently this function only logs the mcfg refresh event info without re-reading NVs, since
  we only have one MCFG setting for TDS.
  
DEPENDENCIES


RETURN VALUE

None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_process_mcfg_refresh_ind(tdsrrc_cmd_type *cmd_ptr)
{
  mcfg_refresh_index_type  sub_index;
  sys_modem_as_id_e_type as_id;
  
  TDSRRC_MSG4(MSG_LEGACY_HIGH,"process mcf refresh ind, reference_id 0x%x, refresh type %d, slot_index %d, sub_index %d",
  	          cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info.reference_id,
  	          cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info.type,
  	          cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info.slot_index,
  	          cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info.sub_index); 

  sub_index = cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info.sub_index;

  if(cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info.sub_index != MCFG_REFRESH_INDEX_0
      && cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info.sub_index != MCFG_REFRESH_INDEX_1)
  {
    /* ignore it if sub_index is not MCFG_REFRESH_INDEX_0 and MCFG_REFRESH_INDEX_1. */
  } 
  else if(tdsrrcmcm_get_rrc_mode() == TDSRRC_MODE_INACTIVE)
  {
    /* As per the requirement, TDS will be put to inactive before receiving the NV refresh callback */

    if(sub_index == MCFG_REFRESH_INDEX_0)
    {
      as_id = SYS_MODEM_AS_ID_1;
    }
    else if(sub_index == MCFG_REFRESH_INDEX_1)
    {
      as_id = SYS_MODEM_AS_ID_2;
    }

    tdsrrcnv_read_nv_refresh(as_id); 
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"receive TDSRRC_MCFG_REFRESH_IND when rrc is not in inactive mode");
  } 

  if(mcfg_refresh_done(cmd_ptr->cmd.mcfg_refresh_ind.mcfg_refresh_info.reference_id))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"notified mcfg task for refresh done");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"failed to notify mcfg task for refresh done");
  }
}
#endif


/*===========================================================================

FUNCTION tdsrrc_get_cell_info

DESCRIPTION
 This function can return below cell information:
 Scell:
  -	PLMN id
  -	LAC
  -	UARFCN
  -	Cell_id
  -	Cell_parameter_id
  -	RSCP

Ncell
  -	UARFCN
  -	Cell_parameter_id
  -	RSCP


DEPENDENCIES
 Caller must provide the memory space for the input parameter.

RETURN VALUE
 FALSE: Memory is NULL.
 TRUE: The information is valid.
 
SIDE EFFECTS
 None

===========================================================================*/
boolean tdsrrc_get_cell_info 
(
  tdsrrc_cell_info_type*  cell_info_ptr
)
{
  rrc_plmn_identity_type plmn_id;
  tdsl1_cell_meas_result scell_result;
  tdsl1_cell_meas_result ncell_result[TDSCDMA_RRC_CM_MAX_NBR_CELL_NUM];
  tds_l1_scell_signal_status_ind_type l1_scell_signal_status_ind;
  uint8 num_ncell = 0;
  rrc_lai_type lai;
  uint8 i;

  if (cell_info_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"null cmd_ptr");
    return FALSE;
  }
  
  memset(cell_info_ptr, 0x00, sizeof(tdsrrc_cell_info_type));
  memset(&scell_result, 0x00, sizeof(tdsl1_cell_meas_result));
  memset(ncell_result, 0x00, TDSCDMA_RRC_CM_MAX_NBR_CELL_NUM * sizeof(tdsl1_cell_meas_result));

  if((tdsrrcmcm_get_rrc_mode() != TDSRRC_MODE_ACTIVE)
     || (tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_MCMR) != TDSRRC_CAMPED_ON))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"triggered when not camped on,rrc mode %d",tdsrrcmcm_get_rrc_mode());
    
    cell_info_ptr->success_flag = FALSE;
  }
  else
  {
    do
    {
      /* Get serving cell information from L1 */
      if(tdsl1_get_scell_meas_result(&scell_result))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Received tdsl1_get_scell_meas_result from L1");

        plmn_id = tdsrrcccm_get_curr_camped_plmn();
        for(i = 0; i < 3; i++)
        {
          cell_info_ptr->serv_cell.mcc[i] = plmn_id.mcc[i];
          cell_info_ptr->serv_cell.mnc[i] = plmn_id.mnc[i];
        }
        cell_info_ptr->serv_cell.num_mnc_digits = plmn_id.num_mnc_digits;

        cell_info_ptr->serv_cell.cell_id = tdsrrcccm_get_curr_camped_cell_id();

        if(SUCCESS == tdsrrc_get_lac_id(&lai))
        {
          cell_info_ptr->serv_cell.lac[0] = lai.lac[0];
          cell_info_ptr->serv_cell.lac[1] = lai.lac[1];
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"failed to get LAC ids");
          cell_info_ptr->success_flag = FALSE;
          break;
        }

        /* Copy serving cell's measurement results */
        cell_info_ptr->serv_cell.uarfcn = scell_result.uarfcn;
        cell_info_ptr->serv_cell.cell_parameter_id = scell_result.cpid;
        cell_info_ptr->serv_cell.rscp = scell_result.rscp;
        cell_info_ptr->serv_cell.timing_advance = scell_result.timing_advance;

        memset(&l1_scell_signal_status_ind, 0, sizeof(tds_l1_scell_signal_status_ind_type));
        /* Get signal status value from L1 through api function below */
        tdssrch_get_serving_cell_signal_strength_info(&l1_scell_signal_status_ind);
        cell_info_ptr->serv_cell.rssi = l1_scell_signal_status_ind.rssi;
        cell_info_ptr->serv_cell.ecio = l1_scell_signal_status_ind.ecio;

        cell_info_ptr->success_flag = TRUE;
        
        TDSRRC_MSG4(MSG_LEGACY_HIGH,"Serving cell freq=%d, cpid=%d, cell_ID=%d, rscp=%d",
                    cell_info_ptr->serv_cell.uarfcn,
                    cell_info_ptr->serv_cell.cell_parameter_id,
                    cell_info_ptr->serv_cell.cell_id,
                    cell_info_ptr->serv_cell.rscp);
        
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Serving cell timing_advance=%d, rssi=%d, ecio=%d",
                    cell_info_ptr->serv_cell.timing_advance,
                    cell_info_ptr->serv_cell.rssi,
                    cell_info_ptr->serv_cell.ecio);

        tdsrrc_print_plmn_id(plmn_id);

        {
          uint16 lac = cell_info_ptr->serv_cell.lac[0];

          lac = (lac << 8) | cell_info_ptr->serv_cell.lac[1];
          
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Serving cell lac = 0x%x (%d)",lac,lac);        
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"failed to get scell's info");
        cell_info_ptr->success_flag = FALSE;
        break;
      }

      /* Get neighbor cells information from L1 */
      if(tdsl1_get_ncell_meas_result(&num_ncell, ncell_result))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received  from L1,num_ncell=%d",num_ncell);

        cell_info_ptr->num_ncell = num_ncell;

        if (num_ncell)
        {
           cell_info_ptr->nbr_success_flag = TRUE;
        }
        else
        {
           cell_info_ptr->nbr_success_flag = FALSE;
           break;
        }


        for(i = 0; (i < num_ncell) && (i < TDSCDMA_RRC_CM_MAX_NBR_CELL_NUM); i++)
        {
          /* Copy neighbor cells' measurement results */
          cell_info_ptr->nbr_ncell[i].uarfcn = ncell_result[i].uarfcn;
          cell_info_ptr->nbr_ncell[i].cell_parameter_id = ncell_result[i].cpid;
          cell_info_ptr->nbr_ncell[i].rscp = ncell_result[i].rscp;

          TDSRRC_MSG4(MSG_LEGACY_HIGH,"Neighbor[%d]: freq=%d, cpid=%d, rscp=%d",
                      i,
                      cell_info_ptr->nbr_ncell[i].uarfcn,
                      cell_info_ptr->nbr_ncell[i].cell_parameter_id,
                      cell_info_ptr->nbr_ncell[i].rscp);
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"failed to get ncells' info");
        cell_info_ptr->nbr_success_flag = FALSE;
      }
    } while(0);
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Put the cell infrom to CM, sucess_flag=%d,nbr_flag=%d.",cell_info_ptr->success_flag,cell_info_ptr->nbr_success_flag);
  return TRUE;
}

/*===========================================================================

FUNCTION tdsrrc_register_tv_adapter_info_cb

DESCRIPTION
  This function is the API to register the function pointer for TDS Voice 
  adapter call back.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_register_tv_adapter_info_cb
(
  tdscdma_ivoice_cmd_open_t *open_cmd
)
{
  sys_modem_as_id_e_type rrc_as_id = open_cmd->as_id;
  *open_cmd->ret_handle = open_cmd->as_id;
  tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func = open_cmd->event_cb;
  TDSRRC_MSG2(MSG_LEGACY_HIGH, "TV_TRRC: registering %x with tdscdma voice adapter,as_id %d",open_cmd->event_cb,rrc_as_id);
}

/*===========================================================================

FUNCTION tdsrrc_deregister_tv_adapter_info_cb

DESCRIPTION
  This function is the API to de-register the function pointer for TDS Voice
  adapter call back.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_deregister_tv_adapter_info_cb
(
  tdscdma_ivoice_cmd_close_t *close_cmd
)
{
  sys_modem_as_id_e_type rrc_as_id = close_cmd->handle;
  if(tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func != NULL)
  {
    tdsrrcdata_codec_info.rrc_tdscdma_call_open_info_cb_func = NULL;
    TDSRRC_MSG1(MSG_LEGACY_HIGH, "TV_TRRC: de-registering with voice adapter,as_id %d",rrc_as_id);
  }
}

/*===========================================================================

FUNCTION tdsrrc_tv_session_process_start

DESCRIPTION
  This function is the API to check Voice Session started

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrc_tv_session_process_start
(
  tdscdma_ivoice_cmd_start_t *start_cmd
)
{
  sys_modem_as_id_e_type rrc_as_id = start_cmd->handle;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: VOICE SESSION STARTED with as_id %d",rrc_as_id);
}

/*===========================================================================

FUNCTION rrc_vsession_process_stop

DESCRIPTION
  This function is the API to check Voice Session stopped

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_tv_session_process_stop
(
  tdscdma_ivoice_cmd_stop_t *stop_cmd
)
{
  sys_modem_as_id_e_type rrc_as_id = stop_cmd->handle;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"TV_TRRC: VOICE SESSION STOPPED with as_id %d",rrc_as_id);
}

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
/*===========================================================================

FUNCTION tdsrrc_set_identity_info_to_global_params

DESCRIPTION
The function set the UE identify info to global params according to sub_id

tdsrrc_tmsi_valid = tdsrrc_tmsi_valid_internal[sub_id];
tdsrrc_ptmsi_valid =tdsrrc_ptmsi_valid_internal[sub_id];
tdsrrc_imsi_valid =tdsrrc_imsi_valid_internal[sub_id];
tdsrrc_tmsi = tdsrrc_tmsi_internal_internal[sub_id];
tdsrrc_ptmsi = tdsrrc_ptmsi_internal[sub_id];
tdsrrc_imsi = tdsrrc_imsi_internal[sub_id];
tdsrrc_lo_access_class = tdsrrc_lo_access_class_internal[sub_id];
tdsrrc_hi_access_class = tdsrrc_hi_access_class_internal[sub_id];
tdsrrcmisc_rcvd_sim_ins_req = tdsrrcmisc_rcvd_sim_ins_req_internal[sub_id];
tdsrrc_rcvd_admin_data = tdsrrc_rcvd_admin_data_internal[sub_id];
tdsrrc_csp_int_data.ue_mode = tdsrrc_csp_int_data.ue_mode_internal[sub_id];

DEPENDENCIES
  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrc_set_identity_info_to_global_params
(
  sys_modem_as_id_e_type sub_id
)
{
  tdsrrc_tmsi_valid = tdsrrc_tmsi_valid_internal[sub_id];
  tdsrrc_ptmsi_valid =tdsrrc_ptmsi_valid_internal[sub_id];
  tdsrrc_imsi_valid =tdsrrc_imsi_valid_internal[sub_id];
  tdsrrc_tmsi = tdsrrc_tmsi_internal[sub_id];
  tdsrrc_ptmsi = tdsrrc_ptmsi_internal[sub_id];
  tdsrrc_imsi = tdsrrc_imsi_internal[sub_id];
  tdsrrc_lo_access_class = tdsrrc_lo_access_class_internal[sub_id];
  tdsrrc_hi_access_class = tdsrrc_hi_access_class_internal[sub_id];
  tdsrrcmisc_rcvd_sim_ins_req = tdsrrcmisc_rcvd_sim_ins_req_internal[sub_id];
  tdsrrc_rcvd_admin_data = tdsrrc_rcvd_admin_data_internal[sub_id];
#ifdef FEATURE_SGLTE
  tdsrrc_csp_int_data.ue_mode = tdsrrc_csp_int_data.ue_mode_internal[sub_id];
#endif
}

/*===========================================================================

FUNCTION tdsrrc_set_identity_info_to_related_sub_params

DESCRIPTION
The function set the UE identify info to related sub params according to sub_id

tdsrrc_tmsi_valid_internal[sub_id] = tdsrrc_tmsi_valid;
tdsrrc_ptmsi_valid_internal[sub_id] = tdsrrc_ptmsi_valid;
tdsrrc_imsi_valid_internal[sub_id] = tdsrrc_imsi_valid;
tdsrrc_tmsi_internal[sub_id] = tdsrrc_tmsi;
tdsrrc_ptmsi_internal[sub_id] = tdsrrc_ptmsi;
tdsrrc_imsi_internal[sub_id] = tdsrrc_imsi;
tdsrrc_lo_access_class_internal[sub_id] = tdsrrc_lo_access_class;
tdsrrc_hi_access_class_internal[sub_id] = tdsrrc_hi_access_class;
tdsrrcmisc_rcvd_sim_ins_req_internal[sub_id] = tdsrrcmisc_rcvd_sim_ins_req;
tdsrrc_rcvd_admin_data_internal[sub_id] = tdsrrc_rcvd_admin_data;
#ifdef FEATURE_SGLTE
tdsrrc_csp_int_data.ue_mode_internal[sub_id] = tdsrrc_csp_int_data.ue_mode;
#endif

DEPENDENCIES
  

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrc_set_identity_info_to_related_sub_params
(
  sys_modem_as_id_e_type sub_id
)
{
  tdsrrc_tmsi_valid_internal[sub_id] = tdsrrc_tmsi_valid;
  tdsrrc_ptmsi_valid_internal[sub_id] = tdsrrc_ptmsi_valid;
  tdsrrc_imsi_valid_internal[sub_id] = tdsrrc_imsi_valid;
  tdsrrc_tmsi_internal[sub_id] = tdsrrc_tmsi;
  tdsrrc_ptmsi_internal[sub_id] = tdsrrc_ptmsi;
  tdsrrc_imsi_internal[sub_id] = tdsrrc_imsi;
  tdsrrc_lo_access_class_internal[sub_id] = tdsrrc_lo_access_class;
  tdsrrc_hi_access_class_internal[sub_id] = tdsrrc_hi_access_class;
  tdsrrcmisc_rcvd_sim_ins_req_internal[sub_id] = tdsrrcmisc_rcvd_sim_ins_req;
  tdsrrc_rcvd_admin_data_internal[sub_id] = tdsrrc_rcvd_admin_data;
#ifdef FEATURE_SGLTE
  tdsrrc_csp_int_data.ue_mode_internal[sub_id] = tdsrrc_csp_int_data.ue_mode;
#endif
}
#endif
