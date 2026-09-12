/*===========================================================================
                        R R C    C S P    D B
DESCRIPTION

   This file contains the access routines for Stored Acquisition Database 
   used for Cell Selection Procedure.

   The Stored Acquisition Database is part of the PLMN Search mechanism. 
   The implemented blocks are marked with a "=======" boundary below.

              CSP PLMN SEARCH MECHANISM
              
       -----------   ------------    =============    =========
       |         |   | CSP      |    | Acq DB    |    | Acq   |
       |CSP      |   | Initial  |    | Access    |    | DB    |
       |State    |   | And .    |<-->| Functions |<-->|       |
       |Machine  |   | Stored   |    | -PLMN     |    |       |
       |Functions|<->|Selection |    | -Acq type |    |       |
       |         |   |Controller|    =============    =========
       |         |   |-PLMN     |    
       |         |   |-PLMN     |    ------------
       -----------   | type     |<-->| CSP      |
            |        ------------    | Initial  |
            |           |            | Selection|
            |           |            | Algorithm| 
       -----------      |            ------------            
       |Internal |   ------------       
       |  Data   |   |Selection |
       |         |   |          |
       -----------   |Data      |
                     ------------

EXTERNALIZED FUNCTIONS   
   
    tdsrrccsp_init_acq_db()
        This function initializes the stored acquisition database. 

    rrc_rrc_csp_add_acq_entry()
        This function adds an acquisition entry to the acquisition 
        list. While doing so, it also updates the acquisition
        database pointers so that the topmost pointer points to this
        most recent acquisition entry.

    tdsrrccsp_get_first_match_acq_entry()
        This function gets the most recent acquisition entry from acq.
        database that matches the criteria given in the parameters. 

    tdsrrccsp_get_next_match_acq_entry()
        This function gets next acquisition entry from acq. database
        that matches the criteria given in the parameters. 


DEFINITIONS

    Acquisition Entry
        Data Structure that contains a mapping of PLMN and Frequency. In
        addition, it also contains the status of cell belonging to this 
        PLMN and found on this frequency. The status may be 
        barred/non-barred, forbidden / not forbidden reg. area.

    Stored Acquisition Database
        List of pointers that point to acquisition entries in the order 
        such that the most recent acquisition entry is pointed to by the 
        topmost pointer and the oldest acquisition entry is pointed to by
        the last pointer going in that order.

    Acquisition List
        An array for storing acquisition entries. The order of the entries
        is not important. All acquisition entries are accessed thru' pointers
        of the acquisition database and there is no direct access. 

  
MECHANISM

    The acquisition database is a two-layered structure as shown below:
   
          ACQ. DATABASE           ACQ. LIST
          -------------           ----------
        [Most recent ptr]------->[Acq. entry]
        [Next recent ptr]------->[Acq. entry]
        [Next recent ptr]------->[Acq. entry]
        [Next recent ptr]------->[Acq. entry]
        [Next recent ptr]------->[Acq. entry]
        [Next recent ptr]------->[Acq. entry]

    The pointers in Acquisition Database are ordered with the first one pointing
    to the most recent acquisition entry and so on. 

    The two-layered structure obviated the need to re-order acquisition list 
    every time a new acquisition entry is added to the list. A re-oder of 
    pointers in acquisition database is done but the acquisition list does not
    need to be ordered. This saves processing and is efficient.

    NOTE: Over time, the figure above will change - the arrows will no longer 
    remain horizontal. This is because acquisition list will not have any order
    but acquisition database pointers will always be ordered.


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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccspdb.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/10/11   zz     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

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
#include "err.h"
#include "assert.h"
#include "tdsuecomdef.h"

#include "tdsrrccspdb.h"
#include "tdsrrcnv.h"
#include "assert.h"
#include "tdsrrccsp.h"
#include "tdsrrccspi.h"
#include "tdsrrcscmgr.h"
#include "tdsl1rrcif.h"

#include "mm_umts.h"

#include "sys_eplmn_list.h"

#if defined(TEST_FRAMEWORK)
#error code not present
#endif


/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */

static tds_nv_acq_db_type      tdsrrccsp_nv_acq_db;
static tds_nv_acq_entry_type   tdsrrccsp_nv_acq_list[TDS_MAX_ACQ_DB_NV_ENTRIES]; 

/* the first uint16 entry of TDS_RRC_NV_PRIM_FREQ_LIST gives the count of freqs in following uint16 entries */
static uint16          tdsrrccsp_nv_prim_freq_list[TDS_MAX_PRIM_FREQ_NBR+1];

static uint32          tdsrrccsp_nv_tds_special_freq; /* TDS_RRC_NV_SPECIAL_FREQ */
static boolean         tdsrrccsp_nv_tds_special_freq_enabled; /* TDS_RRC_NV_SPECIAL_FREQ_ENABLED */


/* Max entries for the NV_ACQ_LIST items. */
#define  TDSMAX_NV_ACQ_DB_ENTRIES1  3
#define  TDSMAX_NV_ACQ_DB_ENTRIES2  2

/* List of Acquisition entries: Note that the Acquisition entries are NOT
 * accessed directly. All access is through access routines that utilize
 * Stored Acquisition Database pointers.
 */

/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrc_csp_acq_entry_type  tdsrrccsp_acq_list[TDS_MAX_ACQ_DB_ENTRIES];

/* LIST DB in which ACQ DB entries are stored. This LIST DB is used to
 * to send freqs to l1 for doing list scan
 */
tdsrrc_csp_acq_entry_type  tdslist_db[TDS_MAX_ACQ_DB_NV_ENTRIES];



/* Stored Acquisition Database: The pointers in the database point to entries
 * in the Acquisition List 
 */

/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrc_csp_acq_db_type     tdsrrccsp_stored_acq_db;

/* Used for storing frequencies scanned during acquisition database scan
 */
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrc_csp_acq_db_freq_scanned_type tdsrrccsp_acq_db_freq_scanned_list;

/* Last Requested PLMN: This stores the last PLMN requested thru' 
 * rrc_csp_get_acq_entry(). The responsibility is with the caller to remember
 * last sent PLMN and send the same again when calling 
 * rrc_csp_get_next_acq_entry(). 
 * This local var serves as a check to compare the passed value in 
 * rrc_csp_get_next_acq_entry() with the one stored by it. Both should match.
 */
/*This variable is used in file scope only and set for global for debugging purpose only*/
rrc_plmn_identity_type   tdsrrccsp_last_req_plmn_id;

/* Last Requested Type of Acq: This stores the last acq. type requested thru'
 * rrc_csp_get_acq_entry(). The responsibility is with the caller to remember
 * last sent acq. type and send the same again when calling 
 * rrc_csp_get_next_acq_entry(). 
 * This local var serves as a check to compare the passed value in 
 * rrc_csp_get_next_acq_entry() with the one stored by it. Both should match.
 */
/*This variable is used in file scope only and set for global for debugging purpose only*/
tdsrrc_csp_acq_e_type       tdsrrccsp_last_req_type_acq;
extern tdsrrc_ds_tuneaway_status_change_ind_type tds_ds_tuneaway_status_change_ind;

/* Flexible Band F search patterns, which is get from NV.
 * Search pattern can be specified for lower band and upper band separately.
 * And for each band, search pattern can be shallow, deep or shallow+deep.
 */
tdsrrc_csp_band_f_pattern_type  tdsrrccsp_band_f_pattern;
/* Flexible Band A search patterns, which is get from NV.*/
tdsrrc_csp_band_a_pattern_type  tdsrrccsp_band_a_pattern;
/* Flexible Band E search patterns, which is get from NV.*/
tdsrrc_csp_band_e_pattern_type  tdsrrccsp_band_e_pattern;

tdsrrc_csp_lock_freq_cell_type tdsrrccsp_lock_freq_cell;
extern boolean tdsrrccsp_ignore_MSET;
/* =======================================================================
**                            Function Definitions
** ======================================================================= */


/*===========================================================================

FUNCTION          RRC_CSP_INIT_ACQ_DB

DESCRIPTION       This function initializes the Stored acquisition database. 
           
DEPENDENCIES
                  None

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.

NOTE
                  At present, the acquisition database is initialized with
                  all pointers set to NULL. Once the NV task is 
                  added, the database can be stored in NV and read from it
                  on powerup.
===========================================================================*/
void  tdsrrccsp_init_acq_db(void)
{
  uint8  i;

  /* Initialize Current Read Index to -1 */
  tdsrrccsp_stored_acq_db.curr_rd_index = -1;

  /* Initialize Current Number of Pointers to 0 */
  tdsrrccsp_stored_acq_db.curr_wr_index = 0;

  tdsrrccsp_stored_acq_db.last_entry_invalid = TRUE;

  /* Initialize all values in ordered acquisition list in acq database to 
   */
  for (i=0; i < TDS_MAX_ACQ_DB_ENTRIES; i++)
  {
    tdsrrccsp_stored_acq_db.acq_list_indices[i] = i;
  }
}  /* tdsrrccsp_init_acq_db */


