#ifndef Q6ZIP_LOG_H 
#define Q6ZIP_LOG_H 
/*===========================================================================

DESCRIPTION
  Definitions of DIAG subsystem packets for configuring Q6ZIP 
  parameters

  Copyright (c) 2013 Qualcomm Technologies Incorporated.
  All Rights Reserved.
  Qualcomm Confidential and Proprietary

  Export of this technology or software is regulated by the U.S. Government.
  Diversion contrary to U.S. law prohibited.

  All ideas, data and information contained in or disclosed by
  this document are confidential and proprietary information of
  Qualcomm Technologies Incorporated and all rights therein are expressly reserved.
  By accepting this material the recipient agrees that this material
  and the information contained therein are held in confidence and in
  trust and will not be used, copied, reproduced in whole or in part,
  nor its contents revealed in any manner to others without the express
  written permission of Qualcomm Technologies Incorporated.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header: 

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/09/14   suj     Initial Revision

===========================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "log.h"
#include "atomic_ops.h"
#include "q6zip_types.h"

/** @brief Define this (below) to enable q6zip log support */
#undef Q6ZIP_LOG_ENABLED

/*===========================================================================

                     CONSTANTS
===========================================================================*/

#define Q6ZIP_LOG_TYPE_REQ    (0)
#define Q6ZIP_LOG_TYPE_RSP    (1)
/* flush asynchronously */
#define Q6ZIP_LOG_ASYNC_FLUSH 

#if defined( Q6ZIP_LOG_ENABLED )
void q6zip_log_init(void);
/*===========================================================================
FUNCTION: q6zip_log_flush

DESCRIPTION
    This function checks the status of req/rsp buffers
    and submits if full 

DEPENDENCIES:

RETURN VALUE:
  None

SIDE EFFECTS:
  None
===========================================================================*/
void q6zip_log_flush(void);

/**
 * @brief Put a Q6ZIP log
 * 
 * @param [in] timetick Timestamp
 * @param [in] seq_num Sequence number. Used to corelate request with response
 * @param [in] lo_q Current length of low priority queue
 * @param [in] hi_q Current length of high priority queue
 * @param [in] q_id Queue used for the request/response
 * @param [in] log_type Request or response? 
 * @param [in] algo Algorithm being used
 * @param [in] payload_size Size (bytes) of payload associated with 
 *        request/response
 */
void q6zip_log_put (
    uint32 timetick, 
    uint8  seq_num, 
    uint8  lo_q, 
    uint8  hi_q, 
    uint8  q_id, 
    uint8  log_type,
    q6zip_algo_t algo,
    uint16 payload_size
);
#else
static inline void q6zip_log_init (void) {}
static inline void q6zip_log_flush (void) {}
static inline void q6zip_log_put (
    uint32 timetick, 
    uint8  seq_num, 
    uint8  lo_q, 
    uint8  hi_q, 
    uint8  q_id, 
    uint8  log_type,
    q6zip_algo_t algo,
    uint16 payload_size
)
{
}
#endif
#endif /* Q6ZIP_LOG_H */
