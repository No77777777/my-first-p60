/*===========================================================================

                   Multi-Mode System Selection Sensor based optimizations


DESCRIPTION
  This file defines the sensor related functionality for scan optimization


  EXTERNALIZED FUNCTIONS



INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2010-2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/


/* <EJECT> */
/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/mmcp.mpss/7.9.0/mmode/sd/src/sdsense.c#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/20/16   aj      Initial version.


===========================================================================*/

/* <EJECT> */
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/


/*lint -esym(766,comdef.h) */
#include "mmcp_variation.h"
#include "comdef.h"    /* Definition for basic types and macros */
#include "sdsense.h"
#ifdef FEATURE_MMODE_CAFI
#include "sdssscr.h"
#include "sdss.h"
#include "sddbg.h"    /* SD debug macros */
#include "sd_msgr_i.h"
#include "sd_v.h"
#include <math.h>

/* For memscpy */
#include <stringl/stringl.h>


#include "cafi.h"

/*===========================================================================
================================= DATA STRUCTURES ============================
===========================================================================*/
typedef struct
{

  uint8                      registered_subs;
  /* Subs that have registered to get battery/motion related notifications
     Updated when each sub registers or deregisters with SDsense based on srv
     status.
  */

  sdsense_reg_stt_e_type     cafi_register;
  /* indicates the state of Cafi registration
     updated based on UTILS_CAFI_REG_REQ, UTILS_CAFI_SCAN_REG_CNF
     and UTILS_CAFI_DEREG_REQ, UTILS_CAFI_SCAN_DEREG_CNF
  */

  sdsense_scan_type_e_type   scan_type ;
  /* Current scan type based on Cafi input
  */

  uint8                      max_bsr_stationary_stage;
  /* Maximum stage value (current count/ reps per stage) that the subs can go to
  ** in stationary mode without reaching the Tslow_bsr_max limit.
  ** If the stage value is more than this, there is no need to check the formula
  ** t0 * pow(2 , stage)since that might lead to overflow problems
  */

  uint8                      max_oos_stationary_stage;
  /* Maximum stage value (current count/ reps per stage) that the subs can go to
  ** in stationary mode without reaching the Tslow_oos_max limit.
  ** If the stage value is more than this, there is no need to check the formula
  ** t0 * pow(2 , stage)since that might lead to overflow problems
  */

} sdsense_common_s_type;


/*===========================================================================
================================= GLOBALS   ================================
===========================================================================*/

static sdsense_common_s_type  sdsense;
extern sys_modem_as_id_e_type global_sd_asubs_id;

/*===========================================================================
================================= COMMON DEFINES ============================
===========================================================================*/

#define sdsense_ptr() &sdsense


/* ----------------------------INTERNAL FUNCTIONS -----------------------------*/


/*===========================================================================
================================= SCAN STATE RELATED  ============================
===========================================================================*/


/*===========================================================================

FUNCTION sdsense_get_scan_state

DESCRIPTION
  Returns the current value of scan_state

DEPENDENCIES
 sdsense_init() should have been called

RETURN VALUE
  scan state

SIDE EFFECTS
  None.

===========================================================================*/
sdsense_scan_type_e_type       sdsense_get_scan_state( )
{
  return sdsense.scan_type;
}


/*===========================================================================

FUNCTION sdsense_set_scan_state

DESCRIPTION
  Sets the current scan state

DEPENDENCIES
 sdsense_init() should have been called

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static boolean       sdsense_set_scan_state(

  sdsense_scan_type_e_type  state

)
{
  if(sdsense.scan_type == state)
    return FALSE;

  SD_MSG_HIGH_2("SDSENSE: prev_state %d new_state %d",  sdsense.scan_type, state);
  sdsense.scan_type = state;
  return TRUE;
}

/*===========================================================================
================================= CAFI RELATED  ============================
===========================================================================*/

/*===========================================================================
FUNCTION sdsense_get_cafi_reg_state

DESCRIPTION
  Returns the current value of cafi registration

DEPENDENCIES
 sdsense_init() should have been called

RETURN VALUE
  TRUE - sdsense is registered with Cafi
  False - sdsense is not registered with Cafi

SIDE EFFECTS
  None.

===========================================================================*/
sdsense_reg_stt_e_type       sdsense_get_cafi_reg_state( )
{
  return sdsense.cafi_register;
}


