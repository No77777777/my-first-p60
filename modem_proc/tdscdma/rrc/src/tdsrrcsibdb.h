#ifndef TDSRRCSIBDB_H
#define TDSRRCSIBDB_H
/*===========================================================================
              R R C   S I B   D A T A B A S E   H E A D E R   M O D U L E 

DESCRIPTION

  This module is the header module for the SIB Database module. This
  contains the external interface functions to be used by
  by other RRC modules. This must not be used by modules outside RRC.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsibdb.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   ttl   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/

/* ==========================================================================
** Includes and Public Data Declarations
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdsrrcasn1.h"
#include "tdsrrcmm_v.h"
#include "tdsrrctmr.h"
#include "tdsrrcsibproci.h"
#include "tdsrrcsibproc.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/*The Index Value is set to 33(Multiplied by 10min) which is 5.30.Hrs that is less than 6hr
   Bcoz for serving cell we will update sibs once UE does Leaving Connected mode
   scan for going to IDLE.In case of UE is IDLE RRC connection may be established for doing 
   LAU or RAU, Since the periodicity at which these(LAU/RAU) are done is tunable by N/W, 
   So setting this timer half an hours(choosing half an hour doesnt have any logic) before its expiry*/
 #define TDSMAX_CELL_INDEX_COUNT_SIXHR_CELL_SEL_RESEL 33

/*Max Six hours sib cell counter = six hours*/
 #define TDSMAX_CELL_INDEX_COUNT_SIXHR 36

/*This is a 10min Timer, which is used to implemnt 6 hrs SIB refresh timer*/
 #define TDSRRCSIB_SIX_HR_SIB_UPDATE_TIMER 600000


/*invalid Value_tag Value for SIB*/
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
#define TDSINVALID_SIB_VALUE_TAG 255
#endif
/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
/* data struct to convert sib_sb type to sib_type and also to get value tags*/
typedef struct
{
  tdsrrc_SIB_type sib_type;
  uint32 sib_tag;
}tdsrrcsib_sib_type_vtag;  

typedef enum
{
  TDSRRCSIB_READ_SIBS_AFTER_VTAG_MATCH,
  TDSRRCSIB_READ_ALL_SIBS
}tdsrrcsib_vtag_match_type;  

 #define TDSRRCSIB_SIB_CHANGE_GUARD_TIMER_VALUE 1000
 #define TDSRRCSIB_PENDING_UPDATE_SIB_TIMER_VALUE 100

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
 #define TDSRRCSIB_SIB_CHANGE_GUARD_TMR_FOR_DEFERRED_SIB 100
#endif

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* =======================================================================
**                        Function Declarations
** ======================================================================= */
 
/*===========================================================================

FUNCTION RRC_SIBDB_RETURN_SIB_FOR_DB_INDEX

DESCRIPTION

  This function returns a pointer to the requested SIB for
  the requested cell index in the SIB database. If there is no cell
  for that particular index, or if this particular SIB has not been read 
  completely for that cell, this function returns
  NULL.
  
  The pointer is typecast to void since different sibs have
  different type definitions. The calling function is responsible
  for typecasting the pointer to the required SIB type.
   
  Caution: The calling function should read all the required
  data from the pointer as soon as the function returns since
  the SIB data could be overwritten if the serving cell changes
  or if a SIB change indication has been received.
  
DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  Pointer to the requested SIB. 
  "NULL" if the requested SIB has not been read or if there
  is no cell stored at the requested index.

SIDE EFFECTS

  None.

===========================================================================*/
void * tdsrrcsibdb_return_sib_for_db_index
(
  tdsrrc_SIB_type  req_sib,          /* The SIB that is required */
  uint32        rrc_sibdb_index   /* Index into the SIB database for
                                  the cell from which the SIB is required.
                                  Must be between 0 and RRC_MAX_SIBDB_INDEX */
);




