/*===========================================================================
                  R R C  P A G I N G  T Y P E 1

DESCRIPTION
  This file contains the implementation for the Paging type 1 procedure.

EXTERNALIZED FUNCTIONS
  tdsrrcpg1_procedure_event_handler
    This function is the event handler for all events that are sent to the
    Paging Type 1 procedure.

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None.

Copyright (c) 2010-2012 Qualcomm Technologies, Incorporated.  
All Rights Reserved.QUALCOMM Proprietary.  Export of this technology or software is regulated
by the U.S. Government. Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrcpg1.c#1 $ $DateTime: 2019/08/19 10:51:23 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/08/11   mkh    Events renamed to TDS
03/10/11   bj     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

===========================================================================*/


/* ==========================================================================
** Includes and Variable Definitions
** ========================================================================*/

/* -----------------------------------------------------------------------
** Include Files
** ----------------------------------------------------------------------- */

#include "tdscdma_variation.h"
#include "comdef.h"

#include "tdsrrccmd_v.h"               /* RRC command definitions */
#include "msg.h"
#include "err.h"
#include "tdsrrcasn1.h"
#include "mmtask.h"
#ifdef FEATURE_CMI
#include "mmtask_v.h"
#endif
#include "tdsrrcscmgr.h"
#include "tdsrrcllc.h"
#include "tdsrrctmr.h"
#include "tdsrrclogging.h"
#include "tdsrrcrcr.h"
#include "tdsrrcmisc.h"
#include "tdsmacrrcif.h"
#include "tdsrrcsibproc.h"
#include "tdsseq.h"
#include "tdsrrcsibdb.h"
#include "tdsrrcpg1.h"
#ifdef TEST_FRAMEWORK
#error code not present
#endif
#include "tdsrrc_dependancy.h"

#include "tdsCFAParser_i.h"
#include "tdsrrcnv.h"
#include "tdsrrcdata.h"

#include "time_svc.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */

#define TDSRRCPG1_TIMER_TO_IGNORE_BCCH_MOD_INFO 200
#define TDSGROUP_RELEASE_COPY_BYTE_1 1
#define TDSGROUP_RELEASE_COPY_BYTE_2 2
#define TDSGROUP_RELEASE_COPY_BYTE_3 3
#define TDSGROUP_RELEASE_COPY_BYTE_4 4

#define TDSRRC_MAX_PAGE1 8

/* Define the shortest DRX cycle*/
#define TDSRRCPG1_MIN_DRX_CYCLE   80



/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */


typedef enum
{
  TDSRRCPG1_SUCCESS,                            /* Successful Status */      
  TDSRRCPG1_RELEASE,                            /* Release the call  */
  TDSRRCPG1_LIST_ABSENT,                        /* Indicate that paging list is absent */
  TDSRRCPG1_FAILURE_MINOR                       /* Minor Failure Status */
} tdsrrcpg1_status_e_type;

typedef enum
{
  TDSRRCPG1_CN_ORIGINATED_PAGE,                   /* CN originated paging in idle mode*/      
  TDSRRCPG1_CN_ORIGINATED_PAGE_TO_CONNECTED_UE,   /* CN originated paging in connected mode*/
  TDSRRCPG1_UTRAN_ORIGINATED_PAGE_TO_CONNECTED_UE /* UTRAN originated paging in connected mode*/
} tdsrrcpg1_paging_type_e_type;


/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

rrc_cn_domain_identity_e_type   tdsrrcpg1_cn_domain_id;
rrc_paging_cause_e_type         tdsrrcpg1_paging_cause;

  /*flag to indicate the paging to upper layers */
static tdsrrcpg1_paging_type_e_type  tdsrrcpg1_paging_type;
/* The UE ID used for paging */
rrc_page_ue_id_choice_e_type    tdsrrcpg1_page_ue_id;

/*This variable is used in file scope only and set for global for debugging purpose only*/
uint32 tdsrrcpg1_num_pages;        /* Number of Paging Type 1 messages 
                                         received */

/*This variable is used in file scope only and set for global for debugging purpose only*/
uint32 tdsrrcpg1_num_pages_matched; 
                                      /* Number of Pages that are 
                                         matched */
static boolean tdsrrcpg1_bcch_mod_timer_status;
                                      /* Status of Timer to ignore
                                         BCCH Modifcation Info */
                                        
/*Saved Mib value tag & BCCH Mod Time from OTA message*/
static tdsrrcpg1_paging_bcch_mod_info tdsrrcpg1_bcch_mod_info;


#ifdef FEATURE_HANDLE_CN_PAGE_IN_CONNECTED_STATE
/*Set whenever goes into PCH 
  Reset whenever UE is actual paged in PCH
  No need to reset when jump out of PCH*/
boolean tdsrrcpg1_ignore_cn_page_in_conn = FALSE;
#endif
/* Define tdsrrcpg1_page_detect_pattern to record all variables of page pattern detect*/
tdsrrcpg1_page_detect_pattern_type tdsrrcpg1_page_detect_pattern;
/* Record time of previous paging type1 message*/
uint32 tdsrrcpg1_previous_pg1_recv_time = 0;
/* Record total paging record number in one DRX cycle*/
uint8 tdsrrcpg1_page_records_num_in_page_cycle = 0;

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */



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
  for example:- function bar() is just below function foo() then function bar() needs to be written like this ?. 
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */



/*===========================================================================

FUNCTION tdsrrcpg1_modify_sibs_for_sfn_update/tdsrrcpg1_ff_modify_sibs_for_sfn_update

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
#ifndef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/*boolean tdsrrcpg1_modify_sibs_for_sfn_update*/
/* before tdsl1rrcif.h change, the type is mis-match for above return value*/
/*we should also check extern in tdsl1rrcif.h*/
void tdsrrcpg1_modify_sibs_for_sfn_update
#else
void tdsrrcpg1_ff_modify_sibs_for_sfn_update
#endif
(
  void
)
{
  tdsrrc_cmd_type *rrc_cmd_ptr;            /* Pointer to the RRC Command */
  tdsrrc_MasterInformationBlock * mib_ptr;
  tdsrrc_state_e_type rrc_state;
  boolean sib_active, status = FALSE;
  boolean is_n_bcch_up = FALSE;

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  tdsl1_ext_cmd_type *l1_cmd_ptr;
#endif

  sib_active = tdsrrcsib_is_sib_active();
  rrc_state = tdsrrc_get_state();

  TDSRRC_MSG3(MSG_LEGACY_ERROR,"sfn update sib %d, rrc %d, timer %d", sib_active, rrc_state, tdsrrcpg1_bcch_mod_timer_status);

  is_n_bcch_up = tdsrrcllc_get_n_bcch_setup_status();

  TDSRRC_MSG1(MSG_LEGACY_HIGH,"N-BCCH set up status = %d",is_n_bcch_up);

  /*If N-BCCH is Up then ignore this attempt of S-BCCH set up request from L1 
   as this function is called by L1 at a timer expiry so there is a corner case where
   in L1 will trigger this request even though N-BCCH is up*/

  if((( TDSRRC_STATE_DISCONNECTED==rrc_state)||( TDSRRC_STATE_CELL_PCH==rrc_state)
        ||( TDSRRC_STATE_URA_PCH==rrc_state)) && 
        (FALSE == sib_active)&&
        (tdssib_change_safe_guard == FALSE)&&
        (is_n_bcch_up == FALSE)
    )
  {
    if ( tdsrrcpg1_bcch_mod_timer_status == FALSE)
    {

      /* Build TDSRRC_UPDATE_SIBS_IND command to post to SIB */
      /* Allocates the buffer to RRC Internal command */
      if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
      {     
        /* Fill in the command parameters for TDSRRC_UPDATE_SIBS_IND */
        rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_UPDATE_SIBS_IND;
        rrc_cmd_ptr->cmd.update_sibs_ind.cause = TDSRRCSIB_SFN_UPDATE; 

        if((mib_ptr= (tdsrrc_MasterInformationBlock *)  tdsrrcsibdb_return_sib_for_srv_cell
           (tdsrrc_MIB)) != NULL)
        {
          rrc_cmd_ptr->cmd.update_sibs_ind.mib_value_tag = mib_ptr->mib_ValueTag;

          TDSRRC_MSG2(MSG_LEGACY_HIGH,"mib-vtag is %d sfn is %d ***",rrc_cmd_ptr->cmd.update_sibs_ind.mib_value_tag,tdsseq_get_sfn());

          rrc_cmd_ptr->cmd.update_sibs_ind.procedure = TDSRRC_PROCEDURE_PG1;
          tdsrrc_put_int_cmd(rrc_cmd_ptr);   
          status = TRUE;
        }
        else
        {
          tdsrrc_free_cmd_buf(rrc_cmd_ptr);
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"RRC_UPDATE_SIB L1 Nudge: Freeing the UPDATE_SIB cmd_ptr");
        }
      } 
      else
      {
        ERR_FATAL("Memory allocation for the %d comamnd failed",TDSRRC_UPDATE_SIBS_IND,0,0);
      }
    }
    else
    {
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH Mod Info is Ignored. Timer ON");
    }
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH Mod Info is Ignored. Incorrect RRC state or SIB event is active");
  }
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  if(status == FALSE)
  {
    if ((l1_cmd_ptr = tdsl1_get_cmd_buf()) == NULL)
    {
      ERR_FATAL("NULL Ptr when sending CPHY_IDLE_REQ",0,0,0);
    }
    
    /* Fill in the Command Header */
    l1_cmd_ptr->cmd_hdr.cmd_id = TDSL1_CPHY_NUDGE_FOR_SFN_MISS_MATCH_RSP;
    l1_cmd_ptr->cmd.rrc_nudge_status_update.status = status;
    
#ifdef FEATURE_DUAL_SIM
	l1_cmd_ptr->cmd_hdr.sys_modem_as_id = tdsrrc_get_as_id();
#endif

    /* Now send the primitive to L1 */
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Sending CPHY_NUDGE_FOR_SFN_MISS_MATCH_RSP status = FALSE");
    tdsl1_put_cmd(l1_cmd_ptr);
  }
#else
  TDSRRC_MSG1(MSG_LEGACY_HIGH,"L1 request for nudging status=%d[0:fail/1-Pass]",status);
  /* before tdsl1rrcif.h change, the type is mis-match for above return value*/
  /*we should also check extern in tdsl1rrcif.h*/
  /*return status;*/
#endif
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*******************************************************************************************
*************************************NON-Demand paging section end*****************************
*******************************************************************************************/

