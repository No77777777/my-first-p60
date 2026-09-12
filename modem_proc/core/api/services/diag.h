#ifndef DIAG_H
#define DIAG_H
/*==========================================================================

                     Diagnostic Task Header File

Description
  Global Data declarations of the diag_task.

Copyright (c) 1991-2018 by QUALCOMM Technologies, Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary

===========================================================================*/


/*===========================================================================

                         Edit History

      $Header: //components/rel/core.mpss/10.0/api/services/diag.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/28/18   vg      Supporting SubID in extended logs and msgs listener callback
05/31/18   kdey    Supporting SubID in extended event listener callback
02/01/18   sa      Deprecate duplicate APIs.
08/01/17   ph      Added new API diag_services_enabled().
05/25/17   sp      Support for DIAG Extended Log and Event APIs on UserPD
03/30/17   ph      Add support for mode change notification.
12/20/16   tbg     Added diag_generate_sync_markers API.
11/14/16   rs      Publish Diag extended API versioning defines and mapped internal defines to the externals ones.
07/08/16   gn      Unused Public API removal
02/08/16   nk      Added support for WCDMA frame number registration for extended packets
04/11/16   ph      Added support for QShrink 4.0
08/31/15   ph      Added support for buffering mode APIs.
11/04/15   ph      1x Core dependency removal from Diag.
06/29/15   rh      Added modem feature mask callback registration
06/28/13   rh      Updated to QShrink 3.0
06/18/13   ph      Added new F3 mask check API.
05/09/13   sr      Added support for diag mask change notification for 
                   registered users   
04/15/13   sr      Support for WCDMA to register with Diag to obtain the 
                   frame num  
10/12/12   ph      Removed the references of peek and poke code.
10/03/12   rh      Added diag_svc_enabled to check whether Diag has been activated 
07/24/12   sa      Support for calling listeners with MSG_SPRINTF_* F3's
06/26/12   sa      Added new extended event/log APIs to remove listener
                   with listener callback and param as arguments.
06/13/12   sa      Support for Extended F3 listeners
05/16/12   rh      Support for F3 listeners
03/26/12   hvm     Extended Event/Log Listener APIs added
03/04/11   is      Add RIVA proc ID
09/14/10   is      New control channel and central routing features 
06/20/10   sg      Added prototype to diag_early_init()
09/30/09   as      Removed inclusion of customer.h  
10/01/09   JV      Moved prototypes for diag_change_default_sio_bitrate and
                   diag_record_dm_baudrate to diagcomm.h
09/29/09   vs      Added DAIG_NO_PROC for initializing master table
07/31/09   JV      Merged Q6 diag code back to mainline
07/31/09   JV      Removed warnings by making signatues of functions uniform
07/15/09   mad     Featured out inclusion of sio.h. Featured out 
                   diag_change_default_sio_bitrate and diag_record_dm_baudrate
07/14/09   mad     Featurized inclusion of customer.h
07/10/09   as      Mainlined code under FEATURE_HTORPC_METACOMMENTS and 
                   removed support for FEATURE_DIAG_SPC_TTL.
06/11/09   JV      Featurized inclusion of customer.h to ease merging of the 
                   Q6 branch
05/07/09   vk      removed dependencies from diag_v.h 
12/12/08   vg      split public and private information diag.h and diag_v.h
10/03/08   vg      Update configuration to enable msm 8200, 2 modem processors
12/22/06   as      Moved proc ID macros to diag.h
12/05/06   as      Added signal for Diag drain timer
11/21/06   as      Moved DIAG internal features from diagi.h to diag.h
03/30/05   sl      Added support for SPC_UNLOCK_TTL to unlock/lock the sp_state
10/17/03   ph      For Rel A builds, redirect to new MC subsystem command.
09/23/03    gr     Function prototypes related to changes that enable more
                   efficient detection of the condition where diag is out of
                   heap space.
08/20/01   jal     Changes to support more Diag packets.  Support for powerup
                   online/offline state, service programming lock state.
04/06/01   lad     Cosmetic changes.
02/23/01   lad     Rearchitected the diagnostics subsystem to be a service 
                   rather than a module.  All coupling to targets has been
                   moved to target-specific implementations.  This file now
                   contains an API.  No other information or coupling remains
                   except for featurized legacy code that impacts existing 
                   targets.
                   Old history comments have been removed since most of them
                   are no longer applicable.

===========================================================================*/

#include "comdef.h"
#include "qw.h"
#include "core_pragmas.h"
#include "msg_diag_service.h" /* For msg_ext_store_type, msg_qsr_store_type */


