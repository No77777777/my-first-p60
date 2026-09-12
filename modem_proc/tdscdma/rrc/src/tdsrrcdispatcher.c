/*===========================================================================
                        R R C  D I S P A T C H E R 

DESCRIPTION

   This module receives and dispatches all external and
   internal commands to the other RRC modules. The RRC task
   has a central wait loop and that wait is performed in the
   dispatcher module. When there are no commands waiting for
   the RRC task, execution control is returned to the os.
   When one or more commands are posted for the RRC task and
   the RRC task is runnable, execution control is returned
   to the dispatcher, and it dispatches the pending commands to
   the appropriate procedures.
   
   tdsrrc_dispatcher() is the main function that waits for
   RRC signals to be set. When a signal is set, the function
   sequentially checks the possible RRC signals and takes
   the appropriate action.
   
   If any of the command signals are set, the dispatcher
   dequeues items from the command queue that has it's signal
   set and dispatches the items to the appropriate functions
   until the command queue is empty.   
   
INITIALIZATION AND SEQUENCING REQUIREMENTS
  
   RRC Task must have been started. All RRC command queues must
   have been initialized. All RRC signals must have been cleared
   at task startup.

Copyright (c) 2010-2011 Qualcomm Technologies, Incorporated. 
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.


===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcdispatcher.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
06/27/12   xp      Add a filter for redundant CCCH OTA message
06/17/12   hx      Add FEATURE_TDSCDMA_MEM_ALLOC_FREE_COUNTER to track L1->RRC 
                   command allocate/free
06/12/12   hx      Change the call flow to avoid race condition for sending UL data during 
                   SRNS relocation.
03/14/12   as      Added code to support feature NB/WB handover support
10/20/11   ysh      modify TDSRRC_SIB7_UPDATE_STATUS_IND's procedure sib_change_ind to sib7_update_status
10/13/11   mkh     Added log version to RRC log packets
09/08/11   mkh     Events renamed to TDS
07/07/11   mkh     Renamed log pkts to TDSxxx. 
03/14/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.93.50
03/14/11   kp      Bug fix to add cmd .i.d RRC_FAST_DORMANCY_REQ in rrc_check_mm_cmds
                   _for_simid_validation instead of procedure i.d.
03/10/11   yzh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
03/10/11   sks     Made changes to implement 5s guard timer for LTOW redirection.
03/02/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.92.50
02/24/11   kp      Added code to store & precess RLC reset during tuneaway
02/07/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.90.50
01/28/11   sks     Added support for LTOW PSHO.
01/28/11   rl      Merged with VU_MODEM_WCDMA_RRC.01.89.57
01/12/11   ad      Added changes for RRC SW decoupling
12/31/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.86.50
12/02/10   su      Provided fix to process TFCC message while another 
                   Reconfiguration is on going.
12/22/10   su      Made code changes to support FEATURE_WCDMA_ETWS.
11/29/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.85.50
11/22/10   sks     Added support for handling EM call during WRLF.
11/11/10   yzh     (3GPP: 10.2.1) ignore Active Set Update message when receive.
11/09/10   rl      Merged with VU_MODEM_WCDMA_RRC.01.84.50
11/08/10   ysh     Add Uplink Physical Channel Control. 3Gpp-8.2.10(3.2.6)
11/02/10   rm      Added code to handle CSFB status indication
10/28/10   as      Added changes to check for DL ACK for Tuneaway
10/27/10   as      Added changes to save DL SDU during Tuneaway
10/26/10   as      Added changes to skip subscription id check for DS Tuneaway status change ind
10/22/10   ysh     When Physical channel reconfiguration, compressed mode runtime error, reconfig failure,
                               this function is FDD only, should delete.3GPP-8.2.11.2(3.2.7)

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "queue.h"
#include "rex.h"
#include "event.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "tdsrrcdispatcher.h"
#include "tdsrrccmd.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include "tdsl1rrcif.h"
#include "tdsrrccmdi.h"
#include "tdsrrcdata.h"
#include "tdsrrcsigs.h"
#include "tdsrrclcm.h"
#include "tdsrrcllc.h"
#include "tdsrrccsp.h"
#include "tdsrrccspi.h"
#include "tdsrrcmm_v.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsibproc.h"
#include "tdsrrcccm.h"
#include "memheap.h"
#include "dsmutil.h"
#include "tdsrrcrbe.h"
#include "tdsrrcrbreconfig.h"
#include "tdsrrctcreconfig.h"
#include "tdsrrcpcreconfig.h"
#include "tdsrrcupphychctrl.h"
#include "tdsrrcrbr.h"
#include "tdsrrcrce.h"
#include "tdsrrcrcr.h"
#include "tdsrrcdt.h"
#include "tdsrrcpg1.h"
#include "tdsrrcmisc.h"
#include "tdsrrcuece.h"
#include "tdsrrcueci.h"
#include "tdsrrcmeas.h"
#include "tdsrrcsmc.h"
#include "tdsrrccu.h"
#include "tdsrrclbt.h"
//#include "dog.h"
#include "dog_hb_rex.h" //using DOG HB mechanism
#include "tdsrrcpg2.h"
#include "tdsrrccspdb.h"
#include "tdsrrcumi.h"
#include "tdsrrccho.h"
#include "tdsrrcmcm.h"
#include "tdsrrclogging.h"
#include "tdsrrcchk.h"
#include "tdsCFAParser.h"
#include "tdsrrcnv.h"

/*#include "tdsrrcrrif.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#include "tdsrrciho.h"
#ifdef T_WINNT  /* Include files when logging is enabled.*/
#error code not present
#endif

/*#include "rr_rrc_if.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"

#include "tdsseq.h"


#include "tdsrrctfcc.h"




#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
#include "tdsrrcmeasi.h"
#endif


/*#ifdef FEATURE_TDSCDMA_DSDS*/
#include "tdsrrcwrm.h"
/*#endif*/

#include "tdsrrcdormancy.h"


#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif



#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#include "tdsrrcfreefloating.h"
#endif

#ifdef FEATURE_TDSCDMA_TO_WCDMA
#include "rrc_tdsrrc_if.h"
#endif

#ifdef FEATURE_WCDMA_TO_TDSCDMA
#include "tdsrrc_rrc_if.h"
#endif

#include "tdsrrctmr.h"

#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
#include "time_svc.h"
#include "tdsrrcrbcommon.h" /*should be indirectly included already*/
#endif
#include "tdsCFAParser_i.h"
#include "tdsrrcqsh.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

typedef enum 
{
  TDSRRC_DEQUEUE_SUCCESS,              /* Status for success */
  TDSRRC_DEQUEUE_FAILURE               /* Status for failure */
}tdsrrc_sdu_dequeue_status_type;

typedef enum
{
  TDSRRC_NORMAL_OPERATION,
  TDSRRC_DEACT_INITIATED, /* Received DEACT_REQ from MM */
  TDSRRC_DEACT_COMPLETED  /* Received DEACT_CNF from Lower Layers */
}tdsrrc_deact_status_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */
tdsrrc_deact_status_type tdsrrc_deact_status = TDSRRC_NORMAL_OPERATION;

//used to get status of decoding.. if decoding error due to mem-allocation,
//then do not process the message any further..
extern tdsrrc_decode_info_type tdsrrc_decode_info;

extern tdsrrc_proc_e_type tdsrrc_wait_for_l2ack_for_reconfig;

extern tdsrrc_csp_int_data_type  tdsrrc_csp_int_data;
extern tdsrrcdt_int_data_type    tdsrrcdt_int_data;

#ifdef FEATURE_TDSCDMA_MEM_ALLOC_FREE_COUNTER
extern uint32 tdsfreeL1cmdCt;
#endif
extern trm_unlock_event_enum_t tdsrrcwrm_unlock_type;

/*logging last DL message for debug purpose*/
tdsrrc_log_dl_last_sigalling_message_type tdsrrc_last_dl_msg_rec[16];
/*Idx should be current log position + 1*/
uint8 tdsrrc_log_dl_last_ota_idx = 0;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */
#undef  TDSDOG_RRC_RPT
#define TDSDOG_RRC_RPT tdsrrc_dog_rpt_var
dog_report_type tdsrrc_dog_rpt_var = 0;      /* Initial Safety */
  
#if 0 //using DOG HB mechanism
#undef  TDSDOG_RRC_RPT_TIME
#define TDSDOG_RRC_RPT_TIME tdsrrc_dog_rpt_time_var
uint32  tdsrrc_dog_rpt_time_var = 0xFFFF;    /* Initial Safety */

/* Timer for kicking the watchdog */
static rex_timer_type tdsrrc_rpt_timer;
#endif

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/* Queue for RRC commands sent by LSM or TM task                 */
extern q_type tdsrrc_lsm_cmd_q;
#endif

#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
extern void tdsrrc_process_modem_stat_info(tdsrrc_cmd_type *cmd_ptr);
#endif
#endif

#ifdef FEATURE_MODEM_CONFIG_REFRESH
extern void tdsrrc_process_mcfg_refresh_ind
(
  tdsrrc_cmd_type *cmd_ptr
);
#endif

/* =======================================================================
**                            Function Definitions
** ======================================================================= */


/*===========================================================================
FUNCTION RRC_EVENT_SIG

DESCRIPTION
  Wait on the specified signal.  Handle signals as they come in. Only time
  critical watchdog and other signals are acknowledged.

  NOTE: The signal passed into this function is not cleared. It has to
  be cleared by the calling function.

DEPENDENCIES
  None
  
RETURN VALUE
  None

SIDE EFFECT
  Some signals are ignored till we're done with the signal

===========================================================================*/
void tdsrrc_event_wait( rex_sigs_type sig)
{
  rex_sigs_type sigs;             /* hold signals from rex_wait() */

  for( ;; )
  {                     /* until we get the specified signal */
    sigs = rex_wait(
          sig | TDSRRC_RPT_TIMER_SIG);

    /* Need to kick the watchdog */
    if( sigs & TDSRRC_RPT_TIMER_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_RPT_TIMER_SIG );
#if 0 //using DOG HB mechanism
      dog_report(TDSDOG_RRC_RPT);
      (void) rex_set_timer(&tdsrrc_rpt_timer, TDSDOG_RRC_RPT_TIME);
#else
      /* Report to the Dog HB */
      dog_hb_report(TDSDOG_RRC_RPT);
#endif
    }
    else if (sigs & sig) /* Received the specified signal. Exit the routine */
    {
      return;
    }
  } /* Continue waiting for the signal */
} /* tdsrrc_event_wait */


/*===========================================================================

FUNCTION    tdsrrcdispatch_identify_procedure 

DESCRIPTION
  This function looks into choice field and identifies the Message type and 
  then returns the corresponding procedure to which that particular message
  is benlongs.
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrc_proc_e_type

SIDE EFFECTS
  None

===========================================================================*/
static tdsrrc_proc_e_type tdsrrcdispatch_identify_procedure(
  tdsrrc_cmd_type *cmd_ptr                 /* To hold received command */

)
{      
  
  tdsrrc_proc_e_type rrc_proc = TDSRRC_PROCEDURE_NONE;
                                        /* Store name of procedure 
                                           that processes the decoded message */
  uint8 msg_type;                       /* Local variable to store message
                                           type */             
                                    
  switch(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  {
    case tdsrrc_BCCH_FACH_Message_PDU:
    case tdsrrc_BCCH_BCH_Message_PDU:
      /* In this case, there are only 2 types of messages possible both r processed
      by SIB proc.  The messages are
       tdsrrc_SystemInformation_BCH
       tdsrrc_SystemInformationChangeIndication */
      rrc_proc = TDSRRC_PROCEDURE_SIB;
      break;


#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
    case TDSRRCLCM_DL_DCCH_SRB5_SDU_TYPE:
      if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length > TDSRRC_SRB5_SDU_LEN) 
      {
        /* Since the TB size that can be expected on
           this logical ID is not more than 10 (3) bits,
           compare length against one octet */
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"TFCC message can't have more than one byte of length..ignoring..");
      } 
      else
      {
        rrc_proc = TDSRRC_PROCEDURE_TFC;         
      }
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"TFC Procedure hit - Data->0x%x; Length->%d",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu[0],cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      break;
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

    case tdsrrc_DL_DCCH_Message_PDU:
      msg_type = 
        tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      switch(msg_type)
      {
        /* The following needs to be updated as and when new RRC
           procedures are implemented */
        case T_tdsrrc_DL_DCCH_MessageType_downlinkDirectTransfer:
          rrc_proc = TDSRRC_PROCEDURE_DDT;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_rrcConnectionRelease:
          rrc_proc = TDSRRC_PROCEDURE_RCR;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_signallingConnectionRelease:
          rrc_proc = TDSRRC_PROCEDURE_SCR;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
          rrc_proc = TDSRRC_PROCEDURE_RBE;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
          rrc_proc = TDSRRC_PROCEDURE_RBRC;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
          rrc_proc = TDSRRC_PROCEDURE_TCR;
        break;
        case T_tdsrrc_DL_DCCH_MessageType_transportFormatCombinationControl:
          rrc_proc = TDSRRC_PROCEDURE_TFC;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
          rrc_proc = TDSRRC_PROCEDURE_PCR;
        break;
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
          rrc_proc = TDSRRC_PROCEDURE_RBR;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_ueCapabilityEnquiry:
          rrc_proc = TDSRRC_PROCEDURE_UECE;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_ueCapabilityInformationConfirm:
          rrc_proc = TDSRRC_PROCEDURE_UECI;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_measurementControl:
         rrc_proc = TDSRRC_PROCEDURE_MCMR;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_securityModeCommand:
          rrc_proc = TDSRRC_PROCEDURE_SMC;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_pagingType2:
          rrc_proc = TDSRRC_PROCEDURE_PG2;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
          rrc_proc = TDSRRC_PROCEDURE_UMI;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
        case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
          rrc_proc = TDSRRC_PROCEDURE_CU;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_cellChangeOrderFromUTRAN:
          rrc_proc = TDSRRC_PROCEDURE_CHO;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
          rrc_proc = TDSRRC_PROCEDURE_IHO;
          break;
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
        case T_tdsrrc_DL_DCCH_MessageType_assistanceDataDelivery:
          rrc_proc = TDSRRC_PROCEDURE_MCMR;
          break;
#endif
        case T_tdsrrc_DL_DCCH_MessageType_counterCheck:
          rrc_proc = TDSRRC_PROCEDURE_CHK;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_uplinkPhysicalChannelControl:
          rrc_proc = TDSRRC_PROCEDURE_UPCC;
          break;
#ifdef FEATURE_TDSCDMA_TO_LTE
        case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_EUTRA:
          rrc_proc = TDSRRC_PROCEDURE_IHO;
          break;
#endif
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"The msg type %d is not supported",msg_type );
          break;
      }  
      break;

    case tdsrrc_DL_CCCH_Message_PDU:

      msg_type = 
        tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      switch(msg_type )
      {
        /* The following needs to be updated as and when new RRC
           procedures are implemented */
        case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject:
          rrc_proc = TDSRRC_PROCEDURE_RCE;
          break;
        case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease:
          rrc_proc = TDSRRC_PROCEDURE_RCR;
          break;
        case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup:
          rrc_proc = TDSRRC_PROCEDURE_RCE;
          break;
        case T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm:
        case T_tdsrrc_DL_CCCH_MessageType_uraUpdateConfirm:
          rrc_proc = TDSRRC_PROCEDURE_CU;
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"The msg type %d is not supported",msg_type);
          break;
      }
      break;

    case tdsrrc_PCCH_Message_PDU:
      rrc_proc = TDSRRC_PROCEDURE_PG1;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"The SDU type %d is not supported",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
      break;
 
  }

  return(rrc_proc);
} /* end function tdsrrcdispatch_identify_procedure */



/*===========================================================================

FUNCTION    tdsrrcdispatch_perform_integrity_protection_check 

DESCRIPTION
  This function call the Security Procedure API to perform Integrity 
  Protection Check. If Integrity Protection Check is successful it
  it will decode the contigous stream of octects  using ASN1 
  to identify the message type. This message type is mapped to 
  the corresponding RRC procedure.
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrc_proc_e_type

SIDE EFFECTS
  None

===========================================================================*/
static uecomdef_status_e_type tdsrrcdispatch_perform_integrity_protection_check(
  tdsrrc_cmd_type *cmd_ptr,                 /* To hold received command */
  tdsrrc_proc_e_type *procedure
)
{  

  uecomdef_status_e_type status = FAILURE;   /* Initially set the status to failure */

  uint8 message_choice;                      /* Local variable to store message
                                                choice */

#ifdef FEATURE_QSH_EVENT_METRIC
  tdsrrcqsh_ota_pdu_type_e sdu_type = TDSRRC_QSH_SDU_TYPE_NONE;
#endif

  /* Initialize the procedure to NONE */                                    
  *procedure = TDSRRC_PROCEDURE_NONE;

  if (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu != NULL) 
  {
    /* Initialize decoded message to NULL, even though its expected to be set to NULL */
    if (NULL != cmd_ptr->cmd.downlink_sdu_ind.decoded_msg)
    {
      cmd_ptr->cmd.downlink_sdu_ind.decoded_msg = NULL;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"DL SDU's decoded msg pointer NOT set to NULL. Setting it to NULL");
    }

    /* Here Integrity Check can be peformed */
    if(SUCCESS == tdsrrcsmc_check_downlink_integrity(cmd_ptr) )
    {   
       /* Identifes the to which procedure the message belongs to */
       *procedure = tdsrrcdispatch_identify_procedure(cmd_ptr); 

       /* If procedure is one of the procedues that
        * can have activation time, timestamp he point when
        * Downlink SDU is received, before ASN1 decoding
        */
       tdsrrc_cfn_before_decode[*procedure] =tdsseq_get_cfn();

       status = SUCCESS;

       
       if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_CCCH_Message_PDU)
       {
         message_choice = tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                                       cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);

         if((T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup == message_choice) ||
            (T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject == message_choice)) 
         {
           if(TDSRRC_STATE_CONNECTING != tdsrrc_get_state())
           {
             status = FAILURE;
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE not in Connecting state.Ignoring ConnSetup/Reject message");
             return status;
           }
           if ((T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup == message_choice) &&
               (tdsrrcrce_verify_ue_identity_later_than_r3(cmd_ptr) == TDSRRCRCE_SUCCESS))
           {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE Identity check passed");
           } 
           else if ((T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject == message_choice) &&
                    ((tdsrrcrce_verify_ue_identity(cmd_ptr) == TDSRRCRCE_SUCCESS) ||
                    (tdsrrcrce_verify_ue_identity_later_than_r3(cmd_ptr) == TDSRRCRCE_SUCCESS)))
           {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE Identity check passed");
           }
           else
           {
             status = FAILURE;
             TDSRRC_MSG1(MSG_LEGACY_HIGH,"Msg %d for other UE",message_choice);
             
             return status;
           } 
         }
       }
#ifdef FEATURE_TDSCDMA_SRB5_SUPPORT
       else if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == TDSRRCLCM_DL_DCCH_SRB5_SDU_TYPE &&
               *procedure == TDSRRC_PROCEDURE_TFC)
       {
         return(status);
       }
#endif /* FEATURE_TDSCDMA_SRB5_SUPPORT */

      /* Check for NULL here required because there is a possibility that 
         PDU is already decoded while checking the incoming message for
         SRNS relocation case. */
      if (NULL == cmd_ptr->cmd.downlink_sdu_ind.decoded_msg)
      {
        /* Call the ASN.1 decoder to decode the message. */
        cmd_ptr->cmd.downlink_sdu_ind.decoded_msg =
        tdsrrcasn1_decode_pdu(  cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,(int )
                             cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type,
                             cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length
                          );
      }

      //if decoding fails, then check to see if it failed due to
      //malloc issues or not.. if malloc, err_fatal immediately,
      //else print a msg_error and let the procedure take care of
      //sending status_message
      if( cmd_ptr->cmd.downlink_sdu_ind.decoded_msg == NULL )
      {
        if( tdsrrc_decode_info.decode_status == TDSASN1_OUT_MEMORY)
        {
          ERR_FATAL("Decoding failed. pdu type %d, pdu size %d",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length,0);
        }
        else if(tdsrrc_decode_info.decode_status == TDSASN1_CONSTRAINT_VIOLATED)
          
          //decoding failed due to constraint check or other reasons
          //fwd the msg to procedures to be able to send STATUS msg with ASN1 failure
        {
          TDSRRC_MSG2(MSG_LEGACY_ERROR,"Decoding failed. pdu type %d,pdu size %d",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
        }

        /* Please notice that here status is NOT reset to FAILURE because for decode_status
         * other than TDSASN1_OUT_MEMORY, there are procedures which should send response back 
         * to NW. E.g., all the RB procedures will send back failure with cause 
         * rrc_asn1_ViolationOrEncodingError.
         */
      }
#ifdef FEATURE_TDSCDMA_ULDL_LOG_SUPPORT
      tdsrrc_get_dl_log_message(cmd_ptr);
#endif
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Integrity Check is failed");
    }

  } /*end if (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu != NULL) */
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"DL SDU length is 0");
  }


#ifdef FEATURE_QSH_EVENT_METRIC
  if(SUCCESS == status)
  {
    /* Save all DCCH and CCCH commands here. Cell update confirm is saved in CU */
    if((cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_CCCH_Message_PDU) ||
       (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU))
    {
      /* Don't buffer CU confirm OTA received on CCCH now as we may get for other UEs also */
      if(!((*procedure == TDSRRC_PROCEDURE_CU) && (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_CCCH_Message_PDU)))
      {
        sdu_type = (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_DL_DCCH_Message_PDU) ? TDSRRC_QSH_DCCH_DL_SDU : TDSRRC_QSH_CCCH_DL_SDU;
        if(sdu_type == TDSRRC_QSH_CCCH_DL_SDU)
        {
          message_choice = tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, 
                                                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
        }
        else
        {
          message_choice = tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, 
                                                cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
        }
        tdsrrcqsh_save_ota(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, 
                           cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length, 
                           sdu_type, 
                           message_choice);
      }
    }
  }
