/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     D S   M U X   P H Y S

GENERAL DESCRIPTION
  This file contains all the functions, definitions and data types needed 
  for  DS MUX  to interface with SIO for physical port connection.


  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/src/ds_mux_phys.c#2 $ 
  $DateTime: 2019/10/22 02:53:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ad     Initial version
===========================================================================*/


/*===========================================================================

                      INCLUDE FILES

===========================================================================*/

#include "ds_mux_phys.h"
#include "ds_mux.h"
#include "ds_mux_logging.h"

#ifdef TEST_FRAMEWORK
  #error code not present
#endif /*  TEST_FRAMEWORK */
ds_mux_phys_sio_conn_info_type    *ds_mux_phys_conn_info = NULL;
ds_mux_phys_rdm_oprt_enum_type     rdm_oprt_status       = DS_MUX_PHYS_RDM_OPRT_MIN;
sio_port_id_type                   cached_sio_port       = SIO_PORT_NULL; 

/*===========================================================================
                            LOCAL DEFINITIONS
===========================================================================*/
#define DS_MUX_PHYS_WM_DEFAULT_LO   175000  
#define DS_MUX_PHYS_WM_DEFAULT_HI   350000  
#define DS_MUX_PHYS_WM_DEFAULT_DNE  400000  

#define DS_MUX_SERVICE               RDM_DATA_MUX_SRVC
#define DS_MUX_PHY_DEV               RDM_UART2_DEV
#define DS_MUX_PHY_DEV_NAME          "RDM_MUX_PHYSICAL_DEV"
#define DS_MUX_MAX_RDM_OPR_RETRY     10  
#define DS_MUX_RDM_OPR_RETRY_TIMER   100 

/*===========================================================================
                      PUBLIC FUNCTION DEFINATIONS
===========================================================================*/
void ds_mux_phys_init
(

)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  ds_mux_phys_conn_info = NULL;


}/* ds_mux_phys_init */
void ds_mux_phys_deinit
(

)
{
  dsm_item_type                  *item_ptr           = NULL;
  dsm_watermark_type             *phy_rx_wmk_ptr     = NULL;
  dsm_watermark_type           *phy_tx_wmk_ptr     = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info )
  {
    DS_MUX_LOG_MSG_INFO1_1( "ds_mux_phys_rdm_open_cb phys conn is NULL %d ", ds_mux_phys_conn_info);
    return;
  }
  
  phy_rx_wmk_ptr = &ds_mux_phys_conn_info->rx_wmk;
  
  if (( NULL != phy_rx_wmk_ptr) && ( NULL != (item_ptr = dsm_dequeue( phy_rx_wmk_ptr ))))
  {
    /*-----------------------------------------------------------------------
     Flush out all the Rx packets
    -----------------------------------------------------------------------*/
    dsm_free_packet(&item_ptr);
  }
    
  phy_tx_wmk_ptr = &ds_mux_phys_conn_info->tx_wmk;
  
  if (( NULL != phy_tx_wmk_ptr) && ( NULL != (item_ptr = dsm_dequeue( phy_tx_wmk_ptr ))))
  {
    /*-----------------------------------------------------------------------
     Flush out all the tx packets
    -----------------------------------------------------------------------*/
    dsm_free_packet(&item_ptr);
  }
    
  /*-----------------------------------------------------------------------
    1. Disable Uplink/Transmit data signal 
  -----------------------------------------------------------------------*/ 
  (void) ds_mux_set_sig_handler(DS_MUX_RX_DATA_SIGNAL, NULL, NULL);
   DS_MUX_CLR_SIGNAL(DS_MUX_RX_DATA_SIGNAL);

  /*-----------------------------------------------------------------------
    2. Destroy queue assosiated with Rx/Tx WMs so that any DSM item stuck in queue will be freed
  -----------------------------------------------------------------------*/ 
  dsm_queue_destroy( &ds_mux_phys_conn_info->tx_wmk );
  dsm_queue_destroy( &ds_mux_phys_conn_info->rx_wmk );
  
  /*-----------------------------------------------------------------------
    3 Free timer if still assosiated with physical connection
  -----------------------------------------------------------------------*/ 
    if ( DS_MUX_TIMER_INVALID_HANDLE != ds_mux_phys_conn_info->timer_handle)
    {
      (void)ds_mux_timer_free( ds_mux_phys_conn_info->timer_handle );
    }

  /*-----------------------------------------------------------------------
    4 In Activity timer 
  -----------------------------------------------------------------------*/ 
    if ( DS_MUX_TIMER_INVALID_HANDLE != ds_mux_phys_conn_info->inactivity_timer_handle)
    {
      (void)ds_mux_timer_free_ext( ds_mux_phys_conn_info->inactivity_timer_handle );
    }

    
  /*-----------------------------------------------------------------------
    5 Delete physical connection
  -----------------------------------------------------------------------*/ 
  if ( NULL != ds_mux_phys_conn_info)
  {
     DS_MUX_MEM_FREE(ds_mux_phys_conn_info);
  }

  ds_mux_phys_conn_info = NULL;
}/* ds_mux_phys_deinit */


