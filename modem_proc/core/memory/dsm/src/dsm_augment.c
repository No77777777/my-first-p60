/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                                  D S M _ A U G M E N T . C

GENERAL DESCRIPTION
  DMSS Data Services memory pool module.

EXTERNALIZED FUNCTIONS

The module contains helper functions to augment DSM memory pool at run time.

INITIALIZATION AND SEQUENCING REQUIREMENTS

  dsm_init() must be called prior to any other DSM function.

Copyright (c) 2013 - 2014 by Qualcomm Technologies, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*/

/*===========================================================================

                        EDIT HISTORY FOR MODULE

  $Header: //components/rel/core.mpss/10.0/memory/dsm/src/dsm_augment.c#1 $

when        who    what, where, why
--------    ---    ----------------------------------------------------------
03/13/13    sh     Implemented FEATURE_DSM_AUGMENT
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
/* Target-independent Include files */
#include "core_variation.h"
#include "comdef.h"
#include "dsm_pool.h"
#include "dsm_item.h"
#include "dsmi.h"
#include "dsm_lock.h"
#include "memheap.h"
#include "modem_mem.h"
#include "msg.h"
#include "err.h"


/* If using heap as external memory */
/* start */
extern mem_heap_type modem_mem_heap;
/* end */



#ifdef FEATURE_DSM_NATIVE_ASSERT
#define ASSERT( cond )      assert( cond )
#endif

/*===========================================================================

                EXTERNAL  DEFINITIONS AND DECLARATIONS FOR MODULE

 ==========================================================================*/

/* All DSM augment definitions in dsmi.h */


/*===========================================================================

                INTERNAL  DEFINITIONS AND DECLARATIONS FOR MODULE

==========================================================================*/

static uint32 dsma_aug_bytes = 0;
static uint32 dsma_aug_bytes_dynamic_pools = 0;
uint32 dsma_ext_memory_check_lower_limit;
uint32 dsma_ext_memory_dynamic_alloc_check_lower_limit = 0;
uint32 dsma_ext_memory_alloc_upper_limit;
uint32 dsma_ext_memory_upper_limit;

static int16 dsma_dynamic_pool_cnt = 0;
dsm_dynamic_pool_data_type dsma_dyn_pools[DSMI_MAX_DYN_POOLS] __attribute__((aligned(8)));

/* DSM Augment statistics */
#define DSM_EVENTS_BUFFER_CNT 20
typedef enum
{
  DSM_MEM_EVENTS_NONE = 0,
  DSM_MAXALLOC_LIMIT_REACHED,
  DSM_EXTTMEM_LIMIT_REACHED,
  DSM_EXTMEM_ALLOC_FAILED,
  DSM_POOL_CNT_LIMIT_REACHED,
  DSM_POOL_ALLOC,
  DSM_POOL_FREE,
  DSM_POOL_DYNAMIC_MEMORY_RELEASE_TRIGGERED
} dsm_mem_alloc_event_type;

typedef struct
{
  dsm_mempool_id_type pool_id;
  uint32 pool_item_cnt;
  uint32 pool_free_cnt;
  uint32 pool_alloc_cnt;
} dsm_mem_pool_statistics_type;

typedef struct
{
  dsm_mem_alloc_event_type mem_alloc_event;
  uint32 ext_mem_dsm_allocated_bytes;
  uint32 ext_mem_current_used_bytes;
  dsm_mem_pool_statistics_type pool_stats;
} dsm_stats_entry_type;

typedef struct
{
  uint32 stats_extmem_alloc_failed_cnt; /* Cumulative count of DSM_EXTMEM_ALLOC_FAILED event*/
  uint32 stats_dsm_maxalloc_reached_cnt; /* Cumulative count of DSM_MAXALLOC_LIMIT_REACHED event*/
  uint32 stats_extmem_limit_reached_cnt; /* Cumulative count of DSM_EXTTMEM_LIMIT_REACHED event*/
  uint32 stats_events_next_entry;           /* Next entry in stats event buffer */
  dsm_stats_entry_type dsm_stats_events_buffer[DSM_EVENTS_BUFFER_CNT]; /* Circular buffer of event statistics */
} dsm_statistics_data_type;

#ifdef FEATURE_DSM_MEM_CHK
dsm_statistics_data_type dsm_event_stats;
#endif /* #ifdef FEATURE_DSM_MEM_CHK*/


/* Forward declarations START */
static void dsma_update_statistics(dsm_mem_alloc_event_type event,
         dsm_pool_mgmt_table_type *table);

void dsm_dynamic_external_memory_deregister_high_usage_cb(void);
void dsm_dynamic_external_memory_register_high_usage_cb(void);
void dsm_dynamic_pools_trigger_free(void);
/* Forward declarations END */



#ifdef FEATURE_DSM_AUGMENT
static uint32* dsma_common_pool_free_list = NULL;
static uint32 dsm_common_pool_block_size = 0;
static uint32 dsm_common_pool_block_count = 0;

/* Pointer to dynamic buffer memory array */
static int16 dsma_augpool_cnt;
dsm_augment_pool_data_type dsma_aug_pools[DSMI_MAX_AUG_POOLS] __attribute__((aligned(8)));
#endif


/*-----------------------------------------------------------------
 * External memory  allocation/free START
 * ------------------------------------------------------------------*/

static inline void dsma_free(void* ptr)
{
  modem_mem_free(ptr,4);
}

static inline void * dsma_alloc(uint32 size)
{
  return modem_mem_alloc(size,4);
}
uint32 dsm_get_external_memory_usage(void)
{
 return modem_mem_heap.heap_used_bytes;
}

void dsm_get_external_memory_max_alloc(void)
{ /* Consider fragmentation and other usage */
    uint32 new_limit = modem_mem_heap.total_bytes - DSMI_AUGMENT_EXT_MEM_BACK_OFF;
    if (dsma_ext_memory_upper_limit != new_limit)
    {
      dsma_ext_memory_check_lower_limit = 0;
      dsma_ext_memory_upper_limit = new_limit;
      dsma_ext_memory_alloc_upper_limit = dsma_ext_memory_upper_limit - DSMI_AUGMENT_EXT_MEM_ALLOC_BACK_OFF;
    }
}
void dsma_register_high_usage_callback(void* fn,uint32 cb_threshold)
{
   /* Pseudo external mem function */
   modem_mem_register_high_usage_callback(fn,cb_threshold);

};

void dsma_deregister_high_usage_callback(void)
{
    /* Pseudo external mem function */
    modem_mem_deregister_high_usage_callback();
};

#ifdef FEATURE_DSM_AUGMENT
static inline
uint8 * dsma_get_common_pool_block(void)
{
  uint32 *block = dsma_common_pool_free_list;
  dsma_common_pool_free_list = (uint32*)(*block);
  dsm_common_pool_block_count--;
  return (uint8 *)block;
}

static inline
void dsma_free_common_pool_block( void *free_block_ptr)
{
    *((uint32*)free_block_ptr) = (uint32)dsma_common_pool_free_list;
    dsma_common_pool_free_list = free_block_ptr;
    dsm_common_pool_block_count++;
}
dsma_block_list_type* dsma_get_block_list_head(void *free_block_ptr)
{
	size_t len = sizeof(dsma_block_list_type);
	/* Make sure length is not more than 56 bytes */
	return (dsma_block_list_type*)((uint8*)free_block_ptr + dsm_common_pool_block_size - len);
}
void dsma_init_block_list_head( void *free_block_ptr)
{
	dsma_block_list_type* start_cpy = dsma_get_block_list_head(free_block_ptr);
	memset((void*)start_cpy,0,sizeof(dsma_block_list_type));
}

#endif /* FEATURE_DSM_AUGMENT */
/*-----------------------------------------------------------------
 * External memory  allocation/free END
 * ------------------------------------------------------------------*/

/*========================================================================
FUNCTION dsma_check_ext_memory_limits()

DESCRIPTION
  Check the configured limits on external memory
   against current usage of external memory

  Threshold is defined to reenable DSMA allocation after external memory limit is reached,
  initialized in   dsma_ext_memory_check_lower_limit

DEPENDENCIES

PARAMETERS

RETURN VALUE
 Current heap allocation ,If Limit is not reached, 0 -  If external memory limits are reached.

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe
=========================================================================*/
static inline uint32 dsma_check_ext_memory_limits(void)
{
    uint32 lheap_alloc;

    lheap_alloc = dsm_get_external_memory_usage();

    if (!lheap_alloc)
    {
      lheap_alloc = 1;
    }
    if (dsma_ext_memory_dynamic_alloc_check_lower_limit)
    {
      if (lheap_alloc < dsma_ext_memory_dynamic_alloc_check_lower_limit )
      {

          dsma_ext_memory_dynamic_alloc_check_lower_limit = 0;
      }
    }

    if (DSM_UNLIKELY(lheap_alloc > dsma_ext_memory_upper_limit))
    {
       /*dsma_ext_memory_check_lower_limit =
            lheap_alloc - (DSMI_AUGMENT_MAX_ALLOC>>1);*/

       /* Wait till external memory is reduced to dsma_ext_memory_check_lower_limit for allocation */
       MSG_HIGH("DSMA: External Memory Upper Limit %d reached \n \n",lheap_alloc,0,0);
       lheap_alloc = 0;
    }
    else if (DSM_UNLIKELY(dsma_ext_memory_check_lower_limit))
    {
      if (lheap_alloc<dsma_ext_memory_check_lower_limit)
      {
        /* Continue with DSM allocation */
        MSG_HIGH(" DSMA: External Memory Lower Limit %d reached \n \n",dsma_ext_memory_check_lower_limit,0,0);
        dsma_ext_memory_check_lower_limit = 0;
      }
      else
      {
        lheap_alloc = 0;
      }
    }
    return lheap_alloc;
}

