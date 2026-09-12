#ifndef TDSRRCSIBPROCI_H
#define TDSRRCSIBPROCI_H
/*===========================================================================
                        R R C   S I B   P R O C E S S I N G  

DESCRIPTION 

  This is the internal header file for the SIB processing module.
  This is shared only between the SIB processing modules and is
  not visible to other RRC modules.
  
  This file contains the definition of the SIB events database that
  is used to hold information on SIB events that are currently active.

Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated.  
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/inc/tdsrrcsibproci.h#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/15/12   gv      Added support to log new event when complete SIB is received
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/18/11   mkh     Changes to stub out tdsrrcsib_check_sib_schedule_for_sibs for
                   RRC QTF

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
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcintif.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* Maximum allowed length for an encoded SIB.
This is calculated like this - Total of 16 segments
are allowed for a SIB. Maximum data bits per segment
is 222. So the total - 16*222/8 = 444 bytes */
#define TDSMAX_SIB_LENGTH_BYTES 444

#define TDSINVALID_PDU_LENGTH 0

/* The same length in bits */
#define TDSMAX_SIB_LENGTH_BITS (TDSMAX_SIB_LENGTH_BYTES*8)

/* Maximum number of BCCHs allowed to be setup for
SIB events */
#define TDSMAX_SIB_BCCH 2

/* Length in bits of a "SIB-Data-Fixed" as defined
by the standard. */
#define TDSFIXED_SIB_DATA_LENGTH_BITS 222

/* Length in bytes of a "SIB-Data-Fixed" (222/8) rounded up. */
#define TDSFIXED_SIB_DATA_LENGTH_BYTES 28

/* Length in bits of a "SIB-Data-Fixed" for a complete
SIB as defined by the standard. */
#define TDSFIXED_COMPLETE_SIB_DATA_LENGTH_BITS 226

/* Length in bytes of a "SIB-Data-Fixed" for a complete
sib (226/8) rounded up. */
#define TDSFIXED_COMPLETE_SIB_DATA_LENGTH_BYTES 29

/* Maximum number of segments allowed for a segmented SIB */
#define TDSMAX_SEGMENTS_FOR_A_SIB 16 

/* Maximum Segment count allowed for a segmented SIB. The
segment count starts from 0 for the first segment. */
#define TDSMAX_SEG_COUNT_FOR_A_SIB (TDSMAX_SEGMENTS_FOR_A_SIB - 1)

/* Maximum Segment Index allowed for a segmented SIB. This
is defined by the spec as 15 since the segment index for
the first segment is 0 and the first subsequent segment is
1. If a segmented SIB has only 2 segments, the last segment
has segment index 1. */
#define TDSMAX_SEG_INDEX_FOR_A_SIB (TDSMAX_SEGMENTS_FOR_A_SIB - 1)

/*invalid Value_tag to invalidate value tags for SB's and MIB's*/
#define TDSINVALID_MIB_SB_VALUE_TAG 4096

/*invalid SFN for schedule and maintaining sfn time*/
#define TDSINVALID_SIB_SCHEDULE_REP 4097

#define TDSEVENT_INDEX_VALID(event_index) \
  (  \
    (event_index<TDSMAX_SIB_EVENTS_INDEX)?(TRUE):(FALSE)\
  )

/* Maximum number of sib segments that can be holed in a temp 
 *buffer which is used for TSIB optimization. Currently its set to 55 
 *based on max number of active sibs segments which are in use, 
 *Below is the data 
 *SIB-11/12      - 14 segments at max  = 14*2  =  28
 *SIB-1/2/MIB    - 2 segments at max   = 2* 3  =  06
 *SIB3/4/5/6/18  - 4 segments at max   = 4*5   =  20  
 *SIB-7          - 1 segments at max   = 1*1   =  01  
 *----------------------------------------------------
 *Total Number of SIB segments                 = 55 
 *Note: looking into test results, we are fine tuning this value to 25 for now
 *If needed in future where N/W start using larger SIB segments for all SIB's 
 *then we need to change this value appropriately
 */
