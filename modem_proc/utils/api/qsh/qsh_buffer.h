/*!
  @file
  qsh_buffer.h

  @brief
  Common management APIs for mtrace buffers.
*/

/*==============================================================================

  Copyright (c) 2017 QUALCOMM Technologies Incorporated. All Rights Reserved

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

$Header: //components/rel/utils.mpss/5.4/api/qsh/qsh_buffer.h#1 $

when       who     what, where, why
--------   ---     -------------------------------------------------------------
05/06/19   mm      CR 2380036: QTrace throttle feature
12/21/18   mm      CR 2369438: Add deinit API
08/29/18   mm      CR 2304039: Use tag bit to indicate internal-only log
08/22/18   mm      CR 2300075: Remove obsolete buffer type
06/18/18   am      CR 2251065: FR51021 QTrace master buffer to use large memory
05/11/18   mm      CR 2234649: Use log count to detect offload done
02/05/18   mm      CR 2184497: Initial check-in
==============================================================================*/

#ifndef QSH_BUFFER_H
#define QSH_BUFFER_H

/*==============================================================================

                           INCLUDE FILES

==============================================================================*/

#include <comdef.h>
#include <qsh_util.h>
#include <mtrace_type1.h>
#include <sys.h>

/*==============================================================================

                   EXTERNAL DEFINITIONS AND TYPES

==============================================================================*/

/*!
  Required alignment of buffer descriptor and trace memory.
*/
#define QSH_BUFFER_ALIGN_BYTES \
  (MTRACE_TYPE1_BLOCK_SIZE_BYTES)

/*!
  Buffer type.
*/
typedef mtrace_type1_descriptor_t * qsh_buffer_t;

/*!
  Timetick type.

  @note
  Only least significant 6 bytes are valid.
*/
typedef uint64 qsh_buffer_record_timetick_t;

/*!
  Maximum timetick value.
*/
#define QSH_BUFFER_TIMETICK_MAX \
  (0xFFFFFFFFFFFFuLL)

/*!
  Buffer type.
*/
typedef enum
{
  /*! QTrace */
  QSH_BUFFER_TYPE_MESSAGE = 0x0,

  /*! QSH event */
  QSH_BUFFER_TYPE_EVENT,

  /*! QSH event - handler */
  QSH_BUFFER_TYPE_EVENT_HANDLER,

  /*! QSH metric */
  QSH_BUFFER_TYPE_METRIC,

  /*! user-defined record */
  QSH_BUFFER_TYPE_USER,

  QSH_BUFFER_TYPE_MAX
} qsh_buffer_type_e;

/*!
  Tag field sizes.
*/
#define QSH_BUFFER_TAG_SUBS_ID_SIZE_BITS \
  (2)
#define QSH_BUFFER_TAG_TYPE_SIZE_BITS \
  (3)
#define QSH_BUFFER_TAG_INTERNAL_SIZE_BITS \
  (1)
#define QSH_BUFFER_TAG_RESERVED_SIZE_BITS \
  (2)

/*!
  Tag union for assignment.
*/
typedef union
{
  struct
  {
    uint8     subs_id : QSH_BUFFER_TAG_SUBS_ID_SIZE_BITS;
    uint8     reserved : QSH_BUFFER_TAG_RESERVED_SIZE_BITS;
    uint8     internal : QSH_BUFFER_TAG_INTERNAL_SIZE_BITS;
    uint8     type : QSH_BUFFER_TAG_TYPE_SIZE_BITS;
  }       fields;
  uint8   full;
} qsh_buffer_tag_u;

/*!
  Callback invoked upon wrap threshold detection.
*/
typedef void (*qsh_buffer_offload_wrap_cb_t)
(
  void
);

/*!
  I/O vector representing a subset of available offload memory.
*/
typedef struct
{
  uint32 *  start_ptr;
  uint32 *  end_ptr;
} qsh_buffer_iovec_s;

/*!
  Wrap check state.
*/
typedef struct
{
  /*! callback invoked upon wrap */
  qsh_buffer_offload_wrap_cb_t    cb;

  /*! threshold in bytes */
  uint32                          thresh_bytes;

  /*! wrap state */
  uint32                          active;
} qsh_buffer_offload_wrap_s;

/*!
  Partial record state.
*/
typedef struct
{
  /*! next partially-written record */
  uint32 *  next_ptr;

  /*! end of next partially-written record */
  uint32 *  next_end_ptr;
} qsh_buffer_offload_partial_s;