/*===========================================================================

FUNCTION          RRC_CSP_READ_NV_ACQ_INFO

DESCRIPTION       This function reads the stored acquisition databse and the 
                  acquisition list from the NV.
           
DEPENDENCIES
                  None

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.

NOTE
                  None.
===========================================================================*/
void  tdsrrccsp_read_nv_acq_info(void)
{
  int  i;
  int  j;
  boolean acq_db_valid = TRUE;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reading TDS_RRC_NV_ACQ_DB");
  if(tdsrrcnv_efs_read(TDS_RRC_NV_ACQ_DB, (byte *) &tdsrrccsp_nv_acq_db, sizeof(tdsrrccsp_nv_acq_db)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Read TDS_RRC_NV_ACQ_DB successfully");
  }
  else
  {
    /* Delete the existing file in case it is an old version */
    boolean del_flag = tdsrrcnv_efs_delete(TDS_RRC_NV_ACQ_DB);
    acq_db_valid = FALSE;
    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_ACQ_DB not read. Delete the NV status %d",del_flag);
  }
  
  tdsrrccsp_stored_acq_db.curr_rd_index = (int8)tdsrrccsp_nv_acq_db.curr_rd_index;
  tdsrrccsp_stored_acq_db.curr_wr_index = (int8)tdsrrccsp_nv_acq_db.curr_wr_index;
  tdsrrccsp_stored_acq_db.last_entry_invalid = tdsrrccsp_nv_acq_db.last_entry_invalid;

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Reading TDS_RRC_NV_ACQ_DB rd=%d, wr=%d, b_last_entry_invalid=%d", 
              tdsrrccsp_stored_acq_db.curr_rd_index, 
              tdsrrccsp_stored_acq_db.curr_wr_index, 
              tdsrrccsp_stored_acq_db.last_entry_invalid);

  /* Do a Sanity check - check read and write index. */
  if ((tdsrrccsp_stored_acq_db.curr_wr_index > TDSMAX_ACQ_DB_INDEX) || 
      (tdsrrccsp_stored_acq_db.curr_rd_index > TDSMAX_ACQ_DB_INDEX) ||
      (tdsrrccsp_stored_acq_db.curr_rd_index < -1) ||
      (tdsrrccsp_stored_acq_db.curr_rd_index > tdsrrccsp_stored_acq_db.curr_wr_index)||
      (tdsrrccsp_stored_acq_db.curr_wr_index < 0))
  {
    acq_db_valid = FALSE;
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Acq db invalid,Wr index %d Rd index %d. Re-init ACQ DB",tdsrrccsp_stored_acq_db.curr_wr_index,tdsrrccsp_stored_acq_db.curr_rd_index);
  }
  if (!acq_db_valid)
  {
    /* Re-initialize the Acquisition database */
    tdsrrccsp_init_acq_db();
  }
  else
  {
  for( i=0; i< TDS_MAX_ACQ_DB_ENTRIES; i++ )
  {
    tdsrrccsp_stored_acq_db.acq_list_indices[i] = tdsrrccsp_nv_acq_db.acq_list_indices[i];
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"ACQ list index for entry %d = %d",i,tdsrrccsp_stored_acq_db.acq_list_indices[i]); 
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reading TDS_RRC_NV_ACQ_LIST");
  if (tdsrrcnv_efs_read( TDS_RRC_NV_ACQ_LIST, (byte *) &tdsrrccsp_nv_acq_list, sizeof(tdsrrccsp_nv_acq_list)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Read TDS_RRC_NV_ACQ_LIST successfully");
    /* Read the acq entries from the NV_ACQ_LIST_1 */
    for( i=0; i< TDS_MAX_ACQ_DB_ENTRIES; i++ )
    {
      tdsrrccsp_acq_list[i].plmn_id.mcc[0] = tdsrrccsp_nv_acq_list[i].plmn_id.mcc[0];
      tdsrrccsp_acq_list[i].plmn_id.mcc[1] = tdsrrccsp_nv_acq_list[i].plmn_id.mcc[1];
      tdsrrccsp_acq_list[i].plmn_id.mcc[2] = tdsrrccsp_nv_acq_list[i].plmn_id.mcc[2];
      tdsrrccsp_acq_list[i].plmn_id.num_mnc_digits = tdsrrccsp_nv_acq_list[i].plmn_id.num_mnc_digits;
      if(tdsrrccsp_acq_list[i].plmn_id.num_mnc_digits > RRC_MAX_MNC_DIGITS)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"NV_ACQ_LIST_1_I has invalid num_mnc_digits");
        tdsrrccsp_acq_list[i].plmn_id.num_mnc_digits = RRC_MAX_MNC_DIGITS;
      }
      for(j=0; j<tdsrrccsp_acq_list[i].plmn_id.num_mnc_digits; j++)
      {
        tdsrrccsp_acq_list[i].plmn_id.mnc[j] = tdsrrccsp_nv_acq_list[i].plmn_id.mnc[j];
      }

      tdsrrccsp_acq_list[i].freq = (uint16)tdsrrccsp_nv_acq_list[i].freq;
      /* Get the band_class based on the frequency */
      tdsrrccsp_acq_list[i].band_class = tdsrrc_get_frequency_band(tdsrrccsp_acq_list[i].freq);
      tdsrrccsp_acq_list[i].status_mask = (uint16)tdsrrccsp_nv_acq_list[i].status_mask;
      tdsrrccsp_acq_list[i].cell_id = tdsrrccsp_nv_acq_list[i].cell_id;
      tdsrrccsp_acq_list[i].cpid = (uint8)tdsrrccsp_nv_acq_list[i].cpid;
      tdsrrccsp_acq_list[i].lac = (uint8)tdsrrccsp_nv_acq_list[i].lac;

      TDSRRC_MSG3(MSG_LEGACY_HIGH,"  Read TDS_RRC_NV_ACQ_LIST[%d]: Freq=%d, CPID=%d", 
                  i, 
                  tdsrrccsp_acq_list[i].freq, 
                  tdsrrccsp_acq_list[i].cpid);
    }
  }
  else
  {
    /* Delete the existing file in case it is an old version */
    boolean del_flag = tdsrrcnv_efs_delete(TDS_RRC_NV_ACQ_LIST);

    TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_ACQ_LIST not read. Delete the NV status %d",del_flag);

	  /* Re-initialize the Acquisition database */
      tdsrrccsp_init_acq_db();
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Setting is_valid_nw_sharing to FALSE");
  for(i = 0; i<TDS_MAX_ACQ_DB_ENTRIES; i++)
  {
    tdsrrccsp_acq_list[i].is_valid_nw_sharing = FALSE;
  }
  }

  /* Init the primary frequency list */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reading TDS_RRC_NV_PRIM_FREQ_LIST");
  if (tdsrrcnv_efs_read( TDS_RRC_NV_PRIM_FREQ_LIST, (byte *) &tdsrrccsp_nv_prim_freq_list, sizeof(tdsrrccsp_nv_prim_freq_list)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Read TDS_RRC_NV_PRIM_FREQ_LIST successfully");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Read TDS_RRC_NV_PRIM_FREQ_LIST fail,use default list");
	if (!tdsrrcnv_get_default_value(TDS_RRC_NV_PRIM_FREQ_LIST,
                                   (byte *) &tdsrrccsp_nv_prim_freq_list,
                                   sizeof(tdsrrccsp_nv_prim_freq_list),
                                   TRUE))
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS prim freq unavailable");
	  tdsrrccsp_nv_prim_freq_list[0] = 0;
    }
  }
  /* Get the num of valid primary freqs */
  tdsrrc_csp_int_data.num_tds_prim_freq = tdsrrccsp_nv_prim_freq_list[0];
  if (tdsrrc_csp_int_data.num_tds_prim_freq > TDS_MAX_PRIM_FREQ_NBR)
  {
     TDSRRC_MSG2(MSG_LEGACY_HIGH,"limit num of TDS prim freq from %d to %d",tdsrrc_csp_int_data.num_tds_prim_freq,TDS_MAX_PRIM_FREQ_NBR);
     tdsrrc_csp_int_data.num_tds_prim_freq = TDS_MAX_PRIM_FREQ_NBR;
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"num of TDS prim freq = %d",tdsrrc_csp_int_data.num_tds_prim_freq);

  for( i=0; i< tdsrrc_csp_int_data.num_tds_prim_freq; i++ )
  {
    tdsrrc_csp_int_data.tds_prim_freq_list[i] = tdsrrccsp_nv_prim_freq_list[i+1];
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Got TDS prim freq %d",tdsrrc_csp_int_data.tds_prim_freq_list[i]);
  }

  /* Init the special freq flag here match with ACQ DB read behavior */
  tdsrrc_csp_int_data.use_special_freq_for_acq = FALSE;

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Reading TDS freq (if present) for single freq scan");
  if(tdsrrcnv_efs_read(TDS_RRC_NV_SPECIAL_FREQ_ENABLED, (byte *) &tdsrrccsp_nv_tds_special_freq_enabled, sizeof(tdsrrccsp_nv_tds_special_freq_enabled)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_SPECIAL_FREQ_ENABLED is %d",tdsrrccsp_nv_tds_special_freq_enabled);
  }
  else
  {
    if (!tdsrrcnv_get_default_value(TDS_RRC_NV_SPECIAL_FREQ_ENABLED,
                                   (byte *) &tdsrrccsp_nv_tds_special_freq_enabled,
                                   sizeof(tdsrrccsp_nv_tds_special_freq_enabled),
                                   TRUE))
    {
       tdsrrccsp_nv_tds_special_freq_enabled = FALSE;
       TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_SPECIAL_FREQ_ENABLED item error. Setting to default: %d",tdsrrccsp_nv_tds_special_freq_enabled);
    }
	TDSRRC_MSG1(MSG_LEGACY_ERROR,"TDS_RRC_NV_SPECIAL_FREQ_ENABLED not read,setting to default: %d",tdsrrccsp_nv_tds_special_freq_enabled);
  }

    tdsrrc_csp_int_data.tds_special_freq_enabled = tdsrrccsp_nv_tds_special_freq_enabled;
    if(tdsrrc_csp_int_data.tds_special_freq_enabled)
    {
      if(tdsrrcnv_efs_read(TDS_RRC_NV_SPECIAL_FREQ, (byte *) &tdsrrccsp_nv_tds_special_freq, sizeof(tdsrrccsp_nv_tds_special_freq)))
      {
        tdsrrc_csp_int_data.use_special_freq_for_acq = TRUE;
        tdsrrc_csp_int_data.tds_special_freq = (uint16)tdsrrccsp_nv_tds_special_freq;
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_SPECIAL_FREQ read. Freq = %d",tdsrrc_csp_int_data.tds_special_freq);
      }
	else if (tdsrrcnv_get_default_value(TDS_RRC_NV_SPECIAL_FREQ,
                                   (byte *) &tdsrrccsp_nv_tds_special_freq,
                                   sizeof(tdsrrccsp_nv_tds_special_freq),
                                   TRUE))
      {
	  tdsrrc_csp_int_data.use_special_freq_for_acq = TRUE;
      tdsrrc_csp_int_data.tds_special_freq = (uint16)tdsrrccsp_nv_tds_special_freq;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"TDS_RRC_NV_SPECIAL_FREQ not read,setting freq = %d as default",tdsrrc_csp_int_data.tds_special_freq);
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDS_RRC_NV_SPECIAL_FREQ not read,turn off tds_special_freq_enabled");
      tdsrrc_csp_int_data.tds_special_freq_enabled = FALSE;
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TDS_RRC_NV_SPECIAL_FREQ_ENABLED set to FALSE");
  } 

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"special_freq_enabled %d,use_special_freq_for_acq %d",tdsrrc_csp_int_data.tds_special_freq_enabled,tdsrrc_csp_int_data.use_special_freq_for_acq);
#if 0//haizhou: need to add new NV in TD ??
#ifdef FEATURE_DUAL_SIM_WCDMA_TUNEAWAY   
  if ( NV_DONE_S ==  (tdsrrc_get_nv( NV_WCDMA_CS_VOICE_OVER_HSPA_ENABLED_I, 
                                  (nv_item_type *) &tds_ds_tuneaway_status_change_ind.tuneaway_status)))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"DSIM_TUNEAWAY: DS Tune Away status : %d ",tds_ds_tuneaway_status_change_ind.tuneaway_status);
  }
  else
  {
    tds_ds_tuneaway_status_change_ind.tuneaway_status = FALSE; /* Default disabled*/
  }
#endif
#endif

}  /* tdsrrccsp_read_nv_acq_info */
/*===========================================================================

FUNCTION          RRCCSP_READ_NV_CS_DOMAIN_BAR_TIMER_LENGTH

DESCRIPTION       This function read cs domain bar timer length from NV
           
DEPENDENCIES
                  None

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.

NOTE
                  None.
===========================================================================*/
void tdsrrccsp_read_nv_cs_domain_bar_timer_length(void)
{
	 if (tdsrrcnv_efs_read(TDS_RRC_NV_CS_FAIL_BAR_TIME, 
						   (byte *) &tds_cs_domain_bar_timer_length,
						   sizeof(tds_cs_domain_bar_timer_length)))
	 {
	   TDSRRC_MSG1(MSG_LEGACY_HIGH," tds_cs_domain_bar_timer_length is %d",tds_cs_domain_bar_timer_length);
	 }
	 else
	 {
	   if (!tdsrrcnv_get_default_value(TDS_RRC_NV_CS_FAIL_BAR_TIME,
									  (byte *) &tds_cs_domain_bar_timer_length,
									  sizeof(tds_cs_domain_bar_timer_length),
									  TRUE))
	   {
		  tds_cs_domain_bar_timer_length = TDS_CS_DOMAIN_BAR_TIME_LENGTH;
		  TDSRRC_MSG1(MSG_LEGACY_ERROR,"tds_cs_domain_bar_timer_length item error. Setting to default: %d",tds_cs_domain_bar_timer_length);
	   }
	   TDSRRC_MSG1(MSG_LEGACY_ERROR,"Read failed for tds_cs_domain_bar_timer_length,set default %d",tds_cs_domain_bar_timer_length);
	 }
}


/*===========================================================================

FUNCTION          RRC_CSP_WRITE_NV_ACQ_INFO

DESCRIPTION       This function saves the stored acquisition database and the 
                  acquisition list into the NV.
           
DEPENDENCIES
                  None

RETURN VALUE
                  None.

SIDE EFFECTS
                  None.

NOTE
                  None.
===========================================================================*/
void  tdsrrccsp_write_nv_acq_info(void)
{
  int  i;
  int  j;

  /* FR 3256 write L1 NV as well */
  tdsl1_write_nv_items();
  
  /* Do a Sanity check - check read and write index. */
  if ((tdsrrccsp_stored_acq_db.curr_wr_index > TDSMAX_ACQ_DB_INDEX) || 
      (tdsrrccsp_stored_acq_db.curr_rd_index > TDSMAX_ACQ_DB_INDEX) ||
      (tdsrrccsp_stored_acq_db.curr_rd_index < -1) ||
      (tdsrrccsp_stored_acq_db.curr_rd_index > tdsrrccsp_stored_acq_db.curr_wr_index) ||
      (tdsrrccsp_stored_acq_db.curr_wr_index < 0))
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"Acq db invalid,Wr index %d Rd index %d. Re-init ACQ DB",tdsrrccsp_stored_acq_db.curr_wr_index,tdsrrccsp_stored_acq_db.curr_rd_index);

    /* Re-initialize the Acquisition database */
    tdsrrccsp_init_acq_db();
  }

  if (!TDSCHECK_OPTIONAL_FEATURE_ENABLE_ACQ_DB_AUTO_SAVE)
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"ACQ DB auto-save not enabled NV66024=0x%x,skip saving ACQ DB",tds_nv_optional_feature_list);
    return;
  }
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Writing TDS_RRC_NV_ACQ_LIST");

  /* Write NV_ACQ_LIST_1_I to NV */
  for( i=0; i< TDS_MAX_ACQ_DB_ENTRIES; i++ )
  {
    tdsrrccsp_nv_acq_list[i].plmn_id.mcc[0] = tdsrrccsp_acq_list[i].plmn_id.mcc[0];
    tdsrrccsp_nv_acq_list[i].plmn_id.mcc[1] = tdsrrccsp_acq_list[i].plmn_id.mcc[1];
    tdsrrccsp_nv_acq_list[i].plmn_id.mcc[2] = tdsrrccsp_acq_list[i].plmn_id.mcc[2];
    tdsrrccsp_nv_acq_list[i].plmn_id.num_mnc_digits = tdsrrccsp_acq_list[i].plmn_id.num_mnc_digits;
    
    for(j=0; j<tdsrrccsp_acq_list[i].plmn_id.num_mnc_digits; j++)
    {
      tdsrrccsp_nv_acq_list[i].plmn_id.mnc[j] = tdsrrccsp_acq_list[i].plmn_id.mnc[j];
    }

    tdsrrccsp_nv_acq_list[i].freq = (uint32) tdsrrccsp_acq_list[i].freq;
    tdsrrccsp_nv_acq_list[i].status_mask = (uint32) tdsrrccsp_acq_list[i].status_mask;
    tdsrrccsp_nv_acq_list[i].cell_id = tdsrrccsp_acq_list[i].cell_id;
    tdsrrccsp_nv_acq_list[i].cpid = (uint32) tdsrrccsp_acq_list[i].cpid;
    tdsrrccsp_nv_acq_list[i].lac = (uint32) tdsrrccsp_acq_list[i].lac;

    TDSRRC_MSG3(MSG_LEGACY_MED,"  Write TDS_RRC_NV_ACQ_LIST[%d]: Freq=%d, CPID=%d", 
                i, 
                tdsrrccsp_nv_acq_list[i].freq, 
                tdsrrccsp_nv_acq_list[i].cpid);
  }

  if (TDS_MAX_ACQ_DB_NV_ENTRIES > TDS_MAX_ACQ_DB_ENTRIES)
  {
    memset(&tdsrrccsp_nv_acq_list[TDS_MAX_ACQ_DB_ENTRIES], 0, (TDS_MAX_ACQ_DB_NV_ENTRIES-TDS_MAX_ACQ_DB_ENTRIES)*sizeof(tds_nv_acq_entry_type));
  }

  if(tdsrrcnv_efs_write(TDS_RRC_NV_ACQ_LIST, (byte *) &tdsrrccsp_nv_acq_list, sizeof(tdsrrccsp_nv_acq_list)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wrote TDS_RRC_NV_ACQ_LIST successfully");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDS_RRC_NV_ACQ_LIST not written");
  }
  
  /* Write NV_ACQ_DB_I to NV */
  /*lint -e732*/
  tdsrrccsp_nv_acq_db.curr_rd_index = tdsrrccsp_stored_acq_db.curr_rd_index;
  tdsrrccsp_nv_acq_db.curr_wr_index = tdsrrccsp_stored_acq_db.curr_wr_index;
  /*lint +e732*/
  tdsrrccsp_nv_acq_db.last_entry_invalid = tdsrrccsp_stored_acq_db.last_entry_invalid;

  for( i=0; i< TDS_MAX_ACQ_DB_ENTRIES; i++ )
  {
    tdsrrccsp_nv_acq_db.acq_list_indices[i] = tdsrrccsp_stored_acq_db.acq_list_indices[i];
    TDSRRC_MSG2(MSG_LEGACY_MED,"ACQ list index for entry %d = %d",i,tdsrrccsp_stored_acq_db.acq_list_indices[i]);
  }

  if (TDS_MAX_ACQ_DB_NV_ENTRIES > TDS_MAX_ACQ_DB_ENTRIES)
  {
    memset(&tdsrrccsp_nv_acq_db.acq_list_indices[TDS_MAX_ACQ_DB_ENTRIES], 0, (TDS_MAX_ACQ_DB_NV_ENTRIES-TDS_MAX_ACQ_DB_ENTRIES)*sizeof(uint8));
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Writing TDS_RRC_NV_ACQ_DB rd=%d, wr=%d, b_last_entry_invalid=%d", 
              tdsrrccsp_stored_acq_db.curr_rd_index, 
              tdsrrccsp_stored_acq_db.curr_wr_index, 
              tdsrrccsp_stored_acq_db.last_entry_invalid);
  
  if(tdsrrcnv_efs_write(TDS_RRC_NV_ACQ_DB, (byte *) &tdsrrccsp_nv_acq_db, sizeof(tdsrrccsp_nv_acq_db)))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Wrote TDS_RRC_NV_ACQ_DB successfully");
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"TDS_RRC_NV_ACQ_DB not written");
  }

  
}  /* tdsrrccsp_write_nv_acq_info */