/* =======================================================================
**                            Function Definitions
** ======================================================================= */
/*===========================================================================

FUNCTION tdsrrcpg1_copy_group_bit_string_to_urnti  

DESCRIPTION
  This function copies the bit_string received in group-paging to local uint32

DEPENDENCIES
  None.

RETURN VALUE
  Returns an integer value of URNTI-Group.

===========================================================================*/
static uint32  tdsrrcpg1_copy_group_bit_string_to_urnti
(
  ASN1BitStr32 group_release_string,
  uint32       copy_length
)
{
  uint32 i=0;
  uint32 local_urnti=0;
  uint32 oss_string_to_urnti_iterations = 4;

  //copy the 4 bytes of ossBitString that represent the U-RNTI group into
  //a 32 bit int value..  Depending on the copy length,  copy the contents
  //from the group_release_string,  else just shift left to make it a 32
  //bit number
  for(i=0; i < oss_string_to_urnti_iterations; i++)
  {
    if(i < copy_length)
    {
      local_urnti |=  group_release_string.data[i] ;
    }
    
    //left shift it to make it a uint32 value of urnti, do it only 3 times.. last iteration, no need to 
    //left shift it as last iteration needs to be in lsb itself
    if(i < 3)
    {
      local_urnti = local_urnti << 8;
    }

  }

  return local_urnti;
}
/*===========================================================================

FUNCTION tdsrrcpg1_compare_group_release_info  

DESCRIPTION
  This function compares GROUP-URNTI received, with the URNTI stored by the
  UE.  

DEPENDENCIES
  None.

RETURN VALUE
SUCCESS:  If the comparison matches
FAILURE:  If the comparison fails

===========================================================================*/
uecomdef_status_e_type tdsrrcpg1_compare_group_release_info
(
  tdsrrc_GroupReleaseInformation * group_release_info
)
{
  tdsrrc_int_u_rnti_type u_rnti_group = 0;     /* Local vriable to store U-RNTI gotten from GROUP*/
  tdsrrc_int_u_rnti_type u_rnti_stored = 0;    /* Local vriable to store U-RNTI received from RRCLLC */

  
  uint32 compare_bit_mask_shift_right_length=0;

  //In the switch stmt below,  compare the URNTI-Group choice and convert the URNTI to a uint32 type
  switch(group_release_info->uRNTI_Group.t)
  {
  case T_tdsrrc_U_RNTI_Group_all: 
    compare_bit_mask_shift_right_length = 0;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Group Match Success for Pg1.  ALL chosen");
    return SUCCESS;
    //break absent here to keep lint happy

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b1: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b1), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_4);
    compare_bit_mask_shift_right_length = 1;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b2: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b2), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_4);
    compare_bit_mask_shift_right_length = 2;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b3: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b3), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_4);
    compare_bit_mask_shift_right_length = 3;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b4: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b4), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_4);
    compare_bit_mask_shift_right_length = 4;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b5: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b5), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_4);
    compare_bit_mask_shift_right_length = 5;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b6: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b6), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_4);
    compare_bit_mask_shift_right_length = 6;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b7: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b7), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_4);
    compare_bit_mask_shift_right_length = 7;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b8: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b8), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_3);
    compare_bit_mask_shift_right_length = 8;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b9: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b9), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_3);
    compare_bit_mask_shift_right_length = 9;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b10: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b10), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_3);
    compare_bit_mask_shift_right_length = 10;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b11: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b11), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_3);
    compare_bit_mask_shift_right_length = 11;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b12: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b12), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_3);
    compare_bit_mask_shift_right_length = 12;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b13: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b13), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_3);
    compare_bit_mask_shift_right_length = 13;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b14: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b14), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_3);
    compare_bit_mask_shift_right_length = 14;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b15: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b15), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_3);
    compare_bit_mask_shift_right_length = 15;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b16: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b16), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_2);
    compare_bit_mask_shift_right_length = 16;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b17: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b17), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_2);
    compare_bit_mask_shift_right_length = 17;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b18: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b18), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_2);
    compare_bit_mask_shift_right_length = 18;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b19: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b19), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_2);
    compare_bit_mask_shift_right_length = 19;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b20: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b20), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_2);
    compare_bit_mask_shift_right_length = 20;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b21: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b21), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_2);
    compare_bit_mask_shift_right_length = 21;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b22: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b22), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_2);
    compare_bit_mask_shift_right_length = 22;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b23: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b23), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_2);
    compare_bit_mask_shift_right_length = 23;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b24: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b24), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_1);
    compare_bit_mask_shift_right_length = 24;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b25: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b25), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_1);
    compare_bit_mask_shift_right_length = 25;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b26: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b26), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_1);
    compare_bit_mask_shift_right_length = 26;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b27: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b27), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_1);
    compare_bit_mask_shift_right_length = 27;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b28: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b28), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_1);
    compare_bit_mask_shift_right_length = 28;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b29: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b29), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_1);
    compare_bit_mask_shift_right_length = 29;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b30: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b30), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_1);
    compare_bit_mask_shift_right_length = 30;
    break;

  case T_tdsrrc_U_RNTI_Group_u_RNTI_BitMaskIndex_b31: 
    u_rnti_group = tdsrrcpg1_copy_group_bit_string_to_urnti(*(group_release_info->uRNTI_Group.u.u_RNTI_BitMaskIndex_b31), 
                                                         TDSGROUP_RELEASE_COPY_BYTE_1);
    compare_bit_mask_shift_right_length = 31;
    break;    

  default:
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Unexpected Bit-Mask.  Paging-Group No-Match");
    return FAILURE;
    //break absent here to keep lint happy
  }

  if(TDSU_RNTI_NOT_PRESENT == tdsrrcllc_get_current_urnti(&u_rnti_stored) )
  {
    ERR_FATAL("URNTI is not present",0,0,0);
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"Stored-URNTI 0x%x, Group-URNTI 0x%x, Group-Bit-Mask %d", u_rnti_stored, u_rnti_group, 
           compare_bit_mask_shift_right_length);
  
  //shift both URNTI's to match the bits' that we need to compare
  u_rnti_stored >>= compare_bit_mask_shift_right_length;
  u_rnti_group >>= compare_bit_mask_shift_right_length;

  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Shifted-Stored-URNTI 0x%x,Shifted-Group-URNTI 0x%x",u_rnti_stored,u_rnti_group);

  if(u_rnti_stored == u_rnti_group)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Group Match for Pg1");
    return SUCCESS;
  }
  else
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Group Match Failure for Pg1");
    return FAILURE;
  }
}
/*===========================================================================

FUNCTION tdsrrcpg1_update_rrc_page_ind  

DESCRIPTION
  This function updates the parameters that are required for RRC_PAGE_IND
  command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

static void tdsrrcpg1_update_rrc_page_ind
(
  tdsrrc_PagingRecord *paging_record_ptr        /* Pointer to PCCH message*/
)
{
  /* Structure to log the event when paging occurs */
  tdsrrc_event_paging_info_type    paging_event_info;

  switch(paging_record_ptr->t)
  {
    case T_tdsrrc_PagingRecord_cn_Identity:
      switch(paging_record_ptr->u.cn_Identity->cn_DomainIdentity)
      { 
        case tdsrrc_CN_DomainIdentity_cs_domain:
          tdsrrcpg1_cn_domain_id = RRC_CS_DOMAIN_CN_ID;
          break;

        case tdsrrc_CN_DomainIdentity_ps_domain:
          tdsrrcpg1_cn_domain_id = RRC_PS_DOMAIN_CN_ID;
          break;

        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown CN Domian Id: %d is received", paging_record_ptr->u.cn_Identity->
               cn_DomainIdentity,0,0);
          break;
      }

      switch(paging_record_ptr->u.cn_Identity->pagingCause)
      {
        case tdsrrc_PagingCause_terminatingConversationalCall:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_CONV_CALL;
          break;

        case tdsrrc_PagingCause_terminatingStreamingCall:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_STREAM_CALL;
          break;

        case tdsrrc_PagingCause_terminatingInteractiveCall:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_INTERACT_CALL;
          break;

        case tdsrrc_PagingCause_terminatingBackgroundCall:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_BACKGND_CALL;
          break;

        case tdsrrc_PagingCause_terminatingHighPrioritySignalling:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_HIGH_PRI_SIGNALLING;
          break;

        case tdsrrc_PagingCause_terminatingLowPrioritySignalling:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_LOW_PRI_SIGNALLING;
          break;

        case tdsrrc_PagingCause_terminatingCauseUnknown:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_UNKNOWN;
          break;


        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Paging Cause: %d",paging_record_ptr->u.cn_Identity->pagingCause);
          break;
      }
      switch(paging_record_ptr->u.cn_Identity->cn_pagedUE_Identity.t)
      {
        case T_tdsrrc_CN_PagedUE_Identity_imsi_GSM_MAP:
          tdsrrcpg1_page_ue_id = RRC_PAGE_WITH_IMSI;
          break;
       
        case T_tdsrrc_CN_PagedUE_Identity_tmsi_GSM_MAP:
          tdsrrcpg1_page_ue_id = RRC_PAGE_WITH_TMSI;
          break;

        case T_tdsrrc_CN_PagedUE_Identity_p_TMSI_GSM_MAP:
          tdsrrcpg1_page_ue_id = RRC_PAGE_WITH_PTMSI;
          break;

        case T_tdsrrc_CN_PagedUE_Identity_imsi_DS_41:
        case T_tdsrrc_CN_PagedUE_Identity_tmsi_DS_41:
        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported UE Id %d is received",paging_record_ptr->u.cn_Identity->
               cn_pagedUE_Identity.t,0,0);
          break;
      }
       
      break;

    case T_tdsrrc_PagingRecord_utran_Identity:
      switch( paging_record_ptr->u.utran_Identity->
              cn_OriginatedPage_connectedMode_UE.cn_DomainIdentity)
      {
        case tdsrrc_CN_DomainIdentity_cs_domain:
          tdsrrcpg1_cn_domain_id = RRC_CS_DOMAIN_CN_ID;
          break;

        case tdsrrc_CN_DomainIdentity_ps_domain:
          tdsrrcpg1_cn_domain_id = RRC_PS_DOMAIN_CN_ID;
          break;

        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown CN Domian Id: %d is received", 
              paging_record_ptr->u.utran_Identity->
              cn_OriginatedPage_connectedMode_UE.cn_DomainIdentity,0,0);
          break;
      }    
  
      switch(paging_record_ptr->u.utran_Identity->
             cn_OriginatedPage_connectedMode_UE.pagingCause)
      {
        case tdsrrc_PagingCause_terminatingConversationalCall:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_CONV_CALL;
          break;
    
        case tdsrrc_PagingCause_terminatingStreamingCall:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_STREAM_CALL;
          break;
    
        case tdsrrc_PagingCause_terminatingInteractiveCall:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_INTERACT_CALL;
          break;
    
        case tdsrrc_PagingCause_terminatingBackgroundCall:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_BACKGND_CALL;
          break;
      
        case tdsrrc_PagingCause_terminatingHighPrioritySignalling:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_HIGH_PRI_SIGNALLING;
          break;
      
        case tdsrrc_PagingCause_terminatingLowPrioritySignalling:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_LOW_PRI_SIGNALLING;
          break;
      
        case tdsrrc_PagingCause_terminatingCauseUnknown:
          tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_UNKNOWN;
          break;
      
        default:
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown Paging Cause: %d",paging_record_ptr->u.cn_Identity->pagingCause);
          break;
      }
      
      switch( paging_record_ptr->u.utran_Identity->
              cn_OriginatedPage_connectedMode_UE.pagingRecordTypeID)
      {
        case tdsrrc_PagingRecordTypeID_imsi_GSM_MAP:
          tdsrrcpg1_page_ue_id = RRC_PAGE_WITH_IMSI;
          break;
       
        case tdsrrc_PagingRecordTypeID_tmsi_GSM_MAP_P_TMSI:
          tdsrrcpg1_page_ue_id = RRC_PAGE_WITH_TMSI;
          break;
      
        case tdsrrc_PagingRecordTypeID_imsi_DS_41:
        case tdsrrc_PagingRecordTypeID_tmsi_DS_41:
        default:
          TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported UE Id %d is received", 
               paging_record_ptr->u.utran_Identity->cn_OriginatedPage_connectedMode_UE.
               pagingRecordTypeID,0,0);
        break;
      }
      break;

    default:
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unknown Paging Identity: %d ",paging_record_ptr->t);
      break;
  }

  /* Fill the Paging type */
  paging_event_info.paging_type = TDSRRCLOG_PAGING_TYPE_1;
  /* Populate the paging cause field with the appro[riate cause*/
  paging_event_info.paging_cause = tdsrrcpg1_paging_cause;
  /* Send the event */

  event_report_payload(EVENT_TDSCDMA_RRC_PAGE_RECEIVED, /* Paging event ID */
                       sizeof(tdsrrc_event_paging_info_type), /* payload size */
                       &paging_event_info); /* paging cause and type */
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Page1 type %d,cause %d ",paging_event_info.paging_type,paging_event_info.paging_cause);

} /* tdsrrcpg1_update_rrc_page_ind */
/*===========================================================================

FUNCTION tdsrrcpg1_validate_paging_type_msg  

DESCRIPTION
  This function looks into all Paging Records of the Paging Type 1 message
  until if finds a match for either IMSI, TMSI or PTMSI.

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcpg1_status_e_type : Returns TDSRRCPG1_SUCCESS if it finds match. Otherwise
  it returns TDSRRCPG1_FAILURE_MINOR.

===========================================================================*/

