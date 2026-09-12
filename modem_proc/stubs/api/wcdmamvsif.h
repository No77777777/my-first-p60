#ifndef WCDMAMVSIF_H
#define WCDMAMVSIF_H

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //commercial/MPSS.TA.3.0.c1/Main/modem_proc/stubs/api/wcdmamvsif.h#3 $ $DateTime: 2021/07/08 09:57:42 $ $Author: tfong $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
03/28/16   kc       Adding loopback mode 4 changes
07/07/15   pr       Pass as_id to MVS instead of the wsub id.
04/08/15   sa       Made changes to support FR25951_SingleVoiceSession
03/10/15   aa       Duplicated L2 task related globals and removed AS_ID hardcodings
03/06/14   pr       L1 support for MVS voice packet exchange.
10/17/12   pkg      Added support for WCDMA Tx Power Indication API for GPS
07/24/12   geg      Upmerge WCDMA.MPSS.1.0 to WCDMA.MPSS.2.0
03/19/12   geg      Cleanup W API files and move internal modem APIs to internal files
10/18/10   ssg      Added changes for new WCDMA-L2 interface.
07/09/10   grk      1. Changes to indicate UL ISR to send RESET_ACK only after 
                       finishing UL RESET procedure.
                    2. Introduced new command to send RESET_ACK for duplicate RESETs.
02/23/10   grk      Modified prototype of DSM WM callback for CMI compliance.
02/22/10   prk      Changed structure defintions for timer optimization in CS over HS.
12/05/09   rmsd     Removed redefinition of rlc_lc_id_type. It is already defined in uecomdef.h
12/04/09   ssg      New L2 interface.
10/01/09   rmsd     CMI change:: Replaced dsmutil.h with dsm.h
09/09/09   rmsd     Removed inclusion of customer.h 
08/06/09   rm       Mainlined all features as part of CMI Phase 2-defeaturization effort.
07/01/09   rm       Updated rlc_cipher_algo_e_type with uecomdef_wcdma_cipher_algo_e_type (Snow 3G changes)
06/01/09   vk       Updated rlc_cipher_algo_e_type with UEA_2 for Snow3G.
03/12/09   rm       Moved MAC RRC "ehs" structures to public.
03/02/09   rm       Initial Release
===========================================================================*/
#include "comdef.h"
#include "dsm.h"
#include "sys.h"

#define MAX_DL_LOGICAL_CHANNEL  24
#define MAX_UL_LOGICAL_CHANNEL  24


/* -------------------------------------- */
/* PUBLIC API section of mcalwcdma_dec.h  */
/* -------------------------------------- */

/** Enum type definition for TTI enum type */
typedef enum {
  L1_TTI_10MS, /**< 10 ms TTI */
  L1_TTI_20MS, /**< 20 ms TTI */
  L1_TTI_40MS, /**< 40 ms TTI */
  L1_TTI_80MS, /**< 80 ms TTI */
  L1_NUM_TTI   /**< Total number oif TTI types */
} mcalwcdma_dec_tti_enum_type;
typedef uint8 rlc_lc_id_type;
/* -------------------------------------- */
/* PUBLIC API section of l1sapcommon.h    */
/* -------------------------------------- */

/* Enum type definition for TTI enum type */
typedef mcalwcdma_dec_tti_enum_type  l1_tti_enum_type;

typedef enum
{
  E_TTI_2MS,
  E_TTI_10MS,
  NUM_E_TTI
} e_tti_enum_type;

typedef enum
{
  RLC_WM_REQ_SUCCESS = 0,
  RLC_WM_REQ_FAIL_LC_NOT_REG,
  RLC_WM_REQ_INVALID_RSP
}rlc_wm_reg_rsp_status;

/****************************************************************************
 * WCDMA COMMON INTERFACE DEFINITION                                        *
 ****************************************************************************/

/* WCDMA AMR transport channels class type.*/
#define WCDMA_ICOMMON_CHAN_CLASS_TYPE_NONE ( 0x20020023 )
  /*< no channels, used for de-registration. */

