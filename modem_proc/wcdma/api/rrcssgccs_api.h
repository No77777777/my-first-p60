#ifndef RRCSSGCCSIF_H
#define RRCSSGCCSIF_H
/*===========================================================================

                    R R C    S S G C C S    I F

DESCRIPTION
  This file contains global declarations and external references
  for the RRC SSGCCS Interface.


  
  Copyright (c) 2000-2008 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/wcdma.mpss/8.4.0/api/rrcssgccs_api.h#2 $ $DateTime: 2020/07/14 01:35:01 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/14/20   ak      Made changes for FR 61612
07/14/20   ak      Made changes to include intra-f nbrs count
07/14/20   gcs     Made changes to bar camped cell when SSGCCS detected fake BTS and AS ID included while sending WCDMA cell information to SSGCCS client
06/19/20   gcs     Made changes for FR54317
===========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "mm_as_types.h"
#include "rrcmmif.h"
#include "uecomdef.h"

#define SSGCCS_MAX_SHARED_PLMNS        6
#define SSGCCS_MAX_INTER_FREQ          4
#define SSGCCS_MAX_GSM_NBR_FREQ        32
#define SSGCCS_MAX_LTE_NBR_FREQ        8
/* maxCellMeas  - Maximum Number of Cells Measured      */
#define SSGCCS_L1_MAX_CELL_MEAS        32
/* Max number of inter frequency layers that can be sent in by 3rd_MEAS_REQ */
#define SSGCCS_L1_MAX_INTERF_PRIORITY_LAYERS 8
/* Max number of LTE layers that can be sent in by 3rd_MEAS_REQ */
#define SSGCCS_L1_MAX_EUTRA_PRIORITY_LAYERS 8
/* Max number of GSM layers that can be sent in by 3rd_MEAS_REQ */
#define SSGCCS_L1_MAX_GSM_PRIORITY_LAYERS 32
#define SSGCCS_MAX_EUTRA_FREQUENCIES 10
/* For EUTRA frequency up to 8 frequencies  */
#define SSGCCS_L1_MAX_EUTRA_FREQ 8
/* Max cells supported by EUTRA frequency */
#define SSGCCS_L1_MAX_EUTRA_CELLS_PER_FREQUENCY 16
#define SSGCCS_RRC_MAX_EUTRA_DED_PRI_LAYERS 64

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef struct{
/* A 16-bit integer representation of MCC. Range: 0 to 999. */
uint16 mcc;

/* A 16-bit integer representation of MNC. Range: 0 to 999. */
uint16 mnc;
}ssgccs_shared_plmn_type;

typedef struct{

  uint8 ifreq_nbr_cnt;
  uint16 ifreq_nbr[SSGCCS_MAX_INTER_FREQ]; /*WCDMA nbr UARFCNS*/

  uint8 gsm_nbr_cnt;
  uint16 gsm_arfcns[SSGCCS_MAX_GSM_NBR_FREQ];  /*GSM nbr ARFCNS*/
/*W intra frequency neighbor info*/
  uint8 intra_f_nbr_cnt;
  uint16 psc[SSGCCS_L1_MAX_CELL_MEAS];

}wcdma_ssgccs_sib11_neighbor_info;

/* Struct to store the Serving Cell priority info received from SIB19 */
typedef struct
{
  int8 priority;          /*Range: (0 to 7) */
  uint8 s_prioritysearch1; /*Range: 2*(0 to 31) */
  uint8 s_prioritysearch2; /*Range: (0 to 7), default is 0*/
  uint8 thresh_servinglow; /*Range: (0 to 31)*/
  uint8 thresh_servinglow2; /*Range: (0 to 31) default is 0 */  
} wcdma_ssgccs_serving_cell_priority_info_type;

/* Struct to store the inter-f priority info received from SIB19 */
typedef struct
{
  uint16 UARFCN;
  int8 priority;     /*Range: (0 to 7) */

  uint8 threshx_high; /*Range: 2*(0 to 31)*/
  uint8 threshx_low; /*Range: 2*(0 to 31)*/
  int16 q_qualmin_fdd; /*Range: -24 to 0, Default : -24 (Spec 25.331, 13.4.15c)*/
  int16 q_rxlevmin_fdd; /*Range: 2*(-60 to -13)+1, Default : -119 (Spec 25.331, 13.4.15c)*/
} wcdma_ssgccs_inter_freq_priority_info_type;

