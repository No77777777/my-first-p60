/*===========================================================================
                  R R C  M I S C  P R O C E D U R E 

DESCRIPTION
  This header file contains function prototypes for the RRC Misc Procedure.
  The RRC Misc Procedure is created for implementation purpose to handle
  processing of unknown or unsupported message type. It also process the 
  TDSRRC_SIM_UPDATE_REQ and TDSRRC_SIM_INSERTED_REQ commands from MM and updates
  suitable RRC Data.


EXTERNALIZED FUNCTIONS

  tdsrrcmisc_init_procedure
    This function initializes everything that the RRC Misc procedure
    needs to have initialized.
  
  tdsrrcmisc_procedure_event_handler

    This procedure is the event handler for the RRC Misc procedure.


INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcmisc.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/10/10   bj       Deleted IE "RNC support for change of UE capability".
                          (3GPP: 8.6.2.3)
11/10/10   bj       Delete IE "Acceptance of requested change of capability".
                          (3GPP: 8.1.6.4)

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"

#include "tdsrrclcm.h"               /* Logical Channel Manager */
#include "tdsrrcsend.h"              /* For sending a signalling message */
#include "msg.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsmc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrcmisc.h"
#include "tdsrrctmr.h"
#include "tdsrrccu.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd.h"
#include "tdsrrcrcr.h"
#include "tdsrrcrbcommon.h"
#include "tdsrrcsibproci.h"
#include "tdsrrcsibdbi.h"
#include "tdsrrcsibproc.h"
#include "tdsrrccui.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcrcr.h"

#include "tdsrrcmeas.h"
#include "tdsrrciho.h"
#include "tdsrrcnv.h"

#include "tdsl1rrcif.h"

#ifdef TEST_FRAMEWORK
#error code not present
#else
#include "rfm.h"
#include "wwan_coex_mgr.h"
#endif
#include "tdsrrcccm.h"
#include "tdsrrcwrm.h"
#include "tdsCFAParser_i.h"
#include "tdsrrcmcm.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSRRCMISC_INVALID_TRANSACTION_ID 0xFF
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
boolean tdsrrc_high_mobility_ind_in_rcr = FALSE;
boolean tdsrrcmisc_is_da_backoff = FALSE;

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
boolean tdsrrcmisc_rcvd_sim_ins_req_internal[TDSCDMA_NUM_SUBS];
boolean tdsrrcmisc_rcvd_sim_ins_req;
#else
static boolean tdsrrcmisc_rcvd_sim_ins_req = FALSE;
#endif

/* Stores the current state of MISC for Multiple DRX */
tdsrrcmisc_substate_e_type  tdsrrcmisc_multiple_drx_substate;
extern boolean tdsrrcmeas_cell_id_in_dch_valid ;
extern boolean tdsrrcmeas_hho_happened_in_dch ;
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
#ifdef FEATURE_TDSCDMA_REL7_SPECRS 

/***************RRC CO UEC ************************************/
#define TDSN_COUEC_TIMER 2

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* This variable holds internal data for Change of UE Capability Procedure*/

tdsrrccouec_int_data_type tdsrrccouec;

/*This global indicates whether Serving RNC supports IE "supportForChangeOfUE-Capability" or not*/
boolean tdsrrc_rnc_cap_change_support = FALSE;

/*This structure stores the UE capabilities throughout the RRC Conn Setup.*/
tdsrrc_ue_cap_update_during_rrc_conn_type tdsrrc_ue_cap_during_rrc_conn;

#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/

boolean tdsrrc_clear_sibdb_for_lpm = FALSE;
extern sys_proc_type_e_type tdsrrc_proc_type;

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */
#ifdef FEATURE_DUAL_SIM
extern byte reg_sim_per_subs_read_ens_flag(sys_modem_as_id_e_type as_id);
#else
extern byte reg_sim_read_ens_flag( void);
#endif

/*===========================================================================

FUNCTION tdsrrcmisc_process_proc_ind  

DESCRIPTION
  This function processes the procedure indication received from NAS
  
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

static void tdsrrcmisc_process_proc_ind
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrc_proc_type = cmd_ptr->cmd.rrc_procedure_ind.proc_type;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: NAS proc type %d",tdsrrc_proc_type);

#ifdef FEATURE_WRLF_SYSTEM_SEL
  if (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS())
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: RRC_PROCEDURE_IND for proc type %d received in conn mode OOS",tdsrrc_proc_type);
  }
  else
#endif
  if ((TDRM_CLI_STATE_IS_IN_LOCK(TDRM_CLIENT_RRC)) &&
       (tdsrrcrce_is_rce_procedure_active()) && ((TDSRRC_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_MISC)))
     )
  {
    tdsrrc_check_send_change_priority();
  }
  else
  {
    /* Currently handled only when RRC is holding the lock and in connected state */
  }
}