#define WCDMA_ICOMMON_CHAN_CLASS_TYPE_A  ( 0x20020024 )
  /*< only class A logical channel. */

#define WCDMA_ICOMMON_CHAN_CLASS_TYPE_AB ( 0x20020025 )
  /*< only class A and B logical channels. */

#define WCDMA_ICOMMON_CHAN_CLASS_TYPE_ABC ( 0x20020026 )
  /*< all class A, B and C logical channels. */

/*
 Event callback provided by a client of the server.

 The client returns WCDMA_EOK from the callback when the client was
 successful in queuing the event to its own thread context for
 processing.
 All other error codes indicate a problem.
 The event parameter pointer is valid only within the callback context.
*/
typedef uint32 ( *wcdma_icommon_event_callback_fn_t ) (

  sys_modem_as_id_e_type asid,
    /**< Modem Active Subscription ID. */

  uint32 event_id,
    /**< Event ID. See WCDMA_IXXX_EVENT_YYY. */

  void* params,
    /**< Pointer to an associated event data structure. */

  uint32 size
    /**< Size of the event data structure. */

);

/****************************************************************************
 WCDMA VOCODER INTERFACE DEFINITION                                         
 ****************************************************************************/

/*WCDMA Vocoder types.*/
#define WCDMA_IVOCODER_ID_AMR ( 0x20020020 )
/* Adaptive Multi-Rate (AMR) vocoder format. */

#define WCDMA_IVOCODER_ID_AMRWB ( 0x20020021 )
/* Adaptive Multi-Rate Wideband (AMR-WB) vocoder format. */

#define WCDMA_IVOCODER_ID_NONE ( 0x20020022 )
/* Adaptive Multi-Rate Wideband (AMR-WB) vocoder format. */

/****************************************************************************
 WCDMA AMR VOCODER INTERFACE DEFINITION                                    
 ****************************************************************************/

/*
 AMR vocoder codec modes. This denotes all the AMR codec bit-rates.
 Reference: 3gpp 26.201
 */
 #define WCDMA_IVOCAMR_CODEC_MODE_0475 ( 0x20020000 )
   /*< 4.75 kbps. */

 #define WCDMA_IVOCAMR_CODEC_MODE_0515 ( 0x20020001 )
   /*< 5.15 kbps. */

 #define WCDMA_IVOCAMR_CODEC_MODE_0590 ( 0x20020002 )
   /*< 5.90 kbps. */

 #define WCDMA_IVOCAMR_CODEC_MODE_0670 ( 0x20020003 )
   /*< 6.70 kbps. */

 #define WCDMA_IVOCAMR_CODEC_MODE_0740 ( 0x20020004 )
   /*< 7.40 kbps. */

 #define WCDMA_IVOCAMR_CODEC_MODE_0795 ( 0x20020005 )
   /*< 7.95 kbps. */

 #define WCDMA_IVOCAMR_CODEC_MODE_1020 ( 0x20020006 )
   /*< 10.20 kbps. */

 #define WCDMA_IVOCAMR_CODEC_MODE_1220 ( 0x20020007 )
   /*< 12.20 kbps. */

/****************************************************************************
   WCDMA AMR-WB VOCODER INTERFACE DEFINITION                                    
 ****************************************************************************/

/*
 AMR-WB vocoder rate types. This denotes all the AMR-WB codec bit-rates.
 Reference: 3gpp 26.201
 */
#define WCDMA_IVOCAMRWB_CODEC_MODE_0660 ( 0x20020008 )
  /*< 6.60 kbps.*/

#define WCDMA_IVOCAMRWB_CODEC_MODE_0885 ( 0x20020009 )
  /*< 8.85 kbps. */

#define WCDMA_IVOCAMRWB_CODEC_MODE_1265 ( 0x2002000A )
  /*< 12.65 kbps. */

