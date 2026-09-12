#ifndef TDSRRCSIBDBI_H
#define TDSRRCSIBDBI_H
/*===========================================================================
       R R C   S I B   D A T A B A S E   I N T E R N A L   H E A D E R 

DESCRIPTION

  This module contains the internal definitions for the SIB database
  modules. This module is visible only to the SIB processing modules.
  It is not to be used by any other RRC module.
  
Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsibdbi.h#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

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
#include "tdsrrcsibdb.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Maximum number of cells for which SIBs are maintained at a given
time. This number should be tweaked as needed to obtain a balance
between memory usage and performance improvement. */

#define TDSSIBDB_MAX_CELLS 10


/* Largest index for which SIBs will be stored in the SIB database. */
#define TDSSIBDB_MAX_INDEX (TDSSIBDB_MAX_CELLS - 1)

/* Invalid index - used to indicate an incorrect index to the
database. Must be higher than the maximum valid index. 1 is chosen
arbitrarily. */
#define TDSSIBDB_INVALID_INDEX (TDSSIBDB_MAX_INDEX + 1)

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* Structure of all the SIB pointers. These pointers will be passed
to the ASN.1 function which writes the decoded SIBs to these pointers.
The memory for those SIBs is also allocated by the ASN.1 pointer */

typedef struct
{
  tdsrrc_MasterInformationBlock *mib;
  tdsrrc_SysInfoType1           *sib1;
  tdsrrc_SysInfoType2           *sib2;
  tdsrrc_SysInfoType3           *sib3;
  tdsrrc_SysInfoType4           *sib4;
  tdsrrc_SysInfoType5           *sib5;
  tdsrrc_SysInfoType6           *sib6;
  tdsrrc_SysInfoType7           *sib7;
  tdsrrc_SysInfoType8           *sib8;
  tdsrrc_SysInfoType9           *sib9;
  tdsrrc_SysInfoType10          *sib10;
  tdsrrc_SysInfoType11          *sib11;
  tdsrrc_SysInfoType12          *sib12;
  tdsrrc_SysInfoType18          *sib18;
  tdsrrc_SysInfoTypeSB1         *sb1;
  tdsrrc_SysInfoTypeSB2         *sb2;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  tdsrrc_SysInfoType11bis       *sib11bis;
#endif
#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrc_SysInfoType19          *sib19;
#endif

/* To hold the encoded SIB.Maximum possible length of a encoded
     SIB is TDSMAX_SIB_LENGTH_BYTES bytes. */
  byte mib_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 mib_encoded_pdu_length;
  byte sib1_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib1_encoded_pdu_length;
  byte sib2_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib2_encoded_pdu_length;
  byte sib3_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib3_encoded_pdu_length;
  byte sib4_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib4_encoded_pdu_length;
  byte sib5_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib5_encoded_pdu_length;
  byte sib6_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib6_encoded_pdu_length;
  byte sib7_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib7_encoded_pdu_length;
  byte sib11_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib11_encoded_pdu_length;
  byte sib12_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib12_encoded_pdu_length;
  byte sib18_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib18_encoded_pdu_length;
  byte sb1_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sb1_encoded_pdu_length;
  byte sb2_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sb2_encoded_pdu_length;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  byte sib11bis_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib11bis_encoded_pdu_length;
#endif
#ifdef FEATURE_TDSCDMA_SIB19
  byte sib19_encoded_ptr[TDSMAX_SIB_LENGTH_BYTES];
  uint32 sib19_encoded_pdu_length;
#endif

}tdsrrc_sib_ptr_type;


