/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

              P H O N E B O O K _ M A N A G E R _ S E R V I C E _ I M P L _ V 0 1  . C

GENERAL DESCRIPTION
  This is the file which defines the pbm service Data structures.

  Copyright (c) 2010-2014, 2016 - 2017, 2019, 2021, 2023 - 2024 Qualcomm Technologies, Inc.
  All rights reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.


  $Header: //components/rel/uim.mpss/6.1.0/pbm/src/phonebook_manager_service_impl_v01.c#3 $
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.14.9
   It was generated on: Fri Oct  4 2024 (Spin 0)
   From IDL File: phonebook_manager_service_v01.idl */

#include "qmi_si.h"
#include "phonebook_manager_service_v01.h"

static const qmi_implemented_optionals pbm_indication_register_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_indication_register_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* reg_mask */
};

static const qmi_implemented_optionals pbm_indication_register_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_indication_register_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbmication_register_reqresp_impl_tuple_v01 = 
{
  &pbm_indication_register_req_msg_impl_v01,
  &pbm_indication_register_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_get_pb_capabilities_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_get_pb_capabilities_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* capability_basic_info */
    QMI_SI_TLV_SET(0x11)   |  /* group_capability */
    QMI_SI_TLV_SET(0x12)   |  /* ad_num_capability */
    QMI_SI_TLV_SET(0x13)   |  /* email_capability */
    QMI_SI_TLV_SET(0x14)   |  /* max_second_name_len */
    QMI_SI_TLV_SET(0x15)   |  /* is_hidden_entry_supported */
    QMI_SI_TLV_SET(0x16)   |  /* gas_capability */
    QMI_SI_TLV_SET(0x17)   |  /* aas_capability */
    QMI_SI_TLV_SET(0x18)   |  /* write_access_info */
    QMI_SI_TLV_SET(0x19)   |  /* num_pb_sets */
    QMI_SI_TLV_SET(0x1A)      /* adn_pb_set_info */
};

static const qmi_implemented_optionals pbm_get_pb_capabilities_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_pb_capabilities_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_pb_capabilities_reqresp_impl_tuple_v01 = 
{
  &pbm_get_pb_capabilities_req_msg_impl_v01,
  &pbm_get_pb_capabilities_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_get_all_pb_capabilities_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* capability_basic_info_array */
    QMI_SI_TLV_SET(0x11)   |  /* group_capability_array */
    QMI_SI_TLV_SET(0x12)   |  /* ad_num_capability_array */
    QMI_SI_TLV_SET(0x13)   |  /* email_capability_array */
    QMI_SI_TLV_SET(0x14)   |  /* second_name_capability_array */
    QMI_SI_TLV_SET(0x15)   |  /* hidden_records_capability_array */
    QMI_SI_TLV_SET(0x16)   |  /* gas_capability_array */
    QMI_SI_TLV_SET(0x17)   |  /* aas_capability_array */
    QMI_SI_TLV_SET(0x18)   |  /* pb_set_info_array */
    QMI_SI_TLV_SET(0x19)   |  /* extended_capability_basic_info_array */
    QMI_SI_TLV_SET(0x1A)   |  /* extended_group_capability_array */
    QMI_SI_TLV_SET(0x1B)   |  /* extended_ad_num_capability_array */
    QMI_SI_TLV_SET(0x1C)   |  /* extended_email_capability_array */
    QMI_SI_TLV_SET(0x1D)   |  /* extended_second_name_capability_array */
    QMI_SI_TLV_SET(0x1E)   |  /* extended_hidden_records_capability_array */
    QMI_SI_TLV_SET(0x1F)   |  /* extended_gas_capability_array */
    QMI_SI_TLV_SET(0x20)   |  /* extended_aas_capability_array */
    QMI_SI_TLV_SET(0x21)      /* extended_pb_set_info_array */
};