static tdsrrcpg1_status_e_type tdsrrcpg1_validate_paging_type_msg
(
  tdsrrc_PCCH_Message *pcch_msg_ptr        /* Pointer to PCCH message*/
)
{
  tdsrrcpg1_status_e_type status = TDSRRCPG1_FAILURE_MINOR;
                                        /* status is initilaized to failure */
  boolean match_found = FALSE;          /* This is local varibale to 
                                           indicates Paging Record conatins
                                           UE initial Identity or U-RNTI */

  boolean error_occur = FALSE;          /* local variable to indicate whehter 
                                           error occured during page pattern detection
                                         */   
  boolean update_result = TRUE;
  time_type up_ms;
  uint32 lo32;								                              

  rrc_imsi_gsm_map_type imsi;           /* Local variable to store the 
                                           IMSI value */
  tdsrrc_int_u_rnti_type u_rnti;           /* Local vriable to store received 
                                           U-RNTI in Paging Type 1 */
  tdsrrc_int_u_rnti_type u_rnti_local;     /* Local vriable to store U-RNTI
                                           received from RRCLLC */
  int32 num_page_records = 1;           /* Local varibales to store number of
                                           Page Records in the Paging Type 1
                                           message */
  uint32 length_in_bits;                /* Temporary variable to store length
                                           of TMSI or PTMSI in bits*/
  uint32 length_in_bytes;               /* Temporary variable to store length
                                           of TMSI or PTMSI in bytes*/
  uint32 idx =0;

#ifdef FEATURE_HANDLE_CN_PAGE_IN_CONNECTED_STATE
  boolean match_found_conn_mode_cn_id = FALSE;   /* This is local varibale to 
                                                                    indicate Paging Record conatins
                                                                    CN paged UE identity in connected mode */

  tdsrrc_cmd_type *int_cmd_ptr;            /* Pointer RRC Command */
#endif //FEATURE_HANDLE_CN_PAGE_IN_CONNECTED_STATE

 /* Check whether externsion chosen */
  if(TDSRRC_CHECK_COMMON_MSG_TYPE(pcch_msg_ptr->message,rrc_PCCH_MessageType_spare))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Extensions are not supported");
    /* Treat as error occur when extensions not support*/
    tdsrrcpg1_page_detect_pattern.error_occur = TRUE;
 
    return (status);
  }

  /* Check whether Paging Record List is present */  
  if(!(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(pcch_msg_ptr->message.u.pagingType1,
        pagingRecordList)))
  {
    TDSRRC_MSG0(MSG_LEGACY_MED,"Paging Record List is not present");
    /* pagingRecordList is not exist, treat as error occurred*/
    tdsrrcpg1_page_detect_pattern.error_occur = TRUE;
   
    return (status);
  }



  while(!(match_found) ||( num_page_records > TDSRRC_MAX_PAGE1) ) 
  {
    switch(pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx].t)
    {
      case T_tdsrrc_PagingRecord_cn_Identity:
        /* we should ignore this page if UE is in connected state */
        if( (tdsrrc_get_state() == TDSRRC_STATE_CONNECTING) || 
            (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED) ) 
        {
          tdsrrcpg1_paging_type = TDSRRCPG1_CN_ORIGINATED_PAGE;
          switch (pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx].u.
                  cn_Identity->cn_pagedUE_Identity.t)
          {
            case T_tdsrrc_CN_PagedUE_Identity_imsi_GSM_MAP:
              /* Translate IMSI to internal format */
              tdsrrc_translate_gsm_map_imsi(pcch_msg_ptr->message.u.pagingType1.
                                           pagingRecordList.elem[idx].u.cn_Identity->
                                           cn_pagedUE_Identity.u.imsi_GSM_MAP,
                                           &imsi);              /* Check whether IMSI recieved from NAS is valid */
              if(tdsrrc_imsi_valid == TRUE)
              {
                /* Search for the IMSI match in the Paging Record */
                if( (imsi.num_digits == tdsrrc_imsi.num_digits) &&
                    (!memcmp((const char *) imsi.value,(const char *) tdsrrc_imsi.value,
                    (imsi.num_digits * sizeof (uint8) ) ) )
                  )
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Search for the IMSI match in the Paging Record");
                  tdsrrcpg1_update_rrc_page_ind(&pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx]);

                  match_found = TRUE;

                  status = TDSRRCPG1_SUCCESS;
                }
              }
              else /* RRC does not have a valid IMSI. So do not look for one in
                      the Paging Record */
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Recieved with IMSI when internal IMSI is invalid");
              }
              /* If current UE is not paged, record IMSI as for monitored*/
              if((match_found == FALSE) &&
			     ((tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_START) ||
				 (tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_CONTINUE))
				)
              {
			    TDSRRC_MSG0(MSG_LEGACY_HIGH,"update IMSI");
                update_result = tdsrrcpg1_update_monitored_ue_info(MONITOR_IMSI, &imsi, NULL, NULL, 0);
              }
           
               
              break;

            case T_tdsrrc_CN_PagedUE_Identity_tmsi_GSM_MAP:
              /* Check whether TMSI recieved from NAS is valid */
              length_in_bits = TDSRRC_GET_BITS_STRING_LEN_PTR(pcch_msg_ptr->message.u.pagingType1.
                 pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                 tmsi_GSM_MAP);
              /* Convert the received length in bits to the length in bytes */
              length_in_bytes = length_in_bits / 8;

              length_in_bytes += (length_in_bits % 8) ? 1: 0;              
              if(tdsrrc_tmsi_valid == TRUE)
              {
                /* Search for the TMSI match in the Paging Record */
                if( (length_in_bytes == RRC_MAX_GSMMAP_TMSI_LENGTH) &&
                    (!memcmp((const char *)(TDSRRC_GET_BITS_PTR_STRING_PTR(pcch_msg_ptr->message.u.pagingType1.
                     pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                     tmsi_GSM_MAP)),
                     (const char *) tdsrrc_tmsi.value,RRC_MAX_GSMMAP_TMSI_LENGTH) ) 
                  )
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Search for the TMSI match in the Paging Record");
                  tdsrrcpg1_update_rrc_page_ind(&pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx]);

                  match_found = TRUE;

                  status = TDSRRCPG1_SUCCESS;
                }
              }
              else /* RRC does not have a valid TMSI. So do not look for one in
                      the Paging Record */
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Recieved with TMSI when internal TMSI is invalid");
              }
              if((match_found == FALSE) &&
			     ((tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_START) ||
				 (tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_CONTINUE))
				)
              {
                if (length_in_bytes == RRC_MAX_GSMMAP_TMSI_LENGTH)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"update TMSI");
                  update_result = tdsrrcpg1_update_monitored_ue_info(MONITOR_TMSI, 0, pcch_msg_ptr->message.u.pagingType1.
                        pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.tmsi_GSM_MAP, NULL, 0);
                }
              }
             
              break;

            case T_tdsrrc_CN_PagedUE_Identity_p_TMSI_GSM_MAP:
              length_in_bits = TDSRRC_GET_BITS_STRING_LEN_PTR(pcch_msg_ptr->message.u.pagingType1.
                   pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                   p_TMSI_GSM_MAP);
              /* Convert the received length in bits to the length in bytes */
              length_in_bytes = length_in_bits / 8;

              length_in_bytes += (length_in_bits % 8) ? 1: 0;

              /* Check whether TMSI recieved from NAS is valid */
              if(tdsrrc_ptmsi_valid == TRUE)
              {
                /* Search for the PTMSI match in the Paging Record */
                if( (length_in_bytes == RRC_MAX_GSMMAP_PTMSI_LENGTH ) &&
                    (!memcmp((const char *) (TDSRRC_GET_BITS_PTR_STRING_PTR(pcch_msg_ptr->message.u.pagingType1.
                     pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                     p_TMSI_GSM_MAP)),
                     (const char *) tdsrrc_ptmsi.value,RRC_MAX_GSMMAP_PTMSI_LENGTH) )
                  ) 
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Search for the PTMSI match in the Paging Record");
                  tdsrrcpg1_update_rrc_page_ind(&pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx]);

                  match_found = TRUE;

                  status = TDSRRCPG1_SUCCESS;
                }
              }
              else /* RRC does not have a valid PTMSI. So do not look for one in
                      the Paging Record */
              {
                TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Recieved with PTMSI when internal PTMSI is invalid");
              }
              if((match_found == FALSE) &&
			     ((tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_START) ||
				 (tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_CONTINUE)
				))
              {
                if (length_in_bytes == RRC_MAX_GSMMAP_PTMSI_LENGTH)
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"update PTMSI");
                  update_result = tdsrrcpg1_update_monitored_ue_info(MONITOR_PTMSI, 0, NULL, pcch_msg_ptr->message.u.pagingType1.
                     pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                     p_TMSI_GSM_MAP, 0);
                }
              }

              break;

            case T_tdsrrc_CN_PagedUE_Identity_imsi_DS_41:
            case T_tdsrrc_CN_PagedUE_Identity_tmsi_DS_41:
            default:
              TDSRRC_MSG3(MSG_LEGACY_HIGH,"Unsupported CN Paged UE Indentity: %d", 
                        pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx].
                        u.cn_Identity->cn_pagedUE_Identity.t,0,0);
              break;
          }

        }
        else
        {
          TDSRRC_MSG1(MSG_LEGACY_ERROR,"Wrong CN orig page in State %d",tdsrrc_get_state());

#ifdef FEATURE_HANDLE_CN_PAGE_IN_CONNECTED_STATE
  
            tdsrrcpg1_paging_type = TDSRRCPG1_CN_ORIGINATED_PAGE;
            switch (pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx].u.
                    cn_Identity->cn_pagedUE_Identity.t)
            {
              case T_tdsrrc_CN_PagedUE_Identity_imsi_GSM_MAP:
                /* Check whether IMSI recieved from NAS is valid */
                if(tdsrrc_imsi_valid == TRUE)
                {
                  /* Translate IMSI to internal format */
                  tdsrrc_translate_gsm_map_imsi(pcch_msg_ptr->message.u.pagingType1.
                                             pagingRecordList.elem[idx].u.cn_Identity->
                                             cn_pagedUE_Identity.u.imsi_GSM_MAP,
                                             &imsi);
                  /* Search for the IMSI match in the Paging Record */
                  if( (imsi.num_digits == tdsrrc_imsi.num_digits) &&
                      (!memcmp((const char *) imsi.value,(const char *) tdsrrc_imsi.value,
                               (imsi.num_digits * sizeof (uint8) ) ) )
                      )
                  {
                    match_found_conn_mode_cn_id = TRUE;
                  }
                  else
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"IMSI mismatch");
                  }
                }
                else /* RRC does not have a valid IMSI. So do not look for one in
                      the Paging Record */
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Recieved with IMSI when internal IMSI is invalid");
                }
                break;
                
              case T_tdsrrc_CN_PagedUE_Identity_tmsi_GSM_MAP:
                length_in_bits = TDSRRC_GET_BITS_STRING_LEN_PTR(pcch_msg_ptr->message.u.pagingType1.
                    pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                    tmsi_GSM_MAP);
                /* Convert the received length in bits to the length in bytes */
                length_in_bytes = length_in_bits / 8;
                  
                length_in_bytes += (length_in_bits % 8) ? 1: 0;

                /* Check whether TMSI recieved from NAS is valid */
                if(tdsrrc_tmsi_valid == TRUE)
                {                  
                  /* Search for the TMSI match in the Paging Record */
                  if( (length_in_bytes == RRC_MAX_GSMMAP_TMSI_LENGTH) &&
                      (!memcmp((const char *)(TDSRRC_GET_BITS_PTR_STRING_PTR(pcch_msg_ptr->message.u.pagingType1.
                               pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                               tmsi_GSM_MAP)),
                               (const char *) tdsrrc_tmsi.value,RRC_MAX_GSMMAP_TMSI_LENGTH) ) 
                      )
                  {
                    match_found_conn_mode_cn_id = TRUE;
                  }
                  else
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"TMSI mismatch");
                  }
                }
                else /* RRC does not have a valid TMSI. So do not look for one in
                      * the Paging Record */
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Recieved with TMSI when internal TMSI is invalid");
                }
                break;
                
              case T_tdsrrc_CN_PagedUE_Identity_p_TMSI_GSM_MAP:
                length_in_bits = TDSRRC_GET_BITS_STRING_LEN_PTR(pcch_msg_ptr->message.u.pagingType1.
                    pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                    p_TMSI_GSM_MAP);
                /* Convert the received length in bits to the length in bytes */
                length_in_bytes = length_in_bits / 8;
                  
                length_in_bytes += (length_in_bits % 8) ? 1: 0;

                
                /* Check whether TMSI recieved from NAS is valid */
                if(tdsrrc_ptmsi_valid == TRUE)
                {
                  /* Search for the PTMSI match in the Paging Record */
                  if( (length_in_bytes == RRC_MAX_GSMMAP_PTMSI_LENGTH ) &&
                      (!memcmp((const char *)(TDSRRC_GET_BITS_PTR_STRING_PTR(pcch_msg_ptr->message.u.pagingType1.
                               pagingRecordList.elem[idx].u.cn_Identity->cn_pagedUE_Identity.u.
                               p_TMSI_GSM_MAP)),
                               (const char *) tdsrrc_ptmsi.value,RRC_MAX_GSMMAP_PTMSI_LENGTH) )
                      ) 
                  {
                    match_found_conn_mode_cn_id = TRUE;
                  }
                  else
                  {
                    TDSRRC_MSG0(MSG_LEGACY_HIGH,"PTMSI mismatch");
                  }
                }
                else /* RRC does not have a valid PTMSI. So do not look for one in
                        the Paging Record */
                {
                  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Recieved with PTMSI when internal PTMSI is invalid");
                }
                break;

              case T_tdsrrc_CN_PagedUE_Identity_imsi_DS_41:
              case T_tdsrrc_CN_PagedUE_Identity_tmsi_DS_41:
              default:
                TDSRRC_MSG3(MSG_LEGACY_HIGH,"Unsupported CN Paged UE Indentity: %d", 
                         pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx].
                         u.cn_Identity->cn_pagedUE_Identity.t,0,0);
                break;
            }
                
          
