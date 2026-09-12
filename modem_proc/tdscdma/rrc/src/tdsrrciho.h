#ifndef TDSRRCIHO_H
#define TDSRRCIHO_H
/*===========================================================================
         R R C   I N T E R - R A T   H A N D O V E R   M O D U L E 

DESCRIPTION

  This module is the header module for the inter-RAT handover module.
  This contains the external interface functions to be used by
  by other RRC modules. This module is only used for dual-mode.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrciho.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
04/30/13   hx      Made chagnes to send TDSRR_CIPHER_SYNC_IND to G whenever new keys are applied 
03/14/12   as      Added code to support feature NB/WB handover support
03/10/11   hx      Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
08/20/10   rm      Fixed compiler warnings

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */
#include "tdscdma_variation.h"
#include "tdsrrccmd_v.h"

#include "tdsrrcasn1.h"

#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#ifdef FEATURE_TDSCDMA_TO_LTE
#include "lte_rrc_ext_api.h"
#endif

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#define TDSRRCIHO_MAX_GSM_MSG_LIST_STRING_BIT_LENGTH 512

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* Enumerated type of the different substates for this procedure           */
typedef enum
{
  TDSRRCIHO_INITIAL,                   /* Initial Substate                    */
  TDSRRCIHO_WAIT_STANDBY_CNF,          /* Waiting for mode change comfirmation
                                       from MCM (to Standby)               */
  TDSRRCIHO_WAIT_INTERRAT_HO_CNF,      /* Waiting for inter-RAT Handover 
                                       comfirmation from the GSM RR        */
  TDSRRCIHO_WAIT_ACTIVE_CNF,           /* Waiting for mode change comfirmation
                                       from MCM (to Active)                */
  TDSRRCIHO_WAIT_CHANNEL_CONFIG_CNF,   /* Waiting for revert back to UTRAN
                                       configuration comfirmation from LLC */
  TDSRRCIHO_WAIT_CELL_UPDATE_CNF,      /* Waiting for cell update comfirmation
                                       from CU                             */
  TDSRRCIHO_WAIT_ABORT_CNF,            /* Waiting for abort cnf from RR       */

    /*added by HX for LTE to use the shared items with GeRAN*/
//#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  
  TDSRRCIHO_WAIT_ACQ_CNF,              /* Waiting for acq cnf from LLC        */
  TDSRRCIHO_WAIT_IRAT_IN_PROGRESS_CNF, /* Waiting for Inter-rat in progress 
                                       Cnf from MCM   */  
  TDSRRCIHO_WAIT_INACTIVE_CNF,         /* Waiting for Inactive Cnf from MCM   */
  TDSRRCIHO_WAIT_CHAN_CONFIG_G2TD_CNF, /* Waiting for Ch Config Cnf from LLC
                                       when HO from GSM to TDS           */
  TDSRRCIHO_WAIT_CIPH_CONFIG_G2TD_CNF, /* Waiting for Ciph Config Cnf from SMC
                                       when config. Ciph for GtoW HO       */

  TDSRRCIHO_WAIT_L2ACK_FOR_HO_TO_UTRAN_COMPLETE,
  TDSRRCIHO_WAIT_ACT_RSP,
//#endif
  
  TDSRRCIHO_MAX_STATES
} tdsrrciho_state_e_type;
  
  
#ifdef FEATURE_TDSCDMA_TO_LTE
typedef struct {
  /* Message to be sent to LRRC */
  lte_rrc_tdscdma_psho_req_s   msg;

  /* DSM pointer to the EUTRA message */
  dsm_item_type   *dsm_ptr;

  /* Size of EUTRA container */
  uint32  eutra_container_size;
  /* A pointer to the container*/
  byte *eutra_container_ptr;

} tdsrrciho_eutra_msg_type;
#endif

#ifdef FEATURE_TDSCDMA_RRC_RB_PRIORITY_OPT
typedef enum
{
  IHO_G2TD_COMPLETE_CONFIG,
  IHO_G2TD_PREDEF_CONFIG,
  IHO_G2TD_DEFAULT_CONFIG
}rrciho_g2td_config_e_type;

