/*===*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

     H D R    S R C H   S C H E D U L E R   G R O U P   F U N C T I O N S

                    Search Scheduler Group 

GENERAL DESCRIPTION

   This file contains the function used in the HDR scheuduler group management. 
   See more comments in hdrsrchschdgrp.h

EXTERNALIZED FUNCTIONS

   void hdrsrchschdgrp_init
   void hdrsrchschdgrp_tc_init()
   void hdrsrchschdgrp_tca_update
   void hdrsrchschdgrp_get_schdgrp_rpt
   void hdrsrchschdgrp_commit_to_dsp

REGIONAL FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Copyright (c) 2007 - 2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

============================================================================

                        EDIT HISTORY FOR MODULE

$Header: //components/rel/hdr.mpss/6.0/srch/src/hdrsrchschdgrp.c#1 $ $DateTime: 2019/06/19 22:12:26 $ $Author: pwbldsvc $

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ------------------------------------------------------- 
12/12/2016   svu     Fixed KW issues.
12/19/2013   sat     Changed memcpy() to assignment or memscpy() (safer version).
11/07/2011   vlc     Fixed Klocwork warnings. 
12/08/2009   ljl     Included amssassert.h. 
12/06/2009   rkc     Added include of amssassert.h.
09/30/2009   smd     Fixed KW errors.
05/09/2008   rkc     Fixed compiler warnings.
05/08/2008   ljl     Fixed the problem to update scheduler group id.
12/26/2007   jyw     Fixed compiling warnings.
12/17/2007   jyw     Made the valid scheudler group id start from 1..
12/03/2007   jyw     Enabled HDRASP_SET_SCHD_GID macro call.
06/01/2006   jyw     Created this file for HDR revB development.
==========================================================================*/


/* <EJECT> */
/*==========================================================================

                     INCLUDE FILES FOR MODULE

==========================================================================*/

#include "hdr_variation.h"
#include "hdrsrchschdgrp.h"
#include "hdrdebug.h"
#include "hdrmdspmcr.h"
#include "err.h"
#include "amssassert.h"

#ifdef FEATURE_HDR_REVB
/* <EJECT> */
/*===========================================================================

            DEFINITIONS AND DECLARATIONS FOR MODULE

This section contains local definitions for constants, macros, types,
variables and other items needed by this module.

===========================================================================*/

/*---------------------------------*
 * HDR SRCH SCHEDULER GROUP TYPE   *
 *---------------------------------*/

typedef struct
{
  uint8                      scheduler_tag;
    /* The scheduler tag assigned by the AN */

  boolean                    tag_updated;
    /* The field indicates if the scheduler group tag for the corresponding
       sect has been updated to the current TCA. the field is reset at the 
       beginning of new TCA processing. It is used to check the error of
       scheduler group split. */

  hdrsrch_pilot_gid_type     schdgrp_gid;
    /* The scheduler group id issued by the hdrsrch */

  uint8                      num_sects;
    /* The number of sectors in the current scheduler group. */

  hdrsrch_sect_struct_type*  sects_list[HDRSRCH_MAX_SUBASET_SIZE];
    /* TBD: the scheduler group size might as big as the entire ASET
       in revB 2nd phase. */
} hdrsrchschdgrp_grp_type;


/*---------------------------------------*
 * HDR SRCH SCHEDULER GROUP TABLE TYPE   *
 *---------------------------------------*/

typedef struct
{
  uint8                   num_grps;
    /* The number of the existing scheduler groups */

  hdrsrchschdgrp_grp_type table[HDRSRCH_MAX_NUM_SCHDGRPS];
    /* The table contains the current groups */
} hdrsrchschdgrp_schdgrptable_type;


/*---------------------------------*
 * HDR SRCH SCHEDULER ID POOL TYPE *
 *---------------------------------*/

/* schgrp id LRU (leaset recent use) table */

typedef struct 
{
  hdrsrch_schdgrp_gid_type lru_table[HDRSRCH_MAX_NUM_SCHD_GIDS];
    /* The ID pool */

  uint8                    lru_idx;
    /* The least recent use idx */

  uint8                    mru_idx;
    /* The most recent use idx */
} hdrsrchschdgrp_gidpool_type;


/*---------------------------------*
 *     Local Global Variables             *
 *---------------------------------*/

static hdrsrchschdgrp_schdgrptable_type hdrsrchschdgrp_schdgrp;
  /* Current scheudler groups */

