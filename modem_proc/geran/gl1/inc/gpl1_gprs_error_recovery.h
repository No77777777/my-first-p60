#ifndef GPL1_GPRS_ERROR_RECOVERY_H
#define GPL1_GPRS_ERROR_RECOVERY_H

#ifdef FEATURE_GERAN_PTM_ERR_RECOVERY

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
                    G L 1   E R R O R    R E C O V E R Y 

GENERAL DESCRIPTION

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

Copyright (c) 2001 - 2015 Qualcomm Technologies, Inc.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

$Header: //components/rel/geran.mpss/7.4.0/gl1/inc/gpl1_gprs_error_recovery.h#1 $ $DateTime: 2019/06/06 23:27:31 $ $Author: mplcsds1 $

when       who       what, where, why
--------   --------  ----------------------------------------------------------
16/03/17   mn      CR2020307 FR40846:Packet transfer Error Recovery mechanism.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE


===========================================================================*/

/*===========================================================================


                        DATA DECLARATIONS

===========================================================================*/

/*========================= typedefs ======================================*/

#define GPL1_FRAME_COUNT_FOR_RECOVERY_50     50    /*FN added to starting time/register time of event added in the event table and compared with current FN*/
#define GPL1_FRAME_COUNT_FOR_RECOVERY_100    100   /*FN added to starting time/register time of event added in the event table and compared with current FN*/
#define GPL1_FRAME_COUNT_FOR_RECOVERY_CHECK  30    /*FN periodicity for recovery check*/
#define GPL1_WAIT_COUNT_TRIGGER_RECOVERY     1     /*No.of times registered event is checked before triggering recovery */
#define GPL1_WAIT_COUNT_TRIGGER_RECOVERY_STATE_MISMATCH     5


/*Events for which error recovery is handled*/
typedef enum
{
  GPL1_UL_CONNECT_EVENT, /*Event used for recovery of Dynamic TBF config related scenerio*/
  GPL1_DL_CONNECT_EVENT, /*Event used for recovery of DL TBF config related scenerio*/
  GPL1_UL_RELEASE_EVENT, /*Event used for recovery of UL RELEASE related scenerio*/
  GPL1_DL_RELEASE_EVENT, /*Event used for recovery of DL RELEASE related scenerio*/
  GPL1_LOW_TS_EVENT,     /*Event used for recovery of when Low TS related scenerio*/
  GPL1_MAX_EVENT 
}gpl1_recovery_events_t;


typedef enum
{
  GPL1_RECOVERY_NULL_STATE, /*Intial Recovery state */
  GPL1_RECOVERY_CHECK_STATE, /*State where check for recovery is performed*/
  GPL1_RECOVERY_IND_SENT_STATE, /*Moved to this state when recovery is identified and GL1 sends MAC_L1_RECOVERY_IND*/
  GPL1_RECOVERY_STARTED_STATE, /*Moved to this state when MAC_L1_RECOVERY_REQ is recieved from MAC*/
}gpl1_recovery_substate;

typedef struct
{
  uint32 starting_time ; /*staring FN of the event*/
  uint32 registered_fn ; /*Recieved FN of the event*/
  boolean valid; /*Validity of the data stored in event table*/
  uint8 recovery_check_count; /*keeps track of no. of times recovery check is performed*/
}gpl1_recovery_event_table_t;

typedef struct
{
  gpl1_recovery_event_table_t event_table[GPL1_MAX_EVENT]; 
  gpl1_recovery_substate sub_state;
  uint8 state_mismatch_count;
  boolean recovery_cnf_pending;
}gpl1_recovery_data_t;


/*===========================================================================

                        PUBLIC DECLARATIONS



===========================================================================*/

/*===========================================================================

FUNCTION      gpl1_gprs_recovery_tick

DESCRIPTION   Called in every ISR in transfer mode . This function Handles error recovery based on the states .

RETURN VALUE  None

===========================================================================*/



void gpl1_gprs_recovery_tick(gas_id_t gas_id);


/*===========================================================================

FUNCTION      gpl1_gprs_recovery_register_event

DESCRIPTION   Register the transfer mode events for which error recovery is handled.

RETURN VALUE  None

===========================================================================*/

void gpl1_gprs_recovery_register_event(gpl1_recovery_events_t event , uint32 starting_time ,gas_id_t gas_id);