/*===========================================================================

FUNCTION RRC_TRANSITION_TO_DISCONNECTED_STATE

DESCRIPTION

  This function is used to send initiate cell selection command to CSP
  with destination state as disconnected. CSP on receiving this command
  releases all channels and takes UE to idle 
  
DEPENDENCIES

  None

RETURN VALUE

  SUCCESS:   command successfully sent
  ERR-FATAL: command could not be sent

SIDE EFFECTS

  None

===========================================================================*/
uecomdef_status_e_type tdsrrc_transition_to_disconnected_state
(    
  tdsrrc_proc_e_type rrc_proc,
  tdsrrc_tx_to_discon_reason_e_type tx_to_discon_reason
)
{
  tdsrrc_cmd_type *int_cmd_ptr;  /* Pointer RRC Command */
  
  /* Allocates the buffer to RRC Internal command */
  if((int_cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  { 
    int_cmd_ptr->cmd_hdr.cmd_id                              = TDSRRC_INITIATE_CELL_SELECTION_IND;
    int_cmd_ptr->cmd.initiate_cell_sel_ind.cause             = TDSRRC_TRANSITION_TO_DISCONNECTED;
    int_cmd_ptr->cmd.initiate_cell_sel_ind.proc              = rrc_proc;
    /*set redirect frequency in TDSRRC_INITIATE_CELL_SELECTION_IND*/
    if ((rrc_proc ==TDSRRC_PROCEDURE_RCR) && 
        (tdsrrcrcr_redirection_info_present == TRUE) &&
        (tdsrrcrcr_saved_redirect_info.choice == TDSRRC_FREQ_REDIRECTION))
    {
       int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present = TRUE;
       int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq  = tdsrrcrcr_saved_redirect_info.u.freq.uarfcn_DL;
    }
    else
    {
       int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present = FALSE;
    }
    int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present  = FALSE;
    int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state        = TDSRRC_STATE_DISCONNECTED;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"tx to discon reason %d",tx_to_discon_reason);
    int_cmd_ptr->cmd.initiate_cell_sel_ind.tx_to_discon_reason = tx_to_discon_reason;

    tdsrrc_put_int_cmd(int_cmd_ptr); 
  }
  else
  {
    ERR_FATAL("Malloc failed for initiate cell sel cmd",0,0,0);
  }
  /*Stop CU timers to avoid cross over scenarios while going to disconnected */
  tdsrrccu_stop_cu_timers();
  
  return SUCCESS;
} /* rrcdt_transition_to_disconnected_state */

/*===========================================================================

FUNCTION tdsrrcmisc_process_downlink_msg  

DESCRIPTION
  This function checks whether TDSRRC_DOWNLINK_SDU_IND is for DCCH logical 
  channel. If the message is received on DCCH logical channel, and message type
  unknown or unsupported, then RRC sends RRC STATUS message with error cause 
  "Message Type Invalid or Unsupported".

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

static void tdsrrcmisc_process_downlink_msg
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{

  rlc_lc_id_type lc_id;                      /* Local variable to store 
                                                RLC logical channel Id */
   
  /* Assert the receieved message is of type tdsrrc_DL_CCCH_Message */
  if(TDSRRC_DOWNLINK_SDU_IND == cmd_ptr->cmd_hdr.cmd_id)
  {
    switch(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
    {
      case tdsrrc_DL_DCCH_Message_PDU:

        lc_id = tdsrrclcm_check_ul_rlc_lc_id(    
                UE_LOGCHAN_DCCH,          /* Uplink Logical Channel Type. */
                DCCH_AM_RADIO_BEARER_ID,  /* Radio Bearer Id assoicated with
                                            the above Downlink Logical 
                                            Channel type. */
                UE_MODE_ACKNOWLEDGED_DATA);
                                          /* Mode of transmission for the RLC 
                                             service entity. */
        if(TDSRRCLCM_RLC_LC_ID_NOT_FOUND == lc_id)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Can't find RLC ID for UL RB 2");
        } 

        /* Set a dummy received Message type to remove compile warning.
         * There are only 0 through 30 valid received message types 
         */
        else /* Valid RLC ID found */
        {
          if(SUCCESS != tdsrrcsend_send_rrc_status_message(
            T_tdsrrc_ProtocolErrorMoreInformation_type1_messageTypeNonexistent,
                                           /* The protocol error cause to be placed in
                                           /  the message */
             TDSRRCMISC_INVALID_TRANSACTION_ID,
                                           /* Received trasactiopn Id */
             tdsrrc_ReceivedMessageType_spare1,
                                           /* Received message identification. These
                                              parameters will not be used to build
                                              RRC STATUS message */
             TDSRRC_PROCEDURE_NONE,           /* RRC procedure */
             lc_id,                        /* Logical Channel Id */
             TDSRRCSEND_L2ACK_NOT_REQUIRED  ) ) 
                                           /* Indicates whether L2 Ack 
                                              required or not */
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failed to transmit RRC STATUS message");
          } 
        } /* Valid RLC ID found */
        break;

      case tdsrrc_DL_CCCH_Message_PDU:
      case tdsrrc_PCCH_Message_PDU:
      case tdsrrc_BCCH_FACH_Message_PDU:
      case tdsrrc_BCCH_BCH_Message_PDU:
        /* Do nothing. Ignore the message */
        break;

      case tdsrrc_DL_SHCCH_Message_PDU:
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupoorted SDU type");
        break;

      default:
        /* This should never happen */
        ERR_FATAL("Invalid SDU type is forwarded to MISC Proc: %d",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type,0,0);
        break;
    }     
 
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Event: %d is forwarded to MISC Procedure",cmd_ptr->cmd_hdr.cmd_id);
  }
  
} /* tdsrrcmisc_process_downlink_msg */
/*===========================================================================

FUNCTION tdsrrcmisc_process_sim_inserted_req  

DESCRIPTION
  This function process the TDSRRC_SIM_INSERTED_REQ that received from MM and 
  updates IMSI, TMSI, PTMSI and Access Class varibales in the RRC DATA module
  and sets enable/ disable these indenties as per the inter-task command.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

static void tdsrrcmisc_process_sim_inserted_req
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{

  uint32 counter;                      /* Local variable to use as counter */
  boolean gcf_nv_test_flag = FALSE;
  byte sim_read_ens_flag;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id,curr_sub_id;
#endif

  /* Assert the received command is TDSRRC_SIM_INSERTED_REQ */
  if(TDSRRC_SIM_INSERTED_REQ == cmd_ptr->cmd_hdr.cmd_id)
  {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    sub_id = cmd_ptr->cmd.rrc_sim_inserted_req.as_id;
    if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", sub_id);
      return;
    }
    curr_sub_id = tdsrrc_get_as_id();
    tdsrrc_set_identity_info_to_related_sub_params(curr_sub_id);
    tdsrrc_set_identity_info_to_global_params(sub_id);
#else
#ifdef FEATURE_DUAL_SIM
    tdsrrc_is_sim_id_valid = TRUE;
    tdsrrc_sim_id_recvd = cmd_ptr->cmd.rrc_sim_inserted_req.as_id;
#endif
#endif

    if(cmd_ptr->cmd.rrc_sim_inserted_req.imsi_present == TRUE)
    {
      tdsrrc_imsi.num_digits = cmd_ptr->cmd.rrc_sim_inserted_req.imsi.num_digits;
      if(tdsrrc_imsi.num_digits > RRC_MAX_GSMMAP_IMSI_DIGITS)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid IMSI is rcvd");
      }
      /* Update IMSI digits */
      for(counter = 0; counter < cmd_ptr->cmd.rrc_sim_inserted_req.imsi.num_digits;
          counter++)
      {
        tdsrrc_imsi.value[counter] = cmd_ptr->cmd.rrc_sim_inserted_req.imsi.value[counter];
      }
      /* Set the IMSI to valid */
      tdsrrc_imsi_valid = TRUE;
    }
    if(cmd_ptr->cmd.rrc_sim_inserted_req.ptmsi_present == TRUE)
    {
      /* Copy the PTMSI value from the command to RRC data */
     for(counter = 0; counter < RRC_MAX_GSMMAP_PTMSI_LENGTH; counter++)
     {
       tdsrrc_ptmsi.value[counter] = cmd_ptr->cmd.rrc_sim_inserted_req.ptmsi.value[counter];
     }
     /* Set the PTMSI to valid */
     tdsrrc_ptmsi_valid = TRUE;
    }

    if(cmd_ptr->cmd.rrc_sim_inserted_req.tmsi_present == TRUE)
    {
      /* Copy the TMSI value from the command to RRC Data*/
     for(counter = 0; counter < RRC_MAX_GSMMAP_TMSI_LENGTH; counter++)
     {
       tdsrrc_tmsi.value[counter] = cmd_ptr->cmd.rrc_sim_inserted_req.tmsi.value[counter];
     }
     /* Set the TMSI to valid */
     tdsrrc_tmsi_valid = TRUE;

    }
    /* Copy Low Access Class */
    tdsrrc_lo_access_class = cmd_ptr->cmd.rrc_sim_inserted_req.lo_access_class;

    /* Copy High Access Class */
    tdsrrc_hi_access_class = cmd_ptr->cmd.rrc_sim_inserted_req.hi_access_class;

   /* Update Ciphering Information in the Security Mode Procedure*/
    (void) tdsrrcsmc_init_integrity_ciphering_config (&cmd_ptr->cmd.rrc_sim_inserted_req);

    tdsrrcmisc_rcvd_sim_ins_req = TRUE;

#ifdef FEATURE_UIM_DISABLE_GSDI_INTERFACE
    tdsrrc_rcvd_admin_data = cmd_ptr->cmd.rrc_sim_inserted_req.admin_data;
#endif

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    if (curr_sub_id == sub_id)
#endif
    {
#ifdef FEATURE_DUAL_SIM
    sim_read_ens_flag = reg_sim_per_subs_read_ens_flag(tdsrrc_get_as_id());
#else
    sim_read_ens_flag = reg_sim_read_ens_flag();
#endif
    gcf_nv_test_flag = mcfg_gcf_nv_get_status();

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"sim_read_ens_flag = %d, gcf_nv_test_flag = %d, tdsrrc_clear_sibdb_for_lpm = %d",
                sim_read_ens_flag, gcf_nv_test_flag, tdsrrc_clear_sibdb_for_lpm);
    if(sim_read_ens_flag || gcf_nv_test_flag)
    {
      tdsrrc_sibdb_clear_cell_header();
    }

    if(tdsrrc_clear_sibdb_for_lpm)
    {
      /* Initialize SIB DB after LPM */
      tdsrrcsib_database_init();
      }
    }
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    tdsrrc_set_identity_info_to_related_sub_params(sub_id);
    tdsrrc_set_identity_info_to_global_params(curr_sub_id);
