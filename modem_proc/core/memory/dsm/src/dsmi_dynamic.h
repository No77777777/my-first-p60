#ifndef DSMI_DYNAMIC_H_
#define DSMI_DYNAMIC_H_
/*===========================================================================

                                  D S M I _DYNAMIC. H

DESCRIPTION
  This file contains types and declarations associated with the DMSS Data
  Service Memory pool and services.

Copyright (c) 2015 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================
                            EDIT HISTORY FOR FILE

  $Header: //components/rel/core.mpss/10.0/memory/dsm/src/dsmi_dynamic.h#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
12/21/15 sko creation
===========================================================================*/

#include "core_variation.h"
#include "comdef.h"
#include "amssassert.h"

#ifndef DSMI_MAX_DYN_POOLS
#define DSMI_MAX_DYN_POOLS  2
#endif /* DSMI_MAX_AUG_POOLS */




/*===========================================================================
                        DATA DECLARATIONS
===========================================================================*/

/************* FEATURE_DSM_AUGMENT START*****************/

/* To configure the augmented pools for a target enable the flags below
 * used only by dsm team
 * do not enable it for production version */
/* #define FEATURE_DSMI_AUG_POOL_CONFIG
   #define FEATURE_DSM_MEM_CHK_EXPENSIVE_DSM_AUGMENT */

#ifdef FEATURE_DSMI_AUG_POOL_CONFIG
  #define MSG_HIGH_DSM_CONFIG(fmt,a,b,c) MSG_HIGH(fmt,a,b,c)
#else
  #define MSG_HIGH_DSM_CONFIG(fmt,a,b,c)
#endif

#ifndef DSMI_AUGMENT_MAX_ALLOC
#define DSMI_AUGMENT_MAX_ALLOC 0x1a0000
#endif

/****************DSM Library configuration*******************/
/* DSM will release memory to external memory
 * external memory allocation is
 * max. external memory
 * - DSMI_AUGMENT_EXT_MEM_BACKOFF
 */
#ifndef DSMI_AUGMENT_EXT_MEM_BACK_OFF
#define DSMI_AUGMENT_EXT_MEM_BACK_OFF 0x60000
#endif

/* DSM will not allocate from external memory if
 * external memory allocation is
 * max. external memory
 * -DSMI_AUGMENT_EXT_MEM_BACKOFF
 * -DSMI_AUGMENT_EXT_MEM_ALLOC_BACK_OFF
 */
#ifndef DSMI_AUGMENT_EXT_MEM_ALLOC_BACK_OFF
#define DSMI_AUGMENT_EXT_MEM_ALLOC_BACK_OFF 0x60000
#endif

/* Minimum free count in an augment buffer
 to recycle the buffer */
#ifndef DSMI_AUGMENT_RECYCLE_MIN_COUNT
#define DSMI_AUGMENT_RECYCLE_MIN_COUNT 70
#endif

/* DSM Dynamic_EXT_MEM_BACK_OFF is registered with heap,
 * to trigger release of all DSM dynamic memory
 * This value should be the amount of heap memory
 * that is needed while dynamic pools can be cleaned up
 * and returned by clients.
 */
#ifndef DSMI_DYNAMIC_EXT_MEM_ALLOC_BACK_OFF
#define DSMI_DYNAMIC_EXT_MEM_ALLOC_BACK_OFF 0x80000
#endif

/* Maximum number of reuse buffers held by
    DSM library for any pool */
#ifndef DSMI_AUGMENT_TOTAL_REUSE_BUFFERS
#define DSMI_AUGMENT_TOTAL_REUSE_BUFFERS 3
#endif


/* DSM Task timer duration for free request for augment
    buffer*/
#ifndef DSMI_AUGMENT_BUFFER_FREE_TIMER_DURATION
#define DSMI_AUGMENT_BUFFER_FREE_TIMER_DURATION 30
#endif

/* For a pool, when free_dbuffer_level is reached,
 reuse buffers are returned to external memory.
 level  = MIN((augment_count +
             (DSM_AUGMENT_REUSE_TRIGGER_FACTOR *
             augment_count/100)),
             DSM_AUGMENT_REUSE_TRIGGER_FACTOR_MAX)
*/
#ifndef DSM_AUGMENT_REUSE_TRIGGER_FACTOR
#define DSM_AUGMENT_REUSE_TRIGGER_FACTOR  100
#endif

#define DSMA_SCAN_BUFFER_CNT_NORMAL 5