static void  ds_mux_phys_tx_wmk_low_func
(
  dsm_watermark_type  * ds_mux_tx_wmk,
  void                * user_data_ptr
)
{
  uint8          dlci_id = DS_MUX_DLCI_ID_0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info )
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_tx_wmk_low_func phys conn is NULL ");
    return;
  }
  
  for(dlci_id = DS_MUX_DLCI_ID_0;  dlci_id <= DS_MUX_MAX_LOGICAL_CONNECTION; dlci_id++  )
  {
    ds_mux_enable_flow_mask( dlci_id, DS_MUX_FLOW_DISABLE_TX_WMK );
  }
  
}/* ds_mux_phys_tx_wmk_low_func */

static void  ds_mux_phys_tx_wmk_hi_func
(
  dsm_watermark_type  * ds_mux_tx_wmk,
  void                * user_data_ptr
)
{
  uint8          dlci_id = DS_MUX_DLCI_ID_0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info )
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_tx_wmk_hi_func phys conn is NULL ");
    return;
  }
  
  for(dlci_id = DS_MUX_DLCI_ID_0;  dlci_id <= DS_MUX_MAX_LOGICAL_CONNECTION; dlci_id++  )
  {
    ds_mux_disable_flow_mask( dlci_id, DS_MUX_FLOW_DISABLE_TX_WMK );
  }
}/* ds_mux_phys_tx_wmk_hi_func */


static void  ds_mux_phys_rx_wmk_low_func
(
  dsm_watermark_type  * ds_mux_rx_wmk,
  void                * user_data_ptr
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info )
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_rx_wmk_low_func phys conn is NULL ");
    return;
  }

  
}/* ds_mux_phys_rx_wmk_low_func */

static void  ds_mux_phys_rx_wmk_hi_func
(
  dsm_watermark_type  * ds_mux_rx_wmk,
  void                * user_data_ptr
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info )
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_rx_wmk_hi_func phys conn is NULL ");
    return;
  }

}/* ds_mux_phys_rx_wmk_hi_func */


static void  ds_mux_phys_rx_wmk_non_empty_func
(
  dsm_watermark_type  * ds_mux_rx_wmk,
  void                * user_data_ptr
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
     Set Rx data signal. DS MUX IO will dequeue the packet in ds_mux_io_rx_sig_hdlr
     and will process the packet
  -----------------------------------------------------------------------*/
  DS_MUX_SET_SIGNAL(DS_MUX_RX_DATA_SIGNAL);
  
}/* ds_mux_phys_rx_wmk_non_empty_func */


void  ds_mux_phys_invoke_wake_up_ioctl
(

)
{
  sio_ioctl_param_type          ioctl_param;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info )
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_invoke_wake_up_ioctl phys conn is NULL ");
    return;
  }
  
  memset( &ioctl_param, 0x0, sizeof(sio_ioctl_param_type));
      
#ifndef TEST_FRAMEWORK
      
  sio_ioctl( ds_mux_phys_conn_info->stream_id,
             SIO_IOCTL_WAKEUP,
             &ioctl_param );
#else
    #error code not present
#endif /* TEST_FRAMEWORK */

}/* ds_mux_phys_invoke_wake_up_ioctl */

boolean ds_mux_phys_is_power_save_allowed
(

)
{
  dsm_watermark_type             *phy_rx_wmk_ptr     = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-----------------------------------------------------------------------
     Get physical connection Rx Watermark 
   -----------------------------------------------------------------------*/ 
  phy_rx_wmk_ptr = ds_mux_phy_get_rx_wmk();
    
  if (( NULL == phy_rx_wmk_ptr) || ( TRUE == dsm_is_wm_empty( phy_rx_wmk_ptr )))
  {
    /*-----------------------------------------------------------------------
      Either queue is empty or phy rx wmk become NULL. 
    -----------------------------------------------------------------------*/
    return TRUE;
  }

  return FALSE;
}/* ds_mux_phys_is_power_save_allowed*/

void ds_mux_phys_sio_flush_handler
(
   void
)
{
  sio_ioctl_param_type          ioctl_param;
  boolean                       status;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info)
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_sio_flush_handler phys conn is NULL ");
    return;
  }
  memset( &ioctl_param, 0x0, sizeof(sio_ioctl_param_type));

  status = ds_mux_complete_power_down();

  if ( TRUE == status)
  {
#ifndef TEST_FRAMEWORK
      
  sio_ioctl( ds_mux_phys_conn_info->stream_id,
             SIO_IOCTL_POWERDOWN,
             &ioctl_param );
#else
    #error code not present
#endif /* TEST_FRAMEWORK */
  }

}/* ds_mux_phys_sio_flush_handler */
void ds_mux_phys_sio_flush_cb
(
   void
)
{
  ds_muxi_cmd_info_type  * ds_mux_cmd_info_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_phys_sio_flush_cb  data %d", NULL);
  
  /*-------------------------------------------------------------------------
 locate DS MUX cmd buf, populate it and post cmd to DS MUX task
 ---------------------------------------------------------------------*/
  DS_MUX_MEM_ALLOC(ds_mux_cmd_info_ptr, sizeof(ds_muxi_cmd_info_type),
                            ds_muxi_cmd_info_type*);
  
  if (NULL == ds_mux_cmd_info_ptr)
  {
    return;
  }
  ds_mux_cmd_info_ptr->cmd_id  = DS_MUXI_PHY_SIO_FLUSH;  
  ds_mux_send_cmd(DS_MUX_CMD, ds_mux_cmd_info_ptr);

}/*ds_mux_phys_sio_flush_cb*/

