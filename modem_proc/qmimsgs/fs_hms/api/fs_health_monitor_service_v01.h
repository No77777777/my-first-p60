#ifndef FS_HMS_SERVICE_01_H
#define FS_HMS_SERVICE_01_H
/**
  @file fs_health_monitor_service_v01.h

  @brief This is the public header file which defines the fs_hms service Data structures.

  This header file defines the types and structures that were defined in
  fs_hms. It contains the constant values defined, enums, structures,
  messages, and service message IDs (in that order) Structures that were
  defined in the IDL as messages contain mandatory elements, optional
  elements, a combination of mandatory and optional elements (mandatory
  always come before optionals in the structure), or nothing (null message)

  An optional element in a message is preceded by a uint8_t value that must be
  set to true if the element is going to be included. When decoding a received
  message, the uint8_t values will be set to true or false by the decode
  routine, and should be checked before accessing the values that they
  correspond to.

  Variable sized arrays are defined as static sized arrays with an unsigned
  integer (32 bit) preceding it that must be set to the number of elements
  in the array that are valid. For Example:

  uint32_t test_opaque_len;
  uint8_t test_opaque[16];

  If only 4 elements are added to test_opaque[] then test_opaque_len must be
  set to 4 before sending the message.  When decoding, the _len value is set
  by the decode routine and should be checked so that the correct number of
  elements in the array will be accessed.

*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
  Copyright (c) 2020,2021,2022 Qualcomm Technologies, Inc.
 All rights reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.


  $Header: //components/rel/qmimsgs.mpss/4.6.9/fs_hms/api/fs_health_monitor_service_v01.h#1 $
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*
 *THIS IS AN AUTO GENERATED FILE. DO NOT ALTER IN ANY WAY
 *====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/* This file was generated with Tool version 6.14.9 
   It was generated on: Thu Oct 27 2022 (Spin 0)
   From IDL File: fs_health_monitor_service_v01.idl */

/** @defgroup fs_hms_qmi_consts Constant values defined in the IDL */
/** @defgroup fs_hms_qmi_msg_ids Constant values for QMI message IDs */
/** @defgroup fs_hms_qmi_enums Enumerated types used in QMI messages */
/** @defgroup fs_hms_qmi_messages Structures sent as QMI messages */
/** @defgroup fs_hms_qmi_aggregates Aggregate types used in QMI messages */
/** @defgroup fs_hms_qmi_accessor Accessor for QMI service object */
/** @defgroup fs_hms_qmi_version Constant values for versioning information */

#include <stdint.h>
#include "qmi_idl_lib.h"
#include "common_v01.h"


