#ifndef TDSRRCCCM_H
#define TDSRRCCCM_H

/*===========================================================================
  R R C    C E L L    C H A N G E    M A N A G E R    H E A D E R    F I L E
DESCRIPTION

   This file contains all prototypes used by Cell Change Manager.

EXTERNALIZED FUNCTIONS   
   

      
INITIALIZATION AND SEQUENCING REQUIREMENTS
  None

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcccm.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ysh     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/


/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "rex.h"
#include "msg.h"

#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcmm_v.h"
#include "tdsrrcasn1.h"
#include "tdsrrcintif.h"
#include "tdsrrccspdb.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* This lists all possible substates for Cell Change Manager.
 * Note that these substates are independent of the Cell Camping status
 * of the UE. For Example CCM in substate Idle could either be when
 * the UE is Camped on a cell or Not Camped on a cell 
 */
typedef enum
{
  /* CCM in Idle state */
  TDSRRC_CCM_IDLE,
  /* Waiting for SIB Confirmation */
  TDSRRC_CCM_WAIT_SIB,
  /* Waiting for SIB Confirmation when another Cell Change Request is pending */
  TDSRRC_CCM_WAIT_SIB_REQ_PENDING,
  /* Waiting for a Channel Config Confirm from LLC */
  TDSRRC_CCM_WAIT_CHAN_CONFIG_CNF,
  /* Waiting for a Cell Reselection confirm from LLC */
  TDSRRC_CCM_WAIT_CELL_RESELECTION_CNF,
  /* max number of valid CCM states */
  TDSRRC_CCM_MAX_STATES
}tdsrrc_ccm_substate_e_type;

/* This enumerates the possible values for Cell Camping
 * status validity. The status must be checked for validity
 * before reading the camping info.
 */
typedef enum
{
  /* Status in Invalid and Cell Camping in progress */ 
  TDSSTATUS_INVALID_CELL_TO_BE_CAMPED,
  /* Status in Invalid and Cell Reselection is in progress */ 
  TDSSTATUS_INVALID_CELL_RESELECTION,
  /* Status in Valid */ 
  TDSSTATUS_VALID
}tdsrrc_ccm_cell_camping_status_valid_e_type;

/* Structure used to maintain the current cell camping info of UE.
 */
typedef struct
{
  /* Indicates if the status is valid. During processing of a Cell Change Req
   * the status is set to TDSSTATUS_INVALID_CELL_TO_BE_CAMPED or
   * TDSSTATUS_INVALID_CELL_RESELECTION. It is also initialized to 
   * TDSSTATUS_INVALID_CELL_TO_BE_CAMPED in the 
   * CCM inititialization.
   */
  tdsrrc_ccm_cell_camping_status_valid_e_type   status;
  /* Indicates the status of cell - it is set based on the Cell Change Req. 
   * It is initialized to TDSRRC_CELL_LOSS_NO_NEW_CELL in CCM initialization.
   */
  tdsrrc_new_cell_status_e_type  cell_camp_status;

  /* Indicates the Procedure who last requested a Cell Change */
  tdsrrc_proc_e_type   req_proc_id;

  /* Indicates requested cell change type */
  tdsrrc_cell_change_e_type  cell_change_type;

  /* Cell data for Current Cell is valid only if camped status is 
   * RRCCCM_CAMPED and the converse is also true. 
   */
  /* Cell data for current cell */
  tdsrrc_csp_acq_entry_type  curr_cell;

  mm_as_cell_access_status_T    curr_cell_access;
   
#ifdef FEATURE_DSAC
  /*  Current CS and PS cell access details are captured
   *  into this structure and curr_dsac_present variable 
   *  tells whether DSAC specifc cell access information
   *  is present (or) not
   */
  boolean curr_dsac_present;

  /* Cell Access information for CS */
  mm_as_cell_access_status_T curr_cs_cell_access;

  /* Cell Access information for PS */
  mm_as_cell_access_status_T curr_ps_cell_access;
#endif
  
  /* Cell data for previous cell */
  tdsrrc_csp_acq_entry_type  prev_cell;

  mm_as_cell_access_status_T    prev_cell_access; 

#ifdef FEATURE_DSAC
 /*  Previous CS and PS cell access details are captured
  *  into this structure and curr_dsac_present variable 
  *  tells whether DSAC specifc cell access information
  *  is present (or) not
  */
  boolean prev_dsac_present;

  /* Cell Access information for CS */
  mm_as_cell_access_status_T prev_cs_cell_access;

  /* Cell Access information for PS */
  mm_as_cell_access_status_T prev_ps_cell_access;
#endif

  /* Indicates if RRC Connection Establishment procedure asked for camping
   * status when we were not camped.
   */
  boolean  rce_found_status_not_camping;

  boolean       is_curr_cell_id_valid;

}tdsrrc_ccm_cell_camping_info_type;

