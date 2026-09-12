#ifndef RFLM_CCS_TASK_DEFS_H
#define RFLM_CCS_TASK_DEFS_H

/*!
  @file 
  rf_bus_ccs_cmd_intf.c

  @brief
  Implementation of generic functions that access CCS via command interface
  This version uses the task queue
 
*/

/*============================================================================== 
   
  Copyright (c) 2013 - 2015 Qualcomm Technologies Incorporated. All Rights Reserved 
   
  Qualcomm Proprietary 
   
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
   
==============================================================================*/ 

/*==============================================================================
   
                        EDIT HISTORY FOR MODULE 
   
This section contains comments describing changes made to the module. 
Notice that changes are listed in reverse chronological order. 
 
$Header: //source/qcom/qct/modem/fw/components/fw_common.mpss/rel/17.0/api/rflm_ccs_task_defs.h#14 $ 
$DateTime: 2024/03/06 07:07:40 $ $Author: gsingara $ 
   
when       who     what, where, why 
--------   ---    -------------------------------------------------------------- 
5/4/2017   hdz    Increased schedQ size from 32 to 40
10/31/16   ck     Removed added mask transaction type (not defined in SWI)
06/27/16   ck     Added shadow logging
05/13/16   apm    Added Mask write support for RFFE transaction
12/04/15    lm    Moved RFFE clock speed enums definition in this file
10/06/15   hdz    Change RFFE_ACCESS_EXT to RFFE_READ_EXT
06/30/15   rca    MC Seq optimization changes (timing fixup table relocation)
04/30/15   rca    Changes to support elimination of ACCESS_XX tasks, task switch speed up PLUS MultiChain(mc) Sequence support.
03/17/15   hdz    Removed SSBI related and updated data bit-width to 4 for Atlas
11/22/14   ra     Use features defined in \build\ms
10/20/14   hdz    Added support to Virtual taskQ, thread safe enqueue and DMA remap
09/24/14   rca    THOR sync up with latest version of this file (due to changes to issue seq update task defs) version 7.2
                  for CCS M3 for THOR
07/29/14   rca    CCS. Integration of qwu changes for "Priority Update" functionality in CCS M3 for BOLT and BOLT PLUS
                  (version 5.10 and 6.4)
07/22/14   rca    THOR featurization (version 7.1 of rflm_ccs_image.c in mdm9x45 area)
06/23/14   rca    CCS. BO.1.1 BO.2.0 Support fix internal issue seq late error in CCS M3
                  (version 5.9 of rflm_ccs_image.c in mdm9x35 area, 6.3 of rflm_ccs_image in msm8994 area)
06/23/14   rca    CCS. BO.1.1 BO.2.0 Support fix for occasional non preemption of lower priority task
                  (version 5.8 of rflm_ccs_image.c in mdm9x35 area, 6.2 of rflm_ccs_image in msm8994 area)
06/20/14   ra     Update number of CCS transactions
06/09/14   ra   Fix  Bolt Plus featurization
05/22/14   ra     Support target based feature - needed for two new RFFE channels on BO.2.0
05/09/14   rca    CCS. Support move update dword count to issue seq update task def
                  (version 5.7 of rflm_ccs_image.c)
05/07/14   rca    CCS. Extended CCS_RF_ERR_PARAM errors (version 5.6 of rflm_ccs_image.c)
05/05/14   rca    CCS. Support for NEW feature ISSUE SEQ UPDATE (vserion 5.5 of rflm_ccs_image.c)
05/02/14   rca    CCS. RFFE EXT reads using DMA (version 5.4 of rflm_ccs_image.c/h)
                  History of rflm_ccs_image.c/h changes
                  5.1 RFFE EXT writes using DMA
                  5.2 New CSS Data Buffer Allocation For All Techs.
                  5.3 Increase number of tasks per TQ to 32
12/16/13   Saul   CCS. Increase number of tasks per TQ to 32.
11/25/13   Saul   CCS. New CSS Data Buffer Allocation For All Techs.
10/18/13   Saul   CCS. RFFE EXT writes using DMA.
10/08/13   cd     Define task types for D3925
09/03/13   ra     Rebaseline: compatible with older modem and new CCS processor
07/31/13   hdz    Added more backward compatability support
07/23/13   ra     Initial version. Created to support backward compatability
==============================================================================*/ 
#define RFLM_CCS_M3_PROCESSOR_ENVIRONMENT
//#define RFLM_CCS_M3_PROCESSOR_ENVIRONMENT FEATURE_ATLAS_MODEM
#ifndef RFLM_CCS_M3_PROCESSOR_ENVIRONMENT
#include "comdef.h"
#endif /* RFLM_CCS_PROCESSOR_ENVIRONMENT */