#endif
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Event: %d is forwarded to MISC Procedure",cmd_ptr->cmd_hdr.cmd_id);
  }
  
} /* tdsrrcmisc_process_sim_inserted_req */
/*===========================================================================

FUNCTION tdsrrcmisc_process_sim_update_req  

DESCRIPTION
  This function process the TDSRRC_SIM_UPDATE_REQ that received from MM and 
  updates  TMSI, PTMSI and Access Class varibales in the RRC DATA module
  and sets enable/ disable these indenties as per the inter-task command.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

static void tdsrrcmisc_process_sim_update_req
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{

  uint32 counter;                      /* Local variable to use as counter */
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id,curr_sub_id;
#endif
  /* Assert the received command is TDSRRC_SIM_UPDATE_REQ */
  if(TDSRRC_SIM_UPDATE_REQ == cmd_ptr->cmd_hdr.cmd_id)
  {
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    sub_id = cmd_ptr->cmd.rrc_sim_update_req.as_id;
    if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
    {
      TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", sub_id);
      return;
    }
    
    curr_sub_id = tdsrrc_get_as_id(); 
    tdsrrc_set_identity_info_to_related_sub_params(curr_sub_id);
    tdsrrc_set_identity_info_to_global_params(sub_id);

    TDSRRC_MSG3(MSG_LEGACY_HIGH,"TDSRRC_SIM_UPDATE_REQ sub_id:%d,ptmsi present:%d,tmsi present:%d",sub_id,cmd_ptr->cmd.rrc_sim_update_req.ptmsi_present,cmd_ptr->cmd.rrc_sim_update_req.tmsi_present);
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Current sub_id:%d status ptmsi valid:%d,tmsi valid:%d",sub_id,tdsrrc_ptmsi_valid,tdsrrc_tmsi_valid);
#else
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"TDSRRC_SIM_UPDATE_REQ ptmsi present:%d,tmsi present:%d",cmd_ptr->cmd.rrc_sim_update_req.ptmsi_present,cmd_ptr->cmd.rrc_sim_update_req.tmsi_present);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Current status ptmsi valid:%d,tmsi valid:%d",tdsrrc_ptmsi_valid,tdsrrc_tmsi_valid);
#endif

    if(cmd_ptr->cmd.rrc_sim_update_req.ptmsi_present == TRUE)
    {
      /* Copy the PTMSI value from the command to RRC data */
     for(counter = 0; counter < RRC_MAX_GSMMAP_PTMSI_LENGTH; counter++)
     {
       tdsrrc_ptmsi.value[counter] = cmd_ptr->cmd.rrc_sim_update_req.ptmsi.value[counter];
     }
     TDSRRC_MSG4(MSG_LEGACY_HIGH,"ptmsi in TDSRRC_SIM_UPDATE_REQ: 0x%x, 0x%x, 0x%x, 0x%x",
				 cmd_ptr->cmd.rrc_sim_update_req.ptmsi.value[0],
				 cmd_ptr->cmd.rrc_sim_update_req.ptmsi.value[1],
				 cmd_ptr->cmd.rrc_sim_update_req.ptmsi.value[2],
				 cmd_ptr->cmd.rrc_sim_update_req.ptmsi.value[3]);
     /* Set the PTMSI to valid */
     tdsrrc_ptmsi_valid = TRUE;
    }
    else
    {
      tdsrrc_ptmsi_valid = FALSE;
    }

    if(cmd_ptr->cmd.rrc_sim_update_req.tmsi_present == TRUE)
    {
      /* Copy the TMSI value from the command to RRC Data*/
      for(counter = 0; counter < RRC_MAX_GSMMAP_TMSI_LENGTH; counter++)
      {
        tdsrrc_tmsi.value[counter] = cmd_ptr->cmd.rrc_sim_update_req.tmsi.value[counter];
      }
	    TDSRRC_MSG4(MSG_LEGACY_HIGH,"tmsi in TDSRRC_SIM_UPDATE_REQ: 0x%x, 0x%x, 0x%x, 0x%x",
				 cmd_ptr->cmd.rrc_sim_update_req.tmsi.value[0],
				 cmd_ptr->cmd.rrc_sim_update_req.tmsi.value[1],
				 cmd_ptr->cmd.rrc_sim_update_req.tmsi.value[2],
				 cmd_ptr->cmd.rrc_sim_update_req.tmsi.value[3]);
     /* Set the TMSI to valid */
     tdsrrc_tmsi_valid = TRUE;

    }
    else
    {
      tdsrrc_tmsi_valid = FALSE;
    }
    /* Copy Low Access Class */
    tdsrrc_lo_access_class = cmd_ptr->cmd.rrc_sim_update_req.lo_access_class;

    /* Copy High Access Class */
    tdsrrc_hi_access_class = cmd_ptr->cmd.rrc_sim_update_req.hi_access_class;

    /* Update Ciphering Information in the Security Mode Procedure*/
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    tdsrrc_set_identity_info_to_related_sub_params(sub_id);
    tdsrrc_set_identity_info_to_global_params(curr_sub_id);  
#endif
    /* Update Ciphering Information in the Security Mode Procedure*/
    tdsrrcsmc_update_integrity_ciphering_config (&cmd_ptr->cmd.rrc_sim_update_req);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid Event: %d is forwarded to MISC Procedure",cmd_ptr->cmd_hdr.cmd_id);
  }
} /* tdsrrcmisc_process_sim_update_req */

/*===========================================================================

FUNCTION tdsrrcmisc_rrc_rcvd_sim_insert_req  

DESCRIPTION
  This function is currently called by CSP before a cell bar request is 
  sent to L1. If the SIM has not been read, then cell bar request is not
  sent to L1. The reason being that it is possible that the cell UE attempts
  to camp on is reserved for Operator use. If UE has high access class(es) 
  present then the cell should not be barred else the cell should be barred
  for 1280s (~21mins). Till SIM is read it is not known whether High ACs are 
  present or not and hence such a cell is not barred and in addition UE
  does not camp on this cell (during cell selection processs only and not
  due to cell access restrictions change via SIB modification). 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: SIM has been read
  FALSE: SIM has not been read

===========================================================================*/
boolean tdsrrcmisc_rrc_rcvd_sim_insert_req()
{
  return (tdsrrcmisc_rcvd_sim_ins_req);

} /* tdsrrcmisc_rrc_rcvd_sim_insert_req */



