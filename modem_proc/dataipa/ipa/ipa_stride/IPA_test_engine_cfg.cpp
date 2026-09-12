/*!
  @file IPA_test_engine_cfg.cpp

  @brief
    IPA Test engine Cfg generator header file
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

$Header: //components/rel/dataipa.mpss/3.2/ipa/ipa_stride/IPA_test_engine_cfg.cpp#1 $

when       who     what, where, why
--------   ---     ------------------------------------------------------------- 
05/30/17   db      CR13276332: Final submit for Napali BU.
07/20/14   SaCh    Randomizing on df_bit in IPv4 header for CLAT scenarios.
02/17/15   mi      CR#739684: IPA ZIP feature related test changes.
05/25/14   rp      Test engine fixes/cleanup for issues seen on full stack build
04/25/14   rp      Changes to support ON-target testing on full stack CRM
04/16/14   pgm     rx pkt test type cfg.
03/26/14   mi      Regression test fixes.
03/06/14   mi      Added fragmentation test changes.
02/28/14   mi      Bearer suspend/resume and deregistration changes.
02/20/14   mi      Bug Fix: Aggregation params wrongly accessed.
12/09/13   mi      Randomly enabling DPL.
12/09/13   pgm     BAM2BAM and fc cfg
10/15/13   rp      DSM Chain fix & seed user cfg
09/19/13   rp      Move from QMAPv2 to QMAPv3
09/19/13   rp      Separate DL/UL aggr protocol support
08/22/13   rp      Initial Checkin
==============================================================================*/

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/
#include <comdef.h>
#include "IPA_test_engine.h"
#include "IPA_test.h"
#include "ipa_test_filter.h"

#pragma optimize("", off)
/*==============================================================================

                         MACROS

==============================================================================*/
#define IPA_TEST_ENGINE_CFG_ENDP_SUP_TYPE_NUM   7

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/
/*==============================================================================

                   INTERNAL DEFINITIONS AND TYPES

==============================================================================*/
/*=========================================================================

                         INTERNAL VARIABLES

=========================================================================*/
typedef struct
{
  ipa_sio_hdr_type_e dl_type;
  ipa_sio_hdr_type_e ul_type;
  uint8              max_num_log_ch; 
  uint16             ul_min_aggr_pkts;
  uint32             ul_min_aggr_bytes;
  uint16             ul_max_aggr_pkts;
  uint32             ul_max_aggr_bytes;
  uint16             dl_min_aggr_pkts;
  uint32             dl_min_aggr_bytes;
  uint16             dl_max_aggr_pkts;
  uint32             dl_max_aggr_bytes;
} ipa_test_engine_cfg_endp_sup_s;

