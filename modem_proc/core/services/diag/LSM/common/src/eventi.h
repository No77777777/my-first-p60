#ifndef EVENTI_H
#define EVENTI_H

/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                          Event Services internal header file

General Description
  Internal declarations to support diag event service.

Initializing and Sequencing Requirements 
  None

Copyright (c) 2007-2016, 2018-2019 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
Qualcomm Confidential and Proprietary
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================
                          Edit History 
   
when       who     what, where, why
--------   ---     ---------------------------------------------------------- 
01/04/19   kdey    Added API event_status
04/29/18   ph      Enable diag boot up logging.
06/04/18   gn      Deprecated unused legacy commands, Featurized commands that 
                   are applicable in a specific configuration
12/23/16   rs      removed the arguments of event_mask_ssm()
11/03/16   sa      Implemented seperate queue for UserPD events and enabled buffering
08/12/15   xy      Verified DIAG uimage APIs in island mode 
06/15/15   xy      Added new DIAG APIs for uImage mode 
04/24/14   tbg     Fixed issues with extended log pkts.
04/01/14   rs      Added support for build mask centralization. 
11/22/13   sr      Removed PACK from pointer references 
10/14/13   sr      Added support for extended event, log and F3 pkts  
10/01/13   sa      listeners clean up function
11/09/12   is      Support for preset masks
09/28/12   sr      Filtering of logs/events based on ssm permission file  
08/31/12   sr      Support for mask retrieval command  
07/25/12   rh      Externed event_config and event_config_sec delcarations from event.c
05/10/12   sa      Mainlined DIAG_CENTRAL_ROUTING
09/15/11   is      Modifed "set mask" via CTRL msgs fot EA support
09/12/11   is      Master Diag forwards "set mask" requests via CTRL msgs
02/01/10   sg      Dual Mask Changes
09/28/10   mad     Added event_ctrl_stale_timer and event_ctrl_report_size
08/10/10   sg      Added function event_set_all_rt_masks
08/25/10   sg      Fixed compiler warnings
02/20/10   sg      Moved event_q_alloc and event_q_put here
01/10/08   mad     Added copyright and file description.
12/5/07    as      Created

===========================================================================*/

#include "comdef.h"
#include "event_defs.h"
#include "queue.h"
#include "diagdiag_v.h"    /* for event_id_type */
#include "diagcomm_io.h"   /* For diagcomm_port_type */
#include "memheap.h"
#include "diagbuf_mpd_type.h"


/* Size of the event mask array, which is derived from the maximum number
 of events.
*/
#define EVENT_MASK_SIZE (EVENT_LAST_ID/8 + 1)


#define EVENT_SEND_MAX 50
#define EVENT_RPT_PKT_LEN_SIZE 0x200

/* Dual Mask Defines */
#define DIAG_EXT_EVENT_REPORT_GET_MASK 1
#define DIAG_EXT_EVENT_REPORT_SET_MASK 2
#define DIAG_EXT_EVENT_REPORT_SET_ALL 3

/* Event reporting config indicator. */
#define EVENT_CONFIG_COMM_MASK 0x01
#define EVENT_CONFIG_VALID_MASK 0x07
#define EVENT_CONFIG_LISTENER_MASK 0x02
#define EVENT_CONFIG_EXT_LISTENER_MASK 0X04


/* For use in event_mask_set_internal_handler() */
#define DIAG_OP_SET_EVENT_STREAM 1
#define DIAG_OP_SET_EVENT_PRESET 2


/* NOTE: diag_event_type and event_store_type purposely use the same
   format, except that event_store type is preceeded by a Q link
   and the event ID field has a union for internal packet formatting.
   If either types are changed, the service will not function properly. */
typedef struct
{
  q_link_type qlink;            /* To be accessed by event_q_xxx() only */
  uint8 reserved;               /* Required? Do we need to align this struct? */
  uint8 stream_id;              /* Flag to indicate which stream this is needed for (including listeners) */
  uint16 version;               /* version field for extended headers */
  uint32 id;                    /* ID field associated with VERSION_1 */

  union
  {
    unsigned int id;
    event_id_type event_id_field;
    uint16 sized_field;         /* Used to copy the event ID structure */
  }
  event_id;

  qword ts;                     /* always store full time stamp */

  event_payload_type payload;
}
event_store_type;

typedef struct 
{
  /* The length field indicates the approximate packet size of the event
     report packet  It is calculated based on the assumption of truncated
     time stamps, since most are truncated (except the first). */
  unsigned int length;
  byte version; /* version of the extended event pkt; currently we support 0 and 1 */

  q_type q;
}event_q_type;

/*This is the structure which contains all the variables needed to alloc and drain events 
**Userpd and Rootpd have their own structure.*/
typedef struct 
{
  uint32 drop_cnt;
  qword drop_ts;
  char *diag_event_heap_buf;
  mem_heap_type diag_event_heap;
  event_store_type* event_rpt_last;
  event_q_type     event_q;
  uint32 *event_alloc_count_p;
  uint32 *event_drop_count_p;
  #ifdef FEATURE_DEBUG_DIAG_HEAP
  uint32 diag_event_malloc_success_cnt;
  uint32 diag_event_free_cnt;
  uint32 event_drop_handler_malloc_cnt;
  uint32 event_drop_handler_free_cnt;
  uint32 event_q_alloc_malloc_cnt;
  uint32 event_drain_free_cnt;
  uint32 event_q_get_cnt;
  #endif
}event_struct;