/*===========================================================================

                   EXTERNAL DEFINITIONS AND TYPES

===========================================================================*/


/*! @brief Version of the CCS RF module.
*/
#if defined(FEATURE_RF_CCS_THOR)
#define CCS_RF_MAJOR_VERSION            7
#define CCS_RF_MINOR_VERSION            2
#elif defined(FEATURE_RF_CCS_BOLT_PLUS)
#define CCS_RF_MAJOR_VERSION            6
#define CCS_RF_MINOR_VERSION            4
#else
#define CCS_RF_MAJOR_VERSION            5
#define CCS_RF_MINOR_VERSION            10
#endif
#define CCS_RF_VERSION                  ((CCS_RF_MAJOR_VERSION << 16) | CCS_RF_MINOR_VERSION)



/*! @brief Index of DSP-to-CCS interrupt used for RF commands. 
*/
#define CCS_RF_CMD_IRQ_IDX              1

/*! @brief Index of DSP-to-CCS interrupt used for RF QDSS log. 
*/
#define CCS_RF_CMD_LOG_IDX              0
/*! @brief Index of CCS-to-DSP interrupt used for RF responses. 
*/
#define CCS_RF_RSP_IRQ_IDX              1


/*! @brief Number of RFFE channels supported.
*/
//#define CCS_RF_NUM_RFFE_CHAN            7
#define CCS_RF_NUM_RFFE_CHAN            5



/*! @brief MAX number of combined bus resources supported. 
    Must be in multiple of 4 bytes and be greater than or equal
    to max buses supported
*/
#define CCS_RF_MAX_BUS_RESOURCES ((CCS_RF_NUM_RFFE_CHAN+3) & (~3))


/*! @brief Maximum number of transactions supported per task. 
    The task field for num_transactions is 8 bits, but memory constraints will
    probably limit this lower.
*/
#define CCS_RF_MAX_NUM_TRANSACTIONS     225



/*! @brief Number of RF task queues.
    First 4 are intended for RAT direct use, fifth is for RFSW commands (via FW).
*/
#define CCS_RF_NUM_TASKQ                5

/*! @brief Number of RF task queues which support a full feature set.
    The remainder have limited functionality, such as not supporting ISSUE_SEQ.
*/
#define CCS_RF_NUM_FULL_TASKQ           4

/*! @brief Number of tasks on each task queue.
*/
#define CCS_RF_TASKQ_NUM_ITEMS          32


// can be removed, used by tech code
#define CCS_RF_TASKQ_PAIRS_DATA_BUF_NUM_WORDS     ( (8*1024) / 4 )
#define CCS_RF_TASKQ_DATA_BUF_PAIR0_NUM_WORDS     CCS_RF_TASKQ_PAIRS_DATA_BUF_NUM_WORDS
#define CCS_RF_NUM_SSBI_CHAN            2
#define CCS_RF_RESOURCE_SSBI0      (0)
#define CCS_RF_RESOURCE_SSBI_LAST  (CCS_RF_RESOURCE_SSBI0+CCS_RF_NUM_SSBI_CHAN-1)
//#define CCS_RF_RESOURCE_RFFE0      (CCS_RF_RESOURCE_SSBI_LAST+1)

#define CCS_RF_TASKQ_CCS_MMU_MEM_ADDR_OFFSET_WORDS 0

#define CCS_RF_MMU_MEM_NUM_WORDS     ( (7*1024) / 4 )

#define  CCS_RF_TASKQ_CCS_MMU_STATIC_REGION_MEM_ADDR_OFFSET_WORDS (CCS_RF_TASKQ_CCS_MMU_MEM_ADDR_OFFSET_WORDS + CCS_RF_MMU_MEM_NUM_WORDS)

#define CCS_RF_MMU_STATIC_REGION_MEM_NUM_WORDS     ( (3*1024) / 4 )

/*! @brief TQ IRAT address offset in the CCS data buffer
*/
#define CCS_RF_TASKQ_DATA_BUF_IRAT_ADDR_OFFSET_WORDS    ( CCS_RF_TASKQ_CCS_MMU_STATIC_REGION_MEM_ADDR_OFFSET_WORDS+CCS_RF_MMU_STATIC_REGION_MEM_NUM_WORDS )

/*! @brief TQ IRAT number of 32-bit words in the CCS data buffer 
    IRAT is allocated 2 K bytes in the CCS data buffer.
    Total size in bytes / size of one word (4 bytes or uint32) = num words
*/
#define CCS_RF_TASKQ_DATA_BUF_IRAT_NUM_WORDS     (((2*1024) / 4) + 70 ) 