static hdrsrchschdgrp_gidpool_type      hdrsrchschdgrp_gidpool;
  /* Scheduler group ID Least Recent Use table */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_GET_GID

DESCRIPTION 
  Get a new scheduler group id.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

hdrsrch_schdgrp_gid_type hdrsrchschdgrp_get_gid( void )
{
  hdrsrch_schdgrp_gid_type    lru_gid;
    /* New scheduler group id */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /* Get least recently used id from the ID pool */

  lru_gid = hdrsrchschdgrp_gidpool.lru_table[hdrsrchschdgrp_gidpool.lru_idx];

  /* Advance get index, and wrap at end of table */

  if ( ++hdrsrchschdgrp_gidpool.lru_idx == HDRSRCH_MAX_NUM_SCHD_GIDS )
  {
    hdrsrchschdgrp_gidpool.lru_idx = 0;
  }

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Issued LRU schdgrp gid: %d", lru_gid );

  return lru_gid;
} /* hdrsrchschdgrp_get_gid */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_PUT_GID

DESCRIPTION 
  Return a scheduler group id to the scheduler group ID pool.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_put_gid
( 
  hdrsrch_schdgrp_gid_type mru_gid 
    /* Most recent used scheduler group id */
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  HDR_MSG_SRCH_1( MSG_LEGACY_MED, "Return MRU schdgrp gid: %d d", 
                  mru_gid );

  hdrsrchschdgrp_gidpool.lru_table[ hdrsrchschdgrp_gidpool.mru_idx ] = mru_gid;
    /* Put most recent use gid back in LRU table at current mru index */

  /* Advance mru index, and wrap at end of table */
  if ( ++hdrsrchschdgrp_gidpool.mru_idx == HDRSRCH_MAX_NUM_SCHD_GIDS )
  {
    hdrsrchschdgrp_gidpool.mru_idx = 0;
  }
} /* hdrsrchschdgrp_put_gid */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_TC_INIT

DESCRIPTION 
  The function should be called to intialize the scheduler group table. 

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_tc_init( void )
{
  uint8 i, num_schdgrps;
    /* Loop index and number of scheduler groups */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  num_schdgrps = hdrsrchschdgrp_schdgrp.num_grps;

  for( i=0; i< num_schdgrps; i++)
  {
    /* Return any outstanding scheduler group gid to the pool */

      hdrsrchschdgrp_put_gid( hdrsrchschdgrp_schdgrp.table[i].schdgrp_gid );
  }

  memset( &hdrsrchschdgrp_schdgrp, 0, 
          sizeof( hdrsrchschdgrp_schdgrptable_type ) );
    /* Initialize the grp table */
} /* void hdrsrchschdgrp_init */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_UPDATE_SCHDTAGS

DESCRIPTION 
  The function update the scheduler tags of the currnet outstanding 
  scheduler group based on the new TCA.

DEPENDENCIES
  None