#endif /*FEATURE_HANDLE_CN_PAGE_IN_CONNECTED_STATE */

        }
        break;

      
      case T_tdsrrc_PagingRecord_utran_Identity:
        if( (tdsrrc_get_state() == TDSRRC_STATE_CONNECTING) || 
            (tdsrrc_get_state() == TDSRRC_STATE_DISCONNECTED) ) 
        {
          /*lint -e666 */
          TDSRRC_MSG1(MSG_LEGACY_HIGH,"Wrong Paging Origination in State %d",tdsrrc_get_state());
          /*lint +e666 */
          error_occur = TRUE;

        }
        else
        {
          tdsrrc_translate_urnti(&pcch_msg_ptr->message.u.pagingType1.
                              pagingRecordList.elem[idx].
                              u.utran_Identity->u_RNTI, &u_rnti);

          if(TDSU_RNTI_NOT_PRESENT == tdsrrcllc_get_current_urnti(&u_rnti_local) )
          {
            ERR_FATAL("URNTI is not present",0,0,0);
          }

          if(u_rnti == u_rnti_local)
          {
            if(pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx].
               u.utran_Identity->m.cn_OriginatedPage_connectedMode_UEPresent)
            {
              tdsrrcpg1_update_rrc_page_ind(&pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx]);

              tdsrrcpg1_paging_type = TDSRRCPG1_CN_ORIGINATED_PAGE_TO_CONNECTED_UE;

              match_found = TRUE;

              status = TDSRRCPG1_SUCCESS;
            }    
            /* if it is not CN originated page then do not inform upper layers about 
            paging but UE has to initiate cell update so return success here */
            else
            {
              match_found = TRUE;

              tdsrrcpg1_paging_type = TDSRRCPG1_UTRAN_ORIGINATED_PAGE_TO_CONNECTED_UE;

              status = TDSRRCPG1_SUCCESS;
            }
          }
          if((match_found == FALSE) &&
		     ((tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_START) ||
			 (tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_CONTINUE)
		    ))
          {
		    TDSRRC_MSG0(MSG_LEGACY_HIGH,"update URNTI");
            update_result = tdsrrcpg1_update_monitored_ue_info(MONITOR_URNTI, 0, NULL, NULL, u_rnti);
          }

        }
        break;
     
     default:
       TDSRRC_MSG1(MSG_LEGACY_HIGH,"Unknown Paging Identity: %d ",pcch_msg_ptr->message.u.pagingType1.pagingRecordList.elem[idx].t);
       break;
    }
    num_page_records++;                /* Increment the Page Record number */

    /* Advance the linked list */
    idx++;

    /* Check if the pointer is NULL */
    if( pcch_msg_ptr->message.u.pagingType1.pagingRecordList.n <= idx )
    {
      TDSRRC_MSG0(MSG_LEGACY_MED,"Paging record list ended");
      break; /* Break out of the while loop */
    }
  }

#ifdef FEATURE_HANDLE_CN_PAGE_IN_CONNECTED_STATE
  /* If we receive CN Identity Page in connected mode and no Utran Page Identity
    is received for us in this Paging record List, we will transition to Idle in order to 
    maintain sync with Network*/
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"PG1: match_found for UTRAN ID Page is %d,and for for CN ID Page is %d ",match_found,match_found_conn_mode_cn_id);
  if((match_found == FALSE) && (match_found_conn_mode_cn_id == TRUE))
  {
    if(tdsrrcpg1_ignore_cn_page_in_conn == FALSE)
    {
    /* Allocates the buffer to RRC Internal command */
    if( (int_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
    { 
      int_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_INITIATE_CELL_SELECTION_IND;
      int_cmd_ptr->cmd.initiate_cell_sel_ind.cause = TDSRRC_TRANSITION_TO_DISCONNECTED;
      /* pretend as if it is coming from CU so that CSP will process it properly*/
      int_cmd_ptr->cmd.initiate_cell_sel_ind.proc = TDSRRC_PROCEDURE_CU;
      int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_freq_present = FALSE;
      int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_cpid_present = FALSE;
      int_cmd_ptr->cmd.initiate_cell_sel_ind.dest_state = TDSRRC_STATE_DISCONNECTED;
      int_cmd_ptr->cmd.initiate_cell_sel_ind.tx_to_discon_reason = TDSRRC_TX_TO_DISCON_OTHER;
      
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Posting cmd to enter IDLE");
      tdsrrc_put_int_cmd(int_cmd_ptr); 
    }
    else
    {
      ERR("Failed to allocate memory",0,0,0);
    }
  }
    else
    {
      tdsrrcpg1_ignore_cn_page_in_conn = FALSE;
      TDSRRC_MSG0(MSG_LEGACY_HIGH,"Ignore CN-paing one time when UE is in PCH. Reset tdsrrcpg1_ignore_cn_page_in_conn");
    }

  }
#endif

/* Set whether current paging is high loading(too many UE in one DRX cycle): 
 * tdsrrcpg1_page_records_num_in_page_cycle > TDS_PAGE_PATTERN_DETECT_HIGH_LOADING_UE_NUM
 */
  /*Get current system time*/
  time_get_uptime_ms(up_ms);
  lo32 = qw_lo(up_ms);
  /* If interval between 2 consecutive paging type1 is smaller than MIN DRX cycle, treat them are received in one DRX cycle
   * Else they should be received in different DRX cycle 
   */
  if ((lo32 -tdsrrcpg1_previous_pg1_recv_time) < TDSRRCPG1_MIN_DRX_CYCLE)
  {
    tdsrrcpg1_page_records_num_in_page_cycle = tdsrrcpg1_page_records_num_in_page_cycle + pcch_msg_ptr->message.u.pagingType1.pagingRecordList.n;
  }
  else   
  {
    tdsrrcpg1_page_records_num_in_page_cycle = pcch_msg_ptr->message.u.pagingType1.pagingRecordList.n;
  }
  tdsrrcpg1_previous_pg1_recv_time = lo32;
  /*Check whether high loading threshold is fulfilled*/
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"page records num in 1 page cycle is %d, high loading thresh is %d",tdsrrcpg1_page_records_num_in_page_cycle,tdsrrcpg1_page_detect_pattern.high_loading_thesh);
  if (tdsrrcpg1_page_records_num_in_page_cycle > tdsrrcpg1_page_detect_pattern.high_loading_thesh)
  {
     tdsdrxmgr_pch_det_patt_set_paging_load_value(1);
     TDSRRC_MSG0(MSG_LEGACY_HIGH,"High loading detected");
  }

  TDSRRC_MSG3(MSG_LEGACY_HIGH,"error_occur %d, match found %d, update result %d",error_occur,match_found,update_result);
  if ((tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_START)
     ||(tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_CONTINUE))
  {
    if ((error_occur == TRUE) || (match_found == TRUE) || (update_result == FALSE))
    {
      tdsrrcpg1_page_detect_pattern.error_occur = TRUE;
    }
  }
  else
  {
    tdsrrcpg1_page_detect_pattern.error_occur = FALSE;
  }

  return(status);

} /*tdsrrcpg1_validate_paging_type_msg */

/*===========================================================================

FUNCTION tdsrrcpg1_validate_paging_type_msg_r5 

DESCRIPTION
  This function looks into all Paging Records of the Paging Type 1 message
  until if finds a match for either IMSI, TMSI or PTMSI.

DEPENDENCIES
  None.

RETURN VALUE
  tdsrrcpg1_status_e_type : Returns TDSRRCPG1_SUCCESS if it finds match. Otherwise
  it returns TDSRRCPG1_FAILURE_MINOR.

===========================================================================*/

static tdsrrcpg1_status_e_type tdsrrcpg1_validate_paging_type_msg_r5
(
  tdsrrc_PCCH_Message *pcch_msg_ptr        /* Pointer to PCCH message*/
)
{
  tdsrrcpg1_status_e_type status = TDSRRCPG1_FAILURE_MINOR;
                                        /* status is initilaized to failure */
  tdsrrc_int_u_rnti_type u_rnti;           /* Local vriable to store received 
                                           U-RNTI in Paging Type 1 */
  tdsrrc_int_u_rnti_type u_rnti_local;     /* Local vriable to store U-RNTI
                                           received from RRCLLC */

  tdsrrc_PagingRecord2List_r5 *local_paging_record_List;
                                       /* Local Pointer to Paging Record List*/

  tdsrrc_state_e_type  rrc_state;
  tdsrrc_PagingRecord2_r5_utran_GroupIdentity  *local_group_release_list;
  uint32 idx =0,idy=0;
 /* Check whether externsion chosen */
  if( TDSRRC_CHECK_COMMON_MSG_TYPE(pcch_msg_ptr->message,rrc_PCCH_MessageType_spare))
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Extensions are not supported");
    return (status);
  }

#ifdef FEATURE_TDSCDMA_REL8
  if(pcch_msg_ptr->message.u.pagingType1.laterNonCriticalExtensions.v590NonCriticalExtensions.
     m.v860NonCriticalExtensionsPresent)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"REL8: v860NonCriticalExtensions not supported");
    status = TDSRRCPG1_LIST_ABSENT;
    return (status);
  }
#endif /*FEATURE_TDSCDMA_REL8*/

  if(!pcch_msg_ptr->message.u.pagingType1.laterNonCriticalExtensions.v590NonCriticalExtensions.
      pagingType1_v590ext.m.pagingRecord2ListPresent)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Paging Record List not Present");
    status = TDSRRCPG1_LIST_ABSENT;
    return (status);
  }

  //for rel-5 only Utran Originated Paging is supported by ASN1
  rrc_state = tdsrrc_get_state();
  if( (TDSRRC_STATE_CONNECTING == rrc_state) || 
      (TDSRRC_STATE_DISCONNECTED == rrc_state) ) 
  {
    /*lint -e666 */
    TDSRRC_MSG1(MSG_LEGACY_HIGH,"Wrong Paging Origination in State %d",tdsrrc_get_state());
    return (status);
    /*lint +e666 */
  }

  /* Update the head of Linked list */
  local_paging_record_List =  &pcch_msg_ptr->message.u.pagingType1.laterNonCriticalExtensions.
    v590NonCriticalExtensions.pagingType1_v590ext.pagingRecord2List;
  idx =0;
  while(local_paging_record_List->n > idx) 
  {
    if(local_paging_record_List->elem[idx].t == T_tdsrrc_PagingRecord2_r5_utran_SingleUE_Identity)
    {
      tdsrrc_translate_urnti(&local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->
                          u_RNTI, &u_rnti);

      if(TDSU_RNTI_NOT_PRESENT == tdsrrcllc_get_current_urnti(&u_rnti_local) )
      {
        ERR_FATAL("URNTI is not present",0,0,0);
      }

      if(u_rnti == u_rnti_local)
      {
        //if network is indicating a call release, then update the release cause and return release enum
        /*According to 25.331 spec section 8.1.2.3 I.E "Release indicator" should be handled independently 
         *of I.E- "CN originated page to connected mode UE" which is optional I.E in Paging record.
         */
        if(local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->rrc_ConnectionReleaseInformation.t == 
            T_tdsrrc_RRC_ConnectionReleaseInformation_release)
        {
          //update the release cause and return release enum
          tdsrrc_release_cause = tdsrrcrcr_update_release_cause(local_paging_record_List->elem[idx].u.
                                                          utran_SingleUE_Identity->
                                                          rrc_ConnectionReleaseInformation.u.
                                                          release->releaseCause);
          return TDSRRCPG1_RELEASE;
        }
     
        if(local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->m.cn_OriginatedPage_connectedMode_UEPresent)
        {
          switch( local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->
                  cn_OriginatedPage_connectedMode_UE.cn_DomainIdentity)         
          { 
            case tdsrrc_CN_DomainIdentity_cs_domain:
              tdsrrcpg1_cn_domain_id = RRC_CS_DOMAIN_CN_ID;
              break;
    
            case tdsrrc_CN_DomainIdentity_ps_domain:
              tdsrrcpg1_cn_domain_id = RRC_PS_DOMAIN_CN_ID;
              break;
    
            default:
              TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown CN Domian Id: %d is received", 
                  local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->
                  cn_OriginatedPage_connectedMode_UE.cn_DomainIdentity,0,0);
              break;
          }    

          switch( local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->
                  cn_OriginatedPage_connectedMode_UE.pagingCause)
          {
          case tdsrrc_PagingCause_terminatingConversationalCall:
            tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_CONV_CALL;
            break;
    
          case tdsrrc_PagingCause_terminatingStreamingCall:
            tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_STREAM_CALL;
            break;
    
          case tdsrrc_PagingCause_terminatingInteractiveCall:
            tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_INTERACT_CALL;
            break;
    
          case tdsrrc_PagingCause_terminatingBackgroundCall:
            tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_TERM_BACKGND_CALL;
            break;
    
          case tdsrrc_PagingCause_terminatingHighPrioritySignalling:
            tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_HIGH_PRI_SIGNALLING;
            break;
    
          case tdsrrc_PagingCause_terminatingLowPrioritySignalling:
            tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_LOW_PRI_SIGNALLING;
            break;
    
          case tdsrrc_PagingCause_terminatingCauseUnknown:
            tdsrrcpg1_paging_cause = RRC_PAGE_CAUSE_UNKNOWN;
            break;
    
          default:
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unknown Paging Cause: %d",local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->
               cn_OriginatedPage_connectedMode_UE.pagingCause,0,0);
            break;
          }
          
          switch( local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->
              cn_OriginatedPage_connectedMode_UE.pagingRecordTypeID)
          {
          case tdsrrc_PagingRecordTypeID_imsi_GSM_MAP:
            tdsrrcpg1_page_ue_id = RRC_PAGE_WITH_IMSI;
            break;
    
          case tdsrrc_PagingRecordTypeID_tmsi_GSM_MAP_P_TMSI:
            tdsrrcpg1_page_ue_id = RRC_PAGE_WITH_TMSI;
            break;
    
          case tdsrrc_PagingRecordTypeID_imsi_DS_41:
          case tdsrrc_PagingRecordTypeID_tmsi_DS_41:
          default:
            TDSRRC_MSG3(MSG_LEGACY_ERROR,"Unsupported UE Id %d is received", 
                local_paging_record_List->elem[idx].u.utran_SingleUE_Identity->
                cn_OriginatedPage_connectedMode_UE.pagingRecordTypeID,0,0);
            break;
          }

          tdsrrcpg1_paging_type = TDSRRCPG1_CN_ORIGINATED_PAGE_TO_CONNECTED_UE;

          return TDSRRCPG1_SUCCESS;
        }    
        /* if it is not CN originated page then do not inform upper layers about 
        paging but UE has to initiate cell update so return success here */
        else
        {
          tdsrrcpg1_paging_type = TDSRRCPG1_UTRAN_ORIGINATED_PAGE_TO_CONNECTED_UE;

          return TDSRRCPG1_SUCCESS;
        }
      }
    }
    else if(local_paging_record_List->elem[idx].t == T_tdsrrc_PagingRecord2_r5_utran_GroupIdentity)
    {
      local_group_release_list = local_paging_record_List->elem[idx].u.utran_GroupIdentity;
      idy =0;
      while(local_group_release_list->n > idy)
      {
        if(SUCCESS == tdsrrcpg1_compare_group_release_info(&local_group_release_list->
                                                     elem[idy].groupReleaseInformation))
        {
          if(local_group_release_list->elem[idy].rrc_ConnectionReleaseInformation.t == 
             T_tdsrrc_RRC_ConnectionReleaseInformation_release)  
          {
            //update the release cause and return release enum
            tdsrrc_release_cause = tdsrrcrcr_update_release_cause(local_paging_record_List->elem[idx].u.
                                                            utran_SingleUE_Identity->
                                                            rrc_ConnectionReleaseInformation.u.
                                                            release->releaseCause);
            return TDSRRCPG1_RELEASE;
          }
          else if(local_group_release_list->elem[idy].rrc_ConnectionReleaseInformation.t == 
                  T_tdsrrc_RRC_ConnectionReleaseInformation_noRelease) 
          {
            // URNTI matched but release info is set to "No Release".Do a cell update
            tdsrrcpg1_paging_type = TDSRRCPG1_UTRAN_ORIGINATED_PAGE_TO_CONNECTED_UE;
            return TDSRRCPG1_SUCCESS;
          }
        }
 
        idy++;
      }
      
      

    }
    idx++;
    /* Check if the pointer is NULL */
    if(local_paging_record_List  == NULL )
    {
      break; /* Break out of the while loop */
    }
  }

  return(status);

} /*tdsrrcpg1_validate_paging_type_msg_r5 */

