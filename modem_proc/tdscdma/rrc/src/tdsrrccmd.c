/*===========================================================================
                          R R C    C O M M A N D S

DESCRIPTION

 This module defines command queues and the necessary
 interface functions for all external and internal
 commands sent to RRC.

 RRC maintains several command queues. These command queues
 are assigned priorities by RRC so that commands from higher priority
 buffers are processed before those from lower priority buffers.

 The priority assignment is not explicit. The RRC dispatcher
 implicitly provides priority by checking each queue
 signal sequentially when an RRC signal is set.

 The following are the command queues maintained by RRC
 (In the order of priority starting from highest) -

 tdsrrc_int_cmd_q - Internal commands
 tdsrrc_l1_cmd_q  - L1 commands
 tdsrrc_mac_cmd_q - MAC commands
 tdsrrc_rlc_cmd_q - RLC commands
 tdsrrc_mm_cmd_q  - MM commands
 tdsrrc_tc_cmd_q  - TC commands
 tdsrrc_rr_cmd_q  - RR commands

EXTERNALIZED FUNCTIONS 

 tdsrrc_get_int_cmd_buf   Get an internal command buffer
 tdsrrc_get_l1_cmd_buf    Get an L1 command buffer
 tdsrrc_get_mac_cmd_buf   Get a MAC command buffer
 rrc_get_rlc_cmd_buf   Get an RLC command buffer
 tdsrrc_get_mm_cmd_buf    Get a MM command buffer
 tdsrrc_get_tc_cmd_buf    Get a TC command buffer
 tdsrrc_get_rr_cmd_buf    Get a RR command buffer
 tdsrrc_get_cm_cmd_buf    Get a CM command buffer
 tdsrrc_put_int_cmd       Put an internal command buffer in tdsrrc_int_cmd_q
 tdsrrc_put_l1_cmd        Put an L1 command buffer in tdsrrc_l1_cmd_q
 tdsrrc_put_mac_cmd       Put a mac command buffer in tdsrrc_mac_cmd_q
 rrc_put_rlc_cmd       Put a mac command buffer in tdsrrc_rlc_cmd_q
 tdsrrc_put_mm_cmd        Put a mm command buffer in tdsrrc_mm_cmd_q
 tdsrrc_put_tc_cmd        Put a TC command buffer in tdsrrc_tc_cmd_q
 tdsrrc_put_rr_cmd        Put a RR command buffer in tdsrrc_rr_cmd_q
 tdsrrc_put_cm_cmd        Put a cm command buffer in tdsrrc_cm_cmd_q
 tdsrrc_free_cmd_buf      Free any RRC command buffer

INITIALIZATION AND SEQUENCING REQUIREMENTS

  Queues must be initialized by calling tdsrrc_init_command_queues()
  at RRC task startup.

Copyright (c) 2013 Qualcomm Technologies, Inc.  All Rights Reserved.
Qualcomm Technologies Proprietary and Confidential.
Export of this technology or software is regulated by the U.S. Government. 
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/rrc/src/tdsrrccmd.c#2 $ $DateTime: 2020/04/28 04:35:54 $ $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/17/12   hx      Add FEATURE_TDSCDMA_MEM_ALLOC_FREE_COUNTER to track L1->RRC 
                   command allocate/free
03/16/12   hx      Merged to VU_MODEM_WCDMA_NIKEL_3_0_MHCI.00.00.00.00.08.04
03/10/11   ttl     Merged with VU_MODEM_WCDMA_RRC_OSYS.01.93.53

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
#include "rex.h"
#include "queue.h"
#include "tdsrrccmd.h"
#include "tdsrrccmd_v.h"
#include "tdsrrccmd_l2.h"
#include "tdsl1rrcif.h"
#include "tdsrrccmdi.h"
#include "tdsrrcsigs.h"
#include "task.h"
#include "tdsrrcdata.h"
#include "tdsrrccommon.h"
#include "tdsrrcasn1util.h"
#include "cfa.h"           /* For CFA Logging */
#include "ms.h"            /* For CFA Logging - MSG_SET IDs */
#include "gs.h"            /* For CFA Logging - QUEUE IDs*/
/*#include "tdsrrcrrif.h"*/
#include "tdsrrc_rr_if.h"
#include "rr_tdsrrc_if.h"
#include "tdsrrcmeas.h"

#if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA)
#include "tdsrrc_rrc_if.h"
#endif

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
#error code not present
#endif
#include "tdsCFAParser.h"

/* -----------------------------------------------------------------------
** Constant / Define Declarations
** ----------------------------------------------------------------------- */


/* -----------------------------------------------------------------------
** Type Declarations
** ----------------------------------------------------------------------- */
#define TDSMAX_CMD_WM 2

/* -----------------------------------------------------------------------
** Global Constant Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Global Data Declarations
** ----------------------------------------------------------------------- */

/* -----------------------------------------------------------------------
** Local Object Definitions
** ----------------------------------------------------------------------- */



/*--------------------------------------------------------*/
/*                                                        */
/*                 RRC COMMAND QUEUES                     */
/*                                                        */
/*  All Command Queues for the RRC Task are defined here  */
/*--------------------------------------------------------*/

/* Queue for internal RRC commands within the RRC task    */
q_type tdsrrc_int_cmd_q;

/* Queue for RRC commands sent by L1 task                 */
q_type tdsrrc_l1_cmd_q;

/* Queue for RRC commands sent by MAC task                */
q_type tdsrrc_mac_cmd_q;

/* Queue for RRC commands sent by RLC task                */
q_type tdsrrc_rlc_cmd_q;