#endif

  return(status);
} /* end function tdsrrcdispatch_perform_integrity_protection_check */
#ifdef FEATURE_TDSCDMA_ULDL_LOG_SUPPORT
/*===========================================================================

FUNCTION    tdsrrcdispatch_get_dl_asn1_message_type

DESCRIPTION
  This function determines the type of the message received and returns this
  information as a byte suitable for reporting in a QXDM event.  This value
  is *not* the same as the ASN.1 based message type values returned by other
  similar functions.  See the rrclogging.h file for details.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns the message type if successful. Otherwise it returns 0xff.
  
SIDE EFFECTS
  None

===========================================================================*/
 uint8 tdsrrcdispatch_get_dl_asn1_message_type(
  tdsrrc_cmd_type              *cmd_ptr /* To hold received command */
)
{
  uint8 asn1_msg_type = 1;

  switch(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  {
    case tdsrrc_BCCH_FACH_Message_PDU:
      asn1_msg_type = T_tdsrrc_BCCH_FACH_MessageType_systemInformationChangeIndication;
      break;

    case tdsrrc_BCCH_BCH_Message_PDU:
      asn1_msg_type = T_tdsrrc_BCCH_FACH_MessageType_systemInformation;
      break;

    case tdsrrc_DL_DCCH_Message_PDU:
      asn1_msg_type = 
        tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      break;

    case tdsrrc_DL_CCCH_Message_PDU:

      asn1_msg_type = 
        tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      break;

    case tdsrrc_PCCH_Message_PDU:
      asn1_msg_type = T_tdsrrc_PCCH_MessageType_pagingType1;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"The SDU type %d is not supported",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
      break;
  }

  return (asn1_msg_type-1);
} /* end function tdsrrcdispatch_get_asn1_dl_message_type */
#endif
/*===========================================================================

FUNCTION    tdsrrcdispatch_get_dl_message_type

DESCRIPTION
  This function determines the type of the message received and returns this
  information as a byte suitable for reporting in a QXDM event.  This value
  is *not* the same as the ASN.1 based message type values returned by other
  similar functions.  See the tdsrrclogging.h file for details.
  
DEPENDENCIES
  None

RETURN VALUE
  uint8 : Returns the message type if successful. Otherwise it returns 0xff.
  
SIDE EFFECTS
  None

===========================================================================*/
static uint8 tdsrrcdispatch_get_dl_message_type(
  tdsrrc_cmd_type              *cmd_ptr /* To hold received command */
)
{
  uint8 asn1_msg_type;
  uint8 msg_type = 0xff;

  switch(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type)
  {
    case tdsrrc_BCCH_FACH_Message_PDU:
      msg_type = TDSRRCLOG_SYSTEM_INFORMATION_CHANGE_INDICATION_MSG;
      break;

    case tdsrrc_BCCH_BCH_Message_PDU:
      msg_type = TDSRRCLOG_SYSTEM_INFORMATION_MSG;
      break;


    case tdsrrc_DL_DCCH_Message_PDU:
      asn1_msg_type = 
        tdsrrc_get_dl_dcch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      switch(asn1_msg_type)
      {
        /* The following needs to be updated as and when new RRC
           procedures are implemented */
        case T_tdsrrc_DL_DCCH_MessageType_downlinkDirectTransfer:
          msg_type = TDSRRCLOG_DOWNLINK_DIRECT_TRANSFER_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_rrcConnectionRelease:
          msg_type = TDSRRCLOG_RRC_CONNECTION_RELEASE_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_signallingConnectionRelease:
          msg_type = TDSRRCLOG_SIGNALLING_CONNECTION_RELEASE_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerSetup:
          msg_type = TDSRRCLOG_RADIO_BEARER_SETUP_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerReconfiguration:
          msg_type = TDSRRCLOG_RADIO_BEARER_RECONFIGURATION_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_transportChannelReconfiguration:
          msg_type = TDSRRCLOG_TRANSPORT_CHANNEL_RECONFIGURATION_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_physicalChannelReconfiguration:
          msg_type = TDSRRCLOG_PHYSICAL_CHANNEL_RECONFIGURATION_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_radioBearerRelease:
          msg_type = TDSRRCLOG_RADIO_BEARER_RELEASE_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_ueCapabilityEnquiry:
          msg_type = TDSRRCLOG_UE_CAPABILITY_ENQUIRY_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_ueCapabilityInformationConfirm:
          msg_type = TDSRRCLOG_UE_CAPABILITY_INFORMATION_CONFIRM_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_activeSetUpdate:
          msg_type = TDSRRCLOG_ACTIVE_SET_UPDATE_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_measurementControl:
          msg_type = TDSRRCLOG_MEASUREMENT_CONTROL_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_securityModeCommand:
          msg_type = TDSRRCLOG_SECURITY_MODE_COMMAND_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_pagingType2:
          msg_type = TDSRRCLOG_PAGING_TYPE_2_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_utranMobilityInformation:
          msg_type = TDSRRCLOG_UTRAN_MOBILITY_INFORMATION_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_cellUpdateConfirm:
          msg_type = TDSRRCLOG_CELL_UPDATE_CONFIRM_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_uraUpdateConfirm:
          msg_type = TDSRRCLOG_URA_UPDATE_CONFIRM_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_cellChangeOrderFromUTRAN:
          msg_type = TDSRRCLOG_CELL_CHANGE_ORDER_FROM_UTRAN_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_handoverFromUTRANCommand_GSM:
          msg_type = TDSRRCLOG_HANDOVER_FROM_UTRAN_COMMAND_MSG;
          break;
        case T_tdsrrc_DL_DCCH_MessageType_counterCheck:
          msg_type = TDSRRCLOG_COUNTER_CHECK_MSG;
          break;

        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"The msg type %d is not supported",msg_type );
          break;
      }  
      break;

    case tdsrrc_DL_CCCH_Message_PDU:

      asn1_msg_type = 
        tdsrrc_get_dl_ccch_message_type(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                                     cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length);
      switch(asn1_msg_type)
      {
        /* The following needs to be updated as and when new RRC
           procedures are implemented */
        case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionReject:
          msg_type = TDSRRCLOG_RRC_CONNECTION_REJECT_MSG;
          break;
        case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionRelease:
          msg_type = TDSRRCLOG_RRC_CONNECTION_RELEASE_MSG;
          break;
        case T_tdsrrc_DL_CCCH_MessageType_rrcConnectionSetup:
          msg_type = TDSRRCLOG_RRC_CONNECTION_SETUP_MSG;
          break;
        case T_tdsrrc_DL_CCCH_MessageType_cellUpdateConfirm:
          msg_type = TDSRRCLOG_CELL_UPDATE_CONFIRM_MSG;
          break;
        case T_tdsrrc_DL_CCCH_MessageType_uraUpdateConfirm:
          msg_type = TDSRRCLOG_URA_UPDATE_CONFIRM_MSG;
          break;
        default:
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"The msg type %d is not supported",msg_type);
          break;
      }
      break;

    case tdsrrc_PCCH_Message_PDU:
      msg_type = TDSRRCLOG_PAGING_TYPE_1_MSG;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"The SDU type %d is not supported",cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
      break;
  }

  return msg_type;
} /* end function tdsrrcdispatch_get_dl_message_type */

/*===========================================================================

FUNCTION    tdsrrcdispatch_log_dl_sig_message 

DESCRIPTION
    This function will log the downlink encoded OTA messages into QXDM.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void tdsrrcdispatch_log_dl_sig_message
(
  tdsrrc_cmd_type *cmd_ptr                 /* To hold received command */
)
{  
  size_t msg_size;

#ifndef T_WINNT
  uint32 log_record_length;                 /* Local variable to store Log 
                                               Record Length */
  tdscdma_rrc_signalling_message_log_packet_type *log_record_ptr;
                                            /* Local variable to store Log
                                               record */
  tdsrrclog_sig_msg_event_type sig_msg_event;  /* Variable for message rx event */
  uint8 rb_id;                              /* Variable for radio bearer ID */
  uint8 channel_type;                       /* Variable for channel type */

#endif /* T_WINNT */ 

  uecomdef_logchan_e_type rrclog_dl_lc;     /*Variable required for logging*/

  /* Gets the Logical Channel Type of SDU for logging purpose */
  (void) tdsrrclcm_get_dl_lc_type(cmd_ptr->cmd.downlink_sdu_ind.lc_id,
                               &rrclog_dl_lc);

#ifdef T_WINNT
  #error code not present
#else  /* QXDM logging */

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
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Sig Msg rcvd on invalid Log Chan Type %d",rrclog_dl_lc);
      break;
  }

  /* Construct and send an event. */
  sig_msg_event.channel_type = channel_type;
  sig_msg_event.message_type = tdsrrcdispatch_get_dl_message_type(cmd_ptr);
  
  /* When NV for filter CCCH message is enabled, in the following case ,Some CCCH 
     will not be traced out in CELL_FACH state.
     1. RRC Connection Setup/reject message. (If RCE has got the ACK of RRC Connection setup complete)
     2. RRC Connection release/CU confirm/UU confirm messaage on CCCH,if Cell update or Ura udpate are both not started.
  */
  if ( (TDSCHECK_OPTIONAL_FEATURE_FILTER_CCCH_MSG) &&
       ((((sig_msg_event.message_type == TDSRRCLOG_RRC_CONNECTION_SETUP_MSG)||
       (sig_msg_event.message_type == TDSRRCLOG_RRC_CONNECTION_REJECT_MSG)) &&
       (TRUE == tdsrrcrce_get_rcsc_status()))||
       (((sig_msg_event.message_type == TDSRRCLOG_RRC_CONNECTION_RELEASE_MSG) || 
       (sig_msg_event.message_type == TDSRRCLOG_CELL_UPDATE_CONFIRM_MSG) ||
       (sig_msg_event.message_type == TDSRRCLOG_URA_UPDATE_CONFIRM_MSG) )&&
       (sig_msg_event.channel_type == TDSRRCLOG_SIG_DL_CCCH) && 
       ( FALSE == tdsrrccu_get_cu_uu_status()))))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Message %d received on CCCH in CELL_FACH.Ignore trace it. ",sig_msg_event.message_type);
    return ;
  }

  event_report_payload(EVENT_TDSCDMA_RRC_MESSAGE_RECEIVED, sizeof(sig_msg_event),
                       (void *)&sig_msg_event);

  /*log last DL signalling OTA message*/
  if(tdsrrc_log_dl_last_ota_idx < 16)
  {
    time_type up_ms;
    uint32 lo32;
    time_get_uptime_ms(up_ms);
    lo32 = qw_lo(up_ms);

    tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].rb_id = rb_id;
    tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].channel_type = channel_type;
    tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].message_type = sig_msg_event.message_type;
    tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].ota_time = lo32;
    tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].sdu_length = (uint16) cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length;

    memscpy((void *)(tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].signalling_message), 
              TDSMAX_SIZE_OF_SDU, cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
                  tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].sdu_length);
   
  }
  /*increase the idx after logging, and limit it to less than 15*/
  tdsrrc_log_dl_last_ota_idx ++; 
  tdsrrc_log_dl_last_ota_idx = tdsrrc_log_dl_last_ota_idx & 0xF;
  /*zero out the next log's id, while keep the sig content*/
  tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].rb_id = 0;
  tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].channel_type = 0;
  tdsrrc_last_dl_msg_rec[tdsrrc_log_dl_last_ota_idx].message_type = 0;


  

  /*Calculate the record length */
  log_record_length = 
    FPOS(tdscdma_rrc_signalling_message_log_packet_type, signalling_message) + 
    cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length;

  /*Allocate memory for Log Record. */
  log_record_ptr = (tdscdma_rrc_signalling_message_log_packet_type *)log_alloc( TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET, log_record_length);
  
  /* Make sure memory allocation is successful before to proceed with 
     logging */
  if(log_record_ptr != NULL)
  {
    log_record_ptr->version = TDSRRC_LOG_VERSION;

    /*Get the Logical Channel Type from LCM */
    (void) tdsrrclcm_get_dl_lc_type(cmd_ptr->cmd.downlink_sdu_ind.lc_id,
                                 &rrclog_dl_lc);
    /* Update RB Id.*/
    log_record_ptr->rb_id = rb_id;

    /*Update Logical Channel Type */
    log_record_ptr->channel_type = channel_type;

    /* Update length of the signalling message */
    if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length <= TDSRRCLOG_MAX_SIG_MSG_SIZE)
    {
      log_record_ptr->signalling_msg_length = 
        (uint16) cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length;
    }
    else
    {
      ERR_FATAL("dl_sdu_length > TDSRRCLOG_MAX_SIG_MSG_SIZE!",0,0,0);
    }

    /* Copy the Signalling mesaage payload to the log record */
    msg_size = memscpy((void *)log_record_ptr->signalling_message, 
                       log_record_ptr->signalling_msg_length,
           cmd_ptr->cmd.downlink_sdu_ind.dl_sdu,
           log_record_ptr->signalling_msg_length);

    TDSRRC_MSG1(MSG_LEGACY_MED,"Logging TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET %d bytes",msg_size);
    /*commit the log record */
    log_commit(log_record_ptr);
  }
  else
  {
    /* The QXDM is not turn ON. Hence Do not log the OTA messages */
  }

#endif /*T_WINNT */

} /* end function tdsrrcdispatch_log_dl_sig_message */

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
/*===========================================================================

FUNCTION    tdsrrc_log_dl_sig_message_for_ext_sibs

DESCRIPTION
    This function will log the downlink encoded OTA messages into QXDM for extension sibS.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrc_log_dl_sig_message_for_ext_sibs
(
  tdsrrc_SIB_type sib_type,
  tdssib_events_index_e_type index
)
{  
  size_t msg_size;
#ifndef T_WINNT
  uint32 log_record_length;                 /* Local variable to store Log 
                                               Record Length */
  tdscdma_rrc_signalling_message_log_packet_type *log_record_ptr;
                                            /* Local variable to store Log
                                               record */
  tdsrrclog_sig_msg_event_type sig_msg_event;  /* Variable for message rx event */
#endif /* T_WINNT */ 

  uecomdef_logchan_e_type rrclog_dl_lc = UE_LOGCHAN_BCCH;     /*Variable required for logging*/
  uint32 dl_sdu_len;
  uint8 msg_type = 0xff;
  uint8 rb_id;                              /* Variable for radio bearer ID */
  
  switch(sib_type)
  {
    case tdsrrc_SIB11bis:         /* Broadcast Control Channel */
      msg_type = TDSRRCLOG_SIB11bis_MSG;
      break;

    case tdsrrc_SIB19:         /* Paging Control Channel.*/
      msg_type = TDSRRCLOG_SIB19_MSG;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Sig Msg rcvd on invalid Log msg type %d",rrclog_dl_lc);
      return;
  }

  if(tdssib_events_ptr->event[index].for_serving_cell == TRUE)
  {
    rb_id = BCCH_S_RADIO_BEARER_ID;
  }
  else
  {
    rb_id = BCCH_N_RADIO_BEARER_ID;
  }

  dl_sdu_len = TDSGET_PDU_LENGTH_IN_BYTES( 
    tdssib_events_ptr->event[index].tsib[sib_type].sib_tot_length );

#ifdef T_WINNT
  #error code not present
#else  /* QXDM logging */

  /* Construct and send an event. */
  sig_msg_event.channel_type = TDSRRCLOG_EXTENSION_SIB;
  sig_msg_event.message_type = msg_type;
  event_report_payload(EVENT_TDSCDMA_RRC_MESSAGE_RECEIVED, sizeof(sig_msg_event),
                       (void *)&sig_msg_event);

  /*Calculate the record length */
  log_record_length = 
    FPOS(tdscdma_rrc_signalling_message_log_packet_type, signalling_message) + dl_sdu_len + 1;

  /*Allocate memory for Log Record. */
  log_record_ptr = (tdscdma_rrc_signalling_message_log_packet_type *)log_alloc( TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET, log_record_length);
  
  /* Make sure memory allocation is successful before to proceed with 
     logging */
  if(log_record_ptr != NULL)
  {
    /* Update RB Id.*/
    log_record_ptr->rb_id = rb_id;

    /*Update Logical Channel Type */
    log_record_ptr->channel_type = TDSRRCLOG_EXTENSION_SIB;

    /* Update length of the signalling message */
    log_record_ptr->signalling_msg_length = dl_sdu_len;

    log_record_ptr->signalling_message[0] = msg_type;

    /* Copy the Signalling mesaage payload to the log record */
    msg_size = memscpy((void *)&log_record_ptr->signalling_message[1], 
                       log_record_ptr->signalling_msg_length,
           (void *)(tdssib_events_ptr->event[index].tsib[sib_type].sib_data_ptr),
           log_record_ptr->signalling_msg_length);

    log_record_ptr->signalling_msg_length += 1;

    TDSRRC_MSG1(MSG_LEGACY_MED,"Logging TDSCDMA_RRC_SIGNALLING_MESSAGE_LOG_PACKET %d bytes",msg_size);
    /*commit the log record */
    log_commit(log_record_ptr);    
  }
  else
  {
    /* The QXDM is not turn ON. Hence Do not log the OTA messages */
  }
#endif /*T_WINNT */
} /* end function tdsrrcdispatch_log_dl_sig_message */

#endif




/*===========================================================================

FUNCTION    RRC_DEQUEUE_DL_SDU 

DESCRIPTION
  This function gets the Logical Channel ID from TDSRRC_DOWNLINK_SDU_IND command
  and identifies the corresponding watermark queue and dequeues the SDU from
  the watermark queue. The dequeued DSM Item is then converted into a 
  contingous stream of octects.
  
DEPENDENCIES
  None

RETURN VALUE
  tdsrrc_sdu_dequeue_status_type

SIDE EFFECTS
  None

===========================================================================*/
static tdsrrc_sdu_dequeue_status_type tdsrrcdispatch_dequeue_dl_sdu(
  tdsrrc_cmd_type *cmd_ptr                 /* To hold received command */
)
{  
  rlc_lc_id_type lc_id;                     /* Downlink logical Channel Id */
  dsm_watermark_type *dl_wm_ptr;            /* Downlink Water Mark pointer */
  dsm_item_type *dsm_item;                  /* DSM Item */
  word dl_sdu_length;                       /* Length of the Downlink SDU
                                               in bytes */

  tdsrrc_sdu_dequeue_status_type dequeue_status = TDSRRC_DEQUEUE_FAILURE;
  /* This is used to hold the return value of the function. If we're
  able to sucessfully dequeue the DSM item from RLC, this value is
  changed to TDSRRC_DEQUEUE_SUCCESS. Otherwise TDSRRC_DEQUEUE_FAILURE is
  returned. */

  /* Checks whether received command is TDSRRC_DOWNLINK_SDU_IND */
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_SDU_IND)
  {
    lc_id = cmd_ptr->cmd.downlink_sdu_ind.lc_id;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"RLC ID received through TDSRRC_DOWNLINK_SDU_IND command %d",lc_id);
    dl_wm_ptr = tdsrrclcm_get_dl_watermark_ptr(lc_id);
                                           /* Gets the Watermark pointer 
                                              corresponding to Logical Channel
                                              Id. */  
    dsm_item = dsm_dequeue(dl_wm_ptr);     /* Dequeues the DSM item from the
                                              water mark queue. */
    if (dsm_item != NULL)
    { 
      dl_sdu_length = (word)dsm_length_packet(dsm_item);
      
      /* Checks whether length of SDU is  valid. */
      if ( (dl_sdu_length > TDSMAX_SIZE_OF_SDU) ||
           (dl_sdu_length == 0) ) 
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Rcvd DSM Item has invalid SDU len: %d",dl_sdu_length);
      }
      else
      {

        /* Allocates the memory for the received SDU and this memory should be
           freed by the tdsrrc_free_cmd_buf function */
        cmd_ptr->cmd.downlink_sdu_ind.dl_sdu = 
                 (uint8 *) tdsrrc_malloc(dl_sdu_length);

        if (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to allocate memory for DL SDU");        
        }
        else
        { 
#ifdef FEATURE_TDSCDMA_SDU_LIFETIME_VALIDATION
          if(tdsrrclcm_get_dl_rb_id(lc_id) == DCCH_AM_RADIO_BEARER_ID)
          {
            time_type up_ms;
            uint32 lo32;
            time_get_uptime_ms(up_ms);
            lo32 = qw_lo(up_ms);
            if(dsm_item->app_field)
            {
              tdsrrc_sdu_recv_time = dsm_item->app_field;
              tdsrrc_sdu_recv_delay = lo32 - dsm_item->app_field;
              if(tdsrrc_sdu_recv_delay > 20000)
              {
                tdsrrc_sdu_recv_delay = 20000;
                TDSRRC_MSG1(MSG_LEGACY_MED,"Limit tdsrrc_sdu_recv_delay to %dms,as App Field may have faulty init",tdsrrc_sdu_recv_delay);
              }
            }
            else
            {
              /*something wrong in DSM*/
              TDSRRC_MSG0(MSG_LEGACY_MED,"Timing is not set in App Field,set default SDU lifetime/delay to 100ms");
              tdsrrc_sdu_recv_time = lo32;
              tdsrrc_sdu_recv_delay = 100;
            }
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"SDU receiving delay: %dms, current time %dms, first PDU time in DSM %dms",
                        tdsrrc_sdu_recv_delay, lo32, dsm_item->app_field);
          }
#endif
          /* Converts the DSM Item into  contiguous stream of bytes */
          dl_sdu_length = dsm_pkt_to_sdu_chain_byte (&dsm_item, 
                              cmd_ptr->cmd.downlink_sdu_ind.dl_sdu);   
    
          TDSRRC_MSG3(MSG_LEGACY_LOW,"Received SDU from DSM Queue. %LX %s %d", dl_wm_ptr,
                   cmd_ptr->cmd.downlink_sdu_ind.dl_sdu, dl_sdu_length);

          /* Copy the length into the command itself for future reference */
          cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_length = dl_sdu_length;

          /* Log the Downlink signalling message */
          tdsrrcdispatch_log_dl_sig_message(cmd_ptr);

          dequeue_status = TDSRRC_DEQUEUE_SUCCESS;
        }
      } /* dl_sdu_length is valid */     
    }    
  }
  return(dequeue_status);
} /* end function tdsrrcdispatch_dequeue_dl_sdu */


/*===========================================================================

FUNCTION  RRC_CHECK_MM_CMDS_FOR_SIMID_VALIDATION

DESCRIPTION
 
  This function validates SIM id for MM commands.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  SUCCESS: Validation is successful.
  else
  FAILURE
  
SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcdispatch_check_mm_cmds_for_simid_validation(tdsrrc_cmd_type *cmd_ptr)
{
  sys_modem_as_id_e_type sim_id_recvd;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id,curr_sub_id;
#endif

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DS_STATUS_CHANGE_IND)
      ||(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DS_TUNEAWAY_STATUS_CHANGE_IND)
    )
  {
    /*If cmd id is DS_STATUS_CHANGE_IND/RRC_DS_TUNEAWAY_STATUS_CHANGE_IND, pass the AS ID validation*/
    return SUCCESS;
  }
#else
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SIM_INSERTED_REQ) ||
     (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DS_STATUS_CHANGE_IND)
     || (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DS_TUNEAWAY_STATUS_CHANGE_IND)
#ifdef FEATURE_SGLTE
     || (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_UE_MODE_IND)
#endif
#ifdef FEATURE_CMCC_SECURE_CALL
     || (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SECURE_CALL_IND)
#endif
    )
  {
    /*If cmd id is DS_STATUS_CHANGE_IND/RRC_DS_TUNEAWAY_STATUS_CHANGE_IND, pass the AS ID validation*/
    return SUCCESS;
  }
#endif

#ifndef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SERVICE_REQ) &&
      (cmd_ptr->cmd.rrc_service_req.network_select_mode == SYS_NETWORK_SELECTION_MODE_LIMITED))
  {
    tdsrrc_is_sim_id_valid = TRUE;
    tdsrrc_sim_id_recvd = cmd_ptr->cmd.rrc_service_req.as_id;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: TDSRRC_SERVICE_REQ recvd with Limited N/W sel mode,use the AS ID: %d from the service request",tdsrrc_sim_id_recvd);
  }
#endif

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  if (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SUBSCRIPTION_ID_IND)
  {
    sub_id = cmd_ptr->cmd.rrc_subscription_id.as_id;
    
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"DSIM: TDSRRC_SUBSCRIPTION_ID_IND is received,use the AS ID: %d from the cmd, tdsrrc_sim_id_recvd = %d, tdsrrc_is_sim_id_valid = %d",
      sub_id,tdsrrc_sim_id_recvd,tdsrrc_is_sim_id_valid);
	
    if((sub_id >= SYS_MODEM_AS_ID_1) && (sub_id < (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
    {
      if((tdsrrc_is_sim_id_valid == FALSE)||(sub_id != tdsrrc_sim_id_recvd))
      {
        /*If no valid sub or sub id changed,clear up dedicated priority owner to NONE if current owner is TDSCDMA */
        tdsrrcmeas_handle_clear_dedicated_pri_ind();

        if ((tdsrrc_is_sim_id_valid == TRUE) &&
            (sub_id != tdsrrc_sim_id_recvd))
        {
          curr_sub_id = tdsrrc_get_as_id();
          tdsrrcsmc_set_smc_integrity_protection_to_related_sub_parms(curr_sub_id);
          tdsrrcsmc_set_smc_security_info_to_related_sub_params(curr_sub_id);
          tdsrrc_set_identity_info_to_related_sub_params(curr_sub_id);
        }
        tdsrrc_is_sim_id_valid = TRUE;
        tdsrrc_sim_id_recvd  = sub_id;
        tdsrrcsmc_set_smc_integrity_protection_to_global_params(sub_id);
        tdsrrcsmc_set_smc_security_info_to_global_params(sub_id);
        tdsrrc_set_identity_info_to_global_params(sub_id);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: tdsrrc_is_sim_id_valid is true but receive same sim id in TDSRRC_SUBSCRIPTION_ID_IND, ignore it!");
      }
      return SUCCESS;
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", tdsrrc_sim_id_recvd);
      return FAILURE;
    }
  }

  if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_FORBIDDEN_LAI_LIST_UPDATE_REQ) ||
     (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_INVALIDATE_SIM_DATA_REQ) ||
     (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SIM_INSERTED_REQ) ||
     (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SIM_UPDATE_REQ) ||
     (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MULTIMODE_SUBS_CHGD_IND) || 
     (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_PREF_CHANGE_IND)
#ifdef FEATURE_SGLTE
     ||(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_UE_MODE_IND)
#endif
  )
  {
    /*If cmd id is TDSRRC_FORBIDDEN_LAI_LIST_UPDATE_REQ/TDSRRC_INVALIDATE_SIM_DATA_REQ/TDSRRC_UE_MODE_IND, pass the AS ID validation*/
    return SUCCESS;
  }
#else
/* TDSRRC_MODE_PREF_CHANGE_IND is retired after FEATURE_DUAL_WCDMA is introduced */
#ifndef FEATURE_DUAL_WCDMA
  /* When SIM is not present and mode pref gets changed resulting in AS ID change it needs to be updated by RRC */
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_PREF_CHANGE_IND)
  {
    tdsrrc_is_sim_id_valid = TRUE;
    tdsrrc_sim_id_recvd = cmd_ptr->cmd.rrc_subscription_id.as_id;
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: TDSRRC_MODE_PREF_CHANGE_IND is received,use the AS ID: %d from the cmd",tdsrrc_sim_id_recvd);
    return SUCCESS;
  }
#endif
  
  if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MULTIMODE_SUBS_CHGD_IND)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSRRC_MULTIMODE_SUBS_CHGD_IND Recvd at rrc mode %d",tdsrrcmcm_get_rrc_mode());
    tdsrrc_is_sim_id_valid = TRUE;
