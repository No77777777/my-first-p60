#ifndef TDSRRCMEAS_H
#define TDSRRCMEAS_H
/*===========================================================================
          R R C   M E A S U R E M E N T S   H E A D E R   F I L E

DESCRIPTION
  This file contains data types and declarations associated with the
  Measurement Control and Report Procedure.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/



/*===========================================================================

          EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcmeas.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20/11   yuh   Merged LTE code
03/10/11   zwj   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
11/08/10   zwj   Modify intra report quantity 10.3.7.41 for TDD. 
11/18/10   ysh   Frequency info save. 3GPP-8.6.6.1(3.4.2)
                                  Add module to get bland HHO or not.

===========================================================================*/



/*===========================================================================
**        Includes and Public Data Declarations
**=========================================================================*/

/*---------------------------------------------------------------------------
**        Include Files
**-------------------------------------------------------------------------*/
#include "tdscdma_variation.h"
#include "tdsrrccmd_v.h"


/*===========================================================================
**        CONSTANT/DEFINE DECLARATIONS
**=========================================================================*/
#define TDSMAX_QHCS_RSCP 26
/*===========================================================================
**        ENUMERATED TYPES
**=========================================================================*/



/*===================================================================
          Enumerated Defn
====================================================================*/


/* Indicates if RACH report available */
typedef enum {
  TDSRRCMEAS_RACH_SUCCESSFUL, 
  TDSRRCMEAS_RACH_FAILURE    
} tdsrrcmeas_rpt_stat_e_type; 


/*===================================================================
          Defines
====================================================================*/



/*===================================================================
          DATA STRUCTURES
====================================================================*/



/*===================================================================
          FUNCTION DECLARATIONS
====================================================================*/
  

/*====================================================================

FUNCTION     RRCMEAS_INIT_PROCEDURE

DESCRIPTION  

  Initialization function which is called on initialization 
  of RRC task. This task registers for state change indication
  manager and also initializes stored variables. Also sets the
  sub state of the measurement state machine to TDSRRCMEAS_INITIAL 
          
          
DEPENDENCIES

    None.

RETURN VALUE

    None.

SIDE EFFECTS

    None

===========================================================================*/

void tdsrrcmeas_init_procedure
(
  void
);


/*===========================================================================

FUNCTION          RRCMEAS_PROCEDURE_EVENT_HANDLER

DESCRIPTION      
 
  This is the main function handling external events received by the
  RRC Measurement procedure. This function is called by the
  dispatcher whenever an event is received for this module. This 
  function checks the current RRC state and calls the event handler for
  that state. 
             
DEPENDENCIES

  None.
  
RETURN VALUE

  None.
  
SIDE EFFECTS

  None
===========================================================================*/
void tdsrrcmeas_procedure_event_handler
(
  tdsrrc_cmd_type*  cmd_ptr
); 


/*===========================================================================

FUNCTION RRCMEAS_APPEND_MEAS_RPT_ON_RACH

DESCRIPTION

  Appends the measured results to RACH messages if this
  information is requested by the UTRAN. First, it checks
  if the meas report can be appended to the measge. If true,
  then it invokes a tdsl1mcmd_process_immediate_meas_req to get 
  intra-freq meas reports from L1.
  
DEPENDENCIES

  RACH reporting must have been activated by utran in sib11 or sib12

RETURN VALUE

  rrc_meas_rpt_stat_e_type
  
SIDE EFFECTS

  None

===========================================================================*/

tdsrrcmeas_rpt_stat_e_type tdsrrcmeas_append_meas_rpt_on_rach
(
  tdsrrc_MeasuredResultsOnRACH* rrcmeas_rach_report_ptr,
  tdsrrc_proc_e_type proc_id,
  boolean exclude_monitored_cell_results
);
/*===========================================================================

FUNCTION   tdsrrcmeas_set_measurement_result_v4b0ext

DESCRIPTION 
  This function is called by every meas result to malloc memory. 
  25.331-RP-040477:both R4 measurement results and additional 
  measurement results(if exists) shall be contained in the single 
  IE "MeasuredResultsList-LCR-r4ext", regardless of their presence
  ranking.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_set_measurement_result_v4b0ext
(
 tdsrrc_UL_DCCH_Message* msg_ptr
);
/*===========================================================================

FUNCTION   tdsrrcmeas_close_measurement_result_v4b0ext

DESCRIPTION 
  This function is called before send meas report to check measreport_v4b0
  memory whether.used by additionalMeasuredResults_LCR.


DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_close_measurement_result_v4b0ext
(
 tdsrrc_UL_DCCH_Message* msg_ptr
);


/*===========================================================================

FUNCTION   RRCMEAS_SAVE_INTRA_INTER_FREQ_MEAS

DESCRIPTION 

  Tells L1 to save Meas Criteria before it goes to Idle Mode for DCH-->FACH
  directed cell measurements

DEPENDENCIES

  None

RETURN VALUE

  None  

SIDE EFFECTS

  
===========================================================================*/
void tdsrrcmeas_save_intra_inter_freq_meas
(
 void
);

