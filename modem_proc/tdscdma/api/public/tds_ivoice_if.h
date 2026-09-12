#ifndef __TDS_IVOICE_IF_H__
#define __TDS_IVOICE_IF_H__

/**
  @file  tds_ivoice_if.h
  @brief This file contains vocoder definitions of the TDSCDMA Protocol
         software.
*/

/*
  ============================================================================

   Copyright (C) 2015 Qualcomm Technologies, Inc.
   All Rights Reserved.
   Confidential and Proprietary - Qualcomm Technologies, Inc.

  ============================================================================

                             Edit History

  $Header: //components/rel/tdscdma.mpss/4.3/api/public/tds_ivoice_if.h#1 $

  when      who   what, where, why
  --------  ---   ------------------------------------------------------------
  03/16/15  cdf   Created

  ============================================================================
*/


#include "comdef.h"
#include "customer.h"
#include "target.h"
#include "sys.h"
#include "msg.h"
//just include this .h for the defination of tdsl2_ul_service_register_type/tdsl2_ul_service_register_type for now. Will remove this file after the old API is not used any more.
#include "tdsl2external.h" 
#include "wcdmamvsif.h"


/*----------------------------------------------------------------------------
  Export Macros
----------------------------------------------------------------------------*/

#ifdef __cplusplus
  #define TDSCDMA_EXTERNAL extern "C"
#else
  #define TDSCDMA_EXTERNAL extern
#endif /* __cplusplus */
  /* This marks an external API that is intended for public use. */

#ifdef __cplusplus
  #define TDSCDMA_INTERNAL extern "C"
#else
  #define TDSCDMA_INTERNAL extern
#endif /* __cplusplus */
  /* This marks an internal API that is intended for internal use. */


/****************************************************************************
 * TDSCDMA VOCODER MACRO DEFINITION                                         *
 ****************************************************************************/

/**
 * TDSCDMA Vocoder types.
 */

#define TDSCDMA_IVOCODER_ID_AMR         ( 0x20030000 )
/** Adaptive Multi-Rate (AMR) vocoder format. */

#define TDSCDMA_IVOCODER_ID_AMRWB       ( 0x20030001 )
/** Adaptive Multi-Rate Wideband (AMR-WB) vocoder format. */

#define TDSCDMA_IVOCODER_ID_NONE        ( 0x20030025 )
/** Default: None vocoder format. */

/****************************************************************************
 * TDSCDMA AMR VOCODER INTERFACE DEFINITION                                    *
 ****************************************************************************/

/**
 * AMR vocoder codec modes. This denotes all the AMR codec bit-rates.
 * Reference: 3gpp 26.201
 */

 #define TDSCDMA_IVOCAMR_CODEC_MODE_0475 ( 0x20030002 )
   /**< 4.75 kbps. */

 #define TDSCDMA_IVOCAMR_CODEC_MODE_0515 ( 0x20030003 )
   /**< 5.15 kbps. */

 #define TDSCDMA_IVOCAMR_CODEC_MODE_0590 ( 0x20030004 )
   /**< 5.90 kbps. */

 #define TDSCDMA_IVOCAMR_CODEC_MODE_0670 ( 0x20030005 )
   /**< 6.70 kbps. */

 #define TDSCDMA_IVOCAMR_CODEC_MODE_0740 ( 0x20030006 )
   /**< 7.40 kbps. */

 #define TDSCDMA_IVOCAMR_CODEC_MODE_0795 ( 0x20030007 )
   /**< 7.95 kbps. */

 #define TDSCDMA_IVOCAMR_CODEC_MODE_1020 ( 0x20030008 )
   /**< 10.20 kbps. */

 #define TDSCDMA_IVOCAMR_CODEC_MODE_1220 ( 0x20030009 )
   /**< 12.20 kbps. */

/****************************************************************************
 * TDSCDMA AMR-WB VOCODER INTERFACE DEFINITION                                    *
 ****************************************************************************/