/* Queue for RRC commands sent by MM task                 */
q_type tdsrrc_mm_cmd_q;

/* Queue for RRC commands sent by TC task                 */
q_type tdsrrc_tc_cmd_q;

/* Queue for RRC commands sent by RR task                 */
q_type tdsrrc_rr_cmd_q;

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/* Queue for RRC commands sent by LSM task                */
q_type tdsrrc_lsm_cmd_q;
#endif


/* Queue for RRC commands sent by WCDMA RRC task          */
q_type tdsrrc_rrc_cmd_q;

/*Queue type for RRC cmds from MSGR                       */
q_type tdsrrc_msgr_cmd_q, tdsrrc_msgr_free_cmd_q;
#define TDSRRC_MAX_MSGR_CMDS 3
static tds_rrc_msgr_cmd_type tds_rrc_msgr_cmd_pool[TDSRRC_MAX_MSGR_CMDS];

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/*Queue type for WL1 sending free floating cmds to RRC*/
q_type tdsrrc_l1_ff_cmd_q;
#endif

#ifdef FEATURE_TDSCDMA_MEM_ALLOC_FREE_COUNTER
  uint32 tdsgetL1cmdBufCt = 0;
  uint32 tdsputL1cmdCt = 0;
  uint32 tdsfreeL1cmdCt = 0;
  uint32 tdsputL1MeasIndCt = 0;
  uint32 tdsfreeL1MeasIndCt = 0;
#endif

/* -----------------------------------------------------------------------
** Forward Declarations
** ----------------------------------------------------------------------- */
/* Any forward declarations go here */


/* =======================================================================
**                            Function Definitions
** ======================================================================= */



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
  for example:- function bar() is just below function foo() then function bar() needs to be written like this  
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__
  void bar(void)
  {
    function body here
  }
  __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__
** ----------------------------------------------------------------------- */


/*===========================================================================

FUNCTION tdsrrc_get_pointer_l1_req_value

DESCRIPTION

  This function get pointer for L1 command

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
void tdsrrc_get_pointer_l1_req_value
(
  tdsl1_req_cmd **rrc_out_cmd_ptr,
  tdsl1_req_cmd **rrc_cmd_l1_req
)
{
  *rrc_out_cmd_ptr = (tdsl1_req_cmd *)tdsrrc_malloc(sizeof(tdsl1_req_cmd));
  if(*rrc_out_cmd_ptr  == NULL) {
      ERR_FATAL("tdsl1_req_cmd not received",0,0,0); 
  }
  *rrc_cmd_l1_req = *rrc_out_cmd_ptr;
}

#else /*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
void tdsrrc_get_pointer_l1_req_value
(
  tdsl1_req_cmd *rrc_out_cmd_ptr,
  tdsl1_req_cmd **rrc_cmd_l1_req
)
{
  *rrc_cmd_l1_req = rrc_out_cmd_ptr;
}
#endif  /*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/

/*===========================================================================

FUNCTION tdsrrc_get_pointer_l2_req_value

DESCRIPTION

  This function get pointer for L2 command

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
void tdsrrc_get_pointer_l2_req_value
(
  tdsl2_ul_req_cmd **rrc_out_cmd_ptr,
  tdsl2_ul_req_cmd **rrc_cmd_l2_req
)
{
  *rrc_out_cmd_ptr = (tdsl2_ul_req_cmd *)tdsrrc_malloc(sizeof(tdsl2_ul_req_cmd));
  if(*rrc_out_cmd_ptr  == NULL) {
      ERR_FATAL("l2_req_cmd not received",0,0,0);
  }
  *rrc_cmd_l2_req = *rrc_out_cmd_ptr;
}

#else /*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
void tdsrrc_get_pointer_l2_req_value
(
  tdsl2_ul_req_cmd *rrc_out_cmd_ptr,
  tdsl2_ul_req_cmd **rrc_cmd_l2_req
)
{
  *rrc_cmd_l2_req = rrc_out_cmd_ptr;
}
#endif  /*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/

/*===========================================================================

FUNCTION RRC_INIT_COMMAND_QUEUES

DESCRIPTION

  This function initializes the RRC command queues. This function needs
  to be called once at the time of the RRC task start-up.

DEPENDENCIES

  This function assumes that all REX signals for RRC task have been cleared
  before this function is called.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
 /***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_init_command_queues( void )
{
  uint8 pool_idx = 0;

  /* Initialize all the command queues */
  (void)q_init(&tdsrrc_int_cmd_q);
  (void)q_init(&tdsrrc_l1_cmd_q);
  (void)q_init(&tdsrrc_mac_cmd_q);
  (void)q_init(&tdsrrc_rlc_cmd_q);
  (void)q_init(&tdsrrc_mm_cmd_q);
  (void)q_init(&tdsrrc_tc_cmd_q);
  (void)q_init(&tdsrrc_rr_cmd_q);

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
  (void)q_init(&tdsrrc_lsm_cmd_q);
#endif


#if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA)
  (void)q_init(&tdsrrc_rrc_cmd_q);
#endif

  (void)q_init(&tdsrrc_msgr_cmd_q);
  (void)q_init(&tdsrrc_msgr_free_cmd_q);

  for(pool_idx = 0; pool_idx <TDSRRC_MAX_MSGR_CMDS; pool_idx++)
  {
    tds_rrc_msgr_cmd_type *cmd_ptr = &tds_rrc_msgr_cmd_pool[pool_idx];
    (void) q_link(cmd_ptr, &(cmd_ptr->link));
    q_put(&tdsrrc_msgr_free_cmd_q,&(cmd_ptr->link));
  }

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
  (void)q_init(&tdsrrc_l1_ff_cmd_q);