/*!
  Stats.
*/
typedef struct
{
  /*! number of logs written */
  uint32    log_count;

  /*! number of logs offloaded */
  uint32    log_offload_count;

  /*! number of logs offloaded when done */
  uint32    log_offload_count_target;

  /*! number of times mtrace_type1_offload() was called */
  uint32    offload_count;
  
  /*! number of times a partial record was found */
  uint32    partial_count;

  /*! number of times a partial record was successfully processed */
  uint32    partial_proc_count;

  /*! total number of bytes logged */
  uint32    logged_bytes;

  /*! number of bytes cleanly offloaded (not including salvaged) */
  uint32    offloaded_bytes;

  /*! number of zero-padding bytes */
  uint32    padding_bytes;

  /*! number of bytes salvaged */
  uint32    salvaged_bytes;

  /*! number of bytes dropped (potentially salvaged) */
  uint32    dropped_bytes;
} qsh_buffer_offload_stats_s;

/*!
  Arguments passed to/from mtrace_type1_offload().
*/
typedef struct
{
  /*! offload buffer */
  void *            offload_buf;

  /*! offload buffer size */
  uint32            offload_buf_size;

  /*! previous sequence # */
  uint32            seq_num_prev;

  /*! current sequence # */
  uint32            seq_num;

  /*! return value */
  int32             result;
} qsh_buffer_offload_invoke_s;

/*!
  Offload descriptor.
*/
typedef struct
{
  /*! ptr to buffer descriptor */
  qsh_buffer_t                  buffer;

  /*! pointer to offload memory */
  uint32 *                      mem_ptr;

  /*! size in bytes */
  uint32                        mem_size_words;

  /*! next iovec to read */
  qsh_buffer_iovec_s            iovec_next;

  /*! partial iovec from previous offload */
  qsh_buffer_iovec_s            iovec_partial;

  /*! wrap state */
  qsh_buffer_offload_wrap_s     wrap;

  /*! user's cb/arg */
  mtrace_type1_callback_t       cb;
  void *                        cb_arg;

  /*! args passed to/from mtrace_type1_offload() */
  qsh_buffer_offload_invoke_s   invoke;

  /*! whether or not offload is done */
  boolean                       done;

  /*! stats */
  qsh_buffer_offload_stats_s    stats;
} qsh_buffer_offload_s;

/*!
  Type to represent record.
*/
typedef struct
{
  /*! mtrace header */
  mtrace_type1_msg_header_t   header;

  /*! user-defined params (optional, find from header.length) */
  uint32                      param[0];
} qsh_buffer_record_s;

/*==============================================================================

                    EXTERNAL FUNCTION PROTOTYPES

==============================================================================*/

/*==============================================================================

  FUNCTION:  qsh_buffer_offload_init

==============================================================================*/
/*!
  Initialize offload descriptor.
*/
/*============================================================================*/
void qsh_buffer_offload_init
(
  /*! offload descriptor */
  qsh_buffer_offload_s *            offload_ptr,

  /*! buffer descriptor */
  qsh_buffer_t                      buffer,

  /*! forwarding function */
  mtrace_type1_callback_t           cb,

  /*! arg provided in callback */
  void *                            cb_arg,

  /*! callback invoked upon (remaining bytes) < threshold */
  qsh_buffer_offload_wrap_cb_t      wrap_cb,

  /*! threshold below which to trigger offload */
  uint32                            wrap_thresh_bytes,

  /*! pointer to memory for offload storage */
  uint32 *                          mem_ptr,

  /*! size of memory */
  uint32                            mem_size_bytes
);

/*==============================================================================

  FUNCTION:  qsh_buffer_offload_deinit

==============================================================================*/
/*!
  Deinitialize offload descriptor.
*/
/*============================================================================*/
void qsh_buffer_offload_deinit
(
  /*! offload descriptor */
  qsh_buffer_offload_s *            offload_ptr
);

/*==============================================================================

  FUNCTION:  qsh_buffer_offload_init_state

==============================================================================*/
/*!
  Returns TRUE if offload descriptor is currently initialized.
*/
/*============================================================================*/
boolean qsh_buffer_offload_init_state
(
  /*! offload descriptor */
  qsh_buffer_offload_s *            offload_ptr
);

