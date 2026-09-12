/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                     D S   M U X   S T U B

GENERAL DESCRIPTION

  This is the external header file for the MUX Task. This file
  contains all the functions, STUB definitions and data types needed for MUX 
  task to interface to SIO for physical port.

  Copyright (c) 2016 by Qualcomm Technologies, Incorporated.  All Rights Reserved.  
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/src/ds_mux_stubs.c#2 $ 
  $DateTime: 2019/10/22 02:53:17 $ 
  $Author: pwbldsvc $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/01/16   ad     Initial version
===========================================================================*/


/*===========================================================================

                      INCLUDE FILES

===========================================================================*/

#include "ds_mux_stub.h"
#include "ds_mux.h"
#include "ds_mux_api.h"
#include "ds_mux_phys.h"
#include "ps_byte.h"
#include "ps_logging_diag.h"
#include <stringl/stringl.h>
#include "vstmr.h" 
#include "ds_mux_platform.h"

#ifdef TEST_FRAMEWORK
#error code not present
#endif  /* TEST_FRAMEWORK */


#ifdef FEATURE_DATA_MUX_TEST_LOOP_BACK

/*! @brief This should be last service and should NOT be in use.  */
#define    DS_MUX_LOOP_BACK_SERV_1               RDM_DATA_MUX_LOGICAL_3_SRVC
#define    DS_MUX_LOOP_BACK_SERV_2               RDM_DATA_MUX_LOGICAL_4_SRVC
#define    DS_MUX_MAX_TEST_APP_CLIENT            0x2
#define    DS_MUX_TEST_APP_CLIENT_1              0x0
#define    DS_MUX_TEST_APP_CLIENT_2              0x1


/*! @brief MAX DSM Data size */
#define  DS_MUX_LOOP_BACK_DSM_MAX_DATA         4096

/*! @brief Define DSM WM default values */
#define  DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_LO    10000  
#define  DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_HI    350000 
#define  DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_DNE   400000  

/*! @brief Define DSM WM  LIMIT */
#define  DS_MUX_LOOP_BACK_DSM_WMK_LIMIT        0x7FFFFFFF
#define  DS_MUX_LOOP_BACK_TIMER                1000
#define  DS_MUX_LOOP_BACK_BASIC_DATA_SIZE      10
#define  DS_MUX_LOOP_BACK_BIG_DATA_SIZE        40000

/*! @brief Pattern of the payload to verify data corruption */
#define  DS_MUX_LOOP_BACK_PAYLOAD_PATTERN_SEND       0x55
#define  DS_MUX_LOOP_BACK_PAYLOAD_PATTERN_RECV       0xAA

/*! @brief Verify Client ID */
#define DS_MUX_STUB_MAX_CLIENT_ID 2
#define DS_MUX_STUB_GET_CLIENT_ID_FROM_INDEX(cid) (cid + 1)
#define DS_MUX_STUB_IS_VALID_CLIENT_ID(cid) (cid > 0 && cid <= DS_MUX_STUB_MAX_CLIENT_ID)

/*! @brief Define DSM WM values for testing */
#define  DS_MUX_LOOP_BACK_DSM_WM_FLOW_CONTROL_TEST_LO    2000  
#define  DS_MUX_LOOP_BACK_DSM_WM_FLOW_CONTROL_TEST_HI    3000 


/*! @brief Global veriables */
ds_mux_loop_back_test_config_info_type  ds_mux_loop_back_config[DS_MUX_MAX_TEST_APP_CLIENT];
/* DS MUX test loop back config */

dsm_watermark_type        ds_mux_loop_tx_wmk[DS_MUX_MAX_TEST_APP_CLIENT];
/* Transmit Watermark, to write to the SIO Port */

q_type                    ds_mux_loop_tx_q[DS_MUX_MAX_TEST_APP_CLIENT];
/* Queue for the tx_wmk */

dsm_watermark_type        ds_mux_loop_rx_wmk[DS_MUX_MAX_TEST_APP_CLIENT];
/* Receive Watermark, to receive from the SIO Port */

q_type                    ds_mux_loop_rx_q[DS_MUX_MAX_TEST_APP_CLIENT];
/* Queue for the rx_wmk */

uint8                     ds_mux_big_buf[DS_MUX_MAX_TEST_APP_CLIENT][DS_MUX_LOOP_BACK_BIG_DATA_SIZE];
uint8                     ds_mux_recv_big_buf[DS_MUX_MAX_TEST_APP_CLIENT][DS_MUX_LOOP_BACK_BIG_DATA_SIZE];
/* Big Buffer, it will contain random data */

ds_mux_stub_integrity_test_info  ds_mux_integrity_test_info;
/* Structure for integrity test */

extern ds_mux_phys_sio_conn_info_type *ds_mux_phys_conn_info;
/* Structure of physical connection to change watermakrs for flow control testing */

typedef struct
{
  uint8  test_type;
  uint16 pkt_len;
  uint16 index;
}ds_mux_stub_pkt_header;

typedef enum 
{
  DS_MUX_LOOP_BACK_PARSE_HEADER_OK = 0,
  DS_MUX_LOOP_BACK_PARSE_HEADER_ERROR
} ds_mux_loop_back_parse_header_err_type;

/*===========================================================================

FUNCTION:  ds_mux_loop_back_dsm_tx_wmk_low_cb

===========================================================================*/
/*!
  @brief Tx Watermark low call back function

  @return None
*/
/*=========================================================================*/
static void ds_mux_loop_back_dsm_tx_wmk_low_cb
(
  dsm_watermark_type *msg_tx_wmk_low, /* DSM TX low wmk pointer */
  void               *user_data_ptr   /* User data pointer */
)
{
  uint8      client_id = (uint8) user_data_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_dsm_tx_wmk_low_cb "
    " Recevied low watermark, Application flow is enabled client id %d ",
    DS_MUX_STUB_GET_CLIENT_ID_FROM_INDEX(client_id));
  /*-----------------------------------------------------------------------
    Flow control Handling (Enable TX flow)
    1. Send Next pending packet
    2. Start the timer
  -----------------------------------------------------------------------*/
  ds_mux_loop_back_config[client_id].flow_enabled = TRUE;

  /*-------------------------------------------------------------------------
   Restart the timer again if number of send is less then MAX number of packets 
  ------------------------------------------------------------------------*/
  if ( TRUE == ds_mux_loop_back_config[client_id].dte_ready_asserted &&
      ds_mux_loop_back_config[client_id].curr_sent_counter < ds_mux_loop_back_config[client_id].max_num_packet)
  {
    ds_mux_timer_start( ds_mux_loop_back_config[client_id].timer_handle, ds_mux_loop_back_config[client_id].time_interval );
  }

} /* ds_mux_loop_back_dsm_tx_wmk_low_cb() */


/*===========================================================================

FUNCTION:  ds_mux_loop_back_dsm_rx_wmk_low_cb

===========================================================================*/
/*!
  @brief Rx Watermark low call back function

  @return None
*/
/*=========================================================================*/
static void ds_mux_loop_back_dsm_rx_wmk_low_cb
(
  dsm_watermark_type *msg_rx_wmk_low, /* DSM RX low wmk pointer */
  void               *user_data_ptr   /* User data pointer */
)
{
  uint8      client_id = (uint8) user_data_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
    
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_dsm_rx_wmk_low_cb client id %d", DS_MUX_STUB_GET_CLIENT_ID_FROM_INDEX(client_id));


  /* Empty for now */
} /* ds_mux_loop_back_dsm_rx_wmk_low_cb() */


/*===========================================================================

FUNCTION:  ds_mux_loop_back_dsm_tx_wmk_hi_cb

===========================================================================*/
/*!
  @brief Tx Watermark high call back function

  @return None
*/
/*=========================================================================*/
static void ds_mux_loop_back_dsm_tx_wmk_hi_cb
(
  dsm_watermark_type *msg_tx_wmk_hi, /* DSM TX high wmk pointer */
  void               *user_data_ptr  /* User data pointer */
)
{
  uint8      client_id = (uint8) user_data_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
    
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_dsm_tx_wmk_hi_cb  "
    " Recevied high watermark,Application flow is disabled client id %d ",
    DS_MUX_STUB_GET_CLIENT_ID_FROM_INDEX(client_id));

  /*-----------------------------------------------------------------------
    Flow control Handling (Enable TX flow)
    1. Stop Sending data packet
    2. Stop/cancel the timer
  -----------------------------------------------------------------------*/
  ds_mux_loop_back_config[client_id].flow_enabled = FALSE;
  ds_mux_timer_cancel( ds_mux_loop_back_config[client_id].timer_handle);

} /* ds_mux_loop_back_dsm_tx_wmk_hi_cb() */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_dsm_rx_wmk_hi_cb