#endif
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
/*===========================================================================
FUNCTION tdsrrc_get_int_cmd_buf_from_heap_only


DESCRIPTION

  This function returns a internal command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.
 The function allocates memory from heap only and not from static buffer
DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_cmd_type *tdsrrc_get_int_cmd_buf_from_heap_only( void )
{
  tdsrrc_cmd_int_type *ptr;

  /* Allocate from the static buffer, if available first. */

  if( (ptr = (tdsrrc_cmd_int_type *)tdsrrc_malloc(sizeof(tdsrrc_cmd_int_type)))
      == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not allocate memory for RRC Internal cmd q!");
  }

  return (tdsrrc_cmd_type *)ptr;

}

/*===========================================================================
FUNCTION RRC_GET_INT_CMD_BUF


DESCRIPTION

  This function returns a internal command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_cmd_type *tdsrrc_get_int_cmd_buf( void )
{

  tdsrrc_cmd_int_type *ptr;
  /* Allocate from the static buffer, if available first. */


  if( (ptr = tdsrrc_malloc(sizeof(tdsrrc_cmd_int_type)))
      == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not allocate memory for RRC Internal cmd q!");
  }
  return((tdsrrc_cmd_type *)ptr);

}

#else/*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/

/*===========================================================================
FUNCTION tdsrrc_get_int_cmd_buf_from_heap_only


DESCRIPTION

  This function returns a internal command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.
 The function allocates memory from heap only and not from static buffer
DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_cmd_type *tdsrrc_get_int_cmd_buf_from_heap_only( void )
{
  tdsrrc_cmd_type *ptr;

  /* Allocate from the static buffer, if available first. */

  if( (ptr = (tdsrrc_cmd_type *)tdsrrc_malloc(sizeof(tdsrrc_cmd_type)))
      == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not allocate memory for RRC Internal cmd q!");
  }

  return(ptr);

}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================
FUNCTION RRC_GET_INT_CMD_BUF


DESCRIPTION

  This function returns a internal command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_cmd_type *tdsrrc_get_int_cmd_buf( void )
{
  tdsrrc_cmd_type *ptr;

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
   #error code not present
#endif /* #if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST) */

  /* Allocate from the static buffer, if available first. */

  if( (ptr = (tdsrrc_cmd_type *)tdsrrc_malloc(sizeof(tdsrrc_cmd_type)))
      == NULL )
  {
    TDSRRC_MSG0(MSG_LEGACY_ERROR,"Could not allocate memory for RRC Internal cmd q!");
  }

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
   #error code not present
#endif /* #if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST) */
  return(ptr);

}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif/*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/




/*===========================================================================

FUNCTION RRC_GET_L1_CMD_BUF

DESCRIPTION

  This function returns a L1 command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_l1_cmd_type *tdsrrc_get_l1_cmd_buf( void )
{
  tdsrrc_l1_cmd_type *ptr;


  /* Allocate memory */
  if( (ptr = (tdsrrc_l1_cmd_type *)tdsrrc_malloc(sizeof(tdsrrc_l1_cmd_type)))
      == NULL )
  {
    ERR_FATAL("Could not allocate memory for RRC L1 cmd q!",0,0,0);
  }
#ifdef FEATURE_TDSCDMA_MEM_ALLOC_FREE_COUNTER
  tdsgetL1cmdBufCt ++;
#endif
  return(ptr);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/*===========================================================================

FUNCTION rrc_get_l1_ff_cmd_buf

DESCRIPTION

  This function returns a L1 command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling rrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type rrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_l1_cmd_type *tdsrrc_get_l1_ff_cmd_buf( void )
{
  tdsrrc_l1_cmd_type *ptr;

  /* Allocate memory */
  if( (ptr = (tdsrrc_l1_cmd_type *)tdsrrc_malloc(sizeof(tdsrrc_l1_cmd_type)))
      == NULL )
  {
    ERR_FATAL("Could not allocate memory for RRC L1 FF cmd q!",0,0,0);
  }
  return(ptr);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif

/*===========================================================================

FUNCTION RRC_GET_MAC_CMD_BUF

DESCRIPTION

  This function returns a MAC command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_mac_cmd_type *tdsrrc_get_mac_cmd_buf( void )
{
  tdsrrc_mac_cmd_type *ptr;


  /* Allocate memory */
  if( (ptr = (tdsrrc_mac_cmd_type *)tdsrrc_malloc( sizeof(tdsrrc_mac_cmd_type)))
      == NULL )
  {
    ERR_FATAL("Could not allocate memory for RRC MAC cmd q!",0,0,0);
  }
  return(ptr);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/




/*===========================================================================

FUNCTION RRC_GET_RLC_CMD_BUF

DESCRIPTION

  This function returns a RLC command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_rlc_cmd_type *tdsrrc_get_rlc_cmd_buf( void )
{
  tdsrrc_rlc_cmd_type *ptr;


  /* Allocate memory */
  if( (ptr = (tdsrrc_rlc_cmd_type *)tdsrrc_malloc(sizeof(tdsrrc_rlc_cmd_type)))
      == NULL )
  {
    ERR_FATAL("Could not allocate memory for RRC RLC cmd q!",0,0,0);
  }
  return(ptr);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/




/*===========================================================================

FUNCTION RRC_GET_MM_CMD_BUF

DESCRIPTION

  This function returns a MM command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_mm_cmd_type *tdsrrc_get_mm_cmd_buf( void )
{
  tdsrrc_mm_cmd_type *ptr;
  
  /* Allocate memory */
  if( (ptr = (tdsrrc_mm_cmd_type *)tdsrrc_malloc(sizeof(tdsrrc_mm_cmd_type)))
      == NULL )
  {
    ERR_FATAL("Could not allocate memory for RRC MM cmd q!",0,0,0);
  }
  
  return(ptr);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION RRC_GET_TC_CMD_BUF

