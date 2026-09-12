/*===========================================================================
            COPYRIGHT INFORMATION

Copyright (c) 2001,2002 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mm/vcs/mmmain.c_v   1.18   03 Jul 2002 15:06:16   jault  $
$Header: //components/rel/mmcp.mpss/7.9.0/nas/mm/src/mmmain.c#1 $ $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/03/01   jca     Added processing for RRC_ACT_CNF.
06/03/01   sbk     Global variables are renamed to use mm_ prefix
06/08/01   jca     Added support for RRC_ABORT_IND, RRC_ABORT_CNF and RRC_DEACT_CNF.
06/20/01   sbk     Added support for RRC_CLOSE_SESSION_IND
08/07/01   ts      Standardized MSG_ERROR and MSG_HIGH usage.
08/15/01   ab      Renamed CM to CNM.
08/22/01   jca     Removed errant CNM renames.  Fixed control flow errors.
09/18/01   sbk     Intertask primitives are logged and QXDM logging is supported
02/20/02   sbk     Routing code to TC added
06/05/02   ks      Added support for Dual mode operation
07/01/02   jca     Added processing of RR_ABORT_CNF and RR_STOP_GSM_MODE_CNF.
07/03/02   jca     Deleted timers TIMER_PLMN_SELECT and TIMER_PLMN_LIST.
07/22/02   ks      Added processing of RRC_STOP_WCDMA_MODE_CNF mm_handle_message
08/03/02   mks     Now processing RR_ACTIVATION_IND message
08/09/02   jca     Stop T3212 timer when L3 message received in state
                   WAIT_FOR_NETWORK_COMMAND (as per 24.008 spec).
09/23/02   ks      Added mmrat.h include file, fixes compiler warning.
10/10/02   mks     Now processing RRC_ACTIVATION_IND primitive from RRC
10/30/02   mks     Added support for integrity checking of CS domain messages
11/12/02   jca     Added support for RRC_CLOSE_SESSION_CNF.
11/14/02   vdr     Added support for transferring CNM data over PS connection
11/14/02   mks     Added support for MM_INFORMATION message
12/04/02   vdr     Fixed processing of the MMCNM_DATA_REQ
12/13/02   vdr     Fixed identification of the MO SMS messages
01/10/03   vdr     Changed routing conditions for the outgoing SMS messages
02/21/03   vdr     o Changed routing of the MMCNM_DATA_REQ which is containing SMS messages
                     according to the message set that they are belonging to.
                   o Added processing WMS' MM_SMS_BEARER_SELECTION_CMD primitive
06/02/03   jca     Now ignoring MMCNM_DATA_REQ if RR connection not established.
06/04/03   ks      Added support for RRC_MODE_CHANGE_CNF for MODE_CHANGE
                   support for UMTS.
06/11/03   jca     Process MMCNM_DATA_REQ for PS domain regardless of MM state.
07/15/03   ab      Added NAS OTA message logging in GSM mode.
04/06/04   cd      LCS support
04/09/04   ab      Fixed CR 40898, some NAS messages are not being logged in GSM.
06/08/04   cd      LCS support.  Replaced processing of rr_lcs_active_ind by lcs_active_ind
08/12/04   cd      Replaced FEATURE_LCS by FEATURE_MMGPS
10/15/04   jca     Ensure connection established before sending MMCNM_DATA_REQ to RRC.
11/16/04   sv      Handling of the new primitive RRC_SERVICE_IND_FROM_OTA_MSG
06/06/05   abe     Added code for CCBS support
07/20/05   abe     Added the handling for the primitive RRC_WTOG_CCO_COMPLETE_IND
                   for FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER.
10/08/05   abe     Added code for FEATURE_INTERRAT_PCCO_GTOW 
12/14/05   abe      Supported changes for FEATURE_GSM_DTM
01/30/06   hj      Added support for CS_ONLY and PS_ONLY routing options for SMS.
03/13/06   abe     Added support for routing UE POSITIONING INFO to LSM Task
03/20/06   ss      Added FEATURE_MMGPS flag, in handling TC messages.
09/21/06   pk      Updated for FEATURE_GSM_EDTM
08/30/11   jbk     Stop TIMER_SCRI_PAGING on receiving MM message
09/16/11   jbk     Timer T3240 is stopped, reset, and started again at receipt of an MM message
11/21/11   gps     Removed refrence for header file "gsdi_exp.h"
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmcp_variation.h"
#include "customer.h"
#include "rex.h"
#include "sys_type.h"
#include "sys_cnst_v.h"
#include "sys_stru_v.h"
#include "ms.h"
#include "timers_v.h"
#include "gs_v.h"
#include "ms_timer_v.h"
#include "mm_v.h"
#include "err.h"
#include "mmtask.h"
#include "mmtask_int.h"
#include "mm_umts_v.h"
#include "mmsecurity.h"
#include "mmrat.h"
#include "mminfo.h"
#include "sys_eplmn_list_v.h"
#include "mm_coord.h"
#include "naslog_v.h"
#include "gmm_int_v.h"
#include "mm_coord.h"
#include <stringl/stringl.h>
#include "mm_ssgccs.h"

#ifdef FEATURE_NAS_ECALL
#ifdef FEATURE_DUAL_SIM
extern  uint32 mm_ecall_inactivity_timer_sim[MAX_NAS_STACKS];
#define mm_ecall_inactivity_timer mm_ecall_inactivity_timer_sim[mm_as_id]

extern  boolean mm_ecall_profile_sim[MAX_NAS_STACKS];
#define mm_ecall_profile mm_ecall_profile_sim[mm_as_id]
#else
extern uint32  mm_ecall_inactivity_timer;
extern boolean mm_ecall_profile;
#endif
#endif
/*===========================================================================

                DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains definitions for constants, macros, types, variables
and other items needed by this module.

===========================================================================*/