boolean ds_mux_phys_flush_tx_data
(

)
{
  /*------------------------------------------------------------------------
    Wait for SIO to flush all the data before issuing POWERDOWN IOCTL. Since
    sio_flush_tx is an asynchronous call, implement blocking primitives by
    waiting on rex signal
  ------------------------------------------------------------------------*/
  #ifndef TEST_FRAMEWORK
    sio_flush_tx( ds_mux_phys_conn_info->stream_id, ds_mux_phys_sio_flush_cb);
  #else
    #error code not present
#endif /*TEST_FRAMEWORK*/
  
  return TRUE;
}/* ds_mux_phys_flush_tx_data */

void ds_mux_phys_inactivity_timer_expire_handler
(
  
)
{
  boolean  status = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info)
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_sio_flush_handler phys conn is NULL ");
    return;
  }
  status = ds_mux_start_power_save_routine();

  DS_MUX_LOG_MSG_INFO1_2( "ds_mux_phys_inactivity_timer_expire_handler power save allow %d mux state %d", 
    status, ds_mux_get_mux_state());

  if (FALSE == status)
  {
    /*-----------------------------------------------------------------------
      Power save is not allowed , Re-start the timer when MUX state is opened
     -----------------------------------------------------------------------*/
    if ( DS_MUX_STATE_OPENED == ds_mux_get_mux_state())
    {
      ds_mux_phys_start_inactivity_timer();
    }
  }
   
}/* ds_mux_phys_inactivity_timer_expire_handler */

static void ds_mux_phys_inactivity_timer_cb
(
  void *  data
)
{
  ds_muxi_cmd_info_type  * ds_mux_cmd_info_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_phys_inactivity_timer_cb  data %d",(uint8) data);
  
  /*-------------------------------------------------------------------------
 Allocate DS MUX cmd buf, populate it and post cmd to DS MUX task
------------------------------------------------------------------------*/
  DS_MUX_MEM_ALLOC(ds_mux_cmd_info_ptr, sizeof(ds_muxi_cmd_info_type),
                            ds_muxi_cmd_info_type*);
  
  if (NULL == ds_mux_cmd_info_ptr)
  {
    return;
  }
  
  ds_mux_cmd_info_ptr->cmd_id  = DS_MUXI_INACTIVITY_TIMER_EXPIRE;  
  ds_mux_send_cmd(DS_MUX_CMD, ds_mux_cmd_info_ptr);

}/* ds_mux_phys_inactivity_timer_cb */

void ds_mux_phys_start_inactivity_timer
(

)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( NULL == ds_mux_phys_conn_info)
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_start_inactivity_timer phys conn is NULL ");
    return;
  }

  if( DS_MUX_TIMER_INVALID_HANDLE == ds_mux_phys_conn_info->inactivity_timer_handle)
  {
    return;
  }
  
  ds_mux_timer_start_ext( ds_mux_phys_conn_info->inactivity_timer_handle,
                              DS_MUX_INACTIVITY_TIMER );
}/* ds_mux_phys_start_inactivity_timer*/

void ds_mux_phys_stop_inactivity_timer
(

)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  if( NULL == ds_mux_phys_conn_info)
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_stop_inactivity_timer phys conn is NULL ");
    return;
  }

  if( DS_MUX_TIMER_INVALID_HANDLE == ds_mux_phys_conn_info->inactivity_timer_handle)
  {
    return;
  }
  
  ds_mux_timer_cancel_ext( ds_mux_phys_conn_info->inactivity_timer_handle );
}/* ds_mux_phys_stop_inactivity_timer*/


boolean ds_mux_phys_get_dtr_status
(

)
{
    sio_ioctl_param_type          ioctl_param;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( NULL == ds_mux_phys_conn_info)
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_get_dtr_status phys conn is NULL ");
    return FALSE;
  }

  memset( &ioctl_param, 0x0, sizeof(sio_ioctl_param_type));
  
  ioctl_param.dte_ready_asserted = &ds_mux_phys_conn_info->dte_ready_asserted;
#ifndef TEST_FRAMEWORK
  sio_ioctl( ds_mux_phys_conn_info->stream_id,
            SIO_IOCTL_DTE_READY_ASSERTED,
            &ioctl_param );
#else
  #error code not present
#endif /* TEST_FRAMEWORK */

  return ds_mux_phys_conn_info->dte_ready_asserted;
}