DESCRIPTION

  This function returns a TC command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_tc_cmd_type *tdsrrc_get_tc_cmd_buf( void )
{
  tdsrrc_tc_cmd_type *ptr;
  
  /* Allocate memory */
  if( (ptr = (tdsrrc_tc_cmd_type *)tdsrrc_malloc( sizeof(tdsrrc_tc_cmd_type)))
	  == NULL )
  {
    ERR_FATAL("Could not allocate memory for RRC TC cmd q!",0,0,0);
  }

  return(ptr);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

#if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA)
/*===========================================================================

FUNCTION TDSRRC_GET_RRC_CMD_BUF

DESCRIPTION

  This function returns a TDSCDMA RRC command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_rrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_rrc_cmd_type * tdsrrc_get_rrc_cmd_buf( void )
{
  tdsrrc_rrc_cmd_type *ptr;
  
  /* Allocate memory */
  if( (ptr = (tdsrrc_rrc_cmd_type *)tdsrrc_malloc(sizeof(tdsrrc_rrc_cmd_type)))
      == NULL )
  {
    ERR_FATAL("Could not allocate memory for WCDMA RRC cmd q!",0,0,0);
  }
  
  return(ptr);

}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif

/*===========================================================================

FUNCTION RRC_GET_RR_CMD_BUF

DESCRIPTION

  This function returns a RR command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_rr_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_rr_cmd_type *tdsrrc_get_rr_cmd_buf( void )
{
  tdsrrc_rr_cmd_type *ptr;


  /* Allocate memory */
  if( (ptr = (tdsrrc_rr_cmd_type *)tdsrrc_malloc( sizeof(tdsrrc_rr_cmd_type)))
      == NULL )
  {
    ERR_FATAL("Could not allocate memory for RRC RR cmd q!",0,0,0);
  }
  return(ptr);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)

/*===========================================================================

FUNCTION RRC_GET_LSM_CMD_BUF

DESCRIPTION

  This function returns a LSM command buffer to the calling function.
  This function dynamically allocates memory for the necessary buffer.
  Note that this buffer MUST be freed by calling tdsrrc_free_cmd_buf().

  The freeing is done by the function that dequeues and processes this
  command buffer.

  Calling function MUST check for a NULL return value and take necessary
  action.

DEPENDENCIES

  None.

RETURN VALUE

  A buffer of type tdsrrc_cmd_type. If memory is not available, NULL
  is returned.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
tdsrrc_cmd_type *tdsrrc_get_lsm_cmd_buf( void )
{
  tdsrrc_cmd_type *ptr;


  /* Allocate memory */
  if( (ptr = (tdsrrc_cmd_type *)tdsrrc_malloc(sizeof(tdsrrc_cmd_type)))
      == NULL )
  {
    ERR_FATAL("Could not allocate memory for RRC LSM cmd q!",0,0,0);
  }
  return(ptr);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif




/*===========================================================================

FUNCTION RRC_PUT_INT_CMD

DESCRIPTION

  This function puts a command buffer into RRC's internal
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_int_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_int_cmd
(
  tdsrrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
   #error code not present
#endif /* #if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST) */

  tds_parse_cfa(TDSCFA_RRC_Q,(void *)cmd_buf);


  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_int_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_int_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in int-q,last cmd:0x%x",q_cnt(&tdsrrc_int_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

#if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST)
   #error code not present
#endif /* #if defined(TEST_FRAMEWORK) && defined(TF_UNIT_TEST) */

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_INT_CMD_Q_SIG);

}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


/*===========================================================================

FUNCTION RRC_PUT_L1_CMD

DESCRIPTION

  This function puts a command buffer into RRC's l1
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_l1_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_l1_cmd
(
  tdsrrc_l1_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
#ifdef FEATURE_TDSCDMA_MEM_ALLOC_FREE_COUNTER
  tdsputL1cmdCt++;
#endif
  
  tds_parse_cfa(TDSCFA_RRC_Q,(void *)cmd_buf);

  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_l1_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_l1_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in l1-q,last cmd:0x%x",q_cnt(&tdsrrc_l1_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_L1_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
/*===========================================================================

FUNCTION tdsrrc_put_l1_ff_cmd

DESCRIPTION

  This function puts a command buffer into RRC's l1 FF
  command queue. Note that the command buffer must have been
  requested through a call to rrc_get_l1_ff_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_l1_ff_cmd
(
  tdsrrc_l1_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
  /* Enable CFA logging when commands are supported by CFA library */
  //cfa_log_packet_ex( GS_QUEUE_RRC,                  /* QUEUE */
  //                   MS_WCDMA_RRC_L1,               /* the message set */
  //                   (byte)cmd_buf->cmd_hdr.cmd_id, /* the message id  */
  //                   255,                           /* first 255 for now */
  //                   (byte*)&(cmd_buf->cmd) );      /* Pointer to the message */

  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_l1_ff_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_l1_ff_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in l1-q,last cmd:0x%x",q_cnt(&tdsrrc_l1_ff_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_L1_FF_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


