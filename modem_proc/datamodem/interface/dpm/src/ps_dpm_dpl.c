/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                             P S _ D P M _ DPL . C

GENERAL DESCRIPTION
  This is the implementation of the Data Path Manager module

Copyright (c) 2013 QUALCOMM Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                            EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/dpm/src/ps_dpm_dpl.c#2 $
  $Author: pwbldsvc $ $DateTime: 2019/10/22 02:53:17 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
01/08/14    bvd    created file.
===========================================================================*/
/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

                                INCLUDE FILES

=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
#include "comdef.h"
#include "customer.h"
#include "dsm.h"
#include "ps_svc.h"
#include "ps_logging.h"
#include "ps_iface.h"
#include "ps_logging_helper.h"
#include "ds_Utils_DebugMsg.h"
#include "ps_dpmi.h"
#include "ps_dpm_dpl.h"
#ifdef FEATURE_DATA_IPA
#include "ipa.h"
#include "ipa_ipfltr.h"
#endif /* FEATURE_DATA_IPA */
#include "dsm.h"
#include "ps_dpm_opt_defs.h"
#include "ps_rm_svc.h"

/*===========================================================================
                            LOCAL DEFINITIONS
===========================================================================*/
#define PS_DPM_DPL_WM_DEFAULT_LO   175000 //10ms*100Mbps
#define PS_DPM_DPL_WM_DEFAULT_HI   350000 //20ms*100Mbps
#define PS_DPM_DPL_WM_DEFAULT_DNE  400000 //HI+50k
/*Discard  received DSM item when DNE packet count hit*/
#define PS_DPM_DPL_WM_DNE_PKT_COUNT  500 

/* Timer interval for DPL log timer */
#define DIAG_DPL_LOG_TIME_INTERVAL 1000

/* Bit mask indicating if logged by timer */
#define DPL_META_SIO_CONFIG_TIMER_MASK 0x20

/* Used to store DPL for sio config */
typedef struct
{
  q_link_type     link;
  dpl_sio_config  dpl_sio_conf;
}ps_dpm_dpl_sio_cached_info_type;

/* Used to store DPL for skip header bytes */
typedef struct
{
  q_link_type  link;
  uint8        mux_id;
  uint8        src_endp;
  uint8        hdr_bytes;
  uint8        direction;
}ps_dpm_dpl_skip_hdr_cached_info_type;

dsm_watermark_type ps_dpm_dpl_wmk;
dsm_watermark_type ps_dpm_dpl_dropped_pkt_wmk;


q_type dpm_dpl_q;
q_type dpm_dpl_dropped_pkt_q;

/* Timer handle to periodically commit DPL log */
ps_timer_handle_type          ps_dpm_dpl_log_timer_handle;
/* Queues to have list of DPL logs */
q_type                        ps_dpm_dpl_sio_conf_log_info_q;
q_type                        ps_dpm_dpl_skip_hdr_log_info_q;


/*===========================================================================
FUNCTION   PS_DPM_DPL_WMK_NON_EMPTY_FUNCTION()

DESCRIPTION
  This function is called whenever IPA DPL watermark becomes non-empty. 

DEPENDENCIES
  None.

PARAMETERS - FIX
  dsm_watermark_type: the Watermark  that is handled
  user_data_ptr: NOT USED

RETURN VALUE
..None

SIDE EFFECTS
  None.
===========================================================================*/
static void  ps_dpm_dpl_wmk_non_empty_function
(
  dsm_watermark_type  * dpm_dpl_wmk,
  void                * user_data_ptr
)
{
  PS_SET_EXT1_SIGNAL(PS_DPM_DPL_SIGNAL);
} /* ps_dpm_dpl_wmk_non_empty_function() */


/*===========================================================================
FUNCTION   PS_DPM_DPL_DROPPED_PKT_WMK_NON_EMPTY_FUNC()

DESCRIPTION
  This function is called whenever DPL dropped packet watermark becomes non-empty. 

DEPENDENCIES
  None.

PARAMETERS - FIX
  dsm_watermark_type: the Watermark  that is handled
  user_data_ptr: NOT USED

RETURN VALUE
..None

SIDE EFFECTS
  None.
===========================================================================*/
static void  ps_dpm_dpl_dropped_pkt_wmk_non_empty_func
(
  dsm_watermark_type  * dpm_dpl_wmk,
  void                * user_data_ptr
)
{
  PS_SET_EXT1_SIGNAL(PS_DPM_DPL_DROPPED_PKT_SIGNAL);
} /* ps_dpm_dpl_wmk_non_empty_function() */


