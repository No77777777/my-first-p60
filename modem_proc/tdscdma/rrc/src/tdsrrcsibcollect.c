/*===========================================================================
             R R C   S I B   C O L L E C T I O N   M O D U L E  

DESCRIPTION
  
  This module is part of RRC SIB Processing procedure. This module performs
  the sib collection part of the RRC procedure. SIB collection process includes
  the following - 
    1) receiving the System Information Message 
    2) Saving the SIB segments in the temporary SIBs holder.
    3) Concatenating multiple segments belonging to the same SIB if needed.
     
  This module does not interact directly with other RRC modules except
  the SIB processing modules. 
  
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

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcsibcollect.c#1 $    $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/15/12   gv      Added support to log new event when complete SIB is received
03/10/11   ttl   Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "bit.h"
#include "comdef.h"
#include "err.h"
#include "msg.h"
#include "tdsrrcasn1.h"
#include "tdsrrcsibproci.h"
#include "tdsrrclogging.h"




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
  /*tdstsib_optimization_db used to hold sib segments received in out of order*/
tdsrrc_temp_sib_optimization_holder_type tdstsib_optimization_db[TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD];

  /*Look up table used to indicate whether a sib segment 
    of a sib tpye is presnt in temp sib optimization db or not*/
boolean tdstsib_opt_db_look_up_table[tdsrrc_MAX_SIB];

#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
/*Look up table for holding the value tag and segments of the deferred SIBs.*/
tdsrrc_deferred_sib_info_type tdsdeferred_sib_info;
#endif
/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */



/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */

/*===========================================================================

FUNCTION PROCESS_FIRST_SEG

DESCRIPTION

  This function processes a "First Segment" of a SIB. The data from the segment
  is copied to the appropriate place in the temp sib holder based on the SIB
  type.
  
DEPENDENCIES

  None.

RETURN VALUE

  A value of type TDSseg_proc_stat_e_type indicating the status of the
  processing.

SIDE EFFECTS

  None.

===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_process_first_seg
(
  tdsrrc_FirstSegment *seg, /* Pointer to the received First Segment */ 
  tdssib_events_index_e_type index,    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
  uint32 sfn_prime  /*SFN prime at which this sib segment received*/

)
{
  tdsrrc_SIB_type sib;  /* To store the received SIBs type */
  void *src;         /* Source pointer for the b_copy function */
  void *dest;        /* Destination pointer for the b_copy function */
  TDSseg_proc_stat_e_type status = TDSSEG_PROC_SUCCEEDED;/*This will say status of processing*/

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    status = TDSSEG_PROC_FAILED;
    return status;
  }
#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
    if(seg->sib_Type == (uint32)tdsrrc_SIB_Type_extensionType)
  {
    sib = tdsrrcsib_map_extension_sib_to_internal_sibtype(0,sfn_prime);
  }
  else
#endif
  {
/* Copy the received sib type. */
    sib = tdsrrcsib_map_asn1_sibtype_to_int_sibtype(seg->sib_Type);
  }

  if(sib == tdsrrc_MAX_SIB)
  {
   status = TDSSEG_PROC_FAILED;
   return status;
  }
  /* If this is SIB5bis, typecast it to SIB5 */
  if(sib == tdsrrc_SIB5bis)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Typecasting SIB5bis segment as SIB5 segment");
    sib = tdsrrc_SIB5;
  }

  /* First check if this SIB is needed. */
  if((sib < (tdsrrc_MAX_SIB - 1)) && (tdssib_events_ptr->event[index].tsib[sib].sib_needed))
  {

    /* Then check if the SIB has already been received */
    if(tdssib_events_ptr->event[index].tsib[sib].sib_received)
    {
      /* Nothing to do here, this is a duplicate */
      return(TDSSEG_PROC_DUPLICATE);
    }

    //honor the first segment, only first time.. this check is added to concatenate
    //SIB's from the last hole where it was left off..  if we received a duplicate first
    //segment then we were un-necessarily reading from first all over again even though
    //we may have received 1,2 earlier..
    if(tdssib_events_ptr->event[index].tsib[sib].seg_index != (TDSMAX_SEG_INDEX_FOR_A_SIB + 1))
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Dup. 1st seg. for sib %d",sib);
      return(TDSSEG_PROC_DUPLICATE);
    }

    /* We have received the first segment of a SIB. We need to copy
    the bits into the SIB events database. First store 
    the number of segments for this SIB. */
    TDSRRC_MSG1(MSG_LEGACY_LOW,"Rxed First seg of SIB %d",seg->sib_Type);
    tdssib_events_ptr->event[index].tsib[sib].seg_count = seg->seg_Count;

    /* Now copy the actual data bits. Use the bit copy function.
    Destination is the sib data pointer inside the sib events database.
    Source is the received segment's data pointer. */
    dest = (void *)tdssib_events_ptr->event[index].tsib[sib].sib_data_ptr;
  src = (void *)seg->sib_Data_fixed.data;

    b_copy( src,  /* Source Pointer */
            0,    /* Source Offset is always 0 since we copy all the received bits */
            dest, /* Destination Pointer */
            0,    /* Destination Offset is 0 since this is the first segment */
            TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the first segment is fixed */
          );

    /* Note the current length of this SIB */
    tdssib_events_ptr->event[index].tsib[sib].sib_tot_length = TDSFIXED_SIB_DATA_LENGTH_BITS;

    /* Since a first segment has been received, we can now make the segment
    index 0. */
    tdssib_events_ptr->event[index].tsib[sib].seg_index = 0;

    status = tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib,index);

    if(status == TDSSEG_PROC_COMPLETE)
    {
      /*Do nothing here & return same value from this function*/
    }
    else
    {
      status = TDSSEG_PROC_SUCCEEDED;
    }

    return( status );
  }
  else
  {
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }
}

/*===========================================================================

FUNCTION PROCESS_SHORT_FIRST_SEG

DESCRIPTION

  This function processes a "First Segment(Short)" of a SIB. The data from the
  segment is copied to the appropriate place in the temp sib holder based on
  the SIB type.
  
DEPENDENCIES

  None.

RETURN VALUE

  A value of type TDSseg_proc_stat_e_type indicating the status of the
  processing.

SIDE EFFECTS

  None.

===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_process_short_first_seg
(
  tdsrrc_FirstSegmentShort *seg, /* Pointer to the received First Segment */ 
  tdssib_events_index_e_type index,    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
  uint32 sfn_prime  /*SFN prime at which this sib segment received*/

)
{
  tdsrrc_SIB_type sib;  /* To store the received SIBs type */
  void *src;         /* Source pointer for the b_copy function */
  void *dest;        /* Destination pointer for the b_copy function */
  TDSseg_proc_stat_e_type status = TDSSEG_PROC_SUCCEEDED;/*This will say status of processing*/

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    status = TDSSEG_PROC_FAILED;
    return status;
  }

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
  if(seg->sib_Type == (uint32)tdsrrc_SIB_Type_extensionType)
  {
    sib = tdsrrcsib_map_extension_sib_to_internal_sibtype(0,sfn_prime);
  }
  else
#endif
  {
/* Copy the received sib type. */
    sib = tdsrrcsib_map_asn1_sibtype_to_int_sibtype(seg->sib_Type);
  }

  if(sib == tdsrrc_MAX_SIB)
  {
    status = TDSSEG_PROC_FAILED;
    return status;
  }

  /* If this is SIB5bis, typecast it to SIB5 */
  if(sib == tdsrrc_SIB5bis)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Typecasting SIB5bis segment as SIB5 segment");
    sib = tdsrrc_SIB5;
  }

  /* First check if the scheduling info for this SIB has been received. This leg might be hit when a
     BCCH-BCH block is received with some segments in it being scheduled in MIB while some are 
     in SB1/SB2 and SB1/SB2 are yet to be received.
  */
  if((sib < (tdsrrc_MAX_SIB - 1)) && (tdssib_events_ptr->sib_sched[sib].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP))
  {
   /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }

  /* First check if this SIB is needed. */
  if((sib < (tdsrrc_MAX_SIB - 1)) && (tdssib_events_ptr->event[index].tsib[sib].sib_needed))
  {

    /* Then check if the SIB has already been received */
    if(tdssib_events_ptr->event[index].tsib[sib].sib_received)
    {
      /* Nothing to do here, this is a duplicate */
      return(TDSSEG_PROC_DUPLICATE);
    }

    if(tdssib_events_ptr->event[index].tsib[sib].seg_index != (TDSMAX_SEG_INDEX_FOR_A_SIB + 1))
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Dup. 1st seg. for sib %d,Now seg_index is %d",sib,tdssib_events_ptr->event[index].tsib[sib].seg_index);
      return(TDSSEG_PROC_DUPLICATE);
    }

    /* We have received the first segment of a SIB. We need to copy
    the bits into the SIB events database. First store
    the number of segments for this SIB. */
    TDSRRC_MSG1(MSG_LEGACY_LOW,"Rxed Short First seg of SIB %d",seg->sib_Type);
    tdssib_events_ptr->event[index].tsib[sib].seg_count = seg->seg_Count;

    /* Now copy the actual data bits. Use the bit copy function.
    Destination is the sib data pointer inside the sib events database.
    Source is the received segment's data pointer. */
    dest = (void *)tdssib_events_ptr->event[index].tsib[sib].sib_data_ptr;
  src = (void *)seg->sib_Data_variable.data;

    b_copy( src,  /* Source Pointer */
            0,    /* Source Offset is always 0 since we copy all the received bits */
            dest, /* Destination Pointer */
            0,    /* Destination Offset is 0 since this is the first segment */
          (word)(seg->sib_Data_variable.numbits) /* Length of the short first segment */
                  /* This is defined as unsigned int, but the maximum value is only
                  214 */
          );

    /* Note the current length of this SIB */
  tdssib_events_ptr->event[index].tsib[sib].sib_tot_length = seg->sib_Data_variable.numbits;

    /* Since a first segment has been received, we can now make the segment
    index 0. */
    tdssib_events_ptr->event[index].tsib[sib].seg_index = 0;

    status = tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib,index);

    if(status == TDSSEG_PROC_COMPLETE)
    {
      /*Do nothing here & return same value from this function*/
    }
    else
    {
      status = TDSSEG_PROC_SUCCEEDED;
    }
    return( status );
  }
  else
  { 
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }
}