/*===========================================================================

FUNCTION tdsrrc_put_int_ff_cmd

DESCRIPTION

  This function puts a command buffer into RRC's l1 FF
  command queue, which is a sharing queue for all FF commands. 
  Note that the command buffer must have been requested through 
  a call to tdsrrc_get_int_cmd_buf(). 
  Also, the calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_int_ff_cmd
(
  tdsrrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
  /* Enable CFA logging when commands are supported by CFA library */
  //cfa_log_packet_ex( GS_QUEUE_RRC,                  /* QUEUE */
  //                   MS_WCDMA_RRC_L1,               /* the message set */
  //                   (byte)cmd_buf->cmd_hdr.cmd_id, /* the message id  */
  //                   255,                           /* first 255 for now */
  //                   (byte*)&(cmd_buf->cmd) );      /* Pointer to the message */

  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_l1_ff_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_l1_ff_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in l1-q,last cmd:0x%x",q_cnt(&tdsrrc_l1_ff_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_L1_FF_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


#endif

/*===========================================================================

FUNCTION RRC_PUT_MAC_CMD

DESCRIPTION

  This function puts a command buffer into RRC's mac
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_mac_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_mac_cmd
(
  tdsrrc_mac_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
  /* Enable CFA logging when commands are supported by CFA library */
  
  tds_parse_cfa(TDSCFA_RRC_Q,(void *)cmd_buf);


  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_mac_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_mac_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in mac-q,last cmd:0x%x",q_cnt(&tdsrrc_mac_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_MAC_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION RRC_PUT_RLC_CMD

DESCRIPTION

  This function puts a command buffer into RRC's rlc
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_rlc_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_rlc_cmd
(
  tdsrrc_rlc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
#ifndef TEST_FRAMEWORK
  tds_parse_cfa(TDSCFA_RRC_Q,(void *)cmd_buf);
#endif 
  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_rlc_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_rlc_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in rlc-q,last cmd:0x%x",q_cnt(&tdsrrc_rlc_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_RLC_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION RRC_PUT_MM_CMD

DESCRIPTION

  This function puts a command buffer into RRC's mm
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_mm_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_mm_cmd
(
  tdsrrc_mm_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{


  /* Log the message: first 256 bytes */
  
  tds_parse_cfa(TDSCFA_RRC_Q,(void *)cmd_buf);


  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_mm_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_mm_cmd_q      ))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in mm-q,last cmd:0x%x",q_cnt(&tdsrrc_mm_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_MM_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
/*===========================================================================

FUNCTION RRC_PUT_TC_CMD

DESCRIPTION

  This function puts a command buffer into RRC's mm
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_tc_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_tc_cmd
(
  tdsrrc_tc_cmd_type *cmd_buf	/* The command buffer to be put in the queue */
)
{
  /* Enable CFA logging when commands are supported by CFA library */
  
  tds_parse_cfa(TDSCFA_RRC_Q,(void *)cmd_buf);
  


 
  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_tc_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_tc_cmd_q))
  {
	TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in tc-q,last cmd:0x%x",q_cnt(&tdsrrc_tc_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_TC_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/


#if defined (FEATURE_TDSCDMA_TO_WCDMA) || defined (FEATURE_WCDMA_TO_TDSCDMA)
/*===========================================================================

FUNCTION TDSRRC_PUT_RRC_CMD

DESCRIPTION

  This function puts a command buffer into TDSCDMA RRC's
  command queue. Note that the command buffer must have been
  requested through a call to rrc_get_tdsrrc_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the TDSCDMA RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_rrc_cmd
(
   tdsrrc_rrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
  /* Log the message: first 256 bytes */
  
  tds_parse_cfa(TDSCFA_RRC_Q,(void *)cmd_buf);


  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_rrc_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_rrc_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in WRRC-q,last cmd:0x%x",q_cnt(&tdsrrc_rrc_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(&tds_rrc_tcb, TDSRRC_RRC_CMD_Q_SIG);

}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif

/*===========================================================================

FUNCTION RRC_PUT_RR_CMD

DESCRIPTION

  This function puts a command buffer into RRC's mm
  command queue. Note that the command buffer must have been
  requested through a call to tdsrrc_get_rr_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_rr_cmd
(
  tdsrrc_rr_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
  
  tds_parse_cfa(TDSCFA_RRC_Q,(void *)cmd_buf);

  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_rr_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_rr_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in tc-q,last cmd:0x%x",q_cnt(&tdsrrc_rr_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_RR_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/





#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
/*===========================================================================

FUNCTION tdsrrccmd_free_internal_req_ptr

DESCRIPTION

  This function frees the internal pointers present in the internal commands

DEPENDENCIES


RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrccmd_free_internal_req_ptr
(
  tdsrrc_cmd_type *cmd_buf 
)
{
  if(cmd_buf->cmd_hdr.cmd_id == TDSRRC_LLC_CMD_REQ)
  {
    if(cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER1_PHY)
    {
      if(cmd_buf->cmd.rrc_llc_req.l1_req != NULL )
      {
        tdsrrc_free(cmd_buf->cmd.rrc_llc_req.l1_req);
        cmd_buf->cmd.rrc_llc_req.l1_req = NULL;
      }
    }
    else if((cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_MAC_UL)
      ||(cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_RLC_UL))
    {
      if(cmd_buf->cmd.rrc_llc_req.l2_ul_req !=NULL )
      {
        tdsrrc_free(cmd_buf->cmd.rrc_llc_req.l2_ul_req);
        cmd_buf->cmd.rrc_llc_req.l2_ul_req = NULL;
      }
    }
    else if((cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_RLC_DL)
   	||(cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_MAC_DL))
    {
      if(cmd_buf->cmd.rrc_llc_req.l2_dl_req !=NULL )
      {
        tdsrrc_free(cmd_buf->cmd.rrc_llc_req.l2_dl_req);
        cmd_buf->cmd.rrc_llc_req.l2_dl_req = NULL;
      }
    }
    else
    {
      // Add other commands requiring embedded pointers to be freed here
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"Unknown type of llc cmd re %d",cmd_buf->cmd.rrc_llc_req.cmd_dest);
    }
  }

}
/*===========================================================================

FUNCTION tdsrrc_free_embedd_buf_llc_req

DESCRIPTION

  This function frees the internal pointers present in the internal LLC commands

DEPENDENCIES


RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_free_embedd_buf_llc_req
(
    tdsrrc_cmd_type *cmd_buf
)
{
  if(cmd_buf->cmd_hdr.cmd_id == TDSRRC_LLC_CMD_REQ)
  {
    if(cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER1_PHY)
    {
      if(cmd_buf->cmd.rrc_llc_req.l1_req != NULL)
      {
        if(cmd_buf->cmd.rrc_llc_req.l1_cmd.cmd_id == TDSL1_CPHY_MEASUREMENT_REQ)
        {
          if(cmd_buf->cmd.rrc_llc_req.l1_req->meas.meas_ptr)
          {
            tdsl1_mem_free(  cmd_buf->cmd.rrc_llc_req.l1_req->meas.meas_ptr);
            cmd_buf->cmd.rrc_llc_req.l1_req->meas.meas_ptr = NULL;
          }
        }
#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
        else if(cmd_buf->cmd.rrc_llc_req.l1_cmd.cmd_id == TDSL1_CPHY_CELL_BAR_REQ)
        {
          if(cmd_buf->cmd.rrc_llc_req.l1_req->cell_bar_req.forbidden_earfch_cell_list)
          {
            tdsl1_mem_free(cmd_buf->cmd.rrc_llc_req.l1_req->cell_bar_req.forbidden_earfch_cell_list);
            cmd_buf->cmd.rrc_llc_req.l1_req->cell_bar_req.forbidden_earfch_cell_list = NULL;
          }
        }
#endif        
        /*Free embedded L1 ptr */
        tdsrrc_free(cmd_buf->cmd.rrc_llc_req.l1_req);
        cmd_buf->cmd.rrc_llc_req.l1_req = NULL;
      }
    }
    else if((cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_MAC_UL)
        ||(cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_RLC_UL))
    {
      if(cmd_buf->cmd.rrc_llc_req.l2_ul_req !=NULL)
      {
        if(cmd_buf->cmd.rrc_llc_req.l2_ul_cmd.cmd_id == TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ)
        {
           if(cmd_buf->cmd.rrc_llc_req.l2_ul_req->mac_ul_traffic_vol_config)
           {
             tdsmac_ul_free_mem_ext_cmd(cmd_buf->cmd.rrc_llc_req.l2_ul_req->mac_ul_traffic_vol_config);
             cmd_buf->cmd.rrc_llc_req.l2_ul_req->mac_ul_traffic_vol_config = NULL; 
           }
        }
        /*Free embedded L2 Ul ptr */
        tdsrrc_free(cmd_buf->cmd.rrc_llc_req.l2_ul_req);
        cmd_buf->cmd.rrc_llc_req.l2_ul_req = NULL; 
      }
    }
    else if((cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_RLC_DL)
        ||(cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_MAC_DL))
    {
      if(cmd_buf->cmd.rrc_llc_req.l2_dl_req !=NULL )
      {
        /*Free embedded L2 Dl ptr */
        tdsrrc_free(cmd_buf->cmd.rrc_llc_req.l2_dl_req);
        cmd_buf->cmd.rrc_llc_req.l2_dl_req = NULL;
      }
    }
    else
    {
      // Add other commands requiring embedded pointers to be freed here
    }
  }
  
}
#else/*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
/*===========================================================================

FUNCTION tdsrrc_free_embedd_buf_llc_req

DESCRIPTION

  This function frees the internal pointers present in the internal LLC commands

DEPENDENCIES


RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
void tdsrrc_free_embedd_buf_llc_req
(
	tdsrrc_cmd_type *cmd_buf
)
{
  if(cmd_buf->cmd_hdr.cmd_id == TDSRRC_LLC_CMD_REQ)
  {
    if(cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER1_PHY)
    {
      if(cmd_buf->cmd.rrc_llc_req.l1_cmd.cmd_id == TDSL1_CPHY_MEASUREMENT_REQ)
      {
        if(cmd_buf->cmd.rrc_llc_req.l1_req.meas.meas_ptr)
        {
          tdsl1_mem_free(cmd_buf->cmd.rrc_llc_req.l1_req.meas.meas_ptr);
          cmd_buf->cmd.rrc_llc_req.l1_req.meas.meas_ptr = NULL;
        }
      }
#ifdef FEATURE_TDSCDMA_TO_LTE_PING_PONG_REDIR_OPT
      else if(cmd_buf->cmd.rrc_llc_req.l1_cmd.cmd_id == TDSL1_CPHY_CELL_BAR_REQ)
      {
        if(cmd_buf->cmd.rrc_llc_req.l1_req.cell_bar_req.forbidden_earfch_cell_list)
        {
          tdsl1_mem_free(cmd_buf->cmd.rrc_llc_req.l1_req.cell_bar_req.forbidden_earfch_cell_list);
          cmd_buf->cmd.rrc_llc_req.l1_req.cell_bar_req.forbidden_earfch_cell_list = NULL;
        }
      }
#endif 
    }
    else if(cmd_buf->cmd.rrc_llc_req.cmd_dest == TDSLAYER2_MAC_UL)
    {
      if(cmd_buf->cmd.rrc_llc_req.l2_ul_cmd.cmd_id == TDSCMAC_UL_TRAFFIC_VOLUME_CONFIG_REQ)
      {
        if(cmd_buf->cmd.rrc_llc_req.l2_ul_req.mac_ul_traffic_vol_config)
        {
          tdsmac_ul_free_mem_ext_cmd( cmd_buf->cmd.rrc_llc_req.l2_ul_req.mac_ul_traffic_vol_config);
          cmd_buf->cmd.rrc_llc_req.l2_ul_req.mac_ul_traffic_vol_config = NULL;
        }
      }
    }
    else
    {
      // Add other commands requiring embedded pointers to be freed here
    }
  }
}

#endif/*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/


/*===========================================================================

FUNCTION RRC_FREE_CMD_BUF

DESCRIPTION

  This function frees a command buffer removed from any RRC
  command queue. Only commands removed from RRC's command queues
  can be passed into this function.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_free_cmd_buf
(
  tdsrrc_cmd_type *cmd_buf    /* Command Buffer                               */
)
{



  /* Check here if command id TDSRRC_DOWNLINK_SDU_IND. If so, then
     deallocated DL SDU before freeing command buffer. */
  switch(cmd_buf->cmd_hdr.cmd_id)
  {
    case TDSRRC_DOWNLINK_SDU_IND:

      if(cmd_buf->cmd.downlink_sdu_ind.dl_sdu != NULL)
      {
        //Free the memory allocated for the received downlink SDU.
        tdsrrc_free( cmd_buf->cmd.downlink_sdu_ind.dl_sdu );
      }

      if( cmd_buf->cmd.downlink_sdu_ind.decoded_msg != NULL)
      {

        // deallocate cmd_ptr->cmd.downlink_sdu_ind.decoded_msg here.
        if(tdsrrcasn1_free_pdu((int)cmd_buf->cmd.downlink_sdu_ind.dl_sdu_type ,
                            cmd_buf->cmd.downlink_sdu_ind.decoded_msg))
        {
          // This is a memory leak and it should never happen.
          ERR_FATAL("Memory freeing is unsuccessful",0,0,0);
        }
      }
      break;
#ifdef FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE
    case TDSRRC_LLC_CMD_REQ:
      tdsrrccmd_free_internal_req_ptr(cmd_buf);
      break;
#endif/*FEATURE_TDSCDMA_RRC_REDUCE_CMD_SIZE*/
    case TDSRRC_OPEN_SESSION_REQ:
      tdsrrc_free_embedded_buf(cmd_buf, cmd_buf->cmd.open_sess_req.nas_msg.value);
      break;

    case TDSRRC_DATA_REQ:
      tdsrrc_free_embedded_buf(cmd_buf, cmd_buf->cmd.data_req.nas_msg.value);
      break;

    case TDSRRC_CPHY_MEASUREMENT_IND:
      tdsrrc_free_embedded_buf(cmd_buf, cmd_buf->cmd.meas_ind.meas_rpt);
      break;

#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
    case TDSRRC_MEASUREMENT_IND:
      tdsrrc_free_embedded_buf(cmd_buf, (void *)cmd_buf->cmd.meas_rpt_ind.meas_rpt);
      break;
#endif

#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
#ifdef FEATURE_TDSCDMA_PLI_NMR_UTRAN
    case TDSRRC_CPHY_UTRAN_NMR_INFO_CNF:
      tdsrrc_free_embedded_buf(cmd_buf, (void *)cmd_buf->cmd.nmr_cnf.l1_meas_utran_nmr_info);
      break;
#endif /*FEATURE_TDSCDMA_PLI_NMR_UTRAN*/
     case TDSRRC_CPHY_GET_IMM_MEAS_RSP:
       tdsrrc_free_embedded_buf(cmd_buf, (void *)cmd_buf->cmd.imm_meas_cnf.immed_meas);
       break; 
#ifdef FEATURE_TRITON_MODEM
#else	
#ifdef FEATURE_TDSCDMA_MODEM_STATISTICS
     case TDSRRC_CPHY_MODEM_STATISTICS_RSP:
       tdsrrc_free_embedded_buf(cmd_buf, (void *)cmd_buf->cmd.modem_stat_rsp.l1_serving_cell_power_info);
       tdsrrc_free_embedded_buf(cmd_buf, (void *)cmd_buf->cmd.modem_stat_rsp.l1_active_set_info);
       tdsrrc_free_embedded_buf(cmd_buf, (void *)cmd_buf->cmd.modem_stat_rsp.l1_monitored_set_info);
      break;
#endif /*FEATURE_TDSCDMA_MODEM_STATISTICS*/
#endif /*FEATURE_TRITON_MODEM */
#endif /*FEATURE_TDSCDMA_FREE_FLOATING_TASK*/

    default:
      // Do nothing
      break;
  }
  tdsrrc_free( cmd_buf );
} /* tdsrrc_free_cmd_buf */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/