/*==============================================================================

  FUNCTION:  qsh_buffer_offload_done_clear

==============================================================================*/
/*!
  Clear flag and capture expected sequence # to detect when offload is done.
*/
/*============================================================================*/
void qsh_buffer_offload_done_clear
(
  /*! offload descriptor */
  qsh_buffer_offload_s *  offload_ptr
);

/*==============================================================================

  FUNCTION:  qsh_buffer_offload_done

==============================================================================*/
/*!
  Return whether or not offload is done.
*/
/*============================================================================*/
boolean qsh_buffer_offload_done
(
  /*! offload descriptor */
  qsh_buffer_offload_s *  offload_ptr
);

/*==============================================================================

  FUNCTION:  qsh_buffer_offload

==============================================================================*/
/*!
  Offload given buffer to destination data.

  @note
  Needed to synchronize read/write pointers.
*/
/*============================================================================*/
void qsh_buffer_offload
(
  /*! offload descriptor */
  qsh_buffer_offload_s *  offload_ptr
);

/*==============================================================================

  FUNCTION:  qsh_buffer_offload_next

==============================================================================*/
/*!
  Return address of next record, or NULL if the end is reached.
*/
/*============================================================================*/
qsh_buffer_record_s * qsh_buffer_offload_next
(
  /*! offload descriptor */
  qsh_buffer_offload_s *  offload_ptr
);

/*==============================================================================

  FUNCTION:  qsh_buffer_record_get_timetick

==============================================================================*/
/*!
  Return 6-byte timestamp.
*/
/*============================================================================*/
static inline __attribute__((always_inline)) qsh_buffer_record_timetick_t 
qsh_buffer_record_get_timetick
(
  mtrace_type1_msg_header_t *   header_ptr
)
{
  return (qsh_buffer_record_timetick_t) (
    (((uint64) header_ptr->timestamp_msw) << 32) |
      header_ptr->timestamp_lsw);
}

/*==============================================================================

  FUNCTION:  qsh_buffer_record_timetick_diff

==============================================================================*/
/*!
  Add two 6-byte timetick values as captured by mtrace.
*/
/*============================================================================*/
static inline qsh_buffer_record_timetick_t qsh_buffer_record_timetick_add
(
  /*! timetick 1 */
  qsh_buffer_record_timetick_t   t1,

  /*! timetick 2 */
  qsh_buffer_record_timetick_t   t2
)
{
  return (t1 + t2) % QSH_BUFFER_TIMETICK_MAX;
}

/*==============================================================================

  FUNCTION:  qsh_buffer_record_timetick_diff

==============================================================================*/
/*!
  Subtract two 6-byte timetick values as captured by mtrace.
*/
/*============================================================================*/
static inline qsh_buffer_record_timetick_t qsh_buffer_record_timetick_diff
(
  /*! older timetick */
  qsh_buffer_record_timetick_t   t1,

  /*! newer timetick */
  qsh_buffer_record_timetick_t   t2
)
{
  return QSH_SUB_MOD(t1, t2, QSH_BUFFER_TIMETICK_MAX);
}

/*==============================================================================

  FUNCTION:  qsh_buffer_record_get_subs_id

==============================================================================*/
/*!
  Return subscriber id.
*/
/*============================================================================*/
static inline sys_modem_as_id_e_type qsh_buffer_record_get_subs_id
(
  mtrace_type1_msg_header_t *   header_ptr
)
{
  qsh_buffer_tag_u tag;
  tag.full = (uint8) header_ptr->tag;
  return (sys_modem_as_id_e_type) tag.fields.subs_id;
}

/*==============================================================================

  FUNCTION:  qsh_buffer_record_get_subs_id

==============================================================================*/
/*!
  Return subscriber id.
*/
/*============================================================================*/
static inline qsh_buffer_type_e qsh_buffer_record_get_type
(
  mtrace_type1_msg_header_t *   header_ptr
)
{
  qsh_buffer_tag_u tag;
  tag.full = (uint8) header_ptr->tag;
  return (qsh_buffer_type_e) tag.fields.type;
}

/*==============================================================================

                         INTERNAL VARIABLES

==============================================================================*/

/*!
  Lookup table of strings per buffer type.
*/
extern const char * qsh_buffer_type_str[QSH_BUFFER_TYPE_MAX];

#endif /* QSH_BUFFER_H */