/*========================================================================

FUNCTION dsmi_augment_init_new_buffer()

DESCRIPTION
 Initialize DSM item headers

DEPENDENCIES
 Pool is initialized.

PARAMETERS
  aug_buffer_index - Augment Buffer ID to fill in DSM item header
  buffer - Memory start address
  count - Number of dsm items
  item_size - Size of DSM item

RETURN VALUE
  Top free item in initialized buffer

SIDE EFFECTS
  This function is NOT threadsafe.
  Except for init time, the function is called only from DSM Task Context
=========================================================================*/
static inline dsm_item_type* dsmi_augment_init_new_buffer(
        uint8 aug_buffer_index,
        uint8* buffer,
        uint32 count,
        uint32 item_size
#ifndef FEATURE_DSM_LOCKLESS
        , dsm_item_type** stack_top
#endif
        )
{
  uint32 i;               /* Array iterator */
  dsm_item_type *item_ptr;     /* pointer to a dsm item  */
  void* top_free_item = NULL;

  /* Make a link of all items
   * Items are initialized at the time of allocation */
  item_ptr = (dsm_item_type *)buffer;
  for (i=0; i < count; i++)
  {
    item_ptr->pool_id = 0;
    item_ptr->references=0;
#ifdef FEATURE_DSM_LOCKLESS
    item_ptr->pool_id_ext = aug_buffer_index;
    item_ptr->app_ptr = top_free_item;
    top_free_item = item_ptr;
#else
    stack_top[i] = item_ptr;
#endif
    item_ptr = (dsm_item_type *)((uint8*)item_ptr +
            DSMI_POOL_OBJECT_SIZE_OPT(item_size));
  }
  return top_free_item;
}


#ifdef FEATURE_DSM_AUGMENT
/*========================================================================
 *  Functions invoked from DSM Library
 * ========================================================================*/

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
          uint32 available_item_count)
{
  dsm_augment_pool_data_type *aug_data = table->augment_data;
  if (aug_data)
  {
    if (available_item_count > ( aug_data->sbuffer_cnt + (aug_data->augment_cnt * 3)))
    {
        return FALSE;
    }
  }
  return TRUE;
}

/*========================================================================
FUNCTION dsmi_augment_get_dbuffer_cnt()

DESCRIPTION
 Get number of dynamic buffers required for the augment items

DEPENDENCIES
 None

PARAMETERS
 augment_items - Total DSM items from augment pool
 augment_count - Number of items in dbuffer

RETURN VALUE
  Number of dynamic buffers required

SIDE EFFECTS None
=========================================================================*/
inline static uint32 dsmi_augment_get_dbuffer_cnt(
        uint32 augment_count,
        uint32 augment_items)
{
  /* Make it multiple of 4 for static buffer calculation items */
  return ((augment_items + augment_count -1)/augment_count);
}

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
void dsm_augment_add_common_pool_memory(uint8* array,uint32 size)
{
    uint32 j,i=0;
    uint32 number_of_blocks;

    array = (uint8*)( ( (uint32)array + 63 ) & ~63 );
    if (!dsm_common_pool_block_size)
    {
      /* First time memory is added to common block */
      /* Initialization */
      dsm_common_pool_block_size = ((DSMI_COMMON_POOL_BLOCK_SIZE +63) & ~63);
      number_of_blocks= size/dsm_common_pool_block_size;
      j = size - number_of_blocks*dsm_common_pool_block_size;
      j = j/number_of_blocks;
      j=j/64;
      dsm_common_pool_block_size+=j*64;
    }
    else
    {
      number_of_blocks= size/dsm_common_pool_block_size;
    }

    while (i<number_of_blocks)
    {
    	dsma_init_block_list_head((void*)array);
        dsma_free_common_pool_block((void*)array);
        array+=dsm_common_pool_block_size;
        i++;
    }
}

/*===========================================================================
FUNCTION DSM_AUGMENT_EXTERNAL_MEM_INIT()

DESCRIPTION
  This function will initialize the Data Service Memory Pool, if augmented from 
  external memory
  This function is called 
   - if Heap memory is not available when DSM is being initialized.
   - if heap is not initialized
   - if static DSM memory is only needed.
DEPENDENCIES
  None
PARAMETERS
  alloc_from_heap - Is heap allocation allowed
RETURN VALUE
  None
SIDE EFFECTS
  Should be called after external memory and dsm pool is initialized
===========================================================================*/
void dsm_augment_external_mem_init(boolean alloc_from_heap)
{
  uint32 i;
  
  /* Allow heap allocation by disabling lower limit check */
  if (alloc_from_heap)
  {
    dsma_ext_memory_check_lower_limit = 0;
  }
  dsm_get_external_memory_max_alloc();
  
  dsm_augment_pools();
  for (i = 0; i < dsma_augpool_cnt; i++)
  {
    if ((!dsma_aug_pools[i].recycle_top_free) &&
        (dsma_aug_pools[i].dynamic_top_free))
    {
      dsma_aug_pools[i].recycle_top_free = dsma_aug_pools[i].dynamic_top_free; 
      dsma_aug_pools[i].dynamic_top_free = NULL;
      dsma_aug_pools[i].dsm_task_alloc = TRUE;      
      if ( DSMI_AUGMENT_RECYCLE_MIN_COUNT < (dsma_aug_pools[i].augment_cnt >> 2))
      {
          dsma_aug_pools[i].reuse_cnt= (dsma_aug_pools[i].augment_cnt >> 2);
      }
      else if ( DSMI_AUGMENT_RECYCLE_MIN_COUNT < ((dsma_aug_pools[i].augment_cnt >> 2) +
              (dsma_aug_pools[i].augment_cnt >> 3)) )
      {
          dsma_aug_pools[i].reuse_cnt= DSMI_AUGMENT_RECYCLE_MIN_COUNT;
      }
      else
      {
          dsma_aug_pools[i].reuse_cnt=((dsma_aug_pools[i].augment_cnt >> 2) +
                  (dsma_aug_pools[i].augment_cnt >> 3));
      }
    }
  }

  dsm_augment_pools();
}

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
  This function is NOT threadsafe.  Initializes static augment buffers.