/* Structure of Database Cell Header. The cell header contains 
all the SIB pointers for a particular cell and other relevant
information about the cell */
typedef struct
{
  boolean index_used;              /* Indicates if this index is used
                                   or not */
  rrc_plmn_identity_type plmn_id;  /* PLMN Id of this cell - internal format */
  tdsrrc_int_cell_id_type cell_id;    /* Cell Id of this cell - internal format */
  uint8 cpid;                 /* Scrambling code of this cell */
  uint32 freq;                     /* Frequency of this cell */
  uint32 age_index;                /* A number showing how long this
                                   cell has been stored in the database.
                                   This varies from 0 to TDSSIBDB_MAX_INDEX.
                                   This is used to remove the oldest cell
                                   when the database is full. */
  boolean sib_valid[TDSRRC_TOT_SIBS]; /* Indicates validity of each SIB */
  uint32 sib_value_tag[TDSRRC_TOT_SIBS]; /* Holds the value tag for each
                                      SIB. Note that uint32 is used to
                                      store all value tag types to 
                                      improve speed efficiency in the ARM.
                                      The following are the ranges of the
                                      different value tags -
                                        
                                        Cell Value Tag - 1 to 4
                                        PLMN Value Tag - 1 to 256
                                        MIB Value Tag -  1 to 8 */
                                        
  tdsrrc_sib_ptr_type sibs;           /* Points to all SIBs for this cell */

  uint8  six_hr_count ;
  boolean sib_db_valid_for_quick_cell_resel;    /* If sibdb of this cell is valid for quick cell reselection */
}tdsrrc_sibdb_cell_header_type;


/* Actual SIB database structure */
typedef struct
{
  boolean database_empty;          /* Indicates that the database is
                                   currently empty. Set to false when
                                   there's atleast one cell entry in
                                   the database. */
  boolean active_cell_sibs_valid;  /* Indicates that the current active
                                   cell has valid SIBs. If atleast one
                                   SIB is valid for the active cell, this
                                   is set to TRUE. */
  uint32 active_cell_index;        /* Indicates the index of the current
                                   active cell in the database */

  uint32 num_stored_cells;         /* Keeps track of total number of stored
                                    * cells in the DB at any time. */

  boolean active_cell_sib_changed[TDSRRC_TOT_SIBS]; /* Used to indicate if
                                   any of the active cell SIBs have 
                                   changed */
  boolean last_plmn_and_cell_id_matched; /* Indicates if the last (latest)
                                   index selected for storing cells already
                                   has stored SIBs for a matching PLMN and Cell
                                   id. This indicates to the sib event controller
                                   whether value-tag matching is needed or not */
  tdsrrc_sibdb_cell_header_type cell[TDSSIBDB_MAX_CELLS];
                                   /* Cell Headers - for TDSSIBDB_MAX_CELLS
                                   number of cells. */
}tdsrrc_sibdb_type;   



/* Externalize the sib database declaration. This is shared only by the
other SIB related procedures. */
extern tdsrrc_sibdb_type tdssibdb;

/* Enumerated type to indicate the results of a value tag matching
process */
typedef enum
{
  TDSVALUE_TAG_MATCHED = 1,           /* The new value tag matched the stored value tag      */
  TDSVALUE_TAG_DID_NOT_MATCH,         /* The new value tag didn't match the stored value tag */
  TDSNO_SIB_FOR_THIS_VALUE_TAG,       /* There was no sib in the database for this value tag */
  TDSTIMER_BASED_SIB_RCVD,            /* This is a timer based SIB, SIB not already recvd    */
  TDSVALUE_TAG_NOT_PRESENT            /* the new value tag was not present in the ref list   */
  #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  , TDSDEFERRED_SIB_RCVD               /* The SIB was deferred. Need to read it again. */
  #endif
}tdsrrc_sibdb_value_tag_result_e_type;

/* Structure containing a list of value tag results for each SIB, to be
filled in by the procedure doing value tag comparisons */
typedef struct
{
  tdsrrc_sibdb_value_tag_result_e_type result[TDSRRC_TOT_SIBS]; /* Result for each SIB */
}tdsrrc_sibdb_value_tag_results_type;

/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */



/* =======================================================================
**                          Macro Definitions
** ======================================================================= */


/*========================================================================
MACRO SIBDB_VALID_INDEX

ARGS
  index - a uint32 value.

DESCRIPTION:
  This macro returns TRUE if the index is a valid SIB database index.
  Else it returns FALSE.
========================================================================== */
#define TDSSIBDB_VALID_INDEX( index ) \
   (((index) < TDSSIBDB_INVALID_INDEX ) ? (TRUE) : (FALSE)) 


