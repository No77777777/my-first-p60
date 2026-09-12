/*!
  @file IPA_test.cpp

  @brief
    IPA Test utilities
*/                               
/*==============================================================================

  Copyright (c) 2013 QUALCOMM Technologies Incorporated. All Rights Reserved

  Qualcomm Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  QUALCOMM Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of QUALCOMM Technologies Incorporated.

==============================================================================*/

/*==============================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/dataipa.mpss/3.2/ipa/ipa_stride/IPA_test.cpp#2 $
$Date: 2020/12/17 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
12/17/20   mk      CR: 2840535 Fix for IPA stride compilation
07/20/14   SaCh    Adding Dest_Opts IPv6 Extn hdr.
07/13/14   SaCh    Fixes in Dl/UL/Loopback CLAT areas.
06/20/14   mi      Added IPV6 fragmentation support.
06/16/14   SaCh    Accepting TCP ACK Pkt Type in tcp protocol params contained 
                   in ip_pkt_cfg which is passed from testcase.
02/17/15   mi      CR#739684: IPA ZIP feature related test changes.
11/21/14   am      QMAPv3 insert offset def change.
07/15/15   mi      IPA 2.1 regression test fixes.
07/23/14   rp      Accelerated DPL changes 
06/23/14   pgm     Changes to support new trailer padding bytes addition.
05/25/14   rp      Test engine fixes/cleanup for issues seen on full stack build
04/25/14   rp      Changes to support ON-target testing on full stack CRM
04/16/13   pgm     IPA2.1 aggr/deaggr test changes.
04/03/14   rp      Support to test QMAP EOF programming to close aggr frame
03/06/14   mi      Added Fragmentation test changes.
02/20/14   mi      Bug Fix: Aggregation params wrongly accessed. 
12/17/13   pgm     BAM2BAM pipe config support.
12/11/13   mi      Added support for API changes in sio mapping API.
12/05/13   mi      Added support for DPL.
11/18/13   pgm     New TCP ack pkt generation changes.
11/12/13   rp      Modifying the Aggregation params for USB/HSIC
10/24/13   rp      Heap memory leak fix
10/15/13   rp      DSM Chain fix & seed user cfg
09/30/13   pgm     Support for testing UL priority packets.
09/19/13   rp      Move from QMAPv2 to QMAPv3
08/22/13   rp      Initial Checkin
==============================================================================*/
/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include "IPA_test_engine.h"
#include "IPA_test.h"

#pragma optimize("", off)
/*==============================================================================

                         MACROS

==============================================================================*/
/*==============================================================================

                   INTERNAL DEFINITIONS AND TYPES

==============================================================================*/
typedef struct
{
  uint8                       random_data_buffer \
    [IPA_TEST_RANDOM_DATA_BUFFER_SIZE];
  uint8                       is_random_gen;
  ipa_test_ip_pkt_frag_ctxt_s frag_ctxt;
  uint32                      ip_id_cnt;
} ipa_test_ip_pkt_s;

typedef struct
{
  uint16 num_dsm_items;
  uint16 size_per_dsm_item;
  uint16 curr_dsm_item_num;
} ipa_test_dsm_chain_s;

typedef enum
{
  IPA_TEST_ERROR_TYPE_DEAGG_QMAP_V3_CKSUM,
  IPA_TEST_ERROR_TYPE_DEAGG_ETH_HDR,
} ipa_test_error_type_e;

typedef struct
{
  ipa_hw_qmap_hdr_s       qmap_hdr;
  ipa_hw_cksum_hdr_s      cksum_hdr;
  ipa_hw_rndis_hdr_s      rndis_hdr;
  ipa_hw_log_status_s     log_status;
  ipa_test_nth_hdr_s      nth_hdr;
  ipa_test_ndp_hdr_s      ndp_hdr;
  ipa_test_ndp_hdr_s      ndp_hdr_arr[IPA_TEST_ENGINE_NDP_NUM_MAX];
  ipa_test_ndp_hdr_pkt_s  pkt_hdr[IPA_TEST_ENGINE_IP_PKT_NUM_MAX];
  dsm_item_type           *mbim_ndp_frame;
  uint16                  curr_mbim_seq_num; 
  uint16                  curr_ndp_hdr_len;
  uint32                  curr_mbim_pkt_byte_cnt;
  uint32                  curr_mbim_frame_byte_cnt;
  uint8                   curr_mbim_ndp_pkts;
  ipa_test_error_type_e   err_type;
  ipa_hw_cksum_trler_s    cksum_trler;
  uint32                  num_qmap_eofs; 
  uint32                  num_log_pkt_eofs; 
  uint16                  no_match_locn;
  uint8                   rx_eth_hdr[IPA_SIO_ETHERNET_HDR_LEN] \
    __attribute__((__aligned__(4)));
  uint8                   tx_eth_hdr[IPA_SIO_ETHERNET_HDR_LEN] \
    __attribute__((__aligned__(4)));
} ipa_test_agg_s;

typedef struct
{ 
  ipa_test_agg_s        agg;
  ipa_test_ip_pkt_s     ip_pkt;
  ipa_test_dsm_chain_s  dsm_chain;
} ipa_test_s;

/*=========================================================================

                         INTERNAL VARIABLES

=========================================================================*/
ipa_test_s ipa_test;
uint16 IPA_TEST_MBIM_SEQ = 0;
/* DAL Timetick Global handle */
DalDeviceHandle* ipa_test_timetick_handle;

/*==============================================================================

                                FUNCTIONS

==============================================================================*/
/*=========================================================================
                         Generic functions
==========================================================================*/
/*==============================================================================

 FUNCTION: ipa_test_timetick_init

==============================================================================*/
/*!
  @brief
  Inits the timetick for tests

  @return
  None
*/
/*============================================================================*/
void ipa_test_timetick_init(void)
{
  DALResult dal_result = DAL_ERROR;

  /* Get timetick handle */
  if (ipa_test_timetick_handle == NULL)
  {
    dal_result = DalTimetick_Attach("SystemTimer", &ipa_test_timetick_handle);
    IPA_ASSERT((dal_result == DAL_SUCCESS) && (ipa_test_timetick_handle != NULL));
  }
}

/*==============================================================================

 FUNCTION: ipa_test_timetick_get

==============================================================================*/
/*!
  @brief
  Gets the timetick for tests

  @return
  None
*/
/*============================================================================*/
ipa_timetick_t ipa_test_timetick_get(void)
{
  uint64 time_ret;

  (void)DalTimetick_GetTimetick64(ipa_test_timetick_handle, &time_ret);

  return (ipa_timetick_t)time_ret;
}

/*==============================================================================

  FUNCTION: ipa_test_generate_random_num

==============================================================================*/
/*!
  @brief
  Generates a random number between & including min & max 

  @return
  None
*/
/*============================================================================*/
uint32 ipa_test_generate_random_num
(
  uint32 max_number,
  uint32 min_number
)
{
  uint32 num = 0;
 
  if (max_number <= min_number)
    return max_number;

  num = ((uint32)rand() % (max_number - min_number + 1)) + min_number;

  return num;
}

/*==============================================================================

  FUNCTION: ipa_test_generate_unique_random_num

==============================================================================*/
/*!
  @brief
  Generates a random number between & including min & max 
  Generates random number which does not already exist in the array provided

  @return
  None
*/
/*============================================================================*/
uint8 ipa_test_generate_unique_random_num
(
  uint8 max_number,
  uint8 min_number,
  uint8 *exist_num_arr_ptr,
  uint8 exist_num_arr_size
)
{
  uint8 i = 0;
  uint8 duplicate = TRUE;
  uint8 num = 0;

  IPA_TF_ASSERT(exist_num_arr_ptr != NULL);

  do
  {
    num = (uint8)ipa_test_generate_random_num(max_number, min_number);
    duplicate = FALSE;

    /* We need unique id & uid should not be equal to 0 */
    for (i = 0; i < exist_num_arr_size; i++) 
    {
      if (num == exist_num_arr_ptr[i]) 
      {
        duplicate = TRUE;
        break;
      }
    }
  } while (duplicate == TRUE);

  return num;
}

/*==============================================================================

  FUNCTION: ipa_test_generate_random_data

==============================================================================*/
/*!
  @brief
  Generates random data into a buffer 

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_random_data
(
  uint8 *rand_data,
  uint32 rand_data_size
)
{
  uint32 i = 0;
  int32 data = 0;
  uint8 *data_ptr = NULL;
  IPA_TF_ASSERT(rand_data != NULL);

  data = rand();
  data_ptr = (uint8 *)&data;

  for (i = 0; i < rand_data_size; i++)
  {
    rand_data[i] = data_ptr[i % sizeof(int32)] + (uint8)rand();
  }
}

/*==============================================================================

  FUNCTION: ipa_test_generate_sequence_data

==============================================================================*/
/*!
  @brief
  Generate sequence data (1-256) in the given memory location for 
  specifed size

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_sequence_data
(
  uint8 *seq_data,
  uint32 data_size
)
{
  uint32 i = 0;
  IPA_TF_ASSERT(seq_data != NULL);

  for (i = 0; i < data_size; i++)
  {
    seq_data[i] = i % 256;
  }
}

/*==============================================================================

  FUNCTION: ipa_test_get_index_from_bitmask

==============================================================================*/
/*!
  @brief
  Get the index for SIO/SIM/bearer from the bit mask

  @return
  None
*/
/*============================================================================*/
uint8 ipa_test_get_index_from_bitmask
(
  uint64 bitmask
)
{
  uint32 mask;
  uint8 index = 0;
  uint32 *mask_ptr = NULL;

  IPA_TF_ASSERT(bitmask != 0);
  
  mask_ptr = ((uint32 *)&bitmask);
  for (mask = 0x01; index < 32; mask <<= 1)
  {
    if (mask & (*mask_ptr)) 
    {
      return index;
    }
    index++;
  }

  mask_ptr = ((uint32 *)&bitmask + 1);
  for (mask = 0x01; index < 64; mask <<= 1)
  {
    if (mask & (*mask_ptr)) 
    {
      return index;
    }
    index++;
  }

  return index;
}

/*=========================================================================
                         Endp Cfg functions
==========================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_set_endp_map

==============================================================================*/
/*!
  @brief
  Sets the Endpoint mapping with the driver

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_set_endp_map
(
  uint8                    phy_ch_idx, 
  ipa_test_engine_phy_ch_s *phy_ch_ptr,
  uint8                    hw_replication
)
{
  ipa_err_code_e err;
  ipa_hw_q6_pipe_id_e cons_pipe_id, prod_pipe_id;
  uint8 ic_type;

  IPA_TF_ASSERT(phy_ch_ptr != NULL);

  switch (phy_ch_idx)
  {
    case 0:
      cons_pipe_id = IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_0_ID;
      prod_pipe_id = IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_0_ID;
      break;

    case 1:
	if (hw_replication == FALSE)
	{
      cons_pipe_id = IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_1_ID;
      prod_pipe_id = IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_1_ID;
	}
	else
	{
	  cons_pipe_id = IPA_HW_Q6_PIPE_ID_MAX;
      prod_pipe_id = IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_1_ID;
	}
	
      break;
#if 0
    case 2:
      cons_pipe_id = IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_2_ID;
      prod_pipe_id = IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_2_ID;
      break;

    case 3:
      cons_pipe_id = IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_3_ID;
      prod_pipe_id = IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_3_ID;
      break;
#endif

    default:
      IPA_TF_ASSERT(0);
      break;
  }

#ifdef FEATURE_IPA_USE_IPA_BAM_DRIVER
  /*If current config is for BAM2BAM mode, consumer pipe id will be B2B Consumer pipe id into IPA*/
  if (TRUE == phy_ch_ptr->per_sim.b2b_mode)
  {
    cons_pipe_id = IPA_HW_Q6_SIM_UL_B2B_CONSUMER_PIPE_0_ID; IPA_HW_Q6_SIM_UL_B2B_CONSUMER_PIPE_0_ID
  }
