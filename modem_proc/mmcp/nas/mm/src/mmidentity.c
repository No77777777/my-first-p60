/*===========================================================================
            COPYRIGHT INFORMATION

Copyright (c) 2001 Qualcomm Technologies, Incorporated and its licensors.  All Rights 
Reserved.  QUALCOMM Proprietary.  Export of this technology or software 
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.

            EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$PVCSPath:  L:/src/asw/MSM5200/mm/vcs/mmidentity.c_v   1.5   01 Jul 2002 11:36:14   jault  $   
$Header: //components/rel/mmcp.mpss/7.9.0/nas/mm/src/mmidentity.c#1 $    $DateTime: 2021/06/15 22:40:27 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/07/01   ts      Standardized MSG_ERROR and MSG_HIGH usage.
04/24/02   jca     Removed errant call to mm_state_control().
02/10/03   cd      Fixed Lint errors
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
#include "timers_v.h"
#include "gs_v.h"
#include "ghdi_exp.h"
#include "ghdi_exp_int.h"
#include "libprot.h"
#include "mm_v.h"
#include "mm_umts_v.h"
#include "err.h"
#include "mm_ssgccs_api.h"
#include "mm_ssgccs.h"
#ifdef FEATURE_NAS_REL11
#include "gmm_msg.h"
#include "mm_v.h"
#include "emm_database.h"
#include "mm_multimode_common.h"
#endif

/*===========================================================================

FUNCTION MM_PACK_NO_IDENTITY

DESCRIPTION
  This function packs the mobile identity of type NO_IDENTITY

DEPENDENCIES
  None

RETURN VALUE
   Number of octets encoded.

SIDE EFFECTS
  None

===========================================================================*/

uint32 mm_pack_no_identity(byte *write_ptr)
{
    /* Set Identity type field (TS 24.008 Table 10.5.4) */
    /* code the length for  NO_IDENTITY */
   *write_ptr = IDENTIFICATION_PROC_NO_IDENTITY_LEN;
   write_ptr++;

   /* code as NO_IDENTITY */
   *write_ptr = 0x08;

   return (IDENTIFICATION_PROC_NO_IDENTITY_LEN + 1);
}

/*===========================================================================

FUNCTION MM_RETURN_IDENTITY

DESCRIPTION
  This function return the identity as requested by the identity request
  message. This can be either the IMSI, TMSI, or IMEI. Function is called
  with a pointer to an IDENTITY_REQUEST messsage
 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void mm_return_identity( mm_cmd_type* msg_p)
{
   byte                       *write_ptr, *start_ptr;
   boolean                    send_msg = TRUE;
   byte                       identity_type;
   byte 					  temp_identity = NO_IDENTITY;   
   
#ifdef FEATURE_NAS_REL11
   byte                       i;
 
#ifdef FEATURE_LTE
   emm_ctrl_data_type *emm_ctrl_data_ptr = NULL;
   mm_gw_identity_param_type gw_identity;
   emm_ctrl_data_ptr = emm_db_get_ctrl_data();   
#endif
#endif

   write_ptr = (byte *)modem_mem_calloc(1,sizeof( identity_response_msg_T ),MODEM_MEM_CLIENT_NAS);
   mm_check_for_null_ptr((void*)write_ptr);

   // Keep a pointer to remember start of the message after message header
   start_ptr = write_ptr;

   // Skip IMH_T for OTA message, which is 4 bytes
   write_ptr += sizeof(IMH_T);
   
   // Pack PD and mesage identity
   *write_ptr++ = PD_MM;
   *write_ptr++ = IDENTITY_RESPONSE;

   identity_type = ((identity_request_msg_T *)((void*)&msg_p->cmd.hdr))->identity_type & 0x07;

#ifdef FEATURE_QSH_EVENT_METRIC
  mm_qsh_ota_metric_signaling_log(IDENTITY_REQUEST,CN_TO_UE_DIRECTION,identity_type, MM_GMM_NO_FAILURE_DEBUG);
#endif
   
   if (identity_type == IMSI )
   {
     /*  Request is for IMSI */
     if(mm_serving_plmn.info.sim_state == SYS_SIM_STATE_NOT_AVAILABLE)
     {
       /* IMSI is not there. So copy NO_IDENTITY and advance the pointer */
       write_ptr += mm_pack_no_identity(write_ptr);     
	   temp_identity=NO_IDENTITY;
     }
     else
     {
       /* Copy IMSI and advance the pointer */
       write_ptr += mm_pack_imsi(write_ptr);
	   temp_identity=IMSI;
     }
   }
   else if (identity_type == TMSI)
   {
      /*  Request is for TMSI */
     if ( !has_valid_tmsi() )
     {
       /* TMSI is not there. So copy NO_IDENTITY and advance the pointer */
       write_ptr += mm_pack_no_identity(write_ptr);          
     }
     else
     {
       /* Copy TMSI and advance the pointer */
       write_ptr += mm_pack_tmsi(write_ptr);          
	   temp_identity=TMSI;
     }
   }
   else if (identity_type == IMEI )
   {

      mm_read_imei( write_ptr );

      write_ptr += *write_ptr + 1;
	  temp_identity=IMEI;

   }
   else if (identity_type == IMEISV )
   {
      mm_read_imeisv( write_ptr);
      write_ptr += *write_ptr + 1;
	  temp_identity=IMEISV;
   }

  /*---------------------------------------------------------------------------------------------------------------------
  **Spec 24.008 4.3.3.2 Upon receipt of the IDENTITY REQUEST message with the Identity Type IE indicating that 
  **  P-TMSI, RAI and P-TMSI signature are being requested:
  **    If the MS does not support S1 mode, it shall handle IDENTITY RESPONSE as follows::
  **      If the MS holds a valid P-TMSI and RAI, the MS shall indicate the P-TMSI in the Mobile identity IE 
  **      and shall indicate the RAI in the Routing area identification IE. In addition, the MS shall include the
  **      P-TMSI type IE with P-TMSI type set to "native P-TMSI". If the MS holds a valid P-TMSI signature, 
  **      it shall include it in the P-TMSI signature IE.  
           
  **   If an MS that supports S1 mode shall handle IDENTITY RESPONSE as follows:
  **     If the TIN indicates "P-TMSI" or "RAT related TMSI" and the MS holds a valid P-TMSI and RAI, 
  **     the MS shall indicate the P-TMSI in the Mobile identity IE and shall indicate the RAI in the Routing area
  **     identification IE. In addition, the MS shall include the P-TMSI type IE with P-TMSI type set to "native P-TMSI"
  **     If the MS holds a valid P-TMSI signature, it shall include it in the P-TMSI signature IE.
  **--------------------------------------------------------------------------------------------------------------------**/