/*===========================================================================

FUNCTION RRC_SIBDB_RETURN_SIB_FOR_CELL

DESCRIPTION

  This function returns a pointer to the requested SIB for
  For a requested Cell id and PLMN Id. If there are no sibs stored
  for that particular PLMN and Cell id combination, or if this particular 
  SIB has not been read completely for that cell, this function returns
  NULL.
  
  The pointer is typecast to void since different sibs have
  different type definitions. The calling function is responsible
  for typecasting the pointer to the required SIB type.
   
  Caution: The calling function should read all the required
  data from the pointer as soon as the function returns since
  the SIB data could be overwritten if the serving cell changes
  or if a SIB change indication has been received.
  
DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  Pointer to the requested SIB. 
  "NULL" if the requested SIB has not been read or if there
  is no cell stored at the requested index.

SIDE EFFECTS

  None.

===========================================================================*/
void * tdsrrc_sibdb_return_sib_for_cell
(
  tdsrrc_SIB_type  req_sib,          /* The SIB that is required */
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell       /* The cell id for the cell */
);

#ifdef FEATURE_TDSCDMA_SIB19
/*===========================================================================

FUNCTION tdsrrcsibdb_free_pseudo_sib19

DESCRIPTION

  This function frees pseudo SIB19 pointer
  
DEPENDENCIES

RETURN VALUE
  none.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_free_pseudo_sib19(void);
#endif /* FEATURE_TDSCDMA_SIB19 */


/*===========================================================================

FUNCTION RRC_SIBDB_RETURN_SIB_FOR_SRV_CELL

DESCRIPTION

  This function returns a pointer to the requested SIB for
  the current serving cell. If there is no serving cell
  selected, or if this particular SIB has not been read 
  completely for the serving cell, this function returns
  NULL.
  
  The pointer is typecast to void since different sibs have
  different type definitions. The calling function is responsible
  for typecasting the pointer to the required SIB type.
  
  Caution: The calling function should read all the required
  data from the pointer as soon as the function returns since
  the SIB data could be overwritten if the serving cell changes
  or if a SIB change indication has been received.

DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  Pointer to the requested SIB. 
  "NULL" if the requested SIB has not been read or if there
  is no serving cell selected.

SIDE EFFECTS

  None.

===========================================================================*/
void * tdsrrcsibdb_return_sib_for_srv_cell
(
  tdsrrc_SIB_type  req_sib  /* The SIB that is required */
);
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
/*===========================================================================

FUNCTION tdsrrcsibdb_is_sib11bis_present_in_srv_cell

DESCRIPTION

  This function returns TRUE is SIB11bis is present in serving cell.
  other wise it returns FALSE
DEPENDENCIES
none
RETURN VALUE

TRUE is SIB11bis is present in serving cell.
FALSE: other wise 

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcsibdb_is_sib11bis_present_in_srv_cell
(
  void
);
#endif

/*===========================================================================

FUNCTION RRCSIB_REGISTER_FOR_SIB_CHANGE_NOTIFICATION

DESCRIPTION

  This function registers SIB change notification requests from 
  other procedures. When a procedure needs to be notified when a 
  particular SIB in the serving cell changes, that procedure must register
  using this function. 

  Only when the SIBs change in the serving cell the procedures
  will be notified. When the serving cell itself changes, the procedures
  are not notified - they must use a NEW_CELL_INDICATION notice for
  that.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_register_for_sib_change_notification
(
  tdsrrc_proc_e_type  procedure,   /* Procedure that's doing this registration*/
  tdsrrc_SIB_type sib              /* Sib for which notification is needed    */
);


/*===========================================================================

FUNCTION RRCSIB_DEREGISTER_SIB_CHANGE_NOTIFICATION

DESCRIPTION

  This function de-registers previously registered SIB change 
  notification requests. When a procedure no longer needs to be 
  notified of sib changes, it may de-register using this
  function.
   
  This is not mandatory but if the procedure does 
  not de-register, it will continue to receive sib change
  notifications.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsib_deregister_sib_change_notification
(
  tdsrrc_proc_e_type  procedure,   /* Procedure that's de-registering         */
  tdsrrc_SIB_type sib              /* Sib for which notification is not needed*/
);