static const qmi_implemented_optionals pbm_get_all_pb_capabilities_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_all_pb_capabilities_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_all_pb_capabilities_reqresp_impl_tuple_v01 = 
{
  NULL,
  &pbm_get_all_pb_capabilities_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_read_records_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_read_records_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* num_of_recs */
};

static const qmi_implemented_optionals pbm_read_records_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_read_records_resp_msg_impl_array_v01
};
static const uint32_t pbm_record_read_ind_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* sname_info_array */
    QMI_SI_TLV_SET(0x11)   |  /* ad_num_info_array */
    QMI_SI_TLV_SET(0x12)   |  /* grp_id_info_array */
    QMI_SI_TLV_SET(0x13)   |  /* email_info_array */
    QMI_SI_TLV_SET(0x14)      /* hidden_record_info_array */
};

static const qmi_implemented_optionals pbm_record_read_ind_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_record_read_ind_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_read_records_reqresp_impl_tuple_v01 = 
{
  &pbm_read_records_req_msg_impl_v01,
  &pbm_read_records_resp_msg_impl_v01,
  &pbm_record_read_ind_msg_impl_v01
};

static const uint32_t pbm_write_record_req_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* sname */
    QMI_SI_TLV_SET(0x11)   |  /* ad_num_info */
    QMI_SI_TLV_SET(0x12)   |  /* grp_id */
    QMI_SI_TLV_SET(0x13)   |  /* email_info */
    QMI_SI_TLV_SET(0x14)      /* is_hidden */
};

static const qmi_implemented_optionals pbm_write_record_req_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_write_record_req_msg_impl_array_v01
};
static const uint32_t pbm_write_record_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* record_id */
};

static const qmi_implemented_optionals pbm_write_record_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_write_record_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_write_record_reqresp_impl_tuple_v01 = 
{
  &pbm_write_record_req_msg_impl_v01,
  &pbm_write_record_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_delete_record_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_delete_record_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* record_id */
};

static const qmi_implemented_optionals pbm_delete_record_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_delete_record_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_delete_record_reqresp_impl_tuple_v01 = 
{
  &pbm_delete_record_req_msg_impl_v01,
  &pbm_delete_record_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_delete_all_pb_records_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals pbm_delete_all_pb_records_resp_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_delete_all_pb_records_reqresp_impl_tuple_v01 = 
{
  &pbm_delete_all_pb_records_req_msg_impl_v01,
  &pbm_delete_all_pb_records_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_search_records_req_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* number */
    QMI_SI_TLV_SET(0x11)      /* name */
};

static const qmi_implemented_optionals pbm_search_records_req_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_search_records_req_msg_impl_array_v01
};
static const uint32_t pbm_search_records_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* record_id */
};

static const qmi_implemented_optionals pbm_search_records_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_search_records_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_search_records_reqresp_impl_tuple_v01 = 
{
  &pbm_search_records_req_msg_impl_v01,
  &pbm_search_records_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_record_update_ind_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_record_update_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_record_update_ind_msg_impl_v01
};

static const qmi_implemented_optionals pbm_refresh_ind_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_refresh_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_refresh_ind_msg_impl_v01
};

static const qmi_implemented_optionals pbm_pb_ready_ind_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_pb_ready_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_pb_ready_ind_msg_impl_v01
};

static const uint32_t pbm_emergency_list_ind_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* nv_emer_nums */
    QMI_SI_TLV_SET(0x11)   |  /* card_emer_nums */
    QMI_SI_TLV_SET(0x12)   |  /* network_emer_nums */
    QMI_SI_TLV_SET(0x13)   |  /* emer_nums_extended */
    QMI_SI_TLV_SET(0x14)   |  /* nv_emer_nums_extended */
    QMI_SI_TLV_SET(0x15)   |  /* emer_nums_extended_cat */
    QMI_SI_TLV_SET(0x16)   |  /* nv_emer_nums_extended_cat */
    QMI_SI_TLV_SET(0x17)   |  /* mcc_emer_nums */
    QMI_SI_TLV_SET(0x18)   |  /* elenl_emer_nums */
    QMI_SI_TLV_SET(0x19)   |  /* serving_mcc_emer_nums */
    QMI_SI_TLV_SET(0x1A)   |  /* mcc_emer_nums_sub */
    QMI_SI_TLV_SET(0x1B)      /* serving_mcc_emer_nums_sub */
};

