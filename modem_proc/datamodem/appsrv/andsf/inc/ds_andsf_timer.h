/*===========================================================================
                      DS_ANDSF_TIMER.H

DESCRIPTION
 timer header file for ANDSF.

EXTERNALIZED FUNCTIONS

 Copyright (c) 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  when        who                            what, where, why
--------    -------                ----------------------------------------
05/23/16    Youjunf                          First version of file
===========================================================================*/

#ifndef DS_ANDSF_TIMER_H
#define DS_ANDSF_TIMER_H

/*===========================================================================
                           INCLUDE FILES
===========================================================================*/
#include "datamodem_variation.h"
#ifdef FEATURE_DATA_ANDSF

#include "rex.h"
#include "ds_andsf_utility.h"

/*---------------------------------------------------------------------------
  ANDSF ran meas timer IDs
    For Multi-Sim, timer ids will be calc as follows
    timer_id per subs_id = (subs_id * 100)+timer_id
---------------------------------------------------------------------------*/
typedef enum
{
  DS_ANDSF_INVALID_TIMERID_T0 = 0,
  DS_ANDSF_LTE_MEAS_SAMPLING_TIMERID_T1,
  DS_ANDSF_1X_MEAS_SAMPLING_TIMERID_T2,
  DS_ANDSF_1X_ACQ_LOST_TIMERID_T3,
  DS_ANDSF_MAX_TIMERID = DS_ANDSF_1X_ACQ_LOST_TIMERID_T3
}ds_andsf_timer_id_e_type;

typedef struct
{
  ds_andsf_subs_idx_e_type    subs_idx;
  ds_andsf_timer_id_e_type    timer_id;
}ds_andsf_timer_expipre_event_type;

/*---------------------------------------------------------------------------
  andsf Timer structure
---------------------------------------------------------------------------*/
typedef struct
{
  rex_timer_type  timer;
}ds_andsf_timer_info_s_type;

/*-------------------------------------------------------------------------
andsf timer list structure
--------------------------------------------------------------------------*/
typedef struct
{
  ds_andsf_timer_info_s_type  lte_sampling_timer;
  ds_andsf_timer_info_s_type  cdma_1x_sampling_timer;
  ds_andsf_timer_info_s_type  cdma_1x_acq_lost_hyst_timer; 
}ds_andsf_timer_list_s_type;



/*===========================================================================
FUNCTION      DS_ANDSF_TIMER_INIT

DESCRIPTION   Init ds_andsf_tiemr module

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  None.
===========================================================================*/
void ds_andsf_timer_init(void);


/*===========================================================================
FUNCTION      DS_ANDSF_TIMER_START_TIMER 
 
DESCRIPTION   Starts the andsf timer for specified subs_idx, timer_id 
 
PARAMETERS 
  subs_idx 
  timer_id
  duration

DEPENDENCIES  None.
RETURN VALUE  None
SIDE EFFECTS  Starts Timer.
===========================================================================*/
void ds_andsf_timer_start_timer
(
  ds_andsf_subs_idx_e_type subs_idx, 
  ds_andsf_timer_id_e_type timer_id,
  uint32                   duration
);


/*===========================================================================
FUNCTION      DS_ANDSF_TIMER_STOP_TIMER

DESCRIPTION   Stops the andsf timer for specified subs_idx, timer_id 
 
PARAMETERS 
  subs_idx 
  timer_id

DEPENDENCIES  None.
RETURN VALUE  None
SIDE EFFECTS  Stops Timer.
===========================================================================*/
void ds_andsf_timer_stop_timer
(
  ds_andsf_subs_idx_e_type subs_idx, 
  ds_andsf_timer_id_e_type timer_id
); 

#endif /* FEATURE_DATA_ANDSF */
#endif /* DS_ANDSF_TIMER_H */