void ds_mux_phys_dtr_changed_cb
(
  void
)
{
  sio_ioctl_param_type          ioctl_param;
  uint8                         dlci_id = DS_MUX_DLCI_ID_1;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info)
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_dtr_changed_cb phys conn is NULL ");
    return;
  }
  
  memset( &ioctl_param, 0x0, sizeof(sio_ioctl_param_type));
  
  ioctl_param.dte_ready_asserted = &ds_mux_phys_conn_info->dte_ready_asserted;

  #ifndef TEST_FRAMEWORK
  
  sio_ioctl( ds_mux_phys_conn_info->stream_id,
            SIO_IOCTL_DTE_READY_ASSERTED,
            &ioctl_param );
  #else
#error code not present
#endif /* TEST_FRAMEWORK */

  for(dlci_id = DS_MUX_DLCI_ID_0;  dlci_id <= DS_MUX_MAX_LOGICAL_CONNECTION; dlci_id++  )
  {
    if( FALSE ==  ds_mux_phys_conn_info->dte_ready_asserted)
    {
      ds_mux_disable_flow_mask( dlci_id, DS_MUX_FLOW_DISABLE_PHY_DTR_LOW );
    }
    else
    {
      ds_mux_enable_flow_mask( dlci_id, DS_MUX_FLOW_DISABLE_PHY_DTR_LOW );
    }
  }

  if( DS_MUX_STATE_OPENED == ds_mux_get_mux_state())
  {
    /*-------------------------------------------------------------------------
      Start the InActivity timer is physical link is ready to communicate.
      Stop/cancel the timer if physical link is not ready, We cann't transmit any PSC(power save msg) 
    -------------------------------------------------------------------------*/
    if( TRUE == ds_mux_phys_conn_info->dte_ready_asserted)
    {
      ds_mux_phys_start_inactivity_timer( );
    }
    else
    {
      ds_mux_phys_stop_inactivity_timer();
    }
  }
  else if (  DS_MUX_STATE_WAKEUP_REQUEST == ds_mux_get_mux_state() )
  {
    /*-------------------------------------------------------------------------
        If DS MUX stuck to send wake-up sequnce because of DTR low. Send the wake-up
        sequnce once DTR become high
    -------------------------------------------------------------------------*/
    if( TRUE == ds_mux_phys_conn_info->dte_ready_asserted)
    {
      ds_mux_invoke_wake_up_routine( );
    }
  }

}/*ds_mux_phys_dtr_changed_cb*/

ds_mux_result_enum_type ds_mux_phys_conn_init
(
)
{
  ds_mux_result_enum_type  result  = DS_MUX_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_MUX_MEM_ALLOC(ds_mux_phys_conn_info, sizeof(ds_mux_phys_sio_conn_info_type),
    ds_mux_phys_sio_conn_info_type* );

  memset(ds_mux_phys_conn_info, 0x0, sizeof(ds_mux_phys_sio_conn_info_type));
  
  if ( NULL == ds_mux_phys_conn_info)
  {
    return DS_MUX_FAILURE;
  }

  do
  {
   /*-----------------------------------------------------------------------
      1.  Initialize the TX Watermark 
      2.  Initialize the RX Watermark
      3.  Set flow control flag as false
      4.  Set RDM operation retry values to 0
    -----------------------------------------------------------------------*/ 
    /* Step 1 */
    dsm_queue_init ( &ds_mux_phys_conn_info->tx_wmk,
                     0x7FFFFFFF,
                     &ds_mux_phys_conn_info->tx_q );
    
    dsm_set_low_wm( &ds_mux_phys_conn_info->tx_wmk, DS_MUX_PHYS_WM_DEFAULT_LO );
    dsm_set_hi_wm( &ds_mux_phys_conn_info->tx_wmk, DS_MUX_PHYS_WM_DEFAULT_HI );
    dsm_set_dne( &ds_mux_phys_conn_info->tx_wmk, DS_MUX_PHYS_WM_DEFAULT_DNE );

    ds_mux_phys_conn_info->tx_wmk.lowater_func_ptr = ds_mux_phys_tx_wmk_low_func;
    ds_mux_phys_conn_info->tx_wmk.hiwater_func_ptr = ds_mux_phys_tx_wmk_hi_func;

    
    /* Step 2 */
    dsm_queue_init ( &ds_mux_phys_conn_info->rx_wmk,
                     0x7FFFFFFF,
                     &ds_mux_phys_conn_info->rx_q );
    
    dsm_set_low_wm( &ds_mux_phys_conn_info->rx_wmk, DS_MUX_PHYS_WM_DEFAULT_LO );
    dsm_set_hi_wm( &ds_mux_phys_conn_info->rx_wmk, DS_MUX_PHYS_WM_DEFAULT_HI );
    dsm_set_dne( &ds_mux_phys_conn_info->rx_wmk, DS_MUX_PHYS_WM_DEFAULT_DNE );
    
    ds_mux_phys_conn_info->rx_wmk.non_empty_func_ptr = ds_mux_phys_rx_wmk_non_empty_func;
    ds_mux_phys_conn_info->rx_wmk.lowater_func_ptr = ds_mux_phys_rx_wmk_low_func;
    ds_mux_phys_conn_info->rx_wmk.hiwater_func_ptr = ds_mux_phys_rx_wmk_hi_func;

    /* Step 3*/
    ds_mux_phys_conn_info->is_tx_flow_controlled   = FALSE;
    ds_mux_phys_conn_info->is_rx_flow_controlled   = FALSE;
    ds_mux_phys_conn_info->stream_id               = SIO_NO_STREAM_ID;
    ds_mux_phys_conn_info->dte_ready_asserted      = TRUE;
    ds_mux_phys_conn_info->sio_port                = SIO_PORT_NULL;

     /* Step 4*/
    ds_mux_phys_conn_info->rdm_oprt_retry = 0;
    ds_mux_phys_conn_info->timer_handle   = DS_MUX_TIMER_INVALID_HANDLE;
    ds_mux_phys_conn_info->inactivity_timer_handle   = DS_MUX_TIMER_INVALID_HANDLE;

    return result;    
  }while(0);

  return result;
}/*ds_mux_phys_conn_init*/