/*===========================================================================

FUNCTION tdsrrcmisc_initiate_cell_update

DESCRIPTION
  This procedure initiates the Cell Update procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcmisc_initiate_cell_update(void)
{
  tdsrrc_cmd_type *int_cmd_ptr = NULL;
  if((int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
    int_cmd_ptr->cmd.initiate_cell_update.cause =tdsrrccu_get_cu_cause_for_wait_timer();
    int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = FALSE;
    int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_MISC;
  
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CELL_UPDATE_INITIATE_REQ sent by RRCMISC due to T320 expiry");
    tdsrrc_put_int_cmd(int_cmd_ptr);
  }
  tdsrrccu_reset_cu_cause_for_wait_timer();
}
/*===========================================================================

FUNCTION tdsrrcmisc_process_T320_expiry

DESCRIPTION
  This procedure handles the Expiry of timer T320.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcmisc_process_T320_expiry(tdsrrc_cmd_type *cmd_ptr)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  if(tdsrrccu_get_cu_cause_for_wait_timer() == TDSRRC_UL_DATA_TRANSMISSION
    )    
  {
    switch(rrc_state)
    {
       /*even in Ura pch cell update has to intited check:PSR*/
      case TDSRRC_STATE_URA_PCH:
      case TDSRRC_STATE_CELL_PCH:
        tdsrrcmisc_initiate_cell_update();
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Error ** T320 expiry in non PCH state. Current RRC State = %d",rrc_state);
        break;
    }
  }
}
/*===========================================================================

FUNCTION tdsrrcmisc_process_multiple_drx

DESCRIPTION
  This function handles the following commands for Multipe DRX feature.

  TDSRRC_T319_EXPIRED_IND
  TDSRRC_BPLMN_SUSPEND_CNF

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
static void tdsrrcmisc_process_multiple_drx(tdsrrc_cmd_type *cmd_ptr)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

  switch(tdsrrcmisc_multiple_drx_substate)
  {
    case  TDSRRCMISC_MULTIPLE_DRX_INITIAL:
      if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_T319_EXPIRED_IND)
      {
         if(((rrc_state == TDSRRC_STATE_CELL_PCH) || 
            (rrc_state == TDSRRC_STATE_URA_PCH)) &&
            (TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
            )
         {
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"UTRAN DRX:BPLMN active. Suspend BPLMN.");
           tdsrrccsp_send_bplmn_suspend_req(TDSRRC_PROCEDURE_MISC,TDSBPLMN_SUSPEND_REASON_OTHER);
           tdsrrcmisc_multiple_drx_substate =  TDSRRCMISC_WAIT_FOR_BPLMN_SUSPEND_CNF;
         }
         else
         {
           tdsrrcllc_update_drx_coef_length();
         }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid event %d in Multiple DRX initial substate",cmd_ptr->cmd_hdr.cmd_id);
      }
      break;

    case TDSRRCMISC_WAIT_FOR_BPLMN_SUSPEND_CNF:
      tdsrrcmisc_multiple_drx_substate =  TDSRRCMISC_MULTIPLE_DRX_INITIAL;
      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_BPLMN_SUSPEND_CNF)
      {
        if(cmd_ptr->cmd.rrc_csp_bplmn_suspend_cnf.status == SUCCESS)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN suspended successfully.");
          tdsrrcllc_update_drx_coef_length();
        }
        else
        {
          if(tdsutran_drx_cycle_info.drx_cycle2_timer_value !=0)
          {
            tdsrrctmr_start_timer(TDSRRCTMR_T_319_TIMER,tdsutran_drx_cycle_info.drx_cycle2_timer_value);
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"UTRAN DRX Coef Length 2 in use");
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"T319 timer = %d,UTRAN DRX Coef Length 1 in use",tdsutran_drx_cycle_info.drx_cycle2_timer_value);
          }
        }
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid event %d in Wait for BPLMN Suspend cnf substate",cmd_ptr->cmd_hdr.cmd_id);
      }
      break;
  }

return;
}

/*===========================================================================

FUNCTION tdsrrcmisc_state_change_handler

DESCRIPTION
  Used to handle state change to/from Cell PCH (or URA PCH)
  
DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcmisc_state_change_handler
(
  tdsrrc_cmd_type *cmd_ptr
)
{
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"State changed to %d from %d",cmd_ptr->cmd.state_change_ind.new_state,cmd_ptr->cmd.state_change_ind.previous_state);
    if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_PCH ||
        cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_URA_PCH)
    {
      if(tdsrrccu_get_cu_wait_timer_value() != TDSRRC_T320_INVALID_VALUE)
      {
        tdsrrctmr_start_timer(TDSRRCTMR_T_320_TIMER, 1000*tdsrrccu_get_cu_wait_timer_value());

        /*Now we have used the value of wait timer to start T320. So we need to reset it.*/		
        tdsrrccu_reset_cu_wait_timer_value();
      }
    }
    /*This case handles the scenarios when next state is not PCH and T320 isn't running.
        In that case, we need to reset wait timer value.*/	 
    else if(tdsrrctmr_get_remaining_time(TDSRRCTMR_T_320_TIMER) == 0)
    {
      tdsrrccu_reset_cu_cause_for_wait_timer();
    }

    /*If UE is going to Idle mode, we need to reset wait timer value and cu cause in CU.*/			
    if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
    {
      tdsrrccu_reset_cu_wait_timer_value(); 
      tdsrrccu_reset_cu_cause_for_wait_timer();
    }


  if(
      (cmd_ptr->cmd.state_change_ind.previous_state == TDSRRC_STATE_CELL_DCH ||
       cmd_ptr->cmd.state_change_ind.previous_state == TDSRRC_STATE_CELL_FACH ) 
       &&
      (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_PCH ||
           cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_URA_PCH)
    )
  {
    if(tdsutran_drx_cycle_info.drx_cycle2_timer_value !=0)
    {
      tdsrrctmr_start_timer(TDSRRCTMR_T_319_TIMER, tdsutran_drx_cycle_info.drx_cycle2_timer_value);
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"UTRAN DRX Coef Length 2 in use");
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"T319 timer = %d,UTRAN DRX Coef Length 1 in use",tdsutran_drx_cycle_info.drx_cycle2_timer_value);
    }
  }
  
  if(
      (cmd_ptr->cmd.state_change_ind.previous_state == TDSRRC_STATE_CELL_PCH || 
        cmd_ptr->cmd.state_change_ind.previous_state == TDSRRC_STATE_URA_PCH) 
        && 
        (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_DCH ||
        cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_FACH)
    )
  {
    if(tdsrrctmr_get_remaining_time(TDSRRCTMR_T_319_TIMER) >0)
    {
      tdsrrctmr_stop_timer(TDSRRCTMR_T_319_TIMER);
  
      tdsrrcllc_update_drx_coef_length();
    }
  }
}


/*===========================================================================

FUNCTION tdsrrcmisc_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the RRC Misc procedure.
  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (received from RLC)
    TDSRRC_SIM_INSERTED_REQ primitive (received from MM)
    TDSRRC_SIM_UPDATE_REQ primitive   (received from MM)


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcmisc_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
  tdsl1_ui_display_info_struct_type l1_data_for_ui_display;
#else
  tdsl1_rssi_info_struct_type l1_rssi;
#endif

  /* RSSI Enhancement */
  if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_STATE_CHANGE_IND)
  {

    if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED) 
    {
      tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSMAX_NUM_OF_PROCEDURES);
    }


#ifdef  FEATURE_TDSCDMA_DATA_FOR_UI_BAR_DISPLAY
    tdsl1_get_data_for_ui_display(&l1_data_for_ui_display);
    tdsrrc_rssi = l1_data_for_ui_display.rssi_value; 
    tdsrrc_ecio = l1_data_for_ui_display.ecio_value;
    tdsrrc_rscp = l1_data_for_ui_display.rscp_value;
    tdsrrc_use_ecio_for_ui_display = l1_data_for_ui_display.use_ecio_for_display;
#else
    tdsl1_get_rssi(&l1_rssi);
    tdsrrc_rssi = l1_rssi.rssi_value; 