===========================================================================*/
/*!
  @brief Rx Watermark high call back function

  @return None
*/
/*=========================================================================*/
static void ds_mux_loop_back_dsm_rx_wmk_hi_cb
(
  dsm_watermark_type *msg_rx_wmk_hi, /* DSM RX high wmk pointer */
  void               *user_data_ptr  /* User data pointer */
)
{
  uint8      client_id = (uint8) user_data_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
    
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_dsm_rx_wmk_hi_cb client id %d", DS_MUX_STUB_GET_CLIENT_ID_FROM_INDEX(client_id));

} /* ds_mux_loop_back_dsm_rx_wmk_hi_cb() */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_sio_transmit

===========================================================================*/
/*!
  @brief Send packet after attaching information of test type and if it is the
         first packet

  @return None
*/
/*=========================================================================*/
static void ds_mux_loop_back_sio_transmit
(
  ds_mux_loop_back_test_type               test_type,
  ds_mux_loop_back_test_config_info_type  *config_info_ptr, /* Configuration pointer    */
  dsm_item_type                          **tx_ptr           /* Packet for transmission  */
)
{
  ds_mux_stub_pkt_header pkt_header;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  

  pkt_header.test_type = test_type;
  pkt_header.pkt_len   = dsm_length_packet( *tx_ptr );
  pkt_header.index     = config_info_ptr->curr_sent_counter;

  /*-----------------------------------------------------------------------
    DS MUX test app header info (5 Bytes)
    Test Type (1) | Packet size (2) | Index (2)
  -----------------------------------------------------------------------*/
  dsm_pushdown_packed(tx_ptr, &pkt_header, sizeof(ds_mux_stub_pkt_header), DSM_DS_SMALL_ITEM_POOL);

  sio_transmit( config_info_ptr->stream_id, *tx_ptr );
}

void ds_mux_loop_back_basic_recv_number_verify
(
  dsm_item_type       * item_ptr,
  void                * user_data_ptr
)
{

  uint8                           data[DS_MUX_LOOP_BACK_BASIC_DATA_SIZE + 1];
  uint32                          pkt_len             = 0;
  uint32                          num_byte_pull       = 0;
  uint32                          recvd_number        = 0;
  uint8                           client_id = (uint8) user_data_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  memset(&data, 0x0, DS_MUX_LOOP_BACK_BASIC_DATA_SIZE + 1);
  
  /*-----------------------------------------------------------------------
    Get packet length
  -----------------------------------------------------------------------*/
  pkt_len = dsm_length_packet( item_ptr );  
  /*-----------------------------------------------------------------------
    Validate packet length, Fetch packet data and print
  -----------------------------------------------------------------------*/
  if( 0 != pkt_len )
  {
    num_byte_pull = ( pkt_len < DS_MUX_LOOP_BACK_BASIC_DATA_SIZE ) ? pkt_len : DS_MUX_LOOP_BACK_BASIC_DATA_SIZE;
    dsm_pullup_long ( &item_ptr, &data, num_byte_pull);
  }
  /*-----------------------------------------------------------------------
    Convert received number 
  -----------------------------------------------------------------------*/
  recvd_number = atoi((const char *)data);

  DS_MUX_LOG_MSG_INFO1_6( "ds_mux_loop_back_basic_recv_number_verify pkt_len %d num pulled byte %d "
    "recvd_number %d next expected index %d num_packet_drop %d client_id %d",
    pkt_len,
    num_byte_pull,
    recvd_number,
    ds_mux_loop_back_config[client_id].expected_recv_num,
    ds_mux_loop_back_config[client_id].num_packet_drop,
    (client_id + 1));

  
  /*-----------------------------------------------------------------------
    Free DSM item
  -----------------------------------------------------------------------*/
  if( item_ptr != NULL)
  {
    dsm_free_packet( &item_ptr );
  }
}/* ds_mux_loop_back_basic_recv_number_verify*/

static void ds_mux_loop_back_latency_recv_verify
(
  dsm_item_type       * item_ptr,
  void                * user_data_ptr,
  uint32                pkt_len
)
{
  uint8  data                          = 0;
  uint32 recvd_number                  = 0;
  uint64 ds_mux_latency_Test_time_diff = 0;
  uint8  client_id = (uint8) user_data_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  

  dsm_peek_byte ( item_ptr, 0, &data );

  /*-----------------------------------------------------------------------
    Convert received number 
    -----------------------------------------------------------------------*/
  recvd_number = (uint32)data;

  if ( DS_MUX_LOOP_BACK_PAYLOAD_PATTERN_SEND == recvd_number )
  {
    ds_mux_latency_Test_time_diff = vstmr_get_ustmr_full() - ds_mux_loop_back_config[client_id].latency_test_start_time;
    DS_MUX_LOG_MSG_INFO1_2( "ds_mux_loop_back_latency_recv_verify received number %d "
        "Total time %d microseconds", recvd_number, ds_mux_latency_Test_time_diff*10/192 );

    /*-----------------------------------------------------------------------
      Free DSM item
    -----------------------------------------------------------------------*/
    if( item_ptr != NULL)
    {
      dsm_free_packet( &item_ptr );
    }

    /*-----------------------------------------------------------------------
      Send packet if timer was triggered
    -----------------------------------------------------------------------*/
    if ( FALSE == ps_timer_is_running(ds_mux_loop_back_config[client_id].timer_handle) &&
         TRUE == ds_mux_loop_back_config[client_id].is_latency_timer_expired )

    {
      ds_mux_loop_back_config[client_id].is_latency_timer_expired = FALSE;
      ds_mux_loop_back_big_data_send( client_id );
    }
  }
  else
  {
    ds_mux_loop_back_sio_transmit( DS_MUX_LOOP_BACK_LATENCY_TEST,
                                   &ds_mux_loop_back_config[client_id],
                                   &item_ptr );
    DS_MUX_LOG_MSG_INFO1_2( "ds_mux_loop_back_latency_recv_verify loop back data %d client_id %d",
        recvd_number, client_id + 1);
  }

  return;
}/* ds_mux_loop_back_latency_recv_verify */

void ds_mux_loop_back_big_data_recv_verify
(
  dsm_item_type       * item_ptr,
  void                * user_data_ptr,
  uint32                pkt_len
)
{
  uint32                          num_byte_pull       = 0;
  uint32                          count               = 0;
  uint8                           client_id = (uint8) user_data_ptr;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  

  memset(&ds_mux_recv_big_buf[client_id][0], 0x0, DS_MUX_LOOP_BACK_BIG_DATA_SIZE);
  
  /*-----------------------------------------------------------------------
    Validate packet length, Fetch packet data and print
  -----------------------------------------------------------------------*/
  if( 0 != pkt_len )
  {
    num_byte_pull = ( pkt_len < DS_MUX_LOOP_BACK_BIG_DATA_SIZE ) ? pkt_len : DS_MUX_LOOP_BACK_BIG_DATA_SIZE;
    dsm_pullup_long ( &item_ptr, &ds_mux_recv_big_buf[client_id][0], num_byte_pull);
  }

  for ( count = 0; count < num_byte_pull; count++)
  {
    if( ds_mux_recv_big_buf[client_id][count] != DS_MUX_LOOP_BACK_PAYLOAD_PATTERN_RECV)
    {
      ds_mux_loop_back_config[client_id].num_packet_drop++;
      DS_MUX_LOG_MSG_ERROR_4( "ds_mux_loop_back_big_data_recv_verify Corrupt buf size %d "
        "drop count %d recv char %d expected char %d ",
          num_byte_pull, 
          ds_mux_loop_back_config[client_id].num_packet_drop,
          ds_mux_recv_big_buf[client_id][count],
          DS_MUX_LOOP_BACK_PAYLOAD_PATTERN_RECV);
      break;
    }
  }

  DS_MUX_LOG_MSG_INFO1_4( "ds_mux_loop_back_big_data_recv_verify current pkt size %d num pulled byte %d "
    "num_packet_drop %d client_id %d",
    pkt_len, 
    num_byte_pull, 
    ds_mux_loop_back_config[client_id].num_packet_drop,
    client_id + 1);
  
  /*-----------------------------------------------------------------------
    Free DSM item
  -----------------------------------------------------------------------*/
  if( item_ptr != NULL)
  {
    dsm_free_packet( &item_ptr );
  }
}/* ds_mux_loop_back_big_data_recv_verify */

void ds_mux_loop_integrity_data_recv_verify
(
  dsm_item_type       * item_ptr,
  void                * user_data_ptr
)
{
  uint32                          pkt_len       = 0;
  ds_mux_stub_integrity_test_info test_info     = {0};
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  

  /*-----------------------------------------------------------------------
    Get packet length
  -----------------------------------------------------------------------*/
  pkt_len = dsm_length_packet( item_ptr );  
  /*-----------------------------------------------------------------------
    Validate packet length, Fetch packet data and print
  -----------------------------------------------------------------------*/
  if( 0 != pkt_len )
  {
    dsm_pullup_long ( &item_ptr, &test_info, sizeof(test_info) );
  }

  if ( 0 !=  memcmp(&test_info, 
                    &ds_mux_integrity_test_info,
                    sizeof(ds_mux_stub_integrity_test_info)) )
  {
    DS_MUX_LOG_MSG_ERROR_6( "ds_mux_loop_integrity_data_recv_verify Corrupt test_val1 %d "
      "test_val2 %d test_val4 %d test_val5 %d test_val7 %d test_val8 %d",
      test_info.test_val1,
      test_info.test_val2,
      test_info.test_val4,
      test_info.test_struct1.test_val5,
      test_info.test_struct2.test_val7,
      test_info.test_struct2.test_val8);
  }
  
  /*-----------------------------------------------------------------------
    Free DSM item
  -----------------------------------------------------------------------*/
  if( item_ptr != NULL)
  {
    dsm_free_packet( &item_ptr );
  }
}/* ds_mux_loop_integrity_data_recv_verify */