/*===========================================================================

FUNCTION sdsense_set_cafi_reg_state

DESCRIPTION
  Sets the current scan state

DEPENDENCIES
 sdsense_init() should have been called

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void       sdsense_set_cafi_reg_state(

  sdsense_reg_stt_e_type        cafi_reg_state

)
{
  if(sdsense.cafi_register == cafi_reg_state)
    return;

  SD_MSG_HIGH_2("SDSENSE: Cafi reg prev_state %d new_state %d",  sdsense.cafi_register, cafi_reg_state);
  sdsense.cafi_register = cafi_reg_state;
}

/*===========================================================================

FUNCTION sdsense_process_cafi_state_change

DESCRIPTION
It processes the new cafi state and sends commands to SD
for each registered subs

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void                   sdsense_process_cafi_state_change(

  sdsense_scan_type_e_type     state
  /* new cafi state */
)
{
  sys_modem_as_id_e_type   subs = SYS_MODEM_AS_ID_1;

  if(sdsense.registered_subs == 0)
    return;

  /* Process cafi state change for each registered sub
  */
  for(subs = SYS_MODEM_AS_ID_1; subs < MAX_SIMS; ++subs)
  {
    /* If a subs is not registered for Cafi input, continue to the next sub
    */
    if (!(BM(subs)& sdsense.registered_subs))
      continue;

    /* Queue the state change indication to SD's command Q
    ** without waiting for a response
    */
    sd_process_sensor_state_change(subs);

  }

} //sdsense_process_cafi_state_change

/*===========================================================================

FUNCTION sdsense_process_cafi_input

DESCRIPTION
It processes the cafi input of battery level
and motion state and determines the scan_type

DEPENDENCIES
  None.

RETURN VALUE
  Initializes the sdsense component

SIDE EFFECTS
  None.

===========================================================================*/
static void                   sdsense_process_cafi_input(

  boolean             batt_normal,
  /* TRUE if battery is above threshhold ,
                                                                                                                                                                       FALSE otherwise */

  boolean             stationary
  /* TRUE if device is stationary  ,
                                                                                                                                                                       FALSE otherwise */

)
{

  sdsense_scan_type_e_type  scan_type   = SDSENSE_SCAN_NORMAL;


  if (batt_normal == TRUE)
  {
    if(stationary == FALSE)
    {
      /* normal battery and not stationary */
      scan_type = SDSENSE_SCAN_NORMAL;
    }
    else
    {
      /* normal battery and stationary */
      scan_type = SDSENSE_SCAN_STATIONARY;
    }
  }
  else // battery is below threshhold
  {
    if(stationary == FALSE)
    {
      /* low battery and not stationary */
      scan_type = SDSENSE_SCAN_NORMAL_BATT_LOW;
    }
    else
    {
      /* low battery and stationary */
      scan_type = SDSENSE_SCAN_STATIONARY_BATT_LOW;
    }
  }
  /* update the new state and take action if there is a change in state
  */
  if(sdsense_set_scan_state(scan_type))
  {
    sdsense_process_cafi_state_change(scan_type);
  }

} //sdsense_process_cafi_input

/*===========================================================================
================================= OOS TIMER   ============================
===========================================================================*/