#define WCDMA_IVOCAMRWB_CODEC_MODE_1425 ( 0x2002000B )
  /*< 14.25 kbps. */

#define WCDMA_IVOCAMRWB_CODEC_MODE_1585 ( 0x2002000C )
  /*< 15.25 kbps. */

#define WCDMA_IVOCAMRWB_CODEC_MODE_1825 ( 0x2002000D )
  /*< 18.25 kbps. */

#define WCDMA_IVOCAMRWB_CODEC_MODE_1985 ( 0x2002000E )
  /*< 19.85 kbps. */

#define WCDMA_IVOCAMRWB_CODEC_MODE_2305 ( 0x2002000F )
  /*< 23.05 kbps. */

#define WCDMA_IVOCAMRWB_CODEC_MODE_2385 ( 0x20020010 )
  /*< 23.85 kbps. */
#define WCDMA_IVOCAMRWB_CODEC_MODE_UNDEF ( 0x200200FF )

typedef struct {

  uint32* ret_handle;
   /*
      Returns the handle that the client must use when making subsequent
      commands.
     */

  sys_modem_as_id_e_type asid;
    /*< Modem Active Subscription ID. */

  wcdma_icommon_event_callback_fn_t event_cb;
    /*<
       Central event callback function, which receives asynchronous events
       from the server.

       Operational contract:

       - The client may only queue the incoming event and signal a worker
       thread to process the event. The client must not perform any other
       processing in the callback context.

       - The client may not call any APIs on the WCDMA protocol software in the
       callback context. This will cause synchronization issues for the driver
       and may lead to a system failure or deadlock.

       - The client may not perform any blocking operations or acquire any
       locks in the event callback context that lead to a system deadlock.

       - The client may spend no more than 5 us while in the callback
       context.

       - It is highly recommended to use atomic operations for
       synchronization needs.

       Failure to meet the operational contract may lead to an undefined
       state with system stability and performance issues.
      */
}wcdma_ivoice_cmd_open_t;


/* Close command (Synchronous Blocking API).

  Use this command to close the previously opened WCDMA voice session
  instance.
  The client may not use the open handle after receiving successful close
  indication.
  Upon a successful close, the command shall return WCDMA_EOK to the caller.
 */

typedef struct {

  uint32 handle;
    /*< Open handle. */
}wcdma_ivoice_cmd_close_t;


/* codec mode event (asynchronous event).

  The WCMDA protocol software sends this event as part of the voice traffic
  set up.

  If the WCDMA protocol software provides the codec mode information before
  selecting vocoder type, the client shall cache the codec mode settings,
  and then reapply when vocoder is available.
  If the codec mode information is not provided, then the default codec mode
  corresponding to the active vocoder shall be used.
  The WCDMA protocol software may also send this event to select a new codec
  mode during an active voice call.
 */

#define WCDMA_IVOICE_EVENT_REQUEST_CODEC_MODE ( 0x20020017 )



typedef struct  {

  uint32 codec_mode;
    /*< codec mode, See WCDMA_IVOCXXX_CODEC_MODE_YYY. */
}wcdma_ivoice_event_request_codec_mode_t;


/* SCR mode event (asynchronous event).
  The WCMDA protocol software sends this event as part of the voice traffic
  set up.
  If the WCDMA protocol software provides the SCR mode information before
  selecting vocoder type, the client shall cache the SCR mode information, and
  then reapply when vocoder is available.
  If the SCR mode information is not provided, then the default SCR mode
  corresponding to the active vocoder shall be used.
  The WCDMA protocol software may also send this event to select a new SCR
  mode during an active voice call.
 */

#define WCDMA_IVOICE_EVENT_REQUEST_SCR_MODE ( 0x20020018 )

typedef struct  {

  boolean enable_flag;
      /*<
         Enable flag:
         - TRUE -- Enable SCR
         - FALSE -- Disable SCR
        */
}wcdma_ivoice_event_request_scr_mode_t;