/*===========================================================================

FUNCTION          RRC_CSP_ADD_ACQ_ENTRY

DESCRIPTION       This function adds an acquisition entry to the acquisition 
                  list. While doing so, it also updates the acquisition
                  database pointers so that the 1st pointer points to this
                  most recent acquisition entry in acquisition list.
                   
           
DEPENDENCIES
                  None

RETURN VALUE
                  Success/Failure. Failure will be an extra-ordinary event and
                  could be due to irreversible causes like corruption of pointers.

SIDE EFFECTS
                  None.

===========================================================================*/
uecomdef_status_e_type  tdsrrccsp_add_acq_entry(tdsrrc_csp_acq_entry_type* acq_entry_ptr)
{

  int   wr_acq_list_index;
  int   i;
  int   ptr_index;
  int   num_valid_entries;
  tdsrrc_csp_band_class_type band_class;
  

  /* Do a Sanity check - check read and write index. A serious error if one
   * of them is out of bounds 
   */
  if ((tdsrrccsp_stored_acq_db.curr_wr_index > TDSMAX_ACQ_DB_INDEX) || 
      (tdsrrccsp_stored_acq_db.curr_rd_index > TDSMAX_ACQ_DB_INDEX) ||
      (tdsrrccsp_stored_acq_db.curr_rd_index < -1) ||
      (tdsrrccsp_stored_acq_db.curr_rd_index > tdsrrccsp_stored_acq_db.curr_wr_index))
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"ERROR: Acq db invalid Wr index %d Rd index %d",tdsrrccsp_stored_acq_db.curr_wr_index,tdsrrccsp_stored_acq_db.curr_rd_index);
    return(FAILURE);
  }

  /* Compute Number of Valid entries */
  if ((tdsrrccsp_stored_acq_db.curr_wr_index == TDSMAX_ACQ_DB_INDEX) && 
      (tdsrrccsp_stored_acq_db.last_entry_invalid == FALSE))
  {
    /* If all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is the last one and is Valid.
     */
    num_valid_entries = tdsrrccsp_stored_acq_db.curr_wr_index + 1;
  }
  else 
  {
    /* If not all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is Invalid and still needs to be initialized.
     */
    num_valid_entries = tdsrrccsp_stored_acq_db.curr_wr_index;
  }

  /* Get a matching acq. entry in acq. list that matches the PLMN and freq with 
   * the entry passed as a parameter.
   */
  for (i = 0; i < num_valid_entries; i++)
  {
    if ((tdsrrc_bcch_plmns_match(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id, 
                              acq_entry_ptr->plmn_id)) &&
        (tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq == acq_entry_ptr->freq))
    {
      /* Here, we found an acq. entry with same PLMN and freq. This acq. entry 
       * is overwritten with the acq. data provided in the parameter. All acq.
       * database pointers, starting from the first (topmost) and going till the
       * acq. database pointer that pointed to this dup entry, are moved one 
       * downwards, freeing the topmost pointer which is made to point to this
       * previously-dup-but-now-updated acq. entry. Note that in this process, the
       * acq. database pointer pointing to dup entry is not moved downwards and is
       * overwritten by the pointer prior to it.
       * Since the total number of acq. entries has not changed, the write index
       * is unaffected.
       */
      wr_acq_list_index = tdsrrccsp_stored_acq_db.acq_list_indices[i];

      for (ptr_index = i; ptr_index > 0; ptr_index--)
      {
        tdsrrccsp_stored_acq_db.acq_list_indices[ptr_index] = 
          tdsrrccsp_stored_acq_db.acq_list_indices[ptr_index-1];
      }

      tdsrrccsp_acq_list[wr_acq_list_index] = *acq_entry_ptr;
      band_class = tdsrrc_get_frequency_band(acq_entry_ptr->freq);
      tdsrrccsp_acq_list[wr_acq_list_index].band_class = band_class;

      /* Initialize the topmost pointer to the most recent acq. list entry */
      tdsrrccsp_stored_acq_db.acq_list_indices[0] = (uint8)wr_acq_list_index;
      /* Return always a Success for the time being */
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Updated: freq %d cpid %d band 0x%x in Acq DB", 
                              acq_entry_ptr->freq, acq_entry_ptr->cpid, band_class);

      return (SUCCESS);
    }
  }


  /* Initialize write pointer to the acquisition entry in acquisition list where
   * the new acq. data can be stored. If all entries in acq. list are already
   * filled, the oldest entry as pointed to by the "last" pointer in acq. database
   * is to be overwritten and to be made the most recent entry.
   */

  if ((tdsrrccsp_stored_acq_db.curr_wr_index == TDSMAX_ACQ_DB_INDEX) &&
      (tdsrrccsp_stored_acq_db.last_entry_invalid == FALSE))
  {
    wr_acq_list_index = tdsrrccsp_stored_acq_db.acq_list_indices[tdsrrccsp_stored_acq_db.curr_wr_index];
  }
  else
  {
    wr_acq_list_index = tdsrrccsp_stored_acq_db.curr_wr_index;
  }

  if((wr_acq_list_index > TDSMAX_ACQ_DB_INDEX) || (wr_acq_list_index < 0))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid write index.");
    return(FAILURE);
  }

  /* Copy the contents now */
  tdsrrccsp_acq_list[wr_acq_list_index] = *acq_entry_ptr;

  band_class = tdsrrc_get_frequency_band(acq_entry_ptr->freq);
  tdsrrccsp_acq_list[wr_acq_list_index].band_class = band_class;

  /* Now push all pointers in acq. database downwards and copy the pointer to 
   * this acq. entry in the first (topmost) acq. database pointer.
   */
  for (i = tdsrrccsp_stored_acq_db.curr_wr_index; i > 0; i--)
  {
    tdsrrccsp_stored_acq_db.acq_list_indices[i] = 
      tdsrrccsp_stored_acq_db.acq_list_indices[i-1];
  }
  /* Initialize the topmost pointer to the most recent acq. list entry */
  tdsrrccsp_stored_acq_db.acq_list_indices[0] = (uint8)wr_acq_list_index;

  /* Increment current number of valid acquisition database pointers if 
   * not all acquisition db pointers have been initialized so far.
   */
  if (tdsrrccsp_stored_acq_db.curr_wr_index < TDSMAX_ACQ_DB_INDEX)
  {
    tdsrrccsp_stored_acq_db.curr_wr_index++;
    tdsrrccsp_stored_acq_db.last_entry_invalid = TRUE;
  }
  else
  {
    tdsrrccsp_stored_acq_db.last_entry_invalid = FALSE;
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Added: freq %d cpid %d band 0x%x in Acq DB", 
                              acq_entry_ptr->freq, acq_entry_ptr->cpid, band_class);
  /* Return always a Success for the time being */
  return (SUCCESS);

}  /* tdsrrccsp_add_acq_entry */


/*===========================================================================

FUNCTION          RRC_CSP_SHOULD_FREQ_BE_SCANNED

DESCRIPTION       This function checks whether an acquisition has already been
                  tried on the indicated frequency as part of acquisition database
                  scan.

DEPENDENCIES
                  None.

RETURN VALUE
                  Boolean. False indicates the frequency has been scanned before
                           True otherwise

SIDE EFFECTS
                  None.

===========================================================================*/
boolean
tdsrrccsp_should_freq_be_scanned(rrc_requested_plmn_type req_plmn, tdsrrc_freq_type freq)
{
  int i;
  int num_valid_entries;
  tdsrrc_state_e_type rrc_current_state = tdsrrc_get_state();

  /* When inter frequency redirection scan is in progress, 
   * UE should not attempt acquisition on the last camped frequency till
   * the expiry of the wait timer. Since this function has been called,
   * it implies that the wait timer has not expired. Hence, always return
   * FALSE, i.e., do not scan this frequency.
   */
  if((rrc_current_state == TDSRRC_STATE_CONNECTING) 
      && (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN) 
        && (tdsrrc_csp_int_data.last_camped_freq == freq))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"freq Rdrct: Do not attempt acq on last camped freq %d during freq scan",tdsrrc_csp_int_data.last_camped_freq);
    return FALSE;
  }



  /* If UE is in connected mode then always scan the frequency - performance;
   * Or if in idle mode the frequency found during full scan is the same as the last
   * camped frequency then scan this frequency again - performance.
   */
  if(((rrc_current_state != TDSRRC_STATE_DISCONNECTED)
      || (freq == tdsrrc_csp_int_data.last_camped_freq_in_idle_mode))
    && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
    && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2TD_BPLMN_MANUAL_SCAN)
    && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2G_BPLMN_AUTOMATIC_SCAN)
    && (tdsrrc_csp_int_data.curr_scan != RRC_CSP_TD2G_BPLMN_MANUAL_SCAN))
  {
    return TRUE;
  }

  /* Compute Number of Valid entries */
  if ((tdsrrccsp_stored_acq_db.curr_wr_index == TDSMAX_ACQ_DB_INDEX) &&
      (tdsrrccsp_stored_acq_db.last_entry_invalid == FALSE))
  {
    /* If all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is the last one and is Valid.
     */
    num_valid_entries = tdsrrccsp_stored_acq_db.curr_wr_index + 1;
  }
  else
  {
    /* If not all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is Invalid and still needs to be initialized.
     */
    num_valid_entries = tdsrrccsp_stored_acq_db.curr_wr_index;
  }  

  /* Now look for an acq. entry that has the PLMN to be matched */
  for (i = 0; i < num_valid_entries; i++)
  {
    /* For SPEC and ANY PLMN_REQ, all the acquisition entries are scanned */
    if(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq == freq)
    { 
      /* Frequency and PLMN found in the acquisition database. Now check if the band 
       * corresponding to this frequency is supported by the UE or not. If not, then
       * during ACQ DB scan this frequency was not scanned and hence needs to be scanned
       * during full frequency scan. Otherwise, this freq was scanned during ACQ DB scan
       * and hence there is no need to re-scan this frequency.
       */
      if(tdsrrccsp_is_supported_band(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class))
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Freq and band found in acq db,don't scan");
        return FALSE;
      }
    }
  }

  return TRUE;

} /* tdsrrccsp_should_freq_be_scanned */


/*===========================================================================

FUNCTION          RRC_CSP_HAS_ACQ_DB_FREQ_BEEN_SCANNED

DESCRIPTION       This function avoids multiple acquisition attempts on 
                  multiple entries of the same frequency in the acquisition 
                  database during any plmn scan.
                  
DEPENDENCIES
                  None.

RETURN VALUE
                  Boolean. True  indicates the frequency has been scanned before
                           False otherwise

SIDE EFFECTS
                  None.

===========================================================================*/
static boolean
tdsrrccsp_has_acq_db_freq_been_scanned(tdsrrc_freq_type freq)
{
  uint8 i;

  for (i = 0; i < tdsrrccsp_acq_db_freq_scanned_list.wr_index; i++)
  {
    if(freq == tdsrrccsp_acq_db_freq_scanned_list.freq[i])
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Acq DB Freq %d already scanned. Don't scan",freq);
      return TRUE;
    }             
  } 

 /* When inter frequency redirection scan is in progress, 
  * UE should not attempt acquisition on the last camped frequency till
  * the expiry of the wait timer. Since this function has been called,
  * it implies that the wait timer has not expired. Hence, always return
  * FALSE, i.e., do not scan this frequency.
  */
  if((tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_INTER_FREQ_REDIRECTION_SCAN) 
      && (tdsrrc_csp_int_data.last_camped_freq == freq))
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Freq Redirect wt tmr has !expired for freq %d. Don't scan",freq);
    return TRUE;
  }

  TDSRRC_MSG1(MSG_LEGACY_MED,"Freq %d NOT scanned during Acq DB scan. Scan",freq);
  return FALSE;
} /* tdsrrccsp_has_acq_db_freq_been_scanned */


/*===========================================================================

FUNCTION          RRC_CSP_UPDATE_ACQ_DB_FREQ_SCANNED_LIST

DESCRIPTION       This function adds the acq db frequency to the scanned list.
                  
DEPENDENCIES
                  None.

RETURN VALUE
                  Boolean. True  indicates the frequency has been scanned before
                           False otherwise

SIDE EFFECTS
                  None.

===========================================================================*/
static void
tdsrrccsp_update_acq_db_freq_scanned_list(tdsrrc_freq_type freq)
{ 
  
  if(tdsrrccsp_acq_db_freq_scanned_list.wr_index >= TDS_MAX_FREQ_SCANNED_ENTRIES)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Max # of available entries in freq scanned list exceeded!");
    tdsrrccsp_acq_db_freq_scanned_list.wr_index = 0;
  }

  /* Add Frequency to the list */
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Adding freq %d to the scanned list",freq);
  tdsrrccsp_acq_db_freq_scanned_list.freq[tdsrrccsp_acq_db_freq_scanned_list.wr_index++] = freq;

  return;
} /* tdsrrccsp_update_acq_db_freq_scanned_list */