/*==========================================================================
 FUNCTION   tdsrrcifreq_free_memory_allocated_for_cell_list_in_meas_ind

DESCRIPTION

  This function takes care of deallocating the cell list  which was allocated by L1 when it is 
  sending a inter frequency measurement indication to RRC
DEPENDENCIES

    None
RETURN VALUE

    None
SIDE EFFECTS

    None

===========================================================================*/
void tdsrrcifreq_free_memory_allocated_for_cell_list_in_meas_ind
(
  tdsrrc_cmd_type* cmd_ptr
);

/*===========================================================================

FUNCTION   tdsrrctvm_is_traffic_volume_indicator_to_be_set_in_cu

DESCRIPTION 
  This function checks if there is any TVM with measurement ID 4, if that measurement is event trigerred
  and valid in all states or all states except DCH, if any present calls the mac function to check if the
  traffic volume is above the configured threshold for event 4A for that measurement

DEPENDENCIES
  None

RETURN VALUE
  TRUE  if there is any TVM with measurement ID 4, if that measurement is event trigerred and valid in 
        all states or all states except DCH, if any present calls the mac function and if traffic volume
        is above the configured threshold for event 4A for that measurement
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrctvm_is_traffic_volume_indicator_to_be_set_in_cu
(
  void
);


/*====================================================================
FUNCTION: tdsrrcmeas_init_dynamic_ptrs()

DESCRIPTION:
  This function is called to allocate dynamic ptrs

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcmeas_init_dynamic_ptrs
(
  void
);
/*====================================================================
FUNCTION: tdsrrcmeas_free_dynamic_ptrs()

DESCRIPTION:
  This function is called to free dynamic ptrs

DEPENDENCIES:
  None.

RETURN VALUE:
  None
  
SIDE EFFECTS:

====================================================================*/
void tdsrrcmeas_free_dynamic_ptrs
(
  void
);

/*===========================================================================

FUNCTION   tdsrrcmeas_set_intra_meas_quan_info

DESCRIPTION 
  This function is set intrafreq meas quantity info.If point TDDList is not 
  NULL. When point TDDList is NULL means initialization intra freq 
  meas quantity.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_set_intra_meas_quan_info
(
 /* Set intra meas quantity to L1 */
 tdsl1_intra_freq_meas_quan_struct_type *meas_quan,
 /* Intra meas quantity from ASN1 */
 tdsrrc_IntraFreqMeasQuantity_TDDList            *TDDList
);

/*===========================================================================

FUNCTION tdsrrcmeas_process_gps_pos_cell_info_req

DESCRIPTION
 Set cell information and send TDSCDMA_RRC_GPS_POS_CELL_INFO_RSP as a response
 of TDSCDMA_RRC_GPS_POS_CELL_INFO_REQ.

DEPENDENCIES
 None

RETURN VALUE
 None

SIDE EFFECTS
 None

===========================================================================*/
void tdsrrcmeas_process_gps_pos_cell_info_req 
(
  tdsrrc_cmd_type*  cmd_ptr,
  sys_modem_as_id_e_type as_id,
  uecomdef_status_e_type simid_validation
);