/*===========================================================================

FUNCTION sdsense_get_normal_oos_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static uint32               sdsense_get_normal_oos_timer(

  uint8               curr_count
  /* Number of OOS cycles completed on the stack
                                                                                                                                                                    */
)
{
  uint32        t_norm_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_T_NORM_MAX,
                             global_sd_asubs_id);
  uint32        t_0        = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_TIMER_VALUE1,
                             global_sd_asubs_id);
  uint32        t_delta    = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_TIME_DELTA,
                             global_sd_asubs_id);
  uint32        reps       = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_REPS_PER_STAGE,
                             global_sd_asubs_id);
  uint32        curr_stage = 0;
  uint32        t_norm     = 0;
  uint32        curr_t     = 0;


  if (reps == 0)
  {
    SD_ASSERT(reps);
    SD_MSG_HIGH_1("Invalid value of OOS reps_per_stage %d -normal", reps);
    reps = SD_3GPP_OOS_REPS_PER_STAGE;
  }
  /* Get current OOS stage
  */
  curr_stage = curr_count / reps;

  if (t_delta == 0 ||
      curr_stage < ((t_norm_max - t_0) / t_delta + 1)
     )
  {
    t_norm = t_0 + curr_stage * t_delta;
    curr_t = MIN(t_norm, t_norm_max);
  }
  else
  {
    curr_t = t_norm_max;
  }


  SD_MSG_HIGH_6("OOS timer Norm subs %d curr_t %d Noos %d t_0 %d t_delta %d reps %d ", global_sd_asubs_id, curr_t, curr_count, t_0, t_delta, reps);
  SD_MSG_HIGH_1("OOS timer norm t_norm_max %d ", t_norm_max);
  return curr_t;

}
/*===========================================================================

FUNCTION sdsense_set_max_oos_stage_val

DESCRIPTION
  Find and set the MAX stage value for the stationary OOS timer (while in motion)
  max_stage = 1 + log(t_slow_max/t_0) / log (2)

  derived from Tslow_max = t_0  * pow (2, max_stage -1)

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void              sdsense_set_max_oos_stage_val()
{

  uint32        t_0        = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_TIMER_VALUE1,
                             global_sd_asubs_id);
  uint32        t_slow_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_T_SLOW_MAX,
                             global_sd_asubs_id);

  /* check that div by 0 is not possible
  */
  if (t_0 == 0)
  {
    SD_ASSERT(t_0);
    SD_MSG_HIGH_1("Invalid value of t_0 %d - OOS stage calc", t_0);
    /* Insert default value and return*/
    sdsense.max_oos_stationary_stage = SD_3GPP_STATIONARY_MAX_STAGE;
    return;
  }

  /* Max stage = no. of stages to reach t_slow_max + 1
  */
  sdsense.max_oos_stationary_stage = 1 + (uint8)(log(t_slow_max / t_0) / log(2));

} //sdsense_set_max_oos_stage_val
/*===========================================================================

FUNCTION sdsense_get_stationary_oos_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static uint32              sdsense_get_stationary_oos_timer(

  uint8               curr_count
  /* Number of OOS cycles completed on the stack
                                                                                                                    */
)
{

  uint32        t_0        = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_TIMER_VALUE1,
                             global_sd_asubs_id);
  uint32        t_slow_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_T_SLOW_MAX,
                             global_sd_asubs_id);
  uint32        reps       = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_REPS_PER_STAGE,
                             global_sd_asubs_id);
  uint32        curr_stage = 0;
  uint32        slow_t =  0;
  uint32        curr_t =  0;

  /* check that div by 0 is not possible
  */
  if (reps == 0)
  {
    SD_ASSERT(reps);
    SD_MSG_HIGH_1("Invalid value of OOS reps_per_stage %d - stationary", reps);
    reps = SD_3GPP_OOS_REPS_PER_STAGE;
  }
  /* get current OOS stage
  */
  curr_stage = curr_count / reps;

  /* To avoid overflow while calculating slow_t using the formula
     Perform the computation only if stage is less than
     the max value
  */
  if ( t_0 > 0 && //unlikely for t_0 == 0 to happen
       curr_stage > sdsense.max_oos_stationary_stage)
  {
    curr_t = t_slow_max;
  }
  else
  {
    /* likely scenario */
    slow_t =  (uint32)(t_0 * pow(2, curr_stage));
    curr_t = MIN(slow_t, t_slow_max);
  }
  SD_MSG_HIGH_6("OOS timer stationary subs %d curr_t %d Noos %d t_0 %d t_slow_max %d reps %d ", global_sd_asubs_id, curr_t, curr_count, t_0, t_slow_max, reps);
  return curr_t;

}