#define TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD 25

/*Value for 1st segment index*/
#define TDSFIRST_SIB_SEGMENT_INDEX_VAL 0

/*Value for complete sib segment index*/
#define TDSCOMPLETE_SIB_SEGMENT_INDEX_VAL 16

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
#ifdef FEATURE_TDSCDMA_SIB19
  #define TDSRRC_DEFERRED_SIBS 5
#else
  #define TDSRRC_DEFERRED_SIBS 4
#endif

#define TDSMAX_DEFERRED_SIB_SEGMENTS_TO_HOLD 30
#endif

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* Enumerated type of status of processing a SIB segment. */
typedef enum
{
  TDSSEG_PROC_NONE,        /* No status to report yet       */
  TDSSEG_PROC_SUCCEEDED,   /* Segment succesfully processed */
  TDSSEG_PROC_FAILED,      /* Segment processing failed     */
  TDSSEG_PROC_DUPLICATE,    /* This segment has been already received
                        and processed. */
  TDSSEG_PROC_STORED   /* Segment processing Stored in temp sib optimization db  */
  ,TDSSEG_PROC_COMPLETE   /* Segment processing from stored temp sib optimization db 
                         resulted in a complete SIB building */
  //More may be needed later
}TDSseg_proc_stat_e_type;


/* -----------------------------------------------------------------------
** Global Constant Data Declarations 
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

   typedef enum 
  {
    tdsrrc_MIB = 0,
    tdsrrc_SIB1 = 1,
    tdsrrc_SIB2 = 2,
    tdsrrc_SIB3 = 3,
    tdsrrc_SIB4 = 4,
    tdsrrc_SIB5 = 5,
    tdsrrc_SIB6 = 6,
    tdsrrc_SIB7 = 7,
    tdsrrc_SIB11 = 8,
    tdsrrc_SIB12 = 9,
    tdsrrc_SIB18 = 10,
    tdsrrc_SB1 = 11,
    tdsrrc_SB2 = 12,
    tdsrrc_SIB5bis = 13,
    tdsrrc_SIB11bis = 14,
    tdsrrc_SIB19 = 15,
    tdsrrc_ExtensionType = 16,
    tdsrrc_MAX_SIB = 17
  }tdsrrc_SIB_type;


typedef struct
{
   uint8  num_segments;
   uint32  sfn_offset[TDSMAX_SEGMENTS_FOR_A_SIB];   
   uint16 repitition_rate;                      
}tdssib_scheduling_info;


/* Data structure to hold an encoded SIB */

typedef struct
{
  boolean sib_needed;     /* Indicates whether this SIB is needed or not.
                          If this is set to FALSE, the data below is 
                          irrelavant. */
  boolean sib_received;   /* Indicate whether a SIB has
                          been completely received */
  boolean sib_decoded;    /* Indicate whether a SIB has been ASN.1 decoded
                          already */
  uint32 seg_count;       /* Total segments in this SIB. Maximum is 16 */
  uint32 seg_index;       /* Index of the latest segment received 
                          Index is 0 for the subsequent segment received
                          first. Maximum is 14 */
  uint32 sib_tot_length;  /* Total length of the encoded SIB in bits.
                          Cannot exeed (MAX_SIB_LENGTH*8) */ 
  byte sib_data_ptr[TDSMAX_SIB_LENGTH_BYTES];   /* To hold the encoded SIB.
                                        Maximum possible length of a encoded
                                        SIB is TDSMAX_SIB_LENGTH_BYTES bytes. */
}tdstemp_sib_holder_type;