#define CCS_RF_TASKQ_RFFE_LOG_BUF_ADDR_OFFSET_WORDS  ( CCS_RF_TASKQ_DATA_BUF_IRAT_ADDR_OFFSET_WORDS+CCS_RF_TASKQ_DATA_BUF_IRAT_NUM_WORDS)

#define CCS_RF_RFFE_LOG_BUF_NUM_WORDS  ((1*1024) / 4)

#define RFLM_CCS_MMU_MEM_SIZE_BYTES         (CCS_RF_MMU_MEM_NUM_WORDS*4)
#define RFLM_CCS_MMU_BLOCK_SIZE_BYTES       32
#define RFLM_CCS_MMU_BITMAP_SIZE                \
                 (RFLM_CCS_MMU_MEM_SIZE_BYTES/RFLM_CCS_MMU_BLOCK_SIZE_BYTES)   
#define RFLM_CCS_MMU_BITMAP_ARRAY_SIZE  (RFLM_CCS_MMU_BITMAP_SIZE/32)
#define RFLM_CCS_MMU_STATUS_WORDS_NUM       2 // add 1 for use_mmu_flag, will remove later
#define RFLM_CCS_MMU_MAX_NUM_USRS           6


/*! @brief Size of taskq data buffer, shared for all TQ.
    + 7k MMU
    + 3k for Static
    + 2k for IRAT, adding 40 words more
    + 1k for RFFE logging
    = 13k bytes in RFC/CCS for data buffer
*/
#define CCS_RF_TASKQ_DATA_BUF_WORDS   (   CCS_RF_MMU_MEM_NUM_WORDS       \
                                        + CCS_RF_MMU_STATIC_REGION_MEM_NUM_WORDS      \
                                        + CCS_RF_TASKQ_DATA_BUF_IRAT_NUM_WORDS  \
                                        + CCS_RF_RFFE_LOG_BUF_NUM_WORDS  )
                                        

//#define RF_LOG_BUF_SIZE 510
#define RF_LOG_BUF_SIZE (CCS_RF_RFFE_LOG_BUF_NUM_WORDS-4) // 256-4 = 252

#define RFLM_CCS_MMU_TASK_MARKER            0xA5A5A5A5


/*! @brief Task types.
*/
#define CCS_RF_TASK_NONE                0
#define CCS_RF_TASK_SSBI_WRITE          1
#define CCS_RF_TASK_SSBI_ACCESS         2
#define CCS_RF_TASK_RFFE_WRITE          3
#define CCS_RF_TASK_RFFE_ACCESS         4
#define CCS_RF_TASK_RFFE_WRITE_EXT      5
#define CCS_RF_TASK_RFFE_ACCESS_EXT     6
#define CCS_RF_TASK_GRFC_WRITE          7
#define CCS_RF_TASK_ISSUE_SEQ           8
#define CCS_RF_TASK_RFFE_READ_EXT       9
#define CCS_RF_TASK_ISSUE_SEQ_UPDATE    10
#define CCS_RF_TASK_PRIORITY_UPDATE     11
#define CCS_RF_TASK_MC_SEQ              12
#define CCS_RF_TASK_RFFE_READ           13
#define CCS_RF_TASK_MAX                 14


/*! @brief Task trigger types ("trig_type" field).
    IMMEDIATE: Task is triggered immediately when channel is available.
    ABSOLUTE:  Task will be started exactly at trigger time, or error .
    DYNAMIC:   Task will be started after trigger time, when channel is available.
*/
#define CCS_RF_TRIG_IMMEDIATE           0
#define CCS_RF_TRIG_ABSOLUTE            1
#define CCS_RF_TRIG_DYNAMIC             2

/*! @brief Trigger time (task "trig_time" field) precision: 
    Number of bits and bitmask, in UNIV_STMR units.
*/
#define CCS_RF_TRIG_TIME_BITS           22
#define CCS_RF_TRIG_TIME_MASK           ((1<<CCS_RF_TRIG_TIME_BITS)-1)

/*! @brief Highest task priority (task "priority" field).
*/
#define CCS_RF_TASK_PRIO_HIGHEST        15

/*! @brief Task state (for ccs_rf_task_status_t.state)
    IDLE:      Task item unoccupied.
    PENDING:   Task placed on queue, but not serviced by CCS yet.
    SCHED:     Task is in CCS scheduler, but dispatch time not yet determined.
    ACTIVE:    Task has been committed to dispatch.
    INTERRUPT: Task completed, and generated interrupt                       .
    DONE:      Task completed and has no interrupt pending.
    EALRY_DEFER:Task is enqueued too early and is buffered in Q6 memory. Will be enequeued by the ustmr thread later.
    EALRY_SKIP:Task is enqueued too early and Q6 storage is full, the task is skipped.
*/
#define CCS_RF_TASK_STATE_IDLE        0
#define CCS_RF_TASK_STATE_PENDING     1
#define CCS_RF_TASK_STATE_SCHED       2
#define CCS_RF_TASK_STATE_ACTIVE      3
#define CCS_RF_TASK_STATE_INTERRUPT   4
#define CCS_RF_TASK_STATE_DONE        5
#define CCS_RF_TASK_STATE_EARLY_DEFER 6
#define CCS_RF_TASK_STATE_EARLY_SKIP  7