static ds_mux_loop_back_parse_header_err_type ds_mux_loop_back_parse_header
(
  uint8                    client_id,
  dsm_item_type          **tx_ptr,
  ds_mux_stub_pkt_header  *pkt_header
)
{
  uint16 pkt_len = 0;
  uint8  data    = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  dsm_pullup_long ( tx_ptr, pkt_header, sizeof(ds_mux_stub_pkt_header));
  pkt_len = dsm_length_packet( *tx_ptr );  

  /*-----------------------------------------------------------------------
    Validate packet size
  -----------------------------------------------------------------------*/
  if ( pkt_len != pkt_header->pkt_len )
  {
    DS_MUX_LOG_MSG_ERROR_2( "ds_mux_loop_back_rx_wmk_non_empty_func packet may have less bytes "
      "expected size %d recvd %d", pkt_header->pkt_len, pkt_len);

    ds_mux_loop_back_config[client_id].num_packet_drop++;
    return DS_MUX_LOOP_BACK_PARSE_HEADER_ERROR;
  }

  DS_MUX_LOG_MSG_INFO1_2( "ds_mux_loop_back_parse_header received packet client_id %d index %d",
    (client_id + 1), pkt_header->index);

  /*-----------------------------------------------------------------------
    Validate packet index
  -----------------------------------------------------------------------*/
  dsm_peek_byte ( *tx_ptr, 0, &data );

  if ( DS_MUX_LOOP_BACK_PAYLOAD_PATTERN_SEND == (uint32)data )
  {
    /*-----------------------------------------------------------------------
      Returned packet for latency test
    -----------------------------------------------------------------------*/
    ds_mux_loop_back_config[client_id].is_latency_pkt_returned = TRUE;
  }
  else if ( 1 == pkt_header->index )
  {
    ds_mux_loop_back_config[client_id].expected_recv_num = 2;
  }
  else if ( ds_mux_loop_back_config[client_id].expected_recv_num != pkt_header->index )
  {
    ds_mux_loop_back_config[client_id].num_packet_drop++;
    DS_MUX_LOG_MSG_ERROR_5( "ds_mux_loop_back_parse_header pkt_len %d "
      "recvd_index %d expected_index %d num_packet_drop %d client_id %d",
      pkt_len,
      pkt_header->index,
      ds_mux_loop_back_config[client_id].expected_recv_num,
      ds_mux_loop_back_config[client_id].num_packet_drop,
      (client_id + 1));

    ds_mux_loop_back_config[client_id].expected_recv_num = pkt_header->index + 1;
  }
  else
  {
    ds_mux_loop_back_config[client_id].expected_recv_num++;
  }

  return DS_MUX_LOOP_BACK_PARSE_HEADER_OK;
}/* ds_mux_loop_back_parse_header */

void  ds_mux_loop_back_rx_wmk_non_empty_func
(
  dsm_watermark_type  * ds_mux_rx_wmk,
  void                * user_data_ptr
)
{
  uint8                  client_id = (uint8) user_data_ptr;
  dsm_item_type          *item_ptr;            /* Packet for transmission       */
  ds_mux_stub_pkt_header pkt_header;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/  
  if (( NULL == (item_ptr = dsm_dequeue( &ds_mux_loop_rx_wmk[client_id] ))))
  {
    /*-----------------------------------------------------------------------
      Either queue is empty or phy rx wmk become NULL. 
    -----------------------------------------------------------------------*/
    return;
  }

  if ( DS_MUX_LOOP_BACK_PARSE_HEADER_OK != ds_mux_loop_back_parse_header( client_id, &item_ptr, &pkt_header ) )
  {
    return;
  }

  switch ( pkt_header.test_type )
  {
    case DS_MUX_LOOP_BACK_BIG_DATA_TEST:
    case DS_MUX_LOOP_BACK_POWER_SAVE_TEST:
    case DS_MUX_LOOP_BACK_FLOW_CTL_TEST:
    {
      ds_mux_loop_back_big_data_recv_verify( item_ptr, user_data_ptr, pkt_header.pkt_len );
      break;
    }
    case DS_MUX_LOOP_BACK_LATENCY_TEST:
    {
      ds_mux_loop_back_latency_recv_verify( item_ptr, user_data_ptr, pkt_header.pkt_len );
      break;
    }
    case DS_MUX_LOOP_BACK_INTEGRITY_TEST:
    {
      ds_mux_loop_integrity_data_recv_verify( item_ptr, user_data_ptr );
      break;
    }
    case DS_MUX_LOOP_BACK_BASIC_TEST:
    case DS_MUX_LOOP_BACK_REGRESSION_TEST:
    default:
    {
      ds_mux_loop_back_basic_recv_number_verify( item_ptr, user_data_ptr );
      break;
    }
  }

}/* ds_mux_loop_back_rx_wmk_non_empty_func */


/*===========================================================================

FUNCTION:  msgr_sio_dtr_changed_cb

===========================================================================*/
/*!
  @brief DTR status call back function

  @return None
*/
/*=========================================================================*/

 void ds_mux_loop_back_sio_dtr_changed_cb_1
(
  void
)
{
  /* Initialize SIO ioctl parameters with zeros */
  sio_ioctl_param_type  ioctl_param = {0};
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  if( SIO_NO_STREAM_ID == ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].stream_id)
  {
    return;
  }
  /* Set DTE ready asserted parameter */
  ioctl_param.dte_ready_asserted = &ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].dte_ready_asserted;

  /*-------------------------------------------------------------------------
   Call SIO IOCTL using DTE ready asserted command 
  ------------------------------------------------------------------------*/
  sio_ioctl( ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].stream_id, SIO_IOCTL_DTE_READY_ASSERTED,
             &ioctl_param );

  DS_MUX_LOG_MSG_INFO1_2( "ds_mux_loop_back_sio_dtr_changed_cb_1 curr dtr status %d timer handle %d",
   ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].dte_ready_asserted,
   ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].timer_handle);
  
} /* ds_mux_loop_back_sio_dtr_changed_cb_1() */

/* ===========================================================================*/
 /*!
   @brief DTR status call back function
 
   @return None
 */
 /*=========================================================================*/
 
  void ds_mux_loop_back_sio_dtr_changed_cb_2
 (
   void
 )
 {
   /* Initialize SIO ioctl parameters with zeros */
   sio_ioctl_param_type  ioctl_param = {0};
   /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
   if( SIO_NO_STREAM_ID == ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].stream_id)
   {
     return;
   }
   /* Set DTE ready asserted parameter */
   ioctl_param.dte_ready_asserted = &ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].dte_ready_asserted;
 
   /*-------------------------------------------------------------------------
    Call SIO IOCTL using DTE ready asserted command 
   ------------------------------------------------------------------------*/
   sio_ioctl( ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].stream_id,
              SIO_IOCTL_DTE_READY_ASSERTED,
              &ioctl_param );
 
   if ( TRUE == ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].dte_ready_asserted )
   {
     if (DS_MUX_LOOP_BACK_SSR_CTL_TEST == ds_mux_loop_back_config[0].test_type &&
        ds_mux_loop_back_config[0].curr_ssr_itr < ds_mux_loop_back_config[0].num_ssr_itr )
     {
       ds_mux_shut_down_cb();
       
     }
   }

   DS_MUX_LOG_MSG_INFO1_2( "ds_mux_loop_back_sio_dtr_changed_cb_2 curr dtr status %d timer handle %d",
    ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].dte_ready_asserted,
    ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].timer_handle);
     
 } /* ds_mux_loop_back_sio_dtr_changed_cb_2() */

 void ds_mux_loop_back_timer_cb
(
  void *  data
)
{
  ds_muxi_cmd_info_type  * ds_mux_cmd_info_ptr;
  uint8                    client_id = (uint8) data;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/ 

  DS_MUX_LOG_MSG_INFO1_2( "ds_mux_loop_back_timer_cb  ds_mux_loop_stream_id %d, client_id %d",
   (uint8) ds_mux_loop_back_config[client_id].stream_id, DS_MUX_STUB_GET_CLIENT_ID_FROM_INDEX(client_id));

  /*-------------------------------------------------------------------------
 Allocate DS MUX cmd buf, populate it and post cmd to DS MUX task
------------------------------------------------------------------------*/
  DS_MUX_MEM_ALLOC(ds_mux_cmd_info_ptr, sizeof(ds_muxi_cmd_info_type),
                            ds_muxi_cmd_info_type*);

  if (NULL == ds_mux_cmd_info_ptr)
  {
    return;
  }
  ds_mux_cmd_info_ptr->cmd_id                          = DS_MUXI_LOOP_BACK_TEST;
  ds_mux_cmd_info_ptr->cmd_info.test_app_timer_cb_info.client_id = client_id;
  ds_mux_send_cmd(DS_MUX_CMD, ds_mux_cmd_info_ptr);

}/* ds_mux_loop_back_timer_cb */