static const qmi_implemented_optionals pbm_emergency_list_ind_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_emergency_list_ind_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_emergency_list_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_emergency_list_ind_msg_impl_v01
};

static const qmi_implemented_optionals pbm_all_pb_init_done_ind_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_all_pb_init_done_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_all_pb_init_done_ind_msg_impl_v01
};

static const uint32_t pbm_get_emergency_list_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* pbm_emer_nums */
    QMI_SI_TLV_SET(0x11)   |  /* pbm_nv_emer_nums */
    QMI_SI_TLV_SET(0x12)   |  /* card_emer_nums */
    QMI_SI_TLV_SET(0x13)   |  /* network_emer_nums */
    QMI_SI_TLV_SET(0x14)   |  /* emer_nums_extended */
    QMI_SI_TLV_SET(0x15)   |  /* nv_emer_nums_extended */
    QMI_SI_TLV_SET(0x16)   |  /* emer_nums_extended_cat */
    QMI_SI_TLV_SET(0x17)   |  /* nv_emer_nums_extended_cat */
    QMI_SI_TLV_SET(0x18)   |  /* mcc_emer_nums */
    QMI_SI_TLV_SET(0x19)   |  /* elenl_emer_nums */
    QMI_SI_TLV_SET(0x1A)   |  /* serving_mcc_emer_nums */
    QMI_SI_TLV_SET(0x1B)   |  /* mcc_emer_nums_sub */
    QMI_SI_TLV_SET(0x1C)      /* serving_mcc_emer_nums_sub */
};

static const qmi_implemented_optionals pbm_get_emergency_list_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_emergency_list_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_emergency_list_reqresp_impl_tuple_v01 = 
{
  NULL,
  &pbm_get_emergency_list_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_get_all_groups_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* groups_data */
};

static const qmi_implemented_optionals pbm_get_all_groups_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_all_groups_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_all_groups_reqresp_impl_tuple_v01 = 
{
  NULL,
  &pbm_get_all_groups_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_set_group_info_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_set_group_info_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* grp_id */
};

static const qmi_implemented_optionals pbm_set_group_info_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_set_group_info_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_set_group_info_reqresp_impl_tuple_v01 = 
{
  &pbm_set_group_info_req_msg_impl_v01,
  &pbm_set_group_info_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_get_pb_state_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_get_pb_state_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* phonebook_state */
};

static const qmi_implemented_optionals pbm_get_pb_state_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_pb_state_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_pb_state_reqresp_impl_tuple_v01 = 
{
  &pbm_get_pb_state_req_msg_impl_v01,
  &pbm_get_pb_state_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_read_all_hidden_records_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_read_all_hidden_records_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* num_of_recs */
};

static const qmi_implemented_optionals pbm_read_all_hidden_records_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_read_all_hidden_records_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_read_all_hidden_records_reqresp_impl_tuple_v01 = 
{
  &pbm_read_all_hidden_records_req_msg_impl_v01,
  &pbm_read_all_hidden_records_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_hidden_record_status_ind_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_hidden_record_status_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_hidden_record_status_ind_msg_impl_v01
};

static const qmi_implemented_optionals pbm_get_next_empty_record_id_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_get_next_empty_record_id_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* record_id */
};

static const qmi_implemented_optionals pbm_get_next_empty_record_id_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_next_empty_record_id_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_next_empty_record_id_reqresp_impl_tuple_v01 = 
{
  &pbm_get_next_empty_record_id_req_msg_impl_v01,
  &pbm_get_next_empty_record_id_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_get_next_non_empty_record_id_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_get_next_non_empty_record_id_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* record_id */
};