/*===========================================================================

FUNCTION          RRC_CSP_SHOULD_ALL_ACQ_DB_BE_SCANNED

DESCRIPTION       This function determines if for a particular CSP scan type
                  the complete ACQ DB should be scanned or not. The decision is
                  based on the CSP scan type and whether full frequency scan
                  is required or not
           
DEPENDENCIES
                  None

RETURN VALUE
                  Boolean
                  TRUE: Complete ACQ DB should be scanned
                  FALSE: Otherwise.
                  
SIDE EFFECTS
                  None.

===========================================================================*/
static boolean tdsrrccsp_should_all_acq_db_be_scanned(void)
{
  boolean status = TRUE;

  switch(tdsrrc_csp_int_data.curr_scan)
  {
    case TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN:
    case TDSRRC_CSP_LEAVING_CONN_MODE_SCAN:
      status = FALSE;
      break;
    case TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN:
    case TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN:
    case TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN:
#ifdef FEATURE_RRC_FREQ_REDIRECTION_THRU_RB_AND_CU
    case TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN:
    case TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN:
    case TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN:
#endif
      if(tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles > 1)
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Do NOT scan ALL ACQ DB. cycle# %d",tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles);
        status = FALSE;
      }
      break;

    case RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN:
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
      /* Under this feature we always scan all ACQ DB entries as we are not sure 
         which PLMN NAS is interested in. NAS needs to be reported with all the PLMNs 
         available till specific PLMN is found */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Do not skip ACQ DB entry scan");
#else
      if(   (tdsrrc_csp_int_data.bplmn_req_list_rd_index != (tdsrrc_csp_int_data.bplmn_num_req - 1))
          || ((!tdsrrc_csp_int_data.plmn_list_freq_scan_needed)
                && (!tdsrrc_csp_int_data.acq_db_empty)))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Do NOT scan ALL ACQ DB. rd_ix %d, num_req %d, fscan_needed %d", 
                                       tdsrrc_csp_int_data.bplmn_req_list_rd_index, 
                                       tdsrrc_csp_int_data.bplmn_num_req, 
                                       tdsrrc_csp_int_data.plmn_list_freq_scan_needed);

        /* Was FALSE for FDD. TDS mostly should be LFS */
        status = TRUE; 
      }
#endif /* FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN */
      break;

    case RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN:
#ifdef FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN
      /* Under this feature we always scan all ACQ DB entries as we are not sure 
         which PLMN NAS is interested in. NAS needs to be reported with all the PLMNs 
         available till specific PLMN is found */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Do not skip ACQ DB entry scan");
#else
      if(   (tdsrrc_csp_int_data.bplmn_req_list_rd_index != (tdsrrc_csp_int_data.wtow_bplmn_num_req - 1))
          || ((!tdsrrc_csp_int_data.plmn_list_freq_scan_needed)
                && (!tdsrrc_csp_int_data.acq_db_empty)))
      {
        TDSRRC_MSG3(MSG_LEGACY_HIGH,"Do NOT scan ALL ACQ DB. rd_ix %d, num_req %d, fscan_needed %d", 
                                       tdsrrc_csp_int_data.bplmn_req_list_rd_index, 
                                       tdsrrc_csp_int_data.wtow_bplmn_num_req, 
                                       tdsrrc_csp_int_data.plmn_list_freq_scan_needed);

        /* Was FALSE for FDD. TDS mostly should be LFS */
        status = TRUE;
      }
#endif /* FEATURE_HPPLMN_SRCH_WITH_MANY_PREF_PLMN */
      break;


    case TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN:
      if(    (tdsrrc_get_state() == TDSRRC_STATE_CELL_DCH)
          && (tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles > 1))
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Do NOT scan ALL ACQ DB. cycle# %d",tdsrrc_csp_curr_select_data.num_acq_db_and_dest_freq_cycles);
        status = FALSE;
      }
      break;


    default:
      break;
  }

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Scan Type %d. Scan ALL Acq DB Status = %d",tdsrrc_csp_int_data.curr_scan,status);
  return (status);

} /* tdsrrccsp_should_all_acq_db_be_scanned */



/*===========================================================================

FUNCTION          RRC_CSP_GET_MATCH_ACQ_ENTRY

DESCRIPTION       This function gets the entry,from the acquisition list that 
                  matches the criteria given in the parameters. It starts 
                  looking for match starting from the acq. database pointer 
                  next to the one indexed by current read index.
           
DEPENDENCIES
                  This function is closely knit with tdsrrccsp_compare_expected_cell()
                  and tdsrrccsp_continue_stored_initial_select_ctrl(). 
                  Any change in this function's logic should be reflected in others.

RETURN VALUE
                  Pointer to acquisition entry. A NULL pointer means no 
                  acquisition entry was found that fulfilled selection criteria.
SIDE EFFECTS
                  None.

===========================================================================*/
static tdsrrc_csp_acq_entry_type*  
tdsrrccsp_get_match_acq_entry(rrc_plmn_identity_type req_plmn_id, tdsrrc_csp_acq_e_type req_type_acq)
{

  int i;
  int num_valid_entries;

  static tdsrrc_csp_acq_entry_type acq_entry_for_pfl;  
  tdsrrc_csp_acq_entry_type  *ret_acq_entry_ptr = NULL;
  boolean is_eplmn = FALSE;
  boolean is_network_shared = FALSE;
  boolean is_acq_db_plmn_matched = FALSE;

  /* Do a Sanity check - check read and write index. A serious error if one
   * of them is out of bounds  Also read index should never be higher than 
   * write index.
   */
  num_valid_entries = tdsrrccsp_assert_acq_db_vars();


  switch(req_type_acq)
  {
    case TDSRRC_CSP_SPEC_PLMN:
      
      /* if specific plmn is not found when UE is in deep sleep then
       * automatically look for any plmn in acq db 
       */
      if(!tdsrrc_csp_int_data.deep_sleep_acq_db_plmn_searchloop)
      {
        
        for (i = tdsrrccsp_stored_acq_db.curr_rd_index + 1; i < num_valid_entries; i++)
        {
          is_network_shared = tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].is_valid_nw_sharing;
          /* Resetting the is_eplmn variable at start of loop */
          is_eplmn = FALSE;
          if(    (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN)
              || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_LEAVING_CONN_MODE_SCAN)
              || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_FACH_SCAN)
              || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_CELL_PCH_SCAN)
              || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_DCH_TO_URA_PCH_SCAN)
              || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_FACH_SCAN)
              || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_CELL_PCH_SCAN)
              || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_FACH_TO_URA_PCH_SCAN)
              /* 
               * we need it now bcoz for rl_failure the scan type is no longer dch->fach
               * and is always oos_area_scan. 
               */
              || (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
              || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
            )
          {
            if((tdsrrccsp_check_for_eplmn(
                    mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tdsrrc_csp_int_data.curr_req_plmn.plmn_id))) == TRUE)
            {
              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Curr requested PLMN is in the NAS equivalent PLMN list");
              if((!is_network_shared) &&
                 (tdsrrccsp_check_for_eplmn(
                    mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id)) == TRUE))
              {
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"PLMN %d-%d is in the eq PLMN list",TDSRRC_CSP_GET_INT_MCC(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id),TDSRRC_CSP_GET_INT_MNC(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id));
                is_eplmn = TRUE;
              }
              else if((is_network_shared) && 
                 ((tdsrrccsp_nas_plmn_equivalent_to_any_of_shared_bcch_plmns(
                                                                          tdsrrc_csp_int_data.curr_req_plmn.plmn_id,
                                                                          tdsrrccsp_stored_acq_db.acq_list_indices[i])) == TRUE))
              {
                is_eplmn = TRUE;
              }
            }
          }
          
          if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
          {
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"ACQ DB PLMN being compared %d-%d. Freq %d",
                     TDSRRC_CSP_GET_INT_MCC(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id),
                     TDSRRC_CSP_GET_INT_MNC(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id),
                     tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq);
          }
          is_acq_db_plmn_matched = FALSE;
          if((!is_network_shared) &&
             (tdsrrc_bcch_and_nas_plmns_match(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id, 
                                           req_plmn_id)))
          {
            is_acq_db_plmn_matched = TRUE;
          }
          else if((is_network_shared) &&
                  (tdsrrccsp_nas_plmn_match_any_of_bcch_plmns(
                                                       req_plmn_id,
                                                       tdsrrccsp_stored_acq_db.acq_list_indices[i])))
          {
            is_acq_db_plmn_matched = TRUE;
          }
          /* haizhoul 2013/1/5 Do not do frequency pruning when get frequency from ACQ DB*/
          if((is_acq_db_plmn_matched
              || (is_eplmn)
              )
			  #if 0
                && (!tdsrrccsp_should_freq_be_skipped(
                tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq, TDSRRCCSP_FINE_SCAN_SKIP_RANGE))
              #endif
                && (tdsrrccsp_is_supported_band(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class))
                && (!tdsrrccsp_has_acq_db_freq_been_scanned(
                          tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq)))
          {
            /* A suitable entry is found. Set the Read index to this db pointer entry and 
             * return pointer to the acquisition list contained in this db pointer entry
             */
            tdsrrccsp_stored_acq_db.curr_rd_index = (int8)i;
            ret_acq_entry_ptr = &tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]];
  
            /* Update scanned list with the frequency to avoid multiple acquisition
             * attempts on the same frequency
             */
            tdsrrccsp_update_acq_db_freq_scanned_list(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq);
            tdsrrc_csp_int_data.acq_db_empty = FALSE;
            break;
          }
        }
      }
      else
      {
        /* If ue is in deep sleep then search for any plmn if
         * specific plmn is not found/could not acquire.
         */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Searching for Any PLMN; looping = TRUE(%d)",tdsrrc_csp_int_data.deep_sleep_acq_db_plmn_searchloop);
      }
      break;

    case TDSRRC_CSP_ANY_PLMN:
      i = tdsrrccsp_stored_acq_db.curr_rd_index + 1;
      while(i < num_valid_entries)
      {
        /* haizhoul 2013/1/5 Do not do frequency pruning when get frequency from ACQ DB*/
        if((!(tdsrrccsp_is_supported_band(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class))) ||
           #if 0
           (tdsrrccsp_should_freq_be_skipped(
             tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq, TDSRRCCSP_FINE_SCAN_SKIP_RANGE)) ||
           #endif
           (tdsrrccsp_has_acq_db_freq_been_scanned(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq)))
        {
          i++;
        }
        else
        {
          tdsrrccsp_stored_acq_db.curr_rd_index = (int8)i;
          ret_acq_entry_ptr = &tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]];

          /* Update scanned list with the frequency to avoid multiple acquisition
           * attempts on the same frequency
           */
          tdsrrccsp_update_acq_db_freq_scanned_list(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq);
          tdsrrc_csp_int_data.acq_db_empty = FALSE;
          break;
        }
      }

      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid req acq type %d",req_type_acq);
      break;
  }

  /* If ret_acq_entry_ptr is NULL and if UE is in deep sleep then try all PLMNs 
   * in the Acq DB before doing a full frequency scan.
   */
  if(    (!ret_acq_entry_ptr)
      && (req_type_acq == TDSRRC_CSP_SPEC_PLMN)
      && (tdsrrccsp_should_all_acq_db_be_scanned())
    )
  {
    /* reset tdsrrccsp_stored_acq_db.curr_rd_index to search the acq db again */
    if(!tdsrrc_csp_int_data.deep_sleep_acq_db_plmn_searchloop)
    {
      tdsrrc_csp_int_data.deep_sleep_acq_db_plmn_searchloop = TRUE;
      tdsrrccsp_stored_acq_db.curr_rd_index                        = -1;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Scan ALL ACQ DB entries now. curr_rd_index=-1 and looping=1");
    }

    /* Look in the acq db for any PLMN. 
     * Code same as in the switch case TDSRRC_CSP_ANY_PLMN above.
     */
    i = tdsrrccsp_stored_acq_db.curr_rd_index + 1;
    while(i < num_valid_entries)
    {
      is_network_shared = tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].is_valid_nw_sharing;
      /* haizhoul 2013/1/5 Do not do frequency pruning when get frequency from ACQ DB*/
      if( 
	      #if 0
	      (tdsrrccsp_should_freq_be_skipped(
               tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq, TDSRRCCSP_FINE_SCAN_SKIP_RANGE)) 
          || 
          #endif
          (!tdsrrccsp_is_supported_band(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class))
          || ((!is_network_shared && 
               tdsrrc_bcch_and_nas_plmns_match(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id, 
                                            req_plmn_id))
              || (is_network_shared &&
                  tdsrrccsp_nas_plmn_match_any_of_bcch_plmns(
                                                       req_plmn_id,
                                                       tdsrrccsp_stored_acq_db.acq_list_indices[i]))
              )
          || (tdsrrccsp_has_acq_db_freq_been_scanned(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq)))
      {
        i++;
      }
      else
      {
        tdsrrccsp_stored_acq_db.curr_rd_index = (int8)i;
        ret_acq_entry_ptr = &tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]];

        /* Update scanned list with the frequency to avoid multiple acquisition
         * attempts on the same frequency
         */
        tdsrrccsp_update_acq_db_freq_scanned_list(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq);
        break;
      }
    }

	/* Look in the prim freq list for any PLMN, same logic as TDSRRC_CSP_ANY_PLMN above. */
    if(!ret_acq_entry_ptr)
    {
      tdsrrc_csp_int_data.tds_prim_freq_list_in_use = TRUE;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"prim freq list in use for auto BPLMN: %d",tdsrrc_csp_int_data.tds_prim_freq_list_in_use);
      i = tdsrrc_csp_int_data.tds_prim_freq_list_rd_index + 1;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Scan prim freq list entries,curr list index %d",i);
      memset(&acq_entry_for_pfl,0,sizeof(tdsrrc_csp_acq_entry_type));
      while (i < tdsrrc_csp_int_data.num_tds_prim_freq)
      {
        acq_entry_for_pfl.freq = tdsrrc_csp_int_data.tds_prim_freq_list[i];
        acq_entry_for_pfl.band_class = tdsrrc_get_frequency_band(acq_entry_for_pfl.freq);
        if(!tdsrrccsp_is_supported_band(acq_entry_for_pfl.band_class) ||
        	   tdsrrccsp_has_acq_db_freq_been_scanned(acq_entry_for_pfl.freq))
        {
          i++;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"ignored prim freq %d,list index %d",acq_entry_for_pfl.freq,i);
        }
        else
        {
          tdsrrc_csp_int_data.tds_prim_freq_list_rd_index = (int8)i;
          ret_acq_entry_ptr = &acq_entry_for_pfl;
          TDSRRC_MSG2(MSG_LEGACY_HIGH,"selected prim freq %d,list index %d",acq_entry_for_pfl.freq,i);
          tdsrrccsp_update_acq_db_freq_scanned_list(acq_entry_for_pfl.freq);
          break;
        }
      }      
    }

    /* if no PLMN is found then reset deep_sleep_acq_db_plmn_searchloop 
     * for future searches in deep sleep
     */

    if(!ret_acq_entry_ptr)
    {
      tdsrrc_csp_int_data.deep_sleep_acq_db_plmn_searchloop = FALSE;
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Exhausted ALL ACQ DB entries scanning. Looping=%d",tdsrrc_csp_int_data.deep_sleep_acq_db_plmn_searchloop);
	  tdsrrc_csp_int_data.tds_prim_freq_list_in_use = FALSE;
	  TDSRRC_MSG1(MSG_LEGACY_HIGH,"prim freq list in use for auto BPLMN: %d",tdsrrc_csp_int_data.tds_prim_freq_list_in_use);
    }
                                                                                                                    
  } /* end ret_acq_entry_ptr is NULL and ue_in_deep_sleep */

  return (ret_acq_entry_ptr);

}  /* tdsrrccsp_get_match_acq_entry */

