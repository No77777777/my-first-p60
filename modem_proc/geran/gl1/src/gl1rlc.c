/*****************************************************************************
***
*** TITLE
***
***  GPRS PL1 TO RLC INTERFACE MODULE
***
***
*** DESCRIPTION
***
***  Implements access function to allow RLC to access services provided by L1
***
***
*** EXTERNALIZED FUNCTIONS
***
***  gprs_pl1_rlc_acc_func()
***  xxx
***
***
*** INITIALIZATION AND SEQUENCING REQUIREMENTS
***
***  xxx
***
***
*** Copyright (c) 2001-2015 Qualcomm Technologies, Inc.
*** All Rights Reserved. QUALCOMM Proprietary.
***
*** Export of this technology or software is regulated by the U.S.
*** Government. Diversion contrary to U.S. law prohibited.
***
*****************************************************************************/


/*****************************************************************************
***
***                        EDIT HISTORY FOR MODULE
***
***  This section contains comments describing changes made to the module.
***  Notice that changes are listed in reverse chronological order.
***
*** $Header: //components/rel/geran.mpss/7.4.0/gl1/src/gl1rlc.c#1 $
***
*** when       who     what, where, why
*** --------   ---     ------------------------------------------------------
*** 21/06/13    cs     Major Triton TSTS Syncup
*** 13/06/13    ws     CR494399 Set RLC_PH_DATA_REQ_SIG to correct l1 task
*** 19/11/12    hv     Added DSDA API support for GRLC
*** 26/11/03    ws     Lint cleanup
*** 06/05/01    hv     Created L1-to-RLC access function stub.
***
*****************************************************************************/


/*****************************************************************************
***
***     Included Files
***
*****************************************************************************/

#include "geran_variation.h"
#include "customer.h" /* This include must be first before #ifdef FEATURE_  */
#include "geran_tasks.h"
#ifdef  DEBUG_GSM_GPRS_DS_UNIT_TEST
#error code not present
#endif /* DEBUG_GSM_GPRS_DS_UNIT_TEST */

#include "gprsdef.h"
#include "gprsdef_g.h"
#include "gl1rlccmn.h"
#include "gl1rlc.h"

#ifdef PL1_FW_SIM
#include "pl1_test_harness.h"
#else
#include "task.h" // for l1_tcb to set layer 1 signal
#endif
#include "l1_task.h"
/*****************************************************************************
***
***     Private Constants & Defines
***
*****************************************************************************/



/*****************************************************************************
***
***     Private Typedefs
***
*****************************************************************************/



/*****************************************************************************
***
***     Private Data Declarations
***
*****************************************************************************/


/*****************************************************************************
***
***     Private Function Prototypes
***
*****************************************************************************/


/*****************************************************************************
***
***     Public Data Declarations
***
***     Ensure an external reference is added to the module's header file!
***
*****************************************************************************/


/*****************************************************************************
***
***     Private Macros
***
*****************************************************************************/

/*===========================================================================
===
===  MACRO         xxx
===
===  DESCRIPTION
===
===    xxx.
===
===  DEPENDENCIES
===
===    xxx.
===
===  RETURN VALUE
===
===    xxx.
===
===  SIDE EFFECTS
===
===    xxx.
===
===========================================================================*/


/*****************************************************************************
***
***     Private Functions
***
*****************************************************************************/

/*===========================================================================
===
===  FUNCTION      xxx
===
===  DESCRIPTION
===
===    xxx.
===
===  DEPENDENCIES
===
===    xxx.
===
===  RETURN VALUE
===
===    xxx.
===
===  SIDE EFFECTS
===
===    xxx.
===
===========================================================================*/



/*****************************************************************************
***
***     Public Functions
***
*****************************************************************************/

/*===========================================================================
===
===  FUNCTION      PL1_SEND_PH_DATA_REQ()
===
===  DESCRIPTION
===
===    Access function to allow RLC to send PH_DATA_REQ primitive to PL1.
===    RLC sends this primitive to indicate to PL1 there are data blocks in
===    the RLC UL/PL1 FIFO to be processed. This only occurs at the beginning
===    of a TBF. Thereafter, PL1 polls the FIFO for more data. This is the only
===    primitive to PL1 and it contains no other paramters. As such a message
===    is not required, hence this function only sets a dedicated signal mask
===    in PL1 space, if the mask is not already set.
===
===
===  PARAMETERS
===
===  DEPENDENCIES
===
===  RETURN VALUE
===
===    none
===
===  SIDE EFFECTS
===
===    none
===
===========================================================================*/


void pl1_send_ph_data_req ( gas_id_t gas_id )
{
#ifdef  DEBUG_GSM_GPRS_RLC_TEST
#error code not present
#else /* Not DEBUG_GSM_GPRS_RLC_TEST */

  /* This function sends the PH_DATA_REQ primitive from RLC UL to PL1.
  ** Since this primitive contains no other parameters, instead of sending
  ** a message, it sets a dedicated signal mask in PL1 space.
  */

  /* set signal mask in PL1 space if it is not already set
  */

  /* TODO:
  ** L1 needs to create a new mask and set the appropriate mask according to 
  ** the gas_id passed
  */

#ifdef PL1_FW_SIM
  (void)rex_set_sigs(&tcb_pl1, RLC_PH_DATA_REQ_SIG);
#else
  (void)rex_set_sigs(l1_task_tcb_read(gas_id), RLC_PH_DATA_REQ_SIG);
#endif

#endif /* DEBUG_GSM_GPRS_RLC_TEST */

}


/*===========================================================================
===
===  FUNCTION      xxx
===
===  DESCRIPTION
===
===    xxx.
===
===  DEPENDENCIES
===
===    xxx.
===
===  RETURN VALUE
===
===    xxx.
===
===  SIDE EFFECTS
===
===    xxx.
===
===========================================================================*/


/*** EOF: don't remove! ***/