ipa_test_engine_cfg_endp_sup_s ipa_test_engine_cfg_endp_sup \
  [IPA_TEST_ENGINE_CFG_ENDP_SUP_TYPE_NUM] = 
{
  {
    IPA_SIO_HDR_TYPE_NONE,      /* dl_type */
    IPA_SIO_HDR_TYPE_NONE,      /* ul_type */
    1,                          /* max_num_log_ch */
    0,                          /* ul_min_aggr_pkts */
    0,                          /* ul_min_aggr_bytes */
    0,                          /* ul_max_aggr_pkts */
    0,                          /* ul_max_aggr_bytes */
    0,                          /* dl_min_aggr_pkts */
    0,                          /* dl_min_aggr_bytes */
    0,                          /* dl_max_aggr_pkts */
    0                           /* dl_max_aggr_bytes */
  },
  {
    IPA_SIO_HDR_TYPE_ETHERNET,  /* dl_type */
    IPA_SIO_HDR_TYPE_ETHERNET,  /* ul_type */
    1,                          /* max_num_log_ch */
    0,                          /* ul_min_aggr_pkts */
    0,                          /* ul_min_aggr_bytes */
    0,                          /* ul_max_aggr_pkts */
    0,                          /* ul_max_aggr_bytes */
    0,                          /* dl_min_aggr_pkts */
    0,                          /* dl_min_aggr_bytes */
    0,                          /* dl_max_aggr_pkts */
    0                           /* dl_max_aggr_bytes */
  },
  {
    IPA_SIO_HDR_TYPE_QMAP,      /* dl_type */
    IPA_SIO_HDR_TYPE_QMAP,      /* ul_type */
    8,                          /* max_num_log_ch */
    1,                          /* ul_min_aggr_pkts */
    2500,                       /* ul_min_aggr_bytes */
    10,                         /* ul_max_aggr_pkts */
    6000,                       /* ul_max_aggr_bytes */
    1,                          /* dl_min_aggr_pkts */
    2500,                       /* dl_min_aggr_bytes */
    10,                         /* dl_max_aggr_pkts */
    6000                        /* dl_max_aggr_bytes */
  },
  {
    IPA_SIO_HDR_TYPE_QMAP_V3,   /* dl_type */
    IPA_SIO_HDR_TYPE_QMAP_V3,   /* ul_type */
    8,                          /* max_num_log_ch */
    1,                          /* ul_min_aggr_pkts */
    2500,                       /* ul_min_aggr_bytes */
    10,                         /* ul_max_aggr_pkts */
    6000,                       /* ul_max_aggr_bytes */
    1,                          /* dl_min_aggr_pkts */
    2500,                       /* dl_min_aggr_bytes */
    10,                         /* dl_max_aggr_pkts */
    6000                        /* dl_max_aggr_bytes */
  },
  {
    IPA_SIO_HDR_TYPE_NONE,      /* dl_type */
    IPA_SIO_HDR_TYPE_DIRECT_DMA,/* ul_type */
    1,                          /* max_num_log_ch */
    0,                          /* ul_min_aggr_pkts */
    0,                          /* ul_min_aggr_bytes */
    0,                          /* ul_max_aggr_pkts */
    0,                          /* ul_max_aggr_bytes */
    0,                          /* dl_min_aggr_pkts */
    0,                          /* dl_min_aggr_bytes */
    0,                          /* dl_max_aggr_pkts */
    0                           /* dl_max_aggr_bytes */
  },
  {
    IPA_SIO_HDR_TYPE_MBIM,      /* dl_type */
    IPA_SIO_HDR_TYPE_MBIM,      /* ul_type */
    8,                          /* max_num_log_ch */
    1,                          /* ul_min_aggr_pkts */
    1536,                       /* ul_min_aggr_bytes */
    10,                         /* ul_max_aggr_pkts */
    1536,                       /* ul_max_aggr_bytes */
    1,                          /* dl_min_aggr_pkts */
    2500,                       /* dl_min_aggr_bytes */
    10,                         /* dl_max_aggr_pkts */
    6000                        /* dl_max_aggr_bytes */
  },
  {
    IPA_SIO_HDR_TYPE_MBIM,      /* dl_type */
    IPA_SIO_HDR_TYPE_NONE,      /* ul_type */
    1,                          /* max_num_log_ch */
    1,                          /* ul_min_aggr_pkts */
    1536,                       /* ul_min_aggr_bytes */
    10,                         /* ul_max_aggr_pkts */
    1536,                       /* ul_max_aggr_bytes */
    1,                          /* dl_min_aggr_pkts */
    2500,                       /* dl_min_aggr_bytes */
    10,                         /* dl_max_aggr_pkts */
    6000                        /* dl_max_aggr_bytes */
  }
};