/*===========================================================================

FUNCTION          RRC_CSP_GET_FIRST_MATCH_ACQ_ENTRY

DESCRIPTION       This function gets the most recent entry from the acquisition 
                  list that matches the criteria given in the parameters. 
                  
                  The most recent acquisition entry is based on the order of 
                  pointers in acquisition database and NOT on the order of entries
                  in acquisition list. 
                  An acquisition entry pointed to by a pointer that occurs before
                  in the acquisition database pointer list is considered more 
                  recent than another entry which is pointed to by a pointer which
                  occurs later in the acquisition database pointer list.
                   
           
DEPENDENCIES
                  None

RETURN VALUE
                  Pointer to acquisition entry. A NULL pointer means no 
                  acquisition entry was found that fulfilled selection criteria.
SIDE EFFECTS
                  None.

===========================================================================*/
tdsrrc_csp_acq_entry_type*  
tdsrrccsp_get_first_match_acq_entry(rrc_plmn_identity_type req_plmn_id, tdsrrc_csp_acq_e_type req_type_acq)
{
  tdsrrccsp_last_req_type_acq = req_type_acq;
  tdsrrccsp_last_req_plmn_id = req_plmn_id;

  /* Reset Read Index of Acq. database to -1 */
  tdsrrccsp_stored_acq_db.curr_rd_index = -1;

  /* Reset number of unique acq db frequencies already scanned */

  /* If Automatic BPLMN search is in progress then determine if 
   * the variables need to be reset or not based on the read_index
   * of the Higher Priority PLMN list
   */
  if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
      || (tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN))
  {
    if(tdsrrc_csp_int_data.bplmn_req_list_rd_index == 0)
    {
      tdsrrccsp_acq_db_freq_scanned_list.wr_index = 0;

      /* Acq db empty variable is initialized to TRUE. The variable is updated if
       * any matching PLMNs are found in ACQ DB
       */
      tdsrrc_csp_int_data.acq_db_empty = TRUE;
    } 
  }
  /* For all other scans, always reset the following variables */
  else
  {
    tdsrrccsp_acq_db_freq_scanned_list.wr_index = 0;

    /* Acq db empty variable is initialized to TRUE. The variable is updated if
     * any matching PLMNs are found in ACQ DB
     */
    tdsrrc_csp_int_data.acq_db_empty = TRUE;
  }

  /* Reset deep_sleep_acq_db_search_loop */
  tdsrrc_csp_int_data.deep_sleep_acq_db_plmn_searchloop = FALSE;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Acq DB Search. Looping = %d and ue_in_deep_sleep = %d",tdsrrc_csp_int_data.deep_sleep_acq_db_plmn_searchloop,tdsrrc_csp_int_data.ue_in_deep_sleep); 

  /* Reset Read Index of prim freq list to -1 */
  tdsrrc_csp_int_data.tds_prim_freq_list_rd_index = -1;
  tdsrrc_csp_int_data.tds_prim_freq_list_in_use = FALSE;

  return(tdsrrccsp_get_match_acq_entry( req_plmn_id, req_type_acq));

}  /* tdsrrccsp_get_first_match_acq_entry */

/*===========================================================================

FUNCTION          RRC_CSP_GET_NEXT_MATCH_ACQ_ENTRY

DESCRIPTION       This function gets next entry from the acquisition list
                  that matches the criteria given in the parameters. 
                  To find the "next" entry, the acquisition database pointer list 
                  is read starting from the pointer next to the one pointed to by
                  curr read index.
                   
                  The most recent acquisition entry is based on the order of 
                  pointers in acquisition database and NOT on the order of entries
                  in acquisition list. 
                  An acquisition entry pointed to by a pointer that occurs before
                  in the acquisition database pointer list is considered more 
                  recent than another entry which is pointed to by a pointer that 
                  occurs later in the acquisition database pointer list.
           
DEPENDENCIES
                  None

RETURN VALUE
                  Pointer to acquisition entry. A NULL pointer means no acquisition
                  entry was found that fulfilled selection criteria.
SIDE EFFECTS
                  None.

===========================================================================*/
tdsrrc_csp_acq_entry_type*  
tdsrrccsp_get_next_match_acq_entry(rrc_plmn_identity_type req_plmn_id, tdsrrc_csp_acq_e_type req_type_acq)
{
  /* Compare the stored acq type from last call to rrc_csp_get_acq_entry() 
   * with the one passed here. Return NULL if they don't match.
   */
  if( tdsrrccsp_last_req_type_acq != req_type_acq )
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Passed acq type %d does not match last acq type %d",req_type_acq,tdsrrccsp_last_req_type_acq);
    return NULL;
  }

  /* If the acq. types match, compare the stored plmn ID passed in last call to 
   * rrc_csp_get_acq_entry() with the one passed here. Return NULL if they don't match.
   */
  if(tdsrrccsp_last_req_type_acq == TDSRRC_CSP_SPEC_PLMN)
  {
    if(!TDSRRC_CSP_SAME_MCC(req_plmn_id, tdsrrccsp_last_req_plmn_id))
    {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Passed plmn.MCC %d does not match last plmn.MCC %d",TDSRRC_CSP_GET_INT_MCC(req_plmn_id),TDSRRC_CSP_GET_INT_MCC(tdsrrccsp_last_req_plmn_id));
        return NULL;
    }
    if(!TDSRRC_CSP_SAME_MNC(req_plmn_id, tdsrrccsp_last_req_plmn_id))
    {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Passed plmn.MNC %d does not match last plmn.MNC %d",TDSRRC_CSP_GET_INT_MNC(req_plmn_id),TDSRRC_CSP_GET_INT_MNC(tdsrrccsp_last_req_plmn_id));
        return NULL;
    }
  }

  /* The initial sanity check is okay - proceed further and get the matching entry */
  return(tdsrrccsp_get_match_acq_entry( req_plmn_id, req_type_acq));

}  /* tdsrrccsp_get_next_match_acq_entry */


/*===========================================================================

FUNCTION          RRCCSP_BPLMN_UPDATE_STORED_ACQ_DB_RD_INDEX

DESCRIPTION       This function is used by CSP to update 
                  tdsrrccsp_stored_acq_db.curr_rd_index if BPLMN srch timer expires
                  before SIBs could be read.
                  
DEPENDENCIES      NONE

RETURN VALUE
                  
SIDE EFFECTS

===========================================================================*/
void tdsrrccsp_bplmn_update_stored_acq_db_rd_index ( )
{
  if (tdsrrc_csp_int_data.tds_prim_freq_list_in_use)
  {
    if (tdsrrc_csp_int_data.tds_prim_freq_list_rd_index > -1)
    {
	  --tdsrrc_csp_int_data.tds_prim_freq_list_rd_index;
    }
  }
  else
  {
  if (tdsrrccsp_stored_acq_db.curr_rd_index > -1)
  {
  --tdsrrccsp_stored_acq_db.curr_rd_index;
  }
  }
  
  if (tdsrrccsp_acq_db_freq_scanned_list.wr_index > 0)
  {
  --tdsrrccsp_acq_db_freq_scanned_list.wr_index;
  }
  TDSRRC_MSG3(MSG_LEGACY_HIGH,"BPLMN: Setting curr_rd_ix=%d & acq_db_scanned_list_wr_ix=%d & tds_prim_freq_list_rd_index=%d", 
                        tdsrrccsp_stored_acq_db.curr_rd_index, tdsrrccsp_acq_db_freq_scanned_list.wr_index,
                        tdsrrc_csp_int_data.tds_prim_freq_list_rd_index);
}


/*===========================================================================

FUNCTION          RRC_CSP_ASSERT_ACQ_DB_VARS

DESCRIPTION       This function is used to check the sanity of acq db entries

DEPENDENCIES      NONE

RETURN VALUE      number of entries in ACQ DB
                  
SIDE EFFECTS      NONE

===========================================================================*/
int tdsrrccsp_assert_acq_db_vars(void)
{
  ASSERT(tdsrrccsp_stored_acq_db.curr_wr_index <= TDSMAX_ACQ_DB_INDEX);
  ASSERT(tdsrrccsp_stored_acq_db.curr_rd_index <= TDSMAX_ACQ_DB_INDEX);
  ASSERT(tdsrrccsp_stored_acq_db.curr_rd_index >= -1);
  ASSERT(tdsrrccsp_stored_acq_db.curr_rd_index <= tdsrrccsp_stored_acq_db.curr_wr_index);

  /* Compute Number of Valid entries */
  if ((tdsrrccsp_stored_acq_db.curr_wr_index == TDSMAX_ACQ_DB_INDEX) && 
      (tdsrrccsp_stored_acq_db.last_entry_invalid == FALSE))
  {
    /* If all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is the last one and is Valid.
     */
    return (tdsrrccsp_stored_acq_db.curr_wr_index + 1);
  }
  else 
  {
    /* If not all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is Invalid and still needs to be initialized.
     */
    return (tdsrrccsp_stored_acq_db.curr_wr_index);
  }
} /* tdsrrccsp_assert_acq_db_vars */

/*===========================================================================

FUNCTION          tdsrrccsp_set_band_f_search_pattern

DESCRIPTION       This function is used to set band F search pattern according
                  to NV setting. And if it's not specified in NV, then use
                  default search pattern (lower band shallow and upper
                  band shallow) instead.

DEPENDENCIES      It's to invoke one time after read NV66024.

RETURN VALUE      NONE
                  
SIDE EFFECTS      NONE

===========================================================================*/
void tdsrrccsp_set_band_f_search_pattern(void)
{
  boolean set_flag = FALSE;
  
  /* Initialize band F search patterns */
  tdsrrccsp_band_f_pattern.lower_band_shallow = FALSE;
  tdsrrccsp_band_f_pattern.lower_band_deep    = FALSE;
  tdsrrccsp_band_f_pattern.upper_band_shallow = FALSE;
  tdsrrccsp_band_f_pattern.upper_band_deep    = FALSE;

  /* Set search patterns according to NV setting */
  if(TDSCHECK_OPTIONAL_FEATURE_LOWER_F_SHALOW_SET)
  {
    tdsrrccsp_band_f_pattern.lower_band_shallow = TRUE;
    set_flag = TRUE;
  }
  if(TDSCHECK_OPTIONAL_FEATURE_LOWER_F_DEEP_SET)
  {
    tdsrrccsp_band_f_pattern.lower_band_deep = TRUE;
    set_flag = TRUE;
  }
  if(TDSCHECK_OPTIONAL_FEATURE_UPPER_F_SHALOW_SET)
  {
    tdsrrccsp_band_f_pattern.upper_band_shallow = TRUE;
    set_flag = TRUE;
  }
  if(TDSCHECK_OPTIONAL_FEATURE_UPPER_F_DEEP_SET)
  {
    tdsrrccsp_band_f_pattern.upper_band_deep = TRUE;
    set_flag = TRUE;
  }
  /*For debug: show NV setting of band F search pattern*/
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Band F search pattern: lower shallow %d and deep %d",tdsrrccsp_band_f_pattern.lower_band_shallow,tdsrrccsp_band_f_pattern.lower_band_deep);
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Band F search pattern: upper shallow %d and deep %d",tdsrrccsp_band_f_pattern.upper_band_shallow,tdsrrccsp_band_f_pattern.upper_band_deep);  
  if(!set_flag)
  {
    /* Use default band F search patterns as nothing is specified in NV */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Use Band F default search pattern: lower shallow and upper shallow");
    tdsrrccsp_band_f_pattern.lower_band_shallow = TRUE;
    tdsrrccsp_band_f_pattern.upper_band_shallow = TRUE;
  }
  return;
}

/*===========================================================================

FUNCTION          tdsrrccsp_set_band_a_search_pattern

DESCRIPTION       This function is used to set band A search pattern according
                  to NV setting. And if it's not specified in NV, then use
                  default search pattern(shallow + deep) instead.

DEPENDENCIES      It's to invoke one time after read NV66024.

RETURN VALUE      NONE
                  
SIDE EFFECTS      NONE

===========================================================================*/
void tdsrrccsp_set_band_a_search_pattern(void)
{
  boolean set_flag = FALSE;
  
  /* Initialize band A search patterns */
  tdsrrccsp_band_a_pattern.band_shallow = FALSE;
  tdsrrccsp_band_a_pattern.band_deep    = FALSE;


  if(TDSCHECK_OPTIONAL_FEATURE_A_SHALOW_SET)
  {
    tdsrrccsp_band_a_pattern.band_shallow = TRUE;
    set_flag = TRUE;
  }
  if(TDSCHECK_OPTIONAL_FEATURE_A_DEEP_SET)
  {
    tdsrrccsp_band_a_pattern.band_deep = TRUE;
    set_flag = TRUE;
  }
  /*For debug: show NV setting of band A search pattern*/
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Band A search pattern: shallow %d and deep %d",tdsrrccsp_band_a_pattern.band_shallow,tdsrrccsp_band_a_pattern.band_deep);

  if(!set_flag)
  {
    /* Use default band A search patterns as nothing is specified in NV */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Use Band A default search pattern: shallow+deep");
    tdsrrccsp_band_a_pattern.band_shallow = TRUE;
    tdsrrccsp_band_a_pattern.band_deep = TRUE;
  }

  return;
}