/*===========================================================================

FUNCTION RRC_GET_SERVING_CELL_ID

DESCRIPTION

  This function gets the current serving cell's id in the
  format of an unsigned char array of 2 elements with the
  LSB 8 bits of the cell-id in the first element and the 
  2nd LSB 8 bits of the cell-id in the second element.
  Only the first 16 bits of the cell-id are used - the rest
  if any, are ignored.

  It will also fill in the length in bits of the cell id.

  At present this function is meant to be used only by
  GSTK module.

DEPENDENCIES

  The calling function is responsible for allocating 
  the appropriate amount of space to the 2 pointers that are
  input to this function.
  
RETURN VALUE
  
  TRUE if cell-id has been copied, FALSE if it's not.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrc_get_serving_cell_id
( 
  uint8* cell_id, /* Cell Id to be copied */
  uint8* length   /* Length of the Cell Id to be copied */
);


/*===========================================================================

FUNCTION RRC_GET_SERVING_CELL_MIB_VALUE_TAG

DESCRIPTION

  This function gets the current serving cell's 
  MIB Value tag to the calling function 

DEPENDENCIES
  None
  
RETURN VALUE
  
  Mib Value tag of serving cell index.

SIDE EFFECTS

  None.

===========================================================================*/
  uint32 tdsrrcsibdb_get_serving_cell_mib_value_tag(void);


/*===========================================================================

FUNCTION RRC_SIBDB_FIND_CELL_INDEX_FOR_PLMN_FREQ_PSC

DESCRIPTION

  This function tries to find the index in the sib
  database for the given frequency and primary scrambling
  code combination.
  
  If a match is found, the correct index is returned.
  
DEPENDENCIES

  SIB Database should be correctly initialized. The PLMN
  and Cell Ids given by the function should be in the
  database.

RETURN VALUE

  Index of the cell. If this cell is not present
  in the SIB database, TDSSIBDB_INVALID_INDEX is returned.

SIDE EFFECTS

  None.

===========================================================================*/
uint32 tdsrrcsibdb_find_cell_index_for_plmn_freq_cpid
(
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  uint32 freq,    /* Frequency of the cell */
  uint8 cpid,  /* cell param id       */
  tdssib_events_index_e_type  event_index /*Current sib event index*/
);

/*===========================================================================

FUNCTION TDSRRCSIBDB_FIND_CELL_INDEX_FOR_FREQ_CPID

DESCRIPTION

  This function tries to find the index in the sib
  database for the given cell info and frequency.
    
  If a match is found,
  compares the serving LAC/RAC with the SIBdb cell's.
     if LAC/RAC mismatch, high cost indicator will be high-cost cell.
     otherwise, it will be normal cell.

  If not found,
  the high cost indicator will be normal cell.
  
DEPENDENCIES


RETURN VALUE

 None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_find_cell_index_for_freq_cpid
(
  tdsl1_cell_info_struct_type  *cell_info,    /* The cell info */ 
  uint32 freq    /* Frequency of the cell */
);
/*===========================================================================

FUNCTION TDSRRCSIBDB_COMPARE_LAC_RAC_WITH_SERVING_CELL

DESCRIPTION

  This function compares the serving LAC/RAC with the SIBdb cell's.
  If LAC/RAC mismatch, return TRUE.
  If match, return FALSE.
  
DEPENDENCIES



RETURN VALUE

  If LAC/RAC mismatch, return TRUE.
  If match, return FALSE.
  
SIDE EFFECTS

  None.

===========================================================================*/

boolean tdsrrcsibdb_compare_lac_rac_with_serving_cell
(
  tdsrrc_SysInfoType1 *SIBdb_sib1_ptr
);