/*===========================================================================

FUNCTION PROCESS_SUBSEQUENT_SEG

DESCRIPTION

  This function processes a "Subsequent Segment" of a SIB. The data from the segment
  is copied to the appropriate place in the temp sib holder based on the SIB
  type. Since other segments of this SIB have been copied before, this data
  is concatenated to the last bit of data received in the previous segment.
  
DEPENDENCIES

  None.

RETURN VALUE

  A value of type TDSseg_proc_stat_e_type indicating the status of the
  processing.

SIDE EFFECTS

  None.

===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_process_subsequent_seg
(
  tdsrrc_SubsequentSegment *seg,  /* Pointer to the received Subsequent Segment */
  tdssib_events_index_e_type index,    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
  uint32 sfn_prime  /*SFN prime at which this sib segment received*/
)
{
  tdsrrc_SIB_type sib;  /* To store the received SIBs type */
  void *src;         /* Source pointer for the b_copy function */
  void *dest;        /* Destination pointer for the b_copy function */
  word dest_offset;  /* Destination offset for the b_copy function */
  TDSseg_proc_stat_e_type status = TDSSEG_PROC_SUCCEEDED;/*This will say status of processing*/


  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    status = TDSSEG_PROC_FAILED;
    return status;  
  }

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
  if(seg->sib_Type == (uint32)tdsrrc_SIB_Type_extensionType)
  {
    sib = tdsrrcsib_map_extension_sib_to_internal_sibtype(seg->segmentIndex, sfn_prime);
  }
  else
#endif
  {
  /* Copy the received sib type. */
    sib = tdsrrcsib_map_asn1_sibtype_to_int_sibtype(seg->sib_Type);
  }

  if(sib == tdsrrc_MAX_SIB)
  {
    status = TDSSEG_PROC_FAILED;
    return status;
  }

  /* If this is SIB5bis, typecast it to SIB5 */
  if(sib == tdsrrc_SIB5bis)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Typecasting SIB5bis segment as SIB5 segment");
    sib = tdsrrc_SIB5;
  }

  /* First check if this SIB is needed. */
  if((sib < (tdsrrc_MAX_SIB - 1)) && (tdssib_events_ptr->event[index].tsib[sib].sib_needed))
  {

    /* Then check if the SIB has already been received */
    if(tdssib_events_ptr->event[index].tsib[sib].sib_received)
    {
      /* Nothing to do here, this is a duplicate */
      return(TDSSEG_PROC_DUPLICATE);
    }

    /* We have received a subsequent segment of a SIB. We need to 
    concatenate the received bits to the end of the already received
    bits stored in the SIB events database. */
    TDSRRC_MSG1(MSG_LEGACY_LOW,"Rxed Subs Seg of SIB %d",seg->sib_Type);

    /* Check if we've received all the segments before this segment for this
    SIB. This is done by checking if the received index is one more than the
    stored index. */
    if( tdssib_events_ptr->event[index].tsib[sib].seg_index != (seg->segmentIndex - 1))
    {
      status = tdsrrcsib_store_sib_seg_in_tsib_optim_db(sib,
                                            sfn_prime,
                                            seg->segmentIndex,
                                            tdssubsequentSegment_chosen,
                                            TDSFIXED_SIB_DATA_LENGTH_BITS,
                                          (void *)seg->sib_Data_fixed.data,
                                            index);
      if(status == TDSSEG_PROC_STORED)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Gap in Seg Seq sib# %d seg# %d,SIB seg saved",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib),seg->segmentIndex);
      }
      else
      {
        /*Simply set this to TDSSEG_PROC_FAILED once more as 
        tdsrrcsib_store_sib_seg_in_tsib_optim_db() would have taken care of*/
        status = TDSSEG_PROC_FAILED;
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Gap in Seg Seq sib# %d seg# %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib),seg->segmentIndex);
      }
      return( status );
    }
    if( seg->segmentIndex > TDSMAX_SEG_INDEX_FOR_A_SIB )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Seg count exceeded! %d",seg->segmentIndex);
      return( TDSSEG_PROC_FAILED );
    }
    tdssib_events_ptr->event[index].tsib[sib].seg_index = seg->segmentIndex;

    /* Just for safety check if the segment will fit into the maximum
    allowed length for the SIB */
    if((tdssib_events_ptr->event[index].tsib[sib].sib_tot_length + 
        TDSFIXED_SIB_DATA_LENGTH_BITS) > TDSMAX_SIB_LENGTH_BITS )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Maximum SIB length exceeded!");
      return( TDSSEG_PROC_FAILED );
    }

    /* Now copy the actual data bits. Use the bit copy function.
    Destination is the sib data pointer inside the sib events database.
    Source is the received segment's data pointer.  Destination offset
    is the next bit after the total received bits so far. */
    dest = (void *)tdssib_events_ptr->event[index].tsib[sib].sib_data_ptr;
  src = (void *)seg->sib_Data_fixed.data;
    /* Ok to typecast to word here since maximum value for length is 3552 */
    dest_offset = (word)(tdssib_events_ptr->event[index].tsib[sib].sib_tot_length);

    b_copy( src,  /* Source Pointer */
            0,    /* Source Offset is always 0 since we copy all the received bits */
            dest, /* Destination Pointer */
            dest_offset,  /* Destination Offset */
            TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
          );

    /* Note the current length of this SIB. Add this segments length to the 
    previous total. */
    tdssib_events_ptr->event[index].tsib[sib].sib_tot_length = 
      (tdssib_events_ptr->event[index].tsib[sib].sib_tot_length + 
        TDSFIXED_SIB_DATA_LENGTH_BITS);
    
    status = tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib,index);

    if(status == TDSSEG_PROC_COMPLETE)
    {
      /*Do nothing here & return same value from this function*/
    }
    else
    {
      status = TDSSEG_PROC_SUCCEEDED;
    }
    return( status );    
  }
  else
  { 
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }
}





/*===========================================================================

FUNCTION PROCESS_LAST_SEG

DESCRIPTION

  This function processes a "Last Segment" of a SIB. The data from the segment
  is copied to the appropriate place in the temp sib holder based on the SIB
  type. Since other segments of this SIB have been copied before, this data
  is concatenated to the last bit of data received in the previous segment.
  
DEPENDENCIES

  None.

RETURN VALUE

  A value of type TDSseg_proc_stat_e_type indicating the status of the
  processing.

SIDE EFFECTS

  None.

===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_process_last_seg
(
  tdsrrc_LastSegment *seg,  /* Pointer to the received Last Segment */
  tdssib_events_index_e_type index,    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
  uint32 sfn_prime  /*SFN prime at which this sib segment received*/
)
{
  tdsrrc_SIB_type sib;  /* To store the received SIBs type */
  void *src;         /* Source pointer for the b_copy function */
  void *dest;        /* Destination pointer for the b_copy function */
  word dest_offset;  /* Destination offset for the b_copy function */
  TDSseg_proc_stat_e_type status = TDSSEG_PROC_SUCCEEDED;/*This will say status of processing*/

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    status = TDSSEG_PROC_FAILED;
    return status;
  }

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
  if(seg->sib_Type == (uint32)tdsrrc_SIB_Type_extensionType)
  {
    sib = tdsrrcsib_map_extension_sib_to_internal_sibtype(seg->segmentIndex, sfn_prime);
  }
  else