/**
 * AMR-WB vocoder rate types. This denotes all the AMR-WB codec bit-rates.
 * Reference: 3gpp 26.201
 */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_0660 ( 0x2003000A )
  /**< 6.60 kbps. */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_0885 ( 0x2003000B )
  /**< 8.85 kbps. */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_1265 ( 0x2003000C )
  /**< 12.65 kbps. */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_1425 ( 0x2003000D )
  /**< 14.25 kbps. */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_1585 ( 0x2003000E )
  /**< 15.25 kbps. */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_1825 ( 0x2003000F )
  /**< 18.25 kbps. */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_1985 ( 0x20030010 )
  /**< 19.85 kbps. */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_2305 ( 0x20030011 )
  /**< 23.05 kbps. */

#define TDSCDMA_IVOCAMRWB_CODEC_MODE_2385 ( 0x20030012 )
  /**< 23.85 kbps. */

/**
 * TDSCDMA AMR transport channels class type.
 */

#define TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_NONE        ( 0x20030021 )
  /**< no channels, used for de-registration. */

#define TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_A           ( 0x20030022 )
  /**< only class A logical channel. */

#define TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_AB          ( 0x20030023 )
  /**< only class A and B logical channels. */

#define TDSCDMA_ICOMMON_CHAN_CLASS_TYPE_ABC         ( 0x20030024 )
  /**< all class A, B and C logical channels. */

/** Undefined AMR CODEC MODE 
 */
#define TDSCDMA_IVOCAMRWB_CODEC_MODE_UNDEF          ( 0x200300FF )

/****************************************************************************
 * TDSCDMA VOICE STRUCTURE DEFINITION                                         *
 ****************************************************************************/

/**
 * Event callback provided by a client of the server.
 *
 * The client returns TDSCDMA_EOK from the callback when the client was
 * successful in queuing the event to its own thread context for
 * processing.
 *
 * All other error codes indicate a problem.
 *
 * The event parameter pointer is valid only within the callback context.
 */
typedef uint32 ( *tdscdma_icommon_event_callback_fn_t ) (

  sys_modem_as_id_e_type as_id,
    /**< Modem Active Subscription ID. */

  uint32 event_id,
    /**< Event ID. See TDSCDMA_IXXX_EVENT_YYY. */

  void* params,
    /**< Pointer to an associated event data structure. */

  uint32 size
    /**< Size of the event data structure. */

);

/** Open command (synchronous API).
 *
 * Use this command to acquire a handle to a new TDSCDMA voice session
 * instance.
 *
 * The client must use the returned handle (ret_handle) provided by
 * the Open command to execute any subsequent commands.
 *
 * The TDSCDMA protocol software sends the voice traffic channel events to the
 * client who has opened a session for a particular subscription. The client
 * will use "asid" to indicate the subscription that it is interested in
 * receiving events for.
 *
 * The client will use these events to set up voice session thereby
 * facilitating vocoder packet exchange between the TDSCDMA protocol software
 * and the client.
 *
 * The TDSCDMA protocol software "RRC" provides the logical channel IDs to the
 * client. The client registers and links logical channel IDs with DSM
 * watermark queues (3 for downlink and 3 for uplink) through RLC.
 *
 * The client enqueues the vocoder packet to DSM watermark queues every time
 * when a vocoder packet is available. The TDSCDMA protocol software dequeues
 * the packets and transmit OTA at appropriate timing synced to the network.
 * The client dequeues the vocoder packet from DSM watermark queues every time
 * it is notified by the DSM watermark non empty callback, and supplies to
 * vocoder for decoding and playback.
 *
 * Upon a successful Open, the command shall return TDSCDMA_EOK to the caller.
 *
 */
//#define TDSCDMA_IVOICE_CMD_OPEN ( 0x20030013 )

typedef struct tdscdma_ivoice_cmd_open_t tdscdma_ivoice_cmd_open_t;

struct tdscdma_ivoice_cmd_open_t {

  uint32* ret_handle;
   /**<
     * Returns the handle that the client must use when making subsequent
     * commands.
     */

  sys_modem_as_id_e_type as_id;
    /**< Modem Active Subscription ID. */