=========================================================================*/
void dsmi_augment_init_pool(dsm_pool_mgmt_table_type* table,
        uint32 augment_count,
        uint32 augment_items,
        uint32 static_augment_items,
        uint8* static_augment_item_array)
{

  uint8 *table_end_address;
  dsm_augment_pool_data_type *aug_data;
  uint32 k,size = 0;
  uint32 common_block_size = dsm_common_pool_block_size;

  ASSERT(dsma_augpool_cnt < DSMI_MAX_AUG_POOLS) 

#ifdef FEATURE_DSM_MEM_CHK
  dsm_event_stats.stats_events_next_entry = 0;
#endif
  /* Disable heap allocation till heap is initialized 
     by setting lower limit */
  dsma_ext_memory_check_lower_limit = 1;

  if (augment_items !=0)
  {
    /* ASSERT (augment_count > 49 ); Remove the assert for Super Large items */

    aug_data = &dsma_aug_pools[dsma_augpool_cnt];

    memset(aug_data,0,
            sizeof(dsm_augment_pool_data_type));

    aug_data->pool = table;

    aug_data->augment_cnt = augment_count;

    aug_data->reuse_cnt = DSMI_AUGMENT_RECYCLE_MIN_COUNT;
    
    aug_data->sbuffer_cnt = static_augment_items;

    aug_data->free_dbuffer_level = augment_count +
        (augment_count * DSM_AUGMENT_REUSE_TRIGGER_FACTOR/100);

    if (aug_data->free_dbuffer_level > DSM_AUGMENT_REUSE_TRIGGER_FACTOR_MAX)
    {
      aug_data->free_dbuffer_level = augment_count;
    }

    table_end_address = table->item_array + table->item_count_static *
            (DSMI_POOL_OBJECT_SIZE_OPT(table->pool_item_size));
    
    /* These values are static and will not change */
    aug_data->static_top_free =
         dsmi_augment_init_new_buffer(DSMI_AUGMENT_TOTAL_BLOCKS,static_augment_item_array,
                                     aug_data->sbuffer_cnt,table->pool_item_size);
    static_augment_item_array +=
              (DSMI_POOL_OBJECT_SIZE_OPT(table->pool_item_size))*aug_data->sbuffer_cnt;
    table->free_count += aug_data->sbuffer_cnt;
    table->item_count += aug_data->sbuffer_cnt;

    ASSERT((static_augment_item_array-1) < table_end_address)
    aug_data->recycle_top_free = 0;
    aug_data->dynamic_top_free = 0;

    /* No need to take care of alignement here
     * as common pool blocks are 64 byte aligned *=*/
    common_block_size = dsm_common_pool_block_size;
    common_block_size-=(sizeof(dsma_block_list_type)+63)& ~63;
    size = DSMI_POOL_OBJECT_SIZE_OPT(table->pool_item_size)* aug_data->augment_cnt;
    aug_data->blocks_to_buffer_ratio = size/common_block_size;
    if (!aug_data->blocks_to_buffer_ratio)
    {
      k = common_block_size/size;
      if (k == 1)
      {
        aug_data->blocks_to_buffer_ratio++;
      }
    }

    if ( aug_data->blocks_to_buffer_ratio > 0)
    {
        ASSERT(aug_data->blocks_to_buffer_ratio <= 6 );
        /* Need couple of blocks to fit a buffer
         * Adjust augment count to fit block size
         * Augment count will be adjusted to less than
         * what is configured*/
        k = common_block_size/DSMI_POOL_OBJECT_SIZE_OPT(table->pool_item_size);
        if ((k*aug_data->blocks_to_buffer_ratio)  <
                (aug_data->augment_cnt - (aug_data->augment_cnt>>2)))
        {
            aug_data->blocks_to_buffer_ratio++;
        }
        aug_data->augment_cnt = k*aug_data->blocks_to_buffer_ratio;
    }
    else
    {
        /* More buffers fit in a common block */
        /* Augment buffers will be adjusted to more than what is configured */
        aug_data->blocks_to_buffer_ratio = common_block_size/size;
        ASSERT(aug_data->blocks_to_buffer_ratio <= 6 );
        k = common_block_size - aug_data->blocks_to_buffer_ratio*size;
        k=k/aug_data->blocks_to_buffer_ratio;
        aug_data->augment_cnt += k/DSMI_POOL_OBJECT_SIZE_OPT(table->pool_item_size);
        /* Keep it negative */
        aug_data->blocks_to_buffer_ratio =0-aug_data->blocks_to_buffer_ratio ;
    }
    aug_data->scan_buffer_count = aug_data->augment_cnt;
    aug_data->dbuffer_limit = dsmi_augment_get_dbuffer_cnt(augment_count,augment_items);

    aug_data->dbuffer_limit += 1; /*To allow for additional dynamic buffer*/

    ASSERT(aug_data->dbuffer_limit < DSMI_AUGMENT_TOTAL_BLOCKS);

    table->augment_data = (void *)aug_data;

    dsma_augpool_cnt++;
    /* Allocate the dynamic heap */
    /* Heap may be initialized with this call
         * dsm_init depends on heap initialization */
         
    aug_data->dsm_task_alloc = TRUE;
    aug_data->dsm_task_free = FALSE;
#ifdef FEATURE_DSM_TRIGGER_EXTERNAL_MEMORY_INIT
    dsm_augment_external_mem_init(FALSE);
#else
    dsm_augment_external_mem_init(TRUE);
#endif
 }

}

/*========================================================================

FUNCTION dsmi_augment_init_new_buffer()

DESCRIPTION
 Initialize DSM item headers

DEPENDENCIES
 Pool is initialized.

PARAMETERS
  aug_buffer_index - Augment Buffer ID to fill in DSM item header
  buffer - Memory start address
  count - Number of dsm items
  item_size - Size of DSM item

RETURN VALUE
  Top free item in initialized buffer

SIDE EFFECTS
  This function is NOT threadsafe.
  Except for init time, the function is called only from DSM Task Context
=========================================================================*/
static inline void dsmi_augment_init_pool_id(
       dsm_item_type* dsm_item_ptr,
       uint8 aug_buffer_index)
{
    dsm_item_type* next = dsm_item_ptr;
    while (next!=NULL)
    {
        next->pool_id_ext=aug_buffer_index;
        next=(dsm_item_type*)next->app_ptr;
    }
}
/*========================================================================
FUNCTION dsma_get_buffer_head()

DESCRIPTION
  Traverse through the array to find a non used buffer location

DEPENDENCIES

PARAMETERS
  aug_data - Augment data of the pool
  new_pool - Buffer head

RETURN VALUE
  Index of empty buffer head or DSMI_AUGMENT_TOTAL_BLOCKS, if none available

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe
=========================================================================*/
static inline uint32 dsma_get_buffer_head(
        dsm_augment_pool_data_type *aug_data,
        dsm_augment_buffer_type** new_pool)
{
    uint32 i = 0;
    *new_pool = NULL;

    while(i < (DSMI_AUGMENT_TOTAL_BLOCKS-1))
    {
      if (!aug_data->buffer[i].dsma_state)
      {
        *new_pool =  &aug_data->buffer[i];
        return i;
      }
      i++;
    }
    return DSMI_AUGMENT_TOTAL_BLOCKS;
}

/*========================================================================
FUNCTION dsma_free_to_common_pool()

DESCRIPTION
  Free buffer to common pool

DEPENDENCIES

PARAMETERS
  aug_data - Augment data of the pool
  size - size in bytes of augment buffer
  pool id - augment pool id
  block_list_index - index to block list
  void* top_free_item;

RETURN VALUE
  None

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe
=========================================================================*/
static void dsma_free_to_common_pool(
        dsm_augment_pool_data_type *aug_data,
        uint32 size,
        uint32 pool_id,
        uint32 block_list_index,
        void* top_item)
{
    uint32 no_of_buffers,j,block_index,buffer_index;
    dsma_block_list_type* block;
    if (aug_data->blocks_to_buffer_ratio > 0) 
    {
        if (block_list_index < DSMI_AUGMENT_TOTAL_BLOCKS /* KW */)
        {
          no_of_buffers = aug_data->blocks_to_buffer_ratio;
          block = aug_data->block_list[block_list_index];
          block->buffer_array[0]=top_item;
          /* Delete the first one last as it has the memory for
           * block */
          j = 1;
          while (j < no_of_buffers)
          {
            dsma_init_block_list_head(block->block_array[j]);
            dsma_free_common_pool_block(block->block_array[j]);
            j++;
          }
          dsma_free_common_pool_block(block->block_array[0]);
          if (no_of_buffers != 1)
          {
             dsma_init_block_list_head(block->block_array[0]);
          }

          aug_data->block_list[block_list_index]=NULL;
        }
    }
    else /* More buffers in 1 block */
    {
        no_of_buffers = 0-aug_data->blocks_to_buffer_ratio;
        aug_data->buffer_available_cnt++;
        block_index = block_list_index/32;
	if (block_index < DSMI_AUGMENT_TOTAL_BLOCKS /* KW */)
        {
          block = aug_data->block_list[block_index];
          buffer_index = block_list_index%32;
        
          block->reference_count++;
          if ( buffer_index < 6) /* only for KW  ; not a real issue */
          {
            block->buffer_array[buffer_index] = top_item;
          }
          if (block->reference_count == no_of_buffers)
          {
            dsma_free_common_pool_block(block->block_array[0]);
            aug_data->buffer_available_cnt-=no_of_buffers;
            aug_data->block_list[block_index]=NULL;
          }
      }
    }
}

