#ifndef TDSRRCSIBPROC_H
#define TDSRRCSIBPROC_H
/*===========================================================================
                        R R C   S I B   P R O C E S S I N G  

DESCRIPTION

  This is the external header file for the SIB processing module.
  This module is to be used by other RRC modules only and not by
  tasks other than RRC.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/inc/tdsrrcsibproc.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsrrccmd_v.h"
#include "tdsrrcsibproci.h"
#include "tdsseq.h"
#include "sd.h"
#include "mm_umts.h"
#include "sys_eplmn_list.h"
/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */
#define TDSSIB4_PRESENT_BIT_MASK 0x40                                                          
#define TDSSIB11_PRESENT_BIT_MASK 0x20
#define TDSSIB6_PRESENT_BIT_MASK 0x10

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
 #define TDSMIB_POSITION 8
 #define TDSMAX_TIME_MIB_WAIT 4
#endif

/* Invert mask after TDSRRCSIB_MASK_INVERT_FACTOR
   times maximum rep rate */
#define TDSRRCSIB_MASK_INVERT_FACTOR 10

/* Transition buffer added to invert mask timer */
#define TDSRRCSIB_TRANS_BUFFER 100

/* Empty mask duration between switching masks */
#define TDSRRCSIB_EMPTY_FRAMES 100

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT)
#define TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(msg_ptr, msg_type_prefix1,msg_type_prefix2)\
        ((msg_ptr->m.v6b0NonCriticalExtensionsPresent)&&\
        (msg_ptr->v6b0NonCriticalExtensions.\
        msg_type_prefix2##_v6b0ext.m.extSIBTypeInfoSchedulingInfo_ListPresent)\
        )
        

#define TDSEXTENSION_SIB_SCHED_PRESENT_IN_MIB(msg_ptr)\
    ((msg_ptr->m.v690NonCriticalExtensionsPresent)&&\
       (msg_ptr->v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent)&&\
       (msg_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.\
          masterInformationBlock_v6b0ext.m.extSIBTypeInfoSchedulingInfo_ListPresent)\
    )
#endif


#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING

#define DEFERRED_MEAS_SUPPORT(sib3_ptr)\
   ((sib3_ptr->m.v4b0NonCriticalExtensionsPresent)&&\
      (sib3_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent)&&\
       (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v5c0NoncriticalExtensionPresent)&&\
          (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.\
          v5c0NoncriticalExtension.m.v670NonCriticalExtensionPresent)&&\
            (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.\
            v5c0NoncriticalExtension.v670NonCriticalExtension.m.v770NonCriticalExtensionPresent)&&\
              (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v5c0NoncriticalExtension.\
              v670NonCriticalExtension.v770NonCriticalExtension.sysInfoType3_v770ext.\
              m.deferredMeasurementControlReadingSupportPresent)\
              )

#define INTRA_FREQ_MEAS_QUAN_FOR_DEFERRED_SIB(sib3_ptr)\
   ((sib3_ptr->m.v4b0NonCriticalExtensionsPresent)&&\
      (sib3_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent)&&\
       (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v5c0NoncriticalExtensionPresent)&&\
          (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.\
          v5c0NoncriticalExtension.m.v670NonCriticalExtensionPresent)&&\
            (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.\
            v5c0NoncriticalExtension.v670NonCriticalExtension.m.v770NonCriticalExtensionPresent)&&\
              (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.\
              v5c0NoncriticalExtension.v670NonCriticalExtension.v770NonCriticalExtension.\
              sysInfoType3_v770ext.m.deferredMeasurementControlReadingSupportPresent)&&\
                (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v5c0NoncriticalExtension.\
                v670NonCriticalExtension.v770NonCriticalExtension.sysInfoType3_v770ext.\
                deferredMeasurementControlReadingSupport.m.modeSpecificInfoPresent)&&\
                  (sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.\
                  v5c0NoncriticalExtension.v670NonCriticalExtension.v770NonCriticalExtension.\
                  sysInfoType3_v770ext.deferredMeasurementControlReadingSupport.\
                  modeSpecificInfo.t == T_tdsrrc_DeferredMeasurementControlReadingSupport_modeSpecificInfo_tdd)\
                  )

#endif

#ifdef FEATURE_TDSCDMA_SIB19
#define TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_MIB(msg_ptr)\
  ((msg_ptr->m.v690NonCriticalExtensionsPresent)&&\
    (msg_ptr->v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent)&&\
      (msg_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent)&&\
        (msg_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.v860NonCriticalExtensions.masterInformationBlock_v860ext.m.extSIBTypeInfoSchedulingInfo_ListPresent)\
  )


#define TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(msg_ptr, msg_type_prefix1,msg_type_prefix2)\
  ((msg_ptr->m.v6b0NonCriticalExtensionsPresent)&&\
    (msg_ptr->v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent)&&\
      (msg_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.msg_type_prefix2##_v860ext.m.extSIBTypeInfoSchedulingInfo_ListPresent)\
  )

#endif


#ifdef FEATURE_3GPP_CSFB_TDS
#define TDS_DEFAULT_SIB_MASK  (MIB_BIT|SB_1_BIT|SB_2_BIT|SIB_1_BIT|SIB_2_BIT|SIB_3_BIT|SIB_4_BIT|SIB_5_BIT|SIB_6_BIT|SIB_7_BIT|SIB_11_BIT|SIB_12_BIT|SIB_18_BIT)
#endif
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
typedef enum
{
  TDSRRCSIB_ABORT_SIB7_UPDATE,
  TDSRRCSIB_ABORT_DUE_TO_OOS,
  TDSRRCSIB_ABORT_DUE_TO_NGH_OOS,
  TDSRRCSIB_ABORT_DUE_TO_TD2TD_BPLMN,

  TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH,                  /*This will abort sib reading & will teardown S-PCCPCH*/
  TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH_NO_PCCPCH_DROP, /*This will abort sib reading & will not teardown S-PCCPCH*/

  TDSRRCSIB_ABORT_DUE_TO_DL_WK_IND
}tdsrrcsib_event_abort_cause_e_type;

//setup cause for channel_config_req
typedef enum
{
  TDSSBCCH_SETUP,
  TDSSBCCH_TEARDOWN,
  TDSSBCCH_NOOP,
  TDSNBCCH_SETUP,
  TDSNBCCH_TEARDOWN,
  TDSNBCCH_NOOP
} tdsrrcsib_bcch_action_type;

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
typedef enum
{
  TDSDEFERRED_SIB11,
  TDSDEFERRED_SIB11_BIS,
  TDSDEFERRED_SIB12,
  TDSDEFERRED_ANY_ALLOWED_SIB,
  TDSDEFERRED_UPDATE_SIB
}tdsrrcsib_deferred_sib_type;
#endif

typedef struct
{
  tdsrrc_MasterInformationBlock *mib;
  tdsrrc_SysInfoType1           *sib1;
  tdsrrc_SysInfoType3           *sib3;
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
  tdsrrc_SysInfoTypeSB1        *sb1;
  tdsrrc_SysInfoTypeSB2        *sb2;
#endif
}tdsrrcsib_bplmn_sibs_type;

/* Command store for buffered processing */
typedef struct {
  boolean pending_flag;
  tdsrrc_cmd_type saved_cmd;
} tdsrrcsib_cmd_store_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                        Function Declarations
** ======================================================================= */

extern boolean tdsrrcrce_is_rce_procedure_active(void);

/*===========================================================================

FUNCTION RRC_SIB_EVENT_CONTROLLER

DESCRIPTION

  This is the main function that controls the SIB processing module.
  The SIB processing module is event based, i.e it waits for events
  to be received from other RRC modules and does the required processing
  for each event. This function receives events from the RRC dispatcher
  and processes the events as needed. 

DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  BCCH logical channels may be set up and teared down. 
===========================================================================*/
void tdsrrcsib_event_controller
(
  tdsrrc_cmd_type *cmd_ptr  /* Pointer to command */
);


/*===========================================================================

FUNCTION RRC_SIB_PROC_INIT

DESCRIPTION

  This function initializes the SIB processing procedure.
  This function needs to be called once only, at powerup.
  The SIB database and the SIB events database are cleared
  by this function.

DEPENDENCIES

  None.

RETURN VALUE

  None.
  
SIDE EFFECTS

  None.

===========================================================================*/
extern void tdsrrcsib_proc_init( void );




/*===========================================================================

FUNCTION RRCSIB_CHANGE_SERVING_CELL_STATUS

DESCRIPTION

  This function is used to give information about any change
  in the serving cell due to cell selection or cell reselection.
  The calling procedure needs to indicate whether a new cell
  has been successfully selected. If so, it should give the PLMN
  and cell ids for the cell. Only after all the required
  sibs for that cell have been read, this function should be
  called.
  
  This function must be used ONLY by the cell selection and
  reselection procedure.
  
DEPENDENCIES

  None.

RETURN VALUE

  SUCCESS if the serving cell status was changed correctly.
  FAILURE if the PLMN and cell id can't be found in the database.
  
SIDE EFFECTS

  None.

===========================================================================*/
uecomdef_status_e_type tdsrrcsib_change_serving_cell_status
(
  boolean new_cell_selection_succeeded,
            /* set to TRUE if a new cell has been selected through
            cell selection or reselection. Set to FALSE if a new
            cell reselection failed. */
  boolean existing_serving_cell_valid,
            /* Set to TRUE if we can go back to the existing serving
            cell. This is only used if new_cell_selection_succeeded
            is set to FALSE. Set to FALSE if the existing cell is
            no longer valid and the new cell selection failed. */
  rrc_plmn_identity_type plmn,    
           /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell       
           /* The cell id for the cell */
);



/*===========================================================================

FUNCTION RRCSIB_CHECK_AND_UPDATE_SIB7_VALIDITY

DESCRIPTION

  This function checks if the SIB7 for the current active
  cell is valid or not. If it's not valid it will generate 
  a command to read SIB7 again. Then it will send a 
  command TDSRRC_SIB7_UPDATE_STATUS_IND to indicate to the
  calling procedure if the SIB7 read succeeded.
  
DEPENDENCIES

  None.

RETURN VALUE

  TRUE if SIB7 is valid. FALSE if SIB7 is not valid. If
  FALSE is returned a TDSRRC_SIB7_UPDATE_STATUS_IND will be sent
  to the calling procedure. 
  
SIDE EFFECTS

  BCCH logical channel may be enabled to read SIB7.

===========================================================================*/
boolean tdsrrcsib_check_and_update_sib7_validity
(
  tdsrrc_proc_e_type  procedure  /* The procedure that needs the SIB7 */
);


/*===========================================================================

FUNCTION RRC_SIB_PROC_VARIABLES_INIT

DESCRIPTION

  This function initializes the SIB variables to their
  initial state. This may be called at powerup as well as
  when the procedure needs to be re-initialized such as
  due to Factory Test Mode or Low Power Mode.

DEPENDENCIES

  None.

RETURN VALUE

  None.
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_sib_proc_variables_init( void );

/*===========================================================================

FUNCTION RRC_SIB_PROC_EVENTS_INIT

DESCRIPTION

  This function initializes the SIB events to their
  initial state. This may be called at powerup as well as
  when the procedure needs to be re-initialized such as
  due to Factory Test Mode or Low Power Mode.

DEPENDENCIES

  None.

RETURN VALUE

  None.
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsib_proc_events_init( void );

/*===========================================================================

FUNCTION RRCSIB_FORCE_SIB_EVENT_INIT

DESCRIPTION

  This function will be used in the following 2 scenarios
  
  1. When RCE needs to abort SIB procedure from reading the SIBs when it had earlier
     requested to update SIB7
     
     Action : SIB procedure will clean up the event specific variables associated
     It will delete all the SIB's that it had partially read for that event. It will
     send CNF with failure to RCE. It will send a CHANNEL_CFG_REQ to LLC procedure 
     to tear down the S-BCCH.
     
  2. CSP can call this function when it detects Out of service area
  
     Action : SIB will clean up all variables associated with the active event. It will
     delete all the sibs that it had partially read. Send FAILURE to the procedure which
     requested it. No need to send a tear down to LLC as by this time all the channels
     would already have been torn down.
  
DEPENDENCIES

  None.

RETURN VALUE

  
  
SIDE EFFECTS

  

===========================================================================*/
void tdsrrcsib_force_sib_event_init(tdsrrcsib_event_abort_cause_e_type abort_cause) ;

/*===========================================================================

FUNCTION RRCSIB_CHECK_MIB_READ_STATUS

DESCRIPTION

  This is the function get's the current active index of SIB_EVENTS.
  Takes care of pending configuration for a particular event.
  
DEPENDENCIES
 
  None.

RETURN VALUE
  TRUE:  If MIB is already read
  FALSE: If MIB is yet to be read
  
SIDE EFFECTS

  None.
 
===========================================================================*/
boolean tdsrrcsib_check_mib_read_status(boolean for_serv_cell);
/*===========================================================================

FUNCTION RRCSIB_GET_SIB_7_UPDATE_STATUS

DESCRIPTION

  API to provide info whether SIB_7 update is going on or not.
    
DEPENDENCIES

  None.

RETURN VALUE

SUCCESS:  SIB_7 update is on-going
FAILURE:  SIB_7 update is not-on-going  
  
SIDE EFFECTS

  

===========================================================================*/
uecomdef_status_e_type tdsrrcsib_get_sib_7_update_status(void);

/*===========================================================================

FUNCTION RRC_BPLMN_RETURN_SIB_FOR_CELL

DESCRIPTION

  This function returns a pointer to the requested SIB.
  if this particular SIB has not been read completely for that cell, 
  this function returns NULL.
  
  The pointer is typecast to void since different sibs have
  different type definitions. The calling function is responsible
  for typecasting the pointer to the required SIB type.
   
DEPENDENCIES

  NONE

RETURN VALUE

  Pointer to the requested SIB. 
  "NULL" if the requested SIB has not been read.
  

SIDE EFFECTS

  None.

===========================================================================*/
void * tdsrrcsib_bplmn_return_sib_for_cell
(
  tdsrrc_SIB_type  req_sib          /* The SIB that is required */
);

/*===========================================================================

FUNCTION RRCSIB_FREE_BPLMN_SIBS

DESCRIPTION

  This function frees the memory used by a System Information
  Block stored in the BPLMN SIB database using the ASN.1 free pdu function.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsib_free_bplmn_sibs
(
  void
);

/*===========================================================================

FUNCTION RRCSIB_IS_SIB_ACTIVE

DESCRIPTION

  Determines whether any event is active in SIB or not.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcsib_is_sib_active
(
  void
);

#if  defined(FEATURE_TDSCDMA_DRX_MANAGER)
/*===========================================================================

FUNCTION RRCSIB_GET_SIB_SLEEP_SCHEDULE_TABLE_LOCATION

DESCRIPTION

  This function returns the location to the sib sleep table.This 
  would be passed to L1 in the TDSL1_CPHY_SETUP_REQ for N-BCCH 
  
DEPENDENCIES
 
  None.

RETURN VALUE

  Pointer to the table

SIDE EFFECTS

  None.
     
===========================================================================*/
uint8* tdsrrcsib_get_sib_sleep_schedule_table_location
(
  void
);

#endif 

/*===========================================================================

FUNCTION  tdsrrcsib_update_sixhr_sib_timer

DESCRIPTION

  This function will Implement the required six hours 
  Timer for each of the valid sib index.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sixhr_sib_timer(void);

/*===========================================================================

FUNCTION tdsrrcsib_is_need_to_cleanup_cell

DESCRIPTION

  This function will check whether the six hours Criteria needs 
  to be checked for this sib event.
  
DEPENDENCIES
  None.

RETURN VALUE

  TRUE: If six hours Criteria needs to be checked 
  FALSE: Other wise

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_is_need_to_cleanup_cell(tdssib_events_index_e_type event_index);


/*===========================================================================

FUNCTION tdsrrcsib_is_event_sixhrs_sib_update

DESCRIPTION

  This function will check whether the event is UPDATE_SIBs
  due to six hrs sib timer expiry
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If this event is UPDATE_SIBs due to six hrs sib timer expiry
  FALSE: Other wise

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_is_event_sixhrs_sib_update
(
tdssib_events_index_e_type  event_index
);
/*===========================================================================

FUNCTION tdsrrcsib_is_sixhrs_sib_timer_expired

DESCRIPTION

  This function will check whether the event is UPDATE_SIBs
  return TRUE if six hrs sib timer has expired for serving cell
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If six hrs sib timer has expired for serving cell
  FALSE: Other wise

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_is_sixhrs_sib_timer_expired
(
tdssib_events_index_e_type  event_index
);
/*===========================================================================

FUNCTION tdsrrcsib_is_current_event_due_to_sfn_drift

DESCRIPTION

  This function will check whether the event is UPDATE_SIBs
  due to SFN drift
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: UPDATE_SIBs is due to SFN drift
  FALSE: Other wise

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_is_current_event_due_to_sfn_drift
(
void
);
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
/*===========================================================================

FUNCTION tdsrrcsib_can_sib7_event_start

DESCRIPTION

  This function will check whether the sib-7 event be started.
  condition for it is UE must be in FACH state & none of other 
  sib events are active.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If sib-7 event can be started.
  FALSE: Other wise

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_can_sib7_event_start
(
void
);
/*===========================================================================

FUNCTION tdsrrcsib_is_current_event_sib7_in_fach

DESCRIPTION

  This function will check whether the current event is sib-7 event 
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If event is sib-7 event in FACH.
  FALSE: If event is other than sib-7 event.

SIDE EFFECTS
  None.
     
===========================================================================*/
 boolean tdsrrcsib_is_current_event_sib7_in_fach
 (
 void
 );
/*===========================================================================

FUNCTION tdsrrcsib_build_sib7_schedule_table

DESCRIPTION

  This function updates the SIB schedule table
  for SIB-7 only.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_build_sib7_schedule_table
(
  tdsrrc_SIB_type sib_type,                /* Indicates whether this update is from MIB or SBs */
  tdssib_events_index_e_type event_index   /* Index of the current event */
);
#endif
/*===========================================================================
FUNCTION tdsrrc_sib_is_sib_segment_needed_by_rrc_fach
DESCRIPTION
  This function will check whether the currently received 
  SIB segment by L1 is needed by RRC or not.If its not 
  needed then L1 will not forward this sib segment to RRC.

  The input to this function is SFN, of currently received 
  SIB segment.
DEPENDENCIES
  None.
RETURN VALUE
  TRUE: If this sib segment is needed by RRC.
  FALSE: other wise.
SIDE EFFECTS
  None.
===========================================================================*/
 boolean tdsrrc_sib_is_sib_segment_needed_by_rrc_fach
 (
 uint16 sfn
 );

#if  defined(FEATURE_TDSCDMA_DRX_MANAGER) || defined(FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH)
/*===========================================================================

FUNCTION tdsrrcsib_gen_bit_mask_for_schedule_table

DESCRIPTION

  This function will generate Bit mask needed for filling 
  SIB schedule table
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
uint8 tdsrrcsib_gen_bit_mask_for_schedule_table
(
  uint8 byte_offset
);
#endif
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
/*===========================================================================

FUNCTION tdsrrcsib_check_event_completion_status_for_cell_id_read

DESCRIPTION

  This function checks the status of the cell id read sib event
  in cell dch state of RRC, the sib event index is passed to this
  function. If all SIBs(i.e.SIB3 only) for the event have been received, 
  it will return TRUE. Else it will return FALSE.
    
DEPENDENCIES
 
  None.

RETURN VALUE

  TRUE is event has been completed, else FALSE.

SIDE EFFECTS

  None.
  
===========================================================================*/
boolean tdsrrcsib_check_event_completion_status_for_cell_id_read
(
  uint32 index
);
#endif

/*===========================================================================
FUNCTION tdsrrcsib_is_current_event_sib_read_in_dch
DESCRIPTION
  This function will check whether the current event is sib reading in dch event 
  DEPENDENCIES
  None.
RETURN VALUE
  TRUE: If event is sib reading in DCH.
  FALSE: otherwise.
SIDE EFFECTS
  None.
===========================================================================*/
boolean tdsrrcsib_is_current_event_sib_read_in_dch
(
void
);

/*===========================================================================
FUNCTION RRC_SIB_GET_PROC_ID_FOR_SIB_READ_IN_DCH
DESCRIPTION
  This function will return the proc id which has initiated sib reading in dch
  DEPENDENCIES
  None.
RETURN VALUE
  Proc id  If event is sib reading in DCH.
  PROC_NONE: otherwise.
SIDE EFFECTS
  None.
===========================================================================*/
tdsrrc_proc_e_type tdsrrcsib_get_proc_id_for_sib_read_in_dch
(
  void
);


#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
/*===========================================================================

FUNCTION tdsrrcsib_check_if_extension_sib_present

DESCRIPTION

  This function checks the status of the sib event, the index of
  which is passed to this function. If all SIBs for the event
  have been received, it will return TRUE. Else it will return FALSE.
    
DEPENDENCIES
 
  None.

RETURN VALUE

  TRUE is event has been completed, else FALSE.

SIDE EFFECTS

  None.
  
===========================================================================*/
boolean tdsrrcsib_check_if_extension_sib_present
(
  uint32 dbindex    /* Database index    */
);
#endif
#ifdef FEATURE_TDSCDMA_SIB19
/*===========================================================================

FUNCTION rrcsib_check_if_sib19_present

DESCRIPTION

  This function checks whether scheduling information for SIB19 is present in
  MIB/SB1/SB2 of the cell stored in the dbindex passed as argument.
    
DEPENDENCIES
 
  None.

RETURN VALUE

  TRUE is SIB19 is scheduled, else FALSE.

SIDE EFFECTS

  None.
  
===========================================================================*/
boolean tdsrrcsib_check_if_sib19_present
(
  uint32 dbindex    /* Database index    */
);
#endif

/*===========================================================================

FUNCTION tdsrrcsib_check_if_connected_mode_sibs_present

DESCRIPTION

  This function will be used to find if SIB4/6/12 Ind present in 
  encoded format of SIB3/5/11
DEPENDENCIES

  None.

RETURN VALUE

  TRUE - If SIB4/6/12 Ind present in SIB3/5/11
  FALSE - Otherwise
  
SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrcsib_check_if_connected_mode_sibs_present
(
  byte val,
  tdsrrc_SIB_type sib_type
);

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
/*===========================================================================
FUNCTION tdsrrcsib_can_s_bcch_torndown_in_fach_to_pch_transition
DESCRIPTION
  This function will return status on whether S-BCCH needs to be dropped at FACH to PCH transition
  DEPENDENCIES
  None.
RETURN VALUE
TRUE - Drop S-BCCH
FALSE - otherwise
SIDE EFFECTS
  None.
===========================================================================*/
boolean tdsrrcsib_can_s_bcch_torndown_in_fach_to_pch_transition
(
  void
);
#endif

/*===========================================================================
FUNCTION tdsrrcsib_decode_duplicate_sib7

DESCRIPTION

  This function decodes the duplicate copy of sib7 
  received for the on-going event.
  
DEPENDENCIES
  None

RETURN VALUE
  Boolean: True, if decoding successful, else false

SIDE EFFECTS
  None.

===========================================================================*/


boolean tdsrrcsib_decode_duplicate_sib7
(
  tdssib_events_index_e_type event_index,
  uint32 pdu_num
);

/*===========================================================================
FUNCTION tdsrrcsib_decode_sib7_in_update_sibs

DESCRIPTION

  This function decodes and saves the sib7 
  received for UPDATE SIBS event.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrrcsib_decode_sib7_in_update_sibs
(
  tdssib_events_index_e_type index   /* Index to the SIB events database
                                  in which SIBs have been completed. */
);