/*===========================================================================

FUNCTION tdsrrcsibdb_get_sib_type_from_sib_type_and_tag

DESCRIPTION

  Helper function to get sib_type from sibsb type received in
  MIB
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_get_sib_type_from_sib_type_and_tag
(
  tdsrrc_SIB_TypeAndTag * sib_type_and_tag,
  tdsrrcsib_sib_type_vtag * sib_type_tag
);
/*===========================================================================

FUNCTION tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag

DESCRIPTION

  Helper function to get sib_type from sibsb type received in
  SB1 and SB2 functions
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag
(
  tdsrrc_SIBSb_TypeAndTag * sib_sb_type_tag,
  tdsrrcsib_sib_type_vtag * sib_type_tag
);
/*===========================================================================

FUNCTION RRC_SIBDB_ADJUST_AGE_INDEX

DESCRIPTION

  This function adjusts the "age_index" for the cells in the SIB
  database. The age_index is a number that indicates how long a cell
  has been stored in the database. The newest cell has the age
  index of 0, and each previous cell has an index one higher than
  the newest cell.
  
  If the database is full, the oldest cell has the age index of 
  TDSSIBDB_MAX_INDEX.  
    
  The index of the newest cell is passed to this function as soon as
  it is created. NOTE: Only tdsrrcsibdb_create_cell_index can call this
  function.
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_adjust_age_index
(
  uint32 new_cell_index  /* Index of the new cell that has just been created */
);

/*===========================================================================

FUNCTION tdsrrcsibdb_invalidate_mib_sb_value_tags_for_interrupted_sib_update

DESCRIPTION

  This function invalidates the MIB and SB's value-tags in case SIB_UPDATE_IND
  is interrupted by a higher priority n_bcch event.  Invalidation of the MIB
  and SB value tags, cause the SIB's in the reference-list and SIB's in SB's to
  be compared for value-tags afresh.  This is needed bcos, it could so happen that
  in the earlier run which was interrupted, SB and MIB were read but all the SIB's
  that were changed are not completely read.
  
DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_invalidate_mib_sb_value_tags_for_interrupted_sib_update
(
  uint32 dbindex     /* SIB Database Index for the cell for which the
                        new Value Tags have been received */
);

/*===========================================================================

FUNCTION tdsrrcsibdb_clear_active_cell_status

DESCRIPTION

  This function reset's the sib-db active cell flags.
  This is done whenever we go OOS or whenever we go from WtoG
  
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_clear_active_cell_status
( 
  void 
);


/*===========================================================================

FUNCTION tdsrrcsibdb_update_six_hr_timer

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

void  tdsrrcsibdb_update_six_hr_timer
(
void
);

/*===========================================================================

FUNCTION rrcsib_clear_a_cell_index

DESCRIPTION

  This function will clear all the SIB's/MIB/SB's for the given cell inedx
  for the SIB db.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsibdb_clear_a_cell_index(
uint32 dbindex
);

/*===========================================================================

FUNCTION tdsrrcsibdb_find_valid_cell_index

DESCRIPTION

  This function tries to find the index in the sib
  database for the given cell id and PLMN id combination.
  If a match is found, the correct index is returned.
  
DEPENDENCIES

  SIB Database should be correctly initialized. The PLMN
  and Cell Ids given by the function should be in the
  database.

RETURN VALUE

  Index of the cell. If this cell is not present
  in the SIB database, TDSSIBDB_INVALID_INDEX is returned.

SIDE EFFECTS

  None.

===========================================================================*/
uint32 tdsrrcsibdb_find_valid_cell_index
(
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell,       /* The cell id for the cell */
  tdssib_events_index_e_type event_index /*Current avtice sib event*/
);
/*===========================================================================

FUNCTION rrcsib_encode_sib_change_mask

DESCRIPTION

  This function prepares sib change indication bit mask.
  which will be sent to all the procedures which have registered 
  with sib for a notification
  
DEPENDENCIES
  None.

RETURN VALUE
none

SIDE EFFECTS
  None.
     
===========================================================================*/
uint8 tdsrrcsib_check_and_prepare_sib_change_mask
(
tdsrrc_sib_change_mask_type *sib_change_mask,
tdsrrc_proc_e_type procedure,
uint8 priority
);