/*===========================================================================
FUNCTION   PS_DPM_DPL_DNE_CB_FUNCTION()

DESCRIPTION
  This function is invoked  when DNE count is reached in enqueue() operation.

DEPENDENCIES
  None.

PARAMETERS - 
  dsm_watermark_type: RX watermark pointer
  dsm_pkt_ptr : Origional packet pointer

RETURN VALUE
..None

SIDE EFFECTS
  None.
===========================================================================*/
static void  ps_dpm_dpl_dne_cb_function
(
  dsm_watermark_type  * ul_wmk_ptr,
  void                * dsm_pkt_ptr
)
{
  dsm_item_type   *item_ptr     = (dsm_item_type *)dsm_pkt_ptr;
  dsm_item_type   *dup_item_ptr = NULL;
  uint16           dup_len      = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
     Step 1: Dup the packet
     Setp 2: Update the APP field to RM watermark. Single Handler will do reverse lookup to find out RM entry
     Setp 3:  Enque in dropped DPL watermark
     -----------------------------------------------------------------------*/

  dup_len =  dsm_dup_packet(&dup_item_ptr, item_ptr, 0, dsm_length_packet(item_ptr));

  if ( 0 != dup_len && NULL != dup_item_ptr)
  {
    dup_item_ptr->app_field = (uint32)ul_wmk_ptr;
    
    dsm_enqueue( &ps_dpm_dpl_dropped_pkt_wmk, &dup_item_ptr);
  }
  
} /* ps_dpm_dpl_dne_cb_function() */

/*===========================================================================
FUNCTION   PS_DPM_DPL_DROPPED_PKT_SIG_HANDLER()

DESCRIPTION
  Signal handler for PS_DPM_DPL_DROPPED_PKT_SIGNAL

DEPENDENCIES
  None.

PARAMETERS
  sig: the Signal that is handled
  user_data_ptr: NOT USED

RETURN VALUE
  TRUE: no more processing to be done
  FALSE: Needs to be called again.

SIDE EFFECTS
  None.
===========================================================================*/
boolean ps_dpm_dpl_dropped_pkt_sig_handler
(
  ps_sig_enum_type    sig,
  void              * user_data_ptr
)
{
  dsm_item_type         * item_ptr = NULL;
  ps_dpm_rm_info_type   * dpm_rm_info_ptr = NULL;
  ps_iface_type         * iface_ptr = NULL;
  uint8                   version;
  dsm_watermark_type     *dpl_wmk_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    Step 1: Get the DPL dropped watermark
    Setp 2: Do reverse lookup to find out RM entry from watermarks
    Setp 3: Findout Iface pointer based on packet IP type
    Ste  4:  Log the packet using dropped dpl macro
      -----------------------------------------------------------------------*/
      
  dpl_wmk_ptr = ps_dpmi_dropped_dpl_get_wmk();

  item_ptr = dsm_dequeue( dpl_wmk_ptr );
   
  if ( item_ptr == NULL )
  {
     return TRUE;
  }
   /*Get the RM entry from RM uplink WM*/
  dpm_rm_info_ptr = ps_dpm_get_dpm_rm_info_by_sio_rx_wmk
                     (
                       (dsm_watermark_type*)item_ptr->app_field
                     );
  if (NULL == dpm_rm_info_ptr)
  {
    LOG_MSG_INFO1_1 ("ps_dpm_dpl_sig_handler(): Invalid RM DPM info %d", 
                      (int32)item_ptr->app_field);
    dsm_free_packet( &item_ptr);
    return FALSE;   
  }
   
  if (!dsm_peek_byte(item_ptr, 0, &version))
  {
    dsm_free_packet( &item_ptr);
    return FALSE;      
  }
   
  if (PS_DPM_OPT_V4_VERSION == (version & PS_DPM_OPT_IP_VERSION_MASK))
  {
    iface_ptr = dpm_rm_info_ptr->v4_iface_ptr;
  }
  else if (PS_DPM_OPT_V6_VERSION == (version & PS_DPM_OPT_IP_VERSION_MASK))
  {
    iface_ptr = dpm_rm_info_ptr->v6_iface_ptr;
  }
   
  if ( !PS_IFACE_IS_VALID( iface_ptr) )
  {
    dsm_free_packet( &item_ptr);
    return FALSE;
  }

  DPL_LOG_NETWORK_RX_DROPPED_PACKET(iface_ptr, item_ptr, DPL_IID_NETPROT_IP);

  dsm_free_packet( &item_ptr);
  return FALSE;
} /* ps_dpm_hw_dpl_sig_handler() */