static const qmi_implemented_optionals pbm_get_next_non_empty_record_id_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_next_non_empty_record_id_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_next_non_empty_record_id_reqresp_impl_tuple_v01 = 
{
  &pbm_get_next_non_empty_record_id_req_msg_impl_v01,
  &pbm_get_next_non_empty_record_id_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_get_all_aas_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* aas_data */
    QMI_SI_TLV_SET(0x11)      /* extended_aas_data */
};

static const qmi_implemented_optionals pbm_get_all_aas_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_all_aas_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_all_aas_reqresp_impl_tuple_v01 = 
{
  NULL,
  &pbm_get_all_aas_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_set_aas_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_set_aas_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* aas_id */
};

static const qmi_implemented_optionals pbm_set_aas_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_set_aas_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_set_aas_reqresp_impl_tuple_v01 = 
{
  &pbm_set_aas_req_msg_impl_v01,
  &pbm_set_aas_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_aas_update_ind_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* aas_ext */
};

static const qmi_implemented_optionals pbm_aas_update_ind_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_aas_update_ind_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_aas_update_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_aas_update_ind_msg_impl_v01
};

static const uint32_t pbm_gas_update_ind_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* gas_ext */
};

static const qmi_implemented_optionals pbm_gas_update_ind_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_gas_update_ind_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_gas_update_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_gas_update_ind_msg_impl_v01
};

static const qmi_implemented_optionals pbm_bind_subscription_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals pbm_bind_subscription_resp_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_bind_subscription_reqresp_impl_tuple_v01 = 
{
  &pbm_bind_subscription_req_msg_impl_v01,
  &pbm_bind_subscription_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_get_subscription_binding_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* subs_type */
};

static const qmi_implemented_optionals pbm_get_subscription_binding_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_subscription_binding_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_subscription_binding_reqresp_impl_tuple_v01 = 
{
  NULL,
  &pbm_get_subscription_binding_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_read_pbset_caps_req_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* start_id */
    QMI_SI_TLV_SET(0x11)      /* end_id */
};

static const qmi_implemented_optionals pbm_read_pbset_caps_req_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_read_pbset_caps_req_msg_impl_array_v01
};
static const uint32_t pbm_read_pbset_caps_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* num_of_pbsets */
};

static const qmi_implemented_optionals pbm_read_pbset_caps_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_read_pbset_caps_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals pbm_pbset_cap_read_ind_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_read_pbset_caps_reqresp_impl_tuple_v01 = 
{
  &pbm_read_pbset_caps_req_msg_impl_v01,
  &pbm_read_pbset_caps_resp_msg_impl_v01,
  &pbm_pbset_cap_read_ind_msg_impl_v01
};

static const qmi_implemented_optionals pbm_read_records_ext_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_read_records_ext_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* num_of_recs */
};

static const qmi_implemented_optionals pbm_read_records_ext_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_read_records_ext_resp_msg_impl_array_v01
};
static const uint32_t pbm_record_read_ext_ind_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* sname_info_array */
    QMI_SI_TLV_SET(0x11)   |  /* ad_num_info_array */
    QMI_SI_TLV_SET(0x12)   |  /* grp_id_info_array */
    QMI_SI_TLV_SET(0x13)   |  /* email_info_array */
    QMI_SI_TLV_SET(0x14)      /* hidden_record_info_array */
};

static const qmi_implemented_optionals pbm_record_read_ext_ind_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_record_read_ext_ind_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_read_records_ext_reqresp_impl_tuple_v01 = 
{
  &pbm_read_records_ext_req_msg_impl_v01,
  &pbm_read_records_ext_resp_msg_impl_v01,
  &pbm_record_read_ext_ind_msg_impl_v01
};