/*==============================================================================

                                FUNCTIONS

==============================================================================*/
/*==============================================================================

  FUNCTION:  ipa_test_engine_cfg_generate

==============================================================================*/
/*!
  @brief
  Generates a random cfg for a test

  @returns
  None
*/
/*============================================================================*/
ipa_test_ret_code_e ipa_test_engine_cfg_generate
(
  ipa_test_engine_params_s *params_ptr
)
{
  ipa_test_ret_code_e ret_code = IPA_TEST_RET_CODE_SUCCESS;
  uint8 idx = 0, i = 0;
  uint32 curr_seed = params_ptr->seed;

  IPA_TF_ASSERT(params_ptr != NULL);
  //IPA_ASSERT(0);

  IPA_MEMSET(params_ptr, 0, sizeof(ipa_test_engine_params_s));

  ipa_test_timetick_init();

  /***********************************************************************
                   Seed Generation 
  ***********************************************************************/
  /* We'll let the user send a non-zero seed to reproduce any test scenario */
  params_ptr->seed = curr_seed;
  if (params_ptr->seed == 0)
  {
    params_ptr->seed = ipa_test_timetick_get();
  }

  /* Now seed the random generator for this test */
  srand(params_ptr->seed);

  /***********************************************************************
                   Test Config Generation 
  ***********************************************************************/
  params_ptr->test_type = (ipa_test_engine_test_type_e)
    ipa_test_generate_random_num(IPA_TEST_ENGINE_TEST_TYPE_DL_LOOPBACK,
                                 IPA_TEST_ENGINE_TEST_TYPE_DL);

  
  /***********************************************************************
                   Endpoint Config Generation 
  ***********************************************************************/
  params_ptr->endp_cfg.num_per_sim = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_PER_SIM_NUM_MAX,
                                 1);

  for (i = 0; i < params_ptr->endp_cfg.num_per_sim; i++)
  {
    /* First generate the cfg for this endpoint */
    idx = 
      ipa_test_generate_random_num((uint32)(IPA_TEST_ENGINE_CFG_ENDP_SUP_TYPE_NUM - 3), 0);

    params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.hdr_type = 
      ipa_test_engine_cfg_endp_sup[idx].dl_type;
    params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.hdr_type = 
      ipa_test_engine_cfg_endp_sup[idx].ul_type;

    /* We want atleast one logical channel per endpoint */
    params_ptr->endp_cfg.num_log_ch[i] = 
      ipa_test_generate_random_num(ipa_test_engine_cfg_endp_sup[idx].max_num_log_ch,
                                   1);

    /*Init endp to SYS2BAM mode by default*/
    params_ptr->endp_cfg.sim_endp_b2b_cfg[i] = FALSE;

    switch (params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.hdr_type)
    {
      case IPA_SIO_HDR_TYPE_NONE:
        break;

      case IPA_SIO_HDR_TYPE_ETHERNET:
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.ether \
          .ipv4_hdr_present = TRUE;
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.ether \
          .ipv6_hdr_present = TRUE;
        ipa_test_generate_random_data(params_ptr->endp_cfg.sim_endp_cfg[i] \
                                      .dl_config.params.ether.ipv4_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
        IPA_MEMCPY(params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params \
                   .ether.ipv6_hdr, 
                   params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params \
                   .ether.ipv4_hdr,
                   IPA_SIO_ETHERNET_HDR_LEN);
        /*! @todo Re-enable this once the issue in the driver is fixed
        ipa_test_generate_random_data(params_ptr->endp_cfg.sim_endp_cfg[i] \
                                      .dl_config.params.ether.ipv6_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
        */
        break;

      case IPA_SIO_HDR_TYPE_QMAP:
      case IPA_SIO_HDR_TYPE_QMAP_V3:
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.qmap \
          .max_aggr_bytes = 
          ipa_test_generate_random_num(ipa_test_engine_cfg_endp_sup[idx] \
                                       .dl_max_aggr_bytes,
                                       ipa_test_engine_cfg_endp_sup[idx] \
                                       .dl_min_aggr_bytes);
        /* Multiple of 1024 K */
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.qmap \
          .max_aggr_bytes /= 1024;
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.qmap \
          .max_aggr_bytes *= 1024;
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.qmap \
          .max_aggr_pkts = 
          (uint16)ipa_test_generate_random_num((uint32)ipa_test_engine_cfg_endp_sup[idx] \
                                       .dl_max_aggr_pkts,
                                       (uint32)ipa_test_engine_cfg_endp_sup[idx] \
                                       .dl_min_aggr_pkts);
        break;

      case IPA_SIO_HDR_TYPE_MBIM:
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.mbim \
          .max_aggr_bytes = 
          ipa_test_generate_random_num(ipa_test_engine_cfg_endp_sup[idx] \
                                       .dl_max_aggr_bytes,
                                       ipa_test_engine_cfg_endp_sup[idx] \
                                       .dl_min_aggr_bytes);
        /* Multiple of 1024 K */
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.mbim\
          .max_aggr_bytes /= 1024;
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.mbim \
          .max_aggr_bytes *= 1024;
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.mbim \
          .max_aggr_pkts = 
          (uint16)ipa_test_generate_random_num(ipa_test_engine_cfg_endp_sup[idx] \
                                       .dl_max_aggr_pkts,
                                        ipa_test_engine_cfg_endp_sup[idx] \
                                       .dl_min_aggr_pkts);
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.mbim.ncm_sig = 
          ipa_test_generate_random_num(0xFFFFFFFF, 1);
        params_ptr->endp_cfg.sim_endp_cfg[i].dl_config.params.mbim.ndp_sig = 
          ipa_test_generate_random_num(0xFFFFFFFF, 1);
        break;

      default:
        IPA_TF_ASSERT(0);
    }

    switch (params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.hdr_type)
    {
      case IPA_SIO_HDR_TYPE_NONE:
      case IPA_SIO_HDR_TYPE_DIRECT_DMA:
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.is_qos_present = FALSE;
        break;

      case IPA_SIO_HDR_TYPE_ETHERNET:
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.ether \
          .ipv4_hdr_present = TRUE;
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.ether \
          .ipv6_hdr_present = TRUE;
        ipa_test_generate_random_data(params_ptr->endp_cfg.sim_endp_cfg[i] \
                                      .ul_config.params.ether.ipv4_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
        IPA_MEMCPY(params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params \
                   .ether.ipv6_hdr, 
                   params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params \
                   .ether.ipv4_hdr,
                   IPA_SIO_ETHERNET_HDR_LEN);
        /*! @todo Re-enable this once the issue in the driver is fixed
        ipa_test_generate_random_data(params_ptr->endp_cfg.sim_endp_cfg[i] \
                                      .ul_config.params.ether.ipv6_hdr,
                                      IPA_SIO_ETHERNET_HDR_LEN);
        */
        break;

      case IPA_SIO_HDR_TYPE_QMAP:
      case IPA_SIO_HDR_TYPE_QMAP_V3:
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.qmap \
          .max_aggr_bytes = 
          ipa_test_generate_random_num(ipa_test_engine_cfg_endp_sup[idx] \
                                       .ul_max_aggr_bytes,
                                       ipa_test_engine_cfg_endp_sup[idx] \
                                       .ul_min_aggr_bytes);
        /* Multiple of 1024 K */
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.qmap \
          .max_aggr_bytes /= 1024;
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.qmap \
          .max_aggr_bytes *= 1024;

        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.qmap \
          .max_aggr_pkts = 
          (uint16)ipa_test_generate_random_num((uint32)ipa_test_engine_cfg_endp_sup[idx] \
                                       .ul_max_aggr_pkts,
                                       (uint32)ipa_test_engine_cfg_endp_sup[idx] \
                                       .ul_min_aggr_pkts);
        break;

      case IPA_SIO_HDR_TYPE_MBIM:
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.mbim \
          .max_aggr_bytes = 
          ipa_test_generate_random_num(ipa_test_engine_cfg_endp_sup[idx] \
                                       .ul_max_aggr_bytes,
                                       ipa_test_engine_cfg_endp_sup[idx] \
                                       .ul_min_aggr_bytes);
        /* Multiple of 1024 K */
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.mbim \
          .max_aggr_bytes /= 1024;
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.mbim \
          .max_aggr_bytes *= 1024;

        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.mbim \
          .max_aggr_pkts = 
          (uint16)ipa_test_generate_random_num((uint32)ipa_test_engine_cfg_endp_sup[idx] \
                                               .ul_max_aggr_pkts,
                                               (uint32)ipa_test_engine_cfg_endp_sup[idx] \
                                               .ul_min_aggr_pkts);
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.mbim.ncm_sig = 
          ipa_test_generate_random_num(0xFFFFFFFF, 1);
        params_ptr->endp_cfg.sim_endp_cfg[i].ul_config.params.mbim.ndp_sig = 
          ipa_test_generate_random_num(0xFFFFFFFF, 1);
        break;

      default:
        IPA_TF_ASSERT(0);
    }
  }

  /***********************************************************************
                   Bearer Config Generation 
  ***********************************************************************/
  params_ptr->wan_cfg.num_bearers = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_BEARER_NUM_MAX, 1);

  if (params_ptr->test_type != IPA_TEST_ENGINE_TEST_TYPE_UL)
  {
    params_ptr->wan_cfg.is_ciph_enabled = 
      ipa_test_generate_random_num(TRUE, FALSE);
  }
  else
  {
    params_ptr->wan_cfg.is_ciph_enabled = FALSE;
  }

  params_ptr->wan_cfg.is_bearer_dereg_case = FALSE;
  params_ptr->wan_cfg.is_bearer_susp_case_1 = FALSE;
  params_ptr->wan_cfg.is_bearer_susp_case_2 = FALSE;

  for (i = 0; i < params_ptr->wan_cfg.num_bearers; i++)
  {
    params_ptr->wan_cfg.is_bearer_bridged[i] = 
      ipa_test_generate_random_num(TRUE, FALSE);
    params_ptr->wan_cfg.bearer_tech[i] = (ipa_wan_bearer_tech_e)
      ipa_test_generate_random_num(IPA_WAN_BEARER_TECH_OTHER, 
                                   IPA_WAN_BEARER_TECH_LTE);
  }

  /*params_ptr->is_dpl_enabled = 
    ipa_test_generate_random_num(TRUE, FALSE);*/

  params_ptr->is_dpl_enabled = FALSE;
  params_ptr->is_hw_replication_enabled = FALSE;
  params_ptr->dl_holb_enabled = FALSE;
  params_ptr->ul_holb_enabled = FALSE;
	params_ptr->is_ul_prod_pipe_suspend_enabled = FALSE;

  /* Disable ZIP by default */
  params_ptr->is_zip_enabled = FALSE;
  /* Disable HOLB verification by default */
  params_ptr->zip_cfg.is_holb_ver_enabled = FALSE;
  params_ptr->zip_cfg.err_type = IPA_TEST_ENGINE_ZIP_ERR_MAX;

  /***********************************************************************
                   Packet Config Generation 
  ***********************************************************************/
  params_ptr->ip_pkt_cfg.ip_hdr_len = 0;
  params_ptr->ip_pkt_cfg.num_dsm_items = 1;
  params_ptr->ip_pkt_cfg.payload_len = 0;

  params_ptr->ip_pkt_cfg.version = (ipa_ip_type_e)
    ipa_test_generate_random_num(IPA_IP_TYPE_IPV6, IPA_IP_TYPE_IPV4);

  params_ptr->ip_pkt_cfg.is_random_data =
    ipa_test_generate_random_num(TRUE, FALSE);

  params_ptr->ip_pkt_cfg.num_pkts = 
    ipa_test_generate_random_num(IPA_TEST_ENGINE_IP_PKT_NUM_MAX, 1);

  params_ptr->ip_pkt_cfg.prot = (ipa_test_engine_ip_pkt_prot_e)
		  ipa_test_generate_random_num(IPA_TEST_ENGINE_IP_PKT_PROT_MAX - 1,
                                 IPA_TEST_ENGINE_IP_PKT_PROT_ICMP);

  /*! By default it is inorder fragmentation test case */
  params_ptr->ip_pkt_cfg.params.ooo_frag_case = FALSE;
  params_ptr->ip_pkt_cfg.params.max_pkt_size = IPA_TEST_MAX_PACKET_SIZE;

  /***********************************************************************
                   Filter Config Generation 
  ***********************************************************************/
  //params_ptr->fltr_cfg.is_filter_enabled =
    //ipa_test_generate_random_num(TRUE, FALSE);

  params_ptr->fltr_cfg.is_filter_enabled = FALSE;

  if (params_ptr->fltr_cfg.is_filter_enabled == TRUE)
  {
    //params_ptr->fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
    //params_ptr->fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_MAX;

    /*params_ptr->fltr_cfg.num_dl_sckt_fltr = 
      ipa_test_generate_random_num(IPA_TEST_FILTER_DL_MAX_RULES, 0);
    params_ptr->fltr_cfg.num_ul_qos_fltr = 
      ipa_test_generate_random_num(IPA_TEST_FILTER_UL_QOS_MAX_RULES, 0);
    params_ptr->fltr_cfg.is_tcp_ack_qos_rule = 
      ipa_test_generate_random_num(TRUE, FALSE);*/

	params_ptr->fltr_cfg.num_dl_sckt_fltr = IPA_TEST_FILTER_MAX_DL_V6_RULES + 1;
	params_ptr->fltr_cfg.num_ul_qos_fltr =  IPA_TEST_FILTER_MAX_QOS_V6_RULES + 1;
	params_ptr->fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_UL_QOS;
    params_ptr->fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
  }
  else
  {
    params_ptr->fltr_cfg.fltr_rule_type = IPA_TEST_ENGINE_FLTR_TYPE_TEST_MAX;
    params_ptr->fltr_cfg.fltr_rule_cat_type = IPA_TEST_ENGINE_FLTR_RULE_MAX;
  }

  /***********************************************************************
                   CLAT  Config Generation 
  ***********************************************************************/
  /*! Randomly enable or disable CLAT */
  params_ptr->is_clat_enabled = FALSE;
  /*! Error scenario by default is None */
  params_ptr->clat_cfg.err_scenario = IPA_TEST_ENGINE_CLAT_ERR_NONE;
  /* Randomly generate number of CLAT PDNs according to the number of logical 
     channels generated. */
  params_ptr->clat_cfg.num_clat_pdn = 
    (uint8) ipa_test_generate_random_num((params_ptr->endp_cfg.num_log_ch[0] +
                                         params_ptr->endp_cfg.num_log_ch[1]),0);

  ipa_test_engine_generate_clat_cntxt_arr(&params_ptr->clat_cfg.global_clat_cfg,
                                          params_ptr->clat_cfg.cntxt_info);

  /***********************************************************************
                   Misc Config Generation 
  ***********************************************************************/
  /*! @todo We don't enable Power collapse by default for now */
#ifdef FEATURE_IPA_CTL_CLK_TEST_UC
  params_ptr->is_pc_enabled = TRUE;
#endif
  params_ptr->is_fc_enabled = FALSE;
  params_ptr->rx_pkt_test_type = IPA_TEST_ENGINE_RX_PKT_WAIT;
  params_ptr->deaggr_err_cfg.err_type = IPA_TEST_ENGINE_DEAGGR_ERR_TYPE_MAX;
  //ipa_cfg.ctl.clk_voting_enabled_for_regression = params_ptr->is_pc_enabled;
  return ret_code;
}
#pragma optimize("", on)