#endif   
    
    /*If we're going out of Cell_DCH state, we need to invalidate the tdslac_in_ota.*/
    if((cmd_ptr->cmd.state_change_ind.new_state != TDSRRC_STATE_CELL_DCH) &&
       (cmd_ptr->cmd.state_change_ind.previous_state == TDSRRC_STATE_CELL_DCH))
    {
      tdslac_in_ota_present = FALSE;
#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS		  
      tdsrac_in_ota_present = FALSE; 
#endif	  
#ifdef FEATURE_TDSCDMA_SRNS_MO_CALL_DEFER        
      /*update tds lac/rac in ota bak to false for SRNS check*/
      tdslac_in_ota_present_bak = FALSE;
#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS		         
      tdsrac_in_ota_present_bak = FALSE;
#endif	  
#endif

      tdsrrcmeas_cell_id_in_dch_valid = FALSE;
      tdsrrcmeas_hho_happened_in_dch = FALSE;
    }
    if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
    {
      tdsrrc_ordered_state = TDSRRC_STATE_MAX;
    }

    /*In case of a state change indication*/
    tdsrrcmisc_state_change_handler(cmd_ptr);
    
    return;
  }
  if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_PROCEDURE_IND )
  {
    tdsrrcmisc_process_proc_ind(cmd_ptr);
    return;
  }

  switch (rrc_state)
  {
    case TDSRRC_STATE_DISCONNECTED: 
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_SIM_INSERTED_REQ:
          tdsrrcmisc_process_sim_inserted_req(cmd_ptr);
          break;

        case TDSRRC_SIM_UPDATE_REQ:
          tdsrrcmisc_process_sim_update_req(cmd_ptr);
          break;

        case TDSRRC_DOWNLINK_SDU_IND:     
          /* In these states, if RRC dispatcher failes to identify OTA message
             Type, RRC does not require to send RRC STATUS message.
          */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to identify the Message Type");
          break;
 
        case TDSRRC_T320_EXPIRED_IND:
          tdsrrcmisc_process_T320_expiry(cmd_ptr);
          break;

        case TDSRRC_T319_EXPIRED_IND:
        case TDSRRC_BPLMN_SUSPEND_CNF:
          tdsrrcmisc_process_multiple_drx(cmd_ptr);
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid event %d is forwarded to MISC proc",cmd_ptr->cmd_hdr.cmd_id);
          break;      }
      break;

    case TDSRRC_STATE_CONNECTING:
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_SIM_INSERTED_REQ:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIM Inserted rcvd in invalid state");
          break;

        case TDSRRC_SIM_UPDATE_REQ:
          tdsrrcmisc_process_sim_update_req(cmd_ptr);
          break;

        case TDSRRC_DOWNLINK_SDU_IND:
          /* In this state, if RRC dispatcher failes to identify OTA message
             Type, RRC does not require to send RRC STATUS message.
          */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unable to identify the Message Type");
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid event %d is forwarded to MISC proc",cmd_ptr->cmd_hdr.cmd_id);
          break;

      }
      break;

    case TDSRRC_STATE_CELL_FACH:    
    case TDSRRC_STATE_CELL_DCH:  
      switch(cmd_ptr->cmd_hdr.cmd_id)
      {
        case TDSRRC_SIM_INSERTED_REQ:
          tdsrrcmisc_process_sim_inserted_req(cmd_ptr);
          break;

        case TDSRRC_SIM_UPDATE_REQ:
          tdsrrcmisc_process_sim_update_req(cmd_ptr);
          break;

        case TDSRRC_DOWNLINK_SDU_IND:
          /* In these states,if RRC dispatcher failes to identify OTA message
             type RRC need to send RRC STATUS message if message is received 
             on DCCH.
          */
          tdsrrcmisc_process_downlink_msg(cmd_ptr);
          break; 

        case TDSRRC_T319_EXPIRED_IND:
          tdsrrcllc_update_drx_coef_length();
          break;


        default:     
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid event %d is forwarded to MISC proc",cmd_ptr->cmd_hdr.cmd_id);
          break;
      }
      break;


    default: 
      /* This should never happen */
      ERR_FATAL("Event is forwarded to MISC in an invalid State: %d",rrc_state,0,0);
      break;
   }  
} /* tdsrrcmisc_procedure_event_handler */

/*===========================================================================

FUNCTION tdsrrcmisc_init_procedure

DESCRIPTION
  This function initializes everything that the RRC Misc procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcmisc_init_procedure(void)
{
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id;
#endif
  tdsrrcmisc_multiple_drx_substate =  TDSRRCMISC_MULTIPLE_DRX_INITIAL;
  /* Initialize the lo and hi access classes to invalid values */
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  for (sub_id =0; (sub_id >= SYS_MODEM_AS_ID_1) && (sub_id < (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS); sub_id++)
  {
    tdsrrc_set_identity_info_to_global_params(sub_id);
    tdsrrc_lo_access_class = RRC_LO_ACCESS_CLASS_ABSENT;
    tdsrrc_hi_access_class = RRC_HI_ACCESS_CLASS_ABSENT;
    tdsrrc_set_identity_info_to_related_sub_params(sub_id);
  }
#else
  tdsrrc_lo_access_class = RRC_LO_ACCESS_CLASS_ABSENT;
  tdsrrc_hi_access_class = RRC_HI_ACCESS_CLASS_ABSENT;
#endif

  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_MISC,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_WILDCARD);
  /* Clear SIB DB upon LPM */
  tdsrrc_clear_sibdb_for_lpm = TRUE;
} /* tdsrrcmisc_init_procedure */


#ifdef FEATURE_TDSCDMA_REL7_SPECRS 

/***************RRC CO UEC ************************************/



/*===========================================================================
FUNCTION tdsrrccouec_save_ue_cap

DESCRIPTION
  This function saves the requested UE  capabilities. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_save_ue_cap
(
  void
)
{
  if(tdsrrccouec.ue_cap_change_requested.is_valid_mask & TDSHSDPA_CAT_INCL)
  {
    tdsrrc_ue_cap_during_rrc_conn.hsdpa_category = tdsrrccouec.ue_cap_change_requested.hsdpa_category;
  }
  if(tdsrrccouec.ue_cap_change_requested.is_valid_mask & TDSHSUPA_CAT_INCL)
  {
    tdsrrc_ue_cap_during_rrc_conn.hsupa_category = tdsrrccouec.ue_cap_change_requested.hsupa_category;
  }
}


/*===========================================================================

FUNCTION tdsrrccouec_send_couec_cnf

DESCRIPTION
  This function sends  TDSRRC_COUEC_CNF comamnd to the requested procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_send_couec_cnf
(
  tdsrrc_couec_cnf_e_type couec_cnf
)
{
  tdsrrc_cmd_type *rrc_cmd_ptr;                 /* Pointer to the RRC Command */
  
  if((rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
  {
    /* Fill in the command parameters for TDSRRC_COUEC_CNF */
    rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_COUEC_CNF;
    rrc_cmd_ptr->cmd.rrc_couec_cnf.proc_id = tdsrrccouec.proc_id;
    rrc_cmd_ptr->cmd.rrc_couec_cnf.ueci_cnf = couec_cnf;
    /* Send TDSRRC_COUEC_CNF primitive */
    tdsrrc_put_int_cmd(rrc_cmd_ptr);       
  }
  else
  {
    ERR_FATAL("Memory allocation for the %d cmd failed",TDSRRC_COUEC_CNF,0,0);
   }
}


/*===========================================================================

FUNCTION tdsrrccouec_process_initial_substate

DESCRIPTION
  This function is the substate event handler of COUEC procedure. This substate handler handles following commands.
  1. TDSRRC_COUEC_REQ 
  2. TDSRRC_STATE_CHANGE_IND - It resets the COUEC procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_process_initial_substate
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{

  tdsrrc_cmd_type *rrc_cmd_ptr;                 /* Pointer to the RRC Command */
  
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_COUEC_REQ:

      tdsrrccouec.proc_id = cmd_ptr->cmd.rrc_couec_req.proc_id;
      
      /*Check whether RNC supports Change in UE Capability or not*/
      if(tdsrrc_rnc_cap_change_support == FALSE)
      {
        /*If RNC doesn't support Change in UE Capability, send unsupported to requested proc.*/
        tdsrrccouec_send_couec_cnf(TDSRRC_UE_CAP_CHANGE_UNSUPPORTED);
        tdsrrccouec_clear_procedure();
      }
      else if(cmd_ptr->cmd.rrc_couec_req.ue_cap_requested.is_valid_mask)        
      {
        /*To initiate TDSRRC_INITIATE_UE_CAPABILITY_REQ*/
        if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        {     
          /* Fill in the command parameters for TDSRRC_INITIATE_UE_CAPABILITY_REQ */
          rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INITIATE_UE_CAPABILITY_REQ;
  
          rrc_cmd_ptr->cmd.initiate_ue_cap.originator = 
              TDSRRC_UE_CAPABILITY_ORIGINATOR_COUEC;

          rrc_cmd_ptr->cmd.initiate_ue_cap.transaction_id = TDSRRCUECI_INVALID_TRANSACTION_ID; 
  
          if(cmd_ptr->cmd.rrc_couec_req.ue_cap_requested.is_valid_mask & TDSHSDPA_CAT_INCL)
          {
            tdsrrccouec.ue_cap_change_requested.is_valid_mask |= TDSHSDPA_CAT_INCL;
            tdsrrccouec.ue_cap_change_requested.hsdpa_category = cmd_ptr->cmd.rrc_couec_req.ue_cap_requested.hsdpa_category;
          }
          
          if(cmd_ptr->cmd.rrc_couec_req.ue_cap_requested.is_valid_mask & TDSHSUPA_CAT_INCL)
          {
            tdsrrccouec.ue_cap_change_requested.is_valid_mask |= TDSHSUPA_CAT_INCL;
            tdsrrccouec.ue_cap_change_requested.hsupa_category = cmd_ptr->cmd.rrc_couec_req.ue_cap_requested.hsupa_category;
          }       
          /* Send TDSRRC_INITIATE_UE_CAPABILITY_REQ primitive */
          
          tdsrrc_put_int_cmd(rrc_cmd_ptr);   
          
          tdsrrccouec.substate = TDSRRCCOUEC_WAIT_FOR_UECI_CNF;
          tdsrrccouec.num_of_retries++;          
        } 
        else
        {
          ERR_FATAL("Memory allocation for the %d cmd failed",TDSRRC_INITIATE_UE_CAPABILITY_REQ,0,0);
        }
      }
      else
      {
        /*In this case, the requested procedure hasn't changed any UE capability, so just send NONE.*/
        tdsrrccouec_send_couec_cnf(TDSRRC_UE_CAP_CHANGE_NONE);
        tdsrrccouec_clear_procedure();
      }
      break;
  
    case TDSRRC_STATE_CHANGE_IND: 
      /*Since we are going to disconnected state (Idle mode), we need to
      reset tdsrrc_rnc_cap_change_support to FALSE and reset the COUEC procedure.*/   
      if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
      {
        tdsrrccouec_clear_procedure();
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unsupported cmd %d in initial substate of UECI Proxy",cmd_ptr->cmd_hdr.cmd_id);        
  }
}