/*! @brief CCS RF errors. 
    PARAM:    Error in task parameters.
    TIMING:   Task with ABSOLUTE timing could not satisfy its dispatch time.
    PREEMPT:  Task with PREEMPT_ERROR was interrupted. 
    PARITY:   RFFE read transaction got parity error.
    SEQ_LATE: An ISSUE_SEQ task could not be programmed by its start time.
*/
#define CCS_RF_ERR_NONE                    0
#define CCS_RF_ERR_PARAM_INVALID_TYPE      1
#define CCS_RF_ERR_TIMING                  2
#define CCS_RF_ERR_PREEMPT                 3
#define CCS_RF_ERR_PARITY                  4
#define CCS_RF_ERR_SEQ_LATE                5
#define CCS_RF_ERR_NO_DISPATCH             6
#define CCS_RF_ERR_PARAM_INVALID_NUM_TRANS 7
#define CCS_RF_ERR_PARAM_INVALID_PTR       8
#define CCS_RF_ERR_PARAM_INVALID_SSBI_CHAN 9
#define CCS_RF_ERR_PARAM_INVALID_RFFE_CHAN 10
#define CCS_RF_ERR_PARAM_INVALID_SEQ_RES   11
#define CCS_RF_ERR_PARAM_INVALID_SEQ_DMA   12

/*! @brief Task resources.
    Each RF channel is a separate resource. GRFCs have no resource, since no conflict.
    Each TQ also is a resource.
*/
#define CCS_RF_RESOURCE_RFFE0      (CCS_RF_RESOURCE_SSBI_LAST+1)
#define CCS_RF_RESOURCE_RFFE_LAST  (CCS_RF_RESOURCE_RFFE0+CCS_RF_NUM_RFFE_CHAN-1)
#define CCS_RF_RESOURCE_BUS_LAST   (CCS_RF_RESOURCE_RFFE_LAST)
#define CCS_RF_RESOURCE_TQ0        (CCS_RF_RESOURCE_RFFE_LAST+1)
#define CCS_RF_RESOURCE_TQ_LAST    (CCS_RF_RESOURCE_TQ0+CCS_RF_NUM_TASKQ-1)
#define CCS_RF_RESOURCE_GRFC_RES   (CCS_RF_RESOURCE_TQ_LAST+1)
#define CCS_RF_RESOURCE_LAST       (CCS_RF_RESOURCE_GRFC_RES)

#define CCS_RF_RESOURCE_LOGGING     (7)
#define CCS_RF_RESOURCE_GRFC        (6)

/*! @brief  Enum for rffe clk*/
typedef enum
{
  RF_HAL_RFFE_19_p_2      = 0,  /*! 19.2 MHz */
  RF_HAL_RFFE_38_p_4      = 1,  /*! 38.4 MHz*/
  RF_HAL_RFFE_MAX_NUM_CLK       = 2, /*! Max number of rffe clks */
} rf_ccs_rffe_clk_enum;

typedef enum
{
  /*! TQ Pair 0 */
  RFLM_CCS_DATA_BUF_ITEM_PAIR0 = 0,
  /*! TQ Pair 1 */
  RFLM_CCS_DATA_BUF_ITEM_PAIR1,
  /*! IRAT */
  RFLM_CCS_DATA_BUF_ITEM_IRAT,
  /*! Dedicated SW Port */
  RFLM_CCS_DATA_BUF_ITEM_DEDICATED,
  /*! TQ Pair Extension*/
  RFLM_CCS_DATA_BUF_ITEM_PAIR_EXTENSION,
  /* ISS SEQ UPDATE */
  RFLM_CCS_RF_MMU_STATIC_REGION
} rflm_ccs_data_buf_item_type;

/*! @brief Common task fields.
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32               : 4;
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_x) */

  /* Word1 */
  uint32               : 8;
  uint32 mode          : 1;
  uint32               : 2;
  uint32 label         : 5;  /*!< task label */ 
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 9;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32               : 32;

} ccs_rf_task_common_t;


