/*=========================================================================*/
/*!
  @file
  IPA_test_engine_ul_p0.cpp

  @brief
  UL p0 tests for STRIDE.

  @details

  Copyright (c) 2017 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary
*/
/*=========================================================================*/

/*===========================================================================
EDIT HISTORY FOR MODULE

Please notice that the changes are listed in reverse chronological order.

$Header: //components/rel/dataipa.mpss/3.2/ipa/ipa_stride/IPA_test_engine_ul_p0.cpp#1 $
$DateTime: 2019/07/05 03:48:36 $
$Author: pwbldsvc $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
05/30/17   db      CR13276332: Final submit for Napali BU.
===========================================================================*/

/*==========================================================================

                            INCLUDE FILES FOR MODULE

===========================================================================*/
#include <comdef.h>
#include "IPA_test_engine.h"
#include "IPA_test.h"
#include "ipa_test_filter.h"
#include "ipa_zip_ro_vectors.h"
#include "ipa_zip_rw_vectors.h"
extern "C"
{
  #include "gsi_test_common.h"
}
#include <ipa_ipfltri.h>
#include <sio.h>

#include "IPA_test_engine_ul_p0.h"

/*==============================================================================
 
                             MACROS
 
==============================================================================*/
/*!@brief Number of times each cfg'ed test will be run */
#define IPA_TEST_ENGINE_P0_TEST_MAX_RUN 1

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/
extern ipa_test_engine_s ipa_test_engine;

/*==============================================================================

                         INTERNAL VARIABLES

==============================================================================*/
ipa_test_engine_params_s engine_params;


void ipa_test_engine_ul_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_ul_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}
 
/****************** Test case ipa_test_engine_ul_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/ 

void ipa_test_engine_ul_p0::test0001()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 10;
  engine_params.ip_pkt_cfg.num_dsm_items= 1;
  engine_params.ip_pkt_cfg.payload_len = 1000;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_ICMP;
  engine_params.wan_cfg.num_bearers = 0;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0002()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 10;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0003()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items= 1;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0004()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  /* HACK */
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items= 1;
  engine_params.ip_pkt_cfg.payload_len = 200;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0005()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  /* HACK */
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

 /****************** Test case ipa_test_engine_ul_p0_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0006()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

  /****************** Test case ipa_test_engine_ul_p0_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0007()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 100;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0008()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0009()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0010()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0011 ***********/
/* 
Test case number : 0011
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0011()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_DIRECT_DMA;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0012 ***********/
/* 
Test case number : 0012
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0012()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_DIRECT_DMA;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_ul_p0_0013 ***********/
/* 
Test case number : 0013
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0013()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_DIRECT_DMA;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 10;
  engine_params.ip_pkt_cfg.num_dsm_items= 1;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_ul_p0_0014 ***********/
/* 
Test case number : 0014
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0014()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_DIRECT_DMA;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_ul_p0_0015 ***********/
/* 
Test case number : 0015
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0015()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_DIRECT_DMA;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_ul_p0_0016 ***********/
/* 
Test case number : 0016
Test Component : IPA Test Engine P0 test

Test description : Remove UL Routing rules back to back without committing
them. This tests that num_qos_rules, num_v6_prefix_rules counts do not get
corrupted.

    Open 1 SIO stream.
    Allocate 4 QoS/V6_Prefix routing rules associated with the same SIO.
    commit first 2 rules.
    remove last 2 of them without commiting them.
    Send a non-QoS packet to match the mux_id rule.
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0016()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;  
    
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.num_dsm_items = 0;

  /*! IPV4 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_ipfltr_rule_handle_t handle[4] = {NULL};
	
  handle[0] = ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_QOS, 
                                            IPA_IPFLTR_IPV4, 
                                            FALSE, 0x12, 
                                            ipa_test_engine.endp.log_ch[0].stream_id);

  /*! Add the !mux_id based metadata eq */
  ipa_ipfltri_add_metadata_meq_32(
     handle[0], 
     ipa_ntohl(IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_MASK), 
     ipa_ntohl(IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_MASK & \
               (~ipa_test_engine.endp.log_ch[0].mux_id << IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_SHIFT) ) );

  handle[1] = ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_QOS, 
                                            IPA_IPFLTR_IPV4, 
                                            FALSE, 0x13, 
                                            ipa_test_engine.endp.log_ch[0].stream_id);
  /*! Add the !mux_id based metadata eq */
  ipa_ipfltri_add_metadata_meq_32(
     handle[1], 
     ipa_ntohl(IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_MASK), 
     ipa_ntohl(IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_MASK & \
               (~ipa_test_engine.endp.log_ch[0].mux_id << IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_SHIFT) ) );

  handle[2] = ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_QOS,
                                            IPA_IPFLTR_IPV4, 
                                            FALSE, 0x14,
                                            ipa_test_engine.endp.log_ch[0].stream_id);
  handle[3] = ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_QOS,
                                            IPA_IPFLTR_IPV4, 
                                            FALSE, 0x15,
                                            ipa_test_engine.endp.log_ch[0].stream_id);

  /* Commit first 2 QoS rules */
  ipa_ipfltri_commit_rules(2, handle);

  ipa_sleep(20);

  /* Remove Rule 2 , 3 */
  ipa_ipfltri_remove_rule(handle[2]);
  ipa_ipfltri_remove_rule(handle[3]);

  ipa_sleep(20);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Remove Rule 0 , 1 */
  ipa_ipfltri_remove_rule(handle[0]);
  ipa_ipfltri_remove_rule(handle[1]);
}  

/****************** Test case ipa_test_engine_ul_p0_0017 ***********/
/* 
Test case number : 0017
Test Component : IPA Test Engine P0 test

Test description : Remove UL Routing rules even before tag from 
h/w is received. This tests that num_qos_rules, num_v6_prefix_rules
counts do not get corrupted.

    Open 1 SIO stream.
    Allocate 4 QoS/v6_prefix routing rules associated with the same SIO. 
    commit first 2 rules together.
    commit last 2 rules together.
    remove last 2 rules quickly right after.
    Send a non-QoS packet to match the mux_id rule.
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0017()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;  
    
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.num_dsm_items = 0;

  /*! IPV4 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_ipfltr_rule_handle_t handle[4] = {NULL};

  handle[0] = ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_QOS, 
                                            IPA_IPFLTR_IPV4, 
                                            FALSE, 0x12, 
                                            ipa_test_engine.endp.log_ch[0].stream_id);

  /*! Add the !mux_id based metadata eq */
  ipa_ipfltri_add_metadata_meq_32(
     handle[0], 
     ipa_ntohl(IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_MASK), 
     ipa_ntohl(IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_MASK & \
               (~ipa_test_engine.endp.log_ch[0].mux_id << IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_SHIFT) ) );

  handle[1] = ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_QOS,
                                            IPA_IPFLTR_IPV4,
                                            FALSE, 0x13,
                                            ipa_test_engine.endp.log_ch[0].stream_id);

  /*! Add the !mux_id based metadata eq */
  ipa_ipfltri_add_metadata_meq_32(
     handle[1], 
     ipa_ntohl(IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_MASK), 
     ipa_ntohl(IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_MASK & \
               (~ipa_test_engine.endp.log_ch[0].mux_id << IPA_IPFLTR_RTNG_UL_DEF_MUX_POS_SHIFT) ) );

  handle[2] = ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_QOS, 
                                            IPA_IPFLTR_IPV4, 
                                            FALSE, 0x14, 
                                            ipa_test_engine.endp.log_ch[0].stream_id);
  handle[3] = ipa_ipfltri_alloc_rule_handle(IPA_IPFLTR_RULE_UL_QOS, 
                                            IPA_IPFLTR_IPV4, 
                                            FALSE, 0x15, 
                                            ipa_test_engine.endp.log_ch[0].stream_id);

  /* Commit first 2 QoS rules */
  ipa_ipfltri_commit_rules(2, handle);
  ipa_sleep(20);

  /* Commit last 2 rules */
  ipa_ipfltri_commit_rules(2, &handle[2]);

  /* Remove them quickly */
  ipa_ipfltri_remove_rule(handle[2]);
  ipa_ipfltri_remove_rule(handle[3]);

  ipa_sleep(20);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }

  /* Remove Rule 0 , 1 */
  ipa_ipfltri_remove_rule(handle[0]);
  ipa_ipfltri_remove_rule(handle[1]);
}