void ds_mux_loop_back_timer_handler
(
  uint8    client_id
)
{

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-------------------------------------------------------------------------
    Stop sending if flow is disabled
  ------------------------------------------------------------------------*/
  if ( FALSE == ds_mux_loop_back_config[client_id].flow_enabled )
  {
    return;
  }

  if( SIO_NO_STREAM_ID == ds_mux_loop_back_config[client_id].stream_id)
  {
   DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_timer_handler  invalid stream id %d",
    ds_mux_loop_back_config[client_id].stream_id);
    return;
  }

  switch ( ds_mux_loop_back_config[client_id].test_type )
  {
    case DS_MUX_LOOP_BACK_BIG_DATA_TEST:
    case DS_MUX_LOOP_BACK_POWER_SAVE_TEST:
    case DS_MUX_LOOP_BACK_FLOW_CTL_TEST:
    {
      ds_mux_loop_back_big_data_send( client_id );
      break;
    }
   	case DS_MUX_LOOP_BACK_LATENCY_TEST:
    {
      if (TRUE == ds_mux_loop_back_config[client_id].is_latency_pkt_returned)
      {
        ds_mux_loop_back_big_data_send( client_id );
      }
      else
      {
        ds_mux_loop_back_config[client_id].is_latency_timer_expired = TRUE;
      }
      break;
    }
    case DS_MUX_LOOP_BACK_INTEGRITY_TEST:
    {
      ds_mux_integrity_loop_back_send( client_id );
      break;
    }
    case DS_MUX_LOOP_BACK_BASIC_TEST:
    case DS_MUX_LOOP_BACK_REGRESSION_TEST:
    default:
    {
      ds_mux_basic_loop_back_send( client_id );
      break;
    }
  }
}/* ds_mux_loop_back_timer_handler */

void  ds_mux_loop_back_big_data_send
(
    uint8    client_id
)
{
  dsm_item_type           *resp_msg         = NULL;
  dsm_item_type          **tx_ptr           = NULL;
  uint32                   buf_len          = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  buf_len = ds_mux_loop_back_config[client_id].min_num_bytes +
    ((ds_mux_loop_back_config[client_id].curr_sent_counter - 1) * ds_mux_loop_back_config[client_id].increment_num_bytes);

  if( 0 == buf_len || buf_len > DS_MUX_LOOP_BACK_BIG_DATA_SIZE)
  {
    DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_big_data_send  invalid buf len %d", buf_len);
    return;
  }

  /*-------------------------------------------------------------------------
   Allocate DSM item (Small DSM item)
  ------------------------------------------------------------------------*/
  resp_msg = dsm_new_buffer(DSM_DS_LARGE_ITEM_POOL);

  if ( NULL  == resp_msg)
  {
    DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_big_data_send  DSM alloc fail for client_id %d",
       (client_id + 1));
    return;
  }
  tx_ptr = &resp_msg;

  /*-------------------------------------------------------------------------
   Copy data into DSM item
  ------------------------------------------------------------------------*/
  dsm_pushdown_tail( tx_ptr, &ds_mux_big_buf[client_id][0], buf_len, DSM_DS_LARGE_ITEM_POOL);

  /*-------------------------------------------------------------------------
   If DTR is high: Transmit the data else free the DSM item
  ------------------------------------------------------------------------*/
  if( TRUE == ds_mux_loop_back_config[client_id].dte_ready_asserted)
  {
    if (DS_MUX_LOOP_BACK_LATENCY_TEST == ds_mux_loop_back_config[client_id].test_type)
    {
      ds_mux_loop_back_config[client_id].latency_test_start_time = vstmr_get_ustmr_full();
      ds_mux_loop_back_config[client_id].is_latency_pkt_returned = FALSE;
    }
    ds_mux_loop_back_sio_transmit( ds_mux_loop_back_config[client_id].test_type,
                                   &ds_mux_loop_back_config[client_id],
                                   tx_ptr );
  }
  else
  {
   DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_big_data_send  failed to trannsmit DTR status %d",
     ds_mux_loop_back_config[client_id].dte_ready_asserted);
    dsm_free_packet( tx_ptr);
  }
  DS_MUX_LOG_MSG_INFO1_6( "ds_mux_big_data_loop_back_send client id %d curr sent count %d "
    "time interval %d  max packets %d test type %d buf len %d",
     client_id + 1,
     ds_mux_loop_back_config[client_id].curr_sent_counter,
     ds_mux_loop_back_config[client_id].time_interval,
     ds_mux_loop_back_config[client_id].max_num_packet,
     ds_mux_loop_back_config[client_id].test_type,
     buf_len);

  /*-------------------------------------------------------------------------
   Restart the timer again if number of send is less then MAX number of packets 
  ------------------------------------------------------------------------*/
  if ( TRUE == ds_mux_loop_back_config[client_id].dte_ready_asserted &&
      ds_mux_loop_back_config[client_id].curr_sent_counter < ds_mux_loop_back_config[client_id].max_num_packet)
  {
    ds_mux_loop_back_config[client_id].curr_sent_counter++;
    ds_mux_timer_start( ds_mux_loop_back_config[client_id].timer_handle, ds_mux_loop_back_config[client_id].time_interval );
  }
    
}/* ds_mux_big_data_loop_back_send */

void  ds_mux_basic_loop_back_send
(
  uint8    client_id
)
{
  dsm_item_type*           resp_msg         = NULL;
  dsm_item_type          **tx_ptr           = NULL;
  uint8                    buf_len          = DS_MUX_LOOP_BACK_BASIC_DATA_SIZE;
  uint8                    data_buffer[DS_MUX_LOOP_BACK_BASIC_DATA_SIZE];
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  /*-------------------------------------------------------------------------
   Allocate DSM item (Small DSM item)
  ------------------------------------------------------------------------*/
  resp_msg = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);

  if ( NULL  == resp_msg)
  {
    DS_MUX_LOG_MSG_INFO1_1( "ds_mux_basic_loop_back_send  mem alloc fail for stream id %d",
       ds_mux_loop_back_config[client_id].stream_id);
    return;
  }
  tx_ptr = &resp_msg;

  memset(&data_buffer, 0x0, DS_MUX_LOOP_BACK_BASIC_DATA_SIZE);
  /*-------------------------------------------------------------------------
   Copy current send count value so that we can review the packet loss
  ------------------------------------------------------------------------*/
  buf_len = snprintf((char *)&data_buffer,DS_MUX_LOOP_BACK_BASIC_DATA_SIZE, "%d", 
    (int)ds_mux_loop_back_config[client_id].curr_sent_counter);
  /*-------------------------------------------------------------------------
   Copy data into DSM item
  ------------------------------------------------------------------------*/
  dsm_pushdown_tail( tx_ptr, &data_buffer[0], buf_len, DSM_DS_SMALL_ITEM_POOL);

  /*-------------------------------------------------------------------------
   If DTR is high: Transmit the data else free the DSM item
  ------------------------------------------------------------------------*/
  if( TRUE == ds_mux_loop_back_config[client_id].dte_ready_asserted)
  {
    ds_mux_loop_back_sio_transmit( ds_mux_loop_back_config[client_id].test_type,
                                   &ds_mux_loop_back_config[client_id],
                                   tx_ptr );
  }
  else
  {
   DS_MUX_LOG_MSG_INFO1_1( "ds_mux_basic_loop_back_send  failed to trannsmit DTR status %d",
    ds_mux_loop_back_config[client_id].dte_ready_asserted);
    dsm_free_packet( tx_ptr);
  }

  DS_MUX_LOG_MSG_INFO1_5( "ds_mux_basic_loop_back_send client id %d curr sent count %d "
    "time interval %d  max packets %d test type %d",
     client_id + 1,
     ds_mux_loop_back_config[client_id].curr_sent_counter,
     ds_mux_loop_back_config[client_id].time_interval,
     ds_mux_loop_back_config[client_id].max_num_packet,
     ds_mux_loop_back_config[client_id].test_type);

  /*-------------------------------------------------------------------------
   Restart the timer again if number of send is less then MAX number of packets 
  ------------------------------------------------------------------------*/
  if ( TRUE == ds_mux_loop_back_config[client_id].dte_ready_asserted &&
      (ds_mux_loop_back_config[client_id].test_type == DS_MUX_LOOP_BACK_REGRESSION_TEST ||
       ds_mux_loop_back_config[client_id].curr_sent_counter < (ds_mux_loop_back_config[client_id].max_num_packet - 1)) )
  {
    ds_mux_loop_back_config[client_id].curr_sent_counter = ds_mux_loop_back_config[client_id].curr_sent_counter + 1;
    ds_mux_timer_start( ds_mux_loop_back_config[client_id].timer_handle, ds_mux_loop_back_config[client_id].time_interval );
  }
}/* ds_mux_basic_loop_back_send */

