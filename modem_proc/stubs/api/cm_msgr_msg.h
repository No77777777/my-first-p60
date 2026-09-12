#ifndef CM_MSGR_MSG_H
#define CM_MSGR_MSG_H
/** Enumeration of the reason for doing access control on modem
*/
typedef enum cm_ac_reason_e{

    CM_AC_REASON_NONE = -1,
    /**< FOR INTERNAL CM USE ONLY! */

    CM_AC_REASON_UNUSED_PPP = 0,
    /**< Clients start access control for PPP cleaning. */

    CM_AC_REASON_MAND_PDN_DISABLED = 1,
    /**< Clients start access control for PDN disabling. */

    CM_AC_REASON_UNSAFE_TEMP = 2,
    /**< Clients start access control due to unsafe temperature. */

    CM_AC_REASON_THIRD_PARTY_IMS_E911 = 3,
    /**< Clients start access control due to 3rd part IMS E911. */

    CM_AC_REASON_THIRD_PARTY_IMS_E911_ECBM = 4,
    /**< Clients start access control due to 3rd part IMS E911. */

    CM_AC_REASON_THIRD_PARTY_IMS_E911_WRONG_STATE = 5,
    /**< Clients failed to change access contrl due to wrong state */

    /** @cond
    */

    CM_AC_REASON_MAX
    /**< FOR INTERNAL CM USE ONLY! */

    /** @endcond
    */
}cm_ac_reason_e_type;

typedef struct 
{
  msgr_hdr_struct_type hdr;


  sys_modem_as_id_e_type   asubs_id;
    /**< For DSDS consideration */

  cm_ac_reason_e_type reason;
    /**< Reason for restricting certain services */

  void  *user_data;

} cm_ac_info_ind_s_type;

/* Emergency mode status information will be sent as
** 1. EMERGENCY_MODE_STARTED on emergency call origination 
**    if UE is not in emergency mode already
** 2. EMERGENCY_MODE_STARTED on third party e911 call origination
**    if UE is not in emergency mode already
** 3. EMERGENCY_MODE_ENDED on emergency call end if 
**    UE is not in ECBM.
** 4. EMERGENCY_MODE_ENDED on ECBM exit. If ECBM exit is due to second
**    emergency call origination, EMERGENCY_MODE_ENDED will not be sent 
** 5. EMERGENCY_MODE_ENDED on third party e911 call end.
** 6. EMERGENCY_MODE_ENDED for the above cases 3,4,5 is sent only if UE is 
**    in emergency mode.
*/
typedef struct
{

  msgr_hdr_s		 msg_hdr;
  /* Message header */

  sys_modem_as_id_e_type   asubs_id;  
  /* Active Subscription */

  cm_emerg_mode_status_e_type 	 emerg_mode_status; 
  /* Emergency mode start and end status*/
  
  sys_sys_mode_e_type sys_mode;
  /* Sys mode of the emergency mode when the call is in connect state.
  ** Note: Clients should only make use of this sys_mode when the emerg_mode_status is CM_EMERGENCY_MODE_CONNECTED
  */
  
}cm_emerg_mode_status_ind_s_type;



/*! @brief UMIDs of the messages sent by the other modules to CM
*/
#define MM_CM_AC_EMERGENCY_ENTER_IND 0
#define MM_CM_AC_EMERGENCY_EXIT_IND 1
#endif /* CM_MSGR_MSG_H */