#ifdef FEATURE_DUAL_WCDMA
    if((cmd_ptr->cmd.rrc_multimode_subs_chgd_ind.subs_capability_mask[SYS_MODEM_AS_ID_1] & SYS_SYS_MODE_MASK_TDS) != 0)
    {
      tdsrrc_sim_id_recvd = SYS_MODEM_AS_ID_1;
    }
    else if((cmd_ptr->cmd.rrc_multimode_subs_chgd_ind.subs_capability_mask[SYS_MODEM_AS_ID_2] & SYS_SYS_MODE_MASK_TDS) != 0)
    {
      tdsrrc_sim_id_recvd = SYS_MODEM_AS_ID_2;
    }
#else
    if(cmd_ptr->cmd.rrc_multimode_subs_chgd_ind.subs_capability[SYS_MODEM_AS_ID_1] == SUBS_CAPABILITY_MULTIMODE)
    {
      tdsrrc_sim_id_recvd = SYS_MODEM_AS_ID_1;
    }
    else if(cmd_ptr->cmd.rrc_multimode_subs_chgd_ind.subs_capability[SYS_MODEM_AS_ID_2] == SUBS_CAPABILITY_MULTIMODE)
    {
      tdsrrc_sim_id_recvd = SYS_MODEM_AS_ID_2;
    }
#endif /* FEATURE_DUAL_WCDMA */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDS recieve the third ,which is not support by TDS now"); 
    }
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"AS-ID Changed to %d",tdsrrc_sim_id_recvd);
    return SUCCESS;
  }
#endif

  if(!tdsrrc_is_sim_id_valid)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: RRC not yet updated with SIM details,Treat as SIM Match");
    return SUCCESS;
  }
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_SERVICE_REQ:
      sim_id_recvd = cmd_ptr->cmd.rrc_service_req.as_id;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: as_id %d in service_req",sim_id_recvd);
      break;

    case RRC_STOP_TDS_MODE_REQ:
      sim_id_recvd = cmd_ptr->cmd.stop_wcdma_mode_req.as_id ;
      break;

    case TDSRRC_DEACT_REQ:
    case TDSRRC_MODE_CHANGE_REQ:
      sim_id_recvd = cmd_ptr->cmd.deact_req.as_id;
      break;

    case TDSRRC_CHANGE_MODE_IND:
      sim_id_recvd = cmd_ptr->cmd.rrc_change_mode_ind.as_id;
      break;

#ifndef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    case TDSRRC_FORBIDDEN_LAI_LIST_UPDATE_REQ:
      sim_id_recvd = cmd_ptr->cmd.forbid_lai_list_update_req.as_id;
      break;
#endif

    case TDSRRC_PLMN_LIST_REQ:
      sim_id_recvd = cmd_ptr->cmd.plmn_list_req.as_id;
      break;

    case TDSRRC_EST_REQ:
      sim_id_recvd = cmd_ptr->cmd.rrc_est_req.as_id;
      break;
    
    case TDSRRC_ABORT_REQ:
      sim_id_recvd = cmd_ptr->cmd.rrc_abort_req.as_id ;
      break;

    case TDSRRC_OPEN_SESSION_REQ:
      sim_id_recvd = cmd_ptr->cmd.open_sess_req.as_id;
      break;
    
    case TDSRRC_DATA_REQ:
      sim_id_recvd = cmd_ptr->cmd.data_req.as_id;
      break;
    
    case TDSRRC_CLOSE_SESSION_REQ:
      sim_id_recvd = cmd_ptr->cmd.close_sess_req.as_id ;
      break;

    case TDSRRC_ACTIVATION_RSP:
      sim_id_recvd = cmd_ptr->cmd.activation_rsp.as_id;
      break;

    case TDSRRC_NW_SEL_MODE_RESET_IND:
      sim_id_recvd = cmd_ptr->cmd.rrc_nw_sel_mode_reset_ind.as_id;
      break;
      
#ifndef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    case TDSRRC_INVALIDATE_SIM_DATA_REQ:
      tdsrrc_is_sim_id_valid = FALSE;
      return SUCCESS;

    case TDSRRC_SIM_UPDATE_REQ:
      sim_id_recvd = cmd_ptr->cmd.rrc_sim_update_req.as_id;
      break;
#endif

    case TDSRRC_FAST_DORMANCY_REQ:
      sim_id_recvd = cmd_ptr->cmd.rrc_fast_dormancy_req.as_id;
      break;
      
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
    case TDSRRC_LAI_REJECT_LIST_IND:
      sim_id_recvd = cmd_ptr->cmd.rrc_lai_reject_list_ind.as_id;
      break;
#endif

#ifdef FEATURE_3GPP_CSFB_TDS
    case TDSRRC_CSFB_CALL_STATUS_IND:
      sim_id_recvd = cmd_ptr->cmd.rrc_csfb_call_status_ind.as_id;
      break;
#endif

#ifdef FEATURE_LTE_TO_TDSCDMA
    case TDSRRC_LTE_RESELECTION_ALLOWED_STATUS_IND:
      sim_id_recvd = cmd_ptr->cmd.rrc_lte_resel_allowed_status_ind.as_id;
      break;
#endif

#ifndef FEATURE_DUAL_WCDMA
    case TDSRRC_MODE_PREF_CHANGE_IND:
      /* When SIM is not present and mode pref gets changed resulting in AS ID change it needs to be updated by RRC */
      tdsrrc_is_sim_id_valid = TRUE;
      tdsrrc_sim_id_recvd = cmd_ptr->cmd.rrc_subscription_id.as_id;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: TDSRRC_MODE_PREF_CHANGE_IND is received,use the AS ID: %d from the cmd",tdsrrc_sim_id_recvd);
      return SUCCESS;
#endif

    case TDSRRC_PROCEDURE_IND:
      sim_id_recvd = cmd_ptr->cmd.rrc_procedure_ind.as_id;
      break;
	case TDSRRC_BARRING_INFO_IND:
		sim_id_recvd = cmd_ptr->cmd.tdsrrc_barring_info_ind.as_id;
	  break;  

#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
#ifdef FEATURE_CMCC_SECURE_CALL
    case TDSRRC_SECURE_CALL_IND:
      sim_id_recvd = cmd_ptr->cmd.rrc_secure_call_ind.as_id;
      break;
#endif
#endif

    default :
#ifdef FEATURE_DUAL_SIM
      sim_id_recvd = cmd_ptr->cmd.rrc_subscription_id.as_id;
#else 
      sim_id_recvd = SYS_MODEM_AS_ID_1;
#endif
      break;
  }/* end switch */

  if(tdsrrc_sim_id_recvd == sim_id_recvd)
  {
    return SUCCESS;
  }
  TDSRRC_MSG2(MSG_LEGACY_ERROR,"AS-id mismatch %d,internal %d", sim_id_recvd, tdsrrc_sim_id_recvd);
  return FAILURE;
}


/*===========================================================================

FUNCTION  TDSRRC_CHECK_RR_CMDS_FOR_SIMID_VALIDATION

DESCRIPTION
 
  This function validates SIM id for RR commands.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None
  
SIDE EFFECTS

  None.

===========================================================================*/
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
uecomdef_status_e_type tdsrrc_check_rr_cmds_for_simid_validation(tdsrrc_rr_cmd_type *cmd_ptr)
{
  sys_modem_as_id_e_type sim_id_recvd;

  sim_id_recvd = cmd_ptr->cmd_hdr.as_id;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: sub-id in rr cmd: %d",(int)sim_id_recvd);

  if(!tdsrrc_is_sim_id_valid)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: RRC not yet updated with SIM details or STOP_TDS_REQ from other SUB,Treat as SIM Match");
    return SUCCESS;
  }
  if(tdsrrc_sim_id_recvd != sim_id_recvd)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid sub-id %d passed. internal %d",(int)sim_id_recvd,(int)tdsrrc_sim_id_recvd);
    return FAILURE;
  }
  return SUCCESS;
}
#else
void tdsrrc_check_rr_cmds_for_simid_validation(tdsrrc_rr_cmd_type *cmd_ptr)
{
  sys_modem_as_id_e_type sim_id_recvd;

  sim_id_recvd = cmd_ptr->cmd_hdr.as_id;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: sub-id in rr cmd: %d",(int)sim_id_recvd);

  if(!tdsrrc_is_sim_id_valid)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: RRC not yet updated with SIM details,Treat as SIM Match");
    return;
  }
  if(tdsrrc_sim_id_recvd != sim_id_recvd)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid sub-id %d passed. internal %d",(int)sim_id_recvd,(int)tdsrrc_sim_id_recvd);
    return;
  }
  return;
}
#endif
/*===========================================================================

FUNCTION  tdsrrc_check_msgr_for_simid_validation

DESCRIPTION
 
  This function validates SIM id for MSGR commands.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  SUCCESS: Validation is successful.
  else
  FAILURE
  
SIDE EFFECTS

  None.

===========================================================================*/

uecomdef_status_e_type tdsrrc_check_msgr_for_simid_validation(sys_modem_as_id_e_type as_id)
{
  if (as_id != tdsrrc_get_as_id())
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid sub-id %d passed. internal %d",(int)as_id,(int)tdsrrc_get_as_id());
    return FAILURE;
  }

  return SUCCESS;
}


#ifdef FEATURE_DUAL_WCDMA

/*===========================================================================

FUNCTION  RRC_CHECK_TC_CMDS_FOR_SIMID_VALIDATION

DESCRIPTION
 
  This function validates SIM id for TC commands.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  SUCCESS: Validation is successful.
  else
  FAILURE
  
SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcdispatch_check_tc_cmds_for_simid_validation(tdsrrc_cmd_type *cmd_ptr)
{
  sys_modem_as_id_e_type sim_id_recvd = SYS_MODEM_AS_ID_NONE;

  if(!tdsrrc_is_sim_id_valid)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: RRC not yet updated with SIM details,Treat as SIM Match");
    return SUCCESS;
  }

#ifndef FEATURE_TC
  TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: FEATURE_TC is disabled for cmd_id:%x",cmd_ptr->cmd_hdr.cmd_id);
  return SUCCESS;
#endif
  
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_TC_MODE_CHANGE:
      sim_id_recvd = cmd_ptr->cmd.mode_change.as_id;
      break;

    case TDSRRC_TC_TEST_LOOPBACK_IND:
      sim_id_recvd = cmd_ptr->cmd.test_loopback_ind.as_id ;
      break;

    default :
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"DSIM: unknown cmd_id:%x",cmd_ptr->cmd_hdr.cmd_id);
      return SUCCESS;
      break;
  }/* end switch */

  if(tdsrrc_sim_id_recvd == sim_id_recvd)
  {
    return SUCCESS;
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"DSIM: received as_id:%d doesn't match with as_id:%d",sim_id_recvd, tdsrrc_sim_id_recvd);
    return FAILURE;
  }
}
#endif

/*===========================================================================

FUNCTION  tdsrrc_check_and_get_cmd

DESCRIPTION
 
  first check int_cmd_q, then get appropriate cmd if needed 
   
DEPENDENCIES

  None.
   
RETURN VALUE

  cmd_ptr
  
SIDE EFFECTS

  None.

===========================================================================*/
tdsrrc_cmd_type * tdsrrc_check_and_get_cmd( q_type  *q_ptr )
{
  tdsrrc_cmd_type    *cmd_ptr;
  tdsrrc_cmd_type    *next_cmd_ptr;
  boolean            hanlde_and_delete_q = FALSE;

  if (tdsrrccsp_get_curr_scan() == TDSRRC_CSP_UNLOCK_BY_SCAN)
  {
    cmd_ptr = (tdsrrc_cmd_type *)q_check (q_ptr);
	
    while(cmd_ptr != NULL)
    {
      next_cmd_ptr = (tdsrrc_cmd_type *)q_next(q_ptr,&cmd_ptr->cmd_hdr.link);
      if ((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DOWNLINK_SDU_IND) || (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_CRLC_STATUS_IND))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"skip CMD: 0x%x", cmd_ptr->cmd_hdr.cmd_id);
      }
      else
      {
        hanlde_and_delete_q = TRUE;
		//TDSRRC_MSG1(MSG_LEGACY_HIGH,"handle CMD: 0x%x", cmd_ptr->cmd_hdr.cmd_id);
        break;
      }
      cmd_ptr = next_cmd_ptr;
    }

    /* delete q from int_cmd_q */
    if( hanlde_and_delete_q == TRUE)
    {
#ifdef FEATURE_Q_NO_SELF_QPTR
      q_delete(q_ptr, &cmd_ptr->cmd_hdr.link );
#else
      q_delete( &cmd_ptr->cmd_hdr.link );
#endif
    }
  } 
  else
  {
    cmd_ptr = q_get(q_ptr);
  }
  
  return cmd_ptr;
}

/*===========================================================================

FUNCTION  RRC_DOG_REPORT_AND_START_TIMER

DESCRIPTION
 
  This function reports dog and restarts the dog timer.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_dog_report_and_start_timer(void)
{
#if 0 //using DOG HB mechanism
  dog_report(TDSDOG_RRC_RPT);
  (void) rex_set_timer(&tdsrrc_rpt_timer, TDSDOG_RRC_RPT_TIME);
#else
  /* Report to the Dog HB */
  dog_hb_report(TDSDOG_RRC_RPT);
#endif
}

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK

/*Feature will be mainlined once we test it*/
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
static void tdsrrcdispatch_check_ff_commands(void)
{

    if((rex_get_sigs(rex_self())) & TDSRRC_L1_FF_CMD_Q_SIG)
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_L1_FF_CMD_Q_SIG );  
      /*A brute force way is to call this function w/o check the FF sigs at all*/
      tdsrrc_dispatch_l1_ff_commands();
    }
}
#endif

/*===========================================================================

FUNCTION  RRC_DISPATCH_L1_FF_COMMANDS_DURING_WAIT_LOCK

DESCRIPTION
 
  This function dispatches all commands received in RRC's free floating
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrc_dispatch_l1_ff_commands_during_wait_lock( void )
{
  tdsrrc_cmd_type *cmd_ptr;      /* To hold received command */

  while((cmd_ptr = q_get(&tdsrrc_l1_ff_cmd_q)) != NULL)
  {
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
      case TDSRRC_CPHY_CHECK_SLEEP_REQ:
        tdsrrcff_fail_l1_sleep_req(cmd_ptr);
        break;
      default:
        ERR_FATAL("FF: Unexpected Message %d",cmd_ptr->cmd_hdr.cmd_id,0,0);
        break;
    }
    tdsrrc_free_cmd_buf( cmd_ptr );
    cmd_ptr = NULL;
  }
}


/*===========================================================================

FUNCTION  RRC_DISPATCH_L1_FF_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's free floating
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrc_dispatch_l1_ff_commands( void )
{
  tdsrrc_cmd_type *cmd_ptr;      /* To hold received command */

  while((cmd_ptr = q_get(&tdsrrc_l1_ff_cmd_q)) != NULL)
  {
    switch(cmd_ptr->cmd_hdr.cmd_id)
    {
      case TDSRRC_CPHY_CHECK_SLEEP_REQ:
        tdsrrcff_check_l1_sleep_req(cmd_ptr);
        break;
      case TDSRRC_REG_SRVING_CELL_CHG_IND:
        (void)tdsrrc_register_serving_cell_change_ind_cb_int(cmd_ptr->cmd.reg_cb_ptr);
        break;
      case TDSRRC_DEREG_SRVING_CELL_CHG_IND:
        (void)tdsrrc_deregister_serving_cell_change_ind_cb_int(cmd_ptr->cmd.dereg_cb_ptr);
        break;
      default:
        ERR_FATAL("FF: Unexpected Message %d",cmd_ptr->cmd_hdr.cmd_id,0,0);
        break;
    }
    tdsrrc_free_cmd_buf( cmd_ptr );
  }
}
#endif
/*===========================================================================

FUNCTION  RRC_DISPATCH_INTERNAL_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's internal
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_internal_commands( void )
{
  
  tdsrrc_cmd_type *cmd_ptr;      /* To hold received command */
  tdsrrc_proc_e_type rrc_proc_name; /* To hold procedure name */
  uint32 count;               /* Local Counter             */
  /* We need to dequeue items from the internal
  command queue and process them till the queue is
  empty */
  tdsrrclog_da_backoff_timer_expired_event_type da_backoff_timer_expired_event;
  TDSRRC_MSG0(MSG_LEGACY_LOW,"Dispatching RRC Internal Commands");
  while( (cmd_ptr = tdsrrc_check_and_get_cmd(&tdsrrc_int_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
      #error code not present
#endif

    /* Check if the incoming internal command is required a further process */
    if (tdsrrcmcm_is_process_required(cmd_ptr) == FALSE) 
    {
      /* Display a XDM message and disgard the command */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMD: %d is disgarded!",cmd_ptr->cmd_hdr.cmd_id);
    }
#ifdef FEATURE_MODEM_CONFIG_REFRESH
    else if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MCFG_REFRESH_IND)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"rcvd TDSRRC_MCFG_REFRESH_IND");
      tdsrrc_process_mcfg_refresh_ind(cmd_ptr);
    }