/*===========================================================================
FUNCTION    PS_DPM_DPL_LOG_TIMER_CB

DESCRIPTION
  Timer callback to periodically Log DPL sio config and skip header bytes
  information.

PARAMETERS
  user_data  : not used

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void ps_dpm_dpl_log_timer_cb
(
  void* user_data
)
{
  ps_dpm_cmd_info_type  * dpm_cmd_info_ptr;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  LOG_MSG_INFO2_0("ps_dpm_dpl_log_timer_cb() ");
  /*-------------------------------------------------------------------------
    Allocate DPM cmd buf, populate it and post cmd to PS_RM task
  -------------------------------------------------------------------------*/
  PS_SYSTEM_HEAP_MEM_ALLOC(dpm_cmd_info_ptr, sizeof(ps_dpm_cmd_info_type),
                           ps_dpm_cmd_info_type*);
  if (NULL == dpm_cmd_info_ptr)
  {
    LOG_MSG_ERROR_INT_0("ps_dpm_dpl_log_timer_cb(): Failed to alloc DPM cmd buf");
    return;
  }

  dpm_cmd_info_ptr->dpm_cmd = PS_DPM_DPL_LOG_TIMER_CMD;
  ps_rm_send_cmd(PS_RM_DPM_CMD, dpm_cmd_info_ptr);

  return;
} /* ps_dpm_dpl_log_timer_cb */

void ps_dpm_dpl_init
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /* Initialize queue */
  dsm_queue_init ( &ps_dpm_dpl_wmk, 0x7FFFFFFF, &dpm_dpl_q );
  /* Initialize queue */
  dsm_queue_init( &ps_dpm_dpl_dropped_pkt_wmk, 0x7FFFFFFF, &dpm_dpl_dropped_pkt_q );

  /*Initialize DPL logging wmk*/
  dsm_set_low_wm( &ps_dpm_dpl_wmk, PS_DPM_DPL_WM_DEFAULT_LO );
  dsm_set_hi_wm( &ps_dpm_dpl_wmk, PS_DPM_DPL_WM_DEFAULT_HI );
  dsm_set_dne( &ps_dpm_dpl_wmk, PS_DPM_DPL_WM_DEFAULT_DNE );
  dsm_set_dne_q_cnt( &ps_dpm_dpl_wmk, PS_DPM_DPL_WM_DNE_PKT_COUNT );

  /*Initialize dropped packet DPL logging wmk*/
  dsm_set_low_wm( &ps_dpm_dpl_dropped_pkt_wmk, PS_DPM_DPL_WM_DEFAULT_LO );
  dsm_set_hi_wm( &ps_dpm_dpl_dropped_pkt_wmk, PS_DPM_DPL_WM_DEFAULT_HI );
  dsm_set_dne( &ps_dpm_dpl_dropped_pkt_wmk, PS_DPM_DPL_WM_DEFAULT_DNE );
  dsm_set_dne_q_cnt( &ps_dpm_dpl_dropped_pkt_wmk, PS_DPM_DPL_WM_DNE_PKT_COUNT );

  /* Set the non-empty function ptr */
  ps_dpm_dpl_wmk.non_empty_func_ptr = &ps_dpm_dpl_wmk_non_empty_function;

  /* Set the non-empty function ptr */
  ps_dpm_dpl_dropped_pkt_wmk.non_empty_func_ptr = &ps_dpm_dpl_dropped_pkt_wmk_non_empty_func;

  /* Register DPM DPL WMK with specific hardware */
  ps_dpm_hw_reg_dpl_wmk(&ps_dpm_dpl_wmk);

  /* Setup signal  for DPL*/
  (void) ps_set_sig_handler(PS_DPM_DPL_SIGNAL, ps_dpm_hw_dpl_sig_handler, NULL);
  
  ps_enable_sig( PS_DPM_DPL_SIGNAL );

  /* Setup signal  for dropped packet logging */
  (void) ps_set_sig_handler(PS_DPM_DPL_DROPPED_PKT_SIGNAL, 
                              ps_dpm_dpl_dropped_pkt_sig_handler, NULL);
  
  ps_enable_sig( PS_DPM_DPL_DROPPED_PKT_SIGNAL );
  
  /*-------------------------------------------------------------------------
    Initialize DPL log queue
  -------------------------------------------------------------------------*/
  memset(&ps_dpm_dpl_skip_hdr_log_info_q, 0, sizeof(q_type));
  memset(&ps_dpm_dpl_sio_conf_log_info_q, 0, sizeof(q_type));
  (void) q_init(&ps_dpm_dpl_skip_hdr_log_info_q);
  (void) q_init(&ps_dpm_dpl_sio_conf_log_info_q);

  /*-------------------------------------------------------------------------
    Allocate deferable timer handle
  -------------------------------------------------------------------------*/
  ps_dpm_dpl_log_timer_handle = ps_timer_alloc(ps_dpm_dpl_log_timer_cb, NULL);
} /* ps_dpm_dpl_init() */