/*===========================================================================

FUNCTION RRC_FREE_MM_CMD_BUF

DESCRIPTION

  This function frees a MM command buffer Memory

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_free_mm_cmd_buf
(
  tdsrrc_mm_cmd_type  *cmd_buf    /* Command Buffer */
)
{
  tdsrrc_free( cmd_buf );
} /* end of tdsrrc_free_mm_cmd_buf */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION RRC_FREE_TC_CMD_BUF

DESCRIPTION

  This function frees a RR command buffer removed from RRC's TC
  command queue. Only commands removed from RRC's TC command queue
  can be passed into this function.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_free_tc_cmd_buf
(
  tdsrrc_tc_cmd_type *cmd_buf    /* Command Buffer */
)
{
  tdsrrc_free( cmd_buf );
} /* end of tdsrrc_free_tc_cmd_buf */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

/*===========================================================================

FUNCTION RRC_FREE_RR_CMD_BUF

DESCRIPTION

  This function frees a RR command buffer removed from RRC's RR
  command queue. Only commands removed from RRC's RR command queue
  can be passed into this function.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  None.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_free_rr_cmd_buf
(
  tdsrrc_rr_cmd_type *cmd_buf    /* Command Buffer */
)
{
  tdsrrc_free( cmd_buf );
} /* end of tdsrrc_free_rr_cmd_buf */
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/