static const uint32_t pbm_write_record_ext_req_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* num */
    QMI_SI_TLV_SET(0x11)   |  /* name */
    QMI_SI_TLV_SET(0x12)   |  /* sname */
    QMI_SI_TLV_SET(0x13)   |  /* ad_num_info */
    QMI_SI_TLV_SET(0x14)   |  /* grp_id */
    QMI_SI_TLV_SET(0x15)   |  /* email_info */
    QMI_SI_TLV_SET(0x16)      /* is_hidden */
};

static const qmi_implemented_optionals pbm_write_record_ext_req_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_write_record_ext_req_msg_impl_array_v01
};
static const uint32_t pbm_write_record_ext_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* record_id */
};

static const qmi_implemented_optionals pbm_write_record_ext_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_write_record_ext_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_write_record_ext_reqresp_impl_tuple_v01 = 
{
  &pbm_write_record_ext_req_msg_impl_v01,
  &pbm_write_record_ext_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_search_records_ext_req_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* number */
    QMI_SI_TLV_SET(0x11)      /* name */
};

static const qmi_implemented_optionals pbm_search_records_ext_req_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_search_records_ext_req_msg_impl_array_v01
};
static const uint32_t pbm_search_records_ext_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* record_id */
};

static const qmi_implemented_optionals pbm_search_records_ext_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_search_records_ext_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_search_records_ext_reqresp_impl_tuple_v01 = 
{
  &pbm_search_records_ext_req_msg_impl_v01,
  &pbm_search_records_ext_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_read_all_hidden_records_ext_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_read_all_hidden_records_ext_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* num_of_recs */
};

static const qmi_implemented_optionals pbm_read_all_hidden_records_ext_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_read_all_hidden_records_ext_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_read_all_hidden_records_ext_reqresp_impl_tuple_v01 = 
{
  &pbm_read_all_hidden_records_ext_req_msg_impl_v01,
  &pbm_read_all_hidden_records_ext_resp_msg_impl_v01,
  NULL
};

static const qmi_implemented_optionals pbm_sim_init_done_ind_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_sim_init_done_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_sim_init_done_ind_msg_impl_v01
};

static const qmi_implemented_optionals pbm_read_records_ext_undecoded_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const uint32_t pbm_read_records_ext_undecoded_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* num_of_recs */
};

static const qmi_implemented_optionals pbm_read_records_ext_undecoded_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_read_records_ext_undecoded_resp_msg_impl_array_v01
};
static const uint32_t pbm_record_read_ext_undecoded_ind_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* sname_info_array */
    QMI_SI_TLV_SET(0x11)   |  /* ad_num_info_array */
    QMI_SI_TLV_SET(0x12)   |  /* grp_id_info_array */
    QMI_SI_TLV_SET(0x13)   |  /* email_info_array */
    QMI_SI_TLV_SET(0x14)      /* hidden_record_info_array */
};

static const qmi_implemented_optionals pbm_record_read_ext_undecoded_ind_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_record_read_ext_undecoded_ind_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_read_records_ext_undecoded_reqresp_impl_tuple_v01 = 
{
  &pbm_read_records_ext_undecoded_req_msg_impl_v01,
  &pbm_read_records_ext_undecoded_resp_msg_impl_v01,
  &pbm_record_read_ext_undecoded_ind_msg_impl_v01
};

static const qmi_implemented_optionals pbm_set_configuration_req_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals pbm_set_configuration_resp_msg_impl_v01 =
{
  0, /* No Optional TLVs in message */
  0
};
static const qmi_implemented_optionals_tuple qmi_pbm_set_configuration_reqresp_impl_tuple_v01 = 
{
  &pbm_set_configuration_req_msg_impl_v01,
  &pbm_set_configuration_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_get_configuration_req_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* pbm_feature_status_mask */
};

static const qmi_implemented_optionals pbm_get_configuration_req_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_configuration_req_msg_impl_array_v01
};
static const uint32_t pbm_get_configuration_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)      /* centralized_ecc */
};