#endif

  phy_ch_ptr->per_ep_map.per_cons_pipe_id = cons_pipe_id;
  phy_ch_ptr->per_ep_map.per_prod_pipe_id = prod_pipe_id;
  phy_ch_ptr->per_ep_map.phy_rx_wm_ptr = &phy_ch_ptr->phy_rx_wm;
  phy_ch_ptr->per_ep_map.phy_tx_wm_ptr = &phy_ch_ptr->phy_tx_wm;
  //ic_type = IPA_SIO_PER_EP_IC_TYPE_RESERVED;
  //phy_ch_ptr->per_ep_map.per_ep_id = (ic_type << 24) | phy_ch_idx;
  phy_ch_ptr->per_ep_map.per_ep_id = (phy_ch_ptr->ic_type << 24) | phy_ch_idx;

  /* Set the mapping of physical SIO stream to peripheral endpoint id
     and peripheral pipe pair */
  err = ipa_sioi_set_per_ep_mapping(phy_ch_ptr->stream_id,
                                    &phy_ch_ptr->per_ep_map);
  if (err == IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
    IPA_TF_ASSERT_LOG(0, "IPA_test: SIO mapping add failed with error %d", err, 0, 0);
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_register_per_sim

==============================================================================*/
/*!
  @brief
  Registers a per sim with the driver and initializes the watermarks associated 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_register_per_sim
(
  uint8                    phy_ch_idx, 
  ipa_test_engine_phy_ch_s *phy_ch_ptr,
  uint8                    tx_delay  
)
{
  ipa_err_code_e err;

  IPA_TF_ASSERT(phy_ch_ptr != NULL);

  /* Initiailize the RX/TX Watermarks */
  IPA_TEST_WM_INIT(&phy_ch_ptr->per_sim.from_ipa_wm_q, 
                   &phy_ch_ptr->per_sim.from_ipa_wm);
  IPA_TEST_WM_INIT(&phy_ch_ptr->per_sim.to_ipa_wm_q, 
                   &phy_ch_ptr->per_sim.to_ipa_wm);

  /* Initialize the Deagg from Mirror Watermark */
  IPA_TEST_WM_INIT(&phy_ch_ptr->per_sim.deagg_from_ipa_wm_q, 
                   &phy_ch_ptr->per_sim.deagg_from_ipa_wm);

  /* Initialize the SIM From Mirror Watermark */
  IPA_TEST_WM_INIT(&phy_ch_ptr->per_sim.mirror_from_ipa_wm_q, 
                   &phy_ch_ptr->per_sim.mirror_from_ipa_wm);

  IPA_TEST_WM_INIT(&phy_ch_ptr->per_sim.mirror_from_ipa_wm_ul_q, 
                   &phy_ch_ptr->per_sim.mirror_from_ipa_wm_ul);

  IPA_TEST_WM_INIT(&phy_ch_ptr->per_sim.mirror_from_ipa_wm_dl_q, 
                   &phy_ch_ptr->per_sim.mirror_from_ipa_wm_dl);

  /* Now set the non-empty callback for data recvd from IPA */
  phy_ch_ptr->per_sim.from_ipa_wm.non_empty_func_data = 
    (void *)(uint32)phy_ch_idx;
  phy_ch_ptr->per_sim.from_ipa_wm.non_empty_func_ptr = 
    ipa_test_engine_sim_rx_wm_ne_cb;

  /* Now register the watermarks with the SIM */
  err = ipa_test_per_sim_reg_wmks_ex(&phy_ch_ptr->per_sim.to_ipa_wm, 
                                     &phy_ch_ptr->per_sim.from_ipa_wm,
                                     tx_delay,
                                     &phy_ch_ptr->per_sim.sim_hdl,
                                     phy_ch_ptr->per_sim.b2b_mode); 
  if (err == IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_deregister_per_sim

==============================================================================*/
/*!
  @brief
  DeRegisters a per sim with the driver and destroys the watermarks associated 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deregister_per_sim
(
  ipa_test_engine_phy_ch_s *phy_ch_ptr
)
{
  ipa_err_code_e err;

  IPA_TF_ASSERT(phy_ch_ptr != NULL);

  /* Initiailize the RX/TX Watermarks */
  IPA_TEST_WM_DESTROY(&phy_ch_ptr->per_sim.from_ipa_wm);
  IPA_TEST_WM_DESTROY(&phy_ch_ptr->per_sim.to_ipa_wm);

  /* Initialize the Deagg from Mirror Watermark */
  IPA_TEST_WM_DESTROY(&phy_ch_ptr->per_sim.deagg_from_ipa_wm);

  /* Initialize the SIM From Mirror Watermark */
  IPA_TEST_WM_DESTROY(&phy_ch_ptr->per_sim.mirror_from_ipa_wm);

  IPA_TEST_WM_DESTROY(&phy_ch_ptr->per_sim.mirror_from_ipa_wm_ul);
  
  IPA_TEST_WM_DESTROY(&phy_ch_ptr->per_sim.mirror_from_ipa_wm_dl);

  /* Now register the watermarks with the SIM */
  err = ipa_test_per_sim_dereg_wmks(&phy_ch_ptr->per_sim.to_ipa_wm, 
                                    &phy_ch_ptr->per_sim.from_ipa_wm,
                                    phy_ch_ptr->per_sim.sim_hdl); 
  if (err == IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_open_phy_sio_port

==============================================================================*/
/*!
  @brief
  Opens a physical SIO port and initilializes the watermarks associated.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_open_phy_sio_port 
(
  uint8                    phy_ch_idx, 
  ipa_test_engine_phy_ch_s *phy_ch_ptr
)
{
  sio_open_type         sio_param;
  sio_port_id_type      port_id;

  IPA_TF_ASSERT(phy_ch_ptr != NULL);

  port_id = ipa_test_sio_get_port_id(FALSE, phy_ch_idx);
  IPA_TF_ASSERT(port_id != 0);

  /* Initiailize the PHY RX/TX Watermarks */
  IPA_TEST_WM_INIT(&phy_ch_ptr->phy_rx_wm_q, &phy_ch_ptr->phy_rx_wm);
  IPA_TEST_WM_INIT(&phy_ch_ptr->phy_tx_wm_q, &phy_ch_ptr->phy_tx_wm);

  /* Initialize the PHY Rx Mirror Watermark */
  IPA_TEST_WM_INIT(&phy_ch_ptr->mirror_phy_rx_wm_q, 
                   &phy_ch_ptr->mirror_phy_rx_wm);
  
  /* Now set the each-enqueue callback for data recvd from IPA driver */
  phy_ch_ptr->phy_rx_wm.each_enqueue_func_data = (void *)(uint32)phy_ch_idx;
  phy_ch_ptr->phy_rx_wm.each_enqueue_func_ptr = 
     ipa_test_engine_phy_rx_wm_each_enqueue_cb;

  IPA_MEMSET(&sio_param, 0, sizeof(sio_param));
  sio_param.port_id = port_id; 
  sio_param.rx_queue = (dsm_watermark_type *)TRUE;
     
  phy_ch_ptr->stream_id = sio_open(&sio_param);

  if (phy_ch_ptr->stream_id != 0xFFFF)
  {
    return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_close_phy_sio_port

==============================================================================*/
/*!
  @brief
  Closes a physical SIO port and destroys the watermarks associated.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_close_phy_sio_port 
(
  ipa_test_engine_phy_ch_s *phy_ch_ptr
)
{
  IPA_TF_ASSERT(phy_ch_ptr != NULL);

  /* Destroy the PHY RX/TX Watermarks */
  IPA_TEST_WM_DESTROY(&phy_ch_ptr->phy_rx_wm);
  IPA_TEST_WM_DESTROY(&phy_ch_ptr->phy_tx_wm);

  /* Destroy the PHY Rx Mirror Watermark */
  IPA_TEST_WM_DESTROY(&phy_ch_ptr->mirror_phy_rx_wm);
  
  sio_close(phy_ch_ptr->stream_id, NULL);

  return IPA_TEST_RET_CODE_SUCCESS;
}


/*==============================================================================

  FUNCTION:  ipa_test_open_log_sio_port

==============================================================================*/
/*!
  @brief
  Opens a logical SIO port and initilializes the watermarks associated.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_open_log_sio_port 
(
  uint8                    log_ch_idx, 
  ipa_test_engine_log_ch_s *log_ch_ptr
)
{
  sio_open_type         sio_param;
  sio_port_id_type      port_id;

  IPA_TF_ASSERT(log_ch_ptr != NULL);

  port_id = ipa_test_sio_get_port_id(TRUE, log_ch_idx);
  IPA_TF_ASSERT(port_id != 0);

  /* Initiailize the RX/TX Watermarks */
  IPA_TEST_WM_INIT(&log_ch_ptr->rx_wm_q, &log_ch_ptr->rx_wm);
  IPA_TEST_WM_INIT(&log_ch_ptr->tx_wm_q, &log_ch_ptr->tx_wm);

  /* Initialize the Rx Mirror Watermark */
  IPA_TEST_WM_INIT(&log_ch_ptr->mirror_rx_wm_q, 
                   &log_ch_ptr->mirror_rx_wm);
  
  /* Now set the each-enqueue callback for data recvd from IPA driver */
  log_ch_ptr->rx_wm.each_enqueue_func_data = (void *)(uint32)log_ch_idx;
  log_ch_ptr->rx_wm.each_enqueue_func_ptr = 
     ipa_test_engine_sio_rx_wm_each_enqueue_cb;

  IPA_MEMSET(&sio_param, 0, sizeof(sio_param));
  sio_param.port_id = port_id; 
  sio_param.rx_queue = &log_ch_ptr->rx_wm;
  sio_param.tx_queue = &log_ch_ptr->tx_wm;
    
  log_ch_ptr->stream_id = sio_open(&sio_param);

  if (log_ch_ptr->stream_id != 0xFFFF)
  {
    return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_close_log_sio_port

==============================================================================*/
/*!
  @brief
  Closes a logical SIO port and destroys the watermarks associated.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_close_log_sio_port 
(
  ipa_test_engine_log_ch_s *log_ch_ptr
)
{
  IPA_TF_ASSERT(log_ch_ptr != NULL);

  /* Destroys the RX/TX Watermarks */
  IPA_TEST_WM_DESTROY(&log_ch_ptr->rx_wm);
  IPA_TEST_WM_DESTROY(&log_ch_ptr->tx_wm);

  /* Destroys the Rx Mirror Watermark */
  IPA_TEST_WM_DESTROY(&log_ch_ptr->mirror_rx_wm);

  sio_close(log_ch_ptr->stream_id, NULL);

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_sio_get_port_id

==============================================================================*/
/*!
  @brief
  Gets a new SIO port ID

  @return
  None
*/
/*============================================================================*/
sio_port_id_type ipa_test_sio_get_port_id
(
  uint8   is_logical,
  uint32  port_index 
)
{
  if(is_logical == TRUE)
  {
     IPA_TF_ASSERT(port_index < IPA_SIO_MAX_INSTANCES);
     switch ( port_index )
     {
       case 0:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_1;
         break;
       case 1:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_2;
         break;
       case 2:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_3;
         break;
       case 3:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_4;
         break;
       case 4:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_5;
         break;
       case 5:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_6;
         break;
       case 6:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_7;
         break;
       case 7:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_8;
         break;
       case 8:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_9;
         break;
       case 9:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_10;
         break;
       case 10:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_11;
         break;
       case 11:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_12;
         break;
       case 12:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_13;
         break;
       case 13:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_14;
         break;
       case 14:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_15;
         break;
       case 15:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_16;
         break;
       case 16:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_17;
         break;
       case 17:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_18;
         break;
       case 18:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_19;
         break;
       case 19:
         return (sio_port_id_type)SIO_PORT_IPA_DS_CH_20;
         break;
     }
  }
  else
  {
     IPA_TF_ASSERT(port_index < IPA_SIO_MAX_PHYSICAL_SIO_STREAMS);
     switch (port_index)
     {
       case 0:
         return (sio_port_id_type)SIO_PORT_IPA_DS_PHYS_1;
         break;
       case 1:
         return (sio_port_id_type)SIO_PORT_IPA_DS_PHYS_2;
         break;
       case 2:
         return (sio_port_id_type)SIO_PORT_IPA_DS_PHYS_3;
         break;
       case 3:
         return (sio_port_id_type)SIO_PORT_IPA_DS_PHYS_4;
         break;
     }
  }

  return (sio_port_id_type)0;
}

/*==============================================================================

  FUNCTION:  ipa_test_sio_get_log_ch_idx

==============================================================================*/
/*!
  @brief
  Find the logical channel index on a physical channel

  @return
  None
*/
/*============================================================================*/
ipa_test_engine_log_ch_s *ipa_test_sio_get_log_ch_idx
(
  ipa_test_engine_phy_ch_s *phy_ch_ptr,
  uint8                    mux_id 
)
{
  uint8 i = 0;
  for (i = 0; i < phy_ch_ptr->log_ch_cnt; i++)
  {
    if (phy_ch_ptr->log_ch_ptr_arr[i]->mux_id == mux_id)
    {
      return phy_ch_ptr->log_ch_ptr_arr[i];
    }
  }

  return NULL;
}

/*==============================================================================

  FUNCTION:  ipa_test_check_sio_aggr_prot_support

==============================================================================*/
/*!
  @brief
  Checks whether the aggregation protocols in current sio cfg is supoorted 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_check_sio_aggr_prot_support
(
  ipa_sio_endpoint_config_s *sio_cfg_ptr,
  uint8                     ic_type
)
{
  ipa_err_code_e err_code = IPA_SUCCESS;
  uint8 max_aggr_pkts;
  uint32 max_aggr_bytes,dl_pad_len;
  IPA_TF_ASSERT(sio_cfg_ptr != NULL);

  err_code = 
    ipa_sioi_get_dl_aggr_prot_support((uint32)(ic_type << 24),
                                      sio_cfg_ptr->dl_config.hdr_type, 
                                      &max_aggr_pkts,
                                      &max_aggr_bytes
                                      ,&dl_pad_len);
  if (err_code != IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }

  err_code = 
    ipa_sioi_get_ul_aggr_prot_support((uint32)(ic_type << 24),
                                      sio_cfg_ptr->ul_config.hdr_type, 
                                      &max_aggr_pkts,
                                      &max_aggr_bytes);
  if (err_code != IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }

  if ((sio_cfg_ptr->ul_config.hdr_type == IPA_SIO_HDR_TYPE_QMAP) || 
      (sio_cfg_ptr->ul_config.hdr_type == IPA_SIO_HDR_TYPE_QMAP_V3)) 
  {
     if ((sio_cfg_ptr->ul_config.params.qmap.max_aggr_pkts > max_aggr_pkts) ||
      (sio_cfg_ptr->ul_config.params.qmap.max_aggr_bytes > max_aggr_bytes))
     {
       return IPA_TEST_RET_CODE_FAILURE;
     }
  }
  else if (sio_cfg_ptr->ul_config.hdr_type == IPA_SIO_HDR_TYPE_MBIM)
  {
     if ((sio_cfg_ptr->ul_config.params.mbim.max_aggr_pkts > max_aggr_pkts) ||
      (sio_cfg_ptr->ul_config.params.mbim.max_aggr_bytes > max_aggr_bytes))
     {
       return IPA_TEST_RET_CODE_FAILURE;
     }
  }
 
  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_set_sio_cfg_map

==============================================================================*/
/*!
  @brief
  Configures a SIO mapping on the driver 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_set_sio_cfg_map
(
  ipa_test_engine_log_ch_s  *log_ch_ptr,
  ipa_sio_endpoint_config_s *sio_cfg_ptr
)
{
  ipa_sio_config_map_s config_map;
  ipa_err_code_e err;

  IPA_TF_ASSERT(log_ch_ptr != NULL);
  IPA_TF_ASSERT(sio_cfg_ptr != NULL);

  if (log_ch_ptr->action == IPA_SIO_CONFIG_ACTION_ADD)
  {
    /* First let's generate the mux id for this SIO mapping */
    log_ch_ptr->mux_id = 
    ipa_test_generate_unique_random_num(IPA_TEST_ENGINE_MUX_ID_NUM_MAX, 1,
                                        log_ch_ptr->phy_ch_ptr->mux_id_arr,
                                        log_ch_ptr->phy_ch_ptr->curr_log_ch_cnt);

    /*! Save the copy of mux id inorder to generate unique mux ids per endpoint */
    log_ch_ptr->phy_ch_ptr->mux_id_arr[log_ch_ptr->phy_ch_ptr->curr_log_ch_cnt]\
       = log_ch_ptr->mux_id;
    log_ch_ptr->phy_ch_ptr->curr_log_ch_cnt ++;

    /* Decide whether the mux ID needs to be overriden with UID */
    log_ch_ptr->mux_override = ipa_test_generate_random_num(TRUE, FALSE);
  }
  
  /* Now set all the params that we want to send to the driver */
  config_map.aggr_hdr_config_ptr = sio_cfg_ptr;
  config_map.mux_id = log_ch_ptr->mux_id;
  config_map.mux_override = log_ch_ptr->mux_override;
  config_map.physical_sio_stream = log_ch_ptr->phy_ch_ptr->stream_id;
  config_map.sio_rx_wm_ptr = &log_ch_ptr->rx_wm;
  config_map.sio_tx_wm_ptr = &log_ch_ptr->tx_wm;

  err = ipa_sioi_set_sio_config_mapping(log_ch_ptr->action,
                                        log_ch_ptr->stream_id,
                                        &config_map);
  
  if (err == IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*=========================================================================
                         WAN Cfg functions
==========================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_set_bearer_cfg

==============================================================================*/
/*!
  @brief
  Configures a WAN beaerer with the driver 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_set_bearer_cfg
(
  uint8                     bearer_idx, 
  ipa_test_engine_bearer_s  *bearer_ptr,
  ipa_test_engine_log_ch_s  *log_ch_ptr,
  uint8                     uid, 
  ipa_wan_subscription_id_t subs_id,
  uint8                     is_bridged, 
  uint8                     is_dpl_enabled,
  ipa_wan_bearer_tech_e     bearer_tech,
  uint8                     is_clat
)
{
  ipa_err_code_e err;
  ipa_wan_ds_reg_bearer_param_s reg_param;
  
  IPA_TF_ASSERT(log_ch_ptr != NULL);
  IPA_TF_ASSERT(bearer_ptr != NULL);

  /* Generate a random bearer ID */
  bearer_ptr->bearer_id = ipa_test_generate_random_num(IPA_WAN_MAX_BEARER_ID, 0);
  bearer_ptr->uid = uid;
  bearer_ptr->subs_id = subs_id;
  bearer_ptr->is_bridged = is_bridged;

  reg_param.uid = bearer_ptr->uid;
  reg_param.sio_stream_id = log_ch_ptr->stream_id;
  reg_param.l2_to_ipa_wm_ptr = &bearer_ptr->pdcp_wm;
  reg_param.ipa_to_ps_wm_ptr = &bearer_ptr->dl_wm;
  reg_param.ps_to_l2_wm_ptr = &bearer_ptr->ul_wm;
  reg_param.bearer_tech = bearer_tech;
  reg_param.bearer_id = bearer_idx;
  reg_param.subscription_id = bearer_ptr->subs_id;

  /* Initialize the watermarks for the bearer */
  IPA_TEST_WM_INIT(&bearer_ptr->dl_wm_q, &bearer_ptr->dl_wm);
  IPA_TEST_WM_INIT(&bearer_ptr->ul_wm_q, &bearer_ptr->ul_wm);
  IPA_TEST_WM_INIT(&bearer_ptr->pdcp_wm_q, &bearer_ptr->pdcp_wm);

  /* Initialize the Rx Mirror Watermark */
  IPA_TEST_WM_INIT(&bearer_ptr->mirror_dl_wm_q, 
                   &bearer_ptr->mirror_dl_wm);

  /* Now set the non-empty callback for data recvd from IPA driver */
  bearer_ptr->dl_wm.non_empty_func_data = 
    (void *)(uint32)bearer_idx;
  bearer_ptr->dl_wm.non_empty_func_ptr = 
    ipa_test_engine_sio_ds_rx_wm_ne_cb;

  /* Register pdcp watermark for bearer
  err = ipa_wan_pdcp_register_watermark(bearer_ptr->bearer_id,
                                        bearer_ptr->subs_id,
                                        &bearer_ptr->pdcp_wm);
  if (err != IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
  */

  /*! @todo Need to recheck this */
  /* Register DS watermarks for bearer */
  err = ipa_wani_ds_register_bearer(&reg_param);

  if (err != IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }

  /* Log random number of bytes in DPL */
  bearer_ptr->dpl_bytes_to_log =
     ipa_test_generate_random_num(IPA_TEST_MAX_PACKET_SIZE,40);

  /*! Configure the DPL watermark for the bearer if the DPL is enabled*/
  if (is_dpl_enabled == TRUE) 
  {
    err = ipa_wani_ds_config_dpl(bearer_ptr->uid,0,
                                 bearer_ptr->dpl_bytes_to_log,
                                 (uint32)bearer_idx);
  }
  
  if (err != IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }

  if (is_clat == FALSE) 
  {
  /* Set up the accelerated bridge for this bearer, if requested */
  if (is_bridged == TRUE)
  {
    err = 
      ipa_wani_ds_config_dl_bridge(bearer_ptr->uid, 
                                   IPA_DL_BRIDGE_STATE_BRIDGE);
  }
  }

  if (err == IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_bearer_destroy

==============================================================================*/
/*!
  @brief
  Destroys a WAN beaerer with the driver 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_bearer_destroy
(
  ipa_test_engine_bearer_s  *bearer_ptr
)
{
  ipa_err_code_e err;

  IPA_TF_ASSERT(bearer_ptr != NULL);

  /* Destroy the watermarks for the bearer */
  IPA_TEST_WM_DESTROY(&bearer_ptr->dl_wm);
  IPA_TEST_WM_DESTROY(&bearer_ptr->ul_wm);
  IPA_TEST_WM_DESTROY(&bearer_ptr->pdcp_wm);

  /* Destroy the Rx Mirror Watermark */
  IPA_TEST_WM_DESTROY(&bearer_ptr->mirror_dl_wm);

  /*! @todo Need to recheck this */
  /* DeRegister DS watermarks for bearer */
  err = ipa_wani_ds_deregister_bearer(bearer_ptr->uid);

  if (err == IPA_SUCCESS)
  {
    return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}
/*==============================================================================

  FUNCTION:  ipa_test_set_dpl_cfg

==============================================================================*/
/*!
  @brief
  Configures a DPL with the driver 

  @return
  None
*/
/*============================================================================*/
void ipa_test_set_dpl_cfg
(
  ipa_test_engine_dpl_s   *dpl_ptr
)

{
  IPA_TF_ASSERT(dpl_ptr != NULL);

 /* Initialize the DPL watermark*/
  IPA_TEST_WM_INIT(&dpl_ptr->dpl_wm_q,&dpl_ptr->dpl_wm);

  /* Intialize the DPL mirror watermark*/
  IPA_TEST_WM_INIT(&dpl_ptr->mirror_dpl_wm_q,&dpl_ptr->mirror_dpl_wm);

  dpl_ptr->dpl_wm.non_empty_func_ptr = 
    ipa_test_engine_dpl_rx_wm_ne_cb;
  
  /* Register the DPL watermark */
  ipa_wani_ds_register_dpl_wm(&dpl_ptr->dpl_wm);
}


/*==============================================================================

  FUNCTION:  ipa_test_dpl_destroy

==============================================================================*/
/*!
  @brief
  Destroys DPL watermarks

  @return
  None
*/
/*============================================================================*/
void ipa_test_dpl_destroy
(
  ipa_test_engine_dpl_s   *dpl_ptr
)
{
  IPA_TF_ASSERT(dpl_ptr != NULL);

  /* Destroy the watermarks for DPL */
  IPA_TEST_WM_DESTROY(&dpl_ptr->dpl_wm);
  IPA_TEST_WM_DESTROY(&dpl_ptr->mirror_dpl_wm);
}

/*=========================================================================
                         Ciphering functions
==========================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_generate_ciph_keystream

==============================================================================*/
/*!
  @brief
  Apply the ciphering algorithm on the data set to all zeroes to 
  get the keystream values for all the cipher param configs

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_ciph_keystream
(
  ipa_test_engine_ciph_params_s *ciph_params
)
{
  qcrypto::AES_CTR_F8   aes_obj;
  qcrypto::Snow3Gf8 snow3g_obj;

  switch (ciph_params->ciph_algo) 
  {
    /* Get keystream for AES algorithm using the cipher parameters*/
    case IPA_WAN_CIPHER_ALGO_AES:
      aes_obj.f8(ciph_params->ciph_key,
                 ciph_params->count_c,
                 ciph_params->bearer_id,
                 ciph_params->direction,
                 ciph_params->dummy_data_dsm_ptr->data_ptr,
                 IPA_BYTES_TO_BITS(ciph_params->keystream_dsm_ptr->used),
                 ciph_params->keystream_dsm_ptr->data_ptr,
                 0);
      break;


    /* Get keystream for SNOW3G algorithm using the cipher parameters*/
    case IPA_WAN_CIPHER_ALGO_LTE_SNOW3G:
      snow3g_obj.f8(ciph_params->ciph_key,
                    ciph_params->count_c,
                    ciph_params->bearer_id,
                    ciph_params->direction,
                    ciph_params->dummy_data_dsm_ptr->data_ptr,
                    IPA_BYTES_TO_BITS(ciph_params->keystream_dsm_ptr->used),
                    ciph_params->keystream_dsm_ptr->data_ptr,
                    0);
      break;

    /* Get keystream for ZUC algorithm using the cipher parameters*/
    case IPA_WAN_CIPHER_ALGO_ZUC:
      zuc_dsm_EEA3(ciph_params->ciph_key,
                   ciph_params->count_c, 
                   ciph_params->bearer_id,
                   ciph_params->direction,
                   ciph_params->keystream_dsm_ptr); 
      break;

    case IPA_WAN_CIPHER_ALGO_NONE:
      break;

    default:
      IPA_TF_ASSERT(0);
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_xor_ciph_keystream

==============================================================================*/
/*!
  @brief
  Cipher the DSM buffer data by XORing the passed DSM pointer 
  with the keystream value of the cipher param used

  @return
  None
*/
/*============================================================================*/
void ipa_test_xor_ciph_keystream
(
  dsm_item_type                   *tx_dsm_ptr,
  uint16                          pkt_len,
  ipa_test_engine_ciph_params_s   *ciph_params
)
{
  uint8 *tmp_pkt_data_ptr , *tmp_keystream_data_ptr;
  dsm_item_type *tmp_keystream_dsm_ptr = NULL;
  dsm_item_type *tmp_data_dsm_ptr = NULL;

  tmp_keystream_dsm_ptr = ciph_params->keystream_dsm_ptr;
  tmp_data_dsm_ptr = tx_dsm_ptr;

  tmp_pkt_data_ptr = tmp_data_dsm_ptr->data_ptr;
  tmp_keystream_data_ptr = tmp_keystream_dsm_ptr->data_ptr;

  ciph_params->num_pkts_processed++;

  while (pkt_len != 0)
  {
    *tmp_pkt_data_ptr ^= *tmp_keystream_data_ptr;

    tmp_pkt_data_ptr++;

    tmp_keystream_data_ptr++;

    pkt_len--;

    if (tmp_data_dsm_ptr->used == 
        (tmp_pkt_data_ptr - tmp_data_dsm_ptr->data_ptr)) 
    {
      if (tmp_data_dsm_ptr->pkt_ptr == NULL) 
      {
        break;
      }
      tmp_data_dsm_ptr = tmp_data_dsm_ptr->pkt_ptr;
      tmp_pkt_data_ptr = tmp_data_dsm_ptr->data_ptr;
    }

    if (tmp_keystream_dsm_ptr->used == 
        (tmp_keystream_data_ptr - tmp_keystream_dsm_ptr->data_ptr)) 
    {
      if (tmp_keystream_dsm_ptr->pkt_ptr == NULL) 
      {
        break;
      }
      tmp_keystream_dsm_ptr = tmp_keystream_dsm_ptr->pkt_ptr;
      tmp_keystream_data_ptr = tmp_keystream_dsm_ptr->data_ptr;
    }
  }

  IPA_TF_ASSERT(pkt_len == 0);
}

/*=========================================================================
                         IP Pkt generation functions
==========================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_calculate_eac

==============================================================================*/
/*!
  @brief
  Calculates the End around carry 

  @return
  None
*/
/*============================================================================*/
uint16 ipa_test_calculate_eac
(    
  /* Carries in high order 16 bits */
  uint32 sum 
)
{
  uint16 csum;

  while((csum = (uint16)(sum >> 16)) != 0)
  {
    sum = csum + (sum & 0xffffL);
  }

  /* Chops to 16 bits */
  return (uint16) (sum & 0xffffL); 
}

/*==============================================================================

  FUNCTION:  ipa_test_calculate_lcsum

==============================================================================*/
/*!
  @brief
  Calculates the lcsum value

  @return
  None
*/
/*============================================================================*/
uint16 ipa_test_calculate_lcsum
(
  uint16 *buf_ptr,
  uint16 cnt
)
{
  uint32 sum = 0;
  uint16 result;
  uint16 last_val = 0;

  /* Make sure address is aligned to 2 bytes */
  if ((cnt > 1) && (((uint32)buf_ptr & 0x2) == 1))
  {
    sum = (uint32)*((uint8 *)buf_ptr++);
  }

  while(cnt > 1)
  {  
    sum += *(buf_ptr++);
    sum = ipa_test_calculate_eac(sum);
    cnt -= 2;
  }

  /* We pad with a zero if we have odd number of bytes */
  if (cnt == 1) 
  {
    last_val |= (uint16)*(uint8 *)buf_ptr;
    sum += last_val;
    sum = ipa_test_calculate_eac(sum);
  }

  result = ipa_test_calculate_eac(sum);

  return result;
}

/*==============================================================================

  FUNCTION:  ipa_test_calculate_cksum

==============================================================================*/
/*!
  @brief
  Calculates the Cksum value

  @return
  None
*/
/*============================================================================*/
uint16 ipa_test_calculate_cksum
(
  /* Array of bytes of data */
  uint16  *buf_ptr,
  /* Number of bytes */
  uint32  count
)
{
  uint32 sum = 0L; 

  IPA_TF_ASSERT(buf_ptr != NULL);

  sum = ipa_test_calculate_lcsum((uint16 *)buf_ptr, count);

  return ((uint16)(~ipa_test_calculate_eac(sum) & 0xffff));
}

/*==============================================================================

  FUNCTION:  ipa_test_calculate_pseudo_hdr_cksum

==============================================================================*/
/*!
  @brief
  Calculates the Cksum value of Pseudo hdr

  @return
  None
*/
/*============================================================================*/
uint16 ipa_test_calculate_pseudo_hdr_cksum
(
  /* Array of bytes of data */
  uint8  *buf_pkt_ptr
)
{
  int i = 0;
  ipa_test_ipv6_pseudo_header_type v6_pseudo_hdr;
  ipa_test_ipv4_pseudo_header_type v4_pseudo_hdr;
  uint16 pseudo_hdr_chksum; 
  
  if (IPA_TEST_GET_IP_VERSION(buf_pkt_ptr) == 0x6)
  {
    
	IPA_MEMSET(&v6_pseudo_hdr, 0, sizeof(v6_pseudo_hdr)); 
	
	for (i = 0; i < 8; i++)
	{
	  v6_pseudo_hdr.src_addr[i] = *(uint16*)(&(buf_pkt_ptr[IPA_TEST_IPV6_SRC_ADDR_OFST + 2*i]));
	  v6_pseudo_hdr.dest_addr[i] = *(uint16*)(&(buf_pkt_ptr[IPA_TEST_IPV6_DEST_ADDR_OFST + 2*i]));
    }
	
	if (buf_pkt_ptr[6] == IPA_TEST_ENGINE_ROUTING_EXTN_HDR_TYPE ||
		buf_pkt_ptr[6] == IPA_TEST_ENGINE_FRAG_EXTN_HDR_TYPE ||
        buf_pkt_ptr[6] == IPA_TEST_ENGINE_DEST_OPTS_EXTN_HDR_TYPE)
	{
	  v6_pseudo_hdr.tcp_length = ipa_htons(*((uint16*)(buf_pkt_ptr+4))) - sizeof(ipa_test_ipv6_pkt_frag_extn_hdr_s);
      #if 0
	  if (buf_pkt_ptr[40] |= IPA_TEST_ENGINE_IP_PKT_PROT_TCP)
	  {
	    v6_pseudo_hdr.protocol = IPA_TEST_TCP_PROT;
	  }
	  else if (buf_pkt_ptr[40] |= IPA_TEST_ENGINE_IP_PKT_PROT_UDP)
	  {
	    v6_pseudo_hdr.protocol = IPA_TEST_UDP_PROT;
	  }
	  #endif
	  v6_pseudo_hdr.protocol = buf_pkt_ptr[40];
	}
	else
	{
	  v6_pseudo_hdr.tcp_length = ipa_htons(*((uint16*)(buf_pkt_ptr+4)));
      #if 0
	  if (buf_pkt_ptr[6] |= IPA_TEST_ENGINE_IP_PKT_PROT_TCP)
	  {
	    v6_pseudo_hdr.protocol = IPA_TEST_TCP_PROT;
	  }
	  else if (buf_pkt_ptr[6] |= IPA_TEST_ENGINE_IP_PKT_PROT_UDP)
	  {
	    v6_pseudo_hdr.protocol = IPA_TEST_UDP_PROT;
	  }
	  #endif
	  v6_pseudo_hdr.protocol = buf_pkt_ptr[6];
	}
	
	 pseudo_hdr_chksum = ipa_test_calculate_cksum((uint16*) &v6_pseudo_hdr, sizeof(v6_pseudo_hdr));
	 pseudo_hdr_chksum = ~pseudo_hdr_chksum & 0xFFFF;
	 //return pseudo_hdr_chksum;
  }
  else if (IPA_TEST_GET_IP_VERSION(buf_pkt_ptr) == 0x4)
  {
    IPA_MEMSET(&v4_pseudo_hdr, 0, sizeof(v4_pseudo_hdr)); 

	v4_pseudo_hdr.src_addr = *(uint32*)(&buf_pkt_ptr[IPA_TEST_IPV4_SRC_ADDR_OFST]);
  	v4_pseudo_hdr.dest_addr = *(uint32*)(&buf_pkt_ptr[IPA_TEST_IPV4_DEST_ADDR_OFST]);

	#if 0
	if (buf_pkt_ptr[9] |= IPA_TEST_ENGINE_IP_PKT_PROT_TCP)
	{
	  v4_pseudo_hdr.protocol = IPA_TEST_TCP_PROT;
	}
	else if (buf_pkt_ptr[9] |= IPA_TEST_ENGINE_IP_PKT_PROT_UDP)
	{
	  v4_pseudo_hdr.protocol = IPA_TEST_UDP_PROT;
	}
	else
	{ 
	  IPA_ASSERT (0);
    }
    #endif
	
	v4_pseudo_hdr.protocol = buf_pkt_ptr[9];
	
  	v4_pseudo_hdr.tcp_length = ipa_htons(*((uint16*)(&buf_pkt_ptr[2]))) - IPA_TEST_IPV4_HDR_LEN;

	pseudo_hdr_chksum = ipa_test_calculate_cksum((uint16*) &v4_pseudo_hdr, sizeof(v4_pseudo_hdr));
    pseudo_hdr_chksum = ~pseudo_hdr_chksum & 0xFFFF;
	//return pseudo_hdr_chksum;
  }
  else
  {
    IPA_ASSERT (0);
  }

  return pseudo_hdr_chksum;
}


/*==============================================================================

  FUNCTION:  ipa_test_calculate_tcp_cksum

==============================================================================*/
/*!
  @brief
  Calculates the TCP Cksum value

  @return
  None
*/
/*============================================================================*/
#if 0
void ipa_test_calculate_tcp_cksum
(
  /* IP Packet buffer */
  uint8 *buf_pkt_ptr
)
{
  int i = 0;
  
  
  if (IPA_TEST_GET_IP_VERSION(buf_pkt_ptr) == 0x6)
  {
  	

	IPA_MEMSET(&v6_pseudo_hdr, 0, sizeof(v6_pseudo_hdr));

    for (i = 0; i < 16; i++)
    {
	  v6_pseudo_hdr.src_addr[i] = buf_pkt_ptr[IPA_TEST_IPV6_SRC_ADDR_OFST + i];
	  v6_pseudo_hdr.dest_addr[i] = buf_pkt_ptr[IPA_TEST_IPV6_DEST_ADDR_OFST + i];
    }

	v6_pseudo_hdr.protocol = IPA_TEST_TCP_PROT;
	v6_pseudo_hdr.tcp_length = *((uint16*)(buf_pkt_ptr+4));

    checksum_data = (uint8*)IPA_MALLOC(1500 + sizeof(v6_pseudo_hdr));
	IPA_MEMCPY(checksum_data, &v6_pseudo_hdr, sizeof(v6_pseudo_hdr));
	IPA_MEMCPY((uint8*)(&checksum_data[sizeof(v6_pseudo_hdr)]), 
			   (uint8*)(&buf_pkt_ptr[IPA_TEST_IPV6_HDR_LEN]), 
			   ipa_htons
(v6_pseudo_hdr.tcp_length));
	
    checksum = ipa_test_calculate_cksum((uint16*) checksum_data, 
					(ipa_htons(v6_pseudo_hdr.tcp_length)) + sizeof(v6_pseudo_hdr));
	checksum = ~checksum;
    checksum &= 0xFFFF;
	//checksum = checksum;

  }
  else if (IPA_TEST_GET_IP_VERSION(buf_pkt_ptr) == 0x4)
  {
    IPA_MEMSET(&v4_pseudo_hdr, 0, sizeof(v4_pseudo_hdr));

    v4_pseudo_hdr.src_addr = buf_pkt_ptr[IPA_TEST_IPV4_SRC_ADDR_OFST];
    v4_pseudo_hdr.dest_addr = buf_pkt_ptr[IPA_TEST_IPV4_DEST_ADDR_OFST];
    

	v4_pseudo_hdr.protocol = IPA_TEST_TCP_PROT;
	v4_pseudo_hdr.tcp_length = ipa_ntohs(ipa_htons(*((uint16*)(buf_pkt_ptr+2))) - IPA_TEST_IPV4_HDR_LEN);

    checksum_data = (uint8*)IPA_MALLOC(1500 + sizeof(v4_pseudo_hdr));
	IPA_MEMCPY(checksum_data, &v4_pseudo_hdr, sizeof(v4_pseudo_hdr));
	IPA_MEMCPY((uint8*)(&checksum_data[sizeof(v4_pseudo_hdr)]), (uint8*)(&buf_pkt_ptr[IPA_TEST_IPV4_HDR_LEN]), 
																						ipa_htons
(v4_pseudo_hdr.tcp_length));
    checksum = ipa_test_calculate_cksum((uint16*) checksum_data, (ipa_htons(v4_pseudo_hdr.tcp_length)) + sizeof(v4_pseudo_hdr));
	checksum = ~checksum;
    checksum &= 0xFFFF;
	//checksum = checksum;

  }
  else
  	IPA_ASSERT (0);

  *(uint16*)&buf_pkt_ptr[IPA_TEST_IPV4_HDR_LEN + IPA_TEST_TCP_CKSUM_HDR_OFST] = checksum;
}
#endif 

/*==============================================================================

  FUNCTION:  ipa_test_get_frag_status

==============================================================================*/
/*!
  @brief
  Get the frag info whether its on or not

  @return
  TRUE if frag is ON. FALSE if frag is off.
*/
/*============================================================================*/
boolean ipa_test_get_frag_status()
{
  return ipa_test.ip_pkt.frag_ctxt.frag_on;
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_ipv4_fragment

==============================================================================*/
/*!
  @brief
  Generate IP Fragment according to the ip_pkt_cfg structure 
  in the specified memory location

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_ipv4_fragment
(
  ipa_test_engine_ip_pkt_info_s *pkt_info
)
{
  ipa_test_ip_pkt_frag_hdr_s   frag_hdr;
  uint16                       payload_len, *frag_hdr_ptr = NULL;

  IPA_TF_ASSERT(pkt_info);

  payload_len = pkt_info->pkt_len - pkt_info->tot_hdr_len;

  /* Store and replace the original pkt len if this is first frag */
  if (ipa_test.ip_pkt.frag_ctxt.frag_on == FALSE)
  {
    IPA_TF_ASSERT(payload_len > 
                  ipa_test.ip_pkt.frag_ctxt.frag_pkt_len);
    ipa_test.ip_pkt.frag_ctxt.orig_pkt_len = 
      ipa_test.ip_pkt.frag_ctxt.rem_pkt_len = 
      payload_len;
    payload_len = ipa_test.ip_pkt.frag_ctxt.frag_pkt_len;
    ipa_test.ip_pkt.frag_ctxt.last_frag_ofst = 0;
    ipa_test.ip_pkt.frag_ctxt.frag_on = TRUE;
    ipa_test.ip_pkt.frag_ctxt.curr_ip_id = *(uint16 *)&pkt_info->pkt_buf_ptr[4];
    frag_hdr.df = 0;
    frag_hdr.mf = 1;
    frag_hdr.ofst = 0;
    pkt_info->pkt_len = payload_len + pkt_info->tot_hdr_len;
    pkt_info->payload_len = payload_len;
  }
  else
  {
    ipa_test.ip_pkt.frag_ctxt.last_frag_ofst += 
      IPA_MIN(ipa_test.ip_pkt.frag_ctxt.frag_pkt_len,
              ipa_test.ip_pkt.frag_ctxt.rem_pkt_len);
    payload_len = ipa_test.ip_pkt.frag_ctxt.frag_pkt_len;
    frag_hdr.df = 0;
    frag_hdr.mf = 1;
    frag_hdr.ofst = ipa_test.ip_pkt.frag_ctxt.last_frag_ofst;
    pkt_info->pkt_len = payload_len + pkt_info->ip_hdr_len;
    pkt_info->payload_len = payload_len;
    *(uint16 *)&pkt_info->pkt_buf_ptr[4] = ipa_test.ip_pkt.frag_ctxt.curr_ip_id;
  }

  if (ipa_test.ip_pkt.frag_ctxt.rem_pkt_len <=  
      ipa_test.ip_pkt.frag_ctxt.frag_pkt_len)
  {
    /* this is the last frag that can be created */
    ipa_test.ip_pkt.frag_ctxt.frag_on = FALSE;
    frag_hdr.df = 0;
    frag_hdr.mf = 0;
    frag_hdr.ofst = ipa_test.ip_pkt.frag_ctxt.last_frag_ofst;
    payload_len = ipa_test.ip_pkt.frag_ctxt.rem_pkt_len;
    pkt_info->pkt_len = payload_len + pkt_info->ip_hdr_len;
    pkt_info->payload_len = payload_len;
  }
  else
  {
    ipa_test.ip_pkt.frag_ctxt.rem_pkt_len -= 
      ipa_test.ip_pkt.frag_ctxt.frag_pkt_len;
  }

  /* write to frag flags/offset in the IP header */
  frag_hdr_ptr = (uint16 *)&frag_hdr;
  IPA_TEST_SET_IP_FRAG_HDR(pkt_info->pkt_buf_ptr, *frag_hdr_ptr);
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_ipv6_fragment

==============================================================================*/
/*!
  @brief
  Generate IP Fragment according to the ip_pkt_cfg structure 
  in the specified memory location

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_ipv6_fragment
( 
  ipa_test_engine_ip_pkt_cfg_s   *ip_pkt_cfg_ptr,
  ipa_test_engine_ip_pkt_info_s  *pkt_info
)
{
  uint16                             payload_len;
  uint64                            *frag_hdr_ptr = NULL;
  ipa_test_ipv6_pkt_frag_extn_hdr_s  frag_hdr;
  uint8 prot = 0;
 
  IPA_TF_ASSERT(pkt_info);
  IPA_TF_ASSERT(ip_pkt_cfg_ptr);
  payload_len = pkt_info->pkt_len - pkt_info->tot_hdr_len;

  /* TCP protocol*/
  if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_TCP)
  {
    prot = IPA_TEST_TCP_PROT;
  }
  /* UDP protocol*/
  else if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_UDP) 
  {
    prot = IPA_TEST_UDP_PROT;
  }
  /* ICMP protocol */
  else if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_ICMP) 
  {
    prot = IPA_TEST_ICMP_PROT;
  }

  /*! Memset thefragmentation extension header to 0*/
  IPA_MEMSET(&frag_hdr, 0 , sizeof(ipa_test_ipv6_pkt_frag_extn_hdr_s));
  /*! Set the frag length in the frag context */
  ipa_test.ip_pkt.frag_ctxt.frag_pkt_len = 
        ip_pkt_cfg_ptr->params.frag_pkt_len;

  /*! Next header as the fragment extension header */
  frag_hdr.nxt_hdr = prot;

  /* Store and replace the original pkt len if this is first frag */
  if (ipa_test.ip_pkt.frag_ctxt.frag_on == FALSE)
  {
    IPA_TF_ASSERT(payload_len > 
                  ipa_test.ip_pkt.frag_ctxt.frag_pkt_len);
    ipa_test.ip_pkt.frag_ctxt.orig_pkt_len = 
      ipa_test.ip_pkt.frag_ctxt.rem_pkt_len = 
      payload_len;
    payload_len = ipa_test.ip_pkt.frag_ctxt.frag_pkt_len;
    ipa_test.ip_pkt.frag_ctxt.last_frag_ofst = 0;
    ipa_test.ip_pkt.frag_ctxt.frag_on = TRUE;
    ipa_test.ip_pkt.frag_ctxt.curr_ip_id = *(uint16 *)&pkt_info->pkt_buf_ptr[44];
    frag_hdr.ofst = 0;
    frag_hdr.mf = 1;
    pkt_info->pkt_len = payload_len + pkt_info->tot_hdr_len ;
    pkt_info->payload_len = payload_len;
  } 
  else
  {
    ipa_test.ip_pkt.frag_ctxt.last_frag_ofst += 
      IPA_MIN(ipa_test.ip_pkt.frag_ctxt.frag_pkt_len,
              ipa_test.ip_pkt.frag_ctxt.rem_pkt_len);
    payload_len = ipa_test.ip_pkt.frag_ctxt.frag_pkt_len;
    frag_hdr.ofst = ipa_test.ip_pkt.frag_ctxt.last_frag_ofst;
    /*! Add the extension fragmentation header length for all fragments */
    pkt_info->pkt_len = payload_len + pkt_info->ip_hdr_len + pkt_info->extn_hdr_len;
    pkt_info->payload_len = payload_len;
    frag_hdr.mf = 1;
    frag_hdr.ipid = ipa_test.ip_pkt.frag_ctxt.curr_ip_id;
  }

  if (ipa_test.ip_pkt.frag_ctxt.rem_pkt_len <=  
      ipa_test.ip_pkt.frag_ctxt.frag_pkt_len)
  {
    /* this is the last frag that can be created */
    ipa_test.ip_pkt.frag_ctxt.frag_on = FALSE;
    frag_hdr.ofst = ipa_test.ip_pkt.frag_ctxt.last_frag_ofst;
    payload_len = ipa_test.ip_pkt.frag_ctxt.rem_pkt_len;
    /*! Add the extension fragmentation header length for all fragments */
    pkt_info->pkt_len = payload_len + pkt_info->ip_hdr_len + pkt_info->extn_hdr_len;
    pkt_info->payload_len = payload_len;
    frag_hdr.mf = 0;
  }
  else
  {
    ipa_test.ip_pkt.frag_ctxt.rem_pkt_len -= 
      ipa_test.ip_pkt.frag_ctxt.frag_pkt_len;
  }
  /* write to frag flags/offset in the IP header */
  frag_hdr_ptr = (uint64*)&frag_hdr;
  IPA_TEST_SET_IPV6_FRAG_HDR(pkt_info->pkt_buf_ptr, *frag_hdr_ptr);
}

/*==============================================================================

  FUNCTION:  ipa_test_add_ipv6_ext_hdr


==============================================================================*/
/*!
  @brief
  Adds Ipv6 Extension Header according to the ip_pkt_cfg structure 
  in the specified memory location

  @return
  None
*/
/*============================================================================*/
void ipa_test_add_ipv6_ext_hdr
(
  ipa_test_engine_ip_pkt_cfg_s  *ip_pkt_cfg_ptr,
  ipa_test_engine_ip_pkt_info_s *pkt_info
)
{
  uint8 prot = 0;

  IPA_TF_ASSERT(ip_pkt_cfg_ptr);
  IPA_TF_ASSERT(pkt_info);

  /* TCP protocol*/
  if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_TCP)
  {
    prot = IPA_TEST_TCP_PROT;
  }
  /* UDP protocol*/
  else if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_UDP) 
  {
    prot = IPA_TEST_UDP_PROT;
  }
  /* ICMP protocol */
  else if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_ICMP) 
  {
    prot = IPA_TEST_ICMP_PROT;
  }

  /*! Add fragmentation extension header if fragmentation is enabled*/
  if (ip_pkt_cfg_ptr->params.frag_pkt_len != 0)
  {
    /* write Next_Header field in IPv6 header i.e. FRAG_EXTN_HDR type */
    IPA_TEST_SET_IPV6_NEXT_HDR(pkt_info->pkt_buf_ptr, 
                             (uint8)ip_pkt_cfg_ptr->params.ipv6_ext_hdr_type);

    ipa_test_generate_ipv6_fragment(ip_pkt_cfg_ptr, pkt_info);
  }
  else
  {
    /*! Memset the IPV6 extension header to 0 */
    IPA_MEMSET( (pkt_info->pkt_buf_ptr + pkt_info->ip_hdr_len), 0, 
                (pkt_info->extn_hdr_len) );

    /* write Next_Header field in IPv6 header */
    IPA_TEST_SET_IPV6_NEXT_HDR(pkt_info->pkt_buf_ptr, 
                             (uint8)ip_pkt_cfg_ptr->params.ipv6_ext_hdr_type);

    /* write Next_Header field in Extension header */
    IPA_TEST_SET_IPV6_EXT_NEXT_HDR(pkt_info->pkt_buf_ptr, prot);

    /* write Header_Ext_len field in Extension header */
    IPA_TEST_SET_IPV6_EXT_HDR_EXT_LEN( pkt_info->pkt_buf_ptr,
                           /* Subtract 8 bytes because Len field excluded it*/
                              ((pkt_info->extn_hdr_len - sizeof(uint64))/8) );
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_ip_packet

==============================================================================*/
/*!
  @brief
  Generate IP Packet according to the ip_pkt_cfg structure 
  in the specified memory location. 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_ip_packet
(
  ipa_test_engine_ip_pkt_cfg_s  *ip_pkt_cfg_ptr, 
  ipa_test_engine_ip_pkt_info_s *pkt_info
)
{
  uint16 offset;
  uint16 icmp_check_sum = 0;

  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 prot = 0;

  IPA_TF_ASSERT(pkt_info);
  IPA_TF_ASSERT(ip_pkt_cfg_ptr);
  /*! If the input pkt version in the global ip pkt config
      for the test is IPA_IP_TYPE_MAX randomly generate the IP version 
      for the packet and this get stored in the TX pkt trace */
  if (ip_pkt_cfg_ptr->version == IPA_IP_TYPE_MAX)
  {
    pkt_info->ip_pkt_version = (ipa_ip_type_e) ipa_test_generate_random_num(
                                           IPA_IP_TYPE_IPV6, IPA_IP_TYPE_IPV4);
  }
  /* Assign the pkt version for this packet set in the pkt config for the test case. */
  else
  {
    pkt_info->ip_pkt_version  = ip_pkt_cfg_ptr->version;
  }

  if (ip_pkt_cfg_ptr->params.frag_pkt_len == 0)
  {
    /*! DF bit is randomly set or cleared */
    pkt_info->df_bit = (uint8) ipa_test_generate_random_num(TRUE, FALSE);
  }
  else
  { 
    pkt_info->df_bit = 0;
  }

  if (pkt_info->ip_pkt_version == IPA_IP_TYPE_IPV4) 
  {
    pkt_info->ip_hdr_len = IPA_TEST_IPV4_HDR_LEN; 
    pkt_info->tot_hdr_len = pkt_info->ip_hdr_len;
  }
  else if (pkt_info->ip_pkt_version  == IPA_IP_TYPE_IPV6)
  {
    pkt_info->ip_hdr_len = IPA_TEST_IPV6_HDR_LEN; 
    pkt_info->tot_hdr_len = pkt_info->ip_hdr_len;

    /*! Configure extension header params if it is a IPV6 frag pkt */
    if (ip_pkt_cfg_ptr->params.frag_pkt_len != 0)
    {
      ip_pkt_cfg_ptr->params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_FRAG_EXTN_HDR_TYPE;
    }

    switch (ip_pkt_cfg_ptr->params.ipv6_ext_hdr_type)
    {
      /*TBD: Add more cases for more Extension Header types */
      case IPA_TEST_ENGINE_ROUTING_EXTN_HDR_TYPE: /* Routing Extn Header */
        pkt_info->extn_hdr_len = sizeof(ipa_test_ipv6_pkt_routing_next_hdr_s);
        if (ip_pkt_cfg_ptr->params.ipv6_ext_hdr_opt_len != 0)
    {
          pkt_info->extn_hdr_len += ip_pkt_cfg_ptr->params.ipv6_ext_hdr_opt_len * 8;
    }
        pkt_info->tot_hdr_len += pkt_info->extn_hdr_len;
        pkt_info->is_ipv6_extn_hdr_present = TRUE;
        break;

      case IPA_TEST_ENGINE_DEST_OPTS_EXTN_HDR_TYPE: /* Destination Options Extn Header */
        pkt_info->extn_hdr_len = sizeof(ipa_test_ipv6_pkt_dest_opts_next_hdr_s); 
        if (ip_pkt_cfg_ptr->params.ipv6_ext_hdr_opt_len != 0)
        {
          pkt_info->extn_hdr_len += ip_pkt_cfg_ptr->params.ipv6_ext_hdr_opt_len * 8;
  }
        pkt_info->tot_hdr_len += pkt_info->extn_hdr_len;
        pkt_info->is_ipv6_extn_hdr_present = TRUE;
        break;

      /*! Fragmentation Extension header type */
      case IPA_TEST_ENGINE_FRAG_EXTN_HDR_TYPE: 
        pkt_info->extn_hdr_len = sizeof(ipa_test_ipv6_pkt_frag_extn_hdr_s);
        pkt_info->tot_hdr_len += pkt_info->extn_hdr_len;
        pkt_info->is_ipv6_extn_hdr_present = TRUE;
        break;

      case IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE:
        /* If the header type is not provided randomly choose to insert the extension header*/
        pkt_info->is_ipv6_extn_hdr_present = (uint8) ipa_test_generate_random_num(TRUE, FALSE);
        if (pkt_info->is_ipv6_extn_hdr_present == TRUE)
  {
          ip_pkt_cfg_ptr->params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_DEST_OPTS_EXTN_HDR_TYPE;
          pkt_info->extn_hdr_len = sizeof(ipa_test_ipv6_pkt_dest_opts_next_hdr_s); 
          pkt_info->tot_hdr_len += pkt_info->extn_hdr_len;
  }
        break;

      default:
        pkt_info->is_ipv6_extn_hdr_present = FALSE;
    }
  }

  /* TCP protocol*/
  if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_TCP)
  {
    pkt_info->tot_hdr_len += IPA_TEST_TCP_HDR_LEN;
    prot = IPA_TEST_TCP_PROT;
  }
  /* UDP protocol*/
  else if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_UDP) 
  {
    pkt_info->tot_hdr_len += IPA_TEST_UDP_HDR_LEN;
    prot = IPA_TEST_UDP_PROT;
  }
  /* ICMP protocol */
  else if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_ICMP) 
  {
    pkt_info->tot_hdr_len += IPA_TEST_ICMP_HDR_LEN;
    prot = IPA_TEST_ICMP_PROT;
  }

  if (ip_pkt_cfg_ptr->payload_len == 0) 
  {
    pkt_info->payload_len = 
      ipa_test_generate_random_num(ip_pkt_cfg_ptr->params.max_pkt_size - pkt_info->tot_hdr_len,
                                   1);
  }
  else
  {
    pkt_info->payload_len = ip_pkt_cfg_ptr->payload_len;
  }
    /* Check for total_pkt_len<=1536 for non-frag pkts. */

    if (ip_pkt_cfg_ptr->params.frag_pkt_len == 0)
  {
    IPA_TF_ASSERT(ip_pkt_cfg_ptr->payload_len + pkt_info->tot_hdr_len <= 
                    ip_pkt_cfg_ptr->params.max_pkt_size);
  }

  pkt_info->pkt_len = pkt_info->payload_len + pkt_info->tot_hdr_len;

  /* Now allocate a new buffer for the packet */
  pkt_info->pkt_buf_ptr = (uint8 *)IPA_MALLOC(pkt_info->pkt_len);
  /* Clear the packet buffer*/
  IPA_MEMSET(pkt_info->pkt_buf_ptr, 0, pkt_info->pkt_len);
  /* Configure header information*/

  IPA_TEST_SET_IP_VERSION(pkt_info->pkt_buf_ptr, pkt_info->ip_pkt_version );

  /*Configure IPV4 Header Specific info*/
  if (pkt_info->ip_pkt_version == IPA_IP_TYPE_IPV4) 
  {
    IPA_TEST_SET_IP_PROTOCOL(pkt_info->pkt_buf_ptr, prot);
    IPA_TEST_SET_IP_IHL(pkt_info->pkt_buf_ptr, pkt_info->ip_hdr_len);
    IPA_TEST_SET_IP_IPID(pkt_info->pkt_buf_ptr, ipa_test.ip_pkt.ip_id_cnt++);
    /*! Set the DF bit if the test has set the df bit in the pkt config*/
    if (pkt_info->df_bit == TRUE)
    {
      pkt_info->pkt_buf_ptr[6] |= 0x40;
    }

    if (ip_pkt_cfg_ptr->params.frag_pkt_len != 0)
    {
      ipa_test.ip_pkt.frag_ctxt.frag_pkt_len = 
        ip_pkt_cfg_ptr->params.frag_pkt_len;
      ipa_test_generate_ipv4_fragment(pkt_info);
    }

    IPA_TEST_SET_IP_PKT_LEN(pkt_info->pkt_buf_ptr, pkt_info->pkt_len);
  }
  /* Configure IPV6 specific Info*/
  else
  {
    // Payload len field in IP V6 hdr includes Extn hdr & L4 prot hdr len also
    IPA_TEST_SET_IP_PAYLOAD_LEN(pkt_info->pkt_buf_ptr, (pkt_info->pkt_len - pkt_info->ip_hdr_len));

    if (pkt_info->is_ipv6_extn_hdr_present == FALSE)
    {
    IPA_TEST_SET_IP_NXT_HEADER(pkt_info->pkt_buf_ptr, prot);
  }
    else
    {
      /* Invoke function to add Extension header fields */
      ipa_test_add_ipv6_ext_hdr(ip_pkt_cfg_ptr, pkt_info);
    }
  }
  /* Configure IP Payload*/
  /* Random Data payload*/
  if (ip_pkt_cfg_ptr->is_random_data == TRUE)
  {
    if (ipa_test.ip_pkt.is_random_gen == FALSE) 
    {
      ipa_test_generate_random_data(ipa_test.ip_pkt.random_data_buffer,
                                    IPA_TEST_RANDOM_DATA_BUFFER_SIZE);
      ipa_test.ip_pkt.is_random_gen = TRUE;
    }
    if(pkt_info->is_ipv6_extn_hdr_present == TRUE)
    {
      offset =
      (uint16)ipa_test_generate_random_num((IPA_TEST_RANDOM_DATA_BUFFER_SIZE -
                                           (pkt_info->payload_len + pkt_info->extn_hdr_len)), 0);
    }
    else
    {
      offset =
      (uint16)ipa_test_generate_random_num((IPA_TEST_RANDOM_DATA_BUFFER_SIZE-
                                            pkt_info->payload_len), 0);
    }
    IPA_MEMCPY(pkt_info->pkt_buf_ptr + pkt_info->tot_hdr_len, 
               ipa_test.ip_pkt.random_data_buffer + offset,
               pkt_info->payload_len);
  }
  /* Sequential data payload*/
  else
  {
    ipa_test_generate_sequence_data(pkt_info->pkt_buf_ptr + pkt_info->tot_hdr_len,
                                    pkt_info->payload_len);
  }
  if ((ip_pkt_cfg_ptr->params.frag_pkt_len == 0) ||
      ((ipa_test.ip_pkt.frag_ctxt.frag_on == TRUE) && 
       (ipa_test.ip_pkt.frag_ctxt.last_frag_ofst == 0)))
  {
    /*Configure UDP header specific info*/
    if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_UDP)
    {
  
      IPA_TEST_SET_UDP_PORTS(&pkt_info->pkt_buf_ptr[pkt_info->ip_hdr_len + pkt_info->extn_hdr_len],
                             ip_pkt_cfg_ptr->prot_params.udp.src_port,
                             ip_pkt_cfg_ptr->prot_params.udp.dst_port);
    }
     /* Configure ICMP  message */
    if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_ICMP)
    {
      IPA_TEST_SET_ICMP_TYPE_CODE(&pkt_info->pkt_buf_ptr[pkt_info->ip_hdr_len + pkt_info->extn_hdr_len],
                                  ip_pkt_cfg_ptr->prot_params.icmp.type, 
                                  ip_pkt_cfg_ptr->prot_params.icmp.code);
      icmp_check_sum = 0xCCCC;
     
      IPA_TEST_SET_ICMP_CKSUM(&pkt_info->pkt_buf_ptr[pkt_info->ip_hdr_len + pkt_info->extn_hdr_len],
                              icmp_check_sum);
    }
  }

  /*if (IPA_TEST_GET_IP_VERSION(pkt_info->pkt_buf_ptr) == 0x4)  
  {
  	IPA_MEMSET(&v4_pseudo_hdr, 0, sizeof(v4_pseudo_hdr));

  	v4_pseudo_hdr.src_addr = *(uint32*)(&pkt_info->pkt_buf_ptr[IPA_TEST_IPV4_SRC_ADDR_OFST]);
  	v4_pseudo_hdr.dest_addr = *(uint32*)(&pkt_info->pkt_buf_ptr[IPA_TEST_IPV4_DEST_ADDR_OFST]);
	  
  
  	v4_pseudo_hdr.protocol = IPA_TEST_TCP_PROT;
  	v4_pseudo_hdr.tcp_length = ipa_ntohs(ipa_htons(*((uint16*)(&pkt_info->pkt_buf_ptr[2])) - IPA_TEST_IPV4_HDR_LEN));

	checksum = ipa_test_calculate_cksum((uint16*) &v4_pseudo_hdr, sizeof(v4_pseudo_hdr));

	*((uint16*)(&pkt_info->pkt_buf_ptr[IPA_TEST_IPV4_HDR_LEN + IPA_TEST_TCP_CKSUM_HDR_OFST])) = checksum;
  }*/
  
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  return ret_code;
}


