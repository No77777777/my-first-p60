
#ifndef EMMSECURITY_H
#define EMMSECURITY_H
/*===========================================================================

              EMM SECURITY HEADER FILE

DESCRIPTION
  This file contains data and function declarations necessary for the
  GMM Authentication procedure implementation .

Copyright (c) 2001,2002,2003 Qualcomm Technologies, Incorporated and its licensors.  All Rights
Reserved.  QUALCOMM Proprietary.  Export of this technology or software
is regulated by the U.S. Government. Diversion contrary to U.S. law prohibited.
===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/mmcp.mpss/7.9.0/nas/mm/src/emm_security.h#2 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/27/11   PA      Added changes for Security Key generation during W2L PS HO
02/01/10   PA      Added changes for Security Key generation during L2W PS HO
02/02/10   RI      Fixed the path for the storage of EPS context.
01/22/10   RI      Added support for handling multiple EPS contexts.
10/20/09   RI      Added security context state update when in connected mode.
10/06/09   RI      Fix compiler warnings.
05/22/09   RI      Added support for NAS security.
04/16/09   RI      Initial Revision
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "mmcp_variation.h"
#include<customer.h>
#ifdef FEATURE_LTE
#include "comdef.h"
#include "customer.h"
#include "mmauth.h"
#include "emm_database.h"
#include "lte_nas_emm_message_type.h"
#include "emm_connection_handler.h"
#include "emm_connection_type.h"
#include "mm_sim.h"
#include "lte_nas_security_info.h"
#include "mm_multimode_common.h"
#include "emm_irat_if_msg.h"
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
#define EPS_NATIVE_SECURITY_CONTEXT_FILE ( char *)"/nvm/alpha/modem/nas/lte_nas_emm_eps_native_context"
#define EPS_MAPPED_SECURITY_CONTEXT_FILE (char *)"lte_nas_emm_eps_mapped_context"

#define MAX_NAS_COUNT_IN_BYTES      4

#define MAX_NAS_KEY_LENGTH_IN_BYTES 16

#define MAX_KASME_SIZE_IN_BYTES     32

struct emm_pended_msg_list_tag;

typedef struct 
{

  byte                          nas_kasme[32];  // NAS KASME

  byte                          nas_ul_cnt[4];  // NAS UL COUNT
    
  byte                          nas_dl_cnt[4];  // NAS DL COUNT

  lte_nas_emm_keyset_identifier_type   nasKSI;  // Key Set Identifier

  lte_nas_emm_security_alg_type security_alg;    // NAS Integrity and Ciphering Algorithms

} emm_efs_security_context_info;

void emm_perform_authentication ( byte *nas_ksi,
                                  auth_rand_type *p_rand,
                                  auth_autn_type *autn
                                 );

void emm_load_security_context(emm_ctrl_data_type *emm_ctrl_data_ptr);

void emm_invalidate_nas_security_params(emm_ctrl_data_type *emm_ctrl_data_ptr);

void emm_update_eps_security_context( emm_ctrl_data_type  *emm_ctrl_data_ptr);

void emm_process_auth_request ( lte_nas_emm_auth_req *emm_auth_request,
                                emm_ctrl_data_type   *emm_ctrl_data_ptr
                              );

void emm_process_authentication_reject(emm_ctrl_data_type   *emm_ctrl_data_ptr);

 void emm_process_identity_request( lte_nas_emm_identity_req  *identity_request,
                                    emm_ctrl_data_type *emm_ctrl_data_ptr
                                   );
boolean emm_security_msg_tx_fail
(
  lte_emm_connection_cause_type  cause,
  dword                          trans_id,
  emm_failure_type               emm_failure_cause,
  lte_nas_tai_lst1_type          tai,
  struct emm_pended_msg_list_tag *message_ptr,
  emm_ctrl_data_type             *emm_ctrl_data_ptr
);


lte_nas_ul_security_info_type *emm_get_ul_security_info(emm_ctrl_data_type *emm_ctrl_data_ptr, lte_nas_ul_security_info_type *nas_security);

lte_nas_dl_security_info_type *emm_get_dl_security_info(emm_ctrl_data_type *emm_ctrl_data_ptr, lte_nas_dl_security_info_type *nas_security, boolean smc,byte *incoming_ota_message);

boolean emm_update_usim_eps_security_context( emm_security_context_info *context_info);

boolean emm_get_efs_security_context(emm_efs_security_context_info *security_context_ptr);

boolean emm_write_efs_security_context_to_efs(emm_efs_security_context_info* efs_security_context_ptr);

boolean emm_write_efs_security_context(emm_security_context_info *context_info);

void emm_delete_mapped_context(void);

void emm_process_security_mode_cmd(lte_nas_emm_security_mode_command *emm_security_mode_cmd,
                                   emm_ctrl_data_type                *emm_ctrl_data_ptr);

void emm_process_guti_realloc_cmd(lte_nas_emm_guti_realloc_cmd *emm_guti_realloc_cmd,
                                  emm_ctrl_data_type           *emm_ctrl_data_ptr);

void emm_update_stored_security_context(emm_ctrl_data_type           *emm_ctrl_data_ptr,
                                        boolean is_invalid);

void emm_process_emm_info(lte_nas_emm_info     *emm_info,
                          emm_ctrl_data_type   *emm_ctrl_data_ptr);


emm_security_context_info *emm_get_context_by_state(emm_ctrl_data_type *emm_ctrl_data_ptr, 
                                                   lte_nas_context_state context_state);
emm_security_context_info *emm_get_context_by_type(emm_ctrl_data_type *emm_ctrl_data_ptr, 
                                                   lte_nas_context_type context_type);
void emm_reset_security_info (emm_security_context_info *security_info);

void emm_delete_efs_security_context(void);

void emm_get_ck_ik( byte *Ik,
                    size_t Ik_buf_len,
                    byte *Ck,
                    size_t Ck_buf_len);

boolean emm_get_srvcc_handover_auth_params( byte *eKSI,
                                         auth_gprs_cipherkey_type    *Ck,
                                         auth_gprs_integritykey_type *Ik );

boolean emm_compute_nas_int_key(emm_ctrl_data_type *emm_ctrl_data_ptr,
                                emm_security_context_info *context_ptr );

boolean emm_compute_nas_enc_key(emm_ctrl_data_type *emm_ctrl_data_ptr,
                                emm_security_context_info *context_ptr );

boolean emm_compute_ck_ik_from_kasme( emm_ctrl_data_type *emm_ctrl_data_ptr, 
                                      auth_gprs_integritykey_type *Ik,
                                      auth_gprs_cipherkey_type    *Ck, 
                                      boolean use_dl_count );

boolean emm_compute_srvcc_ck_ik_from_kasme( emm_ctrl_data_type *emm_ctrl_data_ptr, 
                                      auth_gprs_integritykey_type *Ik_srvcc,
                                      auth_gprs_cipherkey_type    *Ck_srvcc);

emm_security_context_info *emm_alloc_context(emm_ctrl_data_type *emm_ctrl_data_ptr, lte_nas_context_type context_type);

boolean emm_compute_khashasme_psho_mobility(byte *nonce_mme,
                                            size_t nonce_mme_buf_len,
                                            emm_ctrl_data_type *emm_ctrl_data_ptr, 
                                            emm_security_context_info *context_ptr);

void emm_rrc_process_lte_key_indication
(
  lte_rrc_nas_lte_key_ind_s *rrc_nas_lte_key_ind,
  emm_ctrl_data_type    *emm_ctrl_data_ptr
);

void emm_rrc_process_GW_key_indication
(
  lte_rrc_nas_umts_key_ind_s *rrc_nas_umts_key_ind,
  emm_ctrl_data_type    *emm_ctrl_data_ptr
);

void emm_invalidate_srvcc_ck_ik(void);

/*===========================================================================

FUNCTION    EMM_CANCEL_AUTHENTICATION

DESCRIPTION

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS

===========================================================================*/
void emm_cancel_authentication( void );