#endif
    else if(tdsrrc_deact_status != TDSRRC_DEACT_COMPLETED)
    {
      /* Switch on the command id of the received command, and
       * call the appropriate function to process each command. 
       */
      switch( cmd_ptr->cmd_hdr.cmd_id )
      {
        case TDSRRC_DOWNLINK_SDU_IND:
          /* Dequeues the SDU from the corrosponding watermark
          queue. This function dynamically allocates memory for
          DL SDU, the dispatcher should free this memory before freeing
          the command. */
          if (TDSRRC_DEQUEUE_SUCCESS == tdsrrcdispatch_dequeue_dl_sdu (cmd_ptr))
          {
            if (tdsrrcllc_return_current_substate() == TDSLLC_WAIT_CPHY_IDLE_CNF)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"LLC wt idle cnf,discard any DL SDU");
            }
            /* This function dynamically allocates memory for ASN1 decoded 
            message, the dispatcher should use a special function provided
            by the OSS compiler to free the memory before it frees the the 
            command */        
	    else if(FAILURE == tdsrrcdispatch_perform_integrity_protection_check(cmd_ptr,
                            &rrc_proc_name) )
            {
              TDSRRC_MSG0(MSG_LEGACY_MED,"Integrity Check failed or Message not processed");
            }
            else
            {
              /* Allow only NAS,BCCH messages, CUCnf and RCR if RCR is active */
              if ((rrc_proc_name != TDSRRC_PROCEDURE_DDT) && 
                  (rrc_proc_name != TDSRRC_PROCEDURE_SIB) &&
                  (rrc_proc_name != TDSRRC_PROCEDURE_CU) &&
                  (rrc_proc_name != TDSRRC_PROCEDURE_RCR)
                  )
              {
                if (tdsrrcrcr_is_proc_active() == TRUE)
                {
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Ignore DLMsg for %d",rrc_proc_name);
                  break;
                }
              }
              /* Then call the procedure that needs to process this
              command */
              switch (rrc_proc_name)
              {
                case TDSRRC_PROCEDURE_SIB:
                  tdsrrcsib_event_controller( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_CSP:
                  tdsrrccsp_cell_selection_procedure( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_RCE:
                  tdsrrcrce_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_RCR:
                  tdsrrcrcr_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_DDT:
                  tdsrrcddt_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_SCR:
                  tdsrrcscr_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_RBE:
                  tdsrrcrbe_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_RBRC:
                  tdsrrcrbrc_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_TCR:
                  tdsrrctcrc_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_TFC:
                  tdsrrctfcc_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_PCR:
                  tdsrrcpcrc_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_RBR:
                  tdsrrcrbr_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_PG1:
                  tdsrrcpg1_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_UECE:
                  tdsrrcuece_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_UECI:
                  tdsrrcueci_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_SMC:
                  tdsrrcsmc_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_MCMR:
                  tdsrrcmeas_procedure_event_handler( cmd_ptr );
                  break;               

                case TDSRRC_PROCEDURE_PG2:
                  tdsrrcpg2_procedure_event_handler( cmd_ptr );
                  break;               

                case TDSRRC_PROCEDURE_UMI:
                  tdsrrcumi_procedure_event_handler( cmd_ptr );
                  break; 
  
                case TDSRRC_PROCEDURE_CU:
                  tdsrrccu_procedure_event_handler( cmd_ptr );
                  break;            

                case TDSRRC_PROCEDURE_IHO:
                  tdsrrciho_event_handler( cmd_ptr );
                  break;
                case TDSRRC_PROCEDURE_CHO:
                  tdsrrccho_procedure_event_handler( cmd_ptr );
                  break;

                case TDSRRC_PROCEDURE_CHK:
                  tdsrrcchk_procedure_event_handler( cmd_ptr );
                  break;  

                case TDSRRC_PROCEDURE_UPCC:
                  tdsrrcupcc_procedure_event_handler( cmd_ptr );
                  break;


                case TDSRRC_PROCEDURE_NONE:
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : Dispatching TDSRRC_DOWNLINK_SDU_IND Failed to identify the Msg type ");
                  tdsrrcmisc_procedure_event_handler( cmd_ptr );
                  break;

                default:
                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : This proc is not implemented: %d",rrc_proc_name);
                  break;

              }/*switch stmt*/
            }/*else stmt for integrity check*/
          }/*if for dequeue msg pdu*/
          else
          {
            /* Unable to dequeue the message PDU from the DSM queue.
              Indicate error. */
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to dequeue msg PDU from queue");
          }
          break;       

        case TDSRRC_GET_ALL_SIBS_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCCM->RRCSIB: Dispatching TDSRRC_GET_ALL_SIBS_REQ to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;

        case TDSRRC_GET_SPECIFIC_SIBS_REQ:
          switch(cmd_ptr->cmd.get_specific_sibs_req.procedure)
          {
            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCSIB: Dispatching TDSRRC_GET_SPECIFIC_SIBS_REQ to SIB");
              break;
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
            case TDSRRC_PROCEDURE_GPS:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCGPS->RRCSIB: Dispatching TDSRRC_GET_SPECIFIC_SIBS_REQ to SIB");
              break;
#endif
            case TDSRRC_PROCEDURE_SIB:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCSIB: Dispatching TDSRRC_GET_SPECIFIC_SIBS_REQ to SIB");
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRC Get Specific Sibs Req rcvd for unknown proc: %d",cmd_ptr->cmd.get_specific_sibs_req.procedure);
              break;
          }
          tdsrrcsib_event_controller( cmd_ptr );
          break;

        case TDSRRC_UPDATE_SIBS_IND:
          switch(cmd_ptr->cmd.update_sibs_ind.procedure)
          {
            case TDSRRC_PROCEDURE_PG1:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPG1->RRCSIB: Dispatching TDSRRC_UPDATE_SIBS_IND to SIB");
              break;
            case TDSRRC_PROCEDURE_SIB:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCSIB: Dispatching TDSRRC_UPDATE_SIBS_IND to SIB");
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRC Update Sibs Ind rcvd for unknown proc: %d",cmd_ptr->cmd.update_sibs_ind.procedure);
              break;
          }
          tdsrrcsib_event_controller( cmd_ptr );
          break;

        case TDSRRC_GET_ALL_SIBS_CNF:
          switch ( cmd_ptr->cmd.get_sibs_cnf.procedure )
          {
            case TDSRRC_PROCEDURE_CCM:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCCM: Dispatching TDSRRC_GET_ALL_SIBS_CNF to CCM");
              tdsrrcccm_cell_change_manager( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCSP: Dispatching TDSRRC_GET_ALL_SIBS_CNF to CSP");
              tdsrrccsp_cell_selection_procedure( cmd_ptr );
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Unknown Proc 0x%x for GET_ALL_SIBS_CNF",(cmd_ptr->cmd.get_sibs_cnf.procedure));
              break;
          }
          break;

        case TDSRRC_GET_SPECIFIC_SIBS_CNF:
          switch ( cmd_ptr->cmd.get_sibs_cnf.procedure )
          {
            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCSP: Dispatching TDSRRC_GET_SPECIFIC_SIBS_CNF to CSP");
              tdsrrccsp_cell_selection_procedure( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CCM:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCCM: Dispatching TDSRRC_GET_SPECIFIC_SIBS_CNF to CCM");
              tdsrrcccm_cell_change_manager( cmd_ptr );
              break;

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
           case TDSRRC_PROCEDURE_GPS:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCGPS: Dispatching TDSRRC_GET_SPECIFIC_SIBS_CNF to GPS");
              tdsrrcgps_procedure_event_handler( cmd_ptr );
              break;
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/


            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Unknown Proc 0x%x for GET_SPECIFIC_SIBS_CNF",(cmd_ptr->cmd.get_sibs_cnf.procedure));
              break;
          }
          break;

        case TDSRRC_NEW_STATE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCSCMGR: Dispatching TDSRRC_NEW_STATE_IND to SCMGR");
          tdsrrcscmgr_event_handler( cmd_ptr );
          break;

        case TDSRRC_STATE_CHANGE_IND:
          if (cmd_ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED)
          {
             tdsrrccsp_permit_cs_resel_evaluate_by_nas = TRUE;
             tdsrrccsp_permit_ps_resel_evaluate_by_nas = TRUE;
             tdsrrctmr_stop_timer(TDSRRCTMR_BLOCK_CS_RESEL_NAS_API_TIMER);
			 tdsrrctmr_stop_timer(TDSRRCTMR_BLOCK_PS_RESEL_NAS_API_TIMER);
          }

          /* A state change indication command can go to
          multiple procedures. Loop the required number
          of times and send the command to each procedure. */
          for( count=0; 
               count < cmd_ptr->cmd.state_change_ind.num_procedures;
               count++
             )
          {
            switch ( cmd_ptr->cmd.state_change_ind.procedure[count] )
            {
              case TDSRRC_PROCEDURE_SIB:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCSIB: Dispatching TDSRRC_STATE_CHANGE_IND to SIB");
                tdsrrcsib_event_controller( cmd_ptr );
                break;
  
              case TDSRRC_PROCEDURE_RCE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCRCE: Dispatching TDSRRC_STATE_CHANGE_IND to RCE");
                tdsrrcrce_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_CSP:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCCSP: Dispatching TDSRRC_STATE_CHANGE_IND to CSP");
                tdsrrccsp_cell_selection_procedure( cmd_ptr );
                break;
   
              case TDSRRC_PROCEDURE_RCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCRCR: Dispatching TDSRRC_STATE_CHANGE_IND to RCR");
                tdsrrcrcr_procedure_event_handler( cmd_ptr );
                break;
    
              case TDSRRC_PROCEDURE_RBE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCRBE: Dispatching TDSRRC_STATE_CHANGE_IND to RBE");
                tdsrrcrbe_procedure_event_handler( cmd_ptr );
                break;
     
              case TDSRRC_PROCEDURE_RBR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCRBR: Dispatching TDSRRC_STATE_CHANGE_IND to RBR");
                tdsrrcrbr_procedure_event_handler( cmd_ptr );
                break;
  
              case TDSRRC_PROCEDURE_SCRR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCSCRR: Dispatching TDSRRC_STATE_CHANGE_IND to SCRR");
                tdsrrcscrr_procedure_event_handler( cmd_ptr );
                break;
  
              case TDSRRC_PROCEDURE_IDT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCDT: Dispatching TDSRRC_STATE_CHANGE_IND to IDT");
                tdsrrcidt_procedure_event_handler( cmd_ptr );
                break;
   
              case TDSRRC_PROCEDURE_UDT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCDT: Dispatching TDSRRC_STATE_CHANGE_IND to UDT");
                tdsrrcudt_procedure_event_handler( cmd_ptr );
                break;
    
              case TDSRRC_PROCEDURE_UECI:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCUECI: Dispatching TDSRRC_STATE_CHANGE_IND to UECI");
                tdsrrcueci_procedure_event_handler( cmd_ptr );
                break;
     
              case TDSRRC_PROCEDURE_MCMR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCMEAS: Dispatching TDSRRC_STATE_CHANGE_IND to MEAS");
                tdsrrcmeas_procedure_event_handler( cmd_ptr );
                break;
  
              case TDSRRC_PROCEDURE_LBT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCLBT: Dispatching TDSRRC_STATE_CHANGE_IND to LBT");
                tdsrrclbt_procedure_event_handler( cmd_ptr );
                break;
   
              case TDSRRC_PROCEDURE_CU:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCCU: Dispatching TDSRRC_STATE_CHANGE_IND to CU");
                tdsrrccu_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_SMC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCSMC: Dispatching TDSRRC_STATE_CHANGE_IND to SMC");
                tdsrrcsmc_procedure_event_handler( cmd_ptr );
                break;
  
              case TDSRRC_PROCEDURE_UMI:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCUMI: Dispatching TDSRRC_STATE_CHANGE_IND to UMI");
                tdsrrcumi_procedure_event_handler( cmd_ptr );
                break;
   
              case TDSRRC_PROCEDURE_RBRC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCRBRC: Dispatching TDSRRC_STATE_CHANGE_IND to RBRC");
                tdsrrcrbrc_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_TCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCTCR: Dispatching TDSRRC_STATE_CHANGE_IND to TCR");
                tdsrrctcrc_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_PCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCPCR: Dispatching TDSRRC_STATE_CHANGE_IND to PCR");
                tdsrrcpcrc_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_CCM:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCCCM: Dispatching TDSRRC_STATE_CHANGE_IND to CCM");
                tdsrrcccm_cell_change_manager( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_CHO:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCCHO: Dispatching TDSRRC_STATE_CHANGE_IND to CHO");
                tdsrrccho_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_IHO:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCIHO: Dispatching TDSRRC_STATE_CHANGE_IND to IHO");
                tdsrrciho_event_handler( cmd_ptr );
                break;
              case TDSRRC_PROCEDURE_DDT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCDT: Dispatching TDSRRC_STATE_CHANGE_IND to DDT");
                tdsrrcddt_procedure_event_handler( cmd_ptr );
                break;


              case TDSRRC_PROCEDURE_CHK:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCCHK: Dispatching TDSRRC_STATE_CHANGE_IND to CHK");
                tdsrrcchk_procedure_event_handler( cmd_ptr );
                break;  
              /* Other procedures should be checked here as needed. */
  
              case TDSRRC_PROCEDURE_MISC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCMISC: Dispatching TDSRRC_STATE_CHANGE_IND to MISC");
                tdsrrcmisc_procedure_event_handler( cmd_ptr );
                break;
                

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
              case TDSRRC_PROCEDURE_GPS:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCGPS: Dispatching TDSRRC_STATE_CHANGE_IND to GPS");
                tdsrrcgps_procedure_event_handler(cmd_ptr);
                break;
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/

#ifdef FEATURE_TDSCDMA_REL7_SPECRS 
              case TDSRRC_PROCEDURE_COUEC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCMISC: Dispatching TDSRRC_STATE_CHANGE_IND to MISC");
                tdsrrccouec_procedure_event_handler(cmd_ptr);
                break;
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/
              case TDSRRC_PROCEDURE_TFC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCTFCC: Dispatching TDSRRC_STATE_CHANGE_IND to TFCC");
                tdsrrctfcc_procedure_event_handler( cmd_ptr );
                break;  
                
              case TDSRRC_PROCEDURE_DORMANCY:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCMGR->RRCDORMANCY: Dispatching TDSRRC_STATE_CHANGE_IND to DORMANCY");
                tdsrrcdormancy_procedure_event_handler( cmd_ptr );
                break;  
               default:
                 TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Could not id proc: %d for State Chg Ind",( cmd_ptr->cmd.state_change_ind.procedure[count] ));
                 break;

            }
          }/* end for */
//vn_fix_9
          /* Now send a State Change Indication to RRCLLC */
          tdsrrcllc_process_cmd( cmd_ptr );
// end_vn_fix_9
          break;

        case TDSRRC_SIB_CHANGE_IND:
          /* A state change indication command contains the name
          of the procedure for which the command is intended.
          So switch on the procedure name and call the appropriate
          function */
          switch ( cmd_ptr->cmd.sib_change_ind.procedure )
          {
            case TDSRRC_PROCEDURE_SIB:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCSIB: Dispatching TDSRRC_SIB_CHANGE_IND to SIB");
                tdsrrcsib_event_controller( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_CSP:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCSP: Dispatching TDSRRC_SIB_CHANGE_IND to CSP");
                tdsrrccsp_cell_selection_procedure( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RCE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCRCE: Dispatching TDSRRC_SIB_CHANGE_IND to RCE");
                tdsrrcrce_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCRCR: Dispatching TDSRRC_SIB_CHANGE_IND to RCR");
                tdsrrcrcr_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RBE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCRBE: Dispatching TDSRRC_SIB_CHANGE_IND to RBE");
                tdsrrcrbe_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RBR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCRBR: Dispatching TDSRRC_SIB_CHANGE_IND to RBR");
                tdsrrcrbr_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_SCRR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCSCRR: Dispatching TDSRRC_SIB_CHANGE_IND to SCRR");
                tdsrrcscrr_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_IDT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCDT: Dispatching TDSRRC_SIB_CHANGE_IND to IDT");
                tdsrrcidt_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_UDT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCDT: Dispatching TDSRRC_SIB_CHANGE_IND to UDT");
                tdsrrcudt_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_UECI:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCUECI: Dispatching TDSRRC_SIB_CHANGE_IND to UECI");
                tdsrrcueci_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_MCMR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCMEAS: Dispatching TDSRRC_SIB_CHANGE_IND to MEAS");
                tdsrrcmeas_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCU: Dispatching TDSRRC_SIB_CHANGE_IND to CU");
              tdsrrccu_procedure_event_handler( cmd_ptr );
              break;


            /* Other procedures should be checked here as needed. */

            default:
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Could not id Proc: %d for SIB Chg Ind",( cmd_ptr->cmd.sib_change_ind.procedure ));
                break;

          }
          break;

        case TDSRRC_CELL_CHANGE_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCCCM: Dispatching TDSRRC_CELL_CHANGE_REQ to CCM");
          tdsrrcccm_cell_change_manager( cmd_ptr );
          break;

        case TDSRRC_CELL_CHANGE_CNF:
          /* Rout the Cell Change CNF to the procedure ID indicated in cmd data */
          switch(cmd_ptr->cmd.cell_change_cnf.req_proc_id)
          {
            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCCM->RRCCSP: Dispatching TDSRRC_CELL_CHANGE_CNF to CSP");
              tdsrrccsp_cell_selection_procedure( cmd_ptr );
              break;

            /* Add more procedures (that send Cell Change Req.) here */
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : proc %d not found for cell chg cnf",cmd_ptr->cmd.cell_change_cnf.req_proc_id);
              break;
          }
          break;

        case TDSRRC_NEW_CELL_IND:
          /* Call a CCM function to propagate New Cell Ind to all procedures
           * that need it. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCCCM Revd TDSRRC_NEW_CELL_IND");
          tdsrrcccm_propagate_new_cell_to_all_procs( cmd_ptr );
          break;
/*Let RCE handle it at this stage*/  
         case TDSRRC_CELL_RESELECTION_FAILURE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCCSP->RRCRCE: Dispatching TDSRRC_CELL_RESELECTION_FAILURE_IND");
          tdsrrcrce_procedure_event_handler(cmd_ptr);
          break;
        case TDSRRC_CHANNEL_CONFIG_REQ:
          switch(cmd_ptr->cmd.chan_config_req.procedure)
          {
            case TDSRRC_PROCEDURE_CCM:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCCM->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_CHO:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCHO->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_CSP:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_CU:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_IHO:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCIHO->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_LBT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLBT->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_LLC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_PCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPCR->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_RBE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBE->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_RBR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBR->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_RBRC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBRC->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_RCE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRCE->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_SIB:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_SMC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            case TDSRRC_PROCEDURE_TCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTCR->RRCLLC: Dispatching TDSRRC_CHANNEL_CONFIG_REQ to LLC");
                break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRC Channel config Req revd for unknown proc: %d",cmd_ptr->cmd.chan_config_req.procedure);
                break;
          }
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CHANNEL_CONFIG_CNF:
          /* Call the procedure that needs to process this command */
          switch (cmd_ptr->cmd.chan_config_cnf.procedure)
          {
            case TDSRRC_PROCEDURE_SIB:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCSIB: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to SIB");
                tdsrrcsib_event_controller( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_CSP:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCSP: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to CSP");
                tdsrrccsp_cell_selection_procedure( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_CCM:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCCM: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to CCM");
                tdsrrcccm_cell_change_manager( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RCE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCRCE: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to RCE");
                tdsrrcrce_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCRCR: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to RCR");
                tdsrrcrcr_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RBE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCRBE: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to RBE");
                tdsrrcrbe_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RBR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCRBR: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to RBR");
                tdsrrcrbr_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_LBT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCLBT: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to LBT");
                tdsrrclbt_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_CU:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCU: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to CU");
                tdsrrccu_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_RBRC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCRBRC: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to RBRC");
                tdsrrcrbrc_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_TCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCTCR: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to TCR");
                tdsrrctcrc_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_PCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCPCR: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to PCR");
                tdsrrcpcrc_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_CHO:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCHO: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to CHO");
                tdsrrccho_procedure_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_IHO:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCIHO: Dispatching TDSRRC_CHANNEL_CONFIG_CNF to IHO");
                tdsrrciho_event_handler( cmd_ptr );
                break;

            case TDSRRC_PROCEDURE_NONE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : Dispatching TDSRRC_CHANNEL_CONFIG_CNF to TDSRRC_PROCEDURE_NONE");
                /* No procedure to be called here. Just break */
                break;

            default:
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Failed to id Proc: %d for Chan Config Cnf",(cmd_ptr->cmd.chan_config_cnf.procedure));
                break;
          } /* end switch */
          break;

        case TDSRRC_RESET_L1_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCLLC: Dispatching TDSRRC_RESET_L1_REQ to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_RESET_L1_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCSP: Dispatching TDSRRC_RESET_L1_CNF to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_LOWER_LAYER_DEACT_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCLLC: Dispatching TDSRRC_LOWER_LAYER_DEACT_REQ to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          tdsrrc_deact_status = TDSRRC_DEACT_INITIATED;
          break;

        case TDSRRC_LOWER_LAYER_DEACT_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCSP: Dispatching TDSRRC_LOWER_LAYER_DEACT_CNF to CSP");
          /* Write the acq. database and acq. list to NV */
          tdsrrccsp_write_nv_acq_info();
          tdsl1_clear_barred_cells();
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          tdsrrc_deact_status = TDSRRC_DEACT_COMPLETED;
          break;

        case TDSRRC_LLC_CMD_REQ:
          switch(cmd_ptr->cmd.rrc_llc_req.procedure)
          {
            case TDSRRC_PROCEDURE_SIB:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_MCMR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMEAS->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_MCM:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_UMI:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCUMI->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_UPCC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCUPCC->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPCR->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBR->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBRC->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTCR->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_UDT:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCDT->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            case TDSRRC_PROCEDURE_CCM:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCCM->RRCLLC: Dispatching TDSRRC_LLC_CMD_REQ to LLC");
              break;
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRC Llc Cmd Req rcvd for unknown proc: %d",cmd_ptr->cmd.rrc_llc_req.procedure);
              break;
          }
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_LLC_CMD_CNF:
          /* Call the procedure that needs to process this command */
          switch (cmd_ptr->cmd.rrc_llc_cnf.procedure)
          {
            case TDSRRC_PROCEDURE_SIB:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCSIB: Dispatching TDSRRC_LLC_CMD_CNF to SIB");
              tdsrrcsib_event_controller( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCSP: Dispatching TDSRRC_LLC_CMD_CNF to CSP");
              tdsrrccsp_cell_selection_procedure( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CCM:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCCM: Dispatching TDSRRC_LLC_CMD_CNF to CCM");
              tdsrrcccm_cell_change_manager( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_MCMR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCMEAS: Dispatching TDSRRC_LLC_CMD_CNF to MEAS");
              tdsrrcmeas_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_MCM:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCMCM: Dispatching TDSRRC_LLC_CMD_CNF to MCM");
              tdsrrcmcm_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCU: Dispatching TDSRRC_LLC_CMD_CNF to CU");
              tdsrrccu_procedure_event_handler( cmd_ptr );
              break;


            case TDSRRC_PROCEDURE_NONE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : Dispatch TDSRRC_LLC_CMD_CNF to TDSRRC_PROCEDURE_NONE");
              /* No procedure to be called here. Just break */
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Failed to id Proc: %d for LLC Cnf",(cmd_ptr->cmd.rrc_llc_cnf.procedure));
              break;
          } /* end switch */
          break;

        case TDSRRC_DOWNLINK_L2ACK_IND:
          if((tdsrrclcm_get_ul_rb_id(cmd_ptr->cmd.downlink_l2ack_ind.lc_id) == DCCH_AM_RADIO_BEARER_ID) ||
             (tdsrrclcm_get_ul_rb_id(cmd_ptr->cmd.downlink_l2ack_ind.lc_id) == DCCH_DT_HIGH_PRI_RADIO_BEARER_ID))
          {
            tdsrrcwrm_reset_l2_ack_req_mask();
          }
          if(TDSRRCLCM_SUCCESS == tdsrrclcm_identify_proc(cmd_ptr,
               tdsrrclcm_get_ul_rb_id(cmd_ptr->cmd.downlink_l2ack_ind.lc_id),
               &rrc_proc_name))
          {

            if( (tdsrrc_wait_for_l2ack_for_reconfig == rrc_proc_name)
                && (cmd_ptr->cmd.downlink_l2ack_ind.status == SUCCESS))
            {
              tdsrrc_update_reconfig_waiting_for_l2ack_dch(TDSMAX_NUM_OF_PROCEDURES);
            }

            if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
            {
              /* Update RACH tx stats here if we receive L2 ACK Ind fail*/
              if(cmd_ptr->cmd.downlink_l2ack_ind.status != SUCCESS)
              {
                /*RACH tx failed for other reason, so update RACH tx parameters count here*/
                TDSRRC_DEBUG_INCREMENT_STATS(TDSRACH_RRC_RETRY_FAIL_DUE_TO_OTHER_CAUSE);
              }
            }
            switch(rrc_proc_name)
            {
              case TDSRRC_PROCEDURE_RCE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCRCE: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to RCE");
                tdsrrcrce_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_RCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCRCR: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to RCR");
                tdsrrcrcr_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_RBE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCRBE: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to RBE");
                tdsrrcrbe_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_RBR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCRBR: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to RBR");
                tdsrrcrbr_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_MCMR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCMEAS: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to MEAS");
                tdsrrcmeas_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_CU:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCCU: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to CU");
                tdsrrccu_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_SMC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCSMC: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to SMC");
                tdsrrcsmc_procedure_event_handler( cmd_ptr );
                  break;

              case TDSRRC_PROCEDURE_RBRC:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCRBRC: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to RBRC");
                tdsrrcrbrc_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_TCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCTCRC: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to TCRC");
                tdsrrctcrc_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_PCR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCPCR: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to PCR");
                tdsrrcpcrc_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_IDT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCDT: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to IDT");
                tdsrrcidt_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_UDT:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCDT: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to UDT");
                tdsrrcudt_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_SCRR:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCSCRR: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to SCRR");
                tdsrrcscrr_procedure_event_handler( cmd_ptr );
                break;

              case TDSRRC_PROCEDURE_UMI:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCUMI: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to UMI");
                tdsrrcumi_procedure_event_handler( cmd_ptr );
                break;
              case TDSRRC_PROCEDURE_NONE:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : Dispatching TDSRRC_DOWNLINK_L2ACK_IND to TDSRRC_PROCEDURE_NONE");
                /* No procedure will be called. Just break */
                break;

              case TDSRRC_PROCEDURE_DORMANCY:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCDORMANCY: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to DORMANCY");
                tdsrrcdormancy_procedure_event_handler(cmd_ptr );
                break;

#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
              case TDSRRC_PROCEDURE_IHO:
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L2->RRCIHO: Dispatching TDSRRC_DOWNLINK_L2ACK_IND to IHO");
                tdsrrciho_event_handler( cmd_ptr );
                break;
#endif

              default:
                TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : L2ACK rcvd for unknown Proc: %d",rrc_proc_name);
                break;
            }
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failed Id to proc for L2ACK or proc has not registered for L2ACK");
          }
          break;

        case TDSRRC_TIMER_TO_CAMP_ON_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCRCE: Dispatching TDSRRC_TIMER_TO_CAMP_ON_EXPIRED_IND to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_WAIT_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCRCE: Dispatching TDSRRC_WAIT_TIMER_EXPIRED_IND to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;

#ifdef FEATURE_TDSCDMA_REL7_SPECRS 
       
        case TDSRRC_COUEC_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCMISC Revd TDSRRC_COUEC_REQ");
          tdsrrccouec_procedure_event_handler( cmd_ptr );
          break;


        case TDSRRC_TIMER_COUEC_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCMISC: Dispatching TDSRRC_TIMER_COUEC_EXPIRED_IND to MISC");
          tdsrrccouec_procedure_event_handler(cmd_ptr);
          break;
#endif /*FEATURE_TDSCDMA_REL7_SPECRS*/

        case TDSRRC_T320_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCMISC: Dispatching TDSRRC_T320_EXPIRED_IND to MISC");
          tdsrrcmisc_procedure_event_handler(cmd_ptr);
          break;          

#if defined(FEATURE_3GPP_FAST_DORMANCY)
        case TDSRRC_T323_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCDORMANCY: Dispatching TDSRRC_T323_EXPIRED_IND to DORMANCY");
          tdsrrcdormancy_procedure_event_handler(cmd_ptr);
          break;    
#endif
        case TDSRRC_PHY_CHAN_FAILURE_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCRCE: Dispatching TDSRRC_PHY_CHAN_FAILURE_TIMER_EXPIRED_IND to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;
        case TDSRRC_RRC_CON_DELAY_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCRCE: Dispatching TDSRRC_RRC_CON_DELAY_TIMER_EXPIRED_IND to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T300_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCRCE: Dispatching TDSRRC_T300_EXPIRED_IND to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T302_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCU: Dispatching TDSRRC_T302_EXPIRED_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T305_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCU: Dispatching TDSRRC_T305_EXPIRED_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T316_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCU: Dispatching TDSRRC_T316_EXPIRED_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T317_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCU: Dispatching TDSRRC_T317_EXPIRED_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T307_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCU: Dispatching TDSRRC_T307_EXPIRED_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T314_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCU: Dispatching TDSRRC_T314_EXPIRED_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T315_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCU: Dispatching TDSRRC_T315_EXPIRED_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;
 
        case TDSRRC_OOS_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCU: Dispatching TDSRRC_OOS_TIMER_EXPIRED_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

#ifdef FEATURE_INTERRAT_PCCO_G2TD
        case TDSRRC_T3174_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCHO: Dispatching TDSRRC_T3174_EXPIRED_IND to CHO");
          tdsrrccho_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_CONN_SETUP_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRCE->RRCCHO: Dispatching TDSRRC_CONN_SETUP_IND to CHO");
          tdsrrccho_procedure_event_handler( cmd_ptr );
          break;
#endif

        case TDSRRC_TIMER_FOR_RCE_L2ACK_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCRCE Revd TDSRRC_TIMER_FOR_RCE_L2ACK_EXPIRED_IND");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;

        // CR 352866 begin
        case TDSRRC_TIMER_TO_RCE_WAIT_RESELCT_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCRCE: Dispatching TDSRRC_TIMER_TO_RCE_WAIT_RESELCT_EXPIRED_IND to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;
        // CR 352866 end

#ifdef FEATURE_WRLF_SYSTEM_SEL
		// CR 430034 RAU Reject workaround
        case TDSRRC_DELAY_OOS_RLF_RPT_TIMER_EXPIRED_IND:
		  TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCMCM: Dispatching stored TDSRRC_SERVICE_REQ to MCM at TDSRRC_DELAY_OOS_RLF_RPT_TIMER_EXPIRED_IND. CR430034");
          tdsrrcmcm_event_handler(&tdsrrc_csp_int_data.stored_svc_req_for_delay_rlf_rpt);
          break;
#endif

        case TDSRRC_T319_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCMISC: Dispatching TDSRRC_T319_EXPIRED_IND to MISC");
          tdsrrcmisc_procedure_event_handler(cmd_ptr);
          break;
        case TDSRRC_HOLD_PS_TIMER_EXPIRED_IND:
          if (tdsrrcdt_int_data.ps_domain_open == FALSE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCIDT: Dispatching TDSRRC_HOLD_PS_TIMER_EXPIRED_IND to IDT."); 
            tdsrrcidt_procedure_event_handler( cmd_ptr );
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCUDT: Dispatching TDSRRC_HOLD_PS_TIMER_EXPIRED_IND to UDT."); 
            tdsrrcudt_procedure_event_handler( cmd_ptr );
          }
          break;
        case TDSRRC_REDIRECT_REQ:
          switch(cmd_ptr->cmd.redirect_req.proc_id)
          {
            case TDSRRC_PROCEDURE_RCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRCR->RRCCSP: Dispatching TDSRRC_REDIRECT_REQ to CSP");
              break;
              
            case TDSRRC_PROCEDURE_RCE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRCE->RRCCSP: Dispatching TDSRRC_REDIRECT_REQ to CSP");
              break;
              
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRC Redirect Req rcvd for unknown proc: %d",cmd_ptr->cmd.redirect_req.proc_id);
              break;
          }
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_INITIATE_CELL_SELECTION_IND:
          switch(cmd_ptr->cmd.initiate_cell_sel_ind.proc)
          {
            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;
              
            case TDSRRC_PROCEDURE_RCE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRCE->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;
              
            case TDSRRC_PROCEDURE_SIB:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_IDT:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCDT->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_UDT:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCDT->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_LLC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_PG1:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPG1->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_RCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRCR->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_UMI:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCUMI->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_RBE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBE->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBR->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBRC->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTCR->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPCR->RRCCSP: Dispatching TDSRRC_INITIATE_CELL_SELECTION_IND to CSP");
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Initiate Cell selection Ind rcvd for unknown proc: %d",cmd_ptr->cmd.initiate_cell_sel_ind.proc);
              break;
          }
#ifdef FEATURE_CMCC_SECURE_CALL		  
          if (cmd_ptr->cmd.initiate_cell_sel_ind.cause == TDSRRC_TRANSITION_TO_DISCONNECTED)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Secure call: clear secure call status when transition to DISCONNECTED");
            tdsrrcmcm_update_secure_call_status(FALSE);
          }
#endif		  
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_DISABLE_PREV_FREQ_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCCSP Revd TDSRRC_DISABLE_PREV_FREQ_IND ");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_REDIRECT_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCRCE: Dispatching TDSRRC_REDIRECT_CNF to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_TIMER_FOR_RCR_L2ACK_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCRCR: Dispatching TDSRRC_TIMER_FOR_RCR_L2ACK_EXPIRED_IND to RCR");
          tdsrrcrcr_procedure_event_handler( cmd_ptr );
          break; 

        case TDSRRC_T308_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCRCR: Dispatching TDSRRC_T308_EXPIRED_IND to RCR");
          tdsrrcrcr_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_T304_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCUECI: Dispatching TDSRRC_T304_EXPIRED_IND to UECI");
          tdsrrcueci_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_IGNORE_BCCH_MOD_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCPG1: Dispatching TDSRRC_IGNORE_BCCH_MOD_TIMER_EXPIRED_IND to PG1");
          tdsrrcpg1_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_TIMER_FOR_RSSI_BER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : TDSRRC_TIMER_FOR_RSSI_BER_EXPIRED_IND is received");
          tdsrrc_rssi_ber_timeout_handler();
          break;

        case TDSRRC_SIB_WAIT_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCSIB: Dispatching TDSRRC_SIB_WAIT_TIMER_EXPIRED_IND to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;

        case TDSRRC_SIB_STOP_PARTITION_TMR_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCSIB: Dispatching TDSRRC_SIB_STOP_PARTITION_TMR_EXPIRED_IND to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;

        case TDSRRC_SIB_MASK_INVERT_TMR_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCSIB: Dispatching TDSRRC_SIB_MASK_INVERT_TMR_EXPIRED_IND to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;

        case TDSRRC_SIB_EMPTY_MASK_TMR_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCSIB: Dispatching TDSRRC_SIB_EMPTY_MASK_TMR_EXPIRED_IND to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;

        case TDSRRCTMR_SIB_ALL_ONE_MASK_TMR_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCSIB: Dispatching TDSRRCTMR_SIB_ALL_ONE_MASK_TMR_EXPIRED_IND to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;		  

        case TDSRRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_DCH_FACH_CELL_SELECTION_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
        case TDSRRC_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_FACH_TO_FACH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */

        case TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCLLC: Dispatching TDSRRCTMR_L1_DEADLOCK_DETECT_TIMER_EXPIRED_IND to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRCTMR_DELAY_ERR_FATAL_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCLLC: Dispatching TDSRRCTMR_DELAY_ERR_FATAL_TIMER_EXPIRED_IND to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_DCH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_DCH_PCH_CELL_SELECTION_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_INITIATE_UE_CAPABILITY_REQ:
          switch(cmd_ptr->cmd.initiate_ue_cap.originator)
          {
            case TDSRRC_UE_CAPABILITY_ORIGINATOR_UTRAN:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCUECE->RRCUECI: Dispatching TDSRRC_INITIATE_UE_CAPABILITY_REQ to UECI");
              break;
              
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Initiate ue Capability Req Utran revd for unknown proc: %d",cmd_ptr->cmd.initiate_ue_cap.originator);
              break;
          }
          tdsrrcueci_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_CELL_SELECTED_MEAS_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCCM->RRCMEAS: Dispatching TDSRRC_CELL_SELECTED_MEAS_REQ to MEAS");
          tdsrrcmeas_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_CELL_UPDATE_INITIATE_REQ:
          switch(cmd_ptr->cmd.initiate_cell_update.procedure)
          { 
            case TDSRRC_PROCEDURE_SIB:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_UECI:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCUECI->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break; 

            case TDSRRC_PROCEDURE_PG1:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPG1->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;
        
            case TDSRRC_PROCEDURE_MISC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMISC->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_IHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCIHO->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;
            
            case TDSRRC_PROCEDURE_CHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCHO->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_DORMANCY:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCDORMANCY->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_IDT:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCDT->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPCR->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_RBE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBE->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBR->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBRC->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTCR->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_RCE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRCE->RRCCU: Dispatching TDSRRC_CELL_UPDATE_INITIATE_REQ to CU");
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Initiate cell update Req rcvd for unknown proc: %d",cmd_ptr->cmd.initiate_cell_update.procedure);
              break;
          }
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_URA_UPDATE_INITIATE_REQ:
          switch(cmd_ptr->cmd.initiate_ura_update.procedure)
          {
            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCCU: Dispatching TDSRRC_URA_UPDATE_INITIATE_REQ to CU");
              break;
  
            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPCR->RRCCU: Dispatching TDSRRC_URA_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBR->RRCCU: Dispatching TDSRRC_URA_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBRC->RRCCU: Dispatching TDSRRC_URA_UPDATE_INITIATE_REQ to CU");
              break;

            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTCR->RRCCU: Dispatching TDSRRC_URA_UPDATE_INITIATE_REQ to CU");
              break;
              
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Initiate Ura Update Req rcvd for unknown proc: %d",cmd_ptr->cmd.initiate_ura_update.procedure);
              break;
          }
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
        case TDSRRC_RLC_UL_DATA_IND:
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;
#endif
        case TDSRRC_RNTI_UPDATE_REQ:
          switch(cmd_ptr->cmd.rnti_update_req.procedure)
          {
            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCLLC: Dispatching TDSRRC_RNTI_UPDATE_REQ to LLC");
              break;
  
            case TDSRRC_PROCEDURE_RBE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBE->RRCLLC: Dispatching TDSRRC_RNTI_UPDATE_REQ to LLC");
              break;

            case TDSRRC_PROCEDURE_UMI:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCUMI->RRCLLC: Dispatching TDSRRC_RNTI_UPDATE_REQ to LLC");
              break;
        
            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBRC->RRCLLC: Dispatching TDSRRC_RNTI_UPDATE_REQ to LLC");
              break;

            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBR->RRCLLC: Dispatching TDSRRC_RNTI_UPDATE_REQ to LLC");
              break;

            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTCR->RRCLLC: Dispatching TDSRRC_RNTI_UPDATE_REQ to LLC");
              break;
            
            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPCR->RRCLLC: Dispatching TDSRRC_RNTI_UPDATE_REQ to LLC");
              break;
              
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Rnti update Req rcvd for unknown proc: %d",cmd_ptr->cmd.rnti_update_req.procedure);
              break;
          }
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_RNTI_UPDATE_CNF:
          switch(cmd_ptr->cmd.rnti_update_cnf.procedure)
          { 
            case TDSRRC_PROCEDURE_UMI:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCUMI: Dispatching TDSRRC_RNTI_UPDATE_CNF to UMI");
              tdsrrcumi_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCU: Dispatching TDSRRC_RNTI_UPDATE_CNF to CU");
              tdsrrccu_procedure_event_handler( cmd_ptr );  
              break; 

            case TDSRRC_PROCEDURE_RBE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCRBE: Dispatching TDSRRC_RNTI_UPDATE_CNF to RBE");
              tdsrrcrbe_procedure_event_handler(cmd_ptr );
              break;
        
            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCRBRC: Dispatching TDSRRC_RNTI_UPDATE_CNF to RBRC");
              tdsrrcrbrc_procedure_event_handler(cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCRBR: Dispatching TDSRRC_RNTI_UPDATE_CNF to RBR");
              tdsrrcrbr_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCTCRC: Dispatching TDSRRC_RNTI_UPDATE_CNF to TCRC");
              tdsrrctcrc_procedure_event_handler( cmd_ptr );
              break;
            
            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCPCRC: Dispatching TDSRRC_RNTI_UPDATE_CNF to PCRC");
              tdsrrcpcrc_procedure_event_handler( cmd_ptr );
              break;

            /* New procedures need to be added here */

            case TDSRRC_PROCEDURE_NONE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : Dispatched TDSRRC_RNTI_UPDATE_CNF to TDSRRC_PROCEDURE_NONE: ");
              /* This does not call any RRC procedure. Just break */
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Rnti Update Cnf rcvd for unknown proc: %d",cmd_ptr->cmd.rnti_update_cnf.procedure);
              break;
          }
          break;

        case TDSRRC_CELL_UPDATE_COMPLETE_CNF:
          switch(cmd_ptr->cmd.cu_complete_cnf.procedure)
          {
            case TDSRRC_PROCEDURE_SCRR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCDT: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to SCRR");
              tdsrrcscrr_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_DORMANCY:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCDORMANCY: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to DORMANCY");
              tdsrrcdormancy_procedure_event_handler(cmd_ptr );
              break;
            case TDSRRC_PROCEDURE_RCE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCRCE: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to RCE");
              tdsrrcrce_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_IDT:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCDT: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to IDT");
              tdsrrcidt_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_UDT:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCDT: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to UDT");
              tdsrrcudt_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_UECI:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCUECI: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to UECI");
              tdsrrcueci_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_RBE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCRBE: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to RBE");
              tdsrrcrbe_procedure_event_handler(cmd_ptr );
              break;
        
            case TDSRRC_PROCEDURE_IHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCIHO: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to IHO");
              tdsrrciho_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCHO->RRCCHO: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to CHO");
              tdsrrccho_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCRBRC: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to RBRC");
              tdsrrcrbrc_procedure_event_handler(cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCRBR: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to RBR");
              tdsrrcrbr_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCTCRC: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to TCRC");
              tdsrrctcrc_procedure_event_handler( cmd_ptr );
              break;
            
            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCPCRC: Dispatching TDSRRC_CELL_UPDATE_COMPLETE_CNF to PCRC");
              tdsrrcpcrc_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_NONE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : Dispatched TDSRRC_CELL_UPDATE_COMPLETE_CNF to TDSRRC_PROCEDURE_NONE: ");
              /* This does not call any RRC procedure. Just break */
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : CU Comp Cnf rcvd for unknown proc: %d",cmd_ptr->cmd.cu_complete_cnf.procedure);
              break;
          } 
          break;
      
        case TDSRRC_STOP_LOWER_LAYER_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCLLC: Dispatching TDSRRC_STOP_LOWER_LAYER_REQ to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;
  
        case TDSRRC_STOP_LOWER_LAYER_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCMCM: Dispatching TDSRRC_STOP_LOWER_LAYER_CNF to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

        case TDSRRC_MODE_CHANGE_IND:
            /* Inform the procedure based on the procedure identifier in
               the command */
          switch(cmd_ptr->cmd.mode_change_ind.procedure)
          {
            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCCSP: Dispatching TDSRRC_MODE_CHANGE_IND to CSP");
              tdsrrccsp_cell_selection_procedure( cmd_ptr );
              break;
  
            case TDSRRC_PROCEDURE_CCM:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCCCM: Dispatching TDSRRC_MODE_CHANGE_IND to CCM");
              tdsrrcccm_cell_change_manager( cmd_ptr );
              break; 

            case TDSRRC_PROCEDURE_IHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCIHO: Dispatching TDSRRC_MODE_CHANGE_IND to IHO");
                tdsrrciho_event_handler( cmd_ptr );
                break;
				
            case TDSRRC_PROCEDURE_SMC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCSMC: Dispatching TDSRRC_MODE_CHANGE_IND to SMC");
              tdsrrcsmc_procedure_event_handler( cmd_ptr );  
			  break;
			  
              /* Only CSP will handle the mode transition to Inactive for now */
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Procedure:%d doesn't handle MODE_CHANGE_IND",cmd_ptr->cmd.mode_change_ind.procedure);
              break;
          }
          break;

        case TDSRRC_CELL_RESELECTION_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCCM->RRCLLC: Dispatching TDSRRC_CELL_RESELECTION_REQ to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;
  
        case TDSRRC_CELL_RESELECTION_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCCCM: Dispatching TDSRRC_CELL_RESELECTION_CNF to CCM");
          tdsrrcccm_cell_change_manager( cmd_ptr );
          break;
  
        case TDSRRC_SIB7_EXP_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCSIB: Dispatching TDSRRC_SIB7_EXP_TIMER_EXPIRED_IND to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;
  
        case TDSRRC_SIB7_UPDATE_STATUS_IND:
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
          if(cmd_ptr->cmd.sib7_update_status.procedure == TDSRRC_PROCEDURE_CU)
          { 
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCCU: Dispatching TDSRRC_SIB7_UPDATE_STATUS_IND to CU");
            tdsrrccu_procedure_event_handler( cmd_ptr );
          }
          else
#endif
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIB->RRCRCE: Dispatching TDSRRC_SIB7_UPDATE_STATUS_IND to RCE");
            tdsrrcrce_procedure_event_handler( cmd_ptr );
          }
          break;

        case TDSRRC_CHANGE_MODE_REQ:
          switch(cmd_ptr->cmd.change_mode_req.procedure)
          {
            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCMCM: Dispatching TDSRRC_CHANGE_MODE_REQ to MCM");
              break;

            case TDSRRC_PROCEDURE_IHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCIHO->RRCMCM: Dispatching TDSRRC_CHANGE_MODE_REQ to MCM");
              break;
        
            case TDSRRC_PROCEDURE_CHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCHO->RRCMCM: Dispatching TDSRRC_CHANGE_MODE_REQ to MCM");
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRC Change Req rcvd for unknown proc: %d",cmd_ptr->cmd.change_mode_req.procedure);
              break;
          }
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

        case TDSRRC_CHANGE_MODE_CNF:
          switch(cmd_ptr->cmd.change_mode_cnf.procedure)
          {
            /* CSP */
            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCCSP: Dispatching TDSRRC_CHANGE_MODE_CNF to CSP");
              tdsrrccsp_cell_selection_procedure( cmd_ptr );
              break;
        
            /* IHO */
            case TDSRRC_PROCEDURE_IHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCIHO: Dispatching TDSRRC_CHANGE_MODE_CNF to IHO");
              tdsrrciho_event_handler( cmd_ptr );
              break;
        

            /* CHO */
            case TDSRRC_PROCEDURE_CHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCCHO: Dispatching TDSRRC_CHANGE_MODE_CNF to CHO");
              tdsrrccho_procedure_event_handler( cmd_ptr );
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : Procedure:%d doesn't handle CHANGE_MODE_CNF",cmd_ptr->cmd.change_mode_cnf.procedure);
              break;
          }
          break;

        case TDSRRC_NAS_DATA_TRANSFER_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCIHO->RRCDT: Dispatching TDSRRC_NAS_DATA_TRANSFER_IND to IDT and UDT");
          tdsrrcidt_procedure_event_handler( cmd_ptr );
          tdsrrcudt_procedure_event_handler( cmd_ptr );
          break;

          
#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
        case TDSRRC_CSP_ACQ_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCIHO->RRCCSP: Dispatching TDSRRC_CSP_ACQ_REQ to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_CSP_ACQ_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCIHO: Dispatching TDSRRC_CSP_ACQ_CNF to IHO");
          tdsrrciho_event_handler( cmd_ptr );
          break;

        case TDSRRC_HO_CIPH_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCIHO->RRCSMC: Dispatching TDSRRC_HO_CIPH_REQ to SMC");
          tdsrrcsmc_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_HO_CIPH_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCIHO: Dispatching TDSRRC_HO_CIPH_CNF to IHO");
          tdsrrciho_event_handler( cmd_ptr );
          break;

#endif
          /*commands added for ciphering related issues*/
          
        case TDSRRC_CIPH_CONFIG_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCLLC: Dispatching TDSRRC_CIPH_CONFIG_REQ to LLC");
          tdsrrcllc_process_cmd(cmd_ptr);
          break;
          
        case TDSRRC_ABORT_CIPH_CONFIG_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCLLC: Dispatching TDSRRC_ABORT_CIPH_CONFIG_REQ to LLC");
          tdsrrcllc_process_cmd(cmd_ptr);
          break;

           
        case TDSRRC_CIPH_CONFIG_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCSMC: Dispatching TDSRRC_CIPH_CONFIG_CNF to SMC");
          tdsrrcsmc_procedure_event_handler( cmd_ptr );
          break;
          
        case TDSRRC_RECONFIG_COMPLETE_IND:
          if(tdsrrctfcc_is_reconfig_in_progress() == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCTFCC: Dispatching TDSRRC_RECONFIG_COMPLETE_IND to TFCC");
            tdsrrctfcc_procedure_event_handler(cmd_ptr);
          }
          if(tdsrrcsmc_is_reconfig_in_progress() == TRUE)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCSMC: Dispatching TDSRRC_RECONFIG_COMPLETE_IND to SMC");
            tdsrrcsmc_procedure_event_handler( cmd_ptr );
          }
          break;

        case TDSRRC_SIG_CONN_RELEASE_REQ:          
          tdsrrcscrr_procedure_event_handler( cmd_ptr );
          break;
            
        case TDSRRC_SIG_CONN_RELEASE_CNF:
#if defined(FEATURE_3GPP_FAST_DORMANCY)
          switch(cmd_ptr->cmd.rrc_sig_conn_cnf.rrc_proc)
          {
            case TDSRRC_PROCEDURE_RCE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCRR->RRCRCE: Dispatching TDSRRC_SIG_CONN_RELEASE_CNF to RCE");
              tdsrrcrce_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_DORMANCY:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCRR->RRCDORMANCY: Dispatching TDSRRC_SIG_CONN_RELEASE_CNF to DORMANCY");
              tdsrrcdormancy_procedure_event_handler(cmd_ptr );
              break;
                
            default:
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Rrc Sig Conn Cnf rcvd for unknown proc: %d",cmd_ptr->cmd.rrc_sig_conn_cnf.rrc_proc);
              break;
          }
#else
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSCRR->RRCRCE: Dispatching TDSRRC_SIG_CONN_RELEASE_CNF to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
#endif            
          break;


        case TDSRRC_SMC_DDT_L2_ACK_IND:
#ifdef  FEATURE_TDSCDMA_SRNS_OLD_FRESH_BUFFER_DDT
        case TDSRRC_SMC_SRNS_RLC_RE_EST_CMP:
#endif   
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCDT: Dispatching TDSRRC_SMC_DDT_L2_ACK_IND or TDSRRC_SMC_SRNS_RLC_RE_EST_CMP to DDT");
          tdsrrcddt_procedure_event_handler( cmd_ptr );
          break;


        case TDSRRC_RE_EST_RLC_FOR_SRNS_REQ:
          switch(cmd_ptr->cmd.rrc_re_est_srns_req.rrc_proc)
          {
            case TDSRRC_PROCEDURE_RBE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBE->RRCSMC: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_REQ to SMC");
              break;
        
            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBRC->RRCSMC: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_REQ to SMC");
              break;

            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBR->RRCSMC: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_REQ to SMC");
              break;

            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTCRC->RRCSMC: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_REQ to SMC");
              break;
            
            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCPCRC->RRCSMC: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_REQ to SMC");
              break;

            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCSMC: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_REQ to SMC");
              break;
            
            case TDSRRC_PROCEDURE_UMI:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCUMI->RRCSMC: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_REQ to SMC");
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Rrc re est srns Req rcvd for unknown proc: %d",cmd_ptr->cmd.rrc_re_est_srns_req.rrc_proc);
              break;
          }
          tdsrrcsmc_procedure_event_handler(cmd_ptr);
          break;

        case TDSRRC_RE_EST_RLC_FOR_SRNS_CNF:
          switch(cmd_ptr->cmd.rrc_re_est_srns_cnf.rrc_proc)
          {
            case TDSRRC_PROCEDURE_RBE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCRBE: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_CNF to RBE");
              tdsrrcrbe_procedure_event_handler(cmd_ptr );
              break;
        
            case TDSRRC_PROCEDURE_RBRC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCRBRC: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_CNF to RBRC");
              tdsrrcrbrc_procedure_event_handler(cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_RBR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCRBR: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_CNF to RBR");
              tdsrrcrbr_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_TCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCTCR: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_CNF to TCRC");
              tdsrrctcrc_procedure_event_handler( cmd_ptr );
              break;
            
            case TDSRRC_PROCEDURE_PCR:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCPCR: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_CNF to PCRC");
              tdsrrcpcrc_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCCU: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_CNF to CU");
              tdsrrccu_procedure_event_handler( cmd_ptr );
              break;
            
            case TDSRRC_PROCEDURE_UMI:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCUMI: Dispatching TDSRRC_RE_EST_RLC_FOR_SRNS_CNF to UMI");
              tdsrrcumi_procedure_event_handler( cmd_ptr );
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Rrc re est srns Cnf rcvd for unknown proc: %d",cmd_ptr->cmd.rrc_re_est_srns_cnf.rrc_proc);
              break;
          }
          break;

        case TDSRRC_LLC_RE_EST_RLC_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSMC->RRCLLC: Dispatching TDSRRC_LLC_RE_EST_RLC_REQ to LLC");
          tdsrrcllc_process_cmd(cmd_ptr);
          break;

        case TDSRRC_LLC_RE_EST_RLC_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCLLC->RRCSMC: Dispatching TDSRRC_LLC_RE_EST_RLC_CNF to SMC");
          tdsrrcsmc_procedure_event_handler(cmd_ptr);
          break;
        case TDSRRC_ERR_FATAL_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCCSP Revd TDSRRC_ERR_FATAL_IND");
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;

        case TDSRRC_CSP_CAMP_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCHO->RRCCSP: Dispatching TDSRRC_CSP_CAMP_REQ to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_CSP_CAMP_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCCHO: Dispatching TDSRRC_CSP_CAMP_CNF to CHO");
          tdsrrccho_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_FREQ_SCAN_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_FREQ_SCAN_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_BPLMN_SRCH_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_BPLMN_SRCH_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_BPLMN_FREQ_SCAN_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;


        case TDSRRC_DEEP_SLEEP_NO_SVC_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_DEEP_SLEEP_NO_SVC_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case RRC_CSP_TDS_RESUMED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCHO->RRCCSP: Dispatching RRC_CSP_TDS_RESUMED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_FREQ_REDIRECT_WAIT_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_FREQ_REDIRECT_WAIT_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
        
        case TDSRRC_IRAT_ABORT_IND:
          switch(cmd_ptr->cmd.rrc_irat_abort_ind.procedure)
          {
            case TDSRRC_PROCEDURE_CSP:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCCSP: Dispatching TDSRRC_IRAT_ABORT_IND to CSP");
              tdsrrccsp_cell_selection_procedure( cmd_ptr );
              break;
             case TDSRRC_PROCEDURE_CHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCCSP: Dispatching TDSRRC_IRAT_ABORT_IND to CHO");
              tdsrrccho_procedure_event_handler(cmd_ptr);
              break;

            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Rrc Irat Abort Ind rcvd for unknown proc: %d",cmd_ptr->cmd.rrc_irat_abort_ind.procedure);
              break;
          }
          break;

        case TDSRRC_CONN_MODE_OOS_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_CONN_MODE_OOS_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_IDT_COMPLETED_FOR_DOMAIN:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCSCRR Revd TDSRRC_IDT_COMPLETED_FOR_DOMAIN");
          tdsrrcscrr_procedure_event_handler(cmd_ptr);
          break;


        case RRC_TD2TD_BPLMN_DRX_SRCH_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> :RRCTMR->RRCCSP: Dispatching RRC_TD2TD_BPLMN_DRX_SRCH_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_BPLMN_GUARD_SRCH_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_BPLMN_GUARD_SRCH_TIMER_EXPIRED_IND to CSP"); 
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
        case TDSRRC_BLOCK_CS_RESEL_NAS_API_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_BLOCK_CS_RESEL_NAS_API_TIMER_EXPIRED_IND to CSP"); 
          tdsrrccsp_cell_selection_procedure( cmd_ptr ); 
          break;
        case TDSRRC_BLOCK_PS_RESEL_NAS_API_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_BLOCK_PS_RESEL_NAS_API_TIMER_EXPIRED_IND to CSP"); 
          tdsrrccsp_cell_selection_procedure( cmd_ptr ); 
          break;

        case TDSRRC_DA_BACKOFF_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_DA_BACKOFF_TIMER_EXPIRED_IND to MISC"); 
          tdsrrcmisc_da_backoff_timer_expired(); 
          da_backoff_timer_expired_event.expired_band = tdsrrc_csp_int_data.band_pref;
          event_report_payload(EVENT_TDSCDMA_DA_BACKOFF_TIMER_EXPIRED, sizeof(da_backoff_timer_expired_event),
                                    (void *)&da_backoff_timer_expired_event);
          break;

        case TDSRRC_BPLMN_SUSPEND_REQ:
          switch(cmd_ptr->cmd.rrc_csp_bplmn_suspend_req.procedure)
          {
            case TDSRRC_PROCEDURE_RCE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRCE->RRCCSP: Dispatching TDSRRC_BPLMN_SUSPEND_REQ to CSP");
              break;

            case TDSRRC_PROCEDURE_SIB:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCSIBPROC->RRCCSP: Dispatching TDSRRC_BPLMN_SUSPEND_REQ to CSP");
              break;
  
            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCU->RRCCSP: Dispatching TDSRRC_BPLMN_SUSPEND_REQ to CSP");
              break;

            case TDSRRC_PROCEDURE_CHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCHO->RRCCSP: Dispatching TDSRRC_BPLMN_SUSPEND_REQ to CSP");
              break;
              
            case TDSRRC_PROCEDURE_IDT:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCDT->RRCCSP: Dispatching TDSRRC_BPLMN_SUSPEND_REQ to CSP");
              break;

            case TDSRRC_PROCEDURE_MISC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMISC->RRCCSP: Dispatching TDSRRC_BPLMN_SUSPEND_REQ to CSP");
              break;
              
            default:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : Rrc csp bplmn suspend Req rcvd for unknown proc: %d",cmd_ptr->cmd.rrc_csp_bplmn_suspend_req.procedure);
              break;
          }
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
  
        case TDSRRC_BPLMN_SUSPEND_CNF:
          switch(cmd_ptr->cmd.rrc_csp_bplmn_suspend_cnf.procedure)
          {
            case TDSRRC_PROCEDURE_RCE:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCRCE: Dispatching TDSRRC_BPLMN_SUSPEND_CNF to RCE");
              tdsrrcrce_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_SIB:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCSIB: Dispatching TDSRRC_BPLMN_SUSPEND_CNF to SIB");
              tdsrrcsib_event_controller( cmd_ptr );
              break;
  
            case TDSRRC_PROCEDURE_CU:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCCU: Dispatching TDSRRC_BPLMN_SUSPEND_CNF to CU");
              tdsrrccu_procedure_event_handler( cmd_ptr );
              break;

            case TDSRRC_PROCEDURE_CHO:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCCHO: Dispatching TDSRRC_BPLMN_SUSPEND_CNF to CHO");
              tdsrrccho_procedure_event_handler( cmd_ptr );
              break;


           case TDSRRC_PROCEDURE_MISC:
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCMISC: Dispatching TDSRRC_BPLMN_SUSPEND_CNF to MISC");
              tdsrrcmisc_procedure_event_handler( cmd_ptr );
              break;
              
            default:
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> : ERR: Proc id %d not supported in BPLMN_SUSPEND_CNF,throwing away cmd",cmd_ptr->cmd.rrc_csp_bplmn_suspend_req.procedure);
              break;
          }
          break;

        case TDSRRC_BPLMN_SEARCH_RESUME_IND:
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;	

        case TDSRRC_MODE_STATUS_IND:
          {
          tdsrrc_state_e_type tdsrrc_curr_state = tdsrrc_get_state();
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCMEAS: Dispatching TDSRRC_MODE_STATUS_IND to MEAS in rrc st %d",tdsrrc_curr_state);
          tdsrrcmeas_procedure_event_handler( cmd_ptr );
         
          if((tdsrrc_curr_state == TDSRRC_STATE_CELL_FACH)
            ||(tdsrrc_curr_state == TDSRRC_STATE_CELL_PCH)
            ||(tdsrrc_curr_state == TDSRRC_STATE_CELL_DCH)
            ||(tdsrrc_curr_state == TDSRRC_STATE_URA_PCH))
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMCM->RRCUECI: Dispatching TDSRRC_MODE_STATUS_IND to UECI in rrc st %d",tdsrrc_curr_state);
            tdsrrcueci_procedure_event_handler( cmd_ptr );
          }
          }
          break;

        case TDSRRC_SIX_HRS_SIB_TIMER_UPDATE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCSIB: Dispatching TDSRRC_SIX_HRS_SIB_TIMER_UPDATE_IND to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;


        case TDSRRC_CONN_MODE_DEEP_SLEEP_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCLLC: Dispatching TDSRRC_CONN_MODE_DEEP_SLEEP_REQ to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CONN_MODE_DEEP_SLEEP_WAKEUP_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCLLC: Dispatching TDSRRC_CONN_MODE_DEEP_SLEEP_WAKEUP_REQ to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_CONN_MODE_OOS_INITIAL_NO_DS_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
  
        case TDSRRC_CONN_MODE_OOS_DS_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_CONN_MODE_OOS_DS_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_RB_PROC_COMPL_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCRBCOMMON->RRCCU: Dispatching TDSRRC_RB_PROC_COMPL_IND to CU");
          tdsrrccu_procedure_event_handler(cmd_ptr );
          break;

        case TDSRRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCSIB: Dispatching TDSRRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND to SIB");
          tdsrrcsib_event_controller( cmd_ptr );
          break;

        case TDSRRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCPG1: Dispatching TDSRRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND to PG1");
          tdsrrcpg1_procedure_event_handler( cmd_ptr );
          break;

#ifdef FEATURE_TDSCDMA_PLI_NMR_UTRAN
        case TDSRRC_GSTK_UTRAN_NMR_INFO_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCMEAS->RRCMEAS: Dispatching TDSRRC_GSTK_UTRAN_NMR_INFO_REQ to MEAS");
          tdsrrcmeas_procedure_event_handler(cmd_ptr);
          break;
#endif

        case RRC_G2TD_REDIRECTION_WAIT_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching RRC_G2TD_REDIRECTION_WAIT_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
		case RRC_G2TD_REDIRECTION_LFS_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching RRC_G2TD_REDIRECTION_LFS_TIMER_EXPIRED_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case  TDSRRC_CONTINUE_STOP_MODE_REQ:
		  TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCCSP->RRCMCM: Dispatching TDSRRC_CONTINUE_STOP_MODE_REQ to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

        case RRC_TD2G_ABORT_GUARD_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching RRC_TD2G_ABORT_GUARD_TIMER_EXPIRED_IND to CSP"); 
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
        case TDSRRC_WRM_PREEMPT_IND:
          if(tdsrrcwrm_unlock_type == TRM_UNLOCK_IMMEDIATELY)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : DSIM: TDSRRC_WRM_PREEMPT_IND fwd to CSP");
            tdsrrccsp_cell_selection_procedure(cmd_ptr);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : DSIM: RRC_WRM_PREEMPT_IND ignored for unlock type %d",tdsrrcwrm_unlock_type);
          }
          break;

        case TDSRRC_NO_RESOURCE_AVAILABLE_FROM_L1:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : DSIM: RRC_CPHY_NO_RESOURCE_AVAILABLE recvd from L1");
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;

        case TDSRRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : DSIM_TUNEAWAY: RRC_DUAL_SIM_WRM_UNLOCK_BY_TIMER_EXP_IND fwd to CSP");
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;

#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
        case TDSRRC_DED_PRI_VALDITY_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCMEAS Revd TDSRRC_DED_PRI_VALDITY_TIMER_EXPIRED_IND");
          tdsrrcmeas_procedure_event_handler(cmd_ptr);
          break;

        case TDSRRC_DED_PRI_CHANGE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCMEAS Revd TDSRRC_DED_PRI_CHANGE_IND");
          tdsrrcmeas_procedure_event_handler(cmd_ptr);
          break;
#endif

#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
         case TDSRRC_IHO_OPEN_DOMAIN_IND:
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCDT Revd TDSRRC_IHO_OPEN_DOMAIN_IND");
           tdsrrcudt_procedure_event_handler(cmd_ptr);
           break;
#endif
#ifdef FEATURE_LTE_TO_TDSCDMA
      case TDSRRC_LTE_TO_TDSCDMA_SRCH_TIMER_EXPIRY_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : RRCCSP Revd TDSRRC_LTE_TO_TDSCDMA_SRCH_TIMER_EXPIRY_IND");
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;
#endif

#ifdef FEATURE_TDSCDMA_FAST_RETURN_TO_LTE_AFTER_CSFB
      case TDSRRC_INTERNAL_CSFB_ABORT_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : CSFB_HACK: TDSRRC_INTERNAL_CSFB_ABORT_REQ");
          tdsrrcrce_procedure_event_handler(cmd_ptr);
          break;
#endif

      case TDSRRC_SOFT_ABORT_GUARD_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : Dispatching TDSRRC_SOFT_ABORT_GUARD_TIMER_EXPIRED_IND to SCRR");
          tdsrrcscrr_procedure_event_handler(cmd_ptr);
          break;

#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
        case TDSRRC_BPLMN_PARTIAL_REPORT_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_BPLMN_PARTIAL_REPORT_TIMER_EXPIRED_IND");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
#endif

        case TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRCTMR_IGNORE_NON_EXIST_MCM_TIMER_EXPIRED_IND");
          tdsrrcmeas_procedure_event_handler( cmd_ptr );
          break;
        case TDSRRC_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RRCTMR->RRCCSP: Dispatching TDSRRC_TD2TD_MANUAL_BPLMN_GUARD_SRCH_TIMER_EXPIRED_IND to CSP"); 
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

#ifdef FEATURE_QSH_EVENT_METRIC
        case TDSRRC_QSH_CB_IND:
          TDSRRC_MSG0(MSG_LEGACY_MED,"TDSRRC_QSH_CB_IND");
          tdsrrcqsh_handle_metric_cb_ind( &(cmd_ptr->cmd.qsh_cb_ind) );
          break;
#endif
        case TDSRRC_RCE_TA_ALLOW_EXPIRED_IND:       
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC>: Dispatching TDSRRC_RCE_TA_ALLOW_EXPIRED_IND"); 
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;
        case TDSRRC_OUT_OF_SERVICE_INTERNAL_IND:
		  TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC>: Dispatching TDSRRC_OUT_OF_SERVICE_INTERNAL_IND"); 
          if(tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED || 
             tdsrrc_get_state() == TDSRRC_STATE_CONNECTING)
          {
             tdsrrcrce_procedure_event_handler( cmd_ptr );
          }
		  else
          {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC>: RRC not in right state. TDSRRC_OUT_OF_SERVICE_INTERNAL_IND dropped"); 
          }
          break;
        default:
        /* This should not happen. Possibly the command id was never written 
          before this item was put in the queue. Display error for now. */ 
          ERR( "<IFTDSRRC> : Unknown internal cmd rcvd by RRC 0x%x", 
            cmd_ptr->cmd_hdr.cmd_id, 0, 0);
          break;
      }/* end switch */
    }/* end if */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> : Throw away the Internal command");
    }
    /* The command has been processed. We can free the memory for this cmd */
    tdsrrc_free_cmd_buf( cmd_ptr );
  }/* end while */
}/* end function */



/*===========================================================================

FUNCTION  RRC_DISPATCH_L1_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's l1
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_l1_commands( void )
{
  
  tdsrrc_cmd_type *cmd_ptr;   /* To hold received command */

  tdsrrc_csp_scan_e_type csp_curr_scan = tdsrrccsp_get_curr_scan();

  /* We need to dequeue items from the l1
  command queue and process them till the queue is
  empty */
  while( (cmd_ptr = q_get(&tdsrrc_l1_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif
    /* Check if the incoming internal command is required a further process */
    if (tdsrrcmcm_is_process_required(cmd_ptr) == FALSE) 
    {
      /* Display a XDM message and disgard the command */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMD: %d is disgarded!",cmd_ptr->cmd_hdr.cmd_id);
    }
    else if(tdsrrc_deact_status != TDSRRC_DEACT_COMPLETED)
    {
      /* Switch on the command id of the received command, and
       * call the appropriate function to process each command. 
       */
      switch( cmd_ptr->cmd_hdr.cmd_id )
      {
        case TDSRRC_CPHY_ACQ_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_ACQ_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_FREQ_SCAN_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_FREQ_SCAN_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_SETUP_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_SETUP_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_CELL_SELECTION_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_CELL_SELECTION_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_CELL_TRANSITION_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching RRC_CPHY_TRANSITION_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;


        case TDSRRC_CPHY_PHYCHAN_ESTABLISHED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_PHYCHAN_ESTABLISHED_IND to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_OUT_OF_SYNC_IND:
          /* TBD: This command might have to go to cell selection later */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_OUT_OF_SYNC_IND to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_MEASUREMENT_IND:
          /* TBD: This command might have to go to cell selection later */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_MEASUREMENT_IND to LLC");
          tdsrrcmeas_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_CPHY_OUT_OF_SERVICE_AREA_IND:
          /*TBD: Is this really needed*/
          if(cmd_ptr->cmd.oos_ind.oos_cause == TDSL1_FAILED_NO_TRM_LOCK)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_OUT_OF_SERVICE_AREA_IND to CSP");
            tdsrrccsp_cell_selection_procedure(cmd_ptr);
          }
          else
          {
            if(tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED || 
               tdsrrc_get_state() == TDSRRC_STATE_CONNECTING)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_OUT_OF_SERVICE_AREA_IND to RCE");
              tdsrrcrce_procedure_event_handler( cmd_ptr );
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_OUT_OF_SERVICE_AREA_IND to CU");
              tdsrrccu_procedure_event_handler( cmd_ptr );
            }
          }
#ifdef FEATURE_QSH_EVENT_METRIC
          TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_OOS);
          tdsrrcqsh_update_rlf_or_weak_signal_failure(SYS_RLF_WEAK_SIGNAL_OTHER);
#endif
          break;

        case TDSRRC_CPHY_DRX_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_DRX_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_RL_FAILURE_IND:
          /* FR19720 */
          if (!tdsrrcmisc_check_da_backoff())
          {
            if ((TDSOC_SET_FOR_DCH_FACH_TRANS == tdsrrcllc_get_ordered_config_status()) )
            {
              /* UE will go to fach thru cell selection anyways. Ingore*/
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Ignoring RL failure ind from L1");
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_RL_FAILURE_IND to CU"); 
              tdsrrccu_procedure_event_handler( cmd_ptr );
#ifdef FEATURE_QSH_EVENT_METRIC
              TDSRRC_QSH_METRIC_UPDATE(TDSRRC_QSH_RLF);
              tdsrrcqsh_update_rlf_or_weak_signal_failure(SYS_RLF_WEAK_SIGNAL_RLF_TIMER_EXP);
#endif
            }
          }
          break;

        case TDSRRC_CPHY_DEACTIVATE_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_DEACTIVATE_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_MEAS_CTRL_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_MEAS_CTRL_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_CELL_RESELECTION_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_CELL_RESELECTION_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr);
          break;

      
        case RRC_CPHY_START_TDS_MODE_CNF:

          /* For OOS_Scan the start_cnf needs to go to LLC so that LLC can send
           * the commands in the queue (idle request, acq request, etc.) to L1.
           * This is because when the deep sleep timer expires, CSP sends wake up
           * request to LLC followed by idle/acq requests. LLC on receiving the wakeup
           * req sends start_req to L1 and waits for the confirmation from L1 before
           * sending idle/acq req to L1. That is why in this case start_cnf needs to
           * be routed to LLC instead of MCM.
           *
           * We need to check for leaving connected mode scan also because in the case
           * where UE is in deep sleep and T305+T307/T314/T315/etc. expire due to which UE needs to
           * be taken to Idle, CSP on receiving tx to disconnected, will first send wake
           * up request to LLC and then call the function start_sel_leaving_conn_mode
           * so that UE can be taken to Idle. As part of this fn, the curr_scan is changed
           * from OOS_Scan to leaving connected mode scan. Hence even for leaving conn
           * mode scan the start_cnf needs to be sent to LLC instead of MCM.
           */
          if(   (csp_curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
             || (csp_curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)
            )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching RRC_CPHY_START_TDS_MODE_CNF to LLC");
            tdsrrcllc_process_cmd(cmd_ptr);
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching RRC_CPHY_START_TDS_MODE_CNF to MCM");
            tdsrrcmcm_event_handler( cmd_ptr );
          }


          break;

        case RRC_CPHY_STOP_TDS_MODE_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching RRC_CPHY_STOP_TDS_MODE_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case RRC_CPHY_SUSPEND_TDS_MODE_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching RRC_CPHY_SUSPEND_TDS_MODE_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CPHY_RESUME_TDS_MODE_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_RESUME_TDS_MODE_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;
        case TDSRRC_CPHY_ERROR_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_ERROR_IND to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;


        case TDSRRC_CPHY_DL_WEAK_IND:
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
          if( (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH) && (tdsrrcsib_is_current_event_sib_read_in_dch()))
          {
            tdsrrc_proc_e_type proc_id = tdsrrcsib_get_proc_id_for_sib_read_in_dch();
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
            if(proc_id == TDSRRC_PROCEDURE_GPS)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_DL_WEAK_IND to GPS ");
              tdsrrcgps_procedure_event_handler( cmd_ptr );
            }
            else 
#endif
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> L1->RRC: Invalid proc id %d for Cell DCH SIB reading",proc_id);
            }
          }
          else
#endif /*FEATURE_TDSCDMA_GPS_CELL_ID_READING*/
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_DL_WEAK_IND to CSP");
            tdsrrccsp_cell_selection_procedure( cmd_ptr);
          }
          break;
        
        case TDSRRC_CPHY_ACT_CANCEL_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_ACT_CANCEL_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;


#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK

        case TDSRRC_CPHY_MODEM_STATISTICS_RSP:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"FF: Dispatching RRC_MODEM_STATISTICS_RSP to rrcdata");
          tdsrrc_process_modem_stat_info( cmd_ptr );
          break;

#endif
#endif /*FEATURE_TDSCDMA_MODEM_STATISTICS*/

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
        case TDSRRC_CPHY_NUDGE_FOR_SFN_MISS_MATCH_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"FF: Dispatching RRC_NUDGE_FOR_SFN_MISS_MATCH_REQ to RRCPG1");
          tdsrrcpg1_procedure_event_handler( cmd_ptr );
          break;
#endif

          case TDSRRC_CPHY_ACTIVATION_TIME_IND:
#ifdef FEATURE_TDSCDMA_RRC_INDICATE_CODEC_TO_CM
          tdsrrc_send_codec_info_on_activation_timer_expiry();
#endif
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_ACTIVATION_TIME_IND to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;
#ifdef FEATURE_TDSCDMA_FATAL_RECOVERY 
        case TDSRRC_CPHY_HW_ERROR_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_HW_ERROR_IND to MISC");

          /* This command doesn't have payload */
          tdsrrcmisc_procss_hw_error_ind();
          break;
#endif

        case TDSRRC_CPHY_G2T_MODE_IND:
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> L1->RRC: Dispatching TDSRRC_CPHY_G2T_MODE_IND to SIB");
           tdsrrcsib_event_controller( cmd_ptr );
           break;
        default:
          /* This should not happen. Possibly the command id was never written 
          before this item was put in the queue. Display error for now. */ 
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> L1->RRC: Unknown l1 cmd rcvd by RRC 0x%x",cmd_ptr->cmd_hdr.cmd_id);
          break;
      }/* end switch */
    }/* end if */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Throw away the L1 command");
    }
    /* The command has been processed. We can free the memory for this cmd */