/****************** Test case ipa_test_engine_ul_p0_0018 ***********/
/* 
Test case number : 0018
Test Component : IPA Test Engine P0 test

Test description : Add and Remove UL PDN default rules back to back.
This will test if the local cleanup of UL PDN default rules along with 
destination attributes entries is happening correctly.

    Open 2 SIO streams back to back and then quickly close them.
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0018()
{
  ipa_test_ret_code_e ret_code;
  uint8 idx;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;  
    
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.num_dsm_items = 0;

  /*! IPV4 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_ipfltr_rule_handle_t handle[2] = {NULL};
  
  idx = ipa_sio.stream_id_to_index_map[ ipa_test_engine.endp.log_ch[0].stream_id ] ;
  
  handle[0] = ipa_ipfltri_rtng_add_ul_pdn_default_rule(idx);
  handle[1] = ipa_ipfltri_rtng_add_ul_pdn_default_rule(idx);
  
  ipa_ipfltri_remove_rule(handle[1]);
  ipa_ipfltri_remove_rule(handle[0]);

  ipa_sleep(20);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0019 ***********/
/* 
Test case number : 0019
Test Component : IPA Test Engine P0 test

Test description : Add and Remove UL V6 Prefix rules back to back.
This will test if the local cleanup of UL PDN default rules along with 
destination attributes entries is happening correctly.

Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0019()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;  
    
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.num_dsm_items = 0;

  /*! IPV4 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_ipfltr_rule_handle_t handle[2] = {NULL};
  uint8 prefix[16] = {0xFF};

  handle[0] = ipa_ipfltri_add_ul_ipv6_prefix_rule(ipa_test_engine.endp.log_ch[0].stream_id,
                                                  8,
                                                  prefix,
                                                  0xFF);
  handle[1] = ipa_ipfltri_add_ul_ipv6_prefix_rule(ipa_test_engine.endp.log_ch[0].stream_id,
                                                  8,
                                                  prefix,
                                                  0xFF);
  ipa_ipfltri_remove_rule(handle[1]);
  ipa_ipfltri_remove_rule(handle[0]);

  ipa_sleep(20);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_p0_0020 ***********/
/* 
Test case number : 0020
Test Component : IPA Test Engine P0 test with QMAP SW deaggregation
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_p0::test0020()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.ic_type = IPA_SIO_PER_EP_IC_TYPE_PCIE;

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items= 1;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_clat_ul_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_clat_ul_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_clat_ul_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description : UL CLAT Test. Static CLAT configuration. 
                   Send an IPv4 packet. 
 
Pass Criteria: Expect translated IPv6 packet with Frag Extension header.

*/

void ipa_test_engine_clat_ul_p0::test0001()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_clat_enabled = TRUE;

  /*! Explicitly set ciphering disabled */
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 

/****************** Test case ipa_test_engine_clat_ul_p0_0002 ***********/
/* 
Test case number : 0002

Test Component : IPA Test Engine P0 test

Test description : UL CLAT Test. Random CLAT configuration. 
                   Send an IPv4 packet. DF bit is randomly set or cleared.
Pass Criteria: If DF bit is set expect translated IPv6 packet with Frag Extension header.
               If DF bit is not set expect translated IPv6 packet
               without Frag Extension header.          
*/