void emm_get_eps_params( mm_gw_identity_param_type *mm_identity);

void emm_get_eps_handover_auth_params( mm_gw_identity_param_type *mm_identity);

/*==================================================================================
FUNCTION EMM_STOP_AUTH_TIMERS

DESCRIPTION
  This function stops the MAK(TIMER_T3418) & SQN(TIMER_T3420) timers if running.

DEPENDENCIES
  None

RETURN VALUE
  TRUE: If MAK and/or SQN timer(s) were running and stopped
  FALSE: If MAK and/or SQN timer(s) are not running
  
SIDE EFFECTS
  None

===================================================================================*/
boolean emm_stop_auth_timers
(
  emm_ctrl_data_type   *emm_ctrl_data_ptr
);


/*===========================================================================
FUNCTION    EMM_MAKE_MAPPED_CTXT_CURRENT_AND_COMPUTE_CK_IK

DESCRIPTION
  This function makes the mapped EPS security context that was created when
  LTE_RRC_NAS_LTE_KEY_IND was received as the current EPS security context.
  If a previous current full native EPS security context exists, it is made non
  current. CK/IK are computed now using the K'ASME from the mapped EPS security
  context and stored into current mapped EPS security context.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void emm_make_mapped_ctxt_current_and_compute_ck_ik
(
  emm_ctrl_data_type    *emm_ctrl_data_ptr
);
#ifdef MMOC_API_PH_STAT_CNF_WITH_STATUS
extern void emm_abort_ongoing_procedure(boolean cancel_auth);
#endif

/*==================================================================================
FUNCTION EMM_DEACT_RETX_TIMER

DESCRIPTION
  This function instructs MM AUTH server to deactivate a particular EMM procedure timer
  (Attach(T3410), TAU(T3430), Service Request(T3417) or Detach(T3421)) if it was running
  and stopped during Authentication failure. This function is invoked when Emergency sesion
  is active and an EMM procedure completes. This will prevent the EMM procedure timer to be
  restarted upon Authentication timer (MAC/SQN)expiry.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

===================================================================================*/
extern void emm_deact_retx_timer
(
  timer_id_T   timer_id
);