#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup fs_hms_qmi_version
    @{
  */
/** Major Version Number of the IDL used to generate this file */
#define FS_HMS_V01_IDL_MAJOR_VERS 0x01
/** Revision Number of the IDL used to generate this file */
#define FS_HMS_V01_IDL_MINOR_VERS 0x05
/** Major Version Number of the qmi_idl_compiler used to generate this file */
#define FS_HMS_V01_IDL_TOOL_VERS 0x06
/** Maximum Defined Message ID */
#define FS_HMS_V01_MAX_MESSAGE_ID 0x000B
/**
    @}
  */


/** @addtogroup fs_hms_qmi_consts
    @{
  */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_enums
    @{
  */
typedef enum {
  FS_HMS_STATE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  START_V01 = 0, 
  END_V01 = 1, 
  FS_HMS_STATE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}fs_hms_state_v01;
/**
    @}
  */

/** @addtogroup fs_hms_qmi_enums
    @{
  */
typedef enum {
  FS_HMS_IMAGE_TYPE_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  FACTORY_V01 = 0, 
  RUNTIME_V01 = 1, 
  SOFTWARE_ROLL_BACK_V01 = 2, 
  FS_HMS_IMAGE_TYPE_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}fs_hms_image_type_v01;
/**
    @}
  */

/** @addtogroup fs_hms_qmi_enums
    @{
  */
typedef enum {
  FS_HMS_STATUS_MIN_ENUM_VAL_V01 = -2147483647, /**< To force a 32 bit signed enum.  Do not change or use*/
  SUCCESS_V01 = 0, 
  FAILED_V01 = 1, 
  ADDR_VALID_V01 = 2, 
  ADDR_INVALID_V01 = 3, 
  FS_HMS_STATUS_MAX_ENUM_VAL_V01 = 2147483647 /**< To force a 32 bit signed enum.  Do not change or use*/
}fs_hms_status_v01;
/**
    @}
  */

/** @addtogroup fs_hms_qmi_aggregates
    @{
  */
typedef struct {

  fs_hms_state_v01 fs_backup_state;
  /**<   backup state {Start,End}
 */

  uint32_t fs_backup_image_size;
  /**<   size of backup image.
 */

  fs_hms_image_type_v01 fs_backup_type;
  /**<   {Factory, Runtime} - type of backup triggered
 */

  fs_hms_status_v01 fs_backup_status;
  /**<   status , backup process status . only for end case
 */
}fs_hms_backup_ind_type_v01;  /* Type */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_aggregates
    @{
  */
typedef struct {

  fs_hms_state_v01 fs_restore_state;
  /**<   restore state {Start,End}
 */

  fs_hms_status_v01 fs_restore_status;
  /**<   restore status {success,failed}
 */
}fs_hms_restore_ind_type_v01;  /* Type */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_aggregates
    @{
  */
typedef struct {

  fs_hms_status_v01 fs_backup_addr_valid;
  /**<   valid - ADDR_VALID, indicates that the shared memory has been allocated by HLOS,
 and "image_buffer_addr" field is valid (not null).
 */

  uint32_t image_buffer_addr;
  /**<   image_buffer - set if HLOS has allocated memory.
 backup image will be available in this buffer .
 */

  uint32_t scratch_buffer_addr;
  /**<   scratch_buffer- set if HLOS has allocated memory.
  scratch_buffer is used to image operation like compression and encryption.
 */

  uint32_t image_buffer_size;
  /**<   image_buffer_size - size of backup image.
 */

  uint32_t scratch_buffer_size;
  /**<   scratch_buffer_size is the size of scratch buffer.
 */

  uint32_t fs_backup_timer;
  /**<   Timer value in seconds indicating when MPSS should trigger the next request
 if HLOS is not able to allocate shared memory.
 Note that, if this is the case, "valid" will be false here.
 */
}fs_hms_backup_mem_ready_type_v01;  /* Type */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_aggregates
    @{
  */
typedef struct {

  fs_hms_status_v01 fs_restore_addr_valid;
  /**<   valid - ADDR_VALID, indicates that the shared memory has been allocated by HLOS,
 and "image_buffer_addr" field is valid (not null).
 */

  uint32_t image_buffer_addr;
  /**<   image_buffer - set if HLOS has allocated memory.
 backup image will be available in this buffer .
 */

  uint32_t scratch_buffer_addr;
  /**<   scratch_buffer- set if HLOS has allocated memory.
  scratch_buffer is used to image operation like compression and encryption.
 */

  uint32_t image_buffer_size;
  /**<   image_buffer_size - size of backup image.
 */

  uint32_t scratch_buffer_size;
  /**<   scratch_buffer_size is the size of scratch buffer.
 */
}fs_hms_restore_mem_ready_type_v01;  /* Type */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_aggregates
    @{
  */
typedef struct {

  uint32_t max_erase_count;
  /**<   max_erase_count is the maximum erase counter value in EFS.
 */

  uint32_t fs_total_bad_blocks;
  /**<   fs_total_bad_blocks is the total bad blocks count in EFS.
 */

  uint32_t nad_total_bad_blocks;
  /**<   nad_total_bad_blocks is the total bad blocks count in nad_devd.
 */
}fs_hms_block_erase_stats_type_v01;  /* Type */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; Request from client to register for backup notifications */
typedef struct {

  /* Mandatory */
  /*  Applications start fs recovery event */
  uint8_t need_fs_backup_ind;
  /**<   Controls the reporting of backup indication
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_backup_ind_reg_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; Request from client to register for backup notifications */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */
}fs_hms_backup_ind_reg_resp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; Request from client to register for restore notifications */
typedef struct {

  /* Mandatory */
  /*  Applications start fs recovery event */
  uint8_t need_fs_restore_ind;
  /**<   Controls the reporting of QMI_FS_HEALTH_MONITOR_RESTORE_REGISTER_REQ.
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_restore_ind_reg_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; Request from client to register for restore notifications */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */
}fs_hms_restore_ind_reg_resp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Notification Message; Notification from MPSS to HLOS to indicate backup start/end */
typedef struct {

  /* Mandatory */
  /*  fs health monitor status token */
  fs_hms_backup_ind_type_v01 fs_hms_backup_info;
  /**<   structure  that uniquely identifies the current state of fs backup
       operation. */
}fs_hms_backup_ind_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Notification Message; Notification from MPSS to HLOS to indicate restore start/end
    Parameter - {Start,  End}
    Parameter - status - sent only during "End" */
typedef struct {

  /* Mandatory */
  /*  fs health monitor status token */
  fs_hms_restore_ind_type_v01 fs_hms_restore_info;
  /**<   structure  that uniquely identifies the current state of fs restore
       operation. */
}fs_hms_restore_ind_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; response request for backup notification with
                  ram buffer addresses required for backup process. */
typedef struct {

  /* Mandatory */
  /*  Applications start fs recovery event */
  fs_hms_backup_mem_ready_type_v01 fs_hms_backup_mem_ready_info;
  /**<   structure that will be sent to MPSS with required ram buffer, requested
  through backup notification .
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_backup_mem_ready_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; response request for backup notification with
                  ram buffer addresses required for backup process. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */
}fs_hms_backup_mem_ready_resp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; response request for restore notification  with
                  ram buffer addresses required for restore process. */
typedef struct {

  /* Mandatory */
  /*  Applications start fs recovery event */
  fs_hms_restore_mem_ready_type_v01 fs_hms_restore_mem_ready_info;
  /**<   structure that will be sent to MPSS with required ram buffer, requested
     through restore notification .
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_restore_mem_ready_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; response request for restore notification  with
                  ram buffer addresses required for restore process. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */
}fs_hms_restore_mem_ready_resp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; Request from apps to trigger FS partitions backup. */
typedef struct {

  /* Mandatory */
  /*  Applications start fs backup event */
  uint8_t need_trigger_fs_backup;
  /**<   Controls the reporting of fs backup
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_trigger_fs_backup_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; Request from apps to trigger FS partitions backup. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */
}fs_hms_trigger_fs_backup_resp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; Request from apps to trigger FS partitions scrub. */
typedef struct {

  /* Mandatory */
  /*  Applications start fs scrub event */
  uint8_t need_fs_scrub_fs_partitions;
  /**<   Controls the reporting of fs scrub
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_scrub_fs_partitions_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; Request from apps to trigger FS partitions scrub. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */
}fs_hms_scrub_fs_partitions_resp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; request to get block max P/E counter value and bad block stats
                  of fs partitions */
typedef struct {

  /* Mandatory */
  /*  Applications start block_erase_stats */
  uint8_t need_block_erase_stats;
  /**<   structure that will be sent to collect the block_erase_stats.
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_block_erase_stats_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; request to get block max P/E counter value and bad block stats
                  of fs partitions */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */

  /* Mandatory */
  fs_hms_block_erase_stats_type_v01 erase_stats;
}fs_hms_block_erase_stats_resp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; request to set telAf ecall deregrestration time. */
typedef struct {

  /* Mandatory */
  /*  Applications sets telAf ecall deregestration time  */
  uint32_t set_deregestration_time;
  /**<   this variable will send the time to update the nv item on efs.
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_telaf_ecall_set_dereg_time_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; request to set telAf ecall deregrestration time. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */
}fs_hms_telaf_ecall_set_dereg_time_resp_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Request Message; request to get telAf ecall deregrestration time. */
typedef struct {

  /* Mandatory */
  /*  Applications gets telAf ecall deregestration time  */
  uint8_t need_deregestration_time_get;
  /**<   this variable will get the time to update the nv item on efs.
       Values:  \n
       - 0 -- Do not report \n
       - 1 -- Report
  */
}fs_hms_telaf_ecall_get_dereg_time_req_v01;  /* Message */
/**
    @}
  */

/** @addtogroup fs_hms_qmi_messages
    @{
  */
/** Response Message; request to get telAf ecall deregrestration time. */
typedef struct {

  /* Mandatory */
  /*  Result Code */
  qmi_response_type_v01 resp;
  /**<   Standard response type. */

  /* Mandatory */
  uint32_t get_deregestration_time;
  /**<   time value from efs nv item.
  */
}fs_hms_telaf_ecall_get_dereg_time_resp_v01;  /* Message */
/**
    @}
  */

/* Conditional compilation tags for message removal */ 
//#define REMOVE_QMI_FS_HEALTH_MONITOR_BACKUP_IND_V01
//#define REMOVE_QMI_FS_HEALTH_MONITOR_BACKUP_MEM_READY_V01
//#define REMOVE_QMI_FS_HEALTH_MONITOR_BACKUP_REGISTER_V01
//#define REMOVE_QMI_FS_HEALTH_MONITOR_RESTORE_IND_V01
//#define REMOVE_QMI_FS_HEALTH_MONITOR_RESTORE_MEM_READY_V01
//#define REMOVE_QMI_FS_HEALTH_MONITOR_RESTORE_REGISTER_V01
//#define REMOVE_QMI_FS_HEALTH_MONITOR_SCRUB_FS_PARTITIONS_V01
//#define REMOVE_QMI_FS_HEALTH_MONITOR_TRIGGER_FS_BACKUP_V01
//#define REMOVE_QMI_FS_HMS_GET_BLOCK_ERASE_STATS_V01
//#define REMOVE_QMI_FS_HMS_TELAF_ECALL_GET_DEREGISTRATION_TIME_V01
//#define REMOVE_QMI_FS_HMS_TELAF_ECALL_SET_DEREGISTRATION_TIME_V01

/*Service Message Definition*/
/** @addtogroup fs_hms_qmi_msg_ids
    @{
  */
#define QMI_FS_HMS_BACKUP_IND_REG_REQ_V01 0x0001
#define QMI_FS_HMS_BACKUP_IND_REG_RESP_V01 0x0001
#define QMI_FS_HMS_RESTORE_IND_REG_REQ_V01 0x0002
#define QMI_FS_HMS_RESTORE_IND_REG_RESP_V01 0x0002
#define QMI_FS_HMS_FS_BACKUP_IND_V01 0x0003
#define QMI_FS_HMS_FS_RESTORE_IND_V01 0x0004
#define QMI_FS_HMS_BACKUP_MEM_READY_REQ_V01 0x0005
#define QMI_FS_HMS_BACKUP_MEM_READY_RESP_V01 0x0005
#define QMI_FS_HMS_RESTORE_MEM_READY_REQ_V01 0x0006
#define QMI_FS_HMS_RESTORE_MEM_READY_RESP_V01 0x0006
#define QMI_FS_HMS_TRIGGER_FS_BACKUP_REQ_V01 0x0007
#define QMI_FS_HMS_TRIGGER_FS_BACKUP_RESP_V01 0x0007
#define QMI_FS_HMS_SCRUB_FS_PARTI_REQ_V01 0x0008
#define QMI_FS_HMS_SCRUB_FS_PARTI_RESP_V01 0x0008
#define QMI_FS_HMS_GET_BLOCK_ERASE_STATS_REQ_V01 0x0009
#define QMI_FS_HMS_GET_BLOCK_ERASE_STATS_RESP_V01 0x0009
#define QMI_FS_HMS_TELAF_ECALL_SET_DEREG_TIME_REQ_V01 0x000A
#define QMI_FS_HMS_TELAF_ECALL_SET_DEREG_TIME_RESP_V01 0x000A
#define QMI_FS_HMS_TELAF_ECALL_GET_DEREG_TIME_REQ_V01 0x000B
#define QMI_FS_HMS_TELAF_ECALL_GET_DEREG_TIME_RESP_V01 0x000B
/**
    @}
  */

/* Service Object Accessor */
/** @addtogroup wms_qmi_accessor
    @{
  */
/** This function is used internally by the autogenerated code.  Clients should use the
   macro fs_hms_get_service_object_v01( ) that takes in no arguments. */
qmi_idl_service_object_type fs_hms_get_service_object_internal_v01
 ( int32_t idl_maj_version, int32_t idl_min_version, int32_t library_version );

/** This macro should be used to get the service object */
#define fs_hms_get_service_object_v01( ) \
          fs_hms_get_service_object_internal_v01( \
            FS_HMS_V01_IDL_MAJOR_VERS, FS_HMS_V01_IDL_MINOR_VERS, \
            FS_HMS_V01_IDL_TOOL_VERS )
/**
    @}
  */


#ifdef __cplusplus
}
#endif
#endif