RETURN VALUE
  boolean to indicate the integrity of the schduler group in the TCA.
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchschdgrp_update_schdtags( void )
{
  uint8  num_schdgrps, num_sects;
    /* The number of sechuler groups and the number of sectors */

  int8   i, j;
    /* Loop index */

  hdrsrch_sect_struct_type*  sect_ptr;
    /* Convinience pointor */

  boolean  schdgrp_valid = TRUE;
    /* Return value initilized to TRUE */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  num_schdgrps = hdrsrchschdgrp_schdgrp.num_grps;
    /* The number of current scheduler groups */

  for( i=num_schdgrps-1; i>=0 ; i-- )
  {
    hdrsrchschdgrp_schdgrp.table[i].tag_updated = FALSE;
      /* Before processing the scheduler tag update, reset the flag */
  }

  for( i=num_schdgrps-1; i>=0 ; i-- )
  {
    num_sects = hdrsrchschdgrp_schdgrp.table[i].num_sects;
      /* The number of sectors in this group */

    for( j=num_sects-1; j>=0 ; j-- )
    {
      sect_ptr = hdrsrchschdgrp_schdgrp.table[i].sects_list[j];
        /* Convinience pointor */

      if( sect_ptr->set != HDR_ASET ) 
      {
        /* The sector has been removed from the ASET. Drop the entry by
           copying the last one in the list and reduce the group size */ 

        HDR_MSG_SRCH( MSG_LEGACY_ERROR, "Schdgrp has nonASET sector!" );

        hdrsrchschdgrp_schdgrp.table[i].sects_list[j] = 
                   hdrsrchschdgrp_schdgrp.table[i].sects_list[num_sects-1];

        hdrsrchschdgrp_schdgrp.table[i].num_sects--;
      }
      else
      {
        /* If the sect stays, update scheduler tag and the flag */

        if( hdrsrchschdgrp_schdgrp.table[i].tag_updated == FALSE )
        {
          hdrsrchschdgrp_schdgrp.table[i].scheduler_tag = 
            hdrsrchschdgrp_schdgrp.table[i].sects_list[j]->aset.scheduler_tag;

          hdrsrchschdgrp_schdgrp.table[i].tag_updated = TRUE;
            /* Mark the sector */
        }
        else 
        {
            /* Check if group split happens */

          if( hdrsrchschdgrp_schdgrp.table[i].scheduler_tag != 
                sect_ptr->aset.scheduler_tag )
          {
            HDR_MSG_SRCH( MSG_LEGACY_ERROR, 
                          "schdgrp split! system lost!");

            schdgrp_valid = FALSE;
          } 
        }  /* if( !tag_updated )*/
      } /* if( !aset )*/ 
    }  /* for j */
  } /* for i */

  return schdgrp_valid;
} /* boolean hdrsrchschdgrp_update_schdtags( void ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_INTEGRITY_CHECK

DESCRIPTION 

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchschdgrp_integrity_check( void )
{
  uint8    i, j;
    /* loop index */

  uint8    num_schdgrps;
    /* The number of outstanding scheduler groups */

  boolean  schdgrp_valid = TRUE;
    /* return value initialized to TRUE */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  num_schdgrps = hdrsrchschdgrp_schdgrp.num_grps;

  for( i = 0; i < num_schdgrps; i++ )
  {
    for( j = i + 1; j < num_schdgrps; j++ )
    {
      if ( hdrsrchschdgrp_schdgrp.table[i].scheduler_tag == 
             hdrsrchschdgrp_schdgrp.table[j].scheduler_tag )
      {
        HDR_MSG_SRCH( MSG_LEGACY_ERROR,
                      "schgrp combine happens, system lost!");
        schdgrp_valid = FALSE;
      }
    } /* for j*/
  } /* for i */

  return schdgrp_valid;
} /* hdrsrchschdgrp_integrity_check */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_ADD_SECTS