static const qmi_implemented_optionals pbm_get_configuration_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_configuration_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_configuration_reqresp_impl_tuple_v01 = 
{
  &pbm_get_configuration_req_msg_impl_v01,
  &pbm_get_configuration_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_get_emergency_list_ext_resp_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* pbm_emer_nums */
    QMI_SI_TLV_SET(0x11)   |  /* pbm_nv_emer_nums */
    QMI_SI_TLV_SET(0x12)   |  /* card_emer_nums */
    QMI_SI_TLV_SET(0x13)   |  /* network_emer_nums */
    QMI_SI_TLV_SET(0x14)   |  /* emer_nums_extended */
    QMI_SI_TLV_SET(0x15)   |  /* nv_emer_nums_extended */
    QMI_SI_TLV_SET(0x16)   |  /* emer_nums_extended_cat */
    QMI_SI_TLV_SET(0x17)   |  /* nv_emer_nums_extended_cat */
    QMI_SI_TLV_SET(0x18)   |  /* mcc_emer_nums */
    QMI_SI_TLV_SET(0x19)   |  /* elenl_emer_nums */
    QMI_SI_TLV_SET(0x1A)   |  /* serving_mcc_emer_nums */
    QMI_SI_TLV_SET(0x1B)   |  /* mcc_emer_nums_sub */
    QMI_SI_TLV_SET(0x1C)      /* serving_mcc_emer_nums_sub */
};

static const qmi_implemented_optionals pbm_get_emergency_list_ext_resp_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_get_emergency_list_ext_resp_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_get_emergency_list_ext_reqresp_impl_tuple_v01 = 
{
  NULL,
  &pbm_get_emergency_list_ext_resp_msg_impl_v01,
  NULL
};

static const uint32_t pbm_emergency_list_ext_ind_msg_impl_array_v01[] =
{
    QMI_SI_TLV_SET(0x10)   |  /* nv_emer_nums */
    QMI_SI_TLV_SET(0x11)   |  /* card_emer_nums */
    QMI_SI_TLV_SET(0x12)   |  /* network_emer_nums */
    QMI_SI_TLV_SET(0x13)   |  /* emer_nums_extended */
    QMI_SI_TLV_SET(0x14)   |  /* nv_emer_nums_extended */
    QMI_SI_TLV_SET(0x15)   |  /* emer_nums_extended_cat */
    QMI_SI_TLV_SET(0x16)   |  /* nv_emer_nums_extended_cat */
    QMI_SI_TLV_SET(0x17)   |  /* mcc_emer_nums */
    QMI_SI_TLV_SET(0x18)   |  /* elenl_emer_nums */
    QMI_SI_TLV_SET(0x19)   |  /* serving_mcc_emer_nums */
    QMI_SI_TLV_SET(0x1A)   |  /* mcc_emer_nums_sub */
    QMI_SI_TLV_SET(0x1B)      /* serving_mcc_emer_nums_sub */
};

static const qmi_implemented_optionals pbm_emergency_list_ext_ind_msg_impl_v01 =
{
  1, /* number of elements in array */
  pbm_emergency_list_ext_ind_msg_impl_array_v01
};
static const qmi_implemented_optionals_tuple qmi_pbm_emergency_list_ext_impl_tuple_v01 = 
{
  NULL,
  NULL,
  &pbm_emergency_list_ext_ind_msg_impl_v01
};