static void ds_mux_phys_rdm_open_cb
(
  sio_port_id_type port_id
)
{
  ds_muxi_cmd_info_type  * ds_mux_cmd_info_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_phys_rdm_open_cb rdm_oprt %d ", port_id);
  
  /*-------------------------------------------------------------------------
    Allocate DS MUX cmd buf, populate it and post cmd to DS MUX task
  -------------------------------------------------------------------------*/
  DS_MUX_MEM_ALLOC(ds_mux_cmd_info_ptr, sizeof(ds_muxi_cmd_info_type),
                            ds_muxi_cmd_info_type*);
  
  if (NULL == ds_mux_cmd_info_ptr)
  {
    /*-----------------------------------------------------------------------
      No free command buffers available. Notify RDM that the open failed.
    -----------------------------------------------------------------------*/
    rdm_notify( RDM_DATA_SRVC, RDM_APP_BUSY_S );
    return;
  }
  
  ds_mux_cmd_info_ptr->cmd_id  = DS_MUXI_PHY_PORT_OPEN;
  ds_mux_cmd_info_ptr->cmd_info.phy_port_open_info.port_id = port_id;

  ds_mux_send_cmd(DS_MUX_CMD, ds_mux_cmd_info_ptr);

}/* ds_mux_phys_rdm_open_cb */

void ds_mux_phys_rdm_open_handler
(
  sio_port_id_type port_id
)
{
  sio_open_type        open_param;
  sio_ioctl_param_type ioctl_param;
  ds_mux_result_enum_type  result  = DS_MUX_SUCCESS;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info)
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_rdm_open_handler phys conn is NULL ");
      return;
  }
  
  /*-----------------------------------------------------------------------
    Store SIO port 
    -----------------------------------------------------------------------*/
  ds_mux_phys_conn_info->sio_port = port_id;

  memset( &open_param, 0x0, sizeof(sio_open_type));
  memset( &ioctl_param, 0x0, sizeof(sio_ioctl_param_type));
   
  do
  {
    /*-----------------------------------------------------------------------
      1.  Initialize SIO parameters 
      2.  Open port with SIO
      3.  Register for DTR change and also check current DTR status 
      4   Assert DTR, FLOW signal
      5   Set Rx Signal handler
      6   Notify DS MUX about port open successful or failure so that APP can be notified 
      -----------------------------------------------------------------------*/ 
    /* Step 1 */
    open_param.rx_queue    = &ds_mux_phys_conn_info->rx_wmk;
    open_param.tx_queue    = &ds_mux_phys_conn_info->tx_wmk;
    open_param.rx_func_ptr = NULL;
    open_param.stream_mode = SIO_GENERIC_MODE;
    open_param.stream_type = SIO_STREAM_RXTX;
    open_param.port_id     = port_id;
    open_param.rx_bitrate  = SIO_BITRATE_3000000;
    open_param.tx_bitrate  = SIO_BITRATE_3000000;

    /* Step 2 */
    #ifndef TEST_FRAMEWORK
      ds_mux_phys_conn_info->stream_id = sio_open(&open_param);
    #else
      #error code not present
#endif /* TEST_FRAMEWORK */
    
    if ( SIO_NO_STREAM_ID == ds_mux_phys_conn_info->stream_id)
    {
      result  = DS_MUX_FAILURE;
      rdm_notify( RDM_DATA_MUX_SRVC, RDM_APP_BUSY_S );
      /*-----------------------------------------------------------------------
        Close down devices so that in next init we do register with device again
        -----------------------------------------------------------------------*/
      rdm_close_device(DS_MUX_SERVICE, DS_MUX_PHY_DEV, NULL);
      break;
    }
    else if (SIO_PORT_NULL == cached_sio_port )
    {
      rdm_notify( RDM_DATA_MUX_SRVC, RDM_DONE_S );
    }
    /*-----------------------------------------------------------------------
      Allocate In-Activity timer
      -----------------------------------------------------------------------*/
    ds_mux_phys_conn_info->inactivity_timer_handle = ds_mux_timer_alloc_ext( ds_mux_phys_inactivity_timer_cb, 
                                                           NULL );
    
   /* Step 3 */
    ioctl_param.enable_dte_ready_event = ds_mux_phys_dtr_changed_cb;
   
    #ifndef TEST_FRAMEWORK
      sio_ioctl( ds_mux_phys_conn_info->stream_id,
                 SIO_IOCTL_ENABLE_DTR_EVENT,
                 &ioctl_param );
    
      ioctl_param.dte_ready_asserted = &ds_mux_phys_conn_info->dte_ready_asserted;
    
      sio_ioctl( ds_mux_phys_conn_info->stream_id,
            SIO_IOCTL_DTE_READY_ASSERTED,
            &ioctl_param );

    #else
      #error code not present
#endif/* TEST_FRAMEWORK*/

    /* Step 4 */
    #ifndef TEST_FRAMEWORK
    //Enable Flow
    sio_ioctl( ds_mux_phys_conn_info->stream_id,
               SIO_IOCTL_INBOUND_FLOW_ENABLE,
                &ioctl_param );
    //Enable CD
    sio_ioctl( ds_mux_phys_conn_info->stream_id,
               SIO_IOCTL_CD_ASSERT,
               &ioctl_param );
    //Enable DTR
    sio_ioctl( ds_mux_phys_conn_info->stream_id,
               SIO_IOCTL_DSR_ASSERT,
               &ioctl_param );
    #else
    #error code not present
#endif/* TEST_FRAMEWORK*/

    /* Step 5 */
    ds_mux_enable_sig( DS_MUX_RX_DATA_SIGNAL );
    
    ds_mux_set_sig_handler(DS_MUX_RX_DATA_SIGNAL,
                           ds_mux_io_rx_sig_hdlr,
                           NULL);
  }while(0);

  //ds_mux_phys_conn_info will be NULL if status is FAILURE
  DS_MUX_LOG_MSG_INFO1_3( "ds_mux_phys_rdm_open_handler result %d result %d rdm_oprt %d ",
    result, ds_mux_phys_conn_info->stream_id, rdm_oprt_status);

  ds_mux_update_phy_port_open_status( result );

  rdm_oprt_status =  DS_MUX_PHYS_RDM_OPRT_MIN;
}/* ds_mux_phys_rdm_open_cb */