/*===========================================================================

FUNCTION sdsense_get_normal_low_batt_oos_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static uint32              sdsense_get_normal_low_batt_oos_timer(


)
{

  uint32        t_norm_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_T_NORM_MAX,
                             global_sd_asubs_id);

  uint32        curr_t = 2 * t_norm_max;
  SD_MSG_HIGH_3("OOS timer normal low batt subs %d curr_t %d  t_norm_max %d  ", global_sd_asubs_id, curr_t, t_norm_max);
  return curr_t;

}
/*===========================================================================

FUNCTION sdsense_get_stationary_low_batt_oos_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static uint32              sdsense_get_stationary_low_batt_oos_timer(


)
{

  uint32        t_slow_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_OOS_T_SLOW_MAX,
                             global_sd_asubs_id);

  uint32        curr_t = 2 * t_slow_max;
  SD_MSG_HIGH_3("OOS timer stationary low batt subs %d curr_t %d  t_slow_max %d  ", global_sd_asubs_id, curr_t, t_slow_max);
  return curr_t;

}


/*===========================================================================

FUNCTION sdsense_calc_oos_timer_val

DESCRIPTION
   calculates oos timer value given the scan type and the Number of oos cycles
   completed so far

DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static  dword               sdsense_calc_oos_timer_val(

  uint8                           curr_count,
  /* Number of OOS cycles completed on the stack
                                                                                                          */
  sdsense_scan_type_e_type         scan_type
  /* scan type used to determine
                                                                                                          */
)
{


  /* OOS timer derived based on scan type
  */
  switch(scan_type)
  {
    /* normal ( default scan )
    */
    case SDSENSE_SCAN_NORMAL:
    default:
      return sdsense_get_normal_oos_timer(curr_count);

    /* slow scan ( stationary scenario )
    */
    case SDSENSE_SCAN_STATIONARY:
      return sdsense_get_stationary_oos_timer(curr_count);

    /* low battery level scan while in motion
    */
    case SDSENSE_SCAN_NORMAL_BATT_LOW:
      return sdsense_get_normal_low_batt_oos_timer();

    /* low battery level scan while stationary
    */
    case SDSENSE_SCAN_STATIONARY_BATT_LOW:
      return sdsense_get_stationary_low_batt_oos_timer();

  } //switch ( scan_type)

} /* sdsense_calc_oos_timer_val */

/*===========================================================================
================================= BSR  TIMER   ============================
===========================================================================*/

/*===========================================================================

FUNCTION sdsense_get_normal_bsr_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static uint32               sdsense_get_normal_bsr_timer(

  uint8               curr_count
  /* Number of OOS cycles completed on the stack
                                                                                                              */
)
{
  uint32        t_norm_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_T_NORM_MAX,
                             global_sd_asubs_id);
  uint32        t_0        = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_INITIAL_VAL,
                             global_sd_asubs_id);
  uint32        t_delta    = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_TIME_DELTA,
                             global_sd_asubs_id);
  uint32        reps       = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_REPS_PER_STAGE,
                             global_sd_asubs_id);
  /* Get current BSR stage
  */
  uint32        stage  = curr_count / reps;
  uint32        t_norm = t_0 + stage * t_delta;
  uint32        curr_t = MIN(t_norm, t_norm_max);
  SD_MSG_HIGH_6("BSR timer Norm subs %d curr_t %d Nbsr %d t_0 %d t_delta %d reps %d ", global_sd_asubs_id, curr_t, curr_count, t_0, t_delta, reps);
  SD_MSG_HIGH_1("BSR timer norm t_norm_max %d ", t_norm_max);
  return curr_t;

}
/*===========================================================================

FUNCTION sdsense_set_max_bsr_stage_val

DESCRIPTION
  Find and set the MAX stage value for the stationary BSR timer (while in motion)
  max_stage = 1 + log(t_slow_max/t_0) / log (2)

  derived from Tslow_max = t_0  * pow (2, max_stage -1)

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
static void              sdsense_set_max_bsr_stage_val()
{

  uint32        t_0        = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_INITIAL_VAL,
                             global_sd_asubs_id);
  uint32        t_slow_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_MAX_TIME,
                             global_sd_asubs_id);

  /* check that div by 0 is not possible
  */
  if (t_0 == 0)
  {
    SD_ASSERT(t_0);
    SD_MSG_HIGH_1("Invalid value of t_0 %d - BSR stage calc", t_0);
    /* Insert default value and return*/
    sdsense.max_bsr_stationary_stage = SD_3GPP_STATIONARY_MAX_STAGE;
    return;
  }

  /* Max stage = no. of stages to reach t_slow_max + 1
  */
  sdsense.max_bsr_stationary_stage = 1 + (uint8)(log(t_slow_max / t_0) / log(2));

} //sdsense_set_max_bsr_stage_val