  tdscdma_icommon_event_callback_fn_t event_cb;
    /**<
      * Central event callback function, which receives asynchronous events
      * from the server.
      *
      * Operational contract:
      *
      * - The client may only queue the incoming event and signal a worker
      * thread to process the event. The client must not perform any other
      * processing in the callback context.
      *
      * - The client may not call any APIs on the TDSCDMA protocol software in
      * the callback context. This will cause synchronization issues for the
      * driver and may lead to a system failure or deadlock.
      *
      * - The client may not perform any blocking operations or acquire any
      * locks in the event callback context that lead to a system deadlock.
      *
      * - The client may spend no more than 5 us while in the callback
      * context.
      *
      * - It is highly recommended to use atomic operations for
      * synchronization needs.
      *
      * Failure to meet the operational contract may lead to an undefined
      * state with system stability and performance issues.
      */

};


/** Close command (Synchronous Blocking API).
 *
 * Use this command to close the previously opened TDSCDMA voice session
 * instance.
 *
 * The client may not use the open handle after receiving successful close
 * indication.
 *
 * Upon a successful close, the command shall return TDSCDMA_EOK to the caller.
 */
//#define TDSCDMA_IVOICE_CMD_CLOSE ( 0x20030014 )

typedef struct tdscdma_ivoice_cmd_close_t tdscdma_ivoice_cmd_close_t;

struct tdscdma_ivoice_cmd_close_t {

  uint32 handle;
    /**< Open handle. */

};


/** codec mode event (asynchronous event).
 *
 * The TDSCMDA protocol software sends this event as part of the voice traffic
 * set up.
 *
 * If the TDSCDMA protocol software provides the codec mode information before
 * selecting vocoder type, the client shall cache the codec mode settings,
 * and then reapply when vocoder is available.
 *
 * If the codec mode information is not provided, then the default codec mode
 * corresponding to the active vocoder shall be used.
 *
 * The TDSCDMA protocol software may also send this event to select a new codec
 * mode during an active voice call.
 *
 */
//#define TDSCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE ( 0x20030017 )

typedef struct tdscdma_ivoice_event_request_codec_mode_t tdscdma_ivoice_event_request_codec_mode_t;

struct tdscdma_ivoice_event_request_codec_mode_t {

  uint32 codec_mode;
    /**< codec mode, See TDSCDMA_IVOCXXX_CODEC_MODE_YYY. */

};


/** SCR mode event (asynchronous event).
 *
 * The TDSCMDA protocol software sends this event as part of the voice traffic
 * set up.
 *
 * If the TDSCDMA protocol software provides the SCR mode information before
 * selecting vocoder type, the client shall cache the SCR mode information, and
 * then reapply when vocoder is available.
 *
 * If the SCR mode information is not provided, then the default SCR mode
 * corresponding to the active vocoder shall be used.
 *
 * The TDSCDMA protocol software may also send this event to select a new SCR
 * mode during an active voice call.
 *
 */
//#define TDSCDMA_IVOICE_EVENT_REQUEST_SCR_MODE ( 0x20030018 )

typedef struct tdscdma_ivoice_event_request_scr_mode_t tdscdma_ivoice_event_request_scr_mode_t;

struct tdscdma_ivoice_event_request_scr_mode_t {

  boolean enable_flag;
      /**<
        * Enable flag:
        * - TRUE -- Enable SCR
        * - FALSE -- Disable SCR
        */

};


/** SET UL/DL Logical channel events (asynchronous event).
 *
 * The TDSCDMA protocol software "RRC" provides uplink and downlink logical
 * channel ID's to the client. The client registers and links logical channel
 * IDs with DSM watermark queues (3 for downlink and 3 for uplink) through RLC.
 *
 * If the logical channel ID passed is zero, then the corresponding logical
 * channel is de-registered and de-linked with DSM watermark queue.
 *
 * The TDSCDMA protocol software sends this event for both uplink and downlink
 * every time when a RAB is setup, a RAB is released, and also during link
 * adaptation (codec mode changes)
 *
 */