/*========================================================================
FUNCTION dsma_switch_dynamic_pool_to_common_pool()

DESCRIPTION
  Switch dynamic pool to non heap buffer

DEPENDENCIES

PARAMETERS
  aug_data - Augment data of the pool

RETURN VALUE
  None

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe
=========================================================================*/
static void dsma_switch_dynamic_pool_to_common_pool(
        dsm_augment_pool_data_type *aug_data
        )
{
     uint32 j,k,total,size;
     dsm_augment_buffer_type* free_buffer;
     dsm_item_type* top_item, *alloc_top;
     dsma_block_list_type* free_block;

     top_item = aug_data->dynamic_top_free;
     alloc_top = aug_data->alloc_top_free;
     if (top_item)
     {
       free_buffer = &aug_data->buffer[top_item->pool_id_ext-1];
       if (((uint32)free_buffer->block)>8192)
       {
           if ((aug_data->reuse_buffer_cnt) &&
               (aug_data->recycle_top_free) &&
               (aug_data->alloc_top_free) &&
               (top_item->app_ptr !=NULL))
         {
           /* Now we can switch dynamic pool */
           j = DSMI_AUGMENT_TOTAL_BLOCKS-1;
           total = aug_data->dbuffer_count;
           while ((total > 0) && (j >0))
           {
             if (aug_data->buffer[j].dsma_state)
             {
                total--;
                if (aug_data->buffer[j].dsma_state == DSMA_REUSE)
                {
                  free_buffer = &aug_data->buffer[j];
                  if ((uint32)free_buffer->block<8192)
                  {
                    if (atomic_compare_and_set(&free_buffer->dsma_state, DSMA_REUSE, DSMA_ALLOC))
                    {
                      /* No allocation happened */
                      if ((aug_data->alloc_top_free == alloc_top) &&
                       (atomic_compare_and_set(&aug_data->dynamic_top_free,top_item,free_buffer->dsma_free_list.top_item)))
                      {
                        atomic_dec(&aug_data->reuse_buffer_cnt);
                        free_buffer->free_list = 0;
                        free_buffer=&aug_data->buffer[top_item->pool_id_ext-1];
                        atomic_set(&free_buffer->dsma_state,DSMA_FREE);
                        /* from heap */
                        size = DSMA_GET_AUGMENT_BUFFER_SIZE(aug_data->pool->pool_item_size,
                                                       aug_data->augment_cnt);
                        if (size > dsm_common_pool_block_size)
                        {
                          k = 1;
                          free_block = (dsma_block_list_type*)(*((uint32*)free_buffer->block));
                          while (k < aug_data->blocks_to_buffer_ratio)
                          {
                            dsma_free(free_block->block_array[k]);
                            k++;
                          }
                          dsma_free(free_block->block_array[0]);
                          *((uint32*)free_buffer->block)=NULL;
                          dsma_aug_bytes -= dsm_common_pool_block_size *aug_data->blocks_to_buffer_ratio;
                          /* update lower_limit_values */
                          dsma_check_ext_memory_limits();
                        }
                        else
                        {
                          dsma_free(free_buffer->block);
                          dsma_aug_bytes -= size;
                        }
                        if (!dsma_aug_bytes)
                        {
                          dsm_dynamic_external_memory_deregister_high_usage_cb();
                        }
                        memset(free_buffer,0, sizeof(dsm_augment_buffer_type));
                        aug_data->dbuffer_count --;
                        if(aug_data->dbuffer_count < 4)
                        {
                          aug_data->scan_buffer_count = aug_data->augment_cnt;
                        }
                        break;
                      }
                      else
                      {
                        free_buffer->dsma_state = DSMA_REUSE;
                      }
                    }
                  }
                }
             }
             j--;
          }
       }
    }
  }

}
/*========================================================================
FUNCTION dsma_allocate_from_common_pool()

DESCRIPTION
  Allocate from common pool for the augment buffer

DEPENDENCIES

PARAMETERS
  aug_data - Augment data of the pool
  size - to be allocated
  pool_id - Pool id extension of the buffer
  aug_pool_id - Index of augment pool

RETURN VALUE
  Boolean to indicate allocation succeeded

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe
=========================================================================*/
static boolean dsma_allocate_from_common_pool(
        dsm_augment_pool_data_type *aug_data,
        dsm_augment_buffer_type* new_pool,
        uint32 pool_id,
        uint8 aug_pool_id)
{
    boolean allocated = FALSE;
    uint32 no_of_buffers,i,j,block_index;
    dsm_item_type* top_free_item_new=0;
    dsm_item_type* top_free_item_old =0;
    void* buffer =0;
    void* temp_buffer;
    uint32 size,k=0;
    uint8* item_array_start;
    dsm_pool_mgmt_table_type *table;
    dsma_block_list_type* block = NULL;

    table=aug_data->pool;
    /*Allocate from external memory */
    if(!aug_data->dynamic_top_free)
    {
      /* This will return a memory aligned to 32 bytes */
      if (aug_data->blocks_to_buffer_ratio > 0)
      {
          no_of_buffers = aug_data->blocks_to_buffer_ratio;
          if (dsm_common_pool_block_count >= no_of_buffers)
          {
            /* find an empty block in list */
            for (i = 0; i < DSMI_AUGMENT_TOTAL_BLOCKS; i++)
            {
              if(!aug_data->block_list[i])
              {
                  k = i;
                  break;
              }
            }
            if (i == DSMI_AUGMENT_TOTAL_BLOCKS )
            {
              return allocated;
            }
            top_free_item_old = NULL;
            j= 0;
            while (j < no_of_buffers)
            {
              if (!block)
              {
            	/* First time  and buffer will not be 0 */
                buffer = dsma_get_common_pool_block();
            	block= dsma_get_block_list_head(buffer);
            	aug_data->block_list[k] = block;
            	if (no_of_buffers == 1)
            	{
            	  if (block->aug_pool_id  == aug_pool_id)
            	  {
            	    /* We got an initialized block, which can be used directly*/
                    block->block_array[0]=buffer;
            	    top_free_item_new = block->buffer_array[0];
            	    if (top_free_item_new->pool_id_ext != (pool_id+1))
            	    {
            	      dsmi_augment_init_pool_id(top_free_item_new,pool_id+1);
            	    }
            	    block->buffer_array[0]=0;
            	    allocated = TRUE;
            	    break;
            	  }
            	}
              }
              else
              {
            	buffer = dsma_get_common_pool_block();
              }
              dsma_init_block_list_head(buffer);
              block->block_array[j]=buffer;
              item_array_start = buffer;
              item_array_start+=DSM_TAIL_COOKIE_LEN + table->pad_size ;
              top_free_item_new = dsmi_augment_init_new_buffer(pool_id+1,
                            item_array_start,aug_data->augment_cnt/no_of_buffers,
                            table->pool_item_size);
              ((dsm_item_type*)item_array_start)->app_ptr = top_free_item_old;
              top_free_item_old = top_free_item_new;
              j++;
            }
            block->reference_count = no_of_buffers;
            block->aug_pool_id = aug_pool_id;
            block->init_buffer_count=1;
            allocated = TRUE;
            block_index = k;
        }
      }
      else
      {
          /* More buffers in 1 block */
          no_of_buffers = 0-aug_data->blocks_to_buffer_ratio;

          if (aug_data->buffer_available_cnt)
          {
            i = 0;
            while((i < DSMI_AUGMENT_TOTAL_BLOCKS) && (!allocated))
            {
              if(aug_data->block_list[i])
              {
            	block = aug_data->block_list[i];
                if ((block->reference_count!=0)
                    && (block->reference_count < no_of_buffers))
                {
                  /* More blocks in there , not yet allocated */
                  j = 0;
                  while (j< no_of_buffers)
                  {
                    if (block->buffer_array[j] )
                    {
                      if ( j<block->init_buffer_count)
                      {
                    	buffer=0;
                    	top_free_item_new=block->buffer_array[j];
                    	if (top_free_item_new->pool_id_ext != (pool_id+1))
                    	{
                    	  dsmi_augment_init_pool_id(top_free_item_new,pool_id+1);
                    	}
                    	block->buffer_array[j]=NULL;
                      }
                      else
                      {
                        buffer = block->buffer_array[j];
                        block->buffer_array[j] = NULL;
                        block->init_buffer_count++;
                      }
                      block_index = i*32+j;
                      block->reference_count--;
                      allocated = TRUE;
                      aug_data->buffer_available_cnt--;
                      break;
                    }
                    j++;
                  }
                }
              }
              i++;
            }
          }
          else
          {
             for (i = 0; i < DSMI_AUGMENT_TOTAL_BLOCKS; i++)
             {
               if(!aug_data->block_list[i])
               {
                 k = i;
                 break;
               }
             }
             if (i == DSMI_AUGMENT_TOTAL_BLOCKS )
             {
               return allocated;
             }
          }

          if ((!allocated) && (k<DSMI_AUGMENT_TOTAL_BLOCKS))
          {
              buffer = dsma_get_common_pool_block();
              if (buffer)
              {
                block= dsma_get_block_list_head(buffer);
                aug_data->block_list[k] = block;
                block_index = k*32;/*+0*/
                allocated = TRUE;
                if (block->aug_pool_id == aug_pool_id)
                {
                  /* We got an initialized block, which can be used directly*/
                  top_free_item_new = block->buffer_array[0];
                  block->buffer_array[0] = NULL;
                  if (top_free_item_new->pool_id_ext != (pool_id+1))
                  {
                    dsmi_augment_init_pool_id(top_free_item_new,pool_id+1);
                  }
                  block->reference_count = no_of_buffers-1;
                  aug_data->buffer_available_cnt+=no_of_buffers-1;
                  buffer = 0;
                }
                else
                {
                  dsma_init_block_list_head(buffer);
                  block->block_array[0]=buffer;
                  block->reference_count = no_of_buffers-1;
                  block->init_buffer_count=1;
                  block->aug_pool_id =aug_pool_id;
                  size = aug_data->augment_cnt*DSMI_POOL_OBJECT_SIZE_OPT(table->pool_item_size);
                  i = 1;
                  temp_buffer=buffer;
                  while (i < no_of_buffers)
                  {
                    temp_buffer=(uint8*)temp_buffer+size;
                    block->buffer_array[i] = temp_buffer;
                    i++;
                    aug_data->buffer_available_cnt++;
                  }
                  block->buffer_array[0] = 0;
                }
              }
          }
          if (buffer)
          {
            /* No need to worry about alignment */
            buffer=(uint8*)buffer+DSM_TAIL_COOKIE_LEN + table->pad_size ;
            top_free_item_new =
              dsmi_augment_init_new_buffer(pool_id+1,
                 buffer,aug_data->augment_cnt,
                 table->pool_item_size);
          }
        }
        if (allocated)
        {
            MSG_HIGH_DSM_CONFIG(" DSMA alloc_new_buffer id %d dbuffer_cnt %d dbuffer_limit %d",
                           pool_id,aug_data->dbuffer_count,aug_data->dbuffer_limit);
            new_pool->block = (void*)block_index; /* The item array pointer is only used to delete */
            /* No need to worry about alignment */
            new_pool->dsma_free_list.top_item = top_free_item_new;
        }
      }
      return allocated;
}