/*===========================================================================
FUNCTION tdsrrcsib_initiate_cell_selection

DESCRIPTION

  This function sends initiate cell selection indication with cause as OOS, if current state
  is Disconnected/Connecting. If the current state is PCH/FACH, it sends CU initiate req with cause 
  as Reentered Service Area.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/

void tdsrrcsib_initiate_cell_selection
(
  void
);

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/*===========================================================================
FUNCTION tdsrrcsib_can_nw_support_deferred_sib

DESCRIPTION

  This function checks whether Deferred SIB reading is supported by the UTRAN or not. This is done 
  by looking at the IE 'Deferred measurement reading support' present in SIB 3.
  
DEPENDENCIES
  None

RETURN VALUE
  Boolean: True, if Deferred SIB reading is supported, else false

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrrcsib_can_nw_support_deferred_sib
(
  int32 dbindex
);


/*===========================================================================
FUNCTION tdsrrcsib_is_sib11_11bis_12_read_deferred

DESCRIPTION

  This function checks whether reading of SIB11/11bis/12 is deferred for the serving cell.
  
DEPENDENCIES
  None

RETURN VALUE
  Boolean: True, if reading of SIB11/11bis/12 is deferred, else false

SIDE EFFECTS
  None.

===========================================================================*/
boolean tdsrrcsib_is_sib11_11bis_12_read_deferred
(
  tdsrrcsib_deferred_sib_type deferred_sib_type
);
#endif