DESCRIPTION 
  The function adds new TCA sectors into the scheduler group table.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_add_sects( void )
{
  uint8                      i, j, k, l;
    /* Loop index */

  uint8                      num_sects;
    /* Max number of subasets */

  hdrsrch_sect_struct_type   *sect_ptr;
    /* Convinience pointor */

  hdrsrch_pilot_gid_type     schdgrp_gid;
    /* Scheduler group ID managed by hdrsrch */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( i = 0; i < hdrsrchset.subaset_cnt; i ++ )
  {
    for( j = 0; j < hdrsrchset.subaset[i].aset_cnt; j++ )
    {
      sect_ptr = hdrsrchset.subaset[i].aset[j];
        /* Convinience pointor */

      if ( sect_ptr->aset.aset_newbie == TRUE )
      {
        /* If an ASET newbie, then add into scheduler group table. For 
           persistent sectors, the scheduler tag should have been updated
           x in the hdrsrchschdgrp_update_schdtags() */

        for( k = 0; k < hdrsrchschdgrp_schdgrp.num_grps; k ++ )
        {
          /* Search the scheduler group table by the scheduler tag */

          if ( hdrsrchschdgrp_schdgrp.table[k].scheduler_tag ==
                                             sect_ptr->aset.scheduler_tag )
          {
            /* A scheduler group with the same scheduler tag exists */

            break;
          }
        } /* for k */

        if ( k < hdrsrchschdgrp_schdgrp.num_grps )
        {
          /* Found a scheduler group with the same scheduler tag. 
             add the sector into the same group */

          num_sects = hdrsrchschdgrp_schdgrp.table[k].num_sects;
            /* The current scheduler group size */

          /* Check if the sect already in the sector list */

          for( l=0; l<num_sects; l++ )
          {
            if( hdrsrchschdgrp_schdgrp.table[k].sects_list[l] == sect_ptr )
            {
              /* Already in the sect list */
              break;
            }
          }

          if( ( num_sects < HDRSRCH_MAX_SUBASET_SIZE ) &&
              ( l == num_sects ) )
          {
            /* If there is a room and there is NOT a match, then add
               the sect into the scheduler group sect list */

            hdrsrchschdgrp_schdgrp.table[k].sects_list[num_sects] = sect_ptr;
              /* Add the sector into the scheduler group */

            hdrsrchschdgrp_schdgrp.table[k].num_sects ++ ;
              /* Increase the size of the scheduler group */

            sect_ptr->aset.aset_newbie = FALSE;
              /* Reset the newbie flag */
          }
          else if( l < num_sects )
          {
            HDR_MSG_SRCH_3( MSG_LEGACY_MED, 
              "Newbie sector PN(%d), band(%d), chan(%d) already in the list", 
                            sect_ptr->pn_offset,
                            sect_ptr->chan.band,
                            sect_ptr->chan.chan_num );
          }
          else
          {
            /* Invalid scheduler group size */

            ERR_FATAL( "Invalid size of the scheduelr group", 0, 0, 0);
          }
        }
        else 
        {
          /* memset a new group entry */

          memset( &hdrsrchschdgrp_schdgrp.table[hdrsrchschdgrp_schdgrp.num_grps], 
                  0, sizeof( hdrsrchschdgrp_grp_type ));

          /* A new sch group is needed */

          hdrsrchschdgrp_schdgrp.num_grps ++;
            /* Increase the scheduler groups count */

          schdgrp_gid = hdrsrchschdgrp_get_gid();
            /* Get a new scheduler group id from the LRU pool */

          /* Add sector into the table */

          hdrsrchschdgrp_schdgrp.table[k].scheduler_tag =
                                           sect_ptr->aset.scheduler_tag ;
            /* Update the scheduler tag */

          hdrsrchschdgrp_schdgrp.table[k].schdgrp_gid = schdgrp_gid;
            /* Update the scheduler group id */

          hdrsrchschdgrp_schdgrp.table[k].sects_list[0] = sect_ptr;
            /* The first sector */

          hdrsrchschdgrp_schdgrp.table[k].num_sects = 1;
            /* The first sector */

          sect_ptr->aset.aset_newbie = FALSE;     
            /* Reset the newbie flag */
        }/* if k */ 
      } /* if sect_ptr */
    } /* for j*/
  } /* for i */
} /* hdrsrchschdgrp_add_pilots */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_GIDPOOL_INIT

DESCRIPTION 
  Init the schgrp table when hdrsrch task is initialized.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_gidpool_init( void ) 
{
  uint8 i;
    /* Loop index for filling link id buffer */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for (i=0; i<HDRSRCH_MAX_NUM_SCHD_GIDS; i++ )
  {
    hdrsrchschdgrp_gidpool.lru_table[i] = (hdrsrch_schdgrp_gid_type)( i+1 ) ;
      /* Assign LRU schduler group ids in ascending order; Per Data team
         request, the schduler group id starts from 1 */
  }

  hdrsrchschdgrp_gidpool.lru_idx = 0;
    /* Next get operation is at start of table */

  hdrsrchschdgrp_gidpool.mru_idx = 0;
    /* Since table is full, next put operation is also at the start of the
       table. */
} /* hdrsrchschdgrp_gidpool_init */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_INIT

DESCRIPTION 
  One time initializer for the scheduler group

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_init( void ) 
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  memset(&hdrsrchschdgrp_schdgrp, 0, sizeof( hdrsrchschdgrp_schdgrptable_type ) );
    /* Initialize the scheduler group table */

  hdrsrchschdgrp_gidpool_init();
    /* Initialize the scheduler group scheduler ID pool */
} /* hdrsrchschdgrp_gidpool_init */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_RESET

DESCRIPTION 
  Mark all the aset pilots as aset newbies and reset the scheudler
  group table.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_reset( void ) 
{
  uint8 i, j, num_subaset, num_aset_pilots;
    /* Loop indexes and temporary variables. */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  num_subaset = hdrsrchset.subaset_cnt;

  for( i=0; i<num_subaset; i++ )
  {
    num_aset_pilots = hdrsrchset.subaset[i].aset_cnt;
      /* number of aset pilots */

    for( j=0; j<num_aset_pilots; j++ )
    {
      hdrsrchset.subaset[i].aset[j]->aset.aset_newbie = TRUE;
    }
  }

  hdrsrchschdgrp_tc_init();
    /* Reset the scheduler group table */
} /* hdrsrchschdgrp_reset */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_INTEGRATE_SCHDGRPS