#if defined (FEATURE_CGPS_UMTS_CP_TDSCDMA)
/*===========================================================================

FUNCTION RRC_PUT_LSM_CMD

DESCRIPTION

  This function puts a command buffer into RRC's LSM
  command queue. Note that the command buffer must have been
  requested through a call to get_rrc_lsm_cmd_buf(). Also,
  The calling function must have filled all the necessary
  data including the command id before passing the buffer
  to this function.

  This function also sets the appropriate signal to the RRC
  task to indicate that a command has been placed in the
  command queue.

DEPENDENCIES

  None.

RETURN VALUE

  None.

SIDE EFFECTS

  A signal is set for the RRC task and this could cause a context
  switch.

===========================================================================*/
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_put_lsm_cmd
(
  tdsrrc_cmd_type *cmd_buf  /* The command buffer to be put in the queue */
)
{
  /* Enable CFA logging when commands are supported by CFA library */
  //cfa_log_packet_ex( GS_QUEUE_RRC,              /* QUEUE */
  //  MS_LSM_RRC,                                 /* the message set */
  //  (byte)cmd_buf->cmd_hdr.cmd_id,              /* the message id  */
  //  255,                                        /* first 255 for now */
  //  (byte*)&(cmd_buf->cmd) );                   /* Pointer to the message */


  /* First initialize the queue link. This is always done
  before putting an item in a queue. */
  (void) q_link(cmd_buf, &(cmd_buf->cmd_hdr.link));

  /* Then put the buffer in the queue */
  q_put(&tdsrrc_lsm_cmd_q, &(cmd_buf->cmd_hdr.link));

  /* if number of commands in que has already reached wm indicate a warning */
  if (TDSMAX_CMD_WM < q_cnt(&tdsrrc_lsm_cmd_q))
  {
    TDSRRC_MSG2(MSG_LEGACY_HIGH,"%d cmds in lsm-q,last cmd:0x%x",q_cnt(&tdsrrc_lsm_cmd_q),cmd_buf->cmd_hdr.cmd_id);
  }

  /* Then set the signal for this queue */
  (void) rex_set_sigs(tds_rrc_get_tcb(), TDSRRC_LSM_CMD_Q_SIG);
}
/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/
#endif

/*====================================================================
FUNCTION: tdsrrc_free_embedded_buf()

DESCRIPTION:
  This function frees an embedded pointer (if any) from the command,
  queue.  Embedded memory allocations are not freed by tdsrrc_free_cmd_buf.
  Only the memory allocated for tdsrrc_cmd_type is freed.  This is needed
  to free memory allocations in TDSL1_CPHY_MEASUREMENT_REQ and TVM_REQ from
  RRC to L1 and RRC to MAC, when they are not routed through LLC.  If
  these commands are routed to LLC, LLC takes care of sending them to
  L1/MAC and deleting them if they are not routed and discarded.
  This function also frees memory allocated for the NAS Payload and
  measurement ind

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  None.

=====================================================================*/
 /***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED__ /***/
void tdsrrc_free_embedded_buf
(
  tdsrrc_cmd_type *cmd_buf,    /* Command Buffer */
  void         *ptr         /* ptr to the payload that needs to be freed */
)
{
  switch(cmd_buf->cmd_hdr.cmd_id)
  {
    case TDSRRC_LLC_CMD_REQ:
      tdsrrc_free_embedd_buf_llc_req(cmd_buf);
      break;
      
    case TDSRRC_OPEN_SESSION_REQ:
    case TDSRRC_DATA_REQ:
    case TDSRRC_CPHY_MEASUREMENT_IND:
#ifdef FEATURE_TDSCDMA_FREE_FLOATING_TASK
    case TDSRRC_CPHY_UTRAN_NMR_INFO_CNF:
    case TDSRRC_CPHY_GET_IMM_MEAS_RSP:
    case TDSRRC_CPHY_MODEM_STATISTICS_RSP:
#endif

      if(cmd_buf->cmd_hdr.cmd_id == TDSRRC_CPHY_MEASUREMENT_IND)
      {
#ifdef FEATURE_TDSCDMA_MEM_ALLOC_FREE_COUNTER
        tdsfreeL1MeasIndCt++; 
#endif
        tdsrrcifreq_free_memory_allocated_for_cell_list_in_meas_ind(cmd_buf);
        tdsrrc_free_for_external_cmd(ptr);
      }
      else
      {
        tdsrrc_free_for_external_cmd(ptr);
      }
      break;

    case TDSRRC_MEASUREMENT_IND:
      /* ZZTBD: verify where this command is used */
      TDSRRC_MSG0(MSG_LEGACY_ERROR,"ERR: handling of free TDSRRC_MEASUREMENT_IND not supported");
      break;
      
    default:
      TDSRRC_MSG1(MSG_LEGACY_ERROR,"ERR: handling of cmd 0x%x not supported",cmd_buf->cmd_hdr.cmd_id);
      break;
  }
} /* tdsrrc_free_embedded_buf */

/***/ __TDS_RRC_CODE_SEGMENT_NON_DEMANDPAGED_END__ /***/

