#ifndef DS_DSD_EVENT_DP_H
#define DS_DSD_EVENT_DP_H
/*===========================================================================

                      D S _ D S D _ E V E N T _D P . H

DESCRIPTION


EXTERNALIZED FUNCTIONS

 Copyright (c) 2011 - 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/
/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/3gpp/dsd/src/ds_dsd_event_dp.h#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who    what, where, why
--------    ---    ----------------------------------------------------------
11/11/11    sk    Created Module
===========================================================================*/

/*===========================================================================

                          INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "ds_dsd_ext_i.h"

/*===========================================================================

                          PUBLIC DATA DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
    DSD system EVENTs
---------------------------------------------------------------------------*/
typedef enum
{
    DS_DSD_INVALID_EVENT = 0,

    /*-------------------------------------------------------------------------
      Basic Radio Events
    -------------------------------------------------------------------------*/
    DSD_ACQ_RADIO_EV,
    DSD_OOS_RADIO_EV,

    /*-------------------------------------------------------------------------
      Events to:
        - Throttle a system. e.g call connect in 1X throttles HDR
        - Fast Acquisition; e.g HDR in ACQ_HYST will move to HDR_IN_SVC with CC
    -------------------------------------------------------------------------*/
    DSD_CALL_CONNECTED_EV,
    DSD_CALL_ENDED_EV,
    DSD_HAND_DOWN_EV,
    DSD_HAND_DOWN_TIMER_EXPIRED_EV,
    DSD_HAND_DOWN_AVOID_TIMER_EXPIRED_EV,
    DSD_NW_DETACH_HYST_TIMER_EXPIRED_EV,

    /*-------------------------------------------------------------------------
      Voice call Events
    -------------------------------------------------------------------------*/
    DSD_VOICE_CALL_ORIG_EV,
    DSD_VOICE_CALL_CONNECTED_EV,
    DSD_VOICE_CALL_ENDED_EV,

    /*-------------------------------------------------------------------------
      Emergency call Events
    -------------------------------------------------------------------------*/
    DSD_EMERGENCY_CALL_ORIG_EV,
    DSD_EMERGENCY_CALL_CONNECTED_EV,
    DSD_EMERGENCY_CALL_ENDED_EV,

    /*-------------------------------------------------------------------------
      Voice call on other SUB Events
    -------------------------------------------------------------------------*/
    DSD_VOICE_CALL_UP_ON_OTHER_SUB_EV,
    DSD_VOICE_CALL_DOWN_ON_OTHER_SUB_EV,

    /*-------------------------------------------------------------------------
      System level throttle. System level throttling is done in CM through
      DCTM. This information comes to DSD layer through C2K MH. 
    -------------------------------------------------------------------------*/
    DSD_DCTM_THROTTLE_EV,
    DSD_DCTM_UNTHROTTLE_EV,

    /*-------------------------------------------------------------------------
      Network parameters changed, e.g. capability
    -------------------------------------------------------------------------*/
    DSD_NETWORK_PARM_CHANGE_EV,

    /*-------------------------------------------------------------------------
      Session open/close EMM Attach/Detach
    -------------------------------------------------------------------------*/
    DSD_NW_ATTACHED_EV,
    DSD_NW_ATTACHING_EV,
    DSD_NW_DETACHED_EV,
    DSD_EPS_CNTXT_TX_EV,

    /*-------------------------------------------------------------------------
      Timer events
    -------------------------------------------------------------------------*/
    DSD_ACQ_HYST_TIMER_EXPIRED_EV,
    DSD_OOS_HYST_TIMER_EXPIRED_EV,

    /*-------------------------------------------------------------------------
      RAT Expansion event
    -------------------------------------------------------------------------*/
    DSD_RAT_EXP_START_EV,
    DSD_RAT_EXP_END_EV,
    DSD_RAT_EXP_HYST_TIMER_EXPIRED_EV,

    /*-------------------------------------------------------------------------
      LTE extended so event
    -------------------------------------------------------------------------*/
    DSD_LTE_EX_SO_CHANGED_EV,

   /*-------------------------------------------------------------------------
      Roaming status change event
    -------------------------------------------------------------------------*/
    DSD_ROAM_STATUS_CHANGED_EV,

    /*-------------------------------------------------------------------------
      If the 1x network blocks the call retries then exclude 1x from recomputation.
      This information comes to DSD layer through C2K MH. 
    -------------------------------------------------------------------------*/
    DSD_RETRY_DELAY_THROTTLE_EV,
    DSD_RETRY_DELAY_UNTHROTTLE_EV,
    /*Attach pending*/
    DSD_LTE_ATTACH_PENDING_EV,
#ifdef FEATURE_LAPP
    /*-------------------------------------------------------------------------
     5G Acquired and Lost events
     -------------------------------------------------------------------------*/
    DSD_5G_ACQUIRED_EV,
    DSD_5G_LOST_EV,

    DS_DSD_MAX_EVENT = DSD_5G_LOST_EV
#else
    DS_DSD_MAX_EVENT = DSD_LTE_ATTACH_PENDING_EV
#endif /* FEATURE_LAPP */
}dsd_event_type;