/*===========================================================================

FUNCTION tdsrrcpg1_process_paging_type1_msg  

DESCRIPTION
  This function checks whether TDSRRC_DOWNLINK_SDU_IND contains Paging Type 1
  message.If it contains Paging Type 1 mesage,it will processes the same and
  send RRC_PAGE_IND to the MM. If there BCCH modification Info is present, it
  also sends TDSRRC_UPDATE_SIBS_IND to the SIB procedure.

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

static void tdsrrcpg1_process_paging_type1_msg
(
  tdsrrc_cmd_type *cmd_ptr                 /* Pointer to the RRC Command */
)
{
  tdsrrc_PCCH_Message *pcch_msg_ptr;       /* Pointer to PCCH message*/

  mm_cmd_type *mm_cmd_ptr;              /* Pointer to the MM Command */

  tdsrrc_cmd_type *rrc_cmd_ptr;            /* Pointer to the RRC Command */

  boolean false_paging_alarm = TRUE;   /* boolean showing false alarm status */

  tdsrrcpg1_status_e_type page_status = TDSRRCPG1_FAILURE_MINOR;


  /* Assert the receieved message is of type rrc_DL_PCCH_Message */
  TDSRRC_MSG0(MSG_LEGACY_HIGH,"Handle paging type1 start");
  if((TDSRRC_DOWNLINK_SDU_IND == cmd_ptr->cmd_hdr.cmd_id) && 
     (cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type == tdsrrc_PCCH_Message_PDU) )
  {
    if(cmd_ptr->cmd.downlink_sdu_ind.decoded_msg == NULL)
    {
      /* Ignore the message.  ASN1 decoding failed */ 
      TDSRRC_MSG0(MSG_LEGACY_LOW,"ASN1 decoding  Failed");
      /* Treat as error occur when ASN1 decode fail*/
      tdsrrcpg1_page_detect_pattern.error_occur = TRUE;

    }
    else /* ASN1 decoding is successful */
    {
      /* Increment the counter for number of Paging Type 1 messages */
      tdsrrcpg1_num_pages++;

       /* Get the OTA message pointer */
      pcch_msg_ptr = (tdsrrc_PCCH_Message *) 
                     cmd_ptr->cmd.downlink_sdu_ind.decoded_msg;  
      

      if (tdsrrc_nv_rel_indicator & TDSRRC_NV_REL_INDICATOR_REL5)
      {  
        if(TDSRRC_MSG_COMMON_BITMASK_IE(pcch_msg_ptr->message.u.pagingType1,
           tdsrrc_PagingType1,laterNonCriticalExtensions))
        {
          page_status = tdsrrcpg1_validate_paging_type_msg_r5(pcch_msg_ptr);
  
          if(TDSRRCPG1_RELEASE == page_status)
          {
            //send cmd to transition to idle
            (void)tdsrrc_transition_to_disconnected_state(TDSRRC_PROCEDURE_PG1, TDSRRC_TX_TO_DISCON_OTHER);
  
            TDSRRC_MSG0(MSG_LEGACY_HIGH,"UE connection being released by UTRAN");
          }
          else if(TDSRRCPG1_SUCCESS == page_status)
          {
            /* Increment the no. of page matches */
            tdsrrcpg1_num_pages_matched++;
            /* This page is not a false alarm */
            false_paging_alarm = FALSE;
            TDSRRC_MSG3(MSG_LEGACY_HIGH,"Paging type1, type:%d, cause:%d, id:%d",
                     tdsrrcpg1_paging_type,
                     tdsrrcpg1_paging_cause,
                     tdsrrcpg1_page_ue_id
                     );
          }
          /* Page in pagingRecord2List is for current UE, treat as error occur.*/
          if((TDSRRCPG1_RELEASE == page_status) || (TDSRRCPG1_SUCCESS == page_status))
          {
              tdsrrcpg1_page_detect_pattern.error_occur = TRUE;
          }

        }
      }

      //if in Rel-5 Extensions we don't have a successful match,  i.e.  SUCCESS/RELEASE
      //try to look into the Rel-99 message
      if((TDSRRCPG1_SUCCESS != page_status) && (TDSRRCPG1_RELEASE != page_status))
      {        
        page_status = tdsrrcpg1_validate_paging_type_msg(pcch_msg_ptr);

        /* Validate the Paging Type 1 message */
        if(TDSRRCPG1_SUCCESS ==  page_status)
        {
          /* Increment the no. of page matches */
          tdsrrcpg1_num_pages_matched++;
          /* This page is not a false alarm */
          false_paging_alarm = FALSE;
          TDSRRC_MSG3(MSG_LEGACY_HIGH,"Paging type1, type:%d, cause:%d, id:%d",
                   tdsrrcpg1_paging_type,
                   tdsrrcpg1_paging_cause,
                   tdsrrcpg1_page_ue_id
                   );
        }
      }
        
      if(TDSRRCPG1_SUCCESS == page_status)
      {
        if (tdsrrcpg1_paging_type != TDSRRCPG1_UTRAN_ORIGINATED_PAGE_TO_CONNECTED_UE)
        {
          if(tdsrrc_do_not_forward_page_nv == FALSE)
          {
            /* Allocate memory for RRC_PAGE_IND command */
            if( (mm_cmd_ptr =  mm_tdsrrc_get_cmd_buf(RRC_PAGE_IND)) != NULL)
            {
              /* Fill in parameters fro RRC_CLOASE_SESSION_IND command */
         
              mm_cmd_ptr->cmd.rrc_page_ind.cn_domain_id = tdsrrcpg1_cn_domain_id;
  
              mm_cmd_ptr->cmd.rrc_page_ind.paging_cause = tdsrrcpg1_paging_cause;
  
              mm_cmd_ptr->cmd.rrc_page_ind.page_ue_id = tdsrrcpg1_page_ue_id;
  
              mm_cmd_ptr->cmd.rrc_page_ind.as_id = tdsrrc_get_as_id();

              TDSRRC_MSG0(MSG_LEGACY_HIGH," Sent RRC_PAGE_IND to the MM");
              /* Sends the command to MM */
              if(TDSCHECK_OPTIONAL_FEATURE_CFA_SUPPORTED)
              {
                tdscfa_mm_put_cmd(mm_cmd_ptr);
              }
              mm_put_cmd(mm_cmd_ptr);
            }
            else
            {
              ERR_FATAL(" Failed allocated memory for MM Command: %d",RRC_PAGE_IND,0,0);
            }
          }
        }
        /* UTRAN originated page. No need to inform upper layers. Initiate cell
          update procedure with cause "paging response" */
        else
        {
          if (TDSRRC_STATE_CELL_PCH == tdsrrc_get_state() || 
              TDSRRC_STATE_URA_PCH == tdsrrc_get_state() )
          {
            /* Build TDSRRC_CELL_UPDATE_INITIATE_REQ command to post to 
               Cell Update */

            /* Allocates the buffer to RRC Internal command */
            if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
            {     
              /* Fill in the command parameters for TDSRRC_CELL_UPDATE_INITIATE_REQ */
              rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_CELL_UPDATE_INITIATE_REQ;

              rrc_cmd_ptr->cmd.initiate_cell_update.cause = TDSRRC_PAGING_RESPONSE;
              rrc_cmd_ptr ->cmd.initiate_cell_update.cu_complete_cnf_required = FALSE;

              rrc_cmd_ptr->cmd.initiate_cell_update.procedure = TDSRRC_PROCEDURE_PG1;

              TDSRRC_MSG0(MSG_LEGACY_HIGH,"Initiate cell update for UTRAN originated page");
              tdsrrc_put_int_cmd(rrc_cmd_ptr);   
            } 
            else
            {
              ERR_FATAL("Memory allocation for the %d comamnd failed",TDSRRC_CELL_UPDATE_INITIATE_REQ,0,0);
            }
          }
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_ERROR,"Invalid paging type %d in rrc state %d",tdsrrcpg1_paging_type,tdsrrc_get_state());
          }
        }
      }
      /* Check whether BCCH Modification Info is present */
      if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(pcch_msg_ptr->message.u.pagingType1,
        bcch_ModificationInfo))
      {
        /* Increment the no. of page matches */
        tdsrrcpg1_num_pages_matched++;
        /* This page is not a false alarm */
        false_paging_alarm = FALSE;
        if ( tdsrrcpg1_bcch_mod_timer_status == FALSE)
        {
          /*Compare the value of IE "MIB value tag" in the IE "BCCH modification info" 
           with the value tag stored for the master information block in SIB data base 
           for serving cell.If both Differ only then process PG1 BCCH Mod I.E other 
           wise UE has current system info so Ignore this Page1 message*/
          if((tdsrrcsibdb_get_serving_cell_mib_value_tag()) != (uint32)
              (pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo.mib_ValueTag)
            )
          {
            /* Set the timer status to TRUE */
            tdsrrcpg1_bcch_mod_timer_status = TRUE;
            /* Check if the modification time is present.*/
            if(TDSRRC_MSG_COMMON_BITMASK_IE_TYPE2(pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo,
                bcch_ModificationTime))
            {
              tdsrrcpg1_process_bcch_mod_info_msg_ie(
                                              (uint8)pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo.mib_ValueTag,
                                              (uint16)pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo.bcch_ModificationTime,
                                              TRUE
                                             );
              TDSRRC_MSG2(MSG_LEGACY_HIGH,"BCCH modification ind in page, time:%d,value_tag:%d",pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo.bcch_ModificationTime,pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo.mib_ValueTag);
            }
            else
            {
              tdsrrcpg1_process_bcch_mod_info_msg_ie(
                                              (uint8)pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo.mib_ValueTag,
                                              TDSRRCPG1_INVALID_BCCH_MOD_TIMER_VAL,
                                              FALSE
                                             );
              TDSRRC_MSG1(MSG_LEGACY_HIGH,"BCCH modification ind in page,Mib value_tag:%d",pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo.mib_ValueTag);
            }
            /* Start the timer */
            tdsrrctmr_start_timer(TDSRRCTMR_IGNORE_BCCH_MOD_TIMER, 
                               TDSRRCPG1_TIMER_TO_IGNORE_BCCH_MOD_INFO);
          }
          else
          {
            TDSRRC_MSG2(MSG_LEGACY_HIGH,"Ignoring PG1 BCCH mod IE since Srv-cell's MIB tage=%d & rexd MIB tage in PG1 = %d",tdsrrcsibdb_get_serving_cell_mib_value_tag(),pcch_msg_ptr->message.u.pagingType1.bcch_ModificationInfo.mib_ValueTag);
          }
        }
        else
        {
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH Mod Info is Ignored. Timer ON");
        }
      }
      TDSRRC_MSG3(MSG_LEGACY_HIGH,"Total Pages: %d, False Alarms: %d, Curr Pg Status: %d",tdsrrcpg1_num_pages,
                 (tdsrrcpg1_num_pages - tdsrrcpg1_num_pages_matched), false_paging_alarm);

    }
    
  }
  else if (cmd_ptr->cmd_hdr.cmd_id == 
            TDSRRC_IGNORE_BCCH_MOD_TIMER_EXPIRED_IND)
  {
    /* Reset the timer status */
    tdsrrcpg1_bcch_mod_timer_status = FALSE;
  }
  else if(cmd_ptr->cmd_hdr.cmd_id == 
            TDSRRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Rexd TDSRRC_BCCH_MODIFICATION_TIMER_EXPIRED_IND");
    /*Post UPDATE_SIBS_CMD to SIB */
    tdsrrcpg1_post_update_sibs_cmd();
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Invalid Event: %d or SDU type: %d is forwarded to PG1 Procedure",cmd_ptr->cmd_hdr.cmd_id,cmd_ptr->cmd.downlink_sdu_ind.dl_sdu_type);
  }
  
} /* tdsrrcpg1_process_paging_type1_msg */
/*===========================================================================

FUNCTION tdsrrcpg1_procedure_event_handler

DESCRIPTION
  This procedure is the event handler for the Paging Type 1 procedure.

  The following events are handled by this procedure:

    TDSRRC_DOWNLINK_SDU_IND Primitive (with PAGING TYPE 1 Message)


DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcpg1_procedure_event_handler
(
  tdsrrc_cmd_type *cmd_ptr                       /* Pointer to RRC command */
)
{
 
  tdsrrc_state_e_type rrc_state = tdsrrc_get_state();

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  if(TDSRRC_CPHY_NUDGE_FOR_SFN_MISS_MATCH_REQ == cmd_ptr->cmd_hdr.cmd_id)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Handle RRC_NUDGE_FOR_SFN_MISS_MATCH_REQ ");
    tdsrrcpg1_ff_modify_sibs_for_sfn_update();
    return;
  }