/*===========================================================================

FUNCTION tdsrrccouec_process_wait_for_timer_couec_to_expire

DESCRIPTION
  This function is the substate event handler of RRC COUEC procedure. This substate handler handles following commands.
  1. TDSRRC_TIMER_COUEC_EXPIRED_IND - It handles the expiry of COUEC timer.
  2. TDSRRC_STATE_CHANGE_IND - It resets the COUEC procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_process_wait_for_timer_couec_to_expire
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{

  tdsrrc_cmd_type *rrc_cmd_ptr;                 /* Pointer to the RRC Command */
  
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_TIMER_COUEC_EXPIRED_IND:
        
      if(tdsrrccouec.num_of_retries < TDSN_COUEC_TIMER)
      {
        tdsrrctmr_start_timer(TDSRRCTMR_COUEC_TIMER, TDSRRCTMR_COUEC_TIMER_IN_MS);
        
        /*To initiate TDSRRC_INITIATE_UE_CAPABILITY_REQ*/
        if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        {     
        /* Fill in the command parameters for TDSRRC_INITIATE_UE_CAPABILITY_REQ */
          rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INITIATE_UE_CAPABILITY_REQ;
  
          rrc_cmd_ptr->cmd.initiate_ue_cap.originator = 
              TDSRRC_UE_CAPABILITY_ORIGINATOR_COUEC;
  
          rrc_cmd_ptr->cmd.initiate_ue_cap.transaction_id = TDSRRCUECI_INVALID_TRANSACTION_ID; 
  
          /* Send TDSRRC_INITIATE_UE_CAPABILITY_REQ primitive */
          tdsrrc_put_int_cmd(rrc_cmd_ptr);   
          tdsrrccouec.substate = TDSRRCCOUEC_WAIT_FOR_UECI_CNF;
          tdsrrccouec.num_of_retries++;          
        } 
        else
        {
          ERR_FATAL("Memory allocation for the %d cmd failed",TDSRRC_INITIATE_UE_CAPABILITY_REQ,0,0);
        }               
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"No. of retries is more than TDSN_COUEC_TIMER.");
        tdsrrccouec_send_couec_cnf(TDSRRC_UE_CAP_CHANGE_REFUSED);
        tdsrrccouec_clear_procedure();
      }
      break;

    case TDSRRC_STATE_CHANGE_IND: 
      /*Since we are going to disconnected state (Idle mode), we need to
      reset tdsrrc_rnc_cap_change_support to FALSE reset the COUEC procedure.*/   
      if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
      {
        tdsrrccouec_clear_procedure();
      }
      break;      
      
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unexpected cmd %d in COUEC.",cmd_ptr->cmd_hdr.cmd_id);        
  }
}

/*===========================================================================

FUNCTION tdsrrccouec_process_wait_for_ueci_cnf

DESCRIPTION
  This function is the substate event handler of COUEC procedure. This substate handler handles following commands.
  1. TDSRRC_STATE_CHANGE_IND - It resets the COUEC procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_process_wait_for_ueci_cnf
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_STATE_CHANGE_IND: 
    /*Since we are going to disconnected state (Idle mode), we need to
    reset tdsrrc_rnc_cap_change_support to FALSE reset the COUEC procedure.*/   
    if(cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
    {
      tdsrrccouec_clear_procedure();
    }
    break;      

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unexpected cmd %d in COUEC.",cmd_ptr->cmd_hdr.cmd_id);         
  }
}

/*===========================================================================

FUNCTION tdsrrccouec_procedure_event_handler

DESCRIPTION
  This function is the procedure event handler of COUEC procedure. Currently, it processes the following
  RRC Internal commands.
  1. TDSRRC_COUEC_REQ
  2. TDSRRC_STATE_CHANGE_IND
  3. TDSRRC_TIMER_COUEC_EXPIRED_IND

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
  /*If RRC Conn Rel is in progress, no need to process any RRC command in UECI Proxy*/
  if (FALSE == tdsrrcrcr_is_proc_active())
  {
    switch(tdsrrccouec.substate)
    {
      case  TDSRRCCOUEC_INITIAL:
        tdsrrccouec_process_initial_substate(cmd_ptr);     
        break;

      case TDSRRCCOUEC_WAIT_FOR_UECI_CNF:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"COUEC is active and waiting for UECI to complete.");       
        break;      
      
      case  TDSRRCCOUEC_WAIT_FOR_COUEC_TIMER_TO_EXPIRE:
        tdsrrccouec_process_wait_for_timer_couec_to_expire(cmd_ptr);
        break;
      
      default:
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Command %d forwarded in Invalid COUEC substate %d",cmd_ptr->cmd_hdr.cmd_id,tdsrrccouec.substate);       
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Conn Rel is in progress. COUEC not processing cmd %d",cmd_ptr->cmd_hdr.cmd_id);
    tdsrrccouec_clear_procedure();
  }
}

/*===========================================================================

FUNCTION tdsrrccouec_clear_procedure

DESCRIPTION
  This function clears the COUEC  procedure. Essentially, it resets the procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_clear_procedure
(
  void
)
{
  /*Reset all the globals within COUEC procedure*/
  tdsrrccouec.ue_cap_change_requested.is_valid_mask = 0;
  tdsrrccouec.substate = TDSRRCCOUEC_INITIAL;
  tdsrrccouec.num_of_retries = 0;
  tdsrrccouec.proc_id = TDSRRC_PROCEDURE_NONE;
}