/* Struct to store the GSM priority info received from SIB19 */
typedef struct
{
  W_ARFCN_T ARFCN;
  int8 priority;       /*Range: (0 to 7) */
  int16 q_rxlevmin_gsm; /*Range: 2*(-58 to -13)+1 */
  uint8 threshx_high;  /*Range: 2*(0 to 31)*/
  uint8 threshxlow;  /*Range: 2*(0 to 31)*/
} wcdma_ssgccs_inter_rat_gsm_priority_info_type;

/* Struct to store the LTE priority info per EARFCN received from SIB19 */
typedef struct
{
  /* EARFCN of the downlink carrier frequency */
  uint32 EARFCN; /*Range: (0..4,294,967,295)*/
  /* Priority of the LTE freq */
  int8 priority; /* Range: (0..7) */
  /* Threshold to be used when calculating suitability criteria for that LTE EARFCN */
  int16 q_rxlevmin_eutra; /* Range: 2*(-70 to -22) */
  /* Max threshold value RSRP, dB */
  uint8 threshx_high; /* Range: 2*(0 to 31) */
  /* Min threshold value RSRP, dB */
  uint8 threshx_low;  /* Range: 2*(0 to 31) */
  /* Threshold to be used when calculating suitability criteria for that LTE EARFCN */
  /* as per spec RSRQ, dB default value is negative infinity, L1 should treat 0xFF as default */
  int16 q_qualmin_eutra;
  /* Max threshold value RSRQ, dB */
  boolean high2_incl;
  uint8 threshx_high2; 
  /* Min threshold value RSRQ, dB */
  boolean low2_incl;
  uint8 threshx_low2;  
} wcdma_ssgccs_inter_rat_eutra_priority_info_type;

typedef struct{
  wcdma_ssgccs_serving_cell_priority_info_type serving_cell_priority;
  boolean inter_freq_priority_info_incl;
  uint8 num_inter_freq_priorities;
  wcdma_ssgccs_inter_freq_priority_info_type inter_freq_priority[SSGCCS_L1_MAX_INTERF_PRIORITY_LAYERS];

  boolean inter_rat_gsm_priority_info_incl;
  uint8 num_gsm_priorities;
  wcdma_ssgccs_inter_rat_gsm_priority_info_type gsm_priority[SSGCCS_L1_MAX_GSM_PRIORITY_LAYERS];

  boolean inter_rat_eutra_priority_info_incl;

  uint8 num_eutra_priorities;
  wcdma_ssgccs_inter_rat_eutra_priority_info_type eutra_priority[SSGCCS_L1_MAX_EUTRA_PRIORITY_LAYERS];
}wcdma_ssgccs_sib19_neighbor_info;

typedef enum{
  WCDMA_SSGCCS_MIB_BIT = 0x00000001,
  WCDMA_SSGCCS_SB_1_BIT = 0x00000002,
  WCDMA_SSGCCS_SB_2_BIT = 0x00000004,
  WCDMA_SSGCCS_SIB_1_BIT = 0x00000008,
  WCDMA_SSGCCS_SIB_2_BIT = 0x00000010,
  WCDMA_SSGCCS_SIB_3_BIT = 0x00000020,
  WCDMA_SSGCCS_SIB_4_BIT = 0x00000040,
  WCDMA_SSGCCS_SIB_5_BIT = 0x00000080,
  WCDMA_SSGCCS_SIB_6_BIT = 0x00000100,
  WCDMA_SSGCCS_SIB_7_BIT = 0x00000200,
  WCDMA_SSGCCS_SIB_11_BIT = 0x00000400,
  WCDMA_SSGCCS_SIB_12_BIT = 0x00000800,
  WCDMA_SSGCCS_SIB_18_BIT = 0x00001000,
  WCDMA_SSGCCS_SIB_5bis_BIT = 0x00002000,
  WCDMA_SSGCCS_SIB_11BIS_BIT = 0x00004000,
  WCDMA_SSGCCS_SIB_19_BIT = 0x00008000
}wcdma_ssgccs_rrc_sib_bitmask_enum;