#endif
  {
  /* Copy the received sib type. */
    sib = tdsrrcsib_map_asn1_sibtype_to_int_sibtype(seg->sib_Type);
  }

  if(sib == tdsrrc_MAX_SIB)
  {
    status = TDSSEG_PROC_FAILED;
    return status;
  }

  /* If this is SIB5bis, typecast it to SIB5 */
  if(sib == tdsrrc_SIB5bis)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Typecasting SIB5bis segment as SIB5 segment");
    sib = tdsrrc_SIB5;
  }

  /* First check if this SIB is needed. */
  if((sib < (tdsrrc_MAX_SIB - 1)) && (tdssib_events_ptr->event[index].tsib[sib].sib_needed))
  {

    /* Then check if the SIB has already been received */
    if(tdssib_events_ptr->event[index].tsib[sib].sib_received)
    {
      /* Nothing to do here, this is a duplicate */
      return(TDSSEG_PROC_DUPLICATE);
    }

    /* We have received the last segment of a SIB. We need to 
    concatenate the received bits to the end of the already received
    bits stored in the SIB events database. */
    TDSRRC_MSG1(MSG_LEGACY_LOW,"Rxed Last Seg of SIB %d",seg->sib_Type);

    /* Check if we've received all the segments before this segment for this
    SIB. This is done by checking if the received index is one more than the
    stored index. */
    if( tdssib_events_ptr->event[index].tsib[sib].seg_index != (seg->segmentIndex - 1))
    {
      status = tdsrrcsib_store_sib_seg_in_tsib_optim_db(sib,
                                            sfn_prime,
                                            seg->segmentIndex,
                                            tdslastSegment_chosen,
                                            TDSFIXED_SIB_DATA_LENGTH_BITS,
                                          (void *)seg->sib_Data_fixed.data,
                                            index);
      if(status == TDSSEG_PROC_STORED)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Gap in Seg Seq sib# %d seg# %d,SIB seg saved",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib),seg->segmentIndex);
      }
      else
      {
        /*Simply set this to TDSSEG_PROC_FAILED once more as 
        tdsrrcsib_store_sib_seg_in_tsib_optim_db() would have taken care of*/
        status = TDSSEG_PROC_FAILED;
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Gap in Seg Seq sib# %d seg# %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib),seg->segmentIndex);
      }
      return( status );
    }
    if( seg->segmentIndex > TDSMAX_SEG_INDEX_FOR_A_SIB )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Seg count exceeded! - %d",seg->segmentIndex);
      return( TDSSEG_PROC_FAILED );
    }
    tdssib_events_ptr->event[index].tsib[sib].seg_index = seg->segmentIndex;

    /* Just for safety check if the segment will fit into the maximum
    allowed length for the SIB */
    if((tdssib_events_ptr->event[index].tsib[sib].sib_tot_length + 
        TDSFIXED_SIB_DATA_LENGTH_BITS) > TDSMAX_SIB_LENGTH_BITS )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Maximum SIB length exceeded!");
      return( TDSSEG_PROC_FAILED );
    }

    /* Now copy the actual data bits. Use the bit copy function.
    Destination is the sib data pointer inside the sib events database.
    Source is the received segment's data pointer.  Destination offset
    is the next bit after the total received bits so far. */
    dest = (void *)tdssib_events_ptr->event[index].tsib[sib].sib_data_ptr;
  src = (void *)seg->sib_Data_fixed.data;
    /* Ok to typecast to word here since maximum value for length is 3552 */
    dest_offset = (word)(tdssib_events_ptr->event[index].tsib[sib].sib_tot_length);

    b_copy( src,  /* Source Pointer */
            0,    /* Source Offset is always 0 since we copy all the received bits */
            dest, /* Destination Pointer */
            dest_offset,  /* Destination Offset */
            TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
          );

    /* Note the current length of this SIB. Add this segments length to the 
    previous total. */
    tdssib_events_ptr->event[index].tsib[sib].sib_tot_length = 
      (tdssib_events_ptr->event[index].tsib[sib].sib_tot_length + 
        TDSFIXED_SIB_DATA_LENGTH_BITS);

    /* Since this is the last segment, set the "sib_received" indicator
    to TRUE so that we won't process this SIB again. */
    tdssib_events_ptr->event[index].tsib[sib].sib_received = TRUE;
    
    status = tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib,index);

    if(status == TDSSEG_PROC_COMPLETE)
    {
      /*Do nothing here & return same value from this function*/
    }
    else
    {
      status = TDSSEG_PROC_SUCCEEDED;
    }
    return( status );    
  }
  else
  { 
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }
}




/*===========================================================================

FUNCTION PROCESS_SHORT_LAST_SEG

DESCRIPTION

  This function processes a "Last Segment(Short)" of a SIB. The data from the segment
  is copied to the appropriate place in the temp sib holder based on the SIB
  type. Since other segments of this SIB have been copied before, this data
  is concatenated to the last bit of data received in the previous segment.
  
DEPENDENCIES

  None.

RETURN VALUE

  A value of type TDSseg_proc_stat_e_type indicating the status of the
  processing.

SIDE EFFECTS

  None.

===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_process_short_last_seg
(
  tdsrrc_LastSegmentShort *seg,  /* Pointer to the received Last Segment */
  tdssib_events_index_e_type index,    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
  uint32 sfn_prime  /*SFN prime at which this sib segment received*/
)
{
  tdsrrc_SIB_type sib;  /* To store the received SIBs type */
  void *src;         /* Source pointer for the b_copy function */
  void *dest;        /* Destination pointer for the b_copy function */
  word dest_offset;  /* Destination offset for the b_copy function */
  TDSseg_proc_stat_e_type status = TDSSEG_PROC_SUCCEEDED;/*This will say status of processing*/

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    status = TDSSEG_PROC_FAILED;
    return status;
  }

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
  if(seg->sib_Type == (uint32)tdsrrc_SIB_Type_extensionType)
  {
    sib = tdsrrcsib_map_extension_sib_to_internal_sibtype(seg->segmentIndex, sfn_prime);
  }
  else