/*===========================================================================

FUNCTION tdsrrcsib_initialize_supported_sibs_lookup_table

DESCRIPTION

  Initializes lookup table of sibs that are supported.

DEPENDENCIES
 
  None.

RETURN VALUE
 
  None.

SIDE EFFECTS

  None.
        
===========================================================================*/
void tdsrrcsib_initialize_supported_sibs_lookup_table
(
  void
);

#ifdef FEATURE_3GPP_CSFB_TDS

#ifdef FEATURE_3GPP_CSFB_SKIP_OPT_TDSCDMA_SIBS
/*===========================================================================

FUNCTION  rrc_sibdb_invalidate_sib_v_tag_for_csfb

DESCRIPTION

  This function will invalidate the sibdb for requested sib mask

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrc_sibdb_invalidate_sib_v_tag_for_csfb
(
  uint32  req_sib_bit_mask,          /* SIB's  that are required to be invalidated*/
  rrc_plmn_identity_type plmn,       /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell          /* The cell id for the cell */
);
#endif

/*===========================================================================

FUNCTION  rrcsib_read_system_info_container

DESCRIPTION

  This function will perform all the required processing needed once SIBs have been read and the SIB event is to be 
  completed. 

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_read_sibs_from_sys_info_container
(
  tdsrrc_cmd_type *cmd_ptr   /* Pointer to received SIB event */
);