void  ds_mux_integrity_loop_back_send
(
  uint8    client_id
)
{
  dsm_item_type                   *resp_msg         = NULL;
  dsm_item_type                  **tx_ptr           = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Allocate DSM item (Small DSM item)
  ------------------------------------------------------------------------*/
  resp_msg = dsm_new_buffer(DSM_DS_LARGE_ITEM_POOL);

  if ( NULL  == resp_msg)
  {
    DS_MUX_LOG_MSG_INFO1_1( "ds_mux_integrity_loop_back_send  mem alloc fail for stream id %d",
       ds_mux_loop_back_config[client_id].stream_id);
    return;
  }
  tx_ptr = &resp_msg;

  /*-------------------------------------------------------------------------
   Copy data into DSM item
  ------------------------------------------------------------------------*/
  dsm_pushdown_tail( tx_ptr, &ds_mux_integrity_test_info, sizeof(ds_mux_integrity_test_info), DSM_DS_LARGE_ITEM_POOL);

  /*-------------------------------------------------------------------------
   If DTR is high: Transmit the data else free the DSM item
  ------------------------------------------------------------------------*/
  if( TRUE == ds_mux_loop_back_config[client_id].dte_ready_asserted)
  {
    ds_mux_loop_back_sio_transmit( ds_mux_loop_back_config[client_id].test_type,
                                   &ds_mux_loop_back_config[client_id], 
                                   tx_ptr );
  }
  else
  {
   DS_MUX_LOG_MSG_INFO1_1( "ds_mux_integrity_loop_back_send  failed to trannsmit DTR status %d",
     ds_mux_loop_back_config[client_id].dte_ready_asserted);
    dsm_free_packet( tx_ptr);
  }
  
  /*-------------------------------------------------------------------------
   Restart the timer again if number of send is less then MAX number of packets 
  ------------------------------------------------------------------------*/
  if ( TRUE == ds_mux_loop_back_config[client_id].dte_ready_asserted &&
      ds_mux_loop_back_config[client_id].curr_sent_counter < ds_mux_loop_back_config[client_id].max_num_packet)
  {
    ds_mux_loop_back_config[client_id].curr_sent_counter++;
    ds_mux_timer_start( ds_mux_loop_back_config[client_id].timer_handle, ds_mux_loop_back_config[client_id].time_interval );
  }
    
}/* ds_mux_integrity_loop_back_send */

/*===========================================================================

FUNCTION:  ds_mux_reset_current_testcase
===========================================================================*/
void ds_mux_reset_current_testcase
(
  uint8 index
)
{
  /*-------------------------------------------------------------------------
    Cancel timer to stop sending
  ------------------------------------------------------------------------*/
  ds_mux_timer_cancel( ds_mux_loop_back_config[index].timer_handle);

  /*-------------------------------------------------------------------------
   Reset all counters
  ------------------------------------------------------------------------*/
  ds_mux_loop_back_config[index].time_interval           = 0;
  ds_mux_loop_back_config[index].min_num_bytes           = 0;
  ds_mux_loop_back_config[index].increment_num_bytes     = 0;
  ds_mux_loop_back_config[index].latency_test_start_time = 0;
  ds_mux_loop_back_config[index].expected_recv_num       = 1;
  ds_mux_loop_back_config[index].curr_sent_counter       = 1;
  ds_mux_loop_back_config[index].num_packet_drop         = 0;
  ds_mux_loop_back_config[index].max_num_packet          = 0;
  ds_mux_loop_back_config[index].is_latency_pkt_returned = TRUE;
  ds_mux_loop_back_config[index].is_latency_timer_expired = FALSE;

  /*-------------------------------------------------------------------------
    Reset the watermark value to original  value
  ------------------------------------------------------------------------*/
  if ( NULL != ds_mux_phys_conn_info )
  {
    dsm_set_low_wm( &ds_mux_phys_conn_info->tx_wmk, DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_LO );
    dsm_set_hi_wm( &ds_mux_phys_conn_info->tx_wmk, DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_HI );
  }
  
  dsm_set_low_wm( &ds_mux_loop_tx_wmk[index], DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_LO );
  dsm_set_hi_wm( &ds_mux_loop_tx_wmk[index], DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_HI );

  DS_MUX_LOG_MSG_ERROR_2("Reseted previous testcase results"
      " and stoping current testcase %d  Client %d",
      ds_mux_loop_back_config[index].test_type, (index + 1));

}/* ds_mux_reset_current_testcase*/


/*===========================================================================

FUNCTION:  ds_mux_reset_current_testcase
===========================================================================*/
void ds_mux_loop_back_ssr_testcase
(
  uint16    ssr_type,
  uint16    itr
)
{

       ds_mux_loop_back_config[0].curr_ssr_itr = 0;
       ds_mux_loop_back_config[1].curr_ssr_itr = 0;
    /*
         DS_MUX_LOOP_BACK_SSR_POWER_UP_DOWN  First do power down and wait for SIO CLOSE
          Once SIO close call back received from DS MUX do power UP
          wait for DTR to be HIGH; Once DTR is High to power down for all iteration 
    
      */
  switch( ssr_type )
  {
    case DS_MUX_LOOP_BACK_SSR_POWER_DOWN:
    {
      ds_mux_loop_back_config[0].num_ssr_itr = 0;
      ds_mux_loop_back_config[1].num_ssr_itr = 0;
      ds_mux_shut_down_cb();
      break;
    }

    case DS_MUX_LOOP_BACK_SSR_POWER_UP:
    {
      ds_mux_loop_back_config[0].num_ssr_itr  = 0 ;
      ds_mux_loop_back_config[1].num_ssr_itr = 0;
      ds_mux_power_up_cb();
      break;
    }
    case DS_MUX_LOOP_BACK_SSR_POWER_UP_DOWN:
    {
       ds_mux_shut_down_cb();
    }
    default :
    {
        
    }
  }

}/* ds_mux_loop_back_ssr_testcase */

/*===========================================================================

FUNCTION:  ds_mux_diag_command_notify_cb
===========================================================================*/