#ifdef FEATURE_NAS_REL11
   else if (identity_type == PTMSI_RAI_PTMSI_SIG)
   {
     /* Request is for PTMSI, RAI and PTMSI_SIGNATURE */
	   temp_identity=PTMSI_RAI_PTMSI_SIG;
      if(!(mm_checkif_rat_pri_list_has_rat(&mm_rat_pri_list_info, SYS_RAT_LTE_RADIO_ACCESS))
#ifdef FEATURE_LTE                     
            || (emm_ctrl_data_ptr->TIN == NAS_MM_TIN_P_TMSI || 
                 emm_ctrl_data_ptr->TIN == NAS_MM_TIN_RAT_RELATED_TMSI)
#endif         
         )
       {
         if( !(mm_has_valid_ptmsi() && has_valid_rai()))
         {
     /* PTMSI or RAI is not there. So copy NO_IDENTITY and advance the pointer */
           write_ptr += mm_pack_no_identity(write_ptr);
		   temp_identity=NO_IDENTITY;
         }
         else
         {
#ifdef FEATURE_SGLTE
           mm_as_id_e_type local_as_id;
           local_as_id = mm_as_id;
           if(MM_IS_IN_SGLTE_MODE && !MM_IS_IN_SGLTE_STATE_4)
           {
             mm_set_as_id(mm_ps_stack_id);
           }
#endif
           write_ptr += mm_pack_ptmsi(write_ptr);
           if(mm_nv_nas_rel_compliance >= NV_NAS_COMPLIANCE_REL11)
           {
            /*-----------------------------------------------------------------------------------------------------------
            **If the MS holds a valid P-TMSI and RAI, the MS shall indicate the P-TMSI in the Mobile identity IE 
            **and shall indicate the RAI in the Routing area identification IE. In addition, the MS shall include the
            **P-TMSI type IE with P-TMSI type set to "native P-TMSI". If the MS holds a valid P-TMSI signature, 
            **it shall include it in the P-TMSI signature IE.  
            **-----------------------------------------------------------------------------------------------------------*/
          
            *write_ptr++ = 0xE0;
            *write_ptr++ = GMM_IEI_RAI2;
            *write_ptr++ = GMM_IEI_RAI2_LENGTH;
            *write_ptr++ = gmm_stored_rai.plmn_id.identity[0];
            *write_ptr++ = gmm_stored_rai.plmn_id.identity[1];
            *write_ptr++ = gmm_stored_rai.plmn_id.identity[2];
            *write_ptr++ = gmm_stored_rai.location_area_code.lac[0];
            *write_ptr++ = gmm_stored_rai.location_area_code.lac[1];
            *write_ptr++ = gmm_stored_rai.routing_area_code;
            
            if(mm_has_valid_ptmsi_signature())
            {
              *write_ptr++ = GMM_IEI_PTMSI_SIG2;//IEI
              *write_ptr++ = GMM_IEI_PTMSI_SIG2_LENGTH; 
              *write_ptr++ = gmm_stored_ptmsi_signature.value[0];
              *write_ptr++ = gmm_stored_ptmsi_signature.value[1];
              *write_ptr++ = gmm_stored_ptmsi_signature.value[2];
            }
          }
#ifdef FEATURE_SGLTE
          mm_set_as_id(local_as_id);
#endif
        }
      }
#ifdef FEATURE_LTE
       else 
       {
         if((emm_ctrl_data_ptr->TIN == NAS_MM_TIN_GUTI &&
                emm_ctrl_data_ptr->emm_guti_valid == TRUE ))
         {
           /*-------------------------------------------------------------------------------------------------------------
           ** If the TIN indicates "GUTI" and the MS holds a valid GUTI, the MS shall map the GUTI into a P-TMSI, 
           ** P TMSI signature and RAI as specified in 3GPP TS 23.003 [4]. The MS shall indicate the P-TMSI in the 
           ** Mobile identity IE. In addition, the MS shall include the mapped RAI in the Routing area identification IE 
           ** and the mapped P-TMSI signature in the P-TMSI signature IE. In addition, the MS shall include the 
           ** P-TMSI type IE withP-TMSI type set to "mapped P-TMSI". 
           **-------------------------------------------------------------------------------------------------------------*/
#ifdef FEATURE_SGLTE
           mm_as_id_e_type local_as_id;
           local_as_id = mm_as_id;
           if(MM_IS_IN_SGLTE_MODE && !MM_IS_IN_SGLTE_STATE_4)
           {
             mm_set_as_id(mm_ps_stack_id);
           }
#endif
   
           emm_get_mapped_gw_identity(&gw_identity, emm_ctrl_data_ptr);
           *write_ptr++ = PTMSI_SIZE +1; /* ptmsi_size + type field */
           *write_ptr++ = 0xF4; /* type */
           for(i=2; i<(PTMSI_SIZE+2); i++) /* copy mapped ptmsi to identity response mobile id */
           {
            *write_ptr++ = gw_identity.ptmsi.digit[i-2];
           }
           
           if(mm_nv_nas_rel_compliance >= NV_NAS_COMPLIANCE_REL11)
           {
             *write_ptr++ = 0XE1;
             *write_ptr++ = GMM_IEI_RAI2;
             *write_ptr++ = GMM_IEI_RAI2_LENGTH;
             *write_ptr++ = gw_identity.ra_id.plmn_id.identity[0];
             *write_ptr++ = gw_identity.ra_id.plmn_id.identity[1];
             *write_ptr++ = gw_identity.ra_id.plmn_id.identity[2];
             *write_ptr++ = gw_identity.ra_id.location_area_code.lac[0];
             *write_ptr++ = gw_identity.ra_id.location_area_code.lac[1];
             *write_ptr++ = gw_identity.ra_id.routing_area_code;
             *write_ptr++ = GMM_IEI_PTMSI_SIG2; /* IEI */
             *write_ptr++ = GMM_IEI_PTMSI_SIG2_LENGTH;
             for(i=0; i<PTMSI_SIGNATURE_SIZE; i++)
             {
               *write_ptr++ = gw_identity.ptmsi_signature.value[i];
             }
           } 
#ifdef FEATURE_SGLTE
           mm_set_as_id(local_as_id);
#endif
         }
         else
         {
            /* PTMSI or RAI is not there. So copy NO_IDENTITY and advance the pointer */
              write_ptr += mm_pack_no_identity(write_ptr);
         }
        }
#endif
     }
#endif   
 

   else
   {

      /*
       * This should not happen as it is trapped by check for protocol errors
       */
      MSG_ERROR_DS( MM_SUB, "=MM= Unexpected identity type %d", identity_type, 0,0);
      send_msg = FALSE;

   }

   if (send_msg)
   {
      dword length;
      length = write_ptr - start_ptr - sizeof(IMH_T);
               

      MSG_HIGH_DS_0(MM_SUB, "=MM= MM sent IDENTITY RESPONSE");
      
      mm_send_ota_message(start_ptr, length);
	  mm_ssgccs_handle_outgoing_msg(MM_IDENTITY_RESPONSE_MSG,temp_identity,mm_serving_plmn.info.active_rat );
   }
   modem_mem_free( start_ptr , MODEM_MEM_CLIENT_NAS);
}