/*===========================================================================

FUNCTION      gpl1_gprs_recovery_deregister_event

DESCRIPTION   Deregister the transfer mode events which registered from gpl1_gprs_err_recovery_register_event.

RETURN VALUE  None

===========================================================================*/
void gpl1_gprs_recovery_deregister_event(gpl1_recovery_events_t event,gas_id_t gas_id);

/*===========================================================================

FUNCTION      gpl1_gprs_recovery_reset

DESCRIPTION   Reset varaibles used during error recovery.

RETURN VALUE  None

===========================================================================*/

void gpl1_gprs_recovery_reset(gas_id_t gas_id);


/*===========================================================================

FUNCTION      gpl1_gprs_handle_recovery_req

DESCRIPTION   Handles MAC_L1_ERROR_RECOVERY_REQ sent by MAC to L1.

RETURN VALUE  None

===========================================================================*/

void gpl1_gprs_handle_recovery_req(gas_id_t gas_id);

/*===========================================================================

FUNCTION      gpl1_gprs_recovery_send_l1_mac_recovery_ind

DESCRIPTION  L1 send reset Recovery indication to MAC.

RETURN VALUE  None

===========================================================================*/
  

void gpl1_gprs_recovery_send_l1_mac_recovery_ind(gas_id_t gas_id);


/*===========================================================================

FUNCTION      gpl1_gprs_recovery_send_l1_mac_recovery_cnf

DESCRIPTION  GL1 send recovery_cnf  to MAC.

RETURN VALUE  None

===========================================================================*/
  

void gpl1_gprs_recovery_send_l1_mac_recovery_cnf(gas_id_t gas_id);

/*===========================================================================

FUNCTION      gmac_get_tbf_status

DESCRIPTION   Map Gl1 alloc type to corresponding Mac TBF type .

RETURN VALUE  None

===========================================================================*/

void gmac_get_tbf_status (uint8* main_state, uint8* tbf_active, gas_id_t gas_id);

/*===========================================================================

FUNCTION      gpl1_gprs_recovery_l1_mac_state_check

DESCRIPTION   Checks whether state mismatch has Occured between GL1/MAC .

RETURN VALUE   Boolean(state mismatch has ocuured or not)

===========================================================================*/


boolean gpl1_gprs_recovery_l1_mac_state_check(gas_id_t gas_id);

/*===========================================================================

FUNCTION      gpl1_gprs_register_release_recovery_event

DESCRIPTION   Register recovery release events based on release tbf type .

RETURN VALUE   Boolean(state mismatch has ocuured or not)

===========================================================================*/
 void gpl1_gprs_register_release_recovery_event(release_tbf_t release_type,gas_id_t  gas_id);


/*===========================================================================

FUNCTION      gpl1_gprs_set_recovery_state

DESCRIPTION   Gl1 set recovery sub state 

RETURN VALUE   none

===========================================================================*/

void gpl1_gprs_set_recovery_state(gpl1_recovery_substate  state,gas_id_t  gas_id);


/*===========================================================================

FUNCTION      gpl1_gprs_get_recovery_sub_state

DESCRIPTION   Gl1 get recovery sub state .

RETURN VALUE   Recovery state

===========================================================================*/

gpl1_recovery_substate gpl1_gprs_get_recovery_sub_state(gas_id_t  gas_id);


/*===========================================================================

FUNCTION      gpl1_gprs_set_recovery_cnf_pending

DESCRIPTION   Gl1 set recovery cnf pending 

RETURN VALUE   none

===========================================================================*/

void gpl1_gprs_set_recovery_cnf_pending(boolean pending,gas_id_t  gas_id);


/*===========================================================================

FUNCTION      gpl1_gprs_get_recovery_cnf_pending

DESCRIPTION   Gl1 get recovery cnf pending .

RETURN VALUE   Recovery state

===========================================================================*/

boolean gpl1_gprs_get_recovery_cnf_pending(gas_id_t  gas_id);

/*===========================================================================

FUNCTION      gl1_gprs_recovery_read_nv_data

DESCRIPTION   Read the NV from EFS

RETURN VALUE

SIDE EFFECTS

===========================================================================*/
void gl1_gprs_recovery_read_nv_data(gas_id_t  gas_id);


/*===========================================================================

FUNCTION      gl1_gprs_rec_efs_enabled

DESCRIPTION   return stored recovery efs value

RETURN Boolean

SIDE EFFECTS

===========================================================================*/

uint8 gl1_gprs_rec_efs_enabled(gas_id_t  gas_id);

#endif /* FEATURE_GERAN_PTM_ERR_RECOVERY */
#endif