/*========================================================================
FUNCTION dsm_allocate_from_heap()

DESCRIPTION
  Allocate from heap for the augment buffer
DEPENDENCIES

PARAMETERS
  aug_data - Augment data index
  size - to be allocated
  pool_id - Pool id extension of the buffer

RETURN VALUE
  Boolean to indicate allocation succeeded

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe
=========================================================================*/
static boolean dsm_allocate_from_heap(
        uint32 aug_index,
        dsm_augment_buffer_type* new_pool,
        uint32 pool_id,
        uint32 heap_alloc)
{
  boolean allocated = FALSE;
  void* buffer,*temp_buffer;
  uint32 size,i,j,k;
  uint8* item_array_start;
  dsm_pool_mgmt_table_type *table;
  dsm_item_type* top_free_item_new,*top_free_item_old;
  dsm_augment_pool_data_type *aug_data;
  dsma_block_list_type* block = NULL;


  aug_data = &dsma_aug_pools[aug_index];
  table=aug_data->pool;
  top_free_item_new = NULL;

  /* This will return a memory aligned to 64 bytes */
  size = DSMA_GET_AUGMENT_BUFFER_SIZE(table->pool_item_size,
        aug_data->augment_cnt);

  if ((dsma_aug_bytes + size > DSMI_AUGMENT_MAX_ALLOC ) ||
         (heap_alloc + size > dsma_ext_memory_alloc_upper_limit))
  {
       if (dsma_aug_bytes + size > DSMI_AUGMENT_MAX_ALLOC)
       {
         MSG_HIGH("DSMA: Exceeded total bytes that can be allocated from ext memory pool %x size %d ",table,dsma_aug_bytes + size,0);
         dsma_update_statistics(DSM_MAXALLOC_LIMIT_REACHED,table);
       }
       else
       {
         MSG_HIGH("DSM Library Config Warning: Low item count in Pool 0x%x used %d. Contact DSM team",table,dsma_aug_bytes + size,0);
         dsma_update_statistics(DSM_EXTTMEM_LIMIT_REACHED,table);
       }
       aug_data->dsm_task_alloc = FALSE;
       return allocated;
  }

  if (size > dsm_common_pool_block_size)
  {
         /* find an empty block in list */
         for (i = 0; i < DSMI_AUGMENT_TOTAL_BLOCKS; i++)
         {
           if(!aug_data->block_list[i])
           {
            k = i;
            break;
           }
         }
         if (i == DSMI_AUGMENT_TOTAL_BLOCKS )
         {
           return allocated;
         }
         allocated = TRUE;
         top_free_item_old = NULL;       
         j = 0;
         while (j < aug_data->blocks_to_buffer_ratio)
         {
           if (!block)
           {
             /* first time */
             buffer = dsma_alloc(dsm_common_pool_block_size + 128 +
                     sizeof(dsma_block_list_type));
             if (buffer)
             {
               temp_buffer = (uint8*)( ( (uint32)buffer + 63 ) & ~63 );
               block =  dsma_get_block_list_head(temp_buffer);
               dsma_init_block_list_head(temp_buffer);
               block->block_array[0]=buffer;
               aug_data->block_list[k]=block;
             }
           }
           else
           {
               block->block_array[j] = dsma_alloc(dsm_common_pool_block_size + 64);
           }
           if ((!block) || (!block->block_array[j]))
           {
             /* Warning : i is changed inside this loop as the loop breaks here. Not so great code, but it works !*/
               dsma_update_statistics(DSM_EXTMEM_ALLOC_FAILED,table);
               i= 1;
               while((block) &&(i<j))
               {
                   dsma_free(block->block_array[i]);
                   i++;
               }
               if (block)
               {
                 dsma_free(block->block_array[0]);
                 aug_data->block_list[k]=NULL;
               }
               aug_data->dsm_task_alloc = FALSE;
               allocated=FALSE;
               break;
           }

           item_array_start = block->block_array[j];
           item_array_start = (uint8*)( ( (uint32)item_array_start + 63 ) & ~63 );
           item_array_start+=DSM_TAIL_COOKIE_LEN + table->pad_size ;
           top_free_item_new = dsmi_augment_init_new_buffer(pool_id+1,
                                    item_array_start,aug_data->augment_cnt/aug_data->blocks_to_buffer_ratio,
                                    table->pool_item_size);
           ((dsm_item_type*)item_array_start)->app_ptr = top_free_item_old;
           top_free_item_old = top_free_item_new;
           j++;
         }
         if (allocated)
         {
             new_pool->block = (void*)&aug_data->block_list[k];
             new_pool->dsma_free_list.top_item = top_free_item_new;
             dsma_aug_bytes += dsm_common_pool_block_size *aug_data->blocks_to_buffer_ratio;

             top_free_item_old = top_free_item_new;
         }
  }
  else
  {
       buffer = dsma_alloc(size);
       if (buffer != NULL)
       {
         MSG_HIGH_DSM_CONFIG(" DSMA alloc_new_buffer id %d dbuffer_cnt %d dbuffer_limit %d",
            pool_id,aug_data->dbuffer_count,aug_data->dbuffer_limit);
         new_pool->block = buffer; /* The item array pointer is only used to delete */
         /* Align the item_array so that item data lands on a 32 byte boundary */
         /*

                       +---------------+---------------+---------------+---------------+
                       |T|  pad     | Header         |H|Data                           |T|
                       +---------------+---------------+---------------+---------------+

                       +     - indicate 32 byte boundaries.
                       T     - is the tail cookie from the previous buffer.
                       pad   - is padding so that things line up correctly.
                       H     - is the head cookie for this buffer
                       Data  - is the data for this buffer. data may be any multiple of
                         32 bytes.

        */

        item_array_start = (uint8*)(((uint32)buffer + 63 ) & ~63 );
        item_array_start += DSM_TAIL_COOKIE_LEN  + table->pad_size;

        /* This function should be similar to dsmi_init_pool_data, but optimized for run time
            initialization */
        new_pool->dsma_free_list.top_item =
            dsmi_augment_init_new_buffer(pool_id+1,
             item_array_start,aug_data->augment_cnt,
             table->pool_item_size);

        /* Assert below has to be true, but need not be checked at run time*/
        /* ASSERT((uint8 *)(buffer + size) <=
                 (uint8 *)((uint8*)(new_pool->dsma_free_list.top_item)
                + DSMI_POOL_OBJECT_SIZE_OPT(dsma_aug_pools[i].pool->pool_item_size)));
        */
        dsma_aug_bytes += size;
        allocated = TRUE;
      }
      else
      {
        dsma_update_statistics(DSM_EXTMEM_ALLOC_FAILED,table);
      }
  }
  if (dsma_aug_bytes)
  {
    dsm_dynamic_external_memory_register_high_usage_cb();
  }
  return allocated;
}

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
void dsm_augment_pools(void)
{
  uint32 i,heap_alloc;
  uint32 pool_id;
  dsm_augment_buffer_type* new_pool;
  dsm_augment_pool_data_type *aug_data;
  dsm_pool_mgmt_table_type *table;
  boolean allocated = FALSE;
  
  if ((dsma_aug_bytes_dynamic_pools) &&
          (dsma_aug_bytes > (DSMI_AUGMENT_MAX_ALLOC>>1) ||
           (dsma_aug_bytes > DSMI_AUGMENT_MAX_ALLOC-DSMI_DYNAMIC_EXT_MEM_ALLOC_BACK_OFF)))
  {
      /* Data transfer started peaking, trigger free of dynamic pools */
      dsm_dynamic_pools_trigger_free();
  }

  for (i = 0; i < dsma_augpool_cnt; i++)
  {
    if (dsma_aug_pools[i].dsm_task_alloc)
    {
      aug_data = &dsma_aug_pools[i];
      table = aug_data->pool;
      if (aug_data->reuse_buffer_cnt)
      {
        /* Try to reuse free buffer */
        dsma_alloc_dynamic_reuse_recycle_buffer(aug_data);
        if (aug_data->dynamic_top_free)
        {
          aug_data->dsm_task_alloc = FALSE;
          continue;
        }   
      } 
      if ((!aug_data->buffer_available_cnt) && (aug_data->dbuffer_count >= aug_data->dbuffer_limit))
      {
        dsma_update_statistics(DSM_POOL_CNT_LIMIT_REACHED,table);
        MSG_HIGH("DSMA: Cannot allocate from External Memory.DBuffer limit %d reached for pool %x",
         aug_data->dbuffer_limit,
         aug_data->pool,0);
        aug_data->dsm_task_alloc = FALSE;
        continue;
      }
      pool_id = dsma_get_buffer_head(aug_data,&new_pool);
      if (!new_pool)
      {
        aug_data->dsm_task_alloc = FALSE;
        continue;
      }
      if ((aug_data->buffer_available_cnt != 0) || (dsm_common_pool_block_count != 0))
      {
        allocated = dsma_allocate_from_common_pool(&dsma_aug_pools[i],new_pool,pool_id,(i+1));
      }
      if (!allocated)
      {
        dsm_get_external_memory_max_alloc();
        heap_alloc = dsma_check_ext_memory_limits();
        if (!heap_alloc)
        {
          aug_data->dsm_task_alloc = FALSE;
          dsma_update_statistics(DSM_EXTTMEM_LIMIT_REACHED,table);
          /* Cannot allocate from External memory, Upper limit reached */
          continue;
        }
        allocated = dsm_allocate_from_heap(i,new_pool,pool_id,heap_alloc);
      }
      if ( allocated )
      {
          new_pool->dsma_state = DSMA_ALLOC;
          aug_data->dynamic_top_free = new_pool->dsma_free_list.top_item;
          new_pool->free_list = 0;
          atomic_add(&table->free_count,aug_data->augment_cnt);
          table->item_count+=aug_data->augment_cnt;
          aug_data->dbuffer_count++;
          if(aug_data->dbuffer_count > 3)
          {
            aug_data->scan_buffer_count = DSMA_SCAN_BUFFER_CNT_NORMAL;
          }
          if (table->pool_stats)
          {
            if (table->pool_stats->stats_max_item_count < table->item_count)
            {
              table->pool_stats->stats_max_item_count = table->item_count;
            }
          }
          /* Collect statistics */
          if (aug_data->dbuffer_count >
              aug_data->max_dbuffer_count_interval)
          {
            aug_data->max_dbuffer_count_interval = aug_data->dbuffer_count;
          }

          if (aug_data->dbuffer_count >
               aug_data->stats_max_dbuffer_used)
          {
            aug_data->stats_max_dbuffer_used = aug_data->dbuffer_count;
          }
          dsma_update_statistics(DSM_POOL_ALLOC,table);
      }
      else
      {
        break; /* Memory is not available */
      }

      MSG_HIGH_DSM_CONFIG(" DSMA Augent alloc %d dynamic%d, recycle3 %d ",
                          DSMA_GET_TOP_FREE_BUFFER_INDEX(aug_data->alloc_top_free),
                  DSMA_GET_TOP_FREE_BUFFER_INDEX(aug_data->dynamic_top_free),
                  DSMA_GET_TOP_FREE_BUFFER_INDEX(aug_data->recycle_top_free));
  
      MSG_HIGH_DSM_CONFIG(" DSMA Augment pool %x item_cnt %d free_cnt %d",
                table,
                table->item_count,
                table->free_count);
                
      aug_data->dsm_task_alloc = FALSE;
    }
  } 
}

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
  Pool's item  count and free count are modified.