/*==============================================================================

  FUNCTION:  ipa_test_generate_tcp_ack_packet

==============================================================================*/
/*!
  @brief
  Generate TCP ACK Packet according to the ip_pkt_cfg structure 
  in the specified memory location. 
  V4-->
  TCP_ACK1 -->v4 total len=40, prot= TCP,ACK bit =1
  TCP_ACK2 -->v4 total len=52, prot= TCP,ACK bit =1,TCP hdr len = 32(20 + 12 byte TS options)
  TCP_ACK3 -->v4 total len < 128, prot= TCP,ACK bit =1, TCP hdr len = range (32,60)
 
  V6-->
  TCP_ACK1 -->v6 payload len = 20, prot = TCP,TCP ACK bit=1,TCP hdr len =20
  TCP_ACK2 -->v6 payload len = 32, prot = TCP,TCP ACK bit=1,TCP hdr len = 32 (20 + 12 byte TS options)
  TCP_ACK3 -->v6 payload len <64, prot = TCP,TCP ACK bit=1,TCP hdr len = range (32,60)
 
  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_tcp_ack_packet
(
  ipa_test_engine_ip_pkt_cfg_s  *ip_pkt_cfg_ptr, 
  ipa_test_engine_ip_pkt_info_s *pkt_info
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 prot = 0;
  uint8 tcp_hdr_len = 0;
  uint32 tcp_pkt_type = ip_pkt_cfg_ptr->prot_params.tcp.tcp_ack_pkt_type;
  uint16 payload_len = 0; //V6

  IPA_TF_ASSERT(pkt_info);
  IPA_TF_ASSERT(ip_pkt_cfg_ptr);

  /* TCP protocol*/
  if (ip_pkt_cfg_ptr->prot == IPA_TEST_ENGINE_IP_PKT_PROT_TCP)
  {
    prot = IPA_TEST_TCP_PROT;
  }

  IPA_TF_ASSERT(ip_pkt_cfg_ptr->payload_len + pkt_info->tot_hdr_len <= 
                  IPA_TEST_MAX_PACKET_SIZE);

  /*Generate Random TCP Ack pkt types*/
  if( (tcp_pkt_type == IPA_TEST_TCP_ACK_PKT_INVALID) ||
      (tcp_pkt_type >= IPA_TEST_TCP_ACK_PKT_MAX) )
  {
  tcp_pkt_type = ipa_test_generate_random_num
                 (IPA_TEST_TCP_ACK_PKT_3, IPA_TEST_TCP_ACK_PKT_1);
  }

  if (pkt_info->ip_pkt_version  == IPA_IP_TYPE_IPV4) 
  {
     switch (tcp_pkt_type)
     {
       case IPA_TEST_TCP_ACK_PKT_1:
       {
         tcp_hdr_len = IPA_TEST_TCP_HDR_LEN; /*tcp_hdr_len = 20*/
         pkt_info->pkt_len = pkt_info->ip_hdr_len + tcp_hdr_len;
       }
       break;

       case IPA_TEST_TCP_ACK_PKT_2:
       {
         tcp_hdr_len = IPA_TEST_TCP_HDR_LEN + 12; /*tcp_hdr_len = 20 + 12 byte TS options*/
         pkt_info->pkt_len = pkt_info->ip_hdr_len + tcp_hdr_len;
       }
       break;

       case IPA_TEST_TCP_ACK_PKT_3:
       {
         tcp_hdr_len = ipa_test_generate_random_num(32,60);        /*tcp_hdr_len = range (32,60)*/
         pkt_info->pkt_len = pkt_info->ip_hdr_len + tcp_hdr_len;
       }
       break;
     }
  }
  else if (pkt_info->ip_pkt_version == IPA_IP_TYPE_IPV6)
  {
     switch (tcp_pkt_type)
     {
       case IPA_TEST_TCP_ACK_PKT_1:
       {
         tcp_hdr_len = IPA_TEST_TCP_HDR_LEN; /*tcp_hdr_len = 20*/
         payload_len = tcp_hdr_len;
         pkt_info->pkt_len = pkt_info->ip_hdr_len + payload_len;
       }
       break;

       case IPA_TEST_TCP_ACK_PKT_2:
       {
         tcp_hdr_len = IPA_TEST_TCP_HDR_LEN + 12; /*tcp_hdr_len = 20 + 12 byte TS options*/
         payload_len = tcp_hdr_len;
         pkt_info->pkt_len = pkt_info->ip_hdr_len + payload_len;
       }
       break;

       case IPA_TEST_TCP_ACK_PKT_3:
       {
         tcp_hdr_len = ipa_test_generate_random_num(32,60); /*tcp_hdr_len = range (32,60)*/
         payload_len = tcp_hdr_len;
         pkt_info->pkt_len = pkt_info->ip_hdr_len + payload_len;
       }
       break;
     }
  }

  /* Now allocate a new buffer for the packet */
  pkt_info->pkt_buf_ptr = (uint8 *)IPA_MALLOC(pkt_info->pkt_len);

  /* Clear the packet buffer*/
  IPA_MEMSET(pkt_info->pkt_buf_ptr, 0, pkt_info->pkt_len);

  /* Configure header information*/
  IPA_TEST_SET_IP_VERSION(pkt_info->pkt_buf_ptr, pkt_info->ip_pkt_version);

  /*Configure IPV4 Header Specific info*/
  if (pkt_info->ip_pkt_version  == IPA_IP_TYPE_IPV4) 
  {
    IPA_TEST_SET_IP_PROTOCOL(pkt_info->pkt_buf_ptr, prot);
    IPA_TEST_SET_IP_IHL(pkt_info->pkt_buf_ptr, pkt_info->ip_hdr_len);
    IPA_TEST_SET_IP_IPID(pkt_info->pkt_buf_ptr, ipa_test.ip_pkt.ip_id_cnt++);
    IPA_TEST_SET_IP_PKT_LEN(pkt_info->pkt_buf_ptr, pkt_info->pkt_len);
    IPA_TEST_SET_TCP_ACK_FLAG(pkt_info->pkt_buf_ptr);
    IPA_TEST_SET_TCP_HDR_LEN(pkt_info->pkt_buf_ptr , tcp_hdr_len);
  }
  /* Configure IPV6 specific Info*/
  else
  {
    IPA_TEST_SET_IP_PAYLOAD_LEN(pkt_info->pkt_buf_ptr, payload_len);
    IPA_TEST_SET_IP_NXT_HEADER(pkt_info->pkt_buf_ptr, prot);
    IPA_TEST_SET_V6_TCP_ACK_FLAG(pkt_info->pkt_buf_ptr);
    IPA_TEST_SET_V6_TCP_HDR_LEN(pkt_info->pkt_buf_ptr , tcp_hdr_len);
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_dsm_chain

==============================================================================*/
/*!
  @brief
  Make a dsm chain from the packet buffer provided. 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_dsm_chain
(
  dsm_item_type **tx_dsm_item_ptr,
  uint8         tot_num_dsm_buffers,
  uint8         *pkt_buffer_ptr,
  uint32        pkt_len
)
{
  uint8 i = 0;
  uint32 dsm_item_size = 0;
  dsm_item_type *tmp_dsm_ptr = NULL;
  dsm_item_type *tmp_prev_dsm_ptr = NULL;

  if ((tot_num_dsm_buffers < 1) || 
      (tot_num_dsm_buffers > IPA_TEST_ENGINE_DSM_CHAIN_NUM_MAX) ||
      (pkt_len > (tot_num_dsm_buffers * IPA_CFG_SL_DSM_ITEM_POOL_DATA_SIZE)))
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }

  dsm_item_size = pkt_len / (uint32)tot_num_dsm_buffers;
  if (dsm_item_size == 0)
  {
    /* Number of bytes is lesser than the number of buffers. 
    We'll fill as many buffers as possible */
    dsm_item_size = 1;
  }

  /* Set the global struct values for debugging */
  ipa_test.dsm_chain.num_dsm_items = tot_num_dsm_buffers;
  ipa_test.dsm_chain.size_per_dsm_item = dsm_item_size;

  for (i = 0; (i < tot_num_dsm_buffers) && (pkt_len != 0); i++) 
  {
    ipa_test.dsm_chain.curr_dsm_item_num = i;

    /* The last DSM item may not have the same data remaining as the prev items */
    if (i == (tot_num_dsm_buffers - 1)) 
    {
      dsm_item_size = pkt_len;
    }

    if (dsm_item_size <  IPA_CFG_GEN_DSM_ITEM_POOL_DATA_SIZE_SMALL) 
    {
      tmp_dsm_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);
    }
    else
    {
      tmp_dsm_ptr = dsm_new_buffer(DSM_DS_LARGE_ITEM_POOL);
    }
    IPA_TF_ASSERT(tmp_dsm_ptr != NULL);

    if (i == 0) 
    {
      /* Send the head of the chain to the caller */
      *tx_dsm_item_ptr = tmp_dsm_ptr;
    }
    else
    {
      /* Chain the dsm buffer to the dsm chain*/
      tmp_prev_dsm_ptr->pkt_ptr = tmp_dsm_ptr;
    }
    /* Set the used for this DSM item */
    tmp_dsm_ptr->used = (uint16)dsm_item_size;
    tmp_dsm_ptr->pkt_ptr = NULL;

    IPA_MEMCPY(tmp_dsm_ptr->data_ptr, pkt_buffer_ptr, dsm_item_size);
    pkt_buffer_ptr += dsm_item_size;
    pkt_len -= dsm_item_size;

    tmp_prev_dsm_ptr = tmp_dsm_ptr;
  }

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_random_dsm_chain