/*! @brief Common task fields for SSBI and RFFE (not GRFC and not ISSUE_SEQ)
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 channel       : 4;  /*!< SSBI/RFFE channel */
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_x) */

  /* Word1 */
  uint32 num_transactions : 8; /*!< Number of transactions to issue */
  uint32 mode          : 1;
  uint32               : 7;  
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 9;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 transaction_ptr : 32; /*!< Pointer to transaction data */

} ccs_rf_task_common_non_grfc_t;


/*! @brief SSBI_WRITE task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 channel       : 4;  /*!< SSBI channel */
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_SSBI_WRITE) */

  /* Word1 */
  uint32 num_transactions : 8; /*!< Number of transactions to issue */
  uint32 mode          : 1;
  uint32 done_signal   : 7;  
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 9;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 transaction_ptr : 32; /*!< Pointer to transaction data */

} ccs_rf_task_ssbi_write_t;


/*! @brief SSBI_ACCESS task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 channel       : 4;  /*!< SSBI channel */
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_SSBI_ACCESS) */

  /* Word1 */
  uint32 num_transactions : 8; /*!< Number of transactions to issue */
  uint32 mode          : 1;
  uint32 done_signal   : 7;  
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 9;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 transaction_ptr : 32; /*!< Pointer to transaction data */

} ccs_rf_task_ssbi_access_t;


/*! @brief RFFE task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 channel       : 4;  /*!< RFFE channel */
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_RFFE_WRITE/READ) */

  /* Word1 */
  uint32 num_transactions : 8; /*!< Number of transactions to issue */
  uint32 mode          : 1;
  uint32 done_signal   : 7;  
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 2;
  uint32 slave_id      : 4;  /*!< RFFE slave ID, for all transactions */
  uint32 half_rate     : 1;   /*!< Half-rate read */
  uint32 rd_delay      : 1;   /*!< Insert extra cycle of delay before read-back */
  uint32               : 1;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 transaction_ptr : 32; /*!< Pointer to transaction data */

} ccs_rf_task_rffe_t;


/*! @brief RFFE_WRITE task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 channel       : 4;  /*!< RFFE channel */
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_RFFE_WRITE) */

  /* Word1 */
  uint32 num_transactions : 8; /*!< Number of transactions to issue */
  uint32 mode          : 1;
  uint32 done_signal   : 7;  
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 2;
  uint32 slave_id      : 4;  /*!< RFFE slave ID, for all transactions */
  uint32               : 3;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 transaction_ptr : 32; /*!< Pointer to transaction data */

} ccs_rf_task_rffe_write_t;


/*! @brief RFFE_ACCESS task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 channel       : 4;  /*!< RFFE channel */
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_RFFE_ACCESS) */

  /* Word1 */
  uint32 num_transactions : 8; /*!< Number of transactions to issue */
  uint32 mode          : 1;
  uint32 done_signal   : 7;  
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 9;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 transaction_ptr : 32; /*!< Pointer to transaction data */

} ccs_rf_task_rffe_access_t;


/*! @brief RFFE_EXT task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 channel       : 4;  /*!< RFFE channel */
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_RFFE_WRITE_EXT) */

  /* Word1 */
  uint32 num_transactions : 8; /*!< Number of transactions to issue */
  uint32 mode          : 1;
  uint32               : 7;  
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 9;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 transaction_ptr : 32; /*!< Pointer to transaction data */

} ccs_rf_task_rffe_ext_t;


/*! @brief RFFE_WRITE_EXT task 
*/
typedef ccs_rf_task_rffe_ext_t ccs_rf_task_rffe_write_ext_t;


/*! @brief RFFE_READ_EXT task 
*/
typedef ccs_rf_task_rffe_ext_t ccs_rf_task_rffe_read_ext_t;


/*! @brief RFFE_ACCESS_EXT task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 channel       : 4;  /*!< RFFE channel */
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_RFFE_READ_EXT) */

  /* Word1 */
  uint32 num_transactions : 8; /*!< Number of transactions to issue */
  uint32 mode          : 1;
  uint32               : 7;  
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32 preempt_error : 1;  /*!< Generate error if preempted */
  uint32               : 9;
  uint32 atomic        : 1;  /*!< Make task non-preemptable */
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 transaction_ptr : 32; /*!< Pointer to transaction data */

} ccs_rf_task_rffe_access_ext_t;


/*! @brief GRFC_WRITE task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32 level         : 1;  /*!< 1=set high, 0=set low */
  uint32 group         : 1;  /*!< 0=grfc0 to grfc31; 1=grfc32 to grfc34 */
  uint32               : 2;
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_GRFC_WRITE) */

  /* Word1 */
  uint32               : 8; 
  uint32 mode          : 1;
  uint32               : 7;
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32               : 11; 
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 grfc_bitmask  : 32; /*!< GRFC[31:0] control bits */

} ccs_rf_task_grfc_write_t;