=========================================================================*/
boolean dsm_augment_free_buffer(void)
{
  uint32 i,j,k,total,size,heap_alloc;
  dsm_augment_buffer_type* free_buffer;
  dsm_augment_pool_data_type *aug_data;
  boolean lcompleted = TRUE;
  int32 delete_block_cnt;
  dsma_block_list_type* free_block;
  
  heap_alloc = 1;
  if (dsma_aug_bytes)
  {
    heap_alloc = dsma_check_ext_memory_limits();
  }
  
  for (i = 0; i < dsma_augpool_cnt; i++)
  {    
    if (!heap_alloc)
    {
      dsma_aug_pools[i].dsm_task_free = TRUE;
      dsma_switch_dynamic_pool_to_common_pool(&dsma_aug_pools[i]);
    }
    if(!dsma_aug_pools[i].dsm_task_free)
    {
      continue;
    }
    aug_data = &dsma_aug_pools[i];
    if ((!aug_data->dynamic_top_free) || (!aug_data->recycle_top_free))
    {
      /* Try to reuse freed buffers */
      dsma_alloc_dynamic_reuse_recycle_buffer(aug_data);
      MSG_HIGH_DSM_CONFIG(" DSMA allocated in free alloc %d dynamic %d, recycle %d ",
           DSMA_GET_TOP_FREE_BUFFER_INDEX(aug_data->alloc_top_free),
           DSMA_GET_TOP_FREE_BUFFER_INDEX(aug_data->dynamic_top_free),
           DSMA_GET_TOP_FREE_BUFFER_INDEX(aug_data->recycle_top_free));
    }

    /* Release only 1 buffer in the function */
    if(aug_data->reuse_trigger_cnt)
    {
      /* Keep only DSMI_AUGMENT_TOTAL_REUSE_BUFFERS reuse buffers circulating */
      delete_block_cnt =
       (aug_data->reuse_buffer_cnt <= DSMI_AUGMENT_TOTAL_REUSE_BUFFERS) ? \
       0:aug_data->reuse_buffer_cnt-DSMI_AUGMENT_TOTAL_REUSE_BUFFERS+1;
      if (!heap_alloc)
      {
        delete_block_cnt = aug_data->reuse_buffer_cnt;
      }
    }
    else
    {
      delete_block_cnt = aug_data->reuse_buffer_cnt;
      aug_data->max_dbuffer_count_interval = 0;
    }

    /* If not timed right, the value delete_block_cnt, could be negative */
    if( delete_block_cnt > 0)
    {    
      /*if(aug_data->pool->free_count > (aug_data->pool->item_count_static + 3*aug_data->augment_cnt))
      {
        ERR_FATAL("DSM Library Config Error 1 : Low item count in Pool 0x%x Contact DSM team",aug_data->pool,0,0);
      }*/
      j = DSMI_AUGMENT_TOTAL_BLOCKS-1;
      total = aug_data->dbuffer_count;
      while (total >0)
      {
        if (aug_data->buffer[j].dsma_state)
        {
          total--;
          if (aug_data->buffer[j].dsma_state == DSMA_REUSE)
          {
           free_buffer = &aug_data->buffer[j];
           delete_block_cnt--;
           if (atomic_compare_and_set(&free_buffer->dsma_state, DSMA_REUSE,DSMA_FREE))
           {
             /* The value could be modified in DSM library */
             MSG_HIGH_DSM_CONFIG("DSMA Freed buffer %d in pool 0x%x delete_buffer_cnt %d",j, aug_data->pool,delete_block_cnt);
             MSG_HIGH_DSM_CONFIG("DSMA Freed buffer pool free_cnt %d, dbuffer_cnt %d, reuse_buffer_cnt %d",
                                 aug_data->pool->free_count,
                                 aug_data->dbuffer_count,
                                 aug_data->reuse_buffer_cnt);
             aug_data->pool->item_count-=aug_data->augment_cnt;
             if ((uint32)free_buffer->block>8192)
             {
               /* from heap */
               size = DSMA_GET_AUGMENT_BUFFER_SIZE(aug_data->pool->pool_item_size,
                                                       aug_data->augment_cnt);
               if (size > dsm_common_pool_block_size)
               {
                 k = 1;
                 free_block = (dsma_block_list_type*)(*((uint32*)free_buffer->block));
                 while (k < aug_data->blocks_to_buffer_ratio)
                 {
                     dsma_free(free_block->block_array[k]);
                     k++;
                 }
                 dsma_free(free_block->block_array[0]);
                 *((uint32*)free_buffer->block)=NULL;
                 dsma_aug_bytes -= dsm_common_pool_block_size *aug_data->blocks_to_buffer_ratio;
                 /* update lower_limit_values */
                 dsma_check_ext_memory_limits();
               }
               else
               {
                 dsma_free(free_buffer->block);
                 dsma_aug_bytes -= size;
               }
               if (!dsma_aug_bytes)
               {
                 dsm_dynamic_external_memory_deregister_high_usage_cb();
               }
             }
             else
             {
               size = DSMI_POOL_OBJECT_SIZE_OPT(aug_data->pool->pool_item_size)* aug_data->augment_cnt;
               dsma_free_to_common_pool(aug_data,size,j,(uint32)free_buffer->block,(void*)free_buffer->dsma_free_list.top_item);
             }
             memset(free_buffer,0, sizeof(dsm_augment_buffer_type));
             aug_data->dbuffer_count --;
             if(aug_data->dbuffer_count < 4)
             {
               aug_data->scan_buffer_count = aug_data->augment_cnt;
             }
             atomic_dec(&aug_data->reuse_buffer_cnt);
             dsma_update_statistics(DSM_POOL_FREE,aug_data->pool);
             break;
           }
           
         }
        }
        j--;
      }      
    }
    if (delete_block_cnt > 0)
    {
      /* Keep it running in a loop based on timer */
      lcompleted  = FALSE;
    }
    else
    {
      lcompleted  = TRUE;
      aug_data->dsm_task_free = FALSE;
    }
  }
  return lcompleted;
}

#endif /* FEATURE_DSM_AUGMENT */