/* Procedure status */
typedef struct
{
  uint16  count_c_act_time;                  /*count_c activation time sent
                                               in HO to UTRAN complete msg*/
  boolean gsm_ciph_status;                   /*flag to indicate if ciph is
                                               present in GSM RAT         */

  boolean ciph_algo_present;                 /*flag to indicate if ciph
                                               algo is present            */
  uecomdef_umts_cipher_algo_e_type cipheringAlgorithm; /*var to store ciph algo sent
                                               in HO to utran cmd         */

  rrciho_g2td_config_e_type config_type;     /*enum to indicate what config
                                               to use for Handover        */
  uint8               predef_config_id;      /*predefined configuration ID*/
  uint32              default_config_id;     /*default configuration ID   */

  struct tdsrrc_SysInfoType16 * sib16_ptr;      /*ptr to sysInfoType16       */
  boolean             def_config_ptr_present;/*flag to indicate that def
                                               config ptr has been expanded
                                               to SIB16 type.             */

  uint32              maxAllowedUL_TX_Power; /*max allowed UL Tx Power sent
                                               in HOToUtran Command       */

  boolean             act_time_present;      /*flag to indicate if act
                                               time is present in the msg */
  uint32              activation_time;       /*activation time sent in
                                               HOToUtran Command          */

  tdsrrc_HandoverToUTRANCommand *ho_ptr;        /* Pointer to the HO message */
  uint32              ho_msg_len;            /* length of the HO message  */
  boolean             ho_msg_present;        /* boolean to indicate that
                                                we have currently stored
                                                a hoToUtranMsg            */

  tdsrrc_cpid_type  cpid;               /*scrambling code info from
                                               HO to UTRAN cmd            */
  tdsrrc_freq_type      freq;                   /*UARFCN info from the HO to
                                               UTRAN cmd                  */
  int16              rscp;          /* HO to UTRAN target cell pre-measured RSCP by other RAT
                                               used by L1 to skip step 0 and do reAcq */

  boolean            rab_info_included;      /*boolean to indicate if rab
                                               info was included in msg   */

  uint8              num_srb;
  uint8              srb_id[UE_MAX_SRB_SETUP];

#ifndef FEATURE_LTE_TO_TDSCDMA
  tdsrrc_est_rab_info_type  est_rabs;
#else
  uint8  number_of_est_rabs;
  tdsrrc_est_rab_info_type  est_rabs[MAX_RAB_TO_SETUP];
  uint8 cs_rab_index;
  boolean is_cs_rab_present;
  boolean is_ps_rab_present;
#endif /* FEATURE_LTE_TO_TDSCDMA */

  boolean            class_mark_info_present;
  byte *             class_mark_ptr;

#if 0  //FDD only, not needed
  boolean            pn_offset_present;      /*indicates pn_offset present */
  uint16             pn_offset;              /*actual pn_offset            */

  boolean            diversity_info_present; /*indicates if div present    */
  boolean            diversity_info;         /*indicates if known or not   */
#endif

  /* Indicates how a PLMN is to be selected. */
  sys_network_selection_mode_e_type network_select_mode;

  tdsrrc_msg_ver_enum_type  msg_rel_version;
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
  struct tdsrrc_DL_DCCH_Message *rbs_ptr;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */


//#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  /* InterRAT Handover request type, either TDSINTERRAT_HO_TYPE_CS or 
   * TDSINTERRAT_HO_TYPE_PS
   *
   * Replaced by is_cs_rab_present and is_ps_rab_present
   */
  /*tdsrrciho_interrat_handover_req_e_type irat_ho_type;*/
  lte_irat_umts_sec_info_s psho_security_keys;
  /* timing info for re-acq on the target freq */
  uint8                num_of_jds_cells; /* number of JDS cells for interference cancellation */
  uint8                lna_state; /* current LNA state */
  uint16               ts0_position; /* ts0 start position from step0 result */
  uint8                cell_list[TDSIRAT_MAX_TDS_CELLS_PER_FREQ]; /* JDS cell parameter id */

//#endif /* FEATURE_LTE_TO_TDSCDMA */

  /*Flag to L1 in the cphy_setup_req*/
  boolean is_x2td_irat_ho;

} tdsrrciho_g2td_info_type;
#endif  