#endif
  {
  /* Copy the received sib type. */
    sib = tdsrrcsib_map_asn1_sibtype_to_int_sibtype(seg->sib_Type);
  }

  if(sib == tdsrrc_MAX_SIB)
  {
    status = TDSSEG_PROC_FAILED;
    return status;
  }

  /* If this is SIB5bis, typecast it to SIB5 */
  if(sib == tdsrrc_SIB5bis)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Typecasting SIB5bis segment as SIB5 segment");
    sib = tdsrrc_SIB5;
  }

    /* First check if the scheduling info for this SIB has been received. This leg might be hit when a
          BCCH-BCH block is received with some segments in it being scheduled in MIB while some are 
          in SB1/SB2 and SB1/SB2 are yet to be received.
          */
  if((sib < (tdsrrc_MAX_SIB - 1)) && (tdssib_events_ptr->sib_sched[sib].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP))
  {
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }

  /* First check if this SIB is needed. */
  if((sib < (tdsrrc_MAX_SIB - 1)) && (tdssib_events_ptr->event[index].tsib[sib].sib_needed))
  {

    /* Then check if the SIB has already been received */
    if(tdssib_events_ptr->event[index].tsib[sib].sib_received)
    {
      /* Nothing to do here, this is a duplicate */
      return(TDSSEG_PROC_DUPLICATE);
    }

    /* We have received the last segment(short) of a SIB. We need to 
    concatenate the received bits to the end of the already received
    bits stored in the SIB events database.  */
    TDSRRC_MSG1(MSG_LEGACY_LOW,"Received Short Last Segment of SIB %d",seg->sib_Type);

    /* Check if we've received all the segments before this segment for this
    SIB. This is done by checking if the received index is one more than the
    stored index. */
    if( tdssib_events_ptr->event[index].tsib[sib].seg_index != (seg->segmentIndex - 1))
    {
      status = tdsrrcsib_store_sib_seg_in_tsib_optim_db(sib,
                                            sfn_prime,
                                            seg->segmentIndex,
                                            tdslastSegmentShort_chosen,
                                          seg->sib_Data_variable.numbits,
                                          (void *)seg->sib_Data_variable.data,
                                            index);
      if(status == TDSSEG_PROC_STORED)
      {
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Gap in Seg Seq sib# %d seg# %d,SIB seg saved",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib),seg->segmentIndex);
      }
      else
      {
        /*Simply set this to TDSSEG_PROC_FAILED once more as 
        tdsrrcsib_store_sib_seg_in_tsib_optim_db() would have taken care of*/
        status = TDSSEG_PROC_FAILED;
        TDSRRC_MSG2(MSG_LEGACY_HIGH,"Gap in Seg Seq sib# %d seg# %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(sib),seg->segmentIndex);
      }
      return( status );
    }
    if( seg->segmentIndex > TDSMAX_SEG_INDEX_FOR_A_SIB )
    {
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Segment count exceeded! - %d",seg->segmentIndex);
      return( TDSSEG_PROC_FAILED );
    }
    tdssib_events_ptr->event[index].tsib[sib].seg_index = seg->segmentIndex;

    /* Just for safety check if the segment will fit into the maximum
    allowed length for the SIB */
    if((tdssib_events_ptr->event[index].tsib[sib].sib_tot_length + 
      seg->sib_Data_variable.numbits) > TDSMAX_SIB_LENGTH_BITS )
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Maximum SIB length exceeded!");
      return( TDSSEG_PROC_FAILED );
    }

    /* Now copy the actual data bits. Use the bit copy function.
    Destination is the sib data pointer inside the sib events database.
    Source is the received segment's data pointer.  Destination offset
    is the next bit after the total received bits so far. */
    dest = (void *)tdssib_events_ptr->event[index].tsib[sib].sib_data_ptr;
  src = (void *)seg->sib_Data_variable.data;
    /* Ok to typecast to word here since maximum value for length is 3552 */
    dest_offset = (word)(tdssib_events_ptr->event[index].tsib[sib].sib_tot_length);

    b_copy( src,  /* Source Pointer */
            0,    /* Source Offset is always 0 since we copy all the received bits */
            dest, /* Destination Pointer */
            dest_offset,  /* Destination Offset */
          (word)(seg->sib_Data_variable.numbits) /* Length of the subsequent segment is fixed */
                  /* This is defined as unsigned int, but the maximum value is only
                  214 */
          );

    /* Note the current length of this SIB. Add this segments length to the 
    previous total. */
    tdssib_events_ptr->event[index].tsib[sib].sib_tot_length = 
      (tdssib_events_ptr->event[index].tsib[sib].sib_tot_length + 
      seg->sib_Data_variable.numbits);

    /* Since this is the last segment, set the "sib_received" indicator
    to TRUE so that we won't process this SIB again. */
    tdssib_events_ptr->event[index].tsib[sib].sib_received = TRUE;
    
    status = tdsrrcsib_process_sib_seg_in_tsib_optim_db(sib,index);

    if(status == TDSSEG_PROC_COMPLETE)
    {
      /*Do nothing here & return same value from this function*/
    }
    else
    {
      status = TDSSEG_PROC_SUCCEEDED;
    }
    return( status );    
  }
  else
  { 
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }
}



/*===========================================================================

FUNCTION PROCESS_SHORT_COMPLETE_SIB

DESCRIPTION

  This function processes a short complete SIB. The SIB data
  is copied to the appropriate place in the temp sib holder 
  based on the SIB type.

DEPENDENCIES

  None.

RETURN VALUE

  A value of type TDSseg_proc_stat_e_type indicating the status of the
  processing.

SIDE EFFECTS

  None.

===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_process_short_complete_sib
(
  tdsrrc_CompleteSIBshort *seg,       /* Pointer to the received Complete SIB */
  tdssib_events_index_e_type index,    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
  uint32 sfn_prime  /*SFN prime at which this sib segment received*/

)
{
  tdsrrc_SIB_type sib; /* The received SIB */
  uint32 length;    /* Store length for copying */
  uint32 count;     /* Local counter    */

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return TDSSEG_PROC_FAILED;
  }

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
  if(seg->sib_Type == (uint32)tdsrrc_SIB_Type_extensionType)
  {
    sib = tdsrrcsib_map_extension_sib_to_internal_sibtype(0, sfn_prime);
  }
  else
#endif
  {
/* Copy the received sib type. */
    sib = tdsrrcsib_map_asn1_sibtype_to_int_sibtype(seg->sib_Type);
  }

  if(sib == tdsrrc_MAX_SIB)
  {
    return TDSSEG_PROC_FAILED;
  }

  /* If this is SIB5bis, typecast it to SIB5 */
  if(sib == tdsrrc_SIB5bis)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Typecasting SIB5bis segment as SIB5 segment");
    sib = tdsrrc_SIB5;
  }

    /* First check if the scheduling info for this SIB has been received. This leg might be hit when a
          BCCH-BCH block is received with some segments in it being scheduled in MIB while some are 
          in SB1/SB2 and SB1/SB2 are yet to be received.
          */
  if((sib < (tdsrrc_MAX_SIB - 1)) && (tdssib_events_ptr->sib_sched[sib].repitition_rate == TDSINVALID_SIB_SCHEDULE_REP))
  {
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }
  if(sib < (tdsrrc_MAX_SIB - 1))
  {
    /* First check if this SIB is needed. */
    if(!(tdssib_events_ptr->event[index].tsib[sib].sib_needed))
    {
      /* Nothing to do here since this SIB is not required */
      return(TDSSEG_PROC_NONE);
    }

    /* Then check if the SIB has already been received */
    if(tdssib_events_ptr->event[index].tsib[sib].sib_received && sib!=tdsrrc_SIB7)
    {
      /* Nothing to do here, this is a duplicate */
      return(TDSSEG_PROC_DUPLICATE);
    }

    /* Store the length to be copied, in bytes */
  length = TDSGET_PDU_LENGTH_IN_BYTES( seg->sib_Data_variable.numbits );

    /* We need to copy the entire SIB to our temporary buffer. */
    for( count = 0; count <= length; count++ )
    {
      tdssib_events_ptr->event[index].tsib[sib].sib_data_ptr[count]
      = seg->sib_Data_variable.data[count];
    }

    /* Store the length in bits */
    tdssib_events_ptr->event[index].tsib[sib].sib_tot_length 
    = seg->sib_Data_variable.numbits;

    /* Since this is a complete SIB, set the "sib_received" indicator
    to TRUE so that we won't process this SIB again. */
    tdssib_events_ptr->event[index].tsib[sib].sib_received = TRUE;
    return( TDSSEG_PROC_SUCCEEDED );
  }
  else
  {
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }
}




/*===========================================================================

FUNCTION PROCESS_COMPLETE_SIB

DESCRIPTION

  This function processes a complete SIB. The SIB data
  is copied to the appropriate place in the temp sib holder 
  based on the SIB type.
  
DEPENDENCIES

  None.

RETURN VALUE

  A value of type TDSseg_proc_stat_e_type indicating the status of the
  processing.

SIDE EFFECTS

  None.

===========================================================================*/
static TDSseg_proc_stat_e_type tdsrrcsib_process_complete_sib
(
  tdsrrc_CompleteSIB *seg,            /* Pointer to the received Complete SIB */
  tdssib_events_index_e_type index,    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
  uint32 sfn_prime  /*SFN prime at which this sib segment received*/

)
{
  tdsrrc_SIB_type sib; /* The received SIB */
  word count;       /* Local counter    */

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return TDSSEG_PROC_FAILED;
  }

#if defined(FEATURE_TDSCDMA_SIB11BIS_SUPPORT) || defined(FEATURE_TDSCDMA_SIB19)
  if(seg->sib_Type == (uint32)tdsrrc_SIB_Type_extensionType)
  {
    sib = tdsrrcsib_map_extension_sib_to_internal_sibtype(0, sfn_prime);
  }
  else