#ifdef FEATURE_TDSCDMA_MEM_ALLOC_FREE_COUNTER
    tdsfreeL1cmdCt++; 
#endif

    tdsrrc_free_cmd_buf( cmd_ptr );

  }/* end while */
}/* end function */


/*===========================================================================

FUNCTION  RRC_DISPATCH_MAC_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's mac
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_mac_commands( void )
{
  
  tdsrrc_cmd_type *cmd_ptr;  /* To hold received command */

  /* We need to dequeue items from the mac
  command queue and process them till the queue is
  empty */

  while( (cmd_ptr = q_get(&tdsrrc_mac_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif
    /* Check if the incoming internal command is required a further process */
    if (tdsrrcmcm_is_process_required(cmd_ptr) == FALSE) 
    {
      /* Display a XDM message and disgard the command */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMD: %d is disgarded!",cmd_ptr->cmd_hdr.cmd_id);
    }
    else if(tdsrrc_deact_status != TDSRRC_DEACT_COMPLETED)
    {
      /* Switch on the command id of the received command, and
       * call the appropriate function to process each command. 
       */
      switch( cmd_ptr->cmd_hdr.cmd_id )
      {
        case TDSRRC_CMAC_UL_TRAFFIC_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MAC->RRC: Dispatching TDSRRC_CMAC_UL_TRAFFIC_IND to MEAS");
          tdsrrcmeas_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_CMAC_STATUS_IND:
          /* FR19720 MMAX reached for RACH only */
          if ((((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind == TDSMAX_RACH_CYCLES_REACHED)
              && tdsrrcmisc_check_da_backoff())
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"<IFTDSRRC> MAC->RRC: Receive CMAC_STATUS_IND for TDSMAX_RACH_CYCLES_REACHED,rrc state %d",((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind,tdsrrc_get_state());
          }
          else
          {
          
             if(tdsrrcrce_get_rce_sub_state() == TDSRRCRCE_WAIT_FOR_RRC_CONN_SETUP)
             {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MAC->RRC: Dispatching TDSRRC_CMAC_STATUS_IND to RCE"); 
               tdsrrcrce_procedure_event_handler( cmd_ptr );   
             }
#ifdef FEATURE_TDSCDMA_HSUPA
             /* 25.331-8.5.6, If one of the following criteria for E-RUCCH transmission are met, failure indication of the
                        E-RUCCH transmission is sent to RRC by the CMAC-STATUS-Ind primitive with E-RUCCH failure:
                          -     The maximum number Mmax of synchronisation attempts is reached;
                          -     When the hysteresis timer with the value of N-RUCCH times of T-RUCCH period, which is started 
                                 when the the E-RUCCH transmission counter is reached to N_RUCCH, is expired. */
             else if((((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind == TDSMAX_ERUCCH_CYCLES_REACHED) ||
                 (((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind == TDSERUCCH_HYST_TMR_EXP))
             {
               /*Check if CCO is on-going regardless CHO internal state. If CCO is active, discard the E-RUCCH failure */
               if(TRUE == tdsrrccho_is_td2g_cco_active())
               {
                 TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> MAC->RRC: CMAC_STATUS_IND E-RUCCH faliure is dropped due to CCO active,cause is %d",((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind);

               }
               else
               {
                 TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> MAC->RRC: CMAC_STATUS_IND E-RUCCH faliure is sending to CU,cause is %d",((tdsrrc_mac_cmd_type *)cmd_ptr)->cmd.cmac_status_ind);
                 tdsrrccu_procedure_event_handler( cmd_ptr );
               }
             }
#endif
             else
             {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MAC->RRC: Dispatching TDSRRC_CMAC_STATUS_IND to LLC");
               tdsrrcllc_process_cmd( cmd_ptr );
             }
          }
          break;

        case TDSRRC_CMAC_MEAS_CTRL_CNF:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MAC->RRC: Dispatching TDSRRC_CMAC_MEAS_CTRL_CNF to MEAS");
          tdsrrcmeas_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_CMAC_UL_TFC_SUBSET_CFG_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MAC->RRC: Dispatching TDSRRC_CMAC_UL_TFC_SUBSET_CFG_CNF to TFCC");
          tdsrrctfcc_procedure_event_handler( cmd_ptr );
          break;


        default:
          /* This should not happen. Possibly the command id was never written 
          before this item was put in the queue. Display error for now. */ 
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> MAC->RRC: Unknown mac cmd rcvd by RRC 0x%x",cmd_ptr->cmd_hdr.cmd_id);
          break;
      }/* end switch */
    }/* end if */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Throw away the MAC command");
    }
    /* The command has been processed. We can free the memory for this cmd */
    tdsrrc_free_cmd_buf( cmd_ptr );
  }/* end while */
}/* end function */



/*===========================================================================

FUNCTION  RRC_DISPATCH_RLC_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's rlc
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_rlc_commands( void )
{
  
  tdsrrc_cmd_type *cmd_ptr;  /* To hold received command */

  /* We need to dequeue items from the rlc
  command queue and process them till the queue is
  empty */
  
  while( (cmd_ptr = tdsrrc_check_and_get_cmd(&tdsrrc_rlc_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif
    /* Check if the incoming internal command is required a further process */
    if (tdsrrcmcm_is_process_required(cmd_ptr) == FALSE) 
    {
      /* Display a XDM message and disgard the command */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMD: %d is disgarded!",cmd_ptr->cmd_hdr.cmd_id);
    }
    else if(tdsrrc_deact_status != TDSRRC_DEACT_COMPLETED)
    {
      /* Switch on the command id of the received command, and
       * call the appropriate function to process each command. 
       */
      switch( cmd_ptr->cmd_hdr.cmd_id )
      {
        case TDSRRC_CRLC_DL_TM_CONFIG_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RLC->RRC: Dispatching TDSRRC_CRLC_DL_TM_CONFIG_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CRLC_UL_TM_CONFIG_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RLC->RRC: Dispatching TDSRRC_CRLC_UL_TM_CONFIG_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CRLC_DL_UM_CONFIG_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RLC->RRC: Dispatching TDSRRC_CRLC_DL_UM_CONFIG_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CRLC_UL_UM_CONFIG_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RLC->RRC: Dispatching TDSRRC_CRLC_UL_UM_CONFIG_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CRLC_AM_CONFIG_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RLC->RRC: Dispatching TDSRRC_CRLC_AM_CONFIG_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        case TDSRRC_CRLC_STATUS_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RLC->RRC: Dispatching TDSRRC_CRLC_STATUS_IND to CU");
          tdsrrccu_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_CRLC_RELEASE_ALL_CNF:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RLC->RRC: Dispatching TDSRRC_CRLC_RELEASE_ALL_CNF to LLC");
          tdsrrcllc_process_cmd( cmd_ptr );
          break;

        default:
          /* This should not happen. Possibly the command id was never written 
          before this item was put in the queue. Display error for now. */ 
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> RLC->RRC: Unknown rlc cmd rcvd by RRC 0x%x",cmd_ptr->cmd_hdr.cmd_id);
          break;
      }/* end switch */
    } /* end if */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Throw away the RLC command");
    }
    /* The command has been processed. We can free the memory for this cmd */
    tdsrrc_free_cmd_buf( cmd_ptr );
    
  }/* end while */
}/* end function */



/*===========================================================================

FUNCTION  RRC_DISPATCH_MM_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's mm
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_mm_commands( void )
{
  
  tdsrrc_cmd_type *cmd_ptr;   /* To hold received command */
  #ifdef FEATURE_WRLF_SYSTEM_SEL
  size_t data_size;
  #endif
  #ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id,curr_sub_id;
  #endif
  /* We need to dequeue items from the mm
  command queue and process them till the queue is
  empty */

  TDSRRC_MSG0(MSG_LEGACY_MED,"Dispatching MM Commands");
  while( (cmd_ptr = q_get(&tdsrrc_mm_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif
    /* Check if the incoming internal command is required a further process */
    if (tdsrrcmcm_is_process_required(cmd_ptr) == FALSE) 
    {
      /* Display a XDM message and disgard the command */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMD: %d is disgarded!",cmd_ptr->cmd_hdr.cmd_id);
    }
    else if(tdsrrc_deact_status == TDSRRC_NORMAL_OPERATION)
    {
      /* Switch on the command id of the received command, and
       * call the appropriate function to process each command. 
       */
#ifdef FEATURE_DUAL_SIM
      if(tdsrrcdispatch_check_mm_cmds_for_simid_validation(cmd_ptr) == FAILURE)
      {
        if ((cmd_ptr->cmd_hdr.cmd_id == RRC_STOP_TDS_MODE_REQ)||
            (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_DEACT_REQ) ||
            (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_MODE_CHANGE_REQ))
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR, "DSIM: Invalid AS ID in MM cmd %x",cmd_ptr->cmd_hdr.cmd_id);
        }
        else
        {
          ERR_FATAL("DSIM: Invalid AS ID in MM cmd %x",cmd_ptr->cmd_hdr.cmd_id,0,0);
        }
      }
#endif
      switch( cmd_ptr->cmd_hdr.cmd_id )
      {
        case TDSRRC_SERVICE_REQ:
#ifdef FEATURE_WRLF_SYSTEM_SEL
			   /* CR430034 RAU Reject workaround, delay processing service request under OOS/RLF */
		  if ( TDSCHECK_OPTIONAL_FEATURE_CMCC_RLF_SKIP_SVCREQ
              && (tdsrrc_csp_int_data.rlf_skip_first_svc_req == TRUE)
              && (tds_nv_delay_oos_ind_timer > 0) )
		  {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: CR430034 handling of TDSRRC_SERVICE_REQ");
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"rlf_skip_first_svc_req flag is set,buffer received TDSRRC_SERVICE_REQ. CR430034");      
           data_size = memscpy(&tdsrrc_csp_int_data.stored_svc_req_for_delay_rlf_rpt,
                               sizeof(tdsrrc_cmd_type),
                               cmd_ptr, 
                               sizeof(tdsrrc_mm_cmd_type));  /*The cmd_ptr is tdsrrc_mm_cmd_type*/

 	        tdsrrctmr_start_timer(TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER, tds_nv_delay_oos_ind_timer*1000UL);
           TDSRRC_MSG2(MSG_LEGACY_HIGH,"CR430034 TDSRRCTMR_DELAY_OOS_RLF_RPT_TIMER is started for %d seconds,cmd_size = %d",tds_nv_delay_oos_ind_timer,data_size);
		  }
		  else
#endif
		  {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_SERVICE_REQ to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
		  }
          break;

        case RRC_STOP_TDS_MODE_REQ:
		  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received RRC_STOP_TDS_MODE_REQ with Cause %d",cmd_ptr->cmd.stop_wcdma_mode_req.cause);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching RRC_STOP_TDS_MODE_REQ to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

        case TDSRRC_DEACT_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_DEACT_REQ to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

        case TDSRRC_MODE_CHANGE_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_MODE_CHANGE_REQ to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

        case TDSRRC_CHANGE_MODE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_CHANGE_MODE_IND to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

        case TDSRRC_ACT_REQ:
        case TDSRRC_FORBIDDEN_LAI_LIST_UPDATE_REQ:
          if(TDSRRC_ACT_REQ == cmd_ptr->cmd_hdr.cmd_id)
          {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_ACT_REQ to CSP");
          }
          else
          {
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_FORBIDDEN_LAI_LIST_UPDATE_REQ to CSP");
          }
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_PLMN_LIST_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_PLMN_LIST_REQ to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

        case TDSRRC_BPLMN_SEARCH_ABORT_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_BPLMN_SEARCH_ABORT_REQ to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
        
        case TDSRRC_EST_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_EST_REQ to RCE");
          tdsrrcrce_procedure_event_handler( cmd_ptr );
          break;
        
        case TDSRRC_ABORT_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_ABORT_REQ to IHO");
          tdsrrciho_event_handler( cmd_ptr );
          break;

        case TDSRRC_OPEN_SESSION_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_OPEN_SESSION_REQ to IDT");
          tdsrrcidt_procedure_event_handler( cmd_ptr );
          break;
        
        case TDSRRC_DATA_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_DATA_REQ to UDT");
          tdsrrcudt_procedure_event_handler( cmd_ptr );
          break;
        
        case TDSRRC_CLOSE_SESSION_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_CLOSE_SESSION_REQ to SCRR");
          tdsrrcscrr_procedure_event_handler( cmd_ptr );
          break;
        
        case TDSRRC_SIM_INSERTED_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_SIM_INSERTED_REQ to MISC");
          tdsrrcmisc_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_SIM_UPDATE_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_SIM_UPDATE_REQ to MISC");
          tdsrrcmisc_procedure_event_handler( cmd_ptr );
          break;

        case TDSRRC_ACTIVATION_RSP:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_ACTIVATION_RSP to MCM");
          tdsrrcmcm_event_handler( cmd_ptr );
          break;

      case TDSRRC_EQ_PLMN_LIST_CHANGE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_EQ_PLMN_LIST_CHANGE_IND to MEAS and CSP");
          tdsrrcmeas_procedure_event_handler( cmd_ptr );
          tdsrrccsp_cell_selection_procedure( cmd_ptr );

        break;
        case TDSRRC_NW_SEL_MODE_RESET_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_NW_SEL_MODE_RESET_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

#ifdef FEATURE_OOSC_USER_ACTION
        case TDSRRC_EXIT_FROM_PWR_SAVE:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_EXIT_FROM_PWR_SAVE to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
#endif /*FEATURE_OOSC_USER_ACTION */

        case TDSRRC_DS_STATUS_CHANGE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_DS_STATUS_CHANGE_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;
        case TDSRRC_DS_TUNEAWAY_STATUS_CHANGE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_DS_TUNEAWAY_STATUS_CHANGE_IND to CSP");
          tdsrrccsp_cell_selection_procedure( cmd_ptr );
          break;

        case TDSRRC_FAST_DORMANCY_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: Dispatching TDSRRC_FAST_DORMANCY_REQ to DORMANCY");
          tdsrrcdormancy_procedure_event_handler( cmd_ptr );
          break;

#ifdef FEATURE_3GPP_CSFB_TDS
        case TDSRRC_CSFB_CALL_STATUS_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: RRC_CSFB_CALL_STATUS_IND to CSP");
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;
#endif

        case TDSRRC_LTE_RESELECTION_ALLOWED_STATUS_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: TDSRRC_LTE_RESELECTION_ALLOWED_STATUS_IND to CSP");
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;

        case TDSRRC_INVALIDATE_SIM_DATA_REQ:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: TDSRRC_INVALIDATE_SIM_DATA_REQ received. Reset IMSI.");
          /* Resetting the IMSI */
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
          sub_id = cmd_ptr->cmd.rrc_subscription_id.as_id;
          if((sub_id < SYS_MODEM_AS_ID_1) || (sub_id >= (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS))
          {
          	TDSRRC_MSG1(MSG_LEGACY_MED, "TRRC: invalid sub_id %d", sub_id);
          	return;
          }
          curr_sub_id = tdsrrc_get_as_id();
          tdsrrc_set_identity_info_to_related_sub_params(curr_sub_id);
          tdsrrc_set_identity_info_to_global_params(sub_id);
#endif
          memset(&tdsrrc_imsi, 0, sizeof(rrc_imsi_gsm_map_type));
          tdsrrc_imsi_valid = FALSE;
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
          tdsrrc_set_identity_info_to_related_sub_params(sub_id);
          tdsrrc_set_identity_info_to_global_params(curr_sub_id);
#endif
          break;

        case TDSRRC_PSEUDO_LTE_LIST_UPDATE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: TDSRRC_PSEUDO_LTE_LIST_UPDATE_IND to MEAS");
          tdsrrcmeas_process_pseudo_lte_list_update_ind(cmd_ptr);
          break;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
        case TDSRRC_LAI_REJECT_LIST_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: FF: RRC_LAI_REJECT_LIST_IND Recvd");
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;
#endif

#ifdef FEATURE_SGLTE
        case TDSRRC_UE_MODE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: SGLTE,TDSRRC_UE_MODE_IND Recvd");
          tdsrrccsp_cell_selection_procedure(cmd_ptr);
          break;
#endif
#ifdef FEATURE_CMCC_SECURE_CALL
        case TDSRRC_SECURE_CALL_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: TDSRRC_SECURE_CALL_IND Recvd"); 
	  	  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Secure call: secure status is %d ",cmd_ptr->cmd.rrc_secure_call_ind.secure_voice_status); 
          if (cmd_ptr->cmd.rrc_secure_call_ind.secure_voice_status == RRC_SECURE_VOICE_ENABLE)   
          {
            tdsrrcmcm_update_secure_call_status(TRUE);
          }
          else
          {
            tdsrrcmcm_update_secure_call_status(FALSE);
          }
        break;
#endif		
        case TDSRRC_PROCEDURE_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: TDSRRC_PROCEDURE_IND Recvd"); 
          tdsrrcmisc_procedure_event_handler(cmd_ptr);
          break;
        case TDSRRC_BARRING_INFO_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> MM->RRC: TDSRRC_BARRING_INFO_IND Recvd");    
          tdsrrccsp_cell_selection_procedure(cmd_ptr);          
          break;		  
        default:
          /* This should not happen. Possibly the command id was never written 
          before this item was put in the queue. Display error for now. */ 
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> MM->RRC: Unknown mm cmd rcvd by RRC 0x%x",cmd_ptr->cmd_hdr.cmd_id);
          break;
      }/* end switch */
    }/* end if */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Throw away the MM command");
    }
    /* The command has been processed. We can free the memory for this cmd */
    tdsrrc_free_cmd_buf(cmd_ptr);
  }/* end while */
}/* end function */


/*===========================================================================

FUNCTION  RRC_DISPATCH_TC_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's mm
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrcdispatch_dispatch_tc_commands( void )
{
  
  tdsrrc_cmd_type *cmd_ptr;   /* To hold received command */

  /* We need to dequeue items from the mm
  command queue and process them till the queue is
  empty */

  TDSRRC_MSG0(MSG_LEGACY_MED,"Dispatching TC Commands");
  while( (cmd_ptr = q_get(&tdsrrc_tc_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif
    /* Check if the incoming internal command is required a further process */
    if (tdsrrcmcm_is_process_required(cmd_ptr) == FALSE) 
    {
      /* Display a XDM message and disgard the command */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMD: %d is disgarded!",cmd_ptr->cmd_hdr.cmd_id);
    }
    else if(tdsrrc_deact_status == TDSRRC_NORMAL_OPERATION)
    {
#if defined(FEATURE_DUAL_WCDMA)&& defined(FEATURE_DUAL_SIM)
      if(tdsrrcdispatch_check_tc_cmds_for_simid_validation(cmd_ptr) == FAILURE)
      {
        ERR_FATAL("DSIM: Invalid SIM ID in TC cmd %x",cmd_ptr->cmd_hdr.cmd_id,0,0);
      }
#endif
      tdsrrclbt_procedure_event_handler(cmd_ptr);
    }/* end if */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Throw away the TC command");
    }

    /* The command has been processed. We can free the memory for this cmd */
    tdsrrc_free_tc_cmd_buf( (tdsrrc_tc_cmd_type *)cmd_ptr );
  }/* end while */

}/* end function */

#if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA)
/*===========================================================================

FUNCTION  tdsrrc_check_wrrc_cmds_for_simid_validation

DESCRIPTION
 
  This function validates SIM id for WRRC commands.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None
  
SIDE EFFECTS

  None.

===========================================================================*/
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
uecomdef_status_e_type tdsrrc_check_wrrc_cmds_for_simid_validation(tdsrrc_rrc_cmd_type *cmd_ptr)
{
  sys_modem_as_id_e_type sim_id_recvd;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case RRC_TDSRRC_STOP_WCDMA_CNF:
      sim_id_recvd = cmd_ptr->cmd.rrc_tdsrrc_stop_wcdma_cnf.as_id;
      break;
				
    case TDSRRC_RRC_STOP_TDS_REQ:
      sim_id_recvd = cmd_ptr->cmd.tdsrrc_rrc_stop_tds_req.as_id;
      break;
		
    default:
      /* Unexpected WRRC command */ 			
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> WRRC->RRC: Unknown WRRC Command: %d",cmd_ptr->cmd_hdr.cmd_id);
      return FAILURE;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: sub-id in WRRC cmd: %d",(int)sim_id_recvd);
  

  if((!tdsrrc_is_sim_id_valid)||
    (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_RRC_STOP_TDS_REQ))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: RRC not yet updated with SIM details or STOP_TDS_REQ,Treat as SIM Match");
    return SUCCESS;
  }
  
  if(tdsrrc_sim_id_recvd != sim_id_recvd)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid sub-id %d passed. internal %d",(int)sim_id_recvd,(int)tdsrrc_sim_id_recvd);
    return FAILURE;
  }
  return SUCCESS;
}
#else
static void tdsrrc_check_wrrc_cmds_for_simid_validation(tdsrrc_rrc_cmd_type *cmd_ptr)
{
  sys_modem_as_id_e_type sim_id_recvd;

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case RRC_TDSRRC_STOP_WCDMA_CNF:
      sim_id_recvd = cmd_ptr->cmd.rrc_tdsrrc_stop_wcdma_cnf.as_id;
      break;
				
    case TDSRRC_RRC_STOP_TDS_REQ:
      sim_id_recvd = cmd_ptr->cmd.tdsrrc_rrc_stop_tds_req.as_id;
      break;
		
    default:
      /* Unexpected WRRC command */ 			
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> WRRC->RRC: Unknown WRRC Command: %d",cmd_ptr->cmd_hdr.cmd_id);
      return;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: sub-id in WRRC cmd: %d",(int)sim_id_recvd);

  if(!tdsrrc_is_sim_id_valid)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: RRC not yet updated with SIM details,Treat as SIM Match");
    return;
  }
  
  if(tdsrrc_sim_id_recvd != sim_id_recvd)
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid sub-id %d passed. internal %d",(int)sim_id_recvd,(int)tdsrrc_sim_id_recvd);
    return;
  }
  return;
}
#endif