#define DIAG_MODEM_PROC  0
#define DIAG_APP_PROC    1
#define DIAG_COMMON_PROC 2
#define DIAG_DUAL_PROC   2
#define DIAG_QDSP6_PROC  3
#define DIAG_RIVA_PROC   4
#define DIAG_NO_PROC     0xFF /* Use to initialize proc_id if user doesn't specify */

#define DIAGCOMM_CTRL_NO_PORT -1 /* Used to initialize diagpkt_master_table_type.port */

//Error codes for retrieving subscription id from listener callback
#define DIAG_LISTENER_ERROR_NULLPTR    -1  /* This error is returned when a NULL pointer is passed to the API diag_listener_get_subid */
#define DIAG_LISTENER_ERROR_NOTEXT     -2  /* This error is returned when the pointer passed to the API diag_listener_get_subid is not of an extended event, log or msg */
#define DIAG_LISTENER_ERROR_INVALID    -3  /* This error is returned when the pointer of the packet passed to API diag_listener_get_subid is not an extended event, log or msg */
#define DIAG_LISTENER_ERROR_OTHER_TASK -4  /* This error is returned when the API diag_listener_get_subid is called from some task other than diag task. */


/* ---------------------------------------------------------------------------
** Security states.  Some packets are restricted based on security state.
** -------------------------------------------------------------------------*/
#define DIAG_SEC_UNLOCKED 0x00  /* Packet accepted if security is unlocked */
#define DIAG_SEC_LOCKED   0x01  /* Packet accepted if security is locked   */
#define DIAG_SEC_ANY      0xFF  /* Any security */

#define DIAG_SPC_UNLOCKED 0x00  /* SPC has been entered, valid service   */
                   /* programming code has been entered.    */
#define DIAG_SPC_LOCKED   0x01  /* SP is locked, no valid service        */
                   /* programming code has been entered yet */