#endif
  {
  /* Copy the received sib type. */
    sib = tdsrrcsib_map_asn1_sibtype_to_int_sibtype(seg->sib_Type);
  }

  if(sib == tdsrrc_MAX_SIB)
  {
    return TDSSEG_PROC_FAILED;
  }

  /* If this is SIB5bis, typecast it to SIB5 */
  if(sib == tdsrrc_SIB5bis)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Typecasting SIB5bis segment as SIB5 segment");
    sib = tdsrrc_SIB5;
  }

  if(sib < (tdsrrc_MAX_SIB - 1))
  {
    /* First check if this SIB is needed. */
    if(!(tdssib_events_ptr->event[index].tsib[sib].sib_needed))
    {
      /* Nothing to do here since this SIB is not required */
      return(TDSSEG_PROC_NONE);
    }

    /* Then check if the SIB has already been received */
    if(tdssib_events_ptr->event[index].tsib[sib].sib_received && sib!=tdsrrc_SIB7)
    {
      /* Nothing to do here, this is a duplicate */
      return(TDSSEG_PROC_DUPLICATE);
    } 

    /* We need to copy the entire SIB to our temporary buffer. */
    for(count=0; count<TDSFIXED_COMPLETE_SIB_DATA_LENGTH_BYTES; count++)
    {
      tdssib_events_ptr->event[index].tsib[sib].sib_data_ptr[count] =
      seg->sib_Data_fixed.data[count];
    }

    /* Store the length in bits */
    tdssib_events_ptr->event[index].tsib[sib].sib_tot_length = 
      TDSFIXED_COMPLETE_SIB_DATA_LENGTH_BITS;

    /* Since this is a complete SIB, set the "sib_received" indicator
    to TRUE so that we won't process this SIB again. */
    tdssib_events_ptr->event[index].tsib[sib].sib_received = TRUE;
    return( TDSSEG_PROC_SUCCEEDED );
  }
  else
  {
    /* Nothing to do here since this SIB is not required */
    return(TDSSEG_PROC_NONE);
  }
}


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
)
{
  /* Status to be returned to the calling function.
  Assume no SIBs have been completed in this round. */
  tdsrrc_sib_collect_e_type status = TDSNO_SIBS_COMPLETED;
  /* For saving the status of segment processing    */
  TDSseg_proc_stat_e_type seg_status = TDSSEG_PROC_NONE;
  OSRTDListNode *temp_node = NULL;

  /* Pointer to a complete SIB list needed in case a
  complete SIB list is included in the message */
  tdsrrc_CompleteSIB_List *list_ptr = NULL;
  
  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    status = TDSNO_SIBS_COMPLETED;
    return status;
  }
  //Need a way to send sfn prime only once for a cell.
  //probably hook with event controller module for that
  //function to send sfn is TBD

  /* Switch based on the combination type present in
  the message */
  switch( msg->payload.t )
  {
    case( T_tdsrrc_SystemInformation_BCH_payload_noSegment ):
      /* No SIB segments included in this message */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"No Segment chosen");
      /* Nothing to do here, just break. */
      break;

    case( T_tdsrrc_SystemInformation_BCH_payload_firstSegment ):
      /* First segment included in this message   */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"First Segment chosen");
      /* Process the First Segment */
      seg_status = tdsrrcsib_process_first_seg( &(msg->payload.u.firstSegment), index, msg->sfn_Prime );
      if(seg_status == TDSSEG_PROC_COMPLETE)
      {
        status = TDSSIBS_COMPLETED;
      }
      break;


    case( T_tdsrrc_SystemInformation_BCH_payload_subsequentSegment ):
      /* Subsequent segment included in this message */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Subsequent Segment chosen");
      /* Process the Subsequent Segment */
      seg_status = tdsrrcsib_process_subsequent_seg( &(msg->payload.u.subsequentSegment), 
                                           index, msg->sfn_Prime );
      if(seg_status == TDSSEG_PROC_COMPLETE)
      {
        status = TDSSIBS_COMPLETED;
      }
      break;


    case( T_tdsrrc_SystemInformation_BCH_payload_lastSegmentShort ):
      /* Last segment included in this message */
      TDSRRC_MSG0(MSG_LEGACY_LOW," Last Segment chosen");
      /* Process the Last Segment */
      seg_status = tdsrrcsib_process_short_last_seg( &(msg->payload.u.lastSegmentShort),
                                           index, msg->sfn_Prime );
      /* Since this is the last segment, if segment processing is succesful,
      a SIB has been completely received. */
      if((seg_status == TDSSEG_PROC_SUCCEEDED )
         ||(seg_status == TDSSEG_PROC_COMPLETE)
         )
      {
#ifdef FEATURE_RRC_SIB_DEBUG
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",msg->payload.u.lastSegmentShort.sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
        tdsrrc_log_complete_sib(msg->payload.u.lastSegmentShort.sib_Type,
                             msg->payload.u.lastSegmentShort.segmentIndex,
                             msg->sfn_Prime);
        status = TDSSIBS_COMPLETED;
      }
      break;


    case( T_tdsrrc_SystemInformation_BCH_payload_lastAndFirst ):
      /* 1 last segment and 1 first segment 
      included in this message */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Last and First Segment chosen");
      /* First process the last segment */
      seg_status = tdsrrcsib_process_short_last_seg( &(msg->payload.u.lastAndFirst.lastSegmentShort), 
                                           index, msg->sfn_Prime );
      /* Since this is the last segment, if segment processing is succesful,
      a SIB has been completely received. */
      if((seg_status == TDSSEG_PROC_SUCCEEDED )
         ||(seg_status == TDSSEG_PROC_COMPLETE)
         )
      {
#ifdef FEATURE_RRC_SIB_DEBUG
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",msg->payload.u.lastAndFirst.lastSegmentShort.sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
        tdsrrc_log_complete_sib(msg->payload.u.lastAndFirst.lastSegmentShort.sib_Type,
                             msg->payload.u.lastAndFirst.lastSegmentShort.segmentIndex,
                             msg->sfn_Prime);
        status = TDSSIBS_COMPLETED;
      }
      /* Then process the First Segment */
      seg_status = tdsrrcsib_process_short_first_seg( &(msg->payload.u.lastAndFirst.firstSegment),
                                            index, msg->sfn_Prime );
      if(seg_status == TDSSEG_PROC_COMPLETE)
      {
        status = TDSSIBS_COMPLETED;
      }
      break;


    case( T_tdsrrc_SystemInformation_BCH_payload_lastAndComplete ):
      /* Last segment and one or more complete SIBS
      included in this message */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Last Segment and complete SIBs chosen");
      /* First process the last segment */
      seg_status = tdsrrcsib_process_short_last_seg( &(msg->payload.u.lastAndComplete.lastSegmentShort),
                                           index, msg->sfn_Prime );
      /* Since a last semgent is received,
      Indicate that SIBs have been completed. */
      if((seg_status == TDSSEG_PROC_SUCCEEDED )
         ||(seg_status == TDSSEG_PROC_COMPLETE)
        )
      {
#ifdef FEATURE_RRC_SIB_DEBUG
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",msg->payload.u.lastAndComplete.lastSegmentShort.sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
        tdsrrc_log_complete_sib(msg->payload.u.lastAndComplete.lastSegmentShort.sib_Type,
                             msg->payload.u.lastAndComplete.lastSegmentShort.segmentIndex,
                             msg->sfn_Prime);
        status = TDSSIBS_COMPLETED;
      }
      /* Then process each one of the complete SIBs given
      by the message. Copy the first complete SIB pointer to
      the local pointer and then traverse through subsequent 
      SIB pointers. */
      list_ptr = &(msg->payload.u.lastAndComplete.completeSIB_List);
      temp_node = list_ptr->head;
      
      if(temp_node == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB pointer received");
        return FALSE;
      }

      while(temp_node != NULL )
      {
        seg_status = tdsrrcsib_process_short_complete_sib( temp_node->data, index, msg->sfn_Prime );
        /* Update status for each SIB since we're in a while loop */
        if((seg_status == TDSSEG_PROC_SUCCEEDED )
          ||(seg_status == TDSSEG_PROC_COMPLETE)
          )
        {
#ifdef FEATURE_RRC_SIB_DEBUG
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",((tdsrrc_CompleteSIBshort*)(temp_node->data))->sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
          tdsrrc_log_complete_sib(((tdsrrc_CompleteSIBshort*)(temp_node->data))->sib_Type,0,msg->sfn_Prime);
          status = TDSSIBS_COMPLETED;
        }
        if( seg_status == TDSSEG_PROC_FAILED )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Seg proc failed on seg %d",msg->payload.t);
        }
        temp_node = temp_node->next;
      }
      temp_node = NULL;
      list_ptr = NULL;
      break;


    case( T_tdsrrc_SystemInformation_BCH_payload_lastAndCompleteAndFirst ):
    /* Last segment, one or more complete SIBS
    and a First segment included in this message */
      /* No SIB segments included in this message */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Last Segment,complete SIBs and First Segment chosen");
      /* First process the last segment */
      seg_status = tdsrrcsib_process_short_last_seg( &(msg->payload.u.lastAndCompleteAndFirst.lastSegmentShort),
                                     index, msg->sfn_Prime );
      
      /* Since a last semgent is received,
      Indicate that SIBs have been completed. */
      if((seg_status == TDSSEG_PROC_SUCCEEDED )
        ||(seg_status == TDSSEG_PROC_COMPLETE)
        )
      {
#ifdef FEATURE_RRC_SIB_DEBUG
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",msg->payload.u.lastAndCompleteAndFirst.lastSegmentShort.sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
        tdsrrc_log_complete_sib(msg->payload.u.lastAndCompleteAndFirst.lastSegmentShort.sib_Type,
                             msg->payload.u.lastAndCompleteAndFirst.lastSegmentShort.segmentIndex,
                             msg->sfn_Prime);
        status = TDSSIBS_COMPLETED;
      }
      /* Then process each one of the complete SIBs given
      by the message. Copy the first complete SIB pointer to
      the local pointer and then traverse through subsequent 
      SIB pointers. */
      list_ptr = &(msg->payload.u.lastAndComplete.completeSIB_List);
      temp_node = list_ptr->head;
      
      if(temp_node == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB pointer received");
        return FALSE;
      }

      while( temp_node != NULL )
      {
        seg_status = tdsrrcsib_process_short_complete_sib( temp_node->data, index, msg->sfn_Prime );
        /* Update status for each SIB since we're in a while loop */
        if((seg_status == TDSSEG_PROC_SUCCEEDED )
          ||(seg_status == TDSSEG_PROC_COMPLETE)
          )
        {
#ifdef FEATURE_RRC_SIB_DEBUG
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",((tdsrrc_CompleteSIBshort*)(temp_node->data))->sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
          tdsrrc_log_complete_sib(((tdsrrc_CompleteSIBshort*)(temp_node->data))->sib_Type,0,msg->sfn_Prime);
          status = TDSSIBS_COMPLETED;
        }
        temp_node = temp_node->next;
        if( seg_status == TDSSEG_PROC_FAILED )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Seg proc failed on seg %d",msg->payload.t);
        }
      }
      /* Then Process the First segment */
      seg_status = tdsrrcsib_process_short_first_seg( &(msg->payload.u.lastAndCompleteAndFirst.firstSegment),
                                            index, msg->sfn_Prime);
      if(seg_status == TDSSEG_PROC_COMPLETE)
      {
        status = TDSSIBS_COMPLETED;
      }
      temp_node = NULL;
      list_ptr = NULL;
      break;


    case( T_tdsrrc_SystemInformation_BCH_payload_completeSIB_List ):
      /* One or more complete SIBs included in this message */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Complete SIB List chosen");
      /* Process each one of the complete SIBs given
      by the message. Copy the first complete SIB pointer to
      the local pointer and then traverse through subsequent 
      SIB pointers. */
      list_ptr = &(msg->payload.u.completeSIB_List);
      temp_node = list_ptr->head;

      if(temp_node == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB pointer received");
        return FALSE;
      }

      while( temp_node != NULL )
      {
        seg_status = tdsrrcsib_process_short_complete_sib( temp_node->data, index ,msg->sfn_Prime);
        /* Update status for each SIB since we're in a while loop */
        if((seg_status == TDSSEG_PROC_SUCCEEDED )
           ||(seg_status == TDSSEG_PROC_COMPLETE)
           )
        {
#ifdef FEATURE_RRC_SIB_DEBUG
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",((tdsrrc_CompleteSIBshort*)(temp_node->data))->sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
          tdsrrc_log_complete_sib(((tdsrrc_CompleteSIBshort*)(temp_node->data))->sib_Type,0,msg->sfn_Prime);
          status = TDSSIBS_COMPLETED;
        }
        temp_node = temp_node->next;
        if( seg_status == TDSSEG_PROC_FAILED )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Seg proc failed on seg %d",msg->payload.t);
        }
      }
      temp_node = NULL;
      list_ptr = NULL;
      break;
      


    case( T_tdsrrc_SystemInformation_BCH_payload_completeAndFirst ):
      /* One or more complete SIBs and a First segment
      included in this message */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Complete SIBs and First Segment chosen");
      /* Process each one of the complete SIBs given
      by the message. Copy the first complete SIB pointer to
      the local pointer and then traverse through subsequent 
      SIB pointers. */
      list_ptr = &(msg->payload.u.completeAndFirst.completeSIB_List);
      temp_node = list_ptr->head;
      
      if(temp_node == NULL)
      {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB pointer received");
        return FALSE;
      }

      while( temp_node != NULL )
      {
        seg_status = tdsrrcsib_process_short_complete_sib(  temp_node->data, index, msg->sfn_Prime );
        /* Update status for each SIB since we're in a while loop */
        if((seg_status == TDSSEG_PROC_SUCCEEDED )
          ||(seg_status == TDSSEG_PROC_COMPLETE)
          )
        {
#ifdef FEATURE_RRC_SIB_DEBUG
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",((tdsrrc_CompleteSIBshort*)(temp_node->data))->sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
          tdsrrc_log_complete_sib(((tdsrrc_CompleteSIBshort*)(temp_node->data))->sib_Type,0,msg->sfn_Prime);
          status = TDSSIBS_COMPLETED;
        }
        temp_node = temp_node->next;
        if( seg_status == TDSSEG_PROC_FAILED )
        {
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Seg proc failed on seg %d",msg->payload.t);
        }
      }
      /* Then Process the First segment */
      seg_status = tdsrrcsib_process_short_first_seg( &(msg->payload.u.completeAndFirst.firstSegment),
                                            index, msg->sfn_Prime);
      if(seg_status == TDSSEG_PROC_COMPLETE)
      {
        status = TDSSIBS_COMPLETED;
      }
      temp_node = NULL;
      list_ptr = NULL;
      break;

    case( T_tdsrrc_SystemInformation_BCH_payload_completeSIB ):
      /* A single complete SIB is included in this message */
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Single Complete SIB chosen");
      seg_status = tdsrrcsib_process_complete_sib( &(msg->payload.u.completeSIB), index, msg->sfn_Prime);
      /* Since 1 complete SIB is received,
      Indicate that SIBs have been completed. */
      if((seg_status == TDSSEG_PROC_SUCCEEDED )
        ||(seg_status == TDSSEG_PROC_COMPLETE)
        )
      {
#ifdef FEATURE_RRC_SIB_DEBUG
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",msg->payload.u.completeSIB.sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
        tdsrrc_log_complete_sib(msg->payload.u.completeSIB.sib_Type,0,msg->sfn_Prime);
        status = TDSSIBS_COMPLETED;
      }
      break;

    case( T_tdsrrc_SystemInformation_BCH_payload_lastSegment ):
      TDSRRC_MSG0(MSG_LEGACY_LOW,"Last Segment chosen");
      /* A last segment is included in this message */
      seg_status = tdsrrcsib_process_last_seg( &(msg->payload.u.lastSegment), index, msg->sfn_Prime);
      /* Since a last segment is received,
      Indicate that SIBs have been completed. */
      if((seg_status == TDSSEG_PROC_SUCCEEDED )
        ||(seg_status == TDSSEG_PROC_COMPLETE)
        )
      {
#ifdef FEATURE_RRC_SIB_DEBUG
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sfn prime %d",msg->sfn_Prime);
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",msg->payload.u.lastAndCompleteAndFirst.lastSegmentShort.sib_Type);
#endif /* FEATURE_RRC_SIB_DEBUG */
        tdsrrc_log_complete_sib(msg->payload.u.lastSegment.sib_Type,
                             msg->payload.u.lastSegment.segmentIndex,
                             msg->sfn_Prime);
        status = TDSSIBS_COMPLETED;
      }
      break;

    default:
      /* Not a known choice value. Nothing to do here, note the
      status and move on. */
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Unknown Segment choice");
      break;

  }/* End switch */

  /* Check for incorrect segment processing */
  if( seg_status  == TDSSEG_PROC_FAILED )
  {
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Seg proc failed on combination %d",msg->payload.t);
  }
  return( status ); /* Indicate whether any SIB has been completed or not. */
}/* End function tdsrrcsib_process_sys_info_msg */
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
)
{
  uint8 i;
  /*Initialize tsib optimization db here.
    Note:Dont use memset here as Zero value for some of the fields
     e.g sib_type a value of zero is valid & it say MIB, so best is to loop 
     through & init each member of structure*/
  for(i = 0;i<TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;i++)
  {
    tdstsib_optimization_db[i].index_in_use = FALSE;
    tdstsib_optimization_db[i].seg_data_length = 0;
    tdstsib_optimization_db[i].seg_index = 0;
    tdstsib_optimization_db[i].sfn_prime = 0;
    tdstsib_optimization_db[i].sib_seg_type = tdsnoSegment_chosen;
    tdstsib_optimization_db[i].sib_type = tdsrrc_MIB;
    memset(tdstsib_optimization_db[i].seg_data_ptr,0x00, TDSFIXED_SIB_DATA_LENGTH_BYTES);
  }
  /*Initialize tsib look up table*/
  memset(tdstsib_opt_db_look_up_table,FALSE,tdsrrc_MAX_SIB);
}
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
  tdsrrc_SIB_type rrc_sib_type
)
{
  uint8 i;
  /*Initialize tsib optimization db for a sib which is passed as input to this function
   */
  for(i = 0;i<TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;i++)
  {
    if((tdstsib_optimization_db[i].sib_type == rrc_sib_type)&&
       (tdstsib_optimization_db[i].index_in_use == TRUE))
    {
      tdstsib_optimization_db[i].index_in_use = FALSE;
      tdstsib_optimization_db[i].seg_data_length = 0;
      tdstsib_optimization_db[i].seg_index = 0;
      tdstsib_optimization_db[i].sfn_prime = 0;
      tdstsib_optimization_db[i].sib_seg_type = tdsnoSegment_chosen;
      /*We dont save MIB, as its processed the moment its rexd, 
       *so use it as default value to init 
       */
      tdstsib_optimization_db[i].sib_type = tdsrrc_MIB;
      memset(tdstsib_optimization_db[i].seg_data_ptr,0x00, TDSFIXED_SIB_DATA_LENGTH_BYTES);
    }
  }
  /*Re-set this sib type in tsib look up table*/
  tdstsib_opt_db_look_up_table[rrc_sib_type] = FALSE;
}
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
  tdsrrc_SIB_type rrc_sib_type,  /*This will indicate type of SIB,e.g MIB,SIB1...etc*/
  uint8 seg_index,  /*SIB segment index*/
  tdssib_segment_e_type sib_seg_type/*Used to indicate type of sib segment*/
)
{
  uint8 i;
  uint8 first_free_indx = TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;/*Init this value to invalid*/
  boolean index_found = FALSE;
  uint8 return_index = TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;
  
  for(i = 0;i<TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;i++)
  {
    if((tdstsib_optimization_db[i].index_in_use == TRUE)&&
        (tdstsib_optimization_db[i].sib_type == rrc_sib_type)&&
        (tdstsib_optimization_db[i].seg_index == seg_index)&&
        (tdstsib_optimization_db[i].sib_seg_type == sib_seg_type)
      )
    {
      /*If this segment is already present in tsib optimization db then return the
        *same index so that it will be overwritten with the new segment data
        */
      index_found = TRUE;
      break;
    }
    else if((tdstsib_optimization_db[i].index_in_use == FALSE)&&
            (first_free_indx == TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD)
           )
    {
      first_free_indx = i;
    }
  }
  
  if(index_found == TRUE)
  {
    return_index = i;
  }
  else
  {
    return_index = first_free_indx;
  }

  return return_index;
}
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
  tdsrrc_SIB_type rrc_sib_type,  /*This will indicate type of SIB,e.g MIB,SIB1...etc*/
  uint32 sfn_prime,  /*SFN prime at which this sib segment received*/
  uint8 seg_index,  /*SIB segment index*/
  tdssib_segment_e_type sib_seg_type,/*Used to indicate type of sib segment*/
  uint32 seg_data_length,  /*SIB segment data lenght*/
  void *seg_data_ptr,  /*pointer to sib segment data*/
  tdssib_events_index_e_type event_index    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
)
{
  TDSseg_proc_stat_e_type process_status = TDSSEG_PROC_FAILED;

  if(TDSEVENT_INDEX_VALID(event_index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    return process_status;
  }

  /* Rules for storing SIB segment in Tsib optimization db.
   1) If the event is UPDATE SIB then we should not store SIB segments till
       a MIB with updated value tag is received. This will ensure that we are not 
       storing any SIB segment which belongs to the repetition cycle just before 
       new sib's are  broadcasted.
  */
  
  if((tdssib_events_ptr->event[event_index].event_name == TDSRRCSIB_UPDATE_SIBS)&&
       (tdssib_events_ptr->event[event_index].tempholder.mib_decoded == FALSE))
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"New MIB not received for Update SIBs event");
    process_status = TDSSEG_PROC_FAILED;
  }
  else
  {
    uint8 index;
    void *src;         /* Source pointer for the b_copy function */
    void *dest;        /* Destination pointer for the b_copy function */

    index = tdsrrcsib_find_free_index_in_tsib_optim_db(rrc_sib_type,
                                            seg_index,
                                            sib_seg_type);
    if(index >= TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD)
    {
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"Tsib optimization db is full,trashing rexd sib seg");
      process_status = TDSSEG_PROC_FAILED;
    }
    else
    {
      process_status = TDSSEG_PROC_STORED;
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Store sib seg=%d in tsib opti db index=%d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type),index);
      /*Found a free index in tsib optimization db
        continue storing rexd sib segment in tsib 
        optimization db*/
      tdstsib_optimization_db[index].index_in_use = TRUE;
      tdstsib_optimization_db[index].sib_type = rrc_sib_type;
      tdstsib_optimization_db[index].sib_seg_type = sib_seg_type;
      tdstsib_optimization_db[index].seg_data_length = seg_data_length;
      tdstsib_optimization_db[index].sfn_prime = sfn_prime;
      tdstsib_optimization_db[index].seg_index = seg_index;
  
      /* Now copy the actual data bits. Use the bit copy function.
         Destination is the sib data pointer inside the sib events database.
         Source is the received segment's data pointer.  Destination offset
         is the next bit after the total received bits so far. */
      dest = (void *) tdstsib_optimization_db[index].seg_data_ptr;
      src = seg_data_ptr;
    
      b_copy( src,  /* Source Pointer */
              0,    /* Source Offset is always 0 since we copy all the received bits */
              dest, /* Destination Pointer */
              0,  /* Destination Offset */
              seg_data_length  /* Length of the subsequent segment is fixed */
            );
  
      tdstsib_opt_db_look_up_table[rrc_sib_type] = TRUE;
    }
  }
  return(process_status);
}
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
  tdsrrc_SIB_type rrc_sib_type
)
{
  return(tdstsib_opt_db_look_up_table[rrc_sib_type]);
}
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
  tdsrrc_SIB_type rrc_sib_type,   /*Sib type*/
  tdssib_events_index_e_type index    /* Index into the SIB events database
                                   for storing SIBs received in this msg */
)
{
  /*Stores seg processing result, which will be used to return from this function*/
  TDSseg_proc_stat_e_type seg_process_status = TDSSEG_PROC_NONE;

  if(TDSEVENT_INDEX_VALID(index) ==FALSE)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Event Index");
    seg_process_status = TDSSEG_PROC_FAILED;
    return seg_process_status;
  }

  if(rrc_sib_type >= tdsrrc_ExtensionType)
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Invalid SIB Type");
    seg_process_status = TDSSEG_PROC_FAILED;
    return seg_process_status;
  }

  if(tdsrrcsib_is_sib_seg_in_tsib_optim_db(rrc_sib_type) == TRUE)
  {
    void *src;         /* Source pointer for the b_copy function */
    void *dest;        /* Destination pointer for the b_copy function */
    uint8 i;              /*Loop count index*/
    word dest_offset;  /* Destination offset for the b_copy function */
    boolean search_continue_flag = FALSE;

    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Sib segment for sib type =% d,found in tbib optim db ",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
    /*Start processing sib segments from tsib optimization db*/
    do
    {
      /*search_continue_flag is used to loop through when ever a 
        subsequent sib segment is received.We need to re-set this 
        flag here so that in this iteration, if we dont find a sib segment
        in tsib optimization db then we have tostop searching*/
      search_continue_flag = FALSE;
      for(i = 0;i<TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;i++)
      {
        if((tdstsib_optimization_db[i].index_in_use == TRUE)&&
           (tdstsib_optimization_db[i].sib_type == rrc_sib_type))
        {
      
          switch(tdstsib_optimization_db[i].sib_seg_type)
          {
            case tdsnoSegment_chosen:
              /*Do nothing here*/
              break;

            case tdsfirstSegment_chosen:
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"Concatinating first segment for sib %d ",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
              search_continue_flag = TRUE;
              /* Now copy the actual data bits. Use the bit copy function.
              Destination is the sib data pointer inside the sib events database.
              Source is the received segment's data pointer. */
              dest = (void *)tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_data_ptr;
              src = (void *)tdstsib_optimization_db[i].seg_data_ptr;
            
              b_copy( src,  /* Source Pointer */
                      0,    /* Source Offset is always 0 since we copy all the received bits */
                      dest, /* Destination Pointer */
                      0,    /* Destination Offset is 0 since this is the first segment */
                      tdstsib_optimization_db[i].seg_data_length/* Length of the first segment is fixed */
                    );
            
              /* Note the current length of this SIB */
              tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_tot_length = tdstsib_optimization_db[i].seg_data_length;
            
              /* Since a first segment has been received, we can now make the segment
              index 0. */
              tdssib_events_ptr->event[index].tsib[rrc_sib_type].seg_index = 0;

                /*Clear Tsib db index right here*/
              tdstsib_optimization_db[i].index_in_use = FALSE;
              tdstsib_optimization_db[i].seg_data_length = 0;
              tdstsib_optimization_db[i].seg_index = 0;
              tdstsib_optimization_db[i].sfn_prime = 0;
              tdstsib_optimization_db[i].sib_seg_type = tdsnoSegment_chosen;
              /*We dont save MIB, as its processed the moment its rexd, 
               *so use it as default value to init 
               */
              tdstsib_optimization_db[i].sib_type = tdsrrc_MIB;
              memset(tdstsib_optimization_db[i].seg_data_ptr,0x00, TDSFIXED_SIB_DATA_LENGTH_BYTES);
  
              break;

            case tdssubsequentSegment_chosen:
              if( tdssib_events_ptr->event[index].tsib[rrc_sib_type].seg_index == 
                  (uint32)(tdstsib_optimization_db[i].seg_index - 1))
              {
                TDSRRC_MSG2(MSG_LEGACY_HIGH,"Concatinating Subsequent segment index=%d for SIB type=%d",tdstsib_optimization_db[i].seg_index,tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
                search_continue_flag = TRUE;
                dest = (void *)tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_data_ptr;
                src = (void *)tdstsib_optimization_db[i].seg_data_ptr;
                /* Ok to typecast to word here since maximum value for length is 3552 */
                dest_offset = (word)(tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_tot_length);
        
                b_copy( src,  /* Source Pointer */
                        0,    /* Source Offset is always 0 since we copy all the received bits */
                        dest, /* Destination Pointer */
                        dest_offset,  /* Destination Offset */
                        TDSFIXED_SIB_DATA_LENGTH_BITS  /* Length of the subsequent segment is fixed */
                      );
        
                /* Note the current length of this SIB. Add this segments length to the 
                 previous total. */
                tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_tot_length = 
                 (tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_tot_length + 
                 TDSFIXED_SIB_DATA_LENGTH_BITS);
                tdssib_events_ptr->event[index].tsib[rrc_sib_type].seg_index = tdstsib_optimization_db[i].seg_index;
                /*Clear Tsib db index right here*/
                tdstsib_optimization_db[i].index_in_use = FALSE;
                tdstsib_optimization_db[i].seg_data_length = 0;
                tdstsib_optimization_db[i].seg_index = 0;
                tdstsib_optimization_db[i].sfn_prime = 0;
                tdstsib_optimization_db[i].sib_seg_type = tdsnoSegment_chosen;
                /*We dont save MIB, as its processed the moment its rexd, 
                 *so use it as default value to init 
                 */
                tdstsib_optimization_db[i].sib_type = tdsrrc_MIB;
                memset(tdstsib_optimization_db[i].seg_data_ptr,0x00, TDSFIXED_SIB_DATA_LENGTH_BYTES);
              }
              break;
              
              case tdslastSegment_chosen:
              case tdslastSegmentShort_chosen:
                if(tdssib_events_ptr->event[index].tsib[rrc_sib_type].seg_index == 
                  (uint32)(tdstsib_optimization_db[i].seg_index - 1))
                {
                  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Concatinating Last segment index=%d for SIB type=%d",tdstsib_optimization_db[i].seg_index,tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
                  search_continue_flag = FALSE;
                  dest = (void *)tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_data_ptr;
                  src = (void *)tdstsib_optimization_db[i].seg_data_ptr;
                  /* Ok to typecast to word here since maximum value for length is 3552 */
                  dest_offset = (word)(tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_tot_length);
                       
                  b_copy( src,  /* Source Pointer */
                          0,    /* Source Offset is always 0 since we copy all the received bits */
                          dest, /* Destination Pointer */
                          dest_offset,  /* Destination Offset */
                          tdstsib_optimization_db[i].seg_data_length/* Length of the subsequent segment is fixed */
                        );

                  /* Note the current length of this SIB. Add this segments length to the 
                   previous total. */
                  tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_tot_length = 
                   (tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_tot_length + 
                   tdstsib_optimization_db[i].seg_data_length);

                  tdssib_events_ptr->event[index].tsib[rrc_sib_type].seg_index = tdstsib_optimization_db[i].seg_index;
                  /* Since this is the last segment, set the "sib_received" indicator
                     to TRUE so that we won't process this SIB again. */
                  tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_received = TRUE;
                  seg_process_status = TDSSEG_PROC_COMPLETE;
                  TDSRRC_MSG1(MSG_LEGACY_HIGH,"Received SIB %d",tdsrrcsib_map_int_sibtype_to_asn1_sibtype(rrc_sib_type));
                  tdsrrcsib_clear_a_sib_from_tsib_optim_db(rrc_sib_type);
                }
                break;

              case tdscompleteSIB_chosen:
                search_continue_flag = FALSE;
                /* Now copy the actual data bits. Use the bit copy function.
                Destination is the sib data pointer inside the sib events database.
                Source is the received segment's data pointer. */
                dest = (void *)tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_data_ptr;
                src = (void *)tdstsib_optimization_db[i].seg_data_ptr;

                b_copy( src,  /* Source Pointer */
                        0,    /* Source Offset is always 0 since we copy all the received bits */
                        dest, /* Destination Pointer */
                        0,    /* Destination Offset is 0 since this is the first segment */
                        tdstsib_optimization_db[i].seg_data_length/* Length of the first segment is fixed */
                      );
                tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_tot_length = 
                  tdstsib_optimization_db[i].seg_data_length;
                /* Since this is a complete SIB, set the "sib_received" indicator
                to TRUE so that we won't process this SIB again. */
                tdssib_events_ptr->event[index].tsib[rrc_sib_type].sib_received = TRUE;

                seg_process_status = TDSSEG_PROC_COMPLETE;
                tdsrrcsib_clear_a_sib_from_tsib_optim_db(rrc_sib_type);

                break;

              default:
                TDSRRC_MSG1(MSG_LEGACY_HIGH,"Invalid seg type =%d",tdstsib_optimization_db[i].sib_seg_type);
                break;
          }
        }
      }
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Loop through tsib opt db search_continue_flag=%d",search_continue_flag);
    }while(search_continue_flag == TRUE);
    
  }
  else
  {
    /*Do nothing here*/
  }
  return(seg_process_status);
}

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
)
{
  tdsrrc_SIB_type sib = tdsrrc_MAX_SIB;

#ifdef FEATURE_TDSCDMA_SIB11BIS_SUPPORT
  if(tdssib_events_ptr->sib_sched[tdsrrc_SIB11bis].sfn_offset[seg_index] == 
             ((sfn_prime<<1) % tdssib_events_ptr->sib_sched[tdsrrc_SIB11bis].repitition_rate))
  {
    sib = tdsrrc_SIB11bis;
  }
  else
#endif
#ifdef FEATURE_TDSCDMA_SIB19
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
    #error code not present
#else
  if(tdssib_events_ptr->sib_sched[tdsrrc_SIB19].sfn_offset[seg_index] == 
           ((sfn_prime<<1) % tdssib_events_ptr->sib_sched[tdsrrc_SIB19].repitition_rate))
  {
    sib = tdsrrc_SIB19;
  }
  else
#endif
#endif
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unsupported extension SIB");
  }
  return sib;
}
#endif