/*===========================================================================

FUNCTION  TDSRRC_DISPATCH_WRRC_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's 
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/

static void tdsrrcdispatch_dispatch_wrrc_commands(void)
{
  
  tdsrrc_rrc_cmd_type *cmd_ptr;   /* To hold received command */

   /* We need to dequeue items from the WRRC
   command queue and process them till the queue is
   empty */

   while( (cmd_ptr = q_get(&tdsrrc_rrc_cmd_q)) != NULL)
   {
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
      if (tdsrrc_check_wrrc_cmds_for_simid_validation(cmd_ptr) == FAILURE)
      {
        ERR_FATAL("DSIM: Invalid SIM ID in WCDMA cmd %x",cmd_ptr->cmd_hdr.cmd_id,0,0);
      }
#else
      tdsrrc_check_wrrc_cmds_for_simid_validation(cmd_ptr);
#endif
#endif      
      /* Check if the incoming internal command is required a further process */
      if (tdsrrcmcm_is_process_required((tdsrrc_cmd_type *)cmd_ptr) == FALSE)
      {
         /* Display a XDM message and disgard the command */
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> WRRC->RRC: CMD %d is discarded!",cmd_ptr->cmd_hdr.cmd_id);
      }
      else 
      {
         /* WRRC -> RRC uses different command type. Header structure is the same */
         if(tdsrrc_deact_status == TDSRRC_NORMAL_OPERATION)
         {
            /* Switch on the command id of the received command, and
               call the appropriate function to process each command. */
            switch(cmd_ptr->cmd_hdr.cmd_id)
            {
               case RRC_TDSRRC_STOP_WCDMA_CNF:
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> WRRC->RRC: Dispatching RRC_TDSRRC_STOP_WCDMA_CNF to CSP,TDSRRC_NORMAL_OPERATION");
                  tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *)cmd_ptr);
                  break;

               default:
                  /* Unexpected WRRC command */       
                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> WRRC->RRC: Unknown WRRC Command: %d",cmd_ptr->cmd_hdr.cmd_id);
                  break;
            }
         }   
         else if((tdsrrc_deact_status == TDSRRC_DEACT_COMPLETED)
                  && (cmd_ptr->cmd_hdr.cmd_id == RRC_TDSRRC_STOP_WCDMA_CNF))
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> WRRC->RRC: Dispatching RRC_TDSRRC_STOP_WCDMA_CNF to CSP,TDSRRC_DEACT_COMPLETED");
            tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *)cmd_ptr);
         }
         else
         {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> WRRC->RRC: Throw away the WRRC command: %d",cmd_ptr->cmd_hdr.cmd_id);
         }
      }
      /* The command has been processed. We can free the memory for this cmd */
     tdsrrc_free(cmd_ptr);
   }
}/* end function */
#endif /* #if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA) */