/* SET UL/DL Logical channel events (asynchronous event).
  The WCDMA protocol software "RRC" provides uplink and downlink logical
  channel ID's to the client. The client registers and links logical channel
  IDs with DSM watermark queues (3 for downlink and 3 for uplink) through RLC.
  If the logical channel ID passed is zero, then the corresponding logical
  channel is de-registered and de-linked with DSM watermark queue.
  The WCDMA protocol software sends this event for both uplink and downlink
  every time when a RAB is setup, a RAB is released, and also during link
  adaptation (codec mode changes)
 */

#define WCDMA_IVOICE_EVENT_SET_LOGICAL_CHANNELS ( 0x20020019 )

typedef struct   {

  boolean direction;
    /*<
      Direction Flag:
      - TRUE -- Uplink logical channels.
      - FALSE -- Downlink logical channels.
     */

  uint8 lc_class_a;
    /*< RLC logical channel ID channel A.  */

  uint8 lc_class_b;
    /*< RLC logical channel ID channel B.  */

  uint8 lc_class_c;
    /*< RLC logical channel ID channel C.  */

  uint32 class_type;
    /*< RLC logical channel class type  */
}wcdma_ivoice_event_set_logical_channels_t;


/* Voice traffic start request (asynchronous event).
  The WCDMA protocol software uses this event to request client to set up
  the vocoder and start packet exchange with it.
  The WCDMA protocol software will send this event only after VFR timing is
  stabilized. The WCDMA protocol software will also provide vocoder type as
  part of this event.
 */

#define WCDMA_IVOICE_EVENT_REQUEST_START ( 0x2002001B )


typedef struct  {

  uint32 vocoder_id;
    /*< vocoder Id, See WCDMA_IVOCODER_ID_XXX. */
}wcdma_ivoice_event_request_start_t;


/* Start command ( synchronous API ).
  This command shows the client's readiness for vocoder packet exchange with
  the WCDMA protocol software.
  The client sends this command to the WCDMA protocol software as an
  acknowledgement for the event WCDMA_IVOICE_EVENT_REQUEST_START.
  The WCDMA protocol software need not wait for this command, and need not
  perform any actions.
 */



typedef struct  {

  uint32 handle;
    /*< Open handle. */
}wcdma_ivoice_cmd_start_t;


/* Voice traffic stop request (asynchronous event).
  The WCDMA protocol software uses this event to request client to stop vocoder
  and the packet exchange.
  The WCDMA protocol software will send this event during scenarios such as:
  HHO, vocoder change, and in general any intra-RAT handover sceanrios.
  VFR timing reconfiguration shall not be done by the WCDMA protocol software
  until the vocoder is stopped and WCDMA_IVOICE_CMD_STOP is sent by the client.
  This event does not have any parameters.
 */
#define WCDMA_IVOICE_EVENT_REQUEST_STOP ( 0x2002001C )


/* Stop command (synchronous API.)
  This command shows the client cannot do any more vocoder packet exchange
  with the WCDMA protocol software. The client sends this command to the
  WCDMA protocol software as an acknowledgement for the event
  WCDMA_IVOICE_EVENT_REQUEST_STOP.
  The WCDMA protocol software will not do any vocoder packet exchanges
  after receiving this command from the client.
  The WCDMA protocol software will wait for this command before proceeding
  with VFR timing reconfiguration during handovers.
 */


typedef struct  {

  uint32 handle;
    /*< Open handle. */
}wcdma_ivoice_cmd_stop_t;


/* Select owner event (asynchronous event).
  The WCDMA protocol software will send this event to the client in
  scenarios (ex: SRVCC, IRAT HO) when it wants the client to take the
  ownership of the vocoder resource.
  If the vocoder resource is readily available, the voice agent will grant the
  vocoder to the client of WCDMA protocol software immediately. If the vocoder
  resource is being used by other client, then the voice agent will revoke the
  vocoder resource from the current client and grant to the WCDMA protocol
  software.
  If the vocoder resource is not available, then the voice agent would not
  grant the vocoder resource to the WCDMA protocol software.
  This event does not have any parameters.
 */