/*========================================================================
FUNCTION dsmi_dynamic_pool_init()

DESCRIPTION
 Initialize DSM dynamic pool.

DEPENDENCIES
 Pool is initialized.

PARAMETERS
  table - DSM pool to augment

RETURN VALUE
  None

SIDE EFFECTS
  This function is NOT threadsafe.  Initializes static augment buffers.
=========================================================================*/
void dsmi_dynamic_pool_init(dsm_pool_mgmt_table_type* table)
{
  dsm_dynamic_pool_data_type *dynamic_pool_data;

  ASSERT(dsma_dynamic_pool_cnt < DSMI_MAX_DYN_POOLS)

  dynamic_pool_data = &dsma_dyn_pools[dsma_dynamic_pool_cnt];

  memset(dynamic_pool_data,0,
            sizeof(dsm_dynamic_pool_data_type));

  dynamic_pool_data->pool = table;
  table->dynamic_pool = (void*)dynamic_pool_data;  
  dynamic_pool_data->size = DSMA_GET_AUGMENT_BUFFER_SIZE(table->pool_item_size,
                          table->item_count_static);
  if (dynamic_pool_data->size < DSM_DYNAMIC_MAX_SIZE)
  {
    dynamic_pool_data->count = table->item_count_static;
  }
  else
  {
    dynamic_pool_data->count = DSM_DYNAMIC_MAX_SIZE/DSMI_POOL_OBJECT_SIZE_OPT(table->pool_item_size);
  }
  ASSERT(DSMI_MAX_DYNAMIC_BUFFERS >= (table->item_count_static/dynamic_pool_data->count)+1) ;

  dsma_dynamic_pool_cnt++;
}

/*========================================================================
FUNCTION DSM_DYNAMIC_POOLS_TRIGGER_FREE()

DESCRIPTION
    This API is called by other libraries to trigger freeing of
    dynamic memory

DEPENDENCIES

PARAMETERS

RETURN VALUE

SIDE EFFECTS
=========================================================================*/
void dsm_dynamic_pools_trigger_free(void)
{
  uint32 i;
  dsm_pool_mgmt_table_type* pool;
  boolean to_be_released = FALSE;

  for (i =0; i <dsma_dynamic_pool_cnt; i++)
  {
    if (dsma_dyn_pools[i].item_array[0]!=NULL)
    {
      pool = dsma_dyn_pools[i].pool;
      dsma_dyn_pools[i].release_state = TRUE;
      to_be_released = TRUE;
      if (dsma_dyn_pools[i].released_items != pool->item_count )
      {
        if(dsma_dyn_pools[i].trigger_free_cb)
        {
          /* The call back function should set a signal to clients' task and return */
          dsma_dyn_pools[i].trigger_free_cb((dsm_mempool_id_type)pool);
        }
      }
    }
  }
  if(to_be_released)/* Set signal to DSM task and return */
  {
    dsm_task_dynamic_free_req();
  }
}
/*========================================================================
FUNCTION DSM_DYNAMIC_EXTERNAL_MEMORY_HIGH_USAGE_CB()

DESCRIPTION
    This API is called by other libraries to trigger freeing of
    dynamic memory

DEPENDENCIES

PARAMETERS

RETURN VALUE

SIDE EFFECTS
=========================================================================*/
void dsm_dynamic_external_memory_high_usage_cb(uint32 usage)
{
   if (DSM_UNLIKELY(usage > dsma_ext_memory_upper_limit))
   {
      /* Deeper memory usage level */
      if (!dsma_ext_memory_check_lower_limit)
      {
        dsma_ext_memory_check_lower_limit =
                     usage - (DSMI_AUGMENT_MAX_ALLOC>>1);
#ifdef FEATURE_DSM_AUGMENT
        if(dsma_aug_bytes-dsma_aug_bytes_dynamic_pools !=0)
        {
          dsm_task_augment_free_req();
        }
#endif
        dsma_ext_memory_dynamic_alloc_check_lower_limit = 0;
      }
   }

   if ((dsma_aug_bytes_dynamic_pools) && (!dsma_ext_memory_dynamic_alloc_check_lower_limit))
   {
     dsma_ext_memory_dynamic_alloc_check_lower_limit =
                  dsma_ext_memory_alloc_upper_limit- DSMI_DYNAMIC_EXT_MEM_ALLOC_BACK_OFF;
     dsm_dynamic_pools_trigger_free();
   }
}
/*========================================================================
FUNCTION DSM_DYNAMIC_EXTERNAL_MEMORY_REGISTER_HIGH_USAGE_CB()

DESCRIPTION
    This API is called to register external memory high usage cb function

DEPENDENCIES

PARAMETERS

RETURN VALUE

SIDE EFFECTS
=========================================================================*/
void dsm_dynamic_external_memory_register_high_usage_cb(void)
{ /* Consider fragmentation and other usage */

  if(dsma_aug_bytes_dynamic_pools)
  {
    dsma_register_high_usage_callback(&dsm_dynamic_external_memory_high_usage_cb,
       dsma_ext_memory_alloc_upper_limit-DSMI_DYNAMIC_EXT_MEM_ALLOC_BACK_OFF);
  }
  else if (dsma_aug_bytes)
  {
      dsma_register_high_usage_callback(&dsm_dynamic_external_memory_high_usage_cb,
             dsma_ext_memory_upper_limit);
  }
}
/*========================================================================
FUNCTION DSM_DYNAMIC_EXTERNAL_MEMORY_DEREGISTER_HIGH_USAGE_CB()

DESCRIPTION
    This API is called to deregister external memory high usage cb function

DEPENDENCIES

PARAMETERS

RETURN VALUE

SIDE EFFECTS
=========================================================================*/
void dsm_dynamic_external_memory_deregister_high_usage_cb(void)
{ /* Consider fragmentation and other usage */
    /* update lower_limit_values */
    dsma_check_ext_memory_limits();
    dsma_deregister_high_usage_callback();
}