==============================================================================*/
/*!
  @brief
  Generate a basic dsm chain from the packet buffer provided with zero length 
  buffers alternatively. For now this is only used for testing DMA task failure when 
  zero length buffers are programmed into IPA in DL.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_random_dsm_chain
(
  dsm_item_type *head_dsm_ptr
)
{
  uint8 empty_pkt_buf = {0};
  dsm_item_type *temp_dsm_ptr = NULL;
  dsm_item_type *empty_dsm_ptr = NULL;
  dsm_item_type *next_valid_pkt_ptr = NULL;
  
  /* Generate empty dsm buffer */
  empty_dsm_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);
  empty_dsm_ptr->used = 0;
  empty_dsm_ptr->pkt_ptr = NULL;
  IPA_MEMCPY(empty_dsm_ptr->data_ptr, &empty_pkt_buf , 0);

  /* Random Chain
     1->2(empty)->3->4(empty)->5->6(empty)->7->8(empty)->9->10(empty) 
  */
  temp_dsm_ptr = head_dsm_ptr;

  while(temp_dsm_ptr)
  {
    /* Generate empty dsm buffer */
    empty_dsm_ptr = dsm_new_buffer(DSM_DS_SMALL_ITEM_POOL);
    empty_dsm_ptr->used = 0;
    empty_dsm_ptr->pkt_ptr = NULL;
    IPA_MEMCPY(empty_dsm_ptr->data_ptr, &empty_pkt_buf , 0);

    /* Store the next valid dsm ptr in the chain i.e 3*/
    next_valid_pkt_ptr = temp_dsm_ptr->pkt_ptr;

    /* Chain the empty dsm ptr now */
    temp_dsm_ptr->pkt_ptr = empty_dsm_ptr;

    /* Link the empty dsm ptr to next valid dsm ptr */
    empty_dsm_ptr->pkt_ptr = next_valid_pkt_ptr; //3

    temp_dsm_ptr = empty_dsm_ptr->pkt_ptr;
  }

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================
                         Mem comparison functions
==============================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_compare_mem

==============================================================================*/
/*!
  @brief
  Compare the two specified memory location, word by word.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_compare_mem
(
  uint16 buf_len,
  uint8  *buf_ptr1, 
  uint8  *buf_ptr2,
  uint16 *no_match_locn
)
{
  uint32 i = 0, j = 0, *word_ptr1 = (uint32 *)buf_ptr1, 
    *word_ptr2 = (uint32 *)buf_ptr2; 

  IPA_TF_ASSERT(buf_len != 0);
  IPA_TF_ASSERT(no_match_locn != NULL);
  IPA_TF_ASSERT((IPA_PAD_NUM_BYTES(buf_ptr1) == 0) &&
                (IPA_PAD_NUM_BYTES(buf_ptr2) == 0));

  /* Compare word by word first */
  while (buf_len >= 4) 
  {
    if (word_ptr1[i] != word_ptr2[i]) 
    {
      /* any of the 4 bytes can mismatch here */
      *no_match_locn = i * sizeof(uint32);
      return IPA_TEST_RET_CODE_FAILURE;
    }
    i++;
    buf_len -= 4;
  }

  /* If remaining, compare the last few bytes */
  if (buf_len != 0) 
  { 
    for (j = 0; j < buf_len; j++) 
    {
      if (buf_ptr1[i * sizeof(uint32) + j] != buf_ptr2[i * sizeof(uint32) + j]) 
      {
         *no_match_locn = i * sizeof(uint32) + j;
         return IPA_TEST_RET_CODE_FAILURE;
      }
    }
  }
  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================
                         Aggregation/Deaggregation functions
