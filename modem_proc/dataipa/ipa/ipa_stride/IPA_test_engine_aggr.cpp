#include "comdef.h"
#include "IPA_test_engine.h"
#include "IPA_test.h"

#include "IPA_test_engine_aggr.h"

/*==============================================================================
 
                             MACROS
 
==============================================================================*/
/*!@brief Number of times each cfg'ed test will be run */
#define IPA_TEST_ENGINE_P0_TEST_MAX_RUN 1

#define IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH    4

/*==============================================================================

                         INTERNAL VARIABLES

==============================================================================*/
ipa_test_engine_params_s engine_params;

void ipa_test_engine_ul_aggr::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_ul_aggr::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_ul_aggr_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: QMAP_V3 
Sim 2: QMAP  
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0001()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: QMAP
Sim 2: QMAP  
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0002()
{
  ipa_test_ret_code_e ret_code;
 
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: QMAP_V3 
Sim 2: QMAP_V3
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0003()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: QMAP_V3 
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0004()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: QMAP_V3 
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0005()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: ETHERNET
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0006()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: QMAP
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0007()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: NONE
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0008()
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
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: QMAP
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0009()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: NONE
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0010()
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
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0011 ***********/
/* 
Test case number : 0011
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: MBIM
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0011()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0012 ***********/
/* 
Test case number : 0012
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: MBIM
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0012()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0013 ***********/
/* 
Test case number : 0013
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: MBIM
Sim 2: QMAP
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0013()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1);

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0014 ***********/
/* 
Test case number : 0014
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: MBIM
Sim 2: QMAP_V3
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0014()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1);

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_ul_aggr_0015 ***********/
/* 
Test case number : 0015
Test Component : IPA Test Engine Aggregation test
Test Type:  UL 
Test description : 
Sim 1: MBIM
Sim 2: MBIM
Pass Criteria: 

*/

void ipa_test_engine_ul_aggr::test0015()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1);

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_UL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

void ipa_test_engine_dl_aggr::setup()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_cfg_generate(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

void ipa_test_engine_dl_aggr::teardown()
{
  ipa_test_ret_code_e ret_code;
  ret_code = ipa_test_engine_destroy();
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
}

/****************** Test case ipa_test_engine_dl_aggr_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: QMAP_V3 
Sim 2: QMAP  
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0001()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: QMAP
Sim 2: QMAP  
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0002()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: QMAP_V3 
Sim 2: QMAP_V3
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0003()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: QMAP_V3 
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0004()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: QMAP_V3 
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0005()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: ETHERNET
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0006()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1; 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: QMAP
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0007()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: NONE
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0008()
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
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: QMAP
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0009()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: NONE
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0010()
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
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0011 ***********/
/* 
Test case number : 0011
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: MBIM
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0011()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0012 ***********/
/* 
Test case number : 0012
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: MBIM
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0012()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0013 ***********/
/* 
Test case number : 0013
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: MBIM
Sim 2: QMAP
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0013()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1);

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0014 ***********/
/* 
Test case number : 0014
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: MBIM
Sim 2: QMAP_V3
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0014()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1);

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_dl_aggr_0015 ***********/
/* 
Test case number : 0015
Test Component : IPA Test Engine Aggregation test
Test Type:  DL 
Test description : 
Sim 1: MBIM
Sim 2: MBIM
Pass Criteria: 

*/

void ipa_test_engine_dl_aggr::test0015()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1);

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0001 ***********/
/* 
Test case number : 0001
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: QMAP_V3 
Sim 2: QMAP  
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0001()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0002 ***********/
/* 
Test case number : 0002
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: QMAP
Sim 2: QMAP  
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0002()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1);

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0003 ***********/
/* 
Test case number : 0003
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: QMAP_V3 
Sim 2: QMAP_V3
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0003()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1);  

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0004 ***********/
/* 
Test case number : 0004
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: QMAP_V3 
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0004()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0005 ***********/
/* 
Test case number : 0005
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: QMAP_V3 
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0005()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
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
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;  

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0006 ***********/
/* 
Test case number : 0006
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: ETHERNET
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0006()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 1;

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0007 ***********/
/* 
Test case number : 0007
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: QMAP
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0007()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0008 ***********/
/* 
Test case number : 0008
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: NONE
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0008()
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
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0009 ***********/
/* 
Test case number : 0009
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: QMAP
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0009()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;  

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0010 ***********/
/* 
Test case number : 0010
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: NONE
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0010()
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
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;  

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0011 ***********/
/* 
Test case number : 0011
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: MBIM
Sim 2: NONE
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0011()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_NONE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1;  

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0012 ***********/
/* 
Test case number : 0012
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: MBIM
Sim 2: ETHERNET
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0012()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_ETHERNET;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_bytes = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .max_aggr_pkts = 0;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv4_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.ether \
    .ipv6_hdr_present = TRUE;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 1; 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0013 ***********/
/* 
Test case number : 0013
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: MBIM
Sim 2: QMAP
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0013()
{
  ipa_test_ret_code_e ret_code;
  
  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmap \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0014 ***********/
/* 
Test case number : 0014
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: MBIM
Sim 2: QMAP_V3
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0014()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_QMAP_V3;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

/****************** Test case ipa_test_engine_loopback_aggr_0015 ***********/
/* 
Test case number : 0015
Test Component : IPA Test Engine Aggregation test
Test Type:  DL_LOOPBACK
Test description : 
Sim 1: MBIM
Sim 2: MBIM
Pass Criteria: 

*/

void ipa_test_engine_loopback_aggr::test0015()
{
  ipa_test_ret_code_e ret_code;

  /* Now do our specific config */
  engine_params.endp_cfg.num_per_sim = 2;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].dl_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.params.mbim \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[0].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[0] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].dl_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.hdr_type = 
    IPA_SIO_HDR_TYPE_MBIM;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_bytes = 4000;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.params.qmapv2 \
    .max_aggr_pkts = 4;
  engine_params.endp_cfg.sim_endp_cfg[1].ul_config.is_qos_present = FALSE; 
  engine_params.endp_cfg.num_log_ch[1] = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_AGGR_MAX_NUM_LOG_CH, 1); 

  engine_params.test_type = IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK;

  ret_code = ipa_test_engine_configure(&engine_params);
  IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);

  for (uint32 engine_p0_run_cnt = 0; engine_p0_run_cnt < IPA_TEST_ENGINE_P0_TEST_MAX_RUN; engine_p0_run_cnt++)
  {
    ret_code = ipa_test_engine_run();
    IPA_TF_ASSERT(ret_code == IPA_TEST_RET_CODE_SUCCESS);
  }
}