extern void emm_process_guti_purge_ind(mm_cmd_type* message); 

/*===========================================================================
FUNCTION EMM_INVALIDATE_CURRENT_SECURITY_CTXT

DESCRIPTION
  This function invalidates the current security context in emm_ctrl_data_ptr. 

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void emm_invalidate_current_security_ctxt(emm_ctrl_data_type *emm_ctrl_data_ptr); 

/*===========================================================================
FUNCTION EMM_UPDATE_DL_NAS_COUNT_BITMAP

DESCRIPTION
  This function updates the DL NAS COUNT Bitmap (8 bits) to keep track of the
  receive status of last 8 consecutive (including the current DL NAS COUNT)
  DL NAS COUNTs from the network and is called only when the MAC-I check passed
  with the new(estimated) DL NAS COUNT.
  This function is not applicable for SMC message as the bitmap is reset to 0xFF
  directly inside emm_process_security_mode_cmd() upon SMC MAC-I check success.

  Bit 0 is mapped to the status of current DL NAS COUNT and bit 8 is mapped to
  the status of (current - 7)th DL NAS COUNT
  Any bit set to:
   1 - represents NAS message with corresponding DL COUNT is already received from the network
   0 - represents NAS message with corresponding DL COUNT is not yet received from the network

  @dl_nas_count_old - previous DL NAS COUNT stored in UE
  @dl_nas_count_old - DL NAS COUNT used to pass MAC-I check for the received message

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void emm_update_dl_nas_count_bitmap
(
  dword               dl_nas_count_old,
  dword               dl_nas_count_new,
  emm_ctrl_data_type  *emm_ctrl_data_ptr
);


/*===========================================================================
FUNCTION EMM_CHECK_REPLAYED_DL_NAS_COUNT

DESCRIPTION
  This function checks if the network is replaying a DL NAS COUNT.
  This function checks for the estimated DL NAS COUNTs falling in the range 
  [(stored DL NAS COUNT - 7) <-> stored DL NAS COUNT]. For DL NAS COUNTs
  smaller than (stored DL NAS COUNT - 7) this function returns FALSE.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if DL NAS COUNT is already used else FALSE

SIDE EFFECTS
  None
===========================================================================*/
extern boolean emm_check_replayed_dl_nas_count
(
  dword               stored_count,
  dword               estimated_count,
  emm_ctrl_data_type  *emm_ctrl_data_ptr
);


/*===========================================================================
FUNCTION EMM_ESTIMATE_DL_NAS_COUNT_FROM_SQN

DESCRIPTION
  This function estimates the DL NAS COUNT from the 4 LSB of received SQN
  from Network. 

DEPENDENCIES
  None

RETURN VALUE
  TRUE if DL NAS COUNT estimation is successful else FALSE

SIDE EFFECTS
  None
===========================================================================*/
extern boolean emm_estimate_dl_nas_count_from_sqn
(
  byte                          sqn,
  lte_nas_dl_security_info_type *nas_security,
  emm_ctrl_data_type            *emm_ctrl_data_ptr
);


/*===========================================================================
FUNCTION EMM_INITIALIZE_SEC_KEYS

DESCRIPTION
  Sets random values for IRAT related security keys in LTE.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void emm_initialize_sec_keys();
#endif  // FEATURE_LTE

#endif 