/* Enumerated type of the number of SIB related events supported
at a time. Right now, only two events are supported at a time -
one serving cell and one neighbor cell. */
typedef enum
{
  TDSSIB_SRV_INDEX = 0, /* SIB event for the serving cell */
  TDSSIB_NGH_INDEX,     /* SIB event for a neighbor cell  */
  //May need to support more events here - TBD
  TDSMAX_SIB_EVENTS_INDEX
}tdssib_events_index_e_type;


/* These are data structures to store information about a
BCCH setup */

typedef enum
{
  TDSSIB_BCCH_NO_ACTION,       /* No action related to BCCH setup/teardown */
  TDSSIB_BCCH_SETUP_PENDING,   /* A BCCH setup has been requested and an Ack
                            has not been received */
  TDSSIB_BCCH_RELEASE_PENDING, /* A BCCH Teardown has been requested and
                            an ack has not been received */
  TDSSIB_BCCH_SETUP_COMPLETE,  /* The BCCH setup ack has been received */
  TDSSIB_BCCH_RELEASE_COMPLETE /* The BCCH release ack has been received */
}tdssib_bcch_action_e_type;

/*These are the valid sib segment types*/
typedef enum
{
  tdsnoSegment_chosen,
  tdsfirstSegment_chosen,
  tdssubsequentSegment_chosen,
  tdslastSegmentShort_chosen,
  tdslastAndFirst_chosen,
  tdslastAndComplete_chosen,
  tdslastAndCompleteAndFirst_chosen,
  tdscompleteSIB_List_chosen,
  tdscompleteAndFirst_chosen,
  tdscompleteSIB_chosen,
  tdslastSegment_chosen,
  tdspayload_spare5_chosen,
  tdspayload_spare4_chosen,
  tdspayload_spare3_chosen,
  tdspayload_spare2_chosen,
  tdspayload_spare1_chosen
}tdssib_segment_e_type;

typedef struct
{
  tdssib_bcch_action_e_type bcch_action; /* Indicates the current action
                       related to BCCH setup/teardown for this event */
  uint8 cpid; /* Scrambling code of the cell for this BCCH  */
  uint32 freq;     /* Frequency of the cell for this BCCH        */
}tdssib_active_bcch_setup_type;
  


/* Structure to temporarily hold the MIB and SIB3 till we
find the database index for saving SIBs from a cell. Only after
we decode MIB and SIB3 we can find the index since it's based
on the combination of the PLMN Id and Cell Id. */ 
typedef struct
{
  boolean mib_decoded;
  tdsrrc_MasterInformationBlock *mib;
  tdsrrc_SFN_Prime mib_sfn_prime;
  boolean sib3_decoded;
  tdsrrc_SysInfoType3 *sib3;
  boolean sb1_decoded;
  boolean sb2_decoded;
  boolean sib1_decoded;
}tdsrrc_mib_sib3_holder_type;

/* Name (Description) of the SIB event - Used to remember the type
of event requested so that the appropriate response can be sent. */
typedef enum
{
  TDSRRCSIB_GET_ALL_SIBS,      /* Get all SIBs for a cell */
  TDSRRCSIB_GET_SPECIFIC_SIBS, /* Get a few specific SIBs for a cell */
  TDSRRCSIB_UPDATE_SIBS        /* Update SIBs that have changed (I.e either the value
                            tag has changed or the sib timer has expired) */
  /* More events may be needed later */
}tdssib_event_name_e_type;
                    
/* Data structure to store information about SIBs that have
expiration timers */
typedef struct
{
  boolean sib_timer_update_req; /* Indicates if a SIB update based
                                on an expiration timer is required.*/
  boolean sib_ext_proc_update_req; /* Indicates if a SIB update has
                                   been requested by an external
                                   RRC procedure */
  tdsrrc_proc_e_type req_proc;     /* The procedure that requested it */
}tdsrrc_sib_exp_timer_sib_info_type;