#define WCDMA_IVOICE_EVENT_SELECT_OWNER ( 0x2002001A )

/** Send sample rate command (Synchronous Blocking API).
 *
 * The client shall use this command to provide current operating mode for 
 * the configured vocoder.
 *
 * The Server shall constitute a speech codec info indication and sends it
 * to the Call Manager. The server need not be caching the sample rate information.
 * 
 * After successful processing, the command shall return WCDMA_EOK to the caller.
 */

typedef struct {

  uint32  handle;
    /**< Open handle. */

  uint32  vocoder_id;
    /**< vocoder ID, See WCDMA_IVOCODER_ID_XXX. */

  uint32  sample_rate;
    /**< Indicates current operating mode of configured vocoder.
        *    8000hz   : narrowband mode.
        *    16000hz : wideband mode.
        *
        *    In future higher operating modes might be supported.
        */
  
} wcdma_ivoice_cmd_send_sample_rate_t;

typedef struct  {

  sys_modem_as_id_e_type asid;
    /*<
       Modem Active Subscription ID.
       WL1 shall use this to identify the subscription for which the client is
       interested to get VFR notification.
      */

  boolean enable_flag;
    /*<
       TRUE: Enable VFR notification event to client.
       FALSE: Disable VFR notification event to client.
      */
}wcdma_ivoicel1_cmd_set_vfr_notification_t;


/* VFR notification event (asynchronous event).
  WCDMA L1 will send this event every 10msec which is aligned to DPDCH
  frame boundary.
  The client will use this event to pump in silence frames over uplink
  when voice traffic channels are active and vocoder resource is not
  available.
 */

#define WCDMA_IVOICEL1_EVENT_VFR_NOTIFICATION ( 0x2002001E )

typedef struct 
{
  boolean vfr_state;
    /*<
       TRUE: DPDCH time line is stable, ready for vocoder packet exchange.
       FALSE: DPDCH time line is NOT stable, STMR/TX timing is being slammed
       to the new cell during inter-frequency HHO.
      */

  uint8 cfn_value;
    /*< Connection Frame Number - range from 0 to 255. */

  uint8 stmr_tick_count;
    /*<  STMR tick count -  range from 0 to 149. */
}wcdma_ivoicel1_event_vfr_notification_t;
/*Enums for supporting MVS clients for voice packet exchange.*/
typedef void WL1_MODEM_MVS_CLIENT_DATA (void);

typedef void (*wl1_modem_timing_cb_type) (
    boolean steady_state,
                /**
     *  TRUE: DPDCH timeline is stable.
     *              Ready for vocoder packet exchange
     *  FALSE: DPDCH timeline is NOT stable. 
     *              STMR/Tx timing is being slammed to the new cell during inter-frequency HHO
     */
    uint8 cb_cfn,
   /**< Connection Frame Number - range from 0 to 255*/
    uint8 stmr_tick_count,
    /**<  STMR tick count -  range from 0 to 149 */
    void* client_data
    /**< client provided data during registration. Simply pass this back to client. */
);
typedef struct
{
  wl1_modem_timing_cb_type wl1_modem_mvs_timing_cb_ptr;
  WL1_MODEM_MVS_CLIENT_DATA *client_data_mvs;
} wl1_modem_mvs_client_type;

/*============================================================================================
                        DOWNLINK LAYER 2 COMMAND DEFINITIONS
============================================================================================*/

/* Downlink layer 2 command ID types */
typedef enum
{

  RLC_DL_REGISTER_SRVC_REQ = 0, /* Register Downlink Watermark */

  RLC_DL_DEREGISTER_SRVC_REQ, /* DeRegister Downlink Watermark */

  L2_DL_MAX_CMDS            /* Number of DL L2 Command types. must be last entry */

}l2_dl_cmd_enum_type;