void ps_dpm_dpl_deinit
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dsm_queue_destroy(&ps_dpm_dpl_wmk);
  dsm_queue_destroy(&ps_dpm_dpl_dropped_pkt_wmk);

} /*ps_dpm_dpl_deinit() */

/*===========================================================================
FUNCTION   PS_DPMI_DPL_GET_WMK

DESCRIPTION 
  This function is used to return pointer to DPM DPL WM

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  dsm_watermark_type* - Pointer to ps_dpm_dpl_wmk
===========================================================================*/
dsm_watermark_type *ps_dpmi_dpl_get_wmk
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return (&ps_dpm_dpl_wmk);
} /*ps_dpmi_dpl_get_wmk() */

/*===========================================================================
FUNCTION   PS_DPM_DPL_ENABLE_DNE_CALLBACK

DESCRIPTION 
  This function is used to set DNE call back function

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  dsm_watermark_type* - Pointer to uplink rx wm
===========================================================================*/
void ps_dpm_dpl_enable_dne_callback
(
  dsm_watermark_type  *wmk_ptr
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(NULL == wmk_ptr)
  {
    return;
  }
  /*-----------------------------------------------------------------------
     change_cb - True,  to change callback function
     cb - Setup dropped DPL logging DNE call back function
     -----------------------------------------------------------------------*/
  dsm_set_wm_cb
  (
    wmk_ptr,
    DSM_WM_DNE_QCNT_PKTS,
    TRUE,
    ps_dpm_dpl_dne_cb_function,
    FALSE,
    NULL,
    FALSE,
    0  
  );
} /*ps_dpm_dpl_enable_dne_callback() */


/*===========================================================================
FUNCTION   PS_DPM_DPL_DISABLE_DNE_CALLBACK

DESCRIPTION 
  This function is used to reset DNE call back function

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  dsm_watermark_type* - Pointer to uplink rx wm
===========================================================================*/
void ps_dpm_dpl_disable_dne_callback
(
  dsm_watermark_type  *wmk_ptr
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if(NULL == wmk_ptr)
  {
    return;
  }
    /*-----------------------------------------------------------------------
     change_cb - True,  to change callback function
     cb - Set DNE call back function to NULL
     -----------------------------------------------------------------------*/
  dsm_set_wm_cb
  (
    wmk_ptr,
    DSM_WM_DNE_QCNT_PKTS,
    TRUE,
    NULL,
    FALSE,
    NULL,
    FALSE,
    0  
  );

} /*ps_dpm_dpl_disable_dne_callback() */