void ds_mux_diag_command_notify_cb
(
  uint16                      cmd_id,
  ps_logging_diag_input_type  input_params
)
{

  uint16 arg_val       = 0;
  uint16 ssr_itr       = 0;
  uint16 client_id     = 0;
  uint32 max_num_bytes = 0;
  uint8  index           = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  /*-------------------------------------------------------------------------
   Based on diag command argument set the loop back test configuration
   Arg 0 = client ID (starts from 1 but internally starts from 0)
  ------------------------------------------------------------------------*/
  client_id = *(uint16*)&input_params.arg_list[0];
  if ( !DS_MUX_STUB_IS_VALID_CLIENT_ID(client_id) )
  {
    DS_MUX_LOG_MSG_ERROR_1("Client ID should be 1 or 2 client Id %d", client_id);
    return;
  }
  index = client_id - 1;

  
  /*-------------------------------------------------------------------------
   Arg 1 = Test case type
  ------------------------------------------------------------------------*/
  arg_val = *(uint16*)&input_params.arg_list[2];

  if ( DS_MUX_LOOP_BACK_SSR_CTL_TEST  == arg_val)
  {
    ds_mux_loop_back_config[0].test_type = DS_MUX_LOOP_BACK_SSR_CTL_TEST;
    ds_mux_loop_back_config[1].test_type = DS_MUX_LOOP_BACK_SSR_CTL_TEST;
    
    //SSR test case
    arg_val = *(uint16*)&input_params.arg_list[4];
    ssr_itr  = *(uint16*)&input_params.arg_list[6];

    ds_mux_loop_back_config[0].num_ssr_itr = ssr_itr;
    ds_mux_loop_back_config[1].num_ssr_itr = ssr_itr;

    DS_MUX_LOG_MSG_INFO1_2(" ds_mux_diag_command_notify_cb SSR ARG %d ssr itr %d ", arg_val, ssr_itr);

    ds_mux_loop_back_ssr_testcase( arg_val, ssr_itr);

    return;
  }


  if( DS_MUX_LOOP_BACK_RESET_TEST  != arg_val &&
      DS_MUX_LOOP_BACK_RESET_TEST != ds_mux_loop_back_config[index].test_type )
  {
     DS_MUX_LOG_MSG_ERROR_1(" Reset the previous test case result, Send ReSet command %d", arg_val);
     return;
  }
  
  if ( arg_val < DS_MUX_LOOP_BACK_MAX_TEST)
  {
    ds_mux_loop_back_config[index].test_type = arg_val;
  }
  else
  {
    DS_MUX_LOG_MSG_ERROR_1(" Invalid testcase arg_val %d", arg_val);
    return;
  }

  /*-------------------------------------------------------------------------
    Flow control testcase: Set lower watermarks High and low value so that we can hit
    flow control early.
  ------------------------------------------------------------------------*/
  if( DS_MUX_LOOP_BACK_FLOW_CTL_TEST == ds_mux_loop_back_config[index].test_type )
  {
    if ( NULL != ds_mux_phys_conn_info )
    {
      dsm_set_low_wm( &ds_mux_phys_conn_info->tx_wmk, DS_MUX_LOOP_BACK_DSM_WM_FLOW_CONTROL_TEST_LO );
      dsm_set_hi_wm( &ds_mux_phys_conn_info->tx_wmk, DS_MUX_LOOP_BACK_DSM_WM_FLOW_CONTROL_TEST_HI );
    }
    dsm_set_low_wm( &ds_mux_loop_tx_wmk[index], DS_MUX_LOOP_BACK_DSM_WM_FLOW_CONTROL_TEST_LO );
    dsm_set_hi_wm( &ds_mux_loop_tx_wmk[index], DS_MUX_LOOP_BACK_DSM_WM_FLOW_CONTROL_TEST_HI );
  }

  /*-------------------------------------------------------------------------
   Reset all field and stop running test case for DS_MUX_LOOP_BACK_RESET_TEST.
  ------------------------------------------------------------------------*/
  if( DS_MUX_LOOP_BACK_RESET_TEST == ds_mux_loop_back_config[index].test_type )
  {
    ds_mux_reset_current_testcase( index );
    return;
  }

  /*-------------------------------------------------------------------------
   Arg 2 = Number of packets 
   Regression Testcase will send/recive packets until reset command will send by the user.
  ------------------------------------------------------------------------*/
  arg_val = *(uint16*)&input_params.arg_list[4];
  if ( arg_val != 0 )
  {
    ds_mux_loop_back_config[index].max_num_packet = arg_val;
  }
  else if (DS_MUX_LOOP_BACK_REGRESSION_TEST != ds_mux_loop_back_config[index].test_type)
  {
    DS_MUX_LOG_MSG_ERROR_1("The maximum number of pakcets can't be %d.", arg_val);
    return;
  }

  /*-------------------------------------------------------------------------
   Arg 4 = Time interval (ms) in case of power save, timer should expire
   after mux is in power save mode
  ------------------------------------------------------------------------*/
  arg_val = *(uint16*)&input_params.arg_list[6];
  if ( arg_val != 0 )
  {
    ds_mux_loop_back_config[index].time_interval = arg_val;
  }

  if (DS_MUX_LOOP_BACK_POWER_SAVE_TEST == ds_mux_loop_back_config[index].test_type)
  {
    ds_mux_loop_back_config[index].time_interval += DS_MUX_INACTIVITY_TIMER;
    DS_MUX_LOG_MSG_INFO1_1(" Test App will add inactivity timeout %d into time interval "
                           " to send packet after powersave ",DS_MUX_INACTIVITY_TIMER);
  }
  else if ( arg_val == 0)
  {
    ds_mux_loop_back_config[index].time_interval = DS_MUX_LOOP_BACK_TIMER;
  }

  /*-------------------------------------------------------------------------
   Arg 4 = (Minimum) Packet Size (bytes)
  ------------------------------------------------------------------------*/
  arg_val = *(uint16*)&input_params.arg_list[8];
  if ( arg_val != 0 )
  {
    ds_mux_loop_back_config[index].min_num_bytes  = arg_val;
  }

  /*-------------------------------------------------------------------------
   Arg 5 = Maximum Packet Size (bytes)
  ------------------------------------------------------------------------*/
  arg_val = *(uint16*)&input_params.arg_list[10];
  if ( arg_val != 0 )
  {
    max_num_bytes  = arg_val;
  }

  /*-------------------------------------------------------------------------
   Validate the max and min packet size
  ------------------------------------------------------------------------*/
  if (ds_mux_loop_back_config[index].min_num_bytes > max_num_bytes)
  {
    DS_MUX_LOG_MSG_ERROR_0("The maximum size of packets should be equal to or higher than "
        "the minimum.");
    return;
  }

  if (ds_mux_loop_back_config[index].max_num_packet > 1)
  {
    ds_mux_loop_back_config[index].increment_num_bytes =
      (max_num_bytes - ds_mux_loop_back_config[index].min_num_bytes) / (ds_mux_loop_back_config[index].max_num_packet - 1);
  }

  /*-------------------------------------------------------------------------
   Start the test case only when DTR is high and NUM packets are not zero
  ------------------------------------------------------------------------*/
  if ( SIO_NO_STREAM_ID != ds_mux_loop_back_config[index].stream_id&& 
       TRUE == ds_mux_loop_back_config[index].dte_ready_asserted  )
  {
    /*-------------------------------------------------------------------------
      Start the timer and send the packet on first expire. So that Other DS MUX test app
      can be configured to right testcase
      -----------------------------------------------------------------------*/
   ds_mux_timer_start( ds_mux_loop_back_config[index].timer_handle,
                       ds_mux_loop_back_config[index].time_interval );
  }

  DS_MUX_LOG_MSG_INFO1_9( "ds_mux_diag_command_notify_cb client_id %d stream_id %d "
                          "dtr status %d test type %d num itr %d min bytes %d "
                          "max bytes %d inc bytes %d time interval %d ",
         client_id, 
 (uint8) ds_mux_loop_back_config[index].stream_id, 
         ds_mux_loop_back_config[index].dte_ready_asserted,
         ds_mux_loop_back_config[index].test_type,
         ds_mux_loop_back_config[index].max_num_packet,
         ds_mux_loop_back_config[index].min_num_bytes,
         max_num_bytes,
         ds_mux_loop_back_config[index].increment_num_bytes,
         ds_mux_loop_back_config[index].time_interval);

}/* ds_mux_diag_command_notify_cb */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_init_config

===========================================================================*/
/*!
  @brief Init configuration

  @return None
*/
/*=========================================================================*/
void ds_mux_loop_back_init_config
(
  uint32 client_id
)
{

  ds_mux_loop_back_test_type  prev_test_type;
  uint16                      prev_curr_ssr_itr;
  uint16                      prev_num_ssr_itr;

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  prev_test_type = ds_mux_loop_back_config[client_id].test_type;
  prev_curr_ssr_itr = ds_mux_loop_back_config[client_id].curr_ssr_itr;
  prev_num_ssr_itr  = ds_mux_loop_back_config[client_id].num_ssr_itr;
  
  /*-------------------------------------------------------------------------
    Initialize configuration
  ------------------------------------------------------------------------*/
  memset(&ds_mux_loop_back_config[client_id], 0x0, sizeof(ds_mux_loop_back_test_config_info_type));

  ds_mux_loop_back_config[client_id].test_type     = DS_MUX_LOOP_BACK_RESET_TEST;
  ds_mux_loop_back_config[client_id].timer_handle  = 0;
  ds_mux_loop_back_config[client_id].port_id       = 0;
  ds_mux_loop_back_config[client_id].stream_id     = SIO_NO_STREAM_ID;
  ds_mux_loop_back_config[client_id].dte_ready_asserted = 1;
  ds_mux_loop_back_config[client_id].expected_recv_num  = 1;
  ds_mux_loop_back_config[client_id].curr_sent_counter  = 1;
  ds_mux_loop_back_config[client_id].num_packet_drop    = 0;
  ds_mux_loop_back_config[client_id].flow_enabled       = TRUE;
  ds_mux_loop_back_config[client_id].is_latency_pkt_returned  = TRUE;
  ds_mux_loop_back_config[client_id].is_latency_timer_expired = FALSE;
  ds_mux_loop_back_config[client_id].time_interval            = 2000;
  ds_mux_loop_back_config[client_id].max_num_packet           = 100;

  //Reset previous info
  if ( DS_MUX_LOOP_BACK_SSR_CTL_TEST == prev_test_type)
  {
    ds_mux_loop_back_config[client_id].test_type     =  prev_test_type;
    ds_mux_loop_back_config[client_id].curr_ssr_itr  =  prev_curr_ssr_itr;
    ds_mux_loop_back_config[client_id].num_ssr_itr   =  prev_num_ssr_itr;
  }
  
} /* ds_mux_loop_back_init_config */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_init