/*===========================================================================

FUNCTION sdsense_get_stationary_bsr_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static uint32              sdsense_get_stationary_bsr_timer(

  uint8               curr_count
  /* Number of OOS cycles completed on the stack
  */
)
{

  uint32        t_0        = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_INITIAL_VAL,
                             global_sd_asubs_id);
  uint32        t_slow_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_MAX_TIME,
                             global_sd_asubs_id);
  uint32        reps       = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_REPS_PER_STAGE,
                             global_sd_asubs_id);
  uint32        curr_stage = 0;
  uint32        slow_t     =  0 ;
  uint32        curr_t     = 0;

  /* check that div by 0 is not possible
  */
  if(reps == 0)
  {
    SD_ASSERT(reps);
    SD_MSG_HIGH_1("Invalid value of BSR reps_per_stage %d - stationary",reps );
    reps = SD_3GPP_BSR_REPS_PER_STAGE;
  }
  /* get current BSR stage */
  curr_stage = curr_count/reps;

  if(curr_stage >sdsense.max_bsr_stationary_stage && t_0 > 0 )
  {
    curr_t = t_slow_max;
  }
  else
  {
    slow_t =  (uint32) (t_0 * pow(2, curr_stage)) ;
    curr_t = MIN(slow_t,t_slow_max);
  }
  SD_MSG_HIGH_6("BSR timer stationary subs %d curr_t %d Noos %d t_0 %d t_slow_max %d reps %d ", global_sd_asubs_id, curr_t, curr_count, t_0, t_slow_max, reps );
  return curr_t;

}

/*===========================================================================

FUNCTION sdsense_get_normal_low_batt_bsr_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static uint32              sdsense_get_normal_low_batt_bsr_timer(


)
{

  uint32        t_norm_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_T_NORM_MAX,
                             global_sd_asubs_id);

  uint32        curr_t = 2 * t_norm_max;
  SD_MSG_HIGH_3("BSR timer normal low batt subs %d curr_t %d  t_norm_max %d  ", global_sd_asubs_id, curr_t, t_norm_max);
  return curr_t;

}
/*===========================================================================

FUNCTION sdsense_get_stationary_low_batt_bsr_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
static uint32              sdsense_get_stationary_low_batt_bsr_timer(


)
{

  uint32        t_slow_max = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_BSR_MAX_TIME,
                             global_sd_asubs_id);

  uint32        curr_t = 2 * t_slow_max;
  SD_MSG_HIGH_3("BSR timer stationary low batt subs %d curr_t %d  t_norm_max %d  ", global_sd_asubs_id, curr_t, t_slow_max);
  return curr_t;

}

/*===========================================================================

FUNCTION  sdsense_cafi_register

DESCRIPTION
  This sends a Cafi registration message to Cafi utils module

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static errno_enum_type                   sdsense_cafi_register(

  sys_modem_as_id_e_type       asubs_id
  /* subs that is deregistering for battery / motion input
  */

)

{
  errno_enum_type                msgr_error      = E_SUCCESS;
  utils_cafi_reg_req_s           cafi_reg_req;
  sdsense_reg_stt_e_type         reg_state = sdsense_get_cafi_reg_state();

  /* check that SD is not already registered before proceeding.
     Should not hit this error condition
  */
  if ( reg_state == SDSENSE_REG_STT_REGISTERED ||
       reg_state == SDSENSE_REG_STT_REG_PENDING)
  {
    SD_MSG_HIGH_1("SD already registered with cafi %d", reg_state);
    return E_ALREADY_DONE;
  }

  memset(&cafi_reg_req, 0, sizeof(utils_cafi_reg_req_s));
  cafi_reg_req.client       = CAFI_CLIENT_MMODE;
  cafi_reg_req.optimization = CAFI_OPTIMIZATION_MMODE_SCAN;

  msgr_error = sd_msgr_send( UTILS_CAFI_REG_REQ, asubs_id, &(cafi_reg_req.hdr), sizeof(utils_cafi_reg_req_s) );
  if(msgr_error == E_SUCCESS)
  {
    SD_MSG_HIGH_1("SD->MSGR: sub %d, send cafi register req", asubs_id);
  }

  /* Reset scan state to NORMAL at time of registration with Cafi
  */
  (void)sdsense_set_scan_state(SDSENSE_SCAN_NORMAL);

  /* Set registration state to indicate that it is waiting for reg cnf
  */
  sdsense_set_cafi_reg_state( SDSENSE_REG_STT_REG_PENDING );
  return msgr_error;
}

/*===========================================================================

FUNCTION  sdsense_cafi_deregister

DESCRIPTION
  This sends a Cafi deregistration message to Cafi utils module

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static errno_enum_type                   sdsense_cafi_deregister(

  sys_modem_as_id_e_type       asubs_id
  /* subs that is deregistering for battery / motion input
  */

)