void ipa_test_engine_clat_ul_p0::test0002()
{
  ipa_test_ret_code_e ret_code;

  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_clat_enabled = TRUE;

  /*! Explicitly set ciphering disabled */
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 

/****************** Test case ipa_test_engine_clat_ul_p0_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine P0 test
Test description : HDR_TYPE None, Test-Filter config Disabled. 
                  Send mixture of 20 IPV4 and IPV6  pkts.
                  IPV6 packets with/without extension header. 
 
Pass Criteria: Receive 20 pkts in right order.

*/

void ipa_test_engine_clat_ul_p0::test0003()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 

/****************** Test case ipa_test_engine_clat_ul_p0_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine CLAT UL P0 test
Test description : IPA_SIO_HDR_TYPE_QMAP, 
                   Test-Filter config Disabled. 
                   Send 20 mixture of IPv4/IPV6 pkts.
 
Pass Criteria: Receive 20 pkts in right order.

*/

void ipa_test_engine_clat_ul_p0::test0004()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 
  
/****************** Test case ipa_test_engine_clat_ul_p0_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine CLAT UL P0 test
Test description : HDR_TYPE QMAP_V3, Test-Filter config Disabled. 
Send 20 IPv4/IPV6 pkts
 
Pass Criteria: Receive 20 pkts in right order.
*/

void ipa_test_engine_clat_ul_p0::test0005()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V4 ;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 
 
/****************** Test case ipa_test_engine_clat_ul_p0_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine P0 test
Test description : UL CLAT Test. Static CLAT configuration. 
				Send an IPv4 packet. 

Pass Criteria: Expect translated IPv6 packet with Frag Extension header.

*/

void ipa_test_engine_clat_ul_p0::test0006()
{
  ipa_test_ret_code_e ret_code;
 
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
   IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
   .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
   .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
   IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  
  engine_params.is_clat_enabled = TRUE;
  
  /*! Explicitly set ciphering disabled */
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_ul_p0_0007 ***********/
/* 
Test case number : 0007

Test Component : IPA Test Engine P0 test

Test description : UL CLAT Test. Random CLAT configuration. 
                   Send an IPv4 packet. DF bit is randomly set or cleared.
Pass Criteria: If DF bit is set expect translated IPv6 packet with Frag Extension header.
               If DF bit is not set expect translated IPv6 packet
               without Frag Extension header.          
*/

void ipa_test_engine_clat_ul_p0::test0007()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_clat_enabled = TRUE;

  /*! Explicitly set ciphering disabled */
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_ul_p0_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine P0 test
Test description : HDR_TYPE None, Test-Filter config Disabled. 
                  Send mixture of 20 IPV4 and IPV6  pkts.
                  IPV6 packets with/without extension header. 
 
Pass Criteria: Receive 20 pkts in right order.

*/

void ipa_test_engine_clat_ul_p0::test0008()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_ul_p0_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine CLAT UL P0 test
Test description : IPA_SIO_HDR_TYPE_QMAP, 
                   Test-Filter config Disabled. 
                   Send 20 mixture of IPv4/IPV6 pkts.
 
Pass Criteria: Receive 20 pkts in right order.

*/

void ipa_test_engine_clat_ul_p0::test0009()
{
  ipa_test_ret_code_e ret_code;
  
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_ul_p0_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine CLAT UL P0 test
Test description : HDR_TYPE QMAP_V3, Test-Filter config Disabled. 
Send 20 IPv4/IPV6 pkts
 
Pass Criteria: Receive 20 pkts in right order.
*/

void ipa_test_engine_clat_ul_p0::test0010()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V4 ;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  //engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;
  //engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = 0;
  
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_clat_ul_p1::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_clat_ul_p1::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_clat_ul_p1_0001 ***********/
/* 
Test case number : 0001

Test Component : IPA Test Engine P0 test

Test description : 2 CLAT PDNs, MIXED IPV4/IPV6 packet 
                   HDR Type = QMAP
 
Pass Criteria: 
*/

void ipa_test_engine_clat_ul_p1::test0001()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 2;
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_clat_enabled = TRUE;

  /*! Explicitly set ciphering disabled */
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 
  
/****************** Test case ipa_test_engine_clat_ul_p1_0002 ***********/
/* 
Test case number : 0002

Test Component : IPA Test Engine P0 test

Test description : 2 CLAT PDNs, MIXED IPV4/IPV6 packet 
                   HDR Type = QMAP_V3
Pass Criteria: 
*/

void ipa_test_engine_clat_ul_p1::test0002()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 2;
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V4;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V4;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;
  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_clat_enabled = TRUE;

  /*! Explicitly set ciphering disabled */
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 
  
/****************** Test case ipa_test_engine_clat_ul_p1_0003 ***********/
/* 
Test case number : 0003

Test Component : IPA Test Engine P0 test

Test description : 1 CLAT and 1 non CLAT PDN , MIXED IPV4/IPV6 packets
                   HDR Type = QMAP
 
Pass Criteria: 

*/

void ipa_test_engine_clat_ul_p1::test0003()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
     IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_clat_enabled = TRUE;

  /*! Explicitly set ciphering disabled */
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 
  
/****************** Test case ipa_test_engine_clat_ul_p1_0004 ***********/
/* 
Test case number : 0004

Test Component : IPA Test Engine P0 test

Test description : 1 CLAT and 1 non CLAT PDN , MIXED IPV4/IPV6 packets
                   HDR Type = QMAP_V3
 
Pass Criteria: 

*/
 
 void ipa_test_engine_clat_ul_p1::test0004()
{
  ipa_test_ret_code_e ret_code;
  
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 2;

  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V4;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V4;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_clat_enabled = TRUE;

  /*! Explicitly set ciphering disabled */
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
 
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
	ipa_sleep(20);
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
} 

void ipa_test_engine_clat_ul_fltr::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_clat_ul_fltr::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_clat_ul_fltr_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter : ANY ICMP
                   HDR TYPE: QMAP, 1 CLAT PDN
 
Pass Criteria: 

*/

void ipa_test_engine_clat_ul_fltr::test0001()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_CLAT_ANY_ICMP_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_clat_ul_fltr_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine CLAT static fltr test
Test description : UL Static Filter : UDP CHKSUM ZERO
 
Pass Criteria: 

*/

void ipa_test_engine_clat_ul_fltr::test0002()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_CLAT_UDP_CHKSUM_ZERO_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
   
/****************** Test case ipa_test_engine_clat_ul_fltr_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine CLAT static fltr test
Test description : UL Static Filter : TTL = 1

Pass Criteria: 

*/

void ipa_test_engine_clat_ul_fltr::test0003()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_CLAT_TTL_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_clat_ul_fltr_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine CLAT static fltr test
Test description : UL Static Filter : DNS Proxy
 
Pass Criteria: 

*/

void ipa_test_engine_clat_ul_fltr::test0004()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_CLAT_DNS_PROXY_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(1000);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
   
/****************** Test case ipa_test_engine_clat_ul_fltr_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine CLAT static fltr test
Test description : UL Static Filter : Frag v4
 
Pass Criteria: 

*/

void ipa_test_engine_clat_ul_fltr::test0005()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 500;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_CLAT_FRAG_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(1000);  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_ul_fltr::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_ul_fltr::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_ul_fltr_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter : DHCP IPV4
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0001()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10); 
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_ul_fltr_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter : DHCP IPV6
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0002()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;

  /*! IPV6 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_DHCP_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
   
/****************** Test case ipa_test_engine_ul_fltr_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine fltr test
Test description : UL TCP ACK V4 prioritization test. 
                   Random Protocol data pkts(ICMP,UDP,TCP) + TCP ACK pkts.
                   In case of ICMP: ICMPv4 Req pkts are generated and prioritized.
                   In case of UDP :IPv4 DHCP pkts are generated and prioritized.
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0003()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_ul_fltr_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine fltr test
Test description : UL TCP ACK V4 prioritization test. 
                   TCP Protocol data pkts + TCP ACK pkt
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0004()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_ul_fltr_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine fltr test
Test description : UL TCP ACK V6 prioritization test. 
                   Random Protocol data pkts(ICMP,UDP,TCP) + TCP ACK pkts.
                   In case of ICMP: ICMPv6 Req pkts are generated and prioritized.
                   In case of UDP :IPv6 DHCP pkts are generated and prioritized.                    
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0005()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_ul_fltr_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine fltr test
Test description : UL TCP ACK V6 prioritization test. 
                   V6 TCP Protocol data pkts + TCP ACK pkt
Pass Criteria: 
*/

void ipa_test_engine_ul_fltr::test0006()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TCP_ACK_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
    
/****************** Test case ipa_test_engine_ul_fltr_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine fltr test
Test description : UL Fragmentation test. 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0007()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V4;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  
  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 500;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_FRAG_V4;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
   
/****************** Test case ipa_test_engine_ul_fltr_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter : ICMP V4 REQUEST
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0008()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REQ_V6;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_ICMP;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REQ_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
    
/****************** Test case ipa_test_engine_ul_fltr_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter : ICMP REQUEST V6
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0009()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REQ_V6;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  /*! IPV6 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_ICMP;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REQ_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
    
/****************** Test case ipa_test_engine_ul_fltr_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter : ICMP REPLY IPV4
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0010()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;

  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_ICMP;
  
  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
   
/****************** Test case ipa_test_engine_ul_fltr_0011 ***********/
/* 
Test case number : 0011
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter : ICMP REPLY IPV6
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0011()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REP_V6;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;

  /*! IPV6 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_ICMP;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_REP_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
    
/****************** Test case ipa_test_engine_ul_fltr_0012 ***********/
/* 
Test case number : 0012
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter : ICMP NDP IPV6
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0012()
{
  ipa_test_ret_code_e ret_code;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6;  
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 1;

  /*! IPV6 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_ICMP;

  engine_params.is_dpl_enabled =  FALSE; 

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_ICMP_NDP_V6;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_ul_fltr_0013 ***********/
/* 
Test case number : 0013
Test Component : IPA Test Engine fltr test
Test description : UL Static Filter random test
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0013()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap.
    max_aggr_pkts = 4;

  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  /*! IPV4 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.wan_cfg.num_bearers = 0;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_LOCAL;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
    
/****************** Test case ipa_test_engine_ul_fltr_0014 ***********/
/* 
Test case number : 0014
Test Component : IPA Test Engine Fltr test
Test description : UL Static Filter : QOS Random FILTERS IPV4
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0014()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 10;
  engine_params.ip_pkt_cfg.payload_len = 1000;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  /*! IPV4 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.wan_cfg.num_bearers = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_QOS;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
  engine_params.fltr_cfg.num_ul_qos_fltr = IPA_TEST_FILTER_MAX_QOS_V6_RULES + 1;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_ul_fltr_0015 ***********/
/* 
Test case number : 0015
Test Component : IPA Test Engine Fltr test
Test description : UL Static Filter : QOS FILTERS IPV6
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0015()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 10;
  engine_params.ip_pkt_cfg.payload_len = 1000;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  /*! IPV6 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;

  engine_params.wan_cfg.num_bearers = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_QOS;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
  engine_params.fltr_cfg.num_ul_qos_fltr = IPA_TEST_FILTER_MAX_QOS_V6_RULES + 1;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
    
/****************** Test case ipa_test_engine_ul_fltr_0016 ***********/
/* 
Test case number : 0016
Test Component : IPA Test Engine Fltr test

Test description : Back to back removal of UL IPv6 Prefix rule. 
    2 SIO's with 1 IPv6 Prefix rule each. 
    Open SIO 1. Add IPv6 Prefix Rule 1 
    Open SIO 2. Add IPv6 Prefix Rule 2
    Remove IPv6 Prefix Rule 1. Close SIO 1
    Remove IPv6 Prefix Rule 2. Close SIO 2

Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0016()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;  
    
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 2; 

  engine_params.ip_pkt_cfg.num_pkts = 0;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.num_dsm_items = 0;

  /*! IPV6 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_QOS;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  uint8 prefix[16] = {0};
  ipa_ipfltr_rule_handle_t handle[2] = {NULL, NULL};

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

    handle[0] = ipa_ipfltri_add_ul_ipv6_prefix_rule(ipa_test_engine.endp.log_ch[0].stream_id,
                                       64, prefix, 0xFF);
    handle[1] = ipa_ipfltri_add_ul_ipv6_prefix_rule(ipa_test_engine.endp.log_ch[1].stream_id,
                                       64, prefix, 0xFF);

    ipa_sleep(20);

    /* Remove IPv6 prefix Rule 1 */
    ipa_ipfltri_remove_rule(handle[0]);

    /* close sio port 1 */
    sio_close(ipa_test_engine.endp.log_ch[0].stream_id, NULL);

    /* Remove IPv6 prefix Rule 2 */
    ipa_ipfltri_remove_rule(handle[1]);

    /* close sio port 2 */
    sio_close(ipa_test_engine.endp.log_ch[1].stream_id, NULL);

    engine_params.endp_cfg.num_log_ch[0] = 0;
  }
}
   
/****************** Test case ipa_test_engine_ul_fltr_0017 ***********/
/* 
Test case number : 0017
Test Component : IPA Test Engine Fltr test
Test description : UL TCP ACK + QOS V4 test 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0017()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  /*! IPV4 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.wan_cfg.num_bearers = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_MAX;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
  //engine_params.fltr_cfg.num_ul_qos_fltr = IPA_TEST_FILTER_MAX_QOS_V6_RULES + 1;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
    
/****************** Test case ipa_test_engine_ul_fltr_0018 ***********/
/* 
Test case number : 0018
Test Component : IPA Test Engine Fltr test
Test description : UL TCP ACK + QOS V6 test 
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0018()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 10;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  /*! IPV6 Packet*/
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;

  engine_params.wan_cfg.num_bearers = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_MAX;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
    
/****************** Test case ipa_test_engine_ul_fltr_0019 ***********/
/* 
Test case number : 0019
Test Component : IPA Test Engine Fltr test
Test description : UL Static + QOS fltr test
 
Pass Criteria: 

*/

void ipa_test_engine_ul_fltr::test0019()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 15;
  engine_params.ip_pkt_cfg.payload_len = 1000;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;

  engine_params.wan_cfg.num_bearers = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_STATIC_AND_QOS;
  engine_params.fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
  engine_params.fltr_cfg.num_ul_qos_fltr = IPA_TEST_FILTER_MAX_QOS_V6_RULES + 1;
  engine_params.fltr_cfg.num_dl_sckt_fltr = 0;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(10);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_dl_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_dl_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_dl_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0001()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
 /****************** Test case ipa_test_engine_dl_p0_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0002()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_dsm_items = 2;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0003()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[1] = FALSE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_dl_p0_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0004()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 0;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0005()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 5;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[1] = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[2] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[3] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[4] = FALSE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0006()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 5;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[2] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[3] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[4] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0007()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 4;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[2] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[3] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0008()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 4;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.bearer_tech[1] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[2] = FALSE;
  engine_params.wan_cfg.bearer_tech[2] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[3] = TRUE;
  engine_params.wan_cfg.bearer_tech[3] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_dl_p0_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0009()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 10;

  engine_params.wan_cfg.num_bearers = 4;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[2] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[3] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_dl_p0_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0010()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 5;

  engine_params.wan_cfg.num_bearers = 3;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[2] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_dl_p0_0011 ***********/