#ifndef DSM_AUGMENT_REUSE_TRIGGER_FACTOR_MAX
#define DSM_AUGMENT_REUSE_TRIGGER_FACTOR_MAX 200
#endif

/* Number of DSM pools to be augmented */
#ifndef DSMI_MAX_AUG_POOLS
#define DSMI_MAX_AUG_POOLS  4
#endif /* DSMI_MAX_AUG_POOLS */

/* Maximum number of augment buffers for any pool */
#ifndef DSMI_AUGMENT_TOTAL_BLOCKS
#define DSMI_AUGMENT_TOTAL_BLOCKS  20
#endif /* DSMI_AUGMENT_TOTAL_BLOCKS */

#ifndef DSMI_MAX_DYNAMIC_BUFFERS
#define DSMI_MAX_DYNAMIC_BUFFERS 10
#endif

#ifndef DSM_DYNAMIC_MAX_SIZE
#define DSM_DYNAMIC_MAX_SIZE 0x30000
#endif


/**********************DSMA MACROS **********************/
#define DSMA_GET_TOP_FREE_BUFFER_INDEX(X) \
    !(X) ? DSMI_AUGMENT_TOTAL_BLOCKS: ((X)->pool_id_ext-1)

#define DSMA_GET_AUGMENT_BUFFER_SIZE(X,Y) \
        ((DSMI_POOL_OBJECT_SIZE_OPT(X)) * (Y)) + 32 + DSM_TAIL_COOKIE_LEN + 32

#define DSMA_GET_TABLE_FREE_STACK(Y) 4*(Y)

typedef enum
{
  DSMA_FREE = 0,
  DSMA_REUSE,
  DSMA_ALLOC,
  DSMA_USED,
  DSMA_ALLOC_SPECIAL,
  DSMA_USED_SPECIAL,
  DSMA_START_SPECIAL,
  DSMA_START_REUSE,
  DSMA_START_ALLOC,
  DSMA_START_ADD_RECYCLE  
}dsma_state_enum_type;

typedef struct dsma_block_list_type
{
  uint8 aug_pool_id;        /* augment pool id */
  uint8 reference_count;    /* used when more buffers fit in a block */
  uint8 init_buffer_count; /* number of buffers initialized */
  uint8 block_id;         /* type of block*/
  void *buffer_array[6];    /* Pointer to buffer memory array
                                 if more buffers fit in a block */
  void *block_array[6];     /* Pointer to common block array
                                   if more blocks fit in a buffer */
}dsma_block_list_type;

typedef struct dsma_free_list_type
{
  uint32 cnt;           /* Free count in buffer */
  dsm_item_type* top_item;     /* Pointer to top free item in buffer */
}dsma_free_list_type;

typedef struct dsm_augment_buffer_type
{
  dsma_state_enum_type dsma_state; /* state of buffer */
  void* block;  /* Pointer to augment buffer memory array */
  /* top free item and free count are updated atomically */
  union{
  uint64 free_list;
  dsma_free_list_type dsma_free_list;
  };
} dsm_augment_buffer_type;

typedef struct dsm_augment_pool_data_s
{
  uint32 scan_buffer_count;         /* trigger to scan blocks for allocation */
  dsm_item_type* alloc_top_free;    /* Augment allocation head*/
  struct dsm_pool_mgmt_table_s * pool; /* Pool reference */
  uint32 augment_cnt;               /* Dynamic Augment count for the pool */
  uint32 free_dbuffer_level;       /* Count to free reuse buffers in system */

  dsm_item_type* recycle_top_free;  /* Preallocated buffer 1 - reuse/recycle dbuffer*/
  dsm_item_type* dynamic_top_free;  /* Preallocated buffer 2 - reuse/new dbuffer */
  dsm_item_type* static_top_free;   /* Static Augment Buffer Head */
  dsm_augment_buffer_type buffer[DSMI_AUGMENT_TOTAL_BLOCKS];
                                    /* Array of augment buffers, for the pool
                                                                  Static augment buffers followed by
                                                                 dynamic augment buffers */
  uint32 dbuffer_count;             /* current dynamic buffer count */
  uint32 reuse_cnt;                 /* Count to re-use buffers */
  uint32 new_alloc;                 /* New allocation flag */
  int32 blocks_to_buffer_ratio;    /* Number of common pool pool blocks per buffer or vice versa */
  uint32 reuse_buffer_cnt;          /* number of reuse buffers in the pool  */
  uint32 reuse_trigger_cnt;         /* if 0 , do not trigger reuse
                                                                   else used to track augment buffer free activity  */
  boolean dsm_task_alloc;           /* Signal DSM task to allocate dbuffer */
  boolean dsm_task_free;            /* Signal DSM task to free dbuffer */

  uint32 dbuffer_limit;            /* Max allowed dynamic buffers
                                                                   based on pool count */
  uint32 sbuffer_trigger;   /* Static augment alloc buffer trigger count */
  uint32 sbuffer_cnt;          /* static augment items */
  /* Statistics */
  uint32 max_dbuffer_count_interval; /* Max dbuffers used after no reuse */
  uint32 stats_max_dbuffer_used; /* Max dynamic augment buffers used */

  uint32 buffer_available_cnt; /* Count of buffers available */
  dsma_block_list_type* block_list[DSMI_AUGMENT_TOTAL_BLOCKS];
  uint32 num_recycle_buffers; /* Count of added recycle buffers */
  dsm_item_type* recycle_buffers[3]; /* Additional recycle buffers */
}dsm_augment_pool_data_type;


