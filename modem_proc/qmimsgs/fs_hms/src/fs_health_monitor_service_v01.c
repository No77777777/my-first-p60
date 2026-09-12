/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                        F S _ H E A L T H _ M O N I T O R _ S E R V I C E _ V 0 1  . C

GENERAL DESCRIPTION
  This is the file which defines the fs_hms service Data structures.

  Copyright (c) 2020,2021,2022 Qualcomm Technologies, Inc.
 All rights reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.


  $Header: //components/rel/qmimsgs.mpss/4.6.9/fs_hms/src/fs_health_monitor_service_v01.c#1 $
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.14.9 
   It was generated on: Thu Oct 27 2022 (Spin 0)
   From IDL File: fs_health_monitor_service_v01.idl */

#include "stdint.h"
#include "qmi_idl_lib_internal.h"
#include "fs_health_monitor_service_v01.h"
#include "common_v01.h"


/*Type Definitions*/
static const uint8_t fs_hms_backup_ind_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(fs_hms_backup_ind_type_v01, fs_backup_state),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_backup_ind_type_v01, fs_backup_image_size),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(fs_hms_backup_ind_type_v01, fs_backup_type),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(fs_hms_backup_ind_type_v01, fs_backup_status),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t fs_hms_restore_ind_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(fs_hms_restore_ind_type_v01, fs_restore_state),

  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(fs_hms_restore_ind_type_v01, fs_restore_status),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t fs_hms_backup_mem_ready_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(fs_hms_backup_mem_ready_type_v01, fs_backup_addr_valid),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_backup_mem_ready_type_v01, image_buffer_addr),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_backup_mem_ready_type_v01, scratch_buffer_addr),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_backup_mem_ready_type_v01, image_buffer_size),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_backup_mem_ready_type_v01, scratch_buffer_size),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_backup_mem_ready_type_v01, fs_backup_timer),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t fs_hms_restore_mem_ready_type_data_v01[] = {
  QMI_IDL_1_BYTE_ENUM,
  QMI_IDL_OFFSET8(fs_hms_restore_mem_ready_type_v01, fs_restore_addr_valid),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_restore_mem_ready_type_v01, image_buffer_addr),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_restore_mem_ready_type_v01, scratch_buffer_addr),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_restore_mem_ready_type_v01, image_buffer_size),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_restore_mem_ready_type_v01, scratch_buffer_size),

  QMI_IDL_FLAG_END_VALUE
};

static const uint8_t fs_hms_block_erase_stats_type_data_v01[] = {
  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_block_erase_stats_type_v01, max_erase_count),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_block_erase_stats_type_v01, fs_total_bad_blocks),

  QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_block_erase_stats_type_v01, nad_total_bad_blocks),

  QMI_IDL_FLAG_END_VALUE
};

/*Message Definitions*/
static const uint8_t fs_hms_backup_ind_reg_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(fs_hms_backup_ind_reg_req_v01, need_fs_backup_ind)
};

static const uint8_t fs_hms_backup_ind_reg_resp_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_backup_ind_reg_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0)
};

static const uint8_t fs_hms_restore_ind_reg_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(fs_hms_restore_ind_reg_req_v01, need_fs_restore_ind)
};

static const uint8_t fs_hms_restore_ind_reg_resp_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_restore_ind_reg_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0)
};

static const uint8_t fs_hms_backup_ind_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_backup_ind_v01, fs_hms_backup_info),
  QMI_IDL_TYPE88(0, 0)
};

static const uint8_t fs_hms_restore_ind_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_restore_ind_v01, fs_hms_restore_info),
  QMI_IDL_TYPE88(0, 1)
};

static const uint8_t fs_hms_backup_mem_ready_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_backup_mem_ready_req_v01, fs_hms_backup_mem_ready_info),
  QMI_IDL_TYPE88(0, 2)
};

static const uint8_t fs_hms_backup_mem_ready_resp_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_backup_mem_ready_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0)
};

static const uint8_t fs_hms_restore_mem_ready_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_restore_mem_ready_req_v01, fs_hms_restore_mem_ready_info),
  QMI_IDL_TYPE88(0, 3)
};

static const uint8_t fs_hms_restore_mem_ready_resp_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_restore_mem_ready_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0)
};

static const uint8_t fs_hms_trigger_fs_backup_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(fs_hms_trigger_fs_backup_req_v01, need_trigger_fs_backup)
};

static const uint8_t fs_hms_trigger_fs_backup_resp_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_trigger_fs_backup_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0)
};

static const uint8_t fs_hms_scrub_fs_partitions_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(fs_hms_scrub_fs_partitions_req_v01, need_fs_scrub_fs_partitions)
};

static const uint8_t fs_hms_scrub_fs_partitions_resp_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_scrub_fs_partitions_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0)
};

static const uint8_t fs_hms_block_erase_stats_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(fs_hms_block_erase_stats_req_v01, need_block_erase_stats)
};

static const uint8_t fs_hms_block_erase_stats_resp_data_v01[] = {
  0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_block_erase_stats_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0),

  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x03,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_block_erase_stats_resp_v01, erase_stats),
  QMI_IDL_TYPE88(0, 4)
};