/*===========================================================================

FUNCTION rrc_sib_post_complete_for_csfb_sib_read

DESCRIPTION

  This function posts complete to CSP on CSFB SIB read completed.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

===========================================================================*/

void tdsrrc_sib_post_complete_for_csfb_sib_read
(
   tdsrrc_sib_status_e_type status /* Status of the SIB event-success or failure*/
);

#endif

/*===========================================================================

FUNCTION rrcsib_is_event_update_sibs_active

DESCRIPTION

  This function will check whether the event is UPDATE_SIBs
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If this event is UPDATE_SIBs due to deferred SIB reading.
  FALSE: Other wise

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_is_event_update_sibs_active
(
  void
);

/*===========================================================================

FUNCTION RRCSIB_GET_SIB7_EXP_TIMER_VALUE

DESCRIPTION

  This function determines the value of the expiration
  timer for SIB7 for the serving cell.
  
  The expiration time is defined as -
  
  MAX( 320ms, SIB_REP*Expiration Time Factor)

  This function finds SIB_REP of SIB7 from
  the MIB or the Scheduling Blocks and the
  Expiration Time Factor from SIB7 and calculates
  the value of the timer.

DEPENDENCIES
 
  None.

RETURN VALUE

  SIB7 expiration timer value in milliseconds.

SIDE EFFECTS

  None.
   
===========================================================================*/
unsigned long  tdsrrcsib_get_sib7_exp_timer_value
(
  void
);