===========================================================================*/
/*!
  @brief Init function

  @return None
*/
/*=========================================================================*/
void   ds_mux_loop_back_init
(
  void
)
{
  rdm_service_params_type  rdm_params;
  uint32                   client_id = 0;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  for (client_id = 0; client_id < DS_MUX_MAX_TEST_APP_CLIENT; client_id++)
  {
  /*-----------------------------------------------------------------------
    Initialize configuration
  -----------------------------------------------------------------------*/ 
  ds_mux_loop_back_init_config(client_id);

  /*-------------------------------------------------------------------------
   Initialized buffers to verify if payload is not corrupted 
  ------------------------------------------------------------------------*/
  memset( &ds_mux_big_buf[client_id][0], DS_MUX_LOOP_BACK_PAYLOAD_PATTERN_SEND, DS_MUX_LOOP_BACK_BIG_DATA_SIZE );
  ds_mux_integrity_test_info.test_val1              = DS_MUX_TEST_INTEGRITY_TEST1_VALUE;
  ds_mux_integrity_test_info.test_val2              = DS_MUX_TEST_INTEGRITY_TEST2_VALUE;
  strlcpy((char *)ds_mux_integrity_test_info.test_val3,
          DS_MUX_TEST_INTEGRITY_TEST3_VALUE,
          sizeof(ds_mux_integrity_test_info.test_val3));
  ds_mux_integrity_test_info.test_val4              = DS_MUX_TEST_INTEGRITY_TEST4_VALUE;
  ds_mux_integrity_test_info.test_struct1.test_val5 = DS_MUX_TEST_INTEGRITY_TEST5_VALUE;
  strlcpy((char *)ds_mux_integrity_test_info.test_struct1.test_val6,
          DS_MUX_TEST_INTEGRITY_TEST6_VALUE,
          sizeof(ds_mux_integrity_test_info.test_struct1.test_val6));
  ds_mux_integrity_test_info.test_struct2.test_val7 = DS_MUX_TEST_INTEGRITY_TEST7_VALUE;
  ds_mux_integrity_test_info.test_struct2.test_val8 = DS_MUX_TEST_INTEGRITY_TEST8_VALUE;
  ds_mux_integrity_test_info.test_val9              = DS_MUX_TEST_INTEGRITY_TEST9_VALUE;
  strlcpy((char *)ds_mux_integrity_test_info.test_val10,
          DS_MUX_TEST_INTEGRITY_TEST10_VALUE,
          sizeof(ds_mux_integrity_test_info.test_val10));

  /* Step 1 */
  dsm_queue_init ( &ds_mux_loop_tx_wmk[client_id],
                   0x7FFFFFFF,
                   &ds_mux_loop_tx_q[client_id] );
  
  dsm_set_low_wm( &ds_mux_loop_tx_wmk[client_id], DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_LO );
  dsm_set_hi_wm( &ds_mux_loop_tx_wmk[client_id], DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_HI );
  dsm_set_dne( &ds_mux_loop_tx_wmk[client_id], DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_DNE );
  
  ds_mux_loop_tx_wmk[client_id].lowater_func_ptr = ds_mux_loop_back_dsm_tx_wmk_low_cb;
  ds_mux_loop_tx_wmk[client_id].hiwater_func_ptr = ds_mux_loop_back_dsm_tx_wmk_hi_cb;
  
  /*-----------------------------------------------------------------------
   Setup Tx low watermark call function
     -----------------------------------------------------------------------*/
  dsm_set_wm_cb
  (
    &ds_mux_loop_tx_wmk[client_id],
    DSM_WM_LO,
    TRUE,
    ds_mux_loop_back_dsm_tx_wmk_low_cb,
    TRUE,
    (void*)client_id,
    FALSE,
    0  
  );
  /*-----------------------------------------------------------------------
   Setup Tx low watermark call function
     -----------------------------------------------------------------------*/
  dsm_set_wm_cb
  (
    &ds_mux_loop_tx_wmk[client_id],
    DSM_WM_HI,
    TRUE,
    ds_mux_loop_back_dsm_tx_wmk_hi_cb,
    TRUE,
    (void*)client_id,
    FALSE,
    0  
  );

  /* Step 2 */
  dsm_queue_init ( &ds_mux_loop_rx_wmk[client_id],
                   0x7FFFFFFF,
                   &ds_mux_loop_rx_q[client_id] );
  
  dsm_set_low_wm( &ds_mux_loop_rx_wmk[client_id], DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_LO );
  dsm_set_hi_wm( &ds_mux_loop_rx_wmk[client_id], DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_HI );
  dsm_set_dne( &ds_mux_loop_rx_wmk[client_id], DS_MUX_LOOP_BACK_DSM_WM_DEFAULT_DNE );
  
  /*-----------------------------------------------------------------------
   Setup Rx non empty watermark call function
     -----------------------------------------------------------------------*/
  dsm_set_wm_cb
  (
    &ds_mux_loop_rx_wmk[client_id],
    DSM_WM_NON_EMPTY,
    TRUE,
    ds_mux_loop_back_rx_wmk_non_empty_func,
    TRUE,
    (void*)client_id,
    FALSE,
    0  
  );
  /*-----------------------------------------------------------------------
   Setup Rx low watermark call function
     -----------------------------------------------------------------------*/
  dsm_set_wm_cb
  (
    &ds_mux_loop_rx_wmk[client_id],
    DSM_WM_LO,
    TRUE,
    ds_mux_loop_back_dsm_rx_wmk_low_cb,
    TRUE,
    (void*)client_id,
    FALSE,
    0  
  );
  /*-----------------------------------------------------------------------
   Setup Rx low watermark call function
     -----------------------------------------------------------------------*/
  dsm_set_wm_cb
  (
    &ds_mux_loop_rx_wmk[client_id],
    DSM_WM_HI,
    TRUE,
    ds_mux_loop_back_dsm_rx_wmk_hi_cb,
    TRUE,
    (void*)client_id,
    FALSE,
    0  
  );
  /*-----------------------------------------------------------------------
   Setup RDM Paramters
     -----------------------------------------------------------------------*/
  rdm_params.device_support   = RDM_SINGLE_DEV_SUPPORT;
  rdm_params.open_multi_func  = NULL;
  rdm_params.close_multi_func = NULL;
  rdm_params.data             = NULL;

  if ( 0 == client_id )
  {
      /*-----------------------------------------------------------------------
       Register for first service
         -----------------------------------------------------------------------*/
    rdm_params.open_func        = ds_mux_loop_back_client1_rdm_open_cb;
    rdm_params.close_func       = ds_mux_loop_back_client1_rdm_close_cb;
    rdm_register_service ( DS_MUX_LOOP_BACK_SERV_1, &rdm_params );
  }
  else
  {
      /*-----------------------------------------------------------------------
       Register for second service
         -----------------------------------------------------------------------*/
    rdm_params.open_func        = ds_mux_loop_back_client2_rdm_open_cb;
    rdm_params.close_func       = ds_mux_loop_back_client2_rdm_close_cb;
    rdm_register_service ( DS_MUX_LOOP_BACK_SERV_2, &rdm_params );
  }
   
  }
  /*-------------------------------------------------------------------------
   Register with PS DIAG LOGGING
  ------------------------------------------------------------------------*/
  ps_logging_diag_register_cmd_cb(PS_LOGGING_DIAG_DS_MUX, ds_mux_diag_command_notify_cb);
}/* ds_mux_loop_back_init */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_deinit

===========================================================================*/
/*!
  @brief Deinit function

  @return None
*/
/*=========================================================================*/
void   ds_mux_loop_back_deinit
(
  uint32 client_id
)
{
  dsm_item_type      *item_ptr   = NULL;
  dsm_watermark_type *rx_wmk_ptr = NULL;
  dsm_watermark_type *tx_wmk_ptr = NULL;
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  /*-----------------------------------------------------------------------
   Flush out all the Rx packets
  -----------------------------------------------------------------------*/
  rx_wmk_ptr = &ds_mux_loop_rx_wmk[client_id];
  
  if (( NULL != rx_wmk_ptr) && ( NULL != (item_ptr = dsm_dequeue( rx_wmk_ptr ))))
  {
    dsm_free_packet(&item_ptr);
  }
    
  /*-----------------------------------------------------------------------
   Flush out all the tx packets
  -----------------------------------------------------------------------*/
  tx_wmk_ptr = &ds_mux_loop_tx_wmk[client_id];
  
  if (( NULL != tx_wmk_ptr) && ( NULL != (item_ptr = dsm_dequeue( tx_wmk_ptr ))))
  {
    dsm_free_packet(&item_ptr);
  }

  /*-----------------------------------------------------------------------
    Destroy queue assosiated with Rx/Tx WMs so that any DSM item stuck in queue will be freed
  -----------------------------------------------------------------------*/ 
  dsm_queue_destroy( rx_wmk_ptr );
  dsm_queue_destroy( tx_wmk_ptr );
  
  /*-----------------------------------------------------------------------
    Free timer
  -----------------------------------------------------------------------*/ 
  if ( DS_MUX_TIMER_INVALID_HANDLE != ds_mux_loop_back_config[client_id].timer_handle)
  {
    (void)ds_mux_timer_free( ds_mux_loop_back_config[client_id].timer_handle );
  }

  /*-----------------------------------------------------------------------
    Initialize configuration
  -----------------------------------------------------------------------*/ 
  ds_mux_loop_back_init_config(client_id);

} /* ds_mux_loop_back_deinit */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_sio_ioctl_all

===========================================================================*/
/*!
  @brief Call sio_ioctl functions with following commands:
         SIO_IOCTL_ENABLE_DTR_EVENT_EXT
         SIO_IOCTL_DTE_READY_ASSERTED
         SIO_IOCTL_INBOUND_FLOW_ENABLE
         SIO_IOCTL_DSR_ASSERT
         SIO_IOCTL_CD_ASSERT
         SIO_IOCTL_RI_ASSERT     

  @return None
*/
/*=========================================================================*/
 void ds_mux_loop_back_register_sio_ioctl
