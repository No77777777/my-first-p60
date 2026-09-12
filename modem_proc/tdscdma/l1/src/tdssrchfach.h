#ifndef SRCHFACH_H
#define SRCHFACH_H
/*===========================================================================
S R C H    F A C H    S T A T E   R E L A T E D   D E C L A R A T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS


INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS


REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2001-2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "tdscdma_variation.h"
#include "comdef.h"
#include "tdssrchcmd.h"
#include "tdssrchset.h"
#include "tdsl1rrcif.h"
#include "tdsl1m.h"
#include "tdsmathutil.h"

#include "tdssrchmeas.h"

#include "tdssrchgsmdrv.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                          FACH SEARCH SUBSTATES
--------------------------------------------------------------------------*/
/* FACH search substate */
typedef enum
{
  TDSSRCHFACH_INIT,           /* Initialization */
  TDSSRCHFACH_INACTIVE,       /* No search activity */
  TDSSRCHFACH_MAIN,              /* main state */
  TDSSRCHFACH_N,      /* neighbor cell  search */  
} tdssrchfach_substate_enum_type;

extern tdssrchfach_substate_enum_type  tdssrchfach_substate;

#ifdef FEATURE_TDSCDMA_CELL_FACH_MEAS_OCCASION

/*--------------------------------------------------------------------------
                          FACH GSM measurements
--------------------------------------------------------------------------*/

/* Max number of GSM cells for BSIC ID or Reconf */
#define TDSSRCHFACH_NUM_GSM_BSIC_CELLS 6

/* Whether need to wait for FACH GSM meas cleanup to finish */
extern boolean tdssrchfach_gsm_meas_cleanup_pending;

#endif /* FEATURE_TDSCDMA_CELL_FACH_MEAS_OCCASION */

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/
/*===========================================================================

FUNCTION SRCH_FACH_INIT

DESCRIPTION
  This function performs search module FACH initializations. It is called
  by L1 manager when L1 enters the "FACH" state. 
  
  For now, search operation in FACH is similar to that in BCH/PCH with
  the following exceptions:
  
  1. Inter-frequency neihgbor cell search will be scheduled differently.
     In FACH, inter-frequency neighbor searches are scheduled based on 
     "FACH occasion" conveyed by the base station. (This item will be
     implemented in the future. Currently, we don't do inter-frequency 
     neighbor search in PCH either.)
     
  2. In FACH, SRCH module processes "TDSL1_CPHY_IMMEDIATE_MEAS_REQ" commands,
     and responds, with a report to the command immediately based on 
     available measurement results on intra-frequency neighbor cells.
     This is implemented as a function call.
     
  3. Search rates/parameters may need to be adjusted. (TBD.)     
     

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchfach_init( void );
/*===========================================================================
FUNCTION        TDSSRCHFACH_PROCESS_IMMEDIATE_MEAS_REQ

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

RETURN VALUE    
    void.

SIDE EFFECTS    
===========================================================================*/
extern void tdssrchfach_process_immediate_meas_req
(
  tdsl1_immediate_meas_type_tmp *meas,
  tdsl1_intra_freq_rach_rpt_quan_struct_type *meas_quan
);

/*===========================================================================

FUNCTION SRCHFACH_PROC_SRCH_OPS

DESCRIPTION
  This function processes "srch_ops_cmd". 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
extern void tdssrchfach_proc_srch_ops(tdssrchcmd_ops_cmd_type  *cmd_ptr);

/*===========================================================================

FUNCTION      SRCHFACH_PROC_RSCP_DONE

DESCRIPTION
  This function processes the SRCH_DONE local command. 

DEPENDENCIES
  The command is executed in the context of the tdssrchfach_substate and
  the search step.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

===========================================================================*/
/*it will replace tdssrchfach_proc_srch_done*/
void tdssrchfach_proc_rscp_done(uint8 message_id);

/*===========================================================================
FUNCTION        SRCH_FACH_CLEANUP

FILE NAME       

DESCRIPTION     
  This function performs search module FACH cleanup. It is called by L1 manager 
  when L1 exits the FACH state. 

DEPENDENCIES    

RETURN VALUE    
  Boolean value TRUE.

SIDE EFFECTS    
===========================================================================*/
extern boolean tdssrchfach_cleanup( tdsl1m_state_enum_type next_state );

/*================================================================
FUNCTION tdssrchfach_proc_srch_ops_tmp

DESCRIPTION
  This function processes "srch_ops_cmd". 

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.

================================================================*/
extern void tdssrchfach_proc_srch_ops_tmp(tdssrchmeas_ops_parm_struct_type *ops_ptr);


#ifdef FEATURE_TDSCDMA_CELL_FACH_MEAS_OCCASION

#if 0
/*===========================================================================
FUNCTION        SRCHFACH_MEAS_N_INTRA_PARMS_UPDATE

FILE NAME       

DESCRIPTION     update non-intra-F measurement parameters for FACH

DEPENDENCIES     

RETURN VALUE    
  void

SIDE EFFECTS    FACH GSM meas will be started
===========================================================================*/

void tdssrchfach_meas_n_intra_parms_update_temp
( 
  tdsl1_meas_sib_parm_struct_type *parms_ptr
);