/*! @brief ISSUE_SEQ task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32               : 4;
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_ISSUE_SEQ) */

  /* Word1 */
  uint32               : 8; 
  uint32 mode          : 1;
  uint32               : 7;
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32               : 11; 
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 seq_ptr       : 32; /*!< Pointer to task sequence */

} ccs_rf_task_issue_seq_t;



/*! @brief ISSUE_SEQ update task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32               : 4;
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_ISSUE_SEQ) */

  /* Word1 */
  uint32               : 8; 
  uint32 mode          : 1;
  uint32               : 7;
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32               : 1; 
  uint32 dispatch      : 1;  /*!< 0: do not dispatch; 1: dispatch */
  uint32 update_count  : 8;
  uint32               : 1; 
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 update_ptr    : 32; /*!< Pointer to update sequence */

} ccs_rf_task_issue_seq_update_t;


/*! @brief MC_SEQ task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time     : 22; /*!< Trigger time */
  uint32 trig_type     : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32               : 4;
  uint32 type          : 4;  /*!< Task type (CCS_RF_TASK_ISSUE_SEQ) */

  /* Word1 */
  uint32               : 8; 
  uint32 mode          : 1;
  uint32               : 7;
  uint32 int_enable    : 1;  /*!< Generate interrupt when complete */
  uint32               : 11; 
  uint32 priority      : 4;  /*!< Task priority */

  /* Word2 */
  uint32 seq_ptr       : 32; /*!< Pointer to task sequence */

} ccs_rf_task_mc_seq_t;


/*! @brief Priority Update task 
*/
typedef struct
{
  /* Word0 */
  uint32 trig_time        : 22; /*!< Trigger time */
  uint32 trig_type        : 2;  /*!< Trigger type (CCS_RF_TRIG_x) */
  uint32                  : 4;
  uint32 type             : 4;  /*!< Task type (CCS_RF_TASK_PRIORITY_UPDATE) */

  /* Word1 */
  uint32                  : 8; 			
  uint32 mode             : 1;
  uint32                  : 7;
  uint32 int_enable       : 1;  /*!< Generate interrupt when complete */
  uint32                  : 11;
  uint32 priority         : 4;  /*!< Task priority */

  /* Word2 */
  uint32 target_task      : 5;  /*!< the label of the task to change */ 
  uint32 target_priority  : 4;  /*!< the new priority of the task to change */
  uint32                  : 23;

} ccs_rf_task_priority_update_t;


/*! @brief Union of all task types.
*/
typedef union
{
  ccs_rf_task_common_t          common;
  ccs_rf_task_common_non_grfc_t common_non_grfc;
  // Below 2 just for comp fix, as it is used from tech's. SSBI not supported here
  ccs_rf_task_ssbi_write_t      ssbi_write;
  ccs_rf_task_ssbi_access_t     ssbi_access;
  ccs_rf_task_rffe_t            rffe;
  ccs_rf_task_rffe_write_t      rffe_write;
  ccs_rf_task_rffe_access_t     rffe_access;
  ccs_rf_task_rffe_write_ext_t  rffe_write_ext;
  ccs_rf_task_rffe_read_ext_t   rffe_read_ext;
  ccs_rf_task_rffe_ext_t        rffe_ext;
  ccs_rf_task_rffe_access_ext_t rffe_access_ext;
  ccs_rf_task_grfc_write_t      grfc_write;
  ccs_rf_task_issue_seq_t       issue_seq;
  ccs_rf_task_issue_seq_update_t issue_seq_update;
  ccs_rf_task_mc_seq_t          mc_seq;
  ccs_rf_task_priority_update_t priority_update;
  uint32                        task_word[3];

} ccs_rf_task_t;



/*! @brief Task status information.
*/
typedef union
{
  struct {
    uint32 end_time : 22;  /*!< Task completion time */
    uint32          : 2;   
    uint32 error    : 4;   /*!< Task error (CCS_RF_ERR_x) */
    uint32 state    : 4;   /*!< Task state (CCS_RF_TASK_STATE_x) */
  };

  uint32 status_word;

} ccs_rf_task_status_t;


/*! @brief Task+status word pair.
*/
typedef struct
{
  ccs_rf_task_t        task;    /*!< Task parameters */
  ccs_rf_task_status_t status;  /*!< Task status */

} ccs_rf_task_buf_item_t;



/*! @brief Format of a single SSBI or RFFE transaction, used by reads and writes.
*/
typedef union
{
  struct {
    uint8 data;     /*!< SSBI/RFFE data (output for writes, input for reads) */
    uint8 addr;     /*!< SSBI/RFFE address (for RFFE, auto-determines transaction type) */
  };
  uint16 transaction_hword;
} ccs_rf_transaction_ssbi_rffe_rw_t;