/*===========================================================================

FUNCTION tdsrrcsibdb_is_it_registered_procedure_for_sibchange

DESCRIPTION

  This function will loop through regestered procedure 
  data base for sib change to look whether the passed 
  procedure has regestered for sib change
  
DEPENDENCIES
  None.

RETURN VALUE
TRUE : If Sibs have changed for this registered procedure
FALSE: Otherwise

SIDE EFFECTS
  None.
     
===========================================================================*/
uint8 tdsrrcsibdb_is_it_registered_procedure_for_sibchange
(
tdsrrc_proc_e_type procedure
);
/*===========================================================================

FUNCTION tdsrrcsibdb_set_priority

DESCRIPTION

  This function will set a priority for reveived sib type
  for registration which will be used for posting sib change ind
  to procedures when ever these sibs are changed.
DEPENDENCIES
  None.

RETURN VALUE
none
SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsibdb_set_priority
(
  uint32 index,
  tdsrrc_SIB_type sib
);
#ifdef FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING
/*===========================================================================

FUNCTION tdsrrcsibdb_get_vtag_of_a_sib_for_serving_cell

DESCRIPTION

  This function will return the value tag for the SIB type
  passed as input to this function for serving cell. 
  If serving cell sib db index is valid then the corresponding 
  value tag is returned else an invalid value tag is returned
  
DEPENDENCIES
  None.

RETURN VALUE
  If serving cell sib db index is valid then the corresponding 
  value tag is returned 
  else an invalid value tag is returned

SIDE EFFECTS
  None.
     
===========================================================================*/
uint8 tdsrrcsibdb_get_vtag_of_a_sib_for_serving_cell
(
tdsrrc_SIB_type sibtype
);
#endif /*FEATURE_TDSCDMA_INTER_FREQ_RACH_REPORTING*/
/*===========================================================================

FUNCTION tdsrrcsibdb_is_serving_cell_sib_vaild

DESCRIPTION

  This function will return the status of serving cell SIB information. 
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE:If SIB db index for serving cell is valid
  FALSE:otherwise
SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsibdb_is_serving_cell_sib_vaild
(
  void
);

/*===========================================================================

FUNCTION RRC_SIBDB_GET_SIB_PTR_FOR_CELL

DESCRIPTION

  This function returns a pointer to the requested SIB 
  for the requested cell. The pointer is typecast
  to void since different sibs have different type
  definitions. The calling function is reponsible for
  typecasting the returned pointer to the right type.
  
  The calling function MUST verify that the SIB database
  index passed to this function is a valid index
  created by this module.
  
DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  A void pointer that points to the requested SIB in the
  given SIB database index.
  
SIDE EFFECTS

  None.

===========================================================================*/
void * tdsrrcsibdb_get_sib_ptr_for_cell
(
  uint32 index,    /* Index for the requested cell */
  tdsrrc_SIB_type sib /* The SIB that's requested */
);

/*===========================================================================

FUNCTION tdsrrcsibdb_decode_all_sibs_for_a_cell

DESCRIPTION

  This function ASN.1 decode all sibs for a cell. 
  It uses ASN.1 encoded messages which are stored in sibdb 
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_decode_all_sibs_for_a_cell
(
  uint32 index /* Index of the cell header to be initialized */
);
/*===========================================================================

FUNCTION rrc_sibdb_free_all_encodedsibs_for_a_cell

DESCRIPTION

  This function will free all ASN.1 decode sibs for a cell. 
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_free_all_decodedsibs_for_a_cell
(
  uint32 index /* Index of the cell header to be initialized */
);