/*===========================================================================

FUNCTION tdsrrccouec_init_procedure

DESCRIPTION
  This function initializes required parameters for Change of UE Capability procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrccouec_init_procedure
(
  void
)
{
  /* Set COUEC procedure state to TDSRRCCOUEC_INITIAL at powerup */
  tdsrrccouec.substate = TDSRRCCOUEC_INITIAL;

  tdsrrccouec.num_of_retries = 0;

  tdsrrccouec.proc_id = TDSRRC_PROCEDURE_NONE;

  /*Set the valid mask to 0 in UE Cap database internal to COUEC */
  tdsrrccouec.ue_cap_change_requested.is_valid_mask = 0;

  /* Register for transition from any state to Disconnected state */
  tdsrrcscmgr_register_for_scn(TDSRRC_PROCEDURE_COUEC,
                            TDSRRC_STATE_WILDCARD,
                            TDSRRC_STATE_DISCONNECTED);
}
/***************RRC CO UEC ************************************/
#endif/* FEATURE_TDSCDMA_REL7_SPECRS*/



/*====================================================================
FUNCTION: rrcllc_free_config_ptrs()

DESCRIPTION:
  This function is called to free the allocated OC, CC and TOC pointers

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void tdsrrc_free_dynamic_ptrs(
  void
  )
{
  int count;
  TDSRRC_MSG4(MSG_LEGACY_HIGH,"Free config ptrs: 0x%x 0x%x 0x%x 0x%x", 
              tdsordered_config_ptr, 
              tdscurrent_config_ptr, 
              tdstransition_config.toc_ptr,
              tdssib_events_ptr);

  if ((tdsordered_config_ptr == NULL) || (tdscurrent_config_ptr == NULL) || (tdstransition_config.toc_ptr == NULL))
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Trying to free the config pointers that are already free! 0x%x 0x%x 0x%x", 
              (uint32)tdsordered_config_ptr, (uint32)tdscurrent_config_ptr, (uint32)tdstransition_config.toc_ptr);
  }

  if(tdsordered_config_ptr != NULL)
  {
    modem_mem_free(tdsordered_config_ptr, MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT );
    tdsordered_config_ptr = NULL;
  }
   
  if(tdscurrent_config_ptr != NULL)
  {
    modem_mem_free(tdscurrent_config_ptr, MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT );
    tdscurrent_config_ptr = NULL;
  }

  if(tdstransition_config.toc_ptr != NULL)
  {
    modem_mem_free(tdstransition_config.toc_ptr, MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT );
    tdstransition_config.toc_ptr = NULL;
  }
  for( count=0; count<=TDSSIBDB_MAX_INDEX; count++ )
  {
    {
      tdsrrcsibdb_free_all_decodedsibs_for_a_cell(count);
    }
  }
  tdsrrcsib_free_bplmn_sibs();
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
  (void) tdsrrciho_free_sib16();
#endif

  /*Calling rrc_sib_proc_events_init which in turn will call tdsrrcsib_free_tsib
       in order to free the tempholder pointer which is used to hold MIB 
       and SIB3 temporarily before freeing sib_events_ptr.
   */
      
  tdsrrcsib_proc_events_init();
  /*tdsrrc_free_dynamic_ptrs is called when TDS is stopped*/
  tdsrrcsibdb_invalid_sibdb_for_quick_cell_resel();

  if(tdssib_events_ptr !=NULL)
  {
    modem_mem_free(tdssib_events_ptr, MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT );  
    tdssib_events_ptr = NULL;
  }

#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrcsibdb_free_pseudo_sib19();
#endif /* FEATURE_TDSCDMA_SIB19 */

  TDSRRC_MSG4(MSG_LEGACY_HIGH,"Freed config ptrs 0x%x 0x%x 0x%x 0x%x", 
              tdsordered_config_ptr, 
              tdscurrent_config_ptr, 
              tdstransition_config.toc_ptr,
              tdssib_events_ptr);

  tdsrrcmeas_free_dynamic_ptrs();

  // New function call l2_free_mem_ZI_opt() here to replace the MAC_UL_ZI_REDUCTION
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freeing dynamic memeory for MAC");
  tdsmac_ul_free_mem_ZI_opt();
}
/*====================================================================
FUNCTION: rrcllc_initiate_config_ptrs()

DESCRIPTION:
  This function is called to allocate OC, CC and TOC pointers

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void tdsrrc_init_dynamic_ptrs(
  void
  )
{

  TDSRRC_MSG4(MSG_LEGACY_HIGH,"Init config ptrs: 0x%x 0x%x 0x%x 0x%x", 
              tdsordered_config_ptr, 
              tdscurrent_config_ptr, 
              tdstransition_config.toc_ptr,
              tdssib_events_ptr);
  /* Always free dynamic pointers before initialise them*/
  tdsrrc_free_dynamic_ptrs();

  if ((tdsordered_config_ptr != NULL) || (tdscurrent_config_ptr != NULL) || (tdstransition_config.toc_ptr != NULL))
  {
    ERR_FATAL("Trying to initialize the config pointers that are already initialized 0x%x 0x%x 0x%x", 
              (uint32)tdsordered_config_ptr, (uint32)tdscurrent_config_ptr, (uint32)tdstransition_config.toc_ptr);
  }


  tdscurrent_config_ptr =  modem_mem_calloc(1,sizeof(tdsordered_config_type), MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT);
  tdsordered_config_ptr =  modem_mem_calloc(1,sizeof(tdsordered_config_type), MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT);
  tdstransition_config.toc_ptr =  modem_mem_calloc(1,sizeof(tdsordered_config_type), MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT);
  tdssib_events_ptr = modem_mem_calloc(1,sizeof(tdssib_active_event_database_type), MODEM_MEM_CLIENT_TDSCDMA_RRC_GLOBAL_CRIT);
       
  if((tdsordered_config_ptr == NULL) || (tdscurrent_config_ptr == NULL) || (tdstransition_config.toc_ptr == NULL)
    ||(tdssib_events_ptr == NULL))
  {
      ERR_FATAL("Dynamic initialization of config ptrs failed! 0x%x 0x%x 0x%x", 
              (uint32)tdsordered_config_ptr, (uint32)tdscurrent_config_ptr, (uint32)tdstransition_config.toc_ptr);
  }
 
  TDSRRC_MSG4(MSG_LEGACY_HIGH,"Allocated config ptrs 0x%x 0x%x 0x%x 0x%x", 
              tdsordered_config_ptr, 
              tdscurrent_config_ptr, 
              tdstransition_config.toc_ptr,
              tdssib_events_ptr);

  tdsrrcllc_init_config_ptrs();

  tdsrrcmeas_init_dynamic_ptrs();

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Allocating dynamic memory - for MAC");
  tdsmac_ul_allocate_mem_ZI_opt();
}

/*===========================================================================

FUNCTION tdsrrcmisc_is_in_band_F

DESCRIPTION
  This function checks if the input frequency is in band F (B39).

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: in band F.
  FALSE: not in band F.

===========================================================================*/
boolean tdsrrcmisc_is_in_band_F(uint16 freq)
{
   return (((freq >= TDSL1_BAND_F_CHAN_MIN) && (freq <= TDSL1_BAND_F_CHAN_MAX)) ? TRUE : FALSE);
}