==============================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_aggregate_none_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_NONE type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_none_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pushdown_len = 0;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(ul_cfg_ptr != NULL)

  pushdown_len = 
    dsm_pushdown(aggr_frame_dsm_ptr, pkt_arr_ptr[0]->pkt_buf_ptr, 
                 pkt_arr_ptr[0]->pkt_len, 
                 IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(pkt_arr_ptr[0]->pkt_len == pushdown_len);

  (*num_pkts_used) = 1;

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_ethernet_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_ETHERNET type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_ethernet_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pushdown_len = 0;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(ul_cfg_ptr != NULL)

  if ((pkt_arr_ptr[0]->pkt_buf_ptr[0] & 0x40))
  {
    pushdown_len = 
      dsm_pushdown_tail(aggr_frame_dsm_ptr, ul_cfg_ptr->params.ether.ipv4_hdr,
                        IPA_SIO_ETHERNET_HDR_LEN,
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);

    /* Push down this in the mirror dsm ptr as well*/
    pushdown_len = 
      dsm_pushdown(&pkt_arr_ptr[0]->dsm_ptr, ul_cfg_ptr->params.ether.ipv4_hdr,
                   IPA_SIO_ETHERNET_HDR_LEN,
                   IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
  }
  else if ((pkt_arr_ptr[0]->pkt_buf_ptr[0] & 0x60))
  {
    pushdown_len = 
      dsm_pushdown_tail(aggr_frame_dsm_ptr, ul_cfg_ptr->params.ether.ipv6_hdr,
                        IPA_SIO_ETHERNET_HDR_LEN,
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);

    /* Push down this in the mirror dsm ptr as well*/
    pushdown_len = 
      dsm_pushdown(&pkt_arr_ptr[0]->dsm_ptr, ul_cfg_ptr->params.ether.ipv6_hdr,
                   IPA_SIO_ETHERNET_HDR_LEN,
                   IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
  }
  else
  {
    IPA_TF_ASSERT(0);
  }

  /* Now pushdown the packet */
  pushdown_len = 
    dsm_pushdown_tail(aggr_frame_dsm_ptr, pkt_arr_ptr[0]->pkt_buf_ptr, 
                      pkt_arr_ptr[0]->pkt_len, 
                      IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(pkt_arr_ptr[0]->pkt_len == pushdown_len);

  (*num_pkts_used) = 1;

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_qos_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_QOS type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_qos_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pushdown_len = 0;
  uint8 qos_hdr[IPA_TEST_QOS_HDR_LEN];

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(ul_cfg_ptr != NULL)

  ipa_test_generate_random_data(qos_hdr, sizeof(qos_hdr));
  pushdown_len = 
    dsm_pushdown_tail(aggr_frame_dsm_ptr, &qos_hdr,
                      IPA_TEST_QOS_HDR_LEN,
                      IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(IPA_TEST_QOS_HDR_LEN == pushdown_len);

  /* Push down this in the mirror dsm ptr as well*/
  pushdown_len = 
    dsm_pushdown(&pkt_arr_ptr[0]->dsm_ptr, &qos_hdr,
                 IPA_TEST_QOS_HDR_LEN,
                 IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(IPA_TEST_QOS_HDR_LEN == pushdown_len);

  /* Now pushdown the packet */
  pushdown_len = 
    dsm_pushdown_tail(aggr_frame_dsm_ptr, pkt_arr_ptr[0]->pkt_buf_ptr, 
                      pkt_arr_ptr[0]->pkt_len, 
                      IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(pkt_arr_ptr[0]->pkt_len == pushdown_len);

  (*num_pkts_used) = 1;

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_qmap_err_pkt

==============================================================================*/
/*!
  @brief
  Generates errorneous QMAP hdrs based on the test.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_qmap_err_pkt
(
  ipa_test_engine_deaggr_err_cfg_s   * deaggr_cfg,
  ipa_test_engine_deaggr_err_info_s  * deaggr_info,
  ipa_test_engine_ip_pkt_info_s     ** pkt_arr_ptr,
  uint8                              num_pkts_used 
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 frame_cnt, pkt_cnt;

  switch (deaggr_cfg->err_type)
  {

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT:
    {
      /*!   Bad QMAP hdr in the first pkt with rest being valid
            max_aggr_pkts = 3;
            num_pkts = 4;
            all pkts/first pkt = error pkts
            aggr_frame_dsm_ptr --> aggr qmap frame per_sim->IPA
      */
      /* First generate valid QMAP hdr for all pkts.*/
      ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
      ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[num_pkts_used]->mux_id;
      ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[num_pkts_used]->pkt_len +
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);
      ipa_test.agg.qmap_hdr.pad_len = 
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);

      /* Make a copy of this valid hdr for building frames later. We need the
         pkt_len later to dsm_dup frames.
      */
      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = num_pkts_used;
      deaggr_info->qmap_hdr_info[frame_cnt][pkt_cnt] = ipa_test.agg.qmap_hdr; 
      
      /* First pkt of first and second frame has invalid hdr */
      if (num_pkts_used == 0)
      {
         ipa_test.agg.qmap_hdr.pkt_len = 0xFFFC; 
         deaggr_info->err_pkt_gen = TRUE;
      }
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1:
    {
      /*!   Bad QMAP hdr in 2nd pkt of 3 pkt frame
            max_aggr_pkts = 3
            num_pkts = 3
            second_pkt = error_pkt      
            aggr_frame_dsm_ptr --> aggr qmap frame per_sim->IPA
      */

      /* First generate valid QMAP hdr for all pkts.*/
      ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
      ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[num_pkts_used]->mux_id;
      ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[num_pkts_used]->pkt_len +
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);
      ipa_test.agg.qmap_hdr.pad_len = 
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);

      /* Make a copy of this valid hdr for building frames later. We need the
         pkt_len later to dsm_dup frames.
      */
      deaggr_info->qmap_hdr[num_pkts_used] = ipa_test.agg.qmap_hdr;
      
      /* Store first pkt dsm_ptr which is valid to enqueue later.
         Insert bad QMAP hdr for the 2nd pkt.
      */
      if (num_pkts_used == 0)
      {
        deaggr_info->frame_arr[0] = 
           pkt_arr_ptr[num_pkts_used]->dsm_ptr;
      }
      else if (num_pkts_used == 1)
      {       
        ipa_test.agg.qmap_hdr.pkt_len = 0xFFFC; 
        deaggr_info->err_pkt_gen = TRUE;
      }
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE:
    {
      /*! EOT len > Max Msg size(16k) - valid case*/

      /* Fill up the QMAP header params first */
      ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
      ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[num_pkts_used]->mux_id;
      ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[num_pkts_used]->pkt_len +
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);
      ipa_test.agg.qmap_hdr.pad_len = 
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);

      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = num_pkts_used;
      deaggr_info->qmap_hdr_info[frame_cnt][pkt_cnt] = ipa_test.agg.qmap_hdr; 

      /*Since all pkts are valid make a copy of pkt dsm ptr to enqueue as it is*/
      deaggr_info->frame_arr[num_pkts_used] = 
        pkt_arr_ptr[num_pkts_used]->dsm_ptr;

      if (num_pkts_used == 3)
      {
         deaggr_info->err_pkt_gen = TRUE;
      }

    }
    break;
    
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1:
    {
      /*! EOT len > Max Msg size(16k) - 0ed qmap hdr*/

      /* Fill up the QMAP header params first */
      ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
      ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[num_pkts_used]->mux_id;
      ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[num_pkts_used]->pkt_len +
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);
      ipa_test.agg.qmap_hdr.pad_len = 
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);

      /* Make a copy of qmap hdr */
      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = num_pkts_used;
      deaggr_info->qmap_hdr_info[frame_cnt][pkt_cnt] = ipa_test.agg.qmap_hdr; 

      /* Now Memset qmap hdr to 0 for this test */
      IPA_MEMSET(&ipa_test.agg.qmap_hdr,0,sizeof(ipa_hw_qmap_hdr_s));
    }
    break;
   
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES:
    {
      /*! Valid pad_len value in hdr with No padding bytes 
          appended.
      */

      /* Fill up the QMAP header params first */
      ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
      ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[num_pkts_used]->mux_id;
      ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[num_pkts_used]->pkt_len +
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);
      ipa_test.agg.qmap_hdr.pad_len = 
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);

      /* Store QMAP hdr */
      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = num_pkts_used;
      deaggr_info->qmap_hdr_info[frame_cnt][pkt_cnt] = ipa_test.agg.qmap_hdr; 

    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE:
    {
      /*! Truncated QMAP hdr (<4 bytes)*/

      /* Fill up the QMAP header params first */
      ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
      ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[num_pkts_used]->mux_id;
      ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[num_pkts_used]->pkt_len +
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);
      ipa_test.agg.qmap_hdr.pad_len = 
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);

      /* Store QMAP hdr */
      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = num_pkts_used;
      deaggr_info->qmap_hdr_info[frame_cnt][pkt_cnt] = ipa_test.agg.qmap_hdr; 

    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE:
    {
      /*! msg len > max msg size(16k) */

      ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
      ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[num_pkts_used]->mux_id;
      ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[num_pkts_used]->pkt_len +
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);
      ipa_test.agg.qmap_hdr.pad_len = 
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[num_pkts_used]->pkt_len);

      /* Store QMAP hdr */
      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   =  num_pkts_used;
      deaggr_info->qmap_hdr_info[frame_cnt][pkt_cnt] = ipa_test.agg.qmap_hdr; 
    }
    break;

    default:
    break;
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_rndis_err_pkt

==============================================================================*/
/*!
  @brief
  Generates errorneous RNDIS hdr/pkts.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_rndis_err_pkt
(
  ipa_test_engine_deaggr_err_cfg_s   * deaggr_cfg,
  ipa_test_engine_deaggr_err_info_s  * deaggr_info,
  ipa_test_engine_ip_pkt_info_s     ** pkt_arr_ptr,
  uint8                              * num_pkts_used 
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 frame_cnt, pkt_cnt;

  switch (deaggr_cfg->err_type)
  {
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_SPLIT_2_DESC:
    {
      /* create the RNDIS header */
      ipa_test.agg.rndis_hdr.message_type = IPA_TEST_RNDIS_MSG_TYPE;
      ipa_test.agg.rndis_hdr.message_len    = 
         sizeof(ipa_test.agg.rndis_hdr) + IPA_TEST_ETH_HDR_LEN +
         pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
         IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                           IPA_TEST_ETH_HDR_LEN
                          );

      ipa_test.agg.rndis_hdr.data_offset    = IPA_TEST_RNDIS_PAYLOAD_OFFSET;
      ipa_test.agg.rndis_hdr.data_len       = 
         IPA_TEST_ETH_HDR_LEN + pkt_arr_ptr[(*num_pkts_used)]->pkt_len; // Eth pkt length
      ipa_test.agg.rndis_hdr.out_of_band_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_len = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_num_elem = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_len = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.reserved = IPA_TEST_RNDIS_RSVD;
      
      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = (*num_pkts_used);
      deaggr_info->rndis_hdr_info[frame_cnt][pkt_cnt] = ipa_test.agg.rndis_hdr; 

    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_TYPE:
    {
       /*!  Bad Msg type in 2nd RNDIS hdr of 3 pkt frame 
            max_aggr_pkts = 3
            num_pkts = 3
            second_pkt = error_pkt      
            aggr_frame_dsm_ptr --> aggr qmap frame per_sim->IPA
       */


      /* First generate valid RNDIS hdr for all pkts.*/
      ipa_test.agg.rndis_hdr.message_type = IPA_TEST_RNDIS_MSG_TYPE;
      ipa_test.agg.rndis_hdr.message_len    = 
         sizeof(ipa_test.agg.rndis_hdr) + IPA_TEST_ETH_HDR_LEN +
         pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
         IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                           IPA_TEST_ETH_HDR_LEN
                          );

      ipa_test.agg.rndis_hdr.data_offset    = IPA_TEST_RNDIS_PAYLOAD_OFFSET;
      ipa_test.agg.rndis_hdr.data_len       = 
         IPA_TEST_ETH_HDR_LEN + pkt_arr_ptr[(*num_pkts_used)]->pkt_len; 
      ipa_test.agg.rndis_hdr.out_of_band_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_len    = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_num_elem = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_offset  = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_len     = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.reserved             = IPA_TEST_RNDIS_RSVD;

      /* Make a copy of this valid hdr for building frames later. We need the
         pkt_len later to dsm_dup frames.
      */

      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = (*num_pkts_used);
      deaggr_info->rndis_hdr_info[frame_cnt][pkt_cnt] = 
         ipa_test.agg.rndis_hdr; 
      
      if ((*num_pkts_used) == 1)
      {       
        ipa_test.agg.rndis_hdr.message_type = 0xFF;
        deaggr_info->err_pkt_gen = TRUE;

      }
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_OFFSET:
    {
      /*!  Bad data offset field in the 2nd RNDIS hdr of 3 pkt frame 
          (data_offset_field_locn + data_offset_field_value > msg_len )
       
           max_aggr_pkts = 3
           num_pkts = 3
           second_pkt = error_pkt      
           aggr_frame_dsm_ptr --> aggr qmap frame per_sim->IPA
      */

      /* First generate valid RNDIS hdr for all pkts.*/
      ipa_test.agg.rndis_hdr.message_type = IPA_TEST_RNDIS_MSG_TYPE;
      ipa_test.agg.rndis_hdr.message_len    = 
         sizeof(ipa_test.agg.rndis_hdr) + IPA_TEST_ETH_HDR_LEN +
         pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
         IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                           IPA_TEST_ETH_HDR_LEN
                          );

      ipa_test.agg.rndis_hdr.data_offset    = IPA_TEST_RNDIS_PAYLOAD_OFFSET;
      ipa_test.agg.rndis_hdr.data_len       = 
         IPA_TEST_ETH_HDR_LEN + pkt_arr_ptr[(*num_pkts_used)]->pkt_len; 
      ipa_test.agg.rndis_hdr.out_of_band_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_len    = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_num_elem = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_offset  = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_len     = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.reserved             = IPA_TEST_RNDIS_RSVD;

      /* Make a copy of this valid hdr for building frames later. We need the
         pkt_len later to dsm_dup frames.
      */

      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = (*num_pkts_used);
      deaggr_info->rndis_hdr_info[frame_cnt][pkt_cnt] = 
         ipa_test.agg.rndis_hdr; 

      /* Generate error fields for the 2nd RNDIS pkt */
      if ((*num_pkts_used) == 1)
      {       
        ipa_test.agg.rndis_hdr.data_offset = 
           IPA_TEST_RNDIS_PAYLOAD_OFFSET + 5;
        deaggr_info->err_pkt_gen = TRUE;

      }

    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_LEN:
    {
      /*! msg len > EOT len*/

      /* First generate valid RNDIS hdr for all pkts.*/
      ipa_test.agg.rndis_hdr.message_type = IPA_TEST_RNDIS_MSG_TYPE;
      ipa_test.agg.rndis_hdr.message_len    = 
         sizeof(ipa_test.agg.rndis_hdr) + IPA_TEST_ETH_HDR_LEN +
         pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
         IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                           IPA_TEST_ETH_HDR_LEN
                          );

      ipa_test.agg.rndis_hdr.data_offset    = IPA_TEST_RNDIS_PAYLOAD_OFFSET;
      ipa_test.agg.rndis_hdr.data_len       = 
         IPA_TEST_ETH_HDR_LEN + pkt_arr_ptr[(*num_pkts_used)]->pkt_len; 
      ipa_test.agg.rndis_hdr.out_of_band_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_len    = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_num_elem = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_offset  = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_len     = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.reserved             = IPA_TEST_RNDIS_RSVD;

      /* Make a copy of this valid hdr for building frames later. We need the
         pkt_len later to dsm_dup frames.
      */

      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = (*num_pkts_used);
      deaggr_info->rndis_hdr_info[frame_cnt][pkt_cnt] = 
         ipa_test.agg.rndis_hdr; 

      /* Generate error fields for the 2nd RNDIS pkt */
      if ((*num_pkts_used) == 1)
      {       
        ipa_test.agg.rndis_hdr.message_len = 
           4 *( sizeof(ipa_test.agg.rndis_hdr) + IPA_TEST_ETH_HDR_LEN +
                pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                                  IPA_TEST_ETH_HDR_LEN)
               );
        deaggr_info->err_pkt_gen = TRUE;

      }

    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_LEN:
    {
      /*! Invalid Data len: offset + data_len > msg_len*/

      /* First generate valid RNDIS hdr for all pkts.*/
      ipa_test.agg.rndis_hdr.message_type = IPA_TEST_RNDIS_MSG_TYPE;
      ipa_test.agg.rndis_hdr.message_len    = 
         sizeof(ipa_test.agg.rndis_hdr) + IPA_TEST_ETH_HDR_LEN +
         pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
         IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                           IPA_TEST_ETH_HDR_LEN
                          );

      ipa_test.agg.rndis_hdr.data_offset    = IPA_TEST_RNDIS_PAYLOAD_OFFSET;
      ipa_test.agg.rndis_hdr.data_len       = 
         IPA_TEST_ETH_HDR_LEN + pkt_arr_ptr[(*num_pkts_used)]->pkt_len; 
      ipa_test.agg.rndis_hdr.out_of_band_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_len    = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_num_elem = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_offset  = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_len     = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.reserved             = IPA_TEST_RNDIS_RSVD;

      /* Make a copy of this valid hdr for building frames later. We need the
         pkt_len later to dsm_dup frames.
      */

      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = (*num_pkts_used);
      deaggr_info->rndis_hdr_info[frame_cnt][pkt_cnt] = 
         ipa_test.agg.rndis_hdr; 

      /* Generate error fields for the 2nd RNDIS pkt */
      if ((*num_pkts_used) == 2)
      {       
        ipa_test.agg.rndis_hdr.data_len = 
          2 * (IPA_TEST_ETH_HDR_LEN + pkt_arr_ptr[(*num_pkts_used)]->pkt_len);
        deaggr_info->err_pkt_gen = TRUE;

      }
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE:
    {
       /*! RNDIS msg > max_msg_size configured in deaggr.max_pkt_zie */

      /* First generate valid RNDIS hdr for all pkts.*/
      ipa_test.agg.rndis_hdr.message_type = IPA_TEST_RNDIS_MSG_TYPE;
      ipa_test.agg.rndis_hdr.message_len    = 
         sizeof(ipa_test.agg.rndis_hdr) + IPA_TEST_ETH_HDR_LEN +
         pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
         IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                           IPA_TEST_ETH_HDR_LEN
                          );

      ipa_test.agg.rndis_hdr.data_offset    = IPA_TEST_RNDIS_PAYLOAD_OFFSET;
      ipa_test.agg.rndis_hdr.data_len       = 
         IPA_TEST_ETH_HDR_LEN + pkt_arr_ptr[(*num_pkts_used)]->pkt_len; 
      ipa_test.agg.rndis_hdr.out_of_band_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_len    = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_num_elem = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_offset  = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_len     = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.reserved             = IPA_TEST_RNDIS_RSVD;

      /* Make a copy of this valid hdr for building frames later. We need the
         pkt_len later to dsm_dup frames.
      */

      frame_cnt = deaggr_info->frame_cnt;
      pkt_cnt   = (*num_pkts_used);
      deaggr_info->rndis_hdr_info[frame_cnt][pkt_cnt] = 
         ipa_test.agg.rndis_hdr; 
    }
    break;

    default:
    break;
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_mbim_nth_hdr

==============================================================================*/
/*!
  @brief
  Builds MBIM NTH hdr.

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_mbim_nth_hdr
(
  ipa_test_mbim_frame_type_e frame_type,
  uint32                     frame_byte_cnt,
  uint32                     pkt_byte_cnt
)
{
  ipa_test.agg.nth_hdr.sig_nth16 = IPA_TEST_NTH_SIG_VAL;
  ipa_test.agg.nth_hdr.hdr_len = sizeof(ipa_test_nth_hdr_s); //0x0C bytes
  ipa_test.agg.nth_hdr.seq     = ipa_test.agg.curr_mbim_seq_num++;
  ipa_test.agg.nth_hdr.tot_len = frame_byte_cnt;

  if (IPA_TEST_MBIM_PKTS_FIRST == frame_type)
  {
    /*NTH--PKTS--NDP*/
    ipa_test.agg.nth_hdr.idx_ndp = sizeof(ipa_test_nth_hdr_s) +
                                   pkt_byte_cnt;
  }
  else if(IPA_TEST_MBIM_NDP_FIRST == frame_type)
  {
    /* NTH--NDP--PKTS */
    ipa_test.agg.nth_hdr.idx_ndp = sizeof(ipa_test_nth_hdr_s); 
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_mbim_ndp_hdr

==============================================================================*/
/*!
  @brief
  Builds MBIM NDP hdr.

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_mbim_ndp_hdr
(
  ipa_test_mbim_frame_type_e frame_type,
  uint8                      num_ndp,
  uint32                     pkt_byte_cnt,
  uint32                     num_pkts_per_ndp
)
{
  uint8 i = 0;

  for (i = 0 ; i < num_ndp ; i++)
  {
    /*! NDP Sig*/
    ipa_test.agg.ndp_hdr_arr[i].sig_ndp16 = IPA_TEST_NDP_SIG_VAL;

    /*! NDP hdr LEN=base_hdr + dgram_hdr + term_zero_hdr */
    ipa_test.agg.ndp_hdr_arr[i].hdr_len   = 
       sizeof(ipa_test_ndp_hdr_s) + 
       ((num_pkts_per_ndp + 1 ) * sizeof(ipa_test_ndp_hdr_pkt_s));

    ipa_test.agg.curr_ndp_hdr_len += ipa_test.agg.ndp_hdr_arr[i].hdr_len;

    /*! next NDP idx */
    if (i == (num_ndp - 1))
    {
      /* For last ndp idx_nxt = 0 */
      ipa_test.agg.ndp_hdr_arr[i].idx_nxt = 0;
    }
    else
    {
      switch (frame_type)
      {
        case IPA_TEST_MBIM_PKTS_FIRST:
        {
          ipa_test.agg.ndp_hdr_arr[i].idx_nxt = (i == 0)?
             (sizeof(ipa_test_nth_hdr_s) + pkt_byte_cnt + 
             ipa_test.agg.ndp_hdr_arr[i].hdr_len): 
             (ipa_test.agg.ndp_hdr_arr[i-1].idx_nxt + 
             ipa_test.agg.ndp_hdr_arr[i].hdr_len);
        }
        break;

        case IPA_TEST_MBIM_NDP_FIRST:
        {
          ipa_test.agg.ndp_hdr_arr[i].idx_nxt = (i == 0)?
            (sizeof(ipa_test_nth_hdr_s) + ipa_test.agg.ndp_hdr_arr[i].hdr_len):
            (ipa_test.agg.ndp_hdr_arr[i-1].idx_nxt + 
             ipa_test.agg.ndp_hdr_arr[i].hdr_len);
        }
        break;

        default:
        break;
      }
    }
  }/*for total ndps*/
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_mbim_ndp_pkt_hdr