/*===========================================================================

FUNCTION          tdsrrccsp_set_band_e_search_pattern

DESCRIPTION       This function is used to set band E search pattern according
                  to NV setting. And if it's not specified in NV, then use
                  default search pattern(shallow) instead.

DEPENDENCIES      It's to invoke one time after read NV66024.

RETURN VALUE      NONE
                  
SIDE EFFECTS      NONE

===========================================================================*/
void tdsrrccsp_set_band_e_search_pattern(void)
{
  boolean set_flag = FALSE;
  
  /* Initialize band E search patterns */
  tdsrrccsp_band_e_pattern.band_shallow = FALSE;
  tdsrrccsp_band_e_pattern.band_deep    = FALSE;


  if(TDSCHECK_OPTIONAL_FEATURE_E_SHALOW_SET)
  {
    tdsrrccsp_band_e_pattern.band_shallow = TRUE;
    set_flag = TRUE;
  }
  if(TDSCHECK_OPTIONAL_FEATURE_E_DEEP_SET)
  {
    tdsrrccsp_band_e_pattern.band_deep = TRUE;
    set_flag = TRUE;
  }
  /*For debug: show NV setting of band E search pattern*/
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Band E search pattern: shallow %d and deep %d",tdsrrccsp_band_e_pattern.band_shallow,tdsrrccsp_band_e_pattern.band_deep);

  if(!set_flag)
  {
    /* Use default band E search patterns as nothing is specified in NV */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Use Band E default search pattern: shallow");
    tdsrrccsp_band_e_pattern.band_shallow = TRUE;
  }

  return;
}

/*===========================================================================

FUNCTION          RRC_CSP_LFS_UPDATE_LIST_DB_WITH_TARGET_TDD_LIST

DESCRIPTION       This function populates LIST DB with target TDD list given 
                  by GSM-RR and also returns pointer to the first entry in LIST 
                  DB. If LIST DB is empty then NULL is returned.

DEPENDENCIES      This function is called only for G2TD redirection search and 
                  also only after initiating target TDD cell list from GSM.

RETURN VALUE      Pointer to first entry of LIST DB.
                  Otherwise, NULL if LIST DB is empty
                  
SIDE EFFECTS      NONE

===========================================================================*/
tdsrrc_csp_acq_entry_type *tdsrrccsp_lfs_update_list_db_with_target_tdd_list(void)
{
  int i;
  int j;
  boolean freq_found_in_list_db = FALSE;

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"G2TD Redirection: Number of Target TDD frequencies: %d",tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies);
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS: Updating List DB with Target freqs");


  tdsrrc_csp_int_data.num_freq_in_list_db =0;
  tdsrrc_csp_int_data.curr_rd_idx =0;
  tdsrrc_csp_curr_select_data.num_available_freq = 0;
  for(i = 0; i< tdsrrc_csp_int_data.target_tdd_cell_list.num_of_frequencies; i++)
  {
    freq_found_in_list_db = FALSE;
    // for KW
    for(j = 0; (j< tdsrrc_csp_int_data.num_freq_in_list_db)&&(j<MAX_UTRAN_TDS_FREQUNECIES); j++)
    {
      if(tdslist_db[j].freq == tdsrrc_csp_int_data.target_tdd_cell_list.fdd_cell_list[i].uarfcn)
      {
        // Freq already present in List DB
        freq_found_in_list_db = TRUE;
        break;
      }
    }

    if(!freq_found_in_list_db)
    {
      // KW error
      if (tdsrrc_csp_int_data.num_freq_in_list_db < MAX_UTRAN_TDS_FREQUNECIES)
      {
        tdslist_db[tdsrrc_csp_int_data.num_freq_in_list_db].freq =
         tdsrrc_csp_int_data.target_tdd_cell_list.fdd_cell_list[i].uarfcn;
        tdsrrc_csp_int_data.num_freq_in_list_db++;
      }
      else
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"For KW error");
        break;
      }
    }

    if(tdsrrc_csp_int_data.num_freq_in_list_db == MAX_UTRAN_TDS_FREQUNECIES)
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Reached the max limit for MAX_UTRAN_TDS_FREQUNECIES entries %d",MAX_UTRAN_TDS_FREQUNECIES);
      break;
    }
  }
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS: Added %d entries to List DB",tdsrrc_csp_int_data.num_freq_in_list_db);
  return &tdslist_db[0];
}



/*===========================================================================

FUNCTION           RRCCSP_ADD_ACQ_DB_AND_NW_DB_ENTRIES

DESCRIPTION        Adds an entry into the NW sharing DB at the given index Note that one-one mapping
                   is maintained between ACQ DB and NW sharing DB.

DEPENDENCIES       None

RETURN VALUE       Success/Failure. Failure will be an extra-ordinary event and
                   could be due to irreversible causes like corruption of pointers.

SIDE EFFECTS       None
===========================================================================*/
uecomdef_status_e_type tdsrrccsp_add_acq_db_and_nw_db_entries(
                                                           tdsrrc_csp_acq_entry_type* acq_entry_ptr,
                                                           tdsrrc_csp_nw_sharing_entry_type* nw_sharing_entry_ptr)
{
  uint8 index = TDSMAX_ACQ_DB_INDEX;
  uecomdef_status_e_type status;
  rrc_plmn_identity_type chosen_plmn = acq_entry_ptr->plmn_id;

  if(acq_entry_ptr->is_valid_nw_sharing)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Current cell is shared,Adding shared plmns");
    acq_entry_ptr->plmn_id = nw_sharing_entry_ptr->common_plmn_id;
    acq_entry_ptr->shared_list_of_plmns = *nw_sharing_entry_ptr;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: Current cell is not shared,not adding shared plmns");
  }

  if(tdsrrccsp_add_acq_entry(acq_entry_ptr) == SUCCESS)
  {
    status = SUCCESS;
    acq_entry_ptr->plmn_id = chosen_plmn;
  }
  else
  {
    status = FAILURE;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"NWS: Index in ACQ DB: %d,status: %d",index,status);
  }

  return (status);
}

/*===========================================================================

FUNCTION          RRCCSP_NW_SHARING_PRINT_ENTRY

DESCRIPTION       Prints a given network sharing entry

DEPENDENCIES      NONE

RETURN VALUE      None
                  
SIDE EFFECTS      NONE

===========================================================================*/
void tdsrrccsp_nw_sharing_print_entry(tdsrrc_csp_nw_sharing_entry_type* nw_sharing_entry_ptr)
{
  int i;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"NWS: Common PLMN MCC:MNC - %d:%d",TDSRRC_CSP_GET_INT_MCC(nw_sharing_entry_ptr->common_plmn_id),TDSRRC_CSP_GET_INT_MNC(nw_sharing_entry_ptr->common_plmn_id));
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"NWS: Number of Shared PLMNs: %d",nw_sharing_entry_ptr->num_of_shared_plmns);

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: List of Shared PLMNs");
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"******************************************************************");
  for(i =0 ; i<nw_sharing_entry_ptr->num_of_shared_plmns; i++)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"MCC:MNC - %d:%d",TDSRRC_CSP_GET_INT_MCC(nw_sharing_entry_ptr->shared_plmns_access_info[i].plmn_id),TDSRRC_CSP_GET_INT_MNC(nw_sharing_entry_ptr->shared_plmns_access_info[i].plmn_id));
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"******************************************************************");

}


/*===========================================================================

FUNCTION          RRCCSP_NAS_PLMN_EQUIVALENT_TO_ANY_OF_SHARED_BCCH_PLMNS

DESCRIPTION       Finds out whether the NAS requested PLMN and any the of the shared bcch PLMNs are equivalent

DEPENDENCIES      NONE

RETURN VALUE      TRUE/FALSE
                  
SIDE EFFECTS      NONE

===========================================================================*/
boolean tdsrrccsp_nas_plmn_equivalent_to_any_of_shared_bcch_plmns(
                                                               rrc_plmn_identity_type nas_plmn, 
                                                               uint8 index_in_acq_or_shared_db)
{
  rrc_plmn_identity_type bcch_plmn;
  tdsrrc_csp_acq_entry_type acq_entry = tdsrrccsp_acq_list[index_in_acq_or_shared_db];
  int i;

  if(acq_entry.is_valid_nw_sharing)
  {
    for(i=0; i<acq_entry.shared_list_of_plmns.num_of_shared_plmns; i++)
    {
      bcch_plmn = acq_entry.shared_list_of_plmns.shared_plmns_access_info[i].plmn_id;
      if(tdsrrccsp_check_for_eplmn(
          mmumts_convert_rrc_plmn_id_to_nas_plmn_id(bcch_plmn)) == TRUE)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"NWS: PLMN %d-%d is in the eq PLMN list",TDSRRC_CSP_GET_INT_MCC(bcch_plmn),TDSRRC_CSP_GET_INT_MNC(bcch_plmn));
        return TRUE;
      }
    }
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: No BCCH PLMN is in the NAS equivalent PLMN list");
  return FALSE;
}

/*===========================================================================

FUNCTION          RRCCSP_NAS_PLMN_MATCH_ANY_OF_BCCH_PLMNS

DESCRIPTION       Finds out whether the NAS requested PLMN and any the of the shared bcch PLMNs are same.

DEPENDENCIES      NONE

RETURN VALUE      TRUE/FALSE
                  
SIDE EFFECTS      NONE

===========================================================================*/
boolean tdsrrccsp_nas_plmn_match_any_of_bcch_plmns(
                                                rrc_plmn_identity_type nas_plmn, 
                                                uint8 index_in_acq_or_shared_db)
{
  rrc_plmn_identity_type bcch_plmn;
  tdsrrc_csp_acq_entry_type acq_entry = tdsrrccsp_acq_list[index_in_acq_or_shared_db];
  int i;

  if(acq_entry.is_valid_nw_sharing)
  {
    for(i=0; i<acq_entry.shared_list_of_plmns.num_of_shared_plmns; i++)
    {
      bcch_plmn = acq_entry.shared_list_of_plmns.shared_plmns_access_info[i].plmn_id;
      if(tdsrrc_bcch_and_nas_plmns_match(bcch_plmn, nas_plmn))
      {
        return TRUE;
      }
    }
  }
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"NWS: No BCCH PLMN match the NAS requested PLMN");
  return FALSE;
}

/*===========================================================================

FUNCTION          RRCCSP_IS_ANY_OF_BCCH_PLMNS_IN_BPLMN_LIST

DESCRIPTION       Finds if any one of the shared PLMNs are in the list of NAS requested PLMNs. Note that this is 
                  used for early abort of SIB reading. If none of the shared bcch PLMNs are in the NAS requested 
                  PLMN list then SIB reading is avoided immediately after reading the MIB.

DEPENDENCIES      NONE

RETURN VALUE      TRUE/FALSE
                  
SIDE EFFECTS      NONE

===========================================================================*/
boolean tdsrrccsp_is_any_of_bcch_plmns_in_bplmn_list(tdsrrc_csp_nw_sharing_entry_type bplmn_nw_db_entry)
{
uint32  i;
uint32  j;
  rrc_plmn_identity_type      plmn_id;

  if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_G2TD_BPLMN_AUTOMATIC_SCAN)
  {
    for(i = 0; i < tdsrrc_csp_int_data.bplmn_num_req; ++i)
    {
      for(j=0; j<bplmn_nw_db_entry.num_of_shared_plmns && i < TDSMAX_SHARED_PLMNS; j++)
      {
        plmn_id = bplmn_nw_db_entry.shared_plmns_access_info[j].plmn_id;
        if(tdsrrc_bcch_and_nas_plmns_match(plmn_id, tdsrrc_csp_int_data.bplmn_req_list[i].plmn_id))
        {
          return TRUE;
        }
      }
    }   
  }
  else if(tdsrrc_csp_int_data.curr_scan == RRC_CSP_TD2TD_BPLMN_AUTOMATIC_SCAN)
  {
    for(i = 0; i < tdsrrc_csp_int_data.wtow_bplmn_num_req; ++i)
    {
      for(j=0; j<bplmn_nw_db_entry.num_of_shared_plmns && i < TDSMAX_SHARED_PLMNS; j++)
      {
        plmn_id = bplmn_nw_db_entry.shared_plmns_access_info[j].plmn_id;
        if(tdsrrc_bcch_and_nas_plmns_match(plmn_id, tdsrrc_csp_int_data.wtow_bplmn_req_list[i].plmn_id))
        {
          return TRUE;
        }
      }
    }   
  }
  return FALSE;
}


/*===========================================================================

FUNCTION           RRCCSP_ADD_FREQ_BANDS_FROM_ACQ_DB_TO_BAND_LIST

DESCRIPTION        Extracts frequency bands from ACQ DB in LIFO order and adds them
                  to the band_list provided starting from num_of_bands_already_present.

DEPENDENCIES       num_of_bands_already_present should be initialized properly before calling
                   this function.

RETURN VALUE       number of bands present in the band list at the end.

SIDE EFFECTS       None
===========================================================================*/
uint8 
tdsrrccsp_add_freq_bands_from_acq_db_to_band_list
(
  tdsrrc_csp_band_class_type*    band_list,
  uint8 num_of_bands_already_present
)
{
  int    i;
  int    num_valid_entries;

  /* Do a Sanity check - check read and write index. A serious error if one
   * of them is out of bounds 
   */
  if ((tdsrrccsp_stored_acq_db.curr_wr_index > TDSMAX_ACQ_DB_INDEX) || 
      (tdsrrccsp_stored_acq_db.curr_rd_index > TDSMAX_ACQ_DB_INDEX) ||
      (tdsrrccsp_stored_acq_db.curr_rd_index < -1) ||
      (tdsrrccsp_stored_acq_db.curr_rd_index > tdsrrccsp_stored_acq_db.curr_wr_index))
  {
    TDSRRC_MSG2(MSG_LEGACY_ERROR,"ERROR: Acq db invalid Wr index %d Rd index %d",tdsrrccsp_stored_acq_db.curr_wr_index,tdsrrccsp_stored_acq_db.curr_rd_index);
    return num_of_bands_already_present;
  }

  /* Compute Number of Valid entries */
  if ((tdsrrccsp_stored_acq_db.curr_wr_index == TDSMAX_ACQ_DB_INDEX) && 
      (tdsrrccsp_stored_acq_db.last_entry_invalid == FALSE))
  {
    /* If all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is the last one and is Valid.
     */
    num_valid_entries = tdsrrccsp_stored_acq_db.curr_wr_index + 1;
  }
  else 
  {
    /* If not all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is Invalid and still needs to be initialized.
     */
    num_valid_entries = tdsrrccsp_stored_acq_db.curr_wr_index;
  }

  TDSRRC_MSG0(MSG_LEGACY_HIGH,"WBP: Adding frequency bands from ACQ DB");

  for (i = 0; ((i < num_valid_entries) && (num_of_bands_already_present < MAX_NUM_OF_TDS_BANDS)); i++)
  {
    if((tdsrrccsp_is_supported_band(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class) &&
        !tdsrrccsp_is_band_already_added_to_prioritized_list(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class,
                                                          num_of_bands_already_present)))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"WBP: Adding ACQ DB band %d to prioritized band list",tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class);
      band_list[num_of_bands_already_present++] = tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class;
    }
  }

  return num_of_bands_already_present;
}