//#define TDSCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS ( 0x20030019 )

typedef struct tdscdma_ivoice_event_set_logical_channels_t tdscdma_ivoice_event_set_logical_channels_t;

struct  tdscdma_ivoice_event_set_logical_channels_t {

  boolean direction;
    /**<
     * Direction Flag:
     * - TRUE -- Uplink logical channels.
     * - FALSE -- Downlink logical channels.
     */

  rlc_lc_id_type lc_class_a;
    /**< RLC logical channel ID channel A.  */

  rlc_lc_id_type lc_class_b;
    /**< RLC logical channel ID channel B.  */

  rlc_lc_id_type lc_class_c;
    /**< RLC logical channel ID channel C.  */

  uint32 class_type;
    /**< RLC logical channel class type  */

};


/** Voice traffic start request (asynchronous event).
 *
 * The TDSCDMA protocol software uses this event to request client to set up
 * the vocoder and start packet exchange with it.
 *
 * The TDSCDMA protocol software will send this event only after VFR timing is
 * stabilized. The TDSCDMA protocol software will also provide vocoder type as
 * part of this event.
 */
//#define TDSCDMA_IVOICE_EVENT_REQUEST_START ( 0x2003001B )

typedef struct tdscdma_ivoice_event_request_start_t tdscdma_ivoice_event_request_start_t;

struct tdscdma_ivoice_event_request_start_t {

  uint32 vocoder_id;
    /**< vocoder Id, See TDSCDMA_IVOCODER_ID_XXX. */

};


/** Start command ( synchronous API ).
 *
 * This command shows the client's readiness for vocoder packet exchange with
 * the TDSCDMA protocol software.
 *
 * The client sends this command to the TDSCDMA protocol software as an
 * acknowledgement for the event TDSCDMA_IVOICE_EVENT_REQUEST_START.
 *
 * The TDSCDMA protocol software need not wait for this command, and need not
 * perform any actions.
 */
//#define TDSCDMA_IVOICE_CMD_START ( 0x20030015 )

typedef struct tdscdma_ivoice_cmd_start_t tdscdma_ivoice_cmd_start_t;

struct tdscdma_ivoice_cmd_start_t {

  uint32 handle;
    /**< Open handle. */

};


/** Voice traffic stop request (asynchronous event).
 *
 * The TDSCDMA protocol software uses this event to request client to stop
 * vocoder and the packet exchange.
 *
 * The TDSCDMA protocol software will send this event during scenarios such as:
 * HHO, vocoder change, and in general any intra-RAT handover sceanrios.
 *
 * VFR timing reconfiguration shall not be done by the TDSCDMA protocol
 * software until the vocoder is stopped and TDSCDMA_IVOICE_CMD_STOP is sent
 * by the client.
 *
 * This event does not have any parameters.
 */
//#define TDSCDMA_IVOICE_EVENT_REQUEST_STOP ( 0x2003001C )


/** Stop command (synchronous API.)
 *
 * This command shows the client cannot do any more vocoder packet exchange
 * with the TDSCDMA protocol software. The client sends this command to the
 * TDSCDMA protocol software as an acknowledgement for the event
 * TDSCDMA_IVOICE_EVENT_REQUEST_STOP.
 *
 * The TDSCDMA protocol software will not do any vocoder packet exchanges
 * after receiving this command from the client.
 *
 * The TDSCDMA protocol software will wait for this command before proceeding
 * with VFR timing reconfiguration during handovers.
 */
//#define TDSCDMA_IVOICE_CMD_STOP ( 0x20030016 )

typedef struct tdscdma_ivoice_cmd_stop_t tdscdma_ivoice_cmd_stop_t;

struct tdscdma_ivoice_cmd_stop_t {

  uint32 handle;
    /**< Open handle. */

};