/* Enumerated type of internal events. Currently internal
events are used for SIBs with expiration timers */
typedef enum
{
  TDSRRCSIB_INT_SIB7_EVENT,         /* SIB7 expiration           */
  TDSRRCSIB_INT_SIB9_EVENT,         /* SIB9 expiration           */
  TDSRRCSIB_INT_SIB10_EVENT,        /* SIB10 expiration          */
  TDSRRCSIB_INT_MAX_EVENT
}tdsrrc_sib_int_event_e_type;

/* Data structure to hold info for internal SIB events -
i.e. SIB events used by the SIB procedure itself */
typedef struct
{
  tdsrrc_sib_int_event_e_type int_event_type; /* Store type
                                           of int event*/
}tdsrrc_sib_int_event_info_type;

/* This is a data structure that stores all necessary information for
a particular event. Note that this is more than 10Kbytes in size. Each SIB
needs 444 bytes and we need to store TDSRRC_TOT_SIBS number of those. */

typedef struct
{ 

  uint8  transaction_id;       /* Used in associating the channel config confirm with
                                a specific channel config request */
  boolean event_active;         /* Indicates whether this event is active
                                or not. If set to FALSE, information below
                                is irrelevant and invalid. */
  tdssib_event_name_e_type event_name;  /* Name of this event - GET_ALL_SIBS,
                                GET_SPECIFIC_SIBS e.t.c. */
  tdsrrc_proc_e_type req_proc;     /* Name of the procedure that requested
                                this event - To be used while sending
                                the CNF command. */
  boolean for_serving_cell;     /* Set to true if a GET_SPECIFIC_SIBS command
                                is for the serving cell. This var is not
                                used when the command is not GET_SPECIFIC_SIBS. */
  boolean sib_db_index_found;   /* Indicates whether an index in the SIB
                                database has been found for this event 
                                or not. */
  boolean mib_pos_validated;    /* Indicates whether the MIBs position has
                                been validated or not for a cell. The MIB
                                must be in a position where (SFN mod 32 == 0)
                                otherwise the cell is barred. */
  uint32 sib_db_index;          /* The index in the SIB database for this
                                event. */
  tdsrrc_mib_sib3_holder_type tempholder; /* Hold the MIB and SIB3 temporarily
                                       till a SIB database index is found
                                       for this cell */
  tdstemp_sib_holder_type tsib[ TDSRRC_TOT_SIBS ];   /* To hold all encoded 
                                               SIBs */
  tdsrrc_sib_int_event_info_type int_event; /* Hold the type of int
                                 event incase this is an int event */

  uint8 sched_present_for_sibs[TDSRRC_TOT_SIBS];  //contains info if SIB/SB scheduled or not

  tdsrrc_sib_scan_e_type sib_scan_type;           //contains info about scan type

}tdssib_active_event_type;


/* This is a central database for all active events. */

typedef struct
{
  /* An array of active SIB events */
  tdssib_active_event_type event[ TDSMAX_SIB_EVENTS_INDEX ];
  /* Serving Cell BCCH info        */
  tdssib_active_bcch_setup_type srv_bcch;
  /* Neighbor Cell BCCH info        */
  tdssib_active_bcch_setup_type ngh_bcch;
  uint32 mib_value_tag_from_command; /* Only used for TDSRRC_UPDATE_SIBS_IND
  command. Holds the value tag for the new MIB as given in the
  BCCH modification info. */
  tdsrrc_proc_e_type update_sibs_req_proc; /* Only used for TDSRRC_UPDATE_SIBS_IND
  command. Holds the requesting procedure name. */
  tdsrrcsib_update_sibs_cause_e_type update_sibs_cause;/* Only used for TDSRRC_UPDATE_SIBS_IND
    command. Holds the cause for TDSRRC_UPDATE_SIBS_IND. */

  tdssib_scheduling_info sib_sched[TDSRRC_TOT_SIBS]; //contains scheduling information

  uint16 scheduling_max_rept_rate;           //max rept rate for any sib 
                                             //during a particular sib event
  boolean scheduling_updated_by_sb1;         //bool to figure out if SB1 has already update sib-schedule
  boolean scheduling_updated_by_sb2;         //bool to figure out if SB2 has already updated sib-schedule

}tdssib_active_event_database_type;