/*========================================================================
FUNCTION DSM_AUGMENT_ALLOC_DYNAMIC_POOL_MEMORY()

DESCRIPTION
 Allocate memory for Dynamic pool from external memory/heap

DEPENDENCIES

PARAMETERS

RETURN VALUE  TRUE , if memory is allocated

SIDE EFFECTS
=========================================================================*/
boolean dsm_augment_alloc_dynamic_pool_memory(dsm_pool_mgmt_table_type* table)
{

  uint32 heap_alloc;
  uint8* item_array_start;
  dsm_dynamic_pool_data_type *dynamic_pool;
  boolean allocated = FALSE;
  boolean alloc = TRUE;
  uint32 count,size;
  int i,j;
#ifdef FEATURE_DSM_LOCKLESS
  dsm_item_type* top_free_item_old = NULL;
  dsm_item_type* top_free_item_new = NULL;
#else
  uint32 total_item=0;
#endif

  dsm_get_external_memory_max_alloc();
  heap_alloc = dsma_check_ext_memory_limits();
  dynamic_pool = (dsm_dynamic_pool_data_type*)table->dynamic_pool;
  if (!heap_alloc)
  {
     return allocated;
  }

  DSMI_POOL_LOCK(table);

  if (table->item_count !=0)
  {
      /* Already allocated */
   DSMI_POOL_UNLOCK(table);
   return allocated;
  }

  if (((dsma_aug_bytes - dsma_aug_bytes_dynamic_pools) !=0 ) &&
      ((dsma_aug_bytes > (DSMI_AUGMENT_MAX_ALLOC>>1)) ||
      (dsma_aug_bytes > (DSMI_AUGMENT_MAX_ALLOC-DSMI_DYNAMIC_EXT_MEM_ALLOC_BACK_OFF))))
  {
      /* Data transfer started peaking, trigger free of dynamic pools */
	  dsm_dynamic_pools_trigger_free();
      DSMI_POOL_UNLOCK(table);
      return allocated;
  }

  /* Couple more checks for dynamic pools */
  if ((dsma_aug_bytes + dynamic_pool->size > (DSMI_AUGMENT_MAX_ALLOC))||
     (heap_alloc + dynamic_pool->size + DSMI_DYNAMIC_EXT_MEM_ALLOC_BACK_OFF > dsma_ext_memory_alloc_upper_limit))
  {
     MSG_HIGH("DSMA Dynamic: Exceeded total bytes that can be allocated from ext memory pool %x size %d upper limit %d",
             table,dsma_aug_bytes + dynamic_pool->size,dsma_ext_memory_alloc_upper_limit);

     if (dsma_aug_bytes + dynamic_pool->size > DSMI_AUGMENT_MAX_ALLOC)
     {
       dsma_update_statistics(DSM_MAXALLOC_LIMIT_REACHED,table);
     }
     else
     {
       dsma_update_statistics(DSM_EXTTMEM_LIMIT_REACHED,table);
     }
     DSMI_POOL_UNLOCK(table);
     return allocated;
  }
  dynamic_pool = (dsm_dynamic_pool_data_type*)table->dynamic_pool;

  i=table->item_count_static;
  j = 0;
  while ((alloc) && (i>0) && (j < DSMI_MAX_DYNAMIC_BUFFERS))
  {
    if (i >dynamic_pool->count)
    {
      size = DSMA_GET_AUGMENT_BUFFER_SIZE(table->pool_item_size,dynamic_pool->count);
      dynamic_pool->item_array[j] = dsma_alloc(size + (64*(dynamic_pool->instance_id & 0x1)));
    }
    else
    {
      size = DSMA_GET_AUGMENT_BUFFER_SIZE(table->pool_item_size,i);
#ifndef FEATURE_DSM_LOCKLESS
      size+= DSMA_GET_TABLE_FREE_STACK(table->item_count_static);
#endif
      dynamic_pool->item_array[j] = dsma_alloc(size + (64*(dynamic_pool->instance_id & 0x1)));
      if (!dynamic_pool->item_array[j])
      {
         alloc = FALSE;
      }
      break;
    }
    if (!dynamic_pool->item_array[j])
    {
      alloc = FALSE;
      break;
    }
    else
    {
      i-=dynamic_pool->count;
      if (i < 1)
      {
        break;
      }
    }
    j++;
  }


  if ((alloc)&& (j < DSMI_MAX_DYNAMIC_BUFFERS))
  {
    MSG_HIGH_DSM_CONFIG(" DSMA DYNAMIC alloc_new_buffer pool id %d pool_size %d count %d",
      (uint32)table,table->pool_item_size,table->item_count_static);
    /* Align the item_array so that item data lands on a 32 byte boundary */
    /*

                       +---------------+---------------+---------------+---------------+
                       |T|  pad     | Header         |H|Data                           |T|
                       +---------------+---------------+---------------+---------------+

                       +     - indicate 32 byte boundaries.
                       T     - is the tail cookie from the previous buffer.
                       pad   - is padding so that things line up correctly.
                       H     - is the head cookie for this buffer
                       Data  - is the data for this buffer. data may be any multiple of
                         32 bytes.
    */
    count = i;
    
    while (j >=0)
    {
      item_array_start = dynamic_pool->item_array[j];      
      item_array_start = (uint8*)(((uint32)item_array_start + 63 ) & ~63 );
      item_array_start += DSM_TAIL_COOKIE_LEN  + table->pad_size;

#ifdef FEATURE_DSM_LOCKLESS
      top_free_item_new = dsmi_augment_init_new_buffer(0,
              item_array_start,count,
              table->pool_item_size);
      ((dsm_item_type*)item_array_start)->app_ptr = top_free_item_old;
      top_free_item_old = top_free_item_new;
#else
      if (!table->free_stack)
      {
        table->free_stack = (dsm_item_type**)(item_array_start +
                      (DSMI_POOL_OBJECT_SIZE_OPT( table->pool_item_size)*count));
      }
      dsmi_augment_init_new_buffer(0,
                                         item_array_start,count,
                                         table->pool_item_size,
                                         (dsm_item_type**)(((uint32)(table->free_stack))+total_item*4));
      total_item+=count;
#endif
      j--;
      count=dynamic_pool->count;
    }
#ifdef FEATURE_DSM_LOCKLESS
    table->top_free_item=top_free_item_new;
#endif
    table->item_count=table->item_count_static;
    table->free_count=table->item_count;
    dsma_aug_bytes_dynamic_pools+=dynamic_pool->size;
    dsma_aug_bytes+=dynamic_pool->size;
    dsm_dynamic_external_memory_register_high_usage_cb();
    allocated = TRUE;
  }
  else
  {
    for (i = 0; i< DSMI_MAX_DYNAMIC_BUFFERS;i++)
    {
        if (dynamic_pool->item_array[i])
        {
            dsma_free(dynamic_pool->item_array[i]);
            dynamic_pool->item_array[i] = NULL;
        }
    }
    dsma_update_statistics(DSM_EXTMEM_ALLOC_FAILED,table);
    MSG_HIGH("DSMA Dynamic: Could not allocate from Heap %d, size %d ",table,dynamic_pool->size,0);
  }
  DSMI_POOL_UNLOCK(table);
  return allocated;
}

/*========================================================================
FUNCTION DSM_AUGMENT_DYNAMIC_POOL_FREE_BUFFER()

DESCRIPTION
    This API is called to return memory of dynamic pools

DEPENDENCIES

PARAMETERS

RETURN VALUE

SIDE EFFECTS
  This function is called from a single thread and is NOT ThreadSafe.
=========================================================================*/
boolean dsm_augment_dynamic_pool_free_buffer()
{
    dsm_pool_mgmt_table_type* table;
    uint32 i,j;
    boolean ret = TRUE;

    for (i =0; i <dsma_dynamic_pool_cnt; i++)
    {
      if (dsma_dyn_pools[i].release_state)
      {
        table= dsma_dyn_pools[i].pool;
        if (table->item_count == 0)
        {
        	break;
        }
        if (dsma_dyn_pools[i].released_items!=table->item_count)
        {
            ret = dsmi_freeze_pool_allocation(table,FALSE);
        }
        if (ret)
        {
          dsma_dyn_pools[i].instance_id++;
          for (j = 0; j< DSMI_MAX_DYNAMIC_BUFFERS;j++)
          {
            if (dsma_dyn_pools[i].item_array[j])
            {
                dsma_free(dsma_dyn_pools[i].item_array[j]);
                dsma_dyn_pools[i].item_array[j] = NULL;
            }
          }
          dsma_dyn_pools[i].item_array[0] = 0;
          dsma_dyn_pools[i].release_state = FALSE;
          dsma_dyn_pools[i].released_items = 0;
          table->free_count = 0;
#ifdef FEATURE_DSM_LOCKLESS
          table->top_free_item= NULL;
          table->alloc_count = 0;
#else
          table->free_stack = NULL;
#endif
          table->item_count = 0;
          if (dsma_dyn_pools[i].instance_id >250)
          {
              dsma_dyn_pools[i].instance_id= 0;
          }
          dsma_aug_bytes_dynamic_pools-= dsma_dyn_pools[i].size;
          dsma_aug_bytes-=dsma_dyn_pools[i].size;
          MSG_HIGH_DSM_CONFIG("DSMA Dynamic Freed memory in pool 0x%x  aug_bytes_dynamic %d aug_bytes %d",table, dsma_aug_bytes_dynamic_pools,dsma_aug_bytes);
          if (!dsma_aug_bytes)
          {
            dsm_dynamic_external_memory_deregister_high_usage_cb();
          }
        }
      }
    }
    return ret;
    /* This will return a memory aligned to 32 bytes */
}
/*========================================================================
 *  Functions invoked from HIGH PRIO DSM Task AND DSM_INIT()
 *  Deals only with dynamic buffers
 ========================================================================= */

/*========================================================================
FUNCTION dsma_update_statistics()

DESCRIPTION
  Update statistics entries for major external memory allocation events

DEPENDENCIES

PARAMETERS
  table - Pool
  event  - Event that happened

RETURN VALUE
  None

SIDE EFFECTS
  None. Can be commented out if it affects performance.
=========================================================================*/
static void dsma_update_statistics(
        dsm_mem_alloc_event_type event,
         dsm_pool_mgmt_table_type *table)
{
#ifdef FEATURE_DSM_MEM_CHK
    dsm_stats_entry_type* entry;
#ifdef FEATURE_DSM_LOCKLESS
    dsm_augment_pool_data_type *aug_data = (dsm_augment_pool_data_type *)table->augment_data;
#endif
    entry = &(dsm_event_stats.dsm_stats_events_buffer[dsm_event_stats.stats_events_next_entry]);
    dsm_event_stats.stats_extmem_limit_reached_cnt++;
    switch (event) {
      case DSM_MAXALLOC_LIMIT_REACHED:
        dsm_event_stats.stats_dsm_maxalloc_reached_cnt++;
        if (table->pool_stats)
        {
          table->pool_stats->stats_ext_mem_alloc_fail_count++;
        }
        break;
      case DSM_EXTTMEM_LIMIT_REACHED:
        dsm_event_stats.stats_extmem_limit_reached_cnt++;
        break;
      case DSM_EXTMEM_ALLOC_FAILED:
        dsm_event_stats.stats_extmem_alloc_failed_cnt++;
        if (table->pool_stats)
        {
          table->pool_stats->stats_ext_mem_alloc_fail_count++;
        }
        break;
     default:;
    }

    entry->mem_alloc_event = event;
    entry->ext_mem_dsm_allocated_bytes = dsma_aug_bytes;
    entry->ext_mem_current_used_bytes = dsm_get_external_memory_usage();

#ifdef FEATURE_DSM_LOCKLESS
    if (aug_data !=NULL)
    {

      /* Fill only entries of the current pool */
      entry->pool_stats.pool_id = (dsm_mempool_id_type)aug_data->pool;
      entry->pool_stats.pool_item_cnt = aug_data->pool->item_count;
      entry->pool_stats.pool_free_cnt = aug_data->pool->free_count;
      entry->pool_stats.pool_alloc_cnt = aug_data->pool->alloc_count;
    }
#endif
    dsm_event_stats.stats_events_next_entry++;
    if (dsm_event_stats.stats_events_next_entry >= DSM_EVENTS_BUFFER_CNT)
    {
      dsm_event_stats.stats_events_next_entry = 0;
    }
#endif /* FEATURE_DSM_MEM_CHK */
}
