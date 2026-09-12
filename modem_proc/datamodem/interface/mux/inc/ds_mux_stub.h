#ifndef DS_MUX_STUB_H
#define DS_MUX_STUB_H
/*===========================================================================

                        D S _ M U X _ S T U B . H

DESCRIPTION

  This is the internal header file for the MUX Task. This file
  contains all the functions, STUB definitions and data types needed for MUX 
  task to interface to SIO for physical port.
  
INITIALIZATION AND SEQUENCING REQUIREMENTS


Copyright (c) 2016 Qualcomm Technologies Incorporated.
All Rights Reserved.
Qualcomm Confidential and Proprietary.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //components/rel/data.mpss/3.5..2.1/interface/mux/inc/ds_mux_stub.h#2 $ 
  $DateTime: 2019/10/22 02:53:17 $ $Author: pwbldsvc $

===========================================================================*/

/*===========================================================================

                      INCLUDE FILES FOR MODULE

===========================================================================*/

#include "datamodem_variation.h"
#include "comdef.h"
#include "customer.h"       /* Customer Specific Features */
#include "rex.h"
#include "sio.h"
#include "rdevmap.h"
#include "ds_mux_defs.h"
#include "ds_mux.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef TEST_FRAMEWORK
#error code not present
#endif /*TEST_FRAMEWORK */

#ifdef __cplusplus
}
#endif

#define  DS_MUX_TEST_INTEGRITY_TEST1_VALUE   0xA1
#define  DS_MUX_TEST_INTEGRITY_TEST2_VALUE   0xDEADBEEF
#define  DS_MUX_TEST_INTEGRITY_TEST3_VALUE   "abc"
#define  DS_MUX_TEST_INTEGRITY_TEST4_VALUE   0xABCD
#define  DS_MUX_TEST_INTEGRITY_TEST5_VALUE   0xBD
#define  DS_MUX_TEST_INTEGRITY_TEST6_VALUE   "hellotest"
#define  DS_MUX_TEST_INTEGRITY_TEST7_VALUE   0xAE
#define  DS_MUX_TEST_INTEGRITY_TEST8_VALUE   0xDCBA
#define  DS_MUX_TEST_INTEGRITY_TEST9_VALUE   0xDEBF
#define  DS_MUX_TEST_INTEGRITY_TEST10_VALUE  "test"
#define DS_MUX_LOOP_BACK_SSR_POWER_DOWN      0x0
#define DS_MUX_LOOP_BACK_SSR_POWER_UP        0x1
#define DS_MUX_LOOP_BACK_SSR_POWER_UP_DOWN   0x2

typedef enum
{
  DS_MUX_LOOP_BACK_RESET_TEST         = 0,
  /* @brief  Cancel current job and reset status */
  DS_MUX_LOOP_BACK_BASIC_TEST         = 1,
  /* @brief  Send and receive some packets, Verify number of packets*/
  DS_MUX_LOOP_BACK_POWER_SAVE_TEST    = 2,
  /* @brief  MUX should go into power save before sending
       next packet, Once Packet arrive MUX should backup.
       Match the sequence number to figure out drop*/
  DS_MUX_LOOP_BACK_REGRESSION_TEST    = 3,
  /* @brief  Send simple packets for few hours*/
  DS_MUX_LOOP_BACK_BIG_DATA_TEST      = 4,
  /* @brief  Send 5K, 10K, 20K data, Verify size once it is recevied*/
  DS_MUX_LOOP_BACK_INTEGRITY_TEST     = 5,
  /* @brief  Send some stuct type and verify internal fields*/
  DS_MUX_LOOP_BACK_LATENCY_TEST       = 6,
  /* @brief  Send some stuct type and measure latency*/
  DS_MUX_LOOP_BACK_FLOW_CTL_TEST      = 7,
  /* @brief  Flow control test */
  DS_MUX_LOOP_BACK_SSR_CTL_TEST       = 8,
  /* @brief  Flow control test */
  DS_MUX_LOOP_BACK_MAX_TEST           = 9
  /* @brief  Send some stuct type and verify internal fields*/
  
} ds_mux_loop_back_test_type;

typedef struct
{
  ds_mux_loop_back_test_type  test_type;
  /*! @brief  Number of packets sent till now*/
  uint32                      curr_sent_counter;
  /*! @brief  Number of packets sent till now*/
  uint32                      expected_recv_num;
  /*! @brief  Timer difference between two packet send*/
  uint32                      max_num_packet;
   /*! @brief  Maximum iteration */
  uint32                      num_packet_drop;
    /*! @brief  Num packet drops */
  uint64                      time_interval;
  /*! @brief  Test case type*/
  boolean                     dte_ready_asserted ;
  /* @brief DTR status */
  sio_port_id_type            port_id;
  /* SIO port ID */
  sio_stream_id_type          stream_id;
  /* Stream ID */
  ds_mux_timer_handle_type    timer_handle;
  /* @brief Timer handle to retransmit the data */
  uint16                      min_num_bytes;
  /*! @brief  number of bytes of packet when sending starts */
  uint16                      increment_num_bytes;
  /*! @brief  number of bytes of packet to be incremented for each sending */
  uint64                      latency_test_start_time;
  /*! @brief  start time stamp */
  boolean                     flow_enabled;
  /*! @brief  check if flow is enabled */
  boolean                     is_latency_pkt_returned;
  /*! @brief  check if packet is returned from the other side */
  boolean                     is_latency_timer_expired;
  /*! @brief  check if timer for latency was triggered */

  uint16                      curr_ssr_itr;
  uint16                      num_ssr_itr;
  /*! @brief  number ofSSR iteration */

  
}ds_mux_loop_back_test_config_info_type;

typedef struct
{
  uint8  test_val1;
  uint32 test_val2;
  uint8  test_val3[4];
  uint16 test_val4;

  struct {
    uint16 test_val5;
    uint8  test_val6[10];
  } test_struct1;

  struct {
    uint16 test_val7;
    uint32 test_val8;
  } test_struct2;
  
  uint32  test_val9;
  uint8   test_val10[5];
}ds_mux_stub_integrity_test_info;
/*! @brief  Structure for integrity test*/

void ds_mux_loop_back_client1_rdm_open_cb
(
  sio_port_id_type port_id  /* SIO port ID */
);

 void ds_mux_loop_back_client2_rdm_open_cb
(
  sio_port_id_type port_id  /* SIO port ID */
);

void ds_mux_loop_back_client1_rdm_close_cb
(
  void
);

void ds_mux_loop_back_client2_rdm_close_cb
(
  void
);

void ds_mux_loop_back_rdm_open_cb
(
  sio_port_id_type port_id  /* SIO port ID */
);

void ds_mux_loop_back_sio_open
(
  uint8            client_id,
  sio_port_id_type port_id
);

void ds_mux_loop_back_big_data_recv_verify
(
  dsm_item_type       * item_ptr,
  void                * user_data_ptr,
  uint32                pkt_len
);

void  ds_mux_loop_back_big_data_send
(
  uint8    client_id
);

void  ds_mux_basic_loop_back_send
(
  uint8    client_id
);

void  ds_mux_latency_loop_back_send
(
  uint8    client_id
);

void  ds_mux_integrity_loop_back_send
(
  uint8    client_id
);

void ds_mux_loop_back_timer_handler
(
  uint8    client_id
);

void   ds_mux_loop_back_init
(
  void
);


#endif /* STUB */