/** Select owner event (asynchronous event).
 *
 * The TDSCDMA protocol software will send this event to the client in
 * scenarios (ex: SRVCC, IRAT HO) when it wants the client to take the
 * ownership of the vocoder resource.
 *
 * If the vocoder resource is readily available, the voice agent will grant the
 * vocoder to the client of TDSCDMA protocol software immediately. If the
 * vocoder resource is being used by other client, then the voice agent will
 * revoke the vocoder resource from the current client and grant to the TDSCDMA
 * protocol software.
 *
 * If the vocoder resource is not available, then the voice agent would not
 * grant the vocoder resource to the TDSCDMA protocol software.
 *
 * This event does not have any parameters.
 */
//#define TDSCDMA_IVOICE_EVENT_SELECT_OWNER ( 0x2003001A )



/****************************************************************************
 * TDSCDMA VOICEL2 INTERFACE DEFINITION                                       *
 ****************************************************************************/


/** Set VFR notification command (synchronous API).
 *
 * TDS-L2 will provide the VFR notification event to the client for the
 * ongoing traffic over respective access stratum.
 *
 * The client shall provide a valid callback to start receiving
 * the VFR timing notification.
 *
 * The client shall provide NULL callback, to stop receiving the event.
 *
 */
//#define TDSCDMA_IVOICEL2_CMD_SET_VFR_NOTIFICATION ( 0x2003001D )

typedef struct tdscdma_ivoicel2_cmd_set_vfr_notification_t tdscdma_ivoicel2_cmd_set_vfr_notification_t;

struct tdscdma_ivoicel2_cmd_set_vfr_notification_t {

  sys_modem_as_id_e_type as_id;
    /**<
      * Modem Active Subscription ID.
      *
      * TDS-L2 shall use this to identify the subscription for which the client
      * is interested to get VFR notification.
      */
  boolean enable_flag;
    /*<
       TRUE: Enable VFR notification event to client.
       FALSE: Disable VFR notification event to client.
      */
};


/** VFR notification event (asynchronous event).
 *
 * TDSCDMA MAC will send this event every 20msec which is aligned to DPDCH
 * frame boundary/TTI.
 *
 * The client will use this event to pump in silence frames over uplink
 * when voice traffic channels are active and vocoder resource is not
 * available.
 *
 */
//#define TDSCDMA_IVOICEL2_EVENT_VFR_NOTIFICATION ( 0x2003001E )

typedef struct tdscdma_ivoicel2_event_vfr_notification_t tdscdma_ivoicel2_event_vfr_notification_t;

struct tdscdma_ivoicel2_event_vfr_notification_t
{

  uint32 tds_system_time;
    /**<
      * TDS STMR time which is lower 29 bits.
      *  - bits [28:16] represents the sub frame count from TDS.
      *
      *  - bits [15:0] represents the system tick for current TDS sub frame.
      *               (512) ticks are equivalent to (50) usec.
     */

};


/** Register Uplink logical channels with watermark queues (synchronous API)
 *
 */
//#define TDSCDMA_IVOICEL2_CMD_REGISTER_UL_SERVICE ( 0x2003001F )

typedef struct tdscdma_ivoicel2_cmd_register_ul_service_t tdscdma_ivoicel2_cmd_register_ul_service_t;

struct tdscdma_ivoicel2_cmd_register_ul_service_t {

  sys_modem_as_id_e_type as_id;
  tdsl2_ul_service_register_type* service;
    /**< The caller provides a pointer to the UL service registration data structure.. */

};


/** Register Downlink logical channels with watermark queues(synchronous API)
 *
 */
//#define TDSCDMA_IVOICEL2_CMD_REGISTER_DL_SERVICE ( 0x20030020 )

typedef struct tdscdma_ivoicel2_cmd_register_dl_service_t tdscdma_ivoicel2_cmd_register_dl_service_t;

struct tdscdma_ivoicel2_cmd_register_dl_service_t {

  sys_modem_as_id_e_type as_id;
  tdsl2_dl_service_register_type* service;
    /**< The caller provides a pointer to the UL service registration data structure.. */

};

/** Send sample rate command (Synchronous Blocking API).
 *
 * The client shall use this command to provide current operating mode for 
 * the configured vocoder.
 *
 * The Server shall constitute a speech codec info indication and sends it
 * to the Call Manager. The server need not be caching the sample rate information.
 * 
 * After successful processing, the command shall return TDSCDMA_EOK to the caller.
 */