{
  errno_enum_type                msgr_error      = E_SUCCESS;
  utils_cafi_dereg_req_s         cafi_dereg_req;
  sdsense_reg_stt_e_type         reg_state = sdsense_get_cafi_reg_state();

  /* check that current Cafi reg status is TRUE before proceeding.
     Should not hit this error condition
  */
  if ( reg_state == SDSENSE_REG_STT_DEREG_PENDING ||
       reg_state == SDSENSE_REG_STT_DEREGISTERED)
  {
    return E_ALREADY_DONE;
  }

  memset(&cafi_dereg_req, 0, sizeof(utils_cafi_dereg_req_s));
  cafi_dereg_req.client       = CAFI_CLIENT_MMODE;
  cafi_dereg_req.optimization = CAFI_OPTIMIZATION_MMODE_SCAN;
  msgr_error = sd_msgr_send( UTILS_CAFI_DEREG_REQ, asubs_id, &(cafi_dereg_req.hdr), sizeof(utils_cafi_dereg_req_s) );
  if(msgr_error == E_SUCCESS)
  {
    SD_MSG_HIGH_1("SD->MSGR: sub %d, send cafi deregister req", asubs_id);
  }

  sdsense_set_cafi_reg_state( SDSENSE_REG_STT_DEREG_PENDING);
  return msgr_error;
}

/*===========================================================================

FUNCTION  ssdsense_handle_cafi_ind

DESCRIPTION
 Handles MSGR indication from Cafi regarding battery and motion sensor
 information

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void                 sdsense_handle_cafi_ind
(
  void                   *msg
)
{
  utils_cafi_mmode_scan_ind_s    *scan_info_ptr = (utils_cafi_mmode_scan_ind_s*) msg;
  boolean                  batt_normal    = TRUE;
  boolean                  stationary     = FALSE;


  SD_ASSERT(msg != NULL);
  if( scan_info_ptr == NULL)
    return;

  /* If no sub is registered, disregard Cafi indication
  */  
  if(sdsense.registered_subs == 0)
  {
    SD_MSG_HIGH_0("Drop Cafi ind recieved in unexpected state");
    return;
  }


  /* If battery state is LOW, change value of batt_normal to FALSE.
     If the state is HIGH or DISABLED( meaning that the battery info
     is not available), batt_normal can remain as TRUE
  */
  if( scan_info_ptr->battery_sensor.state == CAFI_BATTERY_SENSOR_STATE_LOW )
  {
    batt_normal   = FALSE;
  }
  /* If motion state is stationary of absolute stationary, change value of stationary
     to TRUE.
     If the state is MOTION or DISABLED( meaning that the motion sensor info
     is not available), stationary can remain as FALSE
  */
  if( scan_info_ptr->motion_sensor.state == CAFI_MOTION_SENSOR_STATE_STATIONARY ||
      scan_info_ptr->motion_sensor.state == CAFI_MOTION_SENSOR_STATE_ABS_STATIONARY)
  {
    stationary   = TRUE;
  }
  sdsense_process_cafi_input(batt_normal, stationary);
}
/*===========================================================================

FUNCTION  sdsense_handle_cafi_reg_cnf

DESCRIPTION
 Handles MSGR indication from Cafi regarding registration confirmation

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void sdsense_handle_cafi_reg_cnf
(
  void                   *msg
)
{
  utils_cafi_reg_cnf_s          *reg_cnf   = (utils_cafi_reg_cnf_s *) msg;
  sdsense_reg_stt_e_type         reg_state = sdsense_get_cafi_reg_state();

  SD_ASSERT(msg != NULL);
  if( reg_cnf == NULL)
    return;

  /* If the CNF is not for MMODE or it is not the expected optimization,
     drop the report and terminate processing it further
  */
  if(!( reg_cnf->client       == CAFI_CLIENT_MMODE &&
      reg_cnf->optimization == CAFI_OPTIMIZATION_MMODE_SCAN  ))
  {
    /* CNF is not for MMODE
    */
    SD_MSG_HIGH_4("Cafi reg cnf dropped %d %d %d %d",reg_cnf->cnf_status,reg_cnf->client,reg_cnf->optimization, reg_state );
    return;
  }

  /* Set cafi reg status to TRUE if Cafi indicates reg success ( only if SD is waiting
     for reg cnf)
  */

  if( reg_cnf->cnf_status   == E_SUCCESS &&
      reg_state             == SDSENSE_REG_STT_REG_PENDING )
  {
    sdsense_set_cafi_reg_state( SDSENSE_REG_STT_REGISTERED );
  }
  else
  {
    /*  If we got here, either SD state is not reg_pending or registration is unsuccessful at Cafi
    */
    SD_MSG_HIGH_4("Cafi reg cnf for MMODE unexpected %d %d %d %d",reg_cnf->cnf_status,reg_cnf->client,reg_cnf->optimization, reg_state );
    if(reg_state             == SDSENSE_REG_STT_REG_PENDING )
    {
      /* if reg is not successful, move to deregistered state
      */
      sdsense_set_cafi_reg_state( SDSENSE_REG_STT_DEREGISTERED );
    }
  }
}