typedef struct{

  /* A 16-bit integer representation of MCC. Range: 0 to 999. */
  uint16 mcc;

  /* A 16-bit integer representation of MCC. Range: 0 to 999. */
  uint16 mnc;

  /* Number of PLMNs indicated as shared in the MIB */
  uint8 num_of_shared_plmns;

  /* list of shared PLMNs */
  ssgccs_shared_plmn_type shared_plmn_list[SSGCCS_MAX_SHARED_PLMNS];

  /* Cell identifier of camped cell */
  int32 cellid;

  /* Primary Scrambling Code  ranges from 0-512 */
  uint16 psc;

  /* Frequency  i.e. UTRA Absolute Radio Frequency Number */
  uint16 uarfcn;

  /* S_SearchQual for Intra Frequency cells*/
  int8 s_Intrasearch;

  /* S_SearchQual for Inter frequency cells */
  int8 s_Intersearch;

  /* Minimum required quality level in the cell in dB */
  int16 q_QualMin;

  /* Minimum required Rx level in the cell in dBm */
  int16 q_RxlevMin;

  /* Q Hysteresis value used for measurement purpose */
  uint8 q_Hyst_l_S;

  /* the new cell is better ranked than the serving cell during the preceding time interval Treselection */
  uint8 t_Reselection_S;

  /*Maximum TX power level an UE may use when accessing the cell on RACH */
  int16 maxAllowedUL_TX_Power;

  /* stores current cell access status 0-NormalCalls,1-emergencyCalls ,2-NO calls and 3-All Calls */
  mm_as_cell_access_status_T curr_cell_access_status;

  /* SIB11 W and GSM neighbor info */
  wcdma_ssgccs_sib11_neighbor_info sib11_info;

  /* SIB19 neighbor info and priorities */
  wcdma_ssgccs_sib19_neighbor_info sib19_info;

  wcdma_ssgccs_rrc_sib_bitmask_enum sib_bitmask;
  /* Access stratum subscription ID */
  sys_modem_as_id_e_type wrrc_as_id;

  /*  Shared NW MIB PLMN MCC */
  uint16 shared_nw_mib_plmn_mcc;
  
  /*  Shared NW MIB PLMN MNC */
  uint16 shared_nw_mib_plmn_mnc;
}wcdma_ssgccs_3g_cell_info_ind_s;

typedef struct{
  /* Mobile Country Code */
  uint16 mcc;
  /* Mobile Network Code */
  uint16 mnc;
  /* Cell identifier of camped cell */
  int32 cellid;

  rrc_initial_ue_id_choice_e_type rrc_initial_ue_identity_choice;

  rrc_initial_ue_id_type rrc_initial_ue_identity;
  /* Access stratum subscription ID */
  sys_modem_as_id_e_type wrrc_as_id;

}wcdma_ssgccs_rrc_connection_request_info_ind_s;

typedef struct{
  /* Mobile Country Code */
  uint16 mcc;
  /* Mobile Network Code */
  uint16 mnc;
  /* Cell identifier of camped cell */
  int32 cellid;
  /* RRC connection reject cause */
  uint8 conn_rej_cause;
  /* wait time received through reject message */
  uint8 wait_time;
  /* Redirection info present */
  uint8 redir_info_present;
  /* Redirected RAT */
  uint8 redir_rat;
  /* Access stratum subscription ID */
  sys_modem_as_id_e_type wrrc_as_id;
}wcdma_ssgccs_rrc_connection_reject_info_ind_s;

typedef struct{
  /* Mobile Country Code */
  uint16 mcc;
  /* Mobile Network Code */
  uint16 mnc;
  /* Cell identifier of camped cell */
  int32 cellid;
  boolean ciphering_info_received;  
    /*ciphering algo being used*/
  uecomdef_wcdma_cipher_algo_e_type ciph_algo;
  /* Access stratum subscription ID */
  sys_modem_as_id_e_type wrrc_as_id;

}wcdma_ssgccs_security_mode_command_info_ind_s;