/*===========================================================================

FUNCTION  RRC_DISPATCH_RR_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's RR
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcdispatch_rr_commands( void )
{
  
  tdsrrc_rr_cmd_type *cmd_ptr;   /* To hold received command */

  /* We need to dequeue items from the RR
  command queue and process them till the queue is
  empty */

  TDSRRC_MSG0(MSG_LEGACY_MED,"Dispatching RR Commands");

  while( (cmd_ptr = q_get(&tdsrrc_rr_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif
    /*lint -e740*/
    /* Check if the incoming internal command is required a further process */
    if (tdsrrcmcm_is_process_required((tdsrrc_cmd_type *)cmd_ptr) == FALSE) 
    {
      /* Display a XDM message and disgard the command */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMD: %d is disgarded!",cmd_ptr->cmd_hdr.cmd_id);
    }
    /*lint +e740*/
    else
    {
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    if (tdsrrc_check_rr_cmds_for_simid_validation(cmd_ptr) == FAILURE)
    {
      ERR_FATAL("DSIM: Invalid SIM ID in RR cmd %x",cmd_ptr->cmd_hdr.cmd_id,0,0);
    }
#else
    tdsrrc_check_rr_cmds_for_simid_validation(cmd_ptr);
#endif
#endif
      if(tdsrrc_deact_status == TDSRRC_NORMAL_OPERATION)
      {
        /* Switch on the command id of the received command, and
           call the appropriate function to process each command. */
        switch(cmd_ptr->cmd_hdr.cmd_id)
        {
          case TDSRR_INTERRAT_RESELECTION_REJ:
          case TDSRR_INTERRAT_RESELECTION_ABORT_CNF:
            /* Forward to CSP to process the cmd */
            /*lint -e740*/
            if(TDSRR_INTERRAT_RESELECTION_REJ == cmd_ptr->cmd_hdr.cmd_id)
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_RESELECTION_REJ to CSP");
            }
            else
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_RESELECTION_ABORT_CNF to CSP");
            }
            tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *)cmd_ptr);
            /*lint +e740*/
            break;

          case TDSRR_INTERRAT_CC_ORDER_REJ:
            /* Forward to CHO to process the cmd */
            /*lint -e740*/
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_CC_ORDER_REJ to CHO");
            tdsrrccho_procedure_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            /*lint +e740*/
            break;

          case TDSRR_INTERRAT_CC_ORDER_CNF:
            /* Forward to CHO to process the cmd */
            /*lint -e740*/
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_CC_ORDER_CNF to CHO");
            tdsrrccho_procedure_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            /*lint +e740*/
            break;
          case TDSRR_INTERRAT_CC_ORDER_ABORT_CNF:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_CC_ORDER_ABORT_CNF to CHO");
            tdsrrccho_procedure_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            break;   
          case TDSRRC_GRR_INTERRAT_PROC_ABORT_IND:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_GRR_INTERRAT_PROC_ABORT_IND to CHO");
            tdsrrccho_procedure_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            break;
          case TDSRRC_INTERRAT_RESELECTION_REQ:
          case TDSRRC_INTERRAT_RESELECTION_ABORT_REQ:
            /* Forward the command to MCM for processing */
            /*lint -e740*/
            if(TDSRRC_INTERRAT_RESELECTION_REQ == cmd_ptr->cmd_hdr.cmd_id)
            {

                TDSRRC_DEBUG_INCREMENT_STATS(G2TD_CEL_RESEL_TRIGGERED);
               /* Validity check on CPId */
               if (cmd_ptr->cmd.interrat_reselection_req.cpid >= TDSRRC_CSP_INVALID_CPID)
               {
                  tdsrrc_interrat_reselect_rej_type rrc_irat_reselect_rej;

                  memset(&rrc_irat_reselect_rej,0,sizeof(tdsrrc_interrat_reselect_rej_type));

                  TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> RR->RRC: TDSRRC_INTERRAT_RESELECTION_REQ with invalid CPID %d",cmd_ptr->cmd.interrat_reselection_req.cpid);
    
                  rrc_irat_reselect_rej.reject_reason    = TDSRRC_UMTS_ACQUISITION_FAIL;
                   /* Time ( in Sec ) when the same cell should be considered */
                  rrc_irat_reselect_rej.time_to_reselect = 1280;
    
                  /* Send the RRC_INTERRAT_RESELECT_REJ message to RR */
                  tdsrr_send_rrc_message_to_rr_internal((void *)(&(rrc_irat_reselect_rej)), TDSRRC_INTERRAT_RESELECTION_REJ,
                          sizeof(tdsrrc_interrat_reselect_rej_type));
                  TDSRRC_DEBUG_INCREMENT_STATS(G2TD_CEL_RESEL_FAILURE);
                  TDSRRC_DEBUG_INCREMENT_STATS(G2TD_CEL_RESEL_FAILURE_ACQ_FAILURE);
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_INTERRAT_RESELECT_REJ sent to GSM RR");
                  
                  break;
               }
               
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_RESELECTION_REQ to MCM");
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_RESELECTION_ABORT_REQ to MCM");
            }
            tdsrrcmcm_update_dual_mode_enabled_status(TRUE);
            tdsrrcmcm_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            /*lint +e740*/
            break;
    
          case TDSRR_INTERRAT_HANDOVER_CNF:
          case TDSRR_INTERRAT_HANDOVER_ABORT_CNF:
    
#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
          case TDSRRC_INTERRAT_HANDOVER_ABORT_REQ:
          case TDSRRC_INTERRAT_HANDOVER_REQ:
#endif
            if(TDSRR_INTERRAT_HANDOVER_CNF == cmd_ptr->cmd_hdr.cmd_id)
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_HANDOVER_CNF to MCM and IHO");
            }
            else if(TDSRR_INTERRAT_HANDOVER_ABORT_CNF == cmd_ptr->cmd_hdr.cmd_id)
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_HANDOVER_ABORT_CNF to MCM and IHO");
            }
#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
            else if(TDSRRC_INTERRAT_HANDOVER_ABORT_REQ == cmd_ptr->cmd_hdr.cmd_id)
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_HANDOVER_ABORT_REQ to MCM and IHO");
            }
            else if(TDSRRC_INTERRAT_HANDOVER_REQ == cmd_ptr->cmd_hdr.cmd_id)
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_HANDOVER_REQ to MCM and IHO");
            }
#endif
            else
            {
            }
            /* forward the reset commands to IHO for now */ 
            /*lint -e740*/
            tdsrrcmcm_update_dual_mode_enabled_status(TRUE);
            tdsrrciho_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            /*lint +e740*/
            break;

#if defined(FEATURE_INTERRAT_HANDOVER_G2TD) || defined(FEATURE_TDSCDMA_L2TD_PS_HANDOVER)
          case TDSRRC_NAS_DATA_IND:
            /*lint -e740*/
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_NAS_DATA_IND to UDT");
            tdsrrcudt_procedure_event_handler((tdsrrc_cmd_type *) cmd_ptr );
            /*lint +e740*/
            break;