static const uint8_t fs_hms_telaf_ecall_set_dereg_time_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_telaf_ecall_set_dereg_time_req_v01, set_deregestration_time)
};

static const uint8_t fs_hms_telaf_ecall_set_dereg_time_resp_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_telaf_ecall_set_dereg_time_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0)
};

static const uint8_t fs_hms_telaf_ecall_get_dereg_time_req_data_v01[] = {
  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x01,
   QMI_IDL_GENERIC_1_BYTE,
  QMI_IDL_OFFSET8(fs_hms_telaf_ecall_get_dereg_time_req_v01, need_deregestration_time_get)
};

static const uint8_t fs_hms_telaf_ecall_get_dereg_time_resp_data_v01[] = {
  0x02,
   QMI_IDL_AGGREGATE,
  QMI_IDL_OFFSET8(fs_hms_telaf_ecall_get_dereg_time_resp_v01, resp),
  QMI_IDL_TYPE88(1, 0),

  QMI_IDL_TLV_FLAGS_LAST_TLV | 0x03,
   QMI_IDL_GENERIC_4_BYTE,
  QMI_IDL_OFFSET8(fs_hms_telaf_ecall_get_dereg_time_resp_v01, get_deregestration_time)
};

/* Type Table */
static const qmi_idl_type_table_entry  fs_hms_type_table_v01[] = {
  {sizeof(fs_hms_backup_ind_type_v01), fs_hms_backup_ind_type_data_v01},
  {sizeof(fs_hms_restore_ind_type_v01), fs_hms_restore_ind_type_data_v01},
  {sizeof(fs_hms_backup_mem_ready_type_v01), fs_hms_backup_mem_ready_type_data_v01},
  {sizeof(fs_hms_restore_mem_ready_type_v01), fs_hms_restore_mem_ready_type_data_v01},
  {sizeof(fs_hms_block_erase_stats_type_v01), fs_hms_block_erase_stats_type_data_v01}
};

/* Message Table */
static const qmi_idl_message_table_entry fs_hms_message_table_v01[] = {
  {sizeof(fs_hms_backup_ind_reg_req_v01), fs_hms_backup_ind_reg_req_data_v01},
  {sizeof(fs_hms_backup_ind_reg_resp_v01), fs_hms_backup_ind_reg_resp_data_v01},
  {sizeof(fs_hms_restore_ind_reg_req_v01), fs_hms_restore_ind_reg_req_data_v01},
  {sizeof(fs_hms_restore_ind_reg_resp_v01), fs_hms_restore_ind_reg_resp_data_v01},
  {sizeof(fs_hms_backup_ind_v01), fs_hms_backup_ind_data_v01},
  {sizeof(fs_hms_restore_ind_v01), fs_hms_restore_ind_data_v01},
  {sizeof(fs_hms_backup_mem_ready_req_v01), fs_hms_backup_mem_ready_req_data_v01},
  {sizeof(fs_hms_backup_mem_ready_resp_v01), fs_hms_backup_mem_ready_resp_data_v01},
  {sizeof(fs_hms_restore_mem_ready_req_v01), fs_hms_restore_mem_ready_req_data_v01},
  {sizeof(fs_hms_restore_mem_ready_resp_v01), fs_hms_restore_mem_ready_resp_data_v01},
  {sizeof(fs_hms_trigger_fs_backup_req_v01), fs_hms_trigger_fs_backup_req_data_v01},
  {sizeof(fs_hms_trigger_fs_backup_resp_v01), fs_hms_trigger_fs_backup_resp_data_v01},
  {sizeof(fs_hms_scrub_fs_partitions_req_v01), fs_hms_scrub_fs_partitions_req_data_v01},
  {sizeof(fs_hms_scrub_fs_partitions_resp_v01), fs_hms_scrub_fs_partitions_resp_data_v01},
  {sizeof(fs_hms_block_erase_stats_req_v01), fs_hms_block_erase_stats_req_data_v01},
  {sizeof(fs_hms_block_erase_stats_resp_v01), fs_hms_block_erase_stats_resp_data_v01},
  {sizeof(fs_hms_telaf_ecall_set_dereg_time_req_v01), fs_hms_telaf_ecall_set_dereg_time_req_data_v01},
  {sizeof(fs_hms_telaf_ecall_set_dereg_time_resp_v01), fs_hms_telaf_ecall_set_dereg_time_resp_data_v01},
  {sizeof(fs_hms_telaf_ecall_get_dereg_time_req_v01), fs_hms_telaf_ecall_get_dereg_time_req_data_v01},
  {sizeof(fs_hms_telaf_ecall_get_dereg_time_resp_v01), fs_hms_telaf_ecall_get_dereg_time_resp_data_v01}
};

/* Range Table */
/* No Ranges Defined in IDL */

/* Predefine the Type Table Object */
static const qmi_idl_type_table_object fs_hms_qmi_idl_type_table_object_v01;

/*Referenced Tables Array*/
static const qmi_idl_type_table_object *fs_hms_qmi_idl_type_table_object_referenced_tables_v01[] =
{&fs_hms_qmi_idl_type_table_object_v01, &common_qmi_idl_type_table_object_v01};