#ifdef FEATURE_TDSCDMA_ABSOLUTE_PRIORITY
/*===========================================================================

FUNCTION   tdsrrcmeas_update_dedicated_priority_info_from_ota

DESCRIPTION 
  This function updates the dedicated priority information stored with dedicated
  priority information in UTRAN Mobility Information (UMI) message.
  If dedicated priorities are configured, will start the validity timer also (T322)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_update_dedicated_priority_info_from_ota
(
  tdsrrc_DedicatedPriorityInformation *ded_pri_info_ptr
);


/*===========================================================================

FUNCTION   tdsrrcmeas_post_dedicated_priority_change_indication

DESCRIPTION 
  This function checks posts RRC_DED_PRI_CHANGE_IND to internal queue on reception of dedicated
  priority information in UTRAN Mobility Information (UMI) message

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_post_dedicated_priority_change_indication
(
  void
);

/*===========================================================================

FUNCTION   tdsrrcmeas_invalidate_dedicated_priority_info

DESCRIPTION 
  This function invalidates the dedicated priority information stored and stops the validity timer if running

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_invalidate_dedicated_priority_info
(
  void
);


/*===========================================================================

FUNCTION   tdsrrcmeas_update_dedicated_priority_info_from_irat_req

DESCRIPTION 
  This function updates the dedicated priority information stored with dedicated
   priority information received from interRAT message
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_update_dedicated_priority_info_from_irat_req
(
  lte_irat_dedicated_priority_info_s *irat_ded_priority_list,
  rrc_plmn_identity_type plmn_id
);

/*===========================================================================

FUNCTION   tdsrrcmeas_populate_dedicated_priority_info_for_irat_req

DESCRIPTION 
  This function populates the dedicated priorioty info in inter request with the stored values
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_populate_dedicated_priority_info_for_irat_req
(
  lte_irat_dedicated_priority_info_s *irat_ded_priority_info
);

/*===========================================================================

FUNCTION   tdsrrcmeas_is_update_eutra_priority_info_meas_need

DESCRIPTION 
  This function checks if need to update eutra priority info meas request to TL1

DEPENDENCIES
  None

RETURN VALUE
  boolean   TRUE -- need to update
            FALSE -- no need to update

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcmeas_is_update_eutra_priority_info_meas_need(void);

#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
/*===========================================================================

FUNCTION  tdsrrcmeas_send_clear_ded_pri_ind_set_owner

DESCRIPTION 
  This function checks the current owner of dedicated priorities and send the clear ind to the owner and
  sets owner
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_send_clear_ded_pri_ind_set_owner
(
  void
);
/*===========================================================================

FUNCTION   tdsrrcmeas_send_get_dedicated_priority_req

DESCRIPTION 
  This function checks the current owner of dedicated priorities and send the get req to the owner
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_send_get_dedicated_priority_req
(
  void
);

/*===========================================================================

FUNCTION   tdsrrcmeas_handle_clear_dedicated_pri_ind

DESCRIPTION 
  This function handles the clear dedicated priorities ind from inter RAT

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_handle_clear_dedicated_pri_ind
(
  void
);

#ifdef FEATURE_TDSCDMA_SUPPORT_LTE_DEPRIORITIZATION
/*===========================================================================

FUNCTION   tdsrrcmeas_send_lte_depri_requested_plmn_ind

DESCRIPTION 
  This function sends the NAS requested PLMN id from service_req to LTE RRC, so LTE RRC will clear LTE depriority info
  if needed.
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_send_lte_depri_requested_plmn_ind
(
  rrc_plmn_identity_type rrc_plmn_id
);
#endif
#endif

/*===========================================================================

FUNCTION   tdsrrcmeas_validate_sib19_priority_info

DESCRIPTION 
  This function validates the SIB19 priority info

DEPENDENCIES
  None

RETURN VALUE
  Boolean

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcmeas_validate_sib19_priority_info
(
  void
);

#endif

/*===========================================================================

FUNCTION   tdsrrcmeas_process_pseudo_lte_list_update_ind

DESCRIPTION 
  This function process NAS TDSRRC_PSEUDO_LTE_LIST_UPDATE_IND

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_process_pseudo_lte_list_update_ind
(
  tdsrrc_cmd_type *cmd_ptr
);

/*===========================================================================

FUNCTION   tdsrrc_check_eutradetect_ie

DESCRIPTION 
  This function returns TRUE if eutra detection IE is set in SIB19 otherwise returns FALSE
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

boolean tdsrrc_check_eutradetect_ie
(
  void
);
#ifdef FEATURE_TDSCDMA_DEDICATED_PRI_INFO

/*===========================================================================

FUNCTION   rrcmeas_handle_gsm_get_dedicated_pri_req

DESCRIPTION 
  This function handles the get dedicated priorities req from inter RAT

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/

void tdsrrcmeas_handle_gsm_get_dedicated_pri_req
(
  tdsrrc_rr_cmd_type* cmd_ptr
);

#if 0

/*===========================================================================

FUNCTION   tdsrrcmeas_send_clear_ded_pri_ind_set_owner_gsm

DESCRIPTION 
  This function checks the current owner of dedicated priorities and send the clear ind to the owner and
  sets owner
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_send_clear_ded_pri_ind_set_owner_gsm
(
  void
);

/*===========================================================================

FUNCTION   tdsrrcmeas_send_get_dedicated_priority_req_to_gsm

DESCRIPTION 
  This function checks the current owner of dedicated priorities and send the get req to the owner
   
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrcmeas_send_get_dedicated_priority_req_to_gsm
(
  void
);

#endif
#endif
#endif /* TDSRRCMEAS_H */