#ifdef __cplusplus
extern "C"
{
#endif

/*The following names are used while calling RC event APIs 
to notify clients about mask change*/
#define DIAG_EVENT_MASK_CHANGE_NOTIFY "diag:event_mask_change"
#define DIAG_LOG_MASK_CHANGE_NOTIFY "diag:log_mask_change"
#define DIAG_MSG_MASK_CHANGE_NOTIFY "diag:msg_mask_change"

/*This is used while calling RC event APIs to notify clients 
about tx mode change. */
#define DIAG_TX_MODE_CHANGE_NOTIFY "diag:tx_mode_change"

/* The following are different modes that diag can set to*/

#define DIAG_TX_MODE_STREAMING         0    /* Streaming mode (default) */
#define DIAG_TX_MODE_BUFFERED_THRESH   1    /* Threshold buffering mode */
#define DIAG_TX_MODE_BUFFERED_CIR      2    /* Circular buffering mode */
#define DIAG_TX_MODE_INVALID           0xFF /* Invalid mode */
#define DIAG_TX_MODE_LOCKED            0xFE /* Invalid mode */
#define DIAG_TX_MODE_LAST              2

/* The following are bit mask for various diag services which are used 
in diag_services_enabled API. */
#define DIAG_SERVICE_NONE                   0x0
#define DIAG_SERVICE_MSG                    0x1
#define DIAG_SERVICE_LOG                    0x2
#define DIAG_SERVICE_EVENT                  0x4
#define DIAG_SERVICE_MSG_DCI                0x8
#define DIAG_SERVICE_LOG_DCI                0x10
#define DIAG_SERVICE_EVENT_DCI              0x20
#define DIAG_SERVICE_LOG_LISTEN_S           0x40
#define DIAG_SERVICE_LOG_LISTEN_EXT_S       0x80
#define DIAG_SERVICE_EVENT_LISTEN_S         0x100
#define DIAG_SERVICE_EVENT_LISTEN_EXT_S     0x200
#define DIAG_SERVICE_F3_LISTEN_S            0x400
#define DIAG_SERVICE_F3_LISTEN_EXT_S        0x800
#define DIAG_SERVICE_F3_TRACE               0x1000


/* This structure is sent to an event listener when an event is processed.
   The implementation relies on the format of this structure.  It is 
   dangerous to change this format of this structure. */
typedef struct
{
  unsigned int event_id;    /* event ID */

  qword ts;         /* 8-byte CDMA time stamp. */

  uint8 length;     /* length of the payload */
  /* Payload of size 'length': */
  uint8 payload[255];       /* payload, if length > 0 */

}
diag_event_type;

/* This enum signifies whether the extended packet is an event, log or msg 
   This enum is used as an argument in the API diag_listener_get_subid*/
   
typedef enum
{
	DIAG_EXT_EVENT = 0,
	DIAG_EXT_LOG   = 1,
	DIAG_EXT_MSG   = 2
}diag_ext_pkt_type;


typedef union
{
  /*cmd_code = DIAG_EXT_MSG_F */
  msg_ext_type       * ext_sprintf_msg;
  /* cmd_code = DIAG_EXT_MSG_TERSE_F */
  msg_ext_store_type * ext_msg;
  /* cmd_code = DIAG_QSR_EXT_MSG_TERSE_F
     You will need to check qsr_flag in structure to determine what it is */
  msg_qsr_store_type * qsr_msg; /* qsr_flag = QSR_BITFLAG_IS_HASH */
  /* cmd_code = DIAG_QSR_EXT_MSG_TERSE_F
     You will need to check qsr_flag in structure to determine what it is */
  msg_v3_store_type * qsr_v3_msg; /* qsr_flag = QSR_BITFLAG_IS_V3 */
   /* cmd_code = DIAG_QSR4_EXT_MSG_TERSE_F
     You will need to check qsr_flag in structure to determine what it is */ 
  msg_v4_store_type * qsr_v4_msg; /* qsr_flag = QSR_BITFLAG_IS_V4 */
}
diag_msg_store_type;

/* This structure is sent to an F3 listener when an F3 msg is processed. */
typedef struct
{
  unsigned int cmd_code; /* Specifies the type of F3 msg and union type 
                            to use from diag_msg_store_type. */
  diag_msg_store_type msg_store;
}
diag_msg_type;

typedef struct
{
  unsigned int ssid;
  unsigned int ss_mask;
}
f3_listener_ext_key_type;

/*Enum values used to specify version ID in extended log, event and msg APIs*/
typedef enum
{
  DIAG_EXT_API_VERSION_0 = 0, /*backwards compatibility case - 
                              void* pointer passed in the extended APIs
                               is considered as NULL*/
  DIAG_EXT_API_VERSION_1 = 1, /*The ID field will be supported in the header - 
                              void* pointer passed in the extended APIs
                              is considered as UINT32* where the uint32 value is ID*/   
  
  DIAG_EXT_API_VERSION_2 = 2, /* Ulogs as log packet : log packet which
                                ULog clients can use to push ULog data to the tools */

  DIAG_EXT_API_VERSION_MAX = DIAG_EXT_API_VERSION_2
} diag_ext_api_version_type;

typedef uint32(*diag_wcdma_wl1_frame_cb_type)(void);
typedef uint32(*diag_wcdma_wl1_frame_cb_ext_type)(uint32 id);

/* ---------------------------------------------------------------------------
** Service programming states.  Some packets are restricted until a valid
**  Service Programming Code is entered.
** -------------------------------------------------------------------------*/
typedef uint8 diag_sp_type;
/*~ FIELD diag_event_type.payload VARRAY 255 LENGTH diag_event_type.length */


typedef uint8 diag_security_type;

/*===========================================================================

FUNCTION DIAG_GET_SECURITY_STATE

DESCRIPTION
  This function returns the current security state of the diagnostics service.
  Some packets may be restricted by security state.  The security state is
  unlocked by sending the Security Password.


===========================================================================*/
  diag_security_type diag_get_security_state (void);


/*===========================================================================

FUNCTION DIAG_GET_SP_STATE

DESCRIPTION
  This function returns the current service programming state of the
  diagnostics service.  Some packets may be restricted by SP state.
  The SP state is unlocked by sending the Service Programming Code.


===========================================================================*/
  diag_sp_type diag_get_sp_state (void);

  
/*===========================================================================

FUNCTION DIAG_CHECK_PASSWORD

DESCRIPTION
  This function checks the given password code, and returns TRUE if it matches.
  
  If 'code_ptr' is NULL, TRUE is returned is no code has yet been written.

===========================================================================*/
boolean diag_check_password (void *code_ptr);


/*===========================================================================

FUNCTION DIAG_TIMEOUT

DESCRIPTION
  This procedure makes the diag task time out in the event of a detected 
  security breach.  This security measure is intended to prevent automated
  password and SPC hacking.
  
  The diag task flush all outbound data, timeout for 10 seconds, then flush
  the RX queue.
 
===========================================================================*/
  void diag_timeout (void);



/*!
@ingroup diag_masks_enabled
 @brief 
  This function is used to determine if Diag has been activated to process
  logs, events, and/or F3 messages.  

@sideeffects
   None

@return
  Boolean indicating the whether Diag has any logging services enabled.
  FALSE indicates that none are enabled.
  TRUE indicates that at least one feature is enabled.

@sa
   None

@deprecated
   This API is deprecated. Migrate Code to use 'diag_services_enabled' to check if diag masks are enabled.
*/
 boolean diag_masks_enabled (void)CORE_WARN_DEPRECATED_MSG("This API is deprecated", 
				"Migrate Code to use 'diag_services_enabled' to check if diag masks are enabled. ");

 

/*=========================================================================== 

FUNCTION DIAG_SERVICES_ENABLED

DESCRIPTION
  This function is used to determine if any Diag service is enabled such as 
  logs, events, f3 messages over all available streams. (Including DCI and listener)

RETURN VALUE
  Bit mask indicating the services that are enabled.

===========================================================================*/								
  uint32 diag_services_enabled (void);


/*!
@ingroup diag_f3_trace_enabled_check
 @brief 
    This routine checks if the F3 trace functionality is enabled, based on its
    current mask settings.
    
@sideeffects
   None

@return
    TRUE,  F3 trace is enabled. 
    FALSE, F3 trace is not enabled.
   
@sa
   None

@deprecated
   This API is deprecated. Migrate Code to use 'diag_services_enabled' to check if diag masks are enabled.
*/
 boolean diag_f3_trace_enabled_check(void)CORE_WARN_DEPRECATED_MSG("This API is deprecated", 
                                        "Migrate Code to use 'diag_services_enabled' to check if F3 trace is enabled. ");


/*===========================================================================

FUNCTION TYPE DIAG_IDLE_PROCESSING_FTYPE

DESCRIPTION
  A function type that, if registered, is called when the DIAG task has 
  idle processing time.

===========================================================================*/
  typedef void (*diag_idle_processing_ftype) (void *param);

/*===========================================================================

FUNCTION DIAG_IDLE_PROCESSING_REGISTER

DESCRIPTION
  This procedure registers a function pointer to be called when the DIAG 
  task has idle processing time.
  
  If multiple entries of the same entry are registered, the funciton will
  be called as many times as it is registered.
  
RETURN VALUE
  A boolean indicating success of the registration.  If no resources 
  are available, FALSE is returned.

===========================================================================*/
  boolean diag_idle_processing_register (diag_idle_processing_ftype fp,
                     void *param);

/*===========================================================================

FUNCTION DIAG_IDLE_PROCESSING_UNREGISTER

DESCRIPTION
  This procedure unregisters the specified function pointer.  The function
  will no longer be called during idle DIAG task time.
  
  This routine matches both 'fp' and 'param'.  'param' is not dereferenced.
  
  If multiple entries exist, this routine will only unregister the first
  entry found.

RETURN VALUE
  A boolean indicating success of the registration.  FALSE indicates the
  specified funciton/param pair are not registered.

===========================================================================*/
  boolean diag_idle_processing_unregister (diag_idle_processing_ftype fp,
                       void *param);


/*===========================================================================

FUNCTION TYPE DIAG_EVENT_LISTENER

DESCRIPTION
  This function type is registered via diag_add_event_listener() or
  diag_add_event_range_listener() and is called by the DIAG task when an 
  event ID matching the registered value(s) is generated by DMSS/AMSS.
  
  This routine must not hold the memory very long.  If it is held too long
  it may hinder the performance of diagnostic services.  Memory is owned
  by the DIAG task and is freed when this function returns.
  
  The sequence number is incremented each time a new log is serviced.  This
  is useful to detect duplicated when more than one listener for the same
  type is used.
  
RETURN VALUE
  None.

===========================================================================*/
  typedef void (*diag_event_listener) (uint32 seq_num, 
                                       const diag_event_type * event, 
                                       void *param);

/*===========================================================================

FUNCTION TYPE DIAG_EVENT_LISTENER_EXT

DESCRIPTION
  This function type is registered via diag_add_event_listener_ext()
  and is called by the DIAG task when an event ID matching the
  registered value(s) is generated by DMSS/AMSS.
  
  This routine must not hold the memory very long.  If it is held too long
  it may hinder the performance of diagnostic services.  Memory is owned
  by the DIAG task and is freed when this function returns.
      
RETURN VALUE
  None.

===========================================================================*/
  typedef void (*diag_event_listener_ext) (const diag_event_type * event, 
                                       void *param);  
/*~ PARAM event POINTER */

/*===========================================================================

FUNCTION DIAG_ADD_EVENT_LISTENER

DESCRIPTION
  This routine registers a function to be called by the DIAG task when the
  specified event is generated by DMSS/AMSS.
  
  'param' will be passed unmodified to the registered function when called.
  
  Duplicate entries are allowed.  
    
RETURN VALUE
  TRUE - listener was successfully added to the listener table.
  FALSE - Listener was not added due to lack of resources.

===========================================================================*/
  boolean diag_add_event_listener (const unsigned int event_id,
                   const diag_event_listener listener,
                   void *param);

/*===========================================================================

FUNCTION DIAG_ADD_EVENT_LISTENER_EXT

DESCRIPTION
  This routine registers a function to be called by the DIAG task when any
  one of the events specified by the 'events' array is generated by DMSS/AMSS.
 
  'num_events' should be the number of events present in the 'events' array
 
  'param' will be passed unmodified to the registered function when called.
  
      
RETURN VALUE
  TRUE - listener was successfully added to the listener table.
  FALSE - Listener was not added due to lack of resources.

===========================================================================*/

  boolean diag_add_event_listener_ext (const uint16 *events, const unsigned int num_events, 
                         const diag_event_listener_ext listener, void *param);

/*~ FUNCTION diag_add_event_listener
     CLNTNAME diag_add_event_listener_on_modem */

/*===========================================================================

FUNCTION DIAG_REMOVE_EVENT_LISTENER

DESCRIPTION
  This routine unregisters a function from the event listener table.
  'listener' will no longer be called when the event ID is 
    
  'event_id', 'listener', and 'param' are used to match a registered 
  listener.
  
  If duplicate entries exist, only one is removed.

RETURN VALUE
  Boolean indicating the success of the operation.  If FALSE, the entry was
  not found in the listener table.

===========================================================================*/
  boolean diag_remove_event_listener (const unsigned int event_id,
                      const diag_event_listener listener,
                      void *param);

/*===========================================================================

FUNCTION DIAG_REMOVE_EVENT_LISTENER_EXT

DESCRIPTION
  This routine unregisters a function from the event listener table.
  'listener' will no longer be called when any of the events registered
  earlier by calling diag_add_event_listener_ext() is generated 
    
  'listener' is used to match a registered listener.
  'param' used with listener while adding the listener is not used to
   match listener.
  
  If duplicate entries exist, only one is removed.

RETURN VALUE
  Boolean indicating the success of the operation.  If FALSE, the entry was
  not found in the listener table.

===========================================================================*/
  boolean diag_remove_event_listener_ext (const diag_event_listener_ext listener);

/*~ FUNCTION diag_remove_event_listener
     CLNTNAME diag_remove_event_listener_on_modem */

/*===========================================================================

FUNCTION DIAG_REMOVE_EVENT_LISTENER_EXTP

DESCRIPTION
  This routine unregisters a function from the event listener table.
  'listener' with param 'param' will no longer be called when any of the events registered
  earlier by calling diag_add_event_listener_ext() is generated 
    
  'listener' and 'param' are used to match a registered listener.
  
  If duplicate entries exist, all are removed.

RETURN VALUE
  Boolean indicating the success of the operation.  If FALSE, the entry was
  not found in the listener table.

===========================================================================*/
  boolean diag_remove_event_listener_extp (const diag_event_listener_ext listener, void* param);


/*===========================================================================

FUNCTION TYPE DIAG_LOG_LISTENER

DESCRIPTION
  This function type is registered via diag_add_log_listener() or
  diag_listen_for_log_range() and is called by the DIAG task when an 
  log code matching the register value(s) is called.
  
  This routine must not hold the memory very long.  If it is held too long
  it may hinder the performance of diagnostic services.  Memory is owned
  by the DIAG task and is freed when this function returns.
  
  The sequence number is incremented each time a new log is serviced.  This
  is useful to detect duplicated when more than one listener for the same
  type is used.
  
RETURN VALUE
  None.

===========================================================================*/
  typedef void (*diag_log_listener) (uint32 seq_num, const byte *log, 
                                     unsigned int length, void *param);

/*===========================================================================

FUNCTION TYPE DIAG_LOG_LISTENER_EXT

DESCRIPTION
  This function type is registered via diag_log_event_listener_ext()
  and is called by the DIAG task when a log ID matching the
  registered value(s) is generated by DMSS/AMSS.
  
  This routine must not hold the memory very long.  If it is held too long
  it may hinder the performance of diagnostic services.  Memory is owned
  by the DIAG task and is freed when this function returns.
      
RETURN VALUE
  None.

===========================================================================*/

  typedef void (*diag_log_listener_ext) (const byte *log, 
                                     unsigned int length, void *param);  

/*~ PARAM log VARRAY LENGTH length */
/*===========================================================================

FUNCTION DIAG_ADD_LOG_LISTENER

DESCRIPTION
  This routine registers a function to be called when the specified log
  is generated by DMSS/AMSS.
  
  'param' will be passed unmodified to the registered function when called.
  
  Duplicate entries are allowed.  
    
RETURN VALUE
  TRUE - listener was successfully added to the listener table.
  FALSE - Listener was not added due to lack of resources.

===========================================================================*/
  boolean diag_add_log_listener (const unsigned int log_code,
                 const diag_log_listener listener,
                 void *param);

/*===========================================================================

FUNCTION DIAG_ADD_LOG_LISTENER_EXT

DESCRIPTION
  This routine registers a function to be called by the DIAG task when any
  one of the logs specified by the 'logs' array is generated by DMSS/AMSS.
 
  'num_logs' should be the number of logs present in the 'logs' array
 
  'param' will be passed unmodified to the registered function when called.
  
      
RETURN VALUE
  TRUE - listener was successfully added to the listener table.
  FALSE - Listener was not added due to lack of resources.

===========================================================================*/
  boolean diag_add_log_listener_ext (const uint16* logs, const unsigned int num_logs,
                       const diag_log_listener_ext listener, void *param);

/*~ FUNCTION diag_add_log_listener
     CLNTNAME diag_add_log_listener_on_modem */

/*===========================================================================

FUNCTION DIAG_REMOVE_LOG_LISTENER

DESCRIPTION
  This routine unregisters a function from the log listener table.
  'listener' will no longer be called when the log code is 
    
  'log_code', 'listener', and 'param' are used to match a registered 
  listener.
  
  If duplicate entries exist, only one is removed.

RETURN VALUE
  Boolean indicating the success of the operation.  If FALSE, the entry was
  not found in the listener table.

===========================================================================*/
  boolean diag_remove_log_listener (const unsigned int log_code,
                    const diag_log_listener listener,
                    void *param);

/*===========================================================================

FUNCTION DIAG_REMOVE_LOG_LISTENER_EXT

DESCRIPTION
  This routine unregisters a function from the log listener table.
  'listener' will no longer be called when any of the logs registered
  earlier by calling diag_add_log_listener_ext() is generated 
    
  'listener' is used to match a registered listener.
  'param' used with listener while adding the listener is not used to
   match listener.
  
  If duplicate entries exist, only one is removed.

RETURN VALUE
  Boolean indicating the success of the operation.  If FALSE, the entry was
  not found in the listener table.

===========================================================================*/
  boolean diag_remove_log_listener_ext (const diag_log_listener_ext listener);

/*~ FUNCTION diag_remove_log_listener
     CLNTNAME diag_remove_log_listener_on_modem */

	 
/*===========================================================================

FUNCTION DIAG_REMOVE_LOG_LISTENER_EXTP

DESCRIPTION
  This routine unregisters a function from the log listener table.
  'listener' with param 'param'  will no longer be called when any of the logs registered
  earlier by calling diag_add_log_listener_ext() is generated 
    
  'listener' and 'param' are used to match a registered listener.
  
  If duplicate entries exist, all are removed.

RETURN VALUE
  Boolean indicating the success of the operation.  If FALSE, the entry was
  not found in the listener table.

===========================================================================*/
  boolean diag_remove_log_listener_extp (const diag_log_listener_ext listener, void *param);

	 
/*===========================================================================

FUNCTION TYPE DIAG_F3_LISTENER

DESCRIPTION
  This function type is registered via diag_add_f3_listener() and is called 
  by the DIAG task when an SSID matching the registered value(s) is 
  generated by DMSS/AMSS.
  
  This routine must not hold the memory very long.  If it is held too long
  it may hinder the performance of diagnostic services.  Memory is owned
  by the DIAG task and is freed when this function returns.
  
  The sequence number is incremented each time a new F3 is serviced.  This
  is useful to detect duplicated when more than one listener for the same
  type is used.
  
RETURN VALUE
  None.

WARNING
  Client should not hold onto 'msg' since it will be freed by Diag task 
  when this function returns.
  
===========================================================================*/
  typedef void (*diag_f3_listener) (uint32 seq_num, 
                                    const diag_msg_type * msg, 
                                    void *param);
      

/*===========================================================================

FUNCTION TYPE DIAG_F3_LISTENER_EXT

DESCRIPTION
  This function type is registered via diag_add_f3_listener_ext() and is called 
  by the DIAG task when an SSID matching the registered value(s) is 
  generated by DMSS/AMSS.
  
  This routine must not hold the memory very long.  If it is held too long
  it may hinder the performance of diagnostic services.  Memory is owned
  by the DIAG task and is freed when this function returns.
  
RETURN VALUE
  None.

WARNING
  Client should not hold onto 'msg' since it will be freed by Diag task 
  when this function returns.
  
===========================================================================*/
  typedef void (*diag_f3_listener_ext) (const diag_msg_type * msg, 
                                          void *param);
      

/*===========================================================================

FUNCTION DIAG_ADD_F3_LISTENER

DESCRIPTION
  This routine registers a function to be called when the specified F3
  with matching mask is generated by DMSS/AMSS.
  
  'param' will be passed unmodified to the registered function when called.
  
  Duplicate entries are allowed.  
    
RETURN VALUE
  TRUE - listener was successfully added to the listener table.
  FALSE - Listener was not added due to lack of resources.

===========================================================================*/			
  boolean diag_add_f3_listener (const unsigned int ssid,
                                const unsigned int ss_mask,
                                const diag_f3_listener listener,
                                void *param);

			
/*===========================================================================

FUNCTION DIAG_ADD_F3_LISTENER_EXT

DESCRIPTION
	This routine registers a function to be called by the DIAG Task when any of the SSID+Mask pair in the keys array 
	matches with the F3 generated by DMSS/AMSS.

	'param' will be passed unmodified to the registered function when called.
	
	Duplicate entries are allowed.	
		
RETURN VALUE
	TRUE - listener was successfully added to the listener table.
	FALSE - Listener was not added due to lack of resources.

===========================================================================*/ 		

boolean diag_add_f3_listener_ext (f3_listener_ext_key_type *keys, 
									const unsigned int num_keys,
									const diag_f3_listener_ext listener,
									void *param);
								

/*===========================================================================

FUNCTION DIAG_REMOVE_F3_LISTENER

DESCRIPTION
  This routine unregisters a function from the F3 listener table.
  'listener' will no longer be called when the F3 SSID is 
  'ssid', 'ss_mask', 'listener', and 'param' are used to match a registered 
  listener.
  
  If duplicate entries exist, only one is removed.

RETURN VALUE
  Boolean indicating the success of the operation.  If FALSE, the entry was
  not found in the listener table.

===========================================================================*/               
  boolean diag_remove_f3_listener (const unsigned int ssid,
                                   const unsigned int ss_mask,
                                   const diag_f3_listener listener,
                                   void *param);


/*===========================================================================

FUNCTION DIAG_REMOVE_F3_LISTENER_EXT

DESCRIPTION
  This routine unregisters a function from the F3 listener table.
  'listener' will no longer be called for any F3 generated by DMSS/AMSS.
  
  If duplicate entries exist, all will be removed.

RETURN VALUE
  Boolean indicating the success of the operation.  If FALSE, the entry was
  not found in the listener table.

===========================================================================*/               
  boolean diag_remove_f3_listener_ext (const diag_f3_listener_ext listener, void* param);


/*===========================================================================

FUNCTION DIAG_OTASP_UPDATE

DESCRIPTION
  This procedure tells diag to refresh its status cache from NV.

===========================================================================*/
  void diag_otasp_update (void);


/*==========================================================================

FUNCTION DIAG_EARLY_INIT

DESCRIPTION
  This function initializes the diagnostics message ,log and event services
  If the clients need to use diag apis before diag task is initialized this 
  function should be called

===========================================================================*/
  void diag_early_init(void);
 
/*==========================================================================

FUNCTION DIAG_WCDMA_WL1_FRAME_REG

DESCRIPTION
  This API will be used by WCDMA to register/unregister with Diag. The
  purpose of doing this is to avoid calling WCDMA function directly in
  Diag code.The cb registered is invoked when setting the timestamp for
  LOG/F3s for non-extended packets and also for extended packets if extended
  cb is not registered.
 
PARAMETERS
  Valid pointer - Registers the cb
  NULL - Unregisters the cb
  
DEPENDENCIES
  None

RETURN VALUE
  Current preset mask index.

===========================================================================*/
void diag_wcdma_wl1_frame_reg(diag_wcdma_wl1_frame_cb_type ptr);

/*==========================================================================

FUNCTION DIAG_WCDMA_WL1_FRAME_REG_EXT

DESCRIPTION
  This API will be used by WCDMA to register/unregister with Diag. The
  purpose of doing this is to avoid calling WCDMA function directly in
  Diag code.The cb registered is invoked when setting the timestamp for
  extened LOG/F3 packets. 
 
PARAMETERS
  Valid pointer - Registers the cb
  NULL - Unregisters the cb
  
DEPENDENCIES
  None

RETURN VALUE
  Current preset mask index.

===========================================================================*/
void diag_wcdma_wl1_frame_reg_ext(diag_wcdma_wl1_frame_cb_ext_type ptr);


/*===========================================================================

FUNCTION TYPE DIAG_GET_CUR_CDMA_INFO_CB_TYPE

DESCRIPTION
  A function type that, if registered, is called when the DIAG task needs
  to query the CDMA information from the protocol stack.
  

===========================================================================*/
typedef void (*diag_get_cur_cdma_info_cb_type) (byte* scm,
                                                byte* mobile_cai_rev, 
                                                int8* sci_p);

/*===========================================================================

FUNCTION DIAG_GET_CUR_CDMA_INFO_REG

DESCRIPTION
  This procedure registers a function pointer to be called when the DIAG 
  task needs to query the CDMA information from the protocol stack.
  
RETURN VALUE
  A boolean indicating success of the registration.  If no resources 
  are available, FALSE is returned.

===========================================================================*/
boolean diag_get_cur_cdma_info_reg (diag_get_cur_cdma_info_cb_type fp);

/*==========================================================================

FUNCTION TYPE DIAG_FEATURE_MASK_CB

DESCRIPTION
  This function type is to be used by the modem to provide a callback
  for when the PC queries a device about its feature mask.
 
PARAMETERS
  int* The function will write the length of its returned feature mask here.
  
RETURN VALUE
  Pointer to an array of feature mask bytes

===========================================================================*/
typedef byte* (*diag_feature_mask_cb) (int*);

/*==========================================================================

FUNCTION TYPE DIAG_REG_MODEM_FEATURE_MASK_CB

DESCRIPTION
  This function registers a callback (of the above type) to be called when
  responding to queries from the PC for the device feature mask.
 
PARAMETERS
  cb_ptr Pointer to the function that is being registered
  
DEPENDENCIES
  None

RETURN VALUE
  None

===========================================================================*/
void diag_reg_modem_feature_mask_cb(diag_feature_mask_cb cb_ptr);

/*==========================================================================

FUNCTION DIAG_SET_TX_MODE

DESCRIPTION
  This API sets the TX mode to one of the following :
	DIAG_TX_MODE_STREAMING
	DIAG_TX_MODE_BUFFERED_THRESH
	DIAG_TX_MODE_BUFFERED_CIR
 
PARAMETERS
	mode: Mode which user wants to set to.
  
DEPENDENCIES
  This API sets the mode only if the unlock command is sent.

RETURN VALUE
  Returns the mode that DIAG has set.
  DIAG_TX_MODE_INVALID if DIAG is unable to set the mode.
  DIAG_TX_MODE_LOCKED if API is locked.

===========================================================================*/
uint8 diag_set_tx_mode(uint8 mode);

/*==========================================================================

FUNCTION DIAG_GET_TX_MODE

DESCRIPTION
  This API gets the TX mode that DIAG is operating.

PARAMETERS
    None

DEPENDENCIES
    None

RETURN VALUE
  Returns the mode that DIAG is currently in.
  DIAG_TX_MODE_INVALID if DIAG is unable to get the mode.

===========================================================================*/
uint8 diag_get_tx_mode();

/*==========================================================================

FUNCTION DIAG_DRAIN_TX_BUFFER

DESCRIPTION
  This API flushes the existing packets in the buffer.
 
PARAMETERS
	None
  
DEPENDENCIES
  This API drains the buffer only if the unlock command is sent.

RETURN VALUE
  None
  
===========================================================================*/
void diag_drain_tx_buffer();

/*==========================================================================

FUNCTION DIAG_GENERATE_SYNC_MARKERS

DESCRIPTION
  This API generates logs and events that may be needed to post process
  the Diag data stream. For example, it can generate packets contaniing
  Qshrink4 GUID details, DiagID information, and time synchronization
  information.

PARAMETERS
	None
  
DEPENDENCIES
  Whether or not the individual packets are generated depends on whether
  they are enabled in the mask, and that there is room in their respective
  buffers. 

RETURN VALUE
  None
  
===========================================================================*/
void diag_generate_sync_markers();


/*=====================================================================================

FUNCTION diag_listener_get_subid

DESCRIPTION
  Returns the subscription ID of extended event,log or msg packet. This
  API needs to be called from the listener callback of the client.

PARAMETERS
	pkt_type   The type of extended packet received i.e. event,log or msg.
			   This is of type enum diag_ext_pkt_type.
	
	pkt        event/msg/log pointer from which sub ID need to be extracted.
	           This pointer must be same as the one received in the listener callback.

RETURN
  Returns the subscription ID of the packet in success case.
  Returns DIAG_LISTENER_ERROR_NULLPTR if NULL pointer is passed.
  Returns DIAG_LISTENER_ERROR_NOTEXT when the packet is not an extended packet.
  Returns DIAG_LISTENER_ERROR_INVALID when the packet is neither an event, log nor a msg.
  Returns DIAG_LISTENER_ERROR_OTHER_TASK when the API is called from a task other than diag task.
==========================================================================================*/


int diag_listener_get_subid(diag_ext_pkt_type pkt_type,const void* pkt);

#ifdef __cplusplus
}
#endif
#endif              /* DIAG_H  */