void ds_mux_phys_rdm_close_cb
(
  void
)
{

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  //TBD
  
} /* ds_mux_phys_rdm_close_cb() */

void ds_mux_phys_rdm_assign_port_cb
(
  rdm_assign_status_type status,
  rdm_service_enum_type  service,
  rdm_device_enum_type   device
)
{
  ds_muxi_cmd_info_type  * ds_mux_cmd_info_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_phys_rdm_assign_port_cb status %d ", status);
  
  /*-------------------------------------------------------------------------
   Allocate DS MUX cmd buf, populate it and post cmd to DS MUX task
 -------------------------------------------------------------------------*/
  DS_MUX_MEM_ALLOC(ds_mux_cmd_info_ptr, sizeof(ds_muxi_cmd_info_type),
                            ds_muxi_cmd_info_type*);
  
  if (NULL == ds_mux_cmd_info_ptr)
  {
    return;
  }
  
  ds_mux_cmd_info_ptr->cmd_id  = DS_MUXI_PHY_PORT_ASSIGN;
  ds_mux_cmd_info_ptr->cmd_info.phy_assign_port_info.status = status;
  ds_mux_cmd_info_ptr->cmd_info.phy_assign_port_info.service = service;
  ds_mux_cmd_info_ptr->cmd_info.phy_assign_port_info.device = device;
  
  ds_mux_send_cmd(DS_MUX_CMD, ds_mux_cmd_info_ptr);

}/*ds_mux_phys_rdm_assign_port_cb*/

