/*===========================================================================
                        R R C   S I B   P R O C E S S I N G  

DESCRIPTION 

  This module performs the System Information Blocks processing for RRC.
  This module is the main module for SIB processing and it processes
  all the commands related to SIB processing. This module registers
  requests from other RRC modules to read and store SIBs from the BCCH. 
  When all the required SIBs have been read and stored, it informs the other
  RRC modules that the request has been completed.
  

  The other modules for SIB processing are

  rrcsibcollect  -  Collects encoded SIBs, concatenates SIB segments
                    if needed. 

  rrcsibdb       -  Stores the SIB database and provides access
                    functions for the SIB database.
                  
  These modules are controlled by the main SIB processing module.                 
                  

EXTERNALIZED FUNCTIONS

  tdsrrcsib_event_controller  Receives and processes all events related
                            to SIB processing.

  tdsrrcsib_proc_init         Power-up initialization of the SIB procedure
                            including the SIB database.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  
  The function tdsrrcsib_proc_init must be called once to initialize
  this procedure when the phone powers-up.

Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated.  
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsibproc.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/15/12   gv      Added support to log new event when complete SIB is received
11/05/11   ysh      for FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH, after received sib7,
                               need drop PCCPCH.
11/05/11   ysh      delete tdsrrcsib_check_if_band_supported, follow KW defined but not used.
07/04/11   mkh     Cleanup obsolete feature FEATURE_RRC_SIB7_UPDATE_IN_FACH. 
06/16/11   mkh     Changes to pass pval update ind to MAC only if nASC > 0 and
                   if SIB6 has PRACH_systemInfoList present (for connected mode)
06/09/11   mkh     Added support to pass pval to MAC on reading SIB7 in FACH
06/09/11   ttl     Removed the dup code from last upmerge.
05/27/11   ttl     Clean up the temp changes added during the Upmerge.
05/09/11   mkh     Support to send SIB info to L1 in CELL_FACH state
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53
01/18/11   mkh     Changes to stub out tdsrrcsib_check_sib_schedule_for_sibs for
                   RRC QTF
08/24/10   rm      Fixed compiler warnings
08/18/10   ss      Added SIB19 logging code.

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
#include "err.h"
#include "msg.h"
#include "tdsrrcasn1.h"
#include "tdsrrcasn1util.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmdi.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcllc.h"
#include "tdsrrcllcoc.h"
#include "tdsrrclcm.h"
#include "tdsrrcscmgr.h"
#include "tdsrrcsibproc.h"
#include "tdsrrcsibdbi.h"
#include "tdsrrcsibdb.h"
#include "tdsrrctmr.h"
#include "tdsrrcumi.h"
#include "tdsuecomdef.h"
#include "tdsrrccsp.h"
#include "tdsrrcllcoc.h"
#include "tdsrrccspi.h"
#include "tdsrrcccm.h"
#include "tdsrrcpg1.h"
#include "tdsrrcrce.h"
#include "tdsrrccspi.h"
#include "tdsrrcnv.h"

#include "tdsrrclogging.h"

#include "tdsrrcmcm.h"
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
#include "bit.h"
#endif

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
#include "tdsrrcdispatcher.h"
#endif

#ifdef TEST_FRAMEWORK
#error code not present
#endif

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#include "tdsrrcfreefloating.h"
#endif

#include "tdsrrcwrm.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */
#ifdef FEATURE_3GPP_CSFB_TDS
  typedef struct tdsrrcsib_system_info_cont_store_type {
    tdspdu_buf_type mib_encoded;
    tdspdu_buf_type SB1_encoded;
    tdspdu_buf_type SB2_encoded;
    tdspdu_buf_type sib1_encoded;
    tdspdu_buf_type sib3_encoded;
    tdspdu_buf_type sib5_encoded;
    tdspdu_buf_type sib7_encoded;
    tdspdu_buf_type sib11_encoded;
    tdspdu_buf_type sib11bis_encoded;
    tdspdu_buf_type sib12_encoded;
  } tdsrrcsib_system_info_cont_store_type;
#endif

/* Define a maximum time to wait for SIBs in a new cell. Right now
it's selected as 12 seconds. This may be changed later if needed */
#define TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS 12000

/* 
   Define the wait factor of SIB for BPLMN search as 30(3 * 10), which is 3 times the 10 ms frame numbers 
   Also set the wait timer for receiving a MIB to be 300ms, if not received by that time, we should not
   bother reading this cell
*/
#define TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR 30
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#else
#define TDSRRCSIB_BPLMN_MIB_WAIT_TIMER  300
#endif
/* For manual bplmn search, shorten the mib wait time to 160ms*/
#define TDSRRCSIB_MANUAL_BPLMN_MIB_WAIT_TIMER_IN_MS  160

#define TDSRRCSIB_NBCCH_MIB_WAIT_TIMER  2000

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
/*In most of the N/W SIB3 ret cycle is set to 640 msec
  So set MAx time to wait for SIB3 to 640msec, Any ways
  correct value of the timer will be started after receiving
  MIB for this feature*/
#define TDSRRCSIB_MAX_SIB3_WAIT_TIMER  640

/*We want to give only one chance for reading SIB3
   in DCH so wait factor 1*10msec = 10*/
#define TDSRRCSIB_CELL_ID_SIB3_WAIT_FACTOR 10
#endif

/* Define a maximum time to wait for SIBs in a existing cell. 
This is when we're already camped on the cell but we receive
indication that the SIBs have been modified. Here we wait
for a longer time since the SFN at which they will be modified
may be in the future. Worst case is 1 SFN cycle plus 12 seconds. */
#define TDSRRCSIB_MAX_WAIT_TIME_FOR_UPDATE_SIBS 52880

/* Default expiration time for SIB7 */
#define TDSRRCSIB_DEFAULT_EXP_TIME_FOR_SIB7 320

/* For TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER */
#define TDSRRCSIB_SIB_SCHEDULE_UPDATE_WAIT_TIME_MS  (10)

/* Define minimum SIB repitition for MIB */
#define TDSRRCSIB_MIB_MIN_REP 8

#if  defined(FEATURE_TDSCDMA_DRX_MANAGER) || defined(FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH)
  /* Size of SIB Schedule table in bytes */
  #define TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE 512

  /* Both the below values should not be odd */
  #define TDSRRCSIB_SLEEP_SFN 16
  #define TDSRRCSIB_WAKE_SFN  16

  /* Minimum duration for SIB mask invert timer */
  #define TDSRRCSIB_MIN_MASK_INV_TIMER (TDSRRCSIB_SLEEP_SFN + TDSRRCSIB_WAKE_SFN)
#endif
  
#define TDSRRCSIB_SB_SLEEP_SCHEDUING_MIN_FRAME    (128)
#define TDSRRCSIB_SB_SLEEP_SIB_END_FRAME          (70)
#define TDSRRCSIB_INVLID_SFN_PRIME                (0xFFFF)
#define TDSRRCSIB_MAX_SFN_PRIME                   (2048)

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* Declare a 'tdssib_events' database. This is used to store information
about each active SIB processing event. */
tdssib_active_event_database_type *tdssib_events_ptr=NULL;

/* Boolean to indicate if an TDSRRCSIB_UPDATE_SIBS event is pending. */
boolean tdsupdate_sibs_event_pending;

/* Data type to hold information about SIB7 expiration */
tdsrrc_sib_exp_timer_sib_info_type tdssib7info;

/* Enumerated type of status of configuring BCCH for new
events. */
typedef enum
{
  TDSRRCSIB_BCCH_SETUP_FAILED, /* BCCH setup failed */
  TDSRRCSIB_BCCH_EVENT_SAVED,  /* The event is saved for later due to
                            existing BCCH event  */
  TDSRRCSIB_BCCH_SETUP_DONE    /* BCCH setup was done successfully */
}tdsrrcsib_bcch_setup_status_e_type;

/* To note if power-up initialization has been done already.
This is used in initializing variables for cases other than power-up
init such as FTM and LPM */
boolean tdsrrcsib_powerup_init_done = FALSE;

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  rex_crit_sect_type tdssib7_in_fach_active_mutex;
  boolean tdsis_sib7_in_fach_active = FALSE;
#endif
#endif

/* SIB procedure will fill the channel config request with the current value of the 
transaction id. Transaction ids will help SIB associate a channel config confirm
with a channel config request that it had sent */
uint8  tdstransaction_id = 0;

/* Global Flag which prevents BPLMN(If BPLMN request is received when UPDATE_SIBs
  was on going) to start till all the SIB_Change_ind commands are processed by RRC 
  procedures & L1*/
boolean tdssib_change_safe_guard;

/* Boolean to indicate whether SIB Sleep table will be sent to L1 only after receiving
scheduling info for all SIBs. */
boolean tdssib_sleep_after_sb1_sb2 = TRUE;

//lookup table to keep track of supported sibs
uint8 tdssupported_sibs_lookup_table[TDSRRC_TOT_SIBS];

#if  defined(FEATURE_TDSCDMA_DRX_MANAGER) || defined(FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH)
/* SIB schedule table that holds the SIB scehduling info */
uint8 tdssib_sleep_schedule_table_0[TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE];
uint8 tdssib_sleep_schedule_table_1[TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE];
uint8 tdssib_sleep_schedule_table_backup[TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE];
uint8 tdssib_sleep_schedule_table_in_use;
#endif

/* Structure to store BPLMN SIBs */
tdsrrcsib_bplmn_sibs_type tdsrrcsib_bplmn_sibs;

/* Global command store */
tdsrrcsib_cmd_store_type tdsrrcsib_cmd_store;

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
uint8 tdsno_mib_bar_cell_count=0;
#endif

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
extern tdsrrc_deferred_sib_info_type tdsdeferred_sib_info;
#endif

#ifdef FEATURE_3GPP_CSFB_TDS
extern boolean tdsrrc_csfb_call_status ;
#endif

tds_nv_sib_sleep tdsrrcsib_sib_sleep;

#define TDSRRCSIB_SB_SLEEP_TEST_ENABLED  ((tdsrrcsib_sib_sleep.sib_sleep_flag & 0x0001) && (tdsrrcsib_sib_sleep.sib_sleep_flag & 0x0100))

static uint8 num_bits_half_byte[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};

#define TDSRRCSIB_BYTE_BITS(x) (num_bits_half_byte[(x) & 0x0F] + num_bits_half_byte[(x) >> 4])

boolean tdsrrcsib_pattern_invert = FALSE;
boolean tdsrrcsib_stop_partition = FALSE;
boolean tdsrrcsib_g_in_data_call = FALSE;

#define TDSRRCSIB_TABLE_MOD_MASK       0x01FF
#define TDSRRCSIB_TABLE_INX_INC(idx) (((idx)+1) & TDSRRCSIB_TABLE_MOD_MASK)
/*The below statement is to get the distance from idx2 to idx1:
    If it is greater than 256, it means idx1 < idx2 
    If it is smaller than 256, it means idx1 > idx2*/
#define TDSRRCSIB_TABLE_INX_GT( idx1, idx2, idx_modulus) \
            (((idx1) != (idx2)) && ((((idx1) - (idx2)) & (idx_modulus >> 1)) == 0))

/* TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE is the size of SIB schedule table in bytes in which every bit represents a SFN.
TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_MASK_SIZE is the size of SIB schedule table mask in bytes in which every byte represents a SFN. */
#define TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_MASK_SIZE (TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE << 3)

/* When operate on "table_mask" and "table_ptr", to avoid wrap around at the boundary, it will process the 2nd max SIB repetition. 
It should be OK because "table_mask" and "table_ptr" have the same value in every max SIB repetition. */
#define TDSRRCSIB_HOLE_ENHANCED_OPTIMIZATION_REPT_INDEX 1

/* When operate on "table_mask" and "table_ptr", to avoid wrap around at the boundary, it will process the 5th max SIB repetition.
Table_mask of the 3rd max SIB repetition may be modified by enhanced optimization, so let us use the 5th one.
It should be OK because "table_mask" and "table_ptr" have the same value in every max SIB repetition. */
#define TDSRRCSIB_HOLE_BASIC_OPTIMIZATION_REPT_INDEX 4

/* The below statement is to get some specified bit from "table_ptr".
"table_ptr" is a pointer to an array which is uint8 type. Every bit of an item of the array indicates whether to collect SIB in a SFN(10ms).
The MSB of an item of the array represents the smaller SFN. The LSB of an item of the array represents the larger SFN. */
#define TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, i) \
            (table_ptr[(i) >> 3] & (1 << (7 - ((i) & 0x07))))

/* Combin the following 4 sib sfn bit masks starting with "idx" in "table_ptr" to form a uint32 item */
#define TDSRRCSIB_COMBIN_SFN_BM(table_ptr, idx) \
            (((uint32)table_ptr[(idx)] << 24)|((uint32)table_ptr[(idx) + 1] << 16)|((uint32)table_ptr[(idx) + 2] << 8)|((uint32)table_ptr[(idx) + 3]))

/* record the failure count of SIB hole optimization. */
uint8 tdsrrcsib_hole_opt_fail_count = 0;


#ifdef FEATURE_TDSCDMA_SIB19
/* in real network, SIB19 decode failure happens on some cell,
   we also could try to camp on. */
uint8 tdsrrcsib_sib19_decode_fail_count = 0;
#define TDSRRCSIB_SIB19_DECODE_FAIL_COUNT_VALUE 3
#endif

/* Reset MIB waiting timer after channel setup cnf due to DSDS delay.
   Since TDSRRC_UPDATE_SIBS_IND timer is long enough, 
   only consider for specific SIB event.
   Use TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS for timer value 
   before channel setup and use this saved value to reset the timer
   after channel setup success */
uint32 tdsrrcsib_specific_sib_timer_val = 0;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */
#define TDSGET_TRANS_ID() (++tdstransaction_id % 255)

/*Maximum number of segments received per BCCH block */
#define TDSMAX_SEGMENTS_PER_BCCH_BLOCK 18


/*===========================================================================
** -----------------------------------------------------------------------------------
**-- ----------NON-Demand paging section Srart--------------------------------------------
** -----------------------------------------------------------------------------------
  This section will have code section that will not be demand paged. Function which should be in this section are
  -RRC functions that are called by L1/L2 which are higher priority then RRC
  -RRC functions that are called in ISR context or RTOS timer call back
  -RRC functions that indirectly gets called by L1/L2 /ISR/timer call back   
  For example: Fun1() gets called by L1. Fun2() is called by Fun1(). Now both  Fun1() & Fun2() should be NON demand paged, 
  so both should be added in this section  
  Usage of pragma 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void foo(void)
  {
    function body here
  }
        __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
  If you have another function to be added in the same section then again it needs to be wrapped with these pragma 
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ? 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */



/*===========================================================================

FUNCTION RRCSIB_IS_SIB_ACTIVE

DESCRIPTION

  
    
DEPENDENCIES

  None.
 
RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
boolean tdsrrcsib_is_sib_active
(
  void
)
{
  uint32 count;
  
  /* Check if there is anything pending in the cmd store */
  if(tdsupdate_sibs_event_pending == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB active: Update SIBS pending flag is true");
    return TRUE;
  }

  if (tdssib_events_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdssib_events_ptr is NULL");
    return FALSE;
  }

  /* Check if any event is active */
  for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB event %d active",tdssib_events_ptr->event[count].event_name);
      return TRUE;
    }/* End if active events */
  }
  
  /* Nothing is active.Return FALSE */
  return FALSE;
}                
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

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
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

uint8 tdsrrcsib_gen_bit_mask_for_schedule_table
(
  uint8 byte_offset
)
{
  uint8 bit_mask = 0xFF;
  
  /* The byte_offsets can only be 0,2,4 or 6.Since SIBs are always scheduled on 20ms
       frames,the next sfn after the offset also needs to be set to 1.Always OR the bitmask
       so that settings from other SIB Schedule will not be lost*/
  switch(byte_offset)
  {
    case 0:
      bit_mask = 0xC0; /* 1100 0000 */
    break;
    case 2:
      bit_mask = 0x30; /* 0011 0000 */
    break;
    case 4:
      bit_mask = 0x0C; /* 0000 1100 */   
    break;
    case 6: 
      bit_mask = 0x03; /* 0000 0011 */
    break;
    default:
    /* This cannot happen since Scheduling is always based on even numbered SFN.
         Be conservative and set it to 0xFF */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB_Sleep:Invalid segment offset for SIB ");
      bit_mask = 0xFF;
    break;
  }
  return bit_mask;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif

/*===========================================================================

FUNCTION tdsrrcsib_gen_empty_bit_mask_for_schedule_table

DESCRIPTION

  This function will generate empty Bit mask needed for filling 
  SIB schedule table
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/

uint8 tdsrrcsib_gen_empty_bit_mask_for_schedule_table
(
  uint8 byte_offset
)
{
  uint8 bit_mask = 0x00;
  
  /* The byte_offsets can only be 0,2,4 or 6.Since SIBs are always scheduled on 20ms
       frames,the next sfn after the offset also needs to be set to 1.Always OR the bitmask
       so that settings from other SIB Schedule will not be lost*/
  switch(byte_offset)
  {
    case 0:
      bit_mask = 0x00; /* 0000 0000 */
    break;
    case 2:
      bit_mask = 0xC0; /* 1100 0000 */
    break;
    case 4:
      bit_mask = 0xF0; /* 1111 0000 */   
    break;
    case 6: 
      bit_mask = 0xFC; /* 1111 1100 */
    break;
    default:
    /* This cannot happen since Scheduling is always based on even numbered SFN.
         Be conservative and set it to 0xFF */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB_Sleep:Invalid segment offset for SIB ");
      bit_mask = 0x00;
    break;
  }
  return bit_mask;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

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
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
boolean tdsrrcsib_is_current_event_sib7_in_fach(void)
{
  uint8 count;
  boolean status = FALSE;
  for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
      if(tdssib_events_ptr->event[count].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)
      {
        if((tdssib_events_ptr->event[count].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)&&
           (tdssib_events_ptr->event[count].req_proc == TDSRRC_PROCEDURE_SIB)&&
           (tdssib_events_ptr->event[count].for_serving_cell == TRUE)&&
           (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
          )
        {
          status = TRUE;
          break;
        }
      }
    }/* End if active events */
  }
  return status;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

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
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
boolean tdsrrc_sib_is_sib_segment_needed_by_rrc_fach(uint16 sfn)
{
  boolean status = FALSE;

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  boolean sib7_in_fach_status = FALSE;

  REX_ISR_LOCK(&tdssib7_in_fach_active_mutex);
  sib7_in_fach_status = tdsis_sib7_in_fach_active;
  REX_ISR_UNLOCK(&tdssib7_in_fach_active_mutex);

  if(sib7_in_fach_status == TRUE)
  {
    uint16 byte_pos = sfn>>3;                /* (sfn/8) Gives the byte location in sib sleep schedule table */
    uint8 byte_offset = (sfn & 0x07);     /* (sfn%8)Gives the offset within the byte in sib sleep table */
    uint8 bit_mask = 0xFF;
    bit_mask = tdsrrcsib_gen_bit_mask_for_schedule_table(byte_offset);
    if((tdssib_sleep_schedule_table_0[byte_pos]&bit_mask) != 0)
    {
      status = TRUE;
    }
  }
  else
  {
    status = TRUE;
  }
#else
  if(tdsrrcsib_is_current_event_sib7_in_fach() == TRUE)
  {
    uint16 byte_pos = sfn>>3;                /* (sfn/8) Gives the byte location in sib sleep schedule table */
    uint8 byte_offset = (sfn & 0x07);     /* (sfn%8)Gives the offset within the byte in sib sleep table */
    uint8 bit_mask = 0xFF;
    bit_mask = tdsrrcsib_gen_bit_mask_for_schedule_table(byte_offset);
    if((tdssib_sleep_schedule_table_0[byte_pos]&bit_mask) != 0)
    {
      status = TRUE;
    }
  }
  else
  {
    status = TRUE;
  }
#endif
#endif
  return status;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================
FUNCTION tdsrrc_sib_can_l1_start_fmo
DESCRIPTION
  This function will check whether the current event is sib-7 event 
  which is needed for starting FMO's at L1 in FACH state.
  FMO should take high priority than SIB-7 event.During the 
  frames where FMO's are going on L1 will through away all
  the sib segments, since the content may be incorrect.
DEPENDENCIES
  None.
RETURN VALUE
  TRUE: If event is sib-7 event in FACH, or no sib event present.
  FALSE: If event is other than sib-7 event.
SIDE EFFECTS
  None.
===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
boolean tdsrrc_sib_can_l1_start_fmo(void)
{
  boolean  status = FALSE;
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  boolean  sib7_in_fach_status = FALSE;

  REX_ISR_LOCK(&tdssib7_in_fach_active_mutex);
  sib7_in_fach_status = tdsis_sib7_in_fach_active;
  REX_ISR_UNLOCK(&tdssib7_in_fach_active_mutex);

  if(sib7_in_fach_status == TRUE)
  {
    /*SIB-7 event is active in Cell FACH,So FMO can be started  */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L1 can Start FMO,SIB7 event is active");
    status = TRUE;
  }
  else
  {
    if(tdsrrcsib_is_sib_active() != TRUE)
    {
      /*No SIB event active, So FMO can be started*/
      status = TRUE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"L1 to skip FMO,as SIB event is active");
    }
  }
#else
  if(tdsrrcsib_is_current_event_sib7_in_fach() == TRUE)
  {
    /*SIB-7 event is active in Cell FACH,So FMO can be started  */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"L1 can Start FMO,SIB7 event is active");
    status = TRUE;
  }
  else
  {
    if(tdsrrcsib_is_sib_active() != TRUE)
    {
      /*No SIB event active, So FMO can be started*/
      status = TRUE;
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"L1 to skip FMO,as SIB event is active");
    }
  }
#endif
#endif

return status;
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/* =======================================================================
**                            Function Definitions
** ======================================================================= */

#if  defined(FEATURE_TDSCDMA_DRX_MANAGER)
/*===========================================================================

FUNCTION RRCSIB_INITIALIZE_SIB_SLEEP_SCHEDULE_TABLE

DESCRIPTION

  This function initializes the SIB schedule table  
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_initialize_sib_sleep_schedule_table
(
  void
);

/*===========================================================================

FUNCTION RRCSIB_UPDATE_SIB_SLEEP_SCHEDULE_TABLE

DESCRIPTION

  This function updates the SIB schedule table  
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_update_sib_sleep_schedule_table
(
   tdssib_events_index_e_type event_index   /* Index of the current event */
);

void tdsrrcsib_process_sib_sleep_before_sb
(
   uint16 mib_sfn_pri,
   uint16 sb_rep_ttis,
   uint32 sb_offset,
   tdssib_events_index_e_type event_index 
);

#endif /* #if defined(FEATURE_TDSCDMA_DRX_MANAGER) */

/*===========================================================================

FUNCTION RRCSIB_CHECK_BAND_MISMATCH_FROM_SIB5_SIB6

DESCRIPTION

  This function will be used to find if the band present in SIB5/SIB6
  is supported by the UE or not.
  
DEPENDENCIES

  None.

RETURN VALUE

  TRUE - If there is a band mismatch
  FALSE - If there is no band mismatch or the band info is not specified in SIB5/SIB6
  
SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrcsib_check_band_mismatch_from_sib5_sib6(uint32 index);


/*===========================================================================

FUNCTION POST_PROCESS_COMPLETED_EVENT

DESCRIPTION

  This function performs all the necessary post-processing for events
  that have been completed (i.e. all necessary sibs have been received).
  Post-processing includes -
  
    1) Sending a confirmation event if necessary to the module that
       requested the event.
       
    2) Tearing down the BCCH radio-bearer if it's not needed anymore.
    
    3) Cleanup of internal variables related to the event.
    
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_post_process_completed_event
(
  tdssib_events_index_e_type index /* Index of the event in the sib event database */
);

void tdsrrcsib_get_nv_items(void);


#ifdef FEATURE_TDSCDMA_SIB19
/*===========================================================================

FUNCTION tdsrrcsib_check_and_remove_sib19_if_not_scheduled

DESCRIPTION

  This function check if SIB19 is presnt in sibdb & then check if SIB19 
  was removed from MIB/SB1/SB2 if yes then remove the entry from SIBdb
DEPENDENCIES

  None.

RETURN VALUE

  TRUE - If SIB4/6/12 Ind present in SIB3/5/11
  FALSE - Otherwise
  
SIDE EFFECTS

  None

===========================================================================*/
void tdsrrcsib_check_and_remove_sib19_if_not_scheduled
(
  uint32 index  /* Index of the event in the sib events database */
)
{
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List2 *ext_siblist2_ptr = NULL;
  uint32 dbindex; /* Index to look into the database if needed */
  tdsrrc_MasterInformationBlock *mib_ptr; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr;         /* Local pointer to SB2       */
  boolean is_sib19_present = FALSE;
  uint8 idx=0;

  /* Check if the event is active. It could have failed
   due to a non-gsm map PLMN or a timeout, in which case
   the event will be inactive. */
  if( tdssib_events_ptr->event[index].event_active == FALSE )
  {
    /* Nothing to do here */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:return no event active");
    return;
  }
  if( tdssib_events_ptr->event[index].sib_db_index_found == TRUE )
  {
    /* Database index is found. Get the index */
    dbindex = tdssib_events_ptr->event[index].sib_db_index;
  }
  else
  {
    /* If the database index has not been found, the
    event can't be completed. Just return FALSE */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:return no sib db idx active");
    return;
  }

  if(tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB19] == TRUE)
  {
    mib_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB,dbindex);
    
    if((mib_ptr != NULL)&&(TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_MIB(mib_ptr)))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:get ext_siblist2_ptr from MIB");
      ext_siblist2_ptr = &mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.
          v860NonCriticalExtensions.masterInformationBlock_v860ext.extSIBTypeInfoSchedulingInfo_List;
    }

    if(ext_siblist2_ptr == NULL)
    {
      sb2_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB2,dbindex);
      if((sb2_ptr != NULL)&&(TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(sb2_ptr,
          tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2)))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:get ext_siblist2_ptr from SB2");
        ext_siblist2_ptr = &sb2_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
         sysInfoTypeSB2_v860ext.extSIBTypeInfoSchedulingInfo_List;
      }
      if(ext_siblist2_ptr == NULL)
      {
        sb1_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB1,dbindex);
  
        if((sb1_ptr != NULL)&&(TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(sb1_ptr,
            tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1)))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:get ext_siblist2_ptr from SB1");
          ext_siblist2_ptr = &sb1_ptr->v6b0NonCriticalExtensions.
             v860NonCriticalExtensions.sysInfoTypeSB1_v860ext.extSIBTypeInfoSchedulingInfo_List;
        }
      }
    }

    while((ext_siblist2_ptr != NULL) && (ext_siblist2_ptr->n > idx))
    {
      if(ext_siblist2_ptr->elem[idx].extensionSIB_Type2.t == T_tdsrrc_SIB_TypeExt2_systemInfoType19)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19:Sched present for SIB19 in MIB,bit mask val =%x",ext_siblist2_ptr->elem[idx].extensionSIB_Type2.t);
        is_sib19_present = TRUE;
        break;
      }
      idx++;
    }
    if(is_sib19_present == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:removed sib19 from SIBdb");
      if( tdssibdb.cell[dbindex].sibs.sib19 != NULL )
      {
        tdsrrcsibdb_free_sib(tdsrrc_SysInfoType19_PDU,
                           (void *)tdssibdb.cell[dbindex].sibs.sib19);
        tdssibdb.cell[dbindex].sibs.sib19 = NULL;
      }
#ifdef FEATURE_RRC_OPTIMIZED_SIB_HEAP
      memset(tdssibdb.cell[dbindex].sibs.sib19_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
      tdssibdb.cell[dbindex].sibs.sib19_encoded_pdu_length = TDSINVALID_PDU_LENGTH;
#endif
      if(tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:Mark sibdb for sending SIB change ind for SIB19");
        tdssibdb.active_cell_sib_changed[tdsrrc_SIB19] = TRUE;
      }
      tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB19] = FALSE;
      tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] = TDSINVALID_MIB_SB_VALUE_TAG;
    }
  }
}
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
  tdsrrc_SIB_type rrc_sib_type
)
{
  boolean status = FALSE;
  switch(rrc_sib_type) 
  {
    case tdsrrc_SIB3:
      if(val & TDSSIB4_PRESENT_BIT_MASK)
      {
        status = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 Ind present in SIB3");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 Ind NOT present in SIB3");
      }
      break;

    case tdsrrc_SIB5:
    case tdsrrc_SIB5bis:
      if(val & TDSSIB6_PRESENT_BIT_MASK)
      {
        status = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB6 Ind present in SIB5");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB6 Ind NOT present in SIB5");
      }
      break;

    case tdsrrc_SIB11:
      if(val & TDSSIB11_PRESENT_BIT_MASK)
      {
        status = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB12 Ind present in SIB11");
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB12 Ind NOT present in SIB11");
      }
      break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIBtype %d not supported",rrc_sib_type);
        break;
  }
  return status;
}

/*===========================================================================

FUNCTION RRCSIB_CONVERT_SIB7_REP_TIME_TO_MS

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
static uint32 tdsrrcsib_convert_sib7_rep_time_to_ms
(
tdsrrc_SchedulingInformation * sib_rep,
tdsrrc_ExpirationTimeFactor exp_factor
)
{
  uint32 sib7exp;

  switch(TDSRRCGET_SCHD_INFO_TAG((&sib_rep->scheduling.sib_Pos)))
  {
    //convert the rep rate to ms and multiply with exp_factor.. rep4 represents
    //4 frames or 40ms and so on..
    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep4):
      sib7exp = 40 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep8):
      sib7exp = 80 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep16):
      sib7exp = 160 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep32):
      sib7exp = 320 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep64):
      sib7exp = 640 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep128):
      sib7exp = 1280 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep256):
      sib7exp = 2560 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep512):
      sib7exp = 5120 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep1024):
      sib7exp = 10240 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep2048):
      sib7exp = 20480 * exp_factor;
      break;

    case      TDSRRCGET_SCHD_INFO_SIB_POS(rep4096):
      sib7exp = 40960 * exp_factor;
      break;

    default:

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB_REP in MIB!");
      sib7exp = TDSRRCSIB_DEFAULT_EXP_TIME_FOR_SIB7;
      break;
  }

  return sib7exp;
}
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
unsigned long  tdsrrcsib_get_sib7_exp_timer_value( void )
{
  unsigned long sib7exp=0;        /* To store SIB7 expiration time   */
  uint32 index;                   /* Index into sib database         */
  tdsrrc_SIBSb_ReferenceList *sb_ptr; /* Local pointer to SIBSb ref list */
  tdsrrc_SIB_ReferenceList *ptr;      /* Local pointer to SIB ref list   */
  boolean sib7_found = FALSE;     /* Indicate if sib7 is found       */
  boolean sb1_found = FALSE;      /* Indicate if sb1 is found incase
                                     sib7 is not present in MIB      */
  boolean sb2_found = FALSE;      /* Indicate if sb2 is found incase
                                     sib7 is not present in MIB      */
  tdsrrc_ExpirationTimeFactor exp_factor; /* Expiration timer factor    */
  uint32 count=0, n_value=0;

  /* First check if SIB7 has the expiration timer factor
  present. If not, we just return the default timer value */
  index=tdssibdb.active_cell_index;  /* Note the current serving cell   */

  if(tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SIB7, index) != NULL)
  {
    if(TDSRRCCHECK_SIB_MSG_IE_PRESENT(tdssibdb.cell[index].sibs.sib7,
                                      rrc_SysInfoType7_,
                                      expirationTimeFactor)
    )
    {
      /* Expiration timer factor is present, exp_factor = 2^expirationTimeFactor
         ExpirationTimeFactor ::= INTEGER (1 . . 8 )
      */
      exp_factor = ((uint16) 0x1)<<(tdssibdb.cell[index].sibs.sib7->expirationTimeFactor);
    }
    else
    {
      /* Expiration timer factor not present */
      return( TDSRRCSIB_DEFAULT_EXP_TIME_FOR_SIB7 );
    }
  }
  else
  {
    /* Can't find SIB 7! */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB7 not present in srv cell!");
    return( TDSRRCSIB_DEFAULT_EXP_TIME_FOR_SIB7 );
  }

  if(tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB, index) != NULL)
  {
    /* If the MIB is valid */
    sb_ptr= &(tdssibdb.cell[index].sibs.mib->sibSb_ReferenceList);
    count = 0;
    n_value = sb_ptr->n;

    while( n_value != 0 )
    {
      /* Check if this is for SIB7 */
      
      if(TDSRRCGET_SCHD_INFO_TAG((&sb_ptr->elem[count].sibSb_Type)) == 
         (TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType7)))
      {
        /* We found it */
        sib7_found = TRUE;
        break; /* Out of the while loop */
      }

      /* Else check if this is for SB1 */
      if(TDSRRCCHECK_SIBSb_TypeAndTag((&sb_ptr->elem[count].sibSb_Type),sysInfoTypeSB1))
      {
        /* We found it */
        sb1_found = TRUE;
      }

      /* Else check if this is for SB2 */
      if(TDSRRCCHECK_SIBSb_TypeAndTag((&sb_ptr->elem[count].sibSb_Type),sysInfoTypeSB2))
      {
        /* We found it */
        sb2_found = TRUE;
      }
      /* Now increment the pointer */
      n_value--;
      count++;
    }

    if( (n_value != 0) && (sib7_found == TRUE) )
    {
      sib7exp = tdsrrcsib_convert_sib7_rep_time_to_ms(&sb_ptr->elem[count].scheduling, exp_factor);
    }
  }/* end if MIB is valid */
  else
  {
    /* Can't find MIB! */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB not present in srv cell!");
    return( TDSRRCSIB_DEFAULT_EXP_TIME_FOR_SIB7 );
  }


  /* If SIB7 was not found in the MIB.  It could be
  in one of the scheduling blocks */
  if( (sib7_found == FALSE) && (sb1_found == TRUE) )
  {
    /* Look in SB1 for SIB7 info */
    if(tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB1, index) != NULL)
    {
      /* If the SB1 is valid */
      ptr= &(tdssibdb.cell[index].sibs.sb1->sib_ReferenceList);
      count = 0;
      n_value = ptr->n;
      while(n_value != 0 )
      {
        /* Check if this is for SIB7 */
        if(TDSRRCGET_SCHD_INFO_TAG((&ptr->elem[count].sib_Type)) == 
            (TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType7)))
        {
          /* We found it */
          sib7_found = TRUE;
          break; /* Out of the while loop */
        }
        /* Now increment the pointer */
        n_value--;
        count++;
      }
      if( (n_value != 0) && (sib7_found == TRUE) )
      {
        sib7exp = tdsrrcsib_convert_sib7_rep_time_to_ms(&ptr->elem[count].scheduling, exp_factor);
      }
    }
  } /* end if sb1_found == TRUE */

  else if( (sib7_found == FALSE) && (sb2_found == TRUE) )
  {
    /* Repeat the same search in Scheduling Block 2 */
    if(tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB2, index) != NULL)
    {
      /* If the SB2 is valid */
      ptr= &(tdssibdb.cell[index].sibs.sb2->sib_ReferenceList);
      count = 0;
      n_value = ptr->n;
      
      while(n_value != 0 )
      {
        /* Check if this is for SIB7 */
        if(TDSRRCGET_SCHD_INFO_TAG((&ptr->elem[count].sib_Type)) == 
            (TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType7)))
        {
          /* We found it */
          sib7_found = TRUE;
          break; /* Out of the while loop */
        }
        /* Now increment the pointer */
        n_value--;
        count++;
      }
      if( (n_value != 0) && (sib7_found == TRUE) )
      {
        sib7exp = tdsrrcsib_convert_sib7_rep_time_to_ms(&ptr->elem[count].scheduling, exp_factor);
      }
    }
  } /* end if sb2_found == TRUE */


  /* Finally, check if sib7 scheduling info
  was found in any of the 3 blocks */
  if( sib7_found == TRUE )
  {
    /* Check the MAX of default expiration time and
    found expiration time */
    if( sib7exp < TDSRRCSIB_DEFAULT_EXP_TIME_FOR_SIB7 )
    {
      sib7exp = TDSRRCSIB_DEFAULT_EXP_TIME_FOR_SIB7;
    }
  }
  else
  {
    /* SIB7 scheduling info was not found. Use
    the default */
    sib7exp = TDSRRCSIB_DEFAULT_EXP_TIME_FOR_SIB7;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB7 expiration timer is: %d",sib7exp);

  return( sib7exp );

}

/*===========================================================================

FUNCTION RRCSIB_FREE_TSIB

DESCRIPTION


DEPENDENCIES
 
  None.

RETURN VALUE


SIDE EFFECTS

  None.
   
===========================================================================*/
static void tdsrrcsib_free_tsib
(
  tdssib_events_index_e_type event_index
)
{

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }
  if(tdssib_events_ptr == NULL)
  {
    return;
  }

  tdssib_events_ptr->event[event_index].tempholder.mib_sfn_prime = 
                             TDSRRCSIB_INVLID_SFN_PRIME;

  /* In case the tempholer pointers are not freed, free them
  now */
  if( tdssib_events_ptr->event[event_index].tempholder.mib != NULL )
  {
    tdsrrcsibdb_free_sib(  tdsrrc_MasterInformationBlock_PDU,
     (void *)tdssib_events_ptr->event[event_index].tempholder.mib );
    tdssib_events_ptr->event[event_index].tempholder.mib = NULL;
  }
  
  if( tdssib_events_ptr->event[event_index].tempholder.sib3 != NULL )
  {
    tdsrrcsibdb_free_sib(  tdsrrc_SysInfoType3_PDU,
     (void *)tdssib_events_ptr->event[event_index].tempholder.sib3 );
    tdssib_events_ptr->event[event_index].tempholder.sib3 = NULL;
  }
}
/*===========================================================================

FUNCTION tdsrrcsib_configure_bcch

DESCRIPTION

  This function sends channel config req for S_BCCH and N_BCCH to setup/teardown 
  or NOOP on one of these channels
DEPENDENCIES
 
  None.

RETURN VALUE

  SUCCESS: If action is successful
  FAILURE: If action fails

SIDE EFFECTS

  None.
   
===========================================================================*/
static uecomdef_status_e_type  tdsrrcsib_configure_bcch
(
  tdsrrcsib_bcch_action_type s_bcch_action,
  tdsrrcsib_bcch_action_type n_bcch_action
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
  /*Add reason for chan_config_req which is used by LLC*/
  ,tdsrrcllc_chan_cfg_reason_e_type chan_config_reason
#endif
)
{
  tdsrrc_cmd_type *bcch_cmd_ptr; /* To send an CHANNEL_CONFIG_REQ */
  uint32 num_rb=0;

  //N_BCCH teardown is not supported, also we don't support setting up of 2 serving
  //PCCPCH and also, 2 NOOP's are not needed to be sent to LLC
  if( ((s_bcch_action == TDSSBCCH_SETUP) && (n_bcch_action == TDSNBCCH_SETUP)) ||
       (n_bcch_action == TDSNBCCH_TEARDOWN) ||
      ((s_bcch_action == TDSSBCCH_NOOP) && (n_bcch_action == TDSNBCCH_NOOP))
    )
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid S_BCCH %d,N_BCCH %d action",s_bcch_action,n_bcch_action);
    return FAILURE;
  }

  if(( bcch_cmd_ptr = tdsrrc_get_int_cmd_buf() ) != NULL )
  {
    bcch_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CHANNEL_CONFIG_REQ;    
    bcch_cmd_ptr->cmd.chan_config_req.procedure = TDSRRC_PROCEDURE_SIB;
    bcch_cmd_ptr->cmd.chan_config_req.num_rb_to_config = 0;
    bcch_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = FALSE;
    bcch_cmd_ptr->cmd.chan_config_req.rrc_state_change_required = FALSE;
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
    /*Add reason for chan_config_req which is used by LLC*/
    bcch_cmd_ptr->cmd.chan_config_req.chan_config_reason  =  chan_config_reason;
#endif
    //if a setup is being done, request a transaction id, else we don't wait for
    //a cnf.. no transaction id is needed
    if(n_bcch_action == TDSNBCCH_SETUP || s_bcch_action == TDSSBCCH_SETUP)
    {
      bcch_cmd_ptr->cmd.chan_config_req.transaction_id = TDSGET_TRANS_ID();
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Trans Id for chan_cfg_req setup %d",bcch_cmd_ptr->cmd.chan_config_req.transaction_id);
    }
  }
  else
  {
    ERR_FATAL("No-MEM for CHAN_CONFIG_REQ",0,0,0);
  }
  
  if(n_bcch_action == TDSNBCCH_SETUP)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting up N-BCCH");

    tdssib_events_ptr->event[TDSSIB_NGH_INDEX].transaction_id = 
      (uint8)bcch_cmd_ptr->cmd.chan_config_req.transaction_id;

    /* The first index is used for the N-BCCH */
    bcch_cmd_ptr->cmd.chan_config_req.num_rb_to_config++;
    bcch_cmd_ptr->cmd.chan_config_req.rb[num_rb].rb_id = BCCH_N_RADIO_BEARER_ID;
    bcch_cmd_ptr->cmd.chan_config_req.rb[num_rb].rb_config = TDSESTABLISH_RB;
    bcch_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

    //set action to PENDING until channel-config-cnf is received
    tdssib_events_ptr->ngh_bcch.bcch_action = TDSSIB_BCCH_SETUP_PENDING;
    num_rb++;

#if  defined(FEATURE_TDSCDMA_DRX_MANAGER)
    /* Initialize SIB Sleep schedule table */
    tdsrrcsib_initialize_sib_sleep_schedule_table();
#endif 
  }

  if(s_bcch_action == TDSSBCCH_TEARDOWN)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Tearing down S-BCCH");
    
    bcch_cmd_ptr->cmd.chan_config_req.num_rb_to_config++;
    bcch_cmd_ptr->cmd.chan_config_req.rb[num_rb].rb_id = BCCH_S_RADIO_BEARER_ID;
    bcch_cmd_ptr->cmd.chan_config_req.rb[num_rb].rb_config = TDSRELEASE_RB;

    //set action to NO_ACTION as teardown is not required
    tdssib_events_ptr->srv_bcch.bcch_action = TDSSIB_BCCH_NO_ACTION;    
    num_rb++;
  }
  else if(s_bcch_action == TDSSBCCH_SETUP)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting up S-BCCH");

#if defined(FEATURE_TDSCDMA_DRX_MANAGER)
    /* Initialize SIB Sleep schedule table */
    tdsrrcsib_initialize_sib_sleep_schedule_table();
#endif
    
    tdssib_events_ptr->event[TDSSIB_SRV_INDEX].transaction_id = 
      (uint8)bcch_cmd_ptr->cmd.chan_config_req.transaction_id;

    bcch_cmd_ptr->cmd.chan_config_req.num_rb_to_config++;
    bcch_cmd_ptr->cmd.chan_config_req.rb[num_rb].rb_id = BCCH_S_RADIO_BEARER_ID;
    bcch_cmd_ptr->cmd.chan_config_req.rb[num_rb].rb_config = TDSESTABLISH_RB;
    bcch_cmd_ptr->cmd.chan_config_req.rrc_channel_config_cnf_required = TRUE;

    //set action to PENDING until channel-config-cnf is received
    tdssib_events_ptr->srv_bcch.bcch_action = TDSSIB_BCCH_SETUP_PENDING;
    num_rb++;
  }

  tdsrrc_put_int_cmd( bcch_cmd_ptr );

  return SUCCESS;
}
/*===========================================================================

FUNCTION RRCSIB_SEND_COMMAND_FOR_PENDING_UPDATE_SIBS_EVENT

DESCRIPTION

  This function sends a command to the SIB procedure itself
  to update SIBs based on value tags in the serving cell. This
  is only used when a earlier update_sibs command is pending to be
  processed.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
   
===========================================================================*/
static void tdsrrcsib_send_command_for_pending_update_sibs_event( void )
{
  tdsrrc_cmd_type *ptr; /* To send an UPDATE_SIBS_IND cmd if needed */

  /* Post a command to ourselves for updating the SIBs */
  if( (ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters for TDSRRC_UPDATE_SIBS_IND */
    ptr->cmd_hdr.cmd_id = TDSRRC_UPDATE_SIBS_IND;
         
    /* MIB value tag should be as before */
    ptr->cmd.update_sibs_ind.mib_value_tag = 
      tdssib_events_ptr->mib_value_tag_from_command;


    ptr->cmd.update_sibs_ind.cause = tdssib_events_ptr->update_sibs_cause;
    /* Procedure name */
    ptr->cmd.update_sibs_ind.procedure = tdssib_events_ptr->update_sibs_req_proc;
    tdsrrc_put_int_cmd(ptr);   
  } 
  else
  {
    ERR_FATAL("No Memory for TDSRRC_UPDATE_SIBS_IND cmd",0,0,0); 
  }
}/* end function tdsrrcsib_send_command_for_pending_update_sibs_event */
      
/*===========================================================================

FUNCTION RRCSIB_SEND_COMMAND_FOR_SAVED_UPDATE_SIBS_EVENT

DESCRIPTION

  This function sends a command to the SIB procedure itself
  to update SIBs based on value tags in the serving cell. This
  is only used when a earlier update_sibs command is buffered so that it could 
  be processed later.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
   
===========================================================================*/
static void tdsrrcsib_send_command_for_saved_update_sibs_event( void )
{
  tdsrrc_cmd_type *ptr; /* To send an UPDATE_SIBS_IND cmd if needed */

  /* Post a command to ourselves for updating the SIBs */
  if( (ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {     
    /* Fill in the command parameters for TDSRRC_UPDATE_SIBS_IND */
    ptr->cmd_hdr.cmd_id = TDSRRC_UPDATE_SIBS_IND;
         
    /* MIB value tag should be as before */
    ptr->cmd.update_sibs_ind.mib_value_tag = 
      tdsrrcsib_cmd_store.saved_cmd.cmd.update_sibs_ind.mib_value_tag;


    /* Procedure name */
    ptr->cmd.update_sibs_ind.procedure = tdsrrcsib_cmd_store.saved_cmd.cmd.update_sibs_ind.procedure;
    tdsrrc_put_int_cmd(ptr);   
  } 
  else
  {
    ERR_FATAL("No Memory for TDSRRC_UPDATE_SIBS_IND cmd",0,0,0); 
  }
}/* end function tdsrrcsib_send_command_for_pending_update_sibs_event */

/*===========================================================================

FUNCTION RRCSIB_PROCESS_INT_SIB_EVENT_COMPLETE

DESCRIPTION

  This function does the necessary processing when 
  an internal SIB event is completed.
  
  When the SIB procedue itself requests a SIB event
  it's an internal event. Currently internal events
  are used only for updating SIB7 in various conditions.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
   
===========================================================================*/
static void tdsrrcsib_process_int_sib_event_complete
(
  tdssib_events_index_e_type index,        /* Index into event database     */
  uecomdef_status_e_type event_status   /* Success or Failure indication */
)
{
  tdsrrc_cmd_type *ptr; /* To send a command to other procedures if needed */

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }
  
  /* Currently we only process the SIB7 expiration through
  an internal event. Only check for that command here.
  Later other internal events may be used */
  if( (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_SIB) &&
      (tdssib_events_ptr->event[index].int_event.int_event_type == 
       TDSRRCSIB_INT_SIB7_EVENT)
    )
  {
    /* SIB7 update can be based on timer or external procedure request.
    Check for both and process accordingly */
    if(tdssib7info.sib_ext_proc_update_req == TRUE)
    {
      /* Set the value back to FASLE */
      tdssib7info.sib_ext_proc_update_req = FALSE;
      if( (ptr = tdsrrc_get_int_cmd_buf())!=NULL )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_SIB7_UPDATE_STATUS_IND");
        ptr->cmd_hdr.cmd_id = TDSRRC_SIB7_UPDATE_STATUS_IND;
        ptr->cmd.sib7_update_status.status = event_status;
        ptr->cmd.sib7_update_status.procedure = tdssib7info.req_proc;
        tdsrrc_put_int_cmd( ptr );
      }
      else
      {
        ERR_FATAL("No memory to send cnf event",0,0,0);
      }
    }
    else if(tdssib7info.sib_timer_update_req == TRUE)
    {
      /* Timer based update of SIB7 has been completed. */
      tdssib7info.sib_timer_update_req = FALSE;
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
      if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
      {/*Lets inti int_event_type to max as it will be used by 
          SIB-7 event in FACH*/
        tdssib_events_ptr->event[index].int_event.int_event_type 
           = TDSRRCSIB_INT_MAX_EVENT;
      }
#endif
    }

    /* Check if there is any pending event for updating sibs
    and if so, process that event. */
    if( tdsupdate_sibs_event_pending == TRUE )
    {
      /* Post a command to ourselves for updating the SIBs */
      tdsrrcsib_send_command_for_pending_update_sibs_event();
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pending UPDATE_SIBS event activated");
      tdsupdate_sibs_event_pending = FALSE;
    }
  }
}

/*===========================================================================

FUNCTION TDSRRCSIB_HOLES_OPTIMIZATION_ENHANCED

DESCRIPTION

  This function is to optimize the SIB hole mechanism.

  If a SIB is scheduled but it will not be collected originally because it is in the "160-0" gap.
  But the left or right side of this SIB already has enough gap, we can optimize to collect it.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  uint32 indicating the count of optimization 

SIDE EFFECTS

  None.
   
===========================================================================*/

static uint32 tdsrrcsib_holes_optimization_enhanced(uint8 *table_ptr, uint8 *table_mask, uint32 sleep_sfn, uint32 wake_sfn)
{
  uint32 i, j, k;
  uint32 leftmost_scheduled_sib_pos, rightmost_scheduled_sib_pos, temp_pos;
  uint32 index_jump_max, max_rept, start_pos, end_pos, optimization_count = 0;
  boolean leftmost_scheduled_sib_pos_fixed = FALSE;

  max_rept = tdssib_events_ptr->scheduling_max_rept_rate;

  /* if max repetition is smaller than 32, do not need to optimize it,
  if max repetition is larger than 512, do not optimize it because the basic optimization may access the 5th max repetition, it will cross the boundary.
  And in real network, the max repetition is always not more than 128. */
  if(max_rept < 32 || max_rept > 512)
    return 0;

  /* When operate on "table_mask" and "table_ptr", to avoid wrap around at the boundary, it will process the 2nd max SIB repetition. 
  It should be OK because "table_mask" and "table_ptr" have the same value in every max SIB repetition. */
  start_pos = max_rept * TDSRRCSIB_HOLE_ENHANCED_OPTIMIZATION_REPT_INDEX;
  end_pos = start_pos + max_rept;  

  /* record the max length of successive "1"  for skipping the "160-1" area directly. */
  index_jump_max = tdsrrcsib_pattern_invert ? sleep_sfn : wake_sfn;
  index_jump_max -= 2;

  /* TTI is 20ms and every bit means 10ms, so step of the loop is 2. */
  for(i = start_pos; i < end_pos; i += 2)
  {
    if(table_mask[i] == 0)
    {
      if(i == start_pos + sleep_sfn + wake_sfn)
      {
        /* For avoiding to consider wrap around at right boundary, we may use the value of first 320ms of next max repitition.
        But the value of first 320ms of current max repitition may be modified, so copy it to next repitition. */
        memscpy(table_mask + end_pos, sleep_sfn + wake_sfn, table_mask + start_pos, sleep_sfn + wake_sfn);
      }

      if(TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, i))
      {   
        /*     
        find a SIB which is scheduled in "160-0", call it SIB A.
        define the overlap area between the 160ms-radius circle centered at SIB A and the left "160-1" area from SIB A as A's left neighbor area. 
        Similarly, there is A's right neighbor area.  

        For each SIB A, this SIB can be collected if below conditions are met:
        (a) no SIB scheduled in A's left neighbor area(condition a1), OR the left-most scheduled SIB in A's left neighbor area is preceded with 160ms gap(condition a2);
        (b) no SIB scheduled in A's right neighbor area(condition b1), OR the right-most scheduled SIB in A's right neighbor area is followed by 160ms gap(condition b2);
        (c) the interval between leftmost_scheduled_sib_pos and rightmost_scheduled_sib_pos is not more than 160ms.

        The leftmost_scheduled_sib_pos  and rightmost_scheduled_sib_pos are initialized to the position of SIB A. 
        if SIB A meets condtion a2, leftmost_scheduled_sib_pos is updated to the position of the left-most scheduled SIB in A's left neighbor area.
        if SIB A meets condition b2, rightmost_scheduled_sib_pos is updated to the position of the right-most scheduled SIB in A's right-neighbor area.
        if SIB A is decided to be collected, the value of leftmost_scheduled_sib_pos is fixed and do not need to check condition a1 and a2 while checking remaining SIBs scheduled in the same "160-0" area 
        */
        
        /* step 1:  check whether meet condition (a). if not meet, find next SIB which is scheduled in "160-0". */

        if(leftmost_scheduled_sib_pos_fixed == FALSE)
        {
          leftmost_scheduled_sib_pos = i;

          for(j = i - sleep_sfn; j < i; j += 2)
          {
            if(table_mask[j] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
            {
                /* check whether there is a SIB scheduled in the left neighbor area. */
                leftmost_scheduled_sib_pos = j;
                break;
            }
          }

          if(leftmost_scheduled_sib_pos != i)
          {
            /* there is a SIB scheduled in the left neighbor area. */
            temp_pos = leftmost_scheduled_sib_pos - sleep_sfn;
            
            for(j = temp_pos; j < leftmost_scheduled_sib_pos; j += 2)
            {
              /* check whether the left-most scheduled SIB in the left neighbor area is preceded with 160ms gap*/
              if(table_mask[j] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
                break;
            }

            if(j < leftmost_scheduled_sib_pos)
            {
              /* left-most scheduled SIB in the left neighbor area is not preceded with 160ms gap*/
              continue;
            }
          }
        }
        
        
        /* step 2:  check whether meet condition (b). if not meet, find next SIB which is scheduled in "160-0". */
        
        rightmost_scheduled_sib_pos = i;

        for(j = i + sleep_sfn; j > i; j -= 2)
        {
          if(table_mask[j] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
          {
              /* check whether there is a SIB scheduled in the right neighbor area. */
              rightmost_scheduled_sib_pos = j;
              break;
          }
        }

        if(rightmost_scheduled_sib_pos != i)
        {
          /* there is a SIB scheduled in the right neighbor area. */
          temp_pos = rightmost_scheduled_sib_pos + sleep_sfn;
          
          for(j = rightmost_scheduled_sib_pos + 2; j <= temp_pos; j += 2)
          {
            /* check whether the right-most scheduled SIB in the right neighbor area is followed by 160ms gap. */
            if(table_mask[j] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
              break;
          }

          if(j <= temp_pos)
          {
            /* The right-most scheduled SIB in the right neighbor area is not followed by 160ms gap. 
            If leftmost scheduled SIB's position is fixed, and the remaining SIB scheduled in the same "160-0" area does not meet condition b2,
            go to the next "160-0" area directly. */
            if(leftmost_scheduled_sib_pos_fixed == TRUE)
            {
              for(k = i; k <= rightmost_scheduled_sib_pos; k += 2)
              {
                if(table_mask[k] == 1)
                {
                  if(k == start_pos + sleep_sfn + wake_sfn)
                  {
                    /* For avoiding to consider wrap around at right boundary, we may use the value of first 320ms of next max repitition.
                    But the value of first 320ms of current max repitition may be modified, so copy it to next repitition. */
                    memscpy(table_mask + end_pos, sleep_sfn + wake_sfn, table_mask + start_pos, sleep_sfn + wake_sfn);
                  }
                  i = k + index_jump_max;
                  leftmost_scheduled_sib_pos_fixed = FALSE;
                  break;
                }
              }
            }
            continue;
          }
        }


        /* step 3:  check whether meet condition (c). if meet, set the table_mask to "1", else find next SIB which is scheduled in "160-0". */
        
        if(rightmost_scheduled_sib_pos == leftmost_scheduled_sib_pos)
        {
          /* no SIB scheduled in the left and right neighbor area, set the table_mask to "1" directly */
          table_mask[i] = 1;
          table_mask[i + 1] = 1;
          optimization_count ++;
          leftmost_scheduled_sib_pos_fixed = TRUE;
          /* print the optimizated SIB in the format similar as table_ptr */
          TDSRRC_MSG3(MSG_LEGACY_HIGH, "Enhanced optimization for SIB hole takes effect: byte[%d] = 0x%0X, max_rept = %d", \
                                        (i % max_rept) >> 3, (1 << (7 - (i & 0x07))) | (1 << (7 - ((i + 1) & 0x07))), max_rept);
        }
        else
        {
          /*A SIB scheduled in the left or right neighbor area*/
          if(rightmost_scheduled_sib_pos - leftmost_scheduled_sib_pos < wake_sfn)
          {
            leftmost_scheduled_sib_pos_fixed = TRUE;           
            
            /*meet condition (c), set the the table_mask to "1" for the scheduled SIB between position of current checking SIB and rightmost position */      
            for(j = i; j <= rightmost_scheduled_sib_pos; j += 2)
            {
              if(table_mask[j] == 0 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
              {
                table_mask[j] = 1;
                table_mask[j + 1] = 1;
                optimization_count ++;
                /* print the optimizated SIB in the format similar as table_ptr */
                TDSRRC_MSG3(MSG_LEGACY_HIGH, "Enhanced optimization for SIB hole takes effect: byte[%d] = 0x%0X, max_rept = %d", \
                                              (j % max_rept) >> 3, (1 << (7 - (j & 0x07))) | (1 << (7 - ((j + 1) & 0x07))), max_rept);
              }
              else if(table_mask[j] == 1)
              {
                if(j == start_pos + sleep_sfn + wake_sfn)
                {
                  /* For avoiding to consider wrap around at right boundary, we may use the value of first 320ms of next max repitition.
                  But the value of first 320ms of current max repitition may be modified, so copy it to next repitition. */
                 memscpy(table_mask + end_pos, sleep_sfn + wake_sfn, table_mask + start_pos, sleep_sfn + wake_sfn);
                }
                /* to reduce the count of loop,
                if rightmost_scheduled_sib_pos is not in current "160-0" area, go to the next "160-0" area directly*/
                i = j + index_jump_max;
                leftmost_scheduled_sib_pos_fixed = FALSE;
                break;
              }
            }
          }
        }
      }
    }
    else
    {
      if(i == start_pos + sleep_sfn + wake_sfn)
      {
        /* For avoiding to consider wrap around at right boundary, we may use the value of first 320ms of next max repitition.
        But the value of first 320ms of current max repitition may be modified, so copy it to next repitition. */
        memscpy(table_mask + end_pos, sleep_sfn + wake_sfn, table_mask + start_pos, sleep_sfn + wake_sfn);
      }
      /* to reduce the count of loop, go to the next "160-0" area directly */
      i += index_jump_max; 
      leftmost_scheduled_sib_pos_fixed = FALSE;
    }
  }

  return optimization_count;
}

/*===========================================================================

FUNCTION TDSRRCSIB_HOLES_OPTIMIZATION_BASIC

DESCRIPTION

  This function is to optimize the SIB hole mechanism.

  If a SIB is scheduled but it will not be collected originally because it is in the "160-0" gap.
  But the left and right side of this SIB already has enough gap, we can optimize to collect it.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  uint32 indicating the count of optimization 

SIDE EFFECTS

  None.
   
===========================================================================*/
static uint32 tdsrrcsib_holes_optimization_basic(uint8 *table_ptr, uint8 *table_mask, uint32 sleep_sfn, uint32 wake_sfn)
{
  uint32 i, j, k;
  uint32 index_jump_max, max_rept, start_pos, end_pos, optimization_count = 0;
  boolean already_collect_one_sib = FALSE;

  max_rept = tdssib_events_ptr->scheduling_max_rept_rate;

  /* if max repetition is smaller than 32, do not need to optimize it,
  if max repetition is larger than 512, do not optimize it because the basic optimization may access the 5th max repetition, it will cross the boundary.
  And in real network, the max repetition is always not more than 128. */
  if(max_rept < 32 || max_rept > 512)
    return 0;

  /* When operate on "table_mask" and "table_ptr", to avoid wrap around at the boundary, it will process the 5th max SIB repetition.
  Table_mask of the 3rd max SIB repetition may be modified by enhanced optimization, so let us use the 5th one.
  It should be OK because "table_mask" and "table_ptr" have the same value in every max SIB repetition. */  
  start_pos = max_rept * TDSRRCSIB_HOLE_BASIC_OPTIMIZATION_REPT_INDEX;
  end_pos = start_pos + max_rept;
 
  /* record the max length of successive "1"  for skipping the "160-1" area directly*/
  index_jump_max = tdsrrcsib_pattern_invert ? sleep_sfn : wake_sfn;
  index_jump_max -= 2; 

  /* TTI is 20ms and every bit means 10ms, so step of the loop is 2 */
  for(i = start_pos; i < end_pos; i += 2)
  {
    if(table_mask[i] == 0)
    {
      if(TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, i))
      {   
        /*     
        find a SIB which is scheduled in "160-0", call it SIB A.
        define the overlap area between the 160ms-radius circle centered at SIB A and the left "160-1" area from SIB A as A's left neighbor area. 
        Similarly, there is A's right neighbor area.  

        For each SIB A, this SIB can be collected if below conditions are met:
        (a) no SIB scheduled in A's left neighbor area;
        (b) no SIB scheduled in A's right neighbor area;

        if SIB A is decided to be collected, do not need to check condition (a) while checking remaining SIBs scheduled in the same "160-0" area 
        */

        
        /* check whether there is a SIB scheduled in the left neighbor area. */
        if(already_collect_one_sib == FALSE)
        { 
          for(j = i - sleep_sfn; j < i; j += 2)
          {
            if(table_mask[j] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
            {
                break;
            }
          }

          if(j < i)
            continue;
        }

        
        /* check whether there is a SIB scheduled in the right neighbor area. */
        for(j = i + 2; j <= i + sleep_sfn; j += 2)
        {
          if(table_mask[j] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
          {
              break;
          }
        }

        if(j <= i + sleep_sfn)
        {
          /* There is a SIB scheduled in the right neighbor area,
          do not need to check the remaining SIB in current "160-0" area.*/
          for(k = i + 2; k <= i + sleep_sfn; k += 2)
          {
            if(table_mask[k] == 1)
            {
              break;
            } 
          }
          
          i = k + index_jump_max;
          continue;
        }

        table_mask[i] = 1;
        table_mask[i + 1] = 1;
        already_collect_one_sib = TRUE;
        optimization_count ++;
        /* print the optimizated SIB in the format similar as table_ptr */
        TDSRRC_MSG3(MSG_LEGACY_HIGH, "Basic optimization for SIB hole takes effect: byte[%d] = 0x%0X, max_rept = %d", \
                                      (i % max_rept) >> 3, (1 << (7 - (i & 0x07))) | (1 << (7 - ((i + 1) & 0x07))), max_rept);
      }
    }
    else
    {
      /* to reduce the count of loop, go to the next "160-0" area directly*/
      i += index_jump_max; 
      already_collect_one_sib = FALSE;
    }
  }

  return optimization_count;
}

/*===========================================================================

FUNCTION TDSRRCSIB_CHECK_HOLES_OPTIMIZATION

DESCRIPTION

  This function is to verify whether the SIB hole optimization obeys the two rules below.
  1. the max time for collecting SIB successively is not more than wake_sfn*10ms(or 160ms).
  2. the max gap for not collecting SIB is not less than sleep_sfn*10ms(or 160ms). 
  
DEPENDENCIES
 
  None.

RETURN VALUE

  Boolean TRUE if the optimization obeys the rules, FALSE otherwise.

SIDE EFFECTS

  None.
   
===========================================================================*/
static boolean tdsrrcsib_check_holes_optimization(uint8 *table_ptr, uint8 *table_mask, uint32 sleep_sfn, uint32 wake_sfn)
{
  uint32 i, j, k;
  uint32 right_radius_edge_pos, rightmost_scheduled_sib_pos, temp_pos;
  uint32 max_rept = tdssib_events_ptr->scheduling_max_rept_rate;
  uint32 bytes_of_one_rept = max_rept >> 3; 

  for(i = 0; i < max_rept; i += 2)
  {
    if(table_mask[i] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, i))
    {
      rightmost_scheduled_sib_pos = i;
      right_radius_edge_pos = i + wake_sfn - 2;

      /* get the rightmost SIB scheduled in 160ms */
      for(j = right_radius_edge_pos; j > i; j -= 2)
      {
        if(table_mask[j] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
        {
            rightmost_scheduled_sib_pos = j;
            break;
        }
      }

      /* check whether the rightmost SIB scheduled is followed by 160ms gap */
      temp_pos = rightmost_scheduled_sib_pos + sleep_sfn;
      for(j = rightmost_scheduled_sib_pos + 2; j <= temp_pos; j += 2)
      {
        if(table_mask[j] == 1 && TDSRRCSIB_GET_TABLE_BIT_VALUE(table_ptr, j))
        {
          for(k = 0; k < bytes_of_one_rept; k ++)
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH, "Some wrong happens in SIB hole optimization, table_ptr[%d] = 0x%0X, tdsrrcsib_pattern_invert = %d", \
                                          k, table_ptr[k], tdsrrcsib_pattern_invert);
          }
          TDSRRC_MSG3(MSG_LEGACY_HIGH, "start position: %d, end position: %d, fail postion: %d", \
                                        i, rightmost_scheduled_sib_pos, j);
          return FALSE;
        }
      }
      i = temp_pos;
      
    }
  }

  return TRUE;
}

/*===========================================================================

FUNCTION TDSRRCSIB_INTRO_HOLES_IN_TABLE

DESCRIPTION

  This function sends the SIB sleep table to L1 so that
  L1 will sleep for a while before receiving the SIBs.

  This function will be called when SIB event is completed in both success 
  and failure cases and also when a SIB is received to send the updated 
  sleep table.

DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
   
===========================================================================*/
static void tdsrrcsib_intro_holes_in_table(uint8 *table_ptr, uint32 sleep_sfn, uint32 wake_sfn)
{
  uint8 table_mask[TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_MASK_SIZE], sum;
  uint32 i = 0, j, k;
  uint32 enhanced_optimization_count = 0, basic_optimization_count = 0;
  uint32 memcpy_count, rept_index, max_rept = tdssib_events_ptr->scheduling_max_rept_rate;
  
  if(tdsrrcrce_is_cs_call_pending() == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_G2T: Doesn't introduce hole as CS call is pending");
    return;
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC_G2T: sib pattern is %d",tdsrrcsib_pattern_invert);

  /* Generate a table with sleep_sfn wake_sfn pattern */
  while(i < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_MASK_SIZE)
  {
    for(j = i; ((j < (i+sleep_sfn)) && (j < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_MASK_SIZE)); j++)
    {
      table_mask[j] = tdsrrcsib_pattern_invert;
    }

    for(k = j; ((k < (j+wake_sfn)) && (k < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_MASK_SIZE)); k++)
    {
      table_mask[k] = !tdsrrcsib_pattern_invert;
    }
    i = k;
  }


  enhanced_optimization_count = tdsrrcsib_holes_optimization_enhanced(table_ptr, table_mask, sleep_sfn, wake_sfn);
  basic_optimization_count = tdsrrcsib_holes_optimization_basic(table_ptr, table_mask, sleep_sfn, wake_sfn);

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Optimization for SIB hole, enhanced solution: %d, basic solution: %d, max_rept = %d", 
                              enhanced_optimization_count, basic_optimization_count, tdssib_events_ptr->scheduling_max_rept_rate);

  /* check whether optimization is better, adopt the better one. */
  if(enhanced_optimization_count != 0 || basic_optimization_count != 0)
  {    
    if(enhanced_optimization_count > basic_optimization_count)
    {
      /* adopt enhanced optimization */
      rept_index = TDSRRCSIB_HOLE_ENHANCED_OPTIMIZATION_REPT_INDEX;
    }
    else
    {
      /* adopt basic optimization */
      rept_index = TDSRRCSIB_HOLE_BASIC_OPTIMIZATION_REPT_INDEX;
    }

    /* the table_mask should already be modified in the optimization function, copy it to other repetition. */
    memcpy_count = TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_MASK_SIZE / max_rept;
    for(i = 0; i < memcpy_count ; i ++)
    {
      if(i != rept_index)
        memscpy(table_mask + (i * max_rept), max_rept, table_mask + (rept_index * max_rept), max_rept);
    }

    /* self-checking whether the optimization is OK. If not, restore the table_mask to original value before optimization*/
    if(tdsrrcsib_check_holes_optimization(table_ptr, table_mask, sleep_sfn, wake_sfn) == FALSE)
    {
      tdsrrcsib_hole_opt_fail_count ++;
      TDSRRC_MSG1(MSG_LEGACY_HIGH, "Some wrong happens in SIB hole optimization, cancel it. Total failure count: %d", tdsrrcsib_hole_opt_fail_count);

      for(i = 1; i < memcpy_count ; i ++)
      {
        memscpy(table_mask + (i * max_rept), max_rept, table_mask, max_rept);
      }
    }
  }

  /* and the generated table with table to be masked */
  for(i = 0; i < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE; i++)
  {
    sum = 0;
    k = 128;
    for(j = (i*8); j < ((i*8)+8); j++)
    {
      sum = sum + (table_mask[j] * k);
      k = k/2;
    }
    table_ptr[i] &= sum;
  }

}

/*===========================================================================

FUNCTION  tdsrrcsib_delete_sib_schedule_update_from_int_cmd_queue

DESCRIPTION

  This function deletes pending TDSL1_CPHY_SIB_SCHED_CHANGE_IND commands from the int cmd queue. Should
  be at most one pending.
    
DEPENDENCIES

  None.
 
RETURN VALUE

  Boolean TRUE if deletion has been done, FALSE otherwise.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcsib_delete_sib_schedule_update_from_int_cmd_queue(void)
{
   tdsrrc_cmd_type *cmd_ptr;
   tdsrrc_cmd_type *next_cmd_ptr;

   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete pending SIB_SCHED_CHANGE_IND,current work table: %d",tdssib_sleep_schedule_table_in_use);

   cmd_ptr = (tdsrrc_cmd_type *)q_check (&tdsrrc_int_cmd_q);
   while(cmd_ptr != NULL)
   {
     next_cmd_ptr = (tdsrrc_cmd_type *)q_next(&tdsrrc_int_cmd_q,&cmd_ptr->cmd_hdr.link);
     if ((TDSRRC_LLC_CMD_REQ == cmd_ptr->cmd_hdr.cmd_id) &&
	 	 (TDSRRC_PROCEDURE_SIB == cmd_ptr->cmd.rrc_llc_req.procedure) &&
	 	 (TDSLAYER1_PHY == cmd_ptr->cmd.rrc_llc_req.cmd_dest) &&
         (TDSL1_CPHY_SIB_SCHED_CHANGE_IND == cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id))
	 {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"DSIM: Delete pending SIB_SCHED_CHANGE_IND from int queue");
#ifdef FEATURE_Q_NO_SELF_QPTR
       q_delete(&tdsrrc_int_cmd_q, &cmd_ptr->cmd_hdr.link );
#else
       q_delete( &cmd_ptr->cmd_hdr.link );
#endif
       tdsrrc_free_cmd_buf(cmd_ptr);

	   if(tdssib_sleep_schedule_table_in_use == 0)
	   {
		  tdssib_sleep_schedule_table_in_use = 1;
	   }
	   else
	   {
		  tdssib_sleep_schedule_table_in_use = 0;
	   }
     }
     cmd_ptr = next_cmd_ptr;
   }
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Delete done,current work table: %d",tdssib_sleep_schedule_table_in_use);
}

/*===========================================================================

FUNCTION RRCSIB_POST_PROCESS_FAILED_EVENT

DESCRIPTION

  This function does the necessary processing for
  a failed SIB event. It sends the failure command
  to the calling procedure if requested, and clears 
  the necessary variables.
  
  In case this is a neighbor BCCH event, it also
  tears down the neighbor BCCH pipe if requested
  by the calling function.
  
  In case of serving BCCH events required by other
  procedures, the BCCH is not torn down by
  this module since the PCCPCH needs to be up for
  timing purposes till the SCCPCH is setup.
  However for events requested by this procedure
  itself and for TDSRRCSIB_UPDATE_SIBS event, this
  function tears down the BCCH. This is done only
  if requested by the calling procedure. 

DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
   
===========================================================================*/
static void tdsrrcsib_post_process_failed_event
( 
  tdssib_events_index_e_type event_index, /* Index of the event that failed */
  boolean cnf_cmd_required, /* Indicate if a confirmation command is
                            to be sent.                             */
  tdsrrc_sib_status_e_type failure_cause, /* Cause of failure. Only needed
                                         if CNF command is to be sent. */
  boolean sbcch_teardown_required   /* If the event is for Serving BCCH
                                       indicate if the serving BCCH pipe should be
                                       brought down. */
)
{
  tdsrrc_cmd_type *cnf_ptr;      /* For sending confirmation */
  uint32  dbindex;
  tdsrrc_state_e_type rrc_state;
  rrc_plmn_identity_type plmn;
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
  /*Variable used to hold config reason*/
  tdsrrcllc_chan_cfg_reason_e_type chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_NONE;
#endif
  tdsrrc_cmd_type *int_cmd_ptr = NULL; /* For cell update/cell selection if needed */

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  /* Copy the PLMN id */
  if((tdssib_events_ptr->event[event_index].tempholder.mib != NULL)&&
     ( TDSRRC_CHECK_COMMON_MSG_TYPE(tdssib_events_ptr->event[event_index].tempholder.mib->plmn_Type, 
                rrc_PLMN_Type_gsm_MAP )))
  {
    tdsrrc_translate_gsm_map_plmn_id
                 (&(tdssib_events_ptr->event[event_index].tempholder.mib->plmn_Type.u.gsm_MAP->plmn_Identity), &plmn);
  }
  else
  {
    plmn.mcc[0] = plmn.mcc[1] = plmn.mcc[2] =0;
    plmn.mnc[0] = plmn.mnc[1] = plmn.mnc[2] =0;
    plmn.num_mnc_digits = 0;
  }

#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
  if(tdsrrcsib_is_current_event_sib_read_in_dch() == TRUE)
  {
    /*If the SIB event active was cell id reading in DCH then set chan 
     *config reason to TDSRRCLLC_CHAN_CFG_REASON_SIB_READ_IN_DCH 
     *which is used by LLC
     */
    chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_SIB_READ_IN_DCH;
  }
#endif

  /* Make the event inactive */
  tdssib_events_ptr->event[event_index].event_active = FALSE;

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
  if(tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB_WAIT_TIMER) == (uint64)0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib7:SIB WAIT TIMER is not running");
  }
  else
#endif
  {
    /* Stop the SIB wait timer in case it's running. */
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_STOP_PARTITION_TIMER );
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_MASK_INVERT_TIMER );
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_EMPTY_MASK_TIMER );
  }

  tdsrrctmr_stop_timer( TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER );

  if (((tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
      (tdssib_events_ptr->event[event_index].req_proc == TDSRRC_PROCEDURE_CSP))|| 
      ((tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_ALL_SIBS) && 
      (tdssib_events_ptr->event[event_index].req_proc == TDSRRC_PROCEDURE_CCM))) 
  {
    if (tdssib_events_ptr->event[event_index].sib_db_index_found == TRUE)
    {
      dbindex = tdssib_events_ptr->event[event_index].sib_db_index;
      
     if((dbindex != TDSSIBDB_INVALID_INDEX)&&
        (tdssibdb.active_cell_sibs_valid == TRUE)&&
        (dbindex == tdssibdb.active_cell_index))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Since dbindex = %d is active_cell_index,skip clear of db idx",dbindex);
      }
      else if(dbindex != TDSSIBDB_INVALID_INDEX)
      {
        // If an index has already been found then clear allthe sibs collected so far
        // and clear other variables associated with this index
          tdssibdb.cell[dbindex].index_used = FALSE; /* This index is now used */
          if(tdssibdb.num_stored_cells)
          {
            tdssibdb.num_stored_cells--;
          }
          
          tdsrrcsibdb_adjust_age_index( dbindex ); /* Adjust age index for cells */
          tdsrrcsibdb_cell_header_init(dbindex);
  
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Clearing the sibdb_var's associated with sibdb_index %d",dbindex);
      }
    }
  }
  
  /* In case the tempholer pointers are not freed, free them
  now */
  tdsrrcsib_free_tsib(event_index);
  
  /* Depending on the event a confirmation command
  may need to be sent. Do this only if the calling
  function requests it. */
  switch( tdssib_events_ptr->event[event_index].event_name )
  { 
    case TDSRRCSIB_GET_ALL_SIBS:

      /* If the GET_ALL_SIBS command was
      requested by an external procedure we have
      to send a GET_ALL_SIBS_CNF. If it was
      requested by the SIB procedure itself, we
      have to process it separately. */
      if( tdssib_events_ptr->event[event_index].req_proc
          != TDSRRC_PROCEDURE_SIB )
      {
        if( cnf_cmd_required == TRUE )
        {
          if( (cnf_ptr = tdsrrc_get_int_cmd_buf())!=NULL )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending GET_ALL_SIBS_CNF(failed)");
            cnf_ptr->cmd_hdr.cmd_id = TDSRRC_GET_ALL_SIBS_CNF;
            cnf_ptr->cmd.get_sibs_cnf.status = failure_cause;
            cnf_ptr->cmd.get_sibs_cnf.procedure =
            tdssib_events_ptr->event[event_index].req_proc;
            /* Copy the frequency and Scrambling code into the
            confirmation command. For GET_ALL_SIBS command, it's
            always the serving cell. */
            cnf_ptr->cmd.get_sibs_cnf.freq = tdssib_events_ptr->srv_bcch.freq;
            cnf_ptr->cmd.get_sibs_cnf.cpid = tdssib_events_ptr->srv_bcch.cpid;
            tdsrrc_put_int_cmd( cnf_ptr );
          }
          else
          {
            ERR_FATAL("No memory to send cnf event",0,0,0);
          }
        }
      }
      else
      {
        tdsrrcsib_process_int_sib_event_complete( event_index, FAILURE );
      }
      break;


    case TDSRRCSIB_GET_SPECIFIC_SIBS:

      /* If the GET_SPECIFIC_SIBS command was
      requested by an external procedure we have
      to send a GET_SPECIFIC_SIBS_CNF. If it was
      requested by the SIB procedure itself, we
      have to process it separately. */
      if( tdssib_events_ptr->event[event_index].req_proc
          != TDSRRC_PROCEDURE_SIB )
      {
        if( cnf_cmd_required == TRUE )
        {
          if( (cnf_ptr = tdsrrc_get_int_cmd_buf())!=NULL )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending GET_SPECIFIC_SIBS_CNF(failed)");
            cnf_ptr->cmd_hdr.cmd_id = TDSRRC_GET_SPECIFIC_SIBS_CNF;
            cnf_ptr->cmd.get_sibs_cnf.status = failure_cause;
            cnf_ptr->cmd.get_sibs_cnf.procedure = 
              tdssib_events_ptr->event[event_index].req_proc;
            /*Boolean to indicate whether SIB read over OTA or through the 
                      SYS_INFO_CONTAINER*/
#ifdef FEATURE_3GPP_CSFB_TDS
            if(tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_READ_FROM_SIB_CONTAINER)
            {
              cnf_ptr->cmd.get_sibs_cnf.is_sib_read_from_container = TRUE;
            }
            else
            {
              cnf_ptr->cmd.get_sibs_cnf.is_sib_read_from_container = FALSE;
            }
#endif            
            /* Copy the frequency and Scrambling code into the
            confirmation command. For GET_SPECIFIC_SIBS command, 
            check if it's the serving cell or neigbor cell. */
            if(tdssib_events_ptr->event[event_index].for_serving_cell == TRUE)
            {
              cnf_ptr->cmd.get_sibs_cnf.freq = tdssib_events_ptr->srv_bcch.freq;
              cnf_ptr->cmd.get_sibs_cnf.cpid = tdssib_events_ptr->srv_bcch.cpid;

              if(failure_cause == TDSSIB_EVENT_FAILED_FPLMN)
              {
                cnf_ptr->cmd.get_sibs_cnf.plmn_id = plmn;
                cnf_ptr->cmd.get_sibs_cnf.cell_id = 0;
              }
            }
            else
            {
              cnf_ptr->cmd.get_sibs_cnf.freq = tdssib_events_ptr->ngh_bcch.freq;
              cnf_ptr->cmd.get_sibs_cnf.cpid = tdssib_events_ptr->ngh_bcch.cpid;
            }

            /* delete pending SIB_SCHED_CHANGE_IND in int queue since we are sending SIB_CNF, to avoid race conditions */
            tdsrrcsib_delete_sib_schedule_update_from_int_cmd_queue();
            tdsrrc_put_int_cmd( cnf_ptr );
          }
          else
          {
            ERR_FATAL("No memory to send cnf event",0,0,0);
          }
        }
      }
      else
      {
        tdsrrcsib_process_int_sib_event_complete( event_index, SUCCESS );
      }
      break;

    case TDSRRCSIB_UPDATE_SIBS:

      /* Clear the saved SIB change indications, if any.
       This is only used by UPDATE_SIBS events. */
      tdsrrcsibdb_clear_active_cell_sib_change();

      /* If the UPDATE_SIBS command was
      requested by an external procedure there
      is no confirmation required. If it was
      requested by the SIB procedure itself, we
      have to process it separately. */
      if( tdssib_events_ptr->event[event_index].req_proc
          != TDSRRC_PROCEDURE_SIB )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_UPDATE_SIBS_IND failed");
      }
      else
      {
        tdsrrcsib_process_int_sib_event_complete
          ( event_index, FAILURE );
      }

      /* Failure handling for Update SIBS Event:
         If we had failed to read SIBS during Update SIBS ind,we need to trigger
         cell selection/Out of service area to read the SIBS again.Using the
         stale sibs is not good */    
      if(((TDSSIB_EVENT_FAILED_LOWER_LAYERS == failure_cause) ||
          (TDSSIB_EVENT_FAILED_TIMEOUT == failure_cause))
//         &&(tdssib_events_ptr->update_sibs_real_sib_mod == TRUE)
        )
      {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC Update SIBS event failed,failure cause:%d.Failure handling kicking in",failure_cause);
         rrc_state = tdsrrc_get_state();
         switch(rrc_state)
         {
           case TDSRRC_STATE_CELL_PCH:
           case TDSRRC_STATE_URA_PCH:
           case TDSRRC_STATE_CELL_FACH:
             /* Trigger Out Of Service area */
             if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
             { 
               int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
               int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_REENTERED_SERVICE_AREA;
               int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = FALSE;
               int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_SIB;
               tdsrrc_put_int_cmd(int_cmd_ptr); 
               TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending Cell Update Initiate Req,cause Re-entered Service Area,rrc_state:%d",rrc_state);
             }
             else
             {
               TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to allocate memory for cmd TDSRRC_CELL_UPDATE_INITIATE_REQ");
             }
             break;

           case TDSRRC_STATE_DISCONNECTED:
             /* Trigger Cell Selection */
             if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
             { 
               int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INITIATE_CELL_SELECTION_IND;
               int_cmd_ptr->cmd.initiate_cell_sel_ind.cause = TDSRRC_OUT_OF_SERVICE_AREA_IND;
               int_cmd_ptr->cmd.initiate_cell_sel_ind.proc = TDSRRC_PROCEDURE_SIB;
               int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state = rrc_state;
               int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present = FALSE;
               int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present  = FALSE;
               tdsrrc_put_int_cmd(int_cmd_ptr); 
               TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending Initiate Cell Selection Ind to CSP,cause Out Of Service Ind.rrc_state:%d",rrc_state);
             }
             else
             {
               TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to allocate memory for cmd TDSRRC_INITIATE_CELL_SELECTION_IND");
             }
             break;

           default:
             TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid state,no action taken on S-BCCH failure");
             break;
         }
      }
      break;

    default:

      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown SIB event %d",tdssib_events_ptr->event[event_index].event_name);
      break;
  }/* End switch */


  /* Check if a S-BCCH teardown is required. S-BCCH is torn down
  for a TDSRRCSIB_UPDATE_SIBS command and for any command initiated
  by the SIB procedure itself or request was GET_SPECIFIC_SIBS for serving
  cell*/
  if( (
       (tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_UPDATE_SIBS) ||
       (tdssib_events_ptr->event[event_index].req_proc == TDSRRC_PROCEDURE_SIB)
      )       
      ||      
      (
       ( tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS ) &&
       ( tdssib_events_ptr->event[event_index].for_serving_cell == TRUE )
      )
    )
  {
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
    if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)||
       (TDSOC_SET_FOR_CELL_FACH == tdsrrcllc_get_ordered_config_status())
      )
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"sib7:RRC state is FACH,Dont tear down S-BCCH, for event =%d,req proc=%d",tdssib_events_ptr->event[event_index].event_name,tdssib_events_ptr->event[event_index].req_proc);
    }
    else
#endif
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
    if(((tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH)||(tdsrrc_get_state() == TDSRRC_STATE_URA_PCH))&&
       (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)&&
       ((tdsrrcllc_get_ordered_config_status() != TDSOC_SET_FOR_FACH_CELL_PCH_TRANS)&&
       (tdsrrcllc_get_ordered_config_status() != TDSOC_SET_FOR_FACH_URA_PCH_TRANS))
       )
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB7 read in PCH, tear down S-BCCH, for event =%d,req proc=%d",tdssib_events_ptr->event[event_index].event_name,tdssib_events_ptr->event[event_index].req_proc);
      /* Check if calling procedure wants the BCCH to be torn down.
            In certain conditions the BCCH would already have been taken 
            down by LLC hence it may not be required. */
      if( sbcch_teardown_required == TRUE )
      {
        //teardown S_BCCH, don't change neighbor, and don't request CNF for 
        //S_BCCH setup
        if(tdsrrcsib_configure_bcch(TDSSBCCH_TEARDOWN, TDSNBCCH_NOOP
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
                                       ,chan_config_reason
#endif
                ) ==  FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib configure bcch failed");
        }
      }
    }
    else
#endif
    {
      /* Check if calling procedure wants the BCCH to be torn down.
      In certain conditions the BCCH would already have been taken 
      down by LLC hence it may not be required. */
      if( sbcch_teardown_required == TRUE )
      {
        //teardown S_BCCH, don't change neighbor, and don't request CNF for 
        //S_BCCH setup
        if(tdsrrcsib_configure_bcch(TDSSBCCH_TEARDOWN, TDSNBCCH_NOOP
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
                                 ,chan_config_reason
#endif
          ) ==  FAILURE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib configure bcch failed");
        }
      }
    }
  }

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
  tdsno_mib_bar_cell_count = 0;
#endif

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  REX_ISR_LOCK(&tdssib7_in_fach_active_mutex);
  if(tdsis_sib7_in_fach_active == TRUE)
  {
    tdsis_sib7_in_fach_active = FALSE;
  }
  REX_ISR_UNLOCK(&tdssib7_in_fach_active_mutex);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB7 in cell fach event active is set to %d",tdsis_sib7_in_fach_active);
#endif
#endif

  //after a failed event, reset BCCH action to false always..
  tdssib_events_ptr->srv_bcch.bcch_action = TDSSIB_BCCH_NO_ACTION;
  tdssib_events_ptr->ngh_bcch.bcch_action = TDSSIB_BCCH_NO_ACTION;
  tdssib_events_ptr->event[event_index].sib_scan_type = TDSRRC_SIB_NORMAL;
}/* End function rrcsib_post_process_sib_failed_event */


  
/*===========================================================================

FUNCTION tdsrrcsib_invalidate_sib_scheduling_info

DESCRIPTION


DEPENDENCIES
 
  None.

RETURN VALUE
 
  None.

SIDE EFFECTS

  None.
        
===========================================================================*/
static void tdsrrcsib_invalidate_sib_scheduling_info
(
  tdssib_events_index_e_type index /* Index in the SIB event database */  
)
{
  uint32 count=0;

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }
  
  //reset the flags that checks to see if all SIB's are received  
  memset (tdssib_events_ptr->sib_sched, FALSE, TDSRRC_TOT_SIBS * sizeof(tdssib_scheduling_info));

  for(count=0; count<TDSRRC_TOT_SIBS; count++)
  {
    tdssib_events_ptr->sib_sched[count].repitition_rate = TDSINVALID_SIB_SCHEDULE_REP;
    tdssib_events_ptr->sib_sched[count].num_segments = 0;
  }

  //reset the flags that checks to see if all SIB's are received  
  memset (tdssib_events_ptr->event[index].sched_present_for_sibs, FALSE, TDSRRC_TOT_SIBS * sizeof(uint8));

  //reset the flag's which keep trakc of some 
  tdssib_events_ptr->scheduling_max_rept_rate = 0;
  tdssib_events_ptr->scheduling_updated_by_sb1 = FALSE;
  tdssib_events_ptr->scheduling_updated_by_sb2 = FALSE;

  //reset flags that indicate whether SB's are needed and decoded yet
  tdssib_events_ptr->event[index].tempholder.sb1_decoded=FALSE;
  tdssib_events_ptr->event[index].tempholder.sb2_decoded=FALSE;
}  
/*===========================================================================

FUNCTION tdsrrcsib_update_pos_offset_in_sched_info

DESCRIPTION
  This function updates the position offset of individual sibs based on
  scheduling information received in MIB/SB1/SB2

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Updates tdssib_events database with scheduling information for eacy sib
        
===========================================================================*/
static void tdsrrcsib_update_pos_offset_in_sched_info
(
  tdsrrc_SIB_type                rrc_sib_type,       //sib type
  tdsrrc_SchedulingInformation * scheduling_info //scheduling information
)
{

  tdsrrc_SibOFF_List *pos_offset_ptr;  //local ptr for looping thru pos_offset
  uint32 count=0;                  //counter purposes
  uint32 index=0, n_value=0;

  if(rrc_sib_type >= tdsrrc_ExtensionType)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Type");
    return ;
  }

  //get scheduling information about rep-rate and initial_segment_offset
  //rep-rate is given in steps of 2, left shift by 1 to multiply by 2 to
  //get the real SFN.. we do comparison later on the real SFN, hence do 
  //the comparison here to make life easy to do a simple comparison later
  switch(TDSRRCGET_SCHD_INFO_TAG((&scheduling_info->scheduling.sib_Pos)))
  {
    case TDSRRCGET_SCHD_INFO_SIB_POS(rep4):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 4;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep4 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep8):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 8;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep8 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep16):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 16;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep16 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep32):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 32;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep32 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep64):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 64;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep64 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep128):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 128;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep128 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep256):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 256;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep256 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep512):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 512;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep512 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep1024):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 1024;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep1024 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep2048):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 2048;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep2048 << 1);
      break;

    case TDSRRCGET_SCHD_INFO_SIB_POS(rep4096):
      tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate = 4096;
      tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] =
        (uint32)(scheduling_info->scheduling.sib_Pos.u.rep4096 << 1);
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid sib-rep-pos info %d",TDSRRCGET_SCHD_INFO_TAG((&scheduling_info->scheduling.sib_Pos)));
      break;
  }

  //update the max rept rate if it is lower than the last received one
  if(tdssib_events_ptr->scheduling_max_rept_rate < tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate)
  {
    tdssib_events_ptr->scheduling_max_rept_rate = tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate;
  }

  //check if segment count is present.. then set it.. if not present it is 1
  //as per spec 25.331
  // bitmask rrc_segCount_present is not present in Objective Systems implementation
  if(scheduling_info->scheduling.segCount > 0)
  {
    tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments = 
      (uint8)scheduling_info->scheduling.segCount;
  }
  else
  {
    tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments = 1;
  }

  //if number of segments is > 1, then we need scheduling info for all the
  //subsequent segments..  this may be indicated in posOffsetInfo or we
  //could use the default offset of 2 for all segments no a multiple of 8..
  //segments which r a multiple of 8 are reserved for mib, or have to be
  //explicitly signalled in posOffsetInfo..
  if(tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments > 1)
  {
    if(TDSRRCCHECK_SIB_MSG_IE_PRESENT((&scheduling_info->scheduling),
                                rrc_,
                                sib_PosOffsetInfo))
    {
      pos_offset_ptr = &(scheduling_info->scheduling.sib_PosOffsetInfo);

      //counter is set to 1, as 0 is deemed to be the first segment..
      //below we are only dealing with subsequent segments
      count = 1;
      index = 0;
      n_value = pos_offset_ptr->n;

      //loop thru the offset ptr and initialize the tdssib_events db
      while(n_value != 0)
      {
        switch(pos_offset_ptr->elem[index])
        {
          case TDSRRCGET_SIB_OFF_VALUE(so2):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 2;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so4):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 4;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so6):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 6;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so8):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 8;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so10):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 10;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so12):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 12;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so14):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 14;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so16):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 16;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so18):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 18;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so20):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 20;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so22):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 22;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so24):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 24;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so26):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 26;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so28):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 28;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so30):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 30;    
            break;
          case TDSRRCGET_SIB_OFF_VALUE(so32):
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 32;    
            break;
          default:
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid Seg-Offset %d",pos_offset_ptr->elem[index]);
            break;
        }
        
        //if the number of segments for which pos-offset is received is > than the
        //number of segments indicated in MIB,  print an error and don't update 
        //offset info..  we should not camp on a cell with bad position-offset count
        if((count+1) > tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"More Pos-Offsets than Segments.  Invalid SIB-Sched");
          break;
        }
        else
        {
          //calculate the segment offset according to the absolute offset from
          //the first segment..  this is done to make life easier during comparison
          //of scheduling info later
          tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] +
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count-1];

          count++;
          index++;
          n_value--;
        }
      }
    }
    else
    {
      //posOffsetInfo is not present.. we will use the default seg-offset of 2
      //for the rest of the segments..  use offset of 4 if calculated offset 
      //is a multiple of 8
      //counter is set to 1, as 0 is deemed to be the first segment..
      //below we are only dealing with subsequent segments
      count = 1;
      
      //loop thru the rest of the segments and set the offset to 2 by default
      for(count=1; (count < tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments) &&(count <= TDSMAX_SEG_COUNT_FOR_A_SIB ); count++)
      {
        if(((tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count-1] + 2) & 0x7) == 0)
        {
          tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count-1] + 4;
        }
        else
        {
          tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count] = 
            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[count-1] + 2;
        }
      }
    }
  }

  TDSRRC_MSG6(MSG_LEGACY_HIGH,"Num. Seg for SIB %d is %d rep_rate %d  SEG-Offsets are %d, %d, %d", 
           tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type), 
           tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments,
           tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate,
           tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[0],
           tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[1],
           tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[2]);
}


/*===========================================================================

FUNCTION tdsrrcsib_check_if_sched_info_match

DESCRIPTION
  This function is used to check whether the SIB segments stored in TSIB database were received in proper offsets
  as per the scheduling information in SB1/SB2 or not

DEPENDENCIES
 
  None.

RETURN VALUE
 
  TDSSEG_PROC_COMPLETE : 
  If all the segments of a SIB has been received before SB1/SB2 and the concatenation was 
  successful

  TDSSEG_PROC_NONE :
  If otherwise

SIDE EFFECTS
  None
===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_check_if_sched_info_match
(
  tdsrrc_SIB_type rrc_sib_type,
  tdssib_events_index_e_type  index
)
{
  uint8 i = 0, j=0;
  uint32 sfn_offset = 0;
  boolean valid_sib = FALSE;
  uint8 num_seg=0;
  TDSseg_proc_stat_e_type status = TDSSEG_PROC_NONE;

  if(rrc_sib_type >= tdsrrc_ExtensionType)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Type");
    return TDSSEG_PROC_FAILED;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Checking TSIB DB for SIB %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
  for(i = 0;i<TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;i++)
  {
    if(tdstsib_optimization_db[i].index_in_use == TRUE && tdstsib_optimization_db[i].sib_type == rrc_sib_type)
    {
      valid_sib = FALSE;
      sfn_offset = tdstsib_optimization_db[i].sfn_prime<<1;
      if(tdstsib_optimization_db[i].seg_index < tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments)
      {
        for(j=0;j<tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments;j++)
        {
          if((sfn_offset % tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate) == 
                            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[j])
          {
            num_seg++;
            valid_sib = TRUE;
            break;
          }
        }
        if(valid_sib == FALSE)
        {
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Seg %d of SIB %d was received at wrong SFN",tdstsib_optimization_db[i].seg_index,tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
          tdstsib_optimization_db[i].index_in_use = FALSE;
          tdstsib_optimization_db[i].seg_data_length = 0;
          tdstsib_optimization_db[i].seg_index = 0;
          tdstsib_optimization_db[i].sfn_prime = 0;
          tdstsib_optimization_db[i].sib_seg_type = tdsnoSegment_chosen;
  
          tdstsib_optimization_db[i].sib_type = tdsrrc_MIB;
          memset(tdstsib_optimization_db[i].seg_data_ptr,0x00, TDSFIXED_SIB_DATA_LENGTH_BYTES);
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Segment Index greater than number of segments");
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Invalid segment index %d for SIB %d. Total number of segments = %d",
                          tdstsib_optimization_db[i].seg_index,tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type)
                          ,tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments);
        tdstsib_optimization_db[i].index_in_use = FALSE;
        tdstsib_optimization_db[i].seg_data_length = 0;
        tdstsib_optimization_db[i].seg_index = 0;
        tdstsib_optimization_db[i].sfn_prime = 0;
        tdstsib_optimization_db[i].sib_seg_type = tdsnoSegment_chosen;

        tdstsib_optimization_db[i].sib_type = tdsrrc_MIB;
        memset(tdstsib_optimization_db[i].seg_data_ptr,0x00, TDSFIXED_SIB_DATA_LENGTH_BYTES);
      }
    }
  }

  if(num_seg <= tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Concatenating segments for SIB # %d received before SB",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
    status = tdsrrcsib_process_sib_seg_in_tsib_optim_db(rrc_sib_type,index);
  }

  return status;
  
}

#if defined (FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined (FEATURE_TDSCDMA_SIB19)
/*===========================================================================

FUNCTION tdsrrcsib_check_if_sched_info_match_for_ext_sibs

DESCRIPTION
  This function is used to check whether the SIB segments stored for extension SIBs 
  in TSIB database were received in proper offsets as per the scheduling information 
  in MIB/SB1/SB2 or not.

DEPENDENCIES
 
  None.

RETURN VALUE
 
  TDSSEG_PROC_COMPLETE : 
  If all the segments of a SIB has been received before SB1/SB2 and the concatenation was 
  successful

  TDSSEG_PROC_NONE :
  If otherwise

SIDE EFFECTS
  None
===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_check_if_sched_info_match_for_ext_sibs
(
  tdsrrc_SIB_type rrc_sib_type,
  tdssib_events_index_e_type  index
)
{
  uint8 i = 0, j=0;
  uint32 sfn_offset = 0;
  uint8 num_seg=0;
  TDSseg_proc_stat_e_type status = TDSSEG_PROC_NONE;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Checking TSIB DB for extension SIB");
  for(i = 0;i < TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;i++)
  {
    if(tdstsib_optimization_db[i].index_in_use == TRUE && tdstsib_optimization_db[i].sib_type == tdsrrc_ExtensionType)
    {
      sfn_offset = tdstsib_optimization_db[i].sfn_prime<<1;
      if(tdstsib_optimization_db[i].seg_index < tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments)
      {
        for(j=0;(j<tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments) && (j < TDSMAX_SEGMENTS_FOR_A_SIB);j++)
        {
          if((sfn_offset % tdssib_events_ptr->sib_sched[rrc_sib_type].repitition_rate) == 
                            tdssib_events_ptr->sib_sched[rrc_sib_type].sfn_offset[j])
          {
            num_seg++;
            tdstsib_optimization_db[i].sib_type = rrc_sib_type;
            break;
          }
        }
      }
    }
  }

  if(num_seg <= tdssib_events_ptr->sib_sched[rrc_sib_type].num_segments)
  {
    tdstsib_opt_db_look_up_table[rrc_sib_type] = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Concatenating segments for extension %d received before SB");
    status = tdsrrcsib_process_sib_seg_in_tsib_optim_db(rrc_sib_type,index);
  }

  return status;
  
}

#endif

/*===========================================================================

FUNCTION tdsrrcsib_send_sib_wait_timer_len_to_l1

DESCRIPTION

  This function sends sib wait timer len to L1.
  L1 may use it to change lock priority.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_send_sib_wait_timer_len_to_l1(uint32 timer_length)
{
  tdsrrc_cmd_type * cmd_ptr = NULL;
  tdsl1_req_cmd * rrc_cmd_l1_req = NULL;

  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_cmd_l1_req);
 
    cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_SIB;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1CPHY_SIB_TIMER_LEN_IND;

    rrc_cmd_l1_req->sib_timer_length_ind.timer_length_ms = timer_length;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDSL1CPHY_SIB_TIMER_LEN_IND sent to L1, time_length_ms: %d", timer_length);
    tdsrrc_put_int_cmd(cmd_ptr);
  }
  else
  {
    ERR_FATAL("Cound not get memory for TDSL1CPHY_SIB_TIMER_LEN_IND",0,0,0);
  }
}

/*===========================================================================

FUNCTION tdsrrcsib_update_sched_info_from_mib_sb

DESCRIPTION
  Updates scheduling information based on MIB/SB1/SB2 received OTA

DEPENDENCIES
 
  None.

RETURN VALUE
 
  None.

SIDE EFFECTS
  Updates the function to update scheduling information, which updates the
  tdssib_events database based on scheduling information received.        
===========================================================================*/
static void tdsrrcsib_update_sched_info_from_mib_sb
(
  uint32 pdu_num,    /* PDU number defined by ASN.1 for this SIB */
  void *sib_ptr,    /* Pointer to the SIB typecast to void.     */
  tdssib_events_index_e_type  event_index
)
{
  tdsrrc_MasterInformationBlock *mib_ptr; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr;         /* Local pointer to SB2       */

  tdsrrc_SIBSb_ReferenceList *sibSb_ptr; /* Local pointer to SIBSb ref list */
  tdsrrc_SIB_ReferenceList *sb_ptr = NULL;      /* Local pointer to SIB ref list   */

  tdsrrcsib_sib_type_vtag sib_type_tag;

  uint32 sb1_rep_rate = 0;             //local vars used to calculate 
  uint32 sb2_rep_rate = 0;             //SIB time during bplmn searches
  uint32 max_rept_rate = 0;
  uint32 index=0;
  boolean  sib1_or_sib3_config_in_un_received_sb = FALSE;
  boolean  all_scheduling_blocks_received = TRUE;
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  /*Boolean indicates whether to re-start sib3 wait timer 
    will be set to TRUE when SIB3 scheduling is referanced in SB1 or SB2*/
  boolean is_need_to_restart_sib3_timer = FALSE;
#endif

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List *ext_siblist_ptr = NULL;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List2 *ext_siblist2_ptr = NULL;
#endif


  tdsrrc_SIB_ReferenceList *sb_ptr_tsib = NULL;
  tdsrrc_SIBSb_ReferenceList *sib_sb_ptr_tsib = NULL;
  tdsrrcsib_sib_type_vtag sib_type_tag_tsib;
  TDSseg_proc_stat_e_type status = TDSSEG_PROC_NONE;

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List *ext_siblist_ptr_tsib = NULL;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List2 *ext_siblist2_ptr_tsib = NULL;
#endif



#if defined (FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined (FEATURE_TDSCDMA_SIB19)
  tdsrrclog_complete_sib_rcvd_event_type sib_rcvd_event;
#endif

#ifdef FEATURE_TDSCDMA_SPECIFIC_SIB_READING	
		uint32 sib_db_index;					/* The index in the SIB database for this*/
#endif

  uint32 timer_value;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  if(
      ((pdu_num == tdsrrc_SB1)&&(tdssib_events_ptr->scheduling_updated_by_sb1 == TRUE))
    ||((pdu_num == tdsrrc_SB2)&&(tdssib_events_ptr->scheduling_updated_by_sb2 == TRUE))
    )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Scheduling already updated by %d",pdu_num);
    return;
  }

  switch(pdu_num)
  {
    //if the updating pdu is MIB, get the sib type and later update the
    //scheduling information
    case tdsrrc_MIB:
      mib_ptr = (tdsrrc_MasterInformationBlock *)sib_ptr;

      sibSb_ptr = &(mib_ptr->sibSb_ReferenceList);
  
      index = 0;
      while(sibSb_ptr->n > index)
      {
        tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag(&sibSb_ptr->elem[index].sibSb_Type,
                                                    &sib_type_tag);
        if(sib_type_tag.sib_type != tdsrrc_MAX_SIB)
        {
          tdsrrcsib_update_pos_offset_in_sched_info(
            sib_type_tag.sib_type,
          &sibSb_ptr->elem[index].scheduling);
        }
        index++;
      }

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_MIB(mib_ptr))
      {
        ext_siblist_ptr = &(mib_ptr->v690NonCriticalExtensions.
          v6b0NonCriticalExtensions.masterInformationBlock_v6b0ext.extSIBTypeInfoSchedulingInfo_List);
  
        index = 0;
        while(ext_siblist_ptr->n > index)
        {
          if(ext_siblist_ptr->elem[index].extensionSIB_Type.t & T_tdsrrc_SIB_TypeExt_systemInfoType11bis)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib11bis:Sched present for SIB11bis in MIB,bit mask val =%x",ext_siblist_ptr->elem[index].extensionSIB_Type.t);
            
            tdsrrcsib_update_pos_offset_in_sched_info(tdsrrc_SIB11bis, &ext_siblist_ptr->elem[index].schedulingInfo);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:Ext sib bit mask val =%x,Not supported",ext_siblist_ptr->elem[index].extensionSIB_Type.t);
          }
          index++;
        }
      }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
      if((mib_ptr->m.v690NonCriticalExtensionsPresent) &&
         (mib_ptr->v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent) &&
         (mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
         (mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.
            v860NonCriticalExtensions.masterInformationBlock_v860ext.m.extSIBTypeInfoSchedulingInfo_ListPresent))
      {
        ext_siblist2_ptr = &(mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.
            v860NonCriticalExtensions.masterInformationBlock_v860ext.extSIBTypeInfoSchedulingInfo_List);
  
        index = 0;
        while(ext_siblist2_ptr->n > index)
        {
          if(ext_siblist2_ptr->elem[index].extensionSIB_Type2.t & T_tdsrrc_SIB_TypeExt2_systemInfoType19)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19:Sched present for SIB19 in MIB,bit mask val =%x",ext_siblist2_ptr->elem[index].extensionSIB_Type2.t);

            tdsrrcsib_update_pos_offset_in_sched_info(tdsrrc_SIB19, &ext_siblist2_ptr->elem[index].schedulingInfo);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:Ext sib bit mask val =%x,Not supported",ext_siblist2_ptr->elem[index].extensionSIB_Type2.t);
          }
          index++;
        }
      }

#endif

      index = 0;
      sib_sb_ptr_tsib = &(mib_ptr->sibSb_ReferenceList);
      while(sib_sb_ptr_tsib->n > index)
      {
        tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag(&sib_sb_ptr_tsib->elem[index].sibSb_Type,
                                                          &sib_type_tag_tsib);
        if(sib_type_tag_tsib.sib_type != tdsrrc_MAX_SIB)
        {
          if(tdsrrcsib_is_sib_seg_in_tsib_optim_db(sib_type_tag_tsib.sib_type) == TRUE)
          {
            status = tdsrrcsib_check_if_sched_info_match(sib_type_tag_tsib.sib_type,event_index);
            if(status == TDSSEG_PROC_COMPLETE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"All segments were received prior to scheduling info. Received SIB %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib_type_tag_tsib.sib_type));
              tdsrrc_log_complete_sib(tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib_type_tag_tsib.sib_type),0,0);
              tdssib_events_ptr->event[event_index].tsib[sib_type_tag_tsib.sib_type].sib_received = TRUE;
              tdssib_events_ptr->event[event_index].tsib[sib_type_tag_tsib.sib_type].sib_decoded = FALSE;
            }
          }
        }
        index++;
      }

#ifdef FEATURE_TDSCDMA_SPECIFIC_SIB_READING		
       /* Scheduling info are all received here, Generally we use PLMN Id and Cell Id for indexing 
	        into the database. But to for save power, use Frequency and Primary Scrambling Code
	        to find the SIB DB index if FEATURE_TDSCDMA_SPECIFIC_SIB_READING is
	        defined.This should be used only for cell selections.  So here, If all schedule 
	        info are recieved, we can check if DB index exist or not*/
	     /*The type is normal and req from CSP and is serving cell*/
      if ((tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_NORMAL) 
          && (tdssib_events_ptr->event[event_index].req_proc == TDSRRC_PROCEDURE_CSP) 
          && (tdssib_events_ptr->event[event_index].for_serving_cell == TRUE)
          && (tdssib_events_ptr->event[event_index].sib_db_index_found != TRUE)
          && (tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB3].sib_received != TRUE))
      { 
        if(((!tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB1])
			 	      ||((tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB1])
			 	          &&(tdssib_events_ptr->event[event_index].tempholder.sb1_decoded))) && 
            ((!tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB2])
              ||((tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB2])
                  && (tdssib_events_ptr->event[event_index].tempholder.sb2_decoded)))
				  )
        {
          /* Check if this cell already exists. Do the search based on  frequency, and scrambling code */
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Checking SIBDB index for Freq %d,CPID %d",tdssib_events_ptr->srv_bcch.freq,tdssib_events_ptr->srv_bcch.cpid );

          sib_db_index =
					      tdsrrcsibdb_find_cell_index_for_freq_cpid_for_special_reading(tdssib_events_ptr->srv_bcch.freq,
					                                                  tdssib_events_ptr->srv_bcch.cpid);

          if( sib_db_index != TDSSIBDB_INVALID_INDEX )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIBDB index for Freq and CPID found! DB-Index %d,Cell-id %d",sib_db_index,tdssibdb.cell[sib_db_index].cell_id);
            /* If the sib_db_index is found, only need to get SIB3 to double confirm */
            tdsrrcsib_update_sib_special_reading_schedule_table(tdsrrc_SIB3, event_index);
          }
/*If FEATURE_TDSCDMA_G2T_TUNE_AWAY is enable, it will send specific schedule info to L1 
for serving cell or Nbr cell, So don't need this part of code any more*/
        }
      }
#endif/* FEATURE_TDSCDMA_SPECIFIC_SIB_READING */

 
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
      if(tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN ||
         tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN ||
         tdssib_events_ptr->event[event_index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN )
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"BTD:-sb1_needed =%d,Rept_rate = %d ",tdssib_events_ptr->event[event_index].tsib[tdsrrc_SB1].sib_needed ,tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate);
        if(tdssib_events_ptr->event[event_index].tsib[tdsrrc_SB1].sib_needed == TRUE &&
            tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
        {
          max_rept_rate = tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate;
        }
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"BTD:-sb2_needed =%d,Rept_rate = %d ",tdssib_events_ptr->event[event_index].tsib[tdsrrc_SB2].sib_needed ,tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate);
        if(tdssib_events_ptr->event[event_index].tsib[tdsrrc_SB2].sib_needed == TRUE &&
            tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
        {
          max_rept_rate = ((max_rept_rate>tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate)?
                                      max_rept_rate : tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate);
        }
      }
#endif

      if( ((tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN)||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN)) &&
          tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)
      {
        /* send all 1 bitmask to L1 so that L1 can release lock for other SUB. */
        if (!tdsrrcsib_g_in_data_call)
        {
          tdsrrcsib_send_empty_table(event_index, FALSE);
        }
        
        if((tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP) &&
           (tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP))
        {
          /* Stop the SIB wait timer in case it's running. and start a new timer based on rep factor times the
             reptition rate for sib 3 */
          tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB1,3 in MIB SIB 1 rep %d,SIB 3 rep %d",tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate,tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate);

#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
          if(tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN ||
             tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN ||
             tdssib_events_ptr->event[event_index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN )
          {
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"BTD:-max_rept_rate SB1/SB2= %d, rept_rate SIB3=%d, SIB1 ",max_rept_rate,
            tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate,
            tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate);

            max_rept_rate = (max_rept_rate >tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate)?
                                      max_rept_rate:tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate;
            max_rept_rate = (max_rept_rate >tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate)?
                                      max_rept_rate : tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate;
            
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"BTD:-Final max_rept_rate = %d",max_rept_rate);
          }
          else
#endif
          {
            max_rept_rate = (tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate >
                           tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate) ?
                           tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate:
                           tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate;
          }
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Stop MIB, Start SIB1,3 tmr for %d ms",(max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR));

          timer_value = max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR;
          tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, timer_value);
          tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);
        }
        else
        {

          //sib1,3 info not present in MIB and SB1 and SB2 is not present, this is an invalid config..
          if((tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP) &&
             (tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Mandatory SIB's not present");
          }
          else  //valid config, check max scheduling rate of SB1 and SB2 and start timer
          {
            if(tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
            {
              sb1_rep_rate = tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate;
            }
            if(tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
            {
              sb2_rep_rate = tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate;
            }

            max_rept_rate = (sb1_rep_rate > sb2_rep_rate) ? sb1_rep_rate: sb2_rep_rate;

            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Start SB timer max_rep_rate %d",max_rept_rate);
            /* Stop the SIB wait timer in case it's running. and start a new timer based on rep factor times the
               reptition rate of max of SB1 or SB2 */
            tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );

            timer_value = max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR;
            tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, timer_value);
            tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);
          }
        }
      }
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      else if((tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN) &&
          (tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS))
      {
        /*If the event name is TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN, then we need
         *to re-start SIB wait timer with the value giving max chance of one 
         *SIB3 rept cycle
         */
        if(tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
        {
          uint32 sb3_rep_rate = 0;             //local vars used to calculate 
          /* Stop the SIB wait timer in case it's running. and start a new timer based on rep factor times the
             reptition rate for sib 3 */
          tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );
          sb3_rep_rate = tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate;
          
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS:SIB3 in MIB,SIB 3 rep %d",sb3_rep_rate);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS:Stop MIB,Start SIB3 tmr for %d ms",(sb3_rep_rate * TDSRRCSIB_CELL_ID_SIB3_WAIT_FACTOR));
          tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER,
                              (unsigned long) (sb3_rep_rate * TDSRRCSIB_CELL_ID_SIB3_WAIT_FACTOR));
        }
        else
        {
          //sib3 info not present in MIB and SB1 and SB2 is not present, this is an invalid config..
          if((tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP) &&
             (tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS:Mandatory SIB's not present");
          }
          else  //valid config, check max scheduling rate of SB1 and SB2 and start timer
          {
            if(tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
            {
              sb1_rep_rate = tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate;
            }
            if(tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
            {
              sb2_rep_rate = tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate;
            }
            max_rept_rate = (sb1_rep_rate > sb2_rep_rate) ? sb1_rep_rate: sb2_rep_rate;
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS:Start SB timer max_rep_rate %d",max_rept_rate);
            /* Stop the SIB wait timer in case it's running. and start a new timer based on rep factor times the
               reptition rate of max of SB1 or SB2 */
            tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );
            tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER,
                                (unsigned long) (max_rept_rate * TDSRRCSIB_CELL_ID_SIB3_WAIT_FACTOR));
          }
        }
      }
#endif
      else if(tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_NORMAL)
      {
        /* ateast one sb is present.. get the max and then start a time based on SB */
        if((tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP) ||
           (tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP))
        {
          if(tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
          {
            sb1_rep_rate = tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate;
  
              if (tdsrrcsib_sib_sleep.sib_sleep_flag)
              {
                 tdsrrcsib_process_sib_sleep_before_sb(tdssib_events_ptr->event[event_index].tempholder.mib_sfn_prime,
                                                       (tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate / 2),
                                                       tdssib_events_ptr->sib_sched[tdsrrc_SB1].sfn_offset[0],
                                                       event_index);
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS_RRC_NV_SIB_SLEEP_BEFORE_SB not enabled");

                /* send SIB_SCHED_CHANGE_IND so that L1 can release lock before receiving SB. */
                if(!tdsrrcsib_g_in_data_call)
                {
                  /* Do not need to send SIB_SCHED_CHANGE_IND when tdsrrcsib_g_in_data_call is TRUE,
                  because will send it in function tdsrrcsib_process_completed_sibs(). */
                  tdsrrcsib_send_empty_table(event_index, FALSE);
                }
              }
  
          }

          if(tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
          {
            sb2_rep_rate = tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate;
          }

          max_rept_rate = (sb1_rep_rate > sb2_rep_rate) ? sb1_rep_rate: sb2_rep_rate;

          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Start SB timer for N_BCCH max_rep_rate %d",max_rept_rate);

          if(tdssib_events_ptr->event[event_index].for_serving_cell == FALSE)
          {
            if(tdsrrcsib_g_in_data_call)
            {
              tdsrrctmr_stop_timer(TDSRRCTMR_SIB_WAIT_TIMER);

              /* SIB wait timer is max of 3000, 60*rep_rate */
              timer_value = MAX(3000, 60*max_rept_rate);
              tdsrrctmr_start_timer(TDSRRCTMR_SIB_WAIT_TIMER, timer_value);
              tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);

              tdsrrctmr_start_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER,
                                    MAX(max_rept_rate, TDSRRCSIB_MIN_MASK_INV_TIMER)*TDSRRCSIB_MASK_INVERT_FACTOR
                                        + TDSRRCSIB_TRANS_BUFFER + TDSRRCSIB_EMPTY_FRAMES);

              tdsrrctmr_start_timer(TDSRRCTMR_SIB_STOP_PARTITION_TIMER, MAX(2500, 40*max_rept_rate));
              tdsrrcsib_stop_partition = FALSE;
            }
            else
            {
              tdsrrctmr_stop_timer(TDSRRCTMR_SIB_WAIT_TIMER);
              timer_value = max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR;
              tdsrrctmr_start_timer(TDSRRCTMR_SIB_WAIT_TIMER, timer_value);
              tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);
                
              tdsrrctmr_start_timer(TDSRRCTMR_SIB_STOP_PARTITION_TIMER, 
                                    (unsigned long) (max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR * 2 / 3));
              tdsrrcsib_stop_partition = FALSE;
            }
          }
          else if(tdsrrcsib_g_in_data_call)
          {
            tdsrrctmr_start_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER,
                                  MAX(max_rept_rate, TDSRRCSIB_MIN_MASK_INV_TIMER)*TDSRRCSIB_MASK_INVERT_FACTOR
                                      + TDSRRCSIB_TRANS_BUFFER + TDSRRCSIB_EMPTY_FRAMES);
          }
        }
        /* none of the SB's are present.. get the max-timer updated by mib and start wait timer */
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"SB's not present N_BCCH max_rep_rate is %d",tdssib_events_ptr->scheduling_max_rept_rate);

          //for some unforseen reason,  max_rept_rate is still 0<uninitialized> start the default timer
          //print a MSG_ERROR to get attention
          if(tdssib_events_ptr->scheduling_max_rept_rate == 0)
          {
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"max_rept_rate == 0.  Setting default timer");
            tdsrrctmr_start_timer(TDSRRCTMR_SIB_WAIT_TIMER, TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS);  
            if(tdssib_events_ptr->event[event_index].for_serving_cell == FALSE)
            {
              tdsrrcsib_send_sib_wait_timer_len_to_l1(TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS >> 1);
            }
          }
          else
          {
            if(tdssib_events_ptr->event[event_index].for_serving_cell == FALSE)
            {
              if(tdsrrcsib_g_in_data_call)
              {
                tdsrrctmr_stop_timer(TDSRRCTMR_SIB_WAIT_TIMER);
  
                /* SIB wait timer is max of 3000, 60*rep_rate */
                timer_value = MAX(3000, 60 * tdssib_events_ptr->scheduling_max_rept_rate);
                tdsrrctmr_start_timer(TDSRRCTMR_SIB_WAIT_TIMER, timer_value);
                tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);
  
                tdsrrctmr_start_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER,
                                      MAX(tdssib_events_ptr->scheduling_max_rept_rate, TDSRRCSIB_MIN_MASK_INV_TIMER)*TDSRRCSIB_MASK_INVERT_FACTOR
                                          + TDSRRCSIB_TRANS_BUFFER + TDSRRCSIB_EMPTY_FRAMES);
  
                tdsrrctmr_start_timer(TDSRRCTMR_SIB_STOP_PARTITION_TIMER, MAX(2500, 40*tdssib_events_ptr->scheduling_max_rept_rate));
                tdsrrcsib_stop_partition = FALSE;
              }
              else
              {
                tdsrrctmr_stop_timer(TDSRRCTMR_SIB_WAIT_TIMER);
                timer_value = tdssib_events_ptr->scheduling_max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR;
                tdsrrctmr_start_timer(TDSRRCTMR_SIB_WAIT_TIMER, timer_value);

                tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);

                tdsrrctmr_start_timer(TDSRRCTMR_SIB_STOP_PARTITION_TIMER, 
                                      (unsigned long) (tdssib_events_ptr->scheduling_max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR * 2/3));
                tdsrrcsib_stop_partition = FALSE;
              }
            }
            else if(tdsrrcsib_g_in_data_call)
            {
              tdsrrctmr_start_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER,
                                    MAX(tdssib_events_ptr->scheduling_max_rept_rate, TDSRRCSIB_MIN_MASK_INV_TIMER)*TDSRRCSIB_MASK_INVERT_FACTOR
                                        + TDSRRCSIB_TRANS_BUFFER + TDSRRCSIB_EMPTY_FRAMES);
            }
          }
        }
        
        /* send a timer value to TL1 before receiving SB/SIBs, so that priority inversion could be applied later. */
        if(tdssib_events_ptr->event[event_index].for_serving_cell == TRUE
            && tdsrrcsib_g_in_data_call == FALSE)
        {
          tdsrrcsib_send_sib_wait_timer_len_to_l1(15 * tdssib_events_ptr->scheduling_max_rept_rate);
        }
      }
      break;

    //if the updating pdu is SB1 or SB2, get the sib type and later update the
    //scheduling information
    case tdsrrc_SB1:
    case tdsrrc_SB2:

      if( ((tdssib_events_ptr->event[event_index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN) || 
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN)) &&
          tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)
      {
        if((tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP) ||
           (tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP))
        {
          sib1_or_sib3_config_in_un_received_sb = TRUE;
        }
      }

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      if((tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN) &&
          (tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS))
      {
        if(tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP)
        {
          is_need_to_restart_sib3_timer  =  TRUE;
        }
      }
#endif

      if( pdu_num == (uint32)tdsrrc_SB1 )
      {
        sb1_ptr = (tdsrrc_SysInfoTypeSB1 *)sib_ptr;
        sb_ptr = &(sb1_ptr->sib_ReferenceList);
        tdssib_events_ptr->scheduling_updated_by_sb1 = TRUE;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(sb1_ptr,
             tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1))
        {
          ext_siblist_ptr = &(sb1_ptr->v6b0NonCriticalExtensions.sysInfoTypeSB1_v6b0ext.
            extSIBTypeInfoSchedulingInfo_List);
        }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
        if((sb1_ptr->m.v6b0NonCriticalExtensionsPresent) &&
           (sb1_ptr->v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
           (sb1_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.sysInfoTypeSB1_v860ext.
               m.extSIBTypeInfoSchedulingInfo_ListPresent))
        {
            ext_siblist2_ptr = &sb1_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
              sysInfoTypeSB1_v860ext.extSIBTypeInfoSchedulingInfo_List;
        }
#endif

      }
      else
      {
        sb2_ptr = (tdsrrc_SysInfoTypeSB2 *)sib_ptr;
        sb_ptr = &(sb2_ptr->sib_ReferenceList);
        tdssib_events_ptr->scheduling_updated_by_sb2 = TRUE;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(sb2_ptr,
           tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2))
        {
          ext_siblist_ptr = &(sb2_ptr->v6b0NonCriticalExtensions.sysInfoTypeSB2_v6b0ext.
            extSIBTypeInfoSchedulingInfo_List);
        }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
        if((sb2_ptr->m.v6b0NonCriticalExtensionsPresent) &&
           (sb2_ptr->v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
           (sb2_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
              sysInfoTypeSB2_v860ext.m.extSIBTypeInfoSchedulingInfo_ListPresent))
        {
          ext_siblist2_ptr = &(sb2_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
            sysInfoTypeSB2_v860ext.extSIBTypeInfoSchedulingInfo_List);
        }
#endif

      }

      sb_ptr_tsib = sb_ptr;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      ext_siblist_ptr_tsib = ext_siblist_ptr;
#endif
#ifdef FEATURE_TDSCDMA_SIB19
      ext_siblist2_ptr_tsib = ext_siblist2_ptr;
#endif

      index = 0;
      while(sb_ptr->n > index)
      {

        tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&sb_ptr->elem[index].sib_Type,
                                                    &sib_type_tag);
        if(sib_type_tag.sib_type != tdsrrc_MAX_SIB)
        {
          tdsrrcsib_update_pos_offset_in_sched_info(
            sib_type_tag.sib_type,
          &sb_ptr->elem[index].scheduling);
         }
       index++;
      }
  
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if(ext_siblist_ptr != NULL)
      {
      index = 0;
        while(ext_siblist_ptr->n > index)
        {
            if(ext_siblist_ptr->elem[index].extensionSIB_Type.t & T_tdsrrc_SIB_TypeExt_systemInfoType11bis)        
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib11bis:Sched present for SIB11bis in MIB,bit mask val =%x",ext_siblist_ptr->elem[index].extensionSIB_Type.t);
              
              tdsrrcsib_update_pos_offset_in_sched_info(tdsrrc_SIB11bis, &ext_siblist_ptr->elem[index].schedulingInfo);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:Ext sib bit mask val =%x,Not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[index].extensionSIB_Type)));
          }
          index++;
        }
      }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
      if(ext_siblist2_ptr != NULL)
      {
        index = 0;
        while(ext_siblist2_ptr->n > index)
        {
          if(ext_siblist2_ptr->elem[index].extensionSIB_Type2.t & T_tdsrrc_SIB_TypeExt2_systemInfoType19)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19:Sched present for SIB19 in MIB,bit mask val =%x",ext_siblist2_ptr->elem[index].extensionSIB_Type2.t);
            
            tdsrrcsib_update_pos_offset_in_sched_info(tdsrrc_SIB19, &ext_siblist2_ptr->elem[index].schedulingInfo);
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:Ext sib bit mask val =%x,Not supported",ext_siblist2_ptr->elem[index].extensionSIB_Type2.t);
          }
          index++;
        }
      }
#endif
        
      index = 0;
      while(sb_ptr_tsib->n > index)
      {
        tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&sb_ptr_tsib->elem[index].sib_Type,
                                                      &sib_type_tag_tsib);
        if(sib_type_tag_tsib.sib_type != tdsrrc_MAX_SIB)
        {
          if(tdsrrcsib_is_sib_seg_in_tsib_optim_db(sib_type_tag_tsib.sib_type) == TRUE)
          {
            status = tdsrrcsib_check_if_sched_info_match(sib_type_tag_tsib.sib_type,event_index);
            if(status == TDSSEG_PROC_COMPLETE)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"All segments were received prior to scheduling info. Received SIB %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib_type_tag_tsib.sib_type));
              tdsrrc_log_complete_sib(tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib_type_tag_tsib.sib_type),0,0);
              tdssib_events_ptr->event[event_index].tsib[sib_type_tag_tsib.sib_type].sib_received = TRUE;
              tdssib_events_ptr->event[event_index].tsib[sib_type_tag_tsib.sib_type].sib_decoded = FALSE;
            }
          }
        }
        index++;
      }

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if(ext_siblist_ptr_tsib != NULL)
      {
        index = 0;
        while(ext_siblist_ptr_tsib->n > index)
        {
          if( TDSRRCCHECK_SIB_TypeExt(
                  (&(ext_siblist_ptr_tsib->elem[index].extensionSIB_Type)),
                   systemInfoType11bis)
          )                 
          {
            if(tdsrrcsib_is_sib_seg_in_tsib_optim_db(tdsrrc_SIB11bis) == TRUE)
            {
              status = tdsrrcsib_check_if_sched_info_match_for_ext_sibs(tdsrrc_SIB11bis,event_index);
              if(status == TDSSEG_PROC_COMPLETE)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"All segments were received prior to scheduling info. Received SIB11bis");
                sib_rcvd_event.sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE11BIS;
                event_report_payload(EVENTS_TDSCDMA_RRC_SIB_RCVD_V2, sizeof(sib_rcvd_event),
                                    (void *)&sib_rcvd_event);
                tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB11bis].sib_received = TRUE;
                tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB11bis].sib_decoded = FALSE;
              }
            }
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:Ext sib bit mask val =%x,Not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr_tsib->elem[index].extensionSIB_Type)));
          }
          index++;
        }
      }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
      if(ext_siblist2_ptr_tsib != NULL)
      {
        index = 0;
        while(ext_siblist2_ptr_tsib->n > index)
        {
          if(ext_siblist2_ptr_tsib->elem[index].extensionSIB_Type2.t & 
              T_tdsrrc_SIB_TypeExt2_systemInfoType19)
          {
            if(tdsrrcsib_is_sib_seg_in_tsib_optim_db(tdsrrc_ExtensionType) == TRUE)
            {
              status = tdsrrcsib_check_if_sched_info_match_for_ext_sibs(tdsrrc_SIB19,event_index);
              if(status == TDSSEG_PROC_COMPLETE)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"All segments were received prior to scheduling info. Received SIB19");
                sib_rcvd_event.sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE19;
                event_report_payload(EVENTS_TDSCDMA_RRC_SIB_RCVD_V2, sizeof(sib_rcvd_event),
                                    (void *)&sib_rcvd_event);
                tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB19].sib_received = TRUE;
                tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB19].sib_decoded = FALSE;
              }
            }
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:Ext sib bit mask val =%x,Not supported",ext_siblist2_ptr_tsib->elem[index].extensionSIB_Type2.t);
          }
          index++;
        }
      }
#endif


#ifdef FEATURE_TDSCDMA_SPECIFIC_SIB_READING		
		 /* Scheduling info are all received here, Generally we use PLMN Id and Cell Id for indexing 
				into the database. But to for save power, use Frequency and Primary Scrambling Code
				to find the SIB DB index if FEATURE_TDSCDMA_SPECIFIC_SIB_READING is
				defined.This should be used only for cell selections.  So here, If all schedule 
				info are recieved, we can check if DB index exist or not*/
		 /*The type is normal and req from CSP and is serving cell*/
      if ((tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_NORMAL) 
           && (tdssib_events_ptr->event[event_index].req_proc == TDSRRC_PROCEDURE_CSP) 
           && (tdssib_events_ptr->event[event_index].for_serving_cell == TRUE)
           && (tdssib_events_ptr->event[event_index].sib_db_index_found != TRUE)
           && (tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB3].sib_received != TRUE))
		  { 
        if(((!tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB1])
             ||((tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB1])
								&&(tdssib_events_ptr->event[event_index].tempholder.sb1_decoded))) && 
					  ((!tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB2])
						 ||((tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB2])
								&& (tdssib_events_ptr->event[event_index].tempholder.sb2_decoded))))
        {
          /* Check if this cell already exists. Do the search based on	frequency, and scrambling code */
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Checking SIBDB index for Freq %d,CPID %d",tdssib_events_ptr->srv_bcch.freq,tdssib_events_ptr->srv_bcch.cpid );

          sib_db_index =
              tdsrrcsibdb_find_cell_index_for_freq_cpid_for_special_reading(tdssib_events_ptr->srv_bcch.freq,
																													tdssib_events_ptr->srv_bcch.cpid);

          if( sib_db_index != TDSSIBDB_INVALID_INDEX )
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIBDB index for Freq and CPID found! DB-Index %d,Cell-id %d",sib_db_index,tdssibdb.cell[sib_db_index].cell_id);
            /* If the sib_db_index is found, only need to get SIB3 to double confirm */
            tdsrrcsib_update_sib_special_reading_schedule_table(tdsrrc_SIB3, event_index);
          }
/*If FEATURE_TDSCDMA_G2T_TUNE_AWAY is enable, it will send specific schedule info to L1 
for serving cell or Nbr cell, So don't need this part of code any more*/
        }
      }
#endif/* FEATURE_TDSCDMA_SPECIFIC_SIB_READING */

      //check if scan type is bplmn scan type.. if so, then if SIB1,3 sched info is to be received in
      //scheduling information, check if both are received, then start SIB1,3 timer based on the
      //max rep-rate of SIB1 or 3
      if( ((tdssib_events_ptr->event[event_index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[event_index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN)||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN) || 
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN)) &&
          tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)
      {
        if(sib1_or_sib3_config_in_un_received_sb == TRUE)
        {
          if((tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP) &&
             (tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP))
          {
            /* Stop the SIB wait timer in case it's running. and start a new timer based on rep factor times the
               reptition rate for sib 3 */
            tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );
  
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB1,3 in MIB SIB 1 rep %d,SIB 3 rep %d",tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate,tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate);
  
            max_rept_rate = (tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate >
                             tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate) ?
                             tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate:
                             tdssib_events_ptr->sib_sched[tdsrrc_SIB1].repitition_rate;
  
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Stop SB, Start SIB1,3 tmr for %d ms",(max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR));

            timer_value = max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR;
            tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, timer_value);
            tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);
          }
        }
      }
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      else if((tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN) &&
          (tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS))
      {
        if(tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
        {
          if(is_need_to_restart_sib3_timer == TRUE)
          {
            uint32 sb3_rep_rate = 0;             //local vars used to calculate 
            /* Stop the SIB wait timer in case it's running. and start a new timer based on rep factor times the
               reptition rate for sib 3 */
            tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );
            sb3_rep_rate = tdssib_events_ptr->sib_sched[tdsrrc_SIB3].repitition_rate;
            
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS:IB3 in MIB,SIB 3 rep %d",sb3_rep_rate);
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS:Stop MIB,Start SIB3 tmr for %d ms",(sb3_rep_rate * TDSRRCSIB_CELL_ID_SIB3_WAIT_FACTOR));
            tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER,
                                (unsigned long) (sb3_rep_rate * TDSRRCSIB_CELL_ID_SIB3_WAIT_FACTOR));
          }
        }
      }
#endif
      else if(tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_NORMAL)
      {
        /* ateast one sb is present.. get the max and then start a time based on SB */
        if((tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP) ||
           (tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP))
        {
          if(tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP &&
             tdssib_events_ptr->scheduling_updated_by_sb1 == FALSE)
          {
            all_scheduling_blocks_received = FALSE;
          }

          if(tdssib_events_ptr->sib_sched[tdsrrc_SB2].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP &&
             tdssib_events_ptr->scheduling_updated_by_sb2 == FALSE)
          {
            all_scheduling_blocks_received = FALSE;
          }

          if(TRUE == all_scheduling_blocks_received)
          {
            //for some unforseen reason,  max_rept_rate is still 0<uninitialized> start the default timer
            //print a MSG_ERROR to get attention
            if(tdssib_events_ptr->scheduling_max_rept_rate == 0)
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"max_rept_rate == 0.  Setting default timer");
              tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS );
              if(tdssib_events_ptr->event[event_index].for_serving_cell == FALSE)
              {
                tdsrrcsib_send_sib_wait_timer_len_to_l1(TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS >> 1);
              }
            }
            else
            {
              if(tdssib_events_ptr->event[event_index].for_serving_cell == FALSE)
              {
                if(tdsrrcsib_g_in_data_call)
                {
                  tdsrrctmr_stop_timer(TDSRRCTMR_SIB_WAIT_TIMER);
                  tdsrrctmr_stop_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER);
                  tdsrrctmr_stop_timer(TDSRRCTMR_SIB_STOP_PARTITION_TIMER);

                  /*SIB wait timer is max of 3000, 60*rep_rate*/
                  timer_value = MAX(3000, 60 * tdssib_events_ptr->scheduling_max_rept_rate);
                  tdsrrctmr_start_timer(TDSRRCTMR_SIB_WAIT_TIMER, timer_value);
                  tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);

                  tdsrrctmr_start_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER, 
                                     MAX(tdssib_events_ptr->scheduling_max_rept_rate, TDSRRCSIB_MIN_MASK_INV_TIMER)*TDSRRCSIB_MASK_INVERT_FACTOR
                                     + TDSRRCSIB_TRANS_BUFFER);

                  tdsrrctmr_start_timer(TDSRRCTMR_SIB_STOP_PARTITION_TIMER, MAX(2500, 40*tdssib_events_ptr->scheduling_max_rept_rate));
                  tdsrrcsib_stop_partition = FALSE;
                }
                else
                {
                  tdsrrctmr_stop_timer(TDSRRCTMR_SIB_WAIT_TIMER);
                  timer_value = tdssib_events_ptr->scheduling_max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR;
                  tdsrrctmr_start_timer(TDSRRCTMR_SIB_WAIT_TIMER, timer_value);
                  tdsrrcsib_send_sib_wait_timer_len_to_l1(timer_value >> 1);

                  tdsrrctmr_stop_timer(TDSRRCTMR_SIB_STOP_PARTITION_TIMER);
                  tdsrrctmr_start_timer(TDSRRCTMR_SIB_STOP_PARTITION_TIMER, 
                                       (unsigned long) (tdssib_events_ptr->scheduling_max_rept_rate * TDSRRCSIB_BPLMN_NBCCH_SIB_WAIT_FACTOR * 2/3));
                  tdsrrcsib_stop_partition = FALSE;
                }
              }
              else if(tdsrrcsib_g_in_data_call)
              {
                tdsrrctmr_start_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER, 
                                      MAX(tdssib_events_ptr->scheduling_max_rept_rate, TDSRRCSIB_MIN_MASK_INV_TIMER)*TDSRRCSIB_MASK_INVERT_FACTOR
                                      + TDSRRCSIB_TRANS_BUFFER);
              }
            }

            /* send a timer value to TL1 before receiving other SIBs, so that priority inversion could be applied later. */
            if(tdssib_events_ptr->event[event_index].for_serving_cell == TRUE
                && tdsrrcsib_g_in_data_call == FALSE)
            {
              tdsrrcsib_send_sib_wait_timer_len_to_l1(15 * tdssib_events_ptr->scheduling_max_rept_rate);
            }
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"SB, schedule not present,but SB received");
        }
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Sib-Sched Type %d",pdu_num);
      break;
  }
}
/*===========================================================================

FUNCTION tdsrrcsib_update_mib_sb_sched_info_from_sib_db

DESCRIPTION
  Updates scheduling information from sib_db.
  This is used when we get a serving-cell event that needs to
  update SIB's(eg: SIB7) and we already know the serving cell is valid and have
  the SIB's and MIB stored in DB

DEPENDENCIES
 
  None.

RETURN VALUE
 
  None.

SIDE EFFECTS
  SIB-TDSEVENT DB is updated
===========================================================================*/
static void tdsrrcsib_update_mib_sb_sched_info_from_sib_db
(
uint32 dbindex,
tdssib_events_index_e_type event_index
)
{

  if(tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB, dbindex) != NULL)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sched. Info DB-Idx %d for SIB %d",dbindex,tdsrrc_MIB);
    tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_MIB, tdssibdb.cell[dbindex].sibs.mib, event_index);
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"MIB not present in db %d for idx",dbindex);
  }

  if(tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB1, dbindex) != NULL)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sched. Info DB-Idx %d for SIB %d",dbindex,tdsrrc_SB1);
    tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_SB1,tdssibdb.cell[dbindex].sibs.sb1, event_index);
  }

  if(tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB2, dbindex) != NULL)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Sched. Info DB-Idx %d for SIB %d",dbindex,tdsrrc_SB2);
    tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_SB2, tdssibdb.cell[dbindex].sibs.sb2, event_index);
  }
}

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
)
{
  uint32 count;

  for( count = 0; count < TDSRRC_TOT_SIBS; count++ )
  {
    switch(count)
    {
      case tdsrrc_MIB:
      case tdsrrc_SIB1:
      case tdsrrc_SIB3:
      case tdsrrc_SIB4:
      case tdsrrc_SIB5:
      case tdsrrc_SIB6:
      case tdsrrc_SIB7:
      case tdsrrc_SIB11:
      case tdsrrc_SIB12:
      case tdsrrc_SIB18:
      case tdsrrc_SB1:
      case tdsrrc_SB2:
      case tdsrrc_SIB2:
        tdssupported_sibs_lookup_table[count] = TRUE;
        break;

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      case tdsrrc_SIB11bis:
        if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
        {
          tdssupported_sibs_lookup_table[count] = TRUE;
        }
        else
        {
          tdssupported_sibs_lookup_table[count] = FALSE;
        }
        break;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
      case tdsrrc_SIB19:
        tdssupported_sibs_lookup_table[count] = TRUE;
        break;
#endif

      default:
        tdssupported_sibs_lookup_table[count] = FALSE;
        break;
    }
  }
}
/*===========================================================================

FUNCTION CREATE_NEW_SIB_EVENT

DESCRIPTION

  This function activates and initializes an event for 
  getting SIBs. The event can be for either a serving
  cell or for a neighbor cell. Only one event of either
  cell type can be supported at a time. 

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcsib_create_new_sib_event
(
  tdssib_events_index_e_type index, /* Index in the SIB event database
                                 for this event */
  tdsrrc_cmd_type *cmd_ptr          /* Received command pointer */
)
{
  uint32 count;  /* Used as a counter */
  
  /* Fill in relevant data for the event using the assigned
  index */

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  /* Event is now active */
  tdssib_events_ptr->event[index].event_active = TRUE;

  /* Assume that a SIB Database Index has not been found yet
  for this event. Also mib and sib3 have not been
  decoded yet and mip position hasn't been validated. 
  This is the case for all events except events where
  we're updating specific SIBs for the serving cell.
  This is handled later in this function */

  tdssib_events_ptr->event[index].sib_db_index_found = FALSE;
  tdssib_events_ptr->event[index].tempholder.mib_decoded = FALSE;
  tdssib_events_ptr->event[index].tempholder.sib3_decoded = FALSE;
  tdssib_events_ptr->event[index].tempholder.sib1_decoded = FALSE;
  tdssib_events_ptr->event[index].mib_pos_validated = FALSE;
  tdssib_events_ptr->event[index].tempholder.mib_sfn_prime = TDSRRCSIB_INVLID_SFN_PRIME;

#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrcsib_sib19_decode_fail_count = 0;
#endif

  tdsrrcsib_pattern_invert = FALSE;
  tdsrrcsib_stop_partition = FALSE;
  tdsrrcsib_g_in_data_call = TDSL1_DS_G2T_TA_MODE_ACTIVE();
  if(tdsrrcsib_g_in_data_call == TRUE)
  {
    if(cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE)
    {
      /*Serving cell, Check from connected to idle*/
      if (TRUE == tdsrrccsp_do_opt_for_con_to_idle())
      {
        tdsrrcsib_g_in_data_call = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH, "Cell sel:In DDS from Connected to IDLE, set G2T flag to FALSE");
      }
    }
    else
    {  
      /*Nbr cell*/
      if(TRUE == tdsrrccsp_is_other_sub_in_dds())
      {
        tdsrrcsib_g_in_data_call = FALSE;
         TDSRRC_MSG0(MSG_LEGACY_HIGH, "Cell resel:In DDS, set G2T flag to FALSE");
      }
    }
  }

  //invalidate sib-scheduling info as we are setting up a new event
  tdsrrcsib_invalidate_sib_scheduling_info(index);

//#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
  tdssib_events_ptr->event[index].int_event.int_event_type 
          = TDSRRCSIB_INT_MAX_EVENT;
//#endif

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
  tdsno_mib_bar_cell_count = 0;
#endif

  /*Initialize tsib optimization data base*/
  tdsrrcsib_clear_tsib_optimization_db();

  /* Based on the type of event requested, each SIB that
  needs to be collected will be activated. */

  switch(cmd_ptr->cmd_hdr.cmd_id)
  {
    case TDSRRC_GET_ALL_SIBS_REQ:
    case TDSRRC_UPDATE_SIBS_IND:

      /* We use the same case statement for these two events
      since we have to get all the necessary SIBs for both
      events. But for TDSRRC_UPDATE_SIBS_IND, we can use value-tag
      comparisons later on and get only the sibs that have changed.
      Loop through all SIBs and activate necessary ones. */
      for( count = 0; count < TDSRRC_TOT_SIBS; count++ )
      {
        if(tdssupported_sibs_lookup_table[count] == FALSE)
        {
          tdssib_events_ptr->event[index].tsib[count].sib_needed = FALSE;
        }
        else
        {
          /* We need to collect all other SIBs. Initialize all
          necessary variables within the SIB event database */ 
          tdssib_events_ptr->event[index].tsib[count].sib_needed = TRUE;
          tdssib_events_ptr->event[index].tsib[count].sib_received = FALSE;
          tdssib_events_ptr->event[index].tsib[count].sib_decoded = FALSE;
          tdssib_events_ptr->event[index].tsib[count].seg_count = 0;
          /* For now, make the current segment index invalid. This is
          because only after the first segment is received, the segment
          index will become zero. */
          tdssib_events_ptr->event[index].tsib[count].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
          tdssib_events_ptr->event[index].tsib[count].sib_tot_length = 0;
        }
      }
      /* Note the event name and the name of the procedure 
      requesting this event - this is used later while sending
      the confirmation command for this event. */
      if( cmd_ptr->cmd_hdr.cmd_id == TDSRRC_GET_ALL_SIBS_REQ )
      {
        tdssib_events_ptr->event[index].event_name = TDSRRCSIB_GET_ALL_SIBS;
        tdssib_events_ptr->event[index].req_proc =
          cmd_ptr->cmd.get_all_sibs_req.procedure;
      }
      else
      {
        tdssib_events_ptr->event[index].event_name = TDSRRCSIB_UPDATE_SIBS;
        /* If the event is TDSRRCSIB_UPDATE_SIBS, we also need to save 
        the new value_tag for the MIB. When we read the MIB again
        we will compare the value tag and make sure it's the
        updated MIB before comparing the value tags given by the
        MIB and the time of modification. */
        tdssib_events_ptr->event[index].req_proc =
          cmd_ptr->cmd.update_sibs_ind.procedure;
      }

      /* Note that this case can only be for a serving cell */
      tdssib_events_ptr->event[index].for_serving_cell = TRUE;

      /* Initialize the scan type to NORMAL for safety */
      tdssib_events_ptr->event[index].sib_scan_type = TDSRRC_SIB_NORMAL;

      /* When a new serving cell event is received, if there
      is a TDSRRCSIB_UPDATE_SIBS event pending, we can cancel it
      now. It's no longer valid. */
      if( tdsupdate_sibs_event_pending == TRUE )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pending UPDATE_SIBS event cleared");
        tdsupdate_sibs_event_pending = FALSE;
      }
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
      REX_ISR_LOCK(&tdssib7_in_fach_active_mutex);

      if(tdsis_sib7_in_fach_active == TRUE)
      {
        tdsis_sib7_in_fach_active = FALSE;
      }
      REX_ISR_UNLOCK(&tdssib7_in_fach_active_mutex);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB7 in cell fach event active is set to %d",tdsis_sib7_in_fach_active);
#endif
#endif
      break;

    case TDSRRC_GET_SPECIFIC_SIBS_REQ:

      /* Only get those SIBs specifically requested by the calling
      procedure. Also we need to get the MIB and SIB3 always in order
      to get the index to store these SIBs in the sib database. */

      for( count = 0; count < TDSRRC_TOT_SIBS; count++ )
      {
        boolean local_sib_needed = FALSE;

#ifdef FEATURE_3GPP_CSFB_TDS
        if(cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_READ_FROM_SIB_CONTAINER)
        {
          if(cmd_ptr->cmd.get_specific_sibs_req.sib_req[count] == TRUE)
          {
            local_sib_needed = TRUE;
          }
        }
        else
#endif

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
        if((cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
           ((cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
#ifdef FEATURE_LTE_TO_TDSCDMA
            && (!tdsrrccsp_check_cgi_scan())
#endif
           )
          )
        {
          if(cmd_ptr->cmd.get_specific_sibs_req.sib_req[count] == TRUE)
          {
            local_sib_needed = TRUE;
          }
        }
        else
#endif
        {
          if( (cmd_ptr->cmd.get_specific_sibs_req.sib_req[count] == TRUE) ||
              ( count == (uint32)tdsrrc_MIB ) ||
              ( count == (uint32)tdsrrc_SIB3 ) ||
              ( count == (uint32)tdsrrc_SB1) ||
              ( count == (uint32)tdsrrc_SB2))
          {
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
            if (count ==(uint32) tdsrrc_SIB11bis)
            {
              if(tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
              {
                local_sib_needed = TRUE;
              }
            }
            else
#endif
            {
              local_sib_needed = TRUE;
            }
          }
        }
        if(local_sib_needed == TRUE)
        {
          tdssib_events_ptr->event[index].tsib[count].sib_needed = TRUE;
          tdssib_events_ptr->event[index].tsib[count].sib_received = FALSE;
          tdssib_events_ptr->event[index].tsib[count].sib_decoded = FALSE;
          tdssib_events_ptr->event[index].tsib[count].seg_count = 0;
          /* For now, make the current segment index invalid. This is
           because only after the first segment is received, the segment
           index will become zero. */
          tdssib_events_ptr->event[index].tsib[count].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
          tdssib_events_ptr->event[index].tsib[count].sib_tot_length = 0;
        }
        else
        {
          tdssib_events_ptr->event[index].tsib[count].sib_needed = FALSE;
          tdssib_events_ptr->event[index].tsib[count].sib_received = FALSE;
        }
      }

#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
      if(cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_SCAN ||
          cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN ||
          cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN
        )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"BTD:-Mark SB1/SB2 as needed for scan type = %d ",tdssib_events_ptr->event[index].sib_scan_type);
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed = TRUE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_received = FALSE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_decoded = FALSE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].seg_count = 0;
          /* For now, make the current segment index invalid. This is
           because only after the first segment is received, the segment
           index will become zero. */
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_tot_length = 0;

          tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed = TRUE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_received = FALSE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_decoded = FALSE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].seg_count = 0;
          /* For now, make the current segment index invalid. This is
           because only after the first segment is received, the segment
           index will become zero. */
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_tot_length = 0;

        }
#endif

      //if rrc_state is CELL_FACH, then read SIB7 mandatorily for GET_SPECIFIC_SIBS..
      //this is needed because we will send cell-update immediately and need to know the
      //latest UL-Interference parameters
      if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
         ||(tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH)||
          (tdsrrc_get_state() == TDSRRC_STATE_URA_PCH)
#endif
         )
      {
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_needed = TRUE;
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_received = FALSE;
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_decoded = FALSE;
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].seg_count = 0;
        /* For now, make the current segment index invalid. This is
        because only after the first segment is received, the segment
        index will become zero. */
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].seg_index = 
          (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_tot_length = 0;
      }

      /* Note the event name */
      tdssib_events_ptr->event[index].event_name = TDSRRCSIB_GET_SPECIFIC_SIBS;
      
      /* Note the requesting procedure name */
      tdssib_events_ptr->event[index].req_proc = cmd_ptr->cmd.get_specific_sibs_req.procedure;

      //copy the scan type for this sib..
      tdssib_events_ptr->event[index].sib_scan_type =  cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type;


      /* In case of GET_SPECIFIC_SIBS event, if it's an internal
      event we need to check if it's for SIBs with SIB expiration
      timers. If so save the information in the event database */
      /* First check for SIB7 */
      if( (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_SIB) &&
          ((cmd_ptr->cmd.get_specific_sibs_req.
           sib_req[tdsrrc_SIB7]) == TRUE )
        )
      {
        tdssib_events_ptr->event[index].int_event.int_event_type 
          = TDSRRCSIB_INT_SIB7_EVENT;

        /* For this event, we already know all the information
        about the serving cell. Also we don't want to read
        the MIB and SIB3 again to update value tags at this
        time */
        tdssib_events_ptr->event[index].sib_db_index_found = TRUE;
        /* The index is the active cell index */
        tdssib_events_ptr->event[index].sib_db_index = tdssibdb.active_cell_index;
        tdssib_events_ptr->event[index].tempholder.mib_decoded = TRUE;
        tdssib_events_ptr->event[index].tempholder.sib3_decoded = TRUE;
        tdssib_events_ptr->event[index].mib_pos_validated = TRUE;

        tdssib_events_ptr->sib_sched[tdsrrc_MIB].repitition_rate = 8;
        tdssib_events_ptr->sib_sched[tdsrrc_MIB].sfn_offset[0] =0;
        tdssib_events_ptr->sib_sched[tdsrrc_MIB].sfn_offset[1] =2;
        tdssib_events_ptr->sib_sched[tdsrrc_MIB].sfn_offset[2] =4;
        tdssib_events_ptr->sib_sched[tdsrrc_MIB].sfn_offset[3] =6;


        //update scheduling info first fromt the stored db
        tdsrrcsib_update_mib_sb_sched_info_from_sib_db(tdssib_events_ptr->event[index].sib_db_index, index);

        /* Also indicate that the MIB and SIB3 for this event
        have already been received and decoded. */
        tdssib_events_ptr->event[index].
          tsib[tdsrrc_MIB].sib_received = TRUE;
        tdssib_events_ptr->event[index].
          tsib[tdsrrc_MIB].sib_decoded = TRUE;
        tdssib_events_ptr->event[index].
          tsib[tdsrrc_SIB3].sib_received = TRUE;
        tdssib_events_ptr->event[index].
          tsib[tdsrrc_SIB3].sib_decoded = TRUE;
        
        if(TRUE == tdsrrcsibdb_check_if_sb_present(tdsrrc_SB1,tdssib_events_ptr->event[index].sib_db_index))
        {
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_received = TRUE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_decoded = TRUE;
        }
        
        if(TRUE == tdsrrcsibdb_check_if_sb_present(tdsrrc_SB2, tdssib_events_ptr->event[index].sib_db_index))
        {
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_received = TRUE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_decoded = TRUE;
        }
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
        if(tdsrrcsib_is_current_event_sib7_in_fach() == TRUE)
        {
          /*Since for SIB-7 event SIB will use scheduling info from MIB
            So copy scheduling info of MIB to event table.This will avoid
            discarding any SIB-7 which is scheduled just before MIB 
            when this event is brought up*/
          tdsrrcsib_build_sib7_schedule_table(tdsrrc_SIB7,
                                              TDSSIB_SRV_INDEX);
        }
        else
#endif
        {
          tdsrrcsib_update_sib_sleep_schedule_table(TDSSIB_SRV_INDEX);
        }
      }
      /* Other than SIB7 update event, for any other serving cell
      event we should clear the update sibs flag */
      else if( (cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE) &&
               (tdsupdate_sibs_event_pending == TRUE) )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pending UPDATE_SIBS event cleared");
        tdsupdate_sibs_event_pending = FALSE;
      }

      /* Check other timer based SIBs here when they are supported */

      /* Also make note of whether this GET_SPECIFIC_SIBS event
      is for serving or neighbor cells */
      if(cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE)
      {
        tdssib_events_ptr->event[index].for_serving_cell = TRUE;
      }
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
      if((tdssib_events_ptr->event[index].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)&&
         (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_SIB)&&
         (tdssib_events_ptr->event[index].for_serving_cell == TRUE)&&
         (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB7 in cell fach event active");
        REX_ISR_LOCK(&tdssib7_in_fach_active_mutex);
        tdsis_sib7_in_fach_active = TRUE;
        REX_ISR_UNLOCK(&tdssib7_in_fach_active_mutex);
      }
      else
      {
        REX_ISR_LOCK(&tdssib7_in_fach_active_mutex);
        tdsis_sib7_in_fach_active = FALSE;
      REX_ISR_UNLOCK(&tdssib7_in_fach_active_mutex);
      }
#endif
#endif
      break;
   
    default:
      /* Can't happen since this is caught at a higher level function */
      TDSRRC_MSG1(MSG_LEGACY_MED,"Unknown SIB event %d",cmd_ptr->cmd_hdr.cmd_id);
      break;

  }/* End switch */
}/* end function tdsrrcsib_create_new_sib_event */


/*===========================================================================

FUNCTION RRCSIB_CLEAR_PROCEDURE

DESCRIPTION

  This function clears existing events. It sends confirmation
  commands if needed.
    
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None. 
===========================================================================*/
static void tdsrrcsib_clear_procedure
(  
  tdsrrc_sib_status_e_type failure_cause /* Cause of failure. */
)
{
  tdssib_events_index_e_type count; /* Counter used in loop */

  /* Check if any event is active. If so cancel it */
  for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
      /* Post-process the failed event */
      tdsrrcsib_post_process_failed_event
        ( count, /* Event Index                 */
          TRUE,  /* Confirm Command is required */
          failure_cause, /* Failure Cause       */
          TRUE   /* Tear down S_BCCH if it's up */
        );
    }
  }
}



/*===========================================================================

FUNCTION BCCH_RECONFIGURE_FOR_NEW_SIB_EVENT

DESCRIPTION

  This function is called when a new sib event is
  received while a sib event is in progress. This may
  require a reconfiguration of the BCCH, This function
  resolves the conflict between the events and does the 
  necessary BCCH configurations.
  
  This is how the event conflicts are handled :
  
  A) Existing event is for S-BCCH, new event is for N-BCCH
  
  Tear down S-BCCH, Setup N-BCCH (Both in the same command to LLC).
  In case the existing event is TDSRRCSIB_UPDATE_SIBS event, flag
  it as a pending event so that we can resume it when needed. Only
  if we don't move to a new cell we will resume it. In case the 
  existing event is a GET_SPECIFIC_SIBS event for getting SIB7
  we assume that SIB7 is current for now, since N-BCCH event is 
  considered higher priority.
  
  
  B) Existing event is for N-BCCH, new event is for N-BCCH
  
  Tear down N-BCCH, Setup other N-BCCH (Both in same cmd to LLC).
  
     
  C) Existing event is N-BCCH, new event is S-BCCH
  
  If the new event is TDSRRCSIB_UPDATE_SIBS event, flag it as
  a pending event. No LLC commands are sent. In case the 
  existing event is a GET_SPECIFIC_SIBS event for getting SIB7
  we assume that SIB7 is current for now, since N-BCCH event is 
  considered higher priority. No other S-BCCH event is possible
  when N-BCCH event is in progress.
  
    
  D) Existing event is S-BCCH, new event is S-BCCH
  
  Only 2 S-BCCH events can have conflict : A GET_SPECIFIC_SIBS
  event to update SIB7 and a UPDATE_SIBS event to update sibs
  based on value-tags. In Idle State the GET_SPECIFIC_SIBS event
  to update SIB7 takes precedence since it needs to be read quickly
  for RACH attempts. In CELL_FACH however, since SIB7 updates happen
  every 320 ms, we cannot consider them higher priority than the
  UPDATE_SIBS event (UPDATE_SIBS event may never get done). We
  will postpone the SIB7 event.  

  No other S-BCCH event conflict can happen at this time.
  
DEPENDENCIES
 
  None.

RETURN VALUE
 
  tdsrrcsib_bcch_setup_status_e_type indicating status of
  BCCH setup.

SIDE EFFECTS

  BCCH logical channel may be setup, teared down or modified to a new
  configuration.
       
===========================================================================*/
static tdsrrcsib_bcch_setup_status_e_type tdsrrcsib_bcch_reconfigure_for_new_sib_event
( 
  tdsrrc_cmd_type* ptr, /* Pointer to the command for the new event */
  boolean srv_cell,  /* TRUE if new command is for serving cell  */
  tdssib_events_index_e_type old_index /* Index of the old (existing) event */
)
{
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
  tdsrrc_cmd_type *cmd_ptr;
#endif
  tdsrrcsib_bcch_setup_status_e_type bcch_status
    = TDSRRCSIB_BCCH_SETUP_DONE; /* To hold status */

  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  
  if(TDSEVENT_INDEX_VALID(old_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    bcch_status = TDSRRCSIB_BCCH_SETUP_FAILED;
    return (bcch_status);
  }
  
  /* All serving cell events can be replaced by a neighbor cell
  event. Check if the existing event is a serving cell event. */ 
  if((tdssib_events_ptr->event[old_index].for_serving_cell == TRUE)||
    (tdsrrcllc_get_s_bcch_setup_status()==TRUE))
  {
    /* Check if the new event is for the neighbor cell. */
    if( srv_cell != TRUE )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reconfig: N-BCCH up and S-BCCH down");  

      /* Send an LLC command. Tell it to take the S-BCCH down
      and bring up the N-BCCH for the new event. */
      if(tdsrrcsib_configure_bcch(TDSSBCCH_TEARDOWN, TDSNBCCH_SETUP
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
                ,TDSRRCLLC_CHAN_CFG_REASON_NONE
#endif
           ) == SUCCESS)
      {
        
        /* Copy the other BCCH parameters from the command. For neighbor
        sibs the command type is always "get_specific_sibs_req". */
        tdssib_events_ptr->ngh_bcch.freq = ptr->cmd.get_specific_sibs_req.freq;
        tdssib_events_ptr->ngh_bcch.cpid = ptr->cmd.get_specific_sibs_req.cpid;

        /* Check the existing event type. If it's an TDSRRCSIB_UPDATE_SIBS
        event, flag it as a pending event. */
        if((tdssib_events_ptr->event[old_index].event_active == TRUE) &&
            (tdssib_events_ptr->event[old_index].event_name == TDSRRCSIB_UPDATE_SIBS))
        {
          tdsupdate_sibs_event_pending = TRUE;
          
          //invalidate mib and sb's value tags, as we want to re-read them next time
          tdsrrcsibdb_invalidate_mib_sb_value_tags_for_interrupted_sib_update(tdssibdb.active_cell_index);
        }

        /* If it is a GET_SPECIFIC_SIBS event for updating SIB7, just assume that SIB7 is current 
           since the NBCCH event is higher priority */
        else if( (tdssib_events_ptr->event[old_index].req_proc == TDSRRC_PROCEDURE_SIB) &&
          (tdssib_events_ptr->event[old_index].int_event.int_event_type == 
           TDSRRCSIB_INT_SIB7_EVENT)
          )
        {
          tdsrrcsib_process_int_sib_event_complete( old_index, SUCCESS );
        }
      }

      /* Make the current event inactive */
      tdssib_events_ptr->event[old_index].event_active = FALSE;
      tdssib_events_ptr->event[old_index].sib_scan_type = TDSRRC_SIB_NORMAL;

    }
    else
    {
      if(rrc_state == TDSRRC_STATE_DISCONNECTED)
      {
        /* Check if existing event is SIB7 event and new event is UPDATE_SIBS event. 
           If so the UPDATE_SIBS event can be saved for later. */
        if( (tdssib_events_ptr->event[old_index].req_proc == TDSRRC_PROCEDURE_SIB) &&
            (tdssib_events_ptr->event[old_index].int_event.int_event_type ==
             TDSRRCSIB_INT_SIB7_EVENT) &&
            (ptr->cmd_hdr.cmd_id == TDSRRC_UPDATE_SIBS_IND)
          )
        {
          /* Since UE will go to CONNECTING state after reading SIB7 no need to save
           * update SIBs event as a pending event
           */
          tdsupdate_sibs_event_pending = FALSE;
          /* Indicate to calling function that nothing needs to be
           * done for the new event
           */
          bcch_status = TDSRRCSIB_BCCH_EVENT_SAVED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB7 update in progress. Ignore UPDATE_SIBS event");
        }
        /* Else check if new event is SIB7 event and
         * existing event is UPDATE_SIBS event.
         */
        else if( (ptr->cmd_hdr.cmd_id == TDSRRC_GET_SPECIFIC_SIBS_REQ) &&
          (ptr->cmd.get_specific_sibs_req.procedure == TDSRRC_PROCEDURE_SIB) &&
          (ptr->cmd.get_specific_sibs_req.
             sib_req[tdsrrc_SIB7] == TRUE) &&
          (tdssib_events_ptr->event[old_index].event_name == TDSRRCSIB_UPDATE_SIBS)
               )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Replacing UPDATE_SIBS with SIB7 event");
          /* No need to setup BCCH here since serving BCCH is already
           * setup 
           */
          tdssib_events_ptr->event[old_index].event_name = TDSRRCSIB_GET_SPECIFIC_SIBS;
          tdssib_events_ptr->event[old_index].req_proc = TDSRRC_PROCEDURE_SIB;
          /* Don't flag the existing event as a pending event. */
          tdsupdate_sibs_event_pending = FALSE;
          tdssib_events_ptr->event[old_index].int_event.int_event_type 
            = TDSRRCSIB_INT_SIB7_EVENT;
          /* Indicate to calling function that nothing needs to be
           * done for the new event
           */
          bcch_status = TDSRRCSIB_BCCH_EVENT_SAVED;
        }
      }
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
      else if((rrc_state == TDSRRC_STATE_CELL_PCH) || (rrc_state == TDSRRC_STATE_URA_PCH))
      {
        if( (tdssib_events_ptr->event[old_index].req_proc == TDSRRC_PROCEDURE_SIB) &&
            (tdssib_events_ptr->event[old_index].int_event.int_event_type ==
             TDSRRCSIB_INT_SIB7_EVENT) &&
            (ptr->cmd_hdr.cmd_id == TDSRRC_UPDATE_SIBS_IND)
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB7 update in progress. Set UPDATE_SIBS event as pending");
          tdsupdate_sibs_event_pending = TRUE;
          bcch_status = TDSRRCSIB_BCCH_EVENT_SAVED;
        }
        else if( (ptr->cmd_hdr.cmd_id == TDSRRC_GET_SPECIFIC_SIBS_REQ) &&
          (ptr->cmd.get_specific_sibs_req.procedure == TDSRRC_PROCEDURE_SIB) &&
          (ptr->cmd.get_specific_sibs_req.
             sib_req[tdsrrc_SIB7] == TRUE) &&
          (tdssib_events_ptr->event[old_index].event_name == TDSRRCSIB_UPDATE_SIBS)
               )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"UPDATE_SIBS in progress. Ignore SIB7 update event");
          if((tdssib7info.sib_ext_proc_update_req == TRUE) && (tdssib7info.req_proc == TDSRRC_PROCEDURE_CU))
          {
            /* Set the value back to FASLE */
            tdssib7info.sib_ext_proc_update_req = FALSE;
            if( (cmd_ptr = tdsrrc_get_int_cmd_buf())!=NULL )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_SIB7_UPDATE_STATUS_IND");
              cmd_ptr->cmd_hdr.cmd_id = TDSRRC_SIB7_UPDATE_STATUS_IND;
              cmd_ptr->cmd.sib7_update_status.status = SUCCESS;
              cmd_ptr->cmd.sib7_update_status.procedure = tdssib7info.req_proc;
              tdsrrc_put_int_cmd( cmd_ptr );
            }
            else
            {
              ERR_FATAL("No memory to send cnf event",0,0,0);
            }
           }
          bcch_status = TDSRRCSIB_BCCH_EVENT_SAVED;
        }
      }
#endif
#if (defined(FEATURE_RRC_SIB7_UPDATE_IN_CONNECTING) || defined(FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH))
      else if(rrc_state == TDSRRC_STATE_CONNECTING)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Shouldn't get a S-BCCH reconfig event");
      }
      else if(rrc_state == TDSRRC_STATE_CELL_FACH)
      {
        /* Indicate to calling function that nothing needs to be
         * done for the new event
         */
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
        if((tdssib_events_ptr->event[old_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)&&
           (tdssib_events_ptr->event[old_index].req_proc == TDSRRC_PROCEDURE_SIB)&&
           (tdssib_events_ptr->event[old_index].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib7:Replacing SIB7 event with new event in RRC State FACH");
          bcch_status = TDSRRCSIB_BCCH_SETUP_DONE;
        }
        else
#endif
        {
          bcch_status = TDSRRCSIB_BCCH_EVENT_SAVED;
        }
        /* In CELL_FACH SIB7 updating will happen every
         * 320ms so we cannot hold the UPDATE_SIBS event pending. Consider
         * SIB7 update lower priority in that case.
         */
        tdsrrcsib_process_int_sib_event_complete( old_index, SUCCESS );
      }
#endif
      else
      {
        /* No other new serving cell event can be received when
        another serving cell event is existing. */
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid S-BCCH event rxed! in RRC st %d",rrc_state);
        tdsrrcsib_clear_procedure
          (TDSSIB_EVENT_FAILED_INVALID_EVENT); /* Clear existing procedure to be safe */
        bcch_status = TDSRRCSIB_BCCH_SETUP_FAILED;
      }      
    }/* end if srv_cell != TRUE */
  }/* end if existing event is a srv cell event */
  else
  {
    /* Exisiting event must be a nighbor cell event.
    Check if the new event is for the neighbor cell. */
    if( srv_cell != TRUE )
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Reconfig: N-BCCH up,old N-BCCH down");  

      /* Make the current event inactive */
      tdssib_events_ptr->event[old_index].event_active = FALSE;

      /* Clear the scan type */
      tdssib_events_ptr->event[old_index].sib_scan_type = TDSRRC_SIB_NORMAL;

      /* Send an LLC command. Tell it to take the S-BCCH down
      and bring up the N-BCCH for the new event. */

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
      tdsno_mib_bar_cell_count = 0;
#endif

      /*Initialize tsib optimization data base*/
      tdsrrcsib_clear_tsib_optimization_db();
      //setup nbcch on top of old one.. llc implicitly releases old one
      if(tdsrrcsib_configure_bcch(TDSSBCCH_NOOP, TDSNBCCH_SETUP
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
                ,TDSRRCLLC_CHAN_CFG_REASON_NONE
#endif
            )== SUCCESS)
      {
        /* Copy the other BCCH parameters from the command */
        tdssib_events_ptr->ngh_bcch.freq = ptr->cmd.get_specific_sibs_req.freq;
        tdssib_events_ptr->ngh_bcch.cpid = ptr->cmd.get_specific_sibs_req.cpid;
      }
    }
    else
    {
      /* The new event is a serving cell event. Check if
      it's an TDSRRCSIB_UPDATE_SIBS event. */
      if( ptr->cmd_hdr.cmd_id == TDSRRC_UPDATE_SIBS_IND )
      {
        /* Flag it as a pending event. Don't do anything else
        now */
        tdsupdate_sibs_event_pending = TRUE;
        /* Indicate to calling function that nothing needs to be
        done for the new event */
        bcch_status = TDSRRCSIB_BCCH_EVENT_SAVED;
      }
      /* Else check if it is a GET_SPECIFIC_SIBS event for updating SIB7.
      If so, assume that SIB7 is current since the NBCCH event is higher 
      priority */
      else if( (ptr->cmd_hdr.cmd_id == TDSRRC_GET_SPECIFIC_SIBS_REQ)&&
        (ptr->cmd.get_specific_sibs_req.procedure == TDSRRC_PROCEDURE_SIB) &&
        (ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE)&&
        (ptr->cmd.get_specific_sibs_req.sib_req[tdsrrc_SIB7]
          == TRUE)
        )
      {
        /* SIB7 update can be based on timer or external procedure request.
        Check for both and process accordingly */
        if(tdssib7info.sib_ext_proc_update_req == TRUE)
        {
          tdsrrc_cmd_type *cnf_ptr;
          /* Set the value back to FASLE */
          tdssib7info.sib_ext_proc_update_req = FALSE;
          if( (cnf_ptr = tdsrrc_get_int_cmd_buf())!=NULL )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending TDSRRC_SIB7_UPDATE_STATUS_IND");
            cnf_ptr->cmd_hdr.cmd_id = TDSRRC_SIB7_UPDATE_STATUS_IND;
            cnf_ptr->cmd.sib7_update_status.status = SUCCESS;
            cnf_ptr->cmd.sib7_update_status.procedure = tdssib7info.req_proc;
            tdsrrc_put_int_cmd( cnf_ptr );
          }
          else
          {
            ERR_FATAL("No memory to send cnf event",0,0,0);
          }
        }        /* Indicate to calling function that nothing needs to be
        done for the new event */
        bcch_status = TDSRRCSIB_BCCH_EVENT_SAVED;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid S-BCCH event received!");
        tdsrrcsib_clear_procedure
          (TDSSIB_EVENT_FAILED_INVALID_EVENT); /* Clear existing events to be safe */
        bcch_status = TDSRRCSIB_BCCH_SETUP_FAILED;
      }
    }/* end if srv_cell != TRUE */
  }/* end if existing event is N-BCCH event */

  return( bcch_status );
}/* end function tdsrrcsib_bcch_reconfigure_for_new_sib_event */

/*===========================================================================

FUNCTION SIB_BCCH_SETUP_REQ

DESCRIPTION

  This function requests the LLC module to setup a BCCH logical
  channel for receiving SIBs. All BCCH parameters related to
  the SIB events are stored by this function.
  
  Only one serving cell BCCH or one neighbor cell BCCH can
  be up at the same time. This module checks if a
  BCCH is already up for the requested PLMN and cell id and
  then sets up the BCCH if necessary.

DEPENDENCIES

  None.
 
RETURN VALUE

  tdsrrcsib_bcch_setup_status_e_type indicating status of
  BCCH setup.

SIDE EFFECTS

  Previous BCCH setups may need to be teared down. TBD.

===========================================================================*/
static tdsrrcsib_bcch_setup_status_e_type tdsrrcsib_bcch_setup_req
(
  tdsrrc_cmd_type *cmd_ptr          /* Received command */
)
{

  boolean srv_cell=FALSE; /* To note if the command is for the
                          serving cell or not. */
  boolean setup_error_occured = FALSE; /* Indicate if an error occured
                          for BCCH setup */

  /* Find out if the command is for the serving cell or the neighbour
  cell */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_GET_ALL_SIBS_REQ:
    
      /* TDSRRC_GET_ALL_SIBS_REQ is always for serving cell */
      srv_cell = TRUE;
      break;

    case TDSRRC_UPDATE_SIBS_IND:
    
      /* TDSRRC_UPDATE_SIBS_IND is always for serving cell */
      srv_cell = TRUE;
      break;

    case TDSRRC_GET_SPECIFIC_SIBS_REQ:

      if( cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE )
      {
        srv_cell = TRUE;
      }
      else
      {
        srv_cell = FALSE;
      }
      break;

    default:

      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown SIB command %d",cmd_ptr->cmd_hdr.cmd_id); 
      setup_error_occured = TRUE;
      break;
  }/* End switch */

  if( setup_error_occured == TRUE )
  {
    return(TDSRRCSIB_BCCH_SETUP_FAILED);
  }

  /* Before doing any BCCH setup,
  check if any event is already in progress.
  If so we need to resolve the conflict between the existing
  and new events. We can only handle one at a time. This
  may also require a BCCH reconfig. Call the function to
  handle this. */

TDSRRC_MSG2(MSG_LEGACY_HIGH," Serving cell event %d and LLC S-BCCH setup status %d ",tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_active ,tdsrrcllc_get_s_bcch_setup_status());

  if((tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_active == TRUE) || 
     ((tdsrrcllc_get_s_bcch_setup_status()==TRUE) && 
     (cmd_ptr->cmd_hdr.cmd_id == TDSRRC_GET_SPECIFIC_SIBS_REQ) && 
     (cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == FALSE) && 
     (cmd_ptr->cmd.get_specific_sibs_req.procedure == TDSRRC_PROCEDURE_CSP)))
  {
      /* There is an event active! */
      return(tdsrrcsib_bcch_reconfigure_for_new_sib_event(cmd_ptr, srv_cell, TDSSIB_SRV_INDEX));
      /* Only one event is active at a time so
          no need to continue the for loop if we found
          one active event. */
  }
  else if(tdssib_events_ptr->event[TDSSIB_NGH_INDEX].event_active == TRUE)
  {
        /* There is an event active! */
      return(tdsrrcsib_bcch_reconfigure_for_new_sib_event(cmd_ptr, srv_cell, TDSSIB_NGH_INDEX));
        /* Only one event is active at a time so
        no need to continue the for loop if we found
        one active event. */
  }

  /* If we get here, a BCCH setup is yet to be done.
  Check if the required BCCH is already up. This is done
  by calling the logical channel manager. */
  if( srv_cell == TRUE )
  {
    /* Call the LCM module */
    if ( (tdsrrclcm_check_dl_rlc_lc_id( UE_LOGCHAN_BCCH,
    BCCH_S_RADIO_BEARER_ID,
    UE_MODE_TRANSPARENT ))
    != TDSRRCLCM_RLC_LC_ID_NOT_FOUND)
    {
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
      if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_GET_ALL_SIBS_REQ)
          ||((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)&&
               (TDSRRC_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_SIB))
              )
        )
#else
      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_GET_ALL_SIBS_REQ)
#endif
      {
        /*In RRC State FACH, LLC will keep always BCCH up, 
          so if LC i.d found, avoid sending a set up request to LLC */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH already setup for srv cell");
        tdssib_events_ptr->srv_bcch.bcch_action =  TDSSIB_BCCH_SETUP_COMPLETE;
        return(TDSRRCSIB_BCCH_SETUP_DONE);
      }
      /* BCCH is already up. No need to tear it down and
      setup a new one. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH needs to be setup for srv cell");
      
    }
  }
  else
  {
    /* Call the LCM module */
    if( (tdsrrclcm_check_dl_rlc_lc_id( UE_LOGCHAN_BCCH,
                                    BCCH_N_RADIO_BEARER_ID,
                                    UE_MODE_TRANSPARENT ))
        != TDSRRCLCM_RLC_LC_ID_NOT_FOUND )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH already up for ngh cell");
      /* This should never happen since it's caught in
      the tdsrrcsib_bcch_reconfigure_for_new_sib_event function.
      However, there may be a race condition causing
      this? Let's setup the N-BCCH channel anyway. */
    }
  }

  /* If we get here, a BCCH needs to be setup.
  Send a BCCH setup request command to LLC */
  /* Indicate which radio bearer is needed */
  if( srv_cell == TRUE )
  {
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
	tdsrrcllc_chan_cfg_reason_e_type chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_NONE;
#endif

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
    if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_GET_SPECIFIC_SIBS_REQ)
    {
      if((cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE)&&
          (cmd_ptr->cmd.get_specific_sibs_req.procedure == TDSRRC_PROCEDURE_GPS)
        )
      {
        /*If the SIB event active was cell id reading in DCH then set chan 
         *config reason to TDSRRCLLC_CHAN_CFG_REASON_SIB_READ_IN_DCH 
         *which is used by LLC
         */
        chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_SIB_READ_IN_DCH;
      }
    }
#endif
    if(tdsrrcsib_configure_bcch(TDSSBCCH_SETUP, TDSNBCCH_NOOP
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
          ,chan_config_reason
#endif
        ) == FAILURE)
    {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib config bcch failed");
    }
    
    /* Copy the necessary BCCH parameters from the command */
    tdssib_events_ptr->srv_bcch.freq = cmd_ptr->cmd.get_all_sibs_req.freq;
    tdssib_events_ptr->srv_bcch.cpid = cmd_ptr->cmd.get_all_sibs_req.cpid;
  }
  else
  {
    if(tdsrrcsib_configure_bcch(TDSSBCCH_NOOP, TDSNBCCH_SETUP
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
        ,TDSRRCLLC_CHAN_CFG_REASON_NONE
#endif
        ) == FAILURE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib config bcch failed");
    }      
    
    /* Copy the necessary BCCH parameters from the command */
    tdssib_events_ptr->ngh_bcch.freq = cmd_ptr->cmd.get_specific_sibs_req.freq;
    tdssib_events_ptr->ngh_bcch.cpid = cmd_ptr->cmd.get_specific_sibs_req.cpid;
  }

  return(TDSRRCSIB_BCCH_SETUP_DONE);
}/* end function tdsrrcsib_bcch_setup_req */


/*===========================================================================

FUNCTION REGISTER_NEW_SIB_EVENT

DESCRIPTION

  This function registers a new sib event for the SIB processing module.
  It adds the event to the active events list and updates the active
  event database as needed. Then it sets up  or modifies the BCCH 
  as needed for this event.
  
DEPENDENCIES
 
  None.

RETURN VALUE
 
  None.

SIDE EFFECTS

  BCCH logical channel may be setup, teared down or modified to a new
  configuration.
       
===========================================================================*/
static void tdsrrcsib_register_new_sib_event
(
  tdsrrc_cmd_type *cmd_ptr   /* Pointer to received SIB event */
)
{
  tdsrrcsib_bcch_setup_status_e_type bcch_status; /* Status of BCCH
                                               setup */
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
  unsigned long timer_value;
#endif

  uint8 count = 0;

  /* Reset the stored SIB timer value */
  tdsrrcsib_specific_sib_timer_val = 0;

  /* First setup the BCCH channel required for this command.
  This function also checks if there is an existing SIB
  event and if so resolves the conflict between the events. */
  bcch_status = tdsrrcsib_bcch_setup_req( cmd_ptr );
  
  if( bcch_status == TDSRRCSIB_BCCH_SETUP_FAILED )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sib event dropped: BCCH failure");
    return;
  }

  if( bcch_status == TDSRRCSIB_BCCH_EVENT_SAVED )
  {
    /* Nothing to do here, this event won't be processed now */
    return;
  }

  /* Switch on the type of event that needs to be registered */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
    case TDSRRC_GET_ALL_SIBS_REQ: 
      /* Note the event name and the name of the procedure 
      requesting this event - this is used later while sending
      the confirmation command for this event. */
      tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_name = TDSRRCSIB_GET_ALL_SIBS;
      tdssib_events_ptr->event[TDSSIB_SRV_INDEX].req_proc =
        cmd_ptr->cmd.get_all_sibs_req.procedure;

      /* Note that this case can only be for a serving cell */
      tdssib_events_ptr->event[TDSSIB_SRV_INDEX].for_serving_cell = TRUE;

      /* This event has been completed. Call the post-processing
         function to send the event confirmation if needed,
         do the necessary cleanup and teardown the BCCH radio bearer
         if necessary. */
      tdsrrcsib_post_process_completed_event( TDSSIB_SRV_INDEX );

      break;

    case TDSRRC_UPDATE_SIBS_IND:
      /* Initialize the SIB event database for this event.
      Both "GET_ALL_SIBS" and "UPDATE_SIBS" events can be only
      for the serving cell. */
      tdsrrcsib_create_new_sib_event( TDSSIB_SRV_INDEX, cmd_ptr );

      /* For all new SIB events, we start a SIB wait timer. 
      This timer is the maximum time to wait for SIBs in a cell.
      In case of "UPDATE_SIBS_IND" event, the timer is longer
      since we may have to wait one SFN cycle before getting
      the modified MIB. */
      tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER,
                          TDSRRCSIB_MAX_WAIT_TIME_FOR_UPDATE_SIBS );
      
      /* Nothing else to do here. Eventually L1 will complete the
      BCCH setup and we will get SIBs for this event. */
      break;

    case TDSRRC_GET_SPECIFIC_SIBS_REQ:      
      /* Check if this event is for the serving cell or neighbor
      cell, and initializethe SIB event database for this event. */
      if( cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE )
      {
        /* Create the event using the serving cell index */
        tdsrrcsib_create_new_sib_event( TDSSIB_SRV_INDEX, cmd_ptr );
      }
      else
      {
        /* Create the event using the neighbor cell index */
        tdsrrcsib_create_new_sib_event( TDSSIB_NGH_INDEX, cmd_ptr );
      }
      /* For all new SIB events, we start a SIB wait timer. 
      This timer is the maximum time to wait for SIBs in a cell.
      If the timer expires before the event is completed, a failure
      status is sent to the calling procedure. Start this timer
      right now  for BPLMN/N-Setup,  timer of only 300ms is set*/
      if( (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN) || 
          (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN) || 
          (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) || 
          (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN) ||
          (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN))
      {
        tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS);
        tdsrrcsib_specific_sib_timer_val = TDSRRCSIB_BPLMN_MIB_WAIT_TIMER;
      }
	    /* For td2td manual bplmn search, shorten the mib wait time to 160ms */
	    else if(cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)
	    {
	      tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS);
         tdsrrcsib_specific_sib_timer_val = TDSRRCSIB_MANUAL_BPLMN_MIB_WAIT_TIMER_IN_MS;
	    }
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      else if(cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
      {
        /*For sib_scan_type TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN, start sib wiat timer to 
         *640msec for now, one we receive mib this will be re-started based on sib-3 ret rate */
        tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS);
        tdsrrcsib_specific_sib_timer_val = TDSRRCSIB_MAX_SIB3_WAIT_TIMER;
      }
#endif
      else if(cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == FALSE)
      {
        tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS);
        tdsrrcsib_specific_sib_timer_val = TDSRRCSIB_NBCCH_MIB_WAIT_TIMER;
      }
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
      else if((cmd_ptr->cmd.get_specific_sibs_req.procedure== TDSRRC_PROCEDURE_SIB)&&
             ((tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH)||(tdsrrc_get_state() == TDSRRC_STATE_URA_PCH))&&
             (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)
             )
      {
        timer_value = tdsrrcsib_get_sib7_exp_timer_value();
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Starting SIB 7 timer %d",timer_value);
        tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, 
                         TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS );
        tdsrrcsib_specific_sib_timer_val = timer_value;
      }
#endif
      else
      {
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
        if((cmd_ptr->cmd.get_specific_sibs_req.procedure== TDSRRC_PROCEDURE_SIB)&&
            (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)&&
            (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)
          )
        {/*sib-7 event will be kept ON always in FACH, 
           So no need to run asib wait timer for this event*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib7:This is SIB7 update event in FACH,do not start SIB wait timer");
        }
        else
#endif
        {
          tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS);
          tdsrrcsib_specific_sib_timer_val = TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS;

          //if(tdsrrcsib_g_in_data_call)//Always start TDSRRCTMR_SIB_STOP_PARTITION_TIMER
          {
            tdsrrctmr_start_timer( TDSRRCTMR_SIB_STOP_PARTITION_TIMER, 6000);
          }
        }
      }
      /* If the search is for TD2TD BPLMN,initialize the bplmn sibs variables */
      if((cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN)||
         (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
         (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_SCAN)||
         (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)||
         (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN) ||
         (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN)
         )
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received RRC_TD2TD_BPLMN_SCAN/MANUAL_SCAN/DUMMY_PLMN_SCAN request.Scan type %d",cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type);
        
        /* Free and initialize the bplmns sibs */
        tdsrrcsib_free_bplmn_sibs();
      }

      /*Freeing decoded SIBs if GET_SPECIFIC_SIB_REQ for serving cell
             is received without camping after successful SIB reading, in order
             to avoid exhaustion of decoded contexts*/
      if(((cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_NORMAL)
#ifdef FEATURE_3GPP_CSFB_TDS
         || (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_READ_FROM_SIB_CONTAINER)
#endif
         ) && (cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE) && 
         (cmd_ptr->cmd.get_specific_sibs_req.procedure == TDSRRC_PROCEDURE_CSP))
      {
        for(count = 0; count <(uint8)TDSSIBDB_INVALID_INDEX; count++)
        {
          if(tdssibdb.cell[count].index_used == TRUE)
          {
            TDSRRC_PRINT_SIB_HEAP();
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free ASN.1 decoded sib's for sibdb idx= %d",count);
            tdsrrcsibdb_free_all_decodedsibs_for_a_cell(count);
            TDSRRC_PRINT_SIB_HEAP();
          }
        }
      }    
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR," Unknown SIB event registered %x",cmd_ptr->cmd_hdr.cmd_id);  
      break;
  }
}/* end function tdsrrcsib_register_new_sib_event */

/*===========================================================================

FUNCTION  UPDATE_EVENT_FOR_RECEIVED_VALUE_TAGS

DESCRIPTION

  This function updates a SIB event based on value-tag
  comparisons for the received value-tags. The value-tags
  may be from a MIB, a Scheduling Block 1 or a Scheduling
  Block 2.

DEPENDENCIES

  None.
  
RETURN VALUE
  
  None.

SIDE EFFECTS

  None.

===========================================================================*/
static void tdsrrcsib_update_event_for_received_value_tags
(
  tdsrrc_SIB_type sib,     /* Type of SIB - can be MIB, SB1 or SB2   */
  tdssib_events_index_e_type event_index, /* Index for the SIB event */
  uint32 db_index       /* SIB Database index for the stored SIBs */
)
{
  /* Declare a variable to store the results from value tag
  comparisons */
  tdsrrc_sibdb_value_tag_results_type result;
  tdsrrc_SysInfoTypeSB1         *sb1 = NULL;
  tdsrrc_SysInfoTypeSB2         *sb2 = NULL;
  boolean check_results=FALSE; /* Indicate if results need
  to be checked - only set to true if SIBs have already
  been stored in the database for this cell */

  uint32 count;  /* Local counters */

  tdsrrcsib_vtag_match_type tpye_of_vtag_match;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  /* Initialize result to keep Lint happy. This will be filled
  in by the tdssibdb module when comparing value-tags */
  for( count=0; count<TDSRRC_TOT_SIBS; count++ )
  {
    result.result[count] = TDSVALUE_TAG_NOT_PRESENT;
  }

  /* Switch on the sib type to compare value-tags. */
  switch( sib )
  {
    case tdsrrc_MIB:

     /* Value-tags need to be compared with tags already
     in the database for two reasons -
      
     1) If the current cell already has matching SIBs
     stored in the database due to a previous camp-on
     at this cell, we have to compare
     the received value tags. The SIB database has
     a variable "last_plmn_and_cell_id_matched to
     indicate if this is the case.
      
     2) If we've camped on a cell for a while and
     we get indication that the MIB valuetags have
     changed, then we need to compare the new MIB's
     value-tags with the saved MIB's. */

     if( ((tdssibdb.last_plmn_and_cell_id_matched == TRUE) ||
         (tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_UPDATE_SIBS ))
       )
     {
       /* Call the SIB Database function that does value-tag
       comparisons */
       if((tdsrrcsib_is_event_sixhrs_sib_update(event_index) == TRUE)||
           (tdsrrcsib_is_sixhrs_sib_timer_expired(event_index) == TRUE)
         )
       {
         tpye_of_vtag_match = TDSRRCSIB_READ_ALL_SIBS;
       }
       else
       {
         tpye_of_vtag_match = TDSRRCSIB_READ_SIBS_AFTER_VTAG_MATCH;
       }

       tdsrrcsibdb_compare_value_tags_for_cell
         ( db_index,
           sib,
           ((void *)tdssib_events_ptr->event[event_index].tempholder.mib),
           (&result)
       /*Need a flag to indicate forcibly re-read all sibs, only if the event is 
         UPDATE_SIBs & six hr timer expired*/
          ,
            tpye_of_vtag_match
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          , event_index
#endif
         );
       check_results=TRUE;

       /* whether mib value tag matches or not, always set the SIB7 result to 
          value-tag-not-matched..  this is needed, because:
          * if MIB-Value-TAG matches SIB-DB declares that all SIB's are updated          
          * but, SIB7 does not have a value-tag and hence cannot be update
  
          this is done for:
          * GET_ALL_SIBS always since we need updated SIB7
          * GET_SPECIFIC_SIBS in CELL-FACH to take care of cell-reselections in FACH
  
        */
       if(((tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
           (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)) ||
          ((tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_ALL_SIBS) &&
           tdsrrc_get_state() != TDSRRC_STATE_DISCONNECTED))
       {
         if(tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB7].sib_received == TRUE)
         {
           result.result[tdsrrc_SIB7] = TDSTIMER_BASED_SIB_RCVD;
         }
         else
         {
           result.result[tdsrrc_SIB7] = TDSVALUE_TAG_DID_NOT_MATCH;
         }
       }
     }
     break;

    case tdsrrc_SB1:
  
      /* Value tags from Scheduling Block 1 can be compared
      with the stored sibs for all sib events. Call the SIB 
      Database function that does value-tag comparisons*/

      if((tdsrrcsib_is_event_sixhrs_sib_update(event_index) == TRUE)||
           (tdsrrcsib_is_sixhrs_sib_timer_expired(event_index) == TRUE)
         )
      {
        tpye_of_vtag_match = TDSRRCSIB_READ_ALL_SIBS;
      }
      else
      {
        tpye_of_vtag_match = TDSRRCSIB_READ_SIBS_AFTER_VTAG_MATCH;
      }

      tdsrrcsibdb_compare_value_tags_for_cell
        ( db_index,
          sib,
          ((void *)tdssibdb.cell[db_index].sibs.sb1),
          (&result)
          /*Need a flag to indicate forcibly re-read all sibs, only if the event is 
            UPDATE_SIBs & six hr timer expired*/
          ,
            tpye_of_vtag_match
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          , event_index
#endif
        );

      check_results=TRUE; 
      break;

    case tdsrrc_SB2:

      /* Value tags from Scheduling Block 2 can be compared
      with the stored sibs for all sib events. Call the SIB 
      Database function that does value-tag comparisons*/

      if((tdsrrcsib_is_event_sixhrs_sib_update(event_index) == TRUE)||
           (tdsrrcsib_is_sixhrs_sib_timer_expired(event_index) == TRUE)
         )
      {
        tpye_of_vtag_match = TDSRRCSIB_READ_ALL_SIBS;
      }
      else
      {
        tpye_of_vtag_match = TDSRRCSIB_READ_SIBS_AFTER_VTAG_MATCH;
      }

      tdsrrcsibdb_compare_value_tags_for_cell
        ( db_index,
          sib,
          ((void *)tdssibdb.cell[db_index].sibs.sb2),
          (&result)
          /*Need a flag to indicate forcibly re-read all sibs, only if the event is 
            UPDATE_SIBs & six hr timer expired*/
          ,
            tpye_of_vtag_match
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          , event_index
#endif
        );

      check_results=TRUE;
      break;

    default:
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid type for value-tag comparison %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib));
      break;
  }/* end switch */

  if( check_results == TRUE )
  {
    /* Now, check the results from the value-tag comparison.
    If any of the value-tags match, we already have th current
    SIBs for this cell and don't need to read them again. Do
    this only for SIBs that are not received yet. */
    for( count=0; count<TDSRRC_TOT_SIBS; count++ )
    {

      if((result.result[count] == TDSVALUE_TAG_DID_NOT_MATCH)
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
       ||(result.result[count] == TDSDEFERRED_SIB_RCVD)
#endif
        )
      {
        if(tdssupported_sibs_lookup_table[count] == FALSE)
        {
          tdssib_events_ptr->event[event_index].tsib[count].sib_needed = FALSE;
        }
        else
        {
          tdssib_events_ptr->event[event_index].tsib[count].sib_needed = TRUE;
        }
      }

      /* If a SIB is needed for this event and has not been
      decoded, check if we have a matching value-tag */
      if( (tdssib_events_ptr->event[event_index].tsib[count].sib_needed) &&
          (!(tdssib_events_ptr->event[event_index].tsib[count].sib_decoded))
        )
      {
        if( result.result[count] ==  TDSVALUE_TAG_MATCHED )
        {
#ifdef FEATURE_RRC_SIB_DEBUG
          if ( tdsrrc_MIB == (tdsrrc_SIB_type)count )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB %d is current: Value Tag Matched",tdsrrcsib_map_int_sibtype_to_asn1_sibtype((tdsrrc_SIB_type)count));
          }  
#endif /* FEATURE_RRC_SIB_DEBUG */
          /* The value-tag matches! we already have this SIB 
          in the database. No need to receive or decode this SIB. */
          tdssib_events_ptr->event[event_index].tsib[count].sib_received = TRUE;
          tdssib_events_ptr->event[event_index].tsib[count].sib_decoded = TRUE;

          //if value tags match for Scheduling Blocks for SIB's already read, then
          //update the value tags from the stored SIBs.. also check to see if SB's are
          //already not decoded before MIB/SB1/SB2 are decoded
          if(((uint32)tdsrrc_SB1== count) && 
             (tdssib_events_ptr->event[event_index].tempholder.sb1_decoded==FALSE))
          {
            sb1 = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB1,db_index);
            if(sb1 != NULL)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updating Sched from SIBDB SB1");
              tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_SB1, 
                                                   (void *) sb1, event_index);
            }
          
            /* Since SB1 value tag matched and we have taken care of updating schedule present
               in SB1 from SIB DB,set this flag to true */
            tdssib_events_ptr->event[event_index].tempholder.sb1_decoded = TRUE;
          }

          if(((uint32)tdsrrc_SB2== count) && 
             (tdssib_events_ptr->event[event_index].tempholder.sb2_decoded==FALSE))
          {
            sb2= tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB2,db_index);
            if(sb2!= NULL)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Updating Sched from SIBDB SB2");
              tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_SB2, 
                                                   (void *) sb2, event_index);
            }

            /* Since SB2 value tag matched and we have taken care of updating schedule present
               in SB2 from SIB DB,set this flag to true */
            tdssib_events_ptr->event[event_index].tempholder.sb2_decoded = TRUE;
          }
        }
        else if( result.result[count] == TDSVALUE_TAG_DID_NOT_MATCH)
        {
#ifdef FEATURE_RRC_SIB_DEBUG
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Value tag changed! read SIB %d again",tdsrrcsib_map_int_sibtype_to_asn1_sibtype((tdsrrc_SIB_type)count));
#endif /* FEATURE_RRC_SIB_DEBUG */
        }
        else if(TDSTIMER_BASED_SIB_RCVD == result.result[count])
        {
          /*Timer based SIB's have no value tag.. so if the SIB has already
            been received for this TDSEVENT, treat it as received, but not
            decoded yet..  If the SIB is not even received, then it is
            treated as v_tag did not match and read again */
          tdssib_events_ptr->event[event_index].tsib[count].sib_received = TRUE;
          tdssib_events_ptr->event[event_index].tsib[count].sib_decoded = FALSE;
        }
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
        else if( result.result[count] == TDSDEFERRED_SIB_RCVD)
        {
          if(tdssib_events_ptr->event[event_index].tsib[count].sib_received)
          {
            tdssib_events_ptr->event[event_index].tsib[count].sib_decoded = FALSE;
          }
        }
#endif
      }

      /* Otherwise, if a SIB is needed for this event and has been
      decoded, if we have a non-matching value-tag we may have to
      read the SIB again. */
      else if( (tdssib_events_ptr->event[event_index].tsib[count].sib_needed) &&
               (tdssib_events_ptr->event[event_index].tsib[count].sib_decoded)
             )
      {
        if((result.result[count] == TDSVALUE_TAG_DID_NOT_MATCH)
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
              ||(result.result[count] == TDSDEFERRED_SIB_RCVD)
#endif
           )

        {
#ifdef FEATURE_RRC_SIB_DEBUG
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Value tag changed! read SIB %d again",tdsrrcsib_map_int_sibtype_to_asn1_sibtype((tdsrrc_SIB_type)count));
#endif /* FEATURE_RRC_SIB_DEBUG */
            /*Reset all the variables for this SIB so that we can try to
            read it again */
            tdssib_events_ptr->event[event_index].tsib[count].sib_received = FALSE;
            tdssib_events_ptr->event[event_index].tsib[count].sib_decoded = FALSE;
            tdssib_events_ptr->event[event_index].tsib[count].seg_count = 0;
            tdssib_events_ptr->event[event_index].tsib[count].seg_index = 
              (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
            tdssib_events_ptr->event[event_index].tsib[count].sib_tot_length = 0;
        }
      }
    }/* end for */
  }
}/* end function tdsrrcsib_update_event_for_received_value_tags */

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
  tdsrrc_SystemInformation_BCH * msg_ptr
  ,tdssib_events_index_e_type index
)
{
  uint8 seg_index[TDSMAX_SEGMENTS_PER_BCCH_BLOCK];
  uint32 sib_type_asn1[TDSMAX_SEGMENTS_PER_BCCH_BLOCK] = {0}; // Zero initialization done to suppress Lint error.
  uint32 rrc_sib_type[TDSMAX_SEGMENTS_PER_BCCH_BLOCK] = {0}; // Zero initialization done to suppress Lint error.
  uint32 count=0, i = 0;
  boolean status = FALSE;
  uint8 j = 0;
  boolean sched_info_not_received = FALSE;
  uint32 n_value=0;
  tdsrrc_CompleteSIB_List *local_complete_SIB_List;
  OSRTDListNode *temp_list_ptr = NULL;
  tdsrrc_CompleteSIBshort *temp_node_ptr = NULL;
  
  /*Initialisations have been done to suppress LINT error*/
  uint32 sfn_prime[TDSMAX_SEGMENTS_PER_BCCH_BLOCK] = {0};
  tdssib_segment_e_type seg_type[TDSMAX_SEGMENTS_PER_BCCH_BLOCK] = {tdsnoSegment_chosen};
  uint32 seg_length[TDSMAX_SEGMENTS_PER_BCCH_BLOCK] = {0};
  unsigned char* value[TDSMAX_SEGMENTS_PER_BCCH_BLOCK] = {0};
  TDSseg_proc_stat_e_type seg_status = TDSSEG_PROC_SUCCEEDED;

  //sfn-Prime is always a multiple of 2.. left shift once to get the actual sfn
  uint32 off_set = (msg_ptr->sfn_Prime << 1);

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }
#endif

  //here if mib rep_rate is invalid, return false as we won't have scheduling info for any other
  //sibs, if this SIB is the first segment of a MIB, process it and check set MIB scheduling info
  if(TDSINVALID_SIB_SCHEDULE_REP == tdssib_events_ptr->sib_sched[sib_type_asn1[tdsrrc_MIB]].repitition_rate)
  {
    //if MIB is not yet read and the current SIB does not start from a SFN-Prime of Multiple of 8, then
    //return right away, since MIB is not read and also the current SIB is not a MIB
    if (((off_set) & 0x7) != 0)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIB not rcvd yet. Offset is %d",off_set);
      status=FALSE;
      return status;
    }
    else
    {
      //MIB scheduling is hardcoded by spec.  Rep-Rate is 8, subsequent segments are at an offset of 2,
      //hence we have this hardcoded values seen below.
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"First seg of MIB rcvd. Init MIB-Sched");
      tdssib_events_ptr->sib_sched[rrc_sib_type[tdsrrc_MIB]].repitition_rate = 8;
      tdssib_events_ptr->sib_sched[rrc_sib_type[tdsrrc_MIB]].sfn_offset[0] =0;
      tdssib_events_ptr->sib_sched[rrc_sib_type[tdsrrc_MIB]].sfn_offset[1] =2;
      tdssib_events_ptr->sib_sched[rrc_sib_type[tdsrrc_MIB]].sfn_offset[2] =4;
      tdssib_events_ptr->sib_sched[rrc_sib_type[tdsrrc_MIB]].sfn_offset[3] =6;
    }
  }

  //reset the flags that checks to see if all SIB's are received and decoded
  memset (seg_index, 0x0, TDSMAX_SEGMENTS_PER_BCCH_BLOCK);
  memset (sib_type_asn1, 0x0, TDSMAX_SEGMENTS_PER_BCCH_BLOCK);
  memset (rrc_sib_type, 0x0, TDSMAX_SEGMENTS_PER_BCCH_BLOCK);
  
  switch(TDSRRCGET_SIB_TYPE_AND_TAG((&msg_ptr->payload)))
  {
    //no sensible segment, do nothing but return from here..
    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(noSegment):
      status = FALSE;
      break;

    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(firstSegment):
      seg_index[count] = 0;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdsfirstSegment_chosen;
      seg_length[count] = msg_ptr->payload.u.firstSegment.sib_Data_fixed.numbits;
      value[count] = msg_ptr->payload.u.firstSegment.sib_Data_fixed.data;

      sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.firstSegment.sib_Type;
      break;

    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(subsequentSegment):
      seg_index[count] = msg_ptr->payload.u.subsequentSegment.segmentIndex;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdssubsequentSegment_chosen;
      seg_length[count] = msg_ptr->payload.u.subsequentSegment.sib_Data_fixed.numbits;
      value[count] = msg_ptr->payload.u.subsequentSegment.sib_Data_fixed.data;

      sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.subsequentSegment.sib_Type;
      break;

    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(lastSegmentShort):
      seg_index[count] =  msg_ptr->payload.u.lastSegmentShort.segmentIndex;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdslastSegmentShort_chosen;
      seg_length[count] = msg_ptr->payload.u.lastSegmentShort.sib_Data_variable.numbits;
      value[count] = msg_ptr->payload.u.lastSegmentShort.sib_Data_variable.data;

      sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.lastSegmentShort.sib_Type;
      break;

    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(lastAndFirst):
      seg_index[count] = 0;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdsfirstSegment_chosen;
      seg_length[count] = msg_ptr->payload.u.lastAndFirst.firstSegment.sib_Data_variable.numbits;
      value[count] = msg_ptr->payload.u.lastAndFirst.firstSegment.sib_Data_variable.data;

      sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.lastAndFirst.firstSegment.sib_Type;

      seg_index[count] = msg_ptr->payload.u.lastAndFirst.lastSegmentShort.segmentIndex;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdslastSegmentShort_chosen;
      seg_length[count] = msg_ptr->payload.u.lastAndFirst.lastSegmentShort.sib_Data_variable.numbits;
      value[count] = msg_ptr->payload.u.lastAndFirst.lastSegmentShort.sib_Data_variable.data;

      sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.lastAndFirst.lastSegmentShort.sib_Type;
      break;

    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(lastAndComplete):

      seg_index[count] = msg_ptr->payload.u.lastAndComplete.lastSegmentShort.segmentIndex;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdslastSegmentShort_chosen;
      seg_length[count] = msg_ptr->payload.u.lastAndComplete.lastSegmentShort.sib_Data_variable.numbits;
      value[count] = msg_ptr->payload.u.lastAndComplete.lastSegmentShort.sib_Data_variable.data;

      sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.lastAndComplete.lastSegmentShort.sib_Type;
      

      local_complete_SIB_List = &(msg_ptr->payload.u.lastAndComplete.completeSIB_List);
      n_value = local_complete_SIB_List->count;
      temp_list_ptr = local_complete_SIB_List->head;

      while(n_value != 0)
      {
        seg_index[count] = 0;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdscompleteSIB_chosen;
        temp_node_ptr = (tdsrrc_CompleteSIBshort*)temp_list_ptr->data;
        if(temp_node_ptr == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB lastAndComplete received");
          return FALSE;
        }
        seg_length[count] = temp_node_ptr->sib_Data_variable.numbits;
        value[count] = temp_node_ptr->sib_Data_variable.data;

        sib_type_asn1[count++] = (uint32)temp_node_ptr->sib_Type;

        temp_list_ptr = temp_list_ptr->next;
        n_value--;
      }
      break;

   case TDSRRCGET_SystemInformation_BCH_payload_TYPE(lastAndCompleteAndFirst):
      
     seg_index[count] = msg_ptr->payload.u.lastAndCompleteAndFirst.lastSegmentShort.segmentIndex;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdslastSegmentShort_chosen;
      seg_length[count] = msg_ptr->payload.u.lastAndCompleteAndFirst.lastSegmentShort.sib_Data_variable.numbits;
      value[count] = msg_ptr->payload.u.lastAndCompleteAndFirst.lastSegmentShort.sib_Data_variable.data;

     sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.lastAndCompleteAndFirst.lastSegmentShort.sib_Type;

     seg_index[count] = 0;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdsfirstSegment_chosen;
      seg_length[count] = msg_ptr->payload.u.lastAndCompleteAndFirst.firstSegment.sib_Data_variable.numbits;
      value[count] = msg_ptr->payload.u.lastAndCompleteAndFirst.firstSegment.sib_Data_variable.data;

     sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.lastAndCompleteAndFirst.firstSegment.sib_Type;
      
  
     local_complete_SIB_List = &(msg_ptr->payload.u.lastAndCompleteAndFirst.completeSIB_List);
     n_value = local_complete_SIB_List->count;
     temp_list_ptr = local_complete_SIB_List->head;
  
     while(n_value != 0)
     {
       seg_index[count] = 0;  

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdscompleteSIB_chosen;
      temp_node_ptr = (tdsrrc_CompleteSIBshort*)temp_list_ptr->data;
      if(temp_node_ptr == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB lastAndCompleteAndFirst received");
        return FALSE;
      }
      seg_length[count] = temp_node_ptr->sib_Data_variable.numbits;
      value[count] = temp_node_ptr->sib_Data_variable.data;

       sib_type_asn1[count++] = (uint32)temp_node_ptr->sib_Type;  
        
       temp_list_ptr = temp_list_ptr->next;
       n_value--;
     }
     break;

    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(completeSIB_List):
      
      local_complete_SIB_List = &(msg_ptr->payload.u.completeSIB_List);
      n_value = local_complete_SIB_List->count;
      temp_list_ptr = local_complete_SIB_List->head;

      while(n_value != 0)
      {
        seg_index[count] = 0;

        sfn_prime[count] = msg_ptr->sfn_Prime;
        seg_type[count]= tdscompleteSIB_chosen;
        temp_node_ptr = (tdsrrc_CompleteSIBshort*)temp_list_ptr->data;
        if(temp_node_ptr == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB completeSIB_List received");
          return FALSE;
        }
        seg_length[count] = temp_node_ptr->sib_Data_variable.numbits;
        value[count] = temp_node_ptr->sib_Data_variable.data;

        sib_type_asn1[count++] = (uint32)temp_node_ptr->sib_Type;

        temp_list_ptr = temp_list_ptr->next;
        n_value--;
      }
      break;


    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(completeAndFirst):
      seg_index[count] = 0;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdsfirstSegment_chosen;
      seg_length[count] = msg_ptr->payload.u.completeAndFirst.firstSegment.sib_Data_variable.numbits;
      value[count] = msg_ptr->payload.u.completeAndFirst.firstSegment.sib_Data_variable.data;

      sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.completeAndFirst.firstSegment.sib_Type;

      local_complete_SIB_List = &(msg_ptr->payload.u.completeAndFirst.completeSIB_List);
      n_value = local_complete_SIB_List->count;
      temp_list_ptr = local_complete_SIB_List->head;

      while(n_value != 0)
      {
        seg_index[count] = 0;

        sfn_prime[count] = msg_ptr->sfn_Prime;
        seg_type[count]= tdscompleteSIB_chosen;
        temp_node_ptr = (tdsrrc_CompleteSIBshort*)temp_list_ptr->data;
        if(temp_node_ptr == NULL)
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB completeAndFirst received");
          return FALSE;
        }
        seg_length[count] = temp_node_ptr->sib_Data_variable.numbits;
        value[count] = temp_node_ptr->sib_Data_variable.data;

        sib_type_asn1[count++] = (uint32)temp_node_ptr->sib_Type;

        temp_list_ptr = temp_list_ptr->next;
        n_value--;
      }
      break;

    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(completeSIB):
      seg_index[count] = 0;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdscompleteSIB_chosen;
      seg_length[count] = msg_ptr->payload.u.completeSIB.sib_Data_fixed.numbits;
      value[count] = msg_ptr->payload.u.completeSIB.sib_Data_fixed.data;  

      sib_type_asn1[count++] =(uint32) msg_ptr->payload.u.completeSIB.sib_Type;      
      break;

    case TDSRRCGET_SystemInformation_BCH_payload_TYPE(lastSegment):
      seg_index[count] = msg_ptr->payload.u.lastSegment.segmentIndex;

      sfn_prime[count] = msg_ptr->sfn_Prime;
      seg_type[count]= tdslastSegment_chosen;
      seg_length[count] = msg_ptr->payload.u.lastSegment.sib_Data_fixed.numbits;
      value[count] = msg_ptr->payload.u.lastSegment.sib_Data_fixed.data;

      sib_type_asn1[count++] = (uint32)msg_ptr->payload.u.lastSegment.sib_Type;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown seg-type %d",TDSRRCGET_SIB_TYPE_AND_TAG((&msg_ptr->payload)));
      break;
  }

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
/*If the UE does not find the master information block in a position fulfilling:
     SFN mod 32 = 0 
(Note: These are controlled by TDSMAX_TIME_MIB_WAIT &TDSMIB_POSITION)
but a transport block with correct CRC was found at that position), the UE shall:
1>  consider the master information block as not found; and
1>  consider the cell to be barred and
1>  consider the barred cell as using the value "allowed" in the 
IE "Intra-frequency cell re-selection indicator",and the maximum value in the IE "Tbarred".
Note: This feature we are restricting to cell reselection only for now.
*/
  if(tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received != TRUE)
  {
    if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)&&
       (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_CSP)&&
       (tdssib_events_ptr->event[index].for_serving_cell == FALSE)
      )
    {
      off_set = (msg_ptr->sfn_Prime << 1);
      if((off_set%TDSMIB_POSITION) == 0)
      {
        boolean mib_found = FALSE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB position lets,check whethere MIB is received");
        for(i=0;i<count;i++)
        {
          if(sib_type_asn1[i] == (uint32)TDSRRC_SIB_TYPE(masterInformationBlock))
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB found");
            mib_found = TRUE;
            break;
          }
        }
        if(mib_found == FALSE)
        {
          tdsno_mib_bar_cell_count++;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"MIB not found at MIB position,count=%d",tdsno_mib_bar_cell_count);
          if(tdsno_mib_bar_cell_count == TDSMAX_TIME_MIB_WAIT)
          {
            tdsno_mib_bar_cell_count = 0;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Baring this cell as MIB not found at SFN%32");
            //bar this cell right here
            tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, 1280);
            /* An event was active. Since the timer expired,
            this event failed. */
            tdsrrcsib_post_process_failed_event
            ( index, /* Event Index                 */
              TRUE,  /* Confirm Command is required */
              TDSSIB_EVENT_FAILED_TIMEOUT, /* Timed out*/
              TRUE   /* Tear down S_BCCH if it's up */
            );
            status = FALSE;
            return status;
          }
        }
      }
    }
  }
#endif

  //loop thru the list of sib segments received and compare to see if the
  //sfn-prime at which it is received matches the sib-schedule earlier
  //received in MIB/SB1/SB2.. if it matches, then return true, else return 
  //false
  for(i=0;i<count;i++)
  {
    if(sib_type_asn1[i] != TDSRRC_SIB_TYPE(extensionType))
    {
      rrc_sib_type[i] = tdsrrcsib_map_asn1_sibtype_to_int_sibtype((tdsrrc_SIB_Type)sib_type_asn1[i]);
      if(rrc_sib_type[i] == tdsrrc_MAX_SIB)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupported SIB");
        continue;
      }
      if(rrc_sib_type[i] == (uint32)tdsrrc_SIB5bis)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Treating SIB5bis as SIB5 for checking SIB Schedule");
        rrc_sib_type[i] = (uint32)tdsrrc_SIB5;
      }
      if(tdssib_events_ptr->sib_sched[rrc_sib_type[i]].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
      {
        if(tdssib_events_ptr->sib_sched[rrc_sib_type[i]].repitition_rate == 0)
        {
          ERR_FATAL("Potential Div by zero",0,0,0);
        }
  
        if(tdssib_events_ptr->sib_sched[rrc_sib_type[i]].sfn_offset[seg_index[i]] == 
           (off_set % tdssib_events_ptr->sib_sched[rrc_sib_type[i]].repitition_rate))
        {
          status = TRUE;
  
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Sib %d, rep_rate %d seg_idx %d", tdsrrcsib_map_int_sibtype_to_asn1_sibtype((tdsrrc_SIB_type)rrc_sib_type[i]), 
                    tdssib_events_ptr->sib_sched[rrc_sib_type[i]].repitition_rate, seg_index[i]);
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Msg Offset %d,Seg-Offset %d",off_set,tdssib_events_ptr->sib_sched[rrc_sib_type[i]].sfn_offset[seg_index[i]]);
        }
        else
        {
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Sib_type %d, seg_idx %d, offset %d", tdsrrcsib_map_int_sibtype_to_asn1_sibtype((tdsrrc_SIB_type)rrc_sib_type[i]), seg_index[i], off_set);
        }
      }
      else
      {
  
        if(tdssupported_sibs_lookup_table[rrc_sib_type[i]] == TRUE)
        {
          if(tdssib_events_ptr->sib_sched[rrc_sib_type[i]].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP)
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TSIB - SIB segment received before scheduling information");
            TDSRRC_MSG3(MSG_LEGACY_HIGH," SIB %d, seg_idx %d, offset %d",sib_type_asn1[i],seg_index[i],off_set);
            seg_status = tdsrrcsib_store_sib_seg_in_tsib_optim_db((tdsrrc_SIB_type)rrc_sib_type[i],
                                                  sfn_prime[i],
                                                  seg_index[i],
                                                  seg_type[i],
                                                  seg_length[i],
                                                  (void *)value[i],
                                                  index);
            if(seg_status == TDSSEG_PROC_STORED)
            {
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"TSIB - Segment saved for SIB %d",sib_type_asn1[i]);
            }
            else
            {
              /*Simply set this to TDSSEG_PROC_FAILED once more as 
              tdsrrcsib_store_sib_seg_in_tsib_optim_db() would have taken care of*/
              seg_status = TDSSEG_PROC_FAILED;
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"TSIB - Failed to store segment for SIB %d",sib_type_asn1[i]);
            }
          }
        }
      }
    }
    else
    {
      for(j = tdsrrc_SIB11bis;j < tdsrrc_ExtensionType;j++)
      {
        if(tdssupported_sibs_lookup_table[j] == TRUE)
        {
          if(tdssib_events_ptr->sib_sched[j].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
          {
            if(tdssib_events_ptr->sib_sched[j].repitition_rate == 0)
            {
              ERR_FATAL("Potential Div by zero",0,0,0);
            }
            if(tdssib_events_ptr->sib_sched[j].sfn_offset[seg_index[i]] == 
               (off_set % tdssib_events_ptr->sib_sched[j].repitition_rate))
            {
              status = TRUE;
              sched_info_not_received = FALSE;
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"Extension Sib ,rep_rate %d seg_idx %d",tdssib_events_ptr->sib_sched[rrc_sib_type[i]].repitition_rate,seg_index[i]);
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"Msg Offset %d,Seg-Offset %d",off_set,tdssib_events_ptr->sib_sched[rrc_sib_type[i]].sfn_offset[seg_index[i]]);
              break;
            }
          }
          else
          {
            sched_info_not_received = TRUE;
          }
        }
      }
      if(sched_info_not_received == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TSIB - SIB segment received before scheduling information");
        TDSRRC_MSG2(MSG_LEGACY_HIGH," Extension SIB , seg_idx %d,offset %d",seg_index[i],off_set);
        seg_status = tdsrrcsib_store_sib_seg_in_tsib_optim_db(tdsrrc_ExtensionType,
                                              sfn_prime[i],
                                              seg_index[i],
                                              seg_type[i],
                                              seg_length[i],
                                              (void *)value[i],
                                              index);
        if(seg_status == TDSSEG_PROC_STORED)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TSIB - Segment saved for extension SIB");
        }
        else
        {
          /*Simply set this to TDSSEG_PROC_FAILED once more as 
          tdsrrcsib_store_sib_seg_in_tsib_optim_db() would have taken care of*/
          seg_status = TDSSEG_PROC_FAILED;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TSIB - Failed to store segment for Extension SIB");
        }
      }
      else if(status == FALSE)
      {
        TDSRRC_MSG2(MSG_LEGACY_ERROR,"Extension Sib , seg_idx %d,offset %d",seg_index[i],off_set);
      }
    }
  }

  return status;
}

/*===========================================================================

FUNCTION tdsrrcsib_decode_and_save_sib_for_current_event

DESCRIPTION

  This function checks checks the sib_db for a particular sib, deletes the old
  one if present.  Then decodes the new sib for the on-going event.
  
  
DEPENDENCIES
 
  Assumes that SIB_DB_INDEX is already found for this event

RETURN VALUE
 Boolean: True, if decoding successful, else false

SIDE EFFECTS

  None.
        
===========================================================================*/
static boolean tdsrrcsib_decode_and_save_sib_for_current_event
(
  void ** sib_ptr,
  tdssib_events_index_e_type event_index,
  uint32 db_index,
  tdsrrc_SIB_type rrc_sib_type,
  uint32 pdu_num,
  byte *sib_encoded_ptr,
  uint32 *sib_encoded_pdu_length
)
{
  uint32 pdu_length;
  boolean status = FALSE;
  boolean conn_mode_sibs_present = FALSE;
  tdsrrc_SysInfoType1 *sib1_ptr = NULL;

  /* If the pointer is already being used, free it */

  boolean sib7_decoded = FALSE;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }

  if(rrc_sib_type >= tdsrrc_ExtensionType)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Type");
    return FALSE;
  }

  if((rrc_sib_type == tdsrrc_SIB7)&&
    (tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_decoded == TRUE)
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Duplicate copy of SIB7 to be decoded");
    sib7_decoded = tdsrrcsib_decode_duplicate_sib7(event_index, pdu_num);
    if(sib7_decoded == FALSE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Decoding of duplicate SIB7 failed. Retaining the SIB7 stored in SIB DB");
      return TRUE;
    }
  }
  
  if( *sib_ptr != NULL )
  {
    tdsrrcsibdb_free_sib( (int)pdu_num,
                        (void *)*sib_ptr );
    *sib_ptr=NULL;
  }

  if((rrc_sib_type == tdsrrc_SIB3)||
    (rrc_sib_type == tdsrrc_SIB5)||
    (rrc_sib_type == tdsrrc_SIB11)||
    (rrc_sib_type == tdsrrc_SIB5bis))
  {
    conn_mode_sibs_present = tdsrrcsib_check_if_connected_mode_sibs_present
      (tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_data_ptr[0], 
      rrc_sib_type);
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"conn_mode_sibs_present = %d in sib = %d",conn_mode_sibs_present,rrc_sib_type);
  }

  /* Get the PDU length in bytes */
  pdu_length =  TDSGET_PDU_LENGTH_IN_BYTES( 
    tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_tot_length );

  /* ASN.1 decode the PDU */
  *sib_ptr =  tdsrrcasn1_decode_pdu( 
    (void *)(tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_data_ptr), /* Encoded SIB      */
    (int)pdu_num,                                                       /* SIB's PDU number */
    pdu_length                                                     /* SIB's Length in bytes */
    );
  
  if(*sib_ptr == NULL )
  {
    /* The decoding failed */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"ASN1 Decoding failed for %d",rrc_sib_type);
    {

      /*Reset all the variables for this SIB so that we can try to
      read it again */
      tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_received = FALSE;
      tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_decoded = FALSE;
      tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].seg_count = 0;
      tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].seg_index = 
      (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
      tdssib_events_ptr->event[event_index].tsib[tdsrrc_SB1].sib_tot_length = 0;

      status = FALSE;
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_MED,"SIB_HEAP:Free sib_encoded_ptr,val=%d",sib_encoded_ptr);
      memset(sib_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
      *sib_encoded_pdu_length = TDSINVALID_PDU_LENGTH;

    TDSRRC_MSG0(MSG_LEGACY_MED,"SIB_HEAP:Store encoded sib content in tdssibdb");
    TDSRRC_PRINT_SIB_HEAP();
/*lint -e545*/
    memscpy(sib_encoded_ptr,TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
        &(tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_data_ptr),
        pdu_length);
/*lint +e545*/
    *sib_encoded_pdu_length = pdu_length ;
     TDSRRC_MSG2(MSG_LEGACY_MED,"SIB_HEAP:Saved Sib type=%d,len=%d(bytes) in encoded form",rrc_sib_type,*sib_encoded_pdu_length);

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
    if(tdsrrcsib_is_current_event_sib7_in_fach() == TRUE)
    {
      /*For SIB7 event after receiving SIB7, init the book keeping 
        info for sib7 in event table which will make the same sib7 
        event to ready for receiving another sib7 on BCCH*/
      if(rrc_sib_type == tdsrrc_SIB7)
      {
        tdssibdb.cell[db_index].sib_valid[rrc_sib_type] = TRUE;
        /*Re-starting SIB-7 event*/
        tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_received = FALSE;
        tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_decoded = FALSE;
        tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].seg_count = 0;
        tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].seg_index = 
          (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
        tdssib_events_ptr->event[event_index].tsib[tdsrrc_SB1].sib_tot_length = 0;
      
        status = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib7:Preparing SIB7 event to receive another sib7,tdssibdb valid=%d",tdssibdb.cell[db_index].sib_valid[rrc_sib_type]);

        /* If the current event is SIB7 and UE is in CELL_FACH, we need to extract
           persistence value from SIB7 and pass it on to MAC */
        if(tdsrrcsib_is_current_event_sib7_in_fach() == TRUE)
        {
          tdsrrc_SysInfoType7    *sib7_ptr = 
            (tdsrrc_SysInfoType7 *)tdsrrcsibdb_return_sib_for_srv_cell(tdsrrc_SIB7);
  
          if(NULL != sib7_ptr)
          {
            if (SUCCESS != tdsrrcllc_update_dpl_from_sib7_info_in_fach(sib7_ptr))
            {  
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not send TDSMAC_UL_PERSISTENCE_VAL_CHANGE_IND to MAC");
            }
          }        
        }
      }
    }
    else
#endif
    {
#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
      if((rrc_sib_type == tdsrrc_SIB11bis) ||
         (rrc_sib_type == tdsrrc_SIB19)
        )
      {
        tdsrrc_log_dl_sig_message_for_ext_sibs(rrc_sib_type, event_index);
      }
#endif
      /* Indicate that the PDU has been decoded. */
      tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_decoded = TRUE;

      /* Indicate in the sib database that this sib is now valid */
      tdssibdb.cell[db_index].sib_valid[rrc_sib_type] = TRUE;
      if(rrc_sib_type == tdsrrc_SIB7)
      {
          tdssib_events_ptr->event[event_index].tsib[rrc_sib_type].sib_tot_length = 0;
      }

      /* set default values for idle timers and constants in case IE IdleTimersAndConstants is absent in SIB1 */
      sib1_ptr = (tdsrrc_SysInfoType1 *)(*sib_ptr);
	  
      if((rrc_sib_type == tdsrrc_SIB1) &&
         (!sib1_ptr->m.ue_IdleTimersAndConstantsPresent))
      {
          sib1_ptr->m.ue_IdleTimersAndConstantsPresent = 1;
          sib1_ptr->ue_IdleTimersAndConstants.t_300 = tdsrrc_T_300_ms1000;
          sib1_ptr->ue_IdleTimersAndConstants.n_300 = 3;
          sib1_ptr->ue_IdleTimersAndConstants.t_312 = 1;
          sib1_ptr->ue_IdleTimersAndConstants.n_312 = 1;	  
      }

      status = TRUE;
    }

/*If FEATURE_TDSCDMA_G2T_TUNE_AWAY is enable, it will send specific schedule info to L1 for serving cell or Nbr cell  
   So don't need this part of code any more*/
  }

  return status;
}
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
/*===========================================================================

FUNCTION tdsrrcsib_check_if_scheduling_info_received_for_extended_sibs

DESCRIPTION

  This function walks thru the MIB/SB1/SB2 and checks to see if scheduling
  information is received for  extended sib's.  
DEPENDENCIES
 
  None.

RETURN VALUE

  SUCCESS: If scheduling information is received for extended sib's
  FAILURE: If scheduling information is not received for extended sib's

SIDE EFFECTS
  
===========================================================================*/
boolean tdsrrcsib_check_if_scheduling_info_received_for_extended_sibs
(
  void * sib_ptr,           /* ptr to mib/sb1/sb2                     */
  tdsrrc_SIB_type pdu_num,     /* Type of SIB - can be MIB, SB1 or SB2   */
  uint32 index
)
{
  tdsrrc_MasterInformationBlock *mib_ptr; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr;         /* Local pointer to SB2       */
  boolean status = FALSE;
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List *ext_siblist_ptr = NULL;
  uint32 n_index=0;

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    switch(pdu_num)
    {
      //if the updating pdu is MIB, get the sib type and later update the
      //scheduling information
      case tdsrrc_MIB:
        mib_ptr = (tdsrrc_MasterInformationBlock *)sib_ptr;
        if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_MIB(mib_ptr))
        {
          ext_siblist_ptr = &mib_ptr->v690NonCriticalExtensions.
            v6b0NonCriticalExtensions.masterInformationBlock_v6b0ext.extSIBTypeInfoSchedulingInfo_List;
        }
        break;
        
      //if the updating pdu is SB1 or SB2, get the sib type and later update the
      //scheduling information
      case tdsrrc_SB1:
      case tdsrrc_SB2:
        if(TRUE == tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_MIB])
        {
          if( pdu_num == tdsrrc_SB1 )
          {
            sb1_ptr = (tdsrrc_SysInfoTypeSB1 *)sib_ptr;
            if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(sb1_ptr,
                 tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1))
            {
              ext_siblist_ptr = &sb1_ptr->v6b0NonCriticalExtensions.
            sysInfoTypeSB1_v6b0ext.extSIBTypeInfoSchedulingInfo_List;
            }
          }
          else if( pdu_num == tdsrrc_SB2 ) 
          {
            sb2_ptr = (tdsrrc_SysInfoTypeSB2 *)sib_ptr;
            if(TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(sb2_ptr,
                 tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2))
            {
              ext_siblist_ptr = &sb2_ptr->v6b0NonCriticalExtensions.
            sysInfoTypeSB2_v6b0ext.extSIBTypeInfoSchedulingInfo_List;
            }
          }
        }
        //first MIB has to be decoded, until then do not process sb's
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB not decoded first");
        }
        break;
     
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Sib-Sched Type %d",pdu_num);
        break;
    }
   
    if(ext_siblist_ptr != NULL)
    { 
      n_index = 0;
      while( ext_siblist_ptr->n > n_index)
      {
        if( TDSRRCCHECK_SIB_TypeExt(
              (&(ext_siblist_ptr->elem[n_index].extensionSIB_Type)),
               systemInfoType11bis)
        )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:Sched present for sib11bis ");
          status = TRUE;
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:Extension sib type = %x not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[n_index].extensionSIB_Type)));
        }
        n_index++;
    }
  }
  else
  {
    status = FALSE;
  }
}
  
  return status;
}
#endif

#ifdef FEATURE_TDSCDMA_SIB19
/*===========================================================================

FUNCTION rrcsib_check_if_scheduling_info_received_for_sib19

DESCRIPTION

  This function walks thru the MIB/SB1/SB2 and checks to see if scheduling
  information is received for SIB19.  
DEPENDENCIES
 
  None.

RETURN VALUE

  SUCCESS: If scheduling information is received for SIB19
  FAILURE: If scheduling information is not received for SIB19

SIDE EFFECTS
  
===========================================================================*/
boolean tdsrrcsib_check_if_scheduling_info_received_for_sib19
(
  void * sib_ptr,           /* ptr to mib/sb1/sb2                     */
  tdsrrc_SIB_type pdu_num,     /* Type of SIB - can be MIB, SB1 or SB2   */
  uint32 index
)
{
  tdsrrc_MasterInformationBlock *mib_ptr; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr;         /* Local pointer to SB2       */
  boolean status = FALSE;
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List2 *ext_siblist2_ptr = NULL;
  uint32 n_index=0;

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    switch(pdu_num)
    {
      //if the updating pdu is MIB, get the sib type and later update the
      //scheduling information
      case tdsrrc_MIB:
        mib_ptr = (tdsrrc_MasterInformationBlock *)sib_ptr;
        if((mib_ptr->m.v690NonCriticalExtensionsPresent) &&
           (mib_ptr->v690NonCriticalExtensions.m.v6b0NonCriticalExtensionsPresent) &&
           (mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
           (mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.v860NonCriticalExtensions.
             masterInformationBlock_v860ext.m.extSIBTypeInfoSchedulingInfo_ListPresent))
        {
          ext_siblist2_ptr = &mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.
            v860NonCriticalExtensions.masterInformationBlock_v860ext.extSIBTypeInfoSchedulingInfo_List;
        }
        break;
        
      //if the updating pdu is SB1 or SB2, get the sib type and later update the
      //scheduling information
      case tdsrrc_SB1:
      case tdsrrc_SB2:
        if(TRUE == tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_MIB])
        {
          if( pdu_num == tdsrrc_SB1 )
          {
            sb1_ptr = (tdsrrc_SysInfoTypeSB1 *)sib_ptr;
            if((sb1_ptr->m.v6b0NonCriticalExtensionsPresent) &&
               (sb1_ptr->v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
               (sb1_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.sysInfoTypeSB1_v860ext.
                   m.extSIBTypeInfoSchedulingInfo_ListPresent))
            {
              ext_siblist2_ptr = &sb1_ptr->v6b0NonCriticalExtensions.
                v860NonCriticalExtensions.sysInfoTypeSB1_v860ext.extSIBTypeInfoSchedulingInfo_List;
            }
          }
          else if( pdu_num == tdsrrc_SB2 ) 
          {
            sb2_ptr = (tdsrrc_SysInfoTypeSB2 *)sib_ptr;
            if((sb2_ptr->m.v6b0NonCriticalExtensionsPresent) &&
               (sb2_ptr->v6b0NonCriticalExtensions.m.v860NonCriticalExtensionsPresent) &&
               (sb2_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
                  sysInfoTypeSB2_v860ext.m.extSIBTypeInfoSchedulingInfo_ListPresent))
            {
              ext_siblist2_ptr = &sb2_ptr->v6b0NonCriticalExtensions.
            v860NonCriticalExtensions.sysInfoTypeSB2_v860ext.extSIBTypeInfoSchedulingInfo_List;
            }
          }
        }
        //first MIB has to be decoded, until then do not process sb's
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB not decoded first");
        }
        break;
     
      default:
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Sib-Sched Type %d",pdu_num);
        break;
    }
    
    if(ext_siblist2_ptr != NULL)
    {
      n_index = 0;
      while(ext_siblist2_ptr->n > n_index)
      {
        if(ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t &
            T_tdsrrc_SIB_TypeExt2_systemInfoType19)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:Sched present for sib19 ");
          status = TRUE;
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:Extension sib type = %x not supported",ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t);
        }
        n_index++;
      }
    }
    else
    {
      status = FALSE;
    }
  }
  else
  {
    status = FALSE;
  }
  
  return status;
}
#endif


/*===========================================================================

FUNCTION tdsrrcsib_check_if_scheduling_info_received_for_all_sibs

DESCRIPTION

  This function walks thru the MIB and checks to see if scheduling
  information is received for all mandatory sib's.  
DEPENDENCIES
 
  None.

RETURN VALUE

  SUCCESS: If scheduling information is received for all mandatory sib's
  FAILURE: If scheduling information is not received for all mandatory sib's

SIDE EFFECTS
  
===========================================================================*/
static uecomdef_status_e_type tdsrrcsib_check_if_scheduling_info_received_for_all_sibs
(
  void * sib_ptr,           /* ptr to mib/sb1/sb2                     */
  tdsrrc_SIB_type pdu_num,     /* Type of SIB - can be MIB, SB1 or SB2   */
  tdssib_events_index_e_type index
)
{
  tdsrrc_MasterInformationBlock *mib_ptr; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr;         /* Local pointer to SB2       */

  tdsrrc_SIBSb_ReferenceList *sibSb_ptr = NULL; /* Local pointer to SIBSb ref list */
  tdsrrc_SIB_ReferenceList *sb_ptr=NULL;        /* Local pointer to SIB ref list   */

  tdsrrcsib_sib_type_vtag sib_type_tag;
  boolean sb1_found, sb2_found;      /* Local flags to keep track of sb's  */
  uint32 n_index=0, n_value=0;

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FAILURE;
  }

  switch(pdu_num)
  {
    //if the updating pdu is MIB, get the sib type and later update the
    //scheduling information
    case tdsrrc_MIB:
      mib_ptr = (tdsrrc_MasterInformationBlock *)sib_ptr;

      sibSb_ptr = &(mib_ptr->sibSb_ReferenceList);
  
      //set to true for MIB, as no scheduling info is received for mib
      tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_MIB] = TRUE;

      n_index = 0;
      n_value = sibSb_ptr->n;
      
      while( n_value != 0 )
      {
        tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag(&sibSb_ptr->elem[n_index].sibSb_Type,
                                                    &sib_type_tag);
        if(sib_type_tag.sib_type != tdsrrc_MAX_SIB)
        {
          tdssib_events_ptr->event[index].sched_present_for_sibs[sib_type_tag.sib_type] = TRUE;
        }
        n_value--;
        n_index++;
      }
      break;

    //if the updating pdu is SB1 or SB2, get the sib type and later update the
    //scheduling information
    case tdsrrc_SB1:
    case tdsrrc_SB2:

      if(TRUE == tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_MIB])
      {
        if( pdu_num == tdsrrc_SB1 )
        {
          sb1_ptr = (tdsrrc_SysInfoTypeSB1 *)sib_ptr;
          sb_ptr = &(sb1_ptr->sib_ReferenceList);
        }
        else if( pdu_num == tdsrrc_SB2 ) 
        {
          sb2_ptr = (tdsrrc_SysInfoTypeSB2 *)sib_ptr;
          sb_ptr = &(sb2_ptr->sib_ReferenceList);
        }

        n_index = 0;
        n_value = sb_ptr->n;

        while( n_value != 0 )
        {

          tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&sb_ptr->elem[n_index].sib_Type,
                                                      &sib_type_tag);

          if(sib_type_tag.sib_type != tdsrrc_MAX_SIB)
          {
            tdssib_events_ptr->event[index].sched_present_for_sibs[sib_type_tag.sib_type] = TRUE;
          }
          n_value--;
          n_index++;
        }
      }
      //first MIB has to be decoded, until then do not process sb's
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB not decoded first");
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Sib-Sched Type %d",pdu_num);
      break;
  }

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if(tdsrrcsib_check_if_scheduling_info_received_for_extended_sibs(
               sib_ptr,pdu_num,index) == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:Sched present for sib11bis");
        tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SIB11bis] = TRUE;
      }
#endif

#ifdef FEATURE_TDSCDMA_SIB19
  if(tdsrrcsib_check_if_scheduling_info_received_for_sib19(
           sib_ptr,pdu_num,index) == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:Sched present for extended sib");
    tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SIB19] = TRUE;
  }
#endif

  //check to see if MIB is received first, else don't check to see if all SIB's are received
  if(TRUE == tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_MIB])
  {
    //if MIB is received, check if SB1 is scheduled or not, if scheduled, then check if
    //received.. if not scheduled then indicate that sb1 is found.. do similar stuff for
    //sb2 as below
    if(tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SB1] == TRUE)
    {
      if(TRUE == tdssib_events_ptr->event[index].tempholder.sb1_decoded)
      {
        sb1_found = TRUE;
      }
      else
      {
        sb1_found = FALSE;
      }
    }
    else
    {
      sb1_found = TRUE;
    }

    if(TRUE == tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SB2])
    {
      if(TRUE == tdssib_events_ptr->event[index].tempholder.sb2_decoded)
      {
        sb2_found = TRUE;
      }
      else
      {
        sb2_found = FALSE;
      }
    }
    else
    {
      sb2_found = TRUE;
    }

    if(sb1_found && sb2_found)
    {
      if(!tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SIB1] ||
         !tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SIB3] ||
         !tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SIB5] ||
         !tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SIB7])
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Sched for mand. sibs not present");
        return FAILURE;
      }
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB not decoded first");
  }

  return SUCCESS;

}
/*===========================================================================

FUNCTION RRCSIB_PROCESS_SIB_SCHEDULING_INFO_INCOMPLETE

DESCRIPTION

  This function does the necessary processing when 
  the SIB we don't receiving scheduling information
  for all the SIB's that are mandatorily needed to
  camp on

DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  BCCH logical channels may be teared down. 
===========================================================================*/
static void tdsrrcsib_process_sib_scheduling_info_incomplete( tdssib_events_index_e_type event_index )
{
  tdssib_events_index_e_type count = event_index;          /* Local counter */

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sched-Info for mand. SIB's not rcvd!");

  tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, 1280);

  /* Check if any event is active */
  for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
      /* An event was active. Since the timer expired,
      this event failed. */
      tdsrrcsib_post_process_failed_event
        ( count, /* Event Index                 */
          TRUE,  /* Confirm Command is required */
          TDSSIB_EVENT_FAILED_TIMEOUT, /* Timed out*/
          TRUE   /* Tear down S_BCCH if it's up */
        );
      break; /* Out of the for loop since only one event is active
             at a time. */
    }/* End if active events */
  }/* End for */
}/* End tdsrrcsib_process_sib_scheduling_info_incomplete */

/*============================================================================
FUNCTION: tdsrrcllcsib_validate_t_barred_criteria_for_pich_info

DESCRIPTION:
  This validates SCCPCH info from SIB5/6 and checks to see if it meets
  T-Barred criteria.

DEPENDENCIES:
  None.

RETURN VALUE:
SUCCESS: Indicates that Cell contains atleast PICH info hence don't bar it
FAILURE: Indicates that none of SCCPCH info contains PICH. Hence bar the cell

SIDE EFFECTS:
  None.
==============================================================================*/
static uecomdef_status_e_type tdsrrcllcsib_validate_t_barred_criteria_for_pich_info
(
  tdsrrc_SCCPCH_SystemInformationList_LCR_r4_ext *sccpch_sys_info_ptr
)
{
  /* local PRACH System Info pointer */
  tdsrrc_SCCPCH_SystemInformationList_LCR_r4_ext*ie_ptr = sccpch_sys_info_ptr;
  uint32 n_index;

  /* count of the number of SCCPCH included */
  uint8                               num_of_sccpch = 1;

  if(ie_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"SCCPCH System Info not present");
    return(FAILURE);
  }


  n_index = 0;
    
  while((ie_ptr->n > n_index) && (num_of_sccpch <= TDSUE_MAX_SCCPCH))
  {

    if(TDSRRC_CHECK_MSG_TYPE_IE(ie_ptr->elem[n_index],
                pich_InfoPresent))
    {
      return SUCCESS;
    }
    
    /* get the next element in the linked list */
    num_of_sccpch++;
    n_index++;
  }

  /* if we come this far, then we did not have a single Prach-Sys-Info-List with AICH info.
     hence this cell is a prime candidate to be barred
   */ 
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"PICH System Info not present");
  return FAILURE;
}

/*===========================================================================

FUNCTION tdsrrcsib_check_pich_barred_criteria

DESCRIPTION

  This function checks sib5/6 t barred criteria by checking to see if PICH info
  is present
  
DEPENDENCIES
 
  None.

RETURN VALUE
 
TRUE:  SIB-5 satisfies T-Barred Criteria
FALSE:  SIB-5 does not satisfy T-Barred Criteria

SIDE EFFECTS

  None.
        
===========================================================================*/
static boolean tdsrrcsib_check_pich_barred_criteria
(
  uint32 dbindex,         /* Index to the sib database. */
  tdsrrc_SIB_type rrc_sib_type   /* whether it is sib 5 or 6   */
)
{

  if(tdsrrc_SIB5 == rrc_sib_type)
  {
    if ((tdssibdb.cell[dbindex].sibs.sib5->m.v4b0NonCriticalExtensionsPresent) &&
        (tdssibdb.cell[dbindex].sibs.sib5->v4b0NonCriticalExtensions.m.sysInfoType5_v4b0extPresent) &&
        (tdssibdb.cell[dbindex].sibs.sib5->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.m.tdd128SpecificInfoPresent))
    {
      if(FAILURE == tdsrrcllcsib_validate_t_barred_criteria_for_pich_info(&(tdssibdb.cell[dbindex].sibs.
                    sib5->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.tdd128SpecificInfo.
                    sCCPCH_LCR_ExtensionsList)))
      {
        return TRUE;
      }
    }
  }
  else if (tdsrrc_SIB6 == rrc_sib_type)
  {
    if((tdssibdb.cell[dbindex].sibs.sib6->m.v4b0NonCriticalExtensionsPresent) &&
        (tdssibdb.cell[dbindex].sibs.sib6->v4b0NonCriticalExtensions.m.sysInfoType6_v4b0extPresent) &&
        (tdssibdb.cell[dbindex].sibs.sib6->v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.m.tdd128SpecificInfoPresent))
    {
      if(FAILURE == tdsrrcllcsib_validate_t_barred_criteria_for_pich_info(&(tdssibdb.cell[dbindex].sibs.sib6->
              v4b0NonCriticalExtensions.sysInfoType6_v4b0ext.tdd128SpecificInfo.sCCPCH_LCR_ExtensionsList)))
      {
        return TRUE;
      }
    }
  }

  return FALSE;
}
/*===========================================================================

FUNCTION rrcsib_check_sib5_sib6_barred_criteria

DESCRIPTION

  This function checks sib5/6 t barred criteria by checking to see if PICH info
  is present
  
DEPENDENCIES
 
  None.

RETURN VALUE
 
SUCCESS:  SIB-5 satisfies T-Barred Criteria
FAILURE:  SIB-5 does not satisfy T-Barred Criteria

SIDE EFFECTS

  None.
        
===========================================================================*/
uecomdef_status_e_type tdsrrcsib_check_sib5_sib6_barred_criteria
(
  uint32 dbindex,                /* Index to the sib database. */
  tdssib_events_index_e_type index  /* Index into the tdssib_events database. */
)
{
  tdsrrc_state_e_type rrc_state;     /* store locally the rrc current state */

  boolean sib5_read = FALSE;      /* initialize sib5 read flag to false  */
  boolean sib6_read = FALSE;      /* initialize sib5 read flag to false  */
  
  boolean cell_barred_status = FALSE;  /* indicates that a cell is barred.. */
  rrc_state = tdsrrc_get_state();

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FAILURE;
  }
  //store local information that sib 5 and sib 6 are needed, received and decoded rather than use
  //long loops everywhere
  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_needed == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_received == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_decoded == TRUE))
  {
     sib5_read = TRUE;
  }

  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB6].sib_needed == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB6].sib_received == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB6].sib_decoded == TRUE))
  {
    sib6_read = TRUE;
  }

  if(FALSE == sib5_read && FALSE == sib6_read)
  {
    //if neither SIB5 nor SIB6 are read, then can't really determine barred criteria
    //return success right here
    return SUCCESS;
  }

  if(sib5_read == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:check & decode Sib5 in tdssibdb");
    if( tdssibdb.cell[dbindex].sibs.sib5== NULL )
    {
      /* ASN.1 decode the PDU */
      tdssibdb.cell[dbindex].sibs.sib5=  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[dbindex].sibs.sib5_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType5bis_PDU,                                                       /* SIB's PDU number */
      tdssibdb.cell[dbindex].sibs.sib5_encoded_pdu_length   /* SIB's Length in bytes */
      );
      if(tdssibdb.cell[dbindex].sibs.sib5== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB5",0,0,0);
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:Decode Sib5 in tdssibdb");
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB5 already decoded,sib5_ptr=%d",tdssibdb.cell[dbindex].sibs.sib5);
    }
  }
  if(sib6_read == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:check & decode Sib6 in tdssibdb");
    if( tdssibdb.cell[dbindex].sibs.sib6== NULL )
    {
      /* ASN.1 decode the PDU */
      tdssibdb.cell[dbindex].sibs.sib6=  tdsrrcasn1_decode_pdu( 
              (void *)(tdssibdb.cell[dbindex].sibs.sib6_encoded_ptr), /* Encoded SIB      */
              tdsrrc_SysInfoType6_PDU,                                /* SIB's PDU number */
              tdssibdb.cell[dbindex].sibs.sib6_encoded_pdu_length   /* SIB's Length in bytes */
              );
      if(tdssibdb.cell[dbindex].sibs.sib6== NULL)
      {
        ERR_FATAL("SIB_HEAP:Asn.1 decode failed from SIB6",0,0,0);
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:Decode Sib6 in tdssibdb");
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB6 already decoded,sib6_ptr=%d",tdssibdb.cell[dbindex].sibs.sib6);
    }
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB5 %d,SIB6 %d read status",sib5_read,sib6_read);

  //we should read SIB5 for connecting too, since the only time we read sib's in connecting is
  //to do cell reselection in connecting state
  if((rrc_state == TDSRRC_STATE_CONNECTING) || (rrc_state == TDSRRC_STATE_DISCONNECTED))
  {
    if(TRUE == sib5_read)
    {
      if(TRUE == tdsrrcsib_check_pich_barred_criteria(dbindex, tdsrrc_SIB5))
      {
        //Call CSP API to indicate this cell as barred to L1 only for normal sib scan..
        /* Indicate in the sib database that this sib is now valid */
        cell_barred_status = TRUE;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB5 does not have PICH info");
      }
    }
  }
  else
  {
    //in connected mode, wait for SIB5 too if SIB6 is already decoded.. bcos SIB5 may have some sibs which
    //are not in SIB6
    if(TRUE == sib6_read && FALSE == sib5_read)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB6 only read in connected mode yet.  Wait for SIB5");
      return SUCCESS;
    }

    //if both sib5 and 6 are read
    if(TRUE == sib5_read  && TRUE == sib6_read)
    {      
      /* Check LCRext present in sib5 */
      if ((tdssibdb.cell[dbindex].sibs.sib5->m.v4b0NonCriticalExtensionsPresent) &&
        (tdssibdb.cell[dbindex].sibs.sib5->v4b0NonCriticalExtensions.m.sysInfoType5_v4b0extPresent) &&
        (tdssibdb.cell[dbindex].sibs.sib5->v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.m.tdd128SpecificInfoPresent))
      //check if pich criteria meets for cell barring
      {
        if (FAILURE == tdsrrcllcsib_validate_t_barred_criteria_for_pich_info(&(tdssibdb.cell[dbindex].sibs.sib5->
            v4b0NonCriticalExtensions.sysInfoType5_v4b0ext.tdd128SpecificInfo.sCCPCH_LCR_ExtensionsList)))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"PICH info not present in SIB5");
          cell_barred_status = TRUE;
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"PICH info not present in SIB5");
        cell_barred_status = TRUE;
      }

      
      /* If PICH info absent in sib6 not consider cell barred, RRC can get info from sib5. */
    }
    else if (TRUE == sib5_read)    //else only sib5 read
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB5 only PICH info not present");
      cell_barred_status = tdsrrcsib_check_pich_barred_criteria(dbindex, tdsrrc_SIB5);
    }
    else if (TRUE == sib6_read)   //else only sib6 read
    {
      /* If PICH info absent in sib6 not consider cell barred, RRC can get info from sib5. */      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"only SIB6 present,not check PICH ");
    }
  }

  //if cell is barred, call csp function
  if(TRUE == cell_barred_status)
  {
    //Call CSP API to indicate this cell as barred to L1 only for normal sib scan..
    /* Indicate in the sib database that this sib is now valid */
    tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_CPID_BARRED, 1280);
    tdsrrcsib_post_process_failed_event(index, TRUE, TDSSIB_EVENT_FAILED_OTHER, TRUE );
    return FAILURE;
  }

  return SUCCESS;
}

/*===========================================================================

FUNCTION DECODE_COMPLETED_SIBS_FOR_BPLMN

DESCRIPTION

  This function checks the tdssib_events database for the given index
  and for each SIB that has been completely received, it calls
  the ASN.1 decode function to decode the SIB and save it in the
  correct SIB database index. Only if the SIB database index is known
  for the received SIBs this function should be called.
  
DEPENDENCIES
 
  None.

RETURN VALUE
 
  None.

SIDE EFFECTS

  None.
        
===========================================================================*/
static boolean tdsrrcsib_decode_completed_sibs_for_bplmn
(
  tdssib_events_index_e_type index  /* Index into the tdssib_events database. */
)
{
  uint32 count; /* local counter */
  boolean ret_val = FALSE;
  uint32 sib1_pdu_length;

#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
  uint32 sb1_pdu_length;
  uint32 sb2_pdu_length;
  tdsrrc_SIB_ReferenceList * sb_ptr = NULL;
  tdsrrcsib_sib_type_vtag sib_type_tag;
#endif
//  uint32 ix=0;
  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }

  /* Look at each SIB to see if it has been completely received
     but not yet decoded. */
  for(count = 0; count < TDSRRC_TOT_SIBS; count++)
  {
    if((tdssib_events_ptr->event[index].tsib[count].sib_needed == TRUE) &&
       (tdssib_events_ptr->event[index].tsib[count].sib_received == TRUE) &&
       (tdssib_events_ptr->event[index].tsib[count].sib_decoded == FALSE)
      )
    {
      /* This is a SIB that is needed and received, but not decoded.
         switch on the SIB type and decode the SIB to the correct
         sib pointer */

      switch( count )
      {
        case tdsrrc_SIB1 :

          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: SIB1 has been received");

          if (tdsrrcsib_bplmn_sibs.sib1 == NULL) 
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: Storing SIB1 in bplmn sibs database");

            /* Get the PDU length in bytes */
            sib1_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                              (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].
                               sib_tot_length);

            /* Decode and store SIB1 in bplmn sib database */
            tdsrrcsib_bplmn_sibs.sib1 = 
            (tdsrrc_SysInfoType1 *)tdsrrcasn1_decode_pdu( 
                               (void *)(tdssib_events_ptr->event[index].
                               tsib[tdsrrc_SIB1].sib_data_ptr), /* Encoded SIB3 */
                               tdsrrc_SysInfoType1_PDU, /* SIB3's PDU number         */
                               sib1_pdu_length       /* SIB3's length in bytes    */
                               );

            if(tdsrrcsib_bplmn_sibs.sib1 == NULL)
            {
               TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB1 decoding failed");
            }

            //call csp function to know if LAC is valid or not
            if(tdsrrccsp_check_sib1_validity(tdsrrcsib_bplmn_sibs.sib1) != TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAC not valid from SIB1 (BPLMN),fail SIB reading");
            
              /* Post-process the failed event */
              tdsrrcsib_post_process_failed_event
                ( index, /* Event Index                 */
                  TRUE,  /* Confirm Command is required */
                  TDSSIB_EVENT_FAILED_INVALID_SIBS, /* Failure Cause       */
                  TRUE   /* Tear down S_BCCH if it's up */
                );
            }            
            return TRUE;
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN:SIB1 already stored in BPLMN DB");
          }
          break;
        
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
        case tdsrrc_SB1:

          if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
          )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: SB1 has been received");
  
            if ((tdsrrcsib_bplmn_sibs.sb1 == NULL)) 
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: Storing SB1 in bplmn sibs database");
  
              /* Get the PDU length in bytes */
              sb1_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                                (tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].
                                 sib_tot_length);
  
              /* Decode and store SIB1 in bplmn sib database */
              tdsrrcsib_bplmn_sibs.sb1 = 
              (tdsrrc_SysInfoTypeSB1*)tdsrrcasn1_decode_pdu( 
                                 (void *)(tdssib_events_ptr->event[index].
                                 tsib[tdsrrc_SB1].sib_data_ptr), /* Encoded SIB3 */
                                 tdsrrc_SysInfoTypeSB1_PDU, /* SIB3's PDU number         */
                                 sb1_pdu_length       /* SIB3's length in bytes    */
                                 );
  
              if(tdsrrcsib_bplmn_sibs.sb1 == NULL)
              {
                 TDSRRC_MSG0(MSG_LEGACY_ERROR,"SB1 decoding failed");
              }
              else
              {
                
                sb_ptr = &(tdsrrcsib_bplmn_sibs.sb1->sib_ReferenceList);
                for(ix=0;ix< sb_ptr->n;ix++)
                {
                  tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&sb_ptr->elem[ix].sib_Type,
                                                        &sib_type_tag);
                  if(sib_type_tag.sib_type == tdsrrc_SIB5 ||
                      sib_type_tag.sib_type == tdsrrc_SIB5bis)
                  {
                    break;
                  }
                 
                }
    
                if((ix < sb_ptr->n)
		&&( (sib_type_tag.sib_type == tdsrrc_SIB5)||
                     (sib_type_tag.sib_type == tdsrrc_SIB5bis)))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"BAND_TYPE_DETECTION - SIB5/SIB5bis scheduled in SB1 ");
                  tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed = FALSE;
                }
              }
              return TRUE;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN:SB1 already stored in BPLMN DB");
            }
          }
          break;

        case tdsrrc_SB2:

          if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
          )
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: SB2 has been received");
  
            if ((tdsrrcsib_bplmn_sibs.sb2 == NULL)) 
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: Storing SB2 in bplmn sibs database");
  
              /* Get the PDU length in bytes */
              sb2_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                                (tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].
                                 sib_tot_length);
  
              /* Decode and store SIB1 in bplmn sib database */
              tdsrrcsib_bplmn_sibs.sb2 = 
              (tdsrrc_SysInfoTypeSB2*)tdsrrcasn1_decode_pdu( 
                                 (void *)(tdssib_events_ptr->event[index].
                                 tsib[tdsrrc_SB2].sib_data_ptr), /* Encoded SIB3 */
                                 tdsrrc_SysInfoTypeSB2_PDU, /* SIB3's PDU number         */
                                 sb2_pdu_length       /* SIB3's length in bytes    */
                                 );
  
              if(tdsrrcsib_bplmn_sibs.sb2 == NULL)
              {
                 TDSRRC_MSG0(MSG_LEGACY_ERROR,"SB2 decoding failed");
              }
              else
              {
                ix=0;
                sb_ptr = &tdsrrcsib_bplmn_sibs.sb2->sib_ReferenceList;
                for(ix=0;ix< sb_ptr->n;ix++)
                {
                  tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&sb_ptr->elem[ix].sib_Type,
                                                        &sib_type_tag);
                  if(sib_type_tag.sib_type == tdsrrc_SIB5 ||
                      sib_type_tag.sib_type == tdsrrc_SIB5bis)
                  {
                    break;
                  }
                }
    
                if((ix < sb_ptr->n)&&
                    ((sib_type_tag.sib_type == tdsrrc_SIB5)||
                      (sib_type_tag.sib_type == tdsrrc_SIB5bis)))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"BAND_TYPE_DETECTION - SIB5/SIB5bis scheduled in SB2 ");
                  tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed = FALSE;
                }
              }
              return TRUE;
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN:SB2 already stored in BPLMN DB");
            }
          }
          break;
#endif

        default:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: SIB not SIB1.Ingoring"); 
          break;
      }
    }
  }
  return ret_val;
}

/*===========================================================================

FUNCTION tdsrrcsib_check_lac_validity_from_sib1

DESCRIPTION

  This function checksLAC validity from SIB1

DEPENDENCIES
 
  None.

RETURN VALUE
 
SUCCESS:  LAC is vaid from SIB1
FAILURE:  otherwise

SIDE EFFECTS

  None.
        
===========================================================================*/
static uecomdef_status_e_type tdsrrcsib_check_lac_validity_from_sib1
(
  uint32 dbindex,                /* Index to the sib database. */
  tdssib_events_index_e_type index  /* Index into the sib_events database. */
)
{
  uecomdef_status_e_type status = FAILURE;
  boolean sib1_read = FALSE;      /* initialize sib1 read flag to false  */
  boolean sbcch_teardown_required = TRUE; /* If the event is for Serving BCCH*/
  tdsrrc_sib_status_e_type sib_status = TDSSIB_EVENT_FAILED_OTHER;


  if(TDSEVENT_INDEX_VALID(index) == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FAILURE;
  }
  //store local information that sib 1 are needed, received and decoded rather than use
  //long loops everywhere
  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_needed == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_received == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_decoded == TRUE))
  {
     sib1_read = TRUE;
  }

  if(sib1_read == FALSE)
  {
    status = SUCCESS;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:check & decode Sib5 in sibdb");
    if( tdssibdb.cell[dbindex].sibs.sib1== NULL )
    {
      /* ASN.1 decode the PDU */
      tdssibdb.cell[dbindex].sibs.sib1=  tdsrrcasn1_decode_pdu( 
        (void *)(tdssibdb.cell[dbindex].sibs.sib1_encoded_ptr), /* Encoded SIB      */
        tdsrrc_SysInfoType1_PDU,                                                       /* SIB's PDU number */
        tdssibdb.cell[dbindex].sibs.sib1_encoded_pdu_length   /* SIB's Length in bytes */
         );
      if(tdssibdb.cell[dbindex].sibs.sib1== NULL)
      {
        status = FAILURE;
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB_HEAP:Asn.1 decode failed from SIB5");
      }
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_HEAP:Decode Sib5 in sibdb");
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_HEAP:SIB5 already decoded,sib5_ptr=%d",tdssibdb.cell[dbindex].sibs.sib1);
    }

    //call csp function to know if LAC is valid or not
    if(tdsrrccsp_check_sib1_validity(tdssibdb.cell[dbindex].sibs.sib1) == TRUE)
    {
      status = SUCCESS;
    }
    else
    {/*handle fail case*/
      if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
         (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_CSP))
      {
        /* W Cell Selection or GtoW Cell Reselection */
        if(tdssib_events_ptr->event[index].for_serving_cell == TRUE)
        {
          sbcch_teardown_required = TRUE;
        }
        else
        {
         sbcch_teardown_required = FALSE;
        }
        sib_status = TDSSIB_EVENT_FAILED_INVALID_SIBS;
      }

      if(tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS)
      {
        //set the cause such that we trigger OOS
        sib_status = TDSSIB_EVENT_FAILED_LOWER_LAYERS;
      }
      /* Post-process the failed event */
      tdsrrcsib_post_process_failed_event
         ( index, /* Event Index                 */
                TRUE,  /* Confirm Command is required */
                sib_status, /* Failure Cause       */
                sbcch_teardown_required   /* Tear down S_BCCH if it's up */
              );
      status = FAILURE;
    }
  }
  return (status);
}
/*===========================================================================

FUNCTION DECODE_COMPLETED_SIBS

DESCRIPTION

  This function checks the tdssib_events database for the given index
  and for each SIB that has been completely received, it calls
  the ASN.1 decode function to decode the SIB and save it in the
  correct SIB database index. Only if the SIB database index is known
  for the received SIBs this function should be called.
  
DEPENDENCIES
 
  None.

RETURN VALUE
 
  None.

SIDE EFFECTS

  None.
        
===========================================================================*/
static void tdsrrcsib_decode_completed_sibs
(
  tdssib_events_index_e_type index  /* Index into the tdssib_events database. */
)
{
  uint32 count; /* local counter */
  uint32 dbindex; /* Index to the sib database. */
  boolean decode_succeeded; /* Indicate if a particular SIB
                               decode succeeded or not. */
  rrc_plmn_identity_type plmn;
  tdsrrc_sib_status_e_type status = TDSSIB_EVENT_SUCCEEDED;
  
  tdsrrc_MasterInformationBlock *tmp_mib = NULL;
  tdsrrc_SysInfoType1           *tmp_sib1= NULL;
  tdsrrc_SysInfoType3           *tmp_sib3= NULL;

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  /* Make sure that the database index has been found */
  if(!(tdssib_events_ptr->event[index].sib_db_index_found))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Can't decode SIBs till index is found");
    return;
  }
  
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Cells stored %d,decoding SIBs",tdssibdb.num_stored_cells);  

  /* Copy the database index for this cell */
  dbindex = tdssib_events_ptr->event[index].sib_db_index;

  if ( FAILURE == tdsrrcsib_check_sib5_sib6_barred_criteria(dbindex, index))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell-Barred Criteria met.  Bar cell");
    return;
  }

  if(tdsrrcsib_check_lac_validity_from_sib1(dbindex, index) == FAILURE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAC not valid from SIB1,fail SIB reading");
    return;
  }

  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_received == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_decoded == FALSE)
  )
  {

    decode_succeeded = 
    tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sb1),
                                        index, dbindex, tdsrrc_SB1, 
                                        tdsrrc_SysInfoTypeSB1_PDU,
                                        tdssibdb.cell[dbindex].sibs.sb1_encoded_ptr,
                                        &tdssibdb.cell[dbindex].sibs.sb1_encoded_pdu_length);
  
    if(decode_succeeded == TRUE)
    {
      tdssib_events_ptr->event[index].tempholder.sb1_decoded = TRUE;
      if(tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS) 
      {
        /* This means a SIB has changed for the serving cell
           and it has been decoded correctly. */
        tdsrrcsibdb_save_active_cell_sib_change(tdsrrc_SB1 );        
      }

      //check if scheduling info is received for all sibs
      if(FAILURE == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs((void *)tdssibdb.cell[dbindex].sibs.sb1,
                                                                          tdsrrc_SB1, index))
      {
        tdsrrcsib_process_sib_scheduling_info_incomplete(index); 
        return;
      }
      else
      {
        tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_SB1,
                                             tdssibdb.cell[dbindex].sibs.sb1, index);
      }


      /* When we receive a scheduling block, we can compare the
      value tags in that block to the value tags already stored
        for this event. */
      tdsrrcsib_update_event_for_received_value_tags(
        tdsrrc_SB1, /* Value tags from Scheduling block 1 */
        index,                /* Event index                        */
        dbindex               /* SIB Database index                 */                    
        );
    }
  }

  if( (tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_received == TRUE) &&
    (tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_decoded == FALSE)
  )
  {
    decode_succeeded = 
    tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sb2),
                                        index, dbindex, tdsrrc_SB2, 
                                        tdsrrc_SysInfoTypeSB2_PDU,
                                        tdssibdb.cell[dbindex].sibs.sb2_encoded_ptr,
                                        &tdssibdb.cell[dbindex].sibs.sb2_encoded_pdu_length);
  
    if(decode_succeeded == TRUE)
    {
      tdssib_events_ptr->event[index].tempholder.sb2_decoded = TRUE;
      if(tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS) 
      {
        /* This means a SIB has changed for the serving cell
           and it has been decoded correctly. */
        tdsrrcsibdb_save_active_cell_sib_change(tdsrrc_SB2);        
      }

      //check if scheduling info is received for all sibs
      if(FAILURE == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs((void *)tdssibdb.cell[dbindex].sibs.sb2, 
                                                                          tdsrrc_SB2, index))
      {
        tdsrrcsib_process_sib_scheduling_info_incomplete(index); 
        return;
      }
      else
      {
        tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_SB2,
                                             tdssibdb.cell[dbindex].sibs.sb2, index);
      }  

      /* When we receive a scheduling block, we can compare the
      value tags in that block to the value tags already stored
        for this event. */
      tdsrrcsib_update_event_for_received_value_tags(
        tdsrrc_SB2, /* Value tags from Scheduling block 1 */
        index,                /* Event index                        */
        dbindex               /* SIB Database index                 */                    
        );
    }
  }

  /* Look at each SIB to see if it has been completely received
  but not yet decoded. */
  for( count=0; count<TDSRRC_TOT_SIBS; count++)
  {
    if( (tdssib_events_ptr->event[index].tsib[count].sib_needed == TRUE) &&
        (tdssib_events_ptr->event[index].tsib[count].sib_received == TRUE) &&
        ((tdssib_events_ptr->event[index].tsib[count].sib_decoded == FALSE)||
        ((count == (uint32)tdsrrc_SIB7)&&
         (tdssib_events_ptr->event[index].tsib[count].sib_tot_length != 0)))
      )
    {
      /* This is a SIB that is needed and received, but not decoded.
      switch on the SIB type and decode the SIB to the correct
      sib pointer */

      decode_succeeded = FALSE; /* Set this to FALSE each time. if a decode succeeds, 
                                   it will be set to TRUE in the case statements below. */

      switch( count )
      {
        case tdsrrc_MIB :
          // call the function that decodes mib and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.mib),
                                              index, dbindex, tdsrrc_MIB,
                                              tdsrrc_MasterInformationBlock_PDU,
                                              tdssibdb.cell[dbindex].sibs.mib_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.mib_encoded_pdu_length);
          break;
 
        case tdsrrc_SIB1 :
          // call the function that decodes sib1 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib1),
                                              index, dbindex, tdsrrc_SIB1,
                                              tdsrrc_SysInfoType1_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib1_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib1_encoded_pdu_length);
          if(decode_succeeded == TRUE)
          {
            if(tdsrrcsib_check_lac_validity_from_sib1(dbindex, index) == FAILURE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"LAC not valid from SIB1,fail SIB reading");
              return;
            }
          }
          break;

        case tdsrrc_SIB2 :
          // call the function that decodes sib2 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib2),
                                              index, dbindex, tdsrrc_SIB2,
                                              tdsrrc_SysInfoType2_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib2_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib2_encoded_pdu_length);
          break;

        case tdsrrc_SIB3 :
          // call the function that decodes sib3 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib3),
                                              index, dbindex, tdsrrc_SIB3,
                                              tdsrrc_SysInfoType3_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib3_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib3_encoded_pdu_length);
          if((decode_succeeded == TRUE)&&
             (tdssib_events_ptr->event[index].for_serving_cell != TRUE)&&
             (!(tdssib_events_ptr->event[index].tempholder.sib3_decoded)))            
          {
            tdsrrc_translate_gsm_map_plmn_id(&(tdssibdb.cell[dbindex].sibs.mib->plmn_Type.u.gsm_MAP->plmn_Identity),&plmn);

            /*Do not perform suitablity check for Cell ID in DCH scans*/
            if(tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
            {
              tdsrrccsp_initial_suitability_check(tdssibdb.cell[dbindex].sibs.mib,
                                               tdssibdb.cell[dbindex].sibs.sib3,
                                               plmn,
                                               &status);
            }
            if(status != TDSSIB_EVENT_SUCCEEDED)
            {
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                                TRUE,
                                                status, 
                                                TRUE);
              return;
            }
          }
          break;

        case tdsrrc_SIB4 :
          // call the function that decodes sib4 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib4),
                                              index, dbindex, tdsrrc_SIB4,
                                              tdsrrc_SysInfoType4_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib4_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib4_encoded_pdu_length);
          break;
      
        case tdsrrc_SIB5 :
          // call the function that decodes sib5 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib5),
                                              index, dbindex, tdsrrc_SIB5,
                                              tdsrrc_SysInfoType5bis_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib5_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib5_encoded_pdu_length);
          if(TRUE == decode_succeeded)
          {
            if(FAILURE == tdsrrcsib_check_sib5_sib6_barred_criteria(dbindex, index))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell-Barred Criteria met for SIB5 decode");
              return;
            }
          }
          break;
      
        case tdsrrc_SIB6 :
          // call the function that decodes sib6 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib6),
                                              index, dbindex, tdsrrc_SIB6,
                                              tdsrrc_SysInfoType6_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib6_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib6_encoded_pdu_length);
          if(TRUE == decode_succeeded)
          {
            if(FAILURE == tdsrrcsib_check_sib5_sib6_barred_criteria(dbindex, index))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Cell-Barred Criteria met for SIB6 decode");
              return;
            }
          }
          break;

        case tdsrrc_SIB7 :
          // call the function that decodes sib7 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib7),
                                              index, dbindex, tdsrrc_SIB7,
                                              tdsrrc_SysInfoType7_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib7_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib7_encoded_pdu_length);
          break;

        case tdsrrc_SIB11 :
          // call the function that decodes sib11 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib11),
                                              index, dbindex, tdsrrc_SIB11,
                                              tdsrrc_SysInfoType11_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib11_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib11_encoded_pdu_length);
          break;

        case tdsrrc_SIB12 :
          // call the function that decodes sib12 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib12),
                                              index, dbindex, tdsrrc_SIB12,
                                              tdsrrc_SysInfoType12_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib12_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib12_encoded_pdu_length);
          break;

        case tdsrrc_SIB18 :
          // call the function that decodes sib18 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib18),
                                              index, dbindex, tdsrrc_SIB18,
                                              tdsrrc_SysInfoType18_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib18_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib18_encoded_pdu_length);
          break;

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        case tdsrrc_SIB11bis :
          // call the function that decodes sib18 and returns true/false based on decode status
          decode_succeeded = 
          tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib11bis),
                                              index, dbindex, tdsrrc_SIB11bis,
                                              tdsrrc_SysInfoType11bis_PDU,
                                              tdssibdb.cell[dbindex].sibs.sib11bis_encoded_ptr,
                                              &tdssibdb.cell[dbindex].sibs.sib11bis_encoded_pdu_length);
        break;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
                case tdsrrc_SIB19 :
                  // call the function that decodes sib18 and returns true/false based on decode status
                  decode_succeeded = 
                  tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[dbindex].sibs.sib19),
                                                      index, dbindex, tdsrrc_SIB19,
                                                      tdsrrc_SysInfoType19_PDU,
                                                      tdssibdb.cell[dbindex].sibs.sib19_encoded_ptr,
                                                      &tdssibdb.cell[dbindex].sibs.sib19_encoded_pdu_length);

                  if(decode_succeeded == FALSE && TDS_NV_OPTIONAL_IGNORE_SIB19_DECODE_FAIL_ENABLE)
                  {
                    tdsrrcsib_sib19_decode_fail_count ++;
                  }

                break;
#endif

        default:

          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown SIB%d",count);
          break;
   
      }/* end switch */

      /* If any new sibs have been decoded we may have to
      inform other procedures about it. This is done only
      if the sibs have changed in the serving cell. So
      check if the event in progress is a TDSRRCSIB_UPDATE_SIBS
      event. If so, ask the tdssibdb module to save this 
      SIB change. */
      if( tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS )
      {
        if( decode_succeeded == TRUE )
        {
          /* This means a SIB has changed for the serving cell
          and it has been decoded correctly. */
          tdsrrcsibdb_save_active_cell_sib_change( (tdsrrc_SIB_type)count );        
        }
      }

    }/* end if (sibs need to be decoded) */
  }/* end for (TDSRRC_TOT_SIBS) */

  if (((tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN))
      &&(tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_decoded == TRUE)
      &&(tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received == TRUE)
      &&(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_received == TRUE) 
      &&(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_decoded == TRUE) 
      &&(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_received == TRUE) 
      &&(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_decoded == TRUE))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Forbidden_Cell:Check the forbidden cell status when decode sib 1 and sib 3");
    tmp_mib = (tdsrrc_MasterInformationBlock *)tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB ,dbindex);
    tmp_sib3 = (tdsrrc_SysInfoType3 *)tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SIB3 ,dbindex);
    tmp_sib1 = (tdsrrc_SysInfoType1 *)tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SIB1,dbindex);

    ASSERT(tmp_mib != NULL && tmp_sib1 != NULL && tmp_sib3 != NULL);

    tdsrrc_translate_gsm_map_plmn_id(&(tmp_mib->plmn_Type.u.gsm_MAP->plmn_Identity),&plmn);
    
    tdsrrccsp_check_and_send_bar_forbid_cells(tmp_sib3,
                       plmn,
                       tmp_mib,
                       tmp_sib1,
                       &status);
    if (status != TDSSIB_EVENT_SUCCEEDED)
    {
       tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                      TRUE,
                                      status, 
                                      TRUE);
    }
  }
}/* end function tdsrrcsib_decode_completed_sibs */

/*===========================================================================

FUNCTION tdsrrcsib_process_scheduling_block_sched_info_before_sib_db_decoded

DESCRIPTION


DEPENDENCIES
 
  None.

RETURN VALUE

  SUCCESS: If scheduling information is received for all mandatory sib's
  FAILURE: If scheduling information is not received for all mandatory sib's

SIDE EFFECTS
  
===========================================================================*/
static uecomdef_status_e_type tdsrrcsib_process_scheduling_block_sched_info_before_sib_db_decoded
(
  tdssib_events_index_e_type index, /* Index in the SIB event database */
  tdsrrc_SIB_type sib
)
{
  tdsrrc_SysInfoTypeSB1 *sb1_ptr;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr;         /* Local pointer to SB2       */
  void *             local_sb_ptr = NULL;
  uecomdef_status_e_type status=SUCCESS;
  uint32 pdu_length;

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FAILURE;
  }

  if(sib >= tdsrrc_ExtensionType)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Type");
    return FAILURE;
  }

  /* Decode the sb1/sb2 to the local copy - For this, first we need the PDU
  length in bytes. Divide the PDU lenth in bits with 8 and add 1 since
  we can err on the high side for the length. */
  pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                (tdssib_events_ptr->event[index].tsib[sib].sib_tot_length);

  if(sib == tdsrrc_SB1)
  {
    /* Call tdsrrcasn1_decode_pdu to do ASN1 decoding. Since it returns a 
    void pointer, typecast it to the SB1's type. */
    sb1_ptr =
      ((tdsrrc_SysInfoTypeSB1 *)tdsrrcasn1_decode_pdu( 
        (void *)(tdssib_events_ptr->event[index].
                 tsib[tdsrrc_SB1].sib_data_ptr), /* Encoded MIB */
        tdsrrc_SysInfoTypeSB1_PDU,                            /* SB1 PDU number        */
        pdu_length                                         /* SB1 Length in bytes   */
        ));
    local_sb_ptr = (void *) sb1_ptr;

    tdssib_events_ptr->event[index].tempholder.sb1_decoded=TRUE;
  }
  else
  {
    /* Call tdsrrcasn1_decode_pdu to do ASN1 decoding. Since it returns a 
    void pointer, typecast it to the SB2's type. */
    sb2_ptr =
      ((tdsrrc_SysInfoTypeSB2 *)tdsrrcasn1_decode_pdu( 
        (void *)(tdssib_events_ptr->event[index].
               tsib[tdsrrc_SB2].sib_data_ptr), /* Encoded MIB */
        tdsrrc_SysInfoTypeSB2_PDU,                          /* SB1 PDU number        */
        pdu_length                                       /* SB1 Length in bytes   */
        ));
    local_sb_ptr = (void *) sb2_ptr;

    tdssib_events_ptr->event[index].tempholder.sb2_decoded=TRUE;
  }

  if(local_sb_ptr == NULL )
  {
    /* Decode did not work */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SB decode failed");

    /*Reset all the variables for this SIB so that we can try to
    read it again */
    tdssib_events_ptr->event[index].tsib[sib].sib_received = FALSE;
    tdssib_events_ptr->event[index].tsib[sib].sib_decoded = FALSE;
    tdssib_events_ptr->event[index].tsib[sib].seg_count = 0;
    tdssib_events_ptr->event[index].tsib[sib].seg_index = 
      (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
    tdssib_events_ptr->event[index].tsib[sib].sib_tot_length = 0;
  }
  else
  {
    //check if scheduling info is received for all sibs
    if(FAILURE == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs(local_sb_ptr, sib, index))
    {
      status= FAILURE;
    }
    else
    {
      tdsrrcsib_update_sched_info_from_mib_sb((uint32)sib, local_sb_ptr, index);
    }
  }

  //delete the allocated memory for the scehduling block
  if(local_sb_ptr != NULL)
  {
    if(sib == tdsrrc_SB1)
    { 
      tdsrrcsibdb_free_sib( tdsrrc_SysInfoTypeSB1_PDU,
                          local_sb_ptr);
    }
    else
    {
      tdsrrcsibdb_free_sib( tdsrrc_SysInfoTypeSB2_PDU,
                          local_sb_ptr);
    }
  }
  return status;
}

/*===========================================================================

FUNCTION tdsrrcsib_check_if_plmn_is_gsm_map

DESCRIPTION

  This function will check whether the PLMN is GSM MAP.or not
  
DEPENDENCIES
 
  None.

RETURN VALUE

  TRUE: If the PLMN is GSM MAP.
  FALSE: other wise

SIDE EFFECTS

  Serving cell index in the SIB database may be changed.
  
===========================================================================*/
static boolean  tdsrrcsib_check_if_plmn_is_gsm_map
(
  tdssib_events_index_e_type index,
  boolean sbcch_teardown_required
)
{
  boolean status = TRUE;
  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }
  
  if(tdssib_events_ptr->event[index].tempholder.mib != NULL)
  {
    if(!((TDSRRCGET_PLMN_TYPE((&tdssib_events_ptr->event[index].tempholder.mib->plmn_Type))
                                                      == TDSRRCGET_PLMN_TYPE_TAG(gsm_MAP)) ||
         (TDSRRCGET_PLMN_TYPE((&tdssib_events_ptr->event[index].tempholder.mib->plmn_Type)) 
                                                      == TDSRRCGET_PLMN_TYPE_TAG(gsm_MAP_and_ANSI_41))
        )
      )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Acquired cell PLMN not GSM-MAP!");
      /* No need process this event further, just send a failure
        to the procedure that requested this sib event. */
      status = FALSE;
      tdsrrcsib_post_process_failed_event( index, /* Event Index                 */
                                            TRUE,  /* Confirm Command is required */
                                            TDSSIB_EVENT_FAILED_INVALID_SIBS, /* Failure cause */
                                            sbcch_teardown_required   /* Tear down S_BCCH if it's up */
                                          );
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"tempholder.mib ptr is NULL");
    status = FALSE;
  }
  return status;
}

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
void tdsrrcsib_check_and_update_manual_search_bst(tdssib_events_index_e_type index)
{
  sys_plmn_id_s_type sys_plmn_id;
  sys_sys_mode_e_type sys_mode;
  tdsrrc_csp_band_class_type band_class;
  sys_band_mask_type sys_band;
  rrc_plmn_identity_type plmn;

  /* Check to see if PLMN Type is not correct here. */
  if(tdsrrcsib_check_if_plmn_is_gsm_map(index,FALSE) != FALSE)
  {
	//1:get sys_plmn_id_s_type
	/* Get the plmn_id received in the current MIB */
	tdsrrc_translate_gsm_map_plmn_id
	  (&(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.u.gsm_MAP->plmn_Identity),
	   &plmn);

	sys_plmn_id = mmumts_convert_rrc_plmn_id_to_nas_plmn_id(plmn);
	  
	//2:get sys_sys_mode_e_type 
	sys_mode = SYS_SYS_MODE_TDS;

	//3:get sys_band_mask_type 
	if (tdssib_events_ptr->event[index].for_serving_cell == TRUE)
	{
	  band_class = tdsrrc_get_frequency_band(tdssib_events_ptr->srv_bcch.freq);
	}
    else
	{
	  band_class = tdsrrc_get_frequency_band(tdssib_events_ptr->ngh_bcch.freq);
	}
    sys_band = tdsrrc_convert_band_to_sys_band(band_class);
    
	//4:call API sd_update_manual_search_bst();
#ifdef FEATURE_DUAL_SIM
  sd_update_manual_search_bst_per_subs(sys_plmn_id, sys_mode, sys_band,tdsrrc_get_as_id());
#else
  sd_update_manual_search_bst(sys_plmn_id, sys_mode, sys_band);
#endif
	
	TDSRRC_MSG4(MSG_LEGACY_HIGH, "Update manual_search_bst: MCC %d-MNC %d,sys_mode: %d, sys_band: %d",
				  TDSRRC_CSP_GET_INT_MCC(plmn),
				  TDSRRC_CSP_GET_INT_MNC(plmn),
				  sys_mode,
				  sys_band);
  }
}

/*===========================================================================

FUNCTION PROCESS_COMPLETED_SIBS

DESCRIPTION

  This function processes SIBs that have been completely received 
  It saves the SIBs in the SIB database if the index for saving these
  SIBs is already known. If not, the SIBs are left unprocessed.
  They will be processed after the index is known.
  
  If the index for saving SIBs is not known and the PLMN id and 
  cell id have been received for this SIB, this function tries 
  to match the index for these SIBs in the SIB database. 

  Since the MIB carries the PLMN id and SIB3 carries the cell
  id, this function looks for MIB and SIB3 inorder to find the
  index. MIB and SIB3 are decoded by this function. Once the
  MIB is decoded, the value-tags are also matched if   
  SIBs have already been stored in the database for this SIB.
  
  If a match is found, SIBs can be saved at the existing Index.
  Also if SIBs are previously saved at that index, value_tag
  comparisons will be made and the event database adjusted accordingly.
  
  If a match is not found, SIBs will be saved at a new index in the
  database.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  boolean

SIDE EFFECTS

  Serving cell index in the SIB database may be changed.
  
===========================================================================*/
static boolean tdsrrcsib_process_completed_sibs
(
  tdsrrc_SFN_Prime           sfn_prime, /* SFN Prime from BCH header */
  tdssib_events_index_e_type index   /* Index to the SIB events database
                                  in which SIBs have been completed. */
)
{
  uint32 pdu_length; /* To store length of PDU in bytes to be passed
                     to the ASN.1 decoder. */
  uint32 dbindex;
  rrc_plmn_identity_type plmn; /* Store the PLMN from the received MIB     */
  tdsrrc_int_cell_id_type cell;   /* Store the cell id from the received SIB3 */
  boolean non_gsm_map_plmn = FALSE; /* Indicate if the plmn found was not
                               a GSM-MAP plmn. */
//  uint32 ix =0;
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
  uint32 sb1_pdu_length;
  uint32 sb2_pdu_length;
  tdsrrc_SIBSb_ReferenceList *sibSb_ptr=NULL;
  tdsrrc_SIB_ReferenceList *sb_ptr = NULL;
  tdsrrcsib_sib_type_vtag sib_type_tag;
#endif

    /*Test code start*/
  tdsrrc_int_cell_id_type cell_id;
  rrc_plmn_identity_type plmn_id;
  /*Test code end*/
  
  tdsrrc_sib_status_e_type status = TDSSIB_EVENT_SUCCEEDED;
  
  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }
  
  /* Initialize PLMN to dummy value to keep lint happy */
  plmn.mcc[0] = 0;
  plmn.mcc[1] = 0;
  plmn.mcc[2] = 0;
  plmn.num_mnc_digits = 3;
  plmn.mnc[0] = 0;
  plmn.mnc[1] = 0;
  plmn.mnc[2] = 0;

  if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN)||
     (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN)||
     (tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN) ||
     (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)||
     (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN) ||
     (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN))
  {
    /* Check to see of other SIBS(currently only SIB1) is received,if MIB and SIB3 has already been stored */
    if((tdsrrcsib_bplmn_sibs.mib != NULL) && (tdsrrcsib_bplmn_sibs.sib3 != NULL))
    {
      if(TRUE == tdsrrcsib_decode_completed_sibs_for_bplmn(index))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"TD2TD BPLMN: Returning from tdsrrcsib_decode_completed_sibs_for_bplmn func for TD2TD scan");
        return TRUE;
      }
    }
  }

  if( tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS )
  {
    tdsrrcsib_decode_sib7_in_update_sibs(index);
  }
  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_received)&& 
     (!(tdssib_events_ptr->event[index].tempholder.sib1_decoded)))
  {
    tdsrrc_SysInfoType1 *sib1_ptr;
    pdu_length = TDSGET_PDU_LENGTH_IN_BYTES(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_tot_length);
    sib1_ptr =(tdsrrc_SysInfoType1*)tdsrrcasn1_decode_pdu((void *)
                                                   (tdssib_events_ptr->event[index].
                                                     tsib[tdsrrc_SIB1].sib_data_ptr), /* Encoded SIB1 */
                                                     tdsrrc_SysInfoType1_PDU,/* SIB1's PDU number */
                                                     pdu_length/* SIB1's Length in bytes   */
                                                   );

    if(sib1_ptr == NULL)
    {
      /* Decode did not work */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB1 decode failed");
      /*Reset all the variables for this SIB so that we can try to
      read it again */
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_received = FALSE;
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].seg_count = 0;
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_tot_length = 0;
    }
    else
    { 
      tdssib_events_ptr->event[index].tempholder.sib1_decoded = TRUE;
      if(tdsrrccsp_check_sib1_validity(sib1_ptr) == FALSE)
      {
        tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                          TRUE,
                                          TDSSIB_EVENT_FAILED_INVALID_SIBS, 
                                          tdssib_events_ptr->event[index].for_serving_cell
                                         );
        tdsrrcsibdb_free_sib(tdsrrc_SysInfoType1_PDU,(void *)sib1_ptr);
        return FALSE;
      }
      if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
         (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_CSP) &&
         (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL))
      {               
        if(tdssib_events_ptr->event[index].sib_db_index_found )
        {
          tdsrrc_MasterInformationBlock *temp_mib_ptr = NULL;
          dbindex = tdssib_events_ptr->event[index].sib_db_index;
          temp_mib_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB,dbindex);

          if(temp_mib_ptr != NULL)
          {
            if(tdsrrccsp_check_for_lai_forbidden(sib1_ptr,temp_mib_ptr))
            { 
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"LA is forbidden");
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                                TRUE,
                                                TDSSIB_EVENT_FAILED_FB_LA, 
                                                tdssib_events_ptr->event[index].for_serving_cell
                                                ); 
              tdsrrcsibdb_free_sib(tdsrrc_SysInfoType1_PDU,(void *)sib1_ptr);
              return FALSE;
            
            }
          }
        }
        else
        {
          if(tdssib_events_ptr->event[index].tempholder.mib != NULL)
          {
            if(tdsrrccsp_check_for_lai_forbidden(sib1_ptr,tdssib_events_ptr->event[index].tempholder.mib))
            { 
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"LA is forbidden");
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                                TRUE,
                                                TDSSIB_EVENT_FAILED_FB_LA, 
                                                tdssib_events_ptr->event[index].for_serving_cell
                                                );
              tdsrrcsibdb_free_sib(tdsrrc_SysInfoType1_PDU,(void *)sib1_ptr);
              return FALSE;
            }
          }
        }         
      }
      tdsrrcsibdb_free_sib(tdsrrc_SysInfoType1_PDU,(void *)sib1_ptr);
    }  
  }


  /* Check if this event already has a SIB database index
  for storing the SIBs. We would have the index only if
  we've already decoded the MIB and SIB 3 inorder to get the
  PLMN id and cell id of this cell. */
  if( tdssib_events_ptr->event[index].sib_db_index_found )
  {
    /* We can decode and store whatever SIBs we've received in
    this pass */
    TDSRRC_MSG0(MSG_LEGACY_LOW,"Decoding received SIBs");
    tdsrrcsib_decode_completed_sibs(index);

  }
  else
  {
    /* Check if the MIB was received in this pass */
    if( (tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received)
      && (!(tdssib_events_ptr->event[index].tempholder.mib_decoded)))
    {
      /* MIB has been received this time */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB Rx'ed this pass");

      if ((tdssib_events_ptr->event[index].for_serving_cell == FALSE)
         && (tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS))
      {
         uint16 sfn = sfn_prime * 2;
         uint16 sfn_os = sfn % tdsrrcsib_sib_sleep.repetition_len;
         
         if (TDSRRCSIB_SB_SLEEP_TEST_ENABLED)
         {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"ZZ: TDSRRCSIB_SB_SLEEP_TEST_ENABLED,sib_sleep_flag 0x%x",tdsrrcsib_sib_sleep.sib_sleep_flag); 
            
            if (((tdsrrcsib_sib_sleep.misc_data < tdsrrcsib_sib_sleep.repetition_len) && (sfn_os < tdsrrcsib_sib_sleep.misc_data))
               /* Skip until MIB reaches tdsrrcsib_sib_sleep.misc_data */
                || ((tdsrrcsib_sib_sleep.misc_data >= tdsrrcsib_sib_sleep.repetition_len) && sfn_os) 
                /* Always align tdsrrcsib_sib_sleep.repetition_len to start MIB decoding */
               )
            {
               /* This is only for NV controlled test */
               TDSRRC_MSG5(MSG_LEGACY_HIGH,"ZZ: skip MIB for sfn %d, offset(over %d) %d, sib_end %d mib_start %d", 
                           sfn, 
                           tdsrrcsib_sib_sleep.repetition_len,
                           sfn_os, 
                           tdsrrcsib_sib_sleep.sib_end_frame,
                           tdsrrcsib_sib_sleep.misc_data);
                   
               /*Reset all the variables for this SIB so that we can try to
               read it again */
               tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received = FALSE;
               tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_decoded = FALSE;
               tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].seg_count = 0;
               tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].seg_index = 
                 (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
               tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_tot_length = 0;

               return FALSE;
            }
         }

         TDSRRC_MSG6(MSG_LEGACY_HIGH,"ZZ: SIB sleep enabled %d, sfn %d, offset(over %d) %d, sib_end %d, mib_start %d", 
            tdsrrcsib_sib_sleep.sib_sleep_flag,
            sfn, 
            tdsrrcsib_sib_sleep.repetition_len,
            sfn_os, 
            tdsrrcsib_sib_sleep.sib_end_frame,
            tdsrrcsib_sib_sleep.misc_data);
      }      

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
      tdsno_mib_bar_cell_count = 0;
#endif

      /* Decode the MIB to the local copy - For this, first we need the PDU
      length in bytes. Divide the PDU lenth in bits with 8 and add 1 since
      we can err on the high side for the length. */
      pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                    (tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].
                     sib_tot_length);

      /* Call tdsrrcasn1_decode_pdu to do ASN1 decoding. Since it returns a 
      void pointer, typecast it to the MIB's type. */
      tdssib_events_ptr->event[index].tempholder.mib =
        (tdsrrc_MasterInformationBlock *)tdsrrcasn1_decode_pdu( 
          (void *)(tdssib_events_ptr->event[index].
                 tsib[tdsrrc_MIB].sib_data_ptr), /* Encoded MIB */
          tdsrrc_MasterInformationBlock_PDU,  /* MIB's PDU number        */
          pdu_length                       /* MIB's Length in bytes   */
                        );

      if( tdssib_events_ptr->event[index].tempholder.mib == NULL )
      {
        /* Decode did not work */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB decode failed");

        /*Reset all the variables for this SIB so that we can try to
        read it again */
        tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received = FALSE;
        tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_decoded = FALSE;
        tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].seg_count = 0;
        tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].seg_index = 
          (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
        tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_tot_length = 0;
        tdssib_events_ptr->sib_sched[tdsrrc_MIB].repitition_rate = TDSINVALID_SIB_SCHEDULE_REP;
      }
      else
      {
        tdsrrctmr_stop_timer(TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER);
            
        /*MPLMN Search Time Optimization by using BST table and solution to overcome limitation*/
        tdsrrcsib_check_and_update_manual_search_bst(index);
		
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Assign MIB SFN Prime: %d from %d",sfn_prime,tdssib_events_ptr->event[index].tempholder.mib_sfn_prime);
        
        tdssib_events_ptr->event[index].tempholder.mib_sfn_prime = sfn_prime;
     
        if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
           (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_CSP) &&
           (tdssib_events_ptr->event[index].for_serving_cell == TRUE)
          )
        {
          /* Check to see if PLMN Type is not correct here. */
          if(tdsrrcsib_check_if_plmn_is_gsm_map(index,FALSE) == FALSE)
          {
            return FALSE;
          }
          else
          {
            /* Get the plmn_id received in the current MIB */
            tdsrrc_translate_gsm_map_plmn_id
              (&(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.u.gsm_MAP->plmn_Identity),
               &plmn);
  
            /*In Network sharing cell there may be more than one PLMN broadcasted
               in a cell, so pass MIB pointer.this csp function will check if its N/W 
               shared cell then it will extract all the PLMN's for its processing*/
            if(tdsrrccsp_skip_sibs_for_fplmn(plmn
                                          ,tdssib_events_ptr->event[index].tempholder.mib
              ) == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"FPLMN: Cell belongs to Forbidden PLMN. Skip reading SIBs");
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                               TRUE,
                                               TDSSIB_EVENT_FAILED_FPLMN, 
                                               FALSE);
              return FALSE;
            }

            if(tdsrrccsp_skip_sibs_for_vplmn(plmn, tdssib_events_ptr->event[index].tempholder.mib) == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH, "Found VPLMN already added to the available PLMN list. Skip reading SIBs");
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                               TRUE,
                                               TDSSIB_EVENT_FAILED_OTHER, 
                                               FALSE);
              return FALSE;
            }
			
		  /* Skipping to read the SIBs if MIB PLMN is not matching with the NAS requested PLMN 
			 for X2TD redirection cases */
		  if((tdsrrc_csp_int_data.curr_scan  == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN) && 
			 (FALSE  == tdsrrccsp_check_plmn_match(plmn,tdssib_events_ptr->event[index].tempholder.mib)))
		  {
			TDSRRC_MSG0(MSG_LEGACY_HIGH,"X2TD: Cell doesn't belong to the NAS requested PLMN. Skip reading SIBs");
			tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
											  TRUE,
											  TDSSIB_EVENT_FAILED_PLMN_MISMATCH, 
											  FALSE);
			return FALSE;
		  }

			
          }
        }
     
        /* Haizhou 2013/1/18
		 * Decode and save MIB for all T2T BPLMN.  Because we will check MIB when cell SIB collection is 
		 * interrupted by DRX timer hit MAX.
		 */
        if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
           (tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN) ||
           ((tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
#ifdef FEATURE_LTE_TO_TDSCDMA
            && (!tdsrrccsp_check_cgi_scan())
#endif
           )
          )
        {
		      if(tdsrrcsib_check_if_plmn_is_gsm_map(index,FALSE) == FALSE)
              {
                  return FALSE;
              }
              else
              {
                 if(tdsrrcsib_bplmn_sibs.mib == NULL)
                 {
                   uint32 mib_pdu_length;
			        TDSRRC_MSG1(MSG_LEGACY_HIGH,"BPLMN timer expiry optimization: storing MIB for Manual/Auto scan in BPLMN SIBs db,Scan type %d",tdssib_events_ptr->event[index].sib_scan_type);
  
  
                    /* Get the PDU length in bytes */
                    mib_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                         (tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_tot_length);
  
                    /* Decode and store MIB in bplmn sib database */
                    tdsrrcsib_bplmn_sibs.mib = (tdsrrc_MasterInformationBlock *)tdsrrcasn1_decode_pdu( 
                    (void *)(tdssib_events_ptr->event[index].
                             tsib[tdsrrc_MIB].sib_data_ptr), /* Encoded MIB */
                    tdsrrc_MasterInformationBlock_PDU,                 /* MIB's PDU number        */
                    mib_pdu_length                                  /* MIB's Length in bytes   */
                    );
                 }
              }
        }


#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN     
        /* If this is a manual BPLMN scan,we need to read only the MIB.Decode and save MIB here */
        if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
           ((tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
#ifdef FEATURE_LTE_TO_TDSCDMA
            && (!tdsrrccsp_check_cgi_scan())
#endif
           )
          )
        {
          /* Check to see if PLMN Type is not correct here. */
          if(tdsrrcsib_check_if_plmn_is_gsm_map(index,FALSE) == FALSE)
          {
            return FALSE;
          }
          else
          {
    
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Storing MIB for Manual scan in BPLMN SIBs db,Scan type %d",tdssib_events_ptr->event[index].sib_scan_type);
  
            if(tdsrrcsib_bplmn_sibs.mib == NULL)
            {
              uint32 mib_pdu_length;
  
              /* Get the PDU length in bytes */
              mib_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                (tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_tot_length);
  
              /* Decode and store MIB in bplmn sib database */
              tdsrrcsib_bplmn_sibs.mib = (tdsrrc_MasterInformationBlock *)tdsrrcasn1_decode_pdu( 
                (void *)(tdssib_events_ptr->event[index].
                         tsib[tdsrrc_MIB].sib_data_ptr), /* Encoded MIB */
                tdsrrc_MasterInformationBlock_PDU,                 /* MIB's PDU number        */
                mib_pdu_length                                  /* MIB's Length in bytes   */
                );
  
              if(tdsrrcsib_bplmn_sibs.mib != NULL)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: Storing MIB in bplmn sibs database");
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"SB1 needed = %d SB2 needed = %d",tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed,tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed);
             
                sibSb_ptr = &tdsrrcsib_bplmn_sibs.mib->sibSb_ReferenceList;
				for(ix=0;ix< sibSb_ptr->n;ix++)
                {
                  tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag(&sibSb_ptr->elem[ix].sibSb_Type,
                                                              &sib_type_tag);
                  if(sib_type_tag.sib_type == tdsrrc_SIB5 ||
                      sib_type_tag.sib_type == tdsrrc_SIB5bis)
                  {
                    break;
                  }
                }
                if(((sibSb_ptr->n > ix )&& (ix !=0 ))&&
                  ((sib_type_tag.sib_type == tdsrrc_SIB5) ||
                    (sib_type_tag.sib_type == tdsrrc_SIB5bis)))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-SIB5/SIB5bis found in MIB");
                  tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed = FALSE;
                  tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed = FALSE;
                }
#endif
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_ERROR,"BPLMN MIB decode failure");
              }
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: BPLMN db already has a MIB");
            }
          }   
        }   
#endif

        //check if scheduling info is received for all sibs
        if(FAILURE == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs(
          (void *)(tdssib_events_ptr->event[index].tempholder.mib),
          tdsrrc_MIB,
          index))
        {
          tdsrrcsib_process_sib_scheduling_info_incomplete(index); 
          return TRUE;
        }

        tdssib_events_ptr->event[index].tempholder.mib_decoded = TRUE;

        /*MIB is decoded, we check tdsrrcsib_g_in_data_call again*/
        if(tdsrrcsib_g_in_data_call == FALSE)
        {
          if(tdssib_events_ptr->event[index].for_serving_cell == FALSE)
          {
            /* Reselection case, TDSRRCTMR_SIB_STOP_PARTITION_TIMER is not started yet  */
            tdsrrcsib_g_in_data_call = TDSL1_DS_G2T_TA_MODE_ACTIVE();
            if(TRUE == tdsrrccsp_is_other_sub_in_dds())
            {
              tdsrrcsib_g_in_data_call = FALSE;
            }
          }
          else if ((TRUE == (tdsrrcsib_g_in_data_call = TDSL1_DS_G2T_TA_MODE_ACTIVE())) &&
                   (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL)&&
                   (tdssib_events_ptr->event[index].int_event.int_event_type != TDSRRCSIB_INT_SIB7_EVENT))
          {
            /* Selection case, if it is G2T mode now, we should start TDSRRCTMR_SIB_STOP_PARTITION_TIMER 
                      according to the remaining time of SIB Wait timer*/
            uint64 remaining_time;
            remaining_time = tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB_WAIT_TIMER);          
            if (remaining_time > TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS/2)
            {
              if (TRUE == tdsrrccsp_do_opt_for_con_to_idle())
              {
                tdsrrcsib_g_in_data_call = FALSE;
              }
              else
              {
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"Change to G2T mode %d, remaining_time %d", tdsrrcsib_g_in_data_call, remaining_time);
                tdsrrctmr_start_timer( TDSRRCTMR_SIB_STOP_PARTITION_TIMER, (remaining_time - TDSRRCSIB_MAX_WAIT_TIME_FOR_SIBS/2));
              }
            }
            else
            {
              tdsrrcsib_g_in_data_call = FALSE;
            }
          }
        }
        if( tdssib_events_ptr->event[index].event_name != TDSRRCSIB_UPDATE_SIBS )
        {
          tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_MIB,
                                             tdssib_events_ptr->event[index].tempholder.mib, index);
        }

        TDSRRC_MSG2(MSG_LEGACY_HIGH,"G data call flag %d,int_event_type %d",tdsrrcsib_g_in_data_call,tdssib_events_ptr->event[index].int_event.int_event_type);
        /* This will send empty table as scheduling is not yet updated */
        /*if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
               (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_CSP) &&
               (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL))*/
        if((tdsrrcsib_g_in_data_call)&&
            (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL)&&
            (tdssib_events_ptr->event[index].int_event.int_event_type != TDSRRCSIB_INT_SIB7_EVENT)
          )
        {
          if(tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SB1]
              && tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed == TRUE
              && tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_received == FALSE
              && tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP
              && tdssib_events_ptr->sib_sched[tdsrrc_SB1].sfn_offset[0] % (TDSRRCSIB_SLEEP_SFN + TDSRRCSIB_WAKE_SFN) < TDSRRCSIB_SLEEP_SFN)
          {
            /* Try to receive SB1 firstly if scheduled. */
            tdsrrcsib_pattern_invert = TRUE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH, "Optimization for receiving SB1 ASAP takes effect");
          }
     
          tdsrrcsib_send_empty_table(index, TRUE);
          tdsrrctmr_start_timer(TDSRRCTMR_SIB_EMPTY_MASK_TIMER, TDSRRCSIB_EMPTY_FRAMES);
        } 

        if(((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN )|| 
            (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN)) &&
           (tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS))
        {
          /* Check to see if PLMN Type is not correct here. */
          if(tdsrrcsib_check_if_plmn_is_gsm_map(index,FALSE) == FALSE)
          {
            return FALSE;
          }
          else
          {
            uint32 count=0;  //counter purposes for 2 events
  
            /* Get the plmn_id received in the current MIB */
            tdsrrc_translate_gsm_map_plmn_id 
              (&(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.u.gsm_MAP->plmn_Identity),
               &plmn);
         
            /*In Network sharing cell there may be more than one PLMN broadcasted
               in a cell, so pass MIB pointer.this csp function will check if its N/W 
               shared cell then it will extract all the PLMN's for its processing*/
            if(FALSE == tdsrrccsp_is_in_bplmn_list(plmn
                                              ,tdssib_events_ptr->event[index].tempholder.mib
              ))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"PLMN NOT in BPLMN list");
              
              /* First clear the SIB events database */
              for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
              {
                if (tdssib_events_ptr->event[count].event_active == TRUE)
                {
                  tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,TRUE,TDSSIB_EVENT_FAILED_OTHER, FALSE);
                }
              }//loop thru sib events
            }//not in bplmn list
          }
        }
      }

      /* Here we do some special processing if the event is
      TDSRRCSIB_UPDATE_SIBS. This event is used to update the
      SIBs on the cell we've camped on. We already know
      the sib database index for this cell, so we can
      do value-tag comparisons without waiting for SIB3.
      First check if the value-tag of the MIB is the one 
      we're expecting */
      if( ( tdssib_events_ptr->event[index].tempholder.mib_decoded == TRUE ) &&
          ( tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS )
        )
      {
        /* Check if this MIB's value-tag matches the value-tag
        we received with the command */
        /* Here need to by pass the check for 6hr UPDATE_SIBs, */
		/* If the recieved mib value tag in MIB is not same as stored and the one from paging, RRC should handle such case.  */
        if((tdssib_events_ptr->event[index].tempholder.mib->mib_ValueTag ==
            tdssib_events_ptr->mib_value_tag_from_command )||
            ( ( tdssib_events_ptr->event[index].tempholder.mib->mib_ValueTag != (uint8)tdssib_events_ptr->mib_value_tag_from_command ) &&
              tdssib_events_ptr->event[index].tempholder.mib->mib_ValueTag != (uint8)tdssibdb.cell[tdssibdb.active_cell_index].sib_value_tag[tdsrrc_MIB])||
           (tdssib_events_ptr->update_sibs_cause == TDSRRCSIB_CSFB_CALL_CANCL)
           ||(tdsrrcsib_is_event_sixhrs_sib_update(index) == TRUE)
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"New MIB rxed for UPDATE_SIBS event");
          
          tdsrrcsib_update_sched_info_from_mib_sb((uint32)tdsrrc_MIB,
                                             tdssib_events_ptr->event[index].tempholder.mib, index);
          
          /* We got the MIB that we needed. We can do value-tag
          comparisons. Before that, make sure we have the 
          database index for this event. For TDSRRCSIB_UPDATE_SIBS event,
          the database index is the current active cell's index.*/
          tdssib_events_ptr->event[index].sib_db_index = tdssibdb.active_cell_index;
          tdssib_events_ptr->event[index].sib_db_index_found = TRUE;

          /* Now do the value-tag comparisons */
          tdsrrcsib_update_event_for_received_value_tags( 
            tdsrrc_MIB, /* For the MIB */
            index,                      /* Event Index */
            tdssib_events_ptr->event[index].sib_db_index);

          /* Also free the MIB since we no longer need it */
          tdsrrcsibdb_free_sib( tdsrrc_MasterInformationBlock_PDU,
            (void *)tdssib_events_ptr->event[index].tempholder.mib );
          tdssib_events_ptr->event[index].tempholder.mib = NULL;
          tdssib_events_ptr->event[index].tempholder.mib_sfn_prime = TDSRRCSIB_INVLID_SFN_PRIME;

          //if SB1 is already received, i.e. v-tag matches, then update v-tags immediately
          //for the received value tags
          if((tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_received == TRUE)&&
             (tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_decoded == TRUE))
          {
            //since we alredy have sb1 decoded, set this flag to true
            tdssib_events_ptr->event[index].tempholder.sb1_decoded=TRUE;

            /* Now do the value-tag comparisons */
            tdsrrcsib_update_event_for_received_value_tags( 
              tdsrrc_SB1,       /* For the SB1 */
              index,                      /* Event Index */
              tdssib_events_ptr->event[index].sib_db_index);


            if(FAILURE == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs
               (tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sibs.sb1, 
                tdsrrc_SB1, 
                index))
            {
              tdsrrcsib_process_sib_scheduling_info_incomplete(index); 
              return TRUE;
            }
          }

          //if SB2 is already received, i.e. v-tag matches, then update v-tags immediately
          //for the received value tags
          if((tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_received == TRUE)&&
             (tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_decoded == TRUE))
          {
            //since we alredy have sb1 decoded, set this flag to true
            tdssib_events_ptr->event[index].tempholder.sb2_decoded=TRUE;

            /* Now do the value-tag comparisons */
            tdsrrcsib_update_event_for_received_value_tags( 
              tdsrrc_SB2,       /* For the SB2 */
              index,                      /* Event Index */
              tdssib_events_ptr->event[index].sib_db_index);

            if(FAILURE == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs
               (tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sibs.sb2, 
                tdsrrc_SB2, 
                index))
            {
              tdsrrcsib_process_sib_scheduling_info_incomplete(index); 
              return TRUE;
            }

          }

          /* Now that we have the updated MIB, we can decode and store 
          that MIB and whatever SIBs we've received in this pass. */
          TDSRRC_MSG0(MSG_LEGACY_MED,"Decoding received SIBs");
          tdsrrcsib_decode_completed_sibs(index);

        }
        else
        {
#ifdef FEATURE_RRC_SIB_DEBUG
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB value-tag not changed yet: ignore");
#else
          TDSRRC_MSG0(MSG_LEGACY_LOW,"MIB value-tag not changed yet: ignore");
#endif /* FEATURE_RRC_SIB_DEBUG */
          /* Remove this MIB from tempholder - we won't use this MIB. First
          call the function that frees the mib, then indicate that the mib
          has not been received yet. */
          tdsrrcsibdb_free_sib(  tdsrrc_MasterInformationBlock_PDU,
           (void *)tdssib_events_ptr->event[index].tempholder.mib );
          tdssib_events_ptr->event[index].tempholder.mib = NULL;

          tdssib_events_ptr->event[index].tempholder.mib_decoded = FALSE;
          tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received = FALSE;

          tdssib_events_ptr->event[index].tempholder.mib_sfn_prime = TDSRRCSIB_INVLID_SFN_PRIME;
        }
      }

      /* Generally we use PLMN Id and Cell Id for indexing into the database.
      But to speed up reselections, use Frequency and Primary Scrambling Code
      to find the SIB DB index.This should be used only for cell reselections. */
      
      if((tdssib_events_ptr->event[index].tempholder.mib_decoded) && 
         (tdssib_events_ptr->event[index].for_serving_cell == FALSE))   
      {
        /* Check to see if PLMN Type is not correct here. */
        if(tdsrrcsib_check_if_plmn_is_gsm_map(index,FALSE) == FALSE)
        {
          return FALSE;
        }
        else
        {
          /* Get the plmn_id received in the current MIB */
          tdsrrc_translate_gsm_map_plmn_id 
          (&(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.u.gsm_MAP->plmn_Identity),
             &plmn);
  
          /* Check if this cell already exists. Do the search based on plmn_id, frequency, 
          and scrambling code */
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Checking SIBDB index for Freq %d,CPID %d",tdssib_events_ptr->ngh_bcch.freq,tdssib_events_ptr->ngh_bcch.cpid );
  
          tdssib_events_ptr->event[index].sib_db_index =
            tdsrrcsibdb_find_cell_index_for_plmn_freq_cpid(plmn,tdssib_events_ptr->ngh_bcch.freq,
                                                        tdssib_events_ptr->ngh_bcch.cpid,index);
  
          if( tdssib_events_ptr->event[index].sib_db_index != TDSSIBDB_INVALID_INDEX )
          {
            /* Although we already know the index based on freq and cpid
            call the tdsrrcsibdb_create_cell_index which will do the necessary
            tdssibdb processing for using this index for this event. */
            tdssib_events_ptr->event[index].sib_db_index = 
              tdsrrcsibdb_create_cell_index( 
               tdssibdb.cell[(tdssib_events_ptr->event[index].sib_db_index)].plmn_id, 
               tdssibdb.cell[(tdssib_events_ptr->event[index].sib_db_index)].cell_id, 
                                index);
            tdssib_events_ptr->event[index].sib_db_index_found = TRUE;
  
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIBDB index for Freq and CPID found! DB-Index %d,Cell-id %d",tdssib_events_ptr->event[index].sib_db_index,tdssibdb.cell[(tdssib_events_ptr->event[index].sib_db_index)].cell_id);
  
  
            /* Since we've found the database index and we've gotten the
            MIB, we can check the MIB value tags. */
            tdsrrcsib_update_event_for_received_value_tags( 
              tdsrrc_MIB, /* For the MIB */
              index,                      /* Event Index */
              tdssib_events_ptr->event[index].sib_db_index);
          
            /* We no longer need the temporarily decoded MIB. Free
            the PDU using the tdsrrcsibdb_free_sib function */
            tdsrrcsibdb_free_sib( tdsrrc_MasterInformationBlock_PDU,
              (void *)tdssib_events_ptr->event[index].tempholder.mib );
            tdssib_events_ptr->event[index].tempholder.mib = NULL;
            tdssib_events_ptr->event[index].tempholder.mib_sfn_prime = TDSRRCSIB_INVLID_SFN_PRIME;
    
            /* Now that we have an index, we can decode and store 
            whatever SIBs we've received in this pass. */
            TDSRRC_MSG0(MSG_LEGACY_MED,"Decoding received SIBs");
            tdsrrcsib_decode_completed_sibs(index);
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIBDB index for Freq and CPID not found");
          }
        }
      }


    }/* end if - MIB was received this pass */

    if((tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_received)
      && (tdssib_events_ptr->event[index].tempholder.mib_decoded)
      &&(tdssib_events_ptr->event[index].tempholder.sb1_decoded == FALSE))
    {
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
      if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
       (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
       (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
      )

      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: SB1 has been received");

        if ((tdsrrcsib_bplmn_sibs.sb1 == NULL)) 
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: Storing SB1 in bplmn sibs database");

          /* Get the PDU length in bytes */
          sb1_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                            (tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].
                             sib_tot_length);

          /* Decode and store SIB1 in bplmn sib database */
          tdsrrcsib_bplmn_sibs.sb1 = 
          (tdsrrc_SysInfoTypeSB1*)tdsrrcasn1_decode_pdu( 
                             (void *)(tdssib_events_ptr->event[index].
                             tsib[tdsrrc_SB1].sib_data_ptr), /* Encoded SIB3 */
                             tdsrrc_SysInfoTypeSB1_PDU, /* SIB3's PDU number         */
                             sb1_pdu_length       /* SIB3's length in bytes    */
                             );

          if(tdsrrcsib_bplmn_sibs.sb1 == NULL)
          {
             TDSRRC_MSG0(MSG_LEGACY_ERROR,"SB1 decoding failed");
          }
          else
          {
            sb_ptr = &tdsrrcsib_bplmn_sibs.sb1->sib_ReferenceList;
            ix=0;
			for(ix=0;ix< sb_ptr->n;ix++)
            
            {
              tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&sb_ptr->elem[ix].sib_Type,
                                                    &sib_type_tag);
              if(sib_type_tag.sib_type == tdsrrc_SIB5 ||
                  sib_type_tag.sib_type == tdsrrc_SIB5bis)
              {
                break;
              }
              
            }

            if((ix < sb_ptr->n)&&
                ((sib_type_tag.sib_type == tdsrrc_SIB5)||
                  (sib_type_tag.sib_type == tdsrrc_SIB5bis)))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"BAND_TYPE_DETECTION - SIB5/SIB5bis scheduled in SB1 ");
              tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed = FALSE;
            }
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN:SB1 already stored in BPLMN DB");
        }
      }
#endif

      if(FAILURE == tdsrrcsib_process_scheduling_block_sched_info_before_sib_db_decoded
         (index, tdsrrc_SB1))
      {
        tdsrrcsib_process_sib_scheduling_info_incomplete(index); 
        return TRUE;
      }
    }
  
    if((tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_received)
      && (tdssib_events_ptr->event[index].tempholder.mib_decoded)
      &&(tdssib_events_ptr->event[index].tempholder.sb2_decoded == FALSE))
    {
    
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
      if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
       (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
       (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
      )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: SB2 has been received");

        if ((tdsrrcsib_bplmn_sibs.sb2 == NULL)) 
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: Storing SB2 in bplmn sibs database");

          /* Get the PDU length in bytes */
          sb2_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                            (tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].
                             sib_tot_length);

          /* Decode and store SIB1 in bplmn sib database */
          tdsrrcsib_bplmn_sibs.sb2 = 
          (tdsrrc_SysInfoTypeSB2*)tdsrrcasn1_decode_pdu( 
                             (void *)(tdssib_events_ptr->event[index].
                             tsib[tdsrrc_SB2].sib_data_ptr), /* Encoded SIB3 */
                             tdsrrc_SysInfoTypeSB2_PDU, /* SIB3's PDU number         */
                             sb2_pdu_length       /* SIB3's length in bytes    */
                             );

          if(tdsrrcsib_bplmn_sibs.sb2 == NULL)
          {
             TDSRRC_MSG0(MSG_LEGACY_ERROR,"SB2 decoding failed");
          }
          else
          {
            sb_ptr = &tdsrrcsib_bplmn_sibs.sb2->sib_ReferenceList;
			ix=0;
			for(ix=0;ix< sb_ptr->n;ix++)
            {
              tdsrrcsibdb_get_sib_type_from_sib_type_and_tag(&sb_ptr->elem[ix].sib_Type,
                                                    &sib_type_tag);
              if(sib_type_tag.sib_type == tdsrrc_SIB5 ||
                  sib_type_tag.sib_type == tdsrrc_SIB5bis)
              {
                break;
              }
              
            }

            if((sb_ptr->n > ix)&&
                ((sib_type_tag.sib_type == tdsrrc_SIB5)||
                  (sib_type_tag.sib_type == tdsrrc_SIB5bis)))
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"BAND_TYPE_DETECTION - SIB5/SIB5bis scheduled in SB2 ");
              tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed = FALSE;
            }
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN:SB2 already stored in BPLMN DB");
        }
      }
#endif

      if(FAILURE == tdsrrcsib_process_scheduling_block_sched_info_before_sib_db_decoded
         (index, tdsrrc_SB2))
      {
        tdsrrcsib_process_sib_scheduling_info_incomplete(index); 
        return TRUE;
      }
    }

    /* Then check if the SIB3 was received in this pass. 
    We do this only if the SIBDB index wasn't already found in the
    if statement above. This may be the case for UPDATE_SIB events. */
    if( (tdssib_events_ptr->event[index].sib_db_index_found != TRUE)
      && (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_received)
      && (!(tdssib_events_ptr->event[index].tempholder.sib3_decoded)) )
    {
      /* SIB3 has been received this time */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB3 Rx'ed this pass");
      /* Decode SIB3 to the local copy - For this, first we need the PDU
      length in bytes. Divide the PDU lenth in bits with 8 and add 1 since
      we can err on the high side for the length. */
      pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                    (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].
                     sib_tot_length);
      (void)tdsrrcsib_check_if_connected_mode_sibs_present
      (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_data_ptr[0],
      tdsrrc_SIB3);
      /* Call tdsrrcasn1_decode_pdu to do ASN1 decoding. Since it returns a 
      void pointer, typecast it to the MIB's type. */
      tdssib_events_ptr->event[index].tempholder.sib3 =
        (tdsrrc_SysInfoType3 *)tdsrrcasn1_decode_pdu( 
          (void *)(tdssib_events_ptr->event[index].
                 tsib[tdsrrc_SIB3].sib_data_ptr), /* Encoded SIB3 */
          tdsrrc_SysInfoType3_PDU,            /* SIB3's PDU number         */
          pdu_length                       /* SIB3's length in bytes    */
                        );
      
      if( tdssib_events_ptr->event[index].tempholder.sib3 == NULL )
      {
        /* Decode did not work */
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB3 decode failed");

        /*Reset all the variables for this SIB so that we can try to
        read it again */
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_received = FALSE;
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_decoded = FALSE;
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].seg_count = 0;
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].seg_index = 
          (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_tot_length = 0;
      }
      else
      {
        tdssib_events_ptr->event[index].tempholder.sib3_decoded = TRUE;
        tdsrrc_translate_gsm_map_plmn_id(&(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.u.gsm_MAP->plmn_Identity),&plmn);

        /*Do not perform suitablity check for Cell ID in DCH scans*/
       if((tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
#ifdef FEATURE_LTE_TO_TDSCDMA
          &&(!tdsrrccsp_check_cgi_scan())
#endif
         )
        {
          /* Below function performs, White list check, cell bar status. If it finds a cell as barred sends cell bar req to l1 */
          tdsrrccsp_initial_suitability_check(tdssib_events_ptr->event[index].tempholder.mib,
                                           tdssib_events_ptr->event[index].tempholder.sib3,
                                           plmn,
                                           &status);
        }
        if(status != TDSSIB_EVENT_SUCCEEDED)
        {
          tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                            TRUE,
                                            status, 
                                            TRUE);
          return FALSE;
        }
        else if((!tdssib_events_ptr->event[index].for_serving_cell)&&(FALSE/*s criteria check here*/))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSG: Suitability criteria check failed");
          tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,
                                               TRUE,
                                               TDSSIB_EVENT_FAILED_OTHER, 
                                               TRUE);
          return FALSE;

        }
      }

      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB3 MSG-Ptr is 0x%x",tdssib_events_ptr->event[index].tempholder.sib3);
    }/* end if -  SIB3 was received this pass */

    /* Now if we have both MIB and SIB3 decoded, we can get the
    PLMN and Cell Id and find an index into the sib datbase for this cell
    We do this only if the SIBDB index wasn't already found in the
    if statement above. This may be the case for UPDATE_SIB events. */
    
    if( (tdssib_events_ptr->event[index].sib_db_index_found != TRUE)
      &&(tdssib_events_ptr->event[index].tempholder.mib_decoded)
      &&(tdssib_events_ptr->event[index].tempholder.sib3_decoded) )
    {
      if(tdssib_events_ptr->event[index].tempholder.mib != NULL)
      {
        /* Check to see if PLMN Type is not correct here. */
        if(!((TDSRRC_CHECK_COMMON_MSG_TYPE(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type, 
              rrc_PLMN_Type_gsm_MAP))||
             ((TDSRRC_CHECK_COMMON_MSG_TYPE(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type, 
              rrc_PLMN_Type_gsm_MAP_and_ANSI_41))))
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Acquired cell PLMN not GSM-MAP!");
          /* No need process this event further, just send a failure
            to the procedure that requested this sib event. */
          non_gsm_map_plmn = TRUE;
          tdsrrcsib_post_process_failed_event( index, /* Event Index                 */
                                                TRUE,  /* Confirm Command is required */
                                                TDSSIB_EVENT_FAILED_INVALID_SIBS, /* Failure cause */
                                                TRUE   /* Tear down S_BCCH if it's up */
                                              );
        }
      }

      if( non_gsm_map_plmn != TRUE )
      { 
        if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN ) ||
           (tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN|| /* There seems sth wrong here */
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN)) ||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN) 
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: MIB and SIB3 received and decoded");
          
          if(tdsrrcsib_bplmn_sibs.mib == NULL)
          {
            uint32 mib_pdu_length;
            
            /* Get the PDU length in bytes */
            mib_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                              (tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].
                               sib_tot_length);

            /* Decode and store SIB1 in bplmn sib database */
            tdsrrcsib_bplmn_sibs.mib = 
              (tdsrrc_MasterInformationBlock *)tdsrrcasn1_decode_pdu( 
              (void *)(tdssib_events_ptr->event[index].
                      tsib[tdsrrc_MIB].sib_data_ptr), /* Encoded MIB */
                      tdsrrc_MasterInformationBlock_PDU,  /* MIB's PDU number        */
                      mib_pdu_length                       /* MIB's Length in bytes   */
                      );

            if(tdsrrcsib_bplmn_sibs.mib != NULL)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: Storing MIB in bplmn sibs database");
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
              if(tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN ||
                 tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN ||
                 tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN )
              {
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"SB1 needed = %d SB2 needed = %d",tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed,tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed);
                sibSb_ptr = &tdsrrcsib_bplmn_sibs.mib->sibSb_ReferenceList;
                
				for(ix=0;ix< sibSb_ptr->n;ix++)                
                {
                  tdsrrcsibdb_get_sib_type_from_sibsb_type_and_tag(&sibSb_ptr->elem[ix].sibSb_Type,
                                                                      &sib_type_tag);
                  if(sib_type_tag.sib_type == tdsrrc_SIB5 ||
                     sib_type_tag.sib_type == tdsrrc_SIB5bis)
                  {
                    break;
                  }
           
                }
                if(((sibSb_ptr->n  > ix)&& (ix !=0))&&
                  ((sib_type_tag.sib_type == tdsrrc_SIB5) ||
                  (sib_type_tag.sib_type == tdsrrc_SIB5bis)))
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-SIB5/SIB5bis found in MIB");
                  tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed = FALSE;
                  tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed = FALSE;
                }
              }
#endif
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB decode failure");
            }
          }
          if(tdsrrcsib_bplmn_sibs.sib3 == NULL)
          {
            uint32 sib3_pdu_length;
            
            /* Get the PDU length in bytes */
            sib3_pdu_length = TDSGET_PDU_LENGTH_IN_BYTES
                              (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].
                               sib_tot_length);

            /* Decode and store SIB1 in bplmn sib database */
            tdsrrcsib_bplmn_sibs.sib3 = 
            (tdsrrc_SysInfoType3 *)tdsrrcasn1_decode_pdu( 
                               (void *)(tdssib_events_ptr->event[index].
                               tsib[tdsrrc_SIB3].sib_data_ptr), /* Encoded SIB3 */
                               tdsrrc_SysInfoType3_PDU, /* SIB3's PDU number         */
                               sib3_pdu_length       /* SIB3's length in bytes    */
                               );

            if(tdsrrcsib_bplmn_sibs.sib3 != NULL)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"WTOW BPLMN: Storing SIB3 in bplmn sibs database");
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB3 decode failure");
            }
          }
           
          /* We no longer need the temporarily decoded MIB and SIB3. Free
             the PDUs using the tdsrrcsibdb_free_sib function */
          tdsrrcsib_free_tsib(index);

          /* Check to see of other SIBS are received */
          (void)tdsrrcsib_decode_completed_sibs_for_bplmn(index);
        }
        else
        {
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
          if(tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
          {
            /*just for printing........Dont do any processing here,
             *as sib_scan_type TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN 
             *we dont store any SIB's, return cell I.D from tempholder.sib3
             */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS:MIB and SIB3 received and decoded");
          }
          else
#endif
          {
            if(tdssib_events_ptr->event[index].tempholder.mib!= NULL)
            {
              /* Get the plmn id for this cell. Make sure the
              PLMN id is a GSM-MAP PLMN. Convert it to the internal format used by RRC. */
              if(TDSRRCGET_PLMN_TYPE((&tdssib_events_ptr->event[index].tempholder.mib->plmn_Type))
                                                      == TDSRRCGET_PLMN_TYPE_TAG(gsm_MAP)
              )
              {
                tdsrrc_translate_gsm_map_plmn_id 
                  (&(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.u.gsm_MAP->plmn_Identity),
                   &plmn
                  );
              }
              else if( TDSRRCGET_PLMN_TYPE((&tdssib_events_ptr->event[index].tempholder.mib->plmn_Type))
                                                      == TDSRRCGET_PLMN_TYPE_TAG(gsm_MAP_and_ANSI_41)
              )
              {
                tdsrrc_translate_gsm_map_plmn_id 
                  (&(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.u.
                   gsm_MAP_and_ANSI_41->plmn_Identity),
                   &plmn
                  );
              }
              /* Since the PLMN is good, get the cell id for this cell. */
              cell = tdsrrc_translate_cell_id
                      ( &(tdssib_events_ptr->event[index].tempholder.sib3->cellIdentity));
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Temp SIB3 MSG-Ptr is 0x%x",tdssib_events_ptr->event[index].tempholder.sib3);
              /* Get the SIB Database index for this cell */
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Checking SIBDB index for CellId %d and MCC %d MNC %d", 
                       cell, TDSRRC_CSP_GET_INT_MCC(plmn), TDSRRC_CSP_GET_INT_MNC(plmn));

              /* CR484101 early check S-critiria for X2T reselection */
              /* also CR687705 to apply to X2T redirect as well */
              if (tdssib_events_ptr->event[index].tempholder.sib3
                  && tdssib_events_ptr->event[index].for_serving_cell
                  && tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL
                  && tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS
                  && ((tdsrrccsp_get_curr_scan() == RRC_CSP_GSM_TO_TDS_CELL_RESELECTION_SCAN)
                      || (tdsrrccsp_get_curr_scan() == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN))
                 )
              {
                 tdsrrc_SysInfoType3 *sib3_ptr = tdssib_events_ptr->event[index].tempholder.sib3;
                 boolean status = TRUE;

                 TDSRRC_MSG1(MSG_LEGACY_HIGH,"X2T IRAT scan %d check for S criteria",tdsrrccsp_get_curr_scan());

                 /* Find Q_RxlevMin_offset */
                 /* ZZTBD: FDD has condition rrc_csp_int_data.gsm_camped_suitably checking here */
                 if (sib3_ptr->m.v4b0NonCriticalExtensionsPresent &&
                     sib3_ptr->v4b0NonCriticalExtensions.m.v590NonCriticalExtensionPresent &&
                     sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.m.v5c0NoncriticalExtensionPresent && 
                     sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v5c0NoncriticalExtension.m.v670NonCriticalExtensionPresent &&
                     sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v5c0NoncriticalExtension.v670NonCriticalExtension.m.v770NonCriticalExtensionPresent)
                 {
                     tdsrrc_Q_RxlevMin_Offset Q_RxlevMin_offset = 
                       sib3_ptr->v4b0NonCriticalExtensions.v590NonCriticalExtension.v5c0NoncriticalExtension.v670NonCriticalExtension.v770NonCriticalExtension.sysInfoType3_v770ext.q_RxlevMin_Offset;
  
                     TDSRRC_MSG3(MSG_LEGACY_HIGH,"X2T IRAT scan, %d Rxlevmin %d, q_RxlevMin_Offset %d", 
                                            tdsrrccsp_get_curr_scan(),
                                            (int16)((sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1),
                                            (int16)(Q_RxlevMin_offset * 2));
            
#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
                     status = tdsl1mcmd_is_s_criteria_met_tmp(
                             (int16)((sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1 + (Q_RxlevMin_offset * 2)), 
                             (int16)(sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power));
#else
                     status = tdsrrcff_l1_is_s_criteria_met(
                             (int16)((sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1 + (Q_RxlevMin_offset * 2)), 
                             (int16)(sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power));
#endif
                 }
                 else
                 {   
                    TDSRRC_MSG3(MSG_LEGACY_HIGH,"X2T IRAT scan %d, Rxlevmin %d, MaxULTxPower %d", 
                                            tdsrrccsp_get_curr_scan(),
                                            (int16)((sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1),
                                            (int16)(sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power));
#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
                    status = tdsl1mcmd_is_s_criteria_met_tmp(
                              (int16)((sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1), 
                              (int16)(sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power));
#else
                    status = tdsrrcff_l1_is_s_criteria_met(
                              (int16)((sib3_ptr->cellSelectReselectInfo.modeSpecificInfo.u.tdd->q_RxlevMin * 2) + 1),
                              (int16)(sib3_ptr->cellSelectReselectInfo.maxAllowedUL_TX_Power));
#endif
                 }

                 TDSRRC_MSG3(MSG_LEGACY_ERROR, "X2T IRAT scan %d, S criteria met = %d, NV66024=0x%x", tdsrrccsp_get_curr_scan(), status, tds_nv_optional_feature_list);

                 if (!status)
                 {
                    TDSRRC_MSG1(MSG_LEGACY_ERROR, "X2T IRAT scan %d,S criteria not met. Exit SIB procedure",tdsrrccsp_get_curr_scan());
                    tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,TRUE,TDSSIB_EVENT_FAILED_SCRITERIA,TRUE);
                    return FALSE;
                 }
              }
              
              tdssib_events_ptr->event[index].sib_db_index = tdsrrcsibdb_create_cell_index( plmn, cell,index);
              tdssib_events_ptr->event[index].sib_db_index_found = TRUE;
              if((tdsrrcccm_get_serv_plmn_cellid(&cell_id,&plmn_id) == TDSRRC_NEW_CELL_SELECTED)&&
                 (tdssib_events_ptr->event[index].for_serving_cell == FALSE)&&
                 (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL)
                )
              {
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Current Camped CELL I.D#%d, PLMN I.D MCC %d MNC %d",cell_id,
                                                                              TDSRRC_CSP_GET_INT_MCC(plmn_id), 
                                                                              TDSRRC_CSP_GET_INT_MNC(plmn_id));
                if((cell_id == cell)&&
                  (tdsrrc_bcch_plmns_match(plmn_id,plmn) == TRUE)
                  )
                {
                  tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].six_hr_count = 
                    TDSMAX_CELL_INDEX_COUNT_SIXHR_CELL_SEL_RESEL;
                   TDSRRC_MSG1(MSG_LEGACY_ERROR,"Target cell same as Current Camped Cell,Cell I.D %d",cell); 
                   tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)index,TRUE,TDSSIB_EVENT_FAILED_OTHER, FALSE);
                   tdsrrcsib_initiate_cell_selection();
                   return FALSE;
                }
              }
              /* Since we've found the database index and we've gotten the
              MIB, we can check the MIB value tags. */
              tdsrrcsib_update_event_for_received_value_tags( 
                tdsrrc_MIB, /* For the MIB */
                index,                      /* Event Index */
                tdssib_events_ptr->event[index].sib_db_index);
              /* We no longer need the temporarily decoded MIB and SIB3. Free
              the PDUs using the tdsrrcsibdb_free_sib function */
              tdsrrcsib_free_tsib(index);
              /* Now that we have an index, we can decode and store 
              whatever SIBs we've received in this pass. */
              TDSRRC_MSG0(MSG_LEGACY_MED,"Decoding received SIBs");
              tdsrrcsib_decode_completed_sibs(index);
            }
            else
            {
              TDSRRC_MSG1(MSG_LEGACY_ERROR,"Temp MIB ptr is NULL Value = %d",tdssib_events_ptr->event[index].tempholder.mib);
            }
          }
        }
      }/* end if non_gsm_map_plmn != TRUE */
    }/* end if both mib and sib3 decoded */
  }/* end if index not found */
  return TRUE; 
}/* end function tdsrrcsib_process_completed_sibs */

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
  , boolean for_deferred_sib
  , tdsrrc_SIB_type *def_sib_stored_in_sb1_sb2
#endif
)
{
  tdsrrc_SIBSb_ReferenceList *sb_ptr = NULL; /* Local pointer to SIBSb ref list   */
  tdsrrc_SIB_ReferenceList *ptr = NULL;      /* Local pointer to SIB ref list     */
  boolean sib_schd_found = FALSE; /* Indicate if sib schedule is found */
  boolean sb1_found = FALSE;      /* Indicate if sb1 is found incase
                                     the sib is not present in MIB     */
  boolean sb2_found = FALSE;      /* Indicate if sb2 is found incase
                                     the sib is not present in MIB     */
  uint32 sib_sb_tag = TDSRRC_TOT_SIBS; /* To hold sib_sb value tag type.
                                     Initialize to invalid value       */
  uint32 sib_tag = TDSRRC_TOT_SIBS;  /* To hold sib value tag type.
                                     Initialize to invalid value       */
  tdsrrc_MasterInformationBlock *mib_ptr = NULL; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr = NULL;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr = NULL;         /* Local pointer to SB2       */
  uint32 n_index=0, n_value=0;
  
  if( sib == tdsrrc_MIB )
  {
    /* MIB is always present - no need to check anything */
    return( TRUE );
  }

  /* First determine the sib value tag type definition for this sib */
  switch( sib )
  {
    case tdsrrc_SIB1 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType1);
      break;

    case tdsrrc_SIB2 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType2);
      break;

    case tdsrrc_SIB3 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType3);
      break;

    case tdsrrc_SIB4 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType4);
      break;

    case tdsrrc_SIB5 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType5);
      break;

    case tdsrrc_SIB6 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType6);
      break;

    case tdsrrc_SIB7 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType7);
      break;

    case tdsrrc_SIB11 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType11);
      break;

    case tdsrrc_SIB12 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType12);
      break;

    case tdsrrc_SIB18 :
      sib_sb_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIBSb_TypeAndTag_sysInfoType18);
      break;
  
    case tdsrrc_SB1 :
      sib_sb_tag = TDSRRCGET_SIB_TYPE_AND_TAG_VALUE_SYS_INFO_TYPE(sysInfoTypeSB1);
      break;
  
    case tdsrrc_SB2 :
      sib_sb_tag = TDSRRCGET_SIB_TYPE_AND_TAG_VALUE_SYS_INFO_TYPE(sysInfoTypeSB2);
      break;
  
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown SIB%d",sib);
      break;
  }/* end switch */

  if( sib_sb_tag == TDSRRC_TOT_SIBS )
  {
    /* Unknown SIB - scheduling info not present */
    return( FALSE );
  }
  

  if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_MIB] )
  {
    /* If the MIB is valid */
    mib_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB,dbindex);
    if(mib_ptr != NULL)
    {
      sb_ptr = &mib_ptr->sibSb_ReferenceList;

    n_index = 0;
    n_value = sb_ptr->n;

    while( n_value != 0 )
      {
        /* Check if the given sib is scheduled in the MIB */
      if(TDSRRCGET_SIB_TYPE_AND_TAG((&sb_ptr->elem[n_index].sibSb_Type)) == sib_sb_tag)
        {
          /* We found it */
          sib_schd_found = TRUE;
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if(for_deferred_sib == TRUE)
          {
            *def_sib_stored_in_sb1_sb2 = tdsrrc_MIB;
          }
#endif
          break; /* Out of the while loop */
        }

        /* Else check if this is for SB1 */
      if(TDSRRCCHECK_SIBSb_TypeAndTag((&sb_ptr->elem[n_index].sibSb_Type),sysInfoTypeSB1))
        {
          /* We found it */
          sb1_found = TRUE;
        }

        /* Else check if this is for SB2 */
      if(TDSRRCCHECK_SIBSb_TypeAndTag((&sb_ptr->elem[n_index].sibSb_Type),sysInfoTypeSB2))
        {
          /* We found it */
          sb2_found = TRUE;
        }
        /* Now increment the pointer */
      n_index++;
      n_value--;
      }/* end while */

      if( sib_schd_found == TRUE )
      {
        /* This sib is scheduled in the MIB */
        return( TRUE );
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB is NULL!");
      return( FALSE );
    }
  }/* end if MIB is valid */

  else
  {
    /* Can't find MIB! */
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"MIB not present in srv cell!");
    return( FALSE );
  }


  /* If the given sib was not scheduled in the MIB. Check if the given
  sib itself is a scheduling block */
  if( sib == tdsrrc_SB1 )
  {
    if( sb1_found == TRUE )
    {
      /* Scheduling block 1 is scheduled in MIB */
      return(TRUE);
    }
    else
    {
      return(FALSE);
    }
  }
  /* Same for SB2 */
  if( sib == tdsrrc_SB2 )
  {
    if( sb2_found == TRUE )
    {
      /* Scheduling block 2 is scheduled in MIB */
      return(TRUE);
    }
    else
    {
      return(FALSE);
    }
  }

  /* If we get here  the SIB is not scheduled in the MIB and
  it is not a scheduling block. So we need to find if it is
  scheduled in one of the scheduling blocks. First note the 
  tag type for this sib */
  switch( sib )
  {
    case tdsrrc_SIB1 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType1);
      break;

    case tdsrrc_SIB2 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType2);
      break;

    case tdsrrc_SIB3 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType3);
      break;

    case tdsrrc_SIB4 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType4);
      break;

    case tdsrrc_SIB5 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType5);
      break;

    case tdsrrc_SIB6 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType6);
      break;

    case tdsrrc_SIB7 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType7);
      break;

    case tdsrrc_SIB11 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType11);
      break;

    case tdsrrc_SIB12 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType12);
      break;

    case tdsrrc_SIB18 :
      sib_tag = TDSRRCGET_COMMON_CHOICE_TYPE(rrc_SIB_TypeAndTag_sysInfoType18);
      break;
  
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown SIB%d",sib);
      break;
  }/* end switch */

  if( sib_tag == TDSRRC_TOT_SIBS )
  {
    /* Unknown SIB - scheduling info not present */
    return( FALSE );
  }

  /* Now check if the given sib is scheduled in one
  of the scheduling blocks */
  if( sb1_found == TRUE )
  {
    /* Look in SB1 for this SIB  schedule */
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SB1] )
    {
      /* If the SB1 is valid */
      /* If the MIB is valid */
      sb1_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB1,dbindex);
      if(NULL != sb1_ptr)
      {
        ptr = &(sb1_ptr->sib_ReferenceList);
      }
      n_index = 0;
      n_value = 0;

      if(ptr != NULL)
      {
        n_value = ptr->n;
      }
        
      while( n_value != 0 )
        {
          /* Check if this is for SIB */
        if(TDSRRCGET_SIB_TYPE_AND_TAG((&ptr->elem[n_index].sib_Type)) == sib_tag)
          {
            /* We found it */
            sib_schd_found = TRUE;
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            if(for_deferred_sib == TRUE)
            {
              *def_sib_stored_in_sb1_sb2 = tdsrrc_SB1;
            }
#endif
            break; /* Out of the while loop */
          }
          /* Now increment the pointer */
     n_index++;
        n_value--;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SB1 scheduled but not received,waiting");
      /* This sib could be scheduled in SB1, We can't say for
      sure, so return TRUE for now. We'll wait till we get SB1 */
      return( TRUE );
    }
  } /* end if sb1_found == TRUE */

  if( sb2_found == TRUE )
  {
    /* Repeat the same search in Scheduling Block 2 */
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SB2] )
    {
      /* If the SB2 is valid */
      sb2_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB2,dbindex);
      if(NULL != sb2_ptr)
      {
        ptr = &(sb2_ptr->sib_ReferenceList);
      }
      n_index = 0;
      n_value = 0;
      if(ptr != NULL)
      {
        n_value = ptr->n;
      }
        
      while( n_value != 0 )
        {
          /* Check if this is for SIB */
        if(TDSRRCGET_SIB_TYPE_AND_TAG((&ptr->elem[n_index].sib_Type)) == sib_tag)
          {
            /* We found it */
            sib_schd_found = TRUE;
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            if(for_deferred_sib == TRUE)
            {
              *def_sib_stored_in_sb1_sb2 = tdsrrc_SB2;
            }
#endif
            break; /* Out of the while loop */
          }
          /* Now increment the pointer */
        n_index++;
        n_value--;
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SB2 scheduled but not received,waiting");
      /* This sib could be scheduled in SB2, We can't say for
      sure, so return TRUE for now. We'll wait till we get SB2 */
      return( TRUE );
    }
  } /* end if sb2_found == TRUE */


  /* Finally, check if the sib scheduling info
  was found in either of the 2 blocks */
  if( sib_schd_found == TRUE )
  {
    /* It is scheduled */
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
}/* end function tdsrrcsib_is_sib_scheduled */
   
/*===========================================================================

FUNCTION CHECK_EVENT_COMPLETION_STATUS_FOR_BPLMN

DESCRIPTION

  This function checks the status of the TD2TD BPLMN sib event, the index of
  which is passed to this function. If all SIBs for the event
  have been received, it will return TRUE. Else it will return FALSE.
    
DEPENDENCIES
 
  None.

RETURN VALUE

  TRUE is event has been completed, else FALSE.

SIDE EFFECTS

  None.
  
===========================================================================*/
static boolean tdsrrcsib_check_event_completion_status_for_bplmn
(
  uint32 index
)
{
#ifdef FEATURE_LTE_TO_TDSCDMA
     /* ZZ: workaround of VisualStudio bug */
     boolean cgi = tdsrrccsp_check_cgi_scan();
#endif

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
  if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
     (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
     ((tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
#ifdef FEATURE_LTE_TO_TDSCDMA
     && (!cgi)
#endif
     ))
  {
    if(tdsrrcsib_bplmn_sibs.mib != NULL)
    {
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
      if(tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed == TRUE &&
         tdsrrcsib_bplmn_sibs.sb1 == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BAND_TYPE_DETECTION - SB1 not read so far ");
        return FALSE;
      }
      if(tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed == TRUE &&
         tdsrrcsib_bplmn_sibs.sb2 == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BAND_TYPE_DETECTION - SB2 not read so far ");
        return FALSE;
      }
#endif

      TDSRRC_MSG1(MSG_LEGACY_HIGH," WTOW_BPLMN:BPLMN event completed.Scan type %d",tdssib_events_ptr->event[index].sib_scan_type);
      return TRUE;
    }
    return FALSE;
  }
#endif

  if((tdsrrcsib_bplmn_sibs.mib != NULL) && 
     (tdsrrcsib_bplmn_sibs.sib1 != NULL) &&
     (tdsrrcsib_bplmn_sibs.sib3 != NULL))
  {
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
    if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
       (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN) ||
       (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN))
    {
      if(tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed == TRUE &&
         tdsrrcsib_bplmn_sibs.sb1 == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BAND_TYPE_DETECTION - SB1 not read so far ");
        return FALSE;
      }
      if(tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed == TRUE &&
         tdsrrcsib_bplmn_sibs.sb2 == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"BAND_TYPE_DETECTION - SB2 not read so far ");
        return FALSE;
      }
    }
#endif

    TDSRRC_MSG1(MSG_LEGACY_HIGH," WTOW_BPLMN:BPLMN event completed.Scan type %d",tdssib_events_ptr->event[index].sib_scan_type);
    return TRUE;
  }
  return FALSE;
}
    
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
)
{
  tdsrrc_MasterInformationBlock *mib_ptr = NULL; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr = NULL;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr = NULL;         /* Local pointer to SB2       */
  boolean status = FALSE;
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List *ext_siblist_ptr = NULL;
  uint32 n_index=0;

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_MIB] )
    {
      mib_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB,dbindex);
      if((mib_ptr != NULL) && (TDSEXTENSION_SIB_SCHED_PRESENT_IN_MIB(mib_ptr)))
      {
        ext_siblist_ptr = &(mib_ptr->v690NonCriticalExtensions.
          v6b0NonCriticalExtensions.masterInformationBlock_v6b0ext.extSIBTypeInfoSchedulingInfo_List);

        n_index = 0;
      while(ext_siblist_ptr->n > n_index)
      {
        if( TDSRRCCHECK_SIB_TypeExt(
                (&(ext_siblist_ptr->elem[n_index].extensionSIB_Type)),
                 systemInfoType11bis)
        )
        {
          status = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:Sched present for sib11bis ");
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:Extension sib type = %d not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[n_index].extensionSIB_Type)));
        }
        n_index++;
      }
    }
  }
  
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SB1] )
    {
      sb1_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB1,dbindex);
      if((sb1_ptr != NULL) && (TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(sb1_ptr,
              tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1)))
      {
        ext_siblist_ptr = &(sb1_ptr->v6b0NonCriticalExtensions.
            sysInfoTypeSB1_v6b0ext.extSIBTypeInfoSchedulingInfo_List);

      n_index = 0;
        while(ext_siblist_ptr->n > n_index)
      {
        if( TDSRRCCHECK_SIB_TypeExt(
                (&(ext_siblist_ptr->elem[n_index].extensionSIB_Type)),
                 systemInfoType11bis)
        )
        {
          status = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:Sched present for sib11bis ");
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:Extension sib type = %d not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[n_index].extensionSIB_Type)));
        }
        n_index++;
      }
    }
  }
  
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SB2] )
    {
      sb2_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB2,dbindex);
      if((sb2_ptr != NULL) && TDSEXTENSION_SIB_SCHED_PRESENT_IN_SB1_SB2(sb2_ptr,
              tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2))
      {
        ext_siblist_ptr = &sb2_ptr->v6b0NonCriticalExtensions.
            sysInfoTypeSB2_v6b0ext.extSIBTypeInfoSchedulingInfo_List;
        
      n_index = 0;
        while(ext_siblist_ptr->n > n_index)
      {
        if( TDSRRCCHECK_SIB_TypeExt(
                (&(ext_siblist_ptr->elem[n_index].extensionSIB_Type)),
                 systemInfoType11bis)
        )
        {
          status = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:Sched present for sib11bis ");
        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib11bis:Extension sib type = %d not supported",TDSRRCGET_SIB_TYPE_AND_TAG((&ext_siblist_ptr->elem[n_index].extensionSIB_Type)));
        }
        n_index++;
      }
    }
  }
  }
  else
  {
    status = FALSE;
  }

  return status;
}
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

  TRUE is SIB19 is scheduled else FALSE.

SIDE EFFECTS

  None.
  
===========================================================================*/
boolean tdsrrcsib_check_if_sib19_present
(
  uint32 dbindex    /* Database index    */
)
{
  tdsrrc_MasterInformationBlock *mib_ptr = NULL; /* Local pointer to MIB       */
  tdsrrc_SysInfoTypeSB1 *sb1_ptr = NULL;         /* Local pointer to SB1       */
  tdsrrc_SysInfoTypeSB2 *sb2_ptr = NULL;         /* Local pointer to SB2       */
  boolean status = FALSE;
  tdsrrc_ExtSIBTypeInfoSchedulingInfo_List2 *ext_siblist2_ptr = NULL;
  uint32 n_index=0;

  if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL6)
  {
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_MIB] )
    {
      mib_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_MIB,dbindex);
      
      if((mib_ptr != NULL) && TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_MIB(mib_ptr)) 
      {
        ext_siblist2_ptr = &(mib_ptr->v690NonCriticalExtensions.v6b0NonCriticalExtensions.
          v860NonCriticalExtensions.masterInformationBlock_v860ext.extSIBTypeInfoSchedulingInfo_List);

        n_index = 0;
        while(ext_siblist2_ptr->n > n_index)
        {
          if(ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t ==
                T_tdsrrc_SIB_TypeExt2_systemInfoType19)
          {
            status = TRUE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:Sched present for sib19 ");
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:Extension sib type = %d not supported",ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t);
          }
          n_index++;
        }   
      }      
    }
  
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SB1] )
    {
      sb1_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB1,dbindex);
      
      if((sb1_ptr != NULL) && TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(sb1_ptr,
              tdsrrc_SysInfoTypeSB1,sysInfoTypeSB1))
      {
        ext_siblist2_ptr = &(sb1_ptr->v6b0NonCriticalExtensions.v860NonCriticalExtensions.
          sysInfoTypeSB1_v860ext.extSIBTypeInfoSchedulingInfo_List);

        n_index = 0;
        while(ext_siblist2_ptr->n > n_index)
        {
          if(ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t ==
                T_tdsrrc_SIB_TypeExt2_systemInfoType19)
          {
            status = TRUE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:Sched present for sib19 ");
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:Extension sib type = %d not supported",ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t);
          }
          n_index++;
        } 
      }      
    }
  
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SB2] )
    {
      sb2_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SB2,dbindex);

      if((sb2_ptr != NULL) && TDSEXTENSION_SIB_SCHED_2_PRESENT_IN_SB1_SB2(sb2_ptr,
              tdsrrc_SysInfoTypeSB2,sysInfoTypeSB2))
      {
        ext_siblist2_ptr = &(sb2_ptr->v6b0NonCriticalExtensions.
            v860NonCriticalExtensions.sysInfoTypeSB2_v860ext.extSIBTypeInfoSchedulingInfo_List);

        n_index = 0;
        while(ext_siblist2_ptr->n > n_index)
        {
          if(ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t ==
                T_tdsrrc_SIB_TypeExt2_systemInfoType19)
          {
            status = TRUE;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:Sched present for sib19 ");
          }
          else
          {
            TDSRRC_MSG1(MSG_LEGACY_ERROR,"sib19:Extension sib type = %d not supported",ext_siblist2_ptr->elem[n_index].extensionSIB_Type2.t);
          }
          n_index++;
        }      
      }      
    }
  }
  else
  {
    status = FALSE;
  }

  return status;
}
#endif

/*===========================================================================

FUNCTION CEHCK_EVENT_COMPLETION_STATUS

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
static boolean tdsrrcsib_check_event_completion_status
(
  uint32 index  /* Index of the event in the sib events database */
  #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  , boolean deferred_meas_read_supported
  #endif
)
{
  word count; /* Local counter */
  uint32 dbindex; /* Index to look into the database if needed */
  

#ifdef FEATURE_3GPP_CSFB_TDS
#ifdef FEATURE_3GPP_CSFB_SKIP_OPT_TDSCDMA_SIBS
  if( (tdsrrc_csfb_call_status == TRUE) &&
      (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL) &&
      (tdsrrc_csfb_skip_sib11_opt_nv == TRUE)
    )
  {
      if( (tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received == TRUE) &&
          (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_received == TRUE) &&
          (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_received == TRUE) &&
          (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_received == TRUE) &&
          (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_received == TRUE)
        )
      {
        if(tdssib_events_ptr->event[index].sib_db_index_found == TRUE)
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB Status TRUE, Invalidating SIB11,12,11bis for %d",tdssib_events_ptr->event[index].sib_db_index);
          tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sib_value_tag[tdsrrc_SIB11] = TDSINVALID_MIB_SB_VALUE_TAG;
          tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sib_value_tag[tdsrrc_SIB12] = TDSINVALID_MIB_SB_VALUE_TAG;
          tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sib_value_tag[tdsrrc_SIB11bis] = TDSINVALID_MIB_SB_VALUE_TAG;
          tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sib_value_tag[tdsrrc_SIB4] = TDSINVALID_MIB_SB_VALUE_TAG;
          tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sib_value_tag[tdsrrc_SIB6] = TDSINVALID_MIB_SB_VALUE_TAG;
          tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sib_value_tag[tdsrrc_SIB7] = TDSINVALID_MIB_SB_VALUE_TAG;
          tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sib_value_tag[tdsrrc_SIB18] = TDSINVALID_MIB_SB_VALUE_TAG;
          tdssibdb.cell[tdssib_events_ptr->event[index].sib_db_index].sib_value_tag[tdsrrc_SIB19] = TDSINVALID_MIB_SB_VALUE_TAG;
        }
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB Status TRUE,Mandatory sibs present. Concluding SIB reading");
        return TRUE;  
      }
  }
#endif
#endif
  
  /* Check if the event is active. It could have failed
  due to a non-gsm map PLMN or a timeout, in which case
  the event will be inactive. */
  if( tdssib_events_ptr->event[index].event_active == FALSE )
  {
    /* Nothing to do here */
    return(FALSE);
  }
  if( tdssib_events_ptr->event[index].sib_db_index_found == TRUE )
  {
    /* Database index is found. Get the index */
    dbindex = tdssib_events_ptr->event[index].sib_db_index;
  }
  else
  {
    /* If the database index has not been found, the
    event can't be completed. Just return FALSE */
    return(FALSE);
  }

  for( count = 0; count < TDSRRC_TOT_SIBS; count++ )
  {
    /* Check if each SIB that's needed has been received */
    if( (tdssib_events_ptr->event[index].tsib[count].sib_needed) &&
        (!(tdssib_events_ptr->event[index].tsib[count].sib_decoded)) )
    {
      /* Atleast one necessary SIB has not been decoded.
      Check if this SIB is one of the mandatory SIBs. If so,
      we will return FALSE */
      switch( count )
      {
        case tdsrrc_SIB2:
        /* Check is SIB2 is scheduled in the MIB or the Scheduling
        blocks */
        if( (tdsrrcsib_is_sib_scheduled( tdsrrc_SIB2,
                dbindex
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
              , FALSE
              ,NULL
#endif
        )) == TRUE
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB2 is scheduled in MIB or SB");
          return( FALSE );
        }
        else
        {
          tdssib_events_ptr->event[index].tsib[count].sib_needed = FALSE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB2 is not scheduled in MIB or SB");
        }
        break;

        case tdsrrc_SIB18:
        /* Check is SIB18 is scheduled in the MIB or the Scheduling
           blocks */
        if( (tdsrrcsib_is_sib_scheduled( tdsrrc_SIB18,
              dbindex 
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
            , FALSE
            , NULL
#endif
        )) == TRUE
          )
        {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if(deferred_meas_read_supported == FALSE)
#endif
          {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB18 is scheduled in MIB or SB");
          return( FALSE );
        }
        }
        else
        {
          tdssib_events_ptr->event[index].tsib[count].sib_needed = FALSE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB18 is not scheduled in MIB or SB");
        }
        break;

        case tdsrrc_SIB4:
          /* Check if SIB 4 is present in this cell
          First check if SIB3 is present. SIB3 will indicate if
          SIB4 will be transmitted or not */
          if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB3] == TRUE)
          {
            if(tdsrrcsib_check_if_connected_mode_sibs_present
              (tdssibdb.cell[dbindex].sibs.sib3_encoded_ptr[0],
              tdsrrc_SIB3) == TRUE )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 present in cell but not received");
              /* Check is SIB4 is scheduled in the MIB or the Scheduling
              blocks */
              if( (tdsrrcsib_is_sib_scheduled
                    ( tdsrrc_SIB4,
                      dbindex
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
                    , FALSE
                    , NULL
#endif
                      )) == TRUE
                )
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 is scheduled in MIB or SB");
                return( FALSE );
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB4 is not scheduled in MIB or SB");
              }
            }
          }
          else
          {
            /* SIB3 is not received yet. Make sure SIB3 is needed for this
            event. If not, we won't wait for SIB4. */
            if( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_needed )
            {
              return(FALSE);
            }
          }
          break;

        case tdsrrc_SIB6:
          /* Check if SIB 6 is present in this cell 
          First check if SIB5 is present. SIB5 will indicate if
          SIB6 will be transmitted or not */
          if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB5] == TRUE)
          {
            if(tdsrrcsib_check_if_connected_mode_sibs_present
              (tdssibdb.cell[dbindex].sibs.sib5_encoded_ptr[0],
              tdsrrc_SIB5) == TRUE )
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB6 present in cell but not received");
              /* Check is SIB6 is scheduled in the MIB or the Scheduling
              blocks */
              if( (tdsrrcsib_is_sib_scheduled
                    ( tdsrrc_SIB6,
                      dbindex
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
                    , FALSE
                    , NULL
#endif
                      )) == TRUE
                )
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB6 is scheduled in MIB or SB");
                return( FALSE );
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB6 is not scheduled in MIB or SB");
              }
            }
          }
          else
          {
            /* SIB5 is not received yet. Make sure SIB5 is needed for this
            event. If not, we won't wait for SIB6. */
            if( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_needed )
            {
              return(FALSE);
            }
          }
                    
          break;

        case tdsrrc_SIB12:
          /* Check if SIB 12 is present in this cell
          First check if SIB11 is present. SIB11 will indicate if
          SIB12 will be transmitted or not */
          
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if(deferred_meas_read_supported == FALSE)
#endif
          {
            if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB11] == TRUE)
            {
              if(tdsrrcsib_check_if_connected_mode_sibs_present
                (tdssibdb.cell[dbindex].sibs.sib11_encoded_ptr[0],
                tdsrrc_SIB11) == TRUE )
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 12 present in cell but not received");
                /* Check is SIB 12 is scheduled in the MIB or the Scheduling
                blocks */
                if( (tdsrrcsib_is_sib_scheduled
                      ( tdsrrc_SIB12,
                          dbindex
  #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
                       , FALSE
                       , NULL
  #endif
                          )) == TRUE
                  )
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 12 is scheduled in MIB or SB");
                  return( FALSE );
                }
                else
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 12 is not scheduled in MIB or SB");
                }
              }
            }
            else
            {
              /* SIB11 is not received yet. Make sure SIB11 is needed for this
              event. If not, we won't wait for SIB12. */
              if( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_needed )
              {
                return(FALSE);
              }
            }
          }
          break;

        case tdsrrc_SB1:
          if(tdsrrcsibdb_check_if_sb_present(tdsrrc_SB1, dbindex))
          {
            /* Scheduling block present but not received yet */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SB1 present in cell but not received");
            return(FALSE);
          }
          else
          {
            //invalidate SB1 presence in this cell as it is not scheduled in
            //MIB 
            tdssib_events_ptr->event[index].tsib[count].sib_needed = FALSE;
          }
          break;

        case tdsrrc_SB2:
          if(tdsrrcsibdb_check_if_sb_present(tdsrrc_SB2, dbindex))
          {
            /* Scheduling block present but not received yet */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"SB2 present in cell but not received");
            return(FALSE);
          }
          else
          {
            //invalidate sb2's presence in this cell as it is not scheduled in
            //MIB
            tdssib_events_ptr->event[index].tsib[count].sib_needed = FALSE;
          }
          break;
  
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
        case tdsrrc_SIB11bis:
          if( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_needed )
          {
            if(tdsrrcsib_check_if_extension_sib_present(dbindex) == TRUE)
            {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
              if(deferred_meas_read_supported == FALSE)
#endif
              {
                /* SIB11bis present but not received yet */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:Waiting to receive SIB11bis");
                return(FALSE);
              }
            }
            else
            {
              /* SIB11bis not present in this cell */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib11bis:SIB11bis not present in this cell");
              tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_needed  = FALSE;
            }
          }
          break;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
        case tdsrrc_SIB19:
          if( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_needed )
          {
            if(tdsrrcsib_check_if_sib19_present(dbindex) == TRUE)
            {
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          /* If DMCR is supported and L2T redirection is in progress though SIB19 is present
             return TRUE so that SIB event will be completed */ 
              if((deferred_meas_read_supported == TRUE) && 
                 (RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN == tdsrrc_csp_int_data.curr_scan) &&
                 (TDSRRC_CSP_INTERRAT_LTE == tdsrrc_csp_int_data.interrat_type))

              {
                /* Do nothing */
              }
              else
#endif
              if(tdsrrcsib_sib19_decode_fail_count >= TDSRRCSIB_SIB19_DECODE_FAIL_COUNT_VALUE)
              {
                /* in real network, SIB19 decode failure happens on some cell,
                   we also could try to camp on. */
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib19:SIB19 decode failure happens %d times", tdsrrcsib_sib19_decode_fail_count);
              }
              else
              {
              /* SIB19 present but not received yet */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:Waiting to receive SIB19");
              return(FALSE);
            }
            }
            else
            {
              /* SIB19 not present in this cell */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib19:SIB19 not present in this cell");
              tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_needed  = FALSE;
            }
          }
          break;
#endif

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
        case tdsrrc_SIB11:
          if(deferred_meas_read_supported == TRUE)
          {
            break;
          }
          else
          {
            return(FALSE);
          }
#endif

        default:
          /* All other SIBs that we need are mandatory */
          return(FALSE);
      }
    }
  }/* end for */

#ifdef FEATURE_TDSCDMA_SIB19
  tdsrrcsib_check_and_remove_sib19_if_not_scheduled(index);
#endif

  /* IF we get here, All required sibs have been received */
  return(TRUE);
}/* end function tdsrrcsib_check_event_completion_status */

#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
/*===========================================================================

FUNCTION tdsrrcsib_is_sib5bis_scheduled

DESCRIPTION

  This function checks whether SIB5bis is present in MIB/SB in the BPLMN DB
  
DEPENDENCIES
 
  None.

RETURN VALUE

  TRUE : SIB5BIS is present
  FALSE : Otherwise

SIDE EFFECTS

  None.
  
===========================================================================*/
static boolean tdsrrcsib_is_sib5bis_scheduled
( 
  tdssib_events_index_e_type index
)
{
  tdsrrc_SIBSb_ReferenceList * sibSb_ptr=NULL;
  tdsrrc_SIB_ReferenceList * sb_ptr = NULL;
  uint32 ix=0;
  if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
     (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN)||
     (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN))
  {
    if(tdsrrcsib_bplmn_sibs.mib != NULL)
    {
      sibSb_ptr = &tdsrrcsib_bplmn_sibs.mib->sibSb_ReferenceList;
	  for(ix=0;ix< sibSb_ptr->n;ix++)
      {
        if(TDSRRC_CHECK_COMMON_MSG_TYPE(sibSb_ptr->elem[ix].sibSb_Type,
			rrc_SIB_TypeAndTag_sysInfoType5bis))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-SIB5bis present in MIB. set is_sib5bis_scheduled to TRUE");
          return TRUE;
        }
        else if(TDSRRC_CHECK_COMMON_MSG_TYPE(sibSb_ptr->elem[ix].sibSb_Type,
			rrc_SIB_TypeAndTag_sysInfoType5))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-SIB5 present in MIB. set is_sib5bis_scheduled to FALSE");
          return FALSE;
        }
   
      }
    }
    if(tdsrrcsib_bplmn_sibs.sb1!=NULL)
    {
      sb_ptr = &(tdsrrcsib_bplmn_sibs.sb1->sib_ReferenceList);
 
	  for(ix=0;ix< sb_ptr->n;ix++)
      {
        if(TDSRRC_CHECK_COMMON_MSG_TYPE(sb_ptr->elem[ix].sib_Type,
			rrc_SIB_TypeAndTag_sysInfoType5bis))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-SIB5bis present in SB1. set is_sib5bis_scheduled to TRUE");
          return TRUE;
        }
        else if(TDSRRC_CHECK_COMMON_MSG_TYPE(sb_ptr->elem[ix].sib_Type,
			rrc_SIB_TypeAndTag_sysInfoType5))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-SIB5 present in SB1. set is_sib5bis_scheduled to FALSE");
          return FALSE;
        }

      }
    }

    if(tdsrrcsib_bplmn_sibs.sb2 !=NULL)
    {
      sb_ptr = &tdsrrcsib_bplmn_sibs.sb2->sib_ReferenceList;
	  ix=0;
      for(ix=0;ix< sb_ptr->n;ix++)
      {
        if(TDSRRC_CHECK_COMMON_MSG_TYPE(sb_ptr->elem[ix].sib_Type,
			rrc_SIB_TypeAndTag_sysInfoType5bis))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-SIB5bis present in SB2 set is_sib5bis_scheduled to TRUE");
          return TRUE;
        }
        else if(TDSRRC_CHECK_COMMON_MSG_TYPE(sb_ptr->elem[ix].sib_Type,
			rrc_SIB_TypeAndTag_sysInfoType5))
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-SIB5 present in SB2 set is_sib5bis_scheduled to FALSE");
          return FALSE;
        }
      }
    }
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"ERR:Neither SIB5/SIB5bis is present");
  return FALSE;
}
#endif
/*===========================================================================

FUNCTION POST_PROCESS_COMPLETED_EVENT

DESCRIPTION

  This function performs all the necessary post-processing for events
  that have been completed (i.e. all necessary sibs have been received).
  Post-processing includes -
  
    1) Sending a confirmation event if necessary to the module that
       requested the event.
       
    2) Tearing down the BCCH radio-bearer if it's not needed anymore.
    
    3) Cleanup of internal variables related to the event.
    
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
  
===========================================================================*/
void tdsrrcsib_post_process_completed_event
(
  tdssib_events_index_e_type index /* Index of the event in the sib event database */
)
{
  tdsrrc_cmd_type *cnf_ptr; /* For sending confirmation */
  uint32 dbindex = 0;        /* Index to the SIB database*/
  rrc_plmn_identity_type plmn;

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  /* Initialize PLMN to dummy value to keep lint happy */
  plmn.mcc[0] = 0;
  plmn.mcc[1] = 0;
  plmn.mcc[2] = 0;
  plmn.num_mnc_digits = 3;
  plmn.mnc[0] = 0;
  plmn.mnc[1] = 0;
  plmn.mnc[2] = 0;

  //GET_ALL_SIB's is being by-passed as all SIB's will be requested in GET_SPECIFIC_SIB's itself.
  //since CSP/CCM's state machine now still has this 2 step procedure, and also if we need to
  //revisit GET_ALL_SIB's this cmd is not removed altogether for now.  Will remove it later.
  if(tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_ALL_SIBS)
  {
    if( (cnf_ptr = tdsrrc_get_int_cmd_buf())!=NULL )
    {
      cnf_ptr->cmd_hdr.cmd_id = TDSRRC_GET_ALL_SIBS_CNF;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending GET_ALL_SIBS_CNF cmd");

      /* Indicate that the event succeeded. This is the default
      for now. If we have to send confirmations with a failure status, this
      needs to be changed. also copy proc id to requesting procedure*/
      cnf_ptr->cmd.get_sibs_cnf.status = TDSSIB_EVENT_SUCCEEDED;
      cnf_ptr->cmd.get_sibs_cnf.procedure = tdssib_events_ptr->event[index].req_proc;

      /* Now send the command */
      tdsrrc_put_int_cmd( cnf_ptr );
    }
    else
    {
      ERR_FATAL("No memory to send cnf event",0,0,0);
    }

    /* Make sure the srv_bcch information is reset */
    tdssib_events_ptr->srv_bcch.bcch_action =  TDSSIB_BCCH_NO_ACTION;
    return;
  }

  /* Stop the SIB wait timer */
  tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );

  tdsrrctmr_stop_timer( TDSRRCTMR_SIB_STOP_PARTITION_TIMER );
  tdsrrctmr_stop_timer( TDSRRCTMR_SIB_MASK_INVERT_TIMER );
  tdsrrctmr_stop_timer( TDSRRCTMR_SIB_EMPTY_MASK_TIMER );
  tdsrrctmr_stop_timer( TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER );

  /*Added support for SIB scan type = TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN.
   *Currently this scan type is used to read MIB & SIB3 only, so If we have received 
   *MIB/SIB3 then end this event here itself by sending confirmation to RRCGPS module
   */

  if((tdssib_events_ptr->event[index].sib_scan_type != RRC_SIB_TD2TD_AUTO_BPLMN_SCAN) &&
     (tdssib_events_ptr->event[index].sib_scan_type != RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN) &&
     (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_MANUAL_SCAN) &&
     (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_MANUAL_BPLMN_SCAN)
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
     &&(tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
#endif
     && (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_AUTO_BPLMN_SCAN)
     && (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_DUMMY_PLMN_SCAN))
  {
    /* Note the SIB database index for this event. */
    dbindex = tdssib_events_ptr->event[index].sib_db_index;
  
    /* Once a event is completed succesfully, we have to store all the
    value tags in the SIB database so that we can compare them later
    if needed. */
    tdsrrcsibdb_store_value_tags_for_new_cell(dbindex
    #ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
                                           ,index
    #endif
                                            );

  }


  /* Check if a S-BCCH teardown is required. S-BCCH is torn down
  for a TDSRRCSIB_UPDATE_SIBS command and for any command initiated
  by the SIB procedure itself */
  if( (tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS) ||
      (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_SIB)
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
      ||(tdsrrcsib_is_current_event_sib_read_in_dch() == TRUE)
#endif
    )
  {
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
    if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
    {
      /*With feature FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH defined, 
        BCCH will be ON in FACH, So no need to tear down S-BCCH*/
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC State is FACH,So skip Tearing down S-BCCH");
    }
    else
#endif
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
    if(((tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH)||(tdsrrc_get_state() == TDSRRC_STATE_URA_PCH))&&
        (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)&&
        (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].req_proc == TDSRRC_PROCEDURE_SIB)
     )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB7 Update in PCH state,Tearing down S-BCCH");
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
      tdsrrcllc_chan_cfg_reason_e_type chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_NONE;
      if(tdsrrcsib_is_current_event_sib_read_in_dch() == TRUE)
      {
        chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_SIB_READ_IN_DCH;
      }
#endif
      if(tdsrrcsib_configure_bcch(TDSSBCCH_TEARDOWN, TDSNBCCH_NOOP
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
                        ,chan_config_reason
#endif
                   ) == FAILURE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib configure bcch failed");
      }
    }
    else
#endif
    {
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
      tdsrrcllc_chan_cfg_reason_e_type chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_NONE;
      if(tdsrrcsib_is_current_event_sib_read_in_dch() == TRUE)
      {
        chan_config_reason = TDSRRCLLC_CHAN_CFG_REASON_SIB_READ_IN_DCH;
      }
#endif
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Tearing down S-BCCH");
      if(tdsrrcsib_configure_bcch(TDSSBCCH_TEARDOWN, TDSNBCCH_NOOP
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
                  ,chan_config_reason
#endif
             ) == FAILURE)
      {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib configure bcch failed");
      }
    }
  }

  /* For UPDATE_SIBS event no confirmation is required */
  if( tdssib_events_ptr->event[index].event_name == TDSRRCSIB_UPDATE_SIBS )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"UPDATE_SIBS Event Completed");
    /* Here we have to notify other RRC procedures about
    any SIBs that may have changed for the Active Cell */
    tdsrrcsibdb_notify_sib_change_to_procs();

    /* Once that's done, clear the saved SIB change
    indications */
    tdsrrcsibdb_clear_active_cell_sib_change();
    /*Reset six hrs sib count for serving cell sib db*/
    if((tdsrrcsib_is_event_sixhrs_sib_update(index) == TRUE)||
        (tdsrrcsib_is_sixhrs_sib_timer_expired(index) == TRUE)
      )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"6hr sib:Reset six hrs sib count for serving cell sib db,currnt value=%d",tdssibdb.cell[tdssibdb.active_cell_index].six_hr_count);
      tdssibdb.cell[tdssibdb.active_cell_index].six_hr_count = 0;
    }

    tdssib_change_safe_guard = TRUE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Starting tdssib_change_safe_guard Timer");
    /*Start 1sec timer for safe guarding processing of sib_change_ind against BPLMN*/
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    if(tdssib_events_ptr->update_sibs_cause == TDSRRCSIB_DEFERRED_SIB_READ)
    {
      tdsrrctmr_start_timer( TDSRRC_SIB_CHANGE_GUARD_TIMER,TDSRRCSIB_SIB_CHANGE_GUARD_TMR_FOR_DEFERRED_SIB );
    }
    else
#endif
    {
      tdsrrctmr_start_timer( TDSRRC_SIB_CHANGE_GUARD_TIMER,TDSRRCSIB_SIB_CHANGE_GUARD_TIMER_VALUE );
    }
  }
  else
  {
    /* Check if the request came from another procedure or
    the SIB procedure itself. If it's from another procedure 
    and if it's not an UPDATE_SIBS command, we send a confirm command */
    if( tdssib_events_ptr->event[index].req_proc != TDSRRC_PROCEDURE_SIB )
    {
      if( (cnf_ptr = tdsrrc_get_int_cmd_buf())!=NULL )
      {
        if( tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS )
        {
          cnf_ptr->cmd_hdr.cmd_id = TDSRRC_GET_SPECIFIC_SIBS_CNF;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending GET_SPECIFIC_SIBS_CNF cmd");
    
          /* Copy the frequency and Scrambling code into the
          confirmation command. For GET_SPECIFIC_SIBS command, 
          check if it's the serving cell or neigbor cell. */
          if(tdssib_events_ptr->event[index].for_serving_cell == TRUE)
          {
            cnf_ptr->cmd.get_sibs_cnf.freq = tdssib_events_ptr->srv_bcch.freq;
            cnf_ptr->cmd.get_sibs_cnf.cpid = tdssib_events_ptr->srv_bcch.cpid;
            /* Also Copy the frequency and Scrambling code into the
              SIB Database */
            if((tdssib_events_ptr->event[index].sib_scan_type != RRC_SIB_TD2TD_AUTO_BPLMN_SCAN)&&
               (tdssib_events_ptr->event[index].sib_scan_type != RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)&&
               (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_MANUAL_SCAN)&&
               (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_MANUAL_BPLMN_SCAN)
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
               &&(tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
#endif
              &&   (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_AUTO_BPLMN_SCAN)
              && (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_DUMMY_PLMN_SCAN))
            {
              /* Also Copy the frequency and Scrambling code into the
              SIB Database */
              tdssibdb.cell[dbindex].freq = tdssib_events_ptr->srv_bcch.freq;
              tdssibdb.cell[dbindex].cpid = tdssib_events_ptr->srv_bcch.cpid;
              /* No need to reset srv_bcch status here. We expect to get a
              "GET_ALL_SIBS" command for the same, so we should not setup bcch
              again on the same cell. */
            }
            /* No need to reset srv_bcch status here. We expect to get a
            "GET_ALL_SIBS" command for the same, so we should not setup bcch
            again on the same cell. */
          }
          else
          {
            cnf_ptr->cmd.get_sibs_cnf.freq = tdssib_events_ptr->ngh_bcch.freq;
            cnf_ptr->cmd.get_sibs_cnf.cpid = tdssib_events_ptr->ngh_bcch.cpid;
            /* Also Copy the frequency and Scrambling code into the
            SIB Database */
            if((tdssib_events_ptr->event[index].sib_scan_type != RRC_SIB_TD2TD_AUTO_BPLMN_SCAN)&&
               (tdssib_events_ptr->event[index].sib_scan_type != RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)&&
               (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_MANUAL_SCAN)&&
               (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_MANUAL_BPLMN_SCAN)&&
               (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_AUTO_BPLMN_SCAN)&&
               (tdssib_events_ptr->event[index].sib_scan_type != TDSRRC_SIB_DUMMY_PLMN_SCAN))
            {
              /* Also Copy the frequency and Scrambling code into the
              SIB Database */
              tdssibdb.cell[dbindex].freq = tdssib_events_ptr->ngh_bcch.freq;
              tdssibdb.cell[dbindex].cpid = tdssib_events_ptr->ngh_bcch.cpid;
              /* Make sure the ngh_bcch information is reset */
              tdssib_events_ptr->ngh_bcch.bcch_action =  TDSSIB_BCCH_NO_ACTION;
            }
          }    
        }
        if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN) ||
           (tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN)||
           (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN))
        {
#ifdef FEATURE_LTE_TO_TDSCDMA
          /* ZZ: workaround of VisualStudio bug */
          boolean cgi = tdsrrccsp_check_cgi_scan();
#endif
          /* Copy the PLMN id */
          if( TDSRRCGET_PLMN_TYPE((&tdsrrcsib_bplmn_sibs.mib->plmn_Type))
                == TDSRRCGET_PLMN_TYPE_TAG(gsm_MAP))
          {
            tdsrrc_translate_gsm_map_plmn_id
              (&(tdsrrcsib_bplmn_sibs.mib->plmn_Type.u.gsm_MAP->plmn_Identity), &plmn);
          }
          cnf_ptr->cmd.get_sibs_cnf.plmn_id = plmn;
          
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
          if(tdsrrcsib_is_sib5bis_scheduled(index) == TRUE)
          {
            cnf_ptr->cmd.get_sibs_cnf.is_sib5bis_scheduled = TRUE;
          }
          else 
          {
            cnf_ptr->cmd.get_sibs_cnf.is_sib5bis_scheduled = FALSE;
          }
#endif

#ifdef FEATURE_SKIP_SIB1_3_DURING_MANUAL_PLMN_SCAN
          /* We dont have SIB3 for manual scans and hence we dont have the cell id.So dont
           try accessing the cell id field in BPLMN SIB3.Assign it to zero.CSP should 
           not look into this field */
          if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
             (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN)||
             ((tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN)
#ifdef FEATURE_LTE_TO_TDSCDMA
             && (!cgi)
#endif
             ))
          {
            cnf_ptr->cmd.get_sibs_cnf.cell_id = 0;
          }
          else
#endif
          {
            /* Copy all Cell Id */
            cnf_ptr->cmd.get_sibs_cnf.cell_id = tdsrrc_translate_cell_id(&tdsrrcsib_bplmn_sibs.sib3->cellIdentity);
          }

          /* We dont ahve a db index.Set it to 0.CSP should NOT look into this field */ 
          cnf_ptr->cmd.get_sibs_cnf.sib_index_for_cell = 0;
        }
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
        else if(tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Cell id reading in DCH successful");
          /* Get the plmn id for this cell. Make sure the
          PLMN id is a GSM-MAP PLMN. Convert it to the internal format used by RRC. */
          if(tdssib_events_ptr->event[index].tempholder.mib != NULL)
          {
            if( tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.t
                == T_tdsrrc_PLMN_Type_gsm_MAP )
            {
              tdsrrc_translate_gsm_map_plmn_id 
                (&(tdssib_events_ptr->event[index].tempholder.mib->plmn_Type.u.gsm_MAP->plmn_Identity),
                 &plmn
                );
            }
          }
          cnf_ptr->cmd.get_sibs_cnf.plmn_id = plmn;
          if(tdssib_events_ptr->event[index].tempholder.sib3 != NULL)
          {
             cnf_ptr->cmd.get_sibs_cnf.cell_id  = tdsrrc_translate_cell_id
                      ( &(tdssib_events_ptr->event[index].tempholder.sib3->cellIdentity));
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"RRCGPS:Rexd Cell I.D CellId %d and MCC %d MNC %d", 
                       cnf_ptr->cmd.get_sibs_cnf.cell_id,
                       TDSRRC_CSP_GET_INT_MCC(plmn), 
                       TDSRRC_CSP_GET_INT_MNC(plmn));
          }
          else
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Error temp sib3 ptr = NULL");
            cnf_ptr->cmd.get_sibs_cnf.cell_id = 0;
          }
        }
#endif
        else
        {
          /* Copy all the necessary parameters for the confirmation command */
          cnf_ptr->cmd.get_sibs_cnf.cell_id = tdssibdb.cell[dbindex].cell_id;
          cnf_ptr->cmd.get_sibs_cnf.plmn_id = tdssibdb.cell[dbindex].plmn_id;
          /* SIB DB Index is also sent in the confirmation so that the procedure
          can use the index to read the SIBs if needed. Alternately the PLMN and
          Cell Id can also be used to read SIBs. */
          cnf_ptr->cmd.get_sibs_cnf.sib_index_for_cell = dbindex; 
        }
        
        /*Boolean to indicate whether SIB read over OTA or through the 
               SYS_INFO_CONTAINER*/
#ifdef FEATURE_3GPP_CSFB_TDS
        if(tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_READ_FROM_SIB_CONTAINER)
        {
          cnf_ptr->cmd.get_sibs_cnf.is_sib_read_from_container = TRUE;
        }
        else
        {
          cnf_ptr->cmd.get_sibs_cnf.is_sib_read_from_container = FALSE;
        }
#endif        
        /* Procedure name */
        cnf_ptr->cmd.get_sibs_cnf.procedure = tdssib_events_ptr->event[index].req_proc;
 
  
        /* Indicate that the event succeeded. This is the default
        for now. If we have to send confirmations with a failure status, this
        needs to be changed. */
        cnf_ptr->cmd.get_sibs_cnf.status = TDSSIB_EVENT_SUCCEEDED;
  
        /* delete pending SIB_SCHED_CHANGE_IND in int queue since we are sending SIB_CNF, to avoid race conditions */
        tdsrrcsib_delete_sib_schedule_update_from_int_cmd_queue();
        /* Now send the command */
        tdsrrc_put_int_cmd( cnf_ptr );
      }
      else
      {
        ERR_FATAL("No memory to send cnf event",0,0,0);
      }
    }
    else
    {
      /* This command was requested by the SIB procedure itself.
      Process it accordingly */
      tdsrrcsib_process_int_sib_event_complete( index, SUCCESS );
    }
  }/* end if event is not UPDATE_SIBS event */

#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
  tdsno_mib_bar_cell_count = 0;
#endif

  /* Now do the necessary cleanup for this event */
  tdssib_events_ptr->event[index].event_active = FALSE;
  tdssib_events_ptr->event[index].sib_scan_type = TDSRRC_SIB_NORMAL;

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  REX_ISR_LOCK(&tdssib7_in_fach_active_mutex);

  if(tdsis_sib7_in_fach_active == TRUE)
  {
    tdsis_sib7_in_fach_active = FALSE;
  }
  REX_ISR_UNLOCK(&tdssib7_in_fach_active_mutex);
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB7 in cell fach event active is set to %d",tdsis_sib7_in_fach_active);
#endif
#endif

  /* For safety, in case the tempholer pointers are not freed, free them
  now */
  tdsrrcsib_free_tsib(index);

}/* end function tdsrrcsib_post_process_completed_event */

/*===========================================================================

FUNCTION RRCSIB_PROCESS_STATE_CHANGE_IND

DESCRIPTION

  This function does the necessary processing when RRC state
  changes to disconnected state in idle mode. This includes
  reseting the BCCH parameters and cancelling any sib events
  that have not been completed.

  The reason for cancelling sib events is that as soon as
  we move back to disconnected state, the cell selection
  procedure will start the cell selection process by sending
  fresh GET_SPECIFIC_SIBS and GET_ALL_SIBS cmds.
    
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  BCCH logical channels may be set up and teared down. 
===========================================================================*/
static void tdsrrcsib_process_state_change_ind
( 
  tdsrrc_cmd_type *ptr /* Received command */
)
{
  tdssib_events_index_e_type count;              /* Counter */
#ifdef FEATURE_RRC_SIB7_UPDATE_IN_CONNECTING
  unsigned long timer_value; /* Used if SIB7 timer has to be started */
#endif /* FEATURE_RRC_SIB7_UPDATE_IN_CONNECTING */

  /* Check what state change occured */
  if( ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_DISCONNECTED ) 
  {
    if(tdssib_events_ptr == NULL)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdssib_events_ptr freed.");
      /* Invalidating the serving cell index if TDSCDMA is stopped already
            Serving cell index is not invalidated for TTOL PSHO scenario because T is stopped before state change is received */
      tdsrrcsibdb_clear_active_cell_status();
      return;
    }

    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting SIB cmd store flag and update sibs pending flags to FALSE");
    tdsrrcsib_cmd_store.pending_flag = FALSE;
    tdsupdate_sibs_event_pending = FALSE;
  
    /* State changed to disconnected. We'll re-acquire the cell
    and get all the SIBs. Cancel any events that are in 
    progress */
    for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
    {
      if( tdssib_events_ptr->event[count].event_active == TRUE )
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Going to Idle: SIB event %d failed",tdssib_events_ptr->event[count].event_name);
        /* Post-process the failed event */
        tdsrrcsib_post_process_failed_event
          ( count, /* Event Index                 */
            TRUE,  /* Confirm Command is required */
            TDSSIB_EVENT_FAILED_INCOMPATIBLE_STATE, /* Failure cause */
            FALSE  /* Don't tear down S_BCCH */
          );
      }
    }

    //clear the tdssibdb active-status..  We won't have an active serving cell 
    //until we do cell selection again
    tdsrrcsibdb_clear_active_cell_status();
  }

  if( ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_DCH ) 
  {
    /* State changed to CELL_DCH. LLC would have brought down
    the BCCH-BCH pipe. Cancel any events that are in 
    progress */

    tdsupdate_sibs_event_pending = FALSE;

    for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
    {
      if( tdssib_events_ptr->event[count].event_active == TRUE )
      {
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"Going to DCH: SIB event %d failed",tdssib_events_ptr->event[count].event_name);
        /* Post-process the failed event. No need to
        tear down S-BCCH since it's already torn down
        by LLC while going to DCH. */
        tdsrrcsib_post_process_failed_event
          ( count, /* Event Index                 */
            TRUE,  /* Confirm Command is required */
            TDSSIB_EVENT_FAILED_INCOMPATIBLE_STATE, /* Failure cause */
            FALSE  /* Tear down S_BCCH if it's up */
          );
      }
    }
    tdsrrcpg1_check_and_abort_bcch_mod_info_processing();
  }

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  if( ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_FACH )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: UPDATE_SIBS activated in FACH");
    tdsrrcsib_update_sib_for_deferred_sib();
  }
#endif

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  if((( ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_PCH ) ||
      (ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_URA_PCH ))&&
      (ptr->cmd.state_change_ind.previous_state == TDSRRC_STATE_CELL_DCH))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: UPDATE_SIBS activated in PCH");
    tdsrrcsib_update_sib_for_deferred_sib();
  }
#endif

  if( ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CONNECTING )
  {
    /* We've moved to CONNECTING. Start timer based update of SIB7 */ 
#ifdef FEATURE_RRC_SIB7_UPDATE_IN_CONNECTING
    timer_value = tdsrrcsib_get_sib7_exp_timer_value();
    tdsrrctmr_start_timer( TDSRRCTMR_SIB7_EXP_TIMER, 
                         timer_value ); 
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Moved into CONNECTING,start SIB7 timer %d",timer_value);
#endif /* FEATURE_RRC_SIB7_UPDATE_IN_CONNECTING */
  }

#ifdef FEATURE_RRC_SIB7_UPDATE_IN_CONNECTING
  if( ptr->cmd.state_change_ind.previous_state == TDSRRC_STATE_CONNECTING )
  {
    /*We've moved out of CONNECTING. Stop timer based update of SIB7
    except if we're going to FACH. */ 
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB7_EXP_TIMER ); 
    tdssib7info.sib_timer_update_req = FALSE; 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Moved out of CONNECTING,stop SIB7 timer"); 
  }
#endif /* FEATURE_RRC_SIB7_UPDATE_IN_CONNECTING */
  
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
  if(( ptr->cmd.state_change_ind.previous_state == TDSRRC_STATE_CELL_FACH)&&
      (( ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_CELL_PCH)||
       ( ptr->cmd.state_change_ind.new_state == TDSRRC_STATE_URA_PCH)
      )
    )
  {
    for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
    {
      if( tdssib_events_ptr->event[count].event_active == TRUE )
      {
        /*For UE going out of FACH, always clean up SIB7 event.
          Since SIB7 reading is only appicable in cell FACH*/
        if(tdssib_events_ptr->event[count].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)
        {
          if((tdssib_events_ptr->event[count].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)&&
             (tdssib_events_ptr->event[count].req_proc == TDSRRC_PROCEDURE_SIB)&&
             (tdssib_events_ptr->event[count].for_serving_cell == TRUE)
            )
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib7:Going out of FACH SIB event %d failed",tdssib_events_ptr->event[count].event_name);
             /* Post-process the failed event. No need to
                 tear down S-BCCH since it's already torn down
                 by LLC while going to PCH. */
            tdsrrcsib_post_process_failed_event
              ( (tdssib_events_index_e_type)count, /* Event Index                 */
                FALSE,  /* Confirm Command is required */
                TDSSIB_EVENT_FAILED_INCOMPATIBLE_STATE, /* Failure cause */
                FALSE  /* Don't tear down S_BCCH */
               );
            break;
          }
        }
      }/* End if active events */
    }
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: UPDATE_SIBS activated in PCH");
    tdsrrcsib_update_sib_for_deferred_sib();
#endif
  }
#endif
  /* Add other state changes here */
}

/*===========================================================================

FUNCTION RRCSIB_PROCESS_SIB_WAIT_TIMER_EXPIRATION

DESCRIPTION

  This function does the necessary processing when 
  the SIB wait timer expires. The SIB wait timer 
  is the maximum time to wait for SIBs in a cell.
  
  Note that only one SIB event is supported at a time.
  Hence only one timer is needed. If many SIB events are
  supported simultaneously, many timers would be
  needed.  

DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  BCCH logical channels may be teared down. 
===========================================================================*/
static void tdsrrcsib_process_sib_wait_timer_expiration( void )
{
  tdssib_events_index_e_type count;          /* Local counter */

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB Wait timer expired!");

#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
  if( (tdsrrcsib_is_current_event_sib_read_in_dch() == TRUE) &&
      (TDSRRC_PROCEDURE_GPS == tdsrrcsib_get_proc_id_for_sib_read_in_dch()))
  {
    /*Just for printing if the event in sib read in dch*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS:SIB Wait timer expired for cell i.d DCH");
  }
#endif

  /* Check if any event is active */
  for( count=TDSSIB_SRV_INDEX; count<TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
    
      if((tdssib_events_ptr->event[count].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN) || 
         (tdssib_events_ptr->event[count].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN)||
         (tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN))
      { 
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Do not tear down S-BCCH during BPLMN and MIB/SIB timer expired");
        tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,TRUE,TDSSIB_EVENT_FAILED_TIMEOUT, FALSE);
        break;
      }
      else
      {
        /* An event was active. Since the timer expired,
        this event failed. */
        tdsrrcsib_post_process_failed_event
          ( count, /* Event Index                 */
            TRUE,  /* Confirm Command is required */
            TDSSIB_EVENT_FAILED_TIMEOUT, /* Timed out*/
            TRUE   /* Tear down S_BCCH if it's up */
            );
      }
      break; /* Out of the for loop since only one event is active
                   at a time. */
    }/* End if active events */
  }/* End for */
}/* End function tdsrrcsib_process_sib_wait_timer_expiration */


/*===========================================================================

FUNCTION RRCSIB_PROCESS_SIB7_EXP_TIMER_EXPIRATION

DESCRIPTION

  This function processes the indication that the
  SIB7 expiration timer has expired. It creates a
  command to read SIB7 again. 

DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  S-BCCH logical channels will be setup.
===========================================================================*/
static void tdsrrcsib_process_sib7_exp_timer_expiration( void )
{
  tdsrrc_cmd_type *cmd; /* To send a SIB command if needed */
  uint32 count;      /* Local Counter */

  /* Check if we're in CELL_FACH or CONNECTING. In other states we 
  won't update SIB7 based on a timer. */
  if( (tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH) ||
      (tdsrrc_get_state() == TDSRRC_STATE_CONNECTING) )
  {
    /* Generate a command for the SIB procedure itself to
    get SIB7. Make sure there's not already a pending
    command  */
    if( tdssib7info.sib_ext_proc_update_req == FALSE )
    {
      if((cmd = tdsrrc_get_int_cmd_buf())!=NULL)
      {
        cmd->cmd_hdr.cmd_id = TDSRRC_GET_SPECIFIC_SIBS_REQ;
        cmd->cmd.get_specific_sibs_req.for_serv_cell = TRUE;
        for( count=0; count<TDSRRC_TOT_SIBS; count++ )
        {
          cmd->cmd.get_specific_sibs_req.sib_req[count] = FALSE;
        }
        /* Only SIB7 is requested. */
        cmd->cmd.get_specific_sibs_req.sib_req[tdsrrc_SIB7]
          = TRUE;
        cmd->cmd.get_specific_sibs_req.procedure = TDSRRC_PROCEDURE_SIB;
        /* Get the frequency and the scrambling code from the
        SIB database itself */
        cmd->cmd.get_specific_sibs_req.freq = 
          tdssibdb.cell[tdssibdb.active_cell_index].freq;
        cmd->cmd.get_specific_sibs_req.cpid =
          tdssibdb.cell[tdssibdb.active_cell_index].cpid;
        cmd->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_NORMAL;

        TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib7:posting TDSRRC_GET_SPECIFIC_SIBS_REQ to sib");

        tdsrrc_put_int_cmd( cmd );

        /* Set an internal variable to indicate that SIB7 update
        is based on a timer expiration. */
        tdssib7info.sib_timer_update_req = TRUE;
      }
      else
      {
        ERR_FATAL("No memory to send int event",0,0,0);
      }
    }
    else
    {
      /* Can't support timer based request when an external
      request is pending. */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB7 request error");
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring SIB7 tmr exp");
  }
}/* End function tdsrrcsib_process_sib7_exp_timer_expiration */

/*===========================================================================

FUNCTION RRCSIB_PROCESS_BCCH_SETUP_FAILURE

DESCRIPTION

  This function does the necessary processing when 
  the BCCH setup fails.
  
  Note that only one SIB event is supported at a time.
  If many SIB events are supported simultaneously,
  we have to check which event had the BCCH setup
  failure. 

DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
   
===========================================================================*/
static void tdsrrcsib_process_bcch_setup_failure( void )
{
  tdssib_events_index_e_type count;

  /* Check if any event is active. Only one should
  be active - send a failure status for that event. */
  for( count=TDSSIB_SRV_INDEX; count<TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
      /* An event was active. Since the BCCH setup failed,
      send a failure status. */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"BCCH setup failed for event %d",count);
      tdsrrcsib_post_process_failed_event
        ( count, /* Event Index                 */
          TRUE,  /* Confirm Command is required */
          TDSSIB_EVENT_FAILED_LOWER_LAYERS, /* Failure reason */
          FALSE   /* Do not tear down S_BCCH */
        );
      break; /* Out of the for loop since only one event is active
             at a time. */
    }/* End if active events */
  }/* End for */
}/* End function tdsrrcsib_process_bcch_setup_failure */
/*===========================================================================

FUNCTION RRCSIB_CHECK_MIB_READ_STATUS

DESCRIPTION

  This is the function get's the current active index of tdssib_events_ptr->
  Takes care of pending configuration for a particular event.
  
DEPENDENCIES
 
  None.

RETURN VALUE
  TRUE:  If MIB is already read
  FALSE: If MIB is yet to be read
  
SIDE EFFECTS

  None.
 
===========================================================================*/
boolean tdsrrcsib_check_mib_read_status
(
  boolean for_serv_cell
)
{
  tdssib_events_index_e_type index;  /* Index in the SIB events database
                                     to store information about a SIB event. */
  if(TRUE == for_serv_cell)
  {
    index = TDSSIB_SRV_INDEX;
  }
  else
  {
    index = TDSSIB_NGH_INDEX;
  }

  /* A valid BCCH-BCH message has been received and it needs to be 
     processed. But first check if the event for this index is active.  */
  if( tdssib_events_ptr->event[index].event_active == FALSE )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Index not active %d",index);
  }
  /* If the event is active, make sure we've already received
  the confirmation from LLC that the BCCH has been setup for this
  event. Otherwise this message may not be for this event, so we
  have to ignore it. */
  else if( (index == TDSSIB_SRV_INDEX) && 
           (tdssib_events_ptr->srv_bcch.bcch_action != TDSSIB_BCCH_SETUP_COMPLETE)
         )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"S-BCCH not yet setup");
  }
  else if( (index == TDSSIB_NGH_INDEX) && 
           (tdssib_events_ptr->ngh_bcch.bcch_action != TDSSIB_BCCH_SETUP_COMPLETE)
         )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"N-BCCH not yet setup");
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"MIB for serv *d, decoded %d, sfn_prime", 
                for_serv_cell, 
                tdssib_events_ptr->event[index].tempholder.mib_decoded, 
                tdssib_events_ptr->event[index].tempholder.mib_sfn_prime);

    //if event is active and is setup correctly, then check to see if mib is received and
    //decoded.. if yes return true else return false
    if(FALSE == tdssib_events_ptr->event[index].tempholder.mib_decoded)
    {
      return FALSE;
    }
    else
    {
      return TRUE;
    }
  }

  //by default if event is not active or we are still waiting for cnf from LLC/L1
  //for channel configuration, then return FALSE to indicate that MIB is not yet read.
  return FALSE;
}

/*===========================================================================

FUNCTION PROCESS_BCCH_MESSAGE

DESCRIPTION

  This is the function that processes the received BCCH message.
  The received message can be a BCCH-BCH message or a BCCH-FACH
  message. In case of a BCCH-FACH message, the message type will
  be either a System Information Message or a System Information
  change indication message.
  
  If SIBS have been received in this message, this function also
  calls the necessary functions to decode and store the SIBs.
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
 
===========================================================================*/
static void tdsrrcsib_process_bcch_message
(
  tdsrrc_cmd_type *cmd_ptr  /* Pointer to command */
)
{
  
  tdsrrc_SystemInformation_BCH *msg; /* Local pointer to point to the
                                  the received System Information
                                  message */
  tdssib_events_index_e_type index;  /* Index in the SIB events database
                                  to store information about a SIB event. */
  tdsrrc_sib_collect_e_type result;  /* To save the result of a process_sys
                                  _info_msg function */
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  boolean deferred_meas_read_supported = FALSE;
#endif
  boolean band_mismatch = FALSE;  /* Boolean to indicate band mismatch */
  
  /* Find out whether the message is a BCCH-BCH message or a BCCH-FACH
  message. This is done by checking the logical channel id included 
  in the command. In case of a BCCH-BCH message save the index to
  store the contents of this msg. One index is used for serving cell
  BCCH and the other for neighbor cell BCCH. */
  if((tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id)) == 
    BCCH_S_RADIO_BEARER_ID)
  {
    index = TDSSIB_SRV_INDEX;  /* Message is from Serving cell BCCH */
  }
  else if((tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id)) == 
    BCCH_N_RADIO_BEARER_ID)
  {
    index = TDSSIB_NGH_INDEX;  /* Message is from Neighbor cell BCCH */     
  }
  else if((tdsrrclcm_get_dl_rb_id(cmd_ptr->cmd.downlink_sdu_ind.lc_id)) == 
    BCCH_FACH_RADIO_BEARER_ID)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH-FACH message received");
    //This is not supported yet. When it's supported, we'll check if
    //it's a System Information Change Indication message and if so,
    //process it. We don't process a System Information message on
    //the FACH since only SIB10 is transmitted in the FACH.
    index = TDSMAX_SIB_EVENTS_INDEX;  /* Message is not BCCH-BCH. */     
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Non-BCCH message delivered to SIB module!");
    index = TDSMAX_SIB_EVENTS_INDEX;  /* Invalid message */
  }
      
  if( index < TDSMAX_SIB_EVENTS_INDEX )
  {
    /* A valid BCCH-BCH message has been received and it needs
    to be processed. But first check if the event for this index
    is active.  */

    if( tdssib_events_ptr->event[index].event_active == FALSE )
    {
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
      if(index == TDSSIB_SRV_INDEX)
      {
        if(tdsrrcsib_can_sib7_event_start() == TRUE)
        {
          tdssib7info.sib_ext_proc_update_req = FALSE;
          tdsrrcsib_process_sib7_exp_timer_expiration();
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring the message since event not active");
        }
      }
      else
#endif
      {
        TDSRRC_MSG0(MSG_LEGACY_LOW,"Ignoring the message since event not active");
      }
    }
    /* If the event is active, make sure we've already received
    the confirmation from LLC that the BCCH has been setup for this
    event. Otherwise this message may not be for this event, so we
    have to ignore it. */
    else if( (index == TDSSIB_SRV_INDEX) && 
             (tdssib_events_ptr->srv_bcch.bcch_action != TDSSIB_BCCH_SETUP_COMPLETE)
           )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring msg: BCCH setup ack rxed");
    }
    else if( (index == TDSSIB_NGH_INDEX) && 
             (tdssib_events_ptr->ngh_bcch.bcch_action != TDSSIB_BCCH_SETUP_COMPLETE)
           )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring msg: BCCH setup ack rxed");
    }
    else
    {
      /* Point a local pointer to the received message */
      msg = (tdsrrc_SystemInformation_BCH *)(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg);

   #if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
      #error code not present
#else
      //only if sib-schedule matches continue processing, else do not process sibs
      if((tdsrrcsib_check_sib_schedule_for_sibs(msg ,index) == FALSE))
      {
        return;
      }
   #endif

      /* Now process the message */
      result = tdsrrcsib_process_sys_info_msg( msg, index );

      if( result == TDSSIBS_COMPLETED )
      {
        /* The sibcollect module has completed atleast one SIB.
        We need to look at the SIB event database for completed
        SIBs and ASN.1 decode them. */
        if(FALSE == tdsrrcsib_process_completed_sibs(msg->sfn_Prime, index ))
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Aborting SIB reading");
          return;
        }
      
        if((tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL)
            &&(tdssib_events_ptr->event[index].int_event.int_event_type != TDSRRCSIB_INT_SIB7_EVENT)
            &&(tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received == TRUE)
            &&(tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB_EMPTY_MASK_TIMER) == 0)
        )
        {
          if(
              (tdssib_sleep_after_sb1_sb2 == TRUE)
              ||(tdsrrcsib_g_in_data_call)
          )
          {
            if(((!tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SB1])||
                ((tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SB1])&&
                (tdssib_events_ptr->event[index].tempholder.sb1_decoded)))&&
                ((!tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SB2])||
                ((tdssib_events_ptr->event[index].sched_present_for_sibs[tdsrrc_SB2])&&
                (tdssib_events_ptr->event[index].tempholder.sb2_decoded)))
            )
            {
              tdsrrcsib_update_sib_sleep_schedule_table(index);
            }
          }
          else
          {
            tdsrrcsib_update_sib_sleep_schedule_table(index);
          }
        }
      }
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      if(tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
      {
        /*If the event is cell id read in DCH, then check & perform event completion status here*/
        if(tdsrrcsib_check_event_completion_status_for_cell_id_read((uint32)index) == TRUE )
        {
          tdsrrcsib_post_process_completed_event( index );
        }
      }
      else
#endif
      /* Before exiting, also check the event status to determine
      if all SIBs for the event have been received. If so, a confirmation
      event is sent to the calling procedure. */
      if((tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN) ||
         (tdssib_events_ptr->event[index].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN) ||
         (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN) ||
         (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN)||
         (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN)||
         (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN))
      {
        if(tdsrrcsib_check_event_completion_status_for_bplmn((uint32)index) == TRUE )
        {
          tdsrrcsib_post_process_completed_event( index );
        }
      }
      else 
      {

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
        if(tdsrrcsib_can_event_support_deferred_sib(index) == TRUE)
        {
          if(tdssib_events_ptr->event[index].sib_db_index_found == TRUE)
          {
            deferred_meas_read_supported =
              tdsrrcsib_can_nw_support_deferred_sib(tdssib_events_ptr->event[index].sib_db_index);
          }
        }
#endif

        /* Before exiting, also check the event status to determine
        if all SIBs for the event have been received. If so, a confirmation
        event is sent to the calling procedure. */

        if( tdsrrcsib_check_event_completion_status( (uint32)index 
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
        ,deferred_meas_read_supported
#endif
          ) == TRUE )
        {

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
          if(deferred_meas_read_supported == TRUE)
          {  
            tdsrrcsib_post_process_deferred_sib(index, tdssib_events_ptr->event[index].sib_db_index);
          }
#endif

          /* Check for band mismatch */
          if( (tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) && 
              (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_CSP) &&
              (tdssib_events_ptr->event[index].sib_db_index_found) )
          {
            band_mismatch = tdsrrcsib_check_band_mismatch_from_sib5_sib6((uint32)index);
          }
  
          if(TRUE == band_mismatch) 
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Band mismatch: Barring entire frequency for 1280 sec because of band mismatch");
            tdsrrccsp_send_cphy_cell_bar_req(RRC_CSP_TDS_FREQ_BARRED, 1280);

            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Failing SIB event due to Band Mismatch");
            tdsrrcsib_post_process_failed_event
                     ( index, /* Event Index                 */
                       TRUE,  /* Confirm Command is required */
                       (tdsrrc_sib_status_e_type)TDSSIB_EVENT_FAILED_INVALID_SIBS, /* Failure Cause       */
                       TRUE   /* Tear down S_BCCH if it's up */
                     );
          }
          else
          {
            /* This event has been completed. Call the post-processing
            function to send the event confirmation if needed,
            do the necessary cleanup and teardown the BCCH radio bearer
            if necessary. */
            tdsrrcsib_post_process_completed_event( index );
          } 
        }/* end if( tdsrrcsib_check_event_completion_status( (uint32)index ) == TRUE ) */
      }
    }/* end Else */
  }/* end if( index < TDSMAX_SIB_EVENTS_INDEX ) */
}/* end function tdsrrcsib_process_bcch_message */


/*===========================================================================
FUNCTION RRCSIB_CHECK_FOR_DUPLICATE_UPDATE_SIBS_CMD
DESCRIPTION
  This function checks if there is already a active TDSRRC_UPDATE_SIBS_IND
  event. It should be called when a new TDSRRC_UPDATE_SIBS_IND is received.
DEPENDENCIES
  None.
RETURN VALUE
  TRUE if there is a event, FALSE if not.
SIDE EFFECTS
  None. 
===========================================================================*/
static boolean tdsrrcsib_check_for_duplicate_update_sibs_cmd( uint32 mib_value_tag )
{
  uint32 count; /* Counter used in loop */
  /* Check if any event is active. If so, check the event type  */
  for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
      if( tdssib_events_ptr->event[count].event_name == TDSRRCSIB_UPDATE_SIBS )
      {
        if(tdssib_events_ptr->mib_value_tag_from_command == mib_value_tag)
        {
          /* Event already exists */
          return( TRUE );
        }
        else
        {
          //event is being over-written by a new UPDATE_SIBs as vtag's 
          //are changed..  need to fail the old one and start looking
          //for a MIB with new value tag
          tdsrrcsib_post_process_failed_event
            ( (tdssib_events_index_e_type)count,       /* Event Index                 */
              FALSE,                                /* Confirm Command is required */
              TDSSIB_EVENT_VALID_DUPLICATE_UPDATE_IND, /* Duplicate valid SIB_UPDATE*/
              TRUE                                  /* Tear down S_BCCH if it's up */
            );

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"Valid BCCH-Mod Intrpt. Old-Vtag->%d New-Vtag->%d",tdssib_events_ptr->mib_value_tag_from_command,mib_value_tag);

          return ( FALSE );
        }
      }
    }
  }
  /* Event does not exist */
  return( FALSE );
}

#ifdef FEATURE_3GPP_CSFB_TDS
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
)
{
  tdsrrc_cmd_type *cnf_ptr; /* For sending confirmation */
 
  if( (cnf_ptr = tdsrrc_get_int_cmd_buf()) != NULL )
  {
    cnf_ptr->cmd_hdr.cmd_id = TDSRRC_GET_SPECIFIC_SIBS_CNF;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending GET_SPECIFIC_SIBS_CNF cmd for CSFB");
    /* Copy the frequency and Scrambling code into the
     confirmation command. For GET_SPECIFIC_SIBS command, 
     check if it's the serving cell or neigbor cell. */
    cnf_ptr->cmd.get_sibs_cnf.freq = tdssib_events_ptr->srv_bcch.freq;
    cnf_ptr->cmd.get_sibs_cnf.cpid = tdssib_events_ptr->srv_bcch.cpid;
    cnf_ptr->cmd.get_sibs_cnf.procedure = tdssib_events_ptr->event[TDSSIB_SRV_INDEX].req_proc;

    /*Boolean to indicate whether SIB read over OTA or through the 
           SYS_INFO_container*/
#ifdef FEATURE_3GPP_CSFB_TDS
    if(tdssib_events_ptr->event[TDSSIB_SRV_INDEX].sib_scan_type == TDSRRC_SIB_READ_FROM_SIB_CONTAINER)
    {
      cnf_ptr->cmd.get_sibs_cnf.is_sib_read_from_container = TRUE;
    }
    else
    {
      cnf_ptr->cmd.get_sibs_cnf.is_sib_read_from_container = FALSE;
    }
#endif
    
    cnf_ptr->cmd.get_sibs_cnf.status= status;
    /* Also Copy the frequency and Scrambling code into the
            SIB Database */
    /* No need to reset srv_bcch status here. We expect to get a
       "GET_ALL_SIBS" command for the same, so we should not setup bcch
    again on the same cell. */
    
    /* Copy all the necessary parameters for the confirmation command */
    if(tdssib_events_ptr->event[TDSSIB_SRV_INDEX].sib_db_index_found)
    {
      cnf_ptr->cmd.get_sibs_cnf.cell_id = 
        tdssibdb.cell[tdssib_events_ptr->event[TDSSIB_SRV_INDEX].sib_db_index].cell_id;
      cnf_ptr->cmd.get_sibs_cnf.plmn_id = 
         tdssibdb.cell[tdssib_events_ptr->event[TDSSIB_SRV_INDEX].sib_db_index].plmn_id;
    }

    /* SIB DB Index is also sent in the confirmation so that the procedure
    can use the index to read the SIBs if needed. Alternately the PLMN and
    Cell Id can also be used to read SIBs. */
    cnf_ptr->cmd.get_sibs_cnf.sib_index_for_cell = 
      tdssib_events_ptr->event[TDSSIB_SRV_INDEX].sib_db_index;
    
    /* Now send the command */
    tdsrrc_put_int_cmd( cnf_ptr );
  }


//after a failed event, reset BCCH action to false always..
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB:Clearing SIB event");
  tdssib_events_ptr->srv_bcch.bcch_action = TDSSIB_BCCH_NO_ACTION;
  tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_active = FALSE;
  tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_name = TDSRRCSIB_GET_SPECIFIC_SIBS;
  tdssib_events_ptr->event[TDSSIB_SRV_INDEX].sib_scan_type = TDSRRC_SIB_NORMAL;
  tdssib_events_ptr->event[TDSSIB_SRV_INDEX].sib_db_index_found = FALSE;

}

#endif
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
)
{
  tdssib_events_index_e_type count; /* Counter used in loop */

  tdsrrc_BCCH_FACH_Message *bcch_fach_msg_ptr;         /* Pointer to Downlink DCCH Msg */
  
  uint32 timer_value;

  /* Switch on the received command and take appropriate
  action. */
  switch( cmd_ptr->cmd_hdr.cmd_id )
  {
   
    case TDSRRC_GET_ALL_SIBS_REQ:
      /* A new serving cell has been selected. We need to get
      all the SIBs for this cell */
      /* Call the function to do the necessary processing */
      tdsrrcsib_register_new_sib_event( cmd_ptr );
      break;

    case TDSRRC_GET_SPECIFIC_SIBS_REQ:
      /* This is a request to get specific sibs for either
      the serving cell or a neighbor cell. */

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"GET_SPECIFIC_SIBS contents:for_serving_cell %d,scan type %d",cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell,cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type);

      /* Call the function to do the necessary processing */
      if(((cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN)||
          ((cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN) &&
           (tdsrrc_csp_return_pending_type() != RRC_CSP_TD2TD_BPLMN_DRX_SRCH_TIMER_PENDING))) &&
          (TD2TD_SEARCH_RESUME != tdsrrccsp_bplmn_srch_in_progress()))
      {                                                                      
        TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR BPLMN not active.Trashing the GET_SPECIFIC_SIBS_REQ.Scan type %d",cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type);
        break;      
      }
      if(((cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_NORMAL)||
          (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_MANUAL_SCAN)||
          (cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN)) &&
          (tdsrrccsp_waiting_to_release_lock()))
      {                                                                      
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM: Search Preempted, trash the GET_SPECIFIC_SIBS_REQ,Scan_type %d",cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type);
        break;      
      }
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
      if((tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)&&
         (cmd_ptr->cmd.get_specific_sibs_req.procedure == TDSRRC_PROCEDURE_SIB)
        )
      {
        /*Lets check whether its a sib-7 event*/
        if((cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE)&&
           (cmd_ptr->cmd.get_specific_sibs_req.
                 sib_req[tdsrrc_SIB7] == TRUE)
          )
        {
          /* We call this API as SCCPCH is already up. The scenario being handled here is 
             Section D from the description of this API */
          tdsrrcsib_bcch_reconfigure_for_new_sib_event(cmd_ptr,
                                             cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell,
                                             TDSSIB_SRV_INDEX);
        }
      }
#endif

      if(cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == FALSE)
      {
        if(tdsrrccsp_is_reselection_cleared())
        { 
          /* Break as DL wk indication was received for this cell and reselection data (freq/scr)
                       have been cleared*/
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Reselect info cleared. Trashing GET_SPECIFIC_SIBS_REQ.");
          tdsrrcsib_post_process_failed_event( TDSSIB_NGH_INDEX, TRUE,
                                          TDSSIB_EVENT_FAILED_OTHER, 
                                          FALSE  
                                        );
          break;
        }
      }
#ifdef FEATURE_3GPP_CSFB_TDS
      if((cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type == TDSRRC_SIB_READ_FROM_SIB_CONTAINER) &&
         (tdsrrcsib_read_sibs_from_sys_info_container(cmd_ptr) == TRUE))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB reading successful from SYS_INFO_CONTAINER");
      }
      else
      {
        tdsrrcsib_register_new_sib_event( cmd_ptr );  
      }
#else
      tdsrrcsib_register_new_sib_event( cmd_ptr );
#endif
      break;

    case TDSRRC_UPDATE_SIBS_IND:
      /* We have received indication that the SIBs for
      the cell we have camped on need to be read again.
      Atleast one SIB would have changed. We need to 
      read the MIB again and check the value tags. 
      This is usually triggered by a Paging Type 1 message
      with "BCCH Modification Info IE or by System Info
      Change Indication message. Check for duplicates since
      we can receive several of these messages while processing
      one UPDATE_SIBS command. */
      if((tdsrrcsib_is_current_event_due_to_sfn_drift() == TRUE)&&
          (cmd_ptr->cmd.update_sibs_ind.cause == TDSRRCSIB_SIX_HR_TMR_EXPIRY)
        )
      {
        tdssib_events_ptr->update_sibs_req_proc = TDSRRC_PROCEDURE_SIB;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Overwriting Update_sibs due to SFN drift with six hrs sib");
      }
      else if(tdsrrcsib_is_sib_active() == TRUE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignored UPDATE_SIBS cmd as a sib event is active");
      }
      else if(tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_SIB) == TDSRRC_NOT_CAMPED_ON)           {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignored UPDATE_SIBS cmd as there is no active cell");
      }
      else
      {
        if( (tdsrrcsib_check_for_duplicate_update_sibs_cmd(cmd_ptr->cmd.update_sibs_ind.mib_value_tag)) == TRUE )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignored duplicate UPDATE_SIBS cmd");
        }
        /* Check with LLC is OC status is set for DCH.If so,we are in the middle
        of reconfig and L1 will not be able to handle the PCCPCH setup request.
        Ignore the Update SIBs Ind.If the reconfig is successful we would go to
        DCH anyway.If it fails we would go thru cell selection process and go back to 
        FACH.The only possible pitfall here is if the modification time is out in the
        future and we fail the reconfig,do the cell selection and come back on the same
        cell before the modification time is hit.This is a very narrow window */   
        else if((TDSOC_SET_FOR_CELL_DCH == tdsrrcllc_get_ordered_config_status())||
                    (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)
                  )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring UPDATE_SIBS_IND since OC set for DCH");
        }
        else if(tdsrrc_get_state() == TDSRRC_STATE_CONNECTING)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring UPDATE_SIBS_IND since state is CONNECTING");
        }
        else
        {
        
          /* Check to see if 3G->3G BPLMN is active.If it is active,we have to 
             suspend it before we can send the PCCPCH setup message to L1 */
          if(tdsrrcsib_cmd_store.pending_flag == TRUE)
          {
            {
              /* Save the UPDATE_SIBS command in the command store and set pending flag */
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Overwriting UPDATE_SIBS cmd to process after BPLMN suspension/Update sibs tmr expiry");
              tdsrrcsib_cmd_store.saved_cmd = *cmd_ptr;
              tdsupdate_sibs_event_pending = TRUE;
            }
          }
          else if(TD2TD_SEARCH_STOP != tdsrrccsp_bplmn_srch_in_progress())
          {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"BPLMN active when TDSRRC_UPDATE_SIBS_IND is received");
  
            tdsrrcsib_cmd_store.pending_flag = TRUE;
            tdsupdate_sibs_event_pending = TRUE;
  
            tdsrrccsp_send_bplmn_suspend_req(TDSRRC_PROCEDURE_SIB,TDSBPLMN_SUSPEND_REASON_BCCH_MODIFICATION);
  
            /* Save the UPDATE_SIBS command in the command store and set pending flag */
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Buffering TDSRRC_UPDATE_SIBS_IND command to process after BPLMN suspension");
            tdsrrcsib_cmd_store.saved_cmd = *cmd_ptr;
          }
          else
          {
            /* Save necessary information for this event */
            tdssib_events_ptr->mib_value_tag_from_command =
              cmd_ptr->cmd.update_sibs_ind.mib_value_tag;
    
            tdssib_events_ptr->update_sibs_req_proc = cmd_ptr->cmd.update_sibs_ind.procedure;
  
            tdssib_events_ptr->update_sibs_cause = cmd_ptr->cmd.update_sibs_ind.cause;

            if(tdsrrcllc_get_n_bcch_setup_status() == TRUE)
            {
              tdsrrcsib_cmd_store.pending_flag = TRUE;
              tdsupdate_sibs_event_pending = TRUE;
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Buffering TDSRRC_UPDATE_SIBS_IND command to process after N-BCCH is torn down.");
              tdsrrcsib_cmd_store.saved_cmd = *cmd_ptr;

              if(tdsrrctmr_get_remaining_time(TDSRRC_SIB_CHANGE_GUARD_TIMER) == 0)
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Starting tdssib_change_safe_guard Timer");
               /*Start 1sec timer for safe guarding processing of sib_change_ind against BPLMN*/
                tdsrrctmr_start_timer( TDSRRC_SIB_CHANGE_GUARD_TIMER,TDSRRCSIB_PENDING_UPDATE_SIB_TIMER_VALUE );
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_SIB_CHANGE_GUARD_TIMER is running");
              } 
            } 
            else
            {  
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRC_UPDATE_SIBS_IND cmd received");
              /* Call the function to do the necessary processing */
              tdsrrcsib_register_new_sib_event( cmd_ptr );
            }
          }
        }
      }
      break;

    case TDSRRC_CHANNEL_CONFIG_CNF:
      /* We've received confirmation from LLC for the required
      BCCH setup. Make sure the setup succeeded. */
      /* Check if any event is active. If so cancel it */
      for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
      {
        if( tdssib_events_ptr->event[count].event_active == TRUE )
        {
          if (tdssib_events_ptr->event[count].transaction_id == 
              cmd_ptr->cmd.chan_config_cnf.transaction_id)
          {
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"Transaction Id %d for chan cfg req recvd",cmd_ptr->cmd.chan_config_cnf.transaction_id);

            if( cmd_ptr->cmd.chan_config_cnf.chan_cfg_status == TDSRRCLLC_CHAN_CFG_SUCCESS )
            {
              TDSRRC_MSG2(MSG_LEGACY_HIGH, "Received cnf of BCCH setup, TDSRRCTMR_SIB_WAIT_TIMER remain %d, reset to %d (0: no reset)", 
                          QWORD_LOW(tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB_WAIT_TIMER)),
                          tdsrrcsib_specific_sib_timer_val);

              /* Restart the timer after getting the configure cnf */
              if(tdsrrcsib_specific_sib_timer_val)
              {
                tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );
                tdsrrctmr_start_timer( TDSRRCTMR_SIB_WAIT_TIMER, tdsrrcsib_specific_sib_timer_val);

                /* Clear after one time usage */
                tdsrrcsib_specific_sib_timer_val = 0;
              }
              /* Check which sib_event is waiting for a LLC confirmation */
              if( tdssib_events_ptr->srv_bcch.bcch_action == TDSSIB_BCCH_SETUP_PENDING )
              {
                tdssib_events_ptr->srv_bcch.bcch_action = TDSSIB_BCCH_SETUP_COMPLETE;
              }
              else if( tdssib_events_ptr->ngh_bcch.bcch_action == TDSSIB_BCCH_SETUP_PENDING)
              {
                tdssib_events_ptr->ngh_bcch.bcch_action = TDSSIB_BCCH_SETUP_COMPLETE;
              }
              else
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring CNF for BCCH teardown");
              }

#ifdef FEATURE_3GPP_CSFB_TDS
              if((TDSRRCSIB_GET_SPECIFIC_SIBS == tdssib_events_ptr->event[count].event_name)&&
              (tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_READ_FROM_SIB_CONTAINER))
              {
                tdsrrc_sib_post_complete_for_csfb_sib_read(TDSSIB_EVENT_SUCCEEDED);
              }
#endif
              /* Start ALL_ONE_MASK_TIMER here.
              After it expires, sends all 1 bitmask to L1 so that it can release lock for other SUB.
              After receiving MIB, we can stop the timer. */
              if(tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_NORMAL
                  || tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_AUTO_BPLMN_SCAN
                  || tdssib_events_ptr->event[count].sib_scan_type == RRC_SIB_TD2TD_AUTO_BPLMN_SCAN
                  || tdssib_events_ptr->event[count].sib_scan_type == RRC_SIB_TD2TD_MANUAL_BPLMN_SCAN                    
                  || tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_MANUAL_BPLMN_SCAN
                  || tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_MANUAL_SCAN
                  || tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_DUMMY_PLMN_SCAN)
              {
                /* Generally, SIB_REP of MIB is 8. Treat SIB_REP of MIB as 8 here.
                Make sure the timer value of ALL_ONE_MASK_TIMER is short enough, and there is one MIB to collect before the timer expires. 
                Minimum SIB_REP of MIB is 8, TTI of MIB is 20ms(2 SFN).
                */
                timer_value = ((TDSRRCSIB_MIB_MIN_REP + 2) - (tdsseq_get_sfn() % TDSRRCSIB_MIB_MIN_REP)) * 10;
                
                tdsrrctmr_start_timer(TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER, timer_value);
              }
            }
            else
            {
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
              if(tdsrrcsib_is_current_event_sib_read_in_dch() == TRUE)
              {
                /*Just a Debug message.... */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH setup failed for SIB reading in DCH");
              }
#endif
              TDSRRC_MSG0(MSG_LEGACY_ERROR," BCCH setup failed!");
              /* Handle the BCCH setup failure */
#ifdef FEATURE_3GPP_CSFB_TDS
              if((TDSRRCSIB_GET_SPECIFIC_SIBS == tdssib_events_ptr->event[count].event_name)&&
                 (tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_READ_FROM_SIB_CONTAINER))
              {
                tdsrrc_sib_post_complete_for_csfb_sib_read(TDSSIB_EVENT_FAILED_LOWER_LAYERS);
              }
              else
#endif
              {
                tdsrrcsib_process_bcch_setup_failure();
              }
            }
          }
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Transaction Id %d for chan cfg req did not match with %d",tdssib_events_ptr->event[count].transaction_id,cmd_ptr->cmd.chan_config_cnf.transaction_id);
          }
        }
      }
      /* Note : This is not complete yet! we also need to check for
      TDSSIB_BCCH_RELEASE_PENDING in case we have to release BCCH's.
      This is not implemented right now. */

      break;

    case TDSRRC_DOWNLINK_SDU_IND:
      /* Process the message if it is not NULL */
      if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg != NULL)
      {
        if(cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_BCCH_BCH_Message_PDU)
        {
          /* We've received a System information message. Check
          the message type and call the appropriate function. */
          tdsrrcsib_process_bcch_message( cmd_ptr );
        }
        else  //this is a BCCH_FACH message.. process only sib-mod-ind, we don't support
              //system-info on fach
        {
          /*get the bcch_fach_msg_ptr for sib-change as this message is already decoded*/
          bcch_fach_msg_ptr =  (tdsrrc_BCCH_FACH_Message *) cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;
  
          //we only support sib modification ind.. other messages are not supported
          if(TDSRRCGET_SIB_TYPE_AND_TAG((&bcch_fach_msg_ptr->message))
                  == T_tdsrrc_BCCH_FACH_MessageType_systemInformationChangeIndication)
          {  
           /*Compare the value of IE "MIB value tag" in the IE "BCCH modification info" 
             with the value tag stored for the master information block in SIB data base 
             for serving cell.If both Differ only then process SysInfoChange message other 
             wise UE has current system info so Ignore this sysInfo.This check is made to 
             make UE compliant to 25.331 Standard section 8.1.1.7.3 */
  
            if((tdsrrcsibdb_get_serving_cell_mib_value_tag()) != (bcch_fach_msg_ptr->message.u.systemInformationChangeIndication.
                  bcch_ModificationInfo.mib_ValueTag))
            {
              /*The Handling of BCCH Mod Time is implemented in rrcpg1, So call the 
                the API provided by rrcpg1 for handling BCCH Mod Info I.E*/
  
              /* Check if the modification time is present here*/
              if(TDSRRC_MSG_COMMON_BITMASK_IE_PTR(
                    (&bcch_fach_msg_ptr->message.u.systemInformationChangeIndication.bcch_ModificationInfo),
                    rrc_,
                    bcch_ModificationTime)
              )
              {
                tdsrrcpg1_process_bcch_mod_info_msg_ie(
                                                     (uint8)bcch_fach_msg_ptr->message.u.systemInformationChangeIndication.
                                                       bcch_ModificationInfo.mib_ValueTag,
                                                     (uint16)bcch_fach_msg_ptr->message.u.systemInformationChangeIndication.
                                                       bcch_ModificationInfo.bcch_ModificationTime,
                                                     TRUE
                                                    );
              }
              else
              {
                tdsrrcpg1_process_bcch_mod_info_msg_ie(
                                                     (uint8)bcch_fach_msg_ptr->message.u.systemInformationChangeIndication.
                                                       bcch_ModificationInfo.mib_ValueTag,
                                                     TDSRRCPG1_INVALID_BCCH_MOD_TIMER_VAL,
                                                     FALSE
                                                    );
              }
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignoring SysInfoChange message since Serving cell's MIB value tage is current");
            }
          }//only sibmodificationind is supported at present
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"DOWNLINK_SDU_IND decoded_msg is NULL");
      }
      break;

    case TDSRRC_STATE_CHANGE_IND:

      /* A State Change notification has been received  */
      tdsrrcsib_process_state_change_ind( cmd_ptr );
      break;

    case TDSRRC_SIB_WAIT_TIMER_EXPIRED_IND:

      /* The SIB wait timer has expired. That means we have waited
      for the maximum amount of time for getting SIBs from a cell.
      If any SIB event is active, it needs to be cancelled and a failure
      notice sent to the procedure that requested the event.  */
      tdsrrcsib_process_sib_wait_timer_expiration();
      break;

    case TDSRRC_SIB_STOP_PARTITION_TMR_EXPIRED_IND:
    case TDSRRC_SIB_EMPTY_MASK_TMR_EXPIRED_IND:
    case TDSRRC_SIB_MASK_INVERT_TMR_EXPIRED_IND:
    case TDSRRCTMR_SIB_ALL_ONE_MASK_TMR_EXPIRED_IND:
      
      if(tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_active == TRUE)
      {
        count = TDSSIB_SRV_INDEX;
      }
      else if(tdssib_events_ptr->event[TDSSIB_NGH_INDEX].event_active == TRUE)
      {
        count = TDSSIB_NGH_INDEX;
      }
      else
      {
        return;
      }

      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRCTMR_SIB_ALL_ONE_MASK_TMR_EXPIRED_IND
          && (!tdsrrcsib_g_in_data_call))
      {
        tdsrrcsib_send_empty_table(count, FALSE);      
        return;
      }

      /* Ignore timers for SIB7 events. Added for safety */
      if(tdssib_events_ptr->event[count].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)
      {
        return;
      }
      
      if((FALSE == tdsrrcsib_g_in_data_call) && (cmd_ptr->cmd_hdr.cmd_id != TDSRRC_SIB_STOP_PARTITION_TMR_EXPIRED_IND))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Not in G2T TA mode,ignore CMD %d",cmd_ptr->cmd_hdr.cmd_id);
        return;
      }

      if(cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SIB_STOP_PARTITION_TMR_EXPIRED_IND)
      {
        tdsrrcsib_stop_partition = TRUE;
        if ((tdssib_events_ptr->event[count].tempholder.mib_decoded == TRUE) || 
            (tdssib_events_ptr->event[count].tsib[tdsrrc_MIB].sib_decoded == TRUE))
        {
          tdsrrcsib_update_sib_sleep_schedule_table(count);
        }
        tdsrrctmr_stop_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER);
      }
      else if((cmd_ptr->cmd_hdr.cmd_id == TDSRRC_SIB_MASK_INVERT_TMR_EXPIRED_IND)&&(!tdsrrcsib_stop_partition))
      {
        tdsrrcsib_pattern_invert = !tdsrrcsib_pattern_invert;
        tdsrrcsib_send_empty_table(count, TRUE);
        tdsrrctmr_start_timer(TDSRRCTMR_SIB_EMPTY_MASK_TIMER, TDSRRCSIB_EMPTY_FRAMES);
        tdsrrctmr_start_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER,
                              MAX(tdssib_events_ptr->scheduling_max_rept_rate, TDSRRCSIB_MIN_MASK_INV_TIMER)*TDSRRCSIB_MASK_INVERT_FACTOR
                              + TDSRRCSIB_TRANS_BUFFER + TDSRRCSIB_EMPTY_FRAMES);
      }
      else
      {
        if(((!tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB1])||
           ((tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB1])&&
            (tdssib_events_ptr->event[count].tempholder.sb1_decoded))
           )&&
          ((!tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB2])||
           ((tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB2])&&
           (tdssib_events_ptr->event[count].tempholder.sb2_decoded))
           )
          )
        {
          tdsrrcsib_update_sib_sleep_schedule_table(count);
        }
        else
        {
          tdsrrcsib_send_empty_table(count, FALSE);
        }
      }
      break;
    case TDSRRC_CPHY_G2T_MODE_IND:
      if(tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_active == TRUE)
      {
        count = TDSSIB_SRV_INDEX;
      }
      else if(tdssib_events_ptr->event[TDSSIB_NGH_INDEX].event_active == TRUE)
      {
        count = TDSSIB_NGH_INDEX;
      }
      else
      {
        /*Don't have active SIB event*/
        return;
      }
      
	  /*We only handle in TDSRRC_SIB_NORMAL & not TDSRRCSIB_INT_SIB7_EVENT*/
      if(!((tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_NORMAL)&&
            (tdssib_events_ptr->event[count].int_event.int_event_type != TDSRRCSIB_INT_SIB7_EVENT)))
      {
        return;
      }
	  if ((count == TDSSIB_SRV_INDEX) && (TRUE == tdsrrccsp_do_opt_for_con_to_idle()))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH, "Cell sel:In DDS from Connected to IDLE, don't need to check this flag",0,0,0);
        return;
      }
      else if ((count == TDSSIB_NGH_INDEX) && (TRUE == tdsrrccsp_is_other_sub_in_dds()))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH, "Cell resel:In DDS, don't need to check this flag",0,0,0);
        return;
      } 	  
      if(cmd_ptr->cmd.g2t_ta_mode_ind.is_g2t_ta_mode == TRUE)/*In G2T TA*/
      {
        if(FALSE == tdsrrcsib_g_in_data_call)
        {
          tdsrrcsib_g_in_data_call = TRUE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Enter to G2T TA mode");
          if((tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB_STOP_PARTITION_TIMER) == 0) || (tdsrrcsib_stop_partition == TRUE))
          { 
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Stop partition timer expires,don't need to do partition.");
          }
          else if ((tdssib_events_ptr->event[count].tempholder.mib_decoded == TRUE) || 
                   (tdssib_events_ptr->event[count].tsib[tdsrrc_MIB].sib_decoded == TRUE))
          {
            if(tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB1]
                && tdssib_events_ptr->event[count].tsib[tdsrrc_SB1].sib_needed == TRUE
                && tdssib_events_ptr->event[count].tsib[tdsrrc_SB1].sib_received == FALSE
                && tdssib_events_ptr->sib_sched[tdsrrc_SB1].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP
                && tdssib_events_ptr->sib_sched[tdsrrc_SB1].sfn_offset[0] % (TDSRRCSIB_SLEEP_SFN + TDSRRCSIB_WAKE_SFN) < TDSRRCSIB_SLEEP_SFN)
            {
              /* Try to receive SB1 firstly if scheduled. */
              tdsrrcsib_pattern_invert = TRUE;
              TDSRRC_MSG0(MSG_LEGACY_HIGH, "Optimization for receiving SB1 ASAP takes effect");
            }

            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update bitmask with partition.");
            tdsrrctmr_stop_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER);
            tdsrrctmr_stop_timer(TDSRRCTMR_SIB_EMPTY_MASK_TIMER);
            tdsrrcsib_send_empty_table(count, TRUE);
            tdsrrctmr_start_timer(TDSRRCTMR_SIB_EMPTY_MASK_TIMER, TDSRRCSIB_EMPTY_FRAMES);
            tdsrrctmr_start_timer(TDSRRCTMR_SIB_MASK_INVERT_TIMER,
																	MAX(tdssib_events_ptr->scheduling_max_rept_rate, TDSRRCSIB_MIN_MASK_INV_TIMER)*TDSRRCSIB_MASK_INVERT_FACTOR
																	+ TDSRRCSIB_TRANS_BUFFER + TDSRRCSIB_EMPTY_FRAMES);
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcsib_g_in_data_call is already in TRUE");
        }
      }
      else  /*Not in G2T TA*/
      {
        if(TRUE == tdsrrcsib_g_in_data_call)
        {
          tdsrrcsib_g_in_data_call = FALSE;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Out of G2T TA mode ");
          tdsrrctmr_stop_timer( TDSRRCTMR_SIB_MASK_INVERT_TIMER );
          tdsrrctmr_stop_timer( TDSRRCTMR_SIB_EMPTY_MASK_TIMER );

          /*If  tdsrrcsib_stop_partition is TRUE or this timer expires, L1 already uses full schedule info bitmask */
          if ((tdsrrcsib_stop_partition == FALSE) 
               && (tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB_STOP_PARTITION_TIMER) != 0)
               && ((tdssib_events_ptr->event[count].tempholder.mib_decoded == TRUE) || 
                   (tdssib_events_ptr->event[count].tsib[tdsrrc_MIB].sib_decoded == TRUE)))
          {
            /*Update full schedule info bitmask to L1*/
            if(((!tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB1])||
                ((tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB1])&&
                (tdssib_events_ptr->event[count].tempholder.sb1_decoded))
                )&&
               ((!tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB2])||
                ((tdssib_events_ptr->event[count].sched_present_for_sibs[tdsrrc_SB2])&&
                (tdssib_events_ptr->event[count].tempholder.sb2_decoded))
               )
           )
           {
             tdsrrcsib_update_sib_sleep_schedule_table(count);
           }
           else
           {
             tdsrrcsib_send_empty_table(count, FALSE);
           }
         }
        } 
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdsrrcsib_g_in_data_call is already in FALSE");
        }
      }
      break;

    case TDSRRC_SIB7_EXP_TIMER_EXPIRED_IND:

      /* The SIB7 expiration timer has expired. That means we
      have to read SIB7 again for the serving cell. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB7 expired - Reading it again");
      tdsrrcsib_process_sib7_exp_timer_expiration();
      break;

    case TDSRRC_BPLMN_SUSPEND_CNF:
      
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"WTOW SIB Rx:TDSRRC_BPLMN_SUSPEND_CNF with status %d",cmd_ptr->cmd.rrc_csp_bplmn_suspend_cnf.status);

      if(cmd_ptr->cmd.rrc_csp_bplmn_suspend_cnf.status == SUCCESS)
      {
        /* Check to see if there is a buffered cmd.Currently we support only UPDATE_SIBS_IND */
        if( (tdsrrcsib_cmd_store.pending_flag == TRUE) &&
            (tdsrrcsib_cmd_store.saved_cmd.cmd_hdr.cmd_id == TDSRRC_UPDATE_SIBS_IND) )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Processing buffered TDSRRC_UPDATE_SIBS_IND cmd after BPLMN suspension");

          /* Save necessary information for this event */
          tdssib_events_ptr->mib_value_tag_from_command =
          tdsrrcsib_cmd_store.saved_cmd.cmd.update_sibs_ind.mib_value_tag;

          tdssib_events_ptr->update_sibs_req_proc =
          tdsrrcsib_cmd_store.saved_cmd.cmd.update_sibs_ind.procedure;

          tdssib_events_ptr->update_sibs_cause = 
          tdsrrcsib_cmd_store.saved_cmd.cmd.update_sibs_ind.cause;

          /* Call the function to do the necessary processing */
          tdsrrcsib_register_new_sib_event(&tdsrrcsib_cmd_store.saved_cmd);
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"No pending command in SIB cmd store after BPLMN_SUSPEND_CNF");
        }
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR CSP sent SUSPEND_CNF with status FAILURE");
      }

    /* Set the pending flag to FALSE */
      tdsupdate_sibs_event_pending = FALSE;

    /* Clear the command store */
      tdsrrcsib_cmd_store.pending_flag = FALSE;
      break;

      case TDSRRC_SIX_HRS_SIB_TIMER_UPDATE_IND:
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Update 6hr SIB TIMER");
        tdsrrcsib_update_sixhr_sib_timer();
        break;

      case TDSRRC_SIB_CHANGE_GUARD_TIMER_EXPIRED_IND:
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB CHANGE GUARD Timer expired");
          tdssib_change_safe_guard = FALSE;
          /* Check if there is any pending event for the old serving
                cell an if so, process that event. */
          if((tdsupdate_sibs_event_pending == TRUE) &&
             (tdsrrcsib_cmd_store.pending_flag == TRUE) &&
             (tdsrrcsib_cmd_store.saved_cmd.cmd_hdr.cmd_id == TDSRRC_UPDATE_SIBS_IND)&&
             (tdssibdb.active_cell_index != TDSSIBDB_INVALID_INDEX)&&
             (tdssibdb.active_cell_sibs_valid == TRUE)
            )
          {
            /* Post a command to ourselves for updating the SIBs */
            tdsrrcsib_send_command_for_saved_update_sibs_event();
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Saved UPDATE_SIBS event activated");

            tdsupdate_sibs_event_pending = FALSE;
            tdsrrcsib_cmd_store.pending_flag = FALSE;
          }
          break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid SIB event %x",cmd_ptr->cmd_hdr.cmd_id);
      break;
  }/* end Switch */
}/* end function tdsrrcsib_event_controller */

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
void tdsrrcsib_proc_events_init( void )
{
  tdssib_events_index_e_type count; /* Counter used in loops */
  
  /* First clear the SIB events database */
  if(tdssib_events_ptr != NULL)
  {
    for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
    {
      tdssib_events_ptr->event[count].event_active = FALSE; /* No SIB events active now */
      tdssib_events_ptr->event[count].sib_scan_type = TDSRRC_SIB_NORMAL;
    }
    /* Initialize the BCCH parameters */
    tdssib_events_ptr->srv_bcch.bcch_action = TDSSIB_BCCH_NO_ACTION;
    tdssib_events_ptr->ngh_bcch.bcch_action = TDSSIB_BCCH_NO_ACTION;
  }
  /* Note that there is no need to initialize any other SIB events database
  element here since they will be initialized when an event becomes active */

  /* Indicate there is no TDSRRCSIB_UPDATE_SIBS event pending at this
  time. This is used only after we select a serving cell. */
  tdsupdate_sibs_event_pending = FALSE;

  /* Clear the cmd store pending flag */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting SIB cmd store pending flag to FALSE");
  tdsrrcsib_cmd_store.pending_flag = FALSE;

  /* Indicate there is no SIB7 expiration info stored at present */
  tdssib7info.sib_ext_proc_update_req = FALSE;
  tdssib7info.sib_timer_update_req = FALSE;

  /* Then initialize the SIB database. If this is a power-up
  init, we can just initialize all the variables. Otherwise we
  may have to free any used SIB database pointers to avoid memory
  leaks. */
  if( tdsrrcsib_powerup_init_done == TRUE )
  {
    /* This is not a power-up init, so the procedue may have been
    active before this. Stop any timers in case they are running. */
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_WAIT_TIMER );
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB7_EXP_TIMER ); 

    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_STOP_PARTITION_TIMER );
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_MASK_INVERT_TIMER );
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_EMPTY_MASK_TIMER );
    tdsrrctmr_stop_timer( TDSRRCTMR_SIB_ALL_ONE_MASK_TIMER );

    /* For safety, in case the tempholer pointers are not freed, free them
    now */
    for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
    {
      tdsrrcsib_free_tsib(count);
    }
  }
/* Global Flag which prevents BPLMN(If BPLMN request is received when UPDATE_SIBs
  was on going) to start till all the SIB_Change  commands are processed by procedures*/
  tdssib_change_safe_guard = FALSE;
#ifdef FEATURE_RRC_NO_MIB_FOUND_BAR_CELL
  tdsno_mib_bar_cell_count = 0;
#endif

  /*Initialize tsib optimization data base*/
  tdsrrcsib_clear_tsib_optimization_db();

}/* end function tdsrrcsib_proc_events_init */

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
void tdsrrcsib_database_init( void )
{
  uint32 count; /* Counter used in loops */

  /* If this is a power-up
  init, we can just initialize all the variables. Otherwise we
  may have to free any used SIB database pointers to avoid memory
  leaks. */
  if( tdsrrcsib_powerup_init_done == FALSE )
  {
    tdstransaction_id = 0;
    tdsrrcsibdb_init();
  }
  else
  {
    /* For each SIBDB index, if the index is used, then
    free each sib pointer in the index. */
    for( count=0; count<=TDSSIBDB_MAX_INDEX; count++ )
    {
      if(tdssibdb.cell[count].index_used == TRUE)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB Heap: Freeing SIB Idx %d sibdbinit",count);
        tdsrrcsibdb_cell_header_init(count);/* This frees all the SIBs */
      }
    }
    /* Now initialize SIBDB variables */
    tdsrrcsibdb_init();

    tdsrrcsib_get_nv_items();
  }
}/* end function tdsrrcsib_database_init */

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
void tdsrrcsib_proc_init( void )
{


  tdssib_events_ptr = NULL;

  /* Initialize SIB events */
  tdsrrcsib_proc_events_init();
  /* Initialize SIB DB when power-up */
  tdsrrcsib_database_init();
  /*Initialize the SIB-Proc supported database */
  tdsrrcsib_initialize_supported_sibs_lookup_table();

  /* Also send registration to state change manager. We need to know when
  RRC goes to Idle mode (disconnected state). The BCCH will be cleared at
  that time so we need to clear our internal variables that keep track of
  the BCCH status. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_SIB,  /* Procedure name */
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_DISCONNECTED /* To State    */
                           );

  /* We also need to know when we move from any other state to 
  CELL_DCH state. In CELL_DCH State it is not possible to have
  the BCCH-BCH pipe up and LLC will bring down the pipe.
  We need to clear our internal variables */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_SIB,  /* Procedure name */
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_CELL_DCH  /* To State       */
                           );

  /* We also need to know when we move from any other state to 
  CELL_FACH state. Only in CELL_FACH State we will perform expiration
  timer based update of SIBs. This is done only for SIBs with
  expiration timers. */

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_SIB,   /* Procedure name */ 
                             TDSRRC_STATE_WILDCARD,  /* From State     */     
                             TDSRRC_STATE_CELL_FACH  /* To State       */       
                           );
#endif

#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH

  /* And we register for state change from FACH to any other
  state. This is to stop timer based update of SIBs with expiration
  timers. */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_SIB,   /* Procedure name */ 
                             TDSRRC_STATE_CELL_FACH, /* From State     */
                             TDSRRC_STATE_WILDCARD   /* To State       */
                           );
#endif /* FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH */

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING

  /* And we register for state change from DCH to URA PCH
     state. This is to trigger update SIBs for deferred SIBs */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_SIB,   /* Procedure name */ 
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_URA_PCH   /* To State       */
                           );
  /* And we register for state change from DCH to CELL PCH
     state. This is to trigger update SIBs for deferred SIBs */
  tdsrrcscmgr_register_for_scn( TDSRRC_PROCEDURE_SIB,   /* Procedure name */ 
                             TDSRRC_STATE_WILDCARD, /* From State     */
                             TDSRRC_STATE_CELL_PCH   /* To State       */
                           );
#endif

  //Start 10min timer for getting six hours timer
  tdsrrctmr_start_timer( TDSRRCTMR_SIXHR_SIB_UPDATE_TIMER,
                        TDSRRCSIB_SIX_HR_SIB_UPDATE_TIMER );

  /* Note that power-up initialization has been done */
  tdsrrcsib_powerup_init_done = TRUE;

}/* end function tdsrrcsib_proc_init */



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
)
{
  uint32 temp_index;
  tdsrrc_SysInfoType1 *sib1 = NULL;
  uint32 count = 0;

  if( new_cell_selection_succeeded == TRUE )
  {
    /* A new serving cell has been selected. Store the given
    index. When other modules want to read sibs from the serving
    cell, this index is used to find the required sibs. */
    if( ( temp_index = tdsrrcsibdb_find_cell_index( plmn, cell )) !=
        TDSSIBDB_INVALID_INDEX )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Serving Cell Index is now %d",temp_index);

      for(count = 0; count <TDSSIBDB_INVALID_INDEX; count++)
      {
        if((count != temp_index) && (tdssibdb.cell[count].index_used == TRUE))
        {
          TDSRRC_PRINT_SIB_HEAP();
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free ASN.1 decoded sib's for tdssibdb idx= %d",count);
          tdsrrcsibdb_free_all_decodedsibs_for_a_cell(count);
          TDSRRC_PRINT_SIB_HEAP();
        }
      }

      tdssibdb.active_cell_index = temp_index;
      tdssibdb.active_cell_sibs_valid = TRUE;

      /* Whenever we go to a new cell we need to check if the
        connected mode timers and constants need to be updated.
      */
      if( (tdsrrcumi_check_ue_timer_constants_update_status(TRUE))
          == TDSRRC_CONN_TIMER_AND_CONSTANTS_UPDATE_REQ )
      {
        sib1 = tdsrrcsibdb_return_sib_for_db_index(
          tdsrrc_SIB1,temp_index);
        if(sib1 != NULL)
        {
          (void)tdsrrc_update_ue_conn_timers_and_constants
            (          
            (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib1, rrc_SysInfoType1_,ue_ConnTimersAndConstants)) ? 
                &(sib1->ue_ConnTimersAndConstants):
                NULL,            
            (TDSRRC_MSG_COMMON_BITMASK_IE_PTR(sib1, rrc_SysInfoType1_,v3a0NonCriticalExtensions))? 
                &(sib1->v3a0NonCriticalExtensions.
                sysInfoType1_v3a0ext.ue_ConnTimersAndConstants_v3a0ext):
                NULL
#if defined(FEATURE_3GPP_FAST_DORMANCY)
            ,((sib1->m.v3a0NonCriticalExtensionsPresent) &&
              (sib1->v3a0NonCriticalExtensions.m.v860NonCriticalExtentionsPresent))? 
              &(sib1->v3a0NonCriticalExtensions.v860NonCriticalExtentions.
              sysInfoType1_v860ext.ue_ConnTimersAndConstants):
              NULL
#endif                
            );
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB1 for serving cell is NULL!");
          return( FAILURE );
        }
      }
#if defined(FEATURE_3GPP_FAST_DORMANCY)
      else
      {
        sib1 = tdsrrcsibdb_return_sib_for_db_index(
          tdsrrc_SIB1,temp_index);
        if(sib1 != NULL)
        {
          (void)tdsrrc_update_ue_conn_timers_and_constants(NULL, NULL, 
             ((sib1->m.v3a0NonCriticalExtensionsPresent) &&
              (sib1->v3a0NonCriticalExtensions.m.v860NonCriticalExtentionsPresent))? 
              &(sib1->v3a0NonCriticalExtensions.v860NonCriticalExtentions.
              sysInfoType1_v860ext.ue_ConnTimersAndConstants):
              NULL
            );
        }
      }
#endif      
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid Serving Cell selected");

      //rrc_sibdb_init();
      return( FAILURE );
    }
    /* Pending UPDATE_SIBS event can be cancelled since
    we're on a new cell now. */
    if( tdsupdate_sibs_event_pending == TRUE )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pending UPDATE_SIBS event cleared");
      tdsupdate_sibs_event_pending = FALSE;
      if((tdsrrcsib_cmd_store.pending_flag == TRUE) &&
         (tdsrrcsib_cmd_store.saved_cmd.cmd_hdr.cmd_id == TDSRRC_UPDATE_SIBS_IND)
        )
      {
        tdsrrcsib_cmd_store.pending_flag = FALSE;
      }
      if(tdsrrctmr_get_remaining_time(TDSRRC_SIB_CHANGE_GUARD_TIMER) != 0)
      {
        tdsrrctmr_stop_timer( TDSRRC_SIB_CHANGE_GUARD_TIMER);
      }
    }
    tdsrrcpg1_check_and_abort_bcch_mod_info_processing();

  }
  else
  {
    /* A new cell selection/reselection process failed.
    First check if the old cell is still valid. If not,
    we don't have any valid serving cell. */
    if( existing_serving_cell_valid == FALSE )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Serving Cell no longer valid!");
      for(count = 0; count <TDSSIBDB_INVALID_INDEX; count++)
      {
        if(tdssibdb.cell[count].index_used == TRUE)
        {
          TDSRRC_PRINT_SIB_HEAP();
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free ASN.1 decoded sib's for tdssibdb idx= %d",count);
          tdsrrcsibdb_free_all_decodedsibs_for_a_cell(count);
          TDSRRC_PRINT_SIB_HEAP();
        }
      }
      tdssibdb.active_cell_sibs_valid = FALSE;
      tdssibdb.active_cell_index = TDSSIBDB_INVALID_INDEX;
      /* Pending UPDATE_SIBS event can be cancelled since
         serving cell is now invalid. */
      if( tdsupdate_sibs_event_pending == TRUE )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pending UPDATE_SIBS event cleared");
        tdsupdate_sibs_event_pending = FALSE;
      }
      tdsrrcpg1_check_and_abort_bcch_mod_info_processing();
    }
    else
    {
      for(count = 0; count <TDSSIBDB_INVALID_INDEX; count++)
      {
        if((count != tdssibdb.active_cell_index) && (tdssibdb.cell[count].index_used == TRUE))
        {
              TDSRRC_PRINT_SIB_HEAP();
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free ASN.1 decoded sib's for tdssibdb idx= %d",count);
              tdsrrcsibdb_free_all_decodedsibs_for_a_cell(count);
              TDSRRC_PRINT_SIB_HEAP();
        }
      }
      /* Check if there is any pending event for the old serving
      cell and if so, process that event. */
      if( tdsupdate_sibs_event_pending == TRUE )
      {
        /* Post a command to ourselves for updating the SIBs */
        tdsrrcsib_send_command_for_pending_update_sibs_event();
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Pending UPDATE_SIBS event activated");
        tdsupdate_sibs_event_pending = FALSE;
      }
    }
  }/* end if new_cell_selection_succeeded == FALSE */
  return( SUCCESS );
}/* end function tdsrrcsib_change_serving_cell_status */



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
)
{
  tdsrrc_cmd_type *cmd; /* To send a SIB command if needed */
  uint32 count;      /* Local Counter */
  boolean result = TRUE;    /* Store result  */

  /* If TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER not expired yet,
     consider sib7 to be current and valid */
  if (tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER) > 0) 
  {
    /* Check UPDATE_SIBS event */
    for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
    {
      if( tdssib_events_ptr->event[count].event_active == TRUE )
      {
        if(tdssib_events_ptr->event[count].event_name == TDSRRCSIB_UPDATE_SIBS)
        {
          /* Post-process the failed event.to kill the RRCSIB_UPDATE_SIBS
                   event*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRCTMR_SIB7_UPDATE_WAIT_TIMER not expired, Sikp SIB7,Dropping ongoing TDSRRCSIB_UPDATE_SIBS event");
          tdsrrcsib_post_process_failed_event
            ( (tdssib_events_index_e_type)count, /* Event Index                 */
              FALSE,  /* Confirm Command is required */
              TDSSIB_EVENT_FAILED_OTHER, /* Failure cause */
              TRUE  /* Tear down S_BCCH */
             );
        }
      }
    }/* End if active events */
    return TRUE;
  }

#ifdef FEATURE_3GPP_CSFB_TDS
  if(tdsrrc_csfb_call_status) /*when CSFB is active bypass reading SIB7*/
  {
    for( count = TDSSIB_SRV_INDEX; count < TDSMAX_SIB_EVENTS_INDEX; count++ )
    {
      if( tdssib_events_ptr->event[count].event_active == TRUE )
      {
        if(tdssib_events_ptr->event[count].event_name == TDSRRCSIB_UPDATE_SIBS)
        {
          /* Post-process the failed event.to kill the RRCSIB_UPDATE_SIBS
                   event*/
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Dropping RRCSIB_UPDATE_SIBS event and skippng SIB 7 Update");
          tdsrrcsib_post_process_failed_event
            ( (tdssib_events_index_e_type)count, /* Event Index                 */
              FALSE,  /* Confirm Command is required */
              TDSSIB_EVENT_FAILED_OTHER, /* Failure cause */
              TRUE  /* Don't tear down S_BCCH */
             );
        }
      }
    }/* End if active events */
    return TRUE;
  }
#endif

/*lint -e527 statement unreachable */
  /* Check if any Nighbor BCCH event is active.
  Nieghbor BCCH events are considered higher priority
  so we'll use the current SIB7 rather than pre-empting
  the neighbor BCCH event. */
  if( tdssib_events_ptr->event[TDSSIB_NGH_INDEX].event_active == TRUE)
  {
    /*----------------------------------------------------------------------------
    Check if inter-frequency cell reselection is in progress. If yes, then the
    SIB7 update requested by RCE is given higher priority than the cell-reselection.
    First post the failure indication to CSP. This will enable CSP to tear down
    N-BCCH and block further reselections at L1. After this post an internal
    command to SIB proc with a GET_SPECIFIC_SIBS cmd. By the time, the 
    GET_SPECIFIC_SIBS command is dequeued, there would no conflicts as the 
    N-BCCH would have already been torn down.
    ----------------------------------------------------------------------------*/
    if(tdsrrccsp_is_inter_frequency_cell_resel_in_progress())
    {
      /* First send a failure status to CSP procedure */
      tdsrrcsib_post_process_failed_event
      ( TDSSIB_NGH_INDEX, /* Event Index                 */
        TRUE,  /* Confirm Command is required */
        TDSSIB_EVENT_FAILED_OTHER, /* Failure reason */
        FALSE   /* Dont Tear down S_BCCH  */
      );

      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Replacing GET_SPECIFIC_SIBS for InterFreq with SIB7 event");

      /* Don't flag the existing event as a pending event. */
      tdsupdate_sibs_event_pending = FALSE;

      /* Make the current event inactive */
      tdssib_events_ptr->event[TDSSIB_NGH_INDEX].event_active = FALSE;

    }
    else
    {
      /* If there is no conflict with inter-frequency then proceed as earlier.
      Send TRUE status to RCE so that it can establish the connections */
      result = TRUE;
      return result;
    }
  }
  else if (tdsrrcllc_get_n_bcch_setup_status() == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"N-BCCH is up. Use current SIB7.");
    result = TRUE;
    return result;
  }

  /* Check if we're in Idle Mode. In Idle Mode the UE is in DRX
  mode and we don't wake up to read SIB7 so just assume that it's
  not valid. */
  if ((tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED) 
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_FOR_PCH_TO_FACH
    || (tdsrrc_get_state() == TDSRRC_STATE_CELL_PCH)
    || (tdsrrc_get_state() == TDSRRC_STATE_URA_PCH)
#endif
      )
  {
    result = FALSE; 
    /* Generate a command for the SIB procedure itself to
    get SIB7. Make sure there's not already a pending
    command  */
    if( tdssib7info.sib_ext_proc_update_req == FALSE )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 7 Update requested");
      if((cmd = tdsrrc_get_int_cmd_buf())!=NULL)
      {
        cmd->cmd_hdr.cmd_id = TDSRRC_GET_SPECIFIC_SIBS_REQ;
        cmd->cmd.get_specific_sibs_req.for_serv_cell = TRUE;
        for( count=0; count<TDSRRC_TOT_SIBS; count++ )
        {
          cmd->cmd.get_specific_sibs_req.sib_req[count] = FALSE;
        }
        /* Only SIB7 is requested. */
        cmd->cmd.get_specific_sibs_req.sib_req[tdsrrc_SIB7]
          = TRUE;
        cmd->cmd.get_specific_sibs_req.procedure = TDSRRC_PROCEDURE_SIB;
        /* Get the frequency and the scrambling code from the
        SIB database itself */
        cmd->cmd.get_specific_sibs_req.freq = 
          tdssibdb.cell[tdssibdb.active_cell_index].freq;
        
        cmd->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_NORMAL;

        cmd->cmd.get_specific_sibs_req.cpid =
          tdssibdb.cell[tdssibdb.active_cell_index].cpid;

        tdsrrc_put_int_cmd( cmd );

        /* Set an internal variable to indicate that another
        procedure requested the SIB7 update */
        tdssib7info.sib_ext_proc_update_req = TRUE;
        tdssib7info.req_proc = procedure;
      }
      else
      {
        ERR_FATAL("No memory to send int event",0,0,0);
      }
    }
    else
    {
      /* Can't support more than one procedure request
      at a time */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"SIB7 request error");
    }
  }
  else
  {
    /* For now we don't support CELL_PCH and URA_PCH state
    and we don't sleep in any other state. So SIB7 should
    always be valid */
    result = TRUE; 
  }

  return(result);
/*lint +e527 statement unreachable */
}

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
uecomdef_status_e_type tdsrrcsib_get_sib_7_update_status
(
  void
)
{
  uint32  count; /* Counter used in loops */

  for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    // Check if the event is stil active.
    if (tdssib_events_ptr->event[count].event_active == TRUE)
    {
      // Ensure that the SIB7 update was requested.
      if( (tdssib_events_ptr->event[count].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
          (tdssib_events_ptr->event[count].req_proc == TDSRRC_PROCEDURE_SIB))
      {
        //sib7 update is on-going
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB-7 Update on-going");
        return SUCCESS;
      }
    }
  }

  //sib7 update is not happening
  return FAILURE;
}

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
void tdsrrcsib_force_sib_event_init(tdsrrcsib_event_abort_cause_e_type abort_cause) 
{
  uint32  count; /* Counter used in loops */
  uint32  dbindex;
  uint32  sib;

  tdsrrc_csp_scan_e_type curr_scan = tdsrrccsp_get_curr_scan();


  /*To take care of scenarios where SIB event is getting aborted when CSP is waiting for BCH release confirm*/
  if((curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)||
     (curr_scan == RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN))
  {
    tdsrrccsp_reset_wait_l1_rel_lock();
  }

  switch (abort_cause)
  {
    case TDSRRCSIB_ABORT_SIB7_UPDATE:
      /* First clear the SIB events database */
      for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
      {
        // Check if the event is stil active.
        if (tdssib_events_ptr->event[count].event_active == TRUE)
        {
          // Ensure that the SIB7 update was requested.
          if( (tdssib_events_ptr->event[count].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
              (tdssib_events_ptr->event[count].req_proc == TDSRRC_PROCEDURE_SIB)
#ifdef FEATURE_TDSCDMA_UPDATE_SIB7_IN_FACH
              &&/*Since CSP calls tdsrrcsib_force_sib_event_init with casue RRCSIB_ABORT_SIB7_UPDATE 
                  on  receiving DL_WEAK ind, for sib-7 event in Fach its ok to ignore & continue
                  as RRC will either get OOS or perform Cell reselction at the end */
              (tdsrrcsib_is_current_event_sib7_in_fach() == FALSE)
#endif
            )
          {
            // If the a database index was found/created for this event
            // then clear up all the sibs that were got until this time.
            if (tdssib_events_ptr->event[count].sib_db_index_found == TRUE)
            {
              dbindex = tdssib_events_ptr->event[count].sib_db_index;
      
              // If an index has already been found then clear allthe sibs collected so far
              // and clear other variables associated with this index
              if (dbindex != TDSSIBDB_INVALID_INDEX)
              {
                /* The decoding failed */
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clearing TSIB variables for SIB7");

                sib = (uint32)tdsrrc_SIB7;

                /*Reset all the variables for this SIB so that we can try to
                read it again */
                tdssib_events_ptr->event[count].tsib[sib].sib_received = FALSE;
                tdssib_events_ptr->event[count].tsib[sib].sib_decoded = FALSE;
                tdssib_events_ptr->event[count].tsib[sib].seg_count = 0;
                tdssib_events_ptr->event[count].tsib[sib].seg_index = 
                  (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
                tdssib_events_ptr->event[count].tsib[sib].sib_tot_length = 0;
              }
            }
            
            if (tdssib_events_ptr->srv_bcch.bcch_action == TDSSIB_BCCH_NO_ACTION)
            {
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,FALSE,TDSSIB_EVENT_FAILED_OTHER, FALSE);
            }
            else //if bcch_action == TDSSIB_BCCH_SETUP_COMPLETE, or bcch_action == TDSSIB_BCCH_SETUP_PENDING
            {
              /* If the channel config req for setting serving BCCH is sent to LLC but 
              the confirm for the same is not received yet. or the BCCH has already been
              setup then send another request for teardown. The 4th parameter in the
              below function is set to TRUE so that post_process_failed event will take
              care of sending the BCCH tear down*/
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,FALSE,TDSSIB_EVENT_FAILED_OTHER, TRUE);   
            }
          }
        }
      }

      /* abandon the sib7 update info.. above processing in post_process should take care of resetting
         all flags, but in race conditions when event's are not yet initialized and we get this function
         call, better to reset these flags right here
       */
      tdssib7info.sib_ext_proc_update_req = FALSE;
      tdssib7info.sib_timer_update_req = FALSE;

      break;

    case TDSRRCSIB_ABORT_DUE_TO_OOS:
    case TDSRRCSIB_ABORT_DUE_TO_NGH_OOS:
    case TDSRRCSIB_ABORT_DUE_TO_TD2TD_BPLMN:
    case TDSRRCSIB_ABORT_DUE_TO_DL_WK_IND:
#if defined(FEATURE_TDSCDMA_GPS_CELL_ID_READING) 
    case TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH:
    case TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH_NO_PCCPCH_DROP:
#endif
      /* First clear the SIB events database */
      for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
      {
        if (tdssib_events_ptr->event[count].event_active == TRUE)
        {
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
          if((tdssib_events_ptr->event[count].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)&&
            (tdssib_events_ptr->event[count].req_proc == TDSRRC_PROCEDURE_GPS))
          {
            /*Some Special handling for cell id read in dch event. In some cases tear down 
             *of S-BCCH is taken care by SIB in that case tdsrrcsib_force_sib_event_init() is 
             *called by procedure with cause RRCSIB_ABORT_DUE_SIB_READ_IN_DCH. 
             *In other cases LLC will take care of tearing down of S-BCCH(Secnarios like 
             *W to G HO failure, on resuming on W LLC would simply want to kill sib event, 
             *as there is no S-BCCH)
             */
            if(TDSRRCSIB_ABORT_DUE_SIB_READ_IN_DCH == abort_cause)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Abort SIB read for cell id reading in DCH and DROP BCCH");
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,TRUE,TDSSIB_EVENT_FAILED_OTHER, TRUE);
            }
            else
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS: Abort SIB read for cell id reading in DCH and no BCCH Drop");
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,TRUE,TDSSIB_EVENT_FAILED_OTHER, FALSE);
            }
          }
          else
#endif

          {
            if(TDSRRCSIB_ABORT_DUE_TO_NGH_OOS != abort_cause)
            {
              if(TDSRRCSIB_ABORT_DUE_TO_DL_WK_IND == abort_cause)
              {
                tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,FALSE,TDSSIB_EVENT_FAILED_TIMEOUT, TRUE);
              }
              else
              {
              tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,FALSE,TDSSIB_EVENT_FAILED_OTHER, FALSE);
              }
            }
            else
            {
              if((tdsrrcllc_get_s_bcch_setup_status() == TRUE)||
                ((tdssib_events_ptr->event[TDSSIB_SRV_INDEX].for_serving_cell)&&
                (tdssib_events_ptr->srv_bcch.bcch_action == TDSSIB_BCCH_SETUP_PENDING))
                )
              {
                tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,FALSE,TDSSIB_EVENT_FAILED_OTHER, TRUE);
              }
              else
              {
                tdsrrcsib_post_process_failed_event((tdssib_events_index_e_type)count,FALSE,TDSSIB_EVENT_FAILED_OTHER, FALSE);
              }
            }
          }
        }
      }

      //call the generic sib_proc_Events_init just to reset all other SIB based events that need
      //to be re-iniitalized..  This is added as protection to take care of resetting flags when
      //an event is not generated.. because in post_process_failed_event all resetting is done 
      //only if an TDSEVENT is active
      tdsrrcsib_proc_events_init();
      
      //only if abort is for serving or due to oos, reset the active_cell_Status, for neighbor oos don't
      //reset serving cell status as we need a serving camped cell
      if(((TDSRRCSIB_ABORT_DUE_TO_OOS == abort_cause)||
        (TDSRRCSIB_ABORT_DUE_TO_DL_WK_IND == abort_cause)) && 
        (tdsrrc_get_state() != TDSRRC_STATE_CONNECTING))
      {
        tdsrrcsibdb_clear_active_cell_status();
      }
      break;
    default:
      break;
  }
}

/*===========================================================================

FUNCTION tdsrrcsib_check_band_mismatch_from_sib5_sib6

DESCRIPTION

  This function will be used to find if the band present in SIB5/SIB6
  is supported by the UE or not.
  
DEPENDENCIES

  None.

RETURN VALUE

  TRUE - If there is a band mismatch
  FALSE - If there is no band mismatch or the band info is not specified in SIB5/SIB6
  
SIDE EFFECTS

  None

===========================================================================*/
boolean tdsrrcsib_check_band_mismatch_from_sib5_sib6(uint32 index)
{
  /* Local variables */
  /*tdsrrc_state_e_type rrc_state;     */
  /*tdsrrc_SysInfoType6 *sib6_ptr = NULL; 
  tdsrrc_SysInfoType5 *sib5_ptr = NULL;*/
  tdsrrc_MasterInformationBlock *mib_ptr = NULL;
  boolean sib5_read = FALSE;
  boolean sib6_read = FALSE;

  uint32 dbindex = tdssib_events_ptr->event[index].sib_db_index;
  boolean for_serving_cell = tdssib_events_ptr->event[index].for_serving_cell;
  rrc_plmn_identity_type plmn;
  
  /* Initialize PLMN to dummy value to keep lint happy */
  plmn.mcc[0] = 0;
  plmn.mcc[1] = 0;
  plmn.mcc[2] = 0;
  plmn.num_mnc_digits = 3;
  plmn.mnc[0] = 0;
  plmn.mnc[1] = 0;
  plmn.mnc[2] = 0;

  /* Store local information that sib 5 and sib 6 are needed, received and decoded rather than use
     long loops everywhere */
  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_needed == TRUE) &&
     (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_received == TRUE) &&
     (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_decoded == TRUE))
  {
     sib5_read = TRUE;
  }

  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB6].sib_needed == TRUE) &&
     (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB6].sib_received == TRUE) &&
     (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB6].sib_decoded == TRUE))
  {
    sib6_read = TRUE;
  }

  if((FALSE == sib5_read) && (FALSE == sib6_read))
  {
    /* If neither SIB5 nor SIB6 are read,cant determine band mismatch,return FALSE */ 
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB5 & SIB6 not read yet");
    return FALSE;
  }

  /* Get the current RRC state */
  /*rrc_state = tdsrrc_get_state();*/

  mib_ptr = (tdsrrc_MasterInformationBlock *)tdsrrcsibdb_return_sib_for_db_index
                                     ((tdsrrc_SIB_type)tdsrrc_MIB,
                                       dbindex
                                     );

  if(mib_ptr != NULL)
  {
    /* Get PLMN */
    tdsrrc_translate_gsm_map_plmn_id(&(mib_ptr->plmn_Type.u.gsm_MAP->plmn_Identity),&plmn);
  }


  /* Connected mode handling.Check for SIB6.If freqBandInfo present,check for mismatch and return status */
/*
  if((rrc_state != TDSRRC_STATE_CONNECTING) && (rrc_state != TDSRRC_STATE_DISCONNECTED))
  {
    if(TRUE == sib6_read)
    {
      sib6_ptr = (tdsrrc_SysInfoType6 *)tdsrrcsibdb_return_sib_for_db_index
                                     ((tdsrrc_SIB_type)tdsrrc_SIB6,
                                       dbindex
                                     );
      
     
    }
  }
*/


  /* If we reach this point,it means that we could not use SIB6 to find the band mismatch.Try SIB5 */
  /* Get the SIB5 ptr from tdssibdb */
 /* sib5_ptr = (tdsrrc_SysInfoType5 *)tdsrrcsibdb_return_sib_for_db_index
                                             ((tdsrrc_SIB_type)tdsrrc_SIB5,
                                               dbindex
                                             ); */
  /* Check if frequencyBandIndicator IE is present */
  /* frequencyBandIndicator is FDD only */
  /* Check the FDD2 band field in SIB5 for mismatch */
  /* In tdd, fdd2_extension_present can not set TRUE */
  /* Frequency Band Indicator not present*/
  {
    tdsrrc_csp_band_class_type    band_class;
    boolean band_mismatch = FALSE;

    if(for_serving_cell)
    {
      band_class = tdsrrc_get_frequency_band(
                              tdsrrccsp_get_serving_frequency());
    }
    else
    {
      band_class = tdsrrc_get_frequency_band(
                              tdsrrccsp_get_neighbor_frequency());
    }

    switch(band_class)
    {
      case RRC_CSP_TDS_A_BAND:
      case RRC_CSP_TDS_E_BAND:
      case RRC_CSP_TDS_F_BAND:
      case RRC_CSP_TDS_F_LOWER_BAND:
      case RRC_CSP_TDS_F_UPPER_BAND:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"No band mismatch,Allow camping on band :0x%x",band_class);
        band_mismatch = FALSE;
        break;

      default:
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Band not supported,Don't allow camping on band :0x%x",band_class);
        band_mismatch = TRUE;
        break;
    }

    return band_mismatch;
  }

}

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
)
{
  void *ptr = NULL; /* Local ptr */

  /* We only store MIB,SIB1 and SIB3 for TD2TD BPLMN searches */
  switch(req_sib)
  {
    case tdsrrc_MIB:
      ptr = (void *)tdsrrcsib_bplmn_sibs.mib; 
      break;

    case tdsrrc_SIB1:
      ptr = (void *)tdsrrcsib_bplmn_sibs.sib1; 
      break;

    case tdsrrc_SIB3:
      ptr = (void *)tdsrrcsib_bplmn_sibs.sib3; 
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid SIB query for sib_type %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(req_sib));
      break;
  }

  return ptr;
}

/*===========================================================================

FUNCTION RRCSIB_FREE_BPLMN_SIBS

DESCRIPTION

  This function frees the memory used by a System Information
  Block stored in the BLOMN SIB database using the ASN.1 free pdu function.
    
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
)
{
  /* Check if the given pointer is not NULL */
  if( tdsrrcsib_bplmn_sibs.mib != NULL )
  {
    /* Call the free pdu function. It returns non-zero if an error occured */
    if(tdsrrcasn1_free_pdu(tdsrrc_MasterInformationBlock_PDU, tdsrrcsib_bplmn_sibs.mib ) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error occured in ASN.1 free pdu function");
    }
  }

  if( tdsrrcsib_bplmn_sibs.sib1 != NULL )
  {
    /* Call the free pdu function. It returns non-zero if an error occured */
    if(tdsrrcasn1_free_pdu( tdsrrc_SysInfoType1_PDU, tdsrrcsib_bplmn_sibs.sib1 ) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error occured in ASN.1 free pdu function");
    }
  }

  if( tdsrrcsib_bplmn_sibs.sib3 != NULL )
  {
    /* Call the free pdu function. It returns non-zero if an error occured */
    if(tdsrrcasn1_free_pdu( tdsrrc_SysInfoType3_PDU, tdsrrcsib_bplmn_sibs.sib3 ) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error occured in ASN.1 free pdu function");
    }
  }

#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
  if( tdsrrcsib_bplmn_sibs.sb1!= NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-Free SB1 from BPLMN SIB db");
    /* Call the free pdu function. It returns non-zero if an error occured */
    if(tdsrrcasn1_free_pdu( tdsrrc_SysInfoTypeSB1_PDU, tdsrrcsib_bplmn_sibs.sb1) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error occured in ASN.1 free pdu function");
    }
  }
  if( tdsrrcsib_bplmn_sibs.sb2 != NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BTD:-Free SB2 from BPLMN SIB db");
    /* Call the free pdu function. It returns non-zero if an error occured */
    if(tdsrrcasn1_free_pdu( tdsrrc_SysInfoTypeSB2_PDU, tdsrrcsib_bplmn_sibs.sb2) )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Error occured in ASN.1 free pdu function");
    }
  }
#endif
  /* Initialise the SIBs */
  tdsrrcsib_bplmn_sibs.mib = NULL;
  tdsrrcsib_bplmn_sibs.sib1 = NULL;
  tdsrrcsib_bplmn_sibs.sib3 = NULL;
#ifdef FEATURE_TDSRRC_BAND_TYPE_DETECTION_FOR_BPLMN
  tdsrrcsib_bplmn_sibs.sb1 = NULL;
  tdsrrcsib_bplmn_sibs.sb2 = NULL;
#endif
}

#if  defined(FEATURE_TDSCDMA_DRX_MANAGER)
/*===========================================================================

FUNCTION RRCSIB_INITIALIZE_SIB_SLEEP_SCHEDULE_TABLE

DESCRIPTION

   This function initializes the SIB schedule table  
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_initialize_sib_sleep_schedule_table
(
  void
)
{
  /* Initialise to all 1s so that L1 will stay awake initially */
  memset(tdssib_sleep_schedule_table_0,0xFF,TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);
  memset(tdssib_sleep_schedule_table_1,0xFF,TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);
  tdssib_sleep_schedule_table_in_use = 0;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_Sleep: Initialized SIB Sleep Schedule Tables to all 1s");
}

/*===========================================================================

FUNCTION RRCSIB_UPDATE_SIB_SLEEP_SCHEDULE_TABLE

DESCRIPTION

  This function updates the SIB schedule table  
  
DEPENDENCIES
 
  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.
     
===========================================================================*/
void tdsrrcsib_update_sib_sleep_schedule_table
(
  tdssib_events_index_e_type event_index   /* Index of the current event */
)
{
  uint32 count = 0;
  uint32 seg_count = 0;
  uint32 loop_count = 0;
  uint32 memcpy_count = 0;
  uint32 bytes_to_copy =0;
  uint8 * table_ptr = NULL;
  uint8 work_table;
  tdsrrc_cmd_type * cmd_ptr = NULL;
  uint32 rep_count;
  uint16 num_bits_per_rep = 0;
  uint32 density = 0;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }


  TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_Sleep:Updating SIB Sleep Schedule Table for SIB");

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Previous table in use:%d",tdssib_sleep_schedule_table_in_use);

  /* Check if there is enough wait time to update the schedule.
   * This is to avoid RRC too frequent buffer update that can result 
   * data corruption between RRC and L1
   */

  if (tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER not yet expired. Skip schedule update");
    return;
  }

  if(tdssib_events_ptr->scheduling_max_rept_rate == 0)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"tdssib_events_ptr->scheduling_max_rept_rate is 0");
    return;
  }
  if(tdssib_sleep_schedule_table_in_use == 0)
  {
    table_ptr = tdssib_sleep_schedule_table_1;
    work_table = 1;
  }
  else
  {
    table_ptr = tdssib_sleep_schedule_table_0;
    work_table = 0;
  }

  memset(table_ptr,0x00,TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);   

  /* Loop through the all the SIB(segments) and update the sleep schedule table based on 
     the SIB Schedule */
  for(count = 1; count < TDSRRC_TOT_SIBS; count++)
  {
    /* Include the schedule only if the SIB is needed & the SIB has a valid schedule 
	     and if it has not been decoded already */
    if((tdssib_events_ptr->event[event_index].tsib[count].sib_needed == TRUE) &&
       (tdssib_events_ptr->event[event_index].tsib[count].sib_received == FALSE)&&
       (tdssib_events_ptr->sib_sched[count].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
      )
    {
      /* Calculate the rep count to be filled in the loop */
      rep_count = tdssib_events_ptr->scheduling_max_rept_rate/tdssib_events_ptr->sib_sched[count].repitition_rate;
      
      for (seg_count = 0; seg_count < tdssib_events_ptr->sib_sched[count].num_segments; seg_count++)
      {
        uint32 sfn = tdssib_events_ptr->sib_sched[count].sfn_offset[seg_count]; /* Local copy of SFN */
        uint32 byte_pos = sfn>>3;                /* (sfn/8) Gives the byte location in sib sleep schedule table */
        uint8 byte_offset = (uint8)(sfn & 0x07); /* (sfn%8)Gives the offset within the byte in sib sleep table */
        uint8 bit_mask = 0xFF;
		
        bit_mask = tdsrrcsib_gen_bit_mask_for_schedule_table(byte_offset);
        /* Take into account the repetition rate and set the repetition masks also.Do it only till max rep rate
            since we would do a memcpy for rest of the table.Apply the calculated bitmask for the 
            byte till the max repetition cycle */
        for(loop_count = 0; loop_count < rep_count; loop_count++)
        {
          uint32 rep_byte_pos = byte_pos + (loop_count * (tdssib_events_ptr->sib_sched[count].repitition_rate/8));
          table_ptr[rep_byte_pos] |= bit_mask;                       

          /* Optimization to set a few extra 1s after last segment of Scheduling blocks
             Add 4 1s if this segment in the last segment of SB1 or SB2. this is to keep 
             L1 awake after the last SB segment. SB can have schedule for some SIBs and L1
             needs to stay awake to process the new schedule from SB    */
          if(((count == (uint32)tdsrrc_SB1) ||(count == (uint32)tdsrrc_SB2)) &&
              (seg_count == (uint32)(tdssib_events_ptr->sib_sched[count].num_segments-1)))
          {
            uint8 curr_pos_bit_mask = 0x00;
            uint8 next_pos_bit_mask = 0x00;
            switch(bit_mask)
            {
              case 0xC0:
                /* Actual bitmask : 1100 0000. 
                   Modified bitmask with 4 dummy ones: 1111 1100 (0xFC) */
                curr_pos_bit_mask = 0xFC; 
                break;

              case 0x30:
                /* Actual bitmask : 0011 0000. 
                   Modified bitmask with 4 dummy ones: 0011 1111 (0x3F) */
                curr_pos_bit_mask = 0x3F; 
                break;

              case 0x0C:
                /* Actual bitmask : 0000 1100. 
                   Modified bitmask with 4 dummy ones: 0000 1111 1100 0000(0x0F,0xC0) */
                curr_pos_bit_mask = 0x0F; 
                next_pos_bit_mask = 0xC0;
                break;

              case 0x03:
                /* Actual bitmask : 0000 0011. 
                   Modified bitmask with 4 dummy ones: 0000 0011 1111 0000(0x03,0xF0) */
                next_pos_bit_mask = 0xF0;
                break;

              default:
                /* Actual bitmask : 1111 1111. 
                   Modified bitmask with 4 dummy ones: 1111 1111 1111 0000(0xFF,0xF0) */
                next_pos_bit_mask = 0xF0;
                break;
            }

            /* Modify the current pos bitmask */
            table_ptr[rep_byte_pos] |= curr_pos_bit_mask;               

            /* Set the next byte. Make sure you are not crossing the array boundary */
            if(rep_byte_pos < (TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE -1))  
            {
              /* Set the 4 frames(40ms) in the next byte to all 1s so that L1 will stay awake after SB */
              table_ptr[rep_byte_pos+1] |= next_pos_bit_mask;
            }
            else //wraparound case
            {
              table_ptr[0] |= next_pos_bit_mask;
            }
          }  
        }
      }
    }
  }

  /* Now we have filled the table for max repetition rate bytes.Fill the rest of the table by memcpy */
  //memcpy_count = RRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE/tdssib_events_ptr->scheduling_max_rept_rate;
  memcpy_count = 4096/(tdssib_events_ptr->scheduling_max_rept_rate);
  bytes_to_copy = tdssib_events_ptr->scheduling_max_rept_rate/8;

  for(count = 1; count < memcpy_count ; count++)
  {
    uint32 index = count * bytes_to_copy;
    memscpy(table_ptr+index,bytes_to_copy,table_ptr,bytes_to_copy);
  }

  if((tdsrrcsib_g_in_data_call)&&(!tdsrrcsib_stop_partition))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Introduce holes %d",tdssib_events_ptr->event[event_index].int_event.int_event_type);

    if(!((tdssib_events_ptr->event[event_index].req_proc == TDSRRC_PROCEDURE_SIB) &&
       (tdssib_events_ptr->event[event_index].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)))
    {
      /* Backup the table for revert */
      memscpy(tdssib_sleep_schedule_table_backup,TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE * sizeof(uint8),
              table_ptr, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE * sizeof(uint8));

      tdsrrcsib_intro_holes_in_table(table_ptr, TDSRRCSIB_SLEEP_SFN, TDSRRCSIB_WAKE_SFN);

      /* If the table is all-zero after introduce holes, don't introduce holes to L1 as L1 will crash if so */
      for(count = 0; count < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE; count++)
      {
        if (table_ptr[count] != 0)
        {
          break;
        }
      }

      if(count == TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"G2T Tune Away: Empty table after introducing holes,don't introduce any hole");
        memscpy(table_ptr, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE * sizeof(uint8),
                tdssib_sleep_schedule_table_backup, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE * sizeof(uint8));
      }
    }
  }

  /* If the table is all-zero after introduce holes, don't introduce holes to L1 as L1 will crash if so */
  for(count = 0; count < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE; count++)
  {
    if (table_ptr[count] != 0)
    {
      break;
    }
  }
  if(count == TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Empty table happens,don't need to send TDSL1_CPHY_SIB_SCHED_CHANGE_IND");
    return;
  }
  
  /* Now we have the sched table updated.Send the change_ind to L1 */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * rrc_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_cmd_l1_req);
 
    cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_SIB;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_SIB_SCHED_CHANGE_IND;

    rrc_cmd_l1_req->sib_sched_change_ind_req.sib_sfn_bit_mask = table_ptr;

    rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = FALSE;
    if (tdssib_events_ptr->event[event_index].for_serving_cell == TRUE)
    {
      if(TRUE == tdsrrccsp_do_opt_for_con_to_idle())
      {
        rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = TRUE;
        TDSRRC_MSG3(MSG_LEGACY_HIGH, "Cell sel:In DDS from Connected to IDLE, Hold lock",0,0,0);
      }
    }
    else if (TRUE == tdsrrccsp_is_other_sub_in_dds())
    {
      rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = TRUE;
      TDSRRC_MSG3(MSG_LEGACY_HIGH, "Cell resel:In DDS, Hold lock",0,0,0);
    }
    tdsrrc_put_int_cmd( cmd_ptr );
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_Sleep:TDSL1_CPHY_SIB_SCHED_CHANGE_IND sent to L1");

    /* Start the update wait timer to avoid frequent schedule update */
    tdsrrctmr_start_timer(TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER, 
                          TDSRRCSIB_SIB_SCHEDULE_UPDATE_WAIT_TIME_MS);
  }
  else
  {
    ERR_FATAL("SIB_Sleep:Cound not get memory for TDSL1_CPHY_SIB_SCHED_CHANGE_IND",0,0,0);
  }

  tdssib_sleep_schedule_table_in_use = work_table;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Current table in use:%d",tdssib_sleep_schedule_table_in_use);

  for(count = 0; count < (uint32)(tdssib_events_ptr->scheduling_max_rept_rate/8);count++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB_Sleep: Table Contents byte[%d] = %0X",count,table_ptr[count]);
    
    num_bits_per_rep += TDSRRCSIB_BYTE_BITS(table_ptr[count]);
  }

  /* Debug print */
  density = (num_bits_per_rep * 100) / tdssib_events_ptr->scheduling_max_rept_rate;
  TDSRRC_MSG4(MSG_LEGACY_HIGH,"SIB_Sleep table after update by SIB: Max repetition rate is %d, num_bits_per_rep %d, density %d pc",
            tdssib_events_ptr->scheduling_max_rept_rate,
            num_bits_per_rep,
            density,
			0);

}

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
)
{
  uint8 * table_ptr = NULL;
  uint8 work_table;
  uint32 count;
  tdsrrc_cmd_type * cmd_ptr = NULL;

  if(TDSEVENT_INDEX_VALID(event_index) == FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending empty table %d",is_empty);

  if(tdssib_sleep_schedule_table_in_use == 0)
  {
    table_ptr = tdssib_sleep_schedule_table_1;
    work_table = 1;
  }
  else
  {
    table_ptr = tdssib_sleep_schedule_table_0;
    work_table = 0;
  }

  if(is_empty)
  {
    /*If schedule info exists, insert 100ms empty to schdule table according to current sfn*/
    if(((!tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB1])||
        ((tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB1])&&
        (tdssib_events_ptr->event[event_index].tempholder.sb1_decoded)))&&
          ((!tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB2])||
           ((tdssib_events_ptr->event[event_index].sched_present_for_sibs[tdsrrc_SB2])&&
           (tdssib_events_ptr->event[event_index].tempholder.sb2_decoded))
           ))
    {
      uint32 seg_count = 0;
      uint32 rep_count;
      uint32 loop_count = 0;
      uint32 memcpy_count = 0;
      uint32 bytes_to_copy =0;

      memset(table_ptr, 0x00, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);

      /* Loop through the all the SIB(segments) and update the sleep schedule table based on 
           the SIB Schedule */
      for(count = 1; count < TDSRRC_TOT_SIBS; count++)
      {
        /* Include the schedule only if the SIB is needed & the SIB has a valid schedule 
            and if it has not been decoded already */
        if((tdssib_events_ptr->event[event_index].tsib[count].sib_needed == TRUE) &&
           ((tdssib_events_ptr->event[event_index].tsib[count].sib_received == FALSE)||
           (count == (uint32)tdsrrc_SIB7))&&
           (tdssib_events_ptr->sib_sched[count].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
        )
        {
          /* Calculate the rep count to be filled in the loop */
          rep_count = tdssib_events_ptr->scheduling_max_rept_rate/tdssib_events_ptr->sib_sched[count].repitition_rate;
      
          for (seg_count = 0; seg_count < tdssib_events_ptr->sib_sched[count].num_segments; seg_count++)
          {
            uint32 sfn = tdssib_events_ptr->sib_sched[count].sfn_offset[seg_count]; /* Local copy of SFN */
            uint32 byte_pos = sfn>>3;                /* (sfn/8) Gives the byte location in sib sleep schedule table */
            uint8 byte_offset = (uint8)(sfn & 0x07); /* (sfn%8)Gives the offset within the byte in sib sleep table */
            uint8 bit_mask = 0xFF;
		
            bit_mask = tdsrrcsib_gen_bit_mask_for_schedule_table(byte_offset);
            /* Take into account the repetition rate and set the repetition masks also.Do it only till max rep rate
                  since we would do a memcpy for rest of the table.Apply the calculated bitmask for the 
                  byte till the max repetition cycle */
           for(loop_count = 0; loop_count < rep_count; loop_count++)
           {
             uint32 rep_byte_pos = byte_pos + (loop_count * (tdssib_events_ptr->sib_sched[count].repitition_rate/8));
             table_ptr[rep_byte_pos] |= bit_mask;                       

             /* Optimization to set a few extra 1s after last segment of Scheduling blocks
                    Add 4 1s if this segment in the last segment of SB1 or SB2. this is to keep 
                    L1 awake after the last SB segment. SB can have schedule for some SIBs and L1
                    needs to stay awake to process the new schedule from SB    */
             if(((count == (uint32)tdsrrc_SB1) ||(count == (uint32)tdsrrc_SB2)) &&
                 (seg_count == (uint32)(tdssib_events_ptr->sib_sched[count].num_segments-1)))
             {
               uint8 curr_pos_bit_mask = 0x00;
               uint8 next_pos_bit_mask = 0x00;
               switch(bit_mask)
               {
                 case 0xC0:
                   /* Actual bitmask : 1100 0000. 
                             Modified bitmask with 4 dummy ones: 1111 1100 (0xFC) */
                   curr_pos_bit_mask = 0xFC; 
                   break;

                 case 0x30:
                   /* Actual bitmask : 0011 0000. 
                             Modified bitmask with 4 dummy ones: 0011 1111 (0x3F) */
                   curr_pos_bit_mask = 0x3F; 
                   break;

                 case 0x0C:
                   /* Actual bitmask : 0000 1100. 
                            Modified bitmask with 4 dummy ones: 0000 1111 1100 0000(0x0F,0xC0) */
                   curr_pos_bit_mask = 0x0F; 
                   next_pos_bit_mask = 0xC0;
                   break;

                 case 0x03:
                   /* Actual bitmask : 0000 0011. 
                             Modified bitmask with 4 dummy ones: 0000 0011 1111 0000(0x03,0xF0) */
                   next_pos_bit_mask = 0xF0;
                   break;

                default:
                   /* Actual bitmask : 1111 1111. 
                             Modified bitmask with 4 dummy ones: 1111 1111 1111 0000(0xFF,0xF0) */
                   next_pos_bit_mask = 0xF0;
                   break;
               }

               /* Modify the current pos bitmask */
               table_ptr[rep_byte_pos] |= curr_pos_bit_mask;               

               /* Set the next byte. Make sure you are not crossing the array boundary */
               if(rep_byte_pos < (TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE -1))  
               {
                 /* Set the 4 frames(40ms) in the next byte to all 1s so that L1 will stay awake after SB */
                 table_ptr[rep_byte_pos+1] |= next_pos_bit_mask;
               }
               else //wraparound case
               {
                 table_ptr[0] |= next_pos_bit_mask;
               }
             }  
            }
          }
        }
      }

       /* Now we have filled the table for max repetition rate bytes.Fill the rest of the table by memcpy */
      memcpy_count = 4096/(tdssib_events_ptr->scheduling_max_rept_rate);
      bytes_to_copy = tdssib_events_ptr->scheduling_max_rept_rate/8;

      for(count = 1; count < memcpy_count ; count++)
      {
        uint32 index = count * bytes_to_copy;
        memscpy(table_ptr+index,bytes_to_copy,table_ptr,bytes_to_copy);
      }
    }
    else
    {
      /*If schedule info doesn't exist, insert 100ms empty 0xFF table according to current sfn*/
      memset(table_ptr, 0xFF, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);
    }

    /*Always introduce hole if G PS is active*/  
    if((tdsrrcsib_g_in_data_call)&&(!tdsrrcsib_stop_partition))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Introduce holes %d",tdssib_events_ptr->event[event_index].int_event.int_event_type);
		
      if(!((tdssib_events_ptr->event[event_index].req_proc == TDSRRC_PROCEDURE_SIB) &&
      (tdssib_events_ptr->event[event_index].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)))
      {
        /* Backup the table before introducing holes for revert */
        memscpy(tdssib_sleep_schedule_table_backup,TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE * sizeof(uint8),
                table_ptr, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE * sizeof(uint8));
        
        tdsrrcsib_intro_holes_in_table(table_ptr, TDSRRCSIB_SLEEP_SFN, TDSRRCSIB_WAKE_SFN);

        /* If the table is all-zero after introduce holes, don't introduce holes to L1 as L1 will crash if so */
        for(count = 0; count < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE; count++)
        {
          if (table_ptr[count] != 0)
          {
            break;
          }
        }
        if(count == TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"G2T Tune Away: Empty table after introducing holes,don't introduce hole");
          memscpy(table_ptr, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE * sizeof(uint8),
                  tdssib_sleep_schedule_table_backup, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE * sizeof(uint8));
        }
      }
    }
    
    /*Then intert 100ms empty cfn to this table*/  
    if(tdsrrcrce_is_cs_call_pending() == FALSE)
    {
      uint32 current_sfn;
      uint32 old_byte_pos;
      uint32 byte_pos;
      uint32 last_byte_pos;
      uint8 byte_offset;
      uint8 bit_mask = 0xFF;

      current_sfn = tdsseq_get_sfn();
      current_sfn += current_sfn%2;
      old_byte_pos = byte_pos = (current_sfn>>3)&TDSRRCSIB_TABLE_MOD_MASK; /* (sfn/8) Gives the byte location in sib sleep schedule table */
      last_byte_pos = ((current_sfn + (TDSRRCSIB_EMPTY_FRAMES/10)) >>3)&TDSRRCSIB_TABLE_MOD_MASK;
      byte_offset = (uint8)(current_sfn & 0x07); /* (sfn%8)Gives the offset within the byte in sib sleep table */

      for(count = old_byte_pos; 
          TDSRRCSIB_TABLE_INX_GT((old_byte_pos + 4), count, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);
          (count = TDSRRCSIB_TABLE_INX_INC(count)))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH, "SIB_Sleep: Current_sfn %d, Table Contents before empty mask byte[%d] = %0X", current_sfn, count,table_ptr[count]);
      }

		  /*Set the bitmask of the first bytes starting from current_sfn*/  
      bit_mask = tdsrrcsib_gen_empty_bit_mask_for_schedule_table(byte_offset);
      table_ptr[byte_pos] &= bit_mask;  

      /*Set the bitmask from the next bytes of current_sfn till the end of emtpy frames */  
      for(count = TDSRRCSIB_TABLE_INX_INC(byte_pos); 
          TDSRRCSIB_TABLE_INX_GT(last_byte_pos, count, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);
          (count = TDSRRCSIB_TABLE_INX_INC(count)))
      {
        table_ptr[count] &= 0x00;  
      }
      TDSRRC_MSG3(MSG_LEGACY_HIGH, "SIB_Sleep:First byes at byte_pos %d, bit_mask 0x%X, count %d", byte_pos,bit_mask, count);

			/*Set the last bytes which ends with current_sfn + emtpy frames */
      current_sfn = current_sfn + TDSRRCSIB_EMPTY_FRAMES/10;
      byte_pos = (current_sfn>>3)&TDSRRCSIB_TABLE_MOD_MASK;/* (sfn/8) Gives the byte location in sib sleep schedule table */
      byte_offset = (uint8)(current_sfn & 0x07); /* (sfn%8)Gives the offset within the byte in sib sleep table */
      bit_mask = tdsrrcsib_gen_empty_bit_mask_for_schedule_table(byte_offset);
      table_ptr[byte_pos] &= ~bit_mask; 
      TDSRRC_MSG2(MSG_LEGACY_HIGH, "SIB_Sleep:Last byes at byte_pos %d,bit_mask 0x%X",byte_pos,bit_mask);

      for(count = old_byte_pos; 
          TDSRRCSIB_TABLE_INX_GT((old_byte_pos + 4), count, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);
          (count = TDSRRCSIB_TABLE_INX_INC(count)))
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB_Sleep: Table Contents after empty mask byte[%d] = %0X",count,table_ptr[count]);
      }
    }
    else
    { 
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_G2T: Doesn't introduce empty table as CS call is pending");
    }
  }
  else
  {
    memset(table_ptr, 0xFF, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);
    if(tdsrrcsib_g_in_data_call)
    {
      tdsrrcsib_intro_holes_in_table(table_ptr, TDSRRCSIB_SLEEP_SFN, TDSRRCSIB_WAKE_SFN);
    }
  }
  
  /* trace SFN from 0 to 2560 */
  TDSRRC_MSG8(MSG_LEGACY_HIGH,"SIB_BM[0-255]: %08X %08X %08X %08X %08X %08X %08X %08X",
              TDSRRCSIB_COMBIN_SFN_BM(table_ptr, 0),
              TDSRRCSIB_COMBIN_SFN_BM(table_ptr, 4),
              TDSRRCSIB_COMBIN_SFN_BM(table_ptr, 8),
              TDSRRCSIB_COMBIN_SFN_BM(table_ptr, 12),
              TDSRRCSIB_COMBIN_SFN_BM(table_ptr, 16),
              TDSRRCSIB_COMBIN_SFN_BM(table_ptr, 20),
              TDSRRCSIB_COMBIN_SFN_BM(table_ptr, 24),
              TDSRRCSIB_COMBIN_SFN_BM(table_ptr, 28)
             );

  /* Now we have the sched table updated.Send the change_ind to L1 */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * rrc_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_cmd_l1_req);
 
    cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_SIB;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_SIB_SCHED_CHANGE_IND;

    rrc_cmd_l1_req->sib_sched_change_ind_req.sib_sfn_bit_mask = table_ptr;
    rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = FALSE;
    tdsrrc_put_int_cmd( cmd_ptr );
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_Sleep:TDSL1_CPHY_SIB_SCHED_CHANGE_IND sent to L1");

    /* Start the update wait timer to avoid frequent schedule update */
    tdsrrctmr_start_timer(TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER, 
                          TDSRRCSIB_SIB_SCHEDULE_UPDATE_WAIT_TIME_MS);
  }
  else
  {
    ERR_FATAL("SIB_Sleep:Cound not get memory for TDSL1_CPHY_SIB_SCHED_CHANGE_IND",0,0,0);
  }

  tdssib_sleep_schedule_table_in_use = work_table;

}

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
void tdsrrcsib_send_dummy_sleep_schedule_table(void)
{
  uint8 * table_ptr = NULL;
  uint8 work_table;
  uint32 count;
  tdsl1_ext_cmd_type * l1_cmd_ptr = NULL;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep: Send dummy table to enforce L1 sleep activity,current work_table: %d",tdssib_sleep_schedule_table_in_use);

  if(tdssib_sleep_schedule_table_in_use == 0)
  {
    table_ptr = tdssib_sleep_schedule_table_1;
    work_table = 1;
  }
  else
  {
    table_ptr = tdssib_sleep_schedule_table_0;
    work_table = 0;
  }

  for (count = 0; count < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE; count++)
  {
    table_ptr[count] = (count & 0x1) ? 0 : 0xF; /* 0x0F, 0x00, 0x0F, 0x00... */
  }

  for(count = 0; count < 4; count++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB_Sleep: Table Contents byte[%d] = %0X",count,table_ptr[count]);
  }

  /* Now we have the sched table updated.Send the change_ind to L1. Instead of sending via LLC, this cmd needs to be put in
        TL1 cmd queue directly. This is because RRC will reacquire lock immediately after sending this cmd. The lock request blocks
        LLC from sending cmd to TL1. TL1 may still be unable to sleep and perform dev swap. Note that when SIB_CNF is sent, SIB
        has removed pending SIB_SCHED_CHANGE_IND from int cmd queue. So the 10ms SIB schedule update time is not violated */
  if( (l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
  {
    ERR_FATAL("RRC Unable to get L1 CMD buf for TDSL1_CPHY_SIB_SCHED_CHANGE_IND",0,0,0);
  }
  l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_SIB_SCHED_CHANGE_IND;
  l1_cmd_ptr->cmd.sib_sched_change_ind.sib_sfn_bit_mask = table_ptr;
  l1_cmd_ptr->cmd.sib_sched_change_ind.hold_trm_lock_for_sib_gap = FALSE;
  tdsl1_put_cmd(l1_cmd_ptr);
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:TDSL1_CPHY_SIB_SCHED_CHANGE_IND sent to L1 with work_table: %d",work_table);

  tdssib_sleep_schedule_table_in_use = work_table;
}

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
)
{
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_Sleep:Returning Table location for CPHY-SETUP");
  tdsrrcsib_initialize_sib_sleep_schedule_table();
  return tdssib_sleep_schedule_table_0;
}

void tdsrrcsib_process_sib_sleep_before_sb
(
   uint16 mib_sfn_pri,
   uint16 sb_rep_ttis,
   uint32 sb_offset,
   tdssib_events_index_e_type event_index 
)
{
   uint16 rep_len_ttis = tdsrrcsib_sib_sleep.repetition_len / 2;
   uint16 sib_end_tti = tdsrrcsib_sib_sleep.sib_end_frame / 2;
   uint16 sfn_pri_offset = mib_sfn_pri % rep_len_ttis;

   TDSRRC_MSG6(MSG_LEGACY_HIGH,"Enable SIB sleep before SB: mib_sfn_pri %d, sfn_pri_offset(over %d) %d, sib_end_tti %d, sb1_offset %d, sb1_rep_tti %d", 
              mib_sfn_pri,
              rep_len_ttis,
              sfn_pri_offset,
              sib_end_tti,
              sb_offset,
              sb_rep_ttis);

   if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
   {
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"tdsrrcsib_get_sib_sleep_schedule_table_location: Invalid SIB Event Index %d",event_index);
      return;
   }

   if ((sb_rep_ttis == rep_len_ttis)
      && (mib_sfn_pri != TDSRRCSIB_INVLID_SFN_PRIME)
      && (mib_sfn_pri < TDSRRCSIB_MAX_SFN_PRIME))
   {
     if ((sib_end_tti && (sfn_pri_offset > sib_end_tti) && (sfn_pri_offset > sb_offset))
         || ((sib_end_tti == 0) && sfn_pri_offset))
     {
        tdsrrc_cmd_type * cmd_ptr = NULL;
        uint8 * table_ptr = NULL;
        uint8 work_table;
        uint16 sb_rep_tti_bytes = sb_rep_ttis / 4;
        uint16 start_frame_byte = (mib_sfn_pri - sfn_pri_offset) / 4;
        uint16 end_frame_byte = start_frame_byte + sb_rep_tti_bytes;
        
        uint32 count = 0;
        uint32 seg_count = 0;
        uint32 loop_count = 0;
        uint32 memcpy_count = 0;
        uint32 bytes_to_copy =0;
        uint32 rep_count;
        uint16 num_bits_per_rep = 0;
        uint32 density = 0;
        
        /* SB1 schedule has passed. Send special bit_mask pattern to allow L1 sleep */
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Send L1 special bitmask. sb_rep_tti_bytes: %d, start_frame_byte %d, end_frame_byte %d", 
                                    sb_rep_tti_bytes,
                                    start_frame_byte,
                                    end_frame_byte);

        if(tdssib_sleep_schedule_table_in_use == 0)
        {
          table_ptr = tdssib_sleep_schedule_table_1;
          work_table = 1;
        }
        else
        {
          table_ptr = tdssib_sleep_schedule_table_0;
          work_table = 0;
        }

        /* Need to set most bits to zero to avoid confusing DL */
        memset(table_ptr, 0x00, TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);   

        /* Loop through the all the SIB(segments) and update the sleep schedule table based on 
           the SIB Schedule */
        for(count = 1; count < TDSRRC_TOT_SIBS; count++)
        {
            /* Include the schedule only if the SIB is needed & the SIB has a valid schedule 
               and if it has not been decoded already */
           if((tdssib_events_ptr->event[event_index].tsib[count].sib_needed == TRUE) 
              && ((tdssib_events_ptr->event[event_index].tsib[count].sib_decoded == FALSE) || (count == (uint32)tdsrrc_SIB7))
              && (tdssib_events_ptr->sib_sched[count].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
             )
           {
              /* Calculate the rep count to be filled in the loop */
              rep_count = tdssib_events_ptr->scheduling_max_rept_rate/tdssib_events_ptr->sib_sched[count].repitition_rate;
      
              for (seg_count = 0; seg_count < tdssib_events_ptr->sib_sched[count].num_segments; seg_count++)
              {
                 uint32 sfn = tdssib_events_ptr->sib_sched[count].sfn_offset[seg_count]; /* Local copy of SFN */
                 uint32 byte_pos = sfn>>3;                /* (sfn/8) Gives the byte location in sib sleep schedule table */
                 uint8 byte_offset = (uint8)(sfn & 0x07); /* (sfn%8)Gives the offset within the byte in sib sleep table */
                 uint8 bit_mask = 0xFF;

                 bit_mask = tdsrrcsib_gen_bit_mask_for_schedule_table(byte_offset);
                 /* Take into account the repetition rate and set the repetition masks also.Do it only till max rep rate
                    since we would do a memcpy for rest of the table.Apply the calculated bitmask for the 
                    byte till the max repetition cycle */
                 for(loop_count = 0; loop_count < rep_count; loop_count++)
                 {
                    uint32 rep_byte_pos = byte_pos + (loop_count * (tdssib_events_ptr->sib_sched[count].repitition_rate/8));
                    table_ptr[rep_byte_pos] |= bit_mask;                       

                    /* Don't do the optimizastion here,
                       Optimization to set a few extra 1s after last segment of Scheduling blocks
                       Add 4 1s if this segment in the last segment of SB1 or SB2. this is to keep 
                       L1 awake after the last SB segment. SB can have schedule for some SIBs and L1
                       needs to stay awake to process the new schedule from SB    */
                    if(((count == (uint32)tdsrrc_SB1) ||(count == (uint32)tdsrrc_SB2)) &&
                        (seg_count == (uint32)(tdssib_events_ptr->sib_sched[count].num_segments-1)))
                    {
                       /* Modify the current pos bitmask */
                       table_ptr[rep_byte_pos] |= bit_mask;               
                    }  
                 }
              }
           }
        }

        /* Now we have filled the table for max repetition rate bytes.Fill the rest of the table by memcpy */
        memcpy_count = 4096/(tdssib_events_ptr->scheduling_max_rept_rate);
        bytes_to_copy = tdssib_events_ptr->scheduling_max_rept_rate/8;

        for(count = 1; count < memcpy_count ; count++)
        {
           uint32 index = count * bytes_to_copy;
           memscpy(table_ptr+index, bytes_to_copy, table_ptr, bytes_to_copy);
        }
        
        /* Zero out current block from start till end */
        memset(table_ptr + (uint32) start_frame_byte, 0x00, sb_rep_tti_bytes);
        
        /* Now we have the sched table updated.Send the change_ind to L1 */
        if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
        {
          tdsl1_req_cmd * rrc_cmd_l1_req = NULL;
          tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_cmd_l1_req);
       
          cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
          cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_SIB;
          cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
          cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_SIB_SCHED_CHANGE_IND;

          rrc_cmd_l1_req->sib_sched_change_ind_req.sib_sfn_bit_mask = table_ptr;
          rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = FALSE;
          if (tdssib_events_ptr->event[event_index].for_serving_cell == TRUE)
          {
            if(TRUE == tdsrrccsp_do_opt_for_con_to_idle())
            {
              rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = TRUE;
              TDSRRC_MSG3(MSG_LEGACY_HIGH, "Cell sel:In DDS from Connected to IDLE, Hold lock",0,0,0);
            }
          }
          else if (TRUE == tdsrrccsp_is_other_sub_in_dds())
          {
            rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = TRUE;
            TDSRRC_MSG3(MSG_LEGACY_HIGH, "Cell resel:In DDS, Hold lock",0,0,0);
          }
          tdsrrc_put_int_cmd( cmd_ptr );
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB_SB_Sleep:TDSL1_CPHY_SIB_SCHED_CHANGE_IND sent to L1");
        }
        else
        {
          ERR_FATAL("MIB_SB_Sleep:Cound not get memory for TDSL1_CPHY_SIB_SCHED_CHANGE_IND",0,0,0);
        }

        tdssib_sleep_schedule_table_in_use = work_table;

        /* Debug print */
        {
           uint16 i;

           if (end_frame_byte == TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE)
           {
              for(i = 0; i < sb_rep_tti_bytes; i++)
              {
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"MIB_SB_Sleep: Table Contents byte[%d] = 0x%X",i,table_ptr[i]);

                num_bits_per_rep += TDSRRCSIB_BYTE_BITS(table_ptr[i]);
              }
           }
           else
           {
              for(i = end_frame_byte; (i < end_frame_byte + sb_rep_tti_bytes) && (i < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE); i++)
              {
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"MIB_SB_Sleep: Table Contents byte[%d] = 0x%X",i,table_ptr[i]);

                num_bits_per_rep += TDSRRCSIB_BYTE_BITS(table_ptr[i]);
              }
           }
        }

        density = (num_bits_per_rep * 100) / tdsrrcsib_sib_sleep.repetition_len;
        TDSRRC_MSG5(MSG_LEGACY_HIGH,"MIB_SB_Sleep:Current table in use:%d, sb_rep %d, num_bits_per_rep %d, density %d pc, max_rept %d",
                    tdssib_sleep_schedule_table_in_use,
                    tdsrrcsib_sib_sleep.repetition_len,
                    num_bits_per_rep,
                    density,
                    tdssib_events_ptr->scheduling_max_rept_rate);
     }
     else
     {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"MIB_SB_Sleep: not satisfy sleep condition");
     }
   }
   else
   {
     TDSRRC_MSG2(MSG_LEGACY_HIGH,"MIB_SB_Sleep: mismatched parameters sb_rep_ttis %d,nv_ttis %d",sb_rep_ttis,rep_len_ttis);
   }
}

#endif /* defined(FEATURE_TDSCDMA_DRX_MANAGER) */

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
)
{
  uint32 count = 0;
  uint32 seg_count = 0;
  uint32 loop_count = 0;
  uint32 memcpy_count = 0;
  uint32 bytes_to_copy =0;
  uint8 * table_ptr = NULL;
  uint8 work_table;
  tdsrrc_cmd_type * cmd_ptr = NULL;
  uint32 rep_count;
  uint16 num_bits_per_rep = 0;
  uint32 density = 0;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  if(rrc_sib_type != TDSRRC_TOT_SIBS)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Updating SIB Sleep Schedule Table for SIB :%d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_Sleep:Updating SIB Sleep Schedule Table for all un-received SIB");
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Previous table in use:%d",tdssib_sleep_schedule_table_in_use);

  /* Check if there is enough wait time to update the schedule.
   * This is to avoid RRC too frequent buffer update that can result 
   * data corruption between RRC and L1
   */

  if (tdsrrctmr_get_remaining_time(TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER not yet expired. Skip schedule update");
    return;
  }

  if(tdssib_sleep_schedule_table_in_use == 0)
  {
    table_ptr = tdssib_sleep_schedule_table_1;
    work_table = 1;
  }
  else
  {
    table_ptr = tdssib_sleep_schedule_table_0;
    work_table = 0;
  }

  memset(table_ptr,0x00,TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);   

  /* Loop through the all the SIB(segments) and update the sleep schedule table based on 
     the SIB Schedule */
  for(count = 1; count < TDSRRC_TOT_SIBS; count++)
  {
    if ((count != (uint32)rrc_sib_type)//For specific SIB type
         && (!((rrc_sib_type == tdsrrc_SIB3) && (count == (uint32)tdsrrc_SIB7))) //always contain SIB7 when RRC only needs SIB3
         && (TDSRRC_TOT_SIBS != rrc_sib_type))//For all SIB type
    {
      continue;
    }
    /* Include the schedule only if the SIB is needed & the SIB has a valid schedule 
	     and if it has not been decoded already */
    if((tdssib_events_ptr->event[event_index].tsib[count].sib_needed == TRUE) &&
        ((tdssib_events_ptr->event[event_index].tsib[count].sib_decoded == FALSE)||
        (count == (uint32)tdsrrc_SIB7))&&
       (tdssib_events_ptr->sib_sched[count].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)
      )
    {
      /* Calculate the rep count to be filled in the loop */
      rep_count = tdssib_events_ptr->scheduling_max_rept_rate/tdssib_events_ptr->sib_sched[count].repitition_rate;
      
      for (seg_count = 0; seg_count < tdssib_events_ptr->sib_sched[count].num_segments; seg_count++)
      {
        uint32 sfn = tdssib_events_ptr->sib_sched[count].sfn_offset[seg_count]; /* Local copy of SFN */
        uint32 byte_pos = sfn>>3;                /* (sfn/8) Gives the byte location in sib sleep schedule table */
        uint8 byte_offset = (uint8)(sfn & 0x07); /* (sfn%8)Gives the offset within the byte in sib sleep table */
        uint8 bit_mask = 0xFF;
		
        bit_mask = tdsrrcsib_gen_bit_mask_for_schedule_table(byte_offset);
        /* Take into account the repetition rate and set the repetition masks also.Do it only till max rep rate
            since we would do a memcpy for rest of the table.Apply the calculated bitmask for the 
            byte till the max repetition cycle */
        for(loop_count = 0; loop_count < rep_count; loop_count++)
        {
          uint32 rep_byte_pos = byte_pos + (loop_count * (tdssib_events_ptr->sib_sched[count].repitition_rate/8));
          table_ptr[rep_byte_pos] |= bit_mask;                       

          /* Optimization to set a few extra 1s after last segment of Scheduling blocks
             Add 4 1s if this segment in the last segment of SB1 or SB2. this is to keep 
             L1 awake after the last SB segment. SB can have schedule for some SIBs and L1
             needs to stay awake to process the new schedule from SB    */
          if(((count == (uint32)tdsrrc_SB1) ||(count == (uint32)tdsrrc_SB2)) &&
              (seg_count == (uint32)(tdssib_events_ptr->sib_sched[count].num_segments-1)))
          {
            uint8 curr_pos_bit_mask = 0x00;
            uint8 next_pos_bit_mask = 0x00;
            switch(bit_mask)
            {
              case 0xC0:
                /* Actual bitmask : 1100 0000. 
                   Modified bitmask with 4 dummy ones: 1111 1100 (0xFC) */
                curr_pos_bit_mask = 0xFC; 
                break;

              case 0x30:
                /* Actual bitmask : 0011 0000. 
                   Modified bitmask with 4 dummy ones: 0011 1111 (0x3F) */
                curr_pos_bit_mask = 0x3F; 
                break;

              case 0x0C:
                /* Actual bitmask : 0000 1100. 
                   Modified bitmask with 4 dummy ones: 0000 1111 1100 0000(0x0F,0xC0) */
                curr_pos_bit_mask = 0x0F; 
                next_pos_bit_mask = 0xC0;
                break;

              case 0x03:
                /* Actual bitmask : 0000 0011. 
                   Modified bitmask with 4 dummy ones: 0000 0011 1111 0000(0x03,0xF0) */
                next_pos_bit_mask = 0xF0;
                break;

              default:
                /* Actual bitmask : 1111 1111. 
                   Modified bitmask with 4 dummy ones: 1111 1111 1111 0000(0xFF,0xF0) */
                next_pos_bit_mask = 0xF0;
                break;
            }

            /* Modify the current pos bitmask */
            table_ptr[rep_byte_pos] |= curr_pos_bit_mask;               

            /* Set the next byte. Make sure you are not crossing the array boundary */
            if(rep_byte_pos < (TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE -1))  
            {
              /* Set the 4 frames(40ms) in the next byte to all 1s so that L1 will stay awake after SB */
              table_ptr[rep_byte_pos+1] |= next_pos_bit_mask;
            }
            else //wraparound case
            {
              table_ptr[0] |= next_pos_bit_mask;
            }
          }  
        }
      }
    }
  }

  /* Now we have filled the table for max repetition rate bytes.Fill the rest of the table by memcpy */
  //memcpy_count = RRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE/tdssib_events_ptr->scheduling_max_rept_rate;
  memcpy_count = 4096/(tdssib_events_ptr->scheduling_max_rept_rate);
  bytes_to_copy = tdssib_events_ptr->scheduling_max_rept_rate/8;

  for(count = 1; count < memcpy_count ; count++)
  {
    uint32 index = count * bytes_to_copy;
    memscpy(table_ptr+index,bytes_to_copy,table_ptr,bytes_to_copy);
  }

  /* Now we have the sched table updated.Send the change_ind to L1 */
  if((cmd_ptr = tdsrrc_get_int_cmd_buf()) != NULL)
  {
    tdsl1_req_cmd * rrc_cmd_l1_req = NULL;
    tdsrrc_get_pointer_l1_req_value(&cmd_ptr->cmd.rrc_llc_req.l1_req, &rrc_cmd_l1_req);
 
    cmd_ptr->cmd_hdr.cmd_id= TDSRRC_LLC_CMD_REQ;
    cmd_ptr->cmd.rrc_llc_req.procedure = TDSRRC_PROCEDURE_SIB;
    cmd_ptr->cmd.rrc_llc_req.cmd_dest = TDSLAYER1_PHY;
    cmd_ptr->cmd.rrc_llc_req.l1_cmd.cmd_id = TDSL1_CPHY_SIB_SCHED_CHANGE_IND;

    rrc_cmd_l1_req->sib_sched_change_ind_req.sib_sfn_bit_mask = table_ptr;

    rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = FALSE;
    if (tdssib_events_ptr->event[event_index].for_serving_cell == TRUE)
    {
      if(TRUE == tdsrrccsp_do_opt_for_con_to_idle())
      {
        rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = TRUE;
        TDSRRC_MSG3(MSG_LEGACY_HIGH, "Cell sel:In DDS from Connected to IDLE, Hold lock",0,0,0);
      }
    }
    else if (TRUE == tdsrrccsp_is_other_sub_in_dds())
    {
      rrc_cmd_l1_req->sib_sched_change_ind_req.hold_trm_lock_for_sib_gap = TRUE;
      TDSRRC_MSG3(MSG_LEGACY_HIGH, "Cell resel:In DDS, Hold lock",0,0,0);
    }
    tdsrrc_put_int_cmd( cmd_ptr );
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB_Sleep:TDSL1_CPHY_SIB_SCHED_CHANGE_IND sent to L1");

    /* Start the update wait timer to avoid frequent schedule update */
    tdsrrctmr_start_timer(TDSRRCTMR_SIB_SCHEDULE_UPDATE_WAIT_TIMER, 
                          TDSRRCSIB_SIB_SCHEDULE_UPDATE_WAIT_TIME_MS);
  }
  else
  {
    ERR_FATAL("SIB_Sleep:Cound not get memory for TDSL1_CPHY_SIB_SCHED_CHANGE_IND",0,0,0);
  }

  tdssib_sleep_schedule_table_in_use = work_table;
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Current table in use:%d",tdssib_sleep_schedule_table_in_use);

  for(count = 0; count < (uint32)(tdssib_events_ptr->scheduling_max_rept_rate/8);count++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB_Sleep: Table Contents byte[%d] = %0X",count,table_ptr[count]);
    
    num_bits_per_rep += TDSRRCSIB_BYTE_BITS(table_ptr[count]);
  }

  /* Debug print */
  density = (num_bits_per_rep * 100) / tdssib_events_ptr->scheduling_max_rept_rate;

  if(rrc_sib_type != TDSRRC_TOT_SIBS)
  {
	  TDSRRC_MSG4(MSG_LEGACY_HIGH,"SIB_Sleep table after update by SIB %d :Max repetition rate is %d, num_bits_per_rep %d, density %d pc",
            tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type),
            tdssib_events_ptr->scheduling_max_rept_rate,
            num_bits_per_rep,
            density);
  }
  else
  {
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"SIB_Sleep table after update for all un-received SIB:Max repetition rate is %d, num_bits_per_rep %d, density %d pc",
            tdssib_events_ptr->scheduling_max_rept_rate,
            num_bits_per_rep,
            density);
  }
}
#endif /*FEATURE_TDSCDMA_SPECIFIC_SIB_READING	*/

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
void tdsrrcsib_update_sixhr_sib_timer(void)
{

    /*Call the function which will update the counter
       vaule for each active cell index & restart the 
       10 minTimer*/
    tdsrrcsibdb_update_six_hr_timer();

  if((tdssibdb.active_cell_sibs_valid  == TRUE)&&
      (tdssibdb.cell[tdssibdb.active_cell_index].six_hr_count == TDSMAX_CELL_INDEX_COUNT_SIXHR)
    )
  {
    tdsrrc_cmd_type *rrc_cmd_ptr;            /* Pointer to the RRC Command */
    tdsrrc_MasterInformationBlock * mib_ptr;  /*Pointer to MIB*/
    boolean sib_active;
    tdsrrc_state_e_type rrc_state;

    rrc_state = tdsrrc_get_state();
    sib_active = tdsrrcsib_is_sib_active();
    /*Update sibs due to SFN drift has low priority than six hrs sib, 
        So check for it, If yes let six hrs sib be posted*/
    if(sib_active == TRUE)
    {
      if(tdsrrcsib_is_current_event_due_to_sfn_drift() == TRUE)
      {
        sib_active = FALSE;
      }
    }
    /*Currently six hrs Update_sibs is supported only in DISCONNECTED state.
       six hrs Update_sib will be posted only is all following cases are satisfied
       1) None of the sib event should be active
       2) RCE procedure should not be active, as UE will be moving to CONNECTING state.
       3) tdssib_change_safe_guard should not be set to TRUE which make sure that when procedures
           are acting on sib_change_ind,Differ posting of six hrs Update_sibs.
       4) BPLMN Should not be started.
       5) SIB should not have any pending command.
    */
    if((sib_active == FALSE)&&
        (rrc_state == TDSRRC_STATE_DISCONNECTED) &&
        (tdsrrcsib_cmd_store.pending_flag == FALSE)&&
        (TD2TD_SEARCH_STOP == tdsrrccsp_bplmn_srch_in_progress())&&
        (tdsrrcrce_is_rce_procedure_active() == FALSE)&&
        (tdsrrcpg1_return_bcch_mod_info_status() == FALSE)
      )
    {
      /* Build TDSRRC_UPDATE_SIBS_IND command to post to SIB */
      /* Allocates the buffer to RRC Internal command */
      if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
      {     
        /* Fill in the command parameters for TDSRRC_UPDATE_SIBS_IND */
        rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_UPDATE_SIBS_IND;
  
        if((mib_ptr= (tdsrrc_MasterInformationBlock *)  tdsrrcsibdb_return_sib_for_srv_cell
           (tdsrrc_MIB)) != NULL)
        {
          rrc_cmd_ptr->cmd.update_sibs_ind.mib_value_tag = mib_ptr->mib_ValueTag;
  
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"mib-vtag is %d sfn is %d ***",rrc_cmd_ptr->cmd.update_sibs_ind.mib_value_tag,tdsseq_get_sfn());
  
          rrc_cmd_ptr->cmd.update_sibs_ind.procedure = TDSRRC_PROCEDURE_SIB;
          rrc_cmd_ptr->cmd.update_sibs_ind.cause = TDSRRCSIB_SIX_HR_TMR_EXPIRY;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"6hrs sib:6hrs sib timer expired for serving cell Post TDSRRC_UPDATE_SIBS_IND cmd ");
  
          tdsrrc_put_int_cmd(rrc_cmd_ptr);   
        }
      } 
      else
      {
        ERR_FATAL("Memory allocation for the %d comamnd failed",TDSRRC_UPDATE_SIBS_IND,0,0);
      }
    }
  }
    
}

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
boolean tdsrrcsib_is_need_to_cleanup_cell(  tdssib_events_index_e_type  event_index)
{

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }
  
  if((tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
      (tdssib_events_ptr->event[event_index].sib_scan_type == TDSRRC_SIB_NORMAL) &&
      (tdssib_events_ptr->event[event_index].req_proc == TDSRRC_PROCEDURE_CSP))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"6hr sib:For this event check six hr Criteria if Index found in SIB db");
    return TRUE;
  }
  else
  {
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"6hr sib:No need to check six hr Criteria for this SIB event");
    return FALSE;
  }
}

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
boolean tdsrrcsib_is_event_sixhrs_sib_update(  tdssib_events_index_e_type  event_index)
{
  boolean status = FALSE;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }
  
  if(tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_UPDATE_SIBS )
  {
    if(tdssibdb.active_cell_sibs_valid == TRUE)
    {
       if(tdssib_events_ptr->update_sibs_cause == TDSRRCSIB_SIX_HR_TMR_EXPIRY)
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"6hr sib:This event is six hrs update_sibs event");
          status = TRUE;
        }
    }
  }
  return status;
}
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
boolean tdsrrcsib_is_sixhrs_sib_timer_expired(  tdssib_events_index_e_type  event_index)
{
  boolean status = FALSE; 

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }
  
  if((tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_UPDATE_SIBS )&&
      (tdssibdb.active_cell_sibs_valid == TRUE)&&
      (tdssibdb.cell[tdssibdb.active_cell_index].six_hr_count  == TDSMAX_CELL_INDEX_COUNT_SIXHR)&&
      (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED)
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Six hours SIB timer for serving cell expired");
    status = TRUE;
  }
  return status;
}
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
boolean tdsrrcsib_is_current_event_due_to_sfn_drift(void)
{
  uint8 count;
  boolean status = FALSE;
  for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
      if(tdssib_events_ptr->event[count].event_name == TDSRRCSIB_UPDATE_SIBS)
      {

      }
    }/* End if active events */
  }
  return status;
}
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
boolean tdsrrcsib_can_sib7_event_start(void)
{
  boolean start_sib7_event=FALSE;
  if(tdsrrc_get_state() == TDSRRC_STATE_CELL_FACH)
  {
    tdsrrcllc_oc_set_status_e_type oc_set_status;
    oc_set_status = tdsrrcllc_get_ordered_config_status();
    if(!((TDSOC_SET_FOR_CELL_DCH == oc_set_status)||
           (TDSOC_SET_FOR_URA_PCH == oc_set_status)||
           (TDSOC_SET_FOR_CELL_PCH == oc_set_status)||
           (TDSOC_SET_FOR_FACH_CELL_PCH_TRANS == oc_set_status)||
           (TDSOC_SET_FOR_FACH_URA_PCH_TRANS == oc_set_status)
          )
      )
    {
      if((tdsrrcsib_is_sib_active() == FALSE)&&
          (tdssibdb.active_cell_sibs_valid == TRUE)&&
          (TDSRRC_CAMPED_ON == tdsrrcccm_get_curr_camping_status(TDSRRC_PROCEDURE_SIB))
        )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib7:Start sib-7 event in FACH state");
        start_sib7_event=TRUE;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"sib7:Ignore this attempt to start sib7 as sib event is active");
      }
    }
    else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"sib7:Ignore this attempt to start sib7 event in FACH as OC is set=%d",oc_set_status);
    }
  }
  return start_sib7_event;
}
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
  tdsrrc_SIB_type rrc_sib_type,                /* Indicates whether this update is from MIB or SBs */
  tdssib_events_index_e_type event_index   /* Index of the current event */
)
{
  uint32 count = 0;
  uint32 seg_count = 0;
  uint32 loop_count = 0;
  uint8 * table_ptr = NULL;
  uint32 rep_count;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Updating SIB Sleep Schedule Table for SIB :%d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Current table in use:%d",tdssib_sleep_schedule_table_in_use);

  if(tdsrrcsib_is_current_event_sib7_in_fach() ==TRUE)
  {
  
    table_ptr = tdssib_sleep_schedule_table_0;

    memset(table_ptr,0x00,TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);   
  
    /* Loop through the all the SIB(segments) and update the sleep schedule table based on 
       the SIB Schedule */
    for(count = 1; count < TDSRRC_TOT_SIBS; count++)
    {
      /* Include the schedule only if the SIB is needed & the SIB has a valid schedule 
  	     and if it has not been decoded already */
      if((tdssib_events_ptr->event[event_index].tsib[count].sib_needed == TRUE) &&
         (tdssib_events_ptr->event[event_index].tsib[count].sib_decoded == FALSE) &&
         (tdssib_events_ptr->sib_sched[count].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)) 
      {
        /* Calculate the rep count to be filled in the loop, lets fill complete table so lets keep
            max rept rate to 4096 which is SFN cycle*/
        rep_count = 4096/tdssib_events_ptr->sib_sched[count].repitition_rate;

        for (seg_count = 0; seg_count < tdssib_events_ptr->sib_sched[count].num_segments; seg_count++)
        {
          uint32 sfn = tdssib_events_ptr->sib_sched[count].sfn_offset[seg_count]; /* Local copy of SFN */
          uint32 byte_pos = sfn>>3;                /* (sfn/8) Gives the byte location in sib sleep schedule table */
          uint8 byte_offset = (uint8)(sfn & 0x07); /* (sfn%8)Gives the offset within the byte in sib sleep table */
          uint8 bit_mask = 0xFF;
          boolean is_sib7_rep_less_then_32 = FALSE; 
   
          bit_mask = tdsrrcsib_gen_bit_mask_for_schedule_table(byte_offset);
          if(count == 7)
          {
            if(tdssib_events_ptr->sib_sched[count].repitition_rate < 32)
            {
              is_sib7_rep_less_then_32 = TRUE;
            }
          }
          /* Apply the calculated bitmask for the byte till the max repetition cycle */
          for(loop_count = 0; loop_count < rep_count; loop_count++)
          {
            uint32 rep_byte_pos;
            if(is_sib7_rep_less_then_32 == TRUE)
            {
              /*Lets skipp intermidiate sib-7's, as Spec says SIB-7 have to be read at a minimum of 320msec
                 Its also controled by a calulation, which uses a parameter from sib-7. But since in our implimentation
                 S-BCCH is up all the time in FACH, So lets read SIB7 at a minmum of 320msec*/
              rep_byte_pos = byte_pos + (loop_count * (32/8));
            }
            else
            {
              rep_byte_pos = byte_pos + (loop_count * (tdssib_events_ptr->sib_sched[count].repitition_rate/8));
            }
            
            if(rep_byte_pos < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE)
            {
              table_ptr[rep_byte_pos] |= bit_mask;
            }
          }
        }
      }
    }

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Current table in use:%d",tdssib_sleep_schedule_table_in_use);
  
    /* Debug print */
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB_Sleep table after update by SIB %d :Max repetition rate is %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type),tdssib_events_ptr->scheduling_max_rept_rate);
  
    for(count = 0; count < (uint32)(tdssib_events_ptr->scheduling_max_rept_rate/8);count++)
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB_Sleep: Table Contents byte[%d] = %0X",count,table_ptr[count]);
    }
  }
}
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
)
{
  boolean status = FALSE;
  if (tdssib_events_ptr->event[index].event_active == TRUE)
  {
    if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)&&
       (tdssib_events_ptr->event[index].req_proc == TDSRRC_PROCEDURE_GPS)&&
       (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN))
    {
      /*For sib_scan_type TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN, we need SIB3 only as this event 
       *is to read Cell I.D which comes in SIB3.Since we dont store any SIB's for this event, so check
       *tempholder to see if we have received SIB3, If yes then return TRUE since we completed task 
       *for this sibevent, else return FALSE
       */
      if((tdssib_events_ptr->event[index].tempholder.mib_decoded == TRUE)&&
         (tdssib_events_ptr->event[index].tempholder.sib3_decoded == TRUE))
      {
        status = TRUE;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRCGPS:CELL I.D read in DCH event completed.Scan type %d",tdssib_events_ptr->event[index].sib_scan_type);
      }
    }
  }
  return status;
}
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
boolean tdsrrcsib_is_current_event_sib_read_in_dch(void)
{
  uint8 count;
  boolean status = FALSE;
  for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
      if(tdssib_events_ptr->event[count].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)
      {
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
        if((tdssib_events_ptr->event[count].req_proc == TDSRRC_PROCEDURE_GPS) &&
           (tdssib_events_ptr->event[count].for_serving_cell == TRUE)&&
           (tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN)
          )
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRCGPS:Event I.D. in DCH active");
          status = TRUE;
          break;
        }
#endif
      }
    }/* End if active events */
  }
  return status;
}

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
tdsrrc_proc_e_type tdsrrcsib_get_proc_id_for_sib_read_in_dch(void)
{
  uint8 count;
  tdsrrc_proc_e_type proc_id = TDSRRC_PROCEDURE_NONE;

  for(count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++)
  {
    if( tdssib_events_ptr->event[count].event_active == TRUE )
    {
#ifdef FEATURE_TDSCDMA_GPS_CELL_ID_READING
      if((tdssib_events_ptr->event[count].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS) &&
         (tdssib_events_ptr->event[count].sib_scan_type == TDSRRC_SIB_CELL_ID_READ_IN_DCH_SCAN))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB reading in DCH is active. proc_id %d",tdssib_events_ptr->event[count].req_proc);
        proc_id = tdssib_events_ptr->event[count].req_proc;
        break;
      }
#endif
    }
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"ret proc id is %d",proc_id);
  return proc_id;
}


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
)
{
  boolean status = FALSE;
  boolean is_update_sib_active = FALSE;
  uint8 count;
  /* Check if any event is active */
  for( count = 0; count < (uint8)TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    if(( tdssib_events_ptr->event[count].event_active == TRUE )&&
        ( tdssib_events_ptr->event[count].event_name == TDSRRCSIB_UPDATE_SIBS ))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB event %d active",tdssib_events_ptr->event[count].event_name);
          is_update_sib_active =  TRUE;
      break;
    }/* End if active events */
  }
  
  if(is_update_sib_active == TRUE)
  {
    status = FALSE;
  }
  else
  {
    if(tdsrrcsib_is_current_event_sib7_in_fach() == TRUE)
    {
      tdssib7info.sib_timer_update_req = FALSE;
      tdssib7info.sib_ext_proc_update_req = FALSE;
      tdssib_events_ptr->srv_bcch.bcch_action = TDSSIB_BCCH_NO_ACTION;
      tdsrrcsib_force_sib_event_init(TDSRRCSIB_ABORT_SIB7_UPDATE);
    }
    status = TRUE;
  }
  return status;
}
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
)
{
  uint32  count; /* Counter used in loops */
  boolean status = FALSE;

  for( count = 0; count < (uint32)TDSMAX_SIB_EVENTS_INDEX; count++ )
  {
    // Check if the event is stil active.
    if (tdssib_events_ptr->event[count].event_active == TRUE)
    {
      // Ensure that the SIB7 update was requested.
      if( tdssib_events_ptr->event[count].event_name == TDSRRCSIB_UPDATE_SIBS)
      {
        //sib7 update is on-going
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"UPDATE_SIBS on-going");
        status = TRUE;
      }
    }
  }
  return status;
}

/*===========================================================================
FUNCTION tdsrrcsib_decode_duplicate_sib7

DESCRIPTION

  This function decodes the duplicate copy of sib7 
  received  for the on-going event.
  
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
)
{
   uint32 pdu_length;
   tdsrrc_SysInfoType7 *sib7_ptr = NULL;
   boolean decoded_sib7 = FALSE; 

  /* Get the PDU length in bytes */
  pdu_length =  TDSGET_PDU_LENGTH_IN_BYTES( 
    tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB7].sib_tot_length );

  if(pdu_length!=0)
  {
    /* ASN.1 decode the PDU */
    sib7_ptr =  tdsrrcasn1_decode_pdu( 
      (void *)(tdssib_events_ptr->event[event_index].tsib[tdsrrc_SIB7].sib_data_ptr), /* Encoded SIB      */
      (int)pdu_num,                                                       /* SIB's PDU number */
      pdu_length                                                     /* SIB's Length in bytes */
      );
    
    if(sib7_ptr != NULL )
    {
      tdsrrcsibdb_free_sib( (int)pdu_num,
                               (void *)sib7_ptr);
      sib7_ptr = NULL;
      decoded_sib7 = TRUE;
    }
  }
  return decoded_sib7;
}

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
)
{
  tdsrrc_SIB_type int_sib_type = tdsrrc_MAX_SIB;

  switch(asn1_sib_type)
  {
    case TDSRRC_SIB_TYPE(masterInformationBlock) :
      int_sib_type = tdsrrc_MIB;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType1) :
      int_sib_type = tdsrrc_SIB1; 
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType2) :
      int_sib_type = tdsrrc_SIB2;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType3) :
      int_sib_type = tdsrrc_SIB3;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType4) :
      int_sib_type = tdsrrc_SIB4;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType5) :
      int_sib_type = tdsrrc_SIB5;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType6) :
      int_sib_type = tdsrrc_SIB6;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType7) :
      int_sib_type = tdsrrc_SIB7;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType11) :
      int_sib_type = tdsrrc_SIB11;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType12):
      int_sib_type = tdsrrc_SIB12; 
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType18):
      int_sib_type = tdsrrc_SIB18;
      break;
    case TDSRRC_SIB_TYPE(schedulingBlock1) :
      int_sib_type = tdsrrc_SB1;
      break;
    case TDSRRC_SIB_TYPE(schedulingBlock2) :
      int_sib_type = tdsrrc_SB2;
      break;
    case TDSRRC_SIB_TYPE(systemInformationBlockType5bis) :
      int_sib_type = tdsrrc_SIB5bis;
      break;
    case TDSRRC_SIB_TYPE(extensionType):
      int_sib_type = tdsrrc_ExtensionType;
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Invalid ASN.1 tdsrrc_SIB_Type: %d",asn1_sib_type);
      break;
  }
  return int_sib_type;
}

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
)
{
  tdsrrc_SIB_Type asn1_sib_type = tdsrrc_SIB_Type_dummy;

  switch(int_sib_type)
  {
    case tdsrrc_MIB:
      asn1_sib_type = TDSRRC_SIB_TYPE(masterInformationBlock);
      break;
    case tdsrrc_SIB1 :
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType1); 
      break;
    case tdsrrc_SIB2:
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType2);
      break;
    case tdsrrc_SIB3 :
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType3);
      break;
    case tdsrrc_SIB4 :
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType4);
      break;
    case tdsrrc_SIB5 :
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType5);
      break;
    case tdsrrc_SIB6 :
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType6);
      break;
    case tdsrrc_SIB7 :
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType7);
      break;
    case tdsrrc_SIB11 :
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType11);
      break;
    case tdsrrc_SIB12:
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType12); 
      break;
    case tdsrrc_SIB18:
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType18);
      break;
    case tdsrrc_SB1 :
      asn1_sib_type = TDSRRC_SIB_TYPE(schedulingBlock1);
      break;
    case tdsrrc_SB2 :
      asn1_sib_type = TDSRRC_SIB_TYPE(schedulingBlock2);
      break;
    case tdsrrc_SIB5bis :
      asn1_sib_type = TDSRRC_SIB_TYPE(systemInformationBlockType5bis);
      break;
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    case tdsrrc_SIB11bis:
      asn1_sib_type = TDSRRC_SIB_TYPE(extensionType);
      break;
#endif
#ifdef FEATURE_TDSCDMA_SIB19
    case tdsrrc_SIB19:
      asn1_sib_type = TDSRRC_SIB_TYPE(extensionType);
      break;
#endif

    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unsupported RRC tdsrrc_SIB_type: %d",int_sib_type);
      break;
  }
  return asn1_sib_type;
}

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
)
{
  boolean decode_succeeded = FALSE;

  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_received == TRUE)&&
     (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_tot_length != 0)
     )
  {
  
  /*New SIB7 has been received*/
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Decoding SIB7 in UPDATE SIBS event");
    decode_succeeded = 
        tdsrrcsib_decode_and_save_sib_for_current_event((void *)&(tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib7),
                                            index, tdssibdb.active_cell_index, tdsrrc_SIB7,
                                            tdsrrc_SysInfoType7_PDU,
                                            tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib7_encoded_ptr,
                                            &tdssibdb.cell[tdssibdb.active_cell_index].sibs.sib7_encoded_pdu_length);

/*Decode of SIB7 successful. Indicate the change in tdssibdb. Also, update LLC config pointers with SIB7 content */
  if( decode_succeeded == TRUE )
    {
      tdsrrcsibdb_save_active_cell_sib_change(tdsrrc_SIB7);
      /* New W code but not found in TDS LLC */
      /* (void)rrcllc_update_configs_with_sib7_info_in_fach(); */
    }
  }

  return;
}
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
)
{
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();
  tdsrrc_cmd_type *int_cmd_ptr = NULL;

  switch(rrc_state)
  {
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
    case TDSRRC_STATE_CELL_FACH:
      /* Trigger Out Of Service area */
      if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
      { 
        int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;
        int_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_REENTERED_SERVICE_AREA;
        int_cmd_ptr->cmd.initiate_cell_update.cu_complete_cnf_required = FALSE;
        int_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_SIB;
        tdsrrc_put_int_cmd(int_cmd_ptr); 
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending Cell Update Initiate Req,cause Re-entered Service Area,rrc_state:%d",rrc_state);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to allocate memory for cmd TDSRRC_CELL_UPDATE_INITIATE_REQ");
      }
      break;

    case TDSRRC_STATE_DISCONNECTED:
    case TDSRRC_STATE_CONNECTING:
      /* Trigger Cell Selection */
      if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
      { 
        int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INITIATE_CELL_SELECTION_IND;
        int_cmd_ptr->cmd.initiate_cell_sel_ind.cause = TDSRRC_OUT_OF_SERVICE_AREA_IND;
        int_cmd_ptr->cmd.initiate_cell_sel_ind.proc = TDSRRC_PROCEDURE_SIB;
        int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state = rrc_state;
        int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present = FALSE;
        int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present  = FALSE;
        tdsrrc_put_int_cmd(int_cmd_ptr); 
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sending Initiate Cell Selection Ind to CSP,cause Out Of Service Ind.rrc_state:%d",rrc_state);
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unable to allocate memory for cmd TDSRRC_INITIATE_CELL_SELECTION_IND");
      }
      break;

    default:
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid state,no action taken on S-BCCH failure");
      break;
  }
  return ;
}
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
)
{
  boolean status =  FALSE;

  if((tdssib_events_ptr->event[index].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)&&
     (tdssib_events_ptr->event[index].sib_scan_type == TDSRRC_SIB_NORMAL)&&
     (tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_needed))
  {
    status = TRUE;
  }
  return status;
}

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
)
{
  boolean status =  FALSE;
  tdsrrc_SysInfoType3 *sib3_ptr = NULL;

#ifdef FEATURE_3GPP_CSFB_TDS
  if((tdsrrc_csfb_call_status == TRUE)&&
      (tdsrrc_csfb_skip_sib11_opt_nv == TRUE))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB:Deferred SIB11 feature enabled for CSFB ");
    return TRUE;
  }
#endif

  if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB3] )
  {
    sib3_ptr = tdsrrcsibdb_return_sib_for_db_index(tdsrrc_SIB3,dbindex);
    if(sib3_ptr!=NULL)
    {
      if(DEFERRED_MEAS_SUPPORT(sib3_ptr))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred Measurement reading supported for dbindex %d",dbindex);
        status = TRUE;
      }
      else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred Measurement reading not supported for dbindex %d",dbindex);
      }
    }
  }
  return status;
}


/*===========================================================================

FUNCTION tdsrrcsib_is_event_update_sibs_for_deferred_sib

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
)
{
  boolean status = FALSE;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return FALSE;
  }
  
  if(tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_UPDATE_SIBS )
  {
    if(tdssibdb.active_cell_sibs_valid == TRUE)
    {
       if(tdssib_events_ptr->update_sibs_cause == TDSRRCSIB_DEFERRED_SIB_READ)
       {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB:This event is Update SIBS due to Deferred SIB reading");
          status = TRUE;
       }
       else
       {
         TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB:Event not Update SIBS due to Deferred SIB reading");
       }
    }
  }
  return status;
}
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
)
{
  int32 dbindex = tdssibdb.active_cell_index;
  tdsrrcllc_oc_set_status_e_type oc_set_status ;
  if(tdsrrcsib_cmd_store.pending_flag == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: pending cmd flag true");
  }

  if(tdsrrcsib_is_sib11_11bis_12_read_deferred(TDSDEFERRED_UPDATE_SIB)==TRUE)
  {
    tdsrrc_cmd_type *rrc_cmd_ptr;            /* Pointer to the RRC Command */
    tdsrrc_MasterInformationBlock * mib_ptr;  /*Pointer to MIB*/
    {
      oc_set_status = tdsrrcllc_get_ordered_config_status();
      if(oc_set_status != TDSOC_SET_FOR_CELL_DCH)
      {
        /* Build TDSRRC_UPDATE_SIBS_IND command to post to SIB */
        /* Allocates the buffer to RRC Internal command */
        if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
        {     
          /* Fill in the command parameters for TDSRRC_UPDATE_SIBS_IND */
          rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_UPDATE_SIBS_IND;
    
          if((mib_ptr= (tdsrrc_MasterInformationBlock *)  tdsrrcsibdb_return_sib_for_srv_cell
             (tdsrrc_MIB)) != NULL)
          {
            rrc_cmd_ptr->cmd.update_sibs_ind.mib_value_tag = mib_ptr->mib_ValueTag;
            rrc_cmd_ptr->cmd.update_sibs_ind.procedure = TDSRRC_PROCEDURE_SIB;
            rrc_cmd_ptr->cmd.update_sibs_ind.cause = TDSRRCSIB_DEFERRED_SIB_READ;
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB:Posting TDSRRC_UPDATE_SIBS_IND cmd ");
    
            tdsrrc_put_int_cmd(rrc_cmd_ptr);   
          }
        } 
        else
        {
          ERR_FATAL("Memory allocation for the %d comamnd failed",TDSRRC_UPDATE_SIBS_IND,0,0);
        }
      }
    }
  }
  else
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Deferred SIB:SIB11 already read.Val tag %d",tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11]);
  }
}
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
)
{
  boolean status = FALSE;
  int32 dbindex = tdssibdb.active_cell_index;

  if(tdssibdb.active_cell_sibs_valid  == TRUE)
  {
    if(tdsrrcsib_can_nw_support_deferred_sib(dbindex) == TRUE)
    {
      switch(deferred_sib_type) 
      {
        case TDSDEFERRED_SIB11:
          if(tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] == TDSINVALID_MIB_SB_VALUE_TAG)
          {
            status = TRUE;
          }
          break;
        case TDSDEFERRED_SIB12:
          if((tdsrrcsib_check_if_sib12_present_for_dbindex(dbindex) == TRUE)&&
             (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12]
                  == TDSINVALID_MIB_SB_VALUE_TAG)
             )
          {
            status = TRUE;
          }
          break;
          
        case TDSDEFERRED_SIB11_BIS:
  #ifdef  FEATURE_TDSCDMA_SIB11BIS_SUPPORT
          if((tdsrrcsib_check_if_extension_sib_present(dbindex) == TRUE)&&
                  (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] == TDSINVALID_MIB_SB_VALUE_TAG))
          {
            status = TRUE;
          }
  #endif
          break;
  
        case TDSDEFERRED_ANY_ALLOWED_SIB:
          if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] == TDSINVALID_MIB_SB_VALUE_TAG)||
             ((tdsrrcsib_check_if_sib12_present_for_dbindex(dbindex) == TRUE)&&
             (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12]
                  == TDSINVALID_MIB_SB_VALUE_TAG))
            )
          {
            status = TRUE;
          }
          break;

        case TDSDEFERRED_UPDATE_SIB:
          if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] == TDSINVALID_MIB_SB_VALUE_TAG)||
             ((tdsrrcsib_check_if_sib12_present_for_dbindex(dbindex) == TRUE)&&
             (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12]
                  == TDSINVALID_MIB_SB_VALUE_TAG))
  #ifdef  FEATURE_TDSCDMA_SIB11BIS_SUPPORT
            ||((tdsrrcsib_check_if_extension_sib_present(dbindex) == TRUE)&&
                              (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] == TDSINVALID_MIB_SB_VALUE_TAG))
  #endif
  #ifdef FEATURE_TDSCDMA_SIB19
            || ((tdsrrcsib_check_if_sib19_present(dbindex) == TRUE)&&
               (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] == TDSINVALID_MIB_SB_VALUE_TAG))
  #endif
            || ((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18] == TDSINVALID_MIB_SB_VALUE_TAG)  &&
               ((tdsrrcsib_is_sib_scheduled( tdsrrc_SIB18,dbindex, FALSE,NULL))))
            )
          {
            status = TRUE;
          }
          break;
  
        default:
          TDSRRC_MSG0(MSG_LEGACY_ERROR,"Deferred SIB:Unknown SIB type");
          break;
      }
    }
  }

  if(status == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB:SIBS deferred");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB:SIBS not deferred.");
  }
  return status;
}


/*===========================================================================

FUNCTION  rrcsib_is_sib12_present_in_serving_cell

DESCRIPTION

  This function will check if SIB12 is present for the given SIB DB index.

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
)
{
  if((tdsrrcsib_is_sib_scheduled( tdsrrc_SIB12,
                                                       dbindex
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
                                                       , FALSE
                                                       ,NULL
#endif
                                                       )) == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 12 is scheduled in MIB or SB");
    if( tdssibdb.cell[dbindex].sib_valid[tdsrrc_SIB11] == TRUE)
    {
      if(tdsrrcsib_check_if_connected_mode_sibs_present(tdssibdb.cell[dbindex].sibs.sib11_encoded_ptr[0],
          tdsrrc_SIB11) == TRUE )
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 12 present in cell");
        return TRUE;
      }
      else
      {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 12 is not present in cell");
       return FALSE;
      }
    }
    else
    {
       TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: SIB11 not read");
       return TRUE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"SIB 12 is not scheduled in MIB or SB");
    return FALSE;
  }
}
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

void tdsrrcsib_post_process_deferred_sib(tdssib_events_index_e_type index,   uint32 dbindex)
{
  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_needed)&&
     !(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_decoded))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: SIB11 not read");
    tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] = TDSINVALID_MIB_SB_VALUE_TAG;

    tdsdeferred_sib_info.dsib.sib11.seg_index = 
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].seg_index;

    tdsdeferred_sib_info.dsib.sib11.sib_tot_length = 
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_tot_length;

    b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_data_ptr,  /* Source Pointer */
            0,    /* Source Offset is always 0 since we copy all the received bits */
           tdsdeferred_sib_info.dsib.sib11.sib_data_ptr, /* Destination Pointer */
           tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_tot_length,  /* Destination Offset */
           TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
          );
  }

  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_needed)&&
      !(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_decoded)
     )
  {
    if(tdsrrcsib_check_if_sib12_present_for_dbindex(dbindex)==TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: SIB12 not read");
      tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12] = TDSINVALID_MIB_SB_VALUE_TAG;

      tdsdeferred_sib_info.dsib.sib12.seg_index = 
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].seg_index;
    
      tdsdeferred_sib_info.dsib.sib12.sib_tot_length = 
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_tot_length;

      b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_data_ptr,  /* Source Pointer */
              0,    /* Source Offset is always 0 since we copy all the received bits */
             tdsdeferred_sib_info.dsib.sib12.sib_data_ptr, /* Destination Pointer */
             tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_tot_length,  /* Destination Offset */
             TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
            );

    }
  }

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_needed )&&
     !(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_decoded)
    )
  {
    if(tdsrrcsib_check_if_extension_sib_present(dbindex) == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: SIB11bis not read");
      tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] = TDSINVALID_MIB_SB_VALUE_TAG;
      if(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_data_ptr != NULL)
      {
        tdsdeferred_sib_info.dsib.sib11bis.seg_index = 
          tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].seg_index;
  
        tdsdeferred_sib_info.dsib.sib11bis.sib_tot_length = 
          tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_tot_length;
        
        b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_data_ptr,  /* Source Pointer */
                        0,    /* Source Offset is always 0 since we copy all the received bits */
                tdsdeferred_sib_info.dsib.sib11bis.sib_data_ptr, /* Destination Pointer */
                tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_tot_length,  /* Destination Offset */
                TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
              );
      }
    }
  }
#endif

  /* When the SIB event is completed, SIBs which are deferred but read needs to be copied 
     from sib_events_ptr and stored in deferred_sib_info DB */
  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_needed )&&
     !(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_decoded)
    )
  {
    if((tdsrrcsib_is_sib_scheduled( tdsrrc_SIB18,dbindex, FALSE,NULL) == TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: SIB18 not read");
      tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18] = TDSINVALID_MIB_SB_VALUE_TAG;

      tdsdeferred_sib_info.dsib.sib18.seg_index = 
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].seg_index;

      tdsdeferred_sib_info.dsib.sib18.sib_tot_length = 
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_tot_length;
      
      b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_data_ptr,  /* Source Pointer */
                      0,    /* Source Offset is always 0 since we copy all the received bits */
              tdsdeferred_sib_info.dsib.sib18.sib_data_ptr, /* Destination Pointer */
              tdssib_events_ptr->event[index].tsib[tdsrrc_SIB18].sib_tot_length,  /* Destination Offset */
              TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
            );
    }
  }

#ifdef FEATURE_TDSCDMA_SIB19
  if((tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_needed )&&
     !(tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_decoded)
    )
  {
    if(tdsrrcsib_check_if_sib19_present(dbindex) == TRUE)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: SIB19 not read");
      tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] = TDSINVALID_MIB_SB_VALUE_TAG;

      tdsdeferred_sib_info.dsib.sib19.seg_index = 
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].seg_index;

      tdsdeferred_sib_info.dsib.sib19.sib_tot_length = 
        tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_tot_length;
      
      b_copy( tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_data_ptr,  /* Source Pointer */
                      0,    /* Source Offset is always 0 since we copy all the received bits */
              tdsdeferred_sib_info.dsib.sib19.sib_data_ptr, /* Destination Pointer */
              tdssib_events_ptr->event[index].tsib[tdsrrc_SIB19].sib_tot_length,  /* Destination Offset */
              TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
            );
    }
  }

#endif

  if((tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11] == TDSINVALID_MIB_SB_VALUE_TAG)||
     (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB12] == TDSINVALID_MIB_SB_VALUE_TAG)||
     (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB11bis] == TDSINVALID_MIB_SB_VALUE_TAG) ||
#ifdef FEATURE_TDSCDMA_SIB19
     (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB19] == TDSINVALID_MIB_SB_VALUE_TAG) ||
#endif
     (tdssibdb.cell[dbindex].sib_value_tag[tdsrrc_SIB18] == TDSINVALID_MIB_SB_VALUE_TAG)
    )
  {
    tdsrrcsib_store_sib_seg_in_deferred_sib_db();
    tdsdeferred_sib_info.cell_id = tdssibdb.cell[dbindex].cell_id;
    tdsdeferred_sib_info.plmn_id = tdssibdb.cell[dbindex].plmn_id;
  }

return;
}

#endif

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
)
{
  uint32 count=0, dbindex=0;
  tdsrrc_csp_sib_mask_type  gen_mask=TDSMIB_BIT;
  tdsrrc_SIB_type sib_type=tdsrrc_MIB;

  if( (dbindex = tdsrrcsibdb_find_cell_index(plmn, cell)) == TDSSIBDB_INVALID_INDEX )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Match not found for given PLMN and cell id");
    return ;
  }

  if(!(TDSSIBDB_VALID_INDEX(dbindex)))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid database index given");
    return;
  }
  if( tdssibdb.cell[dbindex].index_used == FALSE )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unused database index given");
    return ;
  }

  for(count=0; count<TDSRRC_TOT_SIBS; count++, gen_mask <<= 1)
  {
    sib_type = tdsrrc_MAX_SIB;
    
    switch (req_sib_bit_mask & gen_mask)
    {
      case TDSMIB_BIT:
        sib_type = tdsrrc_MIB;
        break;

      case TDSSIB_1_BIT:
        sib_type = tdsrrc_SIB1;
        break;
       
      case TDSSIB_2_BIT:
        sib_type = tdsrrc_SIB2;
        break;

      case TDSSIB_3_BIT:
        sib_type = tdsrrc_SIB3;
        break;

      case TDSSIB_4_BIT:
        sib_type = tdsrrc_SIB4;
        break;

      case TDSSIB_5_BIT:
        sib_type = tdsrrc_SIB5;
        break;

      case TDSSIB_6_BIT:
        sib_type = tdsrrc_SIB6;
        break;

      case TDSSIB_7_BIT:
        sib_type = tdsrrc_SIB7;
        break;

      case TDSSIB_11_BIT:
        sib_type = tdsrrc_SIB11;
        break;

      case TDSSIB_12_BIT:
        sib_type = tdsrrc_SIB12;
        break;

      case TDSSIB_18_BIT:
        sib_type = tdsrrc_SIB18;
        break;

      case TDSSB_1_BIT:
        sib_type = tdsrrc_SB1;
        break;

      case TDSSB_2_BIT:
        sib_type = tdsrrc_SB2;
        break;

      case TDSSIB_5bis_BIT:
        sib_type = tdsrrc_SIB5bis;
        break;

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      case TDSSIB_11BIS_BIT:
        sib_type = tdsrrc_SIB11bis;
        break;
#endif

#ifdef FEATURE_TDSCDMA_SIB19
      case TDSSIB_19_BIT:
        sib_type = tdsrrc_SIB19;
        break;
#endif      

      default:
        sib_type = tdsrrc_MAX_SIB;
        break;
    }

    if(sib_type != tdsrrc_MAX_SIB)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalidate %d SIB v-tag",sib_type);
      tdssib_events_ptr->sib_sched[sib_type].repitition_rate = TDSINVALID_SIB_SCHEDULE_REP;
      tdssib_events_ptr->sib_sched[sib_type].num_segments = 0;
      tdssibdb.cell[dbindex].sib_value_tag[sib_type] = TDSINVALID_MIB_SB_VALUE_TAG;
      tdssibdb.cell[dbindex].sib_valid[sib_type] = FALSE;
      tdssibdb.cell[dbindex].six_hr_count = TDSMAX_CELL_INDEX_COUNT_SIXHR_CELL_SEL_RESEL;
    }
  }
}


#endif

/* LTETBD: enable this logging later on */
#if 0
/*===========================================================================

FUNCTION    rrc_log_sys_info_container 

DESCRIPTION
    This function will log rrc_System_Information_Container_PDU message into QXDM.
  
DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void rrc_log_sys_info_container
(
  rrc_cmd_type *cmd_ptr                 /* To hold received command */
)
{  
#ifndef T_WINNT
  uint32 log_record_length;                 /* Local variable to store Log 
                                               Record Length */
  WCDMA_SIGNALLING_MESSAGE_type *log_record_ptr;
                                            /* Local variable to store Log
                                               record */
  rrclog_sig_msg_event_type sig_msg_event;  /* Variable for message rx event */

#else
  #error code not present
#endif /* T_WINNT */ 
  

  uint32 dl_sdu_len = 0;
  uint8 rb_id     = BCCH_S_RADIO_BEARER_ID;
  uint8 msg_type  = RRCLOG_SIB_CONTAINER_COMPLETE_SIB;
  uint8 chan_type = RRCLOG_SIB_CONTAINER;

  //need 1 extra byte for signalling_message[0]

 dl_sdu_len = cmd_ptr->cmd.get_specific_sibs_req.sys_info_cont_ptr->cont_len + 1;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Logging rrc_System_Information_Container_PDU");

#ifdef T_WINNT
  #error code not present
#else  /* QXDM logging */

  /* Construct and send an event. */
  sig_msg_event.channel_type = chan_type;
  sig_msg_event.message_type = msg_type;
  event_report_payload(EVENT_RRC_MESSAGE_RECEIVED, sizeof(sig_msg_event),
                       (void *)&sig_msg_event);

  /*Calculate the record length */
  log_record_length = 
    FPOS(WCDMA_SIGNALLING_MESSAGE_type, signalling_message) + dl_sdu_len;

  /*Allocate memory for Log Record. */
  log_record_ptr = (WCDMA_SIGNALLING_MESSAGE_type *)log_alloc( WCDMA_SIGNALLING_MESSAGE, log_record_length);
  
  /* Make sure memory allocation is successful before to proceed with 
     logging */
  if(log_record_ptr != NULL)
  {
    /* Update RB Id.*/
    log_record_ptr->rb_id = rb_id;

    /*Update Logical Channel Type */
    log_record_ptr->channel_type = chan_type;

    /* Update length of the signalling message */
    log_record_ptr->length = dl_sdu_len;

    log_record_ptr->signalling_message[0] = msg_type;

    /* Copy the Signalling mesaage payload to the log record */
    /* memcpy((void *)&(log_record_ptr->signalling_message[1]), 
           (void *)(cmd_ptr->cmd.get_specific_sibs_req.sys_info_cont_ptr->cont_ptr),
           cmd_ptr->cmd.get_specific_sibs_req.sys_info_cont_ptr->cont_len); */

    /*commit the log record */
    log_commit(log_record_ptr);   
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"log_record_ptr is NULL");
    /* The QXDM is not turn ON. Hence Do not log the OTA messages */
  }
#endif /*T_WINNT */
} /* end function rrc_log_dl_sig_message */
#endif

/*===========================================================================

FUNCTION  rrcsib_check_nw_support_deferred_sib

DESCRIPTION

  This function will check the network functionality for deferred SIB11

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_check_nw_support_deferred_sib
(
  tdsrrc_System_Information_Container  *local_sys_info_cont_ptr
)
{
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  tdsrrc_SysInfoType3 *sib3_decoded = NULL;
#endif

  if((tdsrrc_csfb_call_status == TRUE)&&
      (tdsrrc_csfb_skip_sib11_opt_nv == TRUE))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB:Deferred SIB11 feature enabled for CSFB ");
    return TRUE;
  }

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
  sib3_decoded = &local_sys_info_cont_ptr->sysInfoType3;

  if(DEFERRED_MEAS_SUPPORT(sib3_decoded))
    return TRUE;
  else
#endif
    return FALSE;
}

/*===========================================================================

FUNCTION  rrcsib_reencode_sibs

DESCRIPTION
  This function will reencode SIBS from system information container. This is required to store sibs in encoded format.

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.
     
===========================================================================*/
void  tdsrrcsib_reencode_sibs
(
  tdsrrc_System_Information_Container    *local_sys_info_cont_ptr,       
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store  
)
{
  local_sys_info_cont_enc_store->mib_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->mib, tdsrrc_MasterInformationBlock_PDU);
  if(local_sys_info_cont_enc_store->mib_encoded.length == 0)
  {
    ERR_FATAL("ASN encoding of mib failed",0,0,0);
  }

  if(local_sys_info_cont_ptr->m.sysInfoTypeSB1Present)
  {
    local_sys_info_cont_enc_store->SB1_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoTypeSB1, tdsrrc_SysInfoTypeSB1_PDU);
    if(local_sys_info_cont_enc_store->SB1_encoded.length == 0)
    {
      ERR_FATAL("ASN encoding of sysInfoTypeSB1 failed",0,0,0);
    }
  }
  
  if(local_sys_info_cont_ptr->m.sysInfoTypeSB2Present)
  {
    local_sys_info_cont_enc_store->SB2_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoTypeSB2, tdsrrc_SysInfoTypeSB2_PDU);
    if(local_sys_info_cont_enc_store->SB2_encoded.length == 0)
    {
      ERR_FATAL("ASN encoding of sysInfoTypeSB2 failed",0,0,0);
    }
  }
  
  local_sys_info_cont_enc_store->sib1_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoType1, tdsrrc_SysInfoType1_PDU);
  if(local_sys_info_cont_enc_store->sib1_encoded.length == 0)
  {
    ERR_FATAL("ASN encoding of sysInfoType1 failed",0,0,0);
  }
    
  local_sys_info_cont_enc_store->sib3_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoType3, tdsrrc_SysInfoType3_PDU);
  if(local_sys_info_cont_enc_store->sib3_encoded.length == 0)
  {
    ERR_FATAL("ASN encoding of sysInfoType3 failed",0,0,0);
  }

  local_sys_info_cont_enc_store->sib5_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoType5, tdsrrc_SysInfoType5bis_PDU);
  if(local_sys_info_cont_enc_store->sib5_encoded.length == 0)
  {
    ERR_FATAL("ASN encoding of sysInfoType5 failed",0,0,0);
  }

  local_sys_info_cont_enc_store->sib7_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoType7, tdsrrc_SysInfoType7_PDU);
  if(local_sys_info_cont_enc_store->sib7_encoded.length == 0)
  {
    ERR_FATAL("ASN encoding of sysInfoType7 failed",0,0,0);
  }

  if(local_sys_info_cont_ptr->m.sysInfoType11Present)
  {
    local_sys_info_cont_enc_store->sib11_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoType11, tdsrrc_SysInfoType11_PDU);
    if(local_sys_info_cont_enc_store->sib11_encoded.length == 0)
    {
      ERR_FATAL("ASN encoding of sysInfoType11 failed",0,0,0);
    }
  }

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if(local_sys_info_cont_ptr->m.sysInfoType11bisPresent)
  {
    local_sys_info_cont_enc_store->sib11bis_encoded = tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoType11bis, tdsrrc_SysInfoType11bis_PDU);
    if(local_sys_info_cont_enc_store->sib11bis_encoded.length == 0)
    {
      ERR_FATAL("ASN encoding of sysInfoType11bis failed",0,0,0);
    }
  } 
#endif

  if (local_sys_info_cont_ptr->m.sysInfoType12Present )
  {
    local_sys_info_cont_enc_store->sib12_encoded =  tdsrrcasn1_encode_pdu(&local_sys_info_cont_ptr->sysInfoType12, tdsrrc_SysInfoType12_PDU);
    if(local_sys_info_cont_enc_store->sib12_encoded.length == 0)
    {
      ERR_FATAL("ASN encoding of sysInfoType12 failed",0,0,0);
    }
  }

  return ; 
}

/*===========================================================================

FUNCTION  rrcsib_free_rencoded_sibs

DESCRIPTION

  This function will read the SIBS from system information container

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_free_rencoded_sibs
(
  tdsrrc_System_Information_Container *local_sys_info_cont_ptr,   /* Pointer to received SIB event */
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store
)
{
  if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_MasterInformationBlock_PDU,(void *)local_sys_info_cont_enc_store->mib_encoded.value))
  {
    ERR_FATAL("Unable to free the free mib PDU",0,0,0);
  }
  local_sys_info_cont_enc_store->mib_encoded.value = NULL;
  local_sys_info_cont_enc_store->mib_encoded.length = 0;

  if(local_sys_info_cont_ptr->m.sysInfoTypeSB1Present)  
  {
    if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoTypeSB1_PDU,(void *)local_sys_info_cont_enc_store->SB1_encoded.value))
    {
      ERR_FATAL("Unable to free the free sysInfoTypeSB1 PDU",0,0,0);
    }
    local_sys_info_cont_enc_store->SB1_encoded.value = NULL;
    local_sys_info_cont_enc_store->SB1_encoded.length = 0;
  }

  if(local_sys_info_cont_ptr->m.sysInfoTypeSB2Present)  
  {
    if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoTypeSB2_PDU,(void *)local_sys_info_cont_enc_store->SB2_encoded.value))
    {
      ERR_FATAL("Unable to free the free sysInfoTypeSB2 PDU",0,0,0);
    }
    local_sys_info_cont_enc_store->SB2_encoded.value = NULL;
    local_sys_info_cont_enc_store->SB2_encoded.length = 0;
  }

  if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoType1_PDU,(void *)local_sys_info_cont_enc_store->sib1_encoded.value))
  {
    ERR_FATAL("Unable to free the free sysInfoType1 PDU",0,0,0);
  }
  local_sys_info_cont_enc_store->sib1_encoded.value = NULL;
  local_sys_info_cont_enc_store->sib1_encoded.length = 0;
  
  if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoType3_PDU,(void *)local_sys_info_cont_enc_store->sib3_encoded.value))
  {
    ERR_FATAL("Unable to free the free sysInfoType3 PDU",0,0,0);
  }
  local_sys_info_cont_enc_store->sib3_encoded.value = NULL;
  local_sys_info_cont_enc_store->sib3_encoded.length = 0;

  if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoType5bis_PDU,(void *)local_sys_info_cont_enc_store->sib5_encoded.value))
  {
    ERR_FATAL("Unable to free the free sysInfoType5 PDU",0,0,0);
  }
  local_sys_info_cont_enc_store->sib5_encoded.value = NULL;
  local_sys_info_cont_enc_store->sib5_encoded.length = 0;

  if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoType7_PDU,(void *)local_sys_info_cont_enc_store->sib7_encoded.value))
  {
    ERR_FATAL("Unable to free the free sysInfoType7 PDU",0,0,0);
  }
  local_sys_info_cont_enc_store->sib7_encoded.value = NULL;
  local_sys_info_cont_enc_store->sib7_encoded.length = 0;

  if(local_sys_info_cont_ptr->m.sysInfoType11Present)
  {
    if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoType11_PDU,(void *)local_sys_info_cont_enc_store->sib11_encoded.value))
    {
      ERR_FATAL("Unable to free the free sysInfoType11 PDU",0,0,0);
    }
    local_sys_info_cont_enc_store->sib11_encoded.value = NULL;
    local_sys_info_cont_enc_store->sib11_encoded.length = 0;
  }
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if(local_sys_info_cont_ptr->m.sysInfoType11bisPresent )
  {
    if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoType11bis_PDU,(void *)local_sys_info_cont_enc_store->sib11bis_encoded.value))
    {
      ERR_FATAL("Unable to free the free sysInfoType11bis PDU",0,0,0);
    }
    local_sys_info_cont_enc_store->sib11bis_encoded.value = NULL;
    local_sys_info_cont_enc_store->sib11bis_encoded.length = 0;
  } 
#endif

  if (local_sys_info_cont_ptr->m.sysInfoType12Present )
  {
    if(tdsrrcasn1_free_re_encoded_sibs(tdsrrc_SysInfoType12_PDU,(void *)local_sys_info_cont_enc_store->sib12_encoded.value))
    {
      ERR_FATAL("Unable to free the free sysInfoType12 PDU",0,0,0);
    }
    local_sys_info_cont_enc_store->sib12_encoded.value = NULL;
    local_sys_info_cont_enc_store->sib12_encoded.length = 0;
  }
}

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
boolean tdsrrcsib_validate_sys_info_container
(
  tdsrrc_System_Information_Container *local_sys_info_cont_ptr   /* Pointer to received SIB event */
)
{
  boolean ret_val = TRUE;


  /* Check to see if PLMN Type is not correct here. */
  if(!( (local_sys_info_cont_ptr->mib.plmn_Type.t == T_tdsrrc_PLMN_Type_gsm_MAP)||
        (local_sys_info_cont_ptr->mib.plmn_Type.t == T_tdsrrc_PLMN_Type_gsm_MAP_and_ANSI_41)
      )
    )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB: Acquired cell PLMN not GSM-MAP!");
    ret_val = FALSE;
  }

  /* 25.331, 10.2.48.8.8. When UE receives SIB5 in the System Information Container message, this IE
     shall always be interpreted as FALSE */
  local_sys_info_cont_ptr->sysInfoType5.sib6indicator = 0;

  if(tdsrrccsp_check_sib1_validity(&local_sys_info_cont_ptr->sysInfoType1) == FALSE)
  {
    ret_val = FALSE;
  }

  return ret_val;
}

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
uint32 tdsrrcsib_get_sibdb_index_for_sys_info_container
(
  tdsrrc_System_Information_Container  *local_sys_info_cont_ptr,
  tdssib_events_index_e_type event_index
)
{
  rrc_plmn_identity_type plmn;
  tdsrrc_int_cell_id_type cell;   
  uint32 sib_db_index = TDSSIBDB_INVALID_INDEX;
  
  /* Initialize PLMN to dummy value to keep lint happy */
  plmn.mcc[0] = 0;
  plmn.mcc[1] = 0;
  plmn.mcc[2] = 0;
  plmn.num_mnc_digits = 3;
  plmn.mnc[0] = 0;
  plmn.mnc[1] = 0;
  plmn.mnc[2] = 0;

  /* Get the plmn id for this cell. Make sure the PLMN id is a GSM-MAP PLMN. 
     * Convert it to the internal format used by RRC. */
  if(local_sys_info_cont_ptr->mib.plmn_Type.t == T_tdsrrc_PLMN_Type_gsm_MAP)
  {
    tdsrrc_translate_gsm_map_plmn_id(
              &(local_sys_info_cont_ptr->mib.plmn_Type.u.gsm_MAP->plmn_Identity),
              &plmn
              );
  }
  else if(local_sys_info_cont_ptr->mib.plmn_Type.t == T_tdsrrc_PLMN_Type_gsm_MAP_and_ANSI_41)
  {
    tdsrrc_translate_gsm_map_plmn_id(
              &(local_sys_info_cont_ptr->mib.plmn_Type.u.gsm_MAP_and_ANSI_41->plmn_Identity),
              &plmn
              );
  }
  
  cell = tdsrrc_translate_cell_id(&(local_sys_info_cont_ptr->sysInfoType3.cellIdentity));
  
  /* Get the SIB Database index for this cell */
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSFB: Checking SIBDB index CellId %d and MCC %d MNC %d", cell, 
        TDSRRC_CSP_GET_INT_MCC(plmn), 
        TDSRRC_CSP_GET_INT_MNC(plmn));
  
  sib_db_index = tdsrrcsibdb_create_cell_index(plmn, cell, event_index);

  return sib_db_index;
}

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
void tdsrrcsib_update_tsib_with_acq_sibs
(
  tdsrrc_System_Information_Container    *local_sys_info_cont_ptr,
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,
  tdssib_events_index_e_type index
)
{

  memscpy(
    tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_data_ptr,
    TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
    local_sys_info_cont_enc_store->mib_encoded.value,
    local_sys_info_cont_enc_store->mib_encoded.length
    );
  tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_needed = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_decoded = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].seg_count = 0;
  tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
  tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_tot_length = 0;
  
  if(local_sys_info_cont_ptr->m.sysInfoTypeSB1Present)
    {
      memscpy(
        tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_data_ptr,
        TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
        local_sys_info_cont_enc_store->SB1_encoded.value,
        local_sys_info_cont_enc_store->SB1_encoded.length
        );
      tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed = TRUE;
      tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_received = TRUE;
      tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_decoded = TRUE;
      tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].seg_count = 0;
      tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
      tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_tot_length = 0;
    }

  if(local_sys_info_cont_ptr->m.sysInfoTypeSB2Present)
  {
    memscpy(
      tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_data_ptr,
      TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
      local_sys_info_cont_enc_store->SB2_encoded.value,
      local_sys_info_cont_enc_store->SB2_encoded.length
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_received = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].seg_count = 0;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_tot_length = 0;
  }
  
  memscpy(
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_data_ptr,
    TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
    local_sys_info_cont_enc_store->sib1_encoded.value,
    local_sys_info_cont_enc_store->sib1_encoded.length
    );
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_needed = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_received = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_decoded = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].seg_count = 0;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_tot_length = 0;

  memscpy(
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_data_ptr,
    TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
    local_sys_info_cont_enc_store->sib3_encoded.value,
    local_sys_info_cont_enc_store->sib3_encoded.length
    );
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_needed = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_received = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_decoded = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].seg_count = 0;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_tot_length = 0;


  memscpy(
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_data_ptr,
    TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
    local_sys_info_cont_enc_store->sib5_encoded.value,
    local_sys_info_cont_enc_store->sib5_encoded.length
    );
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_needed = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_received = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_decoded = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].seg_count = 0;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_tot_length = 0;


  memscpy(
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_data_ptr,
    TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
    local_sys_info_cont_enc_store->sib7_encoded.value,
    local_sys_info_cont_enc_store->sib7_encoded.length
    );
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_needed = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_received = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_decoded = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].seg_count = 0;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
  tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_tot_length = 0;
  
  if(local_sys_info_cont_ptr->m.sysInfoType11Present)
  {
    memscpy(
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_data_ptr,
      TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
      local_sys_info_cont_enc_store->sib11_encoded.value,
      local_sys_info_cont_enc_store->sib11_encoded.length
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_needed = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_received = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].seg_count = 0;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_tot_length = 0;
  }

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if(local_sys_info_cont_ptr->m.sysInfoType11bisPresent )
  {
    memscpy(
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_data_ptr,
      TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
      local_sys_info_cont_enc_store->sib11bis_encoded.value,
      local_sys_info_cont_enc_store->sib11bis_encoded.length
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_needed = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_received = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].seg_count = 0;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_tot_length = 0;
  }
#endif

  if(local_sys_info_cont_ptr->m.sysInfoType12Present )
  {
    memscpy(
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_data_ptr,
      TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
      local_sys_info_cont_enc_store->sib12_encoded.value,
      local_sys_info_cont_enc_store->sib12_encoded.length
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_needed = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_received = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].seg_count = 0;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].seg_index = (TDSMAX_SEG_INDEX_FOR_A_SIB + 1);
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_tot_length = 0;
  }
  
}


/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_mib_info

DESCRIPTION

  This function will update SIBDB for MIB

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_mib_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
    /* Update MIB in SIBDB */
    memset(tdssibdb.cell[sib_db_index].sibs.mib_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
    memscpy(tdssibdb.cell[sib_db_index].sibs.mib_encoded_ptr,
           TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
           local_sys_info_cont_enc_store->mib_encoded.value,
           local_sys_info_cont_enc_store->mib_encoded.length);
    
    tdssibdb.cell[sib_db_index].sibs.mib_encoded_pdu_length = local_sys_info_cont_enc_store->mib_encoded.length ;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved MIB type=%d len=%d(bytes) in encoded form",tdsrrc_MasterInformationBlock_PDU,local_sys_info_cont_enc_store->mib_encoded.length);
     /* Update MIB data structures */
    if(tdssibdb.cell[sib_db_index].sibs.mib != NULL)
    {
      tdsrrcsibdb_free_sib(tdsrrc_MasterInformationBlock_PDU, tdssibdb.cell[sib_db_index].sibs.mib );
    }
    tdssibdb.cell[sib_db_index].sibs.mib =  tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[sib_db_index].sibs.mib_encoded_ptr), /* Encoded SIB      */
    tdsrrc_MasterInformationBlock_PDU,                                                       /* SIB's PDU number */
    tdssibdb.cell[sib_db_index].sibs.mib_encoded_pdu_length   /* SIB's Length in bytes */
    );

    tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_needed = FALSE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_MIB].sib_received = FALSE;
    tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_MIB] = TRUE;
    tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_MIB] = TDSINVALID_MIB_SB_VALUE_TAG;
}

/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_SB1_info

DESCRIPTION

  This function will update SIBDB for SB1

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_SB1_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
  /* Update SB1 in SIBDB */
  memset(tdssibdb.cell[sib_db_index].sibs.sb1_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
  memscpy(tdssibdb.cell[sib_db_index].sibs.sb1_encoded_ptr,
         TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
         local_sys_info_cont_enc_store->SB1_encoded.value,
         local_sys_info_cont_enc_store->SB1_encoded.length);
  
  tdssibdb.cell[sib_db_index].sibs.sb1_encoded_pdu_length = local_sys_info_cont_enc_store->SB1_encoded.length ;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SB1 type=%d len=%d(bytes) in encoded form",tdsrrc_SysInfoTypeSB1_PDU,local_sys_info_cont_enc_store->SB1_encoded.length);
   /* Update SB1 data structures */
  if(tdssibdb.cell[sib_db_index].sibs.sb1 != NULL)
  {
    tdsrrcsibdb_free_sib(tdsrrc_SysInfoTypeSB1_PDU, tdssibdb.cell[sib_db_index].sibs.sb1 );
  }
  tdssibdb.cell[sib_db_index].sibs.sb1 = tdsrrcasn1_decode_pdu( 
  (void *)(tdssibdb.cell[sib_db_index].sibs.sb1_encoded_ptr), /* Encoded SIB      */
  tdsrrc_SysInfoTypeSB1_PDU,                                                       /* SIB's PDU number */
  tdssibdb.cell[sib_db_index].sibs.sb1_encoded_pdu_length   /* SIB's Length in bytes */
  );

  tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_decoded = TRUE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_needed = FALSE;
  tdssib_events_ptr->event[index].tsib[tdsrrc_SB1].sib_received = FALSE;
  tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SB1] = TRUE;
  tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SB1] = TDSINVALID_MIB_SB_VALUE_TAG;
}

/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_SB2_info

DESCRIPTION

  This function will update SIBDB for SB2

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_SB2_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,  
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
    /* Update SB2 in SIBDB */
    memset(tdssibdb.cell[sib_db_index].sibs.sb2_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
    memscpy(tdssibdb.cell[sib_db_index].sibs.sb2_encoded_ptr,
           TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
           local_sys_info_cont_enc_store->SB2_encoded.value,
           local_sys_info_cont_enc_store->SB2_encoded.length);
    
    tdssibdb.cell[sib_db_index].sibs.sb2_encoded_pdu_length = local_sys_info_cont_enc_store->SB2_encoded.length;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SB2 type=%d len=%d(bytes) in encoded form",tdsrrc_SysInfoTypeSB2_PDU,local_sys_info_cont_enc_store->SB2_encoded.length);
     /* Update SB2 data structures */
    if(tdssibdb.cell[sib_db_index].sibs.sb2 != NULL)
    {
      tdsrrcsibdb_free_sib(tdsrrc_SysInfoTypeSB2_PDU, tdssibdb.cell[sib_db_index].sibs.sb2 );
    }
    tdssibdb.cell[sib_db_index].sibs.sb2 = tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[sib_db_index].sibs.sb2_encoded_ptr), /* Encoded SIB      */
    tdsrrc_SysInfoTypeSB2_PDU,                                                       /* SIB's PDU number */
    tdssibdb.cell[sib_db_index].sibs.sb2_encoded_pdu_length   /* SIB's Length in bytes */
    );
    
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_needed = FALSE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SB2].sib_received = FALSE;
    tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SB2] = TRUE;
    tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SB2] = TDSINVALID_MIB_SB_VALUE_TAG;
}

/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_sib1_info

DESCRIPTION

  This function will update SIBDB for SIB1

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_sib1_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,  
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
    tdsrrc_SysInfoType1 *sib1_ptr = NULL;
    /* Update MIB in SIBDB */
    memset(tdssibdb.cell[sib_db_index].sibs.sib1_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
    memscpy(tdssibdb.cell[sib_db_index].sibs.sib1_encoded_ptr,
           TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
           local_sys_info_cont_enc_store->sib1_encoded.value,
           local_sys_info_cont_enc_store->sib1_encoded.length);
    
    tdssibdb.cell[sib_db_index].sibs.sib1_encoded_pdu_length = local_sys_info_cont_enc_store->sib1_encoded.length;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SIB1 type=%d len=%d(bytes) in encoded form",tdsrrc_SysInfoType1_PDU,local_sys_info_cont_enc_store->sib1_encoded.length);
     /* Update SIB1 data structures */
    if(tdssibdb.cell[sib_db_index].sibs.sib1 != NULL)
    {
      tdsrrcsibdb_free_sib(tdsrrc_SysInfoType1_PDU, tdssibdb.cell[sib_db_index].sibs.sib1 );
    }
    tdssibdb.cell[sib_db_index].sibs.sib1 = tdsrrcasn1_decode_pdu( 
    (void *)(tdssibdb.cell[sib_db_index].sibs.sib1_encoded_ptr), /* Encoded SIB      */
    tdsrrc_SysInfoType1_PDU,                                                       /* SIB's PDU number */
    tdssibdb.cell[sib_db_index].sibs.sib1_encoded_pdu_length   /* SIB's Length in bytes */
    );

	/* set default values for idle timers and constants in case IE IdleTimersAndConstants is absent in SIB1 */
	sib1_ptr = tdssibdb.cell[sib_db_index].sibs.sib1;

	if(sib1_ptr && (!sib1_ptr->m.ue_IdleTimersAndConstantsPresent))
	{
		sib1_ptr->m.ue_IdleTimersAndConstantsPresent = 1;
		sib1_ptr->ue_IdleTimersAndConstants.t_300 = tdsrrc_T_300_ms1000;
		sib1_ptr->ue_IdleTimersAndConstants.n_300 = 3;
		sib1_ptr->ue_IdleTimersAndConstants.t_312 = 1;
		sib1_ptr->ue_IdleTimersAndConstants.n_312 = 1;		
	}

    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_needed = FALSE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB1].sib_received = FALSE;
    tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SIB1] = TRUE;
    tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SIB1] = TDSINVALID_MIB_SB_VALUE_TAG;
}

/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_sib3_info

DESCRIPTION

  This function will update SIBDB for SIB3

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_sib3_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,  
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
      /* Update SIB3 in SIBDB */
      memset(tdssibdb.cell[sib_db_index].sibs.sib3_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
      memscpy(tdssibdb.cell[sib_db_index].sibs.sib3_encoded_ptr,
             TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
             local_sys_info_cont_enc_store->sib3_encoded.value,
             local_sys_info_cont_enc_store->sib3_encoded.length);
      
      tdssibdb.cell[sib_db_index].sibs.sib3_encoded_pdu_length = local_sys_info_cont_enc_store->sib3_encoded.length ;

      TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SIB3 type=%d,len=%d(bytes) in encoded form",tdsrrc_SysInfoType3_PDU,local_sys_info_cont_enc_store->sib3_encoded.length);
      /* Update SIB3 data structures */
      if(tdssibdb.cell[sib_db_index].sibs.sib3 != NULL)
      {
        tdsrrcsibdb_free_sib(tdsrrc_SysInfoType3_PDU, tdssibdb.cell[sib_db_index].sibs.sib3 );
      }
      tdssibdb.cell[sib_db_index].sibs.sib3 = tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[sib_db_index].sibs.sib3_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType3_PDU,                                                       /* SIB's PDU number */
      tdssibdb.cell[sib_db_index].sibs.sib3_encoded_pdu_length   /* SIB's Length in bytes */
      );
      
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_decoded = TRUE;
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_needed = FALSE;
      tdssib_events_ptr->event[index].tsib[tdsrrc_SIB3].sib_received = FALSE;
      tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SIB3] = TRUE;
      tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SIB3] = TDSINVALID_MIB_SB_VALUE_TAG;
}

/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_sib5_info

DESCRIPTION

  This function will update SIBDB for SIB5

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_sib5_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,  
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
    /* Update MIB in SIBDB */
    memset(tdssibdb.cell[sib_db_index].sibs.sib5_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
    memscpy(tdssibdb.cell[sib_db_index].sibs.sib5_encoded_ptr,
           TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
           local_sys_info_cont_enc_store->sib5_encoded.value,
           local_sys_info_cont_enc_store->sib5_encoded.length);
    
    tdssibdb.cell[sib_db_index].sibs.sib5_encoded_pdu_length = local_sys_info_cont_enc_store->sib5_encoded.length ;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SIB5 type=%d len=%d(bytes) in encoded form",tdsrrc_SysInfoType5bis_PDU,local_sys_info_cont_enc_store->sib5_encoded.length);
     /* Update SIB5 data structures */
    if(tdssibdb.cell[sib_db_index].sibs.sib5 != NULL)
    {
      tdsrrcsibdb_free_sib(tdsrrc_SysInfoType5bis_PDU, tdssibdb.cell[sib_db_index].sibs.sib5);
    }
    tdssibdb.cell[sib_db_index].sibs.sib5 = tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[sib_db_index].sibs.sib5_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType5bis_PDU,                                                       /* SIB's PDU number */
      tdssibdb.cell[sib_db_index].sibs.sib5_encoded_pdu_length   /* SIB's Length in bytes */
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_needed = FALSE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB5].sib_received = FALSE;
    tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SIB5] = TRUE;
    tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SIB5] = TDSINVALID_MIB_SB_VALUE_TAG;
}
/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_sib7_info

DESCRIPTION

  This function will update SIBDB for SIB7

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_sib7_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,  
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
    /* Update MIB in SIBDB */
    memset(tdssibdb.cell[sib_db_index].sibs.sib7_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
    memscpy(tdssibdb.cell[sib_db_index].sibs.sib7_encoded_ptr,
           TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
           local_sys_info_cont_enc_store->sib7_encoded.value,
           local_sys_info_cont_enc_store->sib7_encoded.length);
    
    tdssibdb.cell[sib_db_index].sibs.sib7_encoded_pdu_length = local_sys_info_cont_enc_store->sib7_encoded.length ;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SIB7 type=%d len=%d(bytes) in encoded form",tdsrrc_SysInfoType7_PDU,local_sys_info_cont_enc_store->sib7_encoded.length);
     /* Update SIB7 data structures */
    if(tdssibdb.cell[sib_db_index].sibs.sib7 != NULL)
    {
      tdsrrcsibdb_free_sib(tdsrrc_SysInfoType7_PDU, tdssibdb.cell[sib_db_index].sibs.sib7);
    }
    tdssibdb.cell[sib_db_index].sibs.sib7 =  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[sib_db_index].sibs.sib7_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType7_PDU,                                                       /* SIB's PDU number */
      tdssibdb.cell[sib_db_index].sibs.sib7_encoded_pdu_length   /* SIB's Length in bytes */
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_needed = FALSE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB7].sib_received = FALSE;
    tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SIB7] = TRUE;
    tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SIB7] = TDSINVALID_MIB_SB_VALUE_TAG;
}
/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_sib11_info

DESCRIPTION

  This function will update SIBDB for SIB11

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_sib11_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,  
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
    /* Update MIB in SIBDB */
    memset(tdssibdb.cell[sib_db_index].sibs.sib11_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
    memscpy(tdssibdb.cell[sib_db_index].sibs.sib11_encoded_ptr,
           TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
           local_sys_info_cont_enc_store->sib11_encoded.value,
           local_sys_info_cont_enc_store->sib11_encoded.length);
    
    tdssibdb.cell[sib_db_index].sibs.sib11_encoded_pdu_length = local_sys_info_cont_enc_store->sib11_encoded.length ;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SIB11 type=%d len=%d(bytes) in encoded form",tdsrrc_SysInfoType11_PDU,local_sys_info_cont_enc_store->sib11_encoded.length);
     /* Update MIB data structures */
    if(tdssibdb.cell[sib_db_index].sibs.sib11 != NULL)
    {
      tdsrrcsibdb_free_sib(tdsrrc_SysInfoType11_PDU, tdssibdb.cell[sib_db_index].sibs.sib11);
    }
    tdssibdb.cell[sib_db_index].sibs.sib11 =  tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[sib_db_index].sibs.sib11_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType11_PDU,                                                       /* SIB's PDU number */
      tdssibdb.cell[sib_db_index].sibs.sib11_encoded_pdu_length   /* SIB's Length in bytes */
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_needed = FALSE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11].sib_received = FALSE;
    tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SIB11] = TRUE;
    tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SIB11] = TDSINVALID_MIB_SB_VALUE_TAG;
}

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_sib11Bis_info

DESCRIPTION

  This function will update SIBDB for SIB11Bis

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/

void tdsrrcsib_update_sibdb_with_sib11Bis_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,  
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
    /* Update MIB in SIBDB */
    memset(tdssibdb.cell[sib_db_index].sibs.sib11bis_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
    memscpy(tdssibdb.cell[sib_db_index].sibs.sib11bis_encoded_ptr,
           TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
           local_sys_info_cont_enc_store->sib11bis_encoded.value,
           local_sys_info_cont_enc_store->sib11bis_encoded.length);
    
    tdssibdb.cell[sib_db_index].sibs.sib11bis_encoded_pdu_length = local_sys_info_cont_enc_store->sib11bis_encoded.length ;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SIB11Bis type=%d len=%d(bytes) in encoded form",tdsrrc_SysInfoType11bis_PDU,local_sys_info_cont_enc_store->sib11bis_encoded.length);
     /* Update MIB data structures */
    if(tdssibdb.cell[sib_db_index].sibs.sib11bis != NULL)
    {
      tdsrrcsibdb_free_sib(tdsrrc_SysInfoType11bis_PDU, tdssibdb.cell[sib_db_index].sibs.sib11bis);
    }
    tdssibdb.cell[sib_db_index].sibs.sib11bis = tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[sib_db_index].sibs.sib11bis_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType11bis_PDU,                                                       /* SIB's PDU number */
      tdssibdb.cell[sib_db_index].sibs.sib11bis_encoded_pdu_length   /* SIB's Length in bytes */
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_needed = FALSE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB11bis].sib_received = FALSE;
    tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SIB11bis] = TRUE;
    tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SIB11bis] = TDSINVALID_MIB_SB_VALUE_TAG;
}

#endif
/*===========================================================================

FUNCTION  rrcsib_update_sibdb_with_sib12_info

DESCRIPTION

  This function will update SIBDB for SIB12

DEPENDENCIES
  None.

RETURN VALUE
  True: if present
  False: else

SIDE EFFECTS
  None.
     
===========================================================================*/
void tdsrrcsib_update_sibdb_with_sib12_info
(
  tdsrrcsib_system_info_cont_store_type  *local_sys_info_cont_enc_store,  
  uint32 sib_db_index,
  tdssib_events_index_e_type index
)
{
    /* Update MIB in SIBDB */
    memset(tdssibdb.cell[sib_db_index].sibs.sib12_encoded_ptr, 0, TDSMAX_SIB_LENGTH_BYTES*sizeof(byte));
    memscpy(tdssibdb.cell[sib_db_index].sibs.sib12_encoded_ptr,
           TDSMAX_SIB_LENGTH_BYTES*sizeof(byte),
           local_sys_info_cont_enc_store->sib12_encoded.value,
           local_sys_info_cont_enc_store->sib12_encoded.length);
    
    tdssibdb.cell[sib_db_index].sibs.sib12_encoded_pdu_length = local_sys_info_cont_enc_store->sib12_encoded.length ;

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"CSFB: Saved SIB12 type=%d len=%d(bytes) in encoded form",tdsrrc_SysInfoType12_PDU,local_sys_info_cont_enc_store->sib12_encoded.length);
     /* Update SIB12 data structures */
    if(tdssibdb.cell[sib_db_index].sibs.sib12 != NULL)
    {
      tdsrrcsibdb_free_sib(tdsrrc_SysInfoType12_PDU, tdssibdb.cell[sib_db_index].sibs.sib12);
    }
    tdssibdb.cell[sib_db_index].sibs.sib12 = tdsrrcasn1_decode_pdu( 
      (void *)(tdssibdb.cell[sib_db_index].sibs.sib12_encoded_ptr), /* Encoded SIB      */
      tdsrrc_SysInfoType12_PDU,                                                       /* SIB's PDU number */
      tdssibdb.cell[sib_db_index].sibs.sib12_encoded_pdu_length   /* SIB's Length in bytes */
      );
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_decoded = TRUE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_needed = FALSE;
    tdssib_events_ptr->event[index].tsib[tdsrrc_SIB12].sib_received = FALSE;
    tdssibdb.cell[sib_db_index].sib_valid[tdsrrc_SIB12] = TRUE;
    tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SIB12] = TDSINVALID_MIB_SB_VALUE_TAG;
}

/*===========================================================================

FUNCTION  rrcsib_read_sibs_from_sys_info_container

DESCRIPTION

  This function will read the SIBS from system information container

DEPENDENCIES
  None.

RETURN VALUE
  boolean

SIDE EFFECTS
  None.
     
===========================================================================*/
boolean tdsrrcsib_read_sibs_from_sys_info_container
(
  tdsrrc_cmd_type *cmd_ptr   /* Pointer to received SIB event */
)
{
  tdsrrc_System_Information_Container  *local_sys_info_cont_ptr = NULL;
  tdssib_events_index_e_type index=0;  
  uint32 sib_db_index=TDSSIBDB_INVALID_INDEX;
  rrc_plmn_identity_type plmn;
  tdsrrc_int_cell_id_type cell = 0;
  tdsrrcsib_bcch_setup_status_e_type bcch_status;
  uint8 v_tag_idx = 0;
  tdsrrcsib_system_info_cont_store_type local_sys_info_cont_enc_store; 
  uint8 count = 0;

  /* Initialize PLMN to dummy value to keep lint happy */
  plmn.mcc[0] = 0;
  plmn.mcc[1] = 0;
  plmn.mcc[2] = 0;
  plmn.num_mnc_digits = 3;
  plmn.mnc[0] = 0;
  plmn.mnc[1] = 0;
  plmn.mnc[2] = 0;



  /*Freeing decoded SIBs if GET_SPECIFIC_SIB_REQ for serving cell
         is received without camping after successful SIB reading, in order
         to avoid exhaustion of decoded contexts*/
  if((cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE) && 
     (cmd_ptr->cmd.get_specific_sibs_req.procedure == TDSRRC_PROCEDURE_CSP))
  {
    for(count = 0; count <(uint8)TDSSIBDB_INVALID_INDEX; count++)
    {
      if(tdssibdb.cell[count].index_used == TRUE)
      {
        TDSRRC_PRINT_SIB_HEAP();
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Free ASN.1 decoded sib's for sibdb idx= %d",count);
        tdsrrcsibdb_free_all_decodedsibs_for_a_cell(count);
        TDSRRC_PRINT_SIB_HEAP();
      }
    }
  } 

  if(cmd_ptr->cmd.get_specific_sibs_req.for_serv_cell == TRUE)
    index = TDSSIB_SRV_INDEX;
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not valid for neighor cells");
    cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_NORMAL;
    return FALSE;
  }

  /* LTETBD: enable logging later on */
  /* rrc_log_sys_info_container(cmd_ptr); */

  /* Process System Information Container */
  local_sys_info_cont_ptr = (tdsrrc_System_Information_Container *)tdsrrcasn1_decode_pdu( 
    (void *)cmd_ptr->cmd.get_specific_sibs_req.sys_info_cont_ptr->cont_ptr, 
    tdsrrc_System_Information_Container_PDU,                      
    cmd_ptr->cmd.get_specific_sibs_req.sys_info_cont_ptr->cont_len);          

  if(local_sys_info_cont_ptr == NULL)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"rrc_System_Information_Container_PDU decoding failed");
    cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_NORMAL;
    return FALSE;
  }
  
  /* Validate System Information Container for mandatory SIBS */
  if (FALSE == tdsrrcsib_validate_sys_info_container(local_sys_info_cont_ptr))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Not valid SIB Container");    
    cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_NORMAL;
    if(tdsrrcasn1_free_pdu(tdsrrc_System_Information_Container_PDU,(void *)local_sys_info_cont_ptr))
    {
      ERR_FATAL("Unable to free the rrc_System_Information_Container_PDU",0,0,0);
    }    
    return FALSE;    
  }

  /* If MIB and SIB3 decoding is successful, update SIBDB 
     * Get the SIBDB index and update MIB and SIB3 information in SIBDB */
  sib_db_index = tdsrrcsib_get_sibdb_index_for_sys_info_container(local_sys_info_cont_ptr, index);

  if(sib_db_index == TDSSIBDB_INVALID_INDEX)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Invalid SIBDB index");
    cmd_ptr->cmd.get_specific_sibs_req.sib_scan_type = TDSRRC_SIB_NORMAL;
    if(tdsrrcasn1_free_pdu(tdsrrc_System_Information_Container_PDU,(void *)local_sys_info_cont_ptr))
    {
      ERR_FATAL("Unable to free the decoded PDU",0,0,0);
    }
    return FALSE;
  }

    memset(&local_sys_info_cont_enc_store, 0, sizeof(tdsrrcsib_system_info_cont_store_type));
    tdsrrcsib_reencode_sibs(local_sys_info_cont_ptr, &local_sys_info_cont_enc_store);

    tdssib_events_ptr->event[index].for_serving_cell = TRUE;
    tdssib_events_ptr->event[index].sib_scan_type = TDSRRC_SIB_READ_FROM_SIB_CONTAINER;
    tdssib_events_ptr->event[index].event_name = TDSRRCSIB_GET_SPECIFIC_SIBS;
    tdssib_events_ptr->event[index].req_proc = TDSRRC_PROCEDURE_CSP;
    tdssib_events_ptr->event[index].sib_db_index_found = TRUE;
    tdssib_events_ptr->event[index].sib_db_index = sib_db_index;

    tdsrrcsib_update_sibdb_with_mib_info(&local_sys_info_cont_enc_store, sib_db_index, index);  
    tdsrrcsib_update_sibdb_with_sib3_info(&local_sys_info_cont_enc_store, sib_db_index, index); 

    if(local_sys_info_cont_ptr->m.sysInfoTypeSB1Present)
    {
      tdsrrcsib_update_sibdb_with_SB1_info(&local_sys_info_cont_enc_store, sib_db_index, index); 
    }

    if(local_sys_info_cont_ptr->m.sysInfoTypeSB2Present)
    {
      tdsrrcsib_update_sibdb_with_SB2_info(&local_sys_info_cont_enc_store, sib_db_index, index); 
    }
    
    tdsrrcsib_update_sibdb_with_sib1_info(&local_sys_info_cont_enc_store, sib_db_index, index); 
  
    tdsrrcsib_update_sibdb_with_sib5_info(&local_sys_info_cont_enc_store, sib_db_index, index); 

    tdsrrcsib_update_sibdb_with_sib7_info(&local_sys_info_cont_enc_store, sib_db_index, index); 

    if(local_sys_info_cont_ptr->m.sysInfoType11Present) 
    {
      tdsrrcsib_update_sibdb_with_sib11_info(&local_sys_info_cont_enc_store, sib_db_index, index); 
    } 
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB: SIB11 is not present in SI container");
      /* This will make sure that tdsrrcsib_is_sib11_11bis_12_read_deferred
         returns SIB11 as deferred when network supports DMCR*/
      tdssibdb.cell[sib_db_index].sib_value_tag[tdsrrc_SIB11] = TDSINVALID_MIB_SB_VALUE_TAG;
    } 

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
    if(local_sys_info_cont_ptr->m.sysInfoType11bisPresent) 
    {
      tdsrrcsib_update_sibdb_with_sib11Bis_info(&local_sys_info_cont_enc_store, sib_db_index, index);       
    }
#endif

    if(local_sys_info_cont_ptr->m.sysInfoType12Present) 
    {
      tdsrrcsib_update_sibdb_with_sib12_info(&local_sys_info_cont_enc_store, sib_db_index, index);       
    }
  
  /* If all mandatory SIBS are received, update database and return TRUE */
  if(SUCCESS == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs(
            &local_sys_info_cont_ptr->mib, 
            tdsrrc_MIB, index)
    )
  {
    tdsrrcsib_update_sched_info_from_mib_sb(
            (uint32)tdsrrc_MIB, 
            &local_sys_info_cont_ptr->mib, 
            index);
  }

  if((local_sys_info_cont_ptr->m.sysInfoTypeSB1Present) &&
     (SUCCESS == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs(
            &local_sys_info_cont_ptr->sysInfoTypeSB1, 
            tdsrrc_SB1, index))
    )
  {
    tdsrrcsib_update_sched_info_from_mib_sb(
            (uint32)tdsrrc_SB1, 
            &local_sys_info_cont_ptr->sysInfoTypeSB1, 
            index);
  }

  if((local_sys_info_cont_ptr->m.sysInfoTypeSB2Present) &&
     (SUCCESS == tdsrrcsib_check_if_scheduling_info_received_for_all_sibs(
            &local_sys_info_cont_ptr->sysInfoTypeSB2, 
            tdsrrc_SB2, index))
    )
  {
    tdsrrcsib_update_sched_info_from_mib_sb(
            (uint32)tdsrrc_SB2, 
            &local_sys_info_cont_ptr->sysInfoTypeSB2, 
            index);
  }

  /* Store all the value tags in the SIB database.
     * Only decoded sibs will be updated  */
  tdsrrcsibdb_store_value_tags_for_new_cell(sib_db_index
#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
                                     ,index
#endif
                                      );

  tdsrrcsib_update_tsib_with_acq_sibs(local_sys_info_cont_ptr, &local_sys_info_cont_enc_store, index);

  tdsrrcsib_free_rencoded_sibs(local_sys_info_cont_ptr, &local_sys_info_cont_enc_store);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"CSFB: Set V-Tag for all SIB's as invalid in SIB DB"); 

  for(v_tag_idx = 0;v_tag_idx<TDSRRC_TOT_SIBS;v_tag_idx++)
  {
    tdssibdb.cell[sib_db_index].sib_value_tag[v_tag_idx] = TDSINVALID_MIB_SB_VALUE_TAG;
  }

  if(tdsrrcasn1_free_pdu(tdsrrc_System_Information_Container_PDU,(void *)local_sys_info_cont_ptr))
  {
    ERR_FATAL("Unable to free the decoded PDU",0,0,0);
  }
  
  /* Get the plmn id for this cell. Make sure the PLMN id is a GSM-MAP PLMN. 
     * Convert it to the internal format used by RRC. */
  if(tdssibdb.cell[sib_db_index].sibs.mib->plmn_Type.t == T_tdsrrc_PLMN_Type_gsm_MAP)
  {
    tdsrrc_translate_gsm_map_plmn_id(
              &(tdssibdb.cell[sib_db_index].sibs.mib->plmn_Type.u.gsm_MAP->plmn_Identity),
              &plmn
              );
  }
  else if(tdssibdb.cell[sib_db_index].sibs.mib->plmn_Type.t == T_tdsrrc_PLMN_Type_gsm_MAP_and_ANSI_41)
  {
    tdsrrc_translate_gsm_map_plmn_id(
              &(tdssibdb.cell[sib_db_index].sibs.mib->plmn_Type.u.gsm_MAP_and_ANSI_41->plmn_Identity),
              &plmn
              );
  }
  
  cell = tdsrrc_translate_cell_id(&(tdssibdb.cell[sib_db_index].sibs.sib3->cellIdentity));

  /* Also Copy the frequency and Scrambling code into the
      SIB Database */
  tdssibdb.cell[sib_db_index].plmn_id= plmn;
  tdssibdb.cell[sib_db_index].cell_id = cell;

  /* Get the SIB Database index for this cell */
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"CSFB: Checking SIBDB index CellId %d and MCC %d MNC %d", cell, 
        TDSRRC_CSP_GET_INT_MCC(plmn), 
        TDSRRC_CSP_GET_INT_MNC(plmn));


  /* First setup the BCCH channel required for this command.
  This function also checks if there is an existing SIB
  event and if so resolves the conflict between the events. */
  bcch_status = tdsrrcsib_bcch_setup_req( cmd_ptr );

//Mark event active here a sib_bcch_setup_req will need event_active to be FALSE
  tdssib_events_ptr->event[index].event_active = TRUE;


  if( bcch_status == TDSRRCSIB_BCCH_SETUP_FAILED )
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sib event dropped: BCCH failure");
    tdsrrc_sib_post_complete_for_csfb_sib_read(TDSSIB_EVENT_FAILED_LOWER_LAYERS);
    return FALSE;
  }

  if( bcch_status == TDSRRCSIB_BCCH_SETUP_DONE )
  {
    tdssib_events_ptr->srv_bcch.bcch_action = TDSSIB_BCCH_SETUP_PENDING;
  }

  /* Get the SIB Database index for this cell */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"CSFB: bcch_status = %d",bcch_status);

  return TRUE;
}
#endif /* FEATURE_3GPP_CSFB_TDS */

void tdsrrcsib_get_nv_items()
{

   /*read SIB sleep data from NV*/
   if (tdsrrcnv_efs_read(TDS_RRC_NV_SIB_SLEEP_BEFORE_SB, 
                         (byte *) &tdsrrcsib_sib_sleep,
                         sizeof(tds_nv_sib_sleep)))
   {
      tdsrrcsib_sib_sleep.sib_sleep_flag = 0; /* disable by default */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_SIB_SLEEP_BEFORE_SB is read,enable %d",tdsrrcsib_sib_sleep.sib_sleep_flag);
   }
   else
   {
      if (!tdsrrcnv_get_default_value(TDS_RRC_NV_SIB_SLEEP_BEFORE_SB,
                                      (byte *) &tdsrrcsib_sib_sleep,
                                      sizeof(tds_nv_sib_sleep),
                                      TRUE))
      {
         tdsrrcsib_sib_sleep.sib_sleep_flag = 0; /* disable by default */
         tdsrrcsib_sib_sleep.repetition_len = TDSRRCSIB_SB_SLEEP_SCHEDUING_MIN_FRAME;
         tdsrrcsib_sib_sleep.sib_end_frame = TDSRRCSIB_SB_SLEEP_SIB_END_FRAME;
         tdsrrcsib_sib_sleep.misc_data = 0;
         
         TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDS_RRC_NV_SIB_SLEEP_BEFORE_SB item error. Setting to default values");
      }

      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Read failed for TDS_RRC_NV_TDS_OPTIONAL_FEATURE_LIST,set default all enabled");
   }

   /* Check the validity of tdsrrcsib_sib_sleep. */
   if (tdsrrcsib_sib_sleep.sib_sleep_flag)
   {
      boolean rewrite_sleep_nv = FALSE;
      
      if (tdsrrcsib_sib_sleep.repetition_len % TDSRRCSIB_SB_SLEEP_SCHEDUING_MIN_FRAME)
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcsib_sib_sleep.repetition_len -= tdsrrcsib_sib_sleep.repetition_len % TDSRRCSIB_SB_SLEEP_SCHEDUING_MIN_FRAME;
      }

      if ((tdsrrcsib_sib_sleep.repetition_len < TDSRRCSIB_SB_SLEEP_SCHEDUING_MIN_FRAME)
          || (tdsrrcsib_sib_sleep.repetition_len > 4096))
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcsib_sib_sleep.repetition_len = TDSRRCSIB_SB_SLEEP_SCHEDUING_MIN_FRAME;
      }

      if (tdsrrcsib_sib_sleep.sib_end_frame >= tdsrrcsib_sib_sleep.repetition_len)
      {
         rewrite_sleep_nv = TRUE;
         tdsrrcsib_sib_sleep.sib_end_frame = 2 + (tdsrrcsib_sib_sleep.repetition_len / 2);
      }

      if (rewrite_sleep_nv)
      {
         boolean del_flag = tdsrrcnv_efs_delete(TDS_RRC_NV_SIB_SLEEP_BEFORE_SB);

         TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_SIB_SLEEP_BEFORE_SB Delete the NV status %d",del_flag);

         if (tdsrrcnv_efs_write(TDS_RRC_NV_SIB_SLEEP_BEFORE_SB, 
                                (const byte *)&tdsrrcsib_sib_sleep, 
                                sizeof(tds_nv_sib_sleep)))
         {
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS_RRC_NV_SIB_SLEEP_BEFORE_SB written to NV success");
         }
         else
         {
            /* We still keep valid values in tdsrrcsib_sib_sleep */
            TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDS_RRC_NV_SIB_SLEEP_BEFORE_SB written to NV failed");
         }
      }
   }
}

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
)
{
  tdsrrclog_complete_sib_rcvd_event_type sib_rcvd_event;

  uint8 sib_type;
#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
  tdsrrc_SIB_type sib_int_type;
#endif

  switch(sib_Type)
  {
    case tdsrrc_SIB_Type_masterInformationBlock:
      sib_type = TDSRRCLOG_MASTER_INFORMATION_BLOCK;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType1:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE1;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType2:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE2;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType3:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE3;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType4:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE4;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType5:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE5;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType6:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE6;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType7:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE7;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType11:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE11;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType12:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE12;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType18:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE18;
      break;
    case tdsrrc_SIB_Type_schedulingBlock1:
      sib_type = TDSRRCLOG_SCHEDULING_BLOCK_TYPE1;
      break;
    case tdsrrc_SIB_Type_schedulingBlock2:
      sib_type = TDSRRCLOG_SCHEDULING_BLOCK_TYPE2;
      break;
    case tdsrrc_SIB_Type_systemInformationBlockType5bis:
      sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE5BIS;
      break;
#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
    case tdsrrc_SIB_Type_extensionType:
      sib_int_type = tdsrrcsib_map_extension_sib_to_internal_sibtype(seg_index,sfn_prime);
#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
      if(sib_int_type == tdsrrc_SIB11bis)
      {
        sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE11BIS;
      }
      else 
#endif
#ifdef FEATURE_TDSCDMA_SIB19
      if(sib_int_type == tdsrrc_SIB19)
      {
        sib_type = TDSRRCLOG_SYSTEM_INFORMATION_BLOCK_TYPE19;
      }
      else
#endif
      {
        sib_type = TDSRRCLOG_EXTENSION_SIB_TYPE_UNSUPPORTED;
      }
      break;
#endif
    default:
      sib_type = TDSRRCLOG_SIB_TYPE_UNSUPPORTED;
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupported SIB Type");
      break;
  }

  sib_rcvd_event.sib_type = sib_type;
  event_report_payload(EVENTS_TDSCDMA_RRC_SIB_RCVD_V2, sizeof(sib_rcvd_event),
                       (void *)&sib_rcvd_event);
}


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
boolean tdsrrcsib_is_serving_sib7_reading(void)
{
  

  if (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_active == TRUE)
  {
    if( (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].event_name == TDSRRCSIB_GET_SPECIFIC_SIBS)
      &&(tdssib_events_ptr->event[TDSSIB_SRV_INDEX].req_proc == TDSRRC_PROCEDURE_SIB))
    {
      if ((tdssib_events_ptr->event[TDSSIB_SRV_INDEX].for_serving_cell == TRUE)
        &&(tdssib7info.req_proc == TDSRRC_PROCEDURE_RCE))
      {
        if ((tdssib_events_ptr->event[TDSSIB_SRV_INDEX].tsib[tdsrrc_MIB].sib_needed == TRUE)&&
            (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].tsib[tdsrrc_MIB].sib_received == TRUE)&&
            (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].tsib[tdsrrc_MIB].sib_decoded == TRUE )&&
            (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].tsib[tdsrrc_SIB7].sib_needed == TRUE)&&
            (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].tsib[tdsrrc_SIB7].sib_received == FALSE)&&
            (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].tsib[tdsrrc_SIB7].sib_decoded == FALSE)&&
            (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].int_event.int_event_type == TDSRRCSIB_INT_SIB7_EVENT)&&
            (tdssib7info.sib_timer_update_req == FALSE)&&
            (tdssib7info.sib_ext_proc_update_req == TRUE)&&
            (tdssib7info.req_proc == TDSRRC_PROCEDURE_RCE ))
        {
          /* Only SIB 7 reading needed. */
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"FEATURE_TDSCDMA_SPECIFIC_SIB_READING:SIB want to update SIB7");
          return TRUE;
        }
      }
    }
  }

  return FALSE;
}
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
uint8*  tdsrrcsib_build_sib7_schedule_info_for_random_access(void)
{
  uint32 count = 0;
  uint32 seg_count = 0;
  uint32 loop_count = 0;
  uint8 * table_ptr = NULL;
  uint32 rep_count;
  uint32 sfn = 0; 
  uint32 byte_pos = 0;               
  uint8 byte_offset = 0;  
  uint8 bit_mask = 0;
  boolean is_sib7_rep_less_then_32 = FALSE; 
  uint32 rep_byte_pos = 0;
  uint8 work_table;

  if(TDSEVENT_INDEX_VALID(TDSSIB_SRV_INDEX) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return NULL;
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"SIB_Sleep:Updating SIB Sleep Schedule Table for SIB :%d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(tdsrrc_SIB7));
 
  if(tdssib_sleep_schedule_table_in_use == 0)
  {
    table_ptr = tdssib_sleep_schedule_table_1;
    work_table = 1;
  }
  else
  {
    table_ptr = tdssib_sleep_schedule_table_0;
    work_table = 0;
  }

  memset(table_ptr,0x00,TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE);  

  /* Loop through the all the SIB(segments) and update the sleep schedule table based on 
     the SIB Schedule */

    /* Include the schedule only if the SIB is needed & the SIB has a valid schedule 
	     and if it has not been decoded already */
    if((tdssib_events_ptr->event[TDSSIB_SRV_INDEX].tsib[tdsrrc_SIB7].sib_needed == TRUE) &&
       (tdssib_events_ptr->event[TDSSIB_SRV_INDEX].tsib[tdsrrc_SIB7].sib_decoded == FALSE) &&
       (tdssib_events_ptr->sib_sched[tdsrrc_SIB7].repitition_rate != TDSINVALID_SIB_SCHEDULE_REP)) 
    {
      /* Calculate the rep count to be filled in the loop, lets fill complete table so lets keep
          max rept rate to 4096 which is SFN cycle*/
      rep_count = 4096/tdssib_events_ptr->sib_sched[tdsrrc_SIB7].repitition_rate;

      for (seg_count = 0; seg_count < tdssib_events_ptr->sib_sched[tdsrrc_SIB7].num_segments; seg_count++)
      {
         sfn = tdssib_events_ptr->sib_sched[tdsrrc_SIB7].sfn_offset[seg_count]; /* Local copy of SFN */
         byte_pos = sfn>>3;                /* (sfn/8) Gives the byte location in sib sleep schedule table */
         byte_offset = (uint8)(sfn & 0x07); /* (sfn%8)Gives the offset within the byte in sib sleep table */
         bit_mask = 0xFF;
         is_sib7_rep_less_then_32 = FALSE; 
 
        bit_mask = tdsrrcsib_gen_bit_mask_for_schedule_table(byte_offset);
        if(count == 7)
        {
          if(tdssib_events_ptr->sib_sched[tdsrrc_SIB7].repitition_rate < 32)
          {
            is_sib7_rep_less_then_32 = TRUE;
          }
        }
        /* Apply the calculated bitmask for the byte till the max repetition cycle */
        for(loop_count = 0; loop_count < rep_count; loop_count++)
        {
          if(is_sib7_rep_less_then_32 == TRUE)
          {
            /*Lets skipp intermidiate sib-7's, as Spec says SIB-7 have to be read at a minimum of 320msec
               Its also controled by a calulation, which uses a parameter from sib-7. But since in our implimentation
               S-BCCH is up all the time in FACH, So lets read SIB7 at a minmum of 320msec*/
            rep_byte_pos = byte_pos + (loop_count * (32/8));
          }
          else
          {
            rep_byte_pos = byte_pos + (loop_count * (tdssib_events_ptr->sib_sched[tdsrrc_SIB7].repitition_rate/8));
          }
          
          if(rep_byte_pos < TDSRRCSIB_SIB_SLEEP_SCHEDULE_TABLE_SIZE)
          {
            table_ptr[rep_byte_pos] |= bit_mask;
          }
        }
      }
    }


  /* Debug print */
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB_Sleep table after update by SIB %d :Max repetition rate is %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(tdsrrc_SIB7),tdssib_events_ptr->scheduling_max_rept_rate);
  tdssib_sleep_schedule_table_in_use = work_table;
  for(count = 0; count < (uint32)(tdssib_events_ptr->scheduling_max_rept_rate/8);count++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"SIB_Sleep: Table Contents byte[%d] = %0X",count,table_ptr[count]);
  }
  return table_ptr;

}
#endif