DESCRIPTION 
  The function maintain the integrity of the scheduler group table.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchutil_integrate_schdgrps( void )
{
  int8   i, num_schdgrps;
    /* Loop index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( i=hdrsrchschdgrp_schdgrp.num_grps-1; i>=0; i-- )
  {
    num_schdgrps = hdrsrchschdgrp_schdgrp.num_grps;

    if ( hdrsrchschdgrp_schdgrp.table[i].num_sects == 0 )
    {
      /* Return the schgrp id from an empty scheduler group */

      hdrsrchschdgrp_put_gid( hdrsrchschdgrp_schdgrp.table[i].schdgrp_gid );

      /* Need update the table to remove the empty scheduler group */

      if ( ( num_schdgrps > 0 ) && ( num_schdgrps <= HDRSRCH_MAX_NUM_SCHDGRPS ) )
      {
#ifdef FEATURE_MEMCPY_REMOVAL
        hdrsrchschdgrp_schdgrp.table[i] = 
                       hdrsrchschdgrp_schdgrp.table[num_schdgrps-1];
#else
        memcpy( &hdrsrchschdgrp_schdgrp.table[i], 
                &hdrsrchschdgrp_schdgrp.table[num_schdgrps-1], 
                sizeof (hdrsrchschdgrp_grp_type ));
#endif /* FEATURE_MEMCPY_REMOVAL */
    
        /* clean the last one */

        memset( &(hdrsrchschdgrp_schdgrp.table[ num_schdgrps -1 ]),
                 0, sizeof(hdrsrchschdgrp_grp_type));
      
        /* decrease the grp number */

        hdrsrchschdgrp_schdgrp.num_grps --;
      }
      
    } /* if */
  } /* for i */
} /* hdrsrchschdgrp_integrate_schdtags */




/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_TCA_UPDATE

DESCRIPTION 
  This function is called at the end of TCA processing to update the 
  scheduler group table with the latest grouping information.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void  hdrsrchschdgrp_drop_from_schdgrp
( 
  hdrsrch_sect_struct_type *sect_ptr
)
{
  int8    i, j, num_sects;
    /* Loop index */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

   for( i=0; (i<hdrsrchschdgrp_schdgrp.num_grps) && (i < HDRSRCH_MAX_NUM_SCHDGRPS); i++ )
  {
    num_sects = hdrsrchschdgrp_schdgrp.table[i].num_sects;

    ASSERT( num_sects <= HDRSRCH_MAX_SUBASET_SIZE);

    for( j=num_sects-1; j>=0; j-- )
    {
      /* Locate the sector from the scheduler group list */

      if( sect_ptr == hdrsrchschdgrp_schdgrp.table[i].sects_list[j] )
      {
        /* Remove the sector from the scheduler group table and reduce
           the size of the scheduler group */

        hdrsrchschdgrp_schdgrp.table[i].sects_list[j] =  
                    hdrsrchschdgrp_schdgrp.table[i].sects_list[num_sects-1];
          /* Swap with last sector in the list */

        hdrsrchschdgrp_schdgrp.table[i].sects_list[num_sects-1] = NULL; 
          /* Empty the last one in the list */

        hdrsrchschdgrp_schdgrp.table[i].num_sects -- ;
          /* Reduce the group size */
      } /* if   */
    } /* for j */
  } /* for i */

  hdrsrchutil_integrate_schdgrps();
} /* void  hdrsrchschdgrp_drop_from_schdgrp( sect_ptr ) */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_TCA_UPDATE

DESCRIPTION 
  This function is called at the end of TCA processing to update the 
  scheduler group table with the latest grouping information.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

boolean hdrsrchschdgrp_tca_update( void )
{
  boolean schdgrp_valid = TRUE;  
    /* Return valud initilized to TRUE */
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  schdgrp_valid = hdrsrchschdgrp_update_schdtags();
    /* Update the scheduler tags with the current TCA/ASET. The scheduler
       tags are not persistent from TCA to TCA. It only carries the group
       information */

  schdgrp_valid &= hdrsrchschdgrp_integrity_check();
    /* Check if scheduler group split or combine happened */

  hdrsrchschdgrp_add_sects();
    /* Add newcomers into the scheduler group table */

  return schdgrp_valid;
} /* void hdrsrchschdgrp_tca_update */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_GET_SCHDGRP_RPT