/* Procedure status.
*/
typedef struct
{
  tdsrrc_RRC_TransactionIdentifier transaction_id;

  boolean act_time_present;

  uint32 activation_time;

  uint8                         handover_type;

  union
  {
    tdsrr_interrat_handover_req_type  gsm;

#ifdef FEATURE_TDSCDMA_TO_LTE
    tdsrrciho_eutra_msg_type    eutra;
#endif
  } parms;

  uint32 failure_cause;

  tdsrrc_ProtocolErrorCause protocol_error;

} tdsrrciho_procedure_info_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
extern tdsrrciho_procedure_info_type tdsprocedure_info;

extern tdsrrc_default_config_info_type tdsdefault_config_info[];

extern uint8 tdsdefault_config_0[];
extern uint8 tdsdefault_config_0_len;

extern uint8 tdsdefault_config_1[];
extern uint8 tdsdefault_config_1_len;

extern uint8 tdsdefault_config_3[];
extern uint8 tdsdefault_config_3_len;

extern uint8 tdsdefault_config_4[];
extern uint8 tdsdefault_config_4_len;

extern uint8 tdsdefault_config_6[];
extern uint8 tdsdefault_config_6_len;

extern uint8 tdsdefault_config_7[];
extern uint8 tdsdefault_config_7_len;

extern uint8 tdsdefault_config_8[];
extern uint8 tdsdefault_config_8_len;

extern uint8 tdsdefault_config_9[];
extern uint8 tdsdefault_config_9_len;

extern uint8 tdsdefault_config_10[];
extern uint8 tdsdefault_config_10_len;

extern uint8 tdsdefault_config_15[];
extern uint8 tdsdefault_config_15_len;

extern uint8 tdsdefault_config_16[];
extern uint8 tdsdefault_config_16_len;

extern uint8 tdsdefault_config_17[];
extern uint8 tdsdefault_config_17_len;

extern uint8 tdsdefault_config_18[];
extern uint8 tdsdefault_config_18_len;

extern uint8 tdsdefault_config_19[];
extern uint8 tdsdefault_config_19_len;

extern uint8 tdsdefault_config_20[];
extern uint8 tdsdefault_config_20_len;

extern uint8 tdsdefault_config_21[];
extern uint8 tdsdefault_config_21_len;

extern uint8 tdsdefault_config_22[];
extern uint8 tdsdefault_config_22_len;

extern uint8 tdsdefault_config_23[];
extern uint8 tdsdefault_config_23_len;

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
extern uint8 tdsdefault_config_11[];
extern uint32  tdsdefault_config_11_len;

extern uint8 tdsdefault_config_13[];
extern uint32  tdsdefault_config_13_len;


extern uint8 tdsdefault_config_12[];
extern uint32  tdsdefault_config_12_len;

extern uint8 tdsdefault_config_14[];
extern uint32 tdsdefault_config_14_len;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */
/* =======================================================================
**                        Function Declarations
** ======================================================================= */

/*===========================================================================

FUNCTION RRCIHO_INIT_PROCEDURE


DESCRIPTION

  This function initializes the Inter-RAT handover module.  
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrciho_init_procedure(void);

/*===========================================================================

FUNCTION RRCIHO_EVENT_HANDLER


DESCRIPTION

  This is the main function handling external events received by the
  Inter-RAT Handover module. This function is called by the
  dispatcher whenever an event is received for this module.  
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrciho_event_handler(
  tdsrrc_cmd_type *cmd_ptr                      /* Pointer to command */
);