/* Enumerated type indicating the status of the SIB collection function */
typedef enum
{
  TDSNO_SIBS_COMPLETED, /* None of the SIBs were completed this time */
  TDSSIBS_COMPLETED     /* One or more SIBs were completed this time */
}tdsrrc_sib_collect_e_type;              

typedef struct
{
  /* This indicate whether the sib segment index is being used or free,
  TRUE - Indicate its being currently used, 
  FALSE - Indicate its free to be used */
  boolean index_in_use;
  
  /* This indicate SIB type */
  tdsrrc_SIB_type sib_type;
  
  /* Used to hold SFN prime at which this SIB segment was received*/
  uint32 sfn_prime;
  
  /* Used to hold SIB segment index*/
  uint8 seg_index;
  
  /* Used to hold SIB segment data length*/
  uint32 seg_data_length;
  
  /* Used to hold sib segment type*/
  tdssib_segment_e_type sib_seg_type;
  
  /* Used to hold SIB segment data*/
  byte seg_data_ptr[TDSFIXED_SIB_DATA_LENGTH_BYTES];
}tdsrrc_temp_sib_optimization_holder_type;

  /*tdstsib_optimization_db used to hold sib segments received in out of order*/
extern tdsrrc_temp_sib_optimization_holder_type tdstsib_optimization_db[TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD];

  /*Look up table used to indicate whether a sib segment 
    of a sib tpye is presnt in temp sib optimization db or not*/
extern boolean tdstsib_opt_db_look_up_table[tdsrrc_MAX_SIB];

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING

/* Deferred SIB DB used to hold value tag info, segments of the deferred SIBs */
typedef struct
{
   uint32  sib11_val_tag;
   uint32  sib11bis_val_tag;
   uint32  sib12_val_tag;
   uint32  sib18_val_tag;
#ifdef FEATURE_TDSCDMA_SIB19
   uint32  sib19_val_tag;
#endif
}tdsdeferred_sib_value_tag_info;

typedef struct
{
  uint32 seg_index;       /* Index of the latest segment received 
                          Index is 0 for the subsequent segment received
                          first. Maximum is 14 */
  uint32 sib_tot_length;  /* Total length of the encoded SIB in bits.
                          Cannot exeed (MAX_SIB_LENGTH*8) */ 
  byte sib_data_ptr[TDSMAX_SIB_LENGTH_BYTES];   /* To hold the encoded SIB.
                                        Maximum possible length of a encoded
                                        SIB is TDSMAX_SIB_LENGTH_BYTES bytes. */
}tdsdeferred_sib_holder_type;

typedef struct
{
  tdsdeferred_sib_holder_type sib11;
  tdsdeferred_sib_holder_type sib11bis;
  tdsdeferred_sib_holder_type sib12;
  tdsdeferred_sib_holder_type sib18;
#ifdef FEATURE_TDSCDMA_SIB19
  tdsdeferred_sib_holder_type sib19;
#endif
}tdsdeferred_sib_db_type;

typedef struct
{
  tdsrrc_temp_sib_optimization_holder_type deferred_sib_seg_db[TDSMAX_DEFERRED_SIB_SEGMENTS_TO_HOLD];
  tdsdeferred_sib_db_type dsib;
  tdsdeferred_sib_value_tag_info val_tag;
  rrc_plmn_identity_type plmn_id;  /* PLMN Id of this cell - internal format */
  tdsrrc_int_cell_id_type cell_id;    /* Cell Id of this cell - internal format */
  uint8 cpid;                 /* Scrambling code of this cell */
  uint32 freq;                     /* Frequency of this cell */

}tdsrrc_deferred_sib_info_type;