/*! @brief Format of an SSBI or RFFE transaction pair.
*/
typedef union
{
  ccs_rf_transaction_ssbi_rffe_rw_t transaction[2];  /*!< Pair of transactions */
  uint32 transaction_word;
} ccs_rf_transaction_ssbi_rffe_rw_pair_t;

/*! @brief Format of an SSBI_ACCESS or RFFE_ACCESS transaction.
*/
typedef union
{
  struct {
    uint32 data      : 8;  /*!< SSBI data (output for writes, input for reads) */
    uint32 addr      : 8;  /*!< SSBI address */
    uint32 mask      : 8;  /*!< Bitmask (1=modify) for masked writes only */
    uint32 slave_id  : 4;  /*!< RFFE slave ID (ignored for SSBI) */
    uint32 half_rate : 1;  /*!< Perform read at half rate (RFFE read only) */
    uint32 rd_delay  : 1;  /*!< Insert extra cycle of delay before read-back */
    uint32 mode      : 2;  /*!< Transaction mode (see below) */
#define CCS_RF_TRANSACTION_ACCESS_MODE_WRITE_MASKED 1
#define CCS_RF_TRANSACTION_ACCESS_MODE_WRITE        2
#define CCS_RF_TRANSACTION_ACCESS_MODE_READ         3
  };
  uint32 transaction_word;
} ccs_rf_transaction_ssbi_rffe_access_t;

/*! @brief Aliases for SSBI/RFFE transactions.
*/
typedef ccs_rf_transaction_ssbi_rffe_rw_t      ccs_rf_transaction_ssbi_rw_t;
typedef ccs_rf_transaction_ssbi_rffe_rw_t      ccs_rf_transaction_rffe_rw_t;
typedef ccs_rf_transaction_ssbi_rffe_rw_pair_t ccs_rf_transaction_ssbi_rw_pair_t;
typedef ccs_rf_transaction_ssbi_rffe_rw_pair_t ccs_rf_transaction_rffe_rw_pair_t;
typedef ccs_rf_transaction_ssbi_rffe_access_t  ccs_rf_transaction_ssbi_access_t;
typedef ccs_rf_transaction_ssbi_rffe_access_t  ccs_rf_transaction_rffe_access_t;


/*! @brief Format of RFFE extended transactions (read or write).
*/
typedef union
{
  struct {
    uint32 addr        : 16;  /*!< RFFE address */
    uint32 mode        : 2;   /*!< Transaction mode (see below) */
  #define CCS_RF_TRANSACTION_ACCESS_EXT_MODE_WRITE     2
  #define CCS_RF_TRANSACTION_ACCESS_EXT_MODE_READ      3
    uint32 num_data    : 4;   /*!< Number of data bytes (0=1 byte, 3=4 bytes) */
    uint32 slave_id    : 4;   /*!< RFFE slave ID */
    uint32 half_rate   : 1;   /*!< Half-rate read */
    uint32 rd_delay    : 1;   /*!< Insert extra cycle of delay before read-back */
    uint32 type        : 2;   /*!< Transaction type (see below) */
  #define CCS_RF_TRANSACTION_RFFE_EXT_TYPE_REG0      0
  #define CCS_RF_TRANSACTION_RFFE_EXT_TYPE_REG       1
  #define CCS_RF_TRANSACTION_RFFE_EXT_TYPE_EXT       2
  #define CCS_RF_TRANSACTION_RFFE_EXT_TYPE_EXT_LONG  3
    uint32             : 2;   /*!< ** MUST be set to 0 */

    uint8 data[4];            /*!< Transaction data */
  };

  uint32 transaction_word[2];

} ccs_rf_transaction_rffe_ext_rw_t;

/*! @brief Format of RFFE extended write transaction control.
*/
typedef union
{
  struct {
    uint32 addr        : 16;  /*!< RFFE address */
    uint32 mode        : 2;   /*!< Transaction mode (see below) */
  #define CCS_RF_TRANSACTION_ACCESS_EXT_MODE_WRITE     2
  #define CCS_RF_TRANSACTION_ACCESS_EXT_MODE_READ      3
    uint32 num_data    : 4;   /*!< Number of data bytes (0=1 byte, 3=4 bytes) */
    uint32 slave_id    : 4;   /*!< RFFE slave ID */
    uint32 half_rate   : 1;   /*!< Half-rate read */
    uint32 rd_delay    : 1;   /*!< Insert extra cycle of delay before read-back */
    uint32 type        : 2;   /*!< Transaction type (see below) */
  #define CCS_RF_TRANSACTION_RFFE_EXT_TYPE_REG0      0
  #define CCS_RF_TRANSACTION_RFFE_EXT_TYPE_REG       1
  #define CCS_RF_TRANSACTION_RFFE_EXT_TYPE_EXT       2
  #define CCS_RF_TRANSACTION_RFFE_EXT_TYPE_EXT_LONG  3
    uint32             : 2;   /*!< ** MUST be set to 0 */

  };

  uint32 transaction_ctl;/*!< Transaction Control */

} ccs_rf_transaction_rffe_ext_control_rw_t;