==============================================================================*/
/*!
  @brief
  Builds MBIM NDP dgram hdr.

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_mbim_ndp_pkt_hdr
( 
  ipa_test_engine_ip_pkt_info_s ** pkt_arr_ptr,
  ipa_test_mbim_frame_type_e frame_type,
  uint8 num_pkts
)
{
  uint8 i = 0;

  for (i = 0;i < num_pkts;i++)
  {
    switch (frame_type)
    {
      case IPA_TEST_MBIM_PKTS_FIRST:
      {
        /*dgram idx*/
        ipa_test.agg.pkt_hdr[i].dgram_idx = (i==0)?
         (sizeof(ipa_test_nth_hdr_s)):
         (ipa_test.agg.pkt_hdr[i-1].dgram_idx + 
         ipa_test.agg.pkt_hdr[i-1].dgram_len);

        /*dgram len*/
        ipa_test.agg.pkt_hdr[i].dgram_len = pkt_arr_ptr[i]->pkt_len;
      }
      break;

      case IPA_TEST_MBIM_NDP_FIRST:
      {
        /*dgram idx*/
        ipa_test.agg.pkt_hdr[i].dgram_idx = (i==0)?
         sizeof(ipa_test_nth_hdr_s) + ipa_test.agg.curr_ndp_hdr_len:
         (ipa_test.agg.pkt_hdr[i-1].dgram_idx + 
         ipa_test.agg.pkt_hdr[i-1].dgram_len);

        /*dgram len*/
        ipa_test.agg.pkt_hdr[i].dgram_len = pkt_arr_ptr[i]->pkt_len;
      }
      break;

      default:
      break;
    }
  }

  /* Term zero hdr */
  ipa_test.agg.pkt_hdr[i].dgram_idx = 0;
  ipa_test.agg.pkt_hdr[i].dgram_len = 0;

}

/*==============================================================================

  FUNCTION:  ipa_test_generate_mbim_frame

==============================================================================*/
/*!
  @brief
  Builds the MBIM freame.

  @return
  None
*/
/*============================================================================*/
void ipa_test_generate_mbim_frame
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  ipa_test_mbim_frame_type_e      frame_type,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint32                          num_pkts_per_ndp,
  uint32                          pkt_byte_cnt,
  uint8                           num_ndp,
  uint8                           num_pkts
)
{
  uint16 pushdown_len = 0, i = 0,j = 0, k = 0;
  dsm_item_type *ndp_hdr_dsm_ptr = NULL;
  uint8 pad_buf[IPA_CFG_HW_PAD_ALIGN];

  IPA_MEMSET(pad_buf, 0, sizeof(pad_buf));

  /*! First pushdown pkts and padding*/
  for (i = 0; i < num_pkts; i++)
  {
    pushdown_len = 
      dsm_pushdown_tail(&ipa_test.agg.mbim_ndp_frame, pkt_arr_ptr[i]->pkt_buf_ptr, 
                           pkt_arr_ptr[i]->pkt_len, 
                           IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(pkt_arr_ptr[i]->pkt_len == pushdown_len);

    pushdown_len = 
      dsm_pushdown_tail(&ipa_test.agg.mbim_ndp_frame, &pad_buf,
                           IPA_PAD_NUM_BYTES(pkt_arr_ptr[i]->pkt_len), 
                           IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(IPA_PAD_NUM_BYTES(pkt_arr_ptr[i]->pkt_len) == pushdown_len);
  }

  IPA_TF_ASSERT(dsm_length_packet(ipa_test.agg.mbim_ndp_frame) == pkt_byte_cnt);  

  /*! 2. Pushdown NDP hdr and dgram info hdr into a temp dsm ptr */
  for (i = 0; i < num_ndp; i++)
  {
    pushdown_len = dsm_pushdown_tail(&ndp_hdr_dsm_ptr, &ipa_test.agg.ndp_hdr_arr[i],
                        sizeof(ipa_test_ndp_hdr_s), IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(sizeof(ipa_test_ndp_hdr_s) == pushdown_len);  

    /* Pushdown dgram info for this NDP. j=num_ndps, i=pkt_hdr_arr_idx */
    for (j = 0; j < num_pkts_per_ndp; j++,k++)
    {
      pushdown_len = dsm_pushdown_tail(&ndp_hdr_dsm_ptr, &ipa_test.agg.pkt_hdr[k],
                          sizeof(ipa_test_ndp_hdr_pkt_s), IPA_CFG_SL_DSM_ITEM_POOL); 

      IPA_TF_ASSERT(sizeof(ipa_test_ndp_hdr_pkt_s) == pushdown_len);  
    }

    /* Pushdown terminating zeros*/
    pushdown_len = dsm_pushdown_tail(&ndp_hdr_dsm_ptr, &ipa_test.agg.pkt_hdr[num_pkts],
                          sizeof(ipa_test_ndp_hdr_pkt_s), IPA_CFG_SL_DSM_ITEM_POOL); 
    IPA_TF_ASSERT(sizeof(ipa_test_ndp_hdr_pkt_s) == pushdown_len);  
  }

  IPA_TF_ASSERT(dsm_length_packet(ndp_hdr_dsm_ptr) == ipa_test.agg.curr_ndp_hdr_len);  

  /*! Append hdrs and pkts according the the frame type*/
  switch (frame_type)
  {
    case IPA_TEST_MBIM_PKTS_FIRST:
    {
      /*! Append NDP hdr after the pkts*/
      dsm_append(&ipa_test.agg.mbim_ndp_frame, &ndp_hdr_dsm_ptr);
      dsm_append(aggr_frame_dsm_ptr, &ipa_test.agg.mbim_ndp_frame);
    }
    break;
    case IPA_TEST_MBIM_NDP_FIRST:
    {
      /*! Append pkts after NDP hdr*/
      dsm_append(&ndp_hdr_dsm_ptr,&ipa_test.agg.mbim_ndp_frame);
      dsm_append(aggr_frame_dsm_ptr,&ndp_hdr_dsm_ptr);
    }
    break;

    default:
    break;
  }

  IPA_TF_ASSERT(dsm_length_packet(*aggr_frame_dsm_ptr) == 
                ipa_test.agg.curr_ndp_hdr_len + pkt_byte_cnt);  

  /*! Pushdown NTH hdr*/
  pushdown_len = 
      dsm_pushdown(aggr_frame_dsm_ptr, &ipa_test.agg.nth_hdr, 
                 sizeof(ipa_test_nth_hdr_s), 
                 IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(sizeof(ipa_test_nth_hdr_s) == pushdown_len);
}

/*==============================================================================

  FUNCTION:  ipa_test_init_mbim_frame_info

==============================================================================*/
/*!
  @brief
  Inits MBIM frame variables.

  @return
  None
*/
/*============================================================================*/
void ipa_test_init_mbim_frame_info()
{
  uint8 i = 0;

  ipa_test.agg.curr_mbim_pkt_byte_cnt = 0;
  ipa_test.agg.curr_mbim_ndp_pkts = 0;
  ipa_test.agg.curr_mbim_frame_byte_cnt = 0;
  ipa_test.agg.curr_mbim_seq_num = 0;
  ipa_test.agg.curr_ndp_hdr_len = 0;
  
  for (i = 0;i<IPA_TEST_ENGINE_NDP_NUM_MAX;i++)
  {
    IPA_MEMSET(&ipa_test.agg.ndp_hdr_arr[i],0,sizeof(ipa_test_ndp_hdr_s));
  }

  for (i = 0;i<IPA_TEST_ENGINE_IP_PKT_NUM_MAX;i++)
  {
    IPA_MEMSET(&ipa_test.agg.pkt_hdr[i],0,sizeof(ipa_test_ndp_hdr_pkt_s));
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_generate_mbim_err_pkt

==============================================================================*/
/*!
  @brief
  Generates errorneous MBIM hdr/pkts.

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_generate_mbim_err_pkt
(
  ipa_test_engine_ip_pkt_info_s     ** pkt_arr_ptr,
  uint16                               num_pkts,
  ipa_test_engine_deaggr_err_cfg_s   * deaggr_cfg,
  ipa_test_engine_deaggr_err_info_s  * deaggr_info,
  dsm_item_type                     ** aggr_frame_dsm_ptr,
  uint8                              * num_pkts_used,
  uint32                               aggr_max_byte_limit,
  uint32                               aggr_max_pkt_limit
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 pad_buf[IPA_CFG_HW_PAD_ALIGN];
  uint16 num_ndp = 0, num_pkts_per_ndp = 0, pkt_limit = 0;
  uint32 pkt_byte_cnt = 0, frame_byte_cnt = 0, temp_frame_byte_cnt =0;

  (*num_pkts_used) = 0;

  IPA_MEMSET(pad_buf, 0, sizeof(pad_buf));

  IPA_TF_ASSERT((aggr_max_byte_limit > 0) && (aggr_max_pkt_limit > 0));
  pkt_limit = IPA_MIN(num_pkts, aggr_max_pkt_limit);

  /*! Reset MBIM frame data strucutre variables */
  ipa_test_init_mbim_frame_info();

  /*! Calculate total_byte_cnt and pkt_cnt for this MBIM frame 
      (*num_pkt_used) = no of pkts in the current frame
      frame_byte_cnt  = curre MBIM frame cnt
  */

  /* use 2 ndps for more than 1 pkt */
  num_ndp = (pkt_limit > 1) ? IPA_TEST_ENGINE_NDP_NUM_MAX:
                              IPA_TEST_ENGINE_NDP_NUM_MAX-1;

  /* Init byte_cnt with static MBIM frame part-->
     NTH hdr+2 ndp base hdrs+2 term zero hdrs */
  temp_frame_byte_cnt = sizeof(ipa_test_nth_hdr_s) + 
                        num_ndp * sizeof(ipa_test_ndp_hdr_s) +
                        num_ndp * sizeof(ipa_test_ndp_hdr_pkt_s);
  
  frame_byte_cnt = temp_frame_byte_cnt;

  for ((*num_pkts_used) = 0; (*num_pkts_used) < pkt_limit; (*num_pkts_used)++)
  {
    /* First check if curr pkt can fit in the frame. */
    temp_frame_byte_cnt += 
      pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
      IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len) + 
      sizeof(ipa_test_ndp_hdr_pkt_s);

    if (temp_frame_byte_cnt > aggr_max_byte_limit)
    {
      break;
    }

    /* Current pkt can fit in the frame.Proceed. */
    frame_byte_cnt = temp_frame_byte_cnt;
    pkt_byte_cnt += pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
      IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);      
  }

  /* Store some dbg info */
  ipa_test.agg.curr_mbim_pkt_byte_cnt = pkt_byte_cnt;
  ipa_test.agg.curr_mbim_ndp_pkts = (*num_pkts_used);
  ipa_test.agg.curr_mbim_frame_byte_cnt = frame_byte_cnt;

  if(pkt_limit > 1)
  {
    //TODO:For now we are having even no of pkts. 
    num_pkts_per_ndp = (*num_pkts_used)/2;
  }
  else
  {
    num_pkts_per_ndp = 1;
  }

  /*! Generate NTH hdr */
  ipa_test_generate_mbim_nth_hdr(IPA_TEST_MBIM_PKTS_FIRST,
                                 frame_byte_cnt,pkt_byte_cnt);

  /*! Generate NDP hdrs */
  ipa_test_generate_mbim_ndp_hdr( IPA_TEST_MBIM_PKTS_FIRST, 
                                  num_ndp, pkt_byte_cnt, 
                                  num_pkts_per_ndp);  //TODO

  /*! Generate Dgram hdrs */
  ipa_test_generate_mbim_ndp_pkt_hdr(pkt_arr_ptr,IPA_TEST_MBIM_PKTS_FIRST,
                                     (*num_pkts_used));

  /*! Build erroneous hdrs for deaggr err tests*/
  if (IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != deaggr_cfg->err_type)
  {
    switch (deaggr_cfg->err_type)
    {
       /******! Invalid NTH sig***********/
      case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_SIG:
      {
        ipa_test.agg.nth_hdr.sig_nth16 = 0xFFFFFFFF;
      }
      break;

      /******! Invalid sig in 2nd NDP hdr ************/
      case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_SIG:
      {
        ipa_test.agg.ndp_hdr_arr[num_ndp - 1].sig_ndp16 = 
           0xFFFFFFFF;
      }
      break;

      /****! Out of Range next NDP idx*****/
      case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NXT_NDP_IDX:
      {
        ipa_test.agg.ndp_hdr_arr[0].idx_nxt = 0xAAFF;
      }
      break;

      /****! out of range Dgram idx ****/
      case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_DGRM_IDX:
      {
        ipa_test.agg.pkt_hdr[ipa_test.agg.curr_mbim_ndp_pkts - 1].dgram_idx =
        ipa_test.agg.nth_hdr.tot_len + 5;
      }
      break;

      case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_SEC_EOT_FRAME:
      {
        if (deaggr_info->frame_cnt)
        {
          /* For second frame set nth sig to invalid value */
          ipa_test.agg.nth_hdr.sig_nth16 = 0xFFFFFFFF;
        }
        else
        {
          /*First valid MBIM EOT frame. Have the right mux id in NDP sig */
          ipa_test.agg.ndp_hdr_arr[deaggr_info->frame_cnt].sig_ndp16 |=                                         \
          (((pkt_arr_ptr[deaggr_info->frame_cnt]->mux_id) << 24) & 0xFF000000);           
        }
      }
      break;

      default:
      break;
    }
  }

  /*! Generate MBIM frame */
  ipa_test_generate_mbim_frame(pkt_arr_ptr,IPA_TEST_MBIM_PKTS_FIRST, 
                               aggr_frame_dsm_ptr,num_pkts_per_ndp,
                               pkt_byte_cnt, num_ndp,
                               (*num_pkts_used));

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_qmap_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_QMAP type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_qmap_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  ipa_test_engine_deaggr_err_cfg_s   *deaggr_err_cfg,
  ipa_test_engine_deaggr_err_info_s  *deaggr_err_info,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint16 pushdown_len = 0;
  uint32 byte_cnt = 0, pkt_limit = 0;
  uint8 pad_buf[IPA_CFG_HW_PAD_ALIGN];
  uint32 aggr_max_byte_limit, aggr_max_pkt_limit;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(ul_cfg_ptr != NULL)

  IPA_MEMSET(pad_buf, 0, sizeof(pad_buf));
                      
  aggr_max_byte_limit = ul_cfg_ptr->params.qmap.max_aggr_bytes;
  aggr_max_pkt_limit = ul_cfg_ptr->params.qmap.max_aggr_pkts;
  IPA_TF_ASSERT((aggr_max_byte_limit > 0) && (aggr_max_pkt_limit > 0));

  pkt_limit = IPA_MIN(num_pkts, aggr_max_pkt_limit);

  for ((*num_pkts_used) = 0, byte_cnt = 0; (*num_pkts_used) < pkt_limit; (*num_pkts_used)++)
  {
    byte_cnt += sizeof(ipa_test.agg.qmap_hdr) + 
      pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
      IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);

    if (byte_cnt > aggr_max_byte_limit)
    {
      break;
    }

    /*! If deaggr error tests generate erroneous qmap hdrs.*/
    if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != deaggr_err_cfg->err_type)
    {
      ret_code = ipa_test_generate_qmap_err_pkt(deaggr_err_cfg, 
                                                deaggr_err_info,
                                                pkt_arr_ptr, 
                                                *num_pkts_used);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    }
    else
    {
      /* Fill up the QMAP header params first */
      ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
      ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[(*num_pkts_used)]->mux_id;
      ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);
      ipa_test.agg.qmap_hdr.pad_len = 
        IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);
    }

    /* Swap Endianness as the header is in network byte order */
    *((uint32 *)(&ipa_test.agg.qmap_hdr)) = 
      ipa_ntohl((uint32)(*((uint32 *)(&ipa_test.agg.qmap_hdr))));

    /* Now pushdown the header */
    pushdown_len = 
      dsm_pushdown_tail(aggr_frame_dsm_ptr, &ipa_test.agg.qmap_hdr, 
                        sizeof(ipa_test.agg.qmap_hdr), 
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(sizeof(ipa_test.agg.qmap_hdr) == pushdown_len);

    /* Now pushdown the pkt */
    pushdown_len = 
      dsm_pushdown_tail(aggr_frame_dsm_ptr, pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr, 
                        pkt_arr_ptr[(*num_pkts_used)]->pkt_len, 
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)]->pkt_len == pushdown_len);

    /* Now pushdown the padding */
    if ( deaggr_err_cfg->err_type != IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES )
    {
      pushdown_len = 
        dsm_pushdown_tail(aggr_frame_dsm_ptr, &pad_buf, 
                        IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len), 
                        IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len) == pushdown_len);
    }
    else
    {
      /*! IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES test */

      /* Store the pkt index for which padding was not added */
      if ((IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len) > 0) &&
           !(deaggr_err_info->err_pkt_gen) && ((*num_pkts_used) == 1)
         )
      {
        /* Store only the first pkt in frame with no padding bytes */
        deaggr_err_info->pkt_no_pad_index = (*num_pkts_used);
        deaggr_err_info->err_pkt_gen = TRUE;
      }
      else
      {
        /* If error pkt is already generated with no padding bytes create 
           valid padding for remaining pkts.
        */ 
        pushdown_len = 
        dsm_pushdown_tail(aggr_frame_dsm_ptr, &pad_buf, 
                        IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len), 
                        IPA_CFG_SL_DSM_ITEM_POOL);
        IPA_TF_ASSERT(IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len) == pushdown_len);
      }
    }

    /* Update the number of pkts used */
  }

  /* Validate frame for deaggr err scenario tests. */
  
  if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != deaggr_err_cfg->err_type)
  {

    switch (deaggr_err_cfg->err_type)
    {
      case  IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1:
      {
        ipa_hw_qmap_hdr_s       qmap_hdr_test;
        uint32 pull_up_len = dsm_extract(*aggr_frame_dsm_ptr,0,
                                        (uint32 *)&qmap_hdr_test,sizeof(ipa_hw_qmap_hdr_s));

        *((uint32 *)(&qmap_hdr_test)) = 
         ipa_ntohl((uint32)(*((uint32 *)(&qmap_hdr_test))));

        IPA_TF_ASSERT(pull_up_len == sizeof(ipa_hw_qmap_hdr_s));
        IPA_TF_ASSERT(qmap_hdr_test.pkt_len == deaggr_err_info->qmap_hdr[0].pkt_len);
      }
      break;

      default:
      break;
    }
    
  }
  
  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_qmap_v3_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_QMAP_V2 type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_qmap_v3_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  uint8                         is_clat,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pushdown_len = 0;
  uint32 byte_cnt = 0, pkt_limit = 0;
  uint8 pad_buf[IPA_CFG_HW_PAD_ALIGN];
  uint32 aggr_max_byte_limit, aggr_max_pkt_limit; 
  uint16 cksum_val = 0, *cksum_hdr_ptr = NULL;
  //uint16* checksum_pointer = pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr;
  uint16 pseudo_hdr_chksum = 0, payload_chksum = 0;
  uint16 checksum_buffer[2];

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(ul_cfg_ptr != NULL)

  IPA_MEMSET(pad_buf, 0, sizeof(pad_buf));
                      
  aggr_max_byte_limit = ul_cfg_ptr->params.qmap.max_aggr_bytes;
  aggr_max_pkt_limit = ul_cfg_ptr->params.qmap.max_aggr_pkts;
  IPA_TF_ASSERT((aggr_max_byte_limit > 0) && (aggr_max_pkt_limit > 0));

  pkt_limit = IPA_MIN(num_pkts, aggr_max_pkt_limit);

  //test_num_pkts_total_1 = pkt_limit;
  for ((*num_pkts_used) = 0, byte_cnt = 0; (*num_pkts_used) < pkt_limit; (*num_pkts_used)++)
  {
    byte_cnt += sizeof(ipa_test.agg.qmap_hdr) + 
      sizeof(ipa_test.agg.cksum_hdr) + 
      pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
      IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);


    /* We need to ensure that both QMAP & Cksum header is in 1 buffer. 
    So stop aggregation if this cannot be achieved */
    if ((byte_cnt > aggr_max_byte_limit) ||
        ((IPA_CFG_SL_DSM_ITEM_POOL_DATA_SIZE - (dsm_length_packet(*aggr_frame_dsm_ptr) % IPA_CFG_SL_DSM_ITEM_POOL_DATA_SIZE)) <
         (sizeof(ipa_hw_qmap_hdr_s) + sizeof(ipa_hw_cksum_hdr_s))))
    {
      break;
    }

    /* Fill up the QMAP header params first */
    ipa_test.agg.qmap_hdr.hdr_type = IPA_HW_QMAP_HDR_TYPE_DATA;
    ipa_test.agg.qmap_hdr.mux_id = pkt_arr_ptr[(*num_pkts_used)]->mux_id;
    ipa_test.agg.qmap_hdr.pkt_len = pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
      IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);
    ipa_test.agg.qmap_hdr.pad_len = 
      IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);

    /* Swap Endianness as the header is in network byte order */
    *((uint32 *)(&ipa_test.agg.qmap_hdr)) = 
      ipa_ntohl((uint32)(*((uint32 *)(&ipa_test.agg.qmap_hdr))));

    /* Now pushdown the header */
    pushdown_len = 
      dsm_pushdown_tail(aggr_frame_dsm_ptr, &ipa_test.agg.qmap_hdr, 
                        sizeof(ipa_test.agg.qmap_hdr), 
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(sizeof(ipa_test.agg.qmap_hdr) == pushdown_len);

    /* Fill up the CKSUM header params */
    if (IPA_TEST_GET_IP_VERSION(pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr) == 6)
    {
      if (IPA_TEST_GET_IP_PROTOCOL(pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr) 
          == IPA_TEST_TCP_PROT)
      {
        ipa_test.agg.cksum_hdr.enable = TRUE;
        ipa_test.agg.cksum_hdr.start_offset = IPA_TEST_IPV6_HDR_LEN;
        ipa_test.agg.cksum_hdr.insert_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
        ipa_test.agg.cksum_hdr.udp_ip_ind = FALSE;
      }
      else if (IPA_TEST_GET_IP_PROTOCOL(pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr) 
               == IPA_TEST_UDP_PROT)
      {
        ipa_test.agg.cksum_hdr.enable = TRUE;
        ipa_test.agg.cksum_hdr.start_offset = IPA_TEST_IPV6_HDR_LEN;
        ipa_test.agg.cksum_hdr.insert_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
        ipa_test.agg.cksum_hdr.udp_ip_ind = TRUE;
      }
      else
      {
        ipa_test.agg.cksum_hdr.enable = FALSE;
      }
    }
    else if (IPA_TEST_GET_IP_VERSION(pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr) == 4)
    {
      if (IPA_TEST_GET_IP_PROTOCOL(pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr) 
          == IPA_TEST_TCP_PROT)
      {
        ipa_test.agg.cksum_hdr.enable = TRUE;
        ipa_test.agg.cksum_hdr.start_offset = IPA_TEST_IPV4_HDR_LEN;
        ipa_test.agg.cksum_hdr.insert_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
        ipa_test.agg.cksum_hdr.udp_ip_ind = FALSE;
      }
      else if (IPA_TEST_GET_IP_PROTOCOL(pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr) 
               == IPA_TEST_UDP_PROT)
      {
        ipa_test.agg.cksum_hdr.enable = TRUE;
        ipa_test.agg.cksum_hdr.start_offset = IPA_TEST_IPV4_HDR_LEN;
        ipa_test.agg.cksum_hdr.insert_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
        ipa_test.agg.cksum_hdr.udp_ip_ind = TRUE;
      }
      else
      {
        ipa_test.agg.cksum_hdr.enable = FALSE;
      }
    }
    else
    {
      IPA_TF_ASSERT(0);
    }

    if (ipa_test.agg.cksum_hdr.enable == TRUE)
    {
      pseudo_hdr_chksum = ipa_test_calculate_pseudo_hdr_cksum(pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr);
	  //pseudo_hdr_chksum = ~pseudo_hdr_chksum & 0xFFFF;
	  
      cksum_hdr_ptr = (uint16 *)&pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr \
        [ipa_test.agg.cksum_hdr.insert_offset + ipa_test.agg.cksum_hdr.start_offset];
      *cksum_hdr_ptr = 0;

      /* Let's the find the cksum value of the buffer so that we can update it in
      the mirror wm */
      payload_chksum = 
        ipa_test_calculate_cksum((uint16 *)&pkt_arr_ptr[(*num_pkts_used)]-> \
                                 pkt_buf_ptr[ipa_test.agg.cksum_hdr.start_offset],
                                 pkt_arr_ptr[(*num_pkts_used)]->pkt_len - 
                                 ipa_test.agg.cksum_hdr.start_offset);

      payload_chksum = ~payload_chksum & 0xFFFF;
  
	  checksum_buffer[0] = pseudo_hdr_chksum;
	  checksum_buffer[1] = payload_chksum;
	  cksum_val = ipa_test_calculate_cksum((uint16 *)&checksum_buffer, sizeof(checksum_buffer));

      if (cksum_val == 0)
      {
        if (ipa_test.agg.cksum_hdr.udp_ip_ind == TRUE)
        {
          cksum_val = ~cksum_val;
        }
      }

      if(ul_cfg_ptr->hdr_type == IPA_SIO_HDR_TYPE_QMAP_V3)
      {
	    *cksum_hdr_ptr = pseudo_hdr_chksum;
      }
	  else
	  {
	    *cksum_hdr_ptr = ~pseudo_hdr_chksum & 0xFFFF;
	  }
	 
	  
      pushdown_len =
         dsm_insert(&pkt_arr_ptr[(*num_pkts_used)]->dsm_ptr, 
                    IPA_CFG_SL_DSM_ITEM_POOL,
                    ipa_test.agg.cksum_hdr.start_offset + 
                    ipa_test.agg.cksum_hdr.insert_offset, (uint8 *)&cksum_val,
                    sizeof(cksum_val));
	  
      IPA_TF_ASSERT(pushdown_len == sizeof(cksum_val));
      pkt_arr_ptr[(*num_pkts_used)]->is_qmap_v3 = TRUE;
      pkt_arr_ptr[(*num_pkts_used)]->checksum_val = cksum_val;
      
    }

    /* Swap Endianness as the header is in network byte order */
    *((uint32 *)(&ipa_test.agg.cksum_hdr)) = 
      ipa_ntohl((uint32)(*((uint32 *)(&ipa_test.agg.cksum_hdr))));

    /* Now pushdown the header */
    pushdown_len = 
      dsm_pushdown_tail(aggr_frame_dsm_ptr, &ipa_test.agg.cksum_hdr, sizeof(ipa_test.agg.cksum_hdr), 
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(sizeof(ipa_test.agg.cksum_hdr) == pushdown_len);

    /* Now pushdown the pkt */
    pushdown_len = 
      dsm_pushdown_tail(aggr_frame_dsm_ptr, pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr, 
                        pkt_arr_ptr[(*num_pkts_used)]->pkt_len, 
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)]->pkt_len == pushdown_len);

    /* Now pushdown the padding */
    pushdown_len = 
      dsm_pushdown_tail(aggr_frame_dsm_ptr, &pad_buf, 
                        IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len), 
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len) == pushdown_len);

    /* Update the number of pkts used */
  }

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_mbim_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_MBIM type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_mbim_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  ipa_test_engine_deaggr_err_cfg_s   *deaggr_err_cfg,
  ipa_test_engine_deaggr_err_info_s  *deaggr_err_info,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint16 pushdown_len = 0;
  uint32 byte_cnt = 0, tmp_byte_cnt = 0, pkt_limit = 0, pkt_idx = 0;
  uint8 pad_buf[IPA_CFG_HW_PAD_ALIGN];
  uint32 aggr_max_byte_limit, aggr_max_pkt_limit;
  uint8 old_mux_id = 0;
  uint16 curr_ndp_num_pkts = 0, num_ndp_frames = 0;
  dsm_item_type *tmp_ndp_dsm_ptr = NULL;
  uint16 i = 0;
  uint32 zeros = 0;
  uint8 *tmp_buf = NULL;