/*===========================================================================
FUNCTION   PS_DPMI_DROPPED_DPL_GET_WMK

DESCRIPTION 
  This function is used to return pointer to DPM DROPPED PKT DPL WM

DEPENDENCIES
  None.

PARAMETERS
  None

RETURN VALUE
  dsm_watermark_type* - Pointer to ps_dpm_dpl_dropped_pkt_wmk
===========================================================================*/
dsm_watermark_type *ps_dpmi_dropped_dpl_get_wmk
(
  void
)
{
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  return (&ps_dpm_dpl_dropped_pkt_wmk);
} /*ps_dpmi_dropped_dpl_get_wmk() */

/*===========================================================================
FUNCTION    PS_DPM_DPL_LOG_SIO_DESC_AND_STATUS_ENQUEUE

DESCRIPTION
  Add DPL sio config info to the queue and start timer

PARAMETERS
  dpl_sio_config : DPL sio config information

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
void ps_dpm_dpl_log_sio_desc_and_status_enqueue
(
  dpl_sio_config *dpl_sio_conf_ptr
)
{
  ps_dpm_dpl_sio_cached_info_type * dpl_sio_cached_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dpl_sio_cached_ptr = q_check(&ps_dpm_dpl_sio_conf_log_info_q );
  while ( NULL != dpl_sio_cached_ptr )
  {
    /*-----------------------------------------------------------------------
      Update iface info if the mux ID is found from cached sio info queue
      except the case that IP type is different or only single V4 or V6
      call is tear down and came up again after brining up both callse
    -----------------------------------------------------------------------*/
    if (dpl_sio_cached_ptr->dpl_sio_conf.mux_id == dpl_sio_conf_ptr->mux_id &&
        (dpl_sio_cached_ptr->dpl_sio_conf.num_valid_ifaces <
           dpl_sio_conf_ptr->num_valid_ifaces ||
         (dpl_sio_cached_ptr->dpl_sio_conf.num_valid_ifaces ==
            dpl_sio_conf_ptr->num_valid_ifaces &&
          dpl_sio_cached_ptr->dpl_sio_conf.ifaces_list[0].iface_type ==
            dpl_sio_conf_ptr->ifaces_list[0].iface_type)))
    {
      /*-----------------------------------------------------------------------
        Pull, update, and insert the log at the end of the queue if found one
      -----------------------------------------------------------------------*/
      q_delete( &ps_dpm_dpl_sio_conf_log_info_q, &(dpl_sio_cached_ptr->link) );
      break;
    }

    dpl_sio_cached_ptr = q_next( &ps_dpm_dpl_sio_conf_log_info_q,
                                 &(dpl_sio_cached_ptr->link) );
  }

  if (NULL == dpl_sio_cached_ptr)
  {
    PS_SYSTEM_HEAP_MEM_ALLOC ( dpl_sio_cached_ptr, 
                               sizeof(ps_dpm_dpl_sio_cached_info_type),
                               ps_dpm_dpl_sio_cached_info_type*);

    if (dpl_sio_cached_ptr == NULL)
    {
      LOG_MSG_ERROR_0("ps_dpm_dpl_log_sio_desc_and_status_enqueue: "
                      "failed to allocate memory");
      return;
    }
  }

  /*-------------------------------------------------------------------------
    Initialize and copy log to a new buffer
  -------------------------------------------------------------------------*/
  memset(dpl_sio_cached_ptr, 0, sizeof(ps_dpm_dpl_sio_cached_info_type));
  memscpy(&dpl_sio_cached_ptr->dpl_sio_conf,
          sizeof(dpl_sio_config),
          dpl_sio_conf_ptr,
          sizeof(dpl_sio_config));
  (void) q_link(dpl_sio_cached_ptr, &(dpl_sio_cached_ptr->link));
  q_put(&ps_dpm_dpl_sio_conf_log_info_q, &(dpl_sio_cached_ptr->link));
} /* ps_dpm_dpl_log_sio_desc_and_status_enqueue */

