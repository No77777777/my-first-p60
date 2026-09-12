/*!
  @file IPA_test_engine.cpp

  @brief
    IPA Test engine header file
*/                               
/*==============================================================================

  Copyright (c) 201314 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/dataipa.mpss/3.2/ipa/ipa_stride/IPA_test_engine.cpp#2 $
$Date: 2020/11/21 $
$Author: pwbldsvc $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
11/17/20    mk      CR 2820227: IPA Stride test Compilation Fix
05/30/17   db      CR13276332: Final submit for Napali BU include fixes for: multiple enque,
 				   Usage of new DPL APIs for HW Replication and more. 
07/07/15   pgm     IPAv3 test changes.
02/17/15   mi      CR#739684: IPA ZIP feature related test changes.
07/24/14   mi      Adding UL static filter support and checking for xlat field 
                   in the app_ptr of the DSM item. 
07/20/14   SaCh    Fixing a memory leak in UL CLAT pkt mirroring.
                   Adding support for IPA2.5 exception/error scenarios. 
                   Correcting DL CLAT Pkt Proc/Tx per PDN and only for IPv6 as opposed to global.
                   Correcting UL CLAT Pkt proc/Tx per PDN and only for IPv4 as opposed to global.
07/15/14   mi      IPA 2.1 test regression fixes.
07/13/14   SaCh    Fixes in Dl/UL/Loopback CLAT areas.
06/20/14   mi      Added IPV6 fragmentation support.
06/12/14   mi      IPA 2.1 fixes.
06/03/14   SaCh    Moving all data type definitions to ipa_test_engine.h.
05/14/14   mi      IPA2.5 CLAT test support.
05/25/14   rp      Test engine fixes/cleanup for issues seen on full stack build
04/25/14   rp      Changes to support ON-target testing on full stack CRM
04/16/14   pgm     IPA2.1 aggr/deaggr test support.
03/26/14   mi      Regression test fixes.
03/06/14   mi      Added fragmentation test changes.
02/28/14   mi      Bearer suspend/resume and deregistration changes.
01/17/14   SaCh    CR#600855: Expecting SIO_stream_id instead of uid value in 
                   ipa_ipfltr_alloc_rule_handle API for QOS rule.
12/18/13   mi      Fixed crash seen during Endpoint config.
12/11/13   mi      Added support for sio config mapping API changes.
12/09/13   pgm     ENDP_DELAY Flow control test changes.
12/05/13   mi      Added DPL support. And TCP ACK fixes.
11/19/13   pgm     Enable TCP ACK prioritization only once at Init.
10/25/13   rp      Adding supoort for multiple engine_run within a test
10/15/13   rp      DSM Chain fix & seed user cfg
09/30/13   pgm     Support for testing UL priority packets.
09/19/13   rp      Move from QMAPv2 to QMAPv3
09/17/13   rp      Filter Integration
08/22/13   rp      Initial Checkin
==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include "IPA_test_engine.h"
#include "IPA_test.h"
#include "ipa_test_filter.h"
#include "ipa_hw_common.h"
#include "ipa_ctl.h"
#include "ipa_gsi.h"
#include "ipa_hal.h"
#include "ipa_ul.h"

#pragma optimize("", off)
/*==============================================================================

                         MACROS

==============================================================================*/

/*=========================================================================

                         INTERNAL VARIABLES

=========================================================================*/
ipa_test_engine_s ipa_test_engine;
uint8 ipa_test_engine_sleep_time = 2;
uint16  *test1;
uint8 cb_bit_idx = 0;


/*! @brief Test engine copy of ipa_cfg.ctl, to be saved/restored before/after
 *  each test cases */
static ipa_cfg_ctl_s  ipa_test_engine_cfg_ctl_copy;

/* High priority destination buffers*/
static uint8 hi_prio_dest_buffer_0[IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX] __attribute__((aligned(128)));
static uint8 hi_prio_dest_buffer_1[IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX] __attribute__((aligned(128)));
static uint8 hi_prio_dest_buffer_2[IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX] __attribute__((aligned(128)));
static uint8 hi_prio_dest_buffer_3[IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX] __attribute__((aligned(128)));

/* Default priority destination buffers*/
static uint8 def_prio_dest_buffer_0[IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX] __attribute__((aligned(128)));
static uint8 def_prio_dest_buffer_1[IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX] __attribute__((aligned(128)));
static uint8 def_prio_dest_buffer_2[IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX] __attribute__((aligned(128)));
static uint8 def_prio_dest_buffer_3[IPA_TEST_ENGINE_ZIP_OUTPUT_BUF_SIZE_MAX] __attribute__((aligned(128)));

/*==============================================================================

                                FUNCTIONS

==============================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_engine_convert_v6_to_v4

==============================================================================*/
/*!
  @brief
  Convert the IPV4 packet to a IPV6 packet

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_convert_v6_to_v4
( 
  uint8                                     *dest_v4_buf,
  uint8                                     *src_v6_buf,
  uint16                                     pkt_len,
  ipa_test_engine_clat_cntxt_info_type      *clat_cntxt_ptr
)
{
  IPA_TF_ASSERT(dest_v4_buf != NULL);
  IPA_TF_ASSERT(pkt_len < IPA_TEST_ENGINE_IP_PKT_SIZE_MAX);

  /*! Memsetting the IPv4 header to 0*/
  IPA_MEMSET(dest_v4_buf, 0, IPA_TEST_IPV4_HDR_LEN );

  /*! Set IP Version */
  dest_v4_buf[0] = 0x45;

  /*! Set payload length */
  IPA_TEST_SET_IP_PKT_LEN(dest_v4_buf, (uint16)(pkt_len - IPA_TEST_IPV6_HDR_LEN + IPA_TEST_IPV4_HDR_LEN));

  /*! Set next header from the protocol of the IPV4 packet*/
  IPA_TEST_SET_IP_PROTOCOL(dest_v4_buf, src_v6_buf[5]);

  /*! Set the IPV6 source prefix for the packet */
  IPA_TEST_SET_V4_DEST_ADDR(dest_v4_buf, clat_cntxt_ptr->v4_src_addr);

  /* Copying just the payload from the packet */
  IPA_MEMCPY(&dest_v4_buf[IPA_TEST_IPV4_HDR_LEN], &src_v6_buf[IPA_TEST_IPV6_HDR_LEN], 
             (pkt_len - IPA_TEST_IPV6_HDR_LEN));
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_extract_ipv4_addr_from_ipv6_addr

==============================================================================*/
/*!
  @brief
  extracts IPv4 addr from IPv6 addr for CLAT translation.

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_clat_extract_ipv4_addr_from_ipv6_addr
(
  uint8   v6_prefix_len_in_bytes,
  uint8   *v4_buf,
  uint8   *v6_buf    
)
{
  uint8 offst = v6_prefix_len_in_bytes; 
  
  switch (v6_prefix_len_in_bytes)
  {
    case 4:
      *((uint32*)v4_buf) = *((uint32*)(&v6_buf[offst]));
    break;
       
    case 5:
      v4_buf[0] = *((uint8*)(&v6_buf[offst    ]));
      v4_buf[1] = *((uint8*)(&v6_buf[offst + 1]));
      v4_buf[2] = *((uint8*)(&v6_buf[offst + 2]));
      v4_buf[3] = *((uint8*)(&v6_buf[offst + 4]));
    break;
    
    case 6:
      v4_buf[0] = *((uint8*)(&v6_buf[offst    ]));
      v4_buf[1] = *((uint8*)(&v6_buf[offst + 1]));
      v4_buf[2] = *((uint8*)(&v6_buf[offst + 3]));
      v4_buf[3] = *((uint8*)(&v6_buf[offst + 4]));
    break;
    
    case 7:
      v4_buf[0] = *((uint8*)(&v6_buf[offst    ]));
      v4_buf[1] = *((uint8*)(&v6_buf[offst + 2]));
      v4_buf[2] = *((uint8*)(&v6_buf[offst + 3]));
      v4_buf[3] = *((uint8*)(&v6_buf[offst + 4]));
    break;
    
    case 8:
      v4_buf[0] = *((uint8*)(&v6_buf[offst + 1])); 
      v4_buf[1] = *((uint8*)(&v6_buf[offst + 2]));       
      v4_buf[2] = *((uint8*)(&v6_buf[offst + 3]));       
      v4_buf[3] = *((uint8*)(&v6_buf[offst + 4]));       
    break;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr

==============================================================================*/
/*!
  @brief
  embeds IPv4 addr in IPv6 addr for CLAT translation.

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr
(
  uint8   v6_prefix_len_in_bytes,
  uint8   *dest_v6_buf,
  uint32  v4_addr    
)
{
  IPA_ASSERT(dest_v6_buf != NULL);

  uint8 offst = v6_prefix_len_in_bytes;

  switch (v6_prefix_len_in_bytes)
  {
    case 4:
      *((uint32*)(&dest_v6_buf[offst])) = v4_addr;
    break;

    case 5:
      *((uint8*)(&dest_v6_buf[offst    ])) = ((uint8*)(&v4_addr))[0];
      *((uint8*)(&dest_v6_buf[offst + 1])) = ((uint8*)(&v4_addr))[1];
      *((uint8*)(&dest_v6_buf[offst + 2])) = ((uint8*)(&v4_addr))[2];
      *((uint8*)(&dest_v6_buf[offst + 4])) = ((uint8*)(&v4_addr))[3];
    break;

    case 6:
      *((uint8*)(&dest_v6_buf[offst    ])) = ((uint8*)(&v4_addr))[0];
      *((uint8*)(&dest_v6_buf[offst + 1])) = ((uint8*)(&v4_addr))[1];
      *((uint8*)(&dest_v6_buf[offst + 3])) = ((uint8*)(&v4_addr))[2];
      *((uint8*)(&dest_v6_buf[offst + 4])) = ((uint8*)(&v4_addr))[3];
    break;

    case 7:
      *((uint8*)(&dest_v6_buf[offst    ])) = ((uint8*)(&v4_addr))[0];
      *((uint8*)(&dest_v6_buf[offst + 2])) = ((uint8*)(&v4_addr))[1];
      *((uint8*)(&dest_v6_buf[offst + 3])) = ((uint8*)(&v4_addr))[2];
      *((uint8*)(&dest_v6_buf[offst + 4])) = ((uint8*)(&v4_addr))[3];
    break;

    case 8:
      *((uint8*)(&dest_v6_buf[offst + 1])) = ((uint8*)(&v4_addr))[0];
      *((uint8*)(&dest_v6_buf[offst + 2])) = ((uint8*)(&v4_addr))[1];
      *((uint8*)(&dest_v6_buf[offst + 3])) = ((uint8*)(&v4_addr))[2];
      *((uint8*)(&dest_v6_buf[offst + 4])) = ((uint8*)(&v4_addr))[3];       
    break;

    default:
      IPA_TF_ASSERT(0);
    break;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_set_dsm_checksum

==============================================================================*/
/*!
  @brief
  Calculates and sets the required checksum values on the DSM item control fields. 

  @returns
  IPA_TEST_RET_CODE_SUCCESS - on success
  Other error values otherwise
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_set_dsm_checksum
(
  ipa_test_engine_ip_pkt_info_s *pkt_arr,
  dsm_item_type *mirror_dsm_ptr,
  ipa_test_engine_pkt_trace_elem_s* pkt
  )
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint16 pkt_csum_val;

  pkt_csum_val = ipa_test_calculate_cksum(
    (uint16*)pkt_arr->pkt_buf_ptr, 
    (uint32)pkt_arr->pkt_len); 

  if (pkt != NULL)
  {
    pkt->checksum = pkt_csum_val;
  }
  
  IPA_CSUM_SET_APP_PTR_PARAMS(&mirror_dsm_ptr, 
    ipa_htons(pkt_arr->pkt_len), 
    pkt_csum_val);
  IPA_CSUM_SET_APP_FIELD_PARAMS(&mirror_dsm_ptr, 0, TRUE);

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_enqueue_ipv6_clat_frame

==============================================================================*/
/*!
  @brief
  Enqueues IPV6 CLAT frames in the mirror wmk. These are the frames expected
  from IPA in rx sio wmk for pkt comparison.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_enqueue_ipv6_clat_frame
(
  dsm_watermark_type                        *mirror_wm_ptr,
  boolean                                    is_qmap_v3,
  uint16                                     checksum,
  uint8                                     *pkt_buf_ptr,
  uint16                                    pkt_len,
  ipa_test_engine_clat_cntxt_info_type      *clat_cntxt_ptr
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  dsm_item_type *mirror_dsm_ptr = NULL;
  uint16 pushdown_len = 0;
  uint8  extn_hdr_len = 0;
  uint8 *dest_v6_buf = NULL;
  uint8 offst = 0;
  uint32 v4_src_addr = 0, v6_frag_ipid = 0, v4_dest_addr = 0;
  ipa_test_ipv6_pkt_frag_ofst_s      *frag_ofst_ptr = NULL;
  ipa_test_ip_pkt_frag_hdr_s         *v4_frag_hdr_ptr =  NULL;
  uint16                             v4_frag_hdr = 0, v6_frag_hdr = 0, v4_frag_ipid = 0;
  ipa_test_engine_ip_pkt_info_s pkt_info;
  uint16 pseudo_hdr_chksum;
  uint16 payload_chksum;
  uint16 checksum_buffer[2];
  uint16 cksum_val;
  uint8 chksum_hdr_offset = 0;

  IPA_TF_ASSERT(pkt_buf_ptr!= NULL);
  IPA_TF_ASSERT(mirror_wm_ptr != NULL);
  IPA_TF_ASSERT(clat_cntxt_ptr!= NULL);

  /*! Convert the IPV4 packet to the expected IPV6 packet
      according to the CLAT configuration for the SIO on which the 
      packet is going to be send. */
  dest_v6_buf = (uint8*)IPA_MALLOC(1500);
  IPA_TF_ASSERT(dest_v6_buf != NULL);
  IPA_MEMSET(dest_v6_buf, 0, 1500);

  /*! Set IP Version */
  IPA_TEST_SET_IP_VERSION(dest_v6_buf, IPA_IP_TYPE_IPV6);
  /*! Decrement TTL from IPv4 header and replace in hop_limit */
  dest_v6_buf[7] = pkt_buf_ptr[8] - 1;

  /*! Copy destination Prefix */
  IPA_MEMCPY( (uint8*)(&dest_v6_buf[IPA_TEST_IPV6_DEST_ADDR_OFST]), (uint8*)clat_cntxt_ptr->v6_dest_prefix, 
              (IPA_TEST_GET_PREFIX_LEN(clat_cntxt_ptr->v6_dest_prefix_len))/8);
  /*! Convert Destination Ipv4 Address to Ipv6 Address */  
  ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr(
              (IPA_TEST_GET_PREFIX_LEN(clat_cntxt_ptr->v6_dest_prefix_len))/8,
              &dest_v6_buf[IPA_TEST_IPV6_DEST_ADDR_OFST],
              clat_cntxt_ptr->v4_dest_addr);
  /*! Copy source prefix */
  IPA_MEMCPY( (uint8*)(&dest_v6_buf[IPA_TEST_IPV6_SRC_ADDR_OFST]), (uint8*)clat_cntxt_ptr->v6_src_prefix,
               (IPA_TEST_GET_PREFIX_LEN(clat_cntxt_ptr->v6_src_prefix_len))/8) ;
  /*! Convert Source Ipv4 Address to Ipv6 Address */  
  ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr(
              (IPA_TEST_GET_PREFIX_LEN(clat_cntxt_ptr->v6_src_prefix_len))/8,
              &dest_v6_buf[IPA_TEST_IPV6_SRC_ADDR_OFST], 
              clat_cntxt_ptr->v4_src_addr);  

  /*! If the DF bit was not set for this packet then add the extension header. */
  if ((pkt_buf_ptr[6] & 0x40) == 0)
  {
    extn_hdr_len = sizeof(ipa_test_ipv6_pkt_frag_extn_hdr_s);
    /* write Next_Header field in IPv6 header */
    IPA_TEST_SET_IPV6_NEXT_HDR(dest_v6_buf, IPA_TEST_ENGINE_FRAG_EXTN_HDR_TYPE);

    /* write Next_Header field in Extension header */
    IPA_TEST_SET_IPV6_EXT_NEXT_HDR(dest_v6_buf, pkt_buf_ptr[9]);

    if (ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK)
    {
      /*! update extension header length for loopback*/
      clat_cntxt_ptr->extn_hdr_len = extn_hdr_len;
    }
    if (ipa_test_engine.curr_test.cfg.ip_pkt_cfg.params.frag_pkt_len != 0)
    {
      frag_ofst_ptr = (ipa_test_ipv6_pkt_frag_ofst_s *)&dest_v6_buf[42];
      v4_frag_hdr = ipa_htons(*((uint16 *)&pkt_buf_ptr[6]));
      v4_frag_ipid = ipa_htons(*(uint16*)&pkt_buf_ptr[4]);
      v6_frag_ipid = (uint32)v4_frag_ipid;
      v4_frag_hdr_ptr = (ipa_test_ip_pkt_frag_hdr_s *)&v4_frag_hdr;
      frag_ofst_ptr->ofst = v4_frag_hdr_ptr->ofst;
      frag_ofst_ptr->mf = v4_frag_hdr_ptr->mf;
      v6_frag_hdr = ipa_ntohs(*((uint16*)frag_ofst_ptr));
      IPA_MEMCPY(frag_ofst_ptr,&v6_frag_hdr,sizeof(ipa_test_ip_pkt_frag_hdr_s));
      v6_frag_ipid = ipa_ntohl(v6_frag_ipid);
      IPA_MEMCPY(&dest_v6_buf[44],&v6_frag_ipid ,sizeof(uint32)) ;
    }
  }
  else
  {
    extn_hdr_len = 0;
    /*! Set next header from the protocol of the IPV4 packet*/
    IPA_TEST_SET_IPV6_NEXT_HDR(dest_v6_buf, pkt_buf_ptr[9]);
  }
  /*! Set payload length */
  IPA_TEST_SET_IP_PAYLOAD_LEN(dest_v6_buf,(uint16)(pkt_len - IPA_TEST_IPV4_HDR_LEN + extn_hdr_len));
  /* Copying just the payload from the packet */
  IPA_MEMCPY(&(dest_v6_buf[IPA_TEST_IPV6_HDR_LEN + extn_hdr_len]),
             &(pkt_buf_ptr[IPA_TEST_IPV4_HDR_LEN]), 
             (pkt_len - IPA_TEST_IPV4_HDR_LEN));

  /*if (is_qmap_v3 == TRUE)
  {
    if (pkt_buf_ptr[9] == IPA_TEST_TCP_PROT)
    {
      *((uint16*)(&dest_v6_buf[IPA_TEST_IPV6_HDR_LEN + extn_hdr_len + IPA_TEST_TCP_CKSUM_HDR_OFST])) = checksum;
    }
    else if (pkt_buf_ptr[9] == IPA_TEST_UDP_PROT)
    {
      *((uint16*)(&dest_v6_buf[IPA_TEST_IPV6_HDR_LEN + extn_hdr_len + IPA_TEST_UDP_CKSUM_HDR_OFST])) = checksum;
    }
  }*/

  if (pkt_buf_ptr[9] == IPA_TEST_TCP_PROT)
  {
    chksum_hdr_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
  }
  else if (pkt_buf_ptr[9] == IPA_TEST_UDP_PROT)
  {
    chksum_hdr_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
  }

  //if (pkt_buf_ptr[9] == IPA_TEST_TCP_PROT)
  //{
  	/*IPA_MEMSET(&v6_pseudo_hdr, 0, sizeof(v6_pseudo_hdr)); 

  	for (i = 0; i < 8; i++)
  	{
      v6_pseudo_hdr.src_addr[i] = *(uint16*)(&(dest_v6_buf[IPA_TEST_IPV6_SRC_ADDR_OFST + 2*i]));
      v6_pseudo_hdr.dest_addr[i] = *(uint16*)(&(dest_v6_buf[IPA_TEST_IPV6_DEST_ADDR_OFST + 2*i]));
    }
    v6_pseudo_hdr.protocol = IPA_TEST_TCP_PROT;
    v6_pseudo_hdr.tcp_length = pkt_len - IPA_TEST_IPV4_HDR_LEN;

    pseudo_hdr_chksum = ipa_test_calculate_cksum((uint16*) &v6_pseudo_hdr, sizeof(v6_pseudo_hdr));
    pseudo_hdr_chksum = ~pseudo_hdr_chksum & 0xFFFF;*/

	pseudo_hdr_chksum = ipa_test_calculate_pseudo_hdr_cksum(dest_v6_buf);

	*((uint16*)(&dest_v6_buf[IPA_TEST_IPV6_HDR_LEN + extn_hdr_len + chksum_hdr_offset])) = 0;

    payload_chksum = 
        ipa_test_calculate_cksum((uint16 *)&dest_v6_buf[IPA_TEST_IPV6_HDR_LEN + extn_hdr_len],
                                (pkt_len - IPA_TEST_IPV4_HDR_LEN));
    payload_chksum = ~payload_chksum & 0xFFFF;
	  
    checksum_buffer[0] = pseudo_hdr_chksum;
    checksum_buffer[1] = payload_chksum;
    cksum_val = ipa_test_calculate_cksum((uint16 *)&checksum_buffer, sizeof(checksum_buffer));

	*((uint16*)(&dest_v6_buf[IPA_TEST_IPV6_HDR_LEN + extn_hdr_len + chksum_hdr_offset])) = cksum_val;
 // }

  IPA_TF_ASSERT(mirror_wm_ptr != NULL);
  /*! Pushdown the IPV4 packet in the mirror dsm pointer*/
  pushdown_len = 
        dsm_pushdown(&mirror_dsm_ptr,
                     dest_v6_buf,
                     (pkt_len -  IPA_TEST_IPV4_HDR_LEN + IPA_TEST_IPV6_HDR_LEN + extn_hdr_len),
                     IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(pushdown_len == (pkt_len -  
                                 IPA_TEST_IPV4_HDR_LEN + IPA_TEST_IPV6_HDR_LEN + extn_hdr_len));

  /*! If its a V4 CLAT frag test case then fill the app field to the IPV6 frag filter result*/
  if (ipa_test_engine.curr_test.cfg.fltr_cfg.fltr_rule_type == IPA_TEST_ENGINE_FLTR_TYPE_CLAT_FRAG_V4)
  {
   IPA_IPFLTR_SET_UL_FLTR_RESULT(mirror_dsm_ptr, IPV6_FRAG_FLTR_RES);
  }

  IPA_FREE(dest_v6_buf);
  /* Set packet checksum fields */
  pkt_info.pkt_len = pushdown_len;
  pkt_info.pkt_buf_ptr = mirror_dsm_ptr->data_ptr;

  /*IPA_TF_ASSERT(ipa_test_engine_set_dsm_checksum (
    &pkt_info,
    mirror_dsm_ptr,
    NULL) == IPA_TEST_RET_CODE_SUCCESS);*/
  //IPA_DSM_SET_IP_VERSION_PARAM(&mirror_dsm_ptr, IPA_DSM_IP_VERSION_6);
  /*! Enqueue the DSM item in the mirror watermark. */
  dsm_enqueue(mirror_wm_ptr, &mirror_dsm_ptr);    
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_enqueue_ipv4_clat_frame

==============================================================================*/
/*!
  @brief
  Enqueues IPV4 CLAT frames in the mirror wmk. These are the frames expected
  from IPA in rx sio wmk for pkt comparison.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_enqueue_ipv4_clat_frame
(
  dsm_watermark_type                      *mirror_wm_ptr,
  uint8                                   *pkt_buf_ptr,
  uint16                                  pkt_len,
  uint16                                  extn_hdr_len,
  ipa_test_engine_clat_cntxt_info_type    *clat_cntxt_ptr
)
{
  dsm_item_type *mirror_dsm_ptr = NULL;
  uint16 pushdown_len = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 *dest_v4_buf = NULL;
  ipa_test_engine_ip_pkt_info_s pkt_info;
  uint16 pseudo_hdr_chksum;
  uint16 payload_chksum;
  uint16 checksum_buffer[2];
  uint16 cksum_val;
  uint8 chksum_hdr_offset = 0;
 
  IPA_TF_ASSERT(pkt_buf_ptr != NULL);
  IPA_TF_ASSERT(mirror_wm_ptr != NULL);
  IPA_TF_ASSERT(clat_cntxt_ptr!= NULL);

  /*! Convert the IPV6 packet to the expected IPV4 packet
      according to the CLAT configuration for the SIO on which the 
      packet is going to be send. */

  dest_v4_buf = (uint8*)IPA_MALLOC(1500);
  IPA_TF_ASSERT(dest_v4_buf != NULL);
  IPA_MEMSET(dest_v4_buf, 0, 1500);

  /*! Set IP Version */
  dest_v4_buf[0] = 0x45;
  if(pkt_buf_ptr[6] == IPA_TEST_ENGINE_DEST_OPTS_EXTN_HDR_TYPE || 
  	 extn_hdr_len == 0)
  {
    dest_v4_buf[6] |= (1 << 6);
  }

  /*! Decrement hop-limit from IPv6 header and replace in TTL field */
  dest_v4_buf[8] = pkt_buf_ptr[7] - 1;

  /*! Set Total length field in IPv4 header */
  IPA_TEST_SET_IP_PKT_LEN(dest_v4_buf, (uint16)(pkt_len - IPA_TEST_IPV6_HDR_LEN + IPA_TEST_IPV4_HDR_LEN - extn_hdr_len));

  /*! Set protocol field in IPv4 header */
  if (extn_hdr_len != 0)
  {
    IPA_TEST_SET_IP_PROTOCOL(dest_v4_buf, pkt_buf_ptr[IPA_TEST_IPV6_HDR_LEN]);
  }
  else
  {
    IPA_TEST_SET_IP_PROTOCOL(dest_v4_buf, pkt_buf_ptr[6]);
  }
  
  /*! Set Src address field */
  IPA_TEST_SET_V4_SRC_ADDR(dest_v4_buf, clat_cntxt_ptr->v4_dest_addr);

  IPA_TEST_SET_V4_DEST_ADDR(dest_v4_buf, clat_cntxt_ptr->v4_src_addr);

  /* Copying just the payload from the packet */
  IPA_MEMCPY(&dest_v4_buf[IPA_TEST_IPV4_HDR_LEN], &pkt_buf_ptr[IPA_TEST_IPV6_HDR_LEN + extn_hdr_len], 
             (pkt_len - IPA_TEST_IPV6_HDR_LEN - extn_hdr_len));

  if (dest_v4_buf[9] == IPA_TEST_TCP_PROT)
  {
    chksum_hdr_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
  }
  else if (dest_v4_buf[9] == IPA_TEST_UDP_PROT)
  {
    chksum_hdr_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
  }

  pseudo_hdr_chksum = ipa_test_calculate_pseudo_hdr_cksum(dest_v4_buf);

  *((uint16*)(&dest_v4_buf[IPA_TEST_IPV4_HDR_LEN + chksum_hdr_offset])) = 0;

  payload_chksum = 
        ipa_test_calculate_cksum((uint16 *)&dest_v4_buf[IPA_TEST_IPV4_HDR_LEN],
                                 (pkt_len - IPA_TEST_IPV6_HDR_LEN - extn_hdr_len));
  payload_chksum = ~payload_chksum & 0xFFFF;
	  
  checksum_buffer[0] = pseudo_hdr_chksum;
  checksum_buffer[1] = payload_chksum;
  cksum_val = ipa_test_calculate_cksum((uint16 *)&checksum_buffer, sizeof(checksum_buffer));

  *((uint16*)(&dest_v4_buf[IPA_TEST_IPV4_HDR_LEN + chksum_hdr_offset])) = cksum_val;

  /*! Add Checksum in IPv4 header */
  *((uint16*)&dest_v4_buf[10]) = ipa_test_calculate_cksum((uint16*)dest_v4_buf, IPA_TEST_IPV4_HDR_LEN);

  /*! Pushdown the IPV4 packet in the mirror dsm pointer*/
  pushdown_len = 
        dsm_pushdown(&mirror_dsm_ptr,
                     dest_v4_buf,
                     (pkt_len - IPA_TEST_IPV6_HDR_LEN + IPA_TEST_IPV4_HDR_LEN - extn_hdr_len),
                     IPA_CFG_SL_DSM_ITEM_POOL);
  IPA_TF_ASSERT(pushdown_len == (pkt_len - IPA_TEST_IPV6_HDR_LEN + IPA_TEST_IPV4_HDR_LEN - extn_hdr_len));

  IPA_FREE(dest_v4_buf);

  /* Set packet checksum fields */
  pkt_info.pkt_len = pushdown_len;
  pkt_info.pkt_buf_ptr = mirror_dsm_ptr->data_ptr;
  IPA_TF_ASSERT(ipa_test_engine_set_dsm_checksum (
    &pkt_info,
    mirror_dsm_ptr,
    NULL) == IPA_TEST_RET_CODE_SUCCESS);
  IPA_DSM_SET_IP_VERSION_PARAM(&mirror_dsm_ptr, IPA_DSM_IP_VERSION_4);
  /*! Enqueue the DSM item in the mirror watermark. */
  dsm_enqueue(mirror_wm_ptr, &mirror_dsm_ptr);
  return ret_code;    
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_test_endp_stats

==============================================================================*/
/*!
  @brief
  Stores the stats after a test is run

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_test_endp_stats(void)
{
  uint8 i = 0;
  for (i = 0; i < ipa_test_engine.curr_test.cfg.endp_cfg.num_per_sim; i++)
  {
    switch (ipa_test_engine.curr_test.cfg.test_type)
    {
      case IPA_TEST_ENGINE_TEST_TYPE_DL:
      {
        switch (ipa_test_engine.curr_test.cfg.endp_cfg.sim_endp_cfg[i]. \
                dl_config.hdr_type)
        {
          case IPA_SIO_HDR_TYPE_ETHERNET:
            ipa_test_engine.stats.endp.dl.num_eth_tests++;
            break;
          case IPA_SIO_HDR_TYPE_NONE:
            ipa_test_engine.stats.endp.dl.num_none_tests++;
            break;
          case IPA_SIO_HDR_TYPE_QMAP:
            ipa_test_engine.stats.endp.dl.num_qmap_tests++;
            break;
          case IPA_SIO_HDR_TYPE_QMAP_V3:
            ipa_test_engine.stats.endp.dl.num_qmap_v3_tests++;
            break;
          case IPA_SIO_HDR_TYPE_MBIM:
            ipa_test_engine.stats.endp.dl.num_mbim_tests++;
            break;
          default:
            break;
        }
      }
      break;

      case IPA_TEST_ENGINE_TEST_TYPE_UL:
      {
        switch (ipa_test_engine.curr_test.cfg.endp_cfg.sim_endp_cfg[i]. \
                ul_config.hdr_type)
        {
          case IPA_SIO_HDR_TYPE_ETHERNET:
            ipa_test_engine.stats.endp.ul.num_eth_tests++;
            break;
          case IPA_SIO_HDR_TYPE_NONE:
            ipa_test_engine.stats.endp.ul.num_none_tests++;
            break;
          case IPA_SIO_HDR_TYPE_QMAP:
            ipa_test_engine.stats.endp.ul.num_qmap_tests++;
            break;
          case IPA_SIO_HDR_TYPE_QMAP_V3:
            ipa_test_engine.stats.endp.ul.num_qmap_v3_tests++;
            break;
          case IPA_SIO_HDR_TYPE_MBIM:
            ipa_test_engine.stats.endp.ul.num_mbim_tests++;
            break;
          case IPA_SIO_HDR_TYPE_DIRECT_DMA:
            ipa_test_engine.stats.endp.ul.num_dma_tests++;
            break;
          default:
            break;
        }
      }
      break;

      case IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK:
      case IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK:
      case IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB:
      case IPA_TEST_ENGINE_TEST_TYPE_UL_HOLB:
      {
        switch (ipa_test_engine.curr_test.cfg.endp_cfg.sim_endp_cfg[i]. \
                dl_config.hdr_type)
        {
          case IPA_SIO_HDR_TYPE_ETHERNET:
            ipa_test_engine.stats.endp.dl.num_eth_tests++;
            break;
          case IPA_SIO_HDR_TYPE_NONE:
            ipa_test_engine.stats.endp.dl.num_none_tests++;
            break;
          case IPA_SIO_HDR_TYPE_QMAP:
            ipa_test_engine.stats.endp.dl.num_qmap_tests++;
            break;
          case IPA_SIO_HDR_TYPE_QMAP_V3:
            ipa_test_engine.stats.endp.dl.num_qmap_v3_tests++;
            break;
          case IPA_SIO_HDR_TYPE_MBIM:
            ipa_test_engine.stats.endp.dl.num_mbim_tests++;
            break;
          default:
            break;
        }
        switch (ipa_test_engine.curr_test.cfg.endp_cfg.sim_endp_cfg[i]. \
                ul_config.hdr_type)
        {
          case IPA_SIO_HDR_TYPE_ETHERNET:
            ipa_test_engine.stats.endp.ul.num_eth_tests++;
            break;
          case IPA_SIO_HDR_TYPE_NONE:
            ipa_test_engine.stats.endp.ul.num_none_tests++;
            break;
          case IPA_SIO_HDR_TYPE_QMAP:
            ipa_test_engine.stats.endp.ul.num_qmap_tests++;
            break;
          case IPA_SIO_HDR_TYPE_QMAP_V3:
            ipa_test_engine.stats.endp.ul.num_qmap_v3_tests++;
            break;
          case IPA_SIO_HDR_TYPE_MBIM:
            ipa_test_engine.stats.endp.ul.num_mbim_tests++;
            break;
          case IPA_SIO_HDR_TYPE_DIRECT_DMA:
            ipa_test_engine.stats.endp.ul.num_dma_tests++;
            break;
          default:
            break;
        }
      }
      break;

      default:
        IPA_TF_ASSERT(0);
        break;
    }
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_test_wan_stats

==============================================================================*/
/*!
  @brief
  Stores the stats after a test is run

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_test_wan_stats(void)
{
  uint8 i = 0;
  uint8 c_params = 0;
  uint8 is_ciph_test = FALSE;
  uint8 is_bridged_test = FALSE;
  uint8 is_unbridged_test = FALSE;

  if (ipa_test_engine.curr_test.cfg.test_type != IPA_TEST_ENGINE_TEST_TYPE_UL)
  {
    for (i = 0; i < ipa_test_engine.curr_test.cfg.wan_cfg.num_bearers; i++)
    {
      /* We need a LTE bearer for ciphering to work */
      if ((ipa_test_engine.curr_test.cfg.wan_cfg.bearer_tech[i] == 
           IPA_WAN_BEARER_TECH_LTE) &&
          (ipa_test_engine.curr_test.cfg.wan_cfg.is_ciph_enabled == TRUE))
      {
        is_ciph_test = TRUE;
      }
      if (ipa_test_engine.curr_test.cfg.wan_cfg.is_bearer_bridged[i] == 
          TRUE)
      {
        is_bridged_test = TRUE;
      }
      else
      {
        is_unbridged_test = TRUE;
      }
    }
    if (is_ciph_test == TRUE)
    {
      ipa_test_engine.stats.ciph.num_ciph_tests++;
      for (c_params = 0; c_params < IPA_TEST_ENGINE_CIPH_PARAMS_NUM_MAX; c_params++)
      {
        switch (ipa_test_engine.ciph.ciph_params[c_params].ciph_algo)
        {
          case IPA_WAN_CIPHER_ALGO_AES:
            ipa_test_engine.stats.ciph.num_aes_pkts +=
              ipa_test_engine.ciph.ciph_params[c_params].num_pkts_processed;
            break;

          case IPA_WAN_CIPHER_ALGO_ZUC:
            ipa_test_engine.stats.ciph.num_zuc_pkts +=
              ipa_test_engine.ciph.ciph_params[c_params].num_pkts_processed;
            break;

          case IPA_WAN_CIPHER_ALGO_LTE_SNOW3G:
            ipa_test_engine.stats.ciph.num_snow_pkts +=
              ipa_test_engine.ciph.ciph_params[c_params].num_pkts_processed;
            break;

          case IPA_WAN_CIPHER_ALGO_NONE:
            ipa_test_engine.stats.ciph.num_none_pkts +=
              ipa_test_engine.ciph.ciph_params[c_params].num_pkts_processed;
            break;

          default:
            break;
        }
      }
    }
    if (is_bridged_test == TRUE)
    {
      ipa_test_engine.stats.wan.num_bridge_tests++;
    }
    if (is_unbridged_test == TRUE)
    {
      ipa_test_engine.stats.wan.num_unbridge_tests++;
    }
    if (ipa_test_engine.curr_test.cfg.wan_cfg.num_bearers == 0)
    {
      ipa_test_engine.stats.wan.num_sio_tests++;
    }
    else
    {
      ipa_test_engine.stats.wan.num_pdcp_tests++;
    }
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_test_pkt_stats

==============================================================================*/
/*!
  @brief
  Stores the stats after a test is run

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_test_pkt_stats(void)
{
  if (ipa_test_engine.curr_test.cfg.ip_pkt_cfg.version == IPA_IP_TYPE_IPV4)
  {
    ipa_test_engine.stats.pkt.num_v4_tests++;
  }
  else
  {
    ipa_test_engine.stats.pkt.num_v6_tests++;
  }
  if ((ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_dsm_items != 1) &&
      (ipa_test_engine.curr_test.cfg.test_type != IPA_TEST_ENGINE_TEST_TYPE_UL))
  {
    ipa_test_engine.stats.pkt.num_dsm_split_tests++;
  }
  if (ipa_test_engine.curr_test.cfg.ip_pkt_cfg.params.frag_pkt_len != 0)
  {
    ipa_test_engine.stats.pkt.num_frag_tests++;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_test_fltr_stats

==============================================================================*/
/*!
  @brief
  Stores the stats after a test is run

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_test_fltr_stats(void)
{
  if (ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == TRUE)
  {
    ipa_test_engine.stats.fltr.num_fltr_tests++;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_test_stats

==============================================================================*/
/*!
  @brief
  Stores the stats after a test is run

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_test_stats(void)
{
  /* Incrment the stats */
  ipa_test_engine.stats.num_tests_run++;
  ipa_test_engine.stats.num_pkts += 
    ipa_test_engine.curr_test.num_pkts_tot;

  if (ipa_test_engine.curr_test.cfg.test_type == 
      IPA_TEST_ENGINE_TEST_TYPE_DL)
  {
    ipa_test_engine.stats.num_dl_tests_run++;
    ipa_test_engine.stats.num_dl_pkts += 
      ipa_test_engine.curr_test.num_pkts_tot;

    ipa_test_engine.stats.pkt.num_pkts_dl_split_avg += 
      ipa_test_engine.curr_test.num_pkts_dl_split_avg;
    ipa_test_engine.stats.pkt.num_pkts_dl_split_col += 
      ipa_test_engine.curr_test.num_pkts_dl_split_col;
  }
  else if (ipa_test_engine.curr_test.cfg.test_type == 
           IPA_TEST_ENGINE_TEST_TYPE_UL)
  {
    ipa_test_engine.stats.num_ul_tests_run++;
    ipa_test_engine.stats.num_ul_pkts += 
      ipa_test_engine.curr_test.num_pkts_tot;
  }
  else
  {
    ipa_test_engine.stats.num_loopback_tests_run++;

    /* Loopback tests will have half UL pkts & DL pkts */
    ipa_test_engine.stats.num_dl_pkts += 
      ipa_test_engine.curr_test.num_pkts_tot/2;
    ipa_test_engine.stats.num_ul_pkts += 
      ipa_test_engine.curr_test.num_pkts_tot/2;

    ipa_test_engine.stats.pkt.num_pkts_dl_split_avg += 
      ipa_test_engine.curr_test.num_pkts_dl_split_avg;
    ipa_test_engine.stats.pkt.num_pkts_dl_split_col += 
      ipa_test_engine.curr_test.num_pkts_dl_split_col;
  }

  ipa_test_engine_store_test_endp_stats();

  ipa_test_engine_store_test_wan_stats();

  ipa_test_engine_store_test_pkt_stats();

  ipa_test_engine_store_test_fltr_stats();
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_tx_pkt_trace

==============================================================================*/
/*!
  @brief
  Stores the pkt trace before enqueing into a src wm

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_tx_pkt_trace
(
  ipa_test_engine_pkt_trace_elem_s *pkt
)
{
  uint16 idx = ipa_test_engine.curr_test.num_pkts_tx;

  ipa_test_engine.curr_test.pkt_trace.tx_pkts[idx] = *pkt;

  ipa_test_engine.curr_test.num_pkts_tx++;

  if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE)
  {
    ipa_test_engine.curr_test.num_pkts_tx++;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_rx_pkt_trace

==============================================================================*/
/*!
  @brief
  Stores the pkt trace before enqueing into a src wm

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_rx_pkt_trace
(
  ipa_test_engine_pkt_trace_elem_s *pkt
)
{
  uint16 idx = ipa_test_engine.curr_test.num_pkts_rx;

  ipa_test_engine.curr_test.pkt_trace.rx_pkts[idx] = *pkt;

  ipa_test_engine.curr_test.num_pkts_rx++;

  /* Also increment the total num of pkts in test */
  ipa_test_engine.curr_test.num_pkts_tot++;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_state_trace

==============================================================================*/
/*!
  @brief
  Stores the test state trace 

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_state_trace
(
  ipa_test_engine_state_e state
)
{
  uint16 idx;

  ipa_crit_sect_enter(&ipa_test_engine.state_trace.crit_sect);

  ipa_test_engine.curr_test.state = state;

  idx = ipa_test_engine.state_trace.trace_idx++;
  ipa_test_engine.state_trace.trace_arr[idx].state = state;
  ipa_test_engine.state_trace.trace_arr[idx].timetick = ipa_test_timetick_get();
  ipa_test_engine.state_trace.trace_idx %= IPA_TEST_ENGINE_STATE_TRACE_LEN;

  ipa_crit_sect_leave(&ipa_test_engine.state_trace.crit_sect);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_store_test_trace

==============================================================================*/
/*!
  @brief
  Stores the test trace after the test is completed

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_store_test_trace(void)
{
  uint8 i = 0;
  uint8 idx = ipa_test_engine.test_trace.idx;

  ipa_test_engine.test_trace.tests[idx].cfg = 
    ipa_test_engine.curr_test.cfg;
  ipa_test_engine.test_trace.tests[idx].pkt = 
    ipa_test_engine.curr_test.pkt_trace;

  for (i = 0; i < IPA_TEST_ENGINE_PER_SIM_NUM_MAX; i++)
  {
    ipa_test_engine.test_trace.tests[idx].aggr[i] = 
      ipa_test_engine.curr_test.aggr[i];
  }

  ipa_test_engine.test_trace.idx++;
  ipa_test_engine.test_trace.idx %= IPA_TEST_ENGINE_TEST_TRACE_LEN;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_generate_subs_ids

==============================================================================*/
/*!
  @brief
  Generate unique substem ID's for the system

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_generate_subs_ids(void)
{
  uint8 i;
  
  for (i = 1; i < IPA_TEST_ENGINE_SUBS_NUM_MAX; i++)
  {
    IPA_ASSERT(ipa_wan.cipher.subs[i].in_use == IPA_ENTRY_IN_USE);
    ipa_test_engine.subs_id_arr[i] = IPA_WAN_GET_SUBS_ID_BY_IDX(i);
  } 
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_endp_sio_map_cb_fn

==============================================================================*/
/*!
  @brief
  Cb function for completion of a SIO mapping 

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_endp_sio_map_cb_fn
(
  sio_stream_id_type stream_id,
  uint32             cb_data
)
{
  ipa_test_engine_sio_map_cb_s *sio_map_cb = 
    (ipa_test_engine_sio_map_cb_s *)cb_data;
  IPA_TF_ASSERT(stream_id < SIO_MAX_STREAM);

  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
  /* Check if we are indeed waiting for a callback for this stream ID */
  IPA_TF_ASSERT(sio_map_cb->pend_stream_id_arr[stream_id] == TRUE);
  /* Now set this stream ID to false in pending cb array */
  sio_map_cb->pend_stream_id_arr[stream_id] = FALSE;
  sio_map_cb->num_pend_sio_cb--;
  ipa_crit_sect_leave(&ipa_test_engine.crit_sect);

  ipa_signal_set(&ipa_test_engine.endp_wait_signal);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_cfg_cb_fn

==============================================================================*/
/*!
  @brief
  Cb function for completion of a CLAT config

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_clat_cfg_cb_fn
(
  ipa_clat_cfg_cb_evt_e cb_event,
  ipa_clat_handle_t     clat_hdl,
  void*                 cb_data
)
{
  ipa_test_engine_clat_cfg_cb_s *clat_cfg_cb = 
    (ipa_test_engine_clat_cfg_cb_s *)cb_data;

  uint8 clat_hdl_idx = (uint8)clat_hdl;

  /*! CLAT CBS needs to be invoked twice for clat cfg and assoc sio id*/
  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);

  switch (cb_event)
  {
    case IPA_CLAT_CFG_CB_EVT_REGISTERED:
    {
      IPA_TF_ASSERT(clat_cfg_cb->clat_cfg_arr[clat_hdl_idx] == 
                    IPA_TEST_ENGINE_CLAT_CB_ENABLED);
      clat_cfg_cb->clat_cfg_arr[clat_hdl_idx] =
                    IPA_TEST_ENGINE_CLAT_CB_CFG_DONE;
      clat_cfg_cb->num_pend_clat_cfg_cb--;
      ipa_crit_sect_leave(&ipa_test_engine.crit_sect);
      ipa_signal_set(&ipa_test_engine.clat_cfg_signal);
      
    }
    break;

    case IPA_CLAT_CFG_CB_EVT_SIO_ASSOCIATED:
    {
      IPA_TF_ASSERT(clat_cfg_cb->clat_cfg_arr[clat_hdl_idx] == 
                    IPA_TEST_ENGINE_CLAT_CB_CFG_DONE);
      /*! CLAT is configured now as this is the final CB . 
         So no pending entry and set the signal.*/
      clat_cfg_cb->clat_cfg_arr[clat_hdl_idx] =
                    IPA_TEST_ENGINE_CLAT_CB_ASSOC_SIO_DONE;
      clat_cfg_cb->num_pend_assoc_sio_cb--;
      
      ipa_crit_sect_leave(&ipa_test_engine.crit_sect);
      ipa_signal_set(&ipa_test_engine.clat_assoc_sio_signal);
    }
    break;

    case IPA_CLAT_CFG_CB_EVT_DEREGISTERED:
    {
      IPA_TF_ASSERT(clat_cfg_cb->clat_cfg_arr[clat_hdl_idx] == 
                    IPA_TEST_ENGINE_CLAT_CB_ASSOC_SIO_DONE);
      clat_cfg_cb->clat_cfg_arr[clat_hdl_idx] =
                    IPA_TEST_ENGINE_CLAT_CB_DEREG_DONE;
      clat_cfg_cb->num_pend_dereg_cb--;
      ipa_crit_sect_leave(&ipa_test_engine.crit_sect);
      ipa_signal_set(&ipa_test_engine.clat_dereg_signal);
    }
    break;

    case IPA_CLAT_CFG_CB_EVT_RECONFIGURED:
      ipa_crit_sect_leave(&ipa_test_engine.crit_sect);
    break;

    default:
      ipa_crit_sect_leave(&ipa_test_engine.crit_sect);
    break;

  }
  
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_watchdog_timer_cb

==============================================================================*/
/*!
  @brief
  Timer to detect HW/Driver stalls

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_watchdog_timer_cb
(
  timer_cb_data_type timer_cb_data
)
{
  if (ipa_test_engine.curr_test.cfg.zip_cfg.is_holb_ver_enabled != TRUE)
  {
  IPA_TF_ASSERT_LOG(ipa_test_engine.curr_test.num_pkts_rx_wdog_prev !=
                    ipa_test_engine.curr_test.num_pkts_rx, 
                    "Test Engine Stall detcted", 0, 0, 0);
  }

  timer_set(&ipa_test_engine.watchdog_timer.timer, 
            IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL, 0, T_MSEC);

  ipa_test_engine.curr_test.num_pkts_rx_wdog_prev =
       ipa_test_engine.curr_test.num_pkts_rx;

}

/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_resp_timer_cb

==============================================================================*/
/*!
  @brief
  Timer to detect HW/Driver stalls

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_zip_resp_timer_cb
(
  timer_cb_data_type timer_cb_data
)
{
  IPA_TF_ASSERT_LOG(ipa_test_engine.zip.num_resp_rx_wdog_prev !=
                    ipa_test_engine.zip.num_resp, 
                    "Test Engine Stall detcted", 0, 0, 0);

  timer_set(&ipa_test_engine.watchdog_timer.timer, 
            IPA_TEST_ENGINE_ZIP_RESP_INTERVAL, 0, T_MSEC);

  ipa_test_engine.zip.num_resp_rx_wdog_prev =
       ipa_test_engine.zip.num_resp;

}

/*==============================================================================

  FUNCTION:  ipa_test_engine_flow_ctl_timer_cb

==============================================================================*/
/*!
  @brief
  Timer to detect if flow control kicked in

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_flow_ctl_timer_cb
(
  timer_cb_data_type timer_cb_data
)
{
  sio_stream_id_type stream_id;

  stream_id = ipa_test_engine.endp.log_ch[0].stream_id;
   
  if (TRUE == ipa_test_engine.curr_test.is_flow_disabled)
  {
    /*Assert if pkts were sent after flow control was issued*/
    IPA_TF_ASSERT_LOG(0 == ipa_test_engine.curr_test.num_pkts_rx_to_process, 
                       "UL Flow Control failed!", 0, 0, 0);

    /*Enable UL Flow back and start timer for remaining pkts to be received*/
    sio_ioctl(stream_id, SIO_IOCTL_INBOUND_FLOW_ENABLE, NULL);
    ipa_test_engine.curr_test.is_flow_disabled = FALSE;

    /*Clear Flow Ctl timer*/
    timer_clr(&ipa_test_engine.flow_ctl_timer.timer, T_NONE);
  }
  else
  {
     IPA_ASSERT(0);
  }

}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ds_evnt_timer_cb

==============================================================================*/
/*!
  @brief
  Timer to detect if ds event has taken place

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_ds_evnt_timer_cb
(
  timer_cb_data_type timer_cb_data
)
{
  IPA_ASSERT(0);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_aggr_force_close_timer_cb

==============================================================================*/
/*!
  @brief
  Detects if aggr force close was applied only on one pipe(PIPE0).
  If yes then set FORCE_CLOSE on Pipe1 as well to complete the test.

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_aggr_force_close_timer_cb
(
  timer_cb_data_type timer_cb_data
)
{
  ipa_hwio_def_ipa_state_aggr_active_u aggr_active; 
  uint32 active_pipe_13_mask = 0x400; //Active Pipe 10 mask
  uint32 mirr_frm_ipa_rcvd_cnt = 0;
  uint32 frm_ipa_rcvd_cnt = 0;
  uint8 log_ch_idx = 0;

  /*! Check for Aggr force close set only on PIPE0. DL_PER_PIPE1
     should still have an open frame.
  */

  /* Log_ch0 -> Phy_ch0 -> PerSim Pipe0(Pipe0) */
  frm_ipa_rcvd_cnt = 
     ipa_test_engine.endp.log_ch[log_ch_idx].phy_ch_ptr->per_sim. \
    from_ipa_wm.total_rcvd_cnt;
  mirr_frm_ipa_rcvd_cnt =
    ipa_test_engine.endp.log_ch[log_ch_idx].phy_ch_ptr->per_sim. \
    mirror_from_ipa_wm.total_rcvd_cnt;
  
  /* Since force close was set on Pipe0 all pkts needs to be recevied. 
     Pkts from IPA has qmap hdr appended to it.
  */    
  IPA_TF_ASSERT_LOG(frm_ipa_rcvd_cnt - (2 * IPA_TEST_ENGINE_QMAP_HDR_SIZE_MAX)== 
                    mirr_frm_ipa_rcvd_cnt, 
                    "Aggr force Close fail on Pipe0!", 0, 0, 0);

  /* Log_ch1 -> Phy_ch1 -> PerSim Pipe1(PIPE11) */
  frm_ipa_rcvd_cnt = 
     ipa_test_engine.endp.log_ch[log_ch_idx + 1].phy_ch_ptr->per_sim. \
     from_ipa_wm.total_rcvd_cnt;
  mirr_frm_ipa_rcvd_cnt =
    ipa_test_engine.endp.log_ch[log_ch_idx + 1].phy_ch_ptr->per_sim. \
    mirror_from_ipa_wm.total_rcvd_cnt;

  /* Only one pkt needs to be recevied back as force close should not be 
     set on this pipe. */
  IPA_TF_ASSERT_LOG((frm_ipa_rcvd_cnt - IPA_TEST_ENGINE_QMAP_HDR_SIZE_MAX)== 
                    (mirr_frm_ipa_rcvd_cnt/2), 
                    "Aggr force Close fail on Pipe1!", 0, 0, 0);

  /* Aggr should still be active on second pipe (PIPE11) with an open frame*/ 
  aggr_active.value = IPA_HAL_HWIO_IN_REG(IPA_STATE_AGGR_ACTIVE);
  IPA_TF_ASSERT_LOG((aggr_active.def.endpoints & active_pipe_13_mask), 
                       "Aggr not active on Pipe17!", 0, 0, 0);
  
  /* Clear timer*/
  timer_clr(&ipa_test_engine.aggr_force_close_timer.timer, T_NONE);

  /* Set force close on Pipe1 (PIPE11) now and complete the test */
  ipa_hal_endp_close_aggr((ipa_hw_pipe_id_e)IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_1_ID);
  
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ind_data_activity

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_ind_data_activity
(
  uint8 bit_pos
)
{
  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
  ipa_test_set_64_bitmask(&ipa_test_engine.curr_test.wm_cb_bit_mask, bit_pos);
  ipa_crit_sect_leave(&ipa_test_engine.crit_sect);

  ipa_signal_set(&ipa_test_engine.wait_signal);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ind_data_activity

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_ind_ul_data_activity(uint64 wm_cb_bit_mask_temp)
{
  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
  ipa_test_engine.curr_test.wm_cb_bit_mask |= wm_cb_bit_mask_temp;
  ipa_crit_sect_leave(&ipa_test_engine.crit_sect);

  ipa_signal_set(&ipa_test_engine.wait_signal);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_set_data_activity

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
INLINE void ipa_test_engine_set_data_activity
(
  uint8 bit_pos
)
{
  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
  ipa_test_set_64_bitmask(&ipa_test_engine.curr_test.wm_cb_bit_mask, bit_pos);
  ipa_crit_sect_leave(&ipa_test_engine.crit_sect);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_sio_rx_wm_each_enqueue_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_sio_rx_wm_each_enqueue_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
)
{
  uint8 log_ch_idx;
  static uint64 wm_cb_bit_mask_temp = 0;
  log_ch_idx = (uint32)cb_data;
  IPA_TF_ASSERT(log_ch_idx < ipa_test_engine.endp.curr_log_ch_cnt);

  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_WM_CB);

  ipa_test_engine.curr_test.num_pkts_rx_to_process = 
		                           ipa_ul_num_pkts_rx_to_process();

  /*Update local temp bit mask*/
  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
  ipa_test_set_64_bitmask(&wm_cb_bit_mask_temp, 
                          log_ch_idx + \
                                      IPA_TEST_ENGINE_UL_BITMASK_POS);
  ipa_crit_sect_leave(&ipa_test_engine.crit_sect);


  /*Set temp bit mask in the global mask when all pkts are arrived*/
  if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != 
            ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {
    /* For deaggr error tests no of pkts could be aggr frames sent out. */
    if (ipa_test_engine.curr_test.num_pkts_rx_to_process >=
        ipa_test_engine.curr_test.deaggr_err_info.frame_cnt)
    {
      ipa_test_engine_ind_ul_data_activity(wm_cb_bit_mask_temp);
      wm_cb_bit_mask_temp = 0;
    }
  }
  else if (ipa_test_engine.curr_test.cfg.ul_holb_enabled == TRUE &&
  	       ipa_test_engine.curr_test.num_pkts_rx_to_process == 10)
  {
    ipa_test_engine_ind_ul_data_activity(wm_cb_bit_mask_temp);
    wm_cb_bit_mask_temp = 0;
  }
  else if (ipa_test_engine.curr_test.num_pkts_rx_to_process >=
           ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts && 
           ipa_test_engine.curr_test.cfg.ul_holb_enabled == FALSE)
  {
    ipa_test_engine_ind_ul_data_activity(wm_cb_bit_mask_temp);
    wm_cb_bit_mask_temp = 0;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_phy_rx_wm_each_enqueue_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_phy_rx_wm_each_enqueue_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
)
{
  uint8 phy_ch_idx;
  static uint64 wm_cb_bit_mask_temp = 0;
  phy_ch_idx = (uint32)cb_data;
  IPA_TF_ASSERT(phy_ch_idx < ipa_test_engine.endp.curr_phy_ch_cnt);

  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_WM_CB);

  ipa_test_engine.curr_test.num_pkts_rx_to_process = 
  	                             ipa_ul_num_pkts_rx_to_process();

  /*Update local tem bit mask*/
  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
  ipa_test_set_64_bitmask(&wm_cb_bit_mask_temp, 
                          phy_ch_idx + \
                                      IPA_TEST_ENGINE_UL_PHY_BITMASK_POS);
  ipa_crit_sect_leave(&ipa_test_engine.crit_sect);

  /*Set temp bit mask in the global mask when all pkts are arrived*/
  if (ipa_test_engine.curr_test.num_pkts_rx_to_process >=
      ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts)
  {
    ipa_test_engine_ind_ul_data_activity(wm_cb_bit_mask_temp);
    wm_cb_bit_mask_temp = 0;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_sio_ds_rx_wm_ne_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_sio_ds_rx_wm_ne_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
)
{
  uint8 bearer_idx;
  bearer_idx = (uint32)cb_data;

  IPA_TF_ASSERT(bearer_idx < ipa_test_engine.wan.curr_bearer_cnt);

  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_WM_CB);

  ipa_test_engine_ind_data_activity(bearer_idx + \
                                    IPA_TEST_ENGINE_DL_BITMASK_POS);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_dpl_rx_wm_ne_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_dpl_rx_wm_ne_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
)
{
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_WM_CB);

  ipa_test_engine_ind_data_activity(IPA_TEST_ENGINE_DPL_BITMASK_POS);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_sim_rx_wm_ne_cb

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_sim_rx_wm_ne_cb
(
  struct dsm_watermark_type_s *wm_ptr,
  void                        *cb_data
)
{
  uint8 sim_idx;
  sim_idx = (uint32)cb_data;

  IPA_TF_ASSERT(sim_idx < ipa_test_engine.endp.curr_phy_ch_cnt);

  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_WM_CB);

  ipa_test_engine_ind_data_activity(sim_idx + \
                                    IPA_TEST_ENGINE_SIM_BITMASK_POS);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_wan_event_cb_fn

==============================================================================*/
/*!
  @brief

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_wan_event_cb_fn
(
  ipa_wan_ds_cb_event_e       cb_event,
  uint8                       uid
)
{
  switch (cb_event) 
  {
    case IPA_WAN_DS_CB_EVENT_BEARER_SUSPENDED:
      ipa_signal_set(&ipa_test_engine.bearer_susp_wait_signal);
      break;

    case IPA_WAN_DS_CB_EVENT_BEARER_REGISTERED:
      ipa_signal_set(&ipa_test_engine.bearer_reg_wait_signal);
      break;

    case IPA_WAN_DS_CB_EVENT_BEARER_DEREGISTERED:
      ipa_signal_set(&ipa_test_engine.bearer_dereg_wait_signal);
      break;

    case IPA_WAN_DS_CB_EVENT_BEARER_ACTIVATED:
      ipa_signal_set(&ipa_test_engine.bearer_active_wait_signal);
      break;

    default:
      IPA_ASSERT(0);
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_compare_rx_wm_n_mirror_wm

==============================================================================*/
/*!
  @brief
  Dequeues a packet from the two watermarks specified (original watermark 
  and mirrored watermark) and compares them.
  *Returns the mimatched byte location when comparison fails

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_compare_rx_wm_n_mirror_wm
(
  dsm_watermark_type        *rx_wm_ptr, 
  dsm_watermark_type        *mirror_wm_ptr, 
  ipa_test_engine_pkt_cmp_s *pkt_cmp_ptr,
  ipa_test_engine_pkt_dst_e pkt_dst,
  uint8                     pkt_dst_idx 
)
{
  uint8 flag = 0;
  uint32 pullup_len = 0;
  
  IPA_MEMSET(pkt_cmp_ptr, 0, sizeof(ipa_test_engine_pkt_cmp_s));
  ipa_test_ret_code_e ret_code;
  ipa_test_engine_pkt_trace_elem_s pkt;

  /* Memset the pkt trace to 0 */
  IPA_MEMSET(&pkt, 0 , sizeof(ipa_test_engine_pkt_trace_elem_s));

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_PKT_CMP);

  pkt.dst = pkt_dst;
  pkt.dst_idx = pkt_dst_idx;
  pkt.src_idx = 0;
  pkt.src = IPA_TEST_ENGINE_PKT_SRC_UNKNOWN;

  while((pkt_cmp_ptr->rcvd_pkt_dsm_ptr = dsm_dequeue(rx_wm_ptr)) != NULL)
  {
    pkt_cmp_ptr->sent_pkt_dsm_ptr = dsm_dequeue(mirror_wm_ptr);
    IPA_TF_ASSERT(pkt_cmp_ptr->sent_pkt_dsm_ptr != NULL);

    pkt_cmp_ptr->sent_pkt_len = 
      dsm_length_packet(pkt_cmp_ptr->sent_pkt_dsm_ptr);
    pkt_cmp_ptr->rcvd_pkt_len = 
      dsm_length_packet(pkt_cmp_ptr->rcvd_pkt_dsm_ptr);
#if 1
    if (pkt_dst == IPA_TEST_ENGINE_PKT_DST_DPL)
    {
      if((pkt_cmp_ptr->rcvd_pkt_dsm_ptr->app_field) !=
          (pkt_cmp_ptr->sent_pkt_dsm_ptr->app_field))
      {
        flag = 1;
        ipa_test_engine.pkt_cmp.compare_failure_id = 1;
        break;
      }
    }
#endif 

    /*! Don't check for app field in case of an error scenario */
    if (ipa_test_filter_get_tcp_ack_only_flag() == FALSE && 
             ipa_test_engine.curr_test.cfg.clat_cfg.err_scenario == IPA_TEST_ENGINE_CLAT_ERR_NONE) 
    {
#if 0
      if (IPA_IPFLTR_GET_DL_FLTR_RESULT_IS_SET(pkt_cmp_ptr->sent_pkt_dsm_ptr) &&
          IPA_IPFLTR_GET_DL_FLTR_RESULT_IS_SET(pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
      {
        /* Check the filter result */
        if (IPA_IPFLTR_GET_DL_FLTR_RESULT(pkt_cmp_ptr->sent_pkt_dsm_ptr) != 
            IPA_IPFLTR_GET_DL_FLTR_RESULT(pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
        {
          flag = 1;
          ipa_test_engine.pkt_cmp.compare_failure_id = 2;
          break;
        }  
      } 
#endif

      if (ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_DL)
      {
        /* Check the IP version */
        if (IPA_DSM_GET_IP_VERSION_PARAM(&pkt_cmp_ptr->sent_pkt_dsm_ptr) != 
          IPA_DSM_GET_IP_VERSION_PARAM(&pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
        {
          flag = 1;
          ipa_test_engine.pkt_cmp.compare_failure_id = 3;
        break;
      }

        /* Check the packet length */
        if (IPA_CSUM_GET_PKT_LEN(&pkt_cmp_ptr->sent_pkt_dsm_ptr) != 
          IPA_CSUM_GET_PKT_LEN(&pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
        {
          flag = 1;
          ipa_test_engine.pkt_cmp.compare_failure_id = 4;
          break;
    }

        /* Verify checksum */
        if (IPA_CSUM_GET_CSUM_COMPUTED(&pkt_cmp_ptr->sent_pkt_dsm_ptr) &&
          IPA_CSUM_GET_CSUM_COMPUTED(&pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
    {
          if (IPA_CSUM_GET_CSUM_VALUE(&pkt_cmp_ptr->sent_pkt_dsm_ptr) != 
            IPA_CSUM_GET_CSUM_VALUE(&pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
    {
            flag = 1;
            ipa_test_engine.pkt_cmp.compare_failure_id = 5;
            break;
          }  

          if (IPA_CSUM_GET_START_OFFSET(&pkt_cmp_ptr->sent_pkt_dsm_ptr) != 
            IPA_CSUM_GET_START_OFFSET(&pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
      {
        flag = 1;
            ipa_test_engine.pkt_cmp.compare_failure_id = 6;
        break;
      }
    }
      }
    }


    if (pkt_cmp_ptr->rcvd_pkt_len != pkt_cmp_ptr->sent_pkt_len)
    {
      ipa_test_engine.pkt_cmp.compare_failure_id = 17;
      flag = 1;
      break;
    }
    
    /*! Check the uCP bit for Frag IPV4 test cases. */
    if (ipa_test_engine.curr_test.cfg.fltr_cfg.fltr_rule_type == IPA_TEST_ENGINE_FLTR_TYPE_CLAT_FRAG_V4)
    {
      if (IPA_IPFLTR_GET_UL_PKT_INFO_XLAT(pkt_cmp_ptr->rcvd_pkt_dsm_ptr) == 0)
      {
        ipa_test_engine.pkt_cmp.compare_failure_id = 18;
        flag = 1;
        break;
      }
    }

    /* Check the priority*/
    if (dsm_item_get_priority(pkt_cmp_ptr->sent_pkt_dsm_ptr) != 
        dsm_item_get_priority(pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
    {
      ipa_test_engine.pkt_cmp.compare_failure_id = 19;
      flag = 1;
      break;
    }

    /* Let's pull up the data for memory comaparison */
    pullup_len = 
         dsm_extract(pkt_cmp_ptr->rcvd_pkt_dsm_ptr, 0, pkt_cmp_ptr->rcvd_aggr_pkt_buf,
                            pkt_cmp_ptr->rcvd_pkt_len);
    IPA_TF_ASSERT(pullup_len == pkt_cmp_ptr->rcvd_pkt_len);

    IPA_CACHE_FLUSH(pkt_cmp_ptr->rcvd_aggr_pkt_buf,pkt_cmp_ptr->rcvd_pkt_len);
	
    pullup_len = 
        dsm_extract(pkt_cmp_ptr->sent_pkt_dsm_ptr, 0, pkt_cmp_ptr->sent_aggr_pkt_buf,
                            pkt_cmp_ptr->sent_pkt_len);
    IPA_TF_ASSERT(pullup_len == pkt_cmp_ptr->sent_pkt_len);

    /*Compare the received IP Packet header and payload with the original 
      packet values.*/
    ret_code = 
      ipa_test_compare_mem(pkt_cmp_ptr->rcvd_pkt_len, 
                           pkt_cmp_ptr->sent_aggr_pkt_buf, 
                           pkt_cmp_ptr->rcvd_aggr_pkt_buf, 
                           &pkt_cmp_ptr->no_match_locn);
    if(ret_code != IPA_TEST_RET_CODE_SUCCESS)
    {
       ipa_test_engine.pkt_cmp.compare_failure_id = 21;
       flag = 1;
       break;
    }
    else
    {
      /*! @todo We need to remove ethernet or QOS header len before storing
       the trace */
      pkt.filter_result = IPA_IPFLTR_GET_DL_FLTR_RESULT(pkt_cmp_ptr->rcvd_pkt_dsm_ptr);
      pkt.filter_priority = dsm_item_get_priority(pkt_cmp_ptr->rcvd_pkt_dsm_ptr);
      pkt.pkt_len = pkt_cmp_ptr->rcvd_pkt_len;

      if(IPA_TEST_GET_IP_VERSION(pkt_cmp_ptr->rcvd_aggr_pkt_buf) == 4)
      {
        pkt.ip_ver = IPA_IP_TYPE_IPV4;
      }
      else
      {
        pkt.ip_ver = IPA_IP_TYPE_IPV6;
      }
      ipa_test_engine_store_rx_pkt_trace(&pkt);

      dsm_free_packet(&pkt_cmp_ptr->rcvd_pkt_dsm_ptr);
      dsm_free_packet(&pkt_cmp_ptr->sent_pkt_dsm_ptr);
    }

	 if ((ipa_test_engine.curr_test.cfg.dl_holb_enabled == TRUE || 
	 	 ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled == TRUE) &&
	 	 IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL == pkt_dst &&
	 	 ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB)
     {
       ipa_test_engine.pkt_count_for_holb++;

       if (ipa_test_engine.pkt_count_for_holb > ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts)
       {
	     IPA_ASSERT(0);
       }
     }
  }

  if (ipa_test_engine.curr_test.cfg.dl_holb_enabled == TRUE && 
  	  IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL == pkt_dst &&
  	  ipa_test_engine.pkt_count_for_holb == ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts)
  {
	ipa_test_per_sim_dsm_reinit(0);
	ipa_cfg.dl.is_holb_test_enabled = FALSE;
	ipa_test_engine.curr_test.cfg.dl_holb_enabled = FALSE;
	ipa_test_engine.pkt_count_for_holb = 0;
  }
  else if(ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled == TRUE && 
  	      IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL == pkt_dst &&
  	      ipa_test_engine.pkt_count_for_holb == ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts && 
  	      ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB)
  {
    ipa_cfg.dl.is_hw_rep_holb_test_enabled = FALSE;
	ipa_test_engine.curr_test.cfg.is_hw_replication_enabled = TRUE;
	ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled = FALSE;
	ipa_test_engine.pkt_count_for_holb = 0;
    ipa_test_per_sim_dsm_reinit(1);
  }
  else if(ipa_test_engine.curr_test.cfg.ul_holb_enabled == TRUE &&
  	 	   IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL == pkt_dst &&
  	       ipa_test_engine.curr_test.num_pkts_tot == 10)
  {
    ret_code = ipa_test_engine_generate_ip_pkts();
	
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    ipa_test_engine_dl_tx_pkts();

	
	ret_code = ipa_test_engine_destroy_ip_pkts();
	IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
  
  if (flag == 0) 
  {
     return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
     return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_compare_rx_pkts_n_mirror_wm

==============================================================================*/
/*!
  @brief
  Dequeues a packet from the two watermarks specified (original watermark 
  and mirrored watermark) and compares them.
  *Returns the mimatched byte location when comparison fails

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_compare_rx_pkts_n_mirror_wm
(
  dsm_watermark_type            *mirror_wm_ptr, 
  ipa_test_engine_pkt_cmp_s     *pkt_cmp_ptr,
  ipa_test_engine_ip_pkt_info_s *pkt_info_arr,
  uint16                        num_pkts_rx,
  ipa_test_engine_pkt_dst_e     pkt_dst,
  uint8                         pkt_dst_idx 
)
{
  uint8 flag = 0;
  uint32 pullup_len = 0;
  ipa_test_ret_code_e ret_code;
  uint16 i = 0;
  ipa_test_engine_pkt_trace_elem_s pkt;

  /* Memset the pkt trace to 0 */
  IPA_MEMSET(&pkt, 0 , sizeof(ipa_test_engine_pkt_trace_elem_s));

  IPA_MEMSET(pkt_cmp_ptr, 0, sizeof(ipa_test_engine_pkt_cmp_s));

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_PKT_CMP);

  pkt.dst = pkt_dst;
  pkt.dst_idx = pkt_dst_idx;
  pkt.src_idx = 0;
  pkt.src = IPA_TEST_ENGINE_PKT_SRC_UNKNOWN;
  pkt_cmp_ptr->rcvd_pkt_dsm_ptr = NULL;

  while(i < num_pkts_rx)
  {
    pkt_cmp_ptr->sent_pkt_dsm_ptr = dsm_dequeue(mirror_wm_ptr);
    IPA_TF_ASSERT(pkt_cmp_ptr->sent_pkt_dsm_ptr != NULL);

    pkt_cmp_ptr->sent_pkt_len = 
      dsm_length_packet(pkt_cmp_ptr->sent_pkt_dsm_ptr);

    pkt_cmp_ptr->rcvd_pkt_len = pkt_info_arr[i].pkt_len;

    if (pkt_cmp_ptr->rcvd_pkt_len != pkt_cmp_ptr->sent_pkt_len)
    {
      ipa_test_engine.pkt_cmp.compare_failure_id = 25;
      flag = 1;
      break;
    }
#if 0 
    /* Check the Mux ID received */
    if ((pkt_info_arr[i].mux_id != 0) && 
        (pkt_cmp_ptr->sent_pkt_dsm_ptr->kind != pkt_info_arr[i].mux_id))
    {
      flag = 1;
      break;
    }
#endif

    /* Let's pull up the data for memory comaparison */
    pullup_len = 
      dsm_extract(pkt_cmp_ptr->sent_pkt_dsm_ptr, 0, pkt_cmp_ptr->sent_pkt_buf,
                            pkt_cmp_ptr->sent_pkt_len);
    IPA_TF_ASSERT(pullup_len == pkt_cmp_ptr->sent_pkt_len);

    IPA_MEMCPY(pkt_cmp_ptr->rcvd_pkt_buf, pkt_info_arr[i].pkt_buf_ptr, 
               pkt_cmp_ptr->rcvd_pkt_len);


	 IPA_CACHE_FLUSH(pkt_cmp_ptr->rcvd_pkt_buf,pkt_cmp_ptr->rcvd_pkt_len);
    /*Compare the received IP Packet header and payload with the original 
      packet values.*/
    ret_code = 
      ipa_test_compare_mem(pkt_cmp_ptr->rcvd_pkt_len, 
                           pkt_cmp_ptr->sent_pkt_buf, 
                           pkt_cmp_ptr->rcvd_pkt_buf, 
                           &pkt_cmp_ptr->no_match_locn);
    if(ret_code != IPA_TEST_RET_CODE_SUCCESS)
    {
       ipa_test_engine.pkt_cmp.compare_failure_id = 20;
       flag = 1;
       break;
    }
    else
    {
      pkt.filter_result = 0;
      pkt.filter_priority = 0;
      pkt.pkt_len = pkt_cmp_ptr->rcvd_pkt_len;
      if(IPA_TEST_GET_IP_VERSION(pkt_cmp_ptr->rcvd_pkt_buf) == 4)
      {
        pkt.ip_ver = IPA_IP_TYPE_IPV4;
      }
      else
      {
        pkt.ip_ver = IPA_IP_TYPE_IPV6;
      }
      ipa_test_engine_store_rx_pkt_trace(&pkt);

      dsm_free_packet(&pkt_cmp_ptr->sent_pkt_dsm_ptr);
    }
    i++;
  }

  if (flag == 0) 
  {
     return IPA_TEST_RET_CODE_SUCCESS;
  }
  else
  {
     return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_init

==============================================================================*/
/*!
  @brief
  Initialize the Test Engine 

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_init(void)
{
  if (ipa_test_engine.init_done == FALSE)
  {
    /* Initialize the mutex*/
    ipa_crit_sect_init(&ipa_test_engine.crit_sect);

    ipa_crit_sect_init(&ipa_test_engine.state_trace.crit_sect);

    /* Define timer to be used in the test case to detect any hardware stalls */
    timer_group_set_deferrable(&ipa_test_engine.watchdog_timer.timer_group, 
                               FALSE);
    timer_def((timer_ptr_type)&ipa_test_engine.watchdog_timer.timer,
              &ipa_test_engine.watchdog_timer.timer_group,
              NULL, 0, (timer_t1_cb_type)ipa_test_engine_watchdog_timer_cb, 0);

    timer_group_set_deferrable(&ipa_test_engine.ds_evnt_timer.timer_group, 
                               FALSE);
    timer_def((timer_ptr_type)&ipa_test_engine.ds_evnt_timer.timer,
                    &ipa_test_engine.ds_evnt_timer.timer_group,
                    NULL, 0, (timer_t1_cb_type)ipa_test_engine_ds_evnt_timer_cb,
                     (timer_cb_data_type)0);
    
    /* Define timer to be used in the test case for IPA ZIP events to detect any hardware stalls */
    timer_group_set_deferrable(&ipa_test_engine.zip_resp_timer.timer_group, 
                               FALSE);
    timer_def((timer_ptr_type)&ipa_test_engine.zip_resp_timer.timer,
                    &ipa_test_engine.zip_resp_timer.timer_group,
                    NULL, 0, (timer_t1_cb_type)ipa_test_engine_zip_resp_timer_cb,
                     (timer_cb_data_type)0);
    
    /* Initialize the signal for waiting for packets to come back */
    ipa_signal_init(&ipa_test_engine.wait_signal, rex_self(),
                     (uint32)IPA_TEST_ENGINE_WAIT_SIG);
    /* Initialize the signal for waiting for endp cfg to come back */
    ipa_signal_init(&ipa_test_engine.endp_wait_signal, rex_self(), 
                    (uint32)IPA_TEST_ENGINE_ENDP_WAIT_SIG);
     /* Initialize the signal for waiting for CLAT config to come back */
    ipa_signal_init(&ipa_test_engine.clat_cfg_signal, rex_self(), 
                    (uint32)IPA_TEST_ENGINE_CLAT_CONFIG_SIG);
    /* Initialize the signal for waiting for CLAT config to come back */
    ipa_signal_init(&ipa_test_engine.clat_assoc_sio_signal, rex_self(), 
                    (uint32)IPA_TEST_ENGINE_CLAT_ASSOC_SIO_SIG);
    /* Initialize the signal for waiting for CLAT config to come back */
    ipa_signal_init(&ipa_test_engine.clat_dereg_signal, rex_self(), 
                    (uint32)IPA_TEST_ENGINE_CLAT_DEREG_SIG);
    /* Initialize the signal for waiting for CLAT translation to come back */
    ipa_signal_init(&ipa_test_engine.clat_translation_signal, rex_self(), 
                    (uint32)IPA_TEST_ENGINE_CLAT_TRANSLATION_SIG);
    /* Initialize the signal for waiting for bearer susp cb to come back */
    ipa_signal_init(&ipa_test_engine.bearer_susp_wait_signal, rex_self(), 
                    (uint32)IPA_TEST_ENGINE_BEARER_SUSP_SIG);
    /* Initialize the signal for waiting for bearer susp cb to come back */
    ipa_signal_init(&ipa_test_engine.bearer_active_wait_signal, rex_self(), 
                    (uint32)IPA_TEST_ENGINE_BEARER_ACTIVE_SIG);
    /* Initialize the signal for waiting for bearer susp cb to come back */
    ipa_signal_init(&ipa_test_engine.bearer_dereg_wait_signal, rex_self(), 
                    (uint32)IPA_TEST_ENGINE_BEARER_DEREG_SIG);
    /* Initialize the signal for waiting for bearer susp cb to come back */
    ipa_signal_init(&ipa_test_engine.bearer_reg_wait_signal, rex_self(),
                    (uint32)IPA_TEST_ENGINE_BEARER_REG_SIG);
    /* Initialize the signal for waiting ZIP registration done cb to come back */
    ipa_signal_init(&ipa_test_engine.zip_reg_done_signal, rex_self(),
                    (uint32)IPA_TEST_ENGINE_ZIP_REG_DONE_SIG);
    /* Initialize the signal for waiting for dictionaly load complete cb to come back */
    ipa_signal_init(&ipa_test_engine.zip_dict_load_done_signal, rex_self(),
                    (uint32)IPA_TEST_ENGINE_ZIP_DICT_LOAD_DONE_SIG);
    /* Initialize the signal for waiting for dictionaly load complete cb to come back */
    ipa_signal_init(&ipa_test_engine.zip_resp_rcvd_signal, rex_self(),
                    (uint32)IPA_TEST_ENGINE_ZIP_RESP_RCVD_SIG);
            /* Initialize the signal for waiting for dictionaly load complete cb to come back */
    ipa_signal_init(&ipa_test_engine.zip_err_resp_rcvd_signal, rex_self(),
                    (uint32)IPA_TEST_ENGINE_ZIP_ERR_RESP_RCVD_SIG);
    
    /* First generate susbystem ID's for the system */
    ipa_test_engine_generate_subs_ids();

    /* Define timers needed for other tests once at init. */

    /*! Flow Control test timer */
    timer_group_set_deferrable(&ipa_test_engine.flow_ctl_timer.timer_group, 
                               FALSE);
    timer_def((timer_ptr_type)&ipa_test_engine.flow_ctl_timer.timer,
              &ipa_test_engine.flow_ctl_timer.timer_group,
              NULL, 0, (timer_t1_cb_type)ipa_test_engine_flow_ctl_timer_cb, 0);

    /*! Aggr force close timer */
    timer_group_set_deferrable(&ipa_test_engine.aggr_force_close_timer.timer_group, 
                               FALSE);
    timer_def((timer_ptr_type)&ipa_test_engine.aggr_force_close_timer.timer,
              &ipa_test_engine.aggr_force_close_timer.timer_group, NULL, 0, 
              (timer_t1_cb_type)ipa_test_engine_aggr_force_close_timer_cb, 0);

    ipa_test_engine.init_done = TRUE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_endp_wait_cfg_end

==============================================================================*/
/*!
  @brief
  Wait for endp cfg to be done for all the SIO mapping 
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_endp_wait_cfg_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.endp_wait_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.watchdog_timer.timer , 
            IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL_SIO, 0, T_MSEC);

  /* Wait till we get cb's for all the SIO config's. 
  Do we want to wait forever or assert with a timeout? */
  while (ipa_test_engine.endp.sio_map_cb.num_pend_sio_cb != 0) 
  {
    set_signals = ipa_signal_mask_wait(&sig_mask);
    IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.endp_wait_signal));
    ipa_signal_clear(&ipa_test_engine.endp_wait_signal);
  }

  /* Stop the timer after receiving all the cfg callbacks */
  timer_clr(&ipa_test_engine.watchdog_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_cfg_end

==============================================================================*/
/*!
  @brief
  Wait for CLAT cfg to be done on a particular SIO
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_clat_cfg_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.clat_cfg_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.watchdog_timer.timer , 
            IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL_SIO, 0, T_MSEC);

  /* Wait till we get cb's for CLAT_ASSOC_SIO.
  Do we want to wait forever or assert with a timeout? */

  /*! Loop until both clat_reg and assoc_sio is completed for every 
       clat_context_cfg */
  while (ipa_test_engine.clat.clat_cfg_cb.num_pend_clat_cfg_cb != 0) 
  {
    set_signals = ipa_signal_mask_wait(&sig_mask);
    IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.clat_cfg_signal));
    ipa_signal_clear(&ipa_test_engine.clat_cfg_signal);
  }

  /* Stop the timer after receiving all the cfg callbacks */
  timer_clr(&ipa_test_engine.watchdog_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_cfg_end

==============================================================================*/
/*!
  @brief
  Wait for CLAT cfg to be done on a particular SIO
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_clat_assoc_sio_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.clat_assoc_sio_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.watchdog_timer.timer , 
            IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL_SIO, 0, T_MSEC);

  /* Wait till we get cb's for CLAT_ASSOC_SIO.
  Do we want to wait forever or assert with a timeout? */

  /*! Loop until both clat_reg and assoc_sio is completed for every 
       clat_context_cfg */
  while (ipa_test_engine.clat.clat_cfg_cb.num_pend_assoc_sio_cb != 0) 
  {
    set_signals = ipa_signal_mask_wait(&sig_mask);
    IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.clat_assoc_sio_signal));
    ipa_signal_clear(&ipa_test_engine.clat_assoc_sio_signal);
  }

  /* Stop the timer after receiving all the cfg callbacks */
  timer_clr(&ipa_test_engine.watchdog_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_translation_wait_end

==============================================================================*/
/*!
  @brief
  Wait for CLAT translation CB. 
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_clat_translation_wait_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.clat_translation_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.watchdog_timer.timer , 
            IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL_SIO, 0, T_MSEC);

  set_signals = ipa_signal_mask_wait(&sig_mask);
  IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.clat_translation_signal));
  ipa_signal_clear(&ipa_test_engine.clat_translation_signal);
  
  /* Stop the timer after receiving all the cfg callbacks */
  timer_clr(&ipa_test_engine.watchdog_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ul_clat_ihl_exception_cb

==============================================================================*/
/*!
  @brief
  Validate CLAT ihl exception scenarios. CB invoked from IPA driver when excpetion 
  is detected in the pkt status.

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_ul_clat_ihl_exception_cb
(
  ipa_hw_pkt_status_s  *status_ptr,
  dsm_item_type        *pkt_dsm_ptr,
  dsm_watermark_type   *sio_rx_wm_ptr
)
{
  uint32 dsm_dup_len;
  dsm_item_type *exception_dsm_ptr;

  if (IPA_TEST_ENGINE_CLAT_ERR_IPV6_EXT_HDR_EXCEEDED ==
    ipa_test_engine.curr_test.cfg.clat_cfg.err_scenario)
  {
    /*! We are expecting a IHL exception to be received */
    IPA_TF_ASSERT((status_ptr->exception == IPA_HW_PKT_STATUS_EXCEPTION_IHL) ||
                  (status_ptr->exception == 9));
  }

  dsm_dup_len = 
            dsm_dup_packet(&exception_dsm_ptr, 
                           pkt_dsm_ptr, 0,status_ptr->pkt_len);

 IPA_TF_ASSERT_LOG(dsm_dup_len == status_ptr->pkt_len, 
                          "dsm dup fail!", 0, 0, 0);

  /*! Just to debug if the callback is being called. To be removed later*/
  ipa_test_engine.curr_test.cfg.clat_cfg.exception_cnt++;

  if (sio_rx_wm_ptr != NULL)
  {
    dsm_pullup(&exception_dsm_ptr, NULL, 8);
    dsm_enqueue(sio_rx_wm_ptr, &exception_dsm_ptr);
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_bearer_susp_evnt_wait_end

==============================================================================*/
/*!
  @brief
  Wait for bearer suspend CB. 
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_bearer_susp_evnt_wait_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.bearer_susp_wait_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.ds_evnt_timer.timer,
            IPA_TEST_ENGINE_DS_EVNT_DETECT_INTERVAL, 0, T_MSEC);

  set_signals = ipa_signal_mask_wait(&sig_mask);
  IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.bearer_susp_wait_signal));
  ipa_signal_clear(&ipa_test_engine.bearer_susp_wait_signal);
  
  /* Stop the timer after receiving the event callback */
  timer_clr(&ipa_test_engine.ds_evnt_timer.timer, T_NONE);
}
/*==============================================================================

  FUNCTION:  ipa_test_engine_bearer_reg_evnt_wait_end

==============================================================================*/
/*!
  @brief
  Wait for bearer register callback
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_bearer_reg_evnt_wait_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.bearer_reg_wait_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.ds_evnt_timer.timer,
            IPA_TEST_ENGINE_DS_EVNT_DETECT_INTERVAL, 0, T_MSEC);

  set_signals = ipa_signal_mask_wait(&sig_mask);
  IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.bearer_reg_wait_signal));
  ipa_signal_clear(&ipa_test_engine.bearer_reg_wait_signal);
  
  /* Stop the timer after receiving the event callback */
  timer_clr(&ipa_test_engine.ds_evnt_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_bearer_dereg_evnt_wait_end

==============================================================================*/
/*!
  @brief
  Wait for bearer deregister event callback
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_bearer_dereg_evnt_wait_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.bearer_dereg_wait_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.ds_evnt_timer.timer,
            IPA_TEST_ENGINE_DS_EVNT_DETECT_INTERVAL, 0, T_MSEC);

  set_signals = ipa_signal_mask_wait(&sig_mask);
  IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.bearer_dereg_wait_signal));
  ipa_signal_clear(&ipa_test_engine.bearer_dereg_wait_signal);
  
  /* Stop the timer after receiving the event callback */
  timer_clr(&ipa_test_engine.ds_evnt_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_bearer_active_evnt_wait_end

==============================================================================*/
/*!
  @brief
  Wait for endp cfg to be done for all the SIO mapping 
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_bearer_active_evnt_wait_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.bearer_active_wait_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.ds_evnt_timer.timer,
            IPA_TEST_ENGINE_DS_EVNT_DETECT_INTERVAL, 0, T_MSEC);

  set_signals = ipa_signal_mask_wait(&sig_mask);
  IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.bearer_active_wait_signal));
  ipa_signal_clear(&ipa_test_engine.bearer_active_wait_signal);
  
  /* Stop the timer after receiving the event callback */
  timer_clr(&ipa_test_engine.ds_evnt_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_reg_done_evnt_wait_end

==============================================================================*/
/*!
  @brief
  Wait for IPA ZIP registration to be completed.
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_zip_reg_done_evnt_wait_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.zip_reg_done_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.zip_resp_timer.timer,
            IPA_TEST_ENGINE_ZIP_RESP_INTERVAL, 0, T_MSEC);

  set_signals = ipa_signal_mask_wait(&sig_mask);
  IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.zip_reg_done_signal));
  ipa_signal_clear(&ipa_test_engine.zip_reg_done_signal);
  
  /* Stop the timer after receiving the event callback */
  timer_clr(&ipa_test_engine.zip_resp_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_dict_load_evnt_wait_end

==============================================================================*/
/*!
  @brief
  Wait for IPA ZIP Dictionary to be loaded. 
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_zip_dict_load_evnt_wait_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.zip_dict_load_done_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.zip_resp_timer.timer,
            IPA_TEST_ENGINE_ZIP_RESP_INTERVAL, 0, T_MSEC);

  set_signals = ipa_signal_mask_wait(&sig_mask);
  IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.zip_dict_load_done_signal));
  ipa_signal_clear(&ipa_test_engine.zip_dict_load_done_signal);
  
  /* Stop the timer after receiving the event callback */
  timer_clr(&ipa_test_engine.zip_resp_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_endp_log_ch_cfg

==============================================================================*/
/*!
  @brief
  Configures the IPA driver with a logical channel

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_endp_log_ch_cfg
(
  ipa_sio_endpoint_config_s *sio_cfg_ptr,
  uint8                     phy_ch_idx,
  uint8                     log_ch_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_FAILURE;
  sio_stream_id_type stream_id = 0;

  /* Input Params checking first */
  IPA_TF_ASSERT(sio_cfg_ptr != NULL);
  IPA_TF_ASSERT(log_ch_idx <= IPA_TEST_ENGINE_LOG_CH_NUM_MAX);

  /* First check if the SIO cfg aggr protocols are supported */
  ret_code = ipa_test_check_sio_aggr_prot_support(sio_cfg_ptr, 
             ipa_test_engine.endp.phy_ch[phy_ch_idx].ic_type);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  /* Open the logical sio ports first */
  ret_code = ipa_test_open_log_sio_port(log_ch_idx, 
                                        &ipa_test_engine.endp.log_ch[log_ch_idx]);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  /**-- Internal Data Base Update --**/
  ipa_test_engine.endp.log_ch[log_ch_idx].idx = log_ch_idx;
  /* Set physical channel ptr to which this logical channel belongs */
  ipa_test_engine.endp.log_ch[log_ch_idx].phy_ch_ptr = 
    &ipa_test_engine.endp.phy_ch[phy_ch_idx];
  /* Now set the cfg ptr in the logical channel for reference */
  ipa_test_engine.endp.log_ch[log_ch_idx].endp_cfg_ptr = sio_cfg_ptr; 
  /**-- Internal Data Base Update --**/

  /* Store this stream ID in pend cb array before we set sio mapping to driver */
  stream_id = ipa_test_engine.endp.log_ch[log_ch_idx].stream_id;

  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
  IPA_TF_ASSERT(ipa_test_engine.endp.sio_map_cb.pend_stream_id_arr[stream_id] 
                == FALSE);
  /* Now set this stream ID to TRUE in pending cb array */
  ipa_test_engine.endp.sio_map_cb.pend_stream_id_arr[stream_id] = TRUE;
  ipa_test_engine.endp.sio_map_cb.num_pend_sio_cb++;
  ipa_crit_sect_leave(&ipa_test_engine.crit_sect);
  
  if (ipa_test_engine.endp.log_ch[log_ch_idx].endp_cfg_ptr->dl_config.hdr_type
     == IPA_SIO_HDR_TYPE_ETHERNET)
  { 
   
    ipa_test_engine.endp.log_ch[log_ch_idx].endp_cfg_ptr->dl_config.params.ether \
        .ipv4_hdr_present = FALSE;
    ipa_test_engine.endp.log_ch[log_ch_idx].endp_cfg_ptr->dl_config.params.ether \
        .ipv6_hdr_present = FALSE;
 
    /* Set the SIO mapping now */
    ipa_test_engine.endp.log_ch[log_ch_idx].action = IPA_SIO_CONFIG_ACTION_ADD;
    ret_code = ipa_test_set_sio_cfg_map(&ipa_test_engine.endp.log_ch[log_ch_idx],
                                      sio_cfg_ptr);

    ipa_sleep(2);

    ipa_test_engine.endp.log_ch[log_ch_idx].endp_cfg_ptr->dl_config.params.ether \
          .ipv4_hdr_present = TRUE;
    ipa_test_engine.endp.log_ch[log_ch_idx].endp_cfg_ptr->dl_config.params.ether \
          .ipv6_hdr_present = TRUE;
   
    /* Set the SIO mapping now */
    ipa_test_engine.endp.log_ch[log_ch_idx].action = IPA_SIO_CONFIG_ACTION_ETHERNET_HDR_UPDATE;

    ret_code = ipa_test_set_sio_cfg_map(&ipa_test_engine.endp.log_ch[log_ch_idx],
                                      sio_cfg_ptr);
  }
  else
  {

    ipa_test_engine.endp.log_ch[log_ch_idx].action = IPA_SIO_CONFIG_ACTION_ADD;
    /* Set the SIO mapping now */
    ret_code = ipa_test_set_sio_cfg_map(&ipa_test_engine.endp.log_ch[log_ch_idx],
                                      sio_cfg_ptr);
  }

  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(ipa_test_engine_sleep_time);
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_endp_phy_ch_cfg

==============================================================================*/
/*!
  @brief
  Configures the IPA driver with a phyical channel

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_endp_phy_ch_cfg
(
  ipa_sio_endpoint_config_s *sio_cfg_ptr,
  boolean                   is_b2b_mode,
  uint8                     num_log_ch,   
  uint8                     phy_ch_idx
)
{
  uint8 i = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_FAILURE;

  /* Input Params checking first */
  IPA_TF_ASSERT(sio_cfg_ptr != NULL);
  IPA_TF_ASSERT(phy_ch_idx <= IPA_TEST_ENGINE_PHY_CH_NUM_MAX);
  IPA_TF_ASSERT((num_log_ch <= IPA_TEST_ENGINE_PER_SIM_LOG_CH_NUM_MAX) &&
                (num_log_ch  > 0));

  /*No muxing allowed for these endpoint types */
  if ((sio_cfg_ptr->ul_config.hdr_type == IPA_SIO_HDR_TYPE_DIRECT_DMA) ||
      (sio_cfg_ptr->ul_config.hdr_type == IPA_SIO_HDR_TYPE_NONE) ||
      (sio_cfg_ptr->ul_config.hdr_type == IPA_SIO_HDR_TYPE_ETHERNET))
  {
    IPA_TF_ASSERT(num_log_ch == 1);
  }

  /* Open the physical sio port first */
  ret_code = ipa_test_open_phy_sio_port(phy_ch_idx, 
                                        &ipa_test_engine.endp.phy_ch[phy_ch_idx]);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  /* Register the SIM per's with the driver next */
  if (TRUE == is_b2b_mode)
  {
    /*Only 1 B2B pipe config allowed*/
    IPA_TF_ASSERT(ipa_test_engine.endp.curr_b2b_cnt == 0);

    ipa_test_engine.endp.phy_ch[phy_ch_idx].per_sim.b2b_mode = is_b2b_mode;
    ipa_test_engine.endp.curr_b2b_cnt ++;
  }

  ret_code = ipa_test_register_per_sim(phy_ch_idx, 
                                       &ipa_test_engine.endp.phy_ch[phy_ch_idx],
                                       ipa_test_engine.curr_test.cfg.endp_cfg \
                                       .sim_tx_delay);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_test_engine.endp.phy_ch[phy_ch_idx].ic_type = ipa_test_engine.curr_test.cfg.endp_cfg.ic_type;
  
  /* Set the endpoint mapping in the driver */
  ret_code = ipa_test_set_endp_map(phy_ch_idx, 
                                   &ipa_test_engine.endp.phy_ch[phy_ch_idx],
                                   ipa_test_engine.curr_test.cfg.is_hw_replication_enabled);

  
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE && phy_ch_idx == 1)
  {
  	IPA_MSG ("No Logical Channel mapping needed");	
  }
  else
  {
  	/*if(sio_cfg_ptr->ul_config.hdr_type == IPA_SIO_HDR_TYPE_DIRECT_DMA)
  	{
  		IPA_HAL_HWIO_OUT_REG_n(IPA_ENDP_INIT_SEQ_n, (ipa_hw_pipe_id_e)ipa_test_engine.endp.phy_ch[phy_ch_idx].per_ep_map.per_cons_pipe_id, 
								0x0);
  	}
  	else
  	{
  		IPA_HAL_HWIO_OUT_REG_n(IPA_ENDP_INIT_SEQ_n, (ipa_hw_pipe_id_e)ipa_test_engine.endp.phy_ch[phy_ch_idx].per_ep_map.per_cons_pipe_id, 
						0x0);
  	}*/

  /* Now open all the logical channels on this physical channel with the driver */
  for (i = 0; i < num_log_ch; i++)
  {
    ret_code = ipa_test_engine_endp_log_ch_cfg(sio_cfg_ptr, phy_ch_idx, 
                                               ipa_test_engine.endp.curr_log_ch_cnt);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

    /**-- Internal Data Base Update --**/
    ipa_test_engine.endp.phy_ch[phy_ch_idx].idx = phy_ch_idx;
    /* Assign this logical channel ptr to the phy channel for reference */
    ipa_test_engine.endp.phy_ch[phy_ch_idx].log_ch_ptr_arr[i] = 
      &ipa_test_engine.endp.log_ch[ipa_test_engine.endp.curr_log_ch_cnt];

    /* Increment the number of logical channels on this physical port */
    ipa_test_engine.endp.phy_ch[phy_ch_idx].log_ch_cnt++;
    /**-- Internal Data Base Update --**/

    /* Increment the total number of logical channels */
    ipa_test_engine.endp.curr_log_ch_cnt++;
  }

  }
  /* Initilialize the histogram stats for peripheral pipe */
  for (i = 0; i <= IPA_STATS_AGGR_BUCKET_MAX_PKT_NUM; i++) 
  {
    ipa_test_engine.curr_test.aggr[phy_ch_idx].aggr_hist \
      [ipa_cfg_aggr_stats_bucket_map[i]].num_pkts_per_frame = i;
    ipa_test_engine.curr_test.aggr[phy_ch_idx].deaggr_hist \
      [ipa_cfg_aggr_stats_bucket_map[i]].num_pkts_per_frame = i;
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_endp_cfg

==============================================================================*/
/*!
  @brief
  Configures the IPA driver with the ENDP input params 

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_endp_cfg
(
  ipa_test_engine_endp_cfg_s *endp_cfg_ptr
)
{
  uint8 i = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_FAILURE;
  ipa_err_code_e err_code;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_ENDP_CFG);

  /* Input Params checking first */
  IPA_TF_ASSERT(endp_cfg_ptr != NULL);
  IPA_TF_ASSERT((endp_cfg_ptr->num_per_sim <= IPA_TEST_ENGINE_PER_SIM_NUM_MAX) &&
                (endp_cfg_ptr->num_per_sim > 0));

  /* Before we start with any config. Set up the sio cfg done callback with 
  driver */
  err_code = 
    ipa_sioi_register_ep_config_done_cb(ipa_test_engine_endp_sio_map_cb_fn,
                                        (uint32)&ipa_test_engine.endp.sio_map_cb);
  IPA_TF_ASSERT(err_code == IPA_SUCCESS);

  /* 1. Initialize all the Per SIM endpoints one by one.
     2. Open Physical ports for each of the endpoints 
     3. Open as many logical ports requested by the test for each endpoint
     4. Send SIO mapping for each of the logical port
   */
  for (i = 0; i < endp_cfg_ptr->num_per_sim; i++)
  {
    ret_code = ipa_test_engine_endp_phy_ch_cfg(&endp_cfg_ptr->sim_endp_cfg[i], 
                                               endp_cfg_ptr->sim_endp_b2b_cfg[i],
                                               endp_cfg_ptr->num_log_ch[i],
                                               i);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

    /* Increment the total number of physical channels */
    ipa_test_engine.endp.curr_phy_ch_cnt++;
  }

  /* Now wait for all endpoint cfg's to finish in the driver */
  ipa_test_engine_endp_wait_cfg_end();

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_wan_bearer_cfg

==============================================================================*/
/*!
  @brief
  Configures the IPA driver with single bearer

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_wan_bearer_cfg
(
  uint8                 bearer_idx,
  uint8                 is_bearer_bridged,
  ipa_wan_bearer_tech_e bearer_tech
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_FAILURE;
  uint8 log_ch_idx, subs_idx;

  IPA_TF_ASSERT(bearer_idx <= IPA_TEST_ENGINE_BEARER_NUM_MAX);
  /* Generate a unique ID for this beaerer first */
  /* Store it in our UID table to ensure that we have unique ID's for all
  bearer */
  ipa_test_engine.wan.uid_arr[bearer_idx] =
    ipa_test_generate_unique_random_num(IPA_WAN_MAX_UIDS - 1, 0, 
                                        ipa_test_engine.wan.uid_arr, 
                                        bearer_idx);

  if (IPA_TEST_ENGINE_RX_PKT_WAIT_AGGR_FORCE_CLOSE_PIPES ==
      ipa_test_engine.curr_test.cfg.rx_pkt_test_type)
  {
    /* Tie the bearer to a different log ch each time since we want to 
       excercise both the phy channels in this test case */
    log_ch_idx = bearer_idx;
  }
  else
  {
    /* Now tie this bearer to a random logical channel already opened in the 
       system */

	if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled  == TRUE)
		log_ch_idx = 0;
	else
        log_ch_idx = ipa_test_generate_random_num(ipa_test_engine.endp.curr_log_ch_cnt - 1, 0);
  }

  /* Generate Subsys ID index now */
  subs_idx =  
    ipa_test_generate_random_num(IPA_TEST_ENGINE_SUBS_NUM_MAX - 1, 0);

  ret_code = 
    ipa_test_set_bearer_cfg(bearer_idx,
                            &ipa_test_engine.wan.bearer[bearer_idx],
                            &ipa_test_engine.endp.log_ch[log_ch_idx],
                            ipa_test_engine.wan.uid_arr[bearer_idx], 
                            ipa_test_engine.subs_id_arr[subs_idx],
                            is_bearer_bridged, 
                            ipa_test_engine.curr_test.cfg.is_dpl_enabled,
                            bearer_tech,
                            ipa_test_engine.endp.log_ch[log_ch_idx].is_clat);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  /**-- Internal Data Base Update --**/
  ipa_test_engine.wan.bearer[bearer_idx].idx = bearer_idx;
  ipa_test_engine.wan.bearer[bearer_idx].bearer_tech = bearer_tech;
  /* Assign this logical channel ptr to the bearer for reference */
  ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr = 
    &ipa_test_engine.endp.log_ch[log_ch_idx];

    /* Increment the number of bearers on this logical channel */
  ipa_test_engine.endp.log_ch[log_ch_idx].bearer_ptr_arr \
    [ipa_test_engine.endp.log_ch[log_ch_idx].bearer_cnt] = 
    &ipa_test_engine.wan.bearer[bearer_idx];
  ipa_test_engine.endp.log_ch[log_ch_idx].bearer_cnt++;
  /**-- Internal Data Base Update --**/

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_wan_cfg

==============================================================================*/
/*!
  @brief
  Configures the IPA driver with the WAN input params 

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_wan_cfg
(
  ipa_test_engine_wan_cfg_s *wan_cfg_ptr
)
{
  uint8 i = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_WAN_CFG);

  /* Input Params checking first */
  IPA_TF_ASSERT(wan_cfg_ptr != NULL);
  IPA_TF_ASSERT((wan_cfg_ptr->num_bearers <= IPA_TEST_ENGINE_BEARER_NUM_MAX)); 

  for (i = 0; i < wan_cfg_ptr->num_bearers; i++)
  {
    ret_code = 
      ipa_test_engine_wan_bearer_cfg(i, wan_cfg_ptr->is_bearer_bridged[i],
                                     wan_cfg_ptr->bearer_tech[i]);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

    /* Increment the total number of bearers in the system */
    ipa_test_engine.wan.curr_bearer_cnt++;
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_cfg

==============================================================================*/
ipa_test_ret_code_e ipa_test_engine_clat_cfg()
{
  ipa_err_code_e   err_code = IPA_SUCCESS;
  uint8     i;
  ipa_clat_pdn_cntxt_info_type  clat_ctxt_info;
  ipa_clat_handle_t             clat_hdl;
  /*! Update the current test state */
  ipa_test_engine.curr_test.state = IPA_TEST_ENGINE_STATE_CLAT_CFG;
  
  /*! Register the CLAT configure callback */
  ipa_clat_register_evt_cb((ipa_clat_cfg_evt_cb_type)ipa_test_engine_clat_cfg_cb_fn,
                           (void *)&ipa_test_engine.clat.clat_cfg_cb);

  /*! send global CLAT ocnfiguration */
  err_code = ipa_clat_global_register
              (&ipa_test_engine.curr_test.cfg.clat_cfg.global_clat_cfg);

  IPA_TF_ASSERT(err_code == IPA_SUCCESS);

  /*! Configure all the CLAT PDNs */
  for (i = 0; i < ipa_test_engine.curr_test.cfg.clat_cfg.num_clat_pdn ; i++) 
  {
    /*! Save the CLAT configuration for each SIO in 
        the logical channel structure*/
    ipa_test_engine.endp.log_ch[i].is_clat = TRUE;

    
    /*! Add the stream id to the pending list of stream ids
     on which the CLAT configuration is done.*/
    //ipa_test_engine.clat.clat_cfg_cb.\
    //pend_stream_id_arr[ipa_test_engine.endp.log_ch[i].stream_id] = TRUE;

    ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
    /*! Increment the total number of clat pdn cntxt info */
    ipa_test_engine.clat.clat_cfg_cb.num_pend_clat_cfg_cb++ ;
    ipa_crit_sect_leave(&ipa_test_engine.crit_sect);

    /* Get the clat context info to configure */
    IPA_MEMSET(&clat_ctxt_info, 0, sizeof(ipa_clat_pdn_cntxt_info_type));
    clat_ctxt_info.v4_src_addr =
       ipa_test_engine.curr_test.cfg.clat_cfg.cntxt_info[i].v4_src_addr;
    clat_ctxt_info.v6_dev_prefix_len = 
      ipa_test_engine.curr_test.cfg.clat_cfg.cntxt_info[i].v6_src_prefix_len;
    clat_ctxt_info.v6_plat_prefix_len = 
      ipa_test_engine.curr_test.cfg.clat_cfg.cntxt_info[i].v6_dest_prefix_len;
    IPA_MEMCPY(&clat_ctxt_info.v6_dev_prefix, 
               &ipa_test_engine.curr_test.cfg.clat_cfg.cntxt_info[i].v6_src_prefix, 16);
    IPA_MEMCPY(&clat_ctxt_info.v6_plat_prefix, 
               &ipa_test_engine.curr_test.cfg.clat_cfg.cntxt_info[i].v6_dest_prefix, 16);

    /*! Register clat cfg to get hdl*/
    err_code = ipa_clat_register (&clat_ctxt_info,&clat_hdl);
    IPA_TF_ASSERT(err_code == IPA_SUCCESS);

    /*! Now enable waiting for clat cfg callbacks */
    ipa_test_engine.clat.clat_cfg_cb.clat_cfg_arr[(uint8)clat_hdl] = 
         IPA_TEST_ENGINE_CLAT_CB_ENABLED;
    
    /*! Now update clat cfg and hdl in our internal log channel */
    ipa_test_engine.endp.log_ch[i].clat_cntxt_ptr = 
      &ipa_test_engine.curr_test.cfg.clat_cfg.cntxt_info[i];
    ipa_test_engine.endp.log_ch[i].clat_hdl = clat_hdl;
    ipa_test_engine.endp.log_ch[i].clat_enabled = TRUE;

  } /* For num_clat_pdns */

  /*! Wait for CLAT CFG CB for all clat contexts */
  ipa_test_engine_clat_cfg_end();

  /*! Invoke Assoc sio for all clat contexts */
  for (i = 0; i < ipa_test_engine.curr_test.cfg.clat_cfg.num_clat_pdn ; i++) 
  {
    err_code = ipa_clat_assoc_sio_stream
                 (ipa_test_engine.endp.log_ch[i].clat_hdl, 
                  ipa_test_engine.endp.log_ch[i].stream_id);
    IPA_TF_ASSERT(err_code == IPA_SUCCESS);

    ipa_test_engine.endp.log_ch[i].clat_sio_assocd = TRUE;

    ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
    /*! Increment the total number of clat pdn cntxt info */
    ipa_test_engine.clat.clat_cfg_cb.num_pend_assoc_sio_cb++;   
    ipa_crit_sect_leave(&ipa_test_engine.crit_sect);
    
  }/* For num_clat_pdns */

  /*! Wait for CLAT ASSOC SIO CB */
  ipa_test_engine_clat_assoc_sio_end();

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_dpl_cfg

==============================================================================*/
/*!
  @brief
  Register the dpl watermarks

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_dpl_cfg(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  ipa_test_set_dpl_cfg(&ipa_test_engine.dpl);
  
  return ret_code;

}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ciph_cfg

==============================================================================*/
/*!
  @brief
  Initialize the ciphering parameters to be used for ciphering

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_ciph_cfg(void)
{
  uint32 i, j;
  uint8 bearer_idx = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_CIPH_CFG);

  /*! @todo Need to relook at this. We may need to regenerate the cipher 
    params for each test */
  /* Generate the keys and fill the cipher structure if it has not been
   already done*/
  /* Generate random cipher keys (0-128) and write it to IPA one-by-one*/
  for (i = 0; i < IPA_TEST_ENGINE_CIPH_PARAMS_NUM_MAX; i++) 
  {
    /* First 7 keys are used for subscriber 0 and next 8 for subscriber 1 
      Assignment of subscriptions
         Subscriber 0: First two key indexes AES, 
                       next two SNOW3G, 
                       rest two ZUC 
                       last one in NONE
         Subcriber  1: Next two AES, 
                       next two SNOW3G, 
                       rest two ZUC,
                       Next two are NONE*/

    if (i < IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS) 
    {
      ipa_test_engine.ciph.ciph_params[i].subs_id = 
        ipa_test_engine.subs_id_arr[0];
    }
    else
    {
      ipa_test_engine.ciph.ciph_params[i].subs_id = 
        ipa_test_engine.subs_id_arr[1];
    }

    if (((i % IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS) >= 
         IPA_TEST_ENGINE_CIPH_PARAMS_AES_OFST) && 
        ((i % IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS) < 
         IPA_TEST_ENGINE_CIPH_PARAMS_SNOW_3G_OFST)) 
    {
      ipa_test_engine.ciph.ciph_params[i].ciph_algo = 
        IPA_WAN_CIPHER_ALGO_AES;
    }
    else if (((i % IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS) >= 
              IPA_TEST_ENGINE_CIPH_PARAMS_SNOW_3G_OFST) && 
             ((i % IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS) < 
              IPA_TEST_ENGINE_CIPH_PARAMS_ZUC_OFST)) 
    {
      ipa_test_engine.ciph.ciph_params[i].ciph_algo = 
        IPA_WAN_CIPHER_ALGO_LTE_SNOW3G;
    }
    else if (((i % IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS) >= 
              IPA_TEST_ENGINE_CIPH_PARAMS_ZUC_OFST) && 
             ((i % IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS) < 
              IPA_TEST_ENGINE_CIPH_PARAMS_NONE_OFST)) 
    {
      ipa_test_engine.ciph.ciph_params[i].ciph_algo = 
        IPA_WAN_CIPHER_ALGO_ZUC;
    }
    else
    {
      ipa_test_engine.ciph.ciph_params[i].ciph_algo = 
        IPA_WAN_CIPHER_ALGO_NONE;
    }

    /* Write all the Cipher keys to IPA one by one */
    for (j = 0; j < IPA_WAN_CIPH_KEY_SIZE; j++) 
    {
      ipa_test_engine.ciph.ciph_params[i].ciph_key[j] =
        (uint8)ipa_test_generate_random_num(IPA_TEST_ENGINE_CIPH_KEY_VAL_MAX, 0);
    }

    ipa_test_engine.ciph.ciph_params[i].key_index =
      ipa_wani_pdcp_write_cipher_key(ipa_test_engine.ciph.ciph_params[i] \
                                     .subs_id,
                                     ipa_test_engine.ciph.ciph_params[i] \
                                     .ciph_key);

    /* Assert if the key index returned is ZERO*/
    IPA_TF_ASSERT(ipa_test_engine.ciph.ciph_params[i].key_index != 0);

    /* We want to always decipher in IPA */
    ipa_test_engine.ciph.ciph_params[i].direction = 
      IPA_TEST_ENGINE_CIPH_DIR_DL_DECIPH;

    /* Set a random count value */
    ipa_test_engine.ciph.ciph_params[i].count_c = 
      ipa_test_generate_random_num(0xFFFFFFFF, 0);

    /* Get the keystream and fill up the cipher param structure member: Keystream*/
    IPA_TEST_CIPH_DSM_BUF_INIT(ipa_test_engine.ciph.ciph_params[i] \
                               .keystream_dsm_ptr);

    /* Allocate a dummy dsm data buffer and memset it to zero*/
    IPA_TEST_CIPH_DSM_BUF_INIT(ipa_test_engine.ciph.ciph_params[i] \
                               .dummy_data_dsm_ptr);

    /* Set bearer id for ciphering and taging using the cipher config*/
    bearer_idx = 
      ipa_test_generate_random_num(ipa_test_engine.wan.curr_bearer_cnt - 1, 0);
    ipa_test_engine.ciph.ciph_params[i].bearer_id = 
      ipa_test_engine.wan.bearer[bearer_idx].bearer_id;
  
    /* Get the keystream values using the cipher parameters and store them*/
    ipa_test_generate_ciph_keystream(&ipa_test_engine.ciph.ciph_params[i]);
  }
  
  /*The cipher parameters have been filled in the structure*/
  ipa_test_engine.ciph.init_done = TRUE;
  

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_filter_cfg

==============================================================================*/
/*!
  @brief
  Initialize the filter parameters to be used for the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_filter_cfg
(
  ipa_test_engine_fltr_cfg_s *cfg_ptr
)
{
  ipa_test_filter_init_cfg_s cfg;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_FLTR_CFG);

  cfg.uid_array = ipa_test_engine.wan.uid_arr;

  /* Send the filter configuration information to install filter rules*/
  IPA_TF_ASSERT(ipa_test_filter_init(&cfg) == IPA_TEST_RET_CODE_SUCCESS);

  return IPA_TEST_RET_CODE_SUCCESS;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_static_filter_cfg

==============================================================================*/
/*!
  @brief
  Initialize the filter parameters to be used for the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_static_filter_cfg
(
  ipa_test_engine_fltr_cfg_s *cfg_ptr
)
{
  ipa_test_filter_init_cfg_s cfg;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_FLTR_CFG);

  /*Fill Filter Init Cfg*/
  cfg.bearer_cnt = ipa_test_engine.wan.curr_bearer_cnt;
  cfg.uid_array = ipa_test_engine.wan.uid_arr;
  cfg.num_dl_sckt_fltr = cfg_ptr->num_dl_sckt_fltr;
  cfg.num_ul_qos_fltr = cfg_ptr->num_ul_qos_fltr;
  cfg.fltr_rule_cat_type = cfg_ptr->fltr_rule_cat_type;
  cfg.fltr_rule_type =    cfg_ptr->fltr_rule_type;
  cfg.is_tcp_ack_qos_rule = cfg_ptr->is_tcp_ack_qos_rule;
  cfg.test_type = ipa_test_engine.curr_test.cfg.test_type;
  cfg.filter_pkt_gen = FALSE;
  cfg.total_pkts = ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts;
  cfg.v4_src_subnet_mask = 
    ipa_test_engine.curr_test.cfg.clat_cfg.global_clat_cfg.v4_client_subnet_mask; 
  cfg.is_clat_enabled = ipa_test_engine.curr_test.cfg.is_clat_enabled;

  /* Send the filter configuration information to install filter rules*/
  IPA_TF_ASSERT(ipa_test_static_filter_init(&cfg) == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(ipa_test_engine_sleep_time);

  return IPA_TEST_RET_CODE_SUCCESS;
}/*ipa_test_engine_static_filter_cfg()*/

/*==============================================================================

  FUNCTION:  ipa_test_engine_ul_clat_cb

==============================================================================*/
/*!
  @brief
  Validate CLAT translation scenario. CB invoked from IPA driver when CLAT translation
  is detected in the pkt status.

  @returns
  None
*/
/*============================================================================*/

void ipa_test_engine_ul_clat_translation_cb
(
  ipa_hw_pkt_status_s  * status_ptr
)
{
  /*! Set the signal to exit from the wait for the CLAT transaction callback */
  ipa_signal_set(&ipa_test_engine.clat_translation_signal);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_req_cb

==============================================================================*/
/*!
  @brief
  Test engine callback to be registered with IPA ZIP.

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_zip_req_cb
(
  /*! @brief Callback event type */ 
  ipa_zip_cb_event_type_e  cb_evt,
  /*! @brief Status for the request */ 
  ipa_zip_cb_status_type_e cb_status,
  /*! @brief Tag provided in command request */ 
  ipa_zip_cb_tag_type      cmd_cb_tag,
  /*! @brief Actual dst buf size returned for the command */ 
  uint32                   cmd_dst_buf_size
)
{
  if (cb_status == IPA_ZIP_CB_STATUS_PASS)
  {
    switch (cb_evt)
    {
      case IPA_ZIP_CB_EVENT_REGISTER:
      {
        /* Set the signal to notify that a ZIP Register done response has been received. */
        ipa_signal_set(&ipa_test_engine.zip_reg_done_signal);
        break;
       }

      case IPA_ZIP_CB_EVENT_DICT_LOAD:
      {
        /* Set the signal to notify that a Dictionary load response has been received. */
        ipa_signal_set(&ipa_test_engine.zip_dict_load_done_signal);
        break;
      }

      case IPA_ZIP_CB_EVENT_CMD:
      {
        /* Save a copy of the cb_tag received to compare the response to the 
           corresponding  mirror response. */
        ipa_test_engine.zip.cb_tag_rcvd[ipa_test_engine.zip.num_resp] = cmd_cb_tag;
        /* Save a copy of the destination buffer size received to compare 
           against the expected destination buffer length.*/
        ipa_test_engine.zip.dest_buf_len_rcvd[ipa_test_engine.zip.num_resp] = cmd_dst_buf_size;
        /* Increment the number of ZIP responses received. */
        ipa_test_engine.zip.num_resp++;
        break;
      }

      case IPA_ZIP_CB_EVENT_UNREGISTER:
      { 
        /* Set the signal to notify that a ZIP unregister done response has been received. */
        ipa_signal_set(&ipa_test_engine.zip_unreg_done_signal);
        break;
      }

      default:
        /* assert for a invalid response command from IPA ZIP */
        IPA_ASSERT(0);

      }
  }
  else
  {
    IPA_ASSERT_LOG(0,"CB STATUS FAILED", 0, 0, 0);
  }
  
}/*ipa_test_engine_zip_req_cb()*/


/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_err_cb

==============================================================================*/
/*!
  @brief
  Initialize the ZIP parameters to be used for the test

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_zip_err_cb
(
  /*! @brief Error code returned by HW */
  uint8                     error_code,
  /*! @brief Cb tag of the request that caused the error */
  ipa_zip_cb_tag_type       cb_tag,
  /*! @brief Address in the source buffer that triggered the error */
  uint16                    bytes_left   
)
{
  /*  */
  ipa_test_engine.zip.cb_tag_rcvd[ipa_test_engine.zip.num_resp] = cb_tag;
  ipa_test_engine.zip.error_code[ipa_test_engine.zip.num_resp] = error_code;
  ipa_test_engine.zip.bytes_left[ipa_test_engine.zip.num_resp] = bytes_left;  

  ipa_test_engine.zip.num_resp++;
  
} /*ipa_test_engine_zip_err_cb()*/

/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_cfg

==============================================================================*/
/*!
  @brief
  Initialize the ZIP parameters to be used for the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_zip_cfg
(
  ipa_test_engine_zip_cfg_s *cfg_ptr
)
{
#if 0
  ipa_zip_cfg_type_s cfg;
  ipa_zip_err_code_e err_code;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_FLTR_CFG);

  /*Fill Filter Init Cfg*/
  cfg.req_cb_fn_ptr = (ipa_zip_cb_type)ipa_test_engine_zip_req_cb;
  cfg.err_cb_fn_ptr = (ipa_zip_err_cb_type)ipa_test_engine_zip_err_cb;
  cfg.max_def_prio_reqs = cfg_ptr->max_def_prio_reqs;
  cfg.max_hi_prio_reqs = cfg_ptr->max_hi_prio_reqs;
 
  /* Send the zip register configuration information to register with IPA ZIP*/
  IPA_TF_ASSERT(ipa_zip_register(&cfg) == IPA_ZIP_SUCCESS);

  /* Wait for the IPA ZIP registration to complete */
  ipa_test_engine_zip_reg_done_evnt_wait_end();

  /* Set the IPA ZIP register complete to TRUE */
  ipa_test_engine.zip.register_complete = TRUE;

  /* Now load the RO dictionary */
  IPA_ASSERT(ipa_zip_load_dict(&ipa_test_engine.curr_test.cfg.zip_cfg.dict_type) == IPA_ZIP_SUCCESS);

  /* Wait for Dictionary load to complete. */
  ipa_test_engine_zip_dict_load_evnt_wait_end();
  
  /* Set dictionary load as complete */
  ipa_test_engine.zip.dict_load_complete = TRUE;

  return IPA_TEST_RET_CODE_SUCCESS;
#endif
}/*ipa_test_engine_zip_cfg()*/

/*==============================================================================

  FUNCTION:  ipa_test_engine_ul_deaggr_exception_cb

==============================================================================*/
/*!
  @brief
  Validate Deaggr err scenarios. CB invoked from IPA driver when excpetion 
  is detected in the pkt status.

  @returns
  None
*/
/*============================================================================*/

void ipa_test_engine_ul_deaggr_exception_cb
(
  ipa_hw_pkt_status_s  * status_ptr,
  dsm_item_type        * pkt_dsm_ptr,
  dsm_watermark_type   * sio_rx_wm_ptr
)
{
  ipa_test_engine_deaggr_err_info_s * deaggr_err_info =
    &ipa_test_engine.curr_test.deaggr_err_info;
  dsm_item_type *tmp_tail_ptr = NULL;
  uint32 dsm_dup_len, pkt_dsm_len;
  uint16 pkt1_len, pkt2_len, pkt3_len, pkt_pad_len;
  uint8  pkt_enqueue_flag = TRUE;

  /*! Validate the error frame got from IPA. Error frame will be duped 
      and enqueued to sio wmk for engine pkt comparison.
      
  */
  
  switch (ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {

    /*! 1. QMAP */

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT:
    {
      /* First dsm_dup_packet the required pkt len bytes since the frame 
         can have extra trailer bytes.
      */
      IPA_TF_ASSERT (status_ptr->exception == 
                     IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR);
      deaggr_err_info->exception_cnt++;

      dsm_dup_len = 
            dsm_dup_packet( &deaggr_err_info->rcvd_frame_dsm_ptr \
                            [deaggr_err_info->rcvd_frame_cnt], 
                            pkt_dsm_ptr, 0,status_ptr->pkt_len);

      IPA_TF_ASSERT_LOG(dsm_dup_len == status_ptr->pkt_len, 
                          "dsm dup fail!", 0, 0, 0);

      /* Validate length */
      deaggr_err_info->rcvd_frame_len_arr[deaggr_err_info->rcvd_frame_cnt] =
        dsm_length_packet(deaggr_err_info->rcvd_frame_dsm_ptr \
                          [deaggr_err_info->rcvd_frame_cnt]);

      IPA_TF_ASSERT(
        deaggr_err_info->rcvd_frame_len_arr[deaggr_err_info->rcvd_frame_cnt] == 
        deaggr_err_info->sent_frame_len_arr[deaggr_err_info->rcvd_frame_cnt]
        );
    }
    break;

    /* Bad QMAP hdr in middle of one frame */
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1:
    {
      ipa_hw_qmap_hdr_s       qmap_hdr_test;

      IPA_TF_ASSERT (status_ptr->exception == 
                     IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR);
      deaggr_err_info->exception_cnt++;

      /*1. Check for the invalid pkt_len value */
      uint32 pull_up_len = dsm_extract(pkt_dsm_ptr,0,
                           (uint32 *)&qmap_hdr_test,sizeof(ipa_hw_qmap_hdr_s));

      *((uint32 *)(&qmap_hdr_test)) = 
         ipa_ntohl((uint32)(*((uint32 *)(&qmap_hdr_test))));

      IPA_TF_ASSERT(pull_up_len == sizeof(ipa_hw_qmap_hdr_s));
      IPA_TF_ASSERT(qmap_hdr_test.pkt_len == 0xFFFC);

      /*2. check for the total frame len from status ptr and dsm_length_packet */
      pkt2_len = deaggr_err_info->qmap_hdr[1].pkt_len;
      pkt3_len = deaggr_err_info->qmap_hdr[2].pkt_len;

      IPA_TF_ASSERT( status_ptr->pkt_len == (pkt2_len + pkt3_len + 
                                             2* sizeof(ipa_hw_qmap_hdr_s)));
      
      /*3. Check frame len from dsm_length_packet. First dsm_dup_packet the 
           required pkt2+pkt3 bytes since the frame can have extra trailer bytes.
      */
      dsm_dup_len = 
           dsm_dup_packet( &deaggr_err_info->rcvd_frame_dsm_ptr \
                           [deaggr_err_info->rcvd_frame_cnt], pkt_dsm_ptr, 0,
                           (pkt2_len + pkt3_len + 2*sizeof(ipa_hw_qmap_hdr_s)));
           
      IPA_TF_ASSERT_LOG((dsm_dup_len == pkt2_len + pkt3_len + 
                         (2 * sizeof(ipa_hw_qmap_hdr_s))), 
                         "dsm dup fail!", 0, 0, 0);
        
      pkt_dsm_len = dsm_length_packet(deaggr_err_info->rcvd_frame_dsm_ptr \
                                      [deaggr_err_info->rcvd_frame_cnt]);     
      IPA_TF_ASSERT( pkt_dsm_len == (pkt2_len + pkt3_len + 
                                     2*sizeof(ipa_hw_qmap_hdr_s)));

      /*! Validate exception err status */
      IPA_TF_ASSERT(deaggr_err_info->exception_cnt == 1);
      
    }
    break;

    /* Zeroed QMAP hdr_type */
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1:
    {
      deaggr_err_info->exception_cnt++;
      if(status_ptr->exception == IPA_HW_PKT_STATUS_EXCEPTION_IPTYPE)
      {
        deaggr_err_info->iptype_exception_cnt++;
      }
      
      if (deaggr_err_info->exception_cnt == 3)
      {
	    IPA_TF_ASSERT (status_ptr->exception == 
                     IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR);
      }
      
      dsm_dup_len = 
            dsm_dup_packet( &deaggr_err_info->rcvd_frame_dsm_ptr \
                            [deaggr_err_info->rcvd_frame_cnt], 
                            pkt_dsm_ptr, 0,status_ptr->pkt_len);

      IPA_TF_ASSERT_LOG(dsm_dup_len == status_ptr->pkt_len, 
                          "dsm dup fail!", 0, 0, 0);
    }
    
    break;
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES:
    {

      /* If pkt 1 has no pad bytes appended, IPA will pull in pad bytes from
         second pkt and 1st pkt is always received as valid. Since the 2nd
         qmap hdr is offset by pkt1_pad bytes IPA might not find the start of 
         IP hdr and flag a ip type exception as well. 
      */

      //IPA_TF_ASSERT (status_ptr->exception == 
      //               IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR);
      //deaggr_err_info->exception_cnt++;

      if (deaggr_err_info->pkt_no_pad_index == 0)
      {
        IPA_TF_ASSERT ((status_ptr->exception == 
                        IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR) || 
                       (status_ptr->exception == 
                        IPA_HW_PKT_STATUS_EXCEPTION_IPTYPE));
      
        if(status_ptr->exception == IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR)
        {
          deaggr_err_info->exception_cnt++;
        }
        else if (status_ptr->exception == IPA_HW_PKT_STATUS_EXCEPTION_IPTYPE)
        {
          deaggr_err_info->iptype_exception_cnt++;
        }

        /* Pkt1 --> is a valid pkt with padding bytes pulled from the 2nd pkt 
                    by IPA. status_pkt_len = (pkt_len - pad_len) + qmap hdr + ins hdr.
                    IPA strips out the pad bytes and the valid IP pkt is 
                    received in the sio wmk.

           Pkt2 --> Will have pkt1 pad bytes stripped out. 
                    Status_pkt_len = (pkt_len - pkt1_pad_len) + qmap hdr 
        */

        /* Second pkt will be received as exception pkt. So dup it.*/
        dsm_dup_len = 
            dsm_dup_packet( &deaggr_err_info->rcvd_frame_dsm_ptr \
                            [deaggr_err_info->rcvd_frame_cnt], 
                            pkt_dsm_ptr, 0,status_ptr->pkt_len);

        IPA_TF_ASSERT_LOG(dsm_dup_len == status_ptr->pkt_len, "dsm dup fail!", 0, 0, 0);

      }
      else if (deaggr_err_info->pkt_no_pad_index == 1)
      {
        IPA_TF_ASSERT (status_ptr->exception == 
                     IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR);
        deaggr_err_info->exception_cnt++;

        pkt2_len = deaggr_err_info->qmap_hdr_info[0][1].pkt_len;
        pkt_pad_len = deaggr_err_info->qmap_hdr_info[0][1].pad_len;

        IPA_TF_ASSERT( status_ptr->pkt_len == (pkt2_len + 
                                               sizeof(ipa_hw_qmap_hdr_s) 
                                               - pkt_pad_len ) );

        /*3. Check frame len from dsm_length_packet. First dsm_dup_packet the 
           required pkt2+pkt3 bytes since the frame can have extra trailer bytes.
        */

        dsm_dup_len = 
           dsm_dup_packet( &deaggr_err_info->rcvd_frame_dsm_ptr \
                           [deaggr_err_info->rcvd_frame_cnt], pkt_dsm_ptr, 0,
                           status_ptr->pkt_len );
           
        IPA_TF_ASSERT_LOG((dsm_dup_len == (pkt2_len + sizeof(ipa_hw_qmap_hdr_s) 
                                           - pkt_pad_len)), "dsm dup fail!", 
                                           0, 0, 0);
      }
      else
      {
         pkt3_len = deaggr_err_info->qmap_hdr_info[0][2].pkt_len;
         pkt_pad_len = deaggr_err_info->qmap_hdr_info[0][2].pad_len;

         IPA_TF_ASSERT( status_ptr->pkt_len == (pkt3_len - pkt_pad_len +  
                                                sizeof(ipa_hw_qmap_hdr_s)));
         dsm_dup_len = 
           dsm_dup_packet( &deaggr_err_info->rcvd_frame_dsm_ptr \
                           [deaggr_err_info->rcvd_frame_cnt], 
                           pkt_dsm_ptr, 0, status_ptr->pkt_len);
           
        IPA_TF_ASSERT_LOG(dsm_dup_len == (pkt3_len + sizeof(ipa_hw_qmap_hdr_s)), 
                         "dsm dup fail!", 0, 0, 0);

      }
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_TYPE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_SIG:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_SIG:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NXT_NDP_IDX:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_DGRM_IDX:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_SEC_EOT_FRAME:
    {

      IPA_TF_ASSERT (status_ptr->exception == 
                     IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR);
      deaggr_err_info->exception_cnt++;

      dsm_dup_len = 
            dsm_dup_packet( &deaggr_err_info->rcvd_frame_dsm_ptr \
                            [deaggr_err_info->rcvd_frame_cnt], 
                            pkt_dsm_ptr, 0,status_ptr->pkt_len);

      IPA_TF_ASSERT_LOG(dsm_dup_len == status_ptr->pkt_len, "dsm dup fail!", 0, 0, 0);
    }
    break;

    /*! 2. RNDIS */

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_OFFSET:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_LEN:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_LEN:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE:
    {
      IPA_TF_ASSERT (status_ptr->exception == 
                     IPA_HW_PKT_STATUS_EXCEPTION_DEAGGR);
      deaggr_err_info->exception_cnt++;

      /*! Dont dup and enqueue the pkt since IPA driver will be handling this
         ethernet exception pkt and enqueue in SIO Rx Wmk.
      */
      pkt_enqueue_flag = FALSE;
      
    }
    break;

    default:
     break;
  }
  
  /* Enqueue error frame received in sio rx wmk for pkt comparison. Do this for
     all tests except RNDIS test as ipa driver will itself handle this exception
     pkt and enqueue to ds sio wmk.
  */
  if (pkt_enqueue_flag == TRUE)
  {
     if (sio_rx_wm_ptr != NULL)
     {
       dsm_enqueue(sio_rx_wm_ptr, 
                   &deaggr_err_info->rcvd_frame_dsm_ptr \
                   [deaggr_err_info->rcvd_frame_cnt]);
     }
  }

  deaggr_err_info->rcvd_frame_cnt++;

}

/*==============================================================================

  FUNCTION:  ipa_test_engine_aggr_configure

==============================================================================*/
/*!
  @brief
  Specific Configuration for aggr/deaggr tests.

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_aggr_configure(void)
{
   
  int8 i = 0;

  if(ipa_test_engine.curr_test.cfg.rx_pkt_test_type != 
       IPA_TEST_ENGINE_RX_PKT_WAIT)
  {
    switch (ipa_test_engine.curr_test.cfg.rx_pkt_test_type)
    {
      case IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_HIST_STATS:
      {
        ipa_test_engine.curr_test.pkt_wait = FALSE;

        /* Take the snapshot of the UL status log write index to 
         find the new statuses logged as part of the current test case
         only. */
        ipa_test_engine.curr_test.prev_stts_log_wr_idx = 
           ipa_ul_get_curr_status_log_wr_idx();

        /* Memset deaggr hist stats for UL Per csmr pipe since it will hold
           cumulative stats from previous tests.
        */
        for (i = 0; i <= IPA_STATS_AGGR_BUCKET_MAX; i++) 
        {
          ipa_stats.endp[IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_0_ID]. \
             ul.deaggr_hist[i].num_frames = 0;
        }

      }
      break;

      case IPA_TEST_ENGINE_RX_PKT_WAIT_PREVEOT_BYTE_THRESHOLD:
      case IPA_TEST_ENGINE_RX_PKT_WAIT_AGGR_FORCE_CLOSE_PIPES:
      {
        /* Take the snapshot of the DL status log write index to 
        find the new statuses logged as part of the current test case
        only. */
        ipa_test_engine.curr_test.pkt_wait = TRUE;
        ipa_test_engine.curr_test.prev_stts_log_wr_idx = 
          ipa_dl_cnsmr_get_curr_stts_log_wr_idx();

        /*! Set below Aggr parameters on DL per dest pipe for PREV_EOT test. 
            These will be set during DL sio mapping done below. FORCE_CLOSE
            will be set after reading PREV_EOT status with IPA holding an open
            frame.
        */

        /* 1. Set AGG_TIME_LIMIT = 0*/
        ipa_cfg.ctl.dl_aggr_max_time_limit = 0;

        /* 2. Enable HARD_BYTE_LIMIT. */
        ipa_cfg.ctl.dl_aggr_hard_byte_limit = 1;

        /* 3. pkt limit/byte limit- will be set by the test */

       }
       break;

      case IPA_TEST_ENGINE_RX_PKT_WAIT_DROPPED_PKT_BYTE_THRESHOLD:
      {  
        ipa_test_engine.curr_test.prev_stts_log_wr_idx = 
          ipa_dl_cnsmr_get_curr_stts_log_wr_idx();
        ipa_test_engine.curr_test.pkt_wait = FALSE;
          
        /* 1. Set AGG_TIME_LIMIT = 0*/
        ipa_cfg.ctl.dl_aggr_max_time_limit = 0;

        /* 2. Enable HARD_BYTE_LIMIT. */
        ipa_cfg.ctl.dl_aggr_hard_byte_limit = 1;

        /* 3. Disable PROC_ERR_IRQ for this test since IPA generates this 
              interrupt when pkt is dropped due to hard_byte_limit. Since driver 
              is not servicing this int disable it for now else will assert.
        */
        ipa_cfg.hw.irq_enabled_mask &= ~IPA_HW_IRQ_PROC_ERR;
        ipa_hal_set_irq((ipa_hw_irq_e)ipa_cfg.hw.irq_enabled_mask);
      }
      break;

      case IPA_TEST_ENGINE_RX_PKT_WAIT_TX_THRESHOLD_INT_STATS:
      {
        ipa_test_engine.curr_test.pkt_wait = TRUE;

        /* 1. Disable AGGR_HARD_BYTE_LIMIT since it might be enabled
              from previous tests. Else if set, we might face an issue with pkt
              not coming back as we dont set FORCE_CLOSE for this test.*/
        ipa_cfg.ctl.dl_aggr_hard_byte_limit = 0;

        /* 2. Set AGG_TIME_LIMIT = 0*/
        ipa_cfg.ctl.dl_aggr_max_time_limit = IPA_CFG_CTL_AGGR_MAX_TIME_LIMIT_IN_MS;
        
      }
      break;

      case IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING:
      {
         /* Init deaggr_err_info structure for curr test */
         ipa_test_engine.curr_test.deaggr_err_info.err_pkt_gen = FALSE;
         ipa_test_engine.curr_test.deaggr_err_info.frame_cnt = 0;
         ipa_test_engine.curr_test.deaggr_err_info.rcvd_frame_cnt = 0;
         ipa_test_engine.curr_test.deaggr_err_info.pkt_cnt = 0;
         ipa_test_engine.curr_test.deaggr_err_info.exception_cnt = 0;
         ipa_test_engine.curr_test.deaggr_err_info.pkt_no_pad_index = -1;
         ipa_ul_test_reg_deaggr_err_cb
           (ipa_test_engine_ul_deaggr_exception_cb);

         /*! QMAP deaggr test config */

         /* Zeroed qmap hdr case. Just check the status excpetion */
         ipa_test_engine.curr_test.pkt_wait = 
           (ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type == 
           IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1) ? 
           FALSE : TRUE;

         /* Set Max pkt len(or msg len) to be 4k */ 
         if (IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE == 
             ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type
            )
         {           
            ipa_cfg.ctl.ul_deaggr_max_pkt_len = 0x0FA0;
         }
         
         /* Set max aggr bytes to 32k for reserved port */
         if (IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1 != 
             ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
         {
           ipa_cfg.ctl.aggr_ep_rsvd_ul_max_limit_in_bytes = 0x7EF4;
           ipa_cfg.ctl.aggr_ep_rsvd_dl_max_limit_in_bytes = 0x7EF4;
           #if 0
           ipa_sio.ul_aggr_per_ic_sup[0].max_aggr_bytes = 
           ipa_cfg.ctl.aggr_ep_rsvd_ul_max_limit_in_bytes;
           ipa_sio.dl_aggr_per_ic_sup[0].max_aggr_bytes = 
           ipa_cfg.ctl.aggr_ep_rsvd_dl_max_limit_in_bytes;
           #endif
         }
	 else //TODO
         {
	    /*! Set below Aggr parameters on DL per dest pipe for PREV_EOT test. 
            These will be set during DL sio mapping done below. FORCE_CLOSE
            will be set after reading PREV_EOT status with IPA holding an open
            frame.
            */

            /* 1. Set AGG_TIME_LIMIT = 0*/
            ipa_cfg.ctl.dl_aggr_max_time_limit = 0;
   
            /* 2. Enable HARD_BYTE_LIMIT. */
            ipa_cfg.ctl.dl_aggr_hard_byte_limit = 1;

            /* 3. pkt limit/byte limit- will be set by the test */
	 }

         /*! RNDIS Deaggr test Config */
#ifdef FEATURE_IPA_USE_IPA_BAM_DRIVER
         if (IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_SPLIT_2_DESC == 
             ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
         {
            ipa_hwio_def_bam_p_ctrln_u ctrl;

            /* Read curr value */
            ctrl.value = IPA_HAL_HWIO_IN_REG_n(BAM_P_CTRLn, 
                                 IPA_HW_Q6_SIM_UL_B2B_PRODUCER_PIPE_0_ID);

            /*Set new value on B2B Prod pipe */
            ctrl.def.p_auto_eob = 1;
            ctrl.def.p_auto_eob_sel = 0; // 00->512 bytes
            IPA_HAL_HWIO_OUT_REG_n(BAM_P_CTRLn, 
                               IPA_HW_Q6_SIM_UL_B2B_PRODUCER_PIPE_0_ID,
                               ctrl.value);
         }
#endif

         /* Set Max pkt len(or msg len) to be 1k */ 
         if (IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE == 
             ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type
            )
         {           
            ipa_cfg.ctl.ul_deaggr_max_pkt_len = 0x05DC; /* 1500 bytes */
         }

      }
      break;

      default:
      break;
    }
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_configure

==============================================================================*/
/*!
  @brief
  Does all the configuration before we start with the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_configure
(
  ipa_test_engine_params_s *params_ptr
)
{
  ipa_test_ret_code_e ret_code;
  ipa_err_code_e      err;
  sio_stream_id_type stream_id;
  uint8 i;
  ipa_hwio_def_ipa_log_u log;
  ipa_hwio_def_ipa_endp_init_mode_n_u mode;
  ipa_hwio_def_ipa_endp_status_n_u status;
  ipa_hw_pipe_id_e dl_cnsmr_pipe_num = (ipa_hw_pipe_id_e)
                       IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_DL_CONSUMER_PIPE);
  ipa_hw_pipe_id_e ul_src_pipe_num = (ipa_hw_pipe_id_e)
                       IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_0);

  IPA_TF_ASSERT(params_ptr != NULL);

  ipa_test_engine_init();

  /* Memset our prev database structures */
  IPA_MEMSET(&ipa_test_engine.endp, 0, sizeof(ipa_test_engine.endp));
  IPA_MEMSET(&ipa_test_engine.clat, 0, sizeof(ipa_test_engine.clat));
  IPA_MEMSET(&ipa_test_engine.wan, 0, sizeof(ipa_test_engine.wan));
  IPA_MEMSET(&ipa_test_engine.ip_pkt, 0, sizeof(ipa_test_engine.ip_pkt));
  IPA_MEMSET(&ipa_test_engine.fltr, 0, sizeof(ipa_test_engine.fltr));
  IPA_MEMSET(&ipa_test_engine.curr_test, 0, sizeof(ipa_test_engine.curr_test));
  IPA_MEMSET(&ipa_test_engine.pkt_cmp, 0, sizeof(ipa_test_engine.pkt_cmp));

  /* Before all cfg is done. Store the recvd params in our data struct*/
  ipa_test_engine.curr_test.cfg = *params_ptr;

  /* Before all cfg is done, save current ipa_cfg.ctl */
  ipa_test_engine_cfg_ctl_copy = ipa_cfg.ctl;

  /* Getting GSI out of clock gating in case it was not yet done */ 
  IPA_HAL_HWIO_OUT_REG(IPA_GSI_TOP_GSI_CGC_CTRL, 0x0);
  	
  /* Cfg Static filters first before call is UP */
  if (ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == TRUE)
  {
    ret_code = ipa_test_engine_static_filter_cfg(&params_ptr->fltr_cfg);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Configure IPA ZIP if enabled and IPA ZIP hasn't been registered yet. */
  if (ipa_test_engine.curr_test.cfg.is_zip_enabled == TRUE && 
      (ipa_test_engine.zip.register_complete == FALSE))
  {
    ret_code = ipa_test_engine_zip_cfg(&params_ptr->zip_cfg);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Set required Aggr parameters before endp cfg for no pkt wait 
     aggr/deaggr test */
  ipa_test_engine_aggr_configure();

  ret_code = ipa_test_engine_endp_cfg(&params_ptr->endp_cfg);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  if(ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE)
  {
	 
	  /* DL scenario needed replication configuration is done in DPL API functions */
	  ipa_acc_dpl_init(IPA_ACC_DPL_VER_2, 0xF/*DL aggregation packet limit*/, 0xF/*DL aggregation byte limit*/);
	  /* Enabling replication for UL consumer pipe - needed for all UL scenarios */
	  if(ipa_test_engine.curr_test.cfg.test_type != IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB)//Daria Do we need this check in new implementation ?
	  {
	    /* TBD - should be changed to use the same api for DPL as external client is using */
		ipa_acc_dpl_add_hw_rep_pipe(ul_src_pipe_num); 
	  }
	  ipa_acc_dpl_configure(IPA_ACC_DPL_STATE_ENABLE);
  }
  	

  ret_code = ipa_test_engine_wan_cfg(&params_ptr->wan_cfg);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  /*! Configure CLAT for the PDNs if it is enabled. */
  if (ipa_test_engine.curr_test.cfg.is_clat_enabled  == TRUE)
  {
    ret_code = ipa_test_engine_clat_cfg();

    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

    /*! If CLAT is enabled bridge the bearers later */
    for (i = 0; i < params_ptr->wan_cfg.num_bearers ; i++ )
    {
      if (ipa_test_engine.wan.bearer[i].is_bridged == TRUE) 
      {
        err = 
        ipa_wani_ds_config_dl_bridge(ipa_test_engine.wan.bearer[i].uid, 
                                   IPA_DL_BRIDGE_STATE_BRIDGE);
        IPA_TF_ASSERT(err == IPA_SUCCESS);
      }
    }
  }

  /*! Register the error callback if its a error scenario test case */
  if (IPA_TEST_ENGINE_CLAT_ERR_NONE != ipa_test_engine.curr_test.cfg.clat_cfg.err_scenario)
  {
    ipa_ul_test_reg_deaggr_err_cb(ipa_test_engine_ul_clat_ihl_exception_cb);
  }

  if (ipa_test_engine.curr_test.cfg.is_dpl_enabled  == TRUE)
  {
    ret_code = ipa_test_engine_dpl_cfg();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
  
  if (ipa_test_engine.curr_test.cfg.wan_cfg.is_ciph_enabled == TRUE)
  {
    ret_code = ipa_test_engine_ciph_cfg();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Cfg other qos and DL socket filters*/
  if (ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == TRUE)
  {
    ret_code = ipa_test_engine_filter_cfg(&params_ptr->fltr_cfg);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Flow Control Cfg only if fc test is enabled*/
  if (ipa_test_engine.curr_test.cfg.is_fc_enabled == TRUE)
  {
    //ipa_test_engine.curr_test.num_pkts_to_flow_ctl = IPA_TEST_WM_HIGH;
    ipa_test_engine.curr_test.is_flow_disabled = FALSE;

    /*Get Stream id*/
    stream_id = ipa_test_engine.endp.log_ch[0].stream_id;

    /*Disable UL Flow*/
    sio_ioctl(stream_id, SIO_IOCTL_INBOUND_FLOW_DISABLE, NULL);
    ipa_test_engine.curr_test.is_flow_disabled = TRUE;

  }

  /* Start DL checksum offload */
  err = ipa_offload_checksum();
  IPA_TF_ASSERT(err == IPA_SUCCESS);

  /* Reset the seed so that a stale params_ptr will not re-run the same test again.
  If the user wants to rerun a test with some seed. they need to explicitly set it 
  before calling the cfg_generate */
  /*! @todo remove the hack?? But we'll have to modify all the existing test cases */
  params_ptr->seed = 0;

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_destroy_ip_pkts

==============================================================================*/
/*!
  @brief
  Generates IP packets for a test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_destroy_ip_pkts(void)
{
  uint16 i = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  for (i = 0; i < ipa_test_engine.ip_pkt.curr_num_pkts; i++)
  {
    if (ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr != NULL)
    {
      IPA_FREE(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr);
    }
  }
  
  /* For IPA2.1 test cases keepingtrack of the number of DSM items
     in the pkt_info_arr that can be later cleaned to prevent DSM exhaution.*/
  if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != 
     ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {
    /* Keep a copy of the number of total dsm items in the pkt info array */
    ipa_test_engine.ip_pkt.curr_dsm_items = ipa_test_engine.ip_pkt.curr_num_pkts;
    ipa_test_engine.ip_pkt.curr_num_pkts = 0;
  }
  else if (ipa_test_engine.ip_pkt.curr_num_pkts_ul == 0 && ipa_test_engine.ip_pkt.curr_num_pkts_dl == 0)
  {
    /* Memset the entire strcture */
    IPA_MEMSET(&ipa_test_engine.ip_pkt, 0, sizeof(ipa_test_engine.ip_pkt));
  }
  else 
  {
    ipa_test_engine.ip_pkt.curr_num_pkts = 0;
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_destroy_ul_replicated_ip_pkts

==============================================================================*/
/*!
  @brief
  Generates IP packets for a test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_destroy_ul_replicated_ip_pkts(void)
{
  uint16 i = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  for (i = 0; i < ipa_test_engine.ip_pkt.curr_num_pkts_ul; i++)
  {
    if (ipa_test_engine.ip_pkt.pkt_info_arr_replication_ul[i].pkt_buf_ptr != NULL)
    {
      IPA_FREE(ipa_test_engine.ip_pkt.pkt_info_arr_replication_ul[i].pkt_buf_ptr);
    }
  }
  
  /* For IPA2.1 test cases keepingtrack of the number of DSM items
     in the pkt_info_arr that can be later cleaned to prevent DSM exhaution.*/
  if (ipa_test_engine.ip_pkt.curr_num_pkts == 0 && ipa_test_engine.ip_pkt.curr_num_pkts_dl == 0)
  {
    /* Memset the entire strcture */
    IPA_MEMSET(&ipa_test_engine.ip_pkt, 0, sizeof(ipa_test_engine.ip_pkt));
  }
  else
  {
    ipa_test_engine.ip_pkt.curr_num_pkts_ul = 0;
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_destroy_dl_replicated_ip_pkts

==============================================================================*/
/*!
  @brief
  Generates IP packets for a test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_destroy_dl_replicated_ip_pkts(void)
{
  uint16 i = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  for (i = 0; i < ipa_test_engine.ip_pkt.curr_num_pkts_dl; i++)
  {
    if (ipa_test_engine.ip_pkt.pkt_info_arr_replication_dl[i].pkt_buf_ptr != NULL)
    {
      IPA_FREE(ipa_test_engine.ip_pkt.pkt_info_arr_replication_dl[i].pkt_buf_ptr);
    }
  }
  
  /* For IPA2.1 test cases keepingtrack of the number of DSM items
     in the pkt_info_arr that can be later cleaned to prevent DSM exhaution.*/
  if (ipa_test_engine.ip_pkt.curr_num_pkts == 0 && ipa_test_engine.ip_pkt.curr_num_pkts_ul == 0)
  {
    /* Memset the entire strcture */
    IPA_MEMSET(&ipa_test_engine.ip_pkt, 0, sizeof(ipa_test_engine.ip_pkt));
  }
  else
  {
    ipa_test_engine.ip_pkt.curr_num_pkts_dl = 0;
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ciph_dsm_buffer

==============================================================================*/
/*!
  @brief
  1. Tag the DSM buffer with the cipher information. 
  2. Cipher the DSM buffer data.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_ciph_dsm_buffer
(
  dsm_item_type *tx_dsm_ptr,
  uint8         subs_id,
  uint8         is_ciph_enabled, 
  uint8         is_bearer_bridged
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint32 ciph_param_idx, ciph_param_base;
  uint16 pkt_len = dsm_length_packet(tx_dsm_ptr);
  
  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_CIPH_GEN);

  /* Select a random cipher param to be used for ciphering */
  if (subs_id == ipa_test_engine.subs_id_arr[0]) 
  {
    ciph_param_base = 0;
  }
  else if (subs_id == ipa_test_engine.subs_id_arr[1])
  {
    ciph_param_base = IPA_TEST_ENGINE_CIPH_KEY_NUM_PER_SUBS;
  }
  else
  {
     IPA_TF_ASSERT(0);
  }

  if (is_ciph_enabled == TRUE)
  {
    IPA_TF_ASSERT(ipa_test_engine.ciph.init_done == TRUE);
    if (is_bearer_bridged == TRUE)
    {
      /* For bridged bearers, we can chose any algo */
      ciph_param_idx = 
        ipa_test_generate_random_num(IPA_TEST_ENGINE_CIPH_PARAMS_END_OFST - 1,
                                     IPA_TEST_ENGINE_CIPH_PARAMS_AES_OFST);
    }
    else
    {
      /* For unbridged bearers, we can chose any algo excpet NONE to overcome 
      the packet reordering issue  */
      ciph_param_idx = 
        ipa_test_generate_random_num(IPA_TEST_ENGINE_CIPH_PARAMS_NONE_OFST - 1,
                                     IPA_TEST_ENGINE_CIPH_PARAMS_AES_OFST);
    }
    /* Now add the base */
    ciph_param_idx += ciph_param_base;

    /* For subscription 2 there is only 1 NONE cipher param */
    if (ciph_param_idx == IPA_TEST_ENGINE_CIPH_PARAMS_NUM_MAX)
    {
      ciph_param_idx = IPA_TEST_ENGINE_CIPH_PARAMS_NUM_MAX - 1;
    }

    /* Tag the dsm item with the ciphering parameters*/
    ipa_wani_pdcp_tag_cipher_params
      (tx_dsm_ptr,
       ipa_test_engine.ciph.ciph_params[ciph_param_idx].count_c,
       ipa_test_engine.ciph.ciph_params[ciph_param_idx].bearer_id,
       ipa_test_engine.ciph.ciph_params[ciph_param_idx].key_index,
       ipa_test_engine.ciph.ciph_params[ciph_param_idx].ciph_algo);

    ipa_test_xor_ciph_keystream(tx_dsm_ptr, pkt_len,
                                &ipa_test_engine.ciph.ciph_params[ciph_param_idx]);
  }
  else
  {
    /* Tag the dsm item with the none ciphering parameters*/
    ipa_wani_pdcp_tag_cipher_params(tx_dsm_ptr, 0, 0, 0, IPA_WAN_CIPHER_ALGO_NONE);
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_generate_ip_pkts

==============================================================================*/
/*!
  @brief
  Generates IP packets for a test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_generate_ip_pkts(void)
{
  uint16 i = 0, j = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_PKT_GEN);

  IPA_TF_ASSERT((ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts > 0) &&
                (ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts <= 
                 IPA_TEST_ENGINE_IP_PKT_NUM_MAX));

  for (i = 0; i < ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts; i++)
  {
    if(ipa_test_engine.curr_test.cfg.ip_pkt_cfg.params.frag_pkt_len != 0)
    {
      do{
        ret_code = 
          ipa_test_generate_ip_packet(&ipa_test_engine.curr_test.cfg.ip_pkt_cfg,
                                  &ipa_test_engine.ip_pkt.pkt_info_arr[i+j]);
        IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

        ipa_test_engine.ip_pkt.curr_num_pkts++;

        j++;
      
       }while (ipa_test_get_frag_status() == TRUE); 
    }
    else
    {
      ret_code = 
          ipa_test_generate_ip_packet(&ipa_test_engine.curr_test.cfg.ip_pkt_cfg,
                                  &ipa_test_engine.ip_pkt.pkt_info_arr[i]);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
 
      ipa_test_engine.ip_pkt.curr_num_pkts++;
    }
        
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_dl_process_ip_pkts

==============================================================================*/
/*!
  @brief
  Some Pkt processing to be done for input packets.
  For DL:
  1. Allocate the packet to a specific bearer or logical channel
  2. Modify pkts according to filter cfg'ed 
  3. Split the packet across multiple DSM items

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_dl_process_ip_pkts(void)
{
  uint16 i = 0;
  uint8 num_dsm_items = 1;
  uint16 pushdown_len = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 idx = 0;
  ipa_test_engine_ip_pkt_dl_alloc_bucket_s *dl_alloc_ptr = NULL;
  ipa_test_filter_pkt_gen_cfg_s fltr_cfg;
  uint8 num_max_dsm_split_pkt = 0;
  uint8* pkt_buff;
  uint16 pseudo_hdr_chksum;
  uint16 payload_chksum;
  uint16 checksum_buffer[2];
  uint16 cksum_val;
  uint16 payload_len;
  uint16 ext_hdr_length = 0;
  uint8 chksum_hdr_offset;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_DL_PKT_PROC);

  IPA_TF_ASSERT((ipa_test_engine.ip_pkt.curr_num_pkts > 0) &&
                (ipa_test_engine.ip_pkt.curr_num_pkts <= 
                 IPA_TEST_ENGINE_IP_PKT_NUM_MAX));

  for (i = 0; i < ipa_test_engine.ip_pkt.curr_num_pkts; i++)
  {
    /* First allocate this packet to a bearer/logical channel bucket */
    if (ipa_test_engine.wan.curr_bearer_cnt == 0)
    {
      idx = 
        ipa_test_generate_random_num(ipa_test_engine.endp.curr_log_ch_cnt - 1, 0);

      /* Add it to the corresponding bucket */
      dl_alloc_ptr = &ipa_test_engine.ip_pkt.dl_alloc_buckets[idx];
      dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts++] = 
        &ipa_test_engine.ip_pkt.pkt_info_arr[i];
      ipa_test_engine.ip_pkt.pkt_info_arr[i].mux_id = 
        ipa_test_engine.endp.log_ch[idx].mux_id;
      ipa_test_engine.ip_pkt.pkt_info_arr[i].filter_result = 0;

      /*! If a CLAT case, modify the packet accoding to the CLAT 
          configuration for the logical channel */
      if( (ipa_test_engine.endp.log_ch[idx].is_clat == TRUE) &&
          (ipa_test_engine.curr_test.cfg.is_clat_enabled  == TRUE) &&
          (IPA_TEST_GET_IP_VERSION(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr) == 6) )
      {
          /*! Set the IPV6 source prefix for the packet */
          IPA_TEST_SET_V6_SRC_PREFIX(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr,
                                     ipa_test_engine.endp.log_ch[idx].clat_cntxt_ptr->v6_dest_prefix,
                                     ipa_test_engine.endp.log_ch[idx].clat_cntxt_ptr->v6_dest_prefix_len);
          /*! Embed IPv4 addr in IPv6 addr */
          ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr(
             IPA_TEST_GET_PREFIX_LEN(ipa_test_engine.endp.log_ch[idx].clat_cntxt_ptr->v6_dest_prefix_len)/8,
             &ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[IPA_TEST_IPV6_SRC_ADDR_OFST], 
             ipa_test_engine.endp.log_ch[idx].clat_cntxt_ptr->v4_dest_addr);

          /*! Set the IPV6 destination prefix for the packet */
          IPA_TEST_SET_V6_DEST_PREFIX(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr,
                                      ipa_test_engine.endp.log_ch[idx].clat_cntxt_ptr->v6_src_prefix,
                                      ipa_test_engine.endp.log_ch[idx].clat_cntxt_ptr->v6_src_prefix_len);
          /*! Embed IPv4 addr in IPv6 addr */
          ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr(
             IPA_TEST_GET_PREFIX_LEN(ipa_test_engine.endp.log_ch[idx].clat_cntxt_ptr->v6_src_prefix_len)/8,
             &ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[IPA_TEST_IPV6_DEST_ADDR_OFST], 
             ipa_test_engine.endp.log_ch[idx].clat_cntxt_ptr->v4_src_addr);


		  if (dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr[6] == IPA_TEST_ENGINE_FRAG_EXTN_HDR_TYPE)
  		  {
  		    if (dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr[40] == IPA_TEST_UDP_PROT)
		    {
		      chksum_hdr_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
		    }
		    else if (dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr[40] == IPA_TEST_TCP_PROT)
  		    {
		      chksum_hdr_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
		    }
			payload_len =  ipa_htons(*((uint16*)(&dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr[4])))
		  					- sizeof(ipa_test_ipv6_pkt_frag_extn_hdr_s);
			ext_hdr_length = sizeof(ipa_test_ipv6_pkt_frag_extn_hdr_s);
		  }
		  else if (dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr[6] == IPA_TEST_UDP_PROT)
		  {
		    chksum_hdr_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
			payload_len =  ipa_htons(*((uint16*)(&dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr[4])));
		  }
		  else if (dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr[6] == IPA_TEST_TCP_PROT)
  		  {
		    chksum_hdr_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
			payload_len =  ipa_htons(*((uint16*)(&dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr[4])));
		  }
		  else
		  {
		    IPA_ASSERT(0);
		  }

		  pkt_buff =  dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr;
          pseudo_hdr_chksum = ipa_test_calculate_pseudo_hdr_cksum(dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr);

		  *((uint16*)&pkt_buff[IPA_TEST_IPV6_HDR_LEN + ext_hdr_length + chksum_hdr_offset]) = 0;
		   
          payload_chksum = 
        	  ipa_test_calculate_cksum((uint16*)&dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]-> \
        	  								pkt_buf_ptr[IPA_TEST_IPV6_HDR_LEN + ext_hdr_length],
                                            payload_len);
          payload_chksum = ~payload_chksum & 0xFFFF;

	      checksum_buffer[0] = pseudo_hdr_chksum;
	      checksum_buffer[1] = payload_chksum;
	      cksum_val = ipa_test_calculate_cksum((uint16 *)&checksum_buffer, sizeof(checksum_buffer));

           *((uint16*)&pkt_buff[IPA_TEST_IPV6_HDR_LEN + ext_hdr_length + chksum_hdr_offset]) = cksum_val;
      }
    }
    else
    {
      /* Statically alloc equal pkts to bearer 0 and bearer 1 for Aggr force 
         close on 2 pipes test. We need to have similar PREV_EOT scenario on
         both pipes
      */
      if (IPA_TEST_ENGINE_RX_PKT_WAIT_AGGR_FORCE_CLOSE_PIPES ==
           ipa_test_engine.curr_test.cfg.rx_pkt_test_type)
      {
        switch (i)
        {
          /* First 2 pkts on Bearer 0->Phy Ch0 */
          case 0:
          case 1:
             idx = 0;  
          break;
          
          /* Next 2 pkts on Bearer 1->Phy Ch1 */
          case 2:
          case 3:
            idx = 1;
          break;
          default:
          break;
        }

       }
       else
       {
        idx = 
          ipa_test_generate_random_num(ipa_test_engine.wan.curr_bearer_cnt - 1, 0);
       }

      /* Add it to the corresponding bucket */
      dl_alloc_ptr = &ipa_test_engine.ip_pkt.dl_alloc_buckets[idx];
      dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts++] = &ipa_test_engine.ip_pkt.pkt_info_arr[i];
      ipa_test_engine.ip_pkt.pkt_info_arr[i].mux_id = ipa_test_engine.wan.bearer[idx].log_ch_ptr->mux_id;

      /*! If a CLAT case, modify the packet to be transmitted according to the CLAT 
          configuration for the logical channel */
      if( (ipa_test_engine.wan.bearer[idx].log_ch_ptr->is_clat == TRUE) &&
          (ipa_test_engine.curr_test.cfg.is_clat_enabled  == TRUE) &&
          (IPA_TEST_GET_IP_VERSION(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr) == 6) )
      {
          /*! Set the IPV6 source prefix for the packet */
          IPA_TEST_SET_V6_SRC_PREFIX(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr,
                                     ipa_test_engine.wan.bearer[idx].log_ch_ptr->clat_cntxt_ptr->v6_dest_prefix,
                                     ipa_test_engine.wan.bearer[idx].log_ch_ptr->clat_cntxt_ptr->v6_dest_prefix_len);

          /*! Embed IPv4 addr in IPv6 addr */
          ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr(
             IPA_TEST_GET_PREFIX_LEN(ipa_test_engine.wan.bearer[idx].log_ch_ptr->clat_cntxt_ptr->v6_dest_prefix_len)/8,
             &ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[IPA_TEST_IPV6_SRC_ADDR_OFST], 
             ipa_test_engine.wan.bearer[idx].log_ch_ptr->clat_cntxt_ptr->v4_dest_addr);

          /*! Set the IPV6 destination prefix for the packet */
          IPA_TEST_SET_V6_DEST_PREFIX(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr,
                                      ipa_test_engine.wan.bearer[idx].log_ch_ptr->clat_cntxt_ptr->v6_src_prefix,
                                      ipa_test_engine.wan.bearer[idx].log_ch_ptr->clat_cntxt_ptr->v6_src_prefix_len);

          /*! Embed IPv4 addr in IPv6 addr */
          ipa_test_engine_clat_embed_ipv4_addr_in_ipv6_addr(
             IPA_TEST_GET_PREFIX_LEN(ipa_test_engine.wan.bearer[idx].log_ch_ptr->clat_cntxt_ptr->v6_src_prefix_len)/8,
             &ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[IPA_TEST_IPV6_DEST_ADDR_OFST], 
             ipa_test_engine.wan.bearer[idx].log_ch_ptr->clat_cntxt_ptr->v4_src_addr);        

		  //pkt_buff =  dl_alloc_ptr->pkt_arr[dl_alloc_ptr->num_pkts]->pkt_buf_ptr;

		  if (ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[6] == IPA_TEST_ENGINE_FRAG_EXTN_HDR_TYPE || 
		  	  ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[6] == IPA_TEST_ENGINE_ROUTING_EXTN_HDR_TYPE ||
		  	  ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[6] == IPA_TEST_ENGINE_DEST_OPTS_EXTN_HDR_TYPE)
  		  {
  		    if (ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[40] == IPA_TEST_UDP_PROT)
		    {
		      chksum_hdr_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
		    }
		    else if (ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[40] == IPA_TEST_TCP_PROT)
  		    {
		      chksum_hdr_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
		    }
			payload_len =  ipa_htons(*((uint16*)(&ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[4])))
		  					- sizeof(ipa_test_ipv6_pkt_frag_extn_hdr_s);
			ext_hdr_length = sizeof(ipa_test_ipv6_pkt_frag_extn_hdr_s);
		  }
		  else if (ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[6] == IPA_TEST_UDP_PROT)
		  {
		    chksum_hdr_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
			payload_len =  ipa_htons(*((uint16*)(&ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[4])));
		  }
		  else if (ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[6] == IPA_TEST_TCP_PROT)
  		  {
		    chksum_hdr_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
			payload_len =  ipa_htons(*((uint16*)(&ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[4])));
		  }
		  else
		  {
		    IPA_ASSERT(0);
		  }

          pseudo_hdr_chksum = ipa_test_calculate_pseudo_hdr_cksum(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr);

		  *((uint16*)(&ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[IPA_TEST_IPV6_HDR_LEN + ext_hdr_length + chksum_hdr_offset])) = 0;
		   
          payload_chksum = 
        	  ipa_test_calculate_cksum((uint16*)&ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[IPA_TEST_IPV6_HDR_LEN + ext_hdr_length],
                                            payload_len);
          payload_chksum = ~payload_chksum & 0xFFFF;

	      checksum_buffer[0] = pseudo_hdr_chksum;
	      checksum_buffer[1] = payload_chksum;
	      cksum_val = ipa_test_calculate_cksum((uint16 *)&checksum_buffer, sizeof(checksum_buffer));

		  if (ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[6] == IPA_TEST_UDP_PROT && cksum_val == 0)
		  {
		    cksum_val = ~cksum_val & 0xFFFF;
		  }

          *((uint16*)(&ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr[IPA_TEST_IPV6_HDR_LEN + ext_hdr_length + chksum_hdr_offset])) = cksum_val;
      }
      /* Check if we need to filter the packet */
      if ((ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == TRUE) &&
          (ipa_test_engine.wan.bearer[idx].is_bridged == TRUE))
      {
        /*! Update the current test state */
        ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_FLTR_PROC);

        /* Filters are installed only if there are some bearers in the system */
        fltr_cfg.ip_pkt_info_ptr = &ipa_test_engine.ip_pkt.pkt_info_arr[i];
        fltr_cfg.uid = ipa_test_engine.wan.bearer[idx].uid;
        fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_DL_SOCKET;

        ipa_test_filter_dl_process_ip_pkt(fltr_cfg);
      }
    }

    /* Check if we want to split the packet across DSM items */
    if (ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_dsm_items == 0)
    {
      /* Let's only keep 1/4 th of the packets with more than 
      10 dsm item split to mirror DL logic */
      if (num_max_dsm_split_pkt < (ipa_test_engine.ip_pkt.curr_num_pkts / 
                                   IPA_TEST_ENGINE_DSM_CHAIN_FRACTION_AVG))
      {
        num_dsm_items = 
          ipa_test_generate_random_num(IPA_TEST_ENGINE_DSM_CHAIN_NUM_MAX, 1);
      }
      else
      {
        num_dsm_items = 
          ipa_test_generate_random_num(IPA_TEST_ENGINE_DSM_CHAIN_NUM_AVG, 1);
      }
      if (num_dsm_items > IPA_TEST_ENGINE_DSM_CHAIN_NUM_AVG)
      {
        num_max_dsm_split_pkt++;
        ipa_test_engine.curr_test.num_pkts_dl_split_avg++;
      }
      if (num_dsm_items > IPA_TEST_ENGINE_DSM_CHAIN_NUM_COLLAPSE)
      {
        ipa_test_engine.curr_test.num_pkts_dl_split_col++;
      }
    }
    else
    {
      num_dsm_items = ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_dsm_items;
    }

    if (num_dsm_items > 1)
    {
      /*! Update the current test state */
      ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_DSM_CHAIN_PROC);

      ret_code = 
        ipa_test_generate_dsm_chain(&ipa_test_engine.ip_pkt.pkt_info_arr[i].dsm_ptr,
                                    num_dsm_items,
                                    ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr,
                                    ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_len);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

      /* Generate dsm chain with empty buffers in between for the specific test */
      if (IPA_TEST_ENGINE_RX_PKT_WAIT_EMPTY_DSM_BUFFER_CHAIN ==
          ipa_test_engine.curr_test.cfg.rx_pkt_test_type)
      {
        ret_code = 
            ipa_test_generate_random_dsm_chain(
               ipa_test_engine.ip_pkt.pkt_info_arr[i].dsm_ptr);
        IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

        /* Though empty buffers are induced total dsm len should be same */
        IPA_TF_ASSERT(dsm_length_packet(ipa_test_engine.ip_pkt.pkt_info_arr[i].dsm_ptr) ==
                      ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_len
                     );
      }
    }
    else
    {
      pushdown_len = 
        dsm_pushdown(&ipa_test_engine.ip_pkt.pkt_info_arr[i].dsm_ptr,
                     ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr,
                     ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_len,
                     IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(pushdown_len == 
                    ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_len);
    }

    /* Check if we need to cipher the packet */
    if ((ipa_test_engine.wan.curr_bearer_cnt != 0) && 
        (ipa_test_engine.wan.bearer[idx].bearer_tech == IPA_WAN_BEARER_TECH_LTE))
    {
      ret_code = 
        ipa_test_engine_ciph_dsm_buffer(ipa_test_engine.ip_pkt.pkt_info_arr[i] \
                                        .dsm_ptr,
                                        ipa_test_engine.wan.bearer[idx] \
                                        .subs_id,
                                        ipa_test_engine.curr_test.cfg.wan_cfg \
                                        .is_ciph_enabled,
                                        ipa_test_engine.wan.bearer[idx] \
                                        .is_bridged);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    }
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ul_process_ip_pkts

==============================================================================*/
/*!
  @brief
  Some Pkt processing to be done for input packets.
  For UL:
  1. Allocate the packet to a physical & logical channel
  2. Modify pkts according to filter cfg'ed 
  3. pushdown the packet into a DSM item

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_ul_process_ip_pkts(void)
{
  uint16 i = 0;
  uint16 pushdown_len = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  ipa_test_engine_ip_pkt_ul_alloc_bucket_s *ul_alloc_ptr = NULL;
  uint8 idx = 0;
  uint8 phy_ch_idx = 0;
  ipa_test_filter_pkt_gen_cfg_s fltr_cfg;
  uint32 ipv4_src_address;
  uint32 ipv4_dest_address;
  uint8* pkt_buff;
  uint16 pseudo_hdr_chksum;
  uint16 payload_chksum;
  uint16 checksum_buffer[2];
  uint16 cksum_val;
  uint16 payload_len;
  uint8 chksum_hdr_offset;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_UL_PKT_PROC);

  IPA_TF_ASSERT((ipa_test_engine.ip_pkt.curr_num_pkts > 0) &&
                (ipa_test_engine.ip_pkt.curr_num_pkts <= 
                 IPA_TEST_ENGINE_IP_PKT_NUM_MAX));

  for (i = 0; i < ipa_test_engine.ip_pkt.curr_num_pkts; i++)
  {
    /* First allocate this packet to a physical channel bucket */
    if (ipa_test_engine.wan.curr_bearer_cnt == 0)
    {
      idx = 
        ipa_test_generate_random_num(ipa_test_engine.endp.curr_log_ch_cnt - 1, 0);

      phy_ch_idx = ipa_test_engine.endp.log_ch[idx].phy_ch_ptr->idx;

      /* Add it to the corresponding bucket */
      ul_alloc_ptr = &ipa_test_engine.ip_pkt.ul_alloc_buckets[phy_ch_idx];
      ul_alloc_ptr->log_ch_ptr[ul_alloc_ptr->num_pkts] = 
        &ipa_test_engine.endp.log_ch[idx];
      ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts] = 
        &ipa_test_engine.ip_pkt.pkt_info_arr[i];
      ipa_test_engine.ip_pkt.pkt_info_arr[i].mux_id = 
        ipa_test_engine.endp.log_ch[idx].mux_id;

      /* Static filters are installed if there are no bearers */
      fltr_cfg.ip_pkt_info_ptr = &ipa_test_engine.ip_pkt.pkt_info_arr[i];

    }
    else
    {
      idx = 
        ipa_test_generate_random_num(ipa_test_engine.wan.curr_bearer_cnt - 1, 0);

      phy_ch_idx = ipa_test_engine.wan.bearer[idx].log_ch_ptr->phy_ch_ptr->idx;

      /* Add it to the corresponding bucket */
      ul_alloc_ptr = &ipa_test_engine.ip_pkt.ul_alloc_buckets[phy_ch_idx];
      ul_alloc_ptr->log_ch_ptr[ul_alloc_ptr->num_pkts] = 
        ipa_test_engine.wan.bearer[idx].log_ch_ptr;
      ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts] = 
        &ipa_test_engine.ip_pkt.pkt_info_arr[i];
      ipa_test_engine.ip_pkt.pkt_info_arr[i].mux_id = 
        ipa_test_engine.wan.bearer[idx].log_ch_ptr->mux_id;

      /* QOS Filters are installed only if there are some bearers in the system */
      fltr_cfg.ip_pkt_info_ptr = &ipa_test_engine.ip_pkt.pkt_info_arr[i];
      fltr_cfg.uid = ipa_test_engine.wan.bearer[idx].uid;

    }
    /*! Generate IPV4 packets according to the CLAT configuration provided
          if CLAT is enabled for the SIO. */
      if ((ipa_test_engine.curr_test.cfg.is_clat_enabled == TRUE) &&
        (ul_alloc_ptr->log_ch_ptr[ul_alloc_ptr->num_pkts]->is_clat == TRUE) && 
        (IPA_TEST_GET_IP_VERSION(ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr) == 4))
      {
        ipv4_src_address = ul_alloc_ptr->log_ch_ptr[ul_alloc_ptr->num_pkts]->clat_cntxt_ptr->v4_src_addr;
        IPA_TEST_SET_V4_SRC_ADDR(ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr,
                                 ipv4_src_address);

        ipv4_dest_address = ul_alloc_ptr->log_ch_ptr[ul_alloc_ptr->num_pkts]->clat_cntxt_ptr->v4_dest_addr;
        IPA_TEST_SET_V4_DEST_ADDR(ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr,
                                  ipv4_dest_address);

         /*! For CLAT DL loopback test cases set the DF bit if it need to be set*/
         if (ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->df_bit == TRUE)
         {
           ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr[6] |= 0x40;
         }

		 if (ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr[9] == IPA_TEST_TCP_PROT)
  		 {
		   chksum_hdr_offset = IPA_TEST_TCP_CKSUM_HDR_OFST;
		 }
		 else if (ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr[9] == IPA_TEST_UDP_PROT)
		 {
		   chksum_hdr_offset = IPA_TEST_UDP_CKSUM_HDR_OFST;
		 }
		 
          pkt_buff =  ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr;
          pseudo_hdr_chksum = ipa_test_calculate_pseudo_hdr_cksum(ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr);

          payload_len =  ipa_htons(*((uint16*)(&ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr[2])))
		  					- IPA_TEST_IPV4_HDR_LEN;

		  *((uint16*)&pkt_buff[IPA_TEST_IPV4_HDR_LEN + chksum_hdr_offset]) = 0;
		   
          payload_chksum = 
        	  ipa_test_calculate_cksum((uint16*)&ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr[IPA_TEST_IPV4_HDR_LEN],
                                       payload_len);
          payload_chksum = ~payload_chksum & 0xFFFF;

	      checksum_buffer[0] = pseudo_hdr_chksum;
	      checksum_buffer[1] = payload_chksum;
	      cksum_val = ipa_test_calculate_cksum((uint16 *)&checksum_buffer, sizeof(checksum_buffer));

           *((uint16*)&pkt_buff[IPA_TEST_IPV4_HDR_LEN + chksum_hdr_offset]) = cksum_val;
		
      }

	//ipa_test_calculate_tcp_cksum(ul_alloc_ptr->pkt_arr[ul_alloc_ptr->num_pkts]->pkt_buf_ptr);
	 
    /*! Increment the number of packets in the ul bucket. */
    ul_alloc_ptr->num_pkts++;
    
    /* Check if we need to filter the packet */
    if (ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == TRUE)
    {
      /*! Update the current test state */
      ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_FLTR_PROC);

      ipa_test_filter_ul_process_ip_pkt(fltr_cfg);
    }

    pushdown_len = 
      dsm_pushdown(&ipa_test_engine.ip_pkt.pkt_info_arr[i].dsm_ptr,
                   ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr,
                   ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_len,
                   IPA_CFG_SL_DSM_ITEM_POOL);
    IPA_TF_ASSERT(pushdown_len == 
                  ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_len);
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_aggregate_endp_pkt

==============================================================================*/
/*!
  @brief
  1. Generate an aggregated frame based on the given endp config 

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_aggregate_endp_pkt
(
  ipa_sio_endpoint_config_s     *cfg_ptr,
  uint8                         is_clat,
  uint16                        num_pkts,
  ipa_test_engine_ip_pkt_info_s **pkt_arr_ptr,
  uint8                         *num_pkts_used,
  dsm_item_type                 **frame_dsm_ptr
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_FAILURE;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_UL_AGGR);

  switch (cfg_ptr->ul_config.hdr_type)
  {
    case IPA_SIO_HDR_TYPE_NONE:
      if (cfg_ptr->ul_config.is_qos_present == TRUE)
      {
        ret_code = 
          ipa_test_aggregate_qos_endp_pkt(pkt_arr_ptr, 
                                          num_pkts,
                                          &cfg_ptr->ul_config,
                                          frame_dsm_ptr, 
                                          num_pkts_used);
      }
      else
      {
        ret_code = 
          ipa_test_aggregate_none_endp_pkt(pkt_arr_ptr, 
                                           num_pkts,
                                           &cfg_ptr->ul_config,
                                           frame_dsm_ptr, 
                                           num_pkts_used);
      }
      break;

    case IPA_SIO_HDR_TYPE_ETHERNET:
      ret_code = 
        ipa_test_aggregate_ethernet_endp_pkt(pkt_arr_ptr, 
                                             num_pkts,
                                             &cfg_ptr->ul_config,
                                             frame_dsm_ptr, 
                                             num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_QMAP:
      ret_code = 
        ipa_test_aggregate_qmap_endp_pkt(
                                        pkt_arr_ptr, 
                                        &ipa_test_engine.curr_test.cfg.deaggr_err_cfg,
                                        &ipa_test_engine.curr_test.deaggr_err_info,
                                         num_pkts,
                                         &cfg_ptr->ul_config,
                                         frame_dsm_ptr, 
                                         num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_QMAP_V3:
	case IPA_SIO_HDR_TYPE_QMAP_V4:
      ret_code = 
        ipa_test_aggregate_qmap_v3_endp_pkt(pkt_arr_ptr, 
                                            num_pkts,
                                            &cfg_ptr->ul_config,
                                            is_clat,
                                            frame_dsm_ptr, 
                                            num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_QCNCM:
      break;

    case IPA_SIO_HDR_TYPE_MBIM:
      ret_code = 
        ipa_test_aggregate_mbim_endp_pkt(pkt_arr_ptr, 
                                         &ipa_test_engine.curr_test.cfg.deaggr_err_cfg,
                                         &ipa_test_engine.curr_test.deaggr_err_info,
                                         num_pkts,
                                         &cfg_ptr->ul_config,
                                         frame_dsm_ptr,
                                         num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_RNDIS:
      ret_code = 
        ipa_test_aggregate_rndis_endp_pkt(
                              pkt_arr_ptr, 
                              &ipa_test_engine.curr_test.cfg.deaggr_err_cfg,
                              &ipa_test_engine.curr_test.deaggr_err_info,
                                         num_pkts,
                                         &cfg_ptr->ul_config,
                                         frame_dsm_ptr,
                                         num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_DIRECT_DMA:
      ret_code = 
        ipa_test_aggregate_none_endp_pkt(pkt_arr_ptr, 
                                         num_pkts,
                                         &cfg_ptr->ul_config,
                                         frame_dsm_ptr, 
                                         num_pkts_used);
      break;

    default:
      IPA_TF_ASSERT(0);
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_gen_qmap_deaggr_err_frame

==============================================================================*/
/*!
  @brief
  Generate QMAP error frame based on deaggr err cfg tests.
  Also generates the mix of valid pkts and error frames as expected from IPA 
  and enqueues in mirror wmk for comparison.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_gen_qmap_deaggr_err_frame
(
  ipa_test_engine_deaggr_err_cfg_s * deaggr_err_cfg,
  dsm_item_type                    * aggr_frame_dsm_ptr,
  ipa_test_engine_ip_pkt_info_s   ** pkt_arr_ptr,
  uint8                            * num_total_frames
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  ipa_test_engine_deaggr_err_info_s * deaggr_err_info =
    &ipa_test_engine.curr_test.deaggr_err_info;
  uint16 pkt1_len = 0, pkt2_len = 0, pkt3_len = 0, aggr_frame_len = 0, dsm_dup_len;
  uint16 total_frame_len = 0, pkt_pad_len, pushdown_len = 0;
  uint16 * pull_up_bytes;
  
  switch (ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT:
    {
       /*!  max_aggr_pkts = 3;
            num_pkts = 4;
            all pkts/first pkt = error pkts
            aggr_frame_dsm_ptr --> aggr qmap frame per_sim->IPA
      */      
      /* Validate frame */
      
      if (deaggr_err_info->frame_cnt == 0)
      {
        pkt1_len = deaggr_err_info->qmap_hdr_info \
                                    [deaggr_err_info->frame_cnt][0].pkt_len;
        pkt2_len = deaggr_err_info->qmap_hdr_info \
                                    [deaggr_err_info->frame_cnt][1].pkt_len;
        pkt3_len = deaggr_err_info->qmap_hdr_info \
                                    [deaggr_err_info->frame_cnt][2].pkt_len;
      
        aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
        IPA_TF_ASSERT(aggr_frame_len == 
             (pkt1_len + pkt2_len + pkt3_len + 3 * sizeof(ipa_hw_qmap_hdr_s)));
      }
      else if (deaggr_err_info->frame_cnt == 1)
      {
        pkt1_len = deaggr_err_info->qmap_hdr_info \
                     [deaggr_err_info->frame_cnt][0].pkt_len;
        aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
        IPA_TF_ASSERT(aggr_frame_len == (pkt1_len + sizeof(ipa_hw_qmap_hdr_s)));
      }

      /* Need to Dup the complete aggr frame since the same frame will be enqueued in both
          mirror wmk and per sim wmk.
      */
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
         aggr_frame_len;

      /* Store total aggr pkt cnt got from gen_aggr_pkt. */
      deaggr_err_info->pkt_cnt += *num_total_frames; 
      /* Set to 1 since one frame will be enqueued each time to mirror wmk 
         and not the ip pkts.*/
      * num_total_frames = 1; 
      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->frame_arr_idx      = deaggr_err_info->frame_cnt;
      
      dsm_dup_len = dsm_dup_packet( 
                    &deaggr_err_info->frame_arr[deaggr_err_info->frame_arr_idx], 
                    aggr_frame_dsm_ptr, 0, aggr_frame_len);
           
      IPA_TF_ASSERT_LOG(dsm_dup_len == aggr_frame_len, "dsm dup fail!", 0, 0, 0);
      
      /*Final verification of src and duped framed ptr.*/
      IPA_TF_ASSERT(dsm_length_packet (aggr_frame_dsm_ptr) == 
                    dsm_length_packet (deaggr_err_info->frame_arr \
                                     [deaggr_err_info->frame_arr_idx]));

      /* If total_num_pkts>max_aggr_pkts multiple frames will be generated */
      deaggr_err_info->frame_cnt++;
    }
    break;


    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1:
    {
      /*!  max_aggr_pkts = 3
          num_pkts = 3
          second_pkt = error_pkt      
          aggr_frame_dsm_ptr --> aggr qmap frame per_sim->IPA
      */
    
      /* Build the error frame with pkt2+pkt3(pkt2 qmap hdr is invalid)
         that would be got from IPA to compare. Dsm_dup the frame from
         pkt2 and enqueue it.
      */

      deaggr_err_info->pkt_cnt = ipa_test_engine.ip_pkt.curr_num_pkts;

      pkt1_len = deaggr_err_info->qmap_hdr[0].pkt_len;
      pkt2_len = deaggr_err_info->qmap_hdr[1].pkt_len;
      pkt3_len = deaggr_err_info->qmap_hdr[2].pkt_len;

      /* Validate aggr frame */
      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] = 
        aggr_frame_len;
      IPA_TF_ASSERT(aggr_frame_len == 
        (pkt1_len + pkt2_len + pkt3_len + 3 * sizeof(ipa_hw_qmap_hdr_s)));

      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;

      /* dsm_dup_packets to separate out valid and invalid frames. */
      uint16 dsm_dup_len = 
           dsm_dup_packet( &deaggr_err_info->err_frame_dsm_ptr, 
                           aggr_frame_dsm_ptr, 
                           (sizeof(ipa_hw_qmap_hdr_s) + pkt1_len),
                           (pkt2_len + pkt3_len + 2 * sizeof(ipa_hw_qmap_hdr_s)));
           
      IPA_TF_ASSERT_LOG((dsm_dup_len == 
                         pkt2_len + pkt3_len + (2 * sizeof(ipa_hw_qmap_hdr_s))),
                         "dsm dup fail!", 0, 0, 0);
         
      /*Validate error frame*/
      aggr_frame_len = dsm_length_packet(deaggr_err_info->err_frame_dsm_ptr);
      IPA_TF_ASSERT(aggr_frame_len == 
                    pkt2_len + pkt3_len + (2 * sizeof(ipa_hw_qmap_hdr_s)));
         
      deaggr_err_info->frame_arr[1] = deaggr_err_info->err_frame_dsm_ptr;

      /* Now store the total set of frames in arr that needs to be enqueued.
         Frame1=pkt_1, Frame2=error_frame=pkt2+pkt3
      */         
      * num_total_frames = 2;  
      deaggr_err_info->frame_cnt = * num_total_frames;
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE:
    {
      /* Store total aggr pkt cnt got from gen_aggr_pkt. */
      deaggr_err_info->pkt_cnt += *num_total_frames; 
      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] = 
        dsm_length_packet (aggr_frame_dsm_ptr);

      for (int i =0;i< *num_total_frames;i++)
      {
         total_frame_len += 
           sizeof(ipa_hw_qmap_hdr_s) +
           dsm_length_packet(deaggr_err_info->frame_arr[i]) + 
           deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][i].pad_len;
      }

      /*Final verification of src and duped framed ptr.*/
      IPA_TF_ASSERT(total_frame_len ==
        deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt]);

      /* If total_num_pkts>max_aggr_pkts multiple frames will be generated */
      deaggr_err_info->frame_cnt++;
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1:
    {
      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
         aggr_frame_len;

      /* Store total aggr pkt cnt got from gen_aggr_pkt. */
      deaggr_err_info->pkt_cnt += *num_total_frames; 
      /* Set to 1 since one frame will be enqueued each time to mirror wmk 
         and not the ip pkts.*/
      * num_total_frames = 1; 
      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->frame_arr_idx      = deaggr_err_info->frame_cnt;
      
      dsm_dup_len = 
           dsm_dup_packet( &deaggr_err_info->frame_arr \
                           [deaggr_err_info->frame_arr_idx], 
                           aggr_frame_dsm_ptr, 0, aggr_frame_len);
           
      IPA_TF_ASSERT_LOG(dsm_dup_len == aggr_frame_len, "dsm dup fail!", 0, 0, 0);
      deaggr_err_info->frame_cnt++;
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES:
    {

      /*! Valid pad_len value in hdr with No padding bytes appended. */

      if (deaggr_err_info->pkt_no_pad_index == 0)
      {
         /* First pkt has no padding bytes. So IPA will pull the pad_len 
            bytes from second pkt.So pkt1 will be received as valid and 
            second pkt will be invalid. */

         aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
         deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
           aggr_frame_len;

         /* Store total aggr pkt cnt got from gen_aggr_pkt. */
         deaggr_err_info->pkt_cnt += *num_total_frames; 

         deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
         deaggr_err_info->frame_arr_idx      = deaggr_err_info->frame_cnt;
      
         /* Pkt 1 - valid pkt */
         pkt1_len = 
         deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][0].pkt_len;
         pkt_pad_len = 
         deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][0].pad_len;
         deaggr_err_info->frame_arr[0] = 
              pkt_arr_ptr[deaggr_err_info->pkt_no_pad_index]->dsm_ptr;

         /* Pkt 2 - error pkt. Pkt1_pad_len bytes pulled out 
            Offset = qmap hdr + pkt1 len(pkt_len + pad_len ) . pad_len bytes 
            will be from 2nd pkt.
         */
         pkt2_len = 
            deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][1].pkt_len;
         dsm_dup_len = 
           dsm_dup_packet( &deaggr_err_info->frame_arr[1], aggr_frame_dsm_ptr, 
                           sizeof(ipa_hw_qmap_hdr_s) + pkt1_len, 
                           sizeof(ipa_hw_qmap_hdr_s) + pkt2_len - pkt_pad_len);
           
         IPA_TF_ASSERT(dsm_dup_len == 
                       (sizeof(ipa_hw_qmap_hdr_s) + pkt2_len - pkt_pad_len));
         deaggr_err_info->frame_cnt = *num_total_frames;
       }

       else if (deaggr_err_info->pkt_no_pad_index == 1)
       {
         /* Second pkt has no padding bytes. We have a valid First pkt and an 
            error frame(pkt2 + pkt3)
         */

         deaggr_err_info->pkt_cnt = * num_total_frames;
         
         /* First pkt and an error frame needs to be received*/
         pkt1_len = 
          deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][0].pkt_len;
         pkt2_len = 
          deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][1].pkt_len;
         pkt_pad_len = 
          deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][1].pad_len;

         /* Validate aggr frame */
         aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
         deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;

         /* dsm_dup_packets to separate out valid and invalid frames. */
         dsm_dup_len =
              dsm_dup_packet( &deaggr_err_info->err_frame_dsm_ptr, aggr_frame_dsm_ptr, 
                              (sizeof(ipa_hw_qmap_hdr_s) + pkt1_len),
                              (pkt2_len + sizeof(ipa_hw_qmap_hdr_s) - pkt_pad_len));

         IPA_TF_ASSERT_LOG((dsm_dup_len == 
                            pkt2_len + sizeof(ipa_hw_qmap_hdr_s) - pkt_pad_len), 
                            "dsm dup fail!", 0, 0, 0);

          /* Store lengths of frames */
          deaggr_err_info->eot_aggr_frame_len_arr[0] = aggr_frame_len;
          
          /*0->1st valid IP pkt, 1->error frame with pkt2+pkt3*/
          deaggr_err_info->frame_arr[0] = 
             pkt_arr_ptr[deaggr_err_info->pkt_no_pad_index - 1]->dsm_ptr;
          deaggr_err_info->frame_arr[1] = deaggr_err_info->err_frame_dsm_ptr;

          /* Now store the total set of frames in arr that needs to be enqueued.
            Frame1=pkt_1, Frame2=error_frame=pkt2+pkt3
          */         
          * num_total_frames = 2;  
          deaggr_err_info->frame_cnt = * num_total_frames;
       }
       else if (deaggr_err_info->pkt_no_pad_index == 2)
       {
          /* Third pkt has no padding bytes. We have valid first and second pkt and 
            an error frame(3rd pkt) */

          /* Store lengths of frames */
          deaggr_err_info->eot_aggr_frame_len_arr[0] = 
             dsm_length_packet (aggr_frame_dsm_ptr);

          /*0->1st valid IP pkt, 1->error frame with pkt2+pkt3*/
          deaggr_err_info->frame_arr[0] = pkt_arr_ptr[0]->dsm_ptr;
          deaggr_err_info->frame_arr[1] = pkt_arr_ptr[1]->dsm_ptr;

          /*3-> Error frame*/
          dsm_dup_len = 
               dsm_dup_packet( 
                 &deaggr_err_info->err_frame_dsm_ptr, aggr_frame_dsm_ptr, 
                 (pkt1_len + pkt2_len + 2 * sizeof(ipa_hw_qmap_hdr_s)),
                  sizeof(ipa_hw_qmap_hdr_s) + pkt3_len);

          deaggr_err_info->frame_arr[2] = deaggr_err_info->err_frame_dsm_ptr;

          * num_total_frames = 3;  
          deaggr_err_info->frame_cnt = * num_total_frames;

       }
       else
       {
         /* Both pkts have pad_len = 0 and are valid. Just enqueue the IP pkts. */

         aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
         deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
         deaggr_err_info->pkt_cnt = * num_total_frames;
         * num_total_frames = 2;
         deaggr_err_info->frame_cnt = * num_total_frames;

         deaggr_err_info->frame_arr[0] = pkt_arr_ptr[0]->dsm_ptr;
         deaggr_err_info->frame_arr[1] = pkt_arr_ptr[1]->dsm_ptr;
       }

    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE:
    {
      /*! Truncated QMAP hdr (<4 bytes)*/

      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->pkt_cnt = * num_total_frames;
      
      pkt1_len = 
         deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][0].pkt_len;
      pkt2_len = 
         deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][1].pkt_len;
      
      /*Pkt1->valid*/
      deaggr_err_info->frame_arr[0] = pkt_arr_ptr[0]->dsm_ptr;

      /*Pkt2->trimmed pkt with less than 4 bytes*/
      dsm_trim_packet(&aggr_frame_dsm_ptr , 
                      sizeof(ipa_hw_qmap_hdr_s) + pkt1_len + 3);
      IPA_TF_ASSERT(dsm_length_packet (aggr_frame_dsm_ptr) == 
                    (sizeof(ipa_hw_qmap_hdr_s) + pkt1_len + 3));

      dsm_dup_len =
              dsm_dup_packet( &deaggr_err_info->err_frame_dsm_ptr, 
                              aggr_frame_dsm_ptr, 
                              (sizeof(ipa_hw_qmap_hdr_s) + pkt1_len), 3);

      IPA_TF_ASSERT_LOG((dsm_dup_len == 3), 
                            "dsm dup fail!", 0, 0, 0);
      deaggr_err_info->frame_arr[1] = deaggr_err_info->err_frame_dsm_ptr;
      deaggr_err_info->frame_cnt = * num_total_frames;
    }
    break;

    
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE:
    {
      /*! msg_len > 16k */

      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->pkt_cnt = * num_total_frames;

      /* With 1st pkt being > 16k the whole frame would be received as an
         error frame.
      */
      pkt1_len = 
         deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][0].pkt_len;
      pkt2_len = 
         deaggr_err_info->qmap_hdr_info[deaggr_err_info->frame_cnt][1].pkt_len;

      dsm_dup_len = 
       dsm_dup_packet( &deaggr_err_info->err_frame_dsm_ptr, aggr_frame_dsm_ptr,
                       0,(pkt1_len + pkt2_len + 2 * sizeof(ipa_hw_qmap_hdr_s))
                     );
    
      IPA_TF_ASSERT_LOG((dsm_dup_len == dsm_length_packet(aggr_frame_dsm_ptr)), 
                            "dsm dup fail!", 0, 0, 0);
      deaggr_err_info->frame_arr[0] = deaggr_err_info->err_frame_dsm_ptr;

      * num_total_frames = 1;
      deaggr_err_info->frame_cnt = 1;

      
      
    }
    break;

    default:
    break;

  }
  
  return ret_code;

}

/*==============================================================================

  FUNCTION:  ipa_test_engine_gen_rndis_deaggr_err_frame

==============================================================================*/
/*!
  @brief
  Generate RNDIS error frame based on deaggr err cfg tests.
  Also generates the mix of valid pkts and error frames as expected from IPA 
  and enqueues in mirror wmk for comparison.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_gen_rndis_deaggr_err_frame
(
  ipa_test_engine_deaggr_err_cfg_s * deaggr_err_cfg,
  dsm_item_type                    * aggr_frame_dsm_ptr,
  ipa_test_engine_ip_pkt_info_s   ** pkt_arr_ptr,
  uint8                            * num_total_frames
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  ipa_test_engine_deaggr_err_info_s * deaggr_err_info =
    &ipa_test_engine.curr_test.deaggr_err_info;
  ipa_test_eth_hdr_s eth_hdr;
  uint16 pkt1_len, pkt2_len, pkt3_len, aggr_frame_len, dsm_dup_len;
  uint16 pkt_pad_len, pushdown_len = 0;
  uint16 * pull_up_bytes;

  switch (ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_EOT_LT_MIN_FRAME_SIZE:
    {
      /*! Truncated RNDIS hdr (Less than (44+14) bytes)*/

      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
         aggr_frame_len;
      pkt1_len = deaggr_err_info->rndis_hdr_info \
                          [deaggr_err_info->frame_cnt][0].message_len;
      pkt2_len = deaggr_err_info->rndis_hdr_info \
                          [deaggr_err_info->frame_cnt][1].message_len;

      IPA_TF_ASSERT(aggr_frame_len == pkt1_len + pkt2_len);

      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->pkt_cnt            = * num_total_frames;
      
      
      
      /*Pkt1->valid*/
      /*We get back Eth+IP pkt from IPA in UL. So prepend eth hdr 
      to the IP pkt and then enqueue to the mirror wmk for comparison */
      IPA_MEMSET(&eth_hdr, 0, sizeof(ipa_test_eth_hdr_s));
      IPA_MEMSET(eth_hdr.dst_mac_addr,0xAA, IPA_TEST_ETH_MAC_ADDR_LEN);
      IPA_MEMSET(eth_hdr.src_mac_addr,0x11, IPA_TEST_ETH_MAC_ADDR_LEN);
      eth_hdr.ether_type = (uint16) IPA_TEST_ETHER_TYPE_IPV4;

      pushdown_len = dsm_pushdown(&pkt_arr_ptr[0]->dsm_ptr, &eth_hdr, 
                                  IPA_SIO_ETHERNET_HDR_LEN, 
                                  IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
      
      deaggr_err_info->frame_arr[0] = pkt_arr_ptr[0]->dsm_ptr;

      /*Pkt2->trimmed pkt with less than rndis hdr bytes for the 2nd pkt */
      dsm_trim_packet(&aggr_frame_dsm_ptr , pkt1_len + 54);
      IPA_TF_ASSERT(dsm_length_packet (aggr_frame_dsm_ptr) == 
                    pkt1_len + 54);

      dsm_dup_len =
              dsm_dup_packet( &deaggr_err_info->err_frame_dsm_ptr, 
                              aggr_frame_dsm_ptr, pkt1_len, 54);

      IPA_TF_ASSERT_LOG((dsm_dup_len == 54), 
                            "dsm dup fail!", 0, 0, 0);
      deaggr_err_info->frame_arr[1] = deaggr_err_info->err_frame_dsm_ptr;
      deaggr_err_info->frame_cnt = * num_total_frames;
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_SPLIT_2_DESC:
    {
       /*! RNDIS hdr split across 2 buffers */

      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
         aggr_frame_len;
      pkt1_len = deaggr_err_info->rndis_hdr_info \
                          [deaggr_err_info->frame_cnt][0].message_len;
      pkt2_len = deaggr_err_info->rndis_hdr_info \
                          [deaggr_err_info->frame_cnt][1].message_len;

      IPA_TF_ASSERT(aggr_frame_len == pkt1_len + pkt2_len);

      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->pkt_cnt            = * num_total_frames;

       /*Pkt1->valid*/

       /* We get back Eth+IP pkt from IPA in UL. So prepend eth hdr 
          to the IP pkt and then enqueue to the mirror wmk for comparison */
      IPA_MEMSET(&eth_hdr, 0, sizeof(ipa_test_eth_hdr_s));
      IPA_MEMSET(eth_hdr.dst_mac_addr, 0xAA, IPA_TEST_ETH_MAC_ADDR_LEN);
      IPA_MEMSET(eth_hdr.src_mac_addr, 0x11, IPA_TEST_ETH_MAC_ADDR_LEN);
      eth_hdr.ether_type = (uint16) IPA_TEST_ETHER_TYPE_IPV4;

      pushdown_len = dsm_pushdown(&pkt_arr_ptr[0]->dsm_ptr, &eth_hdr, 
                                  IPA_SIO_ETHERNET_HDR_LEN, 
                                  IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
      deaggr_err_info->frame_arr[0] = pkt_arr_ptr[0]->dsm_ptr;

      /*Pkt2-->valid*/
      pushdown_len = dsm_pushdown(&pkt_arr_ptr[1]->dsm_ptr, &eth_hdr, 
                                  IPA_SIO_ETHERNET_HDR_LEN, 
                                  IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
      deaggr_err_info->frame_arr[1] = pkt_arr_ptr[1]->dsm_ptr;

      deaggr_err_info->frame_cnt = * num_total_frames;
    }
    break;

    
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_TYPE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_OFFSET:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_LEN:
    {
       /*! RNDIS invalid msg type in the 2nd hdr */

      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
         aggr_frame_len;

      pkt1_len = deaggr_err_info->rndis_hdr_info \
                          [deaggr_err_info->frame_cnt][0].message_len;
      pkt2_len = deaggr_err_info->rndis_hdr_info \
                          [deaggr_err_info->frame_cnt][1].message_len;
      pkt3_len = deaggr_err_info->rndis_hdr_info \
                          [deaggr_err_info->frame_cnt][2].message_len;

      IPA_TF_ASSERT(aggr_frame_len == pkt1_len + pkt2_len + pkt3_len);

      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->pkt_cnt            = * num_total_frames;

       /*Pkt1->valid*/

       /* We get back Eth+IP pkt from IPA in UL. So prepend eth hdr 
          to the IP pkt and then enqueue to the mirror wmk for comparison */
      IPA_MEMSET(&eth_hdr, 0, sizeof(ipa_test_eth_hdr_s));
      IPA_MEMSET(eth_hdr.dst_mac_addr, 0xAA, IPA_TEST_ETH_MAC_ADDR_LEN);
      IPA_MEMSET(eth_hdr.src_mac_addr, 0x11, IPA_TEST_ETH_MAC_ADDR_LEN);
      eth_hdr.ether_type = (uint16) IPA_TEST_ETHER_TYPE_IPV4;

      pushdown_len = dsm_pushdown(&pkt_arr_ptr[0]->dsm_ptr, &eth_hdr, 
                                  IPA_SIO_ETHERNET_HDR_LEN, 
                                  IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
      deaggr_err_info->frame_arr[0] = pkt_arr_ptr[0]->dsm_ptr;

      /*Pkt2+pkt3 = error frame*/

      uint16 dsm_dup_len = 
           dsm_dup_packet( &deaggr_err_info->err_frame_dsm_ptr, 
                           aggr_frame_dsm_ptr, pkt1_len, pkt2_len + pkt3_len );
           
      IPA_TF_ASSERT_LOG((dsm_dup_len == pkt2_len + pkt3_len ),
                         "dsm dup fail!", 0, 0, 0);
         
      /*Validate error frame*/
      aggr_frame_len = dsm_length_packet(deaggr_err_info->err_frame_dsm_ptr);
      IPA_TF_ASSERT(aggr_frame_len == pkt2_len + pkt3_len);
         
      deaggr_err_info->frame_arr[1] = deaggr_err_info->err_frame_dsm_ptr;

      * num_total_frames = 2;
      deaggr_err_info->frame_cnt = * num_total_frames;
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_LEN:
    {
      /*! RNDIS invalid data len in the 2nd hdr */

      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
       aggr_frame_len;

      pkt1_len = deaggr_err_info->rndis_hdr_info \
                        [deaggr_err_info->frame_cnt][0].message_len;
      pkt2_len = deaggr_err_info->rndis_hdr_info \
                        [deaggr_err_info->frame_cnt][1].message_len;
      pkt3_len = deaggr_err_info->rndis_hdr_info \
                        [deaggr_err_info->frame_cnt][2].message_len;

      IPA_TF_ASSERT(aggr_frame_len == pkt1_len + pkt2_len + pkt3_len);

      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->pkt_cnt            = * num_total_frames;

      /*Pkt1->valid*/

      /* We get back Eth+IP pkt from IPA in UL. So prepend eth hdr 
        to the IP pkt and then enqueue to the mirror wmk for comparison */
      IPA_MEMSET(&eth_hdr, 0, sizeof(ipa_test_eth_hdr_s));
      IPA_MEMSET(eth_hdr.dst_mac_addr, 0xAA, IPA_TEST_ETH_MAC_ADDR_LEN);
      IPA_MEMSET(eth_hdr.src_mac_addr, 0x11, IPA_TEST_ETH_MAC_ADDR_LEN);
      eth_hdr.ether_type = (uint16) IPA_TEST_ETHER_TYPE_IPV4;
      pushdown_len = dsm_pushdown(&pkt_arr_ptr[0]->dsm_ptr, &eth_hdr, 
                                IPA_SIO_ETHERNET_HDR_LEN, 
                                IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
      deaggr_err_info->frame_arr[0] = pkt_arr_ptr[0]->dsm_ptr;

      /*Pkt2->valid*/
      pushdown_len = dsm_pushdown(&pkt_arr_ptr[1]->dsm_ptr, &eth_hdr, 
                                IPA_SIO_ETHERNET_HDR_LEN, 
                                IPA_CFG_SL_DSM_ITEM_POOL);
      IPA_TF_ASSERT(IPA_SIO_ETHERNET_HDR_LEN == pushdown_len);
      deaggr_err_info->frame_arr[1] = pkt_arr_ptr[1]->dsm_ptr;


      /*pkt3 = error frame*/

      uint16 dsm_dup_len = 
         dsm_dup_packet( &deaggr_err_info->err_frame_dsm_ptr, 
                         aggr_frame_dsm_ptr, pkt1_len + pkt2_len, pkt3_len );

      IPA_TF_ASSERT_LOG((dsm_dup_len == pkt3_len ),
                       "dsm dup fail!", 0, 0, 0);

      /*Validate error frame*/
      aggr_frame_len = dsm_length_packet(deaggr_err_info->err_frame_dsm_ptr);
      IPA_TF_ASSERT(aggr_frame_len == pkt3_len);

      deaggr_err_info->frame_arr[2] = deaggr_err_info->err_frame_dsm_ptr;

      deaggr_err_info->frame_cnt = * num_total_frames; //3 frames
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE:
    {
      /*! RNDIS msg > max_msg_size configured in deaggr.max_pkt_zie */

      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
       aggr_frame_len;

      pkt1_len = deaggr_err_info->rndis_hdr_info \
                        [deaggr_err_info->frame_cnt][0].message_len;
      pkt2_len = deaggr_err_info->rndis_hdr_info \
                        [deaggr_err_info->frame_cnt][1].message_len;
      pkt3_len = deaggr_err_info->rndis_hdr_info \
                        [deaggr_err_info->frame_cnt][2].message_len;

      IPA_TF_ASSERT(aggr_frame_len == pkt1_len + pkt2_len + pkt3_len);

      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->pkt_cnt            = * num_total_frames;

      /*pkt1+pkt2+pkt3 = error frame*/
      uint16 dsm_dup_len = 
         dsm_dup_packet(&deaggr_err_info->err_frame_dsm_ptr,aggr_frame_dsm_ptr, 
                         0, aggr_frame_len );

      IPA_TF_ASSERT_LOG((dsm_dup_len == aggr_frame_len ),
                       "dsm dup fail!", 0, 0, 0);

      deaggr_err_info->frame_arr[0] = deaggr_err_info->err_frame_dsm_ptr;

      * num_total_frames = 1;
      deaggr_err_info->frame_cnt = * num_total_frames; //1 error frame

    }
    break;

    default:
    break;
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_gen_mbim_deaggr_err_frame

==============================================================================*/
/*!
  @brief
  Generate RNDIS error frame based on deaggr err cfg tests.
  Also generates the mix of valid pkts and error frames as expected from IPA 
  and enqueues in mirror wmk for comparison.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_gen_mbim_deaggr_err_frame
(
  ipa_test_engine_deaggr_err_cfg_s * deaggr_err_cfg,
  dsm_item_type                    * aggr_frame_dsm_ptr,
  ipa_test_engine_ip_pkt_info_s   ** pkt_arr_ptr,
  uint8                            * num_total_frames
)
{
  uint16 aggr_frame_len = 0;
  uint16 dsm_dup_len = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  ipa_test_engine_deaggr_err_info_s * deaggr_err_info =
    &ipa_test_engine.curr_test.deaggr_err_info;

  switch (ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_SIG:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_SIG:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NXT_NDP_IDX:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_DGRM_IDX:
    {
      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      
      /* Need to Dup the complete aggr frame since the same frame will be enqueued in both
          mirror wmk and per sim wmk.
      */
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
         aggr_frame_len;

      /* Store total aggr pkt cnt got from gen_aggr_pkt. */
      deaggr_err_info->pkt_cnt += *num_total_frames; 

      /* Set to 1 since one frame will be enqueued each time to mirror wmk 
         and not the ip pkts.*/
      * num_total_frames = 1; 

      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->frame_arr_idx      = deaggr_err_info->frame_cnt;
      
      dsm_dup_len = dsm_dup_packet( 
                    &deaggr_err_info->frame_arr[deaggr_err_info->frame_arr_idx], 
                    aggr_frame_dsm_ptr, 0, aggr_frame_len);
           
      IPA_TF_ASSERT_LOG(dsm_dup_len == aggr_frame_len, "dsm dup fail!", 0, 0, 0);
      
      /*Final verification of src and duped framed ptr.*/
      IPA_TF_ASSERT(dsm_length_packet (aggr_frame_dsm_ptr) == 
                    dsm_length_packet (deaggr_err_info->frame_arr \
                                     [deaggr_err_info->frame_arr_idx]));

      /* If total_num_pkts>max_aggr_pkts multiple frames will be generated */
      deaggr_err_info->frame_cnt++;

    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOT_LT_MIN_FRAME_SIZE:
    {
      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
      deaggr_err_info->pkt_cnt += * num_total_frames;
      deaggr_err_info->frame_arr_idx      = deaggr_err_info->frame_cnt;

      /* Truncate the MBIM frame to 12 bytes of NTH hdr */
      dsm_trim_packet(&aggr_frame_dsm_ptr , sizeof(ipa_test_nth_hdr_s));
      IPA_TF_ASSERT(dsm_length_packet (aggr_frame_dsm_ptr) == 
                    sizeof(ipa_test_nth_hdr_s));

     *num_total_frames = 1; 

      dsm_dup_len = dsm_dup_packet( 
                    &deaggr_err_info->frame_arr[deaggr_err_info->frame_arr_idx], 
                    aggr_frame_dsm_ptr, 0, dsm_length_packet (aggr_frame_dsm_ptr));
           
      IPA_TF_ASSERT_LOG(dsm_dup_len == dsm_length_packet (aggr_frame_dsm_ptr), 
                        "dsm dup fail!", 0, 0, 0);
      
      /*Final verification of src and duped framed ptr.*/
      IPA_TF_ASSERT(dsm_length_packet (aggr_frame_dsm_ptr) == 
                    dsm_length_packet (deaggr_err_info->frame_arr \
                                     [deaggr_err_info->frame_arr_idx]));

      /* If total_num_pkts>max_aggr_pkts multiple frames will be generated */
      deaggr_err_info->frame_cnt++;
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_SEC_EOT_FRAME:
    {

      /* Store frame and pkt info */
      aggr_frame_len = dsm_length_packet (aggr_frame_dsm_ptr);
      deaggr_err_info->eot_aggr_frame_len_arr[deaggr_err_info->frame_cnt] =
         aggr_frame_len;
      deaggr_err_info->pkt_cnt += *num_total_frames; 
      deaggr_err_info->frame_arr_idx      = deaggr_err_info->frame_cnt;

      /*!Second Error EOT frame*/
      if (deaggr_err_info->frame_cnt)
      {
        /* Set to 1 since one frame will be enqueued each time to mirror wmk 
         and not the ip pkts.*/
        * num_total_frames = 1; 

        deaggr_err_info->aggr_frame_dsm_ptr = aggr_frame_dsm_ptr;
        dsm_dup_len = dsm_dup_packet( 
                    &deaggr_err_info->frame_arr[deaggr_err_info->frame_arr_idx], 
                    aggr_frame_dsm_ptr, 0, aggr_frame_len);
           
        IPA_TF_ASSERT_LOG(dsm_dup_len == aggr_frame_len, "dsm dup fail!", 0, 0, 0);
      
        /*Final verification of src and duped framed ptr.*/
        IPA_TF_ASSERT(dsm_length_packet (aggr_frame_dsm_ptr) == 
                    dsm_length_packet (deaggr_err_info->frame_arr \
                                     [deaggr_err_info->frame_arr_idx]));
      }
      else
      {
        /* First frame has valid pkt. Since the valid pkt is in the separate 
           EOT frame usual ul_tx_pkts() flow will take care of it and enqueue in
           the mirror wmk.
        */
         deaggr_err_info->frame_arr[deaggr_err_info->frame_arr_idx] = 
              pkt_arr_ptr[ deaggr_err_info->frame_arr_idx]->dsm_ptr;
      }

      deaggr_err_info->frame_cnt++;
    }
    break;

    default:
    break;
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_gen_deaggr_err_frame

==============================================================================*/
/*!
  @brief
  Generate an error frame based on deaggr err cfg tests.
  Also generates the mix of valid pkts and error frames as expected from IPA 
  and enqueues in mirror wmk for comparison.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_gen_deaggr_err_frame
(
  ipa_test_engine_deaggr_err_cfg_s * deaggr_err_cfg,
  dsm_item_type                    * aggr_frame_dsm_ptr,
  ipa_test_engine_ip_pkt_info_s   ** pkt_arr_ptr,
  uint8                            * num_total_frames
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  switch (deaggr_err_cfg->err_type)
  {
    /*!QMAP*/
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1:     
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE:
    {
      ret_code = ipa_test_engine_gen_qmap_deaggr_err_frame(
                                                 deaggr_err_cfg,
                                                 aggr_frame_dsm_ptr,
                                                 pkt_arr_ptr,
                                                 num_total_frames);
       IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);  
    }
    break;

    /*!RNDIS*/
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_SPLIT_2_DESC:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_TYPE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_LEN:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_OFFSET:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_LEN:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE:
    {
      ret_code = ipa_test_engine_gen_rndis_deaggr_err_frame( 
                                                 deaggr_err_cfg,
                                                 aggr_frame_dsm_ptr,
                                                 pkt_arr_ptr,
                                                 num_total_frames);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);  
    }
    break;

    /*!MBIM*/
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_SIG:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_HDR_LEN:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_IDX_IN_NTH:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_SIG:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NXT_NDP_IDX:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_DGRM_IDX:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_NDP_WITH_NO_TERM_ZEROS:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_SEC_EOT_FRAME:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOTLEN_GT_MAX_MSG_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOT_LT_MIN_FRAME_SIZE:
    {
      ret_code = ipa_test_engine_gen_mbim_deaggr_err_frame( 
                                                 deaggr_err_cfg,
                                                 aggr_frame_dsm_ptr,
                                                 pkt_arr_ptr,
                                                 num_total_frames);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);  
    }
    break;

    default:
    break;
  }

  return ret_code;
}


/*==============================================================================

  FUNCTION:  ipa_test_engine_dl_tx_pkts

==============================================================================*/
/*!
  @brief
  1. Generate aggregated frame and send it to mirror watermark
  2. Send pkts over a random bearer or SIO port

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_dl_tx_pkts(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  ipa_err_code_e err_code = IPA_SUCCESS;
  uint16 pushdown_len = 0, dpl_pkt_len = 0;
  uint16 i = 0, j = 0;
  dsm_item_type *mirror_dsm_ptr = NULL, *mirror_dpl_dsm_ptr = NULL, *hw_rep_dsm_ptr = NULL;
  dsm_watermark_type *dst_wm_ptr = NULL, *mirror_wm_ptr = NULL, *hw_rep_wm_ptr = NULL;
  dsm_watermark_type *mirror_dpl_wm_ptr = NULL;
  ipa_test_engine_pkt_trace_elem_s pkt, dpl_pkt;
  uint8 bearer_idx = 0, log_ch_idx = 0, is_bearer = 0;
  uint8 tot_rx_paths, rx_path = 0;
  uint8 rx_path_arr[IPA_TEST_ENGINE_LOG_CH_NUM_MAX];
  ipa_test_engine_ip_pkt_dl_alloc_bucket_s *dl_alloc_ptr = NULL;
  uint32 pc_sleep_rand = 0;
  uint16 pkt_csum_val;
  uint16 pkt_csum_len;

  /* Memset the pkt trace to 0 */
  IPA_MEMSET(&pkt, 0, sizeof(ipa_test_engine_pkt_trace_elem_s));
  IPA_MEMSET(&dpl_pkt, 0, sizeof(ipa_test_engine_pkt_trace_elem_s));

  IPA_TF_ASSERT((ipa_test_engine.ip_pkt.curr_num_pkts > 0) &&
                (ipa_test_engine.ip_pkt.curr_num_pkts <= 
                 IPA_TEST_ENGINE_IP_PKT_NUM_MAX));

  /* First process the IP pkts */
  ret_code = ipa_test_engine_dl_process_ip_pkts();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  /* Register the DS event cb with the driver*/
    err_code =
    ipa_wani_ds_register_event_cb((ipa_wan_ds_event_cb_type)
                                  ipa_test_engine_wan_event_cb_fn);
    IPA_ASSERT(err_code == IPA_SUCCESS);

  /*! update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_DL_TX);

  /* Send pkts for one RX path in one go, because the DL programming will reorder the pkts
     when they're recvd on the other end */
  if (ipa_test_engine.wan.curr_bearer_cnt == 0)
  {
    tot_rx_paths = ipa_test_engine.endp.curr_log_ch_cnt;
    is_bearer = FALSE;
  }
  else
  {
    tot_rx_paths = ipa_test_engine.wan.curr_bearer_cnt;
    is_bearer = TRUE;
  }

  /* Calculate the order of pkts to send. Let's not use simple round robin! */
  for (i = 0; i < tot_rx_paths; i++)
  {
    rx_path_arr[i] = 
      ipa_test_generate_unique_random_num(tot_rx_paths - 1, 0, rx_path_arr, i);
  }

  if(ipa_test_engine.curr_test.cfg.wan_cfg.is_bearer_susp_case_2 == TRUE)
  {
    /* Send the bearer suspend comand to the driver*/
    ipa_wani_ds_set_bearer_proc_state(ipa_test_engine.wan.bearer[0].uid, 
                                      IPA_WAN_BEARER_PROC_STATE_SUSPEND);
        
    /* Wait for the DS event cb*/
    ipa_test_engine_bearer_susp_evnt_wait_end();
  }
  for (i = 0; i < tot_rx_paths; i++)
  {
    rx_path = rx_path_arr[i];
    dl_alloc_ptr = &ipa_test_engine.ip_pkt.dl_alloc_buckets[rx_path];

    for (j = 0; j < dl_alloc_ptr->num_pkts; j++)
    {
      if (is_bearer == TRUE)
      {
        bearer_idx = rx_path;

        if ((ipa_test_engine.wan.bearer[bearer_idx].is_bridged == TRUE) &&
            (dl_alloc_ptr->pkt_arr[j]->filter_result == 0))
        {
          dst_wm_ptr = &ipa_test_engine.wan.bearer[bearer_idx].pdcp_wm;
          mirror_wm_ptr = &ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr-> \
                      phy_ch_ptr->per_sim.mirror_from_ipa_wm;
          if (ipa_test_engine.curr_test.cfg.is_dpl_enabled == TRUE)
          {
            mirror_dpl_wm_ptr = &ipa_test_engine.dpl.mirror_dpl_wm;
          }
          else if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE)
		      {
		  	    hw_rep_wm_ptr = &ipa_test_engine.endp.phy_ch[1].per_sim.mirror_from_ipa_wm_dl;
		      }          

          /****! Store the pkt trace ***/
          pkt.dst = IPA_TEST_ENGINE_PKT_DST_SIM;
          pkt.src = IPA_TEST_ENGINE_PKT_SRC_BEARER_BRIDGED;
          pkt.src_idx = bearer_idx;
          pkt.dst_idx = ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr-> \
                        phy_ch_ptr->idx;
        }
        else
        {
          dst_wm_ptr = &ipa_test_engine.wan.bearer[bearer_idx].pdcp_wm;
          mirror_wm_ptr = &ipa_test_engine.wan.bearer[bearer_idx].mirror_dl_wm;

          if (ipa_test_engine.wan.bearer[bearer_idx].is_bridged == FALSE)
          {
            /****! Store the pkt trace ***/
            pkt.dst = IPA_TEST_ENGINE_PKT_DST_SIM;
            pkt.src = IPA_TEST_ENGINE_PKT_SRC_BEARER_UNBRIDGED;
            pkt.src_idx = bearer_idx;
            pkt.dst_idx = ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr-> \
                          phy_ch_ptr->idx;
          }
          else
          {
           /****! Store the pkt trace ***/
            pkt.dst = IPA_TEST_ENGINE_PKT_DST_BEARER_DL;
            pkt.src = IPA_TEST_ENGINE_PKT_SRC_BEARER_BRIDGED;
            pkt.src_idx = bearer_idx;
            pkt.dst_idx = bearer_idx;
			
            if(ipa_test_engine.curr_test.cfg.is_dpl_enabled == TRUE)
            {
              mirror_dpl_wm_ptr = &ipa_test_engine.dpl.mirror_dpl_wm;
            }
            
          }
        }

        if ((ipa_test_engine.wan.bearer[bearer_idx].is_bridged == TRUE) &&
           (ipa_test_engine.curr_test.cfg.is_dpl_enabled == TRUE))
        {
          dpl_pkt_len = (uint16)IPA_MIN(ipa_test_engine.wan.bearer[bearer_idx].dpl_bytes_to_log,
                                        dl_alloc_ptr->pkt_arr[j]->pkt_len);

          pushdown_len = 
            dsm_pushdown(&mirror_dpl_dsm_ptr,
                     dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                     dpl_pkt_len,
                     IPA_CFG_SL_DSM_ITEM_POOL);
          IPA_TF_ASSERT(pushdown_len == dpl_pkt_len);

          /*! The CB data expected */
          mirror_dpl_dsm_ptr->app_field =  bearer_idx;

           /* Now send the pkt to the DPL watermark */
          dsm_enqueue(mirror_dpl_wm_ptr, &mirror_dpl_dsm_ptr);
          IPA_TF_ASSERT(mirror_dpl_dsm_ptr == NULL);

          dpl_pkt = pkt;
          dpl_pkt.dst = IPA_TEST_ENGINE_PKT_DST_DPL;
          /*! Store the ip version of the packet that was enqueued in the pkt trace.*/
          pkt.ip_ver = dl_alloc_ptr->pkt_arr[j]->ip_pkt_version;
          ipa_test_engine_store_tx_pkt_trace(&pkt);
        }
		else if((ipa_test_engine.wan.bearer[bearer_idx].is_bridged == TRUE) &&
           (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE))
		{

				dpl_pkt_len = (uint16)IPA_MIN(256, dl_alloc_ptr->pkt_arr[j]->pkt_len);
				pushdown_len = 
        				dsm_pushdown(&hw_rep_dsm_ptr,
                     	dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                     	dpl_pkt_len,
                     	IPA_CFG_SL_DSM_ITEM_POOL);
        		IPA_TF_ASSERT(pushdown_len == dpl_pkt_len);

		}

		
      }
      else
      {
        log_ch_idx = rx_path;

        dst_wm_ptr = &ipa_test_engine.endp.log_ch[log_ch_idx].tx_wm;
        mirror_wm_ptr = &ipa_test_engine.endp.log_ch[log_ch_idx].phy_ch_ptr->per_sim.mirror_from_ipa_wm;

        /****! Store the pkt trace ***/
        pkt.dst = IPA_TEST_ENGINE_PKT_DST_SIM;
        pkt.src = IPA_TEST_ENGINE_PKT_SRC_LOG_CHANNEL;
        pkt.src_idx = log_ch_idx;
        pkt.dst_idx = ipa_test_engine.endp.log_ch[log_ch_idx] \
                      .phy_ch_ptr->idx;
      }

      /*! For OOO frag cases the packets are in the reverse order in the mirror watermark.*/
      if (ipa_test_engine.curr_test.cfg.ip_pkt_cfg.params.ooo_frag_case == TRUE)
      {
        /* For cases where the packets are not filtered the first frag comes first 
          and then the remaining packets arrive in reverse order. */
        if ((j == 0) && (ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == FALSE))
        {
           pushdown_len = 
           dsm_pushdown(&mirror_dsm_ptr,
                       dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                       dl_alloc_ptr->pkt_arr[j]->pkt_len,
                       IPA_CFG_SL_DSM_ITEM_POOL);
           IPA_TF_ASSERT(pushdown_len == 
                      dl_alloc_ptr->pkt_arr[j]->pkt_len);

           /****! Store the pkt trace ***/
           pkt.pkt_len = dl_alloc_ptr->pkt_arr[j]->pkt_len;
           pkt.filter_result = dl_alloc_ptr->pkt_arr[j]->filter_result;
           pkt.ciph_params.value = (uint32)dl_alloc_ptr->pkt_arr[j]->dsm_ptr->app_ptr;
           pkt.ciph_count = dl_alloc_ptr->pkt_arr[j]->dsm_ptr->app_field;

           /* Store the mux ID for some comparison */
           mirror_dsm_ptr->kind = dl_alloc_ptr->pkt_arr[j]->mux_id;
           IPA_IPFLTR_SET_DL_FLTR_RESULT(mirror_dsm_ptr,  
                                   dl_alloc_ptr->pkt_arr[j]->filter_result);

           /* Set packet checksum fields */
           IPA_TF_ASSERT(ipa_test_engine_set_dsm_checksum (
                dl_alloc_ptr->pkt_arr[j],
                mirror_dsm_ptr,
                &pkt) == IPA_TEST_RET_CODE_SUCCESS);

           IPA_DSM_SET_IP_VERSION_PARAM(&mirror_dsm_ptr, 
                        dl_alloc_ptr->pkt_arr[j]->ip_pkt_version);

           /* Now send the pkt to the SIM watermark */
           IPA_TF_ASSERT(mirror_dsm_ptr != NULL);
           dsm_enqueue(mirror_wm_ptr, &mirror_dsm_ptr);
           IPA_TF_ASSERT(mirror_dsm_ptr == NULL);
      
           pushdown_len = 
           dsm_pushdown(&mirror_dsm_ptr,
                       dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j -1]->pkt_buf_ptr,
                       dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->pkt_len,
                       IPA_CFG_SL_DSM_ITEM_POOL);
           IPA_TF_ASSERT(pushdown_len == 
                      dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->pkt_len);

           /****! Store the pkt trace ***/
           pkt.pkt_len = dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j -1]->pkt_len;
           pkt.filter_result = 
             dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->filter_result;
           pkt.ciph_params.value = 
             (uint32)dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->dsm_ptr->app_ptr;
           pkt.ciph_count = 
             dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->dsm_ptr->app_field;

           /* Store the mux ID for some comparison */
           mirror_dsm_ptr->kind = 
             dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts - j - 1)]->mux_id;
           IPA_IPFLTR_SET_DL_FLTR_RESULT(mirror_dsm_ptr,
                                      dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts - j - 1)]->filter_result);

           /* Set packet checksum fields */
           IPA_TF_ASSERT(ipa_test_engine_set_dsm_checksum (
                 dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1],
                 mirror_dsm_ptr,
                 &pkt) == IPA_TEST_RET_CODE_SUCCESS); 
           IPA_DSM_SET_IP_VERSION_PARAM(&mirror_dsm_ptr, 
             dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->ip_pkt_version);
        }
        else
        {
          pushdown_len = 
           dsm_pushdown(&mirror_dsm_ptr,
                       dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->pkt_buf_ptr,
                       dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->pkt_len,
                       IPA_CFG_SL_DSM_ITEM_POOL);
           IPA_TF_ASSERT(pushdown_len == 
                      dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->pkt_len);

           /****! Store the pkt trace ***/
           pkt.pkt_len = dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->pkt_len;
           pkt.filter_result = dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->filter_result;
           pkt.ciph_params.value = (uint32)dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->dsm_ptr->app_ptr;
           pkt.ciph_count = dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->dsm_ptr->app_field;

           /* Store the mux ID for some comparison */
           mirror_dsm_ptr->kind = dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts - j)- 1]->mux_id;
           IPA_IPFLTR_SET_DL_FLTR_RESULT(mirror_dsm_ptr,  
                                   dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts - j)- 1]->filter_result);

           /* Set packet checksum fields */
           IPA_TF_ASSERT(ipa_test_engine_set_dsm_checksum (
             dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts - j)- 1],
             mirror_dsm_ptr,
             &pkt) == IPA_TEST_RET_CODE_SUCCESS);
           IPA_DSM_SET_IP_VERSION_PARAM(&mirror_dsm_ptr, 
             dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts - j)- 1]->ip_pkt_version);
        }
        /* Now send the pkt to the SIM watermark */
        IPA_TF_ASSERT(mirror_dsm_ptr != NULL);
        dsm_enqueue(mirror_wm_ptr, &mirror_dsm_ptr);
        IPA_TF_ASSERT(mirror_dsm_ptr == NULL);
      }
      else if ( !((ipa_test_engine.curr_test.cfg.wan_cfg.is_bearer_dereg_case == TRUE) &&
                 (j > (dl_alloc_ptr->num_pkts / 2)- 1)) )
      {
        /*! Check if there is a valid bearer and has CLAT enabled on it */
        if( (ipa_test_engine.curr_test.cfg.is_clat_enabled == TRUE) &&
            (is_bearer == TRUE) && 
            (ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr->is_clat == TRUE) &&
            (IPA_TEST_GET_IP_VERSION(dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr) == 6) &&
            (ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == FALSE))
        {
          if (ipa_test_engine.curr_test.cfg.test_type != IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK)
          {
            ret_code = ipa_test_engine_enqueue_ipv4_clat_frame(mirror_wm_ptr, 
                                                    dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                                                    dl_alloc_ptr->pkt_arr[j]->pkt_len,
                                                    dl_alloc_ptr->pkt_arr[j]->extn_hdr_len,
                                                               ipa_test_engine.wan.bearer[bearer_idx].\
                                                               log_ch_ptr->clat_cntxt_ptr);
            IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
        }
          else
          {
            ret_code = ipa_test_engine_enqueue_ipv4_clat_frame(mirror_wm_ptr, 
                                                    dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                                                    dl_alloc_ptr->pkt_arr[j]->pkt_len,
                                                               ipa_test_engine.wan.bearer[bearer_idx].\
                                                               log_ch_ptr->clat_cntxt_ptr->extn_hdr_len,
                                                               ipa_test_engine.wan.bearer[bearer_idx].\
                                                               log_ch_ptr->clat_cntxt_ptr);
            IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
        }
          /* Store if xlat is conversion expected on the packet */
          pkt.xlat =  TRUE;
      }
        /*! Check if logical channel has CLAT enabled. */
        else if( (is_bearer == FALSE) && (ipa_test_engine.curr_test.cfg.is_clat_enabled == TRUE) && 
                 (ipa_test_engine.endp.log_ch[log_ch_idx].is_clat == TRUE) &&
                 (IPA_TEST_GET_IP_VERSION(dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr) == 6)&&
                 (ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == FALSE))
        {
          if (ipa_test_engine.curr_test.cfg.test_type != IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK)
          {
            ret_code = ipa_test_engine_enqueue_ipv4_clat_frame(mirror_wm_ptr, 
                                                    dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                                                    dl_alloc_ptr->pkt_arr[j]->pkt_len,
                                                    dl_alloc_ptr->pkt_arr[j]->extn_hdr_len,
                                                    ipa_test_engine.endp.log_ch[log_ch_idx].clat_cntxt_ptr);
            IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
          }
          else
          {
            ret_code = ipa_test_engine_enqueue_ipv4_clat_frame(mirror_wm_ptr, 
                                                    dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                                                    dl_alloc_ptr->pkt_arr[j]->pkt_len,
                                                    ipa_test_engine.endp.log_ch[log_ch_idx].clat_cntxt_ptr->extn_hdr_len,
                                                    ipa_test_engine.endp.log_ch[log_ch_idx].clat_cntxt_ptr);
            IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
          }
          /* Store if xlat is conversion expected on the packet */
          pkt.xlat =  TRUE;
        }
        /*! General scenario with no CLAT/ the SIO is not configured for CLAT*/
        else
        {
          pushdown_len = 
            dsm_pushdown(&mirror_dsm_ptr,
                     dl_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                     dl_alloc_ptr->pkt_arr[j]->pkt_len,
                     IPA_CFG_SL_DSM_ITEM_POOL);
          IPA_TF_ASSERT(pushdown_len == 
                    dl_alloc_ptr->pkt_arr[j]->pkt_len);

		  if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE)
		  {
			dsm_enqueue(hw_rep_wm_ptr, &hw_rep_dsm_ptr);
		  }
     
          /****! Store the pkt trace ***/
          pkt.pkt_len = dl_alloc_ptr->pkt_arr[j]->pkt_len;
          pkt.filter_result = dl_alloc_ptr->pkt_arr[j]->filter_result;
          pkt.ciph_params.value = (uint32)dl_alloc_ptr->pkt_arr[j]->dsm_ptr->app_ptr;
          pkt.ciph_count = dl_alloc_ptr->pkt_arr[j]->dsm_ptr->app_field;
          /* Store if xlat is conversion expected on the packet */
          pkt.xlat =  FALSE;

          /* Store the mux ID for some comparison */
          mirror_dsm_ptr->kind = dl_alloc_ptr->pkt_arr[j]->mux_id;
          IPA_IPFLTR_SET_DL_FLTR_RESULT(mirror_dsm_ptr,  
                                   dl_alloc_ptr->pkt_arr[j]->filter_result);
           /* Set packet checksum fields */
           IPA_TF_ASSERT(ipa_test_engine_set_dsm_checksum (
             dl_alloc_ptr->pkt_arr[j],
             mirror_dsm_ptr,
             &pkt) == IPA_TEST_RET_CODE_SUCCESS);
           IPA_DSM_SET_IP_VERSION_PARAM(&mirror_dsm_ptr, 
             dl_alloc_ptr->pkt_arr[j]->ip_pkt_version);
          /*! send the packet to the mirror watermark*/
        IPA_TF_ASSERT(mirror_dsm_ptr != NULL);
        dsm_enqueue(mirror_wm_ptr, &mirror_dsm_ptr);
        IPA_TF_ASSERT(mirror_dsm_ptr == NULL);
      }
      
      }
#if 0
      /* We need to test power collapse in the driver. So sleep for sometime between
      pkt transmission to trigger power collapse */
      if (ipa_test_engine.curr_test.cfg.is_pc_enabled == TRUE)
      {
        /* Chose a number between 4 seconds to 0 milliseconds */
        pc_sleep_rand = 
          ipa_test_generate_random_num(IPA_TEST_ENGINE_PC_SLEEP_INTERVAL, 0);
        ipa_sleep(pc_sleep_rand);
      }
#endif
      if(ipa_test_engine.curr_test.cfg.ip_pkt_cfg.params.ooo_frag_case == TRUE)
      {
        /* Now Tx the pkt over that bearer/SIO */
        IPA_TF_ASSERT(dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j -1]->dsm_ptr != NULL);
        dsm_enqueue(dst_wm_ptr, &dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts - j)-1]->dsm_ptr);
        IPA_TF_ASSERT(dl_alloc_ptr->pkt_arr[(dl_alloc_ptr->num_pkts) - j - 1]->dsm_ptr == NULL);
      }
      else
      {
        /* Now Tx the pkt over that bearer/SIO */
        IPA_TF_ASSERT(dl_alloc_ptr->pkt_arr[j]->dsm_ptr != NULL);
        dsm_enqueue(dst_wm_ptr, &dl_alloc_ptr->pkt_arr[j]->dsm_ptr);
        IPA_TF_ASSERT(dl_alloc_ptr->pkt_arr[j]->dsm_ptr == NULL);
        
       }

      /*! Store the ip version of the packet that was enqueued in the pkt trace.*/
      pkt.ip_ver = dl_alloc_ptr->pkt_arr[j]->ip_pkt_version;
      /* Increment pkt sent cnt in our curr test status and store the trace  */
       ipa_test_engine_store_tx_pkt_trace(&pkt);

      if (j == ((dl_alloc_ptr->num_pkts / 2) - 1)) 
      {
        if(ipa_test_engine.curr_test.cfg.wan_cfg.is_bearer_dereg_case == TRUE)
        {
           /*! Deregister the bearer */
           IPA_TF_ASSERT(ipa_wani_ds_deregister_bearer(ipa_test_engine.wan.bearer[0].uid) 
                         == IPA_SUCCESS );
           /* Wait for the DS Deregistration event cb*/
           ipa_test_engine_bearer_dereg_evnt_wait_end();
        }
        else if (ipa_test_engine.curr_test.cfg.wan_cfg.is_bearer_susp_case_1 == TRUE)
        {
         /* Suspend the bearer before sending the packets*/
          IPA_ASSERT(ipa_wani_ds_set_bearer_proc_state(ipa_test_engine.wan.bearer[0].uid, 
                                           IPA_WAN_BEARER_PROC_STATE_SUSPEND)== IPA_SUCCESS);
          
          /* Wait for the DS event cb*/
          ipa_test_engine_bearer_susp_evnt_wait_end();
        }
      }
      if(((ipa_test_engine.curr_test.cfg.wan_cfg.is_bearer_susp_case_2 == TRUE) || 
          (ipa_test_engine.curr_test.cfg.wan_cfg.is_bearer_susp_case_1 == TRUE)) &&
          ( j == (dl_alloc_ptr->num_pkts - 1)))
       {
          /* Activate the bearer after sending all the packets for the bearer suspend test cases*/
          IPA_ASSERT(ipa_wani_ds_set_bearer_proc_state(ipa_test_engine.wan.bearer[0].uid,
                                           IPA_WAN_BEARER_PROC_STATE_ACTIVE) == IPA_SUCCESS);
          /* Wait for the DS event cb*/
          ipa_test_engine_bearer_active_evnt_wait_end();
       }
    }
  }
  /* Deregister the bearer suspend cb with the driver*/
  ipa_wani_ds_register_event_cb((ipa_wan_ds_event_cb_type)NULL);

  return ret_code;
}


/*==============================================================================

  FUNCTION:  ipa_test_engine_enqueue_deaggr_err_frames

==============================================================================*/
/*!
  @brief
  Enqueues deaggr error frames in the mirror wmk. These are the frames expected
  from IPA in rx sio wmk for pkt comparison.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_enqueue_deaggr_err_frames
(
  dsm_watermark_type * mirror_wm_ptr,
  int k
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  ipa_test_engine_deaggr_err_info_s * deaggr_err_info =
            &ipa_test_engine.curr_test.deaggr_err_info;

  switch(ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_SPLIT_2_DESC:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_TYPE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_OFFSET:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_LEN:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_LEN:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_SIG:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_SIG:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOT_LT_MIN_FRAME_SIZE:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NXT_NDP_IDX:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_DGRM_IDX:
    {
      IPA_TF_ASSERT(deaggr_err_info->frame_arr[k] != NULL);

      /* Store frame len */
      deaggr_err_info->sent_frame_len_arr[k] =
        dsm_length_packet(deaggr_err_info->frame_arr[k]);

      dsm_enqueue(mirror_wm_ptr, &deaggr_err_info->frame_arr[k]);    
    }
    break;

    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_SEC_EOT_FRAME:
    {
      IPA_TF_ASSERT(
         deaggr_err_info->frame_arr[deaggr_err_info->frame_arr_idx] != NULL);

      /* Store frame len */
      deaggr_err_info->sent_frame_len_arr[deaggr_err_info->frame_arr_idx] =
      dsm_length_packet
        (deaggr_err_info->frame_arr[deaggr_err_info->frame_arr_idx]);
    
      dsm_enqueue(mirror_wm_ptr, 
              &deaggr_err_info->frame_arr[deaggr_err_info->frame_arr_idx]);    
    }      
    break;

    default:
    break;
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ul_tx_pkts

==============================================================================*/
/*!
  @brief
  1. Generate aggregated frame and send it to test per sim
  2. Send pkts to mirror watermark

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_ul_tx_pkts(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 log_ch_idx, phy_ch_idx = 0, bucket_num = 0, num_pkts_consumed = 0;
  uint16 i = 0, j = 0, k = 0;
  uint16 pushdown_len = 0, dpl_pkt_len = 0;
  dsm_watermark_type *dst_wm_ptr = NULL, *mirror_wm_ptr = NULL, *hw_rep_wm_ptr = NULL;
  dsm_item_type *hw_rep_dsm_ptr = NULL;
  ipa_test_engine_pkt_trace_elem_s pkt;
  uint8 tot_tx_paths, tx_path = 0;
  uint8 tx_path_arr[IPA_TEST_ENGINE_LOG_CH_NUM_MAX];
  ipa_test_engine_ip_pkt_ul_alloc_bucket_s *ul_alloc_ptr = NULL;
  uint32 pc_sleep_rand = 0;
  uint32 muxid = 0;
  uint16* cksum_hdr_ptr;
  uint8* pkt_buff;
  uint16 pseudo_hdr_chksum_v4;
  uint16 payload_chksum_v4;
  uint16 checksum_buffer_v4[2];
  uint16 cksum_val_v4;

  /* Memset the pkt trace to 0 */
  IPA_MEMSET(&pkt, 0, sizeof(ipa_test_engine_pkt_trace_elem_s));

  IPA_TF_ASSERT((ipa_test_engine.ip_pkt.curr_num_pkts > 0) &&
                (ipa_test_engine.ip_pkt.curr_num_pkts <= 
                 IPA_TEST_ENGINE_IP_PKT_NUM_MAX));

  /* First process the IP pkts */
  ret_code = ipa_test_engine_ul_process_ip_pkts();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  muxid = (uint32)(ipa_test_engine.ip_pkt.pkt_info_arr[0].mux_id);
  muxid = muxid << 16;

  #if 0
  /*! Apply muxid based filters*/
  IPA_TF_ASSERT((ipa_test_engine.ul_mux_id_handle[0] =
  ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_STATIC_LOCAL,
                                  IPA_IPFLTR_IPV4,
                                  0xFF,
                                  ipa_test_engine.wan.uid_arr[0]))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(ipa_test_engine.ul_mux_id_handle[0],0x81,0x00FF0000,
    muxid) == IPA_IPFLTR_SUCCESS); 

  /*! Apply muxid based filters*/
  IPA_TF_ASSERT((ipa_test_engine.ul_mux_id_handle[1] =
  ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_STATIC_LOCAL,
                                  IPA_IPFLTR_IPV4,
                                  0xEE,
                                  ipa_test_engine.wan.uid_arr[0]))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(ipa_test_engine.ul_mux_id_handle[1],0x81,ipa_ntohl(0x00FF0000),
    ipa_ntohl(muxid)) == IPA_IPFLTR_SUCCESS); 


   /*! Apply muxid based filters*/
  IPA_TF_ASSERT((ipa_test_engine.ul_mux_id_handle[2] =
  ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_STATIC_LOCAL,
                                  IPA_IPFLTR_IPV4,
                                  0xCC,
                                  ipa_test_engine.wan.uid_arr[0]))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(ipa_test_engine.ul_mux_id_handle[2],0x82,ipa_ntohl(0x00FF0000),
    ipa_ntohl(muxid)) == IPA_IPFLTR_SUCCESS); 

 
   /*! Apply muxid based filters*/
  IPA_TF_ASSERT((ipa_test_engine.ul_mux_id_handle[3] =
  ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_STATIC_LOCAL,
                                  IPA_IPFLTR_IPV4,
                                  0xDD,
                                  ipa_test_engine.wan.uid_arr[0]))!= NULL);

  IPA_TF_ASSERT(ipa_ipfltri_add_meq_32(ipa_test_engine.ul_mux_id_handle[3],0x82,0x00FF0000,
    muxid) == IPA_IPFLTR_SUCCESS); 

  /*! Commit first 3 rules out of 7*/
  IPA_TF_ASSERT(ipa_ipfltri_commit_rules(4,ipa_test_engine.ul_mux_id_handle)
                != IPA_ERR_IPFLTR_COMMIT_FAIL);

  ipa_sleep(100);

#endif
  /*! update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_UL_TX);

  if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE)
  {
	tot_tx_paths = ipa_test_engine.endp.curr_phy_ch_cnt - 1;
  }
  else
  {
  tot_tx_paths = ipa_test_engine.endp.curr_phy_ch_cnt;
  }

  /* Calculate the order of pkts to send. Let's not use simple round robin! */
  for (i = 0; i < tot_tx_paths; i++)
  {
    tx_path_arr[i] = 
      ipa_test_generate_unique_random_num(tot_tx_paths - 1, 0, tx_path_arr, i);
  }

  for (i = 0; i < tot_tx_paths; i++)
  {
    tx_path = tx_path_arr[i];
    ul_alloc_ptr = &ipa_test_engine.ip_pkt.ul_alloc_buckets[tx_path];
    j = 0;

    /*! J loop is for the total number of IP pkts in the current config for that 
        per sim channel. It could be more than the num_aggr_pkt cnt where a 
        second frame will be generated.
    */
    while(j < ul_alloc_ptr->num_pkts)
    {
      phy_ch_idx = tx_path;
      dst_wm_ptr = &ipa_test_engine.endp.phy_ch[phy_ch_idx].per_sim.to_ipa_wm;

      /* First generate an endpoint specific frame to send */
      ipa_test_engine.curr_test.aggr[phy_ch_idx].frame_dsm_ptr = NULL;

      ret_code = 
        ipa_test_engine_aggregate_endp_pkt(ipa_test_engine.endp.phy_ch[phy_ch_idx] \
                                           .log_ch_ptr_arr[0]->endp_cfg_ptr,
                                           ipa_test_engine.endp.phy_ch[phy_ch_idx] \
                                           .log_ch_ptr_arr[0]->is_clat,
                                           ul_alloc_ptr->num_pkts - j,
                                           &ul_alloc_ptr->pkt_arr[j],
                                           &num_pkts_consumed,
                                           &ipa_test_engine.curr_test.aggr \
                                           [phy_ch_idx].frame_dsm_ptr);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
      IPA_TF_ASSERT(num_pkts_consumed != 0);

      
      /* Generate error frames from the aggr frames for deaggr err scenario tests. 
         Update total no of pkts that will enqueued and compared. This will change as 
         error frames holding aggregated pkts will be enqueued.
       */
      if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != 
            ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
      {
        ret_code = ipa_test_engine_gen_deaggr_err_frame(
                                           &ipa_test_engine.curr_test.cfg. \
                                           deaggr_err_cfg,
                                           ipa_test_engine.curr_test.aggr \
                                           [phy_ch_idx].frame_dsm_ptr,
                                           &ul_alloc_ptr->pkt_arr[j],
                                           &num_pkts_consumed);
        IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
      }

	  if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE)
	  {
		  	hw_rep_wm_ptr = &ipa_test_engine.endp.phy_ch[1].per_sim.mirror_from_ipa_wm_ul;
      }
      /* Now enqueue all the packets to the mirror watermark */
      /*! K loop is for the num of pkts in current aggregated frame. K ip 
          pkts will be enqueued to mirror wmk  and corresponding aggr frame to 
          per sim wmk to IPA. 
          In some cases of deaggr error scenario tests num_pkts_consumed will be the 
          aggr_frame_cnt that needs to enqueued and not the individual ip pkts.
          So need to update j later with total ip pkts aggregated for above
          while loop to function properly.
      */
      for (k = 0; k < num_pkts_consumed; k++, j++)
      {
        /* For direct DMA we expect the packets in the PHY RX wm */
        if (ipa_test_engine.endp.phy_ch[phy_ch_idx].log_ch_ptr_arr[0] \
            ->endp_cfg_ptr->ul_config.hdr_type == IPA_SIO_HDR_TYPE_DIRECT_DMA)
        {
          mirror_wm_ptr = &ipa_test_engine.endp.phy_ch[phy_ch_idx].mirror_phy_rx_wm;

          /****! Store the pkt trace ***/
          pkt.src = IPA_TEST_ENGINE_PKT_SRC_SIM;
          pkt.dst = IPA_TEST_ENGINE_PKT_DST_PHY_CHANNEL;
          pkt.src_idx = phy_ch_idx;
          pkt.dst_idx = phy_ch_idx;
        }
        else
        {
          mirror_wm_ptr = &ul_alloc_ptr->log_ch_ptr[j]->mirror_rx_wm;
          log_ch_idx =  ul_alloc_ptr->log_ch_ptr[j]->idx;

          /****! Store the pkt trace ***/
          pkt.src = IPA_TEST_ENGINE_PKT_SRC_SIM;
          pkt.dst = IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL;
          pkt.src_idx = phy_ch_idx;
          pkt.dst_idx = log_ch_idx;
        }

        
        /* Now enqueue the pkt on the corresponding SIO mirror watermark before sending 
        it out. */
        IPA_IPFLTR_SET_UL_FLTR_RESULT(ul_alloc_ptr->pkt_arr[j]->dsm_ptr, 
                                   ul_alloc_ptr->pkt_arr[j]->filter_result);
        /* Set the priority of the payload */
        if (ul_alloc_ptr->pkt_arr[j]->is_prioritized == TRUE)
        {
          ul_alloc_ptr->pkt_arr[j]->dsm_ptr->
						pkt_bit_field.dsm_bit_field.q_priority_bit = DSM_HIGHEST;
        }

        pkt.filter_result = ul_alloc_ptr->pkt_arr[j]->filter_result;
        pkt.filter_priority = dsm_item_get_priority(ul_alloc_ptr->pkt_arr[j]->dsm_ptr);
        pkt.pkt_len = ul_alloc_ptr->pkt_arr[j]->pkt_len;
        /* Store if xlat is conversion expected on the packet */
        pkt.xlat = FALSE;

        /* Enqueue frames from deaggr err info if enabled */
        if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != 
             ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
        {
          ret_code = ipa_test_engine_enqueue_deaggr_err_frames
                       (mirror_wm_ptr,k);
          IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
        }
        /*! If it is a CLAT case we expect IPV6 packets back. Hence enqueue 
            IPV6 packet in the mirror watermark. */
        else if((ipa_test_engine.curr_test.cfg.is_clat_enabled == TRUE) &&
                (ul_alloc_ptr->log_ch_ptr[j]->is_clat == TRUE) &&
                (IPA_TEST_GET_IP_VERSION(ul_alloc_ptr->pkt_arr[j]->pkt_buf_ptr) == 4) &&
                ((ipa_test_engine.curr_test.cfg.fltr_cfg.fltr_rule_cat_type == IPA_TEST_ENGINE_FLTR_RULE_MAX)||
                 (ipa_test_engine.curr_test.cfg.fltr_cfg.fltr_rule_type == IPA_TEST_ENGINE_FLTR_TYPE_CLAT_FRAG_V4)))
        {
          ret_code = ipa_test_engine_enqueue_ipv6_clat_frame(
                        mirror_wm_ptr,
                        ul_alloc_ptr->pkt_arr[j]->is_qmap_v3,
                        ul_alloc_ptr->pkt_arr[j]->checksum_val,
                        ul_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                        ul_alloc_ptr->pkt_arr[j]->pkt_len,
                        ul_alloc_ptr->log_ch_ptr[j]->clat_cntxt_ptr);
          IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
          dsm_free_packet(&ul_alloc_ptr->pkt_arr[j]->dsm_ptr);

		  pkt_buff = ul_alloc_ptr->pkt_arr[j]->pkt_buf_ptr;

		  /*IPA_MEMSET(&v4_pseudo_hdr, 0, sizeof(v4_pseudo_hdr)); 

		  v4_pseudo_hdr.src_addr = *(uint32*)(&pkt_buff[IPA_TEST_IPV4_SRC_ADDR_OFST]);
  	      v4_pseudo_hdr.dest_addr = *(uint32*)(&pkt_buff[IPA_TEST_IPV4_DEST_ADDR_OFST]);
	      v4_pseudo_hdr.protocol = IPA_TEST_TCP_PROT;
  	      v4_pseudo_hdr.tcp_length = ul_alloc_ptr->pkt_arr[j]->pkt_len - IPA_TEST_IPV4_HDR_LEN;

		  pseudo_hdr_chksum_v4 = ipa_test_calculate_cksum((uint16*) &v4_pseudo_hdr, sizeof(v4_pseudo_hdr));
		  pseudo_hdr_chksum_v4 = ~pseudo_hdr_chksum_v4 & 0xFFFF;*/

		  /*pseudo_hdr_chksum_v4 = ipa_test_calculate_pseudo_hdr_cksum(ul_alloc_ptr->pkt_arr[j]->pkt_buf_ptr);

          payload_chksum_v4 = 
        	ipa_test_calculate_cksum((uint16*)&pkt_buff[IPA_TEST_IPV4_HDR_LEN],
                                 ul_alloc_ptr->pkt_arr[j]->pkt_len - IPA_TEST_IPV4_HDR_LEN);
		  payload_chksum_v4 = ~payload_chksum_v4 & 0xFFFF;

		  checksum_buffer_v4[0] = pseudo_hdr_chksum_v4;
	      checksum_buffer_v4[1] = payload_chksum_v4;
	      cksum_val_v4 = ipa_test_calculate_cksum((uint16 *)&checksum_buffer_v4, sizeof(checksum_buffer_v4));*/

          //cksum_hdr_ptr = (uint16*)(&pkt_buff[IPA_TEST_IPV4_HDR_LEN + IPA_TEST_TCP_CKSUM_HDR_OFST]);
	      //*cksum_hdr_ptr = cksum_val_v4;

		  /*pushdown_len =
         		dsm_insert(&ipa_test_engine.curr_test.aggr[phy_ch_idx].frame_dsm_ptr, 
                    IPA_CFG_GEN_DSM_ITEM_POOL,
                    IPA_TEST_IPV4_HDR_LEN + IPA_TEST_TCP_CKSUM_HDR_OFST, (uint8 *)&cksum_val_v4,
                    sizeof(cksum_val_v4));
	  
          IPA_TF_ASSERT(pushdown_len == sizeof(cksum_val_v4));*/
          /* Store if xlat is conversion expected on the packet */
          pkt.xlat = TRUE;
        }
        else
        {
          /* Now send the pkt to the SIO Rx Mirror watermark */
          IPA_TF_ASSERT(ul_alloc_ptr->pkt_arr[j]->dsm_ptr != NULL);

		   if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE)
           {
				dpl_pkt_len = (uint16)IPA_MIN(256, ul_alloc_ptr->pkt_arr[j]->pkt_len);
				pushdown_len = 
        					dsm_pushdown(&hw_rep_dsm_ptr,
                     				ul_alloc_ptr->pkt_arr[j]->pkt_buf_ptr,
                     				dpl_pkt_len,
                     				IPA_CFG_SL_DSM_ITEM_POOL);
				IPA_TF_ASSERT(pushdown_len == dpl_pkt_len);

				dsm_enqueue(hw_rep_wm_ptr, &hw_rep_dsm_ptr);
        	}
		   
          dsm_enqueue(mirror_wm_ptr, &ul_alloc_ptr->pkt_arr[j]->dsm_ptr);
          IPA_TF_ASSERT(ul_alloc_ptr->pkt_arr[j]->dsm_ptr == NULL);
        }

        /*! Store the ip version of the packet that was enqueued in the pkt trace.*/
        pkt.ip_ver = ul_alloc_ptr->pkt_arr[j]->ip_pkt_version;
        /* Increment pkt sent cnt in our curr test status and store the trace  */
        ipa_test_engine_store_tx_pkt_trace(&pkt);
      } /*K loop*/

      /* Now store aggregation histogram */
      bucket_num = 
        IPA_STATS_AGGR_HIST_GET_BUCKET_NUM(num_pkts_consumed);
      ipa_test_engine.curr_test.aggr[phy_ch_idx].aggr_hist[bucket_num].num_frames++;
#if 0
      /* We need to test power collapse in the driver. So sleep for sometime between
      pkt transmission to trigger power collapse */
      if (ipa_test_engine.curr_test.cfg.is_pc_enabled == TRUE)
      {
        /* Chose a number between 4 seconds to 0 milliseconds */
        pc_sleep_rand = 
          ipa_test_generate_random_num(IPA_TEST_ENGINE_PC_SLEEP_INTERVAL, 0);
        ipa_sleep(100);
      }
#endif
      /* Now send out the aggregate frame out to the SIO tx */
      IPA_TF_ASSERT(ipa_test_engine.curr_test.aggr[phy_ch_idx].frame_dsm_ptr != NULL);
      dsm_enqueue(dst_wm_ptr, &ipa_test_engine.curr_test.aggr[phy_ch_idx].frame_dsm_ptr);
      IPA_TF_ASSERT(ipa_test_engine.curr_test.aggr[phy_ch_idx].frame_dsm_ptr == NULL);

      /* In case of deaggr error scenario tests, all pkts in the buckets 
         could be consumed. num_pkts_consumed = total_no_frames to enqueue that could
         hold all pkts in ul_alloc_ptr.So break if done.
      */
      switch (ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
      {
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_EOT_LT_MIN_FRAME_SIZE:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_SPLIT_2_DESC:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_TYPE:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_OFFSET:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_LEN:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_LEN:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_SIG:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_SIG:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOT_LT_MIN_FRAME_SIZE:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NXT_NDP_IDX:
        case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_DGRM_IDX:
        {
          /* Need to update j with total num of pkts that are already aggregated. */
          j = ipa_test_engine.curr_test.deaggr_err_info.pkt_cnt;
          if(ipa_test_engine.curr_test.deaggr_err_info.pkt_cnt == 
             ipa_test_engine.ip_pkt.curr_num_pkts)
          {
            break;
          }
        }
        break;
        default:
        break;
      }
    } /* while j loop */
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_tx_pkts

==============================================================================*/
/*!
  @brief
  For DL:
  1. Generate aggregated frame and send it to mirror watermark
  2. Send pkts over a random bearer or SIO port
  for UL:
  1. Generate aggregated frame and send it to test per sim
  2. Send pkts to mirror watermark

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_tx_pkts(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  switch (ipa_test_engine.curr_test.cfg.test_type)
  {
    case IPA_TEST_ENGINE_TEST_TYPE_DL:
    case IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK:
	  case IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB:
      ret_code = ipa_test_engine_dl_tx_pkts();
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
      break;

    case IPA_TEST_ENGINE_TEST_TYPE_UL:
    case IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK:
	  case IPA_TEST_ENGINE_TEST_TYPE_UL_HOLB:
      ret_code = ipa_test_engine_ul_tx_pkts();
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
      break;

    default:
      IPA_TF_ASSERT(0);
  }

  /* Now we can free up the memory that we malloced because it has been copied 
  to DSM items */
  ret_code = ipa_test_engine_destroy_ip_pkts();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  //ipa_test_engine.ip_pkt.curr_num_pkts = 0;
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_request

==============================================================================*/
/*!
  @brief
  Enqueue the ZIP requests according to the ZIP Configurations.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_zip_request(void)
{
#if 0
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint32 req_idx = 0;

  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_ZIP_REQUEST);

  /* Initialize the destination buffers */
  ipa_test_engine.zip.hi_prio_req_dest_buf[0] = (uint8 *)&hi_prio_dest_buffer_0;
  ipa_test_engine.zip.hi_prio_req_dest_buf[1] = (uint8 *)&hi_prio_dest_buffer_1;
  ipa_test_engine.zip.hi_prio_req_dest_buf[2] = (uint8 *)&hi_prio_dest_buffer_2;
  ipa_test_engine.zip.hi_prio_req_dest_buf[3] = (uint8 *)&hi_prio_dest_buffer_3;

  ipa_test_engine.zip.def_prio_req_dest_buf[0] = (uint8 *)&def_prio_dest_buffer_0;
  ipa_test_engine.zip.def_prio_req_dest_buf[1] = (uint8 *)&def_prio_dest_buffer_1;
  ipa_test_engine.zip.def_prio_req_dest_buf[2] = (uint8 *)&def_prio_dest_buffer_2;
  ipa_test_engine.zip.def_prio_req_dest_buf[3] = (uint8 *)&def_prio_dest_buffer_3;

  /* Enqueue all the High priority ZIP requests */
  for (req_idx = 0; req_idx < ipa_test_engine.curr_test.cfg.zip_cfg.num_hi_prio_reqs; req_idx++)
  {
    /* Set the destination buffer info and tag info in the IPA ZIP request */
    ipa_test_engine.curr_test.cfg.zip_cfg.hp_req_arr[req_idx].dst_buf = 
       ipa_test_engine.zip.hi_prio_req_dest_buf[req_idx];
    ipa_test_engine.curr_test.cfg.zip_cfg.hp_req_arr[req_idx].cb_tag = req_idx + 200;
    /* Enqueue a High priority request */
    IPA_ASSERT(ipa_zip_cmd_req_hi_prio
              (&ipa_test_engine.curr_test.cfg.zip_cfg.hp_req_arr[req_idx]) == IPA_ZIP_SUCCESS);
    ipa_test_engine.zip.num_req ++;
  }

  /* Enqueue all the Low priority ZIP requests */
  for (req_idx = 0; req_idx < ipa_test_engine.curr_test.cfg.zip_cfg.num_def_prio_reqs; req_idx++)
  {
    /* Set the destination buffer info and tag info in the IPA ZIP request */
    ipa_test_engine.curr_test.cfg.zip_cfg.hp_req_arr[req_idx].dst_buf = 
       ipa_test_engine.zip.def_prio_req_dest_buf[req_idx];
    /* Set the destination buffer info and tag info in the IPA ZIP request */
    ipa_test_engine.curr_test.cfg.zip_cfg.dp_req_arr[req_idx].cb_tag = req_idx + 100;
    /* Enqueue a Default priority request */
    IPA_ASSERT(ipa_zip_cmd_req_hi_prio
              (&ipa_test_engine.curr_test.cfg.zip_cfg.dp_req_arr[req_idx]) == IPA_ZIP_SUCCESS);
    ipa_test_engine.zip.num_req ++;
  }
  
  return ret_code;
#endif
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_response

==============================================================================*/
/*!
  @brief
  Wait for the ZIP responses.
  Compare the response with the expected result.
 
  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_zip_response(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint32 compare_idx = 0;

  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_ZIP_RESPONSE);

  if (ipa_test_engine.zip.num_req == 0)
  {
    return ret_code;
  }

  if (ipa_test_engine.curr_test.cfg.zip_cfg.err_type == IPA_TEST_ENGINE_ZIP_ERR_MAX)
  {
    while (ipa_test_engine.zip.num_resp < ipa_test_engine.zip.num_req)
    {
      /* Wait for ZIP responses. */
      ipa_sleep(100);
      
      if (ipa_test_engine.zip.num_success_resp < ipa_test_engine.zip.num_req)
      {
        if (ipa_test_engine.zip.error_code[ipa_test_engine.zip.num_success_resp] != 0)
        {
          IPA_ASSERT(ipa_test_engine.zip.cb_tag_rcvd[ipa_test_engine.zip.num_success_resp] 
		  	          == ipa_test_engine.curr_test.cfg.zip_cfg.hp_req_arr
		  	   			  [ipa_test_engine.zip.num_success_resp].cb_tag);

		  IPA_ASSERT(ipa_test_engine.zip.error_code[ipa_test_engine.zip.num_success_resp] 
		  	          == ipa_test_engine.curr_test.cfg.zip_cfg.\
		  	          	 error_code[ipa_test_engine.zip.num_success_resp]);

		  IPA_ASSERT(ipa_test_engine.zip.bytes_left[ipa_test_engine.zip.num_success_resp] 
		  	          == ipa_test_engine.curr_test.cfg.zip_cfg.\
		  	          	 bytes_left[ipa_test_engine.zip.num_success_resp]);
		}
		else
		{
          if (ipa_test_engine.zip.cb_tag_rcvd[ipa_test_engine.zip.num_success_resp] >= 200)
          {
            /* Get the index of the request enqueued on the High priority queue from the cb tag received.*/
          	compare_idx = ipa_test_engine.zip.cb_tag_rcvd[ipa_test_engine.zip.num_success_resp] - 200;
       		/* Compare the received destination length with the expected destination length */
          	IPA_ASSERT(ipa_test_engine.zip.\
                       dest_buf_len_rcvd[ipa_test_engine.zip.num_success_resp]
                       == ipa_test_engine.curr_test.cfg.\
                       zip_cfg.hp_req_arr[compare_idx].dst_len);
          	/* Compare the received destination buffer with the
           	* expected destination buffer in the mirror response. */
          	IPA_ASSERT(IPA_MEMCMP(ipa_test_engine.zip.hi_prio_req_dest_buf[compare_idx], 
                                ipa_test_engine.curr_test.cfg.zip_cfg.\
                                mirror_hpq_resp_dest_buf[compare_idx], 
                                sizeof(ipa_test_engine.curr_test.cfg.\
                                       zip_cfg.mirror_hpq_resp_dest_buf[compare_idx])) == 0);
        	} 
         	else
         	{
          	  /* Get the index of the request enqueued on the
           	  * default priority queue from the cb tag received.*/
           	  compare_idx = (ipa_test_engine.zip.cb_tag_rcvd[ipa_test_engine.zip.num_success_resp] - 100);
           	  /* Compare the received destination length with the expected destination length */
           	  IPA_ASSERT(ipa_test_engine.zip.dest_buf_len_rcvd[ipa_test_engine.zip.num_success_resp]
                       == ipa_test_engine.curr_test.cfg.zip_cfg.dp_req_arr[compare_idx].dst_len);
          	  /* Compare the received destination buffer with the 
           	  * expected destination buffer in the mirror response. */
          	  IPA_ASSERT(IPA_MEMCMP(ipa_test_engine.zip.\
                                  def_prio_req_dest_buf[compare_idx], 
                                  ipa_test_engine.curr_test.cfg.\
                                  zip_cfg.mirror_dpq_resp_dest_buf[compare_idx], 
                                  ipa_test_engine.curr_test.cfg.zip_cfg.\
                                  dp_req_arr[compare_idx].dst_len) == 0);
         	 }
			}

         /* Increment the number of responses successful */
         ipa_test_engine.zip.num_success_resp++;
      }
    }
  }
  else
  {
    /* Wait infinitely for the assert to be hit in the error callback */
    while(1);
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_zip_destroy

==============================================================================*/
/*!
  @brief
  Enqueue the ZIP requests according to the ZIP Configurations.

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_zip_destroy(void)
{ 
  uint32 req_idx = 0;
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_ZIP_DESTROY);

  /* Cleaning up the IPA ZIP request and response stats for the current test. */
  ipa_test_engine.zip.num_req = 0;
  ipa_test_engine.zip.num_resp = 0;
  ipa_test_engine.zip.num_success_resp = 0;

  /* Clean up the response arrays for the current test */
  IPA_MEMSET(&ipa_test_engine.zip.hi_prio_req_dest_buf, 0, sizeof(ipa_test_engine.zip.hi_prio_req_dest_buf));
  IPA_MEMSET(&ipa_test_engine.zip.def_prio_req_dest_buf, 0 , sizeof(ipa_test_engine.zip.def_prio_req_dest_buf));
  IPA_MEMSET(ipa_test_engine.zip.cb_tag_rcvd, 0, sizeof(ipa_test_engine.zip.cb_tag_rcvd));
  IPA_MEMSET(ipa_test_engine.zip.dest_buf_len_rcvd, 0, sizeof(ipa_test_engine.zip.dest_buf_len_rcvd));
  
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_deaggregate_endp_pkt

==============================================================================*/
/*!
  @brief
  1. Deaggregate an aggregated frame based on the given endp config 

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_deaggregate_endp_pkt
(
   ipa_sio_endpoint_config_s     *cfg_ptr,
   uint16                        num_pkts,
   ipa_test_engine_ip_pkt_info_s *pkt_arr_ptr,
   uint8                         *num_pkts_used,
   dsm_item_type                 **frame_dsm_ptr
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_FAILURE;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_DL_DEAGGR);

  switch (cfg_ptr->dl_config.hdr_type)
  {
    case IPA_SIO_HDR_TYPE_NONE:
      ret_code =
        ipa_test_deaggregate_none_endp_pkt(pkt_arr_ptr, num_pkts,
                                           &cfg_ptr->dl_config, 
                                           frame_dsm_ptr, num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_ETHERNET:
      ret_code =
        ipa_test_deaggregate_ethernet_endp_pkt(pkt_arr_ptr, num_pkts,
                                               &cfg_ptr->dl_config, 
                                               frame_dsm_ptr, num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_QMAP:
      ret_code =
        ipa_test_deaggregate_qmap_endp_pkt(pkt_arr_ptr, num_pkts,
                                           &cfg_ptr->dl_config, 
                                           frame_dsm_ptr, num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_QMAP_V3:
      ret_code =
        ipa_test_deaggregate_qmap_v3_endp_pkt(pkt_arr_ptr, num_pkts,
                                              &cfg_ptr->dl_config, 
                                              frame_dsm_ptr, num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_QCNCM:
      break;

    case IPA_SIO_HDR_TYPE_MBIM:
      ret_code =
        ipa_test_deaggregate_mbim_endp_pkt(pkt_arr_ptr, num_pkts,
                                           &cfg_ptr->dl_config, 
                                           frame_dsm_ptr, num_pkts_used);
      break;

    case IPA_SIO_HDR_TYPE_DIRECT_DMA:
      break;

    default:
      IPA_TF_ASSERT(0);
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_dl_sio_tx_pkts

==============================================================================*/
/*!
  @brief
  The packets that needs to be sent back to the driver for unbridged bearers

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_dl_sio_tx_pkts
(
  uint8 bearer_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  dsm_watermark_type *rx_mirror_wm_ptr = NULL, *dst_mirror_wm_ptr = NULL;
  dsm_watermark_type *rx_wm_ptr = NULL, *dst_wm_ptr = NULL;

  rx_wm_ptr =
    &ipa_test_engine.wan.bearer[bearer_idx].dl_wm;
  dst_wm_ptr =
    &ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr->tx_wm;

  rx_mirror_wm_ptr = 
    &ipa_test_engine.wan.bearer[bearer_idx].mirror_dl_wm;
  dst_mirror_wm_ptr =
    &ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr-> \
    phy_ch_ptr->per_sim.mirror_from_ipa_wm;

  while((ipa_test_engine.pkt_cmp.rcvd_pkt_dsm_ptr = 
         dsm_dequeue(rx_wm_ptr)) != NULL)
  {
    /* Let's not do any comparison, but take it out of dl mirror and put it in 
    sim mirror */
    ipa_test_engine.pkt_cmp.sent_pkt_dsm_ptr = dsm_dequeue(rx_mirror_wm_ptr);
    IPA_TF_ASSERT(ipa_test_engine.pkt_cmp.sent_pkt_dsm_ptr != NULL);

    ipa_test_engine.pkt_cmp.rcvd_pkt_len = 
      dsm_length_packet(ipa_test_engine.pkt_cmp.rcvd_pkt_dsm_ptr);

    ipa_test_engine.pkt_cmp.sent_pkt_len = 
      dsm_length_packet(ipa_test_engine.pkt_cmp.sent_pkt_dsm_ptr);

    /* Check lengths for sanity */
    IPA_TF_ASSERT(ipa_test_engine.pkt_cmp.rcvd_pkt_len == 
                  ipa_test_engine.pkt_cmp.sent_pkt_len);

    dsm_enqueue(dst_mirror_wm_ptr, &ipa_test_engine.pkt_cmp.sent_pkt_dsm_ptr);
    IPA_TF_ASSERT(ipa_test_engine.pkt_cmp.sent_pkt_dsm_ptr == NULL);

    /* For ethernet packets we need to attach ethernet hdr vefore sending it
    to DL */
    if (ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr->endp_cfg_ptr \
        ->dl_config.hdr_type == IPA_SIO_HDR_TYPE_ETHERNET)
    {
      if (IPA_TEST_GET_IP_VERSION(ipa_test_engine.pkt_cmp.rcvd_pkt_dsm_ptr->data_ptr)
          == 4)
      {
        dsm_pushdown(&ipa_test_engine.pkt_cmp.rcvd_pkt_dsm_ptr, 
                     ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr->endp_cfg_ptr \
                     ->dl_config.params.ether.ipv4_hdr,
                     IPA_SIO_ETHERNET_HDR_LEN,
                     IPA_CFG_SL_DSM_ITEM_POOL);
      }
      else
      {
        dsm_pushdown(&ipa_test_engine.pkt_cmp.rcvd_pkt_dsm_ptr, 
                     ipa_test_engine.wan.bearer[bearer_idx].log_ch_ptr->endp_cfg_ptr \
                     ->dl_config.params.ether.ipv6_hdr,
                     IPA_SIO_ETHERNET_HDR_LEN,
                     IPA_CFG_SL_DSM_ITEM_POOL);
      }
    }

    /* Enqueue it back into the SIO watermark */
    dsm_enqueue(dst_wm_ptr, &ipa_test_engine.pkt_cmp.rcvd_pkt_dsm_ptr);
    IPA_TF_ASSERT(ipa_test_engine.pkt_cmp.rcvd_pkt_dsm_ptr == NULL);
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_dl_rx_pkts

==============================================================================*/
/*!
  @brief
  1. This is the filtered packet coming back. This needs to be put back to the
  driver via SIO
  2. Packets reaching back for unbridged bearers

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_dl_rx_pkts
(
  uint8 bearer_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_DL_RX);

  IPA_TF_ASSERT(bearer_idx < ipa_test_engine.wan.curr_bearer_cnt);

#ifdef FEATURE_IPA_TEST_ENGINE_IPA_UNBRIDGED_SIO_TX
  /* If unbridged, then send packet back on SIO, 
  else compare filter match and free packet */
  if (ipa_test_engine.wan.bearer[bearer_idx].is_bridged == FALSE)
  {
    ret_code = ipa_test_engine_dl_sio_tx_pkts(bearer_idx);
  }
  else
  {
    ret_code = 
      ipa_test_engine_compare_rx_wm_n_mirror_wm(&ipa_test_engine.wan.bearer \
                                                [bearer_idx].dl_wm,
                                                &ipa_test_engine.wan.bearer \
                                                [bearer_idx].mirror_dl_wm,
                                                &ipa_test_engine.pkt_cmp,
                                                IPA_TEST_ENGINE_PKT_DST_BEARER_DL,
                                                bearer_idx);
  }
#else
  ret_code = 
    ipa_test_engine_compare_rx_wm_n_mirror_wm(&ipa_test_engine.wan.bearer \
                                              [bearer_idx].dl_wm,
                                              &ipa_test_engine.wan.bearer \
                                              [bearer_idx].mirror_dl_wm,
                                              &ipa_test_engine.pkt_cmp,
                                              IPA_TEST_ENGINE_PKT_DST_BEARER_DL,
                                              bearer_idx);
#endif

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ul_rx_loopback_pkts

==============================================================================*/
/*!
  @brief
  1. Receive pkts from IPA driver and loop it back on DL watermark

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_ul_rx_loopback_pkts
(
  dsm_watermark_type        *rx_wm_ptr, 
  dsm_watermark_type        *mirror_wm_ptr, 
  ipa_test_engine_pkt_cmp_s *pkt_cmp_ptr,
  ipa_test_engine_pkt_dst_e dst, 
  uint8                     ch_idx 
)
{
  uint8 flag = 0, i = 0;
  uint32 pullup_len = 0;
  IPA_MEMSET(pkt_cmp_ptr, 0, sizeof(ipa_test_engine_pkt_cmp_s));
  ipa_test_ret_code_e ret_code;
  ipa_test_engine_pkt_trace_elem_s pkt;

  /* Memset the pkt trace to 0 */
  IPA_MEMSET(&pkt, 0 , sizeof(ipa_test_engine_pkt_trace_elem_s));

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_UL_LOOPBACK);

  /* Ensure the cache is clean */
  IPA_TF_ASSERT(ipa_test_engine.ip_pkt.curr_num_pkts == 0);

  pkt.dst = dst;
  pkt.dst_idx = ch_idx;
  pkt.src_idx = 0;
  pkt.src = IPA_TEST_ENGINE_PKT_SRC_UNKNOWN;

  while((pkt_cmp_ptr->rcvd_pkt_dsm_ptr = dsm_dequeue(rx_wm_ptr)) != NULL)
  {
    pkt_cmp_ptr->sent_pkt_dsm_ptr = dsm_dequeue(mirror_wm_ptr);
    IPA_TF_ASSERT(pkt_cmp_ptr->sent_pkt_dsm_ptr != NULL);

    /* Let's only compare lengths & filter results for sanity sake */
    pkt_cmp_ptr->sent_pkt_len = 
      dsm_length_packet(pkt_cmp_ptr->sent_pkt_dsm_ptr);
    pkt_cmp_ptr->rcvd_pkt_len = 
      dsm_length_packet(pkt_cmp_ptr->rcvd_pkt_dsm_ptr);

    if (pkt_cmp_ptr->rcvd_pkt_len != pkt_cmp_ptr->sent_pkt_len)
    {
      flag = 1;
      ipa_test_engine.pkt_cmp.compare_failure_id = 30;
      break;
    }

    /* Now check the filter result */
    if (IPA_IPFLTR_GET_UL_FLTR_RESULT(pkt_cmp_ptr->sent_pkt_dsm_ptr) != 
        IPA_IPFLTR_GET_UL_FLTR_RESULT(pkt_cmp_ptr->rcvd_pkt_dsm_ptr))
    {
      flag = 1;
      ipa_test_engine.pkt_cmp.compare_failure_id = 31;
      break;
    }

    /* Free the mirror dsm item */
    dsm_free_packet(&pkt_cmp_ptr->sent_pkt_dsm_ptr);
    IPA_TF_ASSERT(pkt_cmp_ptr->sent_pkt_dsm_ptr == NULL);

    /* We'll get pkts in PHY RX for Direct DMA mode only */
    if (dst == IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL)
    {
      /* We need to remove ethernet or QOS headers before looping it back */
      if (ipa_test_engine.endp.log_ch[ch_idx].endp_cfg_ptr->ul_config.hdr_type ==
          IPA_SIO_HDR_TYPE_ETHERNET)
      {
        pullup_len = 
          dsm_pullup(&(pkt_cmp_ptr->rcvd_pkt_dsm_ptr),
                     NULL, IPA_SIO_ETHERNET_HDR_LEN);
        IPA_TF_ASSERT(pullup_len == IPA_SIO_ETHERNET_HDR_LEN);
        pkt_cmp_ptr->rcvd_pkt_len -= IPA_SIO_ETHERNET_HDR_LEN;
      }
      if (ipa_test_engine.endp.log_ch[ch_idx].endp_cfg_ptr->ul_config.is_qos_present ==
          TRUE)
      {
        pullup_len = 
          dsm_pullup(&(pkt_cmp_ptr->rcvd_pkt_dsm_ptr),
                     NULL, IPA_TEST_QOS_HDR_LEN);
        IPA_TF_ASSERT(pullup_len == IPA_TEST_QOS_HDR_LEN);
        pkt_cmp_ptr->rcvd_pkt_len -= IPA_TEST_QOS_HDR_LEN;
      }
    }
    
    pkt.filter_result = 0;
    pkt.filter_priority = dsm_item_get_priority(pkt_cmp_ptr->rcvd_pkt_dsm_ptr);

    i = ipa_test_engine.ip_pkt.curr_num_pkts;

    /* Now pullup the data into our cache */
    ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_len = pkt_cmp_ptr->rcvd_pkt_len;
    ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr = 
      (uint8 *)IPA_MALLOC(pkt_cmp_ptr->rcvd_pkt_len);

    pullup_len = 
      dsm_pullup(&(pkt_cmp_ptr->rcvd_pkt_dsm_ptr),
                 ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr,
                 pkt_cmp_ptr->rcvd_pkt_len);
    IPA_TF_ASSERT(pullup_len == pkt_cmp_ptr->rcvd_pkt_len);

    ipa_test_engine.ip_pkt.curr_num_pkts++;

    pkt.pkt_len = pkt_cmp_ptr->rcvd_pkt_len;

    if(IPA_TEST_GET_IP_VERSION(ipa_test_engine.ip_pkt.pkt_info_arr[i].pkt_buf_ptr) == 4)
    {
      pkt.ip_ver = IPA_IP_TYPE_IPV4;
    }
    else
    {
      pkt.ip_ver = IPA_IP_TYPE_IPV6;
    }
    ipa_test_engine_store_rx_pkt_trace(&pkt);
  }

  /* Now loop the packet back to DL watermark */
  if (flag == 0)
  {
    if (ipa_test_engine.ip_pkt.curr_num_pkts > 0)
    {
      ret_code = ipa_test_engine_dl_tx_pkts();
      if (ret_code == IPA_TEST_RET_CODE_SUCCESS)
      {
        /* Now we can free up the memory that we malloced because it has been copied 
        to DSM items */
        ret_code = ipa_test_engine_destroy_ip_pkts();
      }
      return ret_code;
    }
    else
    {
      return IPA_TEST_RET_CODE_SUCCESS;
    }
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ul_rx_pkts

==============================================================================*/
/*!
  @brief
  1. Receive pkts from IPA driver and compare with watermark

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_ul_rx_pkts
(
  uint8 log_ch_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_UL_RX);

  IPA_TF_ASSERT(log_ch_idx < ipa_test_engine.endp.curr_log_ch_cnt);

  if (ipa_test_engine.curr_test.cfg.test_type == 
      IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK)
  {
    /* For loopback test, send the packet back on the DL
    ipa_test_engine_dl_tx_pkts();*/
    ret_code = 
      ipa_test_engine_ul_rx_loopback_pkts(&ipa_test_engine.endp.log_ch \
                                          [log_ch_idx].rx_wm,
                                          &ipa_test_engine.endp.log_ch \
                                          [log_ch_idx].mirror_rx_wm,
                                          &ipa_test_engine.pkt_cmp,
                                          IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL,
                                          log_ch_idx);
  }
  else
  {
    ret_code = 
      ipa_test_engine_compare_rx_wm_n_mirror_wm(&ipa_test_engine.endp.log_ch \
                                                [log_ch_idx].rx_wm,
                                                &ipa_test_engine.endp.log_ch \
                                                [log_ch_idx].mirror_rx_wm,
                                                &ipa_test_engine.pkt_cmp,
                                                IPA_TEST_ENGINE_PKT_DST_LOG_CHANNEL,
                                                log_ch_idx);
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ul_phy_rx_pkts

==============================================================================*/
/*!
  @brief
  1. Receive pkts from IPA driver on PHY RX and compare with watermark

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_ul_phy_rx_pkts
(
  uint8 phy_ch_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_UL_PHY_RX);

  IPA_TF_ASSERT(phy_ch_idx < ipa_test_engine.endp.curr_phy_ch_cnt);

  if (ipa_test_engine.curr_test.cfg.test_type == 
      IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK)
  {
    /* For loopback test, send the packet back on the DL
    ipa_test_engine_dl_tx_pkts();*/
    ret_code = 
      ipa_test_engine_ul_rx_loopback_pkts(&ipa_test_engine.endp.phy_ch \
                                          [phy_ch_idx].phy_rx_wm,
                                          &ipa_test_engine.endp.phy_ch \
                                          [phy_ch_idx].mirror_phy_rx_wm,
                                          &ipa_test_engine.pkt_cmp,
                                          IPA_TEST_ENGINE_PKT_DST_PHY_CHANNEL,
                                          phy_ch_idx);
  }
  else
  {
    ret_code = 
      ipa_test_engine_compare_rx_wm_n_mirror_wm(&ipa_test_engine.endp.phy_ch \
                                                [phy_ch_idx].phy_rx_wm,
                                                &ipa_test_engine.endp.phy_ch \
                                                [phy_ch_idx].mirror_phy_rx_wm,
                                                &ipa_test_engine.pkt_cmp,
                                                IPA_TEST_ENGINE_PKT_DST_PHY_CHANNEL,
                                                phy_ch_idx);
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_dpl_rx_pkts

==============================================================================*/
/*!
  @brief
  DPL packet processing

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_dpl_rx_pkts
(
  uint8 log_ch_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  

  ret_code = 
      ipa_test_engine_compare_rx_wm_n_mirror_wm(&ipa_test_engine.dpl.dpl_wm,
                                                &ipa_test_engine.dpl.mirror_dpl_wm,
                                                &ipa_test_engine.pkt_cmp,
                                                IPA_TEST_ENGINE_PKT_DST_DPL,
                                                0);

  

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_sim_rx_loopback_pkts

==============================================================================*/
/*!
  @brief
  Loops back the pkts in our cache to UL over SIM consumer

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_sim_rx_loopback_pkts
(
  dsm_watermark_type            *mirror_wm_ptr, 
  ipa_test_engine_pkt_cmp_s     *pkt_cmp_ptr,
  ipa_test_engine_ip_pkt_info_s *pkt_info_arr,
  uint16                        num_pkts_rx,
  uint8                         pkt_dst_idx 
)
{
  uint8 flag = 0;
  IPA_MEMSET(pkt_cmp_ptr, 0, sizeof(ipa_test_engine_pkt_cmp_s));
  ipa_test_ret_code_e ret_code;
  uint16 i = 0;
  ipa_test_engine_pkt_trace_elem_s pkt;

  /* Memset the pkt trace to 0 */
  IPA_MEMSET(&pkt, 0 , sizeof(ipa_test_engine_pkt_trace_elem_s));

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_DL_LOOPBACK);

  pkt.dst = IPA_TEST_ENGINE_PKT_DST_SIM;
  pkt.dst_idx = pkt_dst_idx;
  pkt.src_idx = 0;
  pkt.src = IPA_TEST_ENGINE_PKT_SRC_UNKNOWN;
  pkt_cmp_ptr->rcvd_pkt_dsm_ptr = NULL;

  while(i < num_pkts_rx)
  {
    pkt_cmp_ptr->sent_pkt_dsm_ptr = dsm_dequeue(mirror_wm_ptr);
    IPA_TF_ASSERT(pkt_cmp_ptr->sent_pkt_dsm_ptr != NULL);

    pkt_cmp_ptr->sent_pkt_len = 
      dsm_length_packet(pkt_cmp_ptr->sent_pkt_dsm_ptr);

    pkt_cmp_ptr->rcvd_pkt_len = pkt_info_arr[i].pkt_len;

    if (pkt_cmp_ptr->rcvd_pkt_len != pkt_cmp_ptr->sent_pkt_len)
    {
      flag = 1;
      ipa_test_engine.pkt_cmp.compare_failure_id = 33;
      break;
    }
#if 0
    /* Check the Mux ID received */
    if ((pkt_info_arr[i].mux_id != 0) && 
        (pkt_cmp_ptr->sent_pkt_dsm_ptr->kind != pkt_info_arr[i].mux_id))
    {
      flag = 1;
      break;
    }
#endif
    pkt.pkt_len = pkt_cmp_ptr->rcvd_pkt_len;
    pkt.filter_priority = 0;
    pkt.filter_result = 0;
    ipa_test_engine_store_rx_pkt_trace(&pkt);

    /* Now free the mirror DSM item */
    dsm_free_packet(&pkt_cmp_ptr->sent_pkt_dsm_ptr);
    i++;
  }

  /* Now loop the packet back to SIM consumer watermark */
  if (flag == 0)
  {
    if (ipa_test_engine.ip_pkt.curr_num_pkts > 0)
    {
      ret_code = ipa_test_engine_ul_tx_pkts();
      if (ret_code == IPA_TEST_RET_CODE_SUCCESS)
      {
        /* Now we can free up the memory that we malloced because it has been copied 
        to DSM items */
        ret_code = ipa_test_engine_destroy_ip_pkts();
      }
      
      return ret_code;
    }
    else
    {
      return IPA_TEST_RET_CODE_SUCCESS;
    }
  }
  else
  {
    return IPA_TEST_RET_CODE_FAILURE;
  }
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_sim_rx_pkts

==============================================================================*/
/*!
  @brief
  SIM pkt processing 

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_sim_rx_pkts
(
  uint8 sim_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  dsm_watermark_type *wm_ptr = NULL;
  uint8 bucket_num = 0, num_pkts_used, num_pkts_used_ul, num_pkts_used_dl;
  uint16 i = 0, j = 0;

  /*! update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_SIM_RX);

  IPA_TF_ASSERT(sim_idx < ipa_test_engine.endp.curr_phy_ch_cnt);

  /* Make sure our pkt cache is clean when we reach here */
  if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE && sim_idx == 1)
  {
  	IPA_TF_ASSERT(ipa_test_engine.ip_pkt.curr_num_pkts_ul == 0);
	IPA_TF_ASSERT(ipa_test_engine.ip_pkt.curr_num_pkts_dl == 0);
  }
  else
  {
    IPA_TF_ASSERT(ipa_test_engine.ip_pkt.curr_num_pkts == 0);
  }

  /* Now deaggregate the pkts into our cache */

  wm_ptr = &ipa_test_engine.endp.phy_ch[sim_idx].per_sim.from_ipa_wm;
  while (((ipa_test_engine.curr_test.aggr[sim_idx].frame_dsm_ptr = \
           dsm_dequeue(wm_ptr)) != NULL) &&
         (ret_code == IPA_TEST_RET_CODE_SUCCESS))
  {
    /* Deaggregate the frame recvd and then do pkt comparison */

	if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE && sim_idx == 1)
	{
		IPA_MSG("Packet received on Replication Pipe");

		if (ipa_test_engine.curr_test.cfg.dl_holb_enabled == TRUE && 
  	        ipa_test_engine.curr_test.num_pkts_tot > 20)
		{
		  IPA_ASSERT(0);
		}

		ret_code = 
		   ipa_test_deaggregate_replicated_pkts(&ipa_test_engine.ip_pkt.pkt_info_arr_replication_ul[i],
                                           (uint8 *)&num_pkts_used_ul,
                                           &ipa_test_engine.ip_pkt.pkt_info_arr_replication_dl[j],
                                           (uint8 *)&num_pkts_used_dl,
                                           &ipa_test_engine.curr_test.aggr \
                                           [sim_idx].frame_dsm_ptr);

		i += num_pkts_used_ul;
		j += num_pkts_used_dl;

		num_pkts_used = num_pkts_used_ul + num_pkts_used_dl;


		ipa_test_engine.ip_pkt.curr_num_pkts_ul = i;
		ipa_test_engine.ip_pkt.curr_num_pkts_dl = j;
	}
	else
	{
    ret_code =
      ipa_test_engine_deaggregate_endp_pkt(ipa_test_engine.endp.phy_ch[sim_idx] \
                                           .log_ch_ptr_arr[0]->endp_cfg_ptr,
                                           IPA_TEST_ENGINE_IP_PKT_NUM_MAX - i,
                                           &ipa_test_engine.ip_pkt.pkt_info_arr[i],
                                           (uint8 *)&num_pkts_used,
                                           &ipa_test_engine.curr_test.aggr \
                                           [sim_idx].frame_dsm_ptr);

		i += num_pkts_used;

		/* Increment the number of pkts in the cache */
  		ipa_test_engine.ip_pkt.curr_num_pkts = i;

	}
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    IPA_TF_ASSERT(ipa_test_engine.curr_test.aggr[sim_idx].frame_dsm_ptr == NULL);

    /* Now store aggregation histogram */
    bucket_num = 
      IPA_STATS_AGGR_HIST_GET_BUCKET_NUM(num_pkts_used);
    ipa_test_engine.curr_test.aggr[sim_idx].deaggr_hist[bucket_num].num_frames++;

    
  }
  

  	if (ret_code == IPA_TEST_RET_CODE_SUCCESS)
  	{
  		if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE && sim_idx == 1)
  		{
  			if (ipa_test_engine.ip_pkt.curr_num_pkts_ul != 0)
  			{
  				ret_code = 
        			ipa_test_engine_compare_rx_pkts_n_mirror_wm(&ipa_test_engine.endp.phy_ch \
                                                    [sim_idx].per_sim \
                                                    .mirror_from_ipa_wm_ul,
                                                    &ipa_test_engine.pkt_cmp,
                                                    &ipa_test_engine.ip_pkt \
                                                    .pkt_info_arr_replication_ul[0],
                                                    ipa_test_engine.ip_pkt.curr_num_pkts_ul,
                                                    IPA_TEST_ENGINE_PKT_DST_SIM,
                                                    sim_idx);

				IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

				ipa_test_engine.curr_test.num_pkts_hw_rep = ipa_test_engine.curr_test.num_pkts_hw_rep 
					  											+ ipa_test_engine.ip_pkt.curr_num_pkts_ul;

				ret_code = ipa_test_engine_destroy_ul_replicated_ip_pkts();
      			IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  			}

			if (ipa_test_engine.ip_pkt.curr_num_pkts_dl != 0)
  {
				ret_code = 
        			ipa_test_engine_compare_rx_pkts_n_mirror_wm(&ipa_test_engine.endp.phy_ch \
                                                    [sim_idx].per_sim \
                                                    .mirror_from_ipa_wm_dl,
                                                    &ipa_test_engine.pkt_cmp,
                                                    &ipa_test_engine.ip_pkt \
                                                    .pkt_info_arr_replication_dl[0],
                                                    ipa_test_engine.ip_pkt.curr_num_pkts_dl,
                                                    IPA_TEST_ENGINE_PKT_DST_SIM,
                                                    sim_idx);

				IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

				ipa_test_engine.curr_test.num_pkts_hw_rep = ipa_test_engine.curr_test.num_pkts_hw_rep 
					  											+ ipa_test_engine.ip_pkt.curr_num_pkts_dl;

				ret_code = ipa_test_engine_destroy_dl_replicated_ip_pkts();
      			IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
			
			}


  		}
  	    else if (ipa_test_engine.curr_test.cfg.test_type == 
             IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK)
    {
      ret_code = 
        ipa_test_engine_sim_rx_loopback_pkts(&ipa_test_engine.endp.phy_ch \
                                             [sim_idx].per_sim \
                                             .mirror_from_ipa_wm,
                                             &ipa_test_engine.pkt_cmp,
                                             &ipa_test_engine.ip_pkt \
                                             .pkt_info_arr[0],
                                             ipa_test_engine.ip_pkt \
                                             .curr_num_pkts,
                                             sim_idx);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

	  if (ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled == TRUE &&
	 	 ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_UL_HOLB)
      {
        ipa_test_engine.pkt_count_for_holb = ipa_test_engine.pkt_count_for_holb + 
			 									ipa_test_engine.ip_pkt.curr_num_pkts;
      }
    }
    else
    {
      ret_code = 
        ipa_test_engine_compare_rx_pkts_n_mirror_wm(&ipa_test_engine.endp.phy_ch \
                                                    [sim_idx].per_sim \
                                                    .mirror_from_ipa_wm,
                                                    &ipa_test_engine.pkt_cmp,
                                                    &ipa_test_engine.ip_pkt \
                                                    .pkt_info_arr[0],
                                                    ipa_test_engine.ip_pkt \
                                                    .curr_num_pkts,
                                                    IPA_TEST_ENGINE_PKT_DST_SIM,
                                                    sim_idx);
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
	  
      if(ipa_test_engine.curr_test.cfg.ul_holb_enabled == TRUE)
  	  {
        ipa_test_engine.pkt_count_for_holb = ipa_test_engine.pkt_count_for_holb +
												ipa_test_engine.ip_pkt.curr_num_pkts;
      }

      /* Now free up the memory in our temp cache */
      ret_code = ipa_test_engine_destroy_ip_pkts();
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    }
  }

  if ((ipa_test_engine.curr_test.cfg.dl_holb_enabled == TRUE &&
  	   ipa_test_engine.curr_test.num_pkts_tot == 10))
  {
    //ipa_test_per_sim_dsm_reinit();
    ret_code = ipa_test_engine_generate_ip_pkts();
	
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    ipa_test_engine_ul_tx_pkts();

	
	ret_code = ipa_test_engine_destroy_ip_pkts();
	IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
  else if (ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled == TRUE &&
  	   ipa_test_engine.curr_test.num_pkts_hw_rep == 10 &&
  	   ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB &&
  	   ipa_test_engine.curr_test.is_holb == FALSE)
  {
    ipa_test_engine.curr_test.cfg.is_hw_replication_enabled = FALSE;
	ipa_test_engine.curr_test.is_holb = TRUE;
    ret_code = ipa_test_engine_generate_ip_pkts();
	
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    ipa_test_engine_ul_tx_pkts();

	
	ret_code = ipa_test_engine_destroy_ip_pkts();
	IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
  else if (ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled == TRUE &&
  	   ipa_test_engine.curr_test.num_pkts_hw_rep == 10 &&
  	   ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_UL_HOLB)
  {
    //ipa_test_per_sim_dsm_reinit();
    ret_code = ipa_test_engine_generate_ip_pkts();
	
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    ipa_test_engine_dl_tx_pkts();

  	
	ret_code = ipa_test_engine_destroy_ip_pkts();
	IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }


  if(ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled == TRUE &&
  	 ipa_test_engine.pkt_count_for_holb == ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts && 
  	 ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_UL_HOLB)
  {
    ipa_test_per_sim_dsm_reinit(1);
	ipa_cfg.dl.is_hw_rep_holb_test_enabled = FALSE;
	ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled = FALSE;
	ipa_test_engine.pkt_count_for_holb = 0;
  }
  else if(ipa_test_engine.curr_test.cfg.ul_holb_enabled == TRUE &&
  			ipa_test_engine.pkt_count_for_holb == ipa_test_engine.curr_test.cfg.ip_pkt_cfg.num_pkts)
  {
    ipa_hal_endp_update_aggr((ipa_hw_pipe_id_e)IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_UL_PRODUCER_PIPE), 1);
    ipa_ul_prdcr_bam_dsm_topup();
	ipa_cfg.ul.is_holb_test_enabled = FALSE;
	ipa_test_engine.curr_test.cfg.ul_holb_enabled = FALSE;
	ipa_test_engine.pkt_count_for_holb = 0;
	
  }
  
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_wait_for_data_ind

==============================================================================*/
/*!
  @brief
  Waits for the test signal to be set to indicate the data activity

  @returns
  Bit index which generated the data indication
*/
/*============================================================================*/
uint8 ipa_test_engine_wait_for_data_ind(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;
  uint8 ret_idx = 0;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_RX_WAIT);

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.wait_signal);

  while (ipa_test_engine.curr_test.wm_cb_bit_mask == 0)
  {
    set_signals = ipa_signal_mask_wait(&sig_mask);
    IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.wait_signal));
    ipa_signal_clear(&ipa_test_engine.wait_signal);
  }

  /* Get the first bit set in the bitmask */
  ret_idx = 
    ipa_test_get_index_from_bitmask(ipa_test_engine.curr_test.wm_cb_bit_mask);
  IPA_TF_ASSERT(ret_idx != IPA_TEST_ENGINE_ALL_BITMASK_POS);

  /* Now clear that bitmask before processing it */
  ipa_crit_sect_enter(&ipa_test_engine.crit_sect);
  ipa_test_reset_64_bitmask(&ipa_test_engine.curr_test.wm_cb_bit_mask, ret_idx);
  ipa_crit_sect_leave(&ipa_test_engine.crit_sect);

  return ret_idx;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_validate_deaggr_hist_stats

==============================================================================*/
/*!
  @brief
  Checks if DEAGG_FIRST_BIT is set in status mask and deaggr hist stats is 
  updated correctly.
  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_validate_deaggr_hist_stats(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 deaggr_first_cnt = 0;
  uint8 stts_idx, loop_idx, loop_cnt;
  
  /*! 1. Check Deaggr first bit set in the status mask. 2 Status pkts 
         with DEAGG_FIRST bit set should be recevied for test to pass.
  */
  ipa_test_engine.curr_test.curr_stts_log_wr_idx = 
     ipa_ul_get_curr_status_log_wr_idx();

  if (((ipa_test_engine.curr_test.curr_stts_log_wr_idx 
       - ipa_test_engine.curr_test.prev_stts_log_wr_idx) >= (ipa_test_engine.curr_test.num_pkts_tx)) 
      && (ipa_test_engine.curr_test.prev_stts_log_wr_idx < 
       ipa_test_engine.curr_test.curr_stts_log_wr_idx)) 
  {
     loop_cnt = ipa_test_engine.curr_test.curr_stts_log_wr_idx - 
                ipa_test_engine.curr_test.prev_stts_log_wr_idx;
  }
  else
  {
     loop_cnt = IPA_UL_DEAGGR_STATUS_LOG_NUM - 
               ipa_test_engine.curr_test.prev_stts_log_wr_idx +
               ipa_test_engine.curr_test.curr_stts_log_wr_idx;
  }

  if (loop_cnt > 32)
  {
    loop_cnt = 32;
  }

  /*! Happy looping ! :)*/
  for (stts_idx = ipa_test_engine.curr_test.prev_stts_log_wr_idx, loop_idx = 0;
      loop_idx < loop_cnt ; stts_idx++, loop_idx++)
  {
    if (ipa_ul_is_deaggr_stts_mask_first_bit_set(stts_idx) == TRUE)
    {
      deaggr_first_cnt++;
    }
    /*! If we need to still traverse the status log beyond the max length 
        since iyts a circular array, we set i to zero
        and end_idx to the curr write index*/
    if (stts_idx == IPA_UL_DEAGGR_STATUS_LOG_NUM - 1) 
    {
       stts_idx = UINT8_MINUS_1;
    }
  }

  IPA_TF_ASSERT_LOG(deaggr_first_cnt == 2, "DEAGG_FIRST bit not set!", 
      deaggr_first_cnt, stts_idx, loop_cnt);
  
  /*! 2. check deaggr hist stats of UL Per Sim Consumer pipe 1. Bucket 1 
        (3 pkts) needs to be updated for test to pass.
  */
  IPA_TF_ASSERT_LOG(ipa_stats.endp[IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_0_ID]. \
                    ul.deaggr_hist[1].num_frames == 1, 
                    "Hist stats not updated!", 0, 0, 0);
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_validate_prev_eot_byte_threshold

==============================================================================*/
/*!
  @brief
  DL aggr PREV_EOT test. If the subsequent pkt after 1st pkt is exceeding the
  aggr BYTE_LIMIT, pkt will be a part of a new frame with PREV_EOT bit set in
  the status.
 
  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_validate_prev_eot_byte_threshold()
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 prev_eot_byte_thresh_stts_cnt = 0;
  uint8 stts_idx, loop_idx, loop_cnt;

   /*! 1. TODO: Need to add DL PREV_EOT status handlng before checking for stats.
          2 eot frames with one pkt each should be received on the 
          DL PER SIM Prod pipe:PIPE0*/
  ipa_test_engine.curr_test.curr_stts_log_wr_idx = 
     ipa_dl_cnsmr_get_curr_stts_log_wr_idx();

  if (((ipa_test_engine.curr_test.curr_stts_log_wr_idx 
       - ipa_test_engine.curr_test.prev_stts_log_wr_idx) >= (2 * ipa_test_engine.curr_test.num_pkts_tx)) 
      && (ipa_test_engine.curr_test.prev_stts_log_wr_idx < 
       ipa_test_engine.curr_test.curr_stts_log_wr_idx)) 
  {
     loop_cnt = ipa_test_engine.curr_test.curr_stts_log_wr_idx - 
                ipa_test_engine.curr_test.prev_stts_log_wr_idx;
  }
  else
    {
     loop_cnt = IPA_DL_PRDCR_STATUS_LOG_MAX - 
               ipa_test_engine.curr_test.prev_stts_log_wr_idx +
               ipa_test_engine.curr_test.curr_stts_log_wr_idx;
  }

  if (loop_cnt > 32)
  {
    loop_cnt = 32;
  }
  /*! 2. Prev_eot bit needs to be set in status mask for 2nd pkt. 
      Have some basic checking for the 2 frames for now.*/
  for (stts_idx = ipa_test_engine.curr_test.prev_stts_log_wr_idx, loop_idx = 0;
      loop_idx < loop_cnt ; stts_idx++, loop_idx++)
  {
    if(ipa_dl_prod_preveot_stts_mask_set(stts_idx) == TRUE)
    {
      prev_eot_byte_thresh_stts_cnt++;
    }
    if (stts_idx == IPA_DL_PRDCR_STATUS_LOG_MAX - 1) 
    {
       stts_idx = UINT8_MINUS_1;
    }
  }

  /*Assert if any of the above fails*/
  IPA_TF_ASSERT_LOG(prev_eot_byte_thresh_stts_cnt == 1,"DL Aggr prev_eot fail!", 
      prev_eot_byte_thresh_stts_cnt, stts_idx, loop_cnt);
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_validate_force_close_preveot_status

==============================================================================*/
/*!
  @brief
  DL aggr PREV_EOT test. If the subsequent pkt after 1st pkt is exceeding the
  aggr BYTE_LIMIT, pkt will be a part of a new frame with PREV_EOT bit set in
  the status.
 
  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_validate_force_close_preveot_status()
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 prev_eot_stts_cnt = 0;
  uint8 stts_idx, loop_idx, loop_cnt;

   /*! 1. TODO: Need to add DL PREV_EOT status handlng before checking for stats.
          2 eot frames with one pkt each should be received on the 
          DL PER SIM Prod pipe:PIPE0*/
  ipa_test_engine.curr_test.curr_stts_log_wr_idx = 
     ipa_dl_get_curr_stts_log_wr_idx();

  if (((ipa_test_engine.curr_test.curr_stts_log_wr_idx 
       - ipa_test_engine.curr_test.prev_stts_log_wr_idx) >= (2 * ipa_test_engine.curr_test.num_pkts_tx)) 
      && (ipa_test_engine.curr_test.prev_stts_log_wr_idx < 
       ipa_test_engine.curr_test.curr_stts_log_wr_idx)) 
  {
     loop_cnt = ipa_test_engine.curr_test.curr_stts_log_wr_idx - 
                ipa_test_engine.curr_test.prev_stts_log_wr_idx;
  }
  else
  {
     loop_cnt = IPA_DL_PRDCR_STATUS_LOG_MAX - 
               ipa_test_engine.curr_test.prev_stts_log_wr_idx +
               ipa_test_engine.curr_test.curr_stts_log_wr_idx;
  }
        
  if (loop_cnt > 32)
  {
    loop_cnt = 32;
  }
        
  /*! 2. Prev_eot bit needs to be set in status mask for 2nd pkt. 
      Have some basic checking for the 2 frames for now.*/
  for (stts_idx = ipa_test_engine.curr_test.prev_stts_log_wr_idx, loop_idx = 0;
      loop_idx < loop_cnt ; stts_idx++, loop_idx++)
  {
    if(ipa_dl_prod_preveot_stts_mask_set(stts_idx) == TRUE)
    {
      prev_eot_stts_cnt++;
    }
    if (stts_idx == IPA_DL_PRDCR_STATUS_LOG_MAX - 1) 
    {
       stts_idx = UINT8_MINUS_1;
    }
  }

  /*Assert if any of the above fails*/
  IPA_TF_ASSERT_LOG(prev_eot_stts_cnt == 2,"DL Aggr prev_eot fail!", 
      prev_eot_stts_cnt, stts_idx, loop_cnt);
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_validate_pkt_dropped_byte_threshold

==============================================================================*/
/*!
  @brief
  DL aggr BYTE_LIMIT pkt drop test. If the first or the single pkt in frame
  exceeds the aggregation BYTE_LIMIT. Checks dopped pkt stats.
 
  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_validate_pkt_dropped_byte_threshold()
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 pkt_drop_byte_thresh_cnt = 0;
  uint8 stts_idx, loop_idx, loop_cnt;

  /*! 1. Drop pkt cnt needs to be 1*/
  /* For now have the check for >0 as stats is not reset b/w tests and will keep 
     on incrementing. */
  ipa_test_engine.curr_test.curr_stts_log_wr_idx = 
     ipa_dl_cnsmr_get_curr_stts_log_wr_idx();

  if(ipa_stats.dl.num_p_drop_aggr_pkts == 0)
  {
     ret_code = IPA_TEST_RET_CODE_FAILURE;
  }

  if (((ipa_test_engine.curr_test.curr_stts_log_wr_idx 
       - ipa_test_engine.curr_test.prev_stts_log_wr_idx) >= (2 * ipa_test_engine.curr_test.num_pkts_tx)) 
      && (ipa_test_engine.curr_test.prev_stts_log_wr_idx < 
       ipa_test_engine.curr_test.curr_stts_log_wr_idx)) 
  {
     loop_cnt = ipa_test_engine.curr_test.curr_stts_log_wr_idx - 
                ipa_test_engine.curr_test.prev_stts_log_wr_idx;
  }
  else
  {
     loop_cnt = IPA_DL_PRDCR_STATUS_LOG_MAX - 
               ipa_test_engine.curr_test.prev_stts_log_wr_idx +
               ipa_test_engine.curr_test.curr_stts_log_wr_idx;
  }
        
  if (loop_cnt > 32)
  {
    loop_cnt = 32;
  }
        
  /*! 2. Prev_eot bit needs to be set in status mask for 2nd pkt. 
      Have some basic checking for the 2 frames for now.*/
  for (stts_idx = ipa_test_engine.curr_test.prev_stts_log_wr_idx, loop_idx = 0;
      loop_idx < loop_cnt ; stts_idx++, loop_idx++)
  {
    if(ipa_dl_prod_byte_limit_stts_mask_set(stts_idx) == TRUE)
    {
      pkt_drop_byte_thresh_cnt++;
    }
    if (stts_idx == IPA_DL_PRDCR_STATUS_LOG_MAX -1) 
    {
       stts_idx = UINT8_MINUS_1;
    }
  }
  /*Assert if any of the above fails*/
  IPA_TF_ASSERT_LOG(pkt_drop_byte_thresh_cnt == 1,"DL Aggr BYTE_LIMIT fail!", 
      pkt_drop_byte_thresh_cnt, stts_idx, loop_cnt);

  return ret_code;
}


/*==============================================================================

  FUNCTION:  ipa_test_engine_validate_deaggr_err_handling

==============================================================================*/
/*!
  @brief
  Validates statuses for deaggr error scenarios
  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_validate_deaggr_err_handling()
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  ipa_test_engine_deaggr_err_info_s * deaggr_err_info =
    &ipa_test_engine.curr_test.deaggr_err_info;

  switch (ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT:
    {
      IPA_TF_ASSERT(deaggr_err_info->exception_cnt == 2);
       
    }
    break;
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1:
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE:
    {
       IPA_TF_ASSERT(deaggr_err_info->exception_cnt == 1);
    }
    break;
    
    case IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1:
    {
      IPA_TF_ASSERT(deaggr_err_info->exception_cnt == 3);
      IPA_TF_ASSERT(deaggr_err_info->iptype_exception_cnt == 1);
    }
    break;

    default:
    break;
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_aggr_wait_rx_pkts

==============================================================================*/
/*!
  @brief
  Validates statuses for different 2.1 aggr/deaggr tests for EOT force
  close cases.
  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_aggr_wait_rx_pkts(void)
{

  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! Check the IPA status based on the test and accordingly pass/fail the test.*/
  switch (ipa_test_engine.curr_test.cfg.rx_pkt_test_type)
  {
    case IPA_TEST_ENGINE_RX_PKT_WAIT_PREVEOT_BYTE_THRESHOLD:
    {   
      /* Validate PREV_EOT bit set in status mask*/
      ret_code = ipa_test_engine_validate_prev_eot_byte_threshold();
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

      /* Set FORCE_CLOSE on dest ep for IPA to close the second frame*/
      ipa_hal_endp_close_aggr
        ((ipa_hw_pipe_id_e)IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_0_ID);
    }
    break;

    case IPA_TEST_ENGINE_RX_PKT_WAIT_AGGR_FORCE_CLOSE_PIPES:
    {
      /* Validate if 2 PREV_EOTs are received for pkts sent on 2 pipes */
      ret_code = ipa_test_engine_validate_force_close_preveot_status();
      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

      /* Set FORCE_CLOSE on only Pipe 0 for IPA to close the second frame 
         for Pipe0. Only Pipe 0 frame should be closed and pkts should be received. 
         If above happes then FORCE_CLOSE can be set on Pipe 1 to receives pkts 
         to pass the test.
      */
      ipa_hal_endp_close_aggr
        ((ipa_hw_pipe_id_e)IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_0_ID);

      /* Start Timer to detect if force close was applied only PIPE0. */
      timer_set(&ipa_test_engine.aggr_force_close_timer.timer, 
              IPA_TEST_ENGINE_AGGR_FORCE_CLOSE_TEST_INTERVAL, 0, T_MSEC);
    }
      break;

    default:
    break;
  }
  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_verify_deaggr_rx_pkts

==============================================================================*/
/*!
  @brief
  Validates statuses for different 2.1 aggr/deaggr tests.
  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_verify_deaggr_rx_pkts(void)
{

  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /*! Check the IPA status based on the test and accordingly pass/fail the test.*/
  switch (ipa_test_engine.curr_test.cfg.rx_pkt_test_type)
  {
    case IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_HIST_STATS:
    {
      ret_code = ipa_test_engine_validate_deaggr_hist_stats();
      break;
    }
    case IPA_TEST_ENGINE_RX_PKT_WAIT_DROPPED_PKT_BYTE_THRESHOLD:
    {
      ret_code = ipa_test_engine_validate_pkt_dropped_byte_threshold();
      break;
    }
    case IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING:
    {
      ret_code = ipa_test_engine_validate_deaggr_err_handling();
      break;
    }
    default:
    break;
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_rx_pkts

==============================================================================*/
/*!
  @brief
  For DL:
  1. Receive aggregated frame from test per sim and compare with watermark
  for UL:
  1. Receive pkts from IPA driver and compare with watermark

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_rx_pkts(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.watchdog_timer.timer , 
            IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL, 0, T_MSEC);

  /*Start Flow Control Timer if enabled.*/
  if(TRUE == ipa_test_engine.curr_test.cfg.is_fc_enabled)
  {
    /*Start Timer to check if packets are being received after ENDP_DELAY is set.
     *On timer expiry,IPA should not have processed any pkts on UL Per Pipe for 
     *test to pass.
     */
    timer_set(&ipa_test_engine.flow_ctl_timer.timer, 
              IPA_TEST_ENGINE_FLOW_CTL_DETECT_INTERVAL, 0, T_MSEC);
  }

  /*! Hack for the dereg special case only. Sice we will be receiving 
    only half of the sent packets back*/
  if (ipa_test_engine.curr_test.cfg.wan_cfg.is_bearer_dereg_case == TRUE)
  {
    ipa_test_engine.curr_test.num_pkts_tx = ipa_test_engine.curr_test.num_pkts_tx/2;
  }
      
  /* TODO make a bitmask for all these checks */
  /*! For few pkt wait tests check appropriate stats/status to pass/fail test*/
  if ((ipa_test_engine.curr_test.cfg.rx_pkt_test_type == 
       IPA_TEST_ENGINE_RX_PKT_WAIT_PREVEOT_BYTE_THRESHOLD) || 
      (ipa_test_engine.curr_test.cfg.rx_pkt_test_type ==
       IPA_TEST_ENGINE_RX_PKT_WAIT_AGGR_FORCE_CLOSE_PIPES) ||
      (ipa_test_engine.curr_test.cfg.rx_pkt_test_type ==
       IPA_TEST_ENGINE_RX_PKT_WAIT_DROPPED_PKT_BYTE_THRESHOLD)||
      (ipa_test_engine.curr_test.cfg.rx_pkt_test_type ==
       IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_HIST_STATS)||
      (ipa_test_engine.curr_test.cfg.rx_pkt_test_type ==
       IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING))
  {
     /*!Wait for all the statuses to arrive */
    ipa_sleep(100); //TODO

    if((ipa_test_engine.curr_test.cfg.rx_pkt_test_type == 
       IPA_TEST_ENGINE_RX_PKT_WAIT_PREVEOT_BYTE_THRESHOLD) || 
      (ipa_test_engine.curr_test.cfg.rx_pkt_test_type ==
       IPA_TEST_ENGINE_RX_PKT_WAIT_AGGR_FORCE_CLOSE_PIPES))

  {
    ret_code = ipa_test_engine_aggr_wait_rx_pkts();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
  }

  /*if (ipa_test_engine.curr_test.cfg.is_hw_replication_enabled  == TRUE)
  	ipa_test_engine.curr_test.num_pkts_tx = 2 * ipa_test_engine.curr_test.num_pkts_tx;*/
  
  /*! Wait for pkts to come back for pkt wait tests and prev_eot test*/
  if ((IPA_TEST_ENGINE_RX_PKT_WAIT == 
       ipa_test_engine.curr_test.cfg.rx_pkt_test_type) ||
     (TRUE == ipa_test_engine.curr_test.pkt_wait))
  {
    /* Wait till we get all the pkts that we expected */
    while (ipa_test_engine.curr_test.num_pkts_rx != 
           ipa_test_engine.curr_test.num_pkts_tx) 
    {
      /*! Poll on GSI per sim channels*/

	  #ifndef FEATURE_IPA_USE_IPA_BAM_DRIVER
      IPA_ASSERT(
         (IPA_GSI_STATUS_SUCCESS == ipa_test_per_sim_pipe_poll()));
	  #endif

      cb_bit_idx = ipa_test_engine_wait_for_data_ind();

      if ((cb_bit_idx >= IPA_TEST_ENGINE_UL_BITMASK_POS) &&
          (cb_bit_idx < IPA_TEST_ENGINE_DPL_BITMASK_POS))
      {
        ret_code = 
          ipa_test_engine_ul_rx_pkts((cb_bit_idx - IPA_TEST_ENGINE_UL_BITMASK_POS));
      }
      else if (cb_bit_idx == IPA_TEST_ENGINE_DPL_BITMASK_POS) 
      {
        ret_code = 
          ipa_test_engine_dpl_rx_pkts(IPA_TEST_ENGINE_DPL_BITMASK_POS);
      }
      else if ((cb_bit_idx >= IPA_TEST_ENGINE_DL_BITMASK_POS) &&
               (cb_bit_idx < IPA_TEST_ENGINE_SIM_BITMASK_POS))
      {
        ret_code = 
          ipa_test_engine_dl_rx_pkts((cb_bit_idx - IPA_TEST_ENGINE_DL_BITMASK_POS));
      }
      else if ((cb_bit_idx >= IPA_TEST_ENGINE_SIM_BITMASK_POS) &&
               (cb_bit_idx < IPA_TEST_ENGINE_UL_PHY_BITMASK_POS))
      {
        ret_code = 
          ipa_test_engine_sim_rx_pkts((cb_bit_idx - IPA_TEST_ENGINE_SIM_BITMASK_POS));
      }
      else if ((cb_bit_idx >= IPA_TEST_ENGINE_UL_PHY_BITMASK_POS) &&
               (cb_bit_idx < IPA_TEST_ENGINE_ALL_BITMASK_POS))
      {
        ret_code = 
          ipa_test_engine_ul_phy_rx_pkts((cb_bit_idx - IPA_TEST_ENGINE_UL_PHY_BITMASK_POS));
      }
      else
      {
        IPA_TF_ASSERT(0);
      }

      IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

      if ( ( ipa_test_engine.curr_test.num_pkts_rx == ipa_test_engine.curr_test.num_pkts_tot ) &&
           ( ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE ) )
      {
		IPA_ULOG_MSG_SPRINTF_1("IPA Force Close Aggr called for Pipe %d", IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_1_ID);
        ipa_hal_ep_force_close_aggr(IPA_HW_Q6_SIM_DL_PRODUCER_PIPE_1_ID, FALSE);
      }
    }
  } 

  /*! For few pkt wait tests check appropriate stats/status to pass/fail test*/
  if (IPA_TEST_ENGINE_RX_PKT_WAIT != 
      ipa_test_engine.curr_test.cfg.rx_pkt_test_type)
  {
    IPA_TF_ASSERT(ipa_test_engine_verify_deaggr_rx_pkts() == IPA_TEST_RET_CODE_SUCCESS);
  }
  
  /* Stop the timer after receiving all the pkts */
  timer_clr(&ipa_test_engine.watchdog_timer.timer, T_NONE);

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_run_init

==============================================================================*/
/*!
  @brief
  Inits the test run params after the cfg is done 

  @returns
  None
*/
/*============================================================================*/
void ipa_test_engine_run_init(void)
{
  ipa_test_engine.curr_test.num_pkts_tx = 0;
  ipa_test_engine.curr_test.num_pkts_rx = 0;
  ipa_test_engine.curr_test.num_pkts_rx_to_process = 0;
  ipa_test_engine.curr_test.num_pkts_rx_wdog_prev = 0;

  /*! Clear the signal*/
  ipa_signal_clear(&ipa_test_engine.wait_signal);
  /*! Clear the signal*/
  ipa_signal_clear(&ipa_test_engine.bearer_susp_wait_signal);
  /*! Clear the signal*/
  ipa_signal_clear(&ipa_test_engine.bearer_active_wait_signal);
    /*! Clear the signal*/
  ipa_signal_clear(&ipa_test_engine.bearer_dereg_wait_signal);
    /*! Clear the signal*/
  ipa_signal_clear(&ipa_test_engine.bearer_reg_wait_signal);

  ipa_test_filter_run_init();
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_run

==============================================================================*/
/*!
  @brief
  Runs the test after the cfg is done 

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_run(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint32 pc_sleep_rand = 0;
  //ipa_hwio_def_ipa_rx_proc_cmdq_cfg_u rx_cmdq_cfg;

  while ((ipa_ipfltr.fltr.dl.inprog.num_rules != ipa_ipfltr.fltr.dl.active.num_rules + ipa_ipfltr.fltr.dl.active.num_hash_rules) &&
         (ipa_ipfltr.fltr.ul.inprog.num_rules != ipa_ipfltr.fltr.ul.active.num_rules + ipa_ipfltr.fltr.ul.active.num_hash_rules) &&
         (ipa_ipfltr.rtng.dl.inprog.num_rules != ipa_ipfltr.rtng.dl.active.num_rules + ipa_ipfltr.rtng.dl.active.num_hash_rules) &&
         (ipa_ipfltr.rtng.ul.inprog.num_rules != ipa_ipfltr.rtng.ul.active.num_rules + ipa_ipfltr.rtng.ul.active.num_hash_rules))
  {
           ipa_sleep(2);
  }
  //Wait for filters to be configured
  while ((ipa_ipfltr.fltr.ul.pending.pending_activation_flag == TRUE) ||
         (ipa_ipfltr.fltr.dl.pending.pending_activation_flag == TRUE) ||
         (ipa_ipfltr.rtng.ul.pending.pending_activation_flag == TRUE) ||
         (ipa_ipfltr.rtng.dl.pending.pending_activation_flag == TRUE)) 
  {
    ipa_sleep(2);
  }

  /* We need to test power collapse in the driver. So sleep for sometime between
  pkt transmission to trigger power collapse */
  if (ipa_test_engine.curr_test.cfg.is_pc_enabled == TRUE)
  {
    /* Chose a number between 4 seconds to 0 milliseconds */
    pc_sleep_rand = 
      ipa_test_generate_random_num(620, 520);
		IPA_MSG_1("IPA_QTF_TEST: Sleep for %d ms", pc_sleep_rand);
    ipa_sleep(pc_sleep_rand);
  }
	else if (ipa_test_engine.curr_test.cfg.is_ul_prod_pipe_suspend_enabled == TRUE)
	{
	  IPA_MSG("IPA_QTF_TEST: Sleep for 2500 ms");
      ipa_sleep(2500);
	}

  ipa_test_engine_run_init();

  /* Enable HOLB verification */
  if (ipa_test_engine.curr_test.cfg.zip_cfg.is_holb_ver_enabled == TRUE)
  {
	if (ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_UL)
	{
      ipa_hal_endp_update_aggr((ipa_hw_pipe_id_e)IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_UL_PRODUCER_PIPE), 0);
	  ipa_cfg.ul.is_holb_test_enabled = TRUE;
	}
  }
  else if (ipa_test_engine.curr_test.cfg.dl_holb_enabled == TRUE)
  {
    ipa_cfg.dl.is_holb_test_enabled = TRUE;
  }
  else if (ipa_test_engine.curr_test.cfg.hw_rep_holb_enabled == TRUE)
  {
    ipa_cfg.dl.is_hw_rep_holb_test_enabled = TRUE;
  }
  else if (ipa_test_engine.curr_test.cfg.ul_holb_enabled == TRUE)
  {
    ipa_cfg.ul.is_holb_test_enabled = TRUE;
	//ipa_cfg.ul.producer_pipe_initial_dsm_items_cnt = 10;
	ipa_hal_endp_update_aggr((ipa_hw_pipe_id_e)IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_UL_PRODUCER_PIPE), 0);
	//IPA_HAL_HWIO_OUT_REG_n(IPA_ENDP_INIT_AGGR_n, IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_UL_PRODUCER_PIPE), 0x0);
  }
  
  ret_code = ipa_test_engine_generate_ip_pkts();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ret_code = ipa_test_engine_tx_pkts();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  if (ipa_test_engine.curr_test.cfg.zip_cfg.is_holb_ver_enabled == TRUE)
  {
    if (ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_DL)
    {
      //TODO
    }
	else
	{
      while(ipa_test_engine.curr_test.num_pkts_rx_to_process != 60)
      {
         ipa_sleep(5);
      }
	}
  }

  ret_code =  ipa_test_engine_zip_request();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  ret_code =  ipa_test_engine_zip_response();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  if (ipa_test_engine.curr_test.cfg.zip_cfg.is_holb_ver_enabled == TRUE)
  {
    if (ipa_test_engine.curr_test.cfg.test_type == IPA_TEST_ENGINE_TEST_TYPE_DL)
    {
      ipa_test_per_sim_dsm_reinit(0);
    }
	else
	{
      ipa_hal_endp_update_aggr((ipa_hw_pipe_id_e)IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_UL_PRODUCER_PIPE), 1);
      ipa_ul_prdcr_bam_dsm_topup();
	  ipa_cfg.ul.is_holb_test_enabled = FALSE;
	}
  }

  ret_code = ipa_test_engine_rx_pkts();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_get_sio_stream_id

==============================================================================*/
/*!
  @brief
  Gets the SIO stream id for the particular bearer.
  
  @returns
  SIO stream id for the particular bearer
*/
/*============================================================================*/
sio_stream_id_type ipa_test_engine_get_sio_stream_id
(
  uint8 uid
)
{
  uint8 i,bearer_index;
  for (i = 0; i < ipa_test_engine.wan.curr_bearer_cnt; i++)
  {
    if(uid == ipa_test_engine.wan.bearer[i].uid)
    {
      bearer_index = i;
    }
  }
  
  return (ipa_test_engine.wan.bearer[bearer_index].log_ch_ptr->stream_id);
  
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_clat_cfg_end

==============================================================================*/
/*!
  @brief
  Wait for CLAT cfg to be done on a particular SIO
 
  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_clat_dereg_end(void)
{
  ipa_os_signal_t set_signals = 0;
  ipa_os_signal_mask_t sig_mask = 0;

  ipa_signal_mask_init(&sig_mask);

   /* Add the signals to the mask we wait for */
  ipa_signal_mask_add(&sig_mask, &ipa_test_engine.clat_dereg_signal);

  /* Start the timer to detect any data stalls */
  timer_set((timer_ptr_type)&ipa_test_engine.watchdog_timer.timer , 
            IPA_TEST_ENGINE_WATCHDOG_STALL_DETECT_INTERVAL_SIO, 0, T_MSEC);

  /* Wait till we get cb's for CLAT_DEREG
  Do we want to wait forever or assert with a timeout? */

  /*! Loop until both clat_reg and assoc_sio is completed for every 
       clat_context_cfg */
  while (ipa_test_engine.clat.clat_cfg_cb.num_pend_dereg_cb != 0) 
  {
    set_signals = ipa_signal_mask_wait(&sig_mask);
    IPA_TF_ASSERT(ipa_signal_is_set(&set_signals, &ipa_test_engine.clat_dereg_signal));
    ipa_signal_clear(&ipa_test_engine.clat_dereg_signal);
  }

  /* Stop the timer after receiving all the cfg callbacks */
  timer_clr(&ipa_test_engine.watchdog_timer.timer, T_NONE);
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_endp_log_ch_destroy

==============================================================================*/
/*!
  @brief
  Destroy the logical SIO cfg after the end of the test

  @return
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_endp_log_ch_destroy
(
  uint8 log_ch_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_FAILURE;
  uint8 clat_idx = 0;
  boolean clat_flag = FALSE;

  /* Input Params checking first */
  IPA_TF_ASSERT(log_ch_idx <= IPA_TEST_ENGINE_LOG_CH_NUM_MAX);

  /* Open the logical sio ports first */
  ret_code = ipa_test_close_log_sio_port(&ipa_test_engine.endp.log_ch[log_ch_idx]);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(ipa_test_engine_sleep_time);

  /*! Deregister CLAT hdl if enabled*/
  if(ipa_test_engine.endp.log_ch[log_ch_idx].is_clat)
  {
     clat_idx = (uint8)ipa_test_engine.endp.log_ch[log_ch_idx].clat_hdl;
     IPA_ASSERT(ipa_clat_deregister(clat_idx) == IPA_SUCCESS);

     /* Increment no of pending dereg cbs*/
     ipa_test_engine.clat.clat_cfg_cb.num_pend_dereg_cb++;

     ipa_test_engine_clat_dereg_end();

     /* set this flag even if one logical channel was CLAT */
     clat_flag = TRUE;
  }

  /* Remove the CLAT static catchall and CLAT static Frag rules CLAT was enabled on any PDN */
  if (clat_flag == TRUE)
  {
    ipa_clat_remove_ul_fltr_global_rules();
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_endp_phy_ch_destroy

==============================================================================*/
/*!
  @brief
  Destroy the physical SIO cfg after the end of the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_endp_phy_ch_destroy
(
  uint8 phy_ch_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  IPA_TF_ASSERT(phy_ch_idx <= IPA_TEST_ENGINE_PHY_CH_NUM_MAX);

  /* Open the physical sio port first */
  ret_code = ipa_test_close_phy_sio_port(&ipa_test_engine.endp.phy_ch[phy_ch_idx]);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  /* Register the SIM per's with the driver next */
  ret_code = ipa_test_deregister_per_sim(&ipa_test_engine.endp.phy_ch[phy_ch_idx]);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_endp_destroy

==============================================================================*/
/*!
  @brief
  Destroy the SIO cfg after the end of the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_endp_destroy(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 i = 0;

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_ENDP_DESTROY);

  for (i = 0; i < ipa_test_engine.endp.curr_phy_ch_cnt; i++)
  {
    ret_code = ipa_test_engine_endp_phy_ch_destroy(i);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Decrement the total number of physical channels */
  ipa_test_engine.endp.curr_phy_ch_cnt = 0;

  /* Now close all the logical channels on this physical channel with the driver */
  for (i = 0; i < ipa_test_engine.endp.curr_log_ch_cnt; i++)
  {
    ret_code = ipa_test_engine_endp_log_ch_destroy(i);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Decrement the total number of logical channels */
  ipa_test_engine.endp.curr_log_ch_cnt = 0;

  #ifndef FEATURE_IPA_USE_IPA_BAM_DRIVER

  /*! Poll on GSI per sim channels*/
  IPA_ASSERT(
         (IPA_GSI_STATUS_SUCCESS == ipa_test_per_sim_pipe_poll()));

  #endif

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_wan_bearer_destroy

==============================================================================*/
/*!
  @brief
  Destroy the bearer cfg after the end of the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_wan_bearer_destroy
(
  uint8 bearer_idx
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  IPA_TF_ASSERT(bearer_idx <= IPA_TEST_ENGINE_BEARER_NUM_MAX);

  ret_code = 
    ipa_test_bearer_destroy(&ipa_test_engine.wan.bearer[bearer_idx]);

  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_wan_destroy

==============================================================================*/
/*!
  @brief
  Destroy the bearer cfg after the end of the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_wan_destroy(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 i = 0;  

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_WAN_DESTROY);

  for (i = 0; i < ipa_test_engine.wan.curr_bearer_cnt; i++)
  {
    ret_code = 
      ipa_test_engine_wan_bearer_destroy(i);
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Decrement the total number of bearers in the system */
  ipa_test_engine.wan.curr_bearer_cnt = 0;

  return ret_code;
}
/*==============================================================================

  FUNCTION:  ipa_test_engine_dpl_destroy

==============================================================================*/
/*!
  @brief
  Destroy the DPL after the end of the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_dpl_destroy(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;

  ipa_test_dpl_destroy(&ipa_test_engine.dpl);

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_ciph_destroy

==============================================================================*/
/*!
  @brief
  Destroy the ciph cfg after the end of the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_ciph_destroy(void)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 i = 0;  

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_CIPH_DESTROY);

  IPA_TF_ASSERT(ipa_test_engine.ciph.init_done == TRUE);

  for (i = 0; i < IPA_TEST_ENGINE_CIPH_PARAMS_NUM_MAX; i++)
  {
    dsm_free_packet(&ipa_test_engine.ciph.ciph_params[i].keystream_dsm_ptr); 
    IPA_TF_ASSERT(ipa_test_engine.ciph.ciph_params[i].keystream_dsm_ptr == NULL); 

    dsm_free_packet(&ipa_test_engine.ciph.ciph_params[i].dummy_data_dsm_ptr); 
    IPA_TF_ASSERT(ipa_test_engine.ciph.ciph_params[i].dummy_data_dsm_ptr == NULL); 
  }

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_destroy

==============================================================================*/
/*!
  @brief
  Destroy the cfg after the end of the test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_destroy(void)
{
  ipa_test_ret_code_e ret_code;
  uint8 i = 0;
  ipa_hwio_def_ipa_log_u log;
  ipa_hwio_def_ipa_endp_init_mode_n_u mode;
  ipa_hwio_def_ipa_endp_status_n_u status;
  ipa_hw_pipe_id_e dl_cnsmr_pipe_num = (ipa_hw_pipe_id_e)
                       IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_DL_CONSUMER_PIPE);
  ipa_hw_pipe_id_e ul_src_pipe_num = (ipa_hw_pipe_id_e)
                       IPA_HW_GET_Q6_PIPE_ID(IPA_HW_Q6_SIM_UL_CONSUMER_PIPE_0);

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_FLTR_DESTROY);

  ipa_ul_reset_num_pkts_rx_to_process();
  
  if(ipa_test_engine.curr_test.cfg.is_hw_replication_enabled == TRUE)
  {
	
    ipa_acc_dpl_configure(IPA_ACC_DPL_STATE_DISABLE);
	/* TBD - use another dpl API */
	if(ipa_test_engine.curr_test.cfg.test_type != IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB)
	{
	  ipa_acc_dpl_remove_hw_rep_pipe(ul_src_pipe_num);
	}
    ipa_acc_dpl_deinit();
  }

  if(IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX != 
     ipa_test_engine.curr_test.cfg.deaggr_err_cfg.err_type)
  {
     for (i = 0; i < ipa_test_engine.ip_pkt.curr_dsm_items; i++) 
     {
       if(ipa_test_engine.ip_pkt.pkt_info_arr[i].dsm_ptr->references != 0)
       {
         dsm_free_packet(&ipa_test_engine.ip_pkt.pkt_info_arr[i].dsm_ptr);
       }
     }
  /* Memset the entire strcture */
  IPA_MEMSET(&ipa_test_engine.ip_pkt, 0, sizeof(ipa_test_engine.ip_pkt));
  }
  //IPA_MEMSET(&ipa_test_engine.ip_pkt, 0, sizeof(ipa_test_engine.ip_pkt));

  if (ipa_test_engine.curr_test.cfg.fltr_cfg.is_filter_enabled == TRUE)
  {
    ret_code = ipa_test_filter_destroy();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
    ipa_sleep(ipa_test_engine_sleep_time);
  }

  if (ipa_test_engine.curr_test.cfg.wan_cfg.is_ciph_enabled == TRUE)
  {
    ret_code = ipa_test_engine_ciph_destroy();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  ret_code = ipa_test_engine_wan_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  if (ipa_test_engine.curr_test.cfg.is_dpl_enabled == TRUE)
  {
    ret_code = ipa_test_engine_dpl_destroy();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
  
  ret_code = ipa_test_engine_endp_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ret_code = ipa_test_engine_destroy_ip_pkts();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ret_code = ipa_test_engine_zip_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_test_engine_store_test_trace();

  ipa_test_engine_store_test_stats();

  /*! Update the current test state */
  ipa_test_engine_store_state_trace(IPA_TEST_ENGINE_STATE_DONE);

  /* Restore ipa_cfg.ctl after test is done*/
  ipa_cfg.ctl = ipa_test_engine_cfg_ctl_copy;

  return ret_code;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_get_if_frag_case

==============================================================================*/
/*!
  @brief
  Whether it is a frag test

  @returns
  None
*/
/*============================================================================*/
boolean ipa_test_engine_get_if_frag_case()
{
  if (ipa_test_engine.curr_test.cfg.ip_pkt_cfg.params.frag_pkt_len != 0) 
  {
    return TRUE;
  }
  else
    return FALSE;
}

/*==============================================================================

  FUNCTION:  ipa_test_engine_generate_clat_cntxt_arr

==============================================================================*/
/*!
  @brief
  Generates random CLAT context.

  @return
  None
*/
/*============================================================================*/
void ipa_test_engine_generate_clat_cntxt_arr
(
  ipa_clat_global_cntxt_info_type       *global_cntxt_info_ptr,
  ipa_test_engine_clat_cntxt_info_type  *cntxt_info_arr_ptr
)
{
  uint8 i, j, prefix_max_idx = 0;

#if 0
  global_cntxt_info_ptr->v4_client_subnet_mask = 
    ipa_test_generate_random_num(0xFFFFFFFF, 0xFFFF);
  global_cntxt_info_ptr->v4_client_subnet_mask &= 0xFFFF0000;
#endif
  /*! Generate the CLAT context for the number of CLAT PDNs*/
  for (i = 0; i < IPA_TEST_ENGINE_LOG_CH_NUM_MAX; i++) 
  {
#if 0
    /*! Generate V4 source address*/
    cntxt_info_arr_ptr[i].v4_src_addr = ipa_test_generate_random_num(0xFFFFFFFF, 0);
    cntxt_info_arr_ptr[i].v4_src_addr &= 0x0000FFFF;
    cntxt_info_arr_ptr[i].v4_src_addr |= global_cntxt_info_ptr->v4_client_subnet_mask;
#endif    
    cntxt_info_arr_ptr[i].v4_src_addr = ipa_ntohl(ipa_ntohl(0x020000C0) + i);

     /*! Generate V4 destination address*/
    cntxt_info_arr_ptr[i].v4_dest_addr = 0x0B0B0404;
      //ipa_test_generate_random_num(0xFFFFFFFF, 0);

    /*! Generate V6 source address*/
    cntxt_info_arr_ptr[i].v6_src_prefix_len = IPA_CLAT_V6_PREFIX_LEN_64;
      //(ipa_clat_prefix_len_e)ipa_test_generate_random_num((uint32)IPA_CLAT_V6_PREFIX_LEN_32, 
      //                                                    (uint32)IPA_CLAT_V6_PREFIX_LEN_64);

    cntxt_info_arr_ptr[i].v6_dest_prefix_len = IPA_CLAT_V6_PREFIX_LEN_64;
      //(ipa_clat_prefix_len_e)ipa_test_generate_random_num((uint32)IPA_CLAT_V6_PREFIX_LEN_32,
      //                                                    (uint32)IPA_CLAT_V6_PREFIX_LEN_64);

    /*! Prefix length max index */
    prefix_max_idx = 
      (IPA_TEST_GET_PREFIX_LEN(cntxt_info_arr_ptr[i].v6_src_prefix_len))/8;

#if 0
    /*! Generate random V6 source prefix address*/
    for (j = 0; j < prefix_max_idx; j++)
    {
      cntxt_info_arr_ptr[i].v6_src_prefix[j] = 
        ipa_test_generate_random_num(0xFF, 0);
    }
#endif
    cntxt_info_arr_ptr[i].v6_src_prefix[0] = 0x08;
    cntxt_info_arr_ptr[i].v6_src_prefix[1] = 0x09;
    cntxt_info_arr_ptr[i].v6_src_prefix[2] = 0x0A;
    cntxt_info_arr_ptr[i].v6_src_prefix[3] = 0x0B;
    cntxt_info_arr_ptr[i].v6_src_prefix[4] = 0x0C;
    cntxt_info_arr_ptr[i].v6_src_prefix[5] = 0x0D;
    cntxt_info_arr_ptr[i].v6_src_prefix[6] = 0x0E;
    cntxt_info_arr_ptr[i].v6_src_prefix[7] = 0x0F;

    /*! Prefix length max index */
    prefix_max_idx = 
      (IPA_TEST_GET_PREFIX_LEN(cntxt_info_arr_ptr[i].v6_dest_prefix_len))/8;

#if 0
    /*! Generate random V6 destination prefix address*/
    for (j = 0; j < prefix_max_idx; j++)
    {
      cntxt_info_arr_ptr[i].v6_dest_prefix[j] = 
        ipa_test_generate_random_num(0xFF, 0);
    }
#endif
    cntxt_info_arr_ptr[i].v6_dest_prefix[0] = 0x08;
    cntxt_info_arr_ptr[i].v6_dest_prefix[1] = 0x09;
    cntxt_info_arr_ptr[i].v6_dest_prefix[2] = 0x0A;
    cntxt_info_arr_ptr[i].v6_dest_prefix[3] = 0x0B;
    cntxt_info_arr_ptr[i].v6_dest_prefix[4] = 0x0C;
    cntxt_info_arr_ptr[i].v6_dest_prefix[5] = 0x0D;
    cntxt_info_arr_ptr[i].v6_dest_prefix[6] = 0x0E;
    cntxt_info_arr_ptr[i].v6_dest_prefix[7] = 0x0F;
  }

  global_cntxt_info_ptr->v4_client_subnet_addr = 0xC0;
  global_cntxt_info_ptr->v4_client_subnet_mask = 0xE0FFFFFF;
}
#pragma optimize("", on)