/*! @todo Need to re-enable this for MOB later. There is some compilation failure */
#ifndef FEATURE_IPA_TEST_UT
#define TERMINATE_PREV_NDP_FRAME(nxt_ndp_idx)                                 \
  do                                                                          \
  {                                                                           \
    ipa_test.agg.ndp_hdr.hdr_len = sizeof(ipa_test_ndp_hdr_s) +               \
      sizeof(ipa_test_ndp_hdr_pkt_s) * (curr_ndp_num_pkts + 1);               \
    ipa_test.agg.ndp_hdr.idx_nxt = nxt_ndp_idx;                               \
    ipa_test.agg.ndp_hdr.sig_ndp16 |=                                         \
      (((old_mux_id) << 24) & 0xFF000000);                                    \
                                                                              \
    tmp_ndp_dsm_ptr = NULL;                                                   \
                                                                              \
    /* Now pushdown the hdr to the curr NDP frame */                          \
    pushdown_len =                                                            \
      dsm_pushdown_tail(&tmp_ndp_dsm_ptr, &ipa_test.agg.ndp_hdr,              \
                        sizeof(ipa_test_ndp_hdr_s), IPA_CFG_SL_DSM_ITEM_POOL);\
    IPA_TF_ASSERT(sizeof(ipa_test_ndp_hdr_s) == pushdown_len);                \
                                                                              \
    for (i = 0; i < curr_ndp_num_pkts; i++)                                   \
    {                                                                         \
      /* Adjust the pkt datagram index */                                     \
      ipa_test.agg.pkt_hdr[i].dgram_idx += (curr_ndp_num_pkts - i - 1) *      \
        sizeof(ipa_test_ndp_hdr_pkt_s);                                       \
      pushdown_len =                                                          \
        dsm_pushdown_tail(&tmp_ndp_dsm_ptr, &ipa_test.agg.pkt_hdr[i],         \
                          sizeof(ipa_test_ndp_hdr_pkt_s),                     \
                          IPA_CFG_SL_DSM_ITEM_POOL);                         \
      IPA_TF_ASSERT(sizeof(ipa_test_ndp_hdr_pkt_s) == pushdown_len);          \
    }                                                                         \
                                                                              \
    pushdown_len =                                                            \
      dsm_pushdown_tail(&tmp_ndp_dsm_ptr, &zeros,                             \
                        sizeof(zeros), IPA_CFG_SL_DSM_ITEM_POOL);            \
    IPA_TF_ASSERT(sizeof(zeros) == pushdown_len);                             \
                                                                              \
    /* Now append the pkts to the NDP header */                               \
    dsm_append(&tmp_ndp_dsm_ptr, &ipa_test.agg.mbim_ndp_frame);               \
                                                                              \
    /* Now append the entire NDP frame to the aggr frame */                   \
    dsm_append(aggr_frame_dsm_ptr, &tmp_ndp_dsm_ptr);                         \
                                                                              \
    curr_ndp_num_pkts = 0;                                                    \
    num_ndp_frames++;                                                         \
  } while (0)
#else
#define TERMINATE_PREV_NDP_FRAME(nxt_ndp_idx)
#endif

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL);
  IPA_TF_ASSERT(ul_cfg_ptr != NULL);

  IPA_MEMSET(pad_buf, 0, sizeof(pad_buf));
                      
  aggr_max_byte_limit = ul_cfg_ptr->params.mbim.max_aggr_bytes;
  aggr_max_pkt_limit = ul_cfg_ptr->params.mbim.max_aggr_pkts;
  IPA_TF_ASSERT((aggr_max_byte_limit > 0) && (aggr_max_pkt_limit > 0));

  /*! @todo Because we're in BAM->system mode we have to make sure that the entire frame is 
  in 1 buffer. i.e 1536 bytes  */
  //aggr_max_byte_limit = 1536;

  /*! If deaggr error tests generate erroneous mbim pkts and return */
  if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != deaggr_err_cfg->err_type)
  {
    /* Deaggr err tests are run in B2B mode. So no limit on the aggr_byte_limit */
    ret_code = ipa_test_generate_mbim_err_pkt(pkt_arr_ptr,
                                              num_pkts,
                                              deaggr_err_cfg, 
                                              deaggr_err_info,
                                              aggr_frame_dsm_ptr,
                                              num_pkts_used,
                                              aggr_max_byte_limit,
                                              aggr_max_pkt_limit);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    return ret_code;
  }
  else
  {
    /*! @todo Because we're in BAM->system mode we have to make sure that the entire frame is 
    in 1 buffer. i.e 1536 bytes  */
    aggr_max_byte_limit = 1536;
  }

  /* Set some of the hdr params */
  ipa_test.agg.nth_hdr.sig_nth16 = IPA_TEST_NTH_SIG_VAL;
  ipa_test.agg.nth_hdr.hdr_len = sizeof(ipa_test_nth_hdr_s);
  ipa_test.agg.nth_hdr.idx_ndp = sizeof(ipa_test_nth_hdr_s);
  ipa_test.agg.nth_hdr.seq = ipa_test.agg.curr_mbim_seq_num++;

  ipa_test.agg.ndp_hdr.sig_ndp16 = IPA_TEST_NDP_SIG_VAL;

  pkt_limit = IPA_MIN(num_pkts, aggr_max_pkt_limit);

  byte_cnt = sizeof(ipa_test_nth_hdr_s);
  (*num_pkts_used) = 0 ;

  while(1)
  {
    if ((*num_pkts_used) >= pkt_limit)
    {
      if (curr_ndp_num_pkts > 0)
      {
        /* Terminate previous NDP frame */
        TERMINATE_PREV_NDP_FRAME(0);
      }
      break;
    }
    else
    {
      if (old_mux_id != pkt_arr_ptr[(*num_pkts_used)]->mux_id)
      {
        tmp_byte_cnt = byte_cnt + sizeof(ipa_test_ndp_hdr_s) + 
          sizeof(ipa_test_ndp_hdr_pkt_s) * 2 + 
          pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
          IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);
      }
      else
      {
        tmp_byte_cnt = byte_cnt + sizeof(ipa_test_ndp_hdr_pkt_s) + 
          pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
          IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);
      }
    }

    if (tmp_byte_cnt > aggr_max_byte_limit)
    {
      if (curr_ndp_num_pkts > 0)
      {
        /* Terminate previous NDP frame */
        TERMINATE_PREV_NDP_FRAME(0);
      }
      break;
    }
    else
    {
      if (old_mux_id != pkt_arr_ptr[(*num_pkts_used)]->mux_id)
      {
        /* We need to termintate the previous NDP frame */
        if (old_mux_id != 0)
        {
          if (curr_ndp_num_pkts > 0)
          {
            /* Terminate previous NDP frame */
            TERMINATE_PREV_NDP_FRAME(byte_cnt);
          }
        }
        /* we need a new NDP header */
        byte_cnt += sizeof(ipa_test_ndp_hdr_s) + sizeof(ipa_test_ndp_hdr_pkt_s);
        old_mux_id = pkt_arr_ptr[(*num_pkts_used)]->mux_id;
      }
    }

    pkt_idx = byte_cnt + sizeof(ipa_test_ndp_hdr_pkt_s);
    byte_cnt += sizeof(ipa_test_ndp_hdr_pkt_s) + 
      pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
      IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);

    /* Update the pkt's NDP header */
    ipa_test.agg.pkt_hdr[curr_ndp_num_pkts].dgram_idx = pkt_idx;
    ipa_test.agg.pkt_hdr[curr_ndp_num_pkts].dgram_len = 
      pkt_arr_ptr[(*num_pkts_used)]->pkt_len;

    /* Now pushdown the pkt at the tail */
    pushdown_len = 
      dsm_pushdown_tail(&ipa_test.agg.mbim_ndp_frame, pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr, 
                        pkt_arr_ptr[(*num_pkts_used)]->pkt_len, 
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)]->pkt_len == pushdown_len);

    /* Now pushdown the padding */
    pushdown_len = 
      dsm_pushdown_tail(&ipa_test.agg.mbim_ndp_frame, &pad_buf, 
                        IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len), 
                        IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len) == pushdown_len);

    /* Update the number of pkts in the NDP header */
    curr_ndp_num_pkts++;
    (*num_pkts_used)++;
  }

  /* Now add the NTH header */
  ipa_test.agg.nth_hdr.tot_len = byte_cnt;
  pushdown_len = 
    dsm_pushdown(aggr_frame_dsm_ptr, &ipa_test.agg.nth_hdr, 
                 sizeof(ipa_test_nth_hdr_s), 
                 IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(sizeof(ipa_test_nth_hdr_s) == pushdown_len);

  /* remove this */
  IPA_TF_ASSERT(byte_cnt == dsm_length_packet(*aggr_frame_dsm_ptr));
  tmp_buf = (uint8 *)IPA_MALLOC(byte_cnt);
  IPA_TF_ASSERT(tmp_buf != NULL);
  pushdown_len = dsm_pullup(aggr_frame_dsm_ptr, tmp_buf, byte_cnt);
  IPA_TF_ASSERT(pushdown_len == byte_cnt);

  pushdown_len = 
    dsm_pushdown(aggr_frame_dsm_ptr, tmp_buf, 
                 byte_cnt, IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(pushdown_len == byte_cnt);

  IPA_FREE(tmp_buf);

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_aggregate_rndis_endp_pkt

==============================================================================*/
/*!
  @brief
  Aggregates IPA_SIO_HDR_TYPE_RNDIS type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_aggregate_rndis_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  ipa_test_engine_deaggr_err_cfg_s   *deaggr_err_cfg,
  ipa_test_engine_deaggr_err_info_s  *deaggr_err_info,
  uint16                        num_pkts,
  ipa_sio_ul_config_s           *ul_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint16 pushdown_len = 0;
  uint16 aggr_max_byte_limit, aggr_max_pkt_limit;
  uint8 pad_buf[IPA_CFG_HW_PAD_ALIGN];
  uint32 byte_cnt = 0, pkt_limit = 0;
  
  ipa_test_eth_hdr_s eth_hdr;
  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(ul_cfg_ptr != NULL)

  IPA_MEMSET(pad_buf, 0, sizeof(pad_buf));

  aggr_max_byte_limit = ul_cfg_ptr->params.qmap.max_aggr_bytes;
  aggr_max_pkt_limit = ul_cfg_ptr->params.qmap.max_aggr_pkts;
  IPA_TF_ASSERT((aggr_max_byte_limit > 0) && (aggr_max_pkt_limit > 0));

  pkt_limit = IPA_MIN(num_pkts, aggr_max_pkt_limit);

  for ((*num_pkts_used) = 0, byte_cnt = 0; (*num_pkts_used) < pkt_limit; (*num_pkts_used)++)
  {
    byte_cnt += sizeof(ipa_test.agg.rndis_hdr) + 
                pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);

    if (byte_cnt > aggr_max_byte_limit)
    {
      break;
    }

    /*! Generate RNDIS header.*/

    if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != deaggr_err_cfg->err_type)
    {
      /* If RNDIS deaggr error tests then generate erroneous rndis hdr. */
      ret_code = ipa_test_generate_rndis_err_pkt(deaggr_err_cfg, 
                                                 deaggr_err_info,
                                                 pkt_arr_ptr, 
                                                 num_pkts_used);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    }
    else
    {
      /* create the RNDIS header */
      ipa_test.agg.rndis_hdr.message_type = IPA_TEST_RNDIS_MSG_TYPE;
      ipa_test.agg.rndis_hdr.message_len    = 
                    sizeof(ipa_test.agg.rndis_hdr) + 
                    IPA_TEST_ETH_HDR_LEN +
                    pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                    IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len);
      ipa_test.agg.rndis_hdr.data_offset    = IPA_TEST_RNDIS_PAYLOAD_OFFSET;
      ipa_test.agg.rndis_hdr.data_len       = 
          IPA_TEST_ETH_HDR_LEN +
          pkt_arr_ptr[(*num_pkts_used)]->pkt_len; // Eth pkt length
      ipa_test.agg.rndis_hdr.out_of_band_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_len = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.out_of_band_num_elem = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_offset = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.per_pkt_info_len = IPA_TEST_RNDIS_NULL_DATA;
      ipa_test.agg.rndis_hdr.reserved = IPA_TEST_RNDIS_RSVD;
    }

    /*! 
         Pushdown hdrs to create RNDIS pkt.
         --------------------------------------
         |RNDIS hdr|Ethernet hdr|IP Pkt|Padding|
         --------------------------------------
         1. Pushdown RNDIS hdr
         2. Pushdown Eth hdr
         3. Pushdown pkt
         4. Pushdown Padding
    */

    /* 1. pushdown the RNDIS Header to the beginning of the frame*/
  pushdown_len = 
        dsm_pushdown_tail(aggr_frame_dsm_ptr, &ipa_test.agg.rndis_hdr,
                     sizeof(ipa_test.agg.rndis_hdr),
                       IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(sizeof(ipa_test.agg.rndis_hdr) == pushdown_len);
  
    /* 2. Pushdown Ethernet hdr. Also push ethernet hdr in the mirror wmk IP 
          pkts as well */
    IPA_MEMSET(&eth_hdr, 0, sizeof(ipa_test_eth_hdr_s));
    IPA_MEMSET(eth_hdr.dst_mac_addr, 0xAA, IPA_TEST_ETH_MAC_ADDR_LEN);
    IPA_MEMSET(eth_hdr.src_mac_addr, 0x11, IPA_TEST_ETH_MAC_ADDR_LEN);

    if ((pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr[0] & 0x40))
    {

      eth_hdr.ether_type = (uint16) IPA_TEST_ETHER_TYPE_IPV4;
      pushdown_len = 
         dsm_pushdown_tail(aggr_frame_dsm_ptr, &eth_hdr,
                        IPA_SIO_ETHERNET_HDR_LEN,
                       IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);     

    }
    else if ((pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr[0] & 0x60))
    {
      eth_hdr.ether_type = (uint16) IPA_TEST_ETHER_TYPE_IPV6;
      pushdown_len = 
         dsm_pushdown_tail(aggr_frame_dsm_ptr, &eth_hdr,
                        IPA_SIO_ETHERNET_HDR_LEN,
                        IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
    }

    /* 3. Pushdown packet */
    pushdown_len = 
        dsm_pushdown_tail(aggr_frame_dsm_ptr, 
                          pkt_arr_ptr[(*num_pkts_used)]->pkt_buf_ptr,
                          pkt_arr_ptr[(*num_pkts_used)]->pkt_len,
                          IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)]->pkt_len == pushdown_len);

    /* 4. pushdown padding bytes calculated over Eth+IP packet. */
    pushdown_len = 
        dsm_pushdown_tail(aggr_frame_dsm_ptr, 
                          &pad_buf, 
                          IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len +
                                            IPA_TEST_ETH_HDR_LEN
                                            ), 
                  IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(IPA_PAD_NUM_BYTES(pkt_arr_ptr[(*num_pkts_used)]->pkt_len
                                    + IPA_TEST_ETH_HDR_LEN) == pushdown_len);


  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_none_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_NONE type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_none_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pullup_len = 0;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(dl_cfg_ptr != NULL)

  pkt_arr_ptr[0].pkt_len = dsm_length_packet(*aggr_frame_dsm_ptr);
  pkt_arr_ptr[0].pkt_buf_ptr = (uint8 *)IPA_MALLOC(pkt_arr_ptr[0].pkt_len);
  IPA_TF_ASSERT(pkt_arr_ptr[0].pkt_buf_ptr != NULL);

  pullup_len = 
    dsm_pullup(aggr_frame_dsm_ptr, pkt_arr_ptr[0].pkt_buf_ptr, 
               pkt_arr_ptr[0].pkt_len);
  IPA_TF_ASSERT(pkt_arr_ptr[0].pkt_len == pullup_len);

  pkt_arr_ptr[0].mux_id = 0;
  (*num_pkts_used) = 1;

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_ethernet_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_ETHERNET type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_ethernet_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pullup_len = 0;
  ipa_test_ret_code_e ret_code;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(dl_cfg_ptr != NULL)

  /* Check if the ethernet hdr was configured because we cannot pull up the 
  ethernet header without this being TRUE */
  IPA_TF_ASSERT((dl_cfg_ptr->params.ether.ipv4_hdr_present == TRUE) &&
                (dl_cfg_ptr->params.ether.ipv6_hdr_present == TRUE));

  /* Pull up the ethernet hdr first */
  pullup_len = 
    dsm_pullup(aggr_frame_dsm_ptr, &ipa_test.agg.rx_eth_hdr, 
               IPA_SIO_ETHERNET_HDR_LEN);
  IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pullup_len);

  pkt_arr_ptr[0].pkt_len = dsm_length_packet(*aggr_frame_dsm_ptr);
  pkt_arr_ptr[0].pkt_buf_ptr = (uint8 *)IPA_MALLOC(pkt_arr_ptr[0].pkt_len);
  IPA_TF_ASSERT(pkt_arr_ptr[0].pkt_buf_ptr != NULL);

  pullup_len = 
    dsm_pullup(aggr_frame_dsm_ptr, pkt_arr_ptr[0].pkt_buf_ptr, 
               pkt_arr_ptr[0].pkt_len);
  IPA_TF_ASSERT(pkt_arr_ptr[0].pkt_len == pullup_len);

  pkt_arr_ptr[0].mux_id = 0;
  (*num_pkts_used) = 1;

  ipa_test.agg.no_match_locn = 0;
  if (IPA_TEST_GET_IP_VERSION(pkt_arr_ptr[0].pkt_buf_ptr) == 4)
  {
    IPA_MEMCPY(ipa_test.agg.tx_eth_hdr, dl_cfg_ptr->params.ether.ipv4_hdr,
               IPA_SIO_ETHERNET_HDR_LEN);
    ret_code = 
      ipa_test_compare_mem(IPA_SIO_ETHERNET_HDR_LEN, 
                           ipa_test.agg.tx_eth_hdr,
                           ipa_test.agg.rx_eth_hdr, &ipa_test.agg.no_match_locn);
  }
  else if (IPA_TEST_GET_IP_VERSION(pkt_arr_ptr[0].pkt_buf_ptr) == 6)
  {
    IPA_MEMCPY(ipa_test.agg.tx_eth_hdr, dl_cfg_ptr->params.ether.ipv6_hdr,
               IPA_SIO_ETHERNET_HDR_LEN);
    ret_code = 
      ipa_test_compare_mem(IPA_SIO_ETHERNET_HDR_LEN, 
                           ipa_test.agg.tx_eth_hdr,
                           ipa_test.agg.rx_eth_hdr, &ipa_test.agg.no_match_locn);
  }
  else
  {
    IPA_TF_ASSERT(0);
  }

  if (ret_code != IPA_TEST_RET_CODE_SUCCESS)
  {
    ipa_test.agg.err_type = IPA_TEST_ERROR_TYPE_DEAGG_ETH_HDR;
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_qmap_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_QMAP type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_qmap_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pullup_len = 0;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(dl_cfg_ptr != NULL)

  (*num_pkts_used) = 0;

  /* Pull up the QMAP header, packet & buf */
  while (*aggr_frame_dsm_ptr != NULL)
  {
    pullup_len = dsm_pullup(aggr_frame_dsm_ptr, &ipa_test.agg.qmap_hdr,
                            sizeof(ipa_test.agg.qmap_hdr));
    IPA_TF_ASSERT(pullup_len == sizeof(ipa_test.agg.qmap_hdr));

    /* Swap Endianness as the header is in network byte order */
    *((uint32 *)(&ipa_test.agg.qmap_hdr)) = 
      ipa_ntohl((uint32)(*((uint32 *)(&ipa_test.agg.qmap_hdr))));

    /* Pull up the packet based on the QMAP header recvd */
    IPA_TF_ASSERT(ipa_test.agg.qmap_hdr.hdr_type == IPA_HW_QMAP_HDR_TYPE_DATA);

    pkt_arr_ptr[(*num_pkts_used)].mux_id = ipa_test.agg.qmap_hdr.mux_id;
    pkt_arr_ptr[(*num_pkts_used)].pkt_len = 
      ipa_test.agg.qmap_hdr.pkt_len - ipa_test.agg.qmap_hdr.pad_len;

    /* To account for EOF programming by the driver. The last qmap header 
    in the frame could potentially be a zeroed out header which needs to 
    be ignored */
    if ((ipa_test.agg.qmap_hdr.pkt_len == 0) &&
        (ipa_test.agg.qmap_hdr.mux_id == 0))
    {
      IPA_TF_ASSERT(*aggr_frame_dsm_ptr == NULL);
      ipa_test.agg.num_qmap_eofs++;
      continue;
    }

    pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr = 
      (uint8 *)IPA_MALLOC(pkt_arr_ptr[(*num_pkts_used)].pkt_len);
    IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr != NULL);

    pullup_len = 
      dsm_pullup(aggr_frame_dsm_ptr, 
                 pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr, 
                 pkt_arr_ptr[(*num_pkts_used)].pkt_len);
    IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)].pkt_len == pullup_len);

    /* Now pull up the pad bytes */
    pullup_len = 
      dsm_pullup(aggr_frame_dsm_ptr, NULL, ipa_test.agg.qmap_hdr.pad_len);
    IPA_TF_ASSERT(ipa_test.agg.qmap_hdr.pad_len == pullup_len);

    (*num_pkts_used)++;
  }

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_qmap_v3_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_QMAP_v3 type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_qmap_v3_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pullup_len = 0, cksum_val = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL)
  IPA_TF_ASSERT(dl_cfg_ptr != NULL)

  (*num_pkts_used) = 0;

  /* Pull up the QMAP header, packet & buf */
  while (*aggr_frame_dsm_ptr != NULL)
  {
    pullup_len = dsm_pullup(aggr_frame_dsm_ptr, &ipa_test.agg.qmap_hdr,
                            sizeof(ipa_test.agg.qmap_hdr));
    IPA_TF_ASSERT(pullup_len == sizeof(ipa_test.agg.qmap_hdr));

    /* Swap Endianness as the header is in network byte order */
    *((uint32 *)(&ipa_test.agg.qmap_hdr)) = 
      ipa_ntohl((uint32)(*((uint32 *)(&ipa_test.agg.qmap_hdr))));

    /* Pull up the packet based on the QMAP header recvd */
    IPA_TF_ASSERT(ipa_test.agg.qmap_hdr.hdr_type == IPA_HW_QMAP_HDR_TYPE_DATA);

    pkt_arr_ptr[(*num_pkts_used)].mux_id = ipa_test.agg.qmap_hdr.mux_id;
    pkt_arr_ptr[(*num_pkts_used)].pkt_len = 
      ipa_test.agg.qmap_hdr.pkt_len - ipa_test.agg.qmap_hdr.pad_len;

    /* To account for EOF programming by the driver. The last qmap header 
    in the frame could potentially be a zeroed out header which needs to 
    be ignored */
    if ((ipa_test.agg.qmap_hdr.pkt_len == 0) &&
        (ipa_test.agg.qmap_hdr.mux_id == 0))
    {
      IPA_TF_ASSERT(*aggr_frame_dsm_ptr == NULL);
      ipa_test.agg.num_qmap_eofs++;
      continue;
    }

    pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr = 
      (uint8 *)IPA_MALLOC(pkt_arr_ptr[(*num_pkts_used)].pkt_len);
    IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr != NULL);

    pullup_len = 
      dsm_pullup(aggr_frame_dsm_ptr, 
                 pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr, 
                 pkt_arr_ptr[(*num_pkts_used)].pkt_len);
    IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)].pkt_len == pullup_len);

    /* Now pull up the pad bytes */
    pullup_len = 
      dsm_pullup(aggr_frame_dsm_ptr, NULL, ipa_test.agg.qmap_hdr.pad_len);
    IPA_TF_ASSERT(ipa_test.agg.qmap_hdr.pad_len == pullup_len);

    /* Now pull up the cksum trailer */
    pullup_len = dsm_pullup(aggr_frame_dsm_ptr, &ipa_test.agg.cksum_trler,
                            sizeof(ipa_test.agg.cksum_trler));
    IPA_TF_ASSERT(pullup_len == sizeof(ipa_test.agg.cksum_trler));

    /* Swap Endianness as the header is in network byte order */
    *((uint32 *)(&ipa_test.agg.cksum_trler)) = 
      ipa_ntohl((uint32)(*((uint32 *)(&ipa_test.agg.cksum_trler))));
    *((uint32 *)(&ipa_test.agg.cksum_trler) + 1) = 
      ipa_ntohl((uint32)(*((uint32 *)(&ipa_test.agg.cksum_trler) + 1)));

    /* Compare the cksum value */
    /* Let's the find the cksum value of the buffer so that we can compare */
    cksum_val = 
      ipa_test_calculate_cksum((uint16 *)&pkt_arr_ptr[(*num_pkts_used)] \
                               .pkt_buf_ptr[ipa_test.agg.cksum_trler.start_offset],
                               ipa_test.agg.cksum_trler.len);

    /* Now compare the cksum value and indicate error if reqd */
    if ((cksum_val != ipa_ntohs(ipa_test.agg.cksum_trler.value)) || 
        (ipa_test.agg.cksum_trler.valid == FALSE))
    {
      ret_code = IPA_TEST_RET_CODE_FAILURE;
      ipa_test.agg.err_type = IPA_TEST_ERROR_TYPE_DEAGG_QMAP_V3_CKSUM;
      break;
    }

    (*num_pkts_used)++;
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_mbim_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_MBIM type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_mbim_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 pullup_len = 0;
  uint16 i, num_pkts_ndp, ndp_hdr_ofst = 0, mux_id;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL);
  IPA_TF_ASSERT(dl_cfg_ptr != NULL);

  (*num_pkts_used) = 0;

  /* We will use dsm_extract instead of pullup because it makes it 
  easier to navigate the MBIM frame */

  /* Grab the NTH header from the aggr_frame */
  pullup_len = dsm_extract(*aggr_frame_dsm_ptr, 0, &ipa_test.agg.nth_hdr, 
                           sizeof(ipa_test_nth_hdr_s));
  IPA_TF_ASSERT(pullup_len == sizeof(ipa_test_nth_hdr_s));

  /* Verify if we have the correct NTH signature */
  IPA_TF_ASSERT(ipa_test.agg.nth_hdr.sig_nth16 == IPA_TEST_NTH_SIG_VAL);
  IPA_TF_ASSERT(ipa_test.agg.nth_hdr.hdr_len == sizeof(ipa_test_nth_hdr_s));
  IPA_TF_ASSERT(ipa_test.agg.nth_hdr.tot_len == dsm_length_packet(*aggr_frame_dsm_ptr));

  ndp_hdr_ofst = ipa_test.agg.nth_hdr.idx_ndp;
  while (ndp_hdr_ofst != 0)
  {
    /* Grab the NDP header from the aggr_frame */
    pullup_len = dsm_extract(*aggr_frame_dsm_ptr, ndp_hdr_ofst, &ipa_test.agg.ndp_hdr, 
                             sizeof(ipa_test_ndp_hdr_s));
    IPA_TF_ASSERT(pullup_len == sizeof(ipa_test_ndp_hdr_s));

    /* Verify if we have the correct NDP signature */
    IPA_TF_ASSERT((ipa_test.agg.ndp_hdr.sig_ndp16 & 0xFFFFFF) == 
                  (IPA_TEST_NDP_SIG_VAL & 0xFFFFFF));

    /* Fetch the mux id from the last byte of the signature */
    mux_id = (ipa_test.agg.ndp_hdr.sig_ndp16 & 0xFF000000) >> 24;

    /* Find the number of pkts in this NDP. One extra hdr of zeros are at the end of 
    each NDP to determine  termination */
    num_pkts_ndp = ((ipa_test.agg.ndp_hdr.hdr_len - sizeof(ipa_test_ndp_hdr_s)) / 
                    sizeof(ipa_test_ndp_hdr_pkt_s)) - 1;
    for (i = 0; i < num_pkts_ndp; i++)
    {
      /* Grab the NDP packet header first */
      pullup_len = dsm_extract(*aggr_frame_dsm_ptr, ndp_hdr_ofst + sizeof(ipa_test_ndp_hdr_s) +
                               (i * sizeof(ipa_test_ndp_hdr_pkt_s)), 
                               &ipa_test.agg.pkt_hdr[i], 
                               sizeof(ipa_test_ndp_hdr_pkt_s));
      IPA_TF_ASSERT(pullup_len == sizeof(ipa_test_ndp_hdr_pkt_s));

      /* Set the pkt len and mux ID for this pkt */
      pkt_arr_ptr[(*num_pkts_used)].pkt_len = 
        ipa_test.agg.pkt_hdr[i].dgram_len;
      pkt_arr_ptr[(*num_pkts_used)].mux_id = mux_id;

      pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr = 
        (uint8 *)IPA_MALLOC(pkt_arr_ptr[(*num_pkts_used)].pkt_len);
      IPA_TF_ASSERT(pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr != NULL);

      /* Grab the pkts from the current NDP frame */
      pullup_len = dsm_extract(*aggr_frame_dsm_ptr, 
                               ipa_test.agg.pkt_hdr[i].dgram_idx, 
                               pkt_arr_ptr[(*num_pkts_used)].pkt_buf_ptr, 
                               pkt_arr_ptr[(*num_pkts_used)].pkt_len);
      IPA_TF_ASSERT(pullup_len == pkt_arr_ptr[(*num_pkts_used)].pkt_len);
      (*num_pkts_used)++;
    }

    /* Update the position of next NDP header */
    ndp_hdr_ofst = ipa_test.agg.ndp_hdr.idx_nxt;
  }

  /* Let us free the frame now */
  dsm_free_packet(aggr_frame_dsm_ptr);
  IPA_TF_ASSERT(*aggr_frame_dsm_ptr == NULL);

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_deaggregate_rndis_endp_pkt