#endif

  switch (rrc_state)
  {
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"RRC state id : %d",rrc_state);
    case TDSRRC_STATE_DISCONNECTED: 
    case TDSRRC_STATE_CELL_PCH:
    case TDSRRC_STATE_URA_PCH:
    case TDSRRC_STATE_CONNECTING:
      tdsrrcpg1_process_paging_type1_msg(cmd_ptr); 
      break;
 
    case TDSRRC_STATE_CELL_FACH:    
    case TDSRRC_STATE_CELL_DCH:  
      if(TDSRRC_DOWNLINK_SDU_IND == cmd_ptr->cmd_hdr.cmd_id)
      {
        /* These two states are invalid to receive Paging Type 1 message. 
         * But due to race conditions, RRC can process the Paging
           Type 1 after transitioning into CELL_FACH or CELL_DCH states. */
        TDSRRC_MSG1(MSG_LEGACY_HIGH,"Paging Type 1 message is received in invalid state %d",rrc_state); 
      }
      tdsrrcpg1_process_paging_type1_msg(cmd_ptr); 
      break;

    default: 
      /* This should never happen since all the states are covered above */
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"Event is forwarded to PG1 in an invalid State: %d",rrc_state);
      break;
   }  
} /* tdsrrcpg1_procedure_event_handler */
/*===========================================================================

FUNCTION tdsrrcpg1_init_procedure

DESCRIPTION
  This function initializes everything that the Paging Type 1 procedure
  needs to have initialized.

DEPENDENCIES
  None.

RETURN VALUE
  None.

===========================================================================*/

void tdsrrcpg1_init_procedure(void)
{
#ifdef FEATURE_TDSCDMA_MSIM_L_PLUS_L
  sys_modem_as_id_e_type sub_id;
  for (sub_id =0; (sub_id >= SYS_MODEM_AS_ID_1) && (sub_id < (sys_modem_as_id_e_type)TDSCDMA_NUM_SUBS); sub_id++)
  {
    tdsrrc_set_identity_info_to_global_params(sub_id);
    /* Set the TMSI value invalid */
    tdsrrc_tmsi_valid = FALSE;
    /* Set the PTMSI value invalid */
    tdsrrc_ptmsi_valid = FALSE;
    /* Set the IMSI value invalid */
    tdsrrc_imsi_valid = FALSE;
    tdsrrc_set_identity_info_to_related_sub_params(sub_id);
  }
#else
  /* Set the TMSI value invalid */
  tdsrrc_tmsi_valid = FALSE;
  /* Set the PTMSI value invalid */
  tdsrrc_ptmsi_valid = FALSE;
  /* Set the IMSI value invalid */
  tdsrrc_imsi_valid = FALSE;
#endif

  /* Reset the no of pages count */
  tdsrrcpg1_num_pages = 0; 
  /* Reset the count for no of pages that are matched */       
  tdsrrcpg1_num_pages_matched = 0; 
  /* Set the timer status to FALSE */
  tdsrrcpg1_bcch_mod_timer_status = FALSE;
  /*Init BCCH Mod info db here*/
  tdsrrcpg1_bcch_mod_info.bcch_mod_info_db_valid = FALSE;
  tdsrrcpg1_bcch_mod_info.bcch_mod_mib_v_tag = TDSRRCPG1_INVALID_MIB_V_TAG;
  tdsrrcpg1_bcch_mod_info.bcch_mod_timer_value = TDSRRCPG1_INVALID_BCCH_MOD_TIMER_VAL;

  /*Init page detect pattern here*/
  memset(&tdsrrcpg1_page_detect_pattern, 0, sizeof(tdsrrcpg1_page_detect_pattern_type));
  tdsrrcpg1_page_detect_pattern.high_loading_thesh = TDS_PAGE_PATTERN_DETECT_HIGH_LOADING_UE_NUM;
 
} /* tdsrrcpg1_init_procedure */

