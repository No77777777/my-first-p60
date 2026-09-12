#ifndef LOGGING_INT_H
#define LOGGING_INT_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                           Logging Changes

General Description
This files contains logging changes.


Copyright (c) 2006 - 2016 by QUALCOMM Technologies, Incorporated.  All Rights Reserved.
============================================================================

   $Header: //components/rel/wcdma.mpss/8.4.0/common/src/logging_int.h#2 $
   $DateTime: 2020/01/22 08:56:27 $

                           Edit History

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/09/17 as      Added NV control for diag voting
04/03/17 as      Added changes for MCA vote when diag is active
05/18/17    nr      Made changes to add NV 73993 for modem log util task
01/25/17    sk      Add logging changes.

===========================================================================*/

#include "customer.h"
#include "diagcmd.h" 
#include "dog_hb_rex.h"
#include "comdef.h"
#include "customer.h"
#include "msg.h"
#include "err.h"
#include "queue.h"
#include "rex.h"
#include "event.h"
#include "sys.h"
#include "log.h"


#define MAX_MODEM_TASK_CMDS 3

#define LOG_MEM_POOL_SIZE  20480 // 20 KB

#define LOG_UTIL_TASK_MEM_POOL_SIZE  2048 // 2KB

#define LOG_UTIL_NULL           ((void*)0)                                         // return result
#define LOG_UTIL_ZERO           (0)                                                // return result


/* Signal for internal RRC commands within the RRC task */
#define LOG_UTIL_INPUT_CMD_Q_SIG    0x0001 /*required*/

/* Signal to send the watchdog report */
#define LOG_UTIL_RPT_TIMER_SIG    0x0002  /*required*/

#define LOG_UTIL_DIAG_MASK_CHANGE    0x0004  /*required*/


#define LOG_UTIL_DEBUG_COUNT 10

#define MAX_NUM_LOGS_EVENT_BASED_LOGGING 12000

#define MAX_NUM_EVENT_BASED_LOG_INDEX_PER_TABLE 5000

#define MAX_NUM_LOGS_TO_BE_FLUSHED 2000

//#define MAX_NUM__EVENT_BASED_LOG_INDEX_COMMON  2000

#define INVALID_LOG_PACKET_SIZE 0xFFFFFFFF

#define TECH_MASK        0xF000
#define COMMON_LOG_MASK  0x1000
#define WCDMA_LOG_MASK   0x4000
#define GERAN_LOG_MASK   0x5000
#define UMTS_LOG_MASK    0x7000
#define LTE_LOG_MASK     0xB000
#define TDSCDMA_LOG_MASK 0xD000

typedef struct
{
  uint32 log_code;
  void* log_ptr;
  uint8 as_id;
  uint16 sub_common_index;
}event_based_logging_code_ptr;

typedef struct
{
  event_based_logging_code_ptr debug_log_ptr[MAX_NUM_LOGS_EVENT_BASED_LOGGING];
  uint16 dbg_ptr_next_valid_index_to_be_written;
  uint16 dbg_ptr_first_valid_index_to_be_freed;
}event_based_logging_type;

typedef struct
{
  uint16 main_index;
  uint32 filter;
}event_based_logging_index_type;

typedef struct
{
  event_based_logging_index_type event_based_table[MAX_NUM_EVENT_BASED_LOG_INDEX_PER_TABLE];
  uint16 dbg_ptr_next_valid_index_to_be_written;
  uint16 dbg_ptr_first_valid_index_to_be_freed;
}event_based_logging_table_type;

typedef enum
{
  LOG_UTIL_BUFFER_LOG_PACKET,
  LOG_UTIL_FLUSH_LOG_PACKET
}log_util_cmd_e_type;

typedef struct
{
  q_link_type   link;        /* Queue link     */
  log_util_cmd_e_type log_util_cmd_id;     /* The command Id */

}log_util_cmd_hdr_type;

typedef struct
{
  /* Command header common for all commands */
  void *log_ptr;
  uint16 log_packet_no;
  uint16 log_size;
  uint8 as_id;
}log_util_buffer_log_packet_type;

typedef struct
{
  uint32 client_id;
  uint16 event_id;
  uint8 as_id; 
}log_util_flush_log_packet_type;

typedef union
{
  /* Command header common for all commands */
  log_util_buffer_log_packet_type log_packet;
  log_util_flush_log_packet_type log_flush;
  
}log_util_cmd_payload_type;

typedef struct
{
  /* Command header common for all commands */
  log_util_cmd_hdr_type    cmd_hdr;

  /* Command header common for all commands */
  log_util_cmd_payload_type cmd_payload;
}log_util_cmd_type;


typedef struct
{
  int64 timeticks; 
  uint8 diag_mode;
  boolean diag_mask_status;
  uint32  log_util_nv;
}log_util_debug_mask_change_type;


typedef struct
{
  log_util_debug_mask_change_type mask_change[LOG_UTIL_DEBUG_COUNT];
  /*Index maintains the next buf to be filled*/
  uint8 log_util_crash_debug_index;
}log_util_crash_debug_type;


/*===========================================================================
   FUNCTION     log_util_read_nvs
  
   DESCRIPTION
   This function reads the log util nvs
  
   DEPENDENCIES
     None.
  
   PARAMETERS
  
   RETURN VALUE
     None.
  
   SIDE EFFECTS
  
  ===========================================================================*/

void log_util_read_nvs(void);

void log_util_task_main(void);
void log_util_dispatcher(void);
void logging_dog_report_and_clear_sig(void);

#ifdef FEATURE_QSH_EVENT_NOTIFY_HANDLER
#error code not present
#endif



/*===========================================================================

FUNCTION log_util_diag_clock_vote_init

DESCRIPTION
  Registration with diag for diag mask chagne update

===========================================================================*/
void log_util_diag_clock_vote_init(void);

/*===========================================================================

FUNCTION log_util_diag_registration

DESCRIPTION
  Registration with diag for diag mask chagne update

===========================================================================*/
void log_util_diag_registration(void);


/*===========================================================================

FUNCTION log_util_mca_init

DESCRIPTION
  Registration with mca for voting

===========================================================================*/
void log_util_mca_init(void);

/*===========================================================================

FUNCTION log_util_diag_mask_change

DESCRIPTION
  takes care of voting for clock when diag mask is enabled and releasing vote if diag mask is not enabled

===========================================================================*/
void log_util_diag_mask_change(void);

/*===========================================================================

FUNCTION log_util_npa_request_vote_for_logging

DESCRIPTION
  This procedure votes against sleep for the Diag task.

===========================================================================*/
void log_util_npa_request_vote_for_logging(void);

/*===========================================================================

FUNCTION log_util_npa_release_vote

DESCRIPTION
  This procedure votes for sleep for the Diag task.

===========================================================================*/
void log_util_npa_release_vote(void);


/*===========================================================================

FUNCTION log_util_diag_clock_vote_init

DESCRIPTION
  Registration with diag for diag mask chagne update

===========================================================================*/
void log_util_diag_mca_registration(void);

/*===========================================================================

FUNCTION log_util_save_mask_change_history

DESCRIPTION
  This saves the mask change history for debugging

===========================================================================*/
void log_util_save_mask_change_history(void);


#endif