//#define TDSCDMA_IVOICE_CMD_SEND_SAMPLE_RATE ( 0x20030026 )

typedef struct {

  uint32  handle;
    /**< Open handle. */

  uint32  vocoder_id;
    /**< vocoder ID, See TDSCDMA_IVOCODER_ID_XXX. */

  uint32  sample_rate;
    /**< Indicates current operating mode of configured vocoder.
        *    8000hz   : narrowband mode.
        *    16000hz  : wideband mode.
        *
        *    In future higher operating modes might be supported.
        */
  
}  tdscdma_ivoice_cmd_send_sample_rate_t;


/*===========================================================================
FUNCTION:        tdsl2_ul_service_register

DESCRIPTION:
  This function should be called to send a L2 command to L2-UL task for 
  water mark registering or de-registering.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  Command will be enqueued in L2-UL or L2-DL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/
extern  boolean  tdsl2_ul_service_register

(
  tdsl2_ul_service_register_type *srvc_ptr
);

/*===========================================================================

FUNCTION:        tdsl2_dl_service_register

DESCRIPTION:
  This function should be called to send a L2 command to L2-DL task 
  for water mark registering or de-registering.

DEPENDENCIES:
  None.

RETURN VALUE:
  None.

SIDE EFFECTS:
  Command will be enqueued in  L2-DL task queue and a signal will be set 
  to indicate that the item has been enqueued.

===========================================================================*/
extern  boolean  tdsl2_dl_service_register

(
  tdsl2_dl_service_register_type *srvc_ptr
);

/*==============================================================================================
 FUNCTION NAME  tdsl2_set_vfr_notify
 
 DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:
None.
===============================================================================================*/
extern boolean tdsl2_set_vfr_notify 
(
  sys_modem_as_id_e_type as_id,
  boolean enable_flag
);

/*==============================================================================================
 FUNCTION NAME  tdsl2_register_deregister_ivoice_cb
 
 DEPENDENCIES:
  None.

RETURN VALUE:


SIDE EFFECTS:
None.
===============================================================================================*/
extern boolean tdsl2_register_deregister_ivoice_cb
(
  tdscdma_icommon_event_callback_fn_t event_cb, 
  boolean reg_dereg
);

/*===========================================================================

FUNCTION tdsrrc_register_tv_adapter_info_cb

DESCRIPTION
  This function is the API to register the function pointer for TDS Voice 
  adapter call back.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_register_tv_adapter_info_cb
(
  tdscdma_ivoice_cmd_open_t *open_cmd
);

/*===========================================================================

FUNCTION tdsrrc_deregister_tv_adapter_info_cb

DESCRIPTION
  This function is the API to de-register the function pointer for TDS Voice
  adapter call back.

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_deregister_tv_adapter_info_cb
(
  tdscdma_ivoice_cmd_close_t *close_cmd
);


/*===========================================================================

FUNCTION tdsrrc_tv_session_process_start

DESCRIPTION
  This function is the API to check Voice Session started

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/

void tdsrrc_tv_session_process_start
(
  tdscdma_ivoice_cmd_start_t *start_cmd
);

/*===========================================================================

FUNCTION rrc_vsession_process_stop

DESCRIPTION
  This function is the API to check Voice Session stopped

DEPENDENCIES


RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void tdsrrc_tv_session_process_stop
(
  tdscdma_ivoice_cmd_stop_t *stop_cmd
);

/*===========================================================================

FUNCTION tdsrrc_tv_send_sample_rate

DESCRIPTION

  This function is the API to check and notified the voice sampling rate.

DEPENDENCIES


RETURN VALUE

  None

SIDE EFFECTS

  None

===========================================================================*/
void tdsrrc_tv_send_sample_rate(
  tdscdma_ivoice_cmd_send_sample_rate_t *sample_rate_cmd
);

#endif /* __TDS_IVOICE_IF_H__ */