/* 
Test case number : 0011
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0011()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 2;
  
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

    engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
 engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 4;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[2] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[3] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0012 ***********/
/* 
Test case number : 0012
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0012()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
   IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 1536;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim.ncm_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim.ndp_sig = 0xFF;

  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 4;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[2] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[3] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0013 ***********/
/* 
Test case number : 0013
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0013()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0014 ***********/
/* 
Test case number : 0014
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0014()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0015 ***********/
/* 
Test case number : 0015
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0015()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.num_dsm_items = 2;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0016 ***********/
/* 
Test case number : 0016
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0016()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.num_dsm_items = 2;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_dl_p0_0017 ***********/
/* 
Test case number : 0017
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0017()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.payload_len = IPA_TEST_ENGINE_IP_PKT_SIZE_MAX - 30;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.num_dsm_items = 
    IPA_TEST_ENGINE_DSM_CHAIN_NUM_MAX;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0018 ***********/
/* 
Test case number : 0018
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0018()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 5;

  engine_params.wan_cfg.num_bearers = 3;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[2] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0019 ***********/
/* 
Test case number : 0019
Test Component : IPA Test Engine P0 test
Test description : DPL Test Case with 1 bearer and 1 packet
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0019()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  engine_params.is_dpl_enabled =  TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0020 ***********/
/* 
Test case number : 0020
Test Component : IPA Test Engine P0 test
Test description : DPL Test Case with 2 Bridged bearers, 
                    2 Unciphered packets
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0020()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  engine_params.is_dpl_enabled =  TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0021 ***********/
/* 
Test case number : 0021
Test Component : IPA Test Engine P0 test
Test description : DPL Test Case with 1 Bridged, 1 unbridged bearer, 
                    2 Ciphered packets 
                   
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0021()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[1] = FALSE;

  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  engine_params.is_dpl_enabled =  TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0022 ***********/
/* 
Test case number : 0022
Test Component : IPA Test Engine P0 test
Test description : DPL Test Case with 2 bearers, 2 Ciphered packets, 
                   Filters enabled 
                   
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0022()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = IPA_TEST_FILTER_MAX_DL_V6_RULES + 1;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  engine_params.is_dpl_enabled =  TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_p0_0023 ***********/
/* 
Test case number : 0023
Test Component : IPA Test Engine P0 test
Test description : DPL Test Case with 1 Bridged, 1 unbridged bearer, 
                    2 Ciphered packets 
                   
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0023()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.num_dsm_items = 5;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[1] = FALSE;

  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  engine_params.is_dpl_enabled =  TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0024 ***********/
/* 
Test case number : 0024
Test Component : IPA Test Engine P0 test
Test description : DPL Test Case with 1 Bridged, 1 unbridged bearer, 
                    2 Ciphered packets,40 DSM ITEMS 
                   
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0024()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 40;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  engine_params.is_dpl_enabled =  TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
 
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_dl_p0_0025 ***********/
/* 
Test case number : 0025
Test Component : IPA Test Engine P0 test
Test description : DPL Test Case with 1 Bridged, 1 unbridged bearer, 
                    2 Ciphered packets, 45 DSM ITMES
                   
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0025()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 45;
  engine_params.ip_pkt_cfg.payload_len = 1200;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  engine_params.is_dpl_enabled =  TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0026 ***********/
/* 
Test case number : 0026
Test Component : IPA Test Engine P0 test
Test description : DPL Test Case with 1 Bridged, 1 unbridged bearer, 
                    2 Ciphered packets
                    39 DSM items
                   
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0026()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.num_dsm_items = 39;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[1] = FALSE;

  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  engine_params.is_dpl_enabled =  TRUE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0027 ***********/
/* 
Test case number : 0027
Test Component : IPA Test Engine P0 test
Test description : Bearer Suspend Scenario 1 : The bearer is 
                   supended after sending half of the packets.And then
                   activated after all the packets have been send.
 
Pass Criteria: 
We should be receiveing all the packets back. 

*/

void ipa_test_engine_dl_p0::test0027()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_susp_case_1 =  TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_p0_0028 ***********/
/* 
Test case number : 0028
Test Component : IPA Test Engine P0 test
Test description : Bearer deregister scenario: 
                   The bearer is deregistered after sending a few
                   packets. 
 
Pass Criteria: 
We should be receiving only the packets send before deregistering 
the bearer.

*/

void ipa_test_engine_dl_p0::test0028()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_dereg_case =  TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_dl_p0_0029 ***********/
/* 
Test case number : 0029
Test Component : IPA Test Engine P0 test
Test description : Bearer Suspend Scenario 2: 
                 The bearer is supended before sending all the packets. 
 
Pass Criteria: 
Receive all the packets that are send. 

*/

void ipa_test_engine_dl_p0::test0029()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 41;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = FALSE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_susp_case_2 =  TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0030 ***********/
/* 
Test case number : 0030
Test Component : IPA Test Engine P0 test
Test description : Frgamentation test case 1 
In-order fragments, 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0030()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 200;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_p0_0031 ***********/
/* 
Test case number : 0031
Test Component : IPA Test Engine P0 test
Test description : Frgamentation test case 1 
In-order fragments, DPL logging enabled
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0031()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 200;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0032 ***********/
/* 
Test case number : 0032
Test Component : IPA Test Engine P0 test
Test description : Frgamentation test case 1 
In-order fragments, filter enabled 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0032()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 200;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = IPA_TEST_FILTER_MAX_DL_V6_RULES + 1;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0033 ***********/
/* 
Test case number : 0033
Test Component : IPA Test Engine P0 test
Test description : Frgamentation test case 4 
QMAP header + in-order fragments
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0033()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 200;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0034 ***********/
/* 
Test case number : 0034
Test Component : IPA Test Engine P0 test
Test description : Frgamentation test case 5 
OOO fragments
 
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0034()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 200;
  engine_params.ip_pkt_cfg.params.ooo_frag_case =  TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0035 ***********/
/* 
Test case number : 0035
Test Component : IPA Test Engine P0 test
Test description : Frgamentation test case 6
 OOO fragments, filter enabled
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0035()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 200;
  engine_params.ip_pkt_cfg.params.ooo_frag_case =  TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = IPA_TEST_FILTER_MAX_DL_V6_RULES + 1;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0036 ***********/
/* 
Test case number : 0036
Test Component : IPA Test Engine P0 test
Test description : Frgamentation test case 6
OOO fragments + bearer suspend test case
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0036()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 200;
  engine_params.ip_pkt_cfg.params.ooo_frag_case =  TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.wan_cfg.is_bearer_susp_case_1 = TRUE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
  
/****************** Test case ipa_test_engine_dl_p0_0037 ***********/
/* 
Test case number : 0037
Test Component : IPA Test Engine P0 test
Test description : Frgamentation test case 6
In order fragments + bearer suspend test case
Pass Criteria: 

*/