/*===========================================================================

FUNCTION tdsrrcpg1_post_update_sibs_cmd

DESCRIPTION
  This function is used to post UPDATE_SIBS_CMD
  to SIB procedure

DEPENDENCIES
  This function takes MIB Value tag from BCCH ModInfo DB.
  So it should have valid values before calling this function.
  tdsrrcpg1_set_bcch_mod_info_db() should be called once 
  before calling this function.

RETURN VALUE
  None.

===========================================================================*/
void tdsrrcpg1_post_update_sibs_cmd
(
  void
)
{
  tdsrrc_cmd_type *rrc_cmd_ptr;
  if((tdsrrcpg1_bcch_mod_info.bcch_mod_info_db_valid == TRUE)&&
     (tdsrrc_get_state() != TDSRRC_STATE_CELL_DCH)&&
     (tdsrrcllc_get_ordered_config_status() != TDSOC_SET_FOR_CELL_DCH)&&
     ((tdsrrcsibdb_get_serving_cell_mib_value_tag()) != (uint32)
              tdsrrcpg1_bcch_mod_info.bcch_mod_mib_v_tag)
    )
  {
    /* Build TDSRRC_UPDATE_SIBS_IND command to post to SIB */
    /* Allocates the buffer to RRC Internal command */
    if( (rrc_cmd_ptr =  tdsrrc_get_int_cmd_buf()) != NULL)
    {     
      /* Fill in the command parameters for TDSRRC_UPDATE_SIBS_IND */
      rrc_cmd_ptr->cmd_hdr.cmd_id = TDSRRC_UPDATE_SIBS_IND;
       
      rrc_cmd_ptr->cmd.update_sibs_ind.mib_value_tag = 
      tdsrrcpg1_bcch_mod_info.bcch_mod_mib_v_tag;
      
      rrc_cmd_ptr->cmd.update_sibs_ind.procedure = TDSRRC_PROCEDURE_PG1;
      rrc_cmd_ptr->cmd.update_sibs_ind.cause = TDSRRCSIB_REAL_SIB_MOD;
      
  
      tdsrrc_put_int_cmd(rrc_cmd_ptr);
    } 
    else
    {
      ERR_FATAL("Memory allocation for the %d comamnd failed",TDSRRC_UPDATE_SIBS_IND,0,0);
    }
  }
  /*As we are posting Update_SIBs_Ind to SIB, 
   so invalidate the BCCH Mod info db.*/
  tdsrrcpg1_check_and_abort_bcch_mod_info_processing();
}
/*===========================================================================
FUNCTION tdsrrcpg1_check_for_duplicate_bcch_mod_info

DESCRIPTION
This function checks for duplicate received BCCH Mod Info I.E.
If there is BCCH Mod Info I.E in process & the newly received 
I.E has the same parameter as the one under process then this 
function will Indicate duplicate. Other wise it will return appropriate 
status..

DEPENDENCIES
  None.

RETURN VALUE
  TDSRRCPG1_BCCH_MOD_INFO_NOT_IN_PROCESS = Currently BCCH ModInfo i.e is not processed. 
  TDSRRCPG1_BCCH_MOD_INFO_DUPLICATE =  Currently BCCH ModInfo i.e is being processed & received I.E
                                    is duplicate of the one in progress.
  TDSRRCPG1_BCCH_MOD_INFO_IN_PROCESS =  Currently BCCH ModInfo i.e is being processed.
===========================================================================*/
tdsrrcpg1_bcch_mod_info_status_e_type tdsrrcpg1_check_for_duplicate_bcch_mod_info
(
  uint8 mib_v_tag,  /* MIB value tag in rexd OTA message*/
  uint16 bcch_mod_timer_val  /* BCCH modification timer value*/
)
{
  tdsrrcpg1_bcch_mod_info_status_e_type status = TDSRRCPG1_BCCH_MOD_INFO_NOT_IN_PROCESS;
  /*Check to see if we already in process of handling one BCCH Mod info I.E.
    If no then return then we need to return status as FALSE*/
  if(tdsrrcpg1_bcch_mod_info.bcch_mod_info_db_valid  ==   FALSE)
  {
    status = TDSRRCPG1_BCCH_MOD_INFO_NOT_IN_PROCESS;
  }
  else
  {
    /*We are processing one BCCH Mod Info, So lets check whether
       the newly received BCCH Mod Info I.E is duplicate*/
    if((tdsrrcpg1_bcch_mod_info.bcch_mod_mib_v_tag == mib_v_tag)&&
       (tdsrrcpg1_bcch_mod_info.bcch_mod_timer_value == bcch_mod_timer_val)
      )
    {
      TDSRRC_MSG2(MSG_LEGACY_HIGH,"Duplicate BCCHModInfo rexd,Stored MIBVtag=%d,TimerVal=%d",tdsrrcpg1_bcch_mod_info.bcch_mod_mib_v_tag,tdsrrcpg1_bcch_mod_info.bcch_mod_timer_value);
      status = TDSRRCPG1_BCCH_MOD_INFO_DUPLICATE;
    }
    else if(tdsrrctmr_get_remaining_time(TDSRRC_BCCH_MODIFICATION_TIMER) != (uint64)0)
    {
      status = TDSRRCPG1_BCCH_MOD_INFO_IN_PROCESS;
    }
    else
    {
      status = TDSRRCPG1_BCCH_MOD_INFO_NOT_IN_PROCESS;
      tdsrrcpg1_bcch_mod_info.bcch_mod_info_db_valid = FALSE;
    }
  }
  return status;
}
/*===========================================================================
FUNCTION tdsrrcpg1_process_bcch_mod_info()

DESCRIPTION
This function will process BCCH ModInfo I.E.
It will take care of starting timer based on current
SFN & the SFN where BCCH modifiction will happen.

This will also update BCCH ModInfo DB with the 
MIB value tag & BCCH Mod timer received in OTA
message.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcpg1_process_bcch_mod_info
(
  uint8 mib_v_tag,  /* MIB value tag in rexd OTA message*/
  uint16 bcch_mod_timer  /* BCCH modification timer value*/
)
{
  /*BCCH Modification timer value*/
  uint32 bcch_mod_timer_val;
  /*BCCH Modification time interms of SFN value*/
  uint32 bcch_mod_sfn;
  /*Current SFN value*/
  uint32 current_sfn;
  /*BCCH Modification time present, Save this command 
    & start BCCH Modification timer */

  if(tdsrrcpg1_set_bcch_mod_info_db(
                     mib_v_tag,
                     bcch_mod_timer) == TRUE)
  {
  
    /* Actual value BCCH-ModificationTime = IE value * 8 */
    bcch_mod_sfn = ((uint32)tdsrrcpg1_bcch_mod_info.bcch_mod_timer_value) << 3;
    current_sfn = (uint32)tdsseq_get_sfn();
    bcch_mod_timer_val = (bcch_mod_sfn > current_sfn)?
                         (bcch_mod_sfn - current_sfn):((4095 -current_sfn)+bcch_mod_sfn);
  
    TDSRRC_MSG3(MSG_LEGACY_HIGH,"Current SFN=%d,bcch_mod_sfn=%d,bcch timer val=%d",
                                                                current_sfn,
                                                                bcch_mod_sfn,
                                                                bcch_mod_timer_val);
    if(bcch_mod_timer_val > TDSRRCPG1_MAX_TIME_TO_SET_UP_PCCPCH)
    {
      bcch_mod_timer_val -= TDSRRCPG1_MAX_TIME_TO_SET_UP_PCCPCH;
      /*Since SFN is in terms of 10msec, so multiply bcch_mod_timer_val by 10*/
      bcch_mod_timer_val *= TDSRRCPG1_VALUE_OF_ONE_FRAME;
      tdsrrctmr_start_timer(TDSRRC_BCCH_MODIFICATION_TIMER, 
                                                      bcch_mod_timer_val);
      TDSRRC_MSG1(MSG_LEGACY_HIGH,"BCCH mod Timer Started Timer value:%d",bcch_mod_timer_val);
    }
    else
    {
      /*Post UPDATE_SIBS_CMD to SIB */
      tdsrrcpg1_post_update_sibs_cmd();
    }
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Bad Input parameters MibVtag=%d,BcchTimerVal=%d",mib_v_tag,bcch_mod_timer);
  }
}
/*===========================================================================
FUNCTION tdsrrcpg1_check_and_abort_bcch_mod_info_processing()

DESCRIPTION
This function will check & abort processing of BCCH
ModInfo & also it takes care of stopping this timer 
TDSRRC_BCCH_MODIFICATION_TIMER if its running. 

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcpg1_check_and_abort_bcch_mod_info_processing
(
void
)
{
  boolean abort_status = FALSE;
  /*Check & clean up bcch mod info db*/
  if(tdsrrcpg1_bcch_mod_info.bcch_mod_info_db_valid != FALSE)
  {
    tdsrrcpg1_bcch_mod_info.bcch_mod_info_db_valid = FALSE;
    tdsrrcpg1_bcch_mod_info.bcch_mod_mib_v_tag = TDSRRCPG1_INVALID_MIB_V_TAG;
    tdsrrcpg1_bcch_mod_info.bcch_mod_timer_value = TDSRRCPG1_INVALID_BCCH_MOD_TIMER_VAL;
    abort_status = TRUE;
  }
  /*Check to see if BCCH Mod timer is running.
    If so then stop timer right here*/
  if(tdsrrctmr_get_remaining_time(TDSRRC_BCCH_MODIFICATION_TIMER) != (uint64)0)
  {
    tdsrrctmr_stop_timer(TDSRRC_BCCH_MODIFICATION_TIMER);
    abort_status = TRUE;
  }
  if(abort_status == TRUE)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Clearing BCCH ModInfo which was in progress");
  }
}
/*===========================================================================
FUNCTION tdsrrcpg1_set_bcch_mod_info_db()

DESCRIPTION
This function will validate the input parameters & then update
bcch ModInfo db accordingly.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
boolean tdsrrcpg1_set_bcch_mod_info_db
(
  uint8 mib_v_tag,  /* MIB value tag in rexd OTA message*/
  uint16 bcch_mod_timer  /* BCCH modification timer value*/
)
{
  boolean status = FALSE;

  /*Validate & then update MIBVtag into bcch ModInfo db.
    MIB Value tag ranges from 1 to 8 only*/
  if((mib_v_tag < TDSRRCPG1_INVALID_MIB_V_TAG)&&
     (mib_v_tag != 0))
  {
    tdsrrcpg1_bcch_mod_info.bcch_mod_mib_v_tag  =  mib_v_tag;
    status = TRUE;
  }
  
  /*Validate & then update BCCH Mod Timer Value into bcch ModInfo db*/
  if(bcch_mod_timer < TDSRRCPG1_INVALID_BCCH_MOD_TIMER_VAL)
  {
    tdsrrcpg1_bcch_mod_info.bcch_mod_timer_value = bcch_mod_timer;
    status = TRUE;
  }
  
  /*If either MIBVtag or BCCH Mod Timer Value is updated in 
    bcch ModInfo db.Only then set  bcch ModInfo db as valid.*/
  if(status == TRUE)
  {
    tdsrrcpg1_bcch_mod_info.bcch_mod_info_db_valid = TRUE;
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"BCCH ModInfo DB is set with MibVtag=%d,BcchTimer=%d",mib_v_tag,bcch_mod_timer);
  }
  else
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"BCCH ModInfo DB is NOT set Bcoz MibVtag=%d,BcchTimer=%d",mib_v_tag,bcch_mod_timer);
  }
  return status;
}
/*===========================================================================
FUNCTION tdsrrcpg1_process_bcch_mod_info_msg_ie()

DESCRIPTION
This function will process BCCHModInfo I.E received in 
OTA message.

DEPENDENCIES
  None.

RETURN VALUE
  None
===========================================================================*/
void tdsrrcpg1_process_bcch_mod_info_msg_ie
(
  uint8 mib_v_tag,  /* MIB value tag in rexd OTA message*/
  uint16 bcch_mod_time,  /* BCCH modification timer value*/
  boolean bcch_mod_time_present  /*Boolean to indicate whether BCCH Time present in BCCHModInfo i.e*/
)
{
  if(bcch_mod_time_present == TRUE)
  {
    tdsrrcpg1_bcch_mod_info_status_e_type bcch_mod_info_status;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH ModInfo I.E has BCCHModTime value in it");
    bcch_mod_info_status = tdsrrcpg1_check_for_duplicate_bcch_mod_info(
                                                                     mib_v_tag,
                                                                     bcch_mod_time
                                                                   );
    if(bcch_mod_info_status == TDSRRCPG1_BCCH_MOD_INFO_NOT_IN_PROCESS)
    {
      tdsrrcpg1_process_bcch_mod_info(
                                    mib_v_tag,
                                    bcch_mod_time
                                   );
    }
    else if(bcch_mod_info_status == TDSRRCPG1_BCCH_MOD_INFO_IN_PROCESS)
    {
      tdsrrcpg1_check_and_abort_bcch_mod_info_processing();
      tdsrrcpg1_process_bcch_mod_info(
                                    mib_v_tag,
                                    bcch_mod_time
                                   );
    }
    else
    {
      /*This is a Duplicate BCCH ModInfo, so lets ignore it, no need 
        of a debug message here as we have one in function
        tdsrrcpg1_check_for_duplicate_bcch_mod_info()*/
    }
  }
  else
  {
    /*No BCCH modification time present, 
      So post UPDATE_SIBS_CMD to SIB */
    boolean status =FALSE;
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"BCCH ModInfo I.E Doesn't have BCCHModTime value in it");
    /*Make sure that MIB Value Tag is populated properly
      as tdsrrcpg1_post_update_sibs_cmd() picks Mib Vtag 
      from BCCHModInfo db*/
    status = tdsrrcpg1_set_bcch_mod_info_db(
                                         mib_v_tag,
                                         bcch_mod_time
                                        );
    if(status == TRUE)
    {
      tdsrrcpg1_post_update_sibs_cmd();
    }
  }
}
/*===========================================================================
FUNCTION tdsrrcpg1_return_bcch_mod_info_status()

DESCRIPTION
This function will return Bcch mod info
status 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE = If BCCH ModIfo in progress
  FALSE = If BCCH ModIfo is not in progress
===========================================================================*/
boolean tdsrrcpg1_return_bcch_mod_info_status
(
void
)
{
  boolean status = FALSE;
  /*Check & clean up bcch mod info db*/
  if(tdsrrcpg1_bcch_mod_info.bcch_mod_info_db_valid == TRUE)
  {
    status = TRUE;
  }
  return status;
}
/*===========================================================================

FUNCTION tdsrrcpg1_get_num_pages

DESCRIPTION
  This function returns the value of tdsrrcpg1_num_pages.  This indicates
  the total number of pages received [false alarm + valid pages].
  This function will be called to display counter in QXDM

DEPENDENCIES
  None.

RETURN VALUE
  uint32.

===========================================================================*/
uint32 tdsrrcpg1_get_num_pages(void)
{
  return tdsrrcpg1_num_pages;
}
/*===========================================================================

FUNCTION tdsrrcpg1_get_num_pages_matched

DESCRIPTION
  This function returns the value of tdsrrcpg1_num_pages_matched.  This indicates
  the total number of pages minus the false alarms.
  This function will be called to display counter in QXDM

DEPENDENCIES
  None.

RETURN VALUE
  uint32.

===========================================================================*/
uint32 tdsrrcpg1_get_num_pages_matched(void)
{
  return tdsrrcpg1_num_pages_matched;
}


#ifdef FEATURE_HANDLE_CN_PAGE_IN_CONNECTED_STATE
/*===========================================================================

FUNCTION tdsrrcpg1_set_ignore_cn_page_in_conn_flag

DESCRIPTION
  Set whenever goes into PCH 
  Reset whenever UE is actual paged in PCH
  No need to reset when jump out of PCH

DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/

void tdsrrcpg1_set_ignore_cn_page_in_conn_flag(boolean ignore_state)
{
  tdsrrcpg1_ignore_cn_page_in_conn = ignore_state;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"set the ignore cn paging to: %d,rrc state:%d ",tdsrrcpg1_ignore_cn_page_in_conn,tdsrrc_get_state());
}
#endif

/*===========================================================================

FUNCTION tdsrrcpg1_update_page_pattern

DESCRIPTION
  Update page pattern bitmask 
  set UE of ue_index to "1"
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcpg1_update_page_pattern_bitmask(uint16 ue_index)
{
  uint16 tmp_shift_bit_mask = 0x8000;
  tmp_shift_bit_mask = tmp_shift_bit_mask >> tdsrrcpg1_page_detect_pattern.page_pattern_index;
  tdsrrcpg1_page_detect_pattern.monitored_ue[ue_index].page_pattern |= tmp_shift_bit_mask;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Page Pattern detect: update page pattern bitmask at index: %d, page pattern is 0x%x",ue_index,tdsrrcpg1_page_detect_pattern.monitored_ue[ue_index].page_pattern); 
}
/*===========================================================================

FUNCTION tdsrrcpg1_update_page_pattern

DESCRIPTION
  Append monitored UE to the tail of the array
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
boolean tdsrrcpg1_append_monitored_ue_info
(
  tdsrrc_monitor_ue_id_type ue_id_type, 
  /* Monitored UE IMSI*/
  rrc_imsi_gsm_map_type       *monitor_imsi,
  /* Monitored UE TMSI*/
  tdsrrc_TMSI_GSM_MAP         *monitor_tmsi,
  /* Monitored UE PTMSI*/
  tdsrrc_P_TMSI_GSM_MAP       *monitor_ptmsi,
  /* Monitored UE URNTI*/
  tdsrrc_int_u_rnti_type      monitor_urnti
)
{
  uint8 counter = 0; 
  if (tdsrrcpg1_page_detect_pattern.monitored_num >= TDSRRC_PG_PATTERN_MONITOR_UE_MAX_NUM)
  {
    TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Pattern detect: exceed max monitored ue number"); 
    return FALSE;
  }

  switch (ue_id_type)
  {
      case MONITOR_IMSI:
        tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].ue_id_type = MONITOR_IMSI;
        if(monitor_imsi->num_digits <= RRC_MAX_GSMMAP_IMSI_DIGITS)
        {
          tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].u.monitor_imsi.num_digits = monitor_imsi->num_digits;
          for (counter = 0; counter < monitor_imsi->num_digits; counter++)
          {
            tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].u.monitor_imsi.value[counter] = monitor_imsi->value[counter];
          }
          tdsrrcpg1_update_page_pattern_bitmask(tdsrrcpg1_page_detect_pattern.monitored_num);
          tdsrrcpg1_page_detect_pattern.monitored_num++;
          TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Pattern detect: Record monitored IMSI"); 
        }
        break;
      case MONITOR_TMSI:
        tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].ue_id_type = MONITOR_TMSI;
        for (counter = 0; counter < RRC_MAX_GSMMAP_TMSI_LENGTH; counter++)
        {
          tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].u.monitor_tmsi.value[counter] = monitor_tmsi->data[counter];    
        }
        tdsrrcpg1_update_page_pattern_bitmask(tdsrrcpg1_page_detect_pattern.monitored_num);
        tdsrrcpg1_page_detect_pattern.monitored_num++;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Pattern detect: Record monitored TMSI"); 
        break;
      case MONITOR_PTMSI:
        tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].ue_id_type = MONITOR_PTMSI;
        for (counter = 0; counter < RRC_MAX_GSMMAP_PTMSI_LENGTH; counter++)
        {
          tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].u.monitor_ptmsi.value[counter] = monitor_ptmsi->data[counter];    
        }
        tdsrrcpg1_update_page_pattern_bitmask(tdsrrcpg1_page_detect_pattern.monitored_num);
        tdsrrcpg1_page_detect_pattern.monitored_num++;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Pattern detect: Record monitored PTMSI");  
        break;
      case MONITOR_URNTI:
        tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].ue_id_type = MONITOR_URNTI;
        tdsrrcpg1_page_detect_pattern.monitored_ue[tdsrrcpg1_page_detect_pattern.monitored_num].u.monitor_urnti = monitor_urnti;
        tdsrrcpg1_update_page_pattern_bitmask(tdsrrcpg1_page_detect_pattern.monitored_num);
        tdsrrcpg1_page_detect_pattern.monitored_num++;
        TDSRRC_MSG0(MSG_LEGACY_HIGH,"Page Pattern detect: Record monitored URNTI");  
        break;
      default:
        TDSRRC_MSG3(MSG_LEGACY_ERROR,"Invalid UE ID type for page pattern monitor", 0, 0, 0);  
        break;
    }
    return (TRUE);
}