/**********************DSMA MACROS **********************/
typedef struct dsm_dynamic_pool_data_s
{
  struct dsm_pool_mgmt_table_s * pool; /* Pool reference */
  pool_trigger_free_cb_type trigger_free_cb; /* Client registered callback function */
  uint32 size; /* Memory in bytes for allocation for this pool */
  uint32 instance_id; /* Instance id of the allocated memory */
  uint32 release_state; /* Indicate whether dynamic pool is in Release state */
  uint32 released_items;/* when released items = item_count, memory could be released */
  uint8 *item_array[DSMI_MAX_DYNAMIC_BUFFERS];/* Pointer to dynamic buffer memory array */
  uint32 count; /* number of buffers */
  dsm_augment_buffer_type* augment_pool;
}dsm_dynamic_pool_data_type;


/********************* FEATURE_DSM_AUGMENT END ****************************/


#ifdef FEATURE_DSM_AUGMENT
/*========================================================================
FUNCTION dsmi_augment_check_flow_control_levels()

DESCRIPTION
 Check whether the flow control levels matches the current dsm augment
   configuration

DEPENDENCIES
 None

PARAMETERS
  table - pool
  available_item_count - Flow control level

RETURN VALUE
  Return whether the available item count matches the current dsm augment
  configuration

SIDE EFFECTS None
=========================================================================*/
boolean dsmi_augment_check_flow_control_levels(dsm_pool_mgmt_table_type* table,
          uint32 available_item_count);

/*===========================================================================
FUNCTION DSM_AUGMENT_EXTERNAL_MEM_INIT()

DESCRIPTION
  This function will initialize DSM Pool, if augmented from
  external memory
  This function is called
   - if Heap memory is not available when DSM is being initialized.
   - if heap is not initialized
   - if static DSM memory is only needed.
DEPENDENCIES
  None
PARAMETERS
  None
RETURN VALUE
  None
SIDE EFFECTS
  Should be called after external memory and dsm pool is initialized
===========================================================================*/
void dsm_augment_external_mem_init(boolean alloc_from_heap);

/*========================================================================
FUNCTION dsmi_augment_init_pool()

DESCRIPTION
 Initialize DSM augment data structures for the pool.

DEPENDENCIES
 Pool is initialized.
 External memory manager is initialized.

PARAMETERS
  table - DSM pool to augment
  augment_count - Augment item count for the pool
  augment_items - Total DSM items to be allocated from augment pool
  static_augment_items - number of static augment items
  static_augment_item_array - Address of static pool, from which Static Augment buffer can be allocated.

RETURN VALUE
  None

SIDE EFFECTS
  This function is NOT threadsafe.
=========================================================================*/
void dsmi_augment_init_pool(dsm_pool_mgmt_table_type* table,
        uint32 augment_count,
        uint32 augment_items,
        uint32 static_augment_items,
        uint8* static_augment_item_array);

/*========================================================================
FUNCTION dsma_alloc_dynamic_reuse_recycle_buffer()

DESCRIPTION
  Traverse through reused/used dynamic buffers and try to reuse and
  recycle buffers.

DEPENDENCIES

PARAMETERS
  aug_data - Augment data of the pool

RETURN VALUE  None

SIDE EFFECTS
=========================================================================*/
void dsma_alloc_dynamic_reuse_recycle_buffer(
        dsm_augment_pool_data_type *aug_data);