static const qmi_implemented_optionals_tuple * const pbm_service_implemented_array_v01[] =
{
    NULL,                                                /* 0x0000 */
    &qmi_pbmication_register_reqresp_impl_tuple_v01,     /* 0x0001 */
    &qmi_pbm_get_pb_capabilities_reqresp_impl_tuple_v01, /* 0x0002 */
    &qmi_pbm_get_all_pb_capabilities_reqresp_impl_tuple_v01, /* 0x0003 */
    &qmi_pbm_read_records_reqresp_impl_tuple_v01,        /* 0x0004 */
    &qmi_pbm_write_record_reqresp_impl_tuple_v01,        /* 0x0005 */
    &qmi_pbm_delete_record_reqresp_impl_tuple_v01,       /* 0x0006 */
    &qmi_pbm_delete_all_pb_records_reqresp_impl_tuple_v01, /* 0x0007 */
    &qmi_pbm_search_records_reqresp_impl_tuple_v01,      /* 0x0008 */
    &qmi_pbm_record_update_impl_tuple_v01,               /* 0x0009 */
    &qmi_pbm_refresh_impl_tuple_v01,                     /* 0x000A */
    &qmi_pbm_pb_ready_impl_tuple_v01,                    /* 0x000B */
    &qmi_pbm_emergency_list_impl_tuple_v01,              /* 0x000C */
    &qmi_pbm_all_pb_init_done_impl_tuple_v01,            /* 0x000D */
    &qmi_pbm_get_emergency_list_reqresp_impl_tuple_v01,  /* 0x000E */
    &qmi_pbm_get_all_groups_reqresp_impl_tuple_v01,      /* 0x000F */
    &qmi_pbm_set_group_info_reqresp_impl_tuple_v01,      /* 0x0010 */
    &qmi_pbm_get_pb_state_reqresp_impl_tuple_v01,        /* 0x0011 */
    &qmi_pbm_read_all_hidden_records_reqresp_impl_tuple_v01, /* 0x0012 */
    &qmi_pbm_hidden_record_status_impl_tuple_v01,        /* 0x0013 */
    &qmi_pbm_get_next_empty_record_id_reqresp_impl_tuple_v01, /* 0x0014 */
    &qmi_pbm_get_next_non_empty_record_id_reqresp_impl_tuple_v01, /* 0x0015 */
    &qmi_pbm_get_all_aas_reqresp_impl_tuple_v01,         /* 0x0016 */
    &qmi_pbm_set_aas_reqresp_impl_tuple_v01,             /* 0x0017 */
    &qmi_pbm_aas_update_impl_tuple_v01,                  /* 0x0018 */
    &qmi_pbm_gas_update_impl_tuple_v01,                  /* 0x0019 */
    &qmi_pbm_bind_subscription_reqresp_impl_tuple_v01,   /* 0x001A */
    &qmi_pbm_get_subscription_binding_reqresp_impl_tuple_v01, /* 0x001B */
    &qmi_pbm_read_pbset_caps_reqresp_impl_tuple_v01,     /* 0x001C */
    &qmi_pbm_read_records_ext_reqresp_impl_tuple_v01,    /* 0x001D */
    &qmi_pbm_write_record_ext_reqresp_impl_tuple_v01,    /* 0x001E */
    &qmi_pbm_search_records_ext_reqresp_impl_tuple_v01,  /* 0x001F */
    &qmi_pbm_read_all_hidden_records_ext_reqresp_impl_tuple_v01, /* 0x0020 */
    &qmi_pbm_sim_init_done_impl_tuple_v01,               /* 0x0021 */
    &qmi_pbm_read_records_ext_undecoded_reqresp_impl_tuple_v01, /* 0x0022 */
    &qmi_pbm_set_configuration_reqresp_impl_tuple_v01,   /* 0x0023 */
    &qmi_pbm_get_configuration_reqresp_impl_tuple_v01,   /* 0x0024 */
    &qmi_pbm_get_emergency_list_ext_reqresp_impl_tuple_v01, /* 0x0025 */
    &qmi_pbm_emergency_list_ext_impl_tuple_v01           /* 0x0026 */
};

static const qmi_implemented_messages pbm_service_implemented_v01 =
{
  0x0026,  /* maximum message ID in this service */
  pbm_service_implemented_array_v01
};

const qmi_implemented_messages *pbm_get_service_impl_v01 (void)
{
  return &pbm_service_implemented_v01;
}