typedef struct{
  /* Mobile Country Code */
  uint16 mcc;
  /* Mobile Network Code */
  uint16 mnc;
  /* Cell identifier of camped cell */
  int32 cellid;
  /*RRC connection release cause */
  uint8 rrc_release_cause;
  /* redirection info present*/
  boolean redirection_info_present;
  /* Redirected RAT */
  uint8 redirected_rat;
  /* Access stratum subscription ID */
  sys_modem_as_id_e_type wrrc_as_id;

}wcdma_ssgccs_rrc_connection_release_info_ind_s;

typedef enum
{
  SSGCCS_WCDMA_PSC_BARRED,
  SSGCCS_WCDMA_FREQ_BARRED,
  SSGCCS_WCDMA_ASET_BARRED,
}wcdma_ssgccs_bar_info_e_type;

typedef struct{
  /*  cell bar type */
  wcdma_ssgccs_bar_info_e_type ssgccs_bar_info;
  /* how long cell should be barred */
  uint32 ssgccs_tbarred;

  /* Primary Scrambling Code  ranges from 0-512 */
  uint16 psc;

  /* Frequency  i.e. UTRA Absolute Radio Frequency Number */
  uint16 uarfcn;

  /* Setting this variable to TRUE means that on a limited service request L1 has to lift freq
  * and PSC barring both which overrides t-bar feature characteristic that says unbar
  * only the frequency in this case */
  boolean bar_for_resel_only;

}wcdma_ssgccs_rrc_cell_bar_info_ind_s;

typedef struct{
  uint16 freq;
  uint16 psc;
  uint8 t_value;
}wcdma_ssgccs_rrc_cell_depr_info_s;

/*========================================================================
FUNCTION register_ssgccs_3g_cell_info_msg_handler

DESCRIPTION
    Register callback handler for cell info

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_cell_info_msg_handler(void * msg_handler);
/*========================================================================
FUNCTION register_ssgccs_3g_rrc_connection_request_msg_handler

DESCRIPTION
    Register callback handler for RRC connection request

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_rrc_connection_request_msg_handler(void * msg_handler);

/*========================================================================
FUNCTION register_ssgccs_3g_rrc_connection_reject_msg_handler

DESCRIPTION
    Register callback handler for RRC connection request

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_rrc_connection_reject_msg_handler(void* msg_handler);
/*========================================================================
FUNCTION register_ssgccs_3g_security_mode_command_msg_handler

DESCRIPTION
    Register callback handler for security mode command

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_security_mode_command_msg_handler(void* msg_handler);
/*========================================================================
FUNCTION register_ssgccs_3g_rrc_connection_release_msg_handler

DESCRIPTION
    Register callback handler for RRC connection release

PARAMETERS
    msg_handler [in] - callback handler

RETURN VALUE
    None
===========================================================================*/
void register_ssgccs_3g_rrc_connection_release_msg_handler(void* msg_handler);

/*===========================================================================

FUNCTION          rrccsp_ssgccs_cell_bar_req

DESCRIPTION       This function is called by SSGCCS When High Threat score detected and decided to take countermeasure

DEPENDENCIES      NONE

RETURN VALUE      NONE

SIDE EFFECTS      NONE

===========================================================================*/
void rrccsp_ssgccs_cell_bar_req(sys_modem_as_id_e_type wrrc_as_id, wcdma_ssgccs_rrc_cell_bar_info_ind_s);
/*===========================================================================

FUNCTION          rrcssgccs_cell_depri_req_info

DESCRIPTION       This function is called by SSGCCS to send depriortization info for a particular cell

DEPENDENCIES      NONE

RETURN VALUE      NONE

SIDE EFFECTS      NONE

===========================================================================*/
void rrcssgccs_cell_depri_req_info(sys_modem_as_id_e_type wrrc_as_id, wcdma_ssgccs_rrc_cell_depr_info_s ssgccs_depr_info);
#endif