/*===========================================================================

FUNCTION          RRC_CSP_LIST_DB_EXHAUSTED

DESCRIPTION    This function resets lis db after scanning thro' all the entries in it.
DEPENDENCIES
                  None

RETURN VALUE
SIDE EFFECTS
                  None.

===========================================================================*/
static void tdsrrccsp_list_db_exhausted(void)
{
    /* Invalidate the number of frequencies in LFS table*/
    tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = 0;
    
    tdsrrc_csp_curr_select_data.acq_db_in_use=FALSE;
    
    /* Invalidate the indices scanned_index and curr_rd_index*/
     tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index = -1;

    /* Freq Scan Initialization */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: Exhausted list scan db. Initialization done for RAW/FINE frequency scan ");
    tdsrrccsp_init_freq_scan_data();
    return;
}

/*===========================================================================

FUNCTION          RRC_CSP_GET_LIST_DB_MATCH_ACQ_ENTRY

DESCRIPTION    This function will match the frequency and PLMN id of the entry received from L1 as part of the 
                         frequency scan confirmation against the ACQ DB entries. If the match is successful  pointer to the acq list
                         entry will be returned to the called function otherwise NULL will be returned. GSM scan will be initiated if the 
                         List DB exhausts and the criteria for GSM Scan satisifies..

DEPENDENCIES
                  None

RETURN VALUE
                  Pointer to acquisition entry. A NULL pointer means no 
                  acquisition entry was found that fulfilled selection criteria.

SIDE EFFECTS
                  None.

===========================================================================*/
boolean  
tdsrrccsp_get_list_db_match_acq_entry()
{
  uint16 freq;
  int16 curr_freq_index;
  /*rrc_plmn_identity_type plmn_id;*/
  uint8 temp_index = 0;
  /*Initialize both the scanned and curr_rd indices before finding a matching
     ACQ entry from the list db
  */
  /* Apply to both G2T and L2T. They don't use LFS data struct of tdsrrc_csp_curr_select_data*/
  if((tdsrrc_csp_int_data.curr_scan == RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
     && ((!tdsrrc_csp_int_data.g2t_redi_freq_compl 
#if defined (FEATURE_TDSCDMA_TO_LTE) || defined (FEATURE_LTE_TO_TDSCDMA)
          && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_GSM)
#endif
         )
#ifdef FEATURE_LTE_TO_TDSCDMA 
         || (!tdsrrc_csp_int_data.l2t_redi_freq_compl && (tdsrrc_csp_int_data.interrat_type == TDSRRC_CSP_INTERRAT_LTE))
#endif
        )
   )
  {
#if 0 /* Keep W LFS logic for reference */
    if((tdsrrc_csp_int_data.num_freq_in_list_db >0) &&
     (tdsrrc_csp_int_data.curr_rd_idx < tdsrrc_csp_int_data.num_freq_in_list_db))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"LFS: starting acq on freq %d,curr_rd_ix %d",tdslist_db[tdsrrc_csp_int_data.curr_rd_idx].freq,tdsrrc_csp_int_data.curr_rd_idx);
      return(&tdslist_db[tdsrrc_csp_int_data.curr_rd_idx++]);
    }
    else
    {
      return NULL;
    }
#endif    
    /* TDS LFE index can jump */
    if((tdsrrc_csp_int_data.num_freq_in_list_db >0))
    {
       tdsrrc_csp_int_data.curr_rd_idx = tdsrrc_csp_int_data.acq_freq_list.curr_freq_index;
       
       if (tdsrrc_csp_int_data.acq_freq_list.curr_freq_index < (tdsrrc_csp_int_data.num_freq_in_list_db - 1))
       {      
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"LFS: starting acq on freq %d,curr_rd_ix %d",tdslist_db[tdsrrc_csp_int_data.curr_rd_idx].freq,tdsrrc_csp_int_data.curr_rd_idx);
         tdsrrc_csp_int_data.curr_acq_entry = tdslist_db[tdsrrc_csp_int_data.curr_rd_idx];
         tdsrrc_csp_curr_select_data.current_band = tdslist_db[tdsrrc_csp_int_data.curr_rd_idx].band_class;
         return TRUE;
       }
    }

    return FALSE;
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"LFS_COOS: curr_freq_index %d of num_freq %d in total",tdsrrc_csp_int_data.acq_freq_list.curr_freq_index,tdsrrc_csp_int_data.acq_freq_list.num_freq);

    if (tdsrrc_csp_int_data.acq_freq_list.curr_freq_index < tdsrrc_csp_int_data.acq_freq_list.num_freq - 1)
    {
      /* ZZTBD: to be investigated. TDD list scan is acquisition and is one shot. 
       * Right now it needs to be kept to allow continuation of acquisiton to the 
       * next frequency although the data below is not really used in TDD */
      if (tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index < tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq)
      {
        tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index++;
        curr_freq_index = tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index; 
        /* plmn_id = tdsrrc_csp_curr_select_data.scan_info.list_scan.plmn_id[curr_freq_index];*/
        freq = tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[curr_freq_index].ctr_freq;
        /* Get the matching frequency from the current read index */
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"LFS_COOS: curr_freq_index = [%d]--->freq=[%d]",curr_freq_index,freq);
        /* increment the scanned index and current read index will point to the next frequency scan entry */
        tdsrrc_csp_int_data.curr_acq_entry.freq = freq;    
        tdsrrc_csp_curr_select_data.current_band = tdsrrc_get_frequency_band(freq);
        return TRUE;     
#if 0   
        /*Get the matching acquisition entry from the Acq DB*/
        for(i=0;i<MAX_ACQ_DB_ENTRIES;i++)
        {
          if( (freq  == tdsrrccsp_acq_list[i].freq) &&
               (TDSRRC_CSP_SAME_MCC(plmn_id, tdsrrccsp_acq_list[i].plmn_id)) &&
               (TDSRRC_CSP_SAME_MNC(plmn_id, tdsrrccsp_acq_list[i].plmn_id))
            )
          {
            /* update the tdsrrccsp_acq_db_freq_scanned_list with the matching freq 
                 from the acq entry
            */
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: Matching acq entry. update the freq=%d into the scanned list",freq);
            tdsrrccsp_update_acq_db_freq_scanned_list(freq);
            TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: new freq idx=%d ",tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index);
            /* This is the acquisition entry with which CSP will try for acquisition */
            return &tdsrrccsp_acq_list[i];  
          }
        }
#endif
      }
    }
    else if ((tdsrrc_csp_int_data.is_mset_compl_for_rlf_oos == FALSE)
           && (
#ifdef FEATURE_WRLF_SYSTEM_SEL
               ((tdsrrc_csp_int_data.curr_scan==TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) && (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()))
		       ||
#endif
		       (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
               ||(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN)
               ||(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN)
              )
            )
    {
         TDSRRC_MSG1(MSG_LEGACY_HIGH,"MSET are all tried during curr_scan %d,start to try primiary frequency",tdsrrc_csp_int_data.curr_scan);
         
         tdsrrc_csp_int_data.is_mset_compl_for_rlf_oos = TRUE;
         /* Append primiary freq to scan_info.list_scan, then move them to the list beginning*/
         tdsrrccsp_add_prim_freq_into_list_db();
         while ((tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index < tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq - 1) &&
                (tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index < (TDSL1_MAX_SCAN_FREQ - 1)))
         {
            tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[temp_index].ctr_freq = 
               tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index + 1].ctr_freq;
            tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[temp_index].result_plmn_type = 
               tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index + 1].result_plmn_type;
		    tdsrrc_csp_curr_select_data.scan_info.list_scan.source[temp_index] = 
               tdsrrc_csp_curr_select_data.scan_info.list_scan.source[tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index + 1];			   
            temp_index++;
            tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index++;
         }
         /* temp_index != 0 means valid primiary frequencies are append to list. or else, means all primiary
          * frequencies have been tried from MSET before
          */
         if (temp_index != 0)
         {
            tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = temp_index;
            tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index = -1;
            tdsrrc_csp_int_data.acq_freq_list.curr_freq_index = -1;
            tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS;
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Primiary frequency add to list are %d,acq_mode = %d",tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq,tdsrrc_csp_int_data.acq_mode);              
            return TRUE;
         }
         TDSRRC_MSG2(MSG_LEGACY_HIGH,"Primiary frequency add to list are %d,acq_mode = %d",tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq,tdsrrc_csp_int_data.acq_mode);                       
    }
    /*If it comes here, then all list db scan is over*/
    tdsrrccsp_list_db_exhausted();
  }

  return FALSE;
}

/*===========================================================================

FUNCTION          TDSRRCCSP_ADD_PRIM_FREQ_INTO_LIST_DB

DESCRIPTION       This function is used by CSP to run through the primary freq list read from NV and if a suitable entry
                            is found  then the frequency will be stored in the current CSP database

DEPENDENCIES      NONE

RETURN VALUE
                  
SIDE EFFECTS

===========================================================================*/
void tdsrrccsp_add_prim_freq_into_list_db(void)
{
  int i =0;
  int iterator = 0;
  boolean is_freq_add_into_lfs_list = TRUE;
  tdsrrc_csp_band_class_type prim_freq_band;
  
  for (i=0; i<tdsrrc_csp_int_data.num_tds_prim_freq && tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq<TDSL1_MAX_SCAN_FREQ; i++)
  {
    prim_freq_band = tdsrrc_get_frequency_band(tdsrrc_csp_int_data.tds_prim_freq_list[i]);
	if ( /* (!tdsrrccsp_should_freq_be_skipped(tdsrrc_csp_int_data.tds_prim_freq_list[i], TDSRRCCSP_FINE_SCAN_SKIP_RANGE)) && */
		 tdsrrccsp_is_supported_band(prim_freq_band) )
	{
	  /* This check is added to avoid the duplicate frequencies in to the lfs data*/
      for (iterator =0;iterator<tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq;iterator++)
      {
        if (tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[iterator].ctr_freq ==
	  	    tdsrrc_csp_int_data.tds_prim_freq_list[i])
        {
          is_freq_add_into_lfs_list = FALSE;
          break;
        }
      }
	  if (is_freq_add_into_lfs_list)
	  {
	    tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].ctr_freq=
		    tdsrrc_csp_int_data.tds_prim_freq_list[i];
        tdsrrc_csp_curr_select_data.scan_info.list_scan.source[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq]=
                                          PFL;
        tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].result_plmn_type = (uint8) TDSRRC_CSP_ANY_PLMN;

	    TDSRRC_MSG3(MSG_LEGACY_MED,"LFS_COOS: Added a prim freq in lfs table, freq=%d, plmn type = %d, num_freq=%d ",
                 tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].ctr_freq,
                 tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].result_plmn_type,
                 tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq + 1
                  );
	  


	    /* Increment the number of frequencies */
	    tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq++;
	  }
	  else
      {
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: dup frequency =%d in prim freq list is skipped",tdsrrc_csp_int_data.tds_prim_freq_list[i]);
	    is_freq_add_into_lfs_list = TRUE;
      }
	}
	else
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: invalid frequency =%d in prim freq list is skipped",tdsrrc_csp_int_data.tds_prim_freq_list[i]);
    }
  }

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: num_freq %d after primary frequency is added",tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq);
}