void  ds_mux_phys_rdm_assign_port_handler
(
  rdm_assign_status_type status,
  rdm_service_enum_type  service,
  rdm_device_enum_type   device
)
{
  boolean    retry_rdm_oprt = FALSE;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_MUX_LOG_MSG_INFO1_3( "ds_mux_phys_rdm_assign_port_handler status %d device %d service %d",
    status, service, device);

  if ( NULL == ds_mux_phys_conn_info )
  {
    DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_rdm_assign_port_handler phys conn is NULL ");
    return;
  }
  /*-----------------------------------------------------------------------
    Retry port assignment again after sometimes if RDM is busy. 
    Otherwise ignore and "don't retry" in case of success(RDM_DONE_S) or other failures 
    (RDM_NOT_ALLOWED_S, RDM_RESTRICTED_S, RDM_APP_BUSY_S)
    -----------------------------------------------------------------------*/
  if ( RDM_DEVMAP_BUSY_S == status )
  {
    if( ds_mux_phys_conn_info->rdm_oprt_retry  < DS_MUX_MAX_RDM_OPR_RETRY)
    {
      retry_rdm_oprt   =  TRUE;
      ds_mux_phys_conn_info->rdm_oprt_retry++;
      rdm_oprt_status  = DS_MUX_PHYS_RDM_ASSIGN_PORT_IN_PROCESS;
        
      if( PS_TIMER_FAILURE == ds_mux_timer_start( ds_mux_phys_conn_info->timer_handle,
                                DS_MUX_RDM_OPR_RETRY_TIMER ) )
      {
        retry_rdm_oprt = FALSE;
      }
      
    }
  }
  
  if (FALSE == retry_rdm_oprt)
  {
    if ( DS_MUX_TIMER_INVALID_HANDLE != ds_mux_phys_conn_info->timer_handle)
    {
      (void)ds_mux_timer_free( ds_mux_phys_conn_info->timer_handle );
      
      ds_mux_phys_conn_info->rdm_oprt_retry = 0;
      ds_mux_phys_conn_info->timer_handle   = DS_MUX_TIMER_INVALID_HANDLE;
    }
    /*-----------------------------------------------------------------------
      Notify DS MUX about failure so that resouces can we freed
      -----------------------------------------------------------------------*/
    if ( RDM_DONE_S != status )
    {      
      ds_mux_update_phy_port_open_status( DS_MUX_FAILURE );
      rdm_oprt_status  = DS_MUX_PHYS_RDM_ASSIGN_PORT_FAILED;
    }
    else
    {
      rdm_oprt_status  = DS_MUX_PHYS_RDM_ASSIGN_PORT_COMPLETE;
    }
  }
  
  DS_MUX_LOG_MSG_INFO2_4( "ds_mux_phys_rdm_assign_port_handler retry flag %d "
                          " rdm operation retried %d timer handle %d rdm_oprt_status %d",
                          retry_rdm_oprt, ds_mux_phys_conn_info->rdm_oprt_retry ,
                          ds_mux_phys_conn_info->timer_handle, rdm_oprt_status);

}/* ds_mux_phys_rdm_assign_port_handler */

static void ds_mux_phys_rdm_oprt_timer_cb
(
  void *  data
)
{
  boolean                         result = FALSE;
  ds_mux_phys_rdm_oprt_enum_type  rdm_oprt = (ds_mux_phys_rdm_oprt_enum_type) data;
  
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  switch( rdm_oprt )
  {
    case DS_MUX_PHYS_RDM_ASSIGN_PORT:
    {
      result = rdm_assign_port( DS_MUX_SERVICE,
                                DS_MUX_PHY_DEV,
                                ds_mux_phys_rdm_assign_port_cb );
      break;
    }
    case DS_MUX_PHYS_RDM_CLOSE_PORT:
    {
      break;
    }
    default:
      break;
    
  }
  DS_MUX_LOG_MSG_INFO2_2( "ds_mux_phys_rdm_oprt_timer_cb rdm_oprt %d result %d", rdm_oprt, result);
  
}/* ds_mux_phys_rdm_oprt_timer_cb */

ds_mux_result_enum_type ds_mux_phys_port_connect
(
  boolean                is_cmux_supported,
  sio_port_id_type       sio_port_id
)
{
  ds_mux_result_enum_type  result  = DS_MUX_SUCCESS;
  rdm_service_params_type  rdm_params;
  rdm_device_info_type     rdm_device_info;
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info)
  {
    return DS_MUX_FAILURE;
  }
  /*-----------------------------------------------------------------------
    Store SIO port 
    -----------------------------------------------------------------------*/
  ds_mux_phys_conn_info->sio_port = sio_port_id;

  do
  {
    /*-----------------------------------------------------------------------
       0   Use cached port (if available) otherwise register for service and wait for RDM open call back
       1.  Initialize RDM service params 
       2.  Register service with RDM and set compatibility  
            Step 3 to 5 is required only when DS MUX share same port
            with other service(+CMUX AT command supported)
       3.  Initialize RDM device params  and register device with RDM.
       4.  Assign port to service
       5. Allocate the timer and Start timer if RDM is busy else free the timer
     -----------------------------------------------------------------------*/ 
    if (  SIO_PORT_NULL != cached_sio_port )
    {
       /* Step 0 */
      ds_mux_phys_rdm_open_cb(cached_sio_port);
    }
    else
    {
      /* Step 1 */
      rdm_params.device_support   = RDM_SINGLE_DEV_SUPPORT;
      rdm_params.open_multi_func  = NULL;
      rdm_params.close_multi_func = NULL;
      rdm_params.open_func        = ds_mux_phys_rdm_open_cb;
      rdm_params.close_func       = ds_mux_phys_rdm_close_cb;
      rdm_params.data             = NULL;

    /* Step 2 */
      rdm_register_service ( DS_MUX_SERVICE, &rdm_params );

      rdm_set_compatibility ( DS_MUX_SERVICE, DS_MUX_PHY_DEV);
   
      if ( TRUE == is_cmux_supported)
      {

      /* Step 3 */
        rdm_device_info.device    = DS_MUX_PHY_DEV;
        rdm_device_info.sio_port  = sio_port_id;
        rdm_device_info.available = TRUE;
  
      
        (void) ds_mux_memscpy( &rdm_device_info.device_name[0],
                               RDM_MAX_DEV_NAME_LEN,
                               DS_MUX_PHY_DEV_NAME,
                               strlen(DS_MUX_PHY_DEV_NAME) );
      
        rdm_register_device( &rdm_device_info );
      
      /* Step 4*/
        rdm_oprt_status       = DS_MUX_PHYS_RDM_ASSIGN_PORT;

        ds_mux_phys_conn_info->timer_handle = ds_mux_timer_alloc( ds_mux_phys_rdm_oprt_timer_cb, 
                                                              (void *)rdm_oprt_status );

         rdm_assign_port( DS_MUX_SERVICE,
                                                DS_MUX_PHY_DEV,
                                                ds_mux_phys_rdm_assign_port_cb );
      /*-----------------------------------------------------------------------
            RDM can invoke call back and notified failure in ASSIGN API itself.
          ---------------------------------------------------------------------*/
      }
    }
    return result;
    
  }while(0);
  
  result  = DS_MUX_FAILURE;
  return result;
}/*ds_mux_phys_port_connect*/