/*========================================================================
FUNCTION DSM_AUGMENT_POOLS()

DESCRIPTION
  This API is called from High Priority DSM task to augment pool memory from external memory.
  Also called from dsm_init()

DEPENDENCIES

PARAMETERS

RETURN VALUE  None

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe.
  Pool's item  count and free count are modified.
=========================================================================*/
void dsm_augment_pools(void);

/*========================================================================
FUNCTION DSM_AUGMENT_FREE_BUFFER()

DESCRIPTION
    This API is called from High Priority DSM task after augment pool memory
    to perform necessary cleanup of freed buffers.

DEPENDENCIES

PARAMETERS

RETURN VALUE
  boolean - False, indicates, there are more reuse buffers to be freed,
                 True , indicates all reuse buffers are freed.

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe.
  Pool's item count and free count are modified.
=========================================================================*/
boolean dsm_augment_free_buffer(void);

/*===========================================================================
FUNCTION dsm_task_augment_alloc_req()

DESCRIPTION
   The function is called from DSM library to signal allocation request
   from external memory to DSM Task
	
DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsm_task_augment_alloc_req(void);

/*===========================================================================
FUNCTION dsm_task_augment_free_req()

DESCRIPTION
  The function is called from DSM library to signal free request
   to DSM Task to free buffers to external memory.

DEPENDENCIES
  None

PARAMETERS
  None

RETURN VALUE
  None

SIDE EFFECTS
  None
===========================================================================*/
void dsm_task_augment_free_req(void);


#endif /* FEATURE_DSM_AUGMENT */



/*========================================================================
FUNCTION DSM_GET_EXTERNAL_MEMORY_USAGE()

DESCRIPTION
    This API is 

DEPENDENCIES

PARAMETERS

RETURN VALUE
  Returns usage in bytes

SIDE EFFECTS
=========================================================================*/
uint32 dsm_get_external_memory_usage(void);


/*========================================================================
FUNCTION DSM_DYNAMIC_POOL_INIT()

DESCRIPTION
    This API is called to intitialize dynamic pool

DEPENDENCIES

PARAMETERS

RETURN VALUE

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe.
=========================================================================*/
void dsmi_dynamic_pool_init(dsm_pool_mgmt_table_type* table);



/*========================================================================
FUNCTION DSM_AUGMENT_ALLOC_DYNAMIC_POOL_MEMORY()

DESCRIPTION
 Allocate memory for Dynamic pool from external memory/heap

DEPENDENCIES

PARAMETERS

RETURN VALUE  TRUE , if memory is allocated after doing external memory resource checks
False , if memory could not be allocated.

SIDE EFFECTS
=========================================================================*/
boolean dsm_augment_alloc_dynamic_pool_memory(dsm_pool_mgmt_table_type* table);

/*========================================================================
FUNCTION DSM_AUGMENT_DYNAMIC_POOL_FREE_BUFFER()

DESCRIPTION
    This API is called to return memory of dynamic pools

DEPENDENCIES

PARAMETERS

RETURN VALUE
     True, if all items of the pool are released by clients , so that memory can be released.
     False, if some of the items are still allocated and clients need to free them before memory can be released.

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe.
=========================================================================*/
boolean dsm_augment_dynamic_pool_free_buffer(void);

/*========================================================================
FUNCTION DSM_TASK_DYNAMIC_FREE_REQ()

DESCRIPTION
    This API is called to trigger DSM task to free dynamic pool memory

DEPENDENCIES

PARAMETERS

RETURN VALUE  
                 
SIDE EFFECTS
=========================================================================*/
void dsm_task_dynamic_free_req(void);

/*===========================================================================
FUNCTION DSM_AUGMENT_ADD_COMMON_POOL_MEMORY()

DESCRIPTION
  This function will initialize common pool memory for augment pools
  This function is called
   - During DSM initialization
DEPENDENCIES
  Any memory added which is not fitting the block size is wasted
  Block size - DSMI_COMMON_POOL_BLOCK_SIZE
PARAMETERS
  array - Memory
  size - Size in bytes of memory
RETURN VALUE
  None
SIDE EFFECTS
  Should be called during DSM initialization
===========================================================================*/
void dsm_augment_add_common_pool_memory(uint8* array,uint32 size);
#endif /* DSMI_DYNAMIC_H_ */