extern tdsrrc_deferred_sib_info_type tdsdeferred_sib_info;
#endif

/* The database is externalized so that the rrcsibcollect
module can use the database. */
extern tdssib_active_event_database_type *tdssib_events_ptr;

/* =======================================================================
**                          Macro Definitions
** ======================================================================= */

/*========================================================================

MACRO  TDSGET_PDU_LENGTH_IN_BYTES

INPUT: PDU length in bits.

  The bits may not be octet alligned, so first check if they
  are octet alligned. If they are, divide the pdu length in bits
  by 8. If they are not octet alligned, divide by 8 and add 1.

==========================================================================*/

#define TDSGET_PDU_LENGTH_IN_BYTES( length ) \
        ( (((length)%8) == 0) ? ( (length)/8 ) : ((length)/8 + 1) )




/* =======================================================================
**                        Function Declarations
** ======================================================================= */
/*===========================================================================

FUNCTION PROCESS_SYS_INFO_MSG

DESCRIPTION
 
  This function processes a received System Information Message. The
  System Information Message must have already been ASN.1 decoded before
  passing it to this function. Both FACH System Information Messages
  and BCH System Information Messages are processed here.
  
  The function first determines what combination of SIBs or SIB segments
  are included in the message. Then based on the combination, each SIB
  or SIB segment in the message is processed by calling the appropriate
  function.
  
  This function returns a status value which indicates whether any SIB
  has been completely received or not. If a SIB has been completely 
  received, the calling function is responsible to ASN.1 decode the SIB
  and save the decoded SIB in the SIB database.

DEPENDENCIES

  None.

RETURN VALUE

  TDSNO_SIBS_COMPLETED if none of the SIBs were completely received in this
  system information message.
  TDSSIBS_COMPLETED if any SIB was completely received in this message. It's
  not necessary that the entire SIB was received in this message. If the
  last semgment of a SIB was received in this message, TDSSIBS_COMPLETED will
  be returned by this function. 

SIDE EFFECTS

===========================================================================*/
tdsrrc_sib_collect_e_type tdsrrcsib_process_sys_info_msg
(
  //NEED TO MODIFY LATER (FACH msg also needed)
  tdsrrc_SystemInformation_BCH *msg,  /* Pointer to Sys Info Message BCH */

  tdssib_events_index_e_type index    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
);
/*===========================================================================

FUNCTION tdsrrcsib_clear_tsib_optimization_db

DESCRIPTION
  This function will clear tsib optimization data base.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrrcsib_clear_tsib_optimization_db
(
  void
);
/*===========================================================================

FUNCTION tdsrrcsib_clear_a_sib_from_tsib_optim_db

DESCRIPTION
  This function will clear any segment(if its stored) of 
  a sib type from tsib optimization data base.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrrcsib_clear_a_sib_from_tsib_optim_db
(
  tdsrrc_SIB_type sib_type
);
/*===========================================================================

FUNCTION tdsrrcsib_find_free_index_in_tsib_optim_db

DESCRIPTION
  This function will return first free index in tsib optimization db.
  or the index which already has this sib segment

DEPENDENCIES
  None.

RETURN VALUE
  Free index into tsib optimization db.
  else TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD i.e.
  non of the index are free.

SIDE EFFECTS
  None.
===========================================================================*/
uint8 tdsrrcsib_find_free_index_in_tsib_optim_db
(
  tdsrrc_SIB_type sib_type,  /*This will indicate type of SIB,e.g MIB,SIB1...etc*/
  uint8 seg_index,  /*SIB segment index*/
  tdssib_segment_e_type sib_seg_type/*Used to indicate type of sib segment*/
);
/*===========================================================================

FUNCTION tdsrrcsib_store_sib_seg_in_tsib_optim_db

DESCRIPTION
  This function will store received sib segment in tsib
  optimization db only if a index in tsib optimization db
  is free.
  
DEPENDENCIES
  None.

RETURN VALUE
  TDSSEG_PROC_STORED = if sib segment is stored in tsib opt db
  TDSSEG_PROC_FAILED  =  if sib segment could not be stored in tsib opt db
  
SIDE EFFECTS
  None.
===========================================================================*/
TDSseg_proc_stat_e_type tdsrrcsib_store_sib_seg_in_tsib_optim_db
(
  tdsrrc_SIB_type sib_type,  /*This will indicate type of SIB,e.g MIB,SIB1...etc*/
  uint32 sfn_prime,  /*SFN prime at which this sib segment received*/
  uint8 seg_index,  /*SIB segment index*/
  tdssib_segment_e_type sib_seg_type,/*Used to indicate type of sib segment*/
  uint32 seg_data_length,  /*SIB segment data lenght*/
  void *seg_data_ptr,  /*pointer to sib segment data*/
  tdssib_events_index_e_type event_index    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
);
/*===========================================================================

FUNCTION tdsrrcsib_is_sib_seg_in_tsib_optim_db

DESCRIPTION
  This function will say whether a sib segment 
  of a sib type is stored in tsib optimization db
  by looking at look up table..
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE : if a sib segment is stored in tsib optimization db
  FASLE: otherwise.

SIDE EFFECTS
  None.
===========================================================================*/
boolean tdsrrcsib_is_sib_seg_in_tsib_optim_db
(
  tdsrrc_SIB_type sib_type
);
/*===========================================================================

FUNCTION tdsrrcsib_process_sib_seg_in_tsib_optim_db

DESCRIPTION
  This function will process stored sib sements 
  in tsib optimization db if any by looking at
  look up table..
  
DEPENDENCIES
  None.

RETURN VALUE
  If a complete sib is constructed at the end of this function then returns = TDSSEG_PROC_COMPLETE.
  else
  Return TDSSEG_PROC_NONE 

SIDE EFFECTS
  None.
===========================================================================*/
TDSseg_proc_stat_e_type tdsrrcsib_process_sib_seg_in_tsib_optim_db
(
  tdsrrc_SIB_type sib_type,   /*Sib type*/
  tdssib_events_index_e_type index    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
);

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/*===========================================================================
FUNCTION tdsrrcsib_can_event_support_deferred_sib

DESCRIPTION

  This function checks whether the current event can support Deferred SIB reading. s
  
DEPENDENCIES
  None

RETURN VALUE
  Boolean: True, if event can support Deferred SIB reading

SIDE EFFECTS
  None.

===========================================================================*/