/*-------------------------------------------------------------------
TYPE: rlc_lc_id_type

This defines the RLC Logical Channel ID that is used as buffer index
between RLC and RRC, MAC and RRC interfaces.
--------------------------------------------------------------------*/
 typedef struct
 {
   void  *bearer_context;                 /* Used by the caller - RLC just   */
                                          /* just passes back what ever is   */
                                          /* passed in wm_reg_ind_cb         */
 
   void (*wm_reg_ind_cb) (rlc_lc_id_type lc_id,
                         rlc_wm_reg_rsp_status   wm_status,
                         void* bearer_context);
 }rlc_dereg_cb_type;

/*---------------------------------------------------------------------------
  Type for registration details for each downlink logical channel.
---------------------------------------------------------------------------*/
typedef struct {
  rlc_lc_id_type lc_id;                 /* Unique identifier for the       */
                                        /* downlink RLC logical channel.   */
                                        /* Can have a value from 0 to 18.  */
  dsm_watermark_type  *dl_wm_ptr;       /* Watermark for storing downlink  */
                                        /* data                            */
  boolean context;                      /* Task (FALSE) or Interrupt (TRUE)*/

  void *rlc_post_rx_func_ptr_para;      /* 3rd parameter for               */
                                        /* rlc_post_rx_proc_func_ptr()     */
  void  (*rlc_post_rx_proc_func_ptr)(uint8, uint8, void *);
                                        /* The callback function that is   */
                                        /* called by RLC when data is      */
                                        /* available in the downlink queue,*/
                                        /* for further processing by upper */
                                        /* layers.This function passes     */
                                        /* logical channel id as parameter.*/
  void  *bearer_context;                /* Used by the caller - RLC just   */
                                        /* just passes back what ever is   */
                                        /* passed in wm_reg_ind_cb         */

  void (*wm_reg_ind_cb) (rlc_lc_id_type  lc_id,
                          rlc_wm_reg_rsp_status   wm_status,
                          void* bearer_context);
} rlc_dl_channel_reg_type;


/*---------------------------------------------------------------------------
  Type to register the downlink queue for storing data
  bytes received over the air and a callback function to be called each
  time data is available in the queue, for one or more downlink
  logical channels.
---------------------------------------------------------------------------*/
typedef struct {
  uint8 nchan;                          /* Number of downlink logical      */
                                        /* channels to register            */
  rlc_dl_channel_reg_type  rlc_data[MAX_DL_LOGICAL_CHANNEL];
                                        /* Registration details for each   */
                                        /* logical channel                 */
} rlc_dl_register_srvc_type;


/*---------------------------------------------------------------------------
  Type to de-register the downlink queue for storing data
  bytes received over the air and a callback function to be called each
  time data is available in the queue, for one or more downlink
  logical channels.
---------------------------------------------------------------------------*/
typedef struct {
  uint8 nchan;                          /* Number of downlink logical      */
                                        /* channels to de register         */
  rlc_lc_id_type  rlc_id[MAX_DL_LOGICAL_CHANNEL];
                                        /* De-Registration logical channel */
   rlc_dereg_cb_type  cb_info[MAX_UL_LOGICAL_CHANNEL];
} rlc_dl_deregister_srvc_type;


/* Downlink Layer 2 Command header */
typedef struct
{
  q_link_type link; /* Quaue link */

  sys_modem_as_id_e_type as_id;

  l2_dl_cmd_enum_type cmd_id; /* Command ID */

}l2_dl_cmd_hdr_type;

/* Downlink Layer 2 command data type */
typedef union
{
  /* DL Register Srvc Request */
  rlc_dl_register_srvc_type dl_reg;

  /* DL DeRegister Srvc Request */
  rlc_dl_deregister_srvc_type dl_dereg;

}l2_dl_cmd_data_type;

/* DL Layer 2 command buffer type */
typedef struct
{
  l2_dl_cmd_hdr_type    cmd_hdr;

  l2_dl_cmd_data_type   cmd_data;

}l2_dl_cmd_type;