/*===========================================================================

FUNCTION   tdsrrciho_get_sib16_ptr

DESCRIPTION

  This function gets the Sib16 ptr either from the actual sib16 or the
  SIB16 version of Default Configuration

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
struct tdsrrc_SysInfoType16 * tdsrrciho_get_sib16_ptr(void);

#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
/*===========================================================================

FUNCTION   tdsrrciho_get_rbs_ptr

DESCRIPTION

  This function gets the RBS ptr either from the actual RBS or the
  RBS version of Default Configuration

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
void * tdsrrciho_get_rbs_ptr(uint32 *df_config_id);
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

/*===========================================================================

FUNCTION   tdsrrciho_send_sync_ind_for_g2td_ho

DESCRIPTION
  Sends SYNC_IND to MM after a successful Activation Response has been
  received for transitioning to TDS
      
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/  
void tdsrrciho_send_sync_ind_for_g2td_ho(void);

/*===========================================================================

FUNCTION   tdsrrciho_get_activation_time_for_td2g_ho

DESCRIPTION

  This function returns the activation time for WtoG handover and also returns
  status Success/Failure if Activation Time was added or not.
          
DEPENDENCIES

  None.
 
RETURN VALUE

  Success/Failure depending on whether activation time was appended or not

SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrciho_get_activation_time_for_td2g_ho (uint16 * act_time);

/*===========================================================================

FUNCTION tdsrrciho_is_td2g_ho_active


DESCRIPTION

  API to indicate if WTOG HO is active and ongoing.  Used by other procedures
  to query if WTOG HO is ongoing
  
DEPENDENCIES

  None

RETURN VALUE

TRUE: If WTOG HO is active
FALSE: IF WTOG is not active

SIDE EFFECTS

  None
  
===========================================================================*/
boolean tdsrrciho_is_td2g_ho_active
(
 void
);

/*===========================================================================
FUNCTION   tdsrrciho_is_cs_data_rab_present

DESCRIPTION
  This function indicates whether any voice RAB is present or not in g2td est_rabs database.

     
DEPENDENCIES
  None.
 
RETURN VALUE

  TRUE: if CS DATA RAB exist in g2td est_rabs database
  FALSE: if CS DATA RAB doesnt exist g2td in est_rabs database

SIDE EFFECTS
  None.
===========================================================================*/ 
boolean tdsrrciho_is_cs_data_rab_present 
(
  void
);



/*the 
#ifdef FEATURE_TDSCDMA_REL8
rrciho_initialize_srb_list_r8
rrciho_update_rab_info_r8
#endif
could be static
*/


#if defined(FEATURE_3GPP_FAST_DORMANCY)
/*===========================================================================

FUNCTION   tdsrrciho_is_iho_td2g_active

DESCRIPTION

 Returns whether IRAT W2G is active or not

DEPENDENCIES

  None.

RETURN VALUE

  boolean

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrciho_is_iho_td2g_active
(
  void
);

#endif 


/*===========================================================================

FUNCTION   rrciho_free_sib16

DESCRIPTION

 Frees the ptr to sysInfoType16 previously allocated.

DEPENDENCIES

  None.

RETURN VALUE

   FAILURE if an error occurred while freeing else
   SUCCESS.


SIDE EFFECTS

  None.

===========================================================================*/

uecomdef_status_e_type tdsrrciho_free_sib16
(
  void
);


/*===========================================================================

FUNCTION tdsrrciho_is_g2td_ho_active

DESCRIPTION 
    This function returns status of GTOW HO.

DEPENDENCIES
    None.

RETURN VALUE
    None.

===========================================================================*/
boolean tdsrrciho_is_g2td_ho_active(void);
/*FEATURE_WCDMA_HANDLE_MDSP_HALT not merged in*/




/*===========================================================================

FUNCTION   tdsrrciho_send_cipher_sync_ind

DESCRIPTION

  This function sends indication to RR that the new CK/IK keys are activated

DEPENDENCIES

  None.

RETURN VALUE

  NONE

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrciho_send_cipher_sync_ind(void);

/*===========================================================================

FUNCTION   tdsrrciho_is_l2t_ho

DESCRIPTION

  This function checks whether the source RAT is LTE for X2T HO.

DEPENDENCIES

  This function can only be used when the X2T HO is in progress, otherwise 
  the status is not reliable.

RETURN VALUE

  TRUE or FALSE.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrciho_is_l2t_ho (void);


#endif /* TDSRRCIHO_H */