boolean tdsrrcsib_can_event_support_deferred_sib
(
  tdssib_events_index_e_type index
);
/*===========================================================================

FUNCTION  tdsrrcsib_update_sib_for_deferred_sib

DESCRIPTION

  This function will post Update SIBS comand for deferred SIB reading.

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sib_for_deferred_sib
(
  void
);

/*===========================================================================

FUNCTION  tdsrrcsib_post_process_deferred_sib

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

void tdsrrcsib_post_process_deferred_sib(tdssib_events_index_e_type index,   uint32 dbindex);


/*===========================================================================

FUNCTION rrcsib_is_event_deferred_meas_read

DESCRIPTION

  This function will check whether the event is UPDATE_SIBs
  due to deferred SIB reading.
  
DEPENDENCIES
  None.

RETURN VALUE
  TRUE: If this event is UPDATE_SIBs due to deferred SIB reading.
  FALSE: Other wise

SIDE EFFECTS
  None.
     
===========================================================================*/

boolean tdsrrcsib_is_event_update_sibs_for_deferred_sib
(  
  tdssib_events_index_e_type  event_index
);
/*===========================================================================

FUNCTION  rrcsib_is_sib12_present_in_serving_cell

DESCRIPTION

  This function will check if SIB12 is present for the serving cell.

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/

boolean tdsrrcsib_check_if_sib12_present_for_dbindex
(
  int32 dbindex
);

/*===========================================================================

FUNCTION tdsrrcsib_store_sib_seg_in_deferred_sib_db

DESCRIPTION
  This function will clear all SIBs in tsib optimization data base except SIB11/11bis/12.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdsrrcsib_store_sib_seg_in_deferred_sib_db
(
  void
);

#endif
/*===========================================================================
FUNCTION tdsrrcsib_map_asn1_sibtype_to_int_sibtype

DESCRIPTION

  This function maps the value used for an ASN1 sib type to the corresponding internal sib type.
  
DEPENDENCIES
  None

RETURN VALUE
  Value used for internal SIB type

SIDE EFFECTS
  None.

===========================================================================*/