#ifdef FEATURE_TDSCDMA_DEFERRED_SIB11_12_READING
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
)
{
  uint8 i, j=0;
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Deferred SIB: Copying TSIB to Deferred SIB DB");
  
  for(i = 0;i < TDSMAX_DEFERRED_SIB_SEGMENTS_TO_HOLD;i++)
  {
    tdsdeferred_sib_info.deferred_sib_seg_db[i].index_in_use = FALSE;
    tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_length = 0;
    tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_index = 0;
    tdsdeferred_sib_info.deferred_sib_seg_db[i].sfn_prime = 0;
    tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_seg_type = tdsnoSegment_chosen;
    tdsdeferred_sib_info.deferred_sib_seg_db[i].sib_type = tdsrrc_MAX_SIB;
    memset(tdsdeferred_sib_info.deferred_sib_seg_db[i].seg_data_ptr,0x00, TDSFIXED_SIB_DATA_LENGTH_BYTES);
  }
  
  for(i = 0;i < TDSMAX_TEMP_SIB_SEGMENTS_TO_HOLD;i++)
   {
     if((tdstsib_optimization_db[i].sib_type == tdsrrc_SIB11)||
        (tdstsib_optimization_db[i].sib_type == tdsrrc_SIB12)||
        (tdstsib_optimization_db[i].sib_type == tdsrrc_SIB11bis) ||
        (tdstsib_optimization_db[i].sib_type == tdsrrc_SIB18)
#ifdef FEATURE_TDSCDMA_SIB19
        || (tdstsib_optimization_db[i].sib_type == tdsrrc_SIB19)
#endif
        )
     {
       tdsdeferred_sib_info.deferred_sib_seg_db[j] = tdstsib_optimization_db[i];
       TDSRRC_MSG2(MSG_LEGACY_HIGH,"Deferred SIB: Copying TSIB index %d to Deferred SIB index %d",i ,j);
       j = j+1;
     }
   }
  return;
}

#endif