/*===========================================================================

FUNCTION          RRCCSP_GET_MATCH_ACQ_ENTRIES_UPDATE_LIST_DB

DESCRIPTION       This function is used by CSP to run through the acq DB for specific/any PLMN based on the current
                           acquisition type and if a suitable entry is found  then the frequency, PLMN Id will be stored in the 
                           current CSP database

DEPENDENCIES      NONE

RETURN VALUE
                  
SIDE EFFECTS

===========================================================================*/
void tdsrrccsp_update_match_acq_entries_into_list_db(void)
{
  rrc_plmn_identity_type req_plmn_id;
  int num_valid_entries;
  int i=0;
  int iterator = 0;
  boolean is_freq_add_into_lfs_list = TRUE;
  boolean is_eplmn = FALSE;
  tdsrrc_csp_acq_entry_type  *ret_acq_entry_ptr = NULL;
  boolean is_network_shared = FALSE;
  boolean is_acq_db_plmn_matched = FALSE;
  boolean scan_all_acq_db = FALSE;
  tdsl1_mset_list_type tdsl1_mset_list;

  /*Invalidate the curr and scanned indices */
  tdsrrc_csp_curr_select_data.scan_info.list_scan.curr_freq_index = -1;

  /* Initialize the number of frequencies in the list scan table */
  tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq = 0;

  tdsrrccsp_acq_db_freq_scanned_list.wr_index = 0;
  req_plmn_id = tdsrrc_csp_int_data.curr_req_plmn.plmn_id;
  
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: LIST DB UPDATE ");
 
  if ((tdsl1_get_mset_list(&tdsl1_mset_list) == TRUE) && (tdsrrccsp_ignore_MSET == FALSE))
  {
    tdsrrc_csp_int_data.acq_db_empty = TRUE;

    ASSERT(tdsl1_mset_list.num_freq <= TDSL1_MAX_MSET_LIST_NUM);

    num_valid_entries = tdsl1_mset_list.num_freq;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS use MSET list: no of valid entries =%d",num_valid_entries);


  /* Loop through the MSET list and store the matching entries into List DB */
    for (i = 0; i < num_valid_entries; i++)
    {
      /* haizhoul 2013/1/5 Do not do frequency pruning when get frequency list from MSET*/
      if(
      #if 0
	  !tdsrrccsp_should_freq_be_skipped(
          tdsl1_mset_list.freq[i], TDSRRCCSP_FINE_SCAN_SKIP_RANGE) &&
	  #endif
	  (tdsrrccsp_is_supported_band(tdsrrc_get_frequency_band(tdsl1_mset_list.freq[i]))))
      {
      /* This check is added to avoid the duplicate frequencies in to the lfs data*/
        for (iterator =0;iterator<tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq;iterator++)
        {
          if (tdsl1_mset_list.freq[i] == 
              tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[iterator].ctr_freq
             )
          {
            is_freq_add_into_lfs_list = FALSE;
			break;
          }
        }
        if(is_freq_add_into_lfs_list)
        {
        /* A suitable entry is found. Set the Read index to this db pointer entry and 
         * return pointer to the acquisition list contained in this db pointer entry
         */
        /* Store the frequency*/
          tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].ctr_freq =
                                          tdsl1_mset_list.freq[i];
          tdsrrc_csp_curr_select_data.scan_info.list_scan.source[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq]=
                                          MSET;
          if(tdsrrc_csp_curr_select_data.acq_type == TDSRRC_CSP_SPEC_PLMN)
          {
            tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].result_plmn_type = (uint8) TDSRRC_CSP_SPEC_PLMN;
          }
          else
          {
            tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].result_plmn_type = (uint8) TDSRRC_CSP_ANY_PLMN;
          }
          tdsrrc_csp_curr_select_data.scan_info.list_scan.plmn_id[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq] = tdsrrc_csp_int_data.curr_acq_entry.plmn_id;			
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"LFS_COOS: Added an entry in lfs table, freq=%d, plmn type = %d, num_freq=%d ",
               tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].ctr_freq,
               tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].result_plmn_type,
               tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq + 1
                );


          /* Increment the number of frequencies */
          tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq++;
         }
         else
         {
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: frequency =%d can be skipped",tdsl1_mset_list.freq[i]);
           is_freq_add_into_lfs_list = TRUE;
         }
      }
    }


    if (tdsrrc_csp_int_data.curr_scan==RRC_CSP_GSM_TO_TDS_REDIRECTION_SCAN)
    {
      /* use LFS ACQ mode for OOS, RLF and X2T redirection with valid MSET freq, e.g, cell barred */
	  tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS;
    
	  /* add primary freqs into list db if needed */
      if (tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq < TDSL1_MAX_SCAN_FREQ)
      {
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: Adding prim freq into list db during OOS/X2T redir");
	    tdsrrccsp_add_prim_freq_into_list_db();
      }
	  else
	  {
	    TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: Reach max num freq in list db,skip prim freq list during OOS/RLF/X2T redir");
	  }
	}
	else
	{
	  /* For RLF/OOS prceodure, use MSET first and acq_mode set as TDSL1_ACQ_MODE_SHALLOW*/
      if (
#ifdef FEATURE_WRLF_SYSTEM_SEL
          ((tdsrrc_csp_int_data.curr_scan==TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) && (TDSRRC_IS_CONN_MODE_OOS_SRCH_IN_PROGRESS()))
	      ||
#endif
          /* RLF detected, CU may not initiated. But L1 already lost timing, will be in IDLE not SYNC state */
          ((tdsrrc_csp_int_data.curr_scan==TDSRRC_CSP_LEAVING_CONN_MODE_SCAN) && tdsrrc_csp_int_data.rlf_detected)
	      ||(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
          ||(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_IDLE_STATE_SCAN)
          ||(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_IN_CONNECTING_STATE_SCAN))
      {
	    /*Append PFL if MSET exists but not valid, like not in supported band*/
        if(tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq == 0)
        {
          tdsrrc_csp_int_data.is_mset_compl_for_rlf_oos = TRUE;
	      tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: Adding prim freq into list db during RLF/OOS due to no valid MSET");
	      tdsrrccsp_add_prim_freq_into_list_db();
        }
        else
        {
          /* Try MSET first for RLF/OOS. Set acq_mode to TDSL1_ACQ_MODE_SHALLOW.  If MSET fail, then try PFL with TDSL1_ACQ_MODE_LFS*/
          tdsrrc_csp_int_data.is_mset_compl_for_rlf_oos = FALSE;
      	  tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_SHALLOW;
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Using MSET for RLF/OOS,set acq_mode = %d",tdsrrc_csp_int_data.acq_mode);  
        }    
      }
      else
      {
          /* use NORMAL ACQ mode for normal connection release moving to DISCONNECTED */
	      tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_NORMAL;
      }
	  TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: Not adding prim freq into list db");
	}

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: number of freqs added into lfs data from MSET and prim freq list = %d",tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq);

    if(tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq == 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: No freq is found in the MSET search and prim freq list for list scan");
      tdsrrc_csp_curr_select_data.current_band = 
              tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
      tdsrrc_csp_curr_select_data.current_sub_band = 0;
    }
  }
  else
  {
    tdsrrc_csp_int_data.acq_db_empty = TRUE;


  /* Do a Sanity check - check read and write index. A serious error if one
    * of them is out of bounds  Also read index should never be higher than 
    * write index.
  */
    ASSERT(tdsrrccsp_stored_acq_db.curr_wr_index <= TDSMAX_ACQ_DB_INDEX);
    ASSERT(tdsrrccsp_stored_acq_db.curr_rd_index <= TDSMAX_ACQ_DB_INDEX);
    ASSERT(tdsrrccsp_stored_acq_db.curr_rd_index >= -1);
    ASSERT(tdsrrccsp_stored_acq_db.curr_rd_index <= tdsrrccsp_stored_acq_db.curr_wr_index);

  /* Compute Number of Valid entries */
    if ((tdsrrccsp_stored_acq_db.curr_wr_index == TDSMAX_ACQ_DB_INDEX) && 
      (tdsrrccsp_stored_acq_db.last_entry_invalid == FALSE))
    {
    /* If all pointers in acq. dbase are filled up, the write index points
     * to a pointer entry which is the last one and is Valid.
     */
      num_valid_entries = tdsrrccsp_stored_acq_db.curr_wr_index + 1;
    }
    else 
    {
    /* If not all pointers in acq. dbase are filled up, the write index points
    * to a pointer entry which is Invalid and still needs to be initialized.
    */
      num_valid_entries = tdsrrccsp_stored_acq_db.curr_wr_index;
    }
  /* MM has requested to look for specific PLMN Cell Selection Procedure 
      needs this information while finding matching acq entry from the ACQ
      DB */

    TDSRRC_MSG2(MSG_LEGACY_HIGH,"LFS_COOS use ACQ DB: no of valid entries =%d,curr_rd_index=%d",num_valid_entries,tdsrrccsp_stored_acq_db.curr_rd_index);

    if((tdsrrc_csp_curr_select_data.acq_type == TDSRRC_CSP_ANY_PLMN)
      || (tdsrrccsp_should_all_acq_db_be_scanned()))
    {
      scan_all_acq_db = TRUE;
    }


  /* Loop through the ACQ DB and store the matching entries into List DB */
    for (i = 0; i < num_valid_entries; i++)
    {
      is_network_shared = tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].is_valid_nw_sharing;

      if(tdsrrc_csp_curr_select_data.acq_type == TDSRRC_CSP_SPEC_PLMN)
      {
          /* Resetting the is_eplmn variable at start of loop */
          is_eplmn = FALSE;
#if 0
          /*Do not restrict try EQPLMN only for TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN due to it may lead to LFS list empty (eg. PLMNs in ACQ DB are all EQPLMN).  
		   *Cell selection can camp on equvalent PLMN, so try EQPLMN in any case */
          if(tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_OUT_OF_SERVICE_AREA_SCAN)
          {
#endif
            if((tdsrrccsp_check_for_eplmn(
                    mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tdsrrc_csp_int_data.curr_req_plmn.plmn_id))) == TRUE)
              {    
                TDSRRC_MSG0(MSG_LEGACY_MED,"Curr requested PLMN is in the NAS equivalent PLMN list");
                if((!is_network_shared) &&
                 (tdsrrccsp_check_for_eplmn(
                    mmumts_convert_rrc_plmn_id_to_nas_plmn_id(
                      tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id)) == TRUE))
                {
                  TDSRRC_MSG2(MSG_LEGACY_HIGH,"PLMN %d-%d is in the eq PLMN list",TDSRRC_CSP_GET_INT_MCC(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id),TDSRRC_CSP_GET_INT_MNC(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id));
                  is_eplmn = TRUE;
                }
                else if((is_network_shared) && 
                   ((tdsrrccsp_nas_plmn_equivalent_to_any_of_shared_bcch_plmns(
                                                                          tdsrrc_csp_int_data.curr_req_plmn.plmn_id,
                                                                          tdsrrccsp_stored_acq_db.acq_list_indices[i])) == TRUE))
                {
                  is_eplmn = TRUE;
                }
            }
#if 0
          }
#endif
  
            is_acq_db_plmn_matched = FALSE;
            if((!is_network_shared) &&
             (tdsrrc_bcch_and_nas_plmns_match(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].plmn_id, 
                                           req_plmn_id)))
            {
              is_acq_db_plmn_matched = TRUE;
            }
            else if((is_network_shared) &&
                    (tdsrrccsp_nas_plmn_match_any_of_bcch_plmns(
                                                       req_plmn_id,
                                                       tdsrrccsp_stored_acq_db.acq_list_indices[i])))
            {
              is_acq_db_plmn_matched = TRUE;
            }
  
      }
      /* haizhoul 2013/1/5 Do not do frequency pruning when get frequency list from MSET*/
      if((scan_all_acq_db || is_acq_db_plmn_matched
              || (is_eplmn)
        ) && (
		  #if 0
		  (!tdsrrccsp_should_freq_be_skipped(
          tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq, TDSRRCCSP_FINE_SCAN_SKIP_RANGE))
          && 
		  #endif
          (tdsrrccsp_is_supported_band(tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].band_class))
          && (!tdsrrccsp_has_acq_db_freq_been_scanned(
                    tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq))))
      {
      /* This check is added to avoid the duplicate frequencies in to the lfs data*/
        for (iterator =0;iterator<tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq;iterator++)
        {
          if (tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq == 
              tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[iterator].ctr_freq
             )
          {
            is_freq_add_into_lfs_list = FALSE;
			break;
          }
        }
        if(is_freq_add_into_lfs_list)
        {
        /* A suitable entry is found. Set the Read index to this db pointer entry and 
         * return pointer to the acquisition list contained in this db pointer entry
         */
          tdsrrccsp_stored_acq_db.curr_rd_index = (int8)i;
          ret_acq_entry_ptr = &tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]];

        /* Store the frequency*/
          tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].ctr_freq=
                                          ret_acq_entry_ptr->freq;
          tdsrrc_csp_curr_select_data.scan_info.list_scan.source[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq]=
                                          ACQ_DB;

          /* Store plmn type based on the current acq type*/
          if((is_acq_db_plmn_matched)
                      || is_eplmn
            )
          {
            tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].result_plmn_type = (uint8) TDSRRC_CSP_SPEC_PLMN;
          }
          else
          {
            tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].result_plmn_type = (uint8) TDSRRC_CSP_ANY_PLMN;
          }
		
          tdsrrc_csp_curr_select_data.scan_info.list_scan.plmn_id[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq] = ret_acq_entry_ptr->plmn_id;			
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"LFS_COOS: Added an entry in lfs table, freq=%d, plmn type = %d, num_freq=%d ",
               tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].ctr_freq,
               tdsrrc_csp_curr_select_data.scan_info.list_scan.freq_result[tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq].result_plmn_type,
               tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq + 1
                );
        

          /* Increment the number of frequencies */
          tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq++;
          if(is_acq_db_plmn_matched
              || (is_eplmn)
              || (tdsrrc_csp_curr_select_data.acq_type == TDSRRC_CSP_ANY_PLMN))
          {
            tdsrrc_csp_int_data.acq_db_empty = FALSE;
          }
         }
         else
         {
           TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: frequency =%d can be skipped",tdsrrccsp_acq_list[tdsrrccsp_stored_acq_db.acq_list_indices[i]].freq);
           is_freq_add_into_lfs_list = TRUE;
         }
      }
    }

    /* use LFS ACQ mode for initial power up case */
	tdsrrc_csp_int_data.acq_mode = TDSL1_ACQ_MODE_LFS;
	/* Do not add PFL if current scan is TDSRRC_CSP_CELL_SELECTION_SCAN and scan scope is SYS_SCAN_SCOPE_ACQ_DB*/
    if ((tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq < TDSL1_MAX_SCAN_FREQ)
#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
        &&(!((tdsrrc_csp_int_data.srvc_req_scan_scope == SYS_SCAN_SCOPE_ACQ_DB)
           && (tdsrrc_csp_int_data.curr_scan == TDSRRC_CSP_CELL_SELECTION_SCAN)))
#endif		   
       )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: Adding prim freq into list db");
	  tdsrrccsp_add_prim_freq_into_list_db();
    }
	else
	{
#ifdef FEATURE_TDSCDMA_SCAN_SCOPE
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"LFS_COOS: skip prim freq list, num of freq in list db is %d, scan scope is %d and cell selection type is %d",
	                      tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq,
						  tdsrrc_csp_int_data.srvc_req_scan_scope,
						  tdsrrc_csp_int_data.curr_scan);
#else
	  TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: Reach max num freq in list db,skip prim freq list");
#endif
	}
	
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"LFS_COOS: number of freqs added into lfs data = %d",tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq);

    if(ret_acq_entry_ptr == NULL && tdsrrc_csp_curr_select_data.scan_info.list_scan.num_freq == 0)
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"LFS_COOS: No freq is found in the ACQ DB search and prim freq list for list scan");
      tdsrrc_csp_curr_select_data.current_band = 
              tdsrrc_csp_curr_select_data.first_band_to_be_scanned;
      tdsrrc_csp_curr_select_data.current_sub_band = 0;
    }
  }
}


