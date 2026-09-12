#ifndef HEAPANALYZER_H
#define HEAPANALYZER_H

/*!
  @file
  heap_analyzer.h

  @brief
  Header file for data structure defintions for heap analyzer

*/

/*===========================================================================

  Copyright (c) 2009-2014 QUALCOMM Technologies Incorporated. All Rights Reserved

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

===========================================================================*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.mpss/10.0/services/utils/src/heapanalyzer.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
07/24/15   ps      Added file to support  heap analyzer view in QXDM

===========================================================================*/

/*===========================================================================

                           INCLUDE FILES

===========================================================================*/
#include "memheap.h"
#include "log.h"
#include "diagpkt.h"        /* for subsystem commands      */
#include "diagcmd.h"        /* for subsystem commands      */
#include "qurt_thread.h"

#define MAX_HEAP_TASKS 16

//Temp remove later
#define HEAP_ANALYZER_MODEM_PROC

#define HA_MAX_TASK_PROFILE 8
#define HA_MAX_TASK_NAME_LENGTH 16
#define HA_DEFAULT_DURATION 500
#define HA_MIN_LOG_COLLECT_TIMER 100

/* Assuming maximum commands supported in heapanalyzer willl be 32 per processor */
#if defined(HEAP_ANALYZER_MODEM_PROC)
#define HEAP_ANALYZER_CMD_BASE 0
#elif defined(HEAP_ANALYZER_ADSP_PROC)
#define HEAP_ANALYZER_CMD_BASE 32
#endif

#define GET_HEAPS_INFO (HEAP_ANALYZER_CMD_BASE)
#define GET_HEAP_TASK_NAMES (HEAP_ANALYZER_CMD_BASE+1)
#define SET_HEAP_PROFILE_TASKS (HEAP_ANALYZER_CMD_BASE+2)
#define SET_HEAP_PROFILE_TOP_TASKS (HEAP_ANALYZER_CMD_BASE+3)
#define SET_PROFILE_TIME (HEAP_ANALYZER_CMD_BASE+4)
#define STOP_PROFILE (HEAP_ANALYZER_CMD_BASE+5)

#define HEAP_ANALYZER_HEAP_INFO_LOG_VERSION 1
#define HEAP_ANALYZER_TASK_INFO_LOG_VERSION 1

/* For modem 

GET_HEAPS_INFO -            send_data 0x4B 0x6D 0x00 0x00
GET_HEAP_TASK_NAMES -  send_data 0x4B 0x6D 0x01 0x00
SET_HEAP_PROFILE_TASKS            send_data 0x4B 0x6D 0x02 0x00
SET_HEAP_PROFILE_TOP_TASKS                   send_data 0x4B 0x6D 0x03 0x00 0x00 0x00 0x02 0x00 (number of tasks 0x2)
SET_PROFILE_TIME send_data                      send_data 0x4B 0x6D 0x04 0x00 0x00 0x10 (to set time to 0x1000 ticks)
STOP_PROFILE -                send_data 0x4B 0x6D 0x05 0x00

*/

typedef struct 
{
  boolean is_enabled;
  uint16 heap_id;
  uint16 profile_type;
  uint16 num_tasks;
  uint16 task_ids[HA_MAX_TASK_PROFILE];
}ha_profile_type;


typedef PACK(struct)
{
  /* log code header */
  log_hdr_type  header;
  /* log packet version */
  uint8 version;
  /* Heap Identifier */
  uint16 heap_id;
  /* Number of sections in the heap */
   uint16 number_of_sections;
  /* Total blocks in the heap. */
  unsigned long total_blocks;
  /* Total bytes available in the heap. */
  unsigned long total_bytes;
  /* Amount of bytes in use in the heap. */
  unsigned long used_bytes;
  /* Number of bytes used by clients and heap headers */
  unsigned long heap_used_bytes;
  /* Maximum amount that has been used in the heap. */
  unsigned long max_used;
  /* Maximum amount that has been requested in the heap. */
  unsigned long max_request;
  /* Track fragmentation factor */
  uint32 fragmentation_factor;
  /* Largest block on the heap */
  uint32 largest_free_block_size;
}heap_log_info_type;


typedef PACK(struct)
{
  /* log code header */
  log_hdr_type  header;
  /* log packet version */
  uint8 version; 
  /* Heap Identifier */
  uint16 heap_id;
  /* Internal task identifier (TLS entry) */
  uint16 task_id;
  /* Amount of heap memory currently used by this task */
  uint32 current_usage;
  /* Maximum amount of heap memory used by this task at any time. */
  uint32 max_usage;
  /* Heap quota allocated to the task */
  uint32 quota;
}task_log_info_type;

/*==========================================================================
Heap analyzer diag subsystem commands request and response
============================================================================*/

/*---------------------------------------------------------------------------
    GET_HEAPS_INFO 
---------------------------------------------------------------------------*/
typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
} get_heaps_info_req_type;

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16  num_heaps; /* number of heaps < 31*/
} get_heaps_info_rsp_type;

typedef struct {
 uint16 heap_id;
 char heap_name[HEAP_NAME_SIZE];
}heap_id_name_t;

/*---------------------------------------------------------------------------
    GET_HEAP_TASK_NAMES 
---------------------------------------------------------------------------*/
typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16 heap_id;
} get_heap_task_names_req_type;

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16  num_tasks; /* number of heaps < 31*/
} get_heap_task_names_rsp_type;

typedef struct {
  uint16 task_id;
  char task_name[QURT_THREAD_ATTR_NAME_MAXLEN];
}task_id_name_t;


/*---------------------------------------------------------------------------
    SET_HEAP_PROFILE_TOP_TASKS 
---------------------------------------------------------------------------*/

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16 heap_id;
  uint16 num_top_tasks;
} set_heap_profile_top_tasks_req_type;

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16 response;
} set_heap_profile_top_tasks_rsp_type;

/*---------------------------------------------------------------------------
    SET_HEAP_PROFILE_TASKS 
---------------------------------------------------------------------------*/

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16 heap_id;
  uint16 num_tasks;
  uint16 task_ids[16]; //Task IDs of tasks to profile
} set_heap_profile_tasks_req_type;

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16 response;
} set_heap_profile_tasks_rsp_type;

/*---------------------------------------------------------------------------
    SET_PROFILE_TIME 
---------------------------------------------------------------------------*/

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16 log_collect_time;
} set_heap_profile_time_req_type;

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16 response;
} set_heap_profile_time_rsp_type;

/*---------------------------------------------------------------------------
    STOP_PROFILE 
---------------------------------------------------------------------------*/

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
} stop_heap_profile_req_type;

typedef PACK(struct) {
  diagpkt_subsys_header_type hdr;
  uint16 response;
} stop_heap_profile_rsp_type;


/*===========================================================================
FUNCTION heap_analyzer_init()

DESCRIPTION
  Initialize the heap analyzer

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void heap_analyzer_init( void);

#endif /* HEAPANALYZER_H  */