#ifdef FEATURE_TDSCDMA_SPECIFIC_SIB_READING
/*===========================================================================
FUNCTION    tdsrrcsib_is_serving_sib7_reading 

DESCRIPTION
  This function Check the SIB only waiting for SIB7 reading
  
DEPENDENCIES
  None

RETURN VALUE
  TRUE : Only SIB7 reading.
  FALSE: Not only SIB7 reading.

SIDE EFFECTS
  None

===========================================================================*/
boolean tdsrrcsib_is_serving_sib7_reading
(
void
);
/*===========================================================================
FUNCTION    tdsrrcsib_build_sib7_schedule_info_for_random_access 

DESCRIPTION
  This function is used to udpate the SIB7 position to L1.
  
DEPENDENCIES
  None

RETURN VALUE


SIDE EFFECTS
  None

===========================================================================*/
uint8* tdsrrcsib_build_sib7_schedule_info_for_random_access
(
void
);
#endif

/*===========================================================================

FUNCTION tdsrrcsib_send_empty_table

DESCRIPTION

  Send empty table to TL1 if boolean is set else masks 1s with generated table
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_send_empty_table
(
  tdssib_events_index_e_type event_index,
  boolean is_empty
);

/*===========================================================================

FUNCTION tdsrrcsib_send_dummy_sleep_schedule_table

DESCRIPTION

  This function sends a dummy sleep schedule table to L1 to enforce sleep-wakeup activity.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_send_dummy_sleep_schedule_table
(
void
);

/*===========================================================================

FUNCTION tdsrrcsib_check_and_update_manual_search_bst

DESCRIPTION

  MPLMN Search Time Optimization by using BST table and solution to overcome limitation
  This function will call API sd_update_manual_search_bst()to update PLMN info to SD
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None

SIDE EFFECTS

  None
  
===========================================================================*/
void tdsrrcsib_check_and_update_manual_search_bst(tdssib_events_index_e_type index);

/*===========================================================================

FUNCTION RRC_SIB_DATABASE_INIT

DESCRIPTION

  This function initializes the SIB database to it's
  initial state. This may be called at powerup as well as
  when the procedure needs to be re-initialized such as
  due to Factory Test Mode or Low Power Mode.

DEPENDENCIES

  None.

RETURN VALUE

  None.
  
SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsib_database_init( void );
#endif /* TDSRRCSIBPROC_H */