/*===========================================================================

FUNCTION tdsrrcmisc_check_da_backoff

DESCRIPTION
  FR19720. This function checks dual active RF backoff condition.

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: back-off conditions are met.
  FALSE: not back-off condition.

===========================================================================*/
boolean tdsrrcmisc_check_da_backoff()
{
   boolean ret = FALSE;


#ifdef FEATURE_SGLTE
   tdsrrclog_da_backoff_timer_start_event_type da_backoff_timer_start_event;
   TDSRRC_MSG2(MSG_LEGACY_HIGH,"Check working frequency %d for band F,ue_mode = %d",tdscurrent_config_ptr->w_freq,tdsrrc_get_ue_mode());

   if ((tdsrrc_get_ue_mode() == TDSL1_MEAS_MODE_SGLTE)
       && (((tdsrrc_get_state() == TDSRRC_STATE_CONNECTING) && tdsrrcmisc_is_in_band_F(tdsrrcccm_get_curr_camped_freq()))
           || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) && tdsrrcmisc_is_in_band_F(tdsrrcccm_get_curr_camped_freq()))
           || ((tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH) && tdsrrcmisc_is_in_band_F(tdscurrent_config_ptr->w_freq)))
       && !tdsrrc_get_oos_due_to_no_resource_flag_status()
      )
   {      
#ifdef TEST_FRAMEWORK
      #error code not present
#else
      cxm_conn_tbl_type cxm_tbl = cxm_get_conn_state_tbl();

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"FR19720: cxm_get_conn_state_tbl UL active, G1=%d, G2=%d, G3=%d", 
                  cxm_tbl.conn_state[CXM_TECH_GSM1].is_ul_active,
                  cxm_tbl.conn_state[CXM_TECH_GSM2].is_ul_active,
                  cxm_tbl.conn_state[CXM_TECH_GSM3].is_ul_active);

      if (cxm_tbl.conn_state[CXM_TECH_GSM1].is_ul_active || cxm_tbl.conn_state[CXM_TECH_GSM2].is_ul_active)
      {
         ret = rfm_is_coex_backoff_req(TDRM_GET_PRI_DEVICE_ID(), RFM_TDSCDMA_MODE);
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"FR19720: TDS pri_device_id: %d",TDRM_GET_PRI_DEVICE_ID());
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"FR19720: rfm_is_coex_backoff_req = %d",ret);
      }
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"FR19720: band F avoidance enabled %d",ret);

      if (ret)
      {
         /* Start 2min timer and set global value */
         tdsrrctmr_start_timer(TDSRRCTMR_DA_BACKOFF_TIMER, TDSRRCISC_AVOID_BAND_F_TIME_MS);
         tdsrrcmisc_is_da_backoff = TRUE;

         /* Update band preference */
         tdsrrc_csp_int_data.band_pref = tdsrrccsp_extract_tds_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info);
         tdsrrccsp_update_band_preference();
         da_backoff_timer_start_event.start_band = tdsrrc_csp_int_data.band_pref;
         event_report_payload(EVENT_TDSCDMA_DA_BACKOFF_TIMER_START, sizeof(da_backoff_timer_start_event),
                                    (void *)&da_backoff_timer_start_event);
         
         /* Inform L1 to avoid measuring band F frequencies during IRAT */
         tdsl1_set_bandF_disabled(TRUE);
         
         /* directly go to idle because back-off is on*/
         if (tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_MISC, TDSRRC_TX_TO_DISCON_OTHER) != SUCCESS)
         {
            ERR_FATAL("Failed to go to Disconnected state",0,0,0);
         }
      }
   }
#endif

   return ret;
}

/*====================================================================
FUNCTION: tdsrrcmisc_da_backoff_timer_expired()

DESCRIPTION:
  This function process dual active RF backoff expiration.

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcmisc_da_backoff_timer_expired()
{
   TDSRRC_MSG0(MSG_LEGACY_HIGH,"FR19720: Process dual active RF backoff expiration");

   tdsrrcmisc_is_da_backoff = FALSE;

   /* Update band preference */
   tdsrrc_csp_int_data.band_pref = tdsrrccsp_extract_tds_band_pref_from_rat_pri_list(&tdsrrc_csp_int_data.rat_pri_list_info);
   tdsrrccsp_update_band_preference();

   /* Inform L1 to resume measuring band F frequencies during IRAT */
   tdsl1_set_bandF_disabled(FALSE);
}

/*====================================================================
FUNCTION: tdsrrcmisc_is_da_backoff_on()

DESCRIPTION:
  This function checks dual active RF backoff flag.

DEPENDENCIES:
  None.

RETURN VALUE:
  Boolean.
  
SIDE EFFECTS:

====================================================================*/
boolean tdsrrcmisc_is_da_backoff_on()
{
   return tdsrrcmisc_is_da_backoff;
}
#ifdef FEATURE_TDSCDMA_FATAL_RECOVERY 
/*====================================================================
FUNCTION: tdsrrcmisc_procss_hw_error_ind()

DESCRIPTION:
  This function process HW error from L1.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcmisc_procss_hw_error_ind()
{
   tdsrrc_mode_e_type rrc_mode = tdsrrcmcm_get_rrc_mode();
   tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

   if (rrc_mode == TDSRRC_MODE_ACTIVE)
   {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrcmisc_procss_hw_error_ind: process the request on rrc mode %d, state %d", rrc_mode, rrc_state);
      
      if (tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_MISC, TDSRRC_TX_TO_DISCON_OTHER) != SUCCESS)
      {
         ERR_FATAL("Failed to go to Disconnected state on HW error",0,0,0);
      }
      
   }
   else
   {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"tdsrrcmisc_procss_hw_error_ind: ignore request on rrc mode %d, state %d", rrc_mode, rrc_state);
   }
}
#endif

/*====================================================================
FUNCTION: tdsrrcmisc_process_qmi_drx_level_req()

DESCRIPTION:
  This function process drx level req from qmi.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcmisc_process_qmi_drx_level_req
(
  sys_modem_as_id_e_type as_id,
  uecomdef_status_e_type simid_validation
)
{
  tds_rrc_qmi_drx_level_rsp_type qmi_rsp;
  errno_enum_type status;
  uint32 drx_len;

  memset(&qmi_rsp,0,sizeof(tds_rrc_qmi_drx_level_rsp_type));

  if (simid_validation == SUCCESS)
  {
    if (tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_NONE) == TDSRRC_NOT_CAMPED_ON)
    {
      qmi_rsp.drx_status = SYS_DRX_STATUS_NO_SERVICE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QMI SYS_DRX_STATUS_NO_SERVICE");
    }
    else if (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED)
    {
      /* get drx length */
      drx_len = tdsrrcllc_get_drx_info();
      /* get drx level */
      qmi_rsp.drx_level = (uint8)((log((double)drx_len))/(log((double)2)));
      qmi_rsp.drx_status = SYS_DRX_STATUS_SUCCESS;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"QMI drx_len: %d,drx_level %d", drx_len,qmi_rsp.drx_level);
    }
    else
    {
      qmi_rsp.drx_status = SYS_DRX_STATUS_NOT_IDLE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"QMI SYS_DRX_STATUS_NOT_IDLE");
    }
  }
  else
  {
    qmi_rsp.drx_status = SYS_DRX_STATUS_OTHER;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"QMI SYS_DRX_STATUS_OTHER");
  }

  msgr_init_hdr(&qmi_rsp.msg_hdr, MSGR_TDSCDMA_RRC, TDSCDMA_RRC_QMI_DRX_LEVEL_RSP);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  msgr_set_hdr_variant(&qmi_rsp.msg_hdr, (uint8)as_id);
#else
  msgr_set_hdr_inst(&qmi_rsp.msg_hdr, (uint8)SYS_AS_ID_TO_INST_ID(as_id));
#endif
#endif
  status = msgr_send(&qmi_rsp.msg_hdr, sizeof(tds_rrc_qmi_drx_level_rsp_type));

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Send TDSCDMA_RRC_QMI_DRX_LEVEL_RSP, MSGR status: %d",status);

  if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
  {
    tdscfa_send_message_to_qmi(&qmi_rsp.msg_hdr, sizeof(tds_rrc_qmi_drx_level_rsp_type));
  }
}


