#ifndef TDSSRCHBCH_H
#define TDSSRCHBCH_H
/*===========================================================================

   T D S S R C H    B C H    S T A T E   R E L A T E D   D E C L A R A T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2001 - 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchbch.h#1 $ 
$DateTime: 2019/08/19 10:51:23 $ 
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdssrchi.h"
#include "tdssrchcmd.h"
#include "tdssrchset.h"
#include "tdsl1rrcif.h"
#include "tdsl1m.h"
#include "tdssrchmeas.h"
/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/
/* Default minimum values for cell selection S values */
#define TDSSRCH_SQUAL_DEFAULT_MIN (-32767)
#define TDSSRCH_SRXLEV_DEFAULT_MIN (-32767)


typedef struct
{
  /* flag to indicate if RSCP measurement is needed */
  boolean meas_needed;

  /* Cell Selection frequency requested by RRC */
  uint16   cell_selection_freq;

  /* Cell Selection cell parameter id requested by RRC*/
  uint16   cell_selection_cpi;

  tdssrchset_cell_struct_type_tmp* cell_selection_cell_ptr;

  boolean cphy_cell_selection_recvd ;

}tdssrchbch_global_type;

extern tdssrchbch_global_type tdssrchbch;

/*===========================================================================

                      FUNCTION DECLARATIONS
============================================================================

FUNCTION TDSSRCHBCH_CELL_SEL_RPT

DESCRIPTION
  This function sends a report CPHY_CELL_SELECTION_CNF to RRC on cell
  selection search results.
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdssrchbch_cell_sel_rpt (void);
/*===========================================================================

FUNCTION TDSSRCHBCH_CELL_SELECT_SRCH_DONE

DESCRIPTION
  This function processes list search dump for cell selection in BCH state.
  This function retrieves the search results from the LPQ and computes
  the s_qual and s_rxlev Cell Selection criteria. It then calls a function
  to send the results to RRC.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdssrchbch_cell_select_srch_done(void);

/*===========================================================================

FUNCTION TDSSRCHBCH_PROC_SRCH_DONE

DESCRIPTION
  This function processes the SRCH_DONE local command. 

DEPENDENCIES
  The command is executed in the context of the tdssrchbch_substate and
  the search step.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
/*zy modified codes,it will replace tdssrchbch_proc_srch_done in future*/
void tdssrchbch_proc_srch_done(void);

/*===========================================================================

FUNCTION TDSSRCHBCH_PROC_SRCH_OPS

DESCRIPTION
  This function processes "srch_ops_cmd". 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdssrchbch_proc_srch_ops(tdssrchmeas_ops_parm_struct_type  *cmd_ptr);
/*===========================================================================

FUNCTION     SRCHBCH_CELL_SELECT_SRCH

DESCRIPTION
  This function starts search for Cell Selection. This function will
  send a LPQ search for the cell indicated by the cell selection request.
  The results are ready when the LPQ done isr sends the SRCH_DONE local
  command.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
void tdssrchbch_cell_select_srch (void);

/*===========================================================================

FUNCTION TDSSRCHBCH_CELL_SELECT_EVAL

DESCRIPTION
  This function evaluates S criteria of serving or target intra frequency 
  neighbor cell if reselection is in progress

DEPENDENCIES
  None.

RETURN VALUE
  TRUE, if S criteria passes, FALSE otherwise
  
SIDE EFFECTS
  None

===========================================================================*/
extern boolean tdssrchbch_cell_select_eval(int16 q_rxlevmin, tdssrchset_cell_struct_type_tmp* cell_ptr, int16 max_tx_pwr);

#ifdef FEATURE_TDSCDMA_CELL_SELECTION_INTER_SIB_SLEEP

/*===========================================================================

FUNCTION     TDSSRCHBCH_WAKEUP_CELL

DESCRIPTION
  This function is called by DRX Manager to instruct SRCH to beging tracking
  a cell.  

DEPENDENCIES
  None.

PARAMETERS
  wakeup_set_ptr - Array of cell structure(s) to acquire and track.
  boolean ue_wakeup - TRUE: no any channel is on, UE just is wakeup.
                                  - FALSE: at lease one channel is on, and UE already is awake.

RETURN VALUE
  None.
===========================================================================*/
extern void tdssrchbch_wakeup_cell(tdssrch_wakeup_set_struct_type *wakeup_set_ptr, boolean ue_wakeup);


/*===========================================================================

FUNCTION     TDSSRCHBCH_CHECK_FOR_SLEEP

DESCRIPTION
  This function handles the DRX->SEARCHER 'go to sleep' request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdssrchbch_check_for_sleep(void);

/*===========================================================================

FUNCTION     TDSSRCHBCH_WAIT_ON_SRCH_SLEEP

DESCRIPTION
  This function handles the DRX->SEARCHER 'go to sleep' request.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
extern void tdssrchbch_wait_on_srch_sleep(void);

#endif

#endif /* TDSSRCHBCH_H */