/*===========================================================================

FUNCTION  sdsense_handle_cafi_dereg_cnf

DESCRIPTION
 Handles MSGR indication from Cafi regarding de-registration confirmation

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void sdsense_handle_cafi_dereg_cnf
(
  void                   *msg
)
{
  utils_cafi_dereg_cnf_s         *dereg_cnf = (utils_cafi_dereg_cnf_s *) msg;
  sdsense_reg_stt_e_type         reg_state = sdsense_get_cafi_reg_state();

  SD_ASSERT(msg != NULL);
  if( dereg_cnf == NULL)
    return;

  /* If dereg cnf is not for MMODE optimization ,drop further processing and return.
  */
  if(!( dereg_cnf->client       == CAFI_CLIENT_MMODE &&
      dereg_cnf->optimization == CAFI_OPTIMIZATION_MMODE_SCAN ))
  {
    SD_MSG_HIGH_4("Cafi de-reg cnf dropped %d %d %d %d",dereg_cnf->cnf_status,dereg_cnf->client,dereg_cnf->optimization, reg_state );
    return;
  }

  /* Set cafi reg status to NONE if Cafi indicates dereg success
  */
  if( dereg_cnf->cnf_status   == E_SUCCESS &&
      reg_state               == SDSENSE_REG_STT_DEREG_PENDING)
  {
    sdsense_set_cafi_reg_state( SDSENSE_REG_STT_DEREGISTERED );
  }
  else
  {
    /* not expected to hit this
    */
    SD_MSG_HIGH_4("MMODE Cafi de-reg cnf dropped %d %d %d %d",dereg_cnf->cnf_status,dereg_cnf->client,dereg_cnf->optimization, reg_state );
    if(reg_state             == SDSENSE_REG_STT_DEREG_PENDING )
    {
      /* if DEreg is not successful (unlikely given that registration was), move back to registered state
      */
      sdsense_set_cafi_reg_state( SDSENSE_REG_STT_REGISTERED );
    }
  }
}

/*===========================================================================
===================    Internal functions end    ============================
===========================================================================*/

/*===========================================================================
================================= EXTERNAL FUNCTIONS ========================
===========================================================================*/
/*===========================================================================

FUNCTION sdsense_init

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  Initializes the sdsense component

SIDE EFFECTS
  None.

===========================================================================*/
void                   sdsense_init( )
{
  sdsense.registered_subs = 0;
  sdsense.cafi_register = SDSENSE_REG_STT_DEREGISTERED;
  sdsense.scan_type = SDSENSE_SCAN_NORMAL;
  /* sets the MAX stage val that will be used for BSR and OOS stationary scans
  */
  sdsense_set_max_bsr_stage_val();
  sdsense_set_max_oos_stage_val();
}

/*===========================================================================

FUNCTION sdsense_deregister

DESCRIPTION
It deregisters a sub from knowing the Cafi input.
SDsense will update the registered_subs information.
if no subs is registered , SDsense will deregister from Cafi

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void                   sdsense_deregister(

  sys_modem_as_id_e_type       asubs_id
  /* subs that is deregistering for battery / motion input
  */

)
{

  /* If the sub has registered, update the value in sdsense
  */
  sdsense.registered_subs  = sdsense.registered_subs & ~(BM(asubs_id));

  /* If no sub is registered, deregister with Cafi
  */
  if(sdsense.registered_subs == 0 )
  {
    (void)sdsense_cafi_deregister(asubs_id);
  }

}

