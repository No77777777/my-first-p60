#ifndef TDSSRCH_BPLMN_H
#define TDSSRCH_BPLMN_H
/*===========================================================================

   T D S S R C H   B P L M N   S T A T E   R E L A T E D   D E C L A R A T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


REFERENCES
  WCDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2000 to 2009 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/tdscdma.mpss/4.3/l1/src/tdssrchbplmn.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/09/11   weis    initial creation for the feature W to W BPLMN srch

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdsl1rrcif.h"
#include "tfw_sw_intf_measurement.h"

/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

#define TDSL1_BPLMN_MAX_DETECT_CPID_NUM 3


/*--------------------------------------------------------------------------
                          BPLMN SEARCH SUBSTATES

--------------------------------------------------------------------------*/

typedef enum
{
  TDSSRCH_BPLMN_INACTIVE,
  TDSSRCH_BPLMN_FS,
  TDSSRCH_BPLMN_ACQ,
  TDSSRCH_BPLMN_PCCPCH_DECODING,
  TDSSRCH_BPLMN_T2X,
  TDSSRCH_BPLMN_NUM_STATES
}tdssrch_bplmn_substate_enum_type;

extern tdssrch_bplmn_substate_enum_type  tdssrchbplmn_substate ;

typedef struct
{
  uint8 num_cells;
  uint8 cpid[TDSL1_BPLMN_MAX_DETECT_CPID_NUM];
  int16 rscp[TDSL1_BPLMN_MAX_DETECT_CPID_NUM];
}tdssrchbplmn_acq_result_type;


typedef struct
{
  /* ACQ_REQ msg from RRC */
  tdsl1_acq_cmd_type req;

  /* ACQ_CNF msg to RRC */
  tdsl1_acq_cnf_type cnf;

  uint8 current_freq_idx;

  int16 rscp_Th;

  tdssrchbplmn_acq_result_type result[TDSL1_MAX_ACQ_CHANNEL_NUM];

} tdssrchbplmn_acq_type;
extern tdssrchbplmn_acq_type tdssrchbplmn_acq;


/*===========================================================================
FUNCTION     tdssrchbplmn_move_fw_to_trk_state

DESCRIPTION:
  - move TFW state to tracking
  

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================*/
extern void tdssrchbplmn_move_fw_to_trk_state(void);


/*===========================================================================

FUNCTION        SRCHBPLMN_FS_REQ_CMD

DESCRIPTION
  Freq scan for BPLMN searches

DEPENDENCIES

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
extern void tdssrchbplmn_fs_req_cmd(tdsl1_freq_scan_cmd_type *cmd);


/*===========================================================================

FUNCTION tdssrchbplmn_acq_cnf

DESCRIPTION
  This function send ACQ_CNF to RRC 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchbplmn_acq_cnf(uint8 freq_idx);

/*===========================================================================

FUNCTION SRCHBPLMN_ACQ_REQ_CMD

DESCRIPTION
  This function first check if there is any step of neighbor search command
  to be issued following an ASET (HPQ) search done. if yes, the function
  issues the search command to DSP. 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchbplmn_acq_req_cmd(tdsl1_acq_cmd_type *cmd);


/*===========================================================================
FUNCTION     tdssrchbplmn_resume_go_to_sleep

DESCRIPTION
  This function handles the command from RRC that L1 should abort the BPLMN
  search and go to sleep. This command should be ideally received only when 
  the L is actively performing the BPLMN search. This function, retrieves the context
  at which it left PCH_SLEEP and also does clean up functionality of the PCH_BPLMN
  state.


DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchbplmn_resume_go_to_sleep(void);

/*===========================================================================

FUNCTION     tdssrchbplmn_wakeup_cell

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
extern void tdssrchbplmn_wakeup_cell
(
  tdssrch_wakeup_set_struct_type *wakeup_set_ptr,
  boolean ue_wakeup
);

/*===========================================================================
FUNCTION     tdssrchbplmn_suspend_tdscdma_mode

DESCRIPTION
  This function handles the command from RRC that L1 should suspend TDSCDMA mode
  for T2G BPLMN search, TL1 should put TFW to standby mode, and release RF resouce


DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/

extern void tdssrchbplmn_suspend_tdscdma_mode(void);

/*===========================================================================
FUNCTION     tdssrchbplmn_resume_tdscdma_mode

DESCRIPTION
  This function handles the command from RRC that L1 should resume TDSCDMA mode
  This command should be ideally received only when T2G BPLMN search is ongoing
  This function, retrieves the context at which it left PCH_SLEEP 
  and also does clean up functionality of the PCH_BPLMN state.


DEPENDENCIES
  None.

PARAMETERS
  None.

RETURN VALUE

SIDE EFFECTS
  None.

===========================================================================*/

extern void tdssrchbplmn_resume_tdscdma_mode(void);

/*===========================================================================

FUNCTION tdssrchbplmn_check_measure_all_timeout

DESCRIPTION
  None.

DEPENDENCIES
  None.

RETURN VALUE
  boolean.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchbplmn_check_measure_all_timeout(boolean b_stop);


/*===========================================================================
  FUNCTION     : tdssrchbplmn_acq_proc_measure_all_rsp  

  DESCRIPTION: Handle measure_all_rscp_rsp from FW.
  
  DEPENDENCIES:
 
  INPUT PARAMETERS:
    tfw_measure_rscp_rsp_t *rscp_rsp
 
  RETURN VALUE: void

  SIDE EFFECTS: None
===========================================================================*/

extern void tdssrchbplmn_acq_proc_measure_all_rsp(tfw_measure_all_rscp_rsp_t *rscp_rsp);

#endif
