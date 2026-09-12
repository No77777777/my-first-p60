#ifndef SRCHDCH_H
#define SRCHDCH_H
/*===========================================================================

   S R C H    D C H    S T A T E   R E L A T E D   F U N C T I O N S

DESCRIPTION

EXTERNALIZED FUNCTIONS

INTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS
  None


REFERENCES
  CDMA Mobile Station ASIC Specification Sheet

Copyright (c) 2000-2011 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"

#include "tdsl1rrcif.h"


/*===========================================================================

                        DATA DECLARATIONS

===========================================================================*/

/*--------------------------------------------------------------------------
                          DCH SEARCH SUBSTATES

--------------------------------------------------------------------------*/
typedef enum
{
  TDSSRCHDCH_INIT,           /* Initialization */
  TDSSRCHDCH_NONE,        /* No search activity */
  TDSSRCHDCH_MAIN,
  TDSSRCHDCH_PRE_HO,
  TDSSRCHDCH_HO,
  TDSSRCHDCH_TIMING_UNFOUND,
  TDSSRCHDCH_STATE_MAX       /* Last item in enum. */
}
tdssrchdch_substate_enum_type;

extern tdssrchdch_substate_enum_type  tdssrchdch_substate;

extern void tdssrchdch_start_dch_measurements(void);

extern uint8 tdssrchdch_evt_rpt_cell_cnt;

/*===========================================================================

                      FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION TDSSRCHDCH_EVT_3A_EVAL

DESCRIPTION
  This function performs UMTS Quality evaluation for the Event 3a handling
  code.

  Qutran - the quality of the UMTS Active set signals
  Mutran - the measured energy of the UMTS Active set as specified in
           the following formula.
  Mi     - the measured energy of the ith UMTS active set signal
  Mbest  - the measured energy of the best UMTS active set signal
            ( == tdssrchset_aset_ptr[0] )
  Na     - the number of active set cells
  W      - signaled parameter
  Tused  - signaled Threshold parameter
  Hyst3a - signaled hysteresis parameter

  Qutran = 10 * Log(Mutran) = W * 10 * Log(SUM[1..Na] Mi) + (1-W) * 10 * Log(Mbest)

  If the following formula is satisfied the UMTS quality is below the UTRAN specified
  threshold.  Event 3a reporting will take place iff an Inter-RAT signal is above
  the specified threshold.
  
  Qutran <= Tused - Hyst3a/2


DEPENDENCIES
  tdssrchset_aset_ptr[0] always points to the strongest cell in ASET. This is
  guranteed by function srchdch_proc_acset_srch() immediately after 
  each HPQ list search dump.
  
  The neighbor cell's and each ASET cell's ecio have been computed in 
  srchdch_proc_sync_nset_srch() and srchdch_proc_acset_srch().

RETURN VALUE
  if  ( Qutran <= Tused - Hyst3a/2 )
    return (TRUE);
  else
    return (FALSE);

SIDE EFFECTS
  None
===========================================================================*/
boolean tdssrchdch_evt_3a_eval(
  int32  Tused,
    /* Threshold to be used, specified by network */
  int32  Hyst3a
    /* hysterises specified by network */
);

/*===========================================================================

FUNCTION SRCHDCH_EVT_3A_EVAL

DESCRIPTION
  This function performs UMTS Quality evaluation for the Event 3a handling
  code.

  Qutran - the quality of the UMTS Active set signals
  Mutran - the measured energy of the UMTS Active set as specified in
           the following formula.
  Mi     - the measured energy of the ith UMTS active set signal
  Mbest  - the measured energy of the best UMTS active set signal
            ( == srch_aset_ptr[0] )
  Na     - the number of active set cells
  W      - signaled parameter
  Tused  - signaled Threshold parameter
  Hyst3a - signaled hysteresis parameter

  Qutran = 10 * Log(Mutran) = W * 10 * Log(SUM[1..Na] Mi) + (1-W) * 10 * Log(Mbest)

  If the following formula is satisfied the UMTS quality is below the UTRAN specified
  threshold.  Event 3a reporting will take place iff an Inter-RAT signal is above
  the specified threshold.
  
  Qutran <= Tused - Hyst3a/2


DEPENDENCIES
  srch_aset_ptr[0] always points to the strongest cell in ASET. This is
  guranteed by function srchdch_proc_acset_srch() immediately after 
  each HPQ list search dump.
  
  The neighbor cell's and each ASET cell's ecio have been computed in 
  srchdch_proc_sync_nset_srch() and srchdch_proc_acset_srch().

RETURN VALUE
  if  ( Qutran <= Tused - Hyst3a/2 )
    return (TRUE);
  else
    return (FALSE);

SIDE EFFECTS
  None
===========================================================================*/
 #if defined(FEATURE_TDSCDMA_IDLEINTERVAL_INFO_SUPPORT) || defined(FEATURE_TDSCDMA_DMO_SUPPORT)
tdssrchdch_event3a_crit_enum_type tdssrchdch_eutra_evt_3a_eval(

  tdsl1_meas_quan_enum_type  meas_quan,
    /* EcIo or RSCP Measurement Quantity Specified */
  int32  W,
    /* weight specified by network */
  int32  Tused,
    /* Threshold to be used, specified by network */
  int32  Hyst3a
    /* hysterises specified by network */
);
#endif
/*===========================================================================
FUNCTION     tdssrchdch_proc_rscp_done

DESCRIPTION:
    handle rscp_rsp in different substate of dch
  
DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None
===========================================================================*/
extern void tdssrchdch_proc_rscp_done(uint8 message_id);

/*===========================================================================
FUNCTION        SRCHDCH_STOP_DCH_MEASUREMENTS

FILE NAME       

DESCRIPTION     

DEPENDENCIES    

extern void tdssrchdch_sort_reporting_cells_temp (uint16 meas_idx );

extern void tdssrchdch_signal_dch_measurements(void);

SIDE EFFECTS    
===========================================================================*/
extern void tdssrchdch_stop_dch_measurements(void);

extern void tdssrchdch_sort_reporting_cells_temp (uint16 meas_idx );


/*===========================================================================
FUNCTION        SRCHDCH_SIGNAL_DCH_MEASUREMENTS

FILE NAME       

DESCRIPTION     
            Called after DL sync is achieved. Starts DCH measurements if 
            they have not yet been started.
DEPENDENCIES    

RETURN VALUE    
  none
SIDE EFFECTS    
===========================================================================*/
extern void tdssrchdch_signal_dch_measurements(void);

/*===========================================================================

FUNCTION TDSSRCHDCH_DL_INIT_SYNC_STATE

DESCRIPTION
  This function return one status if DL module is in init-sync phase. 

DEPENDENCIES
  None.

RETURN VALUE
  TRUE: in-sync phase.

SIDE EFFECTS
  None.

===========================================================================*/
extern boolean tdssrchdch_dl_init_sync_state( void );

#endif /* SRCHDCH_H */