/*========================================================================
MACRO SIBDB_VALID_SIB

ARGS
  sib - a uint32 value indicating the sib number.

DESCRIPTION:
  This macro returns TRUE if the number is a valid sib.
  Else it returns FALSE.
========================================================================== */
#define TDSSIBDB_VALID_SIB_RANGE( sib ) \
   (((sib) < tdsrrc_ExtensionType ) ? (TRUE) : (FALSE)) 


/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION RRC_SIBDB_INIT

DESCRIPTION

  This function initializes the SIB database. This is called
  once during power-up. Note that individual cell headers are
  not initialized at this time. They are initialized each time
  a new cell is selected and SIBs need to be stored in the cell
  headers.
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_init( void );




/*===========================================================================

FUNCTION RRC_SIBDB_CREATE_CELL_INDEX

DESCRIPTION

  This function creates a new cell index for the given
  PLMN and CELL id if needed. The function first checks
  if the index for this PLMN and Cell id combination is
  already present in the database. If so, it adjusts the
  "age_index" of the cells accordingly and returns the
  existing index. If a match is not found a new cell index
  is created and returned to the calling function.
  If the database is full, the oldest cell is removed and
  that index is used for the new cell.
    
DEPENDENCIES

  SIB Database should be correctly initialized. The PLMN
  and Cell Ids given by the function should be in the
  database.

RETURN VALUE

  The newly created index for the given cell. This ranges
  from 0 to TDSSIBDB_MAX_INDEX.

SIDE EFFECTS

  None.

===========================================================================*/
uint32 tdsrrcsibdb_create_cell_index
(
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell,       /* The cell id for the cell */
  tdssib_events_index_e_type event_index /*Current avtice sib event*/
);


/*===========================================================================

FUNCTION RRC_SIBDB_COMPARE_VALUE_TAGS_FOR_CELL

DESCRIPTION

  This function compares the stored value tags for a particular
  cell with the newly received value tags in the MIB or the
  Scheduling blocks.
  
  If the new value tags don't match the stored value tags, 
  this function frees the space occupied by the SIBs so that
  the new SIBs can be stored in their place.

  NOTE: THE FOLLOWING SIBS DO NOT HAVE VALUE-TAGS
  
    SIB7, SIB9, SIB10, SIB14, SIB17
    
  These SIBs only have expiration timers.

DEPENDENCIES

  SIB Database should be correctly initialized.

  The result_ptr should be initialized by the calling function.
  The results of the value tag comparisons will be stored in
  the result_ptr.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_compare_value_tags_for_cell
(
  uint32 dbindex,     /* SIB Database Index for the cell for which the
                      new Value Tags have been received */
  tdsrrc_SIB_type sib,   /* Indicate which SIB the value-tags are from
                      This can be the MIB or SB1 or SB2. */
  void *block_ptr,    /* Pointer to the MIB, SB1 or SB2, typecast
                      to void */
  tdsrrc_sibdb_value_tag_results_type *result_ptr /* Pointer to store
                      the results of value tag comparisons */
/*Need a flag to indicate forcibly re-read all sibs*/
,
tdsrrcsib_vtag_match_type  vtag_match_type
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  , tdssib_events_index_e_type event_index
#endif

);

/*===========================================================================

FUNCTION RRC_SIBDB_STORE_VALUE_TAGS_FOR_NEW_CELL

DESCRIPTION

  This function stores value tags for a new cell for which
  a MIB or a Scheduling Block was received. When a cell is 
  acquired, and the PLMN and Cell id are known, the sib procedure
  will try to find a match in the sib database. If a match is
  found rrc_sibdb_process_value_tags_for_cell is called to
  compare value_tags. If a match is not found, this function
  should be called so that the value_tags can be copied into
  the database.
    
  NOTE: THE FOLLOWING SIBS DO NOT HAVE VALUE-TAGS
  
    SIB7, SIB9, SIB10, SIB14, SIB17
    
  These SIBs only have expiration timers.

DEPENDENCIES

  SIB Database should be correctly initialized.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_store_value_tags_for_new_cell
(
  uint32 dbindex      /* SIB Database Index for the cell for which the
                      new Value Tags have been received */
  #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  , tdssib_events_index_e_type index
  #endif
);    