/* This holds the current cell camping info for the UE */ 
extern tdsrrc_ccm_cell_camping_info_type   tdsrrc_ccm_cell_camping_info;


/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*===========================================================================

FUNCTION          RRC_CCM_UPDATE_WITH_CELL_LOSS 

DESCRIPTION       This function updates the Camping status in CCM while 
                  maintaining the CCM state.
  
DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None

===========================================================================*/
void tdsrrcccm_update_with_cell_loss(void);


/*===========================================================================
FUNCTION          RRCCCM_WAITING_FOR_RESELECTION_CNF

DESCRIPTION       This function returns a Boolean indicating if CCM is still
                  waiting for Cell Reselection Confirmation.
                  A State Change Indication could have put CCM to Idle substate
                  after it had sent the last Reselection Req. The function 
                  provides info to detect such an incidence.

DEPENDENCIES      None

RETURN VALUE      TRUE/FALSE

SIDE EFFECTS      None.
===========================================================================*/
boolean  tdsrrcccm_waiting_for_reselection_cnf(void);

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_FREQ

DESCRIPTION       This function is responsible for returning the current 
                  Camped on frequency of the UE.

DEPENDENCIES      Calling procedure needs to call 
                  tdsrrcccm_get_curr_camping_status before calling this function.
                  The returned value from this function is valid only if 
                  tdsrrcccm_get_curr_camping_status returns TDSRRC_CAMPED_ON

RETURN VALUE
                  Currently camped on frequency

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_freq_type tdsrrcccm_get_curr_camped_freq(void);

/*===========================================================================

FUNCTION          TDSRRC_CCM_GET_CURR_CAMPED_CPID

DESCRIPTION       This function is responsible for returning the current 
                  Camped on Primary Scrambling Code of the UE.

DEPENDENCIES      Calling procedure needs to call 
                  tdsrrcccm_get_curr_camping_status before calling this function.
                  The returned value from this function is valid only if 
                  tdsrrcccm_get_curr_camping_status returns TDSRRC_CAMPED_ON

RETURN VALUE
                  Currently camped on Primary Scrambling Code

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_cpid_type tdsrrcccm_get_curr_camped_cpid(void);

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_PLMN

DESCRIPTION       This function is responsible for returning the current 
                  Camped on PLMN of the UE.

DEPENDENCIES      NONE

RETURN VALUE
                  Currently camped on PLMN

SIDE EFFECTS      None.
===========================================================================*/
rrc_plmn_identity_type tdsrrcccm_get_curr_camped_plmn(void);

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_CELL_ID

DESCRIPTION       This function is responsible for returning the current 
                  Camped on cell id of the UE.

DEPENDENCIES      NONE

RETURN VALUE
                  Currently camped on cell

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_int_cell_id_type tdsrrcccm_get_curr_camped_cell_id(void);

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPING_STATUS

DESCRIPTION       This function is responsible for returning the current 
                  Camping Status of the UE.

DEPENDENCIES
                  None.

RETURN VALUE
                  The Current Camping Status as maintained by Cell Change Mgr.

SIDE EFFECTS      None.
===========================================================================*/
tdsrrc_camped_on_status_e_type  
tdsrrcccm_get_curr_camping_status(tdsrrc_proc_e_type req_proc_id);

/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CELL_ACCESS_STATUS

DESCRIPTION       This function is responsible for returning the 
                  cell access status for the currently camped on cell.
                  If not camped on a cell, a cell access status of 
                  RRC_NO_CELL_ACCESS is returned.

DEPENDENCIES
                  None.

RETURN VALUE
              
SIDE EFFECTS      None.
===========================================================================*/
mm_as_cell_access_status_T tdsrrcccm_get_curr_cell_access_status(void);


/*===========================================================================

FUNCTION          RRC_CCM_GET_SERV_PLMN_CELLID

DESCRIPTION       This function is responsible for returning the PLMN and 
                  Cell ID of the serving cell.

DEPENDENCIES
                  None

RETURN VALUE
                  A status that indicates whether there is a serving cell 
                  or not..
SIDE EFFECTS
                  The returned status MUST be checked by the caller before
                  reading the PLMN or Cell ID. If returned status indicates
                  No Serving Cell, PLMN and Cell ID will not be initialized.

===========================================================================*/
tdsrrc_new_cell_status_e_type
tdsrrcccm_get_serv_plmn_cellid(tdsrrc_int_cell_id_type *cell_id, rrc_plmn_identity_type *plmn_id);