#endif

#ifdef FEATURE_INTERRAT_PCCO_G2TD
           case TDSRRC_INTERRAT_CC_ORDER_REQ:
           case TDSRRC_INTERRAT_CC_ORDER_ABORT_REQ:
            /*lint -e740*/
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_CC_ORDER_REQ or ABORT to CHO");
            tdsrrccho_procedure_event_handler((tdsrrc_cmd_type *) cmd_ptr );
            /*lint +e740*/
            break;
#endif

          case TDSRRC_INTERRAT_PLMN_SRCH_REQ:
          case TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ:
            /* Forward the command to MCM for processing */
            /*lint -e740*/
            if(TDSRRC_INTERRAT_PLMN_SRCH_REQ == cmd_ptr->cmd_hdr.cmd_id)
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_PLMN_SRCH_REQ to MCM");
               if((SYS_NETWORK_SELECTION_MODE_AUTOMATIC == cmd_ptr->cmd.interrat_plmn_srch_req.network_select_mode)
                ||(SYS_NETWORK_SELECTION_MODE_MANUAL == cmd_ptr->cmd.interrat_plmn_srch_req.network_select_mode))
                {
                    TDSRRC_DEBUG_INCREMENT_STATS(TDSRRC_BPLMN_G2TD);
                }
            }
            else
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_PLMN_SRCH_ABORT_REQ to MCM");
            }
            tdsrrcmcm_update_dual_mode_enabled_status(TRUE);
            tdsrrcmcm_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            /*lint +e740*/
            break;

          case TDSRR_INTERRAT_REDIRECT_REJ:
          case TDSRR_INTERRAT_REDIRECT_ABORT_CNF:
            /* Forward to CSP to process the cmd */
            /*lint -e740*/
            if(TDSRR_INTERRAT_REDIRECT_REJ == cmd_ptr->cmd_hdr.cmd_id)
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_REDIRECT_REJ to CSP");
            }
            else
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_REDIRECT_ABORT_CNF to CSP");
            }
            tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *)cmd_ptr);
            /*lint +e740*/
            break;

          case TDSRR_INTERRAT_PLMN_SRCH_CNF:
          case TDSRR_INTERRAT_PLMN_SRCH_ABORT_CNF:
            /* Forward to CSP to process the cmd */
            /*lint -e740*/
            if(TDSRR_INTERRAT_PLMN_SRCH_CNF == cmd_ptr->cmd_hdr.cmd_id)
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_PLMN_SRCH_CNF to CSP");
            }
            else
            {
               TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_INTERRAT_PLMN_SRCH_ABORT_CNF to CSP");
            }

            tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *)cmd_ptr);
            /*lint +e740*/
            break;

          case TDSRR_CIPHER_SYNC:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_CIPHER_SYNC to SMC");
            tdsrrcsmc_procedure_event_handler((tdsrrc_cmd_type *)cmd_ptr );
            break;


          case TDSRRC_INTERRAT_REDIRECT_REQ:
          case TDSRRC_INTERRAT_REDIRECT_ABORT_REQ:
            /* Forward the command to MCM for processing */
            if(TDSRRC_INTERRAT_REDIRECT_REQ == cmd_ptr->cmd_hdr.cmd_id)
            {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_REDIRECT_REQ to MCM");
                if(TRUE == cmd_ptr->cmd.interrat_redirect_req.pseudo_fr_active)
                {
                    TDSRRC_DEBUG_INCREMENT_STATS(TDSRRC_G2TD_FAST_RETURN_TRIGGERED);
                }
            }
            else
            {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_INTERRAT_REDIRECT_ABORT_REQ to MCM");
            }
            tdsrrcmcm_update_dual_mode_enabled_status(TRUE);
            tdsrrcmcm_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            break;
          case TDSRR_STOP_GSM_CNF:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_STOP_GSM_CNF to CSP");
            tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *)cmd_ptr);
            break;
#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO
          case TDSRR_DEDICATED_PRIORITIES_RSP:
            /*receive Dedicated priorities from RR*/
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_DEDICATED_PRIORITIES_RSP to MEAS");
            tdsrrcmeas_procedure_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            break;
          case TDSRRC_DEDICATED_PRIORITIES_REQ:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_DEDICATED_PRIORITIES_REQ to MEAS");
            tdsrrcmeas_procedure_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            break;
          case TDSRRC_CLEAR_DEDICATED_PRIORITIES_IND:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRRC_CLEAR_DEDICATED_PRIORITIES_IND to MEAS");
            tdsrrcmeas_procedure_event_handler((tdsrrc_cmd_type *)cmd_ptr);
            break;
#endif
           default:
             /* Unexpected RR command */       
             TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Unknown RR Command: %d",cmd_ptr->cmd_hdr.cmd_id);
             break;
        }
      } /* end if */
      else if((tdsrrc_deact_status == TDSRRC_DEACT_COMPLETED)
                 && (cmd_ptr->cmd_hdr.cmd_id == TDSRR_STOP_GSM_CNF))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> RR->RRC: Dispatching TDSRR_STOP_GSM_CNF to CSP");
        tdsrrccsp_cell_selection_procedure((tdsrrc_cmd_type *)cmd_ptr);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Throw away the RR command");
      }
    }
    /* The command has been processed. We can free the memory for this cmd */
    tdsrrc_free_rr_cmd_buf( cmd_ptr );
  }/* end while */
}/* end function */


#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*===========================================================================

FUNCTION  RRC_DISPATCH_LSM_COMMANDS

DESCRIPTION

  This function dispatches all commands received in RRC's lsm
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrcdispatch_lsm_commands( void )
{
  tdsrrc_cmd_type *cmd_ptr;   
  TDSRRC_MSG0(MSG_LEGACY_MED,"Dispatching LSM Commands");
  
  while( (cmd_ptr = q_get(&tdsrrc_lsm_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif    

  /* Switch on the command id of the received command, and
    call the appropriate function to process each command. */
    if (tdsrrcmcm_is_process_required(cmd_ptr) == FALSE) 
    {
      /* Print msg and disgard the command */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"CMD: %d is disgarded!",cmd_ptr->cmd_hdr.cmd_id);
    }
    else if(tdsrrc_deact_status == TDSRRC_NORMAL_OPERATION)
    {
      switch( cmd_ptr->cmd_hdr.cmd_id )
      {
      case TDSRRC_MEASUREMENT_IND:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> LSM->RRC: Dispatching TDSRRC_MEASUREMENT_IND to MEAS");
        tdsrrcmeas_procedure_event_handler( cmd_ptr );
        break;
 #ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      case TDSRRC_GPS_CELL_ID_REQUEST:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> LSM->RRC: Dispatching TDSRRC_GPS_CELL_ID_REQUEST to GPS");
       tdsrrcgps_procedure_event_handler( cmd_ptr );
       break;
 #endif
      default:
      /* This should not happen. Possibly the command id was never written
        before this item was put in the queue. Display error for now. */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"<IFTDSRRC> LSM->RRC: Unknown LSM cmd rcvd by RRC 0x%x",cmd_ptr->cmd_hdr.cmd_id);
        break;
      }/* end switch */
      
    }/* end if */
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Throw away the LSM command");
    }
    /* The command has been processed. We can free the memory for this cmd */
    tdsrrc_free_cmd_buf( cmd_ptr );
  }/* end while */
}/* end function */

#endif





/*===========================================================================

FUNCTION  RRC_DISPATCH_MSGR_COMMANDS

DESCRIPTION
 
  This function dispatches all commands received in RRC's Message Router
  command queue. This function dequeues each command in the command queue
  until the command queue is empty. For each command, it looks at
  the command id and passes the command to the appropriate procedure
  for processing. The command is not freed by this function. Freeing the
  command is the responsibility of the procedure that processes the
  command.
   
DEPENDENCIES

  None.
   
RETURN VALUE

  None. 
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_dispatch_msgr_commands(void)
{
  tds_rrc_msgr_cmd_type *msgr_cmd_ptr;   /* To hold received command */
  tdsrrc_cmd_type rrc_cmd;
  size_t cmd_size;
  uecomdef_status_e_type simid_validation = SUCCESS;

  sys_modem_as_id_e_type local_as_id;

  while( (msgr_cmd_ptr = q_get(&tdsrrc_msgr_cmd_q)) != NULL)
  {
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK_ENHANCEMENT
    tdsrrcdispatch_check_ff_commands();
#endif
#endif

    rrc_cmd.cmd_hdr.cmd_id = TDSRRC_MSGR_CMD;
    cmd_size = memscpy(&(rrc_cmd.cmd.tds_rrc_msgr_cmd),
                       sizeof(tds_rrc_msgr_cmd_type),
                       msgr_cmd_ptr,
                       sizeof(tds_rrc_msgr_cmd_type));
  
    if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
    {
       /* will clear variant in rrc_cmd in function tds_parse_cfa.*/
       tds_parse_cfa(TDSCFA_RRC_Q,(void *)&rrc_cmd);
    }
    
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
    local_as_id = (sys_modem_as_id_e_type)MSGR_GET_VARIANT(&(msgr_cmd_ptr->cmd.msg_hdr));
    MSGR_CLEAR_VARIANT(msgr_cmd_ptr->cmd.msg_hdr.id);
#else
    local_as_id = (int8)(INST_ID_TO_SYS_AS_ID(MSGR_GET_INST(&msgr_cmd_ptr->cmd.msg_hdr)));
#endif

    /* Check if the incoming internal command is required a further process */
    if (tdsrrcmcm_is_process_required(&rrc_cmd) == FALSE) 
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MSGR CMD: %x is disgarded! cmd_size = %d",rrc_cmd.cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id,cmd_size);
    }
    else
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"MSGR CMD: %x is received by RRC! cmd_size = %d",rrc_cmd.cmd.tds_rrc_msgr_cmd.cmd.msg_hdr.id,cmd_size);
#ifdef FEATURE_DUAL_SIM
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
      if (tdsrrc_check_msgr_for_simid_validation(local_as_id) == FAILURE)
      {
        if (TDSCDMA_RRC_QMI_DRX_LEVEL_REQ == msgr_cmd_ptr->cmd.msg_hdr.id)
        {
          simid_validation = TRUE;
        }
        else if (TDSCDMA_RRC_GPS_POS_CELL_INFO_REQ == msgr_cmd_ptr->cmd.msg_hdr.id)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"QMI cmd GPS_POS_CELL_INFO_REQ with mismatch as_id");
          simid_validation = FAILURE;
        }
        else 
        {
          ERR_FATAL("DSIM: Invalid SIM ID in LTE cmd %x",msgr_cmd_ptr->cmd.msg_hdr.id,0,0);
        }
      }  
#else
      simid_validation = tdsrrc_check_msgr_for_simid_validation(local_as_id);
#endif
#endif   
      if(tdsrrc_deact_status == TDSRRC_NORMAL_OPERATION)
      {
        switch(msgr_cmd_ptr->cmd.msg_hdr.id)
        {
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
          case TDSCDMA_RRC_LTE_RESEL_REQ:
          case TDSCDMA_RRC_LTE_ABORT_RESEL_REQ:
          case TDSCDMA_RRC_LTE_PLMN_SRCH_REQ:
          case TDSCDMA_RRC_LTE_ABORT_PLMN_SRCH_REQ:
          case TDSCDMA_RRC_LTE_REDIR_REQ:
          case TDSCDMA_RRC_LTE_ABORT_REDIR_REQ:
            //CGI:
          case TDSCDMA_RRC_LTE_GET_CGI_REQ:
          case TDSCDMA_RRC_LTE_ABORT_CGI_REQ:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> LTE->RRC: Dispatching MSGR CMD %x to MCM",msgr_cmd_ptr->cmd.msg_hdr.id);
            if(TDSCDMA_RRC_LTE_RESEL_REQ == msgr_cmd_ptr->cmd.msg_hdr.id)
            {
                TDSRRC_DEBUG_INCREMENT_STATS(L2TD_CEL_RESEL_TRIGGERED);
            }

            if(TDSCDMA_RRC_LTE_PLMN_SRCH_REQ == msgr_cmd_ptr->cmd.msg_hdr.id)
            {
                if((SYS_SERVICE_SEARCH_BACKGROUND_AUTOMATIC == msgr_cmd_ptr->cmd.tds_rrc_lte_plmn_srch_req.search_type)
                    &&(SYS_SERVICE_SEARCH_BACKGROUND_MANUAL== msgr_cmd_ptr->cmd.tds_rrc_lte_plmn_srch_req.search_type))
                {
                    TDSRRC_DEBUG_INCREMENT_STATS(TDSRRC_BPLMN_L2TD);
                }
            }
            
            tdsrrcmcm_event_handler(&rrc_cmd);
            break;
#ifdef FEATURE_PARTIAL_MPLMN_DISPLAY
          case TDSCDMA_RRC_LTE_GET_PLMN_PRTL_RESULTS_REQ:
		  	TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> LTE->RRC: Dispatching MSGR CMD %x to CSP",msgr_cmd_ptr->cmd.msg_hdr.id);
            tdsrrccsp_cell_selection_procedure(&rrc_cmd);
            break;
#endif
          case LTE_RRC_TDSCDMA_RESEL_FAILED_RSP:
          case LTE_RRC_TDSCDMA_ABORT_RESEL_RSP:
          case LTE_RRC_TDSCDMA_PLMN_SRCH_RSP:
          case LTE_RRC_TDSCDMA_ABORT_PLMN_SRCH_RSP:
          case LTE_RRC_TDSCDMA_REDIR_FAILED_RSP:
          case LTE_RRC_TDSCDMA_ABORT_REDIR_RSP:
          case TDSCDMA_RRC_LTE_PLMN_SRCH_SUSPEND_IND:
#ifdef FEATURE_LTE_TO_TDSCDMA
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_RLF_REPORT_ENHANCEMENT
          case LTE_RRC_UTRA_SERVING_CELL_INFO_REQ:
#endif		  
#endif
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> LTE->RRC: Dispatching MSGR CMD %x to CSP",msgr_cmd_ptr->cmd.msg_hdr.id);
            tdsrrccsp_cell_selection_procedure(&rrc_cmd);
            break;
          case LTE_RRC_EUTRA_CAPABILITIES_UTRA_TDD_RSP:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> LTE->RRC: Dispatching MSGR CMD %x to UECI",msgr_cmd_ptr->cmd.msg_hdr.id);
            tdsrrcueci_procedure_event_handler(&rrc_cmd);
            break;
          case TDSCDMA_RRC_LTE_UTRA_CAPABILITIES_REQ:
          case LTE_RRC_TDSCDMA_PSHO_RSP:
          case LTE_RRC_TDSCDMA_ABORT_PSHO_RSP:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> LTE->RRC: Dispatching MSGR CMD %x to IHO",msgr_cmd_ptr->cmd.msg_hdr.id);
            tdsrrciho_event_handler(&rrc_cmd);
            break;
          case TDSCDMA_RRC_LTE_GET_DEDICATED_PRI_REQ:
          case TDSCDMA_RRC_LTE_CLEAR_DEDICATED_PRI_IND:
          case LTE_RRC_TDSCDMA_GET_DEDICATED_PRI_RSP:
#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_DEPRIORITIZATION
          case TDSCDMA_RRC_LTE_DEPRI_FREQ_REQ:
#endif          
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> LTE->RRC: Dispatching MSGR CMD %x to MEAS",msgr_cmd_ptr->cmd.msg_hdr.id);
            tdsrrcmeas_procedure_event_handler(&rrc_cmd);
            break;

#ifdef FEATURE_TDSCDMA_L2TD_PS_HANDOVER
          case TDSCDMA_RRC_LTE_PSHO_REQ:
          case TDSCDMA_RRC_LTE_ABORT_PSHO_REQ:
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> LTE->RRC: Dispatching MSGR CMD %x to IHO",msgr_cmd_ptr->cmd.msg_hdr.id);
            tdsrrciho_event_handler(&rrc_cmd);
            break;
#endif
#endif /* #if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA) */

          case TDSCDMA_RRC_GPS_POS_CELL_INFO_REQ:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> QMI->RRC: Dispatching TDSCDMA_RRC_GPS_POS_CELL_INFO_REQ to RRC");
            tdsrrcmeas_process_gps_pos_cell_info_req(&rrc_cmd, local_as_id, simid_validation);
            break;

          case TDSCDMA_RRC_GET_CONFIG_REQ:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> QMI->RRC: Dispatching TDSCDMA_RRC_GET_CONFIG_REQ to RRC");
            tdsrrcnv_process_get_cmcc_config(); /* This command doesn't have payload */
            break;

          case TDSCDMA_RRC_SET_CONFIG_REQ:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> QMI->RRC: Dispatching TDSCDMA_RRC_SET_CONFIG_REQ to RRC");
            tdsrrcnv_process_set_cmcc_config(&rrc_cmd);
            break;

          case TDSCDMA_RRC_QMI_DRX_LEVEL_REQ:
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"<IFTDSRRC> QMI->RRC: Dispatching TDSCDMA_RRC_QMI_DRX_LEVEL_REQ to RRC");
            tdsrrcmisc_process_qmi_drx_level_req(local_as_id, simid_validation);
            break;

          default :
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"<IFTDSRRC> : TDS MSGR: ID %x not processed",msgr_cmd_ptr->cmd.msg_hdr.id);
            break;
        }
      } 
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Throw away the MSGR CMD %x",msgr_cmd_ptr->cmd.msg_hdr.id);
      }
    }
    /*Put the cmd back to free queue*/
    q_put(&tdsrrc_msgr_free_cmd_q,&(msgr_cmd_ptr->link));
  }
}

/*===========================================================================

FUNCTION  RRC_DISPATCHER

DESCRIPTION
 
  The RRC_DISPATCHER is the central wait-loop for the RRC task. This
  function constantly waits for events to be received. When events are
  received, they are dispatched to the appropriate function to be processed.
  
  This function is the only wait-loop in the RRC task. No other procedures
  can perform a local wait loop. 
 
  The set of events that the RRC task waits for is determined by RRC's
  event dispatch table. This table must be created before tdsrrc_dispatcher is
  called. Regardless of the current state of RRC, the set of events that
  the tdsrrc_dispatcher is waiting for is the same.
 
DEPENDENCIES

  Event dispatch table must be created before this function is called.  
   
RETURN VALUE

  None. Note that this function does not return unless the RRC task is being
  shut down. (*** This is TBD ***).
  
SIDE EFFECTS

  None.

===========================================================================*/

void tdsrrc_dispatcher ( void )
{
  
  rex_sigs_type rrc_requested_sigs;  /* Signals to wait for */
  rex_sigs_type rrc_received_sigs;   /* Signals received    */



  /* Set Rex signal mask to the desired signals */

  rrc_requested_sigs =   
    
         /* Signal for internal RRC commands  */
         TDSRRC_INT_CMD_Q_SIG

         /* Signal for L1 commands            */
      |  TDSRRC_L1_CMD_Q_SIG

         /* Signal for MAC commands           */
      |  TDSRRC_MAC_CMD_Q_SIG

         /* Signal for RLC commands           */
      |  TDSRRC_RLC_CMD_Q_SIG

         /* Signal for MM commands            */
      |  TDSRRC_MM_CMD_Q_SIG

         /* Signal to send watchdog report    */
      |  TDSRRC_RPT_TIMER_SIG
      
         /* Signal for TC commands            */
      |  TDSRRC_TC_CMD_Q_SIG 
         /* Signal for RR commands            */
      |  TDSRRC_RR_CMD_Q_SIG
#if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA)
         /* Signal for WRRC commands          */
      |  TDSRRC_RRC_CMD_Q_SIG
#endif
#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
      | TDSRRC_LSM_CMD_Q_SIG
#endif
      | TDSRRC_MSGR_SIG
#ifdef TEST_FRAMEWORK
      #error code not present
#endif /*TEST_FRAMEWORK*/
      | TDSRRC_WRM_PREEPTION_SIG
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
      | TDSRRC_L1_FF_CMD_Q_SIG
#endif
         /* Add other signals here            */
            ;

#if 0 //using DOG HB mechanism
  /*Register with DOG, get the Task ID*/
  tdsrrc_dog_rpt_var      = dog_register(tds_rrc_get_tcb(), \
                                  DOG_DEFAULT_TIMEOUT, DOG_DEADLKCHK_DISABLE);
  /*Get report interval*/
  tdsrrc_dog_rpt_time_var = dog_get_report_period(tdsrrc_dog_rpt_var);

  /* With FEATURE_WCDMA_RRC_WDOG_DYNAMIC feature, DOG_RRC_RPT_TIME can be 0. So we need to handle it*/
  if ( TDSDOG_RRC_RPT_TIME > 0 ) 
  {
    rex_def_timer(&tdsrrc_rpt_timer, tds_rrc_get_tcb(), TDSRRC_RPT_TIMER_SIG);
    (void) rex_set_timer(&tdsrrc_rpt_timer, TDSDOG_RRC_RPT_TIME);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Dog report time %d",TDSDOG_RRC_RPT_TIME);
  }
#else
  /* Register with Dob HB. NOTE: Do not register multiple times per task context! */
  TDSDOG_RRC_RPT = dog_hb_register_rex((rex_sigs_type)TDSRRC_RPT_TIMER_SIG);
#endif

  TDSRRC_MSG0(MSG_LEGACY_MED,"Waiting for RRC Signals");

  /* Perform the rex wait */
  while( (rrc_received_sigs = rex_wait( rrc_requested_sigs )) != 0 )
  {

    /* We get here when atleast one of the requested
    signals is set. Check the received signals in sequential
    order of priority so that if more than one signal is
    set, we process the higher priority signal first. */

    /* Signal to send Watchdog report */
    if( rrc_received_sigs & TDSRRC_RPT_TIMER_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_RPT_TIMER_SIG );
#if 0 //using DOG HB mechanism
      /* If it has come till this point then DOG_RRC_RPT_TIME should not be 0*/
      dog_report(TDSDOG_RRC_RPT);
      (void) rex_set_timer(&tdsrrc_rpt_timer, TDSDOG_RRC_RPT_TIME);
#else
      /* Report to the Dog HB */
      dog_hb_report(TDSDOG_RRC_RPT);
#endif
    }

    if( rrc_received_sigs & TDSRRC_WRM_PREEPTION_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_WRM_PREEPTION_SIG );
      tdsrrcwrm_post_preemption_indication();
    }

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
      if( rrc_received_sigs & TDSRRC_L1_FF_CMD_Q_SIG )
      {
        (void) rex_clr_sigs( rex_self(), TDSRRC_L1_FF_CMD_Q_SIG );
        tdsrrc_dispatch_l1_ff_commands();
      }
#endif

    /* RRC Internal Command Queue */
    if( rrc_received_sigs & TDSRRC_INT_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_INT_CMD_Q_SIG );
      tdsrrcdispatch_internal_commands();
    }

    /* RRC L1 Command Queue */
    if( rrc_received_sigs & TDSRRC_L1_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_L1_CMD_Q_SIG );
      tdsrrcdispatch_l1_commands();
    }

    /* RRC MAC Command Queue */
    if( rrc_received_sigs & TDSRRC_MAC_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_MAC_CMD_Q_SIG );
      tdsrrcdispatch_mac_commands();
    }

    /* RRC RLC Command Queue */
    if( rrc_received_sigs & TDSRRC_RLC_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_RLC_CMD_Q_SIG );
      tdsrrcdispatch_rlc_commands();
    }

    /* RRC MM Command Queue */
    if( rrc_received_sigs & TDSRRC_MM_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_MM_CMD_Q_SIG );
      tdsrrcdispatch_mm_commands();
    }
    
    /* RRC TC Command Queue */
    if( rrc_received_sigs & TDSRRC_TC_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_TC_CMD_Q_SIG );
      tdsrrcdispatch_dispatch_tc_commands();
    }

    /* RRC RR Command Queue */
    if( rrc_received_sigs & TDSRRC_RR_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_RR_CMD_Q_SIG );
      tdsrrcdispatch_rr_commands();
    }

#if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA)
    /* RRC WRRC Command Queue */
    if( rrc_received_sigs & TDSRRC_RRC_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_RRC_CMD_Q_SIG );
      tdsrrcdispatch_dispatch_wrrc_commands();
    }
#endif

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
    /* RRC LSM Command Queue */
    if( rrc_received_sigs & TDSRRC_LSM_CMD_Q_SIG )
    {
      (void) rex_clr_sigs( rex_self(), TDSRRC_LSM_CMD_Q_SIG );
      tdsrrcdispatch_lsm_commands();
    }
#endif

    /* Message Router queue */
    if(rrc_received_sigs & TDSRRC_MSGR_SIG)
    {
      (void)rex_clr_sigs(rex_self(), TDSRRC_MSGR_SIG);
      tdsrrc_dispatch_msgr_commands();
    }  
#ifdef TEST_FRAMEWORK
    #error code not present
#endif/*TEST_FRAMEWORK*/     


  }/* end while */

  /* Deregister if leaving the task so that we don't ERR_FATAL */
  dog_hb_deregister(TDSDOG_RRC_RPT);

}/* end function */