/*===========================================================================

                      Downlink L2-FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

FUNCTION  l2_dl_get_cmd_buf

DESCRIPTION
  This function should be called to get a command buffer from downlink Layer 2
  task command free queue

DEPENDENCIES
  None.

RETURN VALUE

  Pointer to downlink L2 command buffer if one is available
  NULL  otherwise

SIDE EFFECTS

===========================================================================*/
extern  l2_dl_cmd_type  *l2_dl_get_cmd_buf
(
  void
);

/*===========================================================================

FUNCTION        l2_dl_put_cmd

DESCRIPTION
  This function should be called to send a command to downlink Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Item will be enqueued for downlink Layer 2 task and signal will be set to
  indicate that the item has been enqueued.

===========================================================================*/
extern  void  l2_dl_put_cmd

(
  /* Pointer to the command buffer */
  l2_dl_cmd_type   *cmdptr

);

/* --------------------------------- */
/* PUBLIC APIs of l2ultask.h         */
/* --------------------------------- */


/*===========================================================================
                        UPLINK LAYER 2 COMMAND DEFINITIONS
===========================================================================*/


/* Uplink layer 2 command ID types */
typedef enum
{

  RLC_UL_REGISTER_SRVC_REQ = 0,   /* Request for service registration for RRC */

  RLC_UL_DEREGISTER_SRVC_REQ,   /* DeRequest for service registration for RRC */

  L2_UL_MAX_CMDS              /* Number of Ul L2 Command types. must be last entry */

} l2_ul_cmd_enum_type;



/*---------------------------------------------------------------------------
  Type for registration details for each uplink logical channel.
---------------------------------------------------------------------------*/
typedef struct {
  rlc_lc_id_type lc_id;                  /* Unique identifier for the      */
                                         /* uplink RLC logical channel.    */
                                         /* Can have a value from 0 to 18. */
  dsm_watermark_type  *ul_wm_ptr;        /* Watermark for storing uplink   */
                                         /* data                           */
  void  *bearer_context;                 /* Used by the caller - RLC just   */
                                         /* just passes back what ever is   */
                                         /* passed in wm_reg_ind_cb         */
  void (*wm_reg_ind_cb) (rlc_lc_id_type lc_id,
                        rlc_wm_reg_rsp_status   wm_status,
                        void* bearer_context);

} rlc_ul_channel_reg_type;


/*---------------------------------------------------------------------------
  Type to register the uplink queue for storing data bytes
  to be transmitted, for one or more uplink logical channels.
---------------------------------------------------------------------------*/
typedef struct {
  uint8 nchan;                          /* Number of uplink logical        */
                                        /* channels to register            */
  rlc_ul_channel_reg_type  rlc_data[MAX_UL_LOGICAL_CHANNEL];
                                        /* Registration details for each   */
                                        /* logical channel                 */
} rlc_ul_register_srvc_type;
  

/*---------------------------------------------------------------------------
  Type to deregister the uplink queue for storing data bytes
  to be transmitted, for one or more uplink logical channels.
---------------------------------------------------------------------------*/
typedef struct {
  uint8 nchan;                          /* Number of uplink logical        */
                                        /* channels to register            */
  rlc_lc_id_type  rlc_id[MAX_UL_LOGICAL_CHANNEL];
                                        /* DeRegistration WM of logical    */
                                        /* channel                         */
  rlc_dereg_cb_type  cb_info[MAX_UL_LOGICAL_CHANNEL];

} rlc_ul_deregister_srvc_type;



/* Command header */
typedef struct
{
  q_link_type       link;       /* Quaue link */

  sys_modem_as_id_e_type as_id;

  l2_ul_cmd_enum_type   cmd_id; /* Command ID */

}l2_ul_cmd_hdr_type;