/*This structure is used to encapsulate the diagmpd_buf and event_struct ptrs.
**Can be later expanded to add more pointers. DIAG_MPD_Q contains element of this type*/
typedef struct
{
  q_link_type  link;
  diagbuf_mpd_type *mpd_buf_ptr;
  event_struct * event_mpd_struct_ptr;
}diag_mpd_ptr_struct;


/*===========================================================================
FUNCTION    find_event_pd

DESCRIPTION
  This function is used to find an element in diag_main_q. 
  This function is compare_func for q_linear_search which is used in EVENT_Q_SEARCH
  If this compare function returns "1", q_linear_search returns a pointer for the element.

DEPENDENCIES
  None.

RETURN VALUE
  1 - PD found
  0 - PD not found
  
SIDE EFFECTS
  None

===========================================================================*/
int find_event_pd(void* ptr, void * val);

/*===========================================================================
MACRO    EVENT_Q_SEARCH

DESCRIPTION
  This function is used to find an element in diag_main_q. 
  It uses q_linear_seach to find the element.

DEPENDENCIES
  fin_event_pd fn is used as a compare function.

RETURN VALUE
  Returns ptr to the element in the queue.
  
SIDE EFFECTS
  None

===========================================================================*/

#define EVENT_Q_SEARCH(pid) (\
{ \
  diag_mpd_ptr_struct * rv=NULL;\
  rv= (diag_mpd_ptr_struct *)q_linear_search(&diag_main_q,find_event_pd,&pid);\
  (rv);\
})

/* ==================================================================
FUNCTION DIAG_EVENT_HEAP_INIT

DESCRIPTION
  Initializes the heap and heap structure which is used later to allocate events for the PD.

PARAMETERS
  ptr - void ptr of type event_struct which contains heap variables.
  heap_size - size of heap to be used.
  
RETURN
  None
===================================================================== */

void
diag_event_heap_init (event_struct * ptr, int heap_size);


/* ==================================================================
FUNCTION DIAG_EVENT_MALLOC

DESCRIPTION
   Dynamically allocates a specified number of bytes from the heap specified.
    
PARAMETERS
   num_bytes - Number of bytes to be malloced
   ptr       - event_struct type pointer for the PD. 
               This should contain informationm about the diag event heap to be used.

DEPENDENCIES
   diag_event_heap structure should be setup by calling diag_event_heap_init.


RETURN
   Returns a void pointer to chunk of memory malloced.
===================================================================== */
  void *diag_event_malloc (unsigned long num_bytes,event_struct *ptr);

/* ==================================================================
FUNCTION DIAG_EVENT_FREE

DESCRIPTION
   Frees a chunk of memory allocated using diag_event_malloc.


PARAMETERS
   mem_ptr - ptr to the block to be freed
   ptr     - event_struct type pointer for the PD. 

DEPENDENCIES
   diag_event_heap structure should be setup by calling diag_event_heap_init.
   

RETURN
   None

===================================================================== */
  void diag_event_free (void *mem_ptr, event_struct * ptr);


void event_q_put(event_store_type * event,byte stream_id, event_struct * eventq);

event_store_type * event_q_alloc (event_id_enum_type id, uint8 payload_length,byte stream_id, diag_mpd_ptr_struct* ptr);

void event_control (uint8 config, uint8 config_mask);
extern uint8 event_config;

void event_control_sec (uint8 config, uint8 config_mask);
extern uint8 event_config_sec;

void event_set_all_rt_masks( uint8 config, byte stream_id);

void * event_pkt_get_local_masks (void * req_pkt, uint16 pkt_len);

void event_ctrl_stale_timer(unsigned int timer_len);
void event_ctrl_report_size(unsigned int report_size, unsigned int send_max_num, unsigned int rpt_pkt_len_siz);
void diag_clean_event_listener_ext(void *param);

event_store_type *event_q_get (event_struct *ptr);
boolean event_mask_enabled ( const unsigned char *mask_ptr, event_id_enum_type event_id);


void diag_switch_event_preset_mask( void );

boolean event_config_mask (int event_id, boolean flag, byte stream_id, uint8 preset_id);

/* On MP Master only */
#if defined(DIAG_MP_MASTER)
void diagcomm_ctrl_send_event_mask( diagcomm_port_type port_type, 
                                    diagcomm_enum_port_type port_num, 
                                    uint8 stream_id,
                                    uint8 preset_id,
                                    boolean send_now );
#endif /* DIAG_MP_MASTER */

#if defined(DIAG_MP) && !defined(DIAG_MP_MASTER)
/* Sends last event id supported to Master Diag */
void diag_ctrl_send_event_id_ctrl_pkt(void);
#endif 

#ifdef FEATURE_SSM
void event_mask_ssm();
#endif

void event_mask_set_internal_handler( uint8 op_code, uint8 id, uint16 numbits,
                                      unsigned int mask_len, unsigned char * mask_ptr );


#endif /* EVENTI_H */