boolean ds_mux_phys_is_port_open
(

)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if (NULL == ds_mux_phys_conn_info)
  {
    return FALSE;
  }
  else if ( SIO_NO_STREAM_ID == ds_mux_phys_conn_info->stream_id)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }

}/*ds_mux_phys_is_port_open*/


dsm_watermark_type* ds_mux_phy_get_rx_wmk
(

)
{
  dsm_watermark_type*   rx_wmk_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL != ds_mux_phys_conn_info )
  {
    rx_wmk_ptr = &ds_mux_phys_conn_info->rx_wmk;
  }

  if( NULL == rx_wmk_ptr)
  {
    LOG_MSG_ERROR_0("ds_mux_phy_get_rx_wmk(): Invalid rx wmk");
  }
  
  return rx_wmk_ptr;
}/* ds_mux_phy_get_rx_wmk */

ds_mux_result_enum_type ds_mux_phys_transmit
( 
  dsm_item_type            *item_ptr
)
{
  ds_mux_result_enum_type       result   = DS_MUX_SUCCESS;
  sio_ioctl_param_type          ioctl_param;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-----------------------------------------------------------------------
    1 Check the DTR status
      1.1 DTR is low then get real time status
    2. DTR is HIGH then transmit the data to SIO
  -----------------------------------------------------------------------*/ 
  if ( NULL == ds_mux_phys_conn_info)
  {
    return DS_MUX_FAILURE;
  }
  /*Step 1*/
  if ( FALSE == ds_mux_phys_conn_info->dte_ready_asserted )
  {
    memset( &ioctl_param, 0x0, sizeof(sio_ioctl_param_type));
    
    ioctl_param.dte_ready_asserted = &ds_mux_phys_conn_info->dte_ready_asserted;

   #ifndef TEST_FRAMEWORK
    sio_ioctl( ds_mux_phys_conn_info->stream_id,
              SIO_IOCTL_DTE_READY_ASSERTED,
              &ioctl_param );
   #else
#error code not present
#endif/* TEST_FRAMEWORK */
  }
  /*Step 2*/
  if ( ds_mux_phys_conn_info->dte_ready_asserted )
  {
    /*-----------------------------------------------------------------------
      Re-start In-Activity timer
    -----------------------------------------------------------------------*/
    ds_mux_phys_start_inactivity_timer();
        
    DS_MUX_LOG_PHY_TX_PACKET( item_ptr);
  
    #ifndef TEST_FRAMEWORK
      sio_transmit( ds_mux_phys_conn_info->stream_id,
                  item_ptr );
    #else
    #error code not present
#endif/* TEST_FRAMEWORK */
  }
  else
  {  
    result   = DS_MUX_FAILURE;
  }
  return result;
}/* ds_mux_phys_transmit */

void ds_mux_phys_sio_port_close_cb
(

)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_MUX_LOG_MSG_INFO2_0( "ds_mux_phys_sio_port_close_cb ");

}/* ds_mux_phys_sio_port_close_cb */
    
void ds_mux_phys_shut_down_interface
(

)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if ( NULL == ds_mux_phys_conn_info)
  {
    return;
  }

  /*-----------------------------------------------------------------------
    Cached previously opened port
    -----------------------------------------------------------------------*/ 
  cached_sio_port = ds_mux_phys_conn_info->sio_port;
  /*-----------------------------------------------------------------------
    Close physical port (if Valid Stream ID is present)
    -----------------------------------------------------------------------*/ 
  if ( SIO_NO_STREAM_ID != ds_mux_phys_conn_info->stream_id )
  {
#ifndef TEST_FRAMEWORK
    sio_close(ds_mux_phys_conn_info->stream_id, ds_mux_phys_sio_port_close_cb );
#else
    #error code not present
#endif /* TEST_FRAMEWORK */
  }
  /*-----------------------------------------------------------------------
    De-init physical interface  (Close call back will be called before sio_close API return hence safe
    to de-init physical layer)
    -----------------------------------------------------------------------*/ 
  ds_mux_phys_deinit();

}/* ds_mux_phys_shut_down_interface */