void ipa_test_engine_dl_p0::test0037()
{
  ipa_test_ret_code_e ret_code;
  
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 2000;
  engine_params.ip_pkt_cfg.params.frag_pkt_len = 200;
  engine_params.ip_pkt_cfg.params.ooo_frag_case =  FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.wan_cfg.is_bearer_susp_case_1 = TRUE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_dl_fltr::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_dl_fltr::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_dl_fltr_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine DL Filter test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_fltr::test0001()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 1000;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.num_bearers = 1;
  
  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = IPA_TEST_FILTER_MAX_DL_V6_RULES + 1;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(15);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_fltr_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine DL Filter test
Test description : 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_fltr::test0002()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 1000;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.num_bearers = 1;
  
  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = IPA_TEST_FILTER_MAX_DL_V6_RULES + 1;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
 
  ipa_sleep(15);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
 
/****************** Test case ipa_test_engine_dl_fltr_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine DL Filter test
Test description : This test configures 8 PDNs to check DL routing 
                   table overflow .
                   Sends 20 packets on a bearer and checks if the
                   configuration with 
 
Pass Criteria: 

*/

void ipa_test_engine_dl_fltr::test0003()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
   engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present \ 
  	= FALSE;  
  engine_params.endp_cfg.num_log_ch[0] = 8; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 1000;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.num_bearers = 1;
  
  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = TRUE;
  engine_params.fltr_cfg.num_ul_qos_fltr = 0;
  engine_params.fltr_cfg.num_dl_sckt_fltr = IPA_TEST_FILTER_MAX_DL_V6_RULES + 1;
  engine_params.fltr_cfg.is_tcp_ack_qos_rule = FALSE;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  ipa_sleep(15);
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_clat_dl_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_clat_dl_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_clat_dl_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description : Static CLAT config, IPV6 Packet, without extension header,
                   HDR Type = NONE
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p0::test0001()
{
  ipa_test_ret_code_e ret_code;
  
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
   engine_params.clat_cfg.global_clat_cfg.v4_client_subnet_mask = 0xFFFFFFFF;
  /*! CLAT context for the first PDN */
  engine_params.clat_cfg.cntxt_info[0].v4_src_addr = 0xAFCA00;
  engine_params.clat_cfg.cntxt_info[0].v4_dest_addr = 0x0B0B0404;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix_len = IPA_CLAT_V6_PREFIX_LEN_32;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix_len = IPA_CLAT_V6_PREFIX_LEN_32;

  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[0] = 0x08;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[1] = 0x09;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[2] = 0x0A;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[3] = 0x0B;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[4] = 0x0C;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[5] = 0x0D;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[6] = 0x0E;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[7] = 0x0F;

  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[0] = 0x08;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[1] = 0x09;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[2] = 0x0A;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[3] = 0x0B;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[4] = 0x0C;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[5] = 0x0D;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[6] = 0x0E;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[7] = 0x0F;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine P0 test
Test description : Random CLAT config, IPV6 Packet without extension header,
                   HDR type = None
 
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p0::test0002()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 1;
   engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0003 ***********/
/* 
Test Component : IPA Test Engine CLAT DL P0 test
Test description : Random CLAT config. 20 IPv6 with and without xtension headers.

Pass Criteria: CLAT translated IPv4 pkts are received back and they are not 
               supposed to have bytes from extension headers in the payload.

*/

void ipa_test_engine_clat_dl_p0::test0003()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled = FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0004 ***********/
/* 
Test Component : IPA Test Engine CLAT DL P0 test
Test description : Random CLAT config. 20 IPV4 and IPV6 pkt.
                   IPV6 packets with and without extension headers.
                   HDR TYPE = QMAP     

Pass Criteria: 
*/

void ipa_test_engine_clat_dl_p0::test0004()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled = FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine P0 test
Test description : Random CLAT config. 
                   20 IPV4 and IPV6 Packets with/without extension header.
                   HDR type = QMAP V3
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p0::test0005()
{
  ipa_test_ret_code_e ret_code;
  
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine P0 test
Test description : Static CLAT config, IPV6 Packet, without extension header,
                   HDR Type = NONE
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p0::test0006()
{
  ipa_test_ret_code_e ret_code;
  
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
   engine_params.clat_cfg.global_clat_cfg.v4_client_subnet_mask = 0xFFFFFFFF;
  /*! CLAT context for the first PDN */
  engine_params.clat_cfg.cntxt_info[0].v4_src_addr = 0xAFCA00;
  engine_params.clat_cfg.cntxt_info[0].v4_dest_addr = 0x0B0B0404;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix_len = IPA_CLAT_V6_PREFIX_LEN_32;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix_len = IPA_CLAT_V6_PREFIX_LEN_32;

  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[0] = 0x08;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[1] = 0x09;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[2] = 0x0A;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[3] = 0x0B;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[4] = 0x0C;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[5] = 0x0D;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[6] = 0x0E;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[7] = 0x0F;

  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[0] = 0x08;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[1] = 0x09;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[2] = 0x0A;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[3] = 0x0B;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[4] = 0x0C;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[5] = 0x0D;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[6] = 0x0E;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[7] = 0x0F;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine P0 test
Test description : Random CLAT config, IPV6 Packet without extension header,
                   HDR type = None
 
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p0::test0007()
{
  ipa_test_ret_code_e ret_code;
 
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 1;
   engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine CLAT DL P0 test
Test description : Random CLAT config. 20 IPv6 with and without xtension headers.

Pass Criteria: CLAT translated IPv4 pkts are received back and they are not 
               supposed to have bytes from extension headers in the payload.

*/

void ipa_test_engine_clat_dl_p0::test0008()
{
  ipa_test_ret_code_e ret_code;
  
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled = FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine CLAT DL P0 test
Test description : Random CLAT config. 20 IPV4 and IPV6 pkt.
                   IPV6 packets with and without extension headers.
                   HDR TYPE = QMAP     

Pass Criteria: 
*/

void ipa_test_engine_clat_dl_p0::test0009()
{
  ipa_test_ret_code_e ret_code;
  
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled = FALSE;
  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p0_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine P0 test
Test description : Random CLAT config. 
                   20 IPV4 and IPV6 Packets with/without extension header.
                   HDR type = QMAP V3
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p0::test0010()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_clat_dl_p1::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_clat_dl_p1::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_clat_dl_p1_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description : 2 CLAT PDNs
                   HDR type : QMAP
                   5 IPV4/ IPV6 packets with and without extension headers.
 
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p1::test0001()
{
  ipa_test_ret_code_e ret_code;
 
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 2;
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 
  
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.bearer_tech[1] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p1_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine P0 test
Test description : 2 CLAT PDNs
                   HDR type: QMAP, 5 IPV4 and IPV6 packets
                   with and without extension headers.
 
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p1::test0002()
{
  ipa_test_ret_code_e ret_code;
 
  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 2;
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
   engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 
  
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.bearer_tech[1] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p1_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine P0 test
Test description : 1 CLAT and 1 non CLAT PDN, 5 IPV4/IPV6 packets
                   HDR TYPE: QMAP
 
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p1::test0003()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.bearer_tech[1] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_clat_dl_p1_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine P0 test
Test description : 1 CLAT and 1 non CLAT PDN, 5 IPV4/IPV6 packets
                   HDR TYPE: QMAP V3 , 
 
Pass Criteria: 

*/

void ipa_test_engine_clat_dl_p1::test0004()
{
  ipa_test_ret_code_e ret_code;

  /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_MAX;
  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.params.ipv6_ext_hdr_type = IPA_TEST_ENGINE_MAX_EXTN_HDR_TYPE;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.bearer_tech[1] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_clat_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_loopback_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_loopback_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_loopback_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description :  Loopback
 
Pass Criteria: 

*/

void ipa_test_engine_loopback_p0::test0001()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_DIRECT_DMA;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_TCP;

  engine_params.wan_cfg.num_bearers = 0;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
   
/****************** Test case ipa_test_engine_loopback_p0_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine P0 test
Test description :  Loopback
 
Pass Criteria: 

*/

void ipa_test_engine_loopback_p0::test0002()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_DIRECT_DMA;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
   
/****************** Test case ipa_test_engine_loopback_p0_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine P0 test
Test description :  Loopback
 
Pass Criteria: 

*/

void ipa_test_engine_loopback_p0::test0003()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}
   
/****************** Test case ipa_test_engine_loopback_p0_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine P0 test
Test description :  Loopback
 
Pass Criteria: 

*/

void ipa_test_engine_loopback_p0::test0004()
{
  ipa_test_ret_code_e ret_code;

  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_clat_loopback_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_clat_loopback_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_clat_loopback_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine CLAT Loopback P0 test
Test description :  DL CLAT Loopback.
                    Static CLAT config. IPv6 pkt sent in DL. Expected to be looped back
                    on UL.
 
Pass Criteria: Packet matches the sent Ipv6 packet Packet but has extension header. 

*/

void ipa_test_engine_clat_loopback_p0::test0001()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
	IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
	IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
   //engine_params.clat_cfg.global_clat_cfg.v4_client_subnet_mask = 0x0A0A0000;

  /*! CLAT context for the first PDN */
  //engine_params.clat_cfg.cntxt_info[0].v4_src_addr = 0x0A0A0505;
  engine_params.clat_cfg.cntxt_info[0].v4_dest_addr = 0x0B0B0404;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix_len = IPA_CLAT_V6_PREFIX_LEN_32;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix_len = IPA_CLAT_V6_PREFIX_LEN_32;

  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[0] = 0x08;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[1] = 0x09;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[2] = 0x0A;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[3] = 0x0B;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[4] = 0x0C;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[5] = 0x0D;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[6] = 0x0E;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[7] = 0x0F;

  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[0] = 0x08;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[1] = 0x09;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[2] = 0x0A;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[3] = 0x0B;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[4] = 0x0C;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[5] = 0x0D;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[6] = 0x0E;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[7] = 0x0F;
  
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  
  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }  
}

/****************** Test case ipa_test_engine_clat_loopback_p0_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine CLAT Loopback P0 test
Test description :  DL CLAT Loopback.
                    Random CLAT config. IPv6 pkt sent in DL. Expected to be looped back
                    on UL in IPv4 packet.
 
Pass Criteria: Packet matches the sent Ipv6 packet but has extension header 
               with the expection of hop limit field is decremented twice.

*/

void ipa_test_engine_clat_loopback_p0::test0002()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV6;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_clat_loopback_p0_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine CLAT Loopback P0 test
Test description :  UL CLAT Loopback.
                    Static CLAT config. IPv4 pkt sent in UL.
                    Expected to be looped back on DL.
 
Pass Criteria: Packet matches the sent Ipv4 packet with the expection of TTL field is decremented twice.

*/

void ipa_test_engine_clat_loopback_p0::test0003()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
   //engine_params.clat_cfg.global_clat_cfg.v4_client_subnet_mask = 0x0A0A0000;

  /*! CLAT context for the first PDN */
  //engine_params.clat_cfg.cntxt_info[0].v4_src_addr = 0x0A0A0505;
  engine_params.clat_cfg.cntxt_info[0].v4_dest_addr = 0x0B0B0404;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix_len = IPA_CLAT_V6_PREFIX_LEN_32;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix_len = IPA_CLAT_V6_PREFIX_LEN_32;

  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[0] = 0x08;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[1] = 0x09;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[2] = 0x0A;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[3] = 0x0B;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[4] = 0x0C;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[5] = 0x0D;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[6] = 0x0E;
  engine_params.clat_cfg.cntxt_info[0].v6_src_prefix[7] = 0x0F;

  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[0] = 0x01;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[1] = 0x02;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[2] = 0x03;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[3] = 0x04;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[4] = 0x05;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[5] = 0x06;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[6] = 0x07;
  engine_params.clat_cfg.cntxt_info[0].v6_dest_prefix[7] = 0x08;
 
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_clat_loopback_p0_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine CLAT Loopback P0 test
Test description :  UL CLAT Loopback.
                    Random CLAT config. IPv4 pkt sent in UL.
                    Expected to be looped back on DL.
 
Pass Criteria: Packet matches the sent Ipv4 packet with the expection of TTL field is decremented twice.

*/

void ipa_test_engine_clat_loopback_p0::test0004()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}   

/****************** Test case ipa_test_engine_clat_loopback_p0_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine CLAT Loopback P0 test
Test description :  UL CLAT Loopback.
                    Random CLAT config. IPv4 pkt sent in UL.
                    Expected to be looped back on DL.
 
Pass Criteria: Packet matches the sent Ipv4 packet with the expection of TTL field is decremented twice.

*/

void ipa_test_engine_clat_loopback_p0::test0005()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

   /*! CLAT configuration */
  engine_params.clat_cfg.num_clat_pdn = 1;
  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.payload_len = 1200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_clat_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

void ipa_test_engine_deaggr_err_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_deaggr_err_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description : Test ENDP_DELAY for UL Flow Control 
 
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0001()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_b2b_cfg[0] = FALSE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 10;
  engine_params.is_fc_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine P0 test
Test description : Test ENDP_DELAY for UL Flow Control with aggr enabled 
 
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0002()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_b2b_cfg[0] = FALSE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 10;
  engine_params.is_fc_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine P0 test
Test description : Test ENDP_DELAY for UL Flow Control 
 
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0003()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 5;
  engine_params.is_fc_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine P0 test
Test description : Test ENDP_DELAY for UL Flow Control with aggr enabled.
 
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0004()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 8;
  engine_params.is_fc_enabled = TRUE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine P0 test
Test description : To test Deaggr histogram stats update.
 
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0005()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.rx_pkt_test_type =
  IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_HIST_STATS;
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.ip_pkt_cfg.num_pkts = 6;
  engine_params.ip_pkt_cfg.payload_len = 1000;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine P0 test
Test description : Test byte threshold based aggregation. 
                   (pkt drop case with BYTE_LIMIT bit set)
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0006()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.rx_pkt_test_type =
  IPA_TEST_ENGINE_RX_PKT_WAIT_DROPPED_PKT_BYTE_THRESHOLD;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 1500;   //AGGR_BYTE_LIMIT will be set to 1k 
                              //since value is floored.
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 0;  //pkt_limit = 0
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine P0 test
Test description : Test byte threshold based aggregation. 
                   (2 EOT frames received with PREV_EOT set)
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0007()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.rx_pkt_test_type =
  IPA_TEST_ENGINE_RX_PKT_WAIT_PREVEOT_BYTE_THRESHOLD;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
  .max_aggr_bytes = 2 * 1504; //AGGR_BYTE_LIMIT will be set to 2k 
                              //since value is floored.
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 0;       //pkt_limit = 0
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine P0 test
Test description : Deaggr error handling scenario_1. 
   Bad QMAP hdr in the first pkt-->
     max_aggr_pkts = 3;
     num_pkts = 4;
     all pkts/first pkt = error pkts
Pass Criteria: 
  1. 2 Status pkts with deaggr_exception set
  2. drop_reason = IPA_UL_DEAGGR_DROP_REASON_EXCEPTION = 0x4),
     pkt_tx_status = IPA_UL_DEAGGR_PKT_TRACE_TX_EXCEPTION_DROP = 0x5),
  3. ipa_stats.ul.num_pkts_dropped = 0x2
     ipa_stats.endp[3].ul.num_exceptions_dropped = 0x2  // B2B consumer pipe

*/

void ipa_test_engine_deaggr_err_p0::test0008()
{
  ipa_test_ret_code_e ret_code;
 
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 13000; /* 3 pkts * 4100 (max_pkt_size)*/
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 4;
  engine_params.ip_pkt_cfg.params.max_pkt_size = 2000;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
  
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine P0 test
Test description : Deaggr error handling scenario_2. 
   Bad QMAP header in the middle of one frame--> 
      max_aggr_pkts = 3
      num_pkts = 3
      second_pkt = error_pkt
Pass Criteria: 
  1. 2 Status pkts with deaggr_exception set
  2. drop_reason = IPA_UL_DEAGGR_DROP_REASON_EXCEPTION = 0x4),
     pkt_tx_status = IPA_UL_DEAGGR_PKT_TRACE_TX_EXCEPTION_DROP = 0x5),
  3. ipa_stats.ul.num_pkts_dropped = 0x2
     ipa_stats.endp[3].ul.num_exceptions_dropped = 0x2  // B2B consumer pipe

*/

void ipa_test_engine_deaggr_err_p0::test0009()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_PKTLEN_GT_EOT_1;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 3;
	engine_params.ip_pkt_cfg.payload_len = 1000;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine P0 test
Test description : Deaggr error handling scenario_6. 
   Trucated qmap hdr (<4 bytes)
   max_aggr_pkts = 2
   num_pkts = 2 
   2nd pkt has a qmap hdr of 3 bytes
Pass Criteria: 
  

*/

void ipa_test_engine_deaggr_err_p0::test0010()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOT_LT_MIN_FRAME_SIZE;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0011 ***********/
/* 
Test case number : 0011
Test Component : IPA Test Engine P0 test
Test description : Deaggr error handling scenario_4. (eot frame len > 16k)
   max_aggr_pkts = 4
   num_pkts=4
   max_aggr_bytes = 32100
   payload_len = 4000
   QMAP hdr = valid
Pass Criteria: 
All pkts are recevied as it is.  

*/

void ipa_test_engine_deaggr_err_p0::test0011()
{
  ipa_test_ret_code_e ret_code;
 
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 32100;
  //engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
  //  .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  //engine_params.ip_pkt_cfg.num_pkts = 4;
  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  //engine_params.ip_pkt_cfg.payload_len = 3972;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.params.max_pkt_size = IPA_TEST_MAX_AGGR_PACKET_SIZE;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0012 ***********/
/* 
Test case number : 0012
Test Component : IPA Test Engine P0 test
Test description : Deaggr error handling scenario_3. (eot frame len > 16k)
   max_aggr_pkts = 4
   num_pkts=4
   max_aggr_bytes = 32100
   payload_len = 4000
   QMAP hdr = 0ed,
   
Pass Criteria: 


*/

void ipa_test_engine_deaggr_err_p0::test0012()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_EOTLEN_GT_MAX_MSG_SIZE_1;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 32100;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 3972;
  engine_params.ip_pkt_cfg.params.max_pkt_size = IPA_TEST_MAX_AGGR_PACKET_SIZE;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0013 ***********/
/* 
Test case number : 0013
Test Component : IPA Test Engine P0 test
Test description : Deaggr error handling scenario_5.
   valid pad len field with no padding bytes
   
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0013()
{
  ipa_test_ret_code_e ret_code;
 
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_NO_PAD_BYTES;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 32100;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  //engine_params.ip_pkt_cfg.payload_len = 3972;
  engine_params.ip_pkt_cfg.params.max_pkt_size = IPA_TEST_MAX_AGGR_PACKET_SIZE;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0014 ***********/
/* 
Test case number : 0014
Test Component : IPA Test Engine P0 test
Test description : Deaggr error handling scenario_6.
   msg_len > 4k. (deaggr.max_pkt_size = 4k)
   
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0014()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
   IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_QMAP_MSGLEN_GT_MAX_MSG_SIZE;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 32100;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  //engine_params.ip_pkt_cfg.payload_len = 15990;
  //engine_params.ip_pkt_cfg.params.max_pkt_size = 16100;

  engine_params.ip_pkt_cfg.payload_len = 3980;
  engine_params.ip_pkt_cfg.params.max_pkt_size = 4200;

  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0015 ***********/
/* 
Test case number : 0015
Test Component : IPA Test Engine P0 test
Test description : Test QMAP Aggr Force close on 1 pipe with 2 active pipes
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0015()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.rx_pkt_test_type =
  IPA_TEST_ENGINE_RX_PKT_WAIT_AGGR_FORCE_CLOSE_PIPES;

  /*Sim1 config*/
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
  .max_aggr_bytes = 2 * 1504; //AGGR_BYTE_LIMIT will be set to 2k 
                              //since value is floored.
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 0;       //pkt_limit = 0
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  /*Sim2 config*/
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 2 * 1504;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 4;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;

  engine_params.wan_cfg.num_bearers = 2;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_bearer_bridged[1] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0016 ***********/
/* 
Test case number : 0016
Test Component : IPA Test Engine P0 test
Test description : RNDIS Deaggr error handling scenario. 
   Trucated rndis hdr (<44+14 bytes)
   max_aggr_pkts = 2
   num_pkts = 2 
   2nd pkt is < 44+14 bytes 
Pass Criteria: 
  

*/

void ipa_test_engine_deaggr_err_p0::test0016()
{
  ipa_test_ret_code_e ret_code;
 
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_EOT_LT_MIN_FRAME_SIZE;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_RNDIS;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  /* Ethernet cfg */
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;

  ipa_test_generate_random_data(engine_params.endp_cfg.sim_endp_cfg[0] \
                                      .ul_config.params.ether.ipv4_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 2;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/***** ************* Test case ipa_test_engine_deaggr_err_p0_0017 ***********/
/* 
Test case number : 0017
Test Component : IPA Test Engine P0 test
Test description : RNDIS Deaggr error handling scenario. 
   max_aggr_pkts = 3
   num_pkts = 3
   2nd RNDIS hdr has invalid data offset field.
   (data_offset_field_locn + data_offset_field_value > msg_len )
Pass Criteria: 
  

*/

void ipa_test_engine_deaggr_err_p0::test0017()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_OFFSET;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_RNDIS;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 5000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  /* Ethernet cfg */
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;

  ipa_test_generate_random_data(engine_params.endp_cfg.sim_endp_cfg[0] \
                                      .ul_config.params.ether.ipv4_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 3;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0018 ***********/
/* 
Test case number : 0018
Test Component : IPA Test Engine P0 test
Test description : RNDIS Deaggr error handling scenario. 
   max_aggr_pkts = 3
   num_pkts = 3
   2nd RNDIS hdr has invalid msg len value.
   
Pass Criteria: 
  

*/

void ipa_test_engine_deaggr_err_p0::test0018()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_MSG_LEN;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_RNDIS;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 5000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  /* Ethernet cfg */
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;

  ipa_test_generate_random_data(engine_params.endp_cfg.sim_endp_cfg[0] \
                                      .ul_config.params.ether.ipv4_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 3;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0019 ***********/
/* 
Test case number : 0019
Test Component : IPA Test Engine P0 test
Test description : RNDIS Deaggr error handling scenario. 
   max_aggr_pkts = 3
   num_pkts = 3
   2nd RNDIS hdr has invalid data len value.
   
Pass Criteria: 
  

*/

void ipa_test_engine_deaggr_err_p0::test0019()
{
  ipa_test_ret_code_e ret_code;
 
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_INVALID_DATA_LEN;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_RNDIS;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 5000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  /* Ethernet cfg */
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;

  ipa_test_generate_random_data(engine_params.endp_cfg.sim_endp_cfg[0] \
                                      .ul_config.params.ether.ipv4_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 3;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0020 ***********/
/* 
Test case number : 0020
Test Component : IPA Test Engine P0 test
Test description : RNDIS Deaggr error handling scenario. 
   max_aggr_pkts = 3
   num_pkts = 3
   1st RNDIS pkt > max_msg_size.
   
Pass Criteria: 
  

*/

void ipa_test_engine_deaggr_err_p0::test0020()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_RNDIS_MSGLEN_GT_MAX_MSG_SIZE;
  engine_params.rx_pkt_test_type =   
    IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_RNDIS;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 5000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 3;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  /* Ethernet cfg */
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;

  ipa_test_generate_random_data(engine_params.endp_cfg.sim_endp_cfg[0] \
                                      .ul_config.params.ether.ipv4_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
  IPA_MEMCPY(engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv6_hdr, 
             engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether.\
             ipv4_hdr, IPA_SIO_ETHERNET_HDR_LEN);

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.ip_pkt_cfg.num_pkts = 3;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.is_fc_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;
  engine_params.is_dpl_enabled =  FALSE;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0021 ***********/
/* 
Test case number : 0021
Test Component : IPA Test Engine P0 test
Test description : Test Tx Threshold interrupt (Yellow/Red marker)
 
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0021()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.rx_pkt_test_type = 
     IPA_TEST_ENGINE_RX_PKT_WAIT_TX_THRESHOLD_INT_STATS;
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;

  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
  .max_aggr_bytes = 2 * 1504; //AGGR_BYTE_LIMIT will be set to 2k 
                              //since value is floored.
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 0;       //pkt_limit = 0
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 2; 
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_OTHER;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0022 ***********/
/* 
Test case number : 0022
Test Component : IPA Test Engine P0 test
Test description : MBIM deaggr err scenario.1 
                   Invalid NTH sig 
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0022()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.rx_pkt_test_type = 
     IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NTH_SIG;
 
 /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ncm_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ndp_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.ip_pkt_cfg.num_pkts = 2; 
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
  
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0023 ***********/
/* 
Test case number : 0023
Test Component : IPA Test Engine P0 test
Test description : MBIM deaggr err scenario.2
                   Invalid NDP sig in 2nd hdr
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0023()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.rx_pkt_test_type = 
     IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NDP_SIG;
 
 /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 1536*2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ncm_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ndp_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.ip_pkt_cfg.num_pkts = 2; 
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0024 ***********/
/* 
Test case number : 0024
Test Component : IPA Test Engine P0 test
Test description : MBIM deaggr err scenario.3
                   Truncated MBIM frame
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0024()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.rx_pkt_test_type = 
     IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_EOT_LT_MIN_FRAME_SIZE;
 
 /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 1536*2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ncm_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ndp_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.ip_pkt_cfg.num_pkts = 2; 
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0025 ***********/
/* 
Test case number : 0025
Test Component : IPA Test Engine P0 test
Test description : MBIM deaggr err scenario.4
                   Invalid Next NDP idx in NDP hdr.
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0025()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.rx_pkt_test_type = 
     IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_NXT_NDP_IDX;
 
 /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 1536*2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ncm_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ndp_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.ip_pkt_cfg.num_pkts = 2; 
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0026 ***********/
/* 
Test case number : 0026
Test Component : IPA Test Engine P0 test
Test description : MBIM deaggr err scenario.5
                   Invalid dgram idx in NDP dgram hdr.
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0026()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.rx_pkt_test_type = 
     IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_DGRM_IDX;
 
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 1536*2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ncm_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ndp_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.ip_pkt_cfg.num_pkts = 2; 
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_deaggr_err_p0_0027 ***********/
/* 
Test case number : 0027
Test Component : IPA Test Engine P0 test
Test description : MBIM deaggr err scenario.6
                   2 EOT frames with second one being invalid
Pass Criteria: 

*/

void ipa_test_engine_deaggr_err_p0::test0027()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.rx_pkt_test_type = 
     IPA_TEST_ENGINE_RX_PKT_WAIT_DEAGGR_ERR_HANDLING;
  engine_params.deaggr_err_cfg.err_type = 
    IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MBIM_INVALID_SEC_EOT_FRAME;
 
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  //engine_params.endp_cfg.sim_endp_b2b_cfg[0] = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 1536*2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 1; /* Build 2 EOT frames with 1 pkt each */
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ncm_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim.ndp_sig = 0xFF;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  engine_params.ip_pkt_cfg.num_pkts = 2; 
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.prot = IPA_TEST_ENGINE_IP_PKT_PROT_UDP;
  engine_params.ip_pkt_cfg.payload_len = 1472;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_holb_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_holb_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_holb_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description :  UL+DL with DL HOLB
 
Pass Criteria: 

*/

void ipa_test_engine_holb_p0::test0001()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 25;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.dl_holb_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_holb_p0_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine P0 test
Test description :  UL+DL+HW Rep with DL HOLB
 
Pass Criteria: 

*/

void ipa_test_engine_holb_p0::test0002()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
		IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.ip_pkt_cfg.num_pkts = 25;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.hw_rep_holb_enabled = TRUE;
  engine_params.is_hw_replication_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_HOLB;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_holb_p0_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine P0 test
Test description :  UL+DL+HW Rep with DL HOLB
 
Pass Criteria: 

*/

void ipa_test_engine_holb_p0::test0003()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
		IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.ip_pkt_cfg.num_pkts = 25;
  engine_params.ip_pkt_cfg.payload_len = 200;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.hw_rep_holb_enabled = TRUE;
  engine_params.is_hw_replication_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL_HOLB;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_holb_p0_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine P0 test
Test description :  UL+DL with UL HOLB
 
Pass Criteria: 

*/

void ipa_test_engine_holb_p0::test0004()
{
  ipa_test_ret_code_e ret_code;
 
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 1;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.ip_pkt_cfg.num_pkts = 25;
  engine_params.ip_pkt_cfg.payload_len = 0;
  engine_params.ip_pkt_cfg.version = IPA_IP_TYPE_IPV4;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.ul_holb_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL_HOLB;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

void ipa_test_engine_hw_rep_p0::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_hw_rep_p0::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_hw_rep_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description : HW replication
 
Pass Criteria: 

*/

void ipa_test_engine_hw_rep_p0::test0001()
{
  ipa_test_ret_code_e ret_code;
   
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
   IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
   IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1;
  
  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;
  
  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_hw_replication_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_hw_rep_p0_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine P0 test
Test description : HW replication
 
Pass Criteria: 

*/

void ipa_test_engine_hw_rep_p0::test0002()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
	IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = TRUE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_hw_replication_enabled =  TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_hw_rep_p0_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine P0 test
Test description :  HW replication
 
Pass Criteria: 

*/

void ipa_test_engine_hw_rep_p0::test0003()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
	IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_hw_replication_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL_LOOPBACK;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_hw_rep_p0_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine P0 test
Test description :  HW replication
 
Pass Criteria: 

*/

void ipa_test_engine_hw_rep_p0::test0004()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
	IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.is_random_data = TRUE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;
  
  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_hw_replication_enabled = TRUE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;
  
  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}

/****************** Test case ipa_test_engine_hw_rep_p0_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine P0 test
Test description : HW replication
 
Pass Criteria: 

*/

void ipa_test_engine_hw_rep_p0::test0005()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[0] = 1; 
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
	 IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.ip_pkt_cfg.num_pkts = 20;
  engine_params.ip_pkt_cfg.num_dsm_items = 1;
  engine_params.ip_pkt_cfg.is_random_data = FALSE;

  engine_params.wan_cfg.num_bearers = 1;
  engine_params.wan_cfg.bearer_tech[0] = IPA_WAN_BEARER_TECH_LTE;
  engine_params.wan_cfg.is_bearer_bridged[0] = TRUE;
  engine_params.wan_cfg.is_ciph_enabled = FALSE;

  engine_params.is_dpl_enabled =  FALSE;
  engine_params.is_hw_replication_enabled = TRUE;

  engine_params.fltr_cfg.is_filter_enabled = FALSE;
  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  } 
}


/****************** Test case ipa_test_engine_reg_p0_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine P0 test
Test description : 
 
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_loopback_p0_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine P0 test
Test description :  Loopback
 
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_loopback_p0_0007 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine P0 test
Test description :  Loopback
 
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA 2.1 Test Engine P0 test
Test description : Test ENDP_DELAY for UL Flow Control in BAM2YS mode
 
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA 2.1 Test Engine P0 test
Test description : Test ENDP_DELAY for UL Flow Control with aggr enabled in BAM2SYS mode
 
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA 2.1 Test Engine P0 test
Test description : Test ENDP_DELAY for UL Flow Control in BAM2BAM mode
 
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA 2.1 Test Engine P0 test
Test description : Test ENDP_DELAY for UL Flow Control in BAM2BAM mode with aggr enabled.
 
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine P0 test
Test description : To test Deaggr histogram stats update.
 
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine P0 test
Test description : Test byte threshold based aggregation. 
                   (pkt drop case with BYTE_LIMIT bit set)
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine P0 test
Test description : Test byte threshold based aggregation. 
                   (2 EOT frames received with PREV_EOT set)
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA 2.1 Test Engine P0 test
Test description : Deaggr error handling scenario_1. 
   Bad QMAP hdr in the first pkt-->
     max_aggr_pkts = 3;
     num_pkts = 4;
     all pkts/first pkt = error pkts
Pass Criteria: 
  1. 2 Status pkts with deaggr_exception set
  2. drop_reason = IPA_UL_DEAGGR_DROP_REASON_EXCEPTION = 0x4),
     pkt_tx_status = IPA_UL_DEAGGR_PKT_TRACE_TX_EXCEPTION_DROP = 0x5),
  3. ipa_stats.ul.num_pkts_dropped = 0x2
     ipa_stats.endp[3].ul.num_exceptions_dropped = 0x2  // B2B consumer pipe

*/

/****************** Test case ipa_test_engine_2_1_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA 2.1 Test Engine P0 test
Test description : Deaggr error handling scenario_2. 
   Bad QMAP header in the middle of one frame--> 
      max_aggr_pkts = 3
      num_pkts = 3
      second_pkt = error_pkt
Pass Criteria: 
  1. 2 Status pkts with deaggr_exception set
  2. drop_reason = IPA_UL_DEAGGR_DROP_REASON_EXCEPTION = 0x4),
     pkt_tx_status = IPA_UL_DEAGGR_PKT_TRACE_TX_EXCEPTION_DROP = 0x5),
  3. ipa_stats.ul.num_pkts_dropped = 0x2
     ipa_stats.endp[3].ul.num_exceptions_dropped = 0x2  // B2B consumer pipe

*/

/****************** Test case ipa_test_engine_2_1_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA 2.1 Test Engine P0 test
Test description : Deaggr error handling scenario_6. 
   Trucated qmap hdr (<4 bytes)
   max_aggr_pkts = 2
   num_pkts = 2 
   2nd pkt has a qmap hdr of 3 bytes
Pass Criteria: 
  

*/

/****************** Test case ipa_test_engine_2_1_0011 ***********/
/* 
Test case number : 0011
Test Component : IPA 2.1 Test Engine P0 test
Test description : Deaggr error handling scenario_4. (eot frame len > 16k)
   max_aggr_pkts = 4
   num_pkts=4
   max_aggr_bytes = 32100
   payload_len = 4000
   QMAP hdr = valid
Pass Criteria: 
All pkts are recevied as it is.  

*/

/****************** Test case ipa_test_engine_2_1_0012 ***********/
/* 
Test case number : 0012
Test Component : IPA 2.1 Test Engine P0 test
Test description : Deaggr error handling scenario_3. (eot frame len > 16k)
   max_aggr_pkts = 4
   num_pkts=4
   max_aggr_bytes = 32100
   payload_len = 4000
   QMAP hdr = 0ed,
   
Pass Criteria: 


*/

/****************** Test case ipa_test_engine_2_1_0013 ***********/
/* 
Test case number : 0013
Test Component : IPA 2.1 Test Engine P0 test
Test description : Deaggr error handling scenario_5.
   valid pad len field with no padding bytes
   
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0014 ***********/
/* 
Test case number : 0014
Test Component : IPA 2.1 Test Engine P0 test
Test description : Deaggr error handling scenario_6.
   msg_len > 4k. (deaggr.max_pkt_size = 4k)
   
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0015 ***********/
/* 
Test case number : 0015
Test Component : IPA Test Engine P0 test
Test description : Test QMAP Aggr Force close on 1 pipe with 2 active pipes
Pass Criteria: 

*/

/****************** Test case ipa_test_engine_2_1_0016 ***********/
/* 
Test case number : 0016
Test Component : IPA 2.1 Test Engine P0 test
Test description : RNDIS Deaggr error handling scenario. 
   Trucated rndis hdr (<44+14 bytes)
   max_aggr_pkts = 2
   num_pkts = 2 
   2nd pkt is < 44+14 bytes 
Pass Criteria: 
  

*/

#if 0
/****************** Test case ipa_test_engine_2_1_0017 ***********/
/* 
Test case number : 0017
Test Component : IPA 2.1 Test Engine P0 test
Test description : RNDIS Deaggr error handling scenario. 
   Rndis pkt split across 2 buffers
   max_aggr_pkts = 2
   num_pkts = 2 
   buffer 1 = RNDIS1 + 20 bytes of RNDIS2
   buffer 2 = RNDIS2
Pass Criteria: 
  

*/

/****************** Test case ipa_test_engine_2_1_0018 ***********/
/* 
Test case number : 0018
Test Component : IPA 2.1 Test Engine P0 test
Test description : RNDIS Deaggr error handling scenario. 
   max_aggr_pkts = 3
   num_pkts = 3
   2nd RNDIS hdr has invalid msg type field.
Pass Criteria: 
  

*/
#endif