/*===========================================================================

FUNCTION RRCSIB_IS_SIB_SCHEDULED

DESCRIPTION

  This function checks the scheduling information present
  in the current cell to determine if the given SIB is
  scheduled by the MIB or one of the scheduling blocks.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  TRUE is the SIB is scheduled, FALSE if not scheduled.

SIDE EFFECTS

  None.
   
===========================================================================*/
boolean tdsrrcsib_is_sib_scheduled
(
  tdsrrc_SIB_type sib, /* SIB to be checked */
  uint32 dbindex    /* Index to the database */
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  , boolean for_deferred_sib  /* Whether this is called for checking 
                                 the scheduling info for Deferred SIBs*/
  , tdsrrc_SIB_type *def_sib_stored_in_sb1_sb2 /* Scheduling block where the scheduling 
                                   info for deferred SIBs is present */
#endif
);

/*===========================================================================
FUNCTION tdsrrcsibdb_print_cellid_psc_plmnid_from_sibdb

DESCRIPTION

  This function will prints cell id, plmn id & CPID from sib db
  
DEPENDENCIES
  None.

RETURN VALUE
  none

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsibdb_print_cellid_psc_plmnid_from_sibdb
(
  void
);
/*===========================================================================

FUNCTION tdsrrc_sibdb_clear_cell_header

DESCRIPTION

  Clear all cell header in tdssibdb
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  This will make SIB to reread all sib's.

===========================================================================*/
void tdsrrc_sibdb_clear_cell_header
(
  void
);

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/*===========================================================================
FUNCTION tdsrrcsibdb_return_intra_freq_meas_quan_for_deferred_sib

DESCRIPTION

  This function returns the Intra Freq Meas Quantity to be used during measurement reporting in RACH
  for Deferred SIB 11/11bis/12reading. This IE is fetched from SIB3.

DEPENDENCIES
none

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void tdsrrcsibdb_return_intra_freq_meas_quan_for_deferred_sib
(
  tdsl1_intra_freq_meas_quan_struct_type *intra_freq_meas_quan
);
#endif

#ifdef FEATURE_TDSCDMA_SPECIFIC_SIB_READING	
/*===========================================================================

FUNCTION tdsrrcsibdb_find_cell_index_for_freq_cpid_for_special_reading

DESCRIPTION

  This function tries to find the index in the sib
  database for the given CPID and frequency.
 
DEPENDENCIES


RETURN VALUE

 SIB_DB_IDX.

SIDE EFFECTS

  None.

===========================================================================*/

uint32 tdsrrcsibdb_find_cell_index_for_freq_cpid_for_special_reading
(
	uint32       freq,    /* Frequency of the cell */
	uint8        cpi    /* The CPID */ 
);
#endif

#ifdef FEATURE_TDSCDMA_SIB19
/*===========================================================================

FUNCTION tdsrrcsibdb_get_serv_ota_sib19

DESCRIPTION

  This function returns OTA SIB19 pointer (vs pseudo SIB19).
 
DEPENDENCIES


RETURN VALUE

 tdsrrc_SysInfoType19 *

SIDE EFFECTS

  None.

===========================================================================*/

tdsrrc_SysInfoType19 * tdsrrcsibdb_get_serv_ota_sib19(void);
#endif /* FEATURE_TDSCDMA_SIB19 */

/*===========================================================================

FUNCTION tdsrrcsibdb_invalid_sibdb_for_quick_cell_resel

DESCRIPTION

  This function is to invalid the sibdb for quick cell reselection if TDS stop happens or LTA is longer thant 5 min.
  
DEPENDENCIES

  None

RETURN VALUE

  None

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_invalid_sibdb_for_quick_cell_resel
(
 void
);

#endif /* TDSRRCSIBDB_H */