/*===========================================================================

FUNCTION sdsense_register

DESCRIPTION
It registers a sub for knowing the Cafi input ( provided slow scans are enabled via NV)
SDsense will update the registered_subs information.
if no sub was registered to begin with , SDsense will register with Cafi

DEPENDENCIES
  None.

RETURN VALUE
  None

SIDE EFFECTS
  None.

===========================================================================*/
void                   sdsense_register(

  sys_modem_as_id_e_type       asubs_id
  /* subs that is deregistering for battery / motion input
  */

)
{

  uint32               slow_scan_enabled = 0;
  errno_enum_type      err               = E_SUCCESS;
  /* If slow scans are disabled, do not register with Cafi
  */
  slow_scan_enabled = sdssscr_efs_item_get(SDSSSCR_EFS_ITEM_3GPP_SLOW_SCAN_ENABLED, asubs_id);
  if (slow_scan_enabled == 0)
  {
    return;
  }

  /* If no sub has registered yet , register with Cafi
  */
  if(sdsense.registered_subs == 0)
  {
    err = sdsense_cafi_register(SYS_MODEM_AS_ID_1);
    /* If Cafi registration req could not be sent successfully or
    ** if SDSENSE is already registered with Cafi( should not happen ideally), return
    */
    if (!(err == E_SUCCESS || err == E_ALREADY_DONE))
    {
      return;
    }
  }

  /* Update the value in sdsense to indicate this sub is registered
  */
  sdsense.registered_subs |= BM(asubs_id);
  SD_MSG_HIGH_1("sdsense registered subs %d",sdsense.registered_subs);

} ////sdsense_register

/*===========================================================================

FUNCTION  sdsense_msgr_handle

DESCRIPTION
 Handles MSGR indications in sdsense module. Calls each individual handler.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void           sdsense_msgr_handle(

  void         *param
  /* sd_msgr_type payload */
)
{
  sd_msgr_type       *msg = (sd_msgr_type *)param;
  if (msg == NULL)
    return;

  switch ( msg->cmd.hdr.id )
  {
  case UTILS_CAFI_MMODE_SCAN_REG_CNF:
      sdsense_handle_cafi_reg_cnf(&(msg->cmd.cafi_reg));
      break;

    case UTILS_CAFI_MMODE_SCAN_DEREG_CNF:
      sdsense_handle_cafi_dereg_cnf(&(msg->cmd.cafi_dereg));
      break;

    case UTILS_CAFI_MMODE_SCAN_IND:
      sdsense_handle_cafi_ind(&(msg->cmd.cafi_info));
      break;

    default:
      SD_MSG_HIGH_1("unhandled msg %d", msg->cmd.hdr.id);
      break;
  }
} //sdsense_msgr_handle


/*===========================================================================

FUNCTION sdsense_get_curr_oos_timer

DESCRIPTION


DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
EXTERN  dword               sdsense_get_curr_oos_timer(

  uint8               curr_count
  /* Number of OOS cycles completed on the stack
  */
)
{

  sdsense_scan_type_e_type         scan_type  = SDSENSE_SCAN_NORMAL;


  /*--------------------------------------------------------------------------------------------*/
  /* Get the scan type
  */
  scan_type = sdsense_get_scan_state( );


  /*--------------------------------------------------------------------------------------------*/
  /* OOS timer derived based on scan type
  */
  return sdsense_calc_oos_timer_val(curr_count,scan_type);


} /* sdsense_get_curr_oos_timer */

/*===========================================================================

FUNCTION sdsense_get_curr_lmtd_bsr_timer

DESCRIPTION
  Returns the pwr save timer that should be used for BSR

DEPENDENCIES
  None.

RETURN VALUE
  timer value

SIDE EFFECTS
  None.

===========================================================================*/
dword               sdsense_get_curr_lmtd_bsr_timer(


  uint8               curr_count
  /* Number of BSR cycles completed on the stack
  */
)

{

  sdsense_scan_type_e_type         scan_type  = SDSENSE_SCAN_NORMAL;


  /*--------------------------------------------------------------------------------------------*/
  /* Get the scan type
  */
  scan_type = sdsense_get_scan_state( );


  /*--------------------------------------------------------------------------------------------*/
  /* BSR timer derived based on scan type
  */

  switch(scan_type)
  {
    /* normal ( default scan )
    */
    case SDSENSE_SCAN_NORMAL:
    default:
      return sdsense_get_normal_bsr_timer(curr_count);

    /* slow scan ( stationary scenario )
    */
    case SDSENSE_SCAN_STATIONARY:
      return sdsense_get_stationary_bsr_timer(curr_count);

    /* low battery level scan while in motion
    */
    case SDSENSE_SCAN_NORMAL_BATT_LOW:
      return sdsense_get_normal_low_batt_bsr_timer();

    /* low battery level scan while stationary
    */
    case SDSENSE_SCAN_STATIONARY_BATT_LOW:
      return sdsense_get_stationary_low_batt_bsr_timer();

  }//switch ( scan_type)

} /* sdsense_get_curr_lmtd_bsr_timer */

#endif //FEATURE_MMODE_CAFI