DESCRIPTION 
  The function returns the information of outstanding scheduler groups. 

DEPENDENCIES
  None

RETURN VALUE
  schdgrp_rpt - ptr to the structue filled containng the outstanding
                schdgrp info.
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_get_schdgrp_rpt
(
  hdrsrch_schdgrp_rpt_type * schdgrp_rpt
)
{
  uint8                     i, j, k;
    /* Loop index */

  uint8                     carrier_cnt;
    /* Number of carriers */

  hdrsrchschdgrp_grp_type * grp_ptr;
    /* Convinience pointor */

  uint8                     subaset_idx;
    /* Subaset index */

  hdrsrch_demod_idx_type    demod_idx;
    /* Demod index */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  schdgrp_rpt->num_schdgrps = hdrsrchschdgrp_schdgrp.num_grps;
    /* The number of outstanding scheduler groups */

  for( i=0; i<schdgrp_rpt->num_schdgrps; i++ )
  {
    /* Counting the carrier number by counting how many different demod 
       indexes exsit in the scheduler group table */

    grp_ptr = &( hdrsrchschdgrp_schdgrp.table[i] );
      /* Convinience pointor */

    carrier_cnt = 0;
      /* Reset the carrier count */

    for( j=0; j<grp_ptr->num_sects; j++ )
    {
      /* The following logic counts the number of demod_idx in the current
         scheduler group */

      ASSERT( j < HDRSRCH_MAX_SUBASET_SIZE );
        /* Array boundary sanity check */

      subaset_idx = hdrsrchschdgrp_schdgrp.table[i].sects_list[j]->aset.subaset_idx;
        /* Get the subactive set index of te sector */

      demod_idx = hdrsrchset.subaset[subaset_idx].demod_idx;
        /* Get the demod index of the subaset the sect belongs to */

      for( k=0; k<j; k++ )
      {
        /* Loop through the list and find out if demod idx repeats */

        subaset_idx = hdrsrchschdgrp_schdgrp.table[i].sects_list[k]->aset.subaset_idx;

        if( demod_idx == hdrsrchset.subaset[subaset_idx].demod_idx )
        {
          break;
            /* Repeated demod index */
        } 
      } /* for k */

      if( k == j )
      {
        carrier_cnt ++;
      }
    } /* for j */

    /* Update the report strcutre */
 
    schdgrp_rpt->schdgrp_list[i].gid = grp_ptr->schdgrp_gid;
      /* Scheduler group ID */

    schdgrp_rpt->schdgrp_list[i].num_carriers = carrier_cnt;
      /* Carrier count for the scheduler group */
  } /* for i */
} /* hdrsrchschdgrp_get_grp_info */


/* EJECT */
/*=========================================================================

FUNCTION HDRSRCHSCHDGRP_COMMIT_TO_DSP();

DESCRIPTION 
  Commit the scheduler group id to the firmware ASP interface.

DEPENDENCIES
  None

RETURN VALUE
  None
  
SIDE EFFECTS
  None

=========================================================================*/

void hdrsrchschdgrp_commit_to_dsp()
{
  uint8 i, j;
    /* loop index */

  hdrsrch_schdgrp_gid_type schdgrp_gid;
    /* Scheduler group id */

  hdrsrch_sect_struct_type *sect;
    /* The sect under processing */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  for( i=0; i<hdrsrchschdgrp_schdgrp.num_grps; i++)
  {
    schdgrp_gid = hdrsrchschdgrp_schdgrp.table[i].schdgrp_gid;

    for( j=0; j<hdrsrchschdgrp_schdgrp.table[i].num_sects; j++)
    {
      ASSERT( j < HDRSRCH_MAX_SUBASET_SIZE );
        /* Array boundary sanity check */

      sect = hdrsrchschdgrp_schdgrp.table[i].sects_list[j];

      if( schdgrp_gid == 0 )
      {
        ERR_FATAL( "Unexpected scheduelr group ID %d", 
                   schdgrp_gid, 0, 0);
      }

      HDRASP_SET_SCHD_GID ( hdrsrchset.subaset[sect->aset.subaset_idx].demod_idx,
                            sect->aset.asp_idx,
                            schdgrp_gid );

    } /* for j */
  } /* for i */
} /* hdrsrchschdgrp_commit_to_dsp */

#endif /* FEATURE_HDR_REVB */