extern void mm_timer_policy_change_pending_expiry(void);



/*===========================================================================

FUNCTION MM_HANDLE_MESSAGE

DESCRIPTION
  This function distributes the mm messages

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mm_handle_message( mm_cmd_type *msg_p)
{
   boolean     message_ok;
   byte        protocol_descriminator;
   byte        message_type;
   boolean     rrc_message_set_mapped_as_rr = FALSE;
   dword       ecall_timer_value = 0;
#ifdef FEATURE_NAS_ECALL
   inter_task_lai_T current_lai;
#endif


   /*-------------------------------------------------------------
   The RRC message set is mapped into RR message set to keep the
   state machine oblivious of low level message sets.  This means
   that the RR and RRC messages id's must not clash
   ---------------------------------------------------------------*/
   if ((msg_p->cmd.hdr.message_set == MS_MM_RRC) &&
       (msg_p->cmd.hdr.message_id != RRC_ACTIVATION_IND))
   {
      msg_p->cmd.hdr.message_set = MS_MM_RR;
      rrc_message_set_mapped_as_rr = TRUE;
   }

   /* Distribute the messages */
   switch (msg_p->cmd.hdr.message_set)
   {
      case MS_MM_RR:
      case MS_MM_RRC:
      case MS_MM_TDSRRC:
         switch (msg_p->cmd.hdr.message_id)
         {
            case MM_DATA_IND:
#ifdef FEATURE_DUAL_SIM
            case RR_DS_DATA_IND:

               if(msg_p->cmd.hdr.message_id == (int)RR_DS_DATA_IND)
#else
            case RR_DATA_IND:   /* layer peer message received */

               /*-----------------------------------------------
                ** LOG OTA Message in GSM mode
                **----------------------------------------------*/
               if(msg_p->cmd.hdr.message_id == (int)RR_DATA_IND)
#endif
               {
                   uint2 length;


#ifdef FEATURE_DUAL_SIM
                   PUT_IMH_LEN(msg_p->cmd.rr_ds_data_ind.l3_message_length, 
                               &msg_p->cmd.hdr);
                   (void) memsmove( msg_p->cmd.rr_data_ind.layer3_message,gmm_max_L3_msg_length(MM_CS_DOMAIN),
                                   msg_p->cmd.rr_ds_data_ind.layer3_message,
                                   msg_p->cmd.rr_ds_data_ind.l3_message_length );
                   msg_p->cmd.rr_data_ind.message_header.message_id = RR_DATA_IND;
#endif
                   GET_IMH_LEN(length, &msg_p->cmd.hdr);
                   mm_send_ota_msg_log_packet(CN_TO_UE_DIRECTION,
                                              length,
                                              msg_p->cmd.rr_data_ind.layer3_message);
               }

               if (msg_p->cmd.rr_data_ind.message_header.message_len_lsb >= 2)
               {
                 protocol_descriminator =
                  (byte)GET_PD( msg_p->cmd.rr_data_ind.layer3_message[0]);
                 message_type = msg_p->cmd.rr_data_ind.layer3_message[1];
               }
               else
               {
               	return;
               }
               
               /* --------------------------------------------------------------------
               ** 24.008 4.4.4.9, 4.5.1.2: cases f.1 
               ** The procedure shall be initiated again, if 
               ** ii)No SECURITY MODE COMMAND message and no NAS messages relating 
               ** to the RR connection were received after the OTA msg was transmitted
               ** -------------------------------------------------------------------- */
               if (mm_cs_domain_possible_reest && is_umts_mode() &&
                   (mm_check_for_integrity_protection(msg_p) == TRUE) &&
                   (( protocol_descriminator == PD_MM ) ||
                    ( protocol_descriminator == PD_CC ) ||
                    ( protocol_descriminator == PD_SS ) ||
                    ( protocol_descriminator == PD_SMS) 
#if defined (FEATURE_MMGPS) || defined (FEATURE_TC) || defined (FEATURE_CGPS_UMTS_CP)
                    || ( protocol_descriminator == PD_TEST )
#endif
                    ))
               {
                  mm_cs_domain_possible_reest = FALSE;
               }

               if (is_umts_mode() && mm_check_for_integrity_protection(msg_p) == FALSE)
               {
                  MSG_ERROR_DS(MM_SUB, "=MM= Discard msg (%d) Integ check fail",
                             message_type, 0,0);
               }
               else if ( protocol_descriminator == PD_MM )
               {
                  /* Sanity check for the skip portion the skip_pd byte to be 0 */
                  if( (msg_p->cmd.rr_data_ind.layer3_message[0] & (byte)(0xF0)) == 0 )
                  {
                     /*---------------------------------------------------------
                       Parse the MM OTA messages for syntax and semantics
                       If successful, the contents of the message are available
                       in a global array IE_Info
                       ---------------------------------------------------------*/
                     message_ok = mm_check_for_protocol_errors( &msg_p->cmd);
                     if( message_ok == FALSE)
                     {
                        MSG_HIGH_DS_1(MM_SUB ,"=MM= The MM ota message of type %d failed due to protocol errors",
                              message_type);
                     }

                     if (message_ok == TRUE)
                     {
                        /* -------------------------------------------------------
                        ** The T3212 timer shall be stopped if the mobile station
                        ** has responded to paging and thereafter has received the
                        ** first correct layer 3 message (3GPP TS 24.008 4.2.2).
                        ** ------------------------------------------------------- */
                        if (mm_state == MM_WAIT_FOR_NETWORK_COMMAND)
                        {
                          mm_stop_timer( TIMER_T3212 );
                          if (mm_nas_feature_to_be_configured.UE_init_rel_on_spurious_page)
                          {
                            mm_stop_timer(TIMER_SCRI_PAGING);
                          }
                        }

                        if ((mm_state == MM_WAIT_FOR_NETWORK_COMMAND) ||
                            (mm_state == MM_LOCATION_UPDATE_REJECTED))
                        {
                          if (mm_timer_status[TIMER_T3240] == TIMER_ACTIVE)
                          {
                            /* Timer T3240 is stopped, reset, and started again at receipt of an MM message.*/
                            mm_start_timer(TIMER_T3240, DEFAULT_TIMEOUT);
                          }
                        }
						if(message_type!=AUTHENTICATION_REQUEST)
						{						
						  mm_ssgccs_handle_icoming_msg(message_type,mm_serving_plmn.info.active_rat,CS_DOMAIN_PROCEDURE,msg_p);
						}
                        switch (message_type)
                        {
                           case LOCATION_UPDATING_ACCEPT:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received LOCATION_UPDATING_ACCEPT");
                              mm_handle_registration(msg_p);
                              break;

                           case LOCATION_UPDATING_REJECT:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received LOCATION_UPDATING_REJECT");
                              mm_handle_registration(msg_p);
                              break;

                           case ABORT:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received ABORT");
                              mm_perform_mm_connection_management(msg_p);
                              break;

                           case CM_SERVICE_ACCEPT:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received CM_SERVICE_ACCEPT");
                              mm_perform_mm_connection_management(msg_p);
                              break;

                           case CM_SERVICE_REJECT:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received CM_SERVICE_REJECT");
                              mm_perform_mm_connection_management(msg_p);
                              break;
                              
                           #ifdef FEATURE_CCBS 
                              case CM_SERVICE_PROMPT:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received CM_SERVICE_PROMPT");
                              mm_perform_mm_connection_management(msg_p);
                              break;
                           #endif 

                           case TMSI_REALLOCATION_COMMAND:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received TMSI_REALLOCATION_COMMAND");
                              mm_handle_security(msg_p);
                              break;

                           case IDENTITY_REQUEST:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received IDENTITY_REQUEST");
                              mm_handle_security(msg_p);
                              break;

                           case AUTHENTICATION_REQUEST:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received AUTHENTICATION_REQUEST");
                              mm_handle_security(msg_p);
                              break;

                           case AUTHENTICATION_REJECT:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received AUTHENTICATION_REJECT");
                              mm_handle_security(msg_p);
                              break;

                           case MM_STATUS:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received MM_STATUS");
                              mm_process_diagnostic(msg_p);
                              break;

                           case MM_INFORMATION:
                              MSG_HIGH_DS_0(MM_SUB, "=MM= MM Received MM_INFORMATION");
                              mm_handle_information(msg_p);
                              break;

                           default:
                              /* unexpected message type - can not handle*/
                              MSG_ERROR_DS(MM_SUB, "=MM= Processing of MM OTA message of type %d not supported",
                                       message_type, 0,0);
                              break;
                        }

                     }

                     /* else do nothing - fault already handled */

                  }

               }
               else if (( protocol_descriminator == PD_CC ) ||
                        ( protocol_descriminator == PD_SS ) ||
                        ( protocol_descriminator == PD_SMS) )
               {
                   /*
                    * message is not for MM, so pass up the stack
                    */
                     mm_pass_CNM_data(msg_p);
					 if(is_umts_mode() && ( protocol_descriminator == PD_SS ))
					 {
					 mm_set_is_ss_request_rrc_sent(TRUE);
					 }
               }
#if defined (FEATURE_MMGPS) || defined (FEATURE_TC) || defined (FEATURE_CGPS_UMTS_CP)
               else if( protocol_descriminator == PD_TEST )
               {
#if defined ( FEATURE_MMGPS ) || defined ( FEATURE_CGPS_UMTS_CP )
                  if(message_type == RESET_UE_POSITIONING_STORED_INFORMATION)
                  {
                    mm_process_lcs_ue_pos_stored_info(msg_p);
                    if (mm_nas_feature_to_be_configured.UE_init_rel_on_spurious_page)
                    {
                      mm_stop_timer(TIMER_SCRI_PAGING);
                    }
                  }
#ifdef FEATURE_TC
                  else
#endif
                  
#endif
#ifdef FEATURE_TC
                  
                  {
                    mm_handle_test_control_data(msg_p);
                    if (mm_nas_feature_to_be_configured.UE_init_rel_on_spurious_page)
                    {
                      mm_stop_timer(TIMER_SCRI_PAGING);
                    }
                  }
#endif
               }
#endif
               else
               {
                 if (mm_state == MM_WAIT_FOR_NETWORK_COMMAND)
                 {
                   if (mm_nas_feature_to_be_configured.UE_init_rel_on_spurious_page)
                   {
                     mm_stop_timer(TIMER_SCRI_PAGING);
                   }
                 } 
#ifdef FEATURE_NAS_REL11
                 mm_stop_timer(TIMER_T3240);
#endif 
                 MSG_HIGH_DS_1(MM_SUB ,"=MM= Unexpected pd %d", protocol_descriminator);
               }

               break;

            case RRC_SERVICE_IND:
            case RRC_SERVICE_IND_FROM_OTA_MSG:
            case RR_SERVICE_IND:
#ifdef FEATURE_GSM_DTM
            case RR_SERVICE_IND_FROM_OTA_MSG:
#endif
            case RR_DEACT_CNF:
            case RRC_DEACT_CNF:
            case RR_PLMN_SELECT_CNF:
            case RR_PLMN_LIST_CNF:
            case RRC_SERVICE_CNF:
            case RRC_PLMN_LIST_CNF:
            case RR_STOP_GSM_MODE_CNF:
            case RRC_STOP_WCDMA_MODE_CNF:
            case RRC_MODE_CHANGE_CNF:
               mm_handle_registration(msg_p);
               break;

            case RR_ACTIVATION_IND:
            case RRC_ACTIVATION_IND:
 #ifdef FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER
            case RRC_WTOG_CCO_COMPLETE_IND:
 #endif /* FEATURE_SPEC_COMPLIANT_WTOG_CELL_CHANGE_ORDER */
 #ifdef FEATURE_INTERRAT_PCCO_GTOW
            case RR_GTOW_CCO_COMPLETE_IND:
 #endif /* FEATURE_INTERRAT_PCCO_GTOW */
               mm_handle_inter_rat_change(msg_p);
               break;

#if defined (FEATURE_GSM_EDTM) || defined (FEATURE_GAN)
            case RR_EST_IND:
#endif
            case RR_PAGING_IND:
            case RRC_OPEN_SESSION_CNF:
            case RR_EST_CNF:
            case RRC_EST_CNF:
            case RRC_REL_IND:
            case RR_REL_IND:

            case RR_ABORT_IND:
            case RR_ABORT_CNF:
            case RRC_ABORT_IND:
            case RRC_ABORT_CNF:
            case RRC_SYNC_IND:
            case RR_SYNC_IND:
            case RRC_PAGE_IND:
            case RRC_CLOSE_SESSION_IND:
            case RRC_CLOSE_SESSION_CNF:
#ifdef FEATURE_DUAL_SIM
            case RR_PREEMPTION_IND:
#endif 
               mm_perform_mm_connection_management(msg_p);
               break;

            default:
               MSG_ERROR_DS(MM_SUB, "=MM= Unexpected mm-rr message id %d", msg_p->cmd.hdr.message_id, 0,0);
               break;

         }
         break;
#if defined ( FEATURE_MMGPS ) || defined ( FEATURE_CGPS_UMTS_CP )
      case MS_MM_LSM:
         switch (msg_p->cmd.hdr.message_id)
         {
            case LCS_ACTIVE_IND:
               mm_perform_mm_connection_management(msg_p);
               break;

            default:
               MSG_ERROR_DS(MM_SUB, "=MM= Unexpected mm-lsm message id %d", msg_p->cmd.hdr.message_id, 0,0);
               break;

         }
         break;

#endif /* end of (FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP) */

      case MS_CC_MM:
      case MS_CC_MM_PS:

         /*
          * All messages from CM
          */

         switch (msg_p->cmd.hdr.message_id)
         {
            case MMCNM_EST_REQ:
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received MMCNM_EST_REQ");
               mm_perform_mm_connection_management(msg_p);
               break;

            case MMCNM_REL_REQ:
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received MMCNM_REL_REQ");
               mm_perform_mm_connection_management(msg_p);
               break;

            case MMCC_REEST_REQ:
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received MMCC_REEST_REQ");
               mm_perform_mm_connection_management(msg_p);
               break;

            case MMCNM_DATA_REQ:

               /* -------------------------
               ** RR connection established
               ** ------------------------- */
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received MMCNM_DATA_REQ");
               if (is_OTA_message( msg_p ))
               {
                  if ((mm_state != MM_IDLE) &&
                      (mm_state != MM_NULL) &&
                      (mm_state != MM_WAIT_FOR_RR_ACTIVE) &&
                      (mm_state != MM_WAIT_FOR_RR_CONNECTION_MM) &&
                      (mm_state != MM_WAIT_FOR_RR_CONNECTION_LU) &&
                      (mm_state != MM_WAIT_FOR_RR_CONNECTION_IMSI_DETACH) &&
                      (mm_state != MM_WAIT_FOR_REESTABLISH_DECISION) &&
                      (mm_state != MM_WAIT_FOR_RR_CONNECTION_REESTABLISHMENT) &&
                      (mm_state != MM_LOCATION_UPDATING_PENDING))
                  {
                     mm_pass_CNM_data( msg_p );
                  }
                  else
                  {
                     MSG_HIGH_DS_1(MM_SUB ,"=MM= MMCNM_DATA_REQ ignored in MM state %d", mm_state);
                  }
               }
               else
               {
                  if ((msg_p->cmd.mmcnm_data_req.data[0] & 0x0F) == PD_SMS)
                  {
                     switch (msg_p->cmd.hdr.message_set)
                     {
                        case MS_CC_MM:
                           if ((mm_state != MM_IDLE) &&
                               (mm_state != MM_NULL) &&
                               (mm_state != MM_WAIT_FOR_RR_ACTIVE) &&
                               (mm_state != MM_WAIT_FOR_RR_CONNECTION_MM) &&
                               (mm_state != MM_WAIT_FOR_RR_CONNECTION_LU) &&
                               (mm_state != MM_WAIT_FOR_RR_CONNECTION_IMSI_DETACH) &&
                               (mm_state != MM_WAIT_FOR_REESTABLISH_DECISION) &&
                               (mm_state != MM_WAIT_FOR_RR_CONNECTION_REESTABLISHMENT) &&
                               (mm_state != MM_LOCATION_UPDATING_PENDING))
                           {
                              mm_pass_CNM_data(msg_p);
                           }
                           else
                           {
                              MSG_HIGH_DS_1(MM_SUB ,"=MM= MMCNM_DATA_REQ ignored in MM state %d", mm_state);
                           }
                           break;
                        case MS_CC_MM_PS:
                           mm_send_ps_cnm_data(msg_p);
                           break;
                        default:
                           MSG_ERROR_DS(MM_SUB, "=MM= MMCNM_DATA_REQ(SMS) - destination is not identified",0,0,0);
                           break;
                     }
                  }
                  else
                  {
                     if (rrc_connection_status == MMRRC_ACTIVE)
                     {
                        mm_pass_CNM_data( msg_p );
                     }
                     else
                     {
                        MSG_ERROR_DS( MM_SUB, "=MM= MMCNM_DATA_REQ ignored in MM state %d", mm_state, 0,0 );
                     }
                  }
               }
               break;
            #ifdef FEATURE_CCBS 
            
            case MMCNM_PROMPT_RES:
               MSG_HIGH_DS_0(MM_SUB,"=MM= MM Received MMCNM_PROMPT_RES");
               mm_perform_mm_connection_management(msg_p);
               break;
               
            #endif /* FEATURE_CCBS */

            default:
               MSG_ERROR_DS(MM_SUB, "=MM= Unexpected cc-mm message id %d", msg_p->cmd.hdr.message_id,0,0);
               break;

         }
         break;

      case MS_MM_REG:
         mm_handle_registration(msg_p);
         break;

      case MS_TIMER:
         if ( msg_p->cmd.hdr.message_id == MID_TIMER_EXPIRY )
         {
            MSG_HIGH_DS_1(MM_SUB ,"=MM= Timer %d expired", msg_p->cmd.timer_expiry.timer_id);

            /*
             * flag that timer has expired in timer_status
             */

            if (msg_p->cmd.timer_expiry.timer_id <= MAX_MM_TIMER )
            {
               mm_timer_status[msg_p->cmd.timer_expiry.timer_id] = TIMER_STOPPED;
            }

            gs_clear_timer_ext( 
                                get_mm_timer_queue(),
                                msg_p->cmd.timer_expiry.timer_id,
                                mm_timer_non_deferrable( msg_p->cmd.timer_expiry.timer_id )
                              );

            /*
             * send timer message off to appropriate handler
             */

            switch (msg_p->cmd.timer_expiry.timer_id)
            {
               case TIMER_T3220:
               case TIMER_T3210:
               case TIMER_T3211:
               case TIMER_T3212:
               case TIMER_T3213:
#ifdef FEATURE_NAS_ECALL
               case TIMER_T3242:
               case TIMER_T3243:
#endif
               case TIMER_MM_FORBIDDEN_LAI_CLEAR:
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
               case TIMER_CSMT_GUARD:
                  mm_handle_registration(msg_p);
                  break;

               case TIMER_T3230:
               case TIMER_SCRI_PAGING:
               case TIMER_T3240:
#if defined ( FEATURE_MMGPS ) || defined ( FEATURE_CGPS_UMTS_CP )
               case TIMER_T3241:
#endif /* end of (FEATURE_MMGPS || FEATURE_CGPS_UMTS_CP) */
               case TIMER_MM_REL:
                  mm_perform_mm_connection_management(msg_p);
                  break;

               case TIMER_T3214:
               case TIMER_T3216:
                 mm_handle_security(msg_p);
                 break;
 
#ifdef FEATURE_FEMTO_CSG
               case TIMER_FORBIDDEN_OPERATOR_CSG_LIST:
                 mm_timer_forbidden_csg_list_expiry();
                 break;
#endif 

               case TIMER_POLICY_CHANGE_PENDING:
                 mm_timer_policy_change_pending_expiry();
                 break;
#if defined(FEATURE_DUAL_SIM) && defined(FEATURE_TDSCDMA)
               case TIMER_TDS_RAT_DEACTIVATION:
                 mm_tds_rat_status_set(SYS_RAT_ACTIVATION_STATUS_DEACTIVATED);
                 break;
#endif 

#ifdef FEATURE_DUAL_SIM
   			   case TIMER_MM_DUAL_SWITCH:
   			     if(mmcoord_pending_reg_message.cmd.hdr.message_id == MMR_STOP_MODE_REQ)
   			     {
   				   mmcoord_process_mmr_stop_mode_req(&mmcoord_pending_reg_message);
   			     }
   			     break;
#endif 
                case TIMER_FRLTE:
                  MSG_HIGH_DS_0(MM_SUB, "=MM= Timer FRLTE expired");
                 break;  
               default:
                  MSG_ERROR_DS( MM_SUB, "=MM= Unexpected timer id %d for timeout",
                       msg_p->cmd.timer_expiry.timer_id, 0,0);
                  break;
            }
         }
         else
         {
            MSG_ERROR_DS( MM_SUB, "=MM= Unexpected message id %d for timer message", msg_p->cmd.hdr.message_id, 0,0 );
         }
         break;

      
#ifdef FEATURE_TC
      case MS_MM_TC:
         if( msg_p->cmd.hdr.message_id == MMTC_UNITDATA_REQ)
         {
            mm_handle_test_control_data(msg_p);
         }
         else
         {
            MSG_ERROR_DS( MM_SUB, "=MM= Unexpected msg id %d from TC task", msg_p->cmd.hdr.message_id, 0,0);
         }
         break;
#endif

      case MS_WMS_MM :
        if( msg_p->cmd.hdr.message_id == MM_SMS_BEARER_SELECTION_CMD)
        {
          switch (msg_p->cmd.sms_bearer_selection_req.sms_bearer_selection)
          {
            case CS_PREFERRED :
            case PS_PREFERRED:
            case CS_ONLY :
            case PS_ONLY :
              mm_sms_preferred_domain_id =
                     msg_p->cmd.sms_bearer_selection_req.sms_bearer_selection ;
              break ;
            default:
              MSG_ERROR_DS(MM_SUB, "=MM= Invalid SMS Bearer selection",0,0,0) ;
              mm_sms_preferred_domain_id = PS_PREFERRED;
              break ;
          }

#ifdef FEATURE_SGLTE
          mm_nv_sms_preferred_domain_id = mm_sms_preferred_domain_id;

          if(MM_IS_IN_SGLTE_MODE)
          {
            mm_sms_preferred_domain_id = CS_PREFERRED;
          }
#endif

          MSG_HIGH_DS_1(MM_SUB ,"=MM= Current MO SMS Preferred Domain ID is %d",
                                              mm_sms_preferred_domain_id) ;
        }
        else
        {
          MSG_ERROR_DS( MM_SUB, "=MM= Unexpected msg id %d from WMS task",
                                              msg_p->cmd.hdr.message_id, 0,0) ;
        }
        break ;

#ifdef FEATURE_NAS_ECALL		
	case MS_CM_MM:
        if( msg_p->cmd.hdr.message_id == MMCM_ECALL_INACTIVITY_TRIGGER)
        {
          MSG_HIGH_DS_1(MM_SUB, "=MM= CM message received %d", msg_p->cmd.hdr.message_id);
          mm_ecall_dereg_in_wrong_state = FALSE;
          if(mm_timer_status[TIMER_T3242] == TIMER_ACTIVE)
          {
             mm_stop_timer(TIMER_T3242);
#ifdef FEATURE_DUAL_SIM
             if(MM_MODEM_STATISTICS_SUB1)
#endif
             {
               mm_timer_expiry_info.timer_bitmask |= SYS_MM_TIMER_T3242_INFO_MASK;
			   mm_set_signal_for_cm(SYS_NAS_TIMER_EXPIRY_INFO_MASK );
		       MSG_HIGH_DS_1(MM_SUB,"=MM= stat signal sent for timer expiry, mm ecall substate %d", mm_idle_substate);
             }
		     mm_perform_ecall_inactivity();
          }
          else if(mm_timer_status[TIMER_T3243] == TIMER_ACTIVE)
          {
             mm_stop_timer(TIMER_T3243);
#ifdef FEATURE_DUAL_SIM
             if(MM_MODEM_STATISTICS_SUB1)
#endif
             {
               mm_timer_expiry_info.timer_bitmask |= SYS_MM_TIMER_T3243_INFO_MASK;
	       mm_set_signal_for_cm(SYS_NAS_TIMER_EXPIRY_INFO_MASK );
		       MSG_HIGH_DS_1(MM_SUB,"=MM= stat signal sent for timer expiry, mm ecall substate %d", mm_idle_substate);
             }
	     mm_perform_ecall_inactivity();
          }
          else
          {
#ifdef FEATURE_DUAL_SIM
            if(MM_MODEM_STATISTICS_SUB1)
#endif
            {
              if(mm_idle_substate != MM_ECALL_INACTIVE)
              {
                mm_ecall_dereg_in_wrong_state = TRUE;
              }
              mm_set_signal_for_cm(SYS_NAS_REGISTRATION_INFO_MASK);
              MSG_HIGH_DS_1(MM_SUB,"=MM= stat signal sent for registartion status, mm ecall substate %d", mm_idle_substate);
            }
		    
          }
             	
        }
        if( msg_p->cmd.hdr.message_id == MMCM_ECALL_REMAINING_TIMER_VALUE)
        {
           
           current_lai.PLMN_id.identity[0] = mm_serving_plmn.info.plmn.identity[0];
           current_lai.PLMN_id.identity[1] = mm_serving_plmn.info.plmn.identity[1];
           current_lai.PLMN_id.identity[2] = mm_serving_plmn.info.plmn.identity[2];
           current_lai.location_area_code[0] = mm_serving_plmn.lac.lac[0];
           current_lai.location_area_code[1] = mm_serving_plmn.lac.lac[1];
           if(mm_ecall_mode_type == MMR_ECALL_MODE_ECALL_ONLY && (mm_timer_status[TIMER_T3242] == TIMER_STOPPED)
		   	             && (mm_timer_status[TIMER_T3243] == TIMER_STOPPED) &&  mm_state == MM_IDLE)
           {
		     mm_need_to_perform_ecall_inactivity = FALSE;

             if(mm_ecall_profile == TRUE)
             {
               ecall_timer_value = MIN((msg_p->cmd.cm_mm_ecall_remaining_timer_value.remaining_timer_value * 1000), (mm_ecall_inactivity_timer *60*1000)); 
             }
             else
             {
               ecall_timer_value = MIN((msg_p->cmd.cm_mm_ecall_remaining_timer_value.remaining_timer_value * 1000), 43200000); 
             }
             if(msg_p->cmd.cm_mm_ecall_remaining_timer_value.timer_id == SYS_TIMER_3242)
             {
               mm_start_timer(TIMER_T3242, ecall_timer_value);
             }
             else if(msg_p->cmd.cm_mm_ecall_remaining_timer_value.timer_id == SYS_TIMER_3243)
             {
               mm_start_timer(TIMER_T3243, ecall_timer_value);
             }
             if ((mm_location_information.location_update_status == UPDATED) && memcmp(mm_location_information.lai,
                              mm_serving_lai,
                              LAI_SIZE) == 0 && (mm_serving_plmn.info.service_status == SYS_SRV_STATUS_SRV))
             {
               MSG_HIGH_DS_0(MM_SUB,"=MM= UE is alreadu updated, just start the inactivity timer");
             }
             else 
             {
		       
    	       if (!(PLMN_MATCH( mm_serving_lai,
                               mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn.identity ) ||
                         (sys_eplmn_list_equivalent_plmn(mmcoord_pending_reg_message.cmd.mmr_reg_req.plmn) && 
                          sys_eplmn_list_equivalent_plmn(mm_serving_plmn.info.plmn))) &&
                         (mmcoord_pending_reg_message.cmd.mmr_reg_req.network_selection_mode ==
                                    SYS_NETWORK_SELECTION_MODE_LIMITED_MANUAL_ACQUISITION))
               {
                 MSG_HIGH_DS_0(MM_SUB,"=MM= Different PLMN found during MAN_LIMITED_ACQ");
               }
               else if ( (mm_check_forbidden_national_roaming(&current_lai)) ||
                         (mm_check_forbidden_regional_roaming(&current_lai)) || 
   #ifdef FEATURE_DUAL_SIM
                 (reg_sim_per_subs_plmn_forbidden(mm_serving_plmn.info.plmn,(sys_modem_as_id_e_type)mm_sub_id))
   #else
                 (reg_sim_plmn_forbidden(mm_serving_plmn.info.plmn))
   #endif
                      )
               {
                 MSG_HIGH_DS_0(MM_SUB,"=MM= Cell is forbidden");
               }
                                       
               else if (((mm_location_information.location_update_status == NOT_UPDATED) ||
                         (mm_location_information.location_update_status == UPDATING_DISABLED)) && mm_serving_plmn.info.service_status != SYS_SRV_STATUS_NO_SRV)
               {
                 mm_substate_control( MM_STD_UPDATE_ATTEMPTED );
                 if (mm_idle_substate == MM_ATTEMPTING_TO_UPDATE)
                 {
                           /* -----------------------------------------------------------
                              ** The attempt counter is reset when (in case of service state
                              ** ATTEMPTING TO UPDATE) location update is triggered by CM
                              ** sublayer requests (3GPP TS 24.008 4.4.4.5)
                              ** ----------------------------------------------------------- */
                   mm_clear_lu_attempt_counter();
                 }   
                 mm_initiate_lu( MM_LU_CAUSE_NORMAL );
              }
            }
           }
           MSG_HIGH_DS_0(MM_SUB,"=MM= stat signal sent for timer start response");
           mm_set_signal_for_cm(SYS_NAS_INACTIVITY_TIMER_RESUME_INFO_MASK);
        }
        break;
#endif		

      default:
         MSG_ERROR_DS(MM_SUB, "=MM= Unexpected message set %d", msg_p->cmd.hdr.message_set, 0,0);
         break;
   }

   /*
    * If RRC set is mapped as RR, put it back
    */
   if( rrc_message_set_mapped_as_rr == TRUE)
   {
      msg_p->cmd.hdr.message_set = MS_MM_RRC;
   }

}