/*---------------------------------------------------------------------------
    DSD system Clients. These are the RADIOs
---------------------------------------------------------------------------*/
typedef enum
{
  DS_DSD_CLIENT_INVALID = 0,
  DS_DSD_CLIENT_FMC,
  DS_DSD_CLIENT_1X,
  DS_DSD_CLIENT_HDR,
  DS_DSD_CLIENT_LTE,
  DS_DSD_CLIENT_GSM,
  DS_DSD_CLIENT_TDSCDMA,
  DS_DSD_CLIENT_WCDMA,
  DS_DSD_CLIENT_WLAN,
#ifdef FEATURE_LAPP
  DS_DSD_CLIENT_5G,
  DS_DSD_CLIENT_MAX=DS_DSD_CLIENT_5G
#else
  DS_DSD_CLIENT_MAX=DS_DSD_CLIENT_WLAN
#endif /* FEATURE_LAPP */
}dsd_event_client_type;

/*===========================================================================

                        PUBLIC FUNCTION DECLARATIONS

===========================================================================*/

/*---------------------------------------------------------------------------
    DSD Client Registers a callback function with Event Dispatcher
---------------------------------------------------------------------------*/
typedef boolean (*ds_dsd_event_notify_func_type)(
                                    dsd_event_type              notified_event,
                                    dsd_event_client_type       client,
                                    dsd_radio_type              source_radio,
                                    ds3gsubsmgr_subs_id_e_type  subs_id,
                                    void                       *aux_info );


/*===========================================================================
FUNCTION      DS_DSD_EVENT_DP_INPUT

DESCRIPTION   This function gets events from: 
                    - Timers
                    - Registered callbacks (e.g. CM, Mode handlers, HDR CP etc)

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  Dispatch event to registered clients
===========================================================================*/
void ds_dsd_event_dp_input
(
  dsd_event_type              event_input,
  dsd_radio_type              source_radio,
  ds3gsubsmgr_subs_id_e_type  subs_id,
  void                       *aux_info
);

/*===========================================================================
FUNCTION      DS_DSD_EVENT_DP_INIT

DESCRIPTION   Called at initialization.  

DEPENDENCIES  None.

RETURN VALUE  None.

SIDE EFFECTS  Queue is defined and initialized.
===========================================================================*/
void ds_dsd_event_dp_init(void);

/*===========================================================================
FUNCTION      DS_DSD_EVENT_DP_REG_CLIENT

DESCRIPTION   Called by clients to register to get event notification.  

DEPENDENCIES  None.

RETURN VALUE  TRUE: if registration successful 
                    if client previously registgered
 
              FALSE: if registration unsuccessful
 
SIDE EFFECTS  Registered client is entered on a queue
===========================================================================*/
boolean ds_dsd_event_dp_reg_client
(
  dsd_event_client_type           dsd_client, 
  ds_dsd_event_notify_func_type   client_notify_func, 
  dsd_radio_type                  source_radio
);

/*===========================================================================
FUNCTION      DS_DSD_EVENT_DP_DEREG_CLIENT

DESCRIPTION   Called by clients to de-register for event notification.  

DEPENDENCIES  None.

RETURN VALUE  TRUE: if de-registration successful 
                    if client previously de-registgered
 
              FALSE: if de-registration unsuccessful
 
SIDE EFFECTS  De-registered client is dequeued 
===========================================================================*/
boolean ds_dsd_event_dp_dereg_client
(
  dsd_event_client_type dsd_client
);

#endif /* DS_DSD_EVENT_DP_H */