/*Type Table Object*/
static const qmi_idl_type_table_object fs_hms_qmi_idl_type_table_object_v01 = {
  sizeof(fs_hms_type_table_v01)/sizeof(qmi_idl_type_table_entry ),
  sizeof(fs_hms_message_table_v01)/sizeof(qmi_idl_message_table_entry),
  1,
  fs_hms_type_table_v01,
  fs_hms_message_table_v01,
  fs_hms_qmi_idl_type_table_object_referenced_tables_v01,
  NULL
};

/*Arrays of service_message_table_entries for commands, responses and indications*/
static const qmi_idl_service_message_table_entry fs_hms_service_command_messages_v01[] = {
  {QMI_FS_HMS_BACKUP_IND_REG_REQ_V01, QMI_IDL_TYPE16(0, 0), 4},
  {QMI_FS_HMS_RESTORE_IND_REG_REQ_V01, QMI_IDL_TYPE16(0, 2), 4},
  {QMI_FS_HMS_BACKUP_MEM_READY_REQ_V01, QMI_IDL_TYPE16(0, 6), 24},
  {QMI_FS_HMS_RESTORE_MEM_READY_REQ_V01, QMI_IDL_TYPE16(0, 8), 20},
  {QMI_FS_HMS_TRIGGER_FS_BACKUP_REQ_V01, QMI_IDL_TYPE16(0, 10), 4},
  {QMI_FS_HMS_SCRUB_FS_PARTI_REQ_V01, QMI_IDL_TYPE16(0, 12), 4},
  {QMI_FS_HMS_GET_BLOCK_ERASE_STATS_REQ_V01, QMI_IDL_TYPE16(0, 14), 4},
  {QMI_FS_HMS_TELAF_ECALL_SET_DEREG_TIME_REQ_V01, QMI_IDL_TYPE16(0, 16), 7},
  {QMI_FS_HMS_TELAF_ECALL_GET_DEREG_TIME_REQ_V01, QMI_IDL_TYPE16(0, 18), 4}
};

static const qmi_idl_service_message_table_entry fs_hms_service_response_messages_v01[] = {
  {QMI_FS_HMS_BACKUP_IND_REG_RESP_V01, QMI_IDL_TYPE16(0, 1), 7},
  {QMI_FS_HMS_RESTORE_IND_REG_RESP_V01, QMI_IDL_TYPE16(0, 3), 7},
  {QMI_FS_HMS_BACKUP_MEM_READY_RESP_V01, QMI_IDL_TYPE16(0, 7), 7},
  {QMI_FS_HMS_RESTORE_MEM_READY_RESP_V01, QMI_IDL_TYPE16(0, 9), 7},
  {QMI_FS_HMS_TRIGGER_FS_BACKUP_RESP_V01, QMI_IDL_TYPE16(0, 11), 7},
  {QMI_FS_HMS_SCRUB_FS_PARTI_RESP_V01, QMI_IDL_TYPE16(0, 13), 7},
  {QMI_FS_HMS_GET_BLOCK_ERASE_STATS_RESP_V01, QMI_IDL_TYPE16(0, 15), 22},
  {QMI_FS_HMS_TELAF_ECALL_SET_DEREG_TIME_RESP_V01, QMI_IDL_TYPE16(0, 17), 7},
  {QMI_FS_HMS_TELAF_ECALL_GET_DEREG_TIME_RESP_V01, QMI_IDL_TYPE16(0, 19), 14}
};

static const qmi_idl_service_message_table_entry fs_hms_service_indication_messages_v01[] = {
  {QMI_FS_HMS_FS_BACKUP_IND_V01, QMI_IDL_TYPE16(0, 4), 10},
  {QMI_FS_HMS_FS_RESTORE_IND_V01, QMI_IDL_TYPE16(0, 5), 5}
};

/*Service Object*/
struct qmi_idl_service_object fs_hms_qmi_idl_service_object_v01 = {
  0x06,
  0x01,
  0x54,
  24,
  { sizeof(fs_hms_service_command_messages_v01)/sizeof(qmi_idl_service_message_table_entry),
    sizeof(fs_hms_service_response_messages_v01)/sizeof(qmi_idl_service_message_table_entry),
    sizeof(fs_hms_service_indication_messages_v01)/sizeof(qmi_idl_service_message_table_entry) },
  { fs_hms_service_command_messages_v01, fs_hms_service_response_messages_v01, fs_hms_service_indication_messages_v01},
  &fs_hms_qmi_idl_type_table_object_v01,
  0x05,
  NULL
};

/* Service Object Accessor */
qmi_idl_service_object_type fs_hms_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version ){
  if ( FS_HMS_V01_IDL_MAJOR_VERS != idl_maj_version || FS_HMS_V01_IDL_MINOR_VERS != idl_min_version
       || FS_HMS_V01_IDL_TOOL_VERS != library_version)
  {
    return NULL;
  }
  return (qmi_idl_service_object_type)&fs_hms_qmi_idl_service_object_v01;
}