/*===========================================================================
FUNCTION          RRC_CCM_IS_RESELECTION_IN_PROGRESS

DESCRIPTION       This function is responsible for returning the current 
                  status of Reselection in progress.
DEPENDENCIES
                  None

RETURN VALUE
                  A status that indicates if a Reselection is in progress
                  or not.
                  
SIDE EFFECTS      None
===========================================================================*/
boolean  tdsrrcccm_is_reselection_in_progress(void);

/*===========================================================================

FUNCTION          RRC_CCM_PROPAGATE_NEW_CELL_TO_ALL_PROCS

DESCRIPTION       This function is responsible for propagating the received 
                  TDSRRC_NEW_CELL_IND to all procedures that deserve to be 
                  informed of it. 
                  One of the procedures that would otherwise be informed can 
                  be over-ruled by putting its ID in the cmd. If a procedure's
                  ID appears as "No Ind Procedure ID" in the cmd, it is NOT
                  informed with TDSRRC_NEW_CELL_IND.
                  
DEPENDENCIES
                  None

RETURN VALUE
                  None
SIDE EFFECTS
                  A possible side effect could come from the order in which
                  procedures are informed. The Cell Selection Procedure must
                  always be informed (if not indicated otherwise in cmd)before
                  any other procedures.
===========================================================================*/
void  tdsrrcccm_propagate_new_cell_to_all_procs( tdsrrc_cmd_type  *cmd_ptr );


/*===========================================================================

FUNCTION          tdsrrcccm_init_cell_change_manager

DESCRIPTION       This function is responsible for initializing data 
                  structures associated with Cell Change Manager.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

NOTE
                  This function SHALL NOT send any cmd to any other procedure.
===========================================================================*/
void tdsrrcccm_init_cell_change_manager( void );


/*===========================================================================

FUNCTION          RRC_CCM_SEND_NAS_CN_INFO

DESCRIPTION       This function is responsible for sending the NAS specific
                  CN information to MM.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send RRC_SERVICE_IND successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type
tdsrrcccm_send_nas_cn_info
(
  tdsrrc_CN_InformationInfo* cn_information_info_ptr
   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  ,boolean primary_plmn_Id_present
   /*pointer to primary plmn i.d if present in OTA msg or NULL otherwise*/
  ,tdsrrc_PLMN_Identity *primary_plmn_Identity
);
/*===========================================================================

FUNCTION          RRC_CCM_BUILD_SERVICE_IND_WITH_NAS_CN_INFO

DESCRIPTION       This function is responsible for building service_ind with
                  the NAS specific CN information.

DEPENDENCIES
                  

RETURN VALUE
                  mm_cmd_type * cmd: mm command

SIDE EFFECTS      None.
===========================================================================*/

mm_cmd_type *
tdsrrcccm_buid_service_ind_with_nas_cn_info
(
  tdsrrc_CN_InformationInfo* cn_information_info_ptr
   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  ,boolean primary_plmn_Id_present
   /*pointer to primary plmn i.d if present in OTA msg or NULL otherwise*/
  ,tdsrrc_PLMN_Identity *primary_plmn_Identity
);



/*===========================================================================

FUNCTION          tdsrrcccm_buid_service_ind_with_nas_cn_info_r6

DESCRIPTION       This function is responsible for building service_ind with
                  the NAS specific CN information.

DEPENDENCIES
                  

RETURN VALUE
                  mm_cmd_type * cmd: mm command

SIDE EFFECTS      None.
===========================================================================*/
mm_cmd_type *
tdsrrcccm_buid_service_ind_with_nas_cn_info_r6
(
  tdsrrc_CN_InformationInfo_r6* cn_information_info_ptr
);

/*===========================================================================

FUNCTION          RRC_CCM_SEND_NAS_CN_INFO_R6

DESCRIPTION       This function is responsible for sending the NAS specific
                  CN information to MM.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send RRC_SERVICE_IND_FROM_OTA_MSG successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type tdsrrcccm_send_nas_cn_info_r6
(
  tdsrrc_CN_InformationInfo_r6* cn_information_info_ptr
);


/*===========================================================================

FUNCTION          RRC_CCM_SEND_NAS_CN_INFO_FULL

DESCRIPTION       This function is responsible for sending the NAS specific
                  CN FULL information to MM.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send RRC_SERVICE_IND successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type
tdsrrcccm_send_nas_cn_info_full
(
  tdsrrc_CN_InformationInfoFull* cn_information_info_full_ptr
   /*Boolean used to indicate primary plmn i.d is present in OTA msg*/
  ,boolean primary_plmn_Id_present
   /*pointer to primary plmn i.d if present in OTA msg or NULL otherwise*/
  ,tdsrrc_PLMN_Identity *primary_plmn_Identity
);