/* Uplink Layer 2 command data type */
typedef union
{
  /* UL Register Srvc Request */
  rlc_ul_register_srvc_type   ul_reg;

  /* UL DeRegister Srvc Request */
  rlc_ul_deregister_srvc_type ul_dereg;

}l2_ul_cmd_data_type;

/* UL Layer 2 command buffer type */
typedef struct
{
  l2_ul_cmd_hdr_type    cmd_hdr;

  l2_ul_cmd_data_type   cmd_data;

}l2_ul_cmd_type;
/*===========================================================================

                      UpLink L2 FUNCTION DECLARATIONS

===========================================================================*/



/*===========================================================================

FUNCTION  l2_ul_get_cmd_buf

DESCRIPTION
  This function should be called to get a command buffer from uplink Layer 2
  task command free queue

DEPENDENCIES
  None.

RETURN VALUE

  Pointer to the uplink L2 command buffer if one is available
  NULL  otherwise

SIDE EFFECTS

===========================================================================*/
extern  l2_ul_cmd_type    *l2_ul_get_cmd_buf
(
  void
);

/*===========================================================================

FUNCTION        l2_ul_put_cmd

DESCRIPTION
  This function should be called to send a command to uplink Layer 2 task.

DEPENDENCIES
  None.

RETURN VALUE
  None.

SIDE EFFECTS
  Item will be enqueued for uplink Layer 2 task and signal will be set to
  indicate that the item has been enqueued.

===========================================================================*/
extern  void     l2_ul_put_cmd

(
  /* Pointer to the command buffer */
  l2_ul_cmd_type   *cmd_ptr

);

/*===========================================================================
FUNCTION wcdma_query_tx_power_dbm

DESCRIPTION
  This function returns the greater of the max TxAGC values computed for
  the immediately previous 670ms time period and current running time period.
  If not in DCH state it will return WCDMA_QUERY_TX_POWER_DBM_NO_INFO.

DEPENDENCIES
  None

RETURN VALUE
  int16. Returns max TxAGC value which is 1 sec old on an average.
 
SIDE EFFECTS
  None
===========================================================================*/
int16 wcdma_query_tx_power_dbm( void );

/* ========================================================================================== 
FUNCTION wl1_register_for_modem_timing

DESCRIPTION
  This function will be used by MVS modules for registering their cb ptr

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_register_for_modem_timing (wl1_modem_timing_cb_type wl1_modem_timing_cb_ptr,
    /**< This is the callback function WL1 calls every 10ms that is aligned with DPDCH frame boundary */
    void* client_data
    /**< The client provided data for client's use. */
);
/* ========================================================================================== 
FUNCTION wl1_reg_dereg_voice_adapter_info

DESCRIPTION
  This function will be used to register and deregister the Audio callback from the AVS module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_reg_dereg_voice_adapter_info(sys_modem_as_id_e_type as_id, wcdma_icommon_event_callback_fn_t event_cb, boolean reg_status);
/* ========================================================================================== 
FUNCTION wl1_vfr_notify_adapter_status

DESCRIPTION
  This function will be used to set the VFR notification status from AVS module.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
extern void wl1_vfr_notify_adapter_status(sys_modem_as_id_e_type as_id, boolean enable);

extern void rrc_register_wv_adapter_info_cb
(
  wcdma_ivoice_cmd_open_t *open_cmd
);

extern void rrc_wv_session_process_start(
wcdma_ivoice_cmd_start_t *start_cmd
);

extern void rrc_wv_session_process_stop(
wcdma_ivoice_cmd_stop_t *stop_cmd
);

extern void rrc_deregister_wv_adapter_info_cb(
  wcdma_ivoice_cmd_close_t *close_cmd
);
extern void rrc_wva_send_sample_rate(
  wcdma_ivoice_cmd_send_sample_rate_t *sample_rate
);
extern void mac_register_deregister_amr_voice_cb ( sys_modem_as_id_e_type as_id, wcdma_icommon_event_callback_fn_t event_cb, boolean reg_dereg);


#endif /* WCDMAMVSIF_H*/