tdsrrc_SIB_type tdsrrcsib_map_asn1_sibtype_to_int_sibtype
(
  tdsrrc_SIB_Type asn1_sib_type
);
/*===========================================================================
FUNCTION tdsrrcsib_map_asn1_sibtype_to_int_sibtype

DESCRIPTION

  This function maps the value used for an ASN1 sib type to the corresponding internal sib type.
  
DEPENDENCIES
  None

RETURN VALUE
  Value used for internal SIB type

SIDE EFFECTS
  None.

===========================================================================*/

tdsrrc_SIB_Type tdsrrcsib_map_int_sibtype_to_asn1_sibtype
(
  tdsrrc_SIB_type int_sib_type
);

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
/*===========================================================================

FUNCTION tdsrrcsib_map_extension_sib_to_internal_sibtype

DESCRIPTION
  This function will return the SIB type in internal format for extension SIBs. It will find out the SIB type based on
  the scheduling info and SFN at which the SIB arrives.
  
DEPENDENCIES
  None.

RETURN VALUE
  Returns the SIB type based on the SFN at which it arrives. 
  
SIDE EFFECTS
  None.
===========================================================================*/
tdsrrc_SIB_type tdsrrcsib_map_extension_sib_to_internal_sibtype
(
  tdsrrc_SegmentIndex seg_index,
  uint32 sfn_prime
);
#endif

/*===========================================================================

FUNCTION tdsrrcsib_check_sib_schedule_for_sibs

DESCRIPTION
  Check if the received bcch message fits into the segment schedule received
  in MIB/SB1 or SB2 earlier.

DEPENDENCIES
 
  None.

RETURN VALUE
  TRUE:  if segment matches schedule
  FALSE: if segment does not match schedule received

SIDE EFFECTS

  None.
        
===========================================================================*/

boolean tdsrrcsib_check_sib_schedule_for_sibs
(
  tdsrrc_SystemInformation_BCH * msg_ptr,
  tdssib_events_index_e_type index
);


/*===========================================================================
FUNCTION    tdsrrc_log_complete_sib 

DESCRIPTION
  This function will log rrc_System_Information_Block_PDU message into QXDM.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void tdsrrc_log_complete_sib
(
  tdsrrc_SIB_Type  sib_Type,
  tdsrrc_SegmentIndex seg_index,
  uint32 sfn_prime
);


#ifdef FEATURE_TDSCDMA_SPECIFIC_SIB_READING	
/*===========================================================================

FUNCTION tdsrrcsib_update_sib_special_reading_schedule_table

DESCRIPTION

  This function updates the SIB schedule table  
  
DEPENDENCIES
 
  If rrc_sib_type is specific, we generate schedule table for specific SIB .
  If rrc_sib_type is TDSRRC_TOT_SIBS, we generate schedule table for all unreceived SIB.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_update_sib_special_reading_schedule_table
(
  tdsrrc_SIB_type rrc_sib_type,                /* Indicates whether this update is from MIB or SBs */
  tdssib_events_index_e_type event_index   /* Index of the current event */
);
#endif

#endif /* TDSRRCSIBPROCI_H */