/*===========================================================================

FUNCTION          RRC_CCM_SEND_L1_DRX_REQ

DESCRIPTION       This function is responsible for sending TDSL1_CPHY_DRX_REQ
                  to L1.

DEPENDENCIES
                  

RETURN VALUE
                  SUCCESS: Able to send TDSL1_CPHY_DRX_REQ successfully.
                  FAILURE: Not able to send due to any of the multiple reasons.

SIDE EFFECTS      None.
===========================================================================*/
uecomdef_status_e_type tdsrrc_ccm_send_l1_drx_req(void);


/*===========================================================================
FUNCTION          tdsrrcccm_update_cell_camped_status

DESCRIPTION       This function is responsible for putting the 
                  "Cell camp status" to TDSRRC_NEW_CELL_SELECTED
                  when that status type is passed as a parameter. 
                  Also, new cell indication is sent to RCE if it was waiting
                  for UE to camp on

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrcccm_update_cell_camped_status(tdsrrc_new_cell_status_e_type status);

#ifdef FEATURE_INTERRAT_HANDOVER_G2TD
/*===========================================================================
FUNCTION          tdsrrcccm_update_for_g2td_handover

DESCRIPTION       This function will update the CCM camping information.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void
tdsrrcccm_update_for_g2td_handover(tdsrrc_csp_acq_entry_type * acq_entry_ptr);
#endif


/*===========================================================================
FUNCTION          tdsrrcccm_update_for_inter_freq_hard_handover

DESCRIPTION       This function will update the CCM camping information with
                  the new frequency and scrambling code after a successful
                  inter-frequency hard handover.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/
void
tdsrrcccm_update_for_inter_freq_hard_handover (tdsrrc_freq_type freq,
                                              boolean cpid_present,
                                             tdsrrc_cpid_type pri_cpid);



/*===========================================================================

FUNCTION          tdsrrcccm_force_init

DESCRIPTION       This function is responsible for initializing data 
                  structures associated with Cell Change Manager.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

NOTE
                  This function SHALL NOT send any cmd to any other procedure.
===========================================================================*/
void tdsrrcccm_force_init( void );


/*===========================================================================

FUNCTION          tdsrrcccm_cell_change_manager

DESCRIPTION       This function is responsible for handling all commands 
                  destined for Cell Change Manager.

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

===========================================================================*/
void  tdsrrcccm_cell_change_manager( tdsrrc_cmd_type  *cmd_ptr );

/*===========================================================================

FUNCTION          RRC_CCM_CELL_CHANGE_TO_SAME_CELL

DESCRIPTION       This function returns a boolean that tells whether UE
                  camped on the same cell as before.
DEPENDENCIES
                  None

RETURN VALUE
                  boolean true : cell is same as before
                         false : cell is different

SIDE EFFECTS
                  None.

===========================================================================*/
boolean tdsrrcccm_cell_change_to_same_cell(void);


/*===========================================================================

FUNCTION          RRC_CCM_UPDATE_SERVICE_INFO

DESCRIPTION       This function updates service info OTA sent to MM

DEPENDENCIES
                  None

RETURN VALUE
                  None

SIDE EFFECTS
                  None.

===========================================================================*/
void tdsrrcccm_update_service_info(mm_cmd_type *service_info_ota_ptr);

#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
/*===========================================================================

FUNCTION          RRC_CCM_PROPAGATE_NEW_CELL_TO_WAITING_PROC

DESCRIPTION       This function is responsible for sending the NEW_CELL_IND to the
                  procedure waiting for it.
                  
DEPENDENCIES
                  None

RETURN VALUE
                  None
SIDE EFFECTS
                  None
===========================================================================*/
void tdsrrcccm_propagate_new_cell_to_waiting_proc
(
  tdsrrc_proc_e_type proc_id,
  boolean cell_same_as_before,
  boolean inform_procedure_rce
);
#endif /* FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU */


/*===========================================================================

FUNCTION          RRC_CCM_GET_CURR_CAMPED_COMMON_PLMN

DESCRIPTION       This function is responsible for updating plmn_id with common PLMN_ID if curr cell is shared

DEPENDENCIES      NONE

RETURN VALUE
                  TRUE      If Curr cell is shared
                  FALSE     Otherwise

SIDE EFFECTS      None.
===========================================================================*/
boolean tdsrrcccm_get_curr_camped_common_plmn
(
  rrc_plmn_identity_type *plmn_id
);
/*===========================================================================

FUNCTION          tdsrrcccm_backup_service_ind

DESCRIPTION     This function is responsible for backup service ind from RRC_SERVICE_IND, 
                       RRC_SERVICE_IND_FROM_OTA_MSG, or RRC_SERVICE_CNF

DEPENDENCIES
                  
RETURN VALUE
                  None

SIDE EFFECTS
                  None
===========================================================================*/

void tdsrrcccm_backup_service_ind(rrc_to_mm_sink_e_type  cmd_id, 
                                         mm_cmd_type *mm_buf_ptr);

#endif