/*===========================================================================
FUNCTION    PS_DPM_DPL_LOG_SKIP_HDR_BYTES_ENQUEUE

DESCRIPTION
  Add skip header bytes information to the queue

PARAMETERS
  mux_id         : MUX ID
  srt_endp       : Source end point ID
  hdr_bytes      : number of bytes for header
  direction      : 0 for UL and 1 for DL

RETURN VALUE
  None

DEPENDENCIES
  None.

SIDE EFFECTS
  None.
===========================================================================*/
static void ps_dpm_dpl_log_skip_hdr_bytes_enqueue
(
  uint8                mux_id,
  uint8                src_endp,
  uint8                hdr_bytes,
  uint8                direction
)
{
  ps_dpm_dpl_skip_hdr_cached_info_type * dpl_skip_hdr_cached_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dpl_skip_hdr_cached_ptr = q_check(&ps_dpm_dpl_skip_hdr_log_info_q );
  while ( NULL != dpl_skip_hdr_cached_ptr )
  {
    if (dpl_skip_hdr_cached_ptr->src_endp == src_endp)
    {
      /*-----------------------------------------------------------------------
        Pull, update, and insert the log at the end of the queue if found one
      -----------------------------------------------------------------------*/
      q_delete( &ps_dpm_dpl_skip_hdr_log_info_q,
                &(dpl_skip_hdr_cached_ptr->link) );
      break;
    }

    dpl_skip_hdr_cached_ptr = q_next( &ps_dpm_dpl_skip_hdr_log_info_q,
                                      &(dpl_skip_hdr_cached_ptr->link) );
  }

  if (NULL == dpl_skip_hdr_cached_ptr)
  {
    PS_SYSTEM_HEAP_MEM_ALLOC ( dpl_skip_hdr_cached_ptr, 
                               sizeof(ps_dpm_dpl_skip_hdr_cached_info_type),
                               ps_dpm_dpl_skip_hdr_cached_info_type*);

    if (dpl_skip_hdr_cached_ptr == NULL)
    {
      LOG_MSG_ERROR_0("dpl_log_update_timer_logs: failed to allocate memory");
      return;
    }
  }

  memset(dpl_skip_hdr_cached_ptr,
         0,
         sizeof(ps_dpm_dpl_skip_hdr_cached_info_type));
  dpl_skip_hdr_cached_ptr->mux_id    = mux_id;
  dpl_skip_hdr_cached_ptr->src_endp  = src_endp;
  dpl_skip_hdr_cached_ptr->hdr_bytes = hdr_bytes;
  dpl_skip_hdr_cached_ptr->direction = direction;

  (void) q_link(dpl_skip_hdr_cached_ptr, &(dpl_skip_hdr_cached_ptr->link));
  q_put(&ps_dpm_dpl_skip_hdr_log_info_q, &(dpl_skip_hdr_cached_ptr->link));

} /* ps_dpm_dpl_log_skip_hdr_bytes_enqueue */

static void dpl_log_delete_all_sio_conf_logs
(
  void
)
{
  ps_dpm_dpl_sio_cached_info_type * dpl_sio_cached_ptr = NULL;
  ps_dpm_dpl_sio_cached_info_type * next_dpl_sio_cached_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dpl_sio_cached_ptr = q_check(&ps_dpm_dpl_sio_conf_log_info_q );
  while ( NULL != dpl_sio_cached_ptr )
  {
    next_dpl_sio_cached_ptr = q_next( &ps_dpm_dpl_sio_conf_log_info_q,
                                      &(dpl_sio_cached_ptr->link) );
    q_delete( &ps_dpm_dpl_sio_conf_log_info_q, &(dpl_sio_cached_ptr->link) );
    PS_SYSTEM_HEAP_MEM_FREE(dpl_sio_cached_ptr);
    dpl_sio_cached_ptr = next_dpl_sio_cached_ptr;
  }

} /* dpl_log_delete_all_sio_conf_logs */