/*===========================================================================

FUNCTION tdsrrcpg1_update_monitored_ue_info

DESCRIPTION
    Update page pattern for specific UE ID in page type1 message
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
boolean tdsrrcpg1_update_page_pattern
(
  tdsrrc_monitor_ue_id_type ue_id_type, 
  /* Monitored UE IMSI*/
  rrc_imsi_gsm_map_type       *monitor_imsi,
  /* Monitored UE TMSI*/
  tdsrrc_TMSI_GSM_MAP         *monitor_tmsi,
  /* Monitored UE PTMSI*/
  tdsrrc_P_TMSI_GSM_MAP       *monitor_ptmsi,
  /* Monitored UE URNTI*/
  tdsrrc_int_u_rnti_type      monitor_urnti
)
{
  uint16 ue_index = 0;
  boolean result = TRUE;
  for (ue_index = 0; ue_index < tdsrrcpg1_page_detect_pattern.monitored_num; ue_index++)
  {
    if (tdsrrcpg1_page_detect_pattern.monitored_ue[ue_index].ue_id_type == ue_id_type)
    {
      if (ue_id_type == MONITOR_IMSI)
      {
        if((monitor_imsi->num_digits == tdsrrcpg1_page_detect_pattern.monitored_ue[ue_index].u.monitor_imsi.num_digits) &&
           (!memcmp((const char *) monitor_imsi->value,(const char *) tdsrrcpg1_page_detect_pattern.monitored_ue[ue_index].u.monitor_imsi.value,
           (monitor_imsi->num_digits * sizeof (uint8))) 
           )
          )
        {
          break;
        }
      }
      else if (ue_id_type == MONITOR_TMSI)
      {
        if(!memcmp((const char *)(TDSRRC_GET_BITS_PTR_STRING_PTR(monitor_tmsi)),
                    (const char *) tdsrrcpg1_page_detect_pattern.monitored_ue[ue_index].u.monitor_tmsi.value,RRC_MAX_GSMMAP_TMSI_LENGTH) 
          )
        {
          break;
        }
      }
      else if (ue_id_type == MONITOR_PTMSI)
      {
        if(!memcmp((const char *) (TDSRRC_GET_BITS_PTR_STRING_PTR(monitor_ptmsi)),
                    (const char *) tdsrrcpg1_page_detect_pattern.monitored_ue[ue_index].u.monitor_ptmsi.value,RRC_MAX_GSMMAP_PTMSI_LENGTH)
          )
        {
          break;
        }
      }
      else if (ue_id_type == MONITOR_URNTI)
      {
        if (monitor_urnti == tdsrrcpg1_page_detect_pattern.monitored_ue[ue_index].u.monitor_urnti)
        {
          break; 
        }
      }  
    }
  }
  /* If this UE is not appeared before, add it to the tail of the monitored UE array*/
  if (ue_index == tdsrrcpg1_page_detect_pattern.monitored_num)
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"Page Pattern detect: Record new monitored UE ID: %d at index %d",ue_id_type,ue_index); 
    result = tdsrrcpg1_append_monitored_ue_info(ue_id_type, monitor_imsi, monitor_tmsi, monitor_ptmsi, monitor_urnti);
  }
  if (result)
  {
    tdsrrcpg1_update_page_pattern_bitmask(ue_index);
  }
  return (result);
}

/*===========================================================================

FUNCTION tdsrrcpg1_update_monitored_ue_info

DESCRIPTION
  Update monitored ue info during page pattern detection
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
boolean tdsrrcpg1_update_monitored_ue_info
(
  tdsrrc_monitor_ue_id_type ue_id_type, 
  /* Monitored UE IMSI*/
  rrc_imsi_gsm_map_type       *monitor_imsi,
  /* Monitored UE TMSI*/
  tdsrrc_TMSI_GSM_MAP         *monitor_tmsi,
  /* Monitored UE PTMSI*/
  tdsrrc_P_TMSI_GSM_MAP       *monitor_ptmsi,
  /* Monitored UE URNTI*/
  tdsrrc_int_u_rnti_type      monitor_urnti
)
{ 
  boolean result = TRUE;
  if(tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_START)
  {
    result = tdsrrcpg1_append_monitored_ue_info(ue_id_type, monitor_imsi, monitor_tmsi, monitor_ptmsi, monitor_urnti);
  }
  else if (tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_CONTINUE)
  {
    result = tdsrrcpg1_update_page_pattern(ue_id_type, monitor_imsi, monitor_tmsi, monitor_ptmsi, monitor_urnti);
  } 
  return (result);
}

/*===========================================================================

FUNCTION tdsrrcpg1_update_page_cycle_from_pattern

DESCRIPTION
  Get page cycle from page pattern detected.
DEPENDENCIES
  None.

RETURN VALUE
  None

===========================================================================*/
void tdsrrcpg1_update_page_cycle_from_pattern(void)
{
   uint16 tmp_shift_bit_mask = 0x8000;
   uint8 page_cycle= 0;
   uint8 max_cycle = 0;
   uint8 index = 0;
   uint16 i = 0;
   boolean ue_appear = FALSE;
   /* Count all monitored UE's page cycle and find the max page cycle*/
   for ( i= 0; i < tdsrrcpg1_page_detect_pattern.monitored_num; i++)
   {   
     TDSRRC_MSG2(MSG_LEGACY_HIGH,"Page pattern detect: monitored UE %d, page pattern is 0x%x", 
               i, tdsrrcpg1_page_detect_pattern.monitored_ue[i].page_pattern);
     while (index <= tdsrrcpg1_page_detect_pattern.page_pattern_index)
     {
       tmp_shift_bit_mask = (0x8000 >> index);    
       if (tmp_shift_bit_mask & tdsrrcpg1_page_detect_pattern.monitored_ue[i].page_pattern)
       {
         page_cycle ++;
         ue_appear = TRUE;
       }
       else 
       {
         /* If the UE is paged before and then we found it's not paged any more, 
          * treat as page cycle detect complete.  eg, page_pattern = 1011, we will
          * stop page cycle count at index = 1 and page_cycle = 1
          * else, continue to find the first position that the UE is paged
          */
         if (ue_appear == TRUE)
         {
           break;
         }
       }
       index++;
     }
     tdsrrcpg1_page_detect_pattern.monitored_ue[i].page_cycle = page_cycle;
     if (max_cycle < page_cycle)
     {
       max_cycle = page_cycle;
     }
     ue_appear = FALSE;
     page_cycle = 0;
     index = 0;
   }
   i = 0;
   /* If MAX cycle is bigger than 1, check whether there is inconsistent instance occur
    * inconsistent instance: valid page cycle during the detect window of some UE is smaller than max cycle
    */
   if (max_cycle > 1)
   {
     while( i < tdsrrcpg1_page_detect_pattern.monitored_num)
     {
       /* If one UE's page cycle <max cycle and its page pattern look like 1xxx or xxx1, treat it as 
        * page pattern detect window does not cover the whole cycle of this UE;
        * or else, treat it as inconsistent for monitored UE, return page cycle 1;
        */
       if (tdsrrcpg1_page_detect_pattern.monitored_ue[i].page_cycle < max_cycle)
       {
         if(!(((tdsrrcpg1_page_detect_pattern.monitored_ue[i].page_pattern & 0x8000) == 0x8000)
             ||((tdsrrcpg1_page_detect_pattern.monitored_ue[i].page_pattern & (0x8000>>tdsrrcpg1_page_detect_pattern.page_pattern_index)) == (0x8000>>tdsrrcpg1_page_detect_pattern.page_pattern_index))))
         {
           max_cycle = 1;
           /* If inconsistent instance occur during periodic detect, decrease high loading threshold*/
           if ((tdsrrcpg1_page_detect_pattern.detect_cause == PERIODIC_DETECT) && (tdsrrcpg1_page_detect_pattern.high_loading_thesh > 1))
           {
		     TDSRRC_MSG0(MSG_LEGACY_HIGH,"Inconsistent page cycle occur, treat as high loading happen and subtract high_loading_thesh");
             tdsrrcpg1_page_detect_pattern.high_loading_thesh --;
			 TDSRRC_MSG1(MSG_LEGACY_HIGH,"high_loading_thesh become %d",tdsrrcpg1_page_detect_pattern.high_loading_thesh);
           }
           /* Indicate high loading detected*/
           tdsdrxmgr_pch_det_patt_set_paging_load_value(1);
           TDSRRC_MSG0(MSG_LEGACY_HIGH,"High loading detected during page pattern detection");
           break;
         }
       }
	   i++;
     }    
   }
   
   tdsrrcpg1_page_detect_pattern.page_cycle = max_cycle;
   TDSRRC_MSG1(MSG_LEGACY_HIGH,"Page pattern detect: update page cycle is %d", 
               max_cycle);
}

/*===========================================================================

FUNCTION tdsrrcpg1_pg_pattern_detect

DESCRIPTION
  Page pattern detect operation function.
  START, CONTINUE or STOP page detection and return page cycle back.
  return SUCCESS -- page detect operation success;
  return FAILURE -- page detect operation fail;
DEPENDENCIES
  None.

RETURN VALUE
  TRUE/FALSE

===========================================================================*/
boolean tdsrrcpg1_pg_pattern_detect(tdsrrc_pg_pattern_detect_operation_enum l1_req, uint8 *detect_value_ptr,tdsrrc_pg_pattern_cause_enum detect_cause)
{
  uint8 thresh = 0;
  TDSRRC_MSG2(MSG_LEGACY_HIGH,"Page pattern detect: operation is %d, current detect status is %d ", l1_req, tdsrrcpg1_page_detect_pattern.detect_status);
  if (l1_req == PATTERN_DETECT_START)
  {
     thresh = tdsrrcpg1_page_detect_pattern.high_loading_thesh;
     /*Init page detect pattern here*/
	 memset(&tdsrrcpg1_page_detect_pattern, 0, sizeof(tdsrrcpg1_page_detect_pattern_type));
     tdsrrcpg1_page_detect_pattern.high_loading_thesh = thresh;
     tdsrrcpg1_page_detect_pattern.detect_cause = detect_cause;
  }
  else if ((tdsrrcpg1_page_detect_pattern.error_occur == TRUE)
      ||(((l1_req == PATTERN_DETECT_CONTINUE) || (l1_req == PATTERN_DETECT_STOP)) 
        && ((tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_STOP) 
		   ||(tdsrrcpg1_page_detect_pattern.detect_status == PATTERN_DETECT_NONE)))
     )
  {
     /*If operation is "CONTINUE" or "STOP" but no "START" is called before, treat as page pattern detect FAILURE*/
     TDSRRC_MSG3(MSG_LEGACY_ERROR,"Page pattern detect: error occur is %d, operation is %d , detect status is %d", tdsrrcpg1_page_detect_pattern.error_occur, l1_req, tdsrrcpg1_page_detect_pattern.detect_status);
     if (detect_value_ptr != NULL)
     {
       *detect_value_ptr = 1;
     }
     tdsrrcpg1_page_detect_pattern.detect_status = PATTERN_DETECT_NONE;
     tdsrrcpg1_page_detect_pattern.error_occur = FALSE;
     tdsrrcpg1_page_detect_pattern.detect_cause = NORMAL_DETECT;
     return FALSE;
  }
  else if (l1_req == PATTERN_DETECT_CONTINUE)
  {
     tdsrrcpg1_page_detect_pattern.page_pattern_index++;
     if (tdsrrcpg1_page_detect_pattern.page_pattern_index >= TDS_PAGE_PATTERN_DETECT_MAX_WINDOW_SIZE) 
     {
        TDSRRC_MSG0(MSG_LEGACY_ERROR,"Page pattern detect: exceed max value");
        if (detect_value_ptr != NULL)
        {
          *detect_value_ptr = 1;
        }
		tdsrrcpg1_page_detect_pattern.detect_status = l1_req;
        tdsrrcpg1_page_detect_pattern.detect_cause = NORMAL_DETECT;
        return FALSE;
     }
  }
  tdsrrcpg1_page_detect_pattern.detect_status = l1_req;
  if (l1_req == PATTERN_DETECT_STOP)
  {
    tdsrrcpg1_update_page_cycle_from_pattern();
  }
  if (detect_value_ptr != NULL)
  {
    *detect_value_ptr = tdsrrcpg1_page_detect_pattern.page_cycle;
  }
  return TRUE;
}