/*===========================================================================
FUNCTION        SRCHFACH_MEAS_TRANS_PARMS_UPDATE

FILE NAME       

DESCRIPTION     update non-intra-F measurement parameters for FACH

DEPENDENCIES    

RETURN VALUE    void

SIDE EFFECTS
    
===========================================================================*/
void tdssrchfach_meas_trans_parms_update_temp
( 
  tdsl1_meas_trans_type *parms_ptr
);

/*===========================================================================
FUNCTION        SRCHFACH_MEAS_N_INTRA_INIT

FILE NAME       

DESCRIPTION     Initialize CELL_FACH non-intra-F cell reselection

DEPENDENCIES    

RETURN VALUE    void
  

SIDE EFFECTS    
===========================================================================*/
void tdssrchfach_meas_n_intra_init( void );

/*===========================================================================
FUNCTION        SRCHFACH_MEAS_N_INTRA_CLEANUP

FILE NAME       

DESCRIPTION     Cleanup CELL_FACH non-intra-F cell reselection

DEPENDENCIES    

RETURN VALUE    FALSE if cannot finish cleanup immediatetly  

SIDE EFFECTS    
===========================================================================*/
boolean tdssrchfach_meas_n_intra_cleanup( boolean save_fach_oksn_info );


/*===========================================================================

FUNCTION        SRCHFACH_GSM_MEAS_CLEANUP

DESCRIPTION     Cleanup FACH GSM cell reselection measurements 

DEPENDENCIES    None

RETURN VALUE    FALSE if cannot finish immediately
  
SIDE EFFECTS    None

===========================================================================*/
boolean tdssrchfach_gsm_meas_cleanup( void );
/*===========================================================================

FUNCTION      SRCHFACH_GSM_PROCESS_RSSI_MEAS_RESULT

DESCRIPTION
  This function will update the RSSI results for all the GSM Cells for
  which the RSSI measurements were made and then sort the GSM cells
  according to the averaged RSSI.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void tdssrchfach_gsm_process_rssi_meas_result( void );

/*===========================================================================

FUNCTION        SRCHFACH_SCCPCH_EVT_HANDLER

DESCRIPTION     ISR for CELL_FACH SCCPCH frame boundary event for inter-RAT 
                and inter-F measurments at measurement occasions 

DEPENDENCIES    Only to be enabled in CELL_FACH state

RETURN VALUE    None
  
SIDE EFFECTS    None

===========================================================================*/
extern void tdssrchfach_sccpch_evt_handler( void );

/*===========================================================================

FUNCTION        SRCHFACH_DISABLE_SCCPCH_EVT

DESCRIPTION     In CELL_FACH state, this function disables
                SEQ_SRCH_FACH_SCCPCH_EVENT and resets SCCPCH SFN

DEPENDENCIES    Valid only in CELL_FACH state

RETURN VALUE    None
  
SIDE EFFECTS    As said above

===========================================================================*/
void tdssrchfach_disable_sccpch_evt( void );
#endif

/*===========================================================================
FUNCTION        SRCHFACH_WAIT_FOR_SIB_MOD_UPDATE

DESCRIPTION
  This function is called to clean up FACH measurement occasion and
  ongoing searches during SIB modification updates.

DEPENDENCIES
  None

RETURN VALUE
  TRUE if caller needs to wait to apply SIB mod changes,
  FALSE otherwise

SIDE EFFECTS
  FACH measurement occasions and searches are cleared

===========================================================================*/
boolean tdssrchfach_wait_for_sib_mod_update(void);

/*===========================================================================

FUNCTION        SRCHFACH_SUSPEND_INTERS_MEAS_FOR_BCH

DESCRIPTION     This function suspends GSM and inter-F meas in FACH state
                through FACH meas reservation manager for BCH setup and
                decoding.

DEPENDENCIES    All variables related to FACH reservation manager must be
                initialzed before.

RETURN VALUE    None
  
SIDE EFFECTS    sets tdssrchfach_inters_meas_suspended to TRUE
                
===========================================================================*/
extern void tdssrchfach_suspend_inters_meas_for_bch(void);

/*===========================================================================

FUNCTION        SRCHFACH_RESUME_INTERS_MEAS_FOR_BCH

DESCRIPTION     This function resumes GSM and inter-F meas in FACH state
                through FACH meas reservation manager for BCH setup and
                decoding.

DEPENDENCIES    All variables related to FACH reservation manager must be
                initialzed before.

RETURN VALUE    None
  
SIDE EFFECTS    sets tdssrchfach_inters_meas_suspended to FALSE
                
===========================================================================*/
extern void tdssrchfach_resume_inters_meas_for_bch(void);



/*===========================================================================
FUNCTION        SRCHFACH_CM_INIT

DESCRIPTION
  This function initializes the search CM driver used for inter-frequency
  measurements during FACH measurement occasions. This function is called
  after we get a new inter-frequency neighbor list from RRC.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void tdssrchfach_gsm_init( void );


#endif /* FEATURE_TDSCDMA_CELL_FACH_MEAS_OCCASION */

#if 0
void tdssrchfach_interf_cleanup_done( void );
#endif

#endif /* SRCHFACH_H */