/*===========================================================================

FUNCTION RRC_SIBDB_NOTIFY_SIB_CHANGE_TO_PROCS

DESCRIPTION

  This function looks through the stored sib change registrations
  to see if the sib changes should be notified to any of
  the registered procedures. This is called by the sib procedure
  when the TDSRRCSIB_UPDATE_SIBS event is active and all changed
  sibs haave been decoded.

  If it finds procedures that need to be notified, it sends
  an internal command TDSRRC_SIB_CHANGE_IND to the internal command
  queue.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_notify_sib_change_to_procs( void );



/*===========================================================================

FUNCTION RRC_SIBDB_FIND_CELL_INDEX

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
uint32 tdsrrcsibdb_find_cell_index
(
  rrc_plmn_identity_type plmn,    /* The PLMN Id for the cell */ 
  tdsrrc_int_cell_id_type cell       /* The cell id for the cell */
);


/*===========================================================================

FUNCTION RRC_SIBDB_CHECK_IF_SB_PRESENT

DESCRIPTION

  This function checks if a Scheduling Block (1 or 2) is
  present in a particular cell. This is done by looking 
  through the MIB's scheduling information.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  TRUE if the SB is present. FALSE otherwise.

SIDE EFFECTS

  None.

===========================================================================*/
boolean tdsrrcsibdb_check_if_sb_present
(
  tdsrrc_SIB_type sib, /* Either SB1 or SB2 */ 
  uint32 dbindex    /* Database index    */
);


/*===========================================================================

FUNCTION RRC_SIBDB_CLEAR_ACTIVE_CELL_SIB_CHANGE

DESCRIPTION

  This function clears the variable that saves
  the active cell SIB change indications.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  TRUE if the SB is present. FALSE otherwise.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_clear_active_cell_sib_change( void );



/*===========================================================================

FUNCTION RRC_SIBDB_SAVE_ACTIVE_CELL_SIB_CHANGE

DESCRIPTION

  If a SIB in the active cell is changed by the UTRAN this function
  is called to save the information about the SIB that has changed.
  Later when all the changed SIBs have been read, the change information
  is sent to other RRC procedures if needed.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_save_active_cell_sib_change
( 
  tdsrrc_SIB_type sib   /* The sib that has changed.*/
);


/*===========================================================================

FUNCTION RRC_SIBDB_FREE_SIB

DESCRIPTION

  This function frees the memory used by a System Information
  Block stored in the SIB database using the ASN.1 free pdu function.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_free_sib
(
  int pdu_num,    /* PDU number defined by ASN.1 for this SIB */
  void *sib_ptr   /* Pointer to the SIB typecast to void.     */
);



/*===========================================================================

FUNCTION RRC_SIBDB_CELL_HEADER_INIT

DESCRIPTION

  This function initializes a cell header within the SIB database.
  This function is called before storing SIBs for a new cell in the
  SIB database.
  
DEPENDENCIES

 None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrcsibdb_cell_header_init
(
  uint32 index /* Index of the cell header to be initialized */
);

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/*===========================================================================

FUNCTION tdsrrcsibdb_copy_deferred_sibdb_to_tsibdb

DESCRIPTION

  This function copies SIB segments from Deferred SIB optimization DB to TSIB.

DEPENDENCIES
none

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
void  tdsrrcsibdb_copy_deferred_sibdb_to_tsibdb
(
  uint32 dbindex,     /* SIB Database Index for the cell for which the
                      new Value Tags have been received */
  tdssib_events_index_e_type event_index,
  void *block_ptr,    /* Pointer to the MIB, SB1 or SB2, typecast
                      to void */
  tdsrrc_SIB_type sib   /* Indicate which SIB the value-tags are from
                      This can be the MIB or SB1 or SB2. */
);
#endif
#endif /* TDSRRCSIBDBI_H */