/*! @brief Format of RFFE extended write transactions data.
*/
typedef union
{
  uint8 data[4];            
  uint32 transaction_data;/*!< Transaction Data */
} ccs_rf_transaction_rffe_ext_data_rw_t;

/*! @brief Single task queue interface.
*/
typedef struct
{
  volatile uint32 enable;       /*!< Enable flag, controlled by DSP */
  volatile uint32 wr_idx;       /*!< Next buffer write location */
  volatile uint32 rd_idx;       /*!< Next buffer execute location */
  volatile uint32 active_tasks; /*!< Bitmask of tasks currently active, in some stage */
  volatile uint32 int_pending;  /*!< Bitmask of interrupts pending */
  volatile uint32 int_clear;    /*!< Written by DSP to clear interrupts */

  ccs_rf_task_buf_item_t task_buf[CCS_RF_TASKQ_NUM_ITEMS]; /*!< Task buffer */

} ccs_rf_taskq_intf_t;


/*! @brief CCS RF shared memory interface.
*/
typedef struct
{
  /*! Version of the CCS RF module */
  uint32 version;         

  /*! Global flags */
  uint32 ignore_parity;     /*!< When set, parity read errors are ignored.
                                 Useful for testing without RF devices connected. */ 
  uint32 debug;             /*!< Enable internal debug features. */

  /*! Task queues */
  ccs_rf_taskq_intf_t taskq[CCS_RF_NUM_TASKQ]; 

  /*! Task transaction data buffer, shared by all task queues */
  uint32 data_buf[CCS_RF_TASKQ_DATA_BUF_WORDS];

  uint8 resource_to_dma_chan[(CCS_RF_RESOURCE_RFFE_LAST+1+3)&(~3)];
  
  /*! M3 bitmap */
  uint32 rflm_ccs_mmu_m3_bitmap[RFLM_CCS_MMU_BITMAP_ARRAY_SIZE];

  /*! Tech mem usage */
  uint32 rflm_ccs_mmu_m3_tech_acc_usage[RFLM_CCS_MMU_MAX_NUM_USRS];

  uint32 rf_log_mask;

  uint32 rf_log_evt;

  uint32 rf_log_buffer_addr;

} ccs_rf_intf_t;


/*! @brief Transaction sequence header.
    For internal and sequence generator use only.
*/
typedef struct
{
  uint32 start_time_ustmr;  /*!< Reference start time of sequence, in USTMR units */
  uint16 duration_ustmr;    /*!< Duration of sequence, in USTMR units */
  uint16 resources;         /*!< Resources used by sequence */
  uint8 num_dma;            /*!< Number of DMA descriptors */
  uint8 num_timing_fixups;  /*!< Number of timing fixups */

  /* Remainder of sequence is variable-length:
     dmac_descriptor_t dma[num_dma];
     uint8 timing_fixup[num_timing_fixups];
   */

} ccs_rf_seq_header_t;


typedef struct
{
  uint32 seq_ptr;
} ccs_rf_seq_update_header_t;

typedef struct
{
  uint16 resources;
  uint16 duration_ustmr;
  uint16 timing_fixup_offset;
  uint8  num_timing_fixups;
  uint8  unused[1];

  /* Remainder of sequence is variable-length:
     dmac_descriptor_t group_start_descr[];
     dmac_descriptor_t intermediate_descr[]
     uint32 transaction_ctl_data[];
     uint8 timing_fixup[num_timing_fixups];
   */

} ccs_rf_mc_seq_header_t;


typedef union
{
   struct
   {
     uint32 block_cnt           : 9;  // 8192/16 = 512
     uint32 bitmap_word_offset  : 5;  // 32
     uint32 bitmap_word_index   : 4;  // (8192/16)/32 = 16
     uint32 tech_id             : 4;  // up to 16 
     uint32 task_idx            : 5;  // up to 32
     uint32 tq_idx              : 3;  // up to 5
     uint32                     : 2;  // reserved 2 bits
   };
   uint32 status_word;
}rflm_ccs_mmu_m3_task_t;

#endif /* RFLM_CCS_TASK_DEFS_H */