==============================================================================*/
/*!
  @brief
  DeAggregates IPA_SIO_HDR_TYPE_RNDIS type frame

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_deaggregate_rndis_endp_pkt
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
  uint16                        num_pkts,
  ipa_sio_dl_config_s           *dl_cfg_ptr,
  dsm_item_type                 **aggr_frame_dsm_ptr,
  uint8                         *num_pkts_used 
)
{
  uint16 i = 0;
  uint16 pullup_len = 0;
  ipa_hw_rndis_hdr_s rndis_header_current;

  IPA_TF_ASSERT(num_pkts > 0);
  IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
  IPA_TF_ASSERT(pkt_arr_ptr != NULL);
  IPA_TF_ASSERT(dl_cfg_ptr != NULL);

  /* grab the RNDIS header from the aggr_frame */
  pullup_len = dsm_pullup(aggr_frame_dsm_ptr, &rndis_header_current, 
                          sizeof(rndis_header_current));
  IPA_TF_ASSERT(pullup_len == sizeof(rndis_header_current));

  i = 0;

  /* set the packet size */
  pkt_arr_ptr[i].pkt_len = rndis_header_current.data_len;

  /* malloc the packet buffer */
  pkt_arr_ptr[i].pkt_buf_ptr = (uint8 *)IPA_MALLOC(pkt_arr_ptr[i].pkt_len);
  IPA_TF_ASSERT(pkt_arr_ptr[i].pkt_buf_ptr != NULL);

  /* pullup the packet */
  pullup_len = 
     dsm_pullup(aggr_frame_dsm_ptr, 
                pkt_arr_ptr[i].pkt_buf_ptr,
                pkt_arr_ptr[i].pkt_len);
  IPA_TF_ASSERT(pkt_arr_ptr[i].pkt_len == pullup_len);

  /* Now pull up the pad bytes */
  pullup_len = 
    dsm_pullup(aggr_frame_dsm_ptr, NULL, 
               IPA_PAD_NUM_BYTES(pkt_arr_ptr[i].pkt_len));
  IPA_TF_ASSERT(IPA_PAD_NUM_BYTES(pkt_arr_ptr[i].pkt_len == pullup_len));

  return IPA_TEST_RET_CODE_SUCCESS;
}

void IPA_TEST_SET_V6_SRC_PREFIX(uint8 *buf_pkt_ptr, uint8 *src_prefix,
                                ipa_clat_prefix_len_e v6_src_prefix_len)
{
  IPA_TF_ASSERT(buf_pkt_ptr != NULL);

  IPA_TF_ASSERT(src_prefix != NULL);

  IPA_MEMCPY( (uint8*)&buf_pkt_ptr[IPA_TEST_IPV6_SRC_ADDR_OFST], (uint8*)src_prefix, IPA_TEST_GET_PREFIX_LEN(v6_src_prefix_len)/8 );
}

/*! @brief
   Sets the Dest Prefix in a V6 pkt*/
void IPA_TEST_SET_V6_DEST_PREFIX(uint8 *buf_pkt_ptr, uint8 *dest_prefix,
                                 ipa_clat_prefix_len_e v6_dest_prefix_len)
{
  IPA_TF_ASSERT(buf_pkt_ptr != NULL);

  IPA_TF_ASSERT(dest_prefix != NULL);

  IPA_MEMCPY( (uint8*)&buf_pkt_ptr[IPA_TEST_IPV6_DEST_ADDR_OFST], (uint8*)dest_prefix, IPA_TEST_GET_PREFIX_LEN(v6_dest_prefix_len)/8 );
}

ipa_test_ret_code_e ipa_test_deaggregate_replicated_pkts 
( 
	ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr_ul,
   	uint8                         *num_pkts_used_ul,
   	ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr_dl,
   	uint8                         *num_pkts_used_dl,
   	dsm_item_type                 **aggr_frame_dsm_ptr
)
{
	
	uint16 pullup_len = 0;
	uint8  *pkt_buf_ptr_discard;
	uint8 pad_bytes = 0;
	
	IPA_TF_ASSERT(aggr_frame_dsm_ptr != NULL);
	IPA_TF_ASSERT(pkt_arr_ptr_dl!= NULL);
	IPA_TF_ASSERT(pkt_arr_ptr_ul!= NULL);
	 
	(*num_pkts_used_ul) = 0;
	(*num_pkts_used_dl) = 0;

    while (*aggr_frame_dsm_ptr != NULL)
  	{
    	pullup_len = dsm_pullup(aggr_frame_dsm_ptr, &ipa_test.agg.log_status,
                            sizeof(ipa_test.agg.log_status));
    	IPA_TF_ASSERT(pullup_len == sizeof(ipa_test.agg.log_status));

    	/* Swap Endianness as the header is in network byte order */
    	/**((uint32 *)(&ipa_test.agg.log_status)) = 
      		ipa_ntohl((uint32)(*((uint32 *)(&ipa_test.agg.log_status))));*/

    	/* Pull up the packet based on the QMAP header recvd */
   		IPA_TF_ASSERT(ipa_test.agg.log_status.status_opcode == IPA_HW_STATUS_LOG);

		
		/* To account for EOF programming by the driver. The last qmap header 
    	in the frame could potentially be a zeroed out header which needs to 
    	be ignored */
    	if (ipa_test.agg.log_status.pkt_len == 0)
    	{
      		IPA_TF_ASSERT(*aggr_frame_dsm_ptr == NULL);
      			ipa_test.agg.num_log_pkt_eofs++;
      		continue;
    	}

		if (ipa_test.agg.log_status.pkt_len == 4 || 
			(ipa_test.agg.log_status.pkt_len == 32 && 
			 ((*aggr_frame_dsm_ptr)->data_ptr[0] != 0x45) &&
			 ((*aggr_frame_dsm_ptr)->data_ptr[0] != 0x60)))
    	{
    	    
			pkt_buf_ptr_discard = 
						(uint8 *)IPA_MALLOC(ipa_test.agg.log_status.pkt_len);
			
    	    pullup_len = 
      			dsm_pullup(aggr_frame_dsm_ptr, 
                 	pkt_buf_ptr_discard, 
                 	ipa_test.agg.log_status.pkt_len);

			IPA_TF_ASSERT(ipa_test.agg.log_status.pkt_len == pullup_len);
      		//IPA_TF_ASSERT(*aggr_frame_dsm_ptr == NULL);
      			ipa_test.agg.num_log_pkt_eofs++;
      		continue;
    	}

		if (ipa_test.agg.log_status.endp_src_idx == IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_DL_CONSUMER_PIPE))
		{
    		pkt_arr_ptr_dl[(*num_pkts_used_dl)].pkt_len = ipa_test.agg.log_status.pkt_len;

		    pkt_arr_ptr_dl[(*num_pkts_used_dl)].pkt_buf_ptr = 
      		(uint8 *)IPA_MALLOC(pkt_arr_ptr_dl[(*num_pkts_used_dl)].pkt_len);

			IPA_TF_ASSERT(pkt_arr_ptr_dl[(*num_pkts_used_dl)].pkt_buf_ptr != NULL);

    		pullup_len = 
      			dsm_pullup(aggr_frame_dsm_ptr, 
                 	pkt_arr_ptr_dl[(*num_pkts_used_dl)].pkt_buf_ptr, 
                 	pkt_arr_ptr_dl[(*num_pkts_used_dl)].pkt_len);

			IPA_TF_ASSERT(pkt_arr_ptr_dl[(*num_pkts_used_dl)].pkt_len == pullup_len);

    		(*num_pkts_used_dl)++;

            if(*aggr_frame_dsm_ptr != NULL)
            {
			  pad_bytes = IPA_PAD_NUM_BYTES((*aggr_frame_dsm_ptr)->data_ptr);
		
			 if (pad_bytes  != 0)
			 {
			   pullup_len = 
	              ipa_dsm_pull_discard(aggr_frame_dsm_ptr, (uint16)pad_bytes);
			 }
            }
		}
		else if (ipa_test.agg.log_status.endp_src_idx == IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_0))
	    {
    		pkt_arr_ptr_ul[(*num_pkts_used_ul)].pkt_len = ipa_test.agg.log_status.pkt_len;

		    pkt_arr_ptr_ul[(*num_pkts_used_ul)].pkt_buf_ptr = 
      		(uint8 *)IPA_MALLOC(pkt_arr_ptr_ul[(*num_pkts_used_ul)].pkt_len);

			IPA_TF_ASSERT(pkt_arr_ptr_ul[(*num_pkts_used_ul)].pkt_buf_ptr != NULL);

    		pullup_len = 
      			dsm_pullup(aggr_frame_dsm_ptr, 
                 	pkt_arr_ptr_ul[(*num_pkts_used_ul)].pkt_buf_ptr, 
                 	pkt_arr_ptr_ul[(*num_pkts_used_ul)].pkt_len);

			IPA_TF_ASSERT(pkt_arr_ptr_ul[(*num_pkts_used_ul)].pkt_len == pullup_len);

    		(*num_pkts_used_ul)++;

			if(*aggr_frame_dsm_ptr != NULL)
            {
			  pad_bytes = IPA_PAD_NUM_BYTES((*aggr_frame_dsm_ptr)->data_ptr);
		
			 if (pad_bytes  != 0)
			 {
			   pullup_len = 
	              ipa_dsm_pull_discard(aggr_frame_dsm_ptr, (uint16)pad_bytes);
			 }
            }
		}
			
  }

  return IPA_TEST_RET_CODE_SUCCESS;
	
		

}
//#pragma optimize("", on)