void ps_dpm_dpl_log_timer
(
  void
)
{
  ps_dpm_dpl_sio_cached_info_type      * dpl_sio_cached_ptr = NULL;
  ps_dpm_dpl_skip_hdr_cached_info_type * dpl_skip_hdr_cached_ptr = NULL;
  boolean                                    status = TRUE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
    Iterate DPL log queue for skip header bytes
  -----------------------------------------------------------------------*/
  dpl_skip_hdr_cached_ptr = q_check(&ps_dpm_dpl_skip_hdr_log_info_q );
  while ( NULL != dpl_skip_hdr_cached_ptr && TRUE == status)
  {
    status = dpl_log_skip_hdr_bytes(dpl_skip_hdr_cached_ptr->mux_id,
                                    dpl_skip_hdr_cached_ptr->src_endp,
                                    dpl_skip_hdr_cached_ptr->hdr_bytes,
                                    dpl_skip_hdr_cached_ptr->direction);
    dpl_skip_hdr_cached_ptr = q_next( &ps_dpm_dpl_skip_hdr_log_info_q,
                                      &(dpl_skip_hdr_cached_ptr->link) );
  }

  /*-----------------------------------------------------------------------
    Iterate DPL log queue for sio configuration
  -----------------------------------------------------------------------*/
  dpl_sio_cached_ptr = q_check(&ps_dpm_dpl_sio_conf_log_info_q );
  while ( NULL != dpl_sio_cached_ptr && TRUE == status )
  {
    status = dpl_log_sio_desc_and_status(
               &dpl_sio_cached_ptr->dpl_sio_conf,
               (uint8)(DPL_META_SIO_CONFIG|DPL_META_SIO_CONFIG_TIMER_MASK));
    dpl_sio_cached_ptr = q_next( &ps_dpm_dpl_sio_conf_log_info_q,
                                 &(dpl_sio_cached_ptr->link) );
  }

  /*-----------------------------------------------------------------------
    Run the timer only if DPL log mask is set
  -----------------------------------------------------------------------*/
  if (TRUE == status)
  {
    ps_timer_start(ps_dpm_dpl_log_timer_handle, DIAG_DPL_LOG_TIME_INTERVAL);
  }
  else
  {
    dpl_log_delete_all_sio_conf_logs();
  }
} /* ps_dpm_dpl_log_timer */

void ps_dpm_dpl_log_reset_sio_conf
(
  uint8 mux_id
)
{
  ps_dpm_dpl_sio_cached_info_type * dpl_sio_cached_ptr = NULL;
  ps_dpm_dpl_sio_cached_info_type * next_dpl_sio_cached_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dpl_sio_cached_ptr = q_check(&ps_dpm_dpl_sio_conf_log_info_q );
  while ( NULL != dpl_sio_cached_ptr )
  {
    next_dpl_sio_cached_ptr = q_next( &ps_dpm_dpl_sio_conf_log_info_q,
                                      &(dpl_sio_cached_ptr->link) );
    if (dpl_sio_cached_ptr->dpl_sio_conf.mux_id == mux_id)
    {
      q_delete( &ps_dpm_dpl_sio_conf_log_info_q,
                &(dpl_sio_cached_ptr->link) );
      PS_SYSTEM_HEAP_MEM_FREE(dpl_sio_cached_ptr);
    }
    dpl_sio_cached_ptr = next_dpl_sio_cached_ptr;
  }

  /* Stop timer if there is no sio config log left */
  if (ps_timer_is_running(ps_dpm_dpl_log_timer_handle) &&
      0 == q_cnt(&ps_dpm_dpl_sio_conf_log_info_q))
  {
    ps_timer_cancel(ps_dpm_dpl_log_timer_handle);
  }
} /* ps_dpm_dpl_log_reset_sio_conf */

void ps_dpm_dpl_log_sio_desc_and_status
(
  dpl_sio_config       *dpl_sio_info_ptr
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dpl_log_sio_desc_and_status(dpl_sio_info_ptr, (uint8)DPL_META_SIO_CONFIG);
  ps_dpm_dpl_log_sio_desc_and_status_enqueue(dpl_sio_info_ptr);

  /* Start timer if not running */
  if (!ps_timer_is_running(ps_dpm_dpl_log_timer_handle))
  {
    ps_timer_start(ps_dpm_dpl_log_timer_handle, DIAG_DPL_LOG_TIME_INTERVAL);
  }

} /* ps_dpm_dpl_log_sio_desc_and_status */

void ps_dpm_dpl_log_skip_hdr_bytes
(
  uint8                mux_id,
  uint8                src_endp,
  uint8                hdr_bytes,
  uint8                direction
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  dpl_log_skip_hdr_bytes(mux_id, src_endp, hdr_bytes, direction);
  ps_dpm_dpl_log_skip_hdr_bytes_enqueue(mux_id, src_endp, hdr_bytes, direction); 
} /* ps_dpm_dpl_log_skip_hdr_bytes */

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK*/