(
  uint32 stream_id, /* Msgr SIO stream ID */
  uint8  client_id
)
{
  /* Initialize SIO ioctl parameters with zeros */
  sio_ioctl_param_type ioctl_param = {0};

  if( DS_MUX_TEST_APP_CLIENT_1 == client_id )
  {
    /* Set the DTE Ready event call back */
    ioctl_param.enable_dte_ready_event = ds_mux_loop_back_sio_dtr_changed_cb_1;
  }
  else
  {
    /* Set the DTE Ready event call back */
    ioctl_param.enable_dte_ready_event = ds_mux_loop_back_sio_dtr_changed_cb_2;
       
  }
 /* Call sio_ioctl function with command SIO_IOCTL_ENABLE_DTR_EVENT_EXT */
  sio_ioctl( stream_id,
             SIO_IOCTL_ENABLE_DTR_EVENT_EXT,
             &ioctl_param );
  
  /* Set DTE ready asserted parameter */
  ioctl_param.dte_ready_asserted = &ds_mux_loop_back_config[client_id].dte_ready_asserted;

  /* Call SIO IOCTL using DTE ready asserted command */
  sio_ioctl( stream_id, SIO_IOCTL_DTE_READY_ASSERTED,
             &ioctl_param );

  /* Call sio_ioctl function with command SIO_IOCTL_INBOUND_FLOW_ENABLE */
  sio_ioctl( stream_id,
             SIO_IOCTL_INBOUND_FLOW_ENABLE,
             &ioctl_param );
  
  /* Call sio_ioctl function with command SIO_IOCTL_DSR_ASSERT */ 
  sio_ioctl( stream_id,
             SIO_IOCTL_DSR_ASSERT,
             &ioctl_param );

  /* Call sio_ioctl function with command SIO_IOCTL_CD_ASSERT */ 
  sio_ioctl( stream_id,
             SIO_IOCTL_CD_ASSERT,
             &ioctl_param );

  /* Call sio_ioctl function with command SIO_IOCTL_RI_ASSERT */ 
  sio_ioctl( stream_id,
             SIO_IOCTL_RI_ASSERT,
             &ioctl_param );
 } /* ds_mux_loop_back_register_sio_ioctl() */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_sio_open

===========================================================================*/
/*!
  @brief Initilizes  interface with SIO

  @return None
*/
/*=========================================================================*/
 void ds_mux_loop_back_sio_open
(
  uint8            client_id,
  sio_port_id_type port_id
)
{
  /* Initialize SIO open parameters with zeros */
  sio_open_type    open_param = {0};

  /* Initialize sio open param values */
  open_param.rx_queue    = &ds_mux_loop_rx_wmk[client_id];
  open_param.tx_queue    = &ds_mux_loop_tx_wmk[client_id];
  open_param.rx_func_ptr = NULL;
  open_param.stream_mode = SIO_GENERIC_MODE;
  open_param.stream_type = SIO_STREAM_RXTX;
  open_param.port_id     = port_id;
  open_param.rx_bitrate  = SIO_BITRATE_9600;
  open_param.tx_bitrate  = SIO_BITRATE_9600;

  /* Obtain the stream ID using sio open */
  ds_mux_loop_back_config[client_id].stream_id= sio_open(&open_param);
  
  /* Enable SIO ioctl using stream ID */
  ds_mux_loop_back_register_sio_ioctl( ds_mux_loop_back_config[client_id].stream_id, client_id);

  DS_MUX_LOG_MSG_INFO1_3( "ds_mux_loop_back_sio_open stream id %d dtr status %d client_id %d",
    ds_mux_loop_back_config[client_id].stream_id, 
    ds_mux_loop_back_config[client_id].dte_ready_asserted,
    DS_MUX_STUB_GET_CLIENT_ID_FROM_INDEX(client_id));

  if ( 0 == client_id )
  {
    rdm_notify( DS_MUX_LOOP_BACK_SERV_1, RDM_DONE_S );
  }
  else
  {
    rdm_notify( DS_MUX_LOOP_BACK_SERV_2, RDM_DONE_S );
  }
} /* ds_mux_loop_back_sio_open() */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_rdm_open_cb

===========================================================================*/
/*!
  @brief RDM open call back function

  @return None
*/
/*=========================================================================*/
 void ds_mux_loop_back_client2_rdm_open_cb
(
  sio_port_id_type port_id  /* SIO port ID */
)
{

  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_client2_rdm_open_cb rdm_oprt %d ", port_id);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].port_id= port_id;
  
  /* An existing port ID is expected from RDM callback */
  ASSERT(port_id != SIO_PORT_NULL);

  ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].timer_handle = ds_mux_timer_alloc( ds_mux_loop_back_timer_cb,
                                                             (void *)DS_MUX_TEST_APP_CLIENT_2 );

  ds_mux_loop_back_sio_open( DS_MUX_TEST_APP_CLIENT_2, port_id );  
} /* ds_mux_loop_back_client2_rdm_open_cb() */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_client1_rdm_open_cb

===========================================================================*/
/*!
  @brief RDM open call back function

  @return None
*/
/*=========================================================================*/
 void ds_mux_loop_back_client1_rdm_open_cb
(
  sio_port_id_type port_id  /* SIO port ID */
)
{

  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_client1_rdm_open_cb rdm_oprt %d ", port_id);
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].port_id= port_id;
  
  /* An existing port ID is expected from RDM callback */
  ASSERT(port_id != SIO_PORT_NULL);

  ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].timer_handle = ds_mux_timer_alloc( ds_mux_loop_back_timer_cb,
                                                             (void *)DS_MUX_TEST_APP_CLIENT_1 );

  ds_mux_loop_back_sio_open( DS_MUX_TEST_APP_CLIENT_1, port_id );  
} /* ds_mux_loop_back_client1_rdm_open_cb() */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_client1_sio_close_cb

===========================================================================*/
/*!
  @brief sio RDM close call back function for client 1

  @return None
*/
/*=========================================================================*/
static void ds_mux_loop_back_client1_sio_close_cb
(
  void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_MUX_LOG_MSG_INFO2_0( "ds_mux_loop_back_client1_sio_close_cb()");

  ds_mux_loop_back_deinit(DS_MUX_TEST_APP_CLIENT_1);
} /* ds_mux_loop_back_client1_sio_close_cb */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_client2_sio_close_cb

===========================================================================*/
/*!
  @brief sio RDM close call back function for client 2

  @return None
*/
/*=========================================================================*/
static void ds_mux_loop_back_client2_sio_close_cb
(
  void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_MUX_LOG_MSG_INFO2_3( "ds_mux_loop_back_client2_sio_close_cb(before) type %d "
                          "current SSR count %d total SSR %d", 
    ds_mux_loop_back_config[0].test_type,
    ds_mux_loop_back_config[0].curr_ssr_itr,
    ds_mux_loop_back_config[0].num_ssr_itr);

  ds_mux_loop_back_deinit(DS_MUX_TEST_APP_CLIENT_2);

  if (DS_MUX_LOOP_BACK_SSR_CTL_TEST == ds_mux_loop_back_config[0].test_type &&
      ds_mux_loop_back_config[0].curr_ssr_itr < ds_mux_loop_back_config[0].num_ssr_itr )
  {
    ds_mux_power_up_cb();
    ds_mux_loop_back_config[0].curr_ssr_itr++;
    
  }

  DS_MUX_LOG_MSG_INFO2_3( "ds_mux_loop_back_client2_sio_close_cb(After) type %d "
                          "current SSR count %d total SSR %d", 
    ds_mux_loop_back_config[0].test_type,
    ds_mux_loop_back_config[0].curr_ssr_itr,
    ds_mux_loop_back_config[0].num_ssr_itr);


  
} /* ds_mux_loop_back_client2_sio_close_cb */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_client1_rdm_close_cb

===========================================================================*/
/*!
  @brief RDM close call back function for client 1

  @return None
*/
/*=========================================================================*/
void ds_mux_loop_back_client1_rdm_close_cb
(
  void
)
{
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_client1_rdm_close_cb rdm_port %d ",
                          ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].port_id);

  sio_close(ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_1].stream_id,
            ds_mux_loop_back_client1_sio_close_cb );

  rdm_notify( DS_MUX_LOOP_BACK_SERV_1, RDM_DONE_S );
} /* ds_mux_loop_back_client1_rdm_close_cb() */

/*===========================================================================

FUNCTION:  ds_mux_loop_back_client2_rdm_close_cb

===========================================================================*/
/*!
  @brief RDM close call back function for client 2

  @return None
*/
/*=========================================================================*/
void ds_mux_loop_back_client2_rdm_close_cb
(
  void
)
{
  DS_MUX_LOG_MSG_INFO1_1( "ds_mux_loop_back_client1_rdm_close_cb rdm_port %d ",
                          ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].port_id);

  sio_close(ds_mux_loop_back_config[DS_MUX_TEST_APP_CLIENT_2].stream_id,
            ds_mux_loop_back_client2_sio_close_cb );

   rdm_notify( DS_MUX_LOOP_BACK_SERV_2, RDM_DONE_S );
} /* ds_mux_loop_back_client2_rdm_close_cb() */

#endif  /* #define FEATURE_DATA_MUX_TEST_LOOP_BACK */